#ifndef _HL7_OBX_H
#define _HL7_OBX_H
#include "stdtypes.h"

typedef enum
{
    HL7_OBX_monitor = 0,
    HL7_OBX_ctrl,
    HL7_OBX_alrmlim,
    HL7_OBX_alrm,
    HL7_OBX_Mesg,
}HL7_OBX_chse_t;

//HL7ÏûÏ¢
typedef enum
{
    OBX1_SetID = 0,
    OBX2_ValType,
    OBX3_ObsID,
    OBX4_ObsSubID,
    OBX5_ObsVal,
    OBX6_Units,
    OBX7_ReferRange,
    OBX8_AbnorFlags,
    OBX9_PR,
    OBX10_AbnorTest,
    OBX11_ObsResStatus,
    OBX12_EffectDate,
    OBX13_UserDef,
    OBX14_ObsDate,
    OBX15_ProdID,
    OBX16_ResObs,
    OBX17_ObsMethod,
    OBX18_EquipID,
    OBX19_AnalyDate,
    OBX20_ObsSite,
    OBX21_ObsInsID,
    OBX22_ModeCode,
    OBX23_PerOrgName,
    OBX24_PerOrgAddr,
    OBX25_PerOrgMed,
    OBX26_Last,
}HL7_OBXx_t;

void HL7_OBX_Send(HL7_OBX_chse_t chse);

#endif
