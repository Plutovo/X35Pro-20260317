//
#include "GUI.H"
#include "GUIType.h"
#include "DIALOG.H"
#include "WM.h"
#include "LCDConf.H"
#include "button.h"
#include "progbar.h"
#include "MULTIPAGE.h"
#include "MainTask.h"
#include "Key_Handle.h"
#include "Static_Menu_Creat.h"
#include "Communication_Handle.h"
#include "Alarm_handle.h"
#include "Skinning_Lib.H"
#include "AddFunETCO2.H" //  20130705
#include "CO2_IRMA.h"
#include "C002_CO2.h"
#include "WIDGET_Patient.h"
#include <math.h> //  [11/8/2022 lyy add]Demo
#include "PowerComm.h"
#include "BubbleCtrl.h"
#include "PawGauge.h"

// #include "MedibusComm.h"
//#include "..\..\board\include\iolpc1788 and 2478.h"
////////////////////////////////////////////////////////////////////////

#if (LOGO_FLAG == ENDURE_LOGO)
const int IE_RatioTable[] = 
{40,35,30,25,20,15,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120};
#else
const int IE_RatioTable[] = 
{40,35,30,25,20,15,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100};
#endif
const int IE_RatioTableMoniter[] = 
{90,85,80,75,70,65,60,55,50,45,40,35,30,25,20,15,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,
105,110,115,120,125,130,135,140,145,150,155,160,165,170,175,180,185,190,195,};

////////////////////////////////////////////////////////////////////////

extern unsigned char CheckChargeStatus(void);
extern int DemoProgBar(void);
extern GUI_CONST_STORAGE GUI_BITMAP bmSASVAT_LOGO_1   ;
extern GUI_CONST_STORAGE GUI_BITMAP bmSASVAT_LOGO_2   ;
extern GUI_CONST_STORAGE GUI_BITMAP bmSASVAT_LOGO_3   ;
extern GUI_CONST_STORAGE GUI_BITMAP bmSASVAT_LOGO_4   ;
extern GUI_CONST_STORAGE GUI_BITMAP bmADVANCED_6000_1;
extern GUI_CONST_STORAGE GUI_BITMAP bmADVANCED_6000_2;
extern GUI_CONST_STORAGE GUI_BITMAP bmADVANCED_6000_3;
extern GUI_CONST_STORAGE GUI_BITMAP bmADVANCED_6000_4;
extern void RealtimeParaUpdate(HOME_MENU_HWIN * hWin);
extern int MULTIPAGE_GetSelection(MULTIPAGE_Handle hObj);
extern unsigned char EtCO2ModuleOff;
extern int OS_TimeMS;
extern PARA_SETTING_STATE ParaSettingState;

void InitSettingParameter(BREATH_SET_PARA * BeathSettingPara);
void InitMonitorParameter(MONITOR_PARA * Parameter);
void InitRealtimeParameter(REALTIME_PARA * Parameter);//  for test 20130328
void InitAlarmSettingParameter(ALARM_SET_PARA * Parameter, Patient_Info_t *Patient, BREATH_SET_PARA * BeathSettingPara);
void InitSoftwareVersionParameter(void);//  20130328
void InitAlarmsWork(ALARM_WORK * work);
void InitMenuHwin(MENU_HWIN *hWin);
void initco2spo2switch(void); //  20130412
void InitAlarmRecordList(ALARM_RECORD_LIST * ListHead);
void InitOperationTime(OPERATION_TIME * RunTime);
void InitBarometer();//djq add 20140305 初始化大气压力传感器
void Read_ConfigMode(void);
void irma_getpara(void);
void InitAlarmSaveData(ALARM_SET_PARA * Parameter);

void CO2_UnitConvert(int Unit);
//
extern RTC_TIMER RTCReadTime();
extern void CheckAlarm(char insp_flag);
extern ALARM_INFO GetHighestAlarm(Int64U Alarms,Int64U Alarms1);
extern void SelfTestStart();
extern void SelfTestEnd();
//
void  WriteOperationTime(OPERATION_TIME * RunTime);
extern void BeepAloudControl(unsigned char status);
void TaskSchedulerManager(GUI_TIMER_TIME t);
extern Int8U CheckChargeFinish(void);
extern Int8U GetBatteryVolume(Int16U BatteryVoltage);
extern void CancelSuctionFunction(void);
extern char RTEEPROMread(Int16U addr);
extern int RTEEPROMwrite(Int16U addr, Int8U databyte);
extern float Change_Unit(unsigned char unit_dest,unsigned char unit_src,float para);
/*const int Pressure[50]={10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,46,48,50,46,
42,37,32,27,22,20,18,15,12,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10};
const int Flow[50]=  {40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,-100,
-90,-70,-60,-50,-40,-30,-20,-10,-5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const int Volume[50] = {0,50,100,150,200,250,300,350,400,450,500,550,600,650,700,750,800,850,900,
950,850,800,750,650,550,450,350,250,200,150,100,50,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};*/
//
extern unsigned char AskMenu_Flag;
unsigned char Picture_Load_Complete;
extern unsigned char butter_press_count;
extern int TouchScreenCalibrate;
int ETCO2Unit = 1;
int preETCO2Unit; //  20130607
unsigned int delay_time[5];   //0: 氧浓度校验延迟1.5min后给BDU发送命令
extern unsigned int DeviceSerial;
VENT_TIME VentTime; //  20130616

extern CO2_S CO2_Disp;//djq add 20170526
/***************************************************************************************************
*  main loop
*/
ALARM_WORK AlarmsWork;
ALARM_INFO ShowedAlarmInfo;
extern ALARM_INFO ShowedAlarmInfoSecond;

BREATH_SET_PARA BeathSettingPara;
MONITOR_PARA MonitorPara;
REALTIME_PARA RealtimePara;
//MESSAGE_QUEUE MsgQueue;
ALARM_SET_PARA AlarmSettingPara;
VERSION_PARA SystemVersion = {1,0,0,0},GUIVersion = {1,0,0,0},BDUVersion = {1,0,0,0},PMUVersion = {1,0,0,0}; //  20130328   //2016.5.30
WM_HWIN Focus_hWin = 0;
MENU_HWIN Menu_hWin;
RTC_TIMER RTCTime;

int CalibrateBatteryFlag = 0;//*///endof   20130304

extern KEY_MSG KeyMsg;
int Wm_Exec_Flag=TRUE;
int MenuType;
int PlanMenuType = 0;
int Task;
int StandbyState = 0xFF;
int LastStandbyState = 0x0FF;
#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C)
int LedUpdateSendMesg = FALSE;
int IndicateLedState = 0;
#endif
int InspFlagForCurve = 0;
int InspFlagForAlarm = 0;
int MenuOperateStatus = 0;
int CalibrationType = 0;
int CalibrateFinishedFlag = 0;
int SuctionTimer = 0;
//int AcgoModeFlag = 0; //  20130415
//int ManualModeFlag = 0;//sun add 20150117
int BreathMode_BeforeACGO = VCV_MODE;
int BreathMode_BeforeManual = VCV_MODE;
int BreathMode_AfterACGO  = 0; //用于保存切到ACGO或手动后的模式
int BreathMode_AfterMANUAL  = 0;
int BreathMode_BeforeConfirm = VCV_MODE;//接受确认之前的模式
Int16U BatteryAdValue = 0;

//sun add 20151102
unsigned char pHaseinMoudleCheck = 0;//检测pHasein是否连接标志
unsigned char LastpHaseinMoudleCheck = 0;//记录上一次检测pHasein是否连接的标志

Int8U   ETCO2_FLAG = 0; //1:波特率38400模块     0:波特率9600模块
AA_PARAMETERS AA_Para = {0,0,0,0,0,0,0,0,0,0,0,0};
extern IM_ALL_DATA      * pAll_Data;
extern C002_CO2_PARAMETERS CO2_Parameters;
GUI_COLOR N2OColor;
float MAC_Base[] =
{
	//          0,        0.075,         0.17,        0.115,        0.205           //cpp del 20130710
	0,         0.75,          1.7,         1.15,         2.05,           6
		//            0,          7.5,           17,         11.5,         20.5,          60 //cpp del 20130712
};

//电池报警及电量显示相关参数
BATTERY_AD_PARA  BatteryPara;

