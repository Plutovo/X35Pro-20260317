#ifndef _MEDIBUSCOMM_CFG_H
#define _MEDIBUSCOMM_CFG_H
#include "lpc_types.h"
#include "Chip.h"

//#ifdef __ICCARM__



//system first init flag
//#define SYSTEM_INIT_FLAG  (u16)0x1234

//medibus debug
#define __Medibus_Debug

//Whether transmit data
#define __PPLAT
#define __PEEP
#define __PPEAK
#define __VTE
#define __VTI
#define __RATE
#define __IE
#define __MV
#define __FIO2
#define __ETCO2
#define __ETAA
//#define __ETN2O
#define __FLOWO2
//#define __FLOWN2O
//#define __FLOWAIR
//#define __PULSE
//#define __SPO2
#define __BREATHMODE

//ml
#define FLOW_MAX_MB            (u16)9999

#define MEDIBUS_CR			(u8)0x0D
#define MEDIBUS_MESSETX     (u8)0x03
#define MEDIBUS_ASCIISPACE  (u8)0x20

//data
#define MEDIBUS_PPLAT		(u8)0x74
#define MEDIBUS_PEEP		(u8)0x78
#define MEDIBUS_PEAK		(u8)0x7D
#define MEDIBUS_VTE			(u8)0x88
#define MEDIBUS_VTI			(u8)0x8B
#define MEDIBUS_RATE 		(u8)0xB4
#define MEDIBUS_IEI			(u8)0xB5
#define MEDIBUS_IEE			(u8)0xB6
#define MEDIBUS_MV			(u8)0xB9
#define MEDIBUS_FiO2		(u8)0xF0
#define MEDIBUS_EtCO2		(u8)0xDB
#define MEDIBUS_EtCO2_kPa	(u8)0xE3
#define MEDIBUS_EtCO2_mmHg	(u8)0xE6
#define MEDIBUS_FlowO2      (u8)0xE2
#define MEDIBUS_FlowN2O     (u8)0xDD
#define MEDIBUS_FlowAir     (u8)0xDE
#define MEDIBUS_EtN2O       (u8)0xFC
#define MEDIBUS_EtHAL       (u8)0xF5
#define MEDIBUS_EtENF       (u8)0xF7
#define MEDIBUS_EtISO       (u8)0xF9
#define MEDIBUS_EtSEV       (u8)0xB1
#define MEDIBUS_EtDES       (u8)0xAF
#define MEDIBUS_Pulse       (u8)0xE1
#define MEDIBUS_SpO2        (u8)0xEB

//Text Message
#define MEDIBUS_VCV			(u8)0x59
#define MEDIBUS_PCV			(u8)0x5A
#define MEDIBUS_PSV			(u8)0x5B
#define MEDIBUS_PRVC		(u8)0x5C
#define MEDIBUS_SIMV_V		(u8)0x5D
#define MEDIBUS_SIMV_P		(u8)0x5E
#define MEDIBUS_SIMV_PRVC	(u8)0x5F
#define MEDIBUS_ACGO		(u8)0x60
#define MEDIBUS_MANUAL		(u8)0x61
#define MEDIBUS_SIGH        (u8)0x62
#define MEDIBUS_UNIT_MMHG   (u8)0x22
#define MEDIBUS_UNIT_KPA    (u8)0x23
#define MEDIBUS_UNIT_SIGN   (u8)0x24

//文本信息个数
#define MEDIBUS_TEXTNUM     (u8)20

//FIFO深度和长度
#define MEDIBUS_FIFOTXNUM   (u8)200
#define MEDIBUS_FIFORXNUM   (u8)100

//Timer delay(ms)
#define MEDIBUS_TMRCOUNT2   (u16)(1.5 *1000/100)
#define MEDIBUS_TMRCOUNT3   (u16)(3   *1000/100)
#define MEDIBUS_TMRCOUNT10  (u16)(9   *1000/100)
#define MEDIBUS_TMRCOUNT20  (u16)(20  *1000/100)

