#include "display_app.h"

extern U8 OxygenSensorCalPromptFlag;//氧浓度传感器检测提示标志位
/*****************************************************************
*检测内容显示
******************************************************************
*/
//检测项目显示字符串
U8 Desktop_CheckItem_Text[CHECKITEM_NUM][60] = {0};//30
//检测结果显示控件ID
U16 CheckItem_Result_Id[TOTAL_TEXT_MAX_NUM] =
{
    TOTAL_TEXT_00_ID,       TOTAL_TEXT_01_ID,      TOTAL_TEXT_02_ID,        TOTAL_TEXT_03_ID,        TOTAL_TEXT_04_ID,
    TOTAL_TEXT_05_ID,       TOTAL_TEXT_06_ID,      TOTAL_TEXT_07_ID,        TOTAL_TEXT_08_ID,        TOTAL_TEXT_09_ID,   
    TOTAL_TEXT_10_ID,       TOTAL_TEXT_11_ID,      TOTAL_TEXT_12_ID,        TOTAL_TEXT_13_ID,    
};
//djq del 20160809 修改自检项目
const U16 CheckItem_Result_Id_Init[CHECKITEM_NUM] = 
{
    MANUAL_RESULT_ID,      BAROMETER_RESULT_ID,    GAS_SUPPLY_ID,           FLOW_SENSOR_ID,
    PRESSURE_SENSOR_ID,    FG_FLOW_SENSOR_ID,      OXYGEN_SENSOR_ID,
    COMPLIANCE_ID,         INTERNAL_LEAKAGE_ID,    BATTERY_ID
};
U8 Checking_Show_Flag = 0;//Checking显示标志
//extern GUI_FONT	GUI_FontHZ_Arial_14;


/***********************************************************
 *函数名：Disp_ClearGasPrompt
 *参数  ：无
 *返回值：无
 *调用位置：气源重新检测对话框
 *功能  ：重新检测气源的时候清除上一次的气源信息。
 ***********************************************************
*/
void Disp_ClearGasPrompt(void)
{
	static U32 Cnt = 1;
    WM_HWIN hWin = 0;
    U16 Disp_ID = 0;
    U8 Gas_Type = 0;
    Disp_ID = CheckItem_Result_Id[(POS_GasSypply) - 1] + 1;
    Gas_Type = 1;
    for(Cnt = 0;Cnt < 4+Gas_Type;Cnt++)
    {
        hWin = WM_GetDialogItem(CheckItem_Total_HANDLE,Disp_ID + Cnt);
        if(WM_IsWindow(hWin))
        {
            TEXT_SetTextColor(hWin,CheckResult_Font_Color);//默认使用通过的颜色
            TEXT_SetText(hWin,"");
        }
    }
//    Result_DispPrompt(hWin,Disp_ID,0,System_Display[ STR_AIR ],System_Display[STR_Empty]);//DISP_SELF_GAS_LOW
    //Result_DispClear(hWin,Disp_ID,Gas_Type);
}


/***********************************************************
 *函数名：Disp_Cheaking
 *参数  ：显示	"正在检测......"
 *返回值：无
 *调用位置：定时器中断中显示
 *功能  ：
 ***********************************************************
*/
void Disp_Cheaking(void)
{
	static U32 cnt = 1;
    WM_HWIN hWin = 0;
    U16 Disp_ID = 0;
    U8 Gas_Type = 0;
    if(Context_GetPosition() != POS_ERROR)
    {
        //djq modify 20160809
        if(Context_GetPosition() < POS_Barometer)
        {
            Disp_ID = CheckItem_Result_Id[Context_GetPosition()] + 1;
        }
        else
        {
            Disp_ID = CheckItem_Result_Id[Context_GetPosition() - 1] + 1;
        }
        

		hWin = CheckItem_Total_HANDLE;

        if(WM_IsWindow(hWin))
        {
            if(Context_IsNodeWindow())//弹出对话框的时候关闭闪烁
			//if(Checking_Show_Flag)
            {
                Result_DispClear(hWin,Disp_ID,Gas_Type);//djq add 20161025每次显示前都清除，防止同时显示。
                if(cnt++%2 == 0)
                {
                    Result_DispTesting(hWin,Disp_ID,Gas_Type);
                }
                else 
                {
//                    Result_DispClear(hWin,Disp_ID,Gas_Type);//djq del 20161025
                }
            }
        }
    }
}

