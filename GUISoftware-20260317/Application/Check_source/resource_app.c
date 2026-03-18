/*
*********************************************************************************************************
*
*
----------------------------------------------------------------------
File        : resource.c
Purpose     : 存储各个对话框的资源表和回调函数
---------------------------END-OF-HEADER------------------------------
*/
#include "resource_app.h"
#include "GUI.h"



extern int PlanMenuType;
extern int Task;

/*
 *Handler
 *
*/
//Desktop
WM_CALLBACK*	DESKTOP_CALL_OLD = NULL;	//进入程序之前桌面的回调函数
WM_HWIN         CheckItem_Left_HANDLE;
WM_HWIN         CheckItem_Right_HANDLE;
WM_HWIN         Desktop_Button_HANDLE;
WM_HWIN         CheckItem_Total_HANDLE;


WM_HWIN		Dialog_OneButton_Prompt_HANDLE;	//一个按钮的一般对话框

WM_HWIN		Dialog_TwoButton_Prompt_HANDLE;	//两个按钮的一般对话框

//对话框显示内容 结构体
Callback_Disp_S Callback_Struct = {{""},{""},{0},{""},{""}};

U32	Alarm_Save_Id = 0;//报警ID保存，在判断是否有报警项目正在检测

U8 OxygenSensorCalPromptFlag = 1;//氧浓度传感器检测提示标志位   1：AIR   2：O2

//检测项目显示字符串
extern U8 Desktop_CheckItem_Text[CHECKITEM_NUM][60];
extern const U16 CheckItem_Result_Id_Init[CHECKITEM_NUM];
extern U16 CheckItem_Result_Id[TOTAL_TEXT_MAX_NUM];

extern U8 Checking_Show_Flag;//Checking显示标志
extern U8 AirSupplyTestResultFlag;//气源检测项结果标志

static unsigned char SystemACGOPromptFlag = 0;
extern BREATH_SET_PARA BeathSettingPara;
/********************************************************
 *函数名：_Callback_Dialog_Desktop
 *参数  ：系统消息
 *返回值：无
 *被调用位置：系统调用
 *功能  ：整个自检程序的主界面的回调程序
*********************************************************
*/
//最外层矩形大小
#define SELFTEST_WINDOW_X1              0    
#define SELFTEST_WINDOW_X2              799
#define SELFTEST_WINDOW_Y1              0
#define SELFTEST_WINDOW_Y2              479
#define SELFTEST_WINDOW_H               (SELFTEST_WINDOW_Y2-SELFTEST_WINDOW_Y1)
#define SELFTEST_WINDOW_W               (SELFTEST_WINDOW_X2-SELFTEST_WINDOW_X1)

//对话框大小
#define X_MARGIN             10//对话框到X轴边距

#define X_START_POS          (SELFTEST_WINDOW_X1 + X_MARGIN)//对话框X轴开始位置
#define X_END_POS            (SELFTEST_WINDOW_X2 - X_MARGIN)//对话框X轴开始位置
#define TITAL_Y_START_POS    (SELFTEST_WINDOW_Y1+5) //标题栏对话框Y轴开始位置
#define TITAL_Y_HEIGHT       115//标题栏对话框高度
#define TITAL_Y_END_POS      (TITAL_Y_START_POS + TITAL_Y_HEIGHT)//标题栏对话框Y轴结束位置，相对于桌面的位置
#define RESULT_Y_START_POS   (TITAL_Y_END_POS + 4)//结果对话框Y轴开始位置
#define RESULT_Y_HEIGHT      300//结果对话框高度
#define RESULT_Y_END_POS     (RESULT_Y_HEIGHT + RESULT_Y_START_POS)//标题栏对话框Y轴结束位置
#define CIRCULAR_L           8

static void _Callback_Dialog_Desktop(WM_MESSAGE *pMsg)
{
    U32 Id, IdCode;
    U8	cnt = 0;
    switch(pMsg->MsgId)
    {
    case WM_PAINT:
        GUI_SetColor(Back_Color);
        GUI_FillRect(SELFTEST_WINDOW_X1,SELFTEST_WINDOW_Y1,SELFTEST_WINDOW_X2,SELFTEST_WINDOW_Y2);//绘制自检背景

        //圆角外边框
        GUI_SetColor(Font_Color);
        LCD_SetColor(DialogBk_Color);

        //标题
        FillBevel(X_START_POS+CIRCULAR_L,
                  TITAL_Y_START_POS + CIRCULAR_L, 
                  X_END_POS - CIRCULAR_L, 
                  TITAL_Y_END_POS-CIRCULAR_L-1,
                  CIRCULAR_L);//10,105,287,573
        //检测项目及结果
        FillBevel(X_START_POS+CIRCULAR_L, 
                  RESULT_Y_START_POS + CIRCULAR_L, 
                  X_END_POS - CIRCULAR_L, 
                  RESULT_Y_END_POS-CIRCULAR_L,
                  CIRCULAR_L);//301,105,287,573
        LCD_SetColor(Back_Color);
        GUI_FillRect((SELFTEST_WINDOW_W)/2-2,
                     SELFTEST_WINDOW_Y1,
                     (SELFTEST_WINDOW_W)/2+2,
                     RESULT_Y_END_POS);//划分左右的竖线
        break;
    default:
        WM_DefaultProc(pMsg);
        break;
    }
}
/***********************************************************
 *资源名  ：_Dialog_CheckItem_Left
 *回调函数：_Callback_Dialog_CheckItem_Left
 *标题    ：
 *显示内容：检测项目和检测内容
 *按钮    ：
************************************************************
*/
#define LEFT_WIN_X          18
#define LEFT_WIN_Y          5
#define LEFT_WIN_W         380
#define LEFT_WIN_H          (RESULT_Y_END_POS-1)

#define TITLE_X_START_POS    10//标题起始位置
#define TEXT_HEIGHT          20
#define TITLE_WIDTH          164   
#define ICO_X_START_POS      222//结果(√ ×)图标的起始位置
#define RESULT_X_START       240//结果文字起始位置
#define ERR_X_START          277//错误代码起始位置
#define TITLE_Y_START_POS(Num)  (123 + 25 * Num)//标题纵坐标起始位置+项目之间的高度*项目数量

static const GUI_WIDGET_CREATE_INFO _Dialog_CheckItem_Left[]={
	{ WINDOW_CreateIndirect, " ",     0,          LEFT_WIN_X,LEFT_WIN_Y,LEFT_WIN_W,LEFT_WIN_H,WM_CF_SHOW,0},
    { TEXT_CreateIndirect,   "",  GUI_ID_TEXT0,   TITLE_X_START_POS,  8, 95/*60*/, 20, 0},//成功
    { TEXT_CreateIndirect,   "",  GUI_ID_TEXT9,   120/*70*/,  8,95/*60*/, 20, 0},//失败
    { TEXT_CreateIndirect,   "",  GUI_ID_TEXT10,  225/*130*/, 8, 95/*60*/, 20, 0},//跳过
	{ TEXT_CreateIndirect,   "",  GUI_ID_TEXT1,   0, 38, LEFT_WIN_W,  25, GUI_TA_HCENTER|GUI_TA_VCENTER},//上一次手动   标题
	{ TEXT_CreateIndirect,   "",  GUI_ID_TEXT2,   0, 65, LEFT_WIN_W,  18, GUI_TA_HCENTER|GUI_TA_VCENTER},//时间
	{ TEXT_CreateIndirect,   "",  GUI_ID_TEXT3,   0, 90, LEFT_WIN_W, 18, GUI_TA_HCENTER|GUI_TA_VCENTER},//上一次结果

	{ TEXT_CreateIndirect,   "",  GUI_ID_TEXT4,  TITLE_X_START_POS, TITLE_Y_START_POS(0), 200, TEXT_HEIGHT, GUI_TA_VCENTER},//手动检测项
	{ TEXT_CreateIndirect,   "",  GUI_ID_TEXT5,  ICO_X_START_POS,   TITLE_Y_START_POS(0), 200, TEXT_HEIGHT, GUI_TA_VCENTER},//检测结果 图标
	{ TEXT_CreateIndirect,   "",  GUI_ID_TEXT6,  RESULT_X_START,    TITLE_Y_START_POS(0), 200, TEXT_HEIGHT, GUI_TA_VCENTER},//检测结果 文字
	{ TEXT_CreateIndirect,   "",  GUI_ID_TEXT7,  ERR_X_START,       TITLE_Y_START_POS(0), 200, TEXT_HEIGHT, GUI_TA_VCENTER},//错误代码
	{ TEXT_CreateIndirect,   "",  GUI_ID_TEXT8,  130,               185,                  200, TEXT_HEIGHT, GUI_TA_VCENTER},//泄漏量

    //Frame Text
    { TEXT_CreateIndirect,   "",  GUI_ID_TEXT24,  0, TITAL_Y_HEIGHT, LEFT_WIN_W, 4, 0},  //边框

};
/********************************************************
 *函数名：_Callback_Dialog_CheckItem_Left
 *参数  ：系统消息
 *返回值：无
 *被调用位置：系统调用
 *功能  ：更新检测项目的显示
*********************************************************
*/
static void _Callback_Dialog_CheckItem_Left(WM_MESSAGE *pMsg)
{
	WM_HWIN FRAMEWIN_HANDLE,BUTTON_CENTER,TEXT_MSG_HANDLE;
	U32 Id, IdCode;
	U8 cnt;
	U8 year,mon,day,hour,min;
    U8 string[40];
	double result_double = 0;
	switch(pMsg->MsgId)
	{
	    case WM_INIT_DIALOG:
			WINDOW_SetBkColor(pMsg->hWin,DialogBk_Color);
			//bar
			TEXT_SetFont(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT0),&GUI_Font_16);
			TEXT_SetFont(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT1),&GUI_Font_24);
			TEXT_SetFont(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT2),&GUI_Font_16);
			TEXT_SetFont(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT3),&GUI_Font_16);
			TEXT_SetFont(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT4),&GUI_Font_16);
			TEXT_SetFont(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT5),&GUI_Font_16);
			TEXT_SetFont(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT6),&GUI_Font_16);
			TEXT_SetFont(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT7),&GUI_Font_16);
			TEXT_SetFont(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT8),&GUI_Font_16);
			TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT0),GUI_WHITE);

            //提示 成功  失败  跳过  的颜色
            TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT0),GUI_GREEN);
            sprintf(string,"%s%s",Sys_Lib_GetDispStr(DP_TICK),Sys_Lib_GetDispStr(DP_PASS));
            TEXT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT0),string);
            TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT9),GUI_RED);
            sprintf(string,"%s%s",Sys_Lib_GetDispStr(DP_CROSS),Sys_Lib_GetDispStr(DP_FAIL));
            TEXT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT9),string);
            TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT10),GUI_WHITE);
            sprintf(string,"%s%s",Sys_Lib_GetDispStr(DP_DASHSMALL),Sys_Lib_GetDispStr(DP_SKIP));
            TEXT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT10),string);     

			TEXT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT1),Sys_Lib_GetDispStr(DP_LASTMANUALLEAKTEST));
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT4),Sys_Lib_GetDispStr(DP_MANUAL));
			//title
			TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT1),GUI_WHITE);
			TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT2),GUI_WHITE);
			TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT4),GUI_WHITE);
			//Check Item
			//TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT5),CheckResult_Font_Color);
			TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT6),CheckResult_Font_Color);
			TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT7),CheckResult_Font_Color);
			TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT8),CheckResult_Font_Color);
            //Frame 
			TEXT_SetBkColor(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT24),Back_Color);
            //上一次自检时间
			year = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_MANUAL_YEAR_OFFSET_ADDR);
            mon = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_MANUAL_MONTH_OFFSET_ADDR);
            day = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_MANUAL_DAY_OFFSET_ADDR);
            hour = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_MANUAL_HOUR_OFFSET_ADDR);
            min = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_MANUAL_MIN_OFFSET_ADDR);
            sprintf(string,"20%02d-%02d-%02d %02d:%02d",year,mon,day,hour,min);
            TEXT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT2),string);
			Init_GlobalParameter(DISPLAY_RESULT,0);//初始化全局参数

			//检测状态统计
            //CONTEXT_APP.CheckInfo.Result_Report._Manual[(RESULT_FAULT>>2)] =RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_LEFT_MANUAL_OFFSET_ADDR + (RESULT_FAULT>>2));
            //CONTEXT_APP.CheckInfo.Result_Report._Manual[(RESULT_SUCCESS>>2)] =RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_LEFT_MANUAL_OFFSET_ADDR + (RESULT_SUCCESS>>2));
            //CONTEXT_APP.CheckInfo.Result_Report._Manual[(RESULT_SKIP>>2)] =RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_LEFT_MANUAL_OFFSET_ADDR + (RESULT_SKIP>>2));
			if(CONTEXT_APP.CheckInfo.Result_Report._Manual[(RESULT_FAULT>>2)])
			{
				sprintf(string,"%s %02d",Sys_Lib_GetDispStr(DP_CROSS),CONTEXT_APP.CheckInfo.Result_Report._Manual[(RESULT_FAULT>>2)]);
			    TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT3),GUI_RED);
				TEXT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT3),string);
			}
			else if(CONTEXT_APP.CheckInfo.Result_Report._Manual[(RESULT_SUCCESS>>2)])
			{
				sprintf(string,"%s %02d",Sys_Lib_GetDispStr(DP_TICK),CONTEXT_APP.CheckInfo.Result_Report._Manual[(RESULT_SUCCESS>>2)]);
			    TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT3),GUI_GREEN);
				TEXT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT3),string);
			}
			else if(CONTEXT_APP.CheckInfo.Result_Report._Manual[(RESULT_SKIP>>2)])
			{
				sprintf(string,"%s %02d",Sys_Lib_GetDispStr(DP_DASHSMALL),CONTEXT_APP.CheckInfo.Result_Report._Manual[(RESULT_SKIP>>2)]);
			    TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT3),GUI_WHITE);
				TEXT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT3),string);
			}

			if(((CONTEXT_APP.CheckInfo.Manual.Check_result[1] >> 4) & RESULT_MARK) == RESULT_SUCCESS)
			{
			    TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT5),GUI_GREEN);
				TEXT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT5),Sys_Lib_GetDispStr(DP_TICK));
				TEXT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT6),Sys_Lib_GetDispStr(DP_PASS));
			}
			else
			{
			    TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT5),GUI_RED);
				TEXT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT5),Sys_Lib_GetDispStr(DP_CROSS));
				TEXT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT6),Sys_Lib_GetDispStr(DP_FAIL));
			}
			//if(((CONTEXT_APP.CheckInfo.Manual.Check_result[1] & 0x30) >> 4) == 1)//数据类型
			//{
			//    result_double = ((CONTEXT_APP.CheckInfo.Manual.Check_result[1]&0xf)<<8 
   //                            |(CONTEXT_APP.CheckInfo.Manual.Check_result[0]))/10.0;
			//	sprintf(string,"Leakage %-2.1f %s",result_double, System_Display[STR_Unit_mL_min]);
			//	TEXT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT8),string);
			//}
			break;
		case WM_PAINT://重绘时，对话框的用户区是目标窗口
			WINDOW_SetBkColor(pMsg->hWin,DialogBk_Color);
            //Frame 
			TEXT_SetBkColor(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT24),Back_Color);
			break;
		default:
            WM_DefaultProc(pMsg);
            break;
	}
}
/***********************************************************
 *资源名  ：_Dialog_CheckItem_Right
 *回调函数：_Callback_Dialog_CheckItem_Right
 *标题    ：
 *显示内容：检测项目和检测内容
 *按钮    ：
************************************************************
*/
#define RIGHT_WIN_X          418
#define RIGHT_WIN_Y          5
#define RIGHT_WIN_W          365
#define RIGHT_WIN_H          (RESULT_Y_END_POS-1)

