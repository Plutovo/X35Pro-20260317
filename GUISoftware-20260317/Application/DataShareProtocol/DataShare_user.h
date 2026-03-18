#ifndef _DATASHARE_USER_H
#define _DATASHARE_USER_H
#include "DataShare_inc_ext.h"

#define DS_READ_GAS		0
//监测参数
#define	DS_PARA_PEEP    MonitorPara.PeepValue
#define	DS_PARA_PPEAK   MonitorPara.PpeakValue
#define	DS_PARA_PPLAT   MonitorPara.PplatValue
#define	DS_PARA_PMEAN   MonitorPara.PmeanValue
#define	DS_PARA_VTE     MonitorPara.VteValue
#define	DS_PARA_VTI     MonitorPara.VtiValue
#define	DS_PARA_MV      MonitorPara.MinuteVolumeValue
#define	DS_PARA_MVSPN   MonitorPara.MVspnValue
#define	DS_PARA_F       MonitorPara.RateTotalValue
#define	DS_PARA_FSPN    MonitorPara.fspnValue
#define	DS_PARA_IEI     Radio_IE.I
#define	DS_PARA_IEE     Radio_IE.E
#define	DS_PARA_COMPL   MonitorPara.ComplianceValue
#define	DS_PARA_RST     MonitorPara.ResistValue
#define	DS_PARA_PMIN    MonitorPara.PminValue
#define DS_PARA_RSBI    0//MonitorPara.RSBI
#define DS_PARA_CST     0//MonitorPara.StaticCompliance
#define DS_PARA_PEEPI   0//MonitorPara.PEEPi
#define	DS_PARA_FIO2    MonitorPara.FiO2Value
#define	DS_PARA_ETO2    0
#define	DS_PARA_ETCO2   CO2_Parameters.etCO2
#define	DS_PARA_FICO2   CO2_Parameters.insCO2
#define	DS_PARA_FIN2O   AA_all_data.AA_detail.AA_N2O.FI
#define	DS_PARA_ETN2O   AA_all_data.AA_detail.AA_N2O.ET
#define	DS_PARA_FIAA    AA_all_data.AA_detail.AA_AA1.FI
#define	DS_PARA_ETAA    AA_all_data.AA_detail.AA_AA1.ET
#define	DS_PARA_FIAA2   AA_all_data.AA_detail.AA_AA2.FI
#define	DS_PARA_ETAA2   AA_all_data.AA_detail.AA_AA2.ET
#define	DS_PARA_AA1ID	AA_all_data.AA_detail.AA_AA1.ID
#define	DS_PARA_AA2ID	AA_all_data.AA_detail.AA_AA2.ID
#define	DS_PARA_MAC     AA_Para.MAC
#define	DS_PARA_FLOW_N2O    MonitorPara.FlowMeterN2O
#define	DS_PARA_FLOW_AIR    MonitorPara.FlowMeterAir
#define	DS_PARA_FLOW_O2     MonitorPara.FlowMeterO2
#define	DS_PARA_SPO2        SpO2_Parameters.SpO2
#define	DS_PARA_PR          SpO2_Parameters.PulseRate
#define	DS_PARA_PI          SpO2_Parameters.PerfusionIndex
#define	DS_PARA_FLOWCHANNEL 0
#define	DS_PARA_STDYSTATE   StandbyState
#define DS_PARA_GASSWITCH   0
#define DS_PARA_INSPTIME    VentTime.InspTime
#define DS_PARA_EXPTIME     VentTime.ExpTime    


//控制参数
#define	DS_CTRLPARA_PEEP	BeathSettingPara.SettingPEEPValue
#define	DS_CTRLPARA_VT		BeathSettingPara.SettingVtValue
#define	DS_CTRLPARA_F		BeathSettingPara.SettingRateValue
#define	DS_CTRLPARA_IE		BeathSettingPara.SettingIEValue
#define	DS_CTRLPARA_TSLOPE	BeathSettingPara.SettingTslopeValue
#define	DS_CTRLPARA_TINSP	BeathSettingPara.SettingTiValue
#define	DS_CTRLPARA_PINSP	BeathSettingPara.SettingPinspValue
#define	DS_CTRLPARA_PSUPP	BeathSettingPara.SettingPsupportValue
#define	DS_CTRLPARA_TPAUSE	BeathSettingPara.SettingTpValue
#define	DS_CTRLPARA_ESENS	BeathSettingPara.EsenseValue
#define	DS_CTRLPARA_PSENS	BeathSettingPara.SettingPtrValue
#define	DS_CTRLPARA_FSENS	BeathSettingPara.SettingFtrValue
#define	DS_CTRLPARA_SIGH	BeathSettingPara.SettingSighTimesValue
#define	DS_CTRLPARA_PHIGH	0
#define	DS_CTRLPARA_PLOW	0
#define	DS_CTRLPARA_THIGH	BeathSettingPara.SettingThighValue
#define	DS_CTRLPARA_TLOW	BeathSettingPara.SettingTlowValue
#define	DS_CTRLPARA_FG		totalFlow//?
#define	DS_CTRLPARA_PERO2	O2Concen//?
#define DS_CTRLPARA_100O2	O2Concen100//?
#define	DS_CTRLPARA_MODE	BeathSettingPara.BreathMode

