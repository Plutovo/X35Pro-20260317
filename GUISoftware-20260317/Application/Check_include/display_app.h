#ifndef _DISPLAY_APP_H_
#define _DISPLAY_APP_H_

#include "system_app.h"
#include "resource_app.h"
#include "button.h"
#include "context_app.h"



#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#if		(IDE_Flag == IAR_IDE)//IAR
	#include <stdio.h>
#endif

//全局检测标志位
#define LEFT_CHECKFLAG              (0)   //左侧检测项目
#define RIGHT_CHECKFLAG             (1)   //右侧检测项目
#define START_CHECK                 (3)   //开始检测
#define DISPLAY_RESULT              (4)   //显示上一次检测结果
#define DISPLAY_TITLE               (5)   //显示标题栏文字
//检测结果标志位
#define RESULT_SUCCESS              (0x00)   //检测结果成功  0<<2
#define RESULT_FAULT                (0x04)   //检测结果失败  1<<2
#define RESULT_SKIP                 (0x08)   //检测结果跳过  2<<2
//#define RESULT_JUMP                 (0x0C)   //检测结果跳过  3<<2

#define RESULT_MARK                 (0x0C)   //0b1100

/***********************************************************
 *函数名：Disp_Cheaking
 *参数  ：显示	"正在检测......"
 *返回值：无
 *调用位置：定时器中断中显示
 *功能  ：
 ***********************************************************
*/
void Disp_Cheaking(void);
/***********************************************************
 *函数名：Disp_Result
 *参数  ：要显示的结果的类型（数据型，结果型），
		  数据型：共三个参数，第二个为数值，第三个为单位
		  结果型：一共两个参数，第二个参数为：通过、跳过、忽略、失败...
 *返回值：无
 *调用位置：Internal_Test()
 *功能  ：内部检测结束后，进行所有的故障处理，决定是否继续运行
 ***********************************************************
*/
void Disp_Result(U8, ...);

/***********************************************************
 *函数名  ：Disp_Dialog_Start
 *标题    ：无
 *显示内容：是否执行设备预检测程序？
 *按钮    ：是-否
 *返回值  ：是或否
 *调用位置：整个程序开始前
 *功能    ：调用时显示该对话框，使用后隐藏
************************************************************
*/
U8 Disp_Dialog_Start(void);


/***********************************************************
 *函数名  ：Disp_Dialog_AirSupply
 *标题    ：气体供应测试
 *显示内容：将测试管道连接在吸气出口与呼气入口之间
 *按钮    ：确定
 *返回值  ：无
 *调用位置：air_supply.c->AirSupply_Test()
 *功能  ：调用时显示该对话框，使用后隐藏
************************************************************
*/
void Disp_Dialog_AirSupply(unsigned char flag);

/***********************************************************
 *函数名  ：Disp_Dialog_CircuitLeak_High
 *标题    ：内部泄露测试
 *显示内容：漏气过多，请检查各项连接。
 *按钮    ：重新检测-取消检测
 *返回值  ：重新检测-取消检测
 *调用位置：internal_leak.c->InternalLeak_Test()
 *功能  ：调用时显示该对话框，使用后隐藏
************************************************************
*/
void Disp_Dialog_CircuitLeak_High(unsigned char flag);

/***********************************************************
 *函数名  ：Disp_Dialog_PressureSensor_Fail
 *标题    ：压力传感器线性错误
 *显示内容：
 *按钮    ：重新检测-取消检测
 *返回值  ：重新检测-取消检测
 *调用位置：
 *功能  ：调用时显示该对话框，使用后隐藏
************************************************************
*/
U8 Disp_Dialog_PressureSensor_Fail(void);

/***********************************************************
 *函数名  ：Disp_Dialog_Battery_DisconnectAC
 *标题    ：电池切换测试
 *显示内容：请拔出电源电缆
 *按钮    ：跳过检测
 *返回值  ：跳过检测
 *图标    ：电池和电源标志
 *调用位置：battery.c->Battery_Test();
 *功能  ：非阻塞创建对话框，并且显示
************************************************************
*/
void Disp_Dialog_Battery_DisconnectAC(void);

/***********************************************************
 *函数名  ：Disp_Dialog_Battery_ConnectAC
 *标题    ：电池切换测试
 *显示内容：请插入电源电缆
 *按钮    ：无
 *返回值  ：无
 *图标    ：电池和电源标志
 *调用位置：battery.c->Battery_Test();
 *功能  ：非阻塞创建对话框，并且显示
************************************************************
*/
void Disp_Dialog_Battery_ConnectAC(void);

void Disp_Dialog_ACGO_Prompt(void);
/***********************************************************
 *函数名：CheckItem_TextSet
 *参数  ： CheckFlag  检测项目组的标志  LEFT_CHECKFLAG：设备检测    
                                        RIGHT_CHECKFLAG：气密性检测
           Operation  进行的操作        START_CHECK:开始检测前，将屏幕清空        
                                        DISPLAY_RESULT:显示当前检测结果
           BasicAddr  显示数据的基地址
 *返回值：无
 *调用位置：
 *功能  ：
 ***********************************************************
*/
void CheckItem_TextSet(int CheckFlag,int Operation,int BasicAddr);


void Result_DispClear(WM_HWIN hWin,U16 WidgetID,U8 Type);
void Result_DispTesting(WM_HWIN hWin,U16 WidgetID,U8 Type);
void Result_DispState(WM_HWIN hWin,U16 WidgetID,U8 Type,U8 State,const char * Name,U8 Code);//√  Pass  0xXX
void Result_DispValue(WM_HWIN hWin,U16 WidgetID,U8 Type,U8 State,const char * Name,const char * Value,const char *Unit,U8 Code);
void Result_DispPrompt(WM_HWIN hWin,U16 WidgetID,U8 Type,const char *Name,const char *Prompt);//×  Fail  PromptString

//显示当前位置的结果
//void Disp_ItemResult(PositionFlag Pos);




#endif  //_DISPLAY_APP_H_