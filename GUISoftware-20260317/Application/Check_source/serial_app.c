#include "serial_app.h"
#include "Communication_Handle.h"
//#include "Debug_App.h"
//source define of Key_Handle.h
#ifndef					USART0
#define                 USART0                          0x55
#define                 USART3                          0x5d
#define                 EXT_USART1                      0x65
#define                 EXT_USART2                      0x6d
#define					USART2                          0x62
#endif

extern Int8U	EXTUsart1_Tx_Buffer[5];
extern Int8U	Usart0_Tx_Buffer[4];
extern Int8U	Usart1_Tx_Buffer[4];
extern Int8U	Usart3_Tx_Buffer[4];
extern char		Usart2_Tx_Buffer[20];
extern unsigned char   USART0_TXD_Busy_FLAG;

Int8U	_EXTUsart2_Rx_Buffer[4] = {0};  //在外部串口中断2中接收到数据，回到Serial_Communaction_Handing_P中处理
Int8U   _EXTUsart2_Tx_Buffer[5] = {0};  //在Serial_Communaction_Handing_P（）函数中填写数据，到外部串口中断2中发送
Int8U	_Usart0_Rx_Buffer[4] = {0};     //在串口中断1中接收到数据，回到Serial_Communaction_Handing（）函数中处理
Int8U	_Usart1_Rx_Buffer[4] = {0};
Int8U	_Usart3_Rx_Buffer[4] = {0};     //在串口中断3中接收到数据，回到Serial_Communaction_Handing（）函数中处理
Int8U	_Usart2_Rx_Buffer[8] = {0};

Int64U TEST_UART0_TXD_REGISTER = 0;

Int8U PressureSensorZero = 0;//压力传感器零点成功失败标志位
Int8U FlowSensorZero = 0;//流量传感器零点成功失败标志位

