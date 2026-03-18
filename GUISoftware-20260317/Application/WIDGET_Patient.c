#include "WIDGET_Patient.h"
// #include "Config.h"
// #include "string.h"
// #include "func_extern.h"
#include "DispStringLib.h"
#include "GUI_Font_cfg.h"
#include "DispStrs_cfg.h"
#include "KeyBoardm.h"
#include "ButtonRotation.h"
#include "static_menu_creat.h"
#include "stdio.h"

extern unsigned char VT_Min_Flag ;     //0: 50    1:20
extern unsigned char HeightUnit;//0:cm  1:inch
extern unsigned char WeightUnit;//0:kg  1:Pound
extern unsigned int FlowHighLimit;
extern unsigned int FlowLowLimit;
extern BREATH_SET_PARA BeathSettingPara;
extern ALARM_SET_PARA AlarmSettingPara;
extern MENU_HWIN Menu_hWin;
extern int PlanMenuType;
extern unsigned char StandbyState;
extern int Task;
extern BREATH_SET_PARA Breath_Setting_Tmp;

static Patient_Info_t Patient_Info;//病人信息
static Patient_Info_t Tmp_PatientInfo = {0};//
unsigned char StartPatientChooseFlag = 1;//开机后是否确定了新病人类型标志

static void _cbDialogNewPatientInfo(WM_MESSAGE * pMsg);
static void _cbDialogOldPatientInfo(WM_MESSAGE * pMsg);
static void Patient_InitOldInfo(Patient_Info_t *Patient,int InitFlag);
static int __GetIBWInfo(Patient_Info_t PatientInfo,int Stature);
static void __GetPatientInfo(WM_HWIN hDlg,Patient_Info_t *PatientInfo);

//病人信息菜单
static const GUI_WIDGET_CREATE_INFO _aDialogCreatePatient[] =
{
	{ WINDOW_CreateIndirect,    "", 0,                 0, 0, 634,288, FRAMEWIN_CF_ACTIVE },
	{ TEXT_CreateIndirect,    "",     GUI_ID_TEXT0,       20, 50,   80,  35, TEXT_CF_LEFT|TEXT_CF_VCENTER},//Patient
	{ TEXT_CreateIndirect,    "",     GUI_ID_TEXT1,       20, 96,  80,  35, TEXT_CF_LEFT|TEXT_CF_VCENTER},//Gender
	{ TEXT_CreateIndirect,    "",     GUI_ID_TEXT2,       20, 158,  80,  35, TEXT_CF_LEFT|TEXT_CF_VCENTER},//Height
	{ TEXT_CreateIndirect,    "",     GUI_ID_TEXT3,       300, 158,  60,  35, TEXT_CF_LEFT|TEXT_CF_VCENTER},//Age


	{ TEXT_CreateIndirect,    "",     GUI_ID_TEXT4,       145, 143+18,  60,  35, TEXT_CF_HCENTER|TEXT_CF_VCENTER},//Ibw value
	{ TEXT_CreateIndirect,    "",     GUI_ID_TEXT5,       210, 143+22,  40,  35, TEXT_CF_LEFT},//Ibw union

	{ TEXT_CreateIndirect,    "",     GUI_ID_TEXT7,       20, 10,   20,  28, TEXT_CF_LEFT|TEXT_CF_VCENTER},//Name

	{EDIT_CreateIndirect,			"", GUI_ID_EDIT1,	 45,6,240,35,0},

	{ TEXT_CreateIndirect,    "",     GUI_ID_TEXT6,       320, 10,   40,  28, TEXT_CF_LEFT|TEXT_CF_VCENTER},//ID Number

	{EDIT_CreateIndirect,			"", GUI_ID_EDIT0,	370,6,240,35,0},

	{ BUTTON_CreateIndirect,    "",   GUI_ID_BUTTON0,     80, 53,  170,  35, 0},//Adult
	{ BUTTON_CreateIndirect,    "",   GUI_ID_BUTTON1,     265, 53,  170,  35, 0},//pead
	{ BUTTON_CreateIndirect,    "",   GUI_ID_BUTTON2,     450, 53,  170,  35, 0},//infant

	{ BUTTON_CreateIndirect,    "",   GUI_ID_BUTTON3,     80, 98,  170,  35, 0},//male
	{ BUTTON_CreateIndirect,    "",   GUI_ID_BUTTON4,     265, 98,  170,  35, 0},//female

	{ ButtonRotation_CreateIndirect,"",GUI_ID_BUTTON8,    80, 143,   70,   70, 0},//ibw
	{ ButtonRotation_CreateIndirect,"",GUI_ID_BUTTON9,    345, 143,   70,   70, 0},//Age

// 	{DrawEffectLine_CreateIndirect,   "",   GUI_ID_LINE0,        25,        155 ,             530, 2,  0},
// 	{DrawEffectLine_CreateIndirect,   "",   GUI_ID_LINE1,        25,        255 ,             530, 2,  0},
// 	{DrawEffectLine_CreateIndirect,   "",   GUI_ID_LINE2,        25,        400 ,             80, 2,  0},
// 	{DrawEffectLine_CreateIndirect,   "",   GUI_ID_LINE3,        330,        400 ,             80, 2,  0},
// 	{DrawEffectLine_CreateIndirect,   "",   GUI_ID_LINE4,        25,        55 ,             80, 2,  0},
// 	{DrawEffectLine_CreateIndirect,   "",   GUI_ID_LINE5,        25,        110 ,             80, 2,  0},


	{ BUTTON_CreateIndirect,    "",   GUI_ID_BUTTON7,     15,   243,  604,  30, 0},//start
};



//创建该对话框的时候，先删除standby，等删除该对话框的时候，在创建standby
WM_HWIN Patient_InfoWinCreate(void)
{
	WM_HWIN hWin,hWin1,hWin2,hWin3,hWin4;

// 	hWin1 = FRAMEWIN_Create("",0,WM_CF_SHOW,11 ,104-28, 578+52, 327);
// 	hWin3 = MULTIPAGE_CreateEx(0, 0, 578, 570, hWin1, WM_CF_SHOW, 0, GUI_ID_MULTIPAGE0);//0, 0, 578,507

	hWin1 = FRAMEWIN_Create("",0,WM_CF_SHOW,3, 146-80, 642, 250+79);//ylk modify 调整对话框大小
	hWin3 = MULTIPAGE_CreateEx(8-6, 10, 625+13, 288+30, hWin1, WM_CF_SHOW, 0, GUI_ID_MULTIPAGE0);//
	hWin = FRAMEWIN_AddCloseButton(hWin1,0,5);


// 	hWin = FRAMEWIN_AddCloseButton(hWin1,1,3);
// 	BUTTON_SetBkColor(hWin,0,0x8E7866);
// 	BUTTON_SetBkColor(hWin,1,0x8E7866);
	hWin2 = GUI_CreateDialogBox(_aDialogCreatePatient,           //new
		GUI_COUNTOF(_aDialogCreatePatient),
		&_cbDialogNewPatientInfo, 0, 0, 0);
	hWin4 = GUI_CreateDialogBox(_aDialogCreatePatient,           //per
		GUI_COUNTOF(_aDialogCreatePatient),
		&_cbDialogOldPatientInfo, 0, 0, 0);
	MULTIPAGE_AddPage(hWin3, hWin2, Sys_Lib_GetDispStr(DP_NEWPATIENT));
	MULTIPAGE_AddPage(hWin3, hWin4, Sys_Lib_GetDispStr(DP_PREVIOUSPATIENT));

// 	MULTIPAGE_SetBkColor(hWin3, GUI_RED, 0);
// 	MULTIPAGE_SetBkColor(hWin3, GUI_RED, 1);

	KeyBoard_Create();
	WM_HideWindow(KeyBoard_GetHandle());
	KeyBoard_SetHook(hWin1);
	KetBoard_SetPos(7, 150+5); //wff modify 

	MULTIPAGE_SelectPage(hWin3,0);
	return(hWin1);
}

