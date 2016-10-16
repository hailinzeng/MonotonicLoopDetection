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

namespace{

	struct MLD: llvm::LoopPass
	{
		static char ID;

		MLD(): llvm::LoopPass(ID)
		{}

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

		virtual bool runOnLoop(llvm::Loop* L, llvm::LPPassManager &LPM)
		{
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
						I.dump();
						std::cerr << isAtLoop(v,index) << std::endl;
					}
				}
			}



			return false;
		}
	};
}

char MLD::ID=0;

static void register_MLD_Pass(const llvm::PassManagerBuilder &, llvm::legacy::PassManagerBase &PM) {
	PM.add(new MLD());
}

static llvm::RegisterStandardPasses RegisterMyPass(llvm::PassManagerBuilder::EP_EarlyAsPossible, register_MLD_Pass);
