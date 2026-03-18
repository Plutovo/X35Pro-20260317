#include "MainTask.h"
#include "Alarm_handle.h"
#include "Static_Menu_Creat.h"
#include "JM1_SpO2.h"
#include "C002_CO2.h"
#include "CO2_IRMA.h"
#include "AddFunETCO2.H" 
#include "MasimoSPO2.h"
#include "DispStrs.h"
#include "WIDGET_Tools.h"
#include "Demo_AlarmTest.h"

struct list_node    alarmlog_node[MAX_RECORD_NUM] POOL_MEM;
struct list_head    alarmlog_head;                 //djq add 2013-12-24
void AlarmList_Inster(struct list_head *alarm_head,Int8U type);//djq add 2013-12-24
const char * searchAAalarm(Int32U type, Int8U AAId);
int AlarmList_TimeCompare(RTC_TIMER Time1,RTC_TIMER Time2);
int AlarmList_RankCompare(struct list_node *Alarm1,struct list_node *Alarm2);
void AlarmList_SetStateNum(struct list_node *node,Int8U Num);
Int8U AlarmList_GetStateNum(struct list_node *node);
void AlarmList_SetActive(struct list_node *node,Int8U Active);
Int8U AlarmList_GetActive(struct list_node *node);
Int8U AlarmList_CleanActive(struct list_head *alarm_head,Int64U AlarmHigh,Int64U AlarmLow);
extern WM_HWIN MuteFlag; //sun add 20140519

//电池报警及电量显示相关参数
extern BATTERY_AD_PARA  BatteryPara;

extern int BatteryStateNum;
//
extern ALARM_SET_PARA AlarmSettingPara;
extern BREATH_SET_PARA BeathSettingPara;
extern ALARM_WORK AlarmsWork;
extern ALARM_INFO ShowedAlarmInfo;
extern REALTIME_PARA RealtimePara;
extern MONITOR_PARA MonitorPara;
extern MENU_HWIN Menu_hWin;
extern int StandbyState;
extern int Task;
extern int LastStandbyState;
//extern int IndicateLedState;
//extern int LedUpdateSendMesg;
//
extern unsigned char SpO2ModuleExist;
extern unsigned char SpO2ModuleOff;
//extern unsigned char AA_Module_Flag.EtCO2ModuleExist;
extern unsigned char EtCO2ModuleOff;
extern unsigned char O2ModuleOff;
extern unsigned char FiO2_Config_On;  //氧浓度传感器  1：on  0：off // FiO2选配


extern unsigned char SpO2_Config_On;  //0:关闭   1：开启
extern unsigned char Masimo_SpO2_Config_On; //0:关闭   1：开启
extern unsigned char EtCO2_Config_On; //0:关闭   1：开启
extern unsigned char EtCO2_KM7002_Config_On; //0:关闭   1：开启
extern unsigned char Masimo_EtCO2_Config_On; //0:关闭   1：开启
extern unsigned char MasimoISA_EtCO2_Config_On; //0:关闭   1：ISA	2:ISA+O2 
extern unsigned char DirverGasFlag;//驱动气体类型标志
extern unsigned char DriverGas_Chose_Flag; //0: off  1:on  djq add 20170609驱动气选配标志
extern unsigned char AlarmAirSourceSwitch;//0：off  1：On

unsigned char alarm_ACGO_Flag = 0;
//报警清除标志，如果报警被清除则不在检测
unsigned char O2ModuleClear_FLag = 0;
unsigned char SpO2ModuleClear_FLag = 0;
unsigned char EtCO2ModuleClear_Flag = 0;
//unsigned char ACGOClear_FLag = 0;
unsigned char StandbyClear_FLag = 1;

extern JM1_SPO2_PARAMETERS SpO2_Parameters;
extern C002_CO2_PARAMETERS CO2_Parameters;
//extern int AcgoModeFlag;
extern Int8U CHECK_DEBUG;
extern unsigned char SpO2_Choose;
unsigned char HeartBeatFlag = 0;
//
extern int MenuType;
extern Int16U  LedUpdateRegister;
extern int CheckAcStatus(void);
extern unsigned char CheckBatteryStatus(void);
struct list_node AlarmRecord;
ALARM_RECORD_LIST AlarmRecordList;
extern RTC_TIMER RTCTime;
//
int BackUpMode;

//sun add 20151102
Int16U phaseinAlarm = 0;  //phasein 技术报警标志
extern Int8U   ETCO2_FLAG;
extern IM_ALL_DATA      * pAll_Data;
extern AA_PARAMETERS AA_Para;
//
Int16U AlarmPriority = 0;
Int16U AlarmPriorityIT = 0;
Int16U LowAlarmPriority = 0;//sun add 20141224
Int16U GasSourceState = 0xFFFF;
Int16U Min_Limit = 0;//  20130325

Int16U BAT_Limit = 0;
extern unsigned char Drager_EtCO2_Config_On;
extern unsigned char Paramagnetic_O2_Flag;
extern unsigned char N2O_Switch;
//
extern ALARM_SAVEVENT_PARA Alarm_SaveVent_para;
#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C)  //sun add 20140710
const Int64U AlarmType[ALARM_NUM] = //高级报警
{
	0x0000000000000001,	0x0000000000000002,	0x0000000000000004,	0x0000000000000008,
	0x0000000000000010,	0x0000000000000020,	0x0000000000000040,	0x0000000000000080,
	0x0000000000000100,	0x0000000000000200,	0x0000000000000400,	0x0000000000000800,
	0x0000000000001000,	0x0000000000002000,	0x0000000000004000,	0x0000000000008000,
	0x0000000000010000,	0x0000000000020000,	0x0000000000040000,	0x0000000000080000,
	0x0000000000100000,	0x0000000000200000,	0x0000000000400000,	0x0000000000800000,
	0x0000000001000000,	0x0000000002000000,	0x0000000004000000,	0x0000000008000000,
	0x0000000010000000,	0x0000000020000000,	0x0000000040000000,	0x0000000080000000,
	0x0000000100000000,	0x0000000200000000,	0x0000000400000000,	0x0000000800000000,
	0x0000001000000000,	0x0000002000000000, 0x0000004000000000
};
const Int64U AlarmType1[ALARM_NUM1] = //中级和低级报警
{ 
		0x00000001,  0x00000002,  0x00000004,  0x00000008,
		0x00000010,  0x00000020,  0x00000040,  0x00000080,
		0x00000100,  0x00000200,  0x00000400,  0x00000800,
		0x00001000,  0x00002000,  0x00004000,  0x00008000,
		0x00010000,  0x00020000,  0x00040000,  0x00080000,
		0x00100000,  0x00200000,  0x00400000,  0x00800000,
		0x01000000,  0x02000000,  0x04000000,  0x08000000,
		0x10000000,  0x20000000,  0x40000000,  0x80000000,

		0x100000000,  0x200000000,  0x400000000,  0x800000000,
		0x1000000000,  0x2000000000,  0x4000000000,  0x8000000000,
		0x10000000000,  0x20000000000,  0x40000000000,  0x80000000000,
		0x100000000000, 0x200000000000, 0x400000000000,
};
const Int8U AlarmRank1[ALARM_NUM1] =  //中级和低级报警
{
		2,           2,           2,           2,
		2,           2,           2,           2,
		2,           2,           2,           2,
		2,           2,           2,           2,
		2,			 2,           2,           2, 
		2,           2,           2,           2,
		2,           2,           2,           2,
		2,           2,           2,           2,
		2,           2,           2,           2,
		2,           4,           4,           4,       
		4,           4,           4,           4,
		4,		      4,  	        4,           
};
#else
const Int64U AlarmType[ALARM_NUM] = //高级报警
{ 
		0x00000001,  0x00000002,  0x00000004,  0x00000008,
		0x00000010,  0x00000020,  0x00000040,  0x00000080,
		0x00000100,  0x00000200,  0x00000400,  0x00000800,
		0x00001000,  0x00002000,  0x00004000,  0x00008000,

		0x00010000,  0x00020000,  0x00040000,  0x00080000,
		0x00100000
};
const Int64U AlarmType1[ALARM_NUM1] = //中级和低级报警
{ 
		0x00000001,  0x00000002,  0x00000004,  0x00000008,
		0x00000010,  0x00000020,  0x00000040,  0x00000080,
		0x00000100,  0x00000200,  0x00000400,  0x00000800,
		0x00001000,  0x00002000
};
const Int8U AlarmRank1[ALARM_NUM1] =  //中级和低级报警
{
		2,           2,           2,           2,
		2,           2,           2,		   2,
		4,           4,           4,           4,
		4,           4
};
#endif

//extern int PowerState;
void WriteAlarmRecordInfo(struct list_node *Record,Int8U type,Int16U num);
void InvertData2Info(struct list_node Info,char Ptext1[]);
void GetAlarmRecordInfo(WM_HWIN hItem,struct list_node *tmp_node,char Pointer[]);
void InquiryAlarmRecordInfo(Int8U type);
ALARM_INFO GetHighestAlarm(Int64U Alarms,Int64U Alarms1);
extern void  RTEEPROMwrite(Int16U addr, Int8U databyte);
extern Int8U RTEEPROMread(Int16U addr);
void WriteAlarmRecordHeadInfo(ALARM_RECORD_LIST *ListHead);
const char * searchAAalarm(Int32U type, Int8U AAId);
extern int TriggerHappenFlag = 0;
unsigned char InspFlag = 0;

void UpdataSystemSettingMenu(void)
{
    WM_HWIN hWin;

    if(MenuType==SYSTEM_SETTING_MENU)
    {
        if(MULTIPAGE_GetSelection(WM_GetDialogItem(Menu_hWin.ActivedWin,GUI_ID_MULTIPAGE0))==1)//更新驱动气体按键
        {
            hWin = WM_GetDialogItem(Menu_hWin.ActivedWin,GUI_ID_MULTIPAGE0);
            hWin = MULTIPAGE_GetWindow(hWin,1);
            WM_SendMessageNoPara(hWin, WM_INIT_DIALOG);
        }
    }
}

/***************************************************************************************************
* @brief  get the battery volume according to battery voltage
* @para   
* @retval 
*/
Int8U GetBatteryVolume(Int16U BatteryVoltage)//ADC数值转换为显示的电池的格数
{
    static  Int8U VolumeRank = 0;
    static  Int8U LastVolumeRank = 1;
    if (BatteryVoltage >= BatteryPara.Battery_Step3)//single cell 1.25v//柳
    {
        if(LastVolumeRank <= 4)//3格电
        {
            VolumeRank = 4;
        }
        LastVolumeRank = 4;
    }
    else if (BatteryVoltage>= BatteryPara.Battery_Step2)//柳
    {
        if(LastVolumeRank <= 3)//2格电
        {
            VolumeRank = 3;
        }
        LastVolumeRank = 3;
    }
    else if(BatteryVoltage >= BatteryPara.Battery_Low)//显示1格点
    {
        if(LastVolumeRank <= 2)
        {
            VolumeRank = 2;
        }
        LastVolumeRank = 2;
    }
    else if(BatteryVoltage >= BatteryPara.Battery_Empty)//低于电池电量低显示0格电
    {
        if(LastVolumeRank <= 1)
        {
            VolumeRank = 1;
        }
        LastVolumeRank = 1;
    }
    else if(BatteryVoltage >= BatteryPara.Battery_Disconnect)//未连接电池
	{
		VolumeRank	   = -1;
		LastVolumeRank = -1;
	}
    else
    {
        VolumeRank = 0;
        LastVolumeRank = 0;
    }
    return(VolumeRank);
}

/***************************************************************************************************
* @brief    屏幕显示报警信息
* @para
* @retval
*/
int InquireAlarmInfoWindows(void)
{
	ALARM_INFO alarm;
	int flag = 0;
	if((AlarmsWork.presentAlarms!=0) || (AlarmsWork.presentAlarms1!=0))//如果当前存在报警
	{
		alarm = GetHighestAlarm(AlarmsWork.presentAlarms,AlarmsWork.presentAlarms1);
		ShowedAlarmInfo.Content = alarm.Content;
		flag = 1;
	}
	else
	{
		AlarmsWork.unclearedAlarms1 &= ~ALARM_NOTLATCH;//低级报警不能锁存
		if((AlarmsWork.unclearedAlarms!=0) || (AlarmsWork.unclearedAlarms1!=0))
		{
			alarm = GetHighestAlarm(AlarmsWork.unclearedAlarms,AlarmsWork.unclearedAlarms1);
			ShowedAlarmInfo.Content = alarm.Content;
			flag = 1;
		}
	}
    return(flag);
}
/***************************************************************************************************
* @brief  get the battery volume according to battery voltage
* @para
* @retval
*/
void UpdateAlarmLed(Int16U Priority)
{
    if (((Priority&LOW_PRIORITY)!=0)|((Priority&MEDIUM_PRIORITY)!=0))
    {
        LedUpdateRegister |= LOW_PRIORITY_ALARM_LED;
    }
    else
    {
        LedUpdateRegister &= ~LOW_PRIORITY_ALARM_LED;
    }
    if(Priority&HIGH_PRIORITY)
    {
        LedUpdateRegister |= HIGH_PRIORITY_ALARM_LED;
    }
    else
    {
        LedUpdateRegister &= ~HIGH_PRIORITY_ALARM_LED;
    }
    SendOperationMessage(LED_UPDATE_OPERATION,USART3);
}
Int8U GetAAId(Int8U num) //sun add 20140610
{
	int cnt = 0;
	struct list_node *tmp_node = alarmlog_head.head; 

	for(cnt = 0;cnt < num - 1;cnt++)
	{
		tmp_node = tmp_node->next;  
	}
	return tmp_node->AlarmAA_ID;
}

/***************************************************************************************************
* @brief  get the alarm rank , content from the Highest alarm
* @para
* @retval
*/
unsigned int GetAlarmNum(Int64U Alarms,Int64U Alarms1)
{
	unsigned int num = 0;
	unsigned char i = 0;
	Int64U tmp = 0;
	Int64U j = 1;

	for(i=0;i<64;i++)
	{
		tmp  = j<<i;
		if(Alarms&tmp)
		{
			num++;
		}
		if(Alarms1&tmp)
		{
			num++;
		}
	}
	return num;
}

ALARM_INFO GetHighestAlarm(Int64U Alarms,Int64U Alarms1) //(Int32U Alarms) //cpp md 20130708
{
	ALARM_INFO alarm = {0};
	Int8U AAId = 0;
	char ** t;
	const char *context;

	int i=0;
	if (Alarms)
	{
		alarm.Type=HIGHEST_ALARM;
		while(!(Alarms & (alarm.Type<<i)))
		{
			i++;
		}
		alarm.Type = AlarmType[i];
		alarm.Grade = 1;

#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C)
		if(((((Int64U)1)<<i)==ALARM_ETAA_HIGH)
		 ||((((Int64U)1)<<i)==ALARM_FIAA_HIGH))
		{
// 			context = searchAAalarm(i, pAll_Data->slow.GenVal.ax1_id); //sun add 20140610
			context = searchAAalarm(i, AA_all_data.AA_detail.AA_AA1.ID);
		}
		else
#endif
		{
			context = AlarmContent[i];
		}
		t=&context; //sun add 20140612
		alarm.Content = *t;
		alarm.ActiveState = TRUE;
	}
	else if(Alarms1)
	{
		alarm.Type=HIGHEST_ALARM;
		while(!(Alarms1 & (alarm.Type<<i)))
		{
			i++;
		} 
		alarm.Type = AlarmType1[i];
		alarm.Grade = AlarmRank1[i];

#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C)
		if(((((Int64U)1)<<i)==ALARM_ETAA_LOW)
			||((((Int64U)1)<<i)==ALARM_FIAA_LOW))
		{
// 			context = searchAAalarm(i, pAll_Data->slow.GenVal.ax1_id);
			context = searchAAalarm(i, AA_all_data.AA_detail.AA_AA1.ID);
		}
		else
#endif
		{
			context = AlarmContent1[i];//
		}
		t=&context;
		alarm.Content = *t;
		alarm.ActiveState = TRUE;
	}
	return(alarm);
}
/***************************************************************************************************
* @brief  查询当前报警的优先级
* @para
* @retval
*/
Int16U InquirePresentAlarmsPriority(Int64U Alarms,Int64U Alarms1) //(Int32U Alarms) //cpp md 20130708
{
	Int16U r = 0;
	Int8U i;
	if(Alarms)
	{
		r |= 1;
	}
	if(Alarms1)
	{
		for(i=0; i<ALARM_NUM1; i++) //30 cpp md 20130708
		{
			if((Alarms1 & (HIGHEST_ALARM<<i)))
			{
				r |= AlarmRank1[i];
			}
		}
	}
	return(r);
}
/***************************************************************************************************
* @brief  保存报警信息的细节
* @para
* @retval
*/
void SaveAlarmInfo(Int64U Alarms,Int8U StrFlag) //(Int32U Alarms) //djq modify 20170113修改了AlarmRecord变量类型
{
	Int8U i = 0;
	Int64U CurrentAlarm = 0;
	while(Alarms)
	{
		CurrentAlarm = (((Int64U)1)<<i);
		if(Alarms&CurrentAlarm) //(1<<i)  //cpp md 20130709
		{
			AlarmRecord.RecordTime.RTC_Hour = RTCTime.RTC_Hour;
			AlarmRecord.RecordTime.RTC_Min  = RTCTime.RTC_Min;
			AlarmRecord.RecordTime.RTC_Mday = RTCTime.RTC_Mday;
			AlarmRecord.RecordTime.RTC_Mon  = RTCTime.RTC_Mon;
			AlarmRecord.RecordTime.RTC_Year = RTCTime.RTC_Year;

			AlarmRecord.AlarmType = i;

			AlarmList_SetActive(&AlarmRecord,1);
#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C)
			if((CurrentAlarm & ALARM_FIAA_HIGH)||(CurrentAlarm & ALARM_ETAA_HIGH)
				||(CurrentAlarm & ALARM_FIAA_LOW)||(CurrentAlarm & ALARM_ETAA_LOW)) //sun add 20140610
			{
				AlarmRecord.AlarmAA_ID = AA_all_data.AA_detail.AA_AA1.ID;
			}
			else
			{
				AlarmRecord.AlarmAA_ID = 0;
			}
#endif
			if(StrFlag)//中级和低级报警
			{
				AlarmRecord.AlarmAA_ID |= 0x80;
			}
			else//高级报警
			{
				AlarmRecord.AlarmAA_ID &= (~0x80);//zkq modify 20171211
			}

			InquiryAlarmRecordInfo(AlarmRecord.AlarmType);

		}
		Alarms &= ~(((Int64U)1)<<i); //(1<<i)  //cpp md 20130709
		i++;
	}
}
/***************************************************************************************************
* @brief  压力报警监测处理函数
* @para
* @retval
*/
#define PawRadio  (10)//ylk modify 20180426 (1-->10)
#define ALARM_CYCLE_MAX1    3    //3次呼吸
#define ALARM_CYCLE_MAX2	2
#define ALARM_TIME_MAX      100  //10s
#define ALARM_TIME_MIN		100

void AlarmCheckPaw(int pressure,int VentFlag)
{
	//呼吸周期计数器
	static  Int8U   PressureLowCount = 0; 
	static  Int8U   PressureHighCount = 0;
	//报警持续时间计数器0.1s
	static  Int8U   PLow_TimeCount = 0;
	static  Int8U   PHigh_TimeCount = 0;

	//------------------------------------------------------------------------------------------
	if (!(AlarmsWork.presentAlarms & ALARM_PAW_LOW))
	{
		if(AlarmSettingPara.PressureLowValue > 0)
		{
			if (pressure <= AlarmSettingPara.PressureLowValue*PawRadio)
			{
				if(VentFlag)
				{
					PressureLowCount++;
				}
				PLow_TimeCount ++;
				if(PressureLowCount>=ALARM_CYCLE_MAX1 && (PLow_TimeCount >= ALARM_TIME_MAX)) //check for 3 consecutive cycles
				{
					AlarmsWork.newAlarms |= ALARM_PAW_LOW;// alarm
					PressureLowCount = 0;
					PLow_TimeCount = 0;
					//20240517 lyy add 压力高报警和压力低报警互斥
					AlarmsWork.presentAlarms &= ~ALARM_PAW_HIGH;// STOP alarm
					PressureHighCount = 0;
					PHigh_TimeCount = 0;
				}
			}
			else
			{
				PressureLowCount = 0;
				PLow_TimeCount = 0;
			}
// 			if(RealtimePara.Pressure <= AlarmSettingPara.PressureLowValue*PawRadio)
// 			{
// 				PLow_TimeCount ++;
// 				if(PLow_TimeCount >= ALARM_TIME_MAX)
// 				{
// 					PressureLowCount = 0;
// 					PLow_TimeCount = 0;
// 					AlarmsWork.newAlarms |= ALARM_PAW_LOW;
// 					//20240517 lyy add 压力高报警和压力低报警互斥
// 					AlarmsWork.presentAlarms &= ~ALARM_PAW_HIGH;// STOP alarm
// 					PressureHighCount = 0;
// 					PHigh_TimeCount = 0;
// 				}
// 			}
// 			else
// 			{
// 				PLow_TimeCount = 0;
// 			}

		}
		else
		{
			AlarmsWork.presentAlarms  &=~ ALARM_PAW_LOW;//stop alarm
			PressureLowCount = 0;
			PLow_TimeCount = 0;
		}
	}
	else
	{
		if(RealtimePara.Pressure > AlarmSettingPara.PressureLowValue*PawRadio)
		{
			PLow_TimeCount++;
		}
		else
		{
			PLow_TimeCount = 0;
		}
		if((pressure > AlarmSettingPara.PressureLowValue*PawRadio)\
			||(AlarmSettingPara.PressureLowValue <= 0))
		{
			if(VentFlag)
			{
				PressureLowCount++;
			}
			if((PressureLowCount>=ALARM_CYCLE_MAX2) || (PLow_TimeCount >= ALARM_TIME_MIN))//check for 2 consecutive cycles
			{
				AlarmsWork.presentAlarms  &=~ ALARM_PAW_LOW;//stop alarm
				PressureLowCount = 0;
				PLow_TimeCount = 0;
			}
		}
		else
		{
			PressureLowCount = 0;
			PLow_TimeCount = 0;
		}
	}

	if (!(AlarmsWork.presentAlarms & ALARM_PAW_HIGH))
	{
		if (pressure >= AlarmSettingPara.PressureHighValue*PawRadio)
		{
			if(VentFlag)
			{
				PressureHighCount++;
			}
			if(PressureHighCount>=ALARM_CYCLE_MAX2)
			{
				PHigh_TimeCount = 0;
				PressureHighCount = 0;
				AlarmsWork.newAlarms |= ALARM_PAW_HIGH;
				//20240517 lyy add 压力高报警和压力低报警互斥
				AlarmsWork.presentAlarms  &=~ ALARM_PAW_LOW;//stop alarm
				PressureLowCount = 0;
				PLow_TimeCount = 0;

			}
		}
		else
		{
			PressureHighCount = 0;
			//PHigh_TimeCount = 0;
		}
		if(RealtimePara.Pressure >= AlarmSettingPara.PressureHighValue*PawRadio)
		{
			PHigh_TimeCount ++;
			if(PHigh_TimeCount >= ALARM_TIME_MAX)
			{
				PressureHighCount = 0;
				PHigh_TimeCount = 0;
				AlarmsWork.newAlarms |= ALARM_PAW_HIGH;
				//20240517 lyy add 压力高报警和压力低报警互斥
				AlarmsWork.presentAlarms  &=~ ALARM_PAW_LOW;//stop alarm
				PressureLowCount = 0;
				PLow_TimeCount = 0;
			}
		}
		else
		{
			PHigh_TimeCount = 0;
		}
	}
	else
	{
		if(RealtimePara.Pressure < AlarmSettingPara.PressureHighValue*PawRadio)
		{
			PHigh_TimeCount++;
		}
		else
		{
			PHigh_TimeCount = 0;
		}
		if(pressure < AlarmSettingPara.PressureHighValue*PawRadio)
		{
			if(VentFlag)
			{
				PressureHighCount++;
			}
		}
		else
		{
			PressureHighCount = 0;
			PHigh_TimeCount = 0;
		}
		if((PressureHighCount>=ALARM_CYCLE_MAX1) || (PHigh_TimeCount >= ALARM_TIME_MIN))//ylk modify 20180426 位置调整
		{
			AlarmsWork.presentAlarms &= ~ALARM_PAW_HIGH;// STOP alarm
			PressureHighCount = 0;
			PHigh_TimeCount = 0;
		}
	}
}
/***************************************************************************************************
* @brief  气道阻塞报警监测处理函数
* @para
* @retval
*/
void AlarmCheckContinuePressure(int pressure)
{
    static  Int8U PressureContinueHighCount = 0;

	if ((AlarmsWork.presentAlarms & ALARM_PAW_HIGH_CONTINUE) &&(LungRec_GetStartStopFlag()==1))
	{
		AlarmsWork.presentAlarms &= ~ALARM_PAW_HIGH_CONTINUE;// STOP alarm
		PressureContinueHighCount = 0;
	}
	if ((LungRec_GetStartStopFlag()==0))
	{
		if(!(AlarmsWork.presentAlarms & ALARM_PAW_HIGH_CONTINUE))
		{
			if (pressure >= (BeathSettingPara.SettingPEEPValue+15))
			{
				PressureContinueHighCount ++;
				if(PressureContinueHighCount>=PEEP_MEASURE_TIME)
				{
					AlarmsWork.newAlarms |= ALARM_PAW_HIGH_CONTINUE;
					PressureContinueHighCount = 0;
				}
			}
			else
			{
				PressureContinueHighCount = 0;
			}
		}
		else
		{
			if (pressure < (BeathSettingPara.SettingPEEPValue+10))
			{
				AlarmsWork.presentAlarms &= ~ALARM_PAW_HIGH_CONTINUE;// STOP alarm
				PressureContinueHighCount = 0;
			}
		}
	}

}
/*************************************************************************************************** 
* @brief
* @para
* @retval
*/
void AlarmCheckPawN10(int pressure,int VentFlag) //  20130607
{
    static  Int8U PawN10Count = 0;
    if(VentFlag)
    {
        if(!(AlarmsWork.presentAlarms & ALARM_PAW_LESSTHAN_N10))
        {
            if(pressure < PAW_N10)
            {
                PawN10Count++;
                if(PawN10Count >= 3)
                {
                    AlarmsWork.newAlarms |= ALARM_PAW_LESSTHAN_N10;
                    PawN10Count = 0;
                }
            }
            else
            {
                PawN10Count = 0;
            }
        }
        else
        {
            if(pressure >= -10)
            {
                PawN10Count++;
                if(PawN10Count >= 2)
                {
                    AlarmsWork.presentAlarms &= ~ALARM_PAW_LESSTHAN_N10;
                    PawN10Count = 0;
                }
            }
            else
            {
                PawN10Count = 0;
            }
        }
    }
}

