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


	llvm::Value* getMax(llvm::ICmpInst* cmp)
	{
		if(llvm::dyn_cast<llvm::ICmpInst>(cmp))
		{

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

	std::pair<llvm::Value*,llvm::Value*> getArrayBounds()
	{
		std::pair<llvm::Value*,llvm::Value*> p;
	}

	void createCheckArrayBounds(llvm::Value* min, llvm::Value* max, llvm::GetElementPtrInst* ptr)
	{

	}

	bool isMonotonic(llvm::Loop* L, llvm::PHINode* phi)
	{
		return false;
	}

	std::vector<llvm::GetElementPtrInst*> getArrays(llvm::BasicBlock* bb)
	{
		std::vector<llvm::GetElementPtrInst*> vec;
		return vec;
	}

	struct MLD: llvm::LoopPass
	{
		static char ID;

		MLD(): llvm::LoopPass(ID)
		{}

		virtual bool runOnLoop(llvm::Loop* L, llvm::LPPassManager &LPM)
		{
			std::cerr << "-------------" << std::endl;
			llvm::Instruction* phi = NULL;

			for(llvm::Instruction& I : *L->getHeader()){
				if(I.getOpcode()==llvm::Instruction::PHI)
				{
					for(llvm::Instruction& i : *L->getLoopLatch())
					{
						if(&i==I.getOperand(1))	phi = &I;
					}
				}
			}

			if(phi==NULL)
			{
				std::cerr << "Phi not found" << std::endl;
				return false;
			}

			llvm::Value* min = getMin(phi);
			llvm::Value* max = getMax(phi);
/*
			if(isMonotonic(L,phi))
			{
				for(idx : getArrays())
				{
					Node* n = new Node();
					n->V = &*idx;
					if(search(n,phi))
					{
						createCheckArrayBounds(min,max,idx);
					}
				}
			}
*/
			return true;
		}
	};
}

char MLD::ID=0;

static void register_MLD_Pass(const llvm::PassManagerBuilder &, llvm::legacy::PassManagerBase &PM) {
	PM.add(new MLD());
}

static llvm::RegisterStandardPasses RegisterMyPass(llvm::PassManagerBuilder::EP_EarlyAsPossible, register_MLD_Pass);
