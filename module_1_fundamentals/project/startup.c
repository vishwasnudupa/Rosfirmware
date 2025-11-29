/*
 * Lab 1.2: Startup Code
 * [FIRMWARE LAYER: SYSTEM / BSP]
 * 
 * This file is the "Big Bang" of the firmware.
 * It is the bridge between the dead silicon and the living C application.
 * Without this, main() never runs.
 */

#include <stdint.h>

/* Symbols defined in the linker script */
extern uint32_t _etext;  /* End of code in Flash */
extern uint32_t _sdata;  /* Start of data in RAM */
extern uint32_t _edata;  /* End of data in RAM */
extern uint32_t _sbss;   /* Start of bss in RAM */
extern uint32_t _ebss;   /* End of bss in RAM */
extern uint32_t _estack; /* Top of Stack */

/* Main function prototype */
int main(void);

/* Reset Handler: The first function to execute */
void Reset_Handler(void)
{
    /* 1. Copy .data section from Flash to RAM */
    uint32_t *pSrc = &_etext;
    uint32_t *pDest = &_sdata;

    while (pDest < &_edata)
    {
        *pDest++ = *pSrc++;
    }

    /* 2. Zero out .bss section in RAM */
    pDest = &_sbss;
    while (pDest < &_ebss)
    {
        *pDest++ = 0;
    }

    /* 3. Call main() */
    main();

    /* 4. Infinite loop if main returns (should not happen) */
    while (1);
}

/* Default Handler for unexpected interrupts */
void Default_Handler(void)
{
    while (1);
}

/* Vector Table: Placed at 0x00000000 via linker script */
/* The 'section' attribute tells the compiler to put this array in the .isr_vector section */
__attribute__((section(".isr_vector")))
const void (*VectorTable[])(void) = 
{
    (void (*)(void))&_estack, /* Initial Stack Pointer */
    Reset_Handler,             /* Reset Handler */
    Default_Handler,           /* NMI Handler */
    Default_Handler,           /* Hard Fault Handler */
    Default_Handler,           /* MemManage Handler */
    Default_Handler,           /* Bus Fault Handler */
    Default_Handler,           /* Usage Fault Handler */
    0, 0, 0, 0,                /* Reserved */
    Default_Handler,           /* SVCall Handler */
    Default_Handler,           /* Debug Monitor Handler */
    0,                         /* Reserved */
    Default_Handler,           /* PendSV Handler */
    Default_Handler            /* SysTick Handler */
};
