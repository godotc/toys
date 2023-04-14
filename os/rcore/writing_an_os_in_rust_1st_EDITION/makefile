ARCH ?= x86_64
KERNEL := build/kernel-$(ARCH).bin
ISO := ./build/os-$(ARCH).iso

LINKER_SCRIPT := src/arch/$(ARCH)/linker.ld
GRUB_CFG := src/arch/$(ARCH)/grub.cfg

ASSEMBLY_SOURCE_FILES := $(wildcard src/arch/$(ARCH)/*.asm) #wildcard: find all  qualified files

# patsubst: translate from `src/arch/$(ARCH)/XYZ.asm`  to `build/arch/$(ARCH)/XYZ.o`
ASSEMBLY_OBJECT_FILES := $(patsubst src/arch/$(ARCH)/%.asm,  \
							build/arch/$(ARCH)/%.o, $(ASSEMBLY_SOURCE_FILES))



.PHONY: all clean run iso

all:$(KERNEL)

run: $(ISO)
	qemu-system-x86_64 -cdrom $(ISO)

iso: $(ISO)

clean:
	@rm -r build

$(ISO): $(KERNEL) $(GRUB_CFG)
	@mkdir -p build/isofiles/boot/grub
	cp $(KERNEL) build/isofiles/boot/kernel.bin
	cp $(GRUB_CFG) build/isofiles/boot/grub
	grub-mkrescue -o $(ISO)  build/isofiles #2> /dev/null
	rm -r build/isofiles

$(KERNEL): $(ASSEMBLY_OBJECT_FILES) $(LINKER_SCRIPT)
	ld -n -t $(LINKER_SCRIPT) -o $(KERNEL) $(ASSEMBLY_OBJECT_FILES)

build/arch/$(ARCH)/%.o: src/arch/$(ARCH)/%.asm
	mkdir -p $(shell dirname $@)
	# shit variables: https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html  
	nasm -felf64 $< -o $@ 






