#include "Static_Menu_Creat.h"
#include "MainTask.h"
#include "arm_comm.h"

//#define Int8U unsigned char
//#define Int16U unsigned short int
//#define Int16S signed short int
//#define Int32U unsigned int
//#define Int64U unsigned long long

#define SPO2_SWITCH_ON    1
#define ETCO2_SWITCH_ON   1
//
#define MAX_RECORD_NUM                1000 
#define ALARM_FRO                     523
#define GAS_LOW_PRESSURE_LIMIT        280 //kPa
#define GAS_MEASURE_TIME              50
#define PEEP_MEASURE_TIME             150
//
#define PAW_N10                       -10 // 气道压力-10cmH2O
//#define FIO2_18P                      18  // 氧浓度18%
//氧浓度监测范围    0≤未连接＜5    6≤故障＜15 ||  105≤故障//djq modify 20161123
// #define FIO2_DISCONNECT_MAX           5
// #define FIO2_DISCONNECT_MIN           0
// #define FIO2_FAIL_MAX                 105
// #define FIO2_FAIL_MIN                 15  
// #define O2_SENSOR_FAILURE_COFIRM(O2Value) ((O2Value < FIO2_FAIL_MIN)  || (O2Value >= FIO2_FAIL_MAX))
// #define O2_SENSOR_FAILURE_CANCEL(O2Value) ((O2Value >= FIO2_FAIL_MIN) && (O2Value < FIO2_FAIL_MAX))

#define FIO2_FAULT_MIN		0
#define FIO2_FAULT_MAX		5
#define FIO2_FAIL_MIN		15
#define FIO2_FAIL_MAX		110

////0-5  //ylk add 20190124
//#define O2_FAULT_COFIRM(O2Value)		( (O2Value >= FIO2_FAULT_MIN) && (O2Value < FIO2_FAULT_MAX))
////>=5
//#define O2_FAULT_CANCEL(O2Value)		( (O2Value >= FIO2_FAULT_MAX))
////(5-15)||(105)
//#define O2_NEEDCAL_COFIRM(O2Value)		( (O2Value >= FIO2_FAULT_MAX && O2Value < FIO2_FAIL_MIN) || (O2Value > FIO2_FAIL_MAX))
////(>=15) && (<=105)
//#define O2_NEEDCAL_CANCEL(O2Value)		( (O2Value >= FIO2_FAIL_MIN) && (O2Value <= FIO2_FAIL_MAX))

#define O2_FAULT_COFIRM(O2Value)		(O2Value <= 18)
#define O2_FAULT_CANCEL(O2Value)		(O2Value > 18)
#define O2_Sensor_Disp100(O2Value)		(O2Value >= 100 && O2Value <= FIO2_FAIL_MAX)
#define O2_Sensor_Disp_(O2Value)		(O2Value > FIO2_FAIL_MAX || O2Value < FIO2_FAIL_MIN)
//
#define ALARM_NEW                     0x11
#define ALARM_RECOVER                 0x22
#define ALARM_RESET                   0x33
#define ALARM_END                     0x44
#define ALARM_RESET_INQUIRE           0x55 //报警复位查询，查询当前是否可以报警复位
#define LOW_PRIORITY                  4
#define MEDIUM_PRIORITY               2
#define HIGH_PRIORITY                 1
//
#define HIGHEST_ALARM							((Int64U)0x0000000000000001)
//
#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C)
#define ALARM_NUM	               39		//高级报警个数  
#define ALARM_NUM1                 47		//中级和低级报警个数 
#define ALARM_NUM1_MEDIUM          37
#else
#define ALARM_NUM	               21		//djq add 20131226  报警项目总数
#define ALARM_NUM1                 14		//中级和低级报警个数
#endif

