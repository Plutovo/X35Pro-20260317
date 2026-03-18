#include "HL7_OBX_AlrmLim.h"
#include "HL7_cfg.h"
#include "HL7_Common.h"
#include "HL7_OBX.h"
#include "DataShare.h"
#include "uart_dp.h"
#include "HL7_maptble.h"

void HL7_AlrmLim_Send(void)
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
    for(int i=HL7_AlrmLimtp_None+1; i<HL7_AlrmLimtp_Last; i++)
    {
        HL7_Read_AlrmLim((HL7_AlrmLimtp_t)HL7_map_read_alrmlimid(i), &para);
        if(para.ID != NULL && para.subID != NULL)
        {
            OBXx[OBX4_ObsSubID] = (u8*)para.subID;
            OBXx[OBX5_ObsVal] = (u8*)para.value;
            OBXx[OBX6_Units] = (u8*)para.unit;
            sprintf(tmp_buf, "%s^", para.ID);
            OBXx[OBX3_ObsID] = (u8*)tmp_buf;
            size = HL7_FillBuf(HL7_MSG_OBX_alrmlim, &send_buf[4], HL7_FRM_MAX_SIZE, OBXx, OBX26_Last);
            size += 4;
            uart_dp_wrsend(send_buf, size);
            HL7_Para_clr(&para);
        }
    }
}


