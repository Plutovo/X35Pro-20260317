#ifndef __AA_MODULE_H    
#define __AA_MODULE_H    

/*  #include directives go here */
#include "co2_irma.h"
#include "Drager.h"
#include "filter.h"

/*  #define directives go here  */
#define AA_MONITOR_STOPCONTTRANS        0x0001
#define AA_MONITOR_INITZERO             0x0002
#define AA_MONITOR_SWITCHPUMPS          0x0004
#define AA_MONITOR_SWITCHVALVES         0x0008
#define AA_MONITOR_SETOPMODE            0x0010
#define AA_MONITOR_SETAAGAS             0x0020
#define AA_MONITOR_SETEXPARA            0x0040
#define AA_MONITOR_SETUNIT              0x0080
#define AA_MONITOR_SETBREATHDETECTION   0x0100
#define AA_MONITOR_SETPATIENTDATA       0x0200
#define AA_MONITOR_GETDEVINFO           0x0400
#define AA_MONITOR_SETIRO2DELAY         0x0800
#define AA_MONITOR_SETO2OPTION          0x1000
#define AA_MONITOR_SETO2SENSORTYPE      0x2000
#define AA_MONITOR_SPANCAL              0x4000
#define AA_MONITOR_SPANCAL21            0x8000

#define AA_TASK_REGISTER AA_all_data.AA_state.module_task_register

extern void SendOperationMessage(Int8U type,Int8U UsartPort);

/*  declarations of external variables go here  */
enum AA_ModuleName
{
    None = 0,
    PhaseIn,
    Drager,
};

enum AA_OPMODE
{
    AA_MODE_NONE = 0,
    AA_MODE_SLFTST,         //模块自检
    AA_MODE_STANDBY,        //模块睡眠
    AA_MODE_MEASURE,        //模块正常
    AA_MODE_CHANGETOMEASURE,//切换为工作
    AA_MODE_CHANGETOSTANDBY,//切换为待机
    AA_MODE_OFF,            //子部件关
    AA_MODE_CLOSING,        //关闭
    AA_MODE_DEMO
};

enum AA_AGENT
{
    AA_NO_AGT = 0,          //0:无麻醉气体
    AA_AGT_HAL,             //1:氟烷
    AA_AGT_ENF,             //2:安氟醚
    AA_AGT_ISO,             //3:异氟醚
    AA_AGT_SEV,             //4:七氟醚
    AA_AGT_DES              //5:地氟醚
};

enum AA_GAS
{
    AA_GAS_CO2,
    AA_GAS_N2O,
    AA_GAS_AA1,
    AA_GAS_AA2,
    AA_GAS_O2
};

enum AA_DETAIL_PARA         //详细参数
{
    AA_DETAIL_CO2,
    AA_DETAIL_N2O,
    AA_DETAIL_AA1,
    AA_DETAIL_AA2,
    AA_DETAIL_O2,
    AA_DETAIL_COMPN,
    AA_DETAIL_PRESSURE,
    AA_DETAIL_BREATH
};

enum AA_GAS_ID              //气体ID
{
    AA_ID_NONE = 0,             //未识别
    AA_ID_HAL,              //氟烷
    AA_ID_ENF,              //安氟醚
    AA_ID_ISO,              //异氟醚
    AA_ID_SEV,              //七氟醚
    AA_ID_DES,              //地氟醚
    AA_ID_NOT_AVAIL,        //不可用
    AA_ID_CO2,              //CO2
    AA_ID_N2O,              //笑气
    AA_ID_O2,               //O2
    AA_ID_OTHER             //其他
};

enum AA_DEVINFO_TYPE
{
    AA_DEV_vendor_code = 0,
    AA_DEV_serial_num,
    AA_DEV_hw_ver,
    AA_DEV_sw_ver,
    AA_DEV_type_num
};

enum AA_PARA_STATUS         //参数状态
{
    PAS_None = 0,
    PAS_Not_Available,
    PAS_Low_Concentration,
    PAS_Identified,         //识别
    PAS_Calculate,
    PAS_Overflow,
    PAS_Mixture,
    PAS_Forced,
    PAS_Estimated
};   
    
enum AA_PARA_MODE           //参数模式
{
    PS_None = 0,
    PS_Valid,
    PS_Overflow,
    PS_Invalid,
    PS_Not_Available
};
    