//
#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C)   //sun add 20140710
#define	ALARM_OXY_SOURCE_LOW					((Int64U)0x0000000000000001)//0
#define ALARM_BDU_COMM_FAULT			        ((Int64U)0x0000000000000002)//1 BDU通讯故障
#define ALARM_BATTERY_FAULT_HIGH                ((Int64U)0x0000000000000004)//2 电池故障
// #define	ALARM_BATTERY_FAIL						((Int64U)0x0000000000000008)//电池耗尽
#define	ALARM_APNEA								((Int64U)0x0000000000000010)
#define	ALARM_PAW_HIGH_CONTINUE					((Int64U)0x0000000000000020)
#define	ALARM_PAW_HIGH							((Int64U)0x0000000000000040)
#define	ALARM_PATIENT_DISCONNECT				((Int64U)0x0000000000000080)
#define	ALARM_FIAA_HIGH							((Int64U)0x0000000000000100)
#define	ALARM_ETAA_HIGH							((Int64U)0x0000000000000200)
#define	ALARM_MV_LOW                     		((Int64U)0x0000000000000400)
#define	ALARM_MV_HIGH							((Int64U)0x0000000000000800)
#define	ALARM_RATE_LOW                 			((Int64U)0x0000000000001000)
#define	ALARM_FIO2_LESSTHAN_18                 	((Int64U)0x0000000000002000)//20210729 add FiO2 < 18% X45A
#define	ALARM_FIO2_HIGH							((Int64U)0x0000000000004000)
#define	ALARM_O2SENSOR_FAILURE					((Int64U)0x0000000000008000)
#define	ALARM_FIO2_LOW							((Int64U)0x0000000000010000)
#define	ALARM_ETCO2_HIGH						((Int64U)0x0000000000020000)
#define	ALARM_FICO2_HIGH						((Int64U)0x0000000000040000)
#define	ALARM_FIN2O_HIGH						((Int64U)0x0000000000080000)
#define	ALARM_STANDBY_ENTER						((Int64U)0x0000000000100000)
#define	ALARM_PAW_LESSTHAN_N10 					((Int64U)0x0000000000200000)
#define	ALARM_PAW_LOW							((Int64U)0x0000000000400000)
#define	ALARM_BATTERY_LOW						((Int64U)0x0000000000800000)//
#define ALARM_MASIMO_SPO2_FAILURE				((Int64U)0x0000000001000000)   //masimo
#define ALARM_MASIMO_LOW_PERFUSION				((Int64U)0x0000000002000000)   //masimo
#define ALARM_MASIMO_DFECTIVE_SENSOR			((Int64U)0x0000000004000000)   //masimo
#define ALARM_MASIMO_UNRECOGNIZED_SENSOR		((Int64U)0x0000000008000000)   //masimo
#define ALARM_MASIMO_LOW_SIGNAL_IQ				((Int64U)0x0000000010000000)   //masimo
#define ALARM_MASIMO_PI_LOW						((Int64U)0x0000000020000000)   //masimo
#define	ALARM_SPO2_LOW							((Int64U)0x0000000040000000)
#define	ALARM_PULSE_RATE_LOW					((Int64U)0x0000000080000000)
#define	ALARM_VTE_HIGH							((Int64U)0x0000000100000000)//
#define ALARM_DRAGER_WATRAPFULL					((Int64U)0x0000000200000000)      //Drager
#define ALARM_DRAGER_WATRAP_OR_LINE_EXCP		((Int64U)0x0000000400000000)      //Drager
#define ALARM_DRAGER_SUPVOLBAD					((Int64U)0x0000000800000000)      //Drager
#define ALARM_DRAGER_CO2FAIL					((Int64U)0x0000001000000000)      //Drager
#define ALARM_DRAGER_O2FAIL					    ((Int64U)0x0000002000000000)      //Drager
#define ALARM_DRAGER_MODULEFAIL					((Int64U)0x0000004000000000)//    //Drager
//中级报警
#define	ALARM_AIR_SOURCE_LOW	                ((Int64U)0x0000000000000001)//0
#define	ALARM_ABSORBER_DISCONNECT	            ((Int64U)0x0000000000000002)//1
#define ALAEM_PRVC_VT_LOW						((Int64U)0x0000000000000004)
#define	ALARM_VTE_LOW	                        ((Int64U)0x0000000000000008)
#define	ALARM_RATE_HIGH	                        ((Int64U)0x0000000000000010)
#define	ALARM_PULSE_RATE_HIGH	                ((Int64U)0x0000000000000020)//5
#define	ALARM_PHASEIN_SW_ERR     				((Int64U)0x0000000000000040)
#define	ALARM_PHASEIN_HW_ERR     				((Int64U)0x0000000000000080)
#define	ALARM_PHASEIN_MOTOR_ERR					((Int64U)0x0000000000000100)
#define	ALARM_PHASEIN_UN_CAL					((Int64U)0x0000000000000200)
#define	ALARM_GAS_REPL_ADP						((Int64U)0x0000000000000400)
#define	ALARM_PHASEIN_NO_ADP					((Int64U)0x0000000000000800)
#define ALARM_PHASEIN_NO_TUBE					((Int64U)0x0000000000001000)
#define	ALARM_PHASEIN_CO2_OR					((Int64U)0x0000000000002000)
#define	ALARM_PHASEIN_N2O_OR					((Int64U)0x0000000000004000)
#define	ALARM_PHASEIN_AX_OR						((Int64U)0x0000000000008000)
#define ALARM_PHASEIN_O2_OR						((Int64U)0x0000000000010000)
#define	ALARM_PHASEIN_TEMP_OR					((Int64U)0x0000000000020000)
#define	ALARM_PHASEIN_PRESS_OR					((Int64U)0x0000000000040000)
#define	ALARM_PHASEIN_AGENTID_UNREAL			((Int64U)0x0000000000080000)
#define ALARM_PHASEIN_O2CALIB					((Int64U)0x0000000000100000)
#define ALARM_PHASEIN_O2SEN_ERR					((Int64U)0x0000000000200000)//ISA O2 Sensor error
#define ALARM_PHASEIN_REPL_O2					((Int64U)0x0000000000400000)//ISA Replace O2 Sensor
#define ALARM_MASIMO_INTERFERNCE				((Int64U)0x0000000000800000)   //masimo
#define ALARM_MASIMO_AMBIENT_LIGHT				((Int64U)0x0000000001000000)   //masimo
#define ALARM_MASIMO_NO_CABLE					((Int64U)0x0000000002000000)   //masimo
#define	ALARM_ETCO2_LINE_OCCLUSION	            ((Int64U)0x0000000004000000)
#define ALARM_DRAGER_WATRAPWILLFULL 		    ((Int64U)0x0000000008000000)   //脱水器将满
#define ALARM_MASIMO_PI_HIGH					((Int64U)0x0000000010000000)   //masimo
#define	ALARM_SPO2_HIGH	                        ((Int64U)0x0000000020000000)
#define	ALARM_ETCO2_LOW	                        ((Int64U)0x0000000040000000)
#define	ALARM_FIAA_LOW	                        ((Int64U)0x0000000080000000)
#define	ALARM_ETAA_LOW	                        ((Int64U)0x0000000100000000)
#define	ALARM_FIN2O_LOW	                        ((Int64U)0x0000000200000000)
#define	ALARM_AA_GTE_3MAC	                    ((Int64U)0x0000000400000000)
#define	ALARM_PHASEIN_ZERO_OR	                ((Int64U)0x0000000800000000)
#define ALARM_AAOVERFLOW	                    ((Int64U)0x0000001000000000)//
//低级报警
#define ALARM_BATTERY_FAULT_LOW					((Int64U)0x0000002000000000)// 电池故障 //低级
#define	ALARM_AA_LESSTHAN_3MAC 	                ((Int64U)0x0000004000000000)//
#define ALARM_GAS_COMM_FAULT							((Int64U)0x0000008000000000)//40
#define	ALARM_AC_FAIL												((Int64U)0x0000010000000000)
#define	ALARM_SPO2_SENSOR_DISCONNECT	    ((Int64U)0x0000020000000000)
#define ALARM_MASIMO_NO_PATIENT					((Int64U)0x0000040000000000)   //masimo
#define	ALARM_ETCO2_MODULE_OFF	                ((Int64U)0x0000080000000000)
#define	ALARM_ACGO_MODE_OPEN						((Int64U)0x0000100000000000)
#define	ALARM_O2_MODULE_OFF							((Int64U)0x0000200000000000)
#define	ALARM_SPO2_MODULE_OFF						((Int64U)0x0000400000000000)//

