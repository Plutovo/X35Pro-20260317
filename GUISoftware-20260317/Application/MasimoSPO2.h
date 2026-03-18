/**************************************************************************************************
FileName :SPO2Module.h
Author:sy
Function:SPO2 Communicaition 
Data:21040611
***************************************************************************************************/
#ifndef _MASIMOSPO2_H
#define _MASIMOSPO2_H

#define SPO2_NoSensorCountFlag      (unsigned short)(1<<15)
#define SPO2_NoSensorClearCount     (unsigned short)(1<<15) 

#define SPO2_NoPatientCountFlag     (unsigned short)(1<<15)
#define SPO2_NoPatientClearCount    (unsigned short)(1<<15) 



void InitialiazeSPO2Varity(void);

//void SPO2Modue_SendBoardMode(void);
//void SPO2Modue_SendWaveConfig(void);
void SPO2Modue_SendAverageTime(unsigned char SPO2AverageTime);//触控设置波形平均时间时，调用该函数
//void SPO2Modue_SendAlgorithmMode(void);
//void SPO2Modue_SendModeConfig(void);
void GetD1D2SequenceData(void);

void SPO2ReceiveNormalMode_NormalSpeed(unsigned char RxData);
//void SPO2ReceiveNormalMode_HighSpeed(unsigned char RxData);
// void SPO2ReceiveEnhanceMode_NormalSpeed(unsigned char RxData);
// void SPO2ReceiveTestMode_NormalSpeed(unsigned char RxData);
// void SPO2ReceiveEnhanceMode_HighSpeed(unsigned char RxData);
// void SPO2ReceiveTestMode_HighSpeed(unsigned char RxData);

void SPO2Modue_SendSensitivity(unsigned char Sensitivity,unsigned char FastSAT);
//void SPO2Modue_SendFastSAT(void);
void BoardFailureCodes(void);
void DiagnoFailureCodes(void);
//void Exception1Code(void);
void Exception2Code(void);
void SensorTypeCode(void);
void WaveformModeCode(void);
//void ProductionIDCode(void);
void DSPFirmVersionCode(void);
void MCUFirmVersionCode(void);
void HardwareRevisionCode(void);
void SensorFailCode(void);
void BoardModCodee(void);
void Wave01ConfigCode(void);
void Wave23ConfigCode(void);
void Wave45ConfigCode(void);
void SPO2AverTimeCode(void);
void AlgModeCode(void);
void BoardParameterCodes(void);
void AvailableParameterCodes(void);
void PVIExceptionCodes(void);
void GetWaveDataSmartToneOff(void);
void GetWaveDataSmartToneON(void);
void SPO2Module_SendSmartToneONOrOff(unsigned char SmartToneON);
void SPO2Module_Send(char d1,char d2,char d3,char d4,char d5,char d6,char d7);//发送给模块的命令
void MSpO2_SendExpirationMsg3(unsigned short data);

//void PVICode(void);
//void AlarmSpO2Code(void);
extern char SPO2BoardMode;
extern char SPO2WaveSource;
extern char SPO2WaveScale;
extern char SPO2WaveSize;
extern char ModeConf[6]; //初始化应为零，！！！！！
extern char SPO2WaveTimer;//初始化应为零！！！！！！
extern char SPO2AlgorithmAPOD;//初始化应为0！！！！！！
extern unsigned char ReceiveStatus ;
extern char SPO2ModeConfig;//初始化应为零！！！！！！
extern char SPO2NNCheckOK;
extern char SPO2CurveReceOK;
extern char FailCode;
extern unsigned int NND1,NND2,NND3,NND4;//初始化应为零！！！！！！
extern unsigned int NNW0,NNW1,NNW2,NNW3,NNW4,NNW5,NNW6,NNW7,NNW8,NNW9;//初始化应为零！！！！！！
//extern char SPO2ModeChoose;//初始化为0
extern char SPO2WaveIR,NNSeq;
extern char SPO2WaveSIQSmartOff;
extern char SPO2WaveSIQSmartOn;
extern char SPO2WaveBeepSmartOff;
extern char SPO2WaveBeepSmartOn;
extern char SmartToneChangeFlag;
extern char FastSatChangeFlag;
extern char SensitivityChangeFlag;
extern char AverageTimeChangeFlag;
extern char SensorFailureFlag;
extern char SPO2FailureFlag;//给报警CodeFailure使用
extern char AlarmDelayTime;

