#ifndef _WIDGET_MULTIPAGE_H
#define _WIDGET_MULTIPAGE_H
 
#include <stdlib.h>
#include "GUI.h"
#include "MULTIPAGE.h"
#include "TextExCtrl.h"
#include "DROPDOWN.h"
#include "MainTask.h"
#include "PROGBAR.h"
#include "Static_Menu_Creat.h"
#include "Alarm_handle.h"
#include "DATABAR.h"
#include "MultiColumnWin.h"
#include "MENU.h"
#include "DATA_FRAME.h"
#include "MultiPage_Button.h"
#include "ButtonRotation.h"
#include "ImageText_Private.h"
#include "cbdialog32.h"
#include "CO2_IRMA.h"
#include "C002_CO2.h" //  20130328
#include "AddFunETCO2.H" //  20130705
#include "Communication_Handle.h"  //sun add 20140514
#include "comm.h"  //sun add 20140514

//
#define   RECORD_EVERY_PAGE             10

extern int StandbyState;
//extern unsigned int LanguageType;
extern int ETCO2Unit; //  20130409
extern int preETCO2Unit; //  20130607
extern unsigned int delay_time[5];
extern int MenuType;
extern int PlanMenuType;
extern int Task;
//int CurveType = PRESSURE_AND_FLOW;
extern MENU_HWIN Menu_hWin;
extern BREATH_SET_PARA BeathSettingPara;
extern MONITOR_PARA MonitorPara;
extern ALARM_SET_PARA AlarmSettingPara;
extern VERSION_PARA SystemVersion,GUIVersion,BDUVersion,PMUVersion; //  20130328
extern RTC_TIMER RTCTime;
extern Int8U SaveOperationTimeFlag; //  20130328
extern int MenuOperateStatus;
extern int CalibrationType;
extern int StandbyState;
extern ALARM_RECORD_LIST AlarmRecordList;
extern struct list_head    alarmlog_head;
extern OPERATION_TIME OperationTime;
extern unsigned short int  LedUpdateRegister;
extern unsigned  char    USART0_TXD_Busy_FLAG;
extern unsigned long long  USART0_TXD_RIGSTER;
extern PARA_SETTING_STATE ParaSettingState;

extern GUI_CONST_STORAGE GUI_BITMAP bmlayout1;
extern GUI_CONST_STORAGE GUI_BITMAP bmlayout2;
extern GUI_CONST_STORAGE GUI_BITMAP bmStandby_Icon;
extern GUI_CONST_STORAGE GUI_BITMAP bmStandby_Icon_Turkish;//2013-12-14
#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C) 
extern const GUI_BITMAP bmmodel;
#endif
extern GUI_CONST_STORAGE GUI_BITMAP bmHigh_limit;
extern GUI_CONST_STORAGE GUI_BITMAP bmLow_limit;
extern GUI_CONST_STORAGE GUI_BITMAP bmWarning;
extern GUI_CONST_STORAGE GUI_BITMAP bmlittle_standby;
extern GUI_CONST_STORAGE GUI_BITMAP bmlittle_standby_Turkish;//2013-12-14
//
extern void CurveTypeChanged(int type);
//

extern C002_CO2_PARAMETERS CO2_Parameters;//  20130510

//extern unsigned char SpO2ModuleExist;
extern unsigned char SpO2ModuleOff;
//extern unsigned char EtCO2ModuleExist;
extern unsigned char EtCO2ModuleOff;
extern unsigned char O2ModuleOff;//djq add 20131226
extern Int16U LowAlarmPriority;//sun add 20141224
//extern int ManualModeFlag;

//sun add 20151102
extern unsigned char pHaseinMoudleCheck;//检测pHasein是否连接标志
extern IM_ALL_DATA      * pAll_Data;
extern Int8U   ETCO2_FLAG;
extern char Usart2_Tx_Buffer[20];
extern float MultiAA[2];

//报警清除标志，如果报警被清除则不在检测
extern unsigned char O2ModuleClear_FLag;
extern unsigned char SpO2ModuleClear_FLag;
extern unsigned char EtCO2ModuleClear_FLag;

//选配-----------------
extern unsigned char Mode_Chose_Flag; //0: off  1:on
extern unsigned char VT_Min_Flag ;     //0: 50    1:20
extern unsigned char VS_Mode_Flag;
extern unsigned char APRV_Mode_Flag;
extern unsigned char PRVCMode_Chose_Flag;

extern unsigned char FlowPause_Config_On;
extern unsigned char LungRec_Config_On;

extern unsigned char DriverGas_Chose_Flag; //0: off  1:on  djq add 20170609

