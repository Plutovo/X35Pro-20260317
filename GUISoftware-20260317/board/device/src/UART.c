#include "includes.h"
#include "UART.h"

extern unsigned char Masimo_SpO2_Config_On; //0:关闭   1：开启

/*#ifdef _DLIB_FILE_DESCRIPTOR

int fputc(int ch, FILE *f)
{
    ETHR1 = ch;
    while(((ELSR1&(1<<6))>>6) == 0);
    
    return ch;
}

#endif*/

float FR_Table1[]=
{
  1.000,1.067,1.071,1.077,1.083,1.091,1.100,1.111,1.125,1.133,
  1.143,1.154,1.167,1.182,1.200,1.214,1.222,1.231,1.250,1.267,
  1.273,1.286,1.300,1.308,1.333,1.357,1.364,1.375,1.385,1.400,
  1.417,1.429,1.444,1.455,1.462,1.467,1.500,1.533,1.538,1.545,
  1.556,1.571,1.583,1.600,1.615,1.625,1.636,1.643,1.667,1.692,
  1.700,1.714,1.727,1.733,1.750,1.769,1.778,1.786,1.800,1.818,
  1.833,1.846,1.857,1.867,1.875,1.889,1.900,1.909,1.917,1.923,
  1.929,1.933
};
unsigned char DIV_Table[]=
{
  0,1,1,1,1,1,1,1,1,2,1,2,1,2,1,3,2,3,1,4,3,2,3,4,1,5,4,3,5,2,5,3,
  4,5,6,7,1,8,7,6,5,4,7,3,8,5,7,9,2,9,7,5,8,11,3,10,7,11,4,9,5,11,6,
  13,7,8,9,10,11,12,13,14
};
unsigned char MUL_Table[]=
{
  1,15,14,13,12,11,10,9,8,15,7,13,6,11,5,14,9,13,4,15,11,7,10,13,3,
  14,11,8,13,5,12,7,9,11,13,15,2,15,13,11,9,7,12,5,13,8,11,14,3,13,10,7,11,15,4,13,9,14,5,11,6,3,7,15,8,9,10,11,12,13,14,15
 
};

volatile unsigned int DL,j;
volatile float FR,DL1;
unsigned char UART0_Init(unsigned int BR)
{
  Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_UART0); 
  
  U0LCR =0;
  U0IER =0;
  U0RBR =0;
  U0FCR =6;
  U0TER =0;
  
  U0LCR = 1<<7;
  DL1 = Chip_Clock_GetPeripheralClockRate() / (float)(16 * BR);
	if( ((int)(DL1*10)%10) != 0 || ((int)(DL1*100)%10) != 0 || ((int)(DL1*1000)%10) != 0)
  {
    do
    {
      FR = FR_Table1[j++];
      if(FR >=2.0f)
        return 1<<4;
      DL = (unsigned int)(Chip_Clock_GetPeripheralClockRate() / (16 * BR * FR));
      FR = (float)Chip_Clock_GetPeripheralClockRate() / (16 * BR * DL);
    }while (!(FR > 1.1 && FR <1.9f)) ;
    U0DLL = DL & 0xFF;
    U0DLM = (DL >> 8) & 0xFF;
    U0FDR=LookForTable(FR);
  }
  else
  {
    U0DLM = (int)DL1>>8 & 0xff;
    U0DLL = (int)DL1 & 0xff;
    U0FDR = 1<<4;
  }
  U0LCR = 0;

  U0IER = 3;
  U0FCR = 7;  
  U0TER = 1<<7;
  U0LCR = 3;
  return 1;
}

