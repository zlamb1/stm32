O ?= build
BIN ?= $(O)/boot.bin
CFLAGS := -g -nostdlib -ffreestanding -fno-strict-aliasing -mcpu=cortex-m4 -march=armv7e-m -mthumb
CROSS_COMPILER ?= arm-none-eabihf
ELF ?= $(O)/boot.elf
SRCS := boot.S main.c f446re.c
SYMS ?= $(O)/boot.syms

.PHONY: all clean

all: $(BIN) $(SYMS)

$(BIN): $(ELF)
	$(CROSS_COMPILER)-objcopy $< --output-target binary $@

$(SYMS): $(ELF)
	$(CROSS_COMPILER)-objcopy $< --only-keep-debug $@

$(ELF): $(SRCS) linker.ld | $(O)
	$(CROSS_COMPILER)-gcc $(CFLAGS) -T linker.ld $(SRCS) -o $@

$(O):
	mkdir -p $@

clean:
	rm -f $(BIN) $(SYMS) $(ELF)