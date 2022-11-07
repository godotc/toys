

run:
	bash ./run.sh

build:
	bash ./build.sh



linux:
	rm build -rf
	cmake -B build \
		-D CMAKE_INCLUDE_PATH=/msys64/mingw64/include \
		-G "Unix Makefiles"
	rm ./compile_commands.json
	ln -s ./build/compile_commands.json 

#-D MING64PREFIX=/msys64/mingw64/lib/cmake \
#-D CMAKE_C_COMPILER=/mingw64/bin/gcc.exe \
#-D CMAKE_CXX_COMPILER=/mingw64/bin/g++.exe \



win:
	del build 
	cmake -B build  \
		-G "Unix Makefiles" \
		-D CMAKE_TOOLCHAIN_FILE=D:/program/vcpkg/vcpkg/scripts/buildsystems/vcpkg.cmake \
		