/***************************************************************************************************
* @brief  检查分钟通气量报警的处理函数
* @para
* @retval
*/
void AlarmCheckMV(int mvData,Int8U Vent_Flag)
{
    static Int8U count0 = 0;
    static Int8U count1 = 0;

#ifdef ACGO_SWITCH
    if(alarm_ACGO_Flag == 1)
    {
        count0 = 0;
        count1 = 0;
        alarm_ACGO_Flag = 0;
    }
#endif
	if((Alarm_SaveVent_para.ALARM_ValUpdateFlag&(1<<1)) == (1<<1))
	{
		Alarm_SaveVent_para.ALARM_ValUpdateFlag &= ~(1<<1);
		count0 = 0;
		count1 = 0;
	}

    if(Vent_Flag==TRUE)
    {
        if (!(AlarmsWork.presentAlarms & ALARM_MV_HIGH))
        {
            if (mvData >= AlarmSettingPara.MVHighValue*100)
            {
                count0++;
                if(count0>=10)
                {
                    AlarmsWork.newAlarms |= ALARM_MV_HIGH;// alarm
                    count0 = 0;
                }
            }
            else
            {
                count0 = 0;
            }
        }
        else
        {
            if(mvData<AlarmSettingPara.MVHighValue*100)
            {
                count0++;
                if(count0>=5)
                {
                    AlarmsWork.presentAlarms &= ~ALARM_MV_HIGH;// stop mv low alarm
                    count0 = 0;
                }
            }
            else
            {
                //count0++;//sun del 20150108
				count0 = 0;//sun add 20150108
            }
        }

		if (LungRec_GetStartStopFlag() == 0)
		{
			if (!(AlarmsWork.presentAlarms & ALARM_MV_LOW))
			{
				if( AlarmSettingPara.MVLowValue > 0 )//  if...条件 20130325
				{
					if (mvData <= AlarmSettingPara.MVLowValue*100)
					{
						count1++;
						if(count1>=10) //check for 10 consecutive cycles
						{
							AlarmsWork.newAlarms |= ALARM_MV_LOW;
							count1 = 0;
						}
					}
					else
					{
						count1 = 0;
					}
				}
				else //  else...内容 20130325
				{
					AlarmsWork.presentAlarms &= ~ALARM_MV_LOW;// stop alarm
					count1 = 0;
				}
			}
			else
			{
				if( (mvData > AlarmSettingPara.MVLowValue*100)
				||( AlarmSettingPara.MVLowValue <= 0 ))
				{
					count1++;
					if(count1>=5)
					{
						AlarmsWork.presentAlarms &= ~ALARM_MV_LOW;// stop alarm
						count1 = 0;
					}
				}
				else
				{
					count1 = 0;
				}
			}

		}
		else
		{
			if (AlarmsWork.presentAlarms & ALARM_MV_LOW)
			{
				AlarmsWork.presentAlarms &= ~ALARM_MV_LOW;// stop alarm
				count1 = 0;
				//timerCount1 = 0;
			}
		}
    }
}
/***************************************************************************************************
* @brief  潮气量高报警的监测处理函数
* @para
* @retval
*/
void AlarmCheckVTE(Int16U Vte,Int8U Vent_Flag)
{
    static Int8U count0 = 0;
    static Int8U count1 = 0;

	if((Alarm_SaveVent_para.ALARM_ValUpdateFlag&(1<<2)) == (1<<2))
	{
		Alarm_SaveVent_para.ALARM_ValUpdateFlag &= ~(1<<2);
		count0 = 0;
		count1 = 0;
	}

    if(Vent_Flag==TRUE)
    {
        if(!(AlarmsWork.presentAlarms & ALARM_VTE_HIGH))
        {
            if(Vte >= AlarmSettingPara.VteHighValue)
            {
                count0++;
                if(count0>=4)//check for 4 consecutive cycles
                {
                    AlarmsWork.newAlarms |= ALARM_VTE_HIGH;// alarm
                    count0 = 0;
                }
            }
            else
            {
                count0 = 0;
            }
        }
        else
        {
            if(Vte < AlarmSettingPara.VteHighValue)
            {
                count0++;
                if(count0>=4)//check for 4 consecutive cycles
                {
                    AlarmsWork.presentAlarms &= ~ALARM_VTE_HIGH;// stop alarm
                    count0 = 0;
                }
            }
            else
            {
                count0 = 0;
            }
        }
        if(!(AlarmsWork.presentAlarms1 & ALARM_VTE_LOW))
        {
            if((Vte <= AlarmSettingPara.VteLowValue)&(AlarmSettingPara.VteLowValue> 10))//  20130325
            {
                count1++;
                if(count1>=4)//check for 4 consecutive cycles
                {
                    AlarmsWork.newAlarms1 |= ALARM_VTE_LOW;// alarm
                    count1 = 0;
                }
            }
            else
            {
                count1 = 0;
            }
        }
        else
        {
            if(AlarmSettingPara.VteLowValue> 10)//  20130325
            {
                if(Vte > AlarmSettingPara.VteLowValue)
                {
                    count1++;
                    if(count1>=4)//check for 4 consecutive cycles
                    {
                        AlarmsWork.presentAlarms1 &= ~ALARM_VTE_LOW;// stop alarm
                        count1 = 0;
                    }
                }
                else
                {
                    count1 = 0;
                }
            }
            else
            {
                if(AlarmsWork.presentAlarms1 & ALARM_VTE_LOW)
                {
                    AlarmsWork.presentAlarms1 &= ~ALARM_VTE_LOW;
                }
            }
        }
    }
}
/***************************************************************************************************
* @brief  呼吸频率总计报警的监测处理函数
* @para
* @retval
*/
void AlarmCheckRate(Int16U Rate,Int8U Vent_Flag)
{
    static Int8U count = 0;
    static Int8U count1 = 0;
#ifdef ACGO_SWITCH
    if(alarm_ACGO_Flag == 1)
    {
        count = 0;
        count1 = 0;
        //alarm_ACGO_Flag = 0;
    }
#endif
	if((Alarm_SaveVent_para.ALARM_ValUpdateFlag&(1<<3)) == (1<<3))
	{
		Alarm_SaveVent_para.ALARM_ValUpdateFlag &= ~(1<<3);
		count = 0;
		count1 = 0;
	}

	if ((LungRec_GetStartStopFlag()==1) && ((AlarmsWork.presentAlarms1 & ALARM_RATE_HIGH)||(AlarmsWork.presentAlarms & ALARM_RATE_LOW)))
	{
		AlarmsWork.presentAlarms &= ~ALARM_RATE_LOW;// stop alarm
		count1 = 0;
		AlarmsWork.presentAlarms1 &= ~ALARM_RATE_HIGH;// stop alarm
		count = 0;
	}

    if( (Vent_Flag==TRUE) && (LungRec_GetStartStopFlag()==0) )
    {
        if(!(AlarmsWork.presentAlarms1 & ALARM_RATE_HIGH))
        {
            if(Rate >= AlarmSettingPara.RateHighValue)
            {
                count++;
                if(count>=20)//check for 3 consecutive cycles
                {
                    AlarmsWork.newAlarms1 |= ALARM_RATE_HIGH;// alarm
                    count = 0;
                }
            }
        }
        else
        {
            if(Rate < AlarmSettingPara.RateHighValue)
            {
                count++;
                if(count>=3)//check for 3 consecutive cycles
                {
                    AlarmsWork.presentAlarms1 &= ~ALARM_RATE_HIGH;// stop alarm
                    count = 0;
                }
            }
            else
            {
                count = 0;
            }
        }
        if(!(AlarmsWork.presentAlarms & ALARM_RATE_LOW))
        {
            if(AlarmSettingPara.RateLowValue > 0)//  if...条件 20130325
            {
                if(Rate <= AlarmSettingPara.RateLowValue)
                {
                    count1++;
                    if(count1>=2)//check for 3 consecutive cycles
                    {
                        AlarmsWork.newAlarms |= ALARM_RATE_LOW;// alarm
                        count1 = 0;
                    }
                }
            }
            else//  else...内容 20130325
            {
                AlarmsWork.presentAlarms &= ~ALARM_RATE_LOW;// stop alarm
                count1 = 0;
            }
        }
        else//  else... 内容 20130325
        {
            if(AlarmSettingPara.RateLowValue <= 0)
            {
                AlarmsWork.presentAlarms &= ~ALARM_RATE_LOW;// stop alarm
                count1 = 0;
            }
            else
            {
                if(Rate > AlarmSettingPara.RateLowValue)
                {
                    count1++;
                    if(count1>=2)//check for 3 consecutive cycles
                    {
                        AlarmsWork.presentAlarms &= ~ALARM_RATE_LOW;// stop alarm
                        count1 = 0;
                    }
                }
                else
                {
                    count1 = 0;
                }
            }
        }
    }
}
/***************************************************************************************************
* @brief  窒息报警的监测处理函数
* @para
* @retval
*/
void AlarmCheckApnea(void)
{
    static Int16U apnea_time = 0, apnea_time2 = 0;
    if ((BeathSettingPara.BreathMode == PSV_MODE)&&(!CPB_GetFlag()) )
    {
        if(!(AlarmsWork.presentAlarms&ALARM_APNEA))
        {
            if(!TriggerHappenFlag)
            {
                apnea_time++;
                if (apnea_time >= AlarmSettingPara.ApneaTimerValue*10)
                {
                    AlarmsWork.newAlarms |= ALARM_APNEA;
                    apnea_time = 0;
                    //send message to tell the controller
                    SendOperationMessage(APNEA_INFORM,USART0);
                    //*//  20130325
//                    BeathSettingPara.BreathMode = BACKUP_MODE; //  20130128  //cpp del 20130513
                    BackUpMode = BACKUP_MODE;
                    Menu_hWin.InfoWin.InfoChangeFlag |= MODES_INFO_CHANGED_FLAG;
                    if(MenuType==MODES_MENU)
                    {
                        WM_HWIN hWin;
                        hWin = WM_GetDialogItem(Menu_hWin.ActivedWin, GUI_ID_MULTIPAGE0);
                        if(BeathSettingPara.ApnoeaMode==VCV_MODE)
                        {
                            hWin = WM_GetDialogItem(MULTIPAGE_GetWindow(hWin,1),GUI_ID_BUTTON4);
                            WM_DisableWindow(hWin);
                        }
                        else
                        {
                            hWin = WM_GetDialogItem(MULTIPAGE_GetWindow(hWin,1),GUI_ID_BUTTON3);
                            WM_DisableWindow(hWin);
                        }
                    }
                    //*///endof   ...
                }
            }
            else
            {
                apnea_time = 0;
                TriggerHappenFlag = 0;
            }
        }//endof  if(!(AlarmsWork.presentAlarms&ALARM_APNEA))
        else
        {
            if(TriggerHappenFlag)
            {
                TriggerHappenFlag = 0;
                apnea_time++;
                if(apnea_time>=2)
                {
                    AlarmsWork.presentAlarms &= ~ALARM_APNEA;
                    apnea_time = 0;
                    SendSetModes(BeathSettingPara.BreathMode);
                    //*//  20130325
                    BeathSettingPara.BreathMode = PSV_MODE; //  20130325 
                    BackUpMode = 0;
                    Menu_hWin.InfoWin.InfoChangeFlag |= MODES_INFO_CHANGED_FLAG;
                    if(MenuType==MODES_MENU)
                    {
                        WM_HWIN hWin;
                        hWin = WM_GetDialogItem(Menu_hWin.ActivedWin, GUI_ID_MULTIPAGE0);
                        if(BeathSettingPara.ApnoeaMode==VCV_MODE)
                        {
                            hWin = WM_GetDialogItem(MULTIPAGE_GetWindow(hWin,1),GUI_ID_BUTTON4);
                            WM_EnableWindow(hWin);
                        }
                        else
                        {
                            hWin = WM_GetDialogItem(MULTIPAGE_GetWindow(hWin,1),GUI_ID_BUTTON3);
                            WM_EnableWindow(hWin);
                        }
                    }
                    //*///endof   ...
                }
            }
        }
    }
	else if((BeathSettingPara.BreathMode == MANUAL_MODE)&&(!CPB_GetFlag()) )	//手动下窒息报警
	{
		if(!(AlarmsWork.presentAlarms&ALARM_APNEA))
		{
			if(!(InspFlag))
			{
				apnea_time2++;
				if (apnea_time2 >= AlarmSettingPara.ApneaTimerValue*10)
				{
					AlarmsWork.newAlarms |= ALARM_APNEA;
					apnea_time = 0;
					apnea_time2 = 0;
					Menu_hWin.InfoWin.InfoChangeFlag |= MODES_INFO_CHANGED_FLAG;
				}
			}
			else
			{
				apnea_time = 0;
				apnea_time2 = 0;
				InspFlag = 0;
			}
		}
		else
		{
			if( InspFlag)
			{
				InspFlag = 0;
				apnea_time2++;
				if(apnea_time2 >= 2)
				{
					AlarmsWork.presentAlarms &= ~ALARM_APNEA;
					apnea_time = 0;
					apnea_time2 = 0;
					//SendSetModes(BeathSettingPara.BreathMode, &BeathSettingPara);
					Menu_hWin.InfoWin.InfoChangeFlag |= MODES_INFO_CHANGED_FLAG;
				}
			}
		}
	}
    else
    {
        AlarmsWork.presentAlarms &= ~ALARM_APNEA;
        apnea_time = 0;
    }
}
/***************************************************************************************************
* @brief  氧浓度报警的监测处理函数
* @para
* @retval
*/
void AlarmCheckO2Sensor(Int16U FiO2_Value, Int8U O2_type)
{
	static Int8U O2FaultTime = 0, O2SenTime = 0;

	if(!O2ModuleOff)
	{
		if(!O2_type)//氧电池
		{
			if(!(AlarmsWork.presentAlarms & ALARM_FIO2_LESSTHAN_18))
			{
				if(O2_FAULT_COFIRM(FiO2_Value))
				{ 
					O2FaultTime++;
					if(O2FaultTime >= 100)
					{
						O2FaultTime = 0;
						AlarmsWork.newAlarms |= ALARM_FIO2_LESSTHAN_18;
						AlarmsWork.presentAlarms &= ~ALARM_FIO2_HIGH;
// 						AlarmsWork.presentAlarms &= ~ALARM_FIO2_LOW;
					}
				}
				else
				{
					O2FaultTime = 0;
				}
			}
			else
			{
				if(O2_FAULT_CANCEL(FiO2_Value))
				{
					O2FaultTime++;
					if (O2FaultTime >= 30)
					{
						O2FaultTime = 0;
						AlarmsWork.presentAlarms &= ~ALARM_FIO2_LESSTHAN_18;
					}
				}
				else
				{
					O2FaultTime = 0;
				}
			}
		}
		else //顺磁氧
		{
			if(!(AlarmsWork.presentAlarms1 & ALARM_GAS_COMM_FAULT))
			{
				if(!(AlarmsWork.presentAlarms & ALARM_FIO2_LESSTHAN_18))
				{
					//Masimo ISA O2 Err || Drager O2 Err
					if(((MasimoISA_EtCO2_Config_On == 2) && (AA_all_data.AA_state.module_err_message[1] & 0x01)) \
						||((Drager_EtCO2_Config_On == 2) && (AA_all_data.AA_state.module_err_message[4] & 0x40))) 
					{
						O2SenTime++;
						if(O2SenTime >= 100)
						{
							O2SenTime = 0;
							AlarmsWork.newAlarms |= ALARM_FIO2_LESSTHAN_18;
							AlarmsWork.presentAlarms &= ~ALARM_FIO2_HIGH;
							// 						AlarmsWork.presentAlarms &= ~ALARM_FIO2_LOW;
						}
					}
					else
					{
						O2SenTime = 0;
					}
				}
				else
				{
					if(((MasimoISA_EtCO2_Config_On == 2) && !(AA_all_data.AA_state.module_err_message[1] & 0x01)) \
						||((Drager_EtCO2_Config_On == 2) && !(AA_all_data.AA_state.module_err_message[4] & 0x40)))
					{
						O2SenTime++;
						if (O2SenTime >= 30)
						{
							O2SenTime = 0;
							AlarmsWork.presentAlarms &= ~ALARM_FIO2_LESSTHAN_18;
						}
					}
					else
					{
						O2SenTime = 0;
					}
				}
			}
			else
			{
				O2SenTime = 0;
				AlarmsWork.presentAlarms &= ~ALARM_FIO2_LESSTHAN_18;
			}
		}
	}
}
// void AlarmCheckO2Sensor(Int16U FiO2_Value, Int8U SensorType)
// {
//     static Int8U O2CellTime = 0, O2SenTime = 0;
// 
//     if(!O2ModuleOff)//开启   //djq add 20131226
//     {
// 		if(!SensorType)//氧电池
// 		{
// 			if(!(AlarmsWork.presentAlarms & ALARM_O2SENSOR_FAILURE)) //0≤未连接＜5    6≤故障＜15 ||  105≤故障
// 			{
// 				if(O2_SENSOR_FAILURE_COFIRM(FiO2_Value))
// 				{
// 					O2CellTime++;
// 					if(O2CellTime >= 100)// check for 5 consecutive cycles
// 					{
// 						AlarmsWork.newAlarms |= ALARM_O2SENSOR_FAILURE;
// 						//20170615 产生了传感器故障报警，就取消氧浓度高报警
// 						if(AlarmsWork.presentAlarms&ALARM_FIO2_HIGH)
// 						{
// 							AlarmsWork.presentAlarms &= ~ALARM_FIO2_HIGH;
// 						}
// 						O2CellTime = 0;
// 					}
// 				}
// 				else
// 				{
// 					O2CellTime = 0;
// 				}
// 			}
// 			else
// 			{
// 				if(O2_SENSOR_FAILURE_CANCEL(FiO2_Value))
// 				{
// 					O2CellTime++;
// 					if (O2CellTime >= 30)//check for 3 consecutive cycles
// 					{
// 						AlarmsWork.presentAlarms &= ~ALARM_O2SENSOR_FAILURE;
// 						O2CellTime = 0;
// 					}
// 				}
// 				else
// 				{
// 					O2CellTime = 0;
// 				}
// 			}
// 		}
// 		else	//顺磁氧
// 		{
// 			if(!(AlarmsWork.presentAlarms & ALARM_O2SENSOR_FAILURE))
// 			{
// //				if(Im_getSlowData(IM_GET_O2_ERR,0))
//                 if(((MasimoISA_EtCO2_Config_On == 2) && (AA_all_data.AA_state.module_err_message[1] & 0x01))\
// 				 ||((Drager_EtCO2_Config_On == 2)      && (AA_all_data.AA_state.module_err_message[4] & 0x40))) 
// 				{
// 					O2SenTime++;
// 					if(O2SenTime >= 100)
// 					{
// 						AlarmsWork.newAlarms |= ALARM_O2SENSOR_FAILURE;
// 						if(AlarmsWork.presentAlarms&ALARM_FIO2_HIGH)
// 						{
// 							AlarmsWork.presentAlarms &= ~ALARM_FIO2_HIGH;
// 						}
// 						O2SenTime = 0;
// 					}
// 				}
// 				else
// 				{
// 					O2SenTime = 0;
// 				}
// 			}
// 			else
// 			{
// //				if(!Im_getSlowData(IM_GET_O2_ERR,0))//ylk modify 20181203
//                 if(((MasimoISA_EtCO2_Config_On == 2) && !(AA_all_data.AA_state.module_err_message[1] & 0x01))\
// 				 ||((Drager_EtCO2_Config_On == 2)      && !(AA_all_data.AA_state.module_err_message[4] & 0x40))) 
// 				{
// 					O2SenTime++;
// 					if (O2SenTime >= 30)
// 					{
// 						AlarmsWork.presentAlarms &= ~ALARM_O2SENSOR_FAILURE;
// 						O2SenTime = 0;
// 					}
// 				}
// 				else
// 				{
// 					O2SenTime = 0;
// 				}
// 			}
// 		}
//     }
// }

void AlarmCheckFiO2(Int16U FiO2_Value,Int8U Vent_Flag)
{
    static Int16U FiO2LowAlarmTime = 0;
    static Int16U FiO2HighAlarmTime = 0;
	static Int16U FiO2LessThanTime = 0;

    if(!O2ModuleOff)//开启   //djq add 20131226
    {
		if(!(AlarmsWork.presentAlarms &ALARM_FIO2_LOW))
		{
			//只有在传感器故障没有产生的时候，才能报警
			if(FiO2_Value<AlarmSettingPara.FiO2LowValue)    //氧浓度小于报警下限
			{
#ifndef __debug
				FiO2LowAlarmTime++;              
				if(FiO2LowAlarmTime >= 100)// check for 10 consecutive cycles
				{
					AlarmsWork.newAlarms |= ALARM_FIO2_LOW;
					FiO2LowAlarmTime = 0;
				} 
#endif
			}
			else
			{
				FiO2LowAlarmTime = 0;
			}
		}
		else
		{
			//如果产生了氧浓度传感器故障，则取消报警
			if(FiO2_Value>=AlarmSettingPara.FiO2LowValue) 
			{
				FiO2LowAlarmTime++;
				if (FiO2LowAlarmTime >= 30)//check for 3 consecutive cycles
				{
					AlarmsWork.presentAlarms &= ~ALARM_FIO2_LOW;
					FiO2LowAlarmTime = 0;
				}
			}
			else
			{
				FiO2LowAlarmTime = 0;
			}
		}

		if(!(AlarmsWork.presentAlarms & ALARM_FIO2_LESSTHAN_18))
		{
			//------------------------------------------------------------------------------------------
			if(!(AlarmsWork.presentAlarms&ALARM_FIO2_HIGH))
			{
				if ((FiO2_Value >= AlarmSettingPara.FiO2HighValue)//氧浓度大于报警限制
					&&(AlarmSettingPara.FiO2HighValue != 101))
				{
					FiO2HighAlarmTime++;
					if(FiO2HighAlarmTime >= 100)// check for 10 consecutive cycles //cpp md 20130718
					{
						AlarmsWork.newAlarms |= ALARM_FIO2_HIGH;
						FiO2HighAlarmTime = 0;
					}
				}
				else
				{
					FiO2HighAlarmTime = 0;
				}
			}
			else
			{
				if(AlarmsWork.presentAlarms&ALARM_FIO2_HIGH)//djq modify 20161123
				{
					//如果产生了氧浓度传感器故障，则取消报警
					if(((FiO2_Value < AlarmSettingPara.FiO2HighValue) && (AlarmSettingPara.FiO2HighValue != 101))
						|| (AlarmSettingPara.FiO2HighValue == 101))
					{
						FiO2HighAlarmTime++;
						if (FiO2HighAlarmTime >= 30)//check for 3 consecutive cycles
						{
							AlarmsWork.presentAlarms &= ~ALARM_FIO2_HIGH;
							FiO2HighAlarmTime = 0;
						}
					}
					else
					{
						FiO2HighAlarmTime = 0;
					}
				}
			}
			//------------------------------------------------------------------------------------------
		}
		else
		{
// 			FiO2LowAlarmTime = 0;
			FiO2HighAlarmTime = 0;
			FiO2LessThanTime = 0;
// 			AlarmsWork.presentAlarms &= ~ALARM_FIO2_LOW;
			AlarmsWork.presentAlarms &= ~ALARM_FIO2_HIGH;
		}
    }
}

void AlarmCheck_PRVC_VT_LOW(int flag)
{
	if((BeathSettingPara.BreathMode == PRVC_MODE)||(BeathSettingPara.BreathMode == SIMV_PRVC_MODE))
	{
		if((flag & 0xff) == 0x55)
		{
			if(!(AlarmsWork.presentAlarms1 & ALAEM_PRVC_VT_LOW))
			{
				AlarmsWork.newAlarms1 |= ALAEM_PRVC_VT_LOW;
			}
		}
		else if((flag & 0xff) == 0xAA)
		{
			AlarmsWork.presentAlarms1 &= ~ALAEM_PRVC_VT_LOW;
			//AlarmsWork.unclearedAlarms1 &= ~ALAEM_PRVC_VT_LOW;                 
		}      
	}
}

