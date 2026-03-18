#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include "includes.h"
#include "MainTask.h"
#include "sys.h"
#include "Communication_Handle.h"
#include "Data_Handle.h"

//
extern int OS_TimeMS;

extern REALTIME_PARA RealtimePara;
extern Int16U BatteryVoltageSample[FILTER_NUM];
static Int16U AirHighPressureSample[FILTER_NUM]= {0};
static Int16U OxyHighPressureSample[FILTER_NUM]= {0};
static Int8U AdConverseFinish = 0;
extern SPEAKER_TEST SpeakerState;
const Int8U VolumeTable[5] = {1,6,2,4,0};
const Int16U HighPriorAud[10]= {100,100,350,100,600,100,100,350,100,6600};
const Int16U HighAlarmTuneTab[5]= {500,500,500,400,200};
const Int16U MediumAlarmTuneTab[5]= {500,500,500};
const Int16U MediumPriorAud[]= {300,300,25000};
extern Int16U   DataFilterHandle(Int16U *Pdata,Int16U Len);
extern Int16U   GetMinData(Int16U *pdata,Int16U len);//  20130319
extern Int16U   CalculatePressureValue(Int16U data);
extern void Uart3IntHandler(void);
extern ALARM_SET_PARA AlarmSettingPara;
extern Int16U LedUpdateRegister;

extern int CalibrateBatteryFlag;//  20130304
//extern void Wr_Battery_Ad(void);//  20130304


extern VENT_TIME GetVentTime(int VentState); //  20130618
extern int GetThisYears_OfDay(int year,int month);
//extern float GetIERatio(VENT_TIME _VentTime); //  20130616 //cpp del 20130716



//sun add 20151102
extern Int8U   ETCO2_FLAG;
extern INT64U  USART0_TXD_RIGSTER;   //djq add 20140312 
extern INT64U  USART0_CHECK_RIGSTER; //djq add 20140312  
extern INT8U   USART0_TXD_Busy_FLAG; //djq add 20140312

extern BREATH_SET_PARA BeathSettingPara;

//djq add 20140305 大气压力
extern int Barameter_Pressure;         //设置信息界面，大气压力值   0~999mmHg
extern int Barameter_Stand_Pressure;   //校验地大气压力值   0~999mmHg
extern int Barameter_Switch;           //0 ：大气压传感器开，显示为BDU上传的值   1：关，显示为设置值。
extern Int16U GasSourceState;          //气源状态。


extern int StandbyState;
extern MENU_HWIN Menu_hWin;
extern int usart0_wait_rx[SEND_COMMAND_NUM];
         
/*报警信号处理      */
void AlarmAuditorySignalsHandle(Int8U Priority,Int32U Type,Int8U flag)
{
    static Int8U num=0;
    static Int8U num1=0;
    static Int16U timer=0;
    static Int8U PulseHighLevel=TRUE;
    static Int8U LastPriority = 0;
    static Int32U LastType = 0;
    Int8U  r = 0;
    if(flag==TRUE)
    {
        if(Priority!=LastPriority)
        {
            timer=0;
            num=0;
            num1=1;
            PulseHighLevel=TRUE;
            r = 1;
        }
        if(Priority==HIGH_PRIORITY)
        {
            if(r)
            {
                PWM_Set(HighAlarmTuneTab[0]);
                PWM_Start();
                num1 = 1;
            }
            timer++;
            if(PulseHighLevel==TRUE)
            {
                if(timer>=150)
                {
                    PWM_Stop();
                    timer = 0;
                    PulseHighLevel = FALSE;
                }
            }
            else
            {
                if(timer>=HighPriorAud[num])
                {
                    timer = 0;
                    num++;
                    if(num>9)
                    {
                        num = 0;
                    }
                    if(num1>=5)
                    {
                        num1=0;
                    }
                    PWM_Set(HighAlarmTuneTab[num1]);
                    num1++;
                    PulseHighLevel = TRUE;
                    PWM_Start();
                }
            }
        }
        else if(Priority==MEDIUM_PRIORITY)
        {
            if(r)
            {
                PWM_Set(500);
                PWM_Start();
            }
            timer++;
            if(PulseHighLevel==TRUE)
            {
                if(timer>=200)
                {
                    PWM_Stop();
                    timer = 0;
                    PulseHighLevel = FALSE;
                }
            }
            else
            {
                if(timer>=MediumPriorAud[num])
                {
                    timer = 0;
                    num++;
                    if(num>=3)
                    {
                        num = 0;
                    }
                    PWM_Set(500);
                    PulseHighLevel = TRUE;
                    PWM_Start();
                }
            }
        }
        else if(Priority==LOW_PRIORITY)
        {
            if(r)
            {
                PWM_Set(700);
                PWM_Start();
            }
            timer++;
            if(timer>=200)
            {
                PWM_Stop();
                timer = 0;
            }
        }
    }
    else
    {
        PWM_Stop();
    }
    LastPriority = Priority;
    LastType = Type;
}

