#ifndef __DRAGER_H    
#define __DRAGER_H    

/*  #include directives go here */
#include "Static_Menu_Creat.h"
#include "AA_Module.h"

/*  #define directives go here  */
//typedef INT8U u8;
//typedef INT16U u16;
//typedef INT8U s8;
//typedef INT16U s16;

#define DRAGER_ID_BASETIME          0x02  //基本时间间隔
#define DRAGER_ID_OPERATION         0x03  //工作模式
#define DRAGER_ID_INFORMATION       0x0A  //设备信息
#define DRAGER_ID_FRAMEDATA         0x12  //病人数据
#define DRAGER_ID_STOPTRANS         0x19  //停止连续传送
#define DRAGER_ID_CHANGEUNITS       0x1B  //改变单位
#define DRAGER_ID_ACCEPTPARA        0x1C  //接收额外参数
#define DRAGER_ID_SELECTAA          0x1D  //选择麻醉气体类型
#define DRAGER_ID_CHANGEBREATH      0x1E  //改变呼吸探测模式
#define DRAGER_ID_INITZERO          0x20  //初始化零点
#define DRAGER_ID_ADJUSTTIME        0x2B  //调整时间信息
#define DRAGER_ID_TRANSMODULEMASK   0x2C  //传输模块特征
#define DRAGER_ID_SWITCHVALVES      0x61  //切换阀
#define DRAGER_ID_SWITCHPUMP        0x62  //切换泵

//工作模式
#define DRAGER_OP_MEASURE       0x00
#define DRAGER_OP_STANGDBY      0x10

//病人数据选择码
#define DRAGER_NONE             0x0000
#define DRAGER_CO2ANDN2O        0x0001
#define DRAGER_O2               0x0002 
#define DRAGER_PRESSUREANDPUMP  0x0004
#define DRAGER_BREATHINFO       0x0008
#define DRAGER_PARINFO          0x0010
#define DRAGER_AA1              0x0020
#define DRAGER_AA2              0x0040
#define DRAGER_PARUNIT          0x0080  //返回所有参数单位
#define DRAGER_MESSAGE          0x0100

//气体选择
//#define DRAGER_O2               0x0002
#define DRAGER_CO2              0x0200
#define DRAGER_N2O              0x0400
//#define DRAGER_AA1              0x0020
//#define DRAGER_AA2              0x0040

#define DRAGER_RT_NONE          0x0000
#define DRAGER_RT_DATANUM       0x4000
#define DRAGER_RT_O2            0x0010
#define DRAGER_RT_AA1           0x0004
#define DRAGER_RT_AA2           0x0008
#define DRAGER_RT_N2O           0x0002
#define DRAGER_RT_CO2           0x0001

//改变参数单位
#define DRAGER_UNIT_ATS   0x00 //默认
#define DRAGER_UNIT_ATPS  0x05

//切换阀
#define DRAGER_VALVE_SAMPLE   0x00
#define DRAGER_VALVE_ROOM     0x01

//切换泵
#define DRAGER_PUMP_OFF   0x00
#define DRAGER_PUMP_HIGH  0x02

//麻醉气体
#define AUTOIDENTIFICATION  0x00
#define HALOTHANE           0x01
#define ENFLURANE           0x02
#define ISOFLURANE          0x03
#define SEVOFLURANE         0x04
#define DESFLURANE          0x05
#define DONOTCHANGE         0xFF

//错误码
#define DRAGER_ERR_STANDBY_MODE      0x0001
#define DRAGER_ERR_AA1_NOAVAILABLE   0x0002
#define DRAGER_ERR_AA1_NOOPERATE     0x0003
#define DRAGER_ERR_OCCLUSION         0x0004
#define DRAGER_ERR_ZEROINPROGRESS    0x0005
#define DRAGER_ERR_WARMUP            0x0006
#define DRAGER_ERR_NOACCURACY        0x0007
#define DRAGER_ERR_AA2_NOAVAILABLE   0x0008
#define DRAGER_ERR_AA2_NOOPERATE     0x0009
#define DRAGER_ERR_INPUTPARAWRONG    0x000A
#define DRAGER_ERR_GAS_NOAVAILABLE   0x000B
#define DRAGER_ERR_GAS_NOOPERATE     0x000C
#define DRAGER_ERR_WATERTRAPDISCONNECTED  0x0010
#define DRAGER_ERR_WATERTRAPWILLFULL      0x0020
#define DRAGER_ERR_WATERTRAPFULL          0x0040
#define DRAGER_ERR_WATERTRAPWRONG         0x0080
#define DRAGER_ERR_COMPONENTFAIL          0x0100
#define DRAGER_ERR_NORESPDECT             0x0200
#define DRAGER_ERR_APNEA                  0x0400

