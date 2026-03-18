#ifndef _DATASHARE_INC_EXT_H
#define _DATASHARE_INC_EXT_H

#include "AA_Module.h"
#include "C002_CO2.h"
#include "Uart.h"
#include "co2_irma.h"
#include "JM1_SpO2.h"
#include "static_Menu_creat.h"
//#include "SpO2.h"
#include "Alarm_Handle.h"
#include "AlarmDispStrLib.h"
#include "WIDGET_Patient.h"


#ifdef AAMODULE
extern AA_ALL_DATA AA_all_data;
extern AA_PARAMETERS AA_Para;
#endif

extern unsigned char StandbyState;
extern MONITOR_PARA MonitorPara;
extern C002_CO2_PARAMETERS CO2_Parameters;
extern BREATH_SET_PARA BeathSettingPara;
extern VENT_TIME VentTime;
extern int ETCO2Unit;
extern int MenuType;
extern MENU_HWIN Menu_hWin;

#ifdef EGMMODULE
extern unsigned char EmergencyChannelSwitch;//应急通道标志  1：开   0：关
extern u8 O2Concen100;
extern u8 O2Concen;
extern u16 totalFlow;
#endif

extern ALARM_SET_PARA AlarmSettingPara;
extern ALARM_WORK AlarmsWork;

extern JM1_SPO2_PARAMETERS SpO2_Parameters;
#endif
