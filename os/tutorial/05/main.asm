[org 0x7c00] ; tell the assembler the offset is boostsector code


; data
HELLO:
	db 'Hello world', 0
GOODBYE:
	db 'Goodbye', 0


mov bx, HELLO
call print

call print_nl

mov dx, 0x12fe
call print_hex

jmp $

%include "print.asm"
%include "print_hex.asm"

times 510-($-$$) db 0
dw 0xaa55
