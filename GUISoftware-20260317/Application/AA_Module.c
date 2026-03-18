/*  #include directives go here */
#include "AA_Module.h"
#include "string.h"

/*  #define directives go here  */
#define AA_Module_Name  AA_all_data.AA_state.dev_info.AA_name

/*  declarations of external variables go here  */
Int8U AA_RX_Buffer[40] = {0};
Int8U AA_TX_Buffer[20] = {0};
Int8U AA_TX_Len = 0;
unsigned char Drager_PORZFlag = 0x80;
enum AA_ModuleName AA_Module = None;

struct AA_TIMER AA_Timer = {0,0,0};

AA_ALL_DATA AA_all_data;
int zeroing_state = 0;
int spaning_state = 0;

extern struct DRAGER_RT_DATA Drager_RT_Data;
extern struct DRAGER_AA_DATA Drager_AA1_Data;
extern struct DRAGER_AA_DATA Drager_AA2_Data;
extern struct DRAGER_GAS_DATA Drager_O2_Data;
extern struct DRAGER_GAS_DATA Drager_CO2_Data;
extern struct DRAGER_GAS_DATA Drager_N2O_Data;
extern struct DRAGER_PRESSANDPUMP Drager_PressureAndPump;
extern struct DRAGER_RESPINFO Drager_RespirationInfo;

extern struct DRAGER_DEVICEINFO Drager_DeviceInfo;
extern struct DRAGER_MODULEFLAG DRAGER_FLAG;

extern IM_ALL_DATA      * pAll_Data;
extern unsigned char Drager_Message_Register[10];
extern DRAGER_ACK Drager_Ack;
extern unsigned char StandbyState;
//extern AA_MODULE_FLAG_REG AA_Module_Flag;
extern unsigned char Drager_EtCO2_Config_On;
struct AA_CMD_PARA AA_CMD_Para;
enum IM_FUNC_STS (*AA_RX_Receive)(Int8U nowbyte);
enum IM_FUNC_STS (*AA_RX_Process)(void);

extern Int16U  USART2_TXD_RIGSTER;
extern Int8U   USART2_TXD_Busy_FLAG;
/*
*************************************************************************
 * Function Name: AA_Init()
 * Parameters: void
 * Return: void
 *
 * Description: init the AA Module
 *
*************************************************************************
*/
enum IM_FUNC_STS Empty_Point(void)
{
    return IM_OK;
}
void AA_ModuleInit(enum AA_ModuleName aa_name)
{
    volatile unsigned char i, *p;
    p = (unsigned char *)(&AA_all_data);
    i = sizeof(AA_all_data);
    while(i--)
    {
        *p++ = 0;
    }
    if(aa_name == PhaseIn)
    {
        AA_RX_Receive = Irma_RX_Int;
        AA_RX_Process = Irma_RX_Process;
        AA_all_data.AA_state.dev_info.AA_name = PhaseIn;
        Irma_Host_Init();
    }
    else if(aa_name == Drager)
    {
        AA_RX_Receive = ReceiveDragerData;
        AA_RX_Process = Empty_Point;
        AA_all_data.AA_state.dev_info.AA_name = Drager;
        Init_Drager();
    }
}
/*
*************************************************************************
 * Function Name: AA_SendOpMode()
 * Parameters: enum AA_OPMODE mode
 * Return: enum IM_FUNC_STS
 *
 * Description: set operate mode
 *
 *************************************************************************
*/
enum IM_FUNC_STS AA_SendOpMode(enum AA_OPMODE mode)
{
    AA_TASK_REGISTER &= ~AA_MONITOR_SETOPMODE;
    AA_CMD_Para.CMD_OpMode = mode;
    if(AA_Module_Name == PhaseIn)
    {
		AA_TX_Len = PhaseIn_SetOpMode(mode-1,AA_TX_Buffer);

		return IM_OK;
    }
    else if(AA_Module_Name == Drager)
    {
        if(mode == AA_MODE_STANDBY)
        {
            AA_TX_Len = Drager_SetOpMode(DRAGER_OP_STANGDBY,AA_TX_Buffer);
            return IM_OK;
        }
        else if(mode == AA_MODE_MEASURE)
        {
            AA_TX_Len = Drager_SetOpMode(DRAGER_OP_MEASURE,AA_TX_Buffer);
            return IM_OK;
        }
        else
        {
            return IM_ERR;
        }
    }
    else
    {
        return IM_ERR;
    }
}
/*
*************************************************************************
 * Function Name: AA_SendAAGas()
 * Parameters:enum AA_GAS aax,enum  AA_AGENT id
 * Return: IM_FUNC_STS
 *
 * Description: set the AA gas
 *
 *************************************************************************
*/
enum IM_FUNC_STS AA_SendAAGas(enum AA_GAS aax,enum  AA_AGENT id)
{
    AA_TASK_REGISTER &= ~AA_MONITOR_SETAAGAS;
    AA_CMD_Para.CMD_SETAA_AAx = aax;
    AA_CMD_Para.CMD_SETAA_id = id;
    if(AA_Module_Name == PhaseIn)
    {
        return IM_ERR;
    }
    else if(AA_Module_Name == Drager)
    {
        if((aax == AA_GAS_AA1)&&(id != AA_NO_AGT))
        {
            AA_TX_Len = Drager_SelectAA(id,AA_TX_Buffer);
            return IM_OK;
        }
        else
        {
            return IM_ERR;
        }
    }
    else
    {
        return IM_ERR;
    }
}
/*
*************************************************************************
 * Function Name: AA_InitZero()
 * Parameters:void
 * Return: IM_FUNC_STS
 *
 * Description: init zero
 *
 *************************************************************************
*/
enum IM_FUNC_STS AA_InitZero(void)
{
    AA_TASK_REGISTER &= ~AA_MONITOR_INITZERO;
    if(AA_Module_Name == PhaseIn)
    {
        Irma_CreateNormZeroCalFrame(AA_TX_Buffer);
        AA_TX_Len = 5;
        return IM_OK;
    }
    else if(AA_Module_Name == Drager)
    {
        AA_TX_Len = Drager_InitZero(AA_TX_Buffer);
        return IM_OK;
    }
    else
    {
        return IM_ERR;
    }
}
/*
*************************************************************************
 * Function Name: AA_SendExPara()
 * Parameters:u16 valuemask , Int16U value
 * Return: IM_FUNC_STS
 *
 * Description: Set para 
 *
 *************************************************************************
*/
enum IM_FUNC_STS AA_SendExPara(enum AA_GAS valuemask,Int16U value)
{
    unsigned char value_phasein = 0;
    AA_TASK_REGISTER &= ~AA_MONITOR_SETEXPARA;
    AA_CMD_Para.CMD_SETPARA_valuemask = valuemask;
    AA_CMD_Para.CMD_SETPARA_value = value;
    if(AA_Module_Name == PhaseIn)
    {
        value_phasein = (unsigned char)value;
        if(valuemask == AA_GAS_O2)
        {
            Irma_CreateTxFrame(IM_SET_O2VOL,value_phasein,AA_TX_Buffer);
            AA_TX_Len = 5;
            return IM_OK;
        }
        else if(valuemask == AA_GAS_N2O)
        {
            Irma_CreateTxFrame(IM_SET_N2OVOL,value_phasein,AA_TX_Buffer);
            AA_TX_Len = 5;
            return IM_OK;
        }
        else
        {
            return IM_ERR;
        }
    }
    else if(AA_Module_Name == Drager)
    {
        if(valuemask == AA_GAS_O2)
        {
            AA_TX_Len = Drager_Accessexpara(0x0010,value,0x01,AA_TX_Buffer);
            return IM_OK;
        }
        else if(valuemask == AA_GAS_N2O)
        {
            AA_TX_Len = Drager_Accessexpara(0x0002,value,0x01,AA_TX_Buffer);
            return IM_OK;
        }
        else
        {
            return IM_ERR;
        }
    }
    else
    {
        return IM_ERR;
    }
}
/*
*************************************************************************
 * Function Name: AA_GetPatientData()
 * Parameters:void
 * Return: IM_FUNC_STS
 *
 * Description: Set PatientData
 *
 *************************************************************************
*/
enum IM_FUNC_STS AA_SetPatientData(void)
{
    AA_TASK_REGISTER &= ~AA_MONITOR_SETPATIENTDATA;
    if(AA_Module_Name == PhaseIn)
    {
        return IM_ERR;
    }
    else if(AA_Module_Name == Drager)
    {
        AA_TX_Len = Drager_GetPatientData(DRAGER_CO2ANDN2O|DRAGER_O2|DRAGER_PRESSUREANDPUMP|
                                          DRAGER_BREATHINFO|DRAGER_PARINFO|DRAGER_AA1|
                                          DRAGER_AA2|DRAGER_PARUNIT|DRAGER_MESSAGE
                                          ,DRAGER_RT_O2|DRAGER_RT_AA1|DRAGER_RT_AA2|
                                          DRAGER_RT_N2O|DRAGER_RT_CO2
                                          ,2//20*2ms
                                          ,AA_TX_Buffer);
        return IM_OK;
    }
    else
    {
        return IM_ERR;
    }
}
/*
*************************************************************************
 * Function Name: AA_SendParaUnit()
 * Parameters:mask,bit0：CO2，bit1：N2O，bit2：AA1，bit3：AA2，bit4：O2
 *            unit,0x00：ATS Vol,0x05：ATPS mmHg
 * Return: IM_FUNC_STS
 *
 * Description: Set ParaUnit
 *
 *************************************************************************
*/
enum IM_FUNC_STS AA_SendParaUnit(Int16U mask,Int8U unit)
{
    AA_TASK_REGISTER &= ~AA_MONITOR_SETUNIT;
    AA_CMD_Para.CMD_SETUNIT_mask = mask;
    AA_CMD_Para.CMD_SETUNIT_unit = unit;
    if(AA_Module_Name == PhaseIn)
    {
        return IM_ERR;
    }
    else if(AA_Module_Name == Drager)
    {
        AA_TX_Len = Drager_ChangeParaUnit(mask,unit,AA_TX_Buffer);
        return IM_OK;
    }
    else
    {
        return IM_ERR;
    }
}
/*
*************************************************************************
 * Function Name: AA_SendBreathDetection()
 * Parameters:void
 * Return: IM_FUNC_STS
 *
 * Description: Set Breath Detection
 *
 *************************************************************************
*/
enum IM_FUNC_STS AA_SendBreathDetection(enum AA_BREATHMODE mode)
{
    AA_TASK_REGISTER &= ~AA_MONITOR_SETBREATHDETECTION;
    AA_CMD_Para.CMD_SETBD_BDmode = mode;
    if(AA_Module_Name == PhaseIn)
    {
        return IM_ERR;
    }
    else if(AA_Module_Name == Drager)
    {
        AA_TX_Len = Drager_ChangeBreathMode(mode,AA_TX_Buffer);
        return IM_OK;
    }
    else
    {
        return IM_ERR;
    }
}
/*
*************************************************************************
 * Function Name: AA_SwitchValves()
 * Parameters:valve,DRAGER_VALVE_SAMPLE/DRAGER_VALVE_ROOM
 * Return: IM_FUNC_STS
 *
 * Description: Switch Valves
 *
 *************************************************************************
*/
enum IM_FUNC_STS AA_SwitchValves(Int8U valve)
{
    AA_TASK_REGISTER &= ~AA_MONITOR_SWITCHVALVES;
    AA_CMD_Para.CMD_SWITCH_valve = valve;
    if(AA_Module_Name == PhaseIn)
    {
        return IM_ERR;
    }
    else if(AA_Module_Name == Drager)
    {
        AA_TX_Len = Drager_SwitchValves(valve,AA_TX_Buffer);
        return IM_OK;
    }
    else
    {
        return IM_ERR;
    }
}
/*
*************************************************************************
 * Function Name: AA_SwitchPumps()
 * Parameters:pump,无流量：DRAGER_PUMP_OFF，高流量：DRAGER_PUMP_HIGH
 * Return: IM_FUNC_STS
 *
 * Description: Switch Pumps
 *
 *************************************************************************
*/
enum IM_FUNC_STS AA_SwitchPumps(enum AA_PUMP_STATE pump)
{
    AA_TASK_REGISTER &= ~AA_MONITOR_SWITCHPUMPS;
    //AA_CMD_Para.CMD_SWITCH_pump = pump;
    if(AA_Module_Name == PhaseIn)
    {
		if(pump == AA_pump_Off)
		{
			AA_TX_Len = PhaseIn_SetPump(PHASEIN_PUMP_OFF,AA_TX_Buffer);
		}
		else if(pump == AA_pump_high)
		{
			AA_TX_Len = PhaseIn_SetPump(PHASEIN_PUMP_ON,AA_TX_Buffer);
		}
		return IM_OK;
    }
    else if(AA_Module_Name == Drager)
    {
        if(pump == AA_pump_Off)
        {
          AA_TX_Len = Drager_SwitchPumps(DRAGER_PUMP_OFF,AA_TX_Buffer);
        }
        else if(pump == AA_pump_high)
        {
          AA_TX_Len = Drager_SwitchPumps(DRAGER_PUMP_HIGH,AA_TX_Buffer);
        }
        return IM_OK;
    }
    else
    {
        return IM_ERR;
    }
}

