[org 0x7c00]
mov ah, 0x0e ; tty 


; Failed for print the memory addresss
mov al, "1"
int 0x10
mov al, the_sercret
int 0x10

mov al, "2"
int 0x10
mov al, [the_sercret]
int 0x10


; add Bios starting offset 0x7c00 to the memory  of X
mov al, "3"
int 0x10
mov bx, the_sercret
add bx, 0x7c00
mov al, [bx]
int 0x10

mov al, "4"
int 0x10
mov al, [0x7c2d]
int 0x10

;jmp $ ; loop

the_sercret:
	db "x"

times 510-($-$$) db 0
dw 0xaa55
