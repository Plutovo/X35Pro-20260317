//
#ifndef __C002_CO2_H__
#define __C002_CO2_H__
//
//
typedef struct
{
    float RR;
    float insCO2;
    float etCO2;
    //单位为mmhg的监测数据 djq add 20170523增加永远为mmhg单位的EtCO2参数。
    float insCO2_mmhg;
    float etCO2_mmhg;
    //波形
    float WaveData;
    unsigned char SampleLineOcclusion;

} C002_CO2_PARAMETERS;
//
void CO2_ModuleDecoding(unsigned char * pointer,C002_CO2_PARAMETERS *CO2_Para);
//
#endif
