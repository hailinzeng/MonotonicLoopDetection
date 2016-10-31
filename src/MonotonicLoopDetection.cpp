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

	Direction operator!(const Direction& d)
	{
		if(d==Direction::Increasing) return Direction::Decreasing;
		else if(d==Direction::Decreasing) return Direction::Increasing;
		return d;
	}

	llvm::PHINode* getLoopVar(llvm::Loop* L)
	{
		for(llvm::Instruction& I : *L->getHeader())
		{
                       	if(I.getOpcode()==llvm::Instruction::PHI)
                        {
	                      	for(llvm::Instruction& i : *L->getLoopLatch())
                                {
        	                     	if(&i==I.getOperand(1)) return llvm::dyn_cast<llvm::PHINode>(&I);
                                }
			}
		}
		return NULL;
	}

	llvm::ICmpInst* getLoopCondition(llvm::Loop* L)
	{
		for(llvm::Instruction& I : *L->getHeader())
		{
                	if(llvm::dyn_cast<llvm::ICmpInst>(&I)) return llvm::dyn_cast<llvm::ICmpInst>(&I);
		}
		return NULL;
	}

	llvm::ConstantInt* isConstantInt(llvm::Value* V)
	{
		return llvm::dyn_cast<llvm::ConstantInt>(V);
	}

	Direction getDirection(llvm::PHINode* phi)
	{
       	        if(llvm::Instruction* ii = llvm::dyn_cast<llvm::Instruction>(phi->getOperand(1)))
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
                return Direction::Unknown;
	}

	Direction getDirection(llvm::Instruction* I)
	{
		if(llvm::PHINode* phi = llvm::dyn_cast<llvm::PHINode>(I)) return getDirection(phi);

		if(I->getOpcode()==llvm::Instruction::Add)
		{
			if((isConstantInt(I->getOperand(1)))&&(llvm::dyn_cast<llvm::PHINode>(I->getOperand(0))))
			{
				auto ci = isConstantInt(I->getOperand(1));
				if(ci->getValue().isNegative()) return Direction::Decreasing;
				else if(*ci->getValue().getRawData() == 0) return Direction::Invariant;
				else if(*ci->getValue().getRawData() > 0) return Direction::Increasing;
			}
		}
		else if(I->getOpcode()==llvm::Instruction::Sub)
		{
			if(auto ci = isConstantInt(I->getOperand(1)))
			{
				if(ci->getValue().isNegative()) return Direction::Increasing;
				else if(*ci->getValue().getRawData() == 0) return Direction::Invariant;
				else if(*ci->getValue().getRawData() > 0) return Direction::Decreasing;
			}
		}
		else if(I->getOpcode()==llvm::Instruction::Mul)
		{
			if(auto ci = isConstantInt(I->getOperand(1)))
			{
				if(ci->getValue().isNegative()) return Direction::Decreasing;
				else if(*ci->getValue().getRawData() == 0) return Direction::Invariant;
				else if(*ci->getValue().getRawData() > 0) return Direction::Increasing;
			}
		}
		else if(I->getOpcode()==llvm::Instruction::SDiv)
		{
			if(auto ci = isConstantInt(I->getOperand(1)))
			{
				if(ci->getValue().isNegative()) return Direction::Increasing;
				else if(*ci->getValue().getRawData() == 0) return Direction::Invariant;
				else if(*ci->getValue().getRawData() > 0) return Direction::Decreasing;
			}
		}

		return Direction::Unknown;
	}

	llvm::BasicBlock* getLoopBody(llvm::Loop* L)
	{
	        for(llvm::Instruction& I : *L->getHeader())
        	        if(llvm::BranchInst* op = llvm::dyn_cast<llvm::BranchInst>(&I)) return llvm::dyn_cast<llvm::BasicBlock>(op->getOperand(2));
	}

	void getBlocks(std::vector<llvm::BasicBlock*>& vec, llvm::BasicBlock* bb)
	{
		for(llvm::Instruction& I : *bb)
		{
			if(llvm::BranchInst* br = llvm::dyn_cast<llvm::BranchInst>(&I))
			{
				if(br->getNumOperands()>1)
				{
					for(unsigned int i = 0; i < br->getNumOperands(); i++)
					{
						if(llvm::BasicBlock* _bb = llvm::dyn_cast<llvm::BasicBlock>(br->getOperand(i)))
						{
							vec.push_back(_bb);
							getBlocks(vec,_bb);
						}
					}
				}
			}
		}
	}

	bool isInvariantInsideLoop(llvm::Loop* L, llvm::Value* V)
	{
		for(auto U : V->users())
		{
			if (llvm::StoreInst* st = llvm::dyn_cast<llvm::StoreInst>(U))
			{
				if(L->contains(st)) return false;
			}
		}
		return true;
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
			Direction loopDir = getDirection(phi);

			if(loopDir==Direction::Increasing) std::cerr << "INCREASING" << std::endl;
			if(loopDir==Direction::Decreasing) std::cerr << "DECREASING" << std::endl;

			std::vector<llvm::BasicBlock*> blocks;
			blocks.push_back(getLoopBody(L));
			getBlocks(blocks,getLoopBody(L));

			for(llvm::BasicBlock* bb : blocks)
			{
				for(llvm::Instruction& i : *bb)
				{
					if(getDirection(&i) == !loopDir)
					{
						i.dump();
					}
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