unsigned char UART1_Init(unsigned int BR)
{
  Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_UART1); 
  
  U1LCR =0;
  U1IER =0;
  U1RBR =0;
  U1FCR =6;
  U1TER =0;
  
  U1LCR = 1<<7;
  DL1 = Chip_Clock_GetPeripheralClockRate() / (float)(16 * BR);
	if( ((int)(DL1*10)%10) != 0 || ((int)(DL1*100)%10) != 0 || ((int)(DL1*1000)%10) != 0)
  {
    do
    {
      FR = FR_Table1[j++];
      if(FR >=2.0f)
        return 1<<5;
      DL = (unsigned int)(Chip_Clock_GetPeripheralClockRate() / (16 * BR * FR));
      FR = (float)Chip_Clock_GetPeripheralClockRate() / (16 * BR * DL);
    }while (!(FR > 1.1 && FR <1.9f)) ;
    U1DLL = DL & 0xFF;
    U1DLM = (DL >> 8) & 0xFF;
    U1FDR=LookForTable(FR);
  }
  else
  {
    U1DLM = (int)DL1>>8 & 0xff;
    U1DLL = (int)DL1 & 0xff;
    U1FDR = 1<<4;
  }
  U1LCR = 0;

  U1IER = 3;
  U1FCR = 7;  
  U1TER = 1<<7;
  
#if (LOGO_FLAG == MSW_LOGO)
  U1LCR = 0x1B;
#else //zkq modify 20171108
  if(Masimo_SpO2_Config_On)  U1LCR = 0x3;
  else  U1LCR = 0xB;
#endif
  
  return 1<<1;
}

unsigned char UART2_Init(unsigned int BR)
{
  Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_UART2); 
  
  U2LCR =0;
  U2IER =0;
  U2RBR =0;
  U2FCR =6;
  U2TER =0;
  
  U2LCR = 1<<7;
  DL1 = Chip_Clock_GetPeripheralClockRate() / (float)(16 * BR);
	if( ((int)(DL1*10)%10) != 0 || ((int)(DL1*100)%10) != 0 || ((int)(DL1*1000)%10) != 0)
  {
    do
    {
      FR = FR_Table1[j++];
      if(FR >=2.0f)
        return 1<<6;
      DL = (unsigned int)(Chip_Clock_GetPeripheralClockRate() / (16 * BR * FR));
      FR = (float)Chip_Clock_GetPeripheralClockRate() / (16 * BR * DL);
    }while (!(FR > 1.1 && FR <1.9f)) ;
    U2DLL = DL & 0xFF;
    U2DLM = (DL >> 8) & 0xFF;
    U2FDR=LookForTable(FR);
  }
  else
  {
    U2DLM = (int)DL1>>8 & 0xff;
    U2DLL = (int)DL1 & 0xff;
    U2FDR = 1<<4;
  }
  U2LCR = 0;

  U2IER = 3;
  U2FCR = 7;  
  U2TER = 1<<7;
  if(AA_Module_Flag.ETCO2_FLAG == 0)//zkq modify 20170306
    U2LCR = 0x03;
  else
    U2LCR = 0x1B;

  return 1<<2;
}

unsigned char UART3_Init(unsigned int BR)
{
  Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_UART3); 
  
  U3LCR =0;
  U3IER =0;
  U3RBR =0;
  U3FCR =6;
  U3TER =0;
  
  U3LCR = 1<<7;
  DL1 = Chip_Clock_GetPeripheralClockRate() / (float)(16 * BR);
	if( ((int)(DL1*10)%10) != 0 || ((int)(DL1*100)%10) != 0 || ((int)(DL1*1000)%10) != 0)
  {
    do
    {
      FR = FR_Table1[j++];
      if(FR >=2.0f)
        return 1<<7;
      DL = (unsigned int)(Chip_Clock_GetPeripheralClockRate() / (16 * BR * FR));
      FR = (float)Chip_Clock_GetPeripheralClockRate() / (16 * BR * DL);
    }while (!(FR > 1.1 && FR <1.9f)) ;
    U3DLL = DL & 0xFF;
    U3DLM = (DL >> 8) & 0xFF;
    U3FDR=LookForTable(FR);
  }
  else
  {
    U3DLM = (int)DL1>>8 & 0xff;
    U3DLL = (int)DL1 & 0xff;
    U3FDR = 1<<4;
  }
  U3LCR = 0;

  U3IER = 3;
  U3FCR = 7;  
  U3TER = 1<<7;
  U3LCR = 3;
  return 1<<3;
}
unsigned char UART4_Init(unsigned int BR) //9600 PM
{
    Int32U Fdiv;  
    PCONP_bit.PCUART4 = 1;

    U4LCR = 0x83;       /* 8 bits, no Parity, 1 Stop bit */   
        
    Fdiv = 208; 
    U4FDR_bit.MULVAL = 2;
    U4FDR_bit.DIVADDVAL = 1;
    U4DLM = Fdiv / 256;
    U4DLL = Fdiv % 256;

    U4LCR = 3;
    U4FCR = 7;  
    U4IER = 1;
    U4TER = 1;
    return 1<<2;   
}
unsigned char LookForTable(float FR)
{
  char i;
  for(i=0;i<71;i++)
  {
    if(FR >=FR_Table1[i] || FR <=FR_Table1[i+1])
    {
      static float a,b;
      a=FR-FR_Table1[i];
      b=FR_Table1[i+1]-FR;
      if(a>=0 && b>=0)
        if(a<=b)
          return DIV_Table[i]<<0 | MUL_Table[i]<<4;
        else
          return DIV_Table[i+1]<<0 | MUL_Table[i+1]<<4;   
    }
  }
  return 0;
}


