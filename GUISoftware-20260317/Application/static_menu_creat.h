#ifndef Static_Menu_Creat_H
#define Static_Menu_Creat_H
#include <stdio.h>
#include "ButtonExCtrl.h"
#include "TracingCurve.h"
#include "RectInfoCtrl.h"
#include "RectPaintCtrl.h"
#include "Key_Handle.h"
#include "POWER_INFO.h"
#include "MULTIPAGE.h"
#include "MainTask.h"
#include "MonitorMenu.h"
#include "Communication_Handle.h"
#include "AA_Module.h"
// #include "MedibusComm_cfg.h"
#include "alarm_handle.h"
#include "HomePrompt_cfg.h"
#include "HomePrompt.h"
#include "os_prio.h"
#include "DispStringLib.h"
#include "AlarmDispStrLib.h"
#include "DispStrs_cfg.h"
#include "DispStrs.h"
#include "MultiText.h"
#include "WIDGET_Tools.h"

#ifndef FALSE
#define FALSE (1 == 0)
#endif
#ifndef TRUE
#define TRUE  (1 == 1)
#endif
//#define INSP_VALVE_CALI_HOSPITAL_TIME 60
#define INSP_VALVE_CALI_FACTORY_TIME 15
#define OXY_CALI_TIME  5
//
#define INSP_PHASE 0x22
#define EXP_PHASE  0x11
//
#define WAVEFORM_FILTER   0X02   //ZY 2016/6/3

#define VENTBUTTON_NUM    8//控制参数按钮个数 ylk add 20181009
/*
*@{ 波形选项
*/
#if (MACHINE_TYPE == X6 || MACHINE_TYPE == Z6) //X6/Z6
	#define WAVEFORM_PRESSURE 0x00
	#define WAVEFORM_FLOW     0x01
	#define WAVEFORM_VOLUME   0x02
	#ifdef SPO2_SWITCH //是否带SPO2
		#define WAVEFORM_SPO2     0x03
		#define WAVEFORM_CO2      0x04
		#define WAVEFORM_LOOP     0x05
		#define LOOP_PV           0x05
		#define LOOP_VF           0x06
		#define LOOP_V_CO2		  0x07

	#else
		#define WAVEFORM_CO2      0x03
		#define WAVEFORM_LOOP     0x04
		#define LOOP_PV           0x04
		#define LOOP_VF           0x05
		#define LOOP_V_CO2		  0x06

	#endif
#else //其他机型
	#define WAVEFORM_PRESSURE 0x00
	#define WAVEFORM_FLOW     0x01
	#define WAVEFORM_VOLUME   0x02
	#ifdef SPO2_SWITCH //带SPO2
		#define WAVEFORM_SPO2     0x03
		#define WAVEFORM_CO2      0x04
		#ifdef AA_WAVE_SWITCH //带AA
			#define WAVEFORM_N2O      0X05
			#define WAVEFORM_PAA      0X06
			#define WAVEFORM_SAA      0X07
			#define LOOP_V_CO2		  0x08
			#define WAVEFORM_LOOP     0x09
			#define LOOP_PV           0x0A
			#define LOOP_VF           0x0B
			#define LOOP_PF		      0x0C

		#else

			#define LOOP_V_CO2		  0x05
			#define WAVEFORM_LOOP     0x06
			#define LOOP_PV           0x07
			#define LOOP_VF           0x08
			#define LOOP_PF		      0x09

		#endif
	#else	//不带SPO2
		#define WAVEFORM_CO2      0x03
		#ifdef AA_WAVE_SWITCH  //带AA
			#define WAVEFORM_N2O      0X04
			#define WAVEFORM_PAA      0X05
			#define WAVEFORM_SAA      0X06
			#define LOOP_V_CO2		  0x07
			#define WAVEFORM_LOOP     0x08
			#define LOOP_PV           0x09
			#define LOOP_VF           0x0A
			#define LOOP_PF		      0x0B

		#else
			#define LOOP_V_CO2		  0x04
			#define WAVEFORM_LOOP     0x05
			#define LOOP_PV           0x06
			#define LOOP_VF           0x07
			#define LOOP_PF		      0x08

		#endif
	#endif
#endif
/*
*@}
*/

#define PAW_VOLUME_SpO2   0x0A
#define PAW_SpO2_LOOP     0x0B
#define PAW_SpO2_CO2      0x0C
//------------------------协议中模式类型值
 //一个不具有实际意义的模式，用于分辨存储的上一个模式是否有效
