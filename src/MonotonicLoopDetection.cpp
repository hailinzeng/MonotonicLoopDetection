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
			bool r = true;
//			std::cerr << I->getName().str() << std::endl;
			if(phi==I) return true;
			for(unsigned int i=0; i<I->getNumOperands(); i++)
			{
				if(llvm::dyn_cast<llvm::ConstantInt>(I->getOperand(i))) continue;
				else if(llvm::dyn_cast<llvm::Instruction>(I->getOperand(i))) r = r && uses(phi,llvm::dyn_cast<llvm::Instruction>(I->getOperand(i)));
				else return false;
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
			//show loop
			L->dump();

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
						llvm::Value* arr = op->getPointerOperand();
						llvm::Value* index = op->getOperand(op->getNumOperands()-1);
						if(!uses(phi,llvm::dyn_cast<llvm::Instruction>(index)))
						{
							std::cerr << "Index request uses non loop variable: ";
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
