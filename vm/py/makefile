cfg:
	xmake f -m debug
	xmake project -k compile_commands

r:
	xmake b $(T)
	xmake r $(T)




bytecodes:
	python -m compileall res/bytecodes/read_bytecode_test.py
	hexdump res/bytecodes/__pycache__/read_bytecode_test.*.pyc > res/bytecodes/hexdump.txt
	xxd res/bytecodes/__pycache__/read_bytecode_test.*.pyc > res/bytecodes/xxd.txt
	

