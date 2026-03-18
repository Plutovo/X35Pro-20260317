#ifndef _HL7_PID_H
#define _HL7_PID_H
#include "stdtypes.h"

typedef enum
{
    PID1_SetID = 0,
    PID2_PID,
    PID3_PIDList,
    PID4_APID,
    PID5_PName,
    PID6_MName,
    PID7_Brith,
    PID8_Sex,
    PID9_Alias,
    PID10_Race,
    PID11_Addr,
    PID12_CY,
    PID13_Phone,
    PIDX_Last,
}HL7_PIDx_t;

u32 HL7_PID_Fill(u8 *buf);

#endif
