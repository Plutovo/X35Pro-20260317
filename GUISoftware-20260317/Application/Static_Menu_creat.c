//
#include "Static_Menu_Creat.h"
#include "MainTask.h"
#include "Alarm_handle.h"
#include "GRAPH_Private.h"
#include "MENU.h"
#include "JM1_SpO2.h"
#include "C002_CO2.h"
#include "AddFunETCO2.H" //  20130705
#include "DispStringLib.h"
#include "CO2_IRMA.h"
#include "BDU_comm.h"
#include "MasimoSPO2.h"
#include "filter.h"
#include "WIDGET_Tools.h"
#include "PowerComm.h"
#include "BubbleCtrl.h"
#include "PawGauge.h"
//

extern KEY_MSG KeyMsg;
extern BREATH_SET_PARA BeathSettingPara;
extern MONITOR_PARA MonitorPara;
extern int MenuType;
extern int Wm_Exec_Flag;
extern REALTIME_PARA RealtimePara;
extern ALARM_SET_PARA AlarmSettingPara;
extern WM_HWIN Focus_hWin;extern int BackUpMode; //= VCV_MODE; //cpp md 20130513
extern int StandbyState;
extern RTC_TIMER RTCTime;
extern WM_HWIN SELFWin;
extern unsigned char N2O_Switch;
//
extern GUI_CONST_STORAGE GUI_BITMAP bmlung;
extern GUI_CONST_STORAGE GUI_BITMAP bmsilence_icon;
extern GUI_CONST_STORAGE GUI_BITMAP bmcaution;
extern GUI_CONST_STORAGE GUI_BITMAP bmfreeze;
extern GUI_CONST_STORAGE GUI_BITMAP bmcompany_icon;
extern GUI_CONST_STORAGE GUI_BITMAP bmdrive_gas_O2;
extern GUI_CONST_STORAGE GUI_BITMAP bmdrive_gas_Air;

extern GUI_CONST_STORAGE GUI_BITMAP bmUnlocked_WB_Icon;//ylk add 20180505 new lock icon 
extern GUI_CONST_STORAGE GUI_BITMAP bmLocked_WB_Icon;//ylk add 20180505 new lock icon 

extern GUI_CONST_STORAGE GUI_BITMAP bmCPB_ImageInfo_CN;
extern GUI_CONST_STORAGE GUI_BITMAP bmCPB_ImageInfo_EN;

extern unsigned char Masimo_EtCO2_zhuliu;
extern unsigned char Masimo_EtCO2_zhuliuco2;
extern unsigned char MasimoISA_EtCO2_pangliuO2;
extern unsigned char MasimoISA_EtCO2_pangliu;
extern unsigned char Masimo_EtCO2_pangliuco2;
//
extern MENU_HWIN Menu_hWin;
//int PowerState = AC_WORK;
int BatteryStateNum = 0;
extern int LastStandbyState;
extern int SilenceTimer=120;
extern ALARM_INFO ShowedAlarmInfo;
extern ALARM_WORK AlarmsWork;
extern ALARM_RECORD_LIST AlarmRecordList;
extern struct list_node AlarmRecord;;
extern OPERATION_TIME OperationTime;
unsigned int AssistFunctionOperationFlag = 0;
extern unsigned char ChargeFinishFlag;
extern int CurveType;
extern int MenuOperateStatus;
extern int ETCO2Unit; //  20130513

extern CO2_S CO2_Disp;//djq add 20170526

extern unsigned char DirverGasFlag;//驱动气体类型标志
extern unsigned char oxyChooseFlag;
//unsigned char ACGOModeFlag = 0;
//unsigned char MANUALModeFlag = 0;
unsigned char AA_Max = 0;
unsigned char AA2_Max = 0;

extern unsigned char DriverGas_Chose_Flag; //0: off  1:on  djq add 20170609驱动气选配标志
unsigned char SpO2ModuleExist = 0;
unsigned char SpO2ModuleOff = 1; //1:关闭  0:开启
extern unsigned char MasimoISA_EtCO2_Config_On;
extern unsigned char Masimo_SpO2_Config_On;
/*
0xXX
bit1:masimo
bit0:国产CO2
*/

unsigned char EtCO2ModuleOff = 0; //0:关闭  1:开启
unsigned char O2ModuleOff = 0; //djq add 20131226   氧浓度传感器，不需要检测是否存在。该标志位开关标志位。

extern unsigned char Masimo_EtCO2_Config_On;
extern unsigned char MasimoISA_EtCO2_Config_On;
unsigned char AssistShowFlag = 0;//肺触发显示标志


//zkq 20161028 波形字体更新
unsigned char PAA_Update0[20];
unsigned char SAA_Update0[20];

unsigned char PAA_Update1[20];
unsigned char SAA_Update1[20];

unsigned char PAA_Update2[20];
unsigned char SAA_Update2[20];

unsigned char AskMenu_Flag = 1;
int TouchScreenCalibrate = 0;

extern const int IE_RatioTable[];//  20130401

extern HOME_MENU_HWIN _HomeMenuWinCreat(void);
//*//  20130401s
extern WM_HWIN _ModesSettingWin(void);
extern WM_HWIN _AlarmsHandleWin(void);
extern WM_HWIN _SystemHandleWin(void);
extern WM_HWIN ActivateStandbyWinCreat(void);
//*/
extern int Barameter_Pressure;   //设置信息界面，大气压力值   0~999mmHg    djq 20140310
extern int Barameter_Stand_Pressure;   //校验地大气压力值   0~999mmHg
extern int Barameter_Switch;   //0 ：大气压传感器开，显示为BDU上传的值   1：关，显示为设置值。  djq 20140310 

extern unsigned char StandbyClear_FLag;
extern unsigned short int  LedUpdateRegister;
//extern int BreathMode_BeforeManual;
extern unsigned char SpO2_Choose;//1：国产   2：Masimo
//extern const char ** cbdialog32string[44];  //sun add 20140522
extern char  contorlText[50];  //sun add 20140523
extern int PlanMenuType;
extern int Task;
void Get_VentParaInfoString(int Disp_Type,float Data);
void Get_VentIEInfoString(int tinspData);
int IEConvertTi(int Mode,int SettingValue, BREATH_SET_PARA *BreathPara);
void TiConvertIE(int SettingValue, BREATH_SET_PARA *BreathPara);
void _ControlTextShow(char * str);

WM_HWIN AlarmInfoWin;//屏幕上方报警字符区域

//*//   20130616
extern VENT_TIME VentTime;//  20130616
//extern float GetIERatio(VENT_TIME _VentTime); //  20130616 //cpp del 20130716
//*///
//sun add 20151102
JM1_SPO2_PARAMETERS SpO2_Parameters = {0};
C002_CO2_PARAMETERS CO2_Parameters  = { 0,0,0,0,0};
extern IM_ALL_DATA      * pAll_Data;
extern float MAC_Base[];

//#ifdef LANGUAGE_CHINESE_FLAG
//extern const char AAName[14][16];
//#else
extern const unsigned short AAName[14];
//#endif

extern char		Usart2_Tx_Buffer[20];
extern AA_PARAMETERS AA_Para;
extern unsigned char pHaseinMoudleCheck;//检测pHasein是否连接标志
extern Int16U  AlarmPriority;
extern GUI_COLOR N2OColor;
extern U8 CHECK_DEBUG;//0：正常  2：开机正式进入界面之前  1：待机
ALARM_SAVEVENT_PARA Alarm_SaveVent_para;
extern BREATH_SET_PARA Breath_Setting_Tmp;//ylk add 20181009
extern unsigned char PresetMode_Flag;
extern char ModeChange_Count;
extern ButtonExCtrl_Handle BreathSettingParaHandle[8];
extern unsigned char Drager_EtCO2_Config_On; //0:关闭   1：开启麻醉气体    2：开启麻醉气体+顺磁氧 
extern WM_HWIN _StandbyWinCreat(void);
extern unsigned int  Vt_HighLimit;
extern unsigned int  Vt_LowLimit;
extern GUI_CONST_STORAGE GUI_BITMAP bmACGO;

extern int Rounding(float fData, int nNarrowMul);//  [2/14/2022 yaoyao]
//////////Demo////////////////////////////////////////////////////////////////
#ifdef DEMO   //  [11/8/2022 lyy add]
extern unsigned char Demo_Switch;
extern char Demo_Change_Cycle;
extern GUI_COLOR Curve_Color;
#endif

//////////////////////////////////////////////////////////////////////////
unsigned char ShutDownAskMenu_Flag = 0;//关机询问界面标志
unsigned char Receive_ShutDown_Flag = START_UP;//接收关机状态标志
unsigned char SendShutDownState = 0;

/***************************************************************************************************
* @brief  菜单切换函数，进入或退出菜单都执行此函数
* @para   
* @retval 
*/
MENU_HWIN StaticMenuSwitch(int Menu,MENU_HWIN *Menu_Obj)
{
    static int LastMenuType = 0;
    static int StaticStandbyState = 0xFF;
    WM_HWIN hItem,hWin;

	if(LastMenuType == ALARM_MENU)//ylk add 20181128
	{
		Menu_hWin.Alarm_Win.AlarmWin_Info = 0;
		Menu_hWin.Alarm_Win.AlarmWin_Lim1 = 0;
		Menu_hWin.Alarm_Win.AlarmWin_Lim2 = 0;
		Menu_hWin.Alarm_Win.AlarmWin_Lim3 = 0;
	}

	if(LastMenuType == PATIENT_DATA_ASK_MENU)
	{
		WM_DeleteWindow(Menu_Obj->ActivedWin);
		Menu_Obj->ActivedWin = 0;
		GUI_Clear();//
	}

    if(Menu==MAIN_MENU)//由其他菜单进 入工作状态 或 按下HOME KEY 
    {
        if(LastMenuType==STANDBY_MENU)
        {
            StandbyState=0;
            StandbyClear_FLag = 0;
            SendOperationMessage(EXIT_STANDBY,USART0);
#ifdef DEMO  //  [11/8/2022 lyy add]
			if (Demo_Switch)
			{
				Demo_Change_Cycle = 1;
			}
#endif
            LedUpdateRegister &= ~STANDBY_LED;
            SendOperationMessage(LED_UPDATE_OPERATION,USART3);

			WM_DeleteWindow(Menu_Obj->Swin);
			Menu_Obj->Swin = 0;
            Menu_Obj->HomeWin = _HomeMenuWinCreat();
            WM_ShowWindow(WM_GetDialogItem(Menu_Obj->hWin,GUI_ID_BUTTON3));


			Menu_hWin.InfoWin.InfoChangeFlag |= CPB_CHANGE_FLAG;
        }
        else if(LastMenuType == WAVEFORM_SETTING_MENU)
        {
            WM_DeleteWindow(Menu_Obj->ActivedWin);
			Menu_Obj->ActivedWin = 0;
            Menu_Obj->cWin.FocusCurve = -1; 
        }
        else
        {

			if (LastMenuType == TOOLS_MENU)
			{
				WM_DeleteWindow(Menu_Obj->ToolsWin.LungWin);
				Menu_Obj->ToolsWin.LungWin = 0;
			}

			WM_DeleteWindow(Menu_Obj->ActivedWin);
			Menu_Obj->ActivedWin = 0;
			BUTTON_SetPressed( WM_GetDialogItem(Menu_hWin.hWin, GUI_ID_BUTTON7), 0);//#1224
			StandbyState=0;
			if(StaticStandbyState!=StandbyState)
			{
                StandbyClear_FLag = 0;
                SendOperationMessage(EXIT_STANDBY,USART0);
#ifdef DEMO //  [11/8/2022 lyy]
				if (Demo_Switch)
				{
					Demo_Change_Cycle = 1;
				}
#endif
                LedUpdateRegister &= ~STANDBY_LED;
                SendOperationMessage(LED_UPDATE_OPERATION,USART3);

				WM_DeleteWindow(Menu_Obj->Swin);
				Menu_Obj->Swin = 0;
                Menu_Obj->HomeWin = _HomeMenuWinCreat();
                WM_ShowWindow(WM_GetDialogItem(Menu_Obj->hWin,GUI_ID_BUTTON3));
            }
            else
            {
                TracingCurve_SetXScale(Menu_hWin.HomeWin.Tobj[0],1);
                TracingCurve_SetXScale(Menu_hWin.HomeWin.Tobj[1],1);
                TracingCurve_SetXScale(Menu_hWin.HomeWin.Tobj[2],1);
				TracingCurve_SetXScale(Menu_hWin.HomeWin.Tobj[3],1);
            }
        }

        MenuType = MAIN_MENU;
        LastMenuType = MAIN_MENU;
        StaticStandbyState = StandbyState;
    }
    else if(Menu==WAVEFORM_SETTING_MENU)//进入 波形设置菜单
    {
        if(LastMenuType!=MAIN_MENU)
        {
            if(WM_IsWindow(Menu_Obj->ActivedWin)) {
                WM_DeleteWindow(Menu_Obj->ActivedWin);
                Menu_Obj->ActivedWin = 0;
            }
        }
        Menu_Obj->ActivedWin = _GraphicMenuCreat(&Menu_Obj->cWin);
        WM_BringToBottom(Menu_Obj->HomeWin.ParentWin);
        MenuType = WAVEFORM_SETTING_MENU;
        LastMenuType = WAVEFORM_SETTING_MENU;
    }
    else if(Menu==MODES_MENU)//进入 模式菜单
    {
        if(  (LastMenuType == ENGINEER_MENU) 
          || (LastMenuType == ENGINEER_CALIBRATION_MENU)
          || (LastMenuType == UPDATE_PROGRAM_MODE)  //sun add 20140514
		  || (LastMenuType == OXYGEN_SENSOR_CAL_MENU) //sun add 20140625
		  || (LastMenuType == OXY_CHOOSE_MENU)
          || (LastMenuType == PHASEIN_ZERO_CAL_MENU)  //sun add 20151103
		  || (LastMenuType == MEDIBUS_COMM_MENU)
          || (LastMenuType == BATTERY_CALIBRATION_MENU)
		  ) //sun add 20140625
        {
            WM_DeleteWindow(Menu_Obj->RecordWin);
			Menu_Obj->RecordWin = 0;
            MenuOperateStatus = 0;
        }
        if(LastMenuType != MODES_MENU)
        {
			if (LastMenuType == TOOLS_MENU)
			{
				WM_DeleteWindow(Menu_Obj->ToolsWin.LungWin);
				Menu_Obj->ToolsWin.LungWin = 0;
			}

            if(LastMenuType == WAVEFORM_SETTING_MENU)
            {
                Menu_Obj->cWin.FocusCurve = -1;
                WM_DeleteWindow(Menu_Obj->ActivedWin);
                Menu_Obj->ActivedWin = 0;
            }
            if((LastMenuType!=MAIN_MENU) && (LastMenuType!=STANDBY_MENU))
            {
                WM_DeleteWindow(Menu_Obj->ActivedWin);
                Menu_Obj->ActivedWin = 0;
            }
            Menu_Obj->ActivedWin = _ModesSettingWin();
			//WM_BringToBottom(Menu_Obj->HomeWin.ParentWin);
			if(StandbyState)
			{
				WM_BringToBottom(Menu_Obj->Swin);
			}
			else
			{
				WM_BringToBottom(Menu_Obj->HomeWin.ParentWin);
			}
		}

        MenuType = MODES_MENU;
        LastMenuType = MODES_MENU;
    }
    
	else if(Menu==TOOLS_MENU)
	{
		if(  (LastMenuType == ENGINEER_MENU) 
			|| (LastMenuType == ENGINEER_CALIBRATION_MENU)
			|| (LastMenuType == UPDATE_PROGRAM_MODE)  //sun add 20140514
			|| (LastMenuType == OXYGEN_SENSOR_CAL_MENU) //sun add 20140625
			|| (LastMenuType == OXY_CHOOSE_MENU)
			|| (LastMenuType == PHASEIN_ZERO_CAL_MENU)  //sun add 20151103
			|| (LastMenuType == MEDIBUS_COMM_MENU)
			|| (LastMenuType == BATTERY_CALIBRATION_MENU)
			) //sun add 20140625
		{
			WM_DeleteWindow(Menu_Obj->RecordWin);
			Menu_Obj->RecordWin = 0;
			MenuOperateStatus = 0;
		}
		if(LastMenuType != TOOLS_MENU)
		{
			if(LastMenuType == WAVEFORM_SETTING_MENU)
			{
				Menu_Obj->cWin.FocusCurve = -1;
				WM_DeleteWindow(Menu_Obj->ActivedWin);
				Menu_Obj->ActivedWin = 0;
			}
			if((LastMenuType!=MAIN_MENU) && (LastMenuType!=STANDBY_MENU))
			{
				WM_DeleteWindow(Menu_Obj->ActivedWin);
				Menu_Obj->ActivedWin = 0;
			}
			Menu_Obj->ActivedWin = _ToolsHandleWin();
			WM_BringToBottom(Menu_Obj->HomeWin.ParentWin);
		}

		MenuType = TOOLS_MENU;
		LastMenuType = TOOLS_MENU;
	}

    else if(Menu==ALARM_MENU)//进入 报警菜单
    {
        if(  (LastMenuType == ENGINEER_MENU) 
          || (LastMenuType == ENGINEER_CALIBRATION_MENU)
          || (LastMenuType == UPDATE_PROGRAM_MODE) //sun add 20140514
		  || (LastMenuType == OXYGEN_SENSOR_CAL_MENU) //sun add 20140625
		  || (LastMenuType == OXY_CHOOSE_MENU)
          ||(LastMenuType == PHASEIN_ZERO_CAL_MENU)  //sun add 20151103
          || (LastMenuType == BATTERY_CALIBRATION_MENU)
		  || (LastMenuType == MEDIBUS_COMM_MENU)
		  )  //sun add 20140625
        {
            WM_DeleteWindow(Menu_Obj->RecordWin);
			Menu_Obj->RecordWin = 0;
            MenuOperateStatus = 0;
        }
        if(LastMenuType != ALARM_MENU)
        {
			if (LastMenuType == TOOLS_MENU)
			{
				WM_DeleteWindow(Menu_Obj->ToolsWin.LungWin);
				Menu_Obj->ToolsWin.LungWin = 0;
			}

            if(LastMenuType == WAVEFORM_SETTING_MENU)
            {
                Menu_Obj->cWin.FocusCurve = -1;
                WM_DeleteWindow(Menu_Obj->ActivedWin);
                Menu_Obj->ActivedWin = 0;
            }
            if((LastMenuType!=MAIN_MENU) && (LastMenuType!=STANDBY_MENU))
            {
                WM_DeleteWindow(Menu_Obj->ActivedWin);
                Menu_Obj->ActivedWin = 0;
            }
            Menu_Obj->ActivedWin = _AlarmsHandleWin();
            WM_BringToBottom(Menu_Obj->HomeWin.ParentWin);
        }
        MenuType = ALARM_MENU;
        LastMenuType = ALARM_MENU;
    }
	
    else if(Menu==MONITOR_MENU)//进入 监测菜单
    {
        if(LastMenuType != MONITOR_MENU)
        {
			if (LastMenuType == TOOLS_MENU)
			{
				WM_DeleteWindow(Menu_Obj->ToolsWin.LungWin);
				Menu_Obj->ToolsWin.LungWin = 0;
			}

            if(LastMenuType == WAVEFORM_SETTING_MENU)
            {
                Menu_Obj->cWin.FocusCurve = -1;
                WM_DeleteWindow(Menu_Obj->ActivedWin);
                Menu_Obj->ActivedWin = 0;
            }
            if(LastMenuType!=MAIN_MENU)
            {
                WM_DeleteWindow(Menu_Obj->ActivedWin);
                Menu_Obj->ActivedWin = 0;
            }
            Menu_Obj->ActivedWin = _MonitorParameterWin();
            WM_BringToBottom(Menu_Obj->HomeWin.ParentWin);
        }
        MenuType = MONITOR_MENU;
        LastMenuType = MONITOR_MENU;
    }
	
    else if(Menu == ACTIVATE_STANDBY_MENU)//进入 激活待机菜单
    {
        if(LastMenuType == WAVEFORM_SETTING_MENU)
        {
            Menu_Obj->cWin.FocusCurve = -1;
            WM_DeleteWindow(Menu_Obj->ActivedWin);
            Menu_Obj->ActivedWin = 0;
        }
		if (LastMenuType == TOOLS_MENU)
		{
			WM_DeleteWindow(Menu_Obj->ToolsWin.LungWin);
			Menu_Obj->ToolsWin.LungWin = 0;
		}

        if(LastMenuType != MAIN_MENU)
        {
            WM_DeleteWindow(Menu_Obj->ActivedWin);
            Menu_Obj->ActivedWin = 0;
        }
      
        Menu_Obj->ActivedWin = ActivateStandbyWinCreat();
        WM_BringToBottom(Menu_Obj->HomeWin.ParentWin);
        MenuType = ACTIVATE_STANDBY_MENU;
        LastMenuType = ACTIVATE_STANDBY_MENU;
    }
	
    else if(Menu==STANDBY_MENU)//进入 待机菜单
    {
        if(LastMenuType==0)
        {
            Menu_Obj->InfoWin = StateInfoWinCreat();
            Menu_Obj->hWin = FunctionMenuButtonCreat();
			Menu_Obj->bWin = _VentParaWinCreat();

            Menu_Obj->Swin =_StandbyWinCreat();
        }
        else if(LastMenuType==ACTIVATE_STANDBY_MENU)
        {
            WM_HideWindow(WM_GetDialogItem(Menu_Obj->hWin,GUI_ID_BUTTON3));
            WM_DeleteWindow(Menu_Obj->ActivedWin);
            Menu_Obj->ActivedWin = 0;
            WM_DeleteWindow(Menu_Obj->HomeWin.ParentWin);
			Menu_Obj->HomeWin.ParentWin = 0;
             BUTTON_SetPressed( WM_GetDialogItem(Menu_hWin.hWin, GUI_ID_BUTTON7), 0);//#1224
            Menu_Obj->Swin =_StandbyWinCreat();
			SetTimerHandle(0);

			Menu_hWin.InfoWin.InfoChangeFlag |= CPB_CHANGE_FLAG;
        }
#ifdef SELFTEST
        else if(LastMenuType==SELFCHECK_MENU) 
        {
            Check_Release();//检测完成
            GUI_SetBkColor(WINDOW_BK_COLOR/*0x420606*/);//zkq modify 恢复原色
            GUI_Clear();
            LCD_SetColor(0xFFFFFF);
//             Bevel(0, 50+20, 637, 390,8);
			WM_ShowWindow(Menu_Obj->Swin);
            WM_ShowWindow(Menu_Obj->bWin);
            WM_ShowWindow(Menu_Obj->hWin);
            WM_ShowWindow(Menu_Obj->InfoWin.bObj[0]);
            WM_ShowWindow(Menu_Obj->InfoWin.bObj[1]);

			WM_ShowWindow(AlarmInfoWin);
// 			WM_ShowWindow(Menu_Obj->InfoWin.bObj[2]);

			WM_ShowWindow(Menu_Obj->InfoWin.bObj[3]);
			WM_ShowWindow(Menu_Obj->InfoWin.bObj[12]);
            WM_ShowWindow(Menu_Obj->InfoWin.bObj[4]);
            WM_ShowWindow(Menu_Obj->InfoWin.bObj[5]);//djq modify 20170408
			WM_ShowWindow(Menu_Obj->InfoWin.bObj[10]);
			WM_ShowWindow(Menu_Obj->InfoWin.bObj[11]);
            WM_ShowWindow(Menu_Obj->InfoWin.pObj);
            if(WM_IsWindow(Menu_Obj->ActivedWin))
            {
                WM_ShowWindow(Menu_Obj->ActivedWin);
            }
            GUI_Delay(1);    
        }
		else if((LastMenuType==SELFCHECK_ASK_MENU) || (LastMenuType==PATIENT_DATA_ASK_MENU))
		{
		    WM_DeleteWindow(Menu_Obj->ActivedWin);
            Menu_Obj->ActivedWin = 0;
			GUI_SetBkColor(WINDOW_BK_COLOR/*0x420606*/);
            GUI_Clear();
            LCD_SetColor(0xFFFFFF);
            //Bevel(0, 50+20, 637, 390,8);
		}
#endif
        else
        {
            if(  (LastMenuType==ENGINEER_MENU)
              || (LastMenuType==ENGINEER_CALIBRATION_MENU))
            {//TODO 测试 以下代码是否能执行到
                WM_DeleteWindow(Menu_Obj->RecordWin);
				Menu_Obj->RecordWin = 0;
                MenuOperateStatus &= ~FRAMEWIN_POP;
            }
            WM_DeleteWindow(Menu_Obj->ActivedWin);
            Menu_Obj->ActivedWin = 0;
        }
        MenuType = STANDBY_MENU;
        LastMenuType = STANDBY_MENU;
        StaticStandbyState = StandbyState;
    }
    else if(Menu == SYSTEM_SETTING_MENU)//进入 系统设置菜单
    {
        WM_HWIN hItem;
        if(  (LastMenuType == ENGINEER_MENU) 
          || (LastMenuType == ENGINEER_CALIBRATION_MENU)//从 工程师（校验）菜单 返回
          || (LastMenuType == UPDATE_PROGRAM_MODE)
          || (LastMenuType == BATTERY_CALIBRATION_MENU)
          ||(LastMenuType == PHASEIN_ZERO_CAL_MENU)  //sun add 20151103
          || (LastMenuType == SOFTWART_OPTIONAL_MENU)
		  || (LastMenuType == MEDIBUS_COMM_MENU)
            || (LastMenuType == SOFTWARE_VERSION_MENU) )
        {
            WM_DeleteWindow(Menu_Obj->RecordWin);
			Menu_Obj->RecordWin = 0;
            hItem = WM_GetDialogItem(Menu_Obj->ActivedWin,GUI_ID_MULTIPAGE0);
            BUTTON_SetPressed(WM_GetDialogItem(MULTIPAGE_GetWindow(hItem,3),GUI_ID_BUTTON3),0);
        }
        //*//退出 氧浓度传感器校验对话框模式 
		else if((LastMenuType == OXYGEN_SENSOR_CAL_MENU)
		  || (LastMenuType == OXY_CHOOSE_MENU))  //sun add 20140514
        {
            WM_DeleteWindow(Menu_Obj->RecordWin);
            hItem = WM_GetDialogItem(Menu_Obj->ActivedWin,GUI_ID_MULTIPAGE0);
            if(!(MenuOperateStatus&CALIBRATION_PRECEED))//  20130417
            {
                BUTTON_SetPressed(WM_GetDialogItem(MULTIPAGE_GetWindow(hItem,3),GUI_ID_BUTTON2),0);//cpp del 20130417
            }
            WM_SetFocus(WM_GetDialogItem(MULTIPAGE_GetWindow(hItem,3),GUI_ID_BUTTON2));
        }//*/
        else if(LastMenuType != SYSTEM_SETTING_MENU)
        {
			if (LastMenuType == TOOLS_MENU)
			{
				WM_DeleteWindow(Menu_Obj->ToolsWin.LungWin);
				Menu_Obj->ToolsWin.LungWin = 0;
			}

            if(LastMenuType == WAVEFORM_SETTING_MENU)
            {
                Menu_Obj->cWin.FocusCurve = -1;
                WM_DeleteWindow(Menu_Obj->ActivedWin);
                Menu_Obj->ActivedWin = 0;
            }
            if((LastMenuType!=MAIN_MENU) && (LastMenuType!=STANDBY_MENU))
            {
                WM_DeleteWindow(Menu_Obj->ActivedWin);
                Menu_Obj->ActivedWin = 0;
            }
            Menu_Obj->ActivedWin = _SystemHandleWin();
			//WM_BringToBottom(Menu_Obj->HomeWin.ParentWin);
			if(StandbyState)
			{
				WM_BringToBottom(Menu_Obj->Swin);
			}
			else
			{
				WM_BringToBottom(Menu_Obj->HomeWin.ParentWin);
			}
		}
        MenuType = SYSTEM_SETTING_MENU;
        LastMenuType = SYSTEM_SETTING_MENU;
    }
	
    else if(Menu == ENGINEER_MENU)//进入密码输入对话框 
    {
        Menu_Obj->RecordWin = _PasswordInputWin();
        WM_BringToTop(Menu_Obj->RecordWin);
        MenuType = ENGINEER_MENU;
        LastMenuType = ENGINEER_MENU;
    }
	
    else if(Menu == ENGINEER_CALIBRATION_MENU)//进入阀 流量传感器校验菜单
    {
        WM_DeleteWindow(Menu_Obj->RecordWin);
		Menu_Obj->RecordWin = 0;
        Menu_Obj->RecordWin = _EngineerCalibrationWin();
        WM_BringToTop(Menu_Obj->RecordWin);
        MenuType = ENGINEER_CALIBRATION_MENU;
        LastMenuType = ENGINEER_CALIBRATION_MENU;
    }
    
	//*//进入 氧浓度传感器校验对话框模式 
    else if(Menu == OXYGEN_SENSOR_CAL_MENU)
    {
		WM_DeleteWindow(Menu_Obj->RecordWin);
        Menu_Obj->RecordWin = _OxygenSensorCalWin();
        WM_BringToTop(Menu_Obj->RecordWin);
        MenuType = OXYGEN_SENSOR_CAL_MENU;
        LastMenuType = OXYGEN_SENSOR_CAL_MENU;
    }//*/
    
    else if(Menu == OXY_CHOOSE_MENU)  //sun add 20140529
    {
        //WM_DeleteWindow(Menu_Obj->RecordWin);
        Menu_hWin.RecordWin = _AffirmO2Choose();
        MenuType = OXY_CHOOSE_MENU;
        LastMenuType = OXY_CHOOSE_MENU;
    }
    
    else if(Menu == UPDATE_PROGRAM_MODE)  //sun add 20140514
    {
        WM_DeleteWindow(Menu_Obj->RecordWin);
        Menu_Obj->RecordWin = _EngineerUpdateWin();
        WM_BringToTop(Menu_Obj->RecordWin);
        MenuType = UPDATE_PROGRAM_MODE;
        LastMenuType = UPDATE_PROGRAM_MODE;
    }
    else if(Menu == BATTERY_CALIBRATION_MENU)
    {
        WM_DeleteWindow(Menu_Obj->RecordWin);
        Menu_Obj->RecordWin = _BatteryCalibrationWin();
        WM_BringToTop(Menu_Obj->RecordWin);
        MenuType = BATTERY_CALIBRATION_MENU;
        LastMenuType = BATTERY_CALIBRATION_MENU;
    }
    else if(Menu == SOFTWART_OPTIONAL_MENU)  //sun add 20150508
    {
        WM_DeleteWindow(Menu_Obj->RecordWin);
        Menu_Obj->RecordWin = _SoftwareOptionalWin();
        WM_BringToTop(Menu_Obj->RecordWin);
        MenuType = SOFTWART_OPTIONAL_MENU;
        LastMenuType = SOFTWART_OPTIONAL_MENU;
    }
	else if(Menu == MEDIBUS_COMM_MENU)
	{
		WM_DeleteWindow(Menu_Obj->RecordWin);
		Menu_Obj->RecordWin = _MedibusCommWin();
		WM_BringToTop(Menu_Obj->RecordWin);
		MenuType = MEDIBUS_COMM_MENU;
		LastMenuType = MEDIBUS_COMM_MENU;
	} 
    else if(Menu == PHASEIN_ZERO_CAL_MENU)//sun add 20151103
    {
        Menu_Obj->RecordWin = PhaseinZeroCail();
        WM_BringToTop(Menu_Obj->RecordWin);
        MenuType = PHASEIN_ZERO_CAL_MENU;
        LastMenuType = PHASEIN_ZERO_CAL_MENU;
    }
    else if(Menu == TREND_MENU)  //sun add 20140514
	{
		if(  (LastMenuType == ENGINEER_MENU) 
          || (LastMenuType == ENGINEER_CALIBRATION_MENU)
		  || (LastMenuType == UPDATE_PROGRAM_MODE)
		  || (LastMenuType == OXY_CHOOSE_MENU)
          || (LastMenuType == PHASEIN_ZERO_CAL_MENU)  //sun add 20151103
          || (LastMenuType == BATTERY_CALIBRATION_MENU)
		  )
        {
            WM_DeleteWindow(Menu_Obj->RecordWin);
            MenuOperateStatus = 0;
        }
		if(LastMenuType != TREND_MENU)
        {
			if (LastMenuType == TOOLS_MENU)
			{
				WM_DeleteWindow(Menu_Obj->ToolsWin.LungWin);
				Menu_Obj->ToolsWin.LungWin = 0;
			}
			if(LastMenuType == WAVEFORM_SETTING_MENU)
            {
                Menu_Obj->cWin.FocusCurve = -1;
                WM_DeleteWindow(Menu_Obj->ActivedWin);
                Menu_Obj->ActivedWin = 0;
            }
            if((LastMenuType!=MAIN_MENU) && (LastMenuType!=STANDBY_MENU))
            {
                WM_DeleteWindow(Menu_Obj->ActivedWin);
                Menu_Obj->ActivedWin = 0;
            }

			Menu_Obj->ActivedWin = _TrendWindowCreat();//_TrendChartWin();
			//WM_BringToBottom(Menu_Obj->HomeWin.ParentWin);
            if(StandbyState == 0xFF)
            {
                WM_BringToBottom(Menu_Obj->Swin);
            }
            else
            {
                WM_BringToBottom(Menu_Obj->HomeWin.ParentWin);
            }
		}
	    MenuType = TREND_MENU;
        LastMenuType = TREND_MENU;
	}
#ifdef SELFTEST
    else if(Menu == SELFCHECK_MENU)
    {
        WM_HideWindow(Menu_Obj->Swin);
        WM_HideWindow(Menu_Obj->bWin);
        WM_HideWindow(Menu_Obj->hWin);
        WM_HideWindow(Menu_Obj->InfoWin.bObj[0]);
        WM_HideWindow(Menu_Obj->InfoWin.bObj[1]);

//      WM_HideWindow(Menu_Obj->InfoWin.bObj[2]);//ylk modify 20181115
		WM_HideWindow(AlarmInfoWin);

		WM_HideWindow(Menu_Obj->InfoWin.bObj[3]);
		WM_HideWindow(Menu_Obj->InfoWin.bObj[12]);
		WM_HideWindow(Menu_Obj->InfoWin.bObj[4]);
		WM_HideWindow(Menu_Obj->InfoWin.bObj[10]);
		WM_HideWindow(Menu_Obj->InfoWin.bObj[11]);
        WM_HideWindow(Menu_Obj->InfoWin.bObj[5]);//djq modify 20170408
        WM_HideWindow(Menu_Obj->InfoWin.pObj);
        GUI_Delay(1);
        
        WM_DeleteWindow(Menu_Obj->ActivedWin);
		Menu_Obj->ActivedWin = 0;
        Start_SelfTest();
        MenuType = SELFCHECK_MENU;
        LastMenuType = SELFCHECK_MENU;
    }
#endif
	else if(Menu == PATIENT_MENU)//ylk add 20191125 Patient
	{
		if( (LastMenuType == ENGINEER_MENU) 
		 || (LastMenuType == ENGINEER_CALIBRATION_MENU)//从 工程师（校验）菜单 返回
		 || (LastMenuType == UPDATE_PROGRAM_MODE)
		 || (LastMenuType == BATTERY_CALIBRATION_MENU)
		 || (LastMenuType == PHASEIN_ZERO_CAL_MENU)  
		 || (LastMenuType == SOFTWART_OPTIONAL_MENU)
		 || (LastMenuType == FONT_VERSION_DISPLAY_MENU)
	 	 || (LastMenuType == MEDIBUS_COMM_MENU) )
		{
			WM_DeleteWindow(Menu_Obj->RecordWin);
			Menu_Obj->RecordWin = 0;
		}
		if(LastMenuType != PATIENT_MENU)
		{
			if((LastMenuType!=MAIN_MENU)&&(LastMenuType!=STANDBY_MENU))
			{
				WM_DeleteWindow(Menu_Obj->ActivedWin);
				Menu_Obj->ActivedWin = 0;
			}
			Menu_Obj->ActivedWin = Patient_InfoWinCreate();  

			if(StandbyState == 0xFF)
			{
				WM_BringToBottom(Menu_Obj->Swin);
			}
			else if(StandbyState == 0)
			{
				WM_BringToBottom(Menu_Obj->HomeWin.ParentWin);//正常情况不会运行到这里
			}
		}
		MenuType = PATIENT_MENU;
		LastMenuType = PATIENT_MENU;
	}
	else if(Menu == PATIENT_DATA_ASK_MENU)
	{
		if(LastMenuType != PATIENT_DATA_ASK_MENU)
		{
			WM_DeleteWindow(Menu_Obj->ActivedWin);
			Menu_Obj->ActivedWin = 0;
			Menu_Obj->ActivedWin = _TrendData_AskCreat();
			WM_BringToBottom(Menu_Obj->Swin);
		}
		MenuType = PATIENT_DATA_ASK_MENU;
		LastMenuType = PATIENT_DATA_ASK_MENU;
	}
    else if(Menu == TOUCH_CALIBRATION_MENU)
    {
        WM_HideWindow(Menu_Obj->Swin);
        WM_HideWindow(Menu_Obj->bWin);
        WM_HideWindow(Menu_Obj->hWin);
        WM_HideWindow(Menu_Obj->InfoWin.bObj[0]);
        WM_HideWindow(Menu_Obj->InfoWin.bObj[1]);

//      WM_HideWindow(Menu_Obj->InfoWin.bObj[2]);
		WM_HideWindow(AlarmInfoWin);

		WM_HideWindow(Menu_Obj->InfoWin.bObj[3]);
		WM_HideWindow(Menu_Obj->InfoWin.bObj[12]);
		WM_HideWindow(Menu_Obj->InfoWin.bObj[4]);
		WM_HideWindow(Menu_Obj->InfoWin.bObj[10]);
		WM_HideWindow(Menu_Obj->InfoWin.bObj[11]);
        WM_HideWindow(Menu_Obj->InfoWin.bObj[5]);//djq modify 20170408
        WM_HideWindow(Menu_Obj->InfoWin.pObj); 
        
        
        TouchPanel_Calibrate();
        TouchScreenCalibrate = 0;
        hItem = WM_GetDialogItem(Menu_hWin.ActivedWin,GUI_ID_MULTIPAGE0);
        BUTTON_SetPressed(WM_GetDialogItem(MULTIPAGE_GetWindow(hItem,3),GUI_ID_BUTTON4),0);
        
        WM_ShowWindow(Menu_Obj->Swin);
        WM_ShowWindow(Menu_Obj->bWin);
        WM_ShowWindow(Menu_Obj->hWin);
        WM_ShowWindow(Menu_Obj->InfoWin.bObj[0]);
        WM_ShowWindow(Menu_Obj->InfoWin.bObj[1]);

//      WM_ShowWindow(Menu_Obj->InfoWin.bObj[2]);
		WM_ShowWindow(AlarmInfoWin);

		WM_ShowWindow(Menu_Obj->InfoWin.bObj[3]);
		WM_ShowWindow(Menu_Obj->InfoWin.bObj[12]);
		WM_ShowWindow(Menu_Obj->InfoWin.bObj[4]);
		WM_ShowWindow(Menu_Obj->InfoWin.bObj[10]);
		WM_ShowWindow(Menu_Obj->InfoWin.bObj[11]);
        WM_HideWindow(Menu_Obj->InfoWin.bObj[5]);//djq modify 20170408
        WM_ShowWindow(Menu_Obj->InfoWin.pObj);
        
        MenuType = TOUCH_CALIBRATION_MENU;//zkq modify 20170224 
        LastMenuType = TOUCH_CALIBRATION_MENU;
    }
	else if (Menu == SOFTWARE_VERSION_MENU)
	{
		WM_DeleteWindow(Menu_Obj->RecordWin);
		Menu_Obj->RecordWin = _SoftwareVersionWin();
		WM_BringToTop(Menu_Obj->RecordWin);
		MenuType = SOFTWARE_VERSION_MENU;
		LastMenuType = SOFTWARE_VERSION_MENU;
	}
	//关机倒计时
	if ((ShutDownAskMenu_Flag == 1))
	{
		if (!WM_IsWindow(Menu_Obj->ShutDowAskWin))
		{
			Menu_Obj->ShutDowAskWin = _ShutDown_AskWin();
		}
		WM_BringToTop(Menu_Obj->ShutDowAskWin);
	}
	else
	{
		if (WM_IsWindow(Menu_Obj->ShutDowAskWin))
		{
			WM_DeleteWindow(Menu_Obj->ShutDowAskWin);
			Menu_Obj->ShutDowAskWin = 0;
			//发送 关机 取消关机
			Power_SendOnOffState(SendShutDownState);
			SendShutDownState = START_UP;
            //Task |= MEAU_UPDATE;
		}
	}

#ifdef SELFTEST
    if( (Menu == SELFCHECK_ASK_MENU) || (AskMenu_Flag == 1))
    {
        AskMenu_Flag = 0;
        if(LastMenuType != SELFCHECK_ASK_MENU)
        {
            Menu_Obj->ActivedWin = _SelfCheck_AskCreat();
        }
        MenuType = SELFCHECK_ASK_MENU;
        LastMenuType = SELFCHECK_ASK_MENU;
    }
#endif
	if(WM_IsVisible(Menu_hWin.TextWin))  //sun add 20140707
    {
        WM_HideWindow(Menu_hWin.TextWin);
    }
#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C)
    if(Menu == SYSTEM_SETTING_MENU)
	{
	    hItem = WM_GetDialogItem(Menu_Obj->ActivedWin, GUI_ID_MULTIPAGE0);
		hWin = MULTIPAGE_GetWindow(hItem,3);
		hItem=WM_GetDialogItem(hWin, GUI_ID_BUTTON4);
		WM_ShowWindow(hItem);
		hItem=WM_GetDialogItem(hWin, GUI_ID_BUTTON5);
		if (Masimo_EtCO2_Config_On && (AA_Module_Flag.PhaseInTypeModuleExist == Masimo_EtCO2_zhuliu || AA_Module_Flag.PhaseInTypeModuleExist == Masimo_EtCO2_zhuliuco2))
		{
			WM_ShowWindow(hItem);
			if((AA_all_data.AA_state.AA_ModuleState.zero_state == disable) ||(AA_all_data.AA_state.AA_ModuleState.zero_state == zero_zeroing))
			{
				WM_DisableWindow(hItem);
			}
			else
			{
				WM_EnableWindow(hItem);
			}
		}
	}
#endif
    return(*Menu_Obj);
}
/***************************************************************************************************
* @brief 状态信息窗
* @para   
* @retval 
*/
INFO_HWIN StateInfoWinCreat(void)
{
    INFO_HWIN iWin;
	WM_HWIN hItem = IMAGETEXT_Create(0, 0, 1024, 66, 0, WM_CF_SHOW);
	IMAGETEXT_SetBkColor(hItem, GUI_WHITE);
    //
    iWin.bObj[0]=IMAGETEXT_Create(104,0,109,66,0,WM_CF_SHOW);//模式信息
	IMAGETEXT_SetBkColor(iWin.bObj[0],WINDOW_BK_COLOR/*0x420606*/);
// 	IMAGETEXT_SetBkColor(iWin.bObj[0],GUI_MAGENTA);
    IMAGETEXT_SetFont(iWin.bObj[0],&GUI_Font_32);
	IMAGETEXT_SetTextAlign(iWin.bObj[0],GUI_TA_HCENTER | GUI_TA_VCENTER);
    //zkq modify 20170116 防止开机时打开ACGO/MANUAL模式不更新
    switch(BeathSettingPara.BreathMode)
    {
    case MANUAL_MODE:
        IMAGETEXT_SetText(iWin.bObj[0],Sys_Lib_GetDispStr(DP_MANUAL));
        break;
    case ACGO_MODE:
        IMAGETEXT_SetText(iWin.bObj[0],"ACGO");
        break;
    default:
        IMAGETEXT_SetText(iWin.bObj[0],"VCV");
        break;
    }
  
	iWin.bObj[1]=IMAGETEXT_Create(137+85+230,0,59,40,0,WM_CF_SHOW);    //触发肺
// 	iWin.bObj[1]=IMAGETEXT_Create(200,40,59,40,0,WM_CF_SHOW);    //触发肺
	IMAGETEXT_SetBkColor(iWin.bObj[1],WINDOW_BK_COLOR/*0x420606*/);
// 	IMAGETEXT_SetBkColor(iWin.bObj[1],GUI_LIGHTCYAN);
    IMAGETEXT_SetFont(iWin.bObj[1],&GUI_Font_16);
    IMAGETEXT_SetTextAlign(iWin.bObj[1],GUI_TA_HCENTER | GUI_TA_BOTTOM);
      
// 	iWin.bObj[2]=IMAGETEXT_Create(181,0,365,40,0,WM_CF_SHOW);//报警提示信息
//  IMAGETEXT_SetBkColor(iWin.bObj[2],0x420606);
//  IMAGETEXT_SetFont(iWin.bObj[2],&GUI_Font_24);
//  IMAGETEXT_SetTextAlign(iWin.bObj[2],GUI_TA_HCENTER | GUI_TA_VCENTER);

    AlarmInfoWin = _AlarmInfoCreat();

	iWin.bObj[2]=WM_GetDialogItem(AlarmInfoWin,GUI_ID_IMAGETEXT1);//Alarm
	IMAGETEXT_SetBkColor(iWin.bObj[2],WINDOW_BK_COLOR);
// 	IMAGETEXT_SetBkColor(iWin.bObj[2],GUI_DARKRED);
	IMAGETEXT_SetFont(iWin.bObj[2],&GUI_Font_24);
	IMAGETEXT_SetTextAlign(iWin.bObj[2],GUI_TA_HCENTER | GUI_TA_VCENTER);

	iWin.bObj[8]=WM_GetDialogItem(AlarmInfoWin,GUI_ID_IMAGETEXT4);//Alarm
	IMAGETEXT_SetBkColor(iWin.bObj[1],WINDOW_BK_COLOR);
	IMAGETEXT_SetFont(iWin.bObj[1],&GUI_Font_24);
	IMAGETEXT_SetTextAlign(iWin.bObj[1],GUI_TA_HCENTER | GUI_TA_VCENTER);

	iWin.bObj[6]=WM_GetDialogItem(AlarmInfoWin,GUI_ID_IMAGETEXT0);//bell
	IMAGETEXT_SetFont(iWin.bObj[6],&GUI_FontArialUnicodeMS32);
	IMAGETEXT_SetBkColor(iWin.bObj[6],WINDOW_BK_COLOR);

	iWin.bObj[7]=WM_GetDialogItem(AlarmInfoWin,GUI_ID_TEXT0);//多重报警数值
// 	TEXT_SetBkColor(iWin.bObj[7],GUI_BROWN);
// 	iWin.bObj[8]=WM_GetDialogItem(AlarmInfoWin,GUI_ID_TEXT1);//未达到高精度提示信息

	iWin.bObj[3]=WM_GetDialogItem(AlarmInfoWin,GUI_ID_IMAGETEXT2);//djq modify 55->40 20170408  报警静音
	IMAGETEXT_SetBkColor(iWin.bObj[3],WINDOW_BK_COLOR);
// 	IMAGETEXT_SetBkColor(iWin.bObj[3],GUI_YAN_BLUE);
    IMAGETEXT_SetFont(iWin.bObj[3],&GUI_FontPatientIcon32X32);
    IMAGETEXT_SetTextAlign(iWin.bObj[3],GUI_TA_HCENTER | GUI_TA_BOTTOM);

	iWin.bObj[12]=WM_GetDialogItem(AlarmInfoWin,GUI_ID_IMAGETEXT3);//Silence text
	IMAGETEXT_SetBkColor(iWin.bObj[12],WINDOW_BK_COLOR);
	IMAGETEXT_SetFont(iWin.bObj[12],&GUI_Font_15);
	IMAGETEXT_SetTextAlign(iWin.bObj[12],GUI_TA_HCENTER | GUI_TA_VCENTER);
	IMAGETEXT_SetText(iWin.bObj[12],Sys_Lib_GetDispStr(DP_MODULEAACO2));

	iWin.pObj = POWER_INFO_Create(801,0,107,66,0,WM_CF_SHOW); // Power
	POWER_INFO_SetBkColor(iWin.pObj, WINDOW_BK_COLOR);
	POWER_INFO_SetStateColor(iWin.pObj,POWER_SELECT_COLOR);
	POWER_INFO_SetBatteryColor(iWin.pObj,POWER_BATTERY_COLOR);
    //
	iWin.bObj[4]=IMAGETEXT_Create(909,0,115,66,0,WM_CF_SHOW);//时间
	IMAGETEXT_SetBkColor(iWin.bObj[4],WINDOW_BK_COLOR);//0x420606
// 	IMAGETEXT_SetBkColor(iWin.bObj[4],GUI_DARKGRAY);//0x420606
    IMAGETEXT_SetTextAlign(iWin.bObj[4],GUI_TA_HCENTER | GUI_TA_VCENTER);
	IMAGETEXT_SetFont(iWin.bObj[4], &GUI_Font_24);

    //djq add 20170408
    iWin.bObj[5]=IMAGETEXT_Create(749,0,51,66,0,WM_CF_SHOW);//驱动气标志
	IMAGETEXT_SetBkColor(iWin.bObj[5],WINDOW_BK_COLOR);
// 	IMAGETEXT_SetBkColor(iWin.bObj[5],GUI_DARKYELLOW);
    IMAGETEXT_SetTextAlign(iWin.bObj[5],GUI_TA_HCENTER | GUI_TA_VCENTER);

    if(DriverGas_Chose_Flag)//djq add 20170609驱动气体选配
    {
        iWin.InfoChangeFlag |= DRIVER_GAS_TYPE_FLAG;
    }
    else
    {
        WM_HideWindow(iWin.bObj[5]);
    }
    //
// 	iWin.pObj = POWER_INFO_Create(670,0,60,40,0,WM_CF_SHOW);
	//iWin.pObj = POWER_INFO_Create(710+15+200,40,60,40+5,0,WM_CF_SHOW);

// 	POWER_INFO_SetBkColor(iWin.pObj, GUI_GREEN);
    //
	iWin.bObj[9] = IMAGETEXT_Create(250,42,176,32,0,WM_CF_SHOW);
	if (LanguageType == Disp_Lg_Chinese)
	{
		IMAGETEXT_SetBitmap(iWin.bObj[9],&bmCPB_ImageInfo_CN);
	}
	else
	{
		IMAGETEXT_SetBitmap(iWin.bObj[9],&bmCPB_ImageInfo_EN);
	}
	WM_HideWindow(iWin.bObj[9]);
	//
	iWin.bObj[10] = IMAGETEXT_Create(0,0,30,66,0,WM_CF_SHOW); // 病人图标
	IMAGETEXT_SetBkColor(iWin.bObj[10],WINDOW_BK_COLOR);
// 	IMAGETEXT_SetBkColor(iWin.bObj[10],GUI_LIGHTCYAN);
	IMAGETEXT_SetFont(iWin.bObj[10],&GUI_FontPatientIcon32X32);
	IMAGETEXT_SetTextAlign(iWin.bObj[10],GUI_TA_HCENTER | GUI_TA_VCENTER);

	//iWin.bObj[11] = IMAGETEXT_Create(32,0,70,66,0,WM_CF_SHOW);
	iWin.bObj[11] = TEXT_Create(30,0,73,66,0,WM_CF_SHOW, " ", GUI_TA_HCENTER | GUI_TA_VCENTER);
	TEXT_SetBkColor(iWin.bObj[11],WINDOW_BK_COLOR);
	TEXT_SetTextColor(iWin.bObj[11],GUI_WHITE);
	TEXT_SetFont(iWin.bObj[11],&GUI_Font_20);

	iWin.InfoChangeFlag |= PATIENT_TYPE_INFO_CHANGE_FLAG;

    return(iWin);
}
/***************************************************************************************************
* @brief 实时参数更新  
* @para   
* @retval 
*/
void MultiText_ParaUpdate(WM_HWIN hWin, char *str)
{
	WM_MESSAGE Msg;
	MultiText_SetParaData(hWin, str);
	Msg.hWin = hWin;
	Msg.MsgId = WM_NOTIFICATION_VALUE_CHANGED;
	Msg.Data.v = WM_USER_VALUE_CHANGED1;
	//WM_SelectWindow(Msg.hWin);
	WM_SendMessage(Msg.hWin, &Msg);
}
void MultiText_OtherParaUpdate(WM_HWIN hWin, char *str)
{
	WM_MESSAGE Msg;
	MultiText_SetOtherParaData(hWin, str);
	Msg.hWin = hWin;
	Msg.MsgId = WM_NOTIFICATION_VALUE_CHANGED;
	Msg.Data.v = WM_USER_VALUE_CHANGED2;
	//WM_SelectWindow(Msg.hWin);
	WM_SendMessage(Msg.hWin, &Msg);
}
void MultiText_NameUpdate(WM_HWIN hWin, char *str)
{
	WM_MESSAGE Msg;
	MultiText_SetParaName(hWin, str);
	Msg.hWin = hWin;
	Msg.MsgId = WM_NOTIFICATION_VALUE_CHANGED;
	Msg.Data.v = WM_USER_VALUE_CHANGED6;
	//WM_SelectWindow(Msg.hWin);
	WM_SendMessage(Msg.hWin, &Msg);
}
void MultiText_UnitUpdate(WM_HWIN hWin, char *str)
{
	WM_MESSAGE Msg;
	MultiText_SetParaUnit(hWin, str);
	Msg.hWin = hWin;
	Msg.MsgId = WM_NOTIFICATION_VALUE_CHANGED;
	Msg.Data.v = WM_USER_VALUE_CHANGED5;
	//WM_SelectWindow(Msg.hWin);
	WM_SendMessage(Msg.hWin, &Msg);
}
void MultiText_OtherUnitUpdate(WM_HWIN hWin, char *str)
{
	WM_MESSAGE Msg;
	MultiText_SetOtherParaUnit(hWin, str);
	Msg.hWin = hWin;
	Msg.MsgId = WM_NOTIFICATION_VALUE_CHANGED;
	Msg.Data.v = WM_USER_VALUE_CHANGED14;
	//WM_SelectWindow(Msg.hWin);
	WM_SendMessage(Msg.hWin, &Msg);
}
void MultiText_MaxValueUpdate(WM_HWIN hWin, char *str)
{
	WM_MESSAGE Msg;
	MultiText_SetParaMaxValue(hWin, str);
	Msg.hWin=hWin;
	Msg.MsgId=WM_NOTIFICATION_VALUE_CHANGED;
	Msg.Data.v = WM_USER_VALUE_CHANGED3;
	WM_SendMessage(Msg.hWin,&Msg);
}
void MultiText_MinValueUpdate(WM_HWIN hWin, char *str)
{
	WM_MESSAGE Msg;
	MultiText_SetParaMinValue(hWin, str);
	Msg.hWin=hWin;
	Msg.MsgId=WM_NOTIFICATION_VALUE_CHANGED;
	Msg.Data.v = WM_USER_VALUE_CHANGED4;
	WM_SendMessage(Msg.hWin,&Msg);
}
void MultiText_MaxValueUpdate1(WM_HWIN hWin, char *str)
{
	WM_MESSAGE Msg;
	MultiText_SetOtherParaMaxValue(hWin, str);
	Msg.hWin=hWin;
	Msg.MsgId=WM_NOTIFICATION_VALUE_CHANGED;
	Msg.Data.v = WM_USER_VALUE_CHANGED12;
	WM_SendMessage(Msg.hWin,&Msg);
}
void MultiText_MinValueUpdate1(WM_HWIN hWin, char *str)
{
	WM_MESSAGE Msg;
	MultiText_SetOtherParaMinValue(hWin, str);
	Msg.hWin=hWin;
	Msg.MsgId=WM_NOTIFICATION_VALUE_CHANGED;
	Msg.Data.v = WM_USER_VALUE_CHANGED13;
	WM_SendMessage(Msg.hWin,&Msg);
}

