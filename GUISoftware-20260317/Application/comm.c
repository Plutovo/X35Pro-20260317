#include "comm.h"
#define extern 
#include "MasimoSPO2.h"

#ifdef __ICCARM__
#include "UART.h"
#endif

char DeviceSerial_String[9] = {0};
char DeviceSerial_Table[8] = {0};
unsigned char  SPO2_RecvFinishFlag = 0;
unsigned int DeviceSerial;
unsigned  char    USART0_TXD_Busy_FLAG;
unsigned long long  USART0_TXD_RIGSTER;
int OS_TimeMS;
Int8U   USART0_TXD_Busy_FLAG = FALSE;
Int8U	Usart0_Tx_Buffer[4];
Int8U	Usart3_Tx_Buffer[4];
Int8U	Usart4_Tx_Buffer[4];
unsigned char butter_press_count;
// u8 ExtUart1_SendBuf[MEDIBUS_FIFOTXNUM] = {0};
 CHINACO2_InitTypedef ChinaCO2_Init;
Int8U   ModeChange_Flag;
PhaseInISA_InitTypedef PhaseInISA_Init;
extern BREATH_SET_PARA Breath_Setting_Tmp;
Int16U  USART2_TXD_RIGSTER;
Int8U   USART2_TXD_Busy_FLAG;
extern int BreathMode_BeforeACGO;
#ifdef DEMO  //  [11/8/2022 lyy add]Demo
extern unsigned char Demo_Switch;
extern char Demo_Change_Cycle_A;
#endif

