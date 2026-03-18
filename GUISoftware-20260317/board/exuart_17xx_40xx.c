#include "chip.h"
#include <nxp/iolpc1788.h>


#ifdef CHIP_EXUART_USER
static LPC_EXUART_TYPE_T LPC_EXUART_TYPE = TL16C550X;
STATIC uint32_t LPC_EXUART_RATE = LPC_TL16C550X_CLK;
volatile uint32_t LPC_EXUART1_BASE = 0;
volatile uint32_t LPC_EXUART2_BASE = 0;
volatile uint32_t LPC_EXUART3_BASE = 0;
volatile uint32_t LPC_EXUART4_BASE = 0;
#endif

STATIC uint32_t Chip_Clock_GetExUartClockRate(void)
{
	return LPC_EXUART_RATE;
}


/*****************************************************************************
 * Public functions
 ****************************************************************************/
#ifdef CHIP_EXUART_USER
void Chip_EXUART_SetChipType(LPC_EXUART_TYPE_T type)
{
    LPC_EXUART_TYPE = type;
    switch(type)
    {
    case TL16C550X:
    default:
        LPC_EXUART_RATE = LPC_TL16C550X_CLK;
        LPC_EXUART1_BASE = EMC_ADDRESS_CS2;
        LPC_EXUART2_BASE = EMC_ADDRESS_CS3;
        LPC_EXUART3_BASE = EMC_ADDRESS_CS3;
        LPC_EXUART4_BASE = EMC_ADDRESS_CS3;
        break;
    case TL16C554X:
        LPC_EXUART_RATE = LPC_TL16C554X_CLK;
        LPC_EXUART1_BASE = (EMC_ADDRESS_CS1 | (0x00 << 3));
        LPC_EXUART2_BASE = (EMC_ADDRESS_CS1 | (0x01 << 3));
        LPC_EXUART3_BASE = (EMC_ADDRESS_CS1 | (0x02 << 3));
        LPC_EXUART4_BASE = (EMC_ADDRESS_CS1 | (0x03 << 3)); 
        break;
    }
}
#endif

/* Reset the Chip */
void Chip_EXUART_Reset(void)
{
    volatile uint32_t i = 0;    
    
//    IOCON_P0_04 = 0x00;//GPIO INTB
//    IOCON_P0_05 = 0x00;//GPIO INTA
//    FIO0DIR &= ~( 1<<4  | 1<<5); 
    
    IOCON_P0_17 = 0x00;
    IOCON_P0_18 = 0x00;
    FIO0DIR &= ~(1<<17 | 1<<18); 
    
    IO0INTENR_bit.P0_5 = 0;
    IO0INTCLR_bit.P0_5 = 1;
    IO0INTENR_bit.P0_4 = 0;
    IO0INTCLR_bit.P0_4 = 1;
    IO0INTENR_bit.P0_17 = 0;
    IO0INTCLR_bit.P0_17 = 1;
    IO0INTENR_bit.P0_18 = 0;   
    IO0INTCLR_bit.P0_18 = 1;
    
    //复位
    IOCON_P1_12 = 0x10;//GPIO 2550RST    
    FIO1DIR1 |= 0x10;//P1[12]输出
	FIO1PIN |= 1<<12;
    for(i=0; i<10000; i++);
    FIO1PIN &= ~(1<<12);
}

/* Initializes the pUART peripheral */
void Chip_EXUART_Init(LPC_EXUART_T *pUART)
{
    uint8_t tmp;
   
	/* Enable FIFOs by default, reset them */
	Chip_EXUART_SetupFIFOS(pUART, (EXUART_FCR_FIFO_EN | EXUART_FCR_RX_RS | EXUART_FCR_TX_RS));
	
    /* Disable interrupts */
	pUART->IER = 0;
	/* Set LCR to default state */
	pUART->LCR = 0;
	
    /* Clear MCR */
    /* Set Modem Control to default state */
    pUART->MCR = 0;
    /*Dummy Reading to Clear Status */
    tmp = pUART->MSR;
    tmp = tmp;

	/* Default 8N1, with DLAB disabled */
	Chip_EXUART_ConfigData(pUART, (EXUART_LCR_WLEN8 | EXUART_LCR_SBS_1BIT | EXUART_LCR_PARITY_DIS));
    
    /* set int pin to input */
    if(pUART == LPC_EXUART1)
    {
        /* enable interrupt */
        IO0INTENR_bit.P0_5 = 1;
    }
    else if(pUART == LPC_EXUART2)
    {
        /* enable interrupt */ 
        IO0INTENR_bit.P0_4 = 1;//INTB 串口2                
    }
    else if(pUART == LPC_EXUART3)
    {
        if(LPC_EXUART_TYPE == TL16C554X)
        {
            /* enable interrupt */
            IO0INTENR_bit.P0_17 = 1;
        }
    }
    else if(pUART == LPC_EXUART4)
    {
        if(LPC_EXUART_TYPE == TL16C554X)
        {
            /* enable interrupt */
            IO0INTENR_bit.P0_18 = 1; 
        }
    }
}

