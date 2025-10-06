

#include <stdint.h>
#include "uart.h"

volatile char rxBuffer[RX_BUF_SIZE];
volatile uint32_t rxHead = 0, rxTail = 0;


void uart_puts(const char *s);
void uart_putc(char c);



void uart_init(void)
{
    /* Any BAUDDIV >= 16 is acceptable for QEMU; exact baud not critical */
    UART0->BAUDDIV = 16;
    UART0->CTRL = UART_CTRL_TXEN | UART_CTRL_RXEN | UART_RX_IRQ_EN; /* enable TX */
    // Enable interrupt in NVIC
    NVIC_EnableIRQ(UART0_IRQn);
}

void uart_irq(void)
{
    volatile uint32_t reg = UART0->INTSTATUS;
    if (UART0->INTSTATUS & UART0_INT_RX)
    {
        char c = UART0->DATA & 0xFF; // read received data
        uint32_t next = (rxHead + 1) % RX_BUF_SIZE;
        if (next != rxTail)
        {
            rxBuffer[rxHead] = c;
            rxHead = next;
        }

        if (c == '\r' || c == '\n') {
            uart_puts("\r\n");
        }
        // Echo back (blocking)
        if (c == 0x08 || c == 0x7F)
        { // Backspace or Delete
            // Erase previous char visually
            uart_puts("\b \b");
        }
        else
        {
            uart_putc(c); // Normal echo
        }
        // Clear RX interrupt
        UART0->INTSTATUS = UART0_INT_RX;
    }
}

void uart_putc(char c)
{
    while (UART0->STATE & UART_STATE_TXFULL)
    { /* wait */
    }
    UART0->DATA = (uint32_t)c;
}

void uart_puts(const char *s)
{
    while (*s)
    {
        if (*s == '\n')
            uart_putc('\r');
        uart_putc(*s++);
    }
}


void uart_print(const char *s)
{
    uart_puts(s);
}

// Simple getchar using buffer
int uart_getchar(void)
{
    if (rxHead == rxTail)
        return -1; // no data

    char c = rxBuffer[rxTail];
    rxTail = (rxTail + 1) % RX_BUF_SIZE;
    return c;
}


int uart_readLine(char * line, int max_len)
{
    static int len = 0;
    int c;
    while ((c = uart_getchar()) != -1)
    {
        if (c == '\n' || c == '\r')
        {
            line[len] = '\0';
            int result = len;
            len = 0;
            return result;
        }
        else if (c == '\b' || c == 0x7F)
        {
            if (len > 0)
            {
                len--;
            }
        }
        else if (len < max_len - 1)
        {
            line[len++] = c;
            line[len] = '\0';
        }
        else
        {
            // error
        }
    }
}