//djq modify 20160809  修改开机自检项目
static const GUI_WIDGET_CREATE_INFO _Dialog_CheckItem_Right[]={
	{ WINDOW_CreateIndirect, " ",     0,RIGHT_WIN_X,RIGHT_WIN_Y,RIGHT_WIN_W,RIGHT_WIN_H,WM_CF_SHOW,0},//301,105,280,573
    { TEXT_CreateIndirect,   "",  GUI_ID_TEXT0,  0, 38, RIGHT_WIN_W,  25, GUI_TA_HCENTER|GUI_TA_VCENTER},  //“最后一次检测”，“正在检测”……    大标题
    { TEXT_CreateIndirect,   "",  GUI_ID_TEXT1,  0, 65, RIGHT_WIN_W,  21, GUI_TA_HCENTER|GUI_TA_VCENTER},  //  时间
    { TEXT_CreateIndirect,   "",  GUI_ID_TEXT2,  125,90, 45,  25, 0}, // 成功
    { TEXT_CreateIndirect,   "",  GUI_ID_TEXT3,  165+5,90, 40,  25, 0}, // 失败
    { TEXT_CreateIndirect,   "",  GUI_ID_TEXT4,  205+5,90, 40,  25, 0}, // 跳过
	//大气压力传感器
	{ TEXT_CreateIndirect,   "",  BAROMETER_RESULT_ID+0,  TITLE_X_START_POS, TITLE_Y_START_POS(0), 185, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  BAROMETER_RESULT_ID+1,  ICO_X_START_POS,   TITLE_Y_START_POS(0), 175, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  BAROMETER_RESULT_ID+2,  RESULT_X_START,    TITLE_Y_START_POS(0), 100, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  BAROMETER_RESULT_ID+3,  ERR_X_START,       TITLE_Y_START_POS(0), 100, TEXT_HEIGHT, GUI_TA_VCENTER},
	//气源
	{ TEXT_CreateIndirect,   "",  GAS_SUPPLY_ID+0,  TITLE_X_START_POS, TITLE_Y_START_POS(1), 185, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  GAS_SUPPLY_ID+1,  ICO_X_START_POS,   TITLE_Y_START_POS(1), 185, TEXT_HEIGHT, GUI_TA_VCENTER},//O2
	{ TEXT_CreateIndirect,   "",  GAS_SUPPLY_ID+2,	RESULT_X_START,    TITLE_Y_START_POS(1), 185, TEXT_HEIGHT, GUI_TA_VCENTER},
	{ TEXT_CreateIndirect,   "",  GAS_SUPPLY_ID+3,	ERR_X_START,       TITLE_Y_START_POS(1), 185, TEXT_HEIGHT, GUI_TA_VCENTER},//N2O
	//回路流量传感器
	{ TEXT_CreateIndirect,   "",  FLOW_SENSOR_ID+0,   TITLE_X_START_POS, TITLE_Y_START_POS(2), 190, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  FLOW_SENSOR_ID+1,	  ICO_X_START_POS,   TITLE_Y_START_POS(2), 175, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  FLOW_SENSOR_ID+2,	  RESULT_X_START,    TITLE_Y_START_POS(2), 100, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  FLOW_SENSOR_ID+3,	  ERR_X_START,       TITLE_Y_START_POS(2), 100, TEXT_HEIGHT, GUI_TA_VCENTER},
	//压力传感器
	{ TEXT_CreateIndirect,   "",  PRESSURE_SENSOR_ID+0,   TITLE_X_START_POS, TITLE_Y_START_POS(3), 185, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  PRESSURE_SENSOR_ID+1,   ICO_X_START_POS,   TITLE_Y_START_POS(3), 175, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  PRESSURE_SENSOR_ID+2,   RESULT_X_START,    TITLE_Y_START_POS(3), 100, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  PRESSURE_SENSOR_ID+3,   ERR_X_START,       TITLE_Y_START_POS(3), 100, TEXT_HEIGHT, GUI_TA_VCENTER},
	//新鲜气体流量传感器  EGM
	{ TEXT_CreateIndirect,   "",  FG_FLOW_SENSOR_ID+0,  TITLE_X_START_POS, TITLE_Y_START_POS(4), 185, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  FG_FLOW_SENSOR_ID+1,  ICO_X_START_POS,   TITLE_Y_START_POS(4), 175, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  FG_FLOW_SENSOR_ID+2,  RESULT_X_START,    TITLE_Y_START_POS(4), 100, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  FG_FLOW_SENSOR_ID+3,  ERR_X_START,       TITLE_Y_START_POS(4), 100, TEXT_HEIGHT, GUI_TA_VCENTER},
    //氧浓度传感器
    { TEXT_CreateIndirect,   "",  OXYGEN_SENSOR_ID+0,  TITLE_X_START_POS,  TITLE_Y_START_POS(5), 185, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  OXYGEN_SENSOR_ID+1,  ICO_X_START_POS,    TITLE_Y_START_POS(5), 175, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  OXYGEN_SENSOR_ID+2,  RESULT_X_START,     TITLE_Y_START_POS(5), 100, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  OXYGEN_SENSOR_ID+3,  ERR_X_START,        TITLE_Y_START_POS(5), 100, TEXT_HEIGHT, GUI_TA_VCENTER},
	//患者管路顺应性
	{ TEXT_CreateIndirect,   "",  COMPLIANCE_ID+0,  TITLE_X_START_POS, TITLE_Y_START_POS(6), 185, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  COMPLIANCE_ID+1,	ICO_X_START_POS,   TITLE_Y_START_POS(6), 175, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  COMPLIANCE_ID+2,	RESULT_X_START,    TITLE_Y_START_POS(6), 100, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  COMPLIANCE_ID+3,  ERR_X_START,       TITLE_Y_START_POS(6), 100, TEXT_HEIGHT, GUI_TA_VCENTER},
	//回路泄露
    { TEXT_CreateIndirect,   "",  INTERNAL_LEAKAGE_ID+0,  TITLE_X_START_POS, TITLE_Y_START_POS(7), 185, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  INTERNAL_LEAKAGE_ID+1,  ICO_X_START_POS,   TITLE_Y_START_POS(7), 175, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  INTERNAL_LEAKAGE_ID+2,  RESULT_X_START,    TITLE_Y_START_POS(7), 100, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  INTERNAL_LEAKAGE_ID+3,  ERR_X_START,       TITLE_Y_START_POS(7), 100, TEXT_HEIGHT, GUI_TA_VCENTER},
	//电池
    { TEXT_CreateIndirect,   "",  BATTERY_ID+0,  TITLE_X_START_POS, TITLE_Y_START_POS(8), 185, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  BATTERY_ID+1,  ICO_X_START_POS,   TITLE_Y_START_POS(8), 175, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  BATTERY_ID+2,  RESULT_X_START,    TITLE_Y_START_POS(8), 100, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  BATTERY_ID+3,  ERR_X_START,       TITLE_Y_START_POS(8), 100, TEXT_HEIGHT, GUI_TA_VCENTER},

    //Frame Text
    { TEXT_CreateIndirect,   "",  GUI_ID_TEXT20,  0, TITAL_Y_HEIGHT, LEFT_WIN_W, 4, 0},  //边框
    ////PEEP阀
    //{ TEXT_CreateIndirect,   "",  PEEP_VALVE_ID+0,   TITLE_X_START_POS, 310, 185, 20, GUI_TA_VCENTER},
    //   { TEXT_CreateIndirect,   "",  PEEP_VALVE_ID+1,               180, 310, 175, 20, GUI_TA_VCENTER},
    //   { TEXT_CreateIndirect,   "",  PEEP_VALVE_ID+2,               198, 310, 100, 20, GUI_TA_VCENTER},
    //   { TEXT_CreateIndirect,   "",  PEEP_VALVE_ID+3,               235, 310, 100, 20, GUI_TA_VCENTER},
    ////安全阀
    //{ TEXT_CreateIndirect,   "",  SAFE_VALVE_ID+0,  TITLE_X_START_POS, 335, 185, 20, GUI_TA_VCENTER},
    //   { TEXT_CreateIndirect,   "",  SAFE_VALVE_ID+1,              180, 335, 175, 20, GUI_TA_VCENTER},
    //   { TEXT_CreateIndirect,   "",  SAFE_VALVE_ID+1,              198, 335, 100, 20, GUI_TA_VCENTER},
    //   { TEXT_CreateIndirect,   "",  SAFE_VALVE_ID+2,              235, 335, 100, 20, GUI_TA_VCENTER},
    ////Pop-off阀
    //{ TEXT_CreateIndirect,   "",  POP_OFF_VALVE_ID+0,  TITLE_X_START_POS, 335, 185, 20, GUI_TA_VCENTER},
    //   { TEXT_CreateIndirect,   "",  POP_OFF_VALVE_ID+1,              180, 335, 175, 20, GUI_TA_VCENTER},
    //   { TEXT_CreateIndirect,   "",  POP_OFF_VALVE_ID+2,              198, 335, 100, 20, GUI_TA_VCENTER},
    //   { TEXT_CreateIndirect,   "",  POP_OFF_VALVE_ID+3,              235, 335, 100, 20, GUI_TA_VCENTER},
	////ACGO
 //   { TEXT_CreateIndirect,   "",  ACGO_ID+0,  TITLE_X_START_POS, 460, 185, 20, GUI_TA_VCENTER},
 //   { TEXT_CreateIndirect,   "",  ACGO_ID+1,              180, 460, 175, 20, GUI_TA_VCENTER},
 //   { TEXT_CreateIndirect,   "",  ACGO_ID+2,              198, 460, 100, 20, GUI_TA_VCENTER},
 //   { TEXT_CreateIndirect,   "",  ACGO_ID+3,              235, 460, 100, 20, GUI_TA_VCENTER},
};
/********************************************************
 *函数名：_Callback_Dialog_CheckItem_Right
 *参数  ：系统消息
 *返回值：无
 *被调用位置：系统调用
 *功能  ：更新检测项目的显示
*********************************************************
*/
static void _Callback_Dialog_CheckItem_Right(WM_MESSAGE *pMsg)
{
	WM_HWIN BUTTON_CENTER,TEXT_MSG_HANDLE;
    WM_HWIN ResultHwinCL[4],ResultHwinCC[4];
	U32 Id, IdCode;
	U8 year,mon,day,hour,min;
	U8 cnt,cnt_01;
	double result_double = 0;
	U8 string[40];
	CheckItem_Right_HANDLE = pMsg->hWin;
	switch(pMsg->MsgId)
	{
	    case WM_INIT_DIALOG:
			WINDOW_SetBkColor(pMsg->hWin,DialogBk_Color);
			//bar
			TEXT_SetFont(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT0),&GUI_Font_24);
			TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT0),GUI_WHITE);
            TEXT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT0),Sys_Lib_GetDispStr(DP_LASTAUTOSYSTEST));
			//title
			TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT1),GUI_WHITE);

			TEXT_SetFont(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT1),&GUI_Font_16);
			TEXT_SetFont(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT2),&GUI_Font_16);
			TEXT_SetFont(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT3),&GUI_Font_16);
			TEXT_SetFont(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT4),&GUI_Font_16);
			TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT2),GUI_GREEN);
			TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT3),GUI_RED);
			TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT4),GUI_WHITE);
			//Check Item   自检待修改
			Init_GlobalParameter(DISPLAY_RESULT,0);//初始化全局参数
            //Check Result
            for(cnt = POS_Barometer;cnt < CHECKITEM_NUM;cnt++)//第一项为手动检测，所以从第二项开始
            {
                if(cnt == POS_GasSypply)
                {
                    //设置检测项目字体颜色
                    TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin,CheckItem_Result_Id_Init[cnt]+0),           GUI_WHITE);
                    //显示检测项目
                    TEXT_SetText(WM_GetDialogItem(pMsg->hWin,CheckItem_Result_Id_Init[cnt]+0),Desktop_CheckItem_Text[cnt]);
                    //设置检测项目字体
                    TEXT_SetFont(WM_GetDialogItem(pMsg->hWin,CheckItem_Result_Id_Init[cnt]+0),&GUI_Font_16);

                    for(cnt_01 = 1;cnt_01<=6;cnt_01++)
                    {
                    //设置检测结果字体
                        TEXT_SetFont(WM_GetDialogItem(pMsg->hWin,CheckItem_Result_Id_Init[cnt]+cnt_01),&GUI_Font_16);
                    //设置监测结果字体颜色
                        TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin,CheckItem_Result_Id_Init[cnt]+cnt_01),CheckResult_Font_Color);
                    }
                }
                else
                {
                    //设置检测项目字体颜色
                    TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin,CheckItem_Result_Id_Init[cnt]+0),           GUI_WHITE);
                    //显示检测项目
                    TEXT_SetText(WM_GetDialogItem(pMsg->hWin,CheckItem_Result_Id_Init[cnt]+0),Desktop_CheckItem_Text[cnt]);
                    //设置检测项目字体
                    TEXT_SetFont(WM_GetDialogItem(pMsg->hWin,CheckItem_Result_Id_Init[cnt]+0),&GUI_Font_16);
                    for(cnt_01 = 1;cnt_01 <= 3;cnt_01++)
                    {
                        //设置字体
                        TEXT_SetFont(WM_GetDialogItem(pMsg->hWin,CheckItem_Result_Id_Init[cnt]+cnt_01),&GUI_Font_16);
                        //设置监测结果字体颜色
                        TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin,CheckItem_Result_Id_Init[cnt]+cnt_01),CheckResult_Font_Color);
                    }
                }
            }

