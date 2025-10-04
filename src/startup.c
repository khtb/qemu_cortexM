
#include <stdint.h>
#include "uart.h"


/* forward decls */
void Reset_Handler(void);
void Default_Handler(void);

void NMI_Handler(void)       __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
/* linker symbols */
extern uint32_t _sidata, _sdata, _edata, _sbss, _ebss, _estack;
extern void main(void);


// NVIC registers (simple version)
#define NVIC_ISER0       (*((volatile uint32_t *)0xE000E100))
#define NVIC_EnableIRQ(irq)   (NVIC_ISER0 = (1 << (irq)))

/* vector table at 0x00000000 */
__attribute__((section(".isr_vector")))
void (* const vector_table[])(void) = {
    (void (*)(void))(&_estack),  /* initial SP */
    Reset_Handler,
    NMI_Handler,
    HardFault_Handler,
    uart_irq
};






void Reset_Handler(void) {
    /* copy .data from FLASH to RAM */
    uint32_t *src = &_sidata, *dst = &_sdata;
    while (dst < &_edata) *dst++ = *src++;
    /* zero .bss */
    for (dst = &_sbss; dst < &_ebss; ) *dst++ = 0;
    /* jump to main */
    (void)main();
    for(;;);
}

void Default_Handler(void) { for(;;); }