#include "WIDGET_Tools.h"
#include "ButtonExCtrl.h"
//#include "TracingCurve.h"
#include "Static_Menu_Creat.h"

extern MENU_HWIN Menu_hWin;
extern BREATH_SET_PARA BeathSettingPara;
extern MONITOR_PARA MonitorPara;
extern int MenuType;
extern int MenuOperateStatus;
extern REALTIME_PARA RealtimePara;
extern int PlanMenuType;
extern int Task;
extern GUI_COLOR Curve_Color;
extern Int8U Usart0_Tx_Buffer[4];

typedef struct
{
	//协议
	int TypeSelect;//0x01-NIF;0x02-P0.1;0x03-Lung
	int MeasureFlag;//0x55-停止;0xAA开始
	//
	int Time;//单位：s
	int CurveTime;
	//
	int LungStartFlag;
	int LungRecPressure;
	int LungRecTime;
	int LungInterval;//肺复张间隔
	//
	//int FlowPauseTime;
	//int FlowPauseStartFlag;
	//
	int CPBFlag;
}TOOLS_PARA;

TOOLS_PARA ToolsPara_t;

static const GUI_WIDGET_CREATE_INFO _aDialogCreateLungRec[] =
{
	{ WINDOW_CreateIndirect,   "",       0,				 0,   0,		659,   279, FRAMEWIN_CF_ACTIVE },

	{ ButtonExCtrl_CreateIndirect,	"",		GUI_ID_BUTTON1,		15,	25,	 88, 75, 0},
	{ ButtonExCtrl_CreateIndirect,	"",		GUI_ID_BUTTON2,		15,	110, 88, 75, 0},
	{ ButtonExCtrl_CreateIndirect,	"",		GUI_ID_BUTTON6,		15,	195, 88, 75, 0},

	{ TracingCurve_CreateIndirect,	"",		GUI_ID_CURVE0,		120, 25, 346, 245, 0},

	{ TEXT_CreateIndirect,			"",		GUI_ID_TEXT0,		467, 25,  190, 120}, 
	{ TEXT_CreateIndirect,			"",		GUI_ID_TEXT1,		467, 140, 190, 120},

	{ TEXT_CreateIndirect,			"",		GUI_ID_TEXT2,		467, 230, 30, 40, TEXT_CF_LEFT|TEXT_CF_VCENTER},
	{ TEXT_CreateIndirect,			"",		GUI_ID_TEXT3,		497, 230, 90, 40, TEXT_CF_LEFT|TEXT_CF_VCENTER},

	{ TEXT_CreateIndirect,			"",		GUI_ID_TEXT4,		125, 0, 300,  20, TEXT_CF_LEFT|TEXT_CF_VCENTER}, 

	{ BUTTON_CreateIndirect,		"",		GUI_ID_BUTTON0,     587, 230, 60, 40, 0},
};
static const GUI_WIDGET_CREATE_INFO _aDialogCreateCPB[] =
{
	{ WINDOW_CreateIndirect,	"",		0,					0,   0,  659, 279, FRAMEWIN_CF_ACTIVE },
	{ TEXT_CreateIndirect,		"",		GUI_ID_TEXT0,		130, 0, 580, 210, TEXT_CF_LEFT |TEXT_CF_VCENTER},
	{ BUTTON_CreateIndirect,	"",		GUI_ID_BUTTON0,     587, 230, 60, 40, 0},
};