/***********************************************************
 *函数名：Disp_Result
 *参数  ：要显示的结果的类型（数据型，结果型），
		  数据型：共4个参数，第2个为结果的单位{string},第3个为结果的数值{U32 or double}，
                  第4个为是否正常通过（决定颜色）0为正常，1为非正常
				  
		  结果型：一共两个参数，第二个参数为检测结果的状态：通过、跳过、忽略、失败...
 *返回值：无
 *调用位置：Internal_Test()
 *功能  ：内部检测结束后，进行所有的故障处理，决定是否继续运行
 ***********************************************************
*/
#define StringEmpty "                            "
void Disp_Result(U8 flag, ...)
 {
	va_list ap;
	double result_double = 0;
	U32	   result_U32	 = 0;
	U8 *str = NULL;			//显示字符或者显示单位
	U8 result_flag  = 1;	//4个参数时，作为字体颜色标志,也是成功与否的标志，成功，则等于零，并且清除产生的errorcode
	U8 str_num = 0;			//4个参数时，存储字符串的长度
	U8 pos_cnt = 0;			//4个参数时，记录显示BUFFER的当前位置
    WM_HWIN hWin;           //当前操作的Text句柄
    U16 Result_ID = 0;      //当前操作的Text ID
    U8 Gas_Type = 0,cnt = 0;
    char EGMErrNum[30] = {0};
    char tmp_str[10];
    char string[35] = {StringEmpty};
//    unsigned char symbol[2] = 0;
        
    GUI_COLOR  old_color = GUI_GetColor();        
	va_start(ap,flag);

	if(CONTEXT_APP.Checking_Control != Ctrl_Choose)//djq tab 20160808  区分当前是刷新结果界面还是检测界面
	{
	    hWin = CheckItem_Total_HANDLE;
		if(Context_GetPosition() < POS_Barometer)
        {
            Result_ID = CheckItem_Result_Id[Context_GetPosition()] + 1;
        }
        else 
        {
            Result_ID = CheckItem_Result_Id[Context_GetPosition() - 1] + 1;
        }
	}
	else
	{
        if(Context_GetPosition() < CONTEXT_APP.Left_CheckItem_Num )
        {
            hWin = CheckItem_Left_HANDLE;
        }
        else
        {
            hWin = CheckItem_Right_HANDLE;
        }

	    Result_ID = CheckItem_Result_Id_Init[Context_GetPosition()] + 1;
	}


	if(flag == 2)
    {
		result_flag = va_arg(ap,U8);
        CONTEXT_APP.Checking_State = result_flag;//获取状态编号

        Result_DispState(hWin,Result_ID,Gas_Type,CONTEXT_APP.Checking_State,Sys_Lib_GetDispStr(DP_EMPTY),CONTEXT_APP.Current_ErrorCode);//√  Pass  0xXX
	}
    else if(flag == 4)
    {
		//获得单位标志，区分出是哪个检测项目，选择证书转换或double
		str = va_arg(ap,U8 *);
		str_num = strlen(str);
		//获得单位相应的数值
		if(!strcmp(str,Sys_Lib_GetDispStr(DP_MMHG))||!strcmp(str,Sys_Lib_GetDispStr(DP_MMHG)))
        {	//气压计压力
			result_U32 =va_arg(ap,U32);
            //取得是否正常通过的标志，设定字体和颜色
    		result_flag = va_arg(ap,U8);
            sprintf(string,"%-4d",result_U32);
		}
		else if(!strcmp(str ,Sys_Lib_GetDispStr(DP_KPA)) || !strcmp(str ,Sys_Lib_GetDispStr(DP_PSI)))
        {	//气源压力
			result_U32 = va_arg(ap,U32);
            //取得是否正常通过的标志，设定字体和颜色
    		result_flag = va_arg(ap,U8);
            if(Context_GetPosition() == POS_GasSypply)
            { 
                if(result_U32 < 20)//如果气源压力低于20kPa则认为无气源，所以显示---
                {
                    sprintf(string,"---");
                }
                else
                {
                    sprintf(string,"%-3d",result_U32);
                }
            }
            else
            {
	            sprintf(string,"%-3d",result_U32);
            }
		}
        else if(!strcmp(str ,Sys_Lib_GetDispStr(DP_CMH2O))||!strcmp(str ,Sys_Lib_GetDispStr(DP_MBAR)))
        {
            result_U32 = va_arg(ap,U32);
            result_flag = va_arg(ap,U8);
            sprintf(string,"%-3d",result_U32);
        }
        else if(!strcmp(str ,Sys_Lib_GetDispStr(DP_MLMIN)))
        {	//内部泄露量
			result_double = va_arg(ap,double);
            //取得是否正常通过的标志，设定字体和颜色
    		result_flag = va_arg(ap,U8);
	        sprintf(string,"%-2.1f",result_double);
		}
        else if(!strcmp(str ,Sys_Lib_GetDispStr(DP_MLCMH2O))||!strcmp(str ,Sys_Lib_GetDispStr(DP_MLMBAR)))//两个单位近似相等，不转换
        {	//管路顺应性
			result_double = va_arg(ap,double);
            //取得是否正常通过的标志，设定字体和颜色
    		result_flag = va_arg(ap,U8);
	        sprintf(string,"%-2.1f",result_double);
		}		

        CONTEXT_APP.Checking_State = (result_flag << 2) | 0x1;    //
        Result_DispValue(hWin,Result_ID,Gas_Type,CONTEXT_APP.Checking_State,Sys_Lib_GetDispStr(DP_EMPTY),string,str,CONTEXT_APP.Current_ErrorCode); 

//		TEXT_SetFont(hWin,&GUI_Font_16);
		//for(pos_cnt = 0;pos_cnt < Desktop_Text_Size;pos_cnt++)
  //      {//去掉数值字符串后面的结束符
		//	if(string[pos_cnt]== '\0')
  //          {
		//		string[pos_cnt] = ' ';
		//	}
		//}
		//strcpy(&string[14],str);//将单位加入到显示buffer中
//		string[Desktop_Text_Size-1] = '\0';
	}
	for(pos_cnt = 0;pos_cnt < Desktop_Text_Size;pos_cnt++)
    {//去掉数值字符串后面的结束符
		if(string[pos_cnt]== '\0')
        {
			string[pos_cnt] = ' ';
		}
	}
    if(result_flag == 0)
    {
        CONTEXT_APP.Current_ErrorCode = 0;//例如：内部泄露检测。第一次检测出现 错误，产生errorcode，重新检测一次成功了，则需要清除。
    }

    if((CONTEXT_APP.Current_ErrorCode) && (flag == 2)
     &&(CONTEXT_APP.Checking_State == 0x04))//如果错误代码为0，则说明没有错误//数据类型的结果，暂时没有错误代码
    {
        switch(CONTEXT_APP.Current_ErrorCode)
        {
        case GAS_SUPPLY_FAILURE:
            Result_DispPrompt(hWin,Result_ID,0,Sys_Lib_GetDispStr(DP_EMPTY),Sys_Lib_GetDispStr(DP_EMPTY));//DISP_SFLF_GAS_SUPPLY_FAIL
            break;
        case GAS_SUPPLY_LOW:
            Result_DispPrompt(hWin,Result_ID,0,Sys_Lib_GetDispStr(DP_EMPTY),Sys_Lib_GetDispStr(DP_EMPTY));//DISP_SELF_GAS_LOW
            break;
        case SAFETY_VLVE_FAILURE:
        case F_SENSOR_INSP_TEST_ERR:
        case F_SENSOR_EXP_TEST_ERR:
        case P_SENSOR_TEST_ERR:
        case COMPLIANCE_HIGH:
            Result_DispPrompt(hWin,Result_ID,0,Sys_Lib_GetDispStr(DP_EMPTY),Sys_Lib_GetDispStr(DP_EMPTY));//DISP_SELF_TEST_FAIL
            break;
        case COMMUNICATION_TIMEOUT_ERR:
            //Result_DispPrompt(hWin,Result_ID,0,System_Display[STR_Empty],System_Display[STR_SelfTest_Err_Timeout]);//DISP_SELF_TIMEOUT
            /*WM_MoveTo(hWin,320,30);*/
			break;
        case FG_F_SENSOR_LINEAR_ERR://EGMErrNum
            //if(CONTEXT_APP.Result & 0x0001)
            //{
            //    sprintf(EGMErrNum, "%s", "0");
            //}
            //if(CONTEXT_APP.Result & 0x0002)
            //{
            //    sprintf(EGMErrNum, "%s%s", EGMErrNum," 1");
            //}
            //if(CONTEXT_APP.Result & 0x0004)
            //{
            //    sprintf(EGMErrNum, "%s%s", EGMErrNum," 2");
            //}
            //if(CONTEXT_APP.Result & 0x0008)
            //{
            //    sprintf(EGMErrNum, "%s%s", EGMErrNum," 3");
            //}
            //if(CONTEXT_APP.Result & 0x0010)
            //{
            //    sprintf(EGMErrNum, "%s%s", EGMErrNum," 4");
            //}
            //if(CONTEXT_APP.Result & 0x0020)
            //{
            //    sprintf(EGMErrNum, "%s%s", EGMErrNum," 5");
            //}
            //if(CONTEXT_APP.Result & 0x0040)
            //{
            //    sprintf(EGMErrNum, "%s%s", EGMErrNum," 6");
            //}
            //if(CONTEXT_APP.Result & 0x0080)
            //{
            //    sprintf(EGMErrNum, "%s%s", EGMErrNum," 7");
            //}
            //if(CONTEXT_APP.Result & 0x0100)
            //{
            //    sprintf(EGMErrNum, "%s%s", EGMErrNum," 8");
            //}
            //if(CONTEXT_APP.Result & 0x0200)
            //{
            //    sprintf(EGMErrNum, "%s%s", EGMErrNum," 9");
            //}
            //Result_DispPrompt(hWin,Result_ID,0,System_Display[],EGMErrNum);
            break;
       default:
            break;
        }


    }
	va_end(ap);
    GUI_SetColor(old_color);
}

