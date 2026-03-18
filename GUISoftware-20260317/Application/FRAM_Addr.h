#ifndef _FRAM_ADDR_H
#define _FRAM_ADDR_H

#ifdef __cplusplus
    extern "C" {
#endif
/* Includes ----------------------------------------------------------------- */
// #include "MedibusComm_cfg.h"
#include "FRAM_Addr_cfg.h"
#include "string.h"

void FRAM_Addr_WriteInfo(void);
//FRAM Init
void FRAM_Addr_Init(void);
//Sys Init
FlagStatus FRAM_Addr_SysInitFlag(void);
void FRAM_Addr_SysInitFlag_Init(void);
//Sys Model Init
FlagStatus FRAM_Addr_SysModelInit(void);
//read boot
FlagStatus FRAM_Addr_SysReadBOOTVer(char *buf, u8 size);
//GUI Version operate
void FRAM_Addr_SysGUIVerInit(void);
FlagStatus FRAM_Addr_SysReadGUIVer(char *buf, u8 size);
void FRAM_Addr_SysWriteGUIVer(void);
//clear update flag
void FRAM_Addr_SysUpdateClear(void);
//FRAM new addr
FlagStatus FRAM_Addr_SysUseNewAddr(void);
//Screen Model
FlagStatus FRAM_Addr_SysScreenModelInit(void);
void FRAM_Addr_SysWriteScreenModel(void);
//device standard
FlagStatus FRAM_Addr_SysStandard(void);
//device type
FlagStatus FRAM_Addr_SysDeviceTypeInit(void);
void FRAM_Addr_SysWriteDeviceType(void);

FlagStatus FRAM_Addr_SysModelType(MODEL_Type type);

//(BDU EFM/EGM PMU) boot and app
void FRAM_Addr_SysBDUBOOTInit(void);
void FRAM_Addr_SysWriteBDUBOOT(void);

void FRAM_Addr_SysBDUAPPInit(void);
void FRAM_Addr_SysWriteBDUAPP(void);

void FRAM_Addr_SysEGMBOOTInit(void);
void FRAM_Addr_SysWriteEGMBOOT(void);

void FRAM_Addr_SysEGMAPPInit(void);
void FRAM_Addr_SysWriteEGMAPP(void);

void FRAM_Addr_SysPMUBOOTInit(void);
void FRAM_Addr_SysWritePMUBOOT(void);

void FRAM_Addr_SysPMUAPPInit(void);
void FRAM_Addr_SysWritePMUAPP(void);
FlagStatus FRAM_Addr_Sys_Version(BOARD_VERSION_Type type);

//clear trend/alarm/self result
void FRAM_Addr_TrendClear(void);
void FRAM_Addr_AlarmClear(void);
void FRAM_Addr_ResultClear(void);
//Move
void FRAM_Addr_DataMove(void);

#ifdef __cplusplus
}
#endif

#endif

