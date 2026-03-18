//
#include "Static_Menu_Creat.h"
#include "Key_Handle.h"
#include "MainTask.h"
/**************************************************************************************************/

//*************************************************************************************************/
//设置值命令
#define         VT_COMMAND_SET          0x53            //设定的潮气量值
#define         PT_COMMAND_SET          0x58            //设定的触发灵敏度
#define         FLOW_TRIGGER_COMMAND_SET    0x5B
#define         FIO2_COMMAND_SET        0x5D            //氧浓度设定值
#define         TI_COMMAND_SET          0x62            //吸气时间设定值
#define         TP_COMMAND_SET          0x67            //屏气时间
#define         PSV_COMMAND_SET         0x6C            //压力支持设定值
#define         PEEP_COMMAND_SET        0x71            //peep设定值
#define         F_COMMAND_SET           0x76            //A/C模式下，呼吸频率设定值
#define         F_SIMV_COMMAND_SET      0x7B            //SIMV模式下，呼吸频率设定值
#define         PH_COMMAND_SET          0x7C            //high pressure level
#define         PL_COMMAND_SET          0x80            //压力
#define         THIGH_COMMAND_SET       0x81
#define         TLOW_COMMAND_SET        0x82
#define         F_BIPAP_COMMAND_SET     0x83
#define         PHL_COMMAND_SET         0x85            //压力上限设定值
#define         TAP_COMMAND_SET         0x8A            //窒息时间设定值
#define         PCV_COMMOND_SET         0xBC
#define         ETS_COMMAND_SET         0xBD

#define         SIGH_TIMES_COMMAND_SET  0x86            //叹息间隔设定值 //  20130328
#define         TSLOPE_COMMAND_SET      0xC9            //设定的压力上升时间 

/**************************************************************************************************/
#define         O2_SCANT_COMMAND_ALARM      0x99            //氧气不足报警
#define         AIR_SCANT_COMMAND_ALARM     0x9E            //空气不足报警
#define         ALL_DATA_REQUSET            0xA3
#define         PRESSURE_TRIGGER_COMMAND    0xA8
#define         OXY_CORRECT_COMPLETE_COMMAND    0xAD        //氧浓度校正完成命令
#define         SYSTEM_MODE_COMMAND         0x8F            //呼吸模式切换命令码
#define         RUN_STANDBY_COMMAND         0x94            //关机命令
#define         ID_COMMAND                  0xC1
#define         COMPLIANCE_MEASURE_COMMAND  0xc6
#define         RESIST_MEASURE_COMMAND      0xcb
#define         PEEPI_MEASURE_COMMAND       0xD0
#define         PEEPI_COMMAND_VALUE         0xD5
#define         RESIST_COMMAND_VALUE        0xDA
#define         COMPLIANCE_COMMAND_VALUE    0xDF

#define         AIR_PRESSURE_COMMAND_VALUE      0xE0
#define         O2_GAS_PRESSURE_COMMAND_VALUE      0xE1 //  20130328
#define         BAROMETER_RECV_COMMAND_VALUE       0xE2    //大气压力   GUI发送命令  djq add 20140304   BAROMETER 
#define         BAROMETER_SEND_COMMAND_VALUE       0xE3    //大气压力   GUI接收命令  djq add 20140304   BAROMETER 
#define         BAROMETER_SEND_STAND_COMMAND_VALUE 0xE5    //校验地大气压力值
#define         BACKUP_MODE_COMMAND                0xEC   //sun add 20141224
#define         PRVC_VT_LOW_COMMAND         0xF8
#define         ABSORBER_DISCONNECT_COMMAND             0xB4    //sun add 20140710  吸收罐断开报警

#define         FLOW_COMMAND_VALUE          0xE4
#define         PRESSURE_COMMAND_VALUE      0xE9
#define         SELF_TEST_COMMAND       0xEA
#define         VOLUME_COMMAND_VALUE    0xEE
#define         VTI_COMMAND_VALUE       0x21            //吸气潮气量值传送命令码
#define         VES_COMMAND_VALUE       0x26            //自主呼吸分钟通气量值传送命令码
#define         VE_COMMAND_VALUE        0x2B            //分钟通气量值
#define         PEEP_COMMAND_VALUE      0x30            //PEEP 监测值
#define         Pp_COMMAND_VALUE        0x35            //平台压监测值
#define         Pm_COMMAND_VALUE        0x3A            //平均压监测值
#define         Pk_COMMAND_VALUE        0x3F            //峰值压力监测值
#define         Ft_COMMAND_VALUE        0x44            //总计呼吸频率
#define         FSPN_COMMAND_VALUE      0x49            //自主呼吸频率数
#define         VTE_COMMAND_VALUE       0x4E            //呼气潮气量值传送命令码
#define         PMIN_COMMAND_VALUE              0x4F
#define         STATIC_COMPLIANCE_COMMAND_VALUE 0xF8
#define         FIO2_COMMAND_VALUE      0xB2                    //氧浓度监测值
#define         FIO2_STATUS_COMMAND     0xEB
#define         EXP_SEND_COMMAND        0xF2
#define         INSP_SEND_COMMAND       0xF3
#define         BARO_SEND_COMMAND       0xF4
#define         INSP_HOLD_COMMAND       0xF5
#define         EXP_PAUSE_COMMAND       0xF6
#define         O2_SUCTION_COMMAND      0xF7
#define         MANUAL_INSP_COMMAND     0xF9
#define         NEBULIZE_SEND_COMMAND   0xFB
#define         TRIG_ACTIVE_COMMAND     0xFD
#define         ESENSE_COMMAND_VALUE    0xC1  //sun add 20140623
#define         DIRVER_GAS_VALUE        0x1A            //驱动气体类型

