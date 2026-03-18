#include "iolpc1788 and 2478.h"
#include "GUI.h"
#include "JM1_SpO2.h"
#include "UART.h"

extern JM1_SPO2_PARAMETERS SpO2_Parameters;
extern unsigned char HeartBeatFlag;
extern unsigned short SPO2_NoPatientFlag;
extern unsigned short SPO2_NoSensorFlag;
unsigned char SPO2_RecvFinishFlag = 0;

#define extern 
#include "MasimoSPO2.h"


void InitialiazeSPO2Varity(void)
{
    ModeConf[6] = 0; //初始化应为零，！！！！！
    SPO2WaveTimer = 0;//初始化应为零！！！！！！
    SPO2AlgorithmAPOD = 0;//初始化应为0！！！！！！
    SPO2ModeConfig = 0;//初始化应为零！！！！！！
    NND1 = 0;
    NND2 = 0;
    NND3 = 0;
    NND4 = 0;//初始化应为零！！！！！！
    NNW0 = 0;
    NNW1 = 0;
    NNW2 = 0;
    NNW3 = 0;
    NNW4 = 0;
    NNW5 = 0;
    NNW6 = 0;
    NNW7 = 0;
    NNW8 = 0;
    NNW9 = 0;//初始化应为零！！！！！！
    SmartToneChangeFlag = 1;
    FastSatChangeFlag = 1;
    SensitivityChangeFlag = 1;
    AverageTimeChangeFlag = 1;
    SPO2FailureFlag = 0;
    SensorFailureFlag = 0;
    AlarmDelayTime = 3;//3s
    SPO2NNCheckOK = 0;
    SPO2CurveReceOK = 0;
    
    //波形初始化
    SPO2WaveIR = 0;
    SPO2WaveSIQSmartOn = -0x5F;//初始化数据
    SPO2WaveSIQSmartOff = -0x5F;
    
    //初始化工作状态
    SPO2Module_SendSmartToneONOrOff(Masimo_SmartToneSwitch);
    SPO2Modue_SendAverageTime(Masimo_AverageTime);
    SPO2Modue_SendSensitivity(Masimo_Sensitivity,Masimo_FastSATSwitch);
//    delay(100);
//    MSpO2_SendExpirationMsg3(MSpO2_Excep3_Open);
    MSpO2_SendExpirationMsg3(0x01);
//    delay(100);
    
}
void SPO2Module_Send(char d1,char d2,char d3,char d4,char d5,char d6,char d7)//发送给模块的命令
{
    char checksum;
    
    
    while(UART_CheckBusy(UART1) != 1);//忙
    checksum = d1%256 + d2%256 + d3%256 + d4%256 + d5%256 + d6%256 + d7%256;
    U1THR = SOM_SPO2;
    U1THR = d1;
    U1THR = d2;
    U1THR = d3;
    U1THR = d4;
    
    U1THR = d5;
    U1THR = d6;
    U1THR = d7;
    U1THR = checksum; 
    U1THR = EOM_SPO2;
}
void MSpO2_SendExpirationMsg3(unsigned short data)
{
    SPO2Module_Send(COM_Expiration3_SpO2, (data>>8)&0x0F, data&0x0F, 0, 0, 0, 0);
}
void SPO2Modue_SendAverageTime(unsigned char SPO2AverageTime)//触控设置波形平均时间时，调用该函数
{
    char avg;
    switch(SPO2AverageTime)
    {
    case 0://2-4秒
        avg = 0;
        AlarmDelayTime = 0;//0s
        break;
    case 1://4-6秒
        avg = 1;
        AlarmDelayTime = 1;//1s
        break;
    case 2://8秒
        avg = 2;
        AlarmDelayTime = 3;//3s
        break;
    case 3://10秒
        avg = 3;
        AlarmDelayTime = 3;//3s
        break;
    case 4://12 秒
        avg = 4;
        AlarmDelayTime = 3;//3s
        break;
    case 5://14秒
        avg = 5;
        AlarmDelayTime = 10;//10s
        break;
    case 6://16秒
        avg =6;
        AlarmDelayTime = 10;//10s
        break;      
    default:
        // avg = 2;//默认为8秒
        //AlarmDelayTime = 10;//10s
        break;
        
    }
    /* 报警 延迟机制
    2s :Confirm time 0s
    4s:Confirm time 1s
    8s:Confirm time 3s
    10s:Confirm time 3s
    12s:Confirm time 3s
    14s:Confirm time 10s
    16s:Confirm time 10s
    */
    SPO2Module_Send(COM_SPO2TIME_SPO2,avg,0,0,0,0,0); 
}

