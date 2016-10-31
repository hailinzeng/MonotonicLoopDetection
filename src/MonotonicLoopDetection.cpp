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

	enum Direction { Increasing, Decreasing, Invariant, Unknown };

	llvm::PHINode* getLoopVar(llvm::Loop* L)
	{
		for(llvm::Instruction& I : *L->getHeader())
		{
                       	if(I.getOpcode()==llvm::Instruction::PHI)
                        {
	                      	for(llvm::Instruction& i : *L->getLoopLatch())
                                {
        	                     	if(&i==I.getOperand(1)) return &I;
                                }
			}
		}
		return NULL;
	}

	llvm::ICmpInst* getLoopCondition(llvm::Loop* L)
	{
		for(llvm::Instruction& I : *L->getHeader())
		{
                	if(llvm::dyn_cast<llvm::ICmpInst>(&I)) return = &I;
		}
		return NULL;
	}

	bool isConstantInt(llvm::Value* V)
	{
		return llvm::dyn_cast<llvm::ConstantInt>(V);
	}

	Direction getLoopDirection(llvm::PHINode* phi)
	{
	        if(llvm::dyn_cast<llvm::PHINode>(I))
                {
        	        if(llvm::Instruction* ii = llvm::dyn_cast<llvm::Instruction>(I->getOperand(1)))
                        {
                	        if(ii->getOpcode()==llvm::Instruction::Add)
                        	{
                                	if((ii->getOperand(0)==phi)&&(isConstantInt(ii->getOperand(1))))
                                        {
                                        	if(isConstantInt(ii->getOperand(1))->getValue().isNegative()) return Direction::Decreasing;
                                                else if(*(isConstantInt(ii->getOperand(1))->getValue().getRawData())>0) return Direction::Increasing;
                                                else return Direction::Invariant;
                                        }
                                        else if((isConstantInt(ii->getOperand(0)))&&(ii->getOperand(1)==phi))
                                        {
                                        	if(isConstantInt(ii->getOperand(0))->getValue().isNegative()) return Direction::Decreasing;
                                                else if(*(isConstantInt(ii->getOperand(0))->getValue().getRawData())<0) return Direction::Increasing;
                                                else return Direction::Invariant;
                                        }
                                }
                         }
                }
                return Direction::Unknown;
	}

	Direction getStatementDirection(llvm::Instruction* I)
	{
		return Direction::Unknown;
	}

	llvm::BasicBlock* getLoopBody(llvm::Loop* L)
	{}

	bool isInvariantInsideLoop(llvm::Loop* L, llvm::Instruction* I)
	{

	}

	bool isLoopMonotonic(llvm::Instruction* I, Direction loopDirection)
	{}

	struct MLD: llvm::LoopPass
	{
		static char ID;

		MLD(): llvm::LoopPass(ID)
		{}

		virtual bool runOnLoop(llvm::Loop* L, llvm::LPPassManager &LPM)
		{
			llvm::PHINode* phi = getLoopVar(L);
			llvm::ICmpInst* cmp = getLoopCondition(L);
			Direction loopDir = getLoopDirection(L);

			return true;
		}
	};
}

char MLD::ID=0;

static void register_MLD_Pass(const llvm::PassManagerBuilder &, llvm::legacy::PassManagerBase &PM) {
	PM.add(new MLD());
}

static llvm::RegisterStandardPasses RegisterMyPass(llvm::PassManagerBuilder::EP_EarlyAsPossible, register_MLD_Pass);
