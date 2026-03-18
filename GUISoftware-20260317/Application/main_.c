/*******************************************************************************************************
 *
*    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : main.c
 *    Description : Define main module
 *
 *    History :
 *    1. Date        : 12, September 2007
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *  This example project shows how to use the IAR Embedded Workbench for ARM
 * to develop code for the IAR LPC-2468 board. It shows basic use of the I/O,
 * the timer and the interrupt controllers.
 *  It starts by blinking USB Link LED.
 *
 * Jumpers:
 *  J5      - depending of power source
 *  ISP     - unfilled
 *  nRESET  - unfilled
 *  EINT0   - filled
 *
 * Note:
 *  After power-up the controller get clock from internal RC oscillator that
 * is unstable and may fail with J-Link auto detect, therefore adaptive clocking
 * should always be used. The adaptive clock can be select from menu:
 *  Project->Options..., section Debugger->J-Link/J-Trace  JTAG Speed - Adaptive.
 *
 *    $Revision: 22761 $
 **************************************************************************/
#include <intrinsics.h>
#include <stdio.h>
#include "board.h"
#include "sys.h"
#include "EXT_UART.h"
#include "sdram_32M_16bit_drv.h"
#include "glcd_drv.h"
#include "MainTask.h"

char DeviceSerial_String[9] = {0};
char DeviceSerial_Table[8] = {0};

extern BREATH_SET_PARA BeathSettingPara;

extern void MainTask(void);
//

void Disable_Interrupt(void)
{
    __disable_interrupt();  
}
void Enable_Interrupt(void)
{
    __enable_interrupt();
}
/***************************************************************************************************
 * Function Name: main
 * Parameters: none
 *
 * Return: none
 *
 * Description: main
 *debugno_cse doubledlib_config 
 *************************************************************************/
void main(void)
{
  int i = 0;
    // MAM init
    Disable_Interrupt();
    MAMCR_bit.MODECTRL = 0;
    MAMTIM_bit.CYCLES  = 3;   // FCLK > 40 MHz
    MAMCR_bit.MODECTRL = 2;   // MAM functions fully enabled
    //GLCD_Ctrl (FALSE);
    // Init clock
    InitClock();
    if(1)     
    {  
        memcpy((INT32U *)0x40000000, (INT32U *)0x00008000,64);       
        MEMMAP = 0x02;    //中断向量表设置在RAM中                   
    }
    // Init GPIO
    GpioInit();
    // Init VIC
    VIC_Init();
    // SDRAM Init
    SDRAM_Init();
    I2C_GPIO_Init();
    GLCD_Init ();
    GLCD_Ctrl (TRUE);
#ifdef MACHINE_TYPE_FLAG
    Uart0_Init(119200); //119200 //cpp md 20130718
#else
    Uart0_Init(38400); //119200 //cpp md 20130718
#endif
#if (LOGO_FLAG == MSW_LOGO)
    Uart1_Init(38400);
#else
    Uart1_Init(4800);
#endif
    Uart2_Init(38400);//9600 //cpp md 20130328
    Uart3_Init(9600);
    BeathSettingPara.BreathMode=VCV_MODE;
    //EXT_USART_Init();
    EXT_Single_USART_Init();
    Adc_Init();
    Timer0_Init(); 
    Timer1_Init();
    Timer2_Init();
    PWM0_Init();
    PWM_Set(500);
    PWM_Start();
    //VolumeChannelSet();
    //BeepAloudControl(ENABLE); //cpp del 20130328
    RTC_Init();
    PWM_Stop();
    Enable_Interrupt();

    
    MainTask();
}