extern U8 OxygenSensorCalPromptFlag;//氧浓度传感器检测提示标志位
extern MONITOR_PARA MonitorPara;
extern unsigned char Drager_EtCO2_Config_On; //0:关闭   1：开启麻醉气体    2：开启麻醉气体+顺磁氧 
/***********************************************************
 *函数名：Test_TXD_Handle
 *参数  ：目标串口标志位
 *返回值：0：还有命令未发送   1：所有命令发送完成。
 *被调用位置：Inquiry_TXD_Handle()
 *功能  ：向目标串口发送自检中使用的命令
************************************************************
*/
Int8U Test_TXD_Handle(Int8U UsartPort)
{
    Int8U ErrFlag = 0;
    Int64U ComFlag = 1;
    Int8U Cnt = 0;
    char prom[100]={0};
    switch(UsartPort)
    {
    case USART0:
        if(TEST_UART0_TXD_REGISTER)
        {
            for(Cnt = 0;Cnt < TEST_COMMUNICATION_NUM;Cnt++)
            {
                if(TEST_UART0_TXD_REGISTER & ComFlag)
                {
                    break;
                }
                ComFlag <<= 1;
            }
            switch(ComFlag)
            {
            case TEST_COM_STARTTEST://Start Self Test
                DataTransmit(0 , BDU_TEST_START ,Usart0_Tx_Buffer,USART0);
                TEST_UART0_TXD_REGISTER &= (~TEST_COM_STARTTEST);
                USART0_TXD_Busy_FLAG = 1;
                break;
            case TEST_COM_MANUAL://手动回路检测
                DataTransmit(0 , S_SEND_TO_BDU_MAN ,Usart0_Tx_Buffer,USART0);
                TEST_UART0_TXD_REGISTER &= (~TEST_COM_MANUAL);
                USART0_TXD_Busy_FLAG = 1;
                break;
            case TEST_COM_BAROMETER://大气压力传感器
                DataTransmit(0 , S_SEND_TO_BDU_BARO ,Usart0_Tx_Buffer,USART0);
                TEST_UART0_TXD_REGISTER &= (~TEST_COM_BAROMETER);
                USART0_TXD_Busy_FLAG = 1;
                break;
            case TEST_COM_AIRSUPPLY://空气气源压力检测
                DataTransmit(0 , S_SEND_TO_BDU_AS_AIR ,Usart0_Tx_Buffer,USART0);//两个气源的命令相同，附加数据不同
                TEST_UART0_TXD_REGISTER &= (~TEST_COM_AIRSUPPLY);
                USART0_TXD_Busy_FLAG = 1;
                break;
                //case TEST_COM_N2OSUPPLY://笑气气源压力检测
                //    DataTransmit(S_RECV_OF_BDU_AS_N2O , S_SEND_TO_BDU_AS_N2O ,Usart0_Tx_Buffer,USART0);//两个气源的命令相同，附加数据不同
                //    TEST_UART0_TXD_REGISTER &= (~TEST_COM_N2OSUPPLY);
                //    USART0_TXD_Busy_FLAG = 1;
                //    break;
            case TEST_COM_O2SUPPLY://氧气气源压力检测
                DataTransmit(0 , S_SEND_TO_BDU_AS_O2 ,Usart0_Tx_Buffer,USART0);//两个气源的命令相同，附加数据不同
                TEST_UART0_TXD_REGISTER &= (~TEST_COM_O2SUPPLY);
                USART0_TXD_Busy_FLAG = 1;
                break;
            case TEST_COM_OXYGEN_SENSOR://氧浓度传感器检测
                if(OxygenSensorCalPromptFlag == 1)//21%  
                {
                    DataTransmit(0xAA , S_SEND_TO_BDU_OXYGEN ,Usart0_Tx_Buffer,USART0);//两个气源的命令相同，附加数据不同
                }
                else if(OxygenSensorCalPromptFlag == 2)//100%
                {
                    DataTransmit(0x55 , S_SEND_TO_BDU_OXYGEN ,Usart0_Tx_Buffer,USART0);//两个气源的命令相同，附加数据不同
                }
                TEST_UART0_TXD_REGISTER &= (~TEST_COM_OXYGEN_SENSOR);
                USART0_TXD_Busy_FLAG = 1;
                break;
            case TEST_COM_FLOW_SENSOR_ZERO://流量传感器零点检测
                DataTransmit(0 , S_SEND_TO_BDU_FL_Z ,Usart0_Tx_Buffer,USART0);
                TEST_UART0_TXD_REGISTER &= (~TEST_COM_FLOW_SENSOR_ZERO);
                USART0_TXD_Busy_FLAG = 1;
                break;
            case TEST_COM_PRESS_SENSOR_ZERO://压力传感器零点检测
                DataTransmit(0 , S_SEND_TO_BDU_PS_Z ,Usart0_Tx_Buffer,USART0);
                TEST_UART0_TXD_REGISTER &= (~TEST_COM_PRESS_SENSOR_ZERO);
                USART0_TXD_Busy_FLAG = 1;
                break;
            case TEST_COM_FLOW_SENSOR://流量传感器检测
                DataTransmit(0 , S_SEND_TO_BDU_FS ,Usart0_Tx_Buffer,USART0);
                TEST_UART0_TXD_REGISTER &= (~TEST_COM_FLOW_SENSOR);
                USART0_TXD_Busy_FLAG = 1;
                break;
            case TEST_COM_PRESS_SENSOR://压力传感器检测
                DataTransmit(0 , S_SEND_TO_BDU_PS ,Usart0_Tx_Buffer,USART0);
                TEST_UART0_TXD_REGISTER &= (~TEST_COM_PRESS_SENSOR);
                USART0_TXD_Busy_FLAG = 1;
                break;
            case TEST_COM_PEEP_VALVE://PEEP阀检测
                DataTransmit(0 , S_SEND_TO_BDU_PPV ,Usart0_Tx_Buffer,USART0);
                TEST_UART0_TXD_REGISTER &= (~TEST_COM_PEEP_VALVE);
                USART0_TXD_Busy_FLAG = 1;
                break;
            case TEST_COM_SAFETY_VALVE://安全阀检测
                DataTransmit(0 , S_SEND_TO_BDU_SV ,Usart0_Tx_Buffer,USART0);
                TEST_UART0_TXD_REGISTER &= (~TEST_COM_SAFETY_VALVE);
                USART0_TXD_Busy_FLAG = 1;
                break;
            case TEST_COM_POP_OFF_VALVE://Pop-off阀检测
                DataTransmit(0 , S_SEND_TO_BDU_POPV ,Usart0_Tx_Buffer,USART0);
                TEST_UART0_TXD_REGISTER &= (~TEST_COM_POP_OFF_VALVE);
                USART0_TXD_Busy_FLAG = 1;
                break;
            case TEST_COM_CIRCUIT_COMPLIANCE://回路顺应性监测
                DataTransmit(0 , S_SEND_TO_BDU_CC   ,Usart0_Tx_Buffer,USART0);
                TEST_UART0_TXD_REGISTER &= (~TEST_COM_CIRCUIT_COMPLIANCE);
                USART0_TXD_Busy_FLAG = 1;
                break;
            case TEST_COM_CIRCUIT_LEAKAGE://回路泄漏检测
                DataTransmit(0 , S_SEND_TO_BDU_CL   ,Usart0_Tx_Buffer,USART0);
                TEST_UART0_TXD_REGISTER &= (~TEST_COM_CIRCUIT_LEAKAGE);
                USART0_TXD_Busy_FLAG = 1;
                break;
            case TEST_COM_FINISHTEST://Finish Self Test
                DataTransmit(0 , S_SEND_TO_BDU_FINISH   ,Usart0_Tx_Buffer,USART0);
                TEST_UART0_TXD_REGISTER &= (~TEST_COM_FINISHTEST);
                USART0_TXD_Busy_FLAG = 1;
                break;
            }
        }
        else 
        {
            ErrFlag = 1;
        }
        break;
    case USART2:
        break;
    case USART3:
        break;
    }
    return ErrFlag;
}
//通讯协议解析

