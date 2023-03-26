
TOOL_PREFIX = x86_64-linux-gnu-


CFLAGS = -g -c -O0 -m32 -fno-pie -fno-stack-protector -nostdlib -nostdinc


all: src/os.c src/os.h src/start.S
	$(TOOL_PREFIX)gcc $(CFLAGS) src/start.S
	$(TOOL_PREFIX)gcc $(CFLAGS) src/os.c
	ld -m elf_i386 -Ttext=0x7c00 start.o os.o -o os.elf
	#
	objcopy  -O binary os.elf  os.bin
	objdump  -x -d -S os.elf > os_dis.txt
	readelf  -a os.elf > os_elf.txt
	dd if=os.bin of=./image/disk.img conv=notrunc

test:
	qemu-system-i386 -m 128M -s -S -drive file=image/disk.img,index=0,media=disk,form=raw -nographic

clean:
	rm -f *.elf *.o *.txt *.bin