static void _cbDialogNewPatientInfo(WM_MESSAGE * pMsg)
{
	WM_HWIN hDlg,hButton0,hButton1,hButton2,hButton3,hButton4,hButton5,hButton6,hButton7;
	WM_HWIN hButtonRation0,hButtonRation1,hText0,hText1,hText2,hText3,hText4,hText5,hText6,hText7;
	WM_HWIN hLine0,hLine1,hLine2,hLine3,hLine4,hLine5;
	WM_HWIN hEdit0,hEdit1;
	int Id,NCode;
	int Data = 0, tmp_IBW = 0, tmp_Height = 0, tmp_Age;
	WM_MESSAGE Msg;
	char IBW_Value[20];
	int tmp = 0;
	hDlg = pMsg->hWin;
	switch (pMsg->MsgId)
	{
	case WM_INIT_DIALOG:
		WINDOW_SetBkColor(hDlg,0xCDAA94);    //GUI_ADD
		//初始化临时变量的信息，每次重新启动的时候才初始化,默认为成年男性
		Tmp_PatientInfo.Patient_Type = PATIENTTYPE_ADULT;
		Tmp_PatientInfo.Patient_Gender = GENDER_MALE;
		Tmp_PatientInfo.Patient_VentType = VENTTYPE_INV;
		Tmp_PatientInfo.Patient_IBW = IBW_ADULT_MALE;
		Tmp_PatientInfo.Patient_Stature = STATURE_ADULT_MALE;
		Tmp_PatientInfo.Patient_Age = AGE_ADULT;

		hEdit0 = WM_GetDialogItem(hDlg, GUI_ID_EDIT0); //wff add 
		EDIT_SetBkColor(hEdit0,1,0xFFEEE6);
		EDIT_SetTextAlign(hEdit0,GUI_TA_VCENTER);
		EDIT_SetFont(hEdit0,&GUI_Font_20);
		EDIT_SetMaxLen(hEdit0, EDIT_PID_SIZE);

		hEdit1 = WM_GetDialogItem(hDlg, GUI_ID_EDIT1); //wff add 
		EDIT_SetBkColor(hEdit1,1,0xFFEEE6);
		EDIT_SetTextAlign(hEdit1,GUI_TA_VCENTER);
		EDIT_SetFont(hEdit1,&GUI_Font_20);
		EDIT_SetMaxLen(hEdit1, EDIT_PN_SIZE);

		//Patient
		hText0 = WM_GetDialogItem(hDlg, GUI_ID_TEXT0);
		TEXT_SetText(hText0, Sys_Lib_GetDispStr(DP_PATIENT));
		hButton0 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON0);
		BUTTON_SetText(hButton0, Sys_Lib_GetDispStr(DP_NEWADULT));
 		BUTTON_SetMinorTextType(hButton0,&GUI_FontPatientIcon32X32,"A", 5,1);
		BUTTON_SetBkColor(hButton0,0,0xD05356);
		hButton1 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON1);
		BUTTON_SetText(hButton1, Sys_Lib_GetDispStr(DP_NEWPAEDIATRIC));
		BUTTON_SetMinorTextType(hButton1,&GUI_FontPatientIcon32X32,"P", 5,1);
		BUTTON_SetBkColor(hButton1,0,0xD05356);
		hButton2 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON2);
		BUTTON_SetText(hButton2, Sys_Lib_GetDispStr(DP_NEWINFANT));
		BUTTON_SetMinorTextType(hButton2,&GUI_FontPatientIcon32X32,"I", 5,1);
		BUTTON_SetBkColor(hButton2,0,0xD05356);

		BUTTON_SetTextWrapMode(hButton0, GUI_WRAPMODE_WORD);
		BUTTON_SetTextWrapMode(hButton1, GUI_WRAPMODE_WORD);
		BUTTON_SetTextWrapMode(hButton2, GUI_WRAPMODE_WORD);
		if(!VT_Min_Flag)
		{
			WM_DisableWindow(hButton2);
		}
		else
		{
			WM_EnableWindow(hButton2);
		}

		BUTTON_SetState(hButton0,BUTTON_STATE_PRESSED);//默认选项
		BUTTON_SetState(hButton1,BUTTON_STATE_FOCUS);
		BUTTON_SetState(hButton2,BUTTON_STATE_FOCUS);

		//Gender
		hText1 = WM_GetDialogItem(hDlg, GUI_ID_TEXT1);
		TEXT_SetText(hText1, Sys_Lib_GetDispStr(DP_GENDER));
		hButton3 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON3);
		BUTTON_SetText(hButton3, Sys_Lib_GetDispStr(DP_MALE));
		BUTTON_SetMinorTextType(hButton3,&GUI_FontPatientIcon32X32,"M", 5,1);
		BUTTON_SetBkColor(hButton3,0,0xD05356);
		hButton4 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON4);
		BUTTON_SetText(hButton4, Sys_Lib_GetDispStr(DP_FEMALE));
		BUTTON_SetMinorTextType(hButton4,&GUI_FontPatientIcon32X32,"F", 5,1);
		BUTTON_SetBkColor(hButton4,0,0xD05356);

		BUTTON_SetTextWrapMode(hButton3, GUI_WRAPMODE_WORD);
		BUTTON_SetTextWrapMode(hButton4, GUI_WRAPMODE_WORD);

		BUTTON_SetState(hButton3,BUTTON_STATE_PRESSED);//默认选项
		BUTTON_SetState(hButton4,BUTTON_STATE_FOCUS);
		//IBW
		hButtonRation0 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON8);
		ButtonRotation_SetTriangleSign(hButtonRation0, 0);
		//ButtonRotation_SetDataInvildSign(hButtonRation0,4);
		ButtonRotation_SetDataInfo(hButtonRation0,110,250,STATURE_ADULT_MALE,1,0);
		ButtonRotation_SetData(hButtonRation0,STATURE_ADULT_MALE);
		if(1/*!HeightUnit*/)//暂时只有cm
		{
			ButtonRotation_SetText(hButtonRation0, Sys_Lib_GetDispStr(DP_EMPTY), Sys_Lib_GetDispStr(DP_CM));
		}
		else
		{
			ButtonRotation_SetText(hButtonRation0, Sys_Lib_GetDispStr(DP_EMPTY), Sys_Lib_GetDispStr(DP_IN));
		}

		//Age
		hButtonRation1 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON9);
		ButtonRotation_SetDataInfo(hButtonRation1,12,130,AGE_ADULT,1,0);
		ButtonRotation_SetData(hButtonRation1,AGE_ADULT);
		ButtonRotation_SetText(hButtonRation1, Sys_Lib_GetDispStr(DP_EMPTY), Sys_Lib_GetDispStr(DP_YEARS));

		hText4 = WM_GetDialogItem(hDlg, GUI_ID_TEXT4);
		TEXT_SetTextAlign(hText4,TEXT_CF_HCENTER|TEXT_CF_VCENTER);
		TEXT_SetFont(hText4,&GUI_Font32B_ASCII);
		sprintf(IBW_Value,"%3.0f",(float)UnitConversion(DISP_WEIGHT_VALUE,WeightUnit,IBW_ADULT_MALE));
		TEXT_SetText(hText4,IBW_Value);//GUI_FLAG IBW身高体重公式

		hText5 = WM_GetDialogItem(hDlg, GUI_ID_TEXT5);
		TEXT_SetFont(hText5,&GUI_Font_16);
		if(1/*!WeightUnit*/)//暂时只有kg
		{
			sprintf(IBW_Value,"%s\n %s",Sys_Lib_GetDispStr(DP_IBW),Sys_Lib_GetDispStr(DP_KG));
			TEXT_SetText(hText5,IBW_Value);  //初始化为.。。    GUI_FLAG 
		}
		else
		{
// 			sprintf(IBW_Value,"%s\n %s",Sys_Lib_GetDispStr(DP_IBW),Sys_Lib_GetDispStr(DP_IB));
// 			TEXT_SetText(hText5,IBW_Value);  //初始化为.。。    GUI_FLAG 
		}

		//Height
		hText2 = WM_GetDialogItem(hDlg, GUI_ID_TEXT2);
		TEXT_SetText(hText2,Sys_Lib_GetDispStr(DP_HEIGHT));
		//Age
		hText3 = WM_GetDialogItem(hDlg, GUI_ID_TEXT3);
		TEXT_SetText(hText3,Sys_Lib_GetDispStr(DP_AGE));

		//Name
		hText6 = WM_GetDialogItem(hDlg, GUI_ID_TEXT6); //wff add 病人名字
		TEXT_SetText(hText6, Sys_Lib_GetDispStr(DP_NAME));
		//Case Number
		hText7 = WM_GetDialogItem(hDlg, GUI_ID_TEXT7); //wff add 病历号
		TEXT_SetText(hText7, "Id");

		//Start
		hButton7 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON7);
		BUTTON_SetFont(hButton7, &GUI_Font_24);
		BUTTON_SetText(hButton7,Sys_Lib_GetDispStr(DP_CONFIRM));

		break;
	case WM_DELETE:
		KeyBoard_deleteWindow();
		break;
	case WM_NOTIFY_PARENT:
		NCode = pMsg->Data.v;
		Id = WM_GetId(pMsg->hWinSrc);;
		switch(NCode)
		{
		case WM_NOTIFICATION_LOST_FOCUS:
			//if(WM_IsVisible(KeyBoard_GetHandle()))
			//{
			//	WM_HideWindow(KeyBoard_GetHandle());
			//}
			break;
		case WM_NOTIFICATION_CLICKED:
			switch(Id)
			{
			case GUI_ID_EDIT0:  //wff add
			case GUI_ID_EDIT1: //wff add
 				if(!WM_IsVisible(KeyBoard_GetHandle()))
 				{
 					WM_ShowWindow(KeyBoard_GetHandle());
 				}
				break;
				//Patient type
			case GUI_ID_BUTTON0:
			case GUI_ID_BUTTON1:
			case GUI_ID_BUTTON2:
				hButtonRation0 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON8);
				ButtonRotation_SetTriangleSign(hButtonRation0,0);
				hButtonRation1 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON9);
				ButtonRotation_SetTriangleSign(hButtonRation1,0);

				BUTTON_SetState(WM_GetDialogItem(hDlg, GUI_ID_BUTTON0),BUTTON_STATE_FOCUS);
				BUTTON_SetState(WM_GetDialogItem(hDlg, GUI_ID_BUTTON1),BUTTON_STATE_FOCUS);
				BUTTON_SetState(WM_GetDialogItem(hDlg, GUI_ID_BUTTON2),BUTTON_STATE_FOCUS);

				BUTTON_SetState(pMsg->hWinSrc,BUTTON_STATE_PRESSED);
				if(Id == GUI_ID_BUTTON0)//成人
				{
					Tmp_PatientInfo.Patient_Type = PATIENTTYPE_ADULT;//设置病人类型
				}
				else if(Id == GUI_ID_BUTTON1)//儿童
				{
					Tmp_PatientInfo.Patient_Type = PATIENTTYPE_PAEDIATRIC;//设置病人类型
				}
				else if(Id == GUI_ID_BUTTON2)//婴儿
				{
					Tmp_PatientInfo.Patient_Type = PATIENTTYPE_INFANT;//设置病人类型
				}
				__GetPatientInfo(hDlg,&Tmp_PatientInfo);
				Data = ButtonRotation_GetData(WM_GetDialogItem(hDlg, GUI_ID_BUTTON8));
				Tmp_PatientInfo.Patient_Stature	= UnitConversion_Reverse(DISP_HEIGHT_VALUE,HeightUnit,Data);
