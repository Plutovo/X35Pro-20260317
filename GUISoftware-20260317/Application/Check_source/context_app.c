#include "context_app.h"
//#include "Debug_App.h"

CONTEXT_S CONTEXT_APP = {0};//有问题，没有初始化

//需要处理的事件
#define _CONTEXT_LEFT_POS      0
#define _CONTEXT_RIGHT_POS     1
#define _CONTEXT_RETEST_ITEM   2

ITEM_PROMPT  Item_Prompt = {0};
extern U8 OxygenSensorCalPromptFlag;//氧浓度传感器检测提示标志位
void TimeOutChech_InitAll(void);


/***********************************************************
 *函数名：Context_SetControlFlag
 *参数  ：Control_Flag：CONTEXT_APP.Checking_Control类型
 *返回值：CONTEXT_APP.Checking_Control设置之前的值
 *被调用位置：所有需要设置CONTEXT_APP.Checking_Control的位置
 *功能  ：设置CONTEXT_APP.Checking_Control为Control_Flag类型
************************************************************
*/
ControlFlag Context_SetControlFlag(ControlFlag Control_Flag)
{
    ControlFlag OldControl = Control_Flag;
    CONTEXT_APP.Checking_Control = Control_Flag;
    return OldControl;
}

/***********************************************************
 *函数名：Context_SetUpdateFlag
 *参数  ：Update_Flag：CONTEXT_APP.Update_Flag类型
 *返回值：CONTEXT_APP.Update_Flag设置之前的值
 *被调用位置：所有需要设置CONTEXT_APP.Update_Flag的位置
 *功能  ：设置CONTEXT_APP.Update_Flag为Update_Flag类型
************************************************************
*/
int Context_SetUpdateFlag(int Update_Flag)
{
    int OldFlag = CONTEXT_APP.Update_Flag;

    CONTEXT_APP.Update_Flag = Update_Flag;

    return OldFlag;
}

/***********************************************************
 *函数名：Context_GetUpdateFlag
 *参数  ：无
 *返回值：Update_Flag的值
 *被调用位置：所有需要获取CONTEXT_APP.Update_Flag的位置
 *功能  ：设置CONTEXT_APP.Update_Flag为Update_Flag类型
************************************************************
*/
int Context_GetUpdateFlag(void)
{
    int Flag = CONTEXT_APP.Update_Flag;
    return Flag;
}

/***********************************************************
 *函数名：Context_SetPosition
 *参数  ：Pos：新的检测位置
 *返回值：设置前的Position值
 *功能  ：切换当前的检测位置。
************************************************************
*/
PositionFlag Context_SetPosition(PositionFlag Pos)
{
    PositionFlag OldPos = CONTEXT_APP.Checking_Position;
    CONTEXT_APP.Checking_Position = Pos;
    return OldPos;
}

PositionFlag Context_AddPosition()
{
    PositionFlag Pos = 0;
    Context_SetPosition(Context_GetPosition()+1);
    return Context_GetPosition();
}

/***********************************************************
 *函数名：Context_GetPosition
 *参数  ：无
 *返回值：当前的Position值
 *功能  ：获取当前的检测位置。
************************************************************
*/
PositionFlag Context_GetPosition(void)
{
    PositionFlag Pos = CONTEXT_APP.Checking_Position;
    return Pos;
}