//////
void LeftRealtimeParaUpdate(HOME_MENU_HWIN * hWin)
{    
	static char pText[20] = {0};
	char pText1[20] = {0};
	int index = 0;
	float MacValue = 0;
	static float macData=0;
	static enum AA_MODULE_STATE OldAAModuleState = No_Connect;
	static unsigned char LastAx1Id = 0, LastAx2Id = 0,fiAA1Data=0,fiAA2Data=0,etAA1Data=0;
	static unsigned char etAA2Data=0,fiN2OData=0,etN2OData=0,OldPulse=0,OldSpO2=0,OldEtCO2=0,OldInCO2=0;

	if((index = Get_MonitorRank_AA(DP_MAC)) != -1)
	{
		MacValue = AA_Para.MAC/10.0;
		if((macData != MacValue)||(AA_Module_Flag.Module_State_Flag != OldAAModuleState))
		{
			if((EtCO2ModuleOff)&&(AA_Module_Flag.EtCO2ModuleExist)&&(AA_Module_Flag.Module_State_Flag == Connect_Valid)\
				&&(AA_all_data.AA_detail.AA_AA1.Accuracy == ACCURACY_Iso))//djq modify 20161230 气道适配器异常处理//zkq add 高精度允许监测
			{
				if(MacValue == 0.0)
				{
					strcpy(pText, "0");
				}
				else
				{
					if((int)MacValue > 100)
					{
						strcpy(pText, "--");						
					}
					else
					{
						strcpy(pText, _Float2String((float)(MacValue/10+0.005),1));						
					}
				}
			}
			else
			{
				strcpy(pText, "--");
			}
			MultiText_ParaUpdate(hWin->Left_Robj[index], pText);
		}
		macData = MacValue;
	}
	if((index = Get_MonitorRank_AA(DP_AA1)) != -1)
	{
		if(LastAx1Id != AA_all_data.AA_detail.AA_AA1.ID)
		{
			LastAx1Id = AA_all_data.AA_detail.AA_AA1.ID;
			switch(LastAx1Id)
			{
			case IM_AGT_HAL:
				strcpy(pText,Sys_Lib_GetDispStr(DP_HAL));
				AA_Max = 8;
				break;
			case IM_AGT_ENF:
				strcpy(pText,Sys_Lib_GetDispStr(DP_ENF));
				AA_Max = 8;
				break;
			case IM_AGT_ISO:
				strcpy(pText,Sys_Lib_GetDispStr(DP_ISO));
				AA_Max = 8;
				break;
			case IM_AGT_SEV:
				strcpy(pText,Sys_Lib_GetDispStr(DP_SEV));
				AA_Max = 10;
				break;
			case IM_AGT_DES:
				strcpy(pText,Sys_Lib_GetDispStr(DP_DES));
				AA_Max = 22;
				break;
			case IM_NO_AGT:
			default:
				strcpy(pText,Sys_Lib_GetDispStr(DP_AA1));
				AA_Max = 10;
				break;
			}
			MultiText_NameUpdate(hWin->Left_Robj[index], pText);
		}
		//
		if((fiAA1Data != AA_all_data.AA_detail.AA_AA1.FI) 
			||(etAA1Data != AA_all_data.AA_detail.AA_AA1.ET)
			||(AA_Module_Flag.Module_State_Flag != OldAAModuleState))
		{
			if((EtCO2ModuleOff)&&(AA_Module_Flag.EtCO2ModuleExist)&&(AA_Module_Flag.Module_State_Flag == Connect_Valid)\
				&&(AA_all_data.AA_detail.AA_AA1.Accuracy == ACCURACY_Iso))//djq modify 20161230 气道适配器异常处理//zkq add 高精度允许监测
			{
				if((AA_all_data.AA_detail.AA_AA1.FI) <= AA_Max*10)
				{
					if((AA_all_data.AA_detail.AA_AA1.FI) >= 100)
					{
						strcpy(pText, _Float2String((float)(AA_all_data.AA_detail.AA_AA1.FI/10.0),0));
					}
					else
					{
						strcpy(pText, _Float2String((float)(AA_all_data.AA_detail.AA_AA1.FI/10.0),1));					
					}
				}
				else
				{
					strcpy(pText, "--");
				}
				//
				if((AA_all_data.AA_detail.AA_AA1.ET) <= AA_Max*10)
				{
					if((AA_all_data.AA_detail.AA_AA1.ET) >= 100)
					{
						strcpy(pText1, _Float2String((float)(AA_all_data.AA_detail.AA_AA1.ET/10.0),0));					
					}
					else
					{
						strcpy(pText1, _Float2String((float)(AA_all_data.AA_detail.AA_AA1.ET/10.0),1));					
					}	
				}
				else
				{
					strcpy(pText1, "--");
				}
			}
			else
			{
				strcpy(pText, "--");
				strcpy(pText1, "--");
			}
			
			MultiText_ParaUpdate(hWin->Left_Robj[index], pText);
			MultiText_OtherParaUpdate(hWin->Left_Robj[index], pText1);

			fiAA1Data = AA_all_data.AA_detail.AA_AA1.FI;
			etAA1Data = AA_all_data.AA_detail.AA_AA1.ET;
		}		
	}
	if((index = Get_MonitorRank_AA(DP_AA2)) != -1)
	{
		if(LastAx2Id != AA_all_data.AA_detail.AA_AA2.ID)
		{
			LastAx2Id = AA_all_data.AA_detail.AA_AA2.ID;
			switch(LastAx2Id)
			{
			case IM_AGT_HAL:
				strcpy(pText,Sys_Lib_GetDispStr(DP_HAL));
				AA2_Max = 8;
				break;
			case IM_AGT_ENF:
				strcpy(pText,Sys_Lib_GetDispStr(DP_ENF));
				AA2_Max = 8;
				break;
			case IM_AGT_ISO:
				strcpy(pText,Sys_Lib_GetDispStr(DP_ISO));
				AA2_Max = 8;
				break;
			case IM_AGT_SEV:
				strcpy(pText,Sys_Lib_GetDispStr(DP_SEV));
				AA2_Max = 10;
				break;
			case IM_AGT_DES:
				strcpy(pText,Sys_Lib_GetDispStr(DP_DES));
				AA2_Max = 22;
				break;
			case IM_NO_AGT:
			default:
				strcpy(pText,Sys_Lib_GetDispStr(DP_AA2));
				AA2_Max = 10;
				break;
			}
			MultiText_NameUpdate(hWin->Left_Robj[index], pText);

		}//
		if((fiAA2Data != AA_all_data.AA_detail.AA_AA2.FI) 
			||(etAA2Data != AA_all_data.AA_detail.AA_AA2.ET)
			||(AA_Module_Flag.Module_State_Flag != OldAAModuleState))
		{
			if((EtCO2ModuleOff)&&(AA_Module_Flag.EtCO2ModuleExist)&&(AA_Module_Flag.Module_State_Flag == Connect_Valid)\
				&&(AA_all_data.AA_detail.AA_AA2.Accuracy == ACCURACY_Iso))//djq modify 20161230 气道适配器异常处理//zkq add 高精度允许监测
			{
				if((AA_all_data.AA_detail.AA_AA2.FI) <= AA2_Max*10)
				{
					if((AA_all_data.AA_detail.AA_AA2.FI) >= 100)
					{
						strcpy(pText, _Float2String((float)(AA_all_data.AA_detail.AA_AA2.FI/10.0),0));
					}
					else
					{
						strcpy(pText, _Float2String((float)(AA_all_data.AA_detail.AA_AA2.FI/10.0),1));					
					}
				}
				else
				{
					strcpy(pText, "--");
				}
				//
				if((AA_all_data.AA_detail.AA_AA2.ET) <= AA2_Max*10)
				{
					if((AA_all_data.AA_detail.AA_AA2.ET) >= 100)
					{
						strcpy(pText1, _Float2String((float)(AA_all_data.AA_detail.AA_AA2.ET/10.0),0));					
					}
					else
					{
						strcpy(pText1, _Float2String((float)(AA_all_data.AA_detail.AA_AA2.ET/10.0),1));					
					}	
				}
				else
				{
					strcpy(pText1, "--");
				}
			}
			else
			{
				strcpy(pText, "--");
				strcpy(pText1, "--");
			}
			MultiText_ParaUpdate(hWin->Left_Robj[index], pText);
			MultiText_OtherParaUpdate(hWin->Left_Robj[index], pText1);

			fiAA2Data = AA_all_data.AA_detail.AA_AA2.FI;
			etAA2Data = AA_all_data.AA_detail.AA_AA2.ET;
		}		
	}
	if((index = Get_MonitorRank_AA(DP_N2O)) != -1)
	{
		//FiN2O
		if((fiN2OData != AA_all_data.AA_detail.AA_N2O.FI)||(AA_Module_Flag.Module_State_Flag != OldAAModuleState))
		{
			if((EtCO2ModuleOff)&&(AA_Module_Flag.EtCO2ModuleExist)&&(AA_Module_Flag.Module_State_Flag == Connect_Valid)\
				&&(AA_all_data.AA_detail.AA_N2O.Accuracy == ACCURACY_Iso))//djq modify 20161230 气道适配器异常处理//zkq add 高精度允许监测
			{
				if(AA_all_data.AA_detail.AA_N2O.FI > 82)
				{
					strcpy(pText, "--");					
				}
				else
				{
					strcpy(pText, _Float2String((float)(AA_all_data.AA_detail.AA_N2O.FI),0));
				}
			}
			else
			{
				strcpy(pText, "--");					
			}
			MultiText_ParaUpdate(hWin->Left_Robj[index], pText);
		}
		fiN2OData = AA_all_data.AA_detail.AA_N2O.FI;
		//EtN2O
		if((etN2OData != AA_all_data.AA_detail.AA_N2O.ET)||(AA_Module_Flag.Module_State_Flag != OldAAModuleState))
		{
			if((EtCO2ModuleOff)&&(AA_Module_Flag.EtCO2ModuleExist)&&(AA_Module_Flag.Module_State_Flag == Connect_Valid)\
				&&(AA_all_data.AA_detail.AA_N2O.Accuracy == ACCURACY_Iso))//djq modify 20161230 气道适配器异常处理//zkq add 高精度允许监测
			{
				if(AA_all_data.AA_detail.AA_N2O.ET > 82)
				{
					strcpy(pText, "--");					
				}
				else
				{
					strcpy(pText, _Float2String((float)(AA_all_data.AA_detail.AA_N2O.ET),0));
				}
			}
			else
			{
				strcpy(pText, "--");					
			}
			MultiText_OtherParaUpdate(hWin->Left_Robj[index], pText);
		}
		etN2OData = AA_all_data.AA_detail.AA_N2O.ET;
	}
	//  [2/16/2022 yaoyao 注释]SPO2的参数在右侧监测区
// 	if((index = Get_MonitorRank_AA(DP_SPO2)) != -1)
// 	{
// 		if(OldPulse != SpO2_Parameters.PulseRate)
// 		{
// 			if((SpO2ModuleExist) && (!SpO2ModuleOff))
// 			{
// 				if((SpO2_Parameters.PulseRate > 0) && (SpO2_Parameters.PulseRate < 250))
// 				{
// 					strcpy(pText, _Float2String((float)SpO2_Parameters.PulseRate,0));					
// 				}
// 				else
// 				{
// 					strcpy(pText, "--");					
// 				}
// 			}
// 			else
// 			{
// 				strcpy(pText, "--");
// 			}
// 			MultiText_ParaUpdate(hWin->Left_Robj[index], pText);
// 		}
// 		//
// 		if(OldSpO2 != SpO2_Parameters.SpO2)
// 		{
// 			if((SpO2ModuleExist) && (!SpO2ModuleOff))
// 			{
// 				if((SpO2_Parameters.SpO2 > 0 ) && (SpO2_Parameters.SpO2 <= 100))
// 				{
// 					strcpy(pText, _Float2String((float)SpO2_Parameters.SpO2,0));					
// 				}
// 				else
// 				{
// 					strcpy(pText, "--");					
// 				}
// 			}
// 			else
// 			{
// 				strcpy(pText, "--");
// 			}	
// 			MultiText_OtherParaUpdate(hWin->Left_Robj[index], pText);
// 		}
// 		OldSpO2 = SpO2_Parameters.SpO2;
// 		OldPulse = SpO2_Parameters.PulseRate;
// 	}
	if((index = Get_MonitorRank_AA(DP_CO2)) != -1)
	{  		
		if((!AA_Module_Flag.EtCO2ModuleExist)||(!EtCO2ModuleOff))
		{
			strcpy(pText, "--");//inCO2
			strcpy(pText1, "--");//etCO2
                        OldInCO2 = 200;
                        OldEtCO2 = 200;
		}
		else
		{   
                        OldInCO2 = 200;
                        OldEtCO2 = 200;
                        
			if((ETCO2Unit == 0)||(ETCO2Unit == 2))
			{
				if(CO2_Parameters.insCO2 > 150/*133*/)
				{
					strcpy(pText, "--");
				}
				else if (CO2_Parameters.insCO2<100)//  [2/14/2022 yaoyao]大于等于10的数据，没有小数，小于10 的数据保存1位小数
				{
					strcpy(pText, _Float2String((float)(CO2_Parameters.insCO2/10),1));
				}
				else
				{
					sprintf(pText, "%d",(int)Rounding(CO2_Parameters.insCO2,10));
				}
				//
				if(CO2_Parameters.etCO2 > 150/*133*/)
				{
					strcpy(pText1, "--");
				}
				else if (CO2_Parameters.etCO2<100)//  [2/14/2022 yaoyao]大于等于10的数据，没有小数，小于10 的数据保存1位小数
				{
					strcpy(pText1, _Float2String((float)(CO2_Parameters.etCO2/10),1));
				}
				else
				{
					sprintf(pText1, "%d",(int)Rounding(CO2_Parameters.etCO2,10));
				}
			}
			else
			{
				if(CO2_Parameters.insCO2 > 113/*100*/)
				{
					strcpy(pText, "--");
				}
				else if (CO2_Parameters.insCO2 <10)//  [2/14/2022 yaoyao]大于等于10的数据，没有小数，小于10 的数据保存1位小数
				{
					strcpy(pText, _Float2String((float)(CO2_Parameters.insCO2),1));
				}
				else
				{
					sprintf(pText, "%d",(int)Rounding(CO2_Parameters.insCO2,1));
				}
				//
				if(CO2_Parameters.etCO2 > 113/*100*/)
				{
					strcpy(pText1, "--");
				}
				else if (CO2_Parameters.etCO2 < 10)//  [2/14/2022 yaoyao]大于等于10的数据，没有小数，小于10 的数据保存1位小数
				{
					strcpy(pText1, _Float2String((float)(CO2_Parameters.etCO2),1));
				}
				else
				{
					sprintf(pText1, "%d",(int)Rounding(CO2_Parameters.etCO2,1));
				}
			}
		}
		if(OldInCO2 != CO2_Parameters.insCO2)
		{
			OldInCO2 = CO2_Parameters.insCO2;
			MultiText_ParaUpdate(hWin->Left_Robj[index], pText);
		}
		
		if(OldEtCO2 != CO2_Parameters.etCO2)
		{
			OldEtCO2 = CO2_Parameters.etCO2;
			MultiText_OtherParaUpdate(hWin->Left_Robj[index], pText1);
		}
	}
}
void RealtimeParaUpdate(HOME_MENU_HWIN * hWin)
{
	WM_MESSAGE Msg;
	char string[20] = {0};
	char string1[20] = {0};
	char * pText;
	static unsigned char LastAx1Id = 0;
	static unsigned short num = 0, flag = 0;
	int index = 0;
	float MacValue = 0;
	static enum AA_MODULE_STATE OldAAModuleState = No_Connect;
	static unsigned char fiN2OData=0;
	static float macData=0;
	static int OldPpeak=0, OldPmean=0, OldPeep=0, OldVTE=0;
	static int OldVTI=0, OldFiO2=0, OldEtCO2=0, OldMV=0, OldF=0;
	static int OldSpO2=0, OldPulse=0;//  [2/16/2022 yaoyao add]
	//----------------------------------------------------------------------------------------------
	if((index = Get_MonitorRank_Rapid(DP_PPEAK)) != -1)//判断当前参数是否可以显示
	{    
		if(BeathSettingPara.BreathMode == ACGO_MODE)
		{
			MultiText_ParaUpdate(hWin->Robj[index], "--");
		}
		else
		{
                  OldPpeak = 0;
			if(MonitorPara.PpeakValue > 1000)//  20130325 //1200 cpp md 20130724
			{				
				strcpy(string, "--");
			}
			else if(MonitorPara.PpeakValue < 0)
			{	
				strcpy(string, _Float2String((float)MonitorPara.PpeakValue,0));	
			}
			else
			{
				strcpy(string, _Float2String((float)MonitorPara.PpeakValue/10,1));				
			}
		}
		if(OldPpeak != MonitorPara.PpeakValue)
		{
			OldPpeak = MonitorPara.PpeakValue;
			MultiText_ParaUpdate(hWin->Robj[index], string);
		}
	}
	//----------------------------------------------------------------------------------------------
	if((index = Get_MonitorRank_Rapid(DP_PMEAN)) != -1)
	{
		if(BeathSettingPara.BreathMode == ACGO_MODE)
		{
			MultiText_ParaUpdate(hWin->Robj[index], "--");
		}
		else
		{
                  OldPmean = 0;
			if(MonitorPara.PmeanValue > 1000)
			{
				strcpy(string, "--");
			}
			else if(MonitorPara.PmeanValue < 0)
			{	
				strcpy(string, _Float2String((float)MonitorPara.PmeanValue,0));	
			}
			else
			{
				strcpy(string, _Float2String((float)MonitorPara.PmeanValue/10,1));				
			}
		}
		if(OldPmean != MonitorPara.PmeanValue)
		{
			OldPmean = MonitorPara.PmeanValue;
			MultiText_ParaUpdate(hWin->Robj[index], string);
		}
	}
	//----------------------------------------------------------------------------------------------
	if((index = Get_MonitorRank_Rapid(DP_PEEP)) != -1)//判断当前参数是否可以显示
	{	
		if(BeathSettingPara.BreathMode == ACGO_MODE)
		{
			MultiText_ParaUpdate(hWin->Robj[index], "--");
		}
		else
		{
                  OldPeep = 0;
			if(MonitorPara.PeepValue > 1000) //zkq 20161127
			{
				strcpy(string, "--");
			}
			else if(MonitorPara.PeepValue < 0)
			{	
				strcpy(string, _Float2String((float)MonitorPara.PeepValue,0));	
			}
			else
			{
				strcpy(string, _Float2String((float)MonitorPara.PeepValue/10,1));	
			}
		}
		if(OldPeep != MonitorPara.PeepValue)
		{
			OldPeep = MonitorPara.PeepValue;
			MultiText_ParaUpdate(hWin->Robj[index], string);
		}
	}
	//----------------------------------------------------------------------------------------------
	if((index = Get_MonitorRank_Rapid(DP_MV)) != -1)//判断当前参数是否可以显示
	{	
		if(BeathSettingPara.BreathMode == ACGO_MODE)
		{
			MultiText_ParaUpdate(hWin->Robj[index], "--");
		}
		else
		{
                  OldMV = 0;
			if(MonitorPara.MinuteVolumeValue > 6000)
			{
				strcpy(string, "--");
			}
			else if((MonitorPara.MinuteVolumeValue <= 6000) && (MonitorPara.MinuteVolumeValue >= 1000))
			{
				strcpy(string, _Float2String((float)MonitorPara.MinuteVolumeValue/100, 1));
			}
			else if(MonitorPara.MinuteVolumeValue < 1000)
			{
				strcpy(string, _Float2String((float)MonitorPara.MinuteVolumeValue/100, 2));
			}
			else if(MonitorPara.MinuteVolumeValue == 0)
			{
				strcpy(string, _Float2String(0, 0));
			}
		}
		if(OldMV != MonitorPara.MinuteVolumeValue)
		{
			OldMV = MonitorPara.MinuteVolumeValue;
			MultiText_ParaUpdate(hWin->Robj[index], string);
		}
	}
	//----------------------------------------------------------------------------------------------
	if((index = Get_MonitorRank_Rapid(DP_F)) != -1)//判断当前参数是否可以显示
	{	
		if(BeathSettingPara.BreathMode == ACGO_MODE)
		{
			MultiText_ParaUpdate(hWin->Robj[index], "--");
		}
		else
		{
                  OldF = 0;
			if(MonitorPara.RateTotalValue > 100)
			{
				strcpy(string, "--");
			}
			else
			{
				if(MonitorPara.RateTotalValue < 0)
				{
					MonitorPara.RateTotalValue = 0;
				}
				strcpy(string, _Float2String((float)MonitorPara.RateTotalValue, 0));
			}
		}
		if(OldF != MonitorPara.RateTotalValue)
		{
			OldF = MonitorPara.RateTotalValue;
			MultiText_ParaUpdate(hWin->Robj[index], string);
		}
	}
	//----------------------------------------------------------------------------------------------
	if((index = Get_MonitorRank_Rapid(DP_VTE)) != -1)//判断当前参数是否可以显示
	{    
		if(BeathSettingPara.BreathMode == ACGO_MODE)
		{
			MultiText_ParaUpdate(hWin->Robj[index], "--");
		}
		else
		{
                  OldVTE = 0;
			if(MonitorPara.VteValue > 2500)//  20130812
			{			
				strcpy(string, "--");
			}
			else if(MonitorPara.VteValue < 0)
			{	
				strcpy(string, _Float2String((float)MonitorPara.VteValue,0));	
			}
			else
			{
				strcpy(string, _Float2String((float)MonitorPara.VteValue,0));
			}
		}
		if(OldVTE != MonitorPara.VteValue)
		{
			OldVTE = MonitorPara.VteValue;
			MultiText_ParaUpdate(hWin->Robj[index], string);
		}		
	}
	//----------------------------------------------------------------------------------------------
	if((index = Get_MonitorRank_Rapid(DP_VTI)) != -1)//判断当前参数是否可以显示
	{    
		if(BeathSettingPara.BreathMode == ACGO_MODE)
		{
			MultiText_ParaUpdate(hWin->Robj[index], "--");
		}
		else
		{
			strcpy(string, _Float2String((float)MonitorPara.VtiValue,0));			
		}
		if(OldVTI != MonitorPara.VtiValue)
		{
			OldVTI = MonitorPara.VtiValue;
			MultiText_ParaUpdate(hWin->Robj[index], string);
		}	
	}
	//----------------------------------------------------------------------------------------------
	if((index = Get_MonitorRank_Rapid(DP_FIO2)) != -1)//判断当前参数是否可以显示
	{
		if(!O2ModuleOff)    //关闭   djq add 20140221
		{
// #if(LOGO_FLAG == PENLON_LOGO)   //gyh add 20230713
#ifdef Pre_ACGO
			if((BeathSettingPara.BreathMode == ACGO_MODE)&&(MasimoISA_EtCO2_Config_On != 2)
				|| (!(AA_Module_Flag.EtCO2ModuleExist)&&((MasimoISA_EtCO2_Config_On==2)||(Drager_EtCO2_Config_On==2))))
			{
				MultiText_ParaUpdate(hWin->Robj[index], "--");
			}
			else
#endif
// #else
// #ifdef Pre_ACGO
// 			if((BeathSettingPara.BreathMode == ACGO_MODE))
// 			{
// 				MultiText_ParaUpdate(hWin->Robj[index], "--");
// 			}
// 			else
// #endif
// #endif
			{
				OldFiO2 = 1000;
				if(O2_Sensor_Disp_(MonitorPara.FiO2Value)) 
				{
					strcpy(string, "--");
				}
				else if(O2_Sensor_Disp100(MonitorPara.FiO2Value))
				{
					strcpy(string, _Float2String((float)100,0));
				}
				else
				{
					strcpy(string, _Float2String((float)MonitorPara.FiO2Value,0));
				}
			}
		}
		else
		{	
			strcpy(string, "--");
			MultiText_ParaUpdate(hWin->Robj[index], string);
               //   MultiText_ParaUpdate(hWin->Robj[index], "--");
		}	
		if(OldFiO2 != MonitorPara.FiO2Value)
		{
			OldFiO2 = MonitorPara.FiO2Value;
			MultiText_ParaUpdate(hWin->Robj[index], string);
		}
	}
	//  [2/16/2022 yaoyao 注释]CO2永远在左边----------------------------------------------------------------------------------------------
// 	if((index = Get_MonitorRank_Rapid(DP_ETCO2)) != -1)//判断当前参数是否可以显示
// 	{  
// 		//etCO2
// 		if((!AA_Module_Flag.EtCO2ModuleExist)||(!EtCO2ModuleOff))
// 		{
// 			MultiText_ParaUpdate(hWin->Robj[index], "--");
// 		}
// 		else
// 		{   //zkq 20161020 超过显示"---"
// 			if((ETCO2Unit == 0)||(ETCO2Unit == 2))
// 			{
// 				if(CO2_Parameters.etCO2 > 150/*133*/)
// 				{
// 					strcpy(string, "--");
// 				}
// 				else
// 				{
// 					strcpy(string, _Float2String((float)(CO2_Parameters.etCO2/10),1));
// 				}
// 			}
// 			else
// 			{
// 				if(CO2_Parameters.etCO2 > 113/*100*/)//  20130325 //MonitorPara.EtCO2Value //cpp md 20130510
// 				{
// 					strcpy(string, "--");
// 				}
// 				else
// 				{
// 					strcpy(string, _Float2String((float)(CO2_Parameters.etCO2),1));
// 				}
// 			}
// 		}
// 		if(OldEtCO2 != CO2_Parameters.etCO2)
// 		{
// 			OldEtCO2 = CO2_Parameters.etCO2;
// 			MultiText_ParaUpdate(hWin->Robj[index], string);
// 		}
// 	}

	//  [2/16/2022 yaoyao add]-Pulse---------------------------------------------------------------------------------------------
	if((index = Get_MonitorRank_Rapid(DP_PULSE)) != -1)
	{
		if((SpO2ModuleExist) && (!SpO2ModuleOff))
		{
			if((SpO2_Parameters.PulseRate > 0) && (SpO2_Parameters.PulseRate < 250))
			{
				strcpy(string, _Float2String((float)SpO2_Parameters.PulseRate,0));
			}
			else
			{
				strcpy(string, "--");
			}
		}
		else
		{
			MultiText_ParaUpdate(hWin->Robj[index], "--");
		}
		if (OldSpO2 != SpO2_Parameters.PulseRate)
		{
			OldSpO2 = SpO2_Parameters.PulseRate;
			MultiText_ParaUpdate(hWin->Robj[index], string);
		}
	}
	//[2/16/2022 yaoyao add]-SpO2---------------------------------------------------------------------------------------------
	if((index = Get_MonitorRank_Rapid(DP_SPO2)) != -1)
	{
		if((SpO2ModuleExist) && (!SpO2ModuleOff))
		{
			OldPulse = 1000;		//gyh add 20230816 血氧参数实时更新
			if((SpO2_Parameters.SpO2 > 0) && (SpO2_Parameters.SpO2 <= 100))
			{
				strcpy(string, _Float2String((float)SpO2_Parameters.SpO2,0));				
			}
			else
			{				
				strcpy(string, "--");
			}
		}
		else
		{
			MultiText_ParaUpdate(hWin->Robj[index], "--");
		}
		if(OldPulse != SpO2_Parameters.SpO2)
		{
			OldPulse = SpO2_Parameters.SpO2;
			MultiText_ParaUpdate(hWin->Robj[index], string);
		}
	}
	
	num++;
	if(num > 500)//Masimo O2 compensate
	{
		num = 0;
		if((Masimo_EtCO2_Config_On || MasimoISA_EtCO2_Config_On == 1) \
			&& AA_Module_Flag.EtCO2ModuleExist)//masimo IRMA / ISA AX+
		{
			if((OldFiO2 - MonitorPara.FiO2Value >= MonitorPara.FiO2Value*0.05) || (MonitorPara.FiO2Value - OldFiO2 >= MonitorPara.FiO2Value*0.05))
			{
				DataTransmit(MonitorPara.FiO2Value,IM_SET_O2VOL,Usart2_Tx_Buffer,USART2);
				OldFiO2 = MonitorPara.FiO2Value;
			}
		}
	}

	LeftRealtimeParaUpdate(hWin);
}

