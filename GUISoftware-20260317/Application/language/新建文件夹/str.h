#ifndef __SRT_H
#define __SRT_H

#define STR_NUM				450	//总数
/*
系统项：
*/   
#define STR_Mode     						0 // 模式
#define STR_Control  						1  //控制
#define STR_Alarm    						2  //报警
#define STR_System   						3  //系统
#define STR_Monitoring  					4  //监测
#define STR_Cancel  						5//取消
#define STR_Confirm 						6  //确定
#define STR_Volume							7  //容量
#define STR_Pressure						8  //压力
#define STR_Invalidtouch 					9 //范围
#define STR_Alarm_Log						10 //报警记录
#define STR_FREQ							11 //呼吸频率
#define STR_MV								12 //分钟通气量
#define STR_FIO2							13 //氧浓度
#define STR_VTE								14 // 呼气潮气量
#define STR_Apnea_time						15 // 窒息时间
#define STR_Date							16 // 日期
#define STR_Time							17 // 时间
#define STR_Event							18 // 事件
#define STR_Info							19 // 系统信息
#define STR_Set								20 // 设置
#define STR_Date_Time						21 // 日期 & 时间
#define STR_Calib							22 // 系统校验
#define STR_Baro_pressure 					23 // 大气压力
#define STR_Gas_Supply_Pressure				24 // 气源压力
#define STR_Run_Time						25 // 运行时间
#define STR_touch							26 // 触摸
#define STR_Sigh_Breath_Every 				27 // 叹息间隔
#define STR_Language						28 //  语言设置
#define STR_Trigger_Type					29 // 触发类型
#define STR_CO2_unit						30 // 二氧化碳单位
#define STR_Loudness						31 // 报警音量
#define STR_Pressure_trigger				32 // 压力触发
#define STR_Flow_trigger		 			33// 流量触发
#define STR_Year 							34		// 年
#define STR_Month							35 // 月
#define STR_Day								36 // 日
#define STR_Hour							37 // 小时
#define STR_Minutes							38 // 分钟
#define STR_Apply							39 // 应用

#define STR_Pressure_Sensor_Cal				42 // 压力传感器校验
#define STR_Flow_Sensor_Cal 				43// 流量传感器校验
#define STR_Oxygen_Cell_Cal	 				44// 氧浓度传感器校验
#define STR_Service_Mode					45 // 攻城狮模式
#define STR_Flow_Zero_Cal					46 // 流量计零点校验
#define STR_PhaseIn_Zero_Cal 				47 // 多气体模块校验
#define STR_Touch_Cal						48 // 触摸屏校验
#define STR_Touch_To_Cal					49 // 请点击十字中心进行校验
#define STR_Set_O2_Concen					50 // 选择校验的氧浓度
#define STR_O2_10LMin 						51// 调节流量计输出大于10L/Min的氧气
#define STR_Air_10LMin						52 // 调节流量计输出大于10L/Min的空气或将氧浓度传感器置于空气中不少于2分钟
#define STR_Return							53 // 返回
// #define STR_Flow_Close						54 // 确定流量阀关闭
#define STR_Iso_gas_moudle					55 // 取下麻醉气体模块置于空气中
#define STR_Input_password					56 // 输入密码
#define STR_Password_Error					57 // 密码错误
#define STR_N2O_Open						58 //确定
#define STR_Insp_Valve_Cal					59 // 吸气阀校验
#define STR_Exp_Flow_Sensor_Cal				60 // 呼气传感器校验
#define STR_Exp_Valve_Cal					61 // 呼气阀校验
#define STR_Flowmeter_Cal					62 // 电子流量计校验
#define STR_Calibrating						63 // 正在校验
#define STR_Cal_Failed						64 // 校验失败！
#define STR_Cal_Success						66 // 校验成功
#define STR_Waiting							68 // 请等待
#define STR_No_Progam						69 // 没有程序可以升级
#define STR_No_Response						70 // 无响应
#define STR_Continue						71 // 继续
#define STR_Restart							72 // 请重新启动设备
#define STR_Full_Serial						73 // 请输入完整序列号
#define STR_Serial_Correct					74 // 序列号正确
#define STR_Serial_Failure					75 // 序列号错误
#define STR_Entry_Serial					76 // 请输入序列号
#define STR_Backspace						77 // 退格
#define STR_Battery_Cal 					78 // 电池校验
#define STR_Battery_Voltage					79 // 电池电压
#define STR_Last_Battery_Cal				80 // 上次电池校验
#define STR_Cal_Date						81 // 校验日期
#define STR_Cal_Duration					82 // 校验持续
#define STR_All								83 // 全部
#define STR_Up								84 // 上一步
#define STR_Input							85  //输入数值
#define STR_Input_Again						86 // 再次输入数值
#define STR_Data_Not_Save					87 // 数据未保存，是否退出？
#define STR_Rec_Error						88 // 接收错误
#define STR_Rec_Timeout						89 // 接受超时
#define STR_Zero_Beyond						90 // 零点超范围
#define STR_EEPROM_Write_Error				91 // EEPROM写错误
#define STR_EEPROM_Read_Error				92 // EEPROM读错误
#define STR_N2O_AD_Beyond					93 // 笑气AD值超范围
#define STR_Check_N2O						94 // 校验笑气
#define STR_Check_AIR						95 // 校验空气
#define STR_Value1							97 // 监测参数
// #define STR_Graphics						98 // 波形设置
// #define STR_Fresh_gas						100 //新鲜气体
#define STR_Total_Gas						101 //总流量
#define STR_Waveform						102 //波形检测
// #define STR_Rapid_Monitor					103 //快捷参数
#define STR_Paw								105 //压力
#define STR_Pleth							106 //Pleth
#define STR_No_Vent_To_Patient				107 //待机模式下患者不能得到通气支持
#define STR_Activate_Standby				108 //激活待机模式
#define STR_Sec								109 //秒
#define STR_Backup							110 //备份通气
#define STR_Paw_V_Loop						111 //压力-容量 环
#define STR_V_Flow_Loop						112 //容量-流速 环
#define STR_ON								113	//开	
#define STR_OFF								114	//关
#define STR_English							115	//英语	
#define STR_Pulse							116	//脉率
#define STR_SPO2							117	//血氧饱和度
#define STR_EFM								121	//EFM应用软件版本
#define STR_Enter							122	//确定
#define STR_EXIT							123	//退出
// #define STR_Layout2							124 //布局 2
#define STR_Limits1 						125//设置 1   
#define STR_Limits2 						126//设置 2   
#define STR_Limits3 						127//设置 3   
#define STR_N2O_Close						128//监测参数 2	

