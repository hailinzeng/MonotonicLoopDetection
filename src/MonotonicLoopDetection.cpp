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
		llvm::Instruction* I;
		Node* father;
		std::vector<Node*> son;
		Node(): I(NULL), father(NULL){}
	};

	bool search(Node* n, llvm::Instruction* fI)
	{
		bool ret = true;
		if(fI==n->I)
		{
			return true;
		}
		else if(n->son.size()==0)
		{
			for(unsigned int i=0; i<n->I->getNumOperands(); i++)
			{
				if(llvm::dyn_cast<llvm::Argument>(n->I->getOperand(i)))
				{
					std::cerr << "ERROR NO MONOTONIC: using function argument" << std::endl;
					return false;
				}
				else if(llvm::dyn_cast<llvm::CallInst>(n->I->getOperand(i)))
				{
					std::cerr << "ERROR NO MONOTONIC: using function output" << std::endl;
					return false;
				}
				else if(llvm::dyn_cast<llvm::ConstantInt>(n->I->getOperand(i))) continue;
				else if(llvm::Instruction* sI = llvm::dyn_cast<llvm::Instruction>(n->I->getOperand(i)))
				{
					Node* sN = new Node();
					sN->father = n;
					sN->I = sI;
					n->son.push_back(sN);
				}
			}
		}
		for(s : n->son)
		{
			if((n->father!=NULL)&&(s->I == n->father->I))
			{
				std::cerr << "ALERT: Circular reference" << std::endl;
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
			std::cerr << std::endl << "#--------------#" << std::endl << std::endl;

			llvm::Instruction* phi = NULL;
			llvm::Instruction* condition = NULL;

			for(llvm::Instruction& I : *L->getHeader()){
				if(llvm::dyn_cast<llvm::ICmpInst>(&I)) condition = &I;

				if(I.getOpcode()==llvm::Instruction::PHI)
				{
					for(llvm::Instruction& i : *L->getLoopLatch())
					{
						if(&i==I.getOperand(1))	phi = &I;
					}
				}
			}

			bool ismonotonic = true;

			std::vector<llvm::BasicBlock *> v = L->getBlocks();
			for(llvm::BasicBlock* bb : v)
			{
				for(llvm::Instruction& I : *bb)
				{
					if (auto* op = llvm::dyn_cast<llvm::GetElementPtrInst>(&I))
					{

						I.dump();

						llvm::MDNode* N = llvm::MDNode::get(I.getContext(), llvm::MDString::get(I.getContext(), "monotonic"));
						if(condition==NULL)
						{
							std::cerr << "ERROR CANNOT DETECT BOUNDS: endless loop" << std::endl;
							I.setMetadata("monotonic.unsafe.index", N);
							return false;
						}

//						llvm::Value* arr = op->getPointerOperand();
						llvm::Value* index = op->getOperand(op->getNumOperands()-1);
						llvm::Instruction* Iroot = llvm::dyn_cast<llvm::Instruction>(index);
						Node* n = new Node();
						n->father = NULL;
						n->I = Iroot;
						if(!search(n,phi))
						{
							ismonotonic = false;
							I.setMetadata("monotonic.fail.index", N);

						}else{
							I.setMetadata("monotonic.safe.index", N);
						}
					}
				}
			}

			std::cerr << "Is monotonic: " << ismonotonic << std::endl;

			return true;
		}
	};
}

char MLD::ID=0;

static void register_MLD_Pass(const llvm::PassManagerBuilder &, llvm::legacy::PassManagerBase &PM) {
	PM.add(new MLD());
}

static llvm::RegisterStandardPasses RegisterMyPass(llvm::PassManagerBuilder::EP_EarlyAsPossible, register_MLD_Pass);
