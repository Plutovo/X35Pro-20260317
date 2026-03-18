#include "system_app.h"

#include "GUI.h"

extern unsigned char O2ModuleOff;
/*
*检测标志
*/
U8 CHECK_DEBUG = 0;//0：正常  2：开机正式进入界面之前  1：待机
/***********************************************************
 *函数名  ：_Resource_Init
 *调用位置：Create_AllGeneralDialog()
 *功能  ：保留控件之前的设置，并且修改控件的默认属性
************************************************************
*/
static void _Item_Init(void)//djq modify 20170109
{
	FRAMEWIN_SetDefaultBarColor(1,DialogBk_Color);
	FRAMEWIN_SetDefaultBarColor(0,Framewin_Title_Color);
    FRAMEWIN_SetDefaultTitleHeight(35);
    FRAMEWIN_SetDefaultBorderSize(0);
}
/***********************************************************
 *函数名  ：Init_GlobalParameter
 *调用位置：每次检测之前，和第一次进入自检界面的时候
 *参数：Choose： DISPLAY_RESULT： 检测日志初始化
                 START_CHECK： 开始检测前初始化
 *功能  ：保留控件之前的设置，并且修改控件的默认属性
************************************************************
*/
void Init_GlobalParameter(U8 Choose,U8 CheckItem_Flag)
{
    U8   cnt;
    U8   tmp;
    U64  Error_Code = 0;
    Context_SetPosition(POS_ERROR);//显示位置（行）
    CONTEXT_APP.Checking_State = 0xff;
    CONTEXT_APP.Global_CancelFlag = 0;
    CONTEXT_APP.Pause_Flag = 0;

    if(Choose == DISPLAY_RESULT)  //初始化日志
    {
        Context_SetControlFlag(Ctrl_Choose);
        Context_SetUpdateFlag(UPDATE_NONE_ITEM);// 刷新标志，在串口接收和超时的地方置位，更新完成后清除
        CONTEXT_APP.Result = 0;                  // 在传口中接收到的结果
        CONTEXT_APP.Left_CheckItem_Num = 1;  //左侧校验项目个数
        CONTEXT_APP.Total_CheckItem_Num = CHECKITEM_NUM; //全部校验项目个数   djq modify 20160809 修改自检项目
        Context_SetWindow_None();
        Context_SetNoneCheckFlag();

		CONTEXT_APP.CheckInfo.Manual.Error_Code.Bit_8 = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_MANUAL_OFFSET_ADDR);
        CONTEXT_APP.CheckInfo.Manual.Check_result[1] = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_MANUAL_OFFSET_ADDR + 1);
        CONTEXT_APP.CheckInfo.Manual.Check_result[0] = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_MANUAL_OFFSET_ADDR + 2);
        //djq add 20160809
        CONTEXT_APP.CheckInfo.Barometer.Error_Code.Bit_8 = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_BAROMETER_OFFSET_ADDR);
        CONTEXT_APP.CheckInfo.Barometer.Check_result[1] = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_BAROMETER_OFFSET_ADDR + 1);
        CONTEXT_APP.CheckInfo.Barometer.Check_result[0] = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_BAROMETER_OFFSET_ADDR + 2);

        CONTEXT_APP.CheckInfo.GasO2.Error_Code.Bit_8 = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_GASO2_OFFSET_ADDR);
        CONTEXT_APP.CheckInfo.GasO2.Check_result[1] = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_GASO2_OFFSET_ADDR + 1);
        CONTEXT_APP.CheckInfo.GasO2.Check_result[0] = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_GASO2_OFFSET_ADDR + 2);

        CONTEXT_APP.CheckInfo.GasN2O.Error_Code.Bit_8 = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_GASN2O_OFFSET_ADDR);
        CONTEXT_APP.CheckInfo.GasN2O.Check_result[1] = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_GASN2O_OFFSET_ADDR + 1);
        CONTEXT_APP.CheckInfo.GasN2O.Check_result[0] = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_GASN2O_OFFSET_ADDR + 2);

        CONTEXT_APP.CheckInfo.GasAir.Error_Code.Bit_8 = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_GASAIR_OFFSET_ADDR);;
        CONTEXT_APP.CheckInfo.GasAir.Check_result[1] = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_GASAIR_OFFSET_ADDR + 1);
        CONTEXT_APP.CheckInfo.GasAir.Check_result[0] = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_GASAIR_OFFSET_ADDR + 2);

        CONTEXT_APP.CheckInfo.Flow_S.Error_Code.Bit_8 = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_FLOW_OFFSET_ADDR);
        CONTEXT_APP.CheckInfo.Flow_S.Check_result[1] = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_FLOW_OFFSET_ADDR + 1);
        CONTEXT_APP.CheckInfo.Flow_S.Check_result[0] = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_FLOW_OFFSET_ADDR + 2);

        CONTEXT_APP.CheckInfo.Pressure_S.Error_Code.Bit_8 = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_PRESSURE_OFFSET_ADDR);
        CONTEXT_APP.CheckInfo.Pressure_S.Check_result[1] = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_PRESSURE_OFFSET_ADDR + 1);//状态类型的结果   | (0x0 << 4)
        CONTEXT_APP.CheckInfo.Pressure_S.Check_result[0] = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_PRESSURE_OFFSET_ADDR + 2);

        //CONTEXT_APP.CheckInfo.FG_Flow_S.Error_Code.Bit_8 = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_FG_FLOW_OFFSET_ADDR);
        //CONTEXT_APP.CheckInfo.FG_Flow_S.Check_result[1] = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_FG_FLOW_OFFSET_ADDR + 1);
        //CONTEXT_APP.CheckInfo.FG_Flow_S.Check_result[0] =RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_FG_FLOW_OFFSET_ADDR + 2);

        CONTEXT_APP.CheckInfo.Compliance.Error_Code.Bit_8 = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_COMPLIANCE_OFFSET_ADDR);
        CONTEXT_APP.CheckInfo.Compliance.Check_result[1] = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_COMPLIANCE_OFFSET_ADDR + 1);
        CONTEXT_APP.CheckInfo.Compliance.Check_result[0] =RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_COMPLIANCE_OFFSET_ADDR + 2);

		CONTEXT_APP.CheckInfo.Internal_Leakage.Error_Code.Bit_8 = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_INTERNAL_LEAKAGE_OFFSET_ADDR);
        CONTEXT_APP.CheckInfo.Internal_Leakage.Check_result[1] = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_INTERNAL_LEAKAGE_OFFSET_ADDR + 1);
        CONTEXT_APP.CheckInfo.Internal_Leakage.Check_result[0] =RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_INTERNAL_LEAKAGE_OFFSET_ADDR + 2);

        CONTEXT_APP.CheckInfo.Oxygen.Error_Code.Bit_8 = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_OXYGEN_OFFSET_ADDR);
        CONTEXT_APP.CheckInfo.Oxygen.Check_result[1] = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_OXYGEN_OFFSET_ADDR + 1);
        CONTEXT_APP.CheckInfo.Oxygen.Check_result[0] = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_OXYGEN_OFFSET_ADDR + 2);

		CONTEXT_APP.CheckInfo.Battery.Error_Code.Bit_8 = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_BATTERY_OFFSET_ADDR);
        CONTEXT_APP.CheckInfo.Battery.Check_result[1] = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_BATTERY_OFFSET_ADDR + 1);
        CONTEXT_APP.CheckInfo.Battery.Check_result[0] =RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_BATTERY_OFFSET_ADDR + 2);