enum AA_ZEROSTATE           //参数调零状态
{
    ZSP_None = 0,
    ZSP_No_In_Progress,
    ZSP_In_Progress
};

enum AA_MODULE_ZEROSTATE    //模块零点状态
{
    zero_none = 0,
    zero_zeroing,
    zero_request,
    disable,
    zero_ok,
    zero_fail
};
enum AA_MODULE_SPANSTATE
{
	SPAN_NoInProg=0,
	SPAN_InProg
};

enum AA_ZERORESULT          //参数调零结果
{
    ZSF_None = 0,
    ZSF_Successful,
    ZSF_Failed
};

enum AA_ACCURACY            //参数精度
{
    ACCURACY_None = 0,
    ACCURACY_Startup,
    ACCURACY_Unknow,
    ACCURACY_Reduced,
    ACCURACY_Iso
};

enum AA_UNIT
{
    PSUS_Default = 0,   //ATS 气体单位% 精度0.01
    PSUS_Additional,    //ATPS 气体单位mmHg 精度0.1
};

enum AA_BREN
{
    BREN_None = 0,
    BREN_Detected,
    BREN_NoDetected
};

enum AA_BREATHMODE
{
    BR_mode_None = 0,
    BR_mode_Valid,
    BR_mode_Auto_wake_up
};

enum AA_BREATHPHASE
{
    BRphase_None = 0,
    BRphase_Insp,
    BRphase_Exp
};

enum AA_APNEA
{
    APNEA_None = 0,
    APNEA_No_Occured,
    APNEA_Occured
};

enum AA_CMD
{
    AA_CMD_None = 0,
    AA_CMD_SetOpMode,
    AA_CMD_AA_SetAA,
    AA_CMD_InitZero,            //无参数
    AA_CMD_Setpara,
    AA_CMD_SetPatientData,      //无参数
    AA_CMD_SetParaUnit,
    AA_CMD_SetBreathDetection,
    AA_CMD_SwitchValves,
    AA_CMD_SwitchPumps,
    AA_CMD_StopContinueTrans,    //无参数
    AA_CMD_GetDevInfo,
	AA_CMD_SetIRO2Delay,
	AA_CMD_SetO2Option,
	AA_CMD_SetO2SensorType,
	AA_CMD_SpanCal,
    AA_CMD_SpanCal21
};

enum AA_VALVE_STATE
{
    AA_valve_unknow = 0,
    AA_valve_sample_gas,
    AA_valve_room
};

enum AA_PUMP_STATE
{
    AA_pump_unknow = 0,
    AA_pump_Off,
    AA_pump_high,
    AA_pump_purge
};

enum  AA_MODULE_TYPE   
{
    //PhaseIn
    AA_PHASEIN_UNKNOW   = 0x00, //0=未知/保留
    AA_PHASEIN_CO2ONLY  = 0x01, //只能监测CO2气体(黄色外壳)
    AA_PHASEIN_ICU      = 0x02, //ICU专用,监测CO2, INSP/EXP O2, 无N2O(黄色外壳)
    AA_PHASEIN_AX       = 0x03, //5种麻醉气体非自动识别,N2O,无O2 (蓝色外壳)
    AA_PHASEIN_OR       = 0x04, //5种麻醉气体非自动识别,N2O,有O2,(蓝色外壳)
    AA_PHASEIN_AXAUTO   = 0x05, //5种麻醉气体自动识别,N2O,无O2(红色外壳)
    AA_PHASEIN_ORAUTO   = 0x06, //5种麻醉气体自动识别,N2O,有O2(红色外壳)
    //旁流模块
    AA_PHASEIN_ISA_CO2 = 0x11,   //旁流CO2或ICU,(黄色外壳)
    AA_PHASEIN_ISA_AX  = 0x12,   //旁流5种麻醉气体非自动识别(或带O2)
    AA_PHASEIN_ISA_AXAUTO = 0x13,//旁流5种麻醉气体自动识别无O2
    AA_PHASEIN_ISA_ORAUTO = 0x14,//旁流5种麻醉气体自动识别带O2
    //Drager
    AA_DRAGER_UNKNOW    = 0x21,  //未知
    AA_DRAGER_NONE      = 0x22,  //裸板
    AA_DRAGER_O2_DUAL   = 0x23,  //可监测两种混合麻醉气体自动识别，可监测O2
    AA_DRAGER_DUAL      = 0x24,  //可监测两种混合麻醉气体自动识别
    AA_DRAGER_O2_SINGLE = 0x25,  //可监测一种麻醉气体手动设置，可监测O2
    AA_DRAGER_SINGLE    = 0x26   //可监测一种麻醉气体手动设置
};

