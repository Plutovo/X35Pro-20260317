#ifndef _HOMEPROMPT_CFG_H
#define _HOMEPROMPT_CFG_H
#include "os_prio.h"

#define HPBufSize  200
#define OS_CFG_PRIO_MAX  32u	//优先级个数

#define FGBTN_LimitColor	0xFFE763
#define FGBTN_StandbyColor	0xF7D7BD
#define FGBTN_WorkColor		0x1E1E1E
#define FGBTN_UpperColor	0x746253
#define FGBTN_PmtColor		0xF3D6C1 

#define STANDBYSTATE(state) ((state == 0xFF) ? 1:0)

#define HomePrompt_4everPos	HomePrompt_Dragger_Prio

//home prompt type priority
#define HomePrompt_NULL_Prio		(Int8U)0
// #define HomePrompt_MinFG_Prio		(INT8U)8	//流量调节限制提示
// #define HomePrompt_Temp_Prio		(INT8U)9	//临时信息提示
// #define HomePrompt_NoGas_Prio		(INT8U)10
// #define HomePrompt_EGMFail_Prio		(INT8U)11
// #define HomePrompt_NoFresh_Prio		(INT8U)12
// #define HomePrompt_O2Sub_Prio		(INT8U)13
// #define HomePrompt_O2Add_Prio		(INT8U)14
#define HomePrompt_Dragger_Prio		(Int8U)15
// #define HomePrompt_MasimoFail_Prio	(INT8U)16
// #define HomePrompt_EmergencyO2_Prio	(INT8U)17
// #define HomePrompt_O2CONLOW_Prio	(INT8U)18
#define HomePrompt_OSIdle_Prio		(Int8U)(OS_CFG_PRIO_MAX-1) //must exist,init and not del
//temp prompt delay(ms)
#define HomePrompt_TempDelay		(Int8U)(3*1000)

#endif