//软件授权码
const char HexTable[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

AA_MODULE_FLAG_REG AA_Module_Flag = {0,0,1,0,0,50,No_Connect}; //ylk add 20181128
extern Int16U  USART2_TXD_RIGSTER;
extern Int8U   USART2_TXD_Busy_FLAG;
extern Int16U  Inquire_TXD_Handler(Int8U UsartPort);
extern unsigned char N2O_Switch;
unsigned char Paramagnetic_O2_Flag = 0;//顺磁氧标志 0:氧电池 1：顺磁氧
//选配
unsigned char Mode_Chose_Flag = 0; //0: off  1:on
unsigned char VT_Min_Flag = 0;     //婴儿模式 1:on  0:off
unsigned char PRVCMode_Chose_Flag = 0; //0: off  1:on
unsigned char VS_Mode_Flag = 0;
unsigned char APRV_Mode_Flag = 0;
unsigned char FiO2_Config_On = 0;  //氧浓度传感器  1：on  0：off //ylk add 20180314 FiO2选配
unsigned char DriverGas_Chose_Flag = 0; //0: off  1:on  djq add 20170609
unsigned char FlowPause_Config_On = 0;
unsigned char LungRec_Config_On = 0;
unsigned char SpO2_Config_On = 0;  //0:关闭   1：开启
unsigned char Masimo_SpO2_Config_On = 0; //0:关闭   1：开启
//麻气选配
unsigned char EtCO2_Config_On = 0; //0:关闭   1：开启
unsigned char Masimo_EtCO2_Config_On = 0; //0:关闭   1：开启
unsigned char MasimoISA_EtCO2_Config_On = 0; //0:关闭   1：ISA	2:ISA+O2
unsigned char EtCO2_KM7002_Config_On = 0; //0:关闭   1：开启 金嘉信CO2
unsigned char Drager_EtCO2_Config_On = 0; //0:关闭   1：开启麻醉气体    2：开启麻醉气体+顺磁氧      Drager模块是不需要校验的，带顺磁氧的模块，不从BDU接收顺磁氧数据。


//芯片ID
extern char DeviceSerial_String[9];
extern char DeviceSerial_Table[8];

extern ALARM_RECORD_LIST AlarmRecordList;
OPERATION_TIME OperationTime;
Int8U SaveOperationTimeFlag = FALSE;
extern unsigned short int  LedUpdateRegister;
extern GUI_CONST_STORAGE GUI_BITMAP bmatese;//bmsafelogo;bmsiriusmed
extern GUI_CONST_STORAGE GUI_BITMAP bmvetland;
extern GUI_CONST_STORAGE GUI_BITMAP bmsiriusmed;   //hanbinsharp 20131225  bmAnesthesia
extern GUI_CONST_STORAGE GUI_BITMAP bmAnesthesia;   //hanbinsharp 20131225
extern GUI_CONST_STORAGE GUI_BITMAP bmhsp1;
extern GUI_CONST_STORAGE GUI_BITMAP bmhsp2;
extern GUI_CONST_STORAGE GUI_BITMAP bmAnesthesia_System;

extern GUI_CONST_STORAGE GUI_BITMAP bmMS;
extern GUI_CONST_STORAGE GUI_BITMAP bmms_westfalia;
extern GUI_CONST_STORAGE GUI_BITMAP bmMajaX45;

extern GUI_CONST_STORAGE GUI_BITMAP bmX6;
extern GUI_CONST_STORAGE GUI_BITMAP bmX45;
extern GUI_CONST_STORAGE GUI_BITMAP bmX45A;
extern GUI_CONST_STORAGE GUI_BITMAP bmNew_Siriusmed;
extern GUI_CONST_STORAGE GUI_BITMAP bmSiriusmed_Care;

extern GUI_CONST_STORAGE GUI_BITMAP bmX6_GA;
extern GUI_CONST_STORAGE GUI_BITMAP bmNew_Siriusmed_GA;
extern GUI_CONST_STORAGE GUI_BITMAP bmSiriusmed_Care_GA;

extern GUI_CONST_STORAGE GUI_BITMAP bm490x140;
extern GUI_CONST_STORAGE GUI_BITMAP bmx215y224;
extern GUI_CONST_STORAGE GUI_BITMAP bmx219y170;
extern GUI_CONST_STORAGE GUI_BITMAP bmx231y166;
extern GUI_CONST_STORAGE GUI_BITMAP bmx546y174;

extern GUI_CONST_STORAGE GUI_BITMAP bmx180y224;
extern GUI_CONST_STORAGE GUI_BITMAP bmx182y170;
extern GUI_CONST_STORAGE GUI_BITMAP bmx194y166;
extern GUI_CONST_STORAGE GUI_BITMAP bmx203y245;
extern GUI_CONST_STORAGE GUI_BITMAP bmx191y249;
extern GUI_CONST_STORAGE GUI_BITMAP bmx416y208;

extern GUI_CONST_STORAGE GUI_BITMAP bmx477y185;
extern GUI_CONST_STORAGE GUI_BITMAP bmx116y181;
extern GUI_CONST_STORAGE GUI_BITMAP bmx127y233;
extern GUI_CONST_STORAGE GUI_BITMAP bmx128y177;
extern GUI_CONST_STORAGE GUI_BITMAP bmx442y151;

extern GUI_CONST_STORAGE GUI_BITMAP bmsiriusmedica;
extern GUI_CONST_STORAGE GUI_BITMAP bmphoto;
extern GUI_CONST_STORAGE GUI_BITMAP bmlogo_2;
extern GUI_CONST_STORAGE GUI_BITMAP bmlogo_1;
extern GUI_CONST_STORAGE GUI_BITMAP bmx45;

//new msw
extern GUI_CONST_STORAGE GUI_BITMAP bmNew_Siriusmed_Blue;
extern GUI_CONST_STORAGE GUI_BITMAP bmNEW_X45;
extern GUI_CONST_STORAGE GUI_BITMAP bmNew_MAJA;


//Penlon
extern GUI_CONST_STORAGE GUI_BITMAP bmpenlon_logo1;
extern GUI_CONST_STORAGE GUI_BITMAP bmpenlon_logo2;
extern GUI_CONST_STORAGE GUI_BITMAP bmpenlon_advance;
extern GUI_CONST_STORAGE GUI_BITMAP bmpenlon_320;
extern GUI_CONST_STORAGE GUI_BITMAP bmpenlon_prima;

extern GUI_CONST_STORAGE GUI_BITMAP bmAndromeda;
extern GUI_CONST_STORAGE GUI_BITMAP bmDC5000;


extern GUI_CONST_STORAGE GUI_BITMAP bmADVANCE_Inc_1;

extern GUI_CONST_STORAGE GUI_BITMAP bmw_ex32;


//  [11/15/2022 lyy add ProMed新的logo]
//extern GUI_CONST_STORAGE GUI_BITMAP bmPRO;
//extern GUI_CONST_STORAGE GUI_BITMAP bmmed;
extern GUI_CONST_STORAGE GUI_BITMAP bmPROMED_PRO;
extern GUI_CONST_STORAGE GUI_BITMAP bmPROMED_MED;
extern GUI_CONST_STORAGE GUI_BITMAP bmPROMED_Logo;


extern GUI_CONST_STORAGE GUI_BITMAP bmNESCO_LOGO;//  [7/14/2022 yaoyao add]定制 PT. Djuniar &Djuniar- Nesco Group 印度尼西亚
extern GUI_CONST_STORAGE GUI_BITMAP bmXN4N5_Icon;//  [7/14/2022 yaoyao add]定制 PT. Djuniar &Djuniar- Nesco Group 印度尼西亚

unsigned char ChargeFinishFlag = 0;//充电是否完成 0：未完成 1充电完成 2没有电池

//
extern unsigned char SpO2ModuleOff;
extern unsigned char SpO2ModuleExist;
//extern unsigned char AA_Module_Flag.EtCO2ModuleExist;
extern unsigned char EtCO2ModuleOff;
extern unsigned char O2ModuleOff; //djq add  20131226
//djq add 20140305 大气压力
extern int Barameter_Pressure;         //设置信息界面，大气压力值   0~999mmHg
extern int Barameter_Stand_Pressure;   //校验地大气压力值   0~999mmHg
extern int Barameter_Switch;           //0 ：大气压传感器开，显示为BDU上传的值   1：关，显示为设置值。
extern WM_HWIN MuteFlag; //sun add 20140519
extern unsigned char Start_Standby_Flag;
//extern unsigned char ACGOModeFlag;
//extern unsigned char MANUALModeFlag;
extern unsigned char alarm_battery_cali;
//extern int PowerState;
extern unsigned char AskTimer;
extern Int8U CHECK_DEBUG;//开始检测标志
extern CHINACO2_InitTypedef ChinaCO2_Init;
//extern JM1_SPO2_PARAMETERS SpO2_Parameters;
extern unsigned char DirverGasFlag;//驱动气体类型标志
extern unsigned char AlarmAirSourceSwitch;//0：off  1：On
extern unsigned char SpO2_Choose;//1：国产   2：Masimo
extern unsigned char SPO2_RecvFinishFlag;
void GasModule_InitConfig(void);
INFO_HWIN iWin;
extern Int8U ModeChange_Flag;
extern void AlarmLimitParaChange(ALARM_SET_PARA *AlarmPara,unsigned char *change_flag);
extern ALARM_SAVEVENT_PARA Alarm_SaveVent_para;

extern BREATH_SET_PARA Breath_Setting_Tmp;//ylk add 20181009
unsigned char AcgoPromptFlag = 0;
unsigned char AcgoTimeFlag = 0;

unsigned int  Vt_HighLimit = 0;//潮气量的上限  开机、系统初始化、病人初始化的时候赋值
unsigned int  Vt_LowLimit = 0; //潮气量的下限  开机、系统出还刷、病人初始化和修改潮气量选配的时候赋值
extern unsigned int FlowHighLimit;
extern unsigned int FlowLowLimit;
void SetVtValue_Limit(Patient_Info_t *Patient, int mode);
void InitPatientInfo(Patient_Info_t *Patient);//tcc add 20220525
extern unsigned char WaveFormsId2;

extern unsigned char oxyChooseFlag;

//////////////////////////////////////////////////////////////////////////
#ifdef DEMO  //  [11/8/2022 lyy add]
unsigned char Demo_Switch = 0;//Demo模式标志 0：关闭 1：开启
GUI_COLOR Curve_Color = GUI_YELLOW;
int Demo_SetR = 20;
int Demo_SetC = 20;
char Demo_Change_Cycle = 0;
char Demo_Change_Cycle_A = 0;
char Demo_InitFspn = 0;
char Demo_InEx_Flg;
int Demo_Time;
int Demo_Cycle_Insp;
int Demo_Cycle_Exsp;
int Demo_Cycle_Tp;
int Demo_Cycle;
float Demo_Paw;
float Demo_Peak;
float Demo_Plat;
float Demo_Flow;
char Demo_InitPsv = 0;
float Demo_VtValue = 500;
float Demo_TiValue = 15;
float Demo_RateValue = 20;
float Demo_TpValue = 50;
float Demo_PeepValue = 3;
float Demo_PinspValue = 15;
float Demo_Et0, Demo_Et1,Demo_Et2,Demo_RC;
float Demo_Time_Exsp,Demo_Time_Timer;
float Demo_V;
void Demo_InitSettingParameter(void);
void TaskDemoGetCycle(void);
void DemoParaUpdate(void);
void DemoParaInit(void);
void Demo_Get_Time(void);
void Demo_Update_Para(void);
void Demo_Get_Cycle(void);
void Demo_Update_Pmean(void) ;
#endif

extern unsigned char ShutDownAskMenu_Flag;
extern unsigned char Receive_ShutDown_Flag;
extern unsigned char SendShutDownState;


/***************************************************************************************************
* @brief
* @para
* @retval
*/


void EEPROM_TEST(char flag,unsigned int addr,unsigned int num)
{
	char st[100]={0};
	volatile unsigned int KeyValue=0;
	static int TextNum = 0,xNum = -1;//ceshi
	volatile unsigned char ADnum = 0,SaveNum;

	KeyValue = addr;
	SaveNum = num;
	if(flag)
	{
		RTEEPROMwrite(KeyValue,SaveNum);
	}
	ADnum = RTEEPROMread(KeyValue);
	if(ADnum != SaveNum)
	{
		sprintf(st,"%#x-%#x",KeyValue,ADnum);
		if(!(TextNum%20))
		{
			xNum++;
			TextNum = 0;
		}
		GUI_DispStringAt(st,100*xNum,40+20*TextNum++);
	}
}
// int color = 0x420606;

void MainTask(void)
{
	static unsigned char ModeRefreshFlag = 0;
	static int LungRec_Wave_Scan = 0;

	static int Info_cnt = 0;
	int KeyValue=0;
	int TextNum = 0;//ceshi
	int ADnum = 0;
	WM_HWIN hWin;
	GUI_PID_STATE TouchState;
	WM_MESSAGE Msg;
	static unsigned char PhaseIn_OldZero_State = 0;
	Picture_Load_Complete=0;
	InitAlarmsWork(&AlarmsWork);
	GUI_Init();

	GUI_Font_Init();//ylk add 20190620

	GUI_UC_SetEncodeUTF8();

	switch(LOGO_FLAG)
	{
	case SIRIUSMED_LOGO:
		GUI_SetBkColor(0xAC5B00); //hanbinsharp  mod 20131225
		GUI_Clear();
		GUI_DrawBitmap(&bmsiriusmed,157,187);      //hanbinsharp  mod 20131225
		GUI_SetColor(GUI_WHITE);
		GUI_SetFont(&GUI_Font_16);
		GUI_DispStringAt(Sys_Lib_GetDispStr(DP_SOFTWAREVERSION),334,330 );
		break;
	case NEW_SIRIUSMED_LOGO:
	case ENDURE_LOGO:
#ifdef LANGUAGE_CHINESE_FLAG

		GUI_SetBkColor(0xAC5B00);
		GUI_Clear();
		GUI_DrawBitmap(&bmphoto,10,295);
		GUI_DrawBitmap(&bmlogo_2,595,190);
		GUI_DrawBitmap(&bmlogo_1,311,190);
		GUI_DrawBitmap(&bmx45,92,390);
		GUI_DrawBitmap(&bmsiriusmedica,31,430);

#else
		GUI_SetBkColor(0xAC5A00);
		GUI_Clear();
#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C)
#if (MACHINE_TYPE == X45A)
		GUI_DrawBitmap(&bmX45A,600,29);
#else
		GUI_DrawBitmap(&bmX45,658,29);
#endif
#else
		GUI_DrawBitmap(&bmX6,700,29);
#endif
		GUI_DrawBitmap(&bmNew_Siriusmed,246,179);
		GUI_DrawBitmap(&bmSiriusmed_Care,231,223);
#endif
		break;
	case NEUTRAL_LOGO:  //sun add 20150812
		GUI_SetBkColor(0x746253);
		GUI_Clear();
		GUI_SetColor(0x941F00);
#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C)
		GUI_DrawHLine(153,167,586);
		GUI_DrawHLine(154,167,586);
		GUI_DrawHLine(183,162,186);
		GUI_DrawHLine(184,162,186);
		GUI_DrawHLine(213,157,176);
		GUI_DrawHLine(214,157,176);
		GUI_DrawHLine(243,152,177);
		GUI_DrawHLine(244,152,177);
		GUI_DrawHLine(273,146,541);
		GUI_DrawHLine(274,146,543);
		GUI_DrawBitmap(&bmx416y208,515,153);
		GUI_DrawBitmap(&bmx180y224,205,235);
		GUI_DrawBitmap(&bmx546y174,571,187);
		GUI_DrawBitmap(&bmx191y249,186,183);
		GUI_DrawBitmap(&bmx203y245,198,180);
#else
		GUI_DrawHLine(151,176,594);
		GUI_DrawHLine(152,176,594);
		GUI_DrawHLine(153,176,594);
		GUI_DrawHLine(181,170,194);
		GUI_DrawHLine(182,170,194);
		GUI_DrawHLine(183,170,194);
		GUI_DrawHLine(211,165,184);
		GUI_DrawHLine(212,165,184);
		GUI_DrawHLine(213,165,184);
		GUI_DrawHLine(241,160,185);
		GUI_DrawHLine(242,160,185);
		GUI_DrawHLine(243,160,185);
		GUI_DrawHLine(271,154,546);
		GUI_DrawHLine(272,154,548);
		GUI_DrawHLine(273,154,548);
		GUI_DrawBitmap(&bmx442y151,522,152);
		GUI_DrawBitmap(&bmx477y185,578,186);
		GUI_DrawBitmap(&bmx127y233,207,235);
		GUI_DrawBitmap(&bmx128y177,208,178);
		GUI_DrawBitmap(&bmx116y181,196,182);
#endif
		break;
	case HOSPITECH_LOGO:
		GUI_SetBkColor(0xFFFFFF);  //sun add 20140723
		GUI_Clear();
		GUI_SetColor(0xCDCECD);
		GUI_FillRect(0, 230, 800, 268);
		GUI_DrawBitmap(&bmhsp1,108,96);  //sun add 20140723
		GUI_DrawBitmap(&bmhsp2,306,159);  //sun add 20140723
		GUI_DrawBitmap(&bmAnesthesia_System,238,235);
// 		GUI_SetColor(GUI_BLACK);
// 		GUI_SetFont(&GUI_Font_16);
// 		GUI_DispStringAt(Sys_Lib_GetDispStr(DP_SOFTWAREVERSION),334,330 );
		break;
	case MS_WESTFALIA_LOGO:  //sun add 20160301
		GUI_SetBkColor(0xF7F7F7);
		GUI_Clear();
		GUI_SetColor(0xBDBDBD);
		GUI_FillRect(0, 387, 800, 424);//灰条 h:37
		GUI_DrawBitmap(&bmMS,271,131);
		GUI_DrawBitmap(&bmms_westfalia,272,245);
		GUI_DrawBitmap(&bmMajaX45,486,354);
		break;
	case SASVAT_LOGO:  //sun add 20160301
		GUI_SetBkColor(0xf7f7f7);
		GUI_Clear();
		GUI_DrawBitmap(&bmSASVAT_LOGO_1,330,73);
		GUI_DrawBitmap(&bmSASVAT_LOGO_2,237,225);
		GUI_DrawBitmap(&bmSASVAT_LOGO_3,237,308);
		GUI_DrawBitmap(&bmSASVAT_LOGO_4,323,330);
		break;
	case ADVANCED_LOGO:  //sun add 20160301
		GUI_SetBkColor(0xf7f7f7);
		GUI_Clear();
		GUI_DrawBitmap(&bmADVANCED_6000_1,446,380);
		GUI_DrawBitmap(&bmADVANCED_6000_2,76,124);
		GUI_DrawBitmap(&bmADVANCED_6000_3,237,156);
		GUI_DrawBitmap(&bmADVANCED_6000_4,711,153);
		break;
	case MSW_LOGO://add zkq 20161104

		/*
		GUI_SetBkColor(0xf7f7f7);
		GUI_Clear();
#ifdef MSW_FLAG //  [7/5/2022 yaoyao add]
		GUI_DrawBitmap(&bmMSW_LOGO,41,28);
#endif
		GUI_DrawBitmap(&bmMSW_x45,457,260);
		GUI_DrawBitmap(&bmMSW_MAJA,275,203);
		GUI_SetColor(0x929497);
		GUI_FillRect(0, 414, 800, 600);//灰条 h:37*/

		GUI_SetBkColor(GUI_WHITE);
		GUI_Clear();
		GUI_DrawBitmap(&bmNew_Siriusmed_Blue,41,35);
		GUI_DrawBitmap(&bmNEW_X45,475,280);
		GUI_DrawBitmap(&bmNew_MAJA,275,203);
		GUI_SetColor(0x929497);
		GUI_FillRect(0, 414, 800, 600);

		break;
	case GALILEA_LOGO:
		GUI_SetBkColor(0xF7F7F7);
		GUI_Clear();
		GUI_DrawBitmap(&bmX6_GA,362,372);
		GUI_DrawBitmap(&bmNew_Siriusmed_GA,246,161);
		GUI_DrawBitmap(&bmSiriusmed_Care_GA,231,207);
		break;
	case PENLON_LOGO://ylk add 20180709
		GUI_SetBkColor(0x000000);
		GUI_Clear();

		GUI_SetColor(0x222222);
		GUI_FillRect(0,397,799,434);

		GUI_DrawBitmap(&bmpenlon_logo1,243,149);
		GUI_DrawBitmap(&bmpenlon_logo2,542,168);
		GUI_DrawBitmap(&bmpenlon_advance,603,383);
		GUI_DrawBitmap(&bmpenlon_320,529,387);
		GUI_DrawBitmap(&bmpenlon_prima,396,387);

		break;
	case ANDROMEDA://ylk add 20181113
		GUI_SetBkColor(0xf7f7f7);
		GUI_Clear();
		GUI_DrawBitmap(&bmAndromeda,198,100);
		GUI_DrawBitmap(&bmDC5000,540,350);
		break;
	case ADVANCE_INC_LOGO://jm 20210604 add
		GUI_SetBkColor(0xf7f7f7);
		GUI_Clear();
		GUI_DrawBitmap(&bmADVANCE_Inc_1,192,150);
		break;
	case PROMED_LOGO:
	//  [11/15/2022 lyy add ProMed新的logo]
		GUI_SetBkColor(GUI_WHITE/*GUI_BLACK*/);
		GUI_Clear();
// 		GUI_DrawBitmap(&bmPRO,223, 150);
// 		GUI_DrawBitmap(&bmmed,390, 150);
		GUI_DrawBitmap(&bmPROMED_Logo, 280,150);//  [11/15/2022 lyy add]
		GUI_DrawBitmap(&bmPROMED_PRO, 220,300);//  [11/15/2022 lyy add]
		GUI_DrawBitmap(&bmPROMED_MED, 390,300);//  [11/15/2022 lyy add]
		break;
	case RN5NO_LOGO://  [8/14/2023 gyh add]
		GUI_SetBkColor(0x9C5918);
		GUI_Clear();
		GUI_DrawBitmap(&bmNESCO_LOGO, 230, 180);
		GUI_DrawBitmap(&bmXN4N5_Icon, 540, 50);
		break;
	case WEX35_LOGO://  [8/14/2023 gyh add]
		GUI_SetBkColor(0xAC5900);
		GUI_Clear();
		GUI_DrawBitmap(&bmw_ex32, 202, 184);
		break;
	}

	FRAM_Addr_WriteInfo();

#ifndef __ICCARM__
	InitLanguage();
#endif

	RTCTime = RTCReadTime();
	ETCO2Unit  = ReadEtCO2Unit(); //  20130409
	preETCO2Unit = ETCO2Unit; //  20130607

	CO2_UnitConvert(ETCO2Unit);

	InitSoftwareVersionParameter(); //  20130328
	Battery_Data_Init();
	//*/
	//StartGraphShow();
	InitAlarmRecordList(&AlarmRecordList);

	SelfTestStart();
    //
    Power_SendMachineModel(POWER_SWITCH_MODE);
    SendOperationMessage(SEND_MACHINE_MODE_FLAG,USART3);
	
	Power_SendBatManageType(POWER_BAT_MANAGE_TYPE);
	
    //
	DemoProgBar(); //cpp del for test 20130326

	Picture_Load_Complete = 1;
	SelfTestEnd();
	//#ifdef SELFTEST    //djq add 20170109
	LCD_SetColor(0xFFFFFF);
	//Bevel(0, 50, 552, 390,8);
	// 	Bevel(0, 70, 637, 390,8);  //sun add 20140516
	//#endif
	TouchAdValue_Read(1);//读取校验数据
	GUI_TOUCH_SetDefaultCalibration();
	PlanMenuType = STANDBY_MENU;
	Task = MEAU_UPDATE;
	Read_ConfigMode();    //选配项目开关
#ifdef LANGUAGE_CHINESE_FLAG
	N2OColor = GUI_GRAY;
#else
	N2OColor = 0xD99C19;//GUI_BLUE
#endif

	SendOperationMessage(ASK_VERSIONFOR_BDU_COM,USART0);//主动询问BDU版本号码
#ifdef DEMO
	Demo_InitSettingParameter();//  [11/8/2022 lyy add初始化Demo设置参数]
#endif
	InitPatientInfo(Patient_GetInfo());//tcc add 20220525

	InitSettingParameter(&BeathSettingPara);
	InitMonitorParameter(&MonitorPara);
	InitAlarmSettingParameter(&AlarmSettingPara, Patient_GetInfo(), &BeathSettingPara);

	InitMenuHwin(&Menu_hWin);
	/////////

	SetTimerStartFlag(1);//定时器开始
	InitOperationTime(&OperationTime);

	//  alarm_str_init(LanguageType);
	InitAlarmInfoRegister();
	// 	SYS_str_init();

	InitBarometer(); //djq add 20140305
	Initcbdialog32string(Patient_GetInfo(), BeathSettingPara.BreathMode);  //sun add 20140521

	if((FRAM_Addr_SysInitFlag() == error))
	{
		//SettingDefaultAlarmPara();
	}

	InitMonitorRank_Auto(1);//ylk add 20181116

	InitAlarmSaveData(&AlarmSettingPara);
	initco2spo2switch();//  20130412
#ifdef __MEDIBUS
	Medibus_Init();//Medibus协议初始化
#endif
	HomePrompt_Init();

	WM_SetDesktopColor(WINDOW_BK_COLOR); // 0x1E1E1E
	Menu_hWin.TextWin = BubbleCtrl_CreateAsChild(0, 624, 130, 45, WM_HMEM_NULL, 0, WM_CF_SHOW);  //信息提示窗口

	//{
	//	WM_HMEM h = PawGauge_CreateAsChild(0, 400, 220, 220, WM_HMEM_NULL, 0, WM_CF_SHOW); 

	//	PawGauge_SetData(h, 20);
	//	PawGauge_SetUnit(h, Sys_Lib_GetDispStr(DP_CMH2O));
	//}
	//ModeTabBar_CreateAsChild(0, 600, 1024, 48, WM_HMEM_NULL, 0, WM_CF_SHOW); 

	//Menu_hWin.TextWin = IMAGETEXT_Create(25, 375, 310, 20,0,WM_CF_SHOW);  //信息提示窗口
	//IMAGETEXT_SetBkColor(Menu_hWin.TextWin, GUI_YAN_BLUE);
	//IMAGETEXT_SetTextColor(Menu_hWin.TextWin, GUI_BLACK);
	//WM_HideWindow(Menu_hWin.TextWin);

	// 	pAll_Data->slow.InspVal[0]=pAll_Data->slow.InspVal[1]=pAll_Data->slow.InspVal[2]=pAll_Data->slow.InspVal[3]=pAll_Data->slow.InspVal[4]=0xff;
	// 	pAll_Data->slow.ExpVal[0] =pAll_Data->slow.ExpVal[1] =pAll_Data->slow.ExpVal[2] =pAll_Data->slow.ExpVal[3] =pAll_Data->slow.ExpVal[4] =0xff;    //zy 2016.8.12
	GUI_ClearKeyBuffer();//zkq add 20170116 防止在进度条过程中按下按键，进度条结束后程序开始响应。
	BreathPara_Assign(&Breath_Setting_Tmp, &BeathSettingPara);
	AA_Module_Flag.InitFinishFlag = 1; //ylk add 201812105
//    SpO2_Para_Init();
	Enable_Interrupt(); //zy 2016.8.9 


	initTools(Patient_GetPType());

	while(1)
	{
		GUI_TIMER_TIME SystemClock = GUI_GetTime();
		TaskSchedulerManager(SystemClock); //根据GUI的时钟进行调度
#ifdef DEMO
		TaskDemoGetCycle();//  [11/8/2022 lyy add Demo的时钟]
#endif
		GasModule_InitConfig();

		HomePrompt_Display();

		
         if(ModeChange_Flag)//防止总是进入AlarmLimitParaChange函数判断
		{
			ModeChange_Flag = 0;
			AlarmLimitParaChange(&AlarmSettingPara,&Alarm_SaveVent_para.ALARM_ValUpdateFlag);

			Msg.hWin = WM_GetClientWindow(Menu_hWin.Swin);
			Msg.MsgId = WM_INIT_DIALOG;
			WM_SendMessage(Msg.hWin,&Msg);
		}

#ifdef __ICCARM__
#ifdef __MEDIBUS
		Medibus_FiniteStateMachine();
#endif
#endif

		if(Task&MEAU_UPDATE)
		{
			if(butter_press_count >=4)//开机按两次进入校验
			{
				AskMenu_Flag = 0;
			}
			Menu_hWin=StaticMenuSwitch(PlanMenuType,&Menu_hWin); //菜单切换
			Task &=~MEAU_UPDATE;
		}
#ifdef SPO2_SWITCH
		if((SpO2_Choose==2)&&(SpO2ModuleExist))//masimo选配&&存在
		{
			if(((SPO2_RecvFinishFlag&0x01) == 0x01))//数据接收一帧完成
			{
				SPO2_RecvFinishFlag = 0;
				GetD1D2SequenceData();
			}
			Exception2Code();
			BoardFailureCodes();
			DiagnoFailureCodes();
		}
#endif
		if(Task&PARAMETER_UPDATE) //490ms
		{
			if(StandbyState!=0xFF)
			{
				MonitorPara.IEValue = GetIERatio(VentTime);//cpp md 20130716
				/**********************ylk add 20181116************************/
				if(InitMonitorRank_Auto(0))//工作下 更新快捷监测菜单
				{
					if(WM_IsWindow(Menu_hWin.HomeWin.ParentWin))
					{
						WM_MESSAGE Msg;
						Msg.hWin = WM_GetClientWindow(Menu_hWin.HomeWin.ParentWin);
						Msg.MsgId = WM_INIT_DIALOG;
						WM_SendMessage(Msg.hWin,&Msg);
						//WM_SendMessageNoPara(Menu_hWin.RapidMonitorWin.ParentWin,WM_INIT_DIALOG);
					}
				}
				/**************************************************************/
				if(WM_IsWindow(Menu_hWin.HomeWin.ParentWin))
				{
					RealtimeParaUpdate(&Menu_hWin.HomeWin); //实时参数更新
				}
				if(MenuType==MONITOR_MENU)
				{
					if(MULTIPAGE_GetSelection(WM_GetDialogItem(Menu_hWin.ActivedWin,GUI_ID_MULTIPAGE0))==0)
					{
						MonitorDataUpdate(Menu_hWin.ActivedWin); //监测参数更新
					}
					else if(MULTIPAGE_GetSelection(WM_GetDialogItem(Menu_hWin.ActivedWin,GUI_ID_MULTIPAGE0))==1)
					{
						MonitorDataUpdate1(Menu_hWin.ActivedWin); //监测参数更新
					}
				}
			}
			else
			{
				if((MenuOperateStatus & CALIBRATION_PRECEED))
				{
					if(_CalibrationProgressShow(CalibrationType,Menu_hWin.ActivedWin,CalibrateFinishedFlag))
					{
						MenuOperateStatus &= ~CALIBRATION_PRECEED;
						CalibrationType = 0;
						CalibrateFinishedFlag = 0;
					}
				}
				// 				if(PhaseIn_OldZero_State != pAll_Data->slow.GenVal.zero_disab)
				if(PhaseIn_OldZero_State != AA_all_data.AA_state.AA_ModuleState.zero_state)
				{
					PhaseIn_OldZero_State = AA_all_data.AA_state.AA_ModuleState.zero_state;
					Menu_hWin.InfoWin.InfoChangeFlag |= PHASEIN_ZERO_CHANGE_FLAG;
				}
			}
			if(MenuType==SYSTEM_SETTING_MENU)//djq add 20140310
			{
				if(MULTIPAGE_GetSelection(WM_GetDialogItem(Menu_hWin.ActivedWin,GUI_ID_MULTIPAGE0))==0)
				{
					UpDate_SettingMenu_Pressure(Menu_hWin.ActivedWin);
				}
			}

			//Medibus FSM
			//Medibus_FiniteStateMachine();

#ifdef __ICCARM__
			//数据获取
			ds_getdatashare();
#endif

			AA_Range_AAID_Change(&Menu_hWin.InfoWin.InfoChangeFlag, (void*)&AA_all_data);

#ifdef __MEDIBUS
			Medibus_PrepareSend();
#endif
			Menu_hWin.InfoWin.InfoChangeFlag |= POWER_INFO_CHANGED_FLAG;
			Task &=~PARAMETER_UPDATE;
		}
		if(Task&WAVE_SCAN) //25ms
		{
			//irma_getpara();
			if((MenuType!=STANDBY_MENU)&&(StandbyState!=0xFF))
			{
				int flag;
				flag = InspFlagForCurve;
				DemoParaUpdate();//  [11/8/2022 lyy add Demo参数更新]
				CurveDataUpdate(&Menu_hWin.cWin,flag,&Menu_hWin.HomeWin);

				//
				// 肺复张波形刷新
				//
				if (LungRec_Wave_Scan > 10)
				{
					LungRec_Wave_Scan = 0;
					if (WM_IsWindow(Menu_hWin.ToolsWin.LungWin)&&(MenuType == TOOLS_MENU))
					{
						LungRec_CurveDataSave(Menu_hWin.ToolsWin.Tobj[0],flag,0 );
						//LungRec_CurveDataUpdate(Menu_hWin.ToolsWin.Tobj[0]);
					}
				}
				LungRec_Wave_Scan++;


				if(flag==InspFlagForCurve)
				{
					InspFlagForCurve = 0;
				}
			}
			Task &=~WAVE_SCAN;
		}
		GUI_TIMER_Exec();//定时器监测
		if(Task&OPERATE_HANDLE) //44ms
		{
#ifdef SELFTEST
			//if(!CHECK_DEBUG)
#endif
			{
				GUI_PollKeyMsg();
				KeyValue=GUI_GetKey();
				Knob_Handle(KeyValue);
				Function_Key_Handle(Start_Standby_Flag);
			}
			//_AssistFunctionIndication();
			if(ParaSettingState.flg)//zkq 20161013 添加读取按钮控件状态函数 若关闭窗口后处于选中状态 则弹起
			{
				WM_HWIN hWin;
				hWin = WM_InquireTouchHandle();
				if(hWin)
				{
					if(ParaSettingState.hWin!=hWin)
					{
						ParaSettingState.flg = 0;
						ButtonExCtrl_SetTriangleSign(ParaSettingState.hWin,0);
						ParaSettingState.hWin = 0;
					}
				}
			}
			WM_Exec();
			Task &=~OPERATE_HANDLE;
		}

		if(Task&GET_SYSTEM_TIME) //1000ms
		{
			if(ModeRefreshFlag > 6)
			{
				ModeRefreshFlag = 0;
				Menu_hWin.InfoWin.InfoChangeFlag |= MODES_INFO_CHANGED_FLAG;
			}
			else
			{
				ModeRefreshFlag++;
			}

			RTCTime = RTCReadTime();
			Get_TrendDate_1s(&MonitorPara,RTCTime);
			//            AlarmsWork.presentAlarms <<= 1;//test
			if (MasimoISA_EtCO2_Config_On != 2)//选配顺磁氧不需要氧电池校验
			{
				if( (delay_time[0] <= OS_TimeMS) &&  delay_time[0] != 0)        //zy   延迟90s后给bdu发送氧浓度校验命令
				{
					SendOperationMessage(OXY_CALIBRATION,USART0);
					delay_time[0] = 0;
				}
			}

			if(AlarmsWork.BeepState==FALSE)
			{
				if(AlarmsWork.MuteTimer>0)
				{
					AlarmsWork.MuteTimer--;
				}
				else
				{
					if(BUTTON_IsPressed(WM_GetDialogItem(Menu_hWin.hWin, GUI_ID_BUTTON4)))
					{
						BUTTON_SetPressed(WM_GetDialogItem(Menu_hWin.hWin, GUI_ID_BUTTON4),0);//sun add 20140519
					}
					AlarmsWork.BeepState = TRUE;
					LedUpdateRegister &= ~MUTE_LED;
					SendOperationMessage(LED_UPDATE_OPERATION,USART3);
#ifdef SELFTEST
					if(!CHECK_DEBUG)
#endif
					{
						SendOperationMessage(BUZZER_CONTROL_OPERATION,USART3); //  20130328
					}
					BeepAloudControl(0xFF);
				}
				Menu_hWin.InfoWin.InfoChangeFlag |= SILENCE_INFO_CHANGED_FLAG;
			}
			if((MenuType == SELFCHECK_ASK_MENU)&&(WM_IsWindow(Menu_hWin.ActivedWin))&&(AskTimer))
			{
				AskTimer--;
				WM_SendMessageNoPara(Menu_hWin.ActivedWin, WM_USER);
			}
			else if (AskTimer)
			{
				AskTimer--;
				if (ShutDownAskMenu_Flag)
				{
					WM_SendMessageNoPara(Menu_hWin.ShutDowAskWin,WM_USER);
				}
			}
			Menu_hWin.InfoWin.InfoChangeFlag |= TIMER_INFO_CHANGED_FLAG;
			//
			LungRecCountDown();
			//
			Task &=~GET_SYSTEM_TIME;
		}
		if(Task&ALARM_CHECK) //100ms
		{
			//
			CheckCurveRangUpdate();
			//
			//AA_all_data.AA_detail.AA_N2O.ET = 70;
#if (LOGO_FLAG == PENLON_LOGO)
			if(BeathSettingPara.BreathMode == ACGO_MODE)
			{
				if(AcgoTimeFlag != 62)
				{
					if(AcgoPromptFlag <= 2)
					{
						IMAGETEXT_SetBkColor(Menu_hWin.InfoWin.bObj[0], WINDOW_BK_COLOR);
					}
					else if((AcgoPromptFlag > 2)&&(AcgoPromptFlag <= 5))
					{
						IMAGETEXT_SetBkColor(Menu_hWin.InfoWin.bObj[0], GUI_YELLOW);
					}
					else
					{
						AcgoPromptFlag = 0;
					}
					AcgoPromptFlag++;
					AcgoTimeFlag++;
				}
			}
#endif

#ifdef __ICCARM__
			//数据共享协议
			uart_dp_send();
			uart_dp_recv();
			uart_dp_tim();
			uart_dp_fsm();
#endif

			CheckAlarm(InspFlagForAlarm);
#ifndef __debug
			InspFlagForAlarm = 0;     //zytmp
#endif
			Task &=~ALARM_CHECK;
		}
		if(Task&INFO_WINDOW_UPDATE) //59ms
		{
#ifdef SELFTEST
			if(CHECK_DEBUG == 1)
			{
				SelfTest_App();
			}
#endif
			Info_cnt++;
			if(Info_cnt > 2)
			{
				Info_cnt = 0;
				if(PlanMenuType == UPDATE_PROGRAM_MODE)  //sun add 20140514
				{
					UpdateProgramWin(Menu_hWin.RecordWin);
				}
				if((MenuType == BATTERY_CALIBRATION_MENU)&&(WM_IsWindow(Menu_hWin.RecordWin)))
				{
					WM_SendMessageNoPara(Menu_hWin.RecordWin,WM_USER);
				}
			}
			if(StandbyState == 0)
			{
				if(pHaseinMoudleCheck != LastpHaseinMoudleCheck)
				{
					if(MenuType==SYSTEM_SETTING_MENU)
					{
						if(MULTIPAGE_GetSelection(WM_GetDialogItem(Menu_hWin.ActivedWin,GUI_ID_MULTIPAGE0))==1)//更新AA按键
						{
							hWin = WM_GetDialogItem(Menu_hWin.ActivedWin,GUI_ID_MULTIPAGE0);
							hWin = MULTIPAGE_GetWindow(hWin,1);
							WM_SendMessageNoPara(hWin, WM_INIT_DIALOG);
						}
					}
					LastpHaseinMoudleCheck = pHaseinMoudleCheck;
				}
			}
			//CalibrationProcess_Ctrl();//校验数据与PC通讯
			Menu_hWin.InfoWin.InfoChangeFlag = InfoWinStateUpdate(Menu_hWin.InfoWin);
			Task &=~INFO_WINDOW_UPDATE;
		}
		if(SaveOperationTimeFlag == TRUE)
		{
			WriteOperationTime(&OperationTime);
			//if(MenuType==SYSTEM_INFO_MENU)
			//{
			//    _UpdateOperationTime(Menu_hWin.RecordWin);//RecordWin
			//}

			if(MenuType==SYSTEM_SETTING_MENU) //wff mofify 20191009 解决运行时间不更新
			{
				if(MULTIPAGE_GetSelection(WM_GetDialogItem(Menu_hWin.ActivedWin,GUI_ID_MULTIPAGE0))==0)//更新
				{
					_UpdateOperationTime(Menu_hWin.ActivedWin);//RecordWin  wff mofify 20191009 解决运行时间不更新
				}
			}
			SaveOperationTimeFlag = FALSE;
		}
#ifndef NEW_POWER_COMM
		if(BatteryAdValue)//完成了一次电量采集
		{
			// SendOperationMessage(CO2_PUMPRUNHOUR,USART2);
			//首先得到电池电压RealtimePara.BatteryVoltage,其次判断BatteryAdFilter返回值
			//如果返回值为0，则表示没有开启电池校验。
			if((BatteryAdValue =BatteryAdFilter(BatteryAdValue,&RealtimePara.BatteryVoltage))
				&&(alarm_battery_cali == 0xFF))
			{
				//                 if(Power_GetState() == BAT_CALI)//BAT_WORK)//如果接入了交流电，则不能保存电量值
				//                 {
				//                     Wr_Battery_Ad(BatteryAdValue); //电池校验，每隔1min写入一个数据到eeprom
				//                 }
			}
			BatteryAdValue = 0;
		}
#else
        RealtimePara.BatteryVoltage = Power_GetBatteryVoltage();
        Power_ChkState();//检测电源供电状态：电池还是电源供电
        ChargeFinishFlag = CheckChargeFinish();//检测电池状态：充电中，充电完成，没有电池
//         Menu_hWin.InfoWin.InfoChangeFlag |= POWER_INFO_CHANGED_FLAG;
#endif
		//
		Receive_ShutDown_Flag = Power_GetOnOffState();
		if (Receive_ShutDown_Flag == SHUT_DOWN && ShutDownAskMenu_Flag==0)
		{
			if (MenuType == SELFCHECK_ASK_MENU)
			{
				AskMenu_Flag = 0;
				AskTimer = 0;
				PlanMenuType = STANDBY_MENU;
				Task |= MEAU_UPDATE;
			}
			if ( !AskTimer)
			{
				ShutDownAskMenu_Flag = 1;
				Task |= MEAU_UPDATE;
			}
		}
		else if (Receive_ShutDown_Flag == CANCEL_SHUT_DOWN && ShutDownAskMenu_Flag == 1)
		{
            SendShutDownState = CANCEL_SHUT_DOWN;
			AskTimer = 2;
			ShutDownAskMenu_Flag = 0;
            Task |= MEAU_UPDATE;
		}

		if(butter_press_count >=4)//开机按两次进入校验
		{
			butter_press_count=0;
			TouchScreenCalibrate = 1;
			PlanMenuType = TOUCH_CALIBRATION_MENU;
			Task |=MEAU_UPDATE;
		}
	}
}
/***************************************************************************************************
* @brief  初始化呼吸参数
* @para
* @retval
*/
void InitSettingParameter(BREATH_SET_PARA * BeathSettingPara)
{
	//zkq modify 20170116
	//BDU开机比GUI更快，如果BDU在GUI初始化前发送了ACGO或者手动模式，那么GUI在初始化的时候应该保留这些状态。
	if((BeathSettingPara->BreathMode != ACGO_MODE) && (BeathSettingPara->BreathMode != MANUAL_MODE))
	{
		BeathSettingPara->BreathMode=VCV_MODE;
		//         BeathSettingPara->BreathMode_Queue[MACHINE_INDEX] = IDLE_MODE;
		//         BeathSettingPara->BreathMode_Queue[MANUAL_INDEX] = IDLE_MODE;
	}
	else
	{
		//         BeathSettingPara->BreathMode_Queue[MACHINE_INDEX] = VCV_MODE;//如果开机前启动了手动或者ACGO，机控模式未初始化，所以要存一个默认值。

		BreathMode_BeforeACGO = VCV_MODE;

	}
	BeathSettingPara->SettingFiO2Value = 21;
	BeathSettingPara->SettingFtrValue = 30;//5
	BeathSettingPara->SettingPEEPValue = 0;//0 cpp md 20130522
	BeathSettingPara->SettingBipapRateValue = 10;
	BeathSettingPara->SettingPinspValue = 15;
	BeathSettingPara->SettingPsupportValue = 15;
//#if(LOGO_FLAG != HOSPITECH_LOGO)
//	BeathSettingPara->SettingPtrValue = 3;
//#else
	BeathSettingPara->SettingPtrValue = -3;
//#endif
	
	//#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C) //zkq modify 20171030 麻醉呼吸机f改为12
	BeathSettingPara->SettingRateValue = 12;
	BeathSettingPara->SettingotherRateValue = 12;
	//#else
	//    BeathSettingPara->SettingRateValue = 20;
	//    BeathSettingPara->SettingotherRateValue = 20;
	//#endif
	BeathSettingPara->SettingTiValue = 166;//10 cpp md 20130411 //ylk mofidy 20181225 166
	BeathSettingPara->SettingTpValue = 0;
	BeathSettingPara->SettingVtValue = 510;//500
	BeathSettingPara->TriggerMode = PRESSURE_TRIGGER;
	//BeathSettingPara->SettingThighValue = 10;
	BeathSettingPara->SettingSimvRateValue = 10;
	BeathSettingPara->ApnoeaMode = VCV_MODE;
	BeathSettingPara->SettingSighTimesValue = 100;
	BeathSettingPara->SettingIEValue = 8;
	BeathSettingPara->SettingTslopeValue = 30; //  20161020
	BeathSettingPara->EsenseValue = 25;  //sun add 20140623

	BeathSettingPara->SettingPhighValue = 15;
	BeathSettingPara->SettingPlowValue = 5;
	BeathSettingPara->SettingPHLimitValue = 40;
	if (Patient_GetPType() == PATIENTTYPE_INFANT)
	{
		BeathSettingPara->SettingThighValue = 5;
		BeathSettingPara->SettingTlowValue = 10;
		BeathSettingPara->SettingVSRateValue = 12;
		BeathSettingPara->SettingVtSupportValue = 35;
	}
	else if (Patient_GetPType() == PATIENTTYPE_PAEDIATRIC)
	{
		BeathSettingPara->SettingThighValue = 10;
		BeathSettingPara->SettingTlowValue = 20;
		BeathSettingPara->SettingVSRateValue = 8;
		BeathSettingPara->SettingVtSupportValue = 120;
	}
	else
	{
		BeathSettingPara->SettingThighValue = 20;
		BeathSettingPara->SettingTlowValue = 40;
		BeathSettingPara->SettingVSRateValue = 4;
		BeathSettingPara->SettingVtSupportValue = 510;
	}

	SetVtValue_Limit(Patient_GetInfo(),BeathSettingPara->BreathMode);
}
/***************************************************************************************************
* @brief  初始化监测参数
* @para
* @retval
*/
void InitMonitorParameter(MONITOR_PARA * Parameter)
{
	Parameter->ComplianceValue = 0;
	Parameter->FiO2Value = 0;
	Parameter->MinuteVolumeValue = 0;
	Parameter->PeepValue = 0;
	Parameter->PmeanValue = 0;
	Parameter->PpeakValue = 0;
	Parameter->PplatValue = 0;
	Parameter->RateTotalValue = 0;
	Parameter->ResistValue = 0;
	Parameter->VteValue = 0;
	Parameter->VtiValue = 0;
	Parameter->VentState = EXP_PHASE;
	Parameter->PminValue = 0;
	Parameter->IEValue = 500; //  20130615
}
/***************************************************************************************************
* @brief  仅作为测试用   for test 20130328
* @para
* @retval
*/
void InitRealtimeParameter(REALTIME_PARA * Parameter)
{
	Parameter->Pressure = 0;
	Parameter->Flow = 0;
	Parameter->Volume = 0;
	Parameter->BatteryVoltage = 850 * 4;
	Parameter->AirGasPressure = 0;
	Parameter->O2GasPressure = 0;
	Parameter->EtCO2 = 0;
	Parameter->SpO2 = 0;
}
void InitAlarmSaveData(ALARM_SET_PARA * Parameter)
{
	Alarm_SaveVent_para.PressureLowValue = Parameter->PressureLowValue;
	Alarm_SaveVent_para.PressureHighValue = Parameter->PressureHighValue;
	Alarm_SaveVent_para.RateLowValue = Parameter->RateLowValue;
	Alarm_SaveVent_para.RateHighValue = Parameter->RateHighValue;
	Alarm_SaveVent_para.MVLowValue = Parameter->MVLowValue;
	Alarm_SaveVent_para.MVHighValue = Parameter->MVHighValue;
	Alarm_SaveVent_para.VteLowValue = Parameter->VteLowValue;
	Alarm_SaveVent_para.VteHighValue = Parameter->VteHighValue;
	//Alarm_SaveVent_para.PATIENTTYPE_ChangeFlag = 1;
	ModeChange_Flag = 1;
	if((BeathSettingPara.BreathMode == MANUAL_MODE))
	{
		Alarm_SaveVent_para.PATIENTTYPE_ChangeFlag = 1;
		//相关参数重新赋值
		Parameter->PressureLowValue = 0;
		Parameter->RateLowValue = 0;
		Parameter->RateHighValue = 100;
		Parameter->MVLowValue = 0;
		Parameter->MVHighValue = 99;
		Parameter->VteLowValue = 0;
		Parameter->VteHighValue = 2000;
	}
	else
	{
		Alarm_SaveVent_para.PATIENTTYPE_ChangeFlag = 0;
	}
}

/***************************************************************************************************
* @brief  初始化报警参数
* @para
* @retval
*/
#define MV_HIGH_RANGE      ((float)(1.4))
#define MV_LOW_RANGE       ((float)(0.6))
void InitAlarmSettingParameter(ALARM_SET_PARA * Parameter, Patient_Info_t *Patient, BREATH_SET_PARA * BeathSettingPara)
{
	unsigned char value;
	unsigned long MvTmp = 0;
	Parameter->AlarmVolumeValue = 0;
	Parameter->ApneaTimerValue = 15;
	Parameter->FiO2HighValue = 101;//djq modify 20161127 100→101默认Off
	//Parameter->FiO2LowValue = 35;
	Parameter->FiO2LowValue = 20;  //sun add 20140710
	Parameter->RateHighValue = 100;
	Parameter->RateLowValue = 5;

	//Parameter->MVHighValue = 40;
	//Parameter->MVLowValue = 0;
	//MV High   (vt*f*Range/1000)
	MvTmp = BeathSettingPara->SettingVtValue*BeathSettingPara->SettingRateValue*MV_HIGH_RANGE;
	if(MvTmp % 1000)//如果有小数，则进位
	{
		MvTmp = MvTmp - (MvTmp %1000) + 1000;
	}
	Parameter->MVHighValue = MvTmp/1000;
	if(Parameter->MVHighValue < 1)
	{
		Parameter->MVHighValue = 1;
	}

	MvTmp = BeathSettingPara->SettingVtValue*BeathSettingPara->SettingRateValue*MV_LOW_RANGE;
	if(MvTmp % 1000)//如果有小数，则进位
	{
		MvTmp = MvTmp - (MvTmp %1000);
		if(MvTmp)
		{
			MvTmp -= 1;//如果MvTmp大于零，则减少1
		}
	}
	Parameter->MVLowValue =  MvTmp/1000;
	if(Parameter->MVLowValue >= Parameter->MVHighValue)
	{
		Parameter->MVLowValue = Parameter->MVHighValue - 1;
	}

	if(Patient_GetPType() == PATIENTTYPE_INFANT)//婴儿30mmH2O
	{
		Parameter->PressureHighValue = 30;//ylk md 20191126
	}
	else
	{
		Parameter->PressureHighValue = 40;
	}

	Parameter->PressureLowValue = 5;
	//Parameter->VteHighValue = 1000;
	//Parameter->VteLowValue = 200;
	//Vte  vt*1.3
	Parameter->VteHighValue = BeathSettingPara->SettingVtValue*MV_HIGH_RANGE;
	if(Parameter->VteHighValue%10)
	{
		Parameter->VteHighValue -= (Parameter->VteHighValue%10);
		Parameter->VteHighValue += 10;
	}
	if(Parameter->VteHighValue < 20)
	{
		Parameter->VteHighValue = 20;
	}
	Parameter->VteLowValue = BeathSettingPara->SettingVtValue*MV_LOW_RANGE;
	if(Parameter->VteLowValue%10)
	{
		Parameter->VteLowValue -= (Parameter->VteLowValue%10);
	}
	if(Parameter->VteLowValue >= Parameter->VteHighValue)
	{
		Parameter->VteLowValue = Parameter->VteHighValue - 10;
	}
	else if(Parameter->VteLowValue < 10)
	{
		Parameter->VteLowValue = 10;
	}

	Parameter->SpO2LowValue = 92;
	Parameter->SpO2HighValue = 100;
	Parameter->PulseLowValue = 50;
	Parameter->PulseHighValue = 120;

	Parameter->EtCO2LowValue = 20;
	//  Parameter->EtCO2LowValue = 0;  //sun add 20140710
	Parameter->EtCO2HighValue = 50;
	Parameter->FiCO2LowValue = 0; //20; //cpp md 20130717
	Parameter->FiCO2HighValue = 10;


	if((ETCO2Unit == 0)||(ETCO2Unit == 2)) //  20130531
	{
		Parameter->EtCO2LowValue = 27;
		// 		Parameter->EtCO2LowValue = 0;  //sun add 20140710
		Parameter->EtCO2HighValue = 67;
		Parameter->FiCO2LowValue = 0;//27; //cpp md 20130717
		Parameter->FiCO2HighValue = 13;//ylk modify 20180611 67->13
	}
	//FiN2O High
	Parameter->N2OHighValue = 82;
	Parameter->N2OLowValue = 0;
	//EtAA High
	Parameter->EtAAHighValue = 30;
	Parameter->EtAALowValue = 0;
	//FiAA High
	Parameter->FiAAHighValue = 30;
	Parameter->FiAALowValue = 0;

	value = RTEEPROMread(ALARM_VOICE_VOLUME_ADDR);
	if(value<=4) //((value>=1)&&(value<=5)) //cpp md 20130328
	{
		Parameter->AlarmVolumeValue = (int)value;
	}
	else
	{
		Parameter->AlarmVolumeValue = 4; //5 cpp md 20130328
		RTEEPROMwrite(ALARM_VOICE_VOLUME_ADDR,4); //5 cpp md 20130328
	}
	SendOperationMessage(BUZZER_VOLUME_SETTING,USART3);
	//*//  20130409

	//djq add 2013-12-26
	if(MasimoISA_EtCO2_Config_On != 2)
	{
		value = RTEEPROMread(O2_ON_OFF_ADDR);
		if((value == 0)||(value == 1))
		{
			O2ModuleOff = value;
		}
		else
		{
			O2ModuleOff = 1;
			RTEEPROMwrite(O2_ON_OFF_ADDR,1);
		}
	}
	//2016.6.29
	if(Masimo_EtCO2_Config_On || MasimoISA_EtCO2_Config_On \
		|| EtCO2_Config_On || EtCO2_KM7002_Config_On)
	{
		EtCO2ModuleOff = RTEEPROMread(ETCO2_ON_OFF_ADDR);
		if(MasimoISA_EtCO2_Config_On == 2)//ISA O2
		{
			O2ModuleOff = !EtCO2ModuleOff;
		}
	}
	//*/
	//VolumeChannelSelect(Parameter->AlarmVolumeValue); //cpp del 20130328

	//djq add 20170407
	value = RTEEPROMread(DIRVER_GAS_ADDR);
	if((value == 0)||(value == 1))
	{
		DirverGasFlag = value;
	}
	else
	{
		DirverGasFlag = 0;//默认为氧气气源
		RTEEPROMwrite(DIRVER_GAS_ADDR,1);
	}
	if(DriverGas_Chose_Flag)//djq add 20170609驱动气体选配
	{
		SendOperationMessage(DIRVER_GAS_FOR_BDU_COM,USART0);
		Menu_hWin.InfoWin.InfoChangeFlag |= DRIVER_GAS_TYPE_FLAG;
	}
	value = RTEEPROMread(AIR_SUPPLY_SWITCH_ADDR);
	if((value == 0)||(value == 1))
	{
		AlarmAirSourceSwitch = value;
	}
	else
	{
		AlarmAirSourceSwitch = 1;//默认为开
		RTEEPROMwrite(AIR_SUPPLY_SWITCH_ADDR,1);
	}
	//
	value = RTEEPROMread(O2_ON_OFF_ADDR);
	if(FiO2_Config_On)//djq add 20170607 氧浓度传感器选配
	{
		if((value == 0)||(value == 1))
		{
			O2ModuleOff = value;
		}
		else
		{
			O2ModuleOff = 1;
			RTEEPROMwrite(O2_ON_OFF_ADDR,1);
		}
	}
	else//开启选配
	{
		O2ModuleOff = 1;
	}
}
/***************************************************************************************************
* @brief  初始化单元软件版本信息        20130328
* @para
* @retval
*/
/***************************************************************************************************
* @brief  初始化软件版本信息
* @para
* @retval
*/
void InitSoftwareVersionParameter(void)
{
	SystemVersion.MainVersion  = SYS_MAIN_VERSION;
	SystemVersion.SubVersion   = SYS_SUB_VERSION;
	SystemVersion.DebugVersion = SYS_DEBUG_VERSION;
	SystemVersion.TestVersion  = SYS_TEST_VERSION;

	GUIVersion.MainVersion   = GUI_MAIN_VERSION;
	GUIVersion.SubVersion    = GUI_SUB_VERSION;
	GUIVersion.DebugVersion  = GUI_DEBUG_VERSION;
	GUIVersion.TestVersion   = GUI_TEST_VERSION;

	//BDUVersion.MainVersion   = 1;
	//BDUVersion.SubVersion    = 0;
	//BDUVersion.DebugVersion  = 0;
	//BDUVersion.TestVersion   = 0;

	//PMUVersion.MainVersion   = 1;
	//PMUVersion.SubVersion    = 0;
	//PMUVersion.DebugVersion  = 0;
	//PMUVersion.TestVersion   = 0;
}

/***************************************************************************************************
* @brief  初始化报警
* @para
* @retval
*/
void InitAlarmsWork(ALARM_WORK * work)
{
	work->BeepState = TRUE;
	work->MuteTimer = 0;//120
	work->newAlarms = 0;
	work->presentAlarms = 0;
	work->unclearedAlarms = 0;
}
/***************************************************************************************************
* @brief  初始化菜单
* @para
* @retval
*/
void InitMenuHwin(MENU_HWIN *hWin)
{
	if(DriverGas_Chose_Flag)//djq add 20170609驱动气体选配
	{
		hWin->InfoWin.InfoChangeFlag = DRIVER_GAS_TYPE_FLAG;//djq add 20170410开机显示。
	}
	hWin->cWin.LoopFlag1 = 0;
	hWin->cWin.WaveForms[0] = WAVEFORM_PRESSURE;
	hWin->cWin.WaveForms[1] = WAVEFORM_FLOW;
	hWin->cWin.WaveForms[2] = WAVEFORM_VOLUME;
	hWin->cWin.WaveForms[3] = LOOP_VF;

	hWin->cWin.FocusCurve = -1;
}
/***************************************************************************************************
* @brief  初始化二氧化碳、血氧模块状态     20130412
* @para
* @retval
*/
void initco2spo2switch(void)
{
	//N2O
#ifdef N2O_SWITCH_FLAG //  [7/5/2022 yaoyao add]
	N2O_Switch = RTEEPROMread(N2O_ON_OFF_ADDR);
#endif
	//SpO2
	if(Masimo_SpO2_Config_On || SpO2_Config_On)
	{
		SpO2ModuleOff = RTEEPROMread(SPO2_ON_OFF_ADDR);
	}
	else
	{
		SpO2ModuleOff = 1;
	}
	//AA CO2
	if(EtCO2_Config_On || MasimoISA_EtCO2_Config_On \
		|| Masimo_EtCO2_Config_On || EtCO2_KM7002_Config_On || Drager_EtCO2_Config_On)
	{
		EtCO2ModuleOff = RTEEPROMread(ETCO2_ON_OFF_ADDR);
	}
	else
	{
		EtCO2ModuleOff = 0;
	}
	//
	WaveFormsId2 = RTEEPROMread(WAVEFORMSID2_ADDR);
	if((WaveFormsId2 == 3) && (!SpO2ModuleOff))
	{
		Menu_hWin.cWin.WaveForms[2] = WaveFormsId2;
	}
	else if((WaveFormsId2 >= 4 && WaveFormsId2 <= 7) && (EtCO2ModuleOff))
	{
		Menu_hWin.cWin.WaveForms[2] = WaveFormsId2;
	}
	else
	{
		WaveFormsId2 = WAVEFORM_VOLUME;
		Menu_hWin.cWin.WaveForms[2] = WAVEFORM_VOLUME;
		RTEEPROMwrite(WAVEFORMSID2_ADDR, WaveFormsId2);
	}
	//if(EtCO2ModuleOff)//20210909 test
	{
		if(AA_all_data.AA_detail.AA_AA1.ID == AA_ID_NONE)
		{
			strcpy(AA_all_data.AA_detail.AA1_Type_Text, Sys_Lib_GetDispStr(DP_AA1));
		}
		if(AA_all_data.AA_detail.AA_AA2.ID == AA_ID_NONE)
		{
			strcpy(AA_all_data.AA_detail.AA2_Type_Text, Sys_Lib_GetDispStr(DP_AA2));
		}
	}
#if (LOGO_FLAG == MSW_LOGO)
	if(((AA_Module_Flag.EtCO2ModuleExist&0x07) == 0x01))
	{
		SendOperationMessage(CO2_MODULEON,USART1);
	}
#else
	if(AA_Module_Flag.ETCO2_FLAG == 1)
	{
		if(EtCO2ModuleOff)
		{
			SendOperationMessage(CO2_MODULEON,USART2);
		}
		else
		{
			SendOperationMessage(CO2_MODULEOFF,USART2);
		}
	}
#endif
	else if(AA_Module_Flag.ETCO2_FLAG == 0)//djq add 2013-12-23
	{
		if(!MasimoISA_EtCO2_Config_On)
		{
			SendOperationMessage(MODULESWITCH_FLAG,USART2);
		}
	}
	if(MasimoISA_EtCO2_Config_On)
	{
		//SetISA_O2IRDelay(0x08); //ylk modify 20181130
		AA_SetIRO2Value(0x08);
		SendOperationMessage(SET_AA_IRO2DELAY,USART2);
	}
	if(Drager_EtCO2_Config_On && (!AA_Module_Flag.ETCO2_FLAG))//ylk add 20181204
	{
		SendOperationMessage(SET_AA_OPERATE_MEASURE,USART2);
	}
	if((Drager_EtCO2_Config_On == 2 || MasimoISA_EtCO2_Config_On == 2) /*&&(AA_Module_Flag.EtCO2ModuleExist  == 2)*/)
	{
		O2ModuleOff = !EtCO2ModuleOff;
		RTEEPROMwrite(O2_ON_OFF_ADDR,O2ModuleOff);
	}
}
/***************************************************************************************************
* @brief  初始化报警记录清单
* @para
* @retval
*/
void InitAlarmRecordList(ALARM_RECORD_LIST * ListHead)
{
	ListHead->Head = RTEEPROMread(ALARM_RECORD_LIST_ADDR);
	ListHead->Head |= (RTEEPROMread(ALARM_RECORD_LIST_ADDR+1)<<8);
	ListHead->Rear = RTEEPROMread(ALARM_RECORD_LIST_ADDR+2);
	ListHead->Rear |= (RTEEPROMread(ALARM_RECORD_LIST_ADDR+3)<<8);
	ListHead->Count = RTEEPROMread(ALARM_RECORD_LIST_ADDR+4);
	ListHead->Count |= (RTEEPROMread(ALARM_RECORD_LIST_ADDR+5)<<8);
	if(ListHead->Count > MAX_RECORD_NUM)
	{
		ListHead->Head = 0;
		ListHead->Rear = 0;
		ListHead->Count = 0;
		WriteAlarmRecordHeadInfo(ListHead);
	}
}
/***************************************************************************************************
* @brief  初始化运行时间
* @para
* @retval
*/
void InitOperationTime(OPERATION_TIME * RunTime)
{
	//RunTime->Hour = 0;
	//RunTime->Hour |= (unsigned int)(RTEEPROMread(RUN_TOTAL_TIME_ADDR));
	//RunTime->Hour |= (unsigned int)((RTEEPROMread(RUN_TOTAL_TIME_ADDR+1)<<8));
	//RunTime->Hour |= (unsigned int)((RTEEPROMread(RUN_TOTAL_TIME_ADDR+2)<<16));
	//RunTime->Hour |= (unsigned int)((RTEEPROMread(RUN_TOTAL_TIME_ADDR+3)<<24));
	//RunTime->Min = RTEEPROMread(RUN_TOTAL_TIME_ADDR+4);

	RunTime->Hour = 0;
	RunTime->Hour |= (unsigned int)(RTEEPROMread(RUN_TOTAL_TIME_ADDR));
	RunTime->Hour |= (unsigned int)((RTEEPROMread(RUN_TOTAL_TIME_ADDR+1)<<8));
	RunTime->Min |= (unsigned int)(RTEEPROMread(RUN_TOTAL_TIME_MIN_ADDR));
}
/***************************************************************************************************
* @brief  写入运行时间到EE
* @para
* @retval
*/
void  WriteOperationTime(OPERATION_TIME * RunTime)
{
	//RTEEPROMwrite(RUN_TOTAL_TIME_ADDR,RunTime->Hour & 0xFF);
	//RTEEPROMwrite(RUN_TOTAL_TIME_ADDR+1,(RunTime->Hour>>8) & 0xFF);
	//RTEEPROMwrite(RUN_TOTAL_TIME_ADDR+2,(RunTime->Hour>>16) & 0xFF);
	//RTEEPROMwrite(RUN_TOTAL_TIME_ADDR+3,(RunTime->Hour>>24) & 0xFF);
	//RTEEPROMwrite(RUN_TOTAL_TIME_ADDR+4,RunTime->Min);

	RTEEPROMwrite(RUN_TOTAL_TIME_ADDR,RunTime->Hour & 0xFF);
	RTEEPROMwrite(RUN_TOTAL_TIME_ADDR+1,(RunTime->Hour>>8) & 0xFF);
	RTEEPROMwrite(RUN_TOTAL_TIME_MIN_ADDR,RunTime->Min & 0xFF);
}
/***************************************************************************************************
* @brief
* @para
* @para
*
*/
void CurveTypeChanged(int type)
{
}

/***************************************************************************************************
* @brief  任务调度管理
* @para
* @retval
*/
void TaskSchedulerManager(GUI_TIMER_TIME t)
{
	static GUI_TIMER_TIME Get_VentTime_T = 0; //  20130617

	static GUI_TIMER_TIME Wave_Scan_T = 0;
	static GUI_TIMER_TIME Parameter_Update_T = 0;
	static GUI_TIMER_TIME Operate_Inquire_T = 0;
	static GUI_TIMER_TIME Get_System_T = 0;
	static GUI_TIMER_TIME Alarm_Check_T = 0;
	static GUI_TIMER_TIME Info_Window_Update_T = 0;
	static unsigned char r = 0;
	if(!r)
	{
		Get_VentTime_T = t; //  20130617

		Wave_Scan_T = t;
		Parameter_Update_T = t;
		Operate_Inquire_T = t;
		Get_System_T = t;
		Alarm_Check_T = t;
		Info_Window_Update_T = t;
		r = 1;
	}
	else
	{
		//------------------------------------------------------------------------------------------
		if(t>=Wave_Scan_T)
		{
			if((t-Wave_Scan_T)>=Wave_Scan_Time)
			{
				Task |=WAVE_SCAN;
				Wave_Scan_T = t;
			}
		}
		else
		{
			if((Wave_Scan_T-(2147483647+t))>=Wave_Scan_Time)
			{
				Task |=WAVE_SCAN;
				Wave_Scan_T = t;
			}
		}
		//------------------------------------------------------------------------------------------
		if(t>=Parameter_Update_T)
		{
			if((t-Parameter_Update_T)>=Parameter_Update_Time)
			{
				Task |=PARAMETER_UPDATE;
				Parameter_Update_T = t;
			}
		}
		else
		{
			if((Parameter_Update_T-(2147483647+t))>=Parameter_Update_Time)
			{
				Task |=PARAMETER_UPDATE;
				Parameter_Update_T = t;
			}
		}
		//------------------------------------------------------------------------------------------
		if(t>=Operate_Inquire_T)
		{
			if((t-Operate_Inquire_T)>=Operate_Inquire_Time)
			{
				Task |=OPERATE_HANDLE;
				Operate_Inquire_T = t;
			}
		}
		else
		{
			if((Operate_Inquire_T-(2147483647+t))>=Operate_Inquire_Time)
			{
				Task |=OPERATE_HANDLE;
				Operate_Inquire_T = t;
			}
		}
		//------------------------------------------------------------------------------------------
		if(t>=Get_System_T)
		{
			if((t-Get_System_T)>=Get_System_Time)
			{
				Task |=GET_SYSTEM_TIME;
				Get_System_T = t;
			}
		}
		else
		{
			if((Get_System_T-(2147483647+t))>=Get_System_Time)
			{
				Task |=GET_SYSTEM_TIME;
				Get_System_T = t;
			}
		}
		//------------------------------------------------------------------------------------------
		if(t>=Alarm_Check_T)
		{
			if((t-Alarm_Check_T)>=Alarm_Check_Time)
			{
				Task |=ALARM_CHECK;
				Alarm_Check_T = t;
			}
		}
		else
		{
			if((Alarm_Check_T-(2147483647+t))>=Alarm_Check_Time)
			{
				Task |=ALARM_CHECK;
				Alarm_Check_T = t;
			}
		}
		//------------------------------------------------------------------------------------------
		if(t>=Info_Window_Update_T)
		{
			if((t-Info_Window_Update_T)>=Info_Window_Update_Time)
			{
				Task |=INFO_WINDOW_UPDATE;
				Info_Window_Update_T = t;
			}
		}
		else
		{
			if((Info_Window_Update_T-(2147483647+t))>=Info_Window_Update_Time)
			{
				Task |=INFO_WINDOW_UPDATE;
				Info_Window_Update_T = t;
			}
		}
		//------------------------------------------------------------------------------------------
	}
}
/***************************************************************************************************
* @brief  检查充电完成
* @para     0充电未完成 1充电完成 2没有电池
* @retval
*/
Int8U CheckChargeFinish(void)
{
  Int8U state = 0;
#ifdef NEW_POWER_COMM
  state = CheckChargeStatus();
#else
  static Int8U timer = 0;
  if(CheckChargeStatus())
  {
    if(timer>=3)
    {
      state = 1;
    }
    else
    {
      timer++;
    }
  }
  else
  {
    timer = 0;
  }
#endif
  return(state);
}
/***************************************************************************************************
* @brief  读取语言类型
* @para
* @retval
*/
// unsigned int ReadLanguageType(void)
// {
//     unsigned int type = 0;
//     type =RTEEPROMread(LANGUAGE_TYPE_ADDR);
// #ifndef LANGUAGE_CHINESE_FLAG
//     if((type < 1)||(type > 3))
//     {
//         type = 1;
//     }
// #else
// 	if((type < 0)||(type > 1))
// 	{
// 		type = 0;
// 	}
// #endif
//     return(type);
// }

void InitLanguage()
{
	LanguageType = RTEEPROMread(LANGUAGE_TYPE_ADDR);
	if(LanguageType > Disp_Lg_Bulgarian)
	{
		LanguageType = Disp_Lg_English;
		RTEEPROMwrite(LANGUAGE_TYPE_ADDR, LanguageType);
	}
	//else if(LanguageType == Disp_Lg_Chinese)
	//{
	//	LanguageType = Disp_Lg_English;
	//	RTEEPROMwrite(LANGUAGE_TYPE_ADDR, LanguageType);
	//}
	Sys_Lib_LgInit(LanguageType);
}
/***************************************************************************************************
* @brief  读取二氧化碳单位
* @para
* @retval
*/
unsigned int ReadEtCO2Unit(void)
{
	unsigned int type = 0;
	type =RTEEPROMread(ETCO2_UNIT_ADDR);
	if(type>2)
	{
		type = 0;
	}
	return(type);
}

//保存电池校验的日期和时间
void Wr_Battery_DateTime(RTC_TIMER *Time)
{
}
//读取电池校验的日期和时间
void Rd_Battery_DateTime(RTC_TIMER *Time)
{
}
#define BATTERY_MAX_AD    (1023*4)//15.6V
#define BATTERY_MIN_AD    (656*4)//10V
int Rd_Battery_Ad(void)
{
}
void Wr_Battery_Ad(int BatteryAd)
{
}
/*
把电池信息从结构体的变量保存到EEPROM中
用于上位机下发数据后使用
*/
void Save_Threth_Alarm_Battery(BATTERY_AD_PARA *Battery_P)
{
}
char ALARM_BATTERY_LOW_MAX = 20;   //剩余20分钟关机  10~20
char ALARM_BATTERY_LOW_MIN = 10;   //剩余20分钟关机  10~20
char ALARM_BATTERY_EMPTY_MAX = 10;    //剩余10分钟关机 5~10
char ALARM_BATTERY_EMPTY_MIN = 5;     //剩余10分钟关机 5~10
#define ALARM_INTERVAL    5  //电池耗尽和电池电量低的最小时间间隔
void Set_Threth_Alarm_Battery(void)
{
}
//#endif
void Rd_Threth_Alarm_Battery()
{

	//zkq add 20170116 电池写入固定值 x*4
#ifndef NEW_POWER_COMM
	BatteryPara.Battery_Step3 = 780*4;
	BatteryPara.Battery_Step2 = 750*4;
	BatteryPara.Battery_Low   = 733*4;
	BatteryPara.Battery_Empty = 710*4;
#else
    BatteryPara.Battery_Step3 = 8000;
    BatteryPara.Battery_Step2 = 4500;
    BatteryPara.Battery_Low   = 1500;
    BatteryPara.Battery_Empty = 100;
    BatteryPara.Battery_Disconnect = 0;
#endif
    
}
/***************************************************************************************************
* @brief  获取呼吸时间，包括吸气和呼气时间的计数，吸气和呼气时间计数器每1ms加1   //  20130616
* @para
* @retval
*/
VENT_TIME GetVentTime(int VentState)
{
	static int preVentState;
	static int inspCount = 0;
	static int expCount = 0;
	static VENT_TIME _VentTime;

	if(BeathSettingPara.BreathMode == ACGO_MODE)
	{
		_VentTime.ExpTime = 0;
	}
	else
	{
		if(VentState == INSP_PHASE)
		{
			if(preVentState == EXP_PHASE)
			{
				_VentTime.ExpTime = expCount;
				inspCount = 1;
			}
			else
			{
				inspCount++;
			}
			preVentState = INSP_PHASE;
		}
		else if(VentState == EXP_PHASE)
		{
			if(preVentState == INSP_PHASE)
			{
				_VentTime.InspTime = inspCount;
				expCount = 1;
			}
			else
			{
				expCount++;
			}
			preVentState = EXP_PHASE;
		}
	}
	return (_VentTime);
}

/***************************************************************************************************
* @brief   获取吸呼比
* @para    _VentTime   以ms为单位 如 _VentTime.InspTime = 1000 表示吸气时间为 1000 ms
* @retval  IERatio     吸呼比 放大了100倍，且最高位为标志位，若最高位为1，则吸气时间大于呼气时间
*                      若IERatio = 0x8000|(200) = 0x80C8，则 I : E = 2.00 : 1
*                      若IERatio = 0x0000|(200) = 0x00C8，则 I : E = 1 : 2.00
*/
int GetIERatio(VENT_TIME _VentTime) //  20130716
{
	int IERatio;
	//    IERatio = (float)_VentTime.InspTime / _VentTime.ExpTime;
	if(_VentTime.InspTime > _VentTime.ExpTime)
	{
		IERatio = 0x8000;
		IERatio |= _VentTime.InspTime * 100 / _VentTime.ExpTime;
	}
	else
	{
		IERatio = 0;
		IERatio |= _VentTime.ExpTime * 100 / _VentTime.InspTime;
	}
	return (IERatio);
}

/***************************************************************************************************
*
*
*/

void InitBarometer()//djq add 20140305 初始化大气压力传感器
{
	Barameter_Switch = RTEEPROMread(BARO_SWITCH_ADDR);  //读取开关
	if(Barameter_Switch)  //大气压力传感器关,则读取保存值为设置值，并且下发数据
	{
		Barameter_Pressure = (RTEEPROMread(BARO_DATA_HIGH_ADDR)<<8) | RTEEPROMread(BARO_DATA_LOW_ADDR);
		SendOperationMessage(BAROMETER_SEND,USART0);//将读取的设置值发送到BDU

	}
}
int _GetRand(int cnt)
{
	while(cnt--)
	{
		rand();
	}
	return rand();
}
/**************************************************************************
GetDeviceSerial:存储一个8byte的设备序列号分别存储到Serial和EEPROM中
Serial:得到的8byte序列号保存到该数组中
EEPROM_Addrint:8byte序列号存储到EEPROM的首地址
year：month：day：hour：min：sec：设备时间
*/
#if 0
void GetDeviceSerial(char *Serial,
					 unsigned int EEPROM_Addrint,unsigned int EEPROM_AddrBack,
					 int year,int month,int day,int hour,int min,int sec)
{
	unsigned int seed = year + month + day*(unsigned long)86400 + hour*3600 + min*60 + sec;
	int index = 0;
	srand(seed);

	for(index = 0;index < 8;index++)
	{
		Serial[index] = _GetRand(RTEEPROMread(ALARM_BATTERY_ADDR + index)%100) % 0xF;   //使用电池电量
	}
	for(index = 0;index < 8;index++)
	{
		RTEEPROMwrite(EEPROM_Addrint+index,Serial[index]);
		RTEEPROMwrite(EEPROM_AddrBack+index,Serial[index]);
	}
}
#endif

void SettingDefaultConfig(void)
{
	int index = 0;
	//mode
	Mode_Chose_Flag = 0;//1:full  0：simple

	//FiO2 Sensior
	FiO2_Config_On = 0;// FiO2选配

	//PRVC mode
	PRVCMode_Chose_Flag = 0;
	//驱动气体开关
	DriverGas_Chose_Flag = 0;//0:off  1:On  djq add 20170609
	//Vt
	VT_Min_Flag = 0;//0: 50    1:20
	//SpO2
	SpO2_Config_On = 0;  //0:关闭   1：开启
	Masimo_SpO2_Config_On = 0; //0:关闭   1：开启
	//CO2
	EtCO2_Config_On = 0; //0:关闭   1：开启
	EtCO2_KM7002_Config_On = 0;
	Masimo_EtCO2_Config_On = 0; //0:关闭   1：开启
	MasimoISA_EtCO2_Config_On = 0; //0:关闭   1：ISA  2:ISA + O2
	Drager_EtCO2_Config_On = 0;
	for(index = 0;index < 8;index++)
	{
		RTEEPROMwrite(OPTIONAL_0_ADDR+index,0);//清除系统配置信息
	}
}
//从EEPROM中读取选配模式的状态
void Read_ConfigMode(void)
{
	int Index = 0;
	char SoftwareSerial[8] = {0};

	for(Index = 0;Index < 8;Index++)
	{
		DeviceSerial_Table[Index] = (DeviceSerial>>(4*(7-Index))) & 0xF;
	}
#if 0
	//如果EEPROM中没有序列号，则读取备份选配信息
	if((RTEEPROMread(DEVICE_FLAG_HIGH_ADDR) != OPTIONAL_PASSWD_HEAD)
		|| (RTEEPROMread(DEVICE_FLAG_LOW_ADDR)  != OPTIONAL_PASSWD_TAIL))
	{
		//如果备份EEPROM中没有序列号，则生成随机数
		if((RTEEPROMread(DEVICE_FLAG_HIGH_ADDR_BACK) != OPTIONAL_PASSWD_HEAD)
			|| (RTEEPROMread(DEVICE_FLAG_LOW_ADDR_BACK)  != OPTIONAL_PASSWD_TAIL))
		{
			//获取序列号
			GetDeviceSerial(DeviceSerial_Table,
				DEVICE_NUM_0_ADDR,DEVICE_NUM_0_ADDR_BACK,
				RTCTime.RTC_Year,RTCTime.RTC_Mon,
				RTCTime.RTC_Mday,RTCTime.RTC_Hour,
				RTCTime.RTC_Min,RTCTime.RTC_Sec);
			RTEEPROMwrite(DEVICE_FLAG_HIGH_ADDR,OPTIONAL_PASSWD_HEAD);
			RTEEPROMwrite(DEVICE_FLAG_LOW_ADDR,OPTIONAL_PASSWD_TAIL);

			RTEEPROMwrite(DEVICE_FLAG_HIGH_ADDR_BACK,OPTIONAL_PASSWD_HEAD);
			RTEEPROMwrite(DEVICE_FLAG_LOW_ADDR_BACK,OPTIONAL_PASSWD_TAIL);
			//重新初始化系统配置
			SettingDefaultConfig();
		}
		else//如果备份地址可用，则把备份地址复制到正常地址中
		{
			for(Index = 0;Index < 8;Index++)//如果正常的选配除了问题，则把备份的选配写入到正常的选配中
			{
				RTEEPROMwrite(DEVICE_NUM_0_ADDR + Index,RTEEPROMread(DEVICE_NUM_0_ADDR_BACK + Index));
			}
			for(Index = 0;Index < 8;Index++)//读取备份的序列号
			{
				DeviceSerial_Table[Index] = RTEEPROMread(DEVICE_NUM_0_ADDR_BACK+Index);
			}
		}
	}
	else  //如果EEPROM中已经存在序列号，那么读取序列号
	{
		if((RTEEPROMread(DEVICE_FLAG_HIGH_ADDR_BACK) != OPTIONAL_PASSWD_HEAD)
			|| (RTEEPROMread(DEVICE_FLAG_LOW_ADDR_BACK)  != OPTIONAL_PASSWD_TAIL))

		{
			for(Index = 0;Index < 8;Index++)//如果备份不可用，则把正常的地址写入到备份
			{
				RTEEPROMwrite(DEVICE_NUM_0_ADDR_BACK + Index,RTEEPROMread(DEVICE_NUM_0_ADDR + Index));
			}
			RTEEPROMwrite(DEVICE_FLAG_HIGH_ADDR_BACK,OPTIONAL_PASSWD_HEAD);
			RTEEPROMwrite(DEVICE_FLAG_LOW_ADDR_BACK,OPTIONAL_PASSWD_TAIL);
		}
		for(Index = 0;Index < 8;Index++)
		{
			DeviceSerial_Table[Index] = RTEEPROMread(DEVICE_NUM_0_ADDR+Index);
		}
	}
#endif
	//软件授权解析
	//将芯片ID号码转换为数组
	for(Index = 0;Index < 8;Index++)
	{
		DeviceSerial_String[Index] = HexTable[DeviceSerial_Table[Index] & 0xF];
	}
	DeviceSerial_String[Index]='\0';
	//解析软件配置
	for(Index = 0;Index < 8;Index++)
	{
		SoftwareSerial[Index] = RTEEPROMread(OPTIONAL_0_ADDR+Index);
	}
	if(!GetOptional_SetFlag(SoftwareSerial,DeviceSerial_Table))//如果正常的地址不能用，则启用备份地址
	{
		for(Index = 0;Index < 8;Index++)
		{
			SoftwareSerial[Index] = RTEEPROMread(OPTIONAL_0_ADDR_BACK+Index);
		}
		if(GetOptional_SetFlag(SoftwareSerial,DeviceSerial_Table))//如果备份地址可用，则把备份地址复制都
		{
			//将备份地址中的内容复制到正常地址中去
			for(Index = 0;Index < 8;Index++)
			{
				RTEEPROMwrite(OPTIONAL_0_ADDR+Index,RTEEPROMread(OPTIONAL_0_ADDR_BACK+Index));
			}
		}
	}
	else
	{
		//将选配码写入到备份地址中
		for(Index = 0;Index < 8;Index++)
		{
			RTEEPROMwrite(OPTIONAL_0_ADDR_BACK+Index,RTEEPROMread(OPTIONAL_0_ADDR+Index));
		}
	}
	//大气压力传感器
	InitBarometer();
	SpO2_Chose();
}

void Battery_Data_Init(void)
{

	Rd_Threth_Alarm_Battery();
}
//初始化病人信息//tcc add 20220525
void InitPatientInfo(Patient_Info_t *Patient)
{
    unsigned int tmp;
    unsigned char ch1,ch2;

    Patient->Patient_Type = PATIENTTYPE_ADULT;
    Patient->Patient_Gender = 0;
    Patient->Patient_VentType = 0;
    Patient->Patient_IBW = 71;
    Patient->Patient_Stature = 175;
    Patient->Patient_Age = 32;
    FlowLowLimit = 10000;
	FlowHighLimit = 1500;
}
/***************************************************************************************************
*
* 主流ETCO2模块获取各种气体数值
*/
void irma_getpara(void)
{
	//tcc add 20220524 MAC计算值添加年龄和大气压补偿
	unsigned char age = Patient_GetAge();
	unsigned int  Mac_Ratio = 0;
	float tmp_MAC = 0;
	u16 tmp_pressure = 0;
	u32 tmp = 0;
	static char zero_state=0,span_state=0;

        if(Patient_GetPType() == PATIENTTYPE_INFANT)//tcc add 20220531 增加婴儿年龄的判断，不然实际会计算会是月份计算MAC值
        {
            if(age >12) age = 2;
            else age = 1;
        }
        else
        {
            age = age;
        }
	if(AA_RX_Process != NULL)
	{
		if(AA_RX_Process() == IM_OK)
		{
			if(AA_Module_Flag.EtCO2ModuleExist == 2)
			{
				if(Drager_EtCO2_Config_On && AA_all_data.AA_state.dev_info.AA_name == Drager)
				{
					//if(etco2_switch)
					{
						if((AA_all_data.AA_state.dev_info.AA_name == Drager) &&
							(AA_all_data.AA_state.AA_ModuleState.currect_mode == AA_MODE_STANDBY))
						{
							SendOperationMessage(SET_AA_OPERATE_MEASURE,USART2);
						}
						AA_Module_Monitor();
						AA_Module_String();
						AA_Module_Alarm_Progress();

						if(AA_TASK_REGISTER)
						{
							USART2_TXD_RIGSTER |= AA_TASK_REGISTER;
							if(USART2_TXD_Busy_FLAG==FALSE)
							{
								Inquire_TXD_Handler(USART2);
							}
						}
					}
				}
				else //PhaseIn下直接监测
				{
					AA_Module_Monitor();
					AA_Module_String();
					AA_Module_Alarm_Progress();

					if(AA_TASK_REGISTER)
					{
						USART2_TXD_RIGSTER |= AA_TASK_REGISTER;
						if(USART2_TXD_Busy_FLAG==FALSE)
						{
							Inquire_TXD_Handler(USART2);
						}
					}
					if ( AA_all_data.AA_state.AA_ModuleState.zero_state != zero_state
						|| AA_all_data.AA_state.AA_ModuleState.span_state != span_state)
					{
						if (MenuType == OXYGEN_SENSOR_CAL_MENU)
						{
							if (WM_IsWindow(Menu_hWin.RecordWin))
							{
								WM_SendMessageNoPara(Menu_hWin.RecordWin, WM_INIT_DIALOG);
								span_state = AA_all_data.AA_state.AA_ModuleState.span_state;
								zero_state = AA_all_data.AA_state.AA_ModuleState.zero_state;
							}
						}
					}
				}
			}
			if(EtCO2ModuleOff)
			{
				if(AA_all_data.AA_state.dev_info.AA_name == PhaseIn)
				{
					if(ETCO2Unit == 0)
					{
						CO2_Parameters.insCO2   = Change_Unit(0,0,(float)AA_all_data.AA_detail.AA_CO2.FI);
						CO2_Parameters.etCO2    = Change_Unit(0,0,(float)AA_all_data.AA_detail.AA_CO2.ET);
						CO2_Parameters.WaveData = Change_Unit(0,0,(float)(AA_all_data.AA_fast.AA_RT_data.CO2value)/100);
					}
					else if(ETCO2Unit == 2)//Kpa
					{
						CO2_Parameters.insCO2   = Change_Unit(2,0,(float)(AA_all_data.AA_detail.AA_CO2.FI)*AA_all_data.AA_detail.AA_Pressure.atmp/1000);
						CO2_Parameters.etCO2    = Change_Unit(2,0,(float)(AA_all_data.AA_detail.AA_CO2.ET)*AA_all_data.AA_detail.AA_Pressure.atmp/1000);
						CO2_Parameters.WaveData = Change_Unit(2,0,(float)(AA_all_data.AA_fast.AA_RT_data.CO2value)/100);
					}
					else//mmHg
					{
						CO2_Parameters.insCO2   = Change_Unit(1,0,(float)(AA_all_data.AA_detail.AA_CO2.FI)*AA_all_data.AA_detail.AA_Pressure.atmp/10000);
						CO2_Parameters.etCO2    = Change_Unit(1,0,(float)(AA_all_data.AA_detail.AA_CO2.ET)*AA_all_data.AA_detail.AA_Pressure.atmp/10000);
						CO2_Parameters.WaveData = Change_Unit(1,0,(float)(AA_all_data.AA_fast.AA_RT_data.CO2value * AA_all_data.AA_detail.AA_Pressure.atmp/1013)/100);
					}
					//djq add 20170523增加永远为mmhg单位的EtCO2参数。
					CO2_Parameters.etCO2_mmhg  = Change_Unit(1,0,(float)(AA_all_data.AA_detail.AA_CO2.ET)*AA_all_data.AA_detail.AA_Pressure.atmp/10000);
					CO2_Parameters.insCO2_mmhg = Change_Unit(1,0,(float)(AA_all_data.AA_detail.AA_CO2.FI)*AA_all_data.AA_detail.AA_Pressure.atmp/10000);
				}
				else if(AA_all_data.AA_state.dev_info.AA_name == Drager)//AA_all_data.AA_detail.AA_CO2.Unit 0:数据源默认为%
					//1:默认数据源为mmHg
				{
					if(AA_all_data.AA_detail.AA_CO2.Unit == PSUS_Default)//ATS 气体单位% 精度0.01
					{
						if(ETCO2Unit != 1)//不需要转换
						{
							CO2_Parameters.insCO2   = (float)AA_all_data.AA_detail.AA_CO2.FI/10.0;
							CO2_Parameters.etCO2    = (float)AA_all_data.AA_detail.AA_CO2.ET/10.0;
							CO2_Parameters.WaveData = (float)AA_all_data.AA_fast.AA_RT_data.CO2value/100.0;
						}
						else
						{
							CO2_Parameters.insCO2   = Change_Unit(ETCO2Unit,0,(float)(AA_all_data.AA_detail.AA_CO2.FI)/100.0);
							CO2_Parameters.etCO2    = Change_Unit(ETCO2Unit,0,(float)(AA_all_data.AA_detail.AA_CO2.ET)/100.0);
							CO2_Parameters.WaveData = Change_Unit(ETCO2Unit,0,(float)(AA_all_data.AA_fast.AA_RT_data.CO2value)/100.0);
						}
						//djq add 20170523增加永远为mmhg单位的EtCO2参数。
						CO2_Parameters.etCO2_mmhg  = Change_Unit(1,0,(float)(AA_all_data.AA_detail.AA_CO2.ET)/100.0);
						CO2_Parameters.insCO2_mmhg = Change_Unit(1,0,(float)(AA_all_data.AA_detail.AA_CO2.FI)/100.0);
					}
					else//ATPS 气体单位mmHg 精度0.1
					{
						if(ETCO2Unit == 1)//不需要转换
						{
							CO2_Parameters.insCO2   = (float)AA_all_data.AA_detail.AA_CO2.FI/10.0;
							CO2_Parameters.etCO2    = (float)AA_all_data.AA_detail.AA_CO2.ET/10.0;
							CO2_Parameters.WaveData = (float)AA_all_data.AA_fast.AA_RT_data.CO2value/10.0;
						}
						else
						{
							CO2_Parameters.insCO2   = Change_Unit(ETCO2Unit,1,(float)AA_all_data.AA_detail.AA_CO2.FI);
							CO2_Parameters.etCO2    = Change_Unit(ETCO2Unit,1,(float)AA_all_data.AA_detail.AA_CO2.ET);
							CO2_Parameters.WaveData = Change_Unit(ETCO2Unit,1,(float)AA_all_data.AA_fast.AA_RT_data.CO2value/10.0);
						}
						//djq add 20170523增加永远为mmhg单位的EtCO2参数。
						CO2_Parameters.etCO2_mmhg  = (float)AA_all_data.AA_detail.AA_CO2.ET/10.0;
						CO2_Parameters.insCO2_mmhg = (float)AA_all_data.AA_detail.AA_CO2.FI/10.0;
					}
				}
				//djq add 20170325 如果开启了Drager+顺磁氧，则使用顺磁氧的氧浓度
				if((Drager_EtCO2_Config_On == 2) ||(MasimoISA_EtCO2_Config_On == 2))
				{
					MonitorPara.FiO2Value = AA_all_data.AA_detail.AA_O2.FI;
				}
				//*//cpp add 20130712
				AA_Para.EtN2O = AA_all_data.AA_detail.AA_N2O.ET;
				AA_Para.InN2O = AA_all_data.AA_detail.AA_N2O.FI;
				AA_Para.EtAX1 = AA_all_data.AA_detail.AA_AA1.ET;
				AA_Para.InAX1 = AA_all_data.AA_detail.AA_AA1.FI;
				AA_Para.EtAX2 = AA_all_data.AA_detail.AA_AA2.ET;
				AA_Para.InAX2 = AA_all_data.AA_detail.AA_AA2.FI;

				/*
				MAC经年龄校准 = MAC * pow(10, (-0.00269*(PatientInfo->Patient_Age-40)));//适用于1岁以上的患者
				*/
				//tcc add 20220524 MAC计算值添加年龄和大气压补偿
				if(age > 80) age = 80;
				if(age <  1) age = 1;
				Mac_Ratio = MAC_Ratio[age];

				//djq modify 20170522 修改MAC   简化的计算公式 EtAA1/MacBaseAA1 + EtAA2/MacBaseAA2 + N2O/10
				if(!N2O_Switch)
				{
					tmp = AA_all_data.AA_detail.AA_N2O.ET*100*10000;
					tmp_MAC = tmp/(105.0*Mac_Ratio);
					//AA_Para.MAC = (AA_all_data.AA_detail.AA_N2O.ET*10*10)/105;
				}
				else
				{
					tmp_MAC = 0;
					//AA_Para.MAC = 0;
				}
				if(AA_all_data.AA_detail.AA_AA1.ID)
				{
					tmp = AA_all_data.AA_detail.AA_AA1.ET*10*10000;
					tmp_MAC +=  tmp/ (MAC_Base[AA_all_data.AA_detail.AA_AA1.ID]*Mac_Ratio*1.0);

					//AA_Para.MAC += (float)(AA_all_data.AA_detail.AA_AA1.ET*10) / MAC_Base[AA_all_data.AA_detail.AA_AA1.ID];
				}
				if(AA_all_data.AA_detail.AA_AA2.ID)
				{
					tmp = AA_all_data.AA_detail.AA_AA2.ET*10*10000;
					tmp_MAC +=  tmp/ (MAC_Base[AA_all_data.AA_detail.AA_AA2.ID]*Mac_Ratio*1.0);

					//AA_Para.MAC += (float)(AA_all_data.AA_detail.AA_AA2.ET*10) / MAC_Base[AA_all_data.AA_detail.AA_AA2.ID];
				}

				if(Barameter_Pressure >= 300 && Barameter_Pressure <= 999)
				{
					tmp_pressure = Barameter_Pressure;
				}
				else
				{
					tmp_pressure = 760;
				}
				tmp_pressure = 760;//暂时不添加大气压补偿，测试后添加
				tmp_MAC *= tmp_pressure/760.0;

				AA_Para.MAC = (int)tmp_MAC;

				AA_Para.N2OWaveData = AA_all_data.AA_fast.AA_RT_data.N2Ovalue;
				AA_Para.AX1WavaData = AA_all_data.AA_fast.AA_RT_data.AA1value;
				AA_Para.AX2WaveData = AA_all_data.AA_fast.AA_RT_data.AA2value;
				//*///
			}
		}
	}
}

void CO2_UnitConvert(int Unit)
{
	switch(Unit)
	{
	case 0://%
		strcpy(CO2_Disp.co2_unit,"%");
		// 		strcpy(CO2_Disp.co2_max,"13.3");
		// 		CO2_Disp.co2max  = 13.3;
		// 		CO2_Disp.co2step = 2.66;

		//  [2/15/2022 yaoyao modify]CO2添加量程变换，变换中的最小量程
		//  		strcpy(CO2_Disp.co2_max,"15");
		//  		CO2_Disp.co2max  = 15;
		//  		CO2_Disp.co2step = 3;
		strcpy(CO2_Disp.co2_max,"5");
		CO2_Disp.co2max  = 5;
		CO2_Disp.co2step = 1;
		break;
	case 1://mmHg
		strcpy(CO2_Disp.co2_unit,"mmHg");//H大写
		//  [2/15/2022 yaoyao modify]CO2添加量程变换，变换中的最小量程
		// 		strcpy(CO2_Disp.co2_max,"113");
		// 		CO2_Disp.co2max  = 113;
		// 		CO2_Disp.co2step = 22.6;
		strcpy(CO2_Disp.co2_max,"37.5");
		CO2_Disp.co2max  = 37.5;
		CO2_Disp.co2step = 7.5;
		break;
	case 2://kPa
		strcpy(CO2_Disp.co2_unit,"kpa");
		// 		strcpy(CO2_Disp.co2_max,"13.3");
		// 		CO2_Disp.co2max  = 13.3;
		// 		CO2_Disp.co2step = 2.66;

		//  [2/15/2022 yaoyao modify]CO2添加量程变换，变换中的最小量程
		// 		strcpy(CO2_Disp.co2_max,"15");
		// 		CO2_Disp.co2max  = 15;
		// 		CO2_Disp.co2step = 3;
		strcpy(CO2_Disp.co2_max,"5");
		CO2_Disp.co2max  = 5;
		CO2_Disp.co2step = 1;
		break;
	}
}

void GasModule_InitConfig(void)
{
	if(MasimoISA_EtCO2_Config_On && (AA_Module_Flag.EtCO2ModuleExist == 2)&&(AA_Module_Flag.PhaseInTypeModuleExist==2))//
	{  
		//Note: 调试下进入零点校验或21/100校验会关泵 此处判断模块第一次开机
		if((!PhaseInISA_Init.PhaseInISA_Flag) && PhaseInISA_Init.PhaseInISA_State)//初始化完毕 && 计时标志
		{
			if( AA_all_data.AA_state.AA_ModuleState.zero_state != disable \
				&& AA_all_data.AA_state.AA_ModuleState.zero_state != zero_zeroing \
				&& AA_all_data.AA_state.AA_ModuleState.span_state != SPAN_InProg) //可以校零//不在校零过程中//不在量程校准过程中
			{
				PhaseInISA_Init.PhaseInISA_Flag = 1;//模块初始化完毕
				PhaseInISA_Init.PhaseInISA_State = 0;
				if(StandbyState == 0xFF)//待机界面
				{
					SendOperationMessage(SET_AA_PUMPOFF,USART2);//关泵
				}
				else 
				{
					if(!EtCO2ModuleOff)//AA开关关
					{
						SendOperationMessage(SET_AA_PUMPOFF,USART2);//关泵
					}
				}
			}
		}
	}
	else if(EtCO2_Config_On || EtCO2_KM7002_Config_On)
	{
		if((ChinaCO2_Init.ChinaCo2_Flag&0x02) == 0x02)//开机15s如果没按工作，则CO2转入待机
		{
			ChinaCO2_Init.ChinaCo2_Flag &= ~0x02;
			if(ChinaCO2_Init.ChinaCo2_State != 1)//没进入工作
			{
				SendOperationMessage(CO2_MODULEOFF,USART2);//关泵
			}
		}
	}
}

/*
**************************************************************************
*函数名称：Change_Unit
*输入参数：unsigned char unit:0:% 1:mmHg 2:kPa
float para
*输出参数：int,kPa与%扩大10倍
*函数功能：参数单位转换
**************************************************************************
*/
float Change_Unit(unsigned char unit_dest,unsigned char unit_src,float para)
{
	float temp = 0;
	if(unit_src == unit_dest)
	{
		return para;
	}
	else
	{
		switch(unit_dest)
		{
		case 0:     //%
			if(unit_src == 1)
			{
				temp = para / 7.5;
			}
			else if(unit_src == 2)
			{
				//近似不变
				temp = para;
			}
			break;
		case 1:     //mmHg
			if(unit_src == 0)
			{
				temp = para * 7.5;
			}
			else if(unit_src == 2)
			{
				temp = para * 7.5;
			}
			break;
		case 2:     //kPa
			if(unit_src == 0)
			{
				//近似不变
				temp = para;
			}
			else if(unit_src == 1)
			{
				temp = para / 7.5;
			}
			break;
		default:
			break;
		}
		return temp;
	}
}
//
void BreathSettingParameterWrite(u8 InitFlag,const BREATH_SET_PARA *OldPara,const BREATH_SET_PARA *NewPara)
{
	//通气模式
	if((InitFlag)||(OldPara->BreathMode != NewPara->BreathMode))
	{
		if((OldPara->BreathMode != MANUAL_MODE)&&(OldPara->BreathMode != ACGO_MODE))
		{
			RTEEPROMwrite(BREATH_BREATHMODE_ADDR,NewPara->BreathMode);
		}
		else
		{
			RTEEPROMwrite(BREATH_BREATHMODE_ADDR,BreathMode_BeforeACGO);
		}
	}
	if((InitFlag)||(OldPara->ApnoeaMode != NewPara->ApnoeaMode))
	{
		RTEEPROMwrite(BREATH_APNOEA_ADDR,NewPara->ApnoeaMode);
	}
	if((InitFlag)||(OldPara->TriggerMode != NewPara->TriggerMode))
	{
		RTEEPROMwrite(BREATH_TRIGGERMODE_ADDR,NewPara->TriggerMode);
	}
	if((InitFlag)||(OldPara->PreBreathMode != NewPara->PreBreathMode))
	{
		RTEEPROMwrite(BREATH_PRE_BREATHMODE_ADDR,NewPara->PreBreathMode);  //上一个使用的呼吸模式
	}
	if((InitFlag)||(OldPara->PreApneaMode != NewPara->PreApneaMode))       //上一个使用的窒息模式
	{
		RTEEPROMwrite(BREATH_PRE_APNOEA_ADDR,NewPara->PreApneaMode);
	}
	//Vt默认值
	if((InitFlag)||(OldPara->SettingVtValue != NewPara->SettingVtValue))
	{
		RTEEPROMwrite(BREATH_VT_HIGH_ADDR,(NewPara->SettingVtValue>>8)&0xff);
		RTEEPROMwrite(BREATH_VT_LOW_ADDR,NewPara->SettingVtValue&0xff);
	}
	//触发值
	if((InitFlag)||(OldPara->SettingFtrValue != NewPara->SettingFtrValue))
	{
		RTEEPROMwrite(BREATH_FTR_ADDR,NewPara->SettingFtrValue);
	}
	if((InitFlag)||(OldPara->SettingPtrValue != NewPara->SettingPtrValue))
	{
		RTEEPROMwrite(BREATH_PTR_ADDR,NewPara->SettingPtrValue);
	}
	//PEEP
	if((InitFlag)||(OldPara->SettingPEEPValue != NewPara->SettingPEEPValue))
	{
		RTEEPROMwrite(BREATH_PEEP_ADDR,NewPara->SettingPEEPValue);
	}
	//Pinsp
	if((InitFlag)||(OldPara->SettingPinspValue != NewPara->SettingPinspValue))
	{
		RTEEPROMwrite(BREATH_PINSP_ADDR,NewPara->SettingPinspValue);
	}
	//Psuppport
	if((InitFlag)||(OldPara->SettingPsupportValue != NewPara->SettingPsupportValue))
	{
		RTEEPROMwrite(BREATH_PS_ADDR,NewPara->SettingPsupportValue);
	}
	//频率
	if((InitFlag)||(OldPara->SettingRateValue != NewPara->SettingRateValue))
	{
		RTEEPROMwrite(BREATH_RATE_ADDR,NewPara->SettingRateValue);
	}
	if((InitFlag)||(OldPara->SettingSimvRateValue != NewPara->SettingSimvRateValue))
	{
		RTEEPROMwrite(BREATH_RATE_SIMV_ADDR,NewPara->SettingSimvRateValue);
	}
	if((InitFlag)||(OldPara->SettingotherRateValue != NewPara->SettingotherRateValue))
	{
		RTEEPROMwrite(BREATH_RATE_OTHER_ADDR,NewPara->SettingotherRateValue);
	}
	//Ti
	if((InitFlag)||(OldPara->SettingTiValue != NewPara->SettingTiValue))
	{
		RTEEPROMwrite(BREATH_Ti_HIGH_ADDR,(NewPara->SettingTiValue>>8)&0xff);
		RTEEPROMwrite(BREATH_Ti_LOW_ADDR,NewPara->SettingTiValue&0xff);
	}
	//Tp
	if((InitFlag)||(OldPara->SettingTpValue != NewPara->SettingTpValue))
	{
		RTEEPROMwrite(BREATH_TP_ADDR,NewPara->SettingTpValue);
	}
	//Sigh
	if((InitFlag)||(OldPara->SettingSighTimesValue != NewPara->SettingSighTimesValue))
	{
		RTEEPROMwrite(BREATH_SIGHTIMES_ADDR,NewPara->SettingSighTimesValue);
	}
	//Tslope
	if((InitFlag)||(OldPara->SettingTslopeValue != NewPara->SettingTslopeValue))
	{
		RTEEPROMwrite(BREATH_TSLOPE_ADDR,NewPara->SettingTslopeValue);
	}
	//I:E
	if((InitFlag)||(OldPara->SettingIEValue != NewPara->SettingIEValue))
	{
		RTEEPROMwrite(BREATH_IE_ADDR,NewPara->SettingIEValue);
	}
	//Esens
	if((InitFlag)||(OldPara->EsenseValue != NewPara->EsenseValue))
	{
		RTEEPROMwrite(BREATH_ESENS_ADDR,NewPara->EsenseValue);
	}
	//Vtsupport
	//if((InitFlag)||(OldPara->SettingVtSupportValue != NewPara->SettingVtSupportValue))
	//{
	//	RTEEPROMwrite(BREATH_VTS_HIGH_ADDR,(NewPara->SettingVtSupportValue>>8)&0xff);
	//	RTEEPROMwrite(BREATH_VTS_LOW_ADDR,NewPara->SettingVtSupportValue&0xff);
	//}
}
/*********************************************************************
*   根据病人设置潮气量的调节范围
*   在系统初始化、更新病人和开机时初始化
*   在改变了潮气量选配后也要调用
*/
void SetVtValue_Limit(Patient_Info_t *Patient, int mode)
{
	if(Patient_GetPType() == PATIENTTYPE_PAEDIATRIC)//pead
	{
		Vt_HighLimit = VTH_PADI_LIMIT;
		Vt_LowLimit = VTL_PADI_LIMIT;
		cbdialog32string[VT_10] = Sys_Lib_GetDispStr(DP_CHILVTMORE50);
	}
	else if(Patient_GetPType() == PATIENTTYPE_INFANT)//infant
	{
		Vt_HighLimit = VTH_INFANT_LIMIT;
		if(VT_Min_Flag && (mode == PRVC_MODE || mode == SIMV_PRVC_MODE || mode == VS_MODE))//5ml选配潮气量
		{
			Vt_LowLimit = VTL_INFANT_LIMIT_5;
			cbdialog32string[VT_10] = Sys_Lib_GetDispStr(DP_INFANTVTMORE5);
		}
		else
		{
			Vt_LowLimit = VTL_INFANT_LIMIT;
			cbdialog32string[VT_10] = Sys_Lib_GetDispStr(DP_INFANTVTMORE10);
		}
	}
	else//adult
	{
		Vt_HighLimit = VTH_ADULT_LIMIT;
		Vt_LowLimit = VTL_ADULT_LIMIT;
		cbdialog32string[VT_10] = Sys_Lib_GetDispStr(DP_ADULTVTMORE100);
	}
}
//系统第一次初始化或者重新选择病人后，保存参数和病人信息
void InitPatient_SettingParameter(BREATH_SET_PARA * Para)
{
	int   tmp_I = 0;
	unsigned long long VtValue = 0;
	float tmp_F = 0;
	//固定不变的参数
	Para->SettingIEValue = 8;
	Para->SettingFiO2Value = 40;
	Para->SettingPinspValue = 15;
	Para->SettingPEEPValue = 0;//3
	Para->SettingPsupportValue = 15;
//#if(LOGO_FLAG != HOSPITECH_LOGO)
//	Para->SettingPtrValue = 3;
//#else
	Para->SettingPtrValue = -3;
//#endif
	Para->SettingFtrValue = 30;
	Para->SettingTpValue = 0;
	Para->SettingSighTimesValue = 100;
	Para->EsenseValue = 25;//5
	Para->PreBreathMode = 0;  //上一个使用的呼吸模式
	Para->PreApneaMode = 0;   //上一个使用的呼吸模式
	Para->SettingPhighValue = 15;
	Para->SettingPlowValue = 5;
	Para->SettingPHLimitValue = 40;

	//重新设置潮气量的上下限
	SetVtValue_Limit(Patient_GetInfo(),Para->BreathMode);

	//随Patient信息变化的参数
	//通气模式
	if(Patient_GetPType() == PATIENTTYPE_INFANT)
	{
		if((Para->BreathMode == MANUAL_MODE) || (Para->BreathMode == ACGO_MODE))
		{
			BreathMode_BeforeACGO = PCV_MODE;
			BreathMode_BeforeConfirm = BreathMode_BeforeACGO;
		}
		else
		{
			Para->BreathMode=PCV_MODE;
		}
		Para->ApnoeaMode = PCV_MODE;
		// 		Para->SettingFlowPattem = SQUARE_FLOWPATTEN;//波形类型    SQUARE_FLOWPATTEN:方波    RAMP_FLOWPATTEN:减速波
		Para->SettingRateValue = 30;
		Para->SettingotherRateValue = 30;
		Para->SettingSimvRateValue = 15;//20170621
		Para->SettingVSRateValue = 12;
	}
	else
	{
		if((Para->BreathMode == MANUAL_MODE) || (Para->BreathMode == ACGO_MODE))
		{
			BreathMode_BeforeACGO = VCV_MODE;
			BreathMode_BeforeConfirm = BreathMode_BeforeACGO;
		}
		else
		{
			Para->BreathMode=VCV_MODE;
		}
		Para->ApnoeaMode = VCV_MODE;
		// 		Para->SettingFlowPattem = SQUARE_FLOWPATTEN;//波形类型    SQUARE_FLOWPATTEN:方波    RAMP_FLOWPATTEN:减速波
		if(Patient_GetPType() == PATIENTTYPE_PAEDIATRIC)
		{
			Para->SettingRateValue = 20;
			Para->SettingotherRateValue = 20;
			Para->SettingSimvRateValue = 10;//20170621
			Para->SettingVSRateValue = 8;

		}
		else //PATIENTTYPE_ADULT
		{
			Para->SettingRateValue = 12;
			Para->SettingotherRateValue = 12;
			Para->SettingSimvRateValue = 6;//20170621
			Para->SettingVSRateValue = 4;

		}
	}
	//BreathMode_BeforeACGO = Para->BreathMode;
	//Vt默认值,根据IBW计算潮气量   换算系数7.25
	//潮气量计算

	tmp_F = Patient_GetIBW()*((Patient_GetPType() == PATIENTTYPE_INFANT )?0.725:7.25);
// 	tmp_F = Patient_GetIBW()*7.25;
	tmp_I = tmp_F;  //整数部分  tmp_F-tmp_I小数部分   tmp_I%10余数部分
	if(tmp_I > 100)
	{
		if((tmp_F-tmp_I)+(tmp_I%10)>=5)
		{
			Para->SettingVtValue = tmp_I - (tmp_I%10) + 10;
		}
		else
		{
			Para->SettingVtValue = tmp_I - (tmp_I%10);
		}
	}
	else
	{
		if(VT_Min_Flag == 1)//vt min 20
		{
			if(tmp_I > 20)
			{
				if((tmp_F-tmp_I)+(tmp_I%5) >= 2.5)
				{
					Para->SettingVtValue = tmp_I - (tmp_I%5) + 5;
				}
				else
				{
					Para->SettingVtValue = tmp_I - (tmp_I%5);
				}
			}
			else
			{
				Para->SettingVtValue = 20;
			}
		}
		else//vt min 50
		{
			if(tmp_I > 50)
			{
				if((tmp_F-tmp_I)+(tmp_I%5) >= 2.5)
				{
					Para->SettingVtValue = tmp_I - (tmp_I%5) + 5;
				}
				else
				{
					Para->SettingVtValue = tmp_I - (tmp_I%5);
				}
			}
			else
			{
				Para->SettingVtValue = 50;
			}
		}
	}
	if(Patient_GetPType() == PATIENTTYPE_ADULT)//adult
	{
		Para->SettingThighValue = 20;
		Para->SettingTlowValue = 40;
	}
	else if(Patient_GetPType() == PATIENTTYPE_PAEDIATRIC)//pead
	{
		Para->SettingThighValue = 10;
		Para->SettingTlowValue = 20;
	}
	else if(Patient_GetPType() == PATIENTTYPE_INFANT)//infant
	{
		Para->SettingThighValue = 5;
		Para->SettingTlowValue = 10;
	}
	if(Para->SettingVtValue > Vt_HighLimit)
	{
		Para->SettingVtValue = Vt_HighLimit;
	}
	else if(Para->SettingVtValue < Vt_LowLimit)
	{
		Para->SettingVtValue = Vt_LowLimit;
	}
	Para->SettingVtSupportValue = Para->SettingVtValue;

	Para->SettingTiValue = IEConvertTi(0,Para->SettingIEValue,Para);
	//根据当前频率和I:E计算出Ti，使用Ti计算出平均流速，反推频率
	VtValue = Para->SettingVtValue*10000/Para->SettingTiValue;
	if((Patient_GetPType() != PATIENTTYPE_INFANT)&&(VtValue < FlowLowLimit))//FLOW_LIMIT
	{
		tmp_F = 20.0*FlowLowLimit/100/Para->SettingVtValue;
		tmp_I = tmp_F;
		if(tmp_F - tmp_I > 0.01)
		{
			tmp_I += 1;//频率有余数的时候，f↑，Ti↓，flow↑
		}
		Para->SettingRateValue = tmp_I;
		Para->SettingotherRateValue = tmp_I;
		Para->SettingTiValue = IEConvertTi(0,Para->SettingIEValue,Para);
	}
	if((Patient_GetPType() != PATIENTTYPE_INFANT)&&(VtValue/100 > FlowHighLimit))//FLOW_LIMIT
	{
		tmp_F = 20.0*FlowHighLimit/100/Para->SettingVtValue;
		tmp_I = tmp_F;
		Para->SettingRateValue = tmp_I;
		Para->SettingotherRateValue = tmp_I;
		Para->SettingTiValue = IEConvertTi(0,Para->SettingIEValue,Para);
	}
	Para->TriggerMode = PRESSURE_TRIGGER;
	Para->SettingTslopeValue = 30;
	if(Para->SettingTslopeValue >= Para->SettingTiValue)
	{
		Para->SettingTslopeValue = Para->SettingTiValue-Para->SettingTiValue%10;
	}
	//保存所有的呼吸参数
	BreathSettingParameterWrite(1, Para, Para);

	initTools(Patient_GetPType());


}

/***************************************************************************************************
*
* 初始化上一病人的呼吸参数
*/
void InitSettingOldPatientParameter(BREATH_SET_PARA * Para)
{
	//通气模式
	if((Para->BreathMode != MANUAL_MODE)&&(Para->BreathMode != ACGO_MODE))
	{
		Para->BreathMode=RTEEPROMread(BREATH_BREATHMODE_ADDR);
	}
	else
	{
		BreathMode_BeforeACGO = RTEEPROMread(BREATH_BREATHMODE_ADDR);
	}
	Para->ApnoeaMode = RTEEPROMread(BREATH_APNOEA_ADDR);
	Para->PreBreathMode = RTEEPROMread(BREATH_PRE_BREATHMODE_ADDR);  //上一个使用的呼吸模式
	Para->PreApneaMode = RTEEPROMread(BREATH_PRE_APNOEA_ADDR);  //上一个使用的呼吸模式
	Para->TriggerMode = RTEEPROMread(BREATH_TRIGGERMODE_ADDR);
	if((Para->TriggerMode != PRESSURE_TRIGGER)&&(Para->TriggerMode != FLOW_TRIGGER))//默认压力触发
	{
		Para->TriggerMode = PRESSURE_TRIGGER;
	}
	//Flow Pattent
	//     Para->SettingFlowPattem = RTEEPROMread(BREATH_FLOWPATTEN_ADDR);//波形类型    0x55:方波    0xAA:减速波
	//     if((Para->SettingFlowPattem != SQUARE_FLOWPATTEN)&&(Para->SettingFlowPattem != RAMP_FLOWPATTEN))//默认方波
	//     {
	//         Para->SettingFlowPattem = SQUARE_FLOWPATTEN;
	//     }
	//Vt默认值
	Para->SettingVtValue = (RTEEPROMread(BREATH_VT_HIGH_ADDR)<<8)|(RTEEPROMread(BREATH_VT_LOW_ADDR));
	if((Para->SettingVtValue < VTL_INFANT_LIMIT)||(Para->SettingVtValue > VTH_ADULT_LIMIT))
	{
		Para->SettingVtValue = 510;
	}
	//触发值
	Para->SettingFtrValue = RTEEPROMread(BREATH_FTR_ADDR);
	if((Para->SettingFtrValue > 200)||(Para->SettingFtrValue < 5))
	{
		Para->SettingFtrValue = 30;
	}
	Para->SettingPtrValue = RTEEPROMread(BREATH_PTR_ADDR);
//#if(LOGO_FLAG != HOSPITECH_LOGO)
//	if((Para->SettingPtrValue > 20)||(Para->SettingPtrValue < 0))
//	{
//		Para->SettingPtrValue = 2;
//	}
//#else
	if((Para->SettingPtrValue > 0)||(Para->SettingPtrValue < -20))
	{
		Para->SettingPtrValue = -3;
	}
//#endif

	//PEEP
	Para->SettingPEEPValue = RTEEPROMread(BREATH_PEEP_ADDR);
	if((Para->SettingPEEPValue < 0)||(Para->SettingPEEPValue > 50))
	{
		Para->SettingPEEPValue = 3;
	}
	//Pinsp
	Para->SettingPinspValue = RTEEPROMread(BREATH_PINSP_ADDR);
	if((Para->SettingPinspValue < 5)||(Para->SettingPinspValue > 70))
	{
		Para->SettingPinspValue = 15;
	}
	//Psuppport
	Para->SettingPsupportValue = RTEEPROMread(BREATH_PS_ADDR);
	if((Para->SettingPsupportValue < 0)||(Para->SettingPsupportValue > 70))
	{
		Para->SettingPsupportValue = 15;
	}
	//频率
	Para->SettingRateValue = RTEEPROMread(BREATH_RATE_ADDR);
	if((Para->SettingRateValue < 1)||(Para->SettingRateValue > 100))
	{
		Para->SettingRateValue = 15;
	}
	Para->SettingSimvRateValue = RTEEPROMread(BREATH_RATE_SIMV_ADDR);
	if((Para->SettingSimvRateValue < 1)||(Para->SettingSimvRateValue > 40))
	{
		Para->SettingSimvRateValue = 5;
	}
	Para->SettingotherRateValue = RTEEPROMread(BREATH_RATE_OTHER_ADDR);
	if((Para->SettingotherRateValue < 4)||(Para->SettingotherRateValue > 100))
	{
		Para->SettingotherRateValue = 15;
	}
	//Ti
	Para->SettingTiValue = (RTEEPROMread(BREATH_Ti_HIGH_ADDR)<<8)|(RTEEPROMread(BREATH_Ti_LOW_ADDR));
	if(Para->BreathMode == VCV_MODE)
	{
		if((Para->SettingTiValue < 10)||(Para->SettingTiValue > 1667))
		{
			Para->SettingTiValue = 154;//1.33s
		}
	}
	else
	{
		if((Para->SettingTiValue < 10)||(Para->SettingTiValue > 1000))
		{
			Para->SettingTiValue = 154;//1.33s
		}
	}
	//Para->SettingTiValue = Para->CalTiValue;
	//Tp
	Para->SettingTpValue = RTEEPROMread(BREATH_TP_ADDR);
	if((Para->SettingTpValue < 0)||(Para->SettingTpValue > 60))
	{
		Para->SettingTpValue = 0;//
	}
	//Sigh
	Para->SettingSighTimesValue = RTEEPROMread(BREATH_SIGHTIMES_ADDR);
	if((Para->SettingSighTimesValue < 50)||(Para->SettingSighTimesValue > 150))
	{
		Para->SettingSighTimesValue = 100;//
	}
	//Tslope
	Para->SettingTslopeValue = RTEEPROMread(BREATH_TSLOPE_ADDR);      //djq add 20140218
	if((Para->SettingTslopeValue < 0)||(Para->SettingTslopeValue > 200))
	{
		Para->SettingTslopeValue = 20;//
	}
	//I:E
	Para->SettingIEValue = RTEEPROMread(BREATH_IE_ADDR);
	if((Para->SettingIEValue < 0)||(Para->SettingIEValue > 24))// 0 ~ 24
	{
		Para->SettingIEValue = 8;
	}
	//Esens
	Para->EsenseValue = RTEEPROMread(BREATH_ESENS_ADDR);
	if((Para->EsenseValue < 0)||(Para->EsenseValue > 80))
	{
		Para->EsenseValue = 25;
	}

	//Para->SettingVtSupportValue = (RTEEPROMread(BREATH_VTS_HIGH_ADDR)<<8)|(RTEEPROMread(BREATH_VTS_LOW_ADDR));

	SetVtValue_Limit(Patient_GetInfo(),Para->BreathMode);

	//*****************************************************************************
	Para->SettingBipapRateValue = 10;
	Para->SettingThighValue = 100;
	Para->SettingTlowValue = 200;
	Para->SettingFiO2Value = 40;
	Para->SettingPhighValue = 15;
	Para->SettingPlowValue = 5;

}
/***************************************************************************************************
*
* 初始化上一病人的报警设置参数
*/
void InitAlarmSettingOldPatientParameter(ALARM_SET_PARA * Parameter)
{
	unsigned char value;

	//**Alarm Limit 1*********************************
	//Phigh
	Parameter->PressureHighValue = RTEEPROMread(PH_HIGH_ADDR);
	if(Parameter->PressureHighValue > 99)
	{
		Parameter->PressureHighValue = 99;
	}
	else if(Parameter->PressureHighValue <= 10)
	{
		Parameter->PressureHighValue = 10;
	}
	Parameter->PressureLowValue = RTEEPROMread(PH_LOW_ADDR);
	if(Parameter->PressureLowValue >= Parameter->PressureHighValue)
	{
		Parameter->PressureLowValue = Parameter->PressureHighValue - 1;
	}
	else if(Parameter->PressureLowValue <= 0)
	{
		Parameter->PressureLowValue = 0;
	}
	//MV High
	Parameter->MVHighValue = RTEEPROMread(MV_HIGH_ADDR);
	if(Parameter->MVHighValue > 99)
	{
		Parameter->MVHighValue = 99;
	}
	else if(Parameter->MVHighValue <= 1)
	{
		Parameter->MVHighValue = 1;
	}
	Parameter->MVLowValue = RTEEPROMread(MV_LOW_ADDR);
	if(Parameter->MVLowValue >= Parameter->MVHighValue)
	{
		Parameter->MVLowValue = Parameter->MVHighValue - 1;
	}
	else if(Parameter->MVLowValue <= 0)
	{
		Parameter->MVLowValue = 0;
	}
	//Rate High
	Parameter->RateHighValue = RTEEPROMread(RATE_HIGH_ADDR);
	if(Parameter->RateHighValue > 100)
	{
		Parameter->RateHighValue = 100;
	}
	else if(Parameter->RateHighValue <= 1)
	{
		Parameter->RateHighValue = 1;
	}
	Parameter->RateLowValue = RTEEPROMread(RATE_LOW_ADDR);
	if(Parameter->RateLowValue >= Parameter->RateHighValue)
	{
		Parameter->RateLowValue = Parameter->RateHighValue - 1;
	}
	else if(Parameter->RateLowValue <= 0)
	{
		Parameter->RateLowValue = 0;
	}
	//FiO2 High
	Parameter->FiO2HighValue = RTEEPROMread(FIO2_HIGH_ADDR);
	if(Parameter->FiO2HighValue >= 101)
	{
		Parameter->FiO2HighValue = 101;
	}
	else if(Parameter->FiO2HighValue <= 19)
	{
		Parameter->FiO2HighValue = 21;
	}
	Parameter->FiO2LowValue = RTEEPROMread(FIO2_LOW_ADDR);
	if(Parameter->FiO2LowValue >= Parameter->FiO2HighValue)
	{
		Parameter->FiO2LowValue = Parameter->FiO2HighValue - 1;
	}
	else if(Parameter->FiO2LowValue <= 18)
	{
		Parameter->FiO2LowValue = 20;
	}
	//Vte
	Parameter->VteHighValue = (RTEEPROMread(VTE_HIGH_ADDR_H)<<8) | RTEEPROMread(VTE_HIGH_ADDR_L);
	if(Parameter->VteHighValue >= 1000)
	{
		Parameter->VteHighValue = 1000;
	}
	else if(Parameter->VteHighValue <= 20)
	{
		Parameter->VteHighValue = 20;
	}
	Parameter->VteLowValue = (RTEEPROMread(VTE_LOW_ADDR_H)<<8) | RTEEPROMread(VTE_LOW_ADDR_L);
	if(Parameter->VteLowValue >= Parameter->VteHighValue)
	{
		Parameter->VteLowValue = Parameter->VteHighValue - 10;
	}
	else if(Parameter->VteLowValue <= 10)
	{
		Parameter->VteLowValue = 10;
	}
	//Apnea Time
	Parameter->ApneaTimerValue = RTEEPROMread(APNEA_TIME_ADDR);
	if(Parameter->ApneaTimerValue%5)
	{
		Parameter->ApneaTimerValue = Parameter->ApneaTimerValue - Parameter->ApneaTimerValue%5;
	}
	//**Alarm Limit 2***************************************************
	//EtCO2 High
	Parameter->EtCO2HighValue = RTEEPROMread(ETCO2_HIGH_ADDR);
	if(Parameter->EtCO2HighValue >= 100)
	{
		Parameter->EtCO2HighValue = 100;
	}
	else if(Parameter->EtCO2HighValue <= 1)
	{
		Parameter->EtCO2HighValue = 1;
	}
	Parameter->EtCO2LowValue = RTEEPROMread(ETCO2_LOW_ADDR);
	if(Parameter->EtCO2LowValue >= Parameter->EtCO2HighValue)
	{
		Parameter->EtCO2LowValue = Parameter->EtCO2HighValue - 1;
	}
	else if(Parameter->EtCO2LowValue <= 0)
	{
		Parameter->EtCO2LowValue = 0;
	}
	switch(ETCO2Unit)
	{
	case 0:
	case 2:
		Parameter->EtCO2LowValue = (float)(Parameter->EtCO2LowValue*1.33)+0.5;
		Parameter->EtCO2HighValue = (float)(Parameter->EtCO2HighValue*1.33)+0.5;
		break;
	case 1:
		break;
	}
	//FiCO2 High
	Parameter->FiCO2HighValue = RTEEPROMread(FICO2_HIGH_ADDR);
	if(Parameter->FiCO2HighValue >= 100)
	{
		Parameter->FiCO2HighValue = 100;
	}
	else if(Parameter->FiCO2HighValue <= 1)
	{
		Parameter->FiCO2HighValue = 1;
	}
	switch(ETCO2Unit)
	{
	case 0:
	case 2:
		Parameter->FiCO2HighValue = (float)(Parameter->FiCO2HighValue*1.33)+0.5;
		break;
	case 1:
		break;
	}
	//FiN2O High
	Parameter->N2OHighValue = RTEEPROMread(FIN2O_HIGH_ADDR);
	if(Parameter->N2OHighValue >= 82)
	{
		Parameter->N2OHighValue = 82;
	}
	else if(Parameter->N2OHighValue <= 1)
	{
		Parameter->N2OHighValue = 1;
	}
	Parameter->N2OLowValue = RTEEPROMread(FIN2O_LOW_ADDR);
	if(Parameter->N2OLowValue >= Parameter->N2OHighValue)
	{
		Parameter->N2OLowValue = Parameter->N2OHighValue - 1;
	}
	else if(Parameter->N2OLowValue <= 0)
	{
		Parameter->N2OLowValue = 0;
	}
	//EtAA High
	Parameter->EtAAHighValue = RTEEPROMread(ETAA_HIGH_ADDR);
	if(Parameter->EtAAHighValue >= 200)
	{
		Parameter->EtAAHighValue = 200;
	}
	else if(Parameter->EtAAHighValue <= 1)
	{
		Parameter->EtAAHighValue = 1;
	}
	Parameter->EtAALowValue = RTEEPROMread(ETAA_LOW_ADDR);
	if(Parameter->EtAALowValue >= Parameter->EtAAHighValue)
	{
		Parameter->EtAALowValue = Parameter->EtAAHighValue - 1;
	}
	else if(Parameter->EtAALowValue <= 0)
	{
		Parameter->EtAALowValue = 0;
	}
	//FiAA High
	Parameter->FiAAHighValue = RTEEPROMread(FIAA_HIGH_ADDR);
	if(Parameter->FiAAHighValue >= 200)
	{
		Parameter->FiAAHighValue = 200;
	}
	else if(Parameter->FiAAHighValue <= 1)
	{
		Parameter->FiAAHighValue = 1;
	}
	Parameter->FiAALowValue = RTEEPROMread(FIAA_LOW_ADDR);
	if(Parameter->FiAALowValue >= Parameter->FiAAHighValue)
	{
		Parameter->FiAALowValue = Parameter->FiAAHighValue - 1;
	}
	else if(Parameter->FiAALowValue <= 0)
	{
		Parameter->FiAALowValue = 0;
	}
	//FG High
	//Parameter->FGHighValue = RTEEPROMread(FG_HIGH_ADDR);
	//if(Parameter->FGHighValue >= 200)
	//{
	//    Parameter->FGHighValue = 200;
	//}
	//else if(Parameter->FGHighValue <= 1)
	//{
	//    Parameter->FGHighValue = 1;
	//}
	//Parameter->FGLowValue = RTEEPROMread(FG_LOW_ADDR);
	//if(Parameter->FGLowValue >= Parameter->FGHighValue)
	//{
	//    Parameter->FGLowValue = Parameter->FGHighValue - 1;
	//}
	//else if(Parameter->FGLowValue <= 0)
	//{
	//    Parameter->FGLowValue = 0;
	//}
	//**Alarm Limit 3**************************************************
	//SpO2 High
	Parameter->SpO2HighValue = RTEEPROMread(SPO2_HIGH_ADDR);
	if(Parameter->SpO2HighValue >= 100)
	{
		Parameter->SpO2HighValue = 100;
	}
	else if(Parameter->SpO2HighValue <= 50)
	{
		Parameter->SpO2HighValue = 50;
	}
	Parameter->SpO2LowValue = RTEEPROMread(SPO2_LOW_ADDR);
	if(Parameter->SpO2LowValue >= Parameter->SpO2HighValue)
	{
		Parameter->SpO2LowValue = Parameter->SpO2HighValue - 1;
	}
	else if(Parameter->SpO2LowValue <= 49)
	{
		Parameter->SpO2LowValue = 49;
	}
	//Pulse High
	Parameter->PulseHighValue = RTEEPROMread(PLUS_HIGH_ADDR);
	if(Parameter->PulseHighValue >= 250)
	{
		Parameter->PulseHighValue = 250;
	}
	else if(Parameter->PulseHighValue <= 31)
	{
		Parameter->PulseHighValue = 31;
	}
	Parameter->PulseLowValue = RTEEPROMread(PLUS_LOW_ADDR);
	if(Parameter->PulseLowValue >=  Parameter->PulseHighValue)
	{
		Parameter->PulseLowValue = Parameter->PulseHighValue - 1;
	}
	else if(Parameter->PulseLowValue <= 30)
	{
		Parameter->PulseLowValue = 30;
	}
	/*
	//PI High
	Parameter->PIHighValue = (RTEEPROMread(PI_HIGH_ADDR_H)<<8) | RTEEPROMread(PI_HIGH_ADDR_L);
	if(Parameter->PIHighValue >= 250)
	{
	Parameter->PIHighValue = 250;
	}
	else if(Parameter->PIHighValue <= 31)
	{
	Parameter->PIHighValue = 31;
	}
	Parameter->PILowValue = (RTEEPROMread(PI_LOW_ADDR_H)<<8) | RTEEPROMread(PI_LOW_ADDR_L);
	if(Parameter->PILowValue >=  Parameter->PIHighValue)
	{
	Parameter->PILowValue = Parameter->PIHighValue - 1;
	}
	else if(Parameter->PILowValue <= 30)
	{
	Parameter->PILowValue = 30;
	}

	//Alarm Volume
	value = RTEEPROMread(ALARM_VOICE_VOLUME_ADDR);
	if((value>=0)&&(value<=4))
	{
	Parameter->AlarmVolumeValue = (int)value;// - 1
	}
	else
	{
	Parameter->AlarmVolumeValue = 4;
	RTEEPROMwrite(ALARM_VOICE_VOLUME_ADDR,4);
	}
	//VolumeChannelSelect(Parameter->AlarmVolumeValue);
	SendOperationMessage(BUZZER_VOLUME_SETTING,USART3);
	*/
}
/************************************************************************/
/* Demo          20221108   lyy add                                                        */
/************************************************************************/
#ifdef DEMO
//初始化Demo的设置参数
void Demo_InitSettingParameter(void)
{
	if (Demo_Switch)
	{
		BeathSettingPara.SettingPEEPValue = 3;
		BeathSettingPara.SettingPinspValue = 30;
		BeathSettingPara.SettingRateValue = 15;
		BeathSettingPara.SettingSimvRateValue = 10;
		BeathSettingPara.SettingotherRateValue =15;
		BeathSettingPara.SettingBipapRateValue = 10;
		BeathSettingPara.SettingTiValue = 150;
		BeathSettingPara.SettingTpValue = 50;
		BeathSettingPara.SettingVtValue = 500;
		MonitorPara.ResistValue = Demo_SetR;
		MonitorPara.ComplianceValue = Demo_SetC;
		//
		CO2_Parameters.etCO2 = 30;
		CO2_Parameters.insCO2 = 5;
		MonitorPara.FiO2Value = BeathSettingPara.SettingFiO2Value;
	}
}
void Demo_Get_Cycle(void)
{
	float Demo_Et = 0;
	Demo_PeepValue = (float)(BeathSettingPara.SettingPEEPValue);
	Demo_PinspValue = (float)(BeathSettingPara.SettingPinspValue);
	Demo_VtValue = (float)(BeathSettingPara.SettingVtValue);
	Demo_TiValue = (float)(BeathSettingPara.SettingTiValue) / 100.0;
	if( (BeathSettingPara.BreathMode == SIMV_PCV_MODE)
		||(BeathSettingPara.BreathMode==SIMV_VCV_MODE))
	{
		Demo_RateValue = (float)(BeathSettingPara.SettingSimvRateValue);
	}
	else
	{
		Demo_RateValue = (float)(BeathSettingPara.SettingRateValue);
	}
	if((BeathSettingPara.BreathMode == VCV_MODE)||(BeathSettingPara.BreathMode == SIGH_MODE))
	{
		Demo_TpValue = BeathSettingPara.SettingTpValue* Demo_TiValue / 100.0;
	}
	else
	{
		Demo_TpValue = 0;
	}
	Demo_Time_Timer = 60.0 /  Demo_RateValue; //周期
	Demo_Time_Exsp = Demo_Time_Timer - Demo_TiValue - Demo_TpValue;
	Demo_Cycle_Insp = (int)(Demo_TiValue * 1000.0 / (float)Wave_Scan_Time);
	Demo_Cycle_Tp = (int)(Demo_TpValue * 1000.0 / (float)Wave_Scan_Time );
	Demo_Cycle  = (int)(Demo_Time_Timer * 1000.0 / (float)Wave_Scan_Time );
	Demo_Cycle_Exsp  =  Demo_Cycle - Demo_Cycle_Insp - Demo_Cycle_Tp;
	Demo_Et0 = exp(-Demo_TiValue * 1000.0 / ((float)(MonitorPara.ResistValue)*(float)(MonitorPara.ComplianceValue)));
	Demo_Et1 = exp(-Demo_TpValue * 1000.0 / ((float)(MonitorPara.ResistValue)*(float)(MonitorPara.ComplianceValue)));
	Demo_Et2 = exp(-Demo_Time_Exsp * 1000.0 / ((float)(MonitorPara.ResistValue)*(float)(MonitorPara.ComplianceValue)));
	Demo_RC = (float)(MonitorPara.ResistValue) * (float) (MonitorPara.ComplianceValue) / 1000.0;
	MonitorPara.FiO2Value = BeathSettingPara.SettingFiO2Value;
	MonitorPara.PeepValue = (int)(Demo_PeepValue * 10 );
	MonitorPara.PminValue = MonitorPara.PeepValue;
	MonitorPara.RateTotalValue =(int) (Demo_RateValue);
	if((BeathSettingPara.BreathMode==PSV_MODE)
		&(!(AlarmsWork.presentAlarms&ALARM_APNEA)))
	{
		MonitorPara.VtiValue = 0;
	}
	else if( (BeathSettingPara.BreathMode == VCV_MODE)
		||(BeathSettingPara.BreathMode == SIGH_MODE)
		||((BeathSettingPara.BreathMode == PSV_MODE)&(BeathSettingPara.ApnoeaMode == VCV_MODE))
		||(BeathSettingPara.BreathMode == SIMV_VCV_MODE))
	{
		MonitorPara.VtiValue = (int)(Demo_VtValue);
	}
	else if( (BeathSettingPara.BreathMode == SIMV_PCV_MODE)
		||((BeathSettingPara.BreathMode==PSV_MODE)&(BeathSettingPara.ApnoeaMode==PCV_MODE))
		||(BeathSettingPara.BreathMode == PCV_MODE)
		||(BeathSettingPara.BreathMode == BIPAP_MODE))
	{
		Demo_Et = exp(-Demo_TiValue * 1000.0 / ((float)(MonitorPara.ResistValue)*(float)(MonitorPara.ComplianceValue)));
		MonitorPara.VtiValue = (int)((MonitorPara.ComplianceValue) * Demo_PinspValue  * (0.5 + 0.5 * Demo_Et * Demo_Et - Demo_Et));
	}
	MonitorPara.MinuteVolumeValue = 9900;//MonitorPara.VtiValue * MonitorPara.RateTotalValue / 10;
	MonitorPara.MVspnValue = 9900;
	MonitorPara.VteValue = MonitorPara.VtiValue;
}
//
void TaskDemoGetCycle(void)
{
	if(Demo_InitFspn)
	{
		Demo_InitFspn = 0;
		MonitorPara.fspnValue = 0;
		MonitorPara.MVspnValue = 0;
		RealtimePara.AirGasPressure = 300;
		RealtimePara.O2GasPressure = 300;
	}
	if(Demo_Change_Cycle)
	{
		Demo_Change_Cycle = 0;
		if((Demo_Switch)&(!StandbyState))
		{
			Demo_Get_Cycle();
		}
	}
}
void Demo_Update_Pmean(void)
{
	if((BeathSettingPara.BreathMode==PSV_MODE)&(!(AlarmsWork.presentAlarms&ALARM_APNEA)))
	{
		MonitorPara.PmeanValue = 0;
	}
	else if( (BeathSettingPara.BreathMode == SIMV_PCV_MODE)
		||(BeathSettingPara.BreathMode ==  PCV_MODE)
		||((BeathSettingPara.BreathMode==PSV_MODE)&(BeathSettingPara.ApnoeaMode==PCV_MODE))
		||(BeathSettingPara.BreathMode == BIPAP_MODE))
	{
		MonitorPara.PmeanValue = (int)(10.0 * (Demo_PinspValue * Demo_TiValue
			+ Demo_Et0 * Demo_RC * Demo_PinspValue
			- Demo_PinspValue * Demo_RC
			+ (1.0 - Demo_Et2) * Demo_RC * Demo_Peak
			) / Demo_Time_Timer);
	}
	else if( (BeathSettingPara.BreathMode == VCV_MODE)
		||(BeathSettingPara.BreathMode == SIGH_MODE)
		||(BeathSettingPara.BreathMode == SIMV_VCV_MODE)
		||((BeathSettingPara.BreathMode==PSV_MODE)&(BeathSettingPara.ApnoeaMode==VCV_MODE)))
	{
		MonitorPara.PmeanValue = (int)(10.0 * (Demo_VtValue * (float)(MonitorPara.ResistValue) / 1000.0
			+ Demo_VtValue * Demo_TiValue / (float) (MonitorPara.ComplianceValue) / 2.0
			+ Demo_Plat * Demo_RC * (1.0 - Demo_Et2)
			+ Demo_Plat * Demo_TpValue + (Demo_Peak - Demo_Plat) * Demo_RC * (1 - Demo_Et1)
			) / Demo_Time_Timer);
	}
	MonitorPara.PmeanValue += MonitorPara.PeepValue;
}
void Demo_Get_Time(void)
{
	switch(Demo_InEx_Flg)
	{
	case 0:
		if(Demo_Time >= Demo_Cycle_Insp)
		{
			if( (BeathSettingPara.BreathMode == VCV_MODE)
				||(BeathSettingPara.BreathMode == SIGH_MODE)
				||((BeathSettingPara.BreathMode == PSV_MODE)&(BeathSettingPara.ApnoeaMode == VCV_MODE))
				)
			{
				Demo_InEx_Flg = 1;
				if(MonitorPara.ComplianceValue) //setted in Demo_InitSettingParameter()
				{
					Demo_Plat = Demo_VtValue / (float)(MonitorPara.ComplianceValue);
				}
				MonitorPara.PplatValue = (int)((Demo_Plat + Demo_PeepValue)* 10);
			}
			else
			{
				Demo_InEx_Flg = 2;
			}
			Demo_Peak = Demo_Paw;
			MonitorPara.PpeakValue = (int)((Demo_Peak + Demo_PeepValue) * 10);
			Demo_Update_Pmean();
			Demo_Time = 0;
// 			MonitorPara.FlowValue = BeathSettingPara.SettingFlowValue;
// 			SpO2_Parameters.SpO2 = 94;

		}
		break;
	case 1:
		if( (BeathSettingPara.BreathMode == VCV_MODE)
			||(BeathSettingPara.BreathMode == SIGH_MODE))
		{
			if(Demo_Time>= Demo_Cycle_Tp)
			{
				Demo_Time = 0;
				Demo_InEx_Flg = 2;
			}
		}
		else
		{
			Demo_InEx_Flg = 2;
			Demo_Time = 0;
		}
		break;
	case 2:
		if(Demo_Time + 1 == Demo_Cycle_Exsp)
		{
			InspFlagForAlarm = 1;
			InspFlagForCurve = 1;
		}
		else  if(Demo_Time >= Demo_Cycle_Exsp)
		{
			Demo_InEx_Flg = 0;
			Demo_Time = 0;
			if(Demo_Change_Cycle_A)
			{
				Demo_Change_Cycle_A = 0;
				Demo_Get_Cycle();
			}
		}
		break;
	}

	if (LungRec_GetStartStopFlag() == 1)
	{
		if (Demo_InEx_Flg > 1)
		{
			Demo_InEx_Flg = 1;
		}
	}
}
void Demo_Update_Para(void)
{
	switch(Demo_InEx_Flg)
	{
	case 0 :
		if((BeathSettingPara.BreathMode==PSV_MODE)&(!(AlarmsWork.presentAlarms&ALARM_APNEA)))
		{
			if(!Demo_InitPsv)
			{
				Demo_InitPsv = 1;
				Demo_Flow = 0;
				Demo_Paw =  0;
				MonitorPara.VtiValue = 0;
				MonitorPara.FiO2Value = 0;
				MonitorPara.VteValue = 0;
				MonitorPara.MinuteVolumeValue = 0;
				MonitorPara.PpeakValue = 0;
				MonitorPara.PmeanValue = 0;
				MonitorPara.PplatValue = 0;
				MonitorPara.PminValue = 0;
				MonitorPara.ResistValue = 0;
				MonitorPara.ComplianceValue = 0;
				MonitorPara.RateTotalValue = 0;
			}
		}
		else if( (BeathSettingPara.BreathMode == VCV_MODE)
			||((BeathSettingPara.BreathMode== PSV_MODE)&(BeathSettingPara.ApnoeaMode==VCV_MODE))
			||(BeathSettingPara.BreathMode == SIMV_VCV_MODE)
			||(BeathSettingPara.BreathMode == SIGH_MODE))
		{
			if(Demo_Time)
			{
				Demo_Flow = Demo_VtValue / Demo_TiValue;
			}
			else
			{
				Demo_Flow = 0;
			}
			Demo_V = (float)(Demo_Time) * Demo_Flow * (float)(Wave_Scan_Time) /1000.0;
			Demo_Paw = (float)(MonitorPara.ResistValue) * (float)(Demo_Flow) /1000.0
				+ 1.0 / (float)(MonitorPara.ComplianceValue)
				* (float)(Demo_Time) * Demo_Flow  * (float)(Wave_Scan_Time) /1000.0;
		}
		else if( (BeathSettingPara.BreathMode == SIMV_PCV_MODE)
			||((BeathSettingPara.BreathMode== PSV_MODE)&(BeathSettingPara.ApnoeaMode==PCV_MODE))
			||(BeathSettingPara.BreathMode == PCV_MODE)
			||(BeathSettingPara.BreathMode == BIPAP_MODE))
		{
			Demo_Paw = Demo_PinspValue
				* (1.0 - exp( - ((float)(Demo_Time) * (float)(Wave_Scan_Time) / 1000.0 + 1.5) / Demo_RC));

			Demo_V = Demo_PinspValue * (float)(MonitorPara.ComplianceValue)
				* (0.5 + 0.5 * exp( - (float)(Demo_Time) * (float)(Wave_Scan_Time) / 1000.0 / Demo_RC)
				* exp( - (float)(Demo_Time) * (float)(Wave_Scan_Time) / 1000.0 / Demo_RC)
				- exp( - (float)(Demo_Time) * (float)(Wave_Scan_Time) / 1000.0 / Demo_RC)
				);

			Demo_Flow = Demo_Paw  * 1000.0 / (float)(MonitorPara.ResistValue)
				*exp( - (float)(Demo_Time) * (float)(Wave_Scan_Time) / 1000.0 / Demo_RC);
		}
		break;
	case 1:

		if (LungRec_GetStartStopFlag() == 1)
		{
			Demo_Paw = Demo_Peak;
			Demo_Flow = 0;
		}
		else
		{
		
		if((BeathSettingPara.BreathMode==PSV_MODE)&(!(AlarmsWork.presentAlarms&ALARM_APNEA)))
		{
			if(!Demo_InitPsv)
			{
				Demo_InitPsv = 1;
				Demo_Flow = 0;
				Demo_Paw =  0;
				MonitorPara.VtiValue = 0;
				MonitorPara.VteValue = 0;
				MonitorPara.MinuteVolumeValue = 0;
				MonitorPara.PpeakValue = 0;
				MonitorPara.PmeanValue = 0;
				MonitorPara.PplatValue = 0;
				MonitorPara.PminValue = 0;
				MonitorPara.FiO2Value = 0;
				MonitorPara.ResistValue = 0;
				MonitorPara.ComplianceValue = 0;
				MonitorPara.RateTotalValue  = 0;
			}
		}
		else  if( (BeathSettingPara.BreathMode == VCV_MODE)
			||(BeathSettingPara.BreathMode == SIGH_MODE)
			||(BeathSettingPara.BreathMode==PSV_MODE)&(BeathSettingPara.ApnoeaMode==PCV_MODE))
		{
			Demo_V = Demo_VtValue;
			Demo_Paw = Demo_Plat +
				((Demo_Peak  - Demo_Plat)
				*exp(-(float)(Demo_Time)  * (float)(Wave_Scan_Time) /(float)(MonitorPara.ResistValue ) / 4.0));

			Demo_Flow = 0;
		}
		
		}
		break;
	case 2:
		if((BeathSettingPara.BreathMode==PSV_MODE)&(!(AlarmsWork.presentAlarms&ALARM_APNEA)))
		{
			if(!Demo_InitPsv)
			{
				Demo_InitPsv = 1;
				Demo_Flow = 0;
				Demo_Paw =  0;
				MonitorPara.VtiValue = 0;
				MonitorPara.VteValue = 0;
				MonitorPara.MinuteVolumeValue = 0;
				MonitorPara.PpeakValue = 0;
				MonitorPara.PmeanValue = 0;
				MonitorPara.PplatValue = 0;
				MonitorPara.PminValue = 0;
				MonitorPara.FiO2Value = 0;
				MonitorPara.ComplianceValue = 0;
				MonitorPara.RateTotalValue  = 0;
			}
		}
		else if( (BeathSettingPara.BreathMode == VCV_MODE)
			||((BeathSettingPara.BreathMode==PSV_MODE)&(BeathSettingPara.ApnoeaMode==VCV_MODE))
			||(BeathSettingPara.BreathMode == SIMV_VCV_MODE)
			||(BeathSettingPara.BreathMode == SIGH_MODE))
		{
			Demo_Paw =  Demo_Plat
				* exp(-(float)(Demo_Time) * (float)(Wave_Scan_Time) /(float)(MonitorPara.ComplianceValue) / 5.0);

			Demo_V =  Demo_Plat * (float)(MonitorPara.ComplianceValue)
				* (exp(-(float)(Demo_Time) * (float)(Wave_Scan_Time) /(float)(MonitorPara.ComplianceValue) /(float)(MonitorPara.ResistValue - 5.0)) - 1.0)
				+ MonitorPara.VtiValue;

			Demo_Flow = -( Demo_Plat  * 1000.0 /  (float)(MonitorPara.ResistValue-5.0)
				* exp(-(float)(Demo_Time)* (float)(Wave_Scan_Time) / (float)(MonitorPara.ComplianceValue) /(float)(MonitorPara.ResistValue-5.0)) );
		}
		else if( (BeathSettingPara.BreathMode == SIMV_PCV_MODE)
			||((BeathSettingPara.BreathMode==PSV_MODE)&(BeathSettingPara.ApnoeaMode==PCV_MODE))
			||(BeathSettingPara.BreathMode == PCV_MODE)
			||(BeathSettingPara.BreathMode == BIPAP_MODE))
		{
			Demo_Paw =  Demo_Peak
				* exp(-(float)(Demo_Time) * (float)(Wave_Scan_Time) /((float)(MonitorPara.ComplianceValue) * 5.0));

			Demo_Flow = -( (float)(Demo_Peak)  * 1000.0 /  (float)(MonitorPara.ResistValue - 5.0)
				* exp(-(float)(Demo_Time) * (float)(Wave_Scan_Time) / (float)(MonitorPara.ComplianceValue) /(float)(MonitorPara.ResistValue - 5.0)) );

			Demo_V = MonitorPara.VtiValue + Demo_Peak * (float)(MonitorPara.ComplianceValue)
				* (exp(-(float)(Demo_Time) * (float)(Wave_Scan_Time) / (float)(MonitorPara.ComplianceValue) / (float)(MonitorPara.ResistValue - 5.0)) - 1.0);
		}
		break;
	}

	RealtimePara.Volume = (int)(Demo_V);
	AA_Para.N2OWaveData = 20;
	AA_Para.AX2WaveData = 20;
	AA_Para.AX1WavaData = 20;
	if(BeathSettingPara.BreathMode == BIPAP_MODE)
	{
		RealtimePara.Pressure = (int)(Demo_Paw  * 10.0);
	}
	else
	{
		RealtimePara.Pressure = (int)((Demo_Paw + Demo_PeepValue) * 10.0);
	}
	RealtimePara.Flow = (int)(Demo_Flow * 3.0 / 50.0);
	Demo_Time ++;
}
//Demo参数更新
void DemoParaUpdate(void)
{
	int flag;
	if(Demo_Switch)
	{
		Demo_Get_Time();
		Demo_Update_Para();
		Curve_Color = GUI_BLUE;
		if(Demo_InEx_Flg)
		{
			Curve_Color = GUI_RED;
		}
	}
}
//Demo参数初始化
void DemoParaInit(void)
{
	if(Demo_Switch)
	{
		Demo_InitFspn = 1;
		Demo_Change_Cycle = 1;
//		BDU_Comm_Set(0);
//		GAS_Comm_Set(0);
	}
	else
	{
		RealtimePara.Pressure = 0;
	}

	//氧气开关打开
	Menu_hWin.InfoWin.InfoChangeFlag |= MODES_INFO_CHANGED_FLAG;
	Demo_InitSettingParameter();

}
#endif