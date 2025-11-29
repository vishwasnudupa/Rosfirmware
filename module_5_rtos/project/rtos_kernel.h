#ifndef RTOS_KERNEL_H
#define RTOS_KERNEL_H

#include <stdint.h>

#define MAX_TASKS 5
#define STACK_SIZE 1024

/* Task Control Block */
typedef struct
{
    uint32_t *sp; /* Stack Pointer */
    uint32_t stack[STACK_SIZE];
} TCB_TypeDef;

/* Function Prototypes */
void RTOS_Init(void);
void RTOS_CreateTask(void (*task_func)(void), uint8_t priority);
void RTOS_Start(void);
void RTOS_Yield(void);
void RTOS_Delay(uint32_t ticks);

#endif