////////结果
            Disp_ItemResult(POS_Barometer);
            Disp_ItemResult(POS_GasSypply);
            Disp_ItemResult(POS_Flow_S);
            Disp_ItemResult(POS_Pressure_S);
            //Disp_ItemResult(POS_FG_Flow_ZERO_S);
            Disp_ItemResult(POS_Oxygen);
            Disp_ItemResult(POS_Compliance);
            Disp_ItemResult(POS_Circuit_Leakage);
            Disp_ItemResult(POS_Battery);


			year = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_AUTOMATIC_YEAR_OFFSET_ADDR);
            mon = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_AUTOMATIC_MONTH_OFFSET_ADDR);
            day = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_AUTOMATIC_DAY_OFFSET_ADDR);
            hour = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_AUTOMATIC_HOUR_OFFSET_ADDR);
            min = RTEEPROMread(MEM_OF_CHECKRESULT_ADDR + MEM_AUTOMATIC_MIN_OFFSET_ADDR);
            sprintf(string,"20%02d-%02d-%02d %02d:%02d",year,mon,day,hour,min);
            TEXT_SetText(WM_GetDialogItem(CheckItem_Right_HANDLE,GUI_ID_TEXT1),string);
			//检测状态统计
            sprintf(string,"%s %02d",Sys_Lib_GetDispStr(DP_TICK),CONTEXT_APP.CheckInfo.Result_Report.Automatic[(RESULT_SUCCESS>>2)]);
            TEXT_SetText(WM_GetDialogItem(CheckItem_Right_HANDLE,GUI_ID_TEXT2),string);
            sprintf(string,"%s %02d",Sys_Lib_GetDispStr(DP_CROSS),CONTEXT_APP.CheckInfo.Result_Report.Automatic[(RESULT_FAULT>>2)]);
            TEXT_SetText(WM_GetDialogItem(CheckItem_Right_HANDLE,GUI_ID_TEXT3),string);
            sprintf(string,"%s %02d",Sys_Lib_GetDispStr(DP_DASHSMALL),CONTEXT_APP.CheckInfo.Result_Report.Automatic[(RESULT_SKIP>>2)]);
            TEXT_SetText(WM_GetDialogItem(CheckItem_Right_HANDLE,GUI_ID_TEXT4),string);
            //Frame
			TEXT_SetBkColor(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT20),Back_Color);
			break;
		case WM_PAINT://重绘时，对话框的用户区是目标窗口
			WINDOW_SetBkColor(pMsg->hWin,DialogBk_Color);
            //Frame 
			TEXT_SetBkColor(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT20),Back_Color);
			break;
		default:
            WM_DefaultProc(pMsg);
            break;
	}
}
/***********************************************************
 *资源名  ：_Dialog_CheckItem_Total
 *回调函数：_Callback_Dialog_CheckItem_Total
 *标题    ：
 *显示内容：检测项目和检测内容
 *按钮    ：
************************************************************
*/
#define TOTAL_WIN_X          18
#define TOTAL_WIN_Y          5
#define TOTAL_WIN_W          763+5
#define TOTAL_WIN_H          480