#define DragerMSG_No_Message                0x00
#define DragerMSG_Supply_Voltage_Bad        0x01
#define DragerMSG_CO2sensor_Hardware_Fail   0x02
#define DragerMSG_O2sensor_Hardware_Fail    0x03
#define DragerMSG_Gas_Inlet_Leakage         0x20
#define DragerMSG_Gas_Inlet_Occlusion       0x21
#define DragerMSG_Gas_Outlet_Occulusion     0x22
#define DragerMSG_Occulusion                0x23
#define DragerMSG_Pneum_Hardware_Error      0x24
#define DragerMSG_Check_Flow                0x25
#define DragerMSG_SampleLine_NotConnected   0x27
#define DragerMSG_SampleLNotC_or_GasInLeak  0x28
#define DragerMSG_Check_Watertrap           0x29
#define DragerMSG_Watertrap_Full            0x2A
#define DragerMSG_CheckWater_or_GasInOcculsion  0x2B
#define DragerMSG_WaterFull_or_GasInOcculsion   0x2C
#define DragerMSG_Fan_Not_turning           0x30
#define DragerMSG_Warm_up                   0x62
#define DragerMSG_Sensor_Too_Warm           0x63
#define DragerMSG_Zero_Required             0x70
#define DragerMSG_Zero_Failed               0x71
#define DragerMSG_Zero_Ok                   0x72
#define DragerMSG_Zero_In_Progress          0x73
#define DragerMSG__Zero_Canceled            0x74
#define DragerMSG_Calibration_Required      0x78
#define DragerMSG_Standby_Mode              0x90
#define DragerMSG_Switched_Off_Mode         0x91
#define DragerMSG_Pump_Switched_Off         0xA8
#define DragerMSG_Valve_Not_Sample          0xA9
#define DragerMSG_Version_Mismatch          0xC1
#define DragerMSG_Download_Incomplete       0xC2
#define DragerMSG_Pneum_Not_Avail           0xD0
#define DragerMSG_CO2sensor_Not_Avail       0xD1
#define DragerMSG_O2sensor_Not_Avail        0xD2
#define DragerMSG_Pneum_Lost                0xD3
#define DragerMSG_CO2sensor_Lost            0xD4
#define DragerMSG_O2sensor_Lost             0xD5

/*  declarations of external variables go here  */

typedef void Send_Data(u8 *tx_array,u8 len);   //麻醉气体模块串口发送函数

typedef struct    //设备信息响应
{
    u8 Data[10];
    u8 Select;
}DRAGER_ACK_INFORMATION;

typedef struct    //病人数据响应
{
    u8 FrameData[10];
    u8 FrameNum;
    u8 ModuleStatus;
    u16 RTDataMask;
    u16 RTParaStatus;
    u8 RTData[16];
    u8 SelectMaskErr[8];
}DRAGER_ACK_FRAMEDATA;

typedef struct    //接收麻醉气体响应数据结构体
{
    u16 BaseTime;
    u8 OperatMode;
    DRAGER_ACK_INFORMATION Information;
    DRAGER_ACK_FRAMEDATA FrameData;
    u8 StopConTrans;
    u8 ChangeParaUnits;
    u8 AcceptExParaData;
    u8 SelectAnestheticAgent;
    u8 SwitchBreathMode;
    u8 InitiateZero;
    u8 Adjusttime;
    u8 GenericModuleMask[4];
    u8 SwitchValves;
    u8 SwitchPump;
    u8 Err;
}DRAGER_ACK;

struct DRAGER_RT_DATA
{
    u16 CO2;
    u16 N2O;
    u16 AA1;
    u16 AA2;
    u16 O2;
    u8 Num;
};

struct DRAGER_AA_DATA   //AA1和AA2的参数数据
{
    u16 FI; //吸气浓度
    u16 ET; //呼气浓度
    u8 IDMF;
    u8 PAS; //0:NoAvailable,1:LowConcentration,2:Identified,3:Calculate,4:OverFlow,5:Mixture,6:Forced,7:Estimated
    u8 PID; //0:NoAvailable,1:Halothane,2:Enflurance,3:Isoflurane,4:Sevoflurane,5:Desflurane
    u8 PS;
    u8 ParaMode;  //0:Valid,1:Overflow,2:Invalid,3:NotAvailable
    u8 ZeroState; //0:No In Progress,1:In Progress
    u8 ZeroResult;  //0:Success,1:Failed
    u8 Accuracy;  //0:Startup,1:Unknown,2:Reduced,3:Iso
    u8 Unit;  //0：默认单位（ATS），1：另选参数（ATPS）
};