void SPO2Modue_SendSensitivity(unsigned char Sensitivity,unsigned char FastSAT)
{
    char alg0 = 0,alg1 = 0,alg2 = 0;
    switch(Sensitivity)//灵敏度触控选择
    {
    case 0:
        alg0 = 0;//最大灵敏度
        break;
    case 1:
        alg0 |= 0x01;//Normal
        break;
    case 2:
        alg0 |=0x04;//APOD
    default:
        //  alg0 |= 0x01;//默认为正常灵敏度
        break; 
    }
    //  char alg1;
    
    //  alg1 = 0;//???? TBD
    switch(FastSAT)//Fast SAT 选择
    {
    case 0://FastSetOFF
        alg1 = 0;
        break;
    case 1://FastSetON
        alg1 |= 0x02;
        break;
    default:
        //   alg1 = 0;//FastSetOFF
        break; 
    }  
    alg2 = alg0 + alg1;//???? TBD
    SPO2Module_Send(COM_ALGRITHOMMODE_SPO2,0,alg2,0,0,0,0);
}

void SPO2Module_SendSmartToneONOrOff(unsigned char SmartToneON)
{
    if(!SmartToneON)
    {
        SPO2Module_Send(COM_WAVECOFIG_SPO2,0x1c,0x3c,0x4c,0,0,0); //开机发送8位 Clipped AutoScaled IR、BEEP、SignalIQ指令
    }
    else
    {
        SPO2Module_Send(COM_WAVECOFIG_SPO2,0x1c,0x6c,0x5c,0,0,0); //开机发送8位 Clipped AutoScaled IR、BEEP smartTone On、SignalIQ SmartTone On指令
    }
    
}

/**********************************************The End*********************************************/

void SPO2ReceiveNormalMode_NormalSpeed(unsigned char RxData)
{
    static unsigned char NNCheck;
    unsigned char NND1_Temp = 0,NND2_Temp = 0; 
    switch(ReceiveStatus)
    {
    case NORMODMORSPEEDSOM:
        if(( RxData)==(0x02))
        {
            ReceiveStatus = NORMODMORSPEEDSEQ;
        }
        else
        {
            NNCheck = 0;
            ReceiveStatus = NORMODMORSPEEDSOM;
        }
        break;
    case NORMODMORSPEEDSEQ:
        if(((RxData)>=0)&&((RxData)<=62))
        {
            ReceiveStatus = NORMODMORSPEEDD1;
            NNSeq = RxData;
            NNCheck += NNSeq;
        }
        else
        {
            NNCheck = 0;
            ReceiveStatus = NORMODMORSPEEDSOM;
        }
        break;
    case NORMODMORSPEEDD1:
        //NND1 = RxData;
        NND1_Temp = RxData;
        //NNCheck += NND1;
        NNCheck += NND1_Temp;
        NND1 = NND1_Temp;
        ReceiveStatus = NORMODMORSPEEDD2;
        break;
    case NORMODMORSPEEDD2:
        //NND2 = RxData;
        NND2_Temp = RxData;
        //NNCheck += NND2;
        NNCheck += NND2_Temp;
        NND2 = NND2_Temp;
        ReceiveStatus = NORMODMORSPEEDW0;
        break;
    case NORMODMORSPEEDW0:
        NNW0 = RxData;
        NNCheck += NNW0;
        ReceiveStatus = NORMODMORSPEEDW1;
        break;
    case NORMODMORSPEEDW1:
        NNW1 = RxData;
        NNCheck += NNW1;
        ReceiveStatus = NORMODMORSPEEDW2;
        break;        
    case NORMODMORSPEEDW2:
        NNW2 = RxData;
        NNCheck += NNW2;
        ReceiveStatus = NORMODMORSPEEDW3;
        break;        
    case NORMODMORSPEEDW3:
        NNW3 = RxData;
        NNCheck += NNW3;
        ReceiveStatus = NORMODMORSPEEDW4;
        break;        
    case NORMODMORSPEEDW4:
        NNW4 = RxData;
        NNCheck += NNW4;
        ReceiveStatus = NORMODMORSPEEDW5;
        break;        
    case NORMODMORSPEEDW5:
        NNW5 = RxData;
        NNCheck += NNW5;
        ReceiveStatus = NORMODMORSPEEDW6;
        break;        
    case NORMODMORSPEEDW6:
        NNW6 = RxData;
        NNCheck += NNW6;
        ReceiveStatus = NORMODMORSPEEDW7;
        break;    
    case NORMODMORSPEEDW7:
        NNW7 = RxData;
        NNCheck += NNW7;
        ReceiveStatus = NORMODMORSPEEDCHK;
        break;  
    case NORMODMORSPEEDCHK:
        if(RxData==NNCheck)
        {
            NNCheck = 0;
            ReceiveStatus = NORMODMORSPEEDEOM; 
        }
        else
        {
            NNCheck = 0;
            ReceiveStatus = NORMODMORSPEEDSOM;
        }
        break;
    case NORMODMORSPEEDEOM:
        if(RxData==0x03)
        {
            SPO2NNCheckOK = 1;
            SPO2CurveReceOK = 1;
            HeartBeatFlag = 1;
            SPO2_RecvFinishFlag |= 0x01;
        }
        NNCheck = 0;
        ReceiveStatus = NORMODMORSPEEDSOM;
        break;
    default:
        NNCheck = 0;
        ReceiveStatus = NORMODMORSPEEDSOM;
        break;
    }
}