static const GUI_WIDGET_CREATE_INFO _Dialog_CheckItem_Total[]={
	{ WINDOW_CreateIndirect, " ",     0,             TOTAL_WIN_X,TOTAL_WIN_Y,TOTAL_WIN_W,TOTAL_WIN_H,WM_CF_SHOW,0},//302,105,280,566
    { TEXT_CreateIndirect,   "",  GUI_ID_TEXT5,      10, 6, 95/*65*/, 20, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  GUI_ID_TEXT6,      120/*70*/, 6, 95/*65*/, 20, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  GUI_ID_TEXT7,      225/*130*/,6, 95/*65*/, 20, GUI_TA_VCENTER},
    { ProcessWidget_CreateIndirect,"",GUI_ID_PROCESSWIDGET,  315, 100, 140, 10},



    { TEXT_CreateIndirect,   "",  GUI_ID_TEXT0,  0, 38, SELFTEST_WINDOW_W-34,  25, GUI_TA_HCENTER|GUI_TA_VCENTER},  //“手动/机控检测”，“正在检测”……    大标题
    { TEXT_CreateIndirect,   "",  GUI_ID_TEXT1,  0, 60, (SELFTEST_WINDOW_W)/2+40,  25, GUI_TA_RIGHT|GUI_TA_VCENTER},  //  时间
    { TEXT_CreateIndirect,   "",  GUI_ID_TEXT2,  (SELFTEST_WINDOW_W)/2+X_MARGIN+30+200-3,  60, 40,  25, GUI_TA_HCENTER|GUI_TA_VCENTER}, // 成功
    { TEXT_CreateIndirect,   "",  GUI_ID_TEXT3,  (SELFTEST_WINDOW_W)/2+X_MARGIN+70+200-3  ,60, 40,  25, GUI_TA_HCENTER|GUI_TA_VCENTER}, // 失败
    { TEXT_CreateIndirect,   "",  GUI_ID_TEXT4,  (SELFTEST_WINDOW_W)/2+X_MARGIN+110+200-3,60, 40,  25, GUI_TA_HCENTER|GUI_TA_VCENTER}, // 跳过
	//Check Items
	//
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_00_ID+0,  TITLE_X_START_POS, TITLE_Y_START_POS(0), 570, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_00_ID+1,  ICO_X_START_POS,   TITLE_Y_START_POS(0), 185, TEXT_HEIGHT, GUI_TA_VCENTER},
	{ TEXT_CreateIndirect,   "",  TOTAL_TEXT_00_ID+2,  RESULT_X_START,    TITLE_Y_START_POS(0), 185, TEXT_HEIGHT, GUI_TA_VCENTER},
	{ TEXT_CreateIndirect,   "",  TOTAL_TEXT_00_ID+3,  ERR_X_START,       TITLE_Y_START_POS(0), 185, TEXT_HEIGHT, GUI_TA_VCENTER},
    //
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_01_ID+0,  TITLE_X_START_POS, TITLE_Y_START_POS(1), 570, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_01_ID+1,  ICO_X_START_POS,   TITLE_Y_START_POS(1), 175, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_01_ID+2,  RESULT_X_START,    TITLE_Y_START_POS(1), 200, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_01_ID+3,  ERR_X_START,       TITLE_Y_START_POS(1), 140, TEXT_HEIGHT, GUI_TA_VCENTER},
    //
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_02_ID+0,  TITLE_X_START_POS, TITLE_Y_START_POS(2), 570, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_02_ID+1,  ICO_X_START_POS,   TITLE_Y_START_POS(2), 175, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_02_ID+2,  RESULT_X_START,    TITLE_Y_START_POS(2), 200, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_02_ID+3,  ERR_X_START,       TITLE_Y_START_POS(2), 140, TEXT_HEIGHT, GUI_TA_VCENTER},
	//
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_03_ID+0,  TITLE_X_START_POS, TITLE_Y_START_POS(3), 570+140, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_03_ID+1,  ICO_X_START_POS,   TITLE_Y_START_POS(3), 175, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_03_ID+2,  RESULT_X_START,    TITLE_Y_START_POS(3), 200, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_03_ID+3,  ERR_X_START,       TITLE_Y_START_POS(3), 140, TEXT_HEIGHT, GUI_TA_VCENTER},
	//
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_04_ID+0,  TITLE_X_START_POS, TITLE_Y_START_POS(4), 570, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_04_ID+1,  ICO_X_START_POS,   TITLE_Y_START_POS(4), 175, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_04_ID+2,  RESULT_X_START,    TITLE_Y_START_POS(4), 200, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_04_ID+3,  ERR_X_START,       TITLE_Y_START_POS(4), 140, TEXT_HEIGHT, GUI_TA_VCENTER},
	//
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_05_ID+0,  TITLE_X_START_POS, TITLE_Y_START_POS(5), 570, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_05_ID+1,  ICO_X_START_POS,   TITLE_Y_START_POS(5), 175, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_05_ID+2,  RESULT_X_START,    TITLE_Y_START_POS(5), 200, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_05_ID+3,  ERR_X_START,       TITLE_Y_START_POS(5), 140, TEXT_HEIGHT, GUI_TA_VCENTER},
	//
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_06_ID+0,  TITLE_X_START_POS, TITLE_Y_START_POS(6), 570, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_06_ID+1,  ICO_X_START_POS,   TITLE_Y_START_POS(6), 175, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_06_ID+2,  RESULT_X_START,    TITLE_Y_START_POS(6), 200, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_06_ID+3,  ERR_X_START,       TITLE_Y_START_POS(6), 140, TEXT_HEIGHT, GUI_TA_VCENTER},
	//
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_07_ID+0,  TITLE_X_START_POS, TITLE_Y_START_POS(7), 570, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_07_ID+1,  ICO_X_START_POS,   TITLE_Y_START_POS(7), 175, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_07_ID+2,  RESULT_X_START,    TITLE_Y_START_POS(7), 200, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_07_ID+3,  ERR_X_START,       TITLE_Y_START_POS(7), 140, TEXT_HEIGHT, GUI_TA_VCENTER},
	//
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_08_ID+0,  TITLE_X_START_POS, TITLE_Y_START_POS(8), 570, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_08_ID+1,  ICO_X_START_POS,   TITLE_Y_START_POS(8), 175, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_08_ID+2,  RESULT_X_START,    TITLE_Y_START_POS(8), 200, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_08_ID+3,  ERR_X_START,       TITLE_Y_START_POS(8), 140, TEXT_HEIGHT, GUI_TA_VCENTER},
	//
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_09_ID+0,  TITLE_X_START_POS, TITLE_Y_START_POS(9), 570+190, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_09_ID+1,  ICO_X_START_POS,   TITLE_Y_START_POS(9), 175, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_09_ID+2,  RESULT_X_START,    TITLE_Y_START_POS(9), 200, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_10_ID+3,  ERR_X_START,       TITLE_Y_START_POS(9), 140, TEXT_HEIGHT, GUI_TA_VCENTER},
	//
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_10_ID+0,  TITLE_X_START_POS, TITLE_Y_START_POS(10), 570, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_10_ID+1,  ICO_X_START_POS,   TITLE_Y_START_POS(10), 175, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_10_ID+2,  RESULT_X_START,    TITLE_Y_START_POS(10), 200, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_10_ID+3,  ERR_X_START,       TITLE_Y_START_POS(10), 140, TEXT_HEIGHT, GUI_TA_VCENTER},
	//
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_11_ID+0,  TITLE_X_START_POS, TITLE_Y_START_POS(11), 570, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_11_ID+1,  ICO_X_START_POS,   TITLE_Y_START_POS(11), 175, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_11_ID+2,  RESULT_X_START,    TITLE_Y_START_POS(11), 200, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_11_ID+3,  ERR_X_START,       TITLE_Y_START_POS(11), 140, TEXT_HEIGHT, GUI_TA_VCENTER},
	//
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_12_ID+0,  TITLE_X_START_POS, TITLE_Y_START_POS(12), 570, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_12_ID+1,  ICO_X_START_POS,   TITLE_Y_START_POS(12), 175, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_12_ID+2,  RESULT_X_START,    TITLE_Y_START_POS(12), 200, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_12_ID+3,  ERR_X_START,       TITLE_Y_START_POS(12), 140, TEXT_HEIGHT, GUI_TA_VCENTER},
	//
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_13_ID+0,  TITLE_X_START_POS, TITLE_Y_START_POS(13), 570, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_13_ID+1,  ICO_X_START_POS,   TITLE_Y_START_POS(13), 175, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_13_ID+2,  RESULT_X_START,    TITLE_Y_START_POS(13), 200, TEXT_HEIGHT, GUI_TA_VCENTER},
    { TEXT_CreateIndirect,   "",  TOTAL_TEXT_13_ID+3,  ERR_X_START,       TITLE_Y_START_POS(13), 140, TEXT_HEIGHT, GUI_TA_VCENTER},

    //Frame Text
    { TEXT_CreateIndirect,   "",  GUI_ID_TEXT20,  0, TITAL_Y_HEIGHT, SELFTEST_WINDOW_W, 4, 0},  //边框
};
/********************************************************
 *函数名：_Callback_Dialog_CheckItem_Total
 *参数  ：系统消息
 *返回值：无
 *被调用位置：系统调用
 *功能  ：更新检测项目的显示
*********************************************************
*/
static void _Callback_Dialog_CheckItem_Total(WM_MESSAGE *pMsg)
{
	WM_HWIN BUTTON_CENTER,TEXT_MSG_HANDLE;
	WM_HWIN hTextItem[TOTAL_TEXT_MAX_NUM];
	U32 Id, IdCode;
	U8 year,mon,day,hour,min;
	U8 cnt;
	double result_double = 0;
	U8 string[40];
	U8 tmp[250] = {0};
	switch(pMsg->MsgId)
	{
	    case WM_INIT_DIALOG:
			WINDOW_SetBkColor(pMsg->hWin,DialogBk_Color);
			//bar
			TEXT_SetFont(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT0),&GUI_Font_24);
			TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT0),GUI_WHITE);
			//Time
			TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT1),GUI_WHITE);

			TEXT_SetFont(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT1),&GUI_Font_16);
			TEXT_SetFont(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT2),&GUI_Font_16);
			TEXT_SetFont(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT3),&GUI_Font_16);
			TEXT_SetFont(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT4),&GUI_Font_16);
			TEXT_SetFont(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT5),&GUI_Font_16);
            TEXT_SetFont(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT6),&GUI_Font_16);
            TEXT_SetFont(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT7),&GUI_Font_16);
			//TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT2),GUI_GREEN);
			//TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT3),GUI_RED);
			//TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT4),GUI_WHITE);//del 20170207

            //提示 成功  失败  跳过  的颜色
            TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT5),GUI_GREEN);
            sprintf(string,"%s%s",Sys_Lib_GetDispStr(DP_TICK),Sys_Lib_GetDispStr(DP_PASS));
            TEXT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT5),string);

            TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT6),GUI_RED);
            sprintf(string,"%s%s",Sys_Lib_GetDispStr(DP_CROSS),Sys_Lib_GetDispStr(DP_FAIL));
            TEXT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT6),string);

            TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT7),GUI_WHITE);
			sprintf(string,"%s%s",Sys_Lib_GetDispStr(DP_DASHSMALL),Sys_Lib_GetDispStr(DP_SKIP));
            TEXT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT7),string);			

			//Check Item 
			for(cnt = 0; cnt < TOTAL_TEXT_MAX_NUM; cnt++)
			{
                //设置提示信息字体、颜色
				hTextItem[cnt] = WM_GetDialogItem(pMsg->hWin,CheckItem_Result_Id[cnt]);
			    TEXT_SetFont(hTextItem[cnt],      &GUI_Font_16);
			    TEXT_SetTextColor(hTextItem[cnt], GUI_WHITE);
				//TEXT_SetText(hTextItem[cnt],Desktop_CheckItem_Text[cnt+1]);

                ////设置检测结果字体、颜色，只有在第一次进入检测的时候才刷新字体颜色，否则不变。
                if((CONTEXT_APP.Checking_Control == Ctrl_ManualTest)
                  ||(CONTEXT_APP.Checking_Control == Ctrl_AutoTest1))
                {
                    TEXT_SetFont(WM_GetDialogItem(pMsg->hWin,CheckItem_Result_Id[cnt]+1),&GUI_Font_16);
                    TEXT_SetFont(WM_GetDialogItem(pMsg->hWin,CheckItem_Result_Id[cnt]+2),&GUI_Font_16);
                    TEXT_SetFont(WM_GetDialogItem(pMsg->hWin,CheckItem_Result_Id[cnt]+3),&GUI_Font_16);

                    TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin,CheckItem_Result_Id[cnt]+1),  CheckResult_Font_Color);
                    TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin,CheckItem_Result_Id[cnt]+2),  CheckResult_Font_Color);
                    TEXT_SetTextColor(WM_GetDialogItem(pMsg->hWin,CheckItem_Result_Id[cnt]+3),  CheckResult_Font_Color);
                }
			}
            //Check Result			

            if(CONTEXT_APP.Checking_Control == Ctrl_ManualPrompt)//手动提示
			{
				TEXT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT0),Sys_Lib_GetDispStr(DP_MANUALLEAKTEST));
				TEXT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT1),Sys_Lib_GetDispStr(DP_EMPTY));
                TEXT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT2),Sys_Lib_GetDispStr(DP_EMPTY));
				TEXT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT3),Sys_Lib_GetDispStr(DP_EMPTY));
                TEXT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT4),Sys_Lib_GetDispStr(DP_EMPTY));
				sprintf(tmp, "1.%s", Sys_Lib_GetDispStr(DP_SWITCHTOBAG));
			    TEXT_SetText(hTextItem[0], tmp);
				sprintf(tmp, "2.%s", Sys_Lib_GetDispStr(DP_CONNYPABSORTESTBLOCK));
				TEXT_SetText(hTextItem[1], tmp);
				sprintf(tmp, "3.%s", Sys_Lib_GetDispStr(DP_MANUALBAGCONNECT));
				TEXT_SetText(hTextItem[2], tmp);
				sprintf(tmp, "4.%s", Sys_Lib_GetDispStr(DP_CONNGASPIPELINE));
				TEXT_SetText(hTextItem[3], tmp);
				sprintf(tmp, "5.%s", Sys_Lib_GetDispStr(DP_CHECKABSORBERWATER));
				TEXT_SetText(hTextItem[4], tmp);
				sprintf(tmp, "6.%s", Sys_Lib_GetDispStr(DP_SETFLOWCONTROLMIN));
				TEXT_SetText(hTextItem[5], tmp);
				sprintf(tmp, "7.%s", Sys_Lib_GetDispStr(DP_TURNOFFVAPOR));
				TEXT_SetText(hTextItem[6], tmp);
				sprintf(tmp, "8.%s", Sys_Lib_GetDispStr(DP_SETACGOOFF));
				TEXT_SetText(hTextItem[7], tmp);
				sprintf(tmp, "10.%s", Sys_Lib_GetDispStr(DP_AIRWAYPRESSURE30AND40));
				TEXT_SetText(hTextItem[9], tmp);
#if (LOGO_FLAG == PENLON_LOGO)	
				sprintf(tmp, "9.%s", Sys_Lib_GetDispStr(DP_SETAPLVALVETO50));
				TEXT_SetText(hTextItem[8], tmp);
				sprintf(tmp, "11.%s", Sys_Lib_GetDispStr(DP_SETACGOON));
				TEXT_SetText(hTextItem[10], tmp);
				sprintf(tmp, "12.%s", Sys_Lib_GetDispStr(DP_PERFORMMANULEAKTEST));
				TEXT_SetText(hTextItem[11], tmp);
#else
				sprintf(tmp, "9.%s", Sys_Lib_GetDispStr(DP_SETAPLVALVE70));
				TEXT_SetText(hTextItem[8], tmp);
				sprintf(tmp, "11.%s", Sys_Lib_GetDispStr(DP_PERFORMMANULEAKTEST));
				TEXT_SetText(hTextItem[10], tmp);
				sprintf(tmp, "12.%s", Sys_Lib_GetDispStr(DP_EMPTY));
				TEXT_SetText(hTextItem[11], tmp);
				TEXT_SetText(hTextItem[11],Sys_Lib_GetDispStr(DP_EMPTY));