/***********************************************************
 *函数名：Serial_Communaction_Handing
 *参数  ：命令类型
 *返回值：
 *被调用位置：每项监测中和固定检测超时发送命令的位置。
 *功能  ：根据命令类型设置发送寄存器并且发送命令
************************************************************
*/
U32 Serial_Communaction_Handing(U32 serial_number)
{
    U32 result_number = 0;
	switch(serial_number)
    {
    case BDU_TEST_START:
//		if(MasimoISA_EtCO2_Config_On == 2)
        if(Drager_EtCO2_Config_On == 2 || MasimoISA_EtCO2_Config_On == 2)//ylk modify 20181204
		{
			SendOperationMessage(SET_AA_PUMPOFF,USART2);
		}
//		sendFlowmeterCommand(S_EGM_COMMAND, BDU_TEST_START, 0);
        TEST_UART0_TXD_REGISTER |= TEST_COM_STARTTEST;
        break;
	case S_SEND_TO_BDU_MAN:
        TEST_UART0_TXD_REGISTER |= TEST_COM_MANUAL;
		break;
    case S_SEND_TO_BDU_BARO:
        TEST_UART0_TXD_REGISTER |= TEST_COM_BAROMETER;
        break;
	case S_SEND_TO_BDU_AS_AIR:
        TEST_UART0_TXD_REGISTER |= TEST_COM_AIRSUPPLY;
		break;
	case S_SEND_TO_BDU_AS_N2O:
//		sendFlowmeterCommand(S_EGM_COMMAND, S_SEND_TO_BDU_AS_N2O, 0);
		break;
    case S_SEND_TO_BDU_AS_O2:
        TEST_UART0_TXD_REGISTER |= TEST_COM_O2SUPPLY;
        break;
    case S_SEND_TO_BDU_OXYGEN:
        if(Drager_EtCO2_Config_On == 2 || MasimoISA_EtCO2_Config_On == 2) 
		{
			SendOperationMessage(SET_AA_PUMPON,USART2);
		}
		else
		{
			TEST_UART0_TXD_REGISTER |= TEST_COM_OXYGEN_SENSOR;
		}
// 		TEST_UART0_TXD_REGISTER |= TEST_COM_OXYGEN_SENSOR; //ylk del 20190103
		break;
	case S_SEND_TO_BDU_FS:
        TEST_UART0_TXD_REGISTER |= TEST_COM_FLOW_SENSOR;
		break;
	case S_SEND_TO_BDU_PS:
        TEST_UART0_TXD_REGISTER |= TEST_COM_PRESS_SENSOR;
		break;
	case S_SEND_TO_BDU_PPV:
        TEST_UART0_TXD_REGISTER |= TEST_COM_PEEP_VALVE;
		break;
	case S_SEND_TO_BDU_SV:
        TEST_UART0_TXD_REGISTER |= TEST_COM_SAFETY_VALVE;
		break;
	case S_SEND_TO_BDU_POPV:
        TEST_UART0_TXD_REGISTER |= TEST_COM_POP_OFF_VALVE;
		break;
	case S_SEND_TO_BDU_CC:
        TEST_UART0_TXD_REGISTER |= TEST_COM_CIRCUIT_COMPLIANCE;
        break;
	case S_SEND_TO_BDU_CL:
        TEST_UART0_TXD_REGISTER |= TEST_COM_CIRCUIT_LEAKAGE;
        break;
    case S_SEND_TO_BDU_FINISH://自检结束标志
//		sendFlowmeterCommand(S_EGM_COMMAND, S_SEND_TO_BDU_FINISH, 0);
        TEST_UART0_TXD_REGISTER |= TEST_COM_FINISHTEST;
        break;
    case S_SEND_TO_UI_FINISH:
		DataTransmit(0x0,0xC7,Usart3_Tx_Buffer,USART3);
        break;
    case S_SEND_TO_EGM_FL_Z://零点校验
//        sendFlowmeterCommand(S_EGM_COMMAND, S_SEND_TO_EGM_FL_Z, 0);
        break;
	}
    if(USART0_TXD_Busy_FLAG != 1)
    {
        Inquire_TXD_Handler(USART0);
    }

	return result_number;//可能是错误代码或者测试结果。
}


