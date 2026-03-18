#ifndef MainTask_H
#define MainTask_H

#include "WM.h"
#include "stdtypes.h"
#include "DispKeyLib.h"
#include "DispStrs.h"

#define X45     0//
#define X6      1//
#define Z6      2//
#define X45C    3
#define X45A	4

#define MACHINE_TYPE   X45
#define DEMO  //  [11/8/2022 lyy add Demo模式]
#define ISDEMO_NOTNORMAL                   1
#define TESTDEMO						   1
//----------------------------------------------
#define SIRIUSMED_LOGO     0
#define NEW_SIRIUSMED_LOGO 1
#define NEUTRAL_LOGO       2  //中性开机界面
#define HOSPITECH_LOGO     3
#define MS_WESTFALIA_LOGO  4  //ODM
#define SASVAT_LOGO        5
#define ADVANCED_LOGO      6
#define MSW_LOGO           7
#define GALILEA_LOGO	   8  //在标准开机界面，修改了背景色和进度条颜色。定制SpO2功能2
#define PENLON_LOGO		   9
#define ANDROMEDA		   10
#define ADVANCE_INC_LOGO   11  //jm 20210604 添加ADVANCE_Inc开机界面
#define PROMED_LOGO        12
#define RN5NO_LOGO		   13  //PT. Djuniar &Djuniar- Nesco Group 印度尼西亚  //[8/14/2023 gyh add]
#define ENDURE_LOGO		   14 //菲律宾耐久医疗定制
#define WEX35_LOGO		   15

#define Pre_ACGO	//ACGO前置	//氧浓度在ACGO下不显示
// #define MSW_FLAG	//MSW标志 定义：开机界面有msw 无定义：无msw标志   //  [7/5/2022 yaoyao add]
//#define N2O_SWITCH_FLAG		//密码进行笑气开关标志	定义：通过密码能打开关闭笑气开关	无定义：默认打开笑气开关	//  [7/5/2022 yaoyao add]

#define NEW_POWER_COMM //定义为新电源管理板 不定义为旧电源管理板
#define POWER_SWITCH_MODE		1
#define POWER_BAT_MANAGE_TYPE 	1
#define LCD_SWITCH_MODE			0

//机型相关的设置
#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A)
//#define LANGUAGE_CHINESE_FLAG          //开启后只有中文模式，关闭为英西俄模式
#define LOGO_FLAG		NEW_SIRIUSMED_LOGO  //控制logo样式

//#ifndef LANGUAGE_CHINESE_FLAG
#define SELFTEST           //定义为有开机自检   取消定义为没有开机自检
//#endif

//功能
#define AA_WAVE_SWITCH
#define ACGO_SWITCH          //定义为开启ACGO模式   取消定义为关闭ACGO模式   (X40使用)
#define MANNUL_SWITCH        //定义为开启硬开关手动模式   取消定义为关闭硬开关手动模式  (Z5使用）

#ifdef LANGUAGE_CHINESE_FLAG
	#define __MEDIBUS
	#define GUI_MAIN_VERSION    2
	#define GUI_SUB_VERSION     0
	#define GUI_DEBUG_VERSION   6
	#define GUI_TEST_VERSION    1//测试版本
#else

#if(LOGO_FLAG == HOSPITECH_LOGO) // [31/7/2023 ylk add 基于X45生成HOSPITECH界面Z6程序，屏蔽ALARM_ABSORBER_DISCONNECT报警]
#define WAVE_PAW_ADD_ALARMHIGH
#define GUI_MAIN_VERSION    1
#define GUI_SUB_VERSION     1
#define GUI_DEBUG_VERSION   4
#define GUI_TEST_VERSION    0 
#else
#if (LOGO_FLAG != MSW_LOGO)//MSW界面下无血氧      //  [7/5/2022 yaoyao 修改]
#define SPO2_SWITCH
#endif
#define GUI_MAIN_VERSION    1
#define GUI_SUB_VERSION     0
#define GUI_DEBUG_VERSION   1
#define GUI_TEST_VERSION    2//测试版本
#endif /*LOGO_FLAG == HOSPITECH_LOGO*/

 #endif /*LANGUAGE_CHINESE_FLAG*/

