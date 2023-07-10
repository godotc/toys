ifeq ($(OS),Windows_NT)
py2=python.bat
else
py2=python2
endif

ev:
	print($(OS))

cfg:
	xmake f -m debug
	xmake project -k compile_commands

r:
	xmake b $(T)
	xmake r $(T)




bytecodes:
	python -m compileall res/bytecodes/read_bytecode_test.py
	hexdump res/bytecodes/__pycache__/read_bytecode_test.*.pyc > res/bytecodes/hexdump_py3.txt
	xxd res/bytecodes/__pycache__/read_bytecode_test.*.pyc > res/bytecodes/xxd_py3.txt

	$(py2) -m compileall res/bytecodes/read_bytecode_test.py
	hexdump res/bytecodes/read_bytecode_test.pyc > res/bytecodes/hexdump_py2.txt
	xxd res/bytecodes/read_bytecode_test.pyc > res/bytecodes/xxd_py2.txt
	

t2:
	./bin/py.exe ./res/bytecodes/read_bytecode_test.pyc
	
t3:
	./bin/py.exe ./res/bytecodes/__pycache__/read_bytecode_test.cpython-310.pyc