// #ifdef LANGUAGE_CHINESE_FLAG
// 				Tmp_PatientInfo.Patient_IBW = __GetIBWInfo(Tmp_PatientInfo,Data);
// #else
// 				if(Tmp_PatientInfo.Patient_Type == PATIENTTYPE_INFANT)	//20181031
// 				{
// 					Tmp_PatientInfo.Patient_IBW = __GetIBWInfo(Tmp_PatientInfo,Data)/10.0;
// 				}
// 				else
// 				{
					Tmp_PatientInfo.Patient_IBW = __GetIBWInfo(Tmp_PatientInfo,Data);
// 				}
// #endif
				Data = ButtonRotation_GetData(WM_GetDialogItem(hDlg, GUI_ID_BUTTON9));
				Tmp_PatientInfo.Patient_Age	= Data;	
				break;
				//Gender
			case GUI_ID_BUTTON3:
			case GUI_ID_BUTTON4:
				hButtonRation0 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON8);
				ButtonRotation_SetTriangleSign(hButtonRation0,0);
				hButtonRation1 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON9);
				ButtonRotation_SetTriangleSign(hButtonRation1,0);

				BUTTON_SetState(WM_GetDialogItem(hDlg, GUI_ID_BUTTON3),BUTTON_STATE_FOCUS);
				BUTTON_SetState(WM_GetDialogItem(hDlg, GUI_ID_BUTTON4),BUTTON_STATE_FOCUS);

				BUTTON_SetState(pMsg->hWinSrc,BUTTON_STATE_PRESSED);
				if(Id == GUI_ID_BUTTON3)//男性
				{
					Tmp_PatientInfo.Patient_Gender = GENDER_MALE;//设置病人性别
				}
				else if(Id == GUI_ID_BUTTON4)//女性
				{
					Tmp_PatientInfo.Patient_Gender = GENDER_FEMALE;//设置病人性别
				}
				__GetPatientInfo(hDlg,&Tmp_PatientInfo);

				//hText4 = WM_GetDialogItem(hDlg, GUI_ID_TEXT4);
				Data = ButtonRotation_GetData(WM_GetDialogItem(hDlg, GUI_ID_BUTTON8));
				Tmp_PatientInfo.Patient_Stature	= UnitConversion_Reverse(DISP_HEIGHT_VALUE,HeightUnit,Data);

// #ifdef LANGUAGE_CHINESE_FLAG
// 				Tmp_PatientInfo.Patient_IBW = __GetIBWInfo(Tmp_PatientInfo,Data);
// #else
// 				if(Tmp_PatientInfo.Patient_Type == PATIENTTYPE_INFANT)	//20181031
// 				{
// 					Tmp_PatientInfo.Patient_IBW = __GetIBWInfo(Tmp_PatientInfo,Data)/10.0;
// 				}
// 				else
// 				{
					Tmp_PatientInfo.Patient_IBW = __GetIBWInfo(Tmp_PatientInfo,Data);
// 				}
//#endif
				Data = ButtonRotation_GetData(WM_GetDialogItem(hDlg, GUI_ID_BUTTON9));
				Tmp_PatientInfo.Patient_Age	= Data;	
				break;
				//start
			case GUI_ID_BUTTON7:
				//获取病人信息
				StartPatientChooseFlag = 0;

				hEdit0 = WM_GetDialogItem(hDlg, GUI_ID_EDIT0);
				EDIT_GetText(hEdit0, Tmp_PatientInfo.Patient_Name, EDIT_GetNumChars(hEdit0));//wff add
				hEdit1 = WM_GetDialogItem(hDlg, GUI_ID_EDIT1);
				EDIT_GetText(hEdit1, Tmp_PatientInfo.Patient_ID, EDIT_GetNumChars(hEdit1));//wff add
				Patient_Info = Tmp_PatientInfo;

				if(Patient_Info.Patient_Type == PATIENTTYPE_ADULT)
				{
					FlowLowLimit = 10000;
					FlowHighLimit = 1500;
				}
				else if(Patient_Info.Patient_Type == PATIENTTYPE_PAEDIATRIC)
				{
					FlowLowLimit = 3333;
					FlowHighLimit = 1000;
				}
				else
				{
					FlowLowLimit = 3333;
					FlowHighLimit = 500;
				}
				//根据病人类型初始化呼吸参数提示信息
				Initcbdialog32string(&Patient_Info, BeathSettingPara.BreathMode);               
                
				//根据病人信息初始化呼吸参数
				InitPatient_SettingParameter(&BeathSettingPara);
				BreathPara_Assign(&Breath_Setting_Tmp, &BeathSettingPara);
				//根据病人信息初始化报警限制参数
				InitAlarmSettingParameter(&AlarmSettingPara, &Patient_Info, &BeathSettingPara);