enum IM_FUNC_STS AA_SetIRO2Delay(Int8U value)
{
	AA_TASK_REGISTER &= ~AA_MONITOR_SETIRO2DELAY;
	//AA_CMD_Para.CMD_SWITCH_pump = pump;
	if(AA_Module_Name == PhaseIn)
	{
		AA_TX_Len = PhaseIn_SetIRO2Delay(value,AA_TX_Buffer);
		return IM_OK;
	}
	else if(AA_Module_Name == Drager)
	{
		return IM_ERR;
	}
	else
	{
		return IM_ERR;
	}
}

enum IM_FUNC_STS AA_O2Option(Int8U value)
{
	AA_TASK_REGISTER &= ~AA_MONITOR_SETO2OPTION;

	if(AA_Module_Name == PhaseIn)
	{
		AA_TX_Len = PhaseIn_SetO2Option(value,AA_TX_Buffer);
		return IM_OK;
	}
	else if(AA_Module_Name == Drager)
	{
		return IM_ERR;
	}
	else
	{
		return IM_ERR;
	}
}

enum IM_FUNC_STS AA_O2SensorType(Int8U type)
{
	AA_TASK_REGISTER &= ~AA_MONITOR_SETO2SENSORTYPE;

	if(AA_Module_Name == PhaseIn)
	{
		AA_TX_Len = PhaseIn_SetO2SensorType(type,AA_TX_Buffer);
		return IM_OK;
	}
	else if(AA_Module_Name == Drager)
	{
		return IM_ERR;
	}
	else
	{
		return IM_ERR;
	}
}

enum IM_FUNC_STS AA_GasSpanCal(GAS_SPANCAL_Type SpanCal,Int8U value)
{
	AA_TASK_REGISTER &= ~AA_MONITOR_SPANCAL;

	if(AA_Module_Name == PhaseIn)
	{
		AA_TX_Len = PhaseIn_SpanCal(SpanCal,value,AA_TX_Buffer);
		return IM_OK;
	}
	else if(AA_Module_Name == Drager)
	{
		return IM_ERR;
	}
	else
	{
		return IM_ERR;
	}
}

enum IM_FUNC_STS AA_InitSpan21(void)//氧浓度21%校验
{
    AA_TASK_REGISTER &= ~AA_MONITOR_SPANCAL21;
    if(AA_Module_Name == PhaseIn)
    {
        Irma_CreatePreSpanCalZeroFrame(AA_TX_Buffer);
        AA_TX_Len = 5;
        return IM_OK;
    }
    else
    {
        return IM_ERR;
    }
}

/*
*************************************************************************
 * Function Name: AA_StopContinueTrans()
 * Parameters:void
 * Return: IM_FUNC_STS
 *
 * Description: Stop Continue Trans
 *
 *************************************************************************
*/
enum IM_FUNC_STS AA_StopContinueTrans(void)
{
    AA_TASK_REGISTER &= ~AA_MONITOR_STOPCONTTRANS;
    if(AA_Module_Name == PhaseIn)
    {
        return IM_ERR;
    }
    else if(AA_Module_Name == Drager)
    {
        AA_TX_Len = Drager_StopContinueTrans(AA_TX_Buffer);
        return IM_OK;
    }
    else
    {
        return IM_ERR;
    }
}
/*
*************************************************************************
 * Function Name: AA_SendDevInfo()
 * Parameters:enum AA_DEVINFO_TYPE dev_info
 * Return: IM_FUNC_STS
 *
 * Description: 
 *
 *************************************************************************
*/
enum IM_FUNC_STS AA_SendDevInfo(enum AA_DEVINFO_TYPE dev_info)
{
    AA_TASK_REGISTER &= ~AA_MONITOR_GETDEVINFO;
    if(AA_Module_Name == PhaseIn)
    {
        return IM_ERR;
    }
    else if(AA_Module_Name == Drager)
    {
        if(dev_info == AA_DEV_vendor_code)
        {
            AA_TX_Len = Drager_GetDivInfo(0x00,AA_TX_Buffer);
        }
        else if(dev_info == AA_DEV_serial_num)
        {
            AA_TX_Len = Drager_GetDivInfo(0x01,AA_TX_Buffer);
        }
        else if(dev_info == AA_DEV_hw_ver)
        {
            AA_TX_Len = Drager_GetDivInfo(0x02,AA_TX_Buffer);
        }
        else if(dev_info == AA_DEV_sw_ver)
        {
            AA_TX_Len = Drager_GetDivInfo(0x03,AA_TX_Buffer);
        }
        else if(dev_info == AA_DEV_type_num)
        {
            AA_TX_Len = Drager_GetDivInfo(0x06,AA_TX_Buffer);
        }
        return IM_OK;
    }
    else
    {
        return IM_ERR;
    }
}
/*
*************************************************************************
 * Function Name: AA_SetOpMode()
 * Parameters:enum AA_OPMODE mode
 * Return: void
 *
 * Description: 
 * 
 *************************************************************************
*/
void AA_SetOpMode(enum AA_OPMODE mode)
{
    AA_CMD_Para.CMD_OpMode = mode;
    AA_TASK_REGISTER |= AA_MONITOR_SETOPMODE;
}

void AA_SetIRO2Value(Int8U Value)
{
	AA_CMD_Para.CMD_IRO2_DELAY = Value;
	AA_TASK_REGISTER |= AA_MONITOR_SETIRO2DELAY;
	SendOperationMessage(SET_AA_IRO2DELAY,USART2);
}

void AA_SetO2Option(Int8U state)
{
	AA_CMD_Para.CMD_SETO2_OPTION = state;
	AA_TASK_REGISTER |= AA_MONITOR_SETO2OPTION;
	SendOperationMessage(SET_AA_SETO2OPTION,USART2);
}

void AA_SetO2SensorType(O2SENSOR_Type O2Sensor)
{
	AA_CMD_Para.CMD_SETO2SENSOR_TYPE = O2Sensor;
	AA_TASK_REGISTER |= AA_MONITOR_SETO2SENSORTYPE;
	SendOperationMessage(SET_AA_SETO2SENSORTYPE,USART2);
}

void AA_SetSpanCalType(GAS_SPANCAL_Type SpanCal,Int8U Value)
{
	AA_CMD_Para.CMD_SPAN_CAL = SpanCal;
	AA_CMD_Para.CMD_SPAN_CALValue = Value;
	AA_TASK_REGISTER |= AA_MONITOR_SPANCAL;
	SendOperationMessage(SET_AA_SPANCAL,USART2);
}

void AA_SetSpanCal21Type(void)//氧浓度21%校验
{
	AA_TASK_REGISTER |= AA_MONITOR_SPANCAL21;
	SendOperationMessage(SET_AA_SPANCAL21,USART2);
}
/*
*************************************************************************
 * Function Name: AA_SetBreathDetection()
 * Parameters:enum AA_BREATHMODE mode
 * Return: void
 *
 * Description: 
 * 
 *************************************************************************
*/
void AA_SetBreathDetection(enum AA_BREATHMODE mode)
{
    AA_CMD_Para.CMD_SETBD_BDmode = mode;
    AA_TASK_REGISTER |= AA_MONITOR_SETBREATHDETECTION;
}
/*
*************************************************************************
 * Function Name: AA_SetAAGas()
 * Parameters:enum AA_GAS aax,enum  AA_AGENT id
 * Return: void
 *
 * Description: 
 * 
 *************************************************************************
*/
void AA_SetAAGas(enum AA_GAS aax,enum  AA_AGENT id)
{
    AA_CMD_Para.CMD_SETAA_AAx = aax;
    AA_CMD_Para.CMD_SETAA_id = id;
    AA_TASK_REGISTER |= AA_MONITOR_SETAAGAS;
}
/*
*************************************************************************
 * Function Name: AA_SetExPara()
 * Parameters:enum AA_GAS valuemask,Int16U value
 * Return: void
 *
 * Description: 
 * 
 *************************************************************************
*/
void AA_SetExPara(enum AA_GAS valuemask,Int16U value)
{
    AA_CMD_Para.CMD_SETPARA_valuemask = valuemask;
    AA_CMD_Para.CMD_SETPARA_value = value;
    AA_TASK_REGISTER |= AA_MONITOR_SETEXPARA;
}
/*
*************************************************************************
 * Function Name: AA_SetParaUnit()
 * Parameters:Int16U mask,Int8U unit
 * Return: void
 *
 * Description: 
 * 
 *************************************************************************
*/
void AA_SetParaUnit(Int16U mask,Int8U unit)
{
    AA_CMD_Para.CMD_SETUNIT_mask = mask;
    AA_CMD_Para.CMD_SETUNIT_unit = unit;
    AA_TASK_REGISTER |= AA_MONITOR_SETUNIT;
}
/*
*************************************************************************
 * Function Name: AA_SetParaUnit()
 * Parameters:Int16U mask,Int8U unit
 * Return: void
 *
 * Description: 
 * 
 *************************************************************************
*/
void AA_SetDevInfo(enum AA_DEVINFO_TYPE type)
{
    AA_CMD_Para.CMD_Dev_info = type;
    AA_TASK_REGISTER |= AA_MONITOR_GETDEVINFO;
}
/*
*************************************************************************
 * Function Name: AA_SetPumpMode()
 * Parameters:enum AA_OPMODE mode
 * Return: void
 *
 * Description: 
 * 
 *************************************************************************
*/
void AA_SetPumpMode(enum AA_PUMP_STATE mode)
{
    AA_CMD_Para.CMD_SWITCH_pump = mode;
    AA_TASK_REGISTER |= AA_MONITOR_SWITCHPUMPS;
}
/*
*************************************************************************
 * Function Name: AA_GetCMDFrame()
 * Parameters:enum AA_CMD cmd
 * Return: Int8U
 *
 * Description: 
 * txbuffer大小需不小于20
 *************************************************************************
*/
Int8U AA_GetCMDFrame(enum AA_CMD cmd,char* tx_buffer)
{
    int i = 0;
    AA_TX_Len = 0;
    switch(cmd)
    {
        case AA_CMD_SetOpMode:
            AA_SendOpMode(AA_CMD_Para.CMD_OpMode);
            break;
        case AA_CMD_AA_SetAA:
            AA_SendAAGas(AA_CMD_Para.CMD_SETAA_AAx,AA_CMD_Para.CMD_SETAA_id);
            break;
        case AA_CMD_InitZero:
            AA_InitZero();
            break;
        case AA_CMD_Setpara:
            AA_SendExPara(AA_CMD_Para.CMD_SETPARA_valuemask,AA_CMD_Para.CMD_SETPARA_value);
            break;
        case AA_CMD_SetPatientData:
            AA_SetPatientData();
            break;
        case AA_CMD_SetParaUnit:
            AA_SendParaUnit(AA_CMD_Para.CMD_SETUNIT_mask,AA_CMD_Para.CMD_SETUNIT_unit);
            break;
        case AA_CMD_SetBreathDetection:
            AA_SendBreathDetection(AA_CMD_Para.CMD_SETBD_BDmode);
            break;
        case AA_CMD_SwitchValves:
            AA_SwitchValves(AA_CMD_Para.CMD_SWITCH_valve);
            break;
        case AA_CMD_SwitchPumps:
            AA_SwitchPumps(AA_CMD_Para.CMD_SWITCH_pump);
            break;
        case AA_CMD_StopContinueTrans:
            AA_StopContinueTrans();
            break;
        case AA_CMD_GetDevInfo:
            AA_SendDevInfo(AA_CMD_Para.CMD_Dev_info);
            break;
		case AA_CMD_SetIRO2Delay:
			AA_SetIRO2Delay(AA_CMD_Para.CMD_IRO2_DELAY);
			break;
		case AA_CMD_SetO2Option:
			AA_O2Option(AA_CMD_Para.CMD_SETO2_OPTION);
			break;
		case AA_CMD_SetO2SensorType:
			AA_O2SensorType((Int8U)AA_CMD_Para.CMD_SETO2SENSOR_TYPE);
			break;
		case AA_CMD_SpanCal:
			AA_GasSpanCal(AA_CMD_Para.CMD_SPAN_CAL,AA_CMD_Para.CMD_SPAN_CALValue);//
			break;
        case AA_CMD_SpanCal21:
            AA_InitSpan21();//21%校验
            break;
		default:
			break;
    }
    if(AA_TX_Len > 0)
    {
        for(i = 0;i < 20;i ++)
        {
            *(tx_buffer+i) = AA_TX_Buffer[i];
        }
        return AA_TX_Len;
    }
    else
    {
        return 0;
    }
}

