#ifndef __INCLUDES_H
#define __INCLUDES_H

extern unsigned char UART_Test_SW;
typedef unsigned int uint32_t;
            /*
#include "Static_Menu_Creat.h"
#include "alarm_handle.h"
#include "sys.h"
#include "MainTask.h"
#include "Communication_Handle.h"
#include "Data_Handle.h"
#include "Alarm_Handle.h"
#include "arm_comm.h"
#include "TracingCurve.h"
        */

//#include "CLK.h"
//#include "EMC.h"
#include "LED_GPIO.h"
#include "iolpc1788 and 2478.h"
#include "LCD1.h"
#include "NVIC.h"
#include <string.h>
#include "TIM.h"
#include <intrinsics.h>
#include "DMA.h"
#include "GUI.h"
#include "IIC.h"
#include "TEST.h"
#include "UART.h"
#include "RTC.h"
#include "AD.h"
#include "IAP.h"
#include "GUI.h"
#include "test.h"
#include "board.h"
#include "NVIC.h"
#include "PWM.h"
#include "exuart_17xx_40xx.h"			
#include "uart_dp.h"	
#include "clock_17xx_40xx.h"


#define GUI_NUMBYTES  1024 * 1024 * 5
#define GUI_BLOCKSIZE 128
#define WINDOW_MEMDEV 0
#define Pi 3.1415f
#define Battery_avg_num 100


void init(void);




extern Int8U   ETCO2_FLAG;  
extern unsigned int Chip_SN;
#endif
