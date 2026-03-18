#ifndef _HL7_OBX_PARA_H
#define _HL7_OBX_PARA_H
#include "stdtypes.h"
#include "HL7_Common.h"
#include "HL7_cfg.h"

//参数类型
typedef enum
{
    HL7_Para_None = 0,
    HL7_Para_PEEP,
    HL7_Para_Ppeak,
    HL7_Para_Pplat,
    HL7_Para_Pmean,
    HL7_Para_VTe,
    HL7_Para_VTi,
    HL7_Para_MV,
    HL7_Para_MVspn,
    HL7_Para_f,
    HL7_Para_fspn,
    HL7_Para_IEI,
    HL7_Para_IEE,    
    HL7_Para_Compl, //cdyn
    HL7_Para_RST,
    HL7_Para_Pmin,
    HL7_Para_EtCO2,
    HL7_Para_FiCO2,
    HL7_Para_FiO2,
    HL7_Para_SpO2,
    HL7_Para_PR,
    HL7_Para_PI,
#if (_HL_TYPE == _HL7_ANES)
    HL7_Para_EtO2,    
    HL7_Para_FiN2O,
    HL7_Para_EtN2O,
    HL7_Para_FiAA,
    HL7_Para_EtAA,
    HL7_Para_FiAA2,
    HL7_Para_EtAA2,
    HL7_Para_MAC,
    HL7_Para_Flow_N2O,
    HL7_Para_Flow_Air,
    HL7_Para_Flow_O2,
#else
    HL7_Para_RSBI,
    HL7_Para_PEEPI,
    HL7_Para_CST,
#endif
    HL7_Para_Last,
}HL7_Paratp_t;


void HL7_Para_Init(void);
void HL7_Para_Send(void);
void HL7_Read_Para(HL7_Paratp_t type, HL7_Para_t *para);
#endif
