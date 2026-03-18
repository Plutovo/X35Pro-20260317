//
#include "C002_CO2.h"
#include "math.h"
#include "MainTask.h"

//
extern int ETCO2Unit;
//extern unsigned char AA_Module_Flag.EtCO2ModuleExist;
extern unsigned char EtCO2ModuleOff; //etco2_switch;
extern unsigned char etco2pump_Occlusion;
//

extern unsigned char SpO2_Config_On;  //0:关闭   1：开启
extern unsigned char Masimo_SpO2_Config_On; //0:关闭   1：开启
extern unsigned char EtCO2_Config_On; //0:关闭   1：开启
extern unsigned char EtCO2_KM7002_Config_On;                          //金嘉信CO2
extern unsigned char Masimo_EtCO2_Config_On; //0:关闭   1：开启      //主流CO2
extern unsigned char MasimoISA_EtCO2_Config_On;                      //1 旁流多气体 2旁流CO2

static void CO2_ParaPacketDecode(unsigned char * pointer,C002_CO2_PARAMETERS *Para);
static float MotorolaFastFloatingPointConversion(int data);
//
void CO2_ModuleDecoding(unsigned char * pointer,C002_CO2_PARAMETERS *CO2_Para)
{
    int CHK = 0;
    int i = 0;    
    int data;

    switch(*pointer)
    {
    case 'w':
    case 'W':
        for(i=0;i<8;i++)
        {
            CHK += *(pointer+1+i);
        }
        if( (((unsigned char)(CHK&0xFF))^0xF0) == *(pointer+9) )
        {
            data = (*(pointer+1)<<24)|(*(pointer+2)<<16)|(*(pointer+3)<<8)|*(pointer+4);
            if((ETCO2Unit == 0)||(ETCO2Unit == 2))
            {
                CO2_Para->WaveData =  MotorolaFastFloatingPointConversion(data) * 0.133;
            }
            else
            {
                CO2_Para->WaveData = MotorolaFastFloatingPointConversion(data);
            }
        }
        break;
    case 'P':
        for(i=0;i<4;i++)
        {
            CHK += *(pointer+3+i);
        }
        if( (((unsigned char)(CHK&0xFF))^0xF0) == *(pointer+7) )
        {
            CO2_ParaPacketDecode(pointer,CO2_Para);
        }
        break;
    case 'U':
        break;
    case 'S':
        for(i=0;i<4;i++)
        {
            CHK += *(pointer+1+i);
        }
        if( (((unsigned char)(CHK&0xFF))^0xF0) == *(pointer+5) )
        {
            if(EtCO2_KM7002_Config_On)
            {
                if((EtCO2ModuleOff) 
                    && ((*(pointer + 2)) & 0x02)	//工作模式
                    && ((*(pointer + 4)) & 0x40))	//阻塞
                {
                    etco2pump_Occlusion = 1;
                }
                else
                {
                    etco2pump_Occlusion = 0;
                }                
            }
            else
            {
                if((EtCO2ModuleOff) 
                    && ((*(pointer + 2)) & 0x02)	//工作模式
                    && !((*(pointer + 1)) & 0x02)	//未开泵
                    && ((*(pointer + 4)) & 0x40))	//阻塞
                {
                    etco2pump_Occlusion = 1;
                }
                else
                {
                    etco2pump_Occlusion = 0;
                }
            }
        }
        break;
    }
}
/***************************************************************************************************
*  V=(-1)^S * M * 2^E   motorola 浮点型 按big endian顺序排列
*
*/
static float MotorolaFastFloatingPointConversion(int data)
{
    int Sign;
    int Mantissa;
    int Exponent;
    float temp;
    Mantissa = (data&0xffffff00)>>8; //小数位
    Sign = (data & 0x80)>>7; //符号位
    Exponent = data & 0x7F;  //指数位
    temp = pow(-1,Sign) * pow(2,(Exponent - 0x40)) * ((float)Mantissa/0x1000000);
    return temp;
}
/***************************************************************************************************
*
*  
*/
static void CO2_ParaPacketDecode(unsigned char * pointer,C002_CO2_PARAMETERS *Para)
{
    int data;
    switch(*(pointer+2))
    {
    case '0':
        data = (*(pointer+3)<<24)|(*(pointer+4)<<16)|(*(pointer+5)<<8)|*(pointer+6);
        Para->RR = MotorolaFastFloatingPointConversion(data);
        break;
    case '1':
        data = (*(pointer+3)<<24)|(*(pointer+4)<<16)|(*(pointer+5)<<8)|*(pointer+6);
        Para->insCO2 = MotorolaFastFloatingPointConversion(data);
        if((ETCO2Unit == 0)||(ETCO2Unit == 2))
        {
            Para->insCO2 = (Para->insCO2) * 1.33;
        }
        //djq add 20170523增加永远为mmhg单位的EtCO2参数。
        Para->insCO2_mmhg = MotorolaFastFloatingPointConversion(data);
        break;
    case '2':
        data = (*(pointer+3)<<24)|(*(pointer+4)<<16)|(*(pointer+5)<<8)|*(pointer+6);
        Para->etCO2 = MotorolaFastFloatingPointConversion(data);
        if((ETCO2Unit == 0)||(ETCO2Unit == 2))
        {
            Para->etCO2 =  (Para->etCO2) * 1.33;
        }
        //djq add 20170523增加永远为mmhg单位的EtCO2参数。
        Para->etCO2_mmhg = MotorolaFastFloatingPointConversion(data);        
        break;
    }
}

