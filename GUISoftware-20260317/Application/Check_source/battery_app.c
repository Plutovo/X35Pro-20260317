#include "system_app.h"



/*
 *内部变量
 *
*/
#define BATTERY_TEST_WAIT           (0)   //等待检测电源初始状态，检测电源连接之后，跳转到BATTERY_TEST_DISCONNECTAC
#define BATTERY_TEST_DISCONNECTAC   (1)   //等待断开电源
#define BATTERY_TEST_CONNECTAC      (2)   //等待连接电源
#define BATTERY_TEST_SUCCESS        (3)   //
#define BATTERY_TEST_FAIL           (4)   //
#define BATTERY_TEST_SKIP           (5)   //
static U8 BatteryStepFlag = BATTERY_TEST_WAIT;//0:检测电池是否断开    1:检测电源是否连接     2:成功   3:失败    4:跳过


/*
 *函数声明
 *
*/
U8 _Battery_Voltage_Test(void);
extern Int8U CheckAcStatus(void);


extern REALTIME_PARA RealtimePara;


/**************************************************************************************
 *函数名：Battery_Test
 *参数  ：0:不初始化参数        1:初始化参数
 *返回值：
 *被调用位置：
 *功能  ：执行电池切换测试程序。如果出现错误，存储到ALARM LOG中，并显示到当前检测项目之后
		  接收到全局的取消命令之后能够直接退出
***************************************************************************************
*/
U8 Battery_Test(U8 SendCommand)
{
	U8 chose_number = 0xff;
//    if(SendCommand)
//    {
//        BatteryStepFlag = BATTERY_TEST_WAIT;
//    }
	//Disp_CheckItem_Text(Context_GetPosition());//显示当前检测项目名称

    if(CONTEXT_APP.Global_CancelFlag == 1)
    {
        Disp_Result(2,RESULT_SKIP);
        Context_NextRight();//检测下一项
    }
    else
    {
        if(BatteryStepFlag == BATTERY_TEST_WAIT)
        {
//            Disp_Dialog_Battery_DisconnectAC();
        }
        else if(BatteryStepFlag == BATTERY_TEST_DISCONNECTAC)
        {
            Disp_Dialog_Battery_DisconnectAC();
        }
		else if(BatteryStepFlag == BATTERY_TEST_CONNECTAC)
        {
            Disp_Dialog_Battery_ConnectAC();
        }
        else if(BatteryStepFlag >= BATTERY_TEST_SUCCESS)
        {
            if(BatteryStepFlag == BATTERY_TEST_SKIP)
            { //跳过检测
                Disp_Result(2,RESULT_SKIP);
            }
            else if(BatteryStepFlag == BATTERY_TEST_FAIL)
            { //检测失败
                Disp_Result(2,RESULT_FAULT);
            }
            else if(BatteryStepFlag == BATTERY_TEST_SUCCESS)
            {//检测成功
                Disp_Result(2,RESULT_SUCCESS);
            }
            Save_ErrorNumber_Reault(MEM_BATTERY_OFFSET_ADDR,&(CONTEXT_APP.Current_ErrorCode),BatteryStepFlag);

            Context_NextRight();
            Context_SetWindow_Hide();
            BatteryStepFlag = BATTERY_TEST_WAIT;//重新初始化变量
        }
    }
	return 0;
}
/*
 *函数名：_Battery_Voltage_Test
 *参数  ：无
 *返回值：0电源和电池同时供电
		  1只有电池供电
		  2只有电源供电
 *被调用位置：Battery_Test()
 *功能  ：电池和电源电压检测程序
*/
static U8 _Battery_Voltage_Test(void)
{
	//电池电压检测程序（电池电压必须一直在合理范围）
	//电源电压检测程序
    if(CheckAcStatus() == 0)
    {//交流电未供电
        return 1;//电池供电
    }
    else if(CheckAcStatus() == 1)
    {
        if(RealtimePara.BatteryVoltage <= 0)
        {
            return 2;  //电源供电
        }
        else 
        {
            return 0;  //双电源
        }
    }
}


U8 Battery_Handle(void)
{
    U8 flag = 0;
    if(BatteryStepFlag == BATTERY_TEST_WAIT)
    {
        if(!_Battery_Voltage_Test())
        {
            BatteryStepFlag = BATTERY_TEST_DISCONNECTAC;
        }
		else if(_Battery_Voltage_Test() == 1)
		{
		    BatteryStepFlag = BATTERY_TEST_CONNECTAC;
		}
		else
		{
		    BatteryStepFlag = BATTERY_TEST_DISCONNECTAC;
		}
		flag = 1;
    }
    else if(BatteryStepFlag == BATTERY_TEST_DISCONNECTAC)//等待断开电源
    {
        if(_Battery_Voltage_Test() == 1)
        {
            BatteryStepFlag = BATTERY_TEST_CONNECTAC;
            flag = 1;
        }
    }
    else if(BatteryStepFlag == BATTERY_TEST_CONNECTAC) //等待连接电源
    {
        if(!_Battery_Voltage_Test())
        {
            BatteryStepFlag = BATTERY_TEST_SUCCESS;
            flag = 1;
        }
    }
    return flag;
}

void Battery_TestSkip(void)
{
    BatteryStepFlag = BATTERY_TEST_SKIP;
    Context_RetestItem();
}

void Battery_TestFail(void)
{
    BatteryStepFlag = BATTERY_TEST_WAIT;//BATTERY_TEST_FAIL
    Context_RetestItem();
}