struct DRAGER_GAS_DATA //O2、CO2、N2O参数数据
{
    u16 FI; //吸气浓度
    u16 ET; //呼气浓度
    u8 PS;
    u8 ParaMode;  //0:Valid,1:Overflow,2:Invalid,3:NotAvailable
    u8 ZeroState; //0:No In Progress,1:In Progress
    u8 ZeroResult;  //0:Success,1:Failed
    u8 Accuracy;  //0:Startup,1:Unknown,2:Reduced,3:Iso
    u8 Unit;  //0：默认单位（ATS），1：另选参数（ATPS）
};

struct DRAGER_DEVICEINFO
{
    u8 Drager_Vendor_Code[5];  //供应商代码
    u8 Drager_Serial_Num[9];  //序列号
    u8 Drager_HandleVersion[2]; //硬件版本
    u8 Drager_SoftwareVersion[8]; //软件版本
    u8 Drager_ProductName[4]; //产品名称
    u8 Drager_PartNumber[7];  //零件号码
    u8 Drager_Dev_Flag;
};

struct DRAGER_MODULEFLAG //组件标志
{
    u8 Connect_Flag; //通讯正常标志
    //u8 Init_Finish_Flag; //完成初始化标志
    enum
    {
        Measure = 0,
        Standby_Mode,
        ComponentsOff,
        TurnToMeasure,
        TurnToStandby,
        ClosingComponents
    }OperateMode;  //工作模式，0：测试，1：待机，2：部件关，3：正在转为测试，4：正在转为待机，5：正在关部件
    u8 ZeroRequest; //零点请求，0：不需校零，1：需要校零
    u8 ZeroState;     //0:无，1:正在校零，2:校零成功，3:校零失败
    u8 BreathPhase;   //呼吸阶段：0：吸气，1：呼气
    u8 CheckWaterOrLine;  //检测脱水器和采样管，0：OK，1：需要检测
    u8 ComponentFail;//子部件错误，0：无错误，1：有错误
    u8 BreathAvail;//呼吸阶段数据可用性，0：传输实时数据，1：传输呼吸数据
    u8 NorespOrApnea;
    enum
    {
        NoneAgent = 0,
        NoAgentAvailable,
        AgentAvailable,
        AgentIdentifiaction
    }SetAgent;
    u8 SetAgent_Finish_Flag;
    u8 PumpState;  //泵状态：0：关，1：高流量
    u16 Err;
};

struct DRAGER_PRESSANDPUMP //压力和泵流量
{
    u8 WatertrapWarning;
    u8 WatertrapFull;
    u8 WatertrapDisconnected;
    u8 ValvesState;
    u8 PumpFlowState;
    u16 Pressure;
    u8 Pre_ParaMode;  //0:Valid,1:Overflow,2:Invalid,3:NotAvailable
    u8 Pre_ZeroState; //0:No In Progress,1:In Progress
    u8 Pre_ZeroResult;  //0:Success,1:Failed
    u8 Pre_Accuracy;  //0:Startup,1:Unknown,2:Reduced,3:Iso
    u8 Pre_Unit;  //0：默认单位（ATS），1：另选参数（ATPS）
    u16 PumpFlow;
    u8 Flow_ParaMode;  //0:Valid,1:Overflow,2:Invalid,3:NotAvailable
    u8 Flow_ZeroState; //0:No In Progress,1:In Progress
    u8 Flow_ZeroResult;  //0:Success,1:Failed
    u8 Flow_Accuracy;  //0:Startup,1:Unknown,2:Reduced,3:Iso
    u8 Flow_Unit;  //0：默认单位（ATS），1：另选参数（ATPS）
};

struct DRAGER_RESPINFO
{
    u16 ApneaDuration;
    u8 RespirationRate;
    u8 BreathDetectionMode;
};

