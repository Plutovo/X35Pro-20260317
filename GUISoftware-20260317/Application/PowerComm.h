#ifndef __POWERCOMM_H
#define __POWERCOMM_H


/*------------------------------------------------------------------------------------*/
//开关机状态
typedef enum
{
	START_UP = 0,
	CANCEL_SHUT_DOWN =3 ,
	SHUT_DOWN = 0x10,
}Power_OnOff_t;

/*---------------------电源板传输协议---------------------------------------------*/
//帧头
#define POWER_HEADER        0xF5
//数据类型
#define PM_TO_GUI   0x01
#define GUI_TO_PM   0x03

#define POWER_DATA_LENGTH 4

#if 1
//起始地址
#define DATA_TYPE_0 0
#define DATA_TYPE_1 1

#define START_ADDR_ONOFF_STATE 		0x14
#define START_ADDR_ONOFF_TYPE			0x15
#define START_ADDR_BAT_MANAGE_TYPE		0x3A

//字偏移量
#define POWER_VERSION       0x02 //电源板版本
#define POWER_VOLTAGE       0x26 //电池电压
#define POWER_BATTERY       0x28//电池电量百分比
#endif
//
//#define POWER_DEVICE        0x01
//#define POWER_CONNECT       0x00
//#define POWER_DISCONNECT    0x01
//#define POWER_CHARGE        0x02
//#define POWER_NOCHARGE      0x03
//
//#define POWER_END           0x55



#define POWER_LEN           50//20//数据包长度

typedef enum{
    HEADER = 0,
    COMMTYPE,
    DATALENGTH,
    DATATYPE,
    DATA,
    CHECKSUM,
}POWER_DATA;   


typedef struct
{
    unsigned char CommType;
    unsigned char DataLen;
    unsigned char DataType;
    unsigned char DataBuf[50];
}POWER_Comm_t;

typedef struct
{
    unsigned short Product;
	unsigned short Hardware1;
    unsigned short Hardware2;
    unsigned short Software1;
    unsigned short Software2;
//	unsigned short RunTime;//无效
    //电源状态   1：无电源有电池 4：有电源无电池 5有电源有电池
	unsigned short PowerState;    
    //以下未实现，无效
//	unsigned short Current24;
//	unsigned short Voltage24;
//	unsigned short Current12;
//	unsigned short Voltage12;
//	unsigned short CurrentPM;
//	unsigned short VoltagePM;
//	unsigned short CurrentCharge;
//	unsigned short VoltageCharge;       
}POWER_Para0_t;

typedef struct
{
	unsigned short Battery1;
	unsigned short Battery2;//1和2合并一个整体
//	unsigned short BatteryTemper;//无效
//	unsigned short BatteryCurrent;//无效
	unsigned short BatteryVoltage;
	unsigned short BatteryShengYuRongLiang;
//	unsigned short BatteryChargeTime;//无效
//	unsigned short BatteryNume;//无效
	unsigned short OnOffState;//开关机状态 0开机 1关机 3取消手动关机
    //无效
//	unsigned short PowerFanState;
//	unsigned short WuHuaFaState;
//	unsigned short CaiYangFaState;
//	unsigned short AnQuanFaState;
    
//    unsigned short Battery1_Charging;  //无效
//    unsigned short Battery2_Charging;//无效
}POWER_Para1_t;
typedef struct
{
//    unsigned short Battery1_Capacity;
//    unsigned short Battery2_Capacity;
//    unsigned short Battery1_Voltage;
//    unsigned short Battery2_Voltage;
//    //无效
    unsigned short Charge_Discharge_State;//电池状态  0断开 1接通  （不使用：此标志的协议包接收较慢导致电池插拔后更新慢）
}POWER_Para2_t;


void Power_SendMachineModel(unsigned short type);
void Power_SendOnOffState(unsigned short state);
void Power_SendBatManageType(unsigned short state);

void Power_Receive(unsigned char data);
void SetBatteryPower(unsigned char ucEvent, unsigned char *ucBuff, int nLen);
int GetBattery(void);
int GetPowerVerL(void);
int GetPowerVerH(void);
//void Power_CheckVersion(unsigned char *TxBuff);
//void Power_CheckBatteryVoltage(unsigned char *TxBuff);

#endif //__POWERCOMM_H