FORMAT_FILE := $(shell find src -name "*.cpp" -or -name "*.hpp" -type f)

.PHONY:init  debug build clean

init:
	cmake -S . -B build
	ln -s  build/compile_commands_json compile_commands_json

build:
	cmake -S . -B build
	cmake --build build --verbose

debug:
	cmake -DCMAKE_BUILD_TYPE=Debug -S . -B build
	cmake --build build --verbose

run:
	build/main


format:
	clang-format -i ${FORMAT_FILE}

clean:
	rm -rf build
