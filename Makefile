all:

#	@if ! grep -q "add_subdirectory(MLD)" llvm/lib/Transforms/CMakeLists.txt; then echo "add_subdirectory(MLD)" >> llvm/lib/Transforms/CMakeLists.txt; fi
#	@rm -rf llvm/lib/Transforms/MLD/
#	@mkdir -p llvm/lib/Transforms/MLD/
#	@cp src/* llvm/lib/Transforms/MLD/

#By now we are doing everything related to MLD test in run
#	@rm -rf llvm/test/MLD/
#	@cp -r test/MLD/ llvm/test/

	@cd ./build; make;

start_llvm:

	@mkdir -p ./build
	@cd ./build; cmake -G "Unix Makefiles" ../llvm;

run:

	@cp lit.cfg llvm/test/
	@rm -rf llvm/test/MLD/*
	@mkdir -p llvm/test/MLD
	@cp test/*.cpp llvm/test/MLD/
	@cp test/*.c llvm/test/MLD/
	@cd llvm/test; ../../build/bin/llvm-lit -sv MLD/*.cpp MLD/*.c


clean:
	@cd test/ && make clean
