#include "HL7_cfg.h"
#include "HL7_maptble.h"
#include "HL7_OBX_Para.h"
#include "HL7_OBX_CtrlPara.h"
#include "HL7_OBX_AlrmLim.h"
#include "HL7_user.h"

//监测参数
const static char ParaID_buf[HL7_Para_Last] = 
{
    HL7_Para_None,
    HL7_Para_PEEP,
    HL7_Para_Ppeak,
    HL7_Para_Pplat,
    HL7_Para_Pmean,
    HL7_Para_VTe,
    HL7_Para_VTi,
    HL7_Para_MV,
    HL7_Para_MVspn,
    HL7_Para_f,
    HL7_Para_fspn,
    HL7_Para_IEI,
    HL7_Para_IEE,    
    HL7_Para_Compl, //cdyn
    HL7_Para_RST,
    HL7_Para_Pmin,
    HL7_Para_EtCO2,
    HL7_Para_FiCO2,
    HL7_Para_FiO2,
    HL7_Para_SpO2,
    HL7_Para_PR,
    HL7_Para_PI,
#if (_HL_TYPE == _HL7_ANES)
    HL7_Para_EtO2,    
    HL7_Para_FiN2O,
    HL7_Para_EtN2O,
    HL7_Para_FiAA,
    HL7_Para_EtAA,
    HL7_Para_FiAA2,
    HL7_Para_EtAA2,
    HL7_Para_MAC,
    HL7_Para_Flow_N2O,
    HL7_Para_Flow_Air,
    HL7_Para_Flow_O2,
#else
    HL7_Para_RSBI,
    HL7_Para_PEEPI,
    HL7_Para_CST,
#endif   
};


//控制参数
const static char CtrlParaID_buf[HL7_CtrlParatp_Last] = 
{
    HL7_CtrlParatp_None,
    HL7_CtrlParatp_PEEP,
    HL7_CtrlParatp_VT,
    HL7_CtrlParatp_f,
    HL7_CtrlParatp_IEI,
    HL7_CtrlParatp_IEE,
    HL7_CtrlParatp_Tslope,
    HL7_CtrlParatp_Tinsp,    
    HL7_CtrlParatp_Pinsp,
    HL7_CtrlParatp_Psupp,
    HL7_CtrlParatp_Tpause,
    HL7_CtrlParatp_Esens,
    HL7_CtrlParatp_Psens,
    HL7_CtrlParatp_Fsens,
    HL7_CtrlParatp_Sigh,
#if (_HL_TYPE == _HL7_ANES)    
    HL7_CtrlParatp_FG,
    HL7_CtrlParatp_PerO2,
#else
    HL7_CtrlParatp_Phigh,
    HL7_CtrlParatp_Plow,
    HL7_CtrlParatp_Phigh,
    HL7_CtrlParatp_Plow,
#endif
};

//报警限制
const static char AlrmLimID_buf[HL7_AlrmLimtp_Last] = 
{
   HL7_AlrmLimtp_None, 
   HL7_AlrmLimtp_PAW_H,
   HL7_AlrmLimtp_PAW_L,
   HL7_AlrmLimtp_MV_H,
   HL7_AlrmLimtp_MV_L,
   HL7_AlrmLimtp_F_H,
   HL7_AlrmLimtp_F_L,
   HL7_AlrmLimtp_FIO2_H,
   HL7_AlrmLimtp_FIO2_L,
   HL7_AlrmLimtp_VTE_H,
   HL7_AlrmLimtp_VTE_L,
#if (_HL_TYPE == _HL7_VENT)   
   HL7_AlrmLimtp_PEEP_H,
   HL7_AlrmLimtp_PEEP_L,
#endif   
   HL7_AlrmLimtp_ETCO2_H,
   HL7_AlrmLimtp_ETCO2_L,
   HL7_AlrmLimtp_FICO2_H,
   HL7_AlrmLimtp_FIN2O_H,
   HL7_AlrmLimtp_FIN2O_L,
   HL7_AlrmLimtp_ETAA_H,
   HL7_AlrmLimtp_ETAA_L,
   HL7_AlrmLimtp_FIAA_H,
   HL7_AlrmLimtp_FIAA_L,
   HL7_AlrmLimtp_PULSE_H,
   HL7_AlrmLimtp_PULSE_L,
   HL7_AlrmLimtp_SPO2_H,
   HL7_AlrmLimtp_SPO2_L,
   HL7_AlrmLimtp_PI_H,
   HL7_AlrmLimtp_PI_L, 
};