#define N 2//
unsigned char PulseRateData[N] = {0};
void GetD1D2SequenceData(void)
{
    //static unsigned char i1=0;
    unsigned char m,n,temp;
    static unsigned char i=0;

    switch(NNSeq)
    {
    case 0:
        //i1 = NND1<<8;
        //i2 = NND2;
        SpO2_Parameters.SpO2 = (NND1*256 + NND2)/10.0+0.5;//1.0-100%界面显示参数,加5 做四舍五入处理
        break;
    case 1:
#if 1
        PulseRateData[i++] = NND1*256  + NND2;//25-240bpm界面显示参数
        if(i >= N)//中值滤波
        {
            i = 0;
            if(PulseRateData[0] > PulseRateData[1])
            {
                temp = PulseRateData[0];
                PulseRateData[0] = PulseRateData[1];
                PulseRateData[1] = temp;
            }            
            SpO2_Parameters.PulseRate = PulseRateData[0];
        }
#else
        SpO2_Parameters.PulseRate =NND1*256  + NND2;
#endif
        break;
    case 2:
#if 1
        SpO2_Parameters.PerfusionIndex = (NND1*256 + NND2)/10.0+0.5;//0.002%-20.000%界面显示参数
#else
         SpO2_Parameters.PerfusionIndex = (NND1*256 + NND2);
#endif
        break;
    case 3:
        SpO2_Parameters.Exception1 = NND1*256  + NND2;//no use
        break;
    case 4:
        SpO2_Parameters.SensorType = NND1*256  + NND2;//no use
        break;
    case 5:
        SpO2_Parameters.WaveFormMode = NND1*256  + NND2;//no use
        break;    
    case 7:
        SpO2_Parameters.Exception2 = NND1*256  + NND2;//useful 提示详细报警信息
        break;    
    case 9:
        SpO2_Parameters.ProducID = NND1*256  + NND2;//no use
        break;    
    case 10:
        SpO2_Parameters.DSPFirmVer = NND1*256  + NND2;//useful
        break;    
    case 11:
        SpO2_Parameters.MCUFirmVer = NND1*256  + NND2;//useful
        break;    
    case 12:
        SpO2_Parameters.HardwareRev = NND1*256  + NND2;//useful
        break;    
    case 13:
        SpO2_Parameters.SensorFail = NND1*256  + NND2;//仅提示sensor fail 即可
        break;    
    case 14:
        SpO2_Parameters.BoardMode = NND1*256  + NND2;//no use
        break;    
    case 15:
        SpO2_Parameters.Wave0Config = NND1;//no use
        SpO2_Parameters.Wave1Config = NND2;//no use
        break;    
    case 16:
        SpO2_Parameters.Wave2Config = NND1;//no use
        SpO2_Parameters.Wave3Config = NND2;//no use
        break;    
    case 17:
        SpO2_Parameters.Wave4Config = NND1;//no use
        SpO2_Parameters.Wave5Config = NND2;//no use
        break;    
    case 18:
        SpO2_Parameters.SPO2AverTime = NND1*256  + NND2;//no use
        break;    
    case 19:
        SpO2_Parameters.AlgMode = NND1*256  + NND2;//no use
        break;    
    case 20:
        SpO2_Parameters.PVI = NND1*256  + NND2;//0-100%//no use
        break;    
    case 21:
        SpO2_Parameters.AlarmSpO2 = (NND1*256  + NND2)/10;//no use
        break;    
    case 61:
        SpO2_Parameters.DiagnoFailCode = NND1*256  + NND2;//useful
        break;    
    case 62:
        SpO2_Parameters.BoardFailCode = NND1*256  + NND2;//useful
        break;    
    default:
        SpO2_Parameters.Reserve = NND1*256  + NND2;//no use
        break;
    }
}

