#ifndef _UART_H_
#define _UART_H_




/* ---- CMSDK UART0 on MPS2-AN386 ---- */
#define UART0_BASE 0x40004000u /* APB UART0 base */
#define UART0_IRQn   0  // IRQ number in QEMU for UART0

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

#define UART_RX_IRQ_EN    (1u << 3)



// in
#define UART0_INT_RX      (1 << 1)

#define RX_BUF_SIZE 64


// NVIC registers (simple version)
#define NVIC_ISER0       (*((volatile uint32_t *)0xE000E100))
#define NVIC_EnableIRQ(irq)   (NVIC_ISER0 = (1 << (irq)))


extern void uart_init(void);
extern void uart_irq(void);
extern void uart_puts(const char* s);
extern void uart_print(const char* s);
extern int uart_getchar(void);
extern int uart_readLine(char * line, int max_len);

#endif