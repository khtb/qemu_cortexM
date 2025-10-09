#include <stdint.h>
#include "uart.h"
#include "FreeRTOS.h"
#include "task.h"


void vTaskCounter(void *pvParameters)
{
    for (;;)
    {
        char line[20] ;
        int x = uart_readLine(line, 64);
        // -1 is no new line
        if (x != -1)
        {
            uart_puts(line);
            uart_puts("\nOK\n");
        }
    }
}


int main(void)
{
    uart_init();
    uart_puts("Hello, MPS2 AN386 (Cortex-M4) via UART0!\n");
    xTaskCreate(vTaskCounter, "CounterTask", 128, NULL, tskIDLE_PRIORITY + 1, NULL);
    // Start scheduler
    vTaskStartScheduler();
}
