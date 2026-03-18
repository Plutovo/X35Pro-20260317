#include "system_app.h"

U8 CircuitCompliance_Test(U8 SendCommand)
{
	U32 return_number = 0;
	double	return_double = 0;
    U8 Test_NextItemFlag = 1;//进行下一步的标志，只有进行错误提示的时候和重新检测的时候不会进行下一步
    
	//Disp_CheckItem_Text(Context_GetPosition());//显示当前检测项目名称

    if(SendCommand)
    {
        Serial_Communaction_Handing(S_SEND_TO_BDU_CC);
		TimeOutChech_Init(CC_TIMEOUT_NUM);
    }
    else 
    {
        if(CONTEXT_APP.Global_CancelFlag == 1)
        {
            Disp_Result(2,RESULT_SKIP);
            Context_NextRight();//检测下一项
        }
        else if(CONTEXT_APP.Result)
        {
            return_number = CONTEXT_APP.Result;
            if(return_number & 0x8000)
            {
                return_number &= 0x7fff;
                switch(return_number)
                {
                case COMMUNICATION_TIMEOUT_ERR:
                    Error_Handing_H(COMMUNICATION_TIMEOUT_ERR);
                    Disp_Result(2,RESULT_FAULT);
                    CONTEXT_APP.Global_CancelFlag = 1;//超时，跳过剩下的自检项目
                    break;
                case CIRCUIT_DISCONNECT_ERR://管路未连接故障； 手动、顺应性、泄露检测
                case CIRCUIR_COM_ERR://回路顺应性,顺应性测试错误
                    Error_Handing_L(LEAKAGE_RATE_HIGH);
                    Disp_Result(2,RESULT_FAULT);
                    Disp_Dialog_PipeCompliance(return_number);
                    Test_NextItemFlag = 0;
                    break;
                }
            }
            else if((return_number & 0x8000) == 0)
            {	//正确
                return_double = return_number/10.0;//低16位
      		    Disp_Result(4,Sys_Lib_GetDispStr(DP_MLCMH2O),return_double,0);
		    }
            Save_ErrorNumber_Reault(MEM_COMPLIANCE_OFFSET_ADDR,&(CONTEXT_APP.Current_ErrorCode),return_number);
            if(Test_NextItemFlag)
            {
                Context_NextRight();//检测下一项
            }
        }
    }
    return 0;
}