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

clean:
	rm -rf build
