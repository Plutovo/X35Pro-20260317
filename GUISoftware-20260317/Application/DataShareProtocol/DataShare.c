#include "DataShare_cfg.h"
#include "DataShare.h"
#include "DataShare_user.h"
#include "string.h"

static DS_Para_t ds_para POOL_EXRAM;
static DS_CtrlPara_t ds_ctrlpara POOL_EXRAM;
static DS_AlrmLims_t ds_alrmlimits POOL_EXRAM;
static DS_Patient_t  ds_patient POOL_EXRAM;
static DS_IE_t Radio_IE      = {0};

static u32 prio_hightbl[3] = {0};
static u32 prio_lowtbl[3] = {0};

//类型
static DS_GasMod_t ds_gasmod;
static DS_SpO2Mod_t ds_spo2mod;
static DS_MachMod_t ds_machmod;
static char **ds_alarm_pstrs0 = NULL;
static char **ds_alarm_pstrs1 = NULL;


void ds_init(void)
{
    memset(&ds_para,        0, sizeof(DS_Para_t));
    memset(&ds_ctrlpara,    0, sizeof(DS_CtrlPara_t));
    memset(&ds_alrmlimits,  0, sizeof(DS_AlrmLims_t));
    memset(&ds_patient,     0, sizeof(DS_Patient_t));
    
    ds_gasmod = DS_GasMod_None;
    ds_spo2mod = DS_SpO2Mod_None;
    ds_machmod = DS_MachMod_Anes;

    prio_hightbl[0] =  (DS_ALARMS_0)&0xFFFFFFFF;
    prio_hightbl[1] =  (DS_ALARMS_0>>32)&0xFFFFFFFF;
    prio_hightbl[2] = 0x01; //保留优先级 不能删除
#ifdef ALARMS1    
    prio_lowtbl[0] = (DS_ALARMS_1)&0xFFFFFFFF;
    prio_lowtbl[1] = (DS_ALARMS_1>>32)&0xFFFFFFFF;
    prio_lowtbl[2] = 0x01; //保留优先级 不能删除
#endif
    ds_alarm_pstrs0 = DS_ALARMS_STRS_0;
    ds_alarm_pstrs1 = DS_ALARMS_STRS_1;
}

void ds_getdatashare(void)
{
    ds_calieratio();        //计算IE
    ds_getpara();           //获得监测参数
    ds_getctrlpara();       //获得控制参数
    ds_getalrmlimits();     //获得报警限制
    ds_getmodel();          //获得当前模块
    ds_getalrmmsg();        //获得报警
    ds_getpatient();        //获取病人数据
}

