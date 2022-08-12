CROSS_COMPILE ?= arm-linux-gnueabihf-
NAME		  ?= led

CC 		:= $(CROSS_COMPILE)gcc
LD		:= $(CROSS_COMPILE)ld
OBJCOPY := $(CROSS_COMPILE)objcopy
OBJDUMP	:= $(CROSS_COMPILE)objdump

obj := start.o main.o

$(NAME).bin:$(obj)
	$(LD) -Timx6u.lds -o $(NAME).elf $^
	$(OBJCOPY) -O binary -S $(NAME).elf $@
	$(OBJDUMP) -D -m arm $(NAME).elf > $(NAME).dis
 
%.o:%.c
	$(CC) -Wall -nostdlib -c $< -o2 -o $@

%.o:%.S
	$(CC) -Wall -nostdlib -c $< -o2 -o $@

clean:
	rm -rf *.o $(NAME).bin $(NAME).elf $(NAME).dis

.PHONY:clean