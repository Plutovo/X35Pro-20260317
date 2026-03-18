#include "HL7_OBX_CtrlPara.h"
#include "HL7_cfg.h"
#include "HL7_Common.h"
#include "HL7_OBX.h"
#include "DataShare.h"
#include "uart_dp.h"
#include "HL7_maptble.h"

void HL7_CtrlPara_Send(void)
{
    u8 *OBXx[OBX26_Last] = {NULL};
    HL7_Para_t para = {0};
    int size = 0;
    char tmp_buf[32];
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
    

    OBXx[OBX2_ValType] = HL7_VAL_NM;        
    for(int i=HL7_CtrlParatp_None+1; i<HL7_CtrlParatp_Last; i++)
    {
        HL7_Read_CtrlPara((HL7_CtrlParatp_t)HL7_map_read_ctrlparaid(i), &para);
        if(para.ID != NULL && para.name != NULL)
        {
            OBXx[OBX4_ObsSubID] = (u8*)para.subID;
            OBXx[OBX5_ObsVal] = (u8*)para.value;
            OBXx[OBX6_Units] = (u8*)para.unit;
            sprintf(tmp_buf, "%s^%s", para.ID, para.name);
            OBXx[OBX3_ObsID] = (u8*)tmp_buf;
            size = HL7_FillBuf(HL7_MSG_OBX_ctrl, &send_buf[4], HL7_FRM_MAX_SIZE, OBXx, OBX26_Last);
            size += 4;
            uart_dp_wrsend(send_buf, size);
            HL7_Para_clr(&para);
        }
    }
}


bool HL7_CtrlPara_Change(void)
{
    static DS_CtrlPara_t tmp_ctrlpara = {0};
    bool flag = false;

    if(ds_readctrlpara_PEEP() != tmp_ctrlpara.DS_CtrlPara_PEEP)
    {
        tmp_ctrlpara.DS_CtrlPara_PEEP = ds_readctrlpara_PEEP();
        flag = true;
    }
    if(ds_readctrlpara_VT() != tmp_ctrlpara.DS_CtrlPara_VT)
    {
        tmp_ctrlpara.DS_CtrlPara_VT = ds_readctrlpara_VT();
        flag = true;
    }
    if(ds_readctrlpara_f() != tmp_ctrlpara.DS_CtrlPara_f)
    {
        tmp_ctrlpara.DS_CtrlPara_f = ds_readctrlpara_f();
        flag = true;
    }
    if(ds_readctrlpara_IE() != tmp_ctrlpara.DS_CtrlPara_IE)
    {
        tmp_ctrlpara.DS_CtrlPara_IE = ds_readctrlpara_IE();
        flag = true;
    }
    if(ds_readctrlpara_Tslope() != tmp_ctrlpara.DS_CtrlPara_Tslope)
    {
        tmp_ctrlpara.DS_CtrlPara_Tslope = ds_readctrlpara_Tslope();
        flag = true;
    }
    if(ds_readctrlpara_Tinsp() != tmp_ctrlpara.DS_CtrlPara_Tinsp)
    {
        tmp_ctrlpara.DS_CtrlPara_Tinsp = ds_readctrlpara_Tinsp();
        flag = true;
    }
    if(ds_readctrlpara_Pinsp() != tmp_ctrlpara.DS_CtrlPara_Pinsp)
    {
        tmp_ctrlpara.DS_CtrlPara_Pinsp = ds_readctrlpara_Pinsp();
        flag = true;
    }
    if(ds_readctrlpara_Psupp() != tmp_ctrlpara.DS_CtrlPara_Psupp)
    {
        tmp_ctrlpara.DS_CtrlPara_Psupp = ds_readctrlpara_Psupp();
        flag = true;
    }
    if(ds_readctrlpara_Tpause() != tmp_ctrlpara.DS_CtrlPara_Tpause)
    {
        tmp_ctrlpara.DS_CtrlPara_Tpause = ds_readctrlpara_Tpause();
        flag = true;
    }
    if(ds_readctrlpara_Esens() != tmp_ctrlpara.DS_CtrlPara_Esens)
    {
        tmp_ctrlpara.DS_CtrlPara_Esens = ds_readctrlpara_Esens();
        flag = true;
    }
    if(ds_readctrlpara_Psens() != tmp_ctrlpara.DS_CtrlPara_Psens)
    {
        tmp_ctrlpara.DS_CtrlPara_Psens = ds_readctrlpara_Psens();
        flag = true;
    }
    if(ds_readctrlpara_Fsens() != tmp_ctrlpara.DS_CtrlPara_Fsens)
    {
        tmp_ctrlpara.DS_CtrlPara_Fsens = ds_readctrlpara_Fsens();
        flag = true;
    }
    if(ds_readctrlpara_Sigh() != tmp_ctrlpara.DS_CtrlPara_Sigh)
    {
        tmp_ctrlpara.DS_CtrlPara_Sigh = ds_readctrlpara_Sigh();
        flag = true;
    }
#if (_HL_TYPE == _HL7_ANES)
    if(ds_readctrlpara_FG() != tmp_ctrlpara.DS_CtrlPara_FG)
    {
        tmp_ctrlpara.DS_CtrlPara_FG = ds_readctrlpara_FG();
        flag = true;
    }
    if(ds_readctrlpara_PerO2() != tmp_ctrlpara.DS_CtrlPara_PerO2)
    {
        tmp_ctrlpara.DS_CtrlPara_PerO2 = ds_readctrlpara_PerO2();
        flag = true;
    }   
#else
    if(ds_readctrlpara_Phigh() != tmp_ctrlpara.DS_CtrlPara_Phigh)
    {
        tmp_ctrlpara.DS_CtrlPara_Phigh = ds_readctrlpara_Phigh();
        flag = true;
    }
    if(ds_readctrlpara_Plow() != tmp_ctrlpara.DS_CtrlPara_Plow)
    {
        tmp_ctrlpara.DS_CtrlPara_Plow = ds_readctrlpara_Plow();
        flag = true;
    }
    if(ds_readctrlpara_Thigh() != tmp_ctrlpara.DS_CtrlPara_Thigh)
    {
        tmp_ctrlpara.DS_CtrlPara_Thigh = ds_readctrlpara_Thigh();
        flag = true;
    }
    if(ds_readctrlpara_Tlow() != tmp_ctrlpara.DS_CtrlPara_Tlow)
    {
        tmp_ctrlpara.DS_CtrlPara_Tlow = ds_readctrlpara_Tlow();
        flag = true;
    }   
#endif
    
    return flag;
}

