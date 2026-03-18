#include "includes.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include "sys.h"
#include "MainTask.h"
//#include "Alarm_handle.h"
#include "Communication_Handle.h"
#include "Data_Handle.h"
#include "Calibration_Protocol.h"
#include "Alarm_handle.h"
#include "MasimoSPO2.h"
#include "MedibusComm.h"
#include "UART.h"

extern int OS_TimeMS;
extern ALARM_INFO ShowedAlarmInfo;
extern REALTIME_PARA RealtimePara;
extern Int16U BatteryVoltageSample[FILTER_NUM];
extern SPEAKER_TEST SpeakerState;
extern Int16U   DataFilterHandle(Int16U *Pdata,Int16U Len);
extern Int16U   GetMinData(Int16U *pdata,Int16U len);//  20130319
extern Int16U   CalculatePressureValue(Int16U data);
extern void Uart3IntHandler(void);
extern void Uart4IntHandler(void);

extern ALARM_SET_PARA AlarmSettingPara;
extern Int16U LedUpdateRegister;
extern OPERATION_TIME OperationTime;
extern Int8U SaveOperationTimeFlag;
extern int CalibrateBatteryFlag;//  20130304
extern VENT_TIME VentTime;//  20130616
extern MONITOR_PARA MonitorPara; //  20130618
extern VENT_TIME GetVentTime(int VentState); //  20130618
extern int GetThisYears_OfDay(int year,int month);
extern int MenuType;
extern int StandbyState;
extern unsigned char EtCO2_Config_On;
extern unsigned char EtCO2_KM7002_Config_On;
extern unsigned char Masimo_EtCO2_Config_On;
extern unsigned char MasimoISA_EtCO2_Config_On;
extern Int8U  ETCO2_FLAG;
extern unsigned int ETCO2_ConnFlag;
//extern unsigned char AA_Module_Flag.EtCO2ModuleExist;
extern unsigned char pHaseinMoudleCheck;//??pHasein??????
extern unsigned int PhaseInIRMA_ConnFlag;//??pHasein?????????
extern unsigned char EtCO2ModuleOff;
extern unsigned char EtCO2ModuleClear_Flag;

Int16U BatteryIndex = 0;

extern BREATH_SET_PARA BeathSettingPara;
int usart0_wait_rx[SEND_COMMAND_NUM]= {0};//??????????????????0 ???????????
extern INT64U  USART0_TXD_RIGSTER;   //djq add 20140312 
extern INT64U  USART0_CHECK_RIGSTER; //djq add 20140312  
extern INT8U   USART0_TXD_Busy_FLAG; //djq add 20140312
//djq add 20140305 ????
extern int Barameter_Pressure;         //??????,?????   0~999mmHg
extern int Barameter_Stand_Pressure;   //????????   0~999mmHg
extern int Barameter_Switch;           //0 :???????,???BDU????   1:?,???????
extern Int16U GasSourceState;          //?????
extern Int16U BatteryAdValue;
extern unsigned char alarm_battery_cali;

extern CALI_PROTOCOL_CTRL_S Cali_Ctrl_S;
extern INT8U   EXTUSART1_TXD_Busy_FLAG;
extern INT8U   EXTUsart1_Tx_Buffer[5];

unsigned short SPO2_NoPatientFlag = 1<<15;
unsigned short SPO2_NoSensorFlag = 1<<15;
PhaseInISA_InitTypedef PhaseInISA_Init;
CHINACO2_InitTypedef ChinaCO2_Init;
extern unsigned char Drager_EtCO2_Config_On; //0:关闭   1：开启麻醉气体    2：开启麻醉气体+顺磁氧 

u8 ExtUart1_SendBuf[MEDIBUS_FIFOTXNUM] = {0};

