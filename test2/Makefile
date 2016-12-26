
all:
	@../build/bin/clang++ -std=c++11 -w -O0 -c -emit-llvm example.cpp -o example.bc
	@../build/bin/opt -loop-extract  -instnamer -mem2reg -break-crit-edges example.bc -o example.bc
	@../build/bin/opt -mld example.bc -o example.bc
	@../build/bin/llvm-dis example.bc

clean:
	@rm -rf *.bc*
	@rm -rf *.ll*
	@rm -rf *.o*

