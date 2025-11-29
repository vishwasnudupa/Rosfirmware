/*
 * Startup Code for Module 2
 * Includes UART Interrupt Handler
 */

#include <stdint.h>

extern uint32_t _etext, _sdata, _edata, _sbss, _ebss, _estack;
int main(void);

/* External ISR prototype */
void UART1_IRQHandler(void); 

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
    Default_Handler, /* NMI */
    Default_Handler, /* HardFault */
    0, 0, 0, 0, 0, 0, 0, /* Reserved */
    Default_Handler, /* SVCall */
    Default_Handler, /* DebugMon */
    0,               /* Reserved */
    Default_Handler, /* PendSV */
    Default_Handler, /* SysTick */
    
    /* External Interrupts */
    Default_Handler, /* WWDG */
    Default_Handler, /* PVD */
    Default_Handler, /* TAMPER */
    Default_Handler, /* RTC */
    Default_Handler, /* FLASH */
    Default_Handler, /* RCC */
    Default_Handler, /* EXTI0 */
    Default_Handler, /* EXTI1 */
    Default_Handler, /* EXTI2 */
    Default_Handler, /* EXTI3 */
    Default_Handler, /* EXTI4 */
    Default_Handler, /* DMA1_Channel1 */
    Default_Handler, /* DMA1_Channel2 */
    Default_Handler, /* DMA1_Channel3 */
    Default_Handler, /* DMA1_Channel4 */
    Default_Handler, /* DMA1_Channel5 */
    Default_Handler, /* DMA1_Channel6 */
    Default_Handler, /* DMA1_Channel7 */
    Default_Handler, /* ADC1_2 */
    Default_Handler, /* USB_HP_CAN1_TX */
    Default_Handler, /* USB_LP_CAN1_RX0 */
    Default_Handler, /* CAN1_RX1 */
    Default_Handler, /* CAN1_SCE */
    Default_Handler, /* EXTI9_5 */
    Default_Handler, /* TIM1_BRK */
    Default_Handler, /* TIM1_UP */
    Default_Handler, /* TIM1_TRG_COM */
    Default_Handler, /* TIM1_CC */
    Default_Handler, /* TIM2 */
    Default_Handler, /* TIM3 */
    Default_Handler, /* TIM4 */
    Default_Handler, /* I2C1_EV */
    Default_Handler, /* I2C1_ER */
    Default_Handler, /* I2C2_EV */
    Default_Handler, /* I2C2_ER */
    Default_Handler, /* SPI1 */
    Default_Handler, /* SPI2 */
    UART1_IRQHandler /* USART1 - Position 37 */
};
