all:
	@if ! grep -q "add_subdirectory(MLD)" llvm/lib/Transforms/CMakeLists.txt; then echo "add_subdirectory(MLD)" >> llvm/lib/Transforms/CMakeLists.txt; fi

	@rm -rf llvm/lib/Transforms/MLD/
	@mkdir -p llvm/lib/Transforms/MLD/
	@cp src/* llvm/lib/Transforms/MLD/

#By now we are doing everything related to MLD test in run
#	@rm -rf llvm/test/MLD/
#	@cp -r test/MLD/ llvm/test/

	@mkdir -p llvm/build
	@cd llvm/build; cmake ..; make -j2;

start_llvm:
	@mkdir -p llvm/build
	@cd llvm/build; cmake ..;

run:

	@cp lit.cfg llvm/test/
	@rm -rf llvm/test/MLD/*
	@mkdir -p llvm/test/MLD
	@cp test/*.cpp llvm/test/MLD/
	@cd llvm/test; ../build/bin/llvm-lit -sv MLD/*.cpp


clean:
	@cd test/ && make clean
