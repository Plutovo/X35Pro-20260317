#include "HL7_OBX_Para.h"
#include "HL7_cfg.h"
#include "HL7_Common.h"
#include "HL7_OBX.h"
#include "HL7_inc_ext.h"
#include "HL7_maptble.h"
#include "uart_dp.h"
#include "DataShare.h"

static char *FiAA_buf[6] POOL_EXRAM; //麻气名称
static char *EtAA_buf[6] POOL_EXRAM; //麻气名称

void HL7_Para_Init(void)
{
#if (_HL_TYPE == _HL7_ANES)    
    FiAA_buf[AA_ID_NONE]  = Sys_Lib_GetDispStr(DP_FIAA1);
    FiAA_buf[AA_ID_HAL]   = Sys_Lib_GetDispStr(DP_FIHAL);
    FiAA_buf[AA_ID_ENF]   = Sys_Lib_GetDispStr(DP_FIENF);
    FiAA_buf[AA_ID_ISO]   = Sys_Lib_GetDispStr(DP_FIISO);
    FiAA_buf[AA_ID_SEV]   = Sys_Lib_GetDispStr(DP_FISEV);
    FiAA_buf[AA_ID_DES]   = Sys_Lib_GetDispStr(DP_FIDES);
    
    FiAA_buf[AA_ID_NONE]  = Sys_Lib_GetDispStr(DP_FIAA1);
    EtAA_buf[AA_ID_HAL]   = Sys_Lib_GetDispStr(DP_ETHAL);
    EtAA_buf[AA_ID_ENF]   = Sys_Lib_GetDispStr(DP_ETENF);
    EtAA_buf[AA_ID_ISO]   = Sys_Lib_GetDispStr(DP_ETISO);
    EtAA_buf[AA_ID_SEV]   = Sys_Lib_GetDispStr(DP_ETSEV);
    EtAA_buf[AA_ID_DES]   = Sys_Lib_GetDispStr(DP_ETDES);       
#endif
}

void HL7_Para_Send(void)
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
    for(int i=HL7_Para_None+1; i<HL7_Para_Last; i++)
    {
        HL7_Read_Para((HL7_Paratp_t)HL7_map_read_paraid(i), &para);            
        if(para.ID != NULL && para.name != NULL)
        {
            OBXx[OBX4_ObsSubID] = (u8*)para.subID;                
            OBXx[OBX5_ObsVal] = (u8*)para.value;
            OBXx[OBX6_Units] = (u8*)para.unit;
            sprintf(tmp_buf, "%s^%s", para.ID, para.name);
            OBXx[OBX3_ObsID] = (u8*)tmp_buf;
            size = HL7_FillBuf(HL7_MSG_OBX_monitor, &send_buf[4], HL7_FRM_MAX_SIZE, OBXx, OBX26_Last);
            size += 4;
            uart_dp_wrsend(send_buf, size);
            HL7_Para_clr(&para);
        }
    }
}