//最终检测结果的统计
        CONTEXT_APP.CheckInfo.Result_Report._Manual[(RESULT_SUCCESS>>2)] = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_LEFT_MANUAL_OFFSET_ADDR + (RESULT_SUCCESS>>2));
        CONTEXT_APP.CheckInfo.Result_Report._Manual[(RESULT_FAULT>>2)] = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_LEFT_MANUAL_OFFSET_ADDR + (RESULT_FAULT>>2));
        CONTEXT_APP.CheckInfo.Result_Report._Manual[(RESULT_SKIP>>2)] = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_LEFT_MANUAL_OFFSET_ADDR + (RESULT_SKIP>>2));
        CONTEXT_APP.CheckInfo.Result_Report.Automatic[(RESULT_SUCCESS>>2)] = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_AUTOMATIC_OFFSET_ADDR + (RESULT_SUCCESS>>2));
        CONTEXT_APP.CheckInfo.Result_Report.Automatic[(RESULT_FAULT>>2)] = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_AUTOMATIC_OFFSET_ADDR + (RESULT_FAULT>>2));
        CONTEXT_APP.CheckInfo.Result_Report.Automatic[(RESULT_SKIP>>2)] = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_AUTOMATIC_OFFSET_ADDR + (RESULT_SKIP>>2));
    }
    else if(Choose == START_CHECK)//检测前初始化
    {
        if(CheckItem_Flag == LEFT_CHECKFLAG)
        {
            CONTEXT_APP.CheckInfo.Result_Report._Manual[(RESULT_SUCCESS>>2)] = 0;
            CONTEXT_APP.CheckInfo.Result_Report._Manual[(RESULT_FAULT>>2)] = 0;
            CONTEXT_APP.CheckInfo.Result_Report._Manual[(RESULT_SKIP>>2)] = 0;
            CONTEXT_APP.CheckInfo.Manual.Error_Code.Bit_8 = 0;
            CONTEXT_APP.CheckInfo.Manual.Check_result[1] = 0x80;//忽略
            CONTEXT_APP.CheckInfo.Manual.Check_result[0] = 0;
        }
        else 
        {
            CONTEXT_APP.CheckInfo.Result_Report.Automatic[(RESULT_SUCCESS>>2)] = 0;
            CONTEXT_APP.CheckInfo.Result_Report.Automatic[(RESULT_FAULT>>2)] = 0;
            CONTEXT_APP.CheckInfo.Result_Report.Automatic[(RESULT_SKIP>>2)] = 0;
            //djq add 20160809
            CONTEXT_APP.CheckInfo.Barometer.Error_Code.Bit_8 =0;
            CONTEXT_APP.CheckInfo.Barometer.Check_result[1] =0x80;//忽略
            CONTEXT_APP.CheckInfo.Barometer.Check_result[0] = 0;

            CONTEXT_APP.CheckInfo.GasO2.Error_Code.Bit_8 =0;
            CONTEXT_APP.CheckInfo.GasO2.Check_result[1] =0x80;//忽略
            CONTEXT_APP.CheckInfo.GasO2.Check_result[0] = 0;

            CONTEXT_APP.CheckInfo.GasN2O.Error_Code.Bit_8 = 0;
            CONTEXT_APP.CheckInfo.GasN2O.Check_result[1] = 0x80;//忽略
            CONTEXT_APP.CheckInfo.GasN2O.Check_result[0] = 0;

            CONTEXT_APP.CheckInfo.GasAir.Error_Code.Bit_8 = 0;
            CONTEXT_APP.CheckInfo.GasAir.Check_result[1] = 0x80;//忽略
            CONTEXT_APP.CheckInfo.GasAir.Check_result[0] = 0;

            CONTEXT_APP.CheckInfo.Flow_S.Error_Code.Bit_8 = 0;
            CONTEXT_APP.CheckInfo.Flow_S.Check_result[1] = 0x80;//忽略
            CONTEXT_APP.CheckInfo.Flow_S.Check_result[0] = 0;

            CONTEXT_APP.CheckInfo.Pressure_S.Error_Code.Bit_8 = 0;
            CONTEXT_APP.CheckInfo.Pressure_S.Check_result[1] = 0x80;//忽略
            CONTEXT_APP.CheckInfo.Pressure_S.Check_result[0] = 0;

            //CONTEXT_APP.CheckInfo.FG_Flow_S.Error_Code.Bit_8 = 0;
            //CONTEXT_APP.CheckInfo.FG_Flow_S.Check_result[1] = 0x80;//忽略
            //CONTEXT_APP.CheckInfo.FG_Flow_S.Check_result[0] = 0;

            CONTEXT_APP.CheckInfo.Compliance.Error_Code.Bit_8 = 0;
            CONTEXT_APP.CheckInfo.Compliance.Check_result[1] = 0x80;//忽略
            CONTEXT_APP.CheckInfo.Compliance.Check_result[0] = 0; 

            CONTEXT_APP.CheckInfo.Internal_Leakage.Error_Code.Bit_8 = 0;
            CONTEXT_APP.CheckInfo.Internal_Leakage.Check_result[1] = 0x80;//忽略
            CONTEXT_APP.CheckInfo.Internal_Leakage.Check_result[0] = 0;


            CONTEXT_APP.CheckInfo.Oxygen.Error_Code.Bit_8 = 0;
            CONTEXT_APP.CheckInfo.Oxygen.Check_result[1] = 0x80;//忽略
            CONTEXT_APP.CheckInfo.Oxygen.Check_result[0] = 0;

            CONTEXT_APP.CheckInfo.Battery.Error_Code.Bit_8 = 0;
            CONTEXT_APP.CheckInfo.Battery.Check_result[1] = 0x80;//忽略
            CONTEXT_APP.CheckInfo.Battery.Check_result[0] = 0; 

            //djq del 20160809
            //CONTEXT_APP.CheckInfo.PEEP_Valve.Error_Code.Bit_8 = 0;
            //CONTEXT_APP.CheckInfo.PEEP_Valve.Check_result[1] = 0x80;//忽略
            //CONTEXT_APP.CheckInfo.PEEP_Valve.Check_result[0] = 0;
            //CONTEXT_APP.CheckInfo.Safety_Valve.Error_Code.Bit_8 = 0;
            //CONTEXT_APP.CheckInfo.Safety_Valve.Check_result[1] = 0x80;//忽略
            //CONTEXT_APP.CheckInfo.Safety_Valve.Check_result[0] = 0;
            //CONTEXT_APP.CheckInfo.Pop_off_Valve.Error_Code.Bit_8 = 0;
            //CONTEXT_APP.CheckInfo.Pop_off_Valve.Check_result[1] = 0x80;//忽略
            //CONTEXT_APP.CheckInfo.Pop_off_Valve.Check_result[0] = 0;
            //CONTEXT_APP.CheckInfo.ACGO_Switch.Error_Code.Bit_8 = 0;
            //CONTEXT_APP.CheckInfo.ACGO_Switch.Check_result[1] = 0x80;//忽略
            //CONTEXT_APP.CheckInfo.ACGO_Switch.Check_result[0] = 0;
        }
    }
    
}


/***********************************************************
 *函数名  ：Sys_Init
 *调用位置：Main_App()
 *功能  ：初始化自检程序所需要的所有属性
************************************************************
*/
void Sys_Init(void)
{
    //初始化对话框样式
    _Item_Init();
    GUI_SetColor(Font_Color);
    //创建自检过程中需要用到的提示对话框
    Create_AllGeneralDialog();
    //初始化全局参数
    Init_GlobalParameter(DISPLAY_RESULT,0);
    //开始检测标志  
    CHECK_DEBUG = 1;     
    //初始化桌面上的检测项目显示字符串
	Desktop_CheckItemText_Init();  
	if(O2ModuleOff)	//氧浓度开关关闭 
	{
		Context_SetO2PromptFlag(0);
	}
	else	//氧浓度开关开启
	{
		Context_SetO2PromptFlag(1);
	}
}


void Start_SelfTest(void)
{
    //初始化环境和参数
    Sys_Init();
    //创建自检界面，并且初始化界面
    Disp_Desktop();

    Serial_Communaction_Handing(BDU_TEST_START);                   ////向BDU发送开始标志
}