/***********************************************************
 *函数名  ：Disp_Dialog_Barometer
 *标题    ：大气压力传感器测试失败
************************************************************
*/
void Disp_Dialog_Barometer()
{
    Context_SetWindow_Show();
    Dialog_Common_Init(Dialog_TwoButton_Prompt_HANDLE,Sys_Lib_GetDispStr(DP_SENSORFAIL),Sys_Lib_GetDispStr(DP_REPEAT),Sys_Lib_GetDispStr(DP_CONTINUE),0);
}

/***********************************************************
 *函数名  ：Disp_Dialog_CircuitFlowSensor
 *标题    ：气体供应测试
************************************************************
*/
void Disp_Dialog_CircuitFlowSensor(unsigned int flag)
{
    Context_SetWindow_Show();
    switch(flag)
    {
    case F_SENSOR_INSP_TEST_ERR://吸气流量传感器测试错误
        Dialog_Common_Init(Dialog_TwoButton_Prompt_HANDLE,Sys_Lib_GetDispStr(DP_INSPFLOWFAIL),Sys_Lib_GetDispStr(DP_REPEAT),Sys_Lib_GetDispStr(DP_CONTINUE),0);
        break;
    case F_SENSOR_EXP_TEST_ERR://呼气流量传感器测试错误
        Dialog_Common_Init(Dialog_TwoButton_Prompt_HANDLE,Sys_Lib_GetDispStr(DP_EXPFLOWFAIL),Sys_Lib_GetDispStr(DP_REPEAT),Sys_Lib_GetDispStr(DP_CONTINUE),0);
        break;
    case (F_SENSOR_INSP_TEST_ERR|F_SENSOR_EXP_TEST_ERR)://吸气流量传感器测试错误和呼气流量传感器测试错误
        Dialog_Common_Init(Dialog_TwoButton_Prompt_HANDLE,Sys_Lib_GetDispStr(DP_INSPFLOWSENSORANDEXP),Sys_Lib_GetDispStr(DP_REPEAT),Sys_Lib_GetDispStr(DP_CONTINUE),0);
        break;
    }
}

/***********************************************************
 *函数名  ：Disp_Dialog_PressureSensor_Fail
 *标题    ：压力传感器线性错误
************************************************************
*/
U8 Disp_Dialog_PressureSensor_Fail(void)
{
    Context_SetWindow_Show();
	Dialog_Common_Init(Dialog_TwoButton_Prompt_HANDLE,Sys_Lib_GetDispStr(DP_SENSORFAIL),Sys_Lib_GetDispStr(DP_REPEAT),Sys_Lib_GetDispStr(DP_CONTINUE),0);
}

/***********************************************************
 *函数名  ：Disp_Dialog_OxygenSensor
 *标题    ：气体供应测试
************************************************************
*/
void Disp_Dialog_OxygenSensor(unsigned int flag)
{
    Context_SetWindow_Show();
    switch(flag)
    {
    case GAS_SUPPLY_FAILURE://气源故障
        Dialog_Common_Init(Dialog_TwoButton_Prompt_HANDLE,Sys_Lib_GetDispStr(DP_O2SUPFAIL),Sys_Lib_GetDispStr(DP_REPEAT),Sys_Lib_GetDispStr(DP_CONTINUE),0);
        break;
    case O2_SENSOR_FAILURE://传感器故障
        Dialog_Common_Init(Dialog_TwoButton_Prompt_HANDLE,Sys_Lib_GetDispStr(DP_EXPFLOWFAIL),Sys_Lib_GetDispStr(DP_REPEAT),Sys_Lib_GetDispStr(DP_CONTINUE),0);
        break;
    case O2_SENSOR_INVALID://传感器失效或者未连接
        Dialog_Common_Init(Dialog_TwoButton_Prompt_HANDLE,Sys_Lib_GetDispStr(DP_SENSORLOSSEFFICACY),Sys_Lib_GetDispStr(DP_REPEAT),Sys_Lib_GetDispStr(DP_CONTINUE),0);
        break;
    }
}
/***********************************************************
 *函数名  ：Disp_Dialog_AirSupply
 *标题    ：气体供应测试
 *显示内容：请连接氧气气源、请链接空气气源、请链接氧气气源和空气气源
 *按钮    ：确定
 *返回值  ：无
 *调用位置：air_supply.c->AirSupply_Test()
 *功能  ：调用时显示该对话框，使用后隐藏
************************************************************
*/
void Disp_Dialog_AirSupply(unsigned char flag)
{
	Context_SetWindow_Show();
    if(flag == 0)//无气体
    {
        if(Context_GetPosition() == POS_GasSypply)
        {
	    	Dialog_Common_Init(Dialog_TwoButton_Prompt_HANDLE,Sys_Lib_GetDispStr(DP_GASSUPPLYFAIL),Sys_Lib_GetDispStr(DP_REPEAT),Sys_Lib_GetDispStr(DP_EXIT),0);
        }
        else if(Context_GetPosition() == POS_Pressure_S)
        {
	    	Dialog_Common_Init(Dialog_TwoButton_Prompt_HANDLE,Sys_Lib_GetDispStr(DP_GASSUPPLYFAIL),Sys_Lib_GetDispStr(DP_REPEAT),Sys_Lib_GetDispStr(DP_CONTINUE),0);
        }
    }
	else if((flag&0x03) == 0)//无空氧
	{
	    Dialog_Common_Init(Dialog_TwoButton_Prompt_HANDLE,Sys_Lib_GetDispStr(DP_O2AIRSUPFAIL),Sys_Lib_GetDispStr(DP_REPEAT),Sys_Lib_GetDispStr(DP_EXIT),0);
	}
	else if((flag&0x05) == 0)//无空笑
	{
	    Dialog_Common_Init(Dialog_TwoButton_Prompt_HANDLE,Sys_Lib_GetDispStr(DP_N2OAIRSUPPLYFAIL),Sys_Lib_GetDispStr(DP_REPEAT),Sys_Lib_GetDispStr(DP_CONTINUE),0);
	}
	else if((flag&0x06) == 0)//无氧笑
	{
	    Dialog_Common_Init(Dialog_TwoButton_Prompt_HANDLE,Sys_Lib_GetDispStr(DP_O2N2OSUPFAIL),Sys_Lib_GetDispStr(DP_REPEAT),Sys_Lib_GetDispStr(DP_CONTINUE),0);
	}
    else if((flag&0x02) == 0)//无氧气
    {
		Dialog_Common_Init(Dialog_TwoButton_Prompt_HANDLE,Sys_Lib_GetDispStr(DP_O2SUPFAIL),Sys_Lib_GetDispStr(DP_REPEAT),Sys_Lib_GetDispStr(DP_CONTINUE),0);
    }
    else if((flag&0x01) == 0)//无空气
    {
		Dialog_Common_Init(Dialog_TwoButton_Prompt_HANDLE,Sys_Lib_GetDispStr(DP_AIRSUPPLYFAIL),Sys_Lib_GetDispStr(DP_REPEAT),Sys_Lib_GetDispStr(DP_CONTINUE),0);
    }
	else if((flag&0x04) == 0)//无笑气
    {
		Dialog_Common_Init(Dialog_TwoButton_Prompt_HANDLE,Sys_Lib_GetDispStr(DP_O2N2OSUPFAIL),Sys_Lib_GetDispStr(DP_REPEAT),Sys_Lib_GetDispStr(DP_CONTINUE),0);
    }
}
/***********************************************************
 *函数名  ：Disp_Dialog_InternalLeak
 *标题    ：内部泄露测试
 *显示内容：使用一根管道连接在吸气口和呼气口之间
 *按钮    ：确定
 *返回值  ：无
 *调用位置：internal_leak.c->InternalLeak_Test()
 *功能  ：调用时显示该对话框，使用后隐藏
************************************************************
*/
U8 Disp_Dialog_Device_Start(void)
{
    WM_HWIN BUTTON_BATTERY,BUTTON_POWER,BUTTON_CENTER;
	CONTEXT_APP.Callback_Return = 0xff; 
    WM_ShowWindow(BUTTON_BATTERY);
    WM_ShowWindow(BUTTON_POWER);
    WM_HideWindow(BUTTON_CENTER);

    Context_SetWindow_Show();//设置显示窗口的标志
    return (CONTEXT_APP.Callback_Return ==TWO_BUTTON_YES_ID)?0:1;
}

