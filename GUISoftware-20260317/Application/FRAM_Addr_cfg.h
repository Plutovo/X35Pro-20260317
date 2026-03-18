#ifndef _FRAM_ADDR_CFG_H
#define _FRAM_ADDR_CFG_H

#ifdef __cplusplus
    extern "C" {
#endif
/* Includes ----------------------------------------------------------------- */        
#include "FRAM_Addr_Sys_cfg.h"        
#include "FRAM_Addr_old.h"
// #include "MedibusComm.h"
        
//接收串口收到的版本     
typedef struct
{
    u8 BDU_BOOT[4];
    u8 BDU_APP[4];
    u8 EGM_BOOT[4];
    u8 EGM_APP[4];
    u8 PMU_BOOT[4];
    u8 PMU_APP[4];
}BOARD_VERSION;

typedef enum
{
    Board_NULL = 0,
    Board_GUI_BOOT,
    Board_GUI_APP,
    Board_BDU_BOOT,
    Board_BDU_APP,
    Board_EGM_BOOT,
    Board_EGM_APP,
    Board_PMU_BOOT,
    Board_PMU_APP,
}BOARD_VERSION_Type;

typedef enum
{
    MType_Model = 0,
    MType_ScreenModel,
    MType_DeviceType,
}MODEL_Type;     
     
/*******************************************************************************
  *
  *                             FRAM新地址
  *
  * @ 根据自己的需要修改对应的存储地址，不需要的地址置0，不允许删除！！！
  ******************************************************************************/         
        
/*******************************************************************************
  *
  *                             FRAM Page 1 address
  *      
  * @Address range: 0x00000~0x0FFFF
  *
  ******************************************************************************/

/*
 * @Type 系统相关    
 * @Size 1KB
 * @Range 0x00000~0x003FF
 * @{
 */
#define SYSTEM_TOTAL_SIZE                   (1*1024)
     
//第一次烧写系统标志 0x00000~0x00003
#define SYSTEM_INIT_FLAG0_ADDR              0x00000
#define SYSTEM_INIT_FLAG1_ADDR              0x00001
#define SYSTEM_INIT_FLAG2_ADDR              0x00002
#define SYSTEM_INIT_FLAG3_ADDR              0x00003
#define SYSTEM_INIT_FLAG_SIZE               4     

//FRAM使用新地址      0x00004~0x00007
#define SYSTEM_NEWADDR_FLAG0_ADDR           0x00004
#define SYSTEM_NEWADDR_FLAG1_ADDR           0x00005
#define SYSTEM_NEWADDR_FLAG2_ADDR           0x00006
#define SYSTEM_NEWADDR_FLAG3_ADDR           0x00007
#define SYSTEM_NEWADDR_SIZE                 4
     
//机器型号  0x00008~0x00027
#define SYSTEM_MODEL_START_ADDR             0x00008
#define SYSTEM_MODEL_END_ADDR               0x00027
#define SYSTEM_MODEL_SIZE                   32 

//BOOT版本    0x00028~0x0002F
#define SYSTEM_BOOT_MAIN                    0x00028
#define SYSTEM_BOOT_SUB                     0x0002A
#define SYSTEM_BOOT_DEBUG                   0x0002C
#define SYSTEM_BOOT_TEST                    0x0002E  
#define SYSTEM_BOOTVer_SIZE                 8

//GUI版本 0x00030~0x00037
#define SYSTEM_GUI_MAIN                     0x00030
#define SYSTEM_GUI_SUB                      0x00032
#define SYSTEM_GUI_DEBUG                    0x00034
#define SYSTEM_GUI_TEST                     0x00036
#define SYSTEM_GUIVer_SIZE                  8

//升级    0x00038~0x00047
//升级地址不要改变
#define ADDRESSFLAG_ADDR                    0x00040
#define SOFTWARE_UPGRADE_BACKUP_ADDR1       0x00041
#define SOFTWARE_UPGRADE_BACKUP_ADDR2       0x00042
#define ErasureMarkADD1                     0x00043
#define SYSTEM_SOFTWARE_SIZE                16

//触摸屏AD 0x00048~0x0004F
#define TOUCH_AD_LEFT                       0x00048                 
#define TOUCH_AD_RIGHT                      0x0004A 
#define TOUCH_AD_TOP                        0x0004C              
#define TOUCH_AD_BOTTOM                     0x0004E 
#define SYSTEM_TOUCH_AD_SIZE                8
     
//流量传感器 0x00050~0x00057
#define FLOW_SERIESNUM_ADDR                 0x00050
#define FLOW_SERIESNUM_SIZE                 8
     
//软件设备序列号 X45使用 0x00058~0x00061
//设备序列号存储
#define DEVICE_NUM_0_ADDR                   0x00058
#define DEVICE_NUM_1_ADDR                   0x00059
#define DEVICE_NUM_2_ADDR                   0x0005A
#define DEVICE_NUM_3_ADDR                   0x0005B
#define DEVICE_NUM_4_ADDR                   0x0005C
#define DEVICE_NUM_5_ADDR                   0x0005D
#define DEVICE_NUM_6_ADDR                   0x0005E
#define DEVICE_NUM_7_ADDR                   0x0005F
#define DEVICE_NUM_SIZE                     8
     
//序列号存在的标志
#define DEVICE_FLAG_HIGH_ADDR               0x00060
#define DEVICE_FLAG_LOW_ADDR                0x00061
#define DEVICE_FLAG_SIZE                    2
     
//美标/欧标切换 0x00062~0x00063
#define EFM_STANDARD_ADDR                   0x00062
#define EGM_STANDARD_ADDR                   0x00062
#define EGM_STANDARD_SIZE                   2
     
//屏幕型号  0x00064~0x00073
#define SYSTEM_SCREEN_START_ADDR            0x00064
#define SYSTEM_SCREEN_END_ADDR              0x00073
#define SYSTEM_SCREEN_SIZE                  16     

//触摸屏校验字节   0x00074~0x00075
#define TOUCH_CHECKSUM                      0x00074
#define TOUCH_CHECKSUM_SIZE                 2    
        
//当前机器是欧标/国标 欧标:1   国标:2    0x00076~0x00077
#define MACHINE_STANDARD_ADDR               0x00076
#define MACHINE_STANDARD_SIZE               2   
        
//BDU boot版本          0x00078~0x0007F
#define SYSTEM_BDU_BOOT_MAIN                0x00078       
#define SYSTEM_BDU_BOOT_SUB                 0x0007A
#define SYSTEM_BDU_BOOT_DEBUG               0x0007C
#define SYSTEM_BDU_BOOT_TEST                0x0007E
#define SYSTEM_BDU_BOOT_SIZE                8       
        
//BDU 版本            0x00080~0x00087
#define SYSTEM_BDU_MAIN                     0x00080       
#define SYSTEM_BDU_SUB                      0x00082
#define SYSTEM_BDU_DEBUG                    0x00084
#define SYSTEM_BDU_TEST                     0x00086
#define SYSTEM_BDU_SIZE                     8   
        
//EGM boot版本        0x00088~0x0008F
#define SYSTEM_EGM_BOOT_MAIN                0x00088       
#define SYSTEM_EGM_BOOT_SUB                 0x0008A
#define SYSTEM_EGM_BOOT_DEBUG               0x0008C
#define SYSTEM_EGM_BOOT_TEST                0x0008E
#define SYSTEM_EGM_BOOT_SIZE                8       
        
//EGM 版本            0x00090~0x00097
#define SYSTEM_EGM_MAIN                     0x00090       
#define SYSTEM_EGM_SUB                      0x00092
#define SYSTEM_EGM_DEBUG                    0x00094
#define SYSTEM_EGM_TEST                     0x00096
#define SYSTEM_EGM_SIZE                     8     
            
//PMU boot版本        0x00098~0x0009F
#define SYSTEM_PMU_BOOT_MAIN                0x00098       
#define SYSTEM_PMU_BOOT_SUB                 0x0009A
#define SYSTEM_PMU_BOOT_DEBUG               0x0009C
#define SYSTEM_PMU_BOOT_TEST                0x0009E
#define SYSTEM_PMU_BOOT_SIZE                8       
        
//PMU 版本            0x000A0~0x000A7
#define SYSTEM_PMU_MAIN                     0x000A0       
#define SYSTEM_PMU_SUB                      0x000A2
#define SYSTEM_PMU_DEBUG                    0x000A4
#define SYSTEM_PMU_TEST                     0x000A6
#define SYSTEM_PMU_SIZE                     8    

//机器类型          0x000A8~0x000B7
#define SYSTEM_MACHINE_TYPE                 0x000A8
#define SYSTEM_MACHINE_TYPE_SIZE            16        
        
//reserve           0x000B8~0x003FF
#define SYSTEM_RESERVE_START_ADDR           0x000B8
#define SYSTEM_RESERVE_END_ADDR             0x003FF

/**
  * @}
  */ 

     
/*
 * @Type 相关设置    
 * @Size 3KB
 * @Range 0x00400~0x00FFF
 * @{
 */     
 #define SETUP_TOTAL_SIZE                   (3*1024)
        
//软件授权码 0x00400~0x0040F
#define OPTIONAL_0_ADDR                     0x00400
#define OPTIONAL_1_ADDR                     0x00401
#define OPTIONAL_2_ADDR                     0x00402
#define OPTIONAL_3_ADDR                     0x00403
#define OPTIONAL_4_ADDR                     0x00404
#define OPTIONAL_5_ADDR                     0x00405
#define OPTIONAL_6_ADDR                     0x00406
#define OPTIONAL_7_ADDR                     0x00407
#define OPTIONAL_8_ADDR                     0x00408
#define OPTIONAL_9_ADDR                     0x00409
#define OPTIONAL_A_ADDR                     0x0040A
#define OPTIONAL_B_ADDR                     0x0040B
#define OPTIONAL_C_ADDR                     0x0040C
#define OPTIONAL_D_ADDR                     0x0040D
#define OPTIONAL_E_ADDR                     0x0040E
#define OPTIONAL_F_ADDR                     0x0040F

//语言标志 0x00410~0x00411
#define LANGUAGE_TYPE_ADDR                  0x00410
//音量    0x00412~0x00413
#define ALARM_VOICE_VOLUME_ADDR             0x00412
//运行时间  0x00414~0x00415
#define RUN_TOTAL_TIME_ADDR                 0x00414
//时间格式
#define TIMEFORMAT_ADDR                     0x00416
//大气压力开关和数值 0x00417~0x00419
#define BARO_SWITCH_ADDR                    0x00417
#define BARO_DATA_HIGH_ADDR                 0x00418
#define BARO_DATA_LOW_ADDR                  0x00419
//笑气开关  0x0041A
#define N2O_SWITCH_ADDR                     0x0041A
//驱动气体
#define DIRVER_GAS_ADDR                     0x0041B
//笑气气源开关 空气气源开关 0x0041C~0x0041D
#define AIR_SUPPLY_SWITCH_ADDR              0x0041C
#define N2O_SUPPLY_SWITCH_ADDR              0x0041D
//模块开关   0x0041E~0x00420
#define O2_SWITCH_ADDR                      0x0041E
#define ETCO2_SWITCH_ADDR                   0x0041F
#define SPO2_SWITCH_ADDR                    0x00420

//Masimo SpO2   0x00421~0x00425
#define MASIMO_BEEPER_ADDR                  0x00421 
#define MASIMO_FAST_SAT_ADDR                0x00422
#define MASIMO_SENSITIVITY_ADDR             0x00423
#define MASIMO_SMART_TONE_ADDR              0x00424 
#define MASIMO_AVERAGE_TIME_ADDR            0x00425
//Unit 0x00426~0x0042B
#define UNIT_BAROMETIRC_ADDR                0x00426
#define UNIT_CO2_ADDR                       0x00427
#define UNIT_HEIGHT_ADDR                    0x00428
#define UNIT_AIRWAY_ADDR                    0x00429
#define UNIT_GAS_ADDR                       0x0042A
#define UNIT_WEIGHT_ADDR                    0x0042B
//Ti/I:E
#define TI_IE_CHOOSE_FLAG_ADDR              0x0042C
#define Medibus_COMM_ADDR			        0x0042D

//运行时间 MIN  0x0042E
#define RUN_TOTAL_TIME_MIN_ADDR           	0x0042E  //zkq add 20191009

//SetUp reserve1 0x0042E~0x0042F
//#define SetUp_RESERVE1_START_ADDR           0x0042E
//#define SetUp_RESERVE1_END_ADDR             0x0042F

#define WAVEFORMSID2_ADDR                   0x0042F  //第三波形保存模块波形选择


//病人设置 0x00430~0x00438
#define PATIENT_INFO_TYPE_ADDR              0x00430
#define PATIENT_INFO_GENDER_ADDR            0x00431
#define PATIENT_INFO_VENTTYPE_ADDR          0x00432
#define PATIENT_INFO_IBW_LOW_ADDR           0x00433
#define PATIENT_INFO_IBW_HIGH_ADDR          0x00434
#define PATIENT_INFO_STATURE_ADDR           0x00435
#define PATIENT_INFO_AGE_ADDR               0x00436 
#define PATIENT_INFO_IDEAVT_HIGH_ADDR       0x00437
#define PATIENT_INFO_IDEAVT_LOW_ADDR        0x00438

//SetUp reserve2 0x00439~0x0044F
#define SetUp_RESERVE2_START_ADDR           0x00439
#define SetUp_RESERVE2_END_ADDR             0x0044F

//参数设置  0x00450~0x00466
//vt
#define BREATH_VT_HIGH_ADDR                 0x00450
#define BREATH_VT_LOW_ADDR                  0x00451
//Ti
#define BREATH_Ti_HIGH_ADDR                 0x00452
#define BREATH_Ti_LOW_ADDR                  0x00453
//Pre
#define BREATH_PRE_BREATHMODE_ADDR          0x00454
#define BREATH_PRE_APNOEA_ADDR              0x00455
//freq
#define BREATH_RATE_ADDR                    0x00456
#define BREATH_RATE_SIMV_ADDR               0x00457
#define BREATH_RATE_OTHER_ADDR              0x00458
//Tp
#define BREATH_TP_ADDR                      0x00459
//Psupport 
#define BREATH_PS_ADDR                      0x0045A
//Pinsp
#define BREATH_PINSP_ADDR                   0x0045B
//Ptrigger
#define BREATH_PTR_ADDR                     0x0045C
//Ftrigger
#define BREATH_FTR_ADDR                     0x0045D
//Peep
#define BREATH_PEEP_ADDR                    0x0045E
//TreggerMode
#define BREATH_TRIGGERMODE_ADDR             0x0045F
//BreathMode
#define BREATH_BREATHMODE_ADDR              0x00460
//ApnoeaMode
#define BREATH_APNOEA_ADDR                  0x00461
//SighTimes
#define BREATH_SIGHTIMES_ADDR               0x00462
//Tslope
#define BREATH_TSLOPE_ADDR                  0x00463
//Esens
#define BREATH_ESENS_ADDR                   0x00464
//FlowPatten
#define BREATH_FLOWPATTEN_ADDR              0x00465
//I:E
#define BREATH_IE_ADDR                      0x00466
//VtSupport
#define BREATH_VTS_HIGH_ADDR				0x00467
#define BREATH_VTS_LOW_ADDR					0x00468

//SetUp reserve3 0x00467~0x0047F
#define SetUp_RESERVE3_START_ADDR           0x00469
#define SetUp_RESERVE3_END_ADDR             0x0047F

//报警限制 0x00480~0x0049F
#define PH_HIGH_ADDR                        0x00480
#define PH_LOW_ADDR                         0x00481
#define MV_HIGH_ADDR                        0x00482
#define MV_LOW_ADDR                         0x00483
#define RATE_HIGH_ADDR                      0x00484
#define RATE_LOW_ADDR                       0x00485
#define FIO2_HIGH_ADDR                      0x00486
#define FIO2_LOW_ADDR                       0x00487
#define VTE_HIGH_ADDR_H                     0x00488
#define VTE_HIGH_ADDR_L                     0x00489
#define VTE_LOW_ADDR_H                      0x0048A
#define VTE_LOW_ADDR_L                      0x0048B
#define APNEA_TIME_ADDR                     0x0048C
//第二页 
#define ETCO2_HIGH_ADDR                     0x0048D
#define ETCO2_LOW_ADDR                      0x0048E
#define FICO2_HIGH_ADDR                     0x0048F
#define FIN2O_HIGH_ADDR                     0x00490
#define FIN2O_LOW_ADDR                      0x00491
#define ETAA_HIGH_ADDR                      0x00492
#define ETAA_LOW_ADDR                       0x00493
#define FIAA_HIGH_ADDR                      0x00494
#define FIAA_LOW_ADDR                       0x00495
#define FG_HIGH_ADDR                        0x00496
#define FG_LOW_ADDR                         0x00497
//第三页
#define PLUS_HIGH_ADDR                      0x00498
#define PLUS_LOW_ADDR                       0x00499
#define SPO2_HIGH_ADDR                      0x0049A
#define SPO2_LOW_ADDR                       0x0049B
#define PI_HIGH_ADDR_H                      0x0049C
#define PI_HIGH_ADDR_L                      0x0049D
#define PI_LOW_ADDR_H                       0x0049E
#define PI_LOW_ADDR_L                       0x0049F

//选配备份地址 0x004A0~0x004AF
#define OPTIONAL_0_ADDR_BACK                0x004A0
#define OPTIONAL_1_ADDR_BACK                0x004A1
#define OPTIONAL_2_ADDR_BACK                0x004A2
#define OPTIONAL_3_ADDR_BACK                0x004A3
#define OPTIONAL_4_ADDR_BACK                0x004A4
#define OPTIONAL_5_ADDR_BACK                0x004A5
#define OPTIONAL_6_ADDR_BACK                0x004A6
#define OPTIONAL_7_ADDR_BACK                0x004A7
#define OPTIONAL_8_ADDR_BACK                0x004A8
#define OPTIONAL_9_ADDR_BACK                0x004A9
#define OPTIONAL_A_ADDR_BACK                0x004AA
#define OPTIONAL_B_ADDR_BACK                0x004AB
#define OPTIONAL_C_ADDR_BACK                0x004AC
#define OPTIONAL_D_ADDR_BACK                0x004AD
#define OPTIONAL_E_ADDR_BACK                0x004AE
#define OPTIONAL_F_ADDR_BACK                0x004AF
        
#define ETCO2_Kpa_HIGH_ADDR                 0x004B0
#define ETCO2_Kpa_LOW_ADDR                  0x004B1
#define FICO2_Kpa_HIGH_ADDR                 0x004B2
#define FICO2_Kpa_LOW_ADDR                  0x004B3

//病历号：Patient Identifier
#define PATIENT_INFO_ID_ADDR				0x004B4
#define PATIENT_INFO_ID_SIZE				38

//病人姓名：Patient Name
#define PATIENT_INFO_Name_ADDR				0x004DA
#define PATIENT_INFO_Name_SIZE				38

//SetUp reserve4 0x004B4~0x00FEF
#define SetUp_RESERVE4_START_ADDR           0x00500
#define SetUp_RESERVE4_END_ADDR             0x00FFF
             
/**
  * @}
  */ 

     
/*
 * @Type Monitor/result    
 * @Size 1KB
 * @Range 0x0CC00~0x0CFFF
 * @{
 */   
        
//吸气保持CST测量值及时间 0x01000~0x01007
#define INSPHOLD_CST_LOW_ADDR               0x01000
#define INSPHOLD_CST_HIGH_ADDR              0x01001
#define INSPHOLD_YEAR_HIGH_ADDR             0x01002
#define INSPHOLD_YEAR_LOW_ADDR              0x01003
#define INSPHOLD_MONTH_ADDR                 0x01004
#define INSPHOLD_DAY_ADDR                   0x01005
#define INSPHOLD_HOUR_ADDR                  0x01006
#define INSPHOLD_MIN_ADDR                   0x01007
//呼气保持PEEPI测量值及时间 0x01008~0x0100F
#define EXPHOLD_PEEPI_LOW_ADDR              0x01008
#define EXPHOLD_PEEPI_HIGH_ADDR             0x01009
#define EXPHOLD_YEAR_HIGH_ADDR              0x0100A
#define EXPHOLD_YEAR_LOW_ADDR               0x0100B
#define EXPHOLD_MONTH_ADDR                  0x0100C
#define EXPHOLD_DAY_ADDR                    0x0100D
#define EXPHOLD_HOUR_ADDR                   0x0100E
#define EXPHOLD_MIN_ADDR                    0x0100F

//自检起始地址
#define MEM_OF_CHECKRESULT_ADDR             0x01010//存放自检结果的起始地址，时间2个 8Byte 
//手动自检结果时间 0x01010~0x01017
//在MEM_OF_CHECKRESULT_ADDR地址地址之上的偏移地址
#define MEM_MANUAL_YEAR_OFFSET_ADDR         0x00
#define MEM_MANUAL_MONTH_OFFSET_ADDR        0x01
#define MEM_MANUAL_DAY_OFFSET_ADDR          0x02
#define MEM_MANUAL_HOUR_OFFSET_ADDR         0x03
#define MEM_MANUAL_MIN_OFFSET_ADDR          0x04

//自动自检结果时间  0x01018~0x0101F
#define MEM_AUTOMATIC_YEAR_OFFSET_ADDR      0x08
#define MEM_AUTOMATIC_MONTH_OFFSET_ADDR     0x09
#define MEM_AUTOMATIC_DAY_OFFSET_ADDR       0x0A
#define MEM_AUTOMATIC_HOUR_OFFSET_ADDR      0x0B
#define MEM_AUTOMATIC_MIN_OFFSET_ADDR       0x0C

//自检结果  0x01020~0x0109F
//总的检测状态2个 共2Byte
//检测结果13个 共26Byte    检测结果格式
//                         (最高2位）存放图标类型   第3、4高位存储结果的类型  （低12位）存放数据或者状态
#define MEM_MANUAL_OFFSET_ADDR              0x10
#define MEM_BAROMETER_OFFSET_ADDR           0x13
#define MEM_GASO2_OFFSET_ADDR               0x16
#define MEM_GASN2O_OFFSET_ADDR              0x19
#define MEM_GASAIR_OFFSET_ADDR              0x1C
#define MEM_FLOW_OFFSET_ADDR                0x1F
#define MEM_PRESSURE_OFFSET_ADDR            0x21
#define MEM_FG_FLOW_OFFSET_ADDR             0x24
#define MEM_COMPLIANCE_OFFSET_ADDR          0x27
#define MEM_INTERNAL_LEAKAGE_OFFSET_ADDR    0x2A
#define MEM_OXYGEN_OFFSET_ADDR              0x2D
#define MEM_BATTERY_OFFSET_ADDR             0x30
#define MEM_LEFT_MANUAL_OFFSET_ADDR         0x33
#define MEM_AUTOMATIC_OFFSET_ADDR           0x36

//SelfTest reserve 0x010A0~0x01EFF
#define SelfTest_RESERVE_START_ADDR         0x010A0
#define SelfTest_RESERVE_END_ADDR           0x01EFF
/**
  * @}
  */ 

     
/*
 * @Type not allowed to use
 * @Size 255Byte
 * @Range 0x01F00~0x01FFF
 * @Note users are not allowed to use the block
 * @{
 */             
#define SOFTWARE_UPGRADE_ADDR1              0x01FFA
#define SOFTWARE_UPGRADE_ADDR2              0x01FFB     
#define ErasureMarkADD2                     0x01FFC
/**
 * @}
 */ 

     
/*
 * @Type 趋势    
 * @Size 36KB
 * @Range 0x02000~0x0AFFF
 * @{
 */     
 #define TREND_TOTAL_SIZE                   (36*1024)
        
//趋势数据大小 0x02000~0x09FFF 32K
#define TREND_EEPROMADDR                    0x02000

//时间范围及设置   0x0A000~0x0A004
#define TREND_TIMERANGE                     0x0A000
#define TREND_GRAPH_PARA1                   0x0A002
#define TREND_GRAPH_PARA2                   0x0A003
#define TREND_GRAPH_PARA3                   0x0A004

//最新时间点 0x0A005~0x0A00C
#define TREND_TIME                          0x0A005
//每一个时间段的头位置        0x0A00D~0x0A10C
#define TREND_SAVE_INDEX                    0x0A00D
//趋势表选项 0x0A10D~0x0A114
#define TREND_TABLE_PARA1                   0x0A10D
#define TREND_TABLE_PARA2                   0x0A10E
#define TREND_TABLE_PARA3                   0x0A10F
#define TREND_TABLE_PARA4                   0x0A110
#define TREND_TABLE_PARA5                   0x0A111
#define TREND_TABLE_PARA6                   0x0A112
#define TREND_TABLE_PARA7                   0x0A113

//Trend reserve 0x0A115~0x0AFFF
#define Trend_RESERVE_START_ADDR            0x0A115
#define Trend_RESERVE_END_ADDR              0x0AFFF

/**
  * @}
  */ 

/*
 * @Type 报警    
 * @Size 11KB
 * @Range 0x0B000~0x0DFFF
 * @{
 */      
 #define ALARM_TOTAL_SIZE                   (12*1024)
        
//报警记录地址 0x0B000~0x0D7FF 10K
#define ALARM_RECORD_ADDR                   0x0B000

//报警记录链表地址 0x0D800~0x0D803
#define ALARM_RECORD_LIST_ADDR              0x0D800
//报警记录链表地址(backup) 0x0D804~0x0D807
#define ALARM_RECORD_LIST_BACKUP_ADDR       0x0D804

//报警记录链表地址CRC校验 32位CRC 0x0D808~0x0D80B
#define ALARM_RECORD_LIST_CRC_ADDR          0x0D808
//报警记录链表地址CRC校验(backup) 32位CRC 0x0D80C~0x0D80F
#define ALARM_RECORD_LIST_CRC_BACKUP_ADDR   0x0D80C

//ALARM reserve 0x0D810~0x0DBFF
#define ALARM_RESERVE_START_ADDR            0x0D810
#define ALARM_RESERVE_END_ADDR              0x0DBFF
     
/**
  * @}
  */ 

     
/*
 * @Type 未使用空间  
 * @Size 12KB
 * @Range 0x0D000~0x0FFFF 
 * @{
 */   
#define PAGE1_RESERVE_TOTAL_SIZE            (8*1024)
#define FRAM_PAGE1_RESERVE_START_ADDR       0x0E000
#define FRAM_PAGE1_RESERVE_END_ADDR         0x0FFFF
/**
  * @}
  */ 


/*******************************************************************************
  *
  *                             FRAM Page 2 address
  *      
  * @Address range: 0x10000~0x1FFFF
  *
  ******************************************************************************/

/*
 * @Type 系统相关(备份)  
 * @Size 1KB
 * @Range 0x10000~0x103FF
 * @{
 */      
#define SYSTEM_TOTAL_SIZE_BACKUP           (1*1024)
        
//第一次烧写系统标志 0x10000~0x10003
#define SYSTEM_INIT_FLAG0_ADDR_BACKUP       0x10000
#define SYSTEM_INIT_FLAG1_ADDR_BACKUP       0x10001
#define SYSTEM_INIT_FLAG2_ADDR_BACKUP       0x10002
#define SYSTEM_INIT_FLAG3_ADDR_BACKUP       0x10003

//FRAM使用新地址      0x10004~0x10007
#define SYSTEM_NEWADDR_FLAG0_ADDR_BACKUP    0x10004
#define SYSTEM_NEWADDR_FLAG1_ADDR_BACKUP    0x10005
#define SYSTEM_NEWADDR_FLAG2_ADDR_BACKUP    0x10006
#define SYSTEM_NEWADDR_FLAG3_ADDR_BACKUP    0x10007
     
//机器型号  0x10008~0x10027
#define SYSTEM_MODEL_START_ADDR_BACKUP      0x10008
#define SYSTEM_MODEL_END_ADDR_BACKUP        0x10027

//BOOT版本   0x10028~0x1002F
#define SYSTEM_BOOT_MAIN_BACKUP             0x10028
#define SYSTEM_BOOT_SUB_BACKUP              0x1002A
#define SYSTEM_BOOT_DEBUG_BACKUP            0x1002C
#define SYSTEM_BOOT_TEST_BACKUP             0x1002E   

//GUI版本 0x10030~0x10037
#define SYSTEM_GUI_MAIN_BACKUP              0x10030
#define SYSTEM_GUI_SUB_BACKUP               0x10032
#define SYSTEM_GUI_DEBUG_BACKUP             0x10034
#define SYSTEM_GUI_TEST_BACKUP              0x10036

//升级    0x10038~0x10047
#define ADDRESSFLAG_ADDR_BACKUP             0x10038
#define SOFTWARE_UPGRADE_ADDR1_BACKUP       0x10039
#define SOFTWARE_UPGRADE_ADDR2_BACKUP       0x1003A
#define ErasureMarkADD1_BACKUP              0x1003B
#define SOFTWARE_UPGRADE_BACKUP_ADDR1_BACKUP     0x1003C
#define SOFTWARE_UPGRADE_BACKUP_ADDR2_BACKUP     0x1003D
#define ErasureMarkADD2_BACKUP              0x1003E

//触摸屏AD 0x10048~0x1004F
#define TOUCH_AD_LEFT_BACKUP                0x10048                 
#define TOUCH_AD_RIGHT_BACKUP               0x1004A 
#define TOUCH_AD_TOP_BACKUP                 0x1004C              
#define TOUCH_AD_BOTTOM_BACKUP              0x1004E 

//流量传感器 0x10050~0x10057
#define FLOW_SERIESNUM_ADDR_BACKUP          0x10050

//软件设备序列号 X45使用 0x10058~0x10061
//设备序列号存储
#define DEVICE_NUM_0_ADDR_BACKUP            0x10058
#define DEVICE_NUM_1_ADDR_BACKUP            0x10059
#define DEVICE_NUM_2_ADDR_BACKUP            0x1005A
#define DEVICE_NUM_3_ADDR_BACKUP            0x1005B
#define DEVICE_NUM_4_ADDR_BACKUP            0x1005C
#define DEVICE_NUM_5_ADDR_BACKUP            0x1005D
#define DEVICE_NUM_6_ADDR_BACKUP            0x1005E
#define DEVICE_NUM_7_ADDR_BACKUP            0x1005F
//序列号存在的标志
#define DEVICE_FLAG_HIGH_ADDR_BACKUP        0x10060
#define DEVICE_FLAG_LOW_ADDR_BACKUP         0x10061

//美标/欧标切换 0x10062~0x10063
#define EGM_STANDARD_ADDR_BACKUP            0x10062
        
//屏幕型号  0x10064~0x10073
#define SYSTEM_SCREEN_START_ADDR_BACKUP     0x10064
#define SYSTEM_SCREEN_END_ADDR_BACKUP       0x10073

//触摸屏校验字节
#define TOUCH_CHECKSUM_BACKUP               0x10074
        
//当前机器是欧标/国标 欧标:1   国标:2        
#define MACHINE_STANDARD_ADDR_BACKUP        0x10076
        
//BDU boot版本        
#define SYSTEM_BDU_BOOT_MAIN_BACKUP         0x10078       
#define SYSTEM_BDU_BOOT_SUB_BACKUP          0x1007A
#define SYSTEM_BDU_BOOT_DEBUG_BACKUP        0x1007C
#define SYSTEM_BDU_BOOT_TEST_BACKUP         0x1007E     
        
//BDU 版本       
#define SYSTEM_BDU_MAIN_BACKUP              0x10080
#define SYSTEM_BDU_SUB_BACKUP               0x10082
#define SYSTEM_BDU_DEBUG_BACKUP             0x10084
#define SYSTEM_BDU_TEST_BACKUP              0x10086  
        
//EGM boot版本        
#define SYSTEM_EGM_BOOT_MAIN_BACKUP         0x10088       
#define SYSTEM_EGM_BOOT_SUB_BACKUP          0x1008A
#define SYSTEM_EGM_BOOT_DEBUG_BACKUP        0x1008C
#define SYSTEM_EGM_BOOT_TEST_BACKUP         0x1008E    
        
//EGM 版本       
#define SYSTEM_EGM_MAIN_BACKUP              0x10090       
#define SYSTEM_EGM_SUB_BACKUP               0x10092
#define SYSTEM_EGM_DEBUG_BACKUP             0x10094
#define SYSTEM_EGM_TEST_BACKUP              0x10096   
        
//PMU boot版本        
#define SYSTEM_PMU_BOOT_MAIN_BACKUP         0x10098       
#define SYSTEM_PMU_BOOT_SUB_BACKUP          0x1009A
#define SYSTEM_PMU_BOOT_DEBUG_BACKUP        0x1009C
#define SYSTEM_PMU_BOOT_TEST_BACKUP         0x1009E      
        
//PMU 版本       
#define SYSTEM_PMU_MAIN_BACKUP              0x100A0       
#define SYSTEM_PMU_SUB_BACKUP               0x100A2
#define SYSTEM_PMU_DEBUG_BACKUP             0x100A4
#define SYSTEM_PMU_TEST_BACKUP              0x100A6 

//机器类型
#define SYSTEM_MACHINE_TYPE_BACKUP          0x100A8   
        
//reserve 0x100B8~0x103FF
#define SYSTEM_RESERVE_START_ADDR_BACKUP    0x100B8
#define SYSTEM_RESERVE_END_ADDR_BACKUP      0x103FF


/**
  * @}
  */ 


/*
 * @Type 校验数据相关
 * @Size 12KB
 * @Range 0x10400~0x133FF 
 * @{
 */     
#define CAIL_TOTAL_SIZE                     (12*1024) 
#define CAIL_BDU_SIZE                       (2*1024)
#define CAIL_EFMEGM_SIZE                    (2*1024)

//BDU校验数据                 0x10400~0x10BFF   2K
#define CALI_BDU_START_ADDR                 0x10400 
//EFM/EGM校验数据             0x10C00~0x113FF   2K
#define CALI_EFMEGM_START_ADDR              0x10C00 
//BDU校验数据(备份)           0x11400~0x11BFF   2K
#define CALI_BDU_START_ADDR_BACKUP          0x11400 
//EFM/EGM校验数据(备份)       0x11C00~0x123FF   2K
#define CALI_EFMEGM_START_ADDR_BACKUP       0x11C00 
//BDU CRC校验                 0x12400~0x12403    4
#define CALI_BDU_CRC_FLAG0_ADDR             0x12400 
#define CALI_BDU_CRC_FLAG1_ADDR             0x12401 
#define CALI_BDU_CRC_FLAG2_ADDR             0x12402 
#define CALI_BDU_CRC_FLAG3_ADDR             0x12403 
//EFM/EGM CRC校验             0x12404~0x12407    4
#define CALI_EFMEGM_CRC_FLAG0_ADDR          0x12404 
#define CALI_EFMEGM_CRC_FLAG1_ADDR          0x12405 
#define CALI_EFMEGM_CRC_FLAG2_ADDR          0x12406 
#define CALI_EFMEGM_CRC_FLAG3_ADDR          0x12407 
//BDU CRC校验(备份)           0x12408~0x1240B    4
#define CALI_BDU_CRC_FLAG0_ADDR_BACKUP      0x12408 
#define CALI_BDU_CRC_FLAG1_ADDR_BACKUP      0x12409 
#define CALI_BDU_CRC_FLAG2_ADDR_BACKUP      0x1240A 
#define CALI_BDU_CRC_FLAG3_ADDR_BACKUP      0x1240B 
//EFM/EGM CRC校验(备份)       0x1240C~0x1240F    4
#define CALI_EFMEGM_CRC_FLAG0_ADDR_BACKUP   0x1240C 
#define CALI_EFMEGM_CRC_FLAG1_ADDR_BACKUP   0x1240D 
#define CALI_EFMEGM_CRC_FLAG2_ADDR_BACKUP   0x1240E 
#define CALI_EFMEGM_CRC_FLAG3_ADDR_BACKUP   0x1240F 

//reserve 0x12410~0x133FF
#define CALI_RESERVE_START_ADDR             0x12410
#define CALI_RESERVE_END_ADDR               0x133FF
/**
  * @}
  */ 

     
/*
 * @Type 日志
 * @Size 15KB
 * @Range 0x13400~0x16FFF 
 * @{
 */ 
#define LOG_TOTAL_SIZE                      (15*1024)
#define DebugLog_Addr			            0x13400
     
/**
  * @}
  */ 

     
/*
 * @Type 未使用空间
 * @Size 36KB
 * @Range 0x17000~0x1FFFF 
 * @{
 */ 
#define PAGE2_RESERVE_TOTAL_SIZE            (36*1024)    
#define FRAM_PAGE2_RESERVE_START_ADDR       0x17000
#define FRAM_PAGE2_RESERVE_END_ADDR         0x1FFFF
/**
  * @}
  */ 

//X40 X45额外定义
#define O2_ON_OFF_ADDR                      O2_SWITCH_ADDR 
#define ETCO2_ON_OFF_ADDR                   ETCO2_SWITCH_ADDR
#define SPO2_ON_OFF_ADDR                    SPO2_SWITCH_ADDR
#define CO2UNIT_ADDR                        UNIT_CO2_ADDR
#define ETCO2_UNIT_ADDR                     UNIT_CO2_ADDR
#define N2O_ON_OFF_ADDR                     N2O_SWITCH_ADDR//ylk add 20181123

#ifdef __cplusplus
}
#endif

#endif

