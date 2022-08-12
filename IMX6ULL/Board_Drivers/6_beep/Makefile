CROSS_COMPILE ?= arm-linux-gnueabihf-
NAME		  ?= beep

CC 		:= $(CROSS_COMPILE)gcc
LD		:= $(CROSS_COMPILE)ld
OBJCOPY := $(CROSS_COMPILE)objcopy
OBJDUMP	:= $(CROSS_COMPILE)objdump

INCLUDIRS	:=	imx6u \
				bsp/clk \
				bsp/led \
				bsp/delay \
				bsp/beep

SRCDIRS		:=	project \
				bsp/clk \
				bsp/led \
				bsp/delay \
				bsp/beep

INCLUDE		:=	$(patsubst %, -I %, $(INCLUDIRS))

SFILES		:=	$(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.S))
CFILES		:=	$(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))

SFILESNDIR	:=	$(notdir $(SFILES))
CFILESNDIR	:=	$(notdir $(CFILES))

SOBJS		:=	$(patsubst %.S, obj/%.o, $(SFILESNDIR))
COBJS		:=	$(patsubst %.c, obj/%.o, $(CFILESNDIR))

OBJS		:=	$(SOBJS)$(COBJS)

VPATH		:=	$(SRCDIRS)


$(NAME).bin:$(OBJS)
	$(LD) -Timx6u.lds -o $(NAME).elf $^
	$(OBJCOPY) -O binary -S $(NAME).elf $@
	$(OBJDUMP) -D -m arm $(NAME).elf > $(NAME).dis
 
$(COBJS):obj/%.o:%.c
	$(CC) -Wall -nostdlib -c -o2 $(INCLUDE) -o $@ $<

$(SOBJS):obj/%.o:%.S
	$(CC) -Wall -nostdlib -c -o2 $(INCLUDE) -o $@ $<

clean:
	rm -rf obj/*.o $(NAME).bin $(NAME).elf $(NAME).dis

print:
	@echo INCLUDE = $(INCLUDE)
	@echo SFILES = $(SFILES)
	@echo CFILES = $(CFILES)
	@echo SFILESNDIR = $(SFILESNDIR)
	@echo CFILESNDIR = $(CFILESNDIR)
	@echo SOBJS = $(SOBJS)
	@echo COBJS = $(COBJS)
	@echo OBJS = $(OBJS)


.PHONY:clean print