void exuart_init(void)
{
    Chip_EXUART_SetChipType(TL16C550X);
    //medibus
    exuart1_init(9600,   EXUART_LCR_PARITY_EN|EXUART_LCR_PARITY_EVEN);
    //EGM/EFM
    exuart2_init(9600,   EXUART_LCR_PARITY_DIS);
    exuart3_init(9600,   EXUART_LCR_PARITY_DIS);
    exuart4_init(9600,   EXUART_LCR_PARITY_DIS);
//    NVIC_IntEnable(NVIC_GPIO);
    NVIC_EnableIRQ(GPIO_IRQn);
}

void exuart1_init(u32 BR, u32 parity)
{
    Chip_EXUART_Init(LPC_EXUART1);
    Chip_EXUART_SetBaud(LPC_EXUART1, BR);
    Chip_EXUART_ConfigData(LPC_EXUART1, EXUART_LCR_WLEN8 | 
                                        EXUART_LCR_SBS_1BIT | 
                                        parity);
    Chip_EXUART_SetupFIFOS(LPC_EXUART1, EXUART_FCR_FIFO_EN | 
                                        EXUART_FCR_RX_RS | 
                                        EXUART_FCR_TX_RS | 
                                        EXUART_FCR_TRG_LEV3);
    Chip_EXUART_IntEnable(LPC_EXUART1, EXUART_IER_RBRINT | EXUART_IER_RLSINT);
    Chip_EXUART_IntDisable(LPC_EXUART1, EXUART_IER_THREINT);
    Chip_EXUART_SetModemControl(LPC_EXUART1, EXUART_MCR_INT_EN);    
}

void exuart2_init(u32 BR, u32 parity)
{
    Chip_EXUART_Init(LPC_EXUART2);
    Chip_EXUART_SetBaud(LPC_EXUART2, BR);
    Chip_EXUART_ConfigData(LPC_EXUART2, EXUART_LCR_WLEN8 | 
                                        EXUART_LCR_SBS_1BIT | 
                                        parity);
    Chip_EXUART_SetupFIFOS(LPC_EXUART2, EXUART_FCR_FIFO_EN | 
                                        EXUART_FCR_RX_RS | 
                                        EXUART_FCR_TX_RS | 
                                        EXUART_FCR_TRG_LEV0);
    Chip_EXUART_IntEnable(LPC_EXUART2, EXUART_IER_RBRINT);
    Chip_EXUART_IntDisable(LPC_EXUART2, EXUART_IER_THREINT);
    Chip_EXUART_SetModemControl(LPC_EXUART2, EXUART_MCR_INT_EN);      
}


void exuart3_init(u32 BR, u32 parity)
{
    Chip_EXUART_Init(LPC_EXUART3);
    Chip_EXUART_SetBaud(LPC_EXUART3, BR);
    Chip_EXUART_ConfigData(LPC_EXUART3, EXUART_LCR_WLEN8 | 
                                        EXUART_LCR_SBS_1BIT | 
                                        parity);
    Chip_EXUART_SetupFIFOS(LPC_EXUART3, EXUART_FCR_FIFO_EN | 
                                        EXUART_FCR_RX_RS | 
                                        EXUART_FCR_TX_RS | 
                                        EXUART_FCR_TRG_LEV0);
    Chip_EXUART_IntEnable(LPC_EXUART3, EXUART_IER_RBRINT);
    Chip_EXUART_IntDisable(LPC_EXUART3, EXUART_IER_THREINT);
    Chip_EXUART_SetModemControl(LPC_EXUART3, EXUART_MCR_INT_EN);     
}


