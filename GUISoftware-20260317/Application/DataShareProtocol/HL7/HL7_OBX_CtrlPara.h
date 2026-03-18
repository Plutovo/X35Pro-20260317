#ifndef _HL7_OBX_CTRLPARA_H
#define _HL7_OBX_CTRLPARA_H
#include "stdtypes.h"
#include "HL7_Common.h"
#include "HL7_cfg.h"

//¿ØÖÆ²ÎÊý
typedef enum
{
    HL7_CtrlParatp_None = 0,
    HL7_CtrlParatp_PEEP,
    HL7_CtrlParatp_VT,
    HL7_CtrlParatp_f,
    HL7_CtrlParatp_IEI,
    HL7_CtrlParatp_IEE,
    HL7_CtrlParatp_Tslope,
    HL7_CtrlParatp_Tinsp,    
    HL7_CtrlParatp_Pinsp,
    HL7_CtrlParatp_Psupp,
    HL7_CtrlParatp_Tpause,
    HL7_CtrlParatp_Esens,
    HL7_CtrlParatp_Psens,
    HL7_CtrlParatp_Fsens,
    HL7_CtrlParatp_Sigh,
#if (_HL_TYPE == _HL7_ANES)    
    HL7_CtrlParatp_FG,
    HL7_CtrlParatp_PerO2,
#else
    HL7_CtrlParatp_Phigh,
    HL7_CtrlParatp_Plow,
    HL7_CtrlParatp_Thigh,
    HL7_CtrlParatp_Tlow,
#endif
    HL7_CtrlParatp_Last,
}HL7_CtrlParatp_t;


void HL7_CtrlPara_Send(void);
bool HL7_CtrlPara_Change(void);
void HL7_Read_CtrlPara(HL7_CtrlParatp_t type, HL7_Para_t *para);
#endif
