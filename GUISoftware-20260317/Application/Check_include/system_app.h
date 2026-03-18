#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include "protocol_app.h"
#include "display_app.h"
#include "error_app.h"
#include "serial_app.h"
#include "context_app.h"
#include "resource_app.h"
#include "MainTask.h"  //R50 头文件
#include "Alarm_handle.h"
#include "Process_Widget.h"
//#include "Debug_App.h"

#include "DrawEffectLine.h"
#include "co2_irma.h"

#include "LCDConf.h"

#include "DispStrs.h"

#include <stddef.h>




//测试标志位
extern U8 CHECK_DEBUG; //
//extern unsigned char AA_Module_Flag.EtCO2ModuleExist;

void Sys_Init(void);
/***********************************************************
 *函数名  ：Init_GlobalParameter
 *调用位置：每次检测之前，和第一次进入自检界面的时候
 *参数：Choose： DISPLAY_RESULT： 检测日志初始化
                 START_CHECK： 开始检测前初始化
 *功能  ：保留控件之前的设置，并且修改控件的默认属性
************************************************************
*/
void Init_GlobalParameter(U8 Choose,U8 CheckItem_Flag);

//void My_Delay_App(int Period);
#define My_Delay_App GUI_Delay
/*
*时间调度函数：由于不能使用中断，所以这个函数需要加在运行时间较长的函数中（特别是循环函数）
*显示一些实时变化的内容
*/
int My_GetTime(U8 disp_flag);
#endif //_SYSTEM_H_