


#include <stdint.h>
#include "uart.h"


volatile char rxBuffer[RX_BUF_SIZE];
volatile uint32_t rxHead = 0, rxTail = 0;

void uart_init(void) {
    /* Any BAUDDIV >= 16 is acceptable for QEMU; exact baud not critical */
    UART0->BAUDDIV = 16;
    UART0->CTRL = UART_CTRL_TXEN | UART_CTRL_RXEN | UART_RX_IRQ_EN  ;  /* enable TX */
        // Enable interrupt in NVIC
}

void uart_irq(void)
{
	
}

static void uart_putc(char c) {
    while (UART0->STATE & UART_STATE_TXFULL) { /* wait */ }
    UART0->DATA = (uint32_t)c;
}



void uart_puts(const char *s) {
    while (*s) { if (*s == '\n') uart_putc('\r'); uart_putc(*s++); }
}