/***************************************************************************************************
* @brief 信息窗状态更新
* @para   
* @retval 
*/
int InfoWinStateUpdate(INFO_HWIN iWin)
{
    WM_MESSAGE Msg;
    WM_HWIN hwin;
    int len = 0;
	int i = 0;
    char pText[50]= {0};
    char pText1[20]= {0};
    char pText2[20]= {0};
    char pText4[10]= {0};
    char pText5[10]= {0};
    char pText6[10]= {0};
    char * pText3;

	char str[70]= {0};

	static GUI_COLOR Tmp_BkColor = WINDOW_BK_COLOR/*0x420606*/;//多重报警和报警静音的背景颜色

    //显示呼吸模式	
    if(iWin.InfoChangeFlag&MODES_INFO_CHANGED_FLAG)
    {
        iWin.InfoChangeFlag &=~MODES_INFO_CHANGED_FLAG;

        if(BeathSettingPara.BreathMode != PSV_MODE) 
        {
            BackUpMode = 0;
        }
		//IMAGETEXT_SetBkColor(iWin.bObj[0], 0x420606);
#ifdef DEMO //  [11/8/2022 lyy add 左上角模式下面显示DEMO]
		if (Demo_Switch)
		{
			sprintf(pText1,"DEMO\n%%s");
		}
		else
#endif
		{
			sprintf(pText1,"%%s");
		}
		switch(BeathSettingPara.BreathMode)
        {
        case VCV_MODE:
			sprintf(pText, pText1,Sys_Lib_GetDispStr(DP_VCV));
            IMAGETEXT_SetText(iWin.bObj[0],pText);
            break;
        case SIGH_MODE:
			sprintf(pText2, pText1, "%s       %s");
// 			IMAGETEXT_SetWrapMode(iWin.bObj[0], GUI_WRAPMODE_WORD);

			sprintf(pText, pText2, Sys_Lib_GetDispStr(DP_VCV), Sys_Lib_GetDispStr(DP_ADDSIGH));
			IMAGETEXT_SetText(iWin.bObj[0],pText);
            break;
        case SIMV_VCV_MODE:
			sprintf(pText, pText1,Sys_Lib_GetDispStr(DP_SIMVV));
			IMAGETEXT_SetText(iWin.bObj[0],pText);
            break;
        case SIMV_PCV_MODE:
			sprintf(pText, pText1,Sys_Lib_GetDispStr(DP_SIMVP));
			IMAGETEXT_SetText(iWin.bObj[0],pText);
            break;
        case PCV_MODE:
			sprintf(pText, pText1,Sys_Lib_GetDispStr(DP_PCV));
			IMAGETEXT_SetText(iWin.bObj[0],pText);
            break;
        case PSV_MODE:
			sprintf(pText, pText1,Sys_Lib_GetDispStr(DP_SPONTPSV));
			if(BackUpMode == BACKUP_MODE)
			{
				//BackUpMode = 0;
				sprintf(pText, pText1,Sys_Lib_GetDispStr(DP_BACKUP));
			}
			IMAGETEXT_SetText(iWin.bObj[0],pText);
            break;
        //*//  20130401
        case MANUAL_MODE:
#ifdef MANNUL_SWITCH
            if(Menu_hWin.hWin)
            {
//                hwin = WM_GetDialogItem(Menu_hWin.hWin, GUI_ID_BUTTON0); 
//                WM_DisableWindow(hwin);//ylk del 20181213
            }
//             if(MenuType == MODES_MENU)
//             {
//                 if(StandbyState!=0xFF)
//                 {
//                     PlanMenuType=MAIN_MENU;
//                 }
//                 else
//                 {
//                     PlanMenuType=STANDBY_MENU;
//                 }
//                 Task|=MEAU_UPDATE;
//                 BUTTON_SetPressed(WM_GetDialogItem(Menu_hWin.hWin,GUI_ID_BUTTON0),0);
//             }
#endif
			sprintf(pText, pText1,Sys_Lib_GetDispStr(DP_MANUAL));
           IMAGETEXT_SetText(iWin.bObj[0],pText);
            break;
//        case BACKUP_MODE:
//            IMAGETEXT_SetText(iWin.bObj[0],"BACKUP");
//            break;
        //*/
        case PRVC_MODE:
			sprintf(pText, pText1,"PRVC");
			IMAGETEXT_SetText(iWin.bObj[0],pText);
            break;
        case SIMV_PRVC_MODE:
			sprintf(pText, pText1,"SIMV-PRVC");
			IMAGETEXT_SetText(iWin.bObj[0],pText);
            break;
		case APRV_MODE:
			sprintf(pText, pText1,"APRV");
			IMAGETEXT_SetText(iWin.bObj[0],pText);
			break;
		case BIPAP_MODE:
			sprintf(pText, pText1,"DuoPhasic");
			IMAGETEXT_SetText(iWin.bObj[0],pText);
			break;
		case VS_MODE:
			sprintf(pText, pText1,"VS");
			IMAGETEXT_SetText(iWin.bObj[0],pText);
			break;
#ifdef ACGO_SWITCH
        case ACGO_MODE:
            if(Menu_hWin.hWin)
            {
//                hwin = WM_GetDialogItem(Menu_hWin.hWin, GUI_ID_BUTTON0); //ylk del 20181213
//                WM_DisableWindow(hwin);
            }
//             if(MenuType == MODES_MENU)
//             {
//                 if(StandbyState!=0xFF)
//                 {
//                     PlanMenuType=MAIN_MENU;
//                 }
//                 else
//                 {
//                     PlanMenuType=STANDBY_MENU;
//                 }
//                 Task|=MEAU_UPDATE;
//                 BUTTON_SetPressed(WM_GetDialogItem(Menu_hWin.hWin,GUI_ID_BUTTON0),0);
//             }
			sprintf(pText, pText1,"ACGO");
          IMAGETEXT_SetText(iWin.bObj[0],pText);
            break;
#endif
        default:
            break;
        }
    }
    //显示辅助肺标志图标
// 	IMAGETEXT_SetBitmapEx(iWin.bObj[1],&bmlung,0,0);
	if(iWin.InfoChangeFlag&ASSIST_INFO_CHANGED_FLAG)
	//if(1)
    {
        if((MonitorPara.VentState==INSP_PHASE)&&(AssistShowFlag))
        {
			IMAGETEXT_SetBitmapEx(iWin.bObj[1],&bmlung,0,0);//8,3 //cpp md 20130410
#ifdef __debug_display
            IMAGETEXT_SetBkColor(iWin.bObj[1],GUI_GREEN);
            IMAGETEXT_SetText(iWin.bObj[1],"TEST\nMODE");
#endif
            //IMAGETEXT_SetText(iWin.bObj[1],"assist");
        }
        else if(MonitorPara.VentState==EXP_PHASE)
        {
            IMAGETEXT__SetDrawObj(iWin.bObj[1],0);
#ifdef __debug_display
            IMAGETEXT_SetBkColor(iWin.bObj[1],GUI_GREEN);
            IMAGETEXT_SetText(iWin.bObj[1],"TEST\nMODE");
#endif        
            //IMAGETEXT_SetText(iWin.bObj[1],"");
        }
        AssistShowFlag = 0;
        iWin.InfoChangeFlag &=~ASSIST_INFO_CHANGED_FLAG;
    }
#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C) 
    if(StandbyState==0xFF)
    {
        IMAGETEXT__SetDrawObj(iWin.bObj[1],0);        
    }
#else
#ifdef DEMO  //djq add 20140214     
    if((StandbyState==0xFF) && (Demo_Switch==0))
    {
        IMAGETEXT__SetDrawObj(iWin.bObj[1],0);        
    }
#else
    if(StandbyState==0xFF)
    {
        IMAGETEXT__SetDrawObj(iWin.bObj[1],0);        
    }   
#endif
#endif
    //显示报警信息
    if(iWin.InfoChangeFlag&ALARM_INFO_CHANGED_FLAG)
    {
        if(ShowedAlarmInfo.ActiveState)
        {
            switch(ShowedAlarmInfo.Grade)
            {
            case LOW_PRIORITY:
            case MEDIUM_PRIORITY:
                IMAGETEXT_SetTextColor(iWin.bObj[2],GUI_BLACK);
                IMAGETEXT_SetBkColor(iWin.bObj[2],GUI_YELLOW);

				Tmp_BkColor = GUI_YELLOW;
                break;
            case HIGH_PRIORITY:
                IMAGETEXT_SetTextColor(iWin.bObj[2],GUI_WHITE);
                IMAGETEXT_SetBkColor(iWin.bObj[2],GUI_RED);

				Tmp_BkColor = GUI_RED;
                break;
            default:
                {
                }
            }

				if((ShowedAlarmInfo.Type == ALARM_FIO2_LESSTHAN_18) && (ShowedAlarmInfo.Grade == 1))
			{
				sprintf(str, "%s \n %s", Sys_Lib_GetDispStr(DP_FIO2LESS18PERCNET), Sys_Lib_GetDispStr(DP_O2FAIL));
				pText3 = str;
			}
			else
			{
				pText3 = ShowedAlarmInfo.Content;
			}
            len = strlen(pText3);
            if(len >= 50)
            {
                IMAGETEXT_SetFont(iWin.bObj[2],&GUI_Font_16);
            }
            else
            {
                IMAGETEXT_SetFont(iWin.bObj[2],&GUI_Font_24);
            }
            IMAGETEXT_SetText(iWin.bObj[2],pText3);
            Msg.hWin=iWin.bObj[2];
            Msg.MsgId=WM_PAINT;
        }
        else
        {
            if((AlarmsWork.unclearedAlarms) || (AlarmsWork.unclearedAlarms1))
            {
                IMAGETEXT_SetBkColor(iWin.bObj[2],WINDOW_BK_COLOR/*0x420606*/);
				if((ShowedAlarmInfo.Type == ALARM_FIO2_LESSTHAN_18) && (ShowedAlarmInfo.Grade == 1))
				{
					sprintf(str, "%s \n %s", Sys_Lib_GetDispStr(DP_FIO2LESS18PERCNET), Sys_Lib_GetDispStr(DP_O2FAIL));
					pText3 = str;
				}
				else
				{
					pText3 = ShowedAlarmInfo.Content;
				}
				len = strlen(pText3);
				if(len >= 50)
				{
					IMAGETEXT_SetFont(iWin.bObj[2],&GUI_Font_16);
				}
				else
				{
					IMAGETEXT_SetFont(iWin.bObj[2],&GUI_Font_24);
				}
                IMAGETEXT_SetTextColor(iWin.bObj[2],GUI_WHITE);
                IMAGETEXT_SetText(iWin.bObj[2],pText3);
            }
            else
            {
                IMAGETEXT_SetBkColor(iWin.bObj[2],WINDOW_BK_COLOR/*0x420606*/);
                IMAGETEXT_SetText(iWin.bObj[2],"");
            }
			Tmp_BkColor = WINDOW_BK_COLOR/*0x420606*/;
            if(BUTTON_IsPressed(WM_GetDialogItem(Menu_hWin.hWin, GUI_ID_BUTTON4)))
            {
		        BUTTON_SetPressed(WM_GetDialogItem(Menu_hWin.hWin, GUI_ID_BUTTON4),0);//sun add 20140723
            }
        }
        iWin.InfoChangeFlag &= ~ALARM_INFO_CHANGED_FLAG;
    }

    
    //if(iWin.InfoChangeFlag&CUR_ALARM_INFO_CHANGED_FLAG)
    //{
    //    AlarmsWork.ActiveNum = PresentAlarm_Num(AlarmsWork.presentAlarms, AlarmsWork.presentAlarms1);


    //    if (AlarmsWork.ActiveNum == 1)
    //    {
    //        ShowedAlarmInfoSecond.ActiveState = FALSE;
    //        ShowedAlarmInfoSecond.Content = "";
    //    }
    //    if(ShowedAlarmInfoSecond.ActiveState)
    //    {
    //        switch(ShowedAlarmInfoSecond.Grade)
    //        {
    //        case LOW_PRIORITY:
    //        case MEDIUM_PRIORITY:

    //            IMAGETEXT_SetTextColor(iWin.bObj[1],GUI_BLACK);
    //            IMAGETEXT_SetBkColor(iWin.bObj[1],GUI_YELLOW);

    //            IMAGETEXT_SetTextColor(iWin.bObj[10],GUI_BLACK);
    //            IMAGETEXT_SetBkColor(iWin.bObj[10],GUI_YELLOW);
    //            //  Tmp_BkColor = GUI_YELLOW;
    //            break;
    //        case HIGH_PRIORITY:

    //            IMAGETEXT_SetTextColor(iWin.bObj[1],GUI_WHITE);
    //            IMAGETEXT_SetBkColor(iWin.bObj[1],GUI_RED);

    //            IMAGETEXT_SetTextColor(iWin.bObj[10],GUI_WHITE);
    //            IMAGETEXT_SetBkColor(iWin.bObj[10],GUI_RED);
    //            //  Tmp_BkColor = GUI_RED;
    //            break;
    //        default:
    //            {
    //            }
    //        }
    //        pText10 = ShowedAlarmInfoSecond.Content;
    //        if (pText10 != NULL)
    //        {
    //            GUI_SetFont(&GUI_Font24B_Arial);
    //            len = GUI_GetStringDistX(pText10);//strlen(pText10);

    //            if(len >= 360)
    //            {
    //                IMAGETEXT_SetFont(iWin.bObj[1],&GUI_Font16B_Arial);
    //            }
    //            else
    //            {
    //                IMAGETEXT_SetFont(iWin.bObj[1],&GUI_Font24B_Arial);
    //            }

    //            IMAGETEXT_SetText(iWin.bObj[1],pText10);
    //            Msg.hWin=iWin.bObj[1];
    //            Msg.MsgId=WM_PAINT;
    //        }
    //    }
    //    else
    //    {
    //        //                  if((AlarmsWork.unclearedAlarms) || (AlarmsWork.unclearedAlarms1))
    //        //                  {
    //        //                      IMAGETEXT_SetBkColor(iWin.bObj[1],GUI_LIGHT_BLACK);
    //        //                      IMAGETEXT_SetBkColor(iWin.bObj[10],GUI_LIGHT_BLACK);
    //        //                      pText10 = ShowedAlarmInfoSecond.Content;
    //        //                      if (pText10 != NULL)
    //        //                      {
    //        //                          len = strlen(pText10);
    //        //                          if(len >= 50)
    //        //                          {
    //        //                              IMAGETEXT_SetFont(iWin.bObj[1],&GUI_Font16B_Arial);
    //        //                          }
    //        //                          else
    //        //                          {
    //        //                              IMAGETEXT_SetFont(iWin.bObj[1],&GUI_Font24B_Arial);
    //        //                          }
    //        //                          IMAGETEXT_SetTextColor(iWin.bObj[1],GUI_WHITE);
    //        //                          IMAGETEXT_SetText(iWin.bObj[1],pText10);
    //        //                      }
    //        //                      
    //        //                  }
    //        //                  else
    //        {
    //            IMAGETEXT_SetBkColor(iWin.bObj[1],GUI_LIGHT_BLACK);
    //            IMAGETEXT_SetText(iWin.bObj[1],"");

    //            IMAGETEXT_SetBkColor(iWin.bObj[10],GUI_LIGHT_BLACK);
    //            IMAGETEXT_SetText(iWin.bObj[10],"");
    //        }
    //        //Tmp_BkColor = GUI_LIGHT_BLACK;
    //        if(BUTTON_IsPressed(WM_GetDialogItem(Menu_hWin.hWin, GUI_ID_BUTTON4)))
    //        {
    //            BUTTON_SetPressed(WM_GetDialogItem(Menu_hWin.hWin, GUI_ID_BUTTON4),0);//sun add 20140723
    //        }
    //    }



    //    iWin.InfoChangeFlag &= ~CUR_ALARM_INFO_CHANGED_FLAG;
    //}

/**************************************************************************************************/
	if(iWin.InfoChangeFlag&ALARM_MUTI_CHANGED_FLAG)
	{
		iWin.InfoChangeFlag &= ~ALARM_MUTI_CHANGED_FLAG;
		IMAGETEXT_SetBkColor(iWin.bObj[6],Tmp_BkColor);//设置背景颜色
		AlarmsWork.ActiveNum = PresentAlarm_Num(AlarmsWork.presentAlarms, AlarmsWork.presentAlarms1);
		if(AlarmsWork.ActiveNum > 1)  //添加多重报警的提示
		{
			switch(ShowedAlarmInfo.Grade)
			{
			case LOW_PRIORITY:
			case MEDIUM_PRIORITY:
				IMAGETEXT_SetTextColor(iWin.bObj[6],GUI_BLACK);
				TEXT_SetTextColor(iWin.bObj[7],GUI_BLACK);
				break;
			case HIGH_PRIORITY:
				IMAGETEXT_SetTextColor(iWin.bObj[6],GUI_WHITE);
				TEXT_SetTextColor(iWin.bObj[7],GUI_WHITE);
				break;
			}
			IMAGETEXT_SetText(iWin.bObj[6],"U");
			sprintf(pText,"%d",AlarmsWork.ActiveNum);
			TEXT_SetText(iWin.bObj[7],pText);
		}
		else  //添加取消多重报警的状态 
		{
			IMAGETEXT_SetText(iWin.bObj[6],"");
			TEXT_SetText(iWin.bObj[7],"");
		}
	}
/**************************************************************************************************/

	//显示静音
    if(iWin.InfoChangeFlag&SILENCE_INFO_CHANGED_FLAG)
    {
		IMAGETEXT_SetBkColor(iWin.bObj[3],Tmp_BkColor);
		IMAGETEXT_SetBkColor(iWin.bObj[12],Tmp_BkColor);
       if((AlarmsWork.BeepState==FALSE) && (AlarmPriority != 0x04))
        {
			switch (ShowedAlarmInfo.Grade)
			{
			case LOW_PRIORITY:
			case MEDIUM_PRIORITY:
				Tmp_BkColor = GUI_YELLOW;
				IMAGETEXT_SetBkColor(iWin.bObj[3],Tmp_BkColor);
				IMAGETEXT_SetBkColor(iWin.bObj[12],Tmp_BkColor);
				break;
			case HIGH_PRIORITY:
				Tmp_BkColor = GUI_RED;
				IMAGETEXT_SetBkColor(iWin.bObj[3],Tmp_BkColor);
				IMAGETEXT_SetBkColor(iWin.bObj[12],Tmp_BkColor);
				break;
			default:
				{
				}
			}
            //IMAGETEXT_SetBitmapEx(iWin.bObj[3],&bmsilence_icon,10,0);//14,0 //cpp md 20130410 
			IMAGETEXT_SetText(iWin.bObj[3],"S");

            sprintf(pText,"%3d",AlarmsWork.MuteTimer);
            IMAGETEXT_SetText(iWin.bObj[12],pText);
        }
        else
        {
            //IMAGETEXT__SetDrawObj(iWin.bObj[3],0,0);
			IMAGETEXT_SetText(iWin.bObj[3],"");

            IMAGETEXT_SetText(iWin.bObj[12],"");
            if(BUTTON_IsPressed(WM_GetDialogItem(Menu_hWin.hWin, GUI_ID_BUTTON4)))
            {
                BUTTON_SetPressed(WM_GetDialogItem(Menu_hWin.hWin, GUI_ID_BUTTON4),0);
            }
        }
        iWin.InfoChangeFlag &= ~SILENCE_INFO_CHANGED_FLAG;
    }
    //显示电源
    if(iWin.InfoChangeFlag&POWER_INFO_CHANGED_FLAG)
    {
        POWER_INFO_SetPowerState(iWin.pObj,Power_GetState());
        if(Power_GetState() == (AC_WORK))//make a charge indication
        {
            if(!ChargeFinishFlag)
            {
                BatteryStateNum ++;
                if(BatteryStateNum > 4)
                {
                    BatteryStateNum = 1;
                }
            }
            else if (ChargeFinishFlag == 1)
            {
                BatteryStateNum = 4;
            }
            else 
            {
                BatteryStateNum = 1;
            }
	    }
        else
        {
            BatteryStateNum = GetBatteryVolume(RealtimePara.BatteryVoltage);//电池电量检测电池格数
        }

        POWER_INFO_SetBatteryState(iWin.pObj,BatteryStateNum);
        Msg.hWin=iWin.pObj;
        Msg.MsgId=WM_STATUS_CHANGED;
        WM_SelectWindow(Msg.hWin);
        WM_SendMessage(Msg.hWin,&Msg);
        iWin.InfoChangeFlag &= ~POWER_INFO_CHANGED_FLAG;
    }

#if 0
    sprintf(pText,"%d",GUI_ALLOC_GetNumFreeBytes());
    //sprintf(pText,"%d",OS_TimeMS);
    //sprintf(pText,"%d\n%d",MonitorPara.FiO2Value,AA_all_data.AA_detail.AA_O2.FI));
    IMAGETEXT_SetText(iWin.bObj[4],pText);
#else
    //显示时间
    if(iWin.InfoChangeFlag&TIMER_INFO_CHANGED_FLAG)
    {
        sprintf(pText4,"%4d",RTCTime.RTC_Year);
        sprintf(pText5,"%02d",RTCTime.RTC_Mon);
        sprintf(pText6,"%02d",RTCTime.RTC_Mday);
        sprintf(pText1,"%02d",RTCTime.RTC_Min);
        sprintf(pText2,"%2d",RTCTime.RTC_Hour);
		sprintf(pText," %.4s/%.2s/%.2s  %.2s:%.2s",pText4,pText5,pText6,pText2,pText1);
        IMAGETEXT_SetText(iWin.bObj[4],pText);
        iWin.InfoChangeFlag &= ~TIMER_INFO_CHANGED_FLAG;
    }
#endif
    //显示驱动气类型  工作和待机都显示
    if(iWin.InfoChangeFlag&DRIVER_GAS_TYPE_FLAG)
    {
        if(DriverGas_Chose_Flag)//djq add 20170609驱动气体选配
        {
			if(DirverGasFlag)
			{
				IMAGETEXT_SetBitmapEx(iWin.bObj[5],&bmdrive_gas_Air,11,19);
			}
			else
			{
				IMAGETEXT_SetBitmapEx(iWin.bObj[5],&bmdrive_gas_O2,11,19);
			}
			if(!WM_IsVisible(iWin.bObj[5]))
			{
				if(!CHECK_DEBUG)
				{
					WM_ShowWindow(iWin.bObj[5]);
				}
			}
		}
		else if(WM_IsVisible(iWin.bObj[5]))
		{
			if(!CHECK_DEBUG)
			{
				WM_HideWindow(iWin.bObj[5]);
			}
		}
        iWin.InfoChangeFlag &= ~DRIVER_GAS_TYPE_FLAG;
    }
	if(iWin.InfoChangeFlag & PHASEIN_ZERO_CHANGE_FLAG)//气源报警发生变化   djq add 20160727
	{
		if((MenuType == PHASEIN_ZERO_CAL_MENU)&&(WM_IsWindow(Menu_hWin.RecordWin)))
		{
			WM_SendMessageNoPara(Menu_hWin.RecordWin,WM_INIT_DIALOG);
		}
		iWin.InfoChangeFlag &= ~PHASEIN_ZERO_CHANGE_FLAG;
	}
	if(iWin.InfoChangeFlag & AA_RANGE_CHANGE_FLAG)//ylk add 20181128
	{
		AA_Range_Para((void*)&AA_all_data);
		iWin.InfoChangeFlag &= ~AA_RANGE_CHANGE_FLAG;
	}
	//CPB
	if (iWin.InfoChangeFlag & CPB_CHANGE_FLAG)
	{
		if (LanguageType == Disp_Lg_Chinese)
		{
			IMAGETEXT_SetBitmap(iWin.bObj[9],&bmCPB_ImageInfo_CN);
		}
		else
		{
			IMAGETEXT_SetBitmap(iWin.bObj[9],&bmCPB_ImageInfo_EN);
		}
		if (CPB_GetFlag()&&!StandbyState)
		{
			WM_ShowWindow(iWin.bObj[9]);
// 			WM_BringToTop(iWin.bObj[9]);
			WM_BringToBottom(Menu_hWin.HomeWin.ParentWin);
		}
		else
		{
			WM_HideWindow(iWin.bObj[9]);
		}
		iWin.InfoChangeFlag &= ~CPB_CHANGE_FLAG;
	}
    //患者
	if (iWin.InfoChangeFlag & PATIENT_TYPE_INFO_CHANGE_FLAG)
	{
		switch (Patient_GetPType())
		{
		case PATIENTTYPE_ADULT:
			IMAGETEXT_SetText(iWin.bObj[10],"A");
			break;
		case PATIENTTYPE_PAEDIATRIC:
			IMAGETEXT_SetText(iWin.bObj[10],"P");
			break;
		case PATIENTTYPE_INFANT:
			IMAGETEXT_SetText(iWin.bObj[10],"I");
			break;
		}
		if (Patient_GetPGender() == GENDER_FEMALE)
		{
			sprintf(pText1,"%s",Sys_Lib_GetDispStr(DP_FEMALE));
		}
		else
		{
			sprintf(pText1,"%s",Sys_Lib_GetDispStr(DP_MALE));
		}
		sprintf(pText2,"%d%s",(int)(Patient_GetIBW()),Sys_Lib_GetDispStr(DP_KG));
		sprintf(pText,"%s\n%s\n%s:%d",pText1,pText2,Sys_Lib_GetDispStr(DP_AGE),Patient_GetAge());
		TEXT_SetText(iWin.bObj[11],pText);
		iWin.InfoChangeFlag &= ~PATIENT_TYPE_INFO_CHANGE_FLAG;
	}
	return(iWin.InfoChangeFlag);
}
/***************************************************************************************************
* @brief  监测数据更新
* @para   
* @retval 
*/
void MonitorDataUpdate(WM_HWIN mWin)
{
    WM_HWIN tWin,hDialog;
    WM_MESSAGE Msg;
    tWin = WM_GetDialogItem(mWin, GUI_ID_MULTIPAGE0);
    hDialog = MULTIPAGE_GetWindow(tWin,0);
	//
    tWin = WM_GetDialogItem(hDialog, GUI_ID_TEXT0);
    if(BeathSettingPara.BreathMode == ACGO_MODE)
    {
        TextExCtrl_SetTextData(tWin,5000);
    }
    else
    {
        TextExCtrl_SetTextData(tWin,MonitorPara.VtiValue);
    }
    Msg.hWin = tWin;
    Msg.MsgId = WM_NOTIFICATION_VALUE_CHANGED;
    WM_SelectWindow(Msg.hWin);
    WM_SendMessage(Msg.hWin,&Msg);
	//
    tWin = WM_GetDialogItem(hDialog, GUI_ID_TEXT1);
    if(BeathSettingPara.BreathMode == ACGO_MODE)
    {
        TextExCtrl_SetTextData(tWin,5000);
    }
    else
    {
        TextExCtrl_SetTextData(tWin,MonitorPara.VteValue);
    }
    Msg.hWin = tWin;
    Msg.MsgId = WM_NOTIFICATION_VALUE_CHANGED;
    WM_SelectWindow(Msg.hWin);
    WM_SendMessage(Msg.hWin,&Msg);
	//
    tWin = WM_GetDialogItem(hDialog, GUI_ID_TEXT2);
    if(BeathSettingPara.BreathMode == ACGO_MODE)
    {
        TextExCtrl_SetTextData(tWin,111150000);
    }
    else
    {
        TextExCtrl_SetTextData(tWin,MonitorPara.MinuteVolumeValue);
    }
    Msg.hWin = tWin;
    Msg.MsgId = WM_NOTIFICATION_VALUE_CHANGED;
    WM_SelectWindow(Msg.hWin);
    WM_SendMessage(Msg.hWin,&Msg);
	//
    tWin = WM_GetDialogItem(hDialog, GUI_ID_TEXT3);
    if(BeathSettingPara.BreathMode == ACGO_MODE)
    {
        TextExCtrl_SetTextData(tWin,111150000);
    }
    else
    {
        TextExCtrl_SetTextData(tWin,MonitorPara.MVspnValue);
    }
    Msg.hWin = tWin;
    Msg.MsgId = WM_NOTIFICATION_VALUE_CHANGED;
    WM_SelectWindow(Msg.hWin);
    WM_SendMessage(Msg.hWin,&Msg);
	//
    tWin = WM_GetDialogItem(hDialog, GUI_ID_TEXT4);
    if(BeathSettingPara.BreathMode == ACGO_MODE)
    {
        TextExCtrl_SetTextData(tWin,5000);
    }
    else
    {
        TextExCtrl_SetTextData(tWin,MonitorPara.RateTotalValue);
    }
    Msg.hWin = tWin;
    Msg.MsgId = WM_NOTIFICATION_VALUE_CHANGED;
    WM_SelectWindow(Msg.hWin);
    WM_SendMessage(Msg.hWin,&Msg);
	//
    tWin = WM_GetDialogItem(hDialog, GUI_ID_TEXT5);
    if(BeathSettingPara.BreathMode == ACGO_MODE)
    {
        TextExCtrl_SetTextData(tWin,5000);
    }
    else
    {
        TextExCtrl_SetTextData(tWin,MonitorPara.fspnValue);
    }
    Msg.hWin = tWin;
    Msg.MsgId = WM_NOTIFICATION_VALUE_CHANGED;
    WM_SelectWindow(Msg.hWin);
    WM_SendMessage(Msg.hWin,&Msg);
	//
    tWin = WM_GetDialogItem(hDialog, GUI_ID_TEXT6);
    if(BeathSettingPara.BreathMode == ACGO_MODE)
    {
        TextExCtrl_SetTextData(tWin,5000);
    }
    else
    {
        TextExCtrl_SetTextData(tWin,MonitorPara.PpeakValue);
    }
    Msg.hWin = tWin;
    Msg.MsgId = WM_NOTIFICATION_VALUE_CHANGED;
    WM_SelectWindow(Msg.hWin);
    WM_SendMessage(Msg.hWin,&Msg);
	//
    tWin = WM_GetDialogItem(hDialog, GUI_ID_TEXT7);
    if(BeathSettingPara.BreathMode == ACGO_MODE)
    {
        TextExCtrl_SetTextData(tWin,5000);
    }
    else
    {
        TextExCtrl_SetTextData(tWin,MonitorPara.PmeanValue);
    }
    Msg.hWin = tWin;
    Msg.MsgId = WM_NOTIFICATION_VALUE_CHANGED;
    WM_SelectWindow(Msg.hWin);
    WM_SendMessage(Msg.hWin,&Msg);
	//
    tWin = WM_GetDialogItem(hDialog, GUI_ID_TEXT8);
    if(BeathSettingPara.BreathMode == ACGO_MODE)
    {
        TextExCtrl_SetTextData(tWin,5000);
    }
    else
    {
        TextExCtrl_SetTextData(tWin,MonitorPara.PplatValue);
    }
    Msg.hWin = tWin;
    Msg.MsgId = WM_NOTIFICATION_VALUE_CHANGED;
    WM_SelectWindow(Msg.hWin);
    WM_SendMessage(Msg.hWin,&Msg);
	//
    tWin = WM_GetDialogItem(hDialog, GUI_ID_TEXT9);
    if(BeathSettingPara.BreathMode == ACGO_MODE)
    {
        TextExCtrl_SetTextData(tWin,5000);
    }
    else
    {
        TextExCtrl_SetTextData(tWin,MonitorPara.PeepValue);
    }
    Msg.hWin = tWin;
    Msg.MsgId = WM_NOTIFICATION_VALUE_CHANGED;
    WM_SelectWindow(Msg.hWin);
    WM_SendMessage(Msg.hWin,&Msg);
	//
    tWin = WM_GetDialogItem(hDialog, GUI_ID_TEXT10);
    if(BeathSettingPara.BreathMode == ACGO_MODE)
    {
        TextExCtrl_SetTextData(tWin,5000);
    }
    else
    {
        TextExCtrl_SetTextData(tWin,MonitorPara.IEValue);
    }
    Msg.hWin = tWin;
    Msg.MsgId = WM_NOTIFICATION_VALUE_CHANGED;
    WM_SelectWindow(Msg.hWin);
    WM_SendMessage(Msg.hWin,&Msg);
	//
    tWin = WM_GetDialogItem(hDialog, GUI_ID_TEXT11);

    if(!O2ModuleOff)//开启   //djq add 20140221
    {
#if(LOGO_FLAG == PENLON_LOGO)          //gyh add 20230713
#ifdef Pre_ACGO
		if((BeathSettingPara.BreathMode == ACGO_MODE)&&(MasimoISA_EtCO2_Config_On != 2))
		{
			TextExCtrl_SetTextData(tWin,5000);
		}
		else
#endif
#else
#ifdef Pre_ACGO
		if(BeathSettingPara.BreathMode == ACGO_MODE)
		{
			TextExCtrl_SetTextData(tWin,5000);
		}
		else
#endif                
#endif
		{
			if(O2_Sensor_Disp_(MonitorPara.FiO2Value)) //ylk add 20190125
			{
				TextExCtrl_SetTextData(tWin,5000);
			}
			else if(O2_Sensor_Disp100(MonitorPara.FiO2Value))
			{
				TextExCtrl_SetTextData(tWin,100);
			}
			else
			{
				TextExCtrl_SetTextData(tWin,MonitorPara.FiO2Value);
			}
		}
    }
    else 
    {
        TextExCtrl_SetTextData(tWin,5000);
    } 
    Msg.hWin = tWin;
    Msg.MsgId = WM_NOTIFICATION_VALUE_CHANGED;
    WM_SelectWindow(Msg.hWin);
    WM_SendMessage(Msg.hWin,&Msg);
	//
    tWin = WM_GetDialogItem(hDialog, GUI_ID_TEXT12);
    if( (BeathSettingPara.SettingTpValue!=0)
      &&((BeathSettingPara.BreathMode==VCV_MODE)||(BeathSettingPara.BreathMode==SIGH_MODE))
        &&(BeathSettingPara.BreathMode != ACGO_MODE)
      )
    {
        TextExCtrl_SetTextData(tWin,MonitorPara.ResistValue);
    }
    else
    {
        TextExCtrl_SetTextData(tWin,5000);
    }
    Msg.hWin = tWin;
    Msg.MsgId = WM_NOTIFICATION_VALUE_CHANGED;
    WM_SelectWindow(Msg.hWin);
    WM_SendMessage(Msg.hWin,&Msg);
	//
    tWin = WM_GetDialogItem(hDialog, GUI_ID_TEXT13);
    if(BeathSettingPara.BreathMode == ACGO_MODE)
    {
        TextExCtrl_SetTextData(tWin,5000);
    }
    else
    {
        TextExCtrl_SetTextData(tWin,MonitorPara.ComplianceValue);//Cdyn
    }
    Msg.hWin = tWin;
    Msg.MsgId = WM_NOTIFICATION_VALUE_CHANGED;
    WM_SelectWindow(Msg.hWin);
    WM_SendMessage(Msg.hWin,&Msg);

#if 1 //  20130409
#ifdef LANGUAGE_CHINESE_FLAG
    //
    tWin = WM_GetDialogItem(hDialog, GUI_ID_TEXT14);
    TextExCtrl_SetTextData(tWin,0xFFFF);
    Msg.hWin = tWin;
    Msg.MsgId = WM_NOTIFICATION_VALUE_CHANGED;
    WM_SelectWindow(Msg.hWin);
    WM_SendMessage(Msg.hWin,&Msg); 
    //
    tWin = WM_GetDialogItem(hDialog, GUI_ID_TEXT15);
    TextExCtrl_SetTextData(tWin,0xFFFF);
    Msg.hWin = tWin;
    Msg.MsgId = WM_NOTIFICATION_VALUE_CHANGED;
    WM_SelectWindow(Msg.hWin);
    WM_SendMessage(Msg.hWin,&Msg); 

	tWin = WM_GetDialogItem(hDialog, GUI_ID_TEXT16);
	TextExCtrl_SetTextData(tWin,0xFFFF);
	Msg.hWin = tWin;
	Msg.MsgId = WM_NOTIFICATION_VALUE_CHANGED;
	WM_SelectWindow(Msg.hWin);
	WM_SendMessage(Msg.hWin,&Msg); 
#else
    tWin = WM_GetDialogItem(hDialog, GUI_ID_TEXT14);
    if((SpO2ModuleExist)&&(!SpO2ModuleOff)) //  20130510
    {
        TextExCtrl_SetTextData(tWin,SpO2_Parameters.SpO2);
    }
    else 
    {
        TextExCtrl_SetTextData(tWin,5000);//djq add 20140221
    }
    Msg.hWin = tWin;
    Msg.MsgId = WM_NOTIFICATION_VALUE_CHANGED;
    WM_SelectWindow(Msg.hWin);
    WM_SendMessage(Msg.hWin,&Msg);
    //
    tWin = WM_GetDialogItem(hDialog, GUI_ID_TEXT15);
    if((SpO2ModuleExist)&&(!SpO2ModuleOff)) //  20130510
    {
        TextExCtrl_SetTextData(tWin,SpO2_Parameters.PulseRate);
    }
    else 
    {
        TextExCtrl_SetTextData(tWin,5000);//djq add 20140221
    }
    Msg.hWin = tWin;
    Msg.MsgId = WM_NOTIFICATION_VALUE_CHANGED;
    WM_SelectWindow(Msg.hWin);
    WM_SendMessage(Msg.hWin,&Msg);
	//
	tWin = WM_GetDialogItem(hDialog, GUI_ID_TEXT16);
	if((SpO2ModuleExist)&&(!SpO2ModuleOff)) //  20130510
	{
        TextExCtrl_SetTextData(tWin,SpO2_Parameters.PerfusionIndex);        
	}
	else 
	{
		TextExCtrl_SetTextData(tWin,5000);//djq add 20140221
	}
	if(SpO2_Choose == 1)//国产SpO2
	{
		TextExCtrl_SetTextData(tWin,5000);
	}	
	Msg.hWin = tWin;
	Msg.MsgId = WM_NOTIFICATION_VALUE_CHANGED;
	WM_SelectWindow(Msg.hWin);
	WM_SendMessage(Msg.hWin,&Msg);
#endif
#endif 
}
/***************************************************************************************************
* @brief  监测数据更新
* @para   
* @retval 
*/
void MonitorDataUpdate1(WM_HWIN mWin) 
{
	float MacValue = 0;

    WM_HWIN tWin,tWin1,hDialog;
    WM_MESSAGE Msg;
	int j=0;
	static unsigned char LastAA1Id = 0,LastAA2Id = 0;

    tWin = WM_GetDialogItem(mWin, GUI_ID_MULTIPAGE0);
    hDialog = MULTIPAGE_GetWindow(tWin,1);
	//
    //----------------------------------------------------------------------------------------------
    tWin = WM_GetDialogItem(hDialog,GUI_ID_TEXT0);
    if((AA_all_data.AA_detail.AA_AA1.ID) && (AA_all_data.AA_detail.AA_AA1.ET != 0xFF))
    {
        MacValue += (float)(AA_all_data.AA_detail.AA_AA1.ET) / MAC_Base[AA_all_data.AA_detail.AA_AA1.ID];  //sun add 20140605
    }
    if((AA_all_data.AA_detail.AA_AA2.ID) && (AA_all_data.AA_detail.AA_AA2.ET != 0xFF))
    {
        MacValue += (float)(AA_all_data.AA_detail.AA_AA2.ET) / MAC_Base[AA_all_data.AA_detail.AA_AA2.ID];  //sun add 20140605
    }
    if(AA_all_data.AA_detail.AA_N2O.ET != 0xFF)
    {
        MacValue += (AA_all_data.AA_detail.AA_N2O.ET)/10;
    }
    MacValue = AA_Para.MAC/10.0; //tcc add 20220525
    if(
       (   (AA_all_data.AA_detail.AA_AA1.ID && AA_all_data.AA_detail.AA_AA2.ID ) 
        || (AA_all_data.AA_detail.AA_AA1.ID && AA_all_data.AA_detail.AA_N2O.ET != 0xFF ) 
        || (AA_all_data.AA_detail.AA_AA2.ID && AA_all_data.AA_detail.AA_N2O.ET != 0xFF )) 
        && (AA_Module_Flag.EtCO2ModuleExist)
        && (EtCO2ModuleOff)                               
      )
    {
        if( MacValue > 100 )//zkq 20161020 MAC监测值大于10% 显示 -- 
        {
            TextExCtrl_SetTextData(tWin,5000);
        }
        else
        {
            TextExCtrl_SetTextData(tWin,(U16)(MacValue + 0.5));
            //TextExCtrl_SetTextData(tWin,AA_Para.MAC);
        }
    }
    else
    {
        TextExCtrl_SetTextData(tWin,5000);
    }

    Msg.hWin = tWin;
    Msg.MsgId = WM_NOTIFICATION_VALUE_CHANGED;
	WM_SelectWindow(Msg.hWin);
    WM_SendMessage(Msg.hWin,&Msg);
#if 1
	//  20130325
	tWin = WM_GetDialogItem(hDialog, GUI_ID_TEXT2);

	if((AA_Module_Flag.EtCO2ModuleExist)&&(EtCO2ModuleOff))
	{   //zkq 20161020 限制参数值
		if(Drager_EtCO2_Config_On == 2)
		{
			if(AA_all_data.AA_detail.AA_CO2.Accuracy == ACCURACY_Iso 
				&& AA_all_data.AA_detail.AA_N2O.Accuracy == ACCURACY_Iso
				&& AA_all_data.AA_detail.AA_AA1.Accuracy == ACCURACY_Iso
				&& AA_all_data.AA_detail.AA_O2.Accuracy  == ACCURACY_Iso)//都达到高精度
			{
				if((ETCO2Unit == 0)||(ETCO2Unit == 2))
				{
					if(CO2_Parameters.etCO2 > 150/*133*/)
					{
						TextExCtrl_SetTextData(tWin,5000);
					}
					else
					{
						TextExCtrl_SetTextData(tWin,(int)(CO2_Parameters.etCO2));
					}
				}
				else
				{
					if(CO2_Parameters.etCO2 > 113/*100*/)
					{
						TextExCtrl_SetTextData(tWin,5000);
					}
					else
					{
						TextExCtrl_SetTextData(tWin,(int)(CO2_Parameters.etCO2 * 10));
					}
				}
			}
		}
		else if(Drager_EtCO2_Config_On == 1)
		{
			if(AA_all_data.AA_detail.AA_CO2.Accuracy == ACCURACY_Iso 
				&& AA_all_data.AA_detail.AA_N2O.Accuracy == ACCURACY_Iso
				&& AA_all_data.AA_detail.AA_AA1.Accuracy == ACCURACY_Iso)//都达到高精度
			{
				if((ETCO2Unit == 0)||(ETCO2Unit == 2))
				{
					if(CO2_Parameters.etCO2 > 150/*133*/)
					{
						TextExCtrl_SetTextData(tWin,5000);
					}
					else
					{
						TextExCtrl_SetTextData(tWin,(int)(CO2_Parameters.etCO2));
					}
				}
				else        
				{
					if(CO2_Parameters.etCO2 > 113/*100*/)
					{
						TextExCtrl_SetTextData(tWin,5000);
					}
					else
					{
						TextExCtrl_SetTextData(tWin,(int)(CO2_Parameters.etCO2 * 10));
					}
				}
			}
			else
			{
				TextExCtrl_SetTextData(tWin,0xFFFF);
			}
		}
		else
		{
			if((ETCO2Unit == 0)||(ETCO2Unit == 2))
			{
				if(CO2_Parameters.etCO2 > 150/*133*/)
				{
					TextExCtrl_SetTextData(tWin,5000);
				}
				else
				{
					TextExCtrl_SetTextData(tWin,(int)(CO2_Parameters.etCO2));
				}
			}
			else        
			{
				if(CO2_Parameters.etCO2 > 113/*100*/)
				{
					TextExCtrl_SetTextData(tWin,5000);
				}
				else
				{
					TextExCtrl_SetTextData(tWin,(int)(CO2_Parameters.etCO2 * 10));
				}
			}
		}
	}
	else 
	{
		TextExCtrl_SetTextData(tWin,5000);//djq add 20140221
	}
	Msg.hWin = tWin;
	Msg.MsgId = WM_NOTIFICATION_VALUE_CHANGED;
	WM_SelectWindow(Msg.hWin);
	WM_SendMessage(Msg.hWin,&Msg);
	//
	tWin = WM_GetDialogItem(hDialog, GUI_ID_TEXT1);

	if((AA_Module_Flag.EtCO2ModuleExist)&&(EtCO2ModuleOff)) //  20130510
	{
		if(Drager_EtCO2_Config_On == 2)
		{
			if(AA_all_data.AA_detail.AA_CO2.Accuracy == ACCURACY_Iso 
				&& AA_all_data.AA_detail.AA_N2O.Accuracy == ACCURACY_Iso
				&& AA_all_data.AA_detail.AA_AA1.Accuracy == ACCURACY_Iso
				&& AA_all_data.AA_detail.AA_O2.Accuracy  == ACCURACY_Iso)//都达到高精度
			{
				if((ETCO2Unit == 0)||(ETCO2Unit == 2))
				{
					if(CO2_Parameters.insCO2 > 150/*133*/)
					{
						TextExCtrl_SetTextData(tWin,5000);
					}
					else
					{
						TextExCtrl_SetTextData(tWin,(int)(CO2_Parameters.insCO2));
					}
				}
				else
				{
					if(CO2_Parameters.insCO2 > 113/*100*/)
					{
						TextExCtrl_SetTextData(tWin,5000);
					}
					else
					{
						TextExCtrl_SetTextData(tWin,(int)(CO2_Parameters.insCO2 * 10));
					}
				}
			}
		}
		else if(Drager_EtCO2_Config_On == 1)
		{
			if(AA_all_data.AA_detail.AA_CO2.Accuracy == ACCURACY_Iso 
				&& AA_all_data.AA_detail.AA_N2O.Accuracy == ACCURACY_Iso
				&& AA_all_data.AA_detail.AA_AA1.Accuracy == ACCURACY_Iso)//都达到高精度
			{
				if((ETCO2Unit == 0)||(ETCO2Unit == 2))
				{
					if(CO2_Parameters.insCO2 > 150/*133*/)
					{
						TextExCtrl_SetTextData(tWin,5000);
					}
					else
					{
						TextExCtrl_SetTextData(tWin,(int)(CO2_Parameters.insCO2));
					}
				}
				else
				{
					if(CO2_Parameters.insCO2 > 113/*100*/)
					{
						TextExCtrl_SetTextData(tWin,5000);
					}
					else
					{
						TextExCtrl_SetTextData(tWin,(int)(CO2_Parameters.insCO2 * 10));
					}
				}
			}
			else
			{
				TextExCtrl_SetTextData(tWin,0xFFFF);
			}
		}
		else
		{
			if((ETCO2Unit == 0)||(ETCO2Unit == 2))
			{
				if(CO2_Parameters.insCO2 > 150/*133*/)
				{
					TextExCtrl_SetTextData(tWin,5000);
				}
				else
				{
					TextExCtrl_SetTextData(tWin,(int)(CO2_Parameters.insCO2));
				}
			}
			else
			{
				if(CO2_Parameters.insCO2 > 113/*100*/)
				{
					TextExCtrl_SetTextData(tWin,5000);
				}
				else
				{
					TextExCtrl_SetTextData(tWin,(int)(CO2_Parameters.insCO2 * 10));
				}
			}
		}
	}
	else 
	{
		TextExCtrl_SetTextData(tWin,5000);//djq add 20140221
	}
	Msg.hWin = tWin;
	Msg.MsgId = WM_NOTIFICATION_VALUE_CHANGED;
	WM_SelectWindow(Msg.hWin);
	WM_SendMessage(Msg.hWin,&Msg);
	//endof   20130325
#endif 
    //----------------------------------------------------------------------------------------------
    tWin = WM_GetDialogItem(hDialog,GUI_ID_TEXT3);
    if((Masimo_EtCO2_Config_On||MasimoISA_EtCO2_Config_On)
        &&(EtCO2ModuleOff)&&(AA_Module_Flag.PhaseInTypeModuleExist == Masimo_EtCO2_zhuliu)
        ||(AA_Module_Flag.PhaseInTypeModuleExist == MasimoISA_EtCO2_pangliuO2)
        ||(AA_Module_Flag.PhaseInTypeModuleExist == MasimoISA_EtCO2_pangliu))         //gyh add 20230725
    {
        TextExCtrl_SetTextData(tWin,AA_all_data.AA_detail.AA_N2O.FI);
    }
    else
    {
        TextExCtrl_SetTextData(tWin,5000);
    }
    /* if((AA_Module_Flag.EtCO2ModuleExist)&&(EtCO2ModuleOff))//sun add 20140619
    {
         TextExCtrl_SetTextData(tWin,AA_all_data.AA_detail.AA_N2O.FI);
    }
    else
    {
        TextExCtrl_SetTextData(tWin,5000);
    }*/
    Msg.hWin = tWin;
    Msg.MsgId = WM_NOTIFICATION_VALUE_CHANGED;
    WM_SelectWindow(Msg.hWin);
    WM_SendMessage(Msg.hWin,&Msg);
    //
    tWin = WM_GetDialogItem(hDialog,GUI_ID_TEXT4);
    if((Masimo_EtCO2_Config_On||MasimoISA_EtCO2_Config_On)
        &&(EtCO2ModuleOff)&&(AA_Module_Flag.PhaseInTypeModuleExist == Masimo_EtCO2_zhuliu)
        ||(AA_Module_Flag.PhaseInTypeModuleExist == MasimoISA_EtCO2_pangliuO2)
        ||(AA_Module_Flag.PhaseInTypeModuleExist == MasimoISA_EtCO2_pangliu))         //gyh add 20230725
    {
        TextExCtrl_SetTextData(tWin,AA_all_data.AA_detail.AA_N2O.ET);
    }
    else
    {
        TextExCtrl_SetTextData(tWin,5000);
    }
   /* if((AA_Module_Flag.EtCO2ModuleExist)&&(EtCO2ModuleOff))//sun add 20140619
    {
        TextExCtrl_SetTextData(tWin,AA_all_data.AA_detail.AA_N2O.ET);
    }
    else
    {
        TextExCtrl_SetTextData(tWin,5000);
    }*/
    //TextExCtrl_SetTextData(tWin,pAll_Data->slow.ExpVal[IM_GAS_N2O]);
    Msg.hWin = tWin;
    Msg.MsgId = WM_NOTIFICATION_VALUE_CHANGED;
    WM_SelectWindow(Msg.hWin);
    WM_SendMessage(Msg.hWin,&Msg);
    //----------------------------------------------------------------------------------------------
    tWin = WM_GetDialogItem(hDialog,GUI_ID_TEXT5);
    tWin1 = WM_GetDialogItem(hDialog,GUI_ID_TEXT6);
	if(LastAA1Id != AA_all_data.AA_detail.AA_AA1.ID)
    {
		LastAA1Id = AA_all_data.AA_detail.AA_AA1.ID;
        switch(LastAA1Id)
        {
        case IM_NO_AGT:
            j = 0;
            break;
        case IM_AGT_HAL:
            j = 1;
            break;
        case IM_AGT_ENF:
            j = 2;
            break;
        case IM_AGT_ISO:
            j = 3;
            break;
        case IM_AGT_SEV:
            j = 4;
            break;
        case IM_AGT_DES:
            j = 5;
            break;
        }
		TextExCtrl_SetTextInfo(tWin,0, Sys_Lib_GetDispStr(AAName[j]), "%");
		switch(LastAA1Id)
        {
        case IM_NO_AGT:
            j = 6;
            break;
        case IM_AGT_HAL:
            j = 7;
            break;
        case IM_AGT_ENF:
            j = 8;
            break;
        case IM_AGT_ISO:
            j = 9;
            break;
        case IM_AGT_SEV:
            j = 10;
            break;
        case IM_AGT_DES:
            j = 11;
            break;
        }
		TextExCtrl_SetTextInfo(tWin1,0, Sys_Lib_GetDispStr(AAName[j]), "%");
    }
     if(AA_all_data.AA_detail.AA_AA1.ID)
//	if(AAType/*AA_all_data.AA_detail.AA_AA1.ID*/ 
//		&&((AA_all_data.AA_state.dev_info.AA_name == PhaseIn) 
//		||((AA_all_data.AA_state.dev_info.AA_name == Drager)&&(AA_Module_Flag.INIT_AA_MODULE_FLAG != 4)))
//		)
    {
        if((AA_Module_Flag.EtCO2ModuleExist)&&(EtCO2ModuleOff))
        {
             if(AA_all_data.AA_detail.AA_AA1.FI > AA_Max*10)//add zkq 20161101
             {
                 TextExCtrl_SetTextData(tWin,5000);
             }
             else
             {
                 TextExCtrl_SetTextData(tWin, AA_all_data.AA_detail.AA_AA1.FI);
             }
//			if(Drager_EtCO2_Config_On)//如果现在是drager
//			{
//				if((AA_Module_Flag.INIT_AA_MODULE_FLAG & 0x02) == 0x02)//选择了麻气
//				{
//					TextExCtrl_SetTextData(tWin,AA_all_data.AA_detail.AA_AA1.FI);
//				}
//				else
//				{
//					TextExCtrl_SetTextData(tWin,0xFFFF);//未选择显示--
//				}
//			}
//			else
//			{
//				TextExCtrl_SetTextData(tWin,AA_all_data.AA_detail.AA_AA1.FI);
//			}
        }
        else
        {
            TextExCtrl_SetTextData(tWin,5000);
        }
    }
    else
    {
        TextExCtrl_SetTextData(tWin,5000);
    }
    Msg.hWin = tWin;
    Msg.MsgId = WM_NOTIFICATION_VALUE_CHANGED;
    WM_SelectWindow(Msg.hWin);
    WM_SendMessage(Msg.hWin,&Msg);
    
     if(AA_all_data.AA_detail.AA_AA1.ID)
//	if(AAType/*AA_all_data.AA_detail.AA_AA1.ID*/ 
//		&&((AA_all_data.AA_state.dev_info.AA_name == PhaseIn) 
//		||((AA_all_data.AA_state.dev_info.AA_name == Drager)&&(AA_Module_Flag.INIT_AA_MODULE_FLAG != 4)))
//		)
    {
        if((AA_Module_Flag.EtCO2ModuleExist)&&(EtCO2ModuleOff))
        {
             if(AA_all_data.AA_detail.AA_AA1.ET > AA_Max*10)//add zkq 20161101
             {
                 TextExCtrl_SetTextData(tWin1,5000);
             }
             else
             {
                 TextExCtrl_SetTextData(tWin1,AA_all_data.AA_detail.AA_AA1.ET);
             }
//			if(Drager_EtCO2_Config_On)//如果现在是drager
//			{
//				if((AA_Module_Flag.INIT_AA_MODULE_FLAG & 0x02) == 0x02)//选择了麻气
//				{
//					TextExCtrl_SetTextData(tWin1,AA_all_data.AA_detail.AA_AA1.ET);
//				}
//				else
//				{
//					TextExCtrl_SetTextData(tWin1,0xFFFF);//未选择显示--
//				}
//			}
//			else
//			{
//				TextExCtrl_SetTextData(tWin1,AA_all_data.AA_detail.AA_AA1.ET);
//			}
        }
        else
        {
            TextExCtrl_SetTextData(tWin1,5000);
        }
    }
    else
    {
        TextExCtrl_SetTextData(tWin1,5000);
    }
    Msg.hWin = tWin1;
    Msg.MsgId = WM_NOTIFICATION_VALUE_CHANGED;
    WM_SelectWindow(Msg.hWin);
    WM_SendMessage(Msg.hWin,&Msg);
    //----------------------------------------------------------------------------------------------
    tWin = WM_GetDialogItem(hDialog,GUI_ID_TEXT7);
    tWin1 = WM_GetDialogItem(hDialog,GUI_ID_TEXT8);
	if(LastAA2Id != AA_all_data.AA_detail.AA_AA2.ID)
    {
        LastAA2Id = AA_all_data.AA_detail.AA_AA2.ID;
        switch(LastAA2Id)
        {
        case IM_NO_AGT:
            AA2_Max = 10;
            j = 12;
            break;
        case IM_AGT_HAL:
            AA2_Max = 8;
            j = 1;
            break;
        case IM_AGT_ENF:
            AA2_Max = 8;
            j = 2;
            break;
        case IM_AGT_ISO:
            AA2_Max = 8;
            j = 3;
            break;
        case IM_AGT_SEV:
            AA2_Max = 10;
            j = 4;
            break;
        case IM_AGT_DES:
            AA2_Max = 22;
            j = 5;
            break;
        }
		TextExCtrl_SetTextInfo(tWin,0, Sys_Lib_GetDispStr(AAName[j]), "%");
		switch(AA_all_data.AA_detail.AA_AA2.ID)
        {
        case IM_NO_AGT:
            j = 13;
            break;
        case IM_AGT_HAL:
            j = 7;
            break;
        case IM_AGT_ENF:
            j = 8;
            break;
        case IM_AGT_ISO:
            j = 9;
            break;
        case IM_AGT_SEV:
            j = 10;
            break;
        case IM_AGT_DES:
            j = 11;
            break;
        }
		TextExCtrl_SetTextInfo(tWin1,0, Sys_Lib_GetDispStr(AAName[j]), "%");
    }

     if(AA_all_data.AA_detail.AA_AA2.ID)
//	if(AA_all_data.AA_detail.AA_AA2.ID 
//		&&((AA_all_data.AA_state.dev_info.AA_name == PhaseIn) 
//		||((AA_all_data.AA_state.dev_info.AA_name == Drager)&&(AA_Module_Flag.INIT_AA_MODULE_FLAG != 4)))
//		)
    {
        if((AA_Module_Flag.EtCO2ModuleExist)&&(EtCO2ModuleOff))
        {
            if(AA_all_data.AA_detail.AA_AA2.FI > AA2_Max*10)//add zkq 20161101
            {
                TextExCtrl_SetTextData(tWin,5000);
            }
            else
            {
                TextExCtrl_SetTextData(tWin,AA_all_data.AA_detail.AA_AA2.FI);
            }
        }
        else
        {
            TextExCtrl_SetTextData(tWin,5000);
        }
    }
    else
    {
        TextExCtrl_SetTextData(tWin,5000);
    }
    Msg.hWin = tWin;
    Msg.MsgId = WM_NOTIFICATION_VALUE_CHANGED;
    WM_SelectWindow(Msg.hWin);
    WM_SendMessage(Msg.hWin,&Msg);
    //
     if(AA_all_data.AA_detail.AA_AA2.ID)
//	if(AA_all_data.AA_detail.AA_AA2.ID 
//		&&((AA_all_data.AA_state.dev_info.AA_name == PhaseIn) 
//		||((AA_all_data.AA_state.dev_info.AA_name == Drager)&&(AA_Module_Flag.INIT_AA_MODULE_FLAG != 4)))
//		)
    {
        if((AA_Module_Flag.EtCO2ModuleExist)&&(EtCO2ModuleOff))
        {
            if(AA_all_data.AA_detail.AA_AA2.ET > AA2_Max*10)
            {
                TextExCtrl_SetTextData(tWin1,5000);
            }
            else
            {
                TextExCtrl_SetTextData(tWin1,AA_all_data.AA_detail.AA_AA2.ET);
            }
        }
        else
        {
            TextExCtrl_SetTextData(tWin1,5000);
        }
    }
    else
    {
        TextExCtrl_SetTextData(tWin1,5000);
    }
    Msg.hWin = tWin1;
    Msg.MsgId = WM_NOTIFICATION_VALUE_CHANGED;
    WM_SelectWindow(Msg.hWin);
    WM_SendMessage(Msg.hWin,&Msg);
}

