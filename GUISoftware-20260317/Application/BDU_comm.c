#include "BDU_comm.h"
#include "Static_Menu_Creat.h"

extern unsigned char EtCO2_Config_On; //0:关闭   1：开启
extern unsigned char EtCO2_KM7002_Config_On; //0:关闭   1：开启
extern unsigned char Masimo_EtCO2_Config_On; //0:关闭   1：开启
extern unsigned char MasimoISA_EtCO2_Config_On; //0:关闭   1：ISA	2:ISA+O2 
extern unsigned char Drager_EtCO2_Config_On; //0:关闭   1：开启麻醉气体    2：开启麻醉气体+顺磁氧 

static Power_Work_t PowerState = AC_WORK;
extern unsigned short  LedUpdateRegister;
extern const int IE_RatioTable[];
extern BREATH_SET_PARA BeathSettingPara;

MODULECOMM Module_Comm;

static int  Filter_bufP[Filter_NumP+1]; //递推平均滤波 压力数据缓存  
static int  Filter_bufF[Filter_NumF+1];  //递推平均滤波 流量数据缓存
static int  Filter_bufV[Filter_NumV+1];  //递推平均滤波 容积数据缓存

void BDU_Comm_Set(unsigned short data)
{
	Module_Comm.BDU_CommFlag = data;
}
void GAS_Comm_Set(unsigned short data)
{
	Module_Comm.GAS_CommFlag = data;
}

unsigned short BDU_Comm_Get(void)
{
	return Module_Comm.BDU_CommFlag;
}
unsigned short GAS_Comm_Get(void)
{
	return Module_Comm.GAS_CommFlag;
}
unsigned char BDU_CommFault_Get(void)
{
	unsigned char BDU_Byte;

	BDU_Byte = (BDU_Comm_Get()&(1<<15)) >> 15;
	return BDU_Byte;//1:fail 0:no fail
}
unsigned char GAS_CommFault_Get(void)
{
	unsigned char GAS_Byte;

	GAS_Byte = (GAS_Comm_Get()&(1<<15)) >> 15;
	return GAS_Byte;//1:fail 0:no fail
}
void Check_CommFault(void)//通讯超时函数
{
	if((Module_Comm.BDU_CommFlag&(~(1<<15))) >= 15000)
	{
		Module_Comm.BDU_CommFlag = 1<<15;
	}
	else
	{
		Module_Comm.BDU_CommFlag++;
	}

	if(EtCO2_KM7002_Config_On || EtCO2_Config_On \
		|| Masimo_EtCO2_Config_On || Drager_EtCO2_Config_On \
		|| MasimoISA_EtCO2_Config_On)
	{
		if((Module_Comm.GAS_CommFlag&(~(1<<15))) >= 15000)
		{
			Module_Comm.GAS_CommFlag = 1<<15;
		}
		else
		{
			Module_Comm.GAS_CommFlag++;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
void Power_ChkState(void)
{
	static unsigned char cnt_bat = 0, cnt_ac = 0;
#ifndef NEW_POWER_COMM
	if(CheckAcStatus() == FALSE)
	{
		cnt_ac = 0;
		cnt_bat++;
		if(cnt_bat >= 1)
		{
			cnt_bat = 0;
			if(PowerState == AC_WORK)
			{
				PowerState = BAT_WORK;
				//send message to open the battery led
				LedUpdateRegister |= BATTERY_RUN_LED;
				SendOperationMessage(LED_UPDATE_OPERATION,USART3);
			}
		}
	}
	else
	{
		cnt_bat = 0;
		cnt_ac++;
		if(cnt_ac >= 1)
		{
			cnt_ac = 0;
			if(PowerState == BAT_WORK)
			{
				//send message to close battery led
				LedUpdateRegister &= ~BATTERY_RUN_LED;
				SendOperationMessage(LED_UPDATE_OPERATION,USART3);
				PowerState = AC_WORK;
			}
		}
	}
#else
    if(CheckAcStatus() == FALSE)
    {
			if(PowerState == AC_WORK)
			{
				PowerState = BAT_WORK;
				//send message to open the battery led
				LedUpdateRegister |= BATTERY_RUN_LED;
				SendOperationMessage(LED_UPDATE_OPERATION,USART3);
			}
    }
    else
    {
			if(PowerState == BAT_WORK)
			{
				PowerState = AC_WORK;
				//send message to close battery led
				LedUpdateRegister &= ~BATTERY_RUN_LED;
				SendOperationMessage(LED_UPDATE_OPERATION,USART3);
			}
    }
#endif
}

Power_Work_t Power_GetState(void)
{
	return PowerState;
}
/*
*函数功能：对除法结果对第几位进行精确四舍五入
*输入参数：float data, 被除数
			int narrow_mul,除数
			unsigned char DecDig要进行四舍五入的小数位数
*返回值：
*/
float Data_Rounding(float data, int narrow_mul,unsigned char DecDig)
{
    float tmp_data = data;
    float tmp_narrow_mul = 1.0*narrow_mul;
    int mul = (int)pow(10, DecDig);

    if(narrow_mul != 0)
    {
        tmp_data = tmp_data/tmp_narrow_mul;
    }
    
    if(DecDig != 0)
    {
        tmp_data = 1.0*mul*tmp_data + 0.5;
        tmp_data = (int)(tmp_data)/(1.0*mul);
    }
    
    return tmp_data;
}

unsigned int IE_readRatio(unsigned int idx)
{
    return IE_RatioTable[idx];
}

/*
	Filter_Pro()

描述：	递推平均滤波实现
功能：	对采样的压力，流量，容积数据继续滤波   

*/ 

int Filter_RecvAver(int data, int *buf, int num) //wff add 20190813
{
	int sum = 0;
    int f = 0;

	buf[num] = data;

	for(f=0;f<num;f++)
	{ 
		buf[f]=buf[f+1];
		sum += buf[f];	
	}		
	return Ceilc(sum,num);
}
/*
	Filter_Pro()

描述：	递推平均滤波调用接口
功能：	对采样的压力滤波   

*/ 
int Filter_RecvAverP(int data)
{
	return Filter_RecvAver(data, Filter_bufP, Filter_NumP);
}
/*
	Filter_Pro()

描述：	递推平均滤波调用接口
功能：	对采样的流量滤波   

*/ 
int Filter_RecvAverF(int data)
{
	int val=0;
	if((BeathSettingPara.BreathMode == VCV_MODE) || 
	   (BeathSettingPara.BreathMode == SIMV_VCV_MODE))
		
	{
		val = Filter_RecvAver(data, Filter_bufF, 5);    //2333
	}
	else
	{      
		val = Filter_RecvAver(data, Filter_bufF, Filter_NumF);
	}
    
    
    return val;
}
/*
	Filter_Pro()

描述：	递推平均滤波调用接口
功能：	对采样的容积数据滤波   

*/ 
int Filter_RecvAverV(int data)
{
	return Filter_RecvAver(data, Filter_bufV, Filter_NumV);
}
