#ifndef _DATASHARE_H
#define _DATASHARE_H
#include "lpc_types.h"
#include "Chip.h"
#include "DataShare_inc_ext.h"
#include "DataShare_cfg.h"

typedef struct
{
    u16 I;
    u16 E;
}DS_IE_t;

typedef enum
{
    DS_CO2Unit_per = 0,
    DS_CO2Unit_mmHg,
    DS_CO2Unit_Kpa,
}DS_CO2Unit_t;

typedef enum
{
    DS_Mode_VCV = 0,
    DS_Mode_SIMVV,
    DS_Mode_PCV,
    DS_Mode_SIMVP,
    DS_Mode_PRVC,
    DS_Mode_SIMVPRVC,
    DS_Mode_PSV,
    DS_Mode_SIGH,
    DS_Mode_MANUAL,
    DS_Mode_ACGO,
    DS_Mode_Backup,
    DS_Mode_Duo,
    DS_Mode_APRV,
    DS_Mode_VS,
    DS_Mode_NIVPC,
    DS_Mode_NIVPSV,
    DS_Mode_Lst,
}DS_Mode_t;

typedef struct
{
    u16 DS_Para_None;
    s16 DS_Para_PEEP;
    s16 DS_Para_Ppeak;
    s16 DS_Para_Pplat;
    u16 DS_Para_Pmean;
    u16 DS_Para_VTe;
    u16 DS_Para_VTi;
    u16 DS_Para_MV;
    u16 DS_Para_MVspn;
    u16 DS_Para_f;
    u16 DS_Para_fspn;
    u16 DS_Para_IEI;
    u16 DS_Para_IEE;
    u16 DS_Para_Compl;
    u16 DS_Para_RST;
    u16 DS_Para_Pmin;
    u16 DS_Para_RSBI;
    u16 DS_Para_CST;
    u16 DS_Para_PEEPI;
    u16 DS_Para_EtCO2;
    u16 DS_Para_FiCO2;
    u16 DS_Para_FiO2;
    u16 DS_Para_EtO2;
    u16 DS_Para_FiN2O;
    u16 DS_Para_EtN2O;
    u16 DS_Para_FiAA;
    u16 DS_Para_EtAA;
    u16 DS_Para_FiAA2;
    u16 DS_Para_EtAA2;
    u16 DS_Para_MAC;
    u16 DS_Para_Flow_N2O;
    u16 DS_Para_Flow_Air;
    u16 DS_Para_Flow_O2;
    u16 DS_Para_SpO2;
    u16 DS_Para_PR;
    u16 DS_Para_PI;  
    enum AA_GAS_ID DS_Para_AA1ID;
    enum AA_GAS_ID DS_Para_AA2ID; 
    //GAS_Channel_t DS_Para_FlowChannel;
    u32 DS_Para_StdyState;
}DS_Para_t;

typedef struct
{
    u16 DS_CtrlPara_PEEP;
    u16 DS_CtrlPara_VT;
    u16 DS_CtrlPara_f;
    u16 DS_CtrlPara_IE;
    u16 DS_CtrlPara_Tslope;
    u16 DS_CtrlPara_Tinsp;
    u16 DS_CtrlPara_Pinsp;
    u16 DS_CtrlPara_Psupp;
    u16 DS_CtrlPara_Tpause;
    u16 DS_CtrlPara_Esens;
    u16 DS_CtrlPara_Psens;
    u16 DS_CtrlPara_Fsens;
    u16 DS_CtrlPara_Sigh;
    u16 DS_CtrlPara_FG;
    u16 DS_CtrlPara_PerO2;
    u16 DS_CtrlPara_Mode;
    u16 DS_CtrlPara_Phigh;
    u16 DS_CtrlPara_Plow;
    u16 DS_CtrlPara_Thigh;
    u16 DS_CtrlPara_Tlow;
}DS_CtrlPara_t;

typedef struct
{
    u16 DS_AlrmLim_PAW;
    u16 DS_AlrmLim_MV;
    u16 DS_AlrmLim_F;
    u16 DS_AlrmLim_FiO2;
    u16 DS_AlrmLim_VTe;
    u16 DS_AlrmLim_PEEP;
    u16 DS_AlrmLim_EtCO2;
    u16 DS_AlrmLim_FiCO2;
    u16 DS_AlrmLim_FiN2O;
    u16 DS_AlrmLim_EtAA;
    u16 DS_AlrmLim_FiAA;
    u16 DS_AlrmLim_Pulse;
    u16 DS_AlrmLim_SpO2;
    u16 DS_AlrmLim_PI;
}DS_AlrmLim_t;


typedef struct
{
    char *DS_Patient_ID;
    char *DS_Patient_Name;
    PGender_t DS_Patient_Gender;
}DS_Patient_t;

typedef struct
{
    DS_AlrmLim_t High;
    DS_AlrmLim_t Low;
}DS_AlrmLims_t;

typedef enum
{
    DS_GasMod_None = 0,
    DS_GasMod_MasimoIRMAAX,
    DS_GasMod_MasimoIRMACO2,
    DS_GasMod_MasimoISAOR,
    DS_GasMod_MasimoISAAX,
    DS_GasMod_MasimoISACO2,
    DS_GasMod_DragerAAO2,
    DS_GasMod_DragerAA,
    DS_GasMod_KingSTCO2,
    DS_GasMod_NMEDCO2    
}DS_GasMod_t;

