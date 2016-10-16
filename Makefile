all:
	@if ! grep -q "add_subdirectory(MLD)" llvm/lib/Transforms/CMakeLists.txt; then echo "add_subdirectory(MLD)" >> llvm/lib/Transforms/CMakeLists.txt; fi
	@mkdir -p llvm/lib/Transforms/MLD/
	@cp src/* llvm/lib/Transforms/MLD/
	@mkdir -p llvm/build
	@cd llvm/build; make -j8

start_llvm:
	@mkdir -p llvm/build
	@cd llvm/build; cmake ..; make -j8; make;
