all:
	@if ! grep -q "add_subdirectory(MLD)" llvm/lib/Transforms/CMakeLists.txt; then echo "add_subdirectory(MLD)" >> llvm/lib/Transforms/CMakeLists.txt; fi
	@mkdir -p llvm/lib/Transforms/MLD/
	@cp src/* llvm/lib/Transforms/MLD/
	@mkdir -p llvm/build
	@cd llvm/build; make -j8

start_llvm:
	@mkdir -p llvm/build
	@cd llvm/build; cmake ..; make -j8; make;

run:
	@rm -rf test/*.bc*
	@rm -rf test/*.ll*
	@rm -rf test/*.o*

	@cd test/; clang++ -std=c++11 -O0 -w -c -emit-llvm test1.cpp -o result1.bc; opt -mem2reg < result1.bc | llvm-dis > result1.ll; clang++ -std=c++11 -O0 -w -c -emit-llvm -Xclang -load -Xclang ../llvm/build/lib/MonotonicLoopDetection.so result1.ll; llvm-dis result1.bc -o result1.ll;
	@cd test/; clang++ -std=c++11 -O0 -w -c -emit-llvm success.cpp -o success.bc; opt -mem2reg < success.bc | llvm-dis > success.ll; clang++ -std=c++11 -O0 -w -c -emit-llvm -Xclang -load -Xclang ../llvm/build/lib/MonotonicLoopDetection.so success.ll; llvm-dis success.bc -o success.ll;


clean:
	@rm -rf test/*.ll test/*.bc