// 				InitPatient_AlarmSettingParameter(0,&Patient_Info,&AlarmSettingPara,&BeathSettingPara);
				//初始化限制保存参数
				InitAlarmSaveData(&AlarmSettingPara);
				//初始化系统设置界面参数  开关(所有安装的模块都打开SpO2、EtCO2 and O2)
				//                InitModeOnOff();    //djq del 20160721
				//初始化Fresh Gas界面O2和FG flow默认数值
				//InitFreshGasHwin();//20150305
                
				//发送模式
				ReviseParamatersAllMode(BeathSettingPara.BreathMode,&BeathSettingPara,AlarmSettingPara.PressureHighValue);
				SendSetModes(BeathSettingPara.BreathMode,&BeathSettingPara);
				//更新模式的显示参数
				WM_SendMessageNoPara(Menu_hWin.bWin,WM_INIT_DIALOG);//更新参数Pinsp<->Vt
				//更新窗口的配色显示

				//清空趋势图信息
				//ClearTrendInfo();

				if(StandbyState==0xFF)
				{
					PlanMenuType = PATIENT_DATA_ASK_MENU;
				}
				else if(StandbyState==0)
				{
					PlanMenuType = MAIN_MENU;
				}
				Task |= MEAU_UPDATE;
				//保存病人信息
				Patient_WriteInfo();
				//更新信息栏图标
				Menu_hWin.InfoWin.InfoChangeFlag = (PATIENT_TYPE_INFO_CHANGE_FLAG|MODES_INFO_CHANGED_FLAG);
				break;
			case GUI_ID_BUTTON8:
				if(ButtonRotation_GetTriangleSign(pMsg->hWinSrc))
				{
					int Data;
					ButtonRotation_SetTriangleSign(pMsg->hWinSrc,0);
					Data = ButtonRotation_GetData1(pMsg->hWinSrc);
					ButtonRotation_SetData(pMsg->hWinSrc,Data);

					//身高
					Tmp_PatientInfo.Patient_Stature	= UnitConversion_Reverse(DISP_HEIGHT_VALUE,HeightUnit,Data);
					//计算IBW
					if(Tmp_PatientInfo.Patient_Type == PATIENTTYPE_INFANT)//婴儿类型为小数
					{
						Tmp_PatientInfo.Patient_IBW = __GetIBWInfo(Tmp_PatientInfo,Data);
						tmp = UnitConversion(DISP_WEIGHT_VALUE,WeightUnit, (int)Tmp_PatientInfo.Patient_IBW);
						sprintf(IBW_Value,"%2.1f", tmp/10.0);//显示IBW
					}
					else 
					{
						Tmp_PatientInfo.Patient_IBW = __GetIBWInfo(Tmp_PatientInfo,Data);
						tmp = UnitConversion(DISP_WEIGHT_VALUE,WeightUnit, (int)Tmp_PatientInfo.Patient_IBW);
						sprintf(IBW_Value,"%d", tmp);//显示IBW
					}
					hText4 = WM_GetDialogItem(hDlg, GUI_ID_TEXT4);
					TEXT_SetTextAlign(hText4,TEXT_CF_HCENTER|TEXT_CF_VCENTER);
					TEXT_SetText(hText4,IBW_Value);
				}
				else
				{
					int Data;
					Data = ButtonRotation_GetData(pMsg->hWinSrc);
					ButtonRotation_SetData1(pMsg->hWinSrc,Data);
					ButtonRotation_SetTriangleSign(pMsg->hWinSrc,1);
					hButtonRation1 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON9);
					ButtonRotation_SetTriangleSign(hButtonRation1,0);
				}
				break;
			case GUI_ID_BUTTON9:
				if(ButtonRotation_GetTriangleSign(pMsg->hWinSrc))
				{
					int Data;
					ButtonRotation_SetTriangleSign(pMsg->hWinSrc,0);
					Data = ButtonRotation_GetData1(pMsg->hWinSrc);
					ButtonRotation_SetData(pMsg->hWinSrc,Data);

					//年龄
					Tmp_PatientInfo.Patient_Age	= Data;
				}
				else
				{
					int Data;
					Data = ButtonRotation_GetData(pMsg->hWinSrc);
					ButtonRotation_SetData1(pMsg->hWinSrc,Data);
					ButtonRotation_SetTriangleSign(pMsg->hWinSrc,1);
					hButtonRation0 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON8);
					ButtonRotation_SetTriangleSign(hButtonRation0,0);

					//计算IBW
					if(Tmp_PatientInfo.Patient_Type == PATIENTTYPE_INFANT)//婴儿类型为小数
					{
						Tmp_PatientInfo.Patient_IBW = __GetIBWInfo(Tmp_PatientInfo, Tmp_PatientInfo.Patient_Stature);
						tmp = UnitConversion(DISP_WEIGHT_VALUE,WeightUnit, (int)Tmp_PatientInfo.Patient_IBW);
						sprintf(IBW_Value,"%2.1f", tmp/10.0);//显示IBW
					}
					else 
					{
						Tmp_PatientInfo.Patient_IBW = __GetIBWInfo(Tmp_PatientInfo, Tmp_PatientInfo.Patient_Stature);
						tmp = UnitConversion(DISP_WEIGHT_VALUE,WeightUnit, (int)Tmp_PatientInfo.Patient_IBW);
						sprintf(IBW_Value,"%d", tmp);//显示IBW
					}
					hText4 = WM_GetDialogItem(hDlg, GUI_ID_TEXT4);
					TEXT_SetTextAlign(hText4,TEXT_CF_HCENTER|TEXT_CF_VCENTER);
					TEXT_SetText(hText4,IBW_Value);
				}
				break;
			default:
				break; 
			}
			break;
		case GUI_KEY_ENTER:
			if(Id == GUI_ID_BUTTON8)
			{
				if(ButtonRotation_GetTriangleSign(pMsg->hWinSrc))
				{
					int Data;
					ButtonRotation_SetTriangleSign(pMsg->hWinSrc,0);
					Data = ButtonRotation_GetData1(pMsg->hWinSrc);
					ButtonRotation_SetData(pMsg->hWinSrc,Data);
					//身高
					Tmp_PatientInfo.Patient_Stature	= Data;

					//计算IBW
					if(Tmp_PatientInfo.Patient_Type == PATIENTTYPE_INFANT)//婴儿类型为小数
					{
						Tmp_PatientInfo.Patient_IBW = __GetIBWInfo(Tmp_PatientInfo,Data);
						tmp = UnitConversion(DISP_WEIGHT_VALUE,WeightUnit, (int)Tmp_PatientInfo.Patient_IBW);
						sprintf(IBW_Value,"%2.1f", tmp/10.0);//身高转换为IBW
					}
					else 
					{
						Tmp_PatientInfo.Patient_IBW = __GetIBWInfo(Tmp_PatientInfo,Data);
						tmp = UnitConversion(DISP_WEIGHT_VALUE,WeightUnit, (int)Tmp_PatientInfo.Patient_IBW);
						sprintf(IBW_Value,"%d", tmp);//显示IBW
					}
					hText4 = WM_GetDialogItem(hDlg, GUI_ID_TEXT4);
					TEXT_SetTextAlign(hText4,TEXT_CF_HCENTER|TEXT_CF_VCENTER);
					TEXT_SetText(hText4,IBW_Value);		
				}
			}
			else if(Id == GUI_ID_BUTTON9)
			{
				if(ButtonRotation_GetTriangleSign(pMsg->hWinSrc))
				{
					int Data;
					ButtonRotation_SetTriangleSign(pMsg->hWinSrc,0);
					Data = ButtonRotation_GetData1(pMsg->hWinSrc);
					ButtonRotation_SetData(pMsg->hWinSrc,Data);
					//身高
					Tmp_PatientInfo.Patient_Age	= Data;		
				}
			}
			else if(Id == GUI_ID_EDIT0 || Id == GUI_ID_EDIT1)
			{
				hEdit0 = WM_GetDialogItem(hDlg, GUI_ID_EDIT0);
				EDIT_GetText(hEdit0,
					Tmp_PatientInfo.Patient_Name,
					EDIT_GetNumChars(hEdit0));//wff add
				hEdit1 = WM_GetDialogItem(hDlg, GUI_ID_EDIT1);
				EDIT_GetText(hEdit1,
					Tmp_PatientInfo.Patient_ID,
					EDIT_GetNumChars(hEdit1));//wff add
			}
 			if(WM_IsVisible(KeyBoard_GetHandle()))
 			{
 				WM_HideWindow(KeyBoard_GetHandle());	//wff add 隐藏键盘
 			}
			break;
		case GUI_KEY_LEFT:
			if(Id == GUI_ID_BUTTON8)
			{
				if(ButtonRotation_GetTriangleSign(pMsg->hWinSrc))
				{
					ButtonRotation_DataSub(pMsg->hWinSrc);
					Msg.MsgId   = WM_NOTIFICATION_VALUE_CHANGED;
					Msg.Data.v  = GUI_KEY_LEFT;
					Msg.hWin = pMsg->hWinSrc;
					WM_SendMessage(pMsg->hWinSrc,&Msg);

					//身高
					tmp_Height	= ButtonRotation_GetData1(pMsg->hWinSrc);

					//计算IBW
					if(Tmp_PatientInfo.Patient_Type == PATIENTTYPE_INFANT)//婴儿类型为小数
					{
						tmp_IBW = __GetIBWInfo(Tmp_PatientInfo,tmp_Height);
						tmp = UnitConversion(DISP_WEIGHT_VALUE,WeightUnit,tmp_IBW);
						sprintf(IBW_Value,"%2.1f", tmp/10.0);//身高转换为IBW
					}
					else 
					{
						tmp_IBW = __GetIBWInfo(Tmp_PatientInfo,tmp_Height);
						tmp = UnitConversion(DISP_WEIGHT_VALUE,WeightUnit, tmp_IBW);
						sprintf(IBW_Value,"%d", tmp);//身高转换为IBW
					}
					hText4 = WM_GetDialogItem(hDlg, GUI_ID_TEXT4);
					TEXT_SetTextAlign(hText4,TEXT_CF_HCENTER|TEXT_CF_VCENTER);
					TEXT_SetText(hText4,IBW_Value);
				}
			}
			if(Id == GUI_ID_BUTTON9)
			{
				if(ButtonRotation_GetTriangleSign(pMsg->hWinSrc))
				{
					ButtonRotation_DataSub(pMsg->hWinSrc);
					Msg.MsgId   = WM_NOTIFICATION_VALUE_CHANGED;
					Msg.Data.v  = GUI_KEY_LEFT;
					Msg.hWin = pMsg->hWinSrc;
					WM_SendMessage(pMsg->hWinSrc,&Msg);

					//身高
					//tmp_Age	= ButtonRotation_GetData1(pMsg->hWinSrc);
				}
			}
			break;
		case GUI_KEY_RIGHT:
			if(Id == GUI_ID_BUTTON8)
			{
				if(ButtonRotation_GetTriangleSign(pMsg->hWinSrc))
				{
					ButtonRotation_DataAdd(pMsg->hWinSrc);
					Msg.MsgId   = WM_NOTIFICATION_VALUE_CHANGED;
					Msg.Data.v  = GUI_KEY_LEFT;
					Msg.hWin = pMsg->hWinSrc;
					WM_SendMessage(pMsg->hWinSrc,&Msg);
					//身高
					tmp_Height	= ButtonRotation_GetData1(pMsg->hWinSrc);

					//计算IBW
					if(Tmp_PatientInfo.Patient_Type == PATIENTTYPE_INFANT)//婴儿类型为小数
					{
						tmp_IBW = __GetIBWInfo(Tmp_PatientInfo,tmp_Height);
						tmp = UnitConversion(DISP_WEIGHT_VALUE,WeightUnit,tmp_IBW);
						sprintf(IBW_Value,"%2.1f", tmp/10.0);//身高转换为IBW
					}
					else 
					{
						tmp_IBW = __GetIBWInfo(Tmp_PatientInfo,tmp_Height);
						tmp = UnitConversion(DISP_WEIGHT_VALUE,WeightUnit, tmp_IBW);
						sprintf(IBW_Value,"%d", tmp);//身高转换为IBW
					}
					hText4 = WM_GetDialogItem(hDlg, GUI_ID_TEXT4);
					TEXT_SetTextAlign(hText4,TEXT_CF_HCENTER|TEXT_CF_VCENTER);
					TEXT_SetText(hText4,IBW_Value);
				}
			}
			if(Id == GUI_ID_BUTTON9)
			{
				if(ButtonRotation_GetTriangleSign(pMsg->hWinSrc))
				{
					ButtonRotation_DataAdd(pMsg->hWinSrc);
					Msg.MsgId   = WM_NOTIFICATION_VALUE_CHANGED;
					Msg.Data.v  = GUI_KEY_LEFT;
					Msg.hWin = pMsg->hWinSrc;
					WM_SendMessage(pMsg->hWinSrc,&Msg);
					//身高
					//tmp_Age	= ButtonRotation_GetData1(pMsg->hWinSrc);
				}
			}
			break;
		}
		break;
	case WM_NOTIFY_PARENT_REFLECTION:
		FunctionKeyPressedResume(Menu_hWin.hWin,0);
		if(StandbyState==0xFF)
		{
			PlanMenuType = STANDBY_MENU;
			Task |= MEAU_UPDATE;
		}
		else if(StandbyState==0)
		{
			PlanMenuType = MAIN_MENU;
			Task |= MEAU_UPDATE;
		}
		break;
	default:
		WM_DefaultProc(pMsg);
		break;
	}
}