void exuart4_init(u32 BR, u32 parity)
{
    Chip_EXUART_Init(LPC_EXUART4);
    Chip_EXUART_SetBaud(LPC_EXUART4, BR);
    Chip_EXUART_ConfigData(LPC_EXUART4, EXUART_LCR_WLEN8 | 
                                        EXUART_LCR_SBS_1BIT | 
                                        parity);
    Chip_EXUART_SetupFIFOS(LPC_EXUART4, EXUART_FCR_FIFO_EN | 
                                        EXUART_FCR_RX_RS | 
                                        EXUART_FCR_TX_RS | 
                                        EXUART_FCR_TRG_LEV0);
    Chip_EXUART_IntEnable(LPC_EXUART4, EXUART_IER_RBRINT);
    Chip_EXUART_IntDisable(LPC_EXUART4, EXUART_IER_THREINT);
    Chip_EXUART_SetModemControl(LPC_EXUART4, EXUART_MCR_INT_EN);
}

//void EXT_UART_Init(unsigned int BR1,unsigned int BR2)
//{
//    volatile unsigned int EXT_UART_CLK;
//    EXT_UART_CLK= 8000000 / 16;
//
//    ELCR2 =0;
//    EIER2 =0;
//    ERHR2 =0;
//    ETHR2 =0;
//    EFCR2 =6;
//    EMCR2 =0;
//
//    ELCR2 = 1<<7;
//    EDLL2 = (EXT_UART_CLK / BR2) & 0xff;
//    EDLM2 = ((EXT_UART_CLK / BR2)>>8) & 0xff;
//    ELCR2 = 3;
//    EIER2 = 1;
//    EMCR2 = 0x08;
//    EFCR2 = 0 ;  
//
//    EXTUART1_Init(BR1);
//}
//
//
//void EXTUART1_Init(unsigned int BR1)
//{
//    volatile unsigned int EXT_UART_CLK;
//    EXT_UART_CLK= 8000000 / 16;
//
//    ELCR1 =0;
//    EIER1 =0;
//    ERHR1 =0;
//    ETHR1 =0;
//    EFCR1 =0;
//    EMCR1 =0;
//
//    ELCR1 = 1<<7;
//    EDLL1 = (EXT_UART_CLK / BR1) & 0xff;
//    EDLM1 = ((EXT_UART_CLK / BR1)>>8) & 0xff;
//    /*
//    bit:  7   6   5   4   3   2   1   0
//        0   0   0   1   1   0   1   1
//    */
//    ELCR1 = 0x1B;//8 0 偶校验
//    EIER1 = 1;
//    EFCR1 = 0x0F; 
//    EMCR1 = 0x08; 
//    EFCR1 = 0x09; 
//}
//
//
// void EXT_UART_RESET()
//{
//	volatile int i;
//	FIO1PIN |= 1<<12;
//  for(i=0; i<10000; i++);
//  FIO1PIN &= ~(1<<12);
//}
////外部串口
//unsigned char ReadExtUsart1(void)
//{
//    return ERHR1;
//}
//
//unsigned char ReadExtUsart1Status(void)
//{
//    return EIIR1;
//}
//void WriteExtUsart1(unsigned char data)
//{
//    ETHR1 = data;
//}
//
//unsigned char ReadExtUsart2(void)
//{
//    return ERHR2;
//}
//unsigned char ReadExtUsart2Status(void)
//{
//    return EIIR2;
//}
//void WriteExtUsart2(unsigned char data)
//{
//    ETHR2 = data;
//}
//
void UART_TxITConfig(Uart_Id Id,FunctionalState state)//使能/失能中断
{
    switch(Id)
    {
    case UART0:
        if(state == ENABLE) U0IER |= 1<<1; 
        else    U0IER &= ~(1<<1);         
        break;
    case UART1:
        if(state == ENABLE) U1IER |= 1<<1; 
        else    U1IER &= ~(1<<1); 
        break;
    case UART2:        
        if(state == ENABLE) U2IER |= 1<<1;
        else    U2IER &= ~(1<<1); 
        break;
    case UART3:
        if(state == ENABLE) U3IER |= 1<<1;
        else    U3IER &= ~(1<<1); 
        break;
    case UART4:
        if(state == ENABLE) U4IER |= 1<<1;
        else    U4IER &= ~(1<<1); 
        break; 
    default:
        break;
    }
}

