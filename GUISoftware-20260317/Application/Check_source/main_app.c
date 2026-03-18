#include "GUI.h"

#include "system_app.h"

extern Int8U	Usart0_Tx_Buffer[4];
extern unsigned int O2Continue_Time;//djq add 20170325 顺磁氧存在的情况下，氧浓度在正常范围内的持续计数器
extern unsigned int O2Fail_Time;


int  My_GetTime(U8 disp_flag);

//SendCommand_Flag:   0:检测结果    1:发送命令
U8 SelfTest_Handle(U8 SendCommand_Flag)
{
    switch(Context_GetPosition())
    {
    case POS_Manual:
		Manual_Test(SendCommand_Flag);
        break;
    case POS_Barometer:
        Barometer_Test(SendCommand_Flag);
        break;
    case POS_GasSypply:
        AirSupply_Test(SendCommand_Flag);
        break;
    case POS_Flow_S:
		FlowSensor_Test(SendCommand_Flag);
        break;
    case POS_Pressure_S:
        PressureSensor_Test(SendCommand_Flag);
        break;
  //  case POS_FG_Flow_ZERO_S:
		//FGFlowSensorZero_Test(SendCommand_Flag);
  //      break;
    case POS_Compliance:
        CircuitCompliance_Test(SendCommand_Flag);
        break;
    case POS_Circuit_Leakage:
        CircuitLeakage_Test(SendCommand_Flag);
        break;
    case POS_Oxygen:
        OxygenSensor_Test(SendCommand_Flag);
        break;
    case POS_Battery:
        Battery_Test(SendCommand_Flag);
        break;
    default:
        break;
    }
}

