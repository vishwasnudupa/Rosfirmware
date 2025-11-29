#ifndef UART_DRIVER_H
#define UART_DRIVER_H

#include <stdint.h>

/* 
 * Simulated UART Peripheral Definition
 * 
 * We support two targets:
 * 1. STM32 (Simulated/Theoretical)
 * 2. QEMU MPS2-AN385 (Real Emulation) - Uses CMSDK UART
 */

#define TARGET_QEMU_MPS2

#ifdef TARGET_QEMU_MPS2
    /* CMSDK APB UART (Used in QEMU mps2-an385) */
    typedef struct
    {
        volatile uint32_t DATA;   /* 0x00: Data Register */
        volatile uint32_t STATE;  /* 0x04: Status Register */
        volatile uint32_t CTRL;   /* 0x08: Control Register */
        volatile uint32_t INTSTATUS; /* 0x0C: Interrupt Status */
        volatile uint32_t BAUDDIV;   /* 0x10: Baud Rate Divider */
    } UART_TypeDef;

    #define UART_BASE   0x40004000 /* UART0 on MPS2 */
    
    /* Bit Definitions for CMSDK UART */
    #define UART_STATE_TXFULL   (1 << 0)
    #define UART_STATE_RXEMPTY  (1 << 1)
    #define UART_CTRL_TXEN      (1 << 0)
    #define UART_CTRL_RXEN      (1 << 1)
    #define UART_CTRL_TXIRQEN   (1 << 2)
    #define UART_CTRL_RXIRQEN   (1 << 3)

#else
    /* STM32-style UART (Theoretical) */
    typedef struct
    {
        volatile uint32_t SR;   /* Status Register */
        volatile uint32_t DR;   /* Data Register */
        volatile uint32_t BRR;  /* Baud Rate Register */
        volatile uint32_t CR1;  /* Control Register 1 */
    } UART_TypeDef;

    #define UART_BASE   0x40004400 
    
    #define UART_SR_TXE     (1 << 7)
    #define UART_SR_RXNE    (1 << 5)
    #define UART_CR1_UE     (1 << 13)
    #define UART_CR1_TE     (1 << 3)
    #define UART_CR1_RE     (1 << 2)
    #define UART_CR1_RXNEIE (1 << 5)
#endif

#define UART1       ((UART_TypeDef *)UART_BASE)

/* Function Prototypes */
void UART_Init(void);
void UART_WriteChar(char c);
void UART_WriteString(const char *str);
char UART_ReadChar(void);

#endif