/***********************************************************
 *函数名：__Context_NextPosition
 *参数  ：需要处理的事件
 *返回值：完成检测标志 0:未完成 1:完成
 *被调用位置：_Contex_PositionHandle。
 *功能  ：根据当前的Pos信息和事件，得到新的Pos信息。
************************************************************
*/
static U8 __Context_NextPosition(U8 Flag)
{
    U8 Test_FinishFlag = 0;
    switch(Context_GetPosition())
    {
    //进行下一步自检 
    case POS_Barometer://djq add 20160809
    case POS_Flow_S:
    case POS_Compliance:
    case POS_Circuit_Leakage:
//    case POS_FG_Flow_ZERO_S:
    case POS_GasSypply:
    //djq del 20160809
    //case POS_PEEP_Valve:
    //case POS_Safety_Valve:
    //case POS_Pop_off_Valve:
    //case POS_ACGO:
        Context_AddPosition();
        break;
    case POS_Pressure_S:
		if(CONTEXT_APP.O2TestPrompt_Flag == 1)	//显示氧浓度自检提示
		{
			Context_SetPosition(POS_ERROR);
			Context_SetControlFlag(Ctrl_AutoFinish1);//结束前半部分自检
		}
		else
		{
			Context_AddPosition();
		}
        break;
    case POS_Oxygen:
		Context_SetPosition(POS_ERROR);
		Context_SetControlFlag(Ctrl_AutoFinish2);//结束前半部分自检
        break;
    //刚开始自检，确定第一项检测的项目
    case POS_Battery:
		//保存自检结果
        Memory_Save_CheckResult(RIGHT_CHECKFLAG);
        //显示自检时间和状态统计
        CheckItem_TextSet(RIGHT_CHECKFLAG,DISPLAY_TITLE,0);//自检之后显示时间

        //清空标志
        Context_SetPosition(POS_ERROR);
        Context_SetNoneCheckFlag();//清除自检类型标志


        Context_SetControlFlag(Ctrl_AutoFinish3);//完成全部自动测试
        Test_FinishFlag = 1;
		break;
	case POS_Manual:
		//保存自检结果
        Memory_Save_CheckResult(LEFT_CHECKFLAG);
        //显示自检时间和状态统计
        CheckItem_TextSet(LEFT_CHECKFLAG,DISPLAY_TITLE,0);//自检之后显示时间

        Context_SetPosition(POS_ERROR);
        Context_SetNoneCheckFlag();//清除自检类型标志

        Context_SetControlFlag(Ctrl_ManualFinish);//完成手动测试
        Test_FinishFlag = 1;
		break;
    case POS_ERROR:
        if(Flag == _CONTEXT_LEFT_POS)
        {
//            CheckItem_TextSet(LEFT_CHECKFLAG,START_CHECK,0);//检测之前先清空屏幕
            Init_GlobalParameter(START_CHECK,LEFT_CHECKFLAG);//检测前初始化环境变量和按钮状态
            Context_SetPosition(POS_Manual);
            Context_SetLeftCheckFlag();//设置自检类型标志
			TimeOutChech_InitAll();
        }
        else if(Flag == _CONTEXT_RIGHT_POS)
        {
            //CheckItem_TextSet(RIGHT_CHECKFLAG,START_CHECK,0);//检测之前先清空屏幕
			//加一提示框，提示用户是否按已设置的气源开启状态测试
            if((CONTEXT_APP.Checking_Control == Ctrl_AutoPrompt1)
             ||(CONTEXT_APP.Checking_Control == Ctrl_AutoTest1))
            {
                Init_GlobalParameter(START_CHECK,RIGHT_CHECKFLAG);//检测前初始化环境变量和按钮状态
                Context_SetPosition(POS_Barometer);
            }
            else if((CONTEXT_APP.Checking_Control == Ctrl_AutoPrompt2)
                ||(CONTEXT_APP.Checking_Control == Ctrl_AutoTest2))
            {
                Context_SetPosition(POS_Oxygen);
            }
            else if((CONTEXT_APP.Checking_Control == Ctrl_AutoPrompt3)
                ||(CONTEXT_APP.Checking_Control == Ctrl_AutoTest3))
            {
                Context_SetPosition(POS_Compliance);
            }

            Context_SetRightCheckFlag();//设置自检类型标志
			TimeOutChech_InitAll();
        }
        break;
    default:
        break;
    }
    return Test_FinishFlag;
}