//ESC:发送   SOH:响应
typedef enum
{
    MEDIBUS_ESC = 0x1B,
    MEDIBUS_SOH = 0x01
}Medibus_DIR;

typedef enum
{
    MEDIBUS_ICC         = 0x51, //init comm
    MEDIBUS_NOP         = 0x30, //Idle state:NOP
    MEDIBUS_STOP        = 0x55, //stop comm
    MEDIBUS_NAK         = 0x15, //CHK err
    MEIDBUS_MEASURED1   = 0x24, //Measured data 1
    MEIDBUS_MEASURED2   = 0x2B, //Measured data 2
    MEIDBUS_TEXTMESS    = 0x2A, //Text message
    MEIDBUS_DEVPCID     = 0x52  //dev and pc ID
}Medibus_CMD;

typedef struct
{
    u8 Medibus_RecvBuf[MEDIBUS_FIFORXNUM];
    u8 Medibus_SendBuf[MEDIBUS_FIFOTXNUM];
    u8 Medibus_SendNum;
}Medibus_Typedef;

typedef struct
{
    u8 ASCII_HIGH;
    u8 ASCII_LOW;
}Medibus_ASCII;

typedef struct
{
    u8 a; //bit:4th
    u8 b; //bit:3rd
    u8 c; //bit:2nd
    u8 d; //bit:1st
}Medibus_I2A;


typedef struct
{
    s16 PplatVal;
    s16 PEEPVal;
    s16 PpeakVal;
    u16 VTeVal;
    u16 VTiVal;
    u16 RateVal;
    u16 IEIVal;
    u16 IEEVal;
    u16 MVVal;
    u16 FiO2Val;
    u16 EtCO2Val;
    u16 EtAAVal;
    u16 EtN2OVal;
    u16 PulseVal;
    u16 SpO2Val;
    u16 FlowO2;
    u16 FlowN2O;
    u16 FlowAir;
    u16 BreathMode;
    u8  AA_Type;
    u8  CO2_Unit;
}Medibus_Measured;

typedef enum
{
    Medibus_Str = 0,
    Medibus_End,
}Medibus_Status_t;

typedef enum
{
    err_none = 0,
    err_suc,    
    err_wait,
    err_esc,
    err_soh,    
    err_cr,
    err_chk,    
}Medibus_err_t;

typedef enum 
{
    dp_allow = 0,    //allow
    dp_Phb,          //prohibit
}Medibus_dp_t;


//Medibus Finite state machine define
#define FSM_STATE u8
typedef FSM_STATE(*Procedure)(void*);

typedef enum
{
    FSM_IDLE,
    FSM_RESET,
    FSM_DEVID,
    FSM_PCID,
    FSM_START,
    FSM_STOP
}MedibusFSM_States;

typedef struct
{
    Medibus_Status_t FSM_Recv_State;
    Medibus_err_t FSM_Err_Recv; //
    Medibus_dp_t FSM_DP_Recv;   //是否允许读FIFO
    
    u16 FSM_Tmr1Count;          //timer count
    u16 FSM_Tmr1TimOut;         //timeout
    u8  FSM_Tmr1Flag;           //bit0: start  flag
                                //bit1: finish flag
    u16 FSM_Tmr2Count;          //timer count
    u16 FSM_Tmr2TimOut;         //timeout
    u8  FSM_Tmr2Flag;           //bit0: start  flag
                                //bit1: finish flag  
    u8  FSM_RecvFlag;           //uart recv char flag
    u8  FSM_CommActiveFlag;     //comm is build flag
    u8  FSM_DevUserCommStop;    //dev user stop communication flag
                                //0:enable comm; 1:disable comm
    u8  FSM_PCResStopFlag;      //0:PC has repsonse stop cmd 
                                //1:PC has not response stop cmd
}MedibusFSM_Var;


#endif