void SpeakerVoiceTest(void)
{
    static int time = 0;
    if(SpeakerState.TestState == TRUE)
    {
        time++;
        if(time == 1)
        {
            //VolumeChannelSelect(SpeakerState.SpeakerVolume);
            PWM_Set(500);
            PWM_Start();
        }
        else if(time>=500)
        {
            time = 0;
            SpeakerState.TestState = FALSE;
            PWM_Stop();
            // VolumeChannelSelect(AlarmSettingPara.AlarmVolumeValue);
        }
    }
}
   
void Content_Check_Handler(INT16U data,INT8U type)
{

    switch(type)
    {
    case VT_COMMAND_SET:
        if(data == BeathSettingPara.SettingVtValue)  
        {
            USART0_CHECK_RIGSTER &= ~(SEND_SET_VT_FLAG);//01
            usart0_wait_rx[0] = 0;
        }
        break;
    case PT_COMMAND_SET:
        if(data == BeathSettingPara.SettingPtrValue)
        {
            USART0_CHECK_RIGSTER &= ~SEND_SET_PT_TRIG_FLAG;//02 
            usart0_wait_rx[1] = 0;
        }
        break;
    case FLOW_TRIGGER_COMMAND_SET:
        if(data == BeathSettingPara.SettingFtrValue)
        {
            USART0_CHECK_RIGSTER &= ~SEND_SET_FLOW_TRIG_FLAG;//03 
            usart0_wait_rx[2] = 0;
        }
        break;
    case FIO2_COMMAND_SET:
        if(data == BeathSettingPara.SettingFiO2Value)
        {
            USART0_CHECK_RIGSTER &= ~SEND_SET_FIO2_FLAG;//04 
            usart0_wait_rx[3] = 0;
        }
        break;
    case TI_COMMAND_SET:
        if(data == BeathSettingPara.SettingTiValue)
        {
            USART0_CHECK_RIGSTER &= ~SEND_SET_TI_FLAG;//05 
            usart0_wait_rx[4] = 0;
        }
        break;
    case PSV_COMMAND_SET:
        if(data == BeathSettingPara.SettingPsupportValue)
        {
            USART0_CHECK_RIGSTER &= ~SEND_SET_PSV_FLAG;//06 
            usart0_wait_rx[5] = 0;
        }
        break;
    case PEEP_COMMAND_SET:
        if(data == BeathSettingPara.SettingPEEPValue)
        {
            USART0_CHECK_RIGSTER &= ~SEND_SET_PEEP_FLAG;//07 
            usart0_wait_rx[6] = 0;
        }
        break;
    case F_COMMAND_SET:
        if(data == BeathSettingPara.SettingRateValue)
        {
            USART0_CHECK_RIGSTER &= ~SEND_SET_F_FLAG;//08 
            usart0_wait_rx[7] = 0;
        }
        break;
    case F_SIMV_COMMAND_SET:
        if(data == BeathSettingPara.SettingRateValue)
        {
            USART0_CHECK_RIGSTER &= ~SEND_SET_F_SIMV_FLAG;//09 
            usart0_wait_rx[8] = 0;
        }
        break;
    case PCV_COMMOND_SET :
        if(data == BeathSettingPara.SettingPinspValue)
        {
            USART0_CHECK_RIGSTER &= ~SEND_SET_PCV_FLAG;//10 
            usart0_wait_rx[9] = 0;
        }
        break;
    case TAP_COMMAND_SET:
        if(data == AlarmSettingPara.ApneaTimerValue)
        {
            USART0_CHECK_RIGSTER &= ~SEND_SET_TAP_FLAG;//11 
            usart0_wait_rx[10] = 0;
        }
        break;
    case F_BIPAP_COMMAND_SET:
        if(data == BeathSettingPara.SettingRateValue)
        {
            USART0_CHECK_RIGSTER &= ~SEND_SET_F_BIPAP_FLAG;//12 
            usart0_wait_rx[11] = 0;
        }
        break;
    case PHL_COMMAND_SET:
        if(data == AlarmSettingPara.PressureHighValue)
        {
            USART0_CHECK_RIGSTER &= ~SEND_SET_PHL_FLAG;//13 
            usart0_wait_rx[12] = 0;
        }
        break;
    case RUN_STANDBY_COMMAND:
        if(data == StandbyState)
        {
            USART0_CHECK_RIGSTER &= ~SEND_RUN_STANDBY_FLAG;//14 
            usart0_wait_rx[13] = 0;
        }
        break;
    case O2_SCANT_COMMAND_ALARM :
        if(data == (GasSourceState>>8)&0xFF)   //15
        {
            USART0_CHECK_RIGSTER &= ~SEND_O2_SCANT_FLAG;
            usart0_wait_rx[14] = 0;
        }
        break;
    case  AIR_SCANT_COMMAND_ALARM :  // 16
        break;
    case SYSTEM_MODE_COMMAND:
        if(data == APNEA_MODE)
        {
            USART0_CHECK_RIGSTER &= ~SEND_APNEA_FLAG;//17 
            usart0_wait_rx[16] = 0;
        }
        else if(data == BeathSettingPara.BreathMode)
        {
            USART0_CHECK_RIGSTER &= ~SEND_SYSTEM_MODE_FLAG;//18 
            usart0_wait_rx[17] = 0;
        }
        
        
        else if(data == PRESS_ZERO_CORRECT_COM<<8)
        {
            USART0_CHECK_RIGSTER &= ~SEND_PRESSURE_CALIBRATION_FLAG; //27 
            usart0_wait_rx[26] = 0;
        }
        else if(data == FLOW_SENSOR_CORRECT_COM<<8)
        {
            USART0_CHECK_RIGSTER &= ~SEND_FLOW_CALIBRATION_FLAG; //28 
            usart0_wait_rx[27] = 0;
        }
        else if(data == OXY_ZERO_CORRECT_COM<<8)
        {
            USART0_CHECK_RIGSTER &= ~SEND_OXY_CALIBRATION_FLAG;//29
            usart0_wait_rx[28] = 0;
        }
     
        else if(data == EXP_VALVE_CORRECT_COM<<8)
        {
            USART0_CHECK_RIGSTER &= ~SEND_EXP_VALVE_CALIBRATION_FLAG;//30
            usart0_wait_rx[29] = 0;
        }
       
        else if(data == FACTORY_VALVE_CORRECT_COM<<8)
        {
            USART0_CHECK_RIGSTER &= ~SEND_INSP_VALVE_CALIBRATION_FACTORY_FLAG;//31
            usart0_wait_rx[30] = 0;
        }
//        else if(data == EXP_FLOWSENSOR_CORRECT_COM<<8)  //32 
        {
//            USART0_CHECK_RIGSTER &= ~SEND_EXP_FLOW_SENSOR_CALIBRATION_FLAG;
//            usart0_wait_rx[31] = 0;
        }
        break;
    case TRIG_ACTIVE_COMMAND:
        if(data == BeathSettingPara.TriggerMode)
        {
            USART0_CHECK_RIGSTER &= ~SEND_TRIG_ACTIVE_FLAG;//19 
            usart0_wait_rx[18] = 0;
        }
        break;
    case INSP_HOLD_COMMAND://20  吸气保持
        break;
    case EXP_PAUSE_COMMAND:// 21 呼气暂停
        break;
    case O2_SUCTION_COMMAND://22 纯氧
        break;
    case MANUAL_INSP_COMMAND:
        if(data == 0)
        {
            USART0_CHECK_RIGSTER &= ~SEND_MANUAL_INSP_FLAG;//23  
            usart0_wait_rx[22] = 0;
        }
        break;
    case THIGH_COMMAND_SET:
        if(data == BeathSettingPara.SettingTiValue)
        {
            USART0_CHECK_RIGSTER &= ~SEND_SET_THIGH_FLAG;//24 
            usart0_wait_rx[23] = 0;
        }
        break;
    case TLOW_COMMAND_SET:
        if(data == BeathSettingPara.SettingTlowValue)
        {
            USART0_CHECK_RIGSTER &= ~SEND_SET_TLOW_FLAG;//25 
            usart0_wait_rx[24] = 0;
        }
        break;
    case TP_COMMAND_SET:
        if(data == BeathSettingPara.SettingTpValue)
        {
            USART0_CHECK_RIGSTER &= ~SEND_SET_TP_FLAG;//26 
            usart0_wait_rx[25] = 0;
        }
        break;
    case SIGH_TIMES_COMMAND_SET:
        if(data == BeathSettingPara.SettingSighTimesValue)
        {
            USART0_CHECK_RIGSTER &= ~SEND_SIGH_TIME_FLAG;//33 
            usart0_wait_rx[32] = 0;
        }
        break;
    case TSLOPE_COMMAND_SET:
        if(data == BeathSettingPara.SettingTslopeValue)
        {
            USART0_CHECK_RIGSTER &= ~ SEND_SET_TSLOP_FLAG;//34 
            usart0_wait_rx[33] = 0;
        }
        break;        
    case BAROMETER_SEND_COMMAND_VALUE:
        if(data == Barameter_Pressure)
        {
            USART0_CHECK_RIGSTER &= ~SEND_BAROMETER_FLAG;//37 
            usart0_wait_rx[36] = 0;
        }
        break;        
    case BAROMETER_SEND_STAND_COMMAND_VALUE:
        if(data == Barameter_Stand_Pressure)
        {
            USART0_CHECK_RIGSTER &= ~SEND_BAROMETER_STAND_FLAG;//38 
            usart0_wait_rx[37] = 0;
        }
        break;
    case ESENSE_COMMAND_VALUE: //sun add 20140623
        if(data == BeathSettingPara.EsenseValue)
        {
            USART0_CHECK_RIGSTER &= ~SEND_SET_ESENSE_FLAG;
            usart0_wait_rx[38] = 0;
        }
        break;
    case BACKUP_MODE_COMMAND:
        if(data == BeathSettingPara.ApnoeaMode)
        {
            USART0_CHECK_RIGSTER &= ~SEND_BACKUP_MODE_FLAG;
            usart0_wait_rx[39] = 0;
        }
        break;      
    }
}