#endif
				TEXT_SetText(hTextItem[12],Sys_Lib_GetDispStr(DP_EMPTY));
				TEXT_SetText(hTextItem[13],Sys_Lib_GetDispStr(DP_EMPTY));

                //清除结果信息
                for(cnt = 0; cnt < TOTAL_TEXT_MAX_NUM; cnt++)
                {
                    TEXT_SetText(WM_GetDialogItem(pMsg->hWin,CheckItem_Result_Id[cnt]+1),Sys_Lib_GetDispStr(DP_EMPTY));
                    TEXT_SetText(WM_GetDialogItem(pMsg->hWin,CheckItem_Result_Id[cnt]+2),Sys_Lib_GetDispStr(DP_EMPTY));
                    TEXT_SetText(WM_GetDialogItem(pMsg->hWin,CheckItem_Result_Id[cnt]+3),Sys_Lib_GetDispStr(DP_EMPTY));
                }
			}
			else if(CONTEXT_APP.Checking_Control == Ctrl_ManualTest)//手动检测
			{
				TEXT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT0),Sys_Lib_GetDispStr(DP_MANUALLEAKTEST));
			    TEXT_SetText(hTextItem[0], Desktop_CheckItem_Text[POS_Manual]);
                //清除标题信息
				for(cnt = 1; cnt < TOTAL_TEXT_MAX_NUM; cnt++)
				{
				    TEXT_SetText(hTextItem[cnt], Sys_Lib_GetDispStr(DP_EMPTY));
				}
                //清除结果信息
                for(cnt = 0; cnt < TOTAL_TEXT_MAX_NUM; cnt++)
                {
                    TEXT_SetText(WM_GetDialogItem(pMsg->hWin,CheckItem_Result_Id[cnt]+1),Sys_Lib_GetDispStr(DP_EMPTY));
                    TEXT_SetText(WM_GetDialogItem(pMsg->hWin,CheckItem_Result_Id[cnt]+2),Sys_Lib_GetDispStr(DP_EMPTY));
                    TEXT_SetText(WM_GetDialogItem(pMsg->hWin,CheckItem_Result_Id[cnt]+3),Sys_Lib_GetDispStr(DP_EMPTY));
                }
			}
			else if(CONTEXT_APP.Checking_Control == Ctrl_AutoPrompt1)//机控提示
			{
				TEXT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT0),Sys_Lib_GetDispStr(DP_AUTOSYSTEMTEST));
				TEXT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT1),Sys_Lib_GetDispStr(DP_EMPTY));
				TEXT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT2),Sys_Lib_GetDispStr(DP_EMPTY));
				TEXT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT3),Sys_Lib_GetDispStr(DP_EMPTY));
				TEXT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT4),Sys_Lib_GetDispStr(DP_EMPTY));
				sprintf(tmp, "1.%s", Sys_Lib_GetDispStr(DP_SWITCHTOVENT));
				TEXT_SetText(hTextItem[0], tmp);
				sprintf(tmp, "2.%s", Sys_Lib_GetDispStr(DP_OPENPATIENTYPIECE));
				TEXT_SetText(hTextItem[1], tmp);
				sprintf(tmp, "3.%s", Sys_Lib_GetDispStr(DP_CHECKABSORBERWATER));
				TEXT_SetText(hTextItem[2], tmp);
				sprintf(tmp, "4.%s", Sys_Lib_GetDispStr(DP_SETFLOWCONTROLMIN));
				TEXT_SetText(hTextItem[3], tmp);
				sprintf(tmp, "5.%s", Sys_Lib_GetDispStr(DP_TURNOFFVAPOR));
				TEXT_SetText(hTextItem[4], tmp);
				sprintf(tmp, "6.%s", Sys_Lib_GetDispStr(DP_SETACGOOFF));
				TEXT_SetText(hTextItem[5], tmp);
				sprintf(tmp, "7.%s", Sys_Lib_GetDispStr(DP_BELLOWCOLLAPSED));
				TEXT_SetText(hTextItem[6], tmp);
				sprintf(tmp, "8.%s", Sys_Lib_GetDispStr(DP_PREFORMAUTOSYSTEST));
				TEXT_SetText(hTextItem[7], tmp);
				TEXT_SetText(hTextItem[8], Sys_Lib_GetDispStr(DP_EMPTY));
				TEXT_SetText(hTextItem[9], Sys_Lib_GetDispStr(DP_EMPTY));
				TEXT_SetText(hTextItem[10],Sys_Lib_GetDispStr(DP_EMPTY));
				TEXT_SetText(hTextItem[11],Sys_Lib_GetDispStr(DP_EMPTY));
				TEXT_SetText(hTextItem[12],Sys_Lib_GetDispStr(DP_EMPTY));
				TEXT_SetText(hTextItem[13],Sys_Lib_GetDispStr(DP_EMPTY));

                //清除结果信息
                for(cnt = 0; cnt < TOTAL_TEXT_MAX_NUM; cnt++)
                {
                    TEXT_SetText(WM_GetDialogItem(pMsg->hWin,CheckItem_Result_Id[cnt]+1),Sys_Lib_GetDispStr(DP_EMPTY));
                    TEXT_SetText(WM_GetDialogItem(pMsg->hWin,CheckItem_Result_Id[cnt]+2),Sys_Lib_GetDispStr(DP_EMPTY));
                    TEXT_SetText(WM_GetDialogItem(pMsg->hWin,CheckItem_Result_Id[cnt]+3),Sys_Lib_GetDispStr(DP_EMPTY));
                }
			}
			else if(CONTEXT_APP.Checking_Control == Ctrl_AutoTest1)//机控检测
			{
				TEXT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT0),Sys_Lib_GetDispStr(DP_AUTOSYSTEMTEST));
                TEXT_SetText(hTextItem[0], Desktop_CheckItem_Text[POS_Barometer]);//djq add 20160809
                TEXT_SetText(hTextItem[1], Desktop_CheckItem_Text[POS_GasSypply]);
				TEXT_SetText(hTextItem[2], Desktop_CheckItem_Text[POS_Flow_S]);
				TEXT_SetText(hTextItem[3], Desktop_CheckItem_Text[POS_Pressure_S]);
				if(Context_GetO2PromptFlag())
				{
					TEXT_SetText(hTextItem[4], Sys_Lib_GetDispStr(DP_EMPTY));
				}
				else
				{
					TEXT_SetText(hTextItem[4], Desktop_CheckItem_Text[POS_Oxygen]);	
				}
                TEXT_SetText(hTextItem[5], Sys_Lib_GetDispStr(DP_EMPTY));
                TEXT_SetText(hTextItem[6], Sys_Lib_GetDispStr(DP_EMPTY));
                TEXT_SetText(hTextItem[7],Sys_Lib_GetDispStr(DP_EMPTY));
                TEXT_SetText(hTextItem[8],Sys_Lib_GetDispStr(DP_EMPTY));
                TEXT_SetText(hTextItem[9],Sys_Lib_GetDispStr(DP_EMPTY));
                TEXT_SetText(hTextItem[10],Sys_Lib_GetDispStr(DP_EMPTY));

                //清除结果信息
                for(cnt = 0; cnt < TOTAL_TEXT_MAX_NUM; cnt++)
                {
                    TEXT_SetText(WM_GetDialogItem(pMsg->hWin,CheckItem_Result_Id[cnt]+1),Sys_Lib_GetDispStr(DP_EMPTY));
                    TEXT_SetText(WM_GetDialogItem(pMsg->hWin,CheckItem_Result_Id[cnt]+2),Sys_Lib_GetDispStr(DP_EMPTY));
                    TEXT_SetText(WM_GetDialogItem(pMsg->hWin,CheckItem_Result_Id[cnt]+3),Sys_Lib_GetDispStr(DP_EMPTY));
                }			
            }
            else if(CONTEXT_APP.Checking_Control == Ctrl_AutoPrompt2)//机控提示2
            {
                TEXT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT0),Sys_Lib_GetDispStr(DP_AUTOSYSTEMTEST));
                TEXT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT1),Sys_Lib_GetDispStr(DP_EMPTY));
                TEXT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT2),Sys_Lib_GetDispStr(DP_EMPTY));
                TEXT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT3),Sys_Lib_GetDispStr(DP_EMPTY));
                TEXT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT4),Sys_Lib_GetDispStr(DP_EMPTY));
                TEXT_SetText(hTextItem[0], Sys_Lib_GetDispStr(DP_SWITCHBAGPLUGPORT));
				if((Drager_EtCO2_Config_On == 2) || (MasimoISA_EtCO2_Config_On == 2))//ylk add 20181221
				{
					TEXT_SetText(hTextItem[1], Sys_Lib_GetDispStr(DP_ADJUSTOXY));
					TEXT_SetText(hTextItem[2], Sys_Lib_GetDispStr(DP_GASMODULESAMPLEELBOW));
					TEXT_SetText(hTextItem[3], Sys_Lib_GetDispStr(DP_PRESSSTARTBUTTON));
				}
				else
				{
					TEXT_SetText(hTextItem[1], Sys_Lib_GetDispStr(DP_ADJUSTOXY));
					TEXT_SetText(hTextItem[2], Sys_Lib_GetDispStr(DP_PRESSSTARTBUTTON));
					TEXT_SetText(hTextItem[3], Sys_Lib_GetDispStr(DP_EMPTY));
				}
                TEXT_SetText(hTextItem[4], Sys_Lib_GetDispStr(DP_EMPTY));
                TEXT_SetText(hTextItem[5], Sys_Lib_GetDispStr(DP_EMPTY));
                TEXT_SetText(hTextItem[6], Sys_Lib_GetDispStr(DP_EMPTY));
                TEXT_SetText(hTextItem[7], Sys_Lib_GetDispStr(DP_EMPTY));
                TEXT_SetText(hTextItem[8], Sys_Lib_GetDispStr(DP_EMPTY));
                TEXT_SetText(hTextItem[9], Sys_Lib_GetDispStr(DP_EMPTY));
                TEXT_SetText(hTextItem[10],Sys_Lib_GetDispStr(DP_EMPTY));
                TEXT_SetText(hTextItem[11],Sys_Lib_GetDispStr(DP_EMPTY));
                TEXT_SetText(hTextItem[12],Sys_Lib_GetDispStr(DP_EMPTY));
                TEXT_SetText(hTextItem[13],Sys_Lib_GetDispStr(DP_EMPTY));

                //清除结果信息
                for(cnt = 0; cnt < TOTAL_TEXT_MAX_NUM; cnt++)
                {
                    TEXT_SetText(WM_GetDialogItem(pMsg->hWin,CheckItem_Result_Id[cnt]+1),Sys_Lib_GetDispStr(DP_EMPTY));
                    TEXT_SetText(WM_GetDialogItem(pMsg->hWin,CheckItem_Result_Id[cnt]+2),Sys_Lib_GetDispStr(DP_EMPTY));
                    TEXT_SetText(WM_GetDialogItem(pMsg->hWin,CheckItem_Result_Id[cnt]+3),Sys_Lib_GetDispStr(DP_EMPTY));
                }
            }
            else if(CONTEXT_APP.Checking_Control == Ctrl_AutoTest2)//机控检测2
            {
                TEXT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT0),Sys_Lib_GetDispStr(DP_AUTOSYSTEMTEST));
                TEXT_SetText(hTextItem[0], Desktop_CheckItem_Text[POS_Barometer]);//djq add 20160809
                TEXT_SetText(hTextItem[1], Desktop_CheckItem_Text[POS_GasSypply]);
                TEXT_SetText(hTextItem[2], Desktop_CheckItem_Text[POS_Flow_S]);
                TEXT_SetText(hTextItem[3], Desktop_CheckItem_Text[POS_Pressure_S]);
//              TEXT_SetText(hTextItem[4], Desktop_CheckItem_Text[POS_FG_Flow_ZERO_S]);
                TEXT_SetText(hTextItem[4], Desktop_CheckItem_Text[POS_Oxygen]);
                TEXT_SetText(hTextItem[5], Sys_Lib_GetDispStr(DP_EMPTY));
                TEXT_SetText(hTextItem[6], Sys_Lib_GetDispStr(DP_EMPTY));
                TEXT_SetText(hTextItem[7], Sys_Lib_GetDispStr(DP_EMPTY));
                TEXT_SetText(hTextItem[8], Sys_Lib_GetDispStr(DP_EMPTY));
                TEXT_SetText(hTextItem[9], Sys_Lib_GetDispStr(DP_EMPTY));
                TEXT_SetText(hTextItem[10], Sys_Lib_GetDispStr(DP_EMPTY));

                //清除结果信息
                for(cnt = 0; cnt < TOTAL_TEXT_MAX_NUM; cnt++)
                {
                    TEXT_SetText(WM_GetDialogItem(pMsg->hWin,CheckItem_Result_Id[cnt]+1),Sys_Lib_GetDispStr(DP_EMPTY));
                    TEXT_SetText(WM_GetDialogItem(pMsg->hWin,CheckItem_Result_Id[cnt]+2),Sys_Lib_GetDispStr(DP_EMPTY));
                    TEXT_SetText(WM_GetDialogItem(pMsg->hWin,CheckItem_Result_Id[cnt]+3),Sys_Lib_GetDispStr(DP_EMPTY));
                }
                Disp_ItemResult(POS_Barometer);
                Disp_ItemResult(POS_GasSypply);
                Disp_ItemResult(POS_Flow_S);
                Disp_ItemResult(POS_Pressure_S);
                //Disp_ItemResult(POS_FG_Flow_ZERO_S);
                //Disp_ItemResult(POS_Oxygen);//ylk modify 20180718
                //Disp_ItemResult(POS_Compliance);
                //Disp_ItemResult(POS_Circuit_Leakage);
                //Disp_ItemResult(POS_Battery);

            }
            else if(CONTEXT_APP.Checking_Control == Ctrl_AutoPrompt3)//机控提示2
            {
                if(AA_all_data.AA_state.AA_ModuleState.currect_mode == AA_MODE_MEASURE)//ylk add 20181221
                {
                    SendOperationMessage(SET_AA_PUMPOFF,USART2);//关泵
                }
                TEXT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT0),Sys_Lib_GetDispStr(DP_AUTOSYSTEMTEST));
                TEXT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT1),Sys_Lib_GetDispStr(DP_EMPTY));
                TEXT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT2),Sys_Lib_GetDispStr(DP_EMPTY));
                TEXT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT3),Sys_Lib_GetDispStr(DP_EMPTY));
                TEXT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT4),Sys_Lib_GetDispStr(DP_EMPTY));

				sprintf(tmp, "1.%s", Sys_Lib_GetDispStr(DP_SWITCHTOVENT));
				TEXT_SetText(hTextItem[0], tmp);
				sprintf(tmp, "2.%s", Sys_Lib_GetDispStr(DP_OCCLUDEYPIECE));
				TEXT_SetText(hTextItem[1], tmp);
				sprintf(tmp, "3.%s", Sys_Lib_GetDispStr(DP_SETFLOWCONTROLMIN));
				TEXT_SetText(hTextItem[2], tmp);
				sprintf(tmp, "4.%s", Sys_Lib_GetDispStr(DP_O2FILLBELLOWS));
				TEXT_SetText(hTextItem[3], tmp);