void initTools(unsigned char Patient_Type)
{
	ToolsPara_t.Time = 60;
	ToolsPara_t.TypeSelect = 0;
	ToolsPara_t.MeasureFlag = 0xAA;

	ToolsPara_t.LungStartFlag = 0;
	ToolsPara_t.LungInterval =  0;
	if (Patient_Type == 0) // Adult
	{
		ToolsPara_t.LungRecPressure = 35;
		ToolsPara_t.LungRecTime = 30;
	}
	else
	{
		ToolsPara_t.LungRecPressure = 20;
		ToolsPara_t.LungRecTime = 15;
	}

	//ToolsPara_t.FlowPauseTime = 60;
	//ToolsPara_t.FlowPauseStartFlag = 0;

	ToolsPara_t.CPBFlag = 0;
}
int Tools_GetTypeSelect(void)
{
	return ToolsPara_t.TypeSelect;
}
int Tools_GetMeasureFlag(void)
{
	return ToolsPara_t.MeasureFlag;
}
int Tools_GetTime(void)
{
	return ToolsPara_t.Time;
}
int LungRec_GetPressure(void)
{
	return ToolsPara_t.LungRecPressure;
}
int LungRec_GetTime(void)
{
	return ToolsPara_t.LungRecTime;
}
int LungRec_GetStartStopFlag(void)
{
	return ToolsPara_t.LungStartFlag;
}
//肺复张倒计时
void LungRecCountDown(void)
{
	WM_MESSAGE Msg;

	if((ToolsPara_t.LungStartFlag == 1) && (MenuType == TOOLS_MENU) && (Menu_hWin.ActivedWin))
	{				
		if(ToolsPara_t.Time > 0) 
		{
			ToolsPara_t.Time--;
			Msg.MsgId = WM_USER;
			Msg.hWin = Menu_hWin.ToolsWin.LungWin;
			WM_SendMessage(Msg.hWin,&Msg);   		
		}
		else
		{
			LungRec_EndDeal();
		}				
	}
	else
	{
		if (ToolsPara_t.LungInterval > 0)
		{
			ToolsPara_t.LungInterval--;
			Msg.MsgId = WM_USER;
			Msg.hWin = Menu_hWin.ToolsWin.LungWin;
			WM_SendMessage(Msg.hWin,&Msg);   				
		}
		else
		{

		}
	}
	if (ToolsPara_t.CurveTime > 0)
	{
		ToolsPara_t.CurveTime--;
	}
}

void LungRec_EndDeal(void)
{
	WM_HWIN hItem;
	if ((ToolsPara_t.LungStartFlag == 1)&& (MenuType == TOOLS_MENU) && (Menu_hWin.ActivedWin))
	{
		ToolsPara_t.LungStartFlag = 0;
		ToolsPara_t.LungInterval = 60;
		ToolsPara_t.MeasureFlag = 0xAA;
		SendOperationMessage(NPLSWITCH_SEND_COM, USART0);//
		//
		SendSetModes(BeathSettingPara.BreathMode,&BeathSettingPara);
		//
		//totalFlow = Old_totalFlow;
		//Flowmeter_Channel_Flag = 1;
		//
		hItem = WM_GetDialogItem(Menu_hWin.ToolsWin.LungWin, GUI_ID_BUTTON0);
		BUTTON_SetText(hItem,Sys_Lib_GetDispStr(DP_START));
		WM_DisableWindow(hItem);
		BUTTON_SetPressed(hItem,0);

		//
		TEXT_SetText(WM_GetDialogItem(Menu_hWin.ToolsWin.LungWin,GUI_ID_TEXT2),Sys_Lib_GetDispStr(DP_EMPTY));
		//
		WM_EnableWindow(WM_GetDialogItem(Menu_hWin.ToolsWin.LungWin,GUI_ID_BUTTON1));
		WM_EnableWindow(WM_GetDialogItem(Menu_hWin.ToolsWin.LungWin,GUI_ID_BUTTON2));
		WM_EnableWindow(WM_GetDialogItem(Menu_hWin.ToolsWin.LungWin,GUI_ID_BUTTON6));
		//
		hItem = WM_GetDialogItem(Menu_hWin.ActivedWin, GUI_ID_MULTIPAGE0);
		MULTIPAGE_SetPageChangeEnable(hItem,1);
		hItem = WM_GetDialogItem(Menu_hWin.ActivedWin, GUI_ID_CLOSE);
		WM_EnableWindow(hItem);
		//
		MenuOperateStatus &= ~FRAMEWIN_POP;
	}
}