void AlarmCheck_Absorber(int flag)
{
	if((flag & 0xFF) == 0xAA)
	{
		if(!(AlarmsWork.presentAlarms1 & ALARM_ABSORBER_DISCONNECT))
		{
			AlarmsWork.newAlarms1 |= ALARM_ABSORBER_DISCONNECT;
		}
	}
	else if((flag & 0xFF) == 0x55)
	{
		AlarmsWork.presentAlarms1 &= ~ALARM_ABSORBER_DISCONNECT;
		//AlarmsWork.unclearedAlarms1 &= ~ALARM_ABSORBER_DISCONNECT;  
	}
}
/***************************************************************************************************
* @brief  气源压力不足报警的监测处理函数
* @para
* @retval
*/
void AlarmCheckGasSupply(Int16U AirGasPressure,Int16U O2GasPressure)
{

    static Int16U AirCheckTimer = 0;
    static Int16U O2CheckTimer = 0;

    if(!(AlarmsWork.presentAlarms&ALARM_OXY_SOURCE_LOW))
    {
        if(O2GasPressure < GAS_LOW_PRESSURE_LIMIT)
        {
#ifndef __debug
            O2CheckTimer ++;      
           if(O2CheckTimer>=GAS_MEASURE_TIME)
           {
                O2CheckTimer = 0;
                AlarmsWork.newAlarms |= ALARM_OXY_SOURCE_LOW;

#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C) 
                if(DriverGas_Chose_Flag)//djq add 20170609驱动气体选配
                {
                    if((!DirverGasFlag)//驱动气为氧气并且
                        &&(AlarmAirSourceSwitch)//空气气源开
                        && !(AlarmsWork.presentAlarms1 & ALARM_AIR_SOURCE_LOW)//存在空气，则切换空气驱动气
						)
					{

                        DirverGasFlag = 1;
                        RTEEPROMwrite(DIRVER_GAS_ADDR,DirverGasFlag);
                        SendOperationMessage(DIRVER_GAS_FOR_BDU_COM,USART0);
                        Menu_hWin.InfoWin.InfoChangeFlag |= DRIVER_GAS_TYPE_FLAG;
                    }
                }
#endif
                UpdataSystemSettingMenu();
                GasSourceState &= 0x00FF;
                SendOperationMessage(OXY_FAILURE_INFORM,USART0);
           } 
#endif
        }
        else
        {
            O2CheckTimer = 0;
        }
    }
    else
    {
        if(O2GasPressure >= GAS_LOW_PRESSURE_LIMIT)
        {
            O2CheckTimer ++;
            if(O2CheckTimer>=GAS_MEASURE_TIME/2)
            {
                O2CheckTimer = 0;
                AlarmsWork.presentAlarms &= ~ALARM_OXY_SOURCE_LOW;
                GasSourceState |= 0xFF00;
                SendOperationMessage(OXY_NORMAL_INFORM,USART0);

#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C) 
                if(DriverGas_Chose_Flag)//djq add 20170609驱动气体选配
                {
                    if((DirverGasFlag)//如果驱动气为空气  空气气源压力低，自动切换为氧气
                    && (AlarmsWork.presentAlarms1 & ALARM_AIR_SOURCE_LOW)
					)//不存在空气
                    {
                        DirverGasFlag = 0;
                        RTEEPROMwrite(DIRVER_GAS_ADDR,DirverGasFlag);
                        SendOperationMessage(DIRVER_GAS_FOR_BDU_COM,USART0);
                        Menu_hWin.InfoWin.InfoChangeFlag |= DRIVER_GAS_TYPE_FLAG;
                    }
                }
#endif
                UpdataSystemSettingMenu();//djq add 20170407

            }
        }
        else
        {
            O2CheckTimer = 0;
        }
    }

#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C) 
    //djq add 20170407
    if((!(AlarmsWork.presentAlarms1&ALARM_AIR_SOURCE_LOW)) && 
		(AlarmAirSourceSwitch) && DriverGas_Chose_Flag )//开关打开的时候报警，关闭的时候报警消除
    {
        if(AirGasPressure < GAS_LOW_PRESSURE_LIMIT)
        {
#ifndef __debug
            AirCheckTimer ++;      
            if(AirCheckTimer>=GAS_MEASURE_TIME)
            {
                AirCheckTimer = 0;
                AlarmsWork.newAlarms1 |= ALARM_AIR_SOURCE_LOW;
                if(DriverGas_Chose_Flag)//djq add 20170609驱动气体选配
                {
                    if(DirverGasFlag)//如果驱动气为空气
                    {
                        DirverGasFlag = 0;
                        RTEEPROMwrite(DIRVER_GAS_ADDR,DirverGasFlag);
                        SendOperationMessage(DIRVER_GAS_FOR_BDU_COM,USART0);
                        Menu_hWin.InfoWin.InfoChangeFlag |= DRIVER_GAS_TYPE_FLAG;
                    }
                }
                UpdataSystemSettingMenu();
            } 
#endif
        }
        else
        {
            AirCheckTimer = 0;
        }
    }
    else if(AlarmsWork.presentAlarms1&ALARM_AIR_SOURCE_LOW)
    {
        if(AirGasPressure >= GAS_LOW_PRESSURE_LIMIT)
        {
            AirCheckTimer ++;
            if(AirCheckTimer>=GAS_MEASURE_TIME/2)
            {
                AirCheckTimer = 0;		
                AlarmsWork.presentAlarms1 &= ~ALARM_AIR_SOURCE_LOW;
                if(DriverGas_Chose_Flag)//djq add 20170609驱动气体选配
                {                    
                    if((!DirverGasFlag)//驱动气为氧气并且
                        &&(AlarmAirSourceSwitch)//空气气源开
                        &&(AlarmsWork.presentAlarms & ALARM_OXY_SOURCE_LOW))//氧气不存在，则切换空气驱动气
                    {
                        DirverGasFlag = 1;
                        RTEEPROMwrite(DIRVER_GAS_ADDR,DirverGasFlag);
                        SendOperationMessage(DIRVER_GAS_FOR_BDU_COM,USART0);
                        Menu_hWin.InfoWin.InfoChangeFlag |= DRIVER_GAS_TYPE_FLAG;
                    }
                }
                UpdataSystemSettingMenu();//djq add 20170407
            }
        }
        else
        {
            AirCheckTimer = 0;
        }
    }
#endif
	/*********************************************ylk add 20181211**********************************************/
	if(Drager_EtCO2_Config_On \
		&& (AA_Module_Flag.EtCO2ModuleExist == 2) && (AA_Module_Flag.PhaseInTypeModuleExist == 0))//是dragger模块
	{
		if(Drager_EtCO2_Config_On == 2)
		{
			if(((AA_all_data.AA_detail.AA_CO2.Accuracy == ACCURACY_Iso) \
				&& (AA_all_data.AA_detail.AA_O2.Accuracy == ACCURACY_Iso) \
				&& (AA_all_data.AA_detail.AA_N2O.Accuracy == ACCURACY_Iso) \
				&& (AA_all_data.AA_detail.AA_AA1.Accuracy == ACCURACY_Iso))\
				|| (!EtCO2ModuleOff))//dragger模块各个精度都达到或者开关处于关闭状态
			{
				HomePrompt_Clr4everPmt(HomePrompt_Dragger_Prio);
			}
			else
			{
				HomePrompt_Set4everPmt(HomePrompt_Dragger_Prio);
			}
		}
		else if(Drager_EtCO2_Config_On == 1)
		{
			if(((AA_all_data.AA_detail.AA_CO2.Accuracy == ACCURACY_Iso) \
				&& (AA_all_data.AA_detail.AA_N2O.Accuracy == ACCURACY_Iso) \
				&& (AA_all_data.AA_detail.AA_AA1.Accuracy == ACCURACY_Iso))\
				|| (!EtCO2ModuleOff))//dragger模块各个精度都达到或者开关处于关闭状态
			{
				HomePrompt_Clr4everPmt(HomePrompt_Dragger_Prio);
			}
			else
			{
				HomePrompt_Set4everPmt(HomePrompt_Dragger_Prio);
			}
		}
		else
		{
			HomePrompt_Clr4everPmt(HomePrompt_Dragger_Prio);
		}
	}
	/***********************************************************************************************************/
}

#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C)
//Masimo技术报警
static void AlarmCheckMasimoSpO2(int CheckFlag)
{
	Int8U Err = 0;
	static Int8U OffPatient = 0;

	if((CheckFlag)&&(!SpO2ModuleOff)) //
	{
		if((Exception2Info.SensorOffPatient)&&(!Err))//干扰
		{
			if(!(AlarmsWork.presentAlarms1 & ALARM_MASIMO_NO_PATIENT))
			{
				AlarmsWork.newAlarms1 |= ALARM_MASIMO_NO_PATIENT;
				LowAlarmPriority |= 0x80;
			}
			Err = 1;
			OffPatient = 0;
		}
		else
		{
			if(AlarmsWork.presentAlarms1 & ALARM_MASIMO_NO_PATIENT)
			{
				if(OffPatient > 20)
				{
					AlarmsWork.presentAlarms1 &= ~ALARM_MASIMO_NO_PATIENT;
					LowAlarmPriority &= ~0x80;
					OffPatient = 0;
				}
				OffPatient++;
			}
		}
		if((Exception2Info.LowPerfusion)&&(!Err))
		{
			if(!(AlarmsWork.presentAlarms & ALARM_MASIMO_LOW_PERFUSION))
			{
				AlarmsWork.newAlarms |= ALARM_MASIMO_LOW_PERFUSION;
			}
			Err = 1;
		}
		else
		{
			if(AlarmsWork.presentAlarms & ALARM_MASIMO_LOW_PERFUSION)
			{
				AlarmsWork.presentAlarms &= ~ALARM_MASIMO_LOW_PERFUSION;
			}
		}
		if((Exception2Info.DefectiveSensor)&&(!Err))//传感器缺陷
		{
			if(!(AlarmsWork.presentAlarms & ALARM_MASIMO_DFECTIVE_SENSOR))
			{
				AlarmsWork.newAlarms |= ALARM_MASIMO_DFECTIVE_SENSOR;
			}
		} 
		else
		{
			if(AlarmsWork.presentAlarms & ALARM_MASIMO_DFECTIVE_SENSOR)
			{
				AlarmsWork.presentAlarms &= ~ALARM_MASIMO_DFECTIVE_SENSOR;
			}
		}
		if((Exception2Info.InterferenceDetected)&&(!Err))//干扰
		{
			if(!(AlarmsWork.presentAlarms1 & ALARM_MASIMO_INTERFERNCE))
			{
				AlarmsWork.newAlarms1 |= ALARM_MASIMO_INTERFERNCE;
			}
		}
		else
		{
			if(AlarmsWork.presentAlarms1 & ALARM_MASIMO_INTERFERNCE)
			{
				AlarmsWork.presentAlarms1 &= ~ALARM_MASIMO_INTERFERNCE;
			}
		}
		if((Exception2Info.TooMuchAmbientLight)&&(!Err))//光照
		{
			if(!(AlarmsWork.presentAlarms1 & ALARM_MASIMO_AMBIENT_LIGHT))
			{
				AlarmsWork.newAlarms1 |= ALARM_MASIMO_AMBIENT_LIGHT;
			}
		}
		else
		{
			if(AlarmsWork.presentAlarms1 & ALARM_MASIMO_AMBIENT_LIGHT)
			{
				AlarmsWork.presentAlarms1 &= ~ALARM_MASIMO_AMBIENT_LIGHT;
			}
		}
		if((Exception2Info.UnrecognizedSensor)&&(!Err))//
		{
			if(!(AlarmsWork.presentAlarms & ALARM_MASIMO_UNRECOGNIZED_SENSOR))
			{
				AlarmsWork.newAlarms |= ALARM_MASIMO_UNRECOGNIZED_SENSOR;
			}
		}
		else
		{
			if(AlarmsWork.presentAlarms & ALARM_MASIMO_UNRECOGNIZED_SENSOR)
			{
				AlarmsWork.presentAlarms &= ~ALARM_MASIMO_UNRECOGNIZED_SENSOR;
			}
		}
		if((Exception2Info.NoCableConnected)&&(!Err))//
		{
			if(!(AlarmsWork.presentAlarms1 & ALARM_MASIMO_NO_CABLE))
			{
				AlarmsWork.newAlarms1 |= ALARM_MASIMO_NO_CABLE;
			}
		}
		else
		{
			if(AlarmsWork.presentAlarms1 & ALARM_MASIMO_NO_CABLE)
			{
				AlarmsWork.presentAlarms1 &= ~ALARM_MASIMO_NO_CABLE;
			}
		}
		if((Exception2Info.LowSignalIQ)&&(!Err))//
		{
			if(!(AlarmsWork.presentAlarms & ALARM_MASIMO_LOW_SIGNAL_IQ))
			{
				AlarmsWork.newAlarms |= ALARM_MASIMO_LOW_SIGNAL_IQ;
			}
		}
		else
		{
			if(AlarmsWork.presentAlarms & ALARM_MASIMO_LOW_SIGNAL_IQ)
			{
				AlarmsWork.presentAlarms &= ~ALARM_MASIMO_LOW_SIGNAL_IQ;
			}
		}
	}
	else
	{
		AlarmsWork.presentAlarms &= ~(ALARM_MASIMO_DFECTIVE_SENSOR|ALARM_MASIMO_UNRECOGNIZED_SENSOR|
			ALARM_MASIMO_LOW_PERFUSION|ALARM_MASIMO_LOW_SIGNAL_IQ);
		AlarmsWork.presentAlarms1 &= ~(ALARM_MASIMO_INTERFERNCE|ALARM_MASIMO_AMBIENT_LIGHT|ALARM_MASIMO_NO_CABLE|ALARM_MASIMO_NO_PATIENT);
	}
	if((SPO2FailureFlag)&&(!SpO2ModuleOff))
	{
		if(!(AlarmsWork.presentAlarms & ALARM_MASIMO_SPO2_FAILURE))
		{
			AlarmsWork.newAlarms |= ALARM_MASIMO_SPO2_FAILURE;
		}           
	}
	else
	{
		if(AlarmsWork.presentAlarms & ALARM_MASIMO_SPO2_FAILURE)
		{
			AlarmsWork.presentAlarms &= ~ALARM_MASIMO_SPO2_FAILURE;
		}
	}
}
#endif

/***************************************************************************************************
* @brief    血氧报警
* @para
* @retval
*/
static void AlarmCheckSpO2Para(JM1_SPO2_PARAMETERS * Para,unsigned char HeartBeatFlag)
{
    static Int8U HeartbeatTimesHigh_S = 0;
    static Int8U HeartbeatTimesLow_S = 0;
    static Int8U HeartbeatTimesHigh_P = 0;
    static Int8U HeartbeatTimesLow_P = 0;

    if(HeartBeatFlag)
    {
        HeartBeatFlag = 0;
        if(AlarmSettingPara.SpO2HighValue < 100)
        {
            if(Para->SpO2 <= 100)
            {
                if(!(AlarmsWork.presentAlarms1&ALARM_SPO2_HIGH))
                {
                    if(Para->SpO2>=AlarmSettingPara.SpO2HighValue)
                    {
                        HeartbeatTimesHigh_S++;
                        if(HeartbeatTimesHigh_S>=10)
                        {
                            AlarmsWork.newAlarms1 |= ALARM_SPO2_HIGH;
                            HeartbeatTimesHigh_S = 0;
                        }
                    }
                    else
                    {
                        HeartbeatTimesHigh_S = 0;
                    }
                }
                else
                {
                    if(Para->SpO2<AlarmSettingPara.SpO2HighValue)
                    {
                        HeartbeatTimesHigh_S++;
                        if(HeartbeatTimesHigh_S>=5)
                        {
                            AlarmsWork.presentAlarms1 &= ~ALARM_SPO2_HIGH;
                            HeartbeatTimesHigh_S = 0;
                        }
                    }
                    else
                    {
                        HeartbeatTimesHigh_S = 0;
                    }
                }
            }
            else
            {
                AlarmsWork.presentAlarms1 &= ~ALARM_SPO2_HIGH;
            }
        }
        else
        {
            AlarmsWork.presentAlarms1 &= ~ALARM_SPO2_HIGH;
        }
        if(Para->SpO2 <= 100)
        {
            if(!(AlarmsWork.presentAlarms&ALARM_SPO2_LOW))
            {
                if(Para->SpO2<=AlarmSettingPara.SpO2LowValue)
                {
                    HeartbeatTimesLow_S++;
                    if(HeartbeatTimesLow_S>=10)
                    {
                        AlarmsWork.newAlarms |= ALARM_SPO2_LOW;
                        HeartbeatTimesLow_S = 0;
                    }
                }
                else
                {
                    HeartbeatTimesLow_S = 0;
                }
            }
            else
            {
                if(Para->SpO2>AlarmSettingPara.SpO2LowValue)
                {
                    HeartbeatTimesLow_S++;
                    if(HeartbeatTimesLow_S>=5)
                    {
                        AlarmsWork.presentAlarms &= ~ALARM_SPO2_LOW;
                        HeartbeatTimesLow_S = 0;
                    }
                }
                else
                {
                    HeartbeatTimesLow_S = 0;
                }
            }
        }
        else
        {
            AlarmsWork.presentAlarms &= ~ALARM_SPO2_LOW;
        }
        if(Para->PulseRate <= 250)
        {
            if(!(AlarmsWork.presentAlarms1&ALARM_PULSE_RATE_HIGH))
            {
                if(Para->PulseRate>=AlarmSettingPara.PulseHighValue)
                {
                    HeartbeatTimesHigh_P++;
                    if(HeartbeatTimesHigh_P>=10)
                    {
                        AlarmsWork.newAlarms1 |= ALARM_PULSE_RATE_HIGH;
                        HeartbeatTimesHigh_P = 0;
                    }
                }
                else
                {
                    HeartbeatTimesHigh_P = 0;
                }
            }
            else
            {
                if(Para->PulseRate<AlarmSettingPara.PulseHighValue)
                {
                    HeartbeatTimesHigh_P++;
                    if(HeartbeatTimesHigh_P>=5)
                    {
                        AlarmsWork.presentAlarms1 &= ~ALARM_PULSE_RATE_HIGH;
                        HeartbeatTimesHigh_P = 0;
                    }
                }
                else
                {
                    HeartbeatTimesHigh_P = 0;
                }
            }
            if(!(AlarmsWork.presentAlarms&ALARM_PULSE_RATE_LOW))
            {
                if(Para->PulseRate<=AlarmSettingPara.PulseLowValue)
                {
                    HeartbeatTimesLow_P++;
                    if(HeartbeatTimesLow_P>=10)
                    {
                        AlarmsWork.newAlarms |= ALARM_PULSE_RATE_LOW;
                        HeartbeatTimesLow_P = 0;
                    }
                }
                else
                {
                    HeartbeatTimesLow_P = 0;
                }
            }
            else
            {
                if(Para->PulseRate>AlarmSettingPara.PulseLowValue)
                {
                    HeartbeatTimesLow_P++;
                    if(HeartbeatTimesLow_P>=5)
                    {
                        AlarmsWork.presentAlarms &= ~ALARM_PULSE_RATE_LOW;
                        HeartbeatTimesLow_P = 0;
                    }
                }
                else
                {
                    HeartbeatTimesLow_P = 0;
                }
            }
        }
        else
        {
            AlarmsWork.presentAlarms1 &= ~ALARM_PULSE_RATE_HIGH;
            AlarmsWork.presentAlarms &= ~ALARM_PULSE_RATE_LOW;
        }
    }
}
/***************************************************************************************************
* @brief    血氧错误报警
* @para
* @retval
*/
static void AlarmCheckSpO2Wrong(Int8U SensorErr)
{
    static Int8U count = 0;
    if(SensorErr)
    {
        if(!(AlarmsWork.presentAlarms1&ALARM_SPO2_SENSOR_DISCONNECT))
        {
            count++;
            if(count>=2)
            {
                AlarmsWork.newAlarms1 |= ALARM_SPO2_SENSOR_DISCONNECT;
                LowAlarmPriority |= 0x02;//sun add 20141224
                if(AlarmsWork.presentAlarms&ALARM_PULSE_RATE_LOW)
                {
                    AlarmsWork.presentAlarms &= ~ALARM_PULSE_RATE_LOW;
                }
                else if(AlarmsWork.presentAlarms1&ALARM_PULSE_RATE_HIGH)
                {
                    AlarmsWork.presentAlarms1 &= ~ALARM_PULSE_RATE_HIGH;
                }
                if(AlarmsWork.presentAlarms&ALARM_SPO2_LOW)
                {
                    AlarmsWork.presentAlarms &= ~ALARM_SPO2_LOW;
                }
                else if(AlarmsWork.presentAlarms1&ALARM_SPO2_HIGH)
                {
                    AlarmsWork.presentAlarms1 &= ~ALARM_SPO2_HIGH;
                }
                count = 0;
            }
        }
    }
    else
    {
        if(AlarmsWork.presentAlarms1&ALARM_SPO2_SENSOR_DISCONNECT)
        {
            count++;
            if(count>=2)
            {
                AlarmsWork.presentAlarms1 &=~ALARM_SPO2_SENSOR_DISCONNECT;
                AlarmsWork.unclearedAlarms1 &= ~ALARM_SPO2_SENSOR_DISCONNECT;
                LowAlarmPriority &= ~0x02;//sun add 20141224
                count = 0;
            }
        }
    }
}
/*
****************************************************************************************************
功能描述：笑气报警
入口参数：
出口参数：
****************************************************************************************************
*/
void AlarmCheckN2O(float N2OValue,int VentFlag)
{
#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C) 
    static  Int8U   N2OLowCount = 0;
    static  Int8U   N2OHighCount = 0;
    static  Int8U   N2OHighTime_Count = 0;
    static  Int8U   N2OLowTime_Count = 0;
    
    if (!(AlarmsWork.presentAlarms1 & ALARM_FIN2O_LOW))
    {
        if(AlarmSettingPara.N2OLowValue > 0)
        {
            if (N2OValue <= AlarmSettingPara.N2OLowValue)
            {
                N2OLowTime_Count ++;
                if(N2OLowTime_Count>=100) //check for 3 consecutive cycles
                {
                    AlarmsWork.newAlarms1 |= ALARM_FIN2O_LOW;// alarm
                    N2OLowTime_Count = 0;
                }
            }
            else
            {
                N2OLowTime_Count = 0;
            }
        }
        else
        {
            AlarmsWork.presentAlarms1  &=~ ALARM_FIN2O_LOW;//stop alarm
            N2OLowTime_Count = 0;
        }
    }
    else
    {
        if((N2OValue > AlarmSettingPara.N2OLowValue)||(AlarmSettingPara.N2OLowValue <= 0))
        {
            N2OLowTime_Count ++;
            if(N2OLowTime_Count>=100)//check for 2 consecutive cycles //ylk 30 --> 100
            {
                AlarmsWork.presentAlarms1  &=~ ALARM_FIN2O_LOW;//stop alarm
                N2OLowTime_Count = 0;
            }
        }
        else
        {
            N2OLowTime_Count = 0;
        }
    }
    if (!(AlarmsWork.presentAlarms & ALARM_FIN2O_HIGH))
    {
        if (N2OValue >= AlarmSettingPara.N2OHighValue)
        {
            N2OHighTime_Count ++;
            if(N2OHighTime_Count>=100)
            {
                N2OHighTime_Count = 0;
                AlarmsWork.newAlarms |= ALARM_FIN2O_HIGH;
            }
        }
        else
        {
            N2OHighTime_Count = 0;
        }
    }
    else
    {
        if(N2OValue < AlarmSettingPara.N2OHighValue)
        {
            N2OHighTime_Count ++;
            if(N2OHighTime_Count>=100)//ylk 30 --> 100
            {
                AlarmsWork.presentAlarms &= ~ALARM_FIN2O_HIGH;// STOP alarm
                N2OHighTime_Count = 0;
            }
        }
        else
        {
            N2OHighTime_Count = 0;
        }
    }    

    
    if(VentFlag)
    {
        if (!(AlarmsWork.presentAlarms1 & ALARM_FIN2O_LOW))
        {
            if(AlarmSettingPara.N2OLowValue > 0)
            {
                if (N2OValue <= AlarmSettingPara.N2OLowValue)
                {
                    N2OLowCount ++;
                    if(N2OLowCount>=4) //check for 3 consecutive cycles
                    {
                        AlarmsWork.newAlarms1 |= ALARM_FIN2O_LOW;// alarm
                        N2OLowCount = 0;
                    }
                }
                else
                {
                    N2OLowCount = 0;
                }
            }
            else
            {
                AlarmsWork.presentAlarms1  &=~ ALARM_FIN2O_LOW;//stop alarm
                N2OLowCount = 0;
            }
        }
        else
        {
            if((N2OValue > AlarmSettingPara.N2OLowValue)||(AlarmSettingPara.N2OLowValue <= 0))
            {
                N2OLowCount ++;
                if(N2OLowCount>=2)//check for 2 consecutive cycles
                {
                    AlarmsWork.presentAlarms1  &=~ ALARM_FIN2O_LOW;//stop alarm
                    N2OLowCount = 0;
                }
            }
            else
            {
                N2OLowCount = 0;
            }
        }
        if (!(AlarmsWork.presentAlarms & ALARM_FIN2O_HIGH))
        {
            if (N2OValue >= AlarmSettingPara.N2OHighValue)
            {
                N2OHighCount ++;
                if(N2OHighCount>=4)
                {
                    N2OHighCount = 0;
                    AlarmsWork.newAlarms |= ALARM_FIN2O_HIGH;
                }
            }
            else
            {
                N2OHighCount = 0;
            }
        }
        else
        {
            if(N2OValue < AlarmSettingPara.N2OHighValue)
            {
                N2OHighCount ++;
                if(N2OHighCount>=2)
                {
                    AlarmsWork.presentAlarms &= ~ALARM_FIN2O_HIGH;// STOP alarm
                    N2OHighCount = 0;
                }
            }
            else
            {
                N2OHighCount = 0;
            }
        }
    }
#endif
}
/*
****************************************************************************************************
功能描述：
入口参数：
出口参数：
****************************************************************************************************
*/
// void AlarmCheckAA3MAC(float AA,float ThreeMAC, int VentFlag)
// {
//     static  Int8U   AALessthan3MacCount = 0;
//     static  Int8U   AAGreaterthan3MacCount = 0;
//     
//     static  Int8U   AALessthan3MacTime_Count = 0;
//     static  Int8U   AAGreaterthan3MacTime_Count = 0;    
//     
// #if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C) 
//     if (!(AlarmsWork.presentAlarms1 & ALARM_AA_LESSTHAN_3MAC))
//     {
//         //if (AA < ThreeMAC)
//         if (ThreeMAC < 3)
//         {
//             AALessthan3MacTime_Count ++;
//             if(AALessthan3MacTime_Count>=100) //check for 3 consecutive cycles
//             {
//                 AlarmsWork.newAlarms1 |= ALARM_AA_LESSTHAN_3MAC;// alarm
//                 LowAlarmPriority |= 0x01;//sun add 20141224
//                 AALessthan3MacTime_Count = 0;
//             }
//         }
//         else
//         {
//             AALessthan3MacTime_Count = 0;
//         }
//     }
//     else
//     {
//         //if(AA >= ThreeMAC)
//         if(ThreeMAC >= 3)
//         {
//             AALessthan3MacTime_Count ++;
//             if(AALessthan3MacTime_Count>=100)//check for 2 consecutive cycles //ylk 30 --> 100
//             {
//                 AlarmsWork.presentAlarms1  &=~ ALARM_AA_LESSTHAN_3MAC;//stop alarm
//                 AlarmsWork.unclearedAlarms1 &= ~ALARM_AA_LESSTHAN_3MAC;
//                 LowAlarmPriority &= ~0x01;//sun add 20141224
//                 AALessthan3MacTime_Count = 0;
//             }
//         }
//         else
//         {
//             AALessthan3MacTime_Count = 0;
//         }
//     }
//     
//     //------------------------------------------------------------------------------------------
//     if (!(AlarmsWork.presentAlarms1 & ALARM_AA_GTE_3MAC))
//     {
//         //if (AA >= ThreeMAC)
//         if (ThreeMAC >= 3)
//         {
//             AAGreaterthan3MacTime_Count ++;
//             if(AAGreaterthan3MacTime_Count>=100)
//             {
//                 AAGreaterthan3MacTime_Count = 0;
//                 AlarmsWork.newAlarms1 |= ALARM_AA_GTE_3MAC;
//             }
//         }
//         else
//         {
//             AAGreaterthan3MacTime_Count = 0;
//         }
//     }
//     else
//     {
//         //if(AA < ThreeMAC)
//         if(ThreeMAC < 3)
//         {
//             AAGreaterthan3MacTime_Count ++;
//             if(AAGreaterthan3MacTime_Count>=100)//ylk 30 --> 100
//             {
//                 AlarmsWork.presentAlarms1 &= ~ALARM_AA_GTE_3MAC;// STOP alarm
//                 AAGreaterthan3MacTime_Count = 0;
//             }
//         }
//         else
//         {
//             AAGreaterthan3MacTime_Count = 0;
//         }
//     }    
//     
//     if(VentFlag)
//     {
//         if (!(AlarmsWork.presentAlarms1 & ALARM_AA_LESSTHAN_3MAC))
//         {
//             //if (AA < ThreeMAC)
//             if (ThreeMAC < 3)
//             {
//                 AALessthan3MacCount ++;
//                 if(AALessthan3MacCount>=4) //check for 3 consecutive cycles
//                 {
//                     AlarmsWork.newAlarms1 |= ALARM_AA_LESSTHAN_3MAC;// alarm
//                     LowAlarmPriority |= 0x01;//sun add 20141224
//                     AALessthan3MacCount = 0;
//                 }
//             }
//             else
//             {
//                 AALessthan3MacCount = 0;
//             }
//         }
//         else
//         {
//             //if(AA >= ThreeMAC)
//             if(ThreeMAC >= 3)
//             {
//                 AALessthan3MacCount ++;
//                 if(AALessthan3MacCount>=2)//check for 2 consecutive cycles
//                 {
//                     AlarmsWork.presentAlarms1  &=~ ALARM_AA_LESSTHAN_3MAC;//stop alarm
//                     AlarmsWork.unclearedAlarms1 &= ~ALARM_AA_LESSTHAN_3MAC;
//                     LowAlarmPriority &= ~0x01;//sun add 20141224
//                     AALessthan3MacCount = 0;
//                 }
//             }
//             else
//             {
//                 AALessthan3MacCount = 0;
//             }
//         }
// 
//         //------------------------------------------------------------------------------------------
//         if (!(AlarmsWork.presentAlarms1 & ALARM_AA_GTE_3MAC))
//         {
//             //if (AA >= ThreeMAC)
//             if (ThreeMAC >= 3)
//             {
//                 AAGreaterthan3MacCount ++;
//                 if(AAGreaterthan3MacCount>=4)
//                 {
//                     AAGreaterthan3MacCount = 0;
//                     AlarmsWork.newAlarms1 |= ALARM_AA_GTE_3MAC;
//                 }
//             }
//             else
//             {
//                 AAGreaterthan3MacCount = 0;
//             }
//         }
//         else
//         {
//             //if(AA < ThreeMAC)
//             if(ThreeMAC < 3)
//             {
//                 AAGreaterthan3MacCount ++;
//                 if(AAGreaterthan3MacCount>=2)
//                 {
//                     AlarmsWork.presentAlarms1 &= ~ALARM_AA_GTE_3MAC;// STOP alarm
//                     AAGreaterthan3MacCount = 0;
//                 }
//             }
//             else
//             {
//                 AAGreaterthan3MacCount = 0;
//             }
//         }
//     }
// #endif
// }
void AlarmCheckAA3MAC(float AA,float ThreeMAC, int VentFlag)
{
	static  u8   AALessthan3MacCount = 0;
	static  u8   AAGreaterthan3MacCount = 0;
	static  u8   AALessthan3MacTimer = 0;
	static  u8   AAGreaterthan3MacTimer = 0;

// 	if(alarm_CountClrFlag)
// 	{
// 		AALessthan3MacCount = 0;
// 		AAGreaterthan3MacCount = 0;
// 		AALessthan3MacTimer = 0;
// 		AAGreaterthan3MacTimer = 0;
// 	}

	if (!(AlarmsWork.presentAlarms1 & ALARM_AA_LESSTHAN_3MAC))
	{
		if (ThreeMAC < 3)
		{
			if(VentFlag)
			{
				AALessthan3MacCount ++;
			}
			AALessthan3MacTimer++;
			if(AALessthan3MacCount>=ALARM_CYCLE_MAX1 || AALessthan3MacTimer >= ALARM_TIME_MAX) //check for 3 consecutive cycles
			{
				AlarmsWork.newAlarms1 |= ALARM_AA_LESSTHAN_3MAC;// alarm
				LowAlarmPriority |= 0x01;//sun add 20141224
				AALessthan3MacTimer = 0;
				AALessthan3MacCount = 0;
			}
		}
		else
		{
			AALessthan3MacCount = 0;
			AALessthan3MacTimer = 0;
		}
	}
	else
	{
		if(ThreeMAC >= 3)
		{
			if(VentFlag)
			{
				AALessthan3MacCount ++;
			}
			AALessthan3MacTimer++;
			if(AALessthan3MacCount>=ALARM_CYCLE_MAX2 || AALessthan3MacTimer >= ALARM_TIME_MIN)//check for 2 consecutive cycles
			{
				AlarmsWork.presentAlarms1  &=~ ALARM_AA_LESSTHAN_3MAC;//stop alarm
				AlarmsWork.unclearedAlarms1 &= ~ALARM_AA_LESSTHAN_3MAC;
				LowAlarmPriority &= ~0x01;//sun add 20141224
				AALessthan3MacCount = 0;
				AALessthan3MacTimer = 0;
			}
		}
		else
		{
			AALessthan3MacCount = 0;
			AALessthan3MacTimer = 0;
		}
	}

	//------------------------------------------------------------------------------------------
	if (!(AlarmsWork.presentAlarms1 & ALARM_AA_GTE_3MAC))
	{
		if (ThreeMAC >= 3)
		{
			if(VentFlag)
			{
				AAGreaterthan3MacCount ++;
			}
			AAGreaterthan3MacTimer++;
			if(AAGreaterthan3MacCount>=ALARM_CYCLE_MAX1 || AAGreaterthan3MacTimer >= ALARM_TIME_MAX)
			{
				AAGreaterthan3MacCount = 0;
				AAGreaterthan3MacTimer = 0;
				AlarmsWork.newAlarms1 |= ALARM_AA_GTE_3MAC;
			}
		}
		else
		{
			AAGreaterthan3MacCount = 0;
			AAGreaterthan3MacTimer = 0;
		}
	}
	else
	{
		if(ThreeMAC < 3)
		{
			if(VentFlag)
			{
				AAGreaterthan3MacCount ++;
			}
			AAGreaterthan3MacTimer++;
			if(AAGreaterthan3MacCount>=ALARM_CYCLE_MAX2 || AAGreaterthan3MacTimer >= ALARM_TIME_MIN)
			{
				AlarmsWork.presentAlarms1 &= ~ALARM_AA_GTE_3MAC;// STOP alarm
				AAGreaterthan3MacCount = 0;
				AAGreaterthan3MacTimer = 0;
			}
		}
		else
		{
			AAGreaterthan3MacCount = 0;
			AAGreaterthan3MacTimer = 0;
		}
	}
}



