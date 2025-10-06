MEMORY
{
  FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 4M
  RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 4M
}

/* initial stack pointer */
_estack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
  .isr_vector ORIGIN(FLASH) : 
  { 
    KEEP(*(.isr_vector)) 
  } > FLASH

  .text : {
    *(.text*) *(.rodata*)
  } > FLASH

  .data : AT (ADDR(.text) + SIZEOF(.text)) {
    _sdata = .; 
    *(.data*) _edata = .;
  } > RAM
  _sidata = LOADADDR(.data);

  .bss (NOLOAD) : {
    _sbss = .; 
    *(.bss*) _ebss = .;
  } > RAM
}