#if (_HL_TYPE == _HL7_ANES)
void HL7_Read_CtrlPara(HL7_CtrlParatp_t type, HL7_Para_t *para)
{
    switch(type)
    {
    case HL7_CtrlParatp_PEEP:
        para->ID = HL7_SET_PEEP;
        para->subID = HL7_MOD_ANES;
        para->name = Sys_Lib_GetDispStr(DP_PEEP);
        sprintf(para->value, "%d", ds_readctrlpara_PEEP());
        para->unit = Sys_Lib_GetDispStr(DP_CMH2O);
        break;
    case HL7_CtrlParatp_VT:
        para->ID = HL7_SET_VT;
        para->subID = HL7_MOD_ANES;
        para->name = Sys_Lib_GetDispStr(DP_VT);
        sprintf(para->value, "%d", ds_readctrlpara_VT());
        para->unit = Sys_Lib_GetDispStr(DP_ML);
        break;
    case HL7_CtrlParatp_f:
        para->ID = HL7_SET_F;
        para->subID = HL7_MOD_ANES;
        para->name = Sys_Lib_GetDispStr(DP_F);
        sprintf(para->value, "%d", ds_readctrlpara_f());
        para->unit = Sys_Lib_GetDispStr(DP_BPM);
        break;
    case HL7_CtrlParatp_IEI:
        para->ID = HL7_SET_IEI;
        para->subID = HL7_MOD_ANES;
        para->name = Sys_Lib_GetDispStr(DP_IEI);

        if(ds_readctrlpara_IE() >= 6)
        {
            sprintf(para->value,"1");
        }
        else
        {
            sprintf(para->value,"%2.1f", IE_readRatio(ds_readctrlpara_IE())/10.0);
        }
        para->unit = NULL;
        break;
    case HL7_CtrlParatp_IEE:
        para->ID = HL7_SET_IEE;
        para->subID = HL7_MOD_ANES;
        para->name = Sys_Lib_GetDispStr(DP_IEE);

        if(ds_readctrlpara_IE() >= 6)
        {
            if(ds_readctrlpara_IE() != 24)
            {
                sprintf(para->value,"%2.1f", IE_readRatio(ds_readctrlpara_IE())/10.0);
            }
            else
            {
                sprintf(para->value,"%2.0f", IE_readRatio(ds_readctrlpara_IE())/10.0);
            }
        }
        else
        {
            sprintf(para->value,"1");
        }
        para->unit = NULL;        
        break;
    case HL7_CtrlParatp_Tslope:
        para->ID = HL7_SET_TSLOPE;
        para->subID = HL7_MOD_ANES;
        para->name = Sys_Lib_GetDispStr(DP_TSLOPE);
        sprintf(para->value, "%.1f", ds_readctrlpara_Tslope()/100.0);
        para->unit = Sys_Lib_GetDispStr(DP_S);
        break;
    case HL7_CtrlParatp_Tinsp:
        para->ID = HL7_SET_TINSP;
        para->subID = HL7_MOD_ANES;
        para->name = Sys_Lib_GetDispStr(DP_TINSP);
        sprintf(para->value, "%.2f", ds_readctrlpara_Tinsp()/100.0);
        para->unit = Sys_Lib_GetDispStr(DP_S);    
        break;
    case HL7_CtrlParatp_Pinsp:
        para->ID = HL7_SET_PINSP;
        para->subID = HL7_MOD_ANES;
        para->name = Sys_Lib_GetDispStr(DP_PINSP);
        sprintf(para->value, "%d", ds_readctrlpara_Pinsp());
        para->unit = Sys_Lib_GetDispStr(DP_CMH2O);      
        break;
    case HL7_CtrlParatp_Psupp:
        para->ID = HL7_SET_PUSPP;
        para->subID = HL7_MOD_ANES;
        para->name = Sys_Lib_GetDispStr(DP_PSUPP);
        sprintf(para->value, "%d", ds_readctrlpara_Psupp());
        para->unit = Sys_Lib_GetDispStr(DP_CMH2O);        
        break;
    case HL7_CtrlParatp_Tpause:
        para->ID = HL7_SET_TPAUSE;
        para->subID = HL7_MOD_ANES;
        para->name = Sys_Lib_GetDispStr(DP_PAUSE);
        sprintf(para->value, "%d", ds_readctrlpara_Tpause());
        para->unit = Sys_Lib_GetDispStr(DP_PERCENT);
        break;
    case HL7_CtrlParatp_Esens:
        para->ID = HL7_SET_ESENS;
        para->subID = HL7_MOD_ANES;
        para->name = Sys_Lib_GetDispStr(DP_ESENS);
        sprintf(para->value, "%d", ds_readctrlpara_Esens());
        para->unit = Sys_Lib_GetDispStr(DP_PERCENT);     
        break;
    case HL7_CtrlParatp_Psens:
        para->ID = HL7_SET_PSENS;
        para->subID = HL7_MOD_ANES;
        para->name = Sys_Lib_GetDispStr(DP_PSENS);
        sprintf(para->value, "%d", ds_readctrlpara_Psens());
        para->unit = Sys_Lib_GetDispStr(DP_CMH2O);       
        break;
    case HL7_CtrlParatp_Fsens:
        para->ID = HL7_SET_FSENS;
        para->subID = HL7_MOD_ANES;
        para->name = Sys_Lib_GetDispStr(DP_FSENS);
        sprintf(para->value, "%.1f", ds_readctrlpara_Fsens()/10.0);
        para->unit = Sys_Lib_GetDispStr(DP_LPM);       
        break;
    case HL7_CtrlParatp_Sigh:
        if(ds_readpara_BreathMode() == DS_Mode_SIGH)    //当前模式是SIGH，发送SIGH的数值
        {
            para->ID = HL7_SET_SIGH;
            para->subID = HL7_MOD_ANES;
            para->name = Sys_Lib_GetDispStr(DP_SIGH);
            sprintf(para->value, "%d", ds_readctrlpara_Sigh());
        }
        else
        {
            para->ID = NULL;
            para->name = NULL;
            para->value[0] = 0;            
        }
        para->unit = NULL;        
        break;
    case HL7_CtrlParatp_FG:
        para->ID = HL7_SET_FG;
        para->subID = HL7_MOD_ANES;
        para->name = Sys_Lib_GetDispStr(DP_FG);
        sprintf(para->value, "%.1f", ds_readctrlpara_FG()/10.0);
        para->unit = Sys_Lib_GetDispStr(DP_LMIN);      
        break;
    case HL7_CtrlParatp_PerO2:
        para->ID = HL7_SET_O2Per;
        para->subID = HL7_MOD_ANES;
        para->name = Sys_Lib_GetDispStr(DP_O2);
        sprintf(para->value, "%d", ds_readctrlpara_PerO2());
        para->unit = Sys_Lib_GetDispStr(DP_PERCENT);      
        break;
    default:
        para->ID = NULL;
        para->name = NULL;
        para->value[0] = 0;
        para->unit = NULL;
        break;
    }
}

