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
			for(s : n->son) del(s);
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

		for(s : n->son)
		{
			if((n->father!=NULL)&&(s->V == n->father->V))
			{
//				std::cerr << "ALERT: value Circular reference" << std::endl;
				continue;
			}
			else ret = ret && validate(s);
		}

		return ret;
	}

	bool search(Node* n, llvm::Instruction* fI)
	{
		bool ret = true;
		if(fI==n->V){
			return true;
		}
		else if(n->son.size()==0){
			llvm::Instruction* I = llvm::dyn_cast<llvm::Instruction>(n->V);
			for(unsigned int i=0; i<I->getNumOperands(); i++)
			{
				if(llvm::dyn_cast<llvm::Argument>(I->getOperand(i))){
					std::cerr << "SEARCH ERROR: using function argument" << std::endl;
					return false;
				}
				else if(llvm::dyn_cast<llvm::CallInst>(I->getOperand(i))){
					std::cerr << "SEARCH: using function output" << std::endl;
					return false;
				}
				else if(llvm::dyn_cast<llvm::ConstantInt>(I->getOperand(i))) continue;
				else if(llvm::Instruction* sI = llvm::dyn_cast<llvm::Instruction>(I->getOperand(i))){
					Node* sN = new Node();
					sN->father = n;
					sN->V = &*sI;
					n->son.push_back(sN);
				}
			}
		}
		for(s : n->son)
		{
			if((n->father!=NULL)&&(s->V == n->father->V)){
//				std::cerr << "ALERT: Circular reference" << std::endl;
				continue;
			}
			else ret = ret && search(s,fI);
		}

		return ret;
	}


	struct MLD: llvm::LoopPass
	{
		static char ID;

		MLD(): llvm::LoopPass(ID)
		{}

		virtual bool runOnLoop(llvm::Loop* L, llvm::LPPassManager &LPM)
		{
			std::cerr << "#--------------#" << std::endl;

			llvm::Instruction* phi = NULL;
			llvm::Instruction* condition = NULL;

			//get phi and cmp instruction
			for(llvm::Instruction& I : *L->getHeader()){
				if(llvm::dyn_cast<llvm::ICmpInst>(&I))
				{
					condition = &I;
				}

				if(I.getOpcode()==llvm::Instruction::PHI)
				{
					for(llvm::Instruction& i : *L->getLoopLatch())
					{
						if(&i==I.getOperand(1))	phi = &I;
					}
				}
			}

			if(!condition)
			{
				std::cerr << "NOT MONOTONIC: Can't detect loop's limits, condition not found" << std::endl;
				return false;
			}
			else
			{
				Node* n = new Node();
				n->V = &*condition;
				if(!validate(n))
				{
					std::cerr << "NOT MONOTONIC: Loop have unsafe value" << std::endl;
					return false;
				}
				del(n);
			}

			llvm::ConstantInt* start = NULL;
			llvm::ConstantInt* end = NULL;

			if(!(start = llvm::dyn_cast<llvm::ConstantInt>(phi->getOperand(0))))
			{
				std::cerr << "NOT MONOTONIC: Start variable is not constant" << std::endl;
				return false;
			}

			for(unsigned int i = 0; i < condition->getNumOperands(); i++)
			{
				if(end = llvm::dyn_cast<llvm::ConstantInt>(condition->getOperand(i))) break;
			}
			if(end == NULL){
				std::cerr << "NOT MONOTONIC: End variable is not constant" << std::endl;
				return false;
			}

			bool changed = false;

			std::vector<llvm::BasicBlock *> v = L->getBlocks();
			for(llvm::BasicBlock* bb : v)
			{
				for(llvm::Instruction& I : *bb)
				{
					if (auto* op = llvm::dyn_cast<llvm::GetElementPtrInst>(&I)){

						I.dump();
						llvm::Value* index = op->getOperand(op->getNumOperands()-1);
						llvm::Instruction* Iroot = llvm::dyn_cast<llvm::Instruction>(index);
						Node* n = new Node();
						n->father = NULL;
						n->V = &*Iroot;
						if(search(n,phi)){
							llvm::MDNode* N = llvm::MDNode::get(I.getContext(), llvm::MDString::get(I.getContext(), "monotonic"));
							I.setMetadata("monotonic.safe.index", N);
							changed = true;
						}
						del(n);
					}
				}
			}

			return (changed==true)?changed:false;
		}
	};
}

char MLD::ID=0;

static void register_MLD_Pass(const llvm::PassManagerBuilder &, llvm::legacy::PassManagerBase &PM) {
	PM.add(new MLD());
}

static llvm::RegisterStandardPasses RegisterMyPass(llvm::PassManagerBuilder::EP_EarlyAsPossible, register_MLD_Pass);