/***************************************************************************************************
 * @brief  : 将整形数据转换成字符串
 * @param  : -radix =10 表示10进制，其他结果为0
 *           -value 要转换的整形数
 *           -buf 转换后的字符串
 *           -radix = 10
 * @retval : none
 ************************************************************************************************/
char *itoa(int value, char *string, int radix)
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;
    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }
    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }
    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';
        /* Make the value positive. */
        value *= -1;
    }
    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;
        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }
    /* Null terminate the string. */
    *ptr = 0;
    return string;
} /* NCL_Itoa */

/*
****************************************************************************************************
 * 函数名：USART2_printf
 * 描述  ：格式化输出，类似于C库中的printf，但这里没有用到C库
 * 输入  ：-USARTx 串口通道，这里只用到了串口1，即USART1
 *           -Data   要发送到串口的内容的指针
 *             -...    其他参数
 * 输出  ：无
 * 返回  ：无
 * 调用  ：外部调用
 *         典型应用USART1_printf( USART1, "\r\n this is a demo \r\n" );
 *                   USART1_printf( USART1, "\r\n %d \r\n", i );
 *                   USART1_printf( USART1, "\r\n %s \r\n", j );
 **************************************************************************************************/
void USART2_printf(INT8U *Data,...)
{
    const char *s;
    int d;
    char buf[16];
    va_list ap;
    va_start(ap, Data);
    while ( *Data != 0)     // 判断是否到达字符串结束符
    {
        if ( *Data == 0x5c )  //'\'
        {
            switch ( *++Data )
            {
            case 'r':                                     //回车符
                U2THR = 0x0d;
                Data ++;
                break;
            case 'n':                                     //换行符
                U2THR = 0x0a;
                Data ++;
                break;
            default:
                Data ++;
                break;
            }
        }
        else if ( *Data == '%')
        {
            //
            switch ( *++Data )
            {
            case 's':                                         //字符串
                s = va_arg(ap, const char *);
                for ( ; *s; s++)
                {
                    U2THR = *s;
                    while(!U2LSR_bit.THRE)
                    {
                        ;
                    }
                }
                Data++;
                break;
            case 'd':                                       //十进制
                d = va_arg(ap, int);
                itoa(d, buf, 10);
                for (s = buf; *s; s++)
                {
                    U2THR = *s;
                    while(!U2LSR_bit.THRE)
                    {
                        ;
                    }
                }
                Data++;
                break;
            default:
                Data++;
                break;
            }
        } /* end of else if */
        else
        {
			U2THR = *Data++;
        }
		while(!U2LSR_bit.THRE)
        {
            ;
        }
    }
}
void VolumeChannelSelect(Int8U channel)
{
  Int8U temp;
  if(channel<1)
  {
    channel = 1;
  }
  if(channel>5)
  {
    channel = 5;
  }
  temp = VolumeTable[channel-1];
  FIO1SET |= temp;
  FIO1CLR |= (~temp & 0x07); 
}