/*  prototypes  */
/*
void Init_Drager(Send_Data *USART_SEND);  //发送串口初始化

void ReceiveDragerData(u8 data);  //串口接收中断数据处理函数
u8 Check_Drager_Sum(u8 len);  //检查校验和
void AnalysisDragerData(void);  //数据解析函数
u8 CalculateCheckSum(u8 len);   //计算校验和
void AnalysisDragerErr(u8 err); //错误代码分析

//设置基本时间间隔，默认20ms，连续数据发送时间=基本时间间隔*设置时间间隔（ms）
void Drager_SetBaseTime(u16 basetime);

//获取基本时间间隔
void Drager_GetBaseTime(void);

//设置工作模式，测试模式：0x00，工作模式：0x10
void Drager_SetOpMode(u8 opmode);

//获取设备信息，供应商代码：0x00，序列号:0x01，硬件版本：0x02，软件版本：0x03,产品名称：0x05，零件号码：0x06
void Drager_GetDivInfo(u8 infoselect);

//获取病人信息
//选择码：bit0：CO2、N2O浓度，bit1：O2浓度，bit2：压力和泵流量，bit3：呼吸信息
//bit4：参数详细状况，bit5：生理药剂1，bit6：生理药剂2，bit7：参数单位信息，bit8：通知信息
//实时参数码：bit0：CO2，bit1：N2O，bit2：生理药剂1，bit3：生理药剂2，bit4：O2，bit14：数据包数量
//设置时间间隔，连续数据发送时间=基本时间间隔*设置时间间隔（ms）
void Drager_GetPatientData(u16 framemask,u16 rtmask,u16 ints);

//停止连续发送
void Drager_StopContinueTrans(void);

//改变参数单位
//参数选择码：bit0：改变单位
//单位：ATS：0x00，ATPS：0x05
void Drager_ChangeParaUnit(u16 paramask,u8 unit);

//接收额外参数
//参数选择码：bit0：CO2，bit1：N2O，bit2：AA1，bit3：AA2，bit4：O2，bit5：平均圧力，bit6：压力
//参数模式：高精度：0x01，低精度：0x02
void Drager_Accessexpara(u16 valuemask,s16 value,u8 paramode);

//选择麻醉气体
//自动识别：0x00，氟烷：0x01，安氟醚：0x02，异氟醚：0x03，七氟醚：0x04，地氟醚：0x05，不改变：0xff
void Drager_SelectAA(u8 anestheticagent);

//改变呼吸探测模式
//程序探测：0x01，自动激活：0x02，在呼吸相1后自动唤醒：0x05,..2:0x06,..3:0x07,..4:0x08,..5:0x09
void Drager_ChangeBreathMode(u8 breathmode);

//初始化零点
//调零参数：所有参数：0x00，必须参数：0x01
//校零类型：室空气零点：0x01，无零点请求：0xFF
void Drager_InitZero();

//调整时间
//时：0~23，分：0~59，秒：0~59，日：1~31，月：1~12，年：0~99
void Drager_Adjusttime(u8 hour,u8 minu,u8 sec,u8 day,u8 month,u8 year);

//传输模块特征码
void Drager_TransModuleMask(void);

//切换阀
//采样气体：0x00，室空气：0x01
void Drager_SwitchValves(u8 valve);

//切换泵
//无流量：0x00，高流量：0x02
void Drager_SwitchPumps(u8 pump);
*/

enum IM_FUNC_STS ReceiveDragerData(u8 data);
u8 Check_Drager_Sum(u8 len);
enum IM_FUNC_STS AnalysisDragerData(void);
u8 CalculateCheckSum(u8 len,u8* tx_buffer);
void AnalysisDragerErr(u8 err);

//void Drager_SetBaseTime(u16 basetime);
void Drager_GetBaseTime(u8* tx_buffer);
u8 Drager_Accessexpara(u16 valuemask,u16 value,u8 paramode,u8* tx_buffer);
void Drager_TransModuleMask(u8* tx_buffer);

u8 Drager_SetOpMode(u8 opmode,u8* tx_buffer);
u8 Drager_StopContinueTrans(u8* tx_buffer);
u8 Drager_ChangeParaUnit(u16 paramask,u8 unit,u8* tx_buffer);
u8 Drager_SelectAA(u8 anestheticagent,u8* tx_buffer);
extern u8 Drager_InitZero();
void Drager_Adjusttime(u8 hour,u8 minu,u8 sec,u8 day,u8 month,u8 year,u8* tx_buffer);
u8 Drager_SwitchValves(u8 valve,u8* tx_buffer);
u8 Drager_SwitchPumps(u8 pump,u8* tx_buffer);

//void Init_Drager(Send_Data *USART_SEND);  //初始化串口发送函数
void Init_Drager(void);
void Drager_SetConnect(void); //握手、开泵、校零
u8 Drager_GetDivInfo(u8 infoselect,u8* tx_buffer);  //获取设备信息
u8 Drager_ChangeBreathMode(u8 breathmode,u8* tx_buffer);  //设置呼吸探测模式
u8 Drager_JudgeAgent(void); //1:需要手动设置AA，2:无需设置，自动识别
u8 Drager_GetPatientData(u16 framemask,u16 rtmask,u16 ints,u8* tx_buffer);
u16 Drager_GetAAData(u16 channel);  //获取AA参数状态
u16 Drager_GetGasData(u16 channel); //获取O2、N2O、Air参数状态
u16 Drager_GetModuleState(u8* tx_buffer);
void AnalysisDragerMessage(u8 high_message,u8 med_message,u8 low_message);
void AnalysisDragerErr(u8 err);
#endif
