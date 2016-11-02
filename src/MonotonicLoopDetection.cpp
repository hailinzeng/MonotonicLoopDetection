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

	Direction operator&&(const Direction& ld, const Direction& rd)
	{
		if(ld == Direction::Unknown || rd == Direction::Unknown) return Direction::Unknown;
		else if(ld == Direction::Invariant && rd == Direction::Invariant) return Direction::Invariant;
		else if(ld == rd) return ld;
		else if(ld == Direction::Invariant) return rd;
		else if(rd == Direction::Invariant) return ld;
		return Direction::Unknown;
	}

	std::ostream& operator<<(std::ostream& os, const Direction& dir) {
		if(dir==Direction::Increasing) os << "Increasing";
		else if(dir==Direction::Decreasing) os << "Decreasing";
		else if(dir==Direction::Invariant) os << "Invariant";
		else if(dir==Direction::Unknown) os << "Unknown";
		return os;
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

	llvm::Argument* isArgument(llvm::Value* V)
	{
		return llvm::dyn_cast<llvm::Argument>(V);
	}

	llvm::BinaryOperator* isBinaryOperator(llvm::Value* V)
	{
		return llvm::dyn_cast<llvm::BinaryOperator>(V);
	}


	llvm::Instruction* isInstruction(llvm::Value* V)
	{
		return llvm::dyn_cast<llvm::Instruction>(V);
	}

	llvm::PHINode* isPHI(llvm::Value* V)
	{
		return llvm::dyn_cast<llvm::PHINode>(V);
	}

	llvm::Instruction* isTurningNegative(llvm::Instruction* I)
	{
		if(I->getOpcode()==llvm::Instruction::Sub)
		{
			if(isConstantInt(I->getOperand(0))) return I;
		}
		return NULL;
	}

	llvm::GetElementPtrInst* isGetElementPtrInst(llvm::Instruction* I)
	{
		return llvm::dyn_cast<llvm::GetElementPtrInst>(I);
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


	bool isInvariantInsideLoop(llvm::Loop* L, llvm::Value* V)
	{
		if(isConstantInt(V)) return true;
		for(auto U : V->users())
		{
			if (llvm::StoreInst* st = llvm::dyn_cast<llvm::StoreInst>(U))
			{
				if(L->contains(st)) return false;
			}
		}
		return true;
	}


	Direction getDirection(llvm::PHINode* phi, llvm::Instruction* I)
	{
		if(auto p = isPHI(I)) return getDirection(p);
		llvm::Value* op1;
		llvm::Value* op2;
		if(I==NULL) return Direction::Unknown;
		else if(isBinaryOperator(I))
		{
			op1 = I->getOperand(0);
			op2 = I->getOperand(1);
		}
		else return Direction::Unknown;

		Direction dir1, dir2;
		if(isConstantInt(op1)) dir1 = Direction::Invariant;
		else if(isArgument(op1)) dir1 = Direction::Unknown;
		else if(isPHI(op1)) dir1 = getDirection(isPHI(op1));
		else if(isBinaryOperator(op1)) dir1 = getDirection(phi, isBinaryOperator(op1));

		if(isConstantInt(op2)) dir2 = Direction::Invariant;
		else if(isArgument(op2)) dir2 = Direction::Unknown;
		else if(isPHI(op2)) dir2 = getDirection(isPHI(op2));
		else if(isBinaryOperator(op2)) dir2 = getDirection(phi, isBinaryOperator(op2));

		if(I->getOpcode()==llvm::Instruction::Add)
		{
			return dir1 && dir2;
		}
		else if(I->getOpcode()==llvm::Instruction::Sub)
		{
			if(isConstantInt(op1)) return !dir2;
			return dir1;
		}
		else if(I->getOpcode()==llvm::Instruction::Mul)
		{
			if(auto ci = isConstantInt(op1))
			{
				if(ci->getValue().isNegative()) return !dir2;
			}
			else if(auto ci = isConstantInt(op2))
			{
				if(ci->getValue().isNegative()) return !dir1;
			}
			else if(dir1 == Direction::Decreasing && dir2 != Direction::Decreasing) return !getDirection(phi);
			else if(dir1 != Direction::Decreasing && dir2 == Direction::Decreasing) return !getDirection(phi);
			else if(dir1 == Direction::Decreasing && dir2 == Direction::Decreasing) return getDirection(phi);
			return dir1 && dir2;
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

	llvm::StoreInst* getStore(llvm::Loop* L, llvm::GetElementPtrInst* ptr)
	{
		for(auto U : ptr->users())
		{
			if (llvm::StoreInst* st = llvm::dyn_cast<llvm::StoreInst>(U))
			{
				if(L->contains(st)) return st;
			}
		}
		return NULL;
	}

	llvm::LoadInst* getLoad(llvm::Loop* L, llvm::GetElementPtrInst* ptr)
	{
		for(auto U : ptr->users())
		{
			if (llvm::LoadInst* ld = llvm::dyn_cast<llvm::LoadInst>(U))
			{
				if(L->contains(ld)) return ld;
			}
		}
		return NULL;
	}

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

			std::vector<llvm::BasicBlock*> blocks;
			blocks.push_back(getLoopBody(L));
			getBlocks(blocks,getLoopBody(L));

			llvm::LLVMContext& C = llvm::getGlobalContext();
			llvm::MDNode* MTN = llvm::MDNode::get(C, llvm::MDString::get(C, "monotonicity"));

			for(llvm::BasicBlock* bb : blocks)
			{
				for(llvm::Instruction& I : *bb)
				{
					if(auto ge = isGetElementPtrInst(&I))
					{
						auto idx = isInstruction(ge->getOperand(2));
						idx = isInstruction(idx->getOperand(0));
						if(getDirection(phi,idx) == loopDir)
						{
							if(auto st = getStore(L,ge))
							{
								if(getDirection(phi,isInstruction(st->getOperand(0))) != loopDir)
								{
									continue;
								}
								st->setMetadata("monotonic.safe",MTN);
								isInstruction(st->getOperand(0))->setMetadata("monotonic.safe",MTN);
							}
							ge->setMetadata("monotonic.safe",MTN);
							if(auto ld = getLoad(L,ge)) ld->setMetadata("monotonic.safe",MTN);
						}
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