void LungRec_CurveDataSave(TracingCurve_Handle hObj,char flag,float pressure)
{
	static float LastPressure = 0;
	float CurrentPressure = (70*RealtimePara.Pressure/10 + 30*LastPressure)/100;  //sun add 20140526
	LastPressure = CurrentPressure;
	//if (((ToolsPara_t.LungStartFlag == 1) || (ToolsPara_t.LungInterval!=0))&&(MenuType == TOOLS_MENU))
	if (ToolsPara_t.CurveTime > 0)
	{
		TracingCurve_SetADDData(hObj,flag,GUI_RED,CurrentPressure, 0,WAVEFORM_PRESSURE,0); //CurrentPressure/10 //cpp md 20130325
	}
}
//void LungRec_CurveDataUpdate(TracingCurve_Handle hObj)
//{
//	//if (((ToolsPara_t.LungStartFlag == 1) || (ToolsPara_t.LungInterval!=0))&&(MenuType == TOOLS_MENU))
//	if (ToolsPara_t.CurveTime > 0)
//	{
//		//TracingCurve_DrawBuffer(hObj,WAVEFORM_HOLD_PRESSURE);
//	}
//
//}
//肺复张

void ButtonExCtrl_LeftValueUpdate(WM_HWIN hWin)
{
	WM_MESSAGE Msg;
	Msg.MsgId = WM_NOTIFICATION_VALUE_CHANGED;
	Msg.Data.v = GUI_KEY_LEFT;
	Msg.hWin = hWin;
	WM_SendMessage(hWin,&Msg);
}
void ButtonExCtrl_RightValueUpdate(WM_HWIN hWin)
{
	WM_MESSAGE Msg;
	Msg.MsgId = WM_NOTIFICATION_VALUE_CHANGED;
	Msg.Data.v = GUI_KEY_RIGHT;
	Msg.hWin = hWin;
	WM_SendMessage(hWin,&Msg);
}


