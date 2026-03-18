#ifndef _FRAM_ADDR_OLD_H
#define _FRAM_ADDR_OLD_H

/*******************************************************************************
  *
  *                             需要搬移的数据的地址
  *
  * @ 1、排列顺序请按照EXCEL表来，并且需要搬移的需要加上 "_Old"
  * @ 2、根据自己的需要增加定义，并且修改对应的存储地址，不需要的地址置0，不允许删除！！！
  * @ 3、只搬移用户设置类数据，趋势、报警、结果/检测、日志类数据不需要搬移
  ******************************************************************************/     

//第一次烧写系统标志
#define SYSTEM_INIT_FLAG_Old                    0xDAAC
#define SYSTEM_INIT_HIGH_ADDR_Old               0x00
#define SYSTEM_INIT_LOW_ADDR_Old                0x00

//触摸屏AD值
#define TOUCH_AD_LEFT_Old                       0x5A0
#define TOUCH_AD_RIGHT_Old                      0x5A2
#define TOUCH_AD_TOP_Old                        0x5A4
#define TOUCH_AD_BOTTOM_Old                     0x5A6
#define TOUCH_CHECKSUM_Old                      0x5A8      

//流量传感器编码
#define FLOW_SERIESNUM_ADDR_Old                 0x490

//软件设备序列号(X45)
#define DEVICE_NUM_0_ADDR_Old                   0x508
#define DEVICE_NUM_1_ADDR_Old                   0x509
#define DEVICE_NUM_2_ADDR_Old                   0x50A
#define DEVICE_NUM_3_ADDR_Old                   0x50B
#define DEVICE_NUM_4_ADDR_Old                   0x50C
#define DEVICE_NUM_5_ADDR_Old                   0x50D
#define DEVICE_NUM_6_ADDR_Old                   0x50E
#define DEVICE_NUM_7_ADDR_Old                   0x50F

//设备序列号存在标志
#define DEVICE_FLAG_HIGH_ADDR_Old               0x510
#define DEVICE_FLAG_LOW_ADDR_Old                0x511

//美标/国标
#define EGM_STANDARD_ADDR_Old                   0x5F0
      
//
//选配
#define OPTIONAL_0_ADDR_Old                     0x500
#define OPTIONAL_1_ADDR_Old                     0x501
#define OPTIONAL_2_ADDR_Old                     0x502
#define OPTIONAL_3_ADDR_Old                     0x503
#define OPTIONAL_4_ADDR_Old                     0x504
#define OPTIONAL_5_ADDR_Old                     0x505
#define OPTIONAL_6_ADDR_Old                     0x506
#define OPTIONAL_7_ADDR_Old                     0x507
#define OPTIONAL_8_ADDR_Old                     0x508
#define OPTIONAL_9_ADDR_Old                     0x509
#define OPTIONAL_A_ADDR_Old                     0x50A
#define OPTIONAL_B_ADDR_Old                     0x50B
#define OPTIONAL_C_ADDR_Old                     0x50C
#define OPTIONAL_D_ADDR_Old                     0x50D
#define OPTIONAL_E_ADDR_Old                     0x50E
#define OPTIONAL_F_ADDR_Old                     0x50F

