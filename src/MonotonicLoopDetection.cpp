#include "llvm/Pass.h"
#include "llvm/Analysis/LoopPass.h"

#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instruction.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Support/Casting.h"
#include "llvm/IR/Constants.h"

#include <iostream> //debug
#include <vector>
//#include <utility>

#include <llvm/IR/Argument.h>
#include <llvm/IR/TypeBuilder.h>
#include <llvm/IR/Attributes.h>

#include <llvm/IR/IRBuilder.h>

namespace{

	struct Node
	{
		llvm::Value* V;
		Node* father;
		std::vector<Node*> son;
		Node(): V(NULL), father(NULL){}
	};

	void del(Node* n)
	{
		if(n==NULL) return;
		else {
			for(auto s : n->son) del(s);
			delete n;
		}
	}

	bool validate(Node* n)
	{
		bool ret = true;
		if(n->V==NULL) return false;

		if(llvm::dyn_cast<llvm::ConstantInt>(n->V)) return true;
		else if(llvm::dyn_cast<llvm::Argument>(n->V))
		{
			std::cerr << "VALIDATION ERROR: value using function argument" << std::endl;
			return false;
		}
		else if(llvm::dyn_cast<llvm::CallInst>(n->V))
		{
			std::cerr << "VALIDATION ERROR: value using function output" << std::endl;
			return false;
		}


		if(n->son.size()==0)
		{
			if(llvm::Instruction* I = llvm::dyn_cast<llvm::Instruction> (n->V))
			{
				for(unsigned int i=0; i<I->getNumOperands(); i++)
				{
					if(llvm::dyn_cast<llvm::Argument>(I->getOperand(i)))
					{
						std::cerr << "VALIDATION ERROR: value using function argument" << std::endl;
						return false;
					}
					else if(llvm::dyn_cast<llvm::CallInst>(I->getOperand(i)))
					{
						std::cerr << "VALIDATION ERROR: value using function output" << std::endl;
						return false;
					}
					else if(llvm::dyn_cast<llvm::ConstantInt>(I->getOperand(i))) continue;
					else if(llvm::Instruction* sI = llvm::dyn_cast<llvm::Instruction>(I->getOperand(i)))
					{
						Node* sN = new Node();
						sN->father = n;
						sN->V = &*sI;
						n->son.push_back(sN);
					}
				}
			}
		}

		for(auto s : n->son)
		{
			if((n->father!=NULL)&&(s->V == n->father->V))
			{
				continue;
			}
			else ret = ret && validate(s);
		}

		return ret;
	}

	bool search(Node* n, llvm::Instruction* fI)
	{
		bool ret = false;
		if(fI==n->V){
			return true;
		}
		else if(n->son.size()==0){
			llvm::Instruction* I = llvm::dyn_cast<llvm::Instruction>(n->V);
			for(unsigned int i=0; i<I->getNumOperands(); i++)
			{
				if(llvm::Instruction* sI = llvm::dyn_cast<llvm::Instruction>(I->getOperand(i))){
					Node* sN = new Node();
					sN->father = n;
					sN->V = &*sI;
					n->son.push_back(sN);
				}
			}
		}
		for(auto s : n->son)
		{
			if((n->father!=NULL)&&(s->V == n->father->V)){
				continue;
			}
			else ret = ret || search(s,fI);
		}

		return ret;
	}


	llvm::Value* getMax(llvm::Instruction* phi, llvm::Instruction* cmp)
	{
		if((cmp && phi)&&(llvm::dyn_cast<llvm::ICmpInst>(cmp)))
		{
			Node* n = new Node();
			n->V = cmp->getOperand(0);
			if(cmp->getOperand(0)==phi) return cmp->getOperand(1);
			else if(cmp->getOperand(1)==phi) return cmp->getOperand(0);
		}
		return NULL;
	}

	llvm::Value* getMin(llvm::Instruction* phi)
	{
		if(llvm::dyn_cast<llvm::PHINode>(phi))
		{
			return phi->getOperand(0);
		}
		return NULL;
	}


	llvm::Function* exit_prototype(llvm::Module* M)
	{
		llvm::LLVMContext& Ctx = M->getContext();
		llvm::Constant* c = M->getOrInsertFunction("exit", llvm::Type::getVoidTy(Ctx), llvm::Type::getInt32Ty(Ctx), NULL);
		llvm::Function* exit_f = llvm::cast<llvm::Function>(c);
		return exit_f;
	}

