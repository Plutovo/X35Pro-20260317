#include "HL7_OBX_Msg.h"
#include "HL7_cfg.h"
#include "HL7_Common.h"
#include "HL7_OBX.h"
#include "uart_dp.h"
#include "HL7_maptble.h"

void HL7_Msg_Send(void)
{
    u8 *OBXx[OBX26_Last] = {NULL};
    HL7_Para_t para = {0};
    int size = 0;
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
    

    //文本信息发送
    //待机/工作
    OBXx[OBX3_ObsID] = HL7_Alrm_Msg;   
#if (_HL_TYPE == _HL7_ANES)    
    OBXx[OBX4_ObsSubID] = HL7_MOD_ANES; 
#else
    OBXx[OBX4_ObsSubID] = HL7_MOD_VENT; 
#endif
    if(ds_readpara_StdyState() != 0)
    {
#if (_HL_TYPE == _HL7_ANES)          
        sprintf(tmp_buf2, "%s^%s", HL7_MSG_STDBY, Sys_Lib_GetDispStr(DP_STANDBY)); 
#else
        sprintf(tmp_buf2, "%s^%s", HL7_MSG_STDBY, "Standby"); 
#endif
    }
    else
    {
#if (_HL_TYPE == _HL7_ANES)         
        sprintf(tmp_buf2, "%s^%s", HL7_MSG_WORK, Sys_Lib_GetDispStr(DP_WORK));     
#else
        sprintf(tmp_buf2, "%s^%s", HL7_MSG_WORK, "Work");
#endif        
    }
    OBXx[OBX5_ObsVal] = (u8*)tmp_buf2; 
    size = HL7_FillBuf(HL7_MSG_OBX_msg, &send_buf[4], HL7_FRM_MAX_SIZE, OBXx, OBX26_Last);
    uart_dp_wrsend(send_buf, size+4); 
    
    //模式
    OBXx[OBX3_ObsID] = HL7_Alrm_Msg;
    HL7_Read_Mode(ds_readpara_BreathMode(), &para);
    sprintf(tmp_buf2, "%s^%s", (char *)HL7_map_read_mode(ds_readpara_BreathMode()), para.name);                       
    OBXx[OBX5_ObsVal] = (u8*)tmp_buf2; 
    size = HL7_FillBuf(HL7_MSG_OBX_msg, &send_buf[4], HL7_FRM_MAX_SIZE, OBXx, OBX26_Last);
    size += 4;
    uart_dp_wrsend(send_buf, size); 
    HL7_Para_clr(&para);
}
    
#if (_HL_TYPE == _HL7_ANES)
void HL7_Read_Mode(DS_Mode_t mode, HL7_Para_t *para)
{
    switch(mode)
    {
    case DS_Mode_VCV:
    default:
        para->name = Sys_Lib_GetDispStr(DP_VCV);
        break;
    case DS_Mode_SIMVV:
        para->name = Sys_Lib_GetDispStr(DP_SIMVV);
        break;
    case DS_Mode_PCV:
        para->name = Sys_Lib_GetDispStr(DP_PCV);
        break;
    case DS_Mode_SIMVP:
        para->name = Sys_Lib_GetDispStr(DP_SIMVP);
        break;
    case DS_Mode_PRVC:
        para->name = Sys_Lib_GetDispStr(DP_PRVC);
        break;
    case DS_Mode_SIMVPRVC:
        para->name = Sys_Lib_GetDispStr(DP_SIMVPRVC);
        break;
    case DS_Mode_PSV:
        para->name = Sys_Lib_GetDispStr(DP_SPONTPSV);
        break;
    case DS_Mode_SIGH:
        para->name = Sys_Lib_GetDispStr(DP_SIGH);
        break;
    case DS_Mode_MANUAL:
        para->name = Sys_Lib_GetDispStr(DP_MANUAL);
        break;
    case DS_Mode_ACGO:
        para->name = Sys_Lib_GetDispStr(DP_ACGO);
        break;   
    case DS_Mode_Backup:
        para->name = Sys_Lib_GetDispStr(DP_BACKUP);
        break;
    }
}
#else
void HL7_Read_Mode(DS_Mode_t mode, HL7_Para_t *para)
{
    switch(mode)
    {
    case DS_Mode_VCV:
    default:
        para->name = (char *)*Global_DS[DISP_MODE_VCV];
        break;
    case DS_Mode_SIMVV:
        para->name = (char *)*Global_DS[DISP_MODE_SIMVVC];
        break;
    case DS_Mode_PCV:
        para->name = (char *)*Global_DS[DISP_MODE_PCV];
        break;
    case DS_Mode_SIMVP:
        para->name = (char *)*Global_DS[DISP_MODE_SIMVPC];
        break;
    case DS_Mode_PRVC:
        para->name = (char *)*Global_DS[DISP_MODE_PRVC];
        break;
    case DS_Mode_SIMVPRVC:
        para->name = (char *)*Global_DS[DISP_MODE_VCV];
        break;
    case DS_Mode_PSV:
        para->name = (char *)*Global_DS[DISP_MODE_CPAPSPONT];
        break;
    case DS_Mode_SIGH:
        para->name = "Sigh";
        break;  
    case DS_Mode_Backup:
        para->name = "Backup";
        break;
    case DS_Mode_Duo:
        para->name = (char *)*Global_DS[DISP_MODE_DUOPHASIC];
        break;
    case DS_Mode_APRV:
        para->name = (char *)*Global_DS[DISP_MODE_APRV];
        break;
    case DS_Mode_VS:
        para->name = (char *)*Global_DS[DISP_MODE_VS];
        break;
    case DS_Mode_NIVPC:
        para->name = (char *)*Global_DS[DISP_MODE_NIVPC];
        break;
    case DS_Mode_NIVPSV:
        para->name = (char *)*Global_DS[DISP_MODE_NIVCPAP];
        break;
    }
}
#endif