//语言
#define LANGUAGE_TYPE_ADDR_Old                  0x60
//音量
#define ALARM_VOICE_VOLUME_ADDR_Old             0x80
//运行时间
#define RUN_TOTAL_TIME_ADDR_Old                 0x88
//时间格式
#define TIMEFORMAT_ADDR_Old                     0x4A8
//大气压力开关
#define BARO_SWITCH_ADDR_Old                    0x482
//大气压力数值
#define BARO_DATA_HIGH_ADDR_Old                 0x483
#define BARO_DATA_LOW_ADDR_Old                  0x484
//笑气开关
#define N2O_SWITCH_ADDR_Old                     0x489
//驱动气体
#define DIRVER_GAS_ADDR_Old                     0x86
//笑气气源开关
#define N2O_SUPPLY_SWITCH_ADDR_Old              0x4C9
//空气气源开关
#define AIR_SUPPLY_SWITCH_ADDR_Old              0x85
//氧气
#define O2_ON_OFF_ADDR_Old                      0x84
#define O2_SWITCH_ADDR_Old                      O2_ON_OFF_ADDR_Old
//AA/CO2
#define ETCO2_ON_OFF_ADDR_Old                   0x82      
#define ETCO2_SWITCH_ADDR_Old                   ETCO2_ON_OFF_ADDR_Old
//SPO2
#define SPO2_ON_OFF_ADDR_Old                    0x81
#define SPO2_SWITCH_ADDR_Old                    SPO2_ON_OFF_ADDR_Old
//MASIMO BEEPER
#define MASIMO_BEEPER_ADDR_Old                  0x5D0 
//MASIMO FAST SAT
#define MASIMO_FAST_SAT_ADDR_Old                0x5D1
//MASIMO SENSITIVITY
#define MASIMO_SENSITIVITY_ADDR_Old             0x5D2
//MASIMO SMART TONE
#define MASIMO_SMART_TONE_ADDR_Old              0x5D3 
//MASIMO AVERAGE TIME
#define MASIMO_AVERAGE_TIME_ADDR_Old            0x5D4
//大气压力单位
#define UNIT_BAROMETIRC_ADDR_Old                0x5D5
//CO2单位
#define ETCO2_UNIT_ADDR_Old                     0x83
#define UNIT_CO2_ADDR_Old                       ETCO2_UNIT_ADDR_Old

//身高单位
#define UNIT_HEIGHT_ADDR_Old                    0x5D7
//气道压力单位
#define UNIT_AIRWAY_ADDR_Old                    0x5D8
//气源单位
#define UNIT_GAS_ADDR_Old                       0x5D9
//体重单位
#define UNIT_WEIGHT_ADDR_Old                    0x5DA
//Ti/IE
#define TI_IE_CHOOSE_FLAG_ADDR_Old              0x44
//Medibus开关
#define Medibus_COMM_ADDR_Old			        0x1FFF
//保留 2字节

//病人类型
#define PATIENT_INFO_TYPE_ADDR_Old              0x4A9
//病人性别
#define PATIENT_INFO_GENDER_ADDR_Old            0x4B0
//病人通气类型
#define PATIENT_INFO_VENTTYPE_ADDR_Old          0x4B1
//病人IBW
#define PATIENT_INFO_IBW_LOW_ADDR_Old           0x4B2
#define PATIENT_INFO_IBW_HIGH_ADDR_Old          0x4B3
//病人身高
#define PATIENT_INFO_STATURE_ADDR_Old           0x4B4
//病人年龄
#define PATIENT_INFO_AGE_ADDR_Old               0x4C1
//病人理想潮气量
#define PATIENT_INFO_IDEAVT_HIGH_ADDR_Old       0x4C3
#define PATIENT_INFO_IDEAVT_LOW_ADDR_Old        0x4C4
//保留    //21字节

//VT
#define BREATH_VT_HIGH_ADDR_Old                 0x5A0
#define BREATH_VT_LOW_ADDR_Old                  0x5A1
//Ti
#define BREATH_Ti_HIGH_ADDR_Old                 0x5A2
#define BREATH_Ti_LOW_ADDR_Old                  0x5A3
//上一个使用的呼吸模式
#define BREATH_PRE_BREATHMODE_ADDR_Old          0x5A4
//上一个使用的窒息模式
#define BREATH_PRE_APNOEA_ADDR_Old              0x5A5
//Rate
#define BREATH_RATE_ADDR_Old                    0x5A6
//SIMV Rate
#define BREATH_RATE_SIMV_ADDR_Old               0x5A7
//Other Rate
#define BREATH_RATE_OTHER_ADDR_Old              0x5A8
//TP
#define BREATH_TP_ADDR_Old                      0x5A9
//PS
#define BREATH_PS_ADDR_Old                      0x5AA
//Pinsp
#define BREATH_PINSP_ADDR_Old                   0x5AB
//PTR
#define BREATH_PTR_ADDR_Old                     0x5AC
//FTR
#define BREATH_FTR_ADDR_Old                     0x5AD
//PEEP
#define BREATH_PEEP_ADDR_Old                    0x5AE
//触发模式
#define BREATH_TRIGGERMODE_ADDR_Old             0x5AF
//呼吸模式
#define BREATH_BREATHMODE_ADDR_Old              0x5B0
//窒息模式
#define BREATH_APNOEA_ADDR_Old                  0x5B1
//叹息时间
#define BREATH_SIGHTIMES_ADDR_Old               0x5B2
//Tslope
#define BREATH_TSLOPE_ADDR_Old                  0x5B3
//Esens
#define BREATH_ESENS_ADDR_Old                   0x5B4
//Flow Patten
#define BREATH_FLOWPATTEN_ADDR_Old              0x5B5
//I:E
#define BREATH_IE_ADDR_Old                      0x5B6
//保留 25字节

