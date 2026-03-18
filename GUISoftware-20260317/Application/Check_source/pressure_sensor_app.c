#include "system_app.h"

extern Int8U PressureSensorZero;//压力传感器零点成功失败标志位
/**************************************************************************************
 *函数名：PressureSensor_Test
 *参数  ：无
 *返回值：产生警告和故障的总数
 *被调用位置：
 *功能  ：执行压力传感器测试程序。如果出现错误，存储到ALARM LOG中，并显示到当前检测项目之后
		  接收到全局的取消命令之后能够直接退出
***************************************************************************************
*/
U8 PressureSensor_Test(U8 SendCommand)
{
	U32 return_number = 0;
    U8 Test_NextItemFlag = 1;//进行下一步的标志，只有进行错误提示的时候和重新检测的时候不会进行下一步

	//Disp_CheckItem_Text(Context_GetPosition());//显示当前检测项目名称
    if(SendCommand)
    {
		Serial_Communaction_Handing(S_SEND_TO_BDU_PS);
		TimeOutChech_Init(PS_TIMEOUT_NUM);
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
            //if((return_number&0x8000) || (!PressureSensorZero))//报错
            if(return_number&0x8000)//djq modify 20160809 
            {
                return_number = return_number&0x7fff;
                if(return_number == COMMUNICATION_TIMEOUT_ERR)
                {	//超时
                    Error_Handing_H(COMMUNICATION_TIMEOUT_ERR);
                    Disp_Result(2,RESULT_FAULT);
                    CONTEXT_APP.Global_CancelFlag = 1;//超时，跳过剩下的自检项目
                }
                else if(return_number == P_SENSOR_TEST_ERR)
                {
                    Error_Handing_H(P_SENSOR_TEST_ERR);
                    Disp_Result(2,RESULT_FAULT);
                    Disp_Dialog_PressureSensor_Fail();
                    Test_NextItemFlag = 0;
                }
            }
            else 
            {
                Disp_Result(2,RESULT_SUCCESS);
            }
			Save_ErrorNumber_Reault(MEM_PRESSURE_OFFSET_ADDR,&(CONTEXT_APP.Current_ErrorCode),return_number);
            if(Test_NextItemFlag)
            {
                Context_NextRight();//检测下一项
            }
        }
    }
	return 0;
}