void ds_getpara(void)
{
    //生理参数
    ds_para.DS_Para_PEEP = DS_PARA_PEEP;
    ds_para.DS_Para_Ppeak = DS_PARA_PPEAK;
    ds_para.DS_Para_Pplat = DS_PARA_PPLAT;
    ds_para.DS_Para_Pmean = DS_PARA_PMEAN;
    ds_para.DS_Para_VTe = DS_PARA_VTE;
    ds_para.DS_Para_VTi = DS_PARA_VTI;
    ds_para.DS_Para_MV = DS_PARA_MV;
    ds_para.DS_Para_MVspn = DS_PARA_MVSPN;
    ds_para.DS_Para_f = DS_PARA_F;
    ds_para.DS_Para_fspn = DS_PARA_FSPN;
    ds_para.DS_Para_IEI = DS_PARA_IEI;
    ds_para.DS_Para_IEE = DS_PARA_IEE;
    ds_para.DS_Para_Compl = DS_PARA_COMPL;
    ds_para.DS_Para_RST = DS_PARA_RST;
    ds_para.DS_Para_Pmin = DS_PARA_PMIN;
    ds_para.DS_Para_RSBI = DS_PARA_RSBI;
    ds_para.DS_Para_CST = DS_PARA_CST;
    ds_para.DS_Para_PEEPI = DS_PARA_PEEPI;
    
    ds_para.DS_Para_FiO2 = DS_PARA_FIO2;
    ds_para.DS_Para_EtO2 = DS_PARA_ETO2;
    ds_para.DS_Para_EtCO2 = (u16)DS_PARA_ETCO2;
    ds_para.DS_Para_FiCO2 = (u16)DS_PARA_FICO2;
#ifdef AAMODULE    
    ds_para.DS_Para_FiN2O = DS_PARA_FIN2O;
    ds_para.DS_Para_EtN2O = DS_PARA_ETN2O;
    ds_para.DS_Para_FiAA = DS_PARA_FIAA;
    ds_para.DS_Para_EtAA = DS_PARA_ETAA;
    ds_para.DS_Para_FiAA2 = DS_PARA_FIAA2;
    ds_para.DS_Para_EtAA2 = DS_PARA_ETAA2;
    ds_para.DS_Para_MAC = DS_PARA_MAC;
#endif
#ifdef EGMMODULE
    //应急供养开关关闭
    if(!DS_PARA_GASSWITCH)
    {
        ds_para.DS_Para_Flow_N2O = DS_PARA_FLOW_N2O*10;
        ds_para.DS_Para_Flow_Air = DS_PARA_FLOW_AIR*10;
        ds_para.DS_Para_Flow_O2 = DS_PARA_FLOW_O2*10;
    }
    else
    {
        ds_para.DS_Para_Flow_N2O = 0;
        ds_para.DS_Para_Flow_Air = 0;
        ds_para.DS_Para_Flow_O2 = 0;        
    }
    ds_para.DS_Para_FlowChannel = DS_PARA_FLOWCHANNEL;     
#endif
#ifdef SPO2MODULE
    ds_para.DS_Para_SpO2 = SpO2_Parameters.SpO2;
    ds_para.DS_Para_PR = SpO2_Parameters.PulseRate;
    ds_para.DS_Para_PI = SpO2_Parameters.PerfusionIndex;   
#endif
    ds_para.DS_Para_StdyState = DS_PARA_STDYSTATE;
}

void ds_getctrlpara(void)
{
    ds_ctrlpara.DS_CtrlPara_PEEP = DS_CTRLPARA_PEEP;
    ds_ctrlpara.DS_CtrlPara_VT = DS_CTRLPARA_VT;
    ds_ctrlpara.DS_CtrlPara_f = DS_CTRLPARA_F;
    ds_ctrlpara.DS_CtrlPara_IE = DS_CTRLPARA_IE;
    ds_ctrlpara.DS_CtrlPara_Tslope = DS_CTRLPARA_TSLOPE;
    ds_ctrlpara.DS_CtrlPara_Tinsp = DS_CTRLPARA_TINSP;
    ds_ctrlpara.DS_CtrlPara_Pinsp = DS_CTRLPARA_PINSP;
    ds_ctrlpara.DS_CtrlPara_Psupp = DS_CTRLPARA_PSUPP;
    ds_ctrlpara.DS_CtrlPara_Tpause = DS_CTRLPARA_TPAUSE;
    ds_ctrlpara.DS_CtrlPara_Esens = DS_CTRLPARA_ESENS;
    ds_ctrlpara.DS_CtrlPara_Psens = DS_CTRLPARA_PSENS;
    ds_ctrlpara.DS_CtrlPara_Fsens = DS_CTRLPARA_FSENS;
    ds_ctrlpara.DS_CtrlPara_Sigh = DS_CTRLPARA_SIGH;
    
    ds_ctrlpara.DS_CtrlPara_Thigh = DS_CTRLPARA_THIGH;
    ds_ctrlpara.DS_CtrlPara_Tlow = DS_CTRLPARA_TLOW;
    ds_ctrlpara.DS_CtrlPara_Phigh = DS_CTRLPARA_PHIGH;
    ds_ctrlpara.DS_CtrlPara_Plow = DS_CTRLPARA_PLOW;
#ifdef EGMMODULE
    ds_ctrlpara.DS_CtrlPara_FG = DS_CTRLPARA_FG;
    if(DS_READ_GAS == GAS_Channel_O2)
    {
        ds_ctrlpara.DS_CtrlPara_PerO2 = DS_CTRLPARA_100O2;  
    }
    else
    {
        ds_ctrlpara.DS_CtrlPara_PerO2 = DS_CTRLPARA_PERO2;  
    }
#endif
    ds_ctrlpara.DS_CtrlPara_Mode = DS_CTRLPARA_MODE;
}