#define IDLE_MODE        0x00 
#define VCV_MODE         0x08
#define SIMV_VCV_MODE    0x12
#define SIMV_PCV_MODE    0x17
#define PCV_MODE         0x21
#define PSV_MODE         0x1C
//#define BACKUP_MODE      0x1D //  20130328
#define BIPAP_MODE       0x30
#define SIGH_MODE        0x0D
#define APRV_MODE        0x2B
#define APNEA_MODE       0x26
#define PRVC_MODE        0x45 //  21030409
#define SIMV_PRVC_MODE   0x4A //  21030409
#define VS_MODE			 0x13 //  21030409

#define ACGO_MODE        0x3A
#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C) 
#define MANUAL_MODE      0x35//0x35 //cpp md 20130719
#else
#define MANUAL_MODE      0x28//0x35 //cpp md 20130719
#endif

#define BACKUP_MODE      0x87 //sun add 20141224
#define PSV_MODE_BACK   BACKUP_MODE
/////////////////////////////////////////
#define PRESSURE_TRIGGER 0x00
#define FLOW_TRIGGER     0x01
//
#define PARA_IN_VCV
#define MODES_INFO_CHANGED_FLAG   0x01
#define ASSIST_INFO_CHANGED_FLAG  0x02
#define ALARM_INFO_CHANGED_FLAG   0x04
#define SILENCE_INFO_CHANGED_FLAG 0x08
#define POWER_INFO_CHANGED_FLAG   0x10
#define TIMER_INFO_CHANGED_FLAG   0x20
#define LOCK_INFO_CHANGED_FLAG    0x40
#define DRIVER_GAS_TYPE_FLAG      0x80//djq add 20170408
#define PHASEIN_ZERO_CHANGE_FLAG  0x100
#define ALARM_MUTI_CHANGED_FLAG   0x200   //多重报警 (同时加上报警，因为多重报警在报警栏中)

#define AA_RANGE_CHANGE_FLAG      0x400	  
#define CPB_CHANGE_FLAG 0x800
#define PATIENT_TYPE_INFO_CHANGE_FLAG 0x1000
#define CUR_ALARM_INFO_CHANGED_FLAG   0x2000

//
#define SCAN_TIME       5
#define LOOP_DISPLAY    0x08
//
#define STANDBY_LED       0x02
#define MUTE_LED          0x01
#define BATTERY_RUN_LED   0x04
//
#define LOCK_LED          0x80
#define MENU_LED          0x04
#define NEBULIZER_LED     0x08
#define INSP_HOLD_LED     0x10
#define EXP_HOLD_LED      0x20
#define OXY_SUCTION_LED  0x200
#define FREZZE_LED       0x100
//
#define LOW_PRIORITY_ALARM_LED    0x0400 
#define MEDIUM_PRIORITY_ALARM_LED LOW_PRIORITY_ALARM_LED
#define HIGH_PRIORITY_ALARM_LED   0x0200
//
#define LED_UPDATE_OPERATION      0x01
#define ALARM_PRIORITY_CHANGED    0
#define HANDSHAKE_INFO_DELIVER    0x02
#define BUZZER_CONTROL_OPERATION  0x03
#define BUZZER_VOLUME_SETTING     0x04
#define BUZZER_VOLUME_TEST        0x05
#define BUZZER_VOLUME_START       0x06 //  20130402

#define SEND_MACHINE_MODE_FLAG 0x08

#define ADD_ITEM        0x11
#define INSERT_ITEM     0x22
//
#define FREEZE_ACTIVE                   0x01
#define FREEZE_CANCEL                   0x02
#define MENU_CREATION                   0xbb
//  20130328
#define FLOW_MIN       3333//35  //最小流速 100mL/s
#define FLOW_MAX       1500 //最大流速 1500mL/s  //  20130618
#define TE_MIN         3//2    //呼气时间最短 0.2s
#define TI_MIN         1    //吸气时间最短 0.1s //  20130618
#define TI_MAX         100  //吸气时间最长 10.0s 
#define TI_MAX_100     1000  //吸气时间最长 10.0s 
#define X10            10   //吸气时间放大倍数



#define VTH_ADULT_LIMIT   1600

#ifdef LANGUAGE_CHINESE_FLAG
#define VTH_PADI_LIMIT    400
#define VTH_INFANT_LIMIT  200
#else
#define VTH_PADI_LIMIT    360
#define VTH_INFANT_LIMIT  100
#endif

#define VTL_ADULT_LIMIT   100
#define VTL_PADI_LIMIT    50

#define VTL_INFANT_LIMIT_5  5
#define VTL_INFANT_LIMIT 10