bool HL7_AlrmLim_Change(void)
{
    static DS_AlrmLims_t tmp_alrmlim = {0};
    bool flag = false;
    
    if(ds_readhighlims_PAW() != tmp_alrmlim.High.DS_AlrmLim_PAW)
    {
        tmp_alrmlim.High.DS_AlrmLim_PAW = ds_readhighlims_PAW();
        flag = true;
    }
    if(ds_readlowlims_PAW() != tmp_alrmlim.Low.DS_AlrmLim_PAW)
    {
        tmp_alrmlim.Low.DS_AlrmLim_PAW = ds_readlowlims_PAW();
        flag = true;
    }        
    if(ds_readhighlims_MV() != tmp_alrmlim.High.DS_AlrmLim_MV)
    {
        tmp_alrmlim.High.DS_AlrmLim_MV = ds_readhighlims_MV();
        flag = true;
    }        
    if(ds_readlowlims_MV() != tmp_alrmlim.Low.DS_AlrmLim_MV)
    {
        tmp_alrmlim.Low.DS_AlrmLim_MV = ds_readlowlims_MV();
        flag = true;
    }        
    if(ds_readhighlims_F() != tmp_alrmlim.High.DS_AlrmLim_F)
    {
        tmp_alrmlim.High.DS_AlrmLim_F = ds_readhighlims_F();
        flag = true;
    }        
    if(ds_readlowlims_F() != tmp_alrmlim.Low.DS_AlrmLim_F)
    {
        tmp_alrmlim.Low.DS_AlrmLim_F = ds_readlowlims_F();
        flag = true;
    }        
    if(ds_readhighlims_FiO2() != tmp_alrmlim.High.DS_AlrmLim_FiO2)
    {
        tmp_alrmlim.High.DS_AlrmLim_FiO2 = ds_readhighlims_FiO2();
        flag = true;
    }
    if(ds_readlowlims_FiO2() != tmp_alrmlim.Low.DS_AlrmLim_FiO2)
    {
        tmp_alrmlim.Low.DS_AlrmLim_FiO2 = ds_readlowlims_FiO2();
        flag = true;
    }        
    if(ds_readhighlims_VTe() != tmp_alrmlim.High.DS_AlrmLim_VTe)
    {
        tmp_alrmlim.High.DS_AlrmLim_VTe = ds_readhighlims_VTe();
        flag = true;
    }        
    if(ds_readlowlims_VTe() != tmp_alrmlim.Low.DS_AlrmLim_VTe)
    {
        tmp_alrmlim.Low.DS_AlrmLim_VTe = ds_readlowlims_VTe();
        flag = true;
    } 
    
    if(ds_readhighlims_EtCO2() != tmp_alrmlim.High.DS_AlrmLim_EtCO2)
    {
        tmp_alrmlim.High.DS_AlrmLim_EtCO2 = ds_readhighlims_EtCO2();
        flag = true;
    }        
    if(ds_readlowlims_EtCO2() != tmp_alrmlim.Low.DS_AlrmLim_EtCO2)
    {
        tmp_alrmlim.Low.DS_AlrmLim_EtCO2 = ds_readlowlims_EtCO2();
        flag = true;
    }        
    if(ds_readhighlims_FiCO2() != tmp_alrmlim.High.DS_AlrmLim_FiCO2)
    {
        tmp_alrmlim.High.DS_AlrmLim_FiCO2 = ds_readhighlims_FiCO2();
        flag = true;
    }
        
#if (_HL_TYPE == _HL7_ANES)
    if(ds_readhighlims_FiN2O() != tmp_alrmlim.High.DS_AlrmLim_FiN2O)
    {
        tmp_alrmlim.High.DS_AlrmLim_FiN2O = ds_readhighlims_FiN2O();
        flag = true;
    }        
    if(ds_readlowlims_FiN2O() != tmp_alrmlim.Low.DS_AlrmLim_FiN2O)
    {
        tmp_alrmlim.Low.DS_AlrmLim_FiN2O = ds_readlowlims_FiN2O();
        flag = true;
    }
    if(ds_readhighlims_EtAA() != tmp_alrmlim.High.DS_AlrmLim_EtAA)
    {
        tmp_alrmlim.High.DS_AlrmLim_EtAA = ds_readhighlims_EtAA();
        flag = true;
    }
    if(ds_readlowlims_EtAA() != tmp_alrmlim.Low.DS_AlrmLim_EtAA)
    {
        tmp_alrmlim.Low.DS_AlrmLim_EtAA = ds_readlowlims_EtAA();
        flag = true;
    }
    if(ds_readhighlims_FiAA() != tmp_alrmlim.High.DS_AlrmLim_FiAA)
    {
        tmp_alrmlim.High.DS_AlrmLim_FiAA = ds_readhighlims_FiAA();
        flag = true;
    }
    if(ds_readlowlims_FiAA() != tmp_alrmlim.Low.DS_AlrmLim_FiAA)
    {
        tmp_alrmlim.Low.DS_AlrmLim_FiAA = ds_readlowlims_FiAA();
        flag = true;
    }
#else
    if(ds_readhighlims_PEEP() != tmp_alrmlim.High.DS_AlrmLim_PEEP)  //ºôÎü»úµÄbeep
    {
        tmp_alrmlim.High.DS_AlrmLim_PEEP = ds_readhighlims_PEEP();
        flag = true;
    }        
    if(ds_readlowlims_PEEP() != tmp_alrmlim.Low.DS_AlrmLim_PEEP)
    {
        tmp_alrmlim.Low.DS_AlrmLim_PEEP = ds_readlowlims_PEEP();
        flag = true;
    }    
#endif
    if(ds_readhighlims_Pulse() != tmp_alrmlim.High.DS_AlrmLim_Pulse)
    {
        tmp_alrmlim.High.DS_AlrmLim_Pulse = ds_readhighlims_Pulse();
        flag = true;
    }
    if(ds_readlowlims_Pulse() != tmp_alrmlim.Low.DS_AlrmLim_Pulse)
    {
        tmp_alrmlim.Low.DS_AlrmLim_Pulse = ds_readlowlims_Pulse();
        flag = true;
    }
    if(ds_readhighlims_SpO2() != tmp_alrmlim.High.DS_AlrmLim_SpO2)
    {
        tmp_alrmlim.High.DS_AlrmLim_SpO2 = ds_readhighlims_SpO2();
        flag = true;
    }
    if(ds_readlowlims_SpO2() != tmp_alrmlim.Low.DS_AlrmLim_SpO2)
    {
        tmp_alrmlim.Low.DS_AlrmLim_SpO2 = ds_readlowlims_SpO2();
        flag = true;
    }
    if(ds_readhighlims_PI() != tmp_alrmlim.High.DS_AlrmLim_PI)
    {
        tmp_alrmlim.High.DS_AlrmLim_PI = ds_readhighlims_PI();
        flag = true;
    }
    if(ds_readlowlims_PI() != tmp_alrmlim.Low.DS_AlrmLim_PI)
    {
        tmp_alrmlim.Low.DS_AlrmLim_PI = ds_readlowlims_PI();
        flag = true;
    }
    
    return flag;
}