/*
****************************************************************************************************
功能描述：
入口参数：
出口参数：
****************************************************************************************************
*/
void AlarmCheckFiAA(float FiAAValue,int VentFlag)
{
    static  Int8U   FiAALowCount = 0;
    static  Int8U   FiAAHighCount = 0;
    static unsigned short int   FiAALowCount_Time;
    static unsigned short int   FiAAHighCount_Time;    
#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C) 
    if (!(AlarmsWork.presentAlarms1 & ALARM_FIAA_LOW))
    {    
        if( (AlarmSettingPara.FiAALowValue > 0 && FiAAValue <= AlarmSettingPara.FiAALowValue) )
        {
            if(++FiAALowCount_Time >= 100 )  //10s
            {
                FiAALowCount_Time=0;
                AlarmsWork.newAlarms1 |= ALARM_FIAA_LOW;
            }
        }
        else
        {
            FiAALowCount_Time=0;   
        }
    }
    else
    {
        if((FiAAValue > AlarmSettingPara.FiAALowValue)||(AlarmSettingPara.FiAALowValue <= 0))
        {
            FiAALowCount ++;
            if(FiAALowCount>=2)//check for 2 consecutive cycles
            {
                AlarmsWork.presentAlarms1  &=~ ALARM_FIAA_LOW;//stop alarm
                FiAALowCount = 0;
            }
        }
        else
        {
            FiAALowCount = 0;
        }  
    }
    
    if (!(AlarmsWork.presentAlarms & ALARM_FIAA_HIGH))
    {    
        if( FiAAValue >= AlarmSettingPara.FiAAHighValue )
        {
            if(++FiAAHighCount_Time >= 100 )  //10s
            {
                FiAAHighCount_Time=0;
                AlarmsWork.newAlarms |= ALARM_FIAA_HIGH;
            }
        }
        else
        {
         FiAAHighCount_Time=0;   
        }
    }
    else
    {
        if( FiAAValue < AlarmSettingPara.FiAAHighValue )
        {
            FiAALowCount ++;
            if(FiAALowCount>=2)//check for 2 consecutive cycles
            {
                AlarmsWork.presentAlarms  &=~ ALARM_FIAA_HIGH;//stop alarm
                FiAALowCount = 0;
            }
        }
        else
        {
            FiAALowCount = 0;
        }  
    }
    /************/ 
    
    if(VentFlag)
    {
        if (!(AlarmsWork.presentAlarms1 & ALARM_FIAA_LOW))
        {
            if(AlarmSettingPara.FiAALowValue > 0)
            {
                if (FiAAValue <= AlarmSettingPara.FiAALowValue)
                {
                    FiAALowCount ++;
                    if(FiAALowCount>=4) //check for 3 consecutive cycles
                    {
                        AlarmsWork.newAlarms1 |= ALARM_FIAA_LOW;// alarm
                        FiAALowCount = 0;
                    }
                }
                else
                {
                    FiAALowCount = 0;
                }
            }
            else
            {
                AlarmsWork.presentAlarms1 &=~ ALARM_FIAA_LOW;//stop alarm
                FiAALowCount = 0;
            }
        }
        else
        {
            if((FiAAValue > AlarmSettingPara.FiAALowValue)||(AlarmSettingPara.FiAALowValue <= 0))
            {
                FiAALowCount ++;
                if(FiAALowCount>=2)//check for 2 consecutive cycles
                {
                    AlarmsWork.presentAlarms1  &=~ ALARM_FIAA_LOW;//stop alarm
                    FiAALowCount = 0;
                }
            }
            else
            {
                FiAALowCount = 0;
            }
        }
        if (!(AlarmsWork.presentAlarms & ALARM_FIAA_HIGH))
        {
            if (FiAAValue >= AlarmSettingPara.FiAAHighValue)
            {
                FiAAHighCount ++;
                if(FiAAHighCount>=4)
                {
                    FiAAHighCount = 0;
                    AlarmsWork.newAlarms |= ALARM_FIAA_HIGH;
                }
            }
            else
            {
                FiAAHighCount = 0;
            }
        }
        else
        {
            if(FiAAValue < AlarmSettingPara.FiAAHighValue)
            {
                FiAAHighCount ++;
                if(FiAAHighCount>=2)
                {
                    AlarmsWork.presentAlarms &= ~ALARM_FIAA_HIGH;// STOP alarm
                    FiAAHighCount = 0;
                }
            }
            else
            {
                FiAAHighCount = 0;
            }
        }
    }
#endif
}

/*
****************************************************************************************************
功能描述：
入口参数：
出口参数：
****************************************************************************************************
*/
void AlarmCheckAARange(Int16U FiVal, Int16U EtVal, int VentFlag)
{
	static  Int8U   AACount = 0;
	static  Int8U   AATmrCount = 0;
	Int16U range = AA_Range_GetRange();

	if(!(AlarmsWork.presentAlarms1 & ALARM_GAS_COMM_FAULT))
	{	
		if (!(AlarmsWork.presentAlarms1 & ALARM_AAOVERFLOW))
	{
		if(FiVal > range || EtVal > range)
		{
			if(VentFlag)
			{
				AACount++;
			}
			AATmrCount++;
			if(AACount >= ALARM_CYCLE_MAX1 || AATmrCount >= ALARM_TIME_MAX)
			{
				AACount = 0;
				AATmrCount = 0;
				AlarmsWork.newAlarms1 |= ALARM_AAOVERFLOW;
			}
		}
		else
		{
			AACount = 0;
			AATmrCount = 0;
		}
	}
	else
	{
		if(FiVal <= range && EtVal <= range)
		{
			if(VentFlag)
			{
				AACount++;
			}
			AATmrCount++;
			if(AACount >= ALARM_CYCLE_MAX2 || AATmrCount >= ALARM_TIME_MIN)
			{
				AACount = 0;
				AATmrCount = 0;
				AlarmsWork.presentAlarms1  &=~ ALARM_AAOVERFLOW;
			}
		}
		else
		{
			AACount = 0;
			AATmrCount = 0;
		}
	}
	}
	else
	{
		AACount = 0;
		AATmrCount = 0;
		AlarmsWork.presentAlarms1  &=~ ALARM_AAOVERFLOW;
	}
}
void AlarmCheckEtAA(float EtAAValue,int VentFlag)
{
    static  Int8U   EtAALowCount = 0;
    static  Int8U   EtAAHighCount = 0;
    static unsigned short int   EtAALowCount_Time;
    static unsigned short int   EtAAHighCount_Time;

#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C) 
    /*zy 2016.6.28***********/
    if (!(AlarmsWork.presentAlarms1 & ALARM_ETAA_LOW))
    {    
        if( (AlarmSettingPara.EtAALowValue > 0 && EtAAValue <= AlarmSettingPara.EtAALowValue) )
        {
            if(++EtAALowCount_Time >= 100 )  //10s
            {
                EtAALowCount_Time=0;
                AlarmsWork.newAlarms1 |= ALARM_ETAA_LOW;
            }
        }
        else
        {
         EtAALowCount_Time=0;   
        }
    }
    else
    {
        if((EtAAValue > AlarmSettingPara.EtAALowValue)||(AlarmSettingPara.EtAALowValue <= 0))
        {
            EtAALowCount ++;
            if(EtAALowCount>=2)//check for 2 consecutive cycles
            {
                AlarmsWork.presentAlarms1  &=~ ALARM_ETAA_LOW;//stop alarm
                EtAALowCount = 0;
            }
        }
        else
        {
            EtAALowCount = 0;
        }  
    }
    
    if (!(AlarmsWork.presentAlarms & ALARM_ETAA_HIGH))
    {    
        if( EtAAValue >= AlarmSettingPara.EtAAHighValue )
        {
            if(++EtAAHighCount_Time >= 100 )  //10s
            {
                EtAAHighCount_Time=0;
                AlarmsWork.newAlarms |= ALARM_ETAA_HIGH;
            }
        }
        else
        {
			EtAAHighCount_Time=0;   
        }
    }
    else
    {
        if( EtAAValue < AlarmSettingPara.EtAAHighValue )
        {
            EtAAHighCount ++;
            if(EtAAHighCount>=2)//check for 2 consecutive cycles
            {
                AlarmsWork.presentAlarms  &=~ ALARM_ETAA_HIGH;//stop alarm
                EtAAHighCount = 0;
            }
        }
        else
        {
            EtAAHighCount = 0;
        }  
    }
    /************/       
    if(VentFlag)
    {
        if (!(AlarmsWork.presentAlarms1 & ALARM_ETAA_LOW))
        {
            if(AlarmSettingPara.EtAALowValue > 0)
            {
                if (EtAAValue <= AlarmSettingPara.EtAALowValue)
                {
                    EtAALowCount ++;
                    if(EtAALowCount>=4) //check for 3 consecutive cycles
                    {
                        AlarmsWork.newAlarms1 |= ALARM_ETAA_LOW;// alarm
                        EtAALowCount = 0;
                    }
                }
                else
                {
                    EtAALowCount = 0;
                }
            }
            else
            {
                AlarmsWork.presentAlarms1  &=~ ALARM_ETAA_LOW;//stop alarm
                EtAALowCount = 0;
            }
        }
        else
        {
            if((EtAAValue > AlarmSettingPara.EtAALowValue)||(AlarmSettingPara.EtAALowValue <= 0))
            {
                EtAALowCount ++;
                if(EtAALowCount>=2)//check for 2 consecutive cycles
                {
                    AlarmsWork.presentAlarms1  &=~ ALARM_ETAA_LOW;//stop alarm
                    EtAALowCount = 0;
                }
            }
            else
            {
                EtAALowCount = 0;
            }
        }
        if (!(AlarmsWork.presentAlarms & ALARM_ETAA_HIGH))
        {
            if (EtAAValue >= AlarmSettingPara.EtAAHighValue)
            {
                EtAAHighCount ++;
                if(EtAAHighCount>=4)
                {
                    EtAAHighCount = 0;
                    AlarmsWork.newAlarms |= ALARM_ETAA_HIGH;
                }
            }
            else
            {
                EtAAHighCount = 0;
            }
        }
        else
        {
            if(EtAAValue < AlarmSettingPara.EtAAHighValue)
            {
                EtAAHighCount ++;
                if(EtAAHighCount>=2)
                {
                    AlarmsWork.presentAlarms &= ~ALARM_ETAA_HIGH;// STOP alarm
                    EtAAHighCount = 0;
                }
            }
            else
            {
                EtAAHighCount = 0;
            }
        }
    }
#endif
}
/***************************************************************************************************
* @brief    呼气末CO2报警
* @para
* @retval
*/
void AlarmCheckEtCO2(Int16U EtCO2_Value,Int8U Vent_Flag)
{
    static unsigned char CountHigh = 0;
    static unsigned char CountLow = 0;
    
    static unsigned short int   EtCO2LowCount_Time;
    static unsigned short int   EtCO2HighCount_Time;    
    
	if (CPB_GetFlag())
	{
		AlarmsWork.presentAlarms &= ~ALARM_ETCO2_HIGH;
		EtCO2HighCount_Time = 0;
		CountHigh = 0;
		AlarmsWork.presentAlarms1 &= ~ALARM_ETCO2_LOW;
		EtCO2LowCount_Time = 0;
		CountLow = 0;
	}
	else
	{
		if(!(AlarmsWork.presentAlarms&ALARM_ETCO2_HIGH))
		{
			if(EtCO2_Value>=AlarmSettingPara.EtCO2HighValue)//柳
			{
				EtCO2HighCount_Time++;
				if(EtCO2HighCount_Time>=100)//3 cpp md 20130423
				{
					AlarmsWork.newAlarms |= ALARM_ETCO2_HIGH;
					EtCO2HighCount_Time = 0;
				}
			}
			else
			{
				EtCO2HighCount_Time = 0;
			}
		}
		else
		{
			if(EtCO2_Value<AlarmSettingPara.EtCO2HighValue)
			{
				EtCO2HighCount_Time++;
				if(EtCO2HighCount_Time>=100)//2 cpp md 20130423 //ylk 30 --> 100
				{
					AlarmsWork.presentAlarms &= ~ALARM_ETCO2_HIGH;
					EtCO2HighCount_Time = 0;
				}
			}
			else
			{
				EtCO2HighCount_Time = 0;
			}
		}    

		if(!(AlarmsWork.presentAlarms1&ALARM_ETCO2_LOW))
		{
			if(EtCO2_Value<AlarmSettingPara.EtCO2LowValue)//柳
			{
				EtCO2LowCount_Time++;
				if(EtCO2LowCount_Time>=100)//3 cpp md 20130423
				{
					AlarmsWork.newAlarms1 |= ALARM_ETCO2_LOW;
					EtCO2LowCount_Time = 0;
				}
			}
			else
			{
				EtCO2LowCount_Time = 0;
			}
		}
		else
		{
			if(EtCO2_Value>AlarmSettingPara.EtCO2LowValue)
			{
				EtCO2LowCount_Time++;
				if(EtCO2LowCount_Time>=100)//2 cpp md 20130423 //ylk 30 --> 100
				{
					AlarmsWork.presentAlarms1 &= ~ALARM_ETCO2_LOW;
					EtCO2LowCount_Time = 0;
				}
			}
			else
			{
				EtCO2LowCount_Time = 0;
			}
		}     
		if(Vent_Flag)
		{
			//        HeartBeatFlag = 0;
			if(!(AlarmsWork.presentAlarms&ALARM_ETCO2_HIGH))
			{
				if(EtCO2_Value>=AlarmSettingPara.EtCO2HighValue)
				{
					CountHigh++;
					if(CountHigh>=10)//3 cpp md 20130423
					{
						AlarmsWork.newAlarms |= ALARM_ETCO2_HIGH;
						CountHigh = 0;
					}
				}
				else
				{
					CountHigh = 0;
				}
			}
			else
			{
				if(EtCO2_Value<AlarmSettingPara.EtCO2HighValue)
				{
					CountHigh++;
					if(CountHigh>=3)//2 cpp md 20130423
					{
						AlarmsWork.presentAlarms &= ~ALARM_ETCO2_HIGH;
						CountHigh = 0;
					}
				}
				else
				{
					CountHigh = 0;
				}
			}
			if(!(AlarmsWork.presentAlarms1&ALARM_ETCO2_LOW))
			{
#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C) 
				if(AlarmSettingPara.EtCO2LowValue >0)
#endif
				{
					if(EtCO2_Value<AlarmSettingPara.EtCO2LowValue)//<= //cpp md 20130423
					{
						CountLow++;
						if(CountLow>=10)//3 cpp md 20130423
						{
							AlarmsWork.newAlarms1 |= ALARM_ETCO2_LOW;
							CountLow = 0;
						}
					}
					else
					{
						CountLow = 0;
					}
				}
			}
			else
			{
				if((EtCO2_Value>AlarmSettingPara.EtCO2LowValue)||(AlarmSettingPara.EtCO2LowValue <=0))
				{
					CountLow++;
					if(CountLow>=3)//2 cpp md 20130423
					{
						AlarmsWork.presentAlarms1 &= ~ALARM_ETCO2_LOW;
						CountLow = 0;
					}
				}
				else
				{
					CountLow = 0;
				}
			}
		}
	}
}
/***************************************************************************************************
* @brief
* @para
* @retval
*/
void AlarmCheckFiCO2(Int16U FiCO2_Value,Int8U Vent_Flag)
{
    static unsigned char CountHigh = 0;
    static unsigned char CountLow = 0;
    
    static unsigned short int   FiCO2LowCount_Time;
    static unsigned short int   FiC02HighCount_Time;    
    
	if (CPB_GetFlag())
	{
		AlarmsWork.presentAlarms &= ~ALARM_FICO2_HIGH;
		FiC02HighCount_Time = 0;
		CountHigh = 0;
	}
	else
	{
		if(!(AlarmsWork.presentAlarms&ALARM_FICO2_HIGH))
		{
			if(FiCO2_Value>=AlarmSettingPara.FiCO2HighValue)
			{
				FiC02HighCount_Time++;
				if(FiC02HighCount_Time>=100)
				{
					AlarmsWork.newAlarms |= ALARM_FICO2_HIGH;
					FiC02HighCount_Time = 0;
				}
			}
			else
			{
				FiC02HighCount_Time = 0;
			}
		}
		else
		{
			if(FiCO2_Value<AlarmSettingPara.FiCO2HighValue)
			{
				FiC02HighCount_Time++;
				if(FiC02HighCount_Time>=100)//2 cpp md 20130423  //ylk 3 --> 100
				{
					AlarmsWork.presentAlarms &= ~ALARM_FICO2_HIGH;
					FiC02HighCount_Time = 0;
				}
			}
			else
			{
				FiC02HighCount_Time = 0;
			}
		}    
		if(Vent_Flag)
		{
#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C) 
			HeartBeatFlag = 0;
#endif
			if(!(AlarmsWork.presentAlarms&ALARM_FICO2_HIGH))
			{
				if(FiCO2_Value>=AlarmSettingPara.FiCO2HighValue)
				{
					CountHigh++;
					if(CountHigh>=10)//3 cpp md 20130423
					{
						AlarmsWork.newAlarms |= ALARM_FICO2_HIGH;
						CountHigh = 0;
					}
				}
				else
				{
					CountHigh = 0;
				}
			}
			else
			{
				if(FiCO2_Value<AlarmSettingPara.FiCO2HighValue)
				{
					CountHigh++;
					if(CountHigh>=3)//2 cpp md 20130423
					{
						AlarmsWork.presentAlarms &= ~ALARM_FICO2_HIGH;
						CountHigh = 0;
					}
				}
				else
				{
					CountHigh = 0;
				}
			}
		}
	}
}