/***********************************************************
 *函数名  ：Disp_Dialog_OxygenSensor_Prompt
 *标题    ：氧浓度传感器提示信息
 *显示内容：
 *按钮    ：重新检测-取消检测
 *返回值  ：重新检测-取消检测
 *调用位置：
 *功能  ：调用时显示该对话框，使用后隐藏
************************************************************
*/
U8 Disp_Dialog_OxygenSensor_Prompt(U8 FlagNum)
{
    Context_SetWindow_Show();
    switch(FlagNum)
    {
    case GAS_SUPPLY_FAILURE://气源压力故障
        Dialog_Common_Init(Dialog_TwoButton_Prompt_HANDLE,Sys_Lib_GetDispStr(DP_O2SUPFAIL),Sys_Lib_GetDispStr(DP_REPEAT),Sys_Lib_GetDispStr(DP_CONTINUE),0);
        break;
    case O2_SENSOR_INVALID://传感器失效或者未连接，更换氧浓度传感器
        Dialog_Common_Init(Dialog_TwoButton_Prompt_HANDLE,Sys_Lib_GetDispStr(DP_SENSORLOSSEFFICACY),Sys_Lib_GetDispStr(DP_REPEAT),Sys_Lib_GetDispStr(DP_CONTINUE),0);
        break;
    case O2_SENSOR_FAILURE://传感器故障
        Dialog_Common_Init(Dialog_TwoButton_Prompt_HANDLE,Sys_Lib_GetDispStr(DP_O2RETEST),Sys_Lib_GetDispStr(DP_REPEAT),Sys_Lib_GetDispStr(DP_CONTINUE),0);
        break;
    }
}
/***********************************************************
 *函数名  ：Disp_Dialog_PressureSensor_Fail
 *标题    ：新鲜气体流量传感器线性错误
 *显示内容：
 *按钮    ：重新检测-取消检测
 *返回值  ：重新检测-取消检测
 *调用位置：
 *功能  ：调用时显示该对话框，使用后隐藏
************************************************************
*/
U8 Disp_Dialog_FGFlowSensor_Fail(void)
{
	Dialog_Common_Init(Dialog_TwoButton_Prompt_HANDLE,Sys_Lib_GetDispStr(DP_SENSORFAIL),Sys_Lib_GetDispStr(DP_REPEAT),Sys_Lib_GetDispStr(DP_CONTINUE),0);
	Context_SetWindow_Show();
}

/***********************************************************
 *函数名  ：Disp_Dialog_PipeCompliance
 *标题    ：病人回路检测
 *显示内容：请检查病人呼吸回路的连接，以及当前顺应性值
 *按钮    ：重新检测-取消检测
 *返回值  ：重新检测-取消检测
 *调用位置：pipe_comliance.c->PipeComliance_Test()
 *功能  ：调用时显示该对话框，使用后隐藏
************************************************************
*/
U8 Disp_Dialog_PipeCompliance(U8 flag)
{
	Context_SetWindow_Show();
    switch(flag)
    {
    case CIRCUIT_DISCONNECT_ERR://管路未连接故障； 手动、顺应性、泄露检测
        Dialog_Common_Init(Dialog_TwoButton_Prompt_HANDLE,Sys_Lib_GetDispStr(DP_CIRCUITNOTCONN),Sys_Lib_GetDispStr(DP_REPEAT),Sys_Lib_GetDispStr(DP_CONTINUE),0);
        break;
    case CIRCUIR_COM_ERR://回路顺应性,顺应性测试错误
        Dialog_Common_Init(Dialog_TwoButton_Prompt_HANDLE,Sys_Lib_GetDispStr(DP_COMPLIANCEFAIL),Sys_Lib_GetDispStr(DP_REPEAT),Sys_Lib_GetDispStr(DP_CONTINUE),0);
        break;
    }
}
/***********************************************************
 *函数名  ：Disp_Dialog_Battery_DisconnectAC
 *标题    ：电池切换测试
 *显示内容：请拔出电源电缆
 *按钮    ：无
 *返回值  ：无
 *图标    ：电池和电源标志
 *调用位置：battery.c->Battery_Test();
 *功能  ：非阻塞创建对话框，并且显示
************************************************************
*/
void Disp_Dialog_Battery_DisconnectAC(void)
{
	strcpy(Callback_Struct.Title_String,Sys_Lib_GetDispStr(DP_BATTERY));
	Dialog_Common_Init(Dialog_OneButton_Prompt_HANDLE,Sys_Lib_GetDispStr(DP_DISCONNECTACPOWER),Sys_Lib_GetDispStr(DP_SKIP),Sys_Lib_GetDispStr(DP_SKIP),0);
	Context_SetWindow_Show();
}
/***********************************************************
 *函数名  ：Disp_Dialog_Battery_ConnectAC
 *标题    ：电池切换测试
 *显示内容：请插入电源电缆 Please connect AC power supply!
 *按钮    ：无
 *返回值  ：无
 *图标    ：电池和电源标志
 *调用位置：battery.c->Battery_Test();
 *功能  ：非阻塞创建对话框，并且显示
************************************************************
*/
void Disp_Dialog_Battery_ConnectAC(void)
{
	strcpy(Callback_Struct.Title_String,Sys_Lib_GetDispStr(DP_BATTERY));
	Dialog_Common_Init(Dialog_OneButton_Prompt_HANDLE,Sys_Lib_GetDispStr(DP_CONNECTACPOWER),Sys_Lib_GetDispStr(DP_SKIP),Sys_Lib_GetDispStr(DP_SKIP),0);
	Context_SetWindow_Show();
}

