#ifndef __UART_H
#define __UART_H
#include "MedibusComm_cfg.h"
#include "stdtypes.h"

//#define UART_FCR_FIFO_EN		((uint8_t)(1<<0))
//#define UART_FCR_RX_RS			((uint8_t)(1<<1))
//#define UART_FCR_TX_RS			((uint8_t)(1<<2))
//#define UART_FCR_DMAMODE_SEL 	((uint8_t)(1<<3))
//#define UART_FCR_TRG_LEV0		((uint8_t)(0))
//#define UART_FCR_TRG_LEV1		((uint8_t)(1<<6))
//#define UART_FCR_TRG_LEV2		((uint8_t)(2<<6))
//#define UART_FCR_TRG_LEV3		((uint8_t)(3<<6))
//#define UART_LSR_RDR		((uint8_t)(1<<0))
//
//#define UART_LSR_OE			((uint8_t)(1<<1))
//#define UART_LSR_PE			((uint8_t)(1<<2))
//#define UART_LSR_FE			((uint8_t)(1<<3))/** Line status register: Break interrupt*/
//#define UART_LSR_BI			((uint8_t)(1<<4))
//#define UART_LSR_THRE		((uint8_t)(1<<5))
//#define UART_LSR_TEMT		((uint8_t)(1<<6))
//#define UART_LSR_RXFE		((uint8_t)(1<<7))

#define RXRDY   0x04
#define TXRDY   0x02

//#ifdef IAR_LPC_1788_STK    //Î´ÓÃµ½
//#define ERHR1  (*(volatile unsigned char *)0x98000000)
//#define ETHR1  (*(volatile unsigned char *)0x98000000)
//#define EIER1  (*(volatile unsigned char *)0x98000001)
//#define EIIR1  (*(volatile unsigned char *)0x98000002)
//#define EFCR1  (*(volatile unsigned char *)0x98000002)
//#define ELCR1  (*(volatile unsigned char *)0x98000003)
//#define EMCR1  (*(volatile unsigned char *)0x98000004)
//#define ELSR1  (*(volatile unsigned char *)0x98000005)
//#define EMSR1  (*(volatile unsigned char *)0x98000006)
//#define ESPR1  (*(volatile unsigned char *)0x98000007)
//#define EDLL1  (*(volatile unsigned char *)0x98000000)
//#define EDLM1  (*(volatile unsigned char *)0x98000001)
//
//#define ERHR2  (*(volatile unsigned char *)0x9c000000)
//#define ETHR2  (*(volatile unsigned char *)0x9c000000)
//#define EIER2  (*(volatile unsigned char *)0x9c000001)
//#define EIIR2  (*(volatile unsigned char *)0x9c000002)
//#define EFCR2  (*(volatile unsigned char *)0x9c000002)
//#define ELCR2  (*(volatile unsigned char *)0x9c000003)
//#define EMCR2  (*(volatile unsigned char *)0x9c000004)
//#define ELSR2  (*(volatile unsigned char *)0x9c000005)
//#define EMSR2  (*(volatile unsigned char *)0x9c000006)
//#define ESPR2  (*(volatile unsigned char *)0x9c000007)
//#define EDLL2  (*(volatile unsigned char *)0x9c000000)
//#define EDLM2  (*(volatile unsigned char *)0x9c000001)
//
//#endif


typedef enum
{
    UART0 = 0,
    UART1,
    UART2,
    UART3,
    UART4
}Uart_Id;

//typedef enum
//{
//    EXTUART0 = 0,
//    EXTUART1,
//    EXTUART2,
//    EXTUART3,
//    EXTUART4
//}ExtUart_Id;

////ext uart IIR
//#define EXTUART_IT_LINE         (u8)0x06
//#define EXTUART_IT_RXNE         (u8)0x04
//#define EXTUART_IT_CTO          (u8)0x0C
//#define EXTUART_IT_THRE         (u8)0x02

//extern u8 ExtUart1_SendBuf[MEDIBUS_FIFOTXNUM];

unsigned char UART0_Init(unsigned int BR);
unsigned char UART1_Init(unsigned int BR);
unsigned char UART2_Init(unsigned int BR);
unsigned char UART3_Init(unsigned int BR);
unsigned char UART4_Init(unsigned int BR);
//void EXT_UART_Init(unsigned int BR1,unsigned int BR2);
//void EXTUART1_Init(unsigned int BR1);
unsigned char LookForTable(float FR);

//unsigned char ReadExtUsart1(void);
//unsigned char ReadExtUsart1Status(void);
//void WriteExtUsart1(unsigned char data);
//unsigned char ReadExtUsart2(void);
//unsigned char ReadExtUsart2Status(void);
//void WriteExtUsart2(unsigned char data);


//void UART0_Interrupt(void);
//void UART1_Interrupt(void);
//void UART2_Interrupt(void);
//void UART3_Interrupt(void);

void UART_TxITConfig(Uart_Id Id,FunctionalState state);
unsigned char UART_CheckBusy(Uart_Id Id);
//unsigned char EXTUART_CheckBusy(ExtUart_Id Id);

//extern unsigned char touch_valid;
extern short int TsXscale,TsYscale;
//extern unsigned char UART3_Receive[20];
//extern unsigned int UART_TIM[6];
//extern short int UART_State_Report_Flag;
//extern unsigned char UART_Check0,UART_Check1,UART_Check2,UART_Check3,UART_Check4,UART_Check5;
//extern unsigned char UART_Order0,UART_Order1,UART_Order2,UART_Order3,UART_Order4,UART_Order5;


u8 UART3_CheckBusy(void);
//u8 EXTUART_CheckBusy(ExtUart_Id Id);
//u8 EXTUART1_ReceiveByte(void);
//u8 EXTUART2_ReceiveByte(void);
//void EXTUART1_SendByte(u8 data);
//void EXTUART2_SendByte(u8 data);
//u8 EXTUART1_GetFlagStatus(void);
//u8 EXTUART1_GetITStatus(void);
//u8 EXTUART2_GetFlagStatus(void);
//u8 EXTUART2_GetITStatus(void);
//void EXTUART_TxITConfig(ExtUart_Id Id,FunctionalState state);
//void EXTUART1_SetBreakCtrl(u8 data);

void exuart_init(void);
void exuart1_init(u32 BR, u32 parity);
void exuart2_init(u32 BR, u32 parity);
void exuart3_init(u32 BR, u32 parity);
void exuart4_init(u32 BR, u32 parity);

#endif

