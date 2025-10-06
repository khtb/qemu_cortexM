PROJECT = app
SRCDIR = src
OUTDIR = out

CC = arm-none-eabi-gcc
LD      = $(CC)
OBJCOPY = arm-none-eabi-objcopy
CFLAGS  = -mcpu=cortex-m4 -mthumb -O0 -g -ffreestanding -fno-builtin -nostdlib
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
	qemu-system-arm -M mps2-an386 -cpu cortex-m4 -nographic -kernel $(ELF)

debug: $(ELF)
	qemu-system-arm -M mps2-an386 -cpu cortex-m4 -nographic -kernel $(ELF) -S -gdb tcp::1234

diss: 
	arm-none-eabi-objdump -d -C $(ELF) > $(OUTDIR)/$(PROJECT).diss