void Disp_Dialog_ACGO_Prompt(void)
{
	strcpy(Callback_Struct.Title_String, " ");
	Dialog_Common_Init(Dialog_OneButton_Prompt_HANDLE,Sys_Lib_GetDispStr(DP_SETACGOOFF),Sys_Lib_GetDispStr(DP_CONFIRM),Sys_Lib_GetDispStr(DP_CONFIRM),0);
	Context_SetWindow_Show();
}
/***********************************************************
 *函数名  ：Disp_Dialog_CircuitLeak_High
 *标题    ：内部泄露测试
 *显示内容：漏气过多，请检查各项连接。
 *按钮    ：重新检测-取消检测
 *返回值  ：重新检测-取消检测
 *调用位置：internal_leak.c->InternalLeak_Test()
 *功能  ：调用时显示该对话框，使用后隐藏
************************************************************
*/
void Disp_Dialog_CircuitLeak_High(U8 flag)
{
	Context_SetWindow_Show();
    switch(flag)
    {
    case CIRCUIT_DISCONNECT_ERR://管路未连接故障； 手动、顺应性、泄露检测
        Dialog_Common_Init(Dialog_TwoButton_Prompt_HANDLE,Sys_Lib_GetDispStr(DP_CIRCUITNOTCONN),Sys_Lib_GetDispStr(DP_REPEAT),Sys_Lib_GetDispStr(DP_CONTINUE),0);
        break;
    case FRESH_VALVE_LEAK_ERR://比例阀或新鲜气体阀漏气故障；手动、顺应性、泄露检测
        Dialog_Common_Init(Dialog_TwoButton_Prompt_HANDLE,Sys_Lib_GetDispStr(DP_INSPFLOWCONTROLVALVE),Sys_Lib_GetDispStr(DP_REPEAT),Sys_Lib_GetDispStr(DP_CONTINUE),0);
        break;
    case LEAKAGE_RATE_HIGH://泄露量大  :手动气密性、回路泄漏量
        Dialog_Common_Init(Dialog_TwoButton_Prompt_HANDLE,Sys_Lib_GetDispStr(DP_LEAKAGEOVERRANGE),Sys_Lib_GetDispStr(DP_REPEAT),Sys_Lib_GetDispStr(DP_CONTINUE),0);
        break;
    }
}


/***********************************************************
 *函数名  ：
 *标题    ：手动气密性测试
 *显示内容：漏气过多，请检查各项连接。
 *按钮    ：重新检测-取消检测
 *返回值  ：
 *调用位置：Manual_app.c->Manual_Test()
 *功能  ：调用时显示该对话框，使用后隐藏
************************************************************
*/
void Disp_Dialog_Manual_Fail(U8 flag)
{
    Context_SetWindow_Show();
    switch(flag)
    {
    case LEAKAGE_RATE_HIGH://泄露超范围
        Dialog_Common_Init(Dialog_TwoButton_Prompt_HANDLE,Sys_Lib_GetDispStr(DP_LEAKAGEOVERRANGE),Sys_Lib_GetDispStr(DP_REPEAT),Sys_Lib_GetDispStr(DP_CONTINUE),0);
        break;
    case MANUAL_CTRLMODE_ERR://手动机控位置
        Dialog_Common_Init(Dialog_TwoButton_Prompt_HANDLE,Sys_Lib_GetDispStr(DP_BAGVENTPOSERR),Sys_Lib_GetDispStr(DP_REPEAT),Sys_Lib_GetDispStr(DP_CONTINUE),0);
        break;
    case CIRCUIT_DISCONNECT_ERR://管路未连接
        Dialog_Common_Init(Dialog_TwoButton_Prompt_HANDLE,Sys_Lib_GetDispStr(DP_CIRCUITNOTCONN),Sys_Lib_GetDispStr(DP_REPEAT),Sys_Lib_GetDispStr(DP_CONTINUE),0);
        break;
    case FRESH_VALVE_LEAK_ERR://比例阀或新鲜气体阀漏气故障
        Dialog_Common_Init(Dialog_TwoButton_Prompt_HANDLE,Sys_Lib_GetDispStr(DP_INSPFLOWCONTROLVALVE),Sys_Lib_GetDispStr(DP_REPEAT),Sys_Lib_GetDispStr(DP_CONTINUE),0);
        break;
    }
}
/***********************************************************
 *函数名：CheckItem_TextSet
 *参数  ： CheckFlag  检测项目组的标志  LEFT_CHECKFLAG：设备检测    
                                        RIGHT_CHECKFLAG：气密性检测
           Operation  进行的操作        START_CHECK:开始检测前，将屏幕清空        
                                        DISPLAY_RESULT:显示上一次检测结果
           BasicAddr  显示数据的基地址
 *返回值：无
 *调用位置：
 *功能  ：
 ***********************************************************
*/
void CheckItem_TextSet(int CheckFlag,int Operation,int BasicAddr)
{
    U8 cnt;
    U8 year,mon,day,hour,min;
    U8 string[40];
    U8 GasType = 0;
	double result_double = 0;
	if(Operation == START_CHECK)//清屏
	{
	    for(cnt = 0; cnt < 13; cnt++)
		{
			TEXT_SetText(WM_GetDialogItem(CheckItem_Total_HANDLE,GUI_ID_TEXT5 + cnt), Sys_Lib_GetDispStr(DP_EMPTY));
		}
		for(cnt = 0; cnt < 39; cnt++)
		{
			TEXT_SetText(WM_GetDialogItem(CheckItem_Total_HANDLE,GAS_SUPPLY_ID + cnt), Sys_Lib_GetDispStr(DP_EMPTY));
		}
	}
    else if(Operation == DISPLAY_TITLE)//显示标题及时间
    {
		if(CheckFlag == LEFT_CHECKFLAG)//Manual
		{
			year = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_MANUAL_YEAR_OFFSET_ADDR);
			mon = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_MANUAL_MONTH_OFFSET_ADDR);
			day = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_MANUAL_DAY_OFFSET_ADDR);
			hour = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_MANUAL_HOUR_OFFSET_ADDR);
			min = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_MANUAL_MIN_OFFSET_ADDR);
			sprintf(string,"20%02d-%02d-%02d %02d:%02d",year,mon,day,hour,min);
			TEXT_SetText(WM_GetDialogItem(CheckItem_Total_HANDLE,GUI_ID_TEXT1),string);
			//检测状态统计
			if(CONTEXT_APP.CheckInfo.Result_Report._Manual[(RESULT_FAULT>>2)])
			{
				sprintf(string,"%s %02d",Sys_Lib_GetDispStr(DP_CROSS),CONTEXT_APP.CheckInfo.Result_Report._Manual[(RESULT_FAULT>>2)]);
				TEXT_SetTextColor(WM_GetDialogItem(CheckItem_Total_HANDLE,GUI_ID_TEXT4),GUI_RED);
				TEXT_SetText(WM_GetDialogItem(CheckItem_Total_HANDLE,GUI_ID_TEXT4),string);
			}
			else if(CONTEXT_APP.CheckInfo.Result_Report._Manual[(RESULT_SUCCESS>>2)])
			{
				sprintf(string,"%s %02d",Sys_Lib_GetDispStr(DP_TICK),CONTEXT_APP.CheckInfo.Result_Report._Manual[(RESULT_SUCCESS>>2)]);
				TEXT_SetTextColor(WM_GetDialogItem(CheckItem_Total_HANDLE,GUI_ID_TEXT4),GUI_GREEN);
				TEXT_SetText(WM_GetDialogItem(CheckItem_Total_HANDLE,GUI_ID_TEXT4),string);
			}
			else if(CONTEXT_APP.CheckInfo.Result_Report._Manual[(RESULT_SKIP>>2)])
			{
				sprintf(string,"%s %02d",Sys_Lib_GetDispStr(DP_DASHSMALL),CONTEXT_APP.CheckInfo.Result_Report._Manual[(RESULT_SKIP>>2)]);
				TEXT_SetTextColor(WM_GetDialogItem(CheckItem_Total_HANDLE,GUI_ID_TEXT4),GUI_WHITE);
				TEXT_SetText(WM_GetDialogItem(CheckItem_Total_HANDLE,GUI_ID_TEXT4),string);
			}
			TEXT_SetText(WM_GetDialogItem(CheckItem_Total_HANDLE,GUI_ID_TEXT0),Sys_Lib_GetDispStr(DP_MANUALLEAKTEST));
		}
		else if(CheckFlag == RIGHT_CHECKFLAG)//Automatic
		{
			year = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_AUTOMATIC_YEAR_OFFSET_ADDR);
			mon = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_AUTOMATIC_MONTH_OFFSET_ADDR);
			day = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_AUTOMATIC_DAY_OFFSET_ADDR);
			hour = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_AUTOMATIC_HOUR_OFFSET_ADDR);
			min = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_AUTOMATIC_MIN_OFFSET_ADDR);
			sprintf(string,"20%02d-%02d-%02d %02d:%02d",year,mon,day,hour,min);
			TEXT_SetText(WM_GetDialogItem(CheckItem_Total_HANDLE,GUI_ID_TEXT1),string);
			//检测状态统计
			//if(CONTEXT_APP.CheckInfo.Result_Report.Automatic[(RESULT_FAULT>>2)])
			{
				sprintf(string,"%s %02d",Sys_Lib_GetDispStr(DP_CROSS),CONTEXT_APP.CheckInfo.Result_Report.Automatic[(RESULT_FAULT>>2)]);
                TEXT_SetTextColor(WM_GetDialogItem(CheckItem_Total_HANDLE,GUI_ID_TEXT3),GUI_RED);//20170207
				TEXT_SetText(WM_GetDialogItem(CheckItem_Total_HANDLE,GUI_ID_TEXT3),string);
			}
			//if(CONTEXT_APP.CheckInfo.Result_Report._Manual[(RESULT_SUCCESS>>2)])
			{
				sprintf(string,"%s %02d",Sys_Lib_GetDispStr(DP_TICK),CONTEXT_APP.CheckInfo.Result_Report.Automatic[(RESULT_SUCCESS>>2)]);
                TEXT_SetTextColor(WM_GetDialogItem(CheckItem_Total_HANDLE,GUI_ID_TEXT2),GUI_GREEN);//20170207
				TEXT_SetText(WM_GetDialogItem(CheckItem_Total_HANDLE,GUI_ID_TEXT2),string);
			}
			//if(CONTEXT_APP.CheckInfo.Result_Report._Manual[(RESULT_SKIP>>2)])
			{
				sprintf(string,"%s %02d",Sys_Lib_GetDispStr(DP_DASHSMALL),CONTEXT_APP.CheckInfo.Result_Report.Automatic[(RESULT_SKIP>>2)]);
                TEXT_SetTextColor(WM_GetDialogItem(CheckItem_Total_HANDLE,GUI_ID_TEXT4),GUI_WHITE);//20170207
				TEXT_SetText(WM_GetDialogItem(CheckItem_Total_HANDLE,GUI_ID_TEXT4),string);
			}
			TEXT_SetText(WM_GetDialogItem(CheckItem_Total_HANDLE,GUI_ID_TEXT0),Sys_Lib_GetDispStr(DP_AUTOSYSTEMTEST));
		}
    }
}
/********************************************************
 *函数名：Desktop_CheckItemText_Init
 *参数  ：void
 *返回值：无
 *被调用位置：初始化函数调用
 *功能  ：初始化当前语言下的检测项目显示
*********************************************************
*/
void Desktop_CheckItemText_Init(void)
{
    strcpy(Desktop_CheckItem_Text[POS_Manual],             Sys_Lib_GetDispStr(DP_MANUAL));
    strcpy(Desktop_CheckItem_Text[POS_Barometer],          Sys_Lib_GetDispStr(DP_BAROMETER));
    strcpy(Desktop_CheckItem_Text[POS_GasSypply],          Sys_Lib_GetDispStr(DP_GASSUPPLY));
    strcpy(Desktop_CheckItem_Text[POS_Flow_S],             Sys_Lib_GetDispStr(DP_CIRCUITFLOWSENSOR));
    strcpy(Desktop_CheckItem_Text[POS_Pressure_S],         Sys_Lib_GetDispStr(DP_PRESSURESENSOR));
//	strcpy(Desktop_CheckItem_Text[POS_FG_Flow_ZERO_S],     System_Display[STR_SelfTest_FG_Flow_Check]);
	strcpy(Desktop_CheckItem_Text[POS_Compliance],         Sys_Lib_GetDispStr(DP_CIRCUITCOMPLIANCE));
    strcpy(Desktop_CheckItem_Text[POS_Circuit_Leakage],    Sys_Lib_GetDispStr(DP_CIRCUITLEAKAGE));
    strcpy(Desktop_CheckItem_Text[POS_Oxygen],             Sys_Lib_GetDispStr(DP_O2SENSOR));
    strcpy(Desktop_CheckItem_Text[POS_Battery],            Sys_Lib_GetDispStr(DP_BATTERY));
}
/********************************************************
 *函数名：Disp_CheckItem_Text
 *参数  ：显示检测项目的编号
 *返回值：无
 *被调用位置：每项检测开始之前
 *功能  ：显示检测项目的名称
*********************************************************
*/
void Disp_CheckItem_Text(int num)
{
	if(num < CONTEXT_APP.Left_CheckItem_Num)  //左侧窗口
	{
        TEXT_SetText(WM_GetDialogItem(CheckItem_Left_HANDLE,GUI_ID_TEXT3+num),Desktop_CheckItem_Text[num]);
	}
	else 
	{
        TEXT_SetText(WM_GetDialogItem(CheckItem_Right_HANDLE,GUI_ID_TEXT3+num-CONTEXT_APP.Left_CheckItem_Num),Desktop_CheckItem_Text[num]);
	}
}