//非报警锁存项 低级报警增加!!!
#define ALARM_NOTLATCH	( ALARM_BATTERY_FAULT_LOW           \
                        | ALARM_AA_LESSTHAN_3MAC			\
						|ALARM_GAS_COMM_FAULT			\
						| ALARM_AC_FAIL						\
						| ALARM_SPO2_SENSOR_DISCONNECT		\
						| ALARM_MASIMO_NO_PATIENT			\
						| ALARM_ETCO2_MODULE_OFF			\
						| ALARM_ACGO_MODE_OPEN				\
						| ALARM_O2_MODULE_OFF				\
						| ALARM_SPO2_MODULE_OFF	)

#else  //X6
#define ALARM_OXY_SOURCE_LOW					((Int64U)0x0000000000000001)   //0   //高级
#define ALARM_BATTERY_FAIL						((Int64U)0x0000000000000002)   
#define ALARM_APNEA								((Int64U)0x0000000000000004)
#define ALARM_PAW_HIGH							((Int64U)0x0000000000000008)
#define ALARM_PATIENT_DISCONNECT				((Int64U)0x0000000000000010)
#define ALARM_MV_LOW							((Int64U)0x0000000000000020)
#define ALARM_MV_HIGH							((Int64U)0x0000000000000040)
#define ALARM_PAW_HIGH_CONTINUE					((Int64U)0x0000000000000080)
#define ALARM_RATE_LOW							((Int64U)0x0000000000000100)
#define ALARM_FIO2_HIGH							((Int64U)0x0000000000000200)
#define ALARM_O2SENSOR_FAILURE					((Int64U)0x0000000000000400)
#define ALARM_FIO2_LOW							((Int64U)0x0000000000000800)
#define ALARM_ETCO2_HIGH						((Int64U)0x0000000000001000)
#define ALARM_FICO2_HIGH						((Int64U)0x0000000000002000)
#define ALARM_STANDBY_ENTER						((Int64U)0x0000000000004000)
#define ALARM_PAW_LESSTHAN_N10					((Int64U)0x0000000000008000)
#define ALARM_PAW_LOW							((Int64U)0x0000000000010000)
#define ALARM_BATTERY_LOW						((Int64U)0x0000000000020000)
#define ALARM_SPO2_LOW							((Int64U)0x0000000000040000)
#define ALARM_PULSE_RATE_LOW					((Int64U)0x0000000000080000)
#define ALARM_VTE_HIGH							((Int64U)0x0000000000100000)   //20
//中级
#define ALAEM_PRVC_VT_LOW						((Int64U)0x0000000000000001)	//0  
#define ALARM_VTE_LOW							((Int64U)0x0000000000000002)    
#define ALARM_RATE_HIGH							((Int64U)0x0000000000000004)
#define ALARM_PULSE_RATE_HIGH					((Int64U)0x0000000000000008)
#define ALARM_ETCO2_LINE_OCCLUSION				((Int64U)0x0000000000000010)
#define ALARM_SPO2_HIGH							((Int64U)0x0000000000000020)
#define ALARM_ETCO2_LOW							((Int64U)0x0000000000000040)
#define ALARM_FICO2_LOW							((Int64U)0x0000000000000080)
//低级
#define ALARM_AC_FAIL							((Int64U)0x0000000000000100)
#define ALARM_SPO2_SENSOR_DISCONNECT			((Int64U)0x0000000000000200)
#define ALARM_ETCO2_MODULE_OFF					((Int64U)0x0000000000000400)
#define ALARM_ACGO_MODE_OPEN					((Int64U)0x0000000000000800)
#define ALARM_O2_MODULE_OFF						((Int64U)0x0000000000001000)
#define ALARM_SPO2_MODULE_OFF					((Int64U)0x0000000000002000)   //13

