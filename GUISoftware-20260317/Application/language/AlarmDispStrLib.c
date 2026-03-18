#include "AlarmDispStrLib.h"
#include "CO2_IRMA.h"

const char* AlarmContent[ALARM_MAXNUM] = {0};
const char* AlarmContent1[ALARM_MAXNUM] = {0};
const char* AlarmContent2[ALARM_AA_MAXNUM] = {0};

extern unsigned char EtCO2_Config_On;
extern unsigned char Masimo_EtCO2_Config_On;
extern unsigned char EtCO2_KM7002_Config_On;
extern unsigned char MasimoISA_EtCO2_Config_On; //0:关闭   1：开启
extern unsigned char Drager_EtCO2_Config_On; //0:关闭   1：开启
extern IM_ALL_DATA      * pAll_Data;

extern unsigned char Masimo_EtCO2_zhuliu;
extern unsigned char Masimo_EtCO2_zhuliuco2;
extern unsigned char MasimoISA_EtCO2_pangliuO2;
extern unsigned char MasimoISA_EtCO2_pangliu;
extern unsigned char Masimo_EtCO2_pangliuco2;

void InitAlarmInfoRegister(void)
{
	//高级报警
	AlarmContent[0] = Sys_Lib_GetDispStr(DP_O2SUPPLYDOWN);
	AlarmContent[1] = Sys_Lib_GetDispStr(DP_BDUCOMMFAIL);
	AlarmContent[2] = Sys_Lib_GetDispStr(DP_BATTRERYFAIL);
	AlarmContent[3] = Sys_Lib_GetDispStr(DP_BATTERYDISCHAGE);
	AlarmContent[4] = Sys_Lib_GetDispStr(DP_APNEA);
	AlarmContent[5] = Sys_Lib_GetDispStr(DP_CONTINUOUSPRESSUREHIGH);
	AlarmContent[6] = Sys_Lib_GetDispStr(DP_PRESSUREHIGH);
	AlarmContent[7] = Sys_Lib_GetDispStr(DP_CIRCUITDISCONNECT);

	AlarmContent[10] = Sys_Lib_GetDispStr(DP_MVLOW);
	AlarmContent[11] = Sys_Lib_GetDispStr(DP_MVHIGH);
	AlarmContent[12] = Sys_Lib_GetDispStr(DP_RATELOW);
	AlarmContent[13] = Sys_Lib_GetDispStr(DP_O2LESS18ANDFAIL);//20210729 FiO2 < 18%
	AlarmContent[14] = Sys_Lib_GetDispStr(DP_FIO2HIGH);
	AlarmContent[15] = Sys_Lib_GetDispStr(DP_O2FAIL);
	AlarmContent[16] = Sys_Lib_GetDispStr(DP_FIO2LOW);
	AlarmContent[17] = Sys_Lib_GetDispStr(DP_ETCO2HIGH);
	AlarmContent[18] = Sys_Lib_GetDispStr(DP_FICO2HIGH);
	AlarmContent[19] = Sys_Lib_GetDispStr(DP_FIN2OHIGH);
	AlarmContent[20] = Sys_Lib_GetDispStr(DP_STANDBYACTIVED);
	AlarmContent[21] = Sys_Lib_GetDispStr(DP_PRESSURELESS10);
	AlarmContent[22] = Sys_Lib_GetDispStr(DP_PRESSURELOW);
	AlarmContent[23] = Sys_Lib_GetDispStr(DP_BATTERYLOW);
	AlarmContent[24] = Sys_Lib_GetDispStr(DP_SPO2FAIL);
	AlarmContent[25] = Sys_Lib_GetDispStr(DP_SPO2LOWPERFUSION);
	AlarmContent[26] = Sys_Lib_GetDispStr(DP_SPO2DEFECTIVESENSOR);
	AlarmContent[27] = Sys_Lib_GetDispStr(DP_SPO2UNRECOGNIZEDSENSOR);
	AlarmContent[28] = Sys_Lib_GetDispStr(DP_SPO2LOWSIGNALIQ); 
	AlarmContent[29] = Sys_Lib_GetDispStr(DP_SPO2PILOW);
	AlarmContent[30] = Sys_Lib_GetDispStr(DP_SPO2LOW);
	AlarmContent[31] = Sys_Lib_GetDispStr(DP_PULSERATELOW);
	AlarmContent[32] = Sys_Lib_GetDispStr(DP_VTEHIGH);
	AlarmContent[33] = Sys_Lib_GetDispStr(DP_AAWTFULL);
	AlarmContent[34] = Sys_Lib_GetDispStr(DP_CHKAAWTLINE);
	AlarmContent[35] = Sys_Lib_GetDispStr(DP_AAMODULEPOWERINPUTFAIL);
	AlarmContent[36] = Sys_Lib_GetDispStr(DP_CO2SENSORFAIL);
	AlarmContent[37] = Sys_Lib_GetDispStr(DP_O2FAIL);
	AlarmContent[38] = Sys_Lib_GetDispStr(DP_AAMODULEFAIL);

	//中级和低级报警
	AlarmContent1[0] = Sys_Lib_GetDispStr(DP_AIRSUPPLYDOWN);
	AlarmContent1[1] = Sys_Lib_GetDispStr(DP_NOABSORBER);
	AlarmContent1[2] = Sys_Lib_GetDispStr(DP_REGULATIONPRESSURELIMIT);
	AlarmContent1[3] = Sys_Lib_GetDispStr(DP_VTELOW);
	AlarmContent1[4] = Sys_Lib_GetDispStr(DP_RATEHIGH);
	AlarmContent1[5] = Sys_Lib_GetDispStr(DP_PULSHRATEHIGH);

	AlarmContent1[6] = Sys_Lib_GetDispStr(DP_GASMODULESOFTWAREERROR);
	AlarmContent1[7] = Sys_Lib_GetDispStr(DP_GASMODULEHARDWAREERROR);
	AlarmContent1[8] = Sys_Lib_GetDispStr(DP_GASMODULEMOTOROVERSPEED);
	AlarmContent1[9] = Sys_Lib_GetDispStr(DP_GASMODULEFACTORYCALIB);
	AlarmContent1[10] = Sys_Lib_GetDispStr(DP_GASMODULEREPLACEADAPTER);
	AlarmContent1[11] = Sys_Lib_GetDispStr(DP_GASMODULENOADAPTER);
	AlarmContent1[12] = Sys_Lib_GetDispStr(DP_GASMODULENOLINE);
	AlarmContent1[13] = Sys_Lib_GetDispStr(DP_CO2UNSPECIFIEDACCURACY);
	AlarmContent1[14] = Sys_Lib_GetDispStr(DP_N2OUNSPECIFIEDACCURACY);
	AlarmContent1[15] = Sys_Lib_GetDispStr(DP_AAUNSPECIFIEDACCURACY);
	AlarmContent1[16] = Sys_Lib_GetDispStr(DP_O2UNSPEACCURACY);
	AlarmContent1[17] = Sys_Lib_GetDispStr(DP_GASMODULETEMPOVERRANGE);
	AlarmContent1[18] = Sys_Lib_GetDispStr(DP_PRESSUREOVERRANGE);
	AlarmContent1[19] = Sys_Lib_GetDispStr(DP_AAIDUNRELIABLE);
	AlarmContent1[20] = Sys_Lib_GetDispStr(DP_O2SPANCALREQUIRED);
	AlarmContent1[21] = Sys_Lib_GetDispStr(DP_O2SENSORERROR);//O2 Sensor error
	AlarmContent1[22] = Sys_Lib_GetDispStr(DP_REPLACEO2SENSOR);//Replace O2 Sensor 

	AlarmContent1[23] = Sys_Lib_GetDispStr(DP_SPO2INTERFERENCEDETE);
	AlarmContent1[24] = Sys_Lib_GetDispStr(DP_SPO2MUCHAMBIENTLIGHT);
	AlarmContent1[25] = Sys_Lib_GetDispStr(DP_SPO2CABLENOTCONNECT);
	AlarmContent1[26] = Sys_Lib_GetDispStr(DP_GASMODULELINEOCC);
	AlarmContent1[27] = Sys_Lib_GetDispStr(DP_AAMODULEDEHYDRATORALMOST);
	AlarmContent1[28] = Sys_Lib_GetDispStr(DP_SPO2PIHIGH);

	AlarmContent1[29] = Sys_Lib_GetDispStr(DP_SPO2HIGH);
	AlarmContent1[30] = Sys_Lib_GetDispStr(DP_ETCO2LOW);
	//AlarmContent1[14] = Sys_Lib_GetDispStr(DP_ETAALOW);
	//AlarmContent1[15] = Sys_Lib_GetDispStr(DP_FIAALOW);
	AlarmContent1[33] = Sys_Lib_GetDispStr(DP_FIN2OLOW);
	AlarmContent1[34] = Sys_Lib_GetDispStr(DP_MACMORETHAN3);//MAC > 3
	AlarmContent1[35] = Sys_Lib_GetDispStr(DP_GASMODULEINACCGASZERO);
	AlarmContent1[36] = Sys_Lib_GetDispStr(DP_AGENTOVERFLOW);
	AlarmContent1[37] = Sys_Lib_GetDispStr(DP_BATTERYFAILURE);
	AlarmContent1[38] = Sys_Lib_GetDispStr(DP_MIXANESTHEICMAC);//MAC <3
	AlarmContent1[39] = Sys_Lib_GetDispStr(DP_GASCOMMFAIL);
	AlarmContent1[40] = Sys_Lib_GetDispStr(DP_MAINSFAIL);	
	AlarmContent1[41] = Sys_Lib_GetDispStr(DP_SPO2SENSORDISCON);
	AlarmContent1[42] = Sys_Lib_GetDispStr(DP_SPO2SENSOROFFPATIENT);
	
    
    if(Masimo_EtCO2_Config_On||MasimoISA_EtCO2_Config_On)           //开通主流选配或旁流或全开通   //gyh add 20230727 多气体和CO2模块关闭报警字符显示
    {
        if(AA_Module_Flag.PhaseInTypeModuleExist == Masimo_EtCO2_zhuliu)      //主流多气体
        {
            AlarmContent1[43] = Sys_Lib_GetDispStr(DP_AAOFF);
        }
        else if(AA_Module_Flag.PhaseInTypeModuleExist == Masimo_EtCO2_zhuliuco2)      //主流CO2
        {
            AlarmContent1[43] = Sys_Lib_GetDispStr(DP_CO2OFF);
        }
        else if(AA_Module_Flag.PhaseInTypeModuleExist == Masimo_EtCO2_pangliuco2)     //旁流CO2
        {
            AlarmContent1[43] = Sys_Lib_GetDispStr(DP_CO2OFF);
        }
        else if((AA_Module_Flag.PhaseInTypeModuleExist == MasimoISA_EtCO2_pangliuO2)       //旁流多气体
                ||(AA_Module_Flag.PhaseInTypeModuleExist == MasimoISA_EtCO2_pangliu))
        {
            AlarmContent1[43] = Sys_Lib_GetDispStr(DP_AAOFF);
        }
        else                                                                  //默认显示AA
        {
            AlarmContent1[43] = Sys_Lib_GetDispStr(DP_AAOFF);
        }
    }
    else                                                                            //金嘉信CO2选配
    {
        AlarmContent1[43] = Sys_Lib_GetDispStr(DP_CO2OFF);
          }
        
        

	/*if((Masimo_EtCO2_Config_On) && ((AA_Module_Flag.PhaseInTypeModuleExist == 1)||(AA_Module_Flag.PhaseInTypeModuleExist == 2)||(AA_Module_Flag.PhaseInTypeModuleExist == 3))			//gyh add 20230706 麻醉多气体模块开通正常监测
		&&(pAll_Data->slow.VerVal.sen_type & 0xF8)||(MasimoISA_EtCO2_Config_On))//ylk md 20190611
	{
		AlarmContent1[43] = Sys_Lib_GetDispStr(DP_AAOFF);	
	}
	else
	{
		AlarmContent1[43] = Sys_Lib_GetDispStr(DP_CO2OFF); 
	}*/
	AlarmContent1[44] = Sys_Lib_GetDispStr(DP_ACGOON);
	AlarmContent1[45] = Sys_Lib_GetDispStr(DP_O2OFF);
	AlarmContent1[46] = Sys_Lib_GetDispStr(DP_SPO2OFF);

	AlarmContent2[0] = Sys_Lib_GetDispStr(DP_FIHALHIGH);
	AlarmContent2[1] = Sys_Lib_GetDispStr(DP_FIENFHIGH);
	AlarmContent2[2] = Sys_Lib_GetDispStr(DP_FIISOHIGH);
	AlarmContent2[3] = Sys_Lib_GetDispStr(DP_FISEVHIGH);
	AlarmContent2[4] = Sys_Lib_GetDispStr(DP_FIDESHIGH);

	AlarmContent2[5] = Sys_Lib_GetDispStr(DP_ETHALHIGH);
	AlarmContent2[6] = Sys_Lib_GetDispStr(DP_ETENFHIGH);
	AlarmContent2[7] = Sys_Lib_GetDispStr(DP_ETISOHIGH);
	AlarmContent2[8] = Sys_Lib_GetDispStr(DP_ETSEVHIGH);
	AlarmContent2[9] = Sys_Lib_GetDispStr(DP_ETDESHIGH);

	AlarmContent2[10] = Sys_Lib_GetDispStr(DP_FIHALLOW);
	AlarmContent2[11] = Sys_Lib_GetDispStr(DP_FIENFLOW);
	AlarmContent2[12] = Sys_Lib_GetDispStr(DP_FIISOLOW);
	AlarmContent2[13] = Sys_Lib_GetDispStr(DP_FISEVLOW);
	AlarmContent2[14] = Sys_Lib_GetDispStr(DP_FIDESLOW);

	AlarmContent2[15] = Sys_Lib_GetDispStr(DP_ETHALLOW);
	AlarmContent2[16] = Sys_Lib_GetDispStr(DP_ETENFLOW);
	AlarmContent2[17] = Sys_Lib_GetDispStr(DP_ETISOLOW);
	AlarmContent2[18] = Sys_Lib_GetDispStr(DP_ETSEVLOW);
	AlarmContent2[19] = Sys_Lib_GetDispStr(DP_ETDESLOW);

	AlarmContent2[20] = Sys_Lib_GetDispStr(DP_FIAAHIGH);
	AlarmContent2[21] = Sys_Lib_GetDispStr(DP_EIAAHIGH);
	AlarmContent2[22] = Sys_Lib_GetDispStr(DP_FIAALOW);
	AlarmContent2[23] = Sys_Lib_GetDispStr(DP_ETAALOW);
}