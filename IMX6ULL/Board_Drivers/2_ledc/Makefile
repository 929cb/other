obj = start.o main.o

led.bin:$(obj)
	arm-linux-gnueabihf-ld -Timx6u.lds $^ -o led.elf
	arm-linux-gnueabihf-objcopy -O binary -S led.elf $@
	arm-linux-gnueabihf-objdump -D -m arm led.elf > led.dis

%.o:%.c
	arm-linux-gnueabihf-gcc -Wall -nostdlib -o2 -c $< -o $@

%.o:%.S
	arm-linux-gnueabihf-gcc -Wall -nostdlib -o2 -c $< -o $@

clean:
	rm -rf *.o led.bin led.elf led.dis