O ?= build
BIN ?= $(O)/boot.bin
CFLAGS := -g -O2 -nostdlib -ffreestanding -fno-strict-aliasing -mcpu=cortex-m4 -march=armv7e-m -mthumb
CROSS_COMPILER ?= arm-none-eabihf
ELF ?= $(O)/boot.elf
LNK ?= src/f446re.ld
SRCS := src/boot.S src/main.c src/f446re.c src/bin.S
SYMS ?= $(O)/boot.syms

.PHONY: all clean

all: $(BIN) $(SYMS)

$(BIN): $(ELF)
	$(CROSS_COMPILER)-objcopy $< --output-target binary $@

$(SYMS): $(ELF)
	$(CROSS_COMPILER)-objcopy $< --only-keep-debug $@

$(ELF): $(SRCS) $(LNK) | $(O)
	$(CROSS_COMPILER)-gcc $(CFLAGS) -T $(LNK) $(SRCS) -o $@

$(O):
	mkdir -p $@

clean:
	rm -f $(BIN) $(SYMS) $(ELF)