#if (_HL_TYPE == _HL7_ANES)
void HL7_Read_Para(HL7_Paratp_t type, HL7_Para_t *para)
{
    switch(type)
    {
    case HL7_Para_PEEP:
        para->ID = HL7_PARA_PEEP;
        para->subID = HL7_MOD_ANES;
        para->name = Sys_Lib_GetDispStr(DP_PEEP);
        sprintf(para->value, "%.1f", ds_readpara_PEEP()/10.0);
        para->unit = Sys_Lib_GetDispStr(DP_CMH2O);
        break;
    case HL7_Para_Ppeak:
        para->ID = HL7_PARA_PPEAK;
        para->subID = HL7_MOD_ANES;
        para->name = Sys_Lib_GetDispStr(DP_PPEAK);
        sprintf(para->value, "%.1f", ds_readpara_Ppeak()/10.0);
        para->unit = Sys_Lib_GetDispStr(DP_CMH2O);        
        break;
    case HL7_Para_Pplat:
        para->ID = HL7_PARA_PPLAT;
        para->subID = HL7_MOD_ANES;
        para->name = Sys_Lib_GetDispStr(DP_PPLAT);
        sprintf(para->value, "%.1f", ds_readpara_Pplat()/10.0);
        para->unit = Sys_Lib_GetDispStr(DP_CMH2O);          
        break;
    case HL7_Para_Pmean:
        para->ID = HL7_PARA_PMEAN;
        para->subID = HL7_MOD_ANES;
        para->name = Sys_Lib_GetDispStr(DP_PMEAN);
        sprintf(para->value, "%.1f", ds_readpara_Pmean()/10.0);
        para->unit = Sys_Lib_GetDispStr(DP_CMH2O);          
        break;
    case HL7_Para_VTe:
        para->ID = HL7_PARA_VTE;
        para->subID = HL7_MOD_ANES;
        para->name = Sys_Lib_GetDispStr(DP_VTE);
        sprintf(para->value, "%d", ds_readpara_VTe());
        para->unit = Sys_Lib_GetDispStr(DP_ML);         
        break;
    case HL7_Para_VTi:
        para->ID = HL7_PARA_VTI;
        para->subID = HL7_MOD_ANES;
        para->name = Sys_Lib_GetDispStr(DP_VTI);
        sprintf(para->value, "%d", ds_readpara_VTi());
        para->unit = Sys_Lib_GetDispStr(DP_ML);         
        break;
    case HL7_Para_MV:
        para->ID = HL7_PARA_MV;
        para->subID = HL7_MOD_ANES;
        para->name = Sys_Lib_GetDispStr(DP_MV);
        sprintf(para->value, "%.2f", ds_readpara_MV()/100.0);
        para->unit = Sys_Lib_GetDispStr(DP_LL);           
        break;
    case HL7_Para_MVspn:
        para->ID = HL7_PARA_MVSPN;
        para->subID = HL7_MOD_ANES;
        para->name = Sys_Lib_GetDispStr(DP_MVSPN);
        sprintf(para->value, "%.2f", ds_readpara_MVspn()/100.0);
        para->unit = Sys_Lib_GetDispStr(DP_LL);          
        break;
    case HL7_Para_f:
        para->ID = HL7_PARA_F;
        para->subID = HL7_MOD_ANES;
        para->name = Sys_Lib_GetDispStr(DP_F);
        sprintf(para->value, "%d", ds_readpara_f());
        para->unit = Sys_Lib_GetDispStr(DP_BPM);         
        break;
    case HL7_Para_fspn:
        para->ID = HL7_PARA_FSPN;
        para->subID = HL7_MOD_ANES;
        para->name = Sys_Lib_GetDispStr(DP_FSPN);
        sprintf(para->value, "%d", ds_readpara_fspn());
        para->unit = Sys_Lib_GetDispStr(DP_BPM);        
        break;
    case HL7_Para_IEI:
        para->ID = HL7_PARA_IEI;
        para->subID = HL7_MOD_ANES;
        para->name = Sys_Lib_GetDispStr(DP_IEI);
        sprintf(para->value, "%.1f", ds_readpara_IEI()/10.0);
        para->unit = NULL;         
        break;
    case HL7_Para_IEE:
        para->ID = HL7_PARA_IEE;
        para->subID = HL7_MOD_ANES;
        para->name = Sys_Lib_GetDispStr(DP_IEE);
        sprintf(para->value, "%.1f", ds_readpara_IEE()/10.0);
        para->unit = NULL;
        break;
    case HL7_Para_Compl:
        para->ID = HL7_PARA_COMPL;
        para->subID = HL7_MOD_ANES;
        para->name = Sys_Lib_GetDispStr(DP_CDYN);
        sprintf(para->value, "%d", ds_readpara_Compl());
        para->unit = Sys_Lib_GetDispStr(DP_MLCMH2O);         
        break;
    case HL7_Para_RST:
        para->ID = HL7_PARA_RST;
        para->subID = HL7_MOD_ANES;
        para->name = Sys_Lib_GetDispStr(DP_RST);
        sprintf(para->value, "%d", ds_readpara_RST());
        para->unit = Sys_Lib_GetDispStr(DP_CMH2OLS);         
        break;
    case HL7_Para_Pmin:
        para->ID = HL7_PARA_PMIN;
        para->subID = HL7_MOD_ANES;
        para->name = Sys_Lib_GetDispStr(DP_PMIN);
        sprintf(para->value, "%.1f", ds_readpara_Pmin()/10.0);
        para->unit = Sys_Lib_GetDispStr(DP_CMH2O);        
        break;
    case HL7_Para_EtCO2:
        para->ID = HL7_PARA_ETCO2;
        para->subID = HL7_MOD_ANES;
        para->name = Sys_Lib_GetDispStr(DP_ETCO2);
        switch(ds_readpara_CO2Unit())
        {
        case DS_CO2Unit_per:
        default:
            sprintf(para->value, "%.1f", ds_readpara_EtCO2()/10.0);
            para->unit = Sys_Lib_GetDispStr(DP_PERCENT);             
            break;
        case DS_CO2Unit_mmHg:
            sprintf(para->value, "%d", ds_readpara_EtCO2());
            para->unit = Sys_Lib_GetDispStr(DP_MMHG);
            break;
        case DS_CO2Unit_Kpa:
            sprintf(para->value, "%.1f", ds_readpara_EtCO2()/10.0);
            para->unit = Sys_Lib_GetDispStr(DP_KPA);
            break;          
        }
        break;
    case HL7_Para_FiCO2:
        para->ID = HL7_PARA_FICO2;
        para->subID = HL7_MOD_ANES;
        para->name = Sys_Lib_GetDispStr(DP_FICO2);
        switch(ds_readpara_CO2Unit())
        {
        case DS_CO2Unit_per:
        default:
            sprintf(para->value, "%.1f", ds_readpara_FiCO2()/10.0);
            para->unit = Sys_Lib_GetDispStr(DP_PERCENT);             
            break;
        case DS_CO2Unit_mmHg:
            sprintf(para->value, "%d", ds_readpara_FiCO2());
            para->unit = Sys_Lib_GetDispStr(DP_MMHG);
            break;
        case DS_CO2Unit_Kpa:
            sprintf(para->value, "%.1f", ds_readpara_FiCO2()/10.0);
            para->unit = Sys_Lib_GetDispStr(DP_KPA);
            break;
        }        
        break;
    case HL7_Para_FiO2:
        para->ID = HL7_PARA_FIO2;
        para->subID = HL7_MOD_ANES;
        para->name = Sys_Lib_GetDispStr(DP_FIO2);
        sprintf(para->value, "%d", ds_readpara_FiO2());
        para->unit = Sys_Lib_GetDispStr(DP_PERCENT);
        break;
    case HL7_Para_EtO2:
        para->ID = HL7_PARA_ETO2;
        para->subID = HL7_MOD_ANES;
        para->name = Sys_Lib_GetDispStr(DP_ETO2);
        sprintf(para->value, "%d", ds_readpara_EtO2());
        para->unit = Sys_Lib_GetDispStr(DP_PERCENT);        
        break;
    case HL7_Para_FiN2O:
        para->ID = HL7_PARA_FIN2O;
        para->subID = HL7_MOD_ANES;
        para->name = Sys_Lib_GetDispStr(DP_FIN2O);
        sprintf(para->value, "%d", ds_readpara_FiN2O());
        para->unit = Sys_Lib_GetDispStr(DP_PERCENT);         
        break;
    case HL7_Para_EtN2O:
        para->ID = HL7_PARA_ETN2O;
        para->subID = HL7_MOD_ANES;
        para->name = Sys_Lib_GetDispStr(DP_ETN2O);
        sprintf(para->value, "%d", ds_readpara_EtN2O());
        para->unit = Sys_Lib_GetDispStr(DP_PERCENT);          
        break;
    case HL7_Para_FiAA:
        para->ID = HL7_PARA_FIAA;
        para->subID = HL7_MOD_ANES;
        para->name = FiAA_buf[ds_readpara_AA1ID()];
        sprintf(para->value, "%.1f", ds_readpara_FiAA()/10.0);
        para->unit = Sys_Lib_GetDispStr(DP_PERCENT);         
        break;
    case HL7_Para_EtAA:
        para->ID = HL7_PARA_ETAA;
        para->subID = HL7_MOD_ANES;
        para->name = EtAA_buf[ds_readpara_AA1ID()];
        sprintf(para->value, "%.1f", ds_readpara_EtAA()/10.0);
        para->unit = Sys_Lib_GetDispStr(DP_PERCENT);         
        break;
    case HL7_Para_FiAA2:
        para->ID = HL7_PARA_FIAA2;
        para->subID = HL7_MOD_ANES;
        para->name = FiAA_buf[ds_readpara_AA2ID()];
        sprintf(para->value, "%.1f", ds_readpara_FiAA2()/10.0);
        para->unit = Sys_Lib_GetDispStr(DP_PERCENT);         
        break;
    case HL7_Para_EtAA2:
        para->ID = HL7_PARA_ETAA2;
        para->subID = HL7_MOD_ANES;
        para->name = EtAA_buf[ds_readpara_AA2ID()];
        sprintf(para->value, "%.1f", ds_readpara_EtAA2()/10.0);
        para->unit = Sys_Lib_GetDispStr(DP_PERCENT);         
        break;
    case HL7_Para_MAC:
        para->ID = HL7_PARA_MAC;
        para->subID = HL7_MOD_ANES;
        para->name = Sys_Lib_GetDispStr(DP_MAC);
        sprintf(para->value, "%.2f", ds_readpara_MAC()/100.0);
        para->unit = NULL;          
        break;
    case HL7_Para_Flow_N2O:
        para->ID = HL7_PARA_FLOWN2O;
        para->subID = HL7_MOD_ANES;
        para->name = Sys_Lib_GetDispStr(DP_N2O);
        sprintf(para->value, "%d", ds_readpara_Flow_N2O());
        para->unit = Sys_Lib_GetDispStr(DP_LMIN);        
        break;
    case HL7_Para_Flow_Air:
        para->ID = HL7_PARA_FLOWAIR;
        para->subID = HL7_MOD_ANES;
        para->name = Sys_Lib_GetDispStr(DP_AIR);
        sprintf(para->value, "%d", ds_readpara_Flow_Air());
        para->unit = Sys_Lib_GetDispStr(DP_LMIN);        
        break;
    case HL7_Para_Flow_O2:
        para->ID = HL7_PARA_FLOWO2;
        para->subID = HL7_MOD_ANES;
        para->name = Sys_Lib_GetDispStr(DP_O2);
        sprintf(para->value, "%d", ds_readpara_Flow_O2());
        para->unit = Sys_Lib_GetDispStr(DP_LMIN);          
        break;
    case HL7_Para_SpO2:
        para->ID = HL7_PARA_SPO2;
        para->subID = HL7_MOD_ANES;
        para->name = Sys_Lib_GetDispStr(DP_SPO2);
        sprintf(para->value, "%d", ds_readpara_SpO2());
        para->unit = Sys_Lib_GetDispStr(DP_PERCENT);        
        break;
    case HL7_Para_PR:
        para->ID = HL7_PARA_PR;
        para->subID = HL7_MOD_ANES;
        para->name = Sys_Lib_GetDispStr(DP_PULSE);
        sprintf(para->value, "%d", ds_readpara_PR());
        para->unit = Sys_Lib_GetDispStr(DP_BPM);          
        break;
    case HL7_Para_PI: 
        para->ID = HL7_PARA_PI;
        para->subID = HL7_MOD_ANES;
        para->name = Sys_Lib_GetDispStr(DP_PI);
        if((ds_readpara_PI() >= 20)&&(ds_readpara_PI() < 995))
        {
            sprintf(para->value, "%.2f", Data_Rounding(ds_readpara_PI(), 1000, 2));
        }
        else if((ds_readpara_PI() >= 995)&&(ds_readpara_PI() <  9950))
        {
            sprintf(para->value, "%.1f", Data_Rounding(ds_readpara_PI(), 1000, 1));
        }
        else if((ds_readpara_PI() >= 9950)&&(ds_readpara_PI() <= 20000))
        {
            sprintf(para->value, "%d", (u32)Data_Rounding(ds_readpara_PI(), 1000, 0));
        }
        else    //zkq 20190731
        {
            sprintf(para->value, "%d", 0);
        }
        
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
void HL7_Read_Para(HL7_Paratp_t type, HL7_Para_t *para)
{
    switch(type)
    {
    case HL7_Para_PEEP:
        para->ID = HL7_VENTPARA_PEEP;
        para->subID = HL7_MOD_VENT;
        para->name = (char *)*Global_DS[DISP_PEEP];
        sprintf(para->value, "%.1f", ds_readpara_PEEP()/10.0);
        para->unit = (char *)*Global_DS[DISP_UNIT_CMH2O];
        break;
    case HL7_Para_Ppeak:
        para->ID = HL7_VENTPARA_PPEAK;
        para->subID = HL7_MOD_VENT;
        para->name = (char *)*Global_DS[DISP_PPEAK];
        sprintf(para->value, "%.1f", ds_readpara_Ppeak()/10.0);
        para->unit = (char *)*Global_DS[DISP_UNIT_CMH2O];
        break;
    case HL7_Para_Pplat:
        para->ID = HL7_VENTPARA_PPLAT;
        para->subID = HL7_MOD_VENT;
        para->name = (char *)*Global_DS[DISP_PPLAT];
        sprintf(para->value, "%.1f", ds_readpara_Pplat()/10.0);
        para->unit = (char *)*Global_DS[DISP_UNIT_CMH2O];          
        break;
    case HL7_Para_Pmean:
        para->ID = HL7_VENTPARA_PMEAN;
        para->subID = HL7_MOD_VENT;
        para->name = (char *)*Global_DS[DISP_PMEAN];
        sprintf(para->value, "%.1f", ds_readpara_Pmean()/10.0);
        para->unit = (char *)*Global_DS[DISP_UNIT_CMH2O];          
        break;
    case HL7_Para_VTe:
        para->ID = HL7_VENTPARA_VTE;
        para->subID = HL7_MOD_VENT;
        para->name = (char *)*Global_DS[DISP_VTE];
        sprintf(para->value, "%d", ds_readpara_VTe());
        para->unit = (char *)*Global_DS[DISP_UNIT_ML];         
        break;
    case HL7_Para_VTi:
        para->ID = HL7_VENTPARA_VTI;
        para->subID = HL7_MOD_VENT;
        para->name = (char *)*Global_DS[DISP_VTI];
        sprintf(para->value, "%d", ds_readpara_VTi());
        para->unit = (char *)*Global_DS[DISP_UNIT_ML];         
        break;
    case HL7_Para_MV:
        para->ID = HL7_VENTPARA_MV;
        para->subID = HL7_MOD_VENT;
        para->name = (char *)*Global_DS[DISP_MV];
        sprintf(para->value, "%.2f", ds_readpara_MV()/100.0);
        para->unit = (char *)*Global_DS[DISP_UNIT_L_MIN];           
        break;
    case HL7_Para_MVspn:
        para->ID = HL7_VENTPARA_MVSPN;
        para->subID = HL7_MOD_VENT;
        para->name = (char *)*Global_DS[DISP_MVSPN];
        sprintf(para->value, "%.2f", ds_readpara_MVspn()/100.0);
        para->unit = (char *)*Global_DS[DISP_UNIT_L_MIN];
        break;
    case HL7_Para_f:
        para->ID = HL7_VENTPARA_F;
        para->subID = HL7_MOD_VENT;
        para->name = (char *)*Global_DS[DISP_FTOTAL];
        sprintf(para->value, "%d", ds_readpara_f());
        para->unit = (char *)*Global_DS[DISP_UNIT_BPM];         
        break;
    case HL7_Para_fspn:
        para->ID = HL7_VENTPARA_FSPN;
        para->subID = HL7_MOD_VENT;
        para->name = (char *)*Global_DS[DISP_FSPN];
        sprintf(para->value, "%d", ds_readpara_fspn());
        para->unit = (char *)*Global_DS[DISP_UNIT_BPM];        
        break;
    case HL7_Para_IEI:
        para->ID = HL7_VENTPARA_IEI;
        para->subID = HL7_MOD_VENT;
        para->name = "I";
        sprintf(para->value, "%.1f", ds_readpara_IEI()/10.0);
        para->unit = NULL;         
        break;
    case HL7_Para_IEE:
        para->ID = HL7_VENTPARA_IEE;
        para->subID = HL7_MOD_VENT;
        para->name = "E";
        sprintf(para->value, "%.1f", ds_readpara_IEE()/10.0);
        para->unit = NULL;
        break;
    case HL7_Para_Compl:
        para->ID = HL7_VENTPARA_CDYN;
        para->subID = HL7_MOD_VENT;
        para->name = (char *)*Global_DS[DISP_CDYN];
        sprintf(para->value, "%d", ds_readpara_Compl());
        para->unit = (char *)*Global_DS[DISP_UNIT_CMH2O];         
        break;
    case HL7_Para_RST:
        para->ID = HL7_VENTPARA_RST;
        para->subID = HL7_MOD_VENT;
        para->name = (char *)*Global_DS[DISP_RST];
        sprintf(para->value, "%d", ds_readpara_RST());
        para->unit = (char *)*Global_DS[DISP_UNIT_CMH2O_L_S];         
        break;
    case HL7_Para_Pmin:
        para->ID = HL7_VENTPARA_PMIN;
        para->subID = HL7_MOD_VENT;
        para->name = (char *)*Global_DS[DISP_PMIN];
        sprintf(para->value, "%.1f", ds_readpara_Pmin()/10.0);
        para->unit = (char *)*Global_DS[DISP_UNIT_CMH2O];        
        break;
    case HL7_Para_EtCO2:
        para->ID = HL7_VENTPARA_ETCO2;
        para->subID = HL7_MOD_VENT;
        para->name = (char *)*Global_DS[DISP_ETCO2];
        switch(ds_readpara_CO2Unit())
        {
        case DS_CO2Unit_per:
        default:
            sprintf(para->value, "%.1f", ds_readpara_EtCO2()/10.0);
            para->unit = (char *)*Global_DS[DISP_UNIT_PERCENT];             
            break;
        case DS_CO2Unit_mmHg:
            sprintf(para->value, "%d", ds_readpara_EtCO2());
            para->unit = (char *)*Global_DS[DISP_UNIT_MMHG];
            break;
        case DS_CO2Unit_Kpa:
            sprintf(para->value, "%.1f", ds_readpara_EtCO2()/10.0);
            para->unit = (char *)*Global_DS[DISP_UNIT_KPA];
            break;          
        }
        break;
    case HL7_Para_FiCO2:
        para->ID = HL7_VENTPARA_FICO2;
        para->subID = HL7_MOD_VENT;
        para->name = (char *)*Global_DS[DISP_FICO2];
        switch(ds_readpara_CO2Unit())
        {
        case DS_CO2Unit_per:
        default:
            sprintf(para->value, "%.1f", ds_readpara_FiCO2()/10.0);
            para->unit = (char *)*Global_DS[DISP_UNIT_PERCENT];             
            break;
        case DS_CO2Unit_mmHg:
            sprintf(para->value, "%d", ds_readpara_FiCO2());
            para->unit = (char *)*Global_DS[DISP_UNIT_MMHG];
            break;
        case DS_CO2Unit_Kpa:
            sprintf(para->value, "%.1f", ds_readpara_FiCO2()/10.0);
            para->unit = (char *)*Global_DS[DISP_UNIT_KPA];
            break;
        }        
        break;
    case HL7_Para_FiO2:
        para->ID = HL7_VENTPARA_FIO2;
        para->subID = HL7_MOD_VENT;
        para->name = (char *)*Global_DS[DISP_FIO2];
        sprintf(para->value, "%d", ds_readpara_FiO2());
        para->unit = (char *)*Global_DS[DISP_UNIT_PERCENT];
        break;
    case HL7_Para_SpO2:
        para->ID = HL7_VENTPARA_SPO2;
        para->subID = HL7_MOD_VENT;
        para->name = (char *)*Global_DS[DISP_SPO2];
        sprintf(para->value, "%d", ds_readpara_SpO2());
        para->unit = (char *)*Global_DS[DISP_UNIT_PERCENT];        
        break;
    case HL7_Para_PR:
        para->ID = HL7_VENTPARA_PULSE;
        para->subID = HL7_MOD_VENT;
        para->name = (char *)*Global_DS[DISP_PULSE];
        sprintf(para->value, "%d", ds_readpara_PR());
        para->unit = (char *)*Global_DS[DISP_UNIT_BPM];          
        break;
    case HL7_Para_PI: 
        para->ID = HL7_VENTPARA_PI;
        para->subID = HL7_MOD_VENT;
        para->name = (char *)*Global_DS[DISP_PI];
        if((ds_readpara_PI() >= 20)&&(ds_readpara_PI() < 995))
        {
            sprintf(para->value, "%.2f", Data_Rounding(ds_readpara_PI(), 1000, 2));
        }
        else if((ds_readpara_PI() >= 995)&&(ds_readpara_PI() <  9950))
        {
            sprintf(para->value, "%.1f", Data_Rounding(ds_readpara_PI(), 1000, 1));
        }
        else if((ds_readpara_PI() >= 9950)&&(ds_readpara_PI() <= 20000))
        {
            sprintf(para->value, "%d", (u32)Data_Rounding(ds_readpara_PI(), 1000, 0));
        }
        else    //zkq 20190731
        {
            sprintf(para->value, "%d", 0);
        }
        
        para->unit = NULL;
        break;
    case HL7_Para_RSBI:
        para->ID = HL7_VENTPARA_RSBI;
        para->subID = HL7_MOD_VENT;
        para->name = (char *)*Global_DS[DISP_RSBI];
        sprintf(para->value, "%d", ds_readpara_RSBI());
        para->unit = (char *)*Global_DS[DISP_UNIT_BPM_L];
        break;
    case HL7_Para_PEEPI:
        para->ID = HL7_VENTPARA_PEEPI;
        para->subID = HL7_MOD_VENT;
        para->name = (char *)*Global_DS[DISP_PEEPI];
        sprintf(para->value, "%d", ds_readpara_PEEPI());
        para->unit = (char *)*Global_DS[DISP_UNIT_CMH2O];
        break;
    case HL7_Para_CST:
        para->ID = HL7_VENTPARA_CST;
        para->subID = HL7_MOD_VENT;
        para->name = (char *)*Global_DS[DISP_CST];
        sprintf(para->value, "%d", ds_readpara_CST());
        para->unit = (char *)*Global_DS[DISP_UNIT_MLCMH2O];        
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


