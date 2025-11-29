#include "rtos_kernel.h"

TCB_TypeDef tasks[MAX_TASKS];
TCB_TypeDef *current_task;
TCB_TypeDef *next_task;
uint8_t task_count = 0;
uint8_t current_task_idx = 0;

/* 
 * Stack Initialization
 * We need to fake the stack content so it looks like the task was interrupted.
 * xPSR, PC, LR, R12, R3, R2, R1, R0, R11-R4
 */
void RTOS_CreateTask(void (*task_func)(void), uint8_t priority)
{
    (void)priority; /* Ignored for Round Robin */
    
    if (task_count >= MAX_TASKS) return;

    TCB_TypeDef *tcb = &tasks[task_count];
    uint32_t *sp = &tcb->stack[STACK_SIZE - 1];

    /* 1. xPSR: Set T bit (Thumb Mode) */
    *sp-- = (1 << 24); 
    
    /* 2. PC: Program Counter (The function entry point) */
    *sp-- = (uint32_t)task_func;
    
    /* 3. LR: Link Register (Return address - should not return) */
    *sp-- = 0xFFFFFFFF;
    
    /* 4. R12, R3, R2, R1, R0 (General Purpose Registers) */
    *sp-- = 0; *sp-- = 0; *sp-- = 0; *sp-- = 0; *sp-- = 0;
    
    /* 5. R11-R4 (Callee Saved Registers) */
    *sp-- = 0; *sp-- = 0; *sp-- = 0; *sp-- = 0;
    *sp-- = 0; *sp-- = 0; *sp-- = 0; *sp-- = 0;

    tcb->sp = sp;
    task_count++;
}

void RTOS_Init(void)
{
    current_task = &tasks[0];
    current_task_idx = 0;
}

/* 
 * The Scheduler: Round Robin
 * Called by SysTick Handler or Yield
 */
void RTOS_Scheduler(void)
{
    current_task_idx++;
    if (current_task_idx >= task_count)
    {
        current_task_idx = 0;
    }
    next_task = &tasks[current_task_idx];
    
    /* Trigger PendSV to perform context switch */
    /* SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk; */
    *(volatile uint32_t *)0xE000ED04 |= (1 << 28);
}

void RTOS_Start(void)
{
    /* Configure SysTick for 1ms interrupts */
    /* SysTick->LOAD = 16000 - 1; */
    *(volatile uint32_t *)0xE000E014 = 15999;
    /* SysTick->VAL = 0; */
    *(volatile uint32_t *)0xE000E018 = 0;
    /* SysTick->CTRL = Enable | TickInt | ClkSource */
    *(volatile uint32_t *)0xE000E010 = 0x07;

    /* Start the first task */
    /* This requires assembly to load the first stack pointer. 
       For simplicity in this C-only course, we will assume the 
       PendSV_Handler handles the first switch naturally or we'd need inline asm.
       Here we just loop.
    */
    while(1);
}

/* 
 * PendSV Handler: The Context Switcher
 * This is usually written in Assembly. 
 * Since we are in C-only mode for simplicity, we will describe what it DOES.
 * In a real project, this file would contain __attribute__((naked)) void PendSV_Handler(void)
 */
