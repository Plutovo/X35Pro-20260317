#include "includes.h"
#include <intrinsics.h>
#include "MainTask.h"
/*2016.5.16  zy 修改:时钟初始化移植到clk_init()
         zy 修改:外设时钟等于系统时钟，初始化时请用驱动的底层函数
*/
char DeviceSerial_String[9] = {0};
char DeviceSerial_Table[8] = {0};

extern BREATH_SET_PARA BeathSettingPara;
//extern AA_MODULE_FLAG_REG AA_Module_Flag;

void MainTask(void);
void Init(void);

void Disable_Interrupt(void)
{

    __disable_interrupt(); 

}    
void Enable_Interrupt(void)
{
    __enable_interrupt();

}

unsigned int DeviceSerial,test;
int sck,pck,eck;   

void SystemInit(void)
{
	unsigned int *pSCB_VTOR = (unsigned int *) 0xE000ED08;

	extern void *__vector_table;

	*pSCB_VTOR = (unsigned int) &__vector_table;


	Board_SystemInit();
}


void main(void)
{
//    volatile u8 x = 0, val = 0;
    volatile unsigned int i = 0;    

    Disable_Interrupt();

//    clk_init();
        	
    IAP_Init();
    DeviceSerial = ReadDeviceSerial();
    
//    sck = Chip_Clock_GetSystemClockRate();
    
//    pck = Chip_Clock_GetPeripheralClockRate();
//    eck = Chip_Clock_GetEMCClockRate();	
    
    GPIO_Init();
    interrupt_init(); 	
    
//    EMC_Init();

    IIC_Init();
    GLCD_Init();     	
    AD_Init();

#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C) 
    UART0_Init(119200);
#else
    UART0_Init(38400); 
#endif  
    
#if (LOGO_FLAG == MSW_LOGO)
    UART1_Init(38400);	
#else   
    UART1_Init(4800);	
#endif   
    
    if(AA_Module_Flag.ETCO2_FLAG == 0)
        UART2_Init(9600);
    else 
        UART2_Init(38400);  
    
    UART3_Init(9600);       
    
#ifdef NEW_POWER_COMM
    UART4_Init(9600);//PM
#endif
    
    exuart_init();

    BeathSettingPara.BreathMode=VCV_MODE;			
    RTC_Init();			  
    TIM_Init(1000,10,10000);	

    GUI_DispStrs_Init();
    InitLanguage();
    
    uart_dp_init(ddsis_com6); //串口数据发送
    ds_init();      //数据共享   
    
    __enable_interrupt(); 
    
//    if(x == 1)
//    {
//        for(i=0; i<128*1024; i++)
//        {
//          RTEEPROMwrite(i, 0x34);
//        }
//    }
//    else if(x == 2)
//    {
//      for(i=0; i<128*1024; i++)
//      {
//        val = RTEEPROMread(i);
//        if(val != 0x34)
//        {
//          while(1);
//        }
//      }
//    }
//    
//    while(1);

   FRAM_Addr_Init();
    
   MainTask();
}