#define STR_Max_Flow  						129 //最大流速
#define STR_Insp_Time						130 //吸气时间
#define STR_I_E								131 //呼吸比1
#define STR_Pressure_Mode					133 //压力模式
#define STR_Chinese							134 //简体中文	
#define STR_Turkish							135 //TúRKεE
#define STR_Spanish							136 //Espaйol
#define STR_Flow							137 //Flow  
#define STR_Total_Flow_Eng					138 //total flow English						
#define STR_N2O								140 //N⑵0	
#define STR_AA1								141 //AA1	
#define STR_AA2								142 //AA2	
#define STR_CO2								143 //CO② //
#define STR_EtCO2							147 //呼末CO②		
#define STR_FiCO2							148 //吸入CO②		
#define STR_Set_Mode						149 //模式设置		
#define STR_Upgrade							150 //升级	
#define STR_Cancel_Upgrade					151 //取消升级
#define STR_Continue_Upgrade				152 //继续升级
#define STR_VT_1500							153 //潮气量最大值1500mL
#define STR_Pinsp_70						154 //压力控制水平最大值70cmH②O
#define STR_Ie_41							155 //吸呼比最大值4:1
#define STR_Tinsp_10						156 //吸气时间最大值10.0s
#define STR_Tisnp_Tpause_10					157 //吸气通气时间最大值10.0s
#define STR_Psupp_70						158 //压力支持水平最大值70cmH②O
#define STR_Simv_Freq_40					159 //SIMV模式下呼吸频率最大值40bpm
#define STR_Freq_100						160 //呼吸频率最大值100bpm
#define STR_Peep_30							161 //呼气末正压最大值30cmH②O
#define STR_Pause_50						162 //吸气暂停百分比最大值60% // // // //han
#define STR_Esense_80						163 //呼气灵敏度最大值80%
#define STR_Tslop_2							164 //压力上升时间最大值2.0s
#define STR_Fsense_20						165 //流量触发灵敏度最大值20lpm
#define STR_Psense_20						166 //压力触发灵敏度最大值20cmH②O
				