/*************************************************************************
*   Old Patient
**/
static void _cbDialogOldPatientInfo(WM_MESSAGE * pMsg)
{
	WM_HWIN hDlg,hButton0,hButton1,hButton2,hButton3,hButton4,hButton5,hButton6,hButton7;
	WM_HWIN hButtonRation0,hButtonRation1,hText0,hText1,hText2,hText3,hText4,hText5,hText6,hText7;
	WM_HWIN hLine0,hLine1,hLine2,hLine3,hLine4,hLine5;
	WM_HWIN hEdit0,hEdit1;
	int Id,NCode;
	unsigned int tmp;
	unsigned char ch1,ch2;
	Patient_Info_t Tmp_OldPatientInfo;
	char IBW_Value[20];
	hDlg = pMsg->hWin;
	switch (pMsg->MsgId)
	{
	case WM_INIT_DIALOG:
		WINDOW_SetBkColor(hDlg,0xCDAA94);    //GUI_ADD

		//Patient
		Patient_InitOldInfo(&Tmp_OldPatientInfo,1);

        WM_HideWindow(KeyBoard_GetHandle());    //gyh add 20230727 点击上一病人隐藏虚拟键盘
        
		hEdit0 = WM_GetDialogItem(hDlg, GUI_ID_EDIT0); //wff add 
		EDIT_SetBkColor(hEdit0,0,0xFFEEE6);
		EDIT_SetTextAlign(hEdit0,GUI_TA_VCENTER);
		EDIT_SetFont(hEdit0,&GUI_Font_20);
		EDIT_SetMaxLen(hEdit0, EDIT_PID_SIZE);
		EDIT_SetText(hEdit0, Tmp_OldPatientInfo.Patient_Name);

		hEdit1 = WM_GetDialogItem(hDlg, GUI_ID_EDIT1); //wff add 
		EDIT_SetBkColor(hEdit1,0,0xFFEEE6);
		EDIT_SetTextAlign(hEdit1,GUI_TA_VCENTER);
		EDIT_SetFont(hEdit1,&GUI_Font_20);
		EDIT_SetMaxLen(hEdit1, EDIT_PN_SIZE);
		EDIT_SetText(hEdit1, Tmp_OldPatientInfo.Patient_ID);

		WM_DisableWindow(hEdit0);
		WM_DisableWindow(hEdit1);

		hText0 = WM_GetDialogItem(hDlg, GUI_ID_TEXT0);
		TEXT_SetText(hText0,Sys_Lib_GetDispStr(DP_PATIENT));

		hButton0 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON0);
		BUTTON_SetText(hButton0,Sys_Lib_GetDispStr(DP_PREVIOUSADULT));
		BUTTON_SetMinorTextType(hButton0,&GUI_FontPatientIcon32X32,"A",5,1);
		hButton1 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON1);
		BUTTON_SetText(hButton1,Sys_Lib_GetDispStr(DP_PREVIOUSPAEDIATRIC));
		BUTTON_SetMinorTextType(hButton1,&GUI_FontPatientIcon32X32,"P",5,1);
		hButton2 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON2);
		BUTTON_SetText(hButton2,Sys_Lib_GetDispStr(DP_PREVIOUSINFANT));
		BUTTON_SetMinorTextType(hButton2,&GUI_FontPatientIcon32X32,"I",5,1);
		//Gender
		hText1 = WM_GetDialogItem(hDlg, GUI_ID_TEXT1);
		TEXT_SetText(hText1,Sys_Lib_GetDispStr(DP_GENDER));

		hButton3 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON3);
		BUTTON_SetText(hButton3,Sys_Lib_GetDispStr(DP_MALE));
		BUTTON_SetMinorTextType(hButton3,&GUI_FontPatientIcon32X32,"M",5,1);
		hButton4 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON4);
		BUTTON_SetText(hButton4,Sys_Lib_GetDispStr(DP_FEMALE));
		BUTTON_SetMinorTextType(hButton4,&GUI_FontPatientIcon32X32,"F",5,1);

		BUTTON_SetTextWrapMode(hButton0, GUI_WRAPMODE_WORD);
		BUTTON_SetTextWrapMode(hButton1, GUI_WRAPMODE_WORD);
		BUTTON_SetTextWrapMode(hButton2, GUI_WRAPMODE_WORD);
		BUTTON_SetTextWrapMode(hButton3, GUI_WRAPMODE_WORD);
		BUTTON_SetTextWrapMode(hButton4, GUI_WRAPMODE_WORD);
		//IBW
		hButtonRation0 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON8);
