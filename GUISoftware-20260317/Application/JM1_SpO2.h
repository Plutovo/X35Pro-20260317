#ifndef __JM1_SPO2_H__
#define __JM1_SPO2_H__
typedef struct
{
    unsigned char NoSensor;
    //²¨ÐÎ
    unsigned char WaveData;
    //°ôÍ¼
    unsigned char NoPatient;
    ///´«¸ÐÆ÷´íÎó
    unsigned char SensorErr;

    unsigned char PulseRate;

    unsigned char SpO2;
    unsigned char HeartBeatFlag;
    unsigned int PerfusionIndex;//2

    //masimo 
    unsigned int Exception1;//3
    unsigned int SensorType;//4
    unsigned char WaveFormMode;//5
    unsigned int Exception2;//7
    unsigned int ProducID;//9
    unsigned int DSPFirmVer;//10
    unsigned int DSPFirmVersion;//10
    unsigned int DSPFirmVerPatch;//10
    unsigned int DSPFirmVerMajor;//10
    unsigned int DSPFirmVerMinor;//10
    unsigned int MCUFirmVer;//11
    unsigned int MCUFirmVersion;//11
    unsigned int MCUFirmVerPatch;//11
    unsigned int MCUFirmVerMajor;//11
    unsigned int MCUFirmVerMinor;//11
    unsigned int HardwareRev;//12
    unsigned int HardwareRevision;//12
    unsigned int HardwareRevPatch;//12
    unsigned int HardwareRevMajor;//12
    unsigned int HardwareRevMinor;//12
    unsigned int SensorFail;//13
    unsigned int BoardMode;//14

    unsigned int Wave0Config;//15
    unsigned int Wave0ConfigSource;//15
    unsigned int Wave0ConfigScale;//15
    unsigned int Wave0ConfigSize;//15

    unsigned int Wave1Config;//15
    unsigned int Wave1ConfigSource;//15
    unsigned int Wave1ConfigScale;//15
    unsigned int Wave1ConfigSize;//15

    unsigned int Wave2Config;//16
    unsigned int Wave2ConfigSource;//16
    unsigned int Wave2ConfigScale;//16
    unsigned int Wave2ConfigSize;//16

    unsigned int Wave3Config;//16
    unsigned int Wave3ConfigSource;//16
    unsigned int Wave3ConfigScale;//16
    unsigned int Wave3ConfigSize;//16

    unsigned int Wave4Config;//17
    unsigned int Wave4ConfigSource;//17
    unsigned int Wave4ConfigScale;//17
    unsigned int Wave4ConfigSize;//17

    unsigned int Wave5Config;//17
    unsigned int Wave5ConfigSource;//17
    unsigned int Wave5ConfigScale;//17
    unsigned int Wave5ConfigSize;//17 

    unsigned int SPO2AverTime;//18
    unsigned int AlgMode;//19
    unsigned int AlgModeMaxSensitivity;//19
    unsigned int AlgModeNormalSensitivity;//19
    unsigned int AlgModeFastSATOff;//19
    unsigned int AlgModeFastSATOn;//19
    unsigned int AlgModeAPOD;//19
    unsigned int PVI;//20
    unsigned int AlarmSpO2;//21
    unsigned int Reserve;//22-60
    unsigned int DiagnoFailCode;//61
    unsigned int BoardFailCode;//62     
} JM1_SPO2_PARAMETERS;
void SPO2_DataDecoding(JM1_SPO2_PARAMETERS *SpO2Para, unsigned char * pointer);
#endif
