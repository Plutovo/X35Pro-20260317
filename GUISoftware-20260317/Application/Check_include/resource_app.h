#ifndef _RESOURCE_APP_H_
#define _RESOURCE_APP_H_

#include "system_app.h"
#include "dialog.h"

//#include "Config.h"
#include <string.h>

// extern char const *(System_Display[STR_NUM]);

/*

*空间ID			#define		(GUI_ID_USER + )
*/
//Desktop button
#define DESKTOP_BUTTON_CANCLE_ID	 (GUI_ID_USER + 0)
#define DESKTOP_BUTTON_FINISH_ID	 (GUI_ID_USER + 1)

#define DESKTOP_BUTTON_DEVICE_CHECK  (GUI_ID_USER + 2)
#define DESKTOP_BUTTON_CIRCUIT_CHECK (GUI_ID_USER + 3)
#define DESKTOP_BUTTON_CANCEL_CHECK  (GUI_ID_USER + 4)

//One button dialog	
#define ONE_BUTTON_FRAMEWIN_ID		(GUI_ID_USER + 13)
#define ONE_BUTTON_CENTER_ID		(GUI_ID_USER + 14)
#define ONE_TEXT_MSG_ID				(GUI_ID_USER + 15)
#define ONE_TEXT_MSG_ID_1			(GUI_ID_USER + 19)
//Two button dialog	
#define TWO_BUTTON_FRAMEWIN_ID		(GUI_ID_USER + 16)
#define TWO_BUTTON_YES_ID			(GUI_ID_USER + 17)
#define TWO_BUTTON_NO_ID			(GUI_ID_USER + 18)

//结果text的ID
#define MANUAL_RESULT_ID                        (GUI_ID_USER + 90) //手动检测显示ID
#define BAROMETER_RESULT_ID                     (GUI_ID_USER + 100)//大气压力检测显示ID
#define GAS_SUPPLY_ID                           (GUI_ID_USER + 110)//气源检测显示ID
#define FLOW_SENSOR_ID                          (GUI_ID_USER + 120)//流量传感器检测显示ID
#define PRESSURE_SENSOR_ID                      (GUI_ID_USER + 130)//压力传感器检测显示ID
#define FG_FLOW_SENSOR_ID                       (GUI_ID_USER + 140)//EGM检测显示ID
#define OXYGEN_SENSOR_ID                        (GUI_ID_USER + 150)//氧浓度传感器检测显示ID
#define COMPLIANCE_ID                           (GUI_ID_USER + 160)//顺应性检测显示ID
#define INTERNAL_LEAKAGE_ID                     (GUI_ID_USER + 170)//泄露量检测显示ID
#define BATTERY_ID                              (GUI_ID_USER + 180)//电池检测显示ID

//#define PEEP_VALVE_ID                           (GUI_ID_USER + 140)
//#define SAFE_VALVE_ID                           (GUI_ID_USER + 150)
//#define POP_OFF_VALVE_ID                        (GUI_ID_USER + 170)
//#define ACGO_ID                                 (GUI_ID_USER + 200)

//检测页面Text的ID
#define TOTAL_TEXT_00_ID                         (GUI_ID_USER + 190)
#define TOTAL_TEXT_01_ID                         (GUI_ID_USER + 200)
#define TOTAL_TEXT_02_ID                         (GUI_ID_USER + 210)
#define TOTAL_TEXT_03_ID                         (GUI_ID_USER + 220)
#define TOTAL_TEXT_04_ID                         (GUI_ID_USER + 230)
#define TOTAL_TEXT_05_ID                         (GUI_ID_USER + 240)
#define TOTAL_TEXT_06_ID                         (GUI_ID_USER + 250)
#define TOTAL_TEXT_07_ID                         (GUI_ID_USER + 260)
#define TOTAL_TEXT_08_ID                         (GUI_ID_USER + 270)
#define TOTAL_TEXT_09_ID                         (GUI_ID_USER + 280)
#define TOTAL_TEXT_10_ID                         (GUI_ID_USER + 290)
#define TOTAL_TEXT_11_ID                         (GUI_ID_USER + 300)
#define TOTAL_TEXT_12_ID                         (GUI_ID_USER + 310)
#define TOTAL_TEXT_13_ID                         (GUI_ID_USER + 320)




/*
*数据结构			
*/
//大小总共的检测项目
#define	CHECKITEM_NUM	        (1 + 9)//手动1项  自动9项
//检测界面下提示信息最大值
#define TOTAL_TEXT_MAX_NUM      14  //djq add 20160809
//显示结果数组的大小
#define Desktop_Text_Size		20
/* 
 *Handler
 *
 *
*/
extern WM_HWIN         CheckItem_Left_HANDLE;
extern WM_HWIN         CheckItem_Right_HANDLE;
extern WM_HWIN         Desktop_Button_HANDLE;
extern WM_HWIN         CheckItem_Total_HANDLE;

extern WM_CALLBACK*		DESKTOP_CALL_OLD;	//进入程序之前桌面的回调函数
extern WM_HWIN				WM_HBKWIN_APP;	//进入程序之后，显示的主窗口


extern WM_HWIN		Dialog_OneButton_Prompt_HANDLE;	//一个按钮的一般对话框

extern WM_HWIN		Dialog_TwoButton_Prompt_HANDLE;	//两个按钮的一般对话框


extern U32	Alarm_Save_Id;  //报警ID保存，在判断是否有报警项目正在检测



//对话框显示内容
typedef struct {
	U8 Title_String[3*30];	//标题
	U8 Disp_String[7*60];	//提示内容
	U8 Button_Number;		//要显示的按键个数
	U8 Button_1[80];		//按键1显示
	U8 Button_2[80];		//按键2显示
//	U8 Button_3[2*5];
}Callback_Disp_S;
extern Callback_Disp_S Callback_Struct;






/********************************************************
 *函数名：Desktop_CheckItemText_Init
 *参数  ：void
 *返回值：无
 *被调用位置：初始化函数调用
 *功能  ：初始化当前语言下的检测项目显示
*********************************************************
*/
void Desktop_CheckItemText_Init(void);
/********************************************************
 *函数名：Disp_CheckItem_Text
 *参数  ：显示检测项目的编号
 *返回值：无
 *被调用位置：每项检测开始之前
 *功能  ：显示检测项目的名称
*********************************************************
*/
void Disp_CheckItem_Text(int num);
/***********************************************************
 *函数名  ：Disp_Desktop
 *按钮    ：取消检测
 *返回值  ：无
 *调用位置：
 *功能    ：绘制主界面
************************************************************
*/
void Disp_Desktop(void);
/***********************************************************
 *函数名  ：Create_AllGeneralDialog
 *调用位置：程序初始化位置
 *功能  ：创建程序里面所有的对话框，并且隐藏
************************************************************
*/
void Create_AllGeneralDialog(void);
/***********************************************************
 *函数名  ：Dialog_Common_Init
 *调用位置：需要修改对话框显示的地方
 *功能  ：设置将要显示的对话框的内容
************************************************************
*/
void Dialog_Common_Init(WM_HWIN hWin,const U8 *Disp,const U8 *Button1,const U8 *Button2, U8 flag);

#endif  //_RESOURCE_APP_H_