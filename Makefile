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
	@echo "##############"
	@echo "TEST 1:"
	@echo "##############"

	@clang++ -O0 -w -c -emit-llvm test/test1.cpp -o test/result1.bc
	@opt -mem2reg < test/result1.bc | llvm-dis > test/result1.ll
	@clang++ -O0 -w -c -emit-llvm -Xclang -load -Xclang llvm/build/lib/MonotonicLoopDetection.so test/result1.ll

	@echo "##############"
	@echo "TEST 2:"
	@echo "##############"


	@clang++ -O0 -w -c -emit-llvm test/test2.cpp -o test/result2.bc
	@opt -mem2reg < test/result2.bc | llvm-dis > test/result2.ll
	@clang++ -O0 -w -c -emit-llvm -Xclang -load -Xclang llvm/build/lib/MonotonicLoopDetection.so test/result2.ll


clean:
	@rm -rf test/result*