extern unsigned char Paramagnetic_O2_Flag;
extern unsigned char FiO2_Config_On;  //氧浓度传感器  1：on  0：off //ylk add 20180314 FiO2选配


extern unsigned char EtCO2_Config_On; //0:关闭   1：开启
extern unsigned char EtCO2_KM7002_Config_On;
extern unsigned char Masimo_EtCO2_Config_On; //0:关闭   1：开启
extern unsigned char MasimoISA_EtCO2_Config_On;
extern unsigned char Drager_EtCO2_Config_On;

extern unsigned char SpO2_Config_On;  //0:关闭   1：开启
extern unsigned char Masimo_SpO2_Config_On; //0:关闭   1：开启

//-------------------------------------
//软件授权码
extern const char HexTable[16];
extern char DeviceSerial_String[9];
extern char DeviceSerial_Table[8];


extern int CalibrateBatteryFlag;//  20130328
//extern int AcgoModeFlag; //  20130415
extern int TouchScreenCalibrate;
//
//extern int PowerState; //  20130426

extern int LastStandbyState;  //sun add 20140514
//
extern const int IE_RatioTable[];//  20130617
//
extern ALARM_WORK AlarmsWork; //  20130705

extern REALTIME_PARA RealtimePara;
extern int BackUpMode;
extern char alarm_battery_cali_index;


extern BATTERY_AD_PARA  BatteryPara;


//
static void ModesKeyInit(WM_HWIN hDialog);
static void ModesSettingConfirm(WM_HWIN hDlg, BREATH_SET_PARA *BeathPara);
void FunctionKeyPressedResume(WM_HWIN hDlg,int Id);
static void AlarmLimitParaInit(WM_HWIN hDlg,ALARM_SET_PARA *Para,int index);
static void VentParaWinInit(WM_HWIN hDlg,BREATH_SET_PARA * Parameter);
static int AlarmParameterLimitAnalysis(ButtonRotation_Handle hObj,int page,int direction,ALARM_SET_PARA *Para);
static void RectPaintParaInit(WM_HWIN hDlg);
static void CurveMenuInit(CURVE_HWIN *hWin,WM_HWIN hDlg);
static void ChangeMultiPageText(int Type,WM_HWIN ParentWin);

void EmptyAlarmRecordList(ALARM_RECORD_LIST * ListHead);  //sun add 20140701
void ReviseParamatersAllMode(int mode,BREATH_SET_PARA *Para,int Ph); //sun add 20140702
WM_HWIN _SystemHandleWin(void);
HOME_MENU_HWIN _HomeMenuWinCreat(void);
MENU_Handle _GraphicMenuCreat(CURVE_HWIN *hObj);
WM_HWIN _MonitorParameterWin(void);
WM_HWIN _VentParaWinCreat(void);
WM_HWIN _AffirmO2Choose(void);
WM_HWIN _OxygenSensorCalWin(void);
WM_HWIN _PasswordInputWin(void);
WM_HWIN _EngineerCalibrationWin(void);
WM_HWIN FunctionMenuButtonCreat(void);
WM_HWIN _StandbyWinCreat(void);
WM_HWIN _SelfCheck_AskCreat(void);
WM_HWIN _VentParaWinCreat(void);
WM_HWIN _ModesSettingWin(void);
WM_HWIN _AlarmsHandleWin(void);
WM_HWIN _SystemHandleWin(void);
WM_HWIN _MonitorParameterWin(void);
MENU_Handle _GraphicMenuCreat(CURVE_HWIN *hObj);
WM_HWIN ActivateStandbyWinCreat(void);
static void CurveMenuInit(CURVE_HWIN *hWin,WM_HWIN hDlg);
void UpdateProgramWin(WM_HWIN hWin);
WM_HWIN _EngineerUpdateWin(void);
WM_HWIN _BatteryCalibrationWin(void);
WM_HWIN _SoftwareOptionalWin(void);
WM_HWIN PhaseinZeroCail(void);
WM_HWIN _TrendWindowCreat(void);
int CheckSighTimesTable(int data);
void MULTIPAGE_SetPageChangeEnable(MULTIPAGE_Handle hObj, unsigned int flg);
void ReviseParamatersAllMode_Normal(int mode,BREATH_SET_PARA *Para,int Ph);

//
static void AlarmSettingValueUpdate(int data,int page,ALARM_SET_PARA *Para,HOME_MENU_HWIN *hWin,int WorkState,int Id);
extern int OS_TimeMS;
extern void Co2ModuleOff(unsigned int off);//  20130412
extern void EXT_UartTransmit(Int8U DataType,const Int8U DataLength,Int8U UsartPort);
int Rounding(float fData, int nNarrowMul);

#endif