#elif (MACHINE_TYPE == X45C)
//Logo
#define LOGO_FLAG   PENLON_LOGO  //控制logo样式
//#define LANGUAGE_CHINESE_FLAG          //开启后只有中文模式，关闭为英西俄模式
//功能
#define SPO2_SWITCH
#define ACGO_SWITCH          //定义为开启ACGO模式   取消定义为关闭ACGO模式   (X40使用)
#define MANNUL_SWITCH        //定义为开启硬开关手动模式   取消定义为关闭硬开关手动模式  (Z5使用）
#define GUI_MAIN_VERSION    0x01
#define GUI_SUB_VERSION     0x01
#define GUI_DEBUG_VERSION   0x04
#define GUI_TEST_VERSION    0x00//测试版本

#elif (MACHINE_TYPE == X6)
//Logo
#define LOGO_FLAG            GALILEA_LOGO//控制logo样式
#define GUI_MAIN_VERSION    0x01
#define GUI_SUB_VERSION     0x01
#define GUI_DEBUG_VERSION   0x01
#define GUI_TEST_VERSION    0x00 //测试版本为0时，为Release版本
#if (LOGO_FLAG == GALILEA_LOGO)//此logo下X6定制SPO2
#define SPO2_SWITCH
#endif

#elif (MACHINE_TYPE == Z6)
#define LOGO_FLAG            HOSPITECH_LOGO//控制logo样式
//功能
#define MANNUL_SWITCH        //定义为开启硬开关手动模式   取消定义为关闭硬开关手动模式  (Z5使用）
//#define PRVC_MODE_SWITCH      //定义后PRVC为选配模式，取消后没有这个选配
//#define WAVE_FREEZE_SWITCH   //定义为开启波形冻结功能   取消定义为关闭波形冻结功能
#define WAVE_PAW_ADD_ALARMHIGH  //定义为Paw波形添加压力报警上限直线，取消定义为不添加压力报警上限直线
#define GUI_MAIN_VERSION    0x01
#define GUI_SUB_VERSION     0x01
#define GUI_DEBUG_VERSION   0x01
#define GUI_TEST_VERSION    0x04 //测试版本为0时，为Release版本
#endif

//#define __debug
//#define  __debug_display

#define MAX_MSG_NUM 30
#define Parameter_Update_Time  490 // 500ms
#define Wave_Scan_Time          24 //22->25 //cpp md 20130424
#define Operate_Inquire_Time    44
#define Get_System_Time       1000 //1s
#define Alarm_Check_Time       100 //0.1s
#define Info_Window_Update_Time 59
#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C)
#define Get_Vent_Time           5
#endif

//
#define MEAU_UPDATE         0x01
#define PARAMETER_UPDATE    0x02
#define WAVE_SCAN           0x04
#define OPERATE_HANDLE      0x08
#define GET_SYSTEM_TIME     0x10
#define ALARM_CHECK         0x20
#define INFO_WINDOW_UPDATE  0x40
#define ACCUMULATE_RUN_TIME 0x80
#define GET_VENT_TIME       0x100

//新增系统版本号，暂时未显示
#define SYS_MAIN_VERSION    0x01
#define SYS_SUB_VERSION     0x01
#define SYS_DEBUG_VERSION   0x00
#define SYS_TEST_VERSION    0x01 //测试版本
//  20130328
//
//
//IIC实现方式
#define IICCONUSE



//
#define DELAY_TIME 5000

