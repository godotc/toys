_data:
	.global _start

	.code16

	.text
_start:
	jmp $0, $offset

offset:
	mov $0, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %ss
	mov %ax, %gs
	mov %ax, %fs

	jmp .
	
	.org 0x1fe
	.byte 0x55, 0xaa