/********************************************************
 *函数名：
 *参数  ：Type  结果的类型  0:正常类型(状态、数值、单位、错误码)   1:气源类型(状态、小标题、数值、单位、错误码)
          State 状态类型
 *返回值：
 *被调用位置：
 *功能  ：处理自检结果显示
*********************************************************
*/
void Result_DispClear(WM_HWIN hWin,U16 WidgetID,U8 Type)
{
    U8 Cnt = 0;
    WM_HWIN hText = 0;

    //djq del 20160809 删除了之前增加的部分，修改为跟呼吸机一样的内容
	//hText = WM_GetDialogItem(hWin,WidgetID);
	//TEXT_SetText(hText,"");
	//hText = WM_GetDialogItem(hWin,WidgetID+1);
	//TEXT_SetText(hText,"");

    Type = Type?1:0;//如果类型为1，则
    for(Cnt = 0;Cnt < 4+Type;Cnt++)
    {
        hText = WM_GetDialogItem(hWin,WidgetID + Cnt);
        if(WM_IsWindow(hText))
        {
            TEXT_SetTextColor(hText,CheckResult_Font_Color);//默认使用通过的颜色
            TEXT_SetText(hText,"");
        }
    }
}


void Result_DispValue(WM_HWIN hWin,U16 WidgetID,U8 Type,U8 State,const char * Name,const char * Value,const char *Unit,U8 Code)
{
    WM_HWIN hText[5]= {0};
    char String[50] = {0};
    U8 Index = 0;
    Type = Type?1:0;//如果类型为1，则
    Result_DispClear(hWin,WidgetID,Type);

    //获得句柄
    for(Index = 0;Index < 4+Type;Index++)
    {
        hText[Index] = WM_GetDialogItem(hWin,WidgetID + Index);
    }
    //区别类型，显示
    if(Type)
    {
		sprintf(String,"%s %s %s", Name, Value, Unit);
		TEXT_SetText(hText[1], String);
        //TEXT_SetText(hText[1],Name);
        //TEXT_SetText(hText[2],Value);
        //TEXT_SetText(hText[3],Unit);
        sprintf(String,"%#x",Code);
//        TEXT_SetText(hText[4],String);
    }
    else
    {
		sprintf(String,"%s %s", Value, Unit);
		TEXT_SetText(hText[1], String);
        //TEXT_SetText(hText[1],Value);
        //TEXT_SetText(hText[2],Unit);
        sprintf(String,"%#x",Code);
//        TEXT_SetText(hText[3],String);
    }

    if((State&RESULT_MARK) == RESULT_SUCCESS)
    {
        TEXT_SetTextColor(hText[0],RESULT_PASS_COLOR);
        TEXT_SetText(hText[0],Sys_Lib_GetDispStr(DP_TICK));
    }
    //else if((State&RESULT_MARK) == RESULT_JUMP)
    //{
    //    TEXT_SetTextColor(hText[0],RESULT_SKIP_COLOR);
    //    TEXT_SetText(hText[0],System_Display[DISP_CANCEL]);
    //} 
    else if((State&RESULT_MARK) == RESULT_SKIP)
    {
        TEXT_SetTextColor(hText[0],RESULT_SKIP_COLOR);
        TEXT_SetText(hText[0],Sys_Lib_GetDispStr(DP_DASHSMALL));
    }
    else if((State&RESULT_MARK) == RESULT_FAULT)
    {
        TEXT_SetTextColor(hText[0],RESULT_FAIL_COLOR);
        TEXT_SetText(hText[0],Sys_Lib_GetDispStr(DP_CROSS));
    }
}