/*
*************************************************************************
 * Function Name: AA_GetWaveData()
 * Parameters:enum AA_GAS gastype
 * Return: IM_FUNC_STS
 *
 * Description: GetWaveData
 *
 *************************************************************************
*/
enum IM_FUNC_STS AA_GetWaveData(enum AA_GAS gastype)
{
    Int16U wavedata = 0;
    //int gas_index = gastype;
    //enum IM_GAS_DATA gas_phasein = (enum IM_GAS_DATA)gas_index;
    if(AA_Module_Name == PhaseIn)
    {
        switch(gastype)
        {
        case AA_GAS_CO2:
            wavedata = Im_getWaveData(IM_GAS_CO2);
// 			wavedata = wavedata * AA_all_data.AA_detail.AA_Pressure.atmp / 1013;
            AA_all_data.AA_fast.AA_RT_data.CO2value = wavedata;
            break;
        case AA_GAS_N2O:
            wavedata = Im_getWaveData(IM_GAS_N2O);
            AA_all_data.AA_fast.AA_RT_data.N2Ovalue = wavedata;
            break;
        case AA_GAS_AA1:
            wavedata = Im_getWaveData(IM_GAS_AX1);
            AA_all_data.AA_fast.AA_RT_data.AA1value = wavedata;
            AA_all_data.AA_fast.AA_RT_data.AA1_ID = (enum AA_AGENT)Im_getSlowData(IM_GET_AX1,IM_GAS_AX1);
            break;
        case AA_GAS_AA2:
            wavedata = Im_getWaveData(IM_GAS_AX2);
            AA_all_data.AA_fast.AA_RT_data.AA2value = wavedata;
            AA_all_data.AA_fast.AA_RT_data.AA2_ID = (enum AA_AGENT)Im_getSlowData(IM_GET_AX2,IM_GAS_AX2);
            break;
        case AA_GAS_O2:
            wavedata = Im_getWaveData(IM_GAS_O2);
            AA_all_data.AA_fast.AA_RT_data.O2value = wavedata;
            break;
        default:
            return IM_ERR;
        }
        return IM_OK;
    }
    else if(AA_Module_Name == Drager)
    {
        switch(gastype)
        {
        case AA_GAS_CO2:
            wavedata = Drager_RT_Data.CO2;
            AA_all_data.AA_fast.AA_RT_data.CO2value = wavedata;
            break;
        case AA_GAS_N2O:
            wavedata = Drager_RT_Data.N2O;
            AA_all_data.AA_fast.AA_RT_data.N2Ovalue = wavedata;
            break;
        case AA_GAS_AA1:
            wavedata = Drager_RT_Data.AA1;
            AA_all_data.AA_fast.AA_RT_data.AA1value = wavedata;
            AA_all_data.AA_fast.AA_RT_data.AA1_ID = (enum AA_AGENT)Drager_AA1_Data.PID;
            break;
        case AA_GAS_AA2:
            wavedata = Drager_RT_Data.AA2;
            AA_all_data.AA_fast.AA_RT_data.AA2value = wavedata;
            AA_all_data.AA_fast.AA_RT_data.AA2_ID = (enum AA_AGENT)Drager_AA2_Data.PID;
            break;
        case AA_GAS_O2:
            wavedata = Drager_RT_Data.O2;
            AA_all_data.AA_fast.AA_RT_data.O2value = wavedata;
            break;
        default:
            return IM_ERR;
        }
        return IM_OK;
    }
    else
    {
        return IM_ERR;
    }
}
/*
*************************************************************************
 * Function Name: AA_GetSlowData()
 * Parameters:void
 * Return: IM_FUNC_STS
 *
 * Description: Get Slow Data
 *
 *************************************************************************
*/
enum IM_FUNC_STS AA_GetSlowData(enum AA_DETAIL_PARA type)
{
    static u8 i;
    if(AA_Module_Name == PhaseIn)
    {
        switch(type)
        {
        case AA_DETAIL_CO2:
            AA_all_data.AA_detail.AA_CO2.FI = Im_getSlowData(IM_GET_INSP,IM_GAS_CO2);
            AA_all_data.AA_detail.AA_CO2.ET = Im_getSlowData(IM_GET_EXP,IM_GAS_CO2);
            AA_all_data.AA_detail.AA_CO2.Momt = Im_getSlowData(IM_GET_MOMT,IM_GAS_CO2);
            AA_all_data.AA_detail.AA_CO2.ParaState = PAS_None;
            AA_all_data.AA_detail.AA_CO2.ID = AA_ID_CO2;
            if(AA_all_data.AA_state.dev_info.module_type == AA_PHASEIN_UNKNOW)
            {
                AA_all_data.AA_detail.AA_CO2.ParaMode = PS_None;
                AA_all_data.AA_detail.AA_CO2.Accuracy = ACCURACY_None;
            }
            else
            {
                AA_all_data.AA_detail.AA_CO2.ParaMode = PS_Valid;
                AA_all_data.AA_detail.AA_CO2.Accuracy = ACCURACY_Iso;
            }
            AA_all_data.AA_detail.AA_CO2.ZeroState = ZSP_None;
            AA_all_data.AA_detail.AA_CO2.ZeroResult = ZSF_None; 
            AA_all_data.AA_detail.AA_CO2.Unit = PSUS_Default;
            break;
        case AA_DETAIL_N2O:
            AA_all_data.AA_detail.AA_N2O.FI = Im_getSlowData(IM_GET_INSP,IM_GAS_N2O);
            AA_all_data.AA_detail.AA_N2O.ET = Im_getSlowData(IM_GET_EXP,IM_GAS_N2O);
            AA_all_data.AA_detail.AA_N2O.Momt = Im_getSlowData(IM_GET_MOMT,IM_GAS_N2O);
            AA_all_data.AA_detail.AA_N2O.ParaState = PAS_None;
            AA_all_data.AA_detail.AA_N2O.ID = AA_ID_N2O;
            if((AA_all_data.AA_state.dev_info.module_type == AA_PHASEIN_UNKNOW)
               ||(AA_all_data.AA_state.dev_info.module_type == AA_PHASEIN_CO2ONLY)
                 ||(AA_all_data.AA_state.dev_info.module_type == AA_PHASEIN_ICU)
                 ||(AA_all_data.AA_state.dev_info.module_type == AA_PHASEIN_ISA_CO2))
            {
                AA_all_data.AA_detail.AA_CO2.ParaMode = PS_None;
                AA_all_data.AA_detail.AA_N2O.Accuracy = ACCURACY_None;
            }
            else
            {
                AA_all_data.AA_detail.AA_N2O.ParaMode = PS_Valid;
                AA_all_data.AA_detail.AA_N2O.Accuracy = ACCURACY_Iso;
            }
            AA_all_data.AA_detail.AA_N2O.ZeroState = ZSP_None;
            AA_all_data.AA_detail.AA_N2O.ZeroResult = ZSF_None;
            AA_all_data.AA_detail.AA_N2O.Unit = PSUS_Default;
            break;
        case AA_DETAIL_AA1:
            AA_all_data.AA_detail.AA_AA1.FI = Im_getSlowData(IM_GET_INSP,IM_GAS_AX1);
            AA_all_data.AA_detail.AA_AA1.ET = Im_getSlowData(IM_GET_EXP,IM_GAS_AX1);
            AA_all_data.AA_detail.AA_AA1.Momt = Im_getSlowData(IM_GET_MOMT,IM_GAS_AX1);
            AA_all_data.AA_detail.AA_AA1.ParaState = PAS_Identified;
            AA_all_data.AA_detail.AA_AA1.ID = (enum AA_GAS_ID)(Im_getSlowData(IM_GET_AX1,IM_GAS_AX1));
            if((AA_all_data.AA_state.dev_info.module_type == AA_PHASEIN_UNKNOW)
               ||(AA_all_data.AA_state.dev_info.module_type == AA_PHASEIN_ICU)
                  ||(AA_all_data.AA_state.dev_info.module_type == AA_PHASEIN_CO2ONLY)
                  ||(AA_all_data.AA_state.dev_info.module_type == AA_PHASEIN_ISA_CO2))
            {
                AA_all_data.AA_detail.AA_AA1.ParaMode = PS_None;
                AA_all_data.AA_detail.AA_AA1.Accuracy = ACCURACY_None;
            }
            else
            {
                AA_all_data.AA_detail.AA_AA1.ParaMode = PS_Valid;
                AA_all_data.AA_detail.AA_AA1.Accuracy = ACCURACY_Iso;
            }
            AA_all_data.AA_detail.AA_AA1.ZeroState = ZSP_None;
            AA_all_data.AA_detail.AA_AA1.ZeroResult = ZSF_None;
            AA_all_data.AA_detail.AA_AA1.Unit = PSUS_Default;
            break;
        case AA_DETAIL_AA2:
            AA_all_data.AA_detail.AA_AA2.FI = Im_getSlowData(IM_GET_INSP,IM_GAS_AX2);
            AA_all_data.AA_detail.AA_AA2.ET = Im_getSlowData(IM_GET_EXP,IM_GAS_AX2);
            AA_all_data.AA_detail.AA_AA2.Momt = Im_getSlowData(IM_GET_MOMT,IM_GAS_AX2);
            AA_all_data.AA_detail.AA_AA2.ParaState = PAS_Identified;
            AA_all_data.AA_detail.AA_AA2.ID = (enum AA_GAS_ID)(Im_getSlowData(IM_GET_AX2,IM_GAS_AX2));
            if((AA_all_data.AA_state.dev_info.module_type == AA_PHASEIN_UNKNOW)
               ||(AA_all_data.AA_state.dev_info.module_type == AA_PHASEIN_ICU)
                  ||(AA_all_data.AA_state.dev_info.module_type == AA_PHASEIN_CO2ONLY)
                  ||(AA_all_data.AA_state.dev_info.module_type == AA_PHASEIN_ISA_CO2))
            {
                AA_all_data.AA_detail.AA_AA2.ParaMode = PS_None;
                AA_all_data.AA_detail.AA_AA2.Accuracy = ACCURACY_None;
            }
            else
            {
                AA_all_data.AA_detail.AA_AA2.ParaMode = PS_Valid;
                AA_all_data.AA_detail.AA_AA2.Accuracy = ACCURACY_Iso;
            }
            AA_all_data.AA_detail.AA_AA2.ZeroState = ZSP_None;
            AA_all_data.AA_detail.AA_AA2.ZeroResult = ZSF_None;
            AA_all_data.AA_detail.AA_AA2.Unit = PSUS_Default;
            break;
        case AA_DETAIL_O2:
            AA_all_data.AA_detail.AA_O2.FI = Im_getSlowData(IM_GET_INSP,IM_GAS_O2);
            AA_all_data.AA_detail.AA_O2.ET = Im_getSlowData(IM_GET_EXP,IM_GAS_O2);
            AA_all_data.AA_detail.AA_O2.Momt = Im_getSlowData(IM_GET_MOMT,IM_GAS_O2);
            AA_all_data.AA_detail.AA_O2.ParaState = PAS_None;
            AA_all_data.AA_detail.AA_O2.ID = AA_ID_O2;
            if(    (AA_all_data.AA_state.dev_info.module_type == AA_PHASEIN_ORAUTO)
                 ||(AA_all_data.AA_state.dev_info.module_type == AA_PHASEIN_ICU)
                 ||(AA_all_data.AA_state.dev_info.module_type == AA_PHASEIN_OR)
                 ||(AA_all_data.AA_state.dev_info.module_type == AA_PHASEIN_ISA_AXAUTO)
                 ||(AA_all_data.AA_state.dev_info.module_type == AA_PHASEIN_ISA_AX)
				 ||(AA_all_data.AA_state.dev_info.module_type == AA_PHASEIN_ISA_ORAUTO))
            {
                AA_all_data.AA_detail.AA_O2.ParaMode = PS_Valid;
                AA_all_data.AA_detail.AA_O2.Accuracy = ACCURACY_Iso;
            }
            else
            {
                AA_all_data.AA_detail.AA_O2.ParaMode = PS_None;
                AA_all_data.AA_detail.AA_O2.Accuracy = ACCURACY_None;
            }
            AA_all_data.AA_detail.AA_O2.ZeroState = ZSP_None;
            AA_all_data.AA_detail.AA_O2.ZeroResult = ZSF_None;
            AA_all_data.AA_detail.AA_O2.Unit = PSUS_Default;
            break;
        case AA_DETAIL_COMPN:
            AA_all_data.AA_detail.AA_Compn.n2o_compn = Im_getSlowData(IM_GET_N2O_CMPEN,IM_GAS_N2O);
            AA_all_data.AA_detail.AA_Compn.o2_compn = Im_getSlowData(IM_GET_O2_CMPEN,IM_GAS_O2);
            break;
        case AA_DETAIL_PRESSURE:
            AA_all_data.AA_detail.AA_Pressure.atmp = Im_getSlowData(IM_GET_ATMP,IM_GAS_O2);//获取大气压力值
            AA_all_data.AA_detail.AA_Pressure.atmp_accuracy = ACCURACY_None;
            AA_all_data.AA_detail.AA_Pressure.atmp_cuve = Im_getSlowData(IM_GET_ATMP_CUVE,IM_GAS_O2);
            break;
        case AA_DETAIL_BREATH:
            AA_all_data.AA_detail.AA_BreathData.breath_rate = Im_getSlowData(IM_GET_BRE_RATE,IM_GAS_O2);
            AA_all_data.AA_detail.AA_BreathData.detection_enable = BREN_None;
            AA_all_data.AA_detail.AA_BreathData.lastbr_time = Im_getSlowData(IM_GET_LASTBRE_TM,IM_GAS_O2);
            AA_all_data.AA_detail.AA_BreathData.nobr_timeout = Im_getSlowData(IM_GET_NOBRE_TMOUT,IM_GAS_O2);
            AA_all_data.AA_detail.AA_BreathData.detection_mode = BR_mode_None;
            AA_all_data.AA_detail.AA_BreathData.apnea_open = APNEA_None;
            AA_all_data.AA_detail.AA_BreathData.apnea_duration = 0;
            AA_all_data.AA_detail.AA_BreathData.breath_phase = BRphase_None;
            break;
        default:
            return IM_ERR;
        }
        return IM_OK;
    }
    else if(AA_Module_Name == Drager)
    {
        switch(type)
        {
        case AA_DETAIL_CO2:
            AA_all_data.AA_detail.AA_CO2.FI = filter(CO2_ValueBuf_Fi,&CO2_Num_Fi,(u16)Drager_CO2_Data.FI);
            AA_all_data.AA_detail.AA_CO2.ET = filter(CO2_ValueBuf_Et,&CO2_Num_Et,(u16)Drager_CO2_Data.ET);
            
            AA_all_data.AA_detail.AA_CO2.Unit = (enum AA_UNIT)(Drager_CO2_Data.Unit);
            AA_all_data.AA_detail.AA_CO2.Momt = 0;
            AA_all_data.AA_detail.AA_CO2.ParaState = PAS_None;
            AA_all_data.AA_detail.AA_CO2.ID = AA_ID_CO2;
            AA_all_data.AA_detail.AA_CO2.ParaMode = (enum AA_PARA_MODE)(Drager_CO2_Data.ParaMode+1);
            AA_all_data.AA_detail.AA_CO2.ZeroState = (enum AA_ZEROSTATE)(Drager_CO2_Data.ZeroState+1);
            AA_all_data.AA_detail.AA_CO2.ZeroResult = (enum AA_ZERORESULT)(Drager_CO2_Data.ZeroResult+1);
            AA_all_data.AA_detail.AA_CO2.Accuracy = (enum AA_ACCURACY)(Drager_CO2_Data.Accuracy+1);
            
            break;
        case AA_DETAIL_N2O:
            AA_all_data.AA_detail.AA_N2O.FI = (Int16U)((float)(filter(NO2_ValueBuf_Fi,&NO2_Num_Fi,(u16)Drager_N2O_Data.FI))/100 + 0.5);//整数
            AA_all_data.AA_detail.AA_N2O.ET = (Int16U)((float)(filter(NO2_ValueBuf_Et,&NO2_Num_Et,(u16)Drager_N2O_Data.ET))/100 + 0.5);
            //AA_all_data.AA_detail.AA_N2O.FI = (Int16U)((float)Drager_N2O_Data.FI/100 + 0.5);//整数
            //AA_all_data.AA_detail.AA_N2O.ET = (Int16U)((float)Drager_N2O_Data.ET/100 + 0.5);
            
            AA_all_data.AA_detail.AA_N2O.Momt = 0;            
            AA_all_data.AA_detail.AA_N2O.ParaState = PAS_None;
            AA_all_data.AA_detail.AA_N2O.ID = AA_ID_N2O;
            AA_all_data.AA_detail.AA_N2O.ParaMode = (enum AA_PARA_MODE)(Drager_N2O_Data.ParaMode+1);
            AA_all_data.AA_detail.AA_N2O.ZeroState = (enum AA_ZEROSTATE)(Drager_N2O_Data.ZeroState+1);
            AA_all_data.AA_detail.AA_N2O.ZeroResult = (enum AA_ZERORESULT)(Drager_N2O_Data.ZeroResult+1);
            AA_all_data.AA_detail.AA_N2O.Accuracy = (enum AA_ACCURACY)(Drager_N2O_Data.Accuracy+1);
            AA_all_data.AA_detail.AA_N2O.Unit = (enum AA_UNIT)(Drager_N2O_Data.Unit);
            break;
        case AA_DETAIL_AA1:
            AA_all_data.AA_detail.AA_AA1.FI = (Int16U)((float)(filter(AA1_ValueBuf_Fi,&AA1_Num_Fi,(u16)Drager_AA1_Data.FI))/10 + 0.5);//一位小数
            AA_all_data.AA_detail.AA_AA1.ET = (Int16U)((float)(filter(AA1_ValueBuf_Et,&AA1_Num_Et,(u16)Drager_AA1_Data.ET))/10 + 0.5);
            //AA_all_data.AA_detail.AA_AA1.FI = (Int16U)((float)Drager_AA1_Data.FI/10 + 0.5);//一位小数
            //AA_all_data.AA_detail.AA_AA1.ET = (Int16U)((float)Drager_AA1_Data.ET/10 + 0.5);

            AA_all_data.AA_detail.AA_AA1.Momt = 0;
            AA_all_data.AA_detail.AA_AA1.ParaState = (enum AA_PARA_STATUS)(Drager_AA1_Data.PAS+1);
            AA_all_data.AA_detail.AA_AA1.ID = (enum AA_GAS_ID)(Drager_AA1_Data.PID);
            AA_all_data.AA_detail.AA_AA1.ParaMode = (enum AA_PARA_MODE)(Drager_AA1_Data.ParaMode+1);
            AA_all_data.AA_detail.AA_AA1.ZeroState = (enum AA_ZEROSTATE)(Drager_AA1_Data.ZeroState+1);
            AA_all_data.AA_detail.AA_AA1.ZeroResult = (enum AA_ZERORESULT)(Drager_AA1_Data.ZeroResult+1);
            AA_all_data.AA_detail.AA_AA1.Accuracy = (enum AA_ACCURACY)(Drager_AA1_Data.Accuracy+1);
            AA_all_data.AA_detail.AA_AA1.Unit = (enum AA_UNIT)(Drager_AA1_Data.Unit);
            break;
        case AA_DETAIL_AA2:
            AA_all_data.AA_detail.AA_AA2.FI = (Int16U)((float)Drager_AA2_Data.FI/10 + 0.5);//一位小数
            AA_all_data.AA_detail.AA_AA2.ET = (Int16U)((float)Drager_AA2_Data.ET/10 + 0.5);
            AA_all_data.AA_detail.AA_AA2.Momt = 0;
            AA_all_data.AA_detail.AA_AA2.ParaState = (enum AA_PARA_STATUS)(Drager_AA2_Data.PAS+1);
            AA_all_data.AA_detail.AA_AA2.ID = (enum AA_GAS_ID)(Drager_AA2_Data.PID);
            AA_all_data.AA_detail.AA_AA2.ParaMode = (enum AA_PARA_MODE)(Drager_AA2_Data.ParaMode+1);
            AA_all_data.AA_detail.AA_AA2.ZeroState = (enum AA_ZEROSTATE)(Drager_AA2_Data.ZeroState+1);
            AA_all_data.AA_detail.AA_AA2.ZeroResult = (enum AA_ZERORESULT)(Drager_AA2_Data.ZeroResult+1);
            AA_all_data.AA_detail.AA_AA2.Accuracy = (enum AA_ACCURACY)(Drager_AA2_Data.Accuracy+1);
            AA_all_data.AA_detail.AA_AA2.Unit = (enum AA_UNIT)(Drager_AA2_Data.Unit);
            break;
        case AA_DETAIL_O2:
            AA_all_data.AA_detail.AA_O2.FI = (Int16U)((float)(filter(O2_ValueBuf_Fi,&O2_Num_Fi,(u16)Drager_O2_Data.FI))/100 + 0.5);//整数
            //AA_all_data.AA_detail.AA_O2.ET = (Int16U)((float)(filter(O2_ValueBuf_Et,&O2_Num_Et,(u16)Drager_O2_Data.ET))/100 + 0.5);//无监测 不需要
            //AA_all_data.AA_detail.AA_O2.FI = (Int16U)((float)Drager_O2_Data.FI/100 + 0.5);//整数
            AA_all_data.AA_detail.AA_O2.ET = (Int16U)((float)Drager_O2_Data.ET/100 + 0.5);

            AA_all_data.AA_detail.AA_O2.Momt = 0;
            AA_all_data.AA_detail.AA_O2.ParaState = PAS_None;
            AA_all_data.AA_detail.AA_O2.ID = AA_ID_O2;
            AA_all_data.AA_detail.AA_O2.ParaMode = (enum AA_PARA_MODE)(Drager_O2_Data.ParaMode+1);
            AA_all_data.AA_detail.AA_O2.ZeroState = (enum AA_ZEROSTATE)(Drager_O2_Data.ZeroState+1);
            AA_all_data.AA_detail.AA_O2.ZeroResult = (enum AA_ZERORESULT)(Drager_O2_Data.ZeroResult+1);
            AA_all_data.AA_detail.AA_O2.Accuracy = (enum AA_ACCURACY)(Drager_O2_Data.Accuracy+1);
            AA_all_data.AA_detail.AA_O2.Unit = (enum AA_UNIT)(Drager_O2_Data.Unit);
            break;
        case AA_DETAIL_COMPN:
            return IM_ERR;
            break;
        case AA_DETAIL_PRESSURE:
            AA_all_data.AA_detail.AA_Pressure.atmp = (Int16U)((float)Drager_PressureAndPump.Pressure/10 + 0.5);//一位小数
            AA_all_data.AA_detail.AA_Pressure.atmp_accuracy = (enum AA_ACCURACY)(Drager_PressureAndPump.Pre_Accuracy+1);
            AA_all_data.AA_detail.AA_Pressure.atmp_cuve = 0;
            break;
        case AA_DETAIL_BREATH:
            AA_all_data.AA_detail.AA_BreathData.breath_rate = (Int16U)((float)Drager_RespirationInfo.RespirationRate/100 + 0.5);//整数
            AA_all_data.AA_detail.AA_BreathData.detection_mode = (enum AA_BREATHMODE)(Drager_RespirationInfo.BreathDetectionMode+1);
            if(AA_all_data.AA_detail.AA_BreathData.detection_mode == BR_mode_Valid)
            {
                AA_all_data.AA_detail.AA_BreathData.detection_enable = (enum AA_BREN)(DRAGER_FLAG.BreathAvail+1);
            }
            AA_all_data.AA_detail.AA_BreathData.lastbr_time = 0;
            AA_all_data.AA_detail.AA_BreathData.nobr_timeout = 0;
            if(AA_all_data.AA_detail.AA_BreathData.detection_mode == BR_mode_Auto_wake_up)
            {
                AA_all_data.AA_detail.AA_BreathData.apnea_open = (enum AA_APNEA)(DRAGER_FLAG.NorespOrApnea+1);
            }
            AA_all_data.AA_detail.AA_BreathData.apnea_duration = Drager_RespirationInfo.ApneaDuration;
            AA_all_data.AA_detail.AA_BreathData.breath_phase = (enum AA_BREATHPHASE)(DRAGER_FLAG.BreathPhase+1);
            break;
        default:
            return IM_ERR;
        }
        return IM_OK;
    }
    else
    {
        return IM_ERR;
    }
}
/*
*************************************************************************
 * Function Name: AA_GetModuleState()
 * Parameters:void
 * Return: IM_FUNC_STS
 *
 * Description: Get ModuleState
 *
 *************************************************************************
*/
enum IM_FUNC_STS AA_GetModuleState(void)
{
    int i = 0;
    char zero_state = 0;
	char span_state = 0;
    unsigned char mode = 0;
    long serialnum_temp = 0;
    unsigned int sw_temp = 0;
    unsigned int hw_temp = 0;
    unsigned int comm_temp = 0;
    unsigned int module_typenum = 0;
    if(AA_Module_Name == PhaseIn)
    {
        if(Irma_getVersion(&serialnum_temp,&sw_temp,&hw_temp,&comm_temp,&module_typenum)==IM_OK)
        {
            //AA_all_data.AA_state.dev_info.AA_name = PhaseIn;
            AA_all_data.AA_state.dev_info.serial_num[2] = serialnum_temp & 0xff;
            AA_all_data.AA_state.dev_info.serial_num[1] = (serialnum_temp>>8) & 0xff;
            AA_all_data.AA_state.dev_info.serial_num[0] = (serialnum_temp>>16) & 0xff;
            for(i = 0;i < 6;i ++)
            {
                AA_all_data.AA_state.dev_info.serial_num[i+3] = 0;
            }
            for(i = 0;i < 5;i ++)
            {
                AA_all_data.AA_state.dev_info.vendor_code[i] = 0;
            }
            AA_all_data.AA_state.dev_info.hw_version[0] = hw_temp & 0xff;
            AA_all_data.AA_state.dev_info.sw_version[1] = sw_temp & 0xff;
            AA_all_data.AA_state.dev_info.sw_version[0] = (sw_temp>>8) & 0xff;
            AA_all_data.AA_state.dev_info.module_typenum[1] = module_typenum & 0xff;
            AA_all_data.AA_state.dev_info.module_typenum[0] = (module_typenum>>8) & 0xff;
            if(!(module_typenum&0x8000)) //【主流类型(0=主流/1=旁流)】
            {
                if((AA_all_data.AA_state.dev_info.module_typenum[0]&0x01)==0x01) //AgentID option fitted
                {
                    //【主流红色模块(AX+/OR+)】
                    if((module_typenum&0xF8) == 0xF8) //麻醉气位有0的, 将返回UNKNOW
                    {
                        if(module_typenum&0x1) //O2 option fitted
                        {
                            AA_all_data.AA_state.dev_info.module_type = AA_PHASEIN_ORAUTO;
                        }
                        else              //No O2
                        {
                            AA_all_data.AA_state.dev_info.module_type = AA_PHASEIN_AXAUTO;
                        }
                        AA_Module_Flag.PhaseInTypeModuleExist = 1;//PhaseIn类型通讯协议 主流
                    }
					else
					{
						if(module_typenum&0x1) //O2 option fitted
						{
							AA_all_data.AA_state.dev_info.module_type = AA_PHASEIN_OR;
						}
						else              //No O2
						{
							AA_all_data.AA_state.dev_info.module_type = AA_PHASEIN_AX;
						}
						AA_Module_Flag.PhaseInTypeModuleExist = 1;//PhaseIn类型通讯协议 主流多
					}
                }
                else //【主流蓝色AX/OR 或 黄色模块CO2/ICU】
                {
                    //是否正确读出类型值?  2009-3-10 Modified
                    //(检测CO2_CFG位是否为1,1=正常,因所有IRMA必有CO2监测,0时返回UNKOWN)
                    if(module_typenum&0x2)
                    {
                        switch(module_typenum&0xFC) //default=UNKNOW
                        {
                        case 0x0: //无麻醉气监测(CO2/ICU)
                            if(module_typenum&0x1) //ICU (黄)(带O2)
                            {
                                AA_all_data.AA_state.dev_info.module_type = AA_PHASEIN_ICU;
                            }
                            else              //CO2 Only (黄)(不带O2)
                            {
                                AA_all_data.AA_state.dev_info.module_type = AA_PHASEIN_CO2ONLY;
                            }
                            AA_Module_Flag.PhaseInTypeModuleExist = 4;  //主流CO2
                            break;
                        case 0xFC:
                            if(module_typenum&0x1)
                            {
                                AA_all_data.AA_state.dev_info.module_type = AA_PHASEIN_OR;
                            }
                            else              //AX  (蓝)(不带O2)
                            {
                                AA_all_data.AA_state.dev_info.module_type = AA_PHASEIN_AX;
                            }
							AA_Module_Flag.PhaseInTypeModuleExist = 1;//PhaseIn类型通讯协议 主流多
                            break;
                        }
                    }
                }
            }
            else
            {
                if((AA_all_data.AA_state.dev_info.module_typenum[0]&0x01)==0x01) //带麻醉气体选件 ISA 
                {
                    if((module_typenum&0xFF) == 0xFF)//旁流ISA OR 带O2
                    {
                        AA_all_data.AA_state.dev_info.module_type = AA_PHASEIN_ISA_ORAUTO;
						AA_Module_Flag.PhaseInTypeModuleExist = 2;//PhaseIn类型通讯协议 旁流OR 2                      
                    }
                    else if((module_typenum&0xFF) == 0xFE)//旁流ISA AX 无O2
                    {
                        AA_all_data.AA_state.dev_info.module_type = AA_PHASEIN_ISA_AXAUTO;
						AA_Module_Flag.PhaseInTypeModuleExist = 3;//PhaseIn类型通讯协议 旁流AX 3                         
                    }
                    else //未知
                    {
                        AA_all_data.AA_state.dev_info.module_type = AA_PHASEIN_UNKNOW;
						AA_Module_Flag.PhaseInTypeModuleExist = 0;               
                    }
                }
                else
                {
                    if(module_typenum&0x2)
                    {
                        switch(module_typenum&0xFC) //default=UNKNOW
                        {
                        case 0x0:  //无麻醉气监测(CO2/ICU)
                            AA_all_data.AA_state.dev_info.module_type = AA_PHASEIN_ISA_CO2;
                            AA_Module_Flag.PhaseInTypeModuleExist = 6;
                            break;
                        case 0xFC://有麻醉气监测(AX/OR)
                            AA_all_data.AA_state.dev_info.module_type = AA_PHASEIN_ISA_AX;
							AA_Module_Flag.PhaseInTypeModuleExist = 3;
                            break;
                        }
                    }
                }
            }
            mode = Im_getSlowData(IM_GET_MODE,IM_GAS_O2);
            if(mode == 0)   //自检
            {
                AA_all_data.AA_state.AA_ModuleState.currect_mode = AA_MODE_SLFTST;
            }
            else if(mode == 1)  //睡眠
            {
                AA_all_data.AA_state.AA_ModuleState.currect_mode = AA_MODE_STANDBY;
            }
            else if(mode == 2)  //正常
            {
                AA_all_data.AA_state.AA_ModuleState.currect_mode = AA_MODE_MEASURE;
            }
            else if(mode == 3)  //菜单
            {
                AA_all_data.AA_state.AA_ModuleState.currect_mode = AA_MODE_DEMO;
            }
            else
            {
                AA_all_data.AA_state.AA_ModuleState.currect_mode = AA_MODE_NONE;
            }
            zero_state = Irma_getZeroState();
            if(zero_state == 1)
            {
                AA_all_data.AA_state.AA_ModuleState.zero_state = disable;
            }
            else if(zero_state == 2)
            {
                AA_all_data.AA_state.AA_ModuleState.zero_state = zero_zeroing;
            }
            else if(zero_state == 3)
            {
                AA_all_data.AA_state.AA_ModuleState.zero_state = zero_request;
            }
            else
            {
                AA_all_data.AA_state.AA_ModuleState.zero_state = zero_none;
            }
			span_state = Irma_getSpanState();
			if(span_state == 1)
			{
				AA_all_data.AA_state.AA_ModuleState.span_state = SPAN_InProg;
			}
			else
			{
				AA_all_data.AA_state.AA_ModuleState.span_state = SPAN_NoInProg;
			}
            return IM_OK;
        }
        else
        {
            return IM_BUSY;
        }
    }
    else if(AA_Module_Name == Drager)
    {
        //AA_all_data.AA_state.dev_info.AA_name = Drager;
        for(i = 0;i < 9;i ++)
        {
            AA_all_data.AA_state.dev_info.serial_num[i] = Drager_DeviceInfo.Drager_Serial_Num[i];
        }
        for(i = 0;i < 5;i ++)
        {
            AA_all_data.AA_state.dev_info.vendor_code[i] = Drager_DeviceInfo.Drager_Vendor_Code[i];
        }
        for(i = 0;i < 2;i ++)
        {
            AA_all_data.AA_state.dev_info.hw_version[i] = Drager_DeviceInfo.Drager_HandleVersion[i];
        }
        for(i = 0;i < 8;i ++)
        {
            AA_all_data.AA_state.dev_info.sw_version[i] = Drager_DeviceInfo.Drager_SoftwareVersion[i];
        }
        for(i = 0;i < 7;i ++)
        {
            AA_all_data.AA_state.dev_info.module_typenum[i] = Drager_DeviceInfo.Drager_PartNumber[i];
        }
        if(AA_all_data.AA_state.dev_info.module_typenum[5]==3)
        {
            if(AA_all_data.AA_state.dev_info.module_typenum[6]==0)
            {
                AA_all_data.AA_state.dev_info.module_type = AA_DRAGER_NONE;
            }
            else if(AA_all_data.AA_state.dev_info.module_typenum[6]==1)
            {
                AA_all_data.AA_state.dev_info.module_type = AA_DRAGER_O2_DUAL;
            }
            else if(AA_all_data.AA_state.dev_info.module_typenum[6]==2)
            {
                AA_all_data.AA_state.dev_info.module_type = AA_DRAGER_DUAL;
            }
            else if(AA_all_data.AA_state.dev_info.module_typenum[6]==3)
            {
                AA_all_data.AA_state.dev_info.module_type = AA_DRAGER_O2_SINGLE;
            }
            else if(AA_all_data.AA_state.dev_info.module_typenum[6]==4)
            {
                AA_all_data.AA_state.dev_info.module_type = AA_DRAGER_SINGLE;
            }
        }
        else
        {
            AA_all_data.AA_state.dev_info.module_type = AA_DRAGER_UNKNOW;
        }
        if(DRAGER_FLAG.OperateMode == Measure)
        {
            AA_all_data.AA_state.AA_ModuleState.currect_mode = AA_MODE_MEASURE;
        }
        else if(DRAGER_FLAG.OperateMode == Standby_Mode)
        {
            AA_all_data.AA_state.AA_ModuleState.currect_mode = AA_MODE_STANDBY;
        }
        else if(DRAGER_FLAG.OperateMode == ComponentsOff)
        {
            AA_all_data.AA_state.AA_ModuleState.currect_mode = AA_MODE_OFF;
        }
        else if(DRAGER_FLAG.OperateMode == TurnToMeasure)
        {
            AA_all_data.AA_state.AA_ModuleState.currect_mode = AA_MODE_CHANGETOMEASURE;
        }
        else if(DRAGER_FLAG.OperateMode == TurnToStandby)
        {
            AA_all_data.AA_state.AA_ModuleState.currect_mode = AA_MODE_CHANGETOSTANDBY;
        }
        else if(DRAGER_FLAG.OperateMode == ClosingComponents)
        {
            AA_all_data.AA_state.AA_ModuleState.currect_mode = AA_MODE_CLOSING;
        }
        else
        {
            AA_all_data.AA_state.AA_ModuleState.currect_mode = AA_MODE_NONE;
        }
        if((DRAGER_FLAG.ZeroRequest)&&(Drager_Message_Register[2]&0x04))
        {
            AA_all_data.AA_state.AA_ModuleState.zero_state = zero_request;
        }
        //else if((DRAGER_FLAG.ZeroState == 2)||(Drager_Message_Register[2]&0x10))
        else if(Drager_Message_Register[2]&0x10)
        {
            AA_all_data.AA_state.AA_ModuleState.zero_state = zero_ok;
            //DRAGER_FLAG.ZeroState = 0;
        }
        //else if((DRAGER_FLAG.ZeroState == 1)||(Drager_Message_Register[2]&0x20))
        else if(Drager_Message_Register[2]&0x20)
        {
            AA_all_data.AA_state.AA_ModuleState.zero_state = zero_zeroing;
            //DRAGER_FLAG.ZeroState = 0;
        }
        else if(Drager_Message_Register[2]&0x08)
        {
            AA_all_data.AA_state.AA_ModuleState.zero_state = zero_fail;
            //DRAGER_FLAG.ZeroState = 0;
        }
        else
        {
            if(AA_Timer.timer_min)
            {
                AA_all_data.AA_state.AA_ModuleState.zero_state = zero_ok;
            }
            else
            {
                AA_all_data.AA_state.AA_ModuleState.zero_state = zero_none;
            }
        }
        if(Drager_PressureAndPump.ValvesState == 0)
        {
            AA_all_data.AA_state.AA_ModuleState.valve_state = AA_valve_sample_gas;
        }
        else if(Drager_PressureAndPump.ValvesState == 3)
        {
            AA_all_data.AA_state.AA_ModuleState.valve_state = AA_valve_room;
        }
        if(Drager_PressureAndPump.PumpFlowState == 0)
        {
            AA_all_data.AA_state.AA_ModuleState.pump_state = AA_pump_Off;
        }
        else if(Drager_PressureAndPump.PumpFlowState == 2)
        {
            AA_all_data.AA_state.AA_ModuleState.pump_state = AA_pump_high;
        }
        else if(Drager_PressureAndPump.PumpFlowState == 3)
        {
            AA_all_data.AA_state.AA_ModuleState.pump_state = AA_pump_purge;
        }
		AA_all_data.AA_state.AA_ModuleState.span_state = SPAN_NoInProg;
        return IM_OK;
    }
    else
    {
        return IM_ERR;
    }
}
/*
*************************************************************************
 * Function Name: AA_Err_Message_Progress()
 * Parameters:void
 * Return:void
 *
 * Description:
 *
 *************************************************************************
*/
void AA_Err_Message_Progress(void)
{
    Int16U Err_Num = 0;
    if(AA_Module_Name == PhaseIn)   //13个
    {
        if(pAll_Data->slow.GenVal.zero_or > 0)  //零点错误
        {
            AA_all_data.AA_state.module_err_message[0] |= 0x01;
            Err_Num++;
        }
        else
        {
            AA_all_data.AA_state.module_err_message[0] &= ~0x01;
        }
        if(pAll_Data->slow.GenVal.un_cal)   //出厂校准缺失
        {
            AA_all_data.AA_state.module_err_message[0] |= 0x02;
            Err_Num++;
        }
        else
        {
            AA_all_data.AA_state.module_err_message[0] &= ~0x02;
        }
        if(pAll_Data->slow.GenVal.sw_err)   //软件版本错误，重启传感器
        {
            AA_all_data.AA_state.module_err_message[0] |= 0x04;
            //Err_Num++;
        }
        else
        {
            AA_all_data.AA_state.module_err_message[0] &= ~0x04;
        }
        if(pAll_Data->slow.GenVal.temp_or)   //内部温度高
        {
            AA_all_data.AA_state.module_err_message[0] |= 0x08;
            Err_Num++;
        }
        else
        {
            AA_all_data.AA_state.module_err_message[0] &= ~0x08;
        }
        if(pAll_Data->slow.GenVal.hW_err)   //硬件版本错误，更换传感器
        {
            AA_all_data.AA_state.module_err_message[0] |= 0x10;
            Err_Num++;
        }
        else
        {
            AA_all_data.AA_state.module_err_message[0] &= ~0x10;
        }
        if(pAll_Data->slow.GenVal.m_fail)   //电机速度超限
        {
            AA_all_data.AA_state.module_err_message[0] |= 0x20;
            Err_Num++;
        }
        else
        {
            AA_all_data.AA_state.module_err_message[0] &= ~0x20;
        }
        if(pAll_Data->slow.GenVal.repl_adapt)   //更换适配器
        {
            AA_all_data.AA_state.module_err_message[0] |= 0x40;
            Err_Num++;
        }
        else
        {
            AA_all_data.AA_state.module_err_message[0] &= ~0x40;
        }
        if(pAll_Data->slow.GenVal.no_adapt)   //无适配器，请连接适配器
        {
            AA_all_data.AA_state.module_err_message[0] |= 0x80;
            Err_Num++;
        }
        else
        {
            AA_all_data.AA_state.module_err_message[0] &= ~0x80;
        }

        if(pAll_Data->slow.GenVal.o2sen_err)//O2传感器错误
        {
            AA_all_data.AA_state.module_err_message[1] |= 0x01;
            Err_Num++;
        }
        else
        {
            AA_all_data.AA_state.module_err_message[1] &= ~0x01;
        }

        if(pAll_Data->slow.GenVal.span_err)//校准错误
        {
            AA_all_data.AA_state.module_err_message[1] |= 0x02;
            Err_Num++;
        }
        else
        {
            AA_all_data.AA_state.module_err_message[1] &= ~0x02;
        }

		if(pAll_Data->slow.GenVal.O2_Calib)//氧气量程校准
		{
			AA_all_data.AA_state.module_err_message[1] |= 0x04;
			Err_Num++;
		}
		else
		{
            AA_all_data.AA_state.module_err_message[1] &= ~0x04;
		}

        if(pAll_Data->slow.GenVal.co2_or)
        {
            AA_all_data.AA_state.module_err_message[4] |= 0x01;
            Err_Num++;
        }
        else
        {
            AA_all_data.AA_state.module_err_message[4] &= ~0x01;
        }
        if(pAll_Data->slow.GenVal.n2o_or)   //一氧化二氮超差
        {
            AA_all_data.AA_state.module_err_message[4] |= 0x02;
            Err_Num++;
        }
        else
        {
            AA_all_data.AA_state.module_err_message[4] &= ~0x02;
        }
        if(pAll_Data->slow.GenVal.ax_or)   //至少一种麻醉气体超差
        {
            AA_all_data.AA_state.module_err_message[4] |= 0x04;
            Err_Num++;
        }
        else
        {
            AA_all_data.AA_state.module_err_message[4] &= ~0x04;
        }
        if(pAll_Data->slow.GenVal.press_or)   //大气压超出运行范围
        {
            AA_all_data.AA_state.module_err_message[4] |= 0x08;
            Err_Num++;
        }
        else
        {
            AA_all_data.AA_state.module_err_message[4] &= ~0x08;
        }
        if(pAll_Data->slow.GenVal.agen_id_unreliable)   //麻醉气体标识和浓度不可信
        {
            AA_all_data.AA_state.module_err_message[4] |= 0x10;
            Err_Num++;
        }
        else
        {
            AA_all_data.AA_state.module_err_message[4] &= ~0x10;
        }
		if(pAll_Data->slow.GenVal.o2_or)   //氧气超差
		{
			AA_all_data.AA_state.module_err_message[4] |= 0x20;
			Err_Num++;
		}
		else
		{
			AA_all_data.AA_state.module_err_message[4] &= ~0x20;
		}
    }
    else if(AA_Module_Name == Drager)   //31个
    {
        //出厂校准缺失
        if((Drager_Message_Register[7]&0x04)||(Drager_Message_Register[6]&0x80)
            ||(Drager_Message_Register[6]&0x10)||(Drager_Message_Register[6]&0x04)
               ||(Drager_Message_Register[6]&0x02))
        {
            AA_all_data.AA_state.module_err_message[0] |= 0x02;
            Err_Num++;
        }
        else
        {
            AA_all_data.AA_state.module_err_message[0] &= ~0x02;
        }
        //软件版本错误
        if((Drager_Message_Register[7]&0x01)||(Drager_Message_Register[3]&0x10))
        {
            AA_all_data.AA_state.module_err_message[0] |= 0x04;
            Err_Num++;
        }
        else
        {
            AA_all_data.AA_state.module_err_message[0] &= ~0x04;
        }
        //传感器过热
        if(Drager_Message_Register[2]&0x02)
        {
            AA_all_data.AA_state.module_err_message[0] |= 0x08;
            Err_Num++;
        }
        else
        {
            AA_all_data.AA_state.module_err_message[0] &= ~0x08;
        }
        //组件失败，有组件不工作，所有参数无效
        if(Drager_Ack.FrameData.ModuleStatus&0x40)
        {
            AA_all_data.AA_state.module_err_message[1] |= 0x01;
            Err_Num++;
        }
        else
        {
            AA_all_data.AA_state.module_err_message[1] &= ~0x01;
        }
        //脱水器满或未连接
        if(Drager_Ack.FrameData.ModuleStatus&0x04)
        {
            AA_all_data.AA_state.module_err_message[1] |= 0x02;
            //脱水器将满
            if(Drager_PressureAndPump.WatertrapWarning)
            {
                AA_all_data.AA_state.module_err_message[1] |= 0x04;
                Err_Num++;
            }
            else
            {
                AA_all_data.AA_state.module_err_message[1] &= ~0x04;
            }
            //脱水器满
            //if((Drager_PressureAndPump.WatertrapFull)||(Drager_Message_Register[1]&0x10))
            if(Drager_PressureAndPump.WatertrapFull)
            {
                AA_all_data.AA_state.module_err_message[1] |= 0x08;
                Err_Num++;
            }
            else
            {
                AA_all_data.AA_state.module_err_message[1] &= ~0x08;
            }
            //脱水器未连接
            if(Drager_PressureAndPump.WatertrapDisconnected)
            {
                AA_all_data.AA_state.module_err_message[1] |= 0x10;
                Err_Num++;
            }
            else
            {
                AA_all_data.AA_state.module_err_message[1] &= ~0x10;
            }
        }
        else
        {
            AA_all_data.AA_state.module_err_message[1] &= ~0x02;
            AA_all_data.AA_state.module_err_message[1] &= ~0x04;
            AA_all_data.AA_state.module_err_message[1] &= ~0x08;
            AA_all_data.AA_state.module_err_message[1] &= ~0x10;
        }


		//脱水器满
		if((Drager_Message_Register[1]&0x10) || (Drager_Message_Register[1]&0x40))
		{
			AA_all_data.AA_state.module_err_message[1] |= 0x08;
			Err_Num++;
		}
		else
		{
			AA_all_data.AA_state.module_err_message[1] &= ~0x08;
		}
        //采样管堵塞
        if(Drager_Ack.FrameData.ModuleStatus&0x02)
        {
            AA_all_data.AA_state.module_err_message[1] |= 0x20;
            Err_Num++;
        }
        else
        {
            AA_all_data.AA_state.module_err_message[1] &= ~0x20;
        }
        //电源输入错误
        if(Drager_Message_Register[0]&0x01)
        {
            AA_all_data.AA_state.module_err_message[1] |= 0x40;
            Err_Num++;
        }
        else
        {
            AA_all_data.AA_state.module_err_message[1] &= ~0x40;
        }
        //气动硬件错误
        if(Drager_Message_Register[0]&0x80)
        {
            AA_all_data.AA_state.module_err_message[1] |= 0x80;
            Err_Num++;
        }
        else
        {
            AA_all_data.AA_state.module_err_message[1] &= ~0x80;
        }
        //输入气体漏气
        if(Drager_Message_Register[0]&0x08)
        {
            AA_all_data.AA_state.module_err_message[2] |= 0x01;
            Err_Num++;
        }
        else
        {
            AA_all_data.AA_state.module_err_message[2] &= ~0x01;
        }
        //输入气体堵塞
        if(Drager_Message_Register[0]&0x10)
        {
            AA_all_data.AA_state.module_err_message[2] |= 0x02;
            Err_Num++;
        }
        else
        {
            AA_all_data.AA_state.module_err_message[2] &= ~0x02;
        }
        //输出气体堵塞
        if(Drager_Message_Register[0]&0x20)
        {
            AA_all_data.AA_state.module_err_message[2] |= 0x04;
            Err_Num++;
        }
        else
        {
            AA_all_data.AA_state.module_err_message[2] &= ~0x04;
        }
        //堵塞
        if(Drager_Message_Register[0]&0x40)
        {
            AA_all_data.AA_state.module_err_message[2] |= 0x08;
            Err_Num++;
        }
        else
        {
            AA_all_data.AA_state.module_err_message[2] &= ~0x08;
        }
        //检测流量
        if(Drager_Message_Register[1]&0x01)
        {
            AA_all_data.AA_state.module_err_message[2] |= 0x10;
            Err_Num++;
        }
        else
        {
            AA_all_data.AA_state.module_err_message[2] &= ~0x10;
        }
        //采样管未连接
        if(Drager_Message_Register[1]&0x02)
        {
            AA_all_data.AA_state.module_err_message[2] |= 0x20;
            Err_Num++;
        }
        else
        {
            AA_all_data.AA_state.module_err_message[2] &= ~0x20;
        }
        //采样管未连接或输入气体漏气
        if(Drager_Message_Register[1]&0x04)
        {
            AA_all_data.AA_state.module_err_message[2] |= 0x40;
            Err_Num++;
        }
        else
        {
            AA_all_data.AA_state.module_err_message[2] &= ~0x40;
        }
//        //检测脱水器
//        if(Drager_Message_Register[1]&0x08)
//        {
//            AA_all_data.AA_state.module_err_message[2] |= 0x80;
//            Err_Num++;
//        }
//        else
//        {
//            AA_all_data.AA_state.module_err_message[2] &= ~0x80;
//        }
        //检测脱水器或输入气体堵塞
        if(Drager_Message_Register[1]&0x20)
        {
            AA_all_data.AA_state.module_err_message[3] |= 0x01;
            Err_Num++;
        }
        else
        {
            AA_all_data.AA_state.module_err_message[3] &= ~0x01;
        }
        //脱水器满或输入气体堵塞
        if(Drager_Message_Register[1]&0x40)
        {
            AA_all_data.AA_state.module_err_message[3] |= 0x02;
            Err_Num++;
        }
        else
        {
            AA_all_data.AA_state.module_err_message[3] &= ~0x02;
        }
        //校零失败
//        if(Drager_Message_Register[2]&0x08)
//        {
//            AA_all_data.AA_state.module_err_message[3] |= 0x04;
//            Err_Num++;
//        }
//        else
//        {
//            AA_all_data.AA_state.module_err_message[3] &= ~0x04;
//        }
        //气动组件不可用
        if(Drager_Message_Register[3]&0x40)
        {
            AA_all_data.AA_state.module_err_message[3] |= 0x08;
            Err_Num++;
        }
        else
        {
            AA_all_data.AA_state.module_err_message[3] &= ~0x08;
        }
        //气动组件连接中断
        if(Drager_Message_Register[4]&0x02)
        {
            AA_all_data.AA_state.module_err_message[3] |= 0x10;
            Err_Num++;
        }
        else
        {
            AA_all_data.AA_state.module_err_message[3] &= ~0x10;
        }

		//检查脱水器	zkq add
		if(Drager_Message_Register[1]&0x08)
		{
			AA_all_data.AA_state.module_err_message[3] |= 0x20;
			Err_Num++;
		}
		else
		{
			AA_all_data.AA_state.module_err_message[3] &= ~0x20;
		}

        //CO2传感器硬件错误
        if(Drager_Message_Register[0]&0x02)
        {
            AA_all_data.AA_state.module_err_message[4] |= 0x20;
            Err_Num++;
        }
        else
        {
            AA_all_data.AA_state.module_err_message[4] &= ~0x20;
        }
        //O2传感器硬件错误
        if(Drager_Message_Register[0]&0x04)
        {
            AA_all_data.AA_state.module_err_message[4] |= 0x40;
            Err_Num++;
        }
        else
        {
            AA_all_data.AA_state.module_err_message[4] &= ~0x40;
        }
        //CO2传感器不可用
        if(Drager_Message_Register[3]&0x80)
        {
            AA_all_data.AA_state.module_err_message[4] |= 0x80;
            Err_Num++;
        }
        else
        {
            AA_all_data.AA_state.module_err_message[4] &= ~0x80;
        }
        //氧气传感器不可用
        if(Drager_Message_Register[4]&0x01)
        {
            AA_all_data.AA_state.module_err_message[5] |= 0x01;
            Err_Num++;
        }
        else
        {
            AA_all_data.AA_state.module_err_message[5] &= ~0x01;
        }
        //CO2传感器连接中断
        if(Drager_Message_Register[4]&0x04)
        {
            AA_all_data.AA_state.module_err_message[5] |= 0x02;
            Err_Num++;
        }
        else
        {
            AA_all_data.AA_state.module_err_message[5] &= ~0x02;
        }
        //氧气传感器连接中断
        if(Drager_Message_Register[4]&0x08)
        {
            AA_all_data.AA_state.module_err_message[5] |= 0x04;
            Err_Num++;
        }
        else
        {
            AA_all_data.AA_state.module_err_message[5] &= ~0x04;
        }
        //风扇停转
        if(Drager_Message_Register[1]&0x80)
        {
            AA_all_data.AA_state.module_err_message[5] |= 0x08;
            Err_Num++;
        }
        else
        {
            AA_all_data.AA_state.module_err_message[5] &= ~0x08;
        }
    }
    else
    {
        
    }
}
/*
*************************************************************************
 * Function Name: AA_Module_Monitor()
 * Parameters:void
 * Return:void
 *
 * Description:
 *
 *************************************************************************
*/
void AA_Module_Monitor(void)
{
    static Int8U Zero_Count = 0;
    static Int8U Zero_request1 = 0x55;
    static Int8U Zero_request2 = 0;
    static Int8U PhaseIn_InitFlag = 1;

    Int8U Zero_Flag = 0;
    Int8U Get_Dev_Info = 1;
    Int8U Para_Accuracy = 0;//0:有参数不是高精度，1:参数都为高精度
    //Int8U Temp_Zero[10] = {0}; 
    //static Int8U Count[10] = {0};
    AA_GetSlowData(AA_DETAIL_CO2);
    AA_GetSlowData(AA_DETAIL_N2O);
    AA_GetSlowData(AA_DETAIL_AA1);
    AA_GetSlowData(AA_DETAIL_AA2);
    AA_GetSlowData(AA_DETAIL_O2);
    AA_GetSlowData(AA_DETAIL_COMPN);
    AA_GetSlowData(AA_DETAIL_PRESSURE);
    AA_GetSlowData(AA_DETAIL_BREATH);
    
    AA_GetWaveData(AA_GAS_CO2);
    AA_GetWaveData(AA_GAS_N2O);
    AA_GetWaveData(AA_GAS_AA1);
    AA_GetWaveData(AA_GAS_AA2);
    AA_GetWaveData(AA_GAS_O2);
    AA_GetModuleState();
    AA_Err_Message_Progress();
    if(AA_Module_Name == PhaseIn)
    {
        if(PhaseIn_InitFlag == 1)
        {
            PhaseIn_InitFlag = 2;
            //AA_SetIRO2Value(0x08);//20171130 tmp del
            //AA_SetO2Option(0x01);//设置氧浓度传感器
            //AA_SetO2SensorType(0x01);//顺磁氧
        }
    }
    else if(AA_Module_Name == Drager)
    {
        //连接Drager模块
        if(DRAGER_FLAG.Connect_Flag == 1)
        {
            DRAGER_FLAG.Connect_Flag = 2;
            //设置呼吸探测模式
            AA_SetBreathDetection(BR_mode_Auto_wake_up);
            AA_TASK_REGISTER |= AA_MONITOR_SETBREATHDETECTION;
            //获取病人信息
            AA_TASK_REGISTER |= AA_MONITOR_SETPATIENTDATA;
            //设置CO2单位
            AA_SetParaUnit(DRAGER_CO2,DRAGER_UNIT_ATPS);
            AA_TASK_REGISTER |= AA_MONITOR_SETUNIT;
            //开泵
            //AA_CMD_Para.CMD_SWITCH_pump = DRAGER_PUMP_HIGH;
            //AA_TASK_REGISTER |= AA_MONITOR_SWITCHPUMPS;
            
            AA_Timer.timer_ms = 0;
            AA_Timer.timer_sec = 0;
            AA_Timer.timer_min = 0;
            AA_Timer.timer_hour = 0;       
        }
        else if(DRAGER_FLAG.Connect_Flag == 2)
        {
            if(Get_Dev_Info)
            {
                if(!(Drager_DeviceInfo.Drager_Dev_Flag&0x01))
                {
                    AA_SetDevInfo(AA_DEV_vendor_code);
                }
                else if(!(Drager_DeviceInfo.Drager_Dev_Flag&0x02))
                {
                    AA_SetDevInfo(AA_DEV_serial_num);
                }
                else if(!(Drager_DeviceInfo.Drager_Dev_Flag&0x04))
                {
                    AA_SetDevInfo(AA_DEV_hw_ver);
                }
                else if(!(Drager_DeviceInfo.Drager_Dev_Flag&0x08))
                {
                    AA_SetDevInfo(AA_DEV_sw_ver);
                }
                else if(!(Drager_DeviceInfo.Drager_Dev_Flag&0x20))
                {
                    AA_SetDevInfo(AA_DEV_type_num);
                }
                else
                {
                    Get_Dev_Info = 0;
                }
            }
            //开机后1分钟监测精度
            if((AA_Timer.timer_min>=2)||(AA_Timer.timer_hour))
            {
                if(AA_all_data.AA_state.AA_ModuleState.zero_state == zero_ok)
                {
                    if((AA_all_data.AA_detail.AA_CO2.ParaMode == PS_Valid) && 
                       (AA_all_data.AA_detail.AA_CO2.Accuracy != ACCURACY_Iso))
                    {
                        Zero_Flag = 1;
                    }
                    if((AA_all_data.AA_detail.AA_N2O.ParaMode == PS_Valid) && 
                       (AA_all_data.AA_detail.AA_N2O.Accuracy != ACCURACY_Iso))
                    {
                        Zero_Flag = 1;
                    }
                    if(Drager_EtCO2_Config_On == 2)
                    {
                        if((AA_all_data.AA_detail.AA_O2.ParaMode == PS_Valid) && 
                           (AA_all_data.AA_detail.AA_O2.Accuracy != ACCURACY_Iso))
                        {
                            Zero_Flag = 1;
                        }
                    }
                    if((AA_all_data.AA_detail.AA_AA1.ParaMode == PS_Valid) && 
                       (AA_all_data.AA_detail.AA_AA1.Accuracy != ACCURACY_Iso))
                    {
                        Zero_Flag = 1;
                    }
                    //实时监测阀状态
                    if((AA_all_data.AA_state.AA_ModuleState.valve_state != AA_valve_sample_gas) && 
                       (AA_all_data.AA_state.AA_ModuleState.pump_state == AA_pump_high))
                    {
                        AA_CMD_Para.CMD_SWITCH_valve = DRAGER_VALVE_SAMPLE;
                        AA_TASK_REGISTER |= AA_MONITOR_SWITCHVALVES;
                    }
                }
                //实时监测校零请求
                else if(AA_all_data.AA_state.AA_ModuleState.zero_state == zero_request)
                {
                    Zero_Flag = 1;
                }
                //实时监测泵状态
                if(!StandbyState)//工作界面下
                {
                    if(AA_all_data.AA_state.AA_ModuleState.pump_state != AA_pump_high)//泵关
                    {
                        //开泵
                        AA_CMD_Para.CMD_SWITCH_pump = DRAGER_PUMP_HIGH;//开泵
                        AA_TASK_REGISTER |= AA_MONITOR_SWITCHPUMPS;
                    }
                }
                if((Drager_PORZFlag&(~(1<<7))) >= 4)//刚开机5分钟
                {
                    Drager_PORZFlag = 0;
                    if(Drager_EtCO2_Config_On == 2)
                    {
                        if(StandbyState == 0xFF)//待机下
                        {
                            if(AA_all_data.AA_detail.AA_O2.ZeroState  == ZSP_No_In_Progress\
                            && AA_all_data.AA_detail.AA_CO2.ZeroState == ZSP_No_In_Progress\
                            && AA_all_data.AA_detail.AA_N2O.ZeroState == ZSP_No_In_Progress\
                            && AA_all_data.AA_detail.AA_AA1.ZeroState == ZSP_No_In_Progress)//无参数正在校验状态下
                            {
                                if(AA_all_data.AA_detail.AA_O2.Accuracy != ACCURACY_Iso\
                                || AA_all_data.AA_detail.AA_CO2.Accuracy != ACCURACY_Iso\
                                || AA_all_data.AA_detail.AA_N2O.Accuracy != ACCURACY_Iso\
                                || AA_all_data.AA_detail.AA_AA1.Accuracy != ACCURACY_Iso)
                                {
                                    AA_TASK_REGISTER |= AA_MONITOR_INITZERO;
                                }
                            }
                        }
                    }
                    else if(Drager_EtCO2_Config_On == 1)
                    {
                        if(StandbyState == 0xFF)//待机下
                        {
                            if(AA_all_data.AA_detail.AA_CO2.ZeroState == ZSP_No_In_Progress\
                            && AA_all_data.AA_detail.AA_N2O.ZeroState == ZSP_No_In_Progress\
                            && AA_all_data.AA_detail.AA_AA1.ZeroState == ZSP_No_In_Progress)//无参数正在校验状态下
                            {
                                if(AA_all_data.AA_detail.AA_CO2.Accuracy != ACCURACY_Iso\
                                || AA_all_data.AA_detail.AA_N2O.Accuracy != ACCURACY_Iso\
                                || AA_all_data.AA_detail.AA_AA1.Accuracy != ACCURACY_Iso)
                                {
                                    AA_TASK_REGISTER |= AA_MONITOR_INITZERO;
                                }
                            }
                        }
                    }
                }
            }
        }
        else
        {
            DRAGER_FLAG.Connect_Flag = 1;
        }
        if(Zero_Flag)
        {
            if(Zero_request1 == (unsigned char)0x55)//Zero_request1默认初始化数值不许使用0-59之内的数值
            {
                Zero_request1 = AA_Timer.timer_min;
                AA_TASK_REGISTER |= AA_MONITOR_INITZERO;
                Zero_Count++;
                Zero_request2 = 0;
            }
            else
            {
                Zero_request2 = AA_Timer.timer_min;
                if(Zero_request2 - Zero_request1)
                {
                    AA_TASK_REGISTER |= AA_MONITOR_INITZERO;
                    Zero_Count++;
                    Zero_request1 = Zero_request2;
                    Zero_request2 = 0;
                }
            }
            if(Zero_Count > 3)
            {
                AA_all_data.AA_state.module_err_message[3] |= 0x04;
                Zero_Count = 0;
            }
        }
        else
        {
            //AA_TASK_REGISTER &= ~AA_MONITOR_INITZERO;
            AA_all_data.AA_state.module_err_message[3] &= ~0x04;
            Zero_Count = 0;
        }
    }
    else
    {
        
    }
}
/*
*************************************************************************
 * Function Name: AA_Module_SendOperationMessage()
 * Parameters:void
 * Return:void
 *
 * Description:
 *
 *************************************************************************
*/
void AA_Module_SendOperationMessage(Int8U type)
{
    switch(type)
    {
    case SET_AA_OPERATE_STANDBY:
        AA_SetOpMode(AA_MODE_STANDBY);
        USART2_TXD_RIGSTER |= AA_TASK_REGISTER;
        if(USART2_TXD_Busy_FLAG==FALSE)
        {
            Inquire_TXD_Handler(USART2);
        }
        break;
    case SET_AA_OPERATE_MEASURE:
        AA_SetOpMode(AA_MODE_MEASURE);
        USART2_TXD_RIGSTER |= AA_TASK_REGISTER;
        if(USART2_TXD_Busy_FLAG==FALSE)
        {
            Inquire_TXD_Handler(USART2);
        }
        break;
    case SET_AA_GAS_HAL:
        AA_SetAAGas(AA_GAS_AA1,AA_AGT_HAL);
        USART2_TXD_RIGSTER |= AA_TASK_REGISTER;
        if(USART2_TXD_Busy_FLAG==FALSE)
        {
            Inquire_TXD_Handler(USART2);
        }
        break;
    case SET_AA_GAS_ENF:
        AA_SetAAGas(AA_GAS_AA1,AA_AGT_ENF);
        USART2_TXD_RIGSTER |= AA_TASK_REGISTER;
        if(USART2_TXD_Busy_FLAG==FALSE)
        {
            Inquire_TXD_Handler(USART2);
        }
        break;
    case SET_AA_GAS_ISO:
        AA_SetAAGas(AA_GAS_AA1,AA_AGT_ISO);
        USART2_TXD_RIGSTER |= AA_TASK_REGISTER;
        if(USART2_TXD_Busy_FLAG==FALSE)
        {
            Inquire_TXD_Handler(USART2);
        }
        break;
    case SET_AA_GAS_SEV:
        AA_SetAAGas(AA_GAS_AA1,AA_AGT_SEV);
        USART2_TXD_RIGSTER |= AA_TASK_REGISTER;
        if(USART2_TXD_Busy_FLAG==FALSE)
        {
            Inquire_TXD_Handler(USART2);
        }
        break;
    case SET_AA_GAS_DES:
        AA_SetAAGas(AA_GAS_AA1,AA_AGT_DES);
        USART2_TXD_RIGSTER |= AA_TASK_REGISTER;
        if(USART2_TXD_Busy_FLAG==FALSE)
        {
            Inquire_TXD_Handler(USART2);
        }
        break;
    case SET_AA_ZERO_INIT:
        USART2_TXD_RIGSTER |= AA_MONITOR_INITZERO;//调零
        if(USART2_TXD_Busy_FLAG==FALSE)
        {
            Inquire_TXD_Handler(USART2);
        }
        break;
    case SET_AA_EXPARA_O2:
        USART2_TXD_RIGSTER |= AA_TASK_REGISTER;
        if(USART2_TXD_Busy_FLAG==FALSE)
        {
            Inquire_TXD_Handler(USART2);
        }
        break;
    case SET_AA_EXPARA_N2O:
        USART2_TXD_RIGSTER |= AA_TASK_REGISTER;
        if(USART2_TXD_Busy_FLAG==FALSE)
        {
            Inquire_TXD_Handler(USART2);
        }
        break;
    case SET_AA_STOP_CONTINUE:
        USART2_TXD_RIGSTER |= AA_MONITOR_STOPCONTTRANS;
        if(USART2_TXD_Busy_FLAG==FALSE)
        {
            Inquire_TXD_Handler(USART2);
        }
        break;
    case SET_AA_UNIT_ATS:
        AA_SetParaUnit(DRAGER_CO2,DRAGER_UNIT_ATS);
        USART2_TXD_RIGSTER |= AA_TASK_REGISTER;
        if(USART2_TXD_Busy_FLAG==FALSE)
        {
            Inquire_TXD_Handler(USART2);
        }
        break;
    case SET_AA_UNIT_ATPS:
        AA_SetParaUnit(DRAGER_CO2,DRAGER_UNIT_ATPS);
        USART2_TXD_RIGSTER |= AA_TASK_REGISTER;
        if(USART2_TXD_Busy_FLAG==FALSE)
        {
            Inquire_TXD_Handler(USART2);
        }
        break;
    case SET_AA_BRDETECTION:
        USART2_TXD_RIGSTER |= AA_TASK_REGISTER;
        if(USART2_TXD_Busy_FLAG==FALSE)
        {
            Inquire_TXD_Handler(USART2);
        }
        break;
    case SET_AA_PUMPON:
        AA_SetPumpMode(AA_pump_high);
        USART2_TXD_RIGSTER |= AA_TASK_REGISTER;
        if(USART2_TXD_Busy_FLAG==FALSE)
        {
            Inquire_TXD_Handler(USART2);
        }
        break;
    case SET_AA_PUMPOFF:
        AA_SetPumpMode(AA_pump_Off);
        USART2_TXD_RIGSTER |= AA_TASK_REGISTER;
        if(USART2_TXD_Busy_FLAG==FALSE)
        {
            Inquire_TXD_Handler(USART2);
        }
        break;
    case SET_AA_IRO2DELAY:
        USART2_TXD_RIGSTER |= AA_MONITOR_SETIRO2DELAY;
        if(USART2_TXD_Busy_FLAG==FALSE)
        {
            Inquire_TXD_Handler(USART2);
        }                
        break;
    case SET_AA_SETO2OPTION:
        USART2_TXD_RIGSTER |= AA_MONITOR_SETO2OPTION;
        if(USART2_TXD_Busy_FLAG==FALSE)
        {
            Inquire_TXD_Handler(USART2);
        }                
        break;
    case SET_AA_SETO2SENSORTYPE:
        USART2_TXD_RIGSTER |= AA_MONITOR_SETO2SENSORTYPE;
        if(USART2_TXD_Busy_FLAG==FALSE)
        {
            Inquire_TXD_Handler(USART2);
        }                
        break;
    case SET_AA_SPANCAL:
        USART2_TXD_RIGSTER |= AA_MONITOR_SPANCAL;
        if(USART2_TXD_Busy_FLAG==FALSE)
        {
            Inquire_TXD_Handler(USART2);
        }                
        break;
    case SET_AA_SPANCAL21:
        USART2_TXD_RIGSTER |= AA_MONITOR_SPANCAL21;
        if(USART2_TXD_Busy_FLAG==FALSE)
        {
            Inquire_TXD_Handler(USART2);
        }                
        break;                
    default:
        break;
    }
}