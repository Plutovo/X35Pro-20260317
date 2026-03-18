#include "filter.h"
#include "AA_Module.h"
#include "Alarm_handle.h"

extern unsigned char StandbyState;
extern MENU_HWIN Menu_hWin;
extern ALARM_SET_PARA AlarmSettingPara;
extern int MenuType;

static BATTERY_CONN Battery_Conn;
static AA_Range_t AA_Range = \
{AA_NO_AGT, AA_NO_AGT, PhaseIn_HAL_RANGE, PhaseIn_HAL_RANGE};

static u16 PhaseIn_AA_Range[5] = \
{
	PhaseIn_HAL_RANGE, \
	PhaseIn_ENF_RANGE, \
	PhaseIn_ISO_RANGE, \
	PhaseIn_SEV_RANGE, \
	PhaseIn_DES_RANGE, \
};

static u16 Drager_AA_Range[5] = \
{
	Drager_HAL_RANGE,	\
	Drager_ENF_RANGE,	\
	Drager_ISO_RANGE,	\
	Drager_SEV_RANGE,	\
	Drager_DES_RANGE,	\
};

static u16 AA_High_Limit[6] = \
{
	AA_HAL_HIGH_Limit,	\
	AA_HAL_HIGH_Limit,	\
	AA_ENF_HIGH_Limit,	\
	AA_ISO_HIGH_Limit,	\
	AA_SEV_HIGH_Limit,	\
	AA_DES_HIGH_Limit,	\
};

static u16 *pAA_Range = PhaseIn_AA_Range;

static Alarm_Chk_t Alarm_Chk;	//是否继续监测报警标志
//滤波数据buf
u8  CO2_Num_Fi = 0;
u16 CO2_ValueBuf_Fi[N] = {0};
u8  CO2_Num_Et = 0;
u16 CO2_ValueBuf_Et[N] = {0};

u8  NO2_Num_Fi = 0;
u16 NO2_ValueBuf_Fi[N] = {0};
u8  NO2_Num_Et = 0;
u16 NO2_ValueBuf_Et[N] = {0};

u8  AA1_Num_Fi = 0;
u16 AA1_ValueBuf_Fi[N] = {0};
u8  AA1_Num_Et = 0;
u16 AA1_ValueBuf_Et[N] = {0};

u8  O2_Num_Fi = 0;
u16 O2_ValueBuf_Fi[N] = {0};
u8  O2_Num_Et = 0;
u16 O2_ValueBuf_Et[N] = {0};


u16 filter(u16 *value_buf,u8 *num,u16 AA_Value)
{  
   u8 count;  
   u32  sum=0;  
   value_buf[(*num)++] = AA_Value;  
   if ( (*num) == N )
   {
       (*num) = 0;  
   }
   for ( count=0;count<N;count++)
   {
        sum += value_buf[count];  
   }
   return (u16)(sum/N);  
}
 
void Battery_Collect(void)
{
    if(Battery_Conn.Bat_Count % 10 == 0 && Battery_Conn.Bat_Count < 100)
    {
        Battery_Conn.Bat_Conn[Battery_Conn.Bat_Count/10] = CheckChargeStatus();
    }
    Battery_Conn.Bat_Count++;
}

void Battery_Check(void)
{
    Int8U i;
    for(i=0;i<10;i++)
    {
        if(Battery_Conn.Bat_Conn[i] == 1)
        {
            Battery_Conn.Bat_DisConnCount++;
        }
    }
    if(Battery_Conn.Bat_DisConnCount >= 5)  Battery_Conn.Bat_DisConnFlag = 1;
}

Int8U Battery_GetDisconnFlag(void)
{
    return Battery_Conn.Bat_DisConnFlag;
}

