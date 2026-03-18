#include "HL7_PID.h"
#include "HL7_cfg.h"
#include "HL7_Common.h"
#include "string.h"
#include "DataShare.h"

u32 HL7_PID_Fill(u8 *buf)
{
    u8 *PIDx[PIDX_Last] = {NULL};
    u32 size = 0;
    u8 tmp_buf[2] = {0};
    
    PIDx[PID1_SetID] = NULL;
    PIDx[PID2_PID] = NULL;
    PIDx[PID3_PIDList] = (u8 *)ds_readpatient_id(); //
    PIDx[PID4_APID] = NULL;
    PIDx[PID5_PName] = (u8 *)ds_readpatient_name();   //
    PIDx[PID6_MName] = NULL;
    PIDx[PID7_Brith] = 0;   //
    tmp_buf[0] = (u8)ds_readpatient_gender();
    tmp_buf[1] = '\0';
    PIDx[PID8_Sex] = tmp_buf;     // 
    PIDx[PID9_Alias] = NULL;
    PIDx[PID10_Race] = NULL;
    PIDx[PID11_Addr] = 0;   //
    PIDx[PID12_CY] = NULL;
    PIDx[PID13_Phone] = 0;  //
    //PID|
    memcpy(buf, HL7_SEG_PID, 3);  
    buf[3] = HL7_ESC_FLD;
    size = HL7_FillBuf(HL7_MSG_PID, &buf[4], HL7_FRM_MAX_SIZE, PIDx, PIDX_Last);
    size += 4;  //PID|
    
    return size;
}
