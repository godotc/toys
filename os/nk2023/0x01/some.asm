#define SECT_SIZE 512

.global _start

.section .data
msg:
    .asciz "hello nk world!\n"

.section .text
.code16
_start:
    lea msg, %si

again:
    mov (%si), %al
    inc %si
    or %al, %al
    jz done
    mov $0x0e, %ah
    mov $0x00, %bh
    int $0x10
    jmp again

done:
    jmp label

label:
    nop

.org SECT_SIZE - 2