void CheckCurveRangUpdate(int VentFlag)
{
	//呼吸周期计数器
	static  Int8U   CurveRangCount = 0; 
	if (StandbyState==0&&WM_IsWindow(Menu_hWin.HomeWin.ParentWin))
	{
		if(VentFlag)
		{
			CurveRangCount++;
		}
		if (CurveRangCount	>	0)
		{
			CurveRangCount = 0;
			TracingCurve_SetRangUpdateFlag(Menu_hWin.HomeWin.Tobj[0],1);
			TracingCurve_SetRangUpdateFlag(Menu_hWin.HomeWin.Tobj[1],1);
			TracingCurve_SetRangUpdateFlag(Menu_hWin.HomeWin.Tobj[2],1);
			TracingCurve_SetRangUpdateFlag(Menu_hWin.HomeWin.Tobj[3],1);
		}
	}
}
/***************************************************************************************************
* @brief 波形数据更新
* @para   
* @retval 
*/
void CurveDataUpdate(CURVE_HWIN *hWin,char flag,HOME_MENU_HWIN * tWin)
{
    static float LastPressure = 0; //float //cpp md 20130325
    static float LastFlow = 0;
    static float LastVolume = 0;
#ifdef AA_WAVE_SWITCH
    static unsigned int N20WaveData = 0;
    static unsigned int Ax2WaveData = 0;
    static unsigned int Ax1WaveData = 0;
#endif
    float CurrentPressure,CurrentFlow,CurrentVolume; //float //cpp md 20130325
    static int i = 0;//  20130325
    //CurrentPressure = (70*RealtimePara.Pressure + 30*LastPressure)/100;  //sun del 20140526
    CurrentPressure = (70*RealtimePara.Pressure/10 + 30*LastPressure)/100;  //sun add 20140526
    CurrentFlow = (70*RealtimePara.Flow/50 + 30*LastFlow)/100;
    CurrentVolume = (70*RealtimePara.Volume + 30*LastVolume)/100; //cpp undel 20130325
#ifdef WAVE_PAW_ADD_ALARMHIGH
    if(RealtimePara.Pressure >= AlarmSettingPara.PressureHighValue*10)
    {
        CurrentPressure = (70*(AlarmSettingPara.PressureHighValue - 1) + 30*LastPressure)/100;
    }
#endif
	//if(SpO2_Choose == 2)
	//{

	//}

    LastPressure = CurrentPressure;
    LastFlow = CurrentFlow;
    LastVolume = CurrentVolume;
    
    
    switch(hWin->WaveForms[0])
    {
    case WAVEFORM_PRESSURE:
        TracingCurve_SetADDData(tWin->Tobj[0],flag,Curve_Color/*GUI_YELLOW*/,CurrentPressure, 0,WAVEFORM_PRESSURE,0); //CurrentPressure/10 //cpp md 20130325
        PawGauge_SetData(Menu_hWin.HomeWin.Left_Robj[5], CurrentPressure);
        break;
    case WAVEFORM_FLOW:
        TracingCurve_SetADDData(tWin->Tobj[0],flag,GUI_GREEN,CurrentFlow, 0,WAVEFORM_FLOW,0);
        break;
    case WAVEFORM_VOLUME:
        TracingCurve_SetADDData(tWin->Tobj[0],flag,GUI_DARKCYAN,CurrentVolume, 0,WAVEFORM_VOLUME,0);//(float)RealtimePara.Volume  //cpp md 20130325
        break;
#ifdef SPO2_SWITCH
    case WAVEFORM_SPO2:
		if(SpO2_Choose == 1)
		{
			TracingCurve_SetADDData(tWin->Tobj[0],flag,0xFFC81E,SpO2_Parameters.WaveData,0, WAVEFORM_SPO2,0);
		}
		else//masimo
		{
			if(SPO2CurveReceOK)
			{
				if(SPO2WaveSIQSmartOn > 0)
				{
					SPO2WaveSIQSmartOn >>= 1;
					SPO2WaveSIQSmartOn -= 0x5F;
				}
				else
				{
					SPO2WaveSIQSmartOn = -0x5F;
				} 
				SPO2CurveReceOK = 0;
			}
			//((signed char)(SPO2WaveIR)*(-100))/128.0	SPO2WaveSIQSmartOn
			TracingCurve_SetADDData(tWin->Tobj[0],flag,0xFFC81E,((signed char)(SPO2WaveIR)*(-100))/128.0, SPO2WaveSIQSmartOn,WAVEFORM_SPO2,1);// 
			if(hWin->WaveForms[0] != WAVEFORM_SPO2)
			{
				SPO2WaveSIQSmartOn = -0x5F;
			}
		}
        break;
#endif

    case WAVEFORM_CO2:
        TracingCurve_SetADDData(tWin->Tobj[0],flag,0xabaa0b,CO2_Parameters.WaveData, 0,WAVEFORM_CO2,0);
        break;
	case LOOP_V_CO2:
		//VF_Flag = 0;
		if (flag)
		{
			RealtimePara.Volume = 0;
		}
		TracingCurve_SetADD2Data(tWin->Tobj[0],0xabaa0b,(float)RealtimePara.Volume,CO2_Parameters.WaveData,flag,LOOP_V_CO2);
		break;

#ifdef AA_WAVE_SWITCH
    case WAVEFORM_N2O:
        Irma_getWaveData(IM_GAS_N2O,&N20WaveData);
        TracingCurve_SetADDData(tWin->Tobj[0],flag,N2OColor,(float)N20WaveData/100.0,0,WAVEFORM_N2O,0);
        break;
    case WAVEFORM_PAA:
        if( !strcmp(PAA_Update0,AA_all_data.AA_detail.AA1_Type_Text) )
        {
            //strcpy(PAA_Update0,pAll_Data->slow.GenVal.ax1_type);
        }
        else
        {
            strcpy(PAA_Update0,AA_all_data.AA_detail.AA1_Type_Text);
            WM_InvalidateWindow(tWin->Tobj[0]);
        }
        
        Irma_getWaveData(IM_GAS_AX1,&Ax1WaveData);
        TracingCurve_SetADDData(tWin->Tobj[0],flag,AA_all_data.AA_detail.AA1_Color,(float)Ax1WaveData/100.0,0,WAVEFORM_PAA,0);
        break;
    case WAVEFORM_SAA:
        //add zkq 20161028 AA字体切换时更新
        if( !strcmp(SAA_Update0,AA_all_data.AA_detail.AA2_Type_Text) )
        {
            //字符串一样 不需要更新
            //strcpy(SAA_Update0,pAll_Data->slow.GenVal.ax2_type);
        }
        else
        {
            //字符串不一样 需要更新
            strcpy(SAA_Update0,AA_all_data.AA_detail.AA2_Type_Text);
            WM_InvalidateWindow(tWin->Tobj[0]);
        }
        
        Irma_getWaveData(IM_GAS_AX2,&Ax2WaveData);
        TracingCurve_SetADDData(tWin->Tobj[0],flag,AA_all_data.AA_detail.AA2_Color,(float)Ax2WaveData/100.0,0,WAVEFORM_SAA,0);
        break;
#endif
    }

    switch(hWin->WaveForms[1])
    {
    case WAVEFORM_PRESSURE:
        TracingCurve_SetADDData(tWin->Tobj[1],flag,Curve_Color/*GUI_YELLOW*/,CurrentPressure, 0,WAVEFORM_PRESSURE,0); //CurrentPressure/10 //cpp md 20130418
        break;
    case WAVEFORM_FLOW:
        TracingCurve_SetADDData(tWin->Tobj[1],flag,GUI_GREEN,CurrentFlow, 0,WAVEFORM_FLOW, 0);
        break;
    case WAVEFORM_VOLUME:
        TracingCurve_SetADDData(tWin->Tobj[1],flag,GUI_DARKCYAN,CurrentVolume, 0, WAVEFORM_VOLUME, 0);//(float)RealtimePara.Volume  //cpp md 20130418
        break;
#ifdef SPO2_SWITCH
    case WAVEFORM_SPO2:
		if(SpO2_Choose == 1)
		{
			TracingCurve_SetADDData(tWin->Tobj[1],flag,0xFFC81E,SpO2_Parameters.WaveData, 0,WAVEFORM_SPO2, 0);
		}
		else//masimo
		{
			if(SPO2CurveReceOK)
			{
				if(SPO2WaveSIQSmartOn > 0)
				{
					SPO2WaveSIQSmartOn >>= 1;
					SPO2WaveSIQSmartOn -= 0x5F;
				}
				else
				{
					SPO2WaveSIQSmartOn = -0x5F;
				} 
				SPO2CurveReceOK = 0;
			}
			//((signed char)(SPO2WaveIR)*(-100))/128.0	SPO2WaveSIQSmartOn
            TracingCurve_SetADDData(tWin->Tobj[1],flag,0xFFC81E,((signed char)(SPO2WaveIR)*(-100))/128.0, SPO2WaveSIQSmartOn,WAVEFORM_SPO2,1);
			if(hWin->WaveForms[1] != WAVEFORM_SPO2)
			{
				SPO2WaveSIQSmartOn = -0x5F;
			}
		}
        break;
#endif
    case WAVEFORM_CO2:
        TracingCurve_SetADDData(tWin->Tobj[1],flag,0xabaa0b,CO2_Parameters.WaveData,0, WAVEFORM_CO2,0);
        break;
	case LOOP_V_CO2:
		//VF_Flag = 0;
		if (flag)
		{
			RealtimePara.Volume = 0;
		}
		TracingCurve_SetADD2Data(tWin->Tobj[1],0xabaa0b,(float)RealtimePara.Volume,CO2_Parameters.WaveData,flag,LOOP_V_CO2);
		break;
    case LOOP_PV:
        TracingCurve_SetADD2Data(tWin->Tobj[1],GUI_YAN_BLUE,CurrentPressure,CurrentVolume,flag, LOOP_PV);//CurrentPressure/10 , (float)RealtimePara.Volume //cpp md 20130418
        break;
    case LOOP_VF:
        TracingCurve_SetADD2Data(tWin->Tobj[1],0x4080FF,CurrentVolume,CurrentFlow,flag, LOOP_VF);//(float)RealtimePara.Volume //cpp md 2030418
        break;
	case LOOP_PF:
		TracingCurve_SetADD2Data(tWin->Tobj[1],0x4080FF,CurrentPressure,CurrentFlow,flag, LOOP_PF);
		break;

#ifdef AA_WAVE_SWITCH
    case WAVEFORM_N2O:
        Irma_getWaveData(IM_GAS_N2O,&N20WaveData);
        TracingCurve_SetADDData(tWin->Tobj[1],flag,N2OColor,(float)N20WaveData/100.0,0,WAVEFORM_N2O,0);
        break;
    case WAVEFORM_PAA:
        if( !strcmp(PAA_Update1,AA_all_data.AA_detail.AA1_Type_Text) )
        {
            //strcpy(PAA_Update1,pAll_Data->slow.GenVal.ax1_type);
        }
        else
        {
            strcpy(PAA_Update1,AA_all_data.AA_detail.AA1_Type_Text);
            WM_InvalidateWindow(tWin->Tobj[1]);
      
        }
        Irma_getWaveData(IM_GAS_AX1,&Ax1WaveData);
        TracingCurve_SetADDData(tWin->Tobj[1],flag,AA_all_data.AA_detail.AA1_Color,(float)Ax1WaveData/100.0,0,WAVEFORM_PAA,0);
        break;
    case WAVEFORM_SAA:

        if( !strcmp(SAA_Update1,AA_all_data.AA_detail.AA2_Type_Text) )
        {
            //字符串一样 不需要更新
            //strcpy(SAA_Update1,pAll_Data->slow.GenVal.ax2_type);
        }
        else
        {
            //字符串不一样 需要更新
            strcpy(SAA_Update1,AA_all_data.AA_detail.AA2_Type_Text);
            WM_InvalidateWindow(tWin->Tobj[1]);
  
        }
        Irma_getWaveData(IM_GAS_AX2,&Ax2WaveData);
        TracingCurve_SetADDData(tWin->Tobj[1],flag,AA_all_data.AA_detail.AA2_Color,(float)Ax2WaveData/100.0,0,WAVEFORM_SAA,0);
        break;
#endif
    }
    
    switch(hWin->WaveForms[2])
    {
    case WAVEFORM_PRESSURE:
	//20221118 lyy modify CurrentPressure/10 ->CurrentPressure
        TracingCurve_SetADDData(tWin->Tobj[2],flag,Curve_Color/*GUI_YELLOW*/,CurrentPressure, 0,WAVEFORM_PRESSURE,0);
        break;
    case WAVEFORM_FLOW:
        TracingCurve_SetADDData(tWin->Tobj[2],flag,GUI_GREEN,CurrentFlow, 0,WAVEFORM_FLOW, 0);
        break;
    case WAVEFORM_VOLUME:
        TracingCurve_SetADDData(tWin->Tobj[2],flag,GUI_DARKCYAN,(float)RealtimePara.Volume, 0,WAVEFORM_VOLUME, 0);
        break;
#ifdef SPO2_SWITCH
    case WAVEFORM_SPO2:
		if(SpO2_Choose == 1)
		{
			TracingCurve_SetADDData(tWin->Tobj[2],flag,0xFFC81E,SpO2_Parameters.WaveData, 0,WAVEFORM_SPO2,0);
		}
		else//masimo
		{
			if(SPO2CurveReceOK)
			{
				if(SPO2WaveSIQSmartOn > 0)
				{
					SPO2WaveSIQSmartOn >>= 1;
					SPO2WaveSIQSmartOn -= 0x5F;
				}
				else
				{
					SPO2WaveSIQSmartOn = -0x5F;
				} 
				SPO2CurveReceOK = 0;
			}
			//((signed char)(SPO2WaveIR)*(-100))/128.0	SPO2WaveSIQSmartOn
            TracingCurve_SetADDData(tWin->Tobj[2],flag,0xFFC81E,((signed char)(SPO2WaveIR)*(-100))/128.0, SPO2WaveSIQSmartOn,WAVEFORM_SPO2,1);
			if(hWin->WaveForms[2] != WAVEFORM_SPO2)
			{
				SPO2WaveSIQSmartOn = -0x5F;
			}
		}
        break;
#endif
    case WAVEFORM_CO2:
        TracingCurve_SetADDData(tWin->Tobj[2],flag,0xabaa0b,CO2_Parameters.WaveData, 0,WAVEFORM_CO2,0);
        break;
	case LOOP_V_CO2:
		if (flag)
		{
			RealtimePara.Volume = 0;
		}
		TracingCurve_SetADD2Data(tWin->Tobj[2],0xabaa0b,(float)RealtimePara.Volume,CO2_Parameters.WaveData,flag,LOOP_V_CO2);
		break;
    case LOOP_PV:
        if(flag)
        {
            RealtimePara.Volume = 0;
        }
        TracingCurve_SetADD2Data(tWin->Tobj[2],GUI_YAN_BLUE,CurrentPressure/10,(float)RealtimePara.Volume,flag, LOOP_PV);
        break;
    case LOOP_VF:
        if(flag)
        {
            RealtimePara.Volume = 0;
            CurrentFlow = 0;
        }
        TracingCurve_SetADD2Data(tWin->Tobj[2],0x4080FF,(float)RealtimePara.Volume,CurrentFlow,flag, LOOP_VF);
        break;

	case LOOP_PF:
		if(flag)
		{
			CurrentFlow = 0;
		}
		TracingCurve_SetADD2Data(tWin->Tobj[2],0x4080FF,CurrentPressure/10,CurrentFlow,flag, LOOP_PF);
		break;

#ifdef AA_WAVE_SWITCH
    case WAVEFORM_N2O:
        Irma_getWaveData(IM_GAS_N2O,&N20WaveData);
        TracingCurve_SetADDData(tWin->Tobj[2],flag,N2OColor,(float)N20WaveData/100.0,0,WAVEFORM_N2O,0);
        break;
    case WAVEFORM_PAA:
        if( !strcmp(PAA_Update2,AA_all_data.AA_detail.AA1_Type_Text) )
        {
            //strcpy(PAA_Update2,pAll_Data->slow.GenVal.ax1_type);
        }
        else
        {
            strcpy(PAA_Update2,AA_all_data.AA_detail.AA1_Type_Text);
            WM_InvalidateWindow(tWin->Tobj[2]);
   
        }
        Irma_getWaveData(IM_GAS_AX1,&Ax1WaveData);
        TracingCurve_SetADDData(tWin->Tobj[2],flag,AA_all_data.AA_detail.AA1_Color,(float)Ax1WaveData/100.0,0,WAVEFORM_PAA,0);
        break;
    case WAVEFORM_SAA:
        if( !strcmp(SAA_Update2,AA_all_data.AA_detail.AA2_Type_Text) )
        {
            //字符串一样 不需要更新
            //strcpy(SAA_Update2,pAll_Data->slow.GenVal.ax2_type);
        }
        else
        {
            //字符串不一样 需要更新
            strcpy(SAA_Update2,AA_all_data.AA_detail.AA2_Type_Text);
            WM_InvalidateWindow(tWin->Tobj[2]);
  
        }
        Irma_getWaveData(IM_GAS_AX2,&Ax2WaveData);
        TracingCurve_SetADDData(tWin->Tobj[2],flag,AA_all_data.AA_detail.AA2_Color,(float)Ax2WaveData/100.0,0,WAVEFORM_SAA,0);
        break;
#endif
    }

    switch(hWin->WaveForms[3]) 
    {
    case LOOP_PV:
        if(flag)
        {
            RealtimePara.Volume = 0;
        }
        TracingCurve_SetADD2Data(tWin->Tobj[3],GUI_YAN_BLUE,CurrentPressure/10,(float)RealtimePara.Volume,flag, LOOP_PV);
        break;
    case LOOP_VF:
        if(flag)
        {
            RealtimePara.Volume = 0;
            CurrentFlow = 0;
        }
        TracingCurve_SetADD2Data(tWin->Tobj[3],0x4080FF,(float)RealtimePara.Volume,CurrentFlow,flag, LOOP_VF);
        break;   
    case LOOP_PF:
        if(flag)
        {
            CurrentFlow = 0;
        }
        TracingCurve_SetADD2Data(tWin->Tobj[3],0x4080FF,CurrentPressure/10,CurrentFlow,flag, LOOP_PF);
        break;
    }

}