void ds_getalrmlimits(void)
{
    ds_alrmlimits.High.DS_AlrmLim_PAW = DS_ALRMLIMHIGH_PAW;
    ds_alrmlimits.High.DS_AlrmLim_MV = DS_ALRMLIMHIGH_MV;
    ds_alrmlimits.High.DS_AlrmLim_F = DS_ALRMLIMHIGH_F;
    ds_alrmlimits.High.DS_AlrmLim_FiO2 = DS_ALRMLIMHIGH_FIO2;
    ds_alrmlimits.High.DS_AlrmLim_VTe = DS_ALRMLIMHIGH_VTE;
    ds_alrmlimits.High.DS_AlrmLim_PEEP = DS_ALRMLIMHIGH_PEEP;
    ds_alrmlimits.High.DS_AlrmLim_EtCO2 = DS_ALRMLIMHIGH_ETCO2;
    ds_alrmlimits.High.DS_AlrmLim_FiCO2 = DS_ALRMLIMHIGH_FICO2;
    
#ifdef AAMODULE     
    ds_alrmlimits.High.DS_AlrmLim_FiN2O = DS_ALRMLIMHIGH_FIN2O;
    ds_alrmlimits.High.DS_AlrmLim_EtAA = DS_ALRMLIMHIGH_ETAA;
    ds_alrmlimits.High.DS_AlrmLim_FiAA = DS_ALRMLIMHIGH_FIAA;
#endif
    
#ifdef SPO2MODULE
    ds_alrmlimits.High.DS_AlrmLim_Pulse = DS_ALRMLIMHIGH_PULSE;
    ds_alrmlimits.High.DS_AlrmLim_SpO2 = DS_ALRMLIMHIGH_SPO2;
    ds_alrmlimits.High.DS_AlrmLim_PI = DS_ALRMLIMHIGH_PI;
#endif
    
    ds_alrmlimits.Low.DS_AlrmLim_PAW = DS_ALRMLIMLOW_PAW;
    ds_alrmlimits.Low.DS_AlrmLim_MV = DS_ALRMLIMLOW_MV;
    ds_alrmlimits.Low.DS_AlrmLim_F = DS_ALRMLIMLOW_F;
    ds_alrmlimits.Low.DS_AlrmLim_FiO2 = DS_ALRMLIMLOW_FIO2;
    ds_alrmlimits.Low.DS_AlrmLim_VTe = DS_ALRMLIMLOW_VTE;
    ds_alrmlimits.Low.DS_AlrmLim_PEEP = DS_ALRMLIMLOW_PEEP;
    ds_alrmlimits.Low.DS_AlrmLim_EtCO2 = DS_ALRMLIMLOW_ETCO2;
    ds_alrmlimits.Low.DS_AlrmLim_FiCO2 = DS_ALRMLIMLOW_FICO2;
    
#ifdef AAMODULE     
    ds_alrmlimits.Low.DS_AlrmLim_FiN2O = DS_ALRMLIMLOW_FIN2O;
    ds_alrmlimits.Low.DS_AlrmLim_EtAA = DS_ALRMLIMLOW_ETAA;
    ds_alrmlimits.Low.DS_AlrmLim_FiAA = DS_ALRMLIMLOW_FIAA;
#endif
    
#ifdef SPO2MODULE    
    ds_alrmlimits.Low.DS_AlrmLim_Pulse = DS_ALRMLIMLOW_PULSE;
    ds_alrmlimits.Low.DS_AlrmLim_SpO2 = DS_ALRMLIMLOW_SPO2;
    ds_alrmlimits.Low.DS_AlrmLim_PI = DS_ALRMLIMLOW_PI;
#endif
}

