TARGET?=
STARTDIR=startup
SRCDIR?=
INCLUDES=include
SOURCES=$(wildcard $(STARTDIR)/*.c) $(wildcard $(STARTDIR)/*.s) $(wildcard $(SRCDIR)/*.c)
LINKSCR=$(SRCDIR)/stm32l433.ld
CFLAGS=-W -Wall -Wextra -Werror -Wno-error=sign-conversion -Wundef -Wshadow -Wdouble-promotion \
            -Wformat-truncation -fno-common -Wconversion \
            -g3 -Os -ffreestanding -ffunction-sections -fdata-sections -I$(INCLUDES) \
            -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32L433xx

LDFLAGS = -T$(LINKSCR) -nostartfiles -Wl,--gc-sections,--undefined=_init

CC = arm-none-eabi-gcc
LD = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

build : $(TARGET).bin

flash : $(TARGET).bin
	st-flash --reset write $< 0x8000000

$(TARGET).bin : $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

$(TARGET).elf : $(SOURCES)
	$(CC) $^ $(CFLAGS) $(LDFLAGS) -o $@

clean: 
	rm -rf $(TARGET).elf $(TARGET).bin