//报警表0
const static u8 *alrm_buf_grp0[64] = 
{
    HL7_Alrm_grp0_0,    HL7_Alrm_grp0_1,    HL7_Alrm_grp0_2,    HL7_Alrm_grp0_3, 
    HL7_Alrm_grp0_4,    HL7_Alrm_grp0_5,    HL7_Alrm_grp0_6,    HL7_Alrm_grp0_7,
    HL7_Alrm_grp0_8,    HL7_Alrm_grp0_9,    HL7_Alrm_grp0_10,   HL7_Alrm_grp0_11,
    HL7_Alrm_grp0_12,   HL7_Alrm_grp0_13,   HL7_Alrm_grp0_14,   HL7_Alrm_grp0_15,
    HL7_Alrm_grp0_16,   HL7_Alrm_grp0_17,   HL7_Alrm_grp0_18,   HL7_Alrm_grp0_19,
    HL7_Alrm_grp0_20,   HL7_Alrm_grp0_21,   HL7_Alrm_grp0_22,   HL7_Alrm_grp0_23,
    HL7_Alrm_grp0_24,   HL7_Alrm_grp0_25,   HL7_Alrm_grp0_26,   HL7_Alrm_grp0_27,
    HL7_Alrm_grp0_28,   HL7_Alrm_grp0_29,   HL7_Alrm_grp0_30,   HL7_Alrm_grp0_31,
    HL7_Alrm_grp0_32,   HL7_Alrm_grp0_33,   HL7_Alrm_grp0_34,   HL7_Alrm_grp0_35,
    HL7_Alrm_grp0_36,   HL7_Alrm_grp0_37,   HL7_Alrm_grp0_38,   HL7_Alrm_grp0_39,
    HL7_Alrm_grp0_40,   HL7_Alrm_grp0_41,   HL7_Alrm_grp0_42,   HL7_Alrm_grp0_43,
    HL7_Alrm_grp0_44,   HL7_Alrm_grp0_45,   HL7_Alrm_grp0_46,   HL7_Alrm_grp0_47,
    HL7_Alrm_grp0_48,   HL7_Alrm_grp0_49,   HL7_Alrm_grp0_50,   HL7_Alrm_grp0_51,
    HL7_Alrm_grp0_52,   HL7_Alrm_grp0_53,   HL7_Alrm_grp0_54,   HL7_Alrm_grp0_55,
    HL7_Alrm_grp0_56,   HL7_Alrm_grp0_57,   HL7_Alrm_grp0_58,   HL7_Alrm_grp0_59,
    HL7_Alrm_grp0_60,   HL7_Alrm_grp0_61,   HL7_Alrm_grp0_62,   HL7_Alrm_grp0_63,
};

//报警表1
const static u8 *alrm_buf_grp1[64] = 
{
    HL7_Alrm_grp1_0,    HL7_Alrm_grp1_1,    HL7_Alrm_grp1_2,    HL7_Alrm_grp1_3, 
    HL7_Alrm_grp1_4,    HL7_Alrm_grp1_5,    HL7_Alrm_grp1_6,    HL7_Alrm_grp1_7, 
    HL7_Alrm_grp1_8,    HL7_Alrm_grp1_9,    HL7_Alrm_grp1_10,   HL7_Alrm_grp1_11, 
    HL7_Alrm_grp1_12,   HL7_Alrm_grp1_13,   HL7_Alrm_grp1_14,   HL7_Alrm_grp1_15, 
    HL7_Alrm_grp1_16,   HL7_Alrm_grp1_17,   HL7_Alrm_grp1_18,   HL7_Alrm_grp1_19, 
    HL7_Alrm_grp1_20,   HL7_Alrm_grp1_21,   HL7_Alrm_grp1_22,   HL7_Alrm_grp1_23, 
    HL7_Alrm_grp1_24,   HL7_Alrm_grp1_25,   HL7_Alrm_grp1_26,   HL7_Alrm_grp1_27, 
    HL7_Alrm_grp1_28,   HL7_Alrm_grp1_29,   HL7_Alrm_grp1_30,   HL7_Alrm_grp1_31, 
    HL7_Alrm_grp1_32,   HL7_Alrm_grp1_33,   HL7_Alrm_grp1_34,   HL7_Alrm_grp1_35, 
    HL7_Alrm_grp1_36,   HL7_Alrm_grp1_37,   HL7_Alrm_grp1_38,   HL7_Alrm_grp1_39, 
    HL7_Alrm_grp1_40,   HL7_Alrm_grp1_41,   HL7_Alrm_grp1_42,   HL7_Alrm_grp1_43, 
    HL7_Alrm_grp1_44,   HL7_Alrm_grp1_45,   HL7_Alrm_grp1_46,   HL7_Alrm_grp1_47, 
    HL7_Alrm_grp1_48,   HL7_Alrm_grp1_49,   HL7_Alrm_grp1_50,   HL7_Alrm_grp1_51, 
    HL7_Alrm_grp1_52,   HL7_Alrm_grp1_53,   HL7_Alrm_grp1_54,   HL7_Alrm_grp1_55, 
    HL7_Alrm_grp1_56,   HL7_Alrm_grp1_57,   HL7_Alrm_grp1_58,   HL7_Alrm_grp1_59, 
    HL7_Alrm_grp1_60,   HL7_Alrm_grp1_61,   HL7_Alrm_grp1_62,   HL7_Alrm_grp1_63, 
};

//模式
const static u8 *mode_buf[DS_Mode_Lst] = 
{
    {HL7_MSG_VCV             },
    {HL7_MSG_SIMVV           },
    {HL7_MSG_PCV             },
    {HL7_MSG_SIMVP           },
    {HL7_MSG_PRVC            },
    {HL7_MSG_SIMVPRVC        },
    {HL7_MSG_PSV             },
    {HL7_MSG_SIGH            },
    {HL7_MSG_MANUAL          },
    {HL7_MSG_ACGO            },
    {HL7_MSG_BACKUP          },
};

char HL7_map_read_paraid(u32 idx)
{
    if(idx < HL7_Para_Last)
    {
        return ParaID_buf[idx];
    }
    return 0;
}

char HL7_map_read_ctrlparaid(u32 idx)
{
    if(idx < HL7_CtrlParatp_Last)
    {
        return CtrlParaID_buf[idx];
    }
    return 0;
}

char HL7_map_read_alrmlimid(u32 idx)
{
    if(idx < HL7_AlrmLimtp_Last)
    {
        return AlrmLimID_buf[idx];
    }
    return 0;
}

u8 *HL7_map_read_alrmid_grp0(u32 idx)
{
    return (u8*)alrm_buf_grp0[idx];
}

u8 *HL7_map_read_alrmid_grp1(u32 idx)
{
    return (u8*)alrm_buf_grp1[idx];  
}

u8 *HL7_map_read_mode(DS_Mode_t idx)
{
    return (u8*)mode_buf[idx];  
}