//报警限制
#define	DS_ALRMLIMHIGH_PAW		AlarmSettingPara.PressureHighValue
#define	DS_ALRMLIMHIGH_MV		AlarmSettingPara.MVHighValue
#define	DS_ALRMLIMHIGH_F		AlarmSettingPara.RateHighValue
#define	DS_ALRMLIMHIGH_FIO2		AlarmSettingPara.FiO2HighValue
#define	DS_ALRMLIMHIGH_VTE		AlarmSettingPara.VteHighValue
#define	DS_ALRMLIMHIGH_PEEP		0
#define	DS_ALRMLIMHIGH_ETCO2	AlarmSettingPara.EtCO2HighValue
#define	DS_ALRMLIMHIGH_FICO2	AlarmSettingPara.FiCO2HighValue
#define	DS_ALRMLIMHIGH_FIN2O	AlarmSettingPara.N2OHighValue
#define	DS_ALRMLIMHIGH_ETAA		AlarmSettingPara.EtAAHighValue
#define	DS_ALRMLIMHIGH_FIAA		AlarmSettingPara.FiAAHighValue
#define	DS_ALRMLIMHIGH_PULSE	AlarmSettingPara.PulseHighValue
#define	DS_ALRMLIMHIGH_SPO2		AlarmSettingPara.SpO2HighValue
#define	DS_ALRMLIMHIGH_PI		0
#define	DS_ALRMLIMLOW_PAW		AlarmSettingPara.PressureLowValue
#define	DS_ALRMLIMLOW_MV		AlarmSettingPara.MVLowValue
#define	DS_ALRMLIMLOW_F			AlarmSettingPara.RateLowValue
#define	DS_ALRMLIMLOW_FIO2		AlarmSettingPara.FiO2LowValue
#define	DS_ALRMLIMLOW_VTE		AlarmSettingPara.VteLowValue
#define	DS_ALRMLIMLOW_PEEP		0
#define	DS_ALRMLIMLOW_ETCO2		AlarmSettingPara.EtCO2LowValue
#define	DS_ALRMLIMLOW_FICO2		AlarmSettingPara.FiCO2LowValue
#define	DS_ALRMLIMLOW_FIN2O		AlarmSettingPara.N2OLowValue
#define	DS_ALRMLIMLOW_ETAA		AlarmSettingPara.EtAALowValue
#define	DS_ALRMLIMLOW_FIAA		AlarmSettingPara.FiAALowValue
#define	DS_ALRMLIMLOW_PULSE		AlarmSettingPara.PulseLowValue
#define	DS_ALRMLIMLOW_SPO2		AlarmSettingPara.SpO2LowValue
#define	DS_ALRMLIMLOW_PI		0

//报警
#define DS_ALARMS_0				AlarmsWork.presentAlarms
#define DS_ALARMS_STRS_0		AlarmContent
#define DS_ALARMS_1				AlarmsWork.presentAlarms1
#define DS_ALARMS_STRS_1		AlarmContent1
#define DS_ALARMS_HIGH_ETAA		ALARM_ETAA_HIGH
#define DS_ALARMS_HIGH_FIAA		ALARM_FIAA_HIGH
#define DS_ALARMS_LOW_ETAA		ALARM_ETAA_LOW
#define DS_ALARMS_LOW_FIAA		ALARM_FIAA_LOW
#define DS_ALARMS_AA_FIND(type, id)	searchAAalarm(type, id)

//病人
#define DS_PATIENT_ID			Patient_GetPID()
#define DS_PATIENT_NAME			Patient_GetPName()
#define DS_PATIENT_GENDER		Patient_GetPGender()


#endif

