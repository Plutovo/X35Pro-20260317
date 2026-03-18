
#include "JM1_SpO2.h"
extern unsigned char SpO2ModuleOff;

void SPO2_DataDecoding(JM1_SPO2_PARAMETERS *SpO2Para, unsigned char * pointer)
{
    if(!SpO2ModuleOff)
    {
        SpO2Para->NoPatient = (*pointer)&0x10;
        SpO2Para->NoSensor = (*pointer)&0x20;
        SpO2Para->HeartBeatFlag = (*pointer)&0x40;
        SpO2Para->WaveData = *(pointer+1)&0x7F;
        SpO2Para->SensorErr = *(pointer+2)&0x10;
        SpO2Para->PulseRate = (((*(pointer+2)&0x40)<<1)|(*(pointer+3)&0x7F));
        SpO2Para->SpO2 = *(pointer+4)&0x7F;
    }
    else
    {
        SpO2Para->HeartBeatFlag = 0;
        SpO2Para->WaveData = 0;
        SpO2Para->PulseRate = 0;
        SpO2Para->SpO2 = 0;
    }
}
