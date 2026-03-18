#include "HL7_MSH.h"
#include "HL7_cfg.h"
#include "HL7_Common.h"
#include "HL7_inc_ext.h"

static char *MSGID_buf[HL7_MSGID_Last] POOL_EXRAM;

void HL7_MSH_Init(void)
{ 
    MSGID_buf[HL7_MSGID_None]       = NULL;
    MSGID_buf[HL7_MSGID_Para]       = HL7_MSG_ID_Para;
    MSGID_buf[HL7_MSGID_CtrlPara]   = HL7_MSG_ID_CtrlID;
    MSGID_buf[HL7_MSGID_Alrm]       = HL7_MSG_ID_Alrm;
    MSGID_buf[HL7_MSGID_AlrmLim]    = HL7_MSG_ID_AlrmLim;
    MSGID_buf[HL7_MSGID_Mesg]       = HL7_MSG_ID_Mesg;     
}

u32 HL7_MSH_Fill(HL7_MSGID_t msgid, u8 *buf)
{
    u8 *MSHx[MSH26_Last] = {NULL};
    u32 size = 0;
    char tmp_SN[32] = {0};
    
    MSHx[MSH1_FieldSeparator]   = HL7_SEG_MSH;
    MSHx[MSH2_EncodingChar]     = HL7_ESC;
    MSHx[MSH3_SendApp]          = HL7_SEND_APP;
    sprintf(tmp_SN, "%s^%s", HL7_SEND_FCLT, HL7_GetSN());
    MSHx[MSH4_SendFacility]     = (u8 *)tmp_SN;  
    MSHx[MSH5_RecvApp]          = NULL;
    MSHx[MSH6_RECVFacility]     = NULL;
    MSHx[MSH7_Date]             = NULL;
    MSHx[MSH8_Security]         = NULL;
    MSHx[MSH9_MsgType]          = HL7_MSG_TYPE_ORU;
    MSHx[MSH10_MsgID]           = (u8 *)MSGID_buf[msgid];
    MSHx[MSH11_ProID]           = HL7_PID_P;
    MSHx[MSH12_VerID]           = HL7_VER; 
    MSHx[MSH13_SeqNum]          = NULL;
    MSHx[MSH14_ContPtr]         = NULL;
    MSHx[MSH15_AckType]         = NULL;
    MSHx[MSH16_AppAckType]      = NULL;
    MSHx[MSH17_CtryCode]        = NULL;
    MSHx[MSH18_SetChar]         = HL7_SET_UTF8;
    MSHx[MSH19_MsgLang]         = NULL;
    MSHx[MSH20_ASetChar]        = NULL;
    MSHx[MSH21_MsgProfileID]    = NULL;
    MSHx[MSH22_SendResOrg]      = NULL;
    MSHx[MSH23_RecvResOrg]      = NULL;
    MSHx[MSH24_SendNetAddr]     = NULL;
    MSHx[MSH25_RecvNetAddr]     = NULL;
    
    size = HL7_FillBuf(HL7_MSG_MSH, buf, HL7_FRM_MAX_SIZE, MSHx, MSH26_Last);
    return size;
}

