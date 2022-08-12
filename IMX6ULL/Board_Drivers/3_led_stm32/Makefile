obj = start.o main.o
gcc = arm-linux-gnueabihf-gcc
ld = arm-linux-gnueabihf-ld
objcopy = arm-linux-gnueabihf-objcopy
objdump = arm-linux-gnueabihf-objdump

led.bin:$(obj)
	$(ld) -Timx6u.lds -o led.elf $^
	$(objcopy) -O binary -S led.elf $@
	$(objdump) -D -m arm led.elf > led.dis

%.o:%.c
	$(gcc) -Wall -nostdlib -c -o2 $< -o $@

%.o:%.S
	$(gcc) -Wall -nostdlib -c -o2 $< -o $@


clean:
	rm -rf *.o led.bin led.elf led.dis

.PHONY:clean