void BeepAloudControl(Int8U status)
{
    if(status==ENABLE)
    {
        FIO1SET_bit.P1_3 = 1;
    }
    else if(status==DISABLE)
    {
        FIO1CLR_bit.P1_3 = 1;
    }
}
#ifndef NEW_POWER_COMM
Int8U CheckAcStatus(void)
{
    Int8U state = FALSE;
    if(FIO0PIN_bit.P0_30&0x01)//IO1PIN_bit.P1_5 //cpp md 20130129
    {
        state = TRUE;
    }
    return(state);
}

Int8U CheckChargeStatus(void)
{
    Int8U state = 1;//0 //cpp md 20130129
    if(FIO0PIN_bit.P0_31&0x01)//IO1PIN_bit.P1_6 //cpp md 20130129
    {
        state = 0;//0 //cpp md 20130129
    }
    return(state);
}
#endif
void SelfTestStart(void)
{
    LedUpdateRegister = 0xFFFF;
    SendOperationMessage(LED_UPDATE_OPERATION,USART3);
    SendOperationMessage(BUZZER_VOLUME_START,USART3);//BUZZER_VOLUME_TEST  //cpp md 20130402
    //VolumeChannelSelect(5);
    //PWM_Set(500);
    //PWM_Start();
}

void SelfTestEnd(void)
{
    LedUpdateRegister = STANDBY_LED;
    SendOperationMessage(LED_UPDATE_OPERATION,USART3);
    Battery_Check();//ylk add 20181115
    //PWM_Stop();
}




