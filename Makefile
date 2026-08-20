.PHONY:  debug build clean

build:
	cmake -S . -B build
	cmake --build build --verbose

debug:
	cmake -DCMAKE_BUILD_TYPE=Debug -S . -B build
	cmake --build build --verbose

clean:
	rm -rf build
