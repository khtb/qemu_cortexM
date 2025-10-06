
#include <stdint.h>
#include "uart.h"


void Reset_Handler(void);
void Default_Handler(void);

void NMI_Handler(void)       __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler")));



/* linker symbols */
extern uint32_t _sidata, _sdata, _edata, _sbss, _ebss, _estack;
extern void main(void);



extern void __main(void);
/* vector table at 0x00000000 */
__attribute__((section(".isr_vector"))) void (*const vector_table[])(void) = {
    (void (*)(void))(&_estack), /* initial SP */
    Reset_Handler,
    NMI_Handler,
    HardFault_Handler,
    [16 + 0] = uart_irq, // UART0 interrupt
};
extern void __libc_init_array();
void Reset_Handler(void) {

    #if 0
    __libc_init_array();
    #else
    /* copy .data from FLASH to RAM */
    uint32_t *src = &_sidata, *dst = &_sdata;
    while (dst < &_edata) *dst++ = *src++;
    /* zero .bss */
    for (dst = &_sbss; dst < &_ebss; ) *dst++ = 0;
    /* jump to main */
    (void)main();
    for(;;);
    #endif
}

void Default_Handler(void) { for(;;); }