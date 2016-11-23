LLVM_SRC = ../llvm

all: clean
	@clang++-3.8 -std=c++11 -w -O0 -c -emit-llvm example2.cpp -o example.bc
	@opt -instnamer -mem2reg -break-crit-edges example.bc -o example.bc
	@clang++-3.8 -std=c++11 -O0 -w -c -emit-llvm -Xclang -load -Xclang $(LLVM_SRC)/build/lib/MonotonicLoopDetection.so example.bc
	@llvm-dis example.bc

clean:
	@rm -rf *.bc*
	@rm -rf *.ll*
	@rm -rf *.o*