// 		ButtonRotation_SetDataInvildSign(hButtonRation0,4);
		ButtonRotation_SetDataInfo(hButtonRation0,48,250,Tmp_OldPatientInfo.Patient_Stature,1,0);
		ButtonRotation_SetData(hButtonRation0,Tmp_OldPatientInfo.Patient_Stature);
		if(!HeightUnit)
		{
			ButtonRotation_SetText(hButtonRation0,Sys_Lib_GetDispStr(DP_EMPTY),Sys_Lib_GetDispStr(DP_CM));
		}
		else
		{
			ButtonRotation_SetText(hButtonRation0,Sys_Lib_GetDispStr(DP_EMPTY),Sys_Lib_GetDispStr(DP_IN));
		}

		//Age
		hButtonRation1 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON9);
		ButtonRotation_SetDataInfo(hButtonRation1,0,130,Tmp_OldPatientInfo.Patient_Age,1,0);
		ButtonRotation_SetData(hButtonRation1,Tmp_OldPatientInfo.Patient_Age);
		if(Tmp_OldPatientInfo.Patient_Type == PATIENTTYPE_INFANT) //软件问题-GUI #1234
		{
			ButtonRotation_SetText(hButtonRation1,Sys_Lib_GetDispStr(DP_EMPTY),Sys_Lib_GetDispStr(DP_MONTHS));
		}
		else
		{
			ButtonRotation_SetText(hButtonRation1,Sys_Lib_GetDispStr(DP_EMPTY),Sys_Lib_GetDispStr(DP_YEARS));
		}
		//IBW
		hText4 = WM_GetDialogItem(hDlg, GUI_ID_TEXT4);
		TEXT_SetFont(hText4,&GUI_Font32B_ASCII);
		if(Tmp_OldPatientInfo.Patient_Type == PATIENTTYPE_INFANT)//婴儿的IBW可以为小数
		{
			sprintf(IBW_Value,"%2.1f",(float)UnitConversion(DISP_WEIGHT_VALUE,WeightUnit,(int)Tmp_OldPatientInfo.Patient_IBW)/10.0);
		}
		else
		{
			sprintf(IBW_Value,"%3.0f",(float)UnitConversion(DISP_WEIGHT_VALUE,WeightUnit,(int)Tmp_OldPatientInfo.Patient_IBW));
		}
		TEXT_SetText(hText4,IBW_Value);  //初始化为.。。    GUI_FLAG  设置IBW的身高体重关系

		hText5 = WM_GetDialogItem(hDlg, GUI_ID_TEXT5);
		TEXT_SetFont(hText5,&GUI_Font_16);
		if(1/*!WeightUnit*/)
		{
			sprintf(IBW_Value,"%s\n %s",Sys_Lib_GetDispStr(DP_IBW),Sys_Lib_GetDispStr(DP_KG));
			TEXT_SetText(hText5,IBW_Value);
		}
		else
		{
// 			sprintf(IBW_Value,"%s\n %s",Sys_Lib_GetDispStr(DP_IBW),Sys_Lib_GetDispStr(DP_IB));
			TEXT_SetText(hText5,IBW_Value);
		}
		//Application
		hText2 = WM_GetDialogItem(hDlg, GUI_ID_TEXT2);
		TEXT_SetText(hText2,Sys_Lib_GetDispStr(DP_HEIGHT));
		hText3 = WM_GetDialogItem(hDlg, GUI_ID_TEXT3);
		TEXT_SetText(hText3,Sys_Lib_GetDispStr(DP_AGE));

		//Name
		hText6 = WM_GetDialogItem(hDlg, GUI_ID_TEXT6); //wff add 
		TEXT_SetText(hText6, Sys_Lib_GetDispStr(DP_NAME));
		//Case Number
		hText7 = WM_GetDialogItem(hDlg, GUI_ID_TEXT7); //wff add
		TEXT_SetText(hText7,"Id");
		//Start
		hButton7 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON7);
		BUTTON_SetFont(hButton7,&GUI_Font_24);
		BUTTON_SetText(hButton7,Sys_Lib_GetDispStr(DP_CONFIRM));

		//Patient Type
		WM_DisableWindow(hButton0);
		WM_DisableWindow(hButton1);
		WM_DisableWindow(hButton2);

		WM_EnableWindow(WM_GetDialogItem(hDlg,(GUI_ID_BUTTON0+Tmp_OldPatientInfo.Patient_Type)));
		BUTTON_SetState(WM_GetDialogItem(hDlg,(GUI_ID_BUTTON0+Tmp_OldPatientInfo.Patient_Type)),BUTTON_STATE_PRESSED);//默认选项
		//Gender
		WM_DisableWindow(hButton3);
		WM_DisableWindow(hButton4);

		WM_EnableWindow(WM_GetDialogItem(hDlg,(GUI_ID_BUTTON3+Tmp_OldPatientInfo.Patient_Gender)));
		BUTTON_SetState(WM_GetDialogItem(hDlg,(GUI_ID_BUTTON3+Tmp_OldPatientInfo.Patient_Gender)),BUTTON_STATE_PRESSED);//默认选项
		//IBW 
		WM_DisableWindow(WM_GetDialogItem(hDlg,GUI_ID_BUTTON8));
		WM_DisableWindow(WM_GetDialogItem(hDlg,GUI_ID_BUTTON9));

		break;
	case WM_NOTIFY_PARENT:
		NCode = pMsg->Data.v;
		Id = WM_GetId(pMsg->hWinSrc);;
		switch(NCode)
		{
		case WM_NOTIFICATION_CLICKED:
			switch(Id)
			{
				//start
			case GUI_ID_BUTTON7:
				StartPatientChooseFlag = 0;

				Patient_InitOldInfo(&Patient_Info,0);

				Initcbdialog32string(&Patient_Info, BeathSettingPara.BreathMode);//根据病人类型初始化呼吸参数提示信息

				InitSettingOldPatientParameter(&BeathSettingPara);
                
                BreathPara_Assign(&Breath_Setting_Tmp, &BeathSettingPara);//
                
				InitAlarmSettingOldPatientParameter(&AlarmSettingPara);
				//初始化Fresh Gas界面O2和FG flow默认数值
				//InitFreshGasHwin();//20150305
				//更新模式的显示参数
				WM_SendMessageNoPara(Menu_hWin.bWin,WM_INIT_DIALOG);//更新参数Pinsp<->Vt

                Menu_hWin.InfoWin.InfoChangeFlag = (PATIENT_TYPE_INFO_CHANGE_FLAG|MODES_INFO_CHANGED_FLAG);
                
				if(StandbyState==0)
				{
					PlanMenuType=MAIN_MENU;
					Task |= MEAU_UPDATE;
				}
				else if(StandbyState==0xFF)
				{
					PlanMenuType=STANDBY_MENU;
					Task |= MEAU_UPDATE;
				}				
				break;
			default:
				break; 
			}
			break;
		}
		break;
	case WM_NOTIFY_PARENT_REFLECTION:
		FunctionKeyPressedResume(Menu_hWin.hWin,0);
		if(StandbyState==0xFF)
		{
			PlanMenuType = STANDBY_MENU;
			Task |= MEAU_UPDATE;  
		}
		else if(StandbyState==0)
		{
			PlanMenuType = MAIN_MENU;
			Task |= MEAU_UPDATE;  
		}
		break;
	default:
		WM_DefaultProc(pMsg);
		break;
	}
}


//初始化上一病人信息
static void Patient_InitOldInfo(Patient_Info_t *Patient,int InitFlag)
{
	unsigned int tmp;
	unsigned char ch1,ch2;

	Patient_ReadID(Patient);
	Patient_ReadName(Patient);

	Patient->Patient_Type = RTEEPROMread(PATIENT_INFO_TYPE_ADDR);
	if((Patient->Patient_Type > 3)||(Patient->Patient_Type < 0))
	{
		Patient->Patient_Type = 0; 
	}
	Patient->Patient_Gender = RTEEPROMread(PATIENT_INFO_GENDER_ADDR);
	if((Patient->Patient_Gender > 1)||(Patient->Patient_Gender < 0))
	{
		Patient->Patient_Gender = 0; 
	}
	Patient->Patient_VentType = RTEEPROMread(PATIENT_INFO_VENTTYPE_ADDR);
	if((Patient->Patient_VentType > 1)||(Patient->Patient_VentType < 0))
	{
		Patient->Patient_VentType = 0; 
	}
	ch1 = RTEEPROMread(PATIENT_INFO_IBW_LOW_ADDR);
	ch2 = RTEEPROMread(PATIENT_INFO_IBW_HIGH_ADDR);
	tmp = ch1 | (ch2<<8);
	if(Patient->Patient_Type == PATIENTTYPE_INFANT)
	{
		tmp = tmp/10;
	}
	else
	{
	}
	if((tmp > 0) && (tmp < 2500))
	{
		Patient->Patient_IBW = tmp/10.0;
	}
	else 
	{
		Patient->Patient_IBW = 71;
	}
	Patient->Patient_Stature = RTEEPROMread(PATIENT_INFO_STATURE_ADDR);
	if((Patient->Patient_Stature < 48) && (Patient->Patient_Stature > 250))
	{
		Patient->Patient_Stature = 175;
	}
	Patient->Patient_Age = RTEEPROMread(PATIENT_INFO_AGE_ADDR);
	if((Patient->Patient_Age > 130) && (Patient->Patient_Age < 0))
	{
		Patient->Patient_Age = 32;
	}
	if(!InitFlag)
	{
		if(Patient->Patient_Type == PATIENTTYPE_ADULT)
		{
			FlowLowLimit = 10000;
			FlowHighLimit = 1500;
		}
		else if(Patient->Patient_Type == PATIENTTYPE_PAEDIATRIC)
		{
			FlowLowLimit = 3333;
			FlowHighLimit = 1000;
		}
		else
		{
			FlowLowLimit = 3333;
			FlowHighLimit = 500;
		}
	}
}

void Patient_WriteInfo(void)
{
	//保存病人信息
	RTEEPROMwrite(PATIENT_INFO_TYPE_ADDR, Patient_Info.Patient_Type);
	RTEEPROMwrite(PATIENT_INFO_GENDER_ADDR, Patient_Info.Patient_Gender);
	RTEEPROMwrite(PATIENT_INFO_VENTTYPE_ADDR, Patient_Info.Patient_VentType);
	if(Patient_Info.Patient_Type == PATIENTTYPE_INFANT)
	{
		RTEEPROMwrite(PATIENT_INFO_IBW_LOW_ADDR, (int)(Patient_Info.Patient_IBW*100)&0xff);
		RTEEPROMwrite(PATIENT_INFO_IBW_HIGH_ADDR, ((int)(Patient_Info.Patient_IBW*100)&0xff00)>>8);
	}
	else
	{
		RTEEPROMwrite(PATIENT_INFO_IBW_LOW_ADDR, (int)(Patient_Info.Patient_IBW*10)&0xff);
		RTEEPROMwrite(PATIENT_INFO_IBW_HIGH_ADDR, ((int)(Patient_Info.Patient_IBW*10)&0xff00)>>8);
	}
	RTEEPROMwrite(PATIENT_INFO_STATURE_ADDR, Patient_Info.Patient_Stature);
	RTEEPROMwrite(PATIENT_INFO_AGE_ADDR, Patient_Info.Patient_Age);

	Patient_WriteID(&Patient_Info);
	Patient_WriteName(&Patient_Info);
}