static void _cbDialogLungRec(WM_MESSAGE * pMsg)
{
	WM_HWIN hDlg, hItem, hPmt0, hPmt1, hPmt_btn, hHoldtime, hWaittime;
	WM_HWIN hButton0, hButton1, hButton2, hButton3, hCurve;
	int Data,Flag = 1;
	int Id,NCode;
	char pText[30];
	ButtonExCtrl_Obj* pObj;
	hDlg = pMsg->hWin;
	hHoldtime = WM_GetDialogItem(hDlg,GUI_ID_TEXT2); // 保持时间，点击start按钮，显示。时间结束，隐藏
	hWaittime = WM_GetDialogItem(hDlg,GUI_ID_TEXT3); // 等待时间
	hPmt_btn  = WM_GetDialogItem(hDlg,GUI_ID_TEXT4); // 参数调节提示信息

	switch(pMsg->MsgId)
	{
	case WM_INIT_DIALOG:
		WINDOW_SetBkColor(hDlg, 0xCDAA94);
		hButton0 = WM_GetDialogItem(hDlg,GUI_ID_BUTTON0);
		BUTTON_SetText(hButton0, Sys_Lib_GetDispStr(DP_START));
		BUTTON_SetFont(hButton0, &GUI_Font_24);
		BUTTON_SetReleased(hButton0,1);

		if (BeathSettingPara.SettingPEEPValue > ToolsPara_t.LungRecPressure - 5)
		{
			ToolsPara_t.LungRecPressure = BeathSettingPara.SettingPEEPValue + 5;
		}
		hButton1 = WM_GetDialogItem(hDlg,GUI_ID_BUTTON1);
		ButtonExCtrl_SetTriangleSign(hButton1,0);
		ButtonExCtrl_SetFontEx(hButton1,&GUI_Font_15,2);
		ButtonExCtrl_SetFontInfo(hButton1,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_HOLDPRESSURE),Sys_Lib_GetDispStr(DP_CMH2O));
		ButtonExCtrl_SetButtonDataInfo(hButton1,20,60,ToolsPara_t.LungRecPressure,1,0);
		ButtonExCtrl_SetData(hButton1,ToolsPara_t.LungRecPressure);
		ButtonExCtrl_SetEdgeColor(hButton1,0xCDAA94);

		hButton2 = WM_GetDialogItem(hDlg,GUI_ID_BUTTON2);
		ButtonExCtrl_SetTriangleSign(hButton2,0);
		ButtonExCtrl_SetFontEx(hButton2,&GUI_Font_15,2);
		ButtonExCtrl_SetFontInfo(hButton2,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_HOLDTIME),Sys_Lib_GetDispStr(DP_S));
		ButtonExCtrl_SetButtonDataInfo(hButton2,10,40,ToolsPara_t.LungRecTime,1,0);
		ButtonExCtrl_SetData(hButton2,ToolsPara_t.LungRecTime);
		ButtonExCtrl_SetEdgeColor(hButton2,0xCDAA94);

		hButton3 = WM_GetDialogItem(hDlg,GUI_ID_BUTTON6);
		ButtonExCtrl_SetTriangleSign(hButton3,0);
		ButtonExCtrl_SetFontEx(hButton3,&GUI_Font_15,2);
		ButtonExCtrl_SetFontInfo(hButton3,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_PEEP),Sys_Lib_GetDispStr(DP_CMH2O));
		ButtonExCtrl_SetButtonDataInfo(hButton3,0,50,BeathSettingPara.SettingPEEPValue,1,0); // OFF
		ButtonExCtrl_SetData(hButton3,BeathSettingPara.SettingPEEPValue);
		ButtonExCtrl_SetEdgeColor(hButton3,0xCDAA94);

		hPmt0 = WM_GetDialogItem(hDlg,GUI_ID_TEXT0); // 提示信息1
		//TEXT_SetText(hPmt0, "1.If the patient\'s physiological state is abnormal during the process of lung recruitment,please immediately terminate the recruitment process.");
		TEXT_SetText(hPmt0, Sys_Lib_GetDispStr(DP_LUNGRECPROMPT1));
		TEXT_SetFont(hPmt0,&GUI_Font_16);
		TEXT_SetTextWrapMode(hPmt0, GUI_WRAPMODE_WORD);

		hPmt1 = WM_GetDialogItem(hDlg,GUI_ID_TEXT1); // 提示信息2
		//TEXT_SetText(hPmt1, "2.After the lung recruitment function is completed,please wait for one minute before using this function.");
		TEXT_SetText(hPmt1, Sys_Lib_GetDispStr(DP_LUNGRECPROMPT2));
		TEXT_SetFont(hPmt1,&GUI_Font_16);
		TEXT_SetTextWrapMode(hPmt1, GUI_WRAPMODE_WORD);
		
		TEXT_SetText(hHoldtime, "");
		TEXT_SetFont(hHoldtime,&GUI_Font_20);
		//TEXT_SetBkColor(hHoldtime, GUI_RED);

		TEXT_SetText(hWaittime, "");
		TEXT_SetFont(hWaittime,&GUI_Font_20);
		//TEXT_SetBkColor(hWaittime, GUI_GREEN);

		TEXT_SetText(hPmt_btn, ""); 
		TEXT_SetFont(hPmt_btn, &GUI_Font_20);
		//TEXT_SetBkColor(hPmt_btn, GUI_RED);

		if (ToolsPara_t.LungInterval <=0)
		{
			WM_HideWindow(hWaittime);
		}
		else
		{
			WM_ShowWindow(hWaittime);
		}
		switch (BeathSettingPara.BreathMode)
		{
		case MANUAL_MODE:
		case ACGO_MODE:
		case SIMV_VCV_MODE:
		case SIMV_PCV_MODE:
		case SIMV_PRVC_MODE:
		case PSV_MODE:
			WM_DisableWindow(hButton0);
			// 			WM_DisableWindow(hButton1);
			// 			WM_DisableWindow(hButton2);
			// 			WM_DisableWindow(hButton3);
			//TEXT_SetText(hText3,"此模式下此功能不可用！");
			break;
		default:
			//if (ToolsPara_t.FlowPauseStartFlag)
			//{
			//	WM_DisableWindow(hButton0);
			//}
			//else
			{
				if (ToolsPara_t.LungInterval > 0)
				{
					WM_DisableWindow(hButton0);
				}
				else
				{
					WM_EnableWindow(hButton0);
				}
			}
			break;
		}

		hCurve = WM_GetDialogItem(hDlg,GUI_ID_CURVE0);
		//TracingCurve_SetStopRemove(hCurve,1);
 		TracingCurve_SetRange(hCurve,(ToolsPara_t.LungRecPressure/5+1));
 		TracingCurve_SetViewMode(hCurve,0,'F');
 		TracingCurve_SetXYINFO(hCurve,'T',0,60,5,GUI_WHITE,Sys_Lib_GetDispStr(DP_S),-1,5,1,GUI_WHITE,
			Sys_Lib_GetDispStr(DP_CMH2O),GUI_WHITE,Sys_Lib_GetDispStr(DP_PAW));
 		//TracingCurve_SetVigilanceLine(hCurve,'T',GUI_WHITE,20); // ?
		//TracingCurve_SetBKColor(hCurve, GUI_RED);

		break;
	case WM_NOTIFY_PARENT:
		NCode = pMsg->Data.v;
		Id = WM_GetId(pMsg->hWinSrc);
		switch(NCode)
		{
		case WM_NOTIFICATION_LOST_FOCUS:
			break;
		case WM_NOTIFICATION_CLICKED:
		case GUI_KEY_ENTER:
			switch(Id)
			{
			case GUI_ID_BUTTON0:
				hButton0 = pMsg->hWinSrc;
				ToolsPara_t.TypeSelect = 0x03;
				if (ToolsPara_t.LungStartFlag == 0)
				{
					//启动时，判断当前设置的保持压力与上一呼吸周期峰值压力，取大值
					if (ToolsPara_t.LungRecPressure < MonitorPara.PpeakValue/10)
					{
						ToolsPara_t.LungRecPressure = MonitorPara.PpeakValue/10;
					}
					TracingCurve_SetRange(WM_GetDialogItem(hDlg,GUI_ID_CURVE0),(ToolsPara_t.LungRecPressure/5+1));
					//
					ToolsPara_t.CurveTime = 60;
					ToolsPara_t.LungStartFlag = 1;
					ToolsPara_t.Time = ToolsPara_t.LungRecTime+3;
					ToolsPara_t.MeasureFlag = 0x55;
					SendOperationMessage(LUNGPAWTIME_SEND_COM, USART0);		
					SendOperationMessage(NPLSWITCH_SEND_COM, USART0);

					//
					//新鲜气体流量置零
					//Old_totalFlow = totalFlow;
					//totalFlow = 0;
					//Flowmeter_Channel_Flag = 1;
					//
					BUTTON_SetPressed(hButton0,1);
					BUTTON_SetText(hButton0,Sys_Lib_GetDispStr(DP_STOP));
					//其他按钮不可选
					WM_DisableWindow(WM_GetDialogItem(hDlg,GUI_ID_BUTTON1));
					WM_DisableWindow(WM_GetDialogItem(hDlg,GUI_ID_BUTTON2));
					WM_DisableWindow(WM_GetDialogItem(hDlg,GUI_ID_BUTTON6));
					//
					hItem = WM_GetDialogItem(Menu_hWin.ActivedWin, GUI_ID_MULTIPAGE0);
					MULTIPAGE_SetPageChangeEnable(hItem,0);
					//hItem = WM_GetDialogItem(Menu_hWin.ActivedWin, GUI_ID_CLOSE);
					hItem = WM_GetDialogItem(Menu_hWin.ActivedWin, GUI_ID_CLOSE);
					WM_DisableWindow(hItem);

					//
					MenuOperateStatus |= FRAMEWIN_POP;
				}
				else
				{
					ToolsPara_t.Time = 0;
					//tracingcurve_
					LungRec_EndDeal();
				}
				break;
			case GUI_ID_BUTTON1:
			case GUI_ID_BUTTON2:
			case GUI_ID_BUTTON6:
				hItem = pMsg->hWinSrc;
				if (ButtonExCtrl_GetTriangleSign(hItem))
				{
					WM_SelectWindow(hItem);
					ButtonExCtrl_SetTriangleSign(hItem,0);
					Data = ButtonExCtrl_GetData1(hItem);
					ButtonExCtrl_SetData(hItem,Data);
					//SendToolsSetParameter(Data,Id);
					if (Id == GUI_ID_BUTTON1)
					{
						ToolsPara_t.LungRecPressure = Data;

						// 						SendOperationMessage(LUNGPAWTIME_SEND_COM, USART0);			
					}
					else if (Id == GUI_ID_BUTTON2)
					{
						ToolsPara_t.LungRecTime = Data;
						// 						SendOperationMessage(LUNGPAWTIME_SEND_COM, USART0);			
					}
					else
					{
						BeathSettingPara.SettingPEEPValue = Data;
						SendSetParameter(Data,Id);
					}
				}
				else
				{
					//TEXT_SetText(WM_GetDialogItem(hDlg,GUI_ID_TEXT2),"");
					hItem = WM_GetDialogItem(hDlg,GUI_ID_BUTTON1);
					while(!ButtonExCtrl_GetTriangleSign(hItem))
					{
						hItem = WM_GetNextSibling(hItem);
						if((hItem == 0) || (WM_GetId(hItem) == GUI_ID_BUTTON0))
						{
							Flag = 0;
							break;
						}
					}
					if (Flag)
					{
						ButtonExCtrl_SetTriangleSign(hItem,0);
					}
					hItem = pMsg->hWinSrc;
					WM_SetFocus(hItem);
					ButtonExCtrl_SetTriangleSign(hItem,1);
					Data = ButtonExCtrl_GetData(hItem);
					ButtonExCtrl_SetData(hItem,Data);
				}

				break;				
			}
			break;
		case GUI_KEY_LEFT:
			if (ButtonExCtrl_GetTriangleSign(pMsg->hWinSrc))
			{
				pObj = ButtonExCtrl_H2P(pMsg->hWinSrc);
				switch (Id)
				{
				case GUI_ID_BUTTON1://pressure
					if (pObj->Data1 - pObj->BDInfo.Step < BeathSettingPara.SettingPEEPValue + 5)
					{
						TEXT_SetText(hPmt_btn, Sys_Lib_GetDispStr(DP_HOLDPPEEP5));
					}
					else
					{
						pObj->Data1 -= pObj->BDInfo.Step;
						TEXT_SetText(hPmt_btn, "");
						ButtonExCtrl_LeftValueUpdate(pMsg->hWinSrc);
					}
					break;
				case GUI_ID_BUTTON2:
					if (pObj->Data1 - pObj->BDInfo.Step < pObj->BDInfo.Min)
					{
						//提示：
					}
					else
					{
						pObj->Data1 -= pObj->BDInfo.Step;
						TEXT_SetText(hPmt_btn, "");
						ButtonExCtrl_LeftValueUpdate(pMsg->hWinSrc);
					}
					break;
				case GUI_ID_BUTTON6://PEEP
					if (pObj->Data1 < 3)
					{
						pObj->Data1 = 0;
					}
					if (pObj->Data1 - pObj->BDInfo.Step < pObj->BDInfo.Min)
					{
// 						TEXT_SetText(hPmt_btn, "PEEP OFF");
					}
					else
					{
						pObj->Data1 -= pObj->BDInfo.Step;
						TEXT_SetText(hPmt_btn, "");
						ButtonExCtrl_LeftValueUpdate(pMsg->hWinSrc);
					}
					break;
				}
			}
			break;
		case GUI_KEY_RIGHT:
			if (ButtonExCtrl_GetTriangleSign(pMsg->hWinSrc))
			{
				pObj = ButtonExCtrl_H2P(pMsg->hWinSrc);
				switch (Id)
				{
				case GUI_ID_BUTTON1:
					if (pObj->Data1 + pObj->BDInfo.Step > pObj->BDInfo.Max)
					{
						//提示：保持压力小于
					}
					else
					{
						pObj->Data1 += pObj->BDInfo.Step;
						TEXT_SetText(hPmt_btn, "");
						ButtonExCtrl_RightValueUpdate(pMsg->hWinSrc);
					}
					break;
				case GUI_ID_BUTTON2:
					if (pObj->Data1 + pObj->BDInfo.Step > pObj->BDInfo.Max)
					{
						//提示：
					}
					else
					{
						pObj->Data1 += pObj->BDInfo.Step;
						TEXT_SetText(hPmt_btn,"");
						ButtonExCtrl_RightValueUpdate(pMsg->hWinSrc);
					}
					break;
				case GUI_ID_BUTTON6://PEEP
					if (pObj->Data1 < 3)
					{
						pObj->Data1 = 2;
					}
					if (pObj->Data1 + pObj->BDInfo.Step > pObj->BDInfo.Max)
					{
						//TEXT_SetText(hText3,cbdialog32string[PEEP_30]);	
						//if(AirwayPressureUnit)----------------------
						//{
						//	TEXT_SetText(hText3,Sys_Lib_GetDispStr(DP_PEEPLESS30MBAR));
						//}
						//else
						//{
						//	TEXT_SetText(hText3,Sys_Lib_GetDispStr(DP_PEEPLESS30CMH2O));
						//}
					}
					else if (pObj->Data1 + pObj->BDInfo.Step + 5 > ToolsPara_t.LungRecPressure)
					{
						TEXT_SetText(hPmt_btn, Sys_Lib_GetDispStr(DP_HOLDPPEEP5));
					}
					else
					{
						pObj->Data1 += pObj->BDInfo.Step;
						TEXT_SetText(hPmt_btn,"");
						ButtonExCtrl_RightValueUpdate(pMsg->hWinSrc);
					}
					break;
				}
			}
			break;
		}
		break;
	case WM_USER:
		if (ToolsPara_t.LungStartFlag == 1)
		{
			sprintf(pText, "%2d s",ToolsPara_t.Time);
			TEXT_SetText(hHoldtime,pText);
		}
		else
		{
			if (ToolsPara_t.LungInterval >1)
			{
				sprintf(pText, "%s %2d s",Sys_Lib_GetDispStr(DP_WAIT),ToolsPara_t.LungInterval);
				TEXT_SetText(hWaittime,pText);
				if (!WM_IsVisible(hWaittime))
				{
					WM_ShowWindow(hWaittime);
				}
			}
			else
			{
				WM_EnableWindow(WM_GetDialogItem(hDlg,GUI_ID_BUTTON0));
				TEXT_SetText(hWaittime,Sys_Lib_GetDispStr(DP_EMPTY));
				if (WM_IsVisible(hWaittime))
				{
					WM_HideWindow(hWaittime);
				}
			}
		}

		break;
	case WM_NOTIFY_PARENT_REFLECTION:
		if (!(MenuOperateStatus&FRAMEWIN_POP))
		{
			FunctionKeyPressedResume(Menu_hWin.hWin,0);
			PlanMenuType = MAIN_MENU;
			Task |= MEAU_UPDATE;
		}
		break;
	case WM_DELETE:
		ToolsPara_t.CurveTime = 0;
		break;

	default:
		WM_DefaultProc(pMsg);
	}
}

