#include "system_app.h"


/**************************************************************************************
 *函数名：Manual_Test
 *参数  ：无
 *返回值：产生警告和故障的总数
 *被调用位置：
 *功能  ：执行气压计测试程序。如果出现错误，存储到ALARM LOG中，并显示到当前检测项目之后.
		  接收到全局的取消命令之后能够直接退出
***************************************************************************************
*/
U8 Manual_Test(U8 SendCommand)/*  返回值是模拟出来的 */
{
	U32 return_number = 0;
	double	return_double = 0;
    U8 Test_NextItemFlag = 1;//进行下一步的标志，只有进行错误提示的时候和重新检测的时候不会进行下一步
	//Disp_CheckItem_Text(Context_GetPosition());//显示当前检测项目名称
    if(SendCommand)
    {
        Serial_Communaction_Handing(S_SEND_TO_BDU_MAN);
		TimeOutChech_Init(MANUAL_TIMEOUT_NUM);
    }
    else
    {
        if(CONTEXT_APP.Global_CancelFlag == 1)//跳过,不保存检测信息
        {
            Disp_Result(2,RESULT_SKIP);
            Context_NextLeft();//检测下一项
        }
        else //if(CONTEXT_APP.Result)
        {
            return_number = CONTEXT_APP.Result;
            if(return_number & 0x8000)//错误
            {
                return_number &= 0x7fff;
                switch(return_number)
                {
                case COMMUNICATION_TIMEOUT_ERR:
                    Error_Handing_H(COMMUNICATION_TIMEOUT_ERR);
                    Disp_Result(2,RESULT_FAULT);
                    CONTEXT_APP.Global_CancelFlag = 1;//超时，跳过剩下的自检项目
                    break;
                case LEAKAGE_RATE_HIGH://泄露超范围
                case MANUAL_CTRLMODE_ERR://手动机控位置
                case CIRCUIT_DISCONNECT_ERR://管路未连接
                case FRESH_VALVE_LEAK_ERR://比例阀或新鲜气体阀漏气故障
                    Error_Handing_L(return_number);//error_number  
                    Disp_Result(2,RESULT_FAULT);
                    Disp_Dialog_Manual_Fail(return_number);
                    Test_NextItemFlag = 0;
                    break;
                }
            }
            else if((return_number & 0x8000) == 0)
            {
                //用于显示手动泄漏量
				//return_double = return_number/10.0;//低16位
    //            Disp_Result(4,System_Display[STR_Unit_mL_min],return_double,0);//正常通过颜色
                Disp_Result(2,RESULT_SUCCESS);
            }
            Save_ErrorNumber_Reault(MEM_MANUAL_OFFSET_ADDR,&(CONTEXT_APP.Current_ErrorCode),return_number);
            if(Test_NextItemFlag)
            {
                Context_NextLeft();//检测下一项
            }
        }
    } 
    CONTEXT_APP.Result = 0;
	return 0;
}