//非报警锁存项 低级报警增加!!!
#define ALARM_NOTLATCH	(ALARM_AC_FAIL					\
						|ALARM_SPO2_SENSOR_DISCONNECT	\
						|ALARM_ETCO2_MODULE_OFF			\
						|ALARM_ACGO_MODE_OPEN			\
						|ALARM_O2_MODULE_OFF			\
						|ALARM_SPO2_MODULE_OFF)
#endif


#define BATTERY_SWITCH_ADDR            0x4A0
#define BATTERY_SWITCH_1_ADDR          0x4A1




//
#define RECORD_SIZE    8 //7
//
#ifndef __alarm_handle_h
#define __alarm_handle_h
typedef struct
{
	Int64U Type;
  Int8U  Grade;
  char * Content;
  Int8U  ActiveState;
}ALARM_INFO;
//
typedef struct
{
    Int64U presentAlarms;
    Int64U newAlarms;
    Int64U unclearedAlarms;
	Int64U presentAlarms1;
	Int64U newAlarms1;
	Int64U unclearedAlarms1;
    Int8U  BeepState;
    Int8U  MuteTimer;
	Int8U  ActiveNum;
} ALARM_WORK;
//
typedef enum
{
	Year_Low_Addr,Year_High_Addr,Mon_Addr,Day_Addr,Hour_Addr,Min_Addr,
	Type_Addr,AA_Id_Addr,EGM_Err_Low_Addr,EGM_Err_High_Addr,
}Alarm_EEPROM_Addr_ENUM;
typedef enum 
{
	RankType_Active_Rise,RankType_Time_Rise,RankType_Priority_Rise,
	RankType_Active_Fall,RankType_Time_Fall,RankType_Priority_Fall,
}RankType_ENUM;
typedef enum 
{
	Lock,UnLock,
}LockType_ENUM;