int CPB_GetFlag(void)
{
	return ToolsPara_t.CPBFlag;
}
//CPB
static void _cbDialogCPB(WM_MESSAGE * pMsg)
{
	WM_HWIN hDlg, hItem;
	WM_HWIN hButton;
	//	WM_MESSAGE Msg;
	int Id,NCode;
	char pText[100];
	hDlg = pMsg->hWin;
	switch(pMsg->MsgId)
	{
	case WM_INIT_DIALOG:
		WINDOW_SetBkColor(hDlg, 0xCDAA94);
		hButton = WM_GetDialogItem(hDlg,GUI_ID_BUTTON0);
		//BUTTON_SetStyle(hButton,15);
		//BUTTON_SetBkColor(hButton,0,GUI_RED);
		//BUTTON_SetBkColor(hButton,3,GUI_BLUE);
		BUTTON_SetFont(hButton,&GUI_Font_24);
		BUTTON_SetPressed(hButton,0);
		if (ToolsPara_t.CPBFlag == 0)
		{
			BUTTON_SetText(hButton,Sys_Lib_GetDispStr(DP_START));
			//BUTTON_SetPressed(hButton,0);
		}
		else
		{
			BUTTON_SetText(hButton,Sys_Lib_GetDispStr(DP_STOP));
			//BUTTON_SetPressed(hButton,1);
		}

		hItem = WM_GetDialogItem(hDlg,GUI_ID_TEXT0);
		sprintf(pText,"%s\n\n%s",Sys_Lib_GetDispStr(DP_CPBPROMPT),Sys_Lib_GetDispStr(DP_CPBPROMPT2));
		TEXT_SetText(hItem ,pText);
		TEXT_SetTextAlign(hItem ,GUI_TA_LEFT|GUI_TA_VCENTER);
		TEXT_SetTextWrapMode(hItem,GUI_WRAPMODE_WORD);
		TEXT_SetFont(hItem, &GUI_Font_24);
		break;
	case WM_NOTIFY_PARENT:
		NCode = pMsg->Data.v;
		Id = WM_GetId(pMsg->hWinSrc);
		switch(NCode)
		{
		case WM_NOTIFICATION_CLICKED:
		case GUI_KEY_ENTER:
			if (Id == GUI_ID_BUTTON0)
			{
				hButton = pMsg->hWinSrc;
				if (ToolsPara_t.CPBFlag == 0)
				{
					BUTTON_SetPressed(hButton,0);
					ToolsPara_t.CPBFlag = 1;
					BUTTON_SetText(hButton,Sys_Lib_GetDispStr(DP_STOP));
					PlanMenuType = CPBINFO_MENU;
					Task |= MEAU_UPDATE;
				}
				else
				{
					BUTTON_SetPressed(hButton,0);
					ToolsPara_t.CPBFlag = 0;
					BUTTON_SetText(hButton,Sys_Lib_GetDispStr(DP_START));
				}
				Menu_hWin.InfoWin.InfoChangeFlag |= CPB_CHANGE_FLAG;	
			}
			break;
			// 		case WM_NOTIFICATION_RELEASED:
			// 			if ((Id == GUI_ID_BUTTON0) && (BUTTON_IsPressed(pMsg->hWinSrc)))
			// 			{
			// 				BUTTON_SetPressed(pMsg->hWinSrc,0);
			// 			}
			// 			break;
		}
		break;
	default:
		WM_DefaultProc(pMsg);
		break;
	}
}