/***************************************************************************************************
* @brief  设置默认报警限值
* @para   
* @retval 
*/
void SettingDefaultAlarmPara(ALARM_SET_PARA * Parameter)
{
    InitPatientInfo(Patient_GetInfo());
    Parameter->AlarmVolumeValue = 5;
    VolumeChannelSelect(Parameter->AlarmVolumeValue);
    Parameter->ApneaTimerValue = 15;
    Parameter->FiO2HighValue = 101;//djq modify 20161127 100→101默认Off
    Parameter->FiO2LowValue = 20;  //djq modify 20161127 35→ 20默认
    Parameter->RateHighValue = 100;
    Parameter->RateLowValue = 5;
    Parameter->MVHighValue = 40;
    Parameter->MVLowValue = 0;
    Parameter->PressureHighValue = 40;
    Parameter->PressureLowValue = 5;
    Parameter->VteHighValue = 1000;
    Parameter->VteLowValue = 200;
}

/***************************************************************************************************
* @brief  校验进度条显示
* @para   
* @retval 
*/
int _CalibrationProgressShow(int type,WM_HWIN hWin,int flag)
{
    static int timer = 0;
    static int timer1 = 0;
    static float timer_flag = 0;
    WM_HWIN hItem,hItem1;
    WM_MESSAGE Msg;
    int r = 0,flg = 0;
    int Err = 0;
    static unsigned char AA_ZeroFlag = 1, O2_CaliFlag = 1;
    hItem1 = WM_GetDialogItem(hWin, GUI_ID_MULTIPAGE0);
    hWin = MULTIPAGE_GetWindow(hItem1,3);
    hItem=WM_GetDialogItem(hWin, GUI_ID_PROGBAR0);
    switch(type)
    {
    case PRESSURE_CALIBRATION:
    case FLOW_CALIBRATION:
        if(timer<100)
        {
            if(!flag)
            {
                timer +=5;
                if(timer==100)
                {
                    Err = -1;
                    r = 1;
                }
            }
            else
            {
                timer = 100;
                Err = 1;
                r = 1;
            }
        }
        break;
    case OXY_CALIBRATION:
		//zkq add 20171106 MASIMO ISA校验
      if((MasimoISA_EtCO2_Config_On==2 )
         && (AA_Module_Flag.EtCO2ModuleExist == 2)
           &&(AA_Module_Flag.PhaseInTypeModuleExist==2))//Masimo ISA O2
      {
        if(timer<100)
        {
          timer1++;
          if(timer1>=OXY_CALI_TIME)
          {
            timer ++;
            timer1 = 0;
          }
          //
          if((timer == 0)&&(timer1 < 2))//发送流量持续2min
          {
          }
          else if((timer == 20)&&(!timer1))//2min流量结束，发送开始校验命令
          {
          }
          else if(timer > 20 && timer <= 100)
          {
            //发送校验命令
            if(O2_CaliFlag == 1)
            {
              if( AA_all_data.AA_state.AA_ModuleState.zero_state != disable \
                && AA_all_data.AA_state.AA_ModuleState.zero_state != zero_zeroing \
                  && AA_all_data.AA_state.AA_ModuleState.span_state != SPAN_InProg)
              {
                O2_CaliFlag = 0;
                if(oxyChooseFlag == 1)//氧气校验
                {
                  AA_SetSpanCalType(CAL_O2,100);
                }
                else//空气校验
                {
                  AA_SetSpanCal21Type();
                }
              }
			  spaning_state = 0;
			  zeroing_state = 0;
            }
//             else
            {
              //检查校验结果
              if(timer > 30 && timer <= 100)//直接成功
              {
                //校准错误 && 不在校准中==》失败
                if ((AA_all_data.AA_state.module_err_message[1] &0x02)
                    && AA_all_data.AA_state.AA_ModuleState.span_state != SPAN_InProg)
                {
                  timer = 100;
                  Err = -1;
                  r = 1;
                  O2_CaliFlag = 1;
                  SendOperationMessage(SET_AA_PUMPOFF,USART2);//关泵
                }
                //直接失败
                else if (timer > 90)
                {
                  timer = 100;
                  Err = -1;
                  r = 1;
                  O2_CaliFlag = 1;
                  SendOperationMessage(SET_AA_PUMPOFF,USART2);//关泵
                }
                //较零校准状态可用 && 不在较零中 && 不在校准中 &&（有过较零中||有过校准中）==成功
                else if(	AA_all_data.AA_state.AA_ModuleState.zero_state != disable \
                  && AA_all_data.AA_state.AA_ModuleState.zero_state != zero_zeroing \
                    && AA_all_data.AA_state.AA_ModuleState.span_state != SPAN_InProg)
                {
                  if (oxyChooseFlag == 1)
                  {
                    if (spaning_state)
                    {
                      timer = 100;
                      Err = 1;
                      r = 1;
                      O2_CaliFlag = 1;
                      if (oxyChooseFlag == 1)
                      {
                        SendOperationMessage(SET_AA_PUMPOFF,USART2);//关泵
                      }
                    }
                  }
                  else
                  {
                    if (zeroing_state)
                    {
                      timer = 100;
                      Err = 1;
                      r = 1;
                      O2_CaliFlag = 1;
                      if (oxyChooseFlag == 1)
                      {
                        SendOperationMessage(SET_AA_PUMPOFF,USART2);//关泵
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
      else
      {
        if(timer<100)
        {
          if(!flag)
          {
            timer1++;
            if(timer1>=OXY_CALI_TIME)
            {
              timer ++;
              timer1 = 0;
            }
            if(timer==100)
            {
              Err = -1;
              r = 1;
            }
          }
          else
          {
            timer = 100;
            Err = 1;
            r = 1;
          }
        }
      }
      if(MasimoISA_EtCO2_Config_On==2 
         && (AA_Module_Flag.EtCO2ModuleExist == 2)
           &&(AA_Module_Flag.PhaseInTypeModuleExist==2))
      {
        if (oxyChooseFlag == 0 && Err == 1)
        {
          oxyChooseFlag = 1;
          PlanMenuType = OXYGEN_SENSOR_CAL_MENU;
          Task |=MEAU_UPDATE;
        }
      }
      break;
    case PHASEIN_ZERO_CALIBRATION://sun add 20140707
      timer++;
        //zkq add 20171106 MASIMO ISA校验
        if(MasimoISA_EtCO2_Config_On && (AA_Module_Flag.EtCO2ModuleExist == 2)&&(AA_Module_Flag.PhaseInTypeModuleExist))//Masimo ISA
        {
            if( AA_all_data.AA_state.AA_ModuleState.zero_state != disable
				&&AA_all_data.AA_state.AA_ModuleState.zero_state != zero_zeroing 
				&& AA_ZeroFlag == 1)//可以校零
            {
                AA_ZeroFlag = 0;
                 SendOperationMessage(SET_AA_ZERO_INIT/*SET_AA_ZEROCAL*/,USART2);//校零
				 zeroing_state = 0;
            }
        }
        if(timer > 60 && timer < 100)
        {
			if( AA_all_data.AA_state.AA_ModuleState.zero_state != disable \
				&& AA_all_data.AA_state.AA_ModuleState.zero_state != zero_zeroing \
				&& AA_all_data.AA_state.AA_ModuleState.span_state != SPAN_InProg
				&& zeroing_state) //可以校零//不在校零过程中//不在量程校准过程中
            {
                timer = 100;
                if((MasimoISA_EtCO2_Config_On || Masimo_EtCO2_Config_On)  && (AA_Module_Flag.EtCO2ModuleExist == 2))
                {
                    AA_ZeroFlag = 1;
                    SendOperationMessage(SET_AA_PUMPOFF,USART2);//关泵
                }
                Err = 1;
                r = 1;
            }          
        }
        else if(timer == 100)
        {
			if((MasimoISA_EtCO2_Config_On || Masimo_EtCO2_Config_On) && (AA_Module_Flag.EtCO2ModuleExist == 2))
			{
                AA_ZeroFlag = 1;
				SendOperationMessage(SET_AA_PUMPOFF,USART2);//关泵
			}
			if( AA_all_data.AA_state.AA_ModuleState.zero_state != disable \
				&& AA_all_data.AA_state.AA_ModuleState.zero_state != zero_zeroing \
				&& AA_all_data.AA_state.AA_ModuleState.span_state != SPAN_InProg
				&& zeroing_state) //可以校零//不在校零过程中//不在量程校准过程中
			{
				Err = 1;
				r = 1;
			}
			else
			{
				Err = -1;
				r = 1;
			}
        }
        break;
    case INSP_VALVE_CALIBRATION_FACTORY:
    case EXP_VALVE_CALIBRATION:
    case EXP_FLOW_SENSOR_CALIBRATION:
        if(timer<100)
        {
            if(!flag)
            {
                timer1++;
                if(timer1>=INSP_VALVE_CALI_FACTORY_TIME)
                {
                    timer ++;
                    timer1 = 0;
                }
                if(timer==100)
                {
                    Err = -1;
                    r = 1;
                }
            }
            else
            {
                timer = 100;
                Err = 1;
                r = 1;
            }
        }
        break;
    }
    if(Err==-1)  //sun add 20151103
    {
		PROGBAR_SetText(hItem, Sys_Lib_GetDispStr(DP_CALFAIL));
    }
    else if(Err==1)//sun add 20151103
    {
		PROGBAR_SetText(hItem, Sys_Lib_GetDispStr(DP_CALSUCCESS));
    }
    PROGBAR_SetValue(hItem,timer);
    if(r)
    {
        //timer_flag = 0;
        timer = 0;
        timer1 = 0;
        switch(type)
        {
        case INSP_VALVE_CALIBRATION_FACTORY:
            hItem = WM_GetDialogItem(Menu_hWin.RecordWin, GUI_ID_BUTTON0);
			MULTIPAGE_SetPageChangeEnable(hItem1,0);//ylk add 20190705
            break;
        case EXP_FLOW_SENSOR_CALIBRATION:
            hItem = WM_GetDialogItem(Menu_hWin.RecordWin, GUI_ID_BUTTON1);
			MULTIPAGE_SetPageChangeEnable(hItem1,0);
            break;
        case EXP_VALVE_CALIBRATION:
            hItem = WM_GetDialogItem(Menu_hWin.RecordWin, GUI_ID_BUTTON2);
			MULTIPAGE_SetPageChangeEnable(hItem1,0);
            break;
        case PRESSURE_CALIBRATION:
            hItem = WM_GetDialogItem(hWin, GUI_ID_BUTTON0);
			MULTIPAGE_SetPageChangeEnable(hItem1,1);
            break;
        case FLOW_CALIBRATION:
            hItem = WM_GetDialogItem(hWin, GUI_ID_BUTTON1);
			MULTIPAGE_SetPageChangeEnable(hItem1,1);
            break;
        case OXY_CALIBRATION:
            hItem = WM_GetDialogItem(hWin, GUI_ID_BUTTON2);
			MULTIPAGE_SetPageChangeEnable(hItem1,1);
            break;
        case PHASEIN_ZERO_CALIBRATION://sun add 20151103
            hItem=WM_GetDialogItem(hWin, GUI_ID_BUTTON4);
            WM_ShowWindow(hItem);
            hItem = WM_GetDialogItem(hWin, GUI_ID_BUTTON5);
            WM_ShowWindow(hItem);
            MULTIPAGE_SetPageChangeEnable(hItem1,1);
            break;
        }
        BUTTON_SetPressed(hItem,0);
// 		MULTIPAGE_SetPageChangeEnable(hItem1,1);
// 		MULTIPAGE_SetPageChangeEnable(hItem1,0);//ylk md 20190422
    }
    return(r);
}
/***************************************************************************************************
* @brief  报警信息更新
* @para   
* @retval 
*/
void ListBoxInfoUpdate(WM_HMEM hDlg,ALARM_RECORD_LIST * list,unsigned char flag,unsigned char type)
{
    char *pText;
    char s[150]= {0};//zkq modify 20170504 增大数组防止死机
    int num;
    WM_MESSAGE Msg;
    WM_HMEM hWin;
    WM_HMEM hItem = WM_GetDialogItem(hDlg,GUI_ID_LISTBOX0);
    AlarmRecord.AlarmType = type;
    InvertData2Info(AlarmRecord,s);

    if(flag == INSERT_ITEM)
    {
        LISTBOX_DeleteItem(hItem,MAX_RECORD_NUM-1);
    }
//  LISTBOX_InsertString(hItem,s,0);//ylk del 20190628
	LISTBOX_InsertStringWithBitmap(hItem, s, 0, AlarmList_GetActive(&AlarmRecord)?1:0);

    LISTBOX_SetSel(hItem,0);
    hWin = WM_GetDialogItem(hDlg,GUI_ID_SCROLLBAR0); //GUI_ID_VSCROLL //cpp md 20130325
	//??? GUI_ID_VSCROLL  GUI_ID_SCROLLBAR0
    num = LISTBOX_GetNumItems(hItem);
    if((num%10)!=0)
    {
        num = num/10 + 1;
    }
    else
    {
        num /= 10;
    }
    SCROLLBAR_SetNumItems(hWin,num);
    SCROLLBAR_SetValue(hWin,0);
    sprintf(s,"%d/%d",1,list->Count);
    hItem = WM_GetDialogItem(hDlg,GUI_ID_TEXT3);
    TEXT_SetText(hItem,s);
}
/***************************************************************************************************
* @brief  呼吸参数限制关系   20130401
* @para   
* @retval 
*/
int ParameterLimitAnalysis(ButtonExCtrl_Handle hObj, int type)
{
	if(BreathMode_AfterACGO == ACGO_MODE || BreathMode_AfterMANUAL == MANUAL_MODE)
	{
		Breath_Setting_Tmp.BreathMode = BreathMode_BeforeConfirm;
		return (ParameterLimitAnalysis_Normal(hObj, type, &Breath_Setting_Tmp));
	}
	else
	{
		return (ParameterLimitAnalysis_Normal(hObj, type, &Breath_Setting_Tmp));
	}
}

int ParameterLimitAnalysis_Normal(ButtonExCtrl_Handle hObj,int type,BREATH_SET_PARA * Parameter)
{
    int r = 0,TiOnlyReadValue = 0;
    WM_HWIN hWin;
	//char text[20] = {0};
    Int64U lowFlow = 0,data = 0;
    ButtonExCtrl_Obj* pObj;
	Int64U FlowTi = 0;
    pObj = ButtonExCtrl_H2P(hObj);
    //hWin = WM_GetDialogItem(Menu_hWin.TextWin,GUI_ID_TEXT4);  //sun add 20140522
    if(type==GUI_KEY_RIGHT)//increase
    {
        switch(pObj->Widget.Id)
        {
        case GUI_ID_BUTTON0: //VT  Pinsp  
            if(Parameter->BreathMode==SIMV_VCV_MODE)//MonitorPara.PeepValue
            { //VT&TI
                if(pObj->Data1 < 100) //  20130523
				{
					pObj->BDInfo.Step = 5;
				}
				else
				{
					pObj->BDInfo.Step = 10;
				}                                    
                if(pObj->Data1 + pObj->BDInfo.Step > pObj->BDInfo.Max)  //sun add 20140522
                {				
					sprintf(contorlText,"%s",cbdialog32string[VT_1600]);
                }
                else if((pObj->Data1 + pObj->BDInfo.Step)*100/Parameter->SettingTiValue > FLOW_MAX)
                {
					strcpy(contorlText, cbdialog32string[FLOW_HIGH]);
                }
                else
                {
                    pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
		            Get_VentParaInfoString(MAX_FLOW, (float)(pObj->Data1*60.0)/(10*Parameter->SettingTiValue));// * (1 - Parameter->SettingTpValue/100)
                    r = 1;
                }
            }
            else if((Parameter->BreathMode==VCV_MODE) || (Parameter->BreathMode==SIGH_MODE)) //|| ((Parameter->BreathMode==PSV_MODE) && (Parameter->ApnoeaMode==VCV_MODE)) //  20130510              
            {	//VT&IE
                if(pObj->Data1 < 100) //  20130523
				{
					pObj->BDInfo.Step = 5;
				}
				else
				{
					pObj->BDInfo.Step = 10;
				}                                                                        
                if(pObj->Data1 + pObj->BDInfo.Step > pObj->BDInfo.Max)
                {
					sprintf(contorlText,"%s",cbdialog32string[VT_1600]);
                }
                else if(Parameter->SettingIEValue >= 6)
                {
					if((pObj->Data1 + pObj->BDInfo.Step)*(10+IE_RatioTable[Parameter->SettingIEValue])*Parameter->SettingRateValue*100 / (60*10*(100-Parameter->SettingTpValue)) > FLOW_MAX)
// 					if((pObj->Data1 + pObj->BDInfo.Step)*100/Parameter->SettingTiValue > FLOW_MAX)
                    {
                        strcpy(contorlText, cbdialog32string[FLOW_HIGH]);
                    }
                    else
                    {
                        pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
                        //Parameter->SettingTiValue = 10*60*100/((10+IE_RatioTable[Parameter->SettingIEValue])*Parameter->SettingRateValue);
                        //Get_VentParaInfoString(MAX_FLOW, (float)(pObj->Data1*60)/(10*Parameter->SettingTiValue * (1 - Parameter->SettingTpValue/100.0)));
                        Get_VentParaInfoString(MAX_FLOW, (float)(pObj->Data1*(10+IE_RatioTable[Parameter->SettingIEValue])*Parameter->SettingRateValue*6.0 /(float)(60.0*10*(100-Parameter->SettingTpValue))));
                        r = 1;
                    }
                }
                else
                {
                    if((pObj->Data1 + pObj->BDInfo.Step)*(10+IE_RatioTable[Parameter->SettingIEValue])*Parameter->SettingRateValue*100 / (60*IE_RatioTable[Parameter->SettingIEValue]*(100-Parameter->SettingTpValue)) > FLOW_MAX)
// 					if((pObj->Data1 + pObj->BDInfo.Step)*100/Parameter->SettingTiValue > FLOW_MAX)
                    {
						strcpy(contorlText, cbdialog32string[FLOW_HIGH]);
                    }
                    else
                    {
                        pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
                        //Parameter->SettingTiValue = IEConvertTi(0, Parameter->SettingIEValue);
                        //Get_VentParaInfoString(MAX_FLOW, (float)(pObj->Data1*60)/(10*Parameter->SettingTiValue * (1 - Parameter->SettingTpValue/100.0)));
                        Get_VentParaInfoString(MAX_FLOW, (float)(pObj->Data1*(10+IE_RatioTable[Parameter->SettingIEValue])*Parameter->SettingRateValue*6.0 /(float)(60.0*IE_RatioTable[Parameter->SettingIEValue]*(100-Parameter->SettingTpValue))));
                        r = 1;
                    }
                }
            }
            else if((Parameter->BreathMode==PSV_MODE) && (Parameter->ApnoeaMode==VCV_MODE))
            {
                if(pObj->Data1 < 100) //  20130523
				{
					pObj->BDInfo.Step = 5;
				}                    
                else
				{
					pObj->BDInfo.Step = 10;
				}                                    
                if(pObj->Data1 + pObj->BDInfo.Step > pObj->BDInfo.Max)
                {                    
					sprintf(contorlText,"%s",cbdialog32string[VT_1600]);
                }
                else if(Parameter->SettingIEValue >= 6)
                {
                    if((pObj->Data1 + pObj->BDInfo.Step)*(10+IE_RatioTable[Parameter->SettingIEValue])*Parameter->SettingRateValue*100 /(60*10*100) > FLOW_MAX)
// 					if((pObj->Data1 + pObj->BDInfo.Step)*100/Parameter->SettingTiValue > FLOW_MAX)
                    {
                        strcpy(contorlText, cbdialog32string[FLOW_HIGH]);
                    }
                    else
                    {
                        pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
                        //Parameter->SettingTiValue = 10*60*100/((10+IE_RatioTable[Parameter->SettingIEValue])*Parameter->SettingRateValue);
                        //Get_VentParaInfoString(MAX_FLOW, (float)(pObj->Data1*60)/(10*Parameter->SettingTiValue * (1 - Parameter->SettingTpValue/100.0)));
                        Get_VentParaInfoString(MAX_FLOW, (float)(pObj->Data1*(10+IE_RatioTable[Parameter->SettingIEValue])*Parameter->SettingRateValue*6.0 /(float)(60.0*10*100)));
                        r = 1;
                    }
                }
                else
                {
                    if((pObj->Data1 + pObj->BDInfo.Step)*(10+IE_RatioTable[Parameter->SettingIEValue])*Parameter->SettingRateValue*100 /(60*IE_RatioTable[Parameter->SettingIEValue]*100) > FLOW_MAX)
// 					if((pObj->Data1 + pObj->BDInfo.Step)*100/Parameter->SettingTiValue > FLOW_MAX)
                    {
						strcpy(contorlText, cbdialog32string[FLOW_HIGH]);
                    }
                    else
                    {
                        pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
                        //Parameter->SettingTiValue = IEConvertTi(0, Parameter->SettingIEValue);
                        //Get_VentParaInfoString(MAX_FLOW, (float)(pObj->Data1*60)/(10*Parameter->SettingTiValue*100));
                        Get_VentParaInfoString(MAX_FLOW, (float)(pObj->Data1*(10+IE_RatioTable[Parameter->SettingIEValue])*Parameter->SettingRateValue*6.0 /(float)(60.0*IE_RatioTable[Parameter->SettingIEValue]*100)));
                        r = 1;
                    }
                }
            }
            else if( (Parameter->BreathMode==PCV_MODE) || (Parameter->BreathMode==SIMV_PCV_MODE) 
				|| ((Parameter->BreathMode==PSV_MODE) && (Parameter->ApnoeaMode==PCV_MODE)) )
            {
                //Pinsp
                if(Parameter->SettingPEEPValue < 3)  //sun add 20140627
                {
                    Parameter->SettingPEEPValue = 0;
                }
                if(pObj->Data1 + pObj->BDInfo.Step > pObj->BDInfo.Max)
                {
			        strcpy(contorlText, cbdialog32string[PINSP_70]);
                }
                else if(pObj->Data1 + pObj->BDInfo.Step + Parameter->SettingPEEPValue > 70)
                {
			        strcpy(contorlText, cbdialog32string[PINSP_PEEP_70]);
                }
                else if(pObj->Data1 + pObj->BDInfo.Step + Parameter->SettingPEEPValue >= AlarmSettingPara.PressureHighValue)
                {
			        strcpy(contorlText, cbdialog32string[PINSP_PEEP_PH]);
                }
                else
                {
                    pObj->Data1 = pObj->Data1 + pObj->BDInfo.Step;
                    r = 1;
                }
            }
			else if((Parameter->BreathMode==PRVC_MODE)||(Parameter->BreathMode==SIMV_PRVC_MODE))//PRVC SIMV_PRVC
			{
				if(pObj->Data1 < 100) //  20130523
				{
					pObj->BDInfo.Step = 5;
				}					
				else
				{
					pObj->BDInfo.Step = 10;
				}					
				if(pObj->Data1 + pObj->BDInfo.Step > pObj->BDInfo.Max)  //sun add 20140522
				{
					sprintf(contorlText,"%s",cbdialog32string[VT_1600]);
				}
				else
				{
					pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
					Get_VentParaInfoString(MAX_FLOW, (float)(pObj->Data1*60.0)/(10*Parameter->SettingTiValue));// * (1 - Parameter->SettingTpValue/100)
					r = 1;
				}
			}
			else if((Parameter->BreathMode==APRV_MODE) || (Parameter->BreathMode==BIPAP_MODE))
			{				
				if(pObj->Data1 + pObj->BDInfo.Step > pObj->BDInfo.Max)  //sun add 20140522
				{
					sprintf(contorlText,"%s","Phigh <= 80 cmH2O");
				}
				else
				{
					pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
					r = 1;
				}
			}
			else if(Parameter->BreathMode==VS_MODE)
			{
				if(pObj->Data1 + pObj->BDInfo.Step > pObj->BDInfo.Max)  
				{
					sprintf(contorlText,"%s",cbdialog32string[VT_1600]);
				}
				else
				{
					pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
					r = 1;
				}
			}
			else  //PRVC SIMV_PRVC
            {
                pObj->Data1 = pObj->Data1 + pObj->BDInfo.Step;
                r = 1;
            }
            break;        
        case GUI_ID_BUTTON1: //I:E  TI    
            if((Parameter->BreathMode==VCV_MODE) || (Parameter->BreathMode==SIGH_MODE)) //|| ((Parameter->BreathMode==PSV_MODE) && (Parameter->ApnoeaMode==VCV_MODE)) //  20130510              
            {
                //IE
		        if(pObj->Data1 + pObj->BDInfo.Step > pObj->BDInfo.Max)
                {
		            strcpy(contorlText, cbdialog32string[IE_110]);
                }
                else if(pObj->Data1 + pObj->BDInfo.Step >= 6)
                {
                    FlowTi = (60*10*(100-Parameter->SettingTpValue))/((10+IE_RatioTable[pObj->Data1+pObj->BDInfo.Step])*Parameter->SettingRateValue*1.0)+0.5;
                    //if(Parameter->SettingVtValue*(10+IE_RatioTable[pObj->Data1+pObj->BDInfo.Step])*Parameter->SettingRateValue*100 /(60*10*(100-Parameter->SettingTpValue)) > FLOW_MAX)
                    if(Parameter->SettingVtValue*100/FlowTi > FLOW_MAX)
                    {
			            strcpy(contorlText, cbdialog32string[FLOW_HIGH]);
                    }
                    else if(10*60*10.0/((10+IE_RatioTable[pObj->Data1+pObj->BDInfo.Step])*Parameter->SettingRateValue)*(1-Parameter->SettingTpValue/100.0) > TI_MAX)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_10]);
                    }
                    else if(10*60*10.0/((10+IE_RatioTable[pObj->Data1+pObj->BDInfo.Step])*Parameter->SettingRateValue)*(1-Parameter->SettingTpValue/100.0) < TI_MIN)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_1]);
                    }
                    else if((10*60*IE_RatioTable[pObj->Data1 + pObj->BDInfo.Step]/((10+IE_RatioTable[pObj->Data1+pObj->BDInfo.Step])*Parameter->SettingRateValue) < TE_MIN)
                        || ( 10*60*10/((10+IE_RatioTable[pObj->Data1+pObj->BDInfo.Step])*Parameter->SettingRateValue) >
                           (10*60/Parameter->SettingRateValue-TE_MIN)*(1-Parameter->SettingTpValue/100) )
			        )
		            {
			            strcpy(contorlText, cbdialog32string[TEXP_LOW]);
                    }
                    else
                    {
                        pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
			            Get_VentParaInfoString(TINSP_TIME, (float)(IEConvertTi(0, pObj->Data1, Parameter)));
                        r = 1;
                    }
                }
                else
                {
                    FlowTi = (60*IE_RatioTable[pObj->Data1 + pObj->BDInfo.Step]*(100-Parameter->SettingTpValue)) /((10+IE_RatioTable[pObj->Data1+pObj->BDInfo.Step])*Parameter->SettingRateValue);
                    //if(Parameter->SettingVtValue*(10+IE_RatioTable[pObj->Data1+pObj->BDInfo.Step])*Parameter->SettingRateValue*100 /(60*IE_RatioTable[pObj->Data1 + pObj->BDInfo.Step]*(100-Parameter->SettingTpValue)) > 1500)
                    if(Parameter->SettingVtValue*100/FlowTi > FLOW_MAX)
                    {
			            strcpy(contorlText, cbdialog32string[FLOW_HIGH]);
                    }
                    else if(10*60.0*IE_RatioTable[pObj->Data1 + pObj->BDInfo.Step]/((10+IE_RatioTable[pObj->Data1+pObj->BDInfo.Step])*Parameter->SettingRateValue)*(1-Parameter->SettingTpValue/100.0) > TI_MAX)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_10]);
                    }
                    else if(10*60.0*IE_RatioTable[pObj->Data1 + pObj->BDInfo.Step]/((10+IE_RatioTable[pObj->Data1+pObj->BDInfo.Step])*Parameter->SettingRateValue)*(1-Parameter->SettingTpValue/100.0) < TI_MIN)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_1]);
                    }
                    else if((10*60*10/((10+IE_RatioTable[pObj->Data1+pObj->BDInfo.Step])*Parameter->SettingRateValue) < TE_MIN)
                        || ( 10*60*IE_RatioTable[pObj->Data1 + pObj->BDInfo.Step]/((10+IE_RatioTable[pObj->Data1+pObj->BDInfo.Step])*Parameter->SettingRateValue) >
                           (10*60/Parameter->SettingRateValue-TE_MIN)*(1-Parameter->SettingTpValue/100) )
			        )
		            {
			            strcpy(contorlText, cbdialog32string[TEXP_LOW]);
                    }
                    else
                    {
                        pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
			            Get_VentParaInfoString(TINSP_TIME, (float)(IEConvertTi(0, pObj->Data1, Parameter)));
                        r = 1;
                    }
                }
            }
            else if((Parameter->BreathMode==PSV_MODE) && (Parameter->ApnoeaMode==VCV_MODE))
            {
                //IE
		        if(pObj->Data1 + pObj->BDInfo.Step > pObj->BDInfo.Max)
                {
		            strcpy(contorlText, cbdialog32string[IE_110]);
                }
                else if(pObj->Data1 + pObj->BDInfo.Step >= 6)
                {
                    FlowTi = (60*10*100) /((10+IE_RatioTable[pObj->Data1+pObj->BDInfo.Step])*Parameter->SettingRateValue*1.0)+0.5;
                    //if(Parameter->SettingVtValue*(10+IE_RatioTable[pObj->Data1+pObj->BDInfo.Step])*Parameter->SettingRateValue*100 /(60*10*100) > FLOW_MAX)
                    if(Parameter->SettingVtValue*100/FlowTi > FLOW_MAX)
                    {
			            strcpy(contorlText, cbdialog32string[FLOW_HIGH]);
                    }
                    else if(10*60*10.0/((10+IE_RatioTable[pObj->Data1+pObj->BDInfo.Step])*Parameter->SettingRateValue) > TI_MAX)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_10]);
                    }
                    else if(10*60*10.0/((10+IE_RatioTable[pObj->Data1+pObj->BDInfo.Step])*Parameter->SettingRateValue) < TI_MIN)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_1]);
                    }
                    else if(10*60*10/((10+IE_RatioTable[pObj->Data1+pObj->BDInfo.Step])*Parameter->SettingRateValue) < Parameter->SettingTslopeValue/10)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_TSLOPE]);
                    }
                    else if((10*60*IE_RatioTable[pObj->Data1 + pObj->BDInfo.Step]/((10+IE_RatioTable[pObj->Data1+pObj->BDInfo.Step])*Parameter->SettingRateValue) < TE_MIN)
                        || ( 10*60*10/((10+IE_RatioTable[pObj->Data1+pObj->BDInfo.Step])*Parameter->SettingRateValue) >
                           (10*60/Parameter->SettingRateValue-TE_MIN) ))
		            {
			            strcpy(contorlText, cbdialog32string[TEXP_LOW]);
                    }
                    else
                    {
                        pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
			            Get_VentParaInfoString(TINSP_TIME, (float)(IEConvertTi(0, pObj->Data1, Parameter)));
                        r = 1;
                    }
                }
                else
                {
                    FlowTi =  (60*IE_RatioTable[pObj->Data1 + pObj->BDInfo.Step]*100)/((10+IE_RatioTable[pObj->Data1+pObj->BDInfo.Step])*Parameter->SettingRateValue);
                    //if(Parameter->SettingVtValue*(10+IE_RatioTable[pObj->Data1+pObj->BDInfo.Step])*Parameter->SettingRateValue*100 /(60*IE_RatioTable[pObj->Data1 + pObj->BDInfo.Step]*100) > 1500)
                    if(Parameter->SettingVtValue*100/FlowTi > FLOW_MAX)
                    {
			            strcpy(contorlText, cbdialog32string[FLOW_HIGH]);
                    }
                    else if(10*60*IE_RatioTable[pObj->Data1 + pObj->BDInfo.Step]/((10+IE_RatioTable[pObj->Data1+pObj->BDInfo.Step])*Parameter->SettingRateValue) > TI_MAX)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_10]);
                    }
                    else if(10*60*IE_RatioTable[pObj->Data1 + pObj->BDInfo.Step]/((10+IE_RatioTable[pObj->Data1+pObj->BDInfo.Step])*Parameter->SettingRateValue) < TI_MIN)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_1]);
                    }
                    else if(10*60*IE_RatioTable[pObj->Data1 + pObj->BDInfo.Step]/((10+IE_RatioTable[pObj->Data1+pObj->BDInfo.Step])*Parameter->SettingRateValue) < Parameter->SettingTslopeValue/10)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_TSLOPE]);
                    }
                    else if((10*60*10/((10+IE_RatioTable[pObj->Data1+pObj->BDInfo.Step])*Parameter->SettingRateValue) < TE_MIN)
                        || ( 10*60*IE_RatioTable[pObj->Data1 + pObj->BDInfo.Step]/((10+IE_RatioTable[pObj->Data1+pObj->BDInfo.Step])*Parameter->SettingRateValue) >
                           (10*60/Parameter->SettingRateValue-TE_MIN) ))
		            {
			            strcpy(contorlText, cbdialog32string[TEXP_LOW]);
                    }
                    else
                    {
                        pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
			            Get_VentParaInfoString(TINSP_TIME, (float)(IEConvertTi(0, pObj->Data1, Parameter)));
                        r = 1;
                    }
                }
            }
            else 
            if((Parameter->BreathMode==PCV_MODE) || (Parameter->BreathMode== PRVC_MODE) || ((Parameter->BreathMode==PSV_MODE) && (Parameter->ApnoeaMode==PCV_MODE)))
            {
		        if(pObj->Data1 + pObj->BDInfo.Step > pObj->BDInfo.Max)
                {
		            strcpy(contorlText, cbdialog32string[IE_110]);
                }
                else if(pObj->Data1 + pObj->BDInfo.Step >= 6)
                {
                    if(10*60*10/((10+IE_RatioTable[pObj->Data1+pObj->BDInfo.Step])*Parameter->SettingRateValue) > TI_MAX)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_10]);
                    }
                    else if(10*60*10/((10+IE_RatioTable[pObj->Data1+pObj->BDInfo.Step])*Parameter->SettingRateValue) < TI_MIN)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_1]);
                    }
                    else if(10*60*10/((10+IE_RatioTable[pObj->Data1+pObj->BDInfo.Step])*Parameter->SettingRateValue) < Parameter->SettingTslopeValue/10)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_TSLOPE]);
                    }
                    else if((10*60*IE_RatioTable[pObj->Data1 + pObj->BDInfo.Step]/((10+IE_RatioTable[pObj->Data1+pObj->BDInfo.Step])*Parameter->SettingRateValue) < TE_MIN)
                        || ( (600*10/((10+IE_RatioTable[pObj->Data1 + pObj->BDInfo.Step])*Parameter->SettingRateValue)) > (600/Parameter->SettingRateValue-TE_MIN) )
			        )
		            {
			            strcpy(contorlText, cbdialog32string[TEXP_LOW]);
                    }
                    else
                    {
                        pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
			            Get_VentParaInfoString(TINSP_TIME, (float)(IEConvertTi(0, pObj->Data1, Parameter)));
                        r = 1;
                    }
                }
                else
                {
                    if(10*60*IE_RatioTable[pObj->Data1 + pObj->BDInfo.Step]/((10+IE_RatioTable[pObj->Data1+pObj->BDInfo.Step])*Parameter->SettingRateValue) > TI_MAX)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_10]);
                    }
                    else if(10*60*IE_RatioTable[pObj->Data1 + pObj->BDInfo.Step]/((10+IE_RatioTable[pObj->Data1+pObj->BDInfo.Step])*Parameter->SettingRateValue) < TI_MIN)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_1]);
                    }
                    else if(10*60*IE_RatioTable[pObj->Data1 + pObj->BDInfo.Step]/((10+IE_RatioTable[pObj->Data1+pObj->BDInfo.Step])*Parameter->SettingRateValue) < Parameter->SettingTslopeValue/10)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_TSLOPE]);
                    }
                    else if((10*60*10/((10+IE_RatioTable[pObj->Data1+pObj->BDInfo.Step])*Parameter->SettingRateValue) < TE_MIN)
                        || ( (600*IE_RatioTable[pObj->Data1 + pObj->BDInfo.Step]/((10+IE_RatioTable[pObj->Data1 + pObj->BDInfo.Step])*Parameter->SettingRateValue)) >
                           (600/Parameter->SettingRateValue-TE_MIN) )
			        )
		            {
			            strcpy(contorlText, cbdialog32string[TEXP_LOW]);
                    }
                    else
                    {
                        pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
			            Get_VentParaInfoString(TINSP_TIME, (float)(IEConvertTi(0, pObj->Data1, Parameter)));
                        r = 1;
                    }
                }
            }
            //TI
            else if(Parameter->BreathMode==SIMV_VCV_MODE)
            {
                if((pObj->Data1%pObj->BDInfo.Step) != 0)
                {
                    TiOnlyReadValue = pObj->Data1 + 10 - (pObj->Data1%pObj->BDInfo.Step);
                }
                else
                {
                    TiOnlyReadValue = pObj->Data1 + pObj->BDInfo.Step;
                }
		        if(TiOnlyReadValue > pObj->BDInfo.Max)
                {
		            strcpy(contorlText, cbdialog32string[TINSP_10]);
                }
                else if((Int32U)(100*Parameter->SettingVtValue*100/TiOnlyReadValue) < FLOW_MIN)
                {
		            strcpy(contorlText, cbdialog32string[FLOW_LOW]);
                }
                else if(TiOnlyReadValue > (100*60/Parameter->SettingSimvRateValue-TE_MIN*10))
                {
                    strcpy(contorlText, cbdialog32string[TEXP_LOW]);
                }
                else
                {
                    pObj->Data1 = TiOnlyReadValue;
                    Get_VentIEInfoString(pObj->Data1);
                    r = 1;
                }
            }
            else if( (Parameter->BreathMode==SIMV_PCV_MODE) || (Parameter->BreathMode==SIMV_PRVC_MODE) )
            {
                if((pObj->Data1%pObj->BDInfo.Step) != 0)
                {
                    TiOnlyReadValue = pObj->Data1 + 10 - (pObj->Data1%pObj->BDInfo.Step);
                }
                else
                {
                    TiOnlyReadValue = pObj->Data1 + pObj->BDInfo.Step;
                }
		        if(TiOnlyReadValue > pObj->BDInfo.Max)
                {
		            strcpy(contorlText, cbdialog32string[TINSP_10]);
                }
		        else if(TiOnlyReadValue > (100*60/Parameter->SettingSimvRateValue-TE_MIN*10))
		        {
		            strcpy(contorlText, cbdialog32string[TEXP_LOW]);
		        }
                else
                {
                    pObj->Data1 = TiOnlyReadValue;
                    Get_VentIEInfoString(pObj->Data1);
                    r = 1;
                }
            }
            else if(Parameter->BreathMode==PSV_MODE)
            {
		        if(pObj->Data1 + pObj->BDInfo.Step > pObj->BDInfo.Max)
                {
			        strcpy(contorlText, cbdialog32string[TINSP_10]);
                }
                else if((Int32U)(100*Parameter->SettingVtValue*100/(pObj->Data1 + pObj->BDInfo.Step)) < FLOW_MIN)
                {
			        strcpy(contorlText, cbdialog32string[FLOW_LOW]);
                }
		        else if((pObj->Data1 + pObj->BDInfo.Step) > (100*60/Parameter->SettingRateValue-TE_MIN*10))
		        {
			        strcpy(contorlText, cbdialog32string[TEXP_LOW]);
		        }
                else
                {
                    pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
		            Get_VentParaInfoString(TINSP_TIME, (float)(IEConvertTi(0, pObj->Data1, Parameter)));
                    r = 1;
                }
            }
			else if((Parameter->BreathMode==APRV_MODE) || (Parameter->BreathMode==BIPAP_MODE))
			{
				if(pObj->Data1 + pObj->BDInfo.Step > pObj->BDInfo.Max)
				{
					sprintf(contorlText,"%s","Plow <= 50 cmH2O");
				}
				else
				{
					pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
					r = 1;
				}
			}
            else
            {
                if((pObj->Data1 + pObj->BDInfo.Step)<=pObj->BDInfo.Max)
                {
                    pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
                    r = 1;
                }
            }
            break;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////			
        case GUI_ID_BUTTON2: //FREQ   
            if((Parameter->BreathMode==SIMV_VCV_MODE) || (Parameter->BreathMode==SIMV_PCV_MODE) || (Parameter->BreathMode==SIMV_PRVC_MODE))
            {
                //FREQ & TI
		        if(pObj->Data1 + pObj->BDInfo.Step > pObj->BDInfo.Max)
                {
					strcpy(contorlText, cbdialog32string[SIMV_FREQ_40]);
                }
		        else if((pObj->Data1 + pObj->BDInfo.Step) > (60*100/((Parameter->SettingTiValue)+10*TE_MIN)))
				{
					strcpy(contorlText, cbdialog32string[TEXP_LOW]);
				}
                else
                {
                    pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
                    r = 1;
                }
            }
            else if((Parameter->BreathMode==VCV_MODE) || (Parameter->BreathMode==SIGH_MODE))
            {
		        if(pObj->Data1 + pObj->BDInfo.Step > pObj->BDInfo.Max)
                {
			        strcpy(contorlText, cbdialog32string[FREQ_100]);
                }
                else if(Parameter->SettingIEValue >= 6)
                {
                    FlowTi = (60*10*(100-Parameter->SettingTpValue)) / ((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 + pObj->BDInfo.Step)*1.0)+0.5;
                    if(10*60*10.0/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 + pObj->BDInfo.Step))*(1-Parameter->SettingTpValue/100.0) > TI_MAX)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_10]);
                    }
                    else if(10*60*10.0/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 + pObj->BDInfo.Step))*(1-Parameter->SettingTpValue/100.0) < TI_MIN)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_1]);
                    }
                    else if((10*60*IE_RatioTable[Parameter->SettingIEValue]/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 + pObj->BDInfo.Step)) < TE_MIN)
                        ||  ( (pObj->Data1 + pObj->BDInfo.Step) > 10*60/(10*60*10/((pObj->Data1 + pObj->BDInfo.Step)*(10+IE_RatioTable[Parameter->SettingIEValue]))+TE_MIN) )
				    )
		            {
			            strcpy(contorlText, cbdialog32string[TEXP_LOW]);
                    }
                    //else if(Parameter->SettingVtValue*(10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 + pObj->BDInfo.Step)*100 / (60*10*(100-Parameter->SettingTpValue)) > 1500)
                    else if(Parameter->SettingVtValue*100/FlowTi > FLOW_MAX)
                    {
			            strcpy(contorlText, cbdialog32string[FLOW_HIGH]);
                    }
                    else
                    {
                        pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
			            Get_VentParaInfoString(TINSP_TIME, (float)(IEConvertTi(1, pObj->Data1, Parameter)));
                        r = 1;
                    }
                }
                else
                {
                    FlowTi = (60*IE_RatioTable[Parameter->SettingIEValue]*(100-Parameter->SettingTpValue)) / ((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 + pObj->BDInfo.Step));
                    if(10*60.0*IE_RatioTable[Parameter->SettingIEValue]/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 + pObj->BDInfo.Step))*(1-Parameter->SettingTpValue/100.0) > TI_MAX)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_10]);
                    }
                    else if(10*60.0*IE_RatioTable[Parameter->SettingIEValue]/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 + pObj->BDInfo.Step))*(1-Parameter->SettingTpValue/100.0) < TI_MIN)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_1]);
                    }
                    else if((10*60*10/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 + pObj->BDInfo.Step)) < TE_MIN)
                        || ( (pObj->Data1 + pObj->BDInfo.Step) > 10*60/(10*60*IE_RatioTable[Parameter->SettingIEValue]/((pObj->Data1 + pObj->BDInfo.Step)*(10+IE_RatioTable[Parameter->SettingIEValue]))+TE_MIN) )
						)
			        {
			            strcpy(contorlText, cbdialog32string[TEXP_LOW]);
                    }
                    //else if(Parameter->SettingVtValue*(10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 + pObj->BDInfo.Step)*100 / (60*IE_RatioTable[Parameter->SettingIEValue]*(100-Parameter->SettingTpValue)) > 1500)
                    else if(Parameter->SettingVtValue*100/FlowTi > FLOW_MAX)
                    {
			            strcpy(contorlText, cbdialog32string[FLOW_HIGH]);
                    }
                    else
                    {
                        pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
			            Get_VentParaInfoString(TINSP_TIME, (float)(IEConvertTi(1, pObj->Data1, Parameter)));
                        r = 1;
                    }
                }
            }
            else if((Parameter->BreathMode==PSV_MODE) && (Parameter->ApnoeaMode==VCV_MODE))
            {
                if(pObj->Data1 + pObj->BDInfo.Step > pObj->BDInfo.Max)
                {
			        strcpy(contorlText, cbdialog32string[FREQ_100]);
                }
                else if(Parameter->SettingIEValue >= 6)
                {
                    FlowTi = (60*10*100) / ((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 + pObj->BDInfo.Step)*1.0)+0.5;
                    if(10*60*10/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 + pObj->BDInfo.Step)) > TI_MAX)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_10]);
                    }
                    else if(10*60*10/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 + pObj->BDInfo.Step)) < TI_MIN)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_1]);
                    }
                    else if((10*60*IE_RatioTable[Parameter->SettingIEValue]/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 + pObj->BDInfo.Step)) < TE_MIN)
                        ||  ( (pObj->Data1 + pObj->BDInfo.Step) > 10*60/(10*60*10/((pObj->Data1 + pObj->BDInfo.Step)*(10+IE_RatioTable[Parameter->SettingIEValue]))+TE_MIN) )
					)
		            {
			            strcpy(contorlText, cbdialog32string[TEXP_LOW]);
                    }
                    //else if(Parameter->SettingVtValue*(10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 + pObj->BDInfo.Step)*100 / (60*10*100) > 1500)
                    else if(Parameter->SettingVtValue*100/FlowTi > FLOW_MAX)
                    {
			            strcpy(contorlText, cbdialog32string[FLOW_HIGH]);
                    }
                    else if(10*60*10/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 + pObj->BDInfo.Step)) < Parameter->SettingTslopeValue/10)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_TSLOPE]);
                    }
                    else
                    {
                        pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
			            Get_VentParaInfoString(TINSP_TIME, (float)(IEConvertTi(1, pObj->Data1, Parameter)));
                        r = 1;
                    }
                }
                else
                {
                    FlowTi = (60*IE_RatioTable[Parameter->SettingIEValue]*100) / ((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 + pObj->BDInfo.Step));
                    if(10*60*IE_RatioTable[Parameter->SettingIEValue]/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 + pObj->BDInfo.Step)) > TI_MAX)
                    {
				        strcpy(contorlText, cbdialog32string[TINSP_10]);
                    }
                    else if(10*60*IE_RatioTable[Parameter->SettingIEValue]/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 + pObj->BDInfo.Step)) < TI_MIN)
                    {
				        strcpy(contorlText, cbdialog32string[TINSP_1]);
                    }
                    else if((10*60*10/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 + pObj->BDInfo.Step)) < TE_MIN)
                        || ( (pObj->Data1 + pObj->BDInfo.Step) > 10*60/(10*60*IE_RatioTable[Parameter->SettingIEValue]/((pObj->Data1 + pObj->BDInfo.Step)*(10+IE_RatioTable[Parameter->SettingIEValue]))+TE_MIN) )
					)
		            {
				        strcpy(contorlText, cbdialog32string[TEXP_LOW]);
                    }
                    //else if(Parameter->SettingVtValue*(10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 + pObj->BDInfo.Step)*100 / (60*IE_RatioTable[Parameter->SettingIEValue]*100) > 1500)
                    else if(Parameter->SettingVtValue*100/FlowTi > FLOW_MAX)
                    {
				        strcpy(contorlText, cbdialog32string[FLOW_HIGH]);
                    }
                    else if(10*60*IE_RatioTable[Parameter->SettingIEValue]/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 + pObj->BDInfo.Step)) < Parameter->SettingTslopeValue/10)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_TSLOPE]);
                    }
                    else
                    {
                        pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
			            Get_VentParaInfoString(TINSP_TIME, (float)(IEConvertTi(1, pObj->Data1, Parameter)));
                        r = 1;
                    }
                }
            }
            else if((Parameter->BreathMode==PSV_MODE) && (Parameter->ApnoeaMode==PCV_MODE))
            {
		        if(pObj->Data1 + pObj->BDInfo.Step > pObj->BDInfo.Max)
                {
			        strcpy(contorlText, cbdialog32string[FREQ_100]);
                }
                else if(Parameter->SettingIEValue >= 6)
                {
                    if(10*60*10/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 + pObj->BDInfo.Step)) > TI_MAX)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_10]);
                    }
                    else if(10*60*10/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 + pObj->BDInfo.Step)) < TI_MIN)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_1]);
                    }
                    else if((10*60*IE_RatioTable[Parameter->SettingIEValue]/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 + pObj->BDInfo.Step)) < TE_MIN)
                        ||  ( (pObj->Data1 + pObj->BDInfo.Step) > 10*60/(10*60*10/((pObj->Data1 + pObj->BDInfo.Step)*(10+IE_RatioTable[Parameter->SettingIEValue]))+TE_MIN) )
			        )
		            {
			            strcpy(contorlText, cbdialog32string[TEXP_LOW]);
                    }
                    else if(10*60*10/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 + pObj->BDInfo.Step)) < Parameter->SettingTslopeValue/10)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_TSLOPE]);
                    }
                    else
                    {
                        pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
			            Get_VentParaInfoString(TINSP_TIME, (float)(IEConvertTi(1, pObj->Data1, Parameter)));
                        r = 1;
                    }
                }
                else
                {
                    if(10*60*IE_RatioTable[Parameter->SettingIEValue]/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 + pObj->BDInfo.Step)) > TI_MAX)
                    {
				        strcpy(contorlText, cbdialog32string[TINSP_10]);
                    }
                    else if(10*60*IE_RatioTable[Parameter->SettingIEValue]/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 + pObj->BDInfo.Step)) < TI_MIN)
                    {
				        strcpy(contorlText, cbdialog32string[TINSP_1]);
                    }
                    else if((10*60*10/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 + pObj->BDInfo.Step)) < TE_MIN)
                        || ( (pObj->Data1 + pObj->BDInfo.Step) > 10*60/(10*60*IE_RatioTable[Parameter->SettingIEValue]/((pObj->Data1 + pObj->BDInfo.Step)*(10+IE_RatioTable[Parameter->SettingIEValue]))+TE_MIN) )
			        )
		            {
			            strcpy(contorlText, cbdialog32string[TEXP_LOW]);
                    }
                    else if(10*60*IE_RatioTable[Parameter->SettingIEValue]/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 + pObj->BDInfo.Step)) < Parameter->SettingTslopeValue/10)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_TSLOPE]);
                    }
                    else
                    {
                        pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
			            Get_VentParaInfoString(TINSP_TIME, (float)(IEConvertTi(1, pObj->Data1, Parameter)));
                        r = 1;
                    }
                }
            }
            else if(Parameter->BreathMode==PCV_MODE || Parameter->BreathMode==PRVC_MODE)
            {
		        if(pObj->Data1 + pObj->BDInfo.Step > pObj->BDInfo.Max)
                {
					strcpy(contorlText, cbdialog32string[FREQ_100]);
                }
                else if(Parameter->SettingIEValue >= 6)
                {
                    if( 10*60*10/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 + pObj->BDInfo.Step)) < Parameter->SettingTslopeValue/10)
                    {
						strcpy(contorlText, cbdialog32string[TINSP_TSLOPE]);
                    }
                    else if(10*60*10/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 + pObj->BDInfo.Step)) > TI_MAX)
                    {
						strcpy(contorlText, cbdialog32string[TINSP_10]);
                    }
                    else if(10*60*10/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 + pObj->BDInfo.Step)) < TI_MIN)
                    {
						strcpy(contorlText, cbdialog32string[TINSP_1]);
                    }
                    else if((10*60*IE_RatioTable[Parameter->SettingIEValue]/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 + pObj->BDInfo.Step)) < TE_MIN)
                        || ((pObj->Data1 + pObj->BDInfo.Step) > (600/(10*600/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 + pObj->BDInfo.Step))+TE_MIN)) )
						)
					{
						strcpy(contorlText, cbdialog32string[TEXP_LOW]);
                    }
                    else
                    {
                        pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
						Get_VentParaInfoString(TINSP_TIME, (float)(IEConvertTi(1, pObj->Data1, Parameter)));
                        r = 1;
                    }
                }
                else
                {
                    if(10*60*IE_RatioTable[Parameter->SettingIEValue]/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 + pObj->BDInfo.Step)) < Parameter->SettingTslopeValue/10 )
                    {
						strcpy(contorlText, cbdialog32string[TINSP_TSLOPE]);
                    }
                    else if(10*60*IE_RatioTable[Parameter->SettingIEValue]/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 + pObj->BDInfo.Step)) > TI_MAX)
                    {
						strcpy(contorlText, cbdialog32string[TINSP_10]);
                    }
                    else if(10*60*IE_RatioTable[Parameter->SettingIEValue]/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 + pObj->BDInfo.Step)) < TI_MIN)
                    {
						strcpy(contorlText, cbdialog32string[TINSP_1]);
                    }
                    else if((10*60*10/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 + pObj->BDInfo.Step)) < TE_MIN)
                        || ((pObj->Data1 + pObj->BDInfo.Step) > (600/(IE_RatioTable[Parameter->SettingIEValue]*600/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 + pObj->BDInfo.Step))+TE_MIN)) )
						)
					{
						strcpy(contorlText, cbdialog32string[TEXP_LOW]);
                    }
                    else
                    {
                        pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
						Get_VentParaInfoString(TINSP_TIME, (float)(IEConvertTi(1, pObj->Data1, Parameter)));
                        r = 1;
                    }
                }
            }
			else if(Parameter->BreathMode==APRV_MODE)
			{
				if(pObj->Data1 + pObj->BDInfo.Step > pObj->BDInfo.Max)
				{
					sprintf(contorlText,"%s","Thigh <= 10s");
				}
				else
				{
					pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
					r = 1;
				}
			}
			else if(Parameter->BreathMode==BIPAP_MODE)
			{
				if(pObj->Data1 + pObj->BDInfo.Step > pObj->BDInfo.Max)
				{
					//sprintf(contorlText,"%s","f <= 100s");
				}
				else
				{
					pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
					r = 1;
				}
			}
			else if(Parameter->BreathMode==VS_MODE)
			{
				if(pObj->Data1 + pObj->BDInfo.Step > pObj->BDInfo.Max)
				{
					//sprintf(contorlText,"%s","f <= 100s");
				}
				else
				{
					pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
					r = 1;
				}
			}
			else // VS_MODE
            {
		        if(pObj->Data1 + pObj->BDInfo.Step > pObj->BDInfo.Max)
                {
					strcpy(contorlText, cbdialog32string[FREQ_100]);
                }
                else
                {
                    pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
					Get_VentParaInfoString(TINSP_TIME, (float)(IEConvertTi(1, pObj->Data1, Parameter)));
                    r = 1;
                }
            }
            break;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case GUI_ID_BUTTON3: //Pause  Psupp    
            if((Parameter->BreathMode==VCV_MODE) || (Parameter->BreathMode==SIGH_MODE))
            {
                //Pause
                if(pObj->Data1 + pObj->BDInfo.Step > pObj->BDInfo.Max)
                {
					strcpy(contorlText, cbdialog32string[PAUSE_60]);
                }
                else if(Parameter->SettingIEValue >= 6)
                {
                    FlowTi = (60*10*(100-(pObj->Data1 + pObj->BDInfo.Step))) / ((10+IE_RatioTable[Parameter->SettingIEValue])*Parameter->SettingRateValue*1.0)+0.5;
                    //if(Parameter->SettingVtValue*(10+IE_RatioTable[Parameter->SettingIEValue])*Parameter->SettingRateValue*100 / (60*10*(100-(pObj->Data1 + pObj->BDInfo.Step))) > FLOW_MAX)
                    if(Parameter->SettingVtValue*100/FlowTi > FLOW_MAX)
                    {
			            strcpy(contorlText, cbdialog32string[FLOW_HIGH]);
                    }
                    else if(10*60*10.0/((10+IE_RatioTable[Parameter->SettingIEValue])*Parameter->SettingRateValue)*(1-(pObj->Data1 + pObj->BDInfo.Step)/100.0) > TI_MAX)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_10]);
                    }
                    else if(10*60*10.0/((10+IE_RatioTable[Parameter->SettingIEValue])*Parameter->SettingRateValue)*(1-(pObj->Data1 + pObj->BDInfo.Step)/100.0) < TI_MIN)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_1]);
                    }
                    else
                    {
                        pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
			            //Parameter->SettingTiValue = 10*60*100/((10+IE_RatioTable[Parameter->SettingIEValue])*Parameter->SettingRateValue);
			            Get_VentParaInfoString(MAX_FLOW, (float)(Parameter->SettingVtValue*(10+IE_RatioTable[Parameter->SettingIEValue])*Parameter->SettingRateValue*6 /(float)(60*10*(100-pObj->Data1))));
                        r = 1;
                    }
                }
				else if(Parameter->BreathMode==APRV_MODE)
				{
					if(pObj->Data1 + pObj->BDInfo.Step > pObj->BDInfo.Max)
					{
						sprintf(contorlText,"%s","Thigh <= 10s");
					}
					else
					{
						pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
						r = 1;
					}
				}
                else
                {
                    FlowTi = (60*IE_RatioTable[Parameter->SettingIEValue]*(100-(pObj->Data1 + pObj->BDInfo.Step))) / ((10+IE_RatioTable[Parameter->SettingIEValue])*Parameter->SettingRateValue);
                    //if(Parameter->SettingVtValue*(10+IE_RatioTable[Parameter->SettingIEValue])*Parameter->SettingRateValue*100 / (60*IE_RatioTable[Parameter->SettingIEValue]*(100-(pObj->Data1 + pObj->BDInfo.Step))) > FLOW_MAX)
                    if(Parameter->SettingVtValue*100/FlowTi > FLOW_MAX)
                    {
			            strcpy(contorlText, cbdialog32string[FLOW_HIGH]);
                    }
                    else if(10*60.0*IE_RatioTable[Parameter->SettingIEValue]/((10+IE_RatioTable[Parameter->SettingIEValue])*Parameter->SettingRateValue)*(1-(pObj->Data1 + pObj->BDInfo.Step)/100.0) > TI_MAX)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_10]);
                    }
                    else if(10*60.0*IE_RatioTable[Parameter->SettingIEValue]/((10+IE_RatioTable[Parameter->SettingIEValue])*Parameter->SettingRateValue)*(1-(pObj->Data1 + pObj->BDInfo.Step)/100.0) < TI_MIN)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_1]);
                    }
                    else
                    {
                        pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
						//Parameter->SettingTiValue = IEConvertTi(0, Parameter->SettingIEValue);
						//Get_VentParaInfoString(MAX_FLOW, (float)(Parameter->SettingVtValue*60)/(10*Parameter->SettingTiValue * (1 - pObj->Data1/100.0)));
                        Get_VentParaInfoString(MAX_FLOW, (float)(Parameter->SettingVtValue*(10+IE_RatioTable[Parameter->SettingIEValue])*Parameter->SettingRateValue*6 /(float)(60*IE_RatioTable[Parameter->SettingIEValue]*(100-pObj->Data1))));
                        r = 1;
                    }
                }
            }
			else if(Parameter->BreathMode==APRV_MODE)
			{
				if(pObj->Data1 + pObj->BDInfo.Step > pObj->BDInfo.Max)
				{
					sprintf(contorlText,"%s","Tlow <= 10s");
				}
				else
				{
					pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
					r = 1;
				}
			}
			else if(Parameter->BreathMode==BIPAP_MODE)
			{
				if(pObj->Data1 + pObj->BDInfo.Step > pObj->BDInfo.Max)
				{
					sprintf(contorlText,"%s","Thigh <= 10s");
				}
				else
				{
					pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
					r = 1;
				}
			}
			else if(Parameter->BreathMode==VS_MODE)
			{
				if(pObj->Data1 + pObj->BDInfo.Step > pObj->BDInfo.Max)
				{
					sprintf(contorlText,"%s","Pressure Limit <= 100 cmH2O");
				}
				else
				{
					pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
					r = 1;
				}
			}
			else
            {
                //Psupp
                if(Parameter->SettingPEEPValue < 3)  //sun add 20140627
                {
                    Parameter->SettingPEEPValue = 0;
                }
                if(pObj->Data1 + pObj->BDInfo.Step > pObj->BDInfo.Max)
                {
			        strcpy(contorlText, cbdialog32string[PSUPP_70]);
                }
                else if(pObj->Data1 + pObj->BDInfo.Step + Parameter->SettingPEEPValue >= AlarmSettingPara.PressureHighValue)
                {
					strcpy(contorlText, cbdialog32string[PSUPP_PEEP_PH]);
                }
                else if(pObj->Data1 + pObj->BDInfo.Step + Parameter->SettingPEEPValue > 70)
                {
					strcpy(contorlText, cbdialog32string[PSUPP_PEEP_70]);
                }
                else
                {
                    pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
                    r = 1;
                }
            }
            break;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case GUI_ID_BUTTON4: //  20130607
            if(pObj->Data1 + pObj->BDInfo.Step > pObj->BDInfo.Max)
            {
		        strcpy(contorlText, cbdialog32string[TSLOPE_2]);
            }
            else if(pObj->Data1 + pObj->BDInfo.Step > Parameter->SettingTiValue )
            {
		        strcpy(contorlText, cbdialog32string[TINSP_TSLOPE]);
            }
	        else
            {
                pObj->Data1 = pObj->Data1 + pObj->BDInfo.Step;
                r = 1;
            }
            break;
        case GUI_ID_BUTTON5:
            if(pObj->Data1 + pObj->BDInfo.Step > pObj->BDInfo.Max)
            {
                if(Parameter->TriggerMode)
                {
                    strcpy(contorlText, cbdialog32string[FSENSE_20]);
                }
                else
                {
//#if (LOGO_FLAG != HOSPITECH_LOGO)
//                    strcpy(contorlText, cbdialog32string[PSENSE_20]);
//#else
					strcpy(contorlText, "Psens <= 0");
//#endif
                }
            }
            else
            //if(pObj->Data1 + pObj->BDInfo.Step <= pObj->BDInfo.Max)
            {
                pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
                r = 1;
            }
            break;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case GUI_ID_BUTTON6: //PEEP     //GUI_ID_BUTTON5 cpp md 20130607
            if(pObj->Data1 < 3)//2013-12-15
            {
                pObj->Data1 = 2;
            }

	        if((pObj->Data1 + pObj->BDInfo.Step) > pObj->BDInfo.Max)
            {
				strcpy(contorlText, cbdialog32string[PEEP_30]);
            }
            else if(Parameter->BreathMode==SIMV_PCV_MODE)
            {
                if((pObj->Data1 + pObj->BDInfo.Step + Parameter->SettingPsupportValue) >= AlarmSettingPara.PressureHighValue)
                {
					strcpy(contorlText, cbdialog32string[PSUPP_PEEP_PH]);
                }
				else if(pObj->Data1 + pObj->BDInfo.Step + Parameter->SettingPinspValue > 70)
				{
					strcpy(contorlText, cbdialog32string[PINSP_PEEP_70]);
				}
				else if(pObj->Data1 + pObj->BDInfo.Step + Parameter->SettingPsupportValue > 70)
				{
					strcpy(contorlText, cbdialog32string[PSUPP_PEEP_70]);
				}
                else if((pObj->Data1 + pObj->BDInfo.Step + Parameter->SettingPinspValue) >= AlarmSettingPara.PressureHighValue)
                {
					strcpy(contorlText, cbdialog32string[PINSP_PEEP_PH]);
                }
                else
                {
                    pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
                    r = 1;
                }
            }
            else if((Parameter->BreathMode==PSV_MODE)\
				 || (Parameter->BreathMode==SIMV_VCV_MODE)\
				 || (Parameter->BreathMode==SIMV_PRVC_MODE))
            {
                if((pObj->Data1 + pObj->BDInfo.Step + Parameter->SettingPsupportValue) >= AlarmSettingPara.PressureHighValue)
                {
					strcpy(contorlText, cbdialog32string[PSUPP_PEEP_PH]);
                }
// 				//add zkq 20161024 PSV BACKUP下防止参数超出限制
// 				else if((pObj->Data1 + pObj->BDInfo.Step + Parameter->SettingPinspValue) >= AlarmSettingPara.PressureHighValue)
// 				{
// 					strcpy(contorlText, cbdialog32string[PINSP_PEEP_PH]);
// 				}
				else if(pObj->Data1 + pObj->BDInfo.Step + Parameter->SettingPsupportValue > 70)
				{
					strcpy(contorlText, cbdialog32string[PSUPP_PEEP_70]);
				}
				else if((Parameter->BreathMode==PSV_MODE) && (Parameter->ApnoeaMode == PCV_MODE))
				{
					//add zkq 20161024 PSV BACKUP下防止参数超出限制
					if((pObj->Data1 + pObj->BDInfo.Step + Parameter->SettingPinspValue) >= AlarmSettingPara.PressureHighValue)
					{
						strcpy(contorlText, cbdialog32string[PINSP_PEEP_PH]);
					}
					else if(pObj->Data1 + pObj->BDInfo.Step + Parameter->SettingPinspValue > 70)
					{
						strcpy(contorlText, cbdialog32string[PINSP_PEEP_70]);
					}
				}
                else
                {
                    pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
                    r = 1;
                }
            }
            else if(Parameter->BreathMode==PCV_MODE)
            {

                if((pObj->Data1 + pObj->BDInfo.Step + Parameter->SettingPinspValue) >= AlarmSettingPara.PressureHighValue)
                {
					strcpy(contorlText, cbdialog32string[PINSP_PEEP_PH]);
                }
				else if(pObj->Data1 + pObj->BDInfo.Step + Parameter->SettingPinspValue > 70)
				{
					strcpy(contorlText, cbdialog32string[PINSP_PEEP_70]);
				}
                else
                {
                    pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
                    r = 1;
                }
            }
            else //PRVC VCV SIGH
            {
                if(pObj->Data1 + pObj->BDInfo.Step >= AlarmSettingPara.PressureHighValue)
                {
					strcpy(contorlText, cbdialog32string[PEEP_PH]);
                }
                else
                {
                    pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
                    r = 1;
                }
            }
            break;
        case GUI_ID_BUTTON7:  //sun add 20140623
            if(pObj->Data1 + pObj->BDInfo.Step > pObj->BDInfo.Max)
            {
                strcpy(contorlText, cbdialog32string[ESENSE_80]);
            }
            else
            {
                pObj->Data1 += pObj->BDInfo.Step;
                r = 1;
            }
            break;
        default:
            if((pObj->Data1 + pObj->BDInfo.Step) <= pObj->BDInfo.Max)
            {
                pObj->Data1 = pObj->Data1 + pObj->BDInfo.Step;
                r = 1;
            }
        }
    }
    //left key decrease
    else if(type==GUI_KEY_LEFT)
    {
        switch(pObj->Widget.Id)
        {
        case GUI_ID_BUTTON0: //VT  Pinsp
            if((Parameter->BreathMode==VCV_MODE)|| (Parameter->BreathMode==SIGH_MODE)) //|| ((Parameter->BreathMode==PSV_MODE) && (Parameter->ApnoeaMode==VCV_MODE)) //  20130510
            {
                //VT
                if(pObj->Data1 <= 100) //  20130523
				{
					pObj->BDInfo.Step = 5;
				}
				else
				{
					pObj->BDInfo.Step = 10;
				}                                                 
                if(pObj->Data1 - pObj->BDInfo.Step < pObj->BDInfo.Min)
                {
					sprintf(contorlText,"%s",cbdialog32string[VT_10]);
                }
                else if(Parameter->SettingIEValue >= 6)
                {
                    data = (100*(10+IE_RatioTable[Parameter->SettingIEValue])*Parameter->SettingRateValue*100);
                    data = data*(pObj->Data1 - pObj->BDInfo.Step);
                    lowFlow = data/(60*10*(100-Parameter->SettingTpValue));
                    if(lowFlow < FLOW_MIN)
                    {
						strcpy(contorlText, cbdialog32string[FLOW_LOW]);
                    }
                    else
                    {
                        pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
//						Parameter->SettingTiValue = 10*60*100/((10+IE_RatioTable[Parameter->SettingIEValue])*Parameter->SettingRateValue);
//						Get_VentParaInfoString(MAX_FLOW, (float)(pObj->Data1*60)/((10*Parameter->SettingTiValue * (1 - Parameter->SettingTpValue/100.0))) );
                        Get_VentParaInfoString(MAX_FLOW, (float)(pObj->Data1*(10+IE_RatioTable[Parameter->SettingIEValue])*Parameter->SettingRateValue*6.0 /(float)(60.0*10*(100-Parameter->SettingTpValue))));
                        r=1;
                    }
                }
                else
                {
                    data = (100*(10+IE_RatioTable[Parameter->SettingIEValue])*Parameter->SettingRateValue*100);
                    data = data*(pObj->Data1 - pObj->BDInfo.Step);
                    lowFlow =  data/(60*IE_RatioTable[Parameter->SettingIEValue]*(100-Parameter->SettingTpValue));
                    if(lowFlow < FLOW_MIN)
                    {
						strcpy(contorlText, cbdialog32string[FLOW_LOW]);
                    }
                    else
                    {
                        pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
// 						Parameter->SettingTiValue = IEConvertTi(0, Parameter->SettingIEValue);
// 						Get_VentParaInfoString(MAX_FLOW, (float)(pObj->Data1*60)/(10*Parameter->SettingTiValue * (1 - Parameter->SettingTpValue/100.0)));
                        Get_VentParaInfoString(MAX_FLOW, (float)(pObj->Data1*(10+IE_RatioTable[Parameter->SettingIEValue])*Parameter->SettingRateValue*6.0 /(float)(60.0*IE_RatioTable[Parameter->SettingIEValue]*(100-Parameter->SettingTpValue))));
                        r=1;
                    }
                }
            }
			else if(Parameter->BreathMode==PRVC_MODE)
			{
				//VT
				if(pObj->Data1 <= 100) //  20130523
				{
					pObj->BDInfo.Step = 5;
				}					
				else
				{
					pObj->BDInfo.Step = 10;
				}					
				if(pObj->Data1 - pObj->BDInfo.Step < pObj->BDInfo.Min)
				{
					sprintf(contorlText,"%s",cbdialog32string[VT_10]);
				}
				else if(Parameter->SettingIEValue >= 6)
				{
					data = (100*(10+IE_RatioTable[Parameter->SettingIEValue])*Parameter->SettingRateValue*100);
					data = data*(pObj->Data1 - pObj->BDInfo.Step);
					lowFlow = data/(60*10*(100-Parameter->SettingTpValue));
					{
						pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
						Get_VentParaInfoString(MAX_FLOW, (float)(pObj->Data1*(10+IE_RatioTable[Parameter->SettingIEValue])*Parameter->SettingRateValue*6.0 /(float)(60.0*10*(100-Parameter->SettingTpValue))));
						r=1;
					}
				}
				else
				{
					data = (100*(10+IE_RatioTable[Parameter->SettingIEValue])*Parameter->SettingRateValue*100);
					data = data*(pObj->Data1 - pObj->BDInfo.Step);
					lowFlow =  data/(60*IE_RatioTable[Parameter->SettingIEValue]*(100-Parameter->SettingTpValue));
					{
						pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
						Get_VentParaInfoString(MAX_FLOW, (float)(pObj->Data1*(10+IE_RatioTable[Parameter->SettingIEValue])*Parameter->SettingRateValue*6.0 /(float)(60.0*IE_RatioTable[Parameter->SettingIEValue]*(100-Parameter->SettingTpValue))));
						r=1;
					}
				}
			}
            else if((Parameter->BreathMode==PSV_MODE) && (Parameter->ApnoeaMode==VCV_MODE))
            {
                if(pObj->Data1 <= 100) //  20130523
				{
					pObj->BDInfo.Step = 5;
				}                    
                else
				{
					pObj->BDInfo.Step = 10;
				}                                    
                if(pObj->Data1 - pObj->BDInfo.Step < pObj->BDInfo.Min)
                {
					sprintf(contorlText,"%s",cbdialog32string[VT_10]);
                }
                else if(Parameter->SettingIEValue >= 6)
                {
                    data = (100*(10+IE_RatioTable[Parameter->SettingIEValue])*Parameter->SettingRateValue*100);
                    data = data*(pObj->Data1 - pObj->BDInfo.Step);
                    lowFlow = data/(60*10*100);
                    if(lowFlow < FLOW_MIN)
                    {
						strcpy(contorlText, cbdialog32string[FLOW_LOW]);
                    }
                    else
                    {
                        pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
// 						Parameter->SettingTiValue = 10*60*100/((10+IE_RatioTable[Parameter->SettingIEValue])*Parameter->SettingRateValue);
// 						Get_VentParaInfoString(MAX_FLOW, (float)(pObj->Data1*60)/((10*Parameter->SettingTiValue)) );
                        Get_VentParaInfoString(MAX_FLOW, (float)(pObj->Data1*(10+IE_RatioTable[Parameter->SettingIEValue])*Parameter->SettingRateValue*6.0 /(float)(60.0*10*100)));
                        r=1;
                    }
                }
                else
                {
                    data = (100*(10+IE_RatioTable[Parameter->SettingIEValue])*Parameter->SettingRateValue*100);
                    data = data*(pObj->Data1 - pObj->BDInfo.Step);
                    lowFlow =  data/(60*IE_RatioTable[Parameter->SettingIEValue]*100);
                    if(lowFlow < FLOW_MIN)
                    {
						strcpy(contorlText, cbdialog32string[FLOW_LOW]);
                    }
                    else
                    {
                        pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
// 						Parameter->SettingTiValue = IEConvertTi(0, Parameter->SettingIEValue);
// 						Get_VentParaInfoString(MAX_FLOW, (float)(pObj->Data1*60)/(10*Parameter->SettingTiValue));
                        Get_VentParaInfoString(MAX_FLOW, (float)(pObj->Data1*(10+IE_RatioTable[Parameter->SettingIEValue])*Parameter->SettingRateValue*6.0 /(float)(60.0*IE_RatioTable[Parameter->SettingIEValue]*100)));
                        r=1;
                    }
                }
            }
            else if(Parameter->BreathMode==SIMV_VCV_MODE)
            {
                if(pObj->Data1 <= 100) //  20130523
				{
					pObj->BDInfo.Step = 5;
				}                    
                else
				{
					pObj->BDInfo.Step = 10;
				}                                
                if(pObj->Data1 - pObj->BDInfo.Step < pObj->BDInfo.Min)
                {
					sprintf(contorlText,"%s",cbdialog32string[VT_10]);
                }
                else if((Int32U)(100*(pObj->Data1 - pObj->BDInfo.Step)*100/Parameter->SettingTiValue) < FLOW_MIN)
                {
					strcpy(contorlText, cbdialog32string[FLOW_LOW]);
                }
                else
                {
                    pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
                    Get_VentParaInfoString(MAX_FLOW, (float)(pObj->Data1*60.0)/(10*Parameter->SettingTiValue));// * (1 - Parameter->SettingTpValue/100)
					//Get_VentParaInfoString(MAX_FLOW, (float)(pObj->Data1*60)/(10*Parameter->SettingTiValue * (1 - Parameter->SettingTpValue/100.0)));
                    r=1;
                }
            }
			else if(Parameter->BreathMode==SIMV_PRVC_MODE)
			{
				if(pObj->Data1 <= 100) //  20130523
				{
					pObj->BDInfo.Step = 5;
				}					
				else
				{
					pObj->BDInfo.Step = 10;
				}					
				if(pObj->Data1 - pObj->BDInfo.Step < pObj->BDInfo.Min)
				{
					sprintf(contorlText,"%s",cbdialog32string[VT_10]);
				}
				else
				{
					pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
					Get_VentParaInfoString(MAX_FLOW, (float)(pObj->Data1*60.0)/(10*Parameter->SettingTiValue));// * (1 - Parameter->SettingTpValue/100)
					//Get_VentParaInfoString(MAX_FLOW, (float)(pObj->Data1*60)/(10*Parameter->SettingTiValue * (1 - Parameter->SettingTpValue/100.0)));
					r=1;
				}
			}
			else if(Parameter->BreathMode==APRV_MODE || Parameter->BreathMode==BIPAP_MODE)
			{
				if(pObj->Data1 - pObj->BDInfo.Step < pObj->BDInfo.Min)
				{
					sprintf(contorlText,"%s","Phigh >= 5 cmH2O");
				}
				else
				{
					pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
					r=1;
				}
			}
			else if(Parameter->BreathMode==VS_MODE)
			{
				if(pObj->Data1 - pObj->BDInfo.Step < pObj->BDInfo.Min)
				{
					sprintf(contorlText,"%s",cbdialog32string[VT_10]);
				}
				else
				{
					pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
					r=1;
				}
			}
			else //Pinsp
            {
                if(pObj->Data1 - pObj->BDInfo.Step < pObj->BDInfo.Min)
                {
			        strcpy(contorlText, cbdialog32string[PINSP_5]);
                }
                else //if((pObj->Data1 - pObj->BDInfo.Step) >= pObj->BDInfo.Min)
                {
                    pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
                    r=1;
                }
            }
            break;
        case GUI_ID_BUTTON1: //TI I:E
            if((Parameter->BreathMode==SIMV_VCV_MODE)) // | (Parameter->BreathMode==PSV_MODE)
            {
                //TI
                if((pObj->Data1%pObj->BDInfo.Step) != 0)
                {
                    TiOnlyReadValue = pObj->Data1 - (pObj->Data1%pObj->BDInfo.Step);
                }
                else
                {
                    TiOnlyReadValue = pObj->Data1 - pObj->BDInfo.Step;
                }
		        if(TiOnlyReadValue < pObj->BDInfo.Min)
                {
		            strcpy(contorlText, cbdialog32string[TINSP_1]);
                }
                else if(TiOnlyReadValue <Parameter->SettingTslopeValue)
                {
		            strcpy(contorlText, cbdialog32string[TINSP_TSLOPE]);
                }
                else if((Parameter->SettingVtValue*100/TiOnlyReadValue) > FLOW_MAX)
                {
		            strcpy(contorlText, cbdialog32string[FLOW_HIGH]);
                }
                else
                {
                    pObj->Data1 = TiOnlyReadValue;
                    Get_VentIEInfoString(pObj->Data1);
                    r = 1;
                }
            }
			else if(Parameter->BreathMode==SIMV_PRVC_MODE)
			{
				//TI
				if((pObj->Data1%pObj->BDInfo.Step) != 0)
				{
					TiOnlyReadValue = pObj->Data1 - (pObj->Data1%pObj->BDInfo.Step);
				}
				else
				{
					TiOnlyReadValue = pObj->Data1 - pObj->BDInfo.Step;
				}
				if(TiOnlyReadValue < pObj->BDInfo.Min)
				{
					strcpy(contorlText, cbdialog32string[TINSP_1]);
				}
				else if(TiOnlyReadValue <Parameter->SettingTslopeValue)
				{
					strcpy(contorlText, cbdialog32string[TINSP_TSLOPE]);
				}
				else
				{
					pObj->Data1 = TiOnlyReadValue;
					Get_VentIEInfoString(pObj->Data1);
					r = 1;
				}
			}
            else if((Parameter->BreathMode==SIMV_PCV_MODE))
            {
                //TI
                if((pObj->Data1%pObj->BDInfo.Step) != 0)
                {
                    TiOnlyReadValue = pObj->Data1 - (pObj->Data1%pObj->BDInfo.Step);
                }
                else
                {
                    TiOnlyReadValue = pObj->Data1 - pObj->BDInfo.Step;
                }
                if(TiOnlyReadValue < pObj->BDInfo.Min)
                {
		            strcpy(contorlText, cbdialog32string[TINSP_1]);
                }
                else if(TiOnlyReadValue <Parameter->SettingTslopeValue)
                {
		            strcpy(contorlText, cbdialog32string[TINSP_TSLOPE]);
                }
                else
                {
                    pObj->Data1 = TiOnlyReadValue;
// 					TiConvertIE(pObj->Data1);
// 					Get_VentParaInfoString(IE_RATIO, (float)(IE_RatioTable[Parameter->SettingIEValue]/10));
                    Get_VentIEInfoString(pObj->Data1);
                    r = 1;
                }
            }
            else if((Parameter->BreathMode==VCV_MODE) || (Parameter->BreathMode==SIGH_MODE)) //|| ((Parameter->BreathMode==PSV_MODE)&&(Parameter->ApnoeaMode == VCV_MODE)) //  20130513              
            {
                //IE
	            if(pObj->Data1 - pObj->BDInfo.Step < pObj->BDInfo.Min)
                {
		            strcpy(contorlText, cbdialog32string[IE_41]);
                }
                else if( (pObj->Data1 - pObj->BDInfo.Step) >= 6)
                {
                    data = (100*(10+IE_RatioTable[pObj->Data1-pObj->BDInfo.Step])*Parameter->SettingRateValue*100);
                    data = data*Parameter->SettingVtValue;
                    lowFlow = data/(60*10*(100-Parameter->SettingTpValue));
                    if(lowFlow < FLOW_MIN)
                    {
			            strcpy(contorlText, cbdialog32string[FLOW_LOW]);
                    }
                    else if(10*60*10.0/((10+IE_RatioTable[pObj->Data1-pObj->BDInfo.Step])*Parameter->SettingRateValue)*(1-Parameter->SettingTpValue/100) > TI_MAX)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_10]);
                    }
                    else if(10*60*10.0/((10+IE_RatioTable[pObj->Data1-pObj->BDInfo.Step])*Parameter->SettingRateValue)*(1-Parameter->SettingTpValue/100) < TI_MIN)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_1]);
                    }
                    else if((10*60*IE_RatioTable[pObj->Data1 - pObj->BDInfo.Step]/((10+IE_RatioTable[pObj->Data1-pObj->BDInfo.Step])*Parameter->SettingRateValue) < TE_MIN)
                        || ( 10*60*10/((10+IE_RatioTable[pObj->Data1-pObj->BDInfo.Step])*Parameter->SettingRateValue) >
                         (10*60/Parameter->SettingRateValue-TE_MIN)*(1-Parameter->SettingTpValue/100) ) 
			        )
		            {
			            strcpy(contorlText, cbdialog32string[TEXP_LOW]);
                    }
                    else
                    {
                        pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
			            Get_VentParaInfoString(TINSP_TIME, (float)(IEConvertTi(0, pObj->Data1, Parameter)));
                        r = 1;
                    }
                }
                else
                {
                    data = (100*(10+IE_RatioTable[pObj->Data1-pObj->BDInfo.Step])*Parameter->SettingRateValue*100);
                    data = data*Parameter->SettingVtValue;
                    lowFlow = data/(60*IE_RatioTable[pObj->Data1 - pObj->BDInfo.Step]*(100-Parameter->SettingTpValue));
                    if(lowFlow < FLOW_MIN)
                    {
			            strcpy(contorlText, cbdialog32string[FLOW_LOW]);
                    }
                    else if(10*60.0*IE_RatioTable[pObj->Data1 - pObj->BDInfo.Step]/((10+IE_RatioTable[pObj->Data1-pObj->BDInfo.Step])*Parameter->SettingRateValue)*(1-Parameter->SettingTpValue/100) > TI_MAX)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_10]);
                    }
                    else if(10*60.0*IE_RatioTable[pObj->Data1 - pObj->BDInfo.Step]/((10+IE_RatioTable[pObj->Data1-pObj->BDInfo.Step])*Parameter->SettingRateValue)*(1-Parameter->SettingTpValue/100) < TI_MIN)
                    {
                        strcpy(contorlText, cbdialog32string[TINSP_1]);
                    }
                    else if((10*60*10/((10+IE_RatioTable[pObj->Data1-pObj->BDInfo.Step])*Parameter->SettingRateValue) < TE_MIN)
                        || ( 10*60*IE_RatioTable[pObj->Data1 - pObj->BDInfo.Step]/((10+IE_RatioTable[pObj->Data1-pObj->BDInfo.Step])*Parameter->SettingRateValue) >
                         (10*60/Parameter->SettingRateValue-TE_MIN)*(1-Parameter->SettingTpValue/100) )
		            )
		            {
			            strcpy(contorlText, cbdialog32string[TEXP_LOW]);
                    }
                    else
                    {
                        pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
			            Get_VentParaInfoString(TINSP_TIME, (float)(IEConvertTi(0, pObj->Data1, Parameter)));
                        r = 1;
                    }
                }
            }
			else if(Parameter->BreathMode== PRVC_MODE)
			{
				//IE
				if(pObj->Data1 - pObj->BDInfo.Step < pObj->BDInfo.Min)
				{
					strcpy(contorlText, cbdialog32string[IE_41]);
				}
				else if( (pObj->Data1 - pObj->BDInfo.Step) >= 6)
				{
					data = (100*(10+IE_RatioTable[pObj->Data1-pObj->BDInfo.Step])*Parameter->SettingRateValue*100);
					data = data*Parameter->SettingVtValue;
					lowFlow = data/(60*10*(100-Parameter->SettingTpValue));
					if(10*60*10.0/((10+IE_RatioTable[pObj->Data1-pObj->BDInfo.Step])*Parameter->SettingRateValue)*(1-Parameter->SettingTpValue/100) > TI_MAX)
					{
						strcpy(contorlText, cbdialog32string[TINSP_10]);
					}
					else if(10*60*10.0/((10+IE_RatioTable[pObj->Data1-pObj->BDInfo.Step])*Parameter->SettingRateValue)*(1-Parameter->SettingTpValue/100) < TI_MIN)
					{
						strcpy(contorlText, cbdialog32string[TINSP_1]);
					}
					else
					{
						pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
						Get_VentParaInfoString(TINSP_TIME, (float)(IEConvertTi(0, pObj->Data1, Parameter)));
						r = 1;
					}
				}
				else
				{
					data = (100*(10+IE_RatioTable[pObj->Data1-pObj->BDInfo.Step])*Parameter->SettingRateValue*100);
					data = data*Parameter->SettingVtValue;
					lowFlow = data/(60*IE_RatioTable[pObj->Data1 - pObj->BDInfo.Step]*(100-Parameter->SettingTpValue));
					if(10*60.0*IE_RatioTable[pObj->Data1 - pObj->BDInfo.Step]/((10+IE_RatioTable[pObj->Data1-pObj->BDInfo.Step])*Parameter->SettingRateValue)*(1-Parameter->SettingTpValue/100) > TI_MAX)
					{
						strcpy(contorlText, cbdialog32string[TINSP_10]);
					}
					else if(10*60.0*IE_RatioTable[pObj->Data1 - pObj->BDInfo.Step]/((10+IE_RatioTable[pObj->Data1-pObj->BDInfo.Step])*Parameter->SettingRateValue)*(1-Parameter->SettingTpValue/100) < TI_MIN)
					{
						strcpy(contorlText, cbdialog32string[TINSP_1]);
					}
					else
					{
						pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
						Get_VentParaInfoString(TINSP_TIME, (float)(IEConvertTi(0, pObj->Data1, Parameter)));
						r = 1;
					}
				}
			}
            else if((Parameter->BreathMode==PSV_MODE)&&(Parameter->ApnoeaMode == VCV_MODE))
            {
                //IE
	            if(pObj->Data1 - pObj->BDInfo.Step < pObj->BDInfo.Min)
                {
		            strcpy(contorlText, cbdialog32string[IE_41]);
                }
                else if( (pObj->Data1 - pObj->BDInfo.Step) >= 6)
                {
                    data = (100*(10+IE_RatioTable[pObj->Data1-pObj->BDInfo.Step])*Parameter->SettingRateValue*100);
                    data = data*Parameter->SettingVtValue;
                    lowFlow = data/(60*10*100);
                    if(lowFlow < FLOW_MIN)
                    {
			            strcpy(contorlText, cbdialog32string[FLOW_LOW]);
                    }
                    else if(10*60*10/((10+IE_RatioTable[pObj->Data1-pObj->BDInfo.Step])*Parameter->SettingRateValue) > TI_MAX)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_10]);
                    }
                    else if(10*60*10/((10+IE_RatioTable[pObj->Data1-pObj->BDInfo.Step])*Parameter->SettingRateValue) < TI_MIN)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_1]);
                    }
                    else if(10*60*10/((10+IE_RatioTable[pObj->Data1-pObj->BDInfo.Step])*Parameter->SettingRateValue) < Parameter->SettingTslopeValue/10)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_TSLOPE]);
                    }
                    else if((10*60*IE_RatioTable[pObj->Data1 - pObj->BDInfo.Step]/((10+IE_RatioTable[pObj->Data1-pObj->BDInfo.Step])*Parameter->SettingRateValue) < TE_MIN)
                        || ( 10*60*10/((10+IE_RatioTable[pObj->Data1-pObj->BDInfo.Step])*Parameter->SettingRateValue) >
                         (10*60/Parameter->SettingRateValue-TE_MIN) ) 
			        )
		            {
			            strcpy(contorlText, cbdialog32string[TEXP_LOW]);
                    }
                    else
                    {
                        pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
			            Get_VentParaInfoString(TINSP_TIME, (float)(IEConvertTi(0, pObj->Data1, Parameter)));
                        r = 1;
                    }
                }
                else
                {
                    data = (100*(10+IE_RatioTable[pObj->Data1-pObj->BDInfo.Step])*Parameter->SettingRateValue*100);
                    data = data*Parameter->SettingVtValue;
                    lowFlow = data/(60*IE_RatioTable[pObj->Data1 - pObj->BDInfo.Step]*100);
                    if(lowFlow < FLOW_MIN)
                    {
			            strcpy(contorlText, cbdialog32string[FLOW_LOW]);
                    }
                    else if(10*60*IE_RatioTable[pObj->Data1 - pObj->BDInfo.Step]/((10+IE_RatioTable[pObj->Data1-pObj->BDInfo.Step])*Parameter->SettingRateValue) > TI_MAX)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_10]);
                    }
                    else if(10*60*IE_RatioTable[pObj->Data1 - pObj->BDInfo.Step]/((10+IE_RatioTable[pObj->Data1-pObj->BDInfo.Step])*Parameter->SettingRateValue) < TI_MIN)
                    {
                        strcpy(contorlText, cbdialog32string[TINSP_1]);
                    }
                    else if(10*60*IE_RatioTable[pObj->Data1 - pObj->BDInfo.Step]/((10+IE_RatioTable[pObj->Data1-pObj->BDInfo.Step])*Parameter->SettingRateValue) < Parameter->SettingTslopeValue/10)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_TSLOPE]);
                    }
                    else if((10*60*10/((10+IE_RatioTable[pObj->Data1-pObj->BDInfo.Step])*Parameter->SettingRateValue) < TE_MIN)
                        || ( 10*60*IE_RatioTable[pObj->Data1 - pObj->BDInfo.Step]/((10+IE_RatioTable[pObj->Data1-pObj->BDInfo.Step])*Parameter->SettingRateValue) >
                         (10*60/Parameter->SettingRateValue-TE_MIN) )
		            )
		            {
			            strcpy(contorlText, cbdialog32string[TEXP_LOW]);
                    }
                    else
                    {
                        pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
			            Get_VentParaInfoString(TINSP_TIME, (float)(IEConvertTi(0, pObj->Data1, Parameter)));
                        r = 1;
                    }
                }
            }
            //*//  20130308
            else 
            if((Parameter->BreathMode == PCV_MODE) || ((Parameter->BreathMode == PSV_MODE)&&(Parameter->ApnoeaMode == PCV_MODE))) //  20130618              
            {
                if(pObj->Data1 - pObj->BDInfo.Step < pObj->BDInfo.Min)
                {
			        strcpy(contorlText, cbdialog32string[IE_41]);
                }
                else if(pObj->Data1 - pObj->BDInfo.Step >= 6)
                {
                    if(10*60*10/((10+IE_RatioTable[pObj->Data1-pObj->BDInfo.Step])*Parameter->SettingRateValue) > TI_MAX)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_10]);
                    }
                    else if(10*60*10/((10+IE_RatioTable[pObj->Data1-pObj->BDInfo.Step])*Parameter->SettingRateValue) < TI_MIN)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_1]);
                    }
                    else if(10*60*IE_RatioTable[pObj->Data1 - pObj->BDInfo.Step]/((10+IE_RatioTable[pObj->Data1-pObj->BDInfo.Step])*Parameter->SettingRateValue) < TE_MIN)
                    {
			            strcpy(contorlText, cbdialog32string[TEXP_LOW]);
                    }
                    else if(10*60*10/((10+IE_RatioTable[pObj->Data1-pObj->BDInfo.Step])*Parameter->SettingRateValue) < Parameter->SettingTslopeValue/10)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_TSLOPE]);
                    }
                    else
                    {
                        pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
			            Get_VentParaInfoString(TINSP_TIME, (float)(IEConvertTi(0, pObj->Data1, Parameter)));
                        r = 1;
                    }
                }
                else
                {
                    if(10*60*IE_RatioTable[pObj->Data1 - pObj->BDInfo.Step]/((10+IE_RatioTable[pObj->Data1-pObj->BDInfo.Step])*Parameter->SettingRateValue) > TI_MAX)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_10]);
                    }
                    else if(10*60*IE_RatioTable[pObj->Data1 - pObj->BDInfo.Step]/((10+IE_RatioTable[pObj->Data1-pObj->BDInfo.Step])*Parameter->SettingRateValue) < TI_MIN)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_1]);
                    }
                    else if(10*60*10/((10+IE_RatioTable[pObj->Data1-pObj->BDInfo.Step])*Parameter->SettingRateValue) < TE_MIN)
                    {
			            strcpy(contorlText, cbdialog32string[TEXP_LOW]);
                    }
                    else if(10*60*IE_RatioTable[pObj->Data1 - pObj->BDInfo.Step]/((10+IE_RatioTable[pObj->Data1-pObj->BDInfo.Step])*Parameter->SettingRateValue) < Parameter->SettingTslopeValue/10)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_TSLOPE]);
                    }
                    else
                    {
                        pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
			            Get_VentParaInfoString(TINSP_TIME, (float)(IEConvertTi(0, pObj->Data1, Parameter)));
                        r = 1;
                    }
                }
            }//*///endof   20130308
			else if(Parameter->BreathMode==APRV_MODE || Parameter->BreathMode==BIPAP_MODE)
			{
				if(pObj->Data1 - pObj->BDInfo.Step < pObj->BDInfo.Min)
				{
					sprintf(contorlText,"%s","Plow >= 2 cmH2O");
				}
				else
				{
					pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
					r=1;
				}
			}
			else
            {
                if((pObj->Data1 - pObj->BDInfo.Step) >= pObj->BDInfo.Min)                 
                {
                    pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
		            r = 1;
                }
            }
            break;
        case GUI_ID_BUTTON2: //FREQ
			if(Parameter->BreathMode==APRV_MODE)
			{
				if(pObj->Data1 - pObj->BDInfo.Step < pObj->BDInfo.Min)
				{
					sprintf(contorlText,"%s","Thigh >= 0.2s");
				}
				else
				{
					pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
					r=1;
				}
			}
			else if(Parameter->BreathMode==VS_MODE || Parameter->BreathMode==BIPAP_MODE)
			{
				if(pObj->Data1 - pObj->BDInfo.Step < pObj->BDInfo.Min)
				{
					//sprintf(contorlText,"%s","f >= 2 bpm");
				}
				else
				{
					pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
					r=1;
				}
			}
	        else if(pObj->Data1 - pObj->BDInfo.Step < pObj->BDInfo.Min)
            {
		        strcpy(contorlText, cbdialog32string[FREQ_1]);
            }
            else if((Parameter->BreathMode==VCV_MODE) || (Parameter->BreathMode==SIGH_MODE)) //| (Parameter->BreathMode==PSV_MODE) //  20130510              
            {
                if(Parameter->SettingIEValue >= 6)
                {
                    data = (100*(10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 - pObj->BDInfo.Step)*100);
                    data = data*Parameter->SettingVtValue;
                    lowFlow = data/(60*10*(100-Parameter->SettingTpValue));
                    if(10*60*10.0/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 - pObj->BDInfo.Step))*(1-Parameter->SettingTpValue/100.0) > TI_MAX)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_10]);
                    }
                    else if(10*60*10.0/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 - pObj->BDInfo.Step))*(1-Parameter->SettingTpValue/100.0) < TI_MIN)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_1]);
                    }
                    else if( 10*60*IE_RatioTable[Parameter->SettingIEValue]/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 - pObj->BDInfo.Step)) < TE_MIN)
                    {
			            strcpy(contorlText, cbdialog32string[TEXP_LOW]);
                    }
                    else if((lowFlow < FLOW_MIN)
                        || ( (pObj->Data1 - pObj->BDInfo.Step) > 10*60/(10*60*10/((pObj->Data1 - pObj->BDInfo.Step)*(10+IE_RatioTable[Parameter->SettingIEValue]))+TE_MIN) )
			        )
                    {
                        strcpy(contorlText, cbdialog32string[FLOW_LOW]);
                    }
                    else
                    {
                        pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
		                Get_VentParaInfoString(TINSP_TIME, (float)(IEConvertTi(1, pObj->Data1, Parameter)));
			            r = 1;
                    }
                }
		        else
                {
                    data = (100*(10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 - pObj->BDInfo.Step)*100);
                    data = data*Parameter->SettingVtValue;
                    lowFlow = data/(60*IE_RatioTable[Parameter->SettingIEValue]*(100-Parameter->SettingTpValue));
                    if(10*60.0*IE_RatioTable[Parameter->SettingIEValue]/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 - pObj->BDInfo.Step))*(1-Parameter->SettingTpValue/100.0) > TI_MAX)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_10]);
                    }
                    else if(10*60.0*IE_RatioTable[Parameter->SettingIEValue]/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 - pObj->BDInfo.Step))*(1-Parameter->SettingTpValue/100.0) < TI_MIN)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_1]);
                    }
                    else if(10*60*10/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 - pObj->BDInfo.Step)) < TE_MIN)
                    {
			            strcpy(contorlText, cbdialog32string[TEXP_LOW]);
                    }
                    else if((lowFlow < FLOW_MIN)
                        || ( (pObj->Data1 - pObj->BDInfo.Step) > 10*60/(10*60*IE_RatioTable[Parameter->SettingIEValue]/((pObj->Data1 - pObj->BDInfo.Step)*(10+IE_RatioTable[Parameter->SettingIEValue]))+TE_MIN) )
				    )
                    {
                        strcpy(contorlText, cbdialog32string[FLOW_LOW]);
                    }
                    else
                    {
                        pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
			            Get_VentParaInfoString(TINSP_TIME, (float)(IEConvertTi(1, pObj->Data1, Parameter)));
			            r = 1;
                    }
		        }
            }
            else if((Parameter->BreathMode==PSV_MODE) && (Parameter->ApnoeaMode==VCV_MODE))
            {
                if(Parameter->SettingIEValue >= 6)
                {
                    data = (100*(10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 - pObj->BDInfo.Step)*100);
                    data = data*Parameter->SettingVtValue;
                    lowFlow = data/(60*10*100);
                    if(10*60*10/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 - pObj->BDInfo.Step)) > TI_MAX)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_10]);
                    }
                    else if(10*60*10/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 - pObj->BDInfo.Step)) < TI_MIN)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_1]);
                    }
                    else if( 10*60*IE_RatioTable[Parameter->SettingIEValue]/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 - pObj->BDInfo.Step)) < TE_MIN)
                    {
			            strcpy(contorlText, cbdialog32string[TEXP_LOW]);
                    }
                    else if(10*60*10/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 - pObj->BDInfo.Step)) < Parameter->SettingTslopeValue/10)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_TSLOPE]);
                    }
                    else if((lowFlow < FLOW_MIN)
                        || ( (pObj->Data1 - pObj->BDInfo.Step) > 10*60/(10*60*10/((pObj->Data1 - pObj->BDInfo.Step)*(10+IE_RatioTable[Parameter->SettingIEValue]))+TE_MIN) )
			        )
                    {
                        strcpy(contorlText, cbdialog32string[FLOW_LOW]);
                    }
                    else
                    {
                        pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
		                Get_VentParaInfoString(TINSP_TIME, (float)(IEConvertTi(1, pObj->Data1, Parameter)));
			            r = 1;
                    }
                }
		        else
                {
                    data = (100*(10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 - pObj->BDInfo.Step)*100);
                    data = data*Parameter->SettingVtValue;
                    lowFlow = data/(60*IE_RatioTable[Parameter->SettingIEValue]*100);
                    if(10*60*IE_RatioTable[Parameter->SettingIEValue]/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 - pObj->BDInfo.Step)) > TI_MAX)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_10]);
                    }
                    else if(10*60*IE_RatioTable[Parameter->SettingIEValue]/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 - pObj->BDInfo.Step)) < TI_MIN)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_1]);
                    }
                    else if(10*60*10/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 - pObj->BDInfo.Step)) < TE_MIN)
                    {
			            strcpy(contorlText, cbdialog32string[TEXP_LOW]);
                    }
                    else if(10*60*IE_RatioTable[Parameter->SettingIEValue]/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 - pObj->BDInfo.Step)) < Parameter->SettingTslopeValue/10)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_TSLOPE]);
                    }
                    else if((lowFlow < FLOW_MIN)
                        || ( (pObj->Data1 - pObj->BDInfo.Step) > 10*60/(10*60*IE_RatioTable[Parameter->SettingIEValue]/((pObj->Data1 - pObj->BDInfo.Step)*(10+IE_RatioTable[Parameter->SettingIEValue]))+TE_MIN) )
				    )
                    {
                        strcpy(contorlText, cbdialog32string[FLOW_LOW]);
                    }
                    else
                    {
                        pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
			            Get_VentParaInfoString(TINSP_TIME, (float)(IEConvertTi(1, pObj->Data1, Parameter)));
			            r = 1;
                    }
		        }
            }
            else if(((Parameter->BreathMode==PSV_MODE) && (Parameter->ApnoeaMode==PCV_MODE))\
				|| (Parameter->BreathMode==PCV_MODE)|| (Parameter->BreathMode==PRVC_MODE))
			{	
				if(pObj->Data1 - pObj->BDInfo.Step > pObj->BDInfo.Max)
				{
					strcpy(contorlText, cbdialog32string[FREQ_100]);
				}
				else if(Parameter->SettingIEValue >= 6)
                {
                    if(10*60*10/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 - pObj->BDInfo.Step)) > TI_MAX)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_10]);
                    }
                    else if(10*60*10/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 - pObj->BDInfo.Step)) < TI_MIN)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_1]);
                    }
                    else if((10*60*IE_RatioTable[Parameter->SettingIEValue]/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 - pObj->BDInfo.Step)) < TE_MIN)
                        ||  ( (pObj->Data1 - pObj->BDInfo.Step) > 10*60/(10*60*10/((pObj->Data1 - pObj->BDInfo.Step)*(10+IE_RatioTable[Parameter->SettingIEValue]))+TE_MIN) )
			        )
		            {
			            strcpy(contorlText, cbdialog32string[TEXP_LOW]);
                    }
                    else if(10*60*10/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 - pObj->BDInfo.Step)) < Parameter->SettingTslopeValue/10)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_TSLOPE]);
                    }
                    else
                    {
                        pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
			            Get_VentParaInfoString(TINSP_TIME, (float)(IEConvertTi(1, pObj->Data1, Parameter)));
                        r = 1;
                    }
                }
                else
                {
                    if(10*60*IE_RatioTable[Parameter->SettingIEValue]/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 - pObj->BDInfo.Step)) > TI_MAX)
                    {
				        strcpy(contorlText, cbdialog32string[TINSP_10]);
                    }
                    else if(10*60*IE_RatioTable[Parameter->SettingIEValue]/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 - pObj->BDInfo.Step)) < TI_MIN)
                    {
				        strcpy(contorlText, cbdialog32string[TINSP_1]);
                    }
                    else if((10*60*10/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 - pObj->BDInfo.Step)) < TE_MIN)
                        || ( (pObj->Data1 - pObj->BDInfo.Step) > 10*60/(10*60*IE_RatioTable[Parameter->SettingIEValue]/((pObj->Data1 - pObj->BDInfo.Step)*(10+IE_RatioTable[Parameter->SettingIEValue]))+TE_MIN) )
			        )
		            {
			            strcpy(contorlText, cbdialog32string[TEXP_LOW]);
                    }
                    else if(10*60*IE_RatioTable[Parameter->SettingIEValue]/((10+IE_RatioTable[Parameter->SettingIEValue])*(pObj->Data1 - pObj->BDInfo.Step)) < Parameter->SettingTslopeValue/10)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_TSLOPE]);
                    }
                    else
                    {
                        pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
			            Get_VentParaInfoString(TINSP_TIME, (float)(IEConvertTi(1, pObj->Data1, Parameter)));
                        r = 1;
                    }
                }
            }
			else
            {
              //  if((pObj->Data1 - pObj->BDInfo.Step) >= pObj->BDInfo.Min)
                {
                    pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
                    r = 1;
                }
            }
            break;
        case GUI_ID_BUTTON3: //Pause Psupp
            if((Parameter->BreathMode==VCV_MODE) || (Parameter->BreathMode==SIGH_MODE))
            {
                if(pObj->Data1 - pObj->BDInfo.Step < pObj->BDInfo.Min)
                {
		            strcpy(contorlText, cbdialog32string[PAUSE_0]);
                }
                else if(Parameter->SettingIEValue >= 6)
                {
                    data = (100*(10+IE_RatioTable[Parameter->SettingIEValue])*Parameter->SettingRateValue*100);
                    data = data*Parameter->SettingVtValue;
                    lowFlow = data/(60*10*(100-(pObj->Data1 - pObj->BDInfo.Step)));
                    if(lowFlow < FLOW_MIN)
                    {
			            strcpy(contorlText, cbdialog32string[FLOW_LOW]);
                    }
                    else if(10*60*10.0/((10+IE_RatioTable[Parameter->SettingIEValue])*Parameter->SettingRateValue)*(1-(pObj->Data1 - pObj->BDInfo.Step)/100.0) > TI_MAX)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_10]);
                    }
                    else if(10*60*10.0/((10+IE_RatioTable[Parameter->SettingIEValue])*Parameter->SettingRateValue)*(1-(pObj->Data1 - pObj->BDInfo.Step)/100.0) < TI_MIN)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_1]);
                    }
                    else
                    {
                        pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
// 						Parameter->SettingTiValue = 10*60*100/((10+IE_RatioTable[Parameter->SettingIEValue])*Parameter->SettingRateValue);
// 						Get_VentParaInfoString(MAX_FLOW, (float)(Parameter->SettingVtValue*60)/(10*Parameter->SettingTiValue * (1 - pObj->Data1/100.0)));
                        Get_VentParaInfoString(MAX_FLOW, (float)(Parameter->SettingVtValue*(10+IE_RatioTable[Parameter->SettingIEValue])*Parameter->SettingRateValue*6 /(float)(60*10*(100-pObj->Data1))));
                        r = 1;
                    }
              	}
                else
                {
                    data = (100*(10+IE_RatioTable[Parameter->SettingIEValue])*Parameter->SettingRateValue*100);
                    data = data*Parameter->SettingVtValue;
                    lowFlow = data/(60*IE_RatioTable[Parameter->SettingIEValue]*(100-(pObj->Data1 - pObj->BDInfo.Step)));
                    if(lowFlow < FLOW_MIN)
                    {
			            strcpy(contorlText, cbdialog32string[FLOW_LOW]);
                    }
                    else if(10*60.0*IE_RatioTable[Parameter->SettingIEValue]/((10+IE_RatioTable[Parameter->SettingIEValue])*Parameter->SettingRateValue)*(1-(pObj->Data1 - pObj->BDInfo.Step)/100.0) > TI_MAX)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_10]);
                    }
                    else if(10*60.0*IE_RatioTable[Parameter->SettingIEValue]/((10+IE_RatioTable[Parameter->SettingIEValue])*Parameter->SettingRateValue)*(1-(pObj->Data1 - pObj->BDInfo.Step)/100.0) < TI_MIN)
                    {
			            strcpy(contorlText, cbdialog32string[TINSP_1]);
                    }
                    else
                    {
                        pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
// 						Parameter->SettingTiValue = IEConvertTi(0, Parameter->SettingIEValue);
// 						Get_VentParaInfoString(MAX_FLOW, (float)(Parameter->SettingVtValue*60)/(10*Parameter->SettingTiValue * (1 - pObj->Data1/100.0)));
                        Get_VentParaInfoString(MAX_FLOW, (float)(Parameter->SettingVtValue*(10+IE_RatioTable[Parameter->SettingIEValue])*Parameter->SettingRateValue*6 /(float)(60*IE_RatioTable[Parameter->SettingIEValue]*(100-pObj->Data1))));
                        r = 1;
                    }
		}
            }
			else if(Parameter->BreathMode==APRV_MODE)
			{
				if(pObj->Data1 - pObj->BDInfo.Step < pObj->BDInfo.Min)
				{
					sprintf(contorlText,"%s","Tlow >= 0.3s");
				}
				else
				{
					pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
					r=1;
				}
			}
			else if(Parameter->BreathMode==BIPAP_MODE)
			{
				if(pObj->Data1 - pObj->BDInfo.Step < pObj->BDInfo.Min)
				{
					//sprintf(contorlText,"%s","Thigh >= 0.2s");
				}
				else
				{
					pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
					r=1;
				}
			}
			else if(Parameter->BreathMode==VS_MODE)
			{
				if(pObj->Data1 - pObj->BDInfo.Step < pObj->BDInfo.Min)
				{
					sprintf(contorlText,"%s","Pressure Limit >= 10 cmH2O");
				}
				else
				{
					pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
					r=1;
				}
			}
			else
            {
                if(pObj->Data1 - pObj->BDInfo.Step < pObj->BDInfo.Min)
                {
		            strcpy(contorlText, cbdialog32string[PSUPP_0]);
                }
                else //if((pObj->Data1 - pObj->BDInfo.Step) >= pObj->BDInfo.Min)
                {
                    pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
                    r = 1;
                }
            }
            break;
        case GUI_ID_BUTTON4://Tslope
            if(pObj->Data1 - pObj->BDInfo.Step < pObj->BDInfo.Min)
            {
                strcpy(contorlText, cbdialog32string[TSLOPE_0]);
            }
            else
            {
                pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
                r = 1;
            }
            break;
        case GUI_ID_BUTTON5://Psense
            if(pObj->Data1 - pObj->BDInfo.Step<pObj->BDInfo.Min)
            {
                if(Parameter->TriggerMode)
                {
                    strcpy(contorlText, cbdialog32string[FSENSE_5]);
                }
                else
                {
//#if (LOGO_FLAG != HOSPITECH_LOGO)
//					strcpy(contorlText, cbdialog32string[PSENSE_0]);
//#else
					strcpy(contorlText, "Psens >= -20");
//#endif
                }
            }
            else
            {
                pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
                r = 1;
            }
            break;
	case GUI_ID_BUTTON6://PEEP
	    if(pObj->Data1 < 3)//sun add 20140627
            {
                pObj->Data1 = 0;
            }
            if(pObj->Data1 - pObj->BDInfo.Step < pObj->BDInfo.Min)
            {
                strcpy(contorlText, cbdialog32string[PEEP_0FF]);
                //PlanMenuType = TEXT_MENU;
                //Task = MEAU_UPDATE;
            }
            else
            {
                pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
                r = 1;
            }
	    break;
        case GUI_ID_BUTTON7:  //sun add 20140623
            if(pObj->Data1 - pObj->BDInfo.Step < pObj->BDInfo.Min)
            {
                strcpy(contorlText, cbdialog32string[ESENSE_5]);
            }
            else
            {
                pObj->Data1 -= pObj->BDInfo.Step;
                r = 1;
            }
            break;
        default:
            pObj->Data1 = pObj->Data1 - pObj->BDInfo.Step;
            if(pObj->Data1<pObj->BDInfo.Min)
            {
                pObj->Data1 = pObj->BDInfo.Min;
                r = 1;
            }
        }
    }
    return (r);
}
/***************************************************************************************************
* @brief  更新运行时间
* @para   
* @retval 
*/
void _UpdateOperationTime(WM_HWIN hWin)
{
	WM_HWIN hItem,tWin,hDialog;

	char * String;
	tWin = WM_GetDialogItem(hWin,GUI_ID_MULTIPAGE0); 
	hDialog = MULTIPAGE_GetWindow(tWin,0);
	if(hDialog)
	{
		hItem = WM_GetDialogItem(hDialog, GUI_ID_TEXT1);
		String = _Float2String((float)OperationTime.Hour,0);
		TEXT_SetText(hItem,String);
	}
}
/***************************************************************************************************
* @brief  
* @para   
* @retval 
*/
void _AssistFunctionIndication(void)
{
    static WM_HWIN hItem = 0;
    WM_MESSAGE Msg;
    if(AssistFunctionOperationFlag)
    {
        if(AssistFunctionOperationFlag&FREEZE_ACTIVE)
        {
            hItem = BUTTON_Create(360,61,24,24,0,WM_CF_SHOW);
            BUTTON_SetBitmapEx(hItem,0,&bmfreeze,0,0);
            AssistFunctionOperationFlag &= ~FREEZE_ACTIVE;
        }
        if(AssistFunctionOperationFlag&FREEZE_CANCEL)
        {
            GUI_RECT r;
            WM_SelectWindow(hItem);
            WM_GetWindowRect(&r);
            GUI_SetBkColor(0x393c39);
            GUI_Clear();
            WM_DeleteWindow(hItem);
            AssistFunctionOperationFlag &= ~FREEZE_CANCEL;
        }
    }
}

