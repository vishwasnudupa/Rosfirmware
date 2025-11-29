#include "can_driver.h"

void CAN_Init(void)
{
    /* 1. Request Initialization Mode */
    CAN1->MCR |= CAN_MCR_INRQ;
    while (!(CAN1->MSR & CAN_MSR_INAK)); /* Wait for Ack */

    /* 2. Exit Sleep Mode */
    CAN1->MCR &= ~0x02; /* Clear SLEEP bit */

    /* 3. Configure Bit Timing (BTR) */
    /* 
     * Assuming PCLK = 36MHz. We want 500kbps.
     * Prescaler = 4 -> Tq = 4/36MHz = 111ns
     * Bit Time = 1/500kHz = 2000ns = 18 Tq
     * SYNC_SEG = 1 Tq
     * BS1 = 12 Tq
     * BS2 = 5 Tq
     * Total = 1 + 12 + 5 = 18 Tq
     */
    CAN1->BTR = (0 << 24) | (4 << 20) | (11 << 16) | (3); /* Simplified for simulation */
    
    /* 4. Configure Test Mode (Loopback) for Lab 4.2 */
    /* In Loopback mode, TX is internally connected to RX */
    CAN1->BTR |= (1 << 30); 

    /* 5. Enter Normal Mode */
    CAN1->MCR &= ~CAN_MCR_INRQ;
    while (CAN1->MSR & CAN_MSR_INAK); /* Wait for Sync */
}

void CAN_Transmit(CAN_Frame *frame)
{
    /* 1. Wait for Mailbox 0 to be Empty */
    while (!(CAN1->TSR & CAN_TSR_TME0));

    /* 2. Set ID (Standard Identifier) */
    CAN1->TI0R = (frame->ID << 21);

    /* 3. Set Data Length */
    CAN1->TDT0R = (frame->DLC & 0xF);

    /* 4. Set Data */
    CAN1->TDL0R = (frame->Data[0] << 0) | (frame->Data[1] << 8) | (frame->Data[2] << 16) | (frame->Data[3] << 24);
    CAN1->TDH0R = (frame->Data[4] << 0) | (frame->Data[5] << 8) | (frame->Data[6] << 16) | (frame->Data[7] << 24);

    /* 5. Request Transmission */
    CAN1->TI0R |= CAN_TI0R_TXRQ;
}

int CAN_Receive(CAN_Frame *frame)
{
    /* 1. Check if FIFO 0 has messages */
    if ((CAN1->RF0R & CAN_RF0R_FMP0) == 0)
    {
        return 0; /* No message */
    }

    /* 2. Read ID */
    frame->ID = (CAN1->RI0R >> 21) & 0x7FF;

    /* 3. Read DLC */
    frame->DLC = (CAN1->RDT0R & 0xF);

    /* 4. Read Data */
    uint32_t data_low = CAN1->RDL0R;
    uint32_t data_high = CAN1->RDH0R;

    frame->Data[0] = (data_low >> 0) & 0xFF;
    frame->Data[1] = (data_low >> 8) & 0xFF;
    frame->Data[2] = (data_low >> 16) & 0xFF;
    frame->Data[3] = (data_low >> 24) & 0xFF;
    frame->Data[4] = (data_high >> 0) & 0xFF;
    frame->Data[5] = (data_high >> 8) & 0xFF;
    frame->Data[6] = (data_high >> 16) & 0xFF;
    frame->Data[7] = (data_high >> 24) & 0xFF;

    /* 5. Release FIFO */
    CAN1->RF0R |= (1 << 5); /* RFOM0: Release Output Mailbox */
    
    return 1; /* Success */
}