/***************************************************************************************************
* @brief
* @para
* @retval
*/
void AlarmCheckAcgoOpen(Int8U AcgoOpenFlag)
{

}

/***************************************************************************************************
* @brief  AC and battery failure alarm
* @para
* @retval
*/
void AlarmCheckPower(Int16U BatteryValue)
{
    static Int8U AcOkCount = 0;
    static Int8U AcFailCount = 0;
    static Int8U BatLowCount = 0;
    static Int8U BatExaustCount = 0;
    if(CheckAcStatus()==FALSE)
    {
        if (!(AlarmsWork.presentAlarms1 & ALARM_AC_FAIL))
        {
            AcFailCount++;
            if(AcFailCount>= 5)
            {                       
#ifndef    __debug
                AlarmsWork.newAlarms1 |= ALARM_AC_FAIL;
//                AlarmsWork.presentAlarms &=~ALARM_BATTERY_FAULT_HIGH;
                LowAlarmPriority |= 0x01;
                Menu_hWin.InfoWin.InfoChangeFlag |= POWER_INFO_CHANGED_FLAG;
                AcFailCount = 0;
#endif
            }
        }
        else
        {
			AcFailCount = 0;
            if(!(AlarmsWork.presentAlarms & ALARM_BATTERY_LOW))
            {
                if(BatteryStateNum == 1)//battery low
                {
                    BatLowCount++;
                    if(BatLowCount>=20)
                    {
                        AlarmsWork.newAlarms |= ALARM_BATTERY_LOW;
                        BatLowCount = 0;
                    }
                }
                else
                {
                    BatLowCount = 0;
                }
            }
			else
			{
				if(BatteryStateNum >1)
				{
					BatLowCount++;
					if(BatLowCount>=20)
					{
						AlarmsWork.presentAlarms &=~ ALARM_BATTERY_LOW;
						BatLowCount = 0;
					}
				}
				else
				{
					BatLowCount = 0;
				}
			}
        }
    }
    else
    {
        if(AlarmsWork.presentAlarms1&ALARM_AC_FAIL)
        {
            AcOkCount++;
            if(AcOkCount>2)
            {
				AlarmsWork.presentAlarms &= ~ALARM_BATTERY_LOW;
                AlarmsWork.presentAlarms1 &= ~ALARM_AC_FAIL;// stop battery and ac alarm
                AlarmsWork.unclearedAlarms1 &= ~ALARM_AC_FAIL;
                LowAlarmPriority &= ~0x01;//sun add 20141224
                Menu_hWin.InfoWin.InfoChangeFlag |= (POWER_INFO_CHANGED_FLAG)|(ALARM_INFO_CHANGED_FLAG);
                AcOkCount = 0;
            }
        }
		//
		AcFailCount = 0;
		 BatLowCount = 0;
    }
}
// void phasein_Technology_Alarm(void)//sun add 20140821
// {
//     if(pAll_Data->slow.GenVal.sw_err & 0x01)//软件版本错误，重启传感器
//     {
//         phaseinAlarm |= 0x0001;
//     }
//     else
//     {

//         phaseinAlarm &= 0xFFFE;
//     }
//     if(pAll_Data->slow.GenVal.hW_err & 0x02)//硬件版本错误，更换传感器报警
//     {
//         phaseinAlarm |= 0x0002;
//     }
//     else
//     {
//         phaseinAlarm &= 0xFFFD;
//     }
//     if(pAll_Data->slow.GenVal.m_fail & 0x04)//电机速度超限
//     {
//         phaseinAlarm |= 0x0004;
//     }
//     else
//     {
//         phaseinAlarm &= 0xFFFB;
//     }
//     if(pAll_Data->slow.GenVal.un_cal & 0x08)//出厂校准缺失中
//     {
//         phaseinAlarm |= 0x0008;
//     }
//     else
//     {
//         phaseinAlarm &= 0xFFF7;
//     }
//     if(pAll_Data->slow.GenVal.repl_adapt & 0x01)//更换适配器
//     {
//         phaseinAlarm |= 0x0010;
//     }
//     else
//     {
//         phaseinAlarm &= 0xFFEF;
//     }
//     if(pAll_Data->slow.GenVal.no_adapt & 0x02)//无适配器，请连接适配器
//     {
//         phaseinAlarm |= 0x0020;
//     }
//     else
//     {
//         phaseinAlarm &= 0xFFDF;
//     }
// 
//     if(pAll_Data->slow.GenVal.co2_or & 0x01)//二氧化碳超差
//     {
//         phaseinAlarm |= 0x0100;
//     }
//     else
//     {
//         phaseinAlarm &= 0xFEFF;
//     }
//     if(pAll_Data->slow.GenVal.n2o_or & 0x02)//一氧化二氮超差
//     {
//         phaseinAlarm |= 0x0200;
//     }
//     else
//     {
//         phaseinAlarm &= 0xFDFF;
//     }
//     if(pAll_Data->slow.GenVal.ax_or & 0x04)//至少一种麻醉气体超差
//     {
//         phaseinAlarm |= 0x0400;
//     }
//     else
//     {
//         phaseinAlarm &= 0xFBFF;
//     }
// 
//     if(pAll_Data->slow.GenVal.temp_or & 0x10)//内部温度高
//     {
//         phaseinAlarm |= 0x1000;
//     }
//     else
//     {
//         phaseinAlarm &= 0xEFFF;
//     }
//     if(pAll_Data->slow.GenVal.press_or & 0x20)//大气压超出运行范围
//     {
//         phaseinAlarm |= 0x2000;
//     }
//     else
//     {
//         phaseinAlarm &= 0xDFFF;
//     }
//     if(pAll_Data->slow.GenVal.zero_or & 0x40)//零基准不准确，需要归零
//     {
//         phaseinAlarm |= 0x4000;
//     }
//     else
//     {
//         phaseinAlarm &= 0xBFFF;
//     }
//     if(pAll_Data->slow.GenVal.agen_id_unreliable & 0x80)//麻醉气体标识和浓度不可信
//     {
//         phaseinAlarm |= 0x8000;
//     }
//     else
//     {
//         phaseinAlarm &= 0x7FFF;
//     }
// }
/***************************************************************************************************
* @brief  报警信息处理,包括复位，重置，更新，显示等操作的判别
* @para
* @retval
*/
Int8U AlarmEventHandle(Int8U alarmEvent)
{
	Int8U   uGrade;
	Int64U  uType;
    Int8U   InquireResult = 0;//0:失败   1：成功
    Int64U  ResetAlarm = 0;
    Int64U  BanResetAlarm = 0;
    ALARM_INFO alarmInfo;
	uGrade = ShowedAlarmInfo.Grade;
	uType = ShowedAlarmInfo.Type;
	if (alarmEvent == ALARM_RESET)
	{
		if(AlarmsWork.presentAlarms)
		{
			if(uType == ALARM_STANDBY_ENTER)
			{
				StandbyClear_FLag = 1;
				AlarmsWork.presentAlarms &= ~ALARM_STANDBY_ENTER;
			}
		}
		else
		{
			if(!AlarmsWork.presentAlarms1)
			{
				ShowedAlarmInfo.Type = 0;
				AlarmsWork.unclearedAlarms = 0;
				AlarmsWork.unclearedAlarms1 = 0;
			}
			else if(uType == ALARM_ETCO2_MODULE_OFF)
			{
				EtCO2ModuleClear_Flag = 1;
				AlarmsWork.presentAlarms1 &= ~ALARM_ETCO2_MODULE_OFF;
				LowAlarmPriority &= ~0x10;//sun add 20141224
			}
			else if(uType == ALARM_O2_MODULE_OFF)//((AlarmsWork.presentAlarms1 & ALARM_O2_MODULE_OFF) && (alarmInfo.Type & ALARM_O2_MODULE_OFF))
			{
				O2ModuleClear_FLag = 1;
				AlarmsWork.presentAlarms1 &= ~ALARM_O2_MODULE_OFF;
				LowAlarmPriority &= ~0x40;//sun add 20141224
			}
			else if(uType == ALARM_SPO2_MODULE_OFF)//((AlarmsWork.presentAlarms1 & ALARM_SPO2_MODULE_OFF) && (alarmInfo.Type & ALARM_SPO2_MODULE_OFF))
			{
				SpO2ModuleClear_FLag = 1;
				AlarmsWork.presentAlarms1 &= ~ALARM_SPO2_MODULE_OFF;
				LowAlarmPriority &= ~0x20;//sun add 20141224
			}
		}

		Menu_hWin.InfoWin.InfoChangeFlag |= (ALARM_INFO_CHANGED_FLAG);
	}
	else if (alarmEvent == ALARM_RECOVER)
	{
	}
	else if (alarmEvent == ALARM_NEW)
	{
		ShowedAlarmInfo = GetHighestAlarm(AlarmsWork.presentAlarms,AlarmsWork.presentAlarms1);
		Menu_hWin.InfoWin.InfoChangeFlag |= (ALARM_INFO_CHANGED_FLAG);
	}
	else if (alarmEvent == ALARM_END)
	{
		AlarmsWork.BeepState = TRUE;
		Menu_hWin.InfoWin.InfoChangeFlag |= (ALARM_INFO_CHANGED_FLAG)|(SILENCE_INFO_CHANGED_FLAG);
		LedUpdateRegister &= ~MUTE_LED;
		SendOperationMessage(LED_UPDATE_OPERATION,USART3);
		SendOperationMessage(BUZZER_CONTROL_OPERATION,USART3);
	}
    else if (alarmEvent == ALARM_RESET_INQUIRE)
    {
        ResetAlarm = ( ALARM_ETCO2_MODULE_OFF | ALARM_O2_MODULE_OFF | ALARM_SPO2_MODULE_OFF ); 
#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C)
		BanResetAlarm = ( ALARM_AA_LESSTHAN_3MAC | ALARM_AC_FAIL | ALARM_SPO2_SENSOR_DISCONNECT );
#else
		BanResetAlarm = (ALARM_AC_FAIL | ALARM_SPO2_SENSOR_DISCONNECT );
#endif
        if((AlarmsWork.presentAlarms1 & ResetAlarm)     //存在可以复位的报警
        &&!(AlarmsWork.presentAlarms1 & (~ResetAlarm))	//中低级报警除复位报警外无其他报警
		&& !AlarmsWork.presentAlarms) //无高级报警
        {
            InquireResult = 1;
        }
        else if((AlarmsWork.presentAlarms1 & BanResetAlarm)//存在禁止复位的报警
        &&!(AlarmsWork.presentAlarms1 & (~(BanResetAlarm | ResetAlarm)))//只有低级报警存在
		&& !AlarmsWork.presentAlarms) //无高级报警
        {
            InquireResult = 2;
        }
    }
    return InquireResult;
}

/***************************************************************************************************
* @brief  通讯故障报警:BDU               ylk add 20181106
* @para
* @retval
*/
void AlarmCheckModuleCommFault(void)
{
	static Int8U BDU_Count = 0, GAS_Count = 0;

	//BDU
	if(!(AlarmsWork.presentAlarms & ALARM_BDU_COMM_FAULT))
	{
		if(BDU_CommFault_Get())//错误标志位置位
		{
			BDU_Count++;
			if(BDU_Count >= 80)
			{
				BDU_Count = 0;
				AlarmsWork.newAlarms |= ALARM_BDU_COMM_FAULT;// alarm
			}
		}
		else
		{
			BDU_Count = 0;
		}
	}
	else
	{
		if(BDU_CommFault_Get() == 0)
		{
			BDU_Count++;
			if(BDU_Count >= 30)
			{
				BDU_Count = 0;
				AlarmsWork.presentAlarms &= ~ALARM_BDU_COMM_FAULT;// clear alarm
			}
		}
		else
		{
			BDU_Count = 0;
		}
	}
	//EGM

	//Gas Module
	if (EtCO2ModuleOff)
	{
		if(!(AlarmsWork.presentAlarms1 & ALARM_GAS_COMM_FAULT))
		{
			if(GAS_CommFault_Get())//错误标志位置位
			{
				GAS_Count++;
				if(GAS_Count >= 80)
				{
					GAS_Count = 0;
					AA_Module_Flag.EtCO2ModuleExist = 0;
					AlarmsWork.newAlarms1 |= ALARM_GAS_COMM_FAULT;// alarm
				}
			}
			else
			{
				GAS_Count = 0;
			}

		}
		else
		{
			if(GAS_CommFault_Get() == 0)
			{
				GAS_Count++;
				if(GAS_Count >= 30)
				{
					GAS_Count = 0;
					AlarmsWork.presentAlarms1 &= ~ALARM_GAS_COMM_FAULT;// clear alarm
				}
			}
			else
			{
				GAS_Count = 0;
			}
		}
	}
	else
	{
		GAS_Count = 0;
		AlarmsWork.presentAlarms1 &= ~ALARM_GAS_COMM_FAULT;
	}
	//SpO2 Module
}


/***************************************************************************************************
* @brief  电池故障报警 ylk add 20181106
* @para
* @retval
*/
void AlarmCheckBatteryFault(Int16U BatteryVoltage)
{
	static Int16U count = 0;

	if((CheckAcStatus() == TRUE) )//接通电源
	{
		if (!(AlarmsWork.presentAlarms & ALARM_BATTERY_FAULT_HIGH))
		{
			if(CheckBatteryStatus() == FALSE)//
			{
				count++;
				if (count > 60)
				{
					count = 0;
					if(!(AlarmsWork.presentAlarms & ALARM_BATTERY_FAULT_HIGH))
					{
						AlarmsWork.newAlarms |= ALARM_BATTERY_FAULT_HIGH;
					}
				}
			}
			else
			{
				count = 0;
			}
		}
		else
		{
			if (CheckBatteryStatus() == TRUE)
			{
				count++;
				if (count >= 2)
				{
					count = 0;
					if ((AlarmsWork.presentAlarms & ALARM_BATTERY_FAULT_HIGH))
					{
						AlarmsWork.presentAlarms &=~ALARM_BATTERY_FAULT_HIGH;
					}
				}
			}
			else
			{
				count = 0;
			}
		}
	}
}