void GetWaveDataSmartToneOff(void)//波形数据SmartToneOff
{
    SPO2WaveIR = NNW0;
    SPO2WaveBeepSmartOff = NNW1;
    SPO2WaveSIQSmartOff = NNW2;   
}
void GetWaveDataSmartToneON(void)//波形数据SmartToneON
{
    if((signed char)(SPO2WaveSIQSmartOn) <= 0)
    {
        SPO2WaveIR  = NNW0;
        SPO2WaveBeepSmartOn = NNW1;
        SPO2WaveSIQSmartOn = NNW2;
    }
}

void BoardFailureCodes(void)
{
    switch( SpO2_Parameters.BoardFailCode)
    {
    case  0:
        SPO2FailureFlag &= ~(0x1);
        //No failure
        break;
    case 1:
        // ROM Boot Application Undefined Fault Condition
        // break;    
    case 2:
        //ROM Boot Application Reset Error
        // break;
    case 3:
        //ROM Boot Application An invalid Flash Signature exists for the application to be loaded
        //break;
    case 4:
        //ROM Boot Application Invalid PLL Information Checksum
        //break;
    case 5:
        //ROM Boot Application Application to loaded has an invalid Encryption Mode
        // break;
    case 6:
        //ROM Boot Application CLKSEL information in invalid
        // break;
    case 7:
        //ROM Boot Application Internal address to load application into is invalid
        // break;
    case 8:
        //ROM Boot Application PLL configuration failed,Defective PLL
        // break;
    case 9:
        //ROM Boot Application EFuse Failed ,Defective EFuse
        //break;
    case 10:
        //ROM Boot Application Hardware initialization failed
        //break;
    case 11:
        //Boot Application Loader Invalid Flash Image Record Header Flag was parsed
        //break;
    case 12:
        //Boot Application Loader Invalid Flash Image Record Header Internal Address was parsed
        // break;
    case 13:
        //Boot Application Loader Invalid Flash Image Record CSUM
        //break;
    case 14:
        //Boot Application Loader Invalid Flash Image Record Length
        // break;
    case 15:
        //Boot Application Loader Invalid Flash Image Branch Address
        // break;
    case 16:
        //System Application Loader Invalid  Flash Image Record Header Flag was parsed
        //break;
    case 17:
        //System Application Loader Invalid Flash Image Record Header Internal Address was parsed
        //break;
    case 18:
        //System Application Loader Invaid Flash Image Record CSUM
        //break;
    case 19:
        //System Application Loader Invalid Flash Image Record Length
        //break;
    case 20:
        //System Application Loader Invalid Flash Image Branch Address
        //break;
    case 21:
        //Boot Application Un-supported flash device detected
        // break;
    case 22:
        //Boot Application Flash Signature is invalid or missing
        // break;        
    case 23:
        //Boot Application Boot Application invalid exit from main processing loop
        // break;        
    case 24:
        //Boot Application Invalid Flash Image Record Header Internal Address was parsed
        // break;        
    case 25:
        //Boot Application Invaid Flash Image Record CSUM
        // break;
    case 26:
        //Boot Application Invalid Flash Image Record Header Flag was parsed 
        // break;        
    case 27:
        //Boot Application Invalid Flash Image Record Length
        // break;        
    case 28:
        //Boot Application Invalid Flash Image Branch Address
        // break;        
    case 32:
        //DSP Checksum Failure
        // break;        
    case 33:
        //DSP Program Memory Test Failure
        // break;        
    case 34:
        //DSP Data Memory Test Failure
        // break;
    case 35:
        //DSP Detector ADC Interrupt Test Failure
        // break;
        //      case 36:
        //        //DSP MCU Intrpt Failure
        //       // break;
    case 37:
        //DSP Diag Queue Overrun
        // break;
    case 38:
        //DSP Hardware Status Failure
        // break;
    case 39:
        //DSP Raw Queue Overrun
        // break;
    case 40:
        //DSP Watch Dog Failure
        // break;
    case 41:
        //DSP Invalid MC State
        // break;
    case 63:
        SPO2FailureFlag |= 0x1;
        //Diagnostic Failure 
        break;  
    default:
        break;
    }
}