// #define STR_Vt_20							167 // //潮气量最小值20mL
#define STR_Vt_50							168 //潮气量最小值50mL
#define STR_Pinsp_5							169 //压力控制水平最小值5cmH②O
#define STR_Ie_110							170 //吸呼比最小值1:10
#define STR_Tinsp_1							171 //吸气时间最小值0.1s
#define STR_Freq_1							172 //呼吸频率最小值1bpm
#define STR_Freq_4							173 //呼吸频率最小值4bpm // //sun //add //20140416
#define STR_Psupport_0						174 //压力支持水平最小值0cmH②O
#define STR_Peep_Off						175 //呼气末正压最小值呼气末正压最小值2cmH②O
#define STR_Pause_0							176 // //吸气暂停百分比最小值0%
#define STR_Tslop_0							177 //压力上升时间最小值0.0s
#define STR_Fsense_5						178 //流量触发最小值0.5lpm
#define STR_Psense_0						179 //压力触发最小值0cmH②O
#define STR_Esense_5						180 //呼气灵敏度最小值5%
				
#define STR_Tinsp_Tpause_1					181 //吸气通气时间最小值0.1s
#define STR_Tinsp_Tslop						182 //压力上升时间不大于吸气通气时间
#define STR_Flow_High						183 //最大流量值限制
#define STR_Flow_Low						184 //最小流量值限制
				
#define STR_Psupport_Peep_70				185 //压力支持水平加呼末正压不高于70cmH②O
#define STR_Pinsp_Peep_70					186 //压力控制水平加呼末正压不高于70cmH②O
#define STR_Pinsp_Peep_Ph					187 //压力控制水平加呼末正压低于压力报警上限
#define STR_Psupp_Peep_Ph					188 //压力支持水平加呼末正压低于压力报警上限
#define STR_Peep_Ph							189 //呼气末正压低于压力报警上限
#define STR_Texp_Low						190 //最小呼气时间限制
				
#define STR_Backup_VCV						191 //备份通气VCV模式激活
#define STR_Backup_PCV						192 //备份通气PCV模式激活
		                         	
#define STR_Pinsp_30						193 //压力控制水平确定大于30cmH②O？
#define STR_Pinsp_50						194 //压力控制水平确定大于50cmH②O？
#define STR_Peep_Psupport_30				195 //压力支持水平加呼末正压大于30cmH②O？
#define STR_Peep_Psupport_50				196 //压力支持水平加呼末正压大于50cmH②O？
#define STR_Peep_20							197 //呼末正压确定大于20cmH②O？
#define STR_Ie_1							198 //吸呼比设置确定大于1:1(反比通气)？		

#define STR_Sec_1            				199 //sec
#define STR_USA_Standard					200 //
#define STR_EN_Standard						201 //
#define STR_FiN2O							202
// #define STR_ET								203
// #define STR_FI								204
// #define STR_FREQ_English					205
#define STR_Value2							206
#define STR_Set_EFM_Cal_Baro_Pressure		207
#define STR_O2                  208
#define STR_N20                 209
#define STR_AIR                 210
#define STR_Cal_Error           211
#define STR_Boot_Check_Error    212
#define STR_O2_AD_Beyond        213
#define STR_AIR_AD_Beyond       214
#define STR_Software_Version    215
#define STR_User_BP_Set         216
#define STR_Cal_BP_set          217
// #define STR_Flowmeter_Air_Test  218

#define STR_Russian             219
#define STR_Hours               220


//////////zkq add 20170121
#define STR_Start               221
#define STR_System_Test         222
#define STR_Preuse_Test         223
#define STR_DISP_ERR_NUM        224
#define STR_Unit_kPa            225
#define STR_Unit_mmHg           226   
#define STR_Unit_Percent        227   
#define STR_Unit_cmH2O          228   
#define STR_Unit_mbar           229   
#define STR_Unit_cm             230   
#define STR_Unit_in             231   
#define STR_Unit_hPa            232   
#define STR_Unit_PSI            233   
#define STR_Unit_lb             234   
#define STR_Unit_kg             235   
#define STR_Unit_L              236   
#define STR_Unit_L_min          237   
#define STR_Unit_mL_min         238   
#define STR_Unit_mL             239   
#define STR_Unit_mL_cmH2O       240   
#define STR_Unit_mL_mbar        241   
#define STR_Unit_mL_cmH2O_L_S   242   
#define STR_Unit_mL_mbar_L_S    243   
#define STR_Unit_mL_sec         244   
#define STR_Unit_mL_s           245   
#define STR_Unit_mL_LPM         246   
#define STR_Unit_mL_bpm         247   
#define STR_Unit_mL_mLPM        248   