void AlarmCancelBatteryFault(void)
{
	static Int8U count = 0;

	if(CheckChargeStatus() != 2 && RealtimePara.BatteryVoltage > BatteryPara.Battery_Disconnect)
	{
		count++;
		if(count >= 120)
		{
			BAT_Limit = 400;
			count = 0;
			if(AlarmsWork.presentAlarms & ALARM_BATTERY_FAULT_HIGH)//消除高级报警 
			{
				AlarmsWork.presentAlarms &= ~ ALARM_BATTERY_FAULT_HIGH;
			}
//			if(AlarmsWork.presentAlarms & ALARM_BATTERY_FAULT_LOW)//消除低级报警 
//			{
//				AlarmsWork.presentAlarms &= ~ ALARM_BATTERY_FAULT_LOW;
//			}
		}
	}
	else
	{
		count  = 0;
	}
}
/***************************************************************************************************
* @brief  0.1s定时执行报警监测函数
* @para
* @retval
*/
void CheckAlarm(char insp_flag)
{
    Int8U changeFlag = 0;
    static Int8U ClearFlag = 0;
    ALARM_INFO HighestAlarm, AlarmInfo;
    static Int16U LastAlarmPriority = 0,LastLowAlarmPriority = 0;
    static Int16U Cycle_Gas_Check = 0;//  20130325
	static Int64U Old_PressentAlarms = 0,Old_PressentAlarms1 = 0;
	unsigned int Alarm_Num = 0;
	static int Old_BreathMode;  
	
	if (!Demo_Alarm)
	{
		if(CHECK_DEBUG != 1)
		{
			//*//  20130325
			if((AlarmSettingPara.EtCO2LowValue <=0)&&(AlarmsWork.presentAlarms1&ALARM_ETCO2_LOW))
			{
				AlarmsWork.presentAlarms1 &= ~ALARM_ETCO2_LOW;
			}
			if((AlarmsWork.presentAlarms & ALARM_RATE_LOW)&&(AlarmSettingPara.RateLowValue <= 0))
			{
				AlarmsWork.presentAlarms &= ~ ALARM_RATE_LOW;
			}
			if((AlarmsWork.presentAlarms & ALARM_MV_LOW)&&(AlarmSettingPara.MVLowValue <= 0))
			{
				AlarmsWork.presentAlarms &= ~ ALARM_MV_LOW;
			}
			if((AlarmsWork.presentAlarms & ALARM_PAW_LOW)&&(AlarmSettingPara.PressureLowValue <= 0))
			{
				AlarmsWork.presentAlarms &= ~ ALARM_PAW_LOW;
			}
			if((AlarmsWork.presentAlarms1 & ALARM_VTE_LOW)&&(AlarmSettingPara.VteLowValue <= 10))
			{
				AlarmsWork.presentAlarms1 &= ~ALARM_VTE_LOW;
			}

			AlarmCheckPower(RealtimePara.BatteryVoltage);       
			AlarmCheckModuleCommFault();//BDU通讯故障报警
			AlarmCheckBatteryFault(RealtimePara.BatteryVoltage);//电池故障报警
			// 		AlarmCancelBatteryFault();//报警清除

			if( (AA_Module_Flag.EtCO2ModuleExist == 1) && (EtCO2ModuleOff) ) //只有国产CO2存在且开启
			{
				AlarmCheckCo2PumpOcclusion(insp_flag);
#if (LOGO_FLAG == MSW_LOGO)
				SendOperationMessage(CO2_PUMPOCCLUSION,USART1);
#else
				SendOperationMessage(CO2_PUMPOCCLUSION,USART2);
#endif
			}
			else if( (AA_Module_Flag.EtCO2ModuleExist== 2) && (EtCO2ModuleOff) )//勿删!!
			{
			}
			//        else if( ((AA_Module_Flag.EtCO2ModuleExist&0x04) == 0x04) && (EtCO2ModuleOff) )//勿删!!
			//        {
			//        }
			else //关闭
			{
				if(AlarmsWork.presentAlarms1 & ALARM_ETCO2_LINE_OCCLUSION)
				{
					AlarmsWork.presentAlarms1 &= ~ALARM_ETCO2_LINE_OCCLUSION;
				}
				//*//  20130424
				if(AlarmsWork.presentAlarms & ALARM_ETCO2_HIGH)
				{
					AlarmsWork.presentAlarms &= ~ALARM_ETCO2_HIGH;
				}
				if(AlarmsWork.presentAlarms1 & ALARM_ETCO2_LOW)
				{
					AlarmsWork.presentAlarms1 &= ~ALARM_ETCO2_LOW;
				}//*/
				//*//  20130608
				if(AlarmsWork.presentAlarms & ALARM_FICO2_HIGH)
				{
					AlarmsWork.presentAlarms &= ~ALARM_FICO2_HIGH;
				}      
			}	

			if(FiO2_Config_On)//氧浓度传感器选配
			{
				if((!O2ModuleClear_FLag)&&(O2ModuleOff))//O2 off   
				{
					if(!(AlarmsWork.presentAlarms1 & ALARM_O2_MODULE_OFF))
					{
						AlarmsWork.newAlarms1 |= ALARM_O2_MODULE_OFF;
						LowAlarmPriority |= 0x10;//sun add 20141224
					} 
				}
			}
			else
			{
				if((AlarmsWork.presentAlarms & ALARM_O2_MODULE_OFF))
				{
					AlarmsWork.presentAlarms &= ~ALARM_O2_MODULE_OFF;
					AlarmsWork.unclearedAlarms &= ~ALARM_O2_MODULE_OFF;
					LowAlarmPriority &= ~0x10;//sun add 20141224
				} 
			}

			if(Old_BreathMode != BeathSettingPara.BreathMode)//解决PSV和手动切换时窒息报警不清除问题
			{
				if(Old_BreathMode == PSV_MODE || Old_BreathMode == BACKUP_MODE)
				{
					AlarmsWork.presentAlarms &= ~ALARM_APNEA;
					AlarmsWork.unclearedAlarms &= ~ALARM_APNEA;
					SendSetModes(BeathSettingPara.BreathMode);
					//BeathSettingPara.BreathMode = PSV_MODE; //  20130325 
					BackUpMode = 0;
				}
				else if(Old_BreathMode == MANUAL_MODE)
				{
					AlarmsWork.presentAlarms &= ~ALARM_APNEA;
					AlarmsWork.unclearedAlarms &= ~ALARM_APNEA;                    
				}
				Old_BreathMode = BeathSettingPara.BreathMode;
			}

#ifndef LANGUAGE_CHINESE_FLAG
			if((SpO2ModuleExist)&&(!SpO2ModuleOff) &&(Masimo_SpO2_Config_On == 1 || SpO2_Config_On == 1))//存在且开启
			{
				AlarmCheckSpO2Wrong(SpO2_Parameters.SensorErr);
#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C)
				if(SpO2_Choose == 2)
				{
					AlarmCheckMasimoSpO2(!(Exception2Info.NoSensorConnected||SPO2FailureFlag));
				}
#endif
			}
			else //关闭
			{
				if(AlarmsWork.presentAlarms1 & ALARM_SPO2_SENSOR_DISCONNECT)
				{
					AlarmsWork.presentAlarms1 &= ~ALARM_SPO2_SENSOR_DISCONNECT;
					AlarmsWork.unclearedAlarms1 &= ~ALARM_SPO2_SENSOR_DISCONNECT;
					LowAlarmPriority &= ~0x02;//sun add 20141224
				}
				else //没有脱落报警 //  20130424
				{
					if(AlarmsWork.presentAlarms1 & ALARM_SPO2_HIGH)
					{
						AlarmsWork.presentAlarms1 &= ~ALARM_SPO2_HIGH;
					}
					if(AlarmsWork.presentAlarms & ALARM_SPO2_LOW)
					{
						AlarmsWork.presentAlarms &= ~ALARM_SPO2_LOW;
					}
					if(AlarmsWork.presentAlarms1 & ALARM_PULSE_RATE_HIGH)
					{
						AlarmsWork.presentAlarms1 &= ~ALARM_PULSE_RATE_HIGH;
					}
					if(AlarmsWork.presentAlarms & ALARM_PULSE_RATE_LOW)
					{
						AlarmsWork.presentAlarms &= ~ALARM_PULSE_RATE_LOW;
					}
				}
			}
#endif
			if((BeathSettingPara.BreathMode != SIMV_PRVC_MODE) && (BeathSettingPara.BreathMode != PRVC_MODE))
			{
				if(MonitorPara.PRVC_VT_LOW_Flag != 0 || (AlarmsWork.presentAlarms1 & ALAEM_PRVC_VT_LOW))
				{
					//从PRVC切换到其他模式的时候，消除该报警 
					MonitorPara.PRVC_VT_LOW_Flag = 0;	//报警标志清零
					AlarmsWork.presentAlarms1 &= ~ALAEM_PRVC_VT_LOW;
					//AlarmsWork.unclearedAlarms1 &= ~ALAEM_PRVC_VT_LOW;//ylk del 20190813 切到其他模式后，保留锁存信息
				}
			}

			//*//  20130325
			if(Cycle_Gas_Check < 20)//柳
			{
				Cycle_Gas_Check ++;//柳
			}
			else
			{
				AlarmCheckGasSupply(RealtimePara.AirGasPressure,RealtimePara.O2GasPressure);
			}//*/
			InitAlarmInfoRegister();
			if((AA_Module_Flag.PhaseInTypeModuleExist)&&(AA_Module_Flag.EtCO2ModuleExist)&&(!EtCO2ModuleOff) && ((Masimo_EtCO2_Config_On)\
				|| (MasimoISA_EtCO2_Config_On) || (EtCO2_Config_On) || (EtCO2_KM7002_Config_On) || (Drager_EtCO2_Config_On))) //AA或CO2模块存在且关闭
			{
				if((!EtCO2ModuleClear_Flag) && (!(AlarmsWork.presentAlarms1 & ALARM_ETCO2_MODULE_OFF)))
				{
					AlarmsWork.newAlarms1 |= ALARM_ETCO2_MODULE_OFF;
					LowAlarmPriority |= 0x04;//sun add 20141224
				}
			}
#ifndef LANGUAGE_CHINESE_FLAG
			if((SpO2ModuleExist)&&(SpO2ModuleOff) && ((Masimo_SpO2_Config_On) || (SpO2_Config_On)))//存在且关闭
			{
				if((!SpO2ModuleClear_FLag) && (!(AlarmsWork.presentAlarms1 & ALARM_SPO2_MODULE_OFF)))
				{
					AlarmsWork.newAlarms1 |= ALARM_SPO2_MODULE_OFF;
					LowAlarmPriority |= 0x20;//sun add 20141224
				}
			}
#endif
			if(FiO2_Config_On)//氧浓度传感器选配 
			{
				if((!O2ModuleClear_FLag)&&(O2ModuleOff))//O2 off     djq add 20131226 
				{
					if(!(AlarmsWork.presentAlarms1 & ALARM_O2_MODULE_OFF)) //  20130705
					{
						AlarmsWork.newAlarms1 |= ALARM_O2_MODULE_OFF;
						LowAlarmPriority |= 0x10;//sun add 20141224
					} 
				}	
			}
			// 		if((LOGO_FLAG != PENLON_LOGO)||(BeathSettingPara.BreathMode != ACGO_MODE))//ylk add 20180709
			// 		{
			// 			AlarmCheckO2Sensor(MonitorPara.FiO2Value,Paramagnetic_O2Flag);//zkq add 20170115 O2 Sensor	
			// 		}
			//------------------------------------------------------------------------------
#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C)         
			if(LOGO_FLAG != HOSPITECH_LOGO)
				AlarmCheck_Absorber(MonitorPara.ABSORBER_Flag);
#endif
			if (StandbyState != 0xFF) //非待机 模式下产生报警
			{
				AlarmCheck_PRVC_VT_LOW(MonitorPara.PRVC_VT_LOW_Flag);
#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C) 
				//             phasein_Technology_Alarm();//sun add 20151102
				if(!phaseinAlarm)//sun add 20151102
				{
					/*
					if( ((AA_Module_Flag.EtCO2ModuleExist&0x06) != 0) && (EtCO2ModuleOff)&&(AA_all_data.AA_detail.AA_AA1.ID) )
					{

					AlarmCheckEtAA(AA_all_data.AA_detail.AA_AA1.ET,insp_flag);///10.0
					AlarmCheckFiAA(AA_all_data.AA_detail.AA_AA1.FI,insp_flag);///10.0

					if(AA_all_data.AA_detail.AA_AA2.ID)
					{
					AlarmCheckAA3MAC(AA_all_data.AA_detail.AA_AA1.FI/10.0,(float)AA_Para.MAC/10,insp_flag);//
					}
					else
					{
					AlarmsWork.presentAlarms1  &=~ ALARM_AA_LESSTHAN_3MAC;
					AlarmsWork.unclearedAlarms1 &= ~ALARM_AA_LESSTHAN_3MAC;
					AlarmsWork.presentAlarms1 &= ~ALARM_AA_GTE_3MAC;
					}

					}
					if(((AA_Module_Flag.EtCO2ModuleExist&0x06) != 0)&&(EtCO2ModuleOff))//sun add 20140611
					{
					//                  if(pAll_Data->slow.VerVal.sen_type & 0x04)
					//  				if(AA_all_data.AA_state.dev_info.module_type & 0x04)//?
					if(AA_all_data.AA_state.dev_info.module_type == AA_PHASEIN_OR)//?
					{
					#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C)
					AlarmCheckN2O(AA_all_data.AA_detail.AA_N2O.FI,insp_flag);
					#endif
					}
					}

					if((AA_Module_Flag.EtCO2ModuleExist)&&(EtCO2ModuleOff))//AA或CO2或都存在
					{
					AlarmCheckEtCO2((Int16U)CO2_Parameters.etCO2,insp_flag);
					AlarmCheckFiCO2((Int16U)CO2_Parameters.insCO2,insp_flag);
					}
					*/

				}
				else //sun add 20151102
				{
					AlarmsWork.presentAlarms  &=~ (ALARM_ETAA_HIGH 
						| ALARM_FIAA_HIGH
						| ALARM_FIN2O_HIGH
						| ALARM_ETCO2_HIGH
						| ALARM_FICO2_HIGH);

					AlarmsWork.presentAlarms1  &=~ (ALARM_ETAA_LOW
						| ALARM_FIAA_LOW  
						| ALARM_AA_LESSTHAN_3MAC
						| ALARM_FIN2O_LOW 
						| ALARM_AA_GTE_3MAC
						| ALARM_ETCO2_LOW);
#ifdef Pre_ACGO//Penlon ACGO下除了顺磁氧 其余模块都要清除氧浓度报警  //ylk add 20181203
					if(!(Drager_EtCO2_Config_On == 2 \
						&& AA_Module_Flag.EtCO2ModuleExist == 2 \
						&& AA_Module_Flag.PhaseInTypeModuleExist == 0))//当前不是drager顺磁氧
					{
						AlarmsWork.presentAlarms &=~ (ALARM_FIO2_HIGH|ALARM_FIO2_LOW);
						AlarmsWork.presentAlarms &= ~ALARM_FIO2_LESSTHAN_18;						
					}
#endif
				}
#else
				if((AA_Module_Flag.EtCO2ModuleExist)&&(EtCO2ModuleOff))
				{
					AlarmCheckEtCO2((Int16U)CO2_Parameters.etCO2,insp_flag);
					AlarmCheckFiCO2((Int16U)CO2_Parameters.insCO2,insp_flag);
				}
#endif

				AA_Err_Message_Progress();
				if(!AA_all_data.AA_state.module_err_number)//sun add 20140821
				{
					if((AA_Module_Flag.EtCO2ModuleExist == 2)&&(EtCO2ModuleOff)&&(AA_all_data.AA_detail.AA_AA1.ID))
					{
						if(Drager_EtCO2_Config_On == 2)//如果现在是drager+顺磁氧
						{
							if(AA_all_data.AA_detail.AA_CO2.Accuracy == ACCURACY_Iso 
								&& AA_all_data.AA_detail.AA_N2O.Accuracy == ACCURACY_Iso
								&& AA_all_data.AA_detail.AA_AA1.Accuracy == ACCURACY_Iso
								&& AA_all_data.AA_detail.AA_O2.Accuracy  == ACCURACY_Iso
								&& (AA_Module_Flag.INIT_AA_MODULE_FLAG & 0x02) == 2)//都达到高精度以上&选择了麻醉气体
							{
								AlarmCheckEtAA(AA_all_data.AA_detail.AA_AA1.ET,insp_flag);///10.0
								AlarmCheckFiAA(AA_all_data.AA_detail.AA_AA1.FI,insp_flag);///10.0
								AlarmCheckAARange(AA_all_data.AA_detail.AA_AA1.FI,\
									AA_all_data.AA_detail.AA_AA1.ET,insp_flag);
							}
						}
						else if(Drager_EtCO2_Config_On == 1)//如果现在是drager
						{
							if(AA_all_data.AA_detail.AA_CO2.Accuracy == ACCURACY_Iso 
								&& AA_all_data.AA_detail.AA_N2O.Accuracy == ACCURACY_Iso
								&& AA_all_data.AA_detail.AA_AA1.Accuracy == ACCURACY_Iso
								&& (AA_Module_Flag.INIT_AA_MODULE_FLAG & 0x02) == 2)//都达到高精度以上&选择了麻醉气体
							{
								AlarmCheckEtAA(AA_all_data.AA_detail.AA_AA1.ET,insp_flag);///10.0
								AlarmCheckFiAA(AA_all_data.AA_detail.AA_AA1.FI,insp_flag);///10.0
								AlarmCheckAARange(AA_all_data.AA_detail.AA_AA1.FI,\
									AA_all_data.AA_detail.AA_AA1.ET,insp_flag);
							}
						}
						else
						{
							AlarmCheckEtAA(AA_all_data.AA_detail.AA_AA1.ET,insp_flag);///10.0
							AlarmCheckFiAA(AA_all_data.AA_detail.AA_AA1.FI,insp_flag);///10.0
							AlarmCheckAARange(AA_all_data.AA_detail.AA_AA1.FI,\
								AA_all_data.AA_detail.AA_AA1.ET,insp_flag);
						}
						if(AA_all_data.AA_detail.AA_AA2.ID)
						{
							//AlarmCheckAA3MAC(pAll_Data->slow.InspVal[IM_GAS_AX1]/10.0,(float)3*AA_Para.MAC/100,insp_flag);//
							AlarmCheckAA3MAC(AA_all_data.AA_detail.AA_AA1.FI/10.0,(float)AA_Para.MAC/100,insp_flag);//djq modify 20170522 扩大了MAC的倍数，所以除数也放大
						}
						else
						{
							AlarmsWork.presentAlarms1  &=~ ALARM_AA_LESSTHAN_3MAC;
							AlarmsWork.unclearedAlarms1 &= ~ALARM_AA_LESSTHAN_3MAC;
							AlarmsWork.presentAlarms1 &= ~ALARM_AA_GTE_3MAC;
						}
					}
					if((AA_Module_Flag.EtCO2ModuleExist == 2)&&(EtCO2ModuleOff))//sun add 20140611
					{
						if(N2O_Switch != 1)
						{
							if(Drager_EtCO2_Config_On == 2)//如果现在是drager 顺磁氧
							{
								if(AA_all_data.AA_detail.AA_CO2.Accuracy == ACCURACY_Iso 
									&& AA_all_data.AA_detail.AA_N2O.Accuracy == ACCURACY_Iso
									&& AA_all_data.AA_detail.AA_AA1.Accuracy == ACCURACY_Iso
									&& AA_all_data.AA_detail.AA_O2.Accuracy  == ACCURACY_Iso)//都达到低精度以上
								{
									AlarmCheckN2O(AA_all_data.AA_detail.AA_N2O.FI,insp_flag);
								}
							}
							else if(Drager_EtCO2_Config_On == 1)//如果现在是drager
							{
								if(AA_all_data.AA_detail.AA_CO2.Accuracy == ACCURACY_Iso 
									&& AA_all_data.AA_detail.AA_N2O.Accuracy == ACCURACY_Iso
									&& AA_all_data.AA_detail.AA_AA1.Accuracy == ACCURACY_Iso)//都达到低精度以上
								{
									AlarmCheckN2O(AA_all_data.AA_detail.AA_N2O.FI,insp_flag);
								}
							}
							else
							{
								AlarmCheckN2O(AA_all_data.AA_detail.AA_N2O.FI,insp_flag);
							}
						}
					}
					if((AA_Module_Flag.EtCO2ModuleExist)&&(EtCO2ModuleOff))
					{
						if(Drager_EtCO2_Config_On == 2)//如果现在是drager 顺磁氧
						{
							if(AA_all_data.AA_detail.AA_CO2.Accuracy == ACCURACY_Iso 
								&& AA_all_data.AA_detail.AA_N2O.Accuracy == ACCURACY_Iso
								&& AA_all_data.AA_detail.AA_AA1.Accuracy == ACCURACY_Iso
								&& AA_all_data.AA_detail.AA_O2.Accuracy  == ACCURACY_Iso)//都达到低精度以上
							{
								AlarmCheckEtCO2((Int16U)CO2_Parameters.etCO2,insp_flag);
								AlarmCheckFiCO2((Int16U)CO2_Parameters.insCO2,insp_flag);
							}
						}
						else if(Drager_EtCO2_Config_On == 1)
						{
							if(AA_all_data.AA_detail.AA_CO2.Accuracy == ACCURACY_Iso 
								&& AA_all_data.AA_detail.AA_N2O.Accuracy == ACCURACY_Iso
								&& AA_all_data.AA_detail.AA_AA1.Accuracy == ACCURACY_Iso)//都达到低精度以上
							{
								AlarmCheckEtCO2((Int16U)CO2_Parameters.etCO2,insp_flag);
								AlarmCheckFiCO2((Int16U)CO2_Parameters.insCO2,insp_flag);
							}
						}
						else
						{
							AlarmCheckEtCO2((Int16U)CO2_Parameters.etCO2,insp_flag);
							AlarmCheckFiCO2((Int16U)CO2_Parameters.insCO2,insp_flag);
						}
					}
				}
				/****************************************************************************************************/

				if(BeathSettingPara.BreathMode != ACGO_MODE) //  if..else.. 20130604
				{
					//*//  20130325
					if(Min_Limit <= 600)//工作一分钟后才开始检查
					{
						Min_Limit ++;//柳
						if(Min_Limit > (1800 / BeathSettingPara.SettingRateValue))//柳
						{ 
							AlarmCheckPaw(MonitorPara.PpeakValue,insp_flag);//ylk modify 20180426 (delete /10)
							//AlarmCheckPawN10((int)RealtimePara.Pressure/10,insp_flag); //  20130607
							AlarmCheckPawN10((int)RealtimePara.Pressure/10,insp_flag);
							AlarmCheckVTE(MonitorPara.VteValue,insp_flag);//柳
						}
					}
					else
					{
						AlarmCheckPaw(MonitorPara.PpeakValue,insp_flag);//ylk modify 20180426 (delete /10)
						AlarmCheckPawN10((int)RealtimePara.Pressure/10,insp_flag); //  20130607
						AlarmCheckVTE(MonitorPara.VteValue,insp_flag);
						AlarmCheckRate(MonitorPara.RateTotalValue,insp_flag);
						AlarmCheckMV(MonitorPara.MinuteVolumeValue,insp_flag);//柳
					}
					if(BeathSettingPara.BreathMode != ACGO_MODE)
					{
						AlarmCheckContinuePressure((int)RealtimePara.Pressure/10); //zkq modify 20170116 手动下可以产生持续起到压力高报警
					}

					AlarmCheckApnea();
					ClearFlag = 1;
				}
				else
				{
					if(ClearFlag == 1)//add zkq 20161025 防止ACGO模式下报警闪烁
					{
						ClearFlag = 0;
#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C) 
						ClearAcgoModeOtherAlarm();
#endif
					}
				} 

#ifndef LANGUAGE_CHINESE_FLAG
				if((SpO2ModuleExist)&&(!SpO2ModuleOff)&&(Masimo_SpO2_Config_On == 1 || SpO2_Config_On == 1))
				{
					if(!(AlarmsWork.presentAlarms1&ALARM_SPO2_SENSOR_DISCONNECT))
					{
						AlarmCheckSpO2Para(&SpO2_Parameters,HeartBeatFlag);
					}
				}
#endif
				// 			if((LOGO_FLAG != PENLON_LOGO)||(BeathSettingPara.BreathMode != ACGO_MODE))//ylk add 20180709
				// 			{
				// 				AlarmCheckFiO2(MonitorPara.FiO2Value,insp_flag);
				// 			}            
				if(AlarmsWork.presentAlarms & ALARM_STANDBY_ENTER)//standby, reset active alarms
				{
					AlarmsWork.presentAlarms &= ~ALARM_STANDBY_ENTER;
				}

			}
			else if (StandbyState == 0xFF)//&(LastStandbyState == 0))
			{
				if((!StandbyClear_FLag) && (!(AlarmsWork.presentAlarms & ALARM_STANDBY_ENTER)))
				{
					//AlarmsWork.newAlarms |= ALARM_STANDBY_ENTER; //zkq del 20170116 去掉待机报警
				}
				if(LastStandbyState == 0)
				{
					//AlarmCheckApnea();////
					ClearParameterAlarm();
					LastStandbyState = StandbyState;
				}
			}

			if(Drager_EtCO2_Config_On == 2)//如果现在是drager 顺磁氧
			{
				if(AA_all_data.AA_detail.AA_CO2.Accuracy == ACCURACY_Iso 
					&& AA_all_data.AA_detail.AA_N2O.Accuracy == ACCURACY_Iso
					&& AA_all_data.AA_detail.AA_AA1.Accuracy == ACCURACY_Iso
					&& AA_all_data.AA_detail.AA_O2.Accuracy  == ACCURACY_Iso)//都达到高精度
				{
					AlarmCheckO2Sensor(MonitorPara.FiO2Value,Paramagnetic_O2_Flag);
					if(StandbyState==0)//工作状态下
					{
						AlarmCheckFiO2(MonitorPara.FiO2Value,insp_flag);
					}
				}
			}
			else if(MasimoISA_EtCO2_Config_On == 2)//Masimo ISA with O2
			{
				AlarmCheckO2Sensor(MonitorPara.FiO2Value,Paramagnetic_O2_Flag);
				if(StandbyState==0)//工作状态下
				{
					AlarmCheckFiO2(MonitorPara.FiO2Value,insp_flag);
				}
			}
			else //化学式氧电池
			{
#ifdef Pre_ACGO	//前置ACGO不监测氧浓度
				if(BeathSettingPara.BreathMode == ACGO_MODE)//非ACGO下
				{
					AlarmsWork.presentAlarms &= ~(ALARM_FIO2_HIGH|ALARM_FIO2_LOW);
					AlarmsWork.presentAlarms &= ~ALARM_FIO2_LESSTHAN_18;						
				}
				else
#endif
				{
					AlarmCheckO2Sensor(MonitorPara.FiO2Value,Paramagnetic_O2_Flag);
					if(StandbyState==0)
					{
						AlarmCheckFiO2(MonitorPara.FiO2Value,insp_flag);
					}
				}
			} 		
			/****************************************************************************************************/
		}
		else
		{
			if(AlarmsWork.presentAlarms != 0 || AlarmsWork.presentAlarms1 != 0)
			{
				AlarmsWork.presentAlarms = 0;
				AlarmsWork.presentAlarms1 = 0;
				AlarmsWork.unclearedAlarms = 0;
				AlarmsWork.unclearedAlarms1 = 0;

				AlarmPriorityIT=InquirePresentAlarmsPriority(AlarmsWork.presentAlarms,AlarmsWork.presentAlarms1);   //zy 2016.8.11
				if(AlarmPriorityIT&HIGH_PRIORITY)
				{
					AlarmPriorityIT |= 0x2001;
				}
				if(AlarmPriorityIT&MEDIUM_PRIORITY)  //sun add 20140512
				{
					AlarmPriorityIT |= 0x4002;
				}
				if(AlarmPriorityIT&LOW_PRIORITY)  //sun add 20140512
				{
					AlarmPriorityIT |= 0x4004;
				}            
#if(LOGO_FLAG == PENLON_LOGO)
				AlarmInfo = GetHighestAlarm(AlarmsWork.presentAlarms,AlarmsWork.presentAlarms1);
				if((AlarmInfo.Type == ALARM_APNEA) && (BeathSettingPara.BreathMode == PSV_MODE) && (AlarmInfo.Grade == 1))
				{                
					AlarmPriorityIT = 0;
				}     
#endif
				SendOperationMessage(ALARM_PRIORITY_CHANGED,USART3);
				//UpdateAlarmLed(InquirePresentAlarmsPriority(AlarmsWork.presentAlarms,AlarmsWork.presentAlarms1));
				UpdateAlarmLed(AlarmPriorityIT);
				SendOperationMessage(BUZZER_CONTROL_OPERATION,USART3);
			}
		}

		//------------------------------------------------------------------------------
		//AlarmCheckGasSupply(RealtimePara.AirGasPressure,RealtimePara.O2GasPressure);//cpp del 20130325
		//AlarmCheckPower(RealtimePara.BatteryVoltage);//cpp del 20130325
	}
    if (AlarmsWork.newAlarms)
    {
        AlarmsWork.presentAlarms |= AlarmsWork.newAlarms;//add new alarms to present
        AlarmsWork.unclearedAlarms |= AlarmsWork.newAlarms;//add new alarms to unclear
        SaveAlarmInfo(AlarmsWork.newAlarms,0);//save alarm information
        AlarmsWork.newAlarms = 0;
        //产生新的报警后，清除报警静音    20140421
        if(AlarmsWork.BeepState == FALSE)
        {
            if(BUTTON_IsPressed(WM_GetDialogItem(Menu_hWin.hWin, GUI_ID_BUTTON4)))
            {
                BUTTON_SetPressed(WM_GetDialogItem(Menu_hWin.hWin, GUI_ID_BUTTON4),0);//sun add 20140519
            }
            AlarmsWork.BeepState = TRUE;
			AlarmsWork.MuteTimer = 0;
            Menu_hWin.InfoWin.InfoChangeFlag |= (ALARM_INFO_CHANGED_FLAG | SILENCE_INFO_CHANGED_FLAG);
#ifdef SELFTEST
            if(!CHECK_DEBUG)
#endif
            {
                SendOperationMessage(BUZZER_CONTROL_OPERATION,USART3);
            }
            LedUpdateRegister &= ~MUTE_LED;
            SendOperationMessage(LED_UPDATE_OPERATION,USART3);   //zy  2016.8.12            
        }
        if(CHECK_DEBUG != 1)
        {
			AlarmPriorityIT=InquirePresentAlarmsPriority(AlarmsWork.presentAlarms,AlarmsWork.presentAlarms1);   //zy 2016.8.11
			if(AlarmPriorityIT&HIGH_PRIORITY)
			{
				AlarmPriorityIT |= 0x2001;
			}
			if(AlarmPriorityIT&MEDIUM_PRIORITY)  //sun add 20140512
			{
				AlarmPriorityIT |= 0x4002;
			}
			if(AlarmPriorityIT&LOW_PRIORITY)  //sun add 20140512
			{
				AlarmPriorityIT |= 0x4004;
			}
#if(LOGO_FLAG == PENLON_LOGO)
			AlarmInfo = GetHighestAlarm(AlarmsWork.presentAlarms,AlarmsWork.presentAlarms1);
			if((AlarmInfo.Type == ALARM_APNEA) && (BeathSettingPara.BreathMode == PSV_MODE) && (AlarmInfo.Grade == 1))
			{                
				AlarmPriorityIT = 0;
			}   
#endif
			SendOperationMessage(ALARM_PRIORITY_CHANGED,USART3);                   //zy  2016.8.11
			//UpdateAlarmLed(InquirePresentAlarmsPriority(AlarmsWork.presentAlarms,AlarmsWork.presentAlarms1));   //zy  2016.8.11 
			UpdateAlarmLed(AlarmPriorityIT);	
        }
    }
	if (AlarmsWork.newAlarms1)
	{
		AlarmsWork.presentAlarms1 |= AlarmsWork.newAlarms1;//add new alarms to present
		AlarmsWork.unclearedAlarms1 |= AlarmsWork.newAlarms1;//add new alarms to unclear
		SaveAlarmInfo(AlarmsWork.newAlarms1,1);//save alarm information
		AlarmsWork.newAlarms1 = 0;
		//产生新的报警后，清除报警静音    20140421
		if(AlarmsWork.BeepState == FALSE)
		{
			if(BUTTON_IsPressed(WM_GetDialogItem(Menu_hWin.hWin, GUI_ID_BUTTON4)))
			{
				BUTTON_SetPressed(WM_GetDialogItem(Menu_hWin.hWin, GUI_ID_BUTTON4),0);//sun add 20140519
			}
			AlarmsWork.BeepState = TRUE;
			AlarmsWork.MuteTimer = 0;
			Menu_hWin.InfoWin.InfoChangeFlag |= (ALARM_INFO_CHANGED_FLAG | SILENCE_INFO_CHANGED_FLAG);
#ifdef SELFTEST
			if(!CHECK_DEBUG)
#endif
			{
				SendOperationMessage(BUZZER_CONTROL_OPERATION,USART3);
			}
			LedUpdateRegister &= ~MUTE_LED;
			SendOperationMessage(LED_UPDATE_OPERATION,USART3);   //zy  2016.8.12            
		}
        if(CHECK_DEBUG != 1)
        {
			AlarmPriorityIT=InquirePresentAlarmsPriority(AlarmsWork.presentAlarms,AlarmsWork.presentAlarms1);   //zy 2016.8.11
			if(AlarmPriorityIT&HIGH_PRIORITY)
			{
				AlarmPriorityIT |= 0x2001;
			}
			if(AlarmPriorityIT&MEDIUM_PRIORITY)  //sun add 20140512
			{
				AlarmPriorityIT |= 0x4002;
			}
			if(AlarmPriorityIT&LOW_PRIORITY)  //sun add 20140512
			{
				AlarmPriorityIT |= 0x4004;
			}

#if(LOGO_FLAG == PENLON_LOGO)
			AlarmInfo = GetHighestAlarm(AlarmsWork.presentAlarms,AlarmsWork.presentAlarms1);
			if((AlarmInfo.Type == ALARM_APNEA) && (BeathSettingPara.BreathMode == PSV_MODE) && (AlarmInfo.Grade == 1))
			{                
				AlarmPriorityIT = 0;
			}   
#endif
			SendOperationMessage(ALARM_PRIORITY_CHANGED,USART3);                   //zy  2016.8.11
			//UpdateAlarmLed(InquirePresentAlarmsPriority(AlarmsWork.presentAlarms,AlarmsWork.presentAlarms1));   //zy  2016.8.11     
			UpdateAlarmLed(AlarmPriorityIT);
        }        
	}
	//有低级报警&&无高级报警 && 无其他低级报警
	if((AlarmsWork.presentAlarms1 & ALARM_NOTLATCH) && AlarmsWork.presentAlarms == 0 \
	/*&& !(AlarmsWork.presentAlarms1 & (~ALARM_NOTLATCH))*/)
	{
		AlarmsWork.unclearedAlarms1 = 0;	//低级报警不锁存
	}
	
	if((Old_PressentAlarms != AlarmsWork.presentAlarms) || (Old_PressentAlarms1 != AlarmsWork.presentAlarms1))//ylk add 20181011如果当前报警消除 则去除报警激活状态标识
	{
		if(AlarmList_CleanActive(&alarmlog_head, ((~AlarmsWork.presentAlarms) & Old_PressentAlarms), 
			 			((~AlarmsWork.presentAlarms1) & Old_PressentAlarms1)))
		{
			//发生了变化，需要更新报警菜单。
			if(MenuType==ALARM_MENU)
			{
#ifdef SPO2_SWITCH//Alarm菜单中有无SpO2参数界面
				if(MULTIPAGE_GetSelection(WM_GetDialogItem(Menu_hWin.ActivedWin,GUI_ID_MULTIPAGE0))==3) 
				{
					WM_SendMessageNoPara(MULTIPAGE_GetWindow(WM_GetDialogItem(Menu_hWin.ActivedWin,GUI_ID_MULTIPAGE0),3),WM_INIT_DIALOG);
				}
#else
				if(MULTIPAGE_GetSelection(WM_GetDialogItem(Menu_hWin.ActivedWin,GUI_ID_MULTIPAGE0))==2) 
				{
					WM_SendMessageNoPara(MULTIPAGE_GetWindow(WM_GetDialogItem(Menu_hWin.ActivedWin,GUI_ID_MULTIPAGE0),2),WM_INIT_DIALOG);
				}
#endif
			}
		}
		Menu_hWin.InfoWin.InfoChangeFlag |= (SILENCE_INFO_CHANGED_FLAG|ALARM_MUTI_CHANGED_FLAG);
	}

	Alarm_Num = GetAlarmNum(AlarmsWork.presentAlarms, AlarmsWork.presentAlarms1);
	if(Alarm_Num == 1)	//当前只剩1个报警
	{
		AlarmsWork.unclearedAlarms = AlarmsWork.presentAlarms;
		if(!(AlarmsWork.presentAlarms1 & ALARM_NOTLATCH))	//剩余的报警不是低级报警
		{
			AlarmsWork.unclearedAlarms1 = AlarmsWork.presentAlarms1;
		}
		else
		{
			AlarmsWork.unclearedAlarms1 = 0;
		}
	}
	else if(Alarm_Num == 0)
	{
	}
	else
	{
		AlarmsWork.unclearedAlarms = 0;
		AlarmsWork.unclearedAlarms1  = 0;
	}

	AlarmPriority = InquirePresentAlarmsPriority(AlarmsWork.presentAlarms,AlarmsWork.presentAlarms1);


    if ((AlarmsWork.presentAlarms) || (AlarmsWork.presentAlarms1))
    {
        if(ShowedAlarmInfo.ActiveState!=TRUE)
        {
            ShowedAlarmInfo.ActiveState=TRUE;
            changeFlag = 1;
        }
		else
		{
			HighestAlarm = GetHighestAlarm(AlarmsWork.presentAlarms,AlarmsWork.presentAlarms1);
			if ((HighestAlarm.Type != ShowedAlarmInfo.Type) || (LastAlarmPriority != AlarmPriority))
			{
				changeFlag = 1;
			}
		}
    }
    else
    {
        if(ShowedAlarmInfo.ActiveState!=FALSE)
        {
            ShowedAlarmInfo.ActiveState = FALSE;
            ShowedAlarmInfo.Grade = 0;
            AlarmEventHandle(ALARM_END);
        }
    }
    if (changeFlag)
    {
        AlarmEventHandle(ALARM_NEW);
    }

    //if((LastAlarmPriority != AlarmPriority) || ((AlarmPriority&4) && (LowAlarmPriority > LastLowAlarmPriority)))//sun add 20141224
	if( (Old_PressentAlarms != AlarmsWork.presentAlarms) || (Old_PressentAlarms1 != AlarmsWork.presentAlarms1) )
    {       
#ifdef SELFTEST
        if(!CHECK_DEBUG)//待优化  PMU程序改为只有取消静音在才发报警声的时候可去掉该行
#endif
        {
            AlarmPriorityIT=InquirePresentAlarmsPriority(AlarmsWork.presentAlarms,AlarmsWork.presentAlarms1);   //zy 2016.8.11
            if(AlarmPriorityIT&HIGH_PRIORITY)
            {
                AlarmPriorityIT |= 0x2001;
            }
            if(AlarmPriorityIT&MEDIUM_PRIORITY)  //sun add 20140512
            {
                AlarmPriorityIT |= 0x4002;
            }
            if(AlarmPriorityIT&LOW_PRIORITY)  //sun add 20140512
            {
                AlarmPriorityIT |= 0x4004;
            }
#if(LOGO_FLAG == PENLON_LOGO)            
            AlarmInfo = GetHighestAlarm(AlarmsWork.presentAlarms,AlarmsWork.presentAlarms1);
			if((AlarmInfo.Type == ALARM_APNEA) && (BeathSettingPara.BreathMode == PSV_MODE) && (AlarmInfo.Grade == 1))//PSV、窒息报警0x10、高级报警
            {
                AlarmPriorityIT = 0;
            }       
#endif
            SendOperationMessage(ALARM_PRIORITY_CHANGED,USART3);
        }       
        //UpdateAlarmLed(AlarmPriority);
		UpdateAlarmLed(AlarmPriorityIT);//ylk md 20190712
    }
	Old_PressentAlarms = AlarmsWork.presentAlarms;
	Old_PressentAlarms1 = AlarmsWork.presentAlarms1;
    LastAlarmPriority = AlarmPriority;
    LastLowAlarmPriority = LowAlarmPriority;//sun add 20141224
    LastStandbyState = StandbyState;//  20130325
}