void EXTUART1_Init(unsigned int BR1)
{

}
void EXTUART1_SendByte(u8 data)
{

}
u8 Medibus_GetPCResStopFlag(void)
{
	return 0;
}
u8 Medibus_GetCommActiveFlag(void)
{
	return 0;
}
u8 Medibus_GetUserStopFlag(void)
{
	return 0;
}
void Medibus_PrepareSend(void)
{

}
void Medibus_Init(void)
{

}
void Medibus_SetUserStopFlag(u8 data)
{
	
}
void Medibus_FiniteStateMachine(void)
{

}
/***************************************************************************************************
* Function Name: SendsetParameter
* Parameters: void
* Return: void
*
* Description:
*
*************************************************************************/
void SendSetParameter(int Value,int Id)
{
    if( (Breath_Setting_Tmp.BreathMode == VCV_MODE)
        || (Breath_Setting_Tmp.BreathMode == SIGH_MODE)

		|| (Breath_Setting_Tmp.BreathMode == APRV_MODE)
		|| (Breath_Setting_Tmp.BreathMode == BIPAP_MODE)
		|| (Breath_Setting_Tmp.BreathMode == VS_MODE)

        || (Breath_Setting_Tmp.BreathMode == PRVC_MODE)
        || (Breath_Setting_Tmp.BreathMode == SIMV_VCV_MODE)
        || (Breath_Setting_Tmp.BreathMode == SIMV_PRVC_MODE)
        || ((Breath_Setting_Tmp.BreathMode == PSV_MODE)&&(Breath_Setting_Tmp.ApnoeaMode == VCV_MODE)) //  20130513
		|| (Breath_Setting_Tmp.BreathMode == MANUAL_MODE)
		|| (Breath_Setting_Tmp.BreathMode == ACGO_MODE)          
		) 
        {
        switch(Id)
        {
        case GUI_ID_BUTTON0:
			if (Breath_Setting_Tmp.BreathMode == VS_MODE)
			{
				Breath_Setting_Tmp.SettingVtSupportValue = Value;
			}
			else if((Breath_Setting_Tmp.BreathMode == APRV_MODE) || (Breath_Setting_Tmp.BreathMode == BIPAP_MODE))
			{
				Breath_Setting_Tmp.SettingPhighValue = Value;
			}
			else
			{
				Breath_Setting_Tmp.SettingVtValue = Value;
			}
            break;
        case GUI_ID_BUTTON1:
			if( (Breath_Setting_Tmp.BreathMode == APRV_MODE) || (Breath_Setting_Tmp.BreathMode == BIPAP_MODE) )
			{
				Breath_Setting_Tmp.SettingPlowValue = Value;
			} 
            else if( (Breath_Setting_Tmp.BreathMode == VCV_MODE)
                || (Breath_Setting_Tmp.BreathMode == SIGH_MODE)
                || (Breath_Setting_Tmp.BreathMode == PRVC_MODE)
                || ((Breath_Setting_Tmp.BreathMode == PSV_MODE)&&(Breath_Setting_Tmp.ApnoeaMode == VCV_MODE))
				|| (((Breath_Setting_Tmp.BreathMode == MANUAL_MODE)||(Breath_Setting_Tmp.BreathMode == ACGO_MODE))
				&& ((BreathMode_BeforeACGO == VCV_MODE)||(BreathMode_BeforeACGO == SIGH_MODE)||(BreathMode_BeforeACGO == PRVC_MODE)
				|| (BreathMode_BeforeACGO == PSV_MODE)))
                )
            {
                Breath_Setting_Tmp.SettingIEValue = Value;
                if(Value >= 6)
                {
                    Value = 10*60*100/((10+IE_RatioTable[Value])*Breath_Setting_Tmp.SettingRateValue);//IE转换成TI cpp //cpp del 20130306
// 					Value = 10*60*(100-Breath_Setting_Tmp.SettingTpValue)/((10+IE_RatioTable[Value])*Breath_Setting_Tmp.SettingRateValue*1.0) + 0.5;//  20130306
                }
                else
                {
                    Value = IE_RatioTable[Value]*60*100/((10+IE_RatioTable[Value])*Breath_Setting_Tmp.SettingRateValue);//cpp del 20130306
// 					Value = IE_RatioTable[Value]*60*(100-Breath_Setting_Tmp.SettingTpValue)/((10+IE_RatioTable[Value])*Breath_Setting_Tmp.SettingRateValue);//  20130306
                }
                Breath_Setting_Tmp.SettingTiValue = Value;//
                //USART0_TXD_RIGSTER |= SEND_SET_TI_FLAG;
            }
            else//SIMV-V SIMV-PRVC 
            {
                Breath_Setting_Tmp.SettingTiValue = Value;
                //USART0_TXD_RIGSTER |= SEND_SET_TI_FLAG;
                Breath_Setting_Tmp.settingTeValue = (6000/Breath_Setting_Tmp.SettingSimvRateValue - Breath_Setting_Tmp.SettingTiValue);
                {
//                        MonitorPara.IEValue = (float)Breath_Setting_Tmp.SettingTiValue / Breath_Setting_Tmp.settingTeValue;
                }//*///
            }
            //*///endof   20130402
            break;
        case GUI_ID_BUTTON2:
			if(Breath_Setting_Tmp.BreathMode == APRV_MODE)
			{
				Breath_Setting_Tmp.SettingThighValue = Value;
			} 
			else if(Breath_Setting_Tmp.BreathMode == BIPAP_MODE)
			{
				Breath_Setting_Tmp.SettingBipapRateValue = Value;
			}
			else if(Breath_Setting_Tmp.BreathMode == VS_MODE)
			{
				Breath_Setting_Tmp.SettingVSRateValue = Value;
			}
            else if((Breath_Setting_Tmp.BreathMode ==SIMV_VCV_MODE)
                ||(Breath_Setting_Tmp.BreathMode ==SIMV_PRVC_MODE)
				||( ((Breath_Setting_Tmp.BreathMode == MANUAL_MODE)||(Breath_Setting_Tmp.BreathMode == ACGO_MODE))
				&&((BreathMode_BeforeACGO == SIMV_VCV_MODE)||(BreathMode_BeforeACGO == SIMV_PRVC_MODE)) )
				)
            {
                Breath_Setting_Tmp.SettingSimvRateValue = Value;
				Breath_Setting_Tmp.SettingRateValue = Value;
                //USART0_TXD_RIGSTER |= SEND_SET_F_SIMV_FLAG;
            }
            else // VS_MODE
            {
                Breath_Setting_Tmp.SettingRateValue = Value;
				Breath_Setting_Tmp.SettingotherRateValue = Value;
                //USART0_TXD_RIGSTER |= SEND_SET_F_FLAG;

                //*//  20130402 发送完F后，还需再发送Ti
                Value = Breath_Setting_Tmp.SettingIEValue;
                if(Value >= 6)
                {
                    Value = 10*60*100/((10+IE_RatioTable[Value])*Breath_Setting_Tmp.SettingRateValue);//IE转换成TI cpp //cpp del 20130306
// 					Value = 10*60*(100-Breath_Setting_Tmp.SettingTpValue)/((10+IE_RatioTable[Value])*Breath_Setting_Tmp.SettingRateValue*1.0) + 0.5;//  20130306
                }
                else
                {
                    Value = IE_RatioTable[Value]*60*100/((10+IE_RatioTable[Value])*Breath_Setting_Tmp.SettingRateValue);//cpp del 20130306
// 					Value = IE_RatioTable[Value]*60*(100-Breath_Setting_Tmp.SettingTpValue)/((10+IE_RatioTable[Value])*Breath_Setting_Tmp.SettingRateValue);//  20130306
                }
                Breath_Setting_Tmp.SettingTiValue = Value;
                //USART0_TXD_RIGSTER |= SEND_SET_TI_FLAG;
                //*///endof   20130402
            }
            break;
        case GUI_ID_BUTTON3:
			if(Breath_Setting_Tmp.BreathMode == APRV_MODE)
			{
				Breath_Setting_Tmp.SettingTlowValue = Value;
			}
			else if(Breath_Setting_Tmp.BreathMode == BIPAP_MODE)
			{
				Breath_Setting_Tmp.SettingThighValue = Value;
			}
			else if(Breath_Setting_Tmp.BreathMode == VS_MODE)
			{
				Breath_Setting_Tmp.SettingPHLimitValue = Value;
			}
            else if((Breath_Setting_Tmp.BreathMode==VCV_MODE)|(Breath_Setting_Tmp.BreathMode==SIGH_MODE)
				||( ((Breath_Setting_Tmp.BreathMode == MANUAL_MODE)||(Breath_Setting_Tmp.BreathMode == ACGO_MODE))
				&&((BreathMode_BeforeACGO == VCV_MODE)||(BreathMode_BeforeACGO == SIGH_MODE)) )
				)
            {
                Breath_Setting_Tmp.SettingTpValue = Value;
                //USART0_TXD_RIGSTER |= SEND_SET_TP_FLAG;
                //*//  20130402 发送完Tp后，还需再发送Ti
                Value = Breath_Setting_Tmp.SettingIEValue;
                if(Value >= 6)
                {
                    Value = 10*60*100/((10+IE_RatioTable[Value])*Breath_Setting_Tmp.SettingRateValue);//IE转换成TI cpp //cpp del 20130306
// 					Value = 10*60*(100-Breath_Setting_Tmp.SettingTpValue)/((10+IE_RatioTable[Value])*Breath_Setting_Tmp.SettingRateValue*1.0) + 0.5;//  20130306
                }
                else
                {
                    Value = IE_RatioTable[Value]*60*100/((10+IE_RatioTable[Value])*Breath_Setting_Tmp.SettingRateValue);//cpp del 20130306
// 					Value = IE_RatioTable[Value]*60*(100-Breath_Setting_Tmp.SettingTpValue)/((10+IE_RatioTable[Value])*Breath_Setting_Tmp.SettingRateValue);//  20130306
                }
                Breath_Setting_Tmp.SettingTiValue = Value;
                //USART0_TXD_RIGSTER |= SEND_SET_TI_FLAG;
                //*///endof   20130402
            }
            else
            {
                Breath_Setting_Tmp.SettingPsupportValue = Value;
                //USART0_TXD_RIGSTER |= SEND_SET_PSV_FLAG;
                Menu_hWin.InfoWin.InfoChangeFlag |= MODES_INFO_CHANGED_FLAG;
            }
            break;
            
        case GUI_ID_BUTTON4: //  20130607
            Breath_Setting_Tmp.SettingTslopeValue = Value;
            //USART0_TXD_RIGSTER |= SEND_SET_PEEP_FLAG;
            break; 
            
        case GUI_ID_BUTTON5: //GUI_ID_BUTTON4 cpp md 20130607
            if(Breath_Setting_Tmp.TriggerMode==PRESSURE_TRIGGER)
            {
                Breath_Setting_Tmp.SettingPtrValue = Value;
                //USART0_TXD_RIGSTER |= SEND_SET_PT_TRIG_FLAG;
            }
            else
            {
                Breath_Setting_Tmp.SettingFtrValue = Value;
                //USART0_TXD_RIGSTER |= SEND_SET_FLOW_TRIG_FLAG;
            }
            break;
        case GUI_ID_BUTTON6: //GUI_ID_BUTTON4 cpp md 20130607
			if(Value < 3)
			{
				Value = 0;
			}
            Breath_Setting_Tmp.SettingPEEPValue = Value;
            //USART0_TXD_RIGSTER |= SEND_SET_PEEP_FLAG;
            break;
		case GUI_ID_BUTTON7: 
			Breath_Setting_Tmp.EsenseValue = Value;
			break;
        default:
            break;
        }
    }
    else 
    if( (Breath_Setting_Tmp.BreathMode == PCV_MODE)
        || (Breath_Setting_Tmp.BreathMode == SIMV_PCV_MODE)
        || ((Breath_Setting_Tmp.BreathMode == PSV_MODE)&(Breath_Setting_Tmp.ApnoeaMode == PCV_MODE)) //  20130513
		|| (Breath_Setting_Tmp.BreathMode == MANUAL_MODE)
		|| (Breath_Setting_Tmp.BreathMode == ACGO_MODE)
        ) 
    {
        switch(Id)
        {
        case GUI_ID_BUTTON0:
            Breath_Setting_Tmp.SettingPinspValue = Value;
            //USART0_TXD_RIGSTER |= SEND_SET_PCV_FLAG;
            break;
        case GUI_ID_BUTTON1:
            if( (Breath_Setting_Tmp.BreathMode == APRV_MODE)
                ||(Breath_Setting_Tmp.BreathMode == BIPAP_MODE)
				||( ((Breath_Setting_Tmp.BreathMode == MANUAL_MODE)||(Breath_Setting_Tmp.BreathMode == ACGO_MODE))
				&&((BreathMode_BeforeACGO == APRV_MODE)||(BreathMode_BeforeACGO == BIPAP_MODE)) )
                )
            {
                Breath_Setting_Tmp.SettingThighValue = Value;
                //USART0_TXD_RIGSTER |= SEND_SET_THIGH_FLAG;
            }
            /*//
            else //cpp del 20130402
            {
                Breath_Setting_Tmp.SettingTiValue = Value;
                USART0_TXD_RIGSTER |= SEND_SET_TI_FLAG;
            }//*/
            //*//  20130402
            else 
            if( (Breath_Setting_Tmp.BreathMode == PCV_MODE)
                ||((Breath_Setting_Tmp.BreathMode == PSV_MODE)&&(Breath_Setting_Tmp.ApnoeaMode == PCV_MODE)) //  20130513
// 				  ||( ((Breath_Setting_Tmp.BreathMode == MANUAL_MODE)||(Breath_Setting_Tmp.BreathMode == ACGO_MODE))
// 				  &&((BreathMode_BeforeACGO == PCV_MODE)||((BreathMode_BeforeACGO == PSV_MODE) && (Breath_Setting_Tmp.ApnoeaMode == PCV_MODE))) )
                )
            {
                Breath_Setting_Tmp.SettingIEValue = Value;
                if(Value >= 6)
                {
                    Value = 10*60*100/((10+IE_RatioTable[Value])*Breath_Setting_Tmp.SettingRateValue*1.0) + 0.5;//IE转换成TI cpp
                }
                else
                {
                    Value = IE_RatioTable[Value]*60*100/((10+IE_RatioTable[Value])*Breath_Setting_Tmp.SettingRateValue);
                }
                Breath_Setting_Tmp.SettingTiValue = Value;
                //USART0_TXD_RIGSTER |= SEND_SET_TI_FLAG;
            }
            else
            {
                Breath_Setting_Tmp.SettingTiValue = Value;
                //USART0_TXD_RIGSTER |= SEND_SET_TI_FLAG;
            }
            //*///endof   20130402
            break;
        case GUI_ID_BUTTON2:
//            if((Breath_Setting_Tmp.BreathMode !=APRV_MODE)
//              &(Breath_Setting_Tmp.BreathMode !=PCV_MODE)
//              &(Breath_Setting_Tmp.BreathMode !=BIPAP_MODE))
            if((Breath_Setting_Tmp.BreathMode == SIMV_PCV_MODE)
				||( ((Breath_Setting_Tmp.BreathMode == MANUAL_MODE)||(Breath_Setting_Tmp.BreathMode == ACGO_MODE))
				&&(BreathMode_BeforeACGO == SIMV_PCV_MODE) )
				)
            {
                Breath_Setting_Tmp.SettingSimvRateValue = Value;
				Breath_Setting_Tmp.SettingRateValue = Value;
                //USART0_TXD_RIGSTER |= SEND_SET_F_SIMV_FLAG;
            }
            else 
            if((Breath_Setting_Tmp.BreathMode == APRV_MODE)
				||( ((Breath_Setting_Tmp.BreathMode == MANUAL_MODE)||(Breath_Setting_Tmp.BreathMode == ACGO_MODE))
				&&(BreathMode_BeforeACGO == APRV_MODE) )
				)
            {
                Breath_Setting_Tmp.SettingTlowValue = Value;
                //USART0_TXD_RIGSTER |= SEND_SET_TLOW_FLAG;
            }
            else 
            if( (Breath_Setting_Tmp.BreathMode == PCV_MODE)
                || ((Breath_Setting_Tmp.BreathMode == PSV_MODE)&&(Breath_Setting_Tmp.ApnoeaMode == PCV_MODE)) //  20130513
				||( ((Breath_Setting_Tmp.BreathMode == MANUAL_MODE)||(Breath_Setting_Tmp.BreathMode == ACGO_MODE))
				&&(BreathMode_BeforeACGO == PCV_MODE)||(BreathMode_BeforeACGO == PSV_MODE) )
                )
            {
                Breath_Setting_Tmp.SettingRateValue = Value;
				Breath_Setting_Tmp.SettingotherRateValue = Value;
                //USART0_TXD_RIGSTER |= SEND_SET_F_FLAG;

                //*//  20130402 发送完频率后，还需再发送吸气时间
                Value = Breath_Setting_Tmp.SettingIEValue;
                if(Value >= 6)
                {
                    Value = 10*60*100/((10+IE_RatioTable[Value])*Breath_Setting_Tmp.SettingRateValue*1.0) + 0.5;//IE转换成TI cpp
                }
                else
                {
                    Value = IE_RatioTable[Value]*60*100/((10+IE_RatioTable[Value])*Breath_Setting_Tmp.SettingRateValue);
                }
                Breath_Setting_Tmp.SettingTiValue = Value;
                //USART0_TXD_RIGSTER |= SEND_SET_TI_FLAG;
                //*///endof   20130221
            }
            else
            {
                Breath_Setting_Tmp.SettingBipapRateValue = Value;
				Breath_Setting_Tmp.SettingRateValue = Value;
                //USART0_TXD_RIGSTER |= SEND_SET_F_BIPAP_FLAG;
            }
            break;
        case GUI_ID_BUTTON3:
            if((Breath_Setting_Tmp.BreathMode!=PCV_MODE)
				||( ((Breath_Setting_Tmp.BreathMode == MANUAL_MODE)||(Breath_Setting_Tmp.BreathMode == ACGO_MODE))
				&&(BreathMode_BeforeACGO!=PCV_MODE) )
				)
            {
                Breath_Setting_Tmp.SettingPsupportValue = Value;
                //USART0_TXD_RIGSTER |= SEND_SET_PSV_FLAG;
                Menu_hWin.InfoWin.InfoChangeFlag |= MODES_INFO_CHANGED_FLAG;
            }
            break;

            
        case GUI_ID_BUTTON4: //  20130607
            Breath_Setting_Tmp.SettingTslopeValue = Value;
            //USART0_TXD_RIGSTER |= SEND_SET_PEEP_FLAG;
            break; 
            
            
        case GUI_ID_BUTTON5: //GUI_ID_BUTTON4 cpp md 20130607
            if(Breath_Setting_Tmp.TriggerMode==PRESSURE_TRIGGER)
            {
                Breath_Setting_Tmp.SettingPtrValue = Value;
                //USART0_TXD_RIGSTER |= SEND_SET_PT_TRIG_FLAG;
            }
            else
            {
                Breath_Setting_Tmp.SettingFtrValue = Value;
                //USART0_TXD_RIGSTER |= SEND_SET_FLOW_TRIG_FLAG;
            }
            break;
        case GUI_ID_BUTTON6: //GUI_ID_BUTTON5 cpp md 20130607
			if(Value < 3)
			{
				Value = 0;
			}
            Breath_Setting_Tmp.SettingPEEPValue = Value;
            //USART0_TXD_RIGSTER |= SEND_SET_PEEP_FLAG;
            break;
//            case GUI_ID_BUTTON6:
//                Breath_Setting_Tmp.SettingFiO2Value = Value;
//                //USART0_TXD_RIGSTER |= SEND_SET_FIO2_FLAG;
//                break;
		case GUI_ID_BUTTON7: 
			Breath_Setting_Tmp.EsenseValue = Value;
			break;
        default:
            break;
        }
    }
    //if(USART0_TXD_Busy_FLAG!=TRUE)
    //{
    //    Inquire_TXD_Handler(USART0);
    //}
#ifdef DEMO  //  [11/8/2022 lyy add]Demo
	if(Demo_Switch)
	{
		Demo_Change_Cycle_A = 1;
	}
#endif
}
void SendSetModes(int modes)
{
}
//struct 
//{
//	char NoSensorConnected;//
//	char DefectiveSensor;//
//	char LowPerfusion;//
//	char PulseSearch;//
//	char InterferenceDetected;//
//	char SensorOffPatient;//
//	char TooMuchAmbientLight;//
//	char UnrecognizedSensor;//
//	char LowSignalIQ;//
//	char MasimoSETProcessingActive;//
//	char NoCableConnected;
//	char NoAdhesiveSensorConnected;
//	char DemoMode;
//}Exception2Info;
//RTC_TIMER RTCReadTime(void)
//{
//    RTC_TIMER time;
//    time.RTC_Hour = 16;
//    time.RTC_Mday = 15;
//    time.RTC_Min = 36;
//    time.RTC_Mon = 7;
//    time.RTC_Year = 2014;
//    return(time);
//}
RTC_TIMER RTCReadTime(void)
{
    time_t systime;
    struct tm *syslocal;
    RTC_TIMER timer;
    systime = time(NULL);
    syslocal = localtime(&systime);
    timer.RTC_Sec  = syslocal->tm_sec;
    timer.RTC_Hour = syslocal->tm_hour;
    timer.RTC_Mday = syslocal->tm_mday;
    timer.RTC_Min = syslocal->tm_min;
    timer.RTC_Mon = syslocal->tm_mon+1;
    timer.RTC_Year = syslocal->tm_year+1900;
    return(timer);
}
// int CheckAcStatus(void)
// {
//   #if 0
//     int temp = TRUE;
//   #else
// 	int temp = FALSE;
//   #endif
//     return(temp);
// }
// unsigned char CheckBatteryStatus(void)
// {
// 	return FALSE; 
// }
void SendOperationMessage(unsigned char type,unsigned char UsartPort)
{
}
void RTCSetTime(RTC_TIMER Time)
{
}
char RTEEPROMread(unsigned int addr)
{
    char data = 0;

    VentTime.InspTime = 1000; //  20130719
    VentTime.ExpTime = 2000;

    return data;
}
void RTEEPROMwrite(unsigned short int addr, unsigned char databyte)
{
}
void VolumeChannelSelect(int data)
{
}
void SelfTestEnd(void)
{
}
void SelfTestStart(void)
{
    DEMOFLAG = 0;
    SpO2ModuleExist = 1;
    AA_Module_Flag.EtCO2ModuleExist = 1; //  20130409
}
void BeepAloudControl(unsigned char status)
{
}
// unsigned char CheckChargeStatus(void)
// {
//     return 0;
// }
void Co2ModuleOff(unsigned int off)
{

}
char RTEEPROMwrite_Start(unsigned short int addr)
{
    return 1;
}
char RTEEPROMwrite_Data(char databyte)
{
    return 1;
}
char RTEEPROMwrite_End(void)
{
    return 1;
}

