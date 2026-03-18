#ifndef _HL7_OBR_H
#define _HL7_OBR_H
#include "stdtypes.h"

//HL7 OBR
typedef enum
{
    OBR1_SetID = 0,
    OBR2_PONum,
    OBR3_FONum,
    OBR4_SID,
    OBR5_ProOBR,
    OBR6_ReqTime,
    OBR7_ObsTime,
    OBRX_Last,
}HL7_OBRx_t;


u32 HL7_OBR_Fill(u8 *buf);

#endif
