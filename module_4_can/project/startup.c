/*
 * Startup Code for Module 4
 */

#include <stdint.h>

extern uint32_t _etext, _sdata, _edata, _sbss, _ebss, _estack;
int main(void);

void Reset_Handler(void)
{
    uint32_t *pSrc = &_etext;
    uint32_t *pDest = &_sdata;
    while (pDest < &_edata) *pDest++ = *pSrc++;
    pDest = &_sbss;
    while (pDest < &_ebss) *pDest++ = 0;
    main();
    while (1);
}

void Default_Handler(void) { while (1); }

__attribute__((section(".isr_vector")))
const void (*VectorTable[])(void) = 
{
    (void (*)(void))&_estack,
    Reset_Handler,
    Default_Handler,
    Default_Handler
};
