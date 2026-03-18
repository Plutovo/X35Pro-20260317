#include "MainTask.h"
#include "str.h"

extern unsigned int LanguageType;
extern unsigned char Drager_EtCO2_Config_On;
extern unsigned char MasimoISA_EtCO2_Config_On;

#ifdef __ICCARM__
const char  *(System_Display[STR_NUM]) @"AHBRAM";
#else
const char  *(System_Display[STR_NUM]);
#endif

void SYS_str_init()
{ 
	switch(LanguageType)
	{
	case 1:
		System_Display[ STR_Mode ]   = "  Modes  ";
		System_Display[ STR_Control ]   = "Controls ";
		System_Display[ STR_Alarm ]   = "Alarms";
		System_Display[ STR_System ]   = "System";
		System_Display[ STR_Monitoring ]   = "Monitoring";
		System_Display[ STR_Cancel ]   = "Cancel";
		System_Display[ STR_Confirm ]	  = "Confirm";
		System_Display[ STR_Volume ]	  = "Volume"; 
		System_Display[ STR_Pressure ]	  = "Pressure";
		System_Display[ STR_Invalidtouch ]	  = ") points is not recalibrate";
		System_Display[ STR_Alarm_Log ]	= "  Alarm Log  ";
		System_Display[ STR_FIO2 ]	 =  "FiO②";
		System_Display[ STR_VTE ]	 =  "VTe";
		System_Display[ STR_MV ]	 =  "MV";

		System_Display[ STR_FREQ ]	= "FREQ";//f
		System_Display[ STR_Mvspn ]  =  "Mvspn";//
		System_Display[ STR_Fspn ]   =  "fspn" ;//
		System_Display[ STR_PPEAK ]	 =  "Ppeak";//
		System_Display[ STR_Pmean ]	 =  "Pmean";//
		System_Display[ STR_PPLAT ]  =  "Pplat";
		System_Display[ STR_PEEP ]   =  "PEEP";
		System_Display[ STR_VTi ]    =  "VTi";//
		System_Display[ STR_Rst ]    =  "Rst";  
		System_Display[ STR_Cdyn ]   =  "Cdyn";
		System_Display[ STR_FiAA ]   =  "FiAA";
		System_Display[ STR_Ftotal ] =  "ftotal";
		System_Display[ STR_Loop ]   =  "LOOP";
		System_Display[ STR_MAC ]    =  "MAC";
		System_Display[ STR_FiN2O ]  =  "FiN②O";
		System_Display[ STR_EtN2O ]  =  "EtN②O";
		System_Display[ STR_FiAA1 ]  =  "FiAA1";
		System_Display[ STR_EtAA1 ]  =  "EtAA1";
		System_Display[ STR_FiAA2 ]  =  "FiAA2";
		System_Display[ STR_EtAA2 ]  =  "EtAA2";
		System_Display[ STR_AA ]     =  "AA";
		System_Display[ STR_Font_Version ]	= "Font_Version:";	

		System_Display[ STR_GUI ]			=  "GUI";
		System_Display[ STR_BDU ]			=  "BDU";
		System_Display[ STR_PMU ]			=  "PMU";
		System_Display[ STR_EFM ]			=  "EFM";

		System_Display[ STR_Pinsp ]         =  "Pinsp";
		System_Display[ STR_Tslope ]        =  "Tslope";
		System_Display[ STR_Psupp ]         =  "Psupp";
		System_Display[ STR_Fsens ]         =  "Fsens";
		System_Display[ STR_Esens ]         =  "Esens";
		System_Display[ STR_Tinsp ]         =  "Tinsp";
		System_Display[ STR_Psens ]         =  "Psens";
		System_Display[ STR_Apnea_time ]	= "  Apnea time";
		System_Display[ STR_Date ]	= "Date"; 
		System_Display[ STR_Time ]	= "Time";  
		System_Display[ STR_Event ]	= "Event";  
		System_Display[ STR_Info ]	= "  Information  "; 
		System_Display[ STR_Set ]	= "  Settings  "; 
		System_Display[ STR_Date_Time ]	= "  Date & time  ";
		System_Display[ STR_Calib ]	= "  Calibration  "; 	
		System_Display[ STR_Baro_pressure ]	= "Barometric:"; 
#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C)
		System_Display[ STR_Gas_Supply_Pressure ]	= "Supply Pressure";
#else
		System_Display[ STR_Gas_Supply_Pressure ]	= "Gas Supply Pressure";
#endif
		System_Display[ STR_Run_Time ]	= "RunTime                             ";
		System_Display[ STR_Hours ] = "Hours";
		System_Display[ STR_touch ]	= "Click on the cross in the center to verify\n            ("            ;
		System_Display[ STR_Sigh_Breath_Every ]	= "Sigh";//"Sigh Breath Every";
		System_Display[ STR_Drager ]	= "Drager Module"; 
		System_Display[ STR_Drager_O2 ]	= "Drager + O② Module"; 
		System_Display[ STR_Drager_Accuracy ]	= "Gas Module Unspecified Accuracy!"; //Drager未达到高精度
		System_Display[ STR_AA_O2 ]	= "AA&O②"; 
		System_Display[ STR_SetAAType ]	= "Set AA Type"; 
		System_Display[ STR_Language ]	= "Language"; 
		System_Display[ STR_Trigger_Type ]	= "Trigger Type";
		System_Display[ STR_CO2_unit ]	= "EtCO② Unit";
		System_Display[ STR_Loudness ]	= "Loudness";
		System_Display[ STR_Pressure_trigger ]	= "Pressure Trigger";
		System_Display[ STR_Flow_trigger ]	= "Flow Trigger";
		System_Display[ STR_Year ]	= "Year"; 
		System_Display[ STR_Month ]	= "Month"; 
		System_Display[ STR_Day ]	= "Day"; 
		System_Display[ STR_Hour ]	= "Hour"; 
		System_Display[ STR_Minutes ]  = "Minute";
		System_Display[ STR_Apply ]	= "Apply";
		System_Display[ STR_Pressure_Sensor_Cal ]	= "Pressure Sensor Calibration"; 
		System_Display[ STR_Flow_Sensor_Cal ]	= "Flow Sensor Calibration";
		System_Display[ STR_Oxygen_Cell_Cal ]	= "O② Cell Calibration";
		System_Display[ STR_Service_Mode ]	= "Service Modes"; 
		System_Display[ STR_Flow_Zero_Cal ]	= "Flowmeter Zero Calibration";
		System_Display[ STR_PhaseIn_Zero_Cal ]	= "Gas Module Zero Calibration";
		System_Display[ STR_Touch_Cal ]	= "Touchscreen Calibration";
		System_Display[ STR_Touch_To_Cal ]	= "Please touch the crosshair to calibrate:"; 
		System_Display[ STR_Set_O2_Concen ]	= "Please  select  O② concentration when Calibrating"; 

		//djq modify 20170527去掉ACGO提示
		System_Display[ STR_O2_10LMin ]	    = "Switch to Bag mode, disconnect breathing tubes, \
plug the bag port and adjust flowmeter's O② flow more than 10L/min.";

		System_Display[ STR_Air_10LMin ]	= "When air supply is normal, switch to Bag mode, \
disconnect breathing tubes, plug bag port and adjust flowmeter's air flow more than 10L/min.\n\
When air supply is not connected, remove O② cell from breathing system and expose it in room air.";

		System_Display[ STR_Return ]	= "Exit";
// 		System_Display[ STR_Flow_Close ]	= "Determine the flow valve \n is closed!";///
		System_Display[ STR_Iso_gas_moudle ]	= "Please remove the gas analyzer \nfrom circuit and put it in the air.";///
		System_Display[ STR_GASNOZERO ] = "Make sure gas module is in air\nand try again.";
		System_Display[ STR_Input_password ]	= "Input Password:";	
		System_Display[ STR_Password_Error ]	= "Password Error!";
		System_Display[ STR_N2O_Open]	= "N②O Open!";	///
		System_Display[ STR_Insp_Valve_Cal ]	= "Insp Valve Calibration";	
		System_Display[ STR_Exp_Flow_Sensor_Cal ]	= "Exp Flow Sensor Calibration";	
		System_Display[ STR_Exp_Valve_Cal ]	= "Exp Valve Calibration";
		System_Display[ STR_Flowmeter_Cal ]	= "Flowmeter Calibration";///
		System_Display[ STR_Calibrating ]	= "Calibrating...";
		System_Display[ STR_Cal_Failed ]	= "Calibration Failure!";	
		System_Display[ STR_Cal_Success ]	= "Calibration Success!";	
		System_Display[ STR_Waiting ]	= "Please waiting......";
		System_Display[ STR_No_Progam ]	= "No program can be upgraded!";
		System_Display[ STR_No_Response ]	= "\nno response!";//
		System_Display[ STR_Continue ]	= "Continue";
		System_Display[ STR_Restart ]	= "Please restart the machine!";	//
		System_Display[ STR_Full_Serial ]	= "Please input complete password!";	//
		System_Display[ STR_Serial_Correct ]	= "Password correct!";	
		System_Display[ STR_Serial_Failure ]	= "Password incorrect!";	
		System_Display[ STR_Entry_Serial ]	= "Input Password:";
		System_Display[ STR_Backspace ]	= "Backspace";
		System_Display[ STR_Battery_Cal ]	= "Battery Calibration";
		System_Display[ STR_Battery_Voltage ]	= "Battery Voltage";
		System_Display[ STR_Last_Battery_Cal ]	= "Last Battery Calibration:";	
		System_Display[ STR_Cal_Date ]	= "       Calibration Date:";	
		System_Display[ STR_Cal_Duration ]	= "       Calibration Duration:";	
		System_Display[ STR_All ]	= "All";	///
		System_Display[ STR_Up ]	= "Up";///
		System_Display[ STR_Input ]	= "Please input: ";///
		System_Display[ STR_Input_Again ]	= "Please input again:";///
		System_Display[ STR_Data_Not_Save ]	= "Data not saved\n confirm or cancel？";///
		System_Display[ STR_Rec_Error ]	= "Receive error!";	///
		System_Display[ STR_Rec_Timeout ]	= "Receive timeout!";	///
		System_Display[ STR_Zero_Beyond ]	= "Zero point beyond!";	///
		System_Display[ STR_EEPROM_Write_Error ]	= "EEPROM write error!";	///
		System_Display[ STR_EEPROM_Read_Error ]  = "EEPROM read error!";///
		System_Display[ STR_N2O_AD_Beyond ]  = "The N2O AD \nsampling beyond!";///
		System_Display[ STR_Check_N2O ]  = "Please check the N2O!";///
		System_Display[ STR_Check_AIR ]  = "Please check the Air!";	///
#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C)
		System_Display[ STR_Value1 ]  = "  Values 1 ";	
#else
		System_Display[ STR_Value1 ]  = "  Values ";	
#endif
// 		System_Display[ STR_Graphics ]  = "  Graphics  ";	
// 		System_Display[ STR_Fresh_gas ] = "Fresh gas";	///
// 		System_Display[ STR_Total_Gas ] = "Total flow";///
// 		System_Display[ STR_Waveform ]	= "Waveforms";///
// 		System_Display[ STR_Rapid_Monitor ]	= "Rapid Monitoring";///
		System_Display[ STR_Paw ]	= "Paw";	
		System_Display[ STR_Pleth ]	= "Pleth";	
		System_Display[ STR_No_Vent_To_Patient ]	= "      No ventilation delivered to the patient";	
		System_Display[ STR_Activate_Standby ]	= "Activate standby";	
		System_Display[ STR_Sec ]	= "Sec";///
		System_Display[ STR_UNIT_S ]	= "s";///
		System_Display[ STR_Backup ]	= "  Backup  ";
		System_Display[ STR_Paw_V_Loop ]	= "Paw-V";
		System_Display[ STR_V_Flow_Loop ]	= "V-Flow";		
		System_Display[ STR_ON ]	= "ON";	///
		System_Display[ STR_OFF ]	= "OFF";	///
		System_Display[ STR_English ]	= "English";		
		System_Display[ STR_Pulse ]	= "Pulse";
		System_Display[ STR_SPO2 ]	= "SpO②";	
		System_Display[ STR_PI ]	= "PI";
		System_Display[ STR_SoftwareVersion ]	= "Software Version";
		System_Display[ STR_Option ]	= "Option";
		System_Display[ STR_Enter ]	= "Enter"; 
		System_Display[ STR_EXIT ]	= "Exit";
// 		System_Display[ STR_Layout2 ] = "Layout 2";
		System_Display[ STR_Limits1 ] = "    Limits 1   ";
		System_Display[ STR_Limits2 ] = "    Limits 2   ";
		System_Display[ STR_Limits3 ] = "    Limits 3   ";///
		System_Display[ STR_N2O_Close ] = "N②O Close";	///
		System_Display[ STR_Max_Flow ]  = "Max Flow";
		System_Display[ STR_Insp_Time ]  = "Inspiratory Time";//
		System_Display[ STR_I_E ]  = "I : E";		
		System_Display[ STR_Pressure_Mode ]  = "Pressure";		
		System_Display[ STR_Chinese ]  = "简体中文";	
		System_Display[ STR_Turkish ]  = "TÜRKÇE";
		System_Display[ STR_Spanish ]  = "Español";
		System_Display[ STR_Flow ]  = "Flow"   ;
		System_Display[ STR_Total_Flow_Eng]  = "Total flow ";///
		System_Display[ STR_N2O ]  = "N②0"	   ;///
		System_Display[ STR_AA1 ]  = "AA1"	   ;///
		System_Display[ STR_AA2 ]  = "AA2"	   ;///
		System_Display[ STR_CO2 ]  = "CO②"    ;
		System_Display[ STR_EtCO2 ]  = "EtCO②"	;	
		System_Display[ STR_FiCO2 ]  = "FiCO②"	;
		System_Display[ STR_Set_Mode ]  = "  Modes  ";	
		System_Display[STR_Upgrade ]  = "Upgrade ";//
		System_Display[STR_Cancel_Upgrade ]  = "Cancel";
		System_Display[ STR_Continue_Upgrade ]  = "Continue";	
		System_Display[ STR_Active ]  = "Active";//ylk add	

		System_Display[ STR_VT_1500 ]  = "Vt <= ";
		System_Display[ STR_Pinsp_70 ]  = "Pinsp <= 70 cmH②O";
		System_Display[ STR_Ie_41 ]  =  "I:E <= 4:1";
		System_Display[ STR_Tinsp_10 ]  = "Tinsp <= 10.0 s";
		System_Display[ STR_Tisnp_Tpause_10 ]  = "Tinsp - Tpause <= 10.0 s";
		System_Display[ STR_Psupp_70 ]  = "Psupp <= 70 cmH②O";
		System_Display[ STR_Simv_Freq_40 ]  = "SIMV FREQ <= 40 bpm";
		System_Display[ STR_Freq_100 ]  =  "FREQ <= 100 bpm";
		System_Display[ STR_Peep_30 ]  = "PEEP <= 30 cmH②O";
		System_Display[ STR_Pause_50 ]  = "Pause <= 60%";    //han
		System_Display[ STR_Esense_80 ]  = "Esens <= 80%";
		System_Display[ STR_Tslop_2 ]  = "Tslope <= 2.0s";
		System_Display[ STR_Fsense_20 ]  = "Fsens <= 20.0 LPM";
		System_Display[ STR_Psense_20 ]  = "Psens <= 20 cmH②O";
// 		System_Display[ STR_Vt_20 ]  =  "Vt Low Limit";
		System_Display[ STR_Vt_50 ]  = "Vt >= ";
		System_Display[ STR_Pinsp_5 ]  =  "Pinsp >= 5 cmH②O";
		System_Display[ STR_Ie_110 ]  = "I:E >= 1:10";
		System_Display[ STR_Tinsp_1 ]  = "Tinsp >= 0.1 s";
		System_Display[ STR_Freq_1 ]  = "SIMV FREQ >= 1 bpm";
		System_Display[ STR_Freq_4 ]  = "FREQ >= 4 bpm";  //sun add 20140416
		System_Display[ STR_Psupport_0 ]  = "Psupp >= 0 cmH②O";
		System_Display[ STR_Peep_Off ]  = "PEEP OFF";
		System_Display[ STR_Pause_0 ]  = "Pause >= 0%";;
		System_Display[ STR_Tslop_0 ]  = "Tslope >= 0.0s";
#if(LOGO_FLAG != PENLON_LOGO)//ylk add 20180709
		System_Display[ STR_Psense_0 ]  = "Psens >= 0 cmH②O";
		System_Display[ STR_Fsense_5 ]  = "Fsens >= 0.5 LPM";
#else
		System_Display[ STR_Psense_0 ]  = "Psens >= 1 cmH②O";
		System_Display[ STR_Fsense_5 ]  = "Fsens >= 1.0 LPM";
#endif		
		System_Display[ STR_Esense_5 ]  = "Esens >= 5%";
		System_Display[ STR_Pause ]  = "Pause";
		System_Display[ STR_Vt ]  = "VT";
		System_Display[ STR_Tinsp_Tpause_1 ]  = "Tinsp - Tpause >= 0.1 s";
		System_Display[ STR_Tinsp_Tslop ]  =  "Tinsp >= Tslope";
		System_Display[ STR_Flow_High ]  = "Flow High Limit";
		System_Display[ STR_Flow_Low ]  = "Flow Low Limit";
		System_Display[ STR_Psupport_Peep_70 ]  = "Psupp + PEEP <= 70 cmH②O";
		System_Display[ STR_Pinsp_Peep_70 ]  =  "Pinsp + PEEP <= 70 cmH②O";
		System_Display[ STR_Pinsp_Peep_Ph ]  = "Pinsp + PEEP < Ph";
		System_Display[ STR_Psupp_Peep_Ph ]  =  "Psupp + PEEP < Ph";
		System_Display[ STR_Peep_Ph ]  = "PEEP < Ph";
		System_Display[ STR_Texp_Low ]  = "Texp Low Limit";
		System_Display[ STR_Backup_VCV ]  = "Backup VCV Actived";
		System_Display[ STR_Backup_PCV ]  =  "Backup PCV Actived";
		System_Display[ STR_Pinsp_30 ]  = "Pinsp > 30 cmH②O ? Confirm";
		System_Display[ STR_Pinsp_50]  = "Pinsp > 50 cmH②O ? Confirm";
		System_Display[ STR_Peep_Psupport_30 ]  = "PEEP + Psupp > 30 cmH②O ? Confirm";
		System_Display[ STR_Peep_Psupport_50 ]  = "PEEP + Psupp > 50 cmH②O ? Confirm";
		System_Display[ STR_Peep_20 ]  = "PEEP > 20 cmH②O ? Confirm";
		System_Display[ STR_Ie_1 ]  = "I:E > 1:1 ? Confirm";
		System_Display[ STR_Sec_1 ]  = "sec";///
		System_Display[ STR_USA_Standard ]  = "USA Standard!";///
		System_Display[ STR_EN_Standard ]  = "EN Standard!";///	
		System_Display[ STR_FiN2O ]  = "FiN②O";///		
// 		System_Display[ STR_ET ]  = "Et";///	
// 		System_Display[ STR_FI ]  = "Fi";///		
// 		System_Display[ STR_FREQ_English ] ="f";///
		System_Display[ STR_Value2 ]  = "  Values 2 ";///	
		System_Display[ STR_O2 ]  = "O②";
		System_Display[ STR_N20 ]  ="N②O"; ///
		System_Display[ STR_AIR ]  =  "Air";///	
// 		System_Display[ STR_O2_AD_Beyond ]   =  "The O② AD \nsampling beyond!";///
// 		System_Display[ STR_AIR_AD_Beyond ]	 =   "The O② AD \nsampling beyond!";		
		System_Display[ STR_User_BP_Set ]	       = "Use BP set:" ;///
		System_Display[ STR_Cal_BP_set  ]	       = "Cal. BP set:" ;	///			
// 		System_Display[ STR_Flowmeter_Air_Test  ]	       = "Flow\nmeter \nAir test" ;	///	
		System_Display[ STR_Russian  ]	       = "Русский" ;   
		System_Display[ STR_System_Test]     = "System Test";
		System_Display[ STR_Preuse_Test]     = "Start pre-use Test?";
		System_Display[ STR_Start]     = "Start";
		System_Display[ STR_Alarm_Silence ]	 = "Alarm Silence";
		System_Display[ STR_Alarm_Reset ]	 = "Alarm Reset";
		System_Display[ STR_Trend]	 = "Trends";
		System_Display[ STR_Trend_graph  ]	 = "Trends Graph";    
		System_Display[ STR_STANDBY	]				 = "Standby" ;      
		System_Display[STR_Dirver_Gas_Air] = "Driver gas is air,can not close the switch!";
// 		System_Display[STR_Dirver_Gas_Change_Air] = "Drive gas has to switch to air!";
// 		System_Display[STR_Dirver_Gas_Change_O2] = "Drive gas has to switch to O②!";
		System_Display[STR_Close_Air_Switch] = "Whether close the Air switch?";
		System_Display[STR_Change_Dirver_Gas] = "Whether to change driver gas?";
		System_Display[STR_DRIVER] = "Driver";
		System_Display[STR_CLEARTREND] = "Modify time will clear trend data?";
		System_Display[STR_MODES_SIGH] = " +SIGH";

		System_Display[ STR_FiHAL ]  =  "FiHAL";
		System_Display[ STR_FiENF ]  =  "FiENF";
		System_Display[ STR_FiISO ]  =  "FiISO";
		System_Display[ STR_FiSEV ]  =  "FiSEV";
		System_Display[ STR_FiDES ]  =  "FiDES";
		System_Display[ STR_EtHAL ]  =  "EtHAL";
		System_Display[ STR_EtENF ]  =  "EtENF";
		System_Display[ STR_EtISO ]  =  "EtISO";
		System_Display[ STR_EtSEV ]  =  "EtSEV";
		System_Display[ STR_EtDES ]  =  "EtDES";

		System_Display[ STR_HAL ]    =  "HAL";//氟烷
		System_Display[ STR_ENF ]    =  "ENF";//安氟醚
		System_Display[ STR_ISO ]    =  "ISO";//异氟醚
		System_Display[ STR_SEV ]    =  "SEV";//七氟醚
		System_Display[ STR_DES ]    =  "DES";//地氟醚

		System_Display[ STR_SPO2VER ]		 =  "SpO② Version";
		System_Display[ STR_AVERTIME ]		 =  "Average Time";
		System_Display[ STR_SENSITIVITY ]	 =  "Sensitivity";
		System_Display[ STR_SMARTTONE ]		 =  "Smart Tone";
		System_Display[ STR_FASTSAT ]		 =  "Fast SAT";
		System_Display[ STR_MCUVER ]		 =  "MCU:";
		System_Display[ STR_DSPVER ]		 =  "DSP:";

		System_Display[ STR_LASTO2CALIB ]	 =  "Last O② span calibration failed!";

		System_Display[ STR_MODE_SIMV_P_PSV ]  =  "SIMV-P & PSV Mode";
		System_Display[ STR_MODE_PRVC ]        =  "PRVC Mode";
		System_Display[ STR_MODE_INFANT ]      =  "Infant Mode";
		System_Display[ STR_MODE_CO2 ]         =  "CO② Module";
		System_Display[ STR_MODE_SpO2 ]        =  "SpO② Module";
		System_Display[ STR_MODE_Driver ]      =  "Gas Driver Mode";
		System_Display[ STR_MODE_O2 ]          =  "O② Module";
		System_Display[ STR_MODE_Masimo_IRMA ]            =  "Masimo IRMA";
		System_Display[ STR_MODE_Masimo_ISA ]             =  "Masimo ISA";
		System_Display[ STR_MODE_Masimo_ISA_O2 ]          =  "Masimo ISA+O②";
		System_Display[ STR_MODE_Masimo_SpO2 ]            =  "Masimo SpO② Module";
		System_Display[ STR_MODE_AA_CO2 ]                 =  "AA & CO② Module";

		System_Display[ STR_MODE_Round ]                  =  "○";

		//System Test
		System_Display[STR_SelfTest_Ask]                      =  "Start pre-use Test?";
		System_Display[STR_SelfTest_Exit]                     =  "Exit";
		System_Display[STR_SelfTest_Manual_Circuit]           =  "Manual Leak Test";
		System_Display[STR_SelfTest_Auto_Circuit]             =  "Automatic System Test";
		System_Display[STR_SelfTest_Last_Manual_Circuit]      =  "Last Manual Leak Test";
		System_Display[STR_SelfTest_Last_Auto_Circuit]        =  "Last Automatic System Test";

		System_Display[STR_SelfTest_Checking]                 =  "Testing...";
		System_Display[STR_SelfTest_Result_State]             =  "√Pass    ×Fail    —Skip";
		System_Display[STR_SelfTest_Symbol_Pass]              =  "√";
		System_Display[STR_SelfTest_Symbol_Skip]              =  "—";
		System_Display[STR_SelfTest_Symbol_Fail]              =  "×";

		System_Display[STR_Pass]                              =  "Pass";
		System_Display[STR_Fail]                              =  "Fail";
		System_Display[STR_Skip]                              =  "Skip";
		System_Display[STR_Finish]                            =  "Finish";
		System_Display[STR_Retry]                             =  "Repeat";
		System_Display[STR_SelfTest_Err_Timeout]              =  "Timeout";

		//Check item
		System_Display[STR_SelfTest_Manual_Check]             =  "Manual";
		System_Display[STR_SelfTest_Baro_Check]               =  "Barometer";
		System_Display[STR_SelfTest_Gas_Check]                =  "Gas Supply";
		System_Display[STR_SelfTest_FG_Flow_Check]            =  "FG Flow Sensor";
		System_Display[STR_SelfTest_CircuitFlow_Check]        =  "Circuit Flow Sensor";
		System_Display[STR_SelfTest_Pressure_Check]           =  "Pressure Sensor";
		System_Display[STR_SelfTest_CircuitCompliance_Check]  =  "Circuit Compliance";
		System_Display[STR_SelfTest_CircuitLeakage_Check]     =  "Circuit Leakage";
		System_Display[STR_SelfTest_Battery_Check]            =  "Battery";
		System_Display[ STR_SelfTest_OxygenSensor_Check ]     =  "Oxygen Sensor";
		//Manual Prompt
		System_Display[STR_SelfTest_Manual_Prompt_1]         =  "1.Set Bag/Vent switch to Bag";
		System_Display[STR_SelfTest_Manual_Prompt_2]         =  "2.Connect the patient circuit Y piece to the absorber test block";
		System_Display[STR_SelfTest_Manual_Prompt_3]         =  "3.Make sure that the Manual Bag is connected";
		System_Display[STR_SelfTest_Manual_Prompt_4]         =  "4.Connect gas pipeline,make sure the supply pressure is between 280-600kPa ";
		System_Display[STR_SelfTest_Manual_Prompt_5]         =  "5.Check the absorber canister and water trap are installed correctly";
		System_Display[STR_SelfTest_Manual_Prompt_6]         =  "6.Set all flow controls to minimum";
		System_Display[STR_SelfTest_Manual_Prompt_7]         =  "7.Turn off the vaporizers";
		System_Display[STR_SelfTest_Manual_Prompt_8]         =  "8.Set ACGO switch to OFF (Circle System position)";
#if (LOGO_FLAG == PENLON_LOGO)	
		System_Display[STR_SelfTest_Manual_Prompt_9]         =  "9.Set the APL valve to 50 cmH②O";
		System_Display[STR_SelfTest_Manual_Prompt_11]        =  "11.Set ACGO switch to ON (Open circuit position)";
		System_Display[STR_SelfTest_Manual_Prompt_12]        =  "12.Press Start button to perform Manual Leak Test";
#else
		System_Display[STR_SelfTest_Manual_Prompt_9]         =  "9.Set the APL valve to 70 cmH②O";	
		System_Display[STR_SelfTest_Manual_Prompt_11]        =  "11.Press Start button to perform Manual Leak Test";
#endif	
		System_Display[STR_SelfTest_Manual_Prompt_10]        =  "10.Push O② flush button until the airway pressure gauge is between 30 and 40 cmH②O";
		//Automatic Prompt 1
		System_Display[STR_SelfTest_Auto_Prompt_1]           =  "1.Set Bag/Vent switch to Vent";
		System_Display[STR_SelfTest_Auto_Prompt_2]           =  "2.Open the patient Y piece";
		System_Display[STR_SelfTest_Auto_Prompt_3]           =  "3.Check the absorber canister and water trap are installed correctly";
		System_Display[STR_SelfTest_Auto_Prompt_4]           =  "4.Set all flow controls to minimum";
		System_Display[STR_SelfTest_Auto_Prompt_5]           =  "5.Turn off the vaporizers";
		System_Display[STR_SelfTest_Auto_Prompt_6]           =  "6.Set ACGO switch to OFF(Circle System position)";
		System_Display[STR_SelfTest_Auto_Prompt_7]           =  "7.Make sure the bellows are collapsed";
		System_Display[STR_SelfTest_Auto_Prompt_8]           =  "8.Press start button to perform Automatic System Test";
		//Automatic Prompt 2
		System_Display[STR_SelfTest_Auto_Prompt_10]          = "1.Set Bag/Vent switch to Bag and plug the bag port";
		if(Drager_EtCO2_Config_On == 2 || MasimoISA_EtCO2_Config_On == 2) //ylk add 20181221
		{
			System_Display[STR_SelfTest_Auto_Prompt_11]      = "2.Adjust the Oxygen flow to 10L/min";
			System_Display[STR_SelfTest_Auto_Prompt_Drager] = "3.Connect gas module sample line to elbow and plug elbow into Y piece";
			System_Display[STR_SelfTest_Auto_Prompt_12]      = "4.Press the start button";
		}
		else
		{
			System_Display[STR_SelfTest_Auto_Prompt_11]      = "2.Adjust the Oxygen flow to 10L/min";
			System_Display[STR_SelfTest_Auto_Prompt_12]          = "3.Press the start button";
		}
		//Automatic Prompt 2
		System_Display[STR_SelfTest_Auto_Prompt_13]          =  "1.Set Bag/Vent switch to Vent";
		System_Display[STR_SelfTest_Auto_Prompt_14]          =  "2.Occlude the patient Y piece";
		System_Display[STR_SelfTest_Auto_Prompt_15]          =  "3.Set all flow controls to minimum";
		System_Display[STR_SelfTest_Auto_Prompt_16]          =  "4.Push O2 flush button to fill the bellows";
#if (LOGO_FLAG == PENLON_LOGO)		
		System_Display[STR_SelfTest_Auto_Prompt_17]          =  "5.Set ACGO switch to ON (Open circuit position)";
		System_Display[STR_SelfTest_Auto_Prompt_18]          =  "6.Press the start button";
#else
		System_Display[STR_SelfTest_Auto_Prompt_17]          =  "5.Press the start button";
#endif
		// 
		System_Display[STR_SelfTest_Err_LeakOut]             =  "Leakage over range!";//泄漏超范围
		System_Display[STR_SelfTest_Err_BagVent]             =  "Bag/Vent switch position error!";//手动/机控位置故障
		System_Display[STR_SelfTest_Err_Circuit_NoConnect]   =  "Circuit not connect!";//管路未连接
		System_Display[STR_SelfTest_Err_ValveLeak]           =  "Inspiratory flow control valve \n or flowmeters leakage failure!";//比例阀或新鲜气体阀漏气故障
		System_Display[STR_SelfTest_Err_Sensor]              =  "Please perform O② Cell Calibration and re-test!";
		System_Display[STR_SelfTest_Err_SensorTest]          =  "Sensor failure!";//传感器测试错误
		System_Display[STR_SelfTest_Err_Sensor_NoConnect]    =  "Sensor lose efficacy or not connect!";//传感器失效或者未连接
		System_Display[STR_SelfTest_Err_Insp_FlowSensor]     =  "Inspiratory flow sensor test failure!";//吸气流量传感器测试失败
		System_Display[STR_SelfTest_Err_Exp_FlowSensor]      =  "Expiratory flow sensor test failure!";//呼气流量传感器测试失败
		System_Display[STR_SelfTest_Err_InspExp_FlowSensor]  =  "Inspiratory flow sensor and\nexpiratory flow sensor test failure!";//吸气和呼气流量传感器测试失败
		System_Display[STR_SelfTest_Err_Compliance]          =  "Compliance test failure!";//顺应性测试失败
		System_Display[STR_Empty]                            =  "";
		System_Display[STR_SelfTest_Disconnect_AC]           =  "Please disconnect AC power supply!";//请断开交流电源!
		System_Display[STR_SelfTest_Connect_AC]              =  "Please connect AC power supply!";//请连接断开交流电源!
		System_Display[STR_SelfTest_Err_Gas_Supply]          =  "Gas supply failure!";//气源故障!
		System_Display[STR_SelfTest_Err_O2_Supply]           =  "O② supply failure!";//氧气气源故障!
		System_Display[STR_SelfTest_Err_N2O_Supply]          =  "N②O supply failure!";//笑气气源故障!
		System_Display[STR_SelfTest_Err_Air_Supply]          =  "Air supply failure!";//空气气源故障!
		System_Display[STR_SelfTest_Err_O2_N2O_Supply]       =  "O② and N②O supply failure!";//氧气与笑气气源故障!
		System_Display[STR_SelfTest_Err_O2_Air_Supply]       =  "O② and Air supply failure!";//氧气与空气气源故障!
		System_Display[STR_SelfTest_Err_N2O_Air_Supply]      =  "N②O and Air supply failure!";//笑气与空气气源故障!
		System_Display[STR_DISP_ERR_NUM]					 =  "Err num:";//错误编号
		break;

#ifdef LANGUAGE_CHINESE_FLAG
	case 0:
		System_Display[ STR_Mode ]   = "  模式  ";
		System_Display[ STR_Control ]   = "控制";
		System_Display[ STR_Alarm ]   = "报警";
		System_Display[ STR_System ]   = "系统";
		System_Display[ STR_Monitoring ]   = "监测";
		System_Display[ STR_Cancel ]   = "取消";
		System_Display[ STR_Confirm ]	  = "确认";
		System_Display[ STR_Volume ]	  = "容量"; 
		System_Display[ STR_Pressure ]	  = "压力";
		System_Display[ STR_Invalidtouch ]	  = ")次未点中则重新校准";
		System_Display[ STR_Alarm_Log ]	= "   报警记录  ";
		System_Display[ STR_FREQ ]	= "呼吸频率";

		System_Display[ STR_VTi ]   =  "吸气潮气量";
		System_Display[ STR_VTE ]	= "呼气潮气量";
		System_Display[ STR_MV ]	= "分钟通气量";
		System_Display[ STR_Mvspn ]  = "自主通气量";
		System_Display[ STR_Fspn ]   = "自主频率" ;
		System_Display[ STR_PPEAK ]	= "峰值压力";
		System_Display[ STR_Pmean ]	= "平均压力";
		System_Display[ STR_PPLAT ] =  "平台压力";
		System_Display[ STR_PEEP ]  =  "呼气末正压";
		System_Display[ STR_FIO2 ]	= "氧浓度";
		System_Display[ STR_Rst ]    = "静态阻力";  
		System_Display[ STR_Cdyn ]   = "动态顺应性";
		System_Display[ STR_FiAA ]  =  "吸入麻醉气";
		System_Display[ STR_Ftotal ] = "呼吸频率";
		System_Display[ STR_SPO2 ] = "血氧饱和度";
		System_Display[ STR_PI ]	= "灌注指数";
		System_Display[ STR_Pulse ] = "脉率";

		System_Display[ STR_MAC ]    =  "MAC";
		System_Display[ STR_FiN2O ]  =  "吸入笑气";
		System_Display[ STR_EtN2O ]  =  "呼末笑气";
		System_Display[ STR_FiAA1 ]  =  "吸入主麻气";
		System_Display[ STR_EtAA1 ]  =  "呼末主麻气";
		System_Display[ STR_FiAA2 ]  =  "吸入辅麻气";
		System_Display[ STR_EtAA2 ]  =  "呼末辅麻气";

		System_Display[ STR_Pinsp ]  = "压力控制";//压力控制水平
		System_Display[ STR_Tslope ] = "压力上升时间";
		System_Display[ STR_Psupp ]  = "压力支持";
		System_Display[ STR_Fsens ]  = "流量触发";
		System_Display[ STR_Esens ]  = "呼气触发 \n灵敏度";
		System_Display[ STR_Tinsp ]  = "吸气时间";
		System_Display[ STR_Psens ]  = "压力触发";
		System_Display[ STR_Loop ]   =  "环图";

		System_Display[ STR_Apnea_time ]	= "窒息时间";
		System_Display[ STR_Date ]	= "日期"; 
		System_Display[ STR_Time ]	= "时间";   
		System_Display[ STR_Event ]	= "事件";  
		System_Display[ STR_Info ]	= "  系统信息  "; 
		System_Display[ STR_Set ]	= "    设置    "; 
		System_Display[ STR_Date_Time ]	= " 日期 & 时间 ";
		System_Display[ STR_Calib ]	= "  系统校验  "; 	
		System_Display[ STR_Baro_pressure ]	= "大气压力"; 
		System_Display[ STR_Gas_Supply_Pressure ]	= "气源压力";
		System_Display[ STR_Run_Time ]	= "运行时间";//
		System_Display[ STR_Hours ] = "小时";
		System_Display[ STR_touch ]	= "请点击十字中心进行验证,(";
		System_Display[ STR_Sigh_Breath_Every ]	= "叹息间隔";
		System_Display[ STR_SetAAType ]	= "设置麻气类型";
		System_Display[ STR_AA_O2 ]	= "麻醉气和氧气"; 
		System_Display[ STR_Drager ]	= "Drager Module"; 
		System_Display[ STR_Drager_O2 ]	= "Drager + O② Module";
		System_Display[ STR_Drager_Accuracy ]	= "气体模块浓度超差!"; //Drager未达到高精度

		System_Display[ STR_Language ]	= "语言设置"; 
		System_Display[ STR_Trigger_Type ]	= "触发类型";
		System_Display[ STR_CO2_unit ]	= "二氧化碳单位";
		System_Display[ STR_Loudness ]	= "报警音量";
		System_Display[ STR_Pressure_trigger ]	= "压力触发";
		System_Display[ STR_Flow_trigger ]	= "流量触发";
		System_Display[ STR_Year ]	= "年"; 
		System_Display[ STR_Month ]	= "月"; 
		System_Display[ STR_Day ]	= "日";  
		System_Display[ STR_Hour ]	= "小时"; 
		System_Display[ STR_Minutes ]  = "分钟";
		System_Display[ STR_Apply ]	= "应用";    
		System_Display[ STR_Pressure_Sensor_Cal ]	= "压力传感器校验"; 
		System_Display[ STR_Flow_Sensor_Cal ]	= "流量传感器校验";
		System_Display[ STR_Oxygen_Cell_Cal ]	= "氧浓度传感器校验";
		System_Display[ STR_Service_Mode ]	= "工程师模式"; 
		System_Display[ STR_Flow_Zero_Cal ]	= "流量计零点校验";
		System_Display[ STR_PhaseIn_Zero_Cal ]	= "气体模块校验";
		System_Display[ STR_Touch_Cal ]	= "触摸屏校验";
		System_Display[ STR_Touch_To_Cal ]	= "请点击十字中心进行校准"; 
		System_Display[ STR_Set_O2_Concen ]	= "请选择校验的氧浓度:"; 
		//zkq modify 20161215 氧浓度校验表述优化,
		System_Display[ STR_O2_10LMin ] = "1.在手动模式下,断开呼吸管路连接并堵上手动皮囊端口\n2.调节氧气流量大于10 L/min 并保持2分钟以上\n3.点击确定开始校验";
		System_Display[ STR_Air_10LMin ] = "1.当空气气源正常时,在手动模式下,断开呼吸管路连接并堵上手动皮囊端口,调节空气流量大于10 L/min; 或将氧浓度传感器置 于空气中 \n2.保持2分钟以上\n3.点击确定开始校验";

		System_Display[ STR_Return ]	= "返回";
// 		System_Display[ STR_Flow_Close ]	= "确定流量阀关闭";///
		System_Display[ STR_Iso_gas_moudle ]	= "将气体模块探头置于空气中";///
		System_Display[ STR_GASNOZERO ] = "确保将气体模块置于空气中,\n并重试";
		System_Display[ STR_Input_password ]	= "请输入密码:";	
		System_Display[ STR_Password_Error ]	= "密码错误!";	
		//System_Display[ STR_Font_Version ]	= "字库版本号:";	

		System_Display[ STR_N2O_Open]	= "笑气已开启!";///	
		System_Display[ STR_Insp_Valve_Cal ]	= "吸气阀校验";	
		System_Display[ STR_Exp_Flow_Sensor_Cal ]	= "呼气传感器校验";	
		System_Display[ STR_Exp_Valve_Cal ]	= "呼气阀校验";
		System_Display[ STR_Flowmeter_Cal ]	= "电子流量计校验";///
		System_Display[ STR_Calibrating ]	= "正在校验...";
		System_Display[ STR_Cal_Failed ]	= "校验失败!";	
		System_Display[ STR_Cal_Success ]	= "校验成功";	
		System_Display[ STR_Waiting ]	= "请等待......";
		System_Display[ STR_No_Progam ]	= "没有可以升级的程序!";
		System_Display[ STR_No_Response ]	= "\n无响应";
		System_Display[ STR_Continue ]	= "继续";
		System_Display[ STR_Restart ]	= "请重启设备";	
		System_Display[ STR_Full_Serial ]	= "请输入完整密码!";	
		System_Display[ STR_Serial_Correct ]	= "密码输入成功!";	
		System_Display[ STR_Serial_Failure ]	= "密码输入失败!";	
		System_Display[ STR_Entry_Serial ]	= "请输入密码:";
		System_Display[ STR_Backspace ]	= "退格";
		System_Display[ STR_Battery_Cal ]	= "电池校验";
		System_Display[ STR_Battery_Voltage ]	= "电池电压:";
		System_Display[ STR_Last_Battery_Cal ]	= "上次电池校验:";	
		System_Display[ STR_Cal_Date ]	= "      校验日期:";	
		System_Display[ STR_Cal_Duration ]	= "      校验持续:";	
		/*************************************************start*******************************************************/
		System_Display[ STR_All ]	= "全部";	
		System_Display[ STR_Up ]	= "上一步";
		System_Display[ STR_Input ]	= "输入数值";
		System_Display[ STR_Input_Again ]	= "再次输入数值";
		System_Display[ STR_Data_Not_Save ]	= "数据未保存，\n是否退出？";
		System_Display[ STR_Rec_Error ]	= "接收错误";	
		System_Display[ STR_Rec_Timeout ]	= "接收超时";	
		System_Display[ STR_Zero_Beyond ]	= "零点超范围";	
		System_Display[ STR_EEPROM_Write_Error ]	= "EEPROM写错误";	
		System_Display[ STR_EEPROM_Read_Error ]  = "EEPROM读错误";
		System_Display[ STR_N2O_AD_Beyond ]  = "笑气AD值超范围";
		System_Display[ STR_Check_N2O ]  = "校验笑气";
		System_Display[ STR_Check_AIR ]  = "校验空气";	
		/*************************************************end*******************************************************/
		System_Display[ STR_Value1 ]  = " 监测参数 1";	
// 		System_Display[ STR_Graphics ]  = " 波形设置 ";	
// 		System_Display[ STR_Fresh_gas ] = "新鲜气体";	///
// 		System_Display[ STR_Total_Gas ] = "总流量";///
// 		System_Display[ STR_Waveform ]	= "波形检测";///
// 		System_Display[ STR_Rapid_Monitor ]	= "快捷参数";///
		System_Display[ STR_Paw ]	= "压力";//
		System_Display[ STR_Pleth ]	= "Pleth";	
		System_Display[ STR_No_Vent_To_Patient ]	= "      待机模式下患者不能得到通气支持";	
		System_Display[ STR_Activate_Standby ]	= "激活待机模式";	
		System_Display[ STR_Sec ]	= "秒";///
		System_Display[ STR_UNIT_S ]	= "秒";///
		System_Display[ STR_Backup ]	= "  备份通气  ";
		System_Display[ STR_Paw_V_Loop ]	= "压力-容量";//Paw-V
		System_Display[ STR_V_Flow_Loop ]	= "容量-流量";//V-Flow    //ylk 20180428 modify 流速->流量	
		System_Display[ STR_ON ]	= "开";	///
		System_Display[ STR_OFF ]	= "关";///
		System_Display[ STR_English ]	= "English";
		System_Display[ STR_EFM ]	= "EFM";///
		System_Display[ STR_SoftwareVersion ]	= "软件       版本";
		System_Display[ STR_Option ]	= "选配";
		System_Display[ STR_Enter ]	= "确定";
		System_Display[ STR_EXIT ]	= "退出";
// 		System_Display[ STR_Layout2 ] = "布局 2";
		System_Display[ STR_Limits1 ] = "    设置 1   ";
		System_Display[ STR_Limits2 ] = "    设置 2   ";
		System_Display[ STR_Limits3 ] = "    设置 3   ";///
		System_Display[ STR_N2O_Close ] = "笑气已关闭!";///
		System_Display[ STR_Max_Flow ]  = "最大流量";
		System_Display[ STR_Insp_Time ]  = "吸气时间";
		System_Display[ STR_I_E ]  = "吸呼比";//X40用
		System_Display[ STR_Pressure_Mode ]  = "压力模式";
		System_Display[ STR_Chinese ]  = "简体中文";	
		System_Display[ STR_Turkish ]  = "TÜRKÇE";
		System_Display[ STR_Spanish ]  = "Español";
		System_Display[ STR_Flow ]  = "流量";
		System_Display[ STR_Total_Flow_Eng]  = "Total flow ";///
		System_Display[ STR_N2O ]  = "笑气"	;
		System_Display[ STR_AA1 ]  = "主麻气";
		System_Display[ STR_AA2 ]  = "辅麻气";
		System_Display[ STR_CO2 ]  = "二氧化碳" ;
		System_Display[ STR_EtCO2 ]  = "呼末CO②"	;	
		System_Display[ STR_FiCO2 ]  = "吸入CO②"	;	
		System_Display[ STR_Set_Mode ]  = "  模式设置  ";	
		System_Display[STR_Upgrade ]  = "升级"	;
		System_Display[STR_Cancel_Upgrade ]  = "取消升级";
		System_Display[ STR_Continue_Upgrade ]  = "继续升级";
		System_Display[ STR_VT_1500 ]  = "潮气量最大值";
		System_Display[ STR_Pinsp_70 ]  = "压力控制最大值70cmH②O";
		System_Display[ STR_Ie_41 ]  = "吸呼比最大值4:1";
		System_Display[ STR_Tinsp_10 ]  = "吸气时间最大值10.0s";
		System_Display[ STR_Tisnp_Tpause_10 ]  = "吸气时间最大值10.0s";
		System_Display[ STR_Psupp_70 ]  = "压力支持最大值70cmH②O";
		System_Display[ STR_Simv_Freq_40 ]  = "SIMV模式下呼吸频率最大值40bpm";
		System_Display[ STR_Freq_100 ]  = "呼吸频率最大值100bpm";
		System_Display[ STR_Peep_30 ]  = "呼气末正压最大值30cmH②O";
		System_Display[ STR_Pause_50 ]  = "吸气暂停最大值60%";    //han
		System_Display[ STR_Esense_80 ]  = "呼气触发灵敏度最大值80%";
		System_Display[ STR_Tslop_2 ]  = "压力上升时间最大值2.0s";
		System_Display[ STR_Fsense_20 ]  = "流量触发最大值20LPM";
		System_Display[ STR_Psense_20 ]  = "压力触发最大值20cmH②O";
// 		System_Display[ STR_Vt_20 ]  =  "潮气量最小值";
		System_Display[ STR_Vt_50 ]  = "潮气量最小值";
		System_Display[ STR_Pinsp_5 ]  = "压力控制最小值5cmH②O";
		System_Display[ STR_Ie_110 ]  = "吸呼比最小值1:10";
		System_Display[ STR_Tinsp_1 ]  = "吸气时间最小值0.1s";
		System_Display[ STR_Freq_1 ]  = "SIMV模式下呼吸频率最小值1bpm";
		System_Display[ STR_Freq_4 ]  = "呼吸频率最小值4bpm";  //sun add 20140416
		System_Display[ STR_Psupport_0 ]  = "压力支持最小值0cmH②O";
		System_Display[ STR_Peep_Off ]  =  "呼气末正压最小值";//ylk 20180428 delete 0cmH②O
		System_Display[ STR_Pause_0 ]  =  "吸气暂停最小值0%";
		System_Display[ STR_Tslop_0 ]  = "压力上升时间最小值0.0s";
#if(LOGO_FLAG != PENLON_LOGO)//ylk add 20180709
		System_Display[ STR_Fsense_5 ]  = "流量触发最小值0.5LPM";
		System_Display[ STR_Psense_0 ]  = "压力触发最小值0cmH②O";
#else
		System_Display[ STR_Fsense_5 ]  = "流量触发最小值1.0LPM";
		System_Display[ STR_Psense_0 ]  = "压力触发最小值1cmH②O";
#endif
		System_Display[ STR_Esense_5 ]  = "呼气触发灵敏度最小值5%";
		System_Display[ STR_Pause ]  = "吸气暂停";//ylk 20180428 modify 屏气时间->吸气暂停
		System_Display[ STR_Vt ]  = "潮气量";
		System_Display[ STR_Tinsp_Tpause_1 ]  = "吸气时间最小值0.1s";
		System_Display[ STR_Tinsp_Tslop ]  = "压力上升时间不大于吸气时间";
		System_Display[ STR_Flow_High ]  = "最大流量值限制";
		System_Display[ STR_Flow_Low ]  = "最小流量值限制";
		System_Display[ STR_Psupport_Peep_70 ]  = "压力支持加呼气末正压不高于70cmH②O";
		System_Display[ STR_Pinsp_Peep_70 ]  = "压力控制加呼气末正压不高于70cmH②O";
		System_Display[ STR_Pinsp_Peep_Ph ]  = "压力控制加呼气末正压低于压力报警上限";
		System_Display[ STR_Psupp_Peep_Ph ]  = "压力支持加呼气末正压低于压力报警上限";
		System_Display[ STR_Peep_Ph ]  = "呼气末正压低于压力报警上限";
		System_Display[ STR_Texp_Low ]  = "最小呼气时间限制";
		System_Display[ STR_Backup_VCV ]  = "备份通气VCV模式激活";
		System_Display[ STR_Backup_PCV ]  = "备份通气PCV模式激活";
		System_Display[ STR_Pinsp_30 ]  = "压力控制确定大于30cmH②O？";
		System_Display[ STR_Pinsp_50]  = "压力控制确定大于50cmH②O？";
		System_Display[ STR_Peep_Psupport_30 ]  = "压力支持加呼气末正压大于30cmH②O？";
		System_Display[ STR_Peep_Psupport_50 ]  = "压力支持加呼气末正压大于50cmH②O？";
		System_Display[ STR_Peep_20 ]  = "呼气末正压确定大于20cmH②O？";
		System_Display[ STR_Ie_1 ]  = "吸呼比设置确定大于1:1(反比通气)？";	
		System_Display[ STR_Sec_1 ]  = "秒";	
		System_Display[ STR_USA_Standard ]  = "已切换至美标!";	
		System_Display[ STR_EN_Standard ]  = "已切换至欧标!!";		
// 		System_Display[ STR_ET ]  = "呼末";	
// 		System_Display[ STR_FI ]  = "吸入";
// 		System_Display[ STR_FREQ_English ] ="f";
		System_Display[ STR_Value2 ]  = " 监测参数 2";	
		System_Display[ STR_Set_EFM_Cal_Baro_Pressure ]  = "请设置EFM校验\n大气压力值";	
		System_Display[ STR_O2 ]  = "氧气";
		System_Display[ STR_N20 ]  ="笑气"; 
		System_Display[ STR_AIR ]  =  "空气";
		System_Display[ STR_AA ]  =  "麻醉气";
		System_Display[ STR_Cal_Error ]  =  "校验出错!";
		System_Display[ STR_Boot_Check_Error ]  =  "BOOT错误";
// 		System_Display[ STR_O2_AD_Beyond ]   =  "氧气AD值超范围";
// 		System_Display[ STR_AIR_AD_Beyond ]	 =  "空气AD值超范围";
		System_Display[ STR_Software_Version] = "软件版本 1. 0"	;	
		System_Display[ STR_User_BP_Set ]	       = "使用地大气压设置:" ;
		System_Display[ STR_Cal_BP_set  ]	       = "校验地大气压设置:" ;		
		System_Display[ STR_System_Test]     = "系统测试";
		//System_Display[ STR_Preuse_Test]     = "是否进行开机自检?";
// 		System_Display[ STR_Flowmeter_Air_Test  ]	       = "Flow\nmeter \nAir test" ;	
		/*************************************************end*******************************************************/
		System_Display[ STR_Start]     = "开始";
		System_Display[ STR_Alarm_Silence ]	 = "报警静音";//
		System_Display[ STR_Alarm_Reset ]	 = "报警复位";
		System_Display[ STR_Trend]	 = "    趋势   ";
		System_Display[ STR_Trend_graph  ]	 = "趋势图";    
		System_Display[ STR_STANDBY	]				 = "待机" ;  
		System_Display[STR_Dirver_Gas_Air] = "驱动气体为空气,不能关闭开关!";
// 		System_Display[STR_Dirver_Gas_Change_Air] = "驱动气体已切换至空气!";
// 		System_Display[STR_Dirver_Gas_Change_O2] = "驱动气体已切换至氧气!";
		System_Display[STR_Close_Air_Switch] = "是否关闭空气气源开关?";
		System_Display[STR_Change_Dirver_Gas] = "是否更改驱动气体?";
		System_Display[STR_DRIVER] = "驱动气";	
		System_Display[STR_CLEARTREND] = "修改时间将清空趋势数据，确认要修改?";
		System_Display[ STR_Russian  ]	       = "Русский";	
		System_Display[STR_MODES_SIGH] = " +叹息";
		System_Display[ STR_GUI ]  =  "显示";
		System_Display[ STR_BDU ]  =  "控制";
		System_Display[ STR_PMU ]  =  "外设";
		System_Display[ STR_FiHAL ]  =  "吸入氟烷";
		System_Display[ STR_FiENF ]  =  "吸入安氟醚";
		System_Display[ STR_FiISO ]  =  "吸入异氟醚";
		System_Display[ STR_FiSEV ]  =  "吸入七氟醚";
		System_Display[ STR_FiDES ]  =  "吸入地氟醚";
		System_Display[ STR_EtHAL ]  =  "呼末氟烷";
		System_Display[ STR_EtENF ]  =  "呼末安氟醚";
		System_Display[ STR_EtISO ]  =  "呼末异氟醚";
		System_Display[ STR_EtSEV ]  =  "呼末七氟醚";
		System_Display[ STR_EtDES ]  =  "呼末地氟醚";

		System_Display[ STR_HAL ]    =  "氟烷";
		System_Display[ STR_ENF ]    =  "安氟醚";
		System_Display[ STR_ISO ]    =  "异氟醚";
		System_Display[ STR_SEV ]    =  "七氟醚";
		System_Display[ STR_DES ]    =  "地氟醚";

		System_Display[ STR_SPO2VER ]		 =  "血氧传感器版本";
		System_Display[ STR_AVERTIME ]		 =  "平均时间";
		System_Display[ STR_SENSITIVITY ]	 =  "灵敏度";
		System_Display[ STR_SMARTTONE ]		 =  "Smart Tone";
		System_Display[ STR_FASTSAT ]		 =  "Fast SAT";
		System_Display[ STR_MCUVER ]		 =  "MCU:";
		System_Display[ STR_DSPVER ]		 =  "DSP:";
		System_Display[ STR_LASTO2CALIB ]	 =  "上一次氧气量程校准失败!";
		System_Display[ STR_MODE_Round ]     =  "○";

		//选配
		System_Display[ STR_MODE_SIMV_P_PSV ]  =  "SIMV-P & PSV 模式";
		System_Display[ STR_MODE_PRVC ]        =  "PRVC 模式";
		System_Display[ STR_MODE_INFANT ]      =  "小儿模式";//ylk 20180428 modify 婴儿->小儿
		System_Display[ STR_MODE_CO2 ]         =  "二氧化碳监测模块";
		System_Display[ STR_MODE_SpO2 ]        =  "血氧监测模块";
		System_Display[ STR_MODE_Driver ]      =  "驱动气体模式";
		System_Display[ STR_MODE_O2 ]          =  "氧浓度监测模块";//ylk 20180428 modify 氧气->氧浓度
		System_Display[ STR_MODE_Masimo_IRMA ]            =  "Masimo IRMA";
		System_Display[ STR_MODE_Masimo_ISA ]             =  "Masimo ISA";
		System_Display[ STR_MODE_Masimo_ISA_O2 ]          =  "Masimo ISA+O②";
		System_Display[ STR_MODE_Masimo_SpO2 ]            =  "Masimo SpO② 监测模块";
		System_Display[ STR_MODE_AA_CO2 ]                 =  "AA & CO② 监测模块";

		//开机自检
		System_Display[STR_SelfTest_Ask]                      =  "是否启动系统测试";//ylk 20180428 modify
		System_Display[STR_SelfTest_Exit]                     =  "退出";
		System_Display[STR_SelfTest_Manual_Circuit]           =  "手动泄漏测试";//
		System_Display[STR_SelfTest_Auto_Circuit]             =  "机控系统测试";//
		System_Display[STR_SelfTest_Last_Manual_Circuit]      =  "上次手动泄漏测试";//
		System_Display[STR_SelfTest_Last_Auto_Circuit]        =  "上次机控系统测试";//
		System_Display[STR_SelfTest_Checking]                 =  "正在检测...";
		System_Display[STR_SelfTest_Result_State]             =  "√通过    ×失败    —跳过";
		System_Display[STR_SelfTest_Symbol_Pass]              =  "√";
		System_Display[STR_SelfTest_Symbol_Skip]              =  "—";
		System_Display[STR_SelfTest_Symbol_Fail]              =  "×";
		System_Display[STR_Pass]                            =  "通过";
		System_Display[STR_Fail]                            =  "失败";
		System_Display[STR_Skip]                            =  "跳过";
		System_Display[STR_Finish]                          =  "完成";
		System_Display[STR_Retry]                           =  "重新检测";
		System_Display[STR_SelfTest_Err_Timeout]            =  "超时";
		//Check item
		System_Display[STR_SelfTest_Manual_Check]             =  "手动泄露";//ylk 20180428 modify
		System_Display[STR_SelfTest_Baro_Check]               =  "大气压力";
		System_Display[STR_SelfTest_Gas_Check]                =  "气源压力";
		System_Display[STR_SelfTest_FG_Flow_Check]            =  "新鲜气体流量传感器";
		System_Display[STR_SelfTest_CircuitFlow_Check]        =  "流量传感器";//ylk 20180428 delete 回路
		System_Display[STR_SelfTest_Pressure_Check]           =  "压力传感器";
		System_Display[STR_SelfTest_CircuitCompliance_Check]  =  "呼吸回路顺应性";
		System_Display[STR_SelfTest_CircuitLeakage_Check]     =  "呼吸回路泄漏量";
		System_Display[STR_SelfTest_Battery_Check]            =  "电池";
		System_Display[ STR_SelfTest_OxygenSensor_Check ]     =  "氧浓度传感器";
		//Manual Prompt
		System_Display[STR_SelfTest_Manual_Prompt_1]         =  "1.设置手动/机控开关至手动状态";
		System_Display[STR_SelfTest_Manual_Prompt_2]         =  "2.连接呼吸管路，并将Y型三通连接至回路测试堵头";//ylk 20180428 modify
		System_Display[STR_SelfTest_Manual_Prompt_3]         =  "3.安装手动呼吸皮囊";
		System_Display[STR_SelfTest_Manual_Prompt_4]         =  "4.确保气源压力在280 - 600 kPa之间";
		System_Display[STR_SelfTest_Manual_Prompt_5]         =  "5.确保二氧化碳吸收罐和积水杯安装正确";
		System_Display[STR_SelfTest_Manual_Prompt_6]         =  "6.关闭所有流量计";
		System_Display[STR_SelfTest_Manual_Prompt_7]         =  "7.确保麻醉蒸发器处于关闭状态";
		System_Display[STR_SelfTest_Manual_Prompt_8]         =  "8.设置ACGO开关为关闭状态(当选配ACGO功能时)";//
#if (LOGO_FLAG == PENLON_LOGO)		
		System_Display[STR_SelfTest_Manual_Prompt_9]         =  "9.设置APL阀为50 cmH②O";
		System_Display[STR_SelfTest_Manual_Prompt_11]        =  "11.设置ACGO开关至开启状态（单管路连接）";
		System_Display[STR_SelfTest_Manual_Prompt_12]        =  "12.点击“开始”按键启动手动泄漏测试";
#else
		System_Display[STR_SelfTest_Manual_Prompt_9]         =  "9.设置APL阀为70 cmH②O";	
		System_Display[STR_SelfTest_Manual_Prompt_11]        =  "11.点击“开始”按键启动手动泄漏测试";
// 		System_Display[STR_SelfTest_Manual_Prompt_12]        =  "12.点击“开始”按键启动手动泄漏测试";
#endif
		System_Display[STR_SelfTest_Manual_Prompt_10]        =  "10.按下O2+按键直至回路压力表在30 - 40 cmH2O之间";
		//Automatic Prompt 1
		System_Display[STR_SelfTest_Auto_Prompt_1]      =  "1.设置手动/机控开关至机控状态";
		System_Display[STR_SelfTest_Auto_Prompt_2]      =  "2.将呼吸管路Y型三通对大气";//将呼吸管路Y型接头对准大气
		System_Display[STR_SelfTest_Auto_Prompt_3]      =  "3.确保二氧化碳吸收罐和积水杯安装正确";
		System_Display[STR_SelfTest_Auto_Prompt_4]      =  "4.关闭所有流量计";
		System_Display[STR_SelfTest_Auto_Prompt_5]      =  "5.确保麻醉蒸发器处于关闭状态";
		System_Display[STR_SelfTest_Auto_Prompt_6]      =  "6.设置ACGO开关为关闭状态(当选配ACGO功能时)";
		System_Display[STR_SelfTest_Auto_Prompt_7]      =  "7.确保风箱折叠囊处于底部";
		System_Display[STR_SelfTest_Auto_Prompt_8]      =  "8.点击开始按键启动机控系统测试";
		//Automatic Prompt 2
		System_Display[STR_SelfTest_Auto_Prompt_10]      = "1.设置手动/机控开关至手动状态并堵住手动皮囊端口";
		System_Display[STR_SelfTest_Auto_Prompt_11]      = "2.调节氧气流量为10 L/min";
		System_Display[STR_SelfTest_Auto_Prompt_12]      = "3.点击开始按键";//钮改为键 
		//Automatic Prompt 2
		System_Display[STR_SelfTest_Auto_Prompt_13]      =  "1.设置手动/机控开关至机控状态";
		System_Display[STR_SelfTest_Auto_Prompt_14]      =  "2.连接呼吸管路，并将Y形三通连接至回路测试堵头";//患者 -》 呼吸
		System_Display[STR_SelfTest_Auto_Prompt_15]      =  "3.关闭所有流量计";
		System_Display[STR_SelfTest_Auto_Prompt_16]      =  "4.按下O2+按键直至风箱折叠囊上升至最顶端";
#if (LOGO_FLAG == PENLON_LOGO)		
		System_Display[STR_SelfTest_Auto_Prompt_17]      =  "5.设置ACGO开关为打开状态(单管路连接)";
		System_Display[STR_SelfTest_Auto_Prompt_18]      =  "6.点击“开始”按键";
#else
		System_Display[STR_SelfTest_Auto_Prompt_17]      =  "5.点击“开始”按键";
#endif		//
		System_Display[STR_SelfTest_Err_LeakOut]             =  "泄漏超范围!";//Leakage over range
		System_Display[STR_SelfTest_Err_BagVent]             =  "手动/机控位置错误!";//Bag/Vent switch position error!
		System_Display[STR_SelfTest_Err_Circuit_NoConnect]   =  "管路未连接!";//Circuit not connect!
		System_Display[STR_SelfTest_Err_ValveLeak]           =  "确保流量计关闭!";//比例阀或新鲜气体阀漏气故障
		System_Display[STR_SelfTest_Err_Sensor]              =  "请进行氧浓度传感器校验后重新测试!";//
		System_Display[STR_SelfTest_Err_SensorTest]          =  "传感器测试错误!";
		System_Display[STR_SelfTest_Err_Sensor_NoConnect]    =  "氧浓度传感器失效或者未连接!";
		System_Display[STR_SelfTest_Err_Insp_FlowSensor]     =  "吸气流量传感器测试失败!";
		System_Display[STR_SelfTest_Err_Exp_FlowSensor]      =  "呼气流量传感器测试失败!";
		System_Display[STR_SelfTest_Err_InspExp_FlowSensor]  =  "吸气和呼气流量传感器测试失败!";
		System_Display[STR_SelfTest_Err_Compliance]          =  "顺应性测试失败!";
		System_Display[STR_SelfTest_Disconnect_AC]           =  "请断开交流电!";
		System_Display[STR_SelfTest_Connect_AC]              =  "请连接交流电!";
		System_Display[STR_SelfTest_Err_Gas_Supply]          =  "气源故障!";//
		System_Display[STR_SelfTest_Err_O2_Supply]           =  "氧气气源故障!";
		System_Display[STR_SelfTest_Err_N2O_Supply]          =  "笑气气源故障!";
		System_Display[STR_SelfTest_Err_Air_Supply]          =  "空气气源故障!";
		System_Display[STR_SelfTest_Err_O2_N2O_Supply]       =  "氧气与笑气气源故障!";
		System_Display[STR_SelfTest_Err_O2_Air_Supply]       =  "氧气与空气气源故障!";
		System_Display[STR_SelfTest_Err_N2O_Air_Supply]      =  "笑气与空气气源故障!";
		System_Display[STR_DISP_ERR_NUM]					 =  "错误编号";
		System_Display[STR_Empty]                            =  "";

		break;
#else
	case 2://西班牙语
		System_Display[ STR_Mode ]   = "  Modos  ";
		System_Display[ STR_Active ]  = "Active";//ylk add	
		System_Display[ STR_Control ]   = "Controles";
		System_Display[ STR_Alarm ]   = "Alarmas";
		System_Display[ STR_System ]   = "Sistema";
		System_Display[ STR_Monitoring ]   = "Monitoreo";
		System_Display[ STR_Cancel ]   = "Cancelar";
		System_Display[ STR_Confirm ]	  = "Confirmar";
		System_Display[ STR_Volume ]	  = "Volumen"; 
		System_Display[ STR_Pressure ]	  = "Presión";
		System_Display[ STR_Invalidtouch ]	  = ") puntos no se calibran";
		System_Display[ STR_Alarm_Log ]	= "  Recuerdo De Alarma  ";
		System_Display[ STR_FREQ ]	= "FREQ";
		System_Display[ STR_MV ]	= "MV";
		System_Display[ STR_FIO2 ]	= "FiO②";
		System_Display[ STR_VTE ]	= "VTe";
		System_Display[ STR_Apnea_time ]	= "Tiempo de Apnea";
		System_Display[ STR_Date ]	= "Fecha "; 
		System_Display[ STR_Time ]	= "Hora";  
		System_Display[ STR_Event ]	= "Evento";  
		System_Display[ STR_Info ]	= "  Información  "; 
		System_Display[ STR_Set ]	= "  Ajustes  "; 
		System_Display[ STR_Date_Time ]	= "  Fecha & Tiempo  ";
		System_Display[ STR_Calib ]	= "  Calibración  "; 	
// 		System_Display[ STR_GUI ]	= "GUI";
		System_Display[ STR_Baro_pressure ]	= "Barometrica"; 
		System_Display[ STR_Gas_Supply_Pressure ]	= "Presión de suministro de gas";
		System_Display[ STR_Run_Time ]	= "Tiempo De Trabajo                 ";
		System_Display[ STR_Hours ] = "Horas";
		System_Display[ STR_touch ]	= "Haga clic en el centro de la cruz para verificar,\n           ("            ;
		System_Display[ STR_Sigh_Breath_Every ]	= "Suspiros";//"Intervalos De Suspiros";
		System_Display[ STR_SetAAType ]	= "Set AA Type";
		System_Display[ STR_AA_O2 ]	= "AA&O②"; 
		System_Display[ STR_Drager_O2 ]	= "Drager + O② Module"; 
		System_Display[ STR_Drager_Accuracy ]	= "Gas Module Unspecified Accuracy!"; //Drager未达到高精度
		System_Display[ STR_Drager ]	= "Drager Module"; 
		System_Display[ STR_Language ]	= "Idioma"; 
		System_Display[ STR_Trigger_Type ]	= "Tipo De Disparo ";
		System_Display[ STR_CO2_unit ]	= "Unidad EtCO②";
		System_Display[ STR_Loudness ]	= "Volumen";
		System_Display[ STR_Pressure_trigger ]	= "Disparo Por Presión";
		System_Display[ STR_Flow_trigger ]	= "Disparo Por Flujo";
		System_Display[ STR_Year ]	= "Año"; 
		System_Display[ STR_Month ]	= "Mes"; 
		System_Display[ STR_Day ]	= "Día"; 
		System_Display[ STR_Hour ]	= "Hora"; 
		System_Display[ STR_Minutes ]  = "Minuto";
		System_Display[ STR_Apply ]	= "Aplicar";
		System_Display[ STR_Pressure_Sensor_Cal ]	= "Calibración de Sensor de Presión"; 
		System_Display[ STR_Flow_Sensor_Cal ]	= "Calibración de Sensor de Flujo";
		System_Display[ STR_Oxygen_Cell_Cal ]	= "Calibración de Celda de O② ";
		System_Display[ STR_Service_Mode ]	= "Modos de Servicio"; 
		System_Display[ STR_Flow_Zero_Cal ]	= "Calibración de Cero de Caudalímetro  ";///
		System_Display[ STR_PhaseIn_Zero_Cal ]	= "Multigas Módulo Cero";///
		System_Display[ STR_Touch_Cal ]	= "Calibración pantalla táctil";
		System_Display[ STR_Touch_To_Cal ]	= "Presione en el centro de la cruz para calibrar"; 
		System_Display[ STR_Set_O2_Concen ]	= "Seleccione Comprobar la concentración de O②"; 

		System_Display[ STR_O2_10LMin ]	    = "Cuando esté en el modo de bolsa, desconecte los tubos de respiración, enchufe el \
											  puerto de la bolsa y ajuste el flujo de oxígeno del caudalímetro más de 10 L / min." ;
		System_Display[ STR_Air_10LMin ]	= "Cuando esté en el modo de bolsa, desconecte los tubos de respiración, \
											  enchufe el puerto de la bolsa y ajuste el flujo de aire del caudalímetro más de 10 L / min. O Retire el sensor de O2 del sistema respiratorio.";

		System_Display[ STR_Return ]	= "Salida";
// 		System_Display[ STR_Flow_Close ]	= "Determinar si la válvula \nde flujo esta cerrada";///
		System_Display[ STR_Iso_gas_moudle ]	= "Retire los módulos de gases anestésicos \ny colocados en el aire";///
		System_Display[ STR_GASNOZERO  ] = "Módulo de gas no puede ser puesto a cero!";
		System_Display[ STR_Input_password ]	= "Introducir Contrasena:";	
		System_Display[ STR_Password_Error ]	= "Error de contraseña!";	
		System_Display[ STR_N2O_Open]	= "N2O abierto!";	///
		System_Display[ STR_Insp_Valve_Cal ]	= "Calibración de Válvula Inspiratoria";	
		System_Display[ STR_Exp_Flow_Sensor_Cal ]	= "Calibración de Sensor Expiratorio";	
		System_Display[ STR_Exp_Valve_Cal ]	= "Calibración de Válvula Expiratorio";
		System_Display[ STR_Flowmeter_Cal ]	= "Calibración de medidor de flujo .";///
		System_Display[ STR_Calibrating ]	= "Calibrando...";
		System_Display[ STR_Cal_Failed ]	= "Fallo De Calibración!";	
		System_Display[ STR_Cal_Success ]	= "Calibración Exitosa!";	
		System_Display[ STR_Waiting ]	= "Por favor, espere......";
		System_Display[ STR_No_Progam ]	= "Ningún programa puede ser actualizado!";
		System_Display[ STR_No_Response ]	= "\nno hay respuesta!";
		System_Display[ STR_Continue ]	= "Continuar";
		System_Display[ STR_Restart ]	= "Por favor, reinicie su máquina!";	
		System_Display[ STR_Full_Serial ]	= "Ingresar contraseña completa,por favor!";	
		System_Display[ STR_Serial_Correct ]	= "Contraseña Correcta!";	
		System_Display[ STR_Serial_Failure ]	= "Contraseña Incorrecta!";	
		System_Display[ STR_Entry_Serial ]	= "Introducir Contraseña:";
		System_Display[ STR_Backspace ]	= "Retroceso";
		System_Display[ STR_Battery_Cal ]	= "Calibración de la batería";
		System_Display[ STR_Battery_Voltage ]	= "Tensión de la batería";
		System_Display[ STR_Last_Battery_Cal ]	= "Ultima calibración de bacteria:";	
		System_Display[ STR_Cal_Date ]	= "   Fecha de calibración:";	
		System_Display[ STR_Cal_Duration ]	= "   Calibración por:     ";	
		/************************************************start*************************************************/
		System_Display[ STR_All ]	= "todo"	;///
		System_Display[ STR_Up ]	= "Previo" ;///
		System_Display[ STR_Input ]	= "Por favor, Introduzca el valor" ;///
		System_Display[ STR_Input_Again ]	= "Por favor, Introduzca el valor \nde nuevo";///
		System_Display[ STR_Data_Not_Save ]	= "Los datos no estan guardados \nconfirme o cancele ?";///
		System_Display[ STR_Rec_Error ]	= "Error de recepción";	///
		System_Display[ STR_Rec_Timeout ]	= "Recibe de tiempo \nde espera";	///
		System_Display[ STR_Zero_Beyond ]	= "Punto más allá del cero";///	
		System_Display[ STR_EEPROM_Write_Error ]	= "Error de escritura EEPROM";///	
		System_Display[ STR_EEPROM_Read_Error ]  = "Error de lectura EEPROM";///
		System_Display[ STR_N2O_AD_Beyond ]  = "Valor AD supera el rango \nde óxido nitroso";///
		System_Display[ STR_Check_N2O ]  = "Por favor compruebe \nel N2O";///
		System_Display[ STR_Check_AIR ]  = "Por favor compruebe \nel aire";	///
		/************************************************end*************************************************/
		System_Display[ STR_Value1 ]  = "  Valores 1";	
		//System_Display[ STR_Value1 ]  = "  Valores";
// 		System_Display[ STR_Graphics ]  = "Gráficos";	
		/************************************************start*************************************************/
// 		System_Display[ STR_Fresh_gas ] = "Gas Fresco";	///
// 		System_Display[ STR_Total_Gas ] = "Flujo total";///
// 		System_Display[ STR_Waveform ]	= "Formas De Onda";///
// 		System_Display[ STR_Rapid_Monitor ]	= "Monitoreo Rápido";///
		/************************************************end*************************************************/
		System_Display[ STR_Paw ]	= "Paw";	
		System_Display[ STR_Pleth ]	= "Pleth";	
		System_Display[ STR_No_Vent_To_Patient ]	= "     Ventilación No Entregada Al Paciente";	
		System_Display[ STR_Activate_Standby ]	=  "Activa Modo En Espera";	
		System_Display[ STR_Sec ]	= "Sec";///
		System_Display[ STR_Backup ]	= "  Reserva  ";
		System_Display[ STR_Paw_V_Loop ]	= "Paw-V";
		System_Display[ STR_V_Flow_Loop ]	= "V-Flujo";		
		System_Display[ STR_ON ]	= "ON";	///
		System_Display[ STR_OFF ]	= "OFF";///
		System_Display[ STR_English ]	= "English";		
		System_Display[ STR_Pulse ]	= "Pulso";
		System_Display[ STR_SPO2 ]	= "SpO②";	
		System_Display[ STR_PI ]	= "PI";
		System_Display[ STR_EFM ]	= "EFM";///
		System_Display[ STR_SoftwareVersion ]	= "Versión De Software";
		System_Display[ STR_Option ]	= "Opción";//选配
		System_Display[ STR_Enter ]	= "Entrar";
		System_Display[ STR_EXIT ]	= "Salida";
// 		System_Display[ STR_Layout2 ] = "Distribución 2";
		System_Display[ STR_Limits1 ] = "  Limites 1  ";
		System_Display[ STR_Limits2 ] = "  Limites 2  ";
		System_Display[ STR_Limits3 ] = "  Limites 3  ";///
		System_Display[ STR_N2O_Close ] = "N②O Cerca";	///
		System_Display[ STR_Max_Flow ]  = "Max Flow";
		System_Display[ STR_Insp_Time ]  = "Inspiratory Time";
		System_Display[ STR_I_E ]  = "I : E";		
		System_Display[ STR_Pressure_Mode ]  = "PRESIÓN";
		System_Display[ STR_Chinese ]  = "简体中文";	
		System_Display[ STR_Turkish ]  = "TÜRKÇE";
		System_Display[ STR_Spanish ]  = "Español";
		System_Display[ STR_Flow ]  = "Flujo"   ;
		System_Display[ STR_Total_Flow_Eng]  = "Flujo total";///
		System_Display[ STR_N2O ]  = "N②0"	   ;///
		System_Display[ STR_AA1 ]  = "AA1"	   ;///
		System_Display[ STR_AA2 ]  = "AA2"	   ;///
		System_Display[ STR_CO2 ]  = "CO②"	   ;
		System_Display[ STR_EtCO2 ]  = "EtCO②"	;	
		System_Display[ STR_FiCO2 ]  = "FiCO②"	;		
		System_Display[ STR_Set_Mode ]  = "  Modos  ";	
		System_Display[ STR_Upgrade ]  = "Actualizar"	;
		System_Display[ STR_Cancel_Upgrade ]  = "Cancelar";
		System_Display[ STR_Continue_Upgrade ]  = "Continuar";	

		System_Display[ STR_VT_1500 ]  = "Vt <= ";
		System_Display[ STR_Pinsp_70 ]  = "Pinsp <= 70 cmH②O";
		System_Display[ STR_Ie_41 ]  =  "I:E <= 4:1";
		System_Display[ STR_Tinsp_10 ]  = "Tinsp <= 10.0 s";
		System_Display[ STR_Tisnp_Tpause_10 ]  = "Tinsp - Tpause <= 10.0 s";
		System_Display[ STR_Psupp_70 ]  = "Psupp <= 70 cmH②O";
		System_Display[ STR_Simv_Freq_40 ]  = "SIMV FREQ <= 40 bpm";
		System_Display[ STR_Freq_100 ]  =  "FREQ <= 100 bpm";
		System_Display[ STR_Peep_30 ]  = "PEEP <= 30 cmH②O";
		System_Display[ STR_Pause_50 ]  = "Pause <= 60%";    //han
		System_Display[ STR_Esense_80 ]  = "Esens <= 80%";
		System_Display[ STR_Tslop_2 ]  = "Tslope <= 2.0s";
		System_Display[ STR_Fsense_20 ]  = "Fsens <= 20.0 LPM";
		System_Display[ STR_Psense_20 ]  = "Psens <= 20 cmH②O";

// 		System_Display[ STR_Vt_20 ]  =  "Vt Low Limit";
		System_Display[ STR_Vt_50 ]  = "Vt >= ";
		System_Display[ STR_Pinsp_5 ]  =  "Pinsp >= 5 cmH②O";
		System_Display[ STR_Ie_110 ]  = "I:E >= 1:10";
		System_Display[ STR_Tinsp_1 ]  = "Tinsp >= 0.1 s";
		System_Display[ STR_Freq_1 ]  = "SIMV FREQ >= 1 bpm";
		System_Display[ STR_Freq_4 ]  = "FREQ >= 4 bpm";  //sun add 20140416
		System_Display[ STR_Psupport_0 ]  = "Psupp >= 0 cmH②O";
		System_Display[ STR_Peep_Off ]  = "PEEP OFF";
		System_Display[ STR_Pause_0 ]  = "Pause >= 0%";;
		System_Display[ STR_Tslop_0 ]  = "Tslope >= 0.0s";
#if(LOGO_FLAG != PENLON_LOGO)//ylk add 20180709
		System_Display[ STR_Psense_0 ]  = "Psens >= 0 cmH②O";
		System_Display[ STR_Fsense_5 ]  = "Fsens >= 0.5 LPM";
#else
		System_Display[ STR_Psense_0 ]  = "Psens >= 1 cmH②O";
		System_Display[ STR_Fsense_5 ]  = "Fsens >= 1.0 LPM";
#endif			
		System_Display[ STR_Esense_5 ]  = "Esens >= 5%";
		System_Display[ STR_Pause ]  = "Pause";
		System_Display[ STR_Vt ]  = "VT";
		//
		System_Display[ STR_Tinsp_Tpause_1 ]  = "Tinsp - Tpause >= 0.1 s";
		System_Display[ STR_Tinsp_Tslop ]  =  "Tinsp >= Tslope";
		System_Display[ STR_Flow_High ]  = "Flow High Limit";
		System_Display[ STR_Flow_Low ]  = "Flow Low Limit";
		//
		System_Display[ STR_Psupport_Peep_70 ]  = "Psupp + PEEP <= 70 cmH②O";
		System_Display[ STR_Pinsp_Peep_70 ]  =  "Pinsp + PEEP <= 70 cmH②O";
		System_Display[ STR_Pinsp_Peep_Ph ]  = "Pinsp + PEEP < Ph";
		System_Display[ STR_Psupp_Peep_Ph ]  =  "Psupp + PEEP < Ph";
		System_Display[ STR_Peep_Ph ]  = "PEEP < Ph";
		System_Display[ STR_Texp_Low ]  = "Texp Low Limit";
		//
		System_Display[ STR_Backup_VCV ]  = "Backup VCV Actived";
		System_Display[ STR_Backup_PCV ]  =  "Backup PCV Actived";
		//
		System_Display[ STR_Pinsp_30 ]  = "Pinsp > 30 cmH②O ? Confirm";
		System_Display[ STR_Pinsp_50]  = "Pinsp > 50 cmH②O ? Confirm";
		System_Display[ STR_Peep_Psupport_30 ]  = "PEEP + Psupp > 30 cmH②O ? Confirm";
		System_Display[ STR_Peep_Psupport_50 ]  = "PEEP + Psupp > 50 cmH②O ? Confirm";
		System_Display[ STR_Peep_20 ]  = "PEEP > 20 cmH②O ? Confirm";
		System_Display[ STR_Ie_1 ]  = "I:E > 1:1 ? Confirm";			
		System_Display[ STR_Sec_1 ]  = "Sec";		
		System_Display[ STR_USA_Standard ]  = "USA Standard!";	
		System_Display[ STR_EN_Standard ]  = "EN Standard!";	
		System_Display[ STR_FiN2O ]  = "FiN②O";		
// 		System_Display[ STR_ET ]  = "Et";	
// 		System_Display[ STR_FI ]  = "Fi";		
// 		System_Display[ STR_FREQ_English ] ="f";
		System_Display[ STR_Value2 ]  ="Valores 2";		
		System_Display[ STR_Set_EFM_Cal_Baro_Pressure ]  = "Please set the EFM\n checking air\n pressure value";		
		System_Display[ STR_O2 ]  = "O②";
		System_Display[ STR_N20 ]  ="N②O"; 
		System_Display[ STR_AIR ]  =  "Aire";
		System_Display[ STR_Cal_Error ]  =  "Fallo De Calibración! "; 
		System_Display[ STR_Boot_Check_Error ]  =  "Error de autocomprobación\n Boot!";		
// 		System_Display[ STR_O2_AD_Beyond ]   =  "O2 Muestreo AD allá!";
// 		System_Display[ STR_AIR_AD_Beyond ]	 =  "Air Muestreo AD allá!";			
		System_Display[ STR_Software_Version] = "Versión del software 1. 0"	;	
		System_Display[ STR_User_BP_Set ]	       = "Ajuste de PB de\nUsuario:" ;
		System_Display[ STR_Cal_BP_set  ]	       = "Ajuste de PB de Calibración:" ;		
// 		System_Display[ STR_Flowmeter_Air_Test  ]	       = "prueba de aire de medidor de flujo  " ;	
		System_Display[ STR_Russian  ]	       = "Русский" ;  	
		System_Display[ STR_System_Test]     = "Prueba del sistema";
		System_Display[ STR_Preuse_Test]     = "Iniciar la prueba previa a su uso?";
		System_Display[ STR_Start]     = "comienzo";
		System_Display[ STR_Alarm_Silence ]	 = "Alarma en silencio";
		System_Display[ STR_Alarm_Reset ]	 = "Alarma en reinicio";
		System_Display[ STR_Trend]	 = "Tendencia";
		System_Display[ STR_Trend_graph  ]	 = "Gráfico de tendencia"; 
		System_Display[ STR_STANDBY	]				 = "En espera";  
		System_Display[STR_Dirver_Gas_Air] = "Gas de accionamiento es aire!, no puede cerrar el interruptor!";
// 		System_Display[STR_Dirver_Gas_Change_Air] = "Gas impulsor tiene que cambiar a aire!";
// 		System_Display[STR_Dirver_Gas_Change_O2] = "Gas impulsor tiene que cambiar a O②!";
		System_Display[STR_Close_Air_Switch] = "Ya sea cerca del interruptor de aire?";
		System_Display[STR_Change_Dirver_Gas] = "Ya sea para cambiar de gas conductor?";
		System_Display[STR_DRIVER] = "Conductor";
		System_Display[STR_CLEARTREND] = "Modificación del tiempo borrará los datos de tendencia?";

		System_Display[ STR_FREQ ]	= "FREQ";
		System_Display[ STR_Mvspn ]  =  "Mvspn";//
		System_Display[ STR_Fspn ]   =  "fspn" ;//
		System_Display[ STR_PPEAK ]	 =  "Ppeak";//
		System_Display[ STR_Pmean ]	 =  "Pmean";//
		System_Display[ STR_PPLAT ]  =  "Pplat";
		System_Display[ STR_PEEP ]   =  "PEEP";
		System_Display[ STR_VTi ]    =  "VTi";//
		System_Display[ STR_Rst ]    =  "Rst";  
		System_Display[ STR_Cdyn ]   =  "Cdyn";
		System_Display[ STR_FiAA ]   =  "FiAA";
		System_Display[ STR_Ftotal ] =  "ftotal";
		System_Display[ STR_Loop ]   =  "LOOP";
		System_Display[ STR_MAC ]    =  "MAC";
		System_Display[ STR_FiN2O ]  =  "FiN②O";
		System_Display[ STR_EtN2O ]  =  "EtN②O";
		System_Display[ STR_FiAA1 ]  =  "FiAA1";
		System_Display[ STR_EtAA1 ]  =  "EtAA1";
		System_Display[ STR_FiAA2 ]  =  "FiAA2";
		System_Display[ STR_EtAA2 ]  =  "EtAA2";
		System_Display[ STR_AA ]     =  "AA";
		System_Display[ STR_Font_Version ]	= "Font_Version:";	
		System_Display[ STR_GUI ]   =  "GUI";
		System_Display[ STR_BDU ]   =  "BDU";
		System_Display[ STR_PMU ]   =  "PMU";
		System_Display[ STR_Pinsp ]         =  "Pinsp";
		System_Display[ STR_Tslope ]        =  "Tslope";
		System_Display[ STR_Psupp ]         =  "Psupp";
		System_Display[ STR_Fsens ]         =  "Fsens";
		System_Display[ STR_Esens ]         =  "Esens";
		System_Display[ STR_Tinsp ]         =  "Tinsp";
		System_Display[ STR_Psens ]         =  "Psens";
		System_Display[ STR_Apnea_time ]	= "  Apnea time";
		//选配
		System_Display[ STR_MODE_SIMV_P_PSV ]  =  "SIMV-P & PSV Mode";
		System_Display[ STR_MODE_PRVC ]        =  "PRVC Mode";
		System_Display[ STR_MODE_INFANT ]      =  "Infant Mode";
		System_Display[ STR_MODE_CO2 ]         =  "CO② Module";
		System_Display[ STR_MODE_SpO2 ]        =  "SpO② Module";
		System_Display[ STR_MODE_Driver ]      =  "Gas Driver Mode";
		System_Display[ STR_MODE_O2 ]          =  "O② Module";
		System_Display[ STR_MODE_Masimo_IRMA ]            =  "Masimo IRMA";
		System_Display[ STR_MODE_Masimo_ISA ]             =  "Masimo ISA";
		System_Display[ STR_MODE_Masimo_ISA_O2 ]          =  "Masimo ISA+O②";
		System_Display[ STR_MODE_Masimo_SpO2 ]            =  "Masimo SpO② Module";
		System_Display[ STR_MODE_AA_CO2 ]                 =  "AA & CO② Module";

		System_Display[ STR_SPO2VER ]		 =  "SpO② Versión";
		System_Display[ STR_AVERTIME ]		 =  "Tiempo Promedio";
		System_Display[ STR_SENSITIVITY ]	 =  "Sensibilidad";
		System_Display[ STR_SMARTTONE ]		 =  "Smart Tone";
		System_Display[ STR_FASTSAT ]		 =  "Fast SAT";
		System_Display[ STR_MCUVER ]		 =  "MCU:";
		System_Display[ STR_DSPVER ]		 =  "DSP:";

		System_Display[ STR_LASTO2CALIB ]	 =  "La última calibración de referencia no O②!";
		System_Display[ STR_MODE_Round ]      =  "○";

		System_Display[ STR_FiHAL ]  =  "FiHAL";
		System_Display[ STR_FiENF ]  =  "FiENF";
		System_Display[ STR_FiISO ]  =  "FiISO";
		System_Display[ STR_FiSEV ]  =  "FiSEV";
		System_Display[ STR_FiDES ]  =  "FiDES";
		System_Display[ STR_EtHAL ]  =  "EtHAL";
		System_Display[ STR_EtENF ]  =  "EtENF";
		System_Display[ STR_EtISO ]  =  "EtISO";
		System_Display[ STR_EtSEV ]  =  "EtSEV";
		System_Display[ STR_EtDES ]  =  "EtDES";

		System_Display[ STR_HAL ]    =  "HAL";//氟烷
		System_Display[ STR_ENF ]    =  "ENF";//安氟醚
		System_Display[ STR_ISO ]    =  "ISO";//异氟醚
		System_Display[ STR_SEV ]    =  "SEV";//七氟醚
		System_Display[ STR_DES ]    =  "DES";//地氟醚
#if 0
		//System Test
		System_Display[STR_SelfTest_Ask]                      =  "Inicio la prueba de pre-uso?";
		System_Display[STR_SelfTest_Exit]                     =  "Salida";//exit
		System_Display[STR_SelfTest_Manual_Circuit]           =  "Prueba fuga Man.";
		System_Display[STR_SelfTest_Auto_Circuit]             =  "Prueba Sist. Aut";
		System_Display[STR_SelfTest_Last_Manual_Circuit]      =  "Última prueba Man.";
		System_Display[STR_SelfTest_Last_Auto_Circuit]        =  "Última prueba auto";
		System_Display[STR_SelfTest_Checking]                 =  "Probando...";
		System_Display[STR_SelfTest_Result_State]             =  "√Pasa    ×Fallar    —Omitir";
		System_Display[STR_SelfTest_Symbol_Pass]              =  "√";
		System_Display[STR_SelfTest_Symbol_Skip]              =  "—";
		System_Display[STR_SelfTest_Symbol_Fail]              =  "×";

		System_Display[STR_Pass]                              =  "Pasa";
		System_Display[STR_Fail]                              =  "Fallar";
		System_Display[STR_Skip]                              =  "Omitir";
		System_Display[STR_Finish]                            =  "Terminar";//
		System_Display[STR_Retry]                             =  "Rever";
		System_Display[STR_SelfTest_Err_Timeout]              =  "Se acabó el tiempo";

		//Check item
		System_Display[STR_SelfTest_Manual_Check]             =  "Manual";//手动
		System_Display[STR_SelfTest_Baro_Check]               =  "Barómetro";
		System_Display[STR_SelfTest_Gas_Check]                =  "Suministro de gas";
		System_Display[STR_SelfTest_FG_Flow_Check]            =  "Sensor de flujo de gas fresco";
		System_Display[STR_SelfTest_CircuitFlow_Check]        =  "Sensor de flujo de circuito";
		System_Display[STR_SelfTest_Pressure_Check]           =  "Sensor De Presión";
		System_Display[STR_SelfTest_CircuitCompliance_Check]  =  "Cumplimiento del circuito";
		System_Display[STR_SelfTest_CircuitLeakage_Check]     =  "Fuga del circuito";
		System_Display[STR_SelfTest_Battery_Check]            =  "Batería";
		System_Display[ STR_SelfTest_OxygenSensor_Check ]     =  "Sensor de oxigeno";
		//Manual Prompt
		System_Display[STR_SelfTest_Manual_Prompt_1]         =  "1.Colocar el interruptor en la posición Manual";
		System_Display[STR_SelfTest_Manual_Prompt_2]         =  "2.Conectar el circuito del paciente y sellar la pieza en Y para la prueba.";
		System_Display[STR_SelfTest_Manual_Prompt_3]         =  "3.Instalar la Bolsa Manual";
		System_Display[STR_SelfTest_Manual_Prompt_4]         =  "4.Conectar la línea de suministro de aire para asegurar que la presión es normal";
		System_Display[STR_SelfTest_Manual_Prompt_5]         =  "5.Verifique si el canister y trampa de agua se han instalado correctamente";
		System_Display[STR_SelfTest_Manual_Prompt_6]         =  "6.Fije todos los controles de flujo al mínimo";
		System_Display[STR_SelfTest_Manual_Prompt_7]         =  "7.Cierre todos los vaporizadores";
		System_Display[STR_SelfTest_Manual_Prompt_8]         =  "8.Cierre el interruptor de AGCO (posición de doble circuito)";
#if (LOGO_FLAG == PENLON_LOGO)		
		System_Display[STR_SelfTest_Manual_Prompt_9]         =  "9.Adjuste valvula APL a 50 cmH②O";
		System_Display[STR_SelfTest_Manual_Prompt_11]        =  "11.Ajuste el interruptor de AGCO en ON (posición de circuito individual)";
		System_Display[STR_SelfTest_Manual_Prompt_12]        =  "12.Presione 'Inicio' para realizar la prueba manual de fugas";
#else
		System_Display[STR_SelfTest_Manual_Prompt_9]         =  "9.Adjuste valvula APL a 70 cmH②O";	
		System_Display[STR_SelfTest_Manual_Prompt_11]        =  "11.Presione 'Inicio' para realizar la prueba manual de fugas";
#endif	
		System_Display[STR_SelfTest_Manual_Prompt_10]        =  "10.Pulse el botón de O2 rápido, hasta que el indicador de presión del circuito alcanza el 30 ~ 40 cm de H2O";
		//Automatic Prompt 1
		System_Display[STR_SelfTest_Auto_Prompt_1]      =  "1.Colocar el interruptor en la posición Vent";
		System_Display[STR_SelfTest_Auto_Prompt_2]      =  "2.Abrir la pieza paciente Y";
		System_Display[STR_SelfTest_Auto_Prompt_3]      =  "3.Check the absorber canister and water trap are installed correctly";
		System_Display[STR_SelfTest_Auto_Prompt_4]      =  "4.Fije todos los controles de flujo al mínimo";
		System_Display[STR_SelfTest_Auto_Prompt_5]      =  "5.Cierre todos los vaporizadores";
		System_Display[STR_SelfTest_Auto_Prompt_6]      =  "6.Cierre el interruptor de AGCO (posición de doble circuito)";
		System_Display[STR_SelfTest_Auto_Prompt_7]      =  "7.Asegúrese de que el fuelle esté desinflado";
		System_Display[STR_SelfTest_Auto_Prompt_8]      =  "8.Presione el botón de inicio para iniciar la detección automática de dispositivos";
		//Automatic Prompt 2
		System_Display[STR_SelfTest_Auto_Prompt_10]      = "1.Ajuste el interruptor manual / mecánico a manual y enchufe al extremo manual";
		if(Drager_EtCO2_Config_On == 2 || MasimoISA_EtCO2_Config_On == 2) //ylk add 20181221
		{
			System_Display[STR_SelfTest_Auto_Prompt_11]      = "2.Adjust the Oxygen flow to 10L/min";
			System_Display[STR_SelfTest_Auto_Prompt_Drager] = "3.Connect gas module sample line to elbow and plug elbow into Y piece";
			System_Display[STR_SelfTest_Auto_Prompt_12]      = "4.Press the start button";
		}
		else
		{
			System_Display[STR_SelfTest_Auto_Prompt_11]      = "2.Adjust the Oxygen flow to 10L/min";
			System_Display[STR_SelfTest_Auto_Prompt_12]          = "3.Press the start button";
		}
		//Automatic Prompt 2
		System_Display[STR_SelfTest_Auto_Prompt_13]      =  "1.Ajuste el interruptor de control manual / máquina al estado de control de la máquina";
		System_Display[STR_SelfTest_Auto_Prompt_14]      =  "2.Bloquear la interfaz Y del paciente";
		System_Display[STR_SelfTest_Auto_Prompt_15]      =  "3.Fije todos los controles de flujo al mínimo";
		System_Display[STR_SelfTest_Auto_Prompt_16]      =  "4.Presione el botón de descarga O2 para llenar el fuelle";
#if (LOGO_FLAG == PENLON_LOGO)		
		System_Display[STR_SelfTest_Auto_Prompt_17]      =  "5.Ajuste el interruptor de AGCO en ON (posición de circuito individual)";
		System_Display[STR_SelfTest_Auto_Prompt_18]      =  "6.Pulse el botón Inicio";
#else
		System_Display[STR_SelfTest_Auto_Prompt_17]      =  "5.Pulse el botón Inicio";
#endif
		//
		System_Display[STR_SelfTest_Err_LeakOut]             =  "Fuga en el rango!";//泄漏超范围!
		System_Display[STR_SelfTest_Err_BagVent]             =  "Fallo posición de control manual / máquina!";//
		System_Display[STR_SelfTest_Err_Circuit_NoConnect]   =  "El circuito no se conecta!";//管路未连接
		System_Display[STR_SelfTest_Err_ValveLeak]           =  "flujo inspiratorio Válvula de control de o de fresco gas válvula de fuga fallo!";//比例阀或新鲜气体阀漏气故障
		System_Display[STR_SelfTest_Err_Sensor]              =  "Please perform O② Cell Calibration and re-test!";//传感器故障
		System_Display[STR_SelfTest_Err_SensorTest]          =  "Error de prueba del sensor!";//传感器测试错误(字符串表未找到)
		System_Display[STR_SelfTest_Err_Sensor_NoConnect]    =  "El sensor pierde eficacia o no se conecta!";//传感器失效或者未连接
		System_Display[STR_SelfTest_Err_Insp_FlowSensor]     =  "Error de prueba del sensor de flujo inspiratorio!";//吸气流量传感器测试错误
		System_Display[STR_SelfTest_Err_Exp_FlowSensor]      =  "Error de prueba del sensor de flujo expiratorio!";//呼气流量传感器测试错误
		System_Display[STR_SelfTest_Err_InspExp_FlowSensor]  =  "Sensor de flujo inspiratorio y error de prueba del sensor de flujo espiratorio!";//吸气和呼气流量传感器测试错误
		System_Display[STR_SelfTest_Err_Compliance]          =  "Error de prueba de cumplimiento!";//顺应性测试错误
		System_Display[STR_SelfTest_Disconnect_AC]           =  "Por favor desconecte AC fuente de alimentación!!";//请断开交流电
		System_Display[STR_SelfTest_Connect_AC]              =  "Por favor conecte la fuente de alimentación de CA!!";//请连接交流电
		System_Display[STR_SelfTest_Err_Gas_Supply]          =  "Fallo en el suministro de aire!!";//气源故障
		System_Display[STR_SelfTest_Err_O2_Supply]           =  "Fallo de suministro de 02!";//氧气气源故障
		System_Display[STR_SelfTest_Err_N2O_Supply]          =  "Fallo en el suministro de N2O!!";//笑气气源故障
		System_Display[STR_SelfTest_Err_Air_Supply]          =  "Fallo en el suministro de aire!!";//空气气源故障
		System_Display[STR_SelfTest_Err_O2_N2O_Supply]       =  "El oxígeno y el suministro de gas de óxido nitroso fracaso!!";//氧气与笑气气源故障
		System_Display[STR_SelfTest_Err_O2_Air_Supply]       =  "Oxígeno y un fallo de suministro de aire!!";//氧气与空气气源故障
		System_Display[STR_SelfTest_Err_N2O_Air_Supply]      =  "Fallo de aire y el suministro de gas óxido nitroso!!";//笑气与空气气源故障
		System_Display[STR_DISP_ERR_NUM]					 =  "num de error";//错误编号
#else
		//System Test
		System_Display[STR_SelfTest_Ask]                      =  "Start pre-use Test?";
		System_Display[STR_SelfTest_Exit]                     =  "Exit";
		System_Display[STR_SelfTest_Manual_Circuit]           =  "Manual Leak Test";
		System_Display[STR_SelfTest_Auto_Circuit]             =  "Automatic System Test";
		System_Display[STR_SelfTest_Last_Manual_Circuit]      =  "Last Manual Leak Test";
		System_Display[STR_SelfTest_Last_Auto_Circuit]        =  "Last Automatic System Test";

		System_Display[STR_SelfTest_Checking]                 =  "Testing...";
		System_Display[STR_SelfTest_Result_State]             =  "√Pass    ×Fail    —Skip";
		System_Display[STR_SelfTest_Symbol_Pass]              =  "√";
		System_Display[STR_SelfTest_Symbol_Skip]              =  "—";
		System_Display[STR_SelfTest_Symbol_Fail]              =  "×";

		System_Display[STR_Pass]                              =  "Pass";
		System_Display[STR_Fail]                              =  "Fail";
		System_Display[STR_Skip]                              =  "Skip";
		System_Display[STR_Finish]                            =  "Finish";
		System_Display[STR_Retry]                             =  "Repeat";
		System_Display[STR_SelfTest_Err_Timeout]              =  "Timeout";

		//Check item
		System_Display[STR_SelfTest_Manual_Check]             =  "Manual";
		System_Display[STR_SelfTest_Baro_Check]               =  "Barometer";
		System_Display[STR_SelfTest_Gas_Check]                =  "Gas Supply";
		System_Display[STR_SelfTest_FG_Flow_Check]            =  "FG Flow Sensor";
		System_Display[STR_SelfTest_CircuitFlow_Check]        =  "Circuit Flow Sensor";
		System_Display[STR_SelfTest_Pressure_Check]           =  "Pressure Sensor";
		System_Display[STR_SelfTest_CircuitCompliance_Check]  =  "Circuit Compliance";
		System_Display[STR_SelfTest_CircuitLeakage_Check]     =  "Circuit Leakage";
		System_Display[STR_SelfTest_Battery_Check]            =  "Battery";
		System_Display[ STR_SelfTest_OxygenSensor_Check ]     =  "Oxygen Sensor";
		//Manual Prompt
		System_Display[STR_SelfTest_Manual_Prompt_1]         =  "1.Set Bag/Vent switch to Bag";
		System_Display[STR_SelfTest_Manual_Prompt_2]         =  "2.Connect the patient circuit Y piece to the absorber test block";
		System_Display[STR_SelfTest_Manual_Prompt_3]         =  "3.Make sure that the Manual Bag is connected";
		System_Display[STR_SelfTest_Manual_Prompt_4]         =  "4.Connect gas pipeline,make sure the supply pressure is between 280-600kPa ";
		System_Display[STR_SelfTest_Manual_Prompt_5]         =  "5.Check the absorber canister and water trap are installed correctly";
		System_Display[STR_SelfTest_Manual_Prompt_6]         =  "6.Set all flow controls to minimum";
		System_Display[STR_SelfTest_Manual_Prompt_7]         =  "7.Turn off the vaporizers";
		System_Display[STR_SelfTest_Manual_Prompt_8]         =  "8.Set ACGO switch to OFF(closed circuit position)";
#if (LOGO_FLAG == PENLON_LOGO)		
		System_Display[STR_SelfTest_Manual_Prompt_9]         =  "9.Set the APL valve to 50 cmH②O";
		System_Display[STR_SelfTest_Manual_Prompt_11]        =  "11.Set ACGO switch to ON (Open circuit position)";
		System_Display[STR_SelfTest_Manual_Prompt_12]        =  "12.Press Start button to perform Manual Leak Test";
#else
		System_Display[STR_SelfTest_Manual_Prompt_9]         =  "9.Set the APL valve to 70 cmH②O";	
		System_Display[STR_SelfTest_Manual_Prompt_11]        =  "11.Press Start button to perform Manual Leak Test";
#endif
		System_Display[STR_SelfTest_Manual_Prompt_10]        =  "10.Push O② flush button until the airway pressure gauge is between 30 and 40 cmH②O";
		//Automatic Prompt 1
		System_Display[STR_SelfTest_Auto_Prompt_1]           =  "1.Set Bag/Vent switch to Vent";
		System_Display[STR_SelfTest_Auto_Prompt_2]           =  "2.Open the patient Y piece";
		System_Display[STR_SelfTest_Auto_Prompt_3]           =  "3.Check the absorber canister and water trap are installed correctly";
		System_Display[STR_SelfTest_Auto_Prompt_4]           =  "4.Set all flow controls to minimum";
		System_Display[STR_SelfTest_Auto_Prompt_5]           =  "5.Turn off the vaporizers";
		System_Display[STR_SelfTest_Auto_Prompt_6]           =  "6.Set ACGO switch to OFF(closed circuit position)";
		System_Display[STR_SelfTest_Auto_Prompt_7]           =  "7.Make sure the bellows are collapsed";
		System_Display[STR_SelfTest_Auto_Prompt_8]           =  "8.Press start button to perform Automatic System Test";
		//Automatic Prompt 2
		System_Display[STR_SelfTest_Auto_Prompt_10]          = "1.Set Bag/Vent switch to Bag and plug the bag port";
		if(Drager_EtCO2_Config_On == 2 || MasimoISA_EtCO2_Config_On == 2) //ylk add 20181221
		{
			System_Display[STR_SelfTest_Auto_Prompt_11]      = "2.Adjust the Oxygen flow to 10L/min";
			System_Display[STR_SelfTest_Auto_Prompt_Drager] = "3.Connect gas module sample line to elbow and plug elbow into Y piece";
			System_Display[STR_SelfTest_Auto_Prompt_12]      = "4.Press the start button";
		}
		else
		{
			System_Display[STR_SelfTest_Auto_Prompt_11]      = "2.Adjust the Oxygen flow to 10L/min";
			System_Display[STR_SelfTest_Auto_Prompt_12]          = "3.Press the start button";
		}
		//Automatic Prompt 2
		System_Display[STR_SelfTest_Auto_Prompt_13]          =  "1.Set Bag/Vent switch to Vent";
		System_Display[STR_SelfTest_Auto_Prompt_14]          =  "2.Occlude the patient Y piece";
		System_Display[STR_SelfTest_Auto_Prompt_15]          =  "3.Set all flow controls to minimum";
		System_Display[STR_SelfTest_Auto_Prompt_16]          =  "4.Push O2 flush button to fill the bellows";
#if (LOGO_FLAG == PENLON_LOGO)		
		System_Display[STR_SelfTest_Auto_Prompt_17]          =  "5.Set ACGO switch to ON (Open circuit position)";
		System_Display[STR_SelfTest_Auto_Prompt_18]          =  "6.Press the start button";
#else
		System_Display[STR_SelfTest_Auto_Prompt_17]          =  "5.Press the start button";
#endif
		//
		System_Display[STR_SelfTest_Err_LeakOut]             =  "Leakage over range!";//泄漏超范围
		System_Display[STR_SelfTest_Err_BagVent]             =  "Bag/Vent switch position error!";//手动/机控位置故障
		System_Display[STR_SelfTest_Err_Circuit_NoConnect]   =  "Circuit not connect!";//管路未连接
		System_Display[STR_SelfTest_Err_ValveLeak]           =  "Inspiratory flow control valve \n or flowmeters leakage failure!";//比例阀或新鲜气体阀漏气故障
		System_Display[STR_SelfTest_Err_Sensor]              =  "Please perform O② Cell Calibration and re-test!";
		System_Display[STR_SelfTest_Err_SensorTest]          =  "Sensor test error!";//传感器测试错误
		System_Display[STR_SelfTest_Err_Sensor_NoConnect]    =  "Sensor lose efficacy or not connect!";//传感器失效或者未连接
		System_Display[STR_SelfTest_Err_Insp_FlowSensor]     =  "Inspiratory flow sensor test failure!";//吸气流量传感器测试失败
		System_Display[STR_SelfTest_Err_Exp_FlowSensor]      =  "Expiratory flow sensor test failure!";//呼气流量传感器测试失败
		System_Display[STR_SelfTest_Err_InspExp_FlowSensor]  =  "Inspiratory flow sensor and\nexpiratory flow sensor test failure!";//吸气和呼气流量传感器测试失败
		System_Display[STR_SelfTest_Err_Compliance]          =  "Compliance test failure!";//顺应性测试失败
		System_Display[STR_Empty]                            =  "";
		System_Display[STR_SelfTest_Disconnect_AC]           =  "Please disconnect AC power supply!";//请断开交流电源!
		System_Display[STR_SelfTest_Connect_AC]              =  "Please connect AC power supply!";//请连接断开交流电源!
		System_Display[STR_SelfTest_Err_Gas_Supply]          =  "Gas supply failure!";//气源故障!
		System_Display[STR_SelfTest_Err_O2_Supply]           =  "O② supply failure!";//氧气气源故障!
		System_Display[STR_SelfTest_Err_N2O_Supply]          =  "N②O supply failure!";//笑气气源故障!
		System_Display[STR_SelfTest_Err_Air_Supply]          =  "Air supply failure!";//空气气源故障!
		System_Display[STR_SelfTest_Err_O2_N2O_Supply]       =  "O② and N②O supply failure!";//氧气与笑气气源故障!
		System_Display[STR_SelfTest_Err_O2_Air_Supply]       =  "O② and Air supply failure!";//氧气与空气气源故障!
		System_Display[STR_SelfTest_Err_N2O_Air_Supply]      =  "N②O and Air supply failure!";//笑气与空气气源故障!
		System_Display[STR_DISP_ERR_NUM]					 =  "Err num:";//错误编号
#endif


		break;
	case 3://俄语
		System_Display[ STR_Mode ]   = "Режимы ";
		System_Display[ STR_Control ]   = "Настройки";
		System_Display[ STR_Alarm ]   = "Тревоги";
		System_Display[ STR_System ]   = "Система"; 
		System_Display[ STR_Monitoring ]   = "Мониторинг";
		System_Display[ STR_Cancel ]   = "Отменить";
		System_Display[ STR_Confirm ]	  = "Подтвердить";
		System_Display[ STR_Volume ]	  = "Объем"; 
		System_Display[ STR_Pressure ]	  = "Давление ";
		System_Display[ STR_Invalidtouch ]	  = " не установлен будет повторно начать калибровку";
		System_Display[ STR_Alarm_Log ]	= "  Журнал тревог  ";
		System_Display[ STR_FREQ ]	= "FREQ";  
		System_Display[ STR_MV ]	= "MV";
		System_Display[ STR_FIO2 ]	= "FiO②";
		System_Display[ STR_VTE ]	= "VTe";
		System_Display[ STR_Apnea_time ]	= " Время апноэ ";
		System_Display[ STR_Date ]	= "Дата "; 
		System_Display[ STR_Time ]	= "Время ";  
		System_Display[ STR_Event ]	= "Событие";  
		System_Display[ STR_Info ]	= "  Информация  "; 
		System_Display[ STR_Set ]	= "  Настройки  "; 
		System_Display[ STR_Date_Time ]	= "  Дата  & Время  ";
		System_Display[ STR_Calib ]	= "  Калибровка  "; 	
// 		System_Display[ STR_GUI ]	= "GUI";
		System_Display[ STR_Baro_pressure ]	= "Барометрический"; 
		System_Display[ STR_Gas_Supply_Pressure ]	= "Газоснабжения";
		System_Display[ STR_Run_Time ]	= "Время работы                ";
		System_Display[ STR_Hours ]	= "Часов";
		System_Display[ STR_touch ]	= "Нажмите на перекрестье для проверки,\n            ";
		System_Display[ STR_Sigh_Breath_Every ]	= "Вздох";//"Вздох каждые";
		System_Display[ STR_SetAAType ]	= "Set AA Type"; 
		System_Display[ STR_AA_O2 ]	= "AA&O②"; 
		System_Display[ STR_Drager_O2 ]	= "Drager + O② Module"; 
		System_Display[ STR_Drager_Accuracy ]	= "Gas Module Unspecified Accuracy!"; //Drager未达到高精度

		System_Display[ STR_Drager ]	= "Drager Module"; 
		System_Display[ STR_Language ]	= "Язык"; 
		System_Display[ STR_Trigger_Type ]	= "Тип триггера";
		System_Display[ STR_CO2_unit ]	= "Единицы EtCO②";
		System_Display[ STR_Loudness ]	= "Звук";
		System_Display[ STR_Pressure_trigger ]	= "триггер давления";
		System_Display[ STR_Flow_trigger ]	= "триггер Потока";
		System_Display[ STR_Year ]	= "Год"; 
		System_Display[ STR_Month ]	= "Месяц"; 
		System_Display[ STR_Day ]	= "День"; 
		System_Display[ STR_Hour ]	= "Час"; 
		System_Display[ STR_Minutes ]  = "Минут";
		System_Display[ STR_Apply ]	= "Применить";
		System_Display[ STR_Pressure_Sensor_Cal ]	= "Калибровка датчика давления"; 
		System_Display[ STR_Flow_Sensor_Cal ]	= "Калибровка датчика потока";
		System_Display[ STR_Oxygen_Cell_Cal ]	= "Калибровка датчика O②";
		System_Display[ STR_Service_Mode ]	= "Сервисные режимы"; 
		System_Display[ STR_Flow_Zero_Cal ]	= "Обнуление флоуметров";///
		System_Display[ STR_PhaseIn_Zero_Cal ]	= "Обнуление газового модуля";///
		System_Display[ STR_Touch_Cal ]	= "Калибровка сенсорного экрана";
		System_Display[ STR_Touch_To_Cal ]	= "Прикоснитесь к перекрестью для калибровки"; 
		System_Display[ STR_Set_O2_Concen ]	= "Выберите проверочную концентрацию O②:"; 

		System_Display[ STR_O2_10LMin ] 	= "В режиме вручную отсоедините дыхательные трубки, порт штепсельной \
											  вилки и отрегулируйте расход кислорода более 10 л / мин.";
		System_Display[ STR_Air_10LMin ]	= "В режиме вручную отсоедините дыхательные трубки, порт штепсельной \
											  вилки и отрегулируйте расход воздуха более 10 л / мин. Или удалить датчик O② из дыхательные системы.";

		System_Display[ STR_Return ]	= "Вернуть";
// 		System_Display[ STR_Flow_Close ]	= "Определение клапана расхода\n закрыт";///
		System_Display[ STR_Iso_gas_moudle ]	= "Убрать модуль газового анализа";///
		System_Display[ STR_GASNOZERO ] = "Газовый модуль не может быть обнулен!";
		System_Display[ STR_Input_password ]	= "Ввести пароль:";	
		System_Display[ STR_Password_Error ]	= "Ошибка пароля!";	
		System_Display[ STR_N2O_Open]	= "Откройте N2O !";	///
		System_Display[ STR_Insp_Valve_Cal ]	= "Калибровка клапана вдоха";	
		System_Display[ STR_Exp_Flow_Sensor_Cal ]	= "Калибровка датчика потока";	
		System_Display[ STR_Exp_Valve_Cal ]	= "Калибровка клапана выдоха";
		System_Display[ STR_Flowmeter_Cal ]	= "Калибровка флоуметра";///
		System_Display[ STR_Calibrating ]	= "Калибровка...";
		System_Display[ STR_Cal_Failed ]	=  "Ошибка калибровки!";	
		System_Display[ STR_Cal_Success ]	= "Калибровка завершена успешно";	
		System_Display[ STR_Waiting ]	= "Пожалуйста, подождите......";
		System_Display[ STR_No_Progam ]	= "Программа не обновлена!";
		System_Display[ STR_No_Response ]	= "\nНет ответа!";
		System_Display[ STR_Continue ]	= "Продолжить";
		System_Display[ STR_Restart ]	= "Выполните перезагрузку\n системы!";	
		System_Display[ STR_Full_Serial ]	= "Введите пароль полностью!";	
		System_Display[ STR_Serial_Correct ]	= "Пароль принят!";	
		System_Display[ STR_Serial_Failure ]	= "Неверный пароль!";	
		System_Display[ STR_Entry_Serial ]	= "Ввести пароль:";
		System_Display[ STR_Backspace ]	= "Возврат";

		System_Display[ STR_Battery_Cal ]	= "Калибровка Баттарея";
		System_Display[ STR_Battery_Voltage ]	= "Напряжение Баттарея:";
		System_Display[ STR_Last_Battery_Cal ]	= "Последняя калибровка:";	
		System_Display[ STR_Cal_Date ]	= "Дата калибровки:";	
		System_Display[ STR_Cal_Duration ]	= "Продолжительность калибровкa:";	
		/********************************************start************************************************/
		System_Display[ STR_All ]	= "Все";	///
		System_Display[ STR_Up ]	= "Вверх";///
		System_Display[ STR_Input ]	= "Пожалуйста, введите";///
		System_Display[ STR_Input_Again ]	= "Пожалуйста, введите снова";///
		System_Display[ STR_Data_Not_Save ]	= "Данные не сохранены. \nПодвтердить или отменить?";///
		System_Display[ STR_Rec_Error ]	= "Получено сообщение об ошибке";	///
		System_Display[ STR_Rec_Timeout ]	= "Задержка приема";	///
		System_Display[ STR_Zero_Beyond ]	= "диапазон превышает ноль?";	///
		System_Display[ STR_EEPROM_Write_Error ]	= "Запись ошибки EEPROM ";	///
		System_Display[ STR_EEPROM_Read_Error ]  = "Чтение ошибки EEPROM ";///
		System_Display[ STR_N2O_AD_Beyond ]  = "Значения AD-диапазона N2O ";///
		System_Display[ STR_Check_N2O ]  = "Проверьте закись азота";///
		System_Display[ STR_Check_AIR ]  = "Проверьте воздух";	///
		/********************************************end************************************************/
		System_Display[ STR_Value1 ]  = "Параметры мониторинга 1";	
// 		System_Display[ STR_Graphics ]  = "Графические параметры";	
		/********************************************start************************************************/
// 		System_Display[ STR_Fresh_gas ] = "Свежий газ";	///
// 		System_Display[ STR_Total_Gas ] = "Общий поток";///
// 		System_Display[ STR_Waveform ]	= "Графический мониторинг";///
// 		System_Display[ STR_Rapid_Monitor ]	= "параметры";///
		/********************************************end************************************************/
		System_Display[ STR_Paw ]	= "Paw";	
		System_Display[ STR_Pleth ]	= "Pleth";	
		System_Display[ STR_No_Vent_To_Patient ]	= "        Вентиляция пациента не осуществляется";	
		System_Display[ STR_Activate_Standby ]	= "Вкл. Standby";	
		System_Display[ STR_Sec ]	= "Sec";///
		System_Display[ STR_Backup ]	= "  Поддержка  ";
		System_Display[ STR_Paw_V_Loop ]	= "Paw-V";
		System_Display[ STR_V_Flow_Loop ]	= "V-Поток"; 
		System_Display[ STR_ON ]	= "ON";	
		System_Display[ STR_OFF ]	= "OFF";///
		System_Display[ STR_English ]	= "English";		
		System_Display[ STR_Pulse ]	= "ЧСС ";
		System_Display[ STR_SPO2 ]	= "SpO②";
		System_Display[ STR_PI ]	= "PI";
		System_Display[ STR_EFM ]	= "EFM";///
		System_Display[ STR_SoftwareVersion ]	= "Версия ПО";
		System_Display[ STR_Option ]	= "вариант";//选配
		System_Display[ STR_Enter ]	= "Ввод";
		System_Display[ STR_EXIT ]	= "Выход";
// 		System_Display[ STR_Layout2 ] = "Схема 2";
		System_Display[ STR_Limits1 ] = "  Границы 1";
		System_Display[ STR_Limits2 ] = "  Границы 2";
		System_Display[ STR_Limits3 ] = "  Границы 3";///
		System_Display[ STR_N2O_Close ] = "закрыть N2O !";///
		System_Display[ STR_Max_Flow ]  = "Max Flow";
		System_Display[ STR_Insp_Time ]  = "Inspiratory Time";
		System_Display[ STR_I_E ]  = "I : E";
		System_Display[ STR_Pressure_Mode ]  = "Давление ";
		System_Display[ STR_Chinese ]  = "简体中文";	
		System_Display[ STR_Turkish ]  = "TÜRKÇE";
		System_Display[ STR_Spanish ]  = "Español";
		System_Display[ STR_Flow ]  = "Поток";
		System_Display[ STR_Total_Flow_Eng]  = "Общий поток ";///
		System_Display[ STR_N2O ]  = "N②0"	;///
		System_Display[ STR_AA1 ]  = "AA1"	;///
		System_Display[ STR_AA2 ]  = "AA2"	;///
		System_Display[ STR_CO2 ]  = "CO②" ;
		System_Display[ STR_EtCO2 ]  = "EtCO②"	;	
		System_Display[ STR_FiCO2 ]  = "FiCO②"	;	
		System_Display[ STR_Set_Mode ]  = "  Режимы  ";	
		System_Display[STR_Upgrade ]  = "Обновить"	;
		System_Display[STR_Cancel_Upgrade ]  = "Отменить";
		System_Display[ STR_Continue_Upgrade ]  = "Продолжить";
		System_Display[ STR_Active ]  = "Active";//ylk add	

		System_Display[ STR_VT_1500 ]  = "Vt <= ";
		System_Display[ STR_Pinsp_70 ]  = "Pinsp <= 70 cmH②O";
		System_Display[ STR_Ie_41 ]  =  "I:E <= 4:1";
		System_Display[ STR_Tinsp_10 ]  = "Tinsp <= 10.0 s";
		System_Display[ STR_Tisnp_Tpause_10 ]  = "Tinsp - Tpause <= 10.0 s";
		System_Display[ STR_Psupp_70 ]  = "Psupp <= 70 cmH②O";
		System_Display[ STR_Simv_Freq_40 ]  = "SIMV FREQ <= 40 bpm";
		System_Display[ STR_Freq_100 ]  =  "FREQ <= 100 bpm";
		System_Display[ STR_Peep_30 ]  = "PEEP <= 30 cmH②O";
		System_Display[ STR_Pause_50 ]  = "Pause <= 60%";    //han
		System_Display[ STR_Esense_80 ]  = "Esens <= 80%";
		System_Display[ STR_Tslop_2 ]  = "Tslope <= 2.0s";
		System_Display[ STR_Fsense_20 ]  = "Fsens <= 20.0 LPM";
		System_Display[ STR_Psense_20 ]  = "Psens <= 20 cmH②O";

// 		System_Display[ STR_Vt_20 ]  =  "Vt Low Limit";
		System_Display[ STR_Vt_50 ]  = "Vt >= ";
		System_Display[ STR_Pinsp_5 ]  =  "Pinsp >= 5 cmH②O";
		System_Display[ STR_Ie_110 ]  = "I:E >= 1:10";
		System_Display[ STR_Tinsp_1 ]  = "Tinsp >= 0.1 s";
		System_Display[ STR_Freq_1 ]  = "SIMV FREQ >= 1 bpm";
		System_Display[ STR_Freq_4 ]  = "FREQ >= 4 bpm";  //sun add 20140416
		System_Display[ STR_Psupport_0 ]  = "Psupp >= 0 cmH②O";
		System_Display[ STR_Peep_Off ]  = "PEEP OFF";
		System_Display[ STR_Pause_0 ]  = "Pause >= 0%";;
		System_Display[ STR_Tslop_0 ]  = "Tslope >= 0.0s";
#if(LOGO_FLAG != PENLON_LOGO)//ylk add 20180709
		System_Display[ STR_Psense_0 ]  = "Psens >= 0 cmH②O";
		System_Display[ STR_Fsense_5 ]  = "Fsens >= 0.5 LPM";
#else
		System_Display[ STR_Psense_0 ]  = "Psens >= 1 cmH②O";
		System_Display[ STR_Fsense_5 ]  = "Fsens >= 1.0 LPM";
#endif			
		System_Display[ STR_Esense_5 ]  = "Esens >= 5%";
		System_Display[ STR_Pause ]  = "Pause";
		System_Display[ STR_Vt ]  = "VT";
		//
		System_Display[ STR_Tinsp_Tpause_1 ]  = "Tinsp - Tpause >= 0.1 s";
		System_Display[ STR_Tinsp_Tslop ]  =  "Tinsp >= Tslope";
		System_Display[ STR_Flow_High ]  = "Flow High Limit";
		System_Display[ STR_Flow_Low ]  = "Flow Low Limit";
		//
		System_Display[ STR_Psupport_Peep_70 ]  = "Psupp + PEEP <= 70 cmH②O";
		System_Display[ STR_Pinsp_Peep_70 ]  =  "Pinsp + PEEP <= 70 cmH②O";
		System_Display[ STR_Pinsp_Peep_Ph ]  = "Pinsp + PEEP < Ph";
		System_Display[ STR_Psupp_Peep_Ph ]  =  "Psupp + PEEP < Ph";
		System_Display[ STR_Peep_Ph ]  = "PEEP < Ph";
		System_Display[ STR_Texp_Low ]  = "Texp Low Limit";
		//
		System_Display[ STR_Backup_VCV ]  = "Активирована поддержка VCV!";
		System_Display[ STR_Backup_PCV ]  =  "Активирована поддержка PCV!";
		//
		System_Display[ STR_Pinsp_30 ]  = "Pinsp > 30 cmH②O ? Confirm";
		System_Display[ STR_Pinsp_50]  = "Pinsp > 50 cmH②O ? Confirm";
		System_Display[ STR_Peep_Psupport_30 ]  = "PEEP + Psupp > 30 cmH②O ? Confirm";
		System_Display[ STR_Peep_Psupport_50 ]  = "PEEP + Psupp > 50 cmH②O ? Confirm";
		System_Display[ STR_Peep_20 ]  = "PEEP > 20 cmH②O ? Confirm";
		System_Display[ STR_Ie_1 ]  = "I:E > 1:1 ? Confirm";			
		/*********************************************start****************************************************/
		System_Display[ STR_Sec_1 ]  = "Sec";	
		System_Display[ STR_USA_Standard ]  = "Стандарт США!";	
		System_Display[ STR_EN_Standard ]  = "Европейский стандарт!";	
		System_Display[ STR_FiN2O ]  = "FiN②O";	
// 		System_Display[ STR_ET ]  = "Et";	
// 		System_Display[ STR_FI ]  = "Fi";
// 		System_Display[ STR_FREQ_English ] ="f";
		System_Display[ STR_Value2 ]  = "Параметры мониторинга 2";	
		System_Display[ STR_Set_EFM_Cal_Baro_Pressure ]  = "Please set the EFM\n checking air\n pressure value";	
		System_Display[ STR_O2 ]  = "O②";
		System_Display[ STR_N20 ]  ="N②O"; 
		System_Display[ STR_AIR ]  = "Air";//
// 		System_Display[ STR_O2_AD_Beyond ]   =  "Значения AD\n-диапазона N②O!";
// 		System_Display[ STR_AIR_AD_Beyond ]	 =  "Значения AD\n-диапазона Air !";	
		System_Display[ STR_User_BP_Set ]	   = "Комплект по\nльзователя BP:" ;
		System_Display[ STR_Cal_BP_set  ]	       = "Калибровка комплекта BP:" ;		
// 		System_Display[ STR_Flowmeter_Air_Test  ]	       = "Flow\nmeter \nAir test" ;	
		/********************************************end*****************************************************/
		//add 20161115
		System_Display[ STR_Russian  ]	       = "Русский" ;	 
		System_Display[ STR_System_Test]     = "Тест системы";
		System_Display[ STR_Preuse_Test]     = "Начать стартовый тест?";

		System_Display[ STR_Start]     = "Старт";
		System_Display[ STR_Alarm_Silence ]	 = "Откл.тревогу";
		System_Display[ STR_Alarm_Reset ]	 = "Сброс тревоги";
		System_Display[ STR_Trend]	 = "Тренды";
		System_Display[ STR_Trend_graph  ]	 = "Графики трендов"; 
		System_Display[ STR_STANDBY	]				 = "Режим Standby" ;  

		System_Display[STR_Dirver_Gas_Air] = "Движущий газ - воздух, закрыть выключатель невозможно!";
// 		System_Display[STR_Dirver_Gas_Change_Air] = "Движущий газ - воздух!";
// 		System_Display[STR_Dirver_Gas_Change_O2] = "Движущий газ - кислород!";
		System_Display[STR_Close_Air_Switch] = "Переключатель подачи воздуха закрыт?";
		System_Display[STR_Change_Dirver_Gas] = "Независимо от изменения типа движущего газа?";
		System_Display[STR_DRIVER] = "Привод.";//"Водитель";//驱动气体
		System_Display[STR_CLEARTREND] = "Изменение настроек времени удалит данные трендов?";

		System_Display[ STR_SPO2VER ]		 =  "SpO② Versión";
		System_Display[ STR_AVERTIME ]		 =  "Среднее время ";
		System_Display[ STR_SENSITIVITY ]	 =  "Чувствительность";
		System_Display[ STR_SMARTTONE ]		 =  "Smart Tone";
		System_Display[ STR_FASTSAT ]		 =  "Fast SAT";
		System_Display[ STR_MCUVER ]		 =  "MCU:";
		System_Display[ STR_DSPVER ]		 =  "DSP:";

		System_Display[ STR_LASTO2CALIB ]	 =  "в прошлом O② не охватывают калибровка!";

		System_Display[ STR_FREQ ]	= "FREQ";
		System_Display[ STR_Mvspn ]  =  "Mvspn";//
		System_Display[ STR_Fspn ]   =  "fspn" ;//
		System_Display[ STR_PPEAK ]	 =  "Ppeak";//
		System_Display[ STR_Pmean ]	 =  "Pmean";//
		System_Display[ STR_PPLAT ]  =  "Pplat";
		System_Display[ STR_PEEP ]   =  "PEEP";
		System_Display[ STR_VTi ]    =  "VTi";//
		System_Display[ STR_Rst ]    =  "Rst";  
		System_Display[ STR_Cdyn ]   =  "Cdyn";
		System_Display[ STR_FiAA ]   =  "FiAA";
		System_Display[ STR_Ftotal ] =  "ftotal";
		System_Display[ STR_Loop ]   =  "LOOP";
		System_Display[ STR_MAC ]    =  "MAC";
		System_Display[ STR_FiN2O ]  =  "FiN②O";
		System_Display[ STR_EtN2O ]  =  "EtN②O";
		System_Display[ STR_FiAA1 ]  =  "FiAA1";
		System_Display[ STR_EtAA1 ]  =  "EtAA1";
		System_Display[ STR_FiAA2 ]  =  "FiAA2";
		System_Display[ STR_EtAA2 ]  =  "EtAA2";
		System_Display[ STR_AA ]     =  "AA";
		System_Display[ STR_Font_Version ]	= "Font_Version:";	
		System_Display[ STR_GUI ]   =  "GUI";
		System_Display[ STR_BDU ]   =  "BDU";
		System_Display[ STR_PMU ]   =  "PMU";
		System_Display[ STR_Pinsp ]         =  "Pinsp";
		System_Display[ STR_Tslope ]        =  "Tslope";
		System_Display[ STR_Psupp ]         =  "Psupp";
		System_Display[ STR_Fsens ]         =  "Fsens";
		System_Display[ STR_Esens ]         =  "Esens";
		System_Display[ STR_Tinsp ]         =  "Tinsp";
		System_Display[ STR_Psens ]         =  "Psens";
		System_Display[ STR_Apnea_time ]	= "  Apnea time";
		//选配
		System_Display[ STR_MODE_SIMV_P_PSV ]  =  "SIMV-P & PSV Mode";
		System_Display[ STR_MODE_PRVC ]        =  "PRVC Mode";
		System_Display[ STR_MODE_INFANT ]      =  "Infant Mode";
		System_Display[ STR_MODE_CO2 ]         =  "CO② Module";
		System_Display[ STR_MODE_SpO2 ]        =  "SpO② Module";
		System_Display[ STR_MODE_Driver ]      =  "Gas Driver Mode";
		System_Display[ STR_MODE_O2 ]          =  "O② Module";
		System_Display[ STR_MODE_Masimo_IRMA ]            =  "Masimo IRMA";
		System_Display[ STR_MODE_Masimo_ISA ]             =  "Masimo ISA";
		System_Display[ STR_MODE_Masimo_ISA_O2 ]          =  "Masimo ISA+O②";
		System_Display[ STR_MODE_Masimo_SpO2 ]            =  "Masimo SpO② Module";
		System_Display[ STR_MODE_AA_CO2 ]                 =  "AA & CO② Module";

		System_Display[ STR_MODE_Round ]      =  "○";

		System_Display[ STR_FiHAL ]  =  "FiHAL";
		System_Display[ STR_FiENF ]  =  "FiENF";
		System_Display[ STR_FiISO ]  =  "FiISO";
		System_Display[ STR_FiSEV ]  =  "FiSEV";
		System_Display[ STR_FiDES ]  =  "FiDES";
		System_Display[ STR_EtHAL ]  =  "EtHAL";
		System_Display[ STR_EtENF ]  =  "EtENF";
		System_Display[ STR_EtISO ]  =  "EtISO";
		System_Display[ STR_EtSEV ]  =  "EtSEV";
		System_Display[ STR_EtDES ]  =  "EtDES";

		System_Display[ STR_HAL ]    =  "HAL";//氟烷
		System_Display[ STR_ENF ]    =  "ENF";//安氟醚
		System_Display[ STR_ISO ]    =  "ISO";//异氟醚
		System_Display[ STR_SEV ]    =  "SEV";//七氟醚
		System_Display[ STR_DES ]    =  "DES";//地氟醚
#if 0
		//System Test
		System_Display[STR_SelfTest_Ask]                      =  "Начать стартовый тест? ";
		System_Display[STR_SelfTest_Exit]                     =  "Выход";
		System_Display[STR_SelfTest_Manual_Circuit]           =  "Ручной тест на утечку";
		System_Display[STR_SelfTest_Auto_Circuit]             =  "Авто-Системы Тест";//Автоматический тест системы
		System_Display[STR_SelfTest_Last_Manual_Circuit]      =  "Последний ручной тест на утечку";
		System_Display[STR_SelfTest_Last_Auto_Circuit]        =  "Последний авто-Системы Тест";//Последний автоматический тест системы

		System_Display[STR_SelfTest_Checking]                 =  "Тестирование…";
		System_Display[STR_SelfTest_Result_State]             =  "√Пройдено    ×Неудачно    —Пропущено";
		System_Display[STR_SelfTest_Symbol_Pass]              =  "√";
		System_Display[STR_SelfTest_Symbol_Skip]              =  "—";
		System_Display[STR_SelfTest_Symbol_Fail]              =  "×";

		System_Display[STR_Pass]                              =  "Пройдено";
		System_Display[STR_Fail]                              =  "Неудачно";
		System_Display[STR_Skip]                              =  "Пропущено";
		System_Display[STR_Finish]                            =  "Завершено";
		System_Display[STR_Retry]                             =  "повторить попытку";//retry
		System_Display[STR_SelfTest_Err_Timeout]              =  "Перерыв";

		//Check item
		System_Display[STR_SelfTest_Manual_Check]             =  "вручную";//Manual手动
		System_Display[STR_SelfTest_Baro_Check]               =  "Баттарея";
		System_Display[STR_SelfTest_Gas_Check]                =  "Газоснабжения";
		System_Display[STR_SelfTest_FG_Flow_Check]            =  "Датчик потока свежего газа";
		System_Display[STR_SelfTest_CircuitFlow_Check]        =  "Датчика потока контурный";
		System_Display[STR_SelfTest_Pressure_Check]           =  "Датчика Давления";
		System_Display[STR_SelfTest_CircuitCompliance_Check]  =  "комплайнс дых. Контура";
		System_Display[STR_SelfTest_CircuitLeakage_Check]     =  "утечки дых. контура";
		System_Display[STR_SelfTest_Battery_Check]            =  "Баттарея";
		System_Display[ STR_SelfTest_OxygenSensor_Check ]     =  "Датчик O②";

		//Manual Prompt
		System_Display[STR_SelfTest_Manual_Prompt_1]         =  "1.Установите переключатель Мех Мешок в положение Мешок!";//"Мех.\Мешок" "Мешок"
		System_Display[STR_SelfTest_Manual_Prompt_2]         =  "2.Присоедините контур пациента и закройте Y-образный соединитель заглушкой";
		System_Display[STR_SelfTest_Manual_Prompt_3]         =  "3.Установите дыхательный мешок";
		System_Display[STR_SelfTest_Manual_Prompt_4]         =  "4.Присоедините шланг подачи газа, убедитесь, что давление газа находится в разрешенных пределах";
		System_Display[STR_SelfTest_Manual_Prompt_5]         =  "5.Убедитесь, что канистра абсорбера и водяная ловушка установлены правильно";
		System_Display[STR_SelfTest_Manual_Prompt_6]         =  "6.Установите все элементы управления потоком на минимум";//
		System_Display[STR_SelfTest_Manual_Prompt_7]         =  "7.Выключитеs	 все испарители";
		System_Display[STR_SelfTest_Manual_Prompt_8]         =  "8.Установите выключатель ACGO в положение OFF(двухконтурная система)";
		System_Display[STR_SelfTest_Manual_Prompt_9]         =  "9.Установите клапан APL на 70 см.вод.ст";
		System_Display[STR_SelfTest_Manual_Prompt_10]        =  "10.Нажмите и удерживайте кнопку O2 Flush до достижения уровня давления (PAW) 30-50 см.вод.ст.";
		System_Display[STR_SelfTest_Manual_Prompt_11]        =  "11.Нажмите кнопку Start для проведения теста на утечку";

		//Automatic Prompt 1
		System_Display[STR_SelfTest_Auto_Prompt_1]      =  "1.Установите переключатель Мех./Мешок в положение Мех";//Мех\Мешок
		System_Display[STR_SelfTest_Auto_Prompt_2]      =  "2.Откройте деталь пациента Y";
		System_Display[STR_SelfTest_Auto_Prompt_3]      =  "3.Убедитесь, что пробоотборная трубка и дегидратор установлены правильно";//检查采样管和脱水器已正确安装
		System_Display[STR_SelfTest_Auto_Prompt_4]      =  "4.Устанавливать все ограничения потока до минимума";//设置所有流量控制达到最小值
		System_Display[STR_SelfTest_Auto_Prompt_5]      =  "5.Выключите все испарители";
		System_Display[STR_SelfTest_Auto_Prompt_6]      =  "6.Установите выключатель ACGO в положение OFF (двухконтурная система)";
		System_Display[STR_SelfTest_Auto_Prompt_7]      =  "7.Убедитесь, что сильфон свернут";
		System_Display[STR_SelfTest_Auto_Prompt_8]      =  "8.Нажмите кнопку Пуск, чтобы начать автоматическое обнаружение устройства.";//按下开始按键启动自动设备检测
		////Automatic Prompt 2
		System_Display[STR_SelfTest_Auto_Prompt_10]      = "1.Установите ручной / механический переключатель в ручную и подключите к ручному концу";//设置手动/机控开关到手动并且插头插到手动端
		if(Drager_EtCO2_Config_On == 2 || MasimoISA_EtCO2_Config_On == 2) //ylk add 20181221
		{
			System_Display[STR_SelfTest_Auto_Prompt_11]      = "2.Adjust the Oxygen flow to 10L/min";
			System_Display[STR_SelfTest_Auto_Prompt_Drager] = "3.Connect gas module sample line to elbow and plug elbow into Y piece";
			System_Display[STR_SelfTest_Auto_Prompt_12]      = "4.Press the start button";
		}
		else
		{
			System_Display[STR_SelfTest_Auto_Prompt_11]      = "2.Adjust the Oxygen flow to 10L/min";
			System_Display[STR_SelfTest_Auto_Prompt_12]          = "3.Press the start button";
		}
		//Automatic Prompt 2
		System_Display[STR_SelfTest_Auto_Prompt_13]      =  "1.Установите переключатель Мех Мешок  в положение Мех";
		System_Display[STR_SelfTest_Auto_Prompt_14]      =  "2.близко деталь пациента Y";//близко 关闭
		System_Display[STR_SelfTest_Auto_Prompt_15]      =  "3.Устанавливать все ограничения потока до минимума";
		System_Display[STR_SelfTest_Auto_Prompt_16]      =  "4.Нажмите кнопку смыва O2, чтобы заполнить сильфоны";
		System_Display[STR_SelfTest_Auto_Prompt_17]      =  "5.Нажмите кнопку Пуск";
#if (LOGO_FLAG == PENLON_LOGO)		
		System_Display[STR_SelfTest_Auto_Prompt_17]      =  "5.Установите выключатель ACGO в положение "ON" (для однотрубного контура)";
		System_Display[STR_SelfTest_Auto_Prompt_18]      =  "6.Нажмите кнопку Пуск";
#else
		System_Display[STR_SelfTest_Auto_Prompt_17]      =  "5.Нажмите кнопку Пуск";
#endif
		//
		System_Display[STR_SelfTest_Err_LeakOut]             =  "Большая утечка!";//泄漏超范围!
		System_Display[STR_SelfTest_Err_BagVent]             =  "Переключатель 《Мех\Мешок》 в неправильном положении!";//手动/机控位置故障
		System_Display[STR_SelfTest_Err_Circuit_NoConnect]   =  "Контур не подключен!";//管路未连接
		System_Display[STR_SelfTest_Err_ValveLeak]           =  "Утечка в клапане потока или клапане свежего газа!";//比例阀或新鲜气体阀漏气故障
		System_Display[STR_SelfTest_Err_Sensor]              =  "Please perform O② Cell Calibration and re-test!";//传感器故障
		System_Display[STR_SelfTest_Err_SensorTest]          =  "датчик тест ошибка!";//传感器测试错误(字符串表未找到)
		System_Display[STR_SelfTest_Err_Sensor_NoConnect]    =  "Датчик теряет эффективность или не подключен!";//传感器失效或者未连接
		System_Display[STR_SelfTest_Err_Insp_FlowSensor]     =  "Ошибка датчика потока (вдох)!";//吸气流量传感器测试错误
		System_Display[STR_SelfTest_Err_Exp_FlowSensor]      =  "Ошибка датчика потока (выдох)!";//呼气流量传感器测试错误
		System_Display[STR_SelfTest_Err_InspExp_FlowSensor]  =  "Ошибка датчииков потока (Вдох\Выдох)!";//吸气和呼气流量传感器测试错误
		System_Display[STR_SelfTest_Err_Compliance]          =  "Комплайнс - тест не пройден.!";//顺应性测试错误
		System_Display[STR_SelfTest_Disconnect_AC]           =  "Пожалуйста, отключите источник питания!";//请断开交流电
		System_Display[STR_SelfTest_Connect_AC]              =  "Пожалуйста, подключите источник питания!";//请连接交流电
		System_Display[STR_SelfTest_Err_Gas_Supply]          =  "Отказ подачи воздуха!";//气源故障
		System_Display[STR_SelfTest_Err_O2_Supply]           =  "Нарушение подачи О2!";//氧气气源故障
		System_Display[STR_SelfTest_Err_N2O_Supply]          =  "Нарушение подачи N2O!";//笑气气源故障
		System_Display[STR_SelfTest_Err_Air_Supply]          =  "Нарушение подачи воздуха!";//空气气源故障
		System_Display[STR_SelfTest_Err_O2_N2O_Supply]       =  "Нарушение подачи О2 и N2O!";//氧气与笑气气源故障
		System_Display[STR_SelfTest_Err_O2_Air_Supply]       =  "Нарушение подачи О2 и воздуха!";//氧气与空气气源故障
		System_Display[STR_SelfTest_Err_N2O_Air_Supply]      =  "Нарушение подачи N2O и воздуха!";//笑气与空气气源故障
		System_Display[STR_DISP_ERR_NUM]					 =  "Номер ошибки:";//错误编号
#else
		//System Test
		System_Display[STR_SelfTest_Ask]                      =  "Start pre-use Test?";
		System_Display[STR_SelfTest_Exit]                     =  "Exit";
		System_Display[STR_SelfTest_Manual_Circuit]           =  "Manual Leak Test";
		System_Display[STR_SelfTest_Auto_Circuit]             =  "Automatic System Test";
		System_Display[STR_SelfTest_Last_Manual_Circuit]      =  "Last Manual Leak Test";
		System_Display[STR_SelfTest_Last_Auto_Circuit]        =  "Last Automatic System Test";

		System_Display[STR_SelfTest_Checking]                 =  "Testing...";
		System_Display[STR_SelfTest_Result_State]             =  "√Pass    ×Fail    —Skip";
		System_Display[STR_SelfTest_Symbol_Pass]              =  "√";
		System_Display[STR_SelfTest_Symbol_Skip]              =  "—";
		System_Display[STR_SelfTest_Symbol_Fail]              =  "×";

		System_Display[STR_Pass]                              =  "Pass";
		System_Display[STR_Fail]                              =  "Fail";
		System_Display[STR_Skip]                              =  "Skip";
		System_Display[STR_Finish]                            =  "Finish";
		System_Display[STR_Retry]                             =  "Repeat";
		System_Display[STR_SelfTest_Err_Timeout]              =  "Timeout";

		//Check item
		System_Display[STR_SelfTest_Manual_Check]             =  "Manual";
		System_Display[STR_SelfTest_Baro_Check]               =  "Barometer";
		System_Display[STR_SelfTest_Gas_Check]                =  "Gas Supply";
		System_Display[STR_SelfTest_FG_Flow_Check]            =  "FG Flow Sensor";
		System_Display[STR_SelfTest_CircuitFlow_Check]        =  "Circuit Flow Sensor";
		System_Display[STR_SelfTest_Pressure_Check]           =  "Pressure Sensor";
		System_Display[STR_SelfTest_CircuitCompliance_Check]  =  "Circuit Compliance";
		System_Display[STR_SelfTest_CircuitLeakage_Check]     =  "Circuit Leakage";
		System_Display[STR_SelfTest_Battery_Check]            =  "Battery";
		System_Display[ STR_SelfTest_OxygenSensor_Check ]     =  "Oxygen Sensor";
		//Manual Prompt
		System_Display[STR_SelfTest_Manual_Prompt_1]         =  "1.Set Bag/Vent switch to Bag";
		System_Display[STR_SelfTest_Manual_Prompt_2]         =  "2.Connect the patient circuit Y piece to the absorber test block";
		System_Display[STR_SelfTest_Manual_Prompt_3]         =  "3.Make sure that the Manual Bag is connected";
		System_Display[STR_SelfTest_Manual_Prompt_4]         =  "4.Connect gas pipeline,make sure the supply pressure is between 280-600kPa ";
		System_Display[STR_SelfTest_Manual_Prompt_5]         =  "5.Check the absorber canister and water trap are installed correctly";
		System_Display[STR_SelfTest_Manual_Prompt_6]         =  "6.Set all flow controls to minimum";
		System_Display[STR_SelfTest_Manual_Prompt_7]         =  "7.Turn off the vaporizers";
		System_Display[STR_SelfTest_Manual_Prompt_8]         =  "8.Set ACGO switch to OFF(closed circuit position)";
#if (LOGO_FLAG == PENLON_LOGO)		
		System_Display[STR_SelfTest_Manual_Prompt_9]         =  "9.Set the APL valve to 50 cmH②O";
		System_Display[STR_SelfTest_Manual_Prompt_11]        =  "11.Set ACGO switch to ON (Open circuit position)";
		System_Display[STR_SelfTest_Manual_Prompt_12]        =  "12.Press Start button to perform Manual Leak Test";
#else
		System_Display[STR_SelfTest_Manual_Prompt_9]         =  "9.Set the APL valve to 70 cmH②O";	
		System_Display[STR_SelfTest_Manual_Prompt_11]        =  "11.Press Start button to perform Manual Leak Test";
#endif
		System_Display[STR_SelfTest_Manual_Prompt_10]        =  "10.Push O② flush button until the airway pressure gauge is between 30 and 40 cmH②O";
		//Automatic Prompt 1
		System_Display[STR_SelfTest_Auto_Prompt_1]           =  "1.Set Bag/Vent switch to Vent";
		System_Display[STR_SelfTest_Auto_Prompt_2]           =  "2.Open the patient Y piece";
		System_Display[STR_SelfTest_Auto_Prompt_3]           =  "3.Check the absorber canister and water trap are installed correctly";
		System_Display[STR_SelfTest_Auto_Prompt_4]           =  "4.Set all flow controls to minimum";
		System_Display[STR_SelfTest_Auto_Prompt_5]           =  "5.Turn off the vaporizers";
		System_Display[STR_SelfTest_Auto_Prompt_6]           =  "6.Set ACGO switch to OFF(closed circuit position)";
		System_Display[STR_SelfTest_Auto_Prompt_7]           =  "7.Make sure the bellows are collapsed";
		System_Display[STR_SelfTest_Auto_Prompt_8]           =  "8.Press start button to perform Automatic System Test";
		//Automatic Prompt 2
		System_Display[STR_SelfTest_Auto_Prompt_10]          = "1.Set Bag/Vent switch to Bag and plug the bag port";
		if(Drager_EtCO2_Config_On == 2 || MasimoISA_EtCO2_Config_On == 2) //ylk add 20181221
		{
			System_Display[STR_SelfTest_Auto_Prompt_11]      = "2.Adjust the Oxygen flow to 10L/min";
			System_Display[STR_SelfTest_Auto_Prompt_Drager] = "3.Connect gas module sample line to elbow and plug elbow into Y piece";
			System_Display[STR_SelfTest_Auto_Prompt_12]      = "4.Press the start button";
		}
		else
		{
			System_Display[STR_SelfTest_Auto_Prompt_11]      = "2.Adjust the Oxygen flow to 10L/min";
			System_Display[STR_SelfTest_Auto_Prompt_12]          = "3.Press the start button";
		}
		//Automatic Prompt 2
		System_Display[STR_SelfTest_Auto_Prompt_13]          =  "1.Set Bag/Vent switch to Vent";
		System_Display[STR_SelfTest_Auto_Prompt_14]          =  "2.Occlude the patient Y piece";
		System_Display[STR_SelfTest_Auto_Prompt_15]          =  "3.Set all flow controls to minimum";
		System_Display[STR_SelfTest_Auto_Prompt_16]          =  "4.Push O2 flush button to fill the bellows";
#if (LOGO_FLAG == PENLON_LOGO)		
		System_Display[STR_SelfTest_Auto_Prompt_17]          =  "5.Set ACGO switch to ON (Open circuit position)";
		System_Display[STR_SelfTest_Auto_Prompt_18]          =  "6.Press the start button";
#else
		System_Display[STR_SelfTest_Auto_Prompt_17]          =  "5.Press the start button";
#endif		//
		System_Display[STR_SelfTest_Err_LeakOut]             =  "Leakage over range!";//泄漏超范围
		System_Display[STR_SelfTest_Err_BagVent]             =  "Bag/Vent switch position error!";//手动/机控位置故障
		System_Display[STR_SelfTest_Err_Circuit_NoConnect]   =  "Circuit not connect!";//管路未连接
		System_Display[STR_SelfTest_Err_ValveLeak]           =  "Inspiratory flow control valve \n or flowmeters leakage failure!";//比例阀或新鲜气体阀漏气故障
		System_Display[STR_SelfTest_Err_Sensor]              =  "Please perform O② Cell Calibration and re-test!";
		System_Display[STR_SelfTest_Err_SensorTest]          =  "Sensor test error!";//传感器测试错误
		System_Display[STR_SelfTest_Err_Sensor_NoConnect]    =  "Sensor lose efficacy or not connect!";//传感器失效或者未连接
		System_Display[STR_SelfTest_Err_Insp_FlowSensor]     =  "Inspiratory flow sensor test failure!";//吸气流量传感器测试失败
		System_Display[STR_SelfTest_Err_Exp_FlowSensor]      =  "Expiratory flow sensor test failure!";//呼气流量传感器测试失败
		System_Display[STR_SelfTest_Err_InspExp_FlowSensor]  =  "Inspiratory flow sensor and\nexpiratory flow sensor test failure!";//吸气和呼气流量传感器测试失败
		System_Display[STR_SelfTest_Err_Compliance]          =  "Compliance test failure!";//顺应性测试失败
		System_Display[STR_Empty]                            =  "";
		System_Display[STR_SelfTest_Disconnect_AC]           =  "Please disconnect AC power supply!";//请断开交流电源!
		System_Display[STR_SelfTest_Connect_AC]              =  "Please connect AC power supply!";//请连接断开交流电源!
		System_Display[STR_SelfTest_Err_Gas_Supply]          =  "Gas supply failure!";//气源故障!
		System_Display[STR_SelfTest_Err_O2_Supply]           =  "O② supply failure!";//氧气气源故障!
		System_Display[STR_SelfTest_Err_N2O_Supply]          =  "N②O supply failure!";//笑气气源故障!
		System_Display[STR_SelfTest_Err_Air_Supply]          =  "Air supply failure!";//空气气源故障!
		System_Display[STR_SelfTest_Err_O2_N2O_Supply]       =  "O② and N②O supply failure!";//氧气与笑气气源故障!
		System_Display[STR_SelfTest_Err_O2_Air_Supply]       =  "O② and Air supply failure!";//氧气与空气气源故障!
		System_Display[STR_SelfTest_Err_N2O_Air_Supply]      =  "N②O and Air supply failure!";//笑气与空气气源故障!
		System_Display[STR_DISP_ERR_NUM]					 =  "Err num:";//错误编号
#endif
		break;
	case 4://土耳其语
		System_Display[ STR_Mode ]   = "  MODLAR  ";
		System_Display[ STR_Control ]   = "Controls ";
		System_Display[ STR_Alarm ]   = "ALARMLAR";
		System_Display[ STR_System ]   = "SǐSTEM";
		System_Display[ STR_Monitoring ]   = "GǒRúNTú";
		System_Display[ STR_Cancel ]   = "Cancelar";
		System_Display[ STR_Confirm ]	  = "ONAYLA";
		System_Display[ STR_Volume ]	  = "HACǐM"; 
		System_Display[ STR_Pressure ]	  = "BASíNε";
		System_Display[ STR_Invalidtouch ]	  = "";
		System_Display[ STR_Alarm_Log ]	= "  ALARM KAYíT  ";
		System_Display[ STR_FREQ ]	= "FREKANS";
		System_Display[ STR_Active ]  = "Active";//ylk add	
		System_Display[ STR_MV ]	= "MV";
		System_Display[ STR_FIO2 ]	= "FiO②";
		System_Display[ STR_VTE ]	= "VTe";
		System_Display[ STR_Apnea_time ]	= "APNEA ZAMANí";
		System_Display[ STR_Date ]	= "TARǐH"; 
		System_Display[ STR_Time ]	= "ZAMAN";  
		System_Display[ STR_Event ]	= "OAY";  
		System_Display[ STR_Info ]	= "    BǐLGǐ    "; 
		System_Display[ STR_Set ]	= "  AYARLAR  "; 
		System_Display[ STR_Date_Time ]	= "   BAKIM   ";
		System_Display[ STR_Calib ]	= "  KALǐBRASYON  "; 	
// 		System_Display[ STR_GUI ]	= "GUí";
		System_Display[ STR_Baro_pressure ]	= "Barometric"; 
		System_Display[ STR_Gas_Supply_Pressure ]	= "Supply Pressure";
		System_Display[ STR_Run_Time ]	= "CALíSMA ZAMANí               ";
		System_Display[ STR_Hours ] = "Hours";
		System_Display[ STR_touch ]	= ""            ;
		System_Display[ STR_Sigh_Breath_Every ]	= "ǐε εEKME SOLUêU";

		System_Display[ STR_SetAAType ]	= "Set AA Type"; 
		System_Display[ STR_AA_O2 ]	= "AA&O②"; 
		System_Display[ STR_Drager_O2 ]	= "Drager + O② Module"; 
		System_Display[ STR_Drager_Accuracy ]	= "Gas Module Unspecified Accuracy!"; //Drager未达到高精度
		System_Display[ STR_Drager ]	= "Drager Module";
		System_Display[ STR_Language ]	= "LǐSAN"; 
		System_Display[ STR_Trigger_Type ]	= "TETǐKLEME TǐPǐ";
		System_Display[ STR_CO2_unit ]	= "EtCO② úNǐTESǐ";
		System_Display[ STR_Loudness ]	= "GúRúLTú";
		System_Display[ STR_Pressure_trigger ]	= "BASíNε TETǐKLEMESǐ";
		System_Display[ STR_Flow_trigger ]	= "AKíζ TETǐKLEMESǐ";
		System_Display[ STR_Year ]	= "YIL"; 
		System_Display[ STR_Month ]	= "AYARLAR"; 
		System_Display[ STR_Day ]	= "GúN"; 
		System_Display[ STR_Hour ]	= "SAAT";
		System_Display[ STR_Minutes ]  = "DAKǐKA";
		System_Display[ STR_Apply ]	= "UYGULA";
		System_Display[ STR_Pressure_Sensor_Cal ]	= "BASíNε.SENS.KAL."; 
		System_Display[ STR_Flow_Sensor_Cal ]	= "AKíζ SENS.KAL.";
		System_Display[ STR_Oxygen_Cell_Cal ]	= "O②.SENS.KAL.";
		System_Display[ STR_Service_Mode ]	= "SERVǐS MODU."; 
		System_Display[ STR_Flow_Zero_Cal ]	= "Flowmeter Zero Calibration";
		System_Display[ STR_PhaseIn_Zero_Cal ]	= "Gas Module Zero Calibration";
		System_Display[ STR_Touch_Cal ]	= "EKRAN KAL.";
		System_Display[ STR_Touch_To_Cal ]	= "Please touch the crosshair to calibrate:"; 
		System_Display[ STR_Set_O2_Concen ]	= "Please select a check \nof O② concentration"; 
		System_Display[ STR_O2_10LMin ]	= "In ACGO or Manuel mode,adjust Air rate being equal or greater than 10L/Min,press Confirm to start";
		System_Display[ STR_Air_10LMin ]	= "Adjust Air rate being equal or greater than 10L/Min,then press Confirm to start";
		System_Display[ STR_Return ]	= "εíKíζ";
// 		System_Display[ STR_Flow_Close ]	= "Determine the flow valve \n is closed!";
		System_Display[ STR_Iso_gas_moudle ]	= "Please isolate the Gas Moudle\n from circuit and put\n it in the air.";
		System_Display[ STR_GASNOZERO ] = "Gas module can not be zeroed!";
		System_Display[ STR_Input_password ]	= "GǐRǐζ ζǐFRESǐ:";	
		System_Display[ STR_Password_Error ]	= "ζǐFRE HATASí!";	
		System_Display[ STR_N2O_Open]	= "N2O Open!";	
		System_Display[ STR_Insp_Valve_Cal ]	= "íNSP.VALF KAL";	
		System_Display[ STR_Exp_Flow_Sensor_Cal ]	= "EKSP.SENS.KAL.";	
		System_Display[ STR_Exp_Valve_Cal ]	= "EKSP.VALF KAL.";
		System_Display[ STR_Flowmeter_Cal ]	= "Flowmeter Calibration";
		System_Display[ STR_Calibrating ]	= "KALǐBRE EDǐLǐYOR...";
		System_Display[ STR_Cal_Failed ]	= "KALǐBRASYON BAζARíSíZ";	
		System_Display[ STR_Cal_Success ]	= "Calibration Success!";	
		System_Display[ STR_Waiting ]	= "Please waiting......";
		System_Display[ STR_No_Progam ]	= "No program can be upgraded!";
		System_Display[ STR_No_Response ]	= "\nno response!";
		System_Display[ STR_Continue ]	= "Continue";
		System_Display[ STR_Restart ]	= "Please restart your machine!";	
		System_Display[ STR_Full_Serial ]	= "Please input complete password!";	
		System_Display[ STR_Serial_Correct ]	= "Password correct!";	
		System_Display[ STR_Serial_Failure ]	= "Password incorrect!";	
		System_Display[ STR_Entry_Serial ]	= "Please entry the password:";
		System_Display[ STR_Backspace ]	= "Backspace";
		System_Display[ STR_Battery_Cal ]	= "Battery CalibrationXXXXXX";
		System_Display[ STR_Battery_Voltage ]	= "Battery Voltage";
		System_Display[ STR_Last_Battery_Cal ]	= "Last Battery Calibration:";	
		System_Display[ STR_Cal_Date ]	= "       Calibration Date:";	
		System_Display[ STR_Cal_Duration ]	= "       Calibration Duration:";	
		System_Display[ STR_All ]	= "All";	
		System_Display[ STR_Up ]	= "Up";
		System_Display[ STR_Input ]	= "Please input: ";
		System_Display[ STR_Input_Again ]	= "Please input again:";
		System_Display[ STR_Data_Not_Save ]	= "Data not saved\n confirm or cancel？";
		System_Display[ STR_Rec_Error ]	= "Receive error!";	
		System_Display[ STR_Rec_Timeout ]	= "Receive timeout!";	
		System_Display[ STR_Zero_Beyond ]	= "Zero point beyond!";	
		System_Display[ STR_EEPROM_Write_Error ]	= "EEPROM write error!";	
		System_Display[ STR_EEPROM_Read_Error ]  = "EEPROM read error!";
		System_Display[ STR_N2O_AD_Beyond ]  = "The N2O AD \nsampling beyond!";
		System_Display[ STR_Check_N2O ]  = "Please check the N2O!";
		System_Display[ STR_Check_AIR ]  = "Please check the Air!";	
		System_Display[ STR_Value1 ]  = "DEêERLER";	
// 		System_Display[ STR_Graphics ]  = "Graphics";	
// 		System_Display[ STR_Fresh_gas ] = "Fresh gas";	
// 		System_Display[ STR_Total_Gas ] = "Total flow";
// 		System_Display[ STR_Waveform ]	= "Waveforms";
// 		System_Display[ STR_Rapid_Monitor ]	= "Rapid Monitoring";
		System_Display[ STR_Paw ]	= "Paw";	
		System_Display[ STR_Pleth ]	= "Pleth";	
		System_Display[ STR_No_Vent_To_Patient ]	= "No ventilation delivered to the patient";	
		System_Display[ STR_Activate_Standby ]	= "Activate standby";	
		System_Display[ STR_Sec ]	= "Sec";
		System_Display[ STR_Backup ]	= "  Backup  ";
		System_Display[ STR_Paw_V_Loop ]	= "Paw-V";
		System_Display[ STR_V_Flow_Loop ]	= "V-Akιτ";		
		System_Display[ STR_ON ]	= "ON";	
		System_Display[ STR_OFF ]	= "OFF";		
		System_Display[ STR_English ]	=  "English";		
		System_Display[ STR_Pulse ]	= "ATIM";
		System_Display[ STR_SPO2 ]	= "SpO②";
		System_Display[ STR_PI ]	= "PI";
		System_Display[ STR_EFM ]	= "EFM";
		System_Display[ STR_SoftwareVersion ]	= "YAZíLíM VERSǐYONU";
		System_Display[ STR_Option ]	= "seçenek";//选配
		System_Display[ STR_Enter ]	= "GǐRǐζ";
		System_Display[ STR_EXIT ]	= "εíKíζ";
// 		System_Display[ STR_Layout2 ] =  "DúZEN 2";
		System_Display[ STR_Limits1 ] = "  SíNíRLAR 1  ";
		System_Display[ STR_Limits2 ] = " SíNíRLAR 2  ";
		System_Display[ STR_Limits3 ] = "SíNíRLAR 3 ";
		System_Display[ STR_N2O_Close ] = "N②O Close";		
		System_Display[ STR_Max_Flow ]  = "Max Flow";
		System_Display[ STR_Insp_Time ]  = "Inspiratory Time";
		System_Display[ STR_I_E ]  = "I : E";			
		System_Display[ STR_Pressure_Mode ]  = "BASíNε";
		System_Display[ STR_Chinese ]  = "简体中文";	
		System_Display[ STR_Turkish ]  = "TÜRKÇE";
		System_Display[ STR_Spanish ]  = "Español";
		System_Display[ STR_Flow ]  = "AKíζ"  ;
		System_Display[ STR_Total_Flow_Eng]  = "Total flow ";
		System_Display[ STR_N2O ]  = "N②0"	   ;
		System_Display[ STR_AA1 ]  = "AA1"	    ;
		System_Display[ STR_AA2 ]  = "AA2"	   ;
		System_Display[ STR_CO2 ]  = "CO②"    ;
		System_Display[ STR_EtCO2 ]  = "EtCO②"	;	
		System_Display[ STR_FiCO2 ]  = "FiCO②"	;		
		System_Display[ STR_Set_Mode ]  = "  MODLAR  "	;	
		System_Display[STR_Upgrade ]  = "Upgrade"	;
		System_Display[STR_Cancel_Upgrade ]  = "Cancel";
		System_Display[ STR_Continue_Upgrade ]  = "Continue";		

		System_Display[ STR_VT_1500 ]  = "Vt <= ";
		System_Display[ STR_Pinsp_70 ]  = "Pinsp <= 70 cmH②O";
		System_Display[ STR_Ie_41 ]  =  "I:E <= 4:1";
		System_Display[ STR_Tinsp_10 ]  = "Tinsp <= 10.0 s";
		System_Display[ STR_Tisnp_Tpause_10 ]  = "Tinsp - Tpause <= 10.0 s";
		System_Display[ STR_Psupp_70 ]  = "Psupp <= 70 cmH②O";
		System_Display[ STR_Simv_Freq_40 ]  = "SIMV FREQ <= 40 bpm";
		System_Display[ STR_Freq_100 ]  =  "FREQ <= 100 bpm";
		System_Display[ STR_Peep_30 ]  = "PEEP <= 30 cmH②O";
		System_Display[ STR_Pause_50 ]  = "Pause <= 60%";    //han
		System_Display[ STR_Esense_80 ]  = "Esens <= 80%";
		System_Display[ STR_Tslop_2 ]  = "Tslope <= 2.0s";
		System_Display[ STR_Fsense_20 ]  = "Fsens <= 20.0 LPM";
		System_Display[ STR_Psense_20 ]  = "Psens <= 20 cmH②O";

// 		System_Display[ STR_Vt_20 ]  =  "Vt Low Limit";
		System_Display[ STR_Vt_50 ]  = "Vt >= ";
		System_Display[ STR_Pinsp_5 ]  =  "Pinsp >= 5 cmH②O";
		System_Display[ STR_Ie_110 ]  = "I:E >= 1:10";
		System_Display[ STR_Tinsp_1 ]  = "Tinsp >= 0.1 s";
		System_Display[ STR_Freq_1 ]  = "SIMV FREQ >= 1 bpm";
		System_Display[ STR_Freq_4 ]  = "FREQ >= 4 bpm";  //sun add 20140416
		System_Display[ STR_Psupport_0 ]  = "Psupp >= 0 cmH②O";
		System_Display[ STR_Peep_Off ]  = "PEEP OFF";
		System_Display[ STR_Pause_0 ]  = "Pause >= 0%";;
		System_Display[ STR_Tslop_0 ]  = "Tslope >= 0.0s";
#if(LOGO_FLAG != PENLON_LOGO)//ylk add 20180709
		System_Display[ STR_Psense_0 ]  = "Psens >= 0 cmH②O";
		System_Display[ STR_Fsense_5 ]  = "Fsens >= 0.5 LPM";
#else
		System_Display[ STR_Psense_0 ]  = "Psens >= 1 cmH②O";
		System_Display[ STR_Fsense_5 ]  = "Fsens >= 1.0 LPM";
#endif			
		System_Display[ STR_Esense_5 ]  = "Esens >= 5%";
		System_Display[ STR_Pause ]  = "Pause";
		System_Display[ STR_Vt ]  = "VT";
		//
		System_Display[ STR_Tinsp_Tpause_1 ]  = "Tinsp - Tpause >= 0.1 s";
		System_Display[ STR_Tinsp_Tslop ]  =  "Tinsp >= Tslope";
		System_Display[ STR_Flow_High ]  = "Flow High Limit";
		System_Display[ STR_Flow_Low ]  = "Flow Low Limit";
		//
		System_Display[ STR_Psupport_Peep_70 ]  = "Psupp + PEEP <= 70 cmH②O";
		System_Display[ STR_Pinsp_Peep_70 ]  =  "Pinsp + PEEP <= 70 cmH②O";
		System_Display[ STR_Pinsp_Peep_Ph ]  = "Pinsp + PEEP < Ph";
		System_Display[ STR_Psupp_Peep_Ph ]  =  "Psupp + PEEP < Ph";
		System_Display[ STR_Peep_Ph ]  = "PEEP < Ph";
		System_Display[ STR_Texp_Low ]  = "Texp Low Limit";
		//
		System_Display[ STR_Backup_VCV ]  = "Backup VCV Actived";
		System_Display[ STR_Backup_PCV ]  =  "Backup PCV Actived";
		//
		System_Display[ STR_Pinsp_30 ]  = "Pinsp > 30 cmH②O ? Confirm";
		System_Display[ STR_Pinsp_50]  = "Pinsp > 50 cmH②O ? Confirm";
		System_Display[ STR_Peep_Psupport_30 ]  = "PEEP + Psupp > 30 cmH②O ? Confirm";
		System_Display[ STR_Peep_Psupport_50 ]  = "PEEP + Psupp > 50 cmH②O ? Confirm";
		System_Display[ STR_Peep_20 ]  = "PEEP > 20 cmH②O ? Confirm";
		System_Display[ STR_Ie_1 ]  = "I:E > 1:1 ? Confirm";					
		System_Display[ STR_Sec_1 ]  = "SANǐYE";
		System_Display[ STR_USA_Standard ]  = "USA Standard!";	
		System_Display[ STR_EN_Standard ]  = "EN Standard!";
		System_Display[ STR_FiN2O ]  = "FiN②O";		
// 		System_Display[ STR_ET ]  = "Et";	
// 		System_Display[ STR_FI ]  = "Fi";
// 		System_Display[ STR_FREQ_English ] ="f";
		System_Display[ STR_Value2 ]  = "DEêERLER 2";	
		System_Display[ STR_Set_EFM_Cal_Baro_Pressure ]  = "Please set the EFM\n checking air\n pressure value";
		System_Display[ STR_O2 ]  = "O②";
		System_Display[ STR_N20 ]  ="N②O"; 
		System_Display[ STR_AIR ]  =  "Air";
		System_Display[ STR_Cal_Error ]  =  "Calibration error！"; 	
		System_Display[ STR_Boot_Check_Error ]  =  "Boot self-checking\n error!";
// 		System_Display[ STR_O2_AD_Beyond ]   =  "The O2 AD \nsampling beyond!";
// 		System_Display[ STR_AIR_AD_Beyond ]	 =   "The Air AD \nsampling beyond!";	
		System_Display[ STR_Software_Version] = "Software Version 1. 0"	;	
		System_Display[ STR_User_BP_Set ]	       = "User BP set:" ;
		System_Display[ STR_Cal_BP_set  ]	       = "Calibration. BP set:" ;				
// 		System_Display[ STR_Flowmeter_Air_Test  ]	       = "Flow\nmeter \nAir test" ;		
		System_Display[ STR_Start]     = "Start";
		System_Display[ STR_Alarm_Silence ]	 = "Alarm Silence";
		System_Display[ STR_Alarm_Reset ]	 = "Alarm Reset";
		System_Display[ STR_Trend]	 = "Trends";
		System_Display[ STR_Trend_graph  ]	 = "Trends Graph";    
		System_Display[ STR_STANDBY	]				 = "Standby" ;   

		System_Display[ STR_FREQ ]	= "FREQ";
		System_Display[ STR_Mvspn ]  =  "Mvspn";//
		System_Display[ STR_Fspn ]   =  "fspn" ;//
		System_Display[ STR_PPEAK ]	 =  "Ppeak";//
		System_Display[ STR_Pmean ]	 =  "Pmean";//
		System_Display[ STR_PPLAT ]  =  "Pplat";
		System_Display[ STR_PEEP ]   =  "PEEP";
		System_Display[ STR_VTi ]    =  "VTi";//
		System_Display[ STR_Rst ]    =  "Rst";  
		System_Display[ STR_Cdyn ]   =  "Cdyn";
		System_Display[ STR_FiAA ]   =  "FiAA";
		System_Display[ STR_Ftotal ] =  "ftotal";
		System_Display[ STR_Loop ]   =  "LOOP";
		System_Display[ STR_MAC ]    =  "MAC";
		System_Display[ STR_FiN2O ]  =  "FiN②O";
		System_Display[ STR_EtN2O ]  =  "EtN②O";
		System_Display[ STR_FiAA1 ]  =  "FiAA1";
		System_Display[ STR_EtAA1 ]  =  "EtAA1";
		System_Display[ STR_FiAA2 ]  =  "FiAA2";
		System_Display[ STR_EtAA2 ]  =  "EtAA2";
		System_Display[ STR_AA ]     =  "AA";
		System_Display[ STR_Font_Version ]	= "Font_Version:";	
		System_Display[ STR_GUI ]   =  "GUI";
		System_Display[ STR_BDU ]   =  "BDU";
		System_Display[ STR_PMU ]   =  "PMU";
		System_Display[ STR_Pinsp ]         =  "Pinsp";
		System_Display[ STR_Tslope ]        =  "Tslope";
		System_Display[ STR_Psupp ]         =  "Psupp";
		System_Display[ STR_Fsens ]         =  "Fsens";
		System_Display[ STR_Esens ]         =  "Esens";
		System_Display[ STR_Tinsp ]         =  "Tinsp";
		System_Display[ STR_Psens ]         =  "Psens";
		System_Display[ STR_Apnea_time ]	= "  Apnea time";
		//选配
		System_Display[ STR_MODE_SIMV_P_PSV ]  =  "SIMV-P & PSV Mode";
		System_Display[ STR_MODE_PRVC ]        =  "PRVC Mode";
		System_Display[ STR_MODE_INFANT ]      =  "Infant Mode";
		System_Display[ STR_MODE_CO2 ]         =  "CO② Module";
		System_Display[ STR_MODE_SpO2 ]        =  "SpO② Module";
		System_Display[ STR_MODE_Driver ]      =  "Gas Driver Mode";
		System_Display[ STR_MODE_O2 ]          =  "O② Module";
		System_Display[ STR_MODE_Masimo_IRMA ]            =  "Masimo IRMA";
		System_Display[ STR_MODE_Masimo_ISA ]             =  "Masimo ISA";
		System_Display[ STR_MODE_Masimo_ISA_O2 ]          =  "Masimo ISA+O②";
		System_Display[ STR_MODE_Masimo_SpO2 ]            =  "Masimo SpO② Module";
		System_Display[ STR_MODE_AA_CO2 ]                 =  "AA & CO② Module";

		System_Display[ STR_MODE_Round ]      =  "○";

		System_Display[ STR_FiHAL ]  =  "FiHAL";
		System_Display[ STR_FiENF ]  =  "FiENF";
		System_Display[ STR_FiISO ]  =  "FiISO";
		System_Display[ STR_FiSEV ]  =  "FiSEV";
		System_Display[ STR_FiDES ]  =  "FiDES";
		System_Display[ STR_EtHAL ]  =  "EtHAL";
		System_Display[ STR_EtENF ]  =  "EtENF";
		System_Display[ STR_EtISO ]  =  "EtISO";
		System_Display[ STR_EtSEV ]  =  "EtSEV";
		System_Display[ STR_EtDES ]  =  "EtDES";

		System_Display[ STR_HAL ]    =  "HAL";//氟烷
		System_Display[ STR_ENF ]    =  "ENF";//安氟醚
		System_Display[ STR_ISO ]    =  "ISO";//异氟醚
		System_Display[ STR_SEV ]    =  "SEV";//七氟醚
		System_Display[ STR_DES ]    =  "DES";//地氟醚

		System_Display[STR_Dirver_Gas_Air] = "Dirver gas is air,can not close the switch!";
// 		System_Display[STR_Dirver_Gas_Change_Air] = "Drive gas has to switch to air!";
// 		System_Display[STR_Dirver_Gas_Change_O2] = "Drive gas has to switch to O②!";
		System_Display[STR_Close_Air_Switch] = "Whether close the Air switch?";
		System_Display[STR_Change_Dirver_Gas] = "Whether to change driver gas?";
		System_Display[STR_DRIVER] = "Driver";
		System_Display[STR_CLEARTREND] = "Modify time will clear trend data?";
		System_Display[ STR_Russian  ]	       = "Русский" ;
		System_Display[ STR_SPO2VER ]		 =  "SpO② Versión";
		System_Display[ STR_AVERTIME ]		 =  "Среднее время ";
		System_Display[ STR_SENSITIVITY ]	 =  "Чувствительность";
		System_Display[ STR_SMARTTONE ]		 =  "Smart Tone";
		System_Display[ STR_FASTSAT ]		 =  "Fast SAT";
		System_Display[ STR_MCUVER ]		 =  "MCU:";
		System_Display[ STR_DSPVER ]		 =  "DSP:";

		break;
#endif
	default:
		break;  
	} 
	//unit   
	System_Display[ STR_Unit_kPa ]				          = "kPa" ;   
	System_Display[ STR_Unit_mmHg ]				          = "mmHg" ;   
	System_Display[ STR_Unit_Percent ]				      = "%" ;   
	System_Display[ STR_Unit_cmH2O ]				      = "cmH②O" ;   
	System_Display[ STR_Unit_mbar ]				          = "mbar" ;   
	System_Display[ STR_Unit_cm ]				          = "cm" ;   
	System_Display[ STR_Unit_in ]				          = "in" ;   
	System_Display[ STR_Unit_hPa ]				          = "hPa" ;   
	System_Display[ STR_Unit_PSI ]				          = "PSI" ;   
	System_Display[ STR_Unit_lb ]				          = "Ib" ;   
	System_Display[ STR_Unit_kg ]				          = "kg" ;   
	System_Display[ STR_Unit_L ]				          = "L" ;   
	System_Display[ STR_Unit_L_min ]				      = "L/min" ;   
	System_Display[ STR_Unit_mL_min ]				      = "mL/min" ;   
	System_Display[ STR_Unit_mL ]				          = "mL" ;   
	System_Display[ STR_Unit_mL_cmH2O ]				      = "mL/cmH②O";   
	System_Display[ STR_Unit_mL_mbar ]				      = "mL/mbar" ;   
	System_Display[ STR_Unit_mL_cmH2O_L_S ]				  = "cmH②O/L/S";   
	System_Display[ STR_Unit_mL_mbar_L_S ]				  = "mbar/L/S" ;   
	System_Display[ STR_Unit_mL_sec ]				      = "sec" ;   
	System_Display[ STR_Unit_mL_s ]				          = "s" ;   
	System_Display[ STR_Unit_mL_LPM ]				      = "LPM" ;   
	System_Display[ STR_Unit_mL_bpm ]				      = "bpm" ;   
	System_Display[ STR_Unit_mL_mLPM ]				      = "MLPM" ; 
	System_Display[STR_AlarmLogSilenceBell]               =  "◎";


	System_Display[ STR_MEDIBUS_Format  ]		= "Medibus Protocol Start/Stop";  
	System_Display[ STR_MEDIBUS_Active  ]		= "Medibus Protocol active!";   
	System_Display[ STR_MEDIBUS_WCC		]		= "Waiting for communication connection...";  
	System_Display[ STR_MEDIBUS_PCRES	]		= "Waiting for PC response...";  
	System_Display[ STR_MEDIBUS_MCStop  ]		= "Medibus Protocol Stopped!";  
	System_Display[ STR_MEDIBUS_Start	]		= "Start";  
	System_Display[ STR_MEDIBUS_Wait	]		= "Waiting";  
	System_Display[ STR_MEDIBUS_Stop	]		= "Stop"; 

}