void DiagnoFailureCodes(void)
{
    switch(SpO2_Parameters.DiagnoFailCode)
    {
    case 0x0:
        SPO2FailureFlag &= ~(0x10);
        break;
    case 0x0001:
        //LED Ground
        // break;
    case 0x0002:
        // Reference Voltage
        // break;  
    case 0x0004:
        //Digital Voltage
        // break;
    case 0x0008:
        //DSP Voltage
        // break;      
    case 0x0010:
        //Analog Voltage
        // break;  
        /*
    case 0x0020:
        
        break;      
    case 0x0040:
        
        break;    
    case 0x0080:
        
        break;
    case 0x0100:
        
        break;  
    case 0x0200:
        
        break;
    case 0x0400:
        
        break;      
    case 0x0800:
        
        break;  
        */
    case 0x1000:
        //LED Current
        //break;      
    case 0x2000:
        //Analog Ground
        //break; 
    case 0x4000:
        //LED Drive Voltage
        //break;
    case 0x8000:
        SPO2FailureFlag |= 0x10;
        //Sensor ID
        break; 
    default:
        break;
    }
}

void Exception2Code(void)
{
    //提示No Sensor 
    if(SpO2_Parameters.Exception2&0x0001)
    {
        //Exception2Info.NoSensorConnected = 1;//在使用处应该清零。
        SPO2_NoSensorFlag &= ~SPO2_NoSensorCountFlag;//停止计数
    }
    else
    {
        //Exception2Info.NoSensorConnected = 0;//
        SPO2_NoSensorFlag |= SPO2_NoSensorCountFlag;//开始计数
    }
    // 提示Defective Sensor  
    if(SpO2_Parameters.Exception2&0x0002)
    {
        Exception2Info.DefectiveSensor = 1;//在使用处应该清零。
    }
    else
    {
        Exception2Info.DefectiveSensor = 0;//
    }
    // 提示Low Perfusion
    if(SpO2_Parameters.Exception2&0x0004)
    {
        Exception2Info.LowPerfusion = 1;//在使用处应该清零。
    }
    else
    {
        Exception2Info.LowPerfusion = 0;//      
    }
    //提示Pulse Search
    if(SpO2_Parameters.Exception2&0x0008)
    {
        Exception2Info.PulseSearch = 1;//在使用处应该清零。
    }
    else
    {
        Exception2Info.PulseSearch = 0;//
    }
    //提示Interference Detected
    if(SpO2_Parameters.Exception2&0x0010)
    {
        Exception2Info.InterferenceDetected = 1;//在使用处应该清零。
    }  
    else
    {
        Exception2Info.InterferenceDetected = 0;//
    }
    //提示Sensor Off
    if(SpO2_Parameters.Exception2&0x0020)//病人未连接
    {
        SPO2_NoPatientFlag &= ~SPO2_NoPatientCountFlag;//停止计数
    }    
    else//病人连接上
    {
        SPO2_NoPatientFlag |= SPO2_NoPatientCountFlag;//开始计数
    }
    //提示Too Much Ambient Light
    if(SpO2_Parameters.Exception2&0x0040)
    {
        Exception2Info.TooMuchAmbientLight = 1;//在使用处应该清零。
    }   
    else
    {
        Exception2Info.TooMuchAmbientLight = 0;//
    }
    //提示Unrecognized Sensor
    if(SpO2_Parameters.Exception2&0x0080)
    {
        Exception2Info.UnrecognizedSensor = 1;//在使用处应该清零。
    }  
    else
    {
        Exception2Info.UnrecognizedSensor = 0;//
    }
    //提示Low Signal IQ
    if(SpO2_Parameters.Exception2&0x0400)
    {
        Exception2Info.LowSignalIQ = 1;//在使用处应该清零。
    }  
    else
    {
        Exception2Info.LowSignalIQ = 0;//
    }
    // 无提示
    if(SpO2_Parameters.Exception2&0x0800)
    {
        Exception2Info.MasimoSETProcessingActive = 1;//在使用处应该清零。
    }  
    else
    {
        Exception2Info.MasimoSETProcessingActive = 0;//
    }
    //提示No Cable Connected
    if(SpO2_Parameters.Exception2&0x1000)
    {
        Exception2Info.NoCableConnected = 1;//在使用处应该清零。
    }  
    else
    {
        Exception2Info.NoCableConnected = 0;//
    }
    //提示No Adhesive Sensor Connected
    if(SpO2_Parameters.Exception2&0x2000)
    {
        Exception2Info.NoAdhesiveSensorConnected = 1;//在使用处应该清零。
    }  
    else
    {
        Exception2Info.NoAdhesiveSensorConnected = 0;//
    }
    // 无提示
    if(SpO2_Parameters.Exception2&0x4000)
    {
        Exception2Info.DemoMode = 1;//在使用处应该清零。
    }  
    else
    {
        Exception2Info.DemoMode = 0;//
    }
}

