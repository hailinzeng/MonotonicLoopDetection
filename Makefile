all:
	@if ! grep -q "add_subdirectory(MLD)" llvm/lib/Transforms/CMakeLists.txt; then echo "add_subdirectory(MLD)" >> llvm/lib/Transforms/CMakeLists.txt; fi
	@mkdir -p llvm/lib/Transforms/MLD/
	@cp src/* llvm/lib/Transforms/MLD/
	@cp -r test/MLD llvm/test/
	@mkdir -p llvm/build
	@cd llvm/build; make -j8;

start_llvm:
	@mkdir -p llvm/build
	@cd llvm/build; cmake ..; make -j8; make;

run:
	@rm -rf test/*.bc*
	@rm -rf test/*.ll*
	@rm -rf test/*.o*

	@cd test/; clang++ -std=c++11 -O0 -w -c -emit-llvm example.cpp -o example.bc; opt -mem2reg < example.bc | llvm-dis > example.ll; clang++ -std=c++11 -O0 -w -c -emit-llvm -Xclang -load -Xclang ../llvm/build/lib/MonotonicLoopDetection.so example.ll; llvm-dis example.bc -o example.ll;

	@#cd llvm/test/MLD/; ../../build/bin/llvm-lit -v test.ll

clean:
	@rm -rf test/*.ll test/*.bc
