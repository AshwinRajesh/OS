# $@ = target file
# $< = first dependency
# $^ = all dependencies

C_SOURCES = $(wildcard *.c)
HEADERS = $(wildcard *.h)
OBJ = ${C_SOURCES:.c=.o interrupt.o}

# First rule is the one executed when no parameters are fed to the Makefile
all: run

# Notice how dependencies are built as needed
kernel.bin: kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

kernel.elf: kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^ 

os_image.bin: boot_main.bin kernel.bin
	cat $^ > $@
	qemu-img resize $@ +32K

run: os_image.bin
	qemu-system-i386 $< -boot c

debug: os_image.bin kernel.elf
	qemu-system-i386 -s -fda os_image.bin &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

%.o: %.c ${HEADERS}
	i386-elf-gcc -ffreestanding -g -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

%.dis: %.bin
	ndisasm -b 32 $< > $@

clean:
	rm -rf *.bin *.dis *.o os_image.bin *.elf