#if (LOGO_FLAG == PENLON_LOGO)	
				sprintf(tmp, "5.%s", Sys_Lib_GetDispStr(DP_SETACGOON));
				TEXT_SetText(hTextItem[4], tmp);
				sprintf(tmp, "6.%s", Sys_Lib_GetDispStr(DP_PRESSSTARTBUTTON));
				TEXT_SetText(hTextItem[5], tmp);
#else
				sprintf(tmp, "5.%s", Sys_Lib_GetDispStr(DP_PRESSSTARTBUTTON));
				TEXT_SetText(hTextItem[4], tmp);
 				TEXT_SetText(hTextItem[5], Sys_Lib_GetDispStr(DP_EMPTY));
#endif
                TEXT_SetText(hTextItem[6], Sys_Lib_GetDispStr(DP_EMPTY));
                TEXT_SetText(hTextItem[7], Sys_Lib_GetDispStr(DP_EMPTY));
                TEXT_SetText(hTextItem[8], Sys_Lib_GetDispStr(DP_EMPTY));
                TEXT_SetText(hTextItem[9], Sys_Lib_GetDispStr(DP_EMPTY));
                TEXT_SetText(hTextItem[10],Sys_Lib_GetDispStr(DP_EMPTY));
                TEXT_SetText(hTextItem[11],Sys_Lib_GetDispStr(DP_EMPTY));
                TEXT_SetText(hTextItem[12],Sys_Lib_GetDispStr(DP_EMPTY));
                TEXT_SetText(hTextItem[13],Sys_Lib_GetDispStr(DP_EMPTY));

                //清除结果信息
                for(cnt = 0; cnt < TOTAL_TEXT_MAX_NUM; cnt++)
                {
                    TEXT_SetText(WM_GetDialogItem(pMsg->hWin,CheckItem_Result_Id[cnt]+1),Sys_Lib_GetDispStr(DP_EMPTY));
                    TEXT_SetText(WM_GetDialogItem(pMsg->hWin,CheckItem_Result_Id[cnt]+2),Sys_Lib_GetDispStr(DP_EMPTY));
                    TEXT_SetText(WM_GetDialogItem(pMsg->hWin,CheckItem_Result_Id[cnt]+3),Sys_Lib_GetDispStr(DP_EMPTY));
                }
            }
            else if(CONTEXT_APP.Checking_Control == Ctrl_AutoTest3)//机控检测2
            {
                TEXT_SetText(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT0),Sys_Lib_GetDispStr(DP_AUTOSYSTEMTEST));
                TEXT_SetText(hTextItem[0], Desktop_CheckItem_Text[POS_Barometer]);//djq add 20160809
                TEXT_SetText(hTextItem[1], Desktop_CheckItem_Text[POS_GasSypply]);
                TEXT_SetText(hTextItem[2], Desktop_CheckItem_Text[POS_Flow_S]);
                TEXT_SetText(hTextItem[3], Desktop_CheckItem_Text[POS_Pressure_S]);
