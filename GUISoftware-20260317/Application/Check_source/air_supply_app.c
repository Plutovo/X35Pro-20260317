#include "system_app.h"

U8 AirSupplyTestResultFlag = 0;//气源检测项结果标志  成功为1，失败为0
/******************************************************************
 *函数名：AirSypply_Test
 *参数  ：无
 *返回值：产生警告和故障的总数
 *被调用位置：
 *功能  ：执行气体供应测试程序。如果出现错误，存储到ALARM LOG中，
		 并显示到当前检测项目之后
*******************************************************************
*/
U8 AirSupply_Test(U8 SendCommand)/*显示两路气源   桌面排列显示啊？   现在只显示一路空气气源*/
{
	U32 return_number = 0;
    U8 Test_NextItemFlag = 1;//进行下一步的标志，只有进行错误提示的时候和重新检测的时候不会进行下一步
    U8  choose_flag = 1;
	//等待连接管路
    if(SendCommand)
    {
        Serial_Communaction_Handing(S_SEND_TO_BDU_AS_O2);
		TimeOutChech_Init(AIR_SUPPLY_TIMEOUT_NUM);
    }
    else
    {
        if(CONTEXT_APP.Global_CancelFlag == 1)//一共3个检测项目，所以跳过也应该有3个
        {
            Disp_Result(2,RESULT_SKIP);
            Save_ErrorNumber_Reault(MEM_GASO2_OFFSET_ADDR,&(CONTEXT_APP.Current_ErrorCode),return_number);
            Context_NextRight();//检测下一项
        }
        else //if(CONTEXT_APP.Result)
        {
#ifdef _SELF_TEST
			return_number = 0;
#else
            return_number = CONTEXT_APP.Result;
#endif
            if(return_number & 0x8000)//错误
            {
                return_number &= 0x7fff;
                if(return_number == COMMUNICATION_TIMEOUT_ERR)
                {//先判断 O2 是否超时
                    Error_Handing_H(COMMUNICATION_TIMEOUT_ERR);
                    Disp_Result(2,RESULT_FAULT);
					CONTEXT_APP.Global_CancelFlag = 1;//超时，跳过剩下的自检项目
                }
                else 
                {
                    Error_Handing_L(PRESSURE_OF_O2_SUPPLY_OVERFLOW_ERR_L);
                    Disp_Result(4,Sys_Lib_GetDispStr(DP_KPA),return_number,1);
                    Disp_Dialog_AirSupply(~0x2);//报出氧气故障，其他两路暂时不报
                    Test_NextItemFlag = 0;
                }
                AirSupplyTestResultFlag = 0;//气源检测失败
            }
            else if((return_number & 0x8000) == 0)
            {//正常
                Disp_Result(4,Sys_Lib_GetDispStr(DP_KPA),return_number,0);
                AirSupplyTestResultFlag = 1;//气源检测成功
            }
            Save_ErrorNumber_Reault(MEM_GASO2_OFFSET_ADDR,&(CONTEXT_APP.Current_ErrorCode),return_number);
            if(Test_NextItemFlag)
            {
                Context_NextLeft();//检测下一项
            }
        }
    }
	return 0;
}

