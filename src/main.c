#include <stdint.h>





/* ---- CMSDK UART0 on MPS2-AN386 ---- */
#define UART0_BASE 0x40004000u /* APB UART0 base */
typedef struct {
    volatile uint32_t DATA;      /* 0x00 */
    volatile uint32_t STATE;     /* 0x04 */
    volatile uint32_t CTRL;      /* 0x08 */
    volatile uint32_t INTSTATUS; /* 0x0C (also INTCLEAR on write) */
    volatile uint32_t BAUDDIV;   /* 0x10 */
} CMSDK_UART;

#define UART0 ((CMSDK_UART *)UART0_BASE)
#define UART_STATE_TXFULL (1u << 0)
#define UART_CTRL_TXEN    (1u << 0)
#define UART_CTRL_RXEN    (1u << 1)

static void uart_init(void) {
    /* Any BAUDDIV >= 16 is acceptable for QEMU; exact baud not critical */
    UART0->BAUDDIV = 16;
    UART0->CTRL = UART_CTRL_TXEN;  /* enable TX */
}

static void uart_putc(char c) {
    while (UART0->STATE & UART_STATE_TXFULL) { /* wait */ }
    UART0->DATA = (uint32_t)c;
}

static void uart_puts(const char *s) {
    while (*s) { if (*s == '\n') uart_putc('\r'); uart_putc(*s++); }
}

int main(void) {
    uart_init();
    uart_puts("Hello, MPS2 AN386 (Cortex-M4) via UART0!\n");
    for (;;); /* loop forever */
}