void Patient_ReadID(Patient_Info_t *info)
{
	char *p = NULL;
	char buf[PID_SIZE+1] = {0};
	int i = 0, pos = 0;

	for(i=0; i<PID_SIZE; i++)
	{
		buf[i] = RTEEPROMread(PATIENT_INFO_ID_ADDR+i);
	}
	buf[i] = '\0';

	if(buf[0] == PID_MARK)
	{
		p = strchr(&buf[1], PID_MARK);
		if(p != 0)
		{
			pos = p-buf-1;
			memcpy(info->Patient_ID, &buf[1], pos);
			info->Patient_ID[pos] = '\0';
		}
		else
		{
			memset(info->Patient_ID, 0, PN_SIZE);
		}
	}
	else
	{
		memset(info->Patient_ID, 0, PN_SIZE);
	}
}

void Patient_WriteID(Patient_Info_t *info)
{
	int i = 0, ofs = PATIENT_INFO_ID_ADDR;
	int size = strlen(info->Patient_ID);

	RTEEPROMwrite(ofs++, PID_MARK);
	if(size <= EDIT_PID_SIZE)
	{
		for(i=0; i<size; i++)
		{
			RTEEPROMwrite(ofs+i, info->Patient_ID[i]);
		}
		RTEEPROMwrite(ofs+i, PID_MARK);
	}
}

void Patient_ReadName(Patient_Info_t *info)
{
	char *p = NULL;
	char buf[PN_SIZE+1] = {0};
	int i = 0, pos = 0;

	for(i=0; i<PN_SIZE; i++)
	{
		buf[i] = RTEEPROMread(PATIENT_INFO_Name_ADDR+i);
	}
	buf[i] = '\0';

	if(buf[0] == PID_MARK)
	{
		p = strchr(&buf[1], PID_MARK);
		if(p != 0)
		{
			pos = p-buf-1;
			memcpy(info->Patient_Name, &buf[1], pos);
			info->Patient_Name[pos] = '\0';
		}
		else
		{
			memset(info->Patient_Name, 0, PN_SIZE);
		}
	}
	else
	{
		memset(info->Patient_Name, 0, PN_SIZE);
	}
}

void Patient_WriteName(Patient_Info_t *info)
{
	int i = 0, ofs = PATIENT_INFO_Name_ADDR;
	int size = strlen(info->Patient_Name);

	RTEEPROMwrite(ofs++, PID_MARK);
	if(size <= EDIT_PN_SIZE)
	{
		for(i=0; i<size; i++)
		{
			RTEEPROMwrite(ofs+i, info->Patient_Name[i]);
		}
		RTEEPROMwrite(ofs+i, PID_MARK);
	}
}


//根据当前病人信息，计算IBW
static int __GetIBWInfo(Patient_Info_t PatientInfo,int Stature)
{
	float ibw;
	int ret;
	//20181031
	if(PatientInfo.Patient_Type == PATIENTTYPE_ADULT)//成人
	{
		if(PatientInfo.Patient_Gender == GENDER_MALE)
		{
			if(Stature > 128)
			{
				ibw =50 + 0.91*((float)Stature - 152.4);
			}
			else
			{
				ibw =(1.65*(float)(Stature*Stature))/1000.0;
			}
		}
		else if(PatientInfo.Patient_Gender == GENDER_FEMALE)
		{
			if(Stature > 136)
			{
				ibw =45.5 + 0.91*((float)Stature - 152.4) ;
			}
			else
			{
				ibw =(1.65*(float)(Stature*Stature))/1000.0;
			}
		}

		if(ibw > 150)
		{
			ibw = 150;
		}
		else if(ibw < 15)
		{
			ibw = 15;
		}
	}
	else if(PatientInfo.Patient_Type == PATIENTTYPE_PAEDIATRIC)//儿童
	{
		if(Stature > 60 && Stature <= 152)
		{
			ibw =(1.65*(float)(Stature*Stature))/1000.0;

			if(ibw > 38)
			{
				ibw = 38;
			}
			else if(ibw < 6)
			{
				ibw = 6;
			}
		}
		else if(Stature >= 50 && Stature <= 60)
		{
			ibw = 0.00157*Stature*Stature + 0.09551*Stature - 5.5;
		}
		else if(Stature >= 48 && Stature < 50)
		{
			ibw = 0.0046*Stature*Stature - 0.2439*Stature + 3.8;
		}
		else 
		{
			//if(PatientInfo.Patient_Gender == GENDER_MALE)//男性
			//{
			ibw =39 + 0.89*((float)Stature - 152.4);
			//}
			//else if(PatientInfo.Patient_Gender == GENDER_FEMALE)//女性
			//{
			//	ibw =42.2 + 0.89*((float)Stature - 152.4);
			//}

			if(ibw > 50)
			{
				ibw = 50;
			}
			else if(ibw < 39)
			{
				ibw = 39;
			}
		}
	}
	else if(PatientInfo.Patient_Type == PATIENTTYPE_INFANT)//婴儿
	{
		if(Stature >= 50)
		{
			ibw = 0.00157*Stature*Stature + 0.09551*Stature - 5.5;
		}
		else 
		{
			ibw = 0.0046*Stature*Stature - 0.2439*Stature + 3.8;
		}
	}
	if(PatientInfo.Patient_Type == PATIENTTYPE_INFANT)//如果是婴儿，先转换成整数输出，在由外边转换为1位小数
	{
		ret = ibw*10 + 0.5;
	}
	else
	{
		ret = ibw + 0.5;
	}
	return (ret);
}

/*************************************************************************
*   New Patient
**/