void ds_getalrmmsg(void)
{
    prio_hightbl[0] =  (DS_ALARMS_0)&0xFFFFFFFF;
    prio_hightbl[1] =  (DS_ALARMS_0>>32)&0xFFFFFFFF;
#ifdef ALARMS1     
    prio_lowtbl[0] = (DS_ALARMS_1)&0xFFFFFFFF;
    prio_lowtbl[1] = (DS_ALARMS_1>>32)&0xFFFFFFFF;
#endif
}

void ds_getpatient(void)
{
#ifdef PATIENT
    ds_patient.DS_Patient_ID = DS_PATIENT_ID;
    ds_patient.DS_Patient_Name = DS_PATIENT_NAME;
    ds_patient.DS_Patient_Gender = DS_PATIENT_GENDER;
#endif
}

void ds_getmodel(void)
{
}

bool ds_alrmisexist(void)
{
    if(prio_hightbl[0] != 0 || prio_hightbl[1] != 0 
#ifdef ALARMS1        
       || prio_lowtbl[0] != 0 || prio_lowtbl[1] != 0
#endif
      )
    {
        return true;
    }
    return false;
}

bool ds_readmsg_alrmhigh(DS_AlrmMsg_t *alrm)
{
    u32 prio = OS_PrioGetHighest_Trail(prio_hightbl);
    if(prio >= 64)
    {
        return false;
    }
    else
    {
        alrm->ID = prio;
#ifdef AAMODULE
        if((((u64)1)<<prio) == DS_ALARMS_HIGH_ETAA ||(((u64)1)<<prio) == DS_ALARMS_HIGH_FIAA)
        {
            alrm->Msg = (u8*)DS_ALARMS_AA_FIND(prio, DS_PARA_AA1ID);
        }
        else
#endif
        {
            alrm->Msg = (u8 *)ds_alarm_pstrs0[prio];
        }
        OS_PrioRemove_Trail(prio_hightbl, prio);
        
        return true;
    }
}

#ifdef ALARMS1 
bool ds_readmsg_alrmlow(DS_AlrmMsg_t *alrm)
{
    u32 prio = OS_PrioGetHighest_Trail(prio_lowtbl);
    if(prio >= 64)
    {
        return false;
    }
    else
    {
        alrm->ID = prio;
#ifdef AAMODULE        
        if((((u64)1)<<prio) == DS_ALARMS_LOW_ETAA || (((u64)1)<<prio) == DS_ALARMS_LOW_FIAA)
        {
            alrm->Msg = (u8*)DS_ALARMS_AA_FIND(prio, DS_PARA_AA2ID);
        }
        else
#endif            
        {
            alrm->Msg = (u8 *)ds_alarm_pstrs1[prio];
        }
        OS_PrioRemove_Trail(prio_lowtbl, prio);
        
        return true;
    }
}
#endif

//para
s16 ds_readpara_PEEP(void)
{
    return ds_para.DS_Para_PEEP;
}

s16 ds_readpara_Pplat(void)
{
    return ds_para.DS_Para_Pplat;
}

s16 ds_readpara_Ppeak(void)
{
    return ds_para.DS_Para_Ppeak;
}


u16 ds_readpara_Pmean(void)
{
    return ds_para.DS_Para_Pmean;
}

u16 ds_readpara_VTe(void)
{
    return ds_para.DS_Para_VTe;
}

u16 ds_readpara_VTi(void)
{
    return ds_para.DS_Para_VTi;
}

u16 ds_readpara_MV(void)
{
    return ds_para.DS_Para_MV;
}

u16 ds_readpara_MVspn(void)
{
    return ds_para.DS_Para_MVspn;
}

u16 ds_readpara_f(void)
{
    return ds_para.DS_Para_f;
}

u16 ds_readpara_fspn(void)
{
    return ds_para.DS_Para_fspn;
}

u16 ds_readpara_IEI(void)
{
    return ds_para.DS_Para_IEI;
}

u16 ds_readpara_IEE(void)
{
    return ds_para.DS_Para_IEE;
}

u16 ds_readpara_Compl(void)
{
    return ds_para.DS_Para_Compl;
}