#if (_HL_TYPE == _HL7_ANES)
void HL7_Read_AlrmLim(HL7_AlrmLimtp_t type, HL7_Para_t *para)
{
    switch(type)
    {
    case HL7_AlrmLimtp_PAW_H:
        para->ID = HL7_ALRM_UP;
        para->subID = HL7_PARA_PPEAK;
        para->name = NULL;
        sprintf(para->value, "%d", ds_readhighlims_PAW());
        para->unit = Sys_Lib_GetDispStr(DP_CMH2O);        
        break;
    case HL7_AlrmLimtp_PAW_L:
        para->ID = HL7_ALRM_LOW;
        para->subID = HL7_PARA_PPEAK;
        para->name = NULL;
        sprintf(para->value, "%d", ds_readlowlims_PAW());
        para->unit = Sys_Lib_GetDispStr(DP_CMH2O);         
        break;
    case HL7_AlrmLimtp_MV_H:
        para->ID = HL7_ALRM_UP;
        para->subID = HL7_PARA_MV;
        para->name = NULL;
        sprintf(para->value, "%d", ds_readhighlims_MV());
        para->unit = Sys_Lib_GetDispStr(DP_LL);         
        break;
    case HL7_AlrmLimtp_MV_L:
        para->ID = HL7_ALRM_LOW;
        para->subID = HL7_PARA_MV;
        para->name = NULL;
        sprintf(para->value, "%d", ds_readlowlims_MV());
        para->unit = Sys_Lib_GetDispStr(DP_LL);          
        break;
    case HL7_AlrmLimtp_F_H:
        para->ID = HL7_ALRM_UP;
        para->subID = HL7_PARA_F;
        para->name = NULL;
        sprintf(para->value, "%d", ds_readhighlims_F());
        para->unit = Sys_Lib_GetDispStr(DP_BPM);        
        break;
    case HL7_AlrmLimtp_F_L:
        para->ID = HL7_ALRM_LOW;
        para->subID = HL7_PARA_F;
        para->name = NULL;
        sprintf(para->value, "%d", ds_readlowlims_F());
        para->unit = Sys_Lib_GetDispStr(DP_BPM);        
        break;
    case HL7_AlrmLimtp_FIO2_H:
        para->ID = HL7_ALRM_UP;
        para->subID = HL7_PARA_FIO2;
        para->name = NULL;
        if(ds_readhighlims_FiO2() == 101)
        {
            sprintf(para->value, "%s", Sys_Lib_GetDispStr(DP_OFF));
        }
        else
        {
            sprintf(para->value, "%d", ds_readhighlims_FiO2());
        }
        para->unit = Sys_Lib_GetDispStr(DP_PERCENT);        
        break;
    case HL7_AlrmLimtp_FIO2_L:
        para->ID = HL7_ALRM_LOW;
        para->subID = HL7_PARA_FIO2;
        para->name = NULL;
        sprintf(para->value, "%d", ds_readlowlims_FiO2());
        para->unit = Sys_Lib_GetDispStr(DP_PERCENT);        
        break;
    case HL7_AlrmLimtp_VTE_H:
        para->ID = HL7_ALRM_UP;
        para->subID = HL7_PARA_VTE;
        para->name = NULL;
        sprintf(para->value, "%d", ds_readhighlims_VTe());
        para->unit = Sys_Lib_GetDispStr(DP_ML);         
        break;
    case HL7_AlrmLimtp_VTE_L:
        para->ID = HL7_ALRM_LOW;
        para->subID = HL7_PARA_VTE;
        para->name = NULL;
        sprintf(para->value, "%d", ds_readlowlims_VTe());
        para->unit = Sys_Lib_GetDispStr(DP_ML);        
        break;
    case HL7_AlrmLimtp_ETCO2_H:
        para->ID = HL7_ALRM_UP;
        para->subID = HL7_PARA_ETCO2;
        para->name = NULL;
        switch(ds_readpara_CO2Unit())
        {
        case DS_CO2Unit_per:
        default:
            para->unit = Sys_Lib_GetDispStr(DP_PERCENT); 
            sprintf(para->value, "%.1f", ds_readhighlims_EtCO2()/10.0);     
            break;
        case DS_CO2Unit_mmHg:
            para->unit = Sys_Lib_GetDispStr(DP_MMHG);
            sprintf(para->value, "%d", ds_readhighlims_EtCO2());                 
            break;
        case DS_CO2Unit_Kpa:
            para->unit = Sys_Lib_GetDispStr(DP_KPA);
            sprintf(para->value, "%.1f", ds_readhighlims_EtCO2()/10.0);
            break;          
        }        
        break;
    case HL7_AlrmLimtp_ETCO2_L:
        para->ID = HL7_ALRM_LOW;
        para->subID = HL7_PARA_ETCO2;
        para->name = NULL;
        
        switch(ds_readpara_CO2Unit())
        {
        case DS_CO2Unit_per:
        default:
            para->unit = Sys_Lib_GetDispStr(DP_PERCENT); 
            sprintf(para->value, "%.1f", ds_readlowlims_EtCO2()/10.0);            
            break;
        case DS_CO2Unit_mmHg:
            para->unit = Sys_Lib_GetDispStr(DP_MMHG);
            sprintf(para->value, "%d", ds_readlowlims_EtCO2());
            break;
        case DS_CO2Unit_Kpa:
            para->unit = Sys_Lib_GetDispStr(DP_KPA);
            sprintf(para->value, "%.1f", ds_readlowlims_EtCO2()/10.0);
            break;          
        }        
        break;
    case HL7_AlrmLimtp_FICO2_H:
        para->ID = HL7_ALRM_UP;
        para->subID = HL7_PARA_FICO2;
        para->name = NULL;
        
        switch(ds_readpara_CO2Unit())
        {
        case DS_CO2Unit_per:
        default:
            para->unit = Sys_Lib_GetDispStr(DP_PERCENT);    
            sprintf(para->value, "%.1f", ds_readhighlims_FiCO2()/10.0); 
            break;
        case DS_CO2Unit_mmHg:
            para->unit = Sys_Lib_GetDispStr(DP_MMHG);
            sprintf(para->value, "%d", ds_readhighlims_FiCO2()); 
            break;
        case DS_CO2Unit_Kpa:
            para->unit = Sys_Lib_GetDispStr(DP_KPA);
            sprintf(para->value, "%.1f", ds_readhighlims_FiCO2()/10.0);
            break;          
        }        
        break;
    case HL7_AlrmLimtp_FIN2O_H:
        para->ID = HL7_ALRM_UP;
        para->subID = HL7_PARA_FIN2O;
        para->name = NULL;
        sprintf(para->value, "%d", ds_readhighlims_FiN2O());
        para->unit = Sys_Lib_GetDispStr(DP_PERCENT);        
        break;
    case HL7_AlrmLimtp_FIN2O_L:
        para->ID = HL7_ALRM_LOW;
        para->subID = HL7_PARA_FIN2O;
        para->name = NULL;
        sprintf(para->value, "%d", ds_readlowlims_FiN2O());
        para->unit = Sys_Lib_GetDispStr(DP_PERCENT);        
        break;
    case HL7_AlrmLimtp_ETAA_H:
        para->ID = HL7_ALRM_UP;
        para->subID = HL7_PARA_ETAA;
        para->name = NULL;
        sprintf(para->value, "%.1f", ds_readhighlims_EtAA()/10.0);
        para->unit = Sys_Lib_GetDispStr(DP_PERCENT);        
        break;
    case HL7_AlrmLimtp_ETAA_L:
        para->ID = HL7_ALRM_LOW;
        para->subID = HL7_PARA_ETAA;
        para->name = NULL;
        sprintf(para->value, "%.1f", ds_readlowlims_EtAA()/10.0);
        para->unit = Sys_Lib_GetDispStr(DP_PERCENT);        
        break;
    case HL7_AlrmLimtp_FIAA_H:
        para->ID = HL7_ALRM_UP;
        para->subID = HL7_PARA_FIAA;
        para->name = NULL;
        sprintf(para->value, "%.1f", ds_readhighlims_FiAA()/10.0);
        para->unit = Sys_Lib_GetDispStr(DP_PERCENT);          
        break;
    case HL7_AlrmLimtp_FIAA_L:
        para->ID = HL7_ALRM_LOW;
        para->subID = HL7_PARA_FIAA;
        para->name = NULL;
        sprintf(para->value, "%.1f", ds_readlowlims_FiAA()/10.0);
        para->unit = Sys_Lib_GetDispStr(DP_PERCENT);         
        break;
    case HL7_AlrmLimtp_PULSE_H:
        para->ID = HL7_ALRM_UP;
        para->subID = HL7_PARA_PR;
        para->name = NULL;
        if(ds_readhighlims_Pulse() == 240)
        {
            sprintf(para->value, "%s", Sys_Lib_GetDispStr(DP_OFF));
        }
        else
        {
            sprintf(para->value, "%d", ds_readhighlims_Pulse());
        }
        para->unit = Sys_Lib_GetDispStr(DP_BPM);        
        break;
    case HL7_AlrmLimtp_PULSE_L:
        para->ID = HL7_ALRM_LOW;
        para->subID = HL7_PARA_PR;
        para->name = NULL;
        sprintf(para->value, "%d", ds_readlowlims_Pulse());
        para->unit = Sys_Lib_GetDispStr(DP_BPM);         
        break;
    case HL7_AlrmLimtp_SPO2_H:
        para->ID = HL7_ALRM_UP;
        para->subID = HL7_PARA_SPO2;
        para->name = NULL;
        if(ds_readhighlims_SpO2() == 100)
        {
            sprintf(para->value, "%s", Sys_Lib_GetDispStr(DP_OFF));
        }
        else
        {
            sprintf(para->value, "%d", ds_readhighlims_SpO2());
        }
        para->unit = Sys_Lib_GetDispStr(DP_PERCENT);        
        break;
    case HL7_AlrmLimtp_SPO2_L:
        para->ID = HL7_ALRM_LOW;
        para->subID = HL7_PARA_SPO2;
        para->name = NULL;
        sprintf(para->value, "%d", ds_readlowlims_SpO2());
        para->unit = Sys_Lib_GetDispStr(DP_PERCENT);        
        break;
    case HL7_AlrmLimtp_PI_H:
        para->ID = HL7_ALRM_UP;
        para->subID = HL7_PARA_PI;
        para->name = NULL;
        if(ds_readhighlims_PI() == 2000)
        {
            sprintf(para->value, "%s", Sys_Lib_GetDispStr(DP_OFF));
        }
        else
        {
            sprintf(para->value, "%.1f", ds_readhighlims_PI()/100.0);
        }
        para->unit = NULL;         
        break;
    case HL7_AlrmLimtp_PI_L: 
        para->ID = HL7_ALRM_LOW;
        para->subID = HL7_PARA_PI;
        para->name = NULL;
        sprintf(para->value, "%.1f", ds_readlowlims_PI()/100.0);
        para->unit = NULL;        
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
void HL7_Read_AlrmLim(HL7_AlrmLimtp_t type, HL7_Para_t *para)
{
    switch(type)
    {
    case HL7_AlrmLimtp_PAW_H:
        para->ID = HL7_VENTALRM_UP;
        para->subID = HL7_VENTPARA_PPEAK;
        para->name = NULL;
        sprintf(para->value, "%d", ds_readhighlims_PAW());
        para->unit = (char *)*Global_DS[DISP_UNIT_CMH2O];        
        break;
    case HL7_AlrmLimtp_PAW_L:
        para->ID = HL7_VENTALRM_LOW;
        para->subID = HL7_VENTPARA_PPEAK;
        para->name = NULL;
        sprintf(para->value, "%d", ds_readlowlims_PAW());
        para->unit = (char *)*Global_DS[DISP_UNIT_CMH2O];         
        break;
    case HL7_AlrmLimtp_MV_H:
        para->ID = HL7_VENTALRM_UP;
        para->subID = HL7_VENTPARA_MV;
        para->name = NULL;
        sprintf(para->value, "%d", ds_readhighlims_MV());
        para->unit = (char *)*Global_DS[DISP_UNIT_L];         
        break;
    case HL7_AlrmLimtp_MV_L:
        para->ID = HL7_VENTALRM_LOW;
        para->subID = HL7_VENTPARA_MV;
        para->name = NULL;
        sprintf(para->value, "%d", ds_readlowlims_MV());
        para->unit = (char *)*Global_DS[DISP_UNIT_L];          
        break;
    case HL7_AlrmLimtp_F_H:
        para->ID = HL7_VENTALRM_UP;
        para->subID = HL7_VENTPARA_F;
        para->name = NULL;
        sprintf(para->value, "%d", ds_readhighlims_F());
        para->unit = (char *)*Global_DS[DISP_UNIT_BPM];        
        break;
    case HL7_AlrmLimtp_F_L:
        para->ID = HL7_VENTALRM_LOW;
        para->subID = HL7_VENTPARA_F;
        para->name = NULL;
        sprintf(para->value, "%d", ds_readlowlims_F());
        para->unit = (char *)*Global_DS[DISP_UNIT_BPM];        
        break;
    case HL7_AlrmLimtp_FIO2_H:
        para->ID = HL7_VENTALRM_UP;
        para->subID = HL7_VENTPARA_FIO2;
        para->name = NULL;
        if(ds_readhighlims_FiO2() == 101)
        {
            sprintf(para->value, "%s", *Global_DS[DISP_WORD_OFF]);
        }
        else
        {
            sprintf(para->value, "%d", ds_readhighlims_FiO2());
        }
        para->unit = (char *)*Global_DS[DISP_UNIT_PERCENT];        
        break;
    case HL7_AlrmLimtp_FIO2_L:
        para->ID = HL7_VENTALRM_LOW;
        para->subID = HL7_VENTPARA_FIO2;
        para->name = NULL;
        sprintf(para->value, "%d", ds_readlowlims_FiO2());
        para->unit = (char *)*Global_DS[DISP_UNIT_PERCENT];        
        break;
    case HL7_AlrmLimtp_VTE_H:
        para->ID = HL7_VENTALRM_UP;
        para->subID = HL7_VENTPARA_VTE;
        para->name = NULL;
        sprintf(para->value, "%d", ds_readhighlims_VTe());
        para->unit = (char *)*Global_DS[DISP_UNIT_ML];         
        break;
    case HL7_AlrmLimtp_VTE_L:
        para->ID = HL7_VENTALRM_LOW;
        para->subID = HL7_VENTPARA_VTE;
        para->name = NULL;
        sprintf(para->value, "%d", ds_readlowlims_VTe());
        para->unit = (char *)*Global_DS[DISP_UNIT_ML];        
        break;
    case HL7_AlrmLimtp_PEEP_H:
        para->ID = HL7_VENTALRM_UP;
        para->subID = HL7_VENTPARA_PEEP;
        para->name = NULL;
        sprintf(para->value, "%d", ds_readhighlims_VTe());
        para->unit = (char *)*Global_DS[DISP_UNIT_CMH2O];          
        break;
    case HL7_AlrmLimtp_PEEP_L:
        para->ID = HL7_VENTALRM_LOW;
        para->subID = HL7_VENTPARA_PEEP;
        para->name = NULL;
        sprintf(para->value, "%d", ds_readlowlims_VTe());
        para->unit = (char *)*Global_DS[DISP_UNIT_CMH2O];         
        break;        
    case HL7_AlrmLimtp_ETCO2_H:
        para->ID = HL7_VENTALRM_UP;
        para->subID = HL7_VENTPARA_ETCO2;
        para->name = NULL;
        switch(ds_readpara_CO2Unit())
        {
        case DS_CO2Unit_per:
        default:
            para->unit = (char *)*Global_DS[DISP_UNIT_PERCENT]; 
            sprintf(para->value, "%.1f", ds_readhighlims_EtCO2()/10.0);     
            break;
        case DS_CO2Unit_mmHg:
            para->unit = (char *)*Global_DS[DISP_UNIT_MMHG];
            sprintf(para->value, "%d", ds_readhighlims_EtCO2());                 
            break;
        case DS_CO2Unit_Kpa:
            para->unit = (char *)*Global_DS[DISP_UNIT_KPA];
            sprintf(para->value, "%.1f", ds_readhighlims_EtCO2()/10.0);
            break;          
        }        
        break;
    case HL7_AlrmLimtp_ETCO2_L:
        para->ID = HL7_VENTALRM_LOW;
        para->subID = HL7_VENTPARA_ETCO2;
        para->name = NULL;
        
        switch(ds_readpara_CO2Unit())
        {
        case DS_CO2Unit_per:
        default:
            para->unit = (char *)*Global_DS[DISP_UNIT_PERCENT]; 
            sprintf(para->value, "%.1f", ds_readlowlims_EtCO2()/10.0);            
            break;
        case DS_CO2Unit_mmHg:
            para->unit = (char *)*Global_DS[DISP_UNIT_MMHG];
            sprintf(para->value, "%d", ds_readlowlims_EtCO2());
            break;
        case DS_CO2Unit_Kpa:
            para->unit = (char *)*Global_DS[DISP_UNIT_KPA];
            sprintf(para->value, "%.1f", ds_readlowlims_EtCO2()/10.0);
            break;          
        }        
        break;
    case HL7_AlrmLimtp_FICO2_H:
        para->ID = HL7_VENTALRM_UP;
        para->subID = HL7_VENTPARA_FICO2;
        para->name = NULL;
            
        switch(ds_readpara_CO2Unit())
        {
        case DS_CO2Unit_per:
        default:
            para->unit = (char *)*Global_DS[DISP_UNIT_PERCENT];    
            sprintf(para->value, "%.1f", ds_readhighlims_FiCO2()/10.0); 
            break;
        case DS_CO2Unit_mmHg:
            para->unit = (char *)*Global_DS[DISP_UNIT_MMHG];
            sprintf(para->value, "%d", ds_readhighlims_FiCO2()); 
            break;
        case DS_CO2Unit_Kpa:
            para->unit = (char *)*Global_DS[DISP_UNIT_KPA];
            sprintf(para->value, "%.1f", ds_readhighlims_FiCO2()/10.0);
            break;          
        }        
        break;
    case HL7_AlrmLimtp_PULSE_H:
        para->ID = HL7_VENTALRM_UP;
        para->subID = HL7_VENTPARA_PULSE;
        para->name = NULL;
        if(ds_readhighlims_Pulse() == 240)
        {
            sprintf(para->value, "%s", (char *)*Global_DS[DISP_WORD_OFF]);
        }
        else
        {
            sprintf(para->value, "%d", ds_readhighlims_Pulse());
        }
        para->unit = (char *)*Global_DS[DISP_UNIT_BPM];     
        break;
    case HL7_AlrmLimtp_PULSE_L:
        para->ID = HL7_VENTALRM_LOW;
        para->subID = HL7_VENTPARA_PULSE;
        para->name = NULL;
        sprintf(para->value, "%d", ds_readlowlims_Pulse());
        para->unit = (char *)*Global_DS[DISP_UNIT_BPM];         
        break;
    case HL7_AlrmLimtp_SPO2_H:
        para->ID = HL7_VENTALRM_UP;
        para->subID = HL7_VENTPARA_SPO2;
        para->name = NULL;
        if(ds_readhighlims_SpO2() == 100)
        {
            sprintf(para->value, "%s", (char *)*Global_DS[DISP_WORD_OFF]);
        }
        else
        {
            sprintf(para->value, "%d", ds_readhighlims_SpO2());
        }
        para->unit = (char *)*Global_DS[DISP_UNIT_PERCENT];        
        break;
    case HL7_AlrmLimtp_SPO2_L:
        para->ID = HL7_VENTALRM_LOW;
        para->subID = HL7_VENTPARA_SPO2;
        para->name = NULL;
        sprintf(para->value, "%d", ds_readlowlims_SpO2());
        para->unit = (char *)*Global_DS[DISP_UNIT_PERCENT];        
        break;
    case HL7_AlrmLimtp_PI_H:
        para->ID = HL7_VENTALRM_UP;
        para->subID = HL7_VENTPARA_PI;
        para->name = NULL;
        if(ds_readhighlims_PI() == 2000)
        {
            sprintf(para->value, "%s", (char *)*Global_DS[DISP_WORD_OFF]);
        }
        else
        {
            sprintf(para->value, "%.1f", ds_readhighlims_PI()/100.0);
        }
        para->unit = NULL;         
        break;
    case HL7_AlrmLimtp_PI_L: 
        para->ID = HL7_VENTALRM_LOW;
        para->subID = HL7_VENTPARA_PI;
        para->name = NULL;
        sprintf(para->value, "%.1f", ds_readlowlims_PI()/100.0);
        para->unit = NULL;        
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