//软件授权码
#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C)
#define OPTIONAL_PASSWD_HEAD              ('$')//选配识别码,最高半字节
#define OPTIONAL_PASSWD_TAIL              ('8')//选配识别码,最低半字节
#else
#define OPTIONAL_PASSWD_HEAD              ('$')//选配识别码,最高半字节
#define OPTIONAL_PASSWD_TAIL              ('5')//选配识别码,最低半字节
#endif
//授权码的位置信息
#define OPTIONAL_INDEX_HEAD               (0)//选配识别码高位在数组中的位置
#define OPTIONAL_INDEX_TAIL               (7)//选配识别码低位在数组中的位置
#define OPTIONAL_INDEX_BYTE_0             (1)//最低位 <<0*4
#define OPTIONAL_INDEX_BYTE_1             (2)//       <<1*4
#define OPTIONAL_INDEX_BYTE_2             (3)//       <<2*4
#define OPTIONAL_INDEX_BYTE_3             (4)//       <<3*4
#define OPTIONAL_INDEX_BYTE_4             (5)//       <<4*4
#define OPTIONAL_INDEX_BYTE_5             (6)//       <<5*4
//选配的掩码
#define OPTIONAL_PSV_MODEFLAG			(0x000001)
#define OPTIONAL_INFANTFLAG             (0x000002)
#define OPTIONAL_ETCO2FLAG              (0x000004)
#define OPTIONAL_SPO2FLAG               (0x000008)
#define OPTIONAL_MASIMOETCO2FLAG        (0x000010)
#define OPTIONAL_MASIMOSPO2FLAG         (0x000020)
#define OPTIONAL_DRIVERGASFLAG          (0x000040)//djq add 20170608驱动气体选配
#define OPTIONAL_PRVC_MODEFLAG			(0x000080)
#define OPTIONAL_MASIMOISAO2FLAG        (0x000100)//Masimo ISA + O2
#define OPTIONAL_MASIMOISAFLAG          (0x000200)//Masimo ISA
#define OPTIONAL_FIO2SENSOR_MODEFLAG    (0x000400)//FiO2 国标下开氧浓度选配
#define OPTIONAL_KM7002_MODEFLAG		(0x000800)//金嘉信CO2

#define OPTIONAL_DRAGER_AA_FLAG         (0x001000)//Drager		no		Paramagnetic O2
#define OPTIONAL_DRAGER_AA_O2_FLAG      (0x002000)//Drager		with	Paramagnetic O2

#define OPTIONAL_VS_MODEFLAG			(0x004000)
#define OPTIONAL_APRV_MODEFLAG			(0x008000)
#define OPTIONAL_FLOWPAUSE_FLAG			(0x010000)
#define OPTIONAL_LUNGREC_FLAG			(0x020000)

#define GUI_VERSION_MAIN    GUI_MAIN_VERSION
#define GUI_VERSION_SUB     GUI_SUB_VERSION
#define GUI_VERSION_DEBUG   GUI_DEBUG_VERSION
#define GUI_VERSION_TEST    GUI_TEST_VERSION

#include "FRAM_Addr_cfg.h"

typedef struct   //  20130616
{
	int  InspTime;
	int  ExpTime;
} VENT_TIME;

enum AA_MODULE_STATE
{
	No_Connect = 0,
	Connect_Valid,
	AllPara_Not_Valid
}; //ylk add 20181128
typedef struct
{
	unsigned char   InitFinishFlag;//初始化完成标志
	unsigned char   ETCO2_FLAG; //1:波特率38400模块     0:波特率9600/19200模块     2013-12-23
	unsigned char   INIT_AA_MODULE_FLAG;//1:PhaseIn,2:Drager
	unsigned char   EtCO2ModuleExist;//djq add 2013-12-23
	unsigned char   PhaseInTypeModuleExist;
	unsigned char   Comm_Count; //0~255，成功接收一次加一，200ms未接受减一，上限为50，即10s未接收，断开标志置1
	enum AA_MODULE_STATE  Module_State_Flag; //0：未连接，1：正常，2：所有参数无效
}AA_MODULE_FLAG_REG; //ylk add 20181128

//
VENT_TIME GetVentTime(int VentState);  //  20130616
//float GetIERatio(VENT_TIME _VentTime); //  20130616 //cpp del 20130716
int GetIERatio(VENT_TIME _VentTime); //  20130716

//
// unsigned int ReadLanguageType(void);
unsigned int ReadEtCO2Unit(void);
//  20130328
int Rd_Battery_Ad(void);
//void Wr_Battery_Ad(void);
void Wr_Battery_Ad(int BatteryAdValue);
void Set_Threth_Alarm_Battery(void);
void Rd_Threth_Alarm_Battery(void);
void Battery_Data_Init(void);
void InitLanguage();

extern AA_MODULE_FLAG_REG AA_Module_Flag;

#endif
