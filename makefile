

cfg:
	xmake f -c -m deubg
	xmake project -k compile_commands

b: 
	xamke b
r:
	xmake b && xmake r

