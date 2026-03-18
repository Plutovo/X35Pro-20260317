#include "HL7_OBR.h"
#include "HL7_cfg.h"
#include "HL7_Common.h"
#include "HL7_inc_ext.h"

u32 HL7_OBR_Fill(u8 *buf)
{
    u8 *OBRx[OBRX_Last] = {NULL};
    //volatile 
    RTC_TIMER time = {0};
    u32 size = 0;
    char timebuf[30] = {0};
    char UID[32] = {0};
    
    sprintf(UID, "%s^%s %s", HL7_GetSN(), HL7_SEND_APP, HL7_SEND_FCLT);
    time = RTCReadTime(); 
    sprintf(timebuf, "%04d%02d%02d%02d%02d%02d", 
            time.RTC_Year, 
            time.RTC_Mon, 
            time.RTC_Mday, 
            time.RTC_Hour, 
            time.RTC_Min, 
            time.RTC_Sec);
    OBRx[OBR1_SetID] = NULL;
    OBRx[OBR2_PONum] = NULL;
    OBRx[OBR3_FONum] = NULL;
    OBRx[OBR4_SID] = (u8 *)UID;
    OBRx[OBR5_ProOBR] = NULL;
    OBRx[OBR6_ReqTime] = NULL;
    OBRx[OBR7_ObsTime] = (u8 *)timebuf; 
    //OBR|
    memcpy(buf, HL7_SEG_OBR, 3);
    buf[3] = HL7_ESC_FLD;
    size = HL7_FillBuf(HL7_MSG_OBR, &buf[4], HL7_FRM_MAX_SIZE, OBRx, OBRX_Last);
    size += 4;  //OBR|
    
    return size;
}
