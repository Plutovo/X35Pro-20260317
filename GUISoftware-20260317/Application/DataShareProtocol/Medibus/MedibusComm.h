#ifndef _MEDIBUSCOMM_H
#define _MEDIBUSCOMM_H

#ifdef __cplusplus
 extern "C" {
#endif
     
#include "MedibusComm_cfg.h"

#include "MainTask.h"
#include "C002_CO2.h"
#include "Uart.h"
#include "co2_irma.h"
#include "JM1_SpO2.h"
#include "static_Menu_creat.h"
#include "FRAM_Addr_cfg.h"
//#include "func_extern.h"
#include "AA_Module.h"
//#include "SpO2.h"     

//FSM
FSM_STATE FSMStep_Idle(void * arg);
FSM_STATE FSMStep_Reset(void * arg);
FSM_STATE FSMStep_DevID(void * arg);
FSM_STATE FSMStep_PCID(void * arg);
FSM_STATE FSMStep_Stop(void * arg);
FSM_STATE FSMStep_Start(void * arg);
void Medibus_FSM(void * invar);
void Medibus_FSM_Start(void);
void Medibus_frmrecv(void);
//Timer
void Medibus_Timer1(void);
void Medibus_Timer2(void);
void Medibus_timer(void);

//uart recv flag
void Medibus_SetUserStopFlag(u8 data);
u8 Medibus_GetCommActiveFlag(void);
u8 Medibus_GetUserStopFlag(void);
u8 Medibus_GetPCResStopFlag(void);

//Medibus data handle
void Medibus_Init(void);
void Medibus_VarInit(void);
void Medibus_SendCMD(Medibus_DIR DIR,Medibus_CMD CMD);
void Medibus_SendDeviceID(void);
void Medibus_SendMeauData(void);
void Medibus_SendTextMess(void);
void Medibus_MeasDataFMT(u8 data_type, u8 *buf);
void Medibus_CheckSumCal(u8 *buf, u8 num);
Medibus_ASCII HEX2ASCII(u8 data);
Medibus_I2A Medibus_int2ascii(u16 val);

Medibus_dp_t Medibus_read_DP_Recv(void);

//get para
void Medibus_GetMeasuredData(void);
void Medibus_GetTextMess(void);


#ifdef __cplusplus
}
#endif

#endif

