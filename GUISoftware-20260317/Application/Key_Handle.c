#include "MainTask.h"
#include "Key_Handle.h"
#include "MULTIPAGE.h"
#include "Static_Menu_Creat.h"
#include "Alarm_handle.h"
#include "ButtonExCtrl.h"
#include "CO2_IRMA.h"

#include "AddFunETCO2.h"

extern int Wm_Exec_Flag;
//extern MESSAGE_QUEUE MsgQueue;
extern int MenuType;
extern BREATH_SET_PARA BeathSettingPara;
extern ALARM_SET_PARA AlarmSettingPara;
extern ALARM_WORK AlarmsWork;
extern int Task;
extern int StandbyState;
extern int LastStandbyState;
extern void SendOperationMessage(unsigned char type,unsigned char UsartPort);
extern void BeepAloudControl(unsigned char status);
extern void SendSetModes(int modes);
extern void RTCSetTime(RTC_TIMER Time);
extern void VolumeChannelSelect(int data);
ALARM_INFO GetHighestAlarm(Int64U Alarms,Int64U Alarms1);
extern Int8U AlarmEventHandle(Int8U alarmEvent);
void CancelSuctionFunction(void);
extern MENU_HWIN Menu_hWin;
extern RTC_TIMER time;
extern int MenuOperateStatus;
extern int CalibrationType;
unsigned char TestSpeakerVoiceFlag = FALSE;
unsigned short int  LedUpdateRegister = 0;
extern unsigned int AssistFunctionOperationFlag;
KEY_MSG KeyMsg;
SPEAKER_TEST SpeakerState;
extern unsigned short int InspHoldState = 0xFF00;
extern unsigned short int ExpHoldState = 0xFF00;
extern unsigned short int SuctionState = 0xFF00;
extern int SuctionTimer;
extern int PlanMenuType;
extern int BackUpMode;
extern unsigned char Start_Standby_Flag;
extern unsigned char StandbyClear_FLag;
//sun add 20151102
extern char		Usart2_Tx_Buffer[20];

extern Int8U CHECK_DEBUG;
extern unsigned char EtCO2ModuleOff;

extern unsigned char SpO2_Config_On;  //0:关闭   1：开启
extern unsigned char Masimo_SpO2_Config_On; //0:关闭   1：开启
extern unsigned char EtCO2_Config_On; //0:关闭   1：开启
extern unsigned char EtCO2_KM7002_Config_On;
extern unsigned char Masimo_EtCO2_Config_On; //0:关闭   1：开启
extern unsigned char MasimoISA_EtCO2_Config_On;

extern CHINACO2_InitTypedef ChinaCO2_Init;
extern Int16U AlarmPriorityIT;
/***************************************************************************************************
*               Knob_Handle
*
*       功能：knob operation handle
*       输出：
***************************************************************************************************/
void Knob_Handle(int KeyValue)
{
    WM_HWIN i;
    WM_MESSAGE Msg;
    if((KeyValue==GUI_KEY_LEFT)|(KeyValue==GUI_KEY_RIGHT)|(KeyValue==GUI_KEY_ENTER))
    {
        i=WM_GetFocussedWindow();
        //WM_GetParent()
        Msg.MsgId   = WM_NOTIFY_PARENT;
        Msg.Data.v  = KeyValue;
        Msg.hWin = i;
        WM_SendToParent(i,&Msg);
    }
}