typedef enum 
{
	Sensor_O2Bat=0,
	Sensor_O2Spring
}O2SENSOR_Type;

typedef enum 
{
	CAL_CO2=0x65,
	CAL_N2O,
	CAL_AA,
	CAL_O2
}GAS_SPANCAL_Type;

struct AA_CMD_PARA
{
    enum AA_OPMODE CMD_OpMode;
    enum AA_GAS CMD_SETAA_AAx;
    enum AA_AGENT CMD_SETAA_id;
    enum AA_GAS CMD_SETPARA_valuemask;
    Int16U CMD_SETPARA_value;
    Int16U CMD_SETUNIT_mask;
    Int8U CMD_SETUNIT_unit;
    enum AA_BREATHMODE CMD_SETBD_BDmode;
    Int8U CMD_SWITCH_valve;
    enum AA_PUMP_STATE CMD_SWITCH_pump;
    enum AA_DEVINFO_TYPE CMD_Dev_info;
    Int8U CMD_IRO2_DELAY;
    Int8U CMD_SETO2_OPTION;
    O2SENSOR_Type CMD_SETO2SENSOR_TYPE;
    GAS_SPANCAL_Type CMD_SPAN_CAL;
    Int8U CMD_SPAN_CALValue;
};

struct AA_GAS_DATA
{
    Int16U FI;
    Int16U ET;
    Int16U Momt;
    enum AA_PARA_STATUS ParaState;
    enum AA_GAS_ID ID;
    enum AA_PARA_MODE ParaMode;
    enum AA_ZEROSTATE ZeroState;
    enum AA_ZERORESULT ZeroResult;
    enum AA_ACCURACY Accuracy;
    enum AA_UNIT Unit;
};
   
struct AA_RT_DATA
{
    Int16U CO2value;
    Int16U N2Ovalue;
    Int16U AA1value;
    Int16U AA2value;
    Int16U O2value;
    enum AA_AGENT AA1_ID;
    enum AA_AGENT AA2_ID;
};

struct AA_COMPN
{
    Int8U o2_compn;
    Int8U n2o_compn;
};

struct AA_PRESSURE
{
    Int16U atmp;
    int atmp_cuve;
    enum AA_ACCURACY atmp_accuracy;
};

struct AA_BREATHDATA
{
    Int8U breath_rate;
    enum AA_BREATHMODE detection_mode;
    enum AA_BREN detection_enable;
    Int8U lastbr_time;
    Int8U nobr_timeout;
    enum AA_APNEA apnea_open;
    Int16U apnea_duration;
    enum AA_BREATHPHASE breath_phase;
};


struct AA_MODULESTATE
{
    enum AA_OPMODE currect_mode;
    enum AA_MODULE_ZEROSTATE zero_state;
    enum AA_VALVE_STATE valve_state;
    enum AA_PUMP_STATE pump_state;
	enum AA_MODULE_SPANSTATE span_state;
};

struct AA_DEVICEINFO
{
    enum AA_ModuleName AA_name; //模块名称
    volatile Int8U vendor_code[5];         //供应商代码
    volatile Int8U serial_num[9];          //序列号
    volatile Int8U hw_version[2];          //硬件版本
    volatile Int8U sw_version[8];          //软件版本
    volatile Int8U module_typenum[7];       //零件号码
    enum  AA_MODULE_TYPE module_type;
};

struct AA_TIMER
{
    int  timer_ms;
    char timer_sec;
    char timer_min;
    char timer_hour;
};

typedef struct _AA_Wave_Data   
{
    struct AA_RT_DATA  AA_RT_data;
} AA_FAST_DATA;