#define         VTSUPPORT_COMMAND_SET   0xC5 // vs模式潮气量支持            
#define         PHIGH_COMMAND_SET   0x87 // Phigh            
#define         PLOW_COMMAND_SET   0x88 // Plow            

#define			SWITCH_COMMAND_SET              0xB7	     //NIF P0.1 肺复张 开关共用一个命令
#define         LUNGPAWTIME_COMMAND_SET         0xBB         //肺复张压力和时间

//

//BDU software  version command recieved from usart0   //  20130328
#define         BDU_MAIN_VERSION_COMMAND    0x11
#define         BDU_SUB_VERSION_COMMAND     0x15
#define         UPDATE_COMMAND              0xAF    //sun add 20140514 用于程序升级命令
#define         VERSION_ASK_COMMAND         0x10    //向BDU请求版本号码
//--------------------------------------------------------------------------------------------------
//操作识别码
#define         OK_COMMAND                  0x00        //接收成功命令码
#define         ERROR_COMMAND               0xFF        //接收失败命令码
#define         PRESS_ZERO_CORRECT_COM      0x05        //压力零点校正
#define         PRESS_GAIN_CORRECT_COM      0x0A        //压力增益校正
#define         OXY_ZERO_CORRECT_COM        0x0E        //氧浓度零点校正
#define         OXY_GAIN_CORRECT_COM        0x14        //氧浓度增益校正
#define         EXP_VALVE_CORRECT_COM       0x19        //呼气阀校正 //0x22
#define         FLOW_SENSOR_CORRECT_COM     0x1D        //流量传感器校正
#define         FACTORY_VALVE_CORRECT_COM   0x22        //工厂校正 //吸气阀
#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C) 
#define         LOCAL_VALVE_CORRECT_COM     0x2C//0x2C//0x27        //现场校正 //呼气流量传感器
#define         CORRECT_FAILURE_COM         0xAA//0xAA//0xFF        //校正失败
#define         CORRECT_SUSCESS_COM         0x55//0x55//0x00        //校正成功
#else
#define         LOCAL_VALVE_CORRECT_COM     0x27//0x2C//0x27        //现场校正 //呼气流量传感器
#define         CORRECT_FAILURE_COM         0xFF//0xAA//0xFF        //校正失败
#define         CORRECT_SUSCESS_COM         0x00//0x55//0x00        //校正成功
#endif
//
#define         ACGO_MODE_COMMAND           0x3A //  20130415
#define         ACGO_OK_COMMAND             0x55
#define         ACGO_ERROR_COMMAND          0xAA

#define         MANUAL_MODE_COMMAND         0x35 //  20130417
#define         MANUAL_OK_COMMAND           0x55
#define         MANUAL_ERROR_COMMAND        0xAA


#define         START_COMMAND               0x55
#define         FINISH_COMMAND              0xAA
/**************************************************************************************************/
//send parameter type flag
/**************************************************************************************************/
#define         SEND_COMMAND_NUM            50            //BDU通讯命令的个数                  

#define         SEND_SET_VT_FLAG                          0x00001  //VT
#define         SEND_SET_PT_TRIG_FLAG                     0x00002  //Psens
#define         SEND_SET_FLOW_TRIG_FLAG                   0x00004  //Fsens
#define         SEND_SET_FIO2_FLAG                        0x00008  //FiO2
#define         SEND_SET_TI_FLAG                          0x00010  //Ti
#define         SEND_SET_PSV_FLAG                         0x00020  //Psupp
#define         SEND_SET_PEEP_FLAG                        0x00040  //PEEP
#define         SEND_SET_F_FLAG                           0x00080  //FREQ
#define         SEND_SET_F_SIMV_FLAG                      0x00100  //SIMV_FREQ
#define         SEND_SET_PCV_FLAG                         0x00200  //Pinsp
#define         SEND_SET_TAP_FLAG                         0x00400  //Apnea_Timer
#define         SEND_SET_F_BIPAP_FLAG                     0x00800  // BIPAP Rate
#define         SEND_SET_PHL_FLAG                         0x01000  //Ph
#define         SEND_RUN_STANDBY_FLAG                     0x02000  //
#define         SEND_O2_SCANT_FLAG                        0x04000

