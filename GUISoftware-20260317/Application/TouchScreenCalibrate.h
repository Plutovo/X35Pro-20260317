#ifndef _TOUCHSCREENCALIBRATE_H
#define _TOUCHSCREENCALIBRATE_H

#include "GUI.h"
#include "GUI_Protected.h"
#include "WM.h"
#include "MainTask.h"
#include "LCDConf.h"
#include "GUITouchConf.h"
#include "Alarm_handle.h"

//touch cali 20150326
#define CALIBRATEPOINT_NUM 4//校验点
#define STAND_POINT        5//标准检测点，用于检测校验数据的正确性
#define STAND_POINT_ERR_NUM  (1)//某一个标准点连续点击1没有点中的话，从新开始校验。大于1会有问题，不解？？？
#define SCR_BKCOLOR        0x420606
#define SCR_COLOR          GUI_WHITE
//extern int LanguageType;
//int TouchScreenCalibrate = 0;//用于判断是否在触摸屏校验窗口
extern int TouchScreenCalibrate;//用于判断是否在触摸屏校验窗口
typedef struct POINT
{
    int x;
    int y;
}coordinate;

static void _DispPromptString(const char *string);
static void ClearScreen(WM_HWIN hWin,unsigned char type);
static void DrawCross(WM_HWIN hWin,U16 Xpos,U16 Ypos,GUI_COLOR color);
int _Conver_Coordinate2Ref(GUI_PID_STATE ts_state[CALIBRATEPOINT_NUM]);
void TouchAdValue_Write(void);
void TouchAdValue_Read(int flag);
void TouchPanel_Calibrate(void);

#endif