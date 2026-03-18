#include "MainTask.h"
#include "Dispstringlib.h"
#include "DispLg.h"
#include "Static_Menu_Creat.h"

Disp_Lg_t LanguageType = Disp_Lg_Russian;
//tcc add 20220524 
const unsigned int MAC_Ratio[MAC_NUM] = {
	10000,	  12732,    12654,    12576,    12498,    12421,    12344,    12268,    12192,
	12117,    12042,    11968,    11894,    11820,    11747,    11675,    11603,    11531,
	11460,    11389,    11319,    11249,    11179,    11110,    11042,    10974,    10906,
	10839,    10772,    10705,    10639,    10573,    10508,    10443,    10379,    10315,
	10251,    10188,    10125,    10062,    10000,     9938,     9877,     9816,     9755,
	9695,     9635,     9576,     9517,     9458,     9399,     9341,     9284,     9226,
	9169,     9113,     9056,     9001,     8945,     8890,     8835,     8780,     8726,
	8672,     8619,     8565,     8513,     8460,     8408,     8356,     8304,     8253,
	8202,     8151,     8101,     8051,     8001,     7952,     7903,     7854,     7805
};
const char *cbdialog32string[DlgStr_MAXNUM];
extern unsigned char VT_Min_Flag;
void Initcbdialog32string(Patient_Info_t *tmp_Info, int mode)
{
	if(tmp_Info->Patient_Type == PATIENTTYPE_ADULT)
	{
		cbdialog32string[VT_1600] = Sys_Lib_GetDispStr(DP_ADULTVTLESS1600);
	}
	else if(tmp_Info->Patient_Type == PATIENTTYPE_PAEDIATRIC)
	{
#ifdef LANGUAGE_CHINESE_FLAG
		cbdialog32string[VT_1600] = Sys_Lib_GetDispStr(DP_CHILVTLESS400);
#else
		cbdialog32string[VT_1600] = Sys_Lib_GetDispStr(DP_CHILVTLESS360);
#endif	
	}
	else
	{
#ifdef LANGUAGE_CHINESE_FLAG
		cbdialog32string[VT_1600] = Sys_Lib_GetDispStr(DP_INFANTVTLESS200);
#else
		cbdialog32string[VT_1600] = Sys_Lib_GetDispStr(DP_INFANTVTLESS100);
#endif
	}
	
	cbdialog32string[PINSP_70] = Sys_Lib_GetDispStr(DP_PINSPLESS70CMH2O);
	cbdialog32string[IE_41] = Sys_Lib_GetDispStr(DP_IELESSTHAN41);
	cbdialog32string[TINSP_10] = Sys_Lib_GetDispStr(DP_TINSPLESS10S);
	cbdialog32string[TINSP_TPAUSE_10] = Sys_Lib_GetDispStr(DP_TINSPTPAUSELESS10S);//未用到
	cbdialog32string[PSUPP_70] = Sys_Lib_GetDispStr(DP_PSUPPLESS70CMH2O);
	cbdialog32string[SIMV_FREQ_40] = Sys_Lib_GetDispStr(DP_SIMVFREQLESS40BPM);
	cbdialog32string[FREQ_100] = Sys_Lib_GetDispStr(DP_FREQLESS100BPM);
	cbdialog32string[PEEP_30] = Sys_Lib_GetDispStr(DP_PEEPLESS30CMH2O);
	cbdialog32string[PAUSE_60] = Sys_Lib_GetDispStr(DP_PAUSELESS60);
	cbdialog32string[ESENSE_80] = Sys_Lib_GetDispStr(DP_ESENS80);
	cbdialog32string[TSLOPE_2] = Sys_Lib_GetDispStr(DP_TSLOPELESS2);
	cbdialog32string[FSENSE_20] = Sys_Lib_GetDispStr(DP_FSENSLESSTHAN20);
	cbdialog32string[PSENSE_20] = Sys_Lib_GetDispStr(DP_PSENSLESS20);
// 	if(VT_Min_Flag)
// 	{
// 	    cbdialog32string[VT_10] = Sys_Lib_GetDispStr(DP_INFANTVTMORE10);
// 	}
// 	else
// 	{
// 		cbdialog32string[VT_10] = Sys_Lib_GetDispStr(DP_VTMORE);
// 	}
	if(tmp_Info->Patient_Type == PATIENTTYPE_ADULT)//成人
	{
		cbdialog32string[VT_10] = Sys_Lib_GetDispStr(DP_ADULTVTMORE100);
	}
	else if(tmp_Info->Patient_Type == PATIENTTYPE_PAEDIATRIC)//儿童
	{
		cbdialog32string[VT_10] = Sys_Lib_GetDispStr(DP_CHILVTMORE50);
	}
	else//婴儿
	{
		if(VT_Min_Flag && (mode == PRVC_MODE || mode == SIMV_PRVC_MODE || mode == VS_MODE))//5ml选配潮气量
		{
			cbdialog32string[VT_10] = Sys_Lib_GetDispStr(DP_INFANTVTMORE5);
		}
		else
		{
			cbdialog32string[VT_10] = Sys_Lib_GetDispStr(DP_INFANTVTMORE10);
		}
	}
	cbdialog32string[PINSP_5] = Sys_Lib_GetDispStr(DP_PINSPMORE5);
#if (LOGO_FLAG == ENDURE_LOGO)
	cbdialog32string[IE_110] = "I:E >= 1:12";
#else
	cbdialog32string[IE_110] = Sys_Lib_GetDispStr(DP_IEMORETHAN110);
#endif
	cbdialog32string[TINSP_1] = Sys_Lib_GetDispStr(DP_TINSPMORE1);
	if((mode == VCV_MODE) || (mode == PCV_MODE) || (mode == PSV_MODE) || (mode == PRVC_MODE)
		|| (mode == SIGH_MODE))
	{
		cbdialog32string[FREQ_1] = Sys_Lib_GetDispStr(DP_FREQMORE4BPM);
	}
	else
	{
		cbdialog32string[FREQ_1] = Sys_Lib_GetDispStr(DP_SIMVFREQMORE1BPM);
	}
	cbdialog32string[PSUPP_0] = Sys_Lib_GetDispStr(DP_PSUPPMORE0);
	cbdialog32string[PEEP_0FF] = Sys_Lib_GetDispStr(DP_PEEPOFF);
	cbdialog32string[PAUSE_0] = Sys_Lib_GetDispStr(DP_PAUSEMORETHAN0);
	cbdialog32string[TSLOPE_0] = Sys_Lib_GetDispStr(DP_TSLOPEMORE0);
#if(LOGO_FLAG != PENLON_LOGO)
	cbdialog32string[FSENSE_5] = Sys_Lib_GetDispStr(DP_FSENSMORE05LPM);
	cbdialog32string[PSENSE_0] = Sys_Lib_GetDispStr(DP_PSENSMORE0);
#else
	cbdialog32string[FSENSE_5] = Sys_Lib_GetDispStr(DP_FSENSMORE1LPM);
	cbdialog32string[PSENSE_0] = Sys_Lib_GetDispStr(DP_PSENSMORE1CMH2O);
#endif
	cbdialog32string[ESENSE_5] = Sys_Lib_GetDispStr(DP_ESENS5);
	cbdialog32string[TINSP_TPAUSE_1] = Sys_Lib_GetDispStr(DP_TINSPTPAUSEMORE01S);//
	cbdialog32string[TINSP_TSLOPE] = Sys_Lib_GetDispStr(DP_TINSPMORETSLOPE);
	cbdialog32string[FLOW_HIGH] = Sys_Lib_GetDispStr(DP_FLOWLIMITH);
	cbdialog32string[FLOW_LOW] = Sys_Lib_GetDispStr(DP_FLOWLIMITL);	
	cbdialog32string[PSUPP_PEEP_70] = Sys_Lib_GetDispStr(DP_PSUPPPEEPLESS80);
	cbdialog32string[PINSP_PEEP_70] = Sys_Lib_GetDispStr(DP_PINSPPEEPLESS70);
	cbdialog32string[PINSP_PEEP_PH] = Sys_Lib_GetDispStr(DP_PINSPPEEPLESSPH);
	cbdialog32string[PSUPP_PEEP_PH] = Sys_Lib_GetDispStr(DP_PSUPPPEEPLESSPH);
	cbdialog32string[PEEP_PH] = Sys_Lib_GetDispStr(DP_PEEPLESSPH);
	cbdialog32string[BACKUP_VCV] = Sys_Lib_GetDispStr(DP_BACKUPVCVACTIVE);//
	cbdialog32string[BACKUP_PCV] = Sys_Lib_GetDispStr(DP_BACKUPPCVACTIVE);//
	cbdialog32string[PINSP_30] = Sys_Lib_GetDispStr(DP_PINSPMORE30);//
	cbdialog32string[PINSP_50] = Sys_Lib_GetDispStr(DP_PINSPMORE50);//
	cbdialog32string[PEEP_PSUPP_30] = Sys_Lib_GetDispStr(DP_PEEPPSUPPMORETHAN30);//
	cbdialog32string[PEEP_PSUPP_50] = Sys_Lib_GetDispStr(DP_PEEPPSUPPMORETHAN50);//
	cbdialog32string[PEEP_20] = Sys_Lib_GetDispStr(DP_PEEPMORETHAN20);//
	cbdialog32string[IE_1] = Sys_Lib_GetDispStr(DP_IEMORETHAN1);
	cbdialog32string[TEXP_LOW] = Sys_Lib_GetDispStr(DP_TEXPLOWLIMIT);
}