int SelfTest_App(void)
{
    int return_value = 0;
	static unsigned int Cnt = 0;
    static ControlFlag OldControl = Ctrl_Choose;
    char Str[100] = {0}, tmp_FiO2Val = 0;
	if(Context_IsHideWindow())//如果设置了隐藏标志，则隐藏显示的提示框。
    {
        if(WM_IsVisible(Dialog_OneButton_Prompt_HANDLE))
        {
            WM_HideWindow(Dialog_OneButton_Prompt_HANDLE);
        }
        if(WM_IsVisible(Dialog_TwoButton_Prompt_HANDLE))
        {
            WM_HideWindow(Dialog_TwoButton_Prompt_HANDLE);
        }
        
        Context_SetWindow_None();
    }
    //判断是否切换状态，如果切换，则刷新界面。  djq add 20161020
    if(CONTEXT_APP.Checking_Control != OldControl)//过程控制
    {
        WM_SendMessageNoPara(Desktop_Button_HANDLE,WM_INIT_DIALOG);

        switch(CONTEXT_APP.Checking_Control)
        {
        case Ctrl_Choose://隐藏Total 显示并且初始化Left和Right
            WM_HideWindow(CheckItem_Total_HANDLE);

            WM_ShowWindow(CheckItem_Left_HANDLE);
            WM_ShowWindow(CheckItem_Right_HANDLE);
            WM_SendMessageNoPara(CheckItem_Left_HANDLE,WM_INIT_DIALOG);
            WM_SendMessageNoPara(CheckItem_Right_HANDLE,WM_INIT_DIALOG);
            break;
        case Ctrl_ManualPrompt://显示并且初始化Total   隐藏Left和Right
        case Ctrl_ManualTest:  //显示并且初始化Total   隐藏Left和Right
        case Ctrl_ManualFinish://显示并且初始化Total   隐藏Left和Right
        case Ctrl_AutoPrompt1://显示并且初始化Total   隐藏Left和Right
        case Ctrl_AutoTest1:  //显示并且初始化Total   隐藏Left和Right
        case Ctrl_AutoFinish1://显示并且初始化Total   隐藏Left和Right
        case Ctrl_AutoPrompt2://显示并且初始化Total   隐藏Left和Right
        case Ctrl_AutoTest2:  //显示并且初始化Total   隐藏Left和Right
        case Ctrl_AutoFinish2://显示并且初始化Total   隐藏Left和Right
        case Ctrl_AutoPrompt3://显示并且初始化Total   隐藏Left和Right
        case Ctrl_AutoTest3:  //显示并且初始化Total   隐藏Left和Right
        case Ctrl_AutoFinish3://显示并且初始化Total   隐藏Left和Right
            WM_HideWindow(CheckItem_Left_HANDLE);
            WM_HideWindow(CheckItem_Right_HANDLE);

            WM_ShowWindow(CheckItem_Total_HANDLE);
            WM_SendMessageNoPara(CheckItem_Total_HANDLE,WM_INIT_DIALOG);
            break;
        }                     

    }
    OldControl = CONTEXT_APP.Checking_Control;

    //如果全局Cancel标志位被置位，那么跳过剩余所有的检测
    if(CONTEXT_APP.Global_CancelFlag == 1)
    {
        if(Context_IsLeftCheckFlag())
        {
            Context_SetUpdateFlag(UPDATE_LEFT_POS);
        }
        else if(Context_IsRightCheckFlag())
        {
            //if(Context_GetPosition() != POS_FG_Flow_ZERO_S)//EGM Test
            {
                Context_SetUpdateFlag(UPDATE_RIGHT_POS);
            }
        }
    }
    //校验命令发送
    if(Context_GetUpdateFlag())
    {
        switch(Context_GetUpdateFlag())
        {
        case UPDATE_LEFT_POS:
            Context_SetUpdateFlag(UPDATE_NONE_ITEM);//先清空标志
            SelfTest_Handle(0);//接收到BDU数据后分析结果
            break;
        case UPDATE_RIGHT_POS:
            Context_SetUpdateFlag(UPDATE_NONE_ITEM);;//先清空标志
            SelfTest_Handle(0);//接收到BDU数据后分析结果
            break;
        case UPDATE_TEST_COMMAND:
            Context_SetUpdateFlag(UPDATE_NONE_ITEM);//先清空标志
            SelfTest_Handle(1);//发送命令
            break;
        case UPDATE_LEFT_FINISH:
        case UPDATE_RIGHT_FINISH:
            Context_SetUpdateFlag(UPDATE_NONE_ITEM);//先清空标志
            break;
        }
    }
    if((!CONTEXT_APP.Pause_Flag)&&(Context_GetPosition() == POS_Battery))
    {
        if(Battery_Handle())
        {
            Context_SetUpdateFlag(UPDATE_RIGHT_POS);
        }
    }
    
    //超时检测
    if((!CONTEXT_APP.Pause_Flag)&&(Context_GetPosition() != POS_Battery))
    {
        TestTimeOut_Check(0,60);
    }

	//zkq add 20171214如果开通了顺磁氧，则UI使用氧浓度来判断，持续30s，氧浓度保持在95~100，则通过
	if((Context_GetPosition() == POS_Oxygen) && (Drager_EtCO2_Config_On == 2 || MasimoISA_EtCO2_Config_On == 2)) //ISA+O2
	{
//		tmp_FiO2Val = Im_getSlowData(IM_GET_INSP, IM_GAS_O2);
        tmp_FiO2Val = AA_all_data.AA_detail.AA_O2.FI;
		SendOperationMessage(SET_AA_PUMPON,USART2);//有顺磁氧开启泵
		//正常范围
		if((tmp_FiO2Val > 95) && (tmp_FiO2Val < 105))
		{
			O2Continue_Time++;
			O2Fail_Time = 0;
			if(O2Continue_Time > 200)//接近10s
			{
				Test_RXD_Handle(0,S_RECV_OF_BDU_OXYGEN,USART0);//模拟氧浓度接收成功
			}
		}
		else
		{
			O2Continue_Time = 0;
			O2Fail_Time++;
			if(O2Fail_Time > 180*6)
			{
				Test_RXD_Handle((0x8000|O2_SENSOR_FAILURE),S_RECV_OF_BDU_OXYGEN,USART0);//模拟氧浓度接收失败
			}
		}
	}

    if(Cnt%8 == 0)
    {
        Disp_Cheaking();	//显示正在检测的标志
    }
    Cnt++;
    return return_value;
}
/*
*时间调度函数：由于不能使用中断，所以这个函数需要加在运行时间较长的函数中（特别是循环函数）
*显示一些实时变化的内容
*disp_flag==2 只刷新显示，不检测跳过标志
*disp_flag==1 刷新显示，并且检测跳过标志
*disp_flag==0 只检测跳过标志
*/	
#define  step_disp_result  500//检测项目闪烁时间
#define  step_refresh      50//刷新屏幕时间

int My_GetTime(U8 disp_flag)
{
	static long now_time = 0;
	static U32  refresh_tiem = 0;
	
	if(disp_flag)
    {
		if(refresh_tiem == 0)
        {
			refresh_tiem = GUI_GetTime();
		}
		if(now_time == 0)
        {
			now_time = GUI_GetTime();
		} 
		if((GUI_GetTime() - now_time) >= step_disp_result)
        {
			now_time = 0;
			Disp_Cheaking();	//显示正在检测的标志
		}
		if((GUI_GetTime() - refresh_tiem) >= step_refresh)
        {
			GUI_Exec();
		}
	}
	if((CONTEXT_APP.Callback_Return == DESKTOP_BUTTON_CANCLE_ID) && (disp_flag != 2))
    {
            CONTEXT_APP.Callback_Return = 0xff;
            CONTEXT_APP.Cancel_Flag = 1;
            return 0x7fffffff;
	}
    else 
    {
        return GUI_GetTime();
    }
}