#define STR_SelfTest_Ask        249
#define STR_SelfTest_Exit       250
#define STR_SelfTest_Manual_Circuit      251 
#define STR_SelfTest_Auto_Circuit        252
#define STR_SelfTest_Last_Manual_Circuit 253
#define STR_SelfTest_Last_Auto_Circuit   254

#define STR_SelfTest_Checking            255
#define STR_SelfTest_Result_State        256
#define STR_SelfTest_Symbol_Pass         257
#define STR_SelfTest_Symbol_Skip         258
#define STR_SelfTest_Symbol_Fail         259

//Check item
#define STR_SelfTest_Manual_Check        260
#define STR_SelfTest_Baro_Check          261
#define STR_SelfTest_Gas_Check           262
#define STR_SelfTest_FG_Flow_Check		 263
#define STR_SelfTest_CircuitFlow_Check   264
#define STR_SelfTest_Pressure_Check      265
#define STR_SelfTest_CircuitCompliance_Check  266
#define STR_SelfTest_CircuitLeakage_Check     267
#define STR_SelfTest_Battery_Check            268
#define STR_SelfTest_OxygenSensor_Check       317
//Self test failure prompt
#define STR_SelfTest_Err_LeakOut              269
#define STR_SelfTest_Err_BagVent              270
#define STR_SelfTest_Err_Circuit_NoConnect    271
#define STR_SelfTest_Err_ValveLeak            272
#define STR_SelfTest_Err_Sensor               273
#define STR_SelfTest_Err_SensorTest           274
#define STR_SelfTest_Err_Sensor_NoConnect     275
#define STR_SelfTest_Err_Insp_FlowSensor      276
#define STR_SelfTest_Err_Exp_FlowSensor       277
#define STR_SelfTest_Err_InspExp_FlowSensor   278
#define STR_SelfTest_Err_Compliance           279


//Manual Prompt
#define STR_SelfTest_Manual_Prompt_1          280
#define STR_SelfTest_Manual_Prompt_2          281
#define STR_SelfTest_Manual_Prompt_3          282
#define STR_SelfTest_Manual_Prompt_4          283
#define STR_SelfTest_Manual_Prompt_5          284
#define STR_SelfTest_Manual_Prompt_6          285
#define STR_SelfTest_Manual_Prompt_7          286
#define STR_SelfTest_Manual_Prompt_8          287
#define STR_SelfTest_Manual_Prompt_9          288
#define STR_SelfTest_Manual_Prompt_10         289
#define STR_SelfTest_Manual_Prompt_11         290
#define STR_SelfTest_Manual_Prompt_12         291
//Automatic Prompt 1
#define STR_SelfTest_Auto_Prompt_1            292
#define STR_SelfTest_Auto_Prompt_2            293
#define STR_SelfTest_Auto_Prompt_3            294
#define STR_SelfTest_Auto_Prompt_4            295
#define STR_SelfTest_Auto_Prompt_5            296
#define STR_SelfTest_Auto_Prompt_6            297
#define STR_SelfTest_Auto_Prompt_7            298
#define STR_SelfTest_Auto_Prompt_8            299
//Automatic Prompt 2
#define STR_SelfTest_Auto_Prompt_9            300
#define STR_SelfTest_Auto_Prompt_10           301
#define STR_SelfTest_Auto_Prompt_11           302

#define STR_SelfTest_Auto_Prompt_Drager	      344 //

#define STR_SelfTest_Auto_Prompt_12           320
#define STR_SelfTest_Auto_Prompt_13           321
#define STR_SelfTest_Auto_Prompt_14           322
#define STR_SelfTest_Auto_Prompt_15           323
#define STR_SelfTest_Auto_Prompt_16           324
#define STR_SelfTest_Auto_Prompt_17           325
#define STR_SelfTest_Auto_Prompt_18           431//

#define STR_Finish                            303
#define STR_Retry                             304
#define STR_SelfTest_Disconnect_AC            305
#define STR_SelfTest_Connect_AC               306

#define STR_SelfTest_Err_Gas_Supply           307
#define STR_SelfTest_Err_O2_Supply            308
#define STR_SelfTest_Err_N2O_Supply           309
#define STR_SelfTest_Err_Air_Supply           310
#define STR_SelfTest_Err_O2_N2O_Supply        311
#define STR_SelfTest_Err_O2_Air_Supply        312
#define STR_SelfTest_Err_N2O_Air_Supply       313