/***************************************************************************************************
* @brief 获得报警记录信息
* @para
* @retval
*/
void GetAlarmRecordInfo(WM_HWIN hItem,struct list_node *tmp_node,char Pointer[]) 
{
	struct list_node E2ROM_Info;
	Int16U cnt = 0;

    
	E2ROM_Info.RecordTime.RTC_Year = tmp_node->RecordTime.RTC_Year;
	E2ROM_Info.RecordTime.RTC_Mon  = tmp_node->RecordTime.RTC_Mon;
	E2ROM_Info.RecordTime.RTC_Mday = tmp_node->RecordTime.RTC_Mday;
	E2ROM_Info.RecordTime.RTC_Hour = tmp_node->RecordTime.RTC_Hour;
	E2ROM_Info.RecordTime.RTC_Min  = tmp_node->RecordTime.RTC_Min;
	E2ROM_Info.AlarmType = tmp_node->AlarmType;
	E2ROM_Info.StateFlag = tmp_node->StateFlag;
	E2ROM_Info.AlarmAA_ID = tmp_node->AlarmAA_ID;
    
	if(E2ROM_Info.AlarmAA_ID & 0x80)
	{
		if(E2ROM_Info.AlarmType>=ALARM_NUM1) //31 //cpp md 20130709
		{
			E2ROM_Info.AlarmType = 0;
		}
	}
	else
	{
		if(E2ROM_Info.AlarmType>=ALARM_NUM) //31 //cpp md 20130709
		{
			E2ROM_Info.AlarmType = 0;
		}
	}
    InvertData2Info(E2ROM_Info,Pointer);    //return(String);
 	LISTBOX_AddStringWithBitmap(hItem, Pointer, AlarmList_GetActive(&E2ROM_Info)?1:0);
}
/*
****************************************************************************************************
功能描述：
入口参数：
出口参数：
****************************************************************************************************
*/
#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C)
const char * searchAAalarm(Int32U type, Int8U AAId)
{
	if((((Int64U)1)<<type)==ALARM_FIAA_HIGH)
	{
		switch(AAId & 0x0F)
		{
		case AX_Hal:
			return AlarmContent2[0];
		case AX_Enf:
			return AlarmContent2[1];
		case AX_Iso:
			return AlarmContent2[2];
		case AX_Sev:
			return AlarmContent2[3];
		case AX_Des:
			return AlarmContent2[4];
		default:
			return AlarmContent2[20];
		}
	}
	else if ((((Int64U)1)<<type)==ALARM_ETAA_HIGH)
	{
		switch(AAId & 0x0F)
		{
		case AX_Hal:
			return AlarmContent2[5];
		case AX_Enf:
			return AlarmContent2[6];
		case AX_Iso:
			return AlarmContent2[7];
		case AX_Sev:
			return AlarmContent2[8];
		case AX_Des:  //sun add 20140605
			return AlarmContent2[9];
		default:
			return AlarmContent2[21];
		}
	}
	else if ((((Int64U)1)<<type)==ALARM_FIAA_LOW)
	{
		switch(AAId & 0x0F)
		{
		case AX_Hal:
			return AlarmContent2[10];
		case AX_Enf:
			return AlarmContent2[11];
		case AX_Iso:
			return AlarmContent2[12];
		case AX_Sev:
			return AlarmContent2[13];
		case AX_Des:  //sun add 20140605
			return AlarmContent2[14];
		default:
			return AlarmContent2[22];
		}
	}
	else if ((((Int64U)1)<<type)==ALARM_ETAA_LOW)
	{
		switch(AAId & 0x0F)
		{
		case AX_Hal:
			return AlarmContent2[15];
		case AX_Enf:
			return AlarmContent2[16];
		case AX_Iso:
			return AlarmContent2[17];
		case AX_Sev:
			return AlarmContent2[18];
		case AX_Des:  //sun add 20140605
			return AlarmContent2[19];
		default:
			return AlarmContent2[23];
		}
	}
	else
	{
		return "";
	}
	return "";
}
#endif
/***************************************************************************************************
* @brief  将报警数据(时间，类型) 转换成报警文字信息 
* @para
* @retval
*/
void InvertData2Info(struct list_node Info,char pText1[])
{
	char pText[100] = {0};//djq modify 20170113  从40改为100
	
	const char *context = NULL;	

	memset(pText, ' ', 30);
	sprintf(pText+0,"%4d",Info.RecordTime.RTC_Year);//year
	pText[4] = '/';
	sprintf(pText+5,"%02d",Info.RecordTime.RTC_Mon);//month
	pText[7] = '/';
	sprintf(pText+8,"%02d",Info.RecordTime.RTC_Mday);//day
	pText[10] = ' ';
	sprintf(pText+12,"%02d",Info.RecordTime.RTC_Hour);//hour
	pText[14] = ':';
	sprintf(pText+15,"%02d",Info.RecordTime.RTC_Min);//minute
	pText[17] = ' ';
	pText[18] = ' ';
	pText[19] = ' ';
	pText[20] = ' ';
	pText[21] = ' ';
	pText[22] = ' ';
	pText[23] = ' ';
	pText[24] = ' ';
	pText[25] = ' ';
	pText[26] = ' ';

	if(Info.AlarmAA_ID & 0x80)//中级和低级报警
	{
#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C)
		if(((((Int64U)1)<<Info.AlarmType)==ALARM_ETAA_LOW)
			||((((Int64U)1)<<Info.AlarmType) == ALARM_FIAA_LOW))//AA
		{
			context = searchAAalarm(Info.AlarmType, Info.AlarmAA_ID); //sun add 20140610
		}
		else
#endif
		{
			context = AlarmContent1[Info.AlarmType];
		}
	}
	else//高级报警
	{
#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C)
		if(((((Int64U)1)<<Info.AlarmType) == ALARM_FIAA_HIGH)
			||((((Int64U)1)<<Info.AlarmType) == ALARM_ETAA_HIGH))//AA
		{
			context = searchAAalarm(Info.AlarmType, Info.AlarmAA_ID); //sun add 20140610
		}
		else
#endif
		{
			context = AlarmContent[Info.AlarmType];
		}
	}
	sprintf(pText1,"%s%s",pText,context);
}
/***************************************************************************************************
* @brief  查询报警记录信息
* @para
* @retval
*/
void InquiryAlarmRecordInfo(Int8U type)
{
	WM_HWIN hWin;
	if(AlarmRecordList.Count<MAX_RECORD_NUM)
	{
		AlarmRecordList.Count ++;
		AlarmRecordList.Head ++;
		if(MenuType==ALARM_MENU)
		{
			hWin = WM_GetDialogItem(Menu_hWin.ActivedWin,GUI_ID_MULTIPAGE0);
#ifdef SPO2_SWITCH //20180315 ylk add
			if(MULTIPAGE_GetSelection(hWin)==3)
			{
				ListBoxInfoUpdate(MULTIPAGE_GetWindow(hWin,3),&AlarmRecordList,ADD_ITEM,type);
			}
#else
			if(MULTIPAGE_GetSelection(hWin)==2)
			{
				ListBoxInfoUpdate(MULTIPAGE_GetWindow(hWin,2),&AlarmRecordList,ADD_ITEM,type);
			}
#endif
		}
	}
	else
	{
		if(AlarmRecordList.Head>=MAX_RECORD_NUM)
		{
			AlarmRecordList.Head = 1;
			AlarmRecordList.Rear = 2;
		}
		else
		{
			AlarmRecordList.Head ++;
			AlarmRecordList.Rear ++;
		}
		if(MenuType==ALARM_MENU)
		{
			hWin = WM_GetDialogItem(Menu_hWin.ActivedWin,GUI_ID_MULTIPAGE0);
// 			if(MULTIPAGE_GetSelection(hWin)==3)
// 			{
// 				ListBoxInfoUpdate(MULTIPAGE_GetWindow(hWin,3),&AlarmRecordList,INSERT_ITEM,type);
// 			}
#ifdef SPO2_SWITCH 
			if(MULTIPAGE_GetSelection(hWin)==3)
			{
				ListBoxInfoUpdate(MULTIPAGE_GetWindow(hWin,3),&AlarmRecordList,INSERT_ITEM,type);
			}
#else
			if(MULTIPAGE_GetSelection(hWin)==2)
			{
				ListBoxInfoUpdate(MULTIPAGE_GetWindow(hWin,2),&AlarmRecordList,INSERT_ITEM,type);
			}
#endif
		}
	}
	AlarmList_Inster(&alarmlog_head,type);
	WriteAlarmRecordHeadInfo(&AlarmRecordList);
	WriteAlarmRecordInfo(&AlarmRecord,type,AlarmRecordList.Head-1);
}
/***************************************************************************************************
* @brief  将报警信息写入EE
* @para
* @retval
*/
void WriteAlarmRecordInfo(struct list_node *Record,Int8U type,Int16U num)
{
	Int16U addr;
	addr = ALARM_RECORD_ADDR + num * RECORD_SIZE;
	RTEEPROMwrite(addr,(Record->RecordTime.RTC_Year)&0xFF);
	RTEEPROMwrite(addr+1,(Record->RecordTime.RTC_Year>>8)&0xFF);
	RTEEPROMwrite(addr+2,Record->RecordTime.RTC_Mon);
	RTEEPROMwrite(addr+3,Record->RecordTime.RTC_Mday);
	RTEEPROMwrite(addr+4,Record->RecordTime.RTC_Hour);
	RTEEPROMwrite(addr+5,Record->RecordTime.RTC_Min);
	RTEEPROMwrite(addr+6,type);
	RTEEPROMwrite(addr+7,Record->AlarmAA_ID); //sun add 20140610   
}
/***************************************************************************************************
* @brief  
* @para
* @retval
*/
void WriteAlarmRecordHeadInfo(ALARM_RECORD_LIST *ListHead)
{
	RTEEPROMwrite(ALARM_RECORD_LIST_ADDR,(ListHead->Head)&0xFF);
	RTEEPROMwrite(ALARM_RECORD_LIST_ADDR+1,(ListHead->Head>>8)&0xFF);
	RTEEPROMwrite(ALARM_RECORD_LIST_ADDR+2,(ListHead->Rear)&0xFF);
	RTEEPROMwrite(ALARM_RECORD_LIST_ADDR+3,(ListHead->Rear>>8)&0xFF);
	RTEEPROMwrite(ALARM_RECORD_LIST_ADDR+4,(ListHead->Count)&0xFF);
	RTEEPROMwrite(ALARM_RECORD_LIST_ADDR+5,(ListHead->Count>>8)&0xFF);
}
/***************************************************************************************************
* @brief  清除参数报警
* @para
* @retval
*/
void ClearParameterAlarm(void)
{
	Int8U num;
	Int64U HighAlarm = 0;
	Int64U LowAlarm  = 0;

	for(num=0; num<ALARM_NUM; num++) //32 //cpp md 20130709
	{
		if(AlarmsWork.presentAlarms&(((Int64U)1)<<num)) //(1<<num) //cpp md 20130709
		{
			if( ((((Int64U)1)<<num)!=ALARM_BATTERY_LOW)
//				&& ((((Int64U)1)<<num)!=ALARM_BATTERY_FAIL)
				&& ((((Int64U)1)<<num)!=ALARM_OXY_SOURCE_LOW)
				&& ((((Int64U)1)<<num)!=ALARM_BDU_COMM_FAULT)//ylk add 20181115 BDU通讯故障报警
				&& ((((Int64U)1)<<num)!=ALARM_BATTERY_FAULT_HIGH)//ylk add 20181115 电池故障报警 高级
				&& ((((Int64U)1)<<num)!=ALARM_FIO2_LESSTHAN_18) // no longer clear the alarm when enter standby
				)
			{
				AlarmsWork.presentAlarms &= ~(((Int64U)1)<<num);
				AlarmsWork.unclearedAlarms &= ~(((Int64U)1)<<num);
				HighAlarm |= (((Int64U)1)<<num);//djq add 20170531
			}
		}
	}
	for(num=0; num<ALARM_NUM1; num++)
	{
		if(AlarmsWork.presentAlarms1&(((Int64U)1)<<num))
		{
			if(  ((((Int64U)1)<<num)!=ALARM_AC_FAIL)
#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C) 
				&& ((((Int64U)1)<<num)!=ALARM_ABSORBER_DISCONNECT)
				&& ((((Int64U)1)<<num)!=ALARM_GAS_COMM_FAULT)
				&& ((((Int64U)1)<<num)!=ALARM_AIR_SOURCE_LOW)
//				&& ((((Int64U)1)<<num)!=ALARM_BATTERY_FAULT_LOW)//ylk add 20181115 电池故障报警 低级 
#endif
				&& ((((Int64U)1)<<num)!=ALARM_SPO2_SENSOR_DISCONNECT)
				&& ((((Int64U)1)<<num)!=ALARM_ETCO2_MODULE_OFF)
				&& ((((Int64U)1)<<num)!=ALARM_SPO2_MODULE_OFF)
				&& ((((Int64U)1)<<num)!=ALARM_O2_MODULE_OFF)
				)
			{
				AlarmsWork.presentAlarms1 &= ~(((Int64U)1)<<num);
				AlarmsWork.unclearedAlarms1 &= ~(((Int64U)1)<<num);
				LowAlarm |= (((Int64U)1)<<num);//djq add 20170531
			}
		}
	}
	AlarmList_CleanActive(&alarmlog_head,HighAlarm,LowAlarm);
	LowAlarmPriority &= ~0x01;//sun add 20141224
}
/*
****************************************************************************************************
* @brief  清除ACGO模式下其他报警 //  20130604
* @para   
* @retval 
*/
void ClearAcgoModeOtherAlarm(void)
{
#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C) 
    Int8U num;

    for(num=0; num<ALARM_NUM; num++)
    {
        if(AlarmsWork.presentAlarms&((Int64U)1<<num))
        {
            if((((Int64U)1<<num)!=ALARM_BATTERY_LOW)
//             && (((Int64U)1<<num)!=ALARM_BATTERY_FAIL)
             && (((Int64U)1<<num)!=ALARM_OXY_SOURCE_LOW)
             && (((Int64U)1<<num)!=ALARM_FIO2_HIGH)
             && (((Int64U)1<<num)!=ALARM_FIO2_LOW)
			 && (((Int64U)1<<num)!=ALARM_FIO2_LESSTHAN_18)
             && (((Int64U)1<<num)!=ALARM_FIAA_HIGH)
             && (((Int64U)1<<num)!=ALARM_ETAA_HIGH)                   
             && (((Int64U)1<<num)!=ALARM_ETCO2_HIGH)
             && (((Int64U)1<<num)!=ALARM_FICO2_HIGH)                 
             && (((Int64U)1<<num)!=ALARM_FIN2O_HIGH)
			 && (((Int64U)1<<num)!=ALARM_BDU_COMM_FAULT)//ylk add 20181115 BDU通讯故障报警                 
			 && (((Int64U)1<<num)!=ALARM_BATTERY_FAULT_HIGH)//ylk add 20181115 电池故障报警 高级
             )
            {
                AlarmsWork.presentAlarms &= ~((Int64U)1<<num);
                AlarmsWork.unclearedAlarms &= ~((Int64U)1<<num);
            }
        }
    }

	for(num=0; num<ALARM_NUM1; num++)
	{
		if(AlarmsWork.presentAlarms1&((Int64U)1<<num))
		{
			if(   (((Int64U)1<<num)!=ALARM_AC_FAIL)
				& (((Int64U)1<<num)!=ALARM_ETCO2_MODULE_OFF)
				& (((Int64U)1<<num)!=ALARM_SPO2_SENSOR_DISCONNECT)
				& (((Int64U)1<<num)!=ALARM_O2_MODULE_OFF)
				& (((Int64U)1<<num)!=ALARM_SPO2_MODULE_OFF)
				& (((Int64U)1<<num)!=ALARM_FIAA_LOW)
				& (((Int64U)1<<num)!=ALARM_ETAA_LOW)                  
				& (((Int64U)1<<num)!=ALARM_ETCO2_LOW)                
				& (((Int64U)1<<num)!=ALARM_FIN2O_LOW) 
#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C)
				& (((Int64U)1<<num)!=ALARM_AA_LESSTHAN_3MAC)
				& (((Int64U)1<<num)!=ALARM_AA_GTE_3MAC)
				& (((Int64U)1<<num)!=ALARM_ABSORBER_DISCONNECT)
//				& (((Int64U)1<<num)!=ALARM_BATTERY_FAULT_LOW)//ylk add 20181115 电池故障报警 低级  
#endif
				)
			{
				AlarmsWork.presentAlarms1 &= ~((Int64U)1<<num);
				AlarmsWork.unclearedAlarms1 &= ~((Int64U)1<<num);
			}
		}
	}
#endif
}

//获得当前报警数量
Int8U PresentAlarm_Num(Int64U PresentAlarm,Int64U PresentAlarm1)
{
	char num = 0,cnt = 0;
	if((PresentAlarm == 0) && (PresentAlarm1 == 0))
	{
		return 0;
	}
	for(cnt = 0;cnt < ALARM_NUM;cnt++)
	{
		if(PresentAlarm & 0x1)
		{
			num++;
		}
		if(PresentAlarm1 & 0x1)
		{
			num++;
		}
		PresentAlarm >>= 1;
		PresentAlarm1 >>= 1;
	}
	return num;
}
/*****************************************************************************************************
功能描述：将EEPROM中的报警日志导入到内存链表当中
入口参数：
出口参数：
****************************************************************************************************
*/
void Init_AlarmLog_As_Ram(void)//djq add 2013-12-10
{
	struct list_node *tmp_node = alarmlog_head.head;  
	Int16U addr;
	U8 num = 0;
	U16 cnt = 0;
	RTC_TIMER OldTime;
	Int8U count = 0;//1分钟内报警次数累计。
	if(AlarmRecordList.Count)
	{
		for(cnt=1; cnt<=AlarmRecordList.Count; cnt++)
		{                                   
			if(AlarmRecordList.Head>AlarmRecordList.Rear)
			{
				addr = ALARM_RECORD_ADDR + (AlarmRecordList.Head - cnt) * RECORD_SIZE;
			}
			else
			{
				if(AlarmRecordList.Head >= cnt)
				{
					addr = ALARM_RECORD_ADDR + (AlarmRecordList.Head - cnt) * RECORD_SIZE;
				}
				else
				{
					addr = ALARM_RECORD_ADDR + ( MAX_RECORD_NUM - cnt +AlarmRecordList.Head) * RECORD_SIZE;
				}
			}

			tmp_node->RecordTime.RTC_Year = RTEEPROMread(addr+Year_Low_Addr) | (RTEEPROMread(addr+Year_High_Addr) << 8);
			tmp_node->RecordTime.RTC_Mon  = RTEEPROMread(addr+Mon_Addr);
			tmp_node->RecordTime.RTC_Mday = RTEEPROMread(addr+Day_Addr);
			tmp_node->RecordTime.RTC_Hour = RTEEPROMread(addr+Hour_Addr);
			tmp_node->RecordTime.RTC_Min  = RTEEPROMread(addr+Min_Addr);
			tmp_node->AlarmType  = RTEEPROMread(addr+Type_Addr);     
			tmp_node->AlarmAA_ID = RTEEPROMread(addr+AA_Id_Addr);                      

			if(AlarmList_TimeCompare(tmp_node->RecordTime,OldTime) == 0)
			{
				count++;
			}
			else
			{
				count = 0;
			}
			AlarmList_SetStateNum(tmp_node,count);
			OldTime = tmp_node->RecordTime;
			//          设置激活Active状态
			AlarmList_SetActive(tmp_node,0);//全部初始化为非激活状态
			tmp_node = tmp_node->next;
		}
	}
}
/*
****************************************************************************************************
功能描述：初始化报警日子链表的节点和数据
入口参数：
出口参数：
****************************************************************************************************
*/
void AlarmList_Init(void)//djq add 2013-12-10初始化链表
{
	Int16U cnt ;
	alarmlog_head.head = &alarmlog_node[0];
	alarmlog_head.RankType = RankType_Time_Fall;//djq add 20170112默认为时间降序
	alarmlog_head.LockType = Lock;            
	for(cnt = 0;cnt < MAX_RECORD_NUM - 1;cnt++){
		alarmlog_node[cnt].next = &alarmlog_node[cnt+1];
	}
	alarmlog_node[MAX_RECORD_NUM - 1].next = NULL;
	Init_AlarmLog_As_Ram();
	alarmlog_head.LockType = UnLock;            
}
/*
****************************************************************************************************
功能描述：每次产生新的警报时，将警报插入到链表头
入口参数：alarm_head:报警日志内存链表的表头指针
          type:新增加的警报的类型
出口参数：
****************************************************************************************************
*/
void AlarmList_Inster(struct list_head *alarm_head,Int8U type)//djq add 2013-12-10
{
	Int8U cnt = 0;
	struct list_node *tmp_node = alarm_head->head, *end_node;
	while(tmp_node->next->next != NULL)
	{
		tmp_node = tmp_node->next;   
	}
	end_node = tmp_node->next;
	end_node->next = alarmlog_head.head;
	alarm_head->head = end_node;
	tmp_node->next = NULL;
	end_node->RecordTime.RTC_Year = AlarmRecord.RecordTime.RTC_Year;
	end_node->RecordTime.RTC_Mon  = AlarmRecord.RecordTime.RTC_Mon;
	end_node->RecordTime.RTC_Mday = AlarmRecord.RecordTime.RTC_Mday;
	end_node->RecordTime.RTC_Hour = AlarmRecord.RecordTime.RTC_Hour;
	end_node->RecordTime.RTC_Min  = AlarmRecord.RecordTime.RTC_Min;
	end_node->AlarmType  = type; 
	end_node->AlarmAA_ID = AlarmRecord.AlarmAA_ID;

	AlarmList_SetActive(end_node,1);
}

