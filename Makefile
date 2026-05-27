O ?= build
BIN ?= $(O)/boot.bin
CFLAGS := -g -O2 -Iinclude -nostdlib -ffreestanding -fno-strict-aliasing -mcpu=cortex-m4 -march=armv7e-m -mthumb
CROSS_COMPILER ?= arm-none-eabihf-
CROSS_CC ?= $(CROSS_COMPILER)cc
CROSS_OBJCOPY ?= $(CROSS_COMPILER)objcopy
COMPILE_COMMANDS ?= $(O)/compile_commands.json
ELF ?= $(O)/boot.elf
LNK ?= src/f446re.ld
ASM_SRCS ?= src/f446re/boot.S src/bin.S
SRCS := src/main.c src/f446re/clock.c src/f446re/gpio.c
SYMS ?= $(O)/boot.syms
OBJS := $(ASM_SRCS:%.S=$(O)/%.o) $(SRCS:%.c=$(O)/%.o)
DEPS := $(OBJS:.o=.d)

.PHONY: all cc clean

all: $(BIN) $(SYMS)

cc: clean $(COMPILE_COMMANDS)

$(COMPILE_COMMANDS):
	bear --output $@ -- make

$(BIN): $(ELF)
	$(CROSS_OBJCOPY) $< --output-target binary $@

$(SYMS): $(ELF)
	$(CROSS_OBJCOPY) $< --only-keep-debug $@

$(ELF): $(OBJS) $(LNK)
	$(CROSS_CC) $(CFLAGS) -T $(LNK) $(OBJS) -o $@

$(O)/%.o: %.S
	mkdir -p `dirname $@`
	$(CROSS_CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(O)/%.o: %.c
	mkdir -p `dirname $@`
	$(CROSS_CC) $(CFLAGS) -MMD -MP -c $< -o $@

clean:
	rm -f $(BIN) $(SYMS) $(ELF) $(OBJS) $(DEPS) $(COMPILE_COMMANDS)

-include $(DEPS)