void Result_DispState(WM_HWIN hWin,U16 WidgetID,U8 Type,U8 State,const char * Name,U8 Code)//√  Pass  0xXX
{
    U8 Index = 0;
    WM_HWIN hText[5]= {0};
    char String[6] = {0};
    Type = Type?1:0;//如果类型为1，则
    Result_DispClear(hWin,WidgetID,Type);


    for(Index = 0;Index < 2+Type;Index++)
    {
        hText[Index] = WM_GetDialogItem(hWin,WidgetID + Index);
    }
    if(Type)
    {
        Index = 2;//状态字符串显示的数组索引值
        TEXT_SetText(hText[1],Name);
//        sprintf(String,"%#x",Code);
////        TEXT_SetText(hText[4],String);
    }
    else
    {
        Index = 1;//状态字符串显示的数组索引值
//        sprintf(String,"%#x",Code);
////        TEXT_SetText(hText[3],String);
    }
    if((State&RESULT_MARK) == RESULT_SUCCESS)
    {
        TEXT_SetTextColor(hText[0],RESULT_PASS_COLOR);
        TEXT_SetText(hText[0],Sys_Lib_GetDispStr(DP_TICK));
        TEXT_SetText(hText[Index],Sys_Lib_GetDispStr(DP_PASS));
    }
    //else if((State&RESULT_MARK) == RESULT_JUMP)
    //{
    //    TEXT_SetTextColor(hText[0],RESULT_SKIP_COLOR);
    //    TEXT_SetText(hText[0],System_Display[DISP_CANCEL]);
    //    TEXT_SetText(hText[Index],System_Display[DISP_CANCEL]);
    //} 
    else if((State&RESULT_MARK) == RESULT_SKIP)
    {
        TEXT_SetTextColor(hText[0],RESULT_SKIP_COLOR);
        TEXT_SetText(hText[0],Sys_Lib_GetDispStr(DP_DASHSMALL));
        TEXT_SetText(hText[Index],Sys_Lib_GetDispStr(DP_SKIP));
    }
    else if((State&RESULT_MARK) == RESULT_FAULT)
    {
        TEXT_SetTextColor(hText[0],RESULT_FAIL_COLOR);
        TEXT_SetText(hText[0],Sys_Lib_GetDispStr(DP_CROSS));
        TEXT_SetText(hText[Index],Sys_Lib_GetDispStr(DP_FAIL));
    }
}

void Result_DispPrompt(WM_HWIN hWin,U16 WidgetID,U8 Type,const char *Name,const char *Prompt)//×  Fail  PromptString
{
    WM_HWIN hText[5]= {0};
    U8 Index = 0;
    Type = Type?1:0;//如果类型为1，则
    Result_DispClear(hWin,WidgetID,Type);

    for(Index = 0;Index < 4+Type;Index++)
    {
        hText[Index] = WM_GetDialogItem(hWin,WidgetID + Index);
    }

    TEXT_SetTextColor(hText[0],RESULT_FAIL_COLOR);
    TEXT_SetText(hText[0],Sys_Lib_GetDispStr(DP_CROSS));
    if(Type)
    {
        Index = 2;//状态字符串显示的数组索引值
        TEXT_SetText(hText[1],Name);
    }
    else
    {
        Index = 2;//状态字符串显示的数组索引值
        TEXT_SetText(hText[1],Sys_Lib_GetDispStr(DP_FAIL));
		//WM_MoveChildTo(hText[1],200,10);
    }
    TEXT_SetText(hText[Index],Prompt);

}

void Result_DispTesting(WM_HWIN hWin,U16 WidgetID,U8 Type)//Testing...
{
    WM_HWIN hText= 0;
	char str[20] = {0};
    //Type = Type?1:0;//如果类型为1，则
    //Result_DispClear(hWin,WidgetID,Type);

    hText = WM_GetDialogItem(hWin,WidgetID);//最长的控件
    TEXT_SetTextColor(hText,CheckResult_Font_Color);
    TEXT_SetText(hText,Sys_Lib_GetDispStr(DP_TESTING));
}