//                TEXT_SetText(hTextItem[4], Desktop_CheckItem_Text[POS_FG_Flow_ZERO_S]);
                TEXT_SetText(hTextItem[4], Desktop_CheckItem_Text[POS_Oxygen]);
                TEXT_SetText(hTextItem[5], Desktop_CheckItem_Text[POS_Compliance]);
                TEXT_SetText(hTextItem[6], Desktop_CheckItem_Text[POS_Circuit_Leakage]);
                TEXT_SetText(hTextItem[7],Desktop_CheckItem_Text[POS_Battery]);
                TEXT_SetText(hTextItem[8], Sys_Lib_GetDispStr(DP_EMPTY));
                TEXT_SetText(hTextItem[9], Sys_Lib_GetDispStr(DP_EMPTY));
                TEXT_SetText(hTextItem[10], Sys_Lib_GetDispStr(DP_EMPTY));

                //清除结果信息
                for(cnt = 0; cnt < TOTAL_TEXT_MAX_NUM; cnt++)
                {
                    TEXT_SetText(WM_GetDialogItem(pMsg->hWin,CheckItem_Result_Id[cnt]+1),Sys_Lib_GetDispStr(DP_EMPTY));
                    TEXT_SetText(WM_GetDialogItem(pMsg->hWin,CheckItem_Result_Id[cnt]+2),Sys_Lib_GetDispStr(DP_EMPTY));
                    TEXT_SetText(WM_GetDialogItem(pMsg->hWin,CheckItem_Result_Id[cnt]+3),Sys_Lib_GetDispStr(DP_EMPTY));
                }
                Disp_ItemResult(POS_Barometer);
                Disp_ItemResult(POS_GasSypply);
                Disp_ItemResult(POS_Flow_S);
                Disp_ItemResult(POS_Pressure_S);
                //Disp_ItemResult(POS_FG_Flow_ZERO_S);
                Disp_ItemResult(POS_Oxygen);
                //Disp_ItemResult(POS_Compliance);
                //Disp_ItemResult(POS_Circuit_Leakage);
                //Disp_ItemResult(POS_Battery);


            }
            //Frame
			TEXT_SetBkColor(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT20),Back_Color);

            //设置当前页码
            switch(CONTEXT_APP.Checking_Control)
            {
            case Ctrl_Choose://Choose下面不会进入这个分支
                break;
                //Exit
            case Ctrl_ManualPrompt:
                ProcessWidget_SetIndexNumber(WM_GetDialogItem(pMsg->hWin,GUI_ID_PROCESSWIDGET),0);
                break;
            case Ctrl_ManualTest:
            case Ctrl_ManualFinish:
                ProcessWidget_SetIndexNumber(WM_GetDialogItem(pMsg->hWin,GUI_ID_PROCESSWIDGET),1);
                break;
            case Ctrl_AutoPrompt1:
                ProcessWidget_SetIndexNumber(WM_GetDialogItem(pMsg->hWin,GUI_ID_PROCESSWIDGET),2);
                break;
            case Ctrl_AutoTest1:
            case Ctrl_AutoFinish1:
                ProcessWidget_SetIndexNumber(WM_GetDialogItem(pMsg->hWin,GUI_ID_PROCESSWIDGET),3);
                break;
            case Ctrl_AutoPrompt2:
                ProcessWidget_SetIndexNumber(WM_GetDialogItem(pMsg->hWin,GUI_ID_PROCESSWIDGET),4);
                break;
            case Ctrl_AutoTest2:
            case Ctrl_AutoFinish2:
                ProcessWidget_SetIndexNumber(WM_GetDialogItem(pMsg->hWin,GUI_ID_PROCESSWIDGET),5);
                
                if(AA_all_data.AA_state.AA_ModuleState.currect_mode == AA_MODE_MEASURE)//ylk add 20181221
                {
                    SendOperationMessage(SET_AA_PUMPOFF,USART2);//关泵
                }
                
                break;
            case Ctrl_AutoPrompt3:
                ProcessWidget_SetIndexNumber(WM_GetDialogItem(pMsg->hWin,GUI_ID_PROCESSWIDGET),6);
                break;
            case Ctrl_AutoTest3:
            case Ctrl_AutoFinish3:
                ProcessWidget_SetIndexNumber(WM_GetDialogItem(pMsg->hWin,GUI_ID_PROCESSWIDGET),7);
                break;
            }                     
			break;
		case WM_PAINT://重绘时，对话框的用户区是目标窗口
			WINDOW_SetBkColor(pMsg->hWin,DialogBk_Color);
            //Frame 
			TEXT_SetBkColor(WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT20),Back_Color);
			break;
		default:
            WM_DefaultProc(pMsg);
            break;
	}
}
/***********************************************************
 *资源名  ：_Dialog_Desktop_Button
 *回调函数：_Callback_Dialog_Desktop_Button
 *标题    ：
 *显示内容：检测项目和检测内容
 *按钮    ：
************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _Dialog_Desktop_Button[]={
	{ WINDOW_CreateIndirect, " ",  0,               0,  RESULT_Y_END_POS+1,SELFTEST_WINDOW_W,60,WM_CF_SHOW,0},
    { BUTTON_CreateIndirect, "",   DESKTOP_BUTTON_CANCLE_ID,         20, 10, 250,35, 0},
    { BUTTON_CreateIndirect, "",   DESKTOP_BUTTON_DEVICE_CHECK,  275,10, 250,35, 0},
    { BUTTON_CreateIndirect, "",   DESKTOP_BUTTON_CIRCUIT_CHECK, 530,10, 250,35, 0},
    { BUTTON_CreateIndirect, "",   TWO_BUTTON_NO_ID,    20,10, 376,35, 0},
    { BUTTON_CreateIndirect, "",   TWO_BUTTON_YES_ID,  404,10, 376,35, 0},
};
/********************************************************
 *函数名：_Callback_Dialog_Desktop_Button
 *参数  ：系统消息
 *返回值：无
 *被调用位置：系统调用
 *功能  ：更新检测项目的显示
*********************************************************
*/
static void _Callback_Dialog_Desktop_Button(WM_MESSAGE *pMsg)//djq modify 2161019整个自检过程下面的按钮都使用这个串口
{
	WM_HWIN hWin;
    WM_HWIN DeskTop_hWin[3] = {0},Testing_hWin[2] = {0};
	U32 Id, IdCode;
#ifdef LANGUAGE_CHINESE_FLAG
    const GUI_FONT *ButtonFont = &GUI_Font_16;
#else
    const GUI_FONT *ButtonFont = &GUI_Font_24;
#endif

	switch(pMsg->MsgId)
	{
	    case WM_INIT_DIALOG:
			WINDOW_SetBkColor(pMsg->hWin,Back_Color);
            DeskTop_hWin[0] = WM_GetDialogItem(pMsg->hWin,DESKTOP_BUTTON_CANCLE_ID);
			BUTTON_SetFont(DeskTop_hWin[0],ButtonFont);
		    BUTTON_SetText(DeskTop_hWin[0],Sys_Lib_GetDispStr(DP_EXIT));
		    BUTTON_SetBkColor(DeskTop_hWin[0],0,Button_Quit_0_Bk_Color);
		    BUTTON_SetBkColor(DeskTop_hWin[0],3,Back_Color);
            DeskTop_hWin[1] = WM_GetDialogItem(pMsg->hWin,DESKTOP_BUTTON_DEVICE_CHECK);
			if(LanguageType == Disp_Lg_Russian || LanguageType == Disp_Lg_Bulgarian)
			{
				BUTTON_SetFont(DeskTop_hWin[1], &GUI_Font_20);
			}
			else
			{
				BUTTON_SetFont(DeskTop_hWin[1], ButtonFont);//GUI_Font_24
			}
			BUTTON_SetTextWrapMode(DeskTop_hWin[1], GUI_WRAPMODE_WORD);
		    BUTTON_SetText(DeskTop_hWin[1] ,Sys_Lib_GetDispStr(DP_MANUALLEAKTEST));
		    BUTTON_SetBkColor(DeskTop_hWin[1] ,0,Button_0_Bk_Color);
		    BUTTON_SetBkColor(DeskTop_hWin[1] ,3,Back_Color);
            DeskTop_hWin[2] = WM_GetDialogItem(pMsg->hWin,DESKTOP_BUTTON_CIRCUIT_CHECK);
			if(LanguageType == Disp_Lg_Russian || LanguageType == Disp_Lg_Bulgarian)
			{
				BUTTON_SetFont(DeskTop_hWin[2], &GUI_Font_20);
			}
			else
			{
				BUTTON_SetFont(DeskTop_hWin[2], ButtonFont);//GUI_Font_24
			}  
			BUTTON_SetTextWrapMode(DeskTop_hWin[2], GUI_WRAPMODE_WORD);
		    BUTTON_SetText(DeskTop_hWin[2],Sys_Lib_GetDispStr(DP_AUTOSYSTEMTEST));
			BUTTON_SetTextWrapMode(DeskTop_hWin[2], GUI_WRAPMODE_WORD);
		    BUTTON_SetBkColor(DeskTop_hWin[2],0,Button_0_Bk_Color);
		    BUTTON_SetBkColor(DeskTop_hWin[2],3,Back_Color);

            Testing_hWin[0] = WM_GetDialogItem(pMsg->hWin,TWO_BUTTON_NO_ID);
		    BUTTON_SetFont(Testing_hWin[0],ButtonFont);//GUI_Font_24
		    BUTTON_SetText(Testing_hWin[0],Sys_Lib_GetDispStr(DP_EXIT));
            BUTTON_SetBkColor(Testing_hWin[0],0,Button_Quit_0_Bk_Color);
            BUTTON_SetBkColor(Testing_hWin[0],3,Back_Color);

            Testing_hWin[1] = WM_GetDialogItem(pMsg->hWin,TWO_BUTTON_YES_ID);
            BUTTON_SetFont(Testing_hWin[1],ButtonFont);//GUI_Font_24
            BUTTON_SetText(Testing_hWin[1],Sys_Lib_GetDispStr(DP_START));
            BUTTON_SetBkColor(Testing_hWin[1],0,Button_0_Bk_Color);
            BUTTON_SetBkColor(Testing_hWin[1],3,Back_Color);
            //控制开始和退出按钮的状态。

            switch(CONTEXT_APP.Checking_Control)
            {
            case Ctrl_Choose://Quit Manual Auto
                WM_DisableWindow(Testing_hWin[0]);
                WM_DisableWindow(Testing_hWin[1]);

                WM_ShowWindow(DeskTop_hWin[0]);
                WM_ShowWindow(DeskTop_hWin[1]);
                WM_ShowWindow(DeskTop_hWin[2]);
                WM_HideWindow(Testing_hWin[0]);
                WM_HideWindow(Testing_hWin[1]);
                break;
            case Ctrl_ManualPrompt://Exit  Start
            case Ctrl_AutoPrompt1://Exit Start
            case Ctrl_AutoPrompt2://Exit Start
            case Ctrl_AutoPrompt3://Exit Start
                WM_EnableWindow(Testing_hWin[0]);
                WM_EnableWindow(Testing_hWin[1]);

                WM_HideWindow(DeskTop_hWin[0]);
                WM_HideWindow(DeskTop_hWin[1]);
                WM_HideWindow(DeskTop_hWin[2]);
                WM_ShowWindow(Testing_hWin[0]);
                WM_ShowWindow(Testing_hWin[1]);
                break;
            case Ctrl_ManualTest://Exit Start  禁止
            case Ctrl_AutoTest1://Exit Start 禁止
            case Ctrl_AutoTest2://Exit Start 禁止
            case Ctrl_AutoTest3://Exit Start 禁止
                WM_DisableWindow(Testing_hWin[0]);
                WM_DisableWindow(Testing_hWin[1]);

                WM_HideWindow(DeskTop_hWin[0]);
                WM_HideWindow(DeskTop_hWin[1]);
                WM_HideWindow(DeskTop_hWin[2]);
                WM_ShowWindow(Testing_hWin[0]);
                WM_ShowWindow(Testing_hWin[1]);
                break;
            case Ctrl_ManualFinish://Exit Continue
            case Ctrl_AutoFinish1://Exit Continue //退出的时候应该给一个是否放弃自检结果的提示
            case Ctrl_AutoFinish2://Exit Continue //退出的时候应该给一个是否放弃自检结果的提示
                WM_EnableWindow(Testing_hWin[0]);
                WM_EnableWindow(Testing_hWin[1]);

                WM_HideWindow(DeskTop_hWin[0]);
                WM_HideWindow(DeskTop_hWin[1]);
                WM_HideWindow(DeskTop_hWin[2]);
                WM_ShowWindow(Testing_hWin[0]);
                WM_ShowWindow(Testing_hWin[1]);

                BUTTON_SetText(Testing_hWin[1],Sys_Lib_GetDispStr(DP_CONTINUE));

                break;
            case Ctrl_AutoFinish3://Exit Finish //Exit禁止态
                WM_DisableWindow(Testing_hWin[0]);
//                WM_DisableWindow(Testing_hWin[1]);
//                WM_EnableWindow(Testing_hWin[0]);
                WM_EnableWindow(Testing_hWin[1]);

                WM_HideWindow(DeskTop_hWin[0]);
                WM_HideWindow(DeskTop_hWin[1]);
                WM_HideWindow(DeskTop_hWin[2]);
                WM_ShowWindow(Testing_hWin[0]);
                WM_ShowWindow(Testing_hWin[1]);

                BUTTON_SetText(Testing_hWin[1],Sys_Lib_GetDispStr(DP_FINISH));

                break;
            }
            break;
		case WM_PAINT://重绘时，对话框的用户区是目标窗口
			WINDOW_SetBkColor(pMsg->hWin,Back_Color);
			break;
		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
			IdCode = pMsg->Data.v;               /* Notification code */
			switch (IdCode)
            {
				case WM_NOTIFICATION_CLICKED:    //检测被按下的控件
// 				case WM_NOTIFICATION_RELEASED:
					if (Id == DESKTOP_BUTTON_CANCLE_ID) 
                    {//取消当前检测,BUG如果按下取消键，抬起时手指并没有在取消键上，则按键不能弹起
						BUTTON_SetState(pMsg->hWinSrc,BUTTON_STATE_FOCUS);
						WM_Paint(pMsg->hWinSrc);

                        PlanMenuType = STANDBY_MENU;
                        Task |=MEAU_UPDATE;
					}
					else if (Id == DESKTOP_BUTTON_DEVICE_CHECK) 
                    {//取消当前检测,BUG如果按下取消键，抬起时手指并没有在取消键上，则按键不能弹起
						BUTTON_SetState(pMsg->hWinSrc,BUTTON_STATE_FOCUS);
						WM_Paint(pMsg->hWinSrc);


                        AirSupplyTestResultFlag = 0;
                        Context_SetControlFlag(Ctrl_ManualPrompt);
                        CONTEXT_APP.Pause_Flag = 1;//弹出对话框后停止检测和超时的检测
                      //  CONTEXT_APP.Pause_Flag = 1;//弹出对话框后停止检测和超时的检测
					}
					else if (Id == DESKTOP_BUTTON_CIRCUIT_CHECK)
                    {//取消当前检测,BUG如果按下取消键，抬起时手指并没有在取消键上，则按键不能弹起
						BUTTON_SetState(pMsg->hWinSrc,BUTTON_STATE_FOCUS);
						WM_Paint(pMsg->hWinSrc);

                        AirSupplyTestResultFlag = 0;
                        Context_SetControlFlag(Ctrl_AutoPrompt1);
                        CONTEXT_APP.Pause_Flag = 1;//弹出对话框后停止检测和超时的检测
                    }
                    else if(Id == TWO_BUTTON_YES_ID)
                    {
                        BUTTON_SetState(pMsg->hWinSrc,BUTTON_STATE_FOCUS);
                        WM_Paint(pMsg->hWinSrc);
                        switch(CONTEXT_APP.Checking_Control)
                        {
                        case Ctrl_Choose://Choose下面不会进入这个分支
                            break;
                        case Ctrl_ManualPrompt://Start 进入 Ctrl_ManualTest
                            CONTEXT_APP.Pause_Flag = 0;
                            Context_SetControlFlag(Ctrl_ManualTest);
                            Context_NextLeft();
                            break;
                        case Ctrl_AutoPrompt1://Start 进入 Ctrl_AutoTest1
                            CONTEXT_APP.Pause_Flag = 0;
                            Context_SetControlFlag(Ctrl_AutoTest1);
                            Context_NextRight();
                            break;
                        case Ctrl_AutoPrompt2://Start 进入 Ctrl_AutoTest2
                            CONTEXT_APP.Pause_Flag = 0;
                            Context_SetControlFlag(Ctrl_AutoTest2);
                            Context_NextRight();
                            break;
                        case Ctrl_AutoPrompt3://Start 进入 Ctrl_AutoTest3
                            CONTEXT_APP.Pause_Flag = 0;
                            Context_SetControlFlag(Ctrl_AutoTest3);
                            Context_NextRight();
                            break;
                        case Ctrl_ManualTest://Start 不能点击
                        case Ctrl_AutoTest1://Start 不能点击
                        case Ctrl_AutoTest2://Start 不能点击
                        case Ctrl_AutoTest3://Start 不能点击
                            break;
                        case Ctrl_ManualFinish://Continue 进入Ctrl_AutoPrompt1
                            Context_SetControlFlag(Ctrl_AutoPrompt1);
                            break; 
                        case Ctrl_AutoFinish1://Continue 进入Ctrl_AutoPrompt2
                            Context_SetControlFlag(Ctrl_AutoPrompt2);
                           break;
                        case Ctrl_AutoFinish2://Finish 进入Ctrl_Choose
                            Context_SetControlFlag(Ctrl_AutoPrompt3);
                            break;
                        case Ctrl_AutoFinish3://Finish 进入Ctrl_Choose
							if(BeathSettingPara.BreathMode != ACGO_MODE)
							{
								Context_SetControlFlag(Ctrl_Choose);
							}
							else
							{
								SystemACGOPromptFlag = 1;
								Disp_Dialog_ACGO_Prompt();
							}                          
                            break;
                        }                     
                    }
                    else if(Id == TWO_BUTTON_NO_ID)
                    {
                        BUTTON_SetState(pMsg->hWinSrc,BUTTON_STATE_FOCUS);
                        WM_Paint(pMsg->hWinSrc);
                        switch(CONTEXT_APP.Checking_Control)
                        {
                        case Ctrl_Choose://Choose下面不会进入这个分支
                            break;
                        //Exit
                        case Ctrl_ManualPrompt:
                        case Ctrl_AutoPrompt1:
                        case Ctrl_AutoPrompt2:
                        case Ctrl_AutoPrompt3:
                        case Ctrl_ManualTest:
                        case Ctrl_AutoTest1:
                        case Ctrl_AutoTest2:
                        case Ctrl_AutoTest3:
                        case Ctrl_ManualFinish:
                        case Ctrl_AutoFinish2:
                        case Ctrl_AutoFinish3:
                            Context_SetControlFlag(Ctrl_Choose);
                            break;
						case Ctrl_AutoFinish1:
							//顺磁氧下关泵 //ylk modify 20181220
							if(Drager_EtCO2_Config_On == 2 || MasimoISA_EtCO2_Config_On == 2)
							{
								SendOperationMessage(SET_AA_PUMPOFF,USART2);
							}
							Context_SetControlFlag(Ctrl_Choose);
							break;
                        }                     

                    }
					break;
			}
			break;	
		default:
            WM_DefaultProc(pMsg);
            break;
	}
}
/***********************************************************
 *函数名  ：Disp_Desktop
 *按钮    ：取消检测
 *返回值  ：无
 *调用位置：
 *功能    ：绘制主界面
************************************************************
*/
void Disp_Desktop(void)
{
    U8 cnt = 0;

	DESKTOP_CALL_OLD = WM_SetCallback(WM_HBKWIN,&_Callback_Dialog_Desktop);//自检结束，需要还原
#if 1
    //设备检测结果显示窗口
    CheckItem_Left_HANDLE =  GUI_CreateDialogBox(_Dialog_CheckItem_Left,
											GUI_COUNTOF(_Dialog_CheckItem_Left),
											&_Callback_Dialog_CheckItem_Left,
											WM_HBKWIN,0,0);
    //管路检测结果显示窗口
    CheckItem_Right_HANDLE =  GUI_CreateDialogBox(_Dialog_CheckItem_Right,
											GUI_COUNTOF(_Dialog_CheckItem_Right),
											&_Callback_Dialog_CheckItem_Right,
											WM_HBKWIN,0,0);
    //按钮窗口
    Desktop_Button_HANDLE  =  GUI_CreateDialogBox(_Dialog_Desktop_Button,
											GUI_COUNTOF(_Dialog_Desktop_Button),
											&_Callback_Dialog_Desktop_Button,
											WM_HBKWIN,0,0);
	//设备检测和管路检测检测过程的显示
	CheckItem_Total_HANDLE = GUI_CreateDialogBox(_Dialog_CheckItem_Total,
											GUI_COUNTOF(_Dialog_CheckItem_Total),
											&_Callback_Dialog_CheckItem_Total,
											WM_HBKWIN,0,0);
#endif
    WM_HideWindow(CheckItem_Total_HANDLE);
	//将3个对话框的属性设置为底层
    WM_BringToBottom(Desktop_Button_HANDLE);
    WM_BringToBottom(CheckItem_Left_HANDLE);
    WM_BringToBottom(CheckItem_Right_HANDLE);
    WM_BringToBottom(CheckItem_Total_HANDLE);
}

