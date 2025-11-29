#ifndef SPI_DRIVER_H
#define SPI_DRIVER_H

#include <stdint.h>

/* 
 * Simulated SPI Peripheral Definition
 */
typedef struct
{
    volatile uint32_t CR1;   /* Control Register 1 */
    volatile uint32_t CR2;   /* Control Register 2 */
    volatile uint32_t SR;    /* Status Register */
    volatile uint32_t DR;    /* Data Register */
} SPI_TypeDef;

#define SPI1_BASE   0x40013000
#define SPI1        ((SPI_TypeDef *)SPI1_BASE)

/* Bit Definitions */
#define SPI_CR1_SPE     (1 << 6)  /* SPI Enable */
#define SPI_CR1_MSTR    (1 << 2)  /* Master Selection */
#define SPI_CR1_BR_DIV8 (2 << 3)  /* Baud Rate Control: fPCLK/8 */
#define SPI_SR_TXE      (1 << 1)  /* Transmit Buffer Empty */
#define SPI_SR_RXNE     (1 << 0)  /* Receive Buffer Not Empty */
#define SPI_SR_BSY      (1 << 7)  /* Busy Flag */

/* Function Prototypes */
void SPI_Init(void);
uint8_t SPI_Transfer(uint8_t data);
void SPI_Transmit(uint8_t *data, uint16_t size);

#endif
