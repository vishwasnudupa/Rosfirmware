#ifndef CAN_DRIVER_H
#define CAN_DRIVER_H

#include <stdint.h>

/* 
 * Simulated CAN Peripheral Definition
 */
typedef struct
{
    volatile uint32_t MCR;   /* Master Control Register */
    volatile uint32_t MSR;   /* Master Status Register */
    volatile uint32_t TSR;   /* Transmit Status Register */
    volatile uint32_t RF0R;  /* Receive FIFO 0 Register */
    volatile uint32_t IER;   /* Interrupt Enable Register */
    volatile uint32_t BTR;   /* Bit Timing Register */
    volatile uint32_t TI0R;  /* TX Mailbox 0 Identifier Register */
    volatile uint32_t TDT0R; /* TX Mailbox 0 Data Length & Time Stamp */
    volatile uint32_t TDL0R; /* TX Mailbox 0 Data Low Register */
    volatile uint32_t TDH0R; /* TX Mailbox 0 Data High Register */
    volatile uint32_t RI0R;  /* RX FIFO 0 Identifier Register */
    volatile uint32_t RDT0R; /* RX FIFO 0 Data Length & Time Stamp */
    volatile uint32_t RDL0R; /* RX FIFO 0 Data Low Register */
    volatile uint32_t RDH0R; /* RX FIFO 0 Data High Register */
} CAN_TypeDef;

#define CAN1_BASE   0x40006400
#define CAN1        ((CAN_TypeDef *)CAN1_BASE)

/* Bit Definitions */
#define CAN_MCR_INRQ    (1 << 0)  /* Initialization Request */
#define CAN_MCR_NART    (1 << 4)  /* No Automatic Retransmission */
#define CAN_MSR_INAK    (1 << 0)  /* Initialization Acknowledge */
#define CAN_TSR_TME0    (1 << 26) /* Transmit Mailbox 0 Empty */
#define CAN_TSR_RQCP0   (1 << 0)  /* Request Completed Mailbox 0 */
#define CAN_TSR_TXOK0   (1 << 1)  /* Transmission OK Mailbox 0 */
#define CAN_RF0R_FMP0   (3 << 0)  /* FIFO 0 Message Pending */
#define CAN_TI0R_TXRQ   (1 << 0)  /* Transmit Request */

/* CAN Frame Structure */
typedef struct
{
    uint32_t ID;        /* Standard ID (11-bit) */
    uint8_t DLC;        /* Data Length Code (0-8) */
    uint8_t Data[8];    /* Data Payload */
} CAN_Frame;

/* Function Prototypes */
void CAN_Init(void);
void CAN_Transmit(CAN_Frame *frame);
int CAN_Receive(CAN_Frame *frame);

#endif