//根据当前病人信息，获取相应的默认信息并且更新IBW的显示
static void __GetPatientInfo(WM_HWIN hDlg,Patient_Info_t *PatientInfo)
{
	WM_HWIN hText4 = 0;
	char IBW_Value[20] = {0};
	int tmp = 0;

	if(PatientInfo->Patient_Type == PATIENTTYPE_ADULT)//成人
	{
		ButtonRotation_SetDataInfo(WM_GetDialogItem(hDlg, GUI_ID_BUTTON8),110,250,STATURE_ADULT_MALE,1,0);//设置身高范围
		if(PatientInfo->Patient_Gender == GENDER_MALE)
		{
			ButtonRotation_SetData(WM_GetDialogItem(hDlg, GUI_ID_BUTTON8),STATURE_ADULT_MALE);//默认身高
			PatientInfo->Patient_Stature = STATURE_ADULT_MALE;
			PatientInfo->Patient_IBW = IBW_ADULT_MALE;//默认IBW
		}
		else if(PatientInfo->Patient_Gender == GENDER_FEMALE)
		{
			ButtonRotation_SetData(WM_GetDialogItem(hDlg, GUI_ID_BUTTON8),STATURE_ADULT_WEMALE);//默认身高
			PatientInfo->Patient_Stature = STATURE_ADULT_WEMALE;
			PatientInfo->Patient_IBW = IBW_ADULT_WEMALE;//默认IBW
		}

		ButtonRotation_SetText(WM_GetDialogItem(hDlg, GUI_ID_BUTTON9), " ", Sys_Lib_GetDispStr(DP_YEARS));//软件问题-GUI #1211
		ButtonRotation_SetDataInfo(WM_GetDialogItem(hDlg, GUI_ID_BUTTON9),12,130,AGE_ADULT,1,0);//设置年龄范围
		ButtonRotation_SetData(WM_GetDialogItem(hDlg, GUI_ID_BUTTON9),AGE_ADULT);//默认年龄
		PatientInfo->Patient_Age = AGE_ADULT;
	}
	else if(PatientInfo->Patient_Type == PATIENTTYPE_PAEDIATRIC)//儿童
	{
		if(PatientInfo->Patient_Gender == GENDER_MALE)
		{
#ifdef LANGUAGE_CHINESE_FLAG
			ButtonRotation_SetDataInfo(WM_GetDialogItem(hDlg, GUI_ID_BUTTON8),48,165,STATURE_PEAD_MALE,1,0);//设置身高范围
#else
			ButtonRotation_SetDataInfo(WM_GetDialogItem(hDlg, GUI_ID_BUTTON8),65,165,STATURE_PEAD_MALE,1,0);//设置身高范围
#endif
			ButtonRotation_SetData(WM_GetDialogItem(hDlg, GUI_ID_BUTTON8),STATURE_PEAD_MALE);//默认身高
			PatientInfo->Patient_Stature = STATURE_PEAD_MALE;
		}
		else if(PatientInfo->Patient_Gender == GENDER_FEMALE)
		{
#ifdef LANGUAGE_CHINESE_FLAG
			ButtonRotation_SetDataInfo(WM_GetDialogItem(hDlg, GUI_ID_BUTTON8),48,161,STATURE_PEAD_WEMALE,1,0);//设置身高范围
#else
			ButtonRotation_SetDataInfo(WM_GetDialogItem(hDlg, GUI_ID_BUTTON8),65,161,STATURE_PEAD_WEMALE,1,0);//设置身高范围
#endif
			ButtonRotation_SetData(WM_GetDialogItem(hDlg, GUI_ID_BUTTON8),STATURE_PEAD_WEMALE);//默认身高
			PatientInfo->Patient_Stature = STATURE_PEAD_WEMALE;
		}
		PatientInfo->Patient_IBW = IBW_PEAD;//默认IBW

		ButtonRotation_SetText(WM_GetDialogItem(hDlg, GUI_ID_BUTTON9), " ", Sys_Lib_GetDispStr(DP_YEARS));//软件问题-GUI #1211
#ifdef LANGUAGE_CHINESE_FLAG        
		ButtonRotation_SetDataInfo(WM_GetDialogItem(hDlg, GUI_ID_BUTTON9),3,16,AGE_PEAD,1,0);//设置年龄范围
#else
		ButtonRotation_SetDataInfo(WM_GetDialogItem(hDlg, GUI_ID_BUTTON9),0,16,AGE_PEAD,1,0);//设置年龄范围
#endif
		ButtonRotation_SetData(WM_GetDialogItem(hDlg, GUI_ID_BUTTON9),AGE_PEAD);//默认年龄
		PatientInfo->Patient_Age = AGE_PEAD;

	}
	else if(PatientInfo->Patient_Type == PATIENTTYPE_INFANT)//婴儿
	{
#ifdef LANGUAGE_CHINESE_FLAG
		ButtonRotation_SetDataInfo(WM_GetDialogItem(hDlg, GUI_ID_BUTTON8),48,120,STATURE_INFANT,1,0);//设置身高范围
#else
		ButtonRotation_SetDataInfo(WM_GetDialogItem(hDlg, GUI_ID_BUTTON8),48,65,STATURE_INFANT,1,0);//设置身高范围
#endif
		ButtonRotation_SetData(WM_GetDialogItem(hDlg, GUI_ID_BUTTON8),STATURE_INFANT);//默认身高
		PatientInfo->Patient_Stature = STATURE_INFANT;
		PatientInfo->Patient_IBW = IBW_INFANT;//默认IBW
#ifdef LANGUAGE_CHINESE_FLAG        
		ButtonRotation_SetText(WM_GetDialogItem(hDlg, GUI_ID_BUTTON9), " ", Sys_Lib_GetDispStr(DP_YEARS));
		ButtonRotation_SetDataInfo(WM_GetDialogItem(hDlg, GUI_ID_BUTTON9),3,6,AGE_INFANT,1,0);//设置年龄范围
#else
		ButtonRotation_SetText(WM_GetDialogItem(hDlg, GUI_ID_BUTTON9), " ", Sys_Lib_GetDispStr(DP_MONTHS));//软件问题-GUI #1211
		ButtonRotation_SetDataInfo(WM_GetDialogItem(hDlg, GUI_ID_BUTTON9),0,24,AGE_INFANT,1,0);//设置年龄范围
#endif
		ButtonRotation_SetData(WM_GetDialogItem(hDlg, GUI_ID_BUTTON9),AGE_INFANT);//默认年龄
		PatientInfo->Patient_Age = AGE_INFANT;
	}

	tmp = UnitConversion(DISP_WEIGHT_VALUE,WeightUnit, (int)PatientInfo->Patient_IBW);
	if(PatientInfo->Patient_Type == PATIENTTYPE_INFANT)
	{
		sprintf(IBW_Value,"%2.1f", (float)tmp);//显示IBW
	}
	else 
	{
		sprintf(IBW_Value,"%d", tmp);//显示IBW
	}
	hText4 = WM_GetDialogItem(hDlg, GUI_ID_TEXT4);
	TEXT_SetTextAlign(hText4,TEXT_CF_HCENTER|TEXT_CF_VCENTER);
	TEXT_SetText(hText4,IBW_Value);//默认IBW
}

Patient_Info_t* Patient_GetInfo(void)
{
	return &Patient_Info;
}

char* Patient_GetPID(void)
{
	return Patient_Info.Patient_ID;
}

char* Patient_GetPName(void)
{
	return Patient_Info.Patient_Name;
}

PType_t Patient_GetPType(void)
{
	return Patient_Info.Patient_Type;
}

PGender_t Patient_GetPGender(void)
{
	return Patient_Info.Patient_Gender;
}

PVentType_t Patient_GetPVentType(void)
{
	return Patient_Info.Patient_VentType;
}

u8 Patient_GetStature(void)
{
	return Patient_Info.Patient_Stature;
}

u8 Patient_GetAge(void)
{
	return Patient_Info.Patient_Age;
}

float Patient_GetIBW(void)
{
	return Patient_Info.Patient_IBW;
}


void Patient_SetPID(char *buf, char size)
{
	memcpy(Patient_Info.Patient_ID, buf, (size>PID_SIZE)?PID_SIZE:size);
}

void Patient_SetPName(char *buf, char size)
{
	memcpy(Patient_Info.Patient_Name, buf, (size>PID_SIZE)?PID_SIZE:size);
}

void Patient_SetPType(PType_t type)
{
	Patient_Info.Patient_Type = type;
}

void Patient_SetPGender(PGender_t type)
{
	Patient_Info.Patient_Gender = type;
}

void Patient_SetPVentType(PVentType_t type)
{
	Patient_Info.Patient_VentType = type;
}

void Patient_SetStature(u8 data)
{
	Patient_Info.Patient_Stature = data;
}

void Patient_SetAge(u8 data)
{
	Patient_Info.Patient_Age = data;
}

void Patient_SetIBW(float data)
{
	Patient_Info.Patient_IBW = data;
}

u8 Patient_GetStartChooseFlag(void)
{
	return StartPatientChooseFlag;
}

/*************************************************************
*单位转换函数(由默认单位转换为非默认单位)
*输入：UnitFlag   要转换的单位的类型：例如：CO2、Height、Weight......
       UnitNum    要转换单位的标志：  co2unit.....
       value      要转换参数当前单位的值
*输出：value转换单位后的值
**************************************************************
*/
int UnitConversion(int UnitFlag,int UnitNum,int value)
{
    int ret = value;
    switch(UnitFlag)
    {
    case DISP_CO2_VALUE:
        if(UnitNum != 1)
        {
            ret = value*1.33+0.5;
        }
        break;
    case DISP_AIRWAY_VALUE:
        if(UnitNum)//非默认单位
        {
            ret = value;//不换算  约等于
        }
        break;
    case DISP_HEIGHT_VALUE://需要两个单位相互转换
        if(UnitNum)//非默认单位
        {
            ret = value*0.394+0.5;  //cm -> inch
        }
        break;
    case DISP_WEIGHT_VALUE:
        if(UnitNum)//非默认单位
        {
            ret = value*2.205+0.5;
        }
        break;
    case DISP_BAROMETRIC_VALUE:
        if(UnitNum)//非默认单位
        {
            ret = value*1.333+0.5;
        }
        break;
    case DISP_GAS_SUPPLY_VALUE:
        if(UnitNum)//非默认单位
        {
            ret = value*0.145+0.5;
        }
        break;
    default:
        ret = value;
        break;
    }
    return ret;
}
/*************************************************************
*单位转换函数(由非默认单位转换为默认单位)
*输入：UnitFlag   要转换的单位的类型：例如：CO2、Height、Weight......
       UnitNum    要转换单位的标志：  co2unit.....
       value      要转换参数当前单位的值
*输出：value转换单位后的值
**************************************************************
*/
int UnitConversion_Reverse(int UnitFlag,int UnitNum,int value)
{
    int ret = value;
    switch(UnitFlag)
    {
    case DISP_CO2_VALUE:
        //if(UnitNum != 1)
        //{
        //    ret = value*1.33+0.5;
        //}
        break;
    case DISP_AIRWAY_VALUE:
        //if(UnitNum)//非默认单位
        //{
        //    ret = value;//不换算  约等于
        //}
        break;
    case DISP_HEIGHT_VALUE://需要两个单位相互转换
        if(UnitNum)//非默认单位
        {
            ret = value/0.39;
        }
        break;
    case DISP_WEIGHT_VALUE:
        //if(UnitNum)//非默认单位
        //{
        //    ret = value*2.20+0.5;
        //}
        break;
    case DISP_BAROMETRIC_VALUE:
        //if(UnitNum)//非默认单位
        //{
        //    ret = value*1.33+0.5;
        //}
        break;
    case DISP_GAS_SUPPLY_VALUE:
        //if(UnitNum)//非默认单位
        //{
        //    ret = value*0.145+0.5;
        //}
        break;
    default:
        ret = value;
        break;
    }
    return (int)(ret);
}

