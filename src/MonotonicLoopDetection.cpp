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

	struct MLD: llvm::LoopPass
	{
		static char ID;

		MLD(): llvm::LoopPass(ID)
		{}

		bool uses(llvm::Instruction* phi, llvm::Instruction* I)
		{
			bool r = false;
			if(I==phi) return true;
			else{
				for(unsigned int i=0; i<I->getNumOperands(); i++)
				{
					if(llvm::dyn_cast<llvm::ConstantInt>(I->getOperand(i))) return true;
					r = r || uses(phi,llvm::dyn_cast<llvm::Instruction>(I->getOperand(i)));
				}
			}
			return r;
		}

/*
		bool isAtLoop(std::vector<llvm::BasicBlock *> vec, llvm::Value* v)
		{
			for(llvm::BasicBlock* bb : vec){
				for(llvm::Instruction& I : *bb){
					llvm::Value* lhs = &I;
					if (v==lhs)
					{
						I.dump();
						bool r = true;
						if(!(I.getOpcode()==llvm::Instruction::PHI)){
							for(unsigned int i=0; i<I.getNumOperands(); i++)
							{
								if(!llvm::dyn_cast<llvm::ConstantInt>(I.getOperand(i))) r = r && isAtLoop(vec,I.getOperand(i));
							}
						}

						return r;
					}
				}
			}
			return false;
		}
*/

		std::vector<llvm::Instruction*> getPhi(llvm::BasicBlock* bb)
		{
			std::vector<llvm::Instruction*> phi;
			for(llvm::Instruction& I : *bb){
				if(I.getOpcode()==llvm::Instruction::PHI) phi.push_back(&I);
			}
			return phi;
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
			L->dump();

			std::pair<llvm::Value*, llvm::Value*> loopranges = getRanges(L->getHeader());
			std::vector<llvm::Instruction*> phi = getPhi(L->getHeader());

//			std::cerr << llvm::dyn_cast<llvm::PHINode>(phi)->getIncomingBlock(1)->getName().str() << std::endl;

			bool ismonotonic = true;

			std::vector<llvm::BasicBlock *> v = L->getBlocks();
			for(llvm::BasicBlock* bb : v)
			{
				for(llvm::Instruction& I : *bb)
				{
					if (auto* op = llvm::dyn_cast<llvm::GetElementPtrInst>(&I))
					{
						llvm::Value* arr = op->getPointerOperand();
						llvm::Value* index = op->getOperand(op->getNumOperands()-1);
						bool usesphi = false;
						for(p:phi)
						{
							if(uses(p,llvm::dyn_cast<llvm::Instruction>(index)))
							{
								usesphi = true;
//								std::cerr << "Index using loop ranges of:  %" << arr->getName().str() << std::endl;
							}
						}
						ismonotonic = usesphi;;
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