/*****************************************************************************
*函数功能：用于检测和更新设置界面的压力显示值
*
*
*****************************************************************************/
void UpDate_SettingMenu_Pressure(WM_HWIN hWin)
{
    WM_HWIN hDialog,tWin;
	char String[5] = {0};
	tWin = WM_GetDialogItem(hWin, GUI_ID_MULTIPAGE0);
    hDialog = MULTIPAGE_GetWindow(tWin,0);

    tWin = WM_GetDialogItem(hDialog,GUI_ID_TEXT10);
	if((Barameter_Pressure >= 0) && (Barameter_Pressure <= 999))   //djq add 20140304  设置大气压力值
	{
	    sprintf(String,"%3d",Barameter_Pressure);
	    TEXT_SetText(tWin,String);     
	}
    else
    {
        strcpy(String,"--");
        TEXT_SetText(tWin,String);
    }
    tWin = WM_GetDialogItem(hDialog,GUI_ID_TEXT13);
    if(RealtimePara.O2GasPressure >= 20)
    {
        sprintf(String,"%3d",RealtimePara.O2GasPressure);  //djq add 20140304 显示O2气源压力
        TEXT_SetText(tWin,String);
    }
    else
    {
        strcpy(String,"--");
        TEXT_SetText(tWin,String);
    }
    //djq add 20170407
    tWin = WM_GetDialogItem(hDialog,GUI_ID_TEXT17);
    if(RealtimePara.AirGasPressure >= 20)
    {
        sprintf(String,"%3d",RealtimePara.AirGasPressure);  //djq add 20140304 显示O2气源压力
        TEXT_SetText(tWin,String);
    }
    else
    {
        strcpy(String,"--");
        TEXT_SetText(tWin,String);
    }
}



