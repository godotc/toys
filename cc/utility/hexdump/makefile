
cfg:
	xmake f -c -m deubg
	xmake project -k compile_commands

b: 
	xmake b
r:
	xmake b && xmake r 


test:
	xmake b
	xmake r hexdump -f ../xmake.lua 