void ExtUsartIntHandler(void)
{
  static  INT16U  ExtUart2RxBuffer[3]= {0};
  static  INT8U   Rx_num=0,Tx_num = 0;

  static  INT16U  Wait_Syn=TRUE;
  static  INT16U  Flowmeter_Syn = TRUE;
  static  INT16U  LastData=0;
  static   INT16U EXT1_TXD_Cnt = 1;
  static   INT8U EXT1_Head1 = 0;
  INT8U data,cnt = 0;
  INT8U   ExtUart1data = 0;
  if(IO0INTSTATR)
  {
      if(IO0INTSTATR_bit.P0_18)
      {
          //      data = ReadExtUsart2Status();
          //      if(data&RXRDY)
          //      {
          //      }
          //      else if(data&TXRDY)
          //      {
          //      }
          //      IO0INTCLR_bit.P0_4 = 1;
          IO0INTCLR_bit.P0_18 = 1;
      }
      if(IO0INTSTATR_bit.P0_17) //medibus
      {       
          IO0INTCLR_bit.P0_17 = 1; 
          data = Chip_EXUART_ReadIntIDReg(LPC_EXUART1);
          //接收器线路状态错误清除
          if((data&EXUART_IIR_INTID_RLS) != RESET)
          {
              Chip_EXUART_ReadLineStatus(LPC_EXUART1);
          }
          
          //接收超时 || 有数据
          if((data&EXUART_IIR_INTID_RDA) != RESET || 
             (data&EXUART_IIR_INTID_CTI) != RESET)
          {
              if(uart_dp_readop() != NULL && uart_dp_readop()->uart_recv != NULL)
              {
                  //读取硬件FIFO所有数据
                  while((Chip_EXUART_ReadLineStatus(LPC_EXUART1)&EXUART_LSR_RDR) != RESET)
                  {
                      data = Chip_EXUART_ReadByte(LPC_EXUART1);
                      uart_dp_readop()->uart_recv(data);
                  }
              }
              else
              {
                  data = Chip_EXUART_ReadByte(LPC_EXUART1);
              }
          }
          
          //发送
          if((data&EXUART_IIR_INTID_THRE) != RESET)//发送中断
          {
              if(uart_dp_readop() != NULL && uart_dp_readop()->uart_send != NULL)
              {
                  uart_dp_readop()->uart_send();
              }
              else
              {
                  Chip_EXUART_IntDisable(LPC_EXUART1, EXUART_IER_THREINT);
              }
          }
      }          
  }
}


void Timer0IntrHandler (void)
{   
  static unsigned char irma_cnt = 0;
    OS_TimeMS++;
    SpeakerVoiceTest();  

#ifdef __debug
		if(U0LSR & (1<<6))
			U0THR = 0x30;
#endif
    //AlarmAuditorySignalsHandle(ShowedAlarmInfo.Grade,ShowedAlarmInfo.Type,ShowedAlarmInfo.ActiveState);
    
    if((MenuType!=STANDBY_MENU)&&(StandbyState!=0xFF))
    {
        VentTime = GetVentTime(MonitorPara.VentState); //  20130616
    }
    //zkq modify 20170504

    if((SPO2_NoPatientFlag&SPO2_NoPatientCountFlag) == SPO2_NoPatientCountFlag)//zkq add 20170628 计时检测病人未连接状态
    {
        SPO2_NoPatientFlag++;
        if( (SPO2_NoPatientFlag&(~SPO2_NoPatientCountFlag)) > 2000)
        {
            SPO2_NoPatientFlag &= SPO2_NoPatientClearCount;//计数清零
            Exception2Info.SensorOffPatient = 0;
        }
    }
    else if((SPO2_NoPatientFlag&SPO2_NoPatientCountFlag) == (0<<15))
    {
        SPO2_NoPatientFlag &= SPO2_NoPatientClearCount;//
        Exception2Info.SensorOffPatient = 1;
    }
    
    if((SPO2_NoSensorFlag&SPO2_NoSensorCountFlag) == SPO2_NoSensorCountFlag)//zkq add 20170628 计时检测sensor未连接状态
    {
        SPO2_NoSensorFlag++;
        if( (SPO2_NoSensorFlag&(~SPO2_NoSensorCountFlag)) > 2000)
        {
            SPO2_NoSensorFlag &= SPO2_NoSensorClearCount;//计数清零
            Exception2Info.NoSensorConnected = 0;
        }
    }
    else if((SPO2_NoSensorFlag&SPO2_NoSensorCountFlag) == (0<<15))
    {
        SPO2_NoSensorFlag &= SPO2_NoSensorClearCount;//
        Exception2Info.NoSensorConnected = 1;
    }    
    
    if(MasimoISA_EtCO2_Config_On && (AA_Module_Flag.EtCO2ModuleExist == 2)&&(AA_Module_Flag.PhaseInTypeModuleExist==2))//PhaseIn ISA
    {
    	if(AA_Module_Flag.InitFinishFlag == 1)//初始化完毕
        {
            if((PhaseInISA_Init.PhaseInISA_Count&(1<<15)) == 0)
            {
                PhaseInISA_Init.PhaseInISA_Count++;
                if((PhaseInISA_Init.PhaseInISA_Count&~(1<<15)) >= 6000)//延时4S
                {
                    PhaseInISA_Init.PhaseInISA_Count = 1<<15;
                    PhaseInISA_Init.PhaseInISA_State = 1;
                }
            }
        }
    }
    
    if((Masimo_EtCO2_Config_On || MasimoISA_EtCO2_Config_On) && Drager_EtCO2_Config_On == 0)
    {
        PhaseInIRMA_ConnFlag++;
        if(PhaseInIRMA_ConnFlag > 5000)//5S内没有通讯则认为断开
        {
            PhaseInIRMA_ConnFlag = 0;
            pHaseinMoudleCheck = 1;
            if(EtCO2ModuleOff)
            {
                EtCO2ModuleClear_Flag = 0;
                //EtCO2ModuleOff = 0;  
            }
            if(AA_Module_Flag.EtCO2ModuleExist == 2)
            {
                AA_Module_Flag.EtCO2ModuleExist = 0;//没有检测到通讯 关闭连接
                AA_Module_Flag.PhaseInTypeModuleExist = 0;//关闭PhaseIn类型模块的连接
            }
        }
    }  

    if(EtCO2_Config_On || EtCO2_KM7002_Config_On)//开通了国产CO2选配
    {
        ETCO2_ConnFlag++;
        if(ETCO2_ConnFlag > 5000)//检测国产CO2是否连接
        {
        ETCO2_ConnFlag = 0;
        if(AA_Module_Flag.EtCO2ModuleExist == 1)
        {
          AA_Module_Flag.EtCO2ModuleExist = 0;//没有检测到通讯 关闭连接
        }
        }
    }
    else
    {
        ETCO2_ConnFlag = 0;
        if((AA_Module_Flag.EtCO2ModuleExist&0x01) == 1)//若开通了选配
        {
        AA_Module_Flag.EtCO2ModuleExist = 0;// 直接关闭连接
        }
    }
    if(EtCO2_Config_On || EtCO2_KM7002_Config_On)//国产CO2 
    {
        if((ChinaCO2_Init.ChinaCo2_Flag&0x01) == 0x00)
        {
            ChinaCO2_Init.ChinaCo2_Count++;
            if(ChinaCO2_Init.ChinaCo2_Count >= 10000)//10s
            {
                ChinaCO2_Init.ChinaCo2_Count = 0;
                ChinaCO2_Init.ChinaCo2_Flag = 0x03;//最低2位置1
            }
        }
    }    
    
    if(irma_cnt > 50) //ylk add 20181205
    {
        irma_cnt = 0;
        if(AA_Module_Flag.InitFinishFlag && (!AA_Module_Flag.ETCO2_FLAG))
        {
            irma_getpara();
        }
    }
    else
    {
      irma_cnt++;
    }
    
#ifdef __MEDIBUS
    Medibus_TimeOut();
    Medibus_Timer1();
    Medibus_Timer2();
#ifndef __Medibus_ITSend
    Medibus_SendTxFIFO();
#endif
#endif
    T0IR_bit.MR0INT = 1;   
}

