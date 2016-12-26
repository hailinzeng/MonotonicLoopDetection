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
#include "llvm/Transforms/Scalar.h"


namespace {

	#define IS_CONSTANTINT(V) llvm::dyn_cast<llvm::ConstantInt>(V)
	#define IS_ARGUMENT(V) llvm::dyn_cast<llvm::Argument>(V)
	#define IS_BINARYOPERATOR(V) llvm::dyn_cast<llvm::BinaryOperator>(V)
	#define IS_INSTRUCTION(V) llvm::dyn_cast<llvm::Instruction>(V)
	#define IS_PHINODE(V) llvm::dyn_cast<llvm::PHINode>(V)
	#define IS_GETELEMENTPTRINST(V) llvm::dyn_cast<llvm::GetElementPtrInst>(V)
	#define IS_ICMPINST(V) llvm::dyn_cast<llvm::ICmpInst>(V)
	#define IS_LOADINST(V) llvm::dyn_cast<llvm::LoadInst>(V)
	#define IS_SEXTINST(V) llvm::dyn_cast<llvm::SExtInst>(V)
	#define IS_ZEXTINST(V) llvm::dyn_cast<llvm::ZExtInst>(V)

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


        llvm::Function* printf_prototype(llvm::Module* M) {
                llvm::FunctionType* printf_type =  llvm::TypeBuilder<int(char *, ...), false>::get(M->getContext());
                llvm::Function* func = llvm::cast<llvm::Function>(M->getOrInsertFunction("printf", printf_type,  llvm::AttributeSet().addAttribute(M->getContext(), 1U, llvm::Attribute::NoAlias)));
                return func;
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
	bool checkFunctionCreated = false;
	bool printfFunctionCreated = false;

	std::pair<llvm::Function*,llvm::Function*> p;

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

	void checkArrayPrototype(llvm::Module* M, llvm::Function* exit_f)
	{
		p.first = createMin(M,exit_f);
		p.second = createMax(M,exit_f);
	}


        Direction getDirection(llvm::PHINode* phi)
        {
		if(!phi) std::cerr << "ERROR: PHI CAN'T BE NULL" << std::endl;
                if(phi->getNumOperands()==1) return getDirection(IS_PHINODE(phi->getOperand(0)));
                if(llvm::Instruction* ii = llvm::dyn_cast<llvm::Instruction>(phi->getOperand(1)))
                {
                        if(ii->getOpcode()==llvm::Instruction::Add)
                        {
                                if((ii->getOperand(0)==phi)&&(IS_CONSTANTINT(ii->getOperand(1))))
                                {
                                        if(IS_CONSTANTINT(ii->getOperand(1))->getValue().isNegative()) return Direction::Decreasing;
                                        else if(*(IS_CONSTANTINT(ii->getOperand(1))->getValue().getRawData())>0) return Direction::Increasing;
                                        else return Direction::Invariant;
                                }
                                else if((IS_CONSTANTINT(ii->getOperand(0)))&&(ii->getOperand(1)==phi))
                                {
                                        if(IS_CONSTANTINT(ii->getOperand(0))->getValue().isNegative()) return Direction::Decreasing;
                                        else if(*(IS_CONSTANTINT(ii->getOperand(0))->getValue().getRawData())>0) return Direction::Increasing;
                                        else return Direction::Invariant;
                                }
                         }
                }
                return Direction::Unknown;
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


	Direction getDirection(llvm::PHINode* phi, llvm::Instruction* I)
	{
		if(I==NULL) return Direction::Unknown;
		if(auto p = IS_PHINODE(I)) return getDirection(p);
		llvm::Value* op1;
		llvm::Value* op2;
		if(IS_BINARYOPERATOR(I))
		{
			op1 = I->getOperand(0);
			op2 = I->getOperand(1);
		}
		else if(auto ld = IS_LOADINST(I))
		{
			return getDirection(phi,IS_INSTRUCTION(ld->getOperand(0)));
		}
		else if(auto ge = IS_GETELEMENTPTRINST(I))
		{
			return getDirection(phi,IS_INSTRUCTION(ge->getOperand(ge->getNumOperands()-1)));
		}
		else if(auto sext = IS_SEXTINST(I))
		{
			return getDirection(phi,IS_INSTRUCTION(sext->getOperand(0)));
		}
		else if(auto zext = IS_ZEXTINST(I))
		{
			return getDirection(phi,IS_INSTRUCTION(zext->getOperand(0)));
		}
		else{
			return Direction::Unknown;
		}

		Direction dir1, dir2;
		if(IS_CONSTANTINT(op1)) dir1 = Direction::Invariant;
		else if(IS_ARGUMENT(op1)) dir1 = Direction::Unknown;
		else if(IS_LOADINST(op1)) dir1 = getDirection(phi,IS_INSTRUCTION(op1));
		else if(IS_PHINODE(op1)) dir1 = getDirection(IS_PHINODE(op1));
		else if(IS_BINARYOPERATOR(op1)) dir1 = getDirection(phi, IS_BINARYOPERATOR(op1));

		if(IS_CONSTANTINT(op2)) dir2 = Direction::Invariant;
		else if(IS_ARGUMENT(op2)) dir2 = Direction::Unknown;
		else if(IS_LOADINST(op2)) dir2 = getDirection(phi,IS_INSTRUCTION(op2));
		else if(IS_PHINODE(op2)) dir2 = getDirection(IS_PHINODE(op2));
		else if(IS_BINARYOPERATOR(op2)) dir2 = getDirection(phi, IS_BINARYOPERATOR(op2));

		if(I->getOpcode()==llvm::Instruction::Add)
		{
			return dir1 && dir2;
		}
		else if(I->getOpcode()==llvm::Instruction::Sub)
		{
			if(IS_CONSTANTINT(op1)) return !dir2;
			return dir1;
		}
		else if(I->getOpcode()==llvm::Instruction::Mul)
		{
			if(auto ci = IS_CONSTANTINT(op1))
			{
				if(ci->getValue().isNegative()) return !dir2;
			}
			else if(auto ci = IS_CONSTANTINT(op2))
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


	class MLDLoop
	{
		private:
			llvm::Loop* L = NULL;
			llvm::PHINode* phi = NULL;
			llvm::ICmpInst* condition = NULL;
			Direction direction = Direction::Unknown;
			llvm::Instruction* beforeLoop = NULL;
			llvm::Value* min = NULL;
			llvm::Value* max = NULL;

			llvm::PHINode* getVar()
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

			llvm::ICmpInst* getCondition()
			{
		                for(llvm::Instruction& I : *L->getHeader())
		                {
		                        if(llvm::dyn_cast<llvm::ICmpInst>(&I)) return llvm::dyn_cast<llvm::ICmpInst>(&I);
		                }
		                return NULL;
			}

		        llvm::Value* getMin(llvm::PHINode* phi)
			{
		                return phi->getOperand(0);
		        }

		        llvm::Value* getMax(llvm::PHINode* phi, llvm::ICmpInst* cmp)
		        {
		                if((cmp && phi) && IS_ICMPINST(cmp))
		                {
		                        Direction dir = getDirection(phi);
		                        if(dir==Direction::Increasing)
		                        {
		                                if(cmp->getOperand(0)==phi)
		                                {
		                                        llvm::IRBuilder<> builder(llvm::getGlobalContext());
		                                        if(IS_CONSTANTINT(cmp->getOperand(1)))
		                                        {
		                                                if(cmp->getPredicate()==llvm::ICmpInst::ICMP_ULT) return builder.getInt32(*(IS_CONSTANTINT(cmp->getOperand(1))->getValue().getRawData())-1);
		                                                else if(cmp->getPredicate()==llvm::ICmpInst::ICMP_SLT) return builder.getInt32(*(IS_CONSTANTINT(cmp->getOperand(1))->getValue().getRawData())-1);
		                                                else if(cmp->getPredicate()==llvm::ICmpInst::ICMP_ULE) return cmp->getOperand(1);
		                                                else if(cmp->getPredicate()==llvm::ICmpInst::ICMP_SLE) return cmp->getOperand(1);
		                                        }
                		                }
		                                else if(cmp->getOperand(1)==phi)
		                                {
		                                        if(IS_CONSTANTINT(cmp->getOperand(0)))
		                                        {
                		                                llvm::IRBuilder<> builder(llvm::getGlobalContext());
		                                                if(cmp->getPredicate()==llvm::ICmpInst::ICMP_UGT) return builder.getInt32(*(IS_CONSTANTINT(cmp->getOperand(0))->getValue().getRawData())-1);
		                                                else if(cmp->getPredicate()==llvm::ICmpInst::ICMP_SGT) return builder.getInt32(*(IS_CONSTANTINT(cmp->getOperand(0))->getValue().getRawData())-1);
	                	                                else if(cmp->getPredicate()==llvm::ICmpInst::ICMP_UGE) return cmp->getOperand(0);
		                                                else if(cmp->getPredicate()==llvm::ICmpInst::ICMP_SGE) return cmp->getOperand(0);
		                                        }
		                                }
		                        }
		                        else if(dir == Direction::Decreasing)
		                        {
                		                if(cmp->getOperand(0)==phi)
		                                {
		                                        if(IS_CONSTANTINT(cmp->getOperand(1)))
		                                        {
		                                                llvm::IRBuilder<> builder(llvm::getGlobalContext());
		                                                if(cmp->getPredicate()==llvm::ICmpInst::ICMP_UGT) return builder.getInt32(*(IS_CONSTANTINT(cmp->getOperand(1))->getValue().getRawData())+1);
		                                                else if(cmp->getPredicate()==llvm::ICmpInst::ICMP_SGT) return builder.getInt32(*(IS_CONSTANTINT(cmp->getOperand(1))->getValue().getRawData())+1);
		                                                else if(cmp->getPredicate()==llvm::ICmpInst::ICMP_UGE) return cmp->getOperand(1);
		                                                else if(cmp->getPredicate()==llvm::ICmpInst::ICMP_SGE) return cmp->getOperand(1);
		                                        }
		                                }
		                                else if(cmp->getOperand(1)==phi)
		                                {
		                                        if(IS_CONSTANTINT(cmp->getOperand(0)))
		                                        {
		                                                llvm::IRBuilder<> builder(llvm::getGlobalContext());
		                                                if(cmp->getPredicate()==llvm::ICmpInst::ICMP_ULT) return builder.getInt32(*(IS_CONSTANTINT(cmp->getOperand(0))->getValue().getRawData())+1);
		                                                else if(cmp->getPredicate()==llvm::ICmpInst::ICMP_SLT) return builder.getInt32(*(IS_CONSTANTINT(cmp->getOperand(0))->getValue().getRawData())+1);
		                                                else if(cmp->getPredicate()==llvm::ICmpInst::ICMP_ULE) return cmp->getOperand(0);
		                                                else if(cmp->getPredicate()==llvm::ICmpInst::ICMP_SLE) return cmp->getOperand(0);
		                                        }
		                                }
		                        }
		                }
		                return NULL;
		        }


			llvm::Instruction* getInstBeforeLoop()
			{
				for(llvm::Instruction& i : L->getLoopPreheader()->getInstList())
				{
					if(llvm::BranchInst* op = llvm::dyn_cast<llvm::BranchInst>(&i)) return op;
				}
				return NULL;
			}

//----
	void createCheckArrayBounds(llvm::GetElementPtrInst* ptr, llvm::Instruction* beforeLoop, bool complex=false, llvm::PHINode* phi=NULL)
	{
			llvm::AllocaInst* arr = llvm::dyn_cast<llvm::AllocaInst>(ptr->getOperand(0));

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

			auto atNullArr = [&]()
			{
				llvm::IRBuilder<> builder(ptr);
				llvm::Value* v = NULL;
				if(llvm::SExtInst* se = llvm::dyn_cast<llvm::SExtInst>(ptr->getOperand(ptr->getNumOperands()-1)))
				{
					builder.SetInsertPoint(se);
					v = se->getOperand(0);
				}
				else if(llvm::ZExtInst* ze = llvm::dyn_cast<llvm::ZExtInst>(ptr->getOperand(ptr->getNumOperands()-1)))
				{
					builder.SetInsertPoint(ze);
					v = ze->getOperand(0);
				}
				llvm::Value* args[] = {v,builder.getInt32(0)};
				builder.CreateCall(p.first,args);
				return;
			};

			if(!arr){
				atNullArr();
				return;
			}

                        llvm::PointerType* _p = arr->getType();
                        llvm::ArrayType* a = llvm::dyn_cast<llvm::ArrayType>(_p->getElementType());


                        std::function<llvm::Value* (llvm::IRBuilder<>&)> getNumElements = [&](llvm::IRBuilder<>& builder)
                        {
                                if(a) return llvm::dyn_cast<llvm::Value>(builder.getInt32(a->getNumElements()));
                                else {
					if(IS_CONSTANTINT(arr->getOperand(0))) return arr->getOperand(0);
					else if(IS_INSTRUCTION(arr->getOperand(0))) return IS_INSTRUCTION(arr->getOperand(0))->getOperand(0);
				}
                        };


			std::function<void (llvm::Instruction*,llvm::PHINode*,llvm::Value*)> insertAndReplace = [&](llvm::Instruction* I, llvm::PHINode* phi, llvm::Value* V)
			{
				for(unsigned int i = 0; i < I->getNumOperands(); i++)
				{
					if(I->getOperand(i)==phi) I->setOperand(i,V);
					else if(auto cli = IS_INSTRUCTION(I->getOperand(i)))
					{
						cli = cli->clone();
						I->setOperand(i,cli);
						insertAndReplace(cli,phi,V);
					}
				}
				I->insertBefore(beforeLoop);
			};

			if(complex)
			{
				llvm::Instruction* cloneI = NULL;
				if(1)
				{
					llvm::IRBuilder<> builder(beforeLoop);
					llvm::Value* v = NULL;
					if(llvm::SExtInst* se = llvm::dyn_cast<llvm::SExtInst>(ptr->getOperand(ptr->getNumOperands()-1)))
					{
						builder.SetInsertPoint(se);
						v = se->getOperand(0);
					}
					else if(llvm::ZExtInst* ze = llvm::dyn_cast<llvm::ZExtInst>(ptr->getOperand(ptr->getNumOperands()-1)))
					{
						builder.SetInsertPoint(ze);
						v = ze->getOperand(0);
					}

					cloneI = IS_INSTRUCTION(v)->clone();
					insertAndReplace(cloneI,phi,min);

					builder.SetInsertPoint(beforeLoop);
					llvm::Value* args[] = {cloneI,builder.getInt32(0)};
					builder.CreateCall(p.first,args);

				}
				if(max)
				{

					llvm::IRBuilder<> builder(beforeLoop);
					llvm::Value* v = NULL;
					if(llvm::SExtInst* se = llvm::dyn_cast<llvm::SExtInst>(ptr->getOperand(ptr->getNumOperands()-1)))
					{
						builder.SetInsertPoint(se);
						v = se->getOperand(0);
					}
					else if(llvm::ZExtInst* ze = llvm::dyn_cast<llvm::ZExtInst>(ptr->getOperand(ptr->getNumOperands()-1)))
					{
						builder.SetInsertPoint(ze);
						v = ze->getOperand(0);
					}
					builder.SetInsertPoint(beforeLoop);
					llvm::Value* args[] = {cloneI, getNumElements(builder)};
					builder.CreateCall(p.second,args);
				}
				else
				{
					llvm::IRBuilder<> builder(beforeLoop);
					llvm::Value* v = NULL;
					if(llvm::SExtInst* se = llvm::dyn_cast<llvm::SExtInst>(ptr->getOperand(ptr->getNumOperands()-1)))
					{
						builder.SetInsertPoint(se);
						v = se->getOperand(0);
					}
					else if(llvm::ZExtInst* ze = llvm::dyn_cast<llvm::ZExtInst>(ptr->getOperand(ptr->getNumOperands()-1)))
					{
						builder.SetInsertPoint(ze);
						v = ze->getOperand(0);
					}
					llvm::Value* args[] = {v,getNumElements(builder)};
					builder.CreateCall(p.second,args);
				}
				return;
			}

			if(min && max)
			{
//				std::cerr << "Min and Max" << std::endl;
				llvm::IRBuilder<> builder(beforeLoop);
				llvm::Value* args1[] = {min,builder.getInt32(0)};

				llvm::Value* args2[] = {max,getNumElements(builder)};
				args1[0] = IS_CONSTANTINT(args1[0]) ? builder.getInt32(*(IS_CONSTANTINT(args1[0])->getValue().getRawData())) : args1[0];
				args2[0] = IS_CONSTANTINT(args2[0]) ? builder.getInt32(*(IS_CONSTANTINT(args2[0])->getValue().getRawData())) : args2[0];

				builder.CreateCall(p.first,args1);
				builder.CreateCall(p.second,args2);
			}
			else if(min && !max)
			{
//				std::cerr << "Only Min" << std::endl;
				llvm::IRBuilder<> builder(beforeLoop);
				llvm::Value* args1[] = {min,builder.getInt32(0)};
				args1[0] = IS_CONSTANTINT(args1[0]) ? builder.getInt32(*(IS_CONSTANTINT(args1[0])->getValue().getRawData())) : args1[0];

				builder.CreateCall(p.first,args1);

				llvm::Value* v = NULL;
				if(llvm::SExtInst* se = llvm::dyn_cast<llvm::SExtInst>(ptr->getOperand(ptr->getNumOperands()-1)))
				{
					builder.SetInsertPoint(se);
					v = se->getOperand(0);
				}
				else if(llvm::ZExtInst* ze = llvm::dyn_cast<llvm::ZExtInst>(ptr->getOperand(ptr->getNumOperands()-1)))
				{
					builder.SetInsertPoint(ze);
					v = ze->getOperand(0);
				}
				llvm::Value* args2[] = {v,getNumElements(builder)};
				args2[0] = IS_CONSTANTINT(args2[0]) ? builder.getInt32(*(IS_CONSTANTINT(args2[0])->getValue().getRawData())) : args2[0];

				builder.CreateCall(p.second,args2);

			}
			else if(!min && max)
			{
///				std::cerr << "Only Max" << std::endl;
				llvm::IRBuilder<> builder(beforeLoop);
				llvm::Value* args2[] = {max,getNumElements(builder)};
				args2[0] = IS_CONSTANTINT(args2[0]) ? builder.getInt32(*(IS_CONSTANTINT(args2[0])->getValue().getRawData())) : args2[0];

				builder.CreateCall(p.second,args2);

				llvm::Value* v = NULL;
				if(llvm::SExtInst* se = llvm::dyn_cast<llvm::SExtInst>(ptr->getOperand(ptr->getNumOperands()-1)))
				{
					builder.SetInsertPoint(se);
					v = se->getOperand(0);
				}
				else if(llvm::ZExtInst* ze = llvm::dyn_cast<llvm::ZExtInst>(ptr->getOperand(ptr->getNumOperands()-1)))
				{
					builder.SetInsertPoint(ze);
					v = ze->getOperand(0);
				}
				llvm::Value* args1[] = {v,builder.getInt32(0)};
				args1[0] = IS_CONSTANTINT(args1[0]) ? builder.getInt32(*(IS_CONSTANTINT(args1[0])->getValue().getRawData())) : args1[0];

				builder.CreateCall(p.first,args1);

			}
			else if(!min && !max)
			{
//				std::cerr << "None" << std::endl;
				if(1)
				{
					llvm::IRBuilder<> builder(ptr);
					llvm::Value* v = NULL;
					if(llvm::SExtInst* se = llvm::dyn_cast<llvm::SExtInst>(ptr->getOperand(ptr->getNumOperands()-1)))
					{
						builder.SetInsertPoint(se);
						v = se->getOperand(0);
					}
					else if(llvm::ZExtInst* ze = llvm::dyn_cast<llvm::ZExtInst>(ptr->getOperand(ptr->getNumOperands()-1)))
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
					if(llvm::SExtInst* se = llvm::dyn_cast<llvm::SExtInst>(ptr->getOperand(ptr->getNumOperands()-1)))
					{
						builder.SetInsertPoint(se);
						v = se->getOperand(0);
					}
					else if(llvm::ZExtInst* ze = llvm::dyn_cast<llvm::ZExtInst>(ptr->getOperand(ptr->getNumOperands()-1)))
					{
						builder.SetInsertPoint(ze);
						v = ze->getOperand(0);
					}
					llvm::Value* args[] = {v,getNumElements(builder)};
					builder.CreateCall(p.second,args);
				}
			}


/*
		else{
			if(1)
			{

				llvm::IRBuilder<> builder(ptr);
				llvm::Value* v = isInstruction(ptr->getOperand(0))->getOperand(0);
				builder.SetInsertPoint(isInstruction(ptr));
				builder.CreateSExt(v,llvm::Type::getInt32Ty(llvm::getGlobalContext()));
				llvm::Value* args[] = {v,builder.getInt32(0)};
				builder.CreateCall(p.first,args);

			}

		}
*/
	}

//----
		public:
			bool operator()()
			{
				//valid loop
				if(!L)
				{
					std::cerr << "ERROR: NULL Loop" << std::endl;
					return true;
				}

				//valid phi var
				if(!(phi = getVar()))
				{
					//std::cerr << "ERROR: Can't detect loop variable" << std::endl;
					return true;
				}
				//check condition
				if(!(condition = getCondition()))
				{
					std::cerr << "WARNING: Loop without condition" << std::endl;
				}
				//check phi direction
				if((direction = getDirection(phi))==Direction::Unknown)
				{
					std::cerr << "WARNING: Loop direction is unknown" << std::endl;
				}
				if(!(beforeLoop = getInstBeforeLoop()))
				{
					std::cerr << "ERROR: Could not get instruction before the loop" << std::endl;
					return true;
				}

				//create META
	                        llvm::LLVMContext& C = llvm::getGlobalContext();
        	                llvm::MDNode* MTN = llvm::MDNode::get(C, llvm::MDString::get(C, "monotonicity"));

				//catch all Loop's blocks
				auto blocks = L->getBlocks();

	                        //try get smallest loop value
	                        min = (direction == Direction::Increasing) ? getMin(phi) : getMax(phi,condition);
	                        //trye get highest loop value
	                        max = (direction == Direction::Increasing) ? getMax(phi,condition) : getMin(phi);


				for(llvm::BasicBlock* bb : blocks)
				for(llvm::Instruction& I : *bb)
				{
					if(llvm::GetElementPtrInst* ge = IS_GETELEMENTPTRINST(&I))
					{

						llvm::Instruction* idx = IS_INSTRUCTION(ge->getOperand(ge->getNumOperands()-1));
						if(!idx) continue;
						idx = IS_INSTRUCTION(idx->getOperand(0));

                                                //check if index variable have the same direction of the loop
                                                if(getDirection(phi,idx) == direction)
                                                {

                                                        if(auto st = getStore(L,ge))
                                                        {
                                                                if(getDirection(phi,IS_INSTRUCTION(st->getOperand(0))) != direction)
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

                                                        //check if index variable is different from PHI and then call complex version of 'createCheckArrayBounds'
                                                        if(idx != phi)
                                                        {
                                                                createCheckArrayBounds(ge, beforeLoop,true,phi);
                                                        }
                                                        else createCheckArrayBounds(ge, beforeLoop);

                                                }

						else{

							if (phi->getMetadata("is.monotonic"))
							{
								phi->setMetadata("is.monotonic",NULL);
							}
							phi->setMetadata("not.monotonic.or.unknown",MTN);
							ge->setMetadata("not.monotonic.or.unknown",MTN);
							createCheckArrayBounds(ge, beforeLoop);

						}
					}
				}

				if(phi->getMetadata("is.monotonic"))
				{
					llvm::IRBuilder<> builder(llvm::getGlobalContext());
					builder.SetInsertPoint(beforeLoop);
					if(printf_function) builder.CreateCall(printf_function, msg);
					else std::cerr << "ERROR: printf call was not created" << std::endl;
				}
				else if(! (phi->getMetadata("not.monotonic.or.unknown")))
				{
					for(llvm::BasicBlock* b : L->blocks())
					{
						for(llvm::Instruction& i : *b)
						{
							if(llvm::GetElementPtrInst* ge = IS_GETELEMENTPTRINST(&i))
							{
								llvm::Instruction* idx = IS_INSTRUCTION(ge->getOperand(ge->getNumOperands()-1));
								if(!idx) continue;
								createCheckArrayBounds(ge, beforeLoop);
							}

						}
					}
				}

				return false;
			}


			MLDLoop(llvm::Loop* l): L(l)
			{}
	};


        //MLD PASS
        struct MLD: llvm::LoopPass
        {
                static char ID;

                MLD(): llvm::LoopPass(ID)
                {}

                //Loop Main Function
                virtual bool runOnLoop(llvm::Loop* L, llvm::LPPassManager &LPM)
                {
			MLDLoop mld(L);
			return !mld();
		}
	};

}

char MLD::ID=0;


// Public interface to the CFGSimplification pass
llvm::Pass *llvm::createMLDPass() {
        return new MLD();
}


static llvm::RegisterPass<MLD> X("mld", "Monotonic Loop Detection Pass", false, false);

