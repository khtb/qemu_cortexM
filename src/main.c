#include <stdint.h>
#include "uart.h"


int main(void)
{
    uart_init();
    uart_puts("Hello, MPS2 AN386 (Cortex-M4) via UART0!\n");
    for (;;)
    {
        int c = uart_getchar();
        if (c != -1)
        {
            // Do something with received data
            if (c == '\r' || c == '\n')
            {
                const char *ok = " [OK]\r\n";
                for (const char *p = ok; *p; p++)
                {
                    while (UART0->STATE & UART_STATE_TXFULL);
                    UART0->DATA = *p;
                }
            }
        } /* loop forever */
    }
}
