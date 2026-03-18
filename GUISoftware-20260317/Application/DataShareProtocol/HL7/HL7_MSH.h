#ifndef _HL7_MSH_H
#define _HL7_MSH_H
#include "stdtypes.h"

typedef enum
{
    HL7_MSGID_None = 0,
    HL7_MSGID_Para,
    HL7_MSGID_CtrlPara,
    HL7_MSGID_Alrm,
    HL7_MSGID_AlrmLim,
    HL7_MSGID_Mesg,
    HL7_MSGID_Last,
}HL7_MSGID_t;

//HL7 MSH
typedef enum
{
    MSH1_FieldSeparator = 0,
    MSH2_EncodingChar,
    MSH3_SendApp,
    MSH4_SendFacility,
    MSH5_RecvApp,
    MSH6_RECVFacility,
    MSH7_Date,
    MSH8_Security,
    MSH9_MsgType,
    MSH10_MsgID,
    MSH11_ProID,
    MSH12_VerID,
    MSH13_SeqNum,
    MSH14_ContPtr,
    MSH15_AckType,
    MSH16_AppAckType,
    MSH17_CtryCode,
    MSH18_SetChar,
    MSH19_MsgLang,
    MSH20_ASetChar,
    MSH21_MsgProfileID,
    MSH22_SendResOrg,
    MSH23_RecvResOrg,
    MSH24_SendNetAddr,
    MSH25_RecvNetAddr,
    MSH26_Last,
}HL7_MSHx_t;

u32 HL7_MSH_Fill(HL7_MSGID_t msgid, u8 *buf);
void HL7_MSH_Init(void);
#endif