/*
****************************************************************************************************
功能描述：AlarmList_TimeCompare 比较两个时间的先后
入口参数：Time1，Time2
出口参数：1：Time1>Time2     0：Time1=Time2    -1：Time1<Time2
改进：可以考虑读取结构体RTC_TIMER到数组中，对数组进行循环比较。
****************************************************************************************************
*/
int AlarmList_TimeCompare(RTC_TIMER Time1,RTC_TIMER Time2)
{
	int ret = 0;
	//年份比较
	if(Time1.RTC_Year > Time2.RTC_Year)
	{
		ret = 1;
	}
	else if(Time1.RTC_Year < Time2.RTC_Year)
	{
		ret = -1;
	}
	//月份比较
	else
	{
		if(Time1.RTC_Mon > Time2.RTC_Mon)
		{
			ret = 1;
		}
		else if(Time1.RTC_Mon < Time2.RTC_Mon)
		{
			ret = -1;
		}
		//日期比较
		else
		{
			if(Time1.RTC_Mday > Time2.RTC_Mday)
			{
				ret = 1;
			}
			else if(Time1.RTC_Mday < Time2.RTC_Mday)
			{
				ret = -1;
			}
			//小时比较
			else
			{
				if(Time1.RTC_Hour > Time2.RTC_Hour)
				{
					ret = 1;
				}
				else if(Time1.RTC_Hour < Time2.RTC_Hour)
				{
					ret = -1;
				}
				//分钟比较
				else
				{
					if(Time1.RTC_Min > Time2.RTC_Min)
					{
						ret = 1;
					}
					else if(Time1.RTC_Min < Time2.RTC_Min)
					{
						ret = -1;
					}
					else
					{
						ret = 0;
					}
				}
			}
		}
	}
	return ret;
}



/*
****************************************************************************************************
模块功能：区分1分钟之内多个报警
****************************************************************************************************
*/
void AlarmList_SetStateNum(struct list_node *node,Int8U Num)
{
	node->StateFlag &= (0xC0);
	node->StateFlag |= (0x3F & Num);
}
Int8U AlarmList_GetStateNum(struct list_node *node)
{
	return (node->StateFlag & 0x3F);
}

/*
****************************************************************************************************
模块功能：设置和获取Active状态   0:非激活   1：激活
****************************************************************************************************
*/
void AlarmList_SetActive(struct list_node *node,Int8U Active)
{
	if(Active)
	{
		node->StateFlag |= 0x80;
	}
	else
	{
		node->StateFlag &= 0x7F;
	}
}
Int8U AlarmList_GetActive(struct list_node *node)
{
	return (node->StateFlag & 0x80)?1:0;
}

/*
****************************************************************************************************
功能描述：AlarmList_RankCompare 比较两个报警的级别
入口参数：Alarm1，Alarm2
出口参数：1：Alarm1>Alarm2     0：Alarm1=Alarm2    -1：Alarm1<Alarm2
****************************************************************************************************
*/
int AlarmList_RankCompare(struct list_node *Alarm1,struct list_node *Alarm2)
{
	Int8U AlarmBit1 = (Alarm1->AlarmAA_ID&0x80),AlarmBit2 = (Alarm2->AlarmAA_ID&0x80);
	if(AlarmBit1 < AlarmBit2)//报警级别判断，高级为0x00，低级为0x80
	{
		return 1;
	}
	else if(AlarmBit1 > AlarmBit2)
	{
		return -1;
	}
	else//如果报警级别相同，则判断报警优先级
	{
		if(Alarm1->AlarmType > Alarm2->AlarmType)
		{
			return 1;
		}
		else if(Alarm1->AlarmType < Alarm2->AlarmType)
		{
			return -1;
		}
		else
		{
			return 0;
		}
	}
}
/*
****************************************************************************************************
功能描述：AlarmList_CleanActive 清除消除的报警的Active标志
入口参数：AlarmHigh，AlarmLow  需要清除的高级和中低级报警的标志
出口参数：ret   0：没有变化    非0：消除报警的个数
****************************************************************************************************
*/
Int8U AlarmList_CleanActive(struct list_head *alarm_head,Int64U AlarmHigh,Int64U AlarmLow)
{
	Int8U i = 0,ret = 0;
	struct list_node *tmp_node = alarm_head->head, node;

	if(AlarmHigh)//如果需要消除高级报警
	{
		while(AlarmHigh)
		{
			if(AlarmHigh&(((Int64U)1)<<i))
			{
				//得到当前消除报警的结构体，用于对比报警级别
				node.AlarmAA_ID = 0;
				node.AlarmType = i;

				tmp_node = alarm_head->head;
				while(tmp_node->next->next != NULL)//遍历链表
				{
					if(!AlarmList_RankCompare(tmp_node,&node))//对比是否为同一个报警。
					{
						AlarmList_SetActive(tmp_node,0);//关闭已经激活的报警
						ret++;
					}
					tmp_node = tmp_node->next;   
				}
				AlarmHigh &= ~(((Int64U)1)<<i);
			}
			i++;
		}
	}
	if(AlarmLow)//如果需要消除低级报警
	{
		while(AlarmLow)
		{
			if(AlarmLow&(((Int64U)1)<<i))
			{
				//得到当前消除报警的结构体，用于对比报警级别
				node.AlarmAA_ID = 0x80;
				node.AlarmType = i;

				tmp_node = alarm_head->head;
				while(tmp_node->next->next != NULL)//遍历链表
				{
					if(!AlarmList_RankCompare(tmp_node,&node))//对比是否为同一个报警。
					{
						AlarmList_SetActive(tmp_node,0);//关闭已经激活的报警
						ret++;
					}
					tmp_node = tmp_node->next;   
				}
				AlarmLow &= ~(((Int64U)1)<<i);
			}
			i++;
		}
	}
	return ret;
}

void AA_Module_String(void)
{
	static enum AA_GAS_ID AA1_temp = AA_ID_NONE;
	static enum AA_GAS_ID AA2_temp = AA_ID_NONE;
	if(AA_all_data.AA_detail.AA_AA1.ID != AA1_temp)
	{
 		switch(AA_all_data.AA_detail.AA_AA1.ID)
		{
		case AA_ID_HAL:
			AA_all_data.AA_detail.AA1_Color = HALCOLOR;
			strcpy(AA_all_data.AA_detail.AA1_Type_Text, Sys_Lib_GetDispStr(DP_HAL));
			break;
		case AA_ID_ENF:
			AA_all_data.AA_detail.AA1_Color = ENFCOLOR;
			strcpy(AA_all_data.AA_detail.AA1_Type_Text, Sys_Lib_GetDispStr(DP_ENF));
			break;
		case AA_ID_ISO:
			AA_all_data.AA_detail.AA1_Color = ISOCOLOR;
			strcpy(AA_all_data.AA_detail.AA1_Type_Text, Sys_Lib_GetDispStr(DP_ISO));
			break;
		case AA_ID_SEV:
			AA_all_data.AA_detail.AA1_Color = SEVCOLOR;
			strcpy(AA_all_data.AA_detail.AA1_Type_Text, Sys_Lib_GetDispStr(DP_SEV));
			break;
		case AA_ID_DES:  
			AA_all_data.AA_detail.AA1_Color = DESCOLOR;
			strcpy(AA_all_data.AA_detail.AA1_Type_Text, Sys_Lib_GetDispStr(DP_DES));
			break;
		default:
			strcpy(AA_all_data.AA_detail.AA1_Type_Text, Sys_Lib_GetDispStr(DP_AA1));
		}
		AA1_temp = AA_all_data.AA_detail.AA_AA1.ID;
	}
 	if(AA_all_data.AA_detail.AA_AA2.ID != AA2_temp)
	{
		switch(AA_all_data.AA_detail.AA_AA2.ID)
		{
		case AA_ID_HAL:
			AA_all_data.AA_detail.AA2_Color = HALCOLOR;
			strcpy(AA_all_data.AA_detail.AA2_Type_Text, Sys_Lib_GetDispStr(DP_HAL));
			break;
		case AA_ID_ENF:
			AA_all_data.AA_detail.AA2_Color = ENFCOLOR;
			strcpy(AA_all_data.AA_detail.AA2_Type_Text, Sys_Lib_GetDispStr(DP_ENF));
			break;
		case AA_ID_ISO:
			AA_all_data.AA_detail.AA2_Color = ISOCOLOR;
			strcpy(AA_all_data.AA_detail.AA2_Type_Text, Sys_Lib_GetDispStr(DP_ISO));
			break;
		case AA_ID_SEV:
			AA_all_data.AA_detail.AA2_Color = SEVCOLOR;
			strcpy(AA_all_data.AA_detail.AA2_Type_Text, Sys_Lib_GetDispStr(DP_SEV));
			break;
		case AA_ID_DES:  
			AA_all_data.AA_detail.AA2_Color = DESCOLOR;
			strcpy(AA_all_data.AA_detail.AA2_Type_Text, Sys_Lib_GetDispStr(DP_DES));
			break;
		default:
			strcpy(AA_all_data.AA_detail.AA2_Type_Text, Sys_Lib_GetDispStr(DP_AA2));
		}
		AA2_temp = AA_all_data.AA_detail.AA_AA2.ID;
	}
}

void AA_Module_Alarm_Progress(void)
{
	Int8U Err_Num = 0;

	if(CHECK_DEBUG != 1)
	{
		if(AA_all_data.AA_state.dev_info.AA_name == PhaseIn)
		{
			if(AA_all_data.AA_state.module_err_message[0] & 0x01)  //零点错误
			{
				if(!(AlarmsWork.presentAlarms1 & ALARM_PHASEIN_ZERO_OR))
				{
					AlarmsWork.newAlarms1 |= ALARM_PHASEIN_ZERO_OR;
				}
				Err_Num++;
			}
			else
			{
				AlarmsWork.presentAlarms1 &= ~ALARM_PHASEIN_ZERO_OR;
			}
			if(AA_all_data.AA_state.module_err_message[0] & 0x04)   //软件版本错误，重启传感器
			{
				if(!(AlarmsWork.presentAlarms1 & ALARM_PHASEIN_SW_ERR))
				{
					AlarmsWork.newAlarms1 |= ALARM_PHASEIN_SW_ERR;  
				}
				Err_Num++;
			}
			else
			{
				AlarmsWork.presentAlarms1 &= ~ALARM_PHASEIN_SW_ERR;
			}
			if(AA_all_data.AA_state.module_err_message[0] & 0x10)   //硬件版本错误，更换传感器
			{
				if(!(AlarmsWork.presentAlarms1 & ALARM_PHASEIN_HW_ERR))
				{              
					AlarmsWork.newAlarms1 |= ALARM_PHASEIN_HW_ERR;
				}
				Err_Num++;
			}
			else
			{
				AlarmsWork.presentAlarms1 &= ~ALARM_PHASEIN_HW_ERR;
			}
			if(AA_all_data.AA_state.module_err_message[0] & 0x20)   //电机速度超限
			{
				if(!(AlarmsWork.presentAlarms1 & ALARM_PHASEIN_MOTOR_ERR))
				{
					AlarmsWork.newAlarms1 |= ALARM_PHASEIN_MOTOR_ERR;
				}
				Err_Num++;
			}
			else
			{
				AlarmsWork.presentAlarms1 &= ~ALARM_PHASEIN_MOTOR_ERR;
			}
			if(AA_all_data.AA_state.module_err_message[0] & 0x02)   //出厂校准缺失
			{
				if(!(AlarmsWork.presentAlarms1 & ALARM_PHASEIN_UN_CAL))
				{
					AlarmsWork.newAlarms1 |= ALARM_PHASEIN_UN_CAL;
				}
				Err_Num++;
			}
			else
			{
				AlarmsWork.presentAlarms1 &= ~ALARM_PHASEIN_UN_CAL;
			}
			if(AA_all_data.AA_state.module_err_message[0] & 0x08)   //内部温度高
			{
				if(!(AlarmsWork.presentAlarms1 & ALARM_PHASEIN_TEMP_OR))
				{ 
					AlarmsWork.newAlarms1 |= ALARM_PHASEIN_TEMP_OR;
				}
				Err_Num++;
			}
			else
			{
				AlarmsWork.presentAlarms1 &= ~ALARM_PHASEIN_TEMP_OR;
			}
			if(Masimo_EtCO2_Config_On)//Masimo 主流
			{
				if(AA_all_data.AA_state.module_err_message[0] & 0x40)   //更换适配器
				{
					if(!(AlarmsWork.presentAlarms1 & ALARM_GAS_REPL_ADP))
					{
						AlarmsWork.newAlarms1 |= ALARM_GAS_REPL_ADP;
					}
					Err_Num++;
				}
				else
				{
					AlarmsWork.presentAlarms1 &= ~ALARM_GAS_REPL_ADP;
				}
				if(AA_all_data.AA_state.module_err_message[0] & 0x80)   //无适配器，请连接适配器
				{
					if(!(AlarmsWork.presentAlarms1 & ALARM_PHASEIN_NO_ADP))
					{
						AlarmsWork.newAlarms1 |= ALARM_PHASEIN_NO_ADP;
					}
					Err_Num++;
				}
				else
				{
					AlarmsWork.presentAlarms1 &= ~ALARM_PHASEIN_NO_ADP;
				}
			}
			else//Masimo 旁流
			{
				if(AA_all_data.AA_state.module_err_message[0] & 0x40)   //采样管阻塞
				{
					if(!(AlarmsWork.presentAlarms1 & ALARM_ETCO2_LINE_OCCLUSION))
					{
						AlarmsWork.newAlarms1 |= ALARM_ETCO2_LINE_OCCLUSION;
					}
					Err_Num++;
				}
				else
				{
					AlarmsWork.presentAlarms1 &= ~ALARM_ETCO2_LINE_OCCLUSION;
				}
				if(AA_all_data.AA_state.module_err_message[0] & 0x80)   //无适配器，请连接适配器
				{
					if(!(AlarmsWork.presentAlarms1 & ALARM_PHASEIN_NO_TUBE))
					{
						AlarmsWork.newAlarms1 |= ALARM_PHASEIN_NO_TUBE;
					}
					Err_Num++;
				}
				else
				{
					AlarmsWork.presentAlarms1 &= ~ALARM_PHASEIN_NO_TUBE;
				}
				if(AA_all_data.AA_state.module_err_message[1] & 0x04)   //无适配器，请连接适配器
				{
					if(!(AlarmsWork.presentAlarms1 & ALARM_PHASEIN_O2CALIB))
					{
						AlarmsWork.newAlarms1 |= ALARM_PHASEIN_O2CALIB;
					}
					Err_Num++;
				}
				else
				{
					AlarmsWork.presentAlarms1 &= ~ALARM_PHASEIN_O2CALIB;
				}

				if(AA_all_data.AA_state.module_err_message[1] & 0x01)//氧浓度传感器错误
				{
					if(!(AlarmsWork.presentAlarms1 & ALARM_PHASEIN_O2SEN_ERR))
					{
						AlarmsWork.newAlarms1 |= ALARM_PHASEIN_O2SEN_ERR;
					}
					Err_Num++;
				}
				else
				{
					AlarmsWork.presentAlarms1 &= ~ALARM_PHASEIN_O2SEN_ERR;
				}
				if(Irma_getState(IM_STS_O2_REPL) == IM_TRUE)//更换氧传感器
				{
					if(!(AlarmsWork.presentAlarms1 & ALARM_PHASEIN_REPL_O2))
					{
						AlarmsWork.newAlarms1 |= ALARM_PHASEIN_REPL_O2;
					}
					Err_Num++;
				}
				else
				{
					AlarmsWork.presentAlarms1 &= ~ALARM_PHASEIN_REPL_O2;
				}
			}
			if(AA_all_data.AA_state.module_err_message[4] & 0x01)   //二氧化碳超差
			{
				if(!(AlarmsWork.presentAlarms1 & ALARM_PHASEIN_CO2_OR))
				{
					AlarmsWork.newAlarms1 |= ALARM_PHASEIN_CO2_OR;
				}
				Err_Num++;
			}
			else
			{
				AlarmsWork.presentAlarms1 &= ~ALARM_PHASEIN_CO2_OR;
			}
			if(AA_all_data.AA_state.module_err_message[4] & 0x02)   //一氧化二氮超差
			{
				if(!(AlarmsWork.presentAlarms1 & ALARM_PHASEIN_N2O_OR))
				{
					AlarmsWork.newAlarms1 |= ALARM_PHASEIN_N2O_OR;
				}
				Err_Num++;
			}
			else
			{
				AlarmsWork.presentAlarms1 &= ~ALARM_PHASEIN_N2O_OR;
			}
			if(AA_all_data.AA_state.module_err_message[4] & 0x04)   //至少一种麻醉气体超差
			{
				if(!(AlarmsWork.presentAlarms1 & ALARM_PHASEIN_AX_OR))
				{                
					AlarmsWork.newAlarms1 |= ALARM_PHASEIN_AX_OR;
				}
				Err_Num++;
			}
			else
			{
				AlarmsWork.presentAlarms1 &= ~ALARM_PHASEIN_AX_OR;
			}
			if(MasimoISA_EtCO2_Config_On == 2)//Masimo 旁流
			{
				if(AA_all_data.AA_state.module_err_message[4] & 0x20)   //O2超差
				{
					if(!(AlarmsWork.presentAlarms1 & ALARM_PHASEIN_O2_OR))
					{                
						AlarmsWork.newAlarms1 |= ALARM_PHASEIN_O2_OR;
					}
					Err_Num++;
				}
				else
				{
					AlarmsWork.presentAlarms1 &= ~ALARM_PHASEIN_O2_OR;
				}
			}
			if(AA_all_data.AA_state.module_err_message[4] & 0x08)   //大气压超出运行范围
			{
				if(!(AlarmsWork.presentAlarms1 & ALARM_PHASEIN_PRESS_OR))
				{ 
					AlarmsWork.newAlarms1 |= ALARM_PHASEIN_PRESS_OR;
				}
				Err_Num++;
			}
			else
			{
				AlarmsWork.presentAlarms1 &= ~ALARM_PHASEIN_PRESS_OR;
			}
			if(AA_all_data.AA_state.module_err_message[4] & 0x10)   //麻醉气体标识和浓度不可信
			{
				if(!(AlarmsWork.presentAlarms1 & ALARM_PHASEIN_AGENTID_UNREAL))
				{ 
					AlarmsWork.newAlarms1 |= ALARM_PHASEIN_AGENTID_UNREAL;
				}
				Err_Num++;
			}
			else
			{
				AlarmsWork.presentAlarms1 &= ~ALARM_PHASEIN_AGENTID_UNREAL;
			}
		}
		else if(AA_all_data.AA_state.dev_info.AA_name == Drager)
		{
			if(AA_all_data.AA_state.module_err_message[1] & 0x04)//脱水器将满
			{
				if(!(AlarmsWork.presentAlarms1 & ALARM_DRAGER_WATRAPWILLFULL))
				{ 
					AlarmsWork.newAlarms1 |= ALARM_DRAGER_WATRAPWILLFULL;
				}
				Err_Num++;
			}
			else
			{
				AlarmsWork.presentAlarms1 &= ~ALARM_DRAGER_WATRAPWILLFULL;
			}
			if(AA_all_data.AA_state.module_err_message[1] & 0x08)//脱水器满
			{
				if(!(AlarmsWork.presentAlarms & ALARM_DRAGER_WATRAPFULL))
				{ 
					AlarmsWork.newAlarms |= ALARM_DRAGER_WATRAPFULL;
				}
				Err_Num++;
			}
			else
			{
				AlarmsWork.presentAlarms &= ~ALARM_DRAGER_WATRAPFULL;
			}
			//        if(AA_all_data.AA_state.module_err_message[1] & 0x10)//脱水器未连接
			//        {
			//            if(!(AlarmsWork.presentAlarms & ALARM_DRAGER_WATRAPNOCONNET))
			//            { 
			//                AlarmsWork.newAlarms |= ALARM_DRAGER_WATRAPNOCONNET;
			//            }
			//            Err_Num++;
			//        }
			//        else
			//        {
			//            AlarmsWork.presentAlarms &= ~ALARM_DRAGER_WATRAPNOCONNET;
			//        }
			//采样管故障||脱水器未连接
			//        if((AA_all_data.AA_state.module_err_message[1] & 0x20) ||    //采样管堵塞
			//           (AA_all_data.AA_state.module_err_message[2] & 0x01) ||    //输入气体漏气
			//           (AA_all_data.AA_state.module_err_message[2] & 0x02) ||    //输入气体堵塞
			//           (AA_all_data.AA_state.module_err_message[2] & 0x04) ||    //输出气体堵塞
			//           (AA_all_data.AA_state.module_err_message[2] & 0x08) ||    //堵塞
			//           (AA_all_data.AA_state.module_err_message[2] & 0x10) ||    //检测流量
			//           (AA_all_data.AA_state.module_err_message[2] & 0x20) ||    //采样管未连接
			//           (AA_all_data.AA_state.module_err_message[2] & 0x40))      //采样管未连接或输入气体漏气
			if( (  (AA_all_data.AA_state.module_err_message[1] & 0x20)
				||(AA_all_data.AA_state.module_err_message[2] & 0x7F)
				||(!(AA_all_data.AA_state.module_err_message[1] & 0x1C)//排除脱水器
				&&(AA_all_data.AA_state.module_err_message[3] & 0x03)))
				||(AA_all_data.AA_state.module_err_message[1] & 0x10)	\
				||(AA_all_data.AA_state.module_err_message[3] & 0x20))//脱水器未连接
			{
				if(!(AlarmsWork.presentAlarms & ALARM_DRAGER_WATRAP_OR_LINE_EXCP))
				{ 
					AlarmsWork.newAlarms |= ALARM_DRAGER_WATRAP_OR_LINE_EXCP;
				}
				Err_Num++;
			}
			else
			{
				AlarmsWork.presentAlarms &= ~ALARM_DRAGER_WATRAP_OR_LINE_EXCP;
			}
			if(AA_all_data.AA_state.module_err_message[1] & 0x40)//电源输入错误
			{
				if(!(AlarmsWork.presentAlarms & ALARM_DRAGER_SUPVOLBAD))
				{ 
					AlarmsWork.newAlarms |= ALARM_DRAGER_SUPVOLBAD;
				}
				Err_Num++;
			}
			else
			{
				AlarmsWork.presentAlarms &= ~ALARM_DRAGER_SUPVOLBAD;
			}
			if((AA_all_data.AA_state.module_err_message[4] & 0x20)||  //CO2传感器硬件错误
				(AA_all_data.AA_state.module_err_message[4] & 0x80)|| //CO2传感器不可用
				(AA_all_data.AA_state.module_err_message[5] & 0x02)) //CO2传感器连接中断
			{
				if(!(AlarmsWork.presentAlarms & ALARM_DRAGER_CO2FAIL))
				{ 
					AlarmsWork.newAlarms |= ALARM_DRAGER_CO2FAIL;
				}
				Err_Num++;
			}
			else
			{
				AlarmsWork.presentAlarms &= ~ALARM_DRAGER_CO2FAIL;
			}
			if((AA_all_data.AA_state.module_err_message[4] & 0x40)||  //O2传感器硬件错误
				(AA_all_data.AA_state.module_err_message[5] & 0x01)|| //O2传感器不可用
				(AA_all_data.AA_state.module_err_message[5] & 0x04)) //O2传感器连接中断
			{
				if(!(AlarmsWork.presentAlarms & ALARM_DRAGER_O2FAIL))
				{ 
					AlarmsWork.newAlarms |= ALARM_DRAGER_O2FAIL;
				}
				Err_Num++;
			}
			else
			{
				AlarmsWork.presentAlarms &= ~ALARM_DRAGER_O2FAIL;
			}
			//模块错误
			//        if((AA_all_data.AA_state.module_err_message[1] & 0x80) || //气动硬件错误
			//           (AA_all_data.AA_state.module_err_message[3] & 0x04) || //校零失败
			//           (AA_all_data.AA_state.module_err_message[3] & 0x08) || //气动组件不可用
			//           (AA_all_data.AA_state.module_err_message[3] & 0x10) || //气动组件连接中断
			//           (AA_all_data.AA_state.module_err_message[5] & 0x08) || //风扇停转
			//           (AA_all_data.AA_state.module_err_message[1] & 0x01) || //组件失败，有组件不工作
			//           (AA_all_data.AA_state.module_err_message[0] & 0x02) || //出厂校准缺失
			//           (AA_all_data.AA_state.module_err_message[0] & 0x04) || //软件版本错误
			//           (AA_all_data.AA_state.module_err_message[0] & 0x08))   //传感器过热
			if((AA_all_data.AA_state.module_err_message[0] & 0x0E) ||
				(AA_all_data.AA_state.module_err_message[1] & 0x81) ||
				(AA_all_data.AA_state.module_err_message[3] & 0x1C) ||
				(AA_all_data.AA_state.module_err_message[5] & 0x08))
			{
				if(!(AlarmsWork.presentAlarms & ALARM_DRAGER_MODULEFAIL))
				{ 
					AlarmsWork.newAlarms |= ALARM_DRAGER_MODULEFAIL;
				}
				Err_Num++;
			}
			else
			{
				AlarmsWork.presentAlarms &= ~ALARM_DRAGER_MODULEFAIL;
			}
		}
		AA_all_data.AA_state.module_err_number = Err_Num;
	}
}

