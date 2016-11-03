all:
	@if ! grep -q "add_subdirectory(MLD)" llvm/lib/Transforms/CMakeLists.txt; then echo "add_subdirectory(MLD)" >> llvm/lib/Transforms/CMakeLists.txt; fi

	@rm -rf llvm/lib/Transforms/MLD/
	@mkdir -p llvm/lib/Transforms/MLD/
	@cp src/* llvm/lib/Transforms/MLD/

	@cp -r test/MLD/ llvm/test/
	@mkdir -p llvm/build
	@cd llvm/build; make -j8;

start_llvm:
	@mkdir -p llvm/build
	@cd llvm/build; cmake ..; make -j8; make;

run:
	@rm -rf test/*.bc*
	@rm -rf test/*.ll*
	@rm -rf test/*.o*

	@cd test/ && make

	@cd llvm/test/MLD/; ../../build/bin/llvm-lit -v test.ll

clean:
	@cd test/ && make clean