	llvm::Function* createMax(llvm::Module* M, llvm::Function* exit_f)
	{
		llvm::LLVMContext& Ctx = M->getContext();
		llvm::Constant* c = M->getOrInsertFunction("__check_array_max", llvm::Type::getInt1Ty(Ctx), llvm::Type::getInt32Ty(Ctx), llvm::Type::getInt32Ty(Ctx), NULL);
		llvm::Function* max_f = llvm::cast<llvm::Function>(c);

		llvm::Function::arg_iterator args = max_f->arg_begin();
		llvm::Value* idx = &*args++;
		idx->setName("idx");
		llvm::Value* mx = &*args++;
		mx->setName("mx");

		llvm::BasicBlock* entry = llvm::BasicBlock::Create(llvm::getGlobalContext(), "entry", max_f);
		llvm::BasicBlock* ret = llvm::BasicBlock::Create(llvm::getGlobalContext(), "return", max_f);
		llvm::BasicBlock* cond_false = llvm::BasicBlock::Create(llvm::getGlobalContext(), "cond_false", max_f);

		llvm::IRBuilder<> builder(entry);

		llvm::Value* less = builder.CreateICmpSLT(idx, mx, "tmp");

		builder.CreateCondBr(less, ret, cond_false);
		builder.SetInsertPoint(ret);
		builder.CreateRet(less);
		builder.SetInsertPoint(cond_false);

		llvm::Value* errval = llvm::ConstantInt::get(llvm::Type::getInt32Ty(M->getContext()),-1);

		builder.CreateCall(exit_f,errval);
		builder.CreateRet(less);

		return max_f;
	}


	llvm::Function* createMin(llvm::Module* M, llvm::Function* exit_f)
	{
		llvm::LLVMContext& Ctx = M->getContext();
		llvm::Constant* c = M->getOrInsertFunction("__check_array_min", llvm::Type::getInt1Ty(Ctx), llvm::Type::getInt32Ty(Ctx), llvm::Type::getInt32Ty(Ctx), NULL);
		llvm::Function* min_f = llvm::cast<llvm::Function>(c);

		llvm::Function::arg_iterator args = min_f->arg_begin();
		llvm::Value* idx = &*args++;
		idx->setName("idx");
		llvm::Value* mn = &*args++;
		mn->setName("mn");

		llvm::BasicBlock* entry = llvm::BasicBlock::Create(llvm::getGlobalContext(), "entry", min_f);
		llvm::BasicBlock* ret = llvm::BasicBlock::Create(llvm::getGlobalContext(), "return", min_f);
		llvm::BasicBlock* cond_false = llvm::BasicBlock::Create(llvm::getGlobalContext(), "cond_false", min_f);

		llvm::IRBuilder<> builder(entry);

		llvm::Value* greater = builder.CreateICmpSGE(idx, mn, "tmp");

		builder.CreateCondBr(greater, ret, cond_false);
		builder.SetInsertPoint(ret);
		builder.CreateRet(greater);
		builder.SetInsertPoint(cond_false);

		llvm::Value* errval = llvm::ConstantInt::get(llvm::Type::getInt32Ty(M->getContext()),-1);

		builder.CreateCall(exit_f,errval);
		builder.CreateRet(greater);

		return min_f;
	}

	std::pair<llvm::Function*,llvm::Function*> p;

	void checkArrayPrototype(llvm::Module* M, llvm::Function* exit_f)
	{
		p.first = createMin(M,exit_f);
		p.second = createMax(M,exit_f);
	}

	void createCheckArrayBounds(llvm::Value* min, llvm::Value* max, llvm::GetElementPtrInst* ptr)
	{
		if(llvm::AllocaInst* arr = llvm::dyn_cast<llvm::AllocaInst>(ptr->getOperand(0)))
		{
			llvm::PointerType* _p = arr->getType();
			llvm::ArrayType* a = llvm::dyn_cast<llvm::ArrayType>(_p->getElementType());
			//llvm::Value* size = llvm::ConstantInt::get(llvm::Type::getInt32Ty(llvm::getGlobalContext()),a->getNumElements());
			//llvm::Value* zero = llvm::ConstantInt::get(llvm::Type::getInt32Ty(llvm::getGlobalContext()),0);

//			llvm::Module* m = ptr->getParent()->getParent()->getParent();

			if(1)
			{
				llvm::IRBuilder<> builder(ptr);
				llvm::Value* v = NULL;
				if(llvm::SExtInst* se = llvm::dyn_cast<llvm::SExtInst>(ptr->getOperand(2)))
				{
					v = se->getOperand(0);
				}

				if(min)
				{
					llvm::Value* args[] = {v,builder.getInt32(0)};
					builder.CreateCall(p.first,args);
				}
			}

			if(1)
			{
				llvm::IRBuilder<> builder(ptr);
				llvm::Value* v = NULL;
				if(llvm::SExtInst* se = llvm::dyn_cast<llvm::SExtInst>(ptr->getOperand(2)))
				{
					v = se->getOperand(0);
				}

				if(max)
				{
					llvm::Value* args[] = {v,builder.getInt32(a->getNumElements())};
					builder.CreateCall(p.second,args);
				}

			}

		}
	}