/***********************************************************
 *函数名：_Contex_PositionHandle
 *参数  ：需要处理的事件
 *返回值：完成检测标志 0:未完成 1:完成
 *被调用位置：Pos变量和Update变量需要更新的地方。
 *功能  ：根据当前的命令设置Pos和Update变量。
************************************************************
*/
static U8 _Contex_PositionHandle(U8 Flag)
{
    U8 Test_FinishFlag = 0;
    switch(Flag)
    {
    case _CONTEXT_LEFT_POS:
        if(__Context_NextPosition(Flag))
        {
            Test_FinishFlag = 1;  //校验完成
            Context_SetUpdateFlag(UPDATE_LEFT_FINISH);
        }
        else
        {
            Context_SetUpdateFlag(UPDATE_TEST_COMMAND);
        }
        break;
    case _CONTEXT_RIGHT_POS:
        if(__Context_NextPosition(Flag))
        {
            Test_FinishFlag = 1;  //校验完成
            Context_SetUpdateFlag(UPDATE_RIGHT_FINISH);
        }
        else
        {
            Context_SetUpdateFlag(UPDATE_TEST_COMMAND);
        }
        break;
    case _CONTEXT_RETEST_ITEM://重新检测，不需要重新设置Pos变量 
        Context_SetUpdateFlag(UPDATE_TEST_COMMAND);
        break;
    }
    return Test_FinishFlag;
}

/***********************************************************
 *函数名：Context_NextLeft
 *参数  ：无
 *返回值：无
 *被调用位置：左侧自检，需要检测下一项目的地方。
 *功能  ：设置环境变量，检测下一个项目
************************************************************
*/
void Context_NextLeft(void)
{
    _Contex_PositionHandle(_CONTEXT_LEFT_POS);//判断测试是否完成
}
/***********************************************************
 *函数名：Context_NextRight
 *参数  ：无
 *返回值：无
 *被调用位置：右侧自检，需要检测下一项目的地方。
 *功能  ：设置环境变量，检测下一个项目
************************************************************
*/
void Context_NextRight(void)
{
    _Contex_PositionHandle(_CONTEXT_RIGHT_POS);//判断测试是否完成
}
/***********************************************************
 *函数名：Context_RetestItem
 *参数  ：无
 *返回值：无
 *被调用位置：需要重新检测当前项目的地方。
 *功能  ：设置环境变量，重新检测当前项目
************************************************************
*/
void Context_RetestItem(void)
{
    _Contex_PositionHandle(_CONTEXT_RETEST_ITEM);
}

/***********************************************************
        ActiveWindow_Flag变量相关函数
************************************************************
*/
void Context_SetWindow_None(void)
{
    CONTEXT_APP.ActiveWindow_Flag = ACTIVEWINDOW_NONE;
}

void Context_SetWindow_Show(void)
{
    CONTEXT_APP.ActiveWindow_Flag = ACTIVEWINDOW_SHOW;
}

void Context_SetWindow_Hide(void)
{
    CONTEXT_APP.ActiveWindow_Flag = ACTIVEWINDOW_HIDE;
}

U8   Context_IsNodeWindow(void)
{
    return (CONTEXT_APP.ActiveWindow_Flag == ACTIVEWINDOW_NONE)?1:0;
}

U8   Context_IsShowWindow(void)
{
    return (CONTEXT_APP.ActiveWindow_Flag == ACTIVEWINDOW_SHOW)?1:0;
}

U8   Context_IsHideWindow(void)
{
    return (CONTEXT_APP.ActiveWindow_Flag == ACTIVEWINDOW_HIDE)?1:0;
}



void Context_SetNoneCheckFlag(void)
{
    CONTEXT_APP.LeftOrRight_Flag = NONE_CHECK_FLAG;
}

void Context_SetLeftCheckFlag(void)
{
    CONTEXT_APP.LeftOrRight_Flag = LEFT_CHECK_FLAG;
}