u16 ds_readpara_RST(void)
{
    return ds_para.DS_Para_RST;
}

u16 ds_readpara_Pmin(void)
{
    return ds_para.DS_Para_Pmin;
}

u16 ds_readpara_RSBI(void)
{
    return ds_para.DS_Para_RSBI;
}

u16 ds_readpara_CST(void)
{
    return ds_para.DS_Para_CST;
}

u16 ds_readpara_PEEPI(void)
{
    return ds_para.DS_Para_PEEPI;
}


u16 ds_readpara_EtCO2(void)
{
    return ds_para.DS_Para_EtCO2;
}

u16 ds_readpara_FiCO2(void)
{
    return ds_para.DS_Para_FiCO2;
}


u16 ds_readpara_FiO2(void)
{
    return ds_para.DS_Para_FiO2;
}

u16 ds_readpara_EtO2(void)
{
    return ds_para.DS_Para_EtO2;
}

u16 ds_readpara_FiN2O(void)
{
    return ds_para.DS_Para_FiN2O;
}

u16 ds_readpara_EtN2O(void)
{
    return ds_para.DS_Para_EtN2O;
}

u16 ds_readpara_FiAA(void)
{
    return ds_para.DS_Para_FiAA;
}

u16 ds_readpara_EtAA(void)
{
    return ds_para.DS_Para_EtAA;
}

u16 ds_readpara_FiAA2(void)
{
    return ds_para.DS_Para_FiAA2;
}

u16 ds_readpara_EtAA2(void)
{
    return ds_para.DS_Para_EtAA2;
}

u16 ds_readpara_MAC(void)
{
    return ds_para.DS_Para_MAC;
}

u16 ds_readpara_Flow_N2O(void)
{
    return ds_para.DS_Para_Flow_N2O;
}

u16 ds_readpara_Flow_Air(void)
{
    return ds_para.DS_Para_Flow_Air;
}

u16 ds_readpara_Flow_O2(void)
{
    return ds_para.DS_Para_Flow_O2;
}

u16 ds_readpara_SpO2(void)
{
    return ds_para.DS_Para_SpO2;
}

u16 ds_readpara_PR(void)
{
    return ds_para.DS_Para_PR;
}

u16 ds_readpara_PI(void)
{
    return ds_para.DS_Para_PI;
}

DS_CO2Unit_t ds_readpara_CO2Unit(void)
{
    return (DS_CO2Unit_t)ETCO2Unit;
}

enum AA_GAS_ID ds_readpara_AA1ID(void)
{
#ifdef AAMODULE
    return AA_all_data.AA_detail.AA_AA1.ID;
#else
    return 0;
#endif
}

enum AA_GAS_ID ds_readpara_AA2ID(void)
{
#ifdef AAMODULE    
    return AA_all_data.AA_detail.AA_AA2.ID;
#else
    return 0;
#endif
}

//GAS_Channel_t ds_readpara_flowchannel(void)
//{
//    return ds_para.DS_Para_FlowChannel;
//}

u32 ds_readpara_StdyState(void)
{
    return ds_para.DS_Para_StdyState;
}


//ctrl para
u16 ds_readctrlpara_PEEP(void)
{
    return ds_ctrlpara.DS_CtrlPara_PEEP;
}

u16 ds_readctrlpara_VT(void)
{
    return ds_ctrlpara.DS_CtrlPara_VT;
}

u16 ds_readctrlpara_f(void)
{
    return ds_ctrlpara.DS_CtrlPara_f;
}

u16 ds_readctrlpara_IE(void)
{
    return ds_ctrlpara.DS_CtrlPara_IE;
}

u16 ds_readctrlpara_Tslope(void)
{
    return ds_ctrlpara.DS_CtrlPara_Tslope;
}

u16 ds_readctrlpara_Tinsp(void)
{
    return ds_ctrlpara.DS_CtrlPara_Tinsp;
}

u16 ds_readctrlpara_Pinsp(void)
{
    return ds_ctrlpara.DS_CtrlPara_Pinsp;
}

u16 ds_readctrlpara_Psupp(void)
{
    return ds_ctrlpara.DS_CtrlPara_Psupp;
}