typedef struct _AA_Detail_Data
{
    struct AA_GAS_DATA AA_CO2;    
    struct AA_GAS_DATA AA_N2O;    
    struct AA_GAS_DATA AA_AA1;    
    struct AA_GAS_DATA AA_AA2;    
    struct AA_GAS_DATA AA_O2;    
    struct AA_COMPN AA_Compn;
    struct AA_PRESSURE AA_Pressure;
    struct AA_BREATHDATA AA_BreathData;
    unsigned int AA1_Color;
    char AA1_Type_Text[20];//4
    unsigned int AA2_Color;
    char AA2_Type_Text[20];//4
} AA_DETAIL_DATA;

typedef struct _AA_Device_State
{
    struct AA_DEVICEINFO dev_info;
    struct AA_MODULESTATE AA_ModuleState;
    unsigned int module_task_register;
    unsigned char module_err_message[10];
    unsigned char module_err_number;
} AA_DEVICE_STATE;    
    
typedef struct _AA_All_Data
{
    AA_FAST_DATA        AA_fast;      //fast_data
    AA_DETAIL_DATA      AA_detail;      //slow_data
    AA_DEVICE_STATE     AA_state;     
} AA_ALL_DATA;  

#define PhaseIn_HAL_RANGE	80
#define PhaseIn_ENF_RANGE	80
#define PhaseIn_ISO_RANGE	80
#define PhaseIn_SEV_RANGE	100
#define PhaseIn_DES_RANGE	220

#define Drager_HAL_RANGE	85
#define Drager_ENF_RANGE	100
#define Drager_ISO_RANGE	85
#define Drager_SEV_RANGE	100
#define Drager_DES_RANGE	200


//记录AA和AA的范围
typedef struct 
{
	enum AA_AGENT AA1_ID;
	enum AA_AGENT AA2_ID;
	Int16U AA_Range;
	Int16U AA1_StdRange;
	Int16U AA2_StdRange;
}AA_Range_t;
    
extern enum IM_FUNC_STS (*AA_RX_Receive)(Int8U nowbyte);
extern  enum IM_FUNC_STS (*AA_RX_Process)(void);
extern AA_ALL_DATA AA_all_data;
extern int zeroing_state;
extern int spaning_state;

void AA_ModuleInit(enum AA_ModuleName aa_name);
enum IM_FUNC_STS AA_SendOpMode(enum AA_OPMODE mode);
enum IM_FUNC_STS AA_SendAAGas(enum AA_GAS aax,enum  AA_AGENT id);
enum IM_FUNC_STS AA_InitZero(void);
enum IM_FUNC_STS AA_SendExPara(enum AA_GAS valuemask,Int16U value);
enum IM_FUNC_STS AA_SetPatientData(void);
enum IM_FUNC_STS AA_SendParaUnit(Int16U mask,Int8U unit);
enum IM_FUNC_STS AA_SendBreathDetection(enum AA_BREATHMODE mode);
enum IM_FUNC_STS AA_SwitchValves(Int8U valve);
enum IM_FUNC_STS AA_SwitchPumps(enum AA_PUMP_STATE pump);
enum IM_FUNC_STS AA_StopContinueTrans(void);

void AA_SetOpMode(enum AA_OPMODE mode);
void AA_SetIRO2Value(Int8U Value);
void AA_SetO2Option(Int8U state);
void AA_SetBreathDetection(enum AA_BREATHMODE mode);
void AA_SetAAGas(enum AA_GAS aax,enum  AA_AGENT id);
void AA_SetExPara(enum AA_GAS valuemask,Int16U value);
void AA_SetParaUnit(Int16U mask,Int8U unit);
void AA_SetPumpMode(enum AA_PUMP_STATE mode);

Int8U AA_GetCMDFrame(enum AA_CMD cmd,char* tx_buffer);
enum IM_FUNC_STS AA_GetWaveData(enum AA_GAS gastype);
enum IM_FUNC_STS AA_GetSlowData(enum AA_DETAIL_PARA type);
enum IM_FUNC_STS AA_GetModuleState(void);
void AA_Err_Message_Progress(void);
void AA_Module_Monitor(void);

void AA_SetO2SensorType(O2SENSOR_Type O2Sensor);
void AA_SetSpanCalType(GAS_SPANCAL_Type SpanCal,Int8U Value);
void AA_SetSpanCal21Type(void);
void AA_Module_SendOperationMessage(Int8U type);
#endif