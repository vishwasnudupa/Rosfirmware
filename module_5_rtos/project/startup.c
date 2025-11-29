/*
 * Startup Code for Module 5
 */

#include <stdint.h>

extern uint32_t _etext, _sdata, _edata, _sbss, _ebss, _estack;
int main(void);

/* RTOS Handlers */
extern void RTOS_Scheduler(void);

void SysTick_Handler(void)
{
    /* Call Scheduler every tick */
    RTOS_Scheduler();
}

void PendSV_Handler(void)
{
    /* 
     * In a real RTOS, this is where the magic happens.
     * 1. Push R4-R11 to current_task->sp
     * 2. current_task = next_task
     * 3. Pop R4-R11 from current_task->sp
     * 4. Return
     * 
     * Since we cannot easily write complex assembly here without
     * compiler-specific syntax, we leave this as a placeholder 
     * for the "Theory" lesson.
     */
}

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
    Default_Handler,
    0, 0, 0, 0, 0, 0, 0,
    Default_Handler, /* SVCall */
    Default_Handler, /* DebugMon */
    0,
    PendSV_Handler,  /* PendSV */
    SysTick_Handler  /* SysTick */
};