char RTEEPROMread_Start(unsigned short int addr)
{
    return 1;
}
char RTEEPROMread_Data(void)
{
    return 1;
}
char   RTEEPROMread_End(void)
{
    return 1;
}
void InitTrendQueue_Alias(WM_HWIN hProgBar)
{

}
void Disable_Interrupt(void)
{
}
void Enable_Interrupt(void)
{
}
void    EXT_UartTransmit(unsigned char DataType,const unsigned char DataLength,unsigned char UsartPort)
{
    return;
}
void WriteExtUsart1(unsigned char data)
{}
void EXT_USART_Init(void)
{}
Int16U  Inquire_TXD_Handler(Int8U UsartPort)
{
    return 1;
}
void    DataTransmit(Int16U data,Int8U type,Int8U * Pointer,Int8U UsartPort)
{
}

unsigned char UART2_Init(unsigned int BR)
{}

unsigned char UART1_Init(unsigned int BR)
{}

void I2C0_MultiSend(unsigned int reg,unsigned short int *buffer,unsigned int num)  
{
	
	
}

void I2C0_MultiReceive(unsigned int reg,unsigned short int  *buffer,unsigned int num)  
{

}
void GetD1D2SequenceData(void)
{
}
void GetWaveDataSmartToneON(void)
{
}
void SPO2Module_Send(char d1,char d2,char d3,char d4,char d5,char d6,char d7)
{
}
void SPO2Modue_SendAverageTime(unsigned char flag)//触控设置波形平均时间时，调用该函数
{
}
void SPO2Modue_SendSensitivity(unsigned char Sensitivity,unsigned char FastSAT)
{
}
void SPO2Module_SendSmartToneONOrOff(unsigned char flag)
{
}
void DiagnoFailureCodes(void)
{
}
void BoardFailureCodes(void)
{
}
void Exception2Code()
{
}
void DSPFirmVersionCode(void)
{
}
void MCUFirmVersionCode(void)
{
}
void InitialiazeSPO2Varity()
{
}
void SpO2_Chose(void)
{


}
// int Power_GetBatteryVoltage()
// {
// 	return 9500;
// }
// void Power_SendMachineModel(int type)
// {
// 
// }
void UART4_SendByte_Poll(unsigned char data)
{
}