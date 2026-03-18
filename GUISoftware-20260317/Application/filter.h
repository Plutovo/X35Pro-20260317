#ifndef _FILTER_H
#define _FILTER_H
#include "AA_Module.h"
#include "stdtypes.h"

#define N 20

typedef struct  
{
    u8 Bat_Conn[10];
    u8 Bat_Count;
    u8 Bat_DisConnCount;
    u8 Bat_DisConnFlag;
}BATTERY_CONN;


typedef struct  
{
	u8 Alarm_Chk_FiAAHigh_Flag;
	u8 Alarm_Chk_EtAAHigh_Flag;
}Alarm_Chk_t;


#define AA_HAL_HIGH_Limit	50
#define AA_ENF_HIGH_Limit	50
#define AA_ISO_HIGH_Limit	50
#define AA_SEV_HIGH_Limit	80
#define AA_DES_HIGH_Limit	150


extern u8  CO2_Num_Fi;
extern u16 CO2_ValueBuf_Fi[N];
extern u8  CO2_Num_Et;
extern u16 CO2_ValueBuf_Et[N];

extern u8  NO2_Num_Fi;
extern u16 NO2_ValueBuf_Fi[N];
extern u8  NO2_Num_Et;
extern u16 NO2_ValueBuf_Et[N];

extern u8  AA1_Num_Fi;
extern u16 AA1_ValueBuf_Fi[N];
extern u8  AA1_Num_Et;
extern u16 AA1_ValueBuf_Et[N];

extern u8  O2_Num_Fi;
extern u16 O2_ValueBuf_Fi[N];
extern u8  O2_Num_Et;
extern u16 O2_ValueBuf_Et[N];

u16 filter(u16 *value_buf,u8 *num,u16 AA_Value);
void Battery_Collect(void);
void Battery_Check(void);
Int8U Battery_GetDisconnFlag(void);
void Battery_SetDisconnFlag(Int8U flag);

void AA_Range_AAID_Change(int *flag, void* data);
void AA_Range_Para(void* data);
Int16U AA_Range_GetRange(void);
Int16U AA_Range_GetAA1StdRange(void);
Int16U AA_Range_GetAA2StdRange(void);

u8 Alarm_Chk_GetFiAAFlag(void);
u8 Alarm_Chk_GetEtAAFlag(void);
void Alarm_Chk_SetFiAAFlag(u8 data);
void Alarm_Chk_SetEtAAFlag(u8 data);
#endif

