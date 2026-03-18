//
#include "Alarm_handle.h"
//
unsigned char etco2pump_Occlusion = 0;
unsigned char etco2pump_needupdate = 0;
char Usart2_Tx_Buffer[8];
unsigned char btpsn2o = 0;
unsigned char lastbtpsn2o = 0;
unsigned char n2oo2 = 0;
unsigned char lastn2oo2 = 0;
extern ALARM_WORK AlarmsWork;
//
void BTPSCompensate()
{
}
void O2CO2Compensate()
{
}
void CheckCo2PumpRunTime()
{
}
void CheckCo2PumpOcclusion()
{
}
void AlarmCheckCo2PumpOcclusion(Int8U Vent_Flag)
{
}
void Co2ModuleONOFF(unsigned int onoff)
{
}
void CO2_Reset(void)
{
}
