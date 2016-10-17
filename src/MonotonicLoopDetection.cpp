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
#include <utility>

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
				if(llvm::dyn_cast<llvm::ConstantInt>(n->I->getOperand(i))) continue;
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
				return true;
			}
			else if(llvm::dyn_cast<llvm::CallInst>(s->I)) return false;
			else ret = ret && search(s,fI);
		}

		return ret;
	}


	struct MLD: llvm::LoopPass
	{
		static char ID;

		MLD(): llvm::LoopPass(ID)
		{}

		llvm::Instruction* getPhi(llvm::BasicBlock* bb)
		{
			std::vector<llvm::Instruction*> phivec;
			for(llvm::Instruction& I : *bb){
				if(I.getOpcode()==llvm::Instruction::PHI) phivec.push_back(&I);
			}

			for(llvm::Instruction& I : *bb){
				if(llvm::dyn_cast<llvm::ICmpInst>(&I))
				{
					for(phi : phivec)
					{
						if(phi==I.getOperand(0)) return phi;
					}
				}
			}

			return NULL;
		}


		std::pair<llvm::Value*,llvm::Value*> getRanges(llvm::BasicBlock* bb)
		{
			llvm::Instruction* condition = NULL;
			llvm::Instruction* phi = NULL;

			for(llvm::Instruction& I : *bb){
				if(llvm::dyn_cast<llvm::ICmpInst>(&I)) condition = &I;
				else if(I.getOpcode()==llvm::Instruction::PHI) phi = &I;
			}
/*
			if (llvm::ConstantInt* CI = llvm::dyn_cast<llvm::ConstantInt>(phi->getOperand(0)))
			{
				std::cerr << "From: " << CI->getSExtValue() << std::endl;
			}

			if (llvm::ConstantInt* CI = llvm::dyn_cast<llvm::ConstantInt>(condition->getOperand(1)))
			{
				std::cerr << "To: " << CI->getSExtValue() << std::endl;
			}
*/

			std::pair<llvm::Value*,llvm::Value*> p;
			p.first = phi->getOperand(0);
			p.second = phi->getOperand(1);

			return p;
		}


		virtual bool runOnLoop(llvm::Loop* L, llvm::LPPassManager &LPM)
		{
			std::cerr << std::endl << "#-#-#-#-#-#-#-#-#-#" << std::endl << std::endl;

			//get the begin and end of loop
			std::pair<llvm::Value*, llvm::Value*> loopranges = getRanges(L->getHeader());
			llvm::Instruction* phi = getPhi(L->getHeader());

			if(phi==NULL){
				std::cerr << "ERROR: Could not get loop phi instruction" << std::endl;
				return false;
			}

			bool ismonotonic = true;

			std::vector<llvm::BasicBlock *> v = L->getBlocks();
			for(llvm::BasicBlock* bb : v)
			{
				for(llvm::Instruction& I : *bb)
				{
					if (auto* op = llvm::dyn_cast<llvm::GetElementPtrInst>(&I))
					{
						op->dump();
						llvm::Value* arr = op->getPointerOperand();
						llvm::Value* index = op->getOperand(op->getNumOperands()-1);
						llvm::Instruction* Iroot = llvm::dyn_cast<llvm::Instruction>(index);
						Node* n = new Node();
						n->father = NULL;
						n->I = Iroot;
						if(!search(n,phi))
						{
							std::cerr << "ERROR: Index request uses non loop variable: ";
							I.dump();
							ismonotonic = false;
						}
					}
				}
			}

			std::cerr << "Is monotonic: " << ismonotonic << std::endl;

			return false;
		}
	};
}

char MLD::ID=0;

static void register_MLD_Pass(const llvm::PassManagerBuilder &, llvm::legacy::PassManagerBase &PM) {
	PM.add(new MLD());
}

static llvm::RegisterStandardPasses RegisterMyPass(llvm::PassManagerBuilder::EP_EarlyAsPossible, register_MLD_Pass);