/***************************************************************************************************
*               Knob_Handle
*
*       功能：knob operation handle
*       输出：
***************************************************************************************************/
void Function_Key_Handle(int KeyValue)
{
    ALARM_INFO alarm;
    WM_HWIN hItem7;
    static char AlarmButton_State = 0,AlarmButton_StateOld = 0;
    //if(KeyValue==GUI_KEY_SILENCE)
    if(KeyValue == 2)
    {
        AlarmEventHandle(ALARM_RESET);
        
        if(AlarmsWork.presentAlarms || AlarmsWork.presentAlarms1)
        {
            alarm = GetHighestAlarm(AlarmsWork.presentAlarms,AlarmsWork.presentAlarms1);
			if(alarm.Grade == 1) //高级报警
			{
				if(AlarmsWork.BeepState==TRUE)
				{
					AlarmsWork.BeepState=FALSE;
					Menu_hWin.InfoWin.InfoChangeFlag |= SILENCE_INFO_CHANGED_FLAG;
					AlarmsWork.MuteTimer = 120;
					//send message to hit led
					LedUpdateRegister |= MUTE_LED;
					SendOperationMessage(LED_UPDATE_OPERATION,USART3);
					SendOperationMessage(BUZZER_CONTROL_OPERATION,USART3); //  20130514
					BeepAloudControl(0);
				}
			}
			else if(alarm.Grade == 2)	//中级报警
			{
				if(AlarmsWork.BeepState==TRUE)
				{
					AlarmsWork.BeepState=FALSE;
					Menu_hWin.InfoWin.InfoChangeFlag |= SILENCE_INFO_CHANGED_FLAG;
					AlarmsWork.MuteTimer = 120;
					//send message to hit led
					LedUpdateRegister |= MUTE_LED;
					SendOperationMessage(LED_UPDATE_OPERATION,USART3);
					SendOperationMessage(BUZZER_CONTROL_OPERATION,USART3); //  20130514
					BeepAloudControl(0);
				}
			}
			else //低级报警
			{	
				BUTTON_SetPressed(WM_GetDialogItem(Menu_hWin.hWin, GUI_ID_BUTTON4),0);
			}
        }
        else
        {
            BUTTON_SetPressed(WM_GetDialogItem(Menu_hWin.hWin, GUI_ID_BUTTON4),0);
        }
        Start_Standby_Flag = 0;
    }

    else if(KeyValue == 1)
    {
        hItem7 = WM_GetDialogItem(Menu_hWin.hWin, GUI_ID_BUTTON7);
        if(  (!(MenuOperateStatus & FRAMEWIN_POP))
          && (!(MenuOperateStatus & CALIBRATION_PRECEED)) )
        {
            if(StandbyState==0xFF)
            {
                if(!(Task & MEAU_UPDATE))//djq  add 20140214 
                {
                    LastStandbyState = StandbyState;
                    PlanMenuType = MAIN_MENU;
                    Task |=MEAU_UPDATE;
                    if(BeathSettingPara.BreathMode == PSV_MODE)
                    {
                        BackUpMode = 0;
                        Menu_hWin.InfoWin.InfoChangeFlag |= MODES_INFO_CHANGED_FLAG;
                    }
                    if(EtCO2ModuleOff)
                    {
                        if( (Masimo_EtCO2_Config_On == 1) &&(AA_Module_Flag.EtCO2ModuleExist == 2) )
                        {
                            DataTransmit(0x02,IM_SET_MODE,Usart2_Tx_Buffer,USART2);//設置phasein運行模式為测量模式 sy add 20140604
                        }
                        else if( (MasimoISA_EtCO2_Config_On) && (AA_Module_Flag.EtCO2ModuleExist == 2) )
// 						else if((Drager_EtCO2_Config_On) || (MasimoISA_EtCO2_Config_On))
                        {
                            SendOperationMessage(SET_AA_PUMPON,USART2);
                        }
                        else if((AA_Module_Flag.EtCO2ModuleExist == 1) && (EtCO2_Config_On || EtCO2_KM7002_Config_On))
                        {
                            ChinaCO2_Init.ChinaCo2_Flag = 0x01;//停止计数
                            ChinaCO2_Init.ChinaCo2_Count = 0x00;
                            ChinaCO2_Init.ChinaCo2_State = 0x01;
                            SendOperationMessage(CO2_MODULEON,USART2);//工作开泵                      
                        }
                    }
                }
                BUTTON_SetText(hItem7, Sys_Lib_GetDispStr(DP_STANDBY));
                FunctionKeyPressedResume(Menu_hWin.hWin,0);

// 				Menu_hWin.InfoWin.InfoChangeFlag |= CPB_CHANGE_FLAG;
            }
            else
            {
                if(MenuType!=ACTIVATE_STANDBY_MENU)
                {
                    if(!(Task & MEAU_UPDATE))//djq  add 20140214
                    {
                        PlanMenuType = ACTIVATE_STANDBY_MENU;
                        Task |=MEAU_UPDATE;
                        FunctionKeyPressedResume(Menu_hWin.hWin,0);
                    }
                    BUTTON_SetPressed(hItem7,1);//#1224
                }
            }
        }
        
        Start_Standby_Flag = 0;
    }
    else//检测静音按键状态。  djq add 20170119修改报警静音
    {
        if( (AlarmEventHandle(ALARM_RESET_INQUIRE) == 1) \
			|| ((AlarmsWork.unclearedAlarms)&&(!AlarmsWork.presentAlarms)) \
			|| ((AlarmsWork.unclearedAlarms1)&&(!AlarmsWork.presentAlarms1)))
        {
            WM_EnableWindow(WM_GetDialogItem(Menu_hWin.hWin, GUI_ID_BUTTON4));
            BUTTON_SetText(WM_GetDialogItem(Menu_hWin.hWin, GUI_ID_BUTTON4),Sys_Lib_GetDispStr(DP_ALARMRESET));//sun add 20140514
        }
        else if( (AlarmEventHandle(ALARM_RESET_INQUIRE) == 2) \
			|| ((AlarmsWork.unclearedAlarms)&&(!AlarmsWork.presentAlarms)) \
			|| ((AlarmsWork.unclearedAlarms1)&&(!AlarmsWork.presentAlarms1)))//zkq modify 20170306
        {
            WM_DisableWindow(WM_GetDialogItem(Menu_hWin.hWin, GUI_ID_BUTTON4));
            BUTTON_SetText(WM_GetDialogItem(Menu_hWin.hWin, GUI_ID_BUTTON4),Sys_Lib_GetDispStr(DP_ALARMRESET));//sun add 20140514
        }
        else
        {
            if((!AlarmsWork.presentAlarms) && (!AlarmsWork.presentAlarms1))//不存在报警
            {
                WM_DisableWindow(WM_GetDialogItem(Menu_hWin.hWin, GUI_ID_BUTTON4));                
            }
            else
            {
#if(LOGO_FLAG == PENLON_LOGO)//Penlon下产生氧气气源压力低报警，禁止报警静音按钮 //ylk add 20190711
				if((AlarmsWork.presentAlarms & ALARM_OXY_SOURCE_LOW) || (!AlarmPriorityIT))//当前无气源压力低报警
				{	
					WM_DisableWindow(WM_GetDialogItem(Menu_hWin.hWin, GUI_ID_BUTTON4));
				}
				else
				{
					WM_EnableWindow(WM_GetDialogItem(Menu_hWin.hWin, GUI_ID_BUTTON4));
				}
#else
				WM_EnableWindow(WM_GetDialogItem(Menu_hWin.hWin, GUI_ID_BUTTON4));
#endif
            }

            BUTTON_SetText(WM_GetDialogItem(Menu_hWin.hWin, GUI_ID_BUTTON4),Sys_Lib_GetDispStr(DP_ALARMSILENCE));//sun add 20140514   
        }
        AlarmButton_StateOld = AlarmButton_State;
    }
}
/**************************************************************************************************/

void CancelSuctionFunction(void)
{
    KeyMsg.Key_O2_Suction_flag = 0;
    LedUpdateRegister &= ~OXY_SUCTION_LED;
    SendOperationMessage(LED_UPDATE_OPERATION,USART3);
    SuctionState = 0xFF00;
    SendOperationMessage(SUCTION_CANCEL,USART0);
    SuctionTimer = 0;
}
