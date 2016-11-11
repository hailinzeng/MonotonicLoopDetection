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

#include <iostream>
#include <vector>
#include <functional>

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
		else os << "Unknown";
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
		if(!V) return NULL;
		return llvm::dyn_cast<llvm::ConstantInt>(V);
	}

	llvm::Argument* isArgument(llvm::Value* V)
	{
		if(!V) return NULL;
		return llvm::dyn_cast<llvm::Argument>(V);
	}

	llvm::BinaryOperator* isBinaryOperator(llvm::Value* V)
	{
		if(!V) return NULL;
		return llvm::dyn_cast<llvm::BinaryOperator>(V);
	}


	llvm::Instruction* isInstruction(llvm::Value* V)
	{
		if(!V) return NULL;
		return llvm::dyn_cast<llvm::Instruction>(V);
	}

	llvm::PHINode* isPHI(llvm::Value* V)
	{
		if(!V) return NULL;
		return llvm::dyn_cast<llvm::PHINode>(V);
	}


	llvm::GetElementPtrInst* isGetElementPtrInst(llvm::Value* V)
	{
		if(!V) return NULL;
		return llvm::dyn_cast<llvm::GetElementPtrInst>(V);
	}

	llvm::ICmpInst* isICmpInst(llvm::Value* V)
	{
		if(!V) return NULL;
		return llvm::dyn_cast<llvm::ICmpInst>(V);
	}

	llvm::LoadInst* isLoadInst(llvm::Value* V)
	{
		if(!V) return NULL;
		return llvm::dyn_cast<llvm::LoadInst>(V);
	}

	llvm::SExtInst* isSExtInst(llvm::Value* V)
	{
		if(!V) return NULL;
		return llvm::dyn_cast<llvm::SExtInst>(V);
	}

	llvm::ZExtInst* isZExtInst(llvm::Value* V)
	{
		if(!V) return NULL;
		return llvm::dyn_cast<llvm::ZExtInst>(V);
	}

	bool isSimpleExpression(llvm::Instruction* I, llvm::Value* V)
	{
		bool ret = true;
		for(unsigned int i = 0; i < I->getNumOperands(); i++)
		{
			if(isConstantInt(I->getOperand(i))) continue;
			else if(I->getOperand(i) == V) continue;
			else if(auto binOp = isBinaryOperator(I->getOperand(i))) ret = ret && isSimpleExpression(binOp,V);
			else return false;
		}
		return ret;
	}

	Direction getDirection(llvm::PHINode* phi)
	{
		if(phi->getNumOperands()==1) return getDirection(isPHI(phi->getOperand(0)));
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
                                        else if(*(isConstantInt(ii->getOperand(0))->getValue().getRawData())>0) return Direction::Increasing;
                                        else return Direction::Invariant;
                                }
                         }
                }
                return Direction::Unknown;
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
		else if(auto ld = isLoadInst(I))
		{
			return getDirection(phi,isInstruction(ld->getOperand(0)));
		}
		else if(auto ge = isGetElementPtrInst(I))
		{
			return getDirection(phi,isInstruction(ge->getOperand(2)));
		}
		else if(auto sext = isSExtInst(I))
		{
			return getDirection(phi,isInstruction(sext->getOperand(0)));
		}
		else if(auto zext = isZExtInst(I))
		{
			return getDirection(phi,isInstruction(zext->getOperand(0)));
		}
		else{
			return Direction::Unknown;
		}

		Direction dir1, dir2;
		if(isConstantInt(op1)) dir1 = Direction::Invariant;
		else if(isArgument(op1)) dir1 = Direction::Unknown;
		else if(isLoadInst(op1)) dir1 = getDirection(phi,isInstruction(op1));
		else if(isPHI(op1)) dir1 = getDirection(isPHI(op1));
		else if(isBinaryOperator(op1)) dir1 = getDirection(phi, isBinaryOperator(op1));

		if(isConstantInt(op2)) dir2 = Direction::Invariant;
		else if(isArgument(op2)) dir2 = Direction::Unknown;
		else if(isLoadInst(op2)) dir2 = getDirection(phi,isInstruction(op2));
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
        	        if(llvm::BranchInst* op = llvm::dyn_cast<llvm::BranchInst>(&I))
			{
				if(op->getNumOperands() > 1) return llvm::dyn_cast<llvm::BasicBlock>(op->getOperand(2));
				else return L->getHeader();
			}
		return NULL;
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



	llvm::Value* getMax(llvm::PHINode* phi, llvm::ICmpInst* cmp)
	{
		if((cmp && phi) && isICmpInst(cmp))
		{
			Direction dir = getDirection(phi);
			if(dir==Direction::Increasing)
			{
				if(cmp->getOperand(0)==phi)
				{
					llvm::IRBuilder<> builder(llvm::getGlobalContext());
					if(cmp->getPredicate()==llvm::ICmpInst::ICMP_ULT) return builder.getInt32(*(isConstantInt(cmp->getOperand(1))->getValue().getRawData())-1);
					else if(cmp->getPredicate()==llvm::ICmpInst::ICMP_SLT) return builder.getInt32(*(isConstantInt(cmp->getOperand(1))->getValue().getRawData())-1);
					else if(cmp->getPredicate()==llvm::ICmpInst::ICMP_ULE) return cmp->getOperand(1);
					else if(cmp->getPredicate()==llvm::ICmpInst::ICMP_SLE) return cmp->getOperand(1);
				}
				else if(cmp->getOperand(1)==phi)
				{
					llvm::IRBuilder<> builder(llvm::getGlobalContext());
					if(cmp->getPredicate()==llvm::ICmpInst::ICMP_UGT) return builder.getInt32(*(isConstantInt(cmp->getOperand(0))->getValue().getRawData())-1);
					else if(cmp->getPredicate()==llvm::ICmpInst::ICMP_SGT) return builder.getInt32(*(isConstantInt(cmp->getOperand(0))->getValue().getRawData())-1);
					else if(cmp->getPredicate()==llvm::ICmpInst::ICMP_UGE) return cmp->getOperand(0);
					else if(cmp->getPredicate()==llvm::ICmpInst::ICMP_SGE) return cmp->getOperand(0);
				}
			}
			else if(dir == Direction::Decreasing)
			{
				if(cmp->getOperand(0)==phi)
				{
					llvm::IRBuilder<> builder(llvm::getGlobalContext());
					if(cmp->getPredicate()==llvm::ICmpInst::ICMP_UGT) return builder.getInt32(*(isConstantInt(cmp->getOperand(1))->getValue().getRawData())+1);
					else if(cmp->getPredicate()==llvm::ICmpInst::ICMP_SGT) return builder.getInt32(*(isConstantInt(cmp->getOperand(1))->getValue().getRawData())+1);
					else if(cmp->getPredicate()==llvm::ICmpInst::ICMP_UGE) return cmp->getOperand(1);
					else if(cmp->getPredicate()==llvm::ICmpInst::ICMP_SGE) return cmp->getOperand(1);
				}
				else if(cmp->getOperand(1)==phi)
				{
					llvm::IRBuilder<> builder(llvm::getGlobalContext());
					if(cmp->getPredicate()==llvm::ICmpInst::ICMP_ULT) return builder.getInt32(*(isConstantInt(cmp->getOperand(0))->getValue().getRawData())+1);
					else if(cmp->getPredicate()==llvm::ICmpInst::ICMP_SLT) return builder.getInt32(*(isConstantInt(cmp->getOperand(0))->getValue().getRawData())+1);
					else if(cmp->getPredicate()==llvm::ICmpInst::ICMP_ULE) return cmp->getOperand(0);
					else if(cmp->getPredicate()==llvm::ICmpInst::ICMP_SLE) return cmp->getOperand(0);
				}
			}
		}
		return NULL;
	}

	llvm::Value* getMin(llvm::PHINode* phi)
	{
		if(!phi) return NULL;
		return phi->getOperand(0);
	}


	llvm::Function* exit_prototype(llvm::Module* M)
	{
		llvm::LLVMContext& Ctx = M->getContext();
		llvm::Constant* c = M->getOrInsertFunction("exit", llvm::Type::getVoidTy(Ctx), llvm::Type::getInt32Ty(Ctx), NULL);
		llvm::Function* exit_f = llvm::cast<llvm::Function>(c);
		return exit_f;
	}

	llvm::Function* printf_function = NULL;
	llvm::Value* msg = NULL;
	llvm::Value* err_msg = NULL;

	llvm::Function* createMax(llvm::Module* M, llvm::Function* exit_f)
	{
		llvm::LLVMContext& Ctx = M->getContext();
		llvm::Constant* c = M->getOrInsertFunction("__check_array_max", llvm::Type::getInt1Ty(Ctx), llvm::Type::getInt32Ty(Ctx), llvm::Type::getInt32Ty(Ctx), NULL);
		llvm::Function* max_f = llvm::cast<llvm::Function>(c);

		llvm::Function::arg_iterator args = max_f->arg_begin();
		llvm::Value* idx = &*args++;
		idx->setName("idx");
		llvm::Value* mx = &*args++;
		mx->setName("mx");

		llvm::BasicBlock* entry = llvm::BasicBlock::Create(llvm::getGlobalContext(), "entry", max_f);
		llvm::BasicBlock* ret = llvm::BasicBlock::Create(llvm::getGlobalContext(), "return", max_f);
		llvm::BasicBlock* cond_false = llvm::BasicBlock::Create(llvm::getGlobalContext(), "cond_false", max_f);

		llvm::IRBuilder<> builder(entry);

		llvm::Value* less = builder.CreateICmpSLT(idx, mx, "tmp");

		builder.CreateCondBr(less, ret, cond_false);
		builder.SetInsertPoint(ret);
		builder.CreateRet(less);
		builder.SetInsertPoint(cond_false);

		llvm::Value* errval = llvm::ConstantInt::get(llvm::Type::getInt32Ty(M->getContext()),-1);

		builder.CreateCall(printf_function,err_msg);
		builder.CreateCall(exit_f,errval);
		builder.CreateRet(less);

		return max_f;
	}


	llvm::Function* createMin(llvm::Module* M, llvm::Function* exit_f)
	{
		llvm::LLVMContext& Ctx = M->getContext();
		llvm::Constant* c = M->getOrInsertFunction("__check_array_min", llvm::Type::getInt1Ty(Ctx), llvm::Type::getInt32Ty(Ctx), llvm::Type::getInt32Ty(Ctx), NULL);
		llvm::Function* min_f = llvm::cast<llvm::Function>(c);

		llvm::Function::arg_iterator args = min_f->arg_begin();
		llvm::Value* idx = &*args++;
		idx->setName("idx");
		llvm::Value* mn = &*args++;
		mn->setName("mn");

		llvm::BasicBlock* entry = llvm::BasicBlock::Create(llvm::getGlobalContext(), "entry", min_f);
		llvm::BasicBlock* ret = llvm::BasicBlock::Create(llvm::getGlobalContext(), "return", min_f);
		llvm::BasicBlock* cond_false = llvm::BasicBlock::Create(llvm::getGlobalContext(), "cond_false", min_f);

		llvm::IRBuilder<> builder(entry);

		llvm::Value* greater = builder.CreateICmpSGE(idx, mn, "tmp");

		builder.CreateCondBr(greater, ret, cond_false);
		builder.SetInsertPoint(ret);
		builder.CreateRet(greater);
		builder.SetInsertPoint(cond_false);

		llvm::Value* errval = llvm::ConstantInt::get(llvm::Type::getInt32Ty(M->getContext()),-1);

		builder.CreateCall(printf_function,err_msg);
		builder.CreateCall(exit_f,errval);
		builder.CreateRet(greater);

		return min_f;
	}

	std::pair<llvm::Function*,llvm::Function*> p;

	void checkArrayPrototype(llvm::Module* M, llvm::Function* exit_f)
	{
		p.first = createMin(M,exit_f);
		p.second = createMax(M,exit_f);
	}

	bool checkFunctionCreated = false;
	bool printfFunctionCreated = false;

	llvm::Function* printf_prototype(llvm::Module* M) {
		llvm::FunctionType* printf_type =  llvm::TypeBuilder<int(char *, ...), false>::get(M->getContext());
		llvm::Function* func = llvm::cast<llvm::Function>(M->getOrInsertFunction("printf", printf_type,  llvm::AttributeSet().addAttribute(M->getContext(), 1U, llvm::Attribute::NoAlias)));
		return func;
	}

	llvm::Instruction* getInstBeforeLoop(llvm::Loop* L)
	{
		for(llvm::Instruction& i : L->getLoopPreheader()->getInstList())
		{
			if(llvm::BranchInst* op = llvm::dyn_cast<llvm::BranchInst>(&i)) return op;
		}
		return NULL;
	}


	void createCheckArrayBounds(llvm::Loop* L, llvm::Value* min, llvm::Value* max, llvm::GetElementPtrInst* ptr, bool complex=false, llvm::PHINode* phi=NULL)
	{

		if(llvm::AllocaInst* arr = llvm::dyn_cast<llvm::AllocaInst>(ptr->getOperand(0)))
		{

			if(!printfFunctionCreated)
			{
				llvm::IRBuilder<> builder(L->getHeader());
				msg = builder.CreateGlobalStringPtr("Monotonic loop detected!\n");
				err_msg = builder.CreateGlobalStringPtr("Assertion failed!\n");
				printf_function = printf_prototype(L->getHeader()->getParent()->getParent());
				printfFunctionCreated = true;
			}

			if(!checkFunctionCreated)
			{
				llvm::Function* exit_f = exit_prototype(L->getHeader()->getModule());
				checkArrayPrototype(L->getHeader()->getModule(),exit_f);
				checkFunctionCreated = true;
			}


			llvm::PointerType* _p = arr->getType();
			llvm::ArrayType* a = llvm::dyn_cast<llvm::ArrayType>(_p->getElementType());


			std::function<void (llvm::Instruction*,llvm::PHINode*,llvm::Value*)> insertAndReplace = [&](llvm::Instruction* I, llvm::PHINode* phi, llvm::Value* V)
			{
				for(unsigned int i = 0; i < I->getNumOperands(); i++)
				{
					if(I->getOperand(i)==phi) I->setOperand(i,V);
					else if(auto cli = isInstruction(I->getOperand(i)))
					{
						cli = cli->clone();
						I->setOperand(i,cli);
						insertAndReplace(cli,phi,V);
					}
				}
				I->insertBefore(getInstBeforeLoop(L));
			};

			if(complex)
			{
				llvm::Instruction* cloneI = NULL;
				if(1)
				{
					llvm::IRBuilder<> builder(getInstBeforeLoop(L));
					llvm::Value* v = NULL;
					if(llvm::SExtInst* se = llvm::dyn_cast<llvm::SExtInst>(ptr->getOperand(2)))
					{
						builder.SetInsertPoint(se);
						v = se->getOperand(0);
					}
					else if(llvm::ZExtInst* ze = llvm::dyn_cast<llvm::ZExtInst>(ptr->getOperand(2)))
					{
						builder.SetInsertPoint(ze);
						v = ze->getOperand(0);
					}

					cloneI = isInstruction(v)->clone();
					insertAndReplace(cloneI,phi,min);

					builder.SetInsertPoint(getInstBeforeLoop(L));
					llvm::Value* args[] = {cloneI,builder.getInt32(0)};
					builder.CreateCall(p.first,args);

				}
				if(max)
				{

					llvm::IRBuilder<> builder(getInstBeforeLoop(L));
					llvm::Value* v = NULL;
					if(llvm::SExtInst* se = llvm::dyn_cast<llvm::SExtInst>(ptr->getOperand(2)))
					{
						builder.SetInsertPoint(se);
						v = se->getOperand(0);
					}
					else if(llvm::ZExtInst* ze = llvm::dyn_cast<llvm::ZExtInst>(ptr->getOperand(2)))
					{
						builder.SetInsertPoint(ze);
						v = ze->getOperand(0);
					}
					builder.SetInsertPoint(getInstBeforeLoop(L));
					llvm::Value* args[] = {cloneI, builder.getInt32(a->getNumElements())};
					builder.CreateCall(p.second,args);
				}
				else
				{
					llvm::IRBuilder<> builder(getInstBeforeLoop(L));
					llvm::Value* v = NULL;
					if(llvm::SExtInst* se = llvm::dyn_cast<llvm::SExtInst>(ptr->getOperand(2)))
					{
						builder.SetInsertPoint(se);
						v = se->getOperand(0);
					}
					else if(llvm::ZExtInst* ze = llvm::dyn_cast<llvm::ZExtInst>(ptr->getOperand(2)))
					{
						builder.SetInsertPoint(ze);
						v = ze->getOperand(0);
					}
					llvm::Value* args[] = {v,builder.getInt32(a->getNumElements())};
					builder.CreateCall(p.second,args);
				}
				return;
			}

			if(min && max)
			{
//				std::cerr << "Min and Max" << std::endl;
				llvm::IRBuilder<> builder(getInstBeforeLoop(L));
				llvm::Value* args1[] = {min,builder.getInt32(0)};

				llvm::Value* args2[] = {max,builder.getInt32(a->getNumElements())};
				args1[0] = isConstantInt(args1[0]) ? builder.getInt32(*(isConstantInt(args1[0])->getValue().getRawData())) : args1[0];
				args2[0] = isConstantInt(args2[0]) ? builder.getInt32(*(isConstantInt(args2[0])->getValue().getRawData())) : args2[0];

				builder.CreateCall(p.first,args1);
				builder.CreateCall(p.second,args2);
			}
			else if(min && !max)
			{
//				std::cerr << "Only Min" << std::endl;
				llvm::IRBuilder<> builder(getInstBeforeLoop(L));
				llvm::Value* args1[] = {min,builder.getInt32(0)};
				args1[0] = isConstantInt(args1[0]) ? builder.getInt32(*(isConstantInt(args1[0])->getValue().getRawData())) : args1[0];

				builder.CreateCall(p.first,args1);

				llvm::Value* v = NULL;
				if(llvm::SExtInst* se = llvm::dyn_cast<llvm::SExtInst>(ptr->getOperand(2)))
				{
					builder.SetInsertPoint(se);
					v = se->getOperand(0);
				}
				else if(llvm::ZExtInst* ze = llvm::dyn_cast<llvm::ZExtInst>(ptr->getOperand(2)))
				{
					builder.SetInsertPoint(ze);
					v = ze->getOperand(0);
				}
				llvm::Value* args2[] = {v,builder.getInt32(a->getNumElements())};
				args2[0] = isConstantInt(args2[0]) ? builder.getInt32(*(isConstantInt(args2[0])->getValue().getRawData())) : args2[0];

				builder.CreateCall(p.second,args2);

			}
			else if(!min && max)
			{
///				std::cerr << "Only Max" << std::endl;
				llvm::IRBuilder<> builder(getInstBeforeLoop(L));
				llvm::Value* args2[] = {max,builder.getInt32(a->getNumElements())};
				args2[0] = isConstantInt(args2[0]) ? builder.getInt32(*(isConstantInt(args2[0])->getValue().getRawData())) : args2[0];

				builder.CreateCall(p.second,args2);

				llvm::Value* v = NULL;
				if(llvm::SExtInst* se = llvm::dyn_cast<llvm::SExtInst>(ptr->getOperand(2)))
				{
					builder.SetInsertPoint(se);
					v = se->getOperand(0);
				}
				else if(llvm::ZExtInst* ze = llvm::dyn_cast<llvm::ZExtInst>(ptr->getOperand(2)))
				{
					builder.SetInsertPoint(ze);
					v = ze->getOperand(0);
				}
				llvm::Value* args1[] = {v,builder.getInt32(0)};
				args1[0] = isConstantInt(args1[0]) ? builder.getInt32(*(isConstantInt(args1[0])->getValue().getRawData())) : args1[0];

				builder.CreateCall(p.first,args1);

			}
			else if(!min && !max)
			{
//				std::cerr << "None" << std::endl;
				if(1)
				{
					llvm::IRBuilder<> builder(ptr);
					llvm::Value* v = NULL;
					if(llvm::SExtInst* se = llvm::dyn_cast<llvm::SExtInst>(ptr->getOperand(2)))
					{
						builder.SetInsertPoint(se);
						v = se->getOperand(0);
					}
					else if(llvm::ZExtInst* ze = llvm::dyn_cast<llvm::ZExtInst>(ptr->getOperand(2)))
					{
						builder.SetInsertPoint(ze);
						v = ze->getOperand(0);
					}
					llvm::Value* args[] = {v,builder.getInt32(0)};
					builder.CreateCall(p.first,args);
				}
				if(1)
				{
					llvm::IRBuilder<> builder(ptr);
					llvm::Value* v = NULL;
					if(llvm::SExtInst* se = llvm::dyn_cast<llvm::SExtInst>(ptr->getOperand(2)))
					{
						builder.SetInsertPoint(se);
						v = se->getOperand(0);
					}
					else if(llvm::ZExtInst* ze = llvm::dyn_cast<llvm::ZExtInst>(ptr->getOperand(2)))
					{
						builder.SetInsertPoint(ze);
						v = ze->getOperand(0);
					}
					llvm::Value* args[] = {v,builder.getInt32(a->getNumElements())};
					builder.CreateCall(p.second,args);
				}
			}

		}
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

			llvm::Value* min = (loopDir == Direction::Increasing) ? getMin(phi) : getMax(phi,cmp);
			llvm::Value* max = (loopDir == Direction::Increasing) ? getMax(phi,cmp) : getMin(phi);

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
									st->setMetadata("not.monotonic.or.unknown",MTN);
									ge->setMetadata("not.monotonic.or.unknown",MTN);
									continue;
								}
								st->setMetadata("is.monotonic",MTN);
							}

							ge->setMetadata("is.monotonic",MTN);
							if(auto ld = getLoad(L,ge)) ld->setMetadata("is.monotonic",MTN);

							if (!phi->getMetadata("not.monotonic.or.unknown")) phi->setMetadata("is.monotonic",MTN);

							if(idx != phi)
							{
								createCheckArrayBounds(L,min,max,ge,true,phi);
							}
							else createCheckArrayBounds(L,min,max,ge);
						}
						else{
							if (phi->getMetadata("is.monotonic"))
							{
								phi->setMetadata("is.monotonic",NULL);
							}
							phi->setMetadata("not.monotonic.or.unknown",MTN);
							ge->setMetadata("not.monotonic.or.unknown",MTN);
							createCheckArrayBounds(L,NULL,NULL,ge);

						}

					}
				}
			}

			if (phi->getMetadata("is.monotonic"))
			{
				llvm::IRBuilder<> builder(llvm::getGlobalContext());
				builder.SetInsertPoint(getInstBeforeLoop(L));
				builder.CreateCall(printf_function, msg);
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