/******************************************************************************************************************************************
*
*
*******************************************************************************************************************************************/

WM_HWIN _ToolsHandleWin(void)
{
	WM_HWIN hItem,hWin1,hWin2,hWin3,hWin4;

	hWin1 = FRAMEWIN_Create("",0,WM_CF_SHOW,3,   146-80+7, 642+37, 250+80-7);//调整窗口位置
	hWin2 = MULTIPAGE_CreateEx(8, 10, 625+37, 230+80-7, hWin1, WM_CF_SHOW, 0, GUI_ID_MULTIPAGE0);

	hWin3 = GUI_CreateDialogBox(_aDialogCreateLungRec,
		GUI_COUNTOF(_aDialogCreateLungRec),
		&_cbDialogLungRec, 0, 0, 0);
	hWin4 = GUI_CreateDialogBox(_aDialogCreateCPB,
		GUI_COUNTOF(_aDialogCreateCPB),
		&_cbDialogCPB, 0, 0, 0);


	MULTIPAGE_AddPage(hWin2, hWin3, Sys_Lib_GetDispStr(DP_LUNGRECRUITMENT));
	MULTIPAGE_AddPage(hWin2, hWin4, Sys_Lib_GetDispStr(DP_CPB));
	MULTIPAGE_SelectPage(hWin2,0);

	MULTIPAGE_SelectPage(hWin2,0);
	FRAMEWIN_AddCloseButton(hWin1,0,5);

	hItem = WM_GetDialogItem(hWin3,GUI_ID_CURVE0);
	TracingCurve_AllocDispBuffer(hItem,260/*TRACINGCURVE_DATALEN_DEFAULT+30*/); // ------------
	TracingCurve_SetXScale(hItem,1);

	Menu_hWin.ToolsWin.LungWin = hWin3;
	Menu_hWin.ToolsWin.Tobj[0] = hItem;

	return(hWin1);
}