//Masimo configure
extern unsigned char Masimo_BeeperSwitch;//0:off  1:on
extern unsigned char Masimo_FastSATSwitch;//0:off 1:on  //如果average time选择为前两个阶段，则FastSAT应该设置为开启
extern unsigned char Masimo_SmartToneSwitch;//0:off 1:on                 
extern unsigned char Masimo_Sensitivity;//0:Max 1:Normal 2:APOD
extern unsigned char Masimo_AverageTime;//0:2-4s  1:4-6s  2:8s  3:10s  4:12s  5:14s  6:16s

#define SOM_SPO2     0x02
#define EOM_SPO2     0x03
#define COM_BOARDNAME_SPO2     0x00
#define COM_WAVECOFIG_SPO2     0x01
#define COM_SPO2TIME_SPO2      0x02
#define COM_ALGRITHOMMODE_SPO2 0x03
#define COM_WAVEMODE_SPO2      0x0D
#define COM_Expiration3_SpO2    0x0F
extern enum
{
      NORMAL_SPO2 = 0,
      TEST_SPO2,
      ENHANCENORMAL,
};

extern enum
{
      OFF,
      RAWIR,
      RAWRD,
      BEEP,
      SIGNALIQ,
      SIGNALIQSMARTTONE,
      BEEPSMARTTONE,
};

extern enum
{
      NONE,
      BANDPASS,
      CONTINUOUSAUTOSCALE,
      CLIPAUTOSCALE,
};
extern enum
{
      BIT8,
      BIT16,
      BIT32,
};
extern enum
{
     SPO2 = 0,
     PULSERATE,
     PERFUSIONINDEX,
     EXCEPTION1,
     SENSORTYPE,
     WAVEMODE,
     EXCEPTION2,
     PRODUCTIONID,
     DSPHWVER,
     MCUHWVER,
     HWREV,
     SENSORFAIL,
     BOARDMODE,
     WAVECONFIG1,
     WAVECONFIG2,
     WAVECONFIG3,
     SPO2AVERT,
     ALGORTHMODE,
     PVI,
     ALARMSPO2,//21
     DIAGNOFAILCODE,//61
     BOARDFAILCODE,//62
};
extern enum 
{
    NORMODMORSPEEDSOM = 0,
    NORMODMORSPEEDSEQ,
    NORMODMORSPEEDD1,
    NORMODMORSPEEDD2,
    NORMODMORSPEEDD3,
    NORMODMORSPEEDD4,
    NORMODMORSPEEDW0,
    NORMODMORSPEEDW1,
    NORMODMORSPEEDW2,
    NORMODMORSPEEDW3,
    NORMODMORSPEEDW4,
    NORMODMORSPEEDW5,
    NORMODMORSPEEDW6,
    NORMODMORSPEEDW7,
    NORMODMORSPEEDW8,
    NORMODMORSPEEDW9,    
    NORMODMORSPEEDCHK,
    NORMODMORSPEEDEOM,


};
extern enum
{
      NORMALSPEEDNORMALMODE = 0,
      NORMALSPEEDENHANCEMODE,
      NORMALSPEEDTESTMODE,
      HIGHSPEEDNORMALMODE,
      HIGHSPEEDENHANCEMODE,
      HIGHSPEEDTESTMODE,

};

extern struct 
{
  char NoSensorConnected;//
  char DefectiveSensor;//
  char LowPerfusion;//
  char PulseSearch;//
  char InterferenceDetected;//
  char SensorOffPatient;//
  char TooMuchAmbientLight;//
  char UnrecognizedSensor;//
  char LowSignalIQ;//
  char MasimoSETProcessingActive;//
  char NoCableConnected;
  char NoAdhesiveSensorConnected;
  char DemoMode;
}Exception2Info;//给Exception2报警使用

#endif
/**********************************************The End*********************************************/