unsigned char UART_CheckBusy(Uart_Id Id)
{
    unsigned char data;
    
    switch(Id)
    {
    case UART0:
        data = (U0LSR&(1<<6)) >> 6;
        break;
    case UART1:
        data = (U1LSR&(1<<6)) >> 6;
        break;
    case UART2:
        data = (U2LSR&(1<<6)) >> 6;
        break;
    case UART3:
        data = (U3LSR&(1<<6)) >> 6;
        break;
    case UART4:
        data = (U4LSR&(1<<6)) >> 6;
        break;
    }
    
    return data;
}
//
//
//
//
////{
//u8 EXTUART1_ReceiveByte(void)
//{
//    u8 data;
//    data = ERHR1;
//    return data;
//}
//
//u8 EXTUART2_ReceiveByte(void)
//{
//    u8 data;
//    data = ERHR2;
//    return data;
//}
//
//void EXTUART1_SendByte(u8 data)
//{
//    ETHR1 = data;
//}
//
//void EXTUART2_SendByte(u8 data)
//{
//    ETHR2 = data;
//}
//
//
//u8 EXTUART1_GetFlagStatus(void)
//{
//    u8 data;
//    data = ELSR1;
//    return data;
//}
//
//
//u8 EXTUART1_GetITStatus(void)
//{
//    u8 data;
//    data = EIIR1;
//    return data;
//}
//
//u8 EXTUART2_GetFlagStatus(void)
//{
//    u8 data;
//    data = ELSR2;
//    return data;
//}
//
//
//u8 EXTUART2_GetITStatus(void)
//{
//    u8 data;
//    data = EIIR2;
//    return data;
//}
//
//
//void EXTUART_TxITConfig(ExtUart_Id Id,FunctionalState state)//使能/失能中断
//{
//    switch(Id)
//    {
//    case EXTUART1:
//        if(state == ENABLE) EIER1 |= 1<<1; 
//        else    EIER1 &= ~(1<<1); 
//        break;
//    case EXTUART2:        
//        if(state == ENABLE) EIER2 |= 1<<1;
//        else    EIER2 &= ~(1<<1); 
//        break;
//    case EXTUART0:
//    default:
//        break;
//    }
//}
//
//
//unsigned char EXTUART_CheckBusy(ExtUart_Id Id)
//{
//    unsigned char data;
//    
//    switch(Id)
//    {
//    case EXTUART1:
//        data = (ELSR1&(1<<6)) >> 6;
//        break;
//    case EXTUART2:
//        data = (ELSR2&(1<<6)) >> 6;
//        break;
////    case EXTUART3:
////        data = (ELSR3&(1<<6)) >> 6;
////        break;
////    case EXTUART4:
////        data = (ELSR4&(1<<6)) >> 6;
////        break;
//    default:
//        break;
//    }
//    
//    return data;
//}
//
//
//void EXTUART1_SetBreakCtrl(u8 data)
//{
//    if(data)
//    {
//        ELCR1 |= 1<<6;
//    }
//    else
//    {
//        ELCR1 &= ~(1<<6);
//    }
//}
//
u8 UART3_CheckBusy(void)
{
    u8 data;
    data = (U3LSR&(1<<6)) >> 6;
    return data;
}

//}

void UART4_SendByte(unsigned char data)
{
    U4THR = data;
}
void UART4_SendByte_Poll(unsigned char data)
{
    UART4_SendByte(data);
}