/* De-initializes the pUART peripheral */
void Chip_EXUART_DeInit(LPC_EXUART_T *pUART)
{
    uint8_t tmp = 0;
    
    /* Set Modem Control to default state */
    pUART->MCR = 0;
    /* Set Modem Control to default state */
    pUART->FCR = 0;
    tmp = pUART->RBR;
    /* Disable interrupts */
    pUART->IER = 0;
    /* Set LCR to default state */
    pUART->LCR = 0;
    tmp = pUART->LSR;
    tmp = pUART->MSR;
    tmp = tmp;
    
    /* set int pin to input */
    if(pUART == LPC_EXUART1)
    {
        /* disable interrupt */
        IO0INTENR_bit.P0_5 = 0;
        IO0INTCLR_bit.P0_5 = 1;
    }
    else if(pUART == LPC_EXUART2)
    {
        /* disable interrupt */
        IO0INTENR_bit.P0_4 = 0;//INTB 串口2 
        IO0INTCLR_bit.P0_4 = 1;                 
    }
    else if(pUART == LPC_EXUART3)
    {
        if(LPC_EXUART_TYPE == TL16C554X)
        {
            /* disable interrupt */
            IO0INTENR_bit.P0_17 = 0; 
            IO0INTCLR_bit.P0_17 = 1;
        }
    }
    else if(pUART == LPC_EXUART4)
    {
        if(LPC_EXUART_TYPE == TL16C554X)
        {
            /* disable interrupt */
            IO0INTENR_bit.P0_18 = 0;  
            IO0INTCLR_bit.P0_18 = 1;
        }
    }
}

/* Transmit a byte array through the UART peripheral (non-blocking) */
int Chip_EXUART_Send(LPC_EXUART_T *pUART, const void *data, int numBytes)
{
	int sent = 0;
	uint8_t *p8 = (uint8_t *) data;

	/* Send until the transmit FIFO is full or out of bytes */
	while ((sent < numBytes) &&
		   ((Chip_EXUART_ReadLineStatus(pUART) & EXUART_LSR_THRE) != 0)) {
		Chip_EXUART_SendByte(pUART, *p8);
		p8++;
		sent++;
	}

	return sent;
}

/* Check whether if UART is busy or not */
FlagStatus Chip_EXUART_CheckBusy(LPC_EXUART_T *pUART)
{
	if (pUART->LSR & EXUART_LSR_TEMT) {
		return RESET;
	}
	else {
		return SET;
	}
}

/* Check whether if UART FIFO is busy or not */
FlagStatus Chip_EXUART_CheckFIFOBusy(LPC_EXUART_T *pUART)
{
	if (pUART->LSR & EXUART_LSR_THRE) {
		return RESET;
	}
	else {
		return SET;
	}    
}

/* Transmit a byte array through the UART peripheral (blocking) */
int Chip_EXUART_SendBlocking(LPC_EXUART_T *pUART, const void *data, int numBytes)
{
	int pass, sent = 0;
	uint8_t *p8 = (uint8_t *) data;

	while (numBytes > 0) {
		pass = Chip_EXUART_Send(pUART, p8, numBytes);
		numBytes -= pass;
		sent += pass;
		p8 += pass;
	}

	return sent;
}


/* Read data through the UART peripheral (non-blocking) */
int Chip_EXUART_Read(LPC_EXUART_T *pUART, void *data, int numBytes)
{
	int readBytes = 0;
	uint8_t *p8 = (uint8_t *) data;

	/* Send until the transmit FIFO is full or out of bytes */
	while ((readBytes < numBytes) &&
		   ((Chip_EXUART_ReadLineStatus(pUART) & EXUART_LSR_RDR) != 0)) {
		*p8 = Chip_EXUART_ReadByte(pUART);
		p8++;
		readBytes++;
	}

	return readBytes;
}

/* Read data through the UART peripheral (blocking) */
int Chip_EXUART_ReadBlocking(LPC_EXUART_T *pUART, void *data, int numBytes)
{
	int pass, readBytes = 0;
	uint8_t *p8 = (uint8_t *) data;

	while (readBytes < numBytes) {
		pass = Chip_EXUART_Read(pUART, p8, numBytes);
		numBytes -= pass;
		readBytes += pass;
		p8 += pass;
	}

	return readBytes;
}

/* Determines and sets best dividers to get a target bit rate */
uint32_t Chip_EXUART_SetBaud(LPC_EXUART_T *pUART, uint32_t baudrate)
{
	uint32_t div, divh, divl, clkin;

	/* Determine UART clock in rate without FDR */
	clkin = Chip_Clock_GetExUartClockRate();
	div = clkin / (baudrate * 16);

	/* High and low halves of the divider */
	divh = (div >> 8) & 0xFF;
	divl = div & 0xFF;

	Chip_EXUART_EnableDivisorAccess(pUART);
	Chip_EXUART_SetDivisorLatches(pUART, divl, divh);
	Chip_EXUART_DisableDivisorAccess(pUART);

	return clkin / div;
}