u16 ds_readctrlpara_Esens(void)
{
    return ds_ctrlpara.DS_CtrlPara_Esens;
}

u16 ds_readctrlpara_Psens(void)
{
    return ds_ctrlpara.DS_CtrlPara_Psens;
}

u16 ds_readctrlpara_Fsens(void)
{
    return ds_ctrlpara.DS_CtrlPara_Fsens;
}

u16 ds_readctrlpara_Sigh(void)
{
    return ds_ctrlpara.DS_CtrlPara_Sigh;
}

u16 ds_readctrlpara_Phigh(void)
{
    return ds_ctrlpara.DS_CtrlPara_Phigh;
}

u16 ds_readctrlpara_Plow(void)
{
    return ds_ctrlpara.DS_CtrlPara_Plow;
}

u16 ds_readctrlpara_Thigh(void)
{
    return ds_ctrlpara.DS_CtrlPara_Thigh;
}

u16 ds_readctrlpara_Tlow(void)
{
    return ds_ctrlpara.DS_CtrlPara_Tlow;
}

u16 ds_readctrlpara_FG(void)
{
    return ds_ctrlpara.DS_CtrlPara_FG;
}

u16 ds_readctrlpara_Tpause(void)
{
    return ds_ctrlpara.DS_CtrlPara_Tpause;
}

u16 ds_readctrlpara_PerO2(void)
{
    return ds_ctrlpara.DS_CtrlPara_PerO2;
}

DS_Mode_t ds_readpara_BreathMode(void)
{
    switch(ds_ctrlpara.DS_CtrlPara_Mode)
    {
    case VCV_MODE:
    default:        
        return DS_Mode_VCV;
    case SIMV_VCV_MODE:
        return DS_Mode_SIMVV;
    case PCV_MODE:
        return DS_Mode_PCV;
    case SIMV_PCV_MODE:
        return DS_Mode_SIMVP;
    case PRVC_MODE:
        return DS_Mode_PRVC;
    case SIMV_PRVC_MODE:
        return DS_Mode_SIMVPRVC;
    case PSV_MODE:
        if(!(AlarmsWork.presentAlarms&ALARM_APNEA))
        {
            return DS_Mode_PSV;
        }
        else
        {
            return DS_Mode_Backup;
        }
    case SIGH_MODE:
        return DS_Mode_SIGH;
    case MANUAL_MODE:
        return DS_Mode_MANUAL;
    case ACGO_MODE:
        return DS_Mode_ACGO;
    case PSV_MODE_BACK:
        return DS_Mode_Backup;
    }
}

//alarm limits
u16 ds_readhighlims_PAW(void)
{
    return ds_alrmlimits.High.DS_AlrmLim_PAW;
}

u16 ds_readlowlims_PAW(void)
{
    return ds_alrmlimits.Low.DS_AlrmLim_PAW;
}

u16 ds_readhighlims_MV(void)
{
    return ds_alrmlimits.High.DS_AlrmLim_MV;
}

u16 ds_readlowlims_MV(void)
{
    return ds_alrmlimits.Low.DS_AlrmLim_MV;
}

u16 ds_readhighlims_F(void)
{
    return ds_alrmlimits.High.DS_AlrmLim_F;
}

u16 ds_readlowlims_F(void)
{
    return ds_alrmlimits.Low.DS_AlrmLim_F;
}

u16 ds_readhighlims_FiO2(void)
{
    return ds_alrmlimits.High.DS_AlrmLim_FiO2;
}

u16 ds_readlowlims_FiO2(void)
{
    return ds_alrmlimits.Low.DS_AlrmLim_FiO2;
}

u16 ds_readhighlims_VTe(void)
{
    return ds_alrmlimits.High.DS_AlrmLim_VTe;
}

u16 ds_readlowlims_VTe(void)
{
    return ds_alrmlimits.Low.DS_AlrmLim_VTe;
}

u16 ds_readhighlims_PEEP(void)
{
    return ds_alrmlimits.High.DS_AlrmLim_PEEP;
}