//PH上限
#define PH_HIGH_ADDR_Old                        0x500
//PH下限
#define PH_LOW_ADDR_Old                         0x501
//MV上限
#define MV_HIGH_ADDR_Old                        0x502
//MV下限
#define MV_LOW_ADDR_Old                         0x503
//RATE上限
#define RATE_HIGH_ADDR_Old                      0x504
//RATE下限
#define RATE_LOW_ADDR_Old                       0x505
//FiO2上限
#define FIO2_HIGH_ADDR_Old                      0x506
//FiO2下限
#define FIO2_LOW_ADDR_Old                       0x507
//VTE上限
#define VTE_HIGH_ADDR_H_Old                     0x508
#define VTE_HIGH_ADDR_L_Old                     0x509
//VTE下限
#define VTE_LOW_ADDR_H_Old                      0x510
#define VTE_LOW_ADDR_L_Old                      0x511
//窒息时间设置
#define APNEA_TIME_ADDR_Old                     0x512
//EtCO2上限
#define ETCO2_HIGH_ADDR_Old                     0x517
//EtCO2下限
#define ETCO2_LOW_ADDR_Old                      0x518
//FiCO2上限
#define FICO2_HIGH_ADDR_Old                     0x587
//FiN2O上限
#define FIN2O_HIGH_ADDR_Old                     0x588
//FiN2O下限
#define FIN2O_LOW_ADDR_Old                      0x589
//EtAA上限
#define ETAA_HIGH_ADDR_Old                      0x58A
//EtAA下限
#define ETAA_LOW_ADDR_Old                       0x58B
//FiAA上限
#define FIAA_HIGH_ADDR_Old                      0x58C
//FiAA下限
#define FIAA_LOW_ADDR_Old                       0x58D
//FG上限
#define FG_HIGH_ADDR_Old                        0x58E
//FG下限
#define FG_LOW_ADDR_Old                         0x58F
//Plus上限
#define PLUS_HIGH_ADDR_Old                      0x515
//Plus下限
#define PLUS_LOW_ADDR_Old                       0x516
//SpO2上限
#define SPO2_HIGH_ADDR_Old                      0x513
//SpO2下限
#define SPO2_LOW_ADDR_Old                       0x514
//PI上限
#define PI_HIGH_ADDR_H_Old                      0x594
#define PI_HIGH_ADDR_L_Old                      0x595
//PI下限
#define PI_LOW_ADDR_H_Old                       0x596
#define PI_LOW_ADDR_L_Old                       0x597


//选配备份
#define OPTIONAL_0_ADDR_BACK_Old                0x512
#define OPTIONAL_1_ADDR_BACK_Old                0x513
#define OPTIONAL_2_ADDR_BACK_Old                0x514
#define OPTIONAL_3_ADDR_BACK_Old                0x515
#define OPTIONAL_4_ADDR_BACK_Old                0x516
#define OPTIONAL_5_ADDR_BACK_Old                0x517
#define OPTIONAL_6_ADDR_BACK_Old                0x518
#define OPTIONAL_7_ADDR_BACK_Old                0x519
        
#define ETCO2_Kpa_HIGH_ADDR_Old                 0x519
#define ETCO2_Kpa_LOW_ADDR_Old                  0x520
#define FICO2_Kpa_HIGH_ADDR_Old                 0x523
#define FICO2_Kpa_LOW_ADDR_Old                  0x524    
//保留
//user add


#endif