/*****************************************************************************
*函数功能：
*
*sun add 20140523
*****************************************************************************/
void _ControlTextShow(char * str)
{
    WM_HWIN hWin=Menu_hWin.TextWin;

    if(hWin)
    {
        if(strlen(str) == 0)
        {
            WM_HideWindow(hWin);
        }
        else
        {
            WM_ShowWindow(hWin);
            //IMAGETEXT_SetText(hWin,str);
			BubbleCtrl_SetText(hWin,str);
        }         
    }
}

/****************************************************************************/
//函数功能：将当前的Ti转换成最接近的吸呼比，保存到BREATH_SET_PARA结构体中
//频率不变，Ti改变   
void TiConvertIE(int SettingValue, BREATH_SET_PARA *BreathPara)
{
    int Ti_Value = SettingValue;
    unsigned int Save_Ti = 0xffff,Save_IE = 0;
    int cnt = 0,tmp;
	int max_cnt = 25;//sizeof(IE_RatioTable)/sizeof(int);

    for(cnt = 0;cnt < max_cnt;cnt++)
    {
        if(cnt >= 6)
        {
            tmp = 10*60*(100)/((10+IE_RatioTable[cnt])*BreathPara->SettingRateValue*1.0)+0.5;//ylk modify 20190102
        }
        else
        {
            tmp = IE_RatioTable[cnt]*60*(100)/((10+IE_RatioTable[cnt])*BreathPara->SettingRateValue);
        }
        
        if(tmp - Ti_Value > 0)
        {
            if(tmp - Ti_Value <= Save_Ti)
            {
                Save_Ti = tmp - Ti_Value;
                Save_IE = cnt;
            }
        }
        else if(tmp - Ti_Value == 0)
        {
            Save_IE = cnt;
            break;
        }
        else 
        {
            if(Ti_Value - tmp <= Save_Ti)
            {
                Save_Ti = Ti_Value - tmp;
                Save_IE = cnt;
            }
        }
    }
	BreathPara->SettingIEValue = Save_IE;
}
/****************************************************************************/
//函数功能：将当前的Ti转换成最接近的吸呼比，保存到BREATH_SET_PARA结构体中
//mode :0:频率不变，IE改变   1：IE不变，频率改变
int IEConvertTi(int Mode,int SettingValue, BREATH_SET_PARA *BreathPara)
{
    int TiValue;
    if(Mode == 0)
    {
        if(SettingValue >= 6)
        {
            TiValue = 10*60*100/((10+IE_RatioTable[SettingValue])*BreathPara->SettingRateValue*1.0) + 0.5;//ylk modify 20190102 BeathSettingPara↓
        }
        else
        {
            TiValue = IE_RatioTable[SettingValue]*60*100/((10+IE_RatioTable[SettingValue])*BreathPara->SettingRateValue);
        }
    }
    else if(Mode == 1)
    {
        if(BreathPara->SettingIEValue >= 6)
        {
			TiValue = 10*60*100/((10+IE_RatioTable[BreathPara->SettingIEValue])*SettingValue);
        }
        else
        {
			TiValue = IE_RatioTable[BreathPara->SettingIEValue]*60*100/((10+IE_RatioTable[BreathPara->SettingIEValue])*SettingValue);
        }
    }
    return (TiValue);// - TiValue%10
}