u16 ds_readlowlims_PEEP(void)
{
    return ds_alrmlimits.Low.DS_AlrmLim_PEEP;
}
u16 ds_readhighlims_EtCO2(void)
{
    return ds_alrmlimits.High.DS_AlrmLim_EtCO2;
}

u16 ds_readlowlims_EtCO2(void)
{
    return ds_alrmlimits.Low.DS_AlrmLim_EtCO2;
}

u16 ds_readhighlims_FiCO2(void)
{
    return ds_alrmlimits.High.DS_AlrmLim_FiCO2;
}

u16 ds_readlowlims_FiCO2(void)
{
    return ds_alrmlimits.Low.DS_AlrmLim_FiCO2;
}

u16 ds_readhighlims_FiN2O(void)
{
    return ds_alrmlimits.High.DS_AlrmLim_FiN2O;
}

u16 ds_readlowlims_FiN2O(void)
{
    return ds_alrmlimits.Low.DS_AlrmLim_FiN2O;
}

u16 ds_readhighlims_EtAA(void)
{
    return ds_alrmlimits.High.DS_AlrmLim_EtAA;
}

u16 ds_readlowlims_EtAA(void)
{
    return ds_alrmlimits.Low.DS_AlrmLim_EtAA;
}

u16 ds_readhighlims_FiAA(void)
{
    return ds_alrmlimits.High.DS_AlrmLim_FiAA;
}

u16 ds_readlowlims_FiAA(void)
{
    return ds_alrmlimits.Low.DS_AlrmLim_FiAA;
}

u16 ds_readhighlims_Pulse(void)
{
    return ds_alrmlimits.High.DS_AlrmLim_Pulse;
}

u16 ds_readlowlims_Pulse(void)
{
    return ds_alrmlimits.Low.DS_AlrmLim_Pulse;
}

u16 ds_readhighlims_SpO2(void)
{
    return ds_alrmlimits.High.DS_AlrmLim_SpO2;
}

u16 ds_readlowlims_SpO2(void)
{
    return ds_alrmlimits.Low.DS_AlrmLim_SpO2;
}

u16 ds_readhighlims_PI(void)
{
    return ds_alrmlimits.High.DS_AlrmLim_PI;
}

u16 ds_readlowlims_PI(void)
{
    return ds_alrmlimits.Low.DS_AlrmLim_PI;
}

//patient
char* ds_readpatient_id(void)
{
    return ds_patient.DS_Patient_ID;
}

char* ds_readpatient_name(void)
{
    return ds_patient.DS_Patient_Name;
}

DS_PGender_t ds_readpatient_gender(void)
{
    DS_PGender_t tmp = DS_PGender_U;
    switch(ds_patient.DS_Patient_Gender)
    {
    case GENDER_MALE:
        tmp = DS_PGender_M;
        break;
    case GENDER_FEMALE:
        tmp = DS_PGender_F;
        break;
    default:
        tmp = DS_PGender_U;
        break;
    }
    return tmp;
}


void ds_calieratio(void)
{
    u16 insp2exp;
    
    if((DS_PARA_INSPTIME > 0) && (DS_PARA_EXPTIME))
    {
        if(DS_PARA_INSPTIME >= DS_PARA_EXPTIME)
        {
            insp2exp = DS_PARA_INSPTIME*10/DS_PARA_EXPTIME;
            if(insp2exp > 90)
            {
                Radio_IE.I = 0x00;
                Radio_IE.E = 0x00;
            }
            else
            {
                Radio_IE.I = insp2exp;
                Radio_IE.E = 0x0A;                
            }
        }
        else 
        {
            insp2exp = DS_PARA_EXPTIME*10/DS_PARA_INSPTIME;
            if(insp2exp > 990)
            {
                Radio_IE.I = 0x00;
                Radio_IE.E = 0x00;
            }
            else
            {
                Radio_IE.I = 0x0A;                  
                Radio_IE.E = insp2exp;
            }
        }
    }
    else
    {
        Radio_IE.I = 0x00;
        Radio_IE.E = 0x00;
    }    
}