#define         SEND_AIR_SCANT_FLAG                       0x08000 //0x08000

#define         SEND_APNEA_FLAG                           0x10000
#define         SEND_SYSTEM_MODE_FLAG                     0x20000
#define         SEND_TRIG_ACTIVE_FLAG                     0x40000
#define         SEND_INSP_HOLD_FLAG                       0x80000 //
#define         SEND_EXP_HOLD_FLAG                        0x100000
#define         SEND_SUCTION_FLAG                         0x200000
#define         SEND_MANUAL_INSP_FLAG                     0x400000

#define         SEND_SET_THIGH_FLAG                       0x800000 //
#define         SEND_SET_TLOW_FLAG                        0x1000000

#define         SEND_SET_TP_FLAG                          0x2000000  //Pause
#define         SEND_PRESSURE_CALIBRATION_FLAG            0x4000000
#define         SEND_FLOW_CALIBRATION_FLAG                0x8000000 //
#define         SEND_OXY_CALIBRATION_FLAG                 0x10000000
#define         SEND_EXP_VALVE_CALIBRATION_FLAG           0x20000000
#define         SEND_INSP_VALVE_CALIBRATION_FACTORY_FLAG  0x40000000
#define         SEND_EXP_FLOW_SENSOR_CALIBRATION_FLAG     0x80000000//
#define         SEND_SIGH_TIME_FLAG                       0x100000000 //  20130221
#define         SEND_SET_TSLOP_FLAG                       0x200000000 //  20130607
//#define         SEND_RECIEVE_MAIN_VERSION_FLAG            0x200000000//未用到
#define         SEND_RECIEVE_SUB_VERSION_FLAG             0x400000000//未用到
#define         SEND_BAROMETER_FLAG                       0x800000000  //djq add 20140305
#define         SEND_BAROMETER_STAND_FLAG                0x1000000000  //djq add 20140305
#define         SEND_UPDATEPROGRAM_FLAG                  0x2000000000   //sun add 20140514
#define         SEND_SET_ESENSE_FLAG                     0x4000000000   //sun add 20140623
#define         SEND_BACKUP_MODE_FLAG                    0x8000000000   //sun add 20141202
#define         SEND_ASK_VERSION_FORBDU                 0x10000000000   //向BDU请求版本号
#define         SEND_DIRVER_GAS_FORBDU                  0x20000000000   //向BDU发送驱动器气体 45

#define         SEND_SET_PHIGH_FLAG                     0x40000000000 // Phigh
#define         SEND_SET_PLOW_FLAG                      0x80000000000 // Plow

#define         SEND_SET_VTSUPP_FLAG                   0x100000000000 // Vt Support
//#define         SEND_SET_PHLIMIT_FLAG                  0x200000000000 // Pressure high limit
#define         SEND_SET_VS_F_FLAG                     0x200000000000 // VS Rate

#define			SEND_NPLSWITCH_FLAG			           0x400000000000	  //NIF P0.1 肺复张
#define         SEND_LUNGPAWTIME_FLAG                  0x800000000000	  //肺复张时间和paw
                                                        
//USART1 transmit register flag
#define         SEND_ALARM_UPDATE_FLAG          0x01
#define         SEND_LED_UPDATE_FLAG            0x02
#define         SEND_HANDSHAKE_FLAG             0x04
//*//  20130328
#define         SEND_BUZZER_CONTROL_FLAG        0x08
#define         SEND_BUZZER_VOLUME_FLAG         0x10
#define         SEND_BUZZER_TEST_FLAG           0x20
#define         SEND_BUZZER_START_FLAG          0x40 //  20130402

#define         U3SEND_UPDATEPROGRAM_FLAG       0x80  //sun add 20140514
#define         U3SEND_MACHINE_MODE_FLAG       0x100
//
#define         MEASURE_COMPLIANCE_FLAG         0x01
#define         MEASURE_PEEPI_FLAG              0x02
#define         MEASURE_RESISTANCE_FLAG         0x04
//
#define         KEY_SEND_COM                    0xC4
#define         HANDSHAKE_SEND_COM              0xC2
#define         KNOB_SEND_COM                   0xc3
#define         TS_XSCALE_SEND_COM              0xC9
#define         TS_YSCALE_SEND_COM              0xCA
#define         ALARM_STATE_CHANGE_COM          0xC7
#define         LED_CHANGE_COM                  0xC8
//*//  20133028
#define         BUZZER_CONTROL_COM              0xCD
#define         BUZZER_TEST_COM                 0xCC
#define         BUZZER_START_COM                0xCF   //  20130402
#define         COMM_MACHINE_MODE               0xCB