/***********************************************************
 *函数名：Test_RXD_Handle
 *参数  ：串口数据接收
 *返回值：接收类型错误，则返回0，正确为左侧自检返回1，右侧返回2
 *被调用位置：串口的中断处理函数中。
 *功能  ：串口接收到的数据进行处理
************************************************************
*/
U8 Test_RXD_Handle(Int16U data,Int8U type,Int8U UsartPort)
{
    U8 err = 0; //
    switch(UsartPort)
    {
    case USART0:
        switch(Context_GetPosition())
        {
        case POS_Manual:
			if(type == S_RECV_OF_BDU_MAN)
            {
                err = 1;
            }
            break;
        case POS_Barometer://djq add 20160809
            if(type == S_RECV_OF_BDU_BARO)
            {
                err = 2;
            }
            break;
        case POS_GasSypply:
            if(type == S_RECV_OF_BDU_AS_O2)
            {
                err = 2;
            }
            break;
		case POS_Oxygen:
            if(type == S_RECV_OF_BDU_OXYGEN)
            {
                err = 2;
            }
            break;
        case POS_Flow_S:
            if(type == S_RECV_OF_BDU_FS)
            {
                err = 2;
            }
            break;
        case POS_Pressure_S:
            if(type == S_RECV_OF_BDU_PS)
            {
                err = 2;
            }
            break;

        case POS_Compliance:
            if(type == S_RECV_OF_BDU_COMPLIANCE)
            {
                err = 2;
            }
            break;
        case POS_Circuit_Leakage:
            if(type == S_RECV_OF_BDU_AIRTIGHT)
            {
                err = 2;
            }
            break;
        case POS_Battery:
            break;

        //djq del 20160809
        //case POS_PEEP_Valve:
        //    if(type == S_RECV_OF_BDU_PPV)
        //    {
        //        err = 2;
        //    }
        //    break;
        //case POS_Safety_Valve:
        //    if(type == S_RECV_OF_BDU_SV)
        //    {
        //        err = 2;
        //    }
        //    break;
        //case POS_Pop_off_Valve:
        //    if(type == S_RECV_OF_BDU_POPV)
        //    {
        //        err = 2;
        //    }
        //    break;    
        //case POS_ACGO:
        //    err = 2;
        //    break;

        default:
            break;
        }
		if(type == S_RECV_OF_BDU_FL_Z)//流量传感器零点
        {
            if(data&0x8000)//失败
            {
                FlowSensorZero = 0;
            }
            else//通过
            {
                FlowSensorZero = 1;
            }
        }
        if(type == S_RECV_OF_BDU_PS_Z)//压力传感器零点
        {
            if(data&0x8000)//失败
            {
                PressureSensorZero = 0;
            }
            else//通过
            {
                PressureSensorZero = 1;
            }
        }
		if(type == ABSORBER_DISCONNECT_COMMAND)
		{
			MonitorPara.ABSORBER_Flag = data;
		}        
        break;
    case USART2://POS_FG_Flow_S_Z  POS_FG_Flow_ZERO_S
        break;
    case USART3:
        break;
    default:
        break;
    }
    if(err == 1)
    {
        Context_SetUpdateFlag(UPDATE_LEFT_POS);//可以更新自检项目
        CONTEXT_APP.Result = data;
        err = 0;
    }
    else if (err == 2)
    {
        Context_SetUpdateFlag(UPDATE_RIGHT_POS);//可以更新自检项目
        CONTEXT_APP.Result = data;
        err = 0;
    }
    else//djq modify 20161017
    {
        //Context_SetUpdateFlag(UPDATE_NONE_ITEM);
        //CONTEXT_APP.Result = 0;
        //err = 1;
    }
    return err;
}