#define MAX_PAW                  70
#define EXPTIME_LIMIT            30
#define PARA_PSUPPORTH_LIMIT     70
#define PARA_PINSPH_LIMIT        70

//麻醉气体模块类型
#define  SET_AA_OPERATE_STANDBY     0x11
#define  SET_AA_OPERATE_MEASURE     0x12
#define  SET_AA_PUMPON              0x13
#define  SET_AA_PUMPOFF             0x14
#define  SET_AA_IRO2DELAY           0x15
#define  SET_AA_SETO2OPTION         0x16
#define  SET_AA_SETO2SENSORTYPE     0x17
#define  SET_AA_ZEROCAL             0x18
#define  SET_AA_SPANCALO2           0x19
#define  SET_AA_SPANCAL             0x1A

#define  SET_AA_GAS_HAL             0x1B // new add 20181127 麻醉气体模块类型
#define  SET_AA_GAS_ENF             0x1C
#define  SET_AA_GAS_ISO             0x1D
#define  SET_AA_GAS_SEV             0x1E
#define  SET_AA_GAS_DES             0x1F
#define  SET_AA_ZERO_INIT           0x20 
#define  SET_AA_EXPARA_O2           0x21
#define  SET_AA_EXPARA_N2O          0x22
#define  SET_AA_STOP_CONTINUE       0x23
#define  SET_AA_UNIT_ATS            0x24
#define  SET_AA_UNIT_ATPS           0x25
#define  SET_AA_BRDETECTION         0x26
#define  SET_AA_SPANCAL21           0x27

//ylk move 20181128
#define HALCOLOR	0x170AA0//0x3504C1
#define ENFCOLOR	0x0074FF//0x0073FF
#define DESCOLOR	0xd49a26//0x483000//0x00DBFE  269ad4
#define SEVCOLOR	0x00E1EB//0x00DBFE
#define ISOCOLOR	0xe413c2//0xFA00D2//0xD681DF
//

#define INFO_BK_COLOR					0x1b1818
#define POWER_SELECT_COLOR				0xc9cc84
#define POWER_BATTERY_COLOR				0x4AB469
#define WINDOW_BK_COLOR	0x181818 

#ifndef PCV_MODE_ENABLE  //  20130130
  #define PCV_MODE_ENABLE  0x00
#endif
#ifndef RTC_TIMER_S
#define RTC_TIMER_S

//#ifndef Int8U
//#define Int8U unsigned char
//#define Int16U unsigned short int
//#endif

typedef struct
{
    Int8U RTC_Sec;      /* Secend value - [0,59]*/
    Int8U RTC_Min;     /* Minute value - [0,59] */
    Int8U RTC_Hour;    /* Hour value - [0,23] */
    Int8U RTC_Mday;    /* Day of the month value - [1,31] */
    Int8U RTC_Mon;     /* Month value - [1,12] */
    Int16U  RTC_Year;    /* Year value - [0,4095] */
} RTC_TIMER;
#endif //RTC_TIMER
typedef struct
{
    //ButtonExCtrl_Handle ButtonEx_hObj[8];
    RectPaintCtrl_Handle RectPaint_hObj[8];
    RectInfoCtrl_Handle RectInfo_hObj;
    //BUTTON_Handle Button_hObj[3];

} FixedMenu_handle;
typedef struct
{
    //TracingCurve_Handle Tobj[3];
    int LoopFlag1;
    int WaveForms[4/*3*/];
    int FocusCurve;
	//int LoopFlag2;
} CURVE_HWIN;
typedef struct
{
    WM_HWIN ParentWin;
    TracingCurve_Handle Tobj[4]; // 
    MULTITEXT_Handle Robj[7];//  [2/16/2022 yaoyao modify 6->7]右侧快捷监测参数改为7个
	MULTITEXT_Handle Left_Robj[8];
    TEXT_Handle tObj; //ylk add 20181211
} HOME_MENU_HWIN;
typedef struct
{
    BUTTON_Handle bObj[13];//djq modify 20170408   5->6   增加驱动气信息显示  ylk modify 6->8 增加多重报警状态显示 9 未达到高精度提示信息
    POWER_INFO_Handle pObj;
    int InfoChangeFlag;
} INFO_HWIN;

typedef struct
{
	WM_HWIN AlarmWin_Lim1;
	WM_HWIN AlarmWin_Lim2;
	WM_HWIN AlarmWin_Lim3;
	WM_HWIN AlarmWin_Info;
}ALARM_HWIN;

