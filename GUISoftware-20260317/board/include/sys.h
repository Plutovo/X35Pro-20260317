//
/***************************************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2007
 *
 *    File name   : sys.c
 *    Description : System module include file
 *
 *    History :
 *    1. Date        : 13, February 2008
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *
 *    $Revision: 22761 $
 **************************************************************************************************/
#ifndef __SYS_H
#define __SYS_H
#include "board.h"
#include "Static_Menu_Creat.h"
//#define FILTER_NUM  10
#define ALARM_FRQ 800

#define	VER_SERIAL 100



void InitClock(void);
Int32U SYS_GetFsclk(void);
Int32U Chip_Clock_GetSystemClockRate(void);


void USART2_printf(INT8U *Data,...);//  20130329




void VolumeChannelSelect(Int8U channel);
void BeepAloudControl(Int8U status);
void I2C_GPIO_Init(void);
void RTCIntHandler (void);
#endif // __SYS_H
//
//