/***********************************************************
 *函数名  ：Dialog_Common_Init
 *调用位置：需要修改对话框显示的地方
 *功能  ：设置将要显示的对话框的内容
************************************************************
*/
void Dialog_Common_Init(WM_HWIN hWin,const U8 *Disp,const U8 *Button1,const U8 *Button2,U8 flag)
{
    WM_HWIN hWin1;
    WM_ShowWindow(hWin);
	if(flag)//
	{
	    hWin1 = hWin;
	}
	else//
	{
		if(hWin == Dialog_OneButton_Prompt_HANDLE)
		{
            hWin1 = WM_GetClientWindow(hWin);
		}
		else
		{
	        hWin1 = hWin;
            CONTEXT_APP.Pause_Flag = 1;//双按钮窗口，弹出对话框后停止检测和超时的检测
		}
	    strcpy(Callback_Struct.Disp_String,Disp);
	}
	strcpy(Callback_Struct.Button_1,Button1);
	strcpy(Callback_Struct.Button_2,Button2);
	//WM_SendMessageNoPara(hWin1,WM_PAINT);
	WM_Paint(hWin1);
}
#define BUTTON_TEO_W         370
#define BUTTON_ONE_W         760
//资源名  ：_Dialog_OneButton_Prompt_R
static const GUI_WIDGET_CREATE_INFO _Dialog_OneButton_Prompt_R[]=
{
	{ FRAMEWIN_CreateIndirect, " ", ONE_BUTTON_FRAMEWIN_ID,X_MARGIN,SELFTEST_WINDOW_H-200, SELFTEST_WINDOW_W-2*X_MARGIN+1,200,WM_CF_SHOW,0},//FRAMEWIN_CreateIndirect
    { BUTTON_CreateIndirect,   " ", TWO_BUTTON_YES_ID,     X_MARGIN,120, BUTTON_ONE_W,34},
	{ TEXT_CreateIndirect,	   " ", ONE_TEXT_MSG_ID,       0,20,  SELFTEST_WINDOW_W-2*X_MARGIN+1,  60,0,TEXT_CF_HCENTER | TEXT_CF_VCENTER},
	{ TEXT_CreateIndirect,	   " ", ONE_TEXT_MSG_ID_1,     0,5,   SELFTEST_WINDOW_W-2*X_MARGIN+1,  80,0,TEXT_CF_HCENTER | TEXT_CF_VCENTER},
    
    {DrawEffectLine_CreateIndirect,   "",   GUI_ID_LINE0,     0,  0,   SELFTEST_WINDOW_W-2*X_MARGIN+1, 2,  0},
};
//资源名  ：_Dialog_TwoButton_Prompt_R
static const GUI_WIDGET_CREATE_INFO _Dialog_TwoButton_Prompt_R[]=
{
	{ FRAMEWIN_CreateIndirect, " ", TWO_BUTTON_FRAMEWIN_ID,X_MARGIN,SELFTEST_WINDOW_H-200,SELFTEST_WINDOW_W-2*X_MARGIN+1,200,WM_CF_SHOW,0},  //0,  677,600,123
	{ BUTTON_CreateIndirect,   " ", TWO_BUTTON_NO_ID,      X_MARGIN,120,BUTTON_TEO_W,34},
	{ BUTTON_CreateIndirect,   " ", TWO_BUTTON_YES_ID,     X_MARGIN+BUTTON_TEO_W+20,120,BUTTON_TEO_W,34},
	{ TEXT_CreateIndirect,	   " ", ONE_TEXT_MSG_ID,       0,20,  SELFTEST_WINDOW_W-2*X_MARGIN+1,  60,0,TEXT_CF_HCENTER | TEXT_CF_VCENTER},
};
/********************************************************
 *函数名：_Callback_Dialog_OneButton_Prompt
 *参数  ：系统消息
 *返回值：无
 *被调用位置：系统调用
 *功能  ：对_Dialog_OneButton_Prompt_R对话框进行操作的时候，系统会调用
      该回调函数，如果点击了按钮，会更改环境变量Callback_Return的值。
	  通过判断Callback_Return的值可以知道用户的选择。
*********************************************************
*/
static void _Callback_Dialog_OneButton_Prompt(WM_MESSAGE *pMsg)
{
    WM_HWIN FRAMEWIN_HANDLE,BUTTON_YES,BUTTON_NO,TEXT_MSG_HANDLE,TEXT_MSG_HANDLE_1,hLine;
    U32 Id, IdCode;
    switch(pMsg->MsgId)
    {
        case WM_INIT_DIALOG:
            BUTTON_YES = WM_GetDialogItem(pMsg->hWin,TWO_BUTTON_YES_ID);
            //TEXT_MSG_HANDLE = WM_GetDialogItem(pMsg->hWin,ONE_TEXT_MSG_ID);
            TEXT_MSG_HANDLE_1 = WM_GetDialogItem(pMsg->hWin,ONE_TEXT_MSG_ID_1);

            //TEXT_SetFont(TEXT_MSG_HANDLE,&GUI_Font_24);
            TEXT_SetFont(TEXT_MSG_HANDLE_1,&GUI_Font_16);
            BUTTON_SetFont(BUTTON_YES,&GUI_Font_16);
            break;
        case WM_PAINT://重绘时，对话框的用户区是目标窗口
            FRAMEWIN_HANDLE =Dialog_OneButton_Prompt_HANDLE;//标题的句柄
            BUTTON_YES = WM_GetDialogItem(pMsg->hWin,TWO_BUTTON_YES_ID);
            //TEXT_MSG_HANDLE = WM_GetDialogItem(pMsg->hWin,ONE_TEXT_MSG_ID);
            TEXT_MSG_HANDLE_1 = WM_GetDialogItem(pMsg->hWin,ONE_TEXT_MSG_ID_1);

            //FRAMEWIN_SetClientColor(FRAMEWIN_HANDLE,GUI_GREEN);
            //FRAMEWIN_SetBarColor(FRAMEWIN_HANDLE,0,GUI_RED);

            //TEXT_SetTextAlign(TEXT_MSG_HANDLE,TEXT_CF_HCENTER | TEXT_CF_VCENTER);
            TEXT_SetTextAlign(TEXT_MSG_HANDLE_1,TEXT_CF_HCENTER | TEXT_CF_VCENTER);
            hLine = WM_GetDialogItem(pMsg->hWin,GUI_ID_LINE0);
            DrawEffectLine_SetColor(hLine,Desktop_Frame_Color);//
            //标题
            FRAMEWIN_SetFont(FRAMEWIN_HANDLE,&GUI_Font_24);
            FRAMEWIN_SetTextAlign(FRAMEWIN_HANDLE,GUI_TA_HCENTER|GUI_TA_VCENTER);
            FRAMEWIN_SetText(FRAMEWIN_HANDLE,Callback_Struct.Title_String);
            //TEXT_SetText(TEXT_MSG_HANDLE,Callback_Struct.Title_String);
            //提示内容
            TEXT_SetText(TEXT_MSG_HANDLE_1,Callback_Struct.Disp_String);
            //按钮内容			
            BUTTON_SetText(BUTTON_YES,Callback_Struct.Button_1);
            BUTTON_SetBkColor(BUTTON_YES,0,Button_0_Bk_Color);
            break;
        case WM_NOTIFY_PARENT:
            Id    = WM_GetId(pMsg->hWinSrc);    
            IdCode = pMsg->Data.v;               
            switch (IdCode) 
            {
            case WM_NOTIFICATION_RELEASED:
                if (Id == TWO_BUTTON_YES_ID)//操作人员确认连接，但是没有检测到，应该报错
                {					
                    if(SystemACGOPromptFlag != 1)//弹出电源提示信息点击skip按钮时进
					{
						Battery_TestSkip();
					}
					else//弹出ACGO提示信息点击Confirm按钮时进
                    {
                        SystemACGOPromptFlag = 0;
                        Context_SetControlFlag(Ctrl_Choose);
                    }
                }
                if(WM_IsWindow(Dialog_OneButton_Prompt_HANDLE))
                {
                    WM_HideWindow(Dialog_OneButton_Prompt_HANDLE);
                }	
                CONTEXT_APP.Pause_Flag = 0;//点击按键后，清除暂停标志，开始继续检测
				BUTTON_SetState(pMsg->hWinSrc,1);
                break;
            }
    }
}

/********************************************************
 *函数名：_Callback_Dialog_TwoButton_Prompt
 *参数  ：系统消息
 *返回值：无
 *被调用位置：系统调用
 *功能  ：对_Dialog_Two_Button_R对话框进行操作的时候，系统会调用
      该回调函数，如果点击了按钮，会更改环境变量Callback_Return的值。
	  通过判断Callback_Return的值可以知道用户的选择。
*********************************************************
*/
static void _Callback_Dialog_TwoButton_Prompt(WM_MESSAGE *pMsg)
{
	WM_HWIN BUTTON_YES,BUTTON_NO,TEXT_MSG_HANDLE;
	U32 Id, IdCode;
	switch(pMsg->MsgId){
		case WM_INIT_DIALOG:
			//WINDOW_SetBkColor(pMsg->hWin,Back_Color);
			BUTTON_SetFont(WM_GetDialogItem(pMsg->hWin,TWO_BUTTON_NO_ID),&GUI_Font_16);
			BUTTON_SetFont(WM_GetDialogItem(pMsg->hWin,TWO_BUTTON_YES_ID),&GUI_Font_16);
			TEXT_SetFont(WM_GetDialogItem(pMsg->hWin,ONE_TEXT_MSG_ID),&GUI_Font_16);
			TEXT_SetTextAlign(WM_GetDialogItem(pMsg->hWin,ONE_TEXT_MSG_ID),TEXT_CF_HCENTER | TEXT_CF_VCENTER);
			break;
		case WM_PAINT:
			BUTTON_SetText(WM_GetDialogItem(pMsg->hWin,TWO_BUTTON_NO_ID),Callback_Struct.Button_1);
			BUTTON_SetText(WM_GetDialogItem(pMsg->hWin,TWO_BUTTON_YES_ID),Callback_Struct.Button_2);
			TEXT_SetText(WM_GetDialogItem(pMsg->hWin,ONE_TEXT_MSG_ID),Callback_Struct.Disp_String);
			break;
		case WM_NOTIFY_PARENT:
			Id    = WM_GetId(pMsg->hWinSrc);    /* Id of widget */
			IdCode = pMsg->Data.v;               /* Notification code */
			switch (IdCode)
            {
				//case WM_NOTIFICATION_CLICKED:
				case WM_NOTIFICATION_RELEASED:
					if (Id == TWO_BUTTON_NO_ID)//重新检测当前项目
                    {
                        if(Context_GetPosition() == POS_GasSypply)//重新检测气源压力，应该从O2开始
                        {
                            Disp_ClearGasPrompt();
                        }
						CONTEXT_APP.Callback_Return = TWO_BUTTON_YES_ID;
						BUTTON_SetState(pMsg->hWinSrc,BUTTON_STATE_FOCUS);
						Context_SetWindow_Hide();

						Context_RetestItem();//重新检测
					}
					else if (Id == TWO_BUTTON_YES_ID)//不进行重新检测，保存错误数据，转到下一项
                    {
						CONTEXT_APP.Callback_Return = TWO_BUTTON_NO_ID;
						BUTTON_SetState(pMsg->hWinSrc,BUTTON_STATE_FOCUS);
                        Context_SetWindow_Hide();

                        if(Context_IsLeftCheckFlag())
                        {
                            Context_NextLeft();
                        }
                        else if(Context_IsRightCheckFlag())
                        {
                            if((Context_GetPosition() == POS_GasSypply) && (AirSupplyTestResultFlag))//气源检测成功
							{
                                Context_NextRight();//检测下一项
                                AirSupplyTestResultFlag = 0;
							}
							else if((Context_GetPosition() == POS_GasSypply) && (!AirSupplyTestResultFlag))//气源检测失败  但可以往下进行
							{
                                CONTEXT_APP.Global_CancelFlag = 1;
                                Context_NextRight();//检测下一项
							}
							else
							{
                                Context_NextRight();
								WM_HideWindow(pMsg->hWin);
							}
                        }
                    }
                    CONTEXT_APP.Pause_Flag = 0;//点击按键后，清除暂停标志，开始继续检测
					BUTTON_SetState(pMsg->hWinSrc,1);
					break;
			}
			break;
		default:
			WM_DefaultProc(pMsg);
			break;
	}
}
/***********************************************************
 *资源名  ：_Dialog_Alarm_R
 *回调函数：
 *标题    ：
 *显示内容：
 *显示图标：电池和电源
 *按钮    ：电池图标按钮、电源图标按钮
************************************************************
*/
/***********************************************************
 *函数名  ：Create_AllGeneralDialog
 *调用位置：程序初始化位置
 *功能  ：创建程序里面(除了alarm1对话框)所有的对话框，并且隐藏
************************************************************
*/
void Create_AllGeneralDialog(void)
{
	Dialog_OneButton_Prompt_HANDLE = GUI_CreateDialogBox(_Dialog_OneButton_Prompt_R,
											GUI_COUNTOF(_Dialog_OneButton_Prompt_R),
											&_Callback_Dialog_OneButton_Prompt,
											WM_HBKWIN,0,0);
	Dialog_TwoButton_Prompt_HANDLE = GUI_CreateDialogBox(_Dialog_TwoButton_Prompt_R,
											GUI_COUNTOF(_Dialog_TwoButton_Prompt_R),
											&_Callback_Dialog_TwoButton_Prompt,
											WM_HBKWIN,0,0);
	WM_HideWindow(Dialog_OneButton_Prompt_HANDLE);
	WM_HideWindow(Dialog_TwoButton_Prompt_HANDLE);
    WM_BringToTop(Dialog_OneButton_Prompt_HANDLE);
    WM_BringToTop(Dialog_TwoButton_Prompt_HANDLE);
}
