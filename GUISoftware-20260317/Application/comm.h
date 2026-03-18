#ifndef _COMM_H
#define _COMM_H

#include "Static_Menu_Creat.h"
#include "Alarm_handle.h"
#include <time.h>

extern BREATH_SET_PARA BeathSettingPara;
extern MONITOR_PARA MonitorPara;

extern int BackUpMode;
extern MENU_HWIN Menu_hWin;

extern unsigned char EtCO2ModuleExist; //  20130409
extern unsigned char SpO2ModuleExist;
extern const int IE_RatioTable[];
extern VENT_TIME VentTime;//  20130616
extern unsigned char DEMOFLAG;   //控制选配是否通过密码开启  1：直接开启  0：通过密码开启

void SendSetParameter(int Value,int Id);

RTC_TIMER RTCReadTime(void);
int CheckAcStatus(void);
char RTEEPROMread(unsigned int addr);
void SelfTestStart(void);
void SendSetModes(int modes);
void    DataTransmit(Int16U data,Int8U type,Int8U * Pointer,Int8U UsartPort);
void RTEEPROMwrite(unsigned short int addr, unsigned char databyte);
#endif