/*****************************************************************************
*函数功能：用于调节通气控制参数过程中，得到流速、I:E和吸气时间字符串
*
*sun add 20140526
*****************************************************************************/
void Get_VentParaInfoString(int Disp_Type,float Data)
{
	char string[50] = {0};//zkq 20170504 增加数组大小防止俄语下死机

    switch(Disp_Type)
    {
    case MAX_FLOW:
		sprintf(string, "%s = %2.1f L/min", Sys_Lib_GetDispStr(DP_MAXFLOW), Data);
     break;
    case TINSP_TIME:
		sprintf(string, "%s = %2.1f s", Sys_Lib_GetDispStr(DP_INSPTIME), Data/100);
        break;
    case IE_RATIO:
        if(BeathSettingPara.SettingIEValue >= 6)
        {
			sprintf(string, "%s = 1:%2.1f", Sys_Lib_GetDispStr(DP_IE), Data);
        }
        else 
        {
			sprintf(string, "%s = %2.1f:1", Sys_Lib_GetDispStr(DP_IE), Data);
        }
        break;
    }
  
	strcpy(contorlText, string);
	memset(string,0,sizeof(string));
}

/*****************************************************************************
*函数功能：用于调节通气控制参数过程中，显示I:E字符串
*
*sun add 20140526
*****************************************************************************/
void Get_VentIEInfoString(int tinspData)
{
	char string[50] = {0};//zkq 20170504 增加数组大小防止俄语下死机
    float texpData = 0;
    
    texpData = (6000/BeathSettingPara.SettingSimvRateValue - tinspData)/100.0;
#ifdef LANGUAGE_CHINESE_FLAG
	if(tinspData <= texpData*100)
	{
		sprintf(string,"%s 1:%2.1f",Sys_Lib_GetDispStr(DP_IE),texpData*100/tinspData);
	}
	else
	{
		sprintf(string,"%s= %2.1f:1",Sys_Lib_GetDispStr(DP_IE),(float)tinspData/100.0/texpData);
	}
#else
	if(tinspData <= texpData*100)
	{
		sprintf(string,"I:E = 1:%2.1f",texpData*100/tinspData);
	}
	else
	{
		sprintf(string,"I:E = %2.1f:1",(float)tinspData/100.0/texpData);
	}

#endif
//     switch(LanguageType)
//     {  
// #ifdef LANGUAGE_CHINESE_FLAG
//     case 2:
//         if(tinspData <= texpData*100)
//         {
//             sprintf(string,"%s 1:%2.1f",Sys_Lib_GetDispStr(DP_IE),texpData*100/tinspData);
// 
//         }
//         else 
//         {
//             sprintf(string,"%s= %2.1f:1",Sys_Lib_GetDispStr(DP_IE),(float)tinspData/100.0/texpData);
//         }
//         break;
// #else
//     case 0://English
//         if(tinspData <= texpData*100)
//         {
//             sprintf(string,"I:E = 1:%2.1f",texpData*100/tinspData);
//         }
//         else 
//         {
//             sprintf(string,"I:E = %2.1f:1",(float)tinspData/100.0/texpData);
//         }
//         break;
//     case 1:
//         if(tinspData <= texpData*100)
//         {
//             sprintf(string,"I:E = 1:%2.1f",texpData*100/tinspData);
//         }
//         else 
//         {
//             sprintf(string,"I:E = %2.1f:1",(float)tinspData/100.0/texpData);
//         }
//         break;
//     case 2:
//         if(tinspData <= texpData*100)
//         {
//             sprintf(string,"I:E = 1:%2.1f",texpData*100/tinspData);
//         }
//         else 
//         {
//             sprintf(string,"I:E = %2.1f:1",(float)tinspData/100.0/texpData);
//         }
//         break;
// #endif
//     }


	strcpy(contorlText, string);
	memset(string,0,sizeof(string));
}
