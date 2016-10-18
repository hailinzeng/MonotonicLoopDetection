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

	@echo "##############"
	@echo "TEST @:"
	@echo "##############"

	@cd test/; clang++ -O0 -w -c -emit-llvm test1.cpp -o result1.bc; opt -mem2reg < result1.bc | llvm-dis > result1.ll; clang++ -O0 -w -c -emit-llvm -Xclang -load -Xclang ../llvm/build/lib/MonotonicLoopDetection.so result1.ll; llvm-dis result1.bc -o result1.ll;


clean:
	@rm -rf test/result*