#else
void HL7_Read_CtrlPara(HL7_CtrlParatp_t type, HL7_Para_t *para)
{
    switch(type)
    {
    case HL7_CtrlParatp_PEEP:
        para->ID = HL7_VENTSET_PEEP;
        para->subID = HL7_MOD_VENT;
        para->name = (char *)*Global_DS[DISP_PEEP];
        sprintf(para->value, "%d", ds_readctrlpara_PEEP());
        para->unit = (char *)*Global_DS[DISP_UNIT_CMH2O];
        break;
    case HL7_CtrlParatp_VT:
        para->ID = HL7_VENTSET_VT;
        para->subID = HL7_MOD_VENT;
        para->name = (char *)*Global_DS[DISP_VT];
        sprintf(para->value, "%d", ds_readctrlpara_VT());
        para->unit = (char *)*Global_DS[DISP_UNIT_ML];
        break;
    case HL7_CtrlParatp_f:
        para->ID = HL7_VENTSET_F;
        para->subID = HL7_MOD_VENT;
        para->name = (char *)*Global_DS[DISP_FTOTAL];
        sprintf(para->value, "%d", ds_readctrlpara_f());
        para->unit = (char *)*Global_DS[DISP_UNIT_ML];
        break;
    case HL7_CtrlParatp_IEI:
        para->ID = HL7_VENTSET_IEI;
        para->subID = HL7_MOD_VENT;
        para->name = "I";

        if(ds_readctrlpara_IE() >= 6)
        {
            sprintf(para->value,"1");
        }
        else
        {
            sprintf(para->value,"%2.1f", IE_readRatio(ds_readctrlpara_IE())/10.0);
        }
        para->unit = NULL;
        break;
    case HL7_CtrlParatp_IEE:
        para->ID = HL7_VENTSET_IEE;
        para->subID = HL7_MOD_VENT;
        para->name = "E";

        if(ds_readctrlpara_IE() >= 6)
        {
            if(ds_readctrlpara_IE() != 24)
            {
                sprintf(para->value,"%2.1f", IE_readRatio(ds_readctrlpara_IE())/10.0);
            }
            else
            {
                sprintf(para->value,"%2.0f", IE_readRatio(ds_readctrlpara_IE())/10.0);
            }
        }
        else
        {
            sprintf(para->value,"1");
        }
        para->unit = NULL;        
        break;
    case HL7_CtrlParatp_Tslope:
        para->ID = HL7_VENTSET_TSLOPE;
        para->subID = HL7_MOD_VENT;
        para->name = (char *)*Global_DS[DISP_TSLOPE];
        sprintf(para->value, "%.1f", ds_readctrlpara_Tslope()/100.0);
        para->unit = (char *)*Global_DS[DISP_UNIT_TIME_S];
        break;
    case HL7_CtrlParatp_Tinsp:
        para->ID = HL7_VENTSET_TINSP;
        para->subID = HL7_MOD_ANES;
        para->name = (char *)*Global_DS[DISP_TI];
        sprintf(para->value, "%.2f", ds_readctrlpara_Tinsp()/100.0);
        para->unit = (char *)*Global_DS[DISP_UNIT_TIME_S];    
        break;
    case HL7_CtrlParatp_Pinsp:
        para->ID = HL7_VENTSET_PINSP;
        para->subID = HL7_MOD_VENT;
        para->name = (char *)*Global_DS[DISP_PINSP];
        sprintf(para->value, "%d", ds_readctrlpara_Pinsp());
        para->unit = (char *)*Global_DS[DISP_UNIT_CMH2O];      
        break;
    case HL7_CtrlParatp_Psupp:
        para->ID = HL7_VENTSET_PSUPP;
        para->subID = HL7_MOD_VENT;
        para->name = (char *)*Global_DS[DISP_PSUPP];
        sprintf(para->value, "%d", ds_readctrlpara_Psupp());
        para->unit = (char *)*Global_DS[DISP_UNIT_CMH2O];        
        break;
    case HL7_CtrlParatp_Tpause:
        para->ID = HL7_VENTSET_PAUSE;
        para->subID = HL7_MOD_VENT;
        para->name = (char *)*Global_DS[DISP_PAUSE];
        sprintf(para->value, "%d", ds_readctrlpara_Tpause());
        para->unit = (char *)*Global_DS[DISP_UNIT_PERCENT];
        break;
    case HL7_CtrlParatp_Esens:
        para->ID = HL7_VENTSET_ESENS;
        para->subID = HL7_MOD_VENT;
        para->name = (char *)*Global_DS[DISP_ESENS];
        sprintf(para->value, "%d", ds_readctrlpara_Esens());
        para->unit = (char *)*Global_DS[DISP_UNIT_PERCENT];     
        break;
    case HL7_CtrlParatp_Psens:
        para->ID = HL7_VENTSET_PSENS;
        para->subID = HL7_MOD_VENT;
        para->name = (char *)*Global_DS[DISP_PSENS];
        sprintf(para->value, "%d", ds_readctrlpara_Psens());
        para->unit = (char *)*Global_DS[DISP_UNIT_CMH2O];       
        break;
    case HL7_CtrlParatp_Fsens:
        para->ID = HL7_VENTSET_FSENS;
        para->subID = HL7_MOD_VENT;
        para->name = (char *)*Global_DS[DISP_FSENS];
        sprintf(para->value, "%.1f", ds_readctrlpara_Fsens()/10.0);
        para->unit = (char *)*Global_DS[DISP_UNIT_LPM];       
        break;
    case HL7_CtrlParatp_Sigh:
        if(ds_readpara_BreathMode() == DS_Mode_SIGH)    //当前模式是SIGH，发送SIGH的数值
        {
            para->ID = HL7_VENTSET_SIGH;
            para->subID = HL7_MOD_VENT;
            para->name = "SIGH";
            sprintf(para->value, "%d", ds_readctrlpara_Sigh());
        }
        else
        {
            para->ID = NULL;
            para->name = NULL;
            para->value[0] = 0;            
        }
        para->unit = NULL;        
        break;
    case HL7_CtrlParatp_Phigh:
        para->ID = HL7_VENTSET_PHIGH;
        para->subID = HL7_MOD_VENT;
        para->name = (char *)*Global_DS[DISP_PHIGH];
        sprintf(para->value, "%.1f", ds_readctrlpara_Phigh());
        para->unit = (char *)*Global_DS[DISP_UNIT_CMH2O];      
        break;
    case HL7_CtrlParatp_Plow:
        para->ID = HL7_VENTSET_PLOW;
        para->subID = HL7_MOD_VENT;
        para->name = (char *)*Global_DS[DISP_PLOW];
        sprintf(para->value, "%d", ds_readctrlpara_Plow());
        para->unit = (char *)*Global_DS[DISP_UNIT_CMH2O];      
        break;
    case HL7_CtrlParatp_Thigh:
        para->ID = HL7_VENTSET_THIGH;
        para->subID = HL7_MOD_VENT;
        para->name = (char *)*Global_DS[DISP_THIGH];
        sprintf(para->value, "%.1f", ds_readctrlpara_Thigh()/10.0);
        para->unit = (char *)*Global_DS[DISP_UNIT_TIME_S];      
        break;
    case HL7_CtrlParatp_Tlow:
        para->ID = HL7_VENTSET_TLOW;
        para->subID = HL7_MOD_VENT;
        para->name = (char *)*Global_DS[DISP_TLOW];
        sprintf(para->value, "%.1f", ds_readctrlpara_Tlow()/10.0);
        para->unit = (char *)*Global_DS[DISP_UNIT_TIME_S];      
        break;        
    default:
        para->ID = NULL;
        para->name = NULL;
        para->value[0] = 0;
        para->unit = NULL;        
        break;
    }
}
#endif




