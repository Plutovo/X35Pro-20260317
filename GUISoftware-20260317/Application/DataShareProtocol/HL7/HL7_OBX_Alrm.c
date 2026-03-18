#include "HL7_OBX_Alrm.h"
#include "HL7_cfg.h"
#include "HL7_Common.h"
#include "HL7_OBX.h"
#include "DataShare.h"
#include "uart_dp.h"
#include "HL7_maptble.h"

void HL7_Alrm_Send(void)
{
    u8 *OBXx[OBX26_Last] = {NULL};
    int size = 0;
    int cnt = 0;
    DS_AlrmMsg_t almmsg;
    char tmp_buf2[150];
    u8 send_buf[HL7_FRM_MAX_SIZE] = {0};
    
    OBXx[OBX1_SetID] = NULL;
    OBXx[OBX7_ReferRange] = NULL;
    OBXx[OBX8_AbnorFlags] = NULL;
    OBXx[OBX9_PR] = NULL;
    OBXx[OBX10_AbnorTest] = NULL;
    OBXx[OBX11_ObsResStatus] = HL7_RES_STATE_F;
    OBXx[OBX12_EffectDate] = NULL;
    OBXx[OBX13_UserDef] = NULL;
    OBXx[OBX14_ObsDate] = NULL;
    OBXx[OBX15_ProdID] = NULL;
    OBXx[OBX16_ResObs] = NULL;
    OBXx[OBX17_ObsMethod] = NULL;
    OBXx[OBX18_EquipID] = NULL;
    OBXx[OBX19_AnalyDate] = NULL;
    OBXx[OBX20_ObsSite] = NULL;
    OBXx[OBX21_ObsInsID] = NULL;
    OBXx[OBX22_ModeCode] = NULL;
    OBXx[OBX23_PerOrgName] = NULL;
    OBXx[OBX24_PerOrgAddr] = NULL;
    OBXx[OBX25_PerOrgMed] = NULL;
    //OBX|
    memcpy(send_buf, HL7_SEG_OBX, 3);
    send_buf[3] = HL7_ESC_FLD;
    
    OBXx[OBX2_ValType] = HL7_VAL_CE;
    OBXx[OBX6_Units] = NULL;

    while(ds_readmsg_alrmhigh(&almmsg) == true)
    {
        OBXx[OBX3_ObsID] = HL7_Alrm_High;
        OBXx[OBX4_ObsSubID] = HL7_MOD_ANES;             
        if(strlen((char *)almmsg.Msg) < 150)
        {
            sprintf(tmp_buf2, "%s^%s", (char *)HL7_map_read_alrmid_grp0(almmsg.ID), almmsg.Msg);
        }
        OBXx[OBX5_ObsVal] = (u8*)tmp_buf2;
        //OBXx[OBX14_ObsDate] = (u8*)tmp_buf;
        
        size = HL7_FillBuf(HL7_MSG_OBX_alrm, &send_buf[4], HL7_FRM_MAX_SIZE, OBXx, OBX26_Last);
        uart_dp_wrsend(send_buf, size+4);      
        
        //·ÀÖ¹ËÀÑ­»·
        cnt++;
        if(cnt > 64)
        {
            break;
        }
    }
#ifdef ALARMS1 
    while(ds_readmsg_alrmlow(&almmsg) == true)
    {
        if(almmsg.ID < ALARM_NUM1_MEDIUM)
        {
            OBXx[OBX3_ObsID] = HL7_Alrm_Medium;
        }
        else
        {
            OBXx[OBX3_ObsID] = HL7_Alrm_Low;
        }
        OBXx[OBX4_ObsSubID] = HL7_MOD_ANES; 
        if(strlen(almmsg.Msg) < 150)
        {
            sprintf(tmp_buf2, "%s^%s", HL7_map_read_alrmid_grp1(almmsg.ID), almmsg.Msg);
        }
        OBXx[OBX5_ObsVal] = (u8*)tmp_buf2;            
        //OBXx[OBX14_ObsDate] = (u8*)tmp_buf;
        
        size = HL7_FillBuf(HL7_MSG_OBX_alrm, &send_buf[4], HL7_FRM_MAX_SIZE, OBXx, OBX26_Last);
        uart_dp_wrsend(send_buf, size+4);
        
        //·ÀÖ¹ËÀÑ­»·
        cnt++;
        if(cnt > 64)
        {
            break;
        }
    }
#endif

}