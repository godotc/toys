loop:
	jmp loop

times 500-($-$$) db 0
dw 0xaa55
