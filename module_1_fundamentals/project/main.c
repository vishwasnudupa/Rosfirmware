/*
 * Lab 1.3: Memory Mapped I/O (Blinking LED)
 * Demonstrates how to access hardware registers using pointers.
 */

#include <stdint.h>

/* 
 * SIMULATED PERIPHERAL
 * Since we are running in a generic QEMU environment without a specific board,
 * we will simulate a "GPIO Port" by defining a struct and placing it in RAM.
 * In a real chip, this address would be fixed (e.g., 0x40021000).
 */

typedef struct
{
    volatile uint32_t MODER;   /* Mode Register (Input/Output) */
    volatile uint32_t ODR;     /* Output Data Register (Read/Write) */
    volatile uint32_t IDR;     /* Input Data Register (Read Only) */
} GPIO_TypeDef;

/* 
 * We place this "Virtual GPIO" at a safe address in RAM (0x20001000) 
 * just to demonstrate the concept of pointer access.
 */
#define GPIOA_BASE  0x20001000
#define GPIOA       ((GPIO_TypeDef *)GPIOA_BASE)

/* Bit definitions */
#define PIN5        (1 << 5)

void delay(volatile uint32_t count)
{
    while (count--)
    {
        __asm("nop"); /* No Operation assembly instruction */
    }
}

int main(void)
{
    /* 1. Configure Pin 5 as Output */
    /* In a real MCU, we would write to MODER. Here we just simulate it. */
    GPIOA->MODER |= PIN5; 

    while (1)
    {
        /* 2. Turn LED ON (Set Bit 5) */
        GPIOA->ODR |= PIN5;
        
        delay(100000);

        /* 3. Turn LED OFF (Clear Bit 5) */
        GPIOA->ODR &= ~PIN5;

        delay(100000);
    }

    return 0;
}