typedef struct
{
    FixedMenu_handle Mobj; //未用 ???
    CURVE_HWIN cWin; 
    WM_HWIN hWin; //功能键
    WM_HWIN bWin; //呼吸参数设置键
    HOME_MENU_HWIN HomeWin; 
    INFO_HWIN InfoWin;
    MULTIPAGE_Handle RecordWin; //校验窗及密码窗
    WM_HWIN ActivedWin; //multipage 窗
	WM_HWIN Swin; //standby 窗
    POWER_INFO_Handle pObj; //未用 ???
    WM_HWIN TextWin;  //sun add 20140521
	WM_HWIN AlarmWin;
	ALARM_HWIN Alarm_Win;//ylk add 20181128
	TOOLS_MENU_HWIN ToolsWin;
	WM_HWIN ShutDowAskWin;

} MENU_HWIN;

enum ModeIndex{ MACHINE_INDEX, MANUAL_INDEX};//zkq add 20170106用于BREATH_SET_PARA结构体BreathMode_Queue数组的索引

typedef struct
{
    int flg;
    WM_HWIN hWin;
} PARA_SETTING_STATE;
typedef struct
{
    int SettingVtValue;
	int SettingVtSupportValue;            //5 ~ 1600

    int SettingTiValue;
    int settingTeValue; //  20130615
    int SettingRateValue;
    int SettingSimvRateValue;
    int SettingotherRateValue;
	int SettingVSRateValue;

    int SettingTpValue;
    int SettingPsupportValue;
    int SettingPinspValue;
    int SettingBipapRateValue;
    int SettingPtrValue;
    int SettingFtrValue;
    int SettingPEEPValue;
    int SettingFiO2Value;
    int TriggerMode;
    int BreathMode;
    int SettingThighValue;
    int SettingTlowValue;
    int ApnoeaMode;
    int PreBreathMode;
    int PreApneaMode;
    int SettingSighTimesValue;
    int SettingIEValue;
    int SettingTslopeValue; //  20130607

	int EsenseValue;  //sun add 20140623

	int SettingPhighValue;
	int SettingPlowValue;
	int SettingPHLimitValue;

    int BreathMode_Queue[2];
} BREATH_SET_PARA;
//
//typedef struct
//{
//    unsigned long long Type;
//    Int8U AA_Id;  //是否是麻醉气体浓度报警标志
//    RTC_TIMER AlarmTime;
//} ALARM_RECORD;
//

typedef struct
{
	Int8U PATIENTTYPE_ChangeFlag;
	Int8U ALARM_ValUpdateFlag;
	Int8U PressureHighValue;
	Int8U PressureLowValue;
	Int8U MVHighValue;
	Int8U MVLowValue;
	Int8U RateLowValue;
	Int8U RateHighValue;
	Int16U VteHighValue;
	Int16U VteLowValue;
} ALARM_SAVEVENT_PARA;

typedef struct
{
    int RateTotalValue;
    int VtiValue;
    int VteValue;
    int MinuteVolumeValue;
    int PeepValue;
    int PpeakValue;
    int PmeanValue;
    int PplatValue;
    int FiO2Value;
    int ResistValue;
    int ComplianceValue;
    int MVspnValue;
    int fspnValue;
    int PminValue;
    int VentState;
    int EtCO2Value;
    int InCO2Value;
    int SpO2Value;
    int PulseRate;
    int FlowMeterAir;
    int FlowMeterO2;
    int FlowMeterN2O;
    int FlowMeterTotal;
    int IEValue; //  20130716
	int PRVC_VT_LOW_Flag;
	int ABSORBER_Flag;

} MONITOR_PARA;
//
typedef struct
{
    int Pressure;
    int Flow;
    int Volume;
    int BatteryVoltage;
    int AirGasPressure;
    int O2GasPressure;
    int EtCO2;
    int SpO2;
} REALTIME_PARA;
//
typedef struct
{
    int PressureHighValue;
    int PressureLowValue;
    int MVHighValue;
    int MVLowValue;
    int RateLowValue;
    int RateHighValue;
    int FiO2HighValue;
    int FiO2LowValue;
    int VteHighValue;
    int VteLowValue;
    int ApneaTimerValue;
    int AlarmVolumeValue;
    int SpO2HighValue;
    int SpO2LowValue;
    int EtCO2HighValue;
    int EtCO2LowValue;
    int FiCO2HighValue;  //  20130607
    int FiCO2LowValue;   //  20130607
    int PulseHighValue;
    int PulseLowValue;
	int N2OHighValue;
    int N2OLowValue;
    int EtAAHighValue;
    int EtAALowValue;
    int FiAAHighValue;
    int FiAALowValue;
} ALARM_SET_PARA;
//  20130328
typedef struct
{
    char MainVersion;
    char SubVersion;  
    char DebugVersion;//djq add 20160826
	/*zy 修改：添加测试版本号*/
	char TestVersion;
}VERSION_PARA;