//PMU software  version command recieved from usart3   //  20130227
#define         PMU_MAIN_VERSION_COMMAND        0x11
#define         PMU_SUB_VERSION_COMMAND         0x15
/**************************************************************************************************/
#define    SEND_VCV_RELATED_PARA           0x00020600DF
#define    SEND_PCV_RELATED_PARA           0x02000602DF
#define    SEND_PRVC_RELATED_PARA          0x02000600DF
#define    SEND_SIMV_VCV_RELATED_PARA      0x420006017F
#define    SEND_SIMV_PCV_RELATED_PARA      0x420006037E
#define    SEND_SIMV_PRVC_RELATED_PARA     0x020006017F
#define    SEND_PSV_RELATED_PARA           0xC2000600FE

#define    SEND_BIPAP_RELATED_PARA        (0xC0200800800|SEND_SYSTEM_MODE_FLAG )// (0x40000000000 | 0x80000000000 | 0x00800 | 0x800000 | 0x200000000) // Phigh Plow f Thigh Tslope    
#define    SEND_APRV_RELATED_PARA          (0xC0001800000|SEND_SYSTEM_MODE_FLAG) // (0x40000000000 | 0x80000000000 | 0x800000 | 0x1000000) Phigh Plow Thigh Tlow
#define    SEND_VS_RELATED_PARA            (0x304200001040|SEND_SYSTEM_MODE_FLAG) // VTSupp f Tslope PEEP ESENS PHLmit 
                                           //(0x100000000000 | 0x400000000000 | 0x200000000 | 0x00002 | 0x00004 | 0x00040 | 0x4000000000 | 0x01000 | 0x40000)
#define    SEND_SIGH_RELATED_PARA          0x01020600DF//ylk add 20190128  (SEND_VCV_RELATED_PARA | SEND_SIGH_TIME_FLAG)

#ifndef MANNUL_SWITCH
#define    SEND_MANUAL_RELATED_PARA        0x0020000 //  20130704
#endif
/**************************************************************************************************/
//CO2
#define MODULESWITCH_FLAG           0x01
#define CALZERO_FLAG                0x02
//Masimo IRMA/ISA
//CMD
#define SET_OPERATE              0x00
#define SET_ZEROCAL              0x06
#define SET_SPANCALO2            0x68
#define SET_IRO2DELAY            0x69
#define SET_PUMP                 0x6B
#define SET_O2OPTION             0x6C
#define SET_O2SENSORTYPE         0x6D
/******************************************************************************************************/
//#define  SET_AA_OPERATE_STANDBY     0x11
//#define  SET_AA_OPERATE_MEASURE     0x12
//#define  SET_AA_PUMPON              0x13
//#define  SET_AA_PUMPOFF             0x14
//#define  SET_AA_IRO2DELAY           0x15
//#define  SET_AA_SETO2OPTION         0x16
//#define  SET_AA_SETO2SENSORTYPE     0x17
//#define  SET_AA_ZEROCAL             0x18
//#define  SET_AA_SPANCALO2           0x19
//#define  SET_AA_SPANCAL             0x1A

/**************************************************************************************************/
#define  MODULE_SWITCH_COMMAND              0x000
#define  SEND_MODULE_SWITCH_FLAG            0x001
#define  SEND_CAL_ZERO_FLAG                 0x002
#define  SEND_AA_OPERATE_STANDBY            0x004
#define  SEND_AA_OPERATE_MEASURE            0x008
#define  SEND_AA_PUMPON                     0x010
#define  SEND_AA_PUMPOFF                    0x020
#define  SEND_AA_IRO2DELAY                  0x040
#define  SEND_AA_SETO2OPTION                0x080
#define  SEND_AA_SETO2SENSORTYPE            0x100
#define  SEND_AA_ZEROCAL                    0x200
#define  SEND_AA_SPANCALO2                  0x400
#define  SEND_AA_SPANCAL                    0x800

//#define  SEND_MODULE_SWITCH_FLAG            0x10000
//#define  SEND_CAL_ZERO_FLAG                 0x20000
/**************************************************************************************************/


//触发呼吸类型
#define MACHINE_TRIGGER  0x55
#define PATIENT_SIMV_TRIGGER     0x54
#define PATIENT_SPONT_TRIGGER  0x2A
#define MANUAL_TRIGGER   0xAA
#define EXP_TRIGGER      0xFF