void Disp_ItemResult(PositionFlag Pos)
{
    PositionFlag OldPos = Context_GetPosition();
    double result_double = 0;
    Context_SetPosition(Pos);
    switch(Pos)
    {
    case POS_Manual:
        break;
    case POS_Barometer:
        CONTEXT_APP.Current_ErrorCode = CONTEXT_APP.CheckInfo.Barometer.Error_Code.Bit_8;
        if(((CONTEXT_APP.CheckInfo.Barometer.Check_result[1] & 0x30) >> 4) == 1)//数据类型
        {
            Disp_Result(4,Sys_Lib_GetDispStr(DP_MMHG),(CONTEXT_APP.CheckInfo.Barometer.Check_result[1]&0xf)<<8 
                | (CONTEXT_APP.CheckInfo.Barometer.Check_result[0]),
                (CONTEXT_APP.CheckInfo.Barometer.Check_result[1]>>6));//获取状态位直接送显示
        }
        else //状态类型
        {
            Disp_Result(2,(CONTEXT_APP.CheckInfo.Barometer.Check_result[1] >> 4));//获取状态位直接送显示
        }
        break;
    case POS_GasSypply:
        CONTEXT_APP.Current_ErrorCode = CONTEXT_APP.CheckInfo.GasO2.Error_Code.Bit_8;
        if(((CONTEXT_APP.CheckInfo.GasO2.Check_result[1] & 0x30) >> 4) == 1)//数据类型
        {
            Disp_Result(4,Sys_Lib_GetDispStr(DP_KPA),(CONTEXT_APP.CheckInfo.GasO2.Check_result[1]&0xf)<<8 
                | (CONTEXT_APP.CheckInfo.GasO2.Check_result[0]),
                (CONTEXT_APP.CheckInfo.GasO2.Check_result[1]>>6));//获取状态位直接送显示
        }
        else //状态类型
        {
            Disp_Result(2,(CONTEXT_APP.CheckInfo.GasO2.Check_result[1] >> 4));//获取状态位直接送显示
        }
        break;
    case POS_Flow_S:
        CONTEXT_APP.Current_ErrorCode = CONTEXT_APP.CheckInfo.Flow_S.Error_Code.Bit_8;
        if(((CONTEXT_APP.CheckInfo.Flow_S.Check_result[1] & 0x30) >> 4) == 1)//数据类型
        {
            result_double = ((CONTEXT_APP.CheckInfo.Flow_S.Check_result[1]&0xf)<<8 
                |(CONTEXT_APP.CheckInfo.Flow_S.Check_result[0]))/10.0;
            Disp_Result(4,Sys_Lib_GetDispStr(DP_MLMIN),result_double,(CONTEXT_APP.CheckInfo.Flow_S.Check_result[1]>>6));//获取状态位直接送显示
        }
        else //状态类型
        {
            Disp_Result(2,(CONTEXT_APP.CheckInfo.Flow_S.Check_result[1] >> 4));//获取状态位直接送显示
        }
        break;
    case POS_Pressure_S:
        CONTEXT_APP.Current_ErrorCode = CONTEXT_APP.CheckInfo.Pressure_S.Error_Code.Bit_8;
        if(((CONTEXT_APP.CheckInfo.Pressure_S.Check_result[1] & 0x30) >> 4) == 1)//数据类型
        {
            result_double = ((CONTEXT_APP.CheckInfo.Pressure_S.Check_result[1]&0xf)<<8 
                |(CONTEXT_APP.CheckInfo.Pressure_S.Check_result[0]))/10.0;
            Disp_Result(4,Sys_Lib_GetDispStr(DP_MLMIN),result_double,(CONTEXT_APP.CheckInfo.Pressure_S.Check_result[1]>>6));//获取状态位直接送显示
        }
        else //状态类型
        {
            Disp_Result(2,(CONTEXT_APP.CheckInfo.Pressure_S.Check_result[1] >> 4));//获取状态位直接送显示
        }
        break;
    //case POS_FG_Flow_ZERO_S:
    //    CONTEXT_APP.Current_ErrorCode = CONTEXT_APP.CheckInfo.FG_Flow_S.Error_Code.Bit_8;
    //    if(((CONTEXT_APP.CheckInfo.FG_Flow_S.Check_result[1] & 0x30) >> 4) == 1)//数据类型
    //    {
    //        result_double = ((CONTEXT_APP.CheckInfo.FG_Flow_S.Check_result[1]&0xf)<<8 
    //            |(CONTEXT_APP.CheckInfo.FG_Flow_S.Check_result[0]))/10.0;
    //        Disp_Result(4,System_Display[STR_Unit_mL_min],result_double,(CONTEXT_APP.CheckInfo.FG_Flow_S.Check_result[1]>>6));//获取状态位直接送显示
    //    }
    //    else //状态类型
    //    {
    //        Disp_Result(2,(CONTEXT_APP.CheckInfo.FG_Flow_S.Check_result[1] >> 4));//获取状态位直接送显示
    //    }
    //    break;
    case POS_Oxygen:
        CONTEXT_APP.Current_ErrorCode = CONTEXT_APP.CheckInfo.Oxygen.Error_Code.Bit_8;
        if(((CONTEXT_APP.CheckInfo.Oxygen.Check_result[1] & 0x30) >> 4) == 1)//数据类型
        {
            result_double = ((CONTEXT_APP.CheckInfo.Oxygen.Check_result[1]&0xf)<<8 
                |(CONTEXT_APP.CheckInfo.Oxygen.Check_result[0]))/10.0;
            Disp_Result(4,Sys_Lib_GetDispStr(DP_MLMIN),result_double,(CONTEXT_APP.CheckInfo.Oxygen.Check_result[1]>>6));//获取状态位直接送显示
        }
        else //状态类型
        {
            Disp_Result(2,(CONTEXT_APP.CheckInfo.Oxygen.Check_result[1] >> 4));//获取状态位直接送显示
        }
        break;
    case POS_Compliance:
        CONTEXT_APP.Current_ErrorCode = CONTEXT_APP.CheckInfo.Compliance.Error_Code.Bit_8;
        if(((CONTEXT_APP.CheckInfo.Compliance.Check_result[1] & 0x30) >> 4) == 1)//数据类型
        {
            result_double = ((CONTEXT_APP.CheckInfo.Compliance.Check_result[1]&0xf)<<8 
                |(CONTEXT_APP.CheckInfo.Compliance.Check_result[0]))/10.0;
            Disp_Result(4,Sys_Lib_GetDispStr(DP_MLCMH2O),
                        result_double,(CONTEXT_APP.CheckInfo.Compliance.Check_result[1]>>6));//获取状态位直接送显示
        }
        else //状态类型
        {
            Disp_Result(2,(CONTEXT_APP.CheckInfo.Compliance.Check_result[1] >> 4));//获取状态位直接送显示
        }
        break;
    case POS_Circuit_Leakage:
        CONTEXT_APP.Current_ErrorCode = CONTEXT_APP.CheckInfo.Internal_Leakage.Error_Code.Bit_8;
        if(((CONTEXT_APP.CheckInfo.Internal_Leakage.Check_result[1] & 0x30) >> 4) == 1)//数据类型
        {
            result_double = ((CONTEXT_APP.CheckInfo.Internal_Leakage.Check_result[1]&0xf)<<8 
                |(CONTEXT_APP.CheckInfo.Internal_Leakage.Check_result[0]))/10.0;
            Disp_Result(4,Sys_Lib_GetDispStr(DP_MLMIN),result_double,(CONTEXT_APP.CheckInfo.Internal_Leakage.Check_result[1]>>6));//获取状态位直接送显示
        }
        else //状态类型
        {
            Disp_Result(2,(CONTEXT_APP.CheckInfo.Internal_Leakage.Check_result[1] >> 4));//获取状态位直接送显示
        }
        break;
    case POS_Battery:
        CONTEXT_APP.Current_ErrorCode = CONTEXT_APP.CheckInfo.Battery.Error_Code.Bit_8;
        if(((CONTEXT_APP.CheckInfo.Battery.Check_result[1] & 0x30) >> 4) == 1)//数据类型
        {
            result_double = ((CONTEXT_APP.CheckInfo.Battery.Check_result[1]&0xf)<<8 
                |(CONTEXT_APP.CheckInfo.Battery.Check_result[0]))/10.0;
            Disp_Result(4,Sys_Lib_GetDispStr(DP_MLMIN),result_double,(CONTEXT_APP.CheckInfo.Battery.Check_result[1]>>6));//获取状态位直接送显示
        }
        else //状态类型
        {
            Disp_Result(2,(CONTEXT_APP.CheckInfo.Battery.Check_result[1] >> 4));//获取状态位直接送显示
        }
        break;
    }
    Context_SetPosition(OldPos);
}