typedef struct//add ylk 20171101
{
	char co2_unit[5];
	float co2max;
	float co2step;
	char  co2_max[5];
}CO2_S;

typedef struct
{
    unsigned char PhaseInISA_Flag;
    unsigned char PhaseInISA_State;//计时完成标志
    unsigned short PhaseInISA_Count;
}PhaseInISA_InitTypedef;


typedef struct
{
    unsigned char ChinaCo2_Flag;
    unsigned char ChinaCo2_State;
    unsigned short ChinaCo2_Count;
}CHINACO2_InitTypedef;

typedef struct //sun add 20151102
{
	int FiO2;
	int EtO2;
	int EtN2O;
	int InN2O;
	int EtAX1;
	int InAX1;
	int EtAX2;
	int InAX2;
	int MAC;
	int N2OWaveData;
	int AX1WavaData;
	int AX2WaveData;
} AA_PARAMETERS;


extern PhaseInISA_InitTypedef PhaseInISA_Init;
extern int BreathMode_BeforeACGO;
extern int BreathMode_AfterACGO;
extern int BreathMode_AfterMANUAL;
extern int BreathMode_BeforeManual;
extern int BreathMode_BeforeConfirm;

//*//
MENU_HWIN StaticMenuSwitch(int Menu,MENU_HWIN *Menu_Obj);
INFO_HWIN StateInfoWinCreat(void);
void RealtimeParaUpdate(HOME_MENU_HWIN * hWin);
int InfoWinStateUpdate(INFO_HWIN iWin);
void MonitorDataUpdate(WM_HWIN mWin);
void MonitorDataUpdate1(WM_HWIN mWin);
void CurveDataUpdate(CURVE_HWIN *hWin,char flag,HOME_MENU_HWIN * tWin);
int _CalibrationProgressShow(int type,WM_HWIN hWin,int flag);
//void ListBoxInfoUpdate(WM_HMEM hDlg,ALARM_RECORD_LIST * list,unsigned char flag,unsigned char type);
int ParameterLimitAnalysis(ButtonExCtrl_Handle hObj,int type);
int ParameterLimitAnalysis_Normal(ButtonExCtrl_Handle hObj,int type,BREATH_SET_PARA * Parameter);
void _UpdateOperationTime(WM_HWIN hWin);
void BreathPara_Assign(BREATH_SET_PARA *Dest,const BREATH_SET_PARA *Src);
void MultiText_OtherUnitUpdate(WM_HWIN hWin, char *str);
void MultiText_UnitUpdate(WM_HWIN hWin, char *str);
void MultiText_MaxValueUpdate(WM_HWIN hWin, char *str);
void MultiText_MinValueUpdate(WM_HWIN hWin, char *str);
void MultiText_MaxValueUpdate1(WM_HWIN hWin, char *str);
void MultiText_MinValueUpdate1(WM_HWIN hWin, char *str);
void MultiText_OtherParaUpdate(WM_HWIN hWin, char *str);
void MultiText_ParaUpdate(WM_HWIN hWin, char *str);
void BreathSettingParameterWrite(u8 InitFlag,const BREATH_SET_PARA *OldPara,const BREATH_SET_PARA *NewPara);
void InitSettingOldPatientParameter(BREATH_SET_PARA * Para);
//*/
/*//cpp del 20130328
MENU_HWIN StaticMenuSwitch(int Menu,MENU_HWIN *Menu_Obj);
FixedMenu_handle  FixedMenuCreat(void);//unused cpp ac 20130328
void MainMenuCreat(CURVE_HWIN *hWin);//unused cpp ac 20130328
//void DeleteInvalidateHandle(BUTTON_EXC_HWIN *hWin);
void MonitorDataUpdate(WM_HWIN mWin);
void CurveDataUpdate(CURVE_HWIN *hWin,char flag,HOME_MENU_HWIN * tWin);
void FirstMultiPageInit(WM_HWIN hWin);//unused cpp ac 20130328
void AlarmParameterUpdate(WM_HWIN hWin);//unused cpp ac 20130328
void SettingDefaultAlarmPara(ALARM_SET_PARA * Parameter);//unused cpp ac 20130328
INFO_HWIN StateInfoWinCreat(void);
int InfoWinStateUpdate(INFO_HWIN iWin);
static void ClearWorkArea(void);//unused cpp ac 20130328
//*/
#endif