	bool isMonotonic(llvm::Instruction* phi, llvm::Value* min, llvm::Value* max)
	{
		if(llvm::Instruction* inc = llvm::dyn_cast<llvm::Instruction>(phi->getOperand(1)))
		{
			Node* n;
			if((!(inc->getOperand(0)==phi))&&(!(inc->getOperand(0)==phi)))
			{
//				std::cerr << "Increment variable don't is related to loop variable" << std::endl;
				return (min || max);
			}
			if(!(inc->getOperand(0)==phi))
			{
				n = new Node();
				n->V = inc->getOperand(0);
				if(!validate(n))
				{
//					std::cerr << "NOT MONOTONIC: loop increment uses variable with unknown value" << std::endl;
					del(n);
					return false;
				}
				del(n);
			}
			if(!(inc->getOperand(1)==phi))
			{
				n = new Node();
				n->V = inc->getOperand(1);
				if(!validate(n))
				{
//					std::cerr << "NOT MONOTONUC: loop increment uses variable with unknow value" << std::endl;
					del(n);
					return false;
				}
				del(n);
			}
		}else{
//			std::cerr << "NOT MONOTONIC: increment not found" << std::endl;
			return false;
		}
		return (min || max);
	}

	std::vector<llvm::GetElementPtrInst*> getArrays(llvm::Loop* L)
	{
		std::vector<llvm::GetElementPtrInst*> vec;
		for(llvm::BasicBlock* bb : L->getBlocks())
		{
			for(llvm::Instruction& I : *bb)
			{
				if (auto* op = llvm::dyn_cast<llvm::GetElementPtrInst>(&I))
				{
					vec.push_back(op);
				}
			}
		}
		return vec;
	}

	std::vector<llvm::Instruction*> getLoadStore(llvm::Loop* L, llvm::GetElementPtrInst* ptr)
	{
		std::vector<llvm::Instruction*> vec;
		for(llvm::BasicBlock* bb : L->getBlocks())
		{
			for(llvm::Instruction& I : *bb)
			{
				if (auto* op = llvm::dyn_cast<llvm::StoreInst>(&I))
				{
					if(op->getPointerOperand()==ptr) vec.push_back(op);
				}
				else if (auto* op = llvm::dyn_cast<llvm::LoadInst>(&I))
				{
					if(op->getOperand(0)==ptr) vec.push_back(op);
				}
			}
		}
		return vec;
	}

	bool checkFunctionCreated = false;

	struct MLD: llvm::LoopPass
	{
		static char ID;

		MLD(): llvm::LoopPass(ID)
		{}

		virtual bool runOnLoop(llvm::Loop* L, llvm::LPPassManager &LPM)
		{

			if(!checkFunctionCreated)
			{
				llvm::Function* exit_f = exit_prototype(L->getHeader()->getModule());
				checkArrayPrototype(L->getHeader()->getModule(),exit_f);
				checkFunctionCreated = true;
			}


//			std::cerr << "-------------" << std::endl;
			llvm::Instruction* phi = NULL;
			llvm::Instruction* condition = NULL;

			for(llvm::Instruction& I : *L->getHeader()){
				if(llvm::dyn_cast<llvm::ICmpInst>(&I)) condition = &I;
				else if(I.getOpcode()==llvm::Instruction::PHI)
				{
					for(llvm::Instruction& i : *L->getLoopLatch())
					{
						if(&i==I.getOperand(1))	phi = &I;
					}
				}
			}

			if(phi==NULL)
			{
//				std::cerr << "Phi not found" << std::endl;
				return false;
			}

			llvm::Value* min = getMin(phi);
/*
			if(min)
			{
				std::cerr << "Min: ";
				min->dump();
			}
*/

			llvm::Value* max = getMax(phi,condition);
/*
			if(max)
			{
				std::cerr << "Max: ";
				max->dump();
			}
*/
			if(isMonotonic(phi,min,max))
			{
//				std::cerr << "Is Monotonic" << std::endl;
				for(llvm::GetElementPtrInst* idx : getArrays(L))
				{
//					idx->dump();
					Node* n = new Node();
					n->V = &*idx;
					llvm::LLVMContext& C = idx->getContext();
					llvm::MDNode* N = llvm::MDNode::get(C, llvm::MDString::get(C, "monotonic.loop"));
					if(search(n,phi))
					{
//						std::cerr << "Create OOB check" << std::endl;
						createCheckArrayBounds(min,max,idx);
						idx->setMetadata("SAFE",N);
						for(llvm::Instruction* ls : getLoadStore(L,idx)) ls->setMetadata("SAFE",N);
					}
					del(n);
				}
			}
			return true;
		}
	};
}

char MLD::ID=0;

static void register_MLD_Pass(const llvm::PassManagerBuilder &, llvm::legacy::PassManagerBase &PM) {
	PM.add(new MLD());
}

static llvm::RegisterStandardPasses RegisterMyPass(llvm::PassManagerBuilder::EP_EarlyAsPossible, register_MLD_Pass);
