#include <stdint.h>
#include "uart.h"


int main(void) {
    uart_init();
    uart_puts("Hello, MPS2 AN386 (Cortex-M4) via UART0!\n");
    for (;;); /* loop forever */
}

