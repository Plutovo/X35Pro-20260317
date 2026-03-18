#include "system_app.h"


extern U8 Checking_Show_Flag;//Checking显示标志
extern U8 OxygenSensorCalPromptFlag;//氧浓度传感器检测提示标志位

extern unsigned char O2ModuleOff;   //djq add 20140213
unsigned int O2Continue_Time = 0;
unsigned int O2Fail_Time = 0;
extern unsigned char Drager_EtCO2_Config_On; //0:关闭   1：开启麻醉气体    2：开启麻醉气体+顺磁氧 
/**************************************************************************************
 *函数名：OxygenSensor_Test
 *参数  ：无
 *返回值：产生警告和故障的总数
 *被调用位置：
 *功能  ：执行压力传感器测试程序。如果出现错误，存储到ALARM LOG中，并显示到当前检测项目之后
		  接收到全局的取消命令之后能够直接退出
***************************************************************************************
*/
U8 OxygenSensor_Test(U8 SendCommand)
{
	U32 return_number = 0;
    U8 Test_NextItemFlag = 1;//进行下一步的标志，只有进行错误提示的时候和重新检测的时候不会进行下一步

	//Disp_CheckItem_Text(Context_GetPosition());//显示当前检测项目名称
    if(SendCommand)
    {                //氧浓度传感器提示
		O2Continue_Time = 0;
		O2Fail_Time = 0;
		if(Drager_EtCO2_Config_On == 2 || MasimoISA_EtCO2_Config_On == 2)//顺磁氧 //ylk modify 20181129
		{
// 			if((!O2ModuleOff) && ((AA_Module_Flag.EtCO2ModuleExist&0x04) == 0x04)\
// 			&& (!Im_getSlowData(IM_GET_O2_ERR,0)))//开关开 && 连接上 && 无传感器错误
// 			&& (!(AA_all_data.AA_state.module_err_message[1]&0x01)))
			if(!O2ModuleOff)
			{
				Serial_Communaction_Handing(S_SEND_TO_BDU_OXYGEN);
				TimeOutChech_Init(OXYGEN_TIMEOUT_NUM);
			}
			else
			{
				//SendOperationMessage(SET_AA_OPERATE_STANDBY,USART2);
				SendOperationMessage(SET_AA_PUMPOFF,USART2);//zkq add 20170408跳过氧浓度自检时关泵
				Disp_Result(2,RESULT_SKIP);
				Save_ErrorNumber_Reault(MEM_OXYGEN_OFFSET_ADDR,&(CONTEXT_APP.Current_ErrorCode),return_number);
				Context_NextRight();//检测下一项
			}
		}
		else
		{
			if(!O2ModuleOff)//标志相反
			{
				Serial_Communaction_Handing(S_SEND_TO_BDU_OXYGEN);
				TimeOutChech_Init(OXYGEN_TIMEOUT_NUM);
			}
			else
			{
				Disp_Result(2,RESULT_SKIP);
				Save_ErrorNumber_Reault(MEM_OXYGEN_OFFSET_ADDR,&(CONTEXT_APP.Current_ErrorCode),return_number);
				Context_NextRight();//检测下一项
			}
		}
    }
    else
    {
        if(CONTEXT_APP.Global_CancelFlag == 1)
        {
            Disp_Result(2,RESULT_SKIP);
			Context_NextRight();//检测下一项
        }
        else //if(CONTEXT_APP.Result)
        {
#ifdef _SELF_TEST
			return_number = 0;
#else
			return_number = CONTEXT_APP.Result;
#endif
//          return_number = CONTEXT_APP.Result;
            if(return_number&0x8000)//报错
            {
                return_number = return_number&0x7fff;
                switch(return_number)
                {
                case COMMUNICATION_TIMEOUT_ERR:
                    if(Drager_EtCO2_Config_On == 2 || MasimoISA_EtCO2_Config_On == 2) //ylk modify 20181204
					{
						SendOperationMessage(SET_AA_PUMPOFF,USART2);
					}
					O2Continue_Time = 0;//zkq add 20170409 清零 防止下次进入直接通过自检
					O2Fail_Time = 0;
                    Error_Handing_H(COMMUNICATION_TIMEOUT_ERR);
                    Disp_Result(2,RESULT_FAULT);
                    Context_NextRight();//检测下一项
                    CONTEXT_APP.Global_CancelFlag = 1;//超时，跳过剩下的自检项目
                    break;
                case GAS_SUPPLY_FAILURE://气源压力故障
                case O2_SENSOR_INVALID://传感器失效或者未连接，更换氧浓度传感器
                case O2_SENSOR_FAILURE://传感器故障
					O2Continue_Time = 0;//zkq add 20170409 清零 防止下次进入直接通过自检
					O2Fail_Time = 0;
                    Error_Handing_H(GAS_SUPPLY_FAILURE);
                    Disp_Result(2,RESULT_FAULT);
                    Disp_Dialog_OxygenSensor_Prompt(return_number);//下一项检测氧浓度传感器。
                    Test_NextItemFlag = 0;
                    break;
                }
            }
            else 
            {
				O2Continue_Time = 0;//zkq add 20170409 清零 防止下次进入直接通过自检
				O2Fail_Time = 0;
                Disp_Result(2,RESULT_SUCCESS);
                Test_NextItemFlag = 1;
            }
			Save_ErrorNumber_Reault(MEM_OXYGEN_OFFSET_ADDR,&(CONTEXT_APP.Current_ErrorCode),return_number);
            //sendFlowmeterCommand(0x01,100,0);//氧浓度结束，发送0流量到EGM  djq add 20161024
            if(Test_NextItemFlag)
            {
                Context_NextRight();//检测下一项
//				if(MasimoISA_EtCO2_Config_On == 2)
                if(Drager_EtCO2_Config_On == 2 || MasimoISA_EtCO2_Config_On == 2) //ylk modify 20181204
				{
					SendOperationMessage(SET_AA_PUMPOFF,USART2);
				}
            }
        }
    }
	return 0;
}
