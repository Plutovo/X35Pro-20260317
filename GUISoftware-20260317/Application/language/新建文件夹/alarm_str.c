#include "MainTask.h"
#include "AA_Module.h"

extern unsigned int LanguageType;

#ifdef __ICCARM__
char const  *(Alarm_Dispaly[64]) @"AHBRAM";
char const  *(Alarm_Dispaly1[64]) @"AHBRAM";
char const  *(Alarm_Dispaly2[30]) @"AHBRAM";
#else
char const  *(Alarm_Dispaly[64]);
char const  *(Alarm_Dispaly1[64]);
char const  *(Alarm_Dispaly2[30]);
#endif

extern unsigned char   ETCO2_FLAG;
extern unsigned char EtCO2_Config_On;
extern unsigned char Masimo_EtCO2_Config_On;
extern unsigned char EtCO2_KM7002_Config_On;

void alarm_str_init(unsigned int LanguageType)
{
	switch(LanguageType)
	{
	case 1:
#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C)
		Alarm_Dispaly[0]	 =  "O② SUPPLY DOWN!!!";
		Alarm_Dispaly[1]	 =  "BDU Communication Failure!!!";//ylk add BDU通讯故障
		Alarm_Dispaly[2]	 =  "BATTERY FAILURE!!!";//ylk add 电池故障 高
		Alarm_Dispaly[3]	 =  "BATTERY DISCHARGED!!!";
		Alarm_Dispaly[4]	 =  "APNEA!!!";
		Alarm_Dispaly[5]	 =  "CONTINUOUS PRESSURE HIGH!!!";
		Alarm_Dispaly[6]	 =  "PRESSURE HIGH!!!";
		Alarm_Dispaly[7]	 =  "CIRCUIT DISCONNECT!!!";
		Alarm_Dispaly[8]	 =  "Gas Module Software Error!!!";
		Alarm_Dispaly[9]	 =  "Gas Module Hardware Error!!!";
		Alarm_Dispaly[10]	 =  "Gas Module Motor Overspeed!!!";
		Alarm_Dispaly[11]	 =  "Gas Module Factory calib lost!!!";
		Alarm_Dispaly[12]	 =  "Gas Module Replace Adapter!!!";
		Alarm_Dispaly[13]	 =  "Gas Module No Adapter!!!";
		Alarm_Dispaly[14]	 =  "Gas module No Line!!!";
		Alarm_Dispaly[15]	 =  "CO② Unspecified Accuracy!!!"; 
		Alarm_Dispaly[16]	 =   "N②O Unspecified Accuracy!!!";  
		Alarm_Dispaly[17]	 =  "AA Unspecified Accuracy!!!";
		Alarm_Dispaly[18]	 =  "O② Unspecified Accuracy!!!";
		Alarm_Dispaly[19]	 =   "Gas Module Temp Out Of Range!!!";
		Alarm_Dispaly[20]	 =  "Pressure Out Of Range!!!";
		Alarm_Dispaly[21]	 =  "AA ID Unreliable!!!";
		Alarm_Dispaly[22]	 =  "O② span calibration required!!!";
		Alarm_Dispaly[23]	 =  "";
		Alarm_Dispaly[24]	 =  "";
		Alarm_Dispaly[25]	 =  "MV LOW!!!";
		Alarm_Dispaly[26]	 =  "MV HIGH!!!";  
		Alarm_Dispaly[27]	 =  "RATE LOW!!!"; 
		Alarm_Dispaly[28]	 =  "FiO② HIGH!!!";
		Alarm_Dispaly[29]	 =  "O② SENSOR FAILURE?!!!"; 
		Alarm_Dispaly[30]	 =  "FiO② LOW!!!";
		Alarm_Dispaly[31]	 =  "EtCO② HIGH!!!";
		Alarm_Dispaly[32]	 =  "FiCO② HIGH!!!";
		Alarm_Dispaly[33]	 =  "FiN②O HIGH !!!";
		Alarm_Dispaly[34]	 =  "STANDBY ACTIVED!!!";
		Alarm_Dispaly[35]	 =  "PRESSURE<-10cmH②O!!!"; 
		Alarm_Dispaly[36]	 =  "PRESSURE LOW!!!";
		Alarm_Dispaly[37]	 =  "BATTERY LOW!!!";
		Alarm_Dispaly[38]	 =  "SPO② FAILURE !!!";     
		Alarm_Dispaly[39]	 =  "SPO② LOW PERFUSION !!!";     
		Alarm_Dispaly[40]	 =  "SPO② DEFECTIVE SENSOR !!!";     
		Alarm_Dispaly[41]	 =  "SPO② UNRECOGNIZED SENSOR !!!";     
		Alarm_Dispaly[42]	 =  "SPO② LOW SIGNAL IQ !!!";     
		Alarm_Dispaly[43]	 =  "SPO② PI LOW !!!";     
		Alarm_Dispaly[44]	 =  "SpO② LOW!!!";
		Alarm_Dispaly[45]	 =  "PULSE RATE LOW!!!";
		Alarm_Dispaly[46]	 =  "VTe HIGH!!!";     
		/***************************ylk add 20181205***************************/
		Alarm_Dispaly[47]	 =  "AA Module Watertrap Full!!!";     
		Alarm_Dispaly[48]	 =  "Check AA Module Watertrap or Sampling Line!!!";     
		Alarm_Dispaly[49]	 =  "AA Module Power Input Failure!!!";     
		Alarm_Dispaly[50]	 =  "CO② Sensor Failure!!!";     
		Alarm_Dispaly[51]	 =  "O② Sensor Failure!!!";     
		Alarm_Dispaly[52]	 =  "AA Module Failure!!!";     
		/**********************************************************************/

		Alarm_Dispaly1[0]	 =  "AIR SUPPLY DOWN!!";   //djq add 20170407   
		Alarm_Dispaly1[1]	 =  "NO ABSORBER!!"; 
		Alarm_Dispaly1[2]	 =  "REGULATION PRESSURE LIMITED!!";
		Alarm_Dispaly1[3]	 = "VTe LOW!!";      
		Alarm_Dispaly1[4]	 = "RATE HIGH!!";
		Alarm_Dispaly1[5]	 = "PULSE RATE HIGH!!";
		Alarm_Dispaly1[6]	 = "SPO② INTERFERENCE DETECTED !!";         
		Alarm_Dispaly1[7]	 = "SPO② TOO MUCH AMBIENT LIGHT !!";         
		Alarm_Dispaly1[8]	 = "SPO② CABLE NO CONNECTED !!";         
		Alarm_Dispaly1[9]	 = "Gas Module Line Occlusion!!";
		Alarm_Dispaly1[10]	 = "AA Module Dehydrator almost Full!!";//ylk add 201812105 脱水器将满
		Alarm_Dispaly1[11]	 = "SPO② PI HIGH !!";    
		Alarm_Dispaly1[12]	 = "SpO② HIGH!!";
		Alarm_Dispaly1[13]	 = "EtCO② LOW!!";
		Alarm_Dispaly1[14]	 =  "";//FiAA LOW
		Alarm_Dispaly1[15]	 = "";//EtAA LOW
		Alarm_Dispaly1[16]	 = "FiN②O LOW!!";
		Alarm_Dispaly1[17]	 = "Mixed anesthetic MAC>=3!!";
		Alarm_Dispaly1[18]	 = "Gas Module Inaccurate Gas Zeroing!!";   
		Alarm_Dispaly1[19]	 = "AGENT OVERFLOW!!"; //ylk add 20181205 麻醉气体浓度超限
		Alarm_Dispaly1[20]	 = "BATTERY FAILURE!";//ylk add 电池故障 低
		Alarm_Dispaly1[21]	 = "Mixed anesthetic MAC<3!";
		Alarm_Dispaly1[22]	 = "MAINS FAILURE!";
		Alarm_Dispaly1[23]	 = "SpO② SENSOR DISCONNECT!";    
		Alarm_Dispaly1[24]	 = "SPO② SENSOR OFF PATIENT?!";
// 		if(AA_Module_Flag.EtCO2ModuleExist == 1 || ((AA_Module_Flag.EtCO2ModuleExist&0x06) != 0 && (!(AA_all_data.AA_state.dev_info.module_type & 0x04))))
// 		if((AA_Module_Flag.EtCO2ModuleExist == 1) || ((AA_Module_Flag.EtCO2ModuleExist == 2)&&
// 			(AA_all_data.AA_state.dev_info.module_type == AA_PHASEIN_CO2ONLY))) //ylk modify 20181218
// 		{
// 			Alarm_Dispaly1[25]	= "EtCO② OFF!";
// 		}
// 		else
// 		{
// 			Alarm_Dispaly1[25]	= "AA OFF!"; 
// 		}
 		if(EtCO2_Config_On || EtCO2_KM7002_Config_On)//ylk md 20190611
		{
			Alarm_Dispaly1[25] = "EtCO② OFF!";
		}
		else if((Masimo_EtCO2_Config_On) && (AA_Module_Flag.PhaseInTypeModuleExist == 4))
		{
			Alarm_Dispaly1[25] = "EtCO② OFF!";
		}
		else
		{
			Alarm_Dispaly1[25] = "AA OFF!"; 
		}
		Alarm_Dispaly1[26]	 = "ACGO OPEN!";
		Alarm_Dispaly1[27]	 = "O② OFF!";
		Alarm_Dispaly1[28]	 = "SpO② OFF!"; 
		Alarm_Dispaly1[29]	 = "Calibrate O② Sensor!";//ylk add 20190124

		//麻醉气相关报警
		Alarm_Dispaly2[0]	 =  "FiHAL HIGH!!!";  
		Alarm_Dispaly2[1]	 =  "FiENF HIGH!!!";  
		Alarm_Dispaly2[2]	 =  "FiISO HIGH!!!";  
		Alarm_Dispaly2[3]	 =  "FiSEV HIGH!!!";  
		Alarm_Dispaly2[4]	 =	"FiDES HIGH!!!";	

		Alarm_Dispaly2[5]	 =  "EtHAL HIGH!!!";								
		Alarm_Dispaly2[6]	 =  "EtENF HIGH!!!"; 
		Alarm_Dispaly2[7]	 =  "EtISO HIGH!!!"; 
		Alarm_Dispaly2[8]	 =  "EtSEV HIGH!!!"; 
		Alarm_Dispaly2[9]	 =  "EtDES HIGH!!!"; 

		Alarm_Dispaly2[10]	 = "FiHAL LOW!!";
		Alarm_Dispaly2[11]	 = "FiENF LOW!!";
		Alarm_Dispaly2[12]	 = "FiISO LOW!!";
		Alarm_Dispaly2[13]	 = "FiSEV LOW!!";
		Alarm_Dispaly2[14]	 = "FiDES LOW!!"; 

		Alarm_Dispaly2[15]	 = "EtHAL LOW!!";
		Alarm_Dispaly2[16]	 = "EtENF LOW!!";
		Alarm_Dispaly2[17]	 = "EtISO LOW!!";
		Alarm_Dispaly2[18]	 = "EtSEV LOW!!";
		Alarm_Dispaly2[19]	 = "EtDES LOW!!";

		Alarm_Dispaly2[20]	 =  "FiAA HIGH!!!";
		Alarm_Dispaly2[21]	 =  "EtAA HIGH!!!";
		Alarm_Dispaly2[22]	 =	"FiAA LOW!!";  
		Alarm_Dispaly2[23]	 =  "EtAA LOW!!";  	
#else
		Alarm_Dispaly[0]	 =  "O② SUPPLY DOWN!!!";          
		Alarm_Dispaly[1]	 =  "BATTERY DISCHARGED!!!";     
		Alarm_Dispaly[2]	 =  "APNEA!!!";       
		Alarm_Dispaly[3]	 =  "PRESSURE HIGH!!!";
		Alarm_Dispaly[4]	 =  "CIRCUIT DISCONNECT!!!";
		Alarm_Dispaly[5]	 =  "MV LOW!!!";
		Alarm_Dispaly[6]	 =  "MV HIGH!!!";          
		Alarm_Dispaly[7]	 =  "CIRCUIT OCCLUSION!!!";
		Alarm_Dispaly[8]	 =  "RATE LOW!!!"; 
		Alarm_Dispaly[9]	 =  "FiO② HIGH!!!";
		Alarm_Dispaly[10]	 =  "O② SENSOR FAILURE?!!!"; 
		Alarm_Dispaly[11]	 =  "FiO② LOW!!!";
		Alarm_Dispaly[12]	 =  "EtCO② HIGH!!!";
		Alarm_Dispaly[13]	 =  "FiCO② HIGH!!!"; 

		Alarm_Dispaly[14]	 =  "STANDBY ACTIVED!!!";
		Alarm_Dispaly[15]	 =  "PRESSURE<-10cmH②O!!!"; 
		Alarm_Dispaly[16]	 =  "PRESSURE LOW!!!";
		Alarm_Dispaly[17]	 =  "BATTERY LOW!!!";
		Alarm_Dispaly[18]	 =   "SpO② LOW!!!";
		Alarm_Dispaly[19]	 =  "PULSE RATE LOW!!!";
		Alarm_Dispaly[20]	 =  "VTt HIGH!!!";  

		Alarm_Dispaly1[0]	 =  "REGULATION PRESSURE LIMITED!!";
		Alarm_Dispaly1[1]	 =  "VTt LOW!!";      
		Alarm_Dispaly1[2]	 = "RATE HIGH!!";
		Alarm_Dispaly1[3]	 = "PULSE RATE HIGH!!";       
		Alarm_Dispaly1[4]	 = "Gas Module Line Occlusion!!";
		Alarm_Dispaly1[5]	 = "SpO② HIGH!!";     
		Alarm_Dispaly1[6]	 = "EtCO② LOW!!"; 
		Alarm_Dispaly1[7]	 = "FiN②O LOW !!";

		Alarm_Dispaly1[8]	 = "MAINS FAILURE!";
		Alarm_Dispaly1[9]	 = "SpO② SENSOR DISCONNECT!"; 

		Alarm_Dispaly1[10]	 = "EtCO② OFF!";     
		Alarm_Dispaly1[11]	 = "ACGO OPEN!";     
		Alarm_Dispaly1[12]	 = "O② OFF!";    
		Alarm_Dispaly1[13]	 =  "SpO② OFF!";           		
#endif
		break;
#ifdef  LANGUAGE_CHINESE_FLAG
	case 0:
#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C) 
		Alarm_Dispaly[0]	 =  "氧气不足!!!";         
		Alarm_Dispaly[1]	 =  "BDU通信故障!!!";
		Alarm_Dispaly[2]	 =  "电池故障!!!";
		Alarm_Dispaly[3]	 =  "电池耗尽!!!" ;       
		Alarm_Dispaly[4]	 =  "窒息!!!";        
		Alarm_Dispaly[5]	 =  "持续气道压力高!!!";
		Alarm_Dispaly[6]	 =  "气道压力高 !!!" ;     
		Alarm_Dispaly[7]	 =  "管路断开!!!";
		Alarm_Dispaly[8]	 =  "气体模块软件错误!!!";    
		Alarm_Dispaly[9]	 =  "气体模块硬件错误!!!";
		Alarm_Dispaly[10]	 =  "气体模块电机速度超限!!!";
		Alarm_Dispaly[11]	 =  "气体模块出厂校准缺失!!!";
		Alarm_Dispaly[12]	 =  "气体模块更换适配器!!!";  
		Alarm_Dispaly[13]	 =  "请连接气体模块适配器!!!"; 
		Alarm_Dispaly[14]	 =  "请连接气体模块采样管!!!";
		Alarm_Dispaly[15]	 =  "气体模块二氧化碳超差!!!";  
		Alarm_Dispaly[16]	 =  "气体模块笑气超差!!!";  
		Alarm_Dispaly[17]	 =  "气体模块至少一种麻醉气体超差!!!" ;
		Alarm_Dispaly[18]	 =  "气体模块氧气超差!!!" ;
		Alarm_Dispaly[19]	 =  "气体模块内部温度高!!!";
		Alarm_Dispaly[20]	 =  "大气压超出运行范围!!!";
		Alarm_Dispaly[21]	 =  "麻醉气体ID和浓度不准确!!!";
		Alarm_Dispaly[22]	 =  "顺磁氧需要校准!!!";
		Alarm_Dispaly[23]	 =  "";
		Alarm_Dispaly[24]	 =  "";
		Alarm_Dispaly[25]	 =  "分钟通气量低!!!";      
		Alarm_Dispaly[26]	 =  "分钟通气量高!!!";            
		Alarm_Dispaly[27]	 =  "呼吸频率低!!!";         
		Alarm_Dispaly[28]	 =  "氧浓度高!!!";          
		Alarm_Dispaly[29]	 =  "氧浓度传感器故障?!!!" ;  
		Alarm_Dispaly[30]	 =  "氧浓度低!!!";         
		Alarm_Dispaly[31]	 =  "呼末二氧化碳浓度高!!!" ;
		Alarm_Dispaly[32]	 =  "吸入二氧化碳浓度高!!!"; 
		Alarm_Dispaly[33]	 =  "吸入笑气浓度高!!!"; //吸入笑气浓度高
		Alarm_Dispaly[34]	 =  "待机模式激活!!!";   
		Alarm_Dispaly[35]	 =  "气道压力 < -10cmH②O!!!"; 
		Alarm_Dispaly[36]	 =  "气道压力低!!!";     
		Alarm_Dispaly[37]	 =  "电池电量低!!!";    
		Alarm_Dispaly[38]	 =  "血氧失效!!!";     
		Alarm_Dispaly[39]	 =  "血氧弱灌注 !!!";     
		Alarm_Dispaly[40]	 =  "血氧探头失效!!!";     
		Alarm_Dispaly[41]	 =  "未识别的血氧探头!!!";     
		Alarm_Dispaly[42]	 =  "血氧信号强度弱 !!!";     
		Alarm_Dispaly[43]	 =  "灌注指数低 !!!";     
		Alarm_Dispaly[44]	 =  "血氧饱和度低!!!";     
		Alarm_Dispaly[45]	 =  "脉率低!!!";     
		Alarm_Dispaly[46]	 =  "潮气量高!!!";           
		/***************************ylk add 20181205***************************/
		Alarm_Dispaly[47]	 =  "麻醉气体模块脱水器已满!!!";     
		Alarm_Dispaly[48]	 =  "检查麻醉气体模块脱水器未连接或采样管!!!";     
		Alarm_Dispaly[49]	 =  "麻醉气体模块电源输入故障!!!";     
		Alarm_Dispaly[50]	 =  "二氧化碳传感器故障!!!";     
		Alarm_Dispaly[51]	 =  "氧气传感器故障!!!";     
		Alarm_Dispaly[52]	 =  "麻醉气体模块故障!!!";     
		/**********************************************************************/

		Alarm_Dispaly1[0]	 = "空气不足!!";  //       djq add 20170407
		Alarm_Dispaly1[1]	 = "二氧化碳吸收罐移除!!";  
		Alarm_Dispaly1[2]	 = "未达到目标潮气量!!" ;
		Alarm_Dispaly1[3]	 = "潮气量低!!";              
		Alarm_Dispaly1[4]	 = "呼吸频率高!!";     
		Alarm_Dispaly1[5]	 = "脉率高!!";         
		Alarm_Dispaly1[6]	 = "血氧干扰!!";         
		Alarm_Dispaly1[7]	 = "血氧周围光太强!!";         
		Alarm_Dispaly1[8]	 = "线缆脱落!!";         
		Alarm_Dispaly1[9]	 = "气体模块采样管阻塞!!";    
		Alarm_Dispaly1[10]	 = "麻醉气体模块脱水器将满!!";//ylk add 201812105 脱水器将满

		Alarm_Dispaly1[11]	 = "灌注指数高 !!";    
		Alarm_Dispaly1[12]	 = "血氧饱和度高!!";        
		Alarm_Dispaly1[13]	 = "呼末二氧化碳浓度低!!"; 
		Alarm_Dispaly1[14]	 = "";
		Alarm_Dispaly1[15]	 = "";
		Alarm_Dispaly1[16]	 = "吸入笑气浓度低!!";    // 吸入笑气浓度低     
		Alarm_Dispaly1[17]	 = "混合麻醉剂MAC>=3!!";
		Alarm_Dispaly1[18]	 = "气体模块需要归零!!";
		Alarm_Dispaly1[19]	 = "麻醉气体浓度超限!!"; //ylk add 20181205
		Alarm_Dispaly1[20]	 = "电池故障!";
		Alarm_Dispaly1[21]	 = "混合麻醉剂MAC < 3!"; 
		Alarm_Dispaly1[22]	 = "交流电故障!";        
		Alarm_Dispaly1[23]	 = "血氧传感器未连接?!";    
		Alarm_Dispaly1[24]	 = "血氧探头与病人未连接?!";  
// 		if(AA_Module_Flag.EtCO2ModuleExist == 1 || ((AA_Module_Flag.EtCO2ModuleExist&0x06) != 0 && (!(AA_all_data.AA_state.dev_info.module_type & 0x04))))
// 		{
// 			Alarm_Dispaly1[25] = "二氧化碳监测模块关闭!"; 
// 		}
// 		else
// 		{
// 			Alarm_Dispaly1[25] = "麻醉气体模块关闭!";//麻醉气体模块关闭
// 		} 
		if(EtCO2_Config_On || EtCO2_KM7002_Config_On)
		{
			Alarm_Dispaly1[25] = "二氧化碳监测模块关闭!";
		}
		else if((Masimo_EtCO2_Config_On) && (AA_Module_Flag.PhaseInTypeModuleExist == 4))
		{
			Alarm_Dispaly1[25] = "二氧化碳监测模块关闭!";
		}
		else
		{
			Alarm_Dispaly1[25] = "麻醉气体模块关闭!";
		}

		Alarm_Dispaly1[26]	 = "ACGO 开启!";          
		Alarm_Dispaly1[27]	 = "氧浓度监测模块关闭!";   
		Alarm_Dispaly1[28]	 = "血氧监测功能关闭!"; 
		Alarm_Dispaly1[29]	 = "氧浓度传感器需要校验!";//ylk add 20190124

		//麻醉气相关报警
		Alarm_Dispaly2[0]	 = "吸入氟烷浓度高!!!";   //吸入氟烷浓度高
		Alarm_Dispaly2[1]	 = "吸入安氟醚浓度高!!!";  //吸入安氟醚浓度高
		Alarm_Dispaly2[2]	 = "吸入异氟醚浓度高!!!";  //吸入异氟醚浓度高
		Alarm_Dispaly2[3]	 = "吸入七氟醚浓度高!!!";  //吸入七氟醚浓度高
		Alarm_Dispaly2[4]	 = "吸入地氟醚浓度高!!!";	//吸入地氟醚浓度高

		Alarm_Dispaly2[5]	 = "呼末氟烷浓度高!!!"; //呼末氟烷浓度高					
		Alarm_Dispaly2[6]	 = "呼末安氟醚浓度高!!!"; //呼末安氟醚浓度高
		Alarm_Dispaly2[7]	 = "呼末异氟醚浓度高!!!"; //呼末异氟醚浓度高
		Alarm_Dispaly2[8]	 = "呼末七氟醚浓度高!!!"; //呼末七氟醚浓度高
		Alarm_Dispaly2[9]	 = "呼末地氟醚浓度高!!!"; //呼末地氟醚浓度高

		Alarm_Dispaly2[10]	 = "吸入氟烷浓度低!!";    //吸入氟烷浓度低
		Alarm_Dispaly2[11]	 = "吸入安氟醚浓度低!!";   //吸入安氟醚浓度低
		Alarm_Dispaly2[12]	 = "吸入异氟醚浓度低!!";  //吸入异氟醚浓度低
		Alarm_Dispaly2[13]	 = "吸入七氟醚浓度低!!";  //吸入七氟醚浓度低
		Alarm_Dispaly2[14]	 = "吸入地氟醚浓度低!!";	//吸入地氟醚浓度低

		Alarm_Dispaly2[15]	 = "呼末氟烷浓度低!!";   //呼末氟烷浓度低
		Alarm_Dispaly2[16]	 = "呼末安氟醚浓度低!!";  //呼末安氟醚浓度低
		Alarm_Dispaly2[17]	 = "呼末异氟醚浓度低!!"; //呼末异氟醚浓度低
		Alarm_Dispaly2[18]	 = "呼末七氟醚浓度低!!";  //呼末七氟醚浓度低
		Alarm_Dispaly2[19]	 = "呼末地氟醚浓度低!!";  //呼末地氟醚浓度低

		Alarm_Dispaly2[20]	 = "吸入麻醉气体高!!!";  
		Alarm_Dispaly2[21]	 = "呼末麻醉气体高!!!"; 
		Alarm_Dispaly2[22]	 = "吸入麻醉气体低!!";  
		Alarm_Dispaly2[23]	 = "呼末麻醉气体低!!";  	
#else
		Alarm_Dispaly[0]	 =  "氧气不足";          
		Alarm_Dispaly[1]	 =  "电池耗尽!!!" ;       
		Alarm_Dispaly[2]	 =  "窒息!!!"    ;        
		Alarm_Dispaly[3]	 =  "气道压力高!!!" ;     
		Alarm_Dispaly[4]	 =  "患者未连接!!!";
		Alarm_Dispaly[5]	 =  "分钟通气量低!!!";      
		Alarm_Dispaly[6]	 =  "分钟通气量高!!!";                 
		Alarm_Dispaly[7]	 =  "持续压力高!!!";
		Alarm_Dispaly[8]	 =  "呼吸频率低!!!";         
		Alarm_Dispaly[9]	 =  "氧浓度高!!!";          
		Alarm_Dispaly[10]	 =  "氧浓度传感器故障?!!!" ;  
		Alarm_Dispaly[11]	 =  "氧浓度低!!!";         
		Alarm_Dispaly[12]	 =  "呼末二氧化碳浓度高!!!" ;
		Alarm_Dispaly[13]	 =  "吸入二氧化碳浓度高!!!";
		Alarm_Dispaly[14]	 =  "待机模式激活!!!";   
		Alarm_Dispaly[15]	 =  "气道压力 < -10cmH②O!!!";        

		Alarm_Dispaly[16]	 =  "气道压力低!!!";     
		Alarm_Dispaly[17]	 =  "电池电量低!!!";    
		Alarm_Dispaly[18]	 =  "血氧饱和度低!!!";     
		Alarm_Dispaly[19]	 =   "脉率低!!!";     
		Alarm_Dispaly[20]	 =  "潮气量高!!!";        

		Alarm_Dispaly1[0]	 = "未达到目标潮气量!!" ;
		Alarm_Dispaly1[1]	 =  "潮气量低!!";              
		Alarm_Dispaly1[2]	 = "呼吸频率高!!";     
		Alarm_Dispaly1[3]	 = "脉率高!!";         
		Alarm_Dispaly1[4]	 =  "气体模块采样管阻塞!!";    
		Alarm_Dispaly1[5]	 =  "血氧饱和度高!!";        
		Alarm_Dispaly1[6]	 =  "呼末二氧化碳浓度低!!"; 
		Alarm_Dispaly1[7]	 = "吸入笑气浓度低!!";     

		Alarm_Dispaly1[8]	 = "交流电故障!";        
		Alarm_Dispaly1[9]	 = "血氧传感器未连接?!";

		Alarm_Dispaly1[10]	 = "二氧化碳监测模块关闭!";          
		Alarm_Dispaly1[11]	 = "ACGO 开启!";          
		Alarm_Dispaly1[12]	 = 	 "氧浓度监测模块关闭!";   
		Alarm_Dispaly1[13]	 =  "血氧监测功能关闭!";           
#endif
		break;
#else
	case 2:
#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C) 
		Alarm_Dispaly[0]	 =  "SUMINISTRO DE O② BAJO!!!";
		Alarm_Dispaly[1]	 =  "Falla de comunicación BDU!!!";//BDU通讯故障
		Alarm_Dispaly[2]	 =  "Falla Bateria!!!";//电池故障
		Alarm_Dispaly[3]	 =  "BATERIA DESCARGADA!!!";
		Alarm_Dispaly[4]	 =  "APNEA!!!";
		Alarm_Dispaly[5]	 =  "PRESIóN ALTA CONTINUOSA!!!";
		Alarm_Dispaly[6]	 =  "PRESIóN ALTA!!!";
		Alarm_Dispaly[7]	 =  "DESCONECCIóN DE CIRCUITO!!!";
		Alarm_Dispaly[8]	 =  "Gas Module Software Error!!!";
		Alarm_Dispaly[9]	 =  "Gas Module Hardware Error!!!";
		Alarm_Dispaly[10]	 =  "Gas Module Motor Overspeed!!!";
		Alarm_Dispaly[11]	 =  "Gas Module Factory calib lost!!!";
		Alarm_Dispaly[12]	 =  "Gas Module Replace Adapter!!!";
		Alarm_Dispaly[13]	 =  "Gas Module No Adapter!!!";
		Alarm_Dispaly[14]    =  "Gas module No Line!!!";
		Alarm_Dispaly[15]	 =  "CO② Unspecified Accuracy!!!"; 
		Alarm_Dispaly[16]	 =  "N②O Unspecified Accuracy!!!";  
		Alarm_Dispaly[17]	 =  "AA Unspecified Accuracy!!!";
		Alarm_Dispaly[18]	 =  "O② Unspecified Accuracy!!!";
		Alarm_Dispaly[19]	 =  "Gas Module Temp Out Of Range!!!";
		Alarm_Dispaly[20]	 =  "Pressure Out Of Range!!!";
		Alarm_Dispaly[21]	 =  "AA ID Unreliable!!!";
		Alarm_Dispaly[22]	 =  "O② span calibration required!!!";
		Alarm_Dispaly[23]	 =  "";
		Alarm_Dispaly[24]	 =  "";
		Alarm_Dispaly[25]	 =  "VOLUMEN MINUTO BAJO!!!";
		Alarm_Dispaly[26]	 =  "VOLUMEN MINUTO ALTO!!!";  

		Alarm_Dispaly[27]	 =  "FRECUENCIA BAJA!!! ";
		Alarm_Dispaly[28]	 =  "FiO② ALTO!!!";
		Alarm_Dispaly[29]	 =  "FALLA O② SENSOR?!!!";
		Alarm_Dispaly[30]	 =  "FiO② BAJO!!!";
		Alarm_Dispaly[31]	 =  "EtCO② ALTO!!!";
		Alarm_Dispaly[32]	 =  "FiCO② ALTO!!!";
		Alarm_Dispaly[33]	 =  "FiN②O ALTO !!!";
		Alarm_Dispaly[34]	 =  "ESPERA ACTIVADA!!!";
		Alarm_Dispaly[35]	 =  "PRESIóN<-10cmH②O!!!"; 
		Alarm_Dispaly[36]	 =  "PRESIóN BAJA!!!";
		Alarm_Dispaly[37]	 =  "BATERIA BAJA!!!";
		Alarm_Dispaly[38]	 =  "SPO② FRACASO !!!";     //
		Alarm_Dispaly[39]	 =  "SPO② baja perfusión !!!";     //
		Alarm_Dispaly[40]	 =  "SPO② Sensor defectuoso !!!";     //
		Alarm_Dispaly[41]	 =  "SPO② SENSOR DESCONOCIDO !!!";     //
		Alarm_Dispaly[42]	 =  "SPO② BAJO SEÑAL IQ !!!";     //
		Alarm_Dispaly[43]	 =  "SPO② PI BAJO !!!";     //
		Alarm_Dispaly[44]	 =  "SpO② BAJO!!!";
		Alarm_Dispaly[45]	 =  "FRECUENCIA DE PULSO BAJO!!!";//
		Alarm_Dispaly[46]	 =  "VTe ALTO!!!";       
		/***************************ylk add 20181205***************************/
		Alarm_Dispaly[47]	 =  "EL DESHIDRATADOR DEL MÓDULO DE AA ESTÁ LLENO!!!";     
		Alarm_Dispaly[48]	 =  "Mod AA trampa de Agua desconectada!!!";     
		Alarm_Dispaly[49]	 =  "Falla en la fuente de entrada del mod AA!!!";     
		Alarm_Dispaly[50]	 =  "FALLA DEL SENSOR DE CO②!!!";     
		Alarm_Dispaly[51]	 =  "FALLA DEL SENSOR DE O②!!!";     
		Alarm_Dispaly[52]	 =  "FALLA DEL MÓDULO DE AA!!!";     
		/**********************************************************************/

		Alarm_Dispaly1[0]	 =  "SUMINISTRO DE AIRE BAJO!!";   //djq add 20170407
		Alarm_Dispaly1[1]	 =  "SIN ABSORBEDOR!!"; 
		Alarm_Dispaly1[2]	 =  "REGULACIóN DE PRESIóN LIMITADA!!";
		Alarm_Dispaly1[3]	 =  "VTe BAJO!!";    
		Alarm_Dispaly1[4]	 =  "FRECUENCIA ALTA!!";
		Alarm_Dispaly1[5]	 =  "FRECUENCIA DE PULSO ALTO!!";//
		Alarm_Dispaly1[6]	 = "SPO② interferencia detectada !!";         //
		Alarm_Dispaly1[7]	 = "SPO② DEMASIADA LUZ AMBIENTE !!";         //
		Alarm_Dispaly1[8]	 = "SPO② cable NO conectado !!";         //;
		Alarm_Dispaly1[9]	 = "Línea de módulo de gas de oclusión!!";//
		Alarm_Dispaly1[10]	 = "EL DESHIDRATADOR DEL MÓDULO DE AA ESTÁ CASI LLENO!!";//ylk add 201812105 脱水器将满
		Alarm_Dispaly1[11]	 = "SPO② PI ALTO !!";    //
		Alarm_Dispaly1[12]	 = "SpO② ALTO!!";
		Alarm_Dispaly1[13]	 = "EtCO② BAJO!!";
		Alarm_Dispaly1[14]	 = "";
		Alarm_Dispaly1[15]	 = "";
		Alarm_Dispaly1[16]	 = "FiN②O BAJO!!";
		Alarm_Dispaly1[17]	 = "Anestésico mezclado MAC>=3!!";
		Alarm_Dispaly1[18]	 = "Gas Module Inaccurate Gas Zeroing!!";
		Alarm_Dispaly1[19]	 = "Falla Bateria!";//电池故障
		Alarm_Dispaly1[20]	 = "AGENT OVERFLOW!!"; //ylk add 20181205 麻醉气体浓度超限 -- 未翻译
		Alarm_Dispaly1[21]	 = "Anestésico mezclado MAC<3!";
		Alarm_Dispaly1[22]	 = "FALLA DE ENERGíA!";
		Alarm_Dispaly1[23]	 = "SpO② SENSOR DESCONECTADO!";
		Alarm_Dispaly1[24]	 = "SPO② SENSOR OFF PACIENTE?!";// 
		if(EtCO2_Config_On || EtCO2_KM7002_Config_On)
		{
			Alarm_Dispaly1[25] = "EtCO② Apagado!"; 
		}
		else if((Masimo_EtCO2_Config_On) && (AA_Module_Flag.PhaseInTypeModuleExist == 4))
		{		
			Alarm_Dispaly1[25] = "EtCO② Apagado!"; 			
		}
		else
		{
			Alarm_Dispaly1[25] = "AA Apagado!";
		}
		Alarm_Dispaly1[26]	 = "ACGO ENCENDIDO!";    //
		Alarm_Dispaly1[27]	 = "O② Apagado!";   //
		Alarm_Dispaly1[28]	 = "SpO② Apagado!";         //
		Alarm_Dispaly1[29]	 = "Calibrate O② Sensor!";//ylk add 20190124

		//麻醉气相关报警
		Alarm_Dispaly2[0]	 =  "FiHAL ALTO !!!";
		Alarm_Dispaly2[1]	 =  "FiENF ALTO !!!";
		Alarm_Dispaly2[2]	 =  "FiISO ALTO !!!";
		Alarm_Dispaly2[3]	 =  "FiSEV ALTO !!!";
		Alarm_Dispaly2[4]	 =	"FiDES ALTO !!!";
		Alarm_Dispaly2[5]	 =  "EtHAL ALTO !!!";
		Alarm_Dispaly2[6]	 =	 "EtENF ALTO !!!";
		Alarm_Dispaly2[7]	 =	 "EtISO ALTO !!!";
		Alarm_Dispaly2[8]	 =	 "EtSEV ALTO !!!";
		Alarm_Dispaly2[9]	 =	 "EtDES ALTO !!!";	

		Alarm_Dispaly2[10]	 =  "FiHAL BAJO !!";
		Alarm_Dispaly2[11]	 =  "FiENF BAJO !!";
		Alarm_Dispaly2[12]	 =  "FiISO BAJO !!";
		Alarm_Dispaly2[13]	 =  "FiSEV BAJO !!";
		Alarm_Dispaly2[14]	 =	"FiDES BAJO !!";
		Alarm_Dispaly2[15]	 =  "EtHAL BAJO !!";
		Alarm_Dispaly2[16]	 =	 "EtENF BAJO !!";
		Alarm_Dispaly2[17]	 =	 "EtISO BAJO !!";
		Alarm_Dispaly2[18]	 =	 "EtSEV BAJO !!";
		Alarm_Dispaly2[19]	 =	 "EtDES BAJO !!";

		Alarm_Dispaly2[20]	 =  "FiAA ALTO!!!";  //
		Alarm_Dispaly2[21]	 =  "EtAA ALTO!!!"; //
		Alarm_Dispaly2[22]	 =	"FiAA BAJO!!";  //
		Alarm_Dispaly2[23]	 =  "EtAA BAJO!!";  //	
#else
		Alarm_Dispaly[0]	 = "SUMINISTRO DE O② BAJO!!!";
		Alarm_Dispaly[1]	 = "BATERIA DESCARGADA!!!";
		Alarm_Dispaly[2]	 = "APNEA!!!";
		Alarm_Dispaly[3]	 = "PRESIóN ALTA CONTINUOSA!!!";
		Alarm_Dispaly[4]	 = "DESCONECCIóN DE CIRCUITO!!!";
		Alarm_Dispaly[5]	 = "VOLUMEN MINUTO BAJO!!!";
		Alarm_Dispaly[6]	 = "VOLUMEN MINUTO ALTO!!!";          
		Alarm_Dispaly[7]	 = "PRESIóN ALTA CONTINUOSA!!!";
		Alarm_Dispaly[8]	 = "TASA BAJA!!!";
		Alarm_Dispaly[9]	 = "FiO② ALTO!!!";
		Alarm_Dispaly[10]	 = "FALLA O② SENSOR?!!!"; 
		Alarm_Dispaly[11]	 = "FiO② BAJO!!!";
		Alarm_Dispaly[12]	 = "EtCO② ALTO!!!";
		Alarm_Dispaly[13]	 = "FiCO② ALTO!!!";

		Alarm_Dispaly[14]	 = "ESPERA ACTIVADA!!!";
		Alarm_Dispaly[15]	 =  "PRESIóN<-10cmH②O!!!"; 
		Alarm_Dispaly[16]	 =  "PRESIóN BAJA!!!";
		Alarm_Dispaly[17]	 =  "BATERIA BAJA!!!";
		Alarm_Dispaly[18]	 =  "SpO② BAJO!!!";
		Alarm_Dispaly[19]	 =  "FRECUENCIA DE PULSO BAJO!!!";//
		Alarm_Dispaly[20]	 = "VTe ALTO!!!";  

		Alarm_Dispaly1[0]	 =  "REGULACIóN DE PRESIóN LIMITADA!!";
		Alarm_Dispaly1[1]	 = "VTe BAJO!!";     
		Alarm_Dispaly1[2]	 = "FRECUENCIA ALTA!!";
		Alarm_Dispaly1[3]	 = "FRECUENCIA DE PULSO ALTO!!";
		Alarm_Dispaly1[4]	 = "Línea de módulo de gas de oclusión!!";//
		Alarm_Dispaly1[5]	 = "SpO② ALTO!!";
		Alarm_Dispaly1[6]	 = "EtCO② BAJO!!";
		Alarm_Dispaly1[7]	 = "FiN②O BAJO !!"; 

		Alarm_Dispaly1[8]	 =  "FALLA CORRIENTE!";
		Alarm_Dispaly1[9]	 = "SpO② SENSOR DESCONECTADO!";  

		Alarm_Dispaly1[10]	 = "EtCO② Apagado!";
		Alarm_Dispaly1[11]	 = "ACGO ENCENDIDO!";
		Alarm_Dispaly1[12]	 = "O② APAGADO!";
		Alarm_Dispaly1[13]	 = "SpO② APAGADO!";	            
#endif  
		break;
	case 3:
#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C)
		Alarm_Dispaly[0]	 =  "Нет подачи O②!!!";//氧气不足 

		Alarm_Dispaly[1]	 =  "Ошибка связи BDU!!!";//BDU通讯故障
		Alarm_Dispaly[2]	 =  "Отказ батареи!!!";//电池故障

		Alarm_Dispaly[3]	 =  "Батарея разряжена!!!";//电池耗尽  
		Alarm_Dispaly[4]	 =  "Апноэ!!!";//窒息 
		Alarm_Dispaly[5]	 =  "Быстрое нарастание давления!!!";//持续气道压力高!!! 
		Alarm_Dispaly[6]	 =  "Высокое давление!!!";//气道压力高 
		Alarm_Dispaly[7]	 =  "Рассоединение!!!";//患者未连接 
		Alarm_Dispaly[8]	 =  "Ошибка ПО газ. модуля!!!";
		Alarm_Dispaly[9]	 =  "Аппаратная ошибка газ. модуля!!!";
		Alarm_Dispaly[10]	 =  "Превыш. об. вент. газ. модуля!!!";
		Alarm_Dispaly[11]	 =  "Раскалибровка газ. модуля!!!";
		Alarm_Dispaly[12]	 =  "Замените адаптер газ. модуля!!!";
		Alarm_Dispaly[13]	 =  "Нет адаптера газ. модуля!!!";
		Alarm_Dispaly[14]	 =  "Нет линия газ. модуля!!!";
		Alarm_Dispaly[15]	 =  "Не указана точность измерения CO②!!!"; 
		Alarm_Dispaly[16]	 =  "Не указана точность измерения N②O!!!";  
		Alarm_Dispaly[17]	 =  "Не указ. точ-ть изм. анестетиков!!!";
		Alarm_Dispaly[18]	 =  "Не указана точность измерения O②!!!";
		Alarm_Dispaly[19]	 =  "Темп. газ. модуля вне уст. границ!!!";
		Alarm_Dispaly[20]	 =  "Давление вне установленых границ!!!";
		Alarm_Dispaly[21]	 =  "Недост-е показатели газ. анализа!!!";
		Alarm_Dispaly[22]	 =  "всего требуется O② калибровки!!!";
		Alarm_Dispaly[23]	 =  "";
		Alarm_Dispaly[24]	 =  "";
		Alarm_Dispaly[25]	 =  "Низкий мин. объем!!!";//分钟通气量低 
		Alarm_Dispaly[26]	 =  "Высокий мин. объем!!!";//分钟通气量高  
		Alarm_Dispaly[27]	 =  "Низкая ЧД!!!";//呼吸频率低 
		Alarm_Dispaly[28]	 =  "Высокий FiO②!!!";//氧浓度高  
		Alarm_Dispaly[29]	 =  "Провал датчика O②?!!!";//氧浓度传感器故障 
		Alarm_Dispaly[30]	 =  "Низкий FiO②!!!";
		Alarm_Dispaly[31]	 =  "Высокий EtCO②!!!" ;//呼末二氧化碳浓度高 
		Alarm_Dispaly[32]	 =  "Высокий FiCO②!!!";//吸入二氧化碳浓度高  
		Alarm_Dispaly[33]	 =  "Высокий FiN②O!!!";
		Alarm_Dispaly[34]	 =  "Режим STANDBY!!!";//待机模式激活 
		Alarm_Dispaly[35]	 =  "Давление < -10cmH②O!!!";//气道压力 < -10cmH②O 
		Alarm_Dispaly[36]	 =  "Низкое давление!!!";//气道压力低 
		Alarm_Dispaly[37]	 =  "Батарея разрядилась!!!"; //电池电量低 
		Alarm_Dispaly[38]	 =  "Ошибка измерения SpO②!!!";     //
		Alarm_Dispaly[39]	 =  "Низкая перфузия SpO②!!!";     //
		Alarm_Dispaly[40]	 =  "Поломка датчика SpO②!!!";     //
		Alarm_Dispaly[41]	 =  "Датчик SpO② не распознан!!!";     //
		Alarm_Dispaly[42]	 =  "Низкий уровень сигнала датчика SpO②!!!";     //
		Alarm_Dispaly[43]	 =  "Низкий уровень сатурации!!!";     //
		Alarm_Dispaly[44]	 =  "SpO② Низкий!!!";//SPO②低  
		Alarm_Dispaly[45]	 =  "Низкая ЧСС!!!";//脉率低 
		Alarm_Dispaly[46]	 =  "Высокий объем выдоха!!!";//潮气量高 
		/***************************ylk add 20181205***************************/
		Alarm_Dispaly[47]	 =  "Модуль AA осушителя полный!!!";     
		Alarm_Dispaly[48]	 =  "модуль отключения AA осушителя!!!";     
		Alarm_Dispaly[49]	 =  "AA Ошибка питания на входе!!!";     
		Alarm_Dispaly[50]	 =  "Неисправность датчика CO②!!!";     
		Alarm_Dispaly[51]	 =  "Неиспр. датчик О②!!!";     
		Alarm_Dispaly[52]	 =  "сбой модуля AA!!!";     
		/**********************************************************************/

		Alarm_Dispaly1[0]	 =  "Нарушение подачи воздуха!!";//空气不足    //djq add 20170407   
		Alarm_Dispaly1[1]	 =  "Нет абсорбера!!";//二氧化碳吸收罐移除  
		Alarm_Dispaly1[2]	 =  "Ограничение давления!!" ;//未达到目标潮气量  
		Alarm_Dispaly1[3]	 =  "Низкий объем выдоха!!";//潮气量低    
		Alarm_Dispaly1[4]	 =  "Высокая ЧД!!";// 呼吸频率高 
		Alarm_Dispaly1[5]	 =  "Высокий ЧСС!!";//脉率高 
		Alarm_Dispaly1[6]	 =  "Помехи датчика SpO②!!";         //
		Alarm_Dispaly1[7]	 =  "Высокая освещенность датчика SpO②!!";         //
		Alarm_Dispaly1[8]	 =  "Рассоединение кабеля датчика SpO②!!";         //;
		Alarm_Dispaly1[9]	 =  "газ модуль линии окклюзия!!";//二氧化碳采样管阻塞 
		Alarm_Dispaly1[10]	 =  "AA Модуль влагоуловителя будет полным!!";//ylk add 201812105 脱水器将满
		Alarm_Dispaly1[11]	 =  "Высокий перфузионный индекс SpO②!!";    //
		Alarm_Dispaly1[12]	 =  "SpO② Высокий!!";//SPO② 高  
		Alarm_Dispaly1[13]	 =  "Низкий EtCO②!!";//呼末二氧化碳浓度低  
		Alarm_Dispaly1[14]	 =  "";//FiAA LOW
		Alarm_Dispaly1[15]	 =  "";//EtAA LOW
		Alarm_Dispaly1[16]	 =  "Низкий FiN②O!!";

		Alarm_Dispaly1[17]	 =  "Общий уровень MAC>=3!!";
		Alarm_Dispaly1[18]	 =  "Некоррект. обнул-е газ. модуля!!";
		Alarm_Dispaly1[19]	 =  "AGENT OVERFLOW!!"; //ylk add 20181205
		Alarm_Dispaly1[20]	 =  "Отказ батареи!";//电池故障

		Alarm_Dispaly1[21]	 =  "Общий уровень MAC<3!";
		Alarm_Dispaly1[22]	 =  "Отказ сети!";//交流电故障 
		Alarm_Dispaly1[23]	 =  "Отсоединение сенсора SpO②!";//"SPO② 传感器未连接   
		Alarm_Dispaly1[24]	 =	"Отсоединение сенсора SpO② от пациента?!";// 
		if(EtCO2_Config_On || EtCO2_KM7002_Config_On)
		{
			Alarm_Dispaly1[25] = "Измерение EtCO② выкл.!";//二氧化碳监测模块关闭 
		}
		else if((Masimo_EtCO2_Config_On) && (AA_Module_Flag.PhaseInTypeModuleExist == 4))
		{			
			Alarm_Dispaly1[25] = "Измерение EtCO② выкл.!";//二氧化碳监测模块关闭			
		}
		else
		{
			Alarm_Dispaly1[25] = "Измерение AA выкл.!";//AA监测模块关闭
		}
		Alarm_Dispaly1[26]	 =  "ACGO вкл.!";//
		Alarm_Dispaly1[27]	 =  "Измерение О② выкл.!";//
		Alarm_Dispaly1[28]	 =  "Измерение SpO② выкл.!";         //
		Alarm_Dispaly1[29]	 = "Calibrate O② Sensor!";//ylk add 20190124

		//麻醉气相关报警
		Alarm_Dispaly2[0]	 =  "Высокий FiHAL!!!";   //
		Alarm_Dispaly2[1]	 =  "Высокий FiENF!!!";  //
		Alarm_Dispaly2[2]	 =  "Высокий FiISO!!!";  //
		Alarm_Dispaly2[3]	 =  "Высокий FiSEV!!!";  //
		Alarm_Dispaly2[4]	 =	"Высокий FiDES!!!";	//

		Alarm_Dispaly2[5]	 =  "Высокий EtHAL!!!";		//						
		Alarm_Dispaly2[6]	 =  "Высокий EtENF!!!"; //
		Alarm_Dispaly2[7]	 =  "Высокий EtISO!!!"; //
		Alarm_Dispaly2[8]	 =  "Высокий EtSEV!!!"; //
		Alarm_Dispaly2[9]	 =  "Высокий EtDES!!!"; //

		Alarm_Dispaly2[10]	 =  "Низкий FiHAL!!";
		Alarm_Dispaly2[11]	 =  "Низкий FiENF!!";
		Alarm_Dispaly2[12]	 =  "Низкий FiISO!!";
		Alarm_Dispaly2[13]	 =  "Низкий FiSEV!!";
		Alarm_Dispaly2[14]	 =  "Низкий FiDES!!"; 
		Alarm_Dispaly2[15]	 =  "Низкий EtHAL!!";
		Alarm_Dispaly2[16]	 =  "Низкий EtENF!!";
		Alarm_Dispaly2[17]	 =  "Низкий EtISO!!";
		Alarm_Dispaly2[18]	 =  "Низкий EtSEV!!";
		Alarm_Dispaly2[19]	 =  "Низкий EtDES!!";

		Alarm_Dispaly2[20]	 =  "Высокий FiAA!!!";  //
		Alarm_Dispaly2[21]	 =  "Высокий EtAA!!!"; //
		Alarm_Dispaly2[22]	 =	"Низкий FiAA!!";  //
		Alarm_Dispaly2[23]	 =  "Низкий EtAA!!";  //	
#else
		Alarm_Dispaly[0]	 =  "Нет подачи O②!!!";//氧气不足 
		Alarm_Dispaly[1]	 =  "Батарея разряжена!!!";//电池耗尽  
		Alarm_Dispaly[2]	 =  "Апноэ!!!";//窒息 
		Alarm_Dispaly[3]	 =  "Высокое давление!!!";//气道压力高 
		Alarm_Dispaly[4]	 =  "Рассоединение!!!";//患者未连接 
		Alarm_Dispaly[5]	 =  "Низкий мин. объем!!!";//分钟通气量低 
		Alarm_Dispaly[6]	 =  "Высокий мин. объем!!!";//分钟通气量高        
		Alarm_Dispaly[7]	 =  "Быстрое нарастание давления!!!";//持续气道压力高!!! 
		Alarm_Dispaly[8]	 =  "Низкая ЧД!!!";//呼吸频率低 
		Alarm_Dispaly[9]	 =  "Высокий FiO②!!!";//氧浓度高  
		Alarm_Dispaly[10]	 =  "Неиспр. датчик O②?!!!";//"Провал датчика O②?!!!";//氧浓度传感器故障 
		Alarm_Dispaly[11]	 =  "Низкий FiO②!!!";//氧浓度低 
		Alarm_Dispaly[12]	 =  "Высокий EtCO②!!!" ;//呼末二氧化碳浓度高 
		Alarm_Dispaly[13]	 =  "Высокий FiCO②!!!";//吸入二氧化碳浓度高  

		Alarm_Dispaly[14]	 =  "Режим STANDBY!!!";//待机模式激活 
		Alarm_Dispaly[15]	 =  "Давление < -10cmH②O!!!";//气道压力 < -10cmH②O 
		Alarm_Dispaly[16]	 =  "Низкое давление!!!";//气道压力低 
		Alarm_Dispaly[17]	 =  "Батарея разрядилась!!!"; //电池电量低 
		Alarm_Dispaly[18]	 =  "SpO② Низкий!!!";//SPO②低  
		Alarm_Dispaly[19]	 =  "Низкая ЧСС!!!";//脉率低
		Alarm_Dispaly[20]	 =  "Высокий объем выдоха!!!";//潮气量高  

		Alarm_Dispaly1[0]	 = "Ограничение давления!!" ;//未达到目标潮气量  
		Alarm_Dispaly1[1]	 =  "Низкий объем выдоха!!";//潮气量低     
		Alarm_Dispaly1[2]	 =  "Высокая ЧД!!";// 呼吸频率高 
		Alarm_Dispaly1[3]	 =  "Высокий ЧСС!!";//脉率高   //
		Alarm_Dispaly1[4]	 =  "газ модуль линии окклюзия!!";//二氧化碳采样管阻塞 
		Alarm_Dispaly1[5]	 =  "SpO② Высокий!!";//SPO② 高  
		Alarm_Dispaly1[6]	 =  "Низкий EtCO②!!";//呼末二氧化碳浓度低  
		Alarm_Dispaly1[7]	 =  "Низкий FiN②O!!";

		Alarm_Dispaly1[8]	 =  "ОТКАЗ СЕТИ!";//交流电故障 
		Alarm_Dispaly1[9]	 =  "Отсоединение сенсора SpO②!";//"SPO② 传感器未连接 

		Alarm_Dispaly1[10]	 =  "Измерение EtCO② выкл.!";//ACGO 开启 
		Alarm_Dispaly1[11]	 =  "ACGO вкл.!";//ACGO 开启 
		Alarm_Dispaly1[12]	 =  "Измерение О② выкл.!";//氧浓度监测模块关闭  //
		Alarm_Dispaly1[13]	 =  "Измерение SpO② выкл.!";//SPO② 关           //	
#endif  
		break;
#ifdef LANGUAGE_TURKISH_FLAG
	case 4:
		Alarm_Dispaly[0]	 =  "O② SUPPLY DOWN!!!";

		Alarm_Dispaly[1]	 =  "BDU Communication Failure!!!";//
		Alarm_Dispaly[2]	 =  "BATTERY FAILURE!!!";//

		Alarm_Dispaly[3]	 =  "BATTERY DISCHARGED!!!";
		Alarm_Dispaly[4]	 =  "APNEA!!!";
		Alarm_Dispaly[5]	 =  "CONTINUOUS PRESSURE HIGH!!!";
		Alarm_Dispaly[6]	 =  "PRESSURE HIGH!!!";
		Alarm_Dispaly[7]	 =  "CIRCUIT DISCONNECT!!!";
		Alarm_Dispaly[8]	 =  "Gas Module Software Error!!!";
		Alarm_Dispaly[9]	 =  "Gas Module Hardware Error!!!";
		Alarm_Dispaly[10]	 =  "Gas Module Motor Overspeed!!!";
		Alarm_Dispaly[11]	 =  "Gas Module Factory calib lost!!!";
		Alarm_Dispaly[12]	 =  "Gas Module Replace Adapter!!!";
		Alarm_Dispaly[13]	 =  "Gas Module No Adapter!!!";
		Alarm_Dispaly[14]	 =  "Gas module No Line!!!";
		Alarm_Dispaly[15]	 =  "CO② Unspecified Accuracy!!!"; 
		Alarm_Dispaly[16]	 =  "N②O Unspecified Accuracy!!!";  
		Alarm_Dispaly[17]	 =  "AA Unspecified Accuracy!!!";
		Alarm_Dispaly[18]	 =  "O② Unspecified Accuracy!!!";
		Alarm_Dispaly[19]	 =  "Gas Module Temp Out Of Range!!!";
		Alarm_Dispaly[20]	 =  "Pressure Out Of Range!!!";
		Alarm_Dispaly[21]	 =  "AA ID Unreliable!!!";
		Alarm_Dispaly[22]	 =  "O② span calibration required!!!";
		
		Alarm_Dispaly[23]	 =  "";
		Alarm_Dispaly[24]	 =  "";
		Alarm_Dispaly[25]	 =  "MV LOW!!!";
		Alarm_Dispaly[26]	 =  "MV HIGH!!!";  

		Alarm_Dispaly[27]	 =  "RATE LOW!!!"; 
		Alarm_Dispaly[28]	 =  "FiO② HIGH!!!";
		Alarm_Dispaly[29]	 =  "O② SENSǒRú YOK?!!!"; 
		Alarm_Dispaly[30]	 =  "FiO② LOW!!!";
		Alarm_Dispaly[31]	 =  "ETCO② HIGH!!!";
		Alarm_Dispaly[32]	 =  "FiCO② HIGH!!!";
		Alarm_Dispaly[33]	 =  "FiN②O HIGH !!!";
		Alarm_Dispaly[34]	 =  "STANDBY ACTIVED!!!";
		Alarm_Dispaly[35]	 =  "PRESSURE<-10cmH②O!!!"; 
		Alarm_Dispaly[36]	 =  "PRESSURE LOW!!!";
		Alarm_Dispaly[37]	 =  "BATTERY LOW!!!";
		Alarm_Dispaly[38]	 =  "SPO② FAILURE !!!";     //
		Alarm_Dispaly[39]	 =  "SPO② LOW PERFUSION !!!";     //
		Alarm_Dispaly[40]	 =  "SPO② DEFECTIVE SENSOR !!!";     //
		Alarm_Dispaly[41]	 =  "SPO② UNRECOGNIZED SENSOR !!!";     //
		Alarm_Dispaly[42]	 =  "SPO② LOW SIGNAL IQ !!!";     //
		Alarm_Dispaly[43]	 =  "SPO② PI LOW !!!";     //
		Alarm_Dispaly[44]	 =  "SpO② LOW!!!";
		Alarm_Dispaly[45]	 =  "PULSE RATE LOW!!!";
		Alarm_Dispaly[46]	 =  "VTe HIGH!!!";       
		/***************************ylk add 20181205***************************/
		Alarm_Dispaly[47]	 =  "AA Module Watertrap Full!!!";     
		Alarm_Dispaly[48]	 =  "Check AA Module Watertrap or Sampling Line!!!";     
		Alarm_Dispaly[49]	 =  "AA Module Power Input Failure!!!";     
		Alarm_Dispaly[50]	 =  "CO② Sensor Failure!!!";     
		Alarm_Dispaly[51]	 =  "O② Sensor Failure!!!";     
		Alarm_Dispaly[52]	 =  "AA Module Failure!!!";     
		/**********************************************************************/

		Alarm_Dispaly1[0]	 =  "AIR SUPPLY DOWN!!";//djq add 20170407      
		Alarm_Dispaly1[1]	 =  "NO ABSORBER!!"; 
		Alarm_Dispaly1[2]	 =  "REGULATION PRESSURE LIMITED!!";
		Alarm_Dispaly1[3]	 =  "VTe LOW!!";      
		Alarm_Dispaly1[4]	 =  "RATE HIGH!!";
		Alarm_Dispaly1[5]	 =  "PULSE RATE HIGH!!";       //
		Alarm_Dispaly1[6]	 =  "SPO② INTERFERENCE DETECTED !!";         //
		Alarm_Dispaly1[7]	 =  "SPO② TOO MUCH AMBIENT LIGHT !!";         //
		Alarm_Dispaly1[8]	 =  "SPO② CABLE NO CONNECTED !!";         //;
		Alarm_Dispaly1[9]	 =  "Gas Module Line Occlusion!!";
		Alarm_Dispaly1[10]	 =  "AA Module Dehydrator almost Full!!";//ylk add 201812105 脱水器将满
		Alarm_Dispaly1[11]	 =  "SPO② PI HIGH !!";    //
		Alarm_Dispaly1[12]	 =  "SpO② HIGH!!";
		Alarm_Dispaly1[13]	 =  "ETCO② LOW!!";
		Alarm_Dispaly1[14]	 =  "";//FiAA LOW
		Alarm_Dispaly1[15]	 =  "";//EtAA LOW
		Alarm_Dispaly1[16]	 =  "FiN②O LOW!!";
		Alarm_Dispaly1[17]	 =  "Mixed anesthetic MAC>=3!!";
		Alarm_Dispaly1[18]	 =  "Gas Module Inaccurate Gas Zeroing!!";
		Alarm_Dispaly1[19]	 =  "AGENT OVERFLOW!!"; //ylk add 20181205
		Alarm_Dispaly1[20]	 =  "BATTERY FAILURE!";//电池故障

		Alarm_Dispaly1[21]	 =  "Mixed anesthetic MAC<3!";
		Alarm_Dispaly1[22]	 =  "MAINS FAILURE!";
		Alarm_Dispaly1[23]	 =  "SpO② SENSOR DISCONNECT!";    
		Alarm_Dispaly1[24]	 =  "SPO② SENSOR OFF PATIENT?!";// 
		if(EtCO2_Config_On || EtCO2_KM7002_Config_On)
		{
			Alarm_Dispaly1[25]	= "ETCO② OFF!" ;
		}
		else if((Masimo_EtCO2_Config_On) && (AA_Module_Flag.PhaseInTypeModuleExist == 4))
		{		
			Alarm_Dispaly1[25]	= "ETCO② OFF!" ;			
		}
		else
		{
			Alarm_Dispaly1[25]	= "AA OFF!"; 
		}
		Alarm_Dispaly1[26]	 =  "ACGO OPEN!";     //
		Alarm_Dispaly1[27]	 =  "O② OFF!";    //
		Alarm_Dispaly1[28]	 =  "SpO② OFF!";           //
		Alarm_Dispaly1[29]	 = "Calibrate O② Sensor!";//ylk add 20190124

		//麻醉气相关报警
		Alarm_Dispaly2[0]	 =  "FiHAL HIGH!!!";   //
		Alarm_Dispaly2[1]	 =  "FiENF HIGH!!!";  //
		Alarm_Dispaly2[2]	 =  "FiISO HIGH!!!";  //
		Alarm_Dispaly2[3]	 =  "FiSEV HIGH!!!";  //
		Alarm_Dispaly2[4]	 =	"FiDES HIGH!!!";	//

		Alarm_Dispaly2[5]	 =  "EtHAL HIGH!!!";		//						
		Alarm_Dispaly2[6]	 =  "EtENF HIGH!!!"; //
		Alarm_Dispaly2[7]	 =  "EtISO HIGH!!!"; //
		Alarm_Dispaly2[8]	 =  "EtSEV HIGH!!!"; //
		Alarm_Dispaly2[9]	 =  "EtDES HIGH!!!"; //

		Alarm_Dispaly2[10]	 =  "FiHAL LOW!!";
		Alarm_Dispaly2[11]	 =  "FiENF LOW!!";
		Alarm_Dispaly2[12]	 =  "FiISO LOW!!";
		Alarm_Dispaly2[13]	 =  "FiSEV LOW!!";
		Alarm_Dispaly2[14]	 =  "FiDES LOW!!"; 
		Alarm_Dispaly2[15]	 =  "EtHAL LOW!!";
		Alarm_Dispaly2[16]	 =  "EtENF LOW!!";
		Alarm_Dispaly2[17]	 =  "EtISO LOW!!";
		Alarm_Dispaly2[18]	 =  "EtSEV LOW!!";
		Alarm_Dispaly2[19]	 =  "EtDES LOW!!";

		Alarm_Dispaly2[20]	 =  "FiAA HIGH!!!";  //
		Alarm_Dispaly2[21]	 =  "EtAA HIGH!!!"; //
		Alarm_Dispaly2[22]	 =	"FiAA LOW!!";  //
		Alarm_Dispaly2[23]	 =  "EtAA LOW!!";  //
		break;
#endif
#endif
	default:
		break;
	}
}