void Battery_SetDisconnFlag(Int8U flag)
{
    Battery_Conn.Bat_DisConnFlag = flag;
}
void AA_Range_Para(void* data)
{
	static enum AA_AGENT Last_AA_ID1, Last_AA_ID2;
	AA_ALL_DATA *pData;
	static char *str;

	if(data == NULL)
	{
		return;
	}

	pData = (AA_ALL_DATA*)data;	
	switch(pData->AA_state.dev_info.AA_name)
	{
	case PhaseIn:
	default:
		pAA_Range = PhaseIn_AA_Range;
		break;
	case Drager:
		pAA_Range = Drager_AA_Range;
		break;
	}

	if(Last_AA_ID1 != AA_Range.AA1_ID)
	{
		Last_AA_ID1 = AA_Range.AA1_ID;
		switch(AA_Range.AA1_ID)
		{
		case AA_ID_HAL:
			AA_Range.AA_Range = pAA_Range[0];
			AA_Range.AA1_StdRange = pAA_Range[0];
			str = (char*)Sys_Lib_GetDispStr(DP_HAL);
			break;
		case AA_ID_ENF:
			AA_Range.AA_Range = pAA_Range[1];
			AA_Range.AA1_StdRange = pAA_Range[1];
			str = (char*)Sys_Lib_GetDispStr(DP_ENF);
			break;
		case AA_ID_ISO:
			AA_Range.AA_Range = pAA_Range[2];
			AA_Range.AA1_StdRange = pAA_Range[2];
			str = (char*)Sys_Lib_GetDispStr(DP_ISO);
			break;
		case AA_ID_SEV:
			AA_Range.AA_Range = pAA_Range[3];
			AA_Range.AA1_StdRange = pAA_Range[3];
			str = (char*)Sys_Lib_GetDispStr(DP_SEV);
			break;
		case AA_ID_DES:
			AA_Range.AA_Range = pAA_Range[4];
			AA_Range.AA1_StdRange = pAA_Range[4];
			str = (char*)Sys_Lib_GetDispStr(DP_DES);
			break;
		default:
			AA_Range.AA_Range = pAA_Range[0];
			AA_Range.AA1_StdRange = pAA_Range[0];
			str = (char*)Sys_Lib_GetDispStr(DP_AA);
			break;
		}
		if(AA_Range.AA1_ID <= AA_ID_DES)
		{
			AlarmSettingPara.FiAAHighValue = AA_High_Limit[AA_Range.AA1_ID];
			AlarmSettingPara.EtAAHighValue = AA_High_Limit[AA_Range.AA1_ID];
			AlarmSettingPara.FiAALowValue = 0;
			AlarmSettingPara.EtAALowValue = 0;
		}
		else
		{
			AlarmSettingPara.FiAAHighValue = AA_High_Limit[0];
			AlarmSettingPara.EtAAHighValue = AA_High_Limit[0];
		}
		if((MenuType==ALARM_MENU) && WM_IsWindow(Menu_hWin.Alarm_Win.AlarmWin_Lim2))
		{
			WM_SendMessageNoPara(Menu_hWin.Alarm_Win.AlarmWin_Lim2,WM_INIT_DIALOG);
		}

		//AAID改变 消除报警
		if(AlarmsWork.presentAlarms&ALARM_FIAA_HIGH)
		{
			AlarmsWork.presentAlarms &= ~ALARM_FIAA_HIGH;
		}

		if(AlarmsWork.presentAlarms&ALARM_ETAA_HIGH)
		{
			AlarmsWork.presentAlarms &= ~ALARM_ETAA_HIGH;
		}

		if(AlarmsWork.presentAlarms1&ALARM_FIAA_LOW)
		{
			AlarmsWork.presentAlarms1 &= ~ALARM_FIAA_LOW;
		}

		if(AlarmsWork.presentAlarms1&ALARM_ETAA_LOW)
		{
			AlarmsWork.presentAlarms1 &= ~ALARM_ETAA_LOW;
		}
		Alarm_Chk.Alarm_Chk_EtAAHigh_Flag = 0;
		Alarm_Chk.Alarm_Chk_FiAAHigh_Flag = 0;
	}

	if(Last_AA_ID2 != AA_Range.AA2_ID)
	{
		Last_AA_ID2 = AA_Range.AA2_ID;
		switch(AA_Range.AA2_ID)
		{
		case AA_ID_HAL:
		default:
			AA_Range.AA2_StdRange = pAA_Range[0];
			break;
		case AA_ID_ENF:
			AA_Range.AA2_StdRange = pAA_Range[1];
			break;
		case AA_ID_ISO:
			AA_Range.AA2_StdRange = pAA_Range[2];
			break;
		case AA_ID_SEV:
			AA_Range.AA2_StdRange = pAA_Range[3];
			break;
		case AA_ID_DES:
			AA_Range.AA2_StdRange = pAA_Range[4];
			break;
		}
	}
	if(StandbyState == 0)
	{
		if(WM_IsWindow(Menu_hWin.HomeWin.ParentWin))
		{
			WM_SendMessageNoPara(Menu_hWin.HomeWin.ParentWin,WM_INIT_DIALOG);
		}
		if(WM_IsWindow(Menu_hWin.HomeWin.ParentWin))
		{
			if(Menu_hWin.cWin.WaveForms[1] == WAVEFORM_PAA)
			{
				TracingCurve_SetXYInfo(Menu_hWin.HomeWin.Tobj[1], GUI_WHITE, str);
			}
			if(Menu_hWin.cWin.WaveForms[2] == WAVEFORM_PAA)
			{
				TracingCurve_SetXYInfo(Menu_hWin.HomeWin.Tobj[2], GUI_WHITE, str);
			}
		}
	}
}




void AA_Range_AAID_Change(int *flag, void* data)
{
	static enum AA_AGENT Last_AA_ID1, Last_AA_ID2;
    AA_ALL_DATA *pData;

    if(data == NULL || flag == NULL)
    {
        return;
    }

	pData = (AA_ALL_DATA*)data;
	AA_Range.AA1_ID = pData->AA_detail.AA_AA1.ID; 
	AA_Range.AA2_ID = pData->AA_detail.AA_AA2.ID;    

	if(AA_Range.AA1_ID != Last_AA_ID1 \
		|| AA_Range.AA2_ID != Last_AA_ID2)
	{
		*flag |= AA_RANGE_CHANGE_FLAG;
		
		Last_AA_ID1 = AA_Range.AA1_ID;
		Last_AA_ID2 = AA_Range.AA2_ID;
	}
}

Int16U AA_Range_GetRange(void)
{
	return AA_Range.AA_Range;
}

Int16U AA_Range_GetAA1StdRange(void)
{
	return AA_Range.AA1_StdRange;
}

Int16U AA_Range_GetAA2StdRange(void)
{
	return AA_Range.AA2_StdRange;
}

u8 Alarm_Chk_GetFiAAFlag(void)
{
	return Alarm_Chk.Alarm_Chk_FiAAHigh_Flag;
}

u8 Alarm_Chk_GetEtAAFlag(void)
{
	return Alarm_Chk.Alarm_Chk_EtAAHigh_Flag;
}

void Alarm_Chk_SetFiAAFlag(u8 data)
{
	Alarm_Chk.Alarm_Chk_FiAAHigh_Flag = data;
}

void Alarm_Chk_SetEtAAFlag(u8 data)
{
	Alarm_Chk.Alarm_Chk_EtAAHigh_Flag = data;
}

