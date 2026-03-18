#ifndef _DATA_HANDLE_H_
#define _DATA_HANDLE_H_
#include "alarm_handle.h"
#include "string.h"
#define FILTER_NUM  600
extern Int16U BatteryVoltageSample[FILTER_NUM];
Int16U  DataFilterHandle_Battery(Int16U *Pdata,Int16U Len);
int BatteryAdFilter(int flag,int *BatteryVoltage);


#endif