//显示格式DSP：Va.b.c.d.
void DSPFirmVersionCode(void)
{
    SpO2_Parameters.DSPFirmVersion = (SpO2_Parameters.DSPFirmVer>>12)&0x0f;//a
    SpO2_Parameters.DSPFirmVerMajor = (SpO2_Parameters.DSPFirmVer>>8)&0x0f; //b 
    SpO2_Parameters.DSPFirmVerMinor = (SpO2_Parameters.DSPFirmVer>>4)&0x0f; //c 
    SpO2_Parameters.DSPFirmVerPatch = SpO2_Parameters.DSPFirmVer&0x0f; //d
}
//显示格式MCU：Vw.x.y.z.
void MCUFirmVersionCode(void)
{
    SpO2_Parameters.MCUFirmVersion = (SpO2_Parameters.MCUFirmVer>>12)&0x0f;//w
    SpO2_Parameters.MCUFirmVerMajor = (SpO2_Parameters.MCUFirmVer>>8)&0x0f; //x 
    SpO2_Parameters.MCUFirmVerMinor= (SpO2_Parameters.MCUFirmVer>>4)&0x0f;//y
    SpO2_Parameters.MCUFirmVerPatch = SpO2_Parameters.MCUFirmVer&0x0f; //z 
}
void HardwareRevisionCode(void)
{
    SpO2_Parameters.HardwareRevision = (SpO2_Parameters.HardwareRev>>12)&0x0f;//no use
    SpO2_Parameters.HardwareRevMajor = (SpO2_Parameters.HardwareRev>>8)&0x0f;//no use
    SpO2_Parameters.HardwareRevMinor = (SpO2_Parameters.HardwareRev>>4)&0x0f; //no use
    SpO2_Parameters.HardwareRevPatch = SpO2_Parameters.HardwareRev&0x0f;  //no use
}
void SensorFailCode(void)
{
    switch(SpO2_Parameters.SensorFail)
    {
    case 0:
        SensorFailureFlag = 0;
        //No Failure
        break;
    case 1:
        //Opem LEDS
        //  break;  
    case 2:
        //Shorted LEDS
        //  break;
    case 3:
        //Shorted Detector
        //  break;      
    case 4:
        //Bad Sensor ID Offset
        // break;      
    case 5:
        //ProCal Failure
        // break;      
    case 6:
        //Memory Failure
        SensorFailureFlag = 1;
        //提示SensorFailure
        break;      
    default:
        break;
    }
}
