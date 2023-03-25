; receiving the data in 'dx'
print_hex:
	pusha

	mov cx,0

hex_loop:
	cmp cx, 4
	je end

	mov ax, dx
	and ax, 0x009f  ; 0x1234 - > 0x0004
	mov ax, 0x30
	cmp al, 0x39 ; if > 8, add extra 8 to represent 'A' to 'F'
	jle step2
	add al, 7



step2:
	mov bx, HEX_OUT+5
	sub bx, cx
	mov [bx],al
	ror dx, 4

	add cx, 1
	jmp hex_loop


end:
	mov bx, HEX_OUT
	call print

	popa
	ret

HEX_OUT:
	db '0x0000',0