typedef struct
{
    Int16U Head;
    Int16U Rear;
    Int16U Count;
} ALARM_RECORD_LIST;
//
typedef struct
{
    Int8U TestState;
    Int8U SpeakerVolume;
} SPEAKER_TEST;
//
typedef struct
{
    Int32U Hour;
    Int8U  Min;
} OPERATION_TIME;

struct list_node
{//add 2013-12-16
	Int8U  StateFlag;//最高位Active标志：0 非激活状态   1 激活状态  ,次高位保留 ,0~5位：1分钟内报警按时间的排序

	RTC_TIMER RecordTime;//报警存储时间
	Int8U  AlarmType;    //报警类型
	Int8U  AlarmAA_ID;   //如果当前为麻醉气体浓度报警，则存储AA的类型。最高位为0则是高级报警，为1是低级报警。

	struct list_node *next;
};

struct list_head{//add 2013-12-16
    struct list_node *head;
	RankType_ENUM RankType;//djq add 20170111  排序条件从报警信息进入报警日志，默认为RankType_Active，正常进入默认为RankType_Time
	LockType_ENUM LockType;//djq add 20170112  排序、插入、删除过程中都需要进行Lock操作。
};  
typedef struct
{
    unsigned short Battery_Num;   //电池校验存储的点数，每个点1分钟

    unsigned short Battery_Step3; //   
    unsigned short Battery_Step2; //   
    unsigned short Battery_Low;   // 
    unsigned short Battery_Empty; //   
    unsigned short Battery_Disconnect;//低于Battery_Disconnect则显示未连接
    RTC_TIMER      BatteryCali_Time;   //上次校验的时间
}BATTERY_AD_PARA;
//
extern ALARM_WORK AlarmsWork;
void AlarmCheckContinuePressure(int pressure);
void AlarmCheckPawN10(int pressure,int VentFlag);
void ClearParameterAlarm(void);
void ClearAcgoModeOtherAlarm(void);
void AA_Module_String(void);
void AA_Module_Alarm_Progress(void);

int InquireAlarmInfoWindows(void);
ALARM_INFO GetHighestAlarm(Int64U Alarms,Int64U Alarms1);
unsigned int GetAlarmNum(Int64U Alarms,Int64U Alarms1);
#endif
