#ifndef _HL7_OBX_ALRMLIM_H
#define _HL7_OBX_ALRMLIM_H
#include "stdtypes.h"
#include "HL7_Common.h"

//±¨¾¯ÏÞÖÆ
typedef enum
{
   HL7_AlrmLimtp_None = 0, 
   HL7_AlrmLimtp_PAW_H,
   HL7_AlrmLimtp_PAW_L,
   HL7_AlrmLimtp_MV_H,
   HL7_AlrmLimtp_MV_L,
   HL7_AlrmLimtp_F_H,
   HL7_AlrmLimtp_F_L,
   HL7_AlrmLimtp_FIO2_H,
   HL7_AlrmLimtp_FIO2_L,
   HL7_AlrmLimtp_VTE_H,
   HL7_AlrmLimtp_VTE_L,
#if (_HL_TYPE == _HL7_VENT) 
   HL7_AlrmLimtp_PEEP_H,
   HL7_AlrmLimtp_PEEP_L,
#endif   
   HL7_AlrmLimtp_ETCO2_H,
   HL7_AlrmLimtp_ETCO2_L,
   HL7_AlrmLimtp_FICO2_H,
   HL7_AlrmLimtp_FIN2O_H,
   HL7_AlrmLimtp_FIN2O_L,
   HL7_AlrmLimtp_ETAA_H,
   HL7_AlrmLimtp_ETAA_L,
   HL7_AlrmLimtp_FIAA_H,
   HL7_AlrmLimtp_FIAA_L,
   HL7_AlrmLimtp_PULSE_H,
   HL7_AlrmLimtp_PULSE_L,
   HL7_AlrmLimtp_SPO2_H,
   HL7_AlrmLimtp_SPO2_L,
   HL7_AlrmLimtp_PI_H,
   HL7_AlrmLimtp_PI_L,
   HL7_AlrmLimtp_Last,
}HL7_AlrmLimtp_t;


void HL7_AlrmLim_Send(void);
bool HL7_AlrmLim_Change(void);
void HL7_Read_AlrmLim(HL7_AlrmLimtp_t type, HL7_Para_t *para);

#endif
