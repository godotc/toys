


help:
	@echo run
	@echo build
	@echo linux
	@echo wsl
	@echo win

run:
	alias r="make run"
	bash ./script/run.sh

build:
	@bash ./script/build.sh


linux:
	rm build -rf
	cmake -B build \
		-G "Unix Makefiles" \
	rm ./compile_commands.json
	ln -s ./build/compile_commands.json 


wsl:
	rm build -rf
	cmake -B build \
		-G "Unix Makefiles"
	rm ./compile_commands.json
	ln -s ./build/compile_commands.json 


win:
	del build 
	cmake -B build  \
		-G "Unix Makefiles" \
		-D CMAKE_TOOLCHAIN_FILE=D:/program/vcpkg/vcpkg/scripts/buildsystems/vcpkg.cmake \
		