//2016.5.17 zy 将2478采样值修改为12位精度，与1788匹配
unsigned short int battery_adc;
void Timer1IntrHandler (void)
{
    Int8U ad_mux_sel;
		
		if(FIO1PIN & 1<<7)
			LED_On();
		else
			LED_Off();
		
    ad_mux_sel = AD0GDR_bit.CHN;
    switch(ad_mux_sel)
    {
#ifndef NEW_POWER_COMM
    case 0x07:
        {
            if(!BatteryAdValue)
            {
                if(BatteryIndex % 100 == 0)
                {
                    BatteryAdValue = BatteryIndex/100;
                    if(((BatteryIndex >= 100)&&(alarm_battery_cali != 0xff))//如果当前没有进行电池校验，那么10s更新一次电池信息。
                     ||((BatteryIndex >= 600)&&(alarm_battery_cali == 0xff)))
                    {
                        BatteryIndex = 0;
                    }        
                }
								
                BatteryVoltageSample[BatteryIndex] = AD0GDR_bit.RESULT ;						
                BatteryIndex++;

                AD_Channel_Start(7);
								//AD_READ(7,&battery_adc);
            }
        }
        break;
#endif
    default:
        break;    
    }

    T1IR_bit.MR0INT = 1;
}

void Timer2IntrHandler()
{
    INT64U i = 0,temp = 1;
//    if(!Demo_Switch)
//    {  
        Check_CommFault();//模块通讯失败检测 //ylk add 20181115 
//    }
    for(; i <SEND_COMMAND_NUM; i++ )
    {
        if(i>0)
        {
            temp = temp<<1;
        }
        if(USART0_CHECK_RIGSTER & temp)
        {
            usart0_wait_rx[i] ++;
            if(usart0_wait_rx[i] <= 2400)
            {
                switch(usart0_wait_rx[i])
                {
                case 1200:
                case 2400:
                    USART0_TXD_RIGSTER |= temp;
                    if(USART0_TXD_Busy_FLAG==FALSE)
                    {
                        Inquire_TXD_Handler(USART0);
                    }
                    break;
                }
            }
            else
            {
                USART0_CHECK_RIGSTER &= ~temp;
                usart0_wait_rx[i] = 0;
            }
        }
    }
    
    T2IR_bit.MR0INT = 1;

}


void RTCIntrHandler(void)
{
    RTCILR_bit.RTCCIF = 1;     /* clear interrupt flag */
    OperationTime.Min++;
    if(OperationTime.Min>=60)
    {
        OperationTime.Min = 0;
        OperationTime.Hour++;
    }
    SaveOperationTimeFlag = TRUE;
}