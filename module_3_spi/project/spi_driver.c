#include "spi_driver.h"

void SPI_Init(void)
{
    /* 1. Enable SPI Clock (Simulated) */
    
    /* 2. Configure SPI */
    /* Master Mode, Baud Rate = fPCLK/8, CPOL=0, CPHA=0 (Mode 0) */
    SPI1->CR1 = SPI_CR1_MSTR | SPI_CR1_BR_DIV8;
    
    /* 3. Enable SPI */
    SPI1->CR1 |= SPI_CR1_SPE;
}

uint8_t SPI_Transfer(uint8_t data)
{
    /* 1. Wait until Transmit Buffer is Empty */
    while (!(SPI1->SR & SPI_SR_TXE));
    
    /* 2. Send Data */
    SPI1->DR = data;
    
    /* 3. Wait until Receive Buffer is Not Empty */
    /* Note: In SPI, for every byte sent, a byte is received. */
    while (!(SPI1->SR & SPI_SR_RXNE));
    
    /* 4. Return Received Data */
    return (uint8_t)(SPI1->DR);
}

void SPI_Transmit(uint8_t *data, uint16_t size)
{
    for (uint16_t i = 0; i < size; i++)
    {
        SPI_Transfer(data[i]);
    }
}