typedef enum
{
    DS_SpO2Mod_None = 0,
    DS_SpO2Mod_MasimoSpO2 = 0,
    DS_SpO2Mod_CNSpO2,
}DS_SpO2Mod_t;

typedef enum
{
    DS_MachMod_Anes = 0,
    DS_MachMod_Vent,
}DS_MachMod_t;

typedef struct
{
    u32 ID;
    u8* Msg;
}DS_AlrmMsg_t;

typedef enum
{
    DS_PGender_U = 'U',
    DS_PGender_M = 'M',
    DS_PGender_F = 'F',
}DS_PGender_t;

void ds_init(void);
void ds_getdatashare(void);
void ds_getpara(void);
void ds_getctrlpara(void);
void ds_getalrmlimits(void);
void ds_getalrmmsg(void);
void ds_getpatient(void);
void ds_getmodel(void);
bool ds_alrmisexist(void);
//read
s16 ds_readpara_PEEP(void);
s16 ds_readpara_Pplat(void);
s16 ds_readpara_Ppeak(void);
u16 ds_readpara_Pmean(void);
u16 ds_readpara_VTe(void);
u16 ds_readpara_VTi(void);
u16 ds_readpara_MV(void);
u16 ds_readpara_MVspn(void);
u16 ds_readpara_f(void);
u16 ds_readpara_fspn(void);
u16 ds_readpara_IEI(void);
u16 ds_readpara_IEE(void);
u16 ds_readpara_Compl(void);
u16 ds_readpara_RST(void);
u16 ds_readpara_Pmin(void);
u16 ds_readpara_RSBI(void);
u16 ds_readpara_CST(void);
u16 ds_readpara_PEEPI(void);

u16 ds_readpara_EtCO2(void);
u16 ds_readpara_FiCO2(void);
u16 ds_readpara_FiO2(void);
u16 ds_readpara_EtO2(void);
u16 ds_readpara_FiN2O(void);
u16 ds_readpara_EtN2O(void);
u16 ds_readpara_FiAA(void);
u16 ds_readpara_EtAA(void);
u16 ds_readpara_FiAA2(void);
u16 ds_readpara_EtAA2(void);
u16 ds_readpara_MAC(void);
u16 ds_readpara_Flow_N2O(void);
u16 ds_readpara_Flow_Air(void);
u16 ds_readpara_Flow_O2(void);
u16 ds_readpara_SpO2(void);
u16 ds_readpara_PR(void);
u16 ds_readpara_PI(void);
DS_CO2Unit_t ds_readpara_CO2Unit(void);
DS_Mode_t ds_readpara_BreathMode(void);
u32 ds_readpara_StdyState(void); 
enum AA_GAS_ID ds_readpara_AA1ID(void);
enum AA_GAS_ID ds_readpara_AA2ID(void);
//GAS_Channel_t ds_readpara_flowchannel(void);


u16 ds_readctrlpara_PEEP(void);
u16 ds_readctrlpara_VT(void);
u16 ds_readctrlpara_f(void);
u16 ds_readctrlpara_IE(void);
u16 ds_readctrlpara_Tslope(void);
u16 ds_readctrlpara_Tinsp(void);
u16 ds_readctrlpara_Pinsp(void);
u16 ds_readctrlpara_Psupp(void);
u16 ds_readctrlpara_Esens(void);
u16 ds_readctrlpara_Psens(void);
u16 ds_readctrlpara_Fsens(void);
u16 ds_readctrlpara_Sigh(void);
u16 ds_readctrlpara_Phigh(void);
u16 ds_readctrlpara_Plow(void);
u16 ds_readctrlpara_Thigh(void);
u16 ds_readctrlpara_Tlow(void);
u16 ds_readctrlpara_FG(void);
u16 ds_readctrlpara_PerO2(void);
u16 ds_readctrlpara_Tpause(void);


u16 ds_readhighlims_PAW(void);
u16 ds_readlowlims_PAW(void);
u16 ds_readhighlims_MV(void);
u16 ds_readlowlims_MV(void);
u16 ds_readhighlims_F(void);
u16 ds_readlowlims_F(void);
u16 ds_readhighlims_FiO2(void);
u16 ds_readlowlims_FiO2(void);
u16 ds_readhighlims_VTe(void);
u16 ds_readlowlims_VTe(void);
u16 ds_readhighlims_PEEP(void);
u16 ds_readlowlims_PEEP(void);
u16 ds_readhighlims_EtCO2(void);
u16 ds_readlowlims_EtCO2(void);
u16 ds_readhighlims_FiCO2(void);
u16 ds_readlowlims_FiCO2(void);
u16 ds_readhighlims_FiN2O(void);
u16 ds_readlowlims_FiN2O(void);
u16 ds_readhighlims_EtAA(void);
u16 ds_readlowlims_EtAA(void);
u16 ds_readhighlims_FiAA(void);
u16 ds_readlowlims_FiAA(void);
u16 ds_readhighlims_Pulse(void);
u16 ds_readlowlims_Pulse(void);
u16 ds_readhighlims_SpO2(void);
u16 ds_readlowlims_SpO2(void);
u16 ds_readhighlims_PI(void);
u16 ds_readlowlims_PI(void);

bool ds_readmsg_alrmhigh(DS_AlrmMsg_t *alrm);
bool ds_readmsg_alrmlow(DS_AlrmMsg_t *alrm);

char* ds_readpatient_id(void);
char* ds_readpatient_name(void);
DS_PGender_t ds_readpatient_gender(void);

void ds_calieratio(void);

#endif