#define STR_SelfTest_Err_Timeout              314
#define STR_Empty                             315
#define STR_Pass                              316
#define STR_Fail                              318
#define STR_Skip                              319
#define STR_Alarm_Reset                       326
#define STR_SelfTest_Automatic                327

#define STR_Alarm_Silence                     328
// #define STR_Dirver_Gas_Change_Air             329
// #define STR_Dirver_Gas_Change_O2              330
#define STR_Dirver_Gas_Air                    331
#define STR_Close_Air_Switch                  332
#define STR_Change_Dirver_Gas                 333
#define STR_Trend                             334
#define STR_Trend_graph                       335
#define STR_STANDBY                           336
#define STR_CLEANTREND                        337
#define STR_DRIVER                            338
#define STR_GASNOZERO                         339     
#define STR_CLEARTREND                        340 

#define STR_PPEAK					   	    341
#define STR_PEEP							342
#define STR_PPLAT							343
#define STR_FiAA							346
#define STR_Fspn							349
#define STR_Ftotal							350
#define STR_VTi								351
#define STR_Rst								352
#define STR_Cdyn							353
#define STR_Mvspn							354
#define STR_Pmean							356

#define STR_AA							    357
#define STR_Font_Version					358
#define STR_Pause                           359
#define STR_Vt								360

#define STR_MAC								361
#define STR_EtN2O						    362
#define STR_FiAA1						    363
#define STR_EtAA1						    364
#define STR_FiAA2						    365
#define STR_EtAA2						    366
#define STR_UNIT_S							367
#define STR_MODES_SIGH						368
#define STR_GUI								369
#define STR_PMU								370
#define STR_BDU								371
#define STR_Pinsp							372
#define STR_FiHAL							373
#define STR_FiENF							374		
#define STR_FiISO							375
#define STR_FiSEV							376
#define STR_FiDES							377
#define STR_EtHAL						    378
#define STR_EtENF							379	
#define STR_EtISO							380
#define STR_EtSEV							381	
#define STR_EtDES							382		

#define STR_Tslope							383
#define STR_Psupp							384
#define STR_Fsens							385
#define STR_Esens							386
#define STR_Tinsp							387
#define STR_Psens							389
#define STR_Loop							390
#define STR_PI								391
#define STR_SPO2VER							392
#define STR_AVERTIME						393
#define STR_SENSITIVITY						394
#define STR_SMARTTONE						395
#define STR_FASTSAT							396
#define STR_MCUVER							397
#define STR_DSPVER							398

#define STR_SoftwareVersion					399



#define STR_Option							400
#define STR_MODE_PRVC						401
#define STR_MODE_SIMV_P_PSV					402
#define STR_MODE_INFANT						403	
#define STR_MODE_Driver						405
#define STR_MODE_CO2						406
#define STR_MODE_SpO2						407

// #define STR_N2O_Range						408 //del 20181207 ylk 
// #define STR_N2O_Range1						409
// #define STR_N2O_Range2						410

#define STR_MODE_Round						411
#define STR_LASTO2CALIB						412
#define STR_MODE_O2                         413
#define STR_MODE_AA_CO2                     432

#define STR_HAL								414
#define STR_ENF								415
#define STR_ISO								416
#define STR_SEV								417
#define STR_DES								418

#define STR_MODE_Masimo_IRMA				419
#define STR_MODE_Masimo_ISA					420
#define STR_MODE_Masimo_ISA_O2				421
#define STR_MODE_Masimo_SpO2				422

#define STR_MEDIBUS_Format					423
#define STR_MEDIBUS_Active					424
#define STR_MEDIBUS_WCC						425
#define STR_MEDIBUS_PCRES					426
#define STR_MEDIBUS_MCStop					427
#define STR_MEDIBUS_Start					428
#define STR_MEDIBUS_Wait					429
#define STR_MEDIBUS_Stop					430

#define STR_Active						    433
#define STR_AlarmLogSilenceBell             434
#define STR_SetAAType                       435
#define STR_AA_O2                           436
#define STR_Drager                          437
#define STR_Drager_O2                       438
#define STR_Drager_Accuracy                 439

//总数不够时,注意修改的数值
void SYS_str_init();
void alarm_str_init(unsigned int LanguageType);
#endif