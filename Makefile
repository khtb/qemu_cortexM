PROJECT = app
SRCDIR = src
OUTDIR = out

LD      = $(CC)
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
CFLAGS  = -mcpu=cortex-m4 -mthumb -Os -ffreestanding -fno-builtin -nostdlib
LDFLAGS = -T linker.ld -nostartfiles -Wl,--gc-sections,-Map=$(OUTDIR)/$(PROJECT).map

SRC     := $(wildcard $(SRCDIR)/*.c)
OBJ     := $(patsubst $(SRCDIR)/%.c,$(OUTDIR)/%.o,$(SRC))
ELF     := $(OUTDIR)/$(PROJECT).elf

all: $(ELF)

$(OUTDIR)/%.o: $(SRCDIR)/%.c | $(OUTDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(ELF): $(OBJ) linker.ld
	$(LD) $(CFLAGS) $(OBJ) -o $@ $(LDFLAGS)

$(OUTDIR):
	mkdir -p $(OUTDIR)

clean:
	rm -rf $(OUTDIR)

run: $(ELF)
	#qemu-system-arm -M cortex-r52 -cpu cortex-r52 -nographic -semihosting -kernel $(ELF)
	qemu-system-arm -M mps2-an386 -cpu cortex-m4 -nographic -kernel $(ELF)