void Context_SetRightCheckFlag(void)
{
    CONTEXT_APP.LeftOrRight_Flag = RIGHT_CHECK_FLAG;
}

U8   Context_IsNoneCheckFlag(void)
{
    return (CONTEXT_APP.LeftOrRight_Flag == NONE_CHECK_FLAG)?1:0;
}

U8   Context_IsLeftCheckFlag(void)
{
    return (CONTEXT_APP.LeftOrRight_Flag == LEFT_CHECK_FLAG)?1:0;
}

U8   Context_IsRightCheckFlag(void)
{
    return (CONTEXT_APP.LeftOrRight_Flag == RIGHT_CHECK_FLAG)?1:0;
}

//超时报错
void TimeOutErr(void)
{
    CONTEXT_APP.Result = COMMUNICATION_TIMEOUT_ERR | 0x8000;//报出超时错误
    if(Context_IsLeftCheckFlag())
    {
        Context_SetUpdateFlag(UPDATE_LEFT_POS);
    }
    else if(Context_IsRightCheckFlag())
    {
        Context_SetUpdateFlag(UPDATE_RIGHT_POS);
    }
}
//超时检测函数
void TestTimeOut_Check(unsigned char InitFlag,int Step)
{
    static int Cnt = -1;//单位为ms
    static char TimeOutCnt = 0;//当前项目超时的次数。
    static PositionFlag OldPos = POS_ERROR;
    if(InitFlag == 1)
    {
        Cnt = 0;//初始化后CNT 为 0
    }
    else if(InitFlag == 2)
    {
        Cnt = 0;//初始化后CNT 为 0
        OldPos = POS_ERROR;
    }
    else
    {
        if(Cnt >= CONTEXT_APP.TimeOutNum)
        {
            if(OldPos != Context_GetPosition())
            {
                TimeOutCnt = 0;
            }
            TimeOutCnt++;
            if(TimeOutCnt >= COMMUNICATION_TIMEOUT_MAX)
            {
                TimeOutErr();
                TimeOutCnt = 0;
            }
            else
            {
                Context_RetestItem(); //重新检测
            }
            Cnt = -1;//完成后Cnt 为 -1
            OldPos = Context_GetPosition();
        }
        if(Cnt >= 0)
        {
            Cnt+=Step;
        }
    }
}
//超时检测函数初始化，一般用于通讯命令发送之后
void TimeOutChech_Init(int TimeOutCnt)
{
    CONTEXT_APP.TimeOutNum = TimeOutCnt;
    TestTimeOut_Check(1,0);
}
//每次自检开始之前进行检测
void TimeOutChech_InitAll(void)
{
    CONTEXT_APP.TimeOutNum = 0;
    TestTimeOut_Check(2,0);
}


void Context_SetO2PromptFlag(U8 flag)
{
	CONTEXT_APP.O2TestPrompt_Flag = flag;
}

U8 Context_GetO2PromptFlag(void)
{
	return CONTEXT_APP.O2TestPrompt_Flag;
}


/*
*{ Prompt Item
*/
void PromptSet_O2SenTestFlag(U8 data)
{
	Item_Prompt.O2SenTest_Flag = data;
}

U8 PromptGet_O2SenTestFlag(void)
{
	return Item_Prompt.O2SenTest_Flag;
}

void PromptSet_STUpdateFlag(U8 data)
{
	Item_Prompt.STUpdate_Flag = data;
}

U8 PromptGet_STUpdateFlag(void)
{
	return Item_Prompt.STUpdate_Flag;
}

void PromptSet_ALTO2Flag(U8 data)
{
	Item_Prompt.AltO2SW_Flag = data;
}

U8 PromptGet_ALTO2Flag(void)
{
	return Item_Prompt.AltO2SW_Flag;
}


unsigned short Context_GetResult(void)
{
	return CONTEXT_APP.Result;
}

/*
*} end Prompt Item
*/


