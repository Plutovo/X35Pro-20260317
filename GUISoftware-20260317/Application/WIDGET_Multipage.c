/*  
****************************************************************************************************
*
* File        : WIDGET_Multipage.c
* Purpose     : Demonstrates the use of a MULTIPAGE widget
*
****************************************************************************************************
*/
#include "WIDGET_Multipage.h"
#include "monitormenu.h"
#include "static_menu_creat.h"
#include "Communication_Handle.h"
#include "MasimoSPO2.h"
#include "JM1_SpO2.h"
#include "DrawEffectLine.h"
#include "FRAMEWIN.h"
#include "GUI_Font_cfg.h"
#include "WIDGET_Patient.h"
#include "PowerComm.h"
#include "Demo_AlarmTest.h"
#include "BubbleCtrl.h"
#include "ModeTabBar.h"
#include "PawGauge.h"

/***************************************************************************************************
*
*        data
* 
****************************************************************************************************
*/
CO2_S CO2_Disp;//djq add 20170526
void CO2_UnitConvert(int Unit);

int BuzzerTestVolume = 0; //  20130402
unsigned char Start_Standby_Flag = 0;
//djq add 20140305
int Barameter_Pressure = -1;   //设置使用地大气压力值   0~999mmHg
int Barameter_Stand_Pressure = -1;   //校验地大气压力值   0~999mmHg
int Barameter_Switch   =  0;   //0 ：大气压传感器开，显示为BDU上传的值   1：关，显示为设置值。
//sun add 20140514程序在线升级
char UpdateSend_Flag = 0;//它的高4位代表是否发送命令：0b1010表示需要发送升级握手命令到相应的下位机，0b1001表示发送完成，0b0000表示不发送。UpdateSend_Flag的低4位表示要发送的命令：从低位到高位依次表示GUI、BDU和PMU。
char UpdateRecv_Flag = 0;//它的高4位代表是否接受完成（或者超时）：0b1010表示正在接受（或者超时），0b1001表示接受完成（如果），0b0000表示没有进入握手程序。
char BDU_UpdateFlag  = 0;//用于发送命令时的数据位，状态为0x55 升级开始；0xaa 升级取消；0 初始状态 
char PMU_UpdateFlag  = 0;//用于发送命令时的数据位，状态为0x55 升级开始；0xaa 升级取消；0 初始状态 
int UpdateProgramTimeOutCnt = 0;//最大超时次数5次
//
extern JM1_SPO2_PARAMETERS SpO2_Parameters;
PARA_SETTING_STATE ParaSettingState;
unsigned char DEMOFLAG = 0;   //控制选配是否通过密码开启  1：直接开启  0：通过密码开启
unsigned char SpO2_Choose = 2;//1：国产   2：Masimo
unsigned char AskTimer = 0;
extern Int8U CHECK_DEBUG;
extern unsigned char AskMenu_Flag;
//
unsigned char alarm_battery_cali = 0;//0xff:电池校验
unsigned char AlarmAirSourceSwitch = 1;//0：off  1：On
unsigned char DirverGasFlag = 0;//0:氧气  1：空气  驱动气体类型标志
//
WM_HWIN MuteFlag = 0, hSpO2, hComp; //sun add 20140519
int hWinCwinFlag = 0;  //sun add 20140526
unsigned char oxyChooseFlag = 0;  //sun add 20140529
extern float MultiArray[4];
extern GUI_COLOR N2OColor;
//**********************************************************************************//
//sun add 20140521
char  contorlText[50] = {0};
const int SighTimesTable[] = {50,75,100,125,150};

unsigned char N2O_Switch = 0; //ylk add 20181128
//extern AA_MODULE_FLAG_REG AA_Module_Flag;

extern unsigned char EtCO2ModuleClear_Flag;

extern unsigned char EtCO2_zhuliu = 1;							//gyh add 20230727
extern unsigned char Masimo_EtCO2_zhuliu = 1;
extern unsigned char Masimo_EtCO2_pangliuco2 = 6;
extern unsigned char Masimo_EtCO2_zhuliuco2 = 4;
extern unsigned char MasimoISA_EtCO2_pangliuO2 = 2;
extern unsigned char MasimoISA_EtCO2_pangliu = 3;

extern void CO2_Reset(void);
extern unsigned char etco2pump_Occlusion;
extern ALARM_SAVEVENT_PARA Alarm_SaveVent_para;
extern Int16U Min_Limit;
extern GUI_CONST_STORAGE GUI_BITMAP bmarrows_right;//箭头
extern GUI_CONST_STORAGE GUI_BITMAP bmarrows_left;//箭头
extern GUI_CONST_STORAGE GUI_BITMAP bmCN_Image1;
extern GUI_CONST_STORAGE GUI_BITMAP bmCN_Image3;
extern GUI_CONST_STORAGE GUI_BITMAP bmEN_Standby;
extern GUI_CONST_STORAGE GUI_BITMAP bmCN_Standby;
extern GUI_CONST_STORAGE GUI_BITMAP bmEN_Standby2;
extern GUI_CONST_STORAGE GUI_BITMAP bmTimer;
extern GUI_CONST_STORAGE GUI_BITMAP bmIconStart;
extern GUI_CONST_STORAGE GUI_BITMAP bmIconStop;

extern void FRAMEWIN_SetBitmap(FRAMEWIN_Handle hObj, const GUI_BITMAP* pBitmap, int x, int y,unsigned char Index);
extern void FRAMEWIN_SetTitleStyle(FRAMEWIN_Handle hObj, int Style);

BREATH_SET_PARA Breath_Setting_Tmp;//ylk add 20181009
unsigned char PresetMode_Flag = 0;//实际模式与预设模式区分标志 实际为0 预设为1
char ModeChange_Count = 0;//ylk add 20181009
ButtonExCtrl_Handle BreathSettingParaHandle[8] = {0};//ylk add 20181009 控制参数按钮句柄
extern float MAC_Base[]; 
extern AA_PARAMETERS AA_Para;
unsigned char AlarmEnterFlag = 0;//报警界面入口标志。    点击按键进入：0   点击报警提示进入：1

unsigned char HeightUnit = 0;//0:cm  1:inch 
unsigned char WeightUnit  = 0;//0:kg  1:Pound

unsigned int FlowHighLimit = 1500;
unsigned int FlowLowLimit = 10000;

extern unsigned int  Vt_HighLimit;//潮气量的上限  开机、系统初始化、病人初始化的时候赋值
extern unsigned int  Vt_LowLimit; //潮气量的下限  开机、系统初始化、病人初始化和修改潮气量选配的时候赋值
extern GUI_CONST_STORAGE GUI_BITMAP bmACGO;

unsigned char WaveFormsId2 = 1;//
//void WaveFromMenu(unsigned Id);

extern float Data_Rounding(float data, int narrow_mul,unsigned char DecDig);

/////Demo/////////////////////////////////////////////////////////////////////
#ifdef DEMO //20221118 lyy add
extern unsigned char Demo_Switch;
extern char Demo_Change_Cycle;
extern char Demo_InitPsv;
extern int Demo_SetR;
extern int Demo_SetC;
#endif

unsigned char PM_Version_Main = 1;
unsigned char PM_Version_Sub = 0;
unsigned char PM_Version_Debug = 0;
unsigned char PM_Version_Test = 0;

extern unsigned char ShutDownAskMenu_Flag;
extern unsigned char SendShutDownState;
extern GUI_CONST_STORAGE GUI_BITMAP bmcurvechoose_rgb;//选择波形按钮
extern GUI_CONST_STORAGE GUI_BITMAP bmIconClear;
/***************************************************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialogs.
* It has been created manually, but could also be created by a GUI-builder.
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate1[] =
{
    { WINDOW_CreateIndirect,    "Dialog 1",   0,                    0,    0,   600,  300, FRAMEWIN_CF_ACTIVE },
  //{ DATA_FRAME_CreateIndirect,        "",   GUI_ID_DATABAR0,      2,    1,   320,  85,  0},
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON3,       174+35,  15,  100,  35,  0},
    { BUTTON_CreateIndirect,    "",           GUI_ID_BUTTON4,       314+35,  15,  100,  35,  0},
    { ButtonRotation_CreateIndirect,    "",   GUI_ID_BUTTON0,       174+35,  80,  70,   90,  0},
    { ButtonRotation_CreateIndirect,    "",   GUI_ID_BUTTON1,       259+35,  80,  70,   90,  0},
    { ButtonRotation_CreateIndirect,    "",   GUI_ID_BUTTON2,       344+35,  80,  70,   90,  0},
  //{ ButtonExCtrl_CreateIndirect,      "",   GUI_ID_BUTTON5,       405,  3,   80,   80,  0},
  //{ ButtonExCtrl_CreateIndirect,      "",   GUI_ID_BUTTON6,       485,  3,   80,   80,  0},
};
//
static const GUI_WIDGET_CREATE_INFO _aDialogCreate2[] =
{
    { WINDOW_CreateIndirect,    "Dialog 2",   0,   0,    0,   625,  350, FRAMEWIN_CF_ACTIVE },
    { TextExCtrl_CreateIndirect, "",GUI_ID_TEXT0,  10,   20,  199,  25,  TEXT_CF_LEFT },
    { TextExCtrl_CreateIndirect, "",GUI_ID_TEXT1,  10,   48,  199,  25,  TEXT_CF_LEFT },
    { TextExCtrl_CreateIndirect, "",GUI_ID_TEXT2,  10,   76,  199,  25,  TEXT_CF_LEFT },
    { TextExCtrl_CreateIndirect, "",GUI_ID_TEXT3,  10,   104, 199,  25,  TEXT_CF_LEFT },
    { TextExCtrl_CreateIndirect, "",GUI_ID_TEXT4,  10,   132, 199,  25,  TEXT_CF_LEFT },
    { TextExCtrl_CreateIndirect, "",GUI_ID_TEXT5,  10,   160, 199,  25,  TEXT_CF_LEFT },
    { TextExCtrl_CreateIndirect, "",GUI_ID_TEXT6,  211,  20,  199,  25,  TEXT_CF_LEFT },
    { TextExCtrl_CreateIndirect, "",GUI_ID_TEXT7,  211,  48,  199,  25,  TEXT_CF_LEFT },
    { TextExCtrl_CreateIndirect, "",GUI_ID_TEXT8,  211,  76,  199,  25,  TEXT_CF_LEFT },
    { TextExCtrl_CreateIndirect, "",GUI_ID_TEXT9,  211,  104, 199,  25,  TEXT_CF_LEFT },
    { TextExCtrl_CreateIndirect, "",GUI_ID_TEXT10, 211,  132, 199,  25,  TEXT_CF_LEFT },
    { TextExCtrl_CreateIndirect, "",GUI_ID_TEXT11, 211,  160, 199,  25,  TEXT_CF_LEFT },
    { TextExCtrl_CreateIndirect, "",GUI_ID_TEXT12, 412,  20,  199,  25,  TEXT_CF_LEFT },
    { TextExCtrl_CreateIndirect, "",GUI_ID_TEXT13, 412,  48,  199,  25,  TEXT_CF_LEFT },
#ifdef SPO2_SWITCH
    { TextExCtrl_CreateIndirect, "",GUI_ID_TEXT14, 412,  76,  199,  25,  TEXT_CF_LEFT },
    { TextExCtrl_CreateIndirect, "",GUI_ID_TEXT15, 412,  104, 199,  25,  TEXT_CF_LEFT },
    { TextExCtrl_CreateIndirect, "",GUI_ID_TEXT16, 412,  132, 199,  25,  TEXT_CF_LEFT },
    //{ TextExCtrl_CreateIndirect, "",GUI_ID_TEXT17, 354,  160, 170,  25,  TEXT_CF_LEFT },
#endif
};

static const GUI_WIDGET_CREATE_INFO _aDialogCreateValues2[] =
{
    { WINDOW_CreateIndirect,    "Dialog Values2",   0,   0,    0,   600,  350, FRAMEWIN_CF_ACTIVE },
    { TextExCtrl_CreateIndirect, "",GUI_ID_TEXT0,  10,   20,  199,  25,  TEXT_CF_LEFT },
    { TextExCtrl_CreateIndirect, "",GUI_ID_TEXT1,  10,   48,  199,  25,  TEXT_CF_LEFT },
    { TextExCtrl_CreateIndirect, "",GUI_ID_TEXT2,  10,   76,  199,  25,  TEXT_CF_LEFT },
    { TextExCtrl_CreateIndirect, "",GUI_ID_TEXT3,  10,   104, 199,  25,  TEXT_CF_LEFT },
    { TextExCtrl_CreateIndirect, "",GUI_ID_TEXT4,  10,   132, 199,  25,  TEXT_CF_LEFT },
    { TextExCtrl_CreateIndirect, "",GUI_ID_TEXT5,  211,  20, 199,  25,  TEXT_CF_LEFT },
    { TextExCtrl_CreateIndirect, "",GUI_ID_TEXT6,  211,  48,  199,  25,  TEXT_CF_LEFT },
    { TextExCtrl_CreateIndirect, "",GUI_ID_TEXT7,  211,  76,  199,  25,  TEXT_CF_LEFT },
    { TextExCtrl_CreateIndirect, "",GUI_ID_TEXT8,  211,  104,  199,  25,  TEXT_CF_LEFT },
};

//
static const GUI_WIDGET_CREATE_INFO _aDialogCreate3[] =
{
    { WINDOW_CreateIndirect,    "Dialog 3", 0,               0,    0,   600, 420,FRAMEWIN_CF_ACTIVE },
    { IMAGETEXT_CreateIndirect, "",      GUI_ID_IMAGETEXT0,  51,   51,  50,  50, 0 },
    { IMAGETEXT_CreateIndirect, "",      GUI_ID_IMAGETEXT1,  51,   131, 50,  50, 0 },
#ifdef LANGUAGE_CHINESE_FLAG
    { TEXT_CreateIndirect,      "",      GUI_ID_TEXT0,       116,   12,   60,  25, TEXT_CF_HCENTER|TEXT_CF_VCENTER },
    { TEXT_CreateIndirect,      "",      GUI_ID_TEXT2,       185,  12,   80,  25, TEXT_CF_HCENTER|TEXT_CF_VCENTER },
    { TEXT_CreateIndirect,      "",      GUI_ID_TEXT4,       270,  12,   80,  25, TEXT_CF_HCENTER|TEXT_CF_VCENTER },
    { TEXT_CreateIndirect,      "",      GUI_ID_TEXT6,       346,  12,   80,  25, TEXT_CF_HCENTER|TEXT_CF_VCENTER },
    { TEXT_CreateIndirect,      "",      GUI_ID_TEXT8,       428,  12,   80,  25, TEXT_CF_HCENTER|TEXT_CF_VCENTER },
    { TEXT_CreateIndirect,      "",      GUI_ID_TEXT10,      510,  12,   90, 25, TEXT_CF_HCENTER|TEXT_CF_VCENTER },
#else
	{ TEXT_CreateIndirect,      "",      GUI_ID_TEXT0,       113,  12,   60,  25, TEXT_CF_HCENTER|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,      "",      GUI_ID_TEXT2,       194,  12,   60,  25, TEXT_CF_HCENTER|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,      "",      GUI_ID_TEXT4,       274,  12,   60,  25, TEXT_CF_HCENTER|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,      "",      GUI_ID_TEXT6,       355,  12,   60,  25, TEXT_CF_HCENTER|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,      "",      GUI_ID_TEXT8,       436,  12,   60,  25, TEXT_CF_HCENTER|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,      "",      GUI_ID_TEXT10,      498-4,  12,   100+10,  25, TEXT_CF_HCENTER|TEXT_CF_VCENTER },
#endif
	{ TEXT_CreateIndirect,      "",   GUI_ID_TEXT1,       126,   175, 40,  25, TEXT_CF_HCENTER|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,      "",   GUI_ID_TEXT3,       209,  175, 40,  25, TEXT_CF_HCENTER|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,      "",   GUI_ID_TEXT5,       299,  175, 20,  25, TEXT_CF_HCENTER|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,      "",   GUI_ID_TEXT7,       379,  175, 20,  25, TEXT_CF_HCENTER|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,      "",   GUI_ID_TEXT9,       459,  175, 20,  25, TEXT_CF_HCENTER|TEXT_CF_VCENTER },
	{ TEXT_CreateIndirect,      "",   GUI_ID_TEXT11,      539,  94,  20,  25, TEXT_CF_HCENTER|TEXT_CF_VCENTER },
    //
    { ButtonRotation_CreateIndirect,    "",   GUI_ID_BUTTON0,      116,   34,  60,  60, 0},
    { ButtonRotation_CreateIndirect,    "",   GUI_ID_BUTTON1,      116,   115, 60,  60, 0},
    { ButtonRotation_CreateIndirect,    "",   GUI_ID_BUTTON2,      196,  34,  60,  60, 0},
    { ButtonRotation_CreateIndirect,    "",   GUI_ID_BUTTON3,      196,  115, 60,  60, 0},
    { ButtonRotation_CreateIndirect,    "",   GUI_ID_BUTTON4,      276,  34,  60,  60, 0},
    { ButtonRotation_CreateIndirect,    "",   GUI_ID_BUTTON5,      276,  115, 60,  60, 0},
    { ButtonRotation_CreateIndirect,    "",   GUI_ID_BUTTON6,      356,  34,  60,  60, 0},
    { ButtonRotation_CreateIndirect,    "",   GUI_ID_BUTTON7,      356,  115, 60,  60, 0},
    { ButtonRotation_CreateIndirect,    "",   GUI_ID_BUTTON8,      436,  34,  60,  60, 0},
    { ButtonRotation_CreateIndirect,    "",   GUI_ID_BUTTON9,      436,  115, 60,  60, 0},
    { ButtonRotation_CreateIndirect,    "",   GUI_ID_BUTTON10,     516,  34,  60,  60, 0},
  //{ ButtonRotation_CreateIndirect,    "",   GUI_ID_BUTTON11,     500,  110, 60,  60, 0},
};
//
static const GUI_WIDGET_CREATE_INFO _aDialogCreate4[] =
{
    { WINDOW_CreateIndirect,         "Dialog 4",     0,                  0,    0,   675,  420, FRAMEWIN_CF_ACTIVE },
	{ TEXT_CreateIndirect,           "",             GUI_ID_TEXT1,       435,  13,  150,  20,  0},
    { DROPDOWN_CreateIndirect,       "",             GUI_ID_DROPDOWN1,   435,  33,  160,  105,  GUI_TA_LEFT|GUI_TA_VCENTER},//语言设置

	{ TEXT_CreateIndirect,			 "",			 GUI_ID_TEXT2,		 7,    13,	150,  20,  0},
	{ DROPDOWN_CreateIndirect,		 "",			 GUI_ID_DROPDOWN2,	 7,    33,  214,  65,  GUI_TA_LEFT|GUI_TA_VCENTER},//触发类型

    { DROPDOWN_CreateIndirect,       "",             GUI_ID_DROPDOWN3,   247,  33,  160,  95,  GUI_TA_LEFT|GUI_TA_VCENTER},//二氧化碳单位
	{ TEXT_CreateIndirect,           "",             GUI_ID_TEXT3,       247,  13,  150,  20,  0},

	{ DROPDOWN_CreateIndirect,       "",             GUI_ID_DROPDOWN0,   520,  33,  90,  150,  GUI_TA_LEFT|GUI_TA_VCENTER},//设置麻醉气体
	{ TEXT_CreateIndirect,           "",             GUI_ID_TEXT0,       520,  13,  150,  20,  0},//

    { ButtonRotation_CreateIndirect, "",             GUI_ID_BUTTON0,     535,  96, 65,   90,  0},//报警音
    { ButtonRotation_CreateIndirect, "",             GUI_ID_BUTTON1,     293,  96, 65,   90,  0},//SpO2
    { ButtonRotation_CreateIndirect, "",             GUI_ID_BUTTON2,     229,  96, 65,   90,  0},//AA
    { ButtonRotation_CreateIndirect, "",             GUI_ID_BUTTON3,     166,  96, 65,   90,  0},//O2

    { ButtonRotation_CreateIndirect, "",             GUI_ID_BUTTON4,     13,  96, 65,   90,  0},//Air
    { ButtonRotation_CreateIndirect, "",             GUI_ID_BUTTON5,     77,  96, 65,   90,  0},//Driver

    { ButtonRotation_CreateIndirect, "",             GUI_ID_BUTTON9,     471,  96, 65,   90,  0},//叹息

    //提示信息
	{ TEXT_CreateIndirect,    "",   GUI_ID_TEXT4,      61,  35,  500,  100, GUI_TA_HCENTER|GUI_TA_VCENTER},//提示信息
	{ BUTTON_CreateIndirect,  "",   GUI_ID_BUTTON6,    60,  160, 245,  35, 0},//取消
	{ BUTTON_CreateIndirect,  "",   GUI_ID_BUTTON7,    315, 160,  245,  35, 0},//确定
	{ BUTTON_CreateIndirect,  "",   GUI_ID_BUTTON8,    60,  160, 500,  35, 0},//确定
	{ BUTTON_CreateIndirect,  "",   GUI_ID_BUTTON10,   364, 109,  97,  30, 0},//Masimo Spo2
    { BUTTON_CreateIndirect,  "",   GUI_ID_BUTTON11,   364, 145,  97,  30, 0},//补偿按钮

	{ TEXT_CreateIndirect,    "",   GUI_ID_TEXT5,      532,  155,  70,  30, GUI_TA_HCENTER|GUI_TA_VCENTER},//Loudness
	{ TEXT_CreateIndirect,    "",   GUI_ID_TEXT6,      71,   155,  80,  30, GUI_TA_HCENTER|GUI_TA_VCENTER},//Driver
};
//
static const GUI_WIDGET_CREATE_INFO _aDialogCreate5[] =
{
    { WINDOW_CreateIndirect,    "Dialog 5",     0,                0,   0,   620,  420, FRAMEWIN_CF_ACTIVE },
    { BUTTON_CreateIndirect,    "",             GUI_ID_BUTTON3,   79,  35,  245,  30,  0},
    { BUTTON_CreateIndirect,    "",             GUI_ID_BUTTON2,   79,  72,  245,  30,  0},
    { BUTTON_CreateIndirect,    "",             GUI_ID_BUTTON5,   79,  109,  245,  30,  0},
    
    { BUTTON_CreateIndirect,    "",             GUI_ID_BUTTON0,   334,  35, 245,  30,  0},  
    { BUTTON_CreateIndirect,    "",             GUI_ID_BUTTON1,   334,  72, 245,  30,  0},//
    { BUTTON_CreateIndirect,    "",             GUI_ID_BUTTON4,   334,  109, 245,  30,  0},//触摸屏校验
     
 
    { PROGBAR_CreateIndirect,   "",             GUI_ID_PROGBAR0,  15,  170, 592,  25,  0},    
};
//
static const GUI_WIDGET_CREATE_INFO _aDialogCreate6[] =
{
	{ WINDOW_CreateIndirect,            "",       0,                 0,    0,   640, 310, FRAMEWIN_CF_ACTIVE },
	//界面拉长(+43)
	{ BUTTON_CreateIndirect,  "",   GUI_ID_BUTTON6,    84,  155,  235,  30, 0},//
	{ BUTTON_CreateIndirect,  "",   GUI_ID_BUTTON7,    339, 155,  235,  30, 0},//
	{ TEXT_CreateIndirect,    "",   GUI_ID_TEXT6,      97,   50,  470,  40, GUI_TA_HCENTER|GUI_TA_VCENTER},//

	{ BUTTON_CreateIndirect,            "",       GUI_ID_BUTTON5,    24,  160, 610,  30,  0},//Apply
	{ ButtonRotation_CreateIndirect,    "",       GUI_ID_BUTTON0,    121,  46,  60,  85,  0},//Year
	{ ButtonRotation_CreateIndirect,    "",       GUI_ID_BUTTON1,    201,  46,  60,  85,  0},//Mouth
	{ ButtonRotation_CreateIndirect,    "",       GUI_ID_BUTTON2,    281,  46,  60,  85,  0},//Day
	{ ButtonRotation_CreateIndirect,    "",       GUI_ID_BUTTON3,    411,  46, 60,  85,  0},//Hour
	{ ButtonRotation_CreateIndirect,    "",       GUI_ID_BUTTON4,    491,  46, 60,  85,  0},//Minute
};
//
static const GUI_WIDGET_CREATE_INFO _aDialogCreate7[] =
{
    { WINDOW_CreateIndirect,     "",    0,                 0,    80, 620, 240, FRAMEWIN_CF_ACTIVE },	
	{ TEXT_CreateIndirect,       "",    GUI_ID_TEXT0,      35,   6,  50,  20,  TEXT_CF_LEFT|TEXT_CF_VCENTER},//active
    { TEXT_CreateIndirect,       "",    GUI_ID_TEXT1,      102,  6,  50,  20,  TEXT_CF_LEFT|TEXT_CF_VCENTER},//Time
    { TEXT_CreateIndirect,       "",    GUI_ID_TEXT2,      241,  6,  50,  20,  TEXT_CF_LEFT|TEXT_CF_VCENTER},//Event
    { TEXT_CreateIndirect,       "",    GUI_ID_TEXT3,      560,  6,  55,  25,  TEXT_CF_LEFT|TEXT_CF_VCENTER},
    { LISTBOX_CreateIndirect,     0,    GUI_ID_LISTBOX0,   35,   26, 550, 164, 0, 100 },
    { SCROLLBAR_CreateIndirect,   0,    GUI_ID_VSCROLL,    590,  26, 30,  164, SCROLLBAR_CF_VERTICAL, 100}
};
//
static const GUI_WIDGET_CREATE_INFO _aDialogCreate8[] =
{
	{ WINDOW_CreateIndirect,    "",		0,		0,    50,  700,  240, FRAMEWIN_CF_ACTIVE },

	{ TEXT_CreateIndirect,      "",     GUI_ID_TEXT19,     78-40,  15, 200,  20, 0},//Software Version  

	{ TEXT_CreateIndirect,      "",		GUI_ID_TEXT3,      78-40,  44-8, 50,  15, 0},//GUI
	{ TEXT_CreateIndirect,      "",		GUI_ID_TEXT4,      142-40,  44-8, 50,  15, 0},//Version

	{ TEXT_CreateIndirect,      "",		GUI_ID_TEXT5,      78-40,  65-8, 50,  15, 0},//BDU
	{ TEXT_CreateIndirect,      "",		GUI_ID_TEXT6,      142-40,  65-8, 50,  15, 0},//Version

	{ TEXT_CreateIndirect,      "",		GUI_ID_TEXT7,      78-40,  86-8, 50,  15, 0},//PMU
	{ TEXT_CreateIndirect,      "",		GUI_ID_TEXT8,      142-40,  86-8, 50,  15, 0},//Version

	{ TEXT_CreateIndirect,      "LANG",		    GUI_ID_TEXT28,      78-40,  107-8, 50,  15, 0},//Lang 
	{ TEXT_CreateIndirect,      "1.00.00",		GUI_ID_TEXT29,      142-40,  107-8, 50,  15, 0},//Version

	{ TEXT_CreateIndirect,      "FONT",		    GUI_ID_TEXT30,      78-40,  128-8, 50,  15, 0},//FONT
	{ TEXT_CreateIndirect,      "1.00.00",		GUI_ID_TEXT31,      142-40,  128-8, 50,  15, 0},//Version

	{ TEXT_CreateIndirect,      "PM",		    GUI_ID_TEXT32,      78-40,  128+6+7, 50,  15, 0},//PM
	{ TEXT_CreateIndirect,      "",		GUI_ID_TEXT33,      142-40,  128+6+7, 50,  15, 0},//Version

	{ TEXT_CreateIndirect,      "",		GUI_ID_TEXT12,     248-40,  15/*23*/, 200,  20, 0},//Gas Supply Pressure:
#if(MACHINE_TYPE == X45 || MACHINE_TYPE == X45C || MACHINE_TYPE == X45A)	
	{ TEXT_CreateIndirect,      "",			GUI_ID_TEXT15,     248-40,  40, 40,  20, 0},//O2
	{ TEXT_CreateIndirect,      "--",		GUI_ID_TEXT13,     308-40,  40, 24,  20, 0},
	{ TEXT_CreateIndirect,      "kPa",		GUI_ID_TEXT14,     342-40,  40, 40,  20, 0},
	{ TEXT_CreateIndirect,      "",			GUI_ID_TEXT16,     248-40,  65, 40+13,  20, 0},//Air
	{ TEXT_CreateIndirect,      "--",		GUI_ID_TEXT17,     308-40,  65, 24,  20, 0},
	{ TEXT_CreateIndirect,      "kPa",		GUI_ID_TEXT18,     342-40,  65, 40,  20, 0},
#else
	{ TEXT_CreateIndirect,      "kPa",		GUI_ID_TEXT14,     348-40,  40, 40,  20, 0},
	{ TEXT_CreateIndirect,      "--",		GUI_ID_TEXT13,     318-40,  40, 24,  20, 0},
	{ TEXT_CreateIndirect,      "",			GUI_ID_TEXT16,     268-40,  65, 40,  20, 0},//Air
	{ TEXT_CreateIndirect,      "--",		GUI_ID_TEXT17,     318-40,  65, 24,  20, 0},
	{ TEXT_CreateIndirect,      "kPa",		GUI_ID_TEXT18,     348-40,  65, 40,  20, 0},
#endif

	{ TEXT_CreateIndirect,      "",		GUI_ID_TEXT9,      248-40, 90, 200, 20, 0},//Barometric Pressure:
    { TEXT_CreateIndirect,      "",		GUI_ID_TEXT10,     248-40, 115, 24, 20, 0},//--
    { TEXT_CreateIndirect,      "mmHg",	GUI_ID_TEXT11,     293-40, 115, 40,  20, 0},//mmHg

	{ TEXT_CreateIndirect,      "",		GUI_ID_TEXT0,      7208,   140, 200,  20, 0},//Run Time:
	{ TEXT_CreateIndirect,      "--",	GUI_ID_TEXT1,	   208,   165, 50,  20, 0},
	{ TEXT_CreateIndirect,      "",		GUI_ID_TEXT2,      253,  165, 80,  20, 0},//Hours

	{ TEXT_CreateIndirect,      "",     GUI_ID_TEXT20,     500,  15, 100,  20, 0},//Option 选配    

// 	{ TEXT_CreateIndirect,      "",     GUI_ID_TEXT21,     448+100,  40, 150,  20, 0},//PRVC模式   
// 	{ TEXT_CreateIndirect,      "",     GUI_ID_TEXT22,     408,  40, 100,  20, 0},//Infant mode 婴儿模式
// 	{ TEXT_CreateIndirect,      "",     GUI_ID_TEXT23,     408,  65, 200,  20, 0},//CO2监测模块   
// 	{ TEXT_CreateIndirect,      "",     GUI_ID_TEXT24,     408,  90, 200,  20, 0},//O2监测模块  
// #ifndef LANGUAGE_CHINESE_FLAG
// 	{ TEXT_CreateIndirect,      "",     GUI_ID_TEXT25,     408,  115, 150,  20, 0},//SIMV-P & PSV
// 	{ TEXT_CreateIndirect,      "",     GUI_ID_TEXT26,     408,  140, 200,  20, 0},//Gas Driver Mode  
// 	{ TEXT_CreateIndirect,      "",     GUI_ID_TEXT27,     408,  165, 150,  20, 0},//SpO2  
// #endif
	{ IMAGETEXT_CreateIndirect,      "",		GUI_ID_IMAGETEXT1, 373,  43,  150,  30, 0},//
	{ IMAGETEXT_CreateIndirect,      "",		GUI_ID_IMAGETEXT0, 373,  73,  150,  30, 0},//
	{ IMAGETEXT_CreateIndirect,      "",		GUI_ID_IMAGETEXT4, 373,  103, 150,  30, 0},//
	{ IMAGETEXT_CreateIndirect,      "",		GUI_ID_IMAGETEXT7, 373,  133, 150,  30, 0},//
	{ IMAGETEXT_CreateIndirect,      "",		GUI_ID_IMAGETEXT8, 373,  163, 150,  30, 0},//
// 	{ IMAGETEXT_CreateIndirect,      "",		GUI_ID_IMAGETEXT6, 373,  193, 150,  30, 0},//
	{ IMAGETEXT_CreateIndirect,      "",		GUI_ID_IMAGETEXT6, 373+150,  43,  150/*15*/,  30, 0},//
	{ IMAGETEXT_CreateIndirect,      "",		GUI_ID_IMAGETEXT5, 373+150,  73/*68*/, 150,  30, 0},//
	{ IMAGETEXT_CreateIndirect,      "",		GUI_ID_IMAGETEXT9, 373+150,  103, 150,  30, 0},//
	{ IMAGETEXT_CreateIndirect,      "",		GUI_ID_IMAGETEXT2, 373+150,  133, 150,  30, 0},//
	{ IMAGETEXT_CreateIndirect,      "",		GUI_ID_IMAGETEXT3, 373+150,  163, 150,  30, 0},//
};



static const GUI_WIDGET_CREATE_INFO _aDialogCreate10[] =
{
	{ WINDOW_CreateIndirect,    "",                 0,                 345,  192, 270, 160, FRAMEWIN_CF_ACTIVE },// 调整窗口位置 
	{ BUTTON_CreateIndirect,    "1",                GUI_ID_BUTTON0,    135,  10,  35,  30,  0},
	{ BUTTON_CreateIndirect,    "2",                GUI_ID_BUTTON1,    175,  10,  35,  30,  0},
	{ BUTTON_CreateIndirect,    "3",                GUI_ID_BUTTON2,    215,  10,  35,  30,  0},
	{ BUTTON_CreateIndirect,    "4",                GUI_ID_BUTTON3,    135,  45,  35,  30,  0},
	{ BUTTON_CreateIndirect,    "5",                GUI_ID_BUTTON4,    175,  45,  35,  30,  0},
	{ BUTTON_CreateIndirect,    "6",                GUI_ID_BUTTON5,    215,  45,  35,  30,  0},
	{ BUTTON_CreateIndirect,    "7",                GUI_ID_BUTTON6,    135,  80,  35,  30,  0},
	{ BUTTON_CreateIndirect,    "8",                GUI_ID_BUTTON7,    175,  80,  35,  30,  0},
	{ BUTTON_CreateIndirect,    "9",                GUI_ID_BUTTON8,    215,  80,  35,  30,  0},
	{ BUTTON_CreateIndirect,    "0",                GUI_ID_BUTTON9,    135,  115, 35,  30,  0},
	{ BUTTON_CreateIndirect,    "Enter",            GUI_ID_BUTTON10,   175,  115, 75,  30,  0},
	{ BUTTON_CreateIndirect,    "Exit",             GUI_ID_BUTTON11,   20,   115, 75,  30,  0},
	{ TEXT_CreateIndirect,      "Input Password:",  GUI_ID_TEXT0,      20,   7,  110, 30,  0},
	{ TEXT_CreateIndirect,      "",                 GUI_ID_TEXT1,      20,   67,  115, 50,  0},
	{ MULTIEDIT_CreateIndirect, "",                 GUI_ID_EDIT0,      20,   38,  80,  25,  0}
};

//
static const GUI_WIDGET_CREATE_INFO _aDialogCreate11[] =
{
    { WINDOW_CreateIndirect,    "",                      0,                 345,  192, 270,  160, FRAMEWIN_CF_ACTIVE },//ylk 调整窗口位置 
    { BUTTON_CreateIndirect,    "Insp Valve Cal",        GUI_ID_BUTTON0,    13,  10,  245,  30,  0},
    { BUTTON_CreateIndirect,    "Exp flow sensor Cal",   GUI_ID_BUTTON1,    13,  47,  245,  33,  0},
    { BUTTON_CreateIndirect,    "Exp Valve Cal",         GUI_ID_BUTTON2,    13,  84,  245,  30,  0},
    { BUTTON_CreateIndirect,    "Exit",                  GUI_ID_BUTTON3,    178, 121, 80,   30,  0},
};

static const GUI_WIDGET_CREATE_INFO _aDialogCreate12[] =
{
#if 0
	{ WINDOW_CreateIndirect,    "",   0,                     345, 192, 285, 160+22, FRAMEWIN_CF_ACTIVE },
	{ BUTTON_CreateIndirect,    "",   GUI_ID_BUTTON0,        20,  120+20, 75,  30,  0},//exit
	{ BUTTON_CreateIndirect,    "",   GUI_ID_BUTTON1,        175, 120+20, 75,  30,  0},//enter

	{ TEXT_CreateIndirect,      "",   GUI_ID_TEXT1,          5,  5,  275, 15,  0},
	{ TEXT_CreateIndirect,      "",   GUI_ID_TEXT0,          5, 20,  275, 120,  0},
#else
	{ WINDOW_CreateIndirect,    "",   0,                     345, 192, 285, 160, FRAMEWIN_CF_ACTIVE },
	{ BUTTON_CreateIndirect,    "",   GUI_ID_BUTTON0,        20,  120, 75,  30,  0},//exit
	{ BUTTON_CreateIndirect,    "",   GUI_ID_BUTTON1,        175, 120, 75,  30,  0},//enter

// 	{ TEXT_CreateIndirect,      "",   GUI_ID_TEXT1,          5,  5,  275, 15,  0},
	{ TEXT_CreateIndirect,      "",   GUI_ID_TEXT0,          5, 5,  275, 100,  0},
#endif
};

static const GUI_WIDGET_CREATE_INFO _aDialogCreate14[] = //sun add 20140516
{
// 	{ FRAMEWIN_CreateIndirect,    "", 0,                  3,    42,  679, 353,  FRAMEWIN_CF_ACTIVE},
	{ FRAMEWIN_CreateIndirect,    "", 0,                  0,    66,  888, 534,  FRAMEWIN_CF_ACTIVE},
	//{ IMAGETEXT_CreateIndirect,   "", GUI_ID_IMAGETEXT0,  140, 40,  360,  50,   0},
	//{ IMAGETEXT_CreateIndirect,   "", GUI_ID_IMAGETEXT1,  223,  140, 220,  50,   0},
	//{ TEXT_CreateIndirect,        "", GUI_ID_TEXT0,       343,  30,  70,   70,   0},
	{ TEXT_CreateIndirect,        "", GUI_ID_TEXT0,       0,  60,  888,   50,   0},
	//{ BUTTON_CreateIndirect,      "", GUI_ID_BUTTON0,     205,  250, 220,  35,   0},
	{ BUTTON_CreateIndirect,      "", GUI_ID_BUTTON0,     94, 197, 318,  73,   0},//System Test
	{ BUTTON_CreateIndirect,      "", GUI_ID_BUTTON1,     495, 197, 318,  73,   0},//Patient
};

//
static const GUI_WIDGET_CREATE_INFO _aDialogCreate15[] =
{
#if (MACHINE_TYPE == X6 || MACHINE_TYPE == Z6)
    { WINDOW_CreateIndirect,    "Waveform", 0,            170,  60, 160, 70, FRAMEWIN_CF_ACTIVE},
    { MENU_CreateIndirect,      "",         GUI_ID_MENU0, 0,    0,  160, 70, MENU_CF_VERTICAL},
#else
    { WINDOW_CreateIndirect,    "Waveform", 0,            475,  255, 160, 170, FRAMEWIN_CF_ACTIVE},
    { MENU_CreateIndirect,      "",         GUI_ID_MENU0, 0,    0,  160, 170, MENU_CF_VERTICAL},
#endif
};
//
static const GUI_WIDGET_CREATE_INFO _aDialogCreate16[] =
{
	{ WINDOW_CreateIndirect,     "", GUI_ID_FRAMEWIN,     3,   40, 670, 200, FRAMEWIN_CF_ACTIVE},
	{ BUTTON_CreateIndirect,     "", GUI_ID_BUTTON0,      18, 170, 637, 30,  0},
	{ IMAGETEXT_CreateIndirect,  "", GUI_ID_IMAGETEXT0,   282,  8, 200, 50,  0},
	{ TEXT_CreateIndirect,        "", GUI_ID_TEXT1,       192,  65 ,  290, 1,  0},
	{ TEXT_CreateIndirect,       "", GUI_ID_TEXT0,        136,  70,  400, 30,  0},
	{ IMAGETEXT_CreateIndirect,  "", GUI_ID_IMAGETEXT1,   200,  20, 220, 50,  0},//2013-12-14
};
//
static const GUI_WIDGET_CREATE_INFO _aDialogCreate17[] =
{
    { WINDOW_CreateIndirect,      "",   0,                    2,   260,		671, 247, FRAMEWIN_CF_ACTIVE},
    { BUTTON_CreateIndirect,      "",   GUI_ID_BUTTON2,      61,	18,		125, 30,  0},//VCV
	{ BUTTON_CreateIndirect,      "",   GUI_ID_BUTTON5,      61,	63,		125, 30,  0},//SIMV-V

    { BUTTON_CreateIndirect,      "",   GUI_ID_BUTTON3,     198,	18,		125, 30,  0},//PCV
	{ BUTTON_CreateIndirect,      "",   GUI_ID_BUTTON6,     198,	63,		125, 30,  0},//SIMV-P

	{ BUTTON_CreateIndirect,      "",   GUI_ID_BUTTON4,     335,	18,		125, 30,  0},//PRVC
	{ BUTTON_CreateIndirect,      "",   GUI_ID_BUTTON7,     335,	63,		125, 30,  0},//SIMV-PRVC

	{ CHECKBOX_CreateIndirect,    "",   GUI_ID_CHECK0,	    472,	18,		125, 35,  0},//+sigh
    { BUTTON_CreateIndirect,      "",   GUI_ID_BUTTON8,     472,	63,		125, 30,  0},//PSV
 
	{ BUTTON_CreateIndirect,      "",   GUI_ID_BUTTON1,     198, 152,  125, 40,  0},//Cancel
    { BUTTON_CreateIndirect,      "",   GUI_ID_BUTTON0,     335, 152,  125, 40,  0},//Confirm

	{ BUTTON_CreateIndirect,      "",   GUI_ID_BUTTON10,     61,  108, 125, 30, 0},
	{ BUTTON_CreateIndirect,      "",   GUI_ID_BUTTON11,     198, 108, 125, 30, 0},
	{ BUTTON_CreateIndirect,      "",   GUI_ID_BUTTON12,     335, 108, 125, 30, 0},


	{ TEXT_CreateIndirect,        "",   GUI_ID_TEXT1,       4,  143,  650, 1,  0},

};

//
static const GUI_WIDGET_CREATE_INFO _aDialogCreate19[] =
{
    { WINDOW_CreateIndirect,         "Dialog 3", 0,                  0,    0,   596, 420, FRAMEWIN_CF_ACTIVE},
    { IMAGETEXT_CreateIndirect,      "",         GUI_ID_IMAGETEXT0,  51,   51,  25,  20,  0},
    { IMAGETEXT_CreateIndirect,      "",         GUI_ID_IMAGETEXT1,  51,   131, 25,  20,  0},
#ifdef LANGUAGE_CHINESE_FLAG
    { TEXT_CreateIndirect,           "",         GUI_ID_TEXT0,       122,  12,  60,  25,  TEXT_CF_HCENTER|TEXT_CF_VCENTER},
    { TEXT_CreateIndirect,           "",         GUI_ID_TEXT2,       195,  12,  60,  25,  TEXT_CF_HCENTER|TEXT_CF_VCENTER},
    { TEXT_CreateIndirect,           "",         GUI_ID_TEXT4,       270,  12,  60,  25,  TEXT_CF_HCENTER|TEXT_CF_VCENTER},   
	//AA
    { TEXT_CreateIndirect,           "",         GUI_ID_TEXT6,       351,  12,  80,  25,  TEXT_CF_HCENTER|TEXT_CF_VCENTER},
	{ TEXT_CreateIndirect,           "",         GUI_ID_TEXT8,       435,  12,  80,  25,  TEXT_CF_HCENTER|TEXT_CF_VCENTER},   
#else
    { TEXT_CreateIndirect,           "",         GUI_ID_TEXT0,       119,  12,  60,  25,  TEXT_CF_HCENTER|TEXT_CF_VCENTER},
    { TEXT_CreateIndirect,           "",         GUI_ID_TEXT2,       197,  12,  60,  25,  TEXT_CF_HCENTER|TEXT_CF_VCENTER},
    { TEXT_CreateIndirect,           "",         GUI_ID_TEXT4,       273,  12,  60,  25,  TEXT_CF_HCENTER|TEXT_CF_VCENTER},   
	//AA
    { TEXT_CreateIndirect,           "",         GUI_ID_TEXT6,       348,  12,  80,  25,  TEXT_CF_HCENTER|TEXT_CF_VCENTER},
	{ TEXT_CreateIndirect,           "",         GUI_ID_TEXT8,       432,  12,  80,  25,  TEXT_CF_HCENTER|TEXT_CF_VCENTER},   
#endif
	{ TEXT_CreateIndirect,           "mmHg",     GUI_ID_TEXT1,       126,  177, 40,  20,  TEXT_CF_HCENTER|TEXT_CF_VCENTER},
	{ TEXT_CreateIndirect,           "mmHg",     GUI_ID_TEXT3,       207,  177, 40,  20,  TEXT_CF_HCENTER|TEXT_CF_VCENTER},
	{ TEXT_CreateIndirect,           "",         GUI_ID_TEXT5,        299,  177, 10,  20,  TEXT_CF_HCENTER|TEXT_CF_VCENTER},
	{ TEXT_CreateIndirect,           "",         GUI_ID_TEXT7,        379,  177, 10,  20,  TEXT_CF_HCENTER|TEXT_CF_VCENTER},
	{ TEXT_CreateIndirect,           "",         GUI_ID_TEXT9,        459,  177, 10,  20,  TEXT_CF_HCENTER|TEXT_CF_VCENTER},

    { ButtonRotation_CreateIndirect, "",         GUI_ID_BUTTON0,     116,   34,  60,  60,  0},
    { ButtonRotation_CreateIndirect, "",         GUI_ID_BUTTON1,     116,   115, 60,  60,  0},
    { ButtonRotation_CreateIndirect, "",         GUI_ID_BUTTON2,     196,  34,  60,  60,  0},
    { ButtonRotation_CreateIndirect, "",         GUI_ID_BUTTON3,     196,  115, 60,  60,  0},
    { ButtonRotation_CreateIndirect, "",         GUI_ID_BUTTON4,     276,  34,  60,  60,  0},
    { ButtonRotation_CreateIndirect, "",         GUI_ID_BUTTON5,     276,  115, 60,  60,  0},
	//AA
    { ButtonRotation_CreateIndirect, "",         GUI_ID_BUTTON6,     356,  34,  60,  60,  0},
    { ButtonRotation_CreateIndirect, "",         GUI_ID_BUTTON7,     356,  115, 60,  60,  0},
	{ ButtonRotation_CreateIndirect, "",         GUI_ID_BUTTON8,     436,  34,  60,  60,  0},
    { ButtonRotation_CreateIndirect, "",         GUI_ID_BUTTON9,     436,  115, 60,  60,  0},
};
static const GUI_WIDGET_CREATE_INFO _aDialogCreate40[] =
{
    { WINDOW_CreateIndirect,         "Dialog 3", 0,                  0,    0,   596, 420, FRAMEWIN_CF_ACTIVE},
    { IMAGETEXT_CreateIndirect,      "",         GUI_ID_IMAGETEXT0,  51,   51,  50,  50,  0},
    { IMAGETEXT_CreateIndirect,      "",         GUI_ID_IMAGETEXT1,  51,   131, 50,  50,  0},
    
    { TEXT_CreateIndirect,           "",         GUI_ID_TEXT0,       114,   12,   60,  25,  TEXT_CF_HCENTER|TEXT_CF_VCENTER},
    { TEXT_CreateIndirect,           "",         GUI_ID_TEXT1,       123,   175, 40,  25,  TEXT_CF_HCENTER|TEXT_CF_VCENTER},
    { TEXT_CreateIndirect,           "",         GUI_ID_TEXT2,       192,  12,   60,  25,  TEXT_CF_HCENTER|TEXT_CF_VCENTER},
    { TEXT_CreateIndirect,           "",         GUI_ID_TEXT3,       202,  175, 40,  25,  TEXT_CF_HCENTER|TEXT_CF_VCENTER},

    { ButtonRotation_CreateIndirect, "",         GUI_ID_BUTTON0,     116,   34,  60,  60,  0},
    { ButtonRotation_CreateIndirect, "",         GUI_ID_BUTTON1,     116,   115, 60,  60,  0},
    { ButtonRotation_CreateIndirect, "",         GUI_ID_BUTTON2,     196,  34,  60,  60,  0},
    { ButtonRotation_CreateIndirect, "",         GUI_ID_BUTTON3,     196,  115, 60,  60,  0},
};
//
//static const GUI_WIDGET_CREATE_INFO _aDialogCreate20[] =
//{
//    { WINDOW_CreateIndirect,      "", 0,                   562, 200, 78, 200, FRAMEWIN_CF_ACTIVE},
//    { BUTTON_CreateIndirect,      "", GUI_ID_BUTTON0,      1,   10,  75, 32,  0},
//    { BUTTON_CreateIndirect,      "", GUI_ID_BUTTON1,      1,   55,  75, 32,  0},
//    { BUTTON_CreateIndirect,      "", GUI_ID_BUTTON2,      1,   100, 75, 32,  0},
//    { BUTTON_CreateIndirect,      "", GUI_ID_BUTTON3,      1,   145, 75, 32,  0},
//};
#if 1
static const GUI_WIDGET_CREATE_INFO _aDialogCreate20[] =  //sun add 20140514
{
	{ WINDOW_CreateIndirect,      "", 0,                   888,	 67,  136, 533, FRAMEWIN_CF_ACTIVE},

	{ BUTTON_CreateIndirect,      "", GUI_ID_BUTTON4,      0,    0,   136, 58,  0},//Alarm Silence

	//{ BUTTON_CreateIndirect,      "", GUI_ID_BUTTON0,      0,    74,  136, 68,  0},//Modes

	{ BUTTON_CreateIndirect,      "", GUI_ID_BUTTON0,      0,    61,  136, 58,  0},//Alarms
	{ BUTTON_CreateIndirect,      "", GUI_ID_BUTTON3,      0,    122, 136, 58,  0},//Monitoring
	{ BUTTON_CreateIndirect,      "", GUI_ID_BUTTON6,      0,    183, 136, 58,  0},//Trends
	{ BUTTON_CreateIndirect,      "", GUI_ID_BUTTON8,      0,    244, 136, 58,  0},//Tools
	{ BUTTON_CreateIndirect,      "", GUI_ID_BUTTON2,      0,    305, 136, 58,  0},//System
	{ BUTTON_CreateIndirect,      "", GUI_ID_BUTTON7,      0,    465, 136, 68,  0},//Standby

	// 计时器
	{ IMAGETEXT_CreateIndirect, "",	GUI_ID_TEXT4,    2,  385-20, 128, 117-22, 0}, // Rect
	{ MultiText_CreateIndirect, "", GUI_ID_TEXT0,    4,  429-60, 124, 43,  0}, // 计时 
	{ MultiText_CreateIndirect, "", GUI_ID_TEXT1,    68, 485-60, 60,  35,  0}, // Start
	{ MultiText_CreateIndirect, "", GUI_ID_TEXT2,    4,  485-60, 60,  35,  0}, // Pause
	//{ TEXT_CreateIndirect,      "", GUI_ID_TEXT3,    8,  416, 112, 25,  TEXT_CF_HCENTER|TEXT_CF_VCENTER},	

	//{ WINDOW_CreateIndirect,      "", 0,                   901,	 79,  121, 586, FRAMEWIN_CF_ACTIVE},
	//{ BUTTON_CreateIndirect,      "", GUI_ID_BUTTON4,      0,    2,   121, 61,  0},//Alarm Silence
	//{ BUTTON_CreateIndirect,      "", GUI_ID_BUTTON0,      0,    67,  121, 61,  0},//Modes
	//{ BUTTON_CreateIndirect,      "", GUI_ID_BUTTON3,      0,    132, 121, 61,  0},//Monitoring
	//{ BUTTON_CreateIndirect,      "", GUI_ID_BUTTON6,      0,    197,  121, 61,  0},//Trends

	//{ BUTTON_CreateIndirect,      "", GUI_ID_BUTTON1,      0,    262, 121, 61,  0},//Alarms
	//{ BUTTON_CreateIndirect,      "", GUI_ID_BUTTON2,      0,    327, 121, 61,  0},//System

	////{ BUTTON_CreateIndirect,      "", GUI_ID_BUTTON5,      0,    392, 121, 61,  0},//O2 Module
	//{ BUTTON_CreateIndirect,      "", GUI_ID_BUTTON8,      0,    392, 121, 61,  0},//吸气保持
	//{ BUTTON_CreateIndirect,      "", GUI_ID_BUTTON7,      0,    457, 121, 61,  0},//Standby
};
#else
static const GUI_WIDGET_CREATE_INFO _aDialogCreate20[] =  //sun add 20140514
{
	{ WINDOW_CreateIndirect,      "", 0,                   687, 80-38,  120, 400-30, FRAMEWIN_CF_ACTIVE},

	{ BUTTON_CreateIndirect,      "", GUI_ID_BUTTON4,      5,   0,  108,  42,  0}, // Alarm Silence
	{ BUTTON_CreateIndirect,      "", GUI_ID_BUTTON0,      5,  45,  108,  42,  0}, // Modes
	{ BUTTON_CreateIndirect,      "", GUI_ID_BUTTON1,      5,  90,  108,  42,  0}, // Alarms
	{ BUTTON_CreateIndirect,      "", GUI_ID_BUTTON6,      5, 135,  108,  42,  0}, // Trends

	//{ BUTTON_CreateIndirect,      "", GUI_ID_BUTTON3,      5, 180,  108,  42,  0}, // Monitoring
	//{ BUTTON_CreateIndirect,      "", GUI_ID_BUTTON8,      5, 225,  108,  42,  0}, // Tools
	//{ BUTTON_CreateIndirect,      "", GUI_ID_BUTTON2,      5, 270,  108,  42,  0}, // System

	{ BUTTON_CreateIndirect,      "", GUI_ID_BUTTON2,      5, 180,  108,  42,  0}, // System

	{ BUTTON_CreateIndirect,      "", GUI_ID_BUTTON3,      5, 225,  108,  42,  0}, // Monitoring
	{ BUTTON_CreateIndirect,      "", GUI_ID_BUTTON8,      5, 270,  108,  42,  0}, // Tools

	{ BUTTON_CreateIndirect,      "", GUI_ID_BUTTON7,      5, 315,  108,  42,  0}, // Standby
};

#endif

static const GUI_WIDGET_CREATE_INFO _aDialogCreate21[] =
{
  //  { WINDOW_CreateIndirect,          "",   0,                 0,    400, 645+66, 80, FRAMEWIN_CF_ACTIVE},
  ////{ EDIT_CreateIndirect,            "",   GUI_ID_EDIT0,      5,    5,   590, 30, EDIT_CF_LEFT|EDIT_CF_VCENTER},
  //  { ButtonExCtrl_CreateIndirect,    "",   GUI_ID_BUTTON0,    0,    3,   88,  75, 0},
  //  { ButtonExCtrl_CreateIndirect,    "",   GUI_ID_BUTTON1,    89,   3,   88,  75, 0},
  //  { ButtonExCtrl_CreateIndirect,    "",   GUI_ID_BUTTON2,    178,  3,   88,  75, 0},
  //  { ButtonExCtrl_CreateIndirect,    "",   GUI_ID_BUTTON3,    267,  3,   88,  75, 0},
  //  { ButtonExCtrl_CreateIndirect,    "",   GUI_ID_BUTTON4,    356,  3,   88,  75, 0},
  //  { ButtonExCtrl_CreateIndirect,    "",   GUI_ID_BUTTON5,    445,  3,   88,  75, 0},
  //  { ButtonExCtrl_CreateIndirect,    "",   GUI_ID_BUTTON6,    534,  3,   88,  75, 0},

  //  { ButtonExCtrl_CreateIndirect,    "",   GUI_ID_BUTTON7,    623,  3,   88,  75, 0},  //sun add 20140623

	{ WINDOW_CreateIndirect,          "",   0,                 0,    666-53-13, 1024, 102+53+13, FRAMEWIN_CF_ACTIVE},
	{ ButtonExCtrl_CreateIndirect,    "",   GUI_ID_BUTTON0,    1,    3+53+11,   111,  97, 0},
	{ ButtonExCtrl_CreateIndirect,    "",   GUI_ID_BUTTON1,    115,  3+53+11,   111,  97, 0},
	{ ButtonExCtrl_CreateIndirect,    "",   GUI_ID_BUTTON2,    229,  3+53+11,   111,  97, 0},
	{ ButtonExCtrl_CreateIndirect,    "",   GUI_ID_BUTTON3,    343,  3+53+11,   111,  97, 0},
	{ ButtonExCtrl_CreateIndirect,    "",   GUI_ID_BUTTON4,    457,  3+53+11,   111,  97, 0},
	{ ButtonExCtrl_CreateIndirect,    "",   GUI_ID_BUTTON5,    571,  3+53+11,   111,  97, 0},
	{ ButtonExCtrl_CreateIndirect,    "",   GUI_ID_BUTTON6,    685,  3+53+11,   111,  97, 0},
	{ ButtonExCtrl_CreateIndirect,    "",   GUI_ID_BUTTON7,    799,  3+53+11,   111,  97, 0},

	{ ModeTabBar_CreateIndirect,      "",   GUI_ID_BUTTON8,    0,  0,   1024,  48+5, 0},

	{ TEXT_CreateIndirect,			  "",   GUI_ID_TEXT0,      0,    0,   1024,  2, 0}, // ?
};

static const GUI_WIDGET_CREATE_INFO _aDialogCreate22[] =  //sun add 20140516
{
  { WINDOW_CreateIndirect,        "", 0,                0,   66,  886,  547-13, FRAMEWIN_CF_ACTIVE},
	 //上侧
//   { IMAGETEXT_CreateIndirect, "",    GUI_ID_IMAGETEXT0,         0,  0,  40,  40},//CPB
//   { IMAGETEXT_CreateIndirect, "",    GUI_ID_IMAGETEXT1,         0,  0,  40,  40},//
//   { IMAGETEXT_CreateIndirect, "",    GUI_ID_IMAGETEXT2,         0,  0,  40,  40},//

	 //波形
  { TracingCurve_CreateIndirect,  "", GUI_ID_CURVE0,    226,   1,  500,  177, 0},// w 486
  { TracingCurve_CreateIndirect,  "", GUI_ID_CURVE1,    226,   179, 500,  177, 0},
  { TracingCurve_CreateIndirect,  "", GUI_ID_CURVE2,    226,   357, 500,  177, 0},
  { TracingCurve_CreateIndirect,  "", GUI_ID_CURVE3,    226,   357, 250,  177, 0}, // 第四道环波

  //{BUTTON_CreateIndirect, "", GUI_ID_BUTTON0, 669, 25,  28, 28, 0}, //
  {BUTTON_CreateIndirect, "", GUI_ID_BUTTON1, 669, 203, 28, 28, 0}, //
  {BUTTON_CreateIndirect, "", GUI_ID_BUTTON2, 669, 380, 28, 28, 0}, //
  {BUTTON_CreateIndirect, "", GUI_ID_BUTTON3, 440, 380, 28, 28, 0}, //

  {PawGauge_CreateIndirect, "", GUI_ID_TEXT6, 0, 0, 223, 217, 0}, // 压力表

  //右侧
  { MultiText_CreateIndirect, "", GUI_ID_TEXT0,     728, 0,  158,  75,  0},
  { MultiText_CreateIndirect, "", GUI_ID_TEXT1,     728, 77,  158,  75,  0},
  { MultiText_CreateIndirect, "", GUI_ID_TEXT2,     728, 154, 158,  74,  0},
  { MultiText_CreateIndirect, "", GUI_ID_TEXT3,     728, 230, 158,  74,  0},
  { MultiText_CreateIndirect, "", GUI_ID_TEXT4,     728, 306, 158,  74,  0},
  { MultiText_CreateIndirect, "", GUI_ID_TEXT5,     728, 382, 158,  75,  0},
  { MultiText_CreateIndirect, "", GUI_ID_TEXT15,    728, 459, 158,  75,  0},
  //

//  { MultiText_CreateIndirect, "", GUI_ID_TEXT6,     0, 31,  224,  58,  0},

  { MultiText_CreateIndirect, "", GUI_ID_TEXT7,     0, 208+11,  224,  51,  0}, // MAC
  { MultiText_CreateIndirect, "", GUI_ID_TEXT8,     0, 261+11, 224,  34,  0}, // Fi Et
  { MultiText_CreateIndirect, "", GUI_ID_TEXT12,     0, 297+11, 224,  74,  0}, // CO2
  //{ MultiText_CreateIndirect, "", GUI_ID_TEXT10,    0, 203, 177,  50,  0}, // AA2
  { MultiText_CreateIndirect, "", GUI_ID_TEXT9,    0, 373+11, 224,  74,  0}, // AA1
  { MultiText_CreateIndirect, "", GUI_ID_TEXT11,    0, 448+11, 224,  74,  0}, // N2O

//  { MultiText_CreateIndirect, "", GUI_ID_TEXT13,    0,  0, 93,  31,  0},//Start/Stop
//  { MultiText_CreateIndirect, "", GUI_ID_TEXT14,    93, 0, 84,  31,  0},//Clear
};

// static const GUI_WIDGET_CREATE_INFO _aDialogCreateCPBInfo[] =    //sun add 20140529
// {
// 	{ WINDOW_CreateIndirect,    "",       0,          0, 42, 176, 32, FRAMEWIN_CF_ACTIVE },
// 	{ MultiText_CreateIndirect, "", GUI_ID_TEXT0,     0, 0,  176, 32, TEXT_CF_LEFT | TEXT_CF_TOP},
// };

static const GUI_WIDGET_CREATE_INFO _aDialogCreate24[] =    //sun add 20140529
{
	{ WINDOW_CreateIndirect,    "",       0,                   345, 192, 270, 160, FRAMEWIN_CF_ACTIVE },
	{ TEXT_CreateIndirect,     "",     GUI_ID_TEXT0,		15,  20, 240,  40, 0},
	{ TEXT_CreateIndirect,     "",     GUI_ID_TEXT1,		15,  60, 240,  40, 0},	
	{ BUTTON_CreateIndirect,    "21%",   GUI_ID_BUTTON0,   185,  115,  70,  30, 0},
	{ BUTTON_CreateIndirect,    "100%",   GUI_ID_BUTTON1,  100,  115,  70,  30, 0},
	{ BUTTON_CreateIndirect,    "EXIT",   GUI_ID_BUTTON2,   15,  115,  70,  30, 0},
};

//电池校验界面 
static const GUI_WIDGET_CREATE_INFO _aDialogBatteryCalibration[] =
{
	{ WINDOW_CreateIndirect,    "",       0,                   15,   190, 615, 190, FRAMEWIN_CF_ACTIVE },//调整窗口位置
	//Battery voltage
	{ TEXT_CreateIndirect,     "",   GUI_ID_TEXT0,   20,   10, 170,  16, TEXT_CF_LEFT |TEXT_CF_VCENTER},//Battery Voltage
	{ TEXT_CreateIndirect,     "",   GUI_ID_TEXT1,   190,  10, 30,  16, TEXT_CF_LEFT |TEXT_CF_VCENTER},//
	{ TEXT_CreateIndirect,     "",   GUI_ID_TEXT2,   220,  10, 80,  16, TEXT_CF_LEFT |TEXT_CF_VCENTER},//V
	{ TEXT_CreateIndirect,     "",   GUI_ID_TEXT3,   20,   30, 200,  16, TEXT_CF_LEFT |TEXT_CF_VCENTER},//
	{ TEXT_CreateIndirect,     "",   GUI_ID_TEXT4,   20,   50, 200,  16, TEXT_CF_LEFT |TEXT_CF_VCENTER},//Cali. Date
	{ TEXT_CreateIndirect,     "",   GUI_ID_TEXT5,   20,   70, 200,  16, TEXT_CF_LEFT |TEXT_CF_VCENTER},//
	{ TEXT_CreateIndirect,     "",   GUI_ID_TEXT6,   20,   90, 200,  16, TEXT_CF_LEFT |TEXT_CF_VCENTER},//Cali. Duration
	{ BUTTON_CreateIndirect,    "",   GUI_ID_BUTTON0,     20,   115, 100, 30, 0},
	{ BUTTON_CreateIndirect,    "",   GUI_ID_BUTTON1,     140,  115, 100, 30, 0},

	//
};
//选配加密界面
static const GUI_WIDGET_CREATE_INFO _aDialogSoftwareOptional[] =
{
    { WINDOW_CreateIndirect,    "",       0,                    50, 187, 550, 196, FRAMEWIN_CF_ACTIVE },//ylk 修改界面位置 20180227
    { BUTTON_CreateIndirect,    "0",   GUI_ID_BUTTON0,    0,    155,  70, 40, 0},
    { BUTTON_CreateIndirect,    "1",   GUI_ID_BUTTON1,    73,   155,  70,  40, 0},
    { BUTTON_CreateIndirect,    "2",   GUI_ID_BUTTON2,    146,  155,  70,  40, 0},
    { BUTTON_CreateIndirect,    "3",   GUI_ID_BUTTON3,    219,  155,  70,  40, 0},
    { BUTTON_CreateIndirect,    "4",   GUI_ID_BUTTON4,    73,   110,  70,  40, 0},
    { BUTTON_CreateIndirect,    "5",   GUI_ID_BUTTON5,    146,  110,  70,  40, 0},
    { BUTTON_CreateIndirect,    "6",   GUI_ID_BUTTON6,    219,  110,  70,  40, 0},
    { BUTTON_CreateIndirect,    "7",   GUI_ID_BUTTON7,    73,   65,  70,  40, 0},
    { BUTTON_CreateIndirect,    "8",   GUI_ID_BUTTON8,    146,  65,  70,  40, 0},
    { BUTTON_CreateIndirect,    "9",   GUI_ID_BUTTON9,    219,  65,  70,  40, 0},
    { BUTTON_CreateIndirect,    "A",   GUI_ID_BUTTON10,   304, 155,  70,  40, 0},
    { BUTTON_CreateIndirect,    "B",   GUI_ID_BUTTON11,   377,  155,  70,  40, 0},
    { BUTTON_CreateIndirect,    "C",   GUI_ID_BUTTON12,   304,  110,  70,  40, 0},
    { BUTTON_CreateIndirect,    "D",   GUI_ID_BUTTON13,   377,  110,  70,  40, 0},
    { BUTTON_CreateIndirect,    "E",   GUI_ID_BUTTON14,   304,  65,  70,  40, 0},
    { BUTTON_CreateIndirect,    "F",   GUI_ID_BUTTON15,   377,  65,  70,  40, 0},
    { BUTTON_CreateIndirect,    "Exit",   GUI_ID_BUTTON16, 0,   65,  70, 83, 0},
    { BUTTON_CreateIndirect,    "Enter",   GUI_ID_BUTTON17, 450, 110, 70, 83, 0},
    { BUTTON_CreateIndirect,    "Backspace",   GUI_ID_BUTTON18, 450,  65,  70, 42, 0},

    { TEXT_CreateIndirect,     "Please entry the serial:",     GUI_ID_TEXT0,   0,  0, 297,  24, TEXT_CF_LEFT |TEXT_CF_VCENTER},//prompt
    { TEXT_CreateIndirect,     "",     GUI_ID_TEXT1,    304,  26, 215,  32, TEXT_CF_LEFT |TEXT_CF_VCENTER},//prompt
    { TEXT_CreateIndirect,     "ID:",      GUI_ID_TEXT2,   304,  0, 30,  32, TEXT_CF_LEFT |TEXT_CF_VCENTER},//prompt
    { TEXT_CreateIndirect,     "",         GUI_ID_TEXT3,   334,  0, 130,  32, TEXT_CF_LEFT |TEXT_CF_VCENTER},//prompt
    { MULTIEDIT_CreateIndirect, "",GUI_ID_EDIT0,     1, 26 , 290,  35,TEXT_CF_HCENTER |TEXT_CF_VCENTER}
};
//开机询问自检界面
static const GUI_WIDGET_CREATE_INFO _aDialogSelfCheck_Ask[] =
{
    { WINDOW_CreateIndirect,    "", 0,                 0, 447, 1024,321, FRAMEWIN_CF_ACTIVE },
    { TEXT_CreateIndirect,    "",     GUI_ID_TEXT0,       0, 20,  1024,  30, TEXT_CF_HCENTER|TEXT_CF_VCENTER},//Hold
    { TEXT_CreateIndirect,    "",     GUI_ID_TEXT1,       0, 75,  1024,  30, TEXT_CF_HCENTER|TEXT_CF_VCENTER},//Result
    { BUTTON_CreateIndirect,  "",   GUI_ID_BUTTON0,       165, 170, 300, 73, 0},//Cancel
    { BUTTON_CreateIndirect,  "",   GUI_ID_BUTTON1,       559, 170, 300, 73, 0},//Cancel
};
//Medibus 通讯打开/关闭窗口
static const GUI_WIDGET_CREATE_INFO _aDialogMedibusComm[] =
{
	{ WINDOW_CreateIndirect,   "",       0,				 15,   190,		615,	190, FRAMEWIN_CF_ACTIVE },
	{ TEXT_CreateIndirect,     "",   GUI_ID_TEXT0,		  30,    10,		560,	30, TEXT_CF_HCENTER |TEXT_CF_VCENTER},
	{ TEXT_CreateIndirect,     "",   GUI_ID_TEXT1,		  30,	50,	    560,	30, TEXT_CF_HCENTER |TEXT_CF_VCENTER},
	{ TEXT_CreateIndirect,     "",   GUI_ID_TEXT2,		  30,   100,		560,	30, TEXT_CF_HCENTER |TEXT_CF_VCENTER},
	{ BUTTON_CreateIndirect,   "",   GUI_ID_BUTTON0,     20,   150,		270,	30, 0},
	{ BUTTON_CreateIndirect,   "",   GUI_ID_BUTTON1,    320,   150,		270,	30, 0},
};
static const GUI_WIDGET_CREATE_INFO _aDialogSettingMenu_SpO2[] =  //系统设置二级菜单-----血氧
{
	{ WINDOW_CreateIndirect,			"",   0,                  0,    0,  605+45,  200, FRAMEWIN_CF_ACTIVE },
	{ TEXT_CreateIndirect,				"",   GUI_ID_TEXT0,      15,   12,  150,   20, TEXT_CF_LEFT |TEXT_CF_VCENTER},//
	{ TEXT_CreateIndirect,				"",   GUI_ID_TEXT1,      15,   40,  150,   20, TEXT_CF_LEFT |TEXT_CF_VCENTER},//
	{ TEXT_CreateIndirect,				"",   GUI_ID_TEXT2,      15,   65,  150,   20, TEXT_CF_LEFT |TEXT_CF_VCENTER},//
	{ BUTTON_CreateIndirect,			"",   GUI_ID_BUTTON0,    13,  163,  112,   30, 0},//

	{ TEXT_CreateIndirect,				"",   GUI_ID_TEXT3,     192,   30,  150,   20, TEXT_CF_LEFT |TEXT_CF_VCENTER},//
	{ DROPDOWN_CreateIndirect,			"",   GUI_ID_DROPDOWN0, 192,   55,  150,  100, GUI_TA_LEFT|GUI_TA_VCENTER},//
	{ TEXT_CreateIndirect,				"",   GUI_ID_TEXT4,     367,   30,  150,   20, TEXT_CF_LEFT |TEXT_CF_VCENTER},//
	{ DROPDOWN_CreateIndirect,			"",   GUI_ID_DROPDOWN1, 367,   55,  150,   75, GUI_TA_LEFT|GUI_TA_VCENTER},//

	{ TEXT_CreateIndirect,				"",   GUI_ID_TEXT5,     205,   97,  150,   20, TEXT_CF_LEFT |TEXT_CF_VCENTER},//Smart tone
	{ ButtonRotation_CreateIndirect,    "",   GUI_ID_BUTTON1,   204,  117,   65,   65, 0},
	{ TEXT_CreateIndirect,				"",   GUI_ID_TEXT6,     286,   97,  85,   20, TEXT_CF_HCENTER |TEXT_CF_VCENTER},//Fast Sat
	{ ButtonRotation_CreateIndirect,    "",   GUI_ID_BUTTON2,   296,  117,   65,   65, 0},
	{ TEXT_CreateIndirect,				"",   GUI_ID_TEXT7,     150,   12,   50,   20, TEXT_CF_HCENTER |TEXT_CF_VCENTER},//
};
static const GUI_WIDGET_CREATE_INFO _aDialogSettingMenu_Compensate[] =  //系统设置二级菜单-----O2和N2O补偿菜单
{
	{ WINDOW_CreateIndirect,			"",   0,                  0,    0,  605,  200, FRAMEWIN_CF_ACTIVE },
	{ BUTTON_CreateIndirect,			"",   GUI_ID_BUTTON0,    13,  163,  112,   30, 0},//

	{ TEXT_CreateIndirect,				"",   GUI_ID_TEXT3,     192,   50,  150,   20, TEXT_CF_LEFT |TEXT_CF_VCENTER},//
	{ DROPDOWN_CreateIndirect,			"",   GUI_ID_DROPDOWN0, 192,   75,  150,   75, GUI_TA_LEFT|GUI_TA_VCENTER},//
	{ TEXT_CreateIndirect,				"",   GUI_ID_TEXT4,     367,   50,  150,   20, TEXT_CF_LEFT |TEXT_CF_VCENTER},//
	{ DROPDOWN_CreateIndirect,			"",   GUI_ID_DROPDOWN1, 367,   75,  150,   50, GUI_TA_LEFT|GUI_TA_VCENTER},//

	{ TEXT_CreateIndirect,				"",   GUI_ID_TEXT7,     150,   12,   80,   20, TEXT_CF_HCENTER |TEXT_CF_VCENTER},//
};
/**************************************************************************************************/

static const GUI_WIDGET_CREATE_INFO _aDialogCreateAlarmInfo[] =
{
	{ WINDOW_CreateIndirect,    "",      0,                     213,  0, 535,  66, FRAMEWIN_CF_ACTIVE },
	{ IMAGETEXT_CreateIndirect, "",    GUI_ID_IMAGETEXT0,         0,  0,  40,  66},//bell
	{ IMAGETEXT_CreateIndirect, "",    GUI_ID_IMAGETEXT1,        41,  0, 453,  33},//alarm text
	{ IMAGETEXT_CreateIndirect, "",    GUI_ID_IMAGETEXT4,        41,  34, 453,  32},//alarm text1

	{ TEXT_CreateIndirect,      "",    GUI_ID_TEXT0,              0,  0,  40,  66, TEXT_CF_HCENTER | TEXT_CF_VCENTER}, // 当前报警数目
// 	{ TEXT_CreateIndirect,      "",    GUI_ID_TEXT1,             35,  42, 300,  19, TEXT_CF_HCENTER|TEXT_CF_VCENTER},

	{ IMAGETEXT_CreateIndirect, "",    GUI_ID_IMAGETEXT2,      495, 0, 40, 40}, // 倒计时图标
	{ IMAGETEXT_CreateIndirect, "",    GUI_ID_IMAGETEXT3,      495,40, 40, 26}, // 倒计时文本

};

static const GUI_WIDGET_CREATE_INFO _aDialogSoftwareVersion[] = 
{
	{ WINDOW_CreateIndirect,   "",       0,				 15,   190,		615,   190, FRAMEWIN_CF_ACTIVE },
	{ TEXT_CreateIndirect,      "",    GUI_ID_TEXT0,      35,  30,  120,  30, TEXT_CF_LEFT|TEXT_CF_TOP},
	{ TEXT_CreateIndirect,      "",    GUI_ID_TEXT1,      35,  65,  120,  30, TEXT_CF_LEFT|TEXT_CF_TOP},
	{ TEXT_CreateIndirect,      "",    GUI_ID_TEXT2,      35,  100,  120,  30, TEXT_CF_LEFT|TEXT_CF_TOP},
	{ TEXT_CreateIndirect,      "",    GUI_ID_TEXT6,      35,  135,  120,  30, TEXT_CF_LEFT|TEXT_CF_TOP},//PM

	{ TEXT_CreateIndirect,      "",    GUI_ID_TEXT3,      155,  30,  120,  30, TEXT_CF_LEFT|TEXT_CF_TOP},//EFM
	{ TEXT_CreateIndirect,      "",    GUI_ID_TEXT4,      155,  65,  120,  30, TEXT_CF_LEFT|TEXT_CF_TOP},//Font
	{ TEXT_CreateIndirect,      "",    GUI_ID_TEXT5,      155,  100,  120,  30, TEXT_CF_LEFT|TEXT_CF_TOP},//Str
	{ BUTTON_CreateIndirect,    "Exit",   GUI_ID_BUTTON0,     520,   130, 80, 45, 0},
};
static const GUI_WIDGET_CREATE_INFO _aDialogShutDown_Ask[] =
{
	{ WINDOW_CreateIndirect,    "", 0,                 0, 250, 800,230, FRAMEWIN_CF_ACTIVE },
	{ TEXT_CreateIndirect,    "",     GUI_ID_TEXT0,       51, 15,   640,  60, TEXT_CF_HCENTER|TEXT_CF_VCENTER},//Confirm shutdown
	{ TEXT_CreateIndirect,    "",     GUI_ID_TEXT1,       340, 80,  50,   60, TEXT_CF_HCENTER|TEXT_CF_VCENTER},//时间
	{ TEXT_CreateIndirect,    "",     GUI_ID_TEXT2,       395, 80,  50,   60, TEXT_CF_HCENTER|TEXT_CF_VCENTER},//s
};

/**************************************************************************************************/
#ifdef LANGUAGE_CHINESE_FLAG
const unsigned short Name[18] = 
{
	DP_VTI,       DP_VTE,       DP_MV,        DP_MVSPN,     DP_FTOTAL,    DP_FSPN,
	DP_PPEAK,     DP_PMEAN,     DP_PPLAT,     DP_PEEP,      DP_IE,       DP_FIO2,
	DP_RST,       DP_CDYN,      DP_ETCO2,     DP_FICO2
};
const char Unit_1[18][10]= 
{
    "mL",        "mL",        "L",         "L",         "bpm",       "bpm",
    "cmH2O",     "cmH2O",     "cmH2O",     "cmH2O",     "",            "%",
    "cmH2O/L/S", "mL/cmH2O",  "mmHg",      "mmHg",      "",         ""
};
int ParaMax[] =
{
    2000,        2500,       6000,        6000,        150,          150,
    1000,        1000,        1000,        1000,        150,          100,
    600,         300,         1000,        1000,        0,          0
};
int ParaMin[] =
{
    0,           0,           0,           0,           0,            0,
    0,           0,           0,           0,           0,           15,
    0,           0,           0,           0,           0,            0
};
int Floatflag[] =
{
    0,           0,           0,           0,           0,           0,
    1,           1,           1,           1,           0,           0,
    0,           0,           1,           1,           0,           0
};
#else
const unsigned short Name[18] = 
{
		DP_VTI,       DP_VTE,       DP_MV,        DP_MVSPN,     DP_FTOTAL,    DP_FSPN,
		DP_PPEAK,     DP_PMEAN,     DP_PPLAT,     DP_PEEP,      DP_IE,       DP_FIO2,
		DP_RST,       DP_CDYN,      DP_SPO2,      DP_PULSE,     DP_PI
};
// const char Unit_1[17][10]= 
// {
// 		"mL",        "mL",        "L",         "L",         "bpm",       "bpm",
// 		"cmH2O",     "cmH2O",     "cmH2O",     "cmH2O",     "",            "%",
// 		"cmH2O/L/S", "mL/cmH2O",  "%",         "bpm",	    "%"
// };

int ParaMax[] =
{
		2000,        2500,        6000,        6000,        150,          150,
		1000,        1000,        1000,        1000,        150,          100,
		600,         300,         100,          250,		2000  
};
int ParaMin[] =
{
		0,           0,           0,           0,           0,            0,
		0,           0,           0,           0,           0,           15,
		0,           0,           0,           0,           0
};
int Floatflag[] =
{
		0,           0,           0,           0,           0,           0,
		1,           1,           1,           1,           0,           0,
		0,           0,           0,           0,			2
};
#endif
/**************************************************************************************************/
/**************************************************************************************************/

const unsigned short Name1[18] = 
{
    DP_MAC,   DP_FICO2, DP_ETCO2, DP_FIN2O,    DP_ETN2O,    DP_FIAA1,   DP_ETAA1,
    DP_FIAA2, DP_ETAA2
};
const char Unit1[][10]= 
{
    "",         "%",         "%",         "%",         "%",         "%",
    "%",		 "%",		  "%"				
};
int ParaMax1[] =
{
    1000,         1000,        1000,         100,         100,         300,         
	 300,          300,			300
};
int ParaMin1[] =
{
    0,           0,           0,           0,           0,            0,
    0,			 0,           0
};
int Floatflag1[] =
{
    1,           1,           1,           0,           0,           1,          
	1,           1,			  1
};

const unsigned short AAName[14] =
{
//     STR_FiAA1,       STR_FiHAL,      STR_FiENF,       STR_FiISO,       STR_FiSEV,       STR_FiDES,
// 	STR_EtAA1,       STR_EtHAL,      STR_EtENF,       STR_EtISO,       STR_EtSEV,       STR_EtDES,
//     STR_FiAA2,       STR_EtAA2

	DP_FIAA1,       DP_FIHAL,      DP_FIENF,       DP_FIISO,       DP_FISEV,       DP_FIDES,
	DP_ETAA1,       DP_ETHAL,      DP_ETENF,       DP_ETISO,       DP_ETSEV,       DP_ETDES,
	DP_FIAA2,       DP_ETAA2	
};

/**************************************************************************************************/
extern WM_HWIN Focus_hWin;
/***************************************************************************************************
*
*       Static code
*
****************************************************************************************************
*/

const char *UnitMonitor[18];

void MonitorUnit_Init()
{
	UnitMonitor[0] = Sys_Lib_GetDispStr(DP_ML);
	UnitMonitor[1] = Sys_Lib_GetDispStr(DP_ML);
	UnitMonitor[2] = Sys_Lib_GetDispStr(DP_LMIN);
	UnitMonitor[3] = Sys_Lib_GetDispStr(DP_LMIN);
	UnitMonitor[4] = Sys_Lib_GetDispStr(DP_BPM);
	UnitMonitor[5] = Sys_Lib_GetDispStr(DP_BPM);
	UnitMonitor[6] = Sys_Lib_GetDispStr(DP_CMH2O);
	UnitMonitor[7] = Sys_Lib_GetDispStr(DP_CMH2O);
	UnitMonitor[8] = Sys_Lib_GetDispStr(DP_CMH2O);
	UnitMonitor[9] = Sys_Lib_GetDispStr(DP_CMH2O);
	UnitMonitor[10] = Sys_Lib_GetDispStr(DP_EMPTY);
	UnitMonitor[11] = Sys_Lib_GetDispStr(DP_PERCENT);
	UnitMonitor[12] = Sys_Lib_GetDispStr(DP_CMH2OLS);
	UnitMonitor[13] = Sys_Lib_GetDispStr(DP_MLCMH2O);
	UnitMonitor[14] = Sys_Lib_GetDispStr(DP_PERCENT);
	UnitMonitor[15] = Sys_Lib_GetDispStr(DP_BPM);
	UnitMonitor[16] = Sys_Lib_GetDispStr(DP_PERCENT);
	UnitMonitor[17] = Sys_Lib_GetDispStr(DP_EMPTY);
}

//Breath_Parameter  呼吸参数结构体赋值函数
void BreathPara_Assign(BREATH_SET_PARA *Dest,const BREATH_SET_PARA *Src)
{
	Dest->SettingVtValue = Src->SettingVtValue;
	Dest->SettingTiValue = Src->SettingTiValue;
	Dest->settingTeValue = Src->settingTeValue;
	Dest->SettingRateValue = Src->SettingRateValue;
	Dest->SettingSimvRateValue = Src->SettingSimvRateValue;
	Dest->SettingotherRateValue = Src->SettingotherRateValue;
	Dest->SettingTpValue = Src->SettingTpValue;
	Dest->SettingPsupportValue = Src->SettingPsupportValue;
	Dest->SettingPinspValue = Src->SettingPinspValue;
	Dest->SettingBipapRateValue = Src->SettingBipapRateValue;
	Dest->SettingPtrValue = Src->SettingPtrValue;
	Dest->SettingFtrValue = Src->SettingFtrValue;
	Dest->SettingPEEPValue = Src->SettingPEEPValue;
	Dest->SettingFiO2Value = Src->SettingFiO2Value;
	Dest->TriggerMode = Src->TriggerMode;
	Dest->BreathMode = Src->BreathMode;
	Dest->SettingThighValue = Src->SettingThighValue;
	Dest->SettingTlowValue = Src->SettingTlowValue;
	Dest->ApnoeaMode = Src->ApnoeaMode;
	Dest->PreBreathMode = Src->PreBreathMode;
	Dest->PreApneaMode = Src->PreApneaMode;
	Dest->SettingSighTimesValue = Src->SettingSighTimesValue;
	Dest->SettingIEValue = Src->SettingIEValue;
	Dest->SettingTslopeValue = Src->SettingTslopeValue;
	Dest->EsenseValue = Src->EsenseValue;
	Dest->SettingPhighValue = Src->SettingPhighValue;
	Dest->SettingPlowValue = Src->SettingPlowValue;
	Dest->SettingVSRateValue = Src->SettingVSRateValue;
	Dest->SettingVtSupportValue = Src->SettingVtSupportValue;
	Dest->SettingPHLimitValue = Src->SettingPHLimitValue;

}
/***************************************************************************************************
*
* @brief   Modes -> Backup
* @para   
* @retval 
*/
static void _cbDialog1(WM_MESSAGE * pMsg)
{
    WM_HWIN i,hDlg,hButt0,hButt1,hButt3,hButt4;
    WM_MESSAGE Msg;
    int Id,NCode,flg=0,flg1=0;
    int Value; //  20130618
    int Data;
    hDlg = pMsg->hWin;
    switch (pMsg->MsgId) 
    {
    case WM_INIT_DIALOG:
        hButt3 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON3);
        hButt4 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON4);
        BUTTON_SetBkColor(hButt3,0,0xD05356);
        BUTTON_SetBkColor(hButt4,0,0xD05356);
        if((Breath_Setting_Tmp.PreBreathMode != PSV_MODE) && (Breath_Setting_Tmp.BreathMode == PSV_MODE))
        {
            ReviseParamatersAllMode(Breath_Setting_Tmp.BreathMode,&Breath_Setting_Tmp,AlarmSettingPara.PressureHighValue);
        }
        hButt0 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON0);
        if(Breath_Setting_Tmp.BreathMode == PSV_MODE)
        {
            if((Breath_Setting_Tmp.ApnoeaMode)!=VCV_MODE)
            {
                BUTTON_SetPressed(hButt4,1);
                BUTTON_SetPressed(hButt3,0);
 				ButtonRotation_SetFontInfo(hButt0,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_PINSP),Sys_Lib_GetDispStr(DP_CMH2O));
                ButtonRotation_SetDataInfo(hButt0,5,70,Breath_Setting_Tmp.SettingPinspValue,1,0);
                ButtonRotation_SetData(hButt0,Breath_Setting_Tmp.SettingPinspValue);
            }
            else
            {       
                BUTTON_SetPressed(hButt3,1);
                BUTTON_SetPressed(hButt4,0);
                ButtonRotation_SetFontInfo(hButt0,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_VT),Sys_Lib_GetDispStr(DP_ML));
				if(Breath_Setting_Tmp.SettingVtValue >= 100)
				{
					ButtonRotation_SetDataInfo(hButt0,Vt_LowLimit,Vt_HighLimit,Breath_Setting_Tmp.SettingVtValue,10,0);
				}
				else
				{
					ButtonRotation_SetDataInfo(hButt0,Vt_LowLimit,Vt_HighLimit,Breath_Setting_Tmp.SettingVtValue,5,0);
				}
                //*//   20130618
				
                Value = Breath_Setting_Tmp.SettingIEValue;
                if(Value >= 6)
                {
                    Value = 10*60*100/((10+IE_RatioTable[Value])*Breath_Setting_Tmp.SettingRateValue*1.0) + 0.5;//  20130306
                }
                else
                {
                    Value = IE_RatioTable[Value]*60*100/((10+IE_RatioTable[Value])*Breath_Setting_Tmp.SettingRateValue);//  20130306
                }
                Breath_Setting_Tmp.SettingTiValue = Value;//(Value/10)*10;            
                
                if(Breath_Setting_Tmp.SettingVtValue*100*3 / Breath_Setting_Tmp.SettingTiValue < 100) 
                {
                    Breath_Setting_Tmp.SettingVtValue = Breath_Setting_Tmp.SettingTiValue;
                }
                if(Breath_Setting_Tmp.SettingVtValue*100 / Breath_Setting_Tmp.SettingTiValue > 1500)
                {
                    Breath_Setting_Tmp.SettingVtValue = 1500/100 * Breath_Setting_Tmp.SettingTiValue; 
                }         
                if(Breath_Setting_Tmp.SettingVtValue > 100)  //sun add 20140707
                {
                    if((Breath_Setting_Tmp.SettingVtValue % 10) != 0)
                    {
                        Breath_Setting_Tmp.SettingVtValue = Breath_Setting_Tmp.SettingVtValue - (Breath_Setting_Tmp.SettingVtValue % 10);
                    }
                }
                else
                {
                    if((Breath_Setting_Tmp.SettingVtValue % 5) != 0)
                    {
                        Breath_Setting_Tmp.SettingVtValue = Breath_Setting_Tmp.SettingVtValue - (Breath_Setting_Tmp.SettingVtValue % 5);
                    }
                }
                ButtonRotation_SetData(hButt0,Breath_Setting_Tmp.SettingVtValue);
            }
        }
        
        hButt0 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON1);
        ButtonRotation_SetFontInfo(hButt0,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_IE),Sys_Lib_GetDispStr(DP_EMPTY));
#if (LOGO_FLAG == ENDURE_LOGO)
		ButtonRotation_SetDataInfo(hButt0,0,28,Breath_Setting_Tmp.SettingIEValue,1,0);//1,120,Breath_Setting_Tmp.SettingTiValue,1,1 //cpp md 20130416
#else
		ButtonRotation_SetDataInfo(hButt0,0,24,Breath_Setting_Tmp.SettingIEValue,1,0);//1,120,Breath_Setting_Tmp.SettingTiValue,1,1 //cpp md 20130416
#endif
        ButtonRotation_SetData(hButt0,Breath_Setting_Tmp.SettingIEValue);
        
        hButt0 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON2);
        ButtonRotation_SetFontInfo(hButt0,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_F),Sys_Lib_GetDispStr(DP_BPM));
        ButtonRotation_SetDataInfo(hButt0,2,100,Breath_Setting_Tmp.SettingotherRateValue,1,0);
        ButtonRotation_SetData(hButt0,Breath_Setting_Tmp.SettingotherRateValue);
        if(AlarmsWork.presentAlarms&ALARM_APNEA)
        {
            if(Breath_Setting_Tmp.ApnoeaMode==VCV_MODE)
            {
                WM_DisableWindow(hButt4);
            }
            else
            {
                WM_DisableWindow(hButt3);
            }
        }
        else
		{
			WM_EnableWindow(hButt3);
			WM_EnableWindow(hButt4);
		}
        
        BUTTON_SetText(hButt3,"VCV");
        BUTTON_SetText(hButt4,"PCV");
        
        break;
    case WM_KEY:
        break;
    case WM_NOTIFY_PARENT:
        NCode = pMsg->Data.v;
        Id = WM_GetId(pMsg->hWinSrc);
        switch(NCode)
        {
        case WM_NOTIFICATION_LOST_FOCUS: //add zkq 20161024 失去焦点后弹起
            ButtonRotation_SetTriangleSign(WM_GetDialogItem(hDlg,GUI_ID_BUTTON0), 0);
            ButtonRotation_SetTriangleSign(WM_GetDialogItem(hDlg,GUI_ID_BUTTON1), 0);
            ButtonRotation_SetTriangleSign(WM_GetDialogItem(hDlg,GUI_ID_BUTTON2), 0);
            
            break;
        case WM_NOTIFICATION_CLICKED:
            switch(Id)
            {
            case GUI_ID_BUTTON0:
            case GUI_ID_BUTTON1:
            case GUI_ID_BUTTON2:
                if(ButtonRotation_GetTriangleSign(pMsg->hWinSrc))
                {
                    WM_SelectWindow(pMsg->hWinSrc);
                    ButtonRotation_SetTriangleSign(pMsg->hWinSrc,0);
                    Data = ButtonRotation_GetData1(pMsg->hWinSrc);
                    ButtonRotation_SetData(pMsg->hWinSrc,Data);
                    SendSetParameter(Data,Id);//transmit the parameter to control mcu
                }
                else
                {
                    ButtonRotation_SetTriangleSign(pMsg->hWinSrc,1);
                    i = WM_GetDialogItem(hDlg,GUI_ID_BUTTON0);
                    while((!ButtonRotation_GetTriangleSign(i))|(i==pMsg->hWinSrc))
                    {
                        i = WM_GetNextSibling(i);
                        if(i==0)
                        {
                            flg = 1;
                            break;
                        }
                    }
                    if(!flg)
                    {
                        ButtonRotation_SetTriangleSign(i,0);
                    }
                    Data = ButtonRotation_GetData(pMsg->hWinSrc);
                    ButtonRotation_SetData1(pMsg->hWinSrc,Data);
                    WM_SetFocus(pMsg->hWinSrc);
                    ButtonRotation_SetTriangleSign(pMsg->hWinSrc,1); //设置按钮为按下 
                }
                break;
            case GUI_ID_BUTTON3: //容量模式
                hButt0 = WM_GetDialogItem(hDlg,GUI_ID_BUTTON4);
                if(BUTTON_IsPressed(hButt0))
                {
                    Breath_Setting_Tmp.ApnoeaMode=VCV_MODE;
                    Breath_Setting_Tmp.PreApneaMode = PCV_MODE;
                    if(!PresetMode_Flag)//ylk add 20181010
                    {
                        SendSetModes(BACKUP_MODE); 
                    }
                    WM_SendMessageNoPara(Menu_hWin.bWin,WM_INIT_DIALOG);
                    ReviseParamatersAllMode(Breath_Setting_Tmp.BreathMode,&Breath_Setting_Tmp,AlarmSettingPara.PressureHighValue);
                    
                    hButt1 = WM_GetDialogItem(hDlg,GUI_ID_BUTTON0);
                    ButtonRotation_SetFontInfo(hButt1,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_VT),"mL");

					if(Breath_Setting_Tmp.SettingVtValue >= 100)
					{
						ButtonRotation_SetDataInfo(hButt1,Vt_LowLimit,Vt_HighLimit,Breath_Setting_Tmp.SettingVtValue,10,0);
					}
					else
					{
						ButtonRotation_SetDataInfo(hButt1,Vt_LowLimit,Vt_HighLimit,Breath_Setting_Tmp.SettingVtValue,5,0);
					}
                    //*//   20130618
                    Value = Breath_Setting_Tmp.SettingIEValue;
                    if(Value >= 6)
                    {
                        Value = 10*60*100/((10+IE_RatioTable[Value])*Breath_Setting_Tmp.SettingRateValue*1.0) + 0.5;//  20130306
                    }
                    else
                    {
                        Value = IE_RatioTable[Value]*60*100/((10+IE_RatioTable[Value])*Breath_Setting_Tmp.SettingRateValue);//  20130306
                    }
                    Breath_Setting_Tmp.SettingTiValue = Value;//(Value/10)*10;            
                    
                    if(Breath_Setting_Tmp.SettingVtValue*100*3 / Breath_Setting_Tmp.SettingTiValue < 100) 
                    {
                        Breath_Setting_Tmp.SettingVtValue = Breath_Setting_Tmp.SettingTiValue;
                    }
                    if(Breath_Setting_Tmp.SettingVtValue*100 / Breath_Setting_Tmp.SettingTiValue > 1500)
                    {
                        Breath_Setting_Tmp.SettingVtValue = 1500/100 * Breath_Setting_Tmp.SettingTiValue; 
                    }//*///            
                    if(Breath_Setting_Tmp.SettingVtValue > 100)  //sun add 20140707
                    {
                        if((Breath_Setting_Tmp.SettingVtValue % 10) != 0)
                        {
                            Breath_Setting_Tmp.SettingVtValue = Breath_Setting_Tmp.SettingVtValue - (Breath_Setting_Tmp.SettingVtValue % 10);
                        }
                    }
                    else
                    {
                        if((Breath_Setting_Tmp.SettingVtValue % 5) != 0)
                        {
                            Breath_Setting_Tmp.SettingVtValue = Breath_Setting_Tmp.SettingVtValue - (Breath_Setting_Tmp.SettingVtValue % 5);
                        }
                    }
                    ButtonRotation_SetData(hButt1,Breath_Setting_Tmp.SettingVtValue);
                    BUTTON_SetPressed(hButt0,0);
                    i = WM_GetDialogItem(hDlg,GUI_ID_BUTTON0);
                    while(!ButtonRotation_GetTriangleSign(i))
                    {
                        i = WM_GetNextSibling(i);
                        if(i==0)
                        {
                            flg = 1;
                            break;
                        }
                    }
                    if(!flg)
                    {
                        ButtonRotation_SetTriangleSign(i,0);
                    }
                }
                break;
            case GUI_ID_BUTTON4: //压力模式
                hButt0 = WM_GetDialogItem(hDlg,GUI_ID_BUTTON3);
                if(BUTTON_IsPressed(hButt0))
                {
                    Breath_Setting_Tmp.ApnoeaMode=PCV_MODE;
                    Breath_Setting_Tmp.PreApneaMode = VCV_MODE;
                    if(!PresetMode_Flag)//ylk add 20181010
                    {
                        SendSetModes(BACKUP_MODE); 
                    }
                    ReviseParamatersAllMode(Breath_Setting_Tmp.BreathMode,&Breath_Setting_Tmp,AlarmSettingPara.PressureHighValue);
                    
                    hButt1 = WM_GetDialogItem(hDlg,GUI_ID_BUTTON0);
                    ButtonRotation_SetFontInfo(hButt1,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_PINSP),Sys_Lib_GetDispStr(DP_CMH2O));
                    ButtonRotation_SetDataInfo(hButt1,5,70,Breath_Setting_Tmp.SettingPinspValue,1,0);
                    ButtonRotation_SetData(hButt1,Breath_Setting_Tmp.SettingPinspValue);
                    BUTTON_SetPressed(hButt0,0);
                    i = WM_GetDialogItem(hDlg,GUI_ID_BUTTON0);
                    while(!ButtonRotation_GetTriangleSign(i))
                    {
                        i = WM_GetNextSibling(i);
                        if(i==0)
                        {
                            flg = 1;
                            break;
                        }
                    }
                    if(!flg)
                    {
                        ButtonRotation_SetTriangleSign(i,0);
                    }
                }
                break;
            }
            break;
        case GUI_KEY_ENTER:
            switch(Id)
            {
            case GUI_ID_BUTTON0:
            case GUI_ID_BUTTON1:
            case GUI_ID_BUTTON2:
                if(ButtonRotation_GetTriangleSign(pMsg->hWinSrc))
                {
                    int Data;
                    WM_SelectWindow(pMsg->hWinSrc);
                    ButtonRotation_SetTriangleSign(pMsg->hWinSrc,0);
                    Data = ButtonRotation_GetData1(pMsg->hWinSrc);
                    ButtonRotation_SetData(pMsg->hWinSrc,Data);
                    SendSetParameter(Data,Id);//transmit the parameter to control mcu
                }
                else
                {
                    ButtonRotation_SetTriangleSign(pMsg->hWinSrc,1);
                    i = WM_GetDialogItem(hDlg,GUI_ID_BUTTON0);
                    while((!ButtonRotation_GetTriangleSign(i))|(i==pMsg->hWinSrc))
                    {
                        i = WM_GetNextSibling(i);
                        if(i==0)
                        {
                            flg = 1;
                            break;
                        }
                    }
                    if(!flg)
                    {
                        ButtonRotation_SetTriangleSign(i,0);
                    }
                }
                break;
            }
            break;
        case GUI_KEY_RIGHT:
            switch(Id)
            {
            case GUI_ID_BUTTON0:
            case GUI_ID_BUTTON1:
            case GUI_ID_BUTTON2:
                if(ButtonRotation_GetTriangleSign(pMsg->hWinSrc))//if setting now?
                {
                    if(ParameterLimitAnalysis(pMsg->hWinSrc,GUI_KEY_RIGHT))
                    {
                        Msg.MsgId   = WM_NOTIFICATION_VALUE_CHANGED;
                        Msg.Data.v  = GUI_KEY_RIGHT;
                        Msg.hWin = pMsg->hWinSrc;
                        WM_SendMessage(pMsg->hWinSrc,&Msg);
                    }
                }
                break;
            }
            break;
        case GUI_KEY_LEFT:
            switch(Id)
            {
            case GUI_ID_BUTTON0:
            case GUI_ID_BUTTON1:
            case GUI_ID_BUTTON2:
                if(ButtonRotation_GetTriangleSign(pMsg->hWinSrc))//if setting now?
                {
                    if(ParameterLimitAnalysis(pMsg->hWinSrc,GUI_KEY_LEFT))
                    {
                        Msg.MsgId   = WM_NOTIFICATION_VALUE_CHANGED;
                        Msg.Data.v  = GUI_KEY_LEFT;
                        Msg.hWin = pMsg->hWinSrc;
                        WM_SendMessage(pMsg->hWinSrc,&Msg);
                    }
                }
                break;
            }
            break;
        }
        break;
    default:
        WM_DefaultProc(pMsg);
    }
}
/***************************************************************************************************
* @brief  Monitoring -> Values
* @para   
* @retval 
*/
static void _cbDialog2(WM_MESSAGE * pMsg)
{
    WM_HWIN hDlg;
    TextExCtrl_Handle hObj,mObj;
    int i=0;
    hDlg = pMsg->hWin;
    switch (pMsg->MsgId)
    {
    case WM_KEY:
        break;
    case WM_INIT_DIALOG:
        hObj = WM_GetDialogItem(hDlg, GUI_ID_TEXT0);
        mObj = hObj;
		MonitorUnit_Init();//
        do
        {
#ifdef LANGUAGE_CHINESE_FLAG
			TextExCtrl_SetTextInfo(mObj,0,Sys_Lib_GetDispStr(Name[i]),&Unit_1[i][0]);
#else
			TextExCtrl_SetTextInfo(mObj,0,Sys_Lib_GetDispStr(Name[i]), UnitMonitor[i]);
#endif

			if(LanguageType == Disp_Lg_Chinese)
			{
				TextExCtrl_SetCtrlFont(mObj,&GUI_Font_15);
			}
			else
			{
				TextExCtrl_SetCtrlFont(mObj,&GUI_Font_16);
			}

            //*//  20130326
            TextExCtrl_SetMaxMinData(mObj,ParaMax[i],ParaMin[i]);
            TextExCtrl_SetFloatLen(mObj,Floatflag[i]);
            //*/
            i++;
            hObj = mObj;
        }
        while((mObj=WM_GetNextSibling(hObj))!=0);
        break;
    case WM_NOTIFY_PARENT_REFLECTION:
        FunctionKeyPressedResume(Menu_hWin.hWin,0);
        if(StandbyState==0xFF)
        {
            PlanMenuType = STANDBY_MENU;
        }
        else
        {
            PlanMenuType = MAIN_MENU;
        }
        Task |= MEAU_UPDATE;
        break;
    default:
        WM_DefaultProc(pMsg);
    }
}
/*************************************************************************************************************
*
*       _cbDialogValues2
*
*/
static void _cbDialogValues2(WM_MESSAGE * pMsg)
{
    WM_HWIN hDlg;
    TextExCtrl_Handle hObj,mObj;
    int i=0,j=0;
    hDlg = pMsg->hWin;
    switch (pMsg->MsgId)
    {
    case WM_KEY:
        break;
    case WM_INIT_DIALOG:
        hObj = WM_GetDialogItem(hDlg, GUI_ID_TEXT0);
        mObj = hObj;
        do
        {
			if(i == 5)
            {
//              switch(AA_all_data.AA_detail.AA_AA1.ID)
				switch(AA_all_data.AA_detail.AA_AA1.ID )              
				{
                case IM_NO_AGT:
                    j = 0;
                    break;
                case IM_AGT_HAL:
                    j = 1;
                    break;
                case IM_AGT_ENF:
                    j = 2;
                    break;
                case IM_AGT_ISO:
                    j = 3;
                    break;
                case IM_AGT_SEV:
                    j = 4;
                    break;
                case IM_AGT_DES:
                    j = 5;
                    break;
                }
				TextExCtrl_SetTextInfo(mObj,0,Sys_Lib_GetDispStr(AAName[j]),&Unit1[i][0]);
				if(LanguageType == Disp_Lg_Chinese)
				{
					TextExCtrl_SetCtrlFont(mObj,&GUI_Font_15);
				}
				else
				{
					TextExCtrl_SetCtrlFont(mObj,&GUI_Font_16);
				}
            }
            else if(i == 6)
            {
				switch(AA_all_data.AA_detail.AA_AA1.ID)
                {
                case IM_NO_AGT:
                    j = 6;
                    break;
                case IM_AGT_HAL:
                    j = 7;
                    break;
                case IM_AGT_ENF:
                    j = 8;
                    break;
                case IM_AGT_ISO:
                    j = 9;
                    break;
                case IM_AGT_SEV:
                    j = 10;
                    break;
                case IM_AGT_DES:
                    j = 11;
                    break;
                }
				TextExCtrl_SetTextInfo(mObj,0,Sys_Lib_GetDispStr(AAName[j]),&Unit1[i][0]);
				if(LanguageType == Disp_Lg_Chinese)
				{
					TextExCtrl_SetCtrlFont(mObj,&GUI_Font_15);
				}
				else
				{
					TextExCtrl_SetCtrlFont(mObj,&GUI_Font_16);
				}
            }
            else if(i == 7)
            {
				switch(AA_all_data.AA_detail.AA_AA2.ID)              
                {
                case IM_NO_AGT:
                    j = 12;
                    break;
                case IM_AGT_HAL:
                    j = 1;
                    break;
                case IM_AGT_ENF:
                    j = 2;
                    break;
                case IM_AGT_ISO:
                    j = 3;
                    break;
                case IM_AGT_SEV:
                    j = 4;
                    break;
                case IM_AGT_DES:
                    j = 5;
                    break;
                }
				TextExCtrl_SetTextInfo(mObj,0,Sys_Lib_GetDispStr(AAName[j]),&Unit1[i][0]);
				if(LanguageType == Disp_Lg_Chinese)
				{
					TextExCtrl_SetCtrlFont(mObj,&GUI_Font_15);
				}
				else
				{
					TextExCtrl_SetCtrlFont(mObj,&GUI_Font_16);
				}
            }
            else if(i == 8)
            {
				switch(AA_all_data.AA_detail.AA_AA2.ID)              
                {
                case IM_NO_AGT:
                    j = 13;
                    break;
                case IM_AGT_HAL:
                    j = 7;
                    break;
                case IM_AGT_ENF:
                    j = 8;
                    break;
                case IM_AGT_ISO:
                    j = 9;
                    break;
                case IM_AGT_SEV:
                    j = 10;
                    break;
                case IM_AGT_DES:
                    j = 11;
                    break;
                }
				TextExCtrl_SetTextInfo(mObj,0,Sys_Lib_GetDispStr(AAName[j]),&Unit1[i][0]);
				if(LanguageType == Disp_Lg_Chinese)
				{
					TextExCtrl_SetCtrlFont(mObj,&GUI_Font_15);
				}
				else
				{
					TextExCtrl_SetCtrlFont(mObj,&GUI_Font_16);
				}
            }
            else
            {
				TextExCtrl_SetTextInfo(mObj,0,Sys_Lib_GetDispStr(Name1[i]),&Unit1[i][0]);
				if(LanguageType == Disp_Lg_Chinese)
				{
					TextExCtrl_SetCtrlFont(mObj,&GUI_Font_15);
				}
				else
				{
					TextExCtrl_SetCtrlFont(mObj,&GUI_Font_16);
				}
            }
            TextExCtrl_SetMaxMinData(mObj,ParaMax1[i],ParaMin1[i]);
            TextExCtrl_SetFloatLen(mObj,Floatflag1[i]);
            i++;
            hObj = mObj;
        }
        while((mObj=WM_GetNextSibling(hObj))!=0);
		hObj = WM_GetDialogItem(hDlg,GUI_ID_TEXT1);
		mObj = WM_GetDialogItem(hDlg,GUI_ID_TEXT2);
		switch(ETCO2Unit)
		{
		case 0:
			TextExCtrl_SetTextUnit(hObj,"%");
			TextExCtrl_SetTextUnit(mObj,"%");
			break;
		case 1:
			TextExCtrl_SetTextUnit(hObj,"mmHg");
			TextExCtrl_SetTextUnit(mObj,"mmHg");
			break;
		case 2:
			TextExCtrl_SetTextUnit(hObj,"KPa");
			TextExCtrl_SetTextUnit(mObj,"KPa");
			break;
		default:break;
		}
        break;
    case WM_NOTIFY_PARENT_REFLECTION:
        FunctionKeyPressedResume(Menu_hWin.hWin,0);
        if(StandbyState==0xFF)
        {
            PlanMenuType = STANDBY_MENU;
        }
        else
        {
            PlanMenuType = MAIN_MENU;
        }
        Task |= MEAU_UPDATE;
        break;
    default:
        WM_DefaultProc(pMsg);
    }
}
/***************************************************************************************************
* @brief  Alarms -> Limits1
* @para   
* @retval 
*/
static void _cbDialog3(WM_MESSAGE * pMsg)
{
    WM_HWIN hDlg,hItem,hText0,hText2,hText4,hText6,hText8;
    WM_HWIN hText10,hText12,hText14,hText16,hText17,hText18,hText19;
    WM_HWIN hButton16,tWin;
    int NCode,Id,flg=0;
    int Data;
    WM_MESSAGE Msg;
    hDlg = pMsg->hWin;
    switch (pMsg->MsgId)
    {
    case WM_INIT_DIALOG:
        hItem = WM_GetDialogItem(hDlg, GUI_ID_IMAGETEXT0);
        IMAGETEXT_SetBitmap(hItem,&bmHigh_limit);
        hItem = WM_GetDialogItem(hDlg, GUI_ID_IMAGETEXT1);
        IMAGETEXT_SetBitmap(hItem,&bmLow_limit);
        hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT1);
		TEXT_SetText(hItem, Sys_Lib_GetDispStr(DP_CMH2O));//cmH2O
        TEXT_SetFont(hItem,&GUI_Font_15);
        hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT3);
		TEXT_SetText(hItem, Sys_Lib_GetDispStr(DP_LMIN));//L
        TEXT_SetFont(hItem,&GUI_Font_15);
        hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT5);
		TEXT_SetText(hItem, Sys_Lib_GetDispStr(DP_BPM));//bpm
        TEXT_SetFont(hItem,&GUI_Font_15);
        hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT7);
		TEXT_SetText(hItem, Sys_Lib_GetDispStr(DP_PERCENT));//%
        TEXT_SetFont(hItem,&GUI_Font_15);
        hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT9);
		TEXT_SetText(hItem, Sys_Lib_GetDispStr(DP_ML));//mL
        TEXT_SetFont(hItem,&GUI_Font_15);
        hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT11);
		TEXT_SetText(hItem, Sys_Lib_GetDispStr(DP_S));//Sec
        TEXT_SetFont(hItem,&GUI_Font_15);
        hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT13);
        TEXT_SetFont(hItem,&GUI_Font_15);
        hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT15);
        TEXT_SetFont(hItem,&GUI_Font_15);
        hText0 = WM_GetDialogItem(hDlg,GUI_ID_TEXT0);
        hText2 = WM_GetDialogItem(hDlg,GUI_ID_TEXT2);
        hText4 = WM_GetDialogItem(hDlg,GUI_ID_TEXT4);
        hText6 = WM_GetDialogItem(hDlg,GUI_ID_TEXT6);
        hText8 = WM_GetDialogItem(hDlg,GUI_ID_TEXT8);
        hText10 = WM_GetDialogItem(hDlg,GUI_ID_TEXT10);
        hText12 = WM_GetDialogItem(hDlg,GUI_ID_TEXT12);
        hText14 = WM_GetDialogItem(hDlg,GUI_ID_TEXT14);
        hText16 = WM_GetDialogItem(hDlg,GUI_ID_TEXT16);
        hText17 = WM_GetDialogItem(hDlg,GUI_ID_TEXT17);
        hText18 = WM_GetDialogItem(hDlg,GUI_ID_TEXT18);
        hText19 = WM_GetDialogItem(hDlg,GUI_ID_TEXT19);
        AlarmLimitParaInit(hDlg,&AlarmSettingPara,0);
        hButton16 = WM_GetDialogItem(hDlg,GUI_ID_BUTTON0);
        WM_SetFocus(hButton16);
        //hButton17 = WM_GetDialogItem(hDlg,GUI_ID_BUTTON17);
        //hButton20 = WM_GetDialogItem(hDlg,GUI_ID_BUTTON20);
// #ifndef LANGUAGE_CHINESE_FLAG
// 		WM_MoveChildTo(hText0, 114,12);
// 		WM_MoveChildTo(hText2, 176,12);
// 		WM_MoveChildTo(hText4, 250,12);
// 		WM_MoveChildTo(hText6, 319,12);
// 		WM_MoveChildTo(hText8, 390,12);
// 		WM_MoveChildTo(hText10,481,12);
// #endif
		// 氧浓度传感器选配
		hItem = WM_GetDialogItem(hDlg,GUI_ID_BUTTON6);
		if(!FiO2_Config_On)
		{
			WM_DisableWindow(hItem);
		}
		else
		{
			WM_EnableWindow(hItem);
		}
		hItem = WM_GetDialogItem(hDlg,GUI_ID_BUTTON7);
		if(!FiO2_Config_On)
		{
			WM_DisableWindow(hItem);
		}
		else
		{
			WM_EnableWindow(hItem);
		}

        TEXT_SetText(hText0, Sys_Lib_GetDispStr(DP_PRESSURE));
        TEXT_SetText(hText2, Sys_Lib_GetDispStr(DP_MV));
        TEXT_SetText(hText4, Sys_Lib_GetDispStr(DP_F));
        TEXT_SetText(hText6, Sys_Lib_GetDispStr(DP_FIO2));
        TEXT_SetText(hText8, Sys_Lib_GetDispStr(DP_VTE));
        TEXT_SetText(hText10, Sys_Lib_GetDispStr(DP_APNEATIME));
        break;
    case WM_NOTIFY_PARENT_REFLECTION:
        FunctionKeyPressedResume(Menu_hWin.hWin,0);
        if(StandbyState==0xFF)
        {
            PlanMenuType = STANDBY_MENU;
        }
        else
        {
            PlanMenuType = MAIN_MENU;
        }
        Task |= MEAU_UPDATE;
        break;
    case WM_NOTIFY_PARENT:
        NCode = pMsg->Data.v;
        Id = WM_GetId(pMsg->hWinSrc);
        switch(NCode)
        {
        case WM_NOTIFICATION_LOST_FOCUS: //  20130604
            tWin = WM_GetDialogItem(hDlg,GUI_ID_BUTTON0);
            while(!ButtonRotation_GetTriangleSign(tWin))
            {
                tWin = WM_GetNextSibling(tWin);
                if(tWin==0)
                {
                    flg = 1;
                    break;
                }
            }
            if(!flg)
            {
                ButtonRotation_SetTriangleSign(tWin,0);
            }
            break;
            
        case WM_NOTIFICATION_CLICKED:
        case GUI_KEY_ENTER:
            if(ButtonRotation_GetTriangleSign(pMsg->hWinSrc))//修改完参数 cpp ac
            {
                ButtonRotation_SetTriangleSign(pMsg->hWinSrc,0);
                Data = ButtonRotation_GetData1(pMsg->hWinSrc);
                ButtonRotation_SetData(pMsg->hWinSrc,Data);
                AlarmSettingValueUpdate(Data,0,&AlarmSettingPara,&Menu_hWin.HomeWin,StandbyState,Id);
                //SendSetParameter(Data,Id);//transmit the parameter to control mcu
            }
            else //进入修改参数 cpp ac
            {
                tWin = WM_GetDialogItem(hDlg,GUI_ID_BUTTON0);
                while(!ButtonRotation_GetTriangleSign(tWin))
                {
                    tWin = WM_GetNextSibling(tWin);
                    if(tWin==0)
                    {
                        flg = 1;
                        break;
                    }
                }
                if(!flg)
                {
                    ButtonRotation_SetTriangleSign(tWin,0);
                }
                Data = ButtonRotation_GetData(pMsg->hWinSrc);
                ButtonRotation_SetData1(pMsg->hWinSrc,Data);
                WM_SetFocus(pMsg->hWinSrc);
                ButtonRotation_SetTriangleSign(pMsg->hWinSrc,1); //设置按钮为按下 //cpp ac
            }
            break;
        case GUI_KEY_LEFT:
            if(ButtonRotation_GetTriangleSign(pMsg->hWinSrc))//if setting now?
            {
                if(AlarmParameterLimitAnalysis(pMsg->hWinSrc,0,GUI_KEY_LEFT,&AlarmSettingPara))
                {
                    Msg.MsgId   = WM_NOTIFICATION_VALUE_CHANGED;
                    Msg.Data.v  = GUI_KEY_LEFT;
                    Msg.hWin = pMsg->hWinSrc;
                    WM_SendMessage(pMsg->hWinSrc,&Msg);
                }
            }
            break;
        case GUI_KEY_RIGHT:
            if(ButtonRotation_GetTriangleSign(pMsg->hWinSrc))//if setting now?
            {
                if(AlarmParameterLimitAnalysis(pMsg->hWinSrc,0,GUI_KEY_RIGHT,&AlarmSettingPara))
                {
                    Msg.MsgId   = WM_NOTIFICATION_VALUE_CHANGED;
                    Msg.Data.v  = GUI_KEY_RIGHT;
                    Msg.hWin = pMsg->hWinSrc;
                    WM_SendMessage(pMsg->hWinSrc,&Msg);
                }
            }
            break;
        }
        break;
    default:
        WM_DefaultProc(pMsg);
    }
}
static void _cbDialogSettingMenu_Compensate(WM_MESSAGE * pMsg)
{
	WM_HWIN hDlg, hButton0, hText3, hText4, hText7, hDropdown0, hDropdown1;
	int Id,NCode;
	GUI_ReduceRect ReduceRect;
	WM_MESSAGE Msg;
	Int8U pText[20];
	int num;
	hDlg = pMsg->hWin;

	hDropdown0	= WM_GetDialogItem(hDlg, GUI_ID_DROPDOWN0);
	hDropdown1	= WM_GetDialogItem(hDlg, GUI_ID_DROPDOWN1); 
	switch (pMsg->MsgId)
	{ 
	case WM_INIT_DIALOG:
		ReduceRect.Reducex0 = 137;//相对于窗体左侧缩小的距离
		ReduceRect.Reducey0 = 22;
		ReduceRect.Reducex1 = 8;
		ReduceRect.Reducey1 = 8;//
		//WINDOW_SetBkColor(hDlg,GUI_BLUE);
		WINDOW_SetFrameStyle(hDlg,GUI_FRAMEARCRECT);
		WINDOW_SetFrameReduceRect(hDlg,ReduceRect,8);
		
		hButton0 = WM_GetDialogItem(hDlg,GUI_ID_BUTTON0);//退出界面
		BUTTON_SetBkColor(hButton0,0,0xD05356);
		BUTTON_SetBitmapEx(hButton0,0,&bmarrows_left,10,11);

		hText3 = WM_GetDialogItem(hDlg, GUI_ID_TEXT3);
		TEXT_SetFont(hText3,&GUI_Font_16);
		TEXT_SetText(hText3,Sys_Lib_GetDispStr(DP_O2));

		DROPDOWN_SetFont(hDropdown0,&GUI_Font_16);
		DROPDOWN_SetAutoScroll(hDropdown0,1);
		DROPDOWN_SetScrollbarWidth(hDropdown0,27);
		DROPDOWN_SetItemSpacing(hDropdown0,8);
		DROPDOWN_SetTextHeight(hDropdown0,25);

		num = DROPDOWN_GetNumItems(hDropdown0);
		while((num--) >= 1)
		{
			DROPDOWN_DeleteItem(hDropdown0,num);
		}

		if(!DROPDOWN_GetNumItems(hDropdown0))
		{
			DROPDOWN_AddString(hDropdown0, Sys_Lib_GetDispStr(DP_HIGH));
			DROPDOWN_AddString(hDropdown0, Sys_Lib_GetDispStr(DP_MEDIUM));
			DROPDOWN_AddString(hDropdown0, Sys_Lib_GetDispStr(DP_LOW));
		}
		DROPDOWN_SetScrollbarWidth(hDropdown0, 5);
		DROPDOWN_SetSel(hDropdown0,0);

		hText4 = WM_GetDialogItem(hDlg, GUI_ID_TEXT4);
		TEXT_SetFont(hText4,&GUI_Font_16);
		TEXT_SetText(hText4,Sys_Lib_GetDispStr(DP_N2O));

		DROPDOWN_SetFont(hDropdown1,&GUI_Font_16);
		DROPDOWN_SetItemSpacing(hDropdown1,8);
		DROPDOWN_SetTextHeight(hDropdown1,25);

		num = DROPDOWN_GetNumItems(hDropdown1);
		while((num--) >= 1)
		{
			DROPDOWN_DeleteItem(hDropdown1,num);
		}

		if(!DROPDOWN_GetNumItems(hDropdown1))
		{
			DROPDOWN_AddString(hDropdown1,Sys_Lib_GetDispStr(DP_ON));
			DROPDOWN_AddString(hDropdown1,Sys_Lib_GetDispStr(DP_OFF));
		}
		DROPDOWN_SetSel(hDropdown1,0);

		if((Masimo_EtCO2_Config_On) && (AA_Module_Flag.PhaseInTypeModuleExist == 1)//IRMA AX+
			|| (MasimoISA_EtCO2_Config_On == 1))//ISA 无O2
		{
			WM_HideWindow(hText4);
			WM_HideWindow(hDropdown1);
		}
		else
		{
			WM_ShowWindow(hText4);
			WM_ShowWindow(hDropdown1);
		}

		hText7 = WM_GetDialogItem(hDlg, GUI_ID_TEXT7);
		TEXT_SetFont(hText7,&GUI_Font_16);
		TEXT_SetText(hText7, Sys_Lib_GetDispStr(DP_COMPENSATION));
		TEXT_SetBkColor(hText7,GUI_YAN_BLUE);
		break;
	case WM_NOTIFY_PARENT:
		NCode = pMsg->Data.v;
		Id = WM_GetId(pMsg->hWinSrc);
		switch(NCode)
		{
		case WM_NOTIFICATION_CLICKED:
			switch(Id)
			{
			case GUI_ID_DROPDOWN0:
			case GUI_ID_DROPDOWN1:
				DROPDOWN_SetState(WM_GetDialogItem(hDlg,GUI_ID_DROPDOWN0), 0);
				DROPDOWN_SetState(WM_GetDialogItem(hDlg,GUI_ID_DROPDOWN1), 0);
				DROPDOWN_SetState(pMsg->hWinSrc,1);
				break;
			case GUI_ID_BUTTON0:
				BUTTON_SetPressed(pMsg->hWinSrc,0);
				WM_HideWindow(hComp);
				break;
			}
			break;
		case GUI_KEY_ENTER:
		case WM_NOTIFICATION_SEL_CHANGED:
			if(DROPDOWN_GetState(hDropdown0))
			{
				int index = 0;
				DROPDOWN_SetState(hDropdown0,0);
				index = DROPDOWN_GetLstWin(hDropdown0);
				Id = LISTBOX_GetSel(index);
				DROPDOWN_SetSel(hDropdown0,Id);
				DROPDOWN_Collapse(hDropdown0);
				if(Id == 0)
				{
					DataTransmit(85,IM_SET_O2VOL,Usart2_Tx_Buffer,USART2);
				}
				else if(Id == 1)
				{
					DataTransmit(50,IM_SET_O2VOL,Usart2_Tx_Buffer,USART2);
				}
				else
				{
					DataTransmit(21,IM_SET_O2VOL,Usart2_Tx_Buffer,USART2);
				}
			}
			hDropdown1 = WM_GetDialogItem(hDlg, GUI_ID_DROPDOWN1);//
			if(DROPDOWN_GetState(hDropdown1))
			{
				int index = 0;
				DROPDOWN_SetState(hDropdown1,0);
				index = DROPDOWN_GetLstWin(hDropdown1);
				Id = LISTBOX_GetSel(index);
				DROPDOWN_SetSel(hDropdown1,Id);
				DROPDOWN_Collapse(hDropdown1);
				if(Id == 0)
				{
					DataTransmit(50,IM_SET_N2OVOL,Usart2_Tx_Buffer,USART2);
				}
				else
				{
					DataTransmit(0,IM_SET_N2OVOL,Usart2_Tx_Buffer,USART2);
				}
			}
			break;
		case GUI_KEY_LEFT:
			if(!Id)
			{
				if(DROPDOWN_GetState(hDropdown0))
				{
					LISTBOX_DecSel(pMsg->hWinSrc);
				}
				if(DROPDOWN_GetState(hDropdown1))
				{
					LISTBOX_DecSel(pMsg->hWinSrc);
				}         
			}
			else 
			{
			}
			break;  
		case GUI_KEY_RIGHT:
			if(!Id)
			{
				if(DROPDOWN_GetState(hDropdown0))
				{
					LISTBOX_IncSel(pMsg->hWinSrc);
				}
				if(DROPDOWN_GetState(hDropdown1))
				{
					LISTBOX_IncSel(pMsg->hWinSrc);
				} 
			}
			else
			{
			}
			break;
		}
		break;
	default:
		WM_DefaultProc(pMsg);
		break;
	}
}

static void _cbDialogSettingMenu_SpO2(WM_MESSAGE * pMsg)
{
	WM_HWIN hDlg, hText0, hText1, hText2, hButton0, hButton1, hButton2;
	WM_HWIN hText3, hText4, hText5, hText6, hText7, hDropdown0, hDropdown1;
	int Id,NCode;
	GUI_ReduceRect ReduceRect;
	Int8U Data, Update_SPO2Wave_Flag = 0;
	Int8U Dropdown_EnterFlag = 0;//0：点击鼠标选择   1：按下飞梭选择
	WM_MESSAGE Msg;
	Int8U pText[20];
	int num;
	hDlg = pMsg->hWin;

	hDropdown0	= WM_GetDialogItem(hDlg, GUI_ID_DROPDOWN0);
	hDropdown1	= WM_GetDialogItem(hDlg, GUI_ID_DROPDOWN1); 
	hButton1	= WM_GetDialogItem(hDlg, GUI_ID_BUTTON1	 );
	hButton2	= WM_GetDialogItem(hDlg, GUI_ID_BUTTON2	 );
	switch (pMsg->MsgId)
	{ 
	case WM_INIT_DIALOG:
		ReduceRect.Reducex0 = 137;//相对于窗体左侧缩小的距离
		ReduceRect.Reducey0 = 22;
		ReduceRect.Reducex1 = 8;
		ReduceRect.Reducey1 = 8;//
		//WINDOW_SetBkColor(hDlg,GUI_BLUE);
		WINDOW_SetFrameStyle(hDlg,GUI_FRAMEARCRECT);
		WINDOW_SetFrameReduceRect(hDlg,ReduceRect,8);
		hText0 = WM_GetDialogItem(hDlg, GUI_ID_TEXT0);
		TEXT_SetText(hText0,Sys_Lib_GetDispStr(DP_SPO2VERSION));

		hText1 = WM_GetDialogItem(hDlg, GUI_ID_TEXT1);
		TEXT_SetTextColor(hText1,0xD65252);
		hText2 = WM_GetDialogItem(hDlg, GUI_ID_TEXT2);
		TEXT_SetTextColor(hText2,0xD65252);

		hButton0 = WM_GetDialogItem(hDlg,GUI_ID_BUTTON0);
		BUTTON_SetBkColor(hButton0,0,0xD05356);
		BUTTON_SetBitmapEx(hButton0,0,&bmarrows_left,10,11);

		hText3 = WM_GetDialogItem(hDlg, GUI_ID_TEXT3);
		TEXT_SetText(hText3,Sys_Lib_GetDispStr(DP_AVERAGETIME));

		DROPDOWN_SetFont(hDropdown0,&GUI_Font_16);
		DROPDOWN_SetAutoScroll(hDropdown0,1);
		DROPDOWN_SetScrollbarWidth(hDropdown0,27);
		DROPDOWN_SetItemSpacing(hDropdown0,8);
		DROPDOWN_SetTextHeight(hDropdown0,25);

		num = DROPDOWN_GetNumItems(hDropdown0);
		while((num--) >= 1)
		{
			DROPDOWN_DeleteItem(hDropdown0,num);
		}
		if(!DROPDOWN_GetNumItems(hDropdown0))
		{
			DROPDOWN_AddString(hDropdown0,"  2 - 4");
			DROPDOWN_AddString(hDropdown0,"  4 - 6");
			DROPDOWN_AddString(hDropdown0,"  8");
			DROPDOWN_AddString(hDropdown0,"  10");
			DROPDOWN_AddString(hDropdown0,"  12");
			DROPDOWN_AddString(hDropdown0,"  14");
			DROPDOWN_AddString(hDropdown0,"  16");
		}
		DROPDOWN_SetScrollbarWidth(hDropdown0, 5);
		DROPDOWN_SetSel(hDropdown0,Masimo_AverageTime);

		hText4 = WM_GetDialogItem(hDlg, GUI_ID_TEXT4);
		TEXT_SetText(hText4,Sys_Lib_GetDispStr(DP_SENSITIVITY));
		
 		DROPDOWN_SetFont(hDropdown1,&GUI_Font_16);
		DROPDOWN_SetItemSpacing(hDropdown1,8);
		DROPDOWN_SetTextHeight(hDropdown1,25);

		num = DROPDOWN_GetNumItems(hDropdown1);
		while((num--) >= 1)
		{
			DROPDOWN_DeleteItem(hDropdown1,num);
		}
		if(!DROPDOWN_GetNumItems(hDropdown1))
		{
			DROPDOWN_AddString(hDropdown1,Sys_Lib_GetDispStr(DP_MAX));
			DROPDOWN_AddString(hDropdown1,Sys_Lib_GetDispStr(DP_NORMAL));
			DROPDOWN_AddString(hDropdown1,"APOD");
		}
		DROPDOWN_SetSel(hDropdown1,Masimo_Sensitivity);

		hText5 = WM_GetDialogItem(hDlg, GUI_ID_TEXT5);
		TEXT_SetText(hText5,Sys_Lib_GetDispStr(DP_SMARTTONE));
		
		ButtonRotation_SetDataInfo(hButton1,0,1,(!Masimo_SmartToneSwitch)&0x01,1,0);
		ButtonRotation_SetData(hButton1,(!Masimo_SmartToneSwitch)&0x01);
		ButtonRotation_SetDataInvildSign(hButton1,3);

		hText6 = WM_GetDialogItem(hDlg, GUI_ID_TEXT6);
		TEXT_SetText(hText6,Sys_Lib_GetDispStr(DP_FASTSAT));
		ButtonRotation_SetDataInfo(hButton2,0,1,(!Masimo_FastSATSwitch)&0x01,1,0);
		ButtonRotation_SetData(hButton2,(!Masimo_FastSATSwitch)&0x01);
		ButtonRotation_SetDataInvildSign(hButton2,3);

		hText7 = WM_GetDialogItem(hDlg, GUI_ID_TEXT7);
		TEXT_SetText(hText7, Sys_Lib_GetDispStr(DP_SPO2));
		TEXT_SetBkColor(hText7,GUI_YAN_BLUE);

		if(Masimo_AverageTime <= 1)
		{
			Masimo_FastSATSwitch = 1;
			WM_DisableWindow(hButton2);
			ButtonRotation_SetData(hButton2,(!Masimo_FastSATSwitch)&0x01);
			RTEEPROMwrite(MASIMO_FAST_SAT_ADDR,Masimo_FastSATSwitch);
		}
		else
		{
			WM_EnableWindow(hButton2);
		}
		if((SpO2_Choose == 2)&&(SpO2ModuleExist))
		{
			DSPFirmVersionCode();
			MCUFirmVersionCode();
			sprintf(pText,"DSP:%2d.%2d.%2d.%2d"\
				,SpO2_Parameters.DSPFirmVersion
				,SpO2_Parameters.DSPFirmVerMajor
				,SpO2_Parameters.DSPFirmVerMinor
				,SpO2_Parameters.DSPFirmVerPatch);
			TEXT_SetText(hText1,pText);

			sprintf(pText,"MCU:%2d.%2d.%2d.%2d"
				,SpO2_Parameters.MCUFirmVersion
				,SpO2_Parameters.MCUFirmVerMajor
				,SpO2_Parameters.MCUFirmVerMinor
				,SpO2_Parameters.MCUFirmVerPatch);
			TEXT_SetText(hText2,pText);
		}
		else
		{
			TEXT_SetText(hText1,"DSP: --");
			TEXT_SetText(hText2,"MCU: --");
		}
		TEXT_SetFont(hText0,&GUI_Font_16);
		TEXT_SetFont(hText1,&GUI_Font_16);
		TEXT_SetFont(hText2,&GUI_Font_16);
		TEXT_SetFont(hText3,&GUI_Font_16);
		TEXT_SetFont(hText4,&GUI_Font_16);
		TEXT_SetFont(hText5,&GUI_Font_16);		
		TEXT_SetFont(hText6,&GUI_Font_16);
		TEXT_SetFont(hText7,&GUI_Font_16);
		break;
	case WM_NOTIFY_PARENT:
		NCode = pMsg->Data.v;
		Id = WM_GetId(pMsg->hWinSrc);
		switch(NCode)
		{
		case WM_NOTIFICATION_LOST_FOCUS:
			ButtonRotation_SetTriangleSign(hButton1, 0);
			ButtonRotation_SetTriangleSign(hButton2, 0);
			break;
		case WM_NOTIFICATION_CLICKED:
				switch(Id)
				{
				case GUI_ID_DROPDOWN0:
				case GUI_ID_DROPDOWN1:
					ButtonRotation_SetTriangleSign(WM_GetDialogItem(hDlg,GUI_ID_BUTTON1), 0);
					ButtonRotation_SetTriangleSign(WM_GetDialogItem(hDlg,GUI_ID_BUTTON2), 0);
					DROPDOWN_SetState(WM_GetDialogItem(hDlg,GUI_ID_DROPDOWN0), 0);
					DROPDOWN_SetState(WM_GetDialogItem(hDlg,GUI_ID_DROPDOWN1), 0);
					DROPDOWN_SetState(pMsg->hWinSrc,1);
					break;
				case GUI_ID_BUTTON0:
					BUTTON_SetPressed(pMsg->hWinSrc,0);
					WM_HideWindow(hSpO2);
// 					WM_DeleteWindow(hSpO2);
// 					hSpO2 = 0;
					break;
				case GUI_ID_BUTTON1:
					if(ButtonRotation_GetTriangleSign(hButton1))
					{
						WM_SelectWindow(hButton1);
						ButtonRotation_SetTriangleSign(hButton1,0);
						Data = ButtonRotation_GetData1(hButton1);
						ButtonRotation_SetData(hButton1,Data);
						Masimo_SmartToneSwitch = ((!Data)&0x01);
						RTEEPROMwrite(MASIMO_SMART_TONE_ADDR,Masimo_SmartToneSwitch);
						if(SpO2ModuleExist&&(SpO2_Choose == 2))
						{
							Update_SPO2Wave_Flag = 1;
							SPO2Module_SendSmartToneONOrOff(Masimo_SmartToneSwitch);
						}
					}
					else
					{
						WM_SetFocus(hButton1);
						Data = ButtonRotation_GetData(hButton1);
						ButtonRotation_SetData1(hButton1,Data);
						ButtonRotation_SetTriangleSign(hButton1,1);
					}
					break;
				case GUI_ID_BUTTON2:
					if(ButtonRotation_GetTriangleSign(hButton2))
					{
						WM_SelectWindow(hButton2);
						ButtonRotation_SetTriangleSign(hButton2,0);
						Data = ButtonRotation_GetData1(hButton2);
						ButtonRotation_SetData(hButton2,Data);
						Masimo_FastSATSwitch = ((!Data)&0x01);
						RTEEPROMwrite(MASIMO_FAST_SAT_ADDR,Masimo_FastSATSwitch);
						if(SpO2ModuleExist&&(SpO2_Choose == 2))
						{
							Update_SPO2Wave_Flag = 1;
							SPO2Modue_SendSensitivity(Masimo_Sensitivity,Masimo_FastSATSwitch);
						}
					}
					else
					{
						WM_SetFocus(hButton2);
						Data = ButtonRotation_GetData(hButton2);
						ButtonRotation_SetData1(hButton2,Data);
						ButtonRotation_SetTriangleSign(hButton2,1);
					}
					break;
				}
			break;
		case GUI_KEY_ENTER:
			if(!Id)
			{
				if(DROPDOWN_GetState(hDropdown0) || DROPDOWN_GetState(hDropdown1))
				{
					Dropdown_EnterFlag = 1;
				}
			}
			else
			{
				if(Id == GUI_ID_BUTTON1)
				{
					if(ButtonRotation_GetTriangleSign(hButton1))
					{
						WM_SelectWindow(hButton1);
						ButtonRotation_SetTriangleSign(hButton1,0);
						Data = ButtonRotation_GetData1(hButton1);
						ButtonRotation_SetData(hButton1,Data);
						Masimo_SmartToneSwitch = ((!Data)&0x01);
						RTEEPROMwrite(MASIMO_SMART_TONE_ADDR,Masimo_SmartToneSwitch);
						if(SpO2ModuleExist&&(SpO2_Choose == 2))
						{
							Update_SPO2Wave_Flag = 1;
							SPO2Module_SendSmartToneONOrOff(Masimo_SmartToneSwitch);
						}
					}
					else
					{
						WM_SetFocus(hButton1);
						Data = ButtonRotation_GetData(hButton1);
						ButtonRotation_SetData1(hButton1,Data);
						ButtonRotation_SetTriangleSign(hButton1,1);
					}
				}
				else if(Id == GUI_ID_BUTTON2)
				{
					if(ButtonRotation_GetTriangleSign(hButton2))
					{
						WM_SelectWindow(hButton2);
						ButtonRotation_SetTriangleSign(hButton2,0);
						Data = ButtonRotation_GetData1(hButton2);
						ButtonRotation_SetData(hButton2,Data);
						Masimo_FastSATSwitch = ((!Data)&0x01);
						RTEEPROMwrite(MASIMO_FAST_SAT_ADDR,Masimo_FastSATSwitch);
						if(SpO2ModuleExist&&(SpO2_Choose == 2))
						{
							Update_SPO2Wave_Flag = 1;
							SPO2Modue_SendSensitivity(Masimo_Sensitivity,Masimo_FastSATSwitch);
						}
					}
					else
					{
						WM_SetFocus(hButton2);
						Data = ButtonRotation_GetData(hButton2);
						ButtonRotation_SetData1(hButton2,Data);
						ButtonRotation_SetTriangleSign(hButton2,1);
					}
				}
				break;
			}
		case WM_NOTIFICATION_SEL_CHANGED:
			if(DROPDOWN_GetState(hDropdown0))
			{
				if(!Dropdown_EnterFlag)//点击选择
				{
					DROPDOWN_SetState(hDropdown0,0);
					Id = DROPDOWN_GetSel(hDropdown0);
				}
				else//按下飞梭选择
				{
					int index = 0;
					DROPDOWN_SetState(hDropdown0,0);
					index = DROPDOWN_GetLstWin(hDropdown0);
					Id = LISTBOX_GetSel(index);
					DROPDOWN_SetSel(hDropdown0,Id);
					DROPDOWN_Collapse(hDropdown0);
					Dropdown_EnterFlag = 0;
				}

				Masimo_AverageTime = Id;
				if(Id <= 1)
				{
					Masimo_FastSATSwitch = 1;
					WM_DisableWindow(hButton2);
					ButtonRotation_SetData(hButton2,(!Masimo_FastSATSwitch)&0x01);
					RTEEPROMwrite(MASIMO_FAST_SAT_ADDR,Masimo_FastSATSwitch);
				}
				else
				{
					WM_EnableWindow(hButton2);
				}

				RTEEPROMwrite(MASIMO_AVERAGE_TIME_ADDR,Masimo_AverageTime);
				if(SpO2ModuleExist&&(SpO2_Choose == 2))
				{
					SPO2Modue_SendSensitivity(Masimo_Sensitivity,Masimo_FastSATSwitch);
					SPO2Modue_SendAverageTime(Masimo_AverageTime);
				}
				if(Masimo_AverageTime <= 1)
				{
				}
				else
				{
					if(SpO2ModuleExist&&(SpO2_Choose == 2))
					{
						SPO2Modue_SendSensitivity(Masimo_Sensitivity,Masimo_FastSATSwitch);
					}
				}
				Update_SPO2Wave_Flag = 1;
			}
			hDropdown1 = WM_GetDialogItem(hDlg, GUI_ID_DROPDOWN1);//
			if(DROPDOWN_GetState(hDropdown1))
			{
				if(!Dropdown_EnterFlag)//点击选择
				{
					DROPDOWN_SetState(hDropdown1,0);
					Id = DROPDOWN_GetSel(hDropdown1);
				}
				else//按下飞梭选择
				{
					int index = 0;
					DROPDOWN_SetState(hDropdown1,0);
					index = DROPDOWN_GetLstWin(hDropdown1);
					Id = LISTBOX_GetSel(index);
					DROPDOWN_SetSel(hDropdown1,Id);
					DROPDOWN_Collapse(hDropdown1);
					Dropdown_EnterFlag = 0;
				}

				Masimo_Sensitivity = Id;
				if(!Masimo_Sensitivity)
				{
					RTEEPROMwrite(MASIMO_SENSITIVITY_ADDR,2);//如果当前选择为Max，则重新开机后默认为apod
				}
				else
				{
					RTEEPROMwrite(MASIMO_SENSITIVITY_ADDR,Masimo_Sensitivity);
				}

				if(SpO2ModuleExist&&(SpO2_Choose == 2))
				{
					Update_SPO2Wave_Flag = 1;
					SPO2Modue_SendSensitivity(Masimo_Sensitivity,Masimo_FastSATSwitch);
				}
			}
			break;
		case GUI_KEY_LEFT:
			if(!Id)
			{
				if(DROPDOWN_GetState(hDropdown0))
				{
					LISTBOX_DecSel(pMsg->hWinSrc);
				}
				if(DROPDOWN_GetState(hDropdown1))
				{
					LISTBOX_DecSel(pMsg->hWinSrc);
				}         
			}
			else 
			{
				switch(Id)
				{
				case GUI_ID_BUTTON1:
				case GUI_ID_BUTTON2:
					if(ButtonRotation_GetTriangleSign(pMsg->hWinSrc))//if setting now?
					{
						ButtonRotation_DataSub(pMsg->hWinSrc);
						Msg.MsgId   = WM_NOTIFICATION_VALUE_CHANGED;
						Msg.Data.v  = GUI_KEY_LEFT;
						Msg.hWin = pMsg->hWinSrc;
						WM_SendMessage(pMsg->hWinSrc,&Msg);
					}
					break;
				default:
					break;
				}
			}
			break;  
		case GUI_KEY_RIGHT:
			if(!Id)
			{
				if(DROPDOWN_GetState(hDropdown0))
				{
					LISTBOX_IncSel(pMsg->hWinSrc);
				}
				if(DROPDOWN_GetState(hDropdown1))
				{
					LISTBOX_IncSel(pMsg->hWinSrc);
				} 
			}
			else
			{
				switch(Id)
				{
				case GUI_ID_BUTTON1:
				case GUI_ID_BUTTON2:
					if(ButtonRotation_GetTriangleSign(pMsg->hWinSrc))//if setting now?
					{
						ButtonRotation_DataAdd(pMsg->hWinSrc);
						Msg.MsgId   = WM_NOTIFICATION_VALUE_CHANGED;
						Msg.Data.v  = GUI_KEY_RIGHT;
						Msg.hWin = pMsg->hWinSrc;
						WM_SendMessage(pMsg->hWinSrc,&Msg);
					}
					break;
				default:
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
#ifdef WAVEFORM_SPO2
	if(Update_SPO2Wave_Flag)
	{
		Update_SPO2Wave_Flag = 0;
		if(StandbyState==0) //非待机状态
		{
			if(Menu_hWin.cWin.WaveForms[0] == WAVEFORM_SPO2)
			{
				if(SpO2_Choose == 2)//masimo
				{
					WM_InvalidateWindow(Menu_hWin.HomeWin.Tobj[0]);
				}
			}
			else if(Menu_hWin.cWin.WaveForms[1] == WAVEFORM_SPO2)
			{
				if(SpO2_Choose == 2)//masimo
				{
					WM_InvalidateWindow(Menu_hWin.HomeWin.Tobj[1]);
				}
			}
		}
	}
#endif
}

/***************************************************************************************************
* @brief  System -> Settings
* @para   
* @retval 
*/

static void _cbDialog4(WM_MESSAGE * pMsg)
{
    WM_HWIN hDlg, hItem,hItem1,hButton0,hButton1,hButton2,hButton3;
    WM_HWIN hButton4,hButton5,hButton6,hButton7,hButton8,hButton9,hButton10,hButton11;
    WM_HWIN hText0,hText1,hText2,hText3,hText4,hText5,hText6,hText7,i;
    unsigned char MasimoMenu_Flag = 0;
    static unsigned char value = 0;
    static unsigned char ShowFlagNum = 0;
    int Data;
    char *pText;
	char pText1[10], pText2[10],pText3[10],pText4[10];//  [2/18/2022 yaoyao]

    int Id,NCode,num=0,flg = 0;
    WM_MESSAGE Msg;
    hDlg = pMsg->hWin;
    //
    switch (pMsg->MsgId)
    {
    case WM_INIT_DIALOG:
        GUI_SetTextMode(GUI_TA_LEFT|GUI_TA_VCENTER);
		/**********麻气选择*******************ylk add 20181206******************************/
		hItem = WM_GetDialogItem(hDlg, GUI_ID_DROPDOWN0);
		DROPDOWN_SetTextHeight(hItem,25);
		DROPDOWN_SetItemSpacing(hItem,7);
		if(!DROPDOWN_GetNumItems(hItem))
		{
			DROPDOWN_AddString(hItem,"NULL");//氟烷
			DROPDOWN_AddString(hItem,Sys_Lib_GetDispStr(DP_HAL));//氟烷
			DROPDOWN_AddString(hItem,Sys_Lib_GetDispStr(DP_ENF));//安氟醚
			DROPDOWN_AddString(hItem,Sys_Lib_GetDispStr(DP_ISO));//异氟醚
			DROPDOWN_AddString(hItem,Sys_Lib_GetDispStr(DP_SEV));//七氟醚
			DROPDOWN_AddString(hItem,Sys_Lib_GetDispStr(DP_DES));//地氟醚
		}
		DROPDOWN_SetSel(hItem,AA_all_data.AA_detail.AA_AA1.ID);
// 		if(Drager_EtCO2_Config_On)
// 		{
// 			WM_ShowWindow(hItem);
// 		}
// 		else
// 		{
			WM_HideWindow(hItem);
// 		}
        //---------------------语言选择---------------------------------------------------------------------//
		hItem = WM_GetDialogItem(hDlg, GUI_ID_DROPDOWN1);
        DROPDOWN_SetTextHeight(hItem,25);
        DROPDOWN_SetItemSpacing(hItem,8);
        if(!DROPDOWN_GetNumItems(hItem))
        {	//少了土耳其语
			DROPDOWN_AddString(hItem, Sys_Lib_GetDispStr(DP_ENGLISH));
			DROPDOWN_AddString(hItem, Sys_Lib_GetDispStr(DP_CHINESE));
#ifndef LANGUAGE_CHINESE_FLAG
			DROPDOWN_AddString(hItem, Sys_Lib_GetDispStr(DP_RUSSIAN));
			DROPDOWN_AddString(hItem, Sys_Lib_GetDispStr(DP_PORTUGUESE));
			DROPDOWN_AddString(hItem, Sys_Lib_GetDispStr(DP_SPANISH));
			DROPDOWN_AddString(hItem, Sys_Lib_GetDispStr(DP_POLISH));
			DROPDOWN_AddString(hItem, Sys_Lib_GetDispStr(DP_FRENCH));
			DROPDOWN_AddString(hItem, Sys_Lib_GetDispStr(DP_GERMAN));
			DROPDOWN_AddString(hItem, Sys_Lib_GetDispStr(DP_ITALIAN));
			DROPDOWN_AddString(hItem, Sys_Lib_GetDispStr(DP_BULGARIAN));
#endif
        }
		DROPDOWN_SetAutoScroll(hItem, 1);
		DROPDOWN_SetScrollbarWidth(hItem, 5);

		DROPDOWN_SetSel(hItem,LanguageType);

        //*//--------------------Trigger Type-------------------------------------------------------------------
        hItem = WM_GetDialogItem(hDlg, GUI_ID_DROPDOWN2);
        DROPDOWN_SetTextHeight(hItem,25);
        DROPDOWN_SetItemSpacing(hItem,14);
        
        num = DROPDOWN_GetNumItems(hItem); //djq 20140103

		while((num--) >= 1)
		{
			DROPDOWN_DeleteItem(hItem,num);
		}

//         while((num--) >= 0)//软件问题-GUI #1241
//         {
// 			DROPDOWN_DeleteItem(hItem,num);
//         }
        
        if(!(num = DROPDOWN_GetNumItems(hItem)))
        {
            DROPDOWN_AddString(hItem,Sys_Lib_GetDispStr(DP_PRESSURETRIGGER));
            DROPDOWN_AddString(hItem,Sys_Lib_GetDispStr(DP_FLOWTRIGGER));           
        }

        DROPDOWN_SetSel(hItem,BeathSettingPara.TriggerMode);//cpp del 20130106 
        //---------------------------CO2 unit---------------------------------------------------------------
        hItem = WM_GetDialogItem(hDlg, GUI_ID_DROPDOWN3);//获得控件的窗口句柄 cpp ac
        DROPDOWN_SetTextHeight(hItem,25);
        DROPDOWN_SetItemSpacing(hItem,14);
        if(!DROPDOWN_GetNumItems(hItem))//不加 if 判断则回调一次添加一次以下选项 cpp ac 
        {
            DROPDOWN_AddString(hItem,Sys_Lib_GetDispStr(DP_PERCENT));
            DROPDOWN_AddString(hItem,Sys_Lib_GetDispStr(DP_MMHG));
            DROPDOWN_AddString(hItem,Sys_Lib_GetDispStr(DP_KPA));
        }
        DROPDOWN_SetSel(hItem,ETCO2Unit);
        //*//-------------------------声音--------------------------------------------------------------
        hButton0 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON0);
        ButtonRotation_SetDataInfo(hButton0,20,100,(AlarmSettingPara.AlarmVolumeValue+1)*20,20,0);
        ButtonRotation_SetData(hButton0,(AlarmSettingPara.AlarmVolumeValue+1)*20);
             ButtonRotation_SetText(hButton0,Sys_Lib_GetDispStr(DP_LOUDNESS),Sys_Lib_GetDispStr(DP_PERCENT));
	//	hText5 = WM_GetDialogItem(hDlg, GUI_ID_TEXT5);//Loudness//软件问题-GUI #1229
//		TEXT_SetText(hText5, Sys_Lib_GetDispStr(DP_LOUDNESS));
	//	TEXT_SetTextWrapMode(hText5, GUI_WRAPMODE_WORD);
        //-----------------------血氧-------------------------------------------------------------------
        hButton1 = WM_GetDialogItem(hDlg,GUI_ID_BUTTON1);
        ButtonRotation_SetText(hButton1,Sys_Lib_GetDispStr(DP_SPO2),"");
        ButtonRotation_SetDataInfo(hButton1,0,1,SpO2ModuleOff,1,0);
        ButtonRotation_SetData(hButton1,SpO2ModuleOff);
        ButtonRotation_SetDataInvildSign(hButton1,3);
        if( (!SpO2ModuleExist) || (!(SpO2_Config_On || Masimo_SpO2_Config_On)))
        {
            WM_DisableWindow(hButton1);
            if(!SpO2ModuleOff)
            {
                SpO2ModuleOff = 1;//无模块显示灰色OFF
				RTEEPROMwrite(SPO2_ON_OFF_ADDR,SpO2ModuleOff);
            }
        }

		hButton10 = WM_GetDialogItem(hDlg,GUI_ID_BUTTON10);
		BUTTON_SetBkColor(hButton10,0,0xD05356);
		BUTTON_SetText(hButton10,Sys_Lib_GetDispStr(DP_MORE));
		BUTTON_SetBitmapEx(hButton10,0,&bmarrows_right,82,11);

		if(Masimo_SpO2_Config_On)
		{
			WM_ShowWindow(hButton10);
		}
		else
		{
			WM_HideWindow(hButton10);
		}
#ifndef SPO2_SWITCH
		WM_HideWindow(hButton1);
		WM_HideWindow(hButton10);
#endif     
		//---------------补偿按钮---------------------------------------------------
		hButton11 = WM_GetDialogItem(hDlg,GUI_ID_BUTTON11);
		BUTTON_SetBkColor(hButton11,0,0xD05356);
		BUTTON_SetText(hButton11,"Comp.");
		BUTTON_SetBitmapEx(hButton11,0,&bmarrows_right,82,11);
		if(Masimo_EtCO2_Config_On || MasimoISA_EtCO2_Config_On)
		{
			if(MasimoISA_EtCO2_Config_On == 2)
			{
				WM_HideWindow(hButton11);
			}
			else
			{
				WM_ShowWindow(hButton11);
			}
		}
		//----麻气开关--------------------------------------------------------------------------------------
        hButton2 = WM_GetDialogItem(hDlg,GUI_ID_BUTTON2);
        
                if(Masimo_EtCO2_Config_On||MasimoISA_EtCO2_Config_On)           //开通主流选配或旁流或全开通			//gyh add 20230727 修改麻气开关判断
                {
                  if(AA_Module_Flag.PhaseInTypeModuleExist == Masimo_EtCO2_zhuliu)      //主流多气体
                  {
                    ButtonRotation_SetText(hButton2,Sys_Lib_GetDispStr(DP_AA),"");
                  }
                  else if(AA_Module_Flag.PhaseInTypeModuleExist == Masimo_EtCO2_zhuliuco2)      //主流CO2
                  {
                    ButtonRotation_SetText(hButton2,Sys_Lib_GetDispStr(DP_CO2),"");
                  }
                  else if(AA_Module_Flag.PhaseInTypeModuleExist == Masimo_EtCO2_pangliuco2)     //旁流CO2
                  {
                    ButtonRotation_SetText(hButton2,Sys_Lib_GetDispStr(DP_CO2),"");
                  }
                  else if((AA_Module_Flag.PhaseInTypeModuleExist == MasimoISA_EtCO2_pangliuO2)       //旁流多气体
                     ||(AA_Module_Flag.PhaseInTypeModuleExist == MasimoISA_EtCO2_pangliu))
                  {
                    ButtonRotation_SetText(hButton2,Sys_Lib_GetDispStr(DP_AA),"");
                  }
                  else                                                                  //默认显示AA
                  {
                    ButtonRotation_SetText(hButton2,Sys_Lib_GetDispStr(DP_AA),"");
                  }
                }
                else                                                                            //金嘉信CO2选配
                {
                  ButtonRotation_SetText(hButton2,Sys_Lib_GetDispStr(DP_CO2),"");
                }
		/*if((Masimo_EtCO2_Config_On) && (AA_Module_Flag.PhaseInTypeModuleExist == 1)
			||(MasimoISA_EtCO2_Config_On) || (Drager_EtCO2_Config_On))//ylk md 20190626
		{

			ButtonRotation_SetText(hButton2,Sys_Lib_GetDispStr(DP_AA),"");
		}
		else if((Masimo_EtCO2_Config_On) && (AA_Module_Flag.PhaseInTypeModuleExist == 3)
			||(MasimoISA_EtCO2_Config_On) || (Drager_EtCO2_Config_On))//gyh add 20230709 旁流多气体模块识别
		{
			ButtonRotation_SetText(hButton2,Sys_Lib_GetDispStr(DP_AA),"");
		}
		else if((Masimo_EtCO2_Config_On) && (AA_Module_Flag.PhaseInTypeModuleExist == 2)
			||(MasimoISA_EtCO2_Config_On) || (Drager_EtCO2_Config_On))//gyh add 20230709 旁流多气体模块识别
		{
			ButtonRotation_SetText(hButton2,Sys_Lib_GetDispStr(DP_AA),"");
		}
                else if((AA_Module_Flag.PhaseInTypeModuleExist == 6) && (Masimo_EtCO2_Config_On)
			||(MasimoISA_EtCO2_Config_On) || (Drager_EtCO2_Config_On))//gyh add 20230709 旁流多气体模块识别
		{
			ButtonRotation_SetText(hButton2,Sys_Lib_GetDispStr(DP_CO2),"");
		}
		else
		{
			ButtonRotation_SetText(hButton2,Sys_Lib_GetDispStr(DP_CO2),"");
		}*/

		if(Drager_EtCO2_Config_On == 2|| MasimoISA_EtCO2_Config_On == 2)
		{
			ButtonRotation_SetText(hButton2,Sys_Lib_GetDispStr(DP_O2AA),"");
		}
		InitAlarmInfoRegister();//报警字符串初始化
        ButtonRotation_SetDataInfo(hButton2,0,1,!EtCO2ModuleOff,1,0);
        ButtonRotation_SetData(hButton2,!EtCO2ModuleOff);
        ButtonRotation_SetDataInvildSign(hButton2,3);
        if((!AA_Module_Flag.EtCO2ModuleExist) \
            || (!(Masimo_EtCO2_Config_On || MasimoISA_EtCO2_Config_On \
                || EtCO2_Config_On || EtCO2_KM7002_Config_On || Drager_EtCO2_Config_On)))
        {
            WM_DisableWindow(hButton2);
            if(EtCO2ModuleOff)
            {
                EtCO2ModuleOff = 0;
				RTEEPROMwrite(ETCO2_ON_OFF_ADDR,EtCO2ModuleOff);//20210909 test
            }
        }

        //--------------------SIGH------------------------------------------------------------------------------------------
        hButton9 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON9);
     //   if (LanguageType == Disp_Lg_French || LanguageType == Disp_Lg_Polish)  //wff add 发语下显示不全 软件问题-GUI #1229
     //   {
           // ButtonRotation_SetFontEx(hButton9, &GUI_Font_15,2);
    //    }
   //     else
    //    {
           // ButtonRotation_SetFontEx(hButton9, &GUI_Font_16,2);
   //     }
        
        ButtonRotation_SetDataInfo(hButton9,50,150,BeathSettingPara.SettingSighTimesValue,25,0);
        ButtonRotation_SetData(hButton9,BeathSettingPara.SettingSighTimesValue);
        ButtonRotation_SetText(hButton9,Sys_Lib_GetDispStr(DP_SIGH),"");
    
        if( AA_Module_Flag.EtCO2ModuleExist != 0 )         
        {
          if((pHaseinMoudleCheck) && (StandbyState == 0))
          {
              WM_DisableWindow(hButton2);
              if(EtCO2ModuleOff)
              {
                  EtCO2ModuleOff = 0;   
				  RTEEPROMwrite(ETCO2_ON_OFF_ADDR,EtCO2ModuleOff);//20210909 test
              }
              AlarmsWork.presentAlarms &= ~ALARM_ETCO2_HIGH;
              AlarmsWork.presentAlarms1 &= ~ALARM_ETCO2_LOW;
              AlarmsWork.presentAlarms1 &= ~ALARM_ETCO2_LINE_OCCLUSION;
#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C) 
              AlarmsWork.presentAlarms &= ~ALARM_FIAA_HIGH;
              AlarmsWork.presentAlarms &= ~ALARM_ETAA_HIGH;
              AlarmsWork.presentAlarms1 &= ~ALARM_FIAA_LOW;
              AlarmsWork.presentAlarms1 &= ~ALARM_ETAA_LOW;
              AlarmsWork.presentAlarms &= ~ALARM_FIN2O_HIGH;
              AlarmsWork.presentAlarms1 &= ~ALARM_FIN2O_LOW;
              AlarmsWork.presentAlarms1 &= ~ALARM_AA_GTE_3MAC;
              AlarmsWork.presentAlarms1 &= ~ALARM_AA_LESSTHAN_3MAC;
#endif
          }
        }
        //----------------------FiO2--------------------------------------------------------------------
        //djq add 20131226
        hButton3 = WM_GetDialogItem(hDlg,GUI_ID_BUTTON3);
		ButtonRotation_SetText(hButton3,Sys_Lib_GetDispStr(DP_FIO2),"");
        ButtonRotation_SetDataInfo(hButton3,0,1,O2ModuleOff,1,0);
        ButtonRotation_SetData(hButton3,O2ModuleOff);
        ButtonRotation_SetDataInvildSign(hButton3,3);

		if(!FiO2_Config_On)//如果没有开启选配  //FiO2选配
		{
			WM_DisableWindow(hButton3);
			ButtonRotation_SetData(hButton3,1);//FiO2选配
		}
		else
		{
			WM_EnableWindow(hButton3);
		}

        //zkq add 20170407--------------------------------------------------------------------------
        hButton4 = WM_GetDialogItem(hDlg,GUI_ID_BUTTON4);//气源开关
        ButtonRotation_SetText(hButton4,Sys_Lib_GetDispStr(DP_AIR),"");
        ButtonRotation_SetDataInfo(hButton4,0,1, 0  ,1,0);
        ButtonRotation_SetData(hButton4,!AlarmAirSourceSwitch);
        ButtonRotation_SetDataInvildSign(hButton4,3);

        hButton5 = WM_GetDialogItem(hDlg,GUI_ID_BUTTON5);//驱动气切换开关
//      ButtonRotation_SetText(hButton5,Sys_Lib_GetDispStr(DP_DRIVER),"");
        ButtonRotation_SetDataInfo(hButton5,0,1, 0 ,1,0);
        ButtonRotation_SetData(hButton5,DirverGasFlag);
        ButtonRotation_SetDataInvildSign(hButton5,4);
		hText5 = WM_GetDialogItem(hDlg,GUI_ID_TEXT6);
		TEXT_SetText(hText5, Sys_Lib_GetDispStr(DP_DRIVER));
		TEXT_SetTextWrapMode(hText5, GUI_WRAPMODE_WORD);

        if(DriverGas_Chose_Flag)//djq add 20170609驱动气体选配
        {
            WM_ShowWindow(hButton4);
            WM_ShowWindow(hButton5);
			WM_ShowWindow(hText5);
        }
        else
        {
            WM_HideWindow(hButton4);
            WM_HideWindow(hButton5);
			WM_HideWindow(hText5);
        }

#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C) 
        if( (AlarmsWork.presentAlarms1 & ALARM_AIR_SOURCE_LOW)//djq add 20170407
         ||(AlarmsWork.newAlarms1 & ALARM_AIR_SOURCE_LOW) 
		 ||(AlarmsWork.presentAlarms & ALARM_OXY_SOURCE_LOW)
         ||(AlarmsWork.newAlarms & ALARM_OXY_SOURCE_LOW)
         ||(!AlarmAirSourceSwitch)
         ||(!StandbyState) )//工作模式下不能切换驱动气
        {
            if(ButtonRotation_GetTriangleSign(hButton5))
            {
                ButtonRotation_SetTriangleSign(hButton5,0);
            }
            WM_DisableWindow(hButton5);
        }
        else
        {
            WM_EnableWindow(hButton5);
        }
#endif

        hButton6 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON6);
        hButton7 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON7);
        hButton8 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON8);
        BUTTON_SetText(hButton6,Sys_Lib_GetDispStr(DP_CANCEL));
		BUTTON_SetText(hButton7,Sys_Lib_GetDispStr(DP_CONFIRM));
		BUTTON_SetText(hButton8,Sys_Lib_GetDispStr(DP_CONFIRM));
		//------------------------------------------------------------------------------------------
        hText0 = WM_GetDialogItem(hDlg, GUI_ID_TEXT0);
        hText1 = WM_GetDialogItem(hDlg, GUI_ID_TEXT1);
        hText2 = WM_GetDialogItem(hDlg, GUI_ID_TEXT2);
        hText3 = WM_GetDialogItem(hDlg, GUI_ID_TEXT3);
        hText4 = WM_GetDialogItem(hDlg, GUI_ID_TEXT4);

        //TEXT_SetText(hText0,System_Display[ STR_N2O_Range ]	); //笑气范围(暂时隐藏).
        TEXT_SetText(hText0,Sys_Lib_GetDispStr(DP_SETAATYPE)); //设置麻醉气体类型
// 		if(Drager_EtCO2_Config_On)
// 		{
// 			WM_ShowWindow(hText0);
// 		}
// 		else
// 		{
			WM_HideWindow(hText0);
// 		}
        TEXT_SetText(hText1,Sys_Lib_GetDispStr(DP_LANGUAGE)	);
        TEXT_SetText(hText2,Sys_Lib_GetDispStr(DP_TRIGGERTYPE));
        TEXT_SetText(hText3,Sys_Lib_GetDispStr(DP_CO2UNIT)	);
//         ButtonRotation_SetText(hButton0,Sys_Lib_GetDispStr(DP_LOUDNESS),"%");

        if(WM_IsVisible(hText4) //如果进入的时候hText是可见的，则隐藏界面  //djq add 20170407
         && !WM_IsVisible(hText0))
        {
            MasimoMenu_Flag = 1;
        }
        WM_HideWindow(hButton6);
        WM_HideWindow(hButton7);
        WM_HideWindow(hButton8);
        WM_HideWindow(hText4);
		if((MasimoISA_EtCO2_Config_On == 2) || (Drager_EtCO2_Config_On == 2))
		{
			WM_HideWindow(hButton3);
		}
        else
        {
            WM_ShowWindow(hButton3);
        }
#if (LOGO_FLAG == MSW_LOGO)
        WM_MoveChildTo(hButton2,263,100);
        WM_MoveChildTo(hButton3,200,100);
#endif
        break;
        //------------------------------------------------------------------------------------------
        
    case WM_KEY:
        break;

    case WM_NOTIFY_PARENT:
        NCode = pMsg->Data.v;
        Id = WM_GetId(pMsg->hWinSrc);
        switch(NCode)
        {
        case WM_NOTIFICATION_LOST_FOCUS: //  20130604
            //如果失去焦点，将按钮设置为弹起
            ButtonRotation_SetTriangleSign(WM_GetDialogItem(hDlg,GUI_ID_BUTTON0), 0);
            ButtonRotation_SetTriangleSign(WM_GetDialogItem(hDlg,GUI_ID_BUTTON1), 0);
            ButtonRotation_SetTriangleSign(WM_GetDialogItem(hDlg,GUI_ID_BUTTON2), 0);
			ButtonRotation_SetTriangleSign(WM_GetDialogItem(hDlg,GUI_ID_BUTTON3), 0);
            ButtonRotation_SetTriangleSign(WM_GetDialogItem(hDlg,GUI_ID_BUTTON4), 0);
            ButtonRotation_SetTriangleSign(WM_GetDialogItem(hDlg,GUI_ID_BUTTON5), 0);
            ButtonRotation_SetTriangleSign(WM_GetDialogItem(hDlg,GUI_ID_BUTTON9), 0);

            break;

        case WM_NOTIFICATION_CLICKED://widget has been clicked
            switch(Id)
            {
            case GUI_ID_DROPDOWN1:
            case GUI_ID_DROPDOWN2://  20130326
            case GUI_ID_DROPDOWN3://  20130409
			case GUI_ID_DROPDOWN0://  add ylk 20180227

                //如果有按钮按下，将按钮设置为弹起
                ButtonRotation_SetTriangleSign(WM_GetDialogItem(hDlg,GUI_ID_BUTTON0), 0);
                ButtonRotation_SetTriangleSign(WM_GetDialogItem(hDlg,GUI_ID_BUTTON1), 0);
                ButtonRotation_SetTriangleSign(WM_GetDialogItem(hDlg,GUI_ID_BUTTON2), 0);
                ButtonRotation_SetTriangleSign(WM_GetDialogItem(hDlg,GUI_ID_BUTTON3), 0);
                ButtonRotation_SetTriangleSign(WM_GetDialogItem(hDlg,GUI_ID_BUTTON4), 0);
                ButtonRotation_SetTriangleSign(WM_GetDialogItem(hDlg,GUI_ID_BUTTON5), 0);
                ButtonRotation_SetTriangleSign(WM_GetDialogItem(hDlg,GUI_ID_BUTTON9), 0);

                //如果有下拉框展开，将其收起 
                DROPDOWN_SetState(WM_GetDialogItem(hDlg,GUI_ID_DROPDOWN1), 0);
                DROPDOWN_SetState(WM_GetDialogItem(hDlg,GUI_ID_DROPDOWN2), 0);
                DROPDOWN_SetState(WM_GetDialogItem(hDlg,GUI_ID_DROPDOWN3), 0);
				DROPDOWN_SetState(WM_GetDialogItem(hDlg,GUI_ID_DROPDOWN0), 0);

                //将下拉控件设置为按下展开
                DROPDOWN_SetState(pMsg->hWinSrc,1); 
#ifndef LANGUAGE_CHINESE_FLAG
				DROPDOWN_SetItemDisabled(WM_GetDialogItem(hDlg,GUI_ID_DROPDOWN1), Disp_Lg_Chinese, 1);//中文禁选
#endif
				//DROPDOWN_SetItemDisabled(WM_GetDialogItem(hDlg,GUI_ID_DROPDOWN1), Disp_Lg_German, 1);//德语禁选
                break;
             //-------------------------------------------------------------------------------------
             case GUI_ID_BUTTON0:
             case GUI_ID_BUTTON1:
             case GUI_ID_BUTTON2:    
             case GUI_ID_BUTTON3://djq add 20131226   
             case GUI_ID_BUTTON4:
             case GUI_ID_BUTTON5:
             case GUI_ID_BUTTON9:
                 if(ButtonRotation_GetTriangleSign(pMsg->hWinSrc)) //setting over
                 {
                     WM_SelectWindow(pMsg->hWinSrc);
                     ButtonRotation_SetTriangleSign(pMsg->hWinSrc,0);
                     Data = ButtonRotation_GetData1(pMsg->hWinSrc);
                     ButtonRotation_SetData(pMsg->hWinSrc,Data);

                     switch(Id)
                     {
                     case GUI_ID_BUTTON0:                             
                         AlarmSettingPara.AlarmVolumeValue = Data;
                         AlarmSettingPara.AlarmVolumeValue = AlarmSettingPara.AlarmVolumeValue/20 - 1;
                         RTEEPROMwrite(ALARM_VOICE_VOLUME_ADDR,AlarmSettingPara.AlarmVolumeValue);//Data);
                         SendOperationMessage(BUZZER_VOLUME_SETTING,USART3);
                         break;
                     case GUI_ID_BUTTON1://SpO2
                         SpO2ModuleOff = Data;
                         RTEEPROMwrite(SPO2_ON_OFF_ADDR,SpO2ModuleOff);
                         if(Data)
                         {
							//添加报警
                            if(!(AlarmsWork.presentAlarms1 & ALARM_SPO2_MODULE_OFF)) //  20130705
                            {
                                SpO2ModuleClear_FLag = 0;
                                AlarmsWork.presentAlarms1 &= ~ALARM_SPO2_SENSOR_DISCONNECT;
                                AlarmsWork.unclearedAlarms1 &= ~ALARM_SPO2_SENSOR_DISCONNECT;
                                AlarmsWork.presentAlarms1 &= ~ALARM_SPO2_HIGH;
                                AlarmsWork.presentAlarms &= ~ALARM_SPO2_LOW;
                                AlarmsWork.presentAlarms &= ~ALARM_PULSE_RATE_LOW;
                                AlarmsWork.presentAlarms1 &= ~ALARM_PULSE_RATE_HIGH;
                                LowAlarmPriority &= ~0x02;//sun add 20141224
                            }                            
                         }
                         else
                         {
                            AlarmsWork.presentAlarms1 &= ~ALARM_SPO2_MODULE_OFF;
                            AlarmsWork.unclearedAlarms1 &= ~ALARM_SPO2_MODULE_OFF;
                            LowAlarmPriority &= ~0x20;//sun add 20141224
                         }                         
                         //添加报警
                         break;
                     case GUI_ID_BUTTON2://CO2
                         EtCO2ModuleOff = !Data;
                         RTEEPROMwrite(ETCO2_ON_OFF_ADDR,EtCO2ModuleOff);

                         if(Data)//模块关
                         {
                           
                           if(StandbyState != 0xFF)			//gyh add 20230727 关闭麻气监测后波形恢复默认监测类型
                           {
                             WM_MESSAGE Msg;
                             switch(Menu_hWin.cWin.WaveForms[0])
                             {
                             case WAVEFORM_CO2:
                             case WAVEFORM_SAA:
                             case WAVEFORM_PAA:
                             case WAVEFORM_N2O:
							 case LOOP_V_CO2:
                               Menu_hWin.cWin.WaveForms[0] = WAVEFORM_PRESSURE;
                               CurveMenuInit(&Menu_hWin.cWin,Menu_hWin.HomeWin.ParentWin);
                               break;
                             }
                             switch(Menu_hWin.cWin.WaveForms[1])
                             {
                             case WAVEFORM_CO2:
                             case WAVEFORM_SAA:
                             case WAVEFORM_PAA:
                             case WAVEFORM_N2O:
							 case LOOP_V_CO2:
                               Menu_hWin.cWin.WaveForms[1] = WAVEFORM_FLOW;
                               CurveMenuInit(&Menu_hWin.cWin,Menu_hWin.HomeWin.ParentWin);
                               break;
                             }
                             switch(Menu_hWin.cWin.WaveForms[2])
                             {
                             case WAVEFORM_CO2:
                             case WAVEFORM_SAA:
                             case WAVEFORM_PAA:
                             case WAVEFORM_N2O:
						     case LOOP_V_CO2:
                               Menu_hWin.cWin.WaveForms[2] = WAVEFORM_VOLUME;
							   WaveFormsId2= WAVEFORM_VOLUME;
                               CurveMenuInit(&Menu_hWin.cWin,Menu_hWin.HomeWin.ParentWin);
                               break;
                           }
                         }
                             if(!(AlarmsWork.presentAlarms1 & ALARM_ETCO2_MODULE_OFF)) //  20130705
                             {
                                 EtCO2ModuleClear_Flag = 0;
                                 AlarmsWork.presentAlarms &= ~ALARM_ETCO2_HIGH;
                                 AlarmsWork.presentAlarms1 &= ~ALARM_ETCO2_LOW;
                                 AlarmsWork.presentAlarms1 &= ~ALARM_ETCO2_LINE_OCCLUSION;
#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C) 
                                 AlarmsWork.presentAlarms &= ~ALARM_FIAA_HIGH;
                                 AlarmsWork.presentAlarms &= ~ALARM_ETAA_HIGH;
                                 AlarmsWork.presentAlarms1 &= ~ALARM_FIAA_LOW;
                                 AlarmsWork.presentAlarms1 &= ~ALARM_ETAA_LOW;
                                 AlarmsWork.presentAlarms &= ~ALARM_FIN2O_HIGH;
                                 AlarmsWork.presentAlarms1 &= ~ALARM_FIN2O_LOW;
                                 AlarmsWork.presentAlarms1 &= ~ALARM_AA_GTE_3MAC;
                                 AlarmsWork.presentAlarms1 &= ~ALARM_AA_LESSTHAN_3MAC;
#endif
#if (LOGO_FLAG == MSW_LOGO)                                 
                                 if(AA_Module_Flag.EtCO2ModuleExist == 1)//是否是国产CO2连接
                                 {
                                     SendOperationMessage(CO2_MODULEOFF,USART1);                                   
                                 }
#else                                 
                                 if(AA_Module_Flag.ETCO2_FLAG == 1)
                                 {
                                     SendOperationMessage(CO2_MODULEOFF,USART2);
                                 }  
#endif                                
                             }
							 if(MasimoISA_EtCO2_Config_On || Drager_EtCO2_Config_On)
							 {
								 SendOperationMessage(SET_AA_PUMPOFF,USART2);
							 }
                         }
                         else
                         {
                           
                           
                            AlarmsWork.presentAlarms1 &= ~ALARM_ETCO2_MODULE_OFF;
                            AlarmsWork.unclearedAlarms1 &= ~ALARM_ETCO2_MODULE_OFF;
                            LowAlarmPriority &= ~0x04;//sun add 20141224    
							//消除EtCO2开关的报警   消除AA开关的报警
							AlarmsWork.presentAlarms1 &= ~ALARM_ETCO2_MODULE_OFF;
							AlarmsWork.unclearedAlarms1 &= ~ALARM_ETCO2_MODULE_OFF;
                            if(etco2pump_Occlusion == 1)
							{
								CO2_Reset();
								etco2pump_Occlusion = 0;
							}
                            else                            
                            {
#if (LOGO_FLAG == MSW_LOGO)                             
                                if(AA_Module_Flag.EtCO2ModuleExist == 1)//是否是国产CO2
                                {
									if(StandbyState == 0)
									{
										SendOperationMessage(CO2_MODULEON,USART1);                                   
									}
                                }           
#else
                                if(AA_Module_Flag.ETCO2_FLAG == 1)
                                {
									if(StandbyState == 0)
									{
										SendOperationMessage(CO2_MODULEON,USART2);
									}
                                } 
#endif
                            }
							if(MasimoISA_EtCO2_Config_On || Drager_EtCO2_Config_On)
							{
								if(!StandbyState)//工作下 允许开泵
								{
									SendOperationMessage(SET_AA_PUMPON,USART2);
								}
							}
                         }
                         if( (Drager_EtCO2_Config_On == 2)
                             ||(MasimoISA_EtCO2_Config_On == 2))
						 {
							 O2ModuleOff = Data;//1关闭 0开启
							 RTEEPROMwrite(O2_ON_OFF_ADDR,O2ModuleOff); 
							 if(Data)
							 {
								 //添加报警
								 if(!(AlarmsWork.presentAlarms1 & ALARM_O2_MODULE_OFF)) //  20130705
								 {
									 O2ModuleClear_FLag = 0;
									 AlarmsWork.presentAlarms &= ~ALARM_FIO2_HIGH;
									 AlarmsWork.presentAlarms &= ~ALARM_FIO2_LOW;
									 AlarmsWork.presentAlarms &= ~ALARM_FIO2_LESSTHAN_18;
								 }
							 }
							 else
							 {
								 AlarmsWork.presentAlarms1 &= ~ALARM_O2_MODULE_OFF;
								 AlarmsWork.unclearedAlarms1 &= ~ALARM_O2_MODULE_OFF;
								 LowAlarmPriority &= ~0x10;//sun add 20141224
							 }
						 }
						 break;
                     case GUI_ID_BUTTON3://djq add 20131226
                         O2ModuleOff = Data;//1关闭 0开启
                         RTEEPROMwrite(O2_ON_OFF_ADDR,O2ModuleOff); 
                         if(Data)
                         {
						 	//添加报警
                             if(!(AlarmsWork.presentAlarms1 & ALARM_O2_MODULE_OFF)) //  20130705
                             {
                                 O2ModuleClear_FLag = 0;
                                 AlarmsWork.presentAlarms &= ~ALARM_FIO2_HIGH;
                                 AlarmsWork.presentAlarms &= ~ALARM_FIO2_LOW;
								 AlarmsWork.presentAlarms &= ~ALARM_FIO2_LESSTHAN_18;
                             }
                         }
                         else
                         {
                             AlarmsWork.presentAlarms1 &= ~ALARM_O2_MODULE_OFF;
                             AlarmsWork.unclearedAlarms1 &= ~ALARM_O2_MODULE_OFF;
                             LowAlarmPriority &= ~0x10;//sun add 20141224
                         }
                         //添加报警
						 break;
                     case GUI_ID_BUTTON4://zkq  add 20170407 空气气源开关
                        WM_SelectWindow(pMsg->hWinSrc);
                        ButtonRotation_SetTriangleSign(pMsg->hWinSrc,0);
                        Data = ButtonRotation_GetData1(pMsg->hWinSrc);
						WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON10));//More
						WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON11));//Comp.
                        if(Data)
                        {
                            //空气作为驱动气的时候，不能关闭气源开关
                            if(DirverGasFlag)//空气为驱动气
                            {
                                MasimoMenu_Flag = 10;
                                ButtonRotation_SetData(pMsg->hWinSrc,0);
                            }
                            else
                            {
                                MasimoMenu_Flag = 11;
                                ButtonRotation_SetData(pMsg->hWinSrc,Data);
                            }
                        }
                        else
                        {
                            AlarmAirSourceSwitch = !Data;
                            ButtonRotation_SetData(pMsg->hWinSrc,Data);
                            RTEEPROMwrite(AIR_SUPPLY_SWITCH_ADDR,AlarmAirSourceSwitch);
                            if(DriverGas_Chose_Flag)//djq add 20170609驱动气体选配
                            {
                                if((AlarmsWork.presentAlarms & ALARM_OXY_SOURCE_LOW)
                                    &&(!DirverGasFlag))
                                { 
                                    DirverGasFlag = 1;
                                    RTEEPROMwrite(DIRVER_GAS_ADDR,DirverGasFlag);
                                    //向BDU发送驱动器类型
                                    SendOperationMessage(DIRVER_GAS_FOR_BDU_COM,USART0);
                                }
                                Menu_hWin.InfoWin.InfoChangeFlag |= DRIVER_GAS_TYPE_FLAG;
                                WM_SendMessageNoPara(hDlg,WM_INIT_DIALOG);
                            }
                        }
                         break;
                     case GUI_ID_BUTTON5://驱动气开关
                         MasimoMenu_Flag = 14;
                         WM_SelectWindow(pMsg->hWinSrc);
                         ButtonRotation_SetTriangleSign(pMsg->hWinSrc,0);
                         value = ButtonRotation_GetData1(pMsg->hWinSrc);
						 WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON10));//More
						 WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON11));//Comp.
                         break;
                     case GUI_ID_BUTTON9://SIGH
                         ButtonRotation_SetTriangleSign(pMsg->hWinSrc,0);
                         value = ButtonRotation_GetData1(pMsg->hWinSrc);
                         ButtonRotation_SetData(pMsg->hWinSrc,value);
                         BeathSettingPara.SettingSighTimesValue = value;

						 BreathPara_Assign(&Breath_Setting_Tmp, &BeathSettingPara);

                         SendOperationMessage(SIGH_TIMES_UPDATE,USART0);
                        break;
                     default:break;
                     }
                 }
                 else //start setting
                 {

                     //如果有按钮按下，将按钮设置为弹起
                     ButtonRotation_SetTriangleSign(WM_GetDialogItem(hDlg,GUI_ID_BUTTON0), 0);
                     ButtonRotation_SetTriangleSign(WM_GetDialogItem(hDlg,GUI_ID_BUTTON1), 0);
                     ButtonRotation_SetTriangleSign(WM_GetDialogItem(hDlg,GUI_ID_BUTTON2), 0);
                     ButtonRotation_SetTriangleSign(WM_GetDialogItem(hDlg,GUI_ID_BUTTON3), 0);//djq add 20131226
                     ButtonRotation_SetTriangleSign(WM_GetDialogItem(hDlg,GUI_ID_BUTTON4), 0);
                     ButtonRotation_SetTriangleSign(WM_GetDialogItem(hDlg,GUI_ID_BUTTON5), 0);
                     ButtonRotation_SetTriangleSign(WM_GetDialogItem(hDlg,GUI_ID_BUTTON9), 0);

                     //*///
                     //将按钮设置为按下
                     WM_SetFocus(pMsg->hWinSrc);
                     Data = ButtonRotation_GetData(pMsg->hWinSrc);
                     ButtonRotation_SetData1(pMsg->hWinSrc,Data);
                     ButtonRotation_SetTriangleSign(pMsg->hWinSrc,1);
                 }
                 break; //endof case GUI_ID_BUTTON2
            //--------------------------------------------------------------------------------------
            case GUI_ID_BUTTON6://取消切换按钮
                MasimoMenu_Flag = 1;
                WM_SendMessageNoPara(hDlg,WM_INIT_DIALOG);
                BUTTON_SetPressed(pMsg->hWinSrc,0);
                break;
            case GUI_ID_BUTTON7://确认切换按钮
                MasimoMenu_Flag = 1;
                if(ShowFlagNum == 2)
                {
                    ButtonRotation_SetData(WM_GetDialogItem(hDlg,GUI_ID_BUTTON4),0);//不能关闭AIR气源开关
                    ButtonRotation_SetTriangleSign(WM_GetDialogItem(hDlg,GUI_ID_BUTTON4),0);
                }
                else if(ShowFlagNum == 3)//空气开关确认  关闭空气气源开关
                {
                    ButtonRotation_SetData(WM_GetDialogItem(hDlg,GUI_ID_BUTTON4),1);
                    ButtonRotation_SetTriangleSign(WM_GetDialogItem(hDlg,GUI_ID_BUTTON4),0);
				    AlarmAirSourceSwitch = 0;
				    RTEEPROMwrite(AIR_SUPPLY_SWITCH_ADDR,AlarmAirSourceSwitch);
#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C) 
                    if(AlarmsWork.presentAlarms1 & ALARM_AIR_SOURCE_LOW)
                    {
                        AlarmsWork.presentAlarms1 &= ~ALARM_AIR_SOURCE_LOW;
                    }
#endif
                }
				else if(ShowFlagNum == 6)//驱动气确认
				{
                    value = ButtonRotation_GetData1(WM_GetDialogItem(hDlg, GUI_ID_BUTTON5));
                    ButtonRotation_SetData(WM_GetDialogItem(hDlg,GUI_ID_BUTTON5),value);//////
                    ButtonRotation_SetTriangleSign(WM_GetDialogItem(hDlg,GUI_ID_BUTTON5),0);
                    if(DriverGas_Chose_Flag)//djq add 20170609驱动气体选配
                    {
                        DirverGasFlag = value;
                        RTEEPROMwrite(DIRVER_GAS_ADDR,DirverGasFlag);
                        //向BDU发送驱动器类型
                        SendOperationMessage(DIRVER_GAS_FOR_BDU_COM,USART0);
                    }
				}
                if(DriverGas_Chose_Flag)//djq add 20170609驱动气体选配
                {
                    Menu_hWin.InfoWin.InfoChangeFlag |= DRIVER_GAS_TYPE_FLAG;
                }
				WM_SendMessageNoPara(hDlg,WM_INIT_DIALOG);
				BUTTON_SetPressed(pMsg->hWinSrc,0);
                break;
            case GUI_ID_BUTTON8:
                MasimoMenu_Flag = 1;
				//WM_SendMessageNoPara(hDlg,WM_INIT_DIALOG);
				BUTTON_SetPressed(pMsg->hWinSrc,0);				
				if(Masimo_SpO2_Config_On)
				{
					WM_ShowWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON10)); //More
				}				
                break;
			case GUI_ID_BUTTON10:
				BUTTON_SetPressed(pMsg->hWinSrc,0);
				if(!WM_IsWindow(hSpO2))//如果没有创建过窗体，则创建
				{
					hSpO2 = GUI_CreateDialogBox(_aDialogSettingMenu_SpO2,
						GUI_COUNTOF(_aDialogSettingMenu_SpO2),
						&_cbDialogSettingMenu_SpO2,hDlg,0,0);
				}
				WM_ShowWindow(hSpO2);
				break;
			case GUI_ID_BUTTON11:
				BUTTON_SetPressed(pMsg->hWinSrc,0);
				if(!WM_IsWindow(hComp))//如果没有创建过窗体，则创建
				{
					hComp = GUI_CreateDialogBox(_aDialogSettingMenu_Compensate,
						GUI_COUNTOF(_aDialogSettingMenu_Compensate),
						&_cbDialogSettingMenu_Compensate,hDlg,0,0);
				}
				WM_ShowWindow(hComp);
				break;
            default:
                break;
            }
            break; //endof case WM_NOTIFICATION_CLICKED           
        //==========================================================================================    
        case WM_NOTIFICATION_SEL_CHANGED: //The selection of the dropdown list has changed
			//--------------------------------------------------------------------------------------
			hItem = WM_GetDialogItem(hDlg, GUI_ID_DROPDOWN0);
			if(DROPDOWN_GetState(hItem))
			{
				int index = 0;
				DROPDOWN_SetState(hItem, 0);
				Id = DROPDOWN_GetLstWin(hItem);
				index = LISTBOX_GetSel(Id);
				DROPDOWN_SetSel(hItem, index);

				AA_all_data.AA_detail.AA_AA1.ID = index;
				AA_Module_String();
			}
            //--------------------------------------------------------------------------------------
            hItem = WM_GetDialogItem(hDlg, GUI_ID_DROPDOWN1);
            if(DROPDOWN_GetState(hItem))
            {
                DROPDOWN_SetState(hItem,0);
                LanguageType = DROPDOWN_GetSel(hItem);
                RTEEPROMwrite(LANGUAGE_TYPE_ADDR,LanguageType & 0xFF);                                				
//              SYS_str_init();              //zy 2016.6.12 
				Sys_Lib_LgInit(LanguageType);                
                InitAlarmInfoRegister();                
				Initcbdialog32string(Patient_GetInfo(), BeathSettingPara.BreathMode);
				HomePrompt_StrInit();
				HomePrompt_UpdateStr();

                Id = WM_GetDialogItem(Menu_hWin.ActivedWin, GUI_ID_MULTIPAGE0);
                ChangeMultiPageText(LanguageType,Id);
				WM_SendMessageNoPara(Menu_hWin.bWin,WM_INIT_DIALOG);//切换语言时初始化呼吸参数设置界面

				if(WM_IsWindow(hSpO2))//创建过窗体
				{
					WM_SendMessageNoPara(hSpO2,WM_INIT_DIALOG);
				}
				if(WM_IsWindow(hComp))//创建过窗体
				{
					WM_SendMessageNoPara(hComp,WM_INIT_DIALOG);
				}

                Msg.hWin =  MULTIPAGE_GetWindow(Id,0); //cpp del 20130626
                Msg.MsgId = WM_INIT_DIALOG;
                WM_SendMessage(Msg.hWin,&Msg);
			
                Msg.hWin =  MULTIPAGE_GetWindow(Id,1);
                Msg.MsgId = WM_INIT_DIALOG;
                WM_SendMessage(Msg.hWin,&Msg);
			
                Msg.hWin =  MULTIPAGE_GetWindow(Id,2);
                Msg.MsgId = WM_INIT_DIALOG;
                WM_SendMessage(Msg.hWin,&Msg);
			
                Msg.hWin =  MULTIPAGE_GetWindow(Id,3);
                Msg.MsgId = WM_INIT_DIALOG;
                WM_SendMessage(Msg.hWin,&Msg);
			

                Msg.hWin =  Menu_hWin.hWin; //右侧功能菜单的语言更换
                Msg.MsgId = WM_INIT_DIALOG;
                WM_SendMessage(Msg.hWin,&Msg);
				
                if((InquireAlarmInfoWindows())!=0)
                {
                    Menu_hWin.InfoWin.InfoChangeFlag |= ALARM_INFO_CHANGED_FLAG;
                }
                if(StandbyState==0xff)   
                {
                    Msg.hWin = WM_GetClientWindow(Menu_hWin.Swin);
                    Msg.MsgId = WM_INIT_DIALOG;
                    WM_SendMessage(Msg.hWin,&Msg);
                }
                else//djq add 20140220 解决：更换语言后，波形提示语不能切换为新的语言
                {
                    Msg.hWin = Menu_hWin.HomeWin.ParentWin;
                    Msg.MsgId = WM_INIT_DIALOG;
                    WM_SendMessage(Msg.hWin,&Msg);
                }

				if (!StandbyState&&CPB_GetFlag())
				{
					Menu_hWin.InfoWin.InfoChangeFlag |= CPB_CHANGE_FLAG;
				}

                DROPDOWN_Collapse(hItem);
				SetRefreshFlag(1);
				SetTimerClearFlag(0);
            }
            //*//-----------------------------------------------------------------------------------
            hItem = WM_GetDialogItem(hDlg, GUI_ID_DROPDOWN2);
            if(DROPDOWN_GetState(hItem))
            {
                int index;
                DROPDOWN_SetState(hItem,0);
                index = DROPDOWN_GetSel(hItem);
                BeathSettingPara.TriggerMode = index;
                SendOperationMessage(TRIGGER_UPDATE,USART0);

				BreathPara_Assign(&Breath_Setting_Tmp, &BeathSettingPara);

                hItem1 = WM_GetDialogItem(Menu_hWin.bWin, GUI_ID_BUTTON5);
                switch(index)
                {
                case 0:
                    ButtonExCtrl_SetFontInfo(hItem1,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_PSENS),Sys_Lib_GetDispStr(DP_CMH2O));
//#if(LOGO_FLAG == PENLON_LOGO)//ylk add 20180709
//					ButtonExCtrl_SetButtonDataInfo(hItem1,1,20,BeathSettingPara.SettingPtrValue,1,0);
//#elif(LOGO_FLAG == HOSPITECH_LOGO)
					ButtonExCtrl_SetButtonDataInfo(hItem1,-20,0,BeathSettingPara.SettingPtrValue,1,0);
//#else
//					ButtonExCtrl_SetButtonDataInfo(hItem1,0,20,BeathSettingPara.SettingPtrValue,1,0);
//#endif
                    ButtonExCtrl_SetData(hItem1,BeathSettingPara.SettingPtrValue);
                    break;
                case 1:
                    ButtonExCtrl_SetFontInfo(hItem1,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_FSENS),"LPM");
#if(LOGO_FLAG == PENLON_LOGO)//ylk add 20180709
					ButtonExCtrl_SetButtonDataInfo(hItem1,10,200,BeathSettingPara.SettingFtrValue,1,1);
#else
					ButtonExCtrl_SetButtonDataInfo(hItem1,5,200,BeathSettingPara.SettingFtrValue,1,1);
#endif
                    ButtonExCtrl_SetData(hItem1,BeathSettingPara.SettingFtrValue);
                    break;
                default:
                    break;
                }
            }
            //*//CO2单位切换-----------------------------------------------------------------------------------
            hItem = WM_GetDialogItem(hDlg, GUI_ID_DROPDOWN3);
            if(DROPDOWN_GetState(hItem))
            {
                int i;
                int index;
                DROPDOWN_SetState(hItem,0);
                index = DROPDOWN_GetSel(hItem);
                ETCO2Unit = index;
				CO2_UnitConvert(ETCO2Unit);
                RTEEPROMwrite(ETCO2_UNIT_ADDR,ETCO2Unit & 0xFF);

                switch(ETCO2Unit) //  20130531
                {
                case 0:
                    if(preETCO2Unit == 1)
                    {
                        AlarmSettingPara.EtCO2HighValue = (int)(AlarmSettingPara.EtCO2HighValue * 1.33 + 0.5);
                        AlarmSettingPara.EtCO2LowValue  = (int)(AlarmSettingPara.EtCO2LowValue * 1.33 + 0.5);
                        AlarmSettingPara.FiCO2HighValue = (int)(AlarmSettingPara.FiCO2HighValue * 1.33 + 0.5);
                        AlarmSettingPara.FiCO2LowValue  = (int)(AlarmSettingPara.FiCO2LowValue * 1.33 + 0.5);                        
                        preETCO2Unit = 0;
                    }
                    break;
                case 1:
                    if((preETCO2Unit == 0)||(preETCO2Unit == 2))
                    {
                        AlarmSettingPara.EtCO2HighValue = (int)(AlarmSettingPara.EtCO2HighValue / 1.33 + 0.5);
                        AlarmSettingPara.EtCO2LowValue  = (int)(AlarmSettingPara.EtCO2LowValue / 1.33 + 0.5);
                        AlarmSettingPara.FiCO2HighValue = (int)(AlarmSettingPara.FiCO2HighValue / 1.33 + 0.5);
                        AlarmSettingPara.FiCO2LowValue  = (int)(AlarmSettingPara.FiCO2LowValue / 1.33 + 0.5);
                        preETCO2Unit = 1;
                    }
                    break;
                case 2:
                    if(preETCO2Unit == 1)
                    {
                        AlarmSettingPara.EtCO2HighValue = (int)(AlarmSettingPara.EtCO2HighValue * 1.33 + 0.5);
                        AlarmSettingPara.EtCO2LowValue  = (int)(AlarmSettingPara.EtCO2LowValue * 1.33 + 0.5);
                        AlarmSettingPara.FiCO2HighValue = (int)(AlarmSettingPara.FiCO2HighValue * 1.33 + 0.5);
                        AlarmSettingPara.FiCO2LowValue  = (int)(AlarmSettingPara.FiCO2LowValue * 1.33 + 0.5);
                        preETCO2Unit = 2;
                    }
                    break;
                default: 
                    break;
                }
                //TODO  立即更改所有 ETCO2 的单位
                if(StandbyState == 0) //ljs add 20130528
                {
					int index = 0;
					switch(ETCO2Unit)
					{
					case 0:
					default:
						pText = Sys_Lib_GetDispStr(DP_PERCENT);
						break;
					case 1:
						pText = Sys_Lib_GetDispStr(DP_MMHG);
						break;
					case 2:
						pText = Sys_Lib_GetDispStr(DP_KPA);
						break;
					}
					if((index = Get_MonitorRank_AA(DP_CO2)) != -1)
					{	
						MultiText_UnitUpdate(Menu_hWin.HomeWin.Left_Robj[index], pText);
						MultiText_OtherUnitUpdate(Menu_hWin.HomeWin.Left_Robj[index], pText);
						//  [2/17/2022 yaoyao add]CO2单位切换后，相应的更新快捷监测区CO2上下限数据
						switch (ETCO2Unit)
						{
						case 0:
						case 2:
						default:
							strcpy(pText1, _Float2String((float)AlarmSettingPara.FiCO2HighValue/10,1));	
							strcpy(pText2, _Float2String((float)AlarmSettingPara.FiCO2LowValue/10,1));	
							strcpy(pText3, _Float2String((float)AlarmSettingPara.EtCO2HighValue/10,1));	
							strcpy(pText4, _Float2String((float)AlarmSettingPara.EtCO2LowValue/10,1));	
							break;
						case 1:
							sprintf(pText1, "%d", AlarmSettingPara.FiCO2HighValue);
							sprintf(pText2, "%d", AlarmSettingPara.FiCO2LowValue);
							sprintf(pText3, "%d", AlarmSettingPara.EtCO2HighValue);
							sprintf(pText4, "%d", AlarmSettingPara.EtCO2LowValue);
							break;
						}
						MultiText_MaxValueUpdate(Menu_hWin.HomeWin.Left_Robj[index], pText1);
						MultiText_MinValueUpdate(Menu_hWin.HomeWin.Left_Robj[index], pText2);
						MultiText_MaxValueUpdate1(Menu_hWin.HomeWin.Left_Robj[index], pText3);
						MultiText_MinValueUpdate1(Menu_hWin.HomeWin.Left_Robj[index], pText4);
					}
// 					//  [2/17/2022 yaoyao 注释]CO2在左侧监测区
					/*if((index = Get_MonitorRank_Rapid(DP_ETCO2)) != -1)
					{
						MultiText_UnitUpdate(Menu_hWin.HomeWin.Robj[index], pText);
					}*/
                    // 
                    for(i=0;i<=3;i++)//现在四条波形//现在三条波形
                    {
                        if(Menu_hWin.cWin.WaveForms[i] == WAVEFORM_CO2)
                        {
							TracingCurve_SetXYINFO(Menu_hWin.HomeWin.Tobj[i],'T',0,8,1,GUI_WHITE,Sys_Lib_GetDispStr(DP_S),
								0,CO2_Disp.co2max,CO2_Disp.co2step,GUI_WHITE,CO2_Disp.co2_unit,GUI_WHITE,Sys_Lib_GetDispStr(DP_CO2));//CO2
                        }
						if(Menu_hWin.cWin.WaveForms[i] == LOOP_V_CO2)
						{
							TracingCurve_SetXYINFO(Menu_hWin.HomeWin.Tobj[i],'T',0,30,5,GUI_WHITE,Sys_Lib_GetDispStr(DP_ML),
								0,CO2_Disp.co2max,CO2_Disp.co2step,GUI_WHITE,CO2_Disp.co2_unit,GUI_WHITE,Sys_Lib_GetDispStr(DP_VCO2));
						}
                        Msg.hWin = Menu_hWin.HomeWin.Tobj[i];
                        Msg.MsgId = WM_NOTIFICATION_SEL_CHANGED;
                        WM_SelectWindow(Msg.hWin);
                        WM_SendMessage(Msg.hWin,&Msg); 
                    }//*///
                    
                }//endof if(StandbyState == 0)
            }
            break; //endof case WM_NOTIFICATION_SEL_CHANGED
        //==========================================================================================
        case GUI_KEY_LEFT:
            if(!Id) //不是控件的Id
            {
				hItem = WM_GetDialogItem(hDlg, GUI_ID_DROPDOWN0);
				if(DROPDOWN_GetState(hItem))
				{
					LISTBOX_DecSel(pMsg->hWinSrc);
				}
                hItem = WM_GetDialogItem(hDlg, GUI_ID_DROPDOWN1);
                if(DROPDOWN_GetState(hItem))
                {
                    LISTBOX_DecSel(pMsg->hWinSrc);
                }
                hItem = WM_GetDialogItem(hDlg, GUI_ID_DROPDOWN2);
                if(DROPDOWN_GetState(hItem))
                {
                    LISTBOX_DecSel(pMsg->hWinSrc);
                }
                hItem = WM_GetDialogItem(hDlg, GUI_ID_DROPDOWN3);
                if(DROPDOWN_GetState(hItem))
                {
                    LISTBOX_DecSel(pMsg->hWinSrc);
                }            
            }
            else 
            {
                switch(Id)
                {
                case GUI_ID_BUTTON0:
                    if(ButtonRotation_GetTriangleSign(pMsg->hWinSrc))//if setting now?
                    {
                        if(!ButtonRotation_DataSub(pMsg->hWinSrc))
                        {
                            BuzzerTestVolume = ButtonRotation_GetData1(pMsg->hWinSrc);
                            BuzzerTestVolume = BuzzerTestVolume/20 - 1;
                            SendOperationMessage(BUZZER_VOLUME_SETTING,USART3);
                        }
                        Msg.MsgId   = WM_NOTIFICATION_VALUE_CHANGED;
                        Msg.Data.v  = GUI_KEY_LEFT;
                        Msg.hWin = pMsg->hWinSrc;
                        WM_SendMessage(pMsg->hWinSrc,&Msg);
                    }
                    break;
                case GUI_ID_BUTTON1:
                case GUI_ID_BUTTON2:
                case GUI_ID_BUTTON3://djq add 2131226
                case GUI_ID_BUTTON4:
                case GUI_ID_BUTTON5:
                case GUI_ID_BUTTON9:
                    if(ButtonRotation_GetTriangleSign(pMsg->hWinSrc))//if setting now?
                    {
                        ButtonRotation_DataSub(pMsg->hWinSrc);
                        Msg.MsgId   = WM_NOTIFICATION_VALUE_CHANGED;
                        Msg.Data.v  = GUI_KEY_LEFT;
                        Msg.hWin = pMsg->hWinSrc;
                        WM_SendMessage(pMsg->hWinSrc,&Msg);
                    }
                    break;
                default:
                    break;
                }
            }
            break; //endof case GUI_KEY_LEFT
        //==========================================================================================
        case GUI_KEY_RIGHT:
            if(!Id)
            {
				hItem = WM_GetDialogItem(hDlg, GUI_ID_DROPDOWN0);
				if(DROPDOWN_GetState(hItem))
				{
					LISTBOX_IncSel(pMsg->hWinSrc);
				}
                hItem = WM_GetDialogItem(hDlg, GUI_ID_DROPDOWN1);
                if(DROPDOWN_GetState(hItem))
                {
                    LISTBOX_IncSel(pMsg->hWinSrc);
                }
                hItem = WM_GetDialogItem(hDlg, GUI_ID_DROPDOWN2);
                if(DROPDOWN_GetState(hItem))
                {
                    LISTBOX_IncSel(pMsg->hWinSrc);
                }
                hItem = WM_GetDialogItem(hDlg, GUI_ID_DROPDOWN3);
                if(DROPDOWN_GetState(hItem))
                {
                    LISTBOX_IncSel(pMsg->hWinSrc);
                }
            }
            else
            {
                switch(Id)
                {
                case GUI_ID_BUTTON0:
                    if(ButtonRotation_GetTriangleSign(pMsg->hWinSrc))//if setting now?
                    {
                        if(!ButtonRotation_DataAdd(pMsg->hWinSrc))
                        {
                            BuzzerTestVolume = ButtonRotation_GetData1(pMsg->hWinSrc);
                            BuzzerTestVolume = BuzzerTestVolume/20 - 1;
                            SendOperationMessage(BUZZER_VOLUME_SETTING,USART3);
                        }
                        Msg.MsgId   = WM_NOTIFICATION_VALUE_CHANGED;
                        Msg.Data.v  = GUI_KEY_RIGHT;
                        Msg.hWin = pMsg->hWinSrc;
                        WM_SendMessage(pMsg->hWinSrc,&Msg);
                    }
                    break;
                case GUI_ID_BUTTON1:
                case GUI_ID_BUTTON2:
                case GUI_ID_BUTTON3://djq add 20131226
                case GUI_ID_BUTTON4:
                case GUI_ID_BUTTON5:
                case GUI_ID_BUTTON9:
                    if(ButtonRotation_GetTriangleSign(pMsg->hWinSrc))//if setting now?
                    {
                        ButtonRotation_DataAdd(pMsg->hWinSrc);
                        Msg.MsgId   = WM_NOTIFICATION_VALUE_CHANGED;
                        Msg.Data.v  = GUI_KEY_RIGHT;
                        Msg.hWin = pMsg->hWinSrc;
                        WM_SendMessage(pMsg->hWinSrc,&Msg);
                    }
                    break;
                default:
                    break;
                }
            }
            break; //endof case GUI_KEY_RIGHT
        //==========================================================================================
        case GUI_KEY_ENTER:
            if(!Id)
            {   
				hItem = WM_GetDialogItem(hDlg, GUI_ID_DROPDOWN0);
				if(DROPDOWN_GetState(hItem))
				{
					int index = 0;
					DROPDOWN_SetState(hItem, 0);
					Id = DROPDOWN_GetLstWin(hItem);
					index = LISTBOX_GetSel(Id);
					DROPDOWN_SetSel(hItem, index);

					AA_all_data.AA_detail.AA_AA1.ID = index;
					AA_Module_String();

					DROPDOWN_Collapse(hItem);//确认后收起下拉菜单
				}
				//----------------------------------------------------------------------------------
                hItem = WM_GetDialogItem(hDlg, GUI_ID_DROPDOWN1);
                if(DROPDOWN_GetState(hItem))
                {
                    int index;  //cpp del 20130426
                    DROPDOWN_SetState(hItem,0);
                    Id = DROPDOWN_GetLstWin(hItem);
                    index = LISTBOX_GetSel(Id);
                    DROPDOWN_SetSel(hItem,index);
                    LanguageType = index;
                    RTEEPROMwrite(LANGUAGE_TYPE_ADDR,LanguageType & 0xFF);				
//                  SYS_str_init();
					Sys_Lib_LgInit(LanguageType);
                    InitAlarmInfoRegister();

					Initcbdialog32string(Patient_GetInfo(), BeathSettingPara.BreathMode); 
					HomePrompt_StrInit();
					HomePrompt_UpdateStr();

					WM_SendMessageNoPara(Menu_hWin.bWin,WM_INIT_DIALOG);//切换语言时初始化呼吸参数设置界面

					if(WM_IsWindow(hSpO2))//创建过窗体
					{
						WM_SendMessageNoPara(hSpO2,WM_INIT_DIALOG);
					}
					if(WM_IsWindow(hComp))//创建过窗体
					{
						WM_SendMessageNoPara(hComp,WM_INIT_DIALOG);
					}

                    hItem1 = WM_GetDialogItem(hDlg, GUI_ID_DROPDOWN2);//  20130407
                    DROPDOWN_SetNumItems(hItem1,0);//  20130407

                    //WM_DeleteWindow(Menu_hWin.ActivedWin);
                    //Menu_hWin.ActivedWin = _SystemHandleWin();
                    Id = WM_GetDialogItem(Menu_hWin.ActivedWin, GUI_ID_MULTIPAGE0);
                    ChangeMultiPageText(LanguageType,Id);
                    //MULTIPAGE_SelectPage(hItem,1);

                    Msg.hWin =  MULTIPAGE_GetWindow(Id,0);
                    Msg.MsgId = WM_INIT_DIALOG;
                    WM_SendMessage(Msg.hWin,&Msg);
					
                    Msg.hWin =  MULTIPAGE_GetWindow(Id,1);
                    Msg.MsgId = WM_INIT_DIALOG;
                    WM_SendMessage(Msg.hWin,&Msg);
					
                    Msg.hWin =  MULTIPAGE_GetWindow(Id,2);
                    Msg.MsgId = WM_INIT_DIALOG;
                    WM_SendMessage(Msg.hWin,&Msg);
					
                    Msg.hWin =  MULTIPAGE_GetWindow(Id,3);
                    Msg.MsgId = WM_INIT_DIALOG;
                    WM_SendMessage(Msg.hWin,&Msg);
					
                    Msg.hWin =  Menu_hWin.hWin; //右侧功能菜单的语言更换
                    Msg.MsgId = WM_INIT_DIALOG;
                    WM_SendMessage(Msg.hWin,&Msg);
					
                    if((InquireAlarmInfoWindows())!=0)
                    {
                        Menu_hWin.InfoWin.InfoChangeFlag |= ALARM_INFO_CHANGED_FLAG;
                    }
                    if(StandbyState==0xff)
                    {
                        Msg.hWin = WM_GetClientWindow(Menu_hWin.Swin);
                        Msg.MsgId = WM_INIT_DIALOG;
                        WM_SendMessage(Msg.hWin,&Msg);
                    }
                    else   //djq add 20140220 解决：更换语言后，波形提示语不能切换为新的语言
                    {
                        Msg.hWin = Menu_hWin.HomeWin.ParentWin;
                        Msg.MsgId = WM_INIT_DIALOG;
                        WM_SendMessage(Msg.hWin,&Msg);
                    }
                    
					if (!StandbyState&&CPB_GetFlag())
					{
						Menu_hWin.InfoWin.InfoChangeFlag |= CPB_CHANGE_FLAG;
					}

                    DROPDOWN_Collapse(hItem);
                }
                //*//-------------------------------------------------------------------------------
                hItem = WM_GetDialogItem(hDlg, GUI_ID_DROPDOWN2);
                if(DROPDOWN_GetState(hItem))
                {
                    int index;
                    DROPDOWN_SetState(hItem,0);
                    Id = DROPDOWN_GetLstWin(hItem);
                    index = LISTBOX_GetSel(Id);
                    DROPDOWN_SetSel(hItem,index);
                    BeathSettingPara.TriggerMode = index;
					BreathPara_Assign(&Breath_Setting_Tmp, &BeathSettingPara);
                    SendOperationMessage(TRIGGER_UPDATE,USART0);

                    hItem1 = WM_GetDialogItem(Menu_hWin.bWin, GUI_ID_BUTTON5);
                    switch(index)
                    {
					case 0:
						ButtonExCtrl_SetFontInfo(hItem1,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_PSENS),Sys_Lib_GetDispStr(DP_CMH2O));
//#if(LOGO_FLAG == PENLON_LOGO)//ylk add 20180709
//						ButtonExCtrl_SetButtonDataInfo(hItem1,1,20,BeathSettingPara.SettingPtrValue,1,0);
//#elif(LOGO_FLAG == HOSPITECH_LOGO)
						ButtonExCtrl_SetButtonDataInfo(hItem1,-20,0,BeathSettingPara.SettingPtrValue,1,0);
//#else
//						ButtonExCtrl_SetButtonDataInfo(hItem1,0,20,BeathSettingPara.SettingPtrValue,1,0);
//#endif
						ButtonExCtrl_SetData(hItem1,BeathSettingPara.SettingPtrValue);
						break;
					case 1:
						ButtonExCtrl_SetFontInfo(hItem1,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_FSENS),"LPM");
#if(LOGO_FLAG == PENLON_LOGO)//ylk add 20180709
						ButtonExCtrl_SetButtonDataInfo(hItem1,10,200,BeathSettingPara.SettingFtrValue,1,1);
#else
						ButtonExCtrl_SetButtonDataInfo(hItem1,5,200,BeathSettingPara.SettingFtrValue,1,1);
#endif
						ButtonExCtrl_SetData(hItem1,BeathSettingPara.SettingFtrValue);
						break;
                    default:break;
                    }
                    DROPDOWN_Collapse(hItem);
                }
                //*//CO2单位切换-------------------------------------------------------------------------------
                hItem = WM_GetDialogItem(hDlg, GUI_ID_DROPDOWN3);
                if(DROPDOWN_GetState(hItem))
                {
                    int index;                    
                    DROPDOWN_SetState(hItem,0);
                    Id = DROPDOWN_GetLstWin(hItem);
                    index = LISTBOX_GetSel(Id);
                    DROPDOWN_SetSel(hItem,index);
                    ETCO2Unit = index;
                    RTEEPROMwrite(ETCO2_UNIT_ADDR,ETCO2Unit & 0xFF);
					CO2_UnitConvert(ETCO2Unit);

                    switch(ETCO2Unit) //  20130531
                    {
                    case 0:
                        if(preETCO2Unit == 1)
                        {
                            AlarmSettingPara.EtCO2HighValue = (int)(AlarmSettingPara.EtCO2HighValue * 1.33 + 0.5);
                            AlarmSettingPara.EtCO2LowValue  = (int)(AlarmSettingPara.EtCO2LowValue * 1.33 + 0.5);
                            AlarmSettingPara.FiCO2HighValue = (int)(AlarmSettingPara.FiCO2HighValue * 1.33 + 0.5);
                            AlarmSettingPara.FiCO2LowValue  = (int)(AlarmSettingPara.FiCO2LowValue * 1.33 + 0.5);                        
                            preETCO2Unit = 0;
                        }
                        break;
                    case 1:
                        if((preETCO2Unit == 0)||(preETCO2Unit == 2))
                        {
                            AlarmSettingPara.EtCO2HighValue = (int)(AlarmSettingPara.EtCO2HighValue / 1.33 + 0.5);
                            AlarmSettingPara.EtCO2LowValue  = (int)(AlarmSettingPara.EtCO2LowValue / 1.33 + 0.5);
                            AlarmSettingPara.FiCO2HighValue = (int)(AlarmSettingPara.FiCO2HighValue / 1.33 + 0.5);
                            AlarmSettingPara.FiCO2LowValue  = (int)(AlarmSettingPara.FiCO2LowValue / 1.33 + 0.5);
                            preETCO2Unit = 1;
                        }
                        break;
                    case 2:
                        if(preETCO2Unit == 1)
                        {
                            AlarmSettingPara.EtCO2HighValue = (int)(AlarmSettingPara.EtCO2HighValue * 1.33 + 0.5);
                            AlarmSettingPara.EtCO2LowValue  = (int)(AlarmSettingPara.EtCO2LowValue * 1.33 + 0.5);
                            AlarmSettingPara.FiCO2HighValue = (int)(AlarmSettingPara.FiCO2HighValue * 1.33 + 0.5);
                            AlarmSettingPara.FiCO2LowValue  = (int)(AlarmSettingPara.FiCO2LowValue * 1.33 + 0.5);
                            preETCO2Unit = 2;
                        }
                        break;
                    default: 
                        break;
                    }
                    
                    //TODO  立即更改所有 ETCO2 的单位
                    
                    if(StandbyState == 0) //ljs add 20130528
                    {
						int index = 0;
						switch(ETCO2Unit)
						{
						case 0:
						default:
							pText = Sys_Lib_GetDispStr(DP_PERCENT);
							break;
						case 1:
							pText = Sys_Lib_GetDispStr(DP_MMHG);
							break;
						case 2:
							pText = Sys_Lib_GetDispStr(DP_KPA);
							break;
						}
						if((index = Get_MonitorRank_AA(DP_CO2)) != -1)
						{		
							MultiText_UnitUpdate(Menu_hWin.HomeWin.Left_Robj[index], pText);
							MultiText_OtherUnitUpdate(Menu_hWin.HomeWin.Left_Robj[index], pText);	
							//  [2/17/2022 yaoyao add]CO2单位切换后，相应的更新快捷监测区CO2上下限数据
							switch (ETCO2Unit)
							{
							case 0:
							case 2:
							default:
								strcpy(pText1, _Float2String((float)AlarmSettingPara.FiCO2HighValue/10,1));	
								strcpy(pText2, _Float2String((float)AlarmSettingPara.FiCO2LowValue/10,1));	
								strcpy(pText3, _Float2String((float)AlarmSettingPara.EtCO2HighValue/10,1));	
								strcpy(pText4, _Float2String((float)AlarmSettingPara.EtCO2LowValue/10,1));	
								break;
							case 1:
								sprintf(pText1, "%d", AlarmSettingPara.FiCO2HighValue);
								sprintf(pText2, "%d", AlarmSettingPara.FiCO2LowValue);
								sprintf(pText3, "%d", AlarmSettingPara.EtCO2HighValue);
								sprintf(pText4, "%d", AlarmSettingPara.EtCO2LowValue);
								break;
							}
							MultiText_MaxValueUpdate(Menu_hWin.HomeWin.Left_Robj[index], pText1);
							MultiText_MinValueUpdate(Menu_hWin.HomeWin.Left_Robj[index], pText2);
							MultiText_MaxValueUpdate1(Menu_hWin.HomeWin.Left_Robj[index], pText3);
							MultiText_MinValueUpdate1(Menu_hWin.HomeWin.Left_Robj[index], pText4);
						}
						//  [2/17/2022 yaoyao 注释]CO2只在左侧监测区 					
						/*if((index = Get_MonitorRank_Rapid(DP_ETCO2)) != -1)
						{
							MultiText_UnitUpdate(Menu_hWin.HomeWin.Robj[index], pText);
						}*/
						// 
						for(i=0;i<=3/*2*/;i++)
						{
							if(Menu_hWin.cWin.WaveForms[i] == WAVEFORM_CO2)
							{
								TracingCurve_SetXYINFO(Menu_hWin.HomeWin.Tobj[i],'T',0,8,1,GUI_WHITE,Sys_Lib_GetDispStr(DP_S),
									0,CO2_Disp.co2max,CO2_Disp.co2step,GUI_WHITE,CO2_Disp.co2_unit,GUI_WHITE,Sys_Lib_GetDispStr(DP_CO2));//CO2
							}
							if(Menu_hWin.cWin.WaveForms[i] == LOOP_V_CO2)
							{
								TracingCurve_SetXYINFO(Menu_hWin.HomeWin.Tobj[i],'T',0,30,5,GUI_WHITE,Sys_Lib_GetDispStr(DP_ML),
									0,CO2_Disp.co2max,CO2_Disp.co2step,GUI_WHITE,CO2_Disp.co2_unit,GUI_WHITE,Sys_Lib_GetDispStr(DP_VCO2));

							}
							Msg.hWin = Menu_hWin.HomeWin.Tobj[i];
							Msg.MsgId = WM_NOTIFICATION_SEL_CHANGED;
							WM_SelectWindow(Msg.hWin);
							WM_SendMessage(Msg.hWin,&Msg); 
						}  

                    }//endof if(StandbyState == 0)
                    DROPDOWN_Collapse(hItem);                    
                }//endof if(DROPDOWN_GetState(hItem))
            }//endof if(!Id)
            else
            {
                switch(Id)
                {
                case GUI_ID_BUTTON0:
                    if(ButtonRotation_GetTriangleSign(pMsg->hWinSrc))
                    {
                        WM_SelectWindow(pMsg->hWinSrc);
                        ButtonRotation_SetTriangleSign(pMsg->hWinSrc,0);
                        Data = ButtonRotation_GetData1(pMsg->hWinSrc);
                        ButtonRotation_SetData(pMsg->hWinSrc,Data);
                        AlarmSettingPara.AlarmVolumeValue = Data;
                        AlarmSettingPara.AlarmVolumeValue = AlarmSettingPara.AlarmVolumeValue/20 - 1;
                        RTEEPROMwrite(ALARM_VOICE_VOLUME_ADDR,AlarmSettingPara.AlarmVolumeValue);//Data);
                        SendOperationMessage(BUZZER_VOLUME_SETTING,USART3);
                    }
                    else
                    {
                        Data = ButtonRotation_GetData(pMsg->hWinSrc);
                        ButtonRotation_SetData1(pMsg->hWinSrc,Data);
                        ButtonRotation_SetTriangleSign(pMsg->hWinSrc,1);
                    }
                    break;
                case GUI_ID_BUTTON1:
                    if(ButtonRotation_GetTriangleSign(pMsg->hWinSrc))
                    {
                        WM_SelectWindow(pMsg->hWinSrc);
                        ButtonRotation_SetTriangleSign(pMsg->hWinSrc,0);
                        Data = ButtonRotation_GetData1(pMsg->hWinSrc);
                        ButtonRotation_SetData(pMsg->hWinSrc,Data);
                        SpO2ModuleOff = Data;
                        RTEEPROMwrite(SPO2_ON_OFF_ADDR,SpO2ModuleOff);//Data);
                        if(Data)
                        {
							//添加报警
                            if(!(AlarmsWork.presentAlarms1 & ALARM_SPO2_MODULE_OFF)) //  20130705
                            {
                                SpO2ModuleClear_FLag = 0;
                                AlarmsWork.presentAlarms1 &= ~ALARM_SPO2_SENSOR_DISCONNECT;
                                AlarmsWork.unclearedAlarms1 &= ~ALARM_SPO2_SENSOR_DISCONNECT;
                                AlarmsWork.presentAlarms1 &= ~ALARM_SPO2_HIGH;
                                AlarmsWork.presentAlarms &= ~ALARM_SPO2_LOW;
                                AlarmsWork.presentAlarms &= ~ALARM_PULSE_RATE_LOW;
                                AlarmsWork.presentAlarms1 &= ~ALARM_PULSE_RATE_HIGH;
                                LowAlarmPriority &= ~0x02;//sun add 20141224
                            }                            
                        }
                        else
                        {
                            AlarmsWork.presentAlarms1 &= ~ALARM_SPO2_MODULE_OFF; 
                            AlarmsWork.unclearedAlarms1 &= ~ALARM_SPO2_MODULE_OFF;
                            LowAlarmPriority &= ~0x20;//sun add 20141224
                        }
                    }
                    else
                    {
                        Data = ButtonRotation_GetData(pMsg->hWinSrc);
                        ButtonRotation_SetData1(pMsg->hWinSrc,Data);
                        ButtonRotation_SetTriangleSign(pMsg->hWinSrc,1);
                    }
                    break;
                case GUI_ID_BUTTON2:
                    if(ButtonRotation_GetTriangleSign(pMsg->hWinSrc))
                    {
                        WM_SelectWindow(pMsg->hWinSrc);
                        ButtonRotation_SetTriangleSign(pMsg->hWinSrc,0);
                        Data = ButtonRotation_GetData1(pMsg->hWinSrc);
                        ButtonRotation_SetData(pMsg->hWinSrc,Data);
                        EtCO2ModuleOff = !Data;
                        RTEEPROMwrite(ETCO2_ON_OFF_ADDR,EtCO2ModuleOff);//Data);
                        if(Data)
                        {
                            if(StandbyState != 0xFF)         //gyh add 20230727 关闭麻气监测后波形恢复默认监测类型
                           {
                             WM_MESSAGE Msg;
                             switch(Menu_hWin.cWin.WaveForms[0])
                             {
                             case WAVEFORM_CO2:
                             case WAVEFORM_SAA:
                             case WAVEFORM_PAA:
                             case WAVEFORM_N2O:
                               Menu_hWin.cWin.WaveForms[0] = WAVEFORM_PRESSURE;
                               CurveMenuInit(&Menu_hWin.cWin,Menu_hWin.HomeWin.ParentWin);
                               break;
                             }
                             switch(Menu_hWin.cWin.WaveForms[1])
                             {
                             case WAVEFORM_CO2:
                             case WAVEFORM_SAA:
                             case WAVEFORM_PAA:
                             case WAVEFORM_N2O:
                               Menu_hWin.cWin.WaveForms[1] = WAVEFORM_FLOW;
                               CurveMenuInit(&Menu_hWin.cWin,Menu_hWin.HomeWin.ParentWin);
                               break;
                             }
                             switch(Menu_hWin.cWin.WaveForms[2])
                             {
                             case WAVEFORM_CO2:
                             case WAVEFORM_SAA:
                             case WAVEFORM_PAA:
                             case WAVEFORM_N2O:
                               Menu_hWin.cWin.WaveForms[2] = WAVEFORM_VOLUME;
							   WaveFormsId2= WAVEFORM_VOLUME;
							   CurveMenuInit(&Menu_hWin.cWin,Menu_hWin.HomeWin.ParentWin);
                               break;
                           }
                         }
                            if(!(AlarmsWork.presentAlarms1 & ALARM_ETCO2_MODULE_OFF)) //  20130705
                            {
                                EtCO2ModuleClear_Flag = 0;
                                AlarmsWork.presentAlarms &= ~ALARM_ETCO2_HIGH;
                                AlarmsWork.presentAlarms1 &= ~ALARM_ETCO2_LOW;
                                AlarmsWork.presentAlarms1 &= ~ALARM_ETCO2_LINE_OCCLUSION;
#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C) 
                                AlarmsWork.presentAlarms &= ~ALARM_FIAA_HIGH;
                                AlarmsWork.presentAlarms &= ~ALARM_ETAA_HIGH;
                                AlarmsWork.presentAlarms1 &= ~ALARM_FIAA_LOW;
                                AlarmsWork.presentAlarms1 &= ~ALARM_ETAA_LOW;
                                AlarmsWork.presentAlarms &= ~ALARM_FIN2O_HIGH;
                                AlarmsWork.presentAlarms1 &= ~ALARM_FIN2O_LOW;
                                AlarmsWork.presentAlarms1 &= ~ALARM_AA_GTE_3MAC;
                                AlarmsWork.presentAlarms1 &= ~ALARM_AA_LESSTHAN_3MAC;
#endif                                
#if (LOGO_FLAG == MSW_LOGO)           
								if(AA_Module_Flag.EtCO2ModuleExist == 1)//是否是国产CO2
								{
									SendOperationMessage(CO2_MODULEOFF,USART1);                                   
								}                                
#else
								if(AA_Module_Flag.ETCO2_FLAG == 1)
								{                               
									SendOperationMessage(CO2_MODULEOFF,USART2);
								}
#endif
                            }

							if(MasimoISA_EtCO2_Config_On || Drager_EtCO2_Config_On)
							{
								SendOperationMessage(SET_AA_PUMPOFF,USART2);
							}
                        }
                        else
                        {
                          
                          
                            AlarmsWork.presentAlarms1 &= ~ALARM_ETCO2_MODULE_OFF;
                            AlarmsWork.unclearedAlarms1 &= ~ALARM_ETCO2_MODULE_OFF;
                            LowAlarmPriority &= ~0x04;//sun add 20141224
                            if(etco2pump_Occlusion == 1)
							{
								CO2_Reset();
								etco2pump_Occlusion = 0;
							}
                            else                            
                            {
#if (LOGO_FLAG == MSW_LOGO)                                 
                                if(AA_Module_Flag.EtCO2ModuleExist == 1)//是否是国产CO2
                                {
									if(StandbyState == 0)
									{
										SendOperationMessage(CO2_MODULEON,USART1);                                   
									}
                                }
#else                             
                                if(AA_Module_Flag.ETCO2_FLAG == 1)
                                {    
									if(StandbyState == 0)
									{
										SendOperationMessage(CO2_MODULEON,USART2);
									}
                                }
#endif                            
                            }
							if(MasimoISA_EtCO2_Config_On || Drager_EtCO2_Config_On)
							{
								if(!StandbyState)//工作下 允许开泵
								{
									SendOperationMessage(SET_AA_PUMPON,USART2);
								}
							}
                        }
// 						if(MasimoISA_EtCO2_Config_On == 2) //ylk modify 20181206
						if((Drager_EtCO2_Config_On == 2) || (MasimoISA_EtCO2_Config_On == 2))
						{
							O2ModuleOff = Data;
							RTEEPROMwrite(O2_ON_OFF_ADDR,O2ModuleOff);
							if(Data)
							{
								//添加报警
								if(!(AlarmsWork.presentAlarms1 & ALARM_O2_MODULE_OFF)) //  20130705
								{
									O2ModuleClear_FLag = 0;
									AlarmsWork.presentAlarms &= ~ALARM_FIO2_HIGH;
									AlarmsWork.presentAlarms &= ~ALARM_FIO2_LOW;
									AlarmsWork.presentAlarms &= ~ALARM_FIO2_LESSTHAN_18;
								}
							}
							else
							{
								AlarmsWork.presentAlarms1 &= ~ALARM_O2_MODULE_OFF;
								AlarmsWork.unclearedAlarms1 &= ~ALARM_O2_MODULE_OFF;
								LowAlarmPriority &= ~0x10;//sun add 20141224
							}
						}
                    }
                    else
                    {
                        Data = ButtonRotation_GetData(pMsg->hWinSrc);
                        ButtonRotation_SetData1(pMsg->hWinSrc,Data);
                        ButtonRotation_SetTriangleSign(pMsg->hWinSrc,1);
                    }
                    break;
                case GUI_ID_BUTTON3://djq add 20131226
                   if(ButtonRotation_GetTriangleSign(pMsg->hWinSrc))
                    {
                        WM_SelectWindow(pMsg->hWinSrc);
                        ButtonRotation_SetTriangleSign(pMsg->hWinSrc,0);
                        Data = ButtonRotation_GetData1(pMsg->hWinSrc);
                        ButtonRotation_SetData(pMsg->hWinSrc,Data);
                        O2ModuleOff = Data;
                        RTEEPROMwrite(O2_ON_OFF_ADDR,O2ModuleOff);
                        if(Data)
                        {
							//添加报警
                            if(!(AlarmsWork.presentAlarms1 & ALARM_O2_MODULE_OFF)) //  20130705
                            {
                                O2ModuleClear_FLag = 0;
                                AlarmsWork.presentAlarms &= ~ALARM_FIO2_HIGH;
                                AlarmsWork.presentAlarms &= ~ALARM_FIO2_LOW;
								AlarmsWork.presentAlarms &= ~ALARM_FIO2_LESSTHAN_18;
                            }
                        }
                        else
                        {
                            AlarmsWork.presentAlarms1 &= ~ALARM_O2_MODULE_OFF;
                            AlarmsWork.unclearedAlarms1 &= ~ALARM_O2_MODULE_OFF;
                            LowAlarmPriority &= ~0x10;//sun add 20141224
                        }
                    }
                    else
                    {
                        Data = ButtonRotation_GetData(pMsg->hWinSrc);
                        ButtonRotation_SetData1(pMsg->hWinSrc,Data);
                        ButtonRotation_SetTriangleSign(pMsg->hWinSrc,1);
                    }
					break;
                case GUI_ID_BUTTON4://zkq  add 20170407 空气气源开关
                   if(ButtonRotation_GetTriangleSign(pMsg->hWinSrc))
                   {
                        WM_SelectWindow(pMsg->hWinSrc);
                        ButtonRotation_SetTriangleSign(pMsg->hWinSrc,0);
                        Data = ButtonRotation_GetData1(pMsg->hWinSrc);
						WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON10));//More
						WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON11));//Comp.
                        if(Data)
                        {
                            //空气作为驱动气的时候，不能关闭气源开关
						    if(DirverGasFlag)//空气为驱动气
						    {
						        MasimoMenu_Flag = 10;
                                ButtonRotation_SetData(pMsg->hWinSrc,0);
						    }
						    else
						    {
						        MasimoMenu_Flag = 11;
                                ButtonRotation_SetData(pMsg->hWinSrc,Data);
						    }
                        }
                        else
                        {
                            AlarmAirSourceSwitch = !Data;
                            ButtonRotation_SetData(pMsg->hWinSrc,Data);
                            RTEEPROMwrite(AIR_SUPPLY_SWITCH_ADDR,AlarmAirSourceSwitch);
                            if(DriverGas_Chose_Flag)//djq add 20170609驱动气体选配
                            {
                                if((AlarmsWork.presentAlarms & ALARM_OXY_SOURCE_LOW)
                                  &&(!DirverGasFlag))
                                { 
                                    DirverGasFlag = 1;
                                    RTEEPROMwrite(DIRVER_GAS_ADDR,DirverGasFlag);
                                    //向BDU发送驱动器类型
                                    SendOperationMessage(DIRVER_GAS_FOR_BDU_COM,USART0);
                                }
                                Menu_hWin.InfoWin.InfoChangeFlag |= DRIVER_GAS_TYPE_FLAG;
                                WM_SendMessageNoPara(hDlg,WM_INIT_DIALOG);
                            }
                        }
                    }
                   else
                   {
                       Data = ButtonRotation_GetData(pMsg->hWinSrc);
                       ButtonRotation_SetData1(pMsg->hWinSrc,Data);
                       ButtonRotation_SetTriangleSign(pMsg->hWinSrc,1);
                   }
                    break;
                case GUI_ID_BUTTON5://驱动气开关
                    MasimoMenu_Flag = 14;
					WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON10));//More
					WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON11));//Comp.
                    if(ButtonRotation_GetTriangleSign(pMsg->hWinSrc))
                    {
                        WM_SelectWindow(pMsg->hWinSrc);
                        ButtonRotation_SetTriangleSign(pMsg->hWinSrc,0);
                        value = ButtonRotation_GetData1(pMsg->hWinSrc);
                    }
                    else
                    {
                        Data = ButtonRotation_GetData(pMsg->hWinSrc);
                        ButtonRotation_SetData1(pMsg->hWinSrc,Data);
                        ButtonRotation_SetTriangleSign(pMsg->hWinSrc,1);
                    }
                    break;
                case GUI_ID_BUTTON9://SIGH
                    if(ButtonRotation_GetTriangleSign(pMsg->hWinSrc))
                    {
                        ButtonRotation_SetTriangleSign(pMsg->hWinSrc,0);
                        value = ButtonRotation_GetData1(pMsg->hWinSrc);
                        ButtonRotation_SetData(pMsg->hWinSrc,value);
                        BeathSettingPara.SettingSighTimesValue = value;
						BreathPara_Assign(&Breath_Setting_Tmp, &BeathSettingPara);

                        SendOperationMessage(SIGH_TIMES_UPDATE,USART0);

                    }
                    else
                    {
                        Data = ButtonRotation_GetData(pMsg->hWinSrc);
                        ButtonRotation_SetData1(pMsg->hWinSrc,Data);
                        ButtonRotation_SetTriangleSign(pMsg->hWinSrc,1);
                    }
                    break;
                case GUI_ID_DROPDOWN0:
                case GUI_ID_DROPDOWN1:
                case GUI_ID_DROPDOWN2:
                case GUI_ID_DROPDOWN3:
                    break;
                default:break;
                }
            }
            break; //endof case GUI_KEY_ENTER
        }
        break;//endof case WM_NOTIFY_PARENT
    default:
        WM_DefaultProc(pMsg);
    }
    if(MasimoMenu_Flag == 1)
    {
		ShowFlagNum = 0;
        MasimoMenu_Flag = 0;
		WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_TEXT4));
	    WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON6));
		WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON7));
		WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON8));

		if(Drager_EtCO2_Config_On)
		{
			WM_ShowWindow(WM_GetDialogItem(hDlg, GUI_ID_TEXT0));
		}
		WM_ShowWindow(WM_GetDialogItem(hDlg, GUI_ID_TEXT1));
		WM_ShowWindow(WM_GetDialogItem(hDlg, GUI_ID_TEXT2));
        WM_ShowWindow(WM_GetDialogItem(hDlg, GUI_ID_TEXT3));
		WM_ShowWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON0));
#if (LOGO_FLAG != MSW_LOGO)
		WM_ShowWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON1));
#endif
		WM_ShowWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON2));
        WM_ShowWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON3));
        WM_ShowWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON4));
        WM_ShowWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON5));
        WM_ShowWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON9));
		WM_ShowWindow(WM_GetDialogItem(hDlg, GUI_ID_DROPDOWN1));
        WM_ShowWindow(WM_GetDialogItem(hDlg, GUI_ID_DROPDOWN2));
        WM_ShowWindow(WM_GetDialogItem(hDlg, GUI_ID_DROPDOWN3));
		WM_ShowWindow(WM_GetDialogItem(hDlg, GUI_ID_TEXT5));//Loudness
		if(DriverGas_Chose_Flag)
		{
			WM_ShowWindow(WM_GetDialogItem(hDlg, GUI_ID_TEXT6));//Driver
		}
    }
    else if((MasimoMenu_Flag >= 10) && (MasimoMenu_Flag <= 14))//气源开关、驱动气体开关
    {
        WM_ShowWindow(WM_GetDialogItem(hDlg, GUI_ID_TEXT4));
		if(MasimoMenu_Flag == 10)
		{
		    WM_ShowWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON8));
		}
		else
		{
 			WM_ShowWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON6));
			WM_ShowWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON7));
		}
        if(MasimoMenu_Flag == 10)//空气为驱动气时
		{
			ShowFlagNum = 2;
			if(DirverGasFlag)
			{
				//提示驱动气体为空气,不能关闭开关
				TEXT_SetText(WM_GetDialogItem(hDlg, GUI_ID_TEXT4), Sys_Lib_GetDispStr(DP_DRIVEGASISAIR));
			}
		}
		else if(MasimoMenu_Flag == 11)
		{
			ShowFlagNum = 3;
            //是否关闭空气气源开关?
			TEXT_SetText(WM_GetDialogItem(hDlg, GUI_ID_TEXT4), Sys_Lib_GetDispStr(DP_CLOSEAIRSWITCH));
		}
		else if(MasimoMenu_Flag == 14)
		{   
            //是否更改驱动气体
			TEXT_SetText(WM_GetDialogItem(hDlg, GUI_ID_TEXT4), Sys_Lib_GetDispStr(DP_CHANGEDRIVERGAS));
			ShowFlagNum = 6;
		}
       	WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_TEXT0));
		WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_TEXT1));
		WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_TEXT2));
		WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_TEXT3));
		WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON0));
#if (LOGO_FLAG != MSW_LOGO)
		WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON1));
#endif
		WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON2));
        WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON3));
        WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON4));
        WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON5));
        WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON9));
		WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_DROPDOWN1));
        WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_DROPDOWN2));
        WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_DROPDOWN3));
		WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_TEXT5));//Loudness
		if(DriverGas_Chose_Flag)
		{
			WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_TEXT6));//Dirver
		}
    }
}

/***************************************************************************************************
* @brief  查叹息时间表
* @para   
* @retval 
*/
int CheckSighTimesTable(int data)
{
    int index = 0;
    while(data!=SighTimesTable[index])
    {
        index++;
        if(index>4)
        {
            break;
        }
    }
    return index;
}
/***************************************************************************************************
* @brief  System -> Calibration
* @para   
* @retval 
*/
static void _cbDialog5(WM_MESSAGE * pMsg)
{
    WM_HWIN hDlg,hItem,hButton0,hButton1,hButton2,hButton3,hButton4,hButton5;
    int Id,NCode;
    hDlg = pMsg->hWin;
    switch (pMsg->MsgId)
    {
    case WM_INIT_DIALOG:
        hButton0 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON0);
        BUTTON_SetBkColor(hButton0,0,0xD05356);
        hButton1 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON1);
        BUTTON_SetBkColor(hButton1,0,0xD05356);
        hButton2 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON2);
        BUTTON_SetBkColor(hButton2,0,0xD05356);
        hButton3 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON3);
        BUTTON_SetBkColor(hButton3,0,0xD05356);
        hButton4 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON4);
        BUTTON_SetBkColor(hButton4,0,0xD05356);
        hButton5 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON5);
        BUTTON_SetBkColor(hButton5,0,0xD05356);
        hItem = WM_GetDialogItem(hDlg, GUI_ID_PROGBAR0);
        PROGBAR_SetBarColor(hItem,1,GUI_YAN_BLUE);
        PROGBAR_SetBarColor(hItem,0,GUI_BLUE);
        PROGBAR_SetFont(hItem,&GUI_Font_16);
        WM_HideWindow(hItem);

		//lyy 20240429 modify
// #if(LOGO_FLAG == PENLON_LOGO)    //gyh add 20230714 零点校验在masimo主流旁流下都可进行    
                if(AA_Module_Flag.PhaseInTypeModuleExist == 6 || AA_Module_Flag.PhaseInTypeModuleExist == 0)
				{
					WM_HideWindow(hButton5);
				}
				else
				{
					WM_ShowWindow(hButton5);
					if (MasimoISA_EtCO2_Config_On)
					{
						if (PhaseInISA_Init.PhaseInISA_Flag&&AA_Module_Flag.PhaseInTypeModuleExist)
						{
							WM_EnableWindow(hButton5);
						}
						else
						{
							WM_DisableWindow(hButton5);
						}
					}
					else
					{
						if (AA_Module_Flag.PhaseInTypeModuleExist)
						{
							WM_EnableWindow(hButton5);
						}
						else
						{
							WM_DisableWindow(hButton5);
						}
					}
				}
// #else
// 		if(AA_Module_Flag.EtCO2ModuleExist == 2)//masimo IRMA
// 		{
// 			if((AA_all_data.AA_state.AA_ModuleState.zero_state == disable) ||(AA_all_data.AA_state.AA_ModuleState.zero_state == zero_zeroing))  //sun add 20140708
// 			{
// 				WM_DisableWindow(hButton5);
// 			}
// 			else
// 			{
// 				WM_EnableWindow(hButton5);
// 			}
// 		}
// 		else if((AA_Module_Flag.EtCO2ModuleExist == 2)&&(AA_Module_Flag.PhaseInTypeModuleExist==2))
// 		{
// 			WM_EnableWindow(hButton5);
// 		}
// 		else
// 		{
// 			WM_DisableWindow(hButton5);
// 		}
// #endif
                
// 	if (Masimo_EtCO2_Config_On && (AA_Module_Flag.PhaseInTypeModuleExist == Masimo_EtCO2_zhuliu || AA_Module_Flag.PhaseInTypeModuleExist == Masimo_EtCO2_zhuliuco2))
// 	{
// 		WM_ShowWindow(hButton5);
// 		if((AA_all_data.AA_state.AA_ModuleState.zero_state == disable) ||(AA_all_data.AA_state.AA_ModuleState.zero_state == zero_zeroing))
// 		{
// 			WM_DisableWindow(hButton5);
// 		}
// 		else
// 		{
// 			WM_EnableWindow(hButton5);
// 		}
// 	}
// 	else
// 	{
// 		WM_HideWindow(hButton5);
// 	}


#if(MACHINE_TYPE == X6 || MACHINE_TYPE == Z6)
		WM_HideWindow(hButton5);//ylk add 20170906 
#endif

        BUTTON_SetText(hButton3,Sys_Lib_GetDispStr(DP_SERVICEMODES));
        BUTTON_SetText(hButton2,Sys_Lib_GetDispStr(DP_O2CELLCAL));
		BUTTON_SetText(hButton5,Sys_Lib_GetDispStr(DP_GASMODULEZEROCAL));
		BUTTON_SetText(hButton0,Sys_Lib_GetDispStr(DP_PRESSURESENSORCAL));
		BUTTON_SetText(hButton1,Sys_Lib_GetDispStr(DP_FLOWSENSORCAL));
		BUTTON_SetText(hButton4,Sys_Lib_GetDispStr(DP_TOUCHSCREENCAL));  

		if(!FiO2_Config_On)
		{
			WM_DisableWindow(hButton2);
		}
		else
		{
			WM_EnableWindow(hButton2);
		}

		if(MasimoISA_EtCO2_Config_On == 2)
		{
			if(!PhaseInISA_Init.PhaseInISA_Flag)
			{
				WM_DisableWindow(hButton2);
// 				WM_DisableWindow(hButton5);
			}
			else
			{
				WM_EnableWindow(hButton2);//ylk modify 
// 				WM_EnableWindow(hButton5);
			}
		}
 
        break;
    case WM_NOTIFY_PARENT:
        NCode = pMsg->Data.v;
        Id = WM_GetId(pMsg->hWinSrc);
        switch(NCode)
        {
        case WM_NOTIFICATION_CLICKED:
            if(  (!(MenuOperateStatus & CALIBRATION_PRECEED)) 
              && (!(MenuOperateStatus & FRAMEWIN_POP)) )
            {
                switch(Id)
                {
                case GUI_ID_BUTTON0:
                    hItem = WM_GetDialogItem(Menu_hWin.ActivedWin, GUI_ID_MULTIPAGE0);
                    MULTIPAGE_SetPageChangeEnable(hItem,0);
                    MenuOperateStatus |= CALIBRATION_PRECEED;
                    SendOperationMessage(PRESSURE_CALIBRATION,USART0);
                    CalibrationType = PRESSURE_CALIBRATION;
                    hItem=WM_GetDialogItem(hDlg, GUI_ID_PROGBAR0);
                    WM_ShowWindow(hItem);
					PROGBAR_SetText(hItem,Sys_Lib_GetDispStr(DP_CALIBRATING));
                    break;
                case GUI_ID_BUTTON1:
                    hItem = WM_GetDialogItem(Menu_hWin.ActivedWin, GUI_ID_MULTIPAGE0);
                    MULTIPAGE_SetPageChangeEnable(hItem,0);
                    MenuOperateStatus |= CALIBRATION_PRECEED;
                    SendOperationMessage(FLOW_CALIBRATION,USART0);
                    CalibrationType = FLOW_CALIBRATION;
                    hItem=WM_GetDialogItem(hDlg, GUI_ID_PROGBAR0);
                    WM_ShowWindow(hItem);
					PROGBAR_SetText(hItem,Sys_Lib_GetDispStr(DP_CALIBRATING));
                    break;
                case GUI_ID_BUTTON2:
                    //*//氧浓度传感器校验对话框模式 //  20130417
                    hItem = WM_GetDialogItem(Menu_hWin.ActivedWin, GUI_ID_MULTIPAGE0);
                    MULTIPAGE_SetPageChangeEnable(hItem,0);
					//ISA选配并且已经连接上
					if((MasimoISA_EtCO2_Config_On == 2) && (AA_Module_Flag.EtCO2ModuleExist == 2)&&(AA_Module_Flag.PhaseInTypeModuleExist==2))
					{
						SendOperationMessage(SET_AA_PUMPON,USART2);//开泵
						oxyChooseFlag = 0;
						PlanMenuType = OXYGEN_SENSOR_CAL_MENU;
					}
					else 
					{
						PlanMenuType = OXY_CHOOSE_MENU;  //sun add 20140529
					}
					Task |= MEAU_UPDATE;
                    hItem = WM_GetDialogItem(hDlg, GUI_ID_PROGBAR0);//cpp del 20130118
                    WM_HideWindow(hItem);
#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C)
                    hItem = WM_GetDialogItem(hDlg, GUI_ID_BUTTON4);//cpp del 20130118
                    WM_HideWindow(hItem);
#endif
                    MenuOperateStatus |= FRAMEWIN_POP;
                    break;
                case GUI_ID_BUTTON3:
                    hItem = WM_GetDialogItem(Menu_hWin.ActivedWin, GUI_ID_MULTIPAGE0);
                    MULTIPAGE_SetPageChangeEnable(hItem,0);
                    PlanMenuType = ENGINEER_MENU;
                    Task |=MEAU_UPDATE;
                    hItem=WM_GetDialogItem(hDlg, GUI_ID_PROGBAR0);
                    WM_HideWindow(hItem);
#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C)
                    hItem = WM_GetDialogItem(hDlg, GUI_ID_BUTTON4);
                    WM_HideWindow(hItem);
#endif
                    //hItem = WM_GetDialogItem(hDlg, GUI_ID_BUTTON5);
                    //WM_HideWindow(hItem);
                    MenuOperateStatus |= FRAMEWIN_POP;
                    break;
                case GUI_ID_BUTTON4:
                    TouchScreenCalibrate = 1;
                    PlanMenuType = TOUCH_CALIBRATION_MENU;
                    Task |=MEAU_UPDATE;
                    break;
                case GUI_ID_BUTTON5:  //sun add 20140707
					//进入校验界面后发送开泵命令
					if((MasimoISA_EtCO2_Config_On || Masimo_EtCO2_Config_On) && (AA_Module_Flag.EtCO2ModuleExist == 2)
						&&AA_Module_Flag.PhaseInTypeModuleExist)//选配了Masimo ISA
					{
						SendOperationMessage(SET_AA_PUMPON,USART2);
					}
                    hItem = WM_GetDialogItem(Menu_hWin.ActivedWin, GUI_ID_MULTIPAGE0);
                    MULTIPAGE_SetPageChangeEnable(hItem,0);
                    hItem=WM_GetDialogItem(hDlg, GUI_ID_PROGBAR0);
                    WM_HideWindow(hItem);
                    PlanMenuType = PHASEIN_ZERO_CAL_MENU;
                    Task |=MEAU_UPDATE;
                    MenuOperateStatus |= FRAMEWIN_POP;
                    break;
                }
                //break;
            }
            else
            {
                switch(Id)
                {
                case GUI_ID_BUTTON0:
                case GUI_ID_BUTTON1:
                case GUI_ID_BUTTON2:
                case GUI_ID_BUTTON3:
                case GUI_ID_BUTTON4:
                case GUI_ID_BUTTON5:
                    BUTTON_SetPressed(pMsg->hWinSrc,0);
                    break;
                }
            }
			break;//
        }
        break;
    default:
        WM_DefaultProc(pMsg);
    }
      }
//返回29：闰年   返回28：平年
extern unsigned short GUI_TOUCH_CalLeft;
extern unsigned short GUI_TOUCH_CalRight;
extern unsigned short GUI_TOUCH_CalTop;
extern unsigned short GUI_TOUCH_CalBottom;

int LeapYear(int year)
{
    int ret;
    if(((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0))
    {
        ret = 29;
    }
    else 
    {
        ret = 28;
    }
    return ret;
}
int GetThisYears_OfDay(int year,int month)
{
    int ret;
    if((month == 4)||(month == 6)||(month == 9)||(month == 11))
    {
        ret = 30;
    }
    else if(month == 2)
    {
        ret = LeapYear(year);
    }
    else
    {
        ret = 31;
    }
    return ret;
}
void __DateDialogInit(WM_HWIN hDlg,int init_flag)//用于设置时间日期窗口的初始化
{
    WM_HWIN  hItem0,hItem1,hItem2,hItem3,hItem4;
    RTC_TIMER time;
    int years = 0,months = 0,days = 0,dates = 0;
    
    hItem0 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON0);
    hItem1 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON1);
    hItem2 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON2);
    hItem3 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON3);
    hItem4 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON4);
    time = RTCTime;
    dates = GetThisYears_OfDay(time.RTC_Year,time.RTC_Mon);

    if(init_flag == 1)
    {
        ButtonRotation_SetDataInfo(hItem0,2015,2065,time.RTC_Year,1,0);
        ButtonRotation_SetData(hItem0,time.RTC_Year);
    
        ButtonRotation_SetDataInfo(hItem1,1,12,time.RTC_Mon,1,0);
        ButtonRotation_SetData(hItem1,time.RTC_Mon);
    
        ButtonRotation_SetDataInfo(hItem2,1,dates,time.RTC_Mday,1,0);
        if(time.RTC_Mday >= dates)
        {
            ButtonRotation_SetData(hItem2,dates);
        }
        else
        {
            ButtonRotation_SetData(hItem2,time.RTC_Mday);
        }
    
        ButtonRotation_SetDataInfo(hItem3,0,23,time.RTC_Hour,1,0);
        ButtonRotation_SetData(hItem3,time.RTC_Hour);
    
        ButtonRotation_SetDataInfo(hItem4,0,59,time.RTC_Min,1,0);
        ButtonRotation_SetData(hItem4,time.RTC_Min);
    }
    else
    {
        years = ButtonRotation_GetData(hItem0);
        months = ButtonRotation_GetData(hItem1);
        days = ButtonRotation_GetData(hItem2);
        dates = GetThisYears_OfDay(years,months);
        ButtonRotation_SetDataInfo(hItem2,1,dates,time.RTC_Mday,1,0);
        if(days > dates)
        {
            ButtonRotation_SetData(hItem2,dates);
        }
    }
}
/***************************************************************************************************
* @brief  System -> Date&time
* @para   
* @retval 
*/
static void _cbDialog6(WM_MESSAGE * pMsg)
{
    WM_HWIN hDlg,hItem0,hItem1,hItem2,hItem3,hItem4,hItem5,i;
	WM_HWIN tWin,hCancel,hConform,hPrompt; //  20130604
	char ShowFlag = 0;
    int Id,NCode,flg =0;
    int Data;
    RTC_TIMER time;
    WM_MESSAGE Msg;
    //char pText[5]= {0};
    hDlg = pMsg->hWin;
    switch (pMsg->MsgId)
    {
    case WM_INIT_DIALOG:
        hItem0 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON0);
        hItem1 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON1);
        hItem2 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON2);
        hItem3 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON3);
        hItem4 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON4);
		hItem5 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON5);
		BUTTON_SetBkColor(hItem5,0,0xD05356);
		hCancel  = WM_GetDialogItem(hDlg, GUI_ID_BUTTON6);//
		BUTTON_SetTextAlign(hCancel,GUI_TA_HCENTER|GUI_TA_VCENTER);
		//BUTTON_SetBkColor(hCancel,0,0xD05356);
		hConform = WM_GetDialogItem(hDlg, GUI_ID_BUTTON7);//
		BUTTON_SetTextAlign(hConform,GUI_TA_HCENTER|GUI_TA_VCENTER);
		//BUTTON_SetBkColor(hConform,0,0xD05356);
		hPrompt  = WM_GetDialogItem(hDlg, GUI_ID_TEXT6);
        __DateDialogInit(hDlg,1);
		ShowFlag = 1;
        ButtonRotation_SetText(hItem0,Sys_Lib_GetDispStr(DP_YEAR),"");
        ButtonRotation_SetText(hItem1,Sys_Lib_GetDispStr(DP_MONTH),"");
        ButtonRotation_SetText(hItem2,Sys_Lib_GetDispStr(DP_DAY),"");
        ButtonRotation_SetText(hItem3,Sys_Lib_GetDispStr(DP_HOUR),"");
        ButtonRotation_SetText(hItem4,Sys_Lib_GetDispStr(DP_MINUTE),"");
        BUTTON_SetText(hItem5,Sys_Lib_GetDispStr(DP_APPLY));

		BUTTON_SetText(hCancel,Sys_Lib_GetDispStr(DP_CANCEL));
		BUTTON_SetText(hConform,Sys_Lib_GetDispStr(DP_CONFIRM));
		TEXT_SetText(hPrompt,Sys_Lib_GetDispStr(DP_MODIFYINGTHETIMEWILL));
 
        break;
    case WM_NOTIFY_PARENT:
        NCode = pMsg->Data.v;
        Id = WM_GetId(pMsg->hWinSrc);
        switch(NCode)
        {
        case WM_NOTIFICATION_LOST_FOCUS: //  20130604
            tWin = WM_GetDialogItem(hDlg,GUI_ID_BUTTON0);
            while(!ButtonRotation_GetTriangleSign(tWin))
            {
                tWin = WM_GetNextSibling(tWin);
                if(tWin==0)
                {
                    flg = 1;
                    break;
                }
            }
            if(!flg)
            {
                ButtonRotation_SetTriangleSign(tWin,0);
            }
            break;
        
        case WM_NOTIFICATION_CLICKED:
            switch(Id)
            {
            case GUI_ID_BUTTON5:
                i = WM_GetDialogItem(hDlg,GUI_ID_BUTTON0);
                while((!ButtonRotation_GetTriangleSign(i)))
                {
                    i = WM_GetNextSibling(i);
                    if(i==0)
                    {
                        flg = 1;
                        break;
                    }
                }
                if(!flg)
                {
                    ButtonRotation_SetTriangleSign(i,0);
                }
				BUTTON_SetPressed(pMsg->hWinSrc,0);
				ShowFlag = 2;
				//BUTTON_SetPressed(hItem0,0);
				break;
			case GUI_ID_BUTTON6:
				ShowFlag = 1;
				BUTTON_SetPressed(pMsg->hWinSrc,0);
				break;
			case GUI_ID_BUTTON7:
				hItem0 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON0);
				time.RTC_Year = ButtonRotation_GetData(hItem0);
				hItem0 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON1);
				time.RTC_Mon = ButtonRotation_GetData(hItem0);
				hItem0 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON2);
				time.RTC_Mday = ButtonRotation_GetData(hItem0);
				hItem0 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON3);
				time.RTC_Hour = ButtonRotation_GetData(hItem0);
				hItem0 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON4);
				time.RTC_Min = ButtonRotation_GetData(hItem0);
				RTCSetTime(time);
				
				ClearTrendInfo();

				ShowFlag = 1;
				BUTTON_SetPressed(pMsg->hWinSrc,0);
				break;
            case GUI_ID_BUTTON0:
            case GUI_ID_BUTTON1:
            case GUI_ID_BUTTON2:
            case GUI_ID_BUTTON3:
            case GUI_ID_BUTTON4:
                if(ButtonRotation_GetTriangleSign(pMsg->hWinSrc))
                {
                    ButtonRotation_SetTriangleSign(pMsg->hWinSrc,0);
                    Data = ButtonRotation_GetData1(pMsg->hWinSrc);
                    ButtonRotation_SetData(pMsg->hWinSrc,Data);
                    __DateDialogInit(hDlg,0);
                }
                else
                {
                    i = WM_GetDialogItem(hDlg,GUI_ID_BUTTON0);
                    while((!ButtonRotation_GetTriangleSign(i)))
                    {
                        i = WM_GetNextSibling(i);
                        if(i==0)
                        {
                            flg = 1;
                            break;
                        }
                    }
                    if(!flg)
                    {
                        ButtonRotation_SetTriangleSign(i,0);
                    }
                    Data = ButtonRotation_GetData(pMsg->hWinSrc);
                    ButtonRotation_SetData1(pMsg->hWinSrc,Data);
                    WM_SetFocus(pMsg->hWinSrc);
                    ButtonRotation_SetTriangleSign(pMsg->hWinSrc,1);
                }
                break;
            }
            break;
        case GUI_KEY_LEFT:
            switch(Id)
            {
            case GUI_ID_BUTTON0:
            case GUI_ID_BUTTON1:
            case GUI_ID_BUTTON2:
            case GUI_ID_BUTTON3:
            case GUI_ID_BUTTON4:
                if(ButtonRotation_GetTriangleSign(pMsg->hWinSrc))//if setting now?
                {
                    ButtonRotation_DataSub_1(pMsg->hWinSrc);
                    Msg.MsgId   = WM_NOTIFICATION_VALUE_CHANGED;
                    Msg.Data.v  = GUI_KEY_LEFT;
                    Msg.hWin = pMsg->hWinSrc;
                    WM_SendMessage(pMsg->hWinSrc,&Msg);
                }
                break;
            }
            break;
        case GUI_KEY_RIGHT:
            switch(Id)
            {
            case GUI_ID_BUTTON0:
            case GUI_ID_BUTTON1:
            case GUI_ID_BUTTON2:
            case GUI_ID_BUTTON3:
            case GUI_ID_BUTTON4:
                if(ButtonRotation_GetTriangleSign(pMsg->hWinSrc))//if setting now?
                {
                    ButtonRotation_DataAdd_1(pMsg->hWinSrc);
                    Msg.MsgId   = WM_NOTIFICATION_VALUE_CHANGED;
                    Msg.Data.v  = GUI_KEY_RIGHT;
                    Msg.hWin = pMsg->hWinSrc;
                    WM_SendMessage(pMsg->hWinSrc,&Msg);
                }
                break;
            }
            break;
        case GUI_KEY_ENTER:
            switch(Id)
            {
            case GUI_ID_BUTTON0:
            case GUI_ID_BUTTON1:
            case GUI_ID_BUTTON2:
            case GUI_ID_BUTTON3:
            case GUI_ID_BUTTON4:
                if(ButtonRotation_GetTriangleSign(pMsg->hWinSrc))//if setting now?
                {
                    ButtonRotation_SetTriangleSign(pMsg->hWinSrc,0);
                    Data = ButtonRotation_GetData1(pMsg->hWinSrc);
                    ButtonRotation_SetData(pMsg->hWinSrc,Data);
                    __DateDialogInit(hDlg,0);
                }
                else
                {
                    i = WM_GetDialogItem(hDlg,GUI_ID_BUTTON0);
                    while((!ButtonRotation_GetTriangleSign(i)))
                    {
                        i = WM_GetNextSibling(i);
                        if(i==0)
                        {
                            flg = 1;
                            break;
                        }
                    }
                    if(!flg)
                    {
                        ButtonRotation_SetTriangleSign(i,0);
                    }
                    Data = ButtonRotation_GetData(pMsg->hWinSrc);
                    ButtonRotation_SetData1(pMsg->hWinSrc,Data);
                    WM_SetFocus(pMsg->hWinSrc);
                    ButtonRotation_SetTriangleSign(pMsg->hWinSrc,1);
                }
                break;
            }
            break;
        case WM_NOTIFICATION_RELEASED:
            if(BUTTON_IsPressed(pMsg->hWinSrc))
            {
                BUTTON_SetPressed(pMsg->hWinSrc,0);
            }
        }
        break;
    default:
        WM_DefaultProc(pMsg);
    }

	if(ShowFlag == 1)
	{
		WM_ShowWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON0));
		WM_ShowWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON1));
		WM_ShowWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON2));
		WM_ShowWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON3));
		WM_ShowWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON4));
		WM_ShowWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON5));

		WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_TEXT6));
		WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON6));
		WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON7));
	}
	if(ShowFlag == 2)
	{
		WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON0));
		WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON1));
		WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON2));
		WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON3));
		WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON4));
		WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON5));

		WM_ShowWindow(WM_GetDialogItem(hDlg, GUI_ID_TEXT6));
		WM_ShowWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON6));
		WM_ShowWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON7));
	}

}

static void _Creat_AlarmLog(WM_HWIN hItem, int Start, int Num)
{
	char pText[100]= {0};
	int cnt = 0;
	cnt = LISTBOX_GetNumItems(hItem);
	if(cnt)
	{
		while(cnt)
		{
			cnt--;
			LISTBOX_DeleteItem(hItem,cnt);
		}
	}
	if ((Num >0)&&(Start > 0)&&(Start + Num - 1 <= MAX_RECORD_NUM))
	{
		struct list_node *tmp_node = alarmlog_head.head;
		for (cnt = 0; cnt<Start-1; cnt++)
		{
			if (tmp_node->next != NULL)
			{
				tmp_node = tmp_node->next;
			}
		}
		for (cnt = Start; cnt < Start + Num; cnt++)
		{
			GetAlarmRecordInfo(hItem,tmp_node,pText);
			if (tmp_node->next != NULL)
			{
				tmp_node = tmp_node->next;
			}
		}
		LISTBOX_SetSel(hItem,0);
	}

}
/***************************************************************************************************
* @brief  Alarms -> Alarm Log
* @para   
* @retval 
*/
static void _cbDialog7(WM_MESSAGE * pMsg)
{
    WM_HWIN hDlg,hItem,hWin,hText0,hText1,hText2,hText3;
    unsigned char num,cnt;
    int Id,NCode;
	char pText[20]= {0};
    hDlg = pMsg->hWin;
    switch (pMsg->MsgId)
    {
    case WM_INIT_DIALOG:
        WM_SelectWindow(hDlg);
        GUI_SetTextMode(GUI_TA_LEFT|GUI_TA_TOP);

        hItem = WM_GetDialogItem(hDlg, GUI_ID_LISTBOX0);
		LISTBOX_SetBitmap(hItem, &bmalrmlog_bell);
		LISTBOX_SetOff_X(hItem, 65);	
// 		LISTBOX_SetOff_Y(hItem, 2/*3*/);
// 		LISTBOX_SetItemSpacing(hItem,2/*8*/);

        hWin = WM_GetDialogItem(hDlg, GUI_ID_VSCROLL);
		SCROLLBAR_SetArrow(hWin, 1);//区分下拉菜单中的滚轴和AlarmLog里的滚轴
        SCROLLBAR_SetPageSize(hWin,1);

        if(AlarmRecordList.Count)
        {
			if (AlarmRecordList.Count > RECORD_EVERY_PAGE)
			{
				cnt = RECORD_EVERY_PAGE;
			}
			else
			{
				cnt = AlarmRecordList.Count;
			}
			_Creat_AlarmLog(hItem,1,cnt);

            LISTBOX_SetSel(hItem,0);
            num = AlarmRecordList.Count / RECORD_EVERY_PAGE;
            if((AlarmRecordList.Count % RECORD_EVERY_PAGE)!=0)
            {
                num += 1;
            }
            SCROLLBAR_SetNumItems(hWin,num);
            SCROLLBAR_SetValue(hWin,0);
        }
        else
        {
            SCROLLBAR_SetNumItems(hWin,0);
            SCROLLBAR_SetValue(hWin,0);
        }
        hText0 = WM_GetDialogItem(hDlg, GUI_ID_TEXT0);
        hText1 = WM_GetDialogItem(hDlg, GUI_ID_TEXT1);
        hText2 = WM_GetDialogItem(hDlg, GUI_ID_TEXT2);
        hText3 = WM_GetDialogItem(hDlg, GUI_ID_TEXT3);        
        
        sprintf(pText,"%d/%d",(LISTBOX_GetSel(hItem)+1),AlarmRecordList.Count);
        TEXT_SetText(hText3,pText);
		TEXT_SetText(hText0,Sys_Lib_GetDispStr(DP_ACTIVE));
        TEXT_SetText(hText1,Sys_Lib_GetDispStr(DP_TIME));
        TEXT_SetText(hText2,Sys_Lib_GetDispStr(DP_MESSAGE));
        
    case WM_NOTIFY_PARENT:
        NCode = pMsg->Data.v;
        Id = WM_GetId(pMsg->hWinSrc);
        switch(NCode)
        {
        case WM_NOTIFICATION_CLICKED:
            switch(Id)
			{
			case GUI_ID_LISTBOX0:
				{
					hWin = WM_GetDialogItem(hDlg,GUI_ID_VSCROLL);
					sprintf(pText,"%d/%d",(SCROLLBAR_GetValue(hWin)*RECORD_EVERY_PAGE+ (LISTBOX_GetSel(pMsg->hWinSrc)+1)),AlarmRecordList.Count);
					hText3 = WM_GetDialogItem(hDlg, GUI_ID_TEXT3);
					TEXT_SetText(hText3,pText);
				}
				break;
			case GUI_ID_VSCROLL:

                num = SCROLLBAR_GetValue(pMsg->hWinSrc);
                hItem = WM_GetDialogItem(hDlg, GUI_ID_LISTBOX0);
				if (num == AlarmRecordList.Count/RECORD_EVERY_PAGE - ((AlarmRecordList.Count%RECORD_EVERY_PAGE)?0:1))//最后一页
				{
					cnt = AlarmRecordList.Count - num*RECORD_EVERY_PAGE;
				}
				else
				{
					cnt = RECORD_EVERY_PAGE;
				}
				LISTBOX_SetSel(hItem,LISTBOX_GetNumItems(hItem)-1);
				_Creat_AlarmLog(hItem, num*RECORD_EVERY_PAGE+1,cnt);
				LISTBOX_SetSel(hItem,0);
				WM_SetFocus(hItem);

				sprintf(pText,"%d/%d",(num*RECORD_EVERY_PAGE+1),AlarmRecordList.Count);                    
				hText3 = WM_GetDialogItem(hDlg, GUI_ID_TEXT3);
				TEXT_SetText(hText3,pText);
				break;
            default:
                break;
            }
            break;
        case GUI_KEY_LEFT:
            switch(Id)
            {
            case GUI_ID_LISTBOX0:
				hItem = pMsg->hWinSrc;
				hWin = WM_GetDialogItem(hDlg, GUI_ID_VSCROLL);
				if (LISTBOX_GetSel(hItem) > 0)
				{
					LISTBOX_DecSel(hItem);
				}
				else if (LISTBOX_GetSel(hItem) == 0)//翻页
				{
					num = SCROLLBAR_GetValue(hWin);
					if (num > 0)
					{
						num--;
						cnt = RECORD_EVERY_PAGE;
						LISTBOX_SetSel(hItem,0);
						_Creat_AlarmLog(hItem, num*RECORD_EVERY_PAGE+1,cnt);
						LISTBOX_SetSel(hItem,RECORD_EVERY_PAGE);
						SCROLLBAR_SetValue(hWin,num);
					}
					else
					{
						LISTBOX_SetSel(hItem,0);
					}

				}
				sprintf(pText,"%d/%d",(SCROLLBAR_GetValue(hWin)*RECORD_EVERY_PAGE 
					+ (LISTBOX_GetSel(pMsg->hWinSrc)+1)),AlarmRecordList.Count);
				hText3 = WM_GetDialogItem(hDlg, GUI_ID_TEXT3);
				TEXT_SetText(hText3,pText);
                break;
            }
            break;
        case GUI_KEY_RIGHT:
            switch(Id)
            {
            case GUI_ID_LISTBOX0:
				hItem = pMsg->hWinSrc;
				hWin = WM_GetDialogItem(hDlg, GUI_ID_VSCROLL);
				if (LISTBOX_GetSel(hItem) < RECORD_EVERY_PAGE - 1)
				{
					LISTBOX_IncSel(pMsg->hWinSrc);
				}
				else if (LISTBOX_GetSel(hItem) == RECORD_EVERY_PAGE - 1)
				{
					num = SCROLLBAR_GetValue(hWin);
					if (num < AlarmRecordList.Count/RECORD_EVERY_PAGE - ((AlarmRecordList.Count%RECORD_EVERY_PAGE)?0:1))
					{
						num++;
						if (num == AlarmRecordList.Count/RECORD_EVERY_PAGE - ((AlarmRecordList.Count%RECORD_EVERY_PAGE)?0:1))
						{
							cnt = AlarmRecordList.Count - num*RECORD_EVERY_PAGE;
						}
						else
						{
							cnt = RECORD_EVERY_PAGE;
						}
						LISTBOX_SetSel(hItem,0);
						_Creat_AlarmLog(hItem, num*RECORD_EVERY_PAGE+1,cnt);
						LISTBOX_SetSel(hItem,0);
						SCROLLBAR_SetValue(hWin,num);
					}
				}
				sprintf(pText,"%d/%d",(SCROLLBAR_GetValue(hWin)*RECORD_EVERY_PAGE 
					+ (LISTBOX_GetSel(pMsg->hWinSrc)+1)),AlarmRecordList.Count);
				hText3 = WM_GetDialogItem(hDlg, GUI_ID_TEXT3);
				TEXT_SetText(hText3,pText);
                break;
            }
            break;
        }
        break;
    default:
        WM_DefaultProc(pMsg);
    }
}

static void _cbDialog8(WM_MESSAGE * pMsg)
{
	WM_HWIN hDlg,hItem;
	WM_HWIN hText0,hText1,hText2,hText3,hText4,hText5,hText6,hText7,hText8,hText9,hText10,
			hText11,hText12,hText13,hText14,hText15,hText16,hText17,hText18,hText19,hText20,
			hText28,hText29,hText30,hText31;
	WM_HWIN hTextImage0,hTextImage1,hTextImage2,hTextImage3,hTextImage4,hTextImage5,hTextImage6,hTextImage7,hTextImage8,hTextImage9;
	char  String[80];
	hDlg = pMsg->hWin;
	switch (pMsg->MsgId)
	{
	case WM_INIT_DIALOG:
		hText0 = WM_GetDialogItem(hDlg, GUI_ID_TEXT0);
		hText1 = WM_GetDialogItem(hDlg, GUI_ID_TEXT1);
		hText2 = WM_GetDialogItem(hDlg, GUI_ID_TEXT2);
		hText3 = WM_GetDialogItem(hDlg, GUI_ID_TEXT3);
		hText4 = WM_GetDialogItem(hDlg, GUI_ID_TEXT4);
		hText5 = WM_GetDialogItem(hDlg, GUI_ID_TEXT5);
		hText6 = WM_GetDialogItem(hDlg, GUI_ID_TEXT6);
		hText7 = WM_GetDialogItem(hDlg, GUI_ID_TEXT7);
		hText8 = WM_GetDialogItem(hDlg, GUI_ID_TEXT8);
		hText28 = WM_GetDialogItem(hDlg, GUI_ID_TEXT28);
		hText29 = WM_GetDialogItem(hDlg, GUI_ID_TEXT29);
		hText30 = WM_GetDialogItem(hDlg, GUI_ID_TEXT30);
		hText31 = WM_GetDialogItem(hDlg, GUI_ID_TEXT31);

		hText9 = WM_GetDialogItem(hDlg, GUI_ID_TEXT9); //bp
		hText10 = WM_GetDialogItem(hDlg, GUI_ID_TEXT10);
		hText11 = WM_GetDialogItem(hDlg, GUI_ID_TEXT11);
		hText12 = WM_GetDialogItem(hDlg, GUI_ID_TEXT12);
		hText13 = WM_GetDialogItem(hDlg, GUI_ID_TEXT13);
		hText14 = WM_GetDialogItem(hDlg, GUI_ID_TEXT14);

		hText15 = WM_GetDialogItem(hDlg, GUI_ID_TEXT15);
		hText16 = WM_GetDialogItem(hDlg, GUI_ID_TEXT16);
		hText17 = WM_GetDialogItem(hDlg, GUI_ID_TEXT17);
		hText18 = WM_GetDialogItem(hDlg, GUI_ID_TEXT18);
		hText19 = WM_GetDialogItem(hDlg, GUI_ID_TEXT19);

		hText20 = WM_GetDialogItem(hDlg, GUI_ID_TEXT20);//选配

		hTextImage0 = WM_GetDialogItem(hDlg, GUI_ID_IMAGETEXT0);//选配标志
		hTextImage1 = WM_GetDialogItem(hDlg, GUI_ID_IMAGETEXT1);
		hTextImage2 = WM_GetDialogItem(hDlg, GUI_ID_IMAGETEXT2);
		hTextImage3 = WM_GetDialogItem(hDlg, GUI_ID_IMAGETEXT3);
		hTextImage4 = WM_GetDialogItem(hDlg, GUI_ID_IMAGETEXT4);
		hTextImage5 = WM_GetDialogItem(hDlg, GUI_ID_IMAGETEXT5);
		hTextImage6 = WM_GetDialogItem(hDlg, GUI_ID_IMAGETEXT6);
		hTextImage7 = WM_GetDialogItem(hDlg, GUI_ID_IMAGETEXT7);
		hTextImage8 = WM_GetDialogItem(hDlg, GUI_ID_IMAGETEXT8);
		hTextImage9 = WM_GetDialogItem(hDlg, GUI_ID_IMAGETEXT9);
/***************************ylk modify 修改系统信息界面显示(拉伸)***************************/
		//选配
		TEXT_SetText(hText20,Sys_Lib_GetDispStr(DP_OPTION));
		IMAGETEXT_SetText(hTextImage0,Sys_Lib_GetDispStr(DP_MODEPRVC));
		IMAGETEXT_SetTextAlign(hTextImage0,GUI_TA_LEFT|GUI_TA_TOP);
		IMAGETEXT_SetBitmapTextFlag(hTextImage0,2);
		IMAGETEXT_SetText(hTextImage1,Sys_Lib_GetDispStr(DP_MODEINFANT));
		IMAGETEXT_SetTextAlign(hTextImage1,GUI_TA_LEFT|GUI_TA_TOP);
		IMAGETEXT_SetBitmapTextFlag(hTextImage1,2);
		IMAGETEXT_SetText(hTextImage2,Sys_Lib_GetDispStr(DP_MODULECO2));
		IMAGETEXT_SetTextAlign(hTextImage2,GUI_TA_LEFT|GUI_TA_TOP);
		IMAGETEXT_SetBitmapTextFlag(hTextImage2,2);
		IMAGETEXT_SetText(hTextImage3,Sys_Lib_GetDispStr(DP_MODULEO2));
		IMAGETEXT_SetTextAlign(hTextImage3,GUI_TA_LEFT|GUI_TA_TOP);
		IMAGETEXT_SetBitmapTextFlag(hTextImage3,2);
// 		IMAGETEXT_SetText(hTextImage4,Sys_Lib_GetDispStr(DP_MODEPRVC));
		IMAGETEXT_SetTextAlign(hTextImage4,GUI_TA_LEFT|GUI_TA_TOP);
		IMAGETEXT_SetBitmapTextFlag(hTextImage4,2);
// 		IMAGETEXT_SetText(hTextImage5,Sys_Lib_GetDispStr(DP_MODEPRVC));
		IMAGETEXT_SetTextAlign(hTextImage5,GUI_TA_LEFT|GUI_TA_TOP);
// 		IMAGETEXT_SetBkColor(hTextImage5,GUI_RED);
		IMAGETEXT_SetBitmapTextFlag(hTextImage5,2);
// 		IMAGETEXT_SetText(hTextImage6,Sys_Lib_GetDispStr(DP_MODEPRVC));
		IMAGETEXT_SetTextAlign(hTextImage6,GUI_TA_LEFT|GUI_TA_TOP);
		IMAGETEXT_SetBitmapTextFlag(hTextImage6,2);
		IMAGETEXT_SetText(hTextImage7,"VS Mode");
		IMAGETEXT_SetTextAlign(hTextImage7,GUI_TA_LEFT|GUI_TA_TOP);
		IMAGETEXT_SetBitmapTextFlag(hTextImage7,2);
		IMAGETEXT_SetText(hTextImage8,"APRV Mode");
		IMAGETEXT_SetTextAlign(hTextImage8,GUI_TA_LEFT|GUI_TA_TOP);
		IMAGETEXT_SetBitmapTextFlag(hTextImage8,2);
		IMAGETEXT_SetText(hTextImage9,Sys_Lib_GetDispStr(DP_LUNGRECRUITMENT));
		IMAGETEXT_SetTextAlign(hTextImage9,GUI_TA_LEFT|GUI_TA_TOP);
		IMAGETEXT_SetBitmapTextFlag(hTextImage9,2);

		//PRVC SIMV-PRVC
		if(PRVCMode_Chose_Flag)
		{
			IMAGETEXT_SetBitmapEx(hTextImage0,&bmOptionFlag,0,2);
			IMAGETEXT_SetTextColor(hTextImage0,GUI_BLACK);
		}
		else
		{
			IMAGETEXT_DeleteBitmap(hTextImage0);
			IMAGETEXT_SetTextColor(hTextImage0,GUI_GRAY);
		}
		//婴儿模式
 		if(VT_Min_Flag)
 		{
			IMAGETEXT_SetBitmapEx(hTextImage1,&bmOptionFlag,0,2);
			IMAGETEXT_SetTextColor(hTextImage1,GUI_BLACK);
 		}
 		else
 		{
			IMAGETEXT_DeleteBitmap(hTextImage1);
			IMAGETEXT_SetTextColor(hTextImage1,GUI_GRAY);
 		}
		//CO2
		if((EtCO2_Config_On) || (EtCO2_KM7002_Config_On) || (Masimo_EtCO2_Config_On)\
			|| (MasimoISA_EtCO2_Config_On) || (MasimoISA_EtCO2_Config_On) || (Drager_EtCO2_Config_On))
		{
			IMAGETEXT_SetBitmapEx(hTextImage2,&bmOptionFlag,0,2);
			IMAGETEXT_SetTextColor(hTextImage2,GUI_BLACK);
		}
		else
		{
			IMAGETEXT_DeleteBitmap(hTextImage2);
			IMAGETEXT_SetTextColor(hTextImage2,GUI_GRAY);
		}
		if((EtCO2_Config_On || EtCO2_KM7002_Config_On) && Masimo_EtCO2_Config_On)
		{
			IMAGETEXT_SetText(hTextImage2,Sys_Lib_GetDispStr(DP_MODULEAACO2));
		}
		else if(EtCO2_Config_On || EtCO2_KM7002_Config_On)
		{
			IMAGETEXT_SetText(hTextImage2,Sys_Lib_GetDispStr(DP_MODULECO2));
		}
		else if(Masimo_EtCO2_Config_On)//masimo主流
		{
			IMAGETEXT_SetText(hTextImage2,Sys_Lib_GetDispStr(DP_MASIMOIRMA));
		}
		else if(MasimoISA_EtCO2_Config_On  == 1)//masimo旁流
		{
			IMAGETEXT_SetText(hTextImage2,Sys_Lib_GetDispStr(DP_MASIMOISA));
		}
		else if(MasimoISA_EtCO2_Config_On  == 2)//masimo旁流+O2
		{
			IMAGETEXT_SetText(hTextImage2,Sys_Lib_GetDispStr(DP_MASIMOISAO2));
		}
		else if(Drager_EtCO2_Config_On == 1)
		{
			IMAGETEXT_SetText(hTextImage2,Sys_Lib_GetDispStr(DP_MODULEDRAGER));//Drager
		}
		else if(Drager_EtCO2_Config_On == 2)
		{
			IMAGETEXT_SetText(hTextImage2,Sys_Lib_GetDispStr(DP_MODULEDRAGERO2));//Drager + O2
		}
		//Fio2
		if(FiO2_Config_On)
		{
			IMAGETEXT_SetBitmapEx(hTextImage3,&bmOptionFlag,0,2);
			IMAGETEXT_SetTextColor(hTextImage3,GUI_BLACK);
		}
		else
		{
			IMAGETEXT_DeleteBitmap(hTextImage3);
			IMAGETEXT_SetTextColor(hTextImage3,GUI_GRAY);
		}

#ifndef LANGUAGE_CHINESE_FLAG
		//SIMV-P PSV
		IMAGETEXT_SetText(hTextImage4,Sys_Lib_GetDispStr(DP_MODESIMVPCVPSV));
		if(Mode_Chose_Flag)
		{
			IMAGETEXT_SetBitmapEx(hTextImage4,&bmOptionFlag,0,2);
			IMAGETEXT_SetTextColor(hTextImage4,GUI_BLACK);
		}
		else
		{
			IMAGETEXT_DeleteBitmap(hTextImage4);
			IMAGETEXT_SetTextColor(hTextImage4,GUI_GRAY);
		}
		//Driver Gas
		IMAGETEXT_SetText(hTextImage5,Sys_Lib_GetDispStr(DP_MODEGASDRIVER));
		if(DriverGas_Chose_Flag)
		{
			IMAGETEXT_SetBitmapEx(hTextImage5,&bmOptionFlag,0,2);
			IMAGETEXT_SetTextColor(hTextImage5,GUI_BLACK);
		}
		else
		{
			IMAGETEXT_DeleteBitmap(hTextImage5);
			IMAGETEXT_SetTextColor(hTextImage5,GUI_GRAY);
		}
		//SpO2
#if(LOGO_FLAG != MSW_LOGO)
// 		TEXT_SetTextWrapMode(hTextImage6, GUI_WRAPMODE_WORD);
		if(SpO2_Config_On)
		{
			IMAGETEXT_SetText(hTextImage6,Sys_Lib_GetDispStr(DP_MODELESPO2));
		}
		if(Masimo_SpO2_Config_On)
		{
			IMAGETEXT_SetText(hTextImage6,Sys_Lib_GetDispStr(DP_MASIMOSPO2MODULE));
		}
		if((SpO2_Config_On)||(Masimo_SpO2_Config_On))
		{
			IMAGETEXT_SetBitmapEx(hTextImage6,&bmOptionFlag,0,2);
			IMAGETEXT_SetTextColor(hTextImage6,GUI_BLACK);
		}
		else
		{
			IMAGETEXT_DeleteBitmap(hTextImage6);
			IMAGETEXT_SetTextColor(hTextImage6,GUI_GRAY);
		}
#endif
#endif
		if (VS_Mode_Flag)
		{
			IMAGETEXT_SetBitmapEx(hTextImage7,&bmOptionFlag,0,2);
			IMAGETEXT_SetTextColor(hTextImage7,GUI_BLACK);
		}
		else
		{
			IMAGETEXT_DeleteBitmap(hTextImage7);
			IMAGETEXT_SetTextColor(hTextImage7,GUI_GRAY);
		}
		if (APRV_Mode_Flag)
		{
			IMAGETEXT_SetBitmapEx(hTextImage8,&bmOptionFlag,0,2);
			IMAGETEXT_SetTextColor(hTextImage8,GUI_BLACK);
		}
		else
		{
			IMAGETEXT_DeleteBitmap(hTextImage8);
			IMAGETEXT_SetTextColor(hTextImage8,GUI_GRAY);
		}
		if (LungRec_Config_On)
		{
			IMAGETEXT_SetBitmapEx(hTextImage9,&bmOptionFlag,0,2);
			IMAGETEXT_SetTextColor(hTextImage9,GUI_BLACK);
		}
		else
		{
			IMAGETEXT_DeleteBitmap(hTextImage9);
			IMAGETEXT_SetTextColor(hTextImage9,GUI_GRAY);
		}


		//软件版本
		TEXT_SetText(hText19,Sys_Lib_GetDispStr(DP_SOFTWAREVERSION));
		TEXT_SetText(hText3,Sys_Lib_GetDispStr(DP_GUI));
		TEXT_SetText(hText5,Sys_Lib_GetDispStr(DP_BDU));
		TEXT_SetText(hText7,Sys_Lib_GetDispStr(DP_PMU));

		//运行时间
		TEXT_SetText(hText0,Sys_Lib_GetDispStr(DP_RUNTIME));
		hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT1);
		TEXT_SetTextAlign(hItem,TEXT_CF_HCENTER|TEXT_CF_VCENTER);
		strcpy(String, _Float2String((float)OperationTime.Hour,0));
		TEXT_SetText(hItem,String);
		//WM_ClrHasTrans(hItem);//显示运行时间白框
		//小时
		TEXT_SetText(hText2,Sys_Lib_GetDispStr(DP_HOURS));
		//大气压力
		TEXT_SetText(hText9,Sys_Lib_GetDispStr(DP_BAROMETRIC));//djq add 20140305
		//气源压力
		TEXT_SetText(hText12,Sys_Lib_GetDispStr(DP_GASSUPPLYPRESSURE));//djq add 20140305
		TEXT_SetText(hText15,Sys_Lib_GetDispStr(DP_O2));//djq add 20140305
		TEXT_SetText(hText16,Sys_Lib_GetDispStr(DP_AIR));//djq add 20140305
/**************************************************************************************/
		//GUI version 
		sprintf(String,"%01d\.%02d\.%02d",GUIVersion.MainVersion,GUIVersion.SubVersion,GUIVersion.DebugVersion);
		TEXT_SetText(hText4,String);		

		//BDU version
		sprintf(String,"%01d\.%02d\.%02d",BDUVersion.MainVersion,BDUVersion.SubVersion,BDUVersion.DebugVersion);
		TEXT_SetText(hText6,String);

		//PMU version
		sprintf(String,"%01d\.%02d\.%02d",PMUVersion.MainVersion,PMUVersion.SubVersion,PMUVersion.DebugVersion);
		TEXT_SetText(hText8,String);

		//PM version
		sprintf(String,"%01d\.%02d\.%02d",PM_Version_Main,PM_Version_Sub,PM_Version_Debug);
		TEXT_SetText(WM_GetDialogItem(hDlg, GUI_ID_TEXT33),String);

		//Str Version
#ifdef __ICCARM__        
		sprintf(String,"%01d.%02d.%02d",  
			*((unsigned char *)STR_BASE_ADDR + 0),
			*((unsigned char *)STR_BASE_ADDR + 1),
			*((unsigned char *)STR_BASE_ADDR + 2));
#else
		sprintf(String,"%01d.%02d.%02d",  1 , 0, 1);
#endif
		TEXT_SetText(hText29,String);

		//Font Version
#ifdef __ICCARM__         
		sprintf(String,"%01d.%02d.%02d",
			*((unsigned char *)FONT_SIF_BASE_ADDR + 0),
			*((unsigned char *)FONT_SIF_BASE_ADDR + 1),
			*((unsigned char *)FONT_SIF_BASE_ADDR + 2));
#else
		sprintf(String,"%01d.%02d.%02d",  1 , 0, 0);
#endif   
		TEXT_SetText(hText31,String);

		if((Barameter_Pressure >= 0) && (Barameter_Pressure <= 999))   //djq add 20140305  设置大气压力值
		{
			sprintf(String,"%3d",Barameter_Pressure);
			TEXT_SetText(hText10,String);     
		}
		if(RealtimePara.O2GasPressure >= 20) //djq add 20140305
		{
			sprintf(String,"%3d",RealtimePara.O2GasPressure);  //djq add 20140305 显示O2气源压力
			TEXT_SetText(hText13,String);     
		}
		if(RealtimePara.AirGasPressure >= 20) //djq add 20170407
		{
			sprintf(String,"%3d",RealtimePara.AirGasPressure);  
			TEXT_SetText(hText17,String);     
		}
		if(DriverGas_Chose_Flag)
		{
			WM_ShowWindow(hText16);
			WM_ShowWindow(hText17);
			WM_ShowWindow(hText18);
			//WM_MoveChildTo(hText0,40,160);
			//WM_MoveChildTo(hText1,250,158);
			//WM_MoveChildTo(hText2,300,160);
		}
		else
		{
			WM_HideWindow(hText16);
			WM_HideWindow(hText17);
			WM_HideWindow(hText18);
			//WM_MoveChildTo(hText0,40,135);
			//WM_MoveChildTo(hText1,250,133);
			//WM_MoveChildTo(hText2,300,135);
		}

		break;
	case WM_NOTIFY_PARENT_REFLECTION:
		if(  (!(MenuOperateStatus&CALIBRATION_PRECEED)) 
			&& (!(MenuOperateStatus&FRAMEWIN_POP)) )
		{//TODO 添加此限制条件没必要？
			FunctionKeyPressedResume(Menu_hWin.hWin,0);
			if(StandbyState==0xFF)
			{
				PlanMenuType = STANDBY_MENU;
			}
			else
			{
				PlanMenuType = MAIN_MENU;
			}
			Task |= MEAU_UPDATE;
		}
		else
		{//TODO 永远不会执行以下代码？
			hItem = WM_GetDialogItem(Menu_hWin.ActivedWin, GUI_ID_CLOSE);
			BUTTON_SetPressed(hItem,0);
		}
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}

/***************************************************************************************************
* @brief  Waveform Setting 波形监测界面
* @para   
* @retval 
*/
static unsigned char s_last_focus_curve3 = 0;

static void _cbDialog9(WM_MESSAGE * pMsg)
{
	WM_HWIN hDlg,hItem;
	int Id,NCode;
	char pText[10] = {0};
	int sec = 0, min = 0, hour = 0;
	hDlg = pMsg->hWin;
	switch (pMsg->MsgId)
	{
	case WM_INIT_DIALOG:
		WINDOW_SetBkColor(hDlg,0x1E1E1E/*GUI_BLACK*/);//0x1E1E1E
        
		//WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON0));
		//hItem = WM_GetDialogItem(hDlg, GUI_ID_BUTTON0);
		//BUTTON_SetBitmapEx(hItem, 0, &bmcurvechoose_rgb, 0, 0);
		//BUTTON_SetBitmapEx(hItem, 1, &bmcurvechoose_rgb, 0, 0);
		//WM_BringToTop(hItem);
		
		hItem = WM_GetDialogItem(hDlg, GUI_ID_BUTTON1);
		BUTTON_SetBitmapEx(hItem, 0, &bmcurvechoose_rgb, 0, 0);
		BUTTON_SetBitmapEx(hItem, 1, &bmcurvechoose_rgb, 0, 0);
		WM_BringToTop(hItem);

		hItem = WM_GetDialogItem(hDlg, GUI_ID_BUTTON2);
		BUTTON_SetBitmapEx(hItem, 0, &bmcurvechoose_rgb, 0, 0);
		BUTTON_SetBitmapEx(hItem, 1, &bmcurvechoose_rgb, 0, 0);
		WM_BringToTop(hItem);

		hItem = WM_GetDialogItem(hDlg, GUI_ID_BUTTON3);
		BUTTON_SetBitmapEx(hItem, 0, &bmcurvechoose_rgb, 0, 0);
		BUTTON_SetBitmapEx(hItem, 1, &bmcurvechoose_rgb, 0, 0);
		WM_BringToTop(hItem);
		WM_HideWindow(hItem);

		WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_CURVE3));

		RectPaintParaInit(hDlg);
		CurveMenuInit(&Menu_hWin.cWin,hDlg);
		break;
	case WM_NOTIFY_PARENT:
		NCode = pMsg->Data.v;
		Id = WM_GetId(pMsg->hWinSrc);
		switch(NCode)
		{
		case WM_NOTIFICATION_CLICKED:
			switch(Id)
			{				
			case GUI_ID_BUTTON1:
			case GUI_ID_BUTTON2:
			case GUI_ID_BUTTON3:
//				BUTTON_SetPressed(pMsg->hWinSrc, 1/*BUTTON_STATE_FOCUS*/);								
   				BUTTON_SetState(pMsg->hWinSrc, BUTTON_STATE_FOCUS);								

//				if(MenuType != WAVEFORM_SETTING_MENU) 
				{
					FunctionKeyPressedResume(Menu_hWin.hWin, 0);
                    
					PlanMenuType = WAVEFORM_SETTING_MENU;
					Task |= MEAU_UPDATE;

					if(Id == GUI_ID_BUTTON1) {
						Menu_hWin.cWin.FocusCurve = 1;
						s_last_focus_curve3 = Menu_hWin.cWin.FocusCurve;
					}
					else if(Id == GUI_ID_BUTTON2) {
						Menu_hWin.cWin.FocusCurve = 2;
						s_last_focus_curve3 = Menu_hWin.cWin.FocusCurve;
					}	
					else {
						Menu_hWin.cWin.FocusCurve = 3;
						s_last_focus_curve3 = Menu_hWin.cWin.LoopFlag1;
					}					
				}

#if 0
				else {
					hItem = WM_GetDialogItem(Menu_hWin.ActivedWin, GUI_ID_MENU0);
					if(Id == GUI_ID_BUTTON1) {
						Menu_hWin.cWin.FocusCurve = 1;
						s_last_focus_curve3 = Menu_hWin.cWin.FocusCurve;
						WM_MoveTo(Menu_hWin.ActivedWin, 475, 255);
						MENU_SetSel(hItem, Menu_hWin.cWin.WaveForms[1]);
					}
					else if(Id == GUI_ID_BUTTON2) {
						Menu_hWin.cWin.FocusCurve = 2;
						s_last_focus_curve3 = Menu_hWin.cWin.FocusCurve;
						WM_MoveTo(Menu_hWin.ActivedWin, 475, 410);
						MENU_SetSel(hItem, Menu_hWin.cWin.WaveForms[2]);
					}	
					else {
						Menu_hWin.cWin.FocusCurve = 3;
						WM_MoveTo(Menu_hWin.ActivedWin, 405, 310);
						MENU_SetSel(hItem, Menu_hWin.cWin.WaveForms[3]);
					}	
				}
#endif                
                
				WM_SetFocus(Menu_hWin.ActivedWin);
				//
				TracingCurve_SetXScale(Menu_hWin.HomeWin.Tobj[0],1);
				TracingCurve_SetXScale(Menu_hWin.HomeWin.Tobj[1],1);
				TracingCurve_SetXScale(Menu_hWin.HomeWin.Tobj[2],1);
				TracingCurve_SetXScale(Menu_hWin.HomeWin.Tobj[3],1);
			break;

//			case GUI_ID_TEXT13:
//				if(GetTimerClearFlag())//Confirm
//				{
//					SetTimerStop(Menu_hWin.HomeWin.Left_Robj[5]);
//					MultiText_ParaUpdate(Menu_hWin.HomeWin.Left_Robj[5], "00.00.00");
//					SetTimerSec(0);
//					SetTimerMin(0);
//					SetTimerHour(0);	
//
//					MultiText_NameUpdate(Menu_hWin.HomeWin.Left_Robj[6], Sys_Lib_GetDispStr(DP_START));
//					MultiText_NameUpdate(Menu_hWin.HomeWin.Left_Robj[7], Sys_Lib_GetDispStr(DP_CLEAR));
//					MultiText_EX_SetBitmapEx(Menu_hWin.HomeWin.Left_Robj[5], &bmTimer, 10, 15);
//					MultiText_SetTextRect(Menu_hWin.HomeWin.Left_Robj[5], 50, 120, 19, 30, MultiText_Data);
//
//					SetTimerStartFlag(1);//Display Start or Stop
//					SetTimerClearFlag(0);
//					SetBitmapFlag(0);				
//				}
//				else//Start/Stop
//				{
//					if(GetTimerStartFlag())//Start
//					{
//						SetTimerStart(1000);//1s
//
//						MultiText_NameUpdate(Menu_hWin.HomeWin.Left_Robj[6], Sys_Lib_GetDispStr(DP_STOP));
//						MultiText_EX_SetBitmapEx(Menu_hWin.HomeWin.Left_Robj[5], &bmIconStop, 10, 15);
//
//						SetTimerStartFlag(0);
//						SetBitmapFlag(1);
//						SetRefreshFlag(1);//不点暂停，点Clear，继续计时
//					}
//					else//Stop
//					{
//						SetTimerStop();
//						sec = GetTimerSec();
//						min = GetTimerMin();
//						hour = GetTimerHour();
//						sprintf(pText, "%02d.%02d.%02d", hour,min,sec);
//						MultiText_ParaUpdate(Menu_hWin.HomeWin.Left_Robj[5], pText);
//
//						MultiText_NameUpdate(Menu_hWin.HomeWin.Left_Robj[6], Sys_Lib_GetDispStr(DP_START));
//						MultiText_EX_SetBitmapEx(Menu_hWin.HomeWin.Left_Robj[5], &bmIconStart, 10, 15);
//						SetTimerStartFlag(1);
//					}
//				}
//				break;
//			case GUI_ID_TEXT14:
//				if(GetTimerClearFlag() == 0)//Clear
//				{	
//					MultiText_NameUpdate(Menu_hWin.HomeWin.Left_Robj[6], Sys_Lib_GetDispStr(DP_CONFIRM));//
//					MultiText_NameUpdate(Menu_hWin.HomeWin.Left_Robj[7], Sys_Lib_GetDispStr(DP_CANCEL));
//					MultiText_EX_SetBitmapEx(Menu_hWin.HomeWin.Left_Robj[5], 0, 10, 15);
//					MultiText_SetTextRect(Menu_hWin.HomeWin.Left_Robj[5], 33, 120, 19, 30, MultiText_Data);
//					MultiText_ParaUpdate(Menu_hWin.HomeWin.Left_Robj[5], "Retime?");
//
//					SetTimerClearFlag(1);
//					SetRefreshFlag(0);
//				}
//				else//Cancel
//				{
//					sec = GetTimerSec();
//					min = GetTimerMin();
//					hour = GetTimerHour();	
//					sprintf(pText, "%02d.%02d.%02d", hour,min,sec);
//					MultiText_ParaUpdate(Menu_hWin.HomeWin.Left_Robj[5], pText);
//
//					MultiText_SetTextRect(Menu_hWin.HomeWin.Left_Robj[5], 50, 120, 19, 30, MultiText_Data);
//					MultiText_NameUpdate(Menu_hWin.HomeWin.Left_Robj[7], Sys_Lib_GetDispStr(DP_CLEAR));
//
//					if(GetTimerHandle())
//					{
//						MultiText_NameUpdate(Menu_hWin.HomeWin.Left_Robj[6], Sys_Lib_GetDispStr(DP_STOP));
//						MultiText_EX_SetBitmapEx(Menu_hWin.HomeWin.Left_Robj[5], &bmIconStop, 10, 15);
//					}
//					else
//					{
//						MultiText_NameUpdate(Menu_hWin.HomeWin.Left_Robj[6], Sys_Lib_GetDispStr(DP_START));
//						MultiText_EX_SetBitmapEx(Menu_hWin.HomeWin.Left_Robj[5], &bmIconStart, 10, 15);
//					}
//
//					SetTimerClearFlag(0);
//					SetRefreshFlag(1);
//				}			
//				break;
			}
		}
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}


/***************************************************************************************************
* @brief  Password input
* @para   
* @retval 
*/
static void _cbDialog12(WM_MESSAGE * pMsg)
{
    WM_HWIN hDlg,hItem;
    WM_HWIN hButton10,hButton11,hText0;
	static int title_barometer_flag = 0;//1开始提示大气压力语句,2开始提示校验地设置值得大气压力值语句，0结束提示语句
    static int Flow_SensorFlag = 0;//0正常模式  1流量传感器模式
    static char Flow_SeriesNum[10] = "";
    int Id,NCode,tmp;
    char src[2] = {0};
    char dest[10]= {0};
	char dest_firstpart[4] = {0};
	char title[70];//zkq modify 增大数组防止死机 
    hDlg = pMsg->hWin;
    switch (pMsg->MsgId)
    {
    case WM_INIT_DIALOG:
        WINDOW_SetUse_3D(hDlg,1);
        hItem = WM_GetDialogItem(hDlg, GUI_ID_EDIT0);
        MULTIEDIT_SetBkColor(hItem,MULTIEDIT_CI_EDIT,GUI_WHITE);//必须是MULTIEDIT_CI_EDIT 下颜色才会改变?   20130401
        MULTIEDIT_SetFont(hItem,&GUI_Font_24);
        MULTIEDIT_SetPasswordMode(hItem,1);
        hButton10 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON10);
        hButton11 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON11);
        hText0 = WM_GetDialogItem(hDlg, GUI_ID_TEXT0);
        
        BUTTON_SetText(hButton10,Sys_Lib_GetDispStr(DP_ENTER));
        BUTTON_SetText(hButton11,Sys_Lib_GetDispStr(DP_EXIT));
        TEXT_SetText(hText0,Sys_Lib_GetDispStr(DP_INPUTPASSWORD));
        TEXT_SetTextWrapMode(hText0,GUI_WRAPMODE_WORD);    //  "输入密码:"
        
        break;
    case WM_NOTIFY_PARENT:
        NCode = pMsg->Data.v;
        Id = WM_GetId(pMsg->hWinSrc);
        switch(NCode)
        {
        case WM_NOTIFICATION_CLICKED:
            FunctionKeyPressedResume(hDlg,Id);
            switch(Id)
            {
            case GUI_ID_BUTTON10:
#ifndef MACHINE_TYPE_FLAG
                if(!Flow_SensorFlag)//密码模式
#endif
                {
                    hItem = WM_GetDialogItem(hDlg, GUI_ID_EDIT0);
                    MULTIEDIT_GetText(hItem,dest,7);
                    dest_firstpart[0] = dest[0];
                    dest_firstpart[1] = dest[1];
                    dest_firstpart[2] = dest[2];
                    //MULTIEDIT_SetText(hItem,"");
                    if(!strcmp(dest,"853210"))
                    {
                        PlanMenuType = ENGINEER_CALIBRATION_MENU;
                        Task |=MEAU_UPDATE;
                    }
                    //*//  20130328
                    else if(!strcmp(dest,"853219"))  //sun add 20150209
                    {
                        PlanMenuType = SOFTWART_OPTIONAL_MENU;
                        Task |=MEAU_UPDATE;
                    }
                    else if(!strcmp(dest_firstpart,"861"))
                    {
                        if(MULTIEDIT_GetTextSize(hItem) > 6)
                        {
                            tmp = (dest[3] - 48)* 100 + (dest[4] - 48)* 10 + (dest[5] - 48);
                        }
                        else  if(MULTIEDIT_GetTextSize(hItem) == 6)
                        {
                            tmp = (dest[3] - 48)* 10 + (dest[4] - 48);
                        }
                        else  if(MULTIEDIT_GetTextSize(hItem) == 5)
                        {
                            tmp = (dest[3] - 48);
                        }
                        else 
                        {
                            tmp = -1;
                        }
                        OperationTime.Hour = tmp;
                        OperationTime.Min  = 0;
                        SaveOperationTimeFlag = TRUE;

                        EmptyAlarmRecordList(&AlarmRecordList);//sun add 20140701
                        hItem = WM_GetDialogItem(Menu_hWin.ActivedWin, GUI_ID_MULTIPAGE0);
                        MULTIPAGE_SetPageChangeEnable(hItem,1);
                        MenuOperateStatus &= ~FRAMEWIN_POP;
                        PlanMenuType = SYSTEM_SETTING_MENU;
                        Task |=MEAU_UPDATE;
                    }//*/
#ifdef DEMO  //  [11/8/2022 lyy add]
					else if (!strcmp(dest, "123456"))//Demo模式开启关闭
					{
						Demo_Switch = !Demo_Switch;
						DemoParaInit();
						hItem = WM_GetDialogItem(Menu_hWin.ActivedWin, GUI_ID_MULTIPAGE0);
						MULTIPAGE_SetPageChangeEnable(hItem,1);
						MenuOperateStatus &= ~FRAMEWIN_POP;
						PlanMenuType = SYSTEM_SETTING_MENU;
						Task |=MEAU_UPDATE;
					}
#endif
#if TESTDEMO
					else if(!strcmp(dest,"654321"))
					{
						Demo_Alarm = !Demo_Alarm;
						if(Demo_Alarm)
						{
							DEMO_AlarmClear();
						}
						else
						{
							AlarmsWork.presentAlarms = 0;
							AlarmsWork.newAlarms = 0;
							AlarmsWork.presentAlarms1 = 0;
							AlarmsWork.newAlarms1 = 0;
							EmptyAlarmRecordList(&AlarmRecordList);
						}

						Menu_hWin.InfoWin.InfoChangeFlag |= ALARM_INFO_CHANGED_FLAG;
						hItem = WM_GetDialogItem(Menu_hWin.ActivedWin, GUI_ID_MULTIPAGE0);
						MULTIPAGE_SetPageChangeEnable(hItem,1);
						MenuOperateStatus &= ~FRAMEWIN_POP;
						PlanMenuType = SYSTEM_SETTING_MENU;
						Task |=MEAU_UPDATE;
						break;
					}
#endif

#ifndef MACHINE_TYPE_FLAG
                    else if(!strcmp(dest,"852002"))//开启密码输入
                    {
                        Flow_SensorFlag = 1;
                        hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT1);
                        TEXT_SetText(hItem,"Input Series Num:\n");

                    }
                    else if(!strcmp(dest,"333333"))//查询密码
                    {
                        static char month = 0;
                        hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT1);
                        strcpy(dest,"");
                        for(tmp = 0;tmp < 7;tmp++)
                        {
                            Id = RTEEPROMread(FLOW_SERIESNUM_ADDR+tmp);
                            if(tmp < 2)
                            {
                               dest[tmp] = Id+0x30;
                            }
                            else if(tmp > 3)
                            {
                                dest[tmp-1] = Id+0x30;
                            }
                            else if(tmp == 2)
                            {
                                month = Id*10;
                            }
                            else if(tmp == 3)
                            {
                                month += Id;
                                if(month > 12)
                                {
                                    month = 12;
                                }
                                dest[2] = 'A'+month - 1;
                            }
                        }
                        dest[7] = '\0';
                        TEXT_SetText(hItem,dest);
                    }
#endif
                    else if(!strcmp(dest,"987654"))  //sun add 20140514
                    {
                        PlanMenuType = UPDATE_PROGRAM_MODE;
                        Task |=MEAU_UPDATE;
                    }
                    else if(!strcmp(dest_firstpart,"851"))  //djq add 20140305  关闭大气压力传感器，并且发送压力值到BDU 
                    {
                        title_barometer_flag = 0;  //关闭提示语句
                        if(MULTIEDIT_GetTextSize(hItem) > 6)
                        {
                           tmp = (dest[3] - 48)* 100 + (dest[4] - 48)* 10 + (dest[5] - 48);
                        }
                        else  if(MULTIEDIT_GetTextSize(hItem) == 6)
                        {
                           tmp = (dest[3] - 48)* 10 + (dest[4] - 48);
                        }
                        else  if(MULTIEDIT_GetTextSize(hItem) == 5)
                        {
                           tmp = (dest[3] - 48);
                        }
                        else 
                        {
                           tmp = -1;
                        }
                        if((tmp >= 0) && (tmp <= 999))
                        {
                            Barameter_Pressure = tmp;
                            SendOperationMessage(BAROMETER_SEND,USART0);//数据在合理范围内则发送数据   
                            Barameter_Switch = 1;                       //大气压力传感器关
                            RTEEPROMwrite(BARO_SWITCH_ADDR,Barameter_Switch);
                            RTEEPROMwrite(BARO_DATA_HIGH_ADDR,(Barameter_Pressure &0xff00)>>8); //写入大气压力的高位
                            RTEEPROMwrite(BARO_DATA_LOW_ADDR,Barameter_Pressure & 0xff); //写入大气压力的低位                       
                        }
    
                         hItem = WM_GetDialogItem(Menu_hWin.ActivedWin, GUI_ID_MULTIPAGE0);
                         MULTIPAGE_SetPageChangeEnable(hItem,1);
                         MenuOperateStatus &= ~FRAMEWIN_POP;
                         PlanMenuType = SYSTEM_SETTING_MENU;
                         Task |=MEAU_UPDATE;
                    }
                    else if(!strcmp(dest_firstpart,"841"))  //djq add 20140305  发送校验地大气压力值
                    {
                        title_barometer_flag = 0;  //关闭提示语句
                        if(MULTIEDIT_GetTextSize(hItem) > 6)
                        {
                           tmp = (dest[3] - 48)* 100 + (dest[4] - 48)* 10 + (dest[5] - 48);
                        }
                        else  if(MULTIEDIT_GetTextSize(hItem) == 6)
                        {
                           tmp = (dest[3] - 48)* 10 + (dest[4] - 48);
                        }
                        else  if(MULTIEDIT_GetTextSize(hItem) == 5)
                        {
                           tmp = (dest[3] - 48);
                        }
                        else 
                        {
                           tmp = -1;
                        }
                        if((tmp >= 0) && (tmp <= 999))
                        {
                            Barameter_Stand_Pressure = tmp;
                            Barameter_Switch = 1;                       //大气压力传感器关
                            RTEEPROMwrite(BARO_SWITCH_ADDR,Barameter_Switch);
                            SendOperationMessage(BAROMETER_STAND_SEND,USART0);//数据在合理范围内则发送数据   
                        }
    
                         hItem = WM_GetDialogItem(Menu_hWin.ActivedWin, GUI_ID_MULTIPAGE0);
                         MULTIPAGE_SetPageChangeEnable(hItem,1);
                         MenuOperateStatus &= ~FRAMEWIN_POP;
                         PlanMenuType = SYSTEM_SETTING_MENU;
                         Task |=MEAU_UPDATE;
                    }
                    else if(!strcmp(dest,"852001"))        //开启大气压力传感器
                    {
                        Barameter_Pressure = (0xAA << 8);//开启大气压力命令
                        SendOperationMessage(BAROMETER_SEND,USART0);//数据在合理范围内则发送数据   
                        Barameter_Switch = 0;            //大气压力传感器开
                        RTEEPROMwrite(BARO_SWITCH_ADDR,Barameter_Switch);  //
                        
                        hItem = WM_GetDialogItem(Menu_hWin.ActivedWin, GUI_ID_MULTIPAGE0);
                        MULTIPAGE_SetPageChangeEnable(hItem,1);
                        MenuOperateStatus &= ~FRAMEWIN_POP;
                        PlanMenuType = SYSTEM_SETTING_MENU;
                        Task |=MEAU_UPDATE;
                    }
                    //*//  20130328
                    else if(!strcmp(dest,"111111"))
                    {                      
                        PlanMenuType = BATTERY_CALIBRATION_MENU;
                        Task |=MEAU_UPDATE;
                    }
					else if(!strcmp(dest,"321470"))
					{
						PlanMenuType = MEDIBUS_COMM_MENU;
						Task |=MEAU_UPDATE;
						break;
					}
#ifdef N2O_SWITCH_FLAG //  [7/5/2022 yaoyao add]
					else if(!strcmp(dest,"853215"))//ylk add 20181128
					{
						if(N2O_Switch)
						{
							N2O_Switch = 0;//

						}
						else 
						{
							N2O_Switch = 1;//
						}
						RTEEPROMwrite(N2O_ON_OFF_ADDR, N2O_Switch);
						PlanMenuType = SYSTEM_SETTING_MENU;
						Task |=MEAU_UPDATE;
					}
#endif
					else if (!strcmp(dest, "853216"))
					{
						PlanMenuType = SOFTWARE_VERSION_MENU;
						Task |=MEAU_UPDATE;
					}
                    else
                    {
                        hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT1);
                        TEXT_SetText(hItem,Sys_Lib_GetDispStr(DP_PASSWORDINCORRECT));
                        TEXT_SetTextWrapMode(hItem,GUI_WRAPMODE_WORD);
                    }  
                    hItem = WM_GetDialogItem(hDlg, GUI_ID_EDIT0);
                    MULTIEDIT_SetText(hItem,"");
                }
#ifndef MACHINE_TYPE_FLAG
                else
                {
                    for(tmp = 0;tmp < 7; tmp++)
                    {
                        RTEEPROMwrite(FLOW_SERIESNUM_ADDR+tmp,Flow_SeriesNum[tmp]-0x30);
                    }
                    //初始化
                    strcpy(Flow_SeriesNum,"");
                    Flow_SensorFlag = 0;


                    MenuOperateStatus &= ~FRAMEWIN_POP;
                    PlanMenuType = SYSTEM_SETTING_MENU;
                    Task |=MEAU_UPDATE;
                }
#endif
                break;
            case GUI_ID_BUTTON11:
   			    title_barometer_flag = 0;  //关闭提示语句

                Flow_SensorFlag = 0;
			    hItem = WM_GetDialogItem(Menu_hWin.ActivedWin, GUI_ID_MULTIPAGE0);
                MULTIPAGE_SetPageChangeEnable(hItem,1);
                MenuOperateStatus &= ~FRAMEWIN_POP;
                PlanMenuType = SYSTEM_SETTING_MENU;
                Task |=MEAU_UPDATE;
                break;
            default:
#ifndef MACHINE_TYPE_FLAG
                hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT1);
                if(!Flow_SensorFlag)
#endif
                {
                    BUTTON_GetText(pMsg->hWinSrc,src,2);
                    hItem = WM_GetDialogItem(hDlg, GUI_ID_EDIT0);
                    if(MULTIEDIT_GetTextSize(hItem)<=6)
                    {
                        MULTIEDIT_GetText(hItem,dest,6);
                        strcat(dest,src);
                        MULTIEDIT_SetText(hItem,dest);
						//djq add 20140305  大气压力提示语句
						if(!strcmp(dest,"851"))
						{
							title_barometer_flag = 1;
						}
						else if(!strcmp(dest,"841"))
						{
							title_barometer_flag = 2;
						}

						if(title_barometer_flag == 1)
						{
                            hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT1);  
							TEXT_SetTextWrapMode(hItem,GUI_WRAPMODE_WORD);
                            sprintf(title,"%s%s",Sys_Lib_GetDispStr(DP_USEBPSET),(dest + 3));
                            TEXT_SetText(hItem,title);
						}
						else if(title_barometer_flag == 2)
						{
                            hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT1);  
                            TEXT_SetTextWrapMode(hItem,GUI_WRAPMODE_WORD);
							sprintf(title,"%s%s",Sys_Lib_GetDispStr(DP_CALBPSET),(dest + 3));
                            TEXT_SetText(hItem,title);
						}
                    }
                }
#ifndef MACHINE_TYPE_FLAG
                else
                {
                    static char month = 0,cnt = 0;
                    static char String[10]="";
                    BUTTON_GetText(pMsg->hWinSrc,src,2);
                    if(strlen(Flow_SeriesNum)==0)
                    {
                        cnt = 0;
                        strcpy(String,"");
                    }
                    if(cnt < 7)
                    {
                        strcat(Flow_SeriesNum,src);
                        if(cnt == 2)
                        {
                            month = atoi(src)*10;
                        }
                        else if(cnt == 3)
                        {
                            month += atoi(src);
                            if(month>12)
                            {
                                month = 12;
                                Flow_SeriesNum[2] = '1';
                                Flow_SeriesNum[3] = '2';
                            }
                            sprintf(src,"%c",'A'+month-1);
                            strcat(String,src);
                        }
                        else
                        {
                            strcat(String,src);
                        }
                        sprintf(title,"%s\n%s","Input Series Num:",String);
                        TEXT_SetText(hItem,title);
                        cnt++;
                    }
                }
#endif
                break;
            }
            break;
        case WM_NOTIFICATION_RELEASED:
            if(BUTTON_IsPressed(pMsg->hWinSrc))
            {
                BUTTON_SetPressed(pMsg->hWinSrc,0);
            }
            break;
        }
        break;
    default:
        WM_DefaultProc(pMsg);
    }
}
/***************************************************************************************************
* @brief   Valve calibration
* @para   
* @retval 
*/
static void _cbDialog13(WM_MESSAGE * pMsg)
{
    WM_HWIN hButton0,hButton1,hButton2,hButton3;
    WM_HWIN hDlg,hItem;
    int Id,NCode;
    hDlg = pMsg->hWin;
    switch (pMsg->MsgId)
    {
    case WM_INIT_DIALOG:
        WINDOW_SetUse_3D(hDlg,1);
        hButton0 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON0);
        hButton1 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON1);
        hButton2 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON2);
        hButton3 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON3);

        BUTTON_SetText(hButton0,Sys_Lib_GetDispStr(DP_INSPVALVECAL));
        BUTTON_SetText(hButton1,Sys_Lib_GetDispStr(DP_EXPFLOWSENSORCAL));
		BUTTON_SetTextWrapMode(hButton1, GUI_WRAPMODE_WORD);

        BUTTON_SetText(hButton2,Sys_Lib_GetDispStr(DP_EXPVALVECAL));
        BUTTON_SetText(hButton3,Sys_Lib_GetDispStr(DP_EXIT));

        break;
    case WM_NOTIFY_PARENT:
        NCode = pMsg->Data.v;
        Id = WM_GetId(pMsg->hWinSrc);
        switch(NCode)
        {
        case WM_NOTIFICATION_CLICKED:
            if(!(MenuOperateStatus & CALIBRATION_PRECEED))
            {
                switch(Id)
                {
                case GUI_ID_BUTTON0:
                    hItem = WM_GetDialogItem(Menu_hWin.ActivedWin, GUI_ID_MULTIPAGE0);
                    MULTIPAGE_SetPageChangeEnable(hItem,0);
                    SendOperationMessage(INSP_VALVE_CALIBRATION_FACTORY,USART0);
                    CalibrationType = INSP_VALVE_CALIBRATION_FACTORY;
                    MenuOperateStatus |= CALIBRATION_PRECEED;
                    hItem=WM_GetDialogItem(MULTIPAGE_GetWindow(hItem,3), GUI_ID_PROGBAR0);
                    WM_ShowWindow(hItem);
					PROGBAR_SetText(hItem,Sys_Lib_GetDispStr(DP_CALIBRATING));

                    break;
                case GUI_ID_BUTTON1:
                    SendOperationMessage(EXP_FLOW_SENSOR_CALIBRATION,USART0);
                    CalibrationType = EXP_FLOW_SENSOR_CALIBRATION;
                    MenuOperateStatus |= CALIBRATION_PRECEED;
                    hItem = WM_GetDialogItem(Menu_hWin.ActivedWin, GUI_ID_MULTIPAGE0);
                    MULTIPAGE_SetPageChangeEnable(hItem,0);
                    hItem=WM_GetDialogItem(MULTIPAGE_GetWindow(hItem,3), GUI_ID_PROGBAR0);
                    WM_ShowWindow(hItem);
					PROGBAR_SetText(hItem,Sys_Lib_GetDispStr(DP_CALIBRATING));

                    break;
                case GUI_ID_BUTTON2:
                    SendOperationMessage(EXP_VALVE_CALIBRATION,USART0);
                    CalibrationType = EXP_VALVE_CALIBRATION;
                    MenuOperateStatus |= CALIBRATION_PRECEED;
                    hItem = WM_GetDialogItem(Menu_hWin.ActivedWin, GUI_ID_MULTIPAGE0);
                    MULTIPAGE_SetPageChangeEnable(hItem,0);
                    hItem=WM_GetDialogItem(MULTIPAGE_GetWindow(hItem,3), GUI_ID_PROGBAR0);
                    WM_ShowWindow(hItem);
					PROGBAR_SetText(hItem,Sys_Lib_GetDispStr(DP_CALIBRATING));

                    break;
                case GUI_ID_BUTTON3:
                    PlanMenuType = SYSTEM_SETTING_MENU;
                    Task |=MEAU_UPDATE;
                    hItem = WM_GetDialogItem(Menu_hWin.ActivedWin, GUI_ID_MULTIPAGE0);
                    MULTIPAGE_SetPageChangeEnable(hItem,1);
                    hItem=WM_GetDialogItem(MULTIPAGE_GetWindow(hItem,3), GUI_ID_PROGBAR0);
                    WM_HideWindow(hItem);
                    MenuOperateStatus &= ~FRAMEWIN_POP;
                    break;
                }
                break;
            }
            else
            {
                
                switch(Id)
                {
                case GUI_ID_BUTTON0:
                case GUI_ID_BUTTON1:
                case GUI_ID_BUTTON2:
                case GUI_ID_BUTTON3:
                    BUTTON_SetPressed(pMsg->hWinSrc,0);
                }
            }
        }
        break;
    default:
        WM_DefaultProc(pMsg);
    }
}
/***************************************************************************************************
* @brief  standby
* @para   
* @retval 
*/
static void _cbDialog14(WM_MESSAGE * pMsg)
{
    WM_HWIN hDlg,hButton0,hButton1,hItem,hItem_standby;
    int Id,NCode;
    hDlg = pMsg->hWin;
    switch (pMsg->MsgId)
    {
    case WM_INIT_DIALOG:
        FRAMEWIN_SetClientColor(hDlg,GUI_BLACK);
		//FRAMEWIN_SetBarColor(hDlg,1,0x333333);//修改标题栏颜色 1表示激活 0表示未激活
		//FRAMEWIN_SetTitleHeight(hDlg,126);//标题栏宽度调节
		//FRAMEWIN_SetBitmap(hDlg,&bmCN_Image1,215,57,1);
		FRAMEWIN_SetTitleStyle(hDlg,1);
		FRAMEWIN_SetTitleHeight(hDlg,180);
		if(BeathSettingPara.BreathMode == ACGO_MODE)
		{
			FRAMEWIN_SetBitmap(hDlg,&bmCN_Image1,324,57,0);
			FRAMEWIN_SetBitmap(hDlg,&bmACGO,409,65,1);
		}
		else
		{
			FRAMEWIN_SetBitmap(hDlg,&bmCN_Image1,307,57,0);
			FRAMEWIN_SetBitmap(hDlg,&bmEN_Standby,392,65,1);	
		}
		hItem = WM_GetDialogItem(hDlg,GUI_ID_TEXT0);
		TEXT_SetFont(hItem,&GUI_Font_32);
		TEXT_SetBkColor(hItem,GUI_BLACK);
		TEXT_SetTextColor(hItem,GUI_WHITE);
		TEXT_SetTextAlign(hItem,GUI_TA_HCENTER|GUI_TA_VCENTER);
		TEXT_SetText(hItem,Sys_Lib_GetDispStr(DP_NOVENTDELIVER));

        //hItem_standby = WM_GetDialogItem(hDlg, GUI_ID_IMAGETEXT1);
        //IMAGETEXT_SetBkColor(hItem_standby,0);
        //hItem = WM_GetDialogItem(hDlg, GUI_ID_IMAGETEXT0);
        //IMAGETEXT_SetBitmap(hItem,&bmWarning);
        //IMAGETEXT_SetBkColor(hItem,GUI_YELLOW);
        //IMAGETEXT_SetTextColor(hItem,GUI_BLACK);
        hButton0 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON0);
        BUTTON_SetBkColor(hButton0,0,0xD05356);
        BUTTON_SetBkColor(hButton0,3,GUI_BLACK);
		BUTTON_SetText(hButton0,Sys_Lib_GetDispStr(DP_SYSTEMTEST));  
		BUTTON_SetFont(hButton0, &GUI_Font_32);
		hButton1 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON1);
		BUTTON_SetBkColor(hButton1,0,0xD05356);
		BUTTON_SetBkColor(hButton1,3,GUI_BLACK);
		BUTTON_SetText(hButton1,Sys_Lib_GetDispStr(DP_PATIENT));  
		BUTTON_SetFont(hButton1, &GUI_Font_32);

#ifndef SELFTEST        
        WM_HideWindow(hButton0);   //不加入自检的程序 隐藏
#endif
        BUTTON_SetText(hButton0,Sys_Lib_GetDispStr(DP_SYSTEMTEST));        
        break;
    case WM_NOTIFY_PARENT:
        NCode = pMsg->Data.v;
        Id = WM_GetId(pMsg->hWinSrc);;
        switch(NCode)
        {
        case WM_NOTIFICATION_CLICKED:
			switch(Id)
			{
			case GUI_ID_BUTTON0:
				BUTTON_SetPressed(WM_GetDialogItem(Menu_hWin.hWin,GUI_ID_BUTTON0),0);
				BUTTON_SetPressed(WM_GetDialogItem(Menu_hWin.hWin,GUI_ID_BUTTON7),0);
				BUTTON_SetPressed(WM_GetDialogItem(Menu_hWin.hWin,GUI_ID_BUTTON2),0);
				BUTTON_SetPressed(WM_GetDialogItem(Menu_hWin.hWin,GUI_ID_BUTTON6),0);
				PlanMenuType=SELFCHECK_MENU;
				Task=MEAU_UPDATE;
				BUTTON_SetPressed(pMsg->hWinSrc,0);
				break;
			case GUI_ID_BUTTON1:
				PlanMenuType = PATIENT_MENU;
				Task = MEAU_UPDATE;
				BUTTON_SetPressed(pMsg->hWinSrc, 0);
				break;
			}
        }
        break;        
    default:
        WM_DefaultProc(pMsg);
    }
}
/*************************************************************************
*   _cbSelfCheck_Ask 
**/
static void _cbSelfCheck_Ask(WM_MESSAGE * pMsg)
{
    WM_HWIN hDlg,hItem,hButton0,hButton1,hText0;
    int Id,NCode;
    char Value[5];
	hDlg = pMsg->hWin;
    switch (pMsg->MsgId)
    {
    case WM_INIT_DIALOG:
        CHECK_DEBUG = 2;
        AskTimer = 5;

        hButton0 = WM_GetDialogItem(hDlg,GUI_ID_BUTTON0);
        BUTTON_SetFont(hButton0,&GUI_Font_24);
        
        BUTTON_SetBkColor(hButton0,0,0xD05356);
        hButton1 = WM_GetDialogItem(hDlg,GUI_ID_BUTTON1);
        BUTTON_SetFont(hButton1,&GUI_Font_24);
        
        BUTTON_SetBkColor(hButton1,0,0xD05356);
        hText0 = WM_GetDialogItem(hDlg,GUI_ID_TEXT0);
        TEXT_SetFont(hText0,&GUI_Font_32);
		//TEXT_SetTextAlign(hText0, GUI_TA_HCENTER|GUI_TA_VCENTER);
        
        hItem = WM_GetDialogItem(hDlg,GUI_ID_TEXT1);
        TEXT_SetFont(hItem,&GUI_Font_24);
        sprintf(Value,"%d",AskTimer);
        TEXT_SetText(hItem,Value);

        BUTTON_SetText(hButton0,Sys_Lib_GetDispStr(DP_CANCEL));
        BUTTON_SetText(hButton1,Sys_Lib_GetDispStr(DP_ENTER));
        TEXT_SetText(hText0,Sys_Lib_GetDispStr(DP_STARTPREUSETEST));
 
        break;
    case WM_NOTIFY_PARENT:
        NCode = pMsg->Data.v;
        Id = WM_GetId(pMsg->hWinSrc);
        switch(NCode)
        {
        case WM_NOTIFICATION_CLICKED:
            if(Id == GUI_ID_BUTTON0)//cancel
            {
                PlanMenuType = STANDBY_MENU;
                Task |= MEAU_UPDATE;
            }
            else if(Id == GUI_ID_BUTTON1)//accept
            {
                PlanMenuType = SELFCHECK_MENU;
                Task |= MEAU_UPDATE;
            }
            AskMenu_Flag = 0;//2018
            CHECK_DEBUG = 0;
            break;
        }
        break;
    case WM_USER:
        if(!AskTimer)
        {
            CHECK_DEBUG = 0;
            PlanMenuType = STANDBY_MENU;
            Task |= MEAU_UPDATE;
        }
        hItem = WM_GetDialogItem(hDlg,GUI_ID_TEXT1);
        sprintf(Value,"%d",AskTimer);
        TEXT_SetText(hItem,Value);
        break;
    default:
        WM_DefaultProc(pMsg);
		break;
    }
}
/*************************************************************************
*   _cbTrendData_Ask 
**/
static void _cbTrendData_Ask(WM_MESSAGE * pMsg)
{
	WM_HWIN hDlg,hItem;
	WM_MESSAGE Msg;
	int Id,NCode;
	char Value[5];
	hDlg = pMsg->hWin;
	switch (pMsg->MsgId)
	{
	case WM_INIT_DIALOG:
		//WINDOW_SetBkColor(hDlg,WINDOW_BK_COLOR);
		//WINDOW_SetFrameFlag(hDlg,2);

		hItem = WM_GetDialogItem(hDlg,GUI_ID_BUTTON0);
		BUTTON_SetFont(hItem,&GUI_Font_24);
		BUTTON_SetText(hItem,Sys_Lib_GetDispStr(DP_CANCEL));
		hItem = WM_GetDialogItem(hDlg,GUI_ID_BUTTON1);
		BUTTON_SetFont(hItem,&GUI_Font_24);
		BUTTON_SetText(hItem,Sys_Lib_GetDispStr(DP_CONFIRM));
		BUTTON_SetBkColor(hItem,0,GUI_WHITE);//
		hItem = WM_GetDialogItem(hDlg,GUI_ID_TEXT0);
		TEXT_SetFont(hItem,&GUI_Font_24);
		TEXT_SetText(hItem,Sys_Lib_GetDispStr(DP_CLEARTRENDDATA));
		hItem = WM_GetDialogItem(hDlg,GUI_ID_TEXT1);
		TEXT_SetText(hItem,"");
		break;
	case WM_NOTIFY_PARENT:
		NCode = pMsg->Data.v;
		Id = WM_GetId(pMsg->hWinSrc);
		switch(NCode)
		{
		case WM_NOTIFICATION_CLICKED:
			if(Id == GUI_ID_BUTTON0)//cancel
			{
			}
			else if(Id == GUI_ID_BUTTON1)//accept
			{
				//清空趋势图信息
				ClearTrendInfo();
			}
			PlanMenuType=STANDBY_MENU;
			Task|=MEAU_UPDATE;
			break;
		}
		break;
	default:
		WM_DefaultProc(pMsg);
		break;
	}
}

void CurveInitSetup(WM_HMEM hItem, char loopSign, int range, char flagSignY)
{
	TracingCurve_SetXScale(hItem, 1);
	TracingCurve_SetViewMode(hItem, 0, loopSign);
	TracingCurve_SetExceedRangeFlg(hItem, 0);
	TracingCurve_SetRange(hItem, range);
	TracingCurve_SetYsign(hItem, flagSignY);  
}
//相关波形创建
const int g_curve1_x = 226;
const int g_curve1_x1 = 476;
const int g_curve1_y = 179;
const int g_curve_x_size = 500;
const int g_curve_y_size = 177;
const int g_curve2_x = 226;
const int g_curve2_y = 357;
const int g_curve3_x = 226;
const int g_curve3_y = 357;
const int g_loop_x_size = 249;
const int g_loop_y_size = 177;

//const int g_loop_choose_btn1_x = 693;
//const int g_curve_choose_btn1_x = 672;
//const int g_choose_btn1_y = 203;

const int g_choose_btn1_x = 669;
const int g_choose_btn1_x1 = 689;
const int g_choose_btn1_y = 203;

const int g_choose_btn2_x = 669;
const int g_choose_btn2_x1 = 689;
const int g_choose_btn2_y = 380;

const int g_choose_btn3_x = 440;
const int g_choose_btn3_y = 203;
const int g_choose_btn3_y1 = 380;

static unsigned char s_curve_menu_flag = 0;

void WaveFromMenu(unsigned Id)
{
	WM_HMEM hItem, hItem1, hItem2;
	int focus_curve = 0;

	//int xSize, ySize, xSizeLoop, ySizeLoop;
	//int x, xLoop, y0, y1, y2, yLoop;//  [2/14/2022 yaoyao add]
	int nXMax = 8;//  [2/14/2022 yaoyao add]波形x轴量程最大值

    switch(Id)
    {
    case WAVEFORM_PRESSURE:     
		if(Menu_hWin.cWin.LoopFlag1 != 0) // 之前是环图
		{
			if(Menu_hWin.cWin.LoopFlag1 == Menu_hWin.cWin.FocusCurve || Menu_hWin.cWin.FocusCurve == 3)
			{
                Menu_hWin.cWin.LoopFlag1 = 0;

				WM_HideWindow(Menu_hWin.HomeWin.Tobj[3]); // 隐藏第三道LOOP
				WM_HideWindow(WM_GetDialogItem(Menu_hWin.HomeWin.ParentWin, GUI_ID_BUTTON3)); // 波形选择按钮
			}
			//
			hItem = Menu_hWin.HomeWin.Tobj[s_last_focus_curve3];
			focus_curve = s_last_focus_curve3;
            
			if(s_last_focus_curve3 == 1)
			{
				WM_SetSize(hItem, g_curve_x_size, g_curve_y_size);
				WM_MoveChildTo(hItem, g_curve1_x, g_curve1_y);
				WM_MoveChildTo(WM_GetDialogItem(Menu_hWin.HomeWin.ParentWin, GUI_ID_BUTTON1), g_choose_btn1_x, g_choose_btn1_y); // 波形选择按钮
			}
			else if(s_last_focus_curve3 == 2)
			{
				WM_SetSize(hItem, g_curve_x_size, g_curve_y_size);
				WM_MoveChildTo(hItem, g_curve2_x, g_curve2_y);
				WM_MoveChildTo(WM_GetDialogItem(Menu_hWin.HomeWin.ParentWin, GUI_ID_BUTTON2), g_choose_btn2_x, g_choose_btn2_y);
			}
		}
		else
		{
			hItem = Menu_hWin.HomeWin.Tobj[Menu_hWin.cWin.FocusCurve];
			focus_curve = Menu_hWin.cWin.FocusCurve;
		}

		CurveInitSetup(hItem, 'F', MultiArray[0], 'T');

		TracingCurve_SetXYINFO(hItem, 'T', 0, nXMax, 1, GUI_WHITE, Sys_Lib_GetDispStr(DP_S), -1 ,5, 1, GUI_WHITE, 
			Sys_Lib_GetDispStr(DP_CMH2O), GUI_WHITE, Sys_Lib_GetDispStr(DP_PAW));

		Menu_hWin.cWin.WaveForms[focus_curve] = WAVEFORM_PRESSURE;
        break;

#ifdef AA_WAVE_SWITCH
    case WAVEFORM_N2O:
		if(Menu_hWin.cWin.LoopFlag1 != 0) // 之前是环图
		{
			if(Menu_hWin.cWin.LoopFlag1 == Menu_hWin.cWin.FocusCurve || Menu_hWin.cWin.FocusCurve == 3)
			{                
                Menu_hWin.cWin.LoopFlag1 = 0;

				WM_HideWindow(Menu_hWin.HomeWin.Tobj[3]); // 隐藏第三道LOOP
				WM_HideWindow(WM_GetDialogItem(Menu_hWin.HomeWin.ParentWin, GUI_ID_BUTTON3)); // 波形选择按钮
			}
			//
			hItem = Menu_hWin.HomeWin.Tobj[s_last_focus_curve3];
			focus_curve = s_last_focus_curve3;
            
			if(s_last_focus_curve3 == 1)
			{
				WM_SetSize(hItem, g_curve_x_size, g_curve_y_size);
				WM_MoveChildTo(hItem, g_curve1_x, g_curve1_y);
				WM_MoveChildTo(WM_GetDialogItem(Menu_hWin.HomeWin.ParentWin, GUI_ID_BUTTON1), g_choose_btn1_x, g_choose_btn1_y); // 波形选择按钮
			}
			else if(s_last_focus_curve3 == 2)
			{
				WM_SetSize(hItem, g_curve_x_size, g_curve_y_size);
				WM_MoveChildTo(hItem, g_curve2_x, g_curve2_y);
				WM_MoveChildTo(WM_GetDialogItem(Menu_hWin.HomeWin.ParentWin, GUI_ID_BUTTON2), g_choose_btn2_x, g_choose_btn2_y);
			}
		}
		else
		{
			hItem = Menu_hWin.HomeWin.Tobj[Menu_hWin.cWin.FocusCurve];
			focus_curve = Menu_hWin.cWin.FocusCurve;
		}

		CurveInitSetup(hItem, 'F', 1, 'T');

		TracingCurve_SetXYINFO(hItem,'T',0,nXMax,1,GUI_WHITE,Sys_Lib_GetDispStr(DP_S),0,100,20,
			GUI_WHITE,"%",GUI_WHITE,Sys_Lib_GetDispStr(DP_N2O));

		Menu_hWin.cWin.WaveForms[focus_curve] = WAVEFORM_N2O;
		break;

    case WAVEFORM_PAA:   
		if(Menu_hWin.cWin.LoopFlag1 != 0) // 之前是环图
		{
			if(Menu_hWin.cWin.LoopFlag1 == Menu_hWin.cWin.FocusCurve || Menu_hWin.cWin.FocusCurve == 3)
			{                
                Menu_hWin.cWin.LoopFlag1 = 0;

				WM_HideWindow(Menu_hWin.HomeWin.Tobj[3]); // 隐藏第三道LOOP
				WM_HideWindow(WM_GetDialogItem(Menu_hWin.HomeWin.ParentWin, GUI_ID_BUTTON3)); // 波形选择按钮
			}
			//
			hItem = Menu_hWin.HomeWin.Tobj[s_last_focus_curve3];
			focus_curve = s_last_focus_curve3;
            
			if(s_last_focus_curve3 == 1)
			{
				WM_SetSize(hItem, g_curve_x_size, g_curve_y_size);
				WM_MoveChildTo(hItem, g_curve1_x, g_curve1_y);
				WM_MoveChildTo(WM_GetDialogItem(Menu_hWin.HomeWin.ParentWin, GUI_ID_BUTTON1), g_choose_btn1_x, g_choose_btn1_y); // 波形选择按钮
			}
			else if(s_last_focus_curve3 == 2)
			{
				WM_SetSize(hItem, g_curve_x_size, g_curve_y_size);
				WM_MoveChildTo(hItem, g_curve2_x, g_curve2_y);
				WM_MoveChildTo(WM_GetDialogItem(Menu_hWin.HomeWin.ParentWin, GUI_ID_BUTTON2), g_choose_btn2_x, g_choose_btn2_y);
			}
		}
		else
		{
			hItem = Menu_hWin.HomeWin.Tobj[Menu_hWin.cWin.FocusCurve];
			focus_curve = Menu_hWin.cWin.FocusCurve;
		}

		CurveInitSetup(hItem, 'F', MultiAA[0], 'T');

		TracingCurve_SetXYINFO(hItem,'T',0,nXMax,1,GUI_WHITE,
			Sys_Lib_GetDispStr(DP_S),0,5,1,GUI_WHITE,"%",GUI_WHITE,AA_all_data.AA_detail.AA1_Type_Text);

		Menu_hWin.cWin.WaveForms[focus_curve] = WAVEFORM_PAA;
        break;

    case WAVEFORM_SAA:
		if(Menu_hWin.cWin.LoopFlag1 != 0) // 之前是环图
		{
			if(Menu_hWin.cWin.LoopFlag1 == Menu_hWin.cWin.FocusCurve || Menu_hWin.cWin.FocusCurve == 3)
			{
                Menu_hWin.cWin.LoopFlag1 = 0;

				WM_HideWindow(Menu_hWin.HomeWin.Tobj[3]); // 隐藏第三道LOOP
				WM_HideWindow(WM_GetDialogItem(Menu_hWin.HomeWin.ParentWin, GUI_ID_BUTTON3)); // 波形选择按钮
			}
			//
			hItem = Menu_hWin.HomeWin.Tobj[s_last_focus_curve3];
			focus_curve = s_last_focus_curve3;
			if(s_last_focus_curve3 == 1)
			{
				WM_SetSize(hItem, g_curve_x_size, g_curve_y_size);
				WM_MoveChildTo(hItem, g_curve1_x, g_curve1_y);
				WM_MoveChildTo(WM_GetDialogItem(Menu_hWin.HomeWin.ParentWin, GUI_ID_BUTTON1), g_choose_btn1_x, g_choose_btn1_y); // 波形选择按钮
			}
			else if(s_last_focus_curve3 == 2)
			{
				WM_SetSize(hItem, g_curve_x_size, g_curve_y_size);
				WM_MoveChildTo(hItem, g_curve2_x, g_curve2_y);
				WM_MoveChildTo(WM_GetDialogItem(Menu_hWin.HomeWin.ParentWin, GUI_ID_BUTTON2), g_choose_btn2_x, g_choose_btn2_y);
			}
		}
		else
		{
			hItem = Menu_hWin.HomeWin.Tobj[Menu_hWin.cWin.FocusCurve];
			focus_curve = Menu_hWin.cWin.FocusCurve;
		}

		CurveInitSetup(hItem, 'F', MultiAA[1], 'T');

		TracingCurve_SetXYINFO(hItem,'T',0,nXMax,1,GUI_WHITE,\
			Sys_Lib_GetDispStr(DP_S),0,5,1,GUI_WHITE,"%",GUI_WHITE,AA_all_data.AA_detail.AA2_Type_Text);

        Menu_hWin.cWin.WaveForms[focus_curve] = WAVEFORM_SAA;
        break;
#endif

    case WAVEFORM_FLOW:
		if(Menu_hWin.cWin.LoopFlag1 != 0) // 之前是环图
		{
			if(Menu_hWin.cWin.LoopFlag1 == Menu_hWin.cWin.FocusCurve || Menu_hWin.cWin.FocusCurve == 3)
			{
                Menu_hWin.cWin.LoopFlag1 = 0;

				WM_HideWindow(Menu_hWin.HomeWin.Tobj[3]); // 隐藏第三道LOOP
				WM_HideWindow(WM_GetDialogItem(Menu_hWin.HomeWin.ParentWin, GUI_ID_BUTTON3)); // 波形选择按钮
			}
			//
			hItem = Menu_hWin.HomeWin.Tobj[s_last_focus_curve3];
			focus_curve = s_last_focus_curve3;

			if(s_last_focus_curve3 == 1)
			{
				WM_SetSize(hItem, g_curve_x_size, g_curve_y_size);
				WM_MoveChildTo(hItem, g_curve1_x, g_curve1_y);
				WM_MoveChildTo(WM_GetDialogItem(Menu_hWin.HomeWin.ParentWin, GUI_ID_BUTTON1), g_choose_btn1_x, g_choose_btn1_y); // 波形选择按钮
			}
			else if(s_last_focus_curve3 == 2)
			{
				WM_SetSize(hItem, g_curve_x_size, g_curve_y_size);
				WM_MoveChildTo(hItem, g_curve2_x, g_curve2_y);
				WM_MoveChildTo(WM_GetDialogItem(Menu_hWin.HomeWin.ParentWin, GUI_ID_BUTTON2), g_choose_btn2_x, g_choose_btn2_y);
			}
		}
		else
		{
			hItem = Menu_hWin.HomeWin.Tobj[Menu_hWin.cWin.FocusCurve];
			focus_curve = Menu_hWin.cWin.FocusCurve;
		}

		CurveInitSetup(hItem, 'F', MultiArray[1], 'T');

		TracingCurve_SetXYINFO(hItem,'T',0,nXMax,1,GUI_WHITE,
			Sys_Lib_GetDispStr(DP_S),-3,3,1,GUI_WHITE,"LPM",GUI_WHITE,Sys_Lib_GetDispStr(DP_FLOW));

		Menu_hWin.cWin.WaveForms[focus_curve] = WAVEFORM_FLOW;

        break;

    case WAVEFORM_VOLUME:
		if(Menu_hWin.cWin.LoopFlag1 != 0) // 之前是环图
		{
			if(Menu_hWin.cWin.LoopFlag1 == Menu_hWin.cWin.FocusCurve || Menu_hWin.cWin.FocusCurve == 3)
			{
                Menu_hWin.cWin.LoopFlag1 = 0;

				WM_HideWindow(Menu_hWin.HomeWin.Tobj[3]); // 隐藏第三道LOOP
				WM_HideWindow(WM_GetDialogItem(Menu_hWin.HomeWin.ParentWin, GUI_ID_BUTTON3)); // 波形选择按钮
			}
			//
			hItem = Menu_hWin.HomeWin.Tobj[s_last_focus_curve3];
			focus_curve = s_last_focus_curve3;
			if(s_last_focus_curve3 == 1)
			{
				WM_SetSize(hItem, g_curve_x_size, g_curve_y_size);
				WM_MoveChildTo(hItem, g_curve1_x, g_curve1_y);
				WM_MoveChildTo(WM_GetDialogItem(Menu_hWin.HomeWin.ParentWin, GUI_ID_BUTTON1), g_choose_btn1_x, g_choose_btn1_y); // 波形选择按钮
			}
			else if(s_last_focus_curve3 == 2)
			{
				WM_SetSize(hItem, g_curve_x_size, g_curve_y_size);
				WM_MoveChildTo(hItem, g_curve2_x, g_curve2_y);
				WM_MoveChildTo(WM_GetDialogItem(Menu_hWin.HomeWin.ParentWin, GUI_ID_BUTTON2), g_choose_btn2_x, g_choose_btn2_y);
			}
		}
		else
		{
			hItem = Menu_hWin.HomeWin.Tobj[Menu_hWin.cWin.FocusCurve];
			focus_curve = Menu_hWin.cWin.FocusCurve;
		}

		CurveInitSetup(hItem, 'F', MultiArray[2], 'T');

		TracingCurve_SetXYINFO(hItem,'T',0,nXMax,1,GUI_WHITE,
			Sys_Lib_GetDispStr(DP_S),0,30,5,GUI_WHITE,"mL",GUI_WHITE,Sys_Lib_GetDispStr(DP_VOLUME));

		Menu_hWin.cWin.WaveForms[focus_curve] = WAVEFORM_VOLUME;
        break;

#ifdef SPO2_SWITCH
    case WAVEFORM_SPO2://心率
		if(Menu_hWin.cWin.LoopFlag1 != 0) // 之前是环图
		{
			if(Menu_hWin.cWin.LoopFlag1 == Menu_hWin.cWin.FocusCurve || Menu_hWin.cWin.FocusCurve == 3)
			{
                Menu_hWin.cWin.LoopFlag1 = 0;

				WM_HideWindow(Menu_hWin.HomeWin.Tobj[3]); // 隐藏第三道LOOP
				WM_HideWindow(WM_GetDialogItem(Menu_hWin.HomeWin.ParentWin, GUI_ID_BUTTON3)); // 波形选择按钮
			}
			//
			hItem = Menu_hWin.HomeWin.Tobj[s_last_focus_curve3];
			focus_curve = s_last_focus_curve3;
			if(s_last_focus_curve3 == 1)
			{
				WM_SetSize(hItem, g_curve_x_size, g_curve_y_size);
				WM_MoveChildTo(hItem, g_curve1_x, g_curve1_y);
				WM_MoveChildTo(WM_GetDialogItem(Menu_hWin.HomeWin.ParentWin, GUI_ID_BUTTON1), g_choose_btn1_x, g_choose_btn1_y); // 波形选择按钮
			}
			else if(s_last_focus_curve3 == 2)
			{
				WM_SetSize(hItem, g_curve_x_size, g_curve_y_size);
				WM_MoveChildTo(hItem, g_curve2_x, g_curve2_y);
				WM_MoveChildTo(WM_GetDialogItem(Menu_hWin.HomeWin.ParentWin, GUI_ID_BUTTON2), g_choose_btn2_x, g_choose_btn2_y);
			}
		}
		else
		{
			hItem = Menu_hWin.HomeWin.Tobj[Menu_hWin.cWin.FocusCurve];
			focus_curve = Menu_hWin.cWin.FocusCurve;
		}

		TracingCurve_SetXScale(hItem, 1);
		TracingCurve_SetViewMode(hItem, 0, 'F');

        if(Masimo_SpO2_Config_On)
		{
			TracingCurve_SetRange(hItem,20);
			TracingCurve_SetXYINFO(hItem,'T',0,nXMax,1,GUI_WHITE,Sys_Lib_GetDispStr(DP_S),-5,5,1,GUI_WHITE,"",GUI_WHITE, Sys_Lib_GetDispStr(DP_PLETH));
		}
		else
		{
			TracingCurve_SetRange(hItem,1);
			TracingCurve_SetXYINFO(hItem,'T',0,nXMax,1,GUI_WHITE,Sys_Lib_GetDispStr(DP_S),0,100,20,GUI_WHITE,"",GUI_WHITE, Sys_Lib_GetDispStr(DP_PLETH));                 
			TracingCurve_SetYsign(hItem,'F');
		}
        TracingCurve_SetExceedRangeFlg(hItem,0);

        Menu_hWin.cWin.WaveForms[focus_curve] = WAVEFORM_SPO2;
        break;

#endif
    case WAVEFORM_CO2:
		if(Menu_hWin.cWin.LoopFlag1 != 0) // 之前是环图
		{
			if(Menu_hWin.cWin.LoopFlag1 == Menu_hWin.cWin.FocusCurve || Menu_hWin.cWin.FocusCurve == 3)
			{
                Menu_hWin.cWin.LoopFlag1 = 0;

				WM_HideWindow(Menu_hWin.HomeWin.Tobj[3]); // 隐藏第三道LOOP
				WM_HideWindow(WM_GetDialogItem(Menu_hWin.HomeWin.ParentWin, GUI_ID_BUTTON3)); // 波形选择按钮
			}
			//
			hItem = Menu_hWin.HomeWin.Tobj[s_last_focus_curve3];
			focus_curve = s_last_focus_curve3;
			if(s_last_focus_curve3 == 1)
			{
				WM_SetSize(hItem, g_curve_x_size, g_curve_y_size);
				WM_MoveChildTo(hItem, g_curve1_x, g_curve1_y);
				WM_MoveChildTo(WM_GetDialogItem(Menu_hWin.HomeWin.ParentWin, GUI_ID_BUTTON1), g_choose_btn1_x, g_choose_btn1_y); // 波形选择按钮
			}
			else if(s_last_focus_curve3 == 2)
			{
				WM_SetSize(hItem, g_curve_x_size, g_curve_y_size);
				WM_MoveChildTo(hItem, g_curve2_x, g_curve2_y);
				WM_MoveChildTo(WM_GetDialogItem(Menu_hWin.HomeWin.ParentWin, GUI_ID_BUTTON2), g_choose_btn2_x, g_choose_btn2_y);
			}
		}
		else
		{
			hItem = Menu_hWin.HomeWin.Tobj[Menu_hWin.cWin.FocusCurve];
			focus_curve = Menu_hWin.cWin.FocusCurve;
		}
		
		CurveInitSetup(hItem, 'F', 1, 'T');

		TracingCurve_SetXYINFO(hItem,'T',0,nXMax,1,GUI_WHITE,
			Sys_Lib_GetDispStr(DP_S),0,CO2_Disp.co2max,CO2_Disp.co2step,GUI_WHITE,CO2_Disp.co2_unit,GUI_WHITE,Sys_Lib_GetDispStr(DP_CO2));

		Menu_hWin.cWin.WaveForms[focus_curve] = WAVEFORM_CO2;
        break;

	case LOOP_V_CO2:
		if(Menu_hWin.cWin.LoopFlag1 != 0) // 之前是环图
		{
			if(Menu_hWin.cWin.LoopFlag1 == Menu_hWin.cWin.FocusCurve || Menu_hWin.cWin.FocusCurve == 3)
			{
                Menu_hWin.cWin.LoopFlag1 = 0;

				WM_HideWindow(Menu_hWin.HomeWin.Tobj[3]); // 隐藏第三道LOOP
				WM_HideWindow(WM_GetDialogItem(Menu_hWin.HomeWin.ParentWin, GUI_ID_BUTTON3)); // 波形选择按钮
			}
			//
			hItem = Menu_hWin.HomeWin.Tobj[s_last_focus_curve3];
			focus_curve = s_last_focus_curve3;
			if(s_last_focus_curve3 == 1)
			{
				WM_SetSize(hItem, g_curve_x_size, g_curve_y_size);
				WM_MoveChildTo(hItem, g_curve1_x, g_curve1_y);
				WM_MoveChildTo(WM_GetDialogItem(Menu_hWin.HomeWin.ParentWin, GUI_ID_BUTTON1), g_choose_btn1_x, g_choose_btn1_y); // 波形选择按钮
			}
			else if(s_last_focus_curve3 == 2)
			{
				WM_SetSize(hItem, g_curve_x_size, g_curve_y_size);
				WM_MoveChildTo(hItem, g_curve2_x, g_curve2_y);
				WM_MoveChildTo(WM_GetDialogItem(Menu_hWin.HomeWin.ParentWin, GUI_ID_BUTTON2), g_choose_btn2_x, g_choose_btn2_y);
			}
		}
		else
		{
			hItem = Menu_hWin.HomeWin.Tobj[Menu_hWin.cWin.FocusCurve];
			focus_curve = Menu_hWin.cWin.FocusCurve;
		}

		CurveInitSetup(hItem, 'F', 1, 'T');

		TracingCurve_SetXYINFO(hItem,'T',0,30,5,GUI_WHITE,
			Sys_Lib_GetDispStr(DP_ML),0,CO2_Disp.co2max,CO2_Disp.co2step,GUI_WHITE,CO2_Disp.co2_unit,GUI_WHITE,Sys_Lib_GetDispStr(DP_VCO2));

		Menu_hWin.cWin.WaveForms[focus_curve] = LOOP_V_CO2;
		break;

	case WAVEFORM_LOOP:
		WM_ShowWindow(WM_GetDialogItem(Menu_hWin.HomeWin.ParentWin, GUI_ID_BUTTON3)); // 波形选择按钮3

		Menu_hWin.cWin.LoopFlag1 = Menu_hWin.cWin.FocusCurve; // 指示第几道波形变成了环图

		hItem = Menu_hWin.HomeWin.Tobj[Menu_hWin.cWin.FocusCurve];
		WM_SetSize(hItem, g_loop_x_size, g_loop_y_size);
		if(Menu_hWin.cWin.FocusCurve == 1)
		{
			WM_MoveChildTo(hItem, g_curve1_x1, g_curve1_y);
        	WM_MoveChildTo(WM_GetDialogItem(Menu_hWin.HomeWin.ParentWin, GUI_ID_BUTTON1), g_choose_btn1_x1, g_choose_btn1_y);
			WM_MoveChildTo(WM_GetDialogItem(Menu_hWin.HomeWin.ParentWin, GUI_ID_BUTTON3), g_choose_btn3_x, g_choose_btn3_y); 
		}			
		else
		{
			WM_MoveChildTo(hItem, g_curve1_x1, g_curve2_y);
        	WM_MoveChildTo(WM_GetDialogItem(Menu_hWin.HomeWin.ParentWin, GUI_ID_BUTTON2), g_choose_btn2_x1, g_choose_btn2_y);            
			WM_MoveChildTo(WM_GetDialogItem(Menu_hWin.HomeWin.ParentWin, GUI_ID_BUTTON3), g_choose_btn3_x, g_choose_btn3_y1);
		}

		CurveInitSetup(hItem, 'T', MultiArray[2], 'T');
		TracingCurve_SetRangeX(hItem, MultiArray[0]);

		TracingCurve_SetXYINFO(hItem,'T',-1,5,1,GUI_WHITE,Sys_Lib_GetDispStr(DP_CMH2O),\
			0,30,5,GUI_WHITE,Sys_Lib_GetDispStr(DP_ML),GUI_WHITE,"P-V");

		Menu_hWin.cWin.WaveForms[Menu_hWin.cWin.FocusCurve] = LOOP_PV;

		//
		hItem = Menu_hWin.HomeWin.Tobj[3];
		WM_ShowWindow(hItem);

		if(Menu_hWin.cWin.FocusCurve == 1)
			WM_MoveChildTo(hItem, g_curve3_x, g_curve1_y);
		else
			WM_MoveChildTo(hItem, g_curve3_x, g_curve2_y);

		CurveInitSetup(hItem, 'T', MultiArray[1], 'T');
		TracingCurve_SetRangeX(hItem, MultiArray[2]);

		TracingCurve_SetXYINFO(hItem,'T',0,30,5,GUI_WHITE,Sys_Lib_GetDispStr(DP_ML),
			-3,3,1,GUI_WHITE,Sys_Lib_GetDispStr(DP_LPM),GUI_WHITE,"V-F");

		Menu_hWin.cWin.WaveForms[3/*2*/] = LOOP_VF;

		break;

    case LOOP_PV:
		hItem = Menu_hWin.HomeWin.Tobj[Menu_hWin.cWin.FocusCurve];
		WM_SetSize(hItem, g_loop_x_size, g_loop_y_size);
		if(Menu_hWin.cWin.FocusCurve == 3)
		{
			WM_ShowWindow(hItem);
		}
		CurveInitSetup(hItem, 'T', MultiArray[2], 'T');
		TracingCurve_SetRangeX(hItem, MultiArray[0]);

		TracingCurve_SetXYINFO(hItem,'T',-1,5,1,GUI_WHITE,Sys_Lib_GetDispStr(DP_CMH2O),\
			0,30,5,GUI_WHITE,Sys_Lib_GetDispStr(DP_ML),GUI_WHITE,"P-V");

		Menu_hWin.cWin.WaveForms[Menu_hWin.cWin.FocusCurve] = LOOP_PV;
		break;

	case LOOP_VF:
		hItem = Menu_hWin.HomeWin.Tobj[Menu_hWin.cWin.FocusCurve];

		if(Menu_hWin.cWin.FocusCurve == 3)
		{
			WM_ShowWindow(hItem);
		}

		WM_SetSize(hItem, g_loop_x_size, g_loop_y_size);

		//if(Menu_hWin.cWin.FocusCurve == 1)
		//	WM_MoveChildTo(hItem, g_curve3_x, g_curve1_y);
		//else
		//	WM_MoveChildTo(hItem, g_curve3_x, g_curve2_y);

		CurveInitSetup(hItem, 'T', MultiArray[1], 'T');
		TracingCurve_SetRangeX(hItem, MultiArray[2]);

		TracingCurve_SetXYINFO(hItem,'T',0,30,5,GUI_WHITE,Sys_Lib_GetDispStr(DP_ML),
			-3,3,1,GUI_WHITE,Sys_Lib_GetDispStr(DP_LPM),GUI_WHITE,"V-F");

		Menu_hWin.cWin.WaveForms[Menu_hWin.cWin.FocusCurve] = LOOP_VF;
		break;

	case LOOP_PF:
		hItem = Menu_hWin.HomeWin.Tobj[Menu_hWin.cWin.FocusCurve];
		if(Menu_hWin.cWin.FocusCurve == 3)
		{
			WM_ShowWindow(hItem);
		}
		WM_SetSize(hItem, g_loop_x_size, g_loop_y_size);

		CurveInitSetup(hItem, 'T', MultiArray[1], 'T');
		TracingCurve_SetRangeX(hItem, MultiArray[0]);

		TracingCurve_SetXYINFO(hItem, 'T', -1, 5, 1, GUI_WHITE, Sys_Lib_GetDispStr(DP_CMH2O), -3, 3, 1, GUI_WHITE,
			Sys_Lib_GetDispStr(DP_LMIN), GUI_WHITE, "P-F");

		Menu_hWin.cWin.WaveForms[Menu_hWin.cWin.FocusCurve] = LOOP_PF;
		break;
    }

	WaveFormsId2 = Menu_hWin.cWin.WaveForms[2];
}

// 新波形初始化函数
void CurveInit(WM_HWIN hDlg, unsigned char id, unsigned char focus_curve)
{
    WM_HWIN hItem;
    int nXMax = 8;

    switch(id)
    {
    case WAVEFORM_PRESSURE:     
		if(Menu_hWin.cWin.LoopFlag1 == Menu_hWin.cWin.FocusCurve || Menu_hWin.cWin.FocusCurve == 3) {		
            Menu_hWin.cWin.LoopFlag1 = 0;
            
            WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_CURVE3)); // 隐藏第三道LOOP 
            WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON3)); // 波形选择按钮
		}
        
        hItem = WM_GetDialogItem(hDlg, GUI_ID_CURVE1);
		if(focus_curve == 1) {		
			WM_SetSize(hItem, g_curve_x_size, g_curve_y_size);
			WM_MoveChildTo(hItem, g_curve1_x, g_curve1_y);
            
			WM_MoveChildTo(WM_GetDialogItem(hDlg, GUI_ID_BUTTON1), g_choose_btn1_x, g_choose_btn1_y); 
		}
		else if(focus_curve == 2) {		
            hItem = WM_GetDialogItem(hDlg, GUI_ID_CURVE2);
			WM_SetSize(hItem, g_curve_x_size, g_curve_y_size);
			WM_MoveChildTo(hItem, g_curve2_x, g_curve2_y);
            
			WM_MoveChildTo(WM_GetDialogItem(hDlg, GUI_ID_BUTTON2), g_choose_btn2_x, g_choose_btn2_y);
		}
        else {
            hItem = WM_GetDialogItem(hDlg, GUI_ID_CURVE0);
        }

		CurveInitSetup(hItem, 'F', MultiArray[0], 'T');
		TracingCurve_SetXYINFO(hItem, 'T', 0, nXMax, 1, GUI_WHITE, Sys_Lib_GetDispStr(DP_S), -1 ,5, 1, GUI_WHITE, 
			Sys_Lib_GetDispStr(DP_CMH2O), GUI_WHITE, Sys_Lib_GetDispStr(DP_PAW));

        break;

#ifdef AA_WAVE_SWITCH
    case WAVEFORM_N2O:
		if(Menu_hWin.cWin.LoopFlag1 == Menu_hWin.cWin.FocusCurve || Menu_hWin.cWin.FocusCurve == 3) {		
            Menu_hWin.cWin.LoopFlag1 = 0;
            
            WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_CURVE3)); // 隐藏第三道LOOP 
            WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON3)); // 波形选择按钮
		}
        
        hItem = WM_GetDialogItem(hDlg, GUI_ID_CURVE1);
		if(focus_curve == 1) {		
			WM_SetSize(hItem, g_curve_x_size, g_curve_y_size);
			WM_MoveChildTo(hItem, g_curve1_x, g_curve1_y);
            
			WM_MoveChildTo(WM_GetDialogItem(hDlg, GUI_ID_BUTTON1), g_choose_btn1_x, g_choose_btn1_y); 
		}
		else if(focus_curve == 2) {		
            hItem = WM_GetDialogItem(hDlg, GUI_ID_CURVE2);
			WM_SetSize(hItem, g_curve_x_size, g_curve_y_size);
			WM_MoveChildTo(hItem, g_curve2_x, g_curve2_y);
            
			WM_MoveChildTo(WM_GetDialogItem(hDlg, GUI_ID_BUTTON2), g_choose_btn2_x, g_choose_btn2_y);
		}	

		CurveInitSetup(hItem, 'F', 1, 'T');
		TracingCurve_SetXYINFO(hItem,'T',0,nXMax,1,GUI_WHITE,Sys_Lib_GetDispStr(DP_S),0,100,20,
			GUI_WHITE,"%",GUI_WHITE,Sys_Lib_GetDispStr(DP_N2O));

		break;

    case WAVEFORM_PAA:   
		if(Menu_hWin.cWin.LoopFlag1 == Menu_hWin.cWin.FocusCurve || Menu_hWin.cWin.FocusCurve == 3) {		
            Menu_hWin.cWin.LoopFlag1 = 0;
            
            WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_CURVE3)); // 隐藏第三道LOOP 
            WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON3)); // 波形选择按钮
		}
        
        hItem = WM_GetDialogItem(hDlg, GUI_ID_CURVE1);
		if(focus_curve == 1) {		
			WM_SetSize(hItem, g_curve_x_size, g_curve_y_size);
			WM_MoveChildTo(hItem, g_curve1_x, g_curve1_y);
            
			WM_MoveChildTo(WM_GetDialogItem(hDlg, GUI_ID_BUTTON1), g_choose_btn1_x, g_choose_btn1_y); 
		}
		else if(focus_curve == 2) {		
            hItem = WM_GetDialogItem(hDlg, GUI_ID_CURVE2);
			WM_SetSize(hItem, g_curve_x_size, g_curve_y_size);
			WM_MoveChildTo(hItem, g_curve2_x, g_curve2_y);
            
			WM_MoveChildTo(WM_GetDialogItem(hDlg, GUI_ID_BUTTON2), g_choose_btn2_x, g_choose_btn2_y);
		}	

		CurveInitSetup(hItem, 'F', MultiAA[0], 'T');
		TracingCurve_SetXYINFO(hItem,'T',0,nXMax,1,GUI_WHITE,
			Sys_Lib_GetDispStr(DP_S),0,5,1,GUI_WHITE,"%",GUI_WHITE,AA_all_data.AA_detail.AA1_Type_Text);

        break;

    case WAVEFORM_SAA:
		if(Menu_hWin.cWin.LoopFlag1 == Menu_hWin.cWin.FocusCurve || Menu_hWin.cWin.FocusCurve == 3) {		
            Menu_hWin.cWin.LoopFlag1 = 0;
            
            WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_CURVE3)); // 隐藏第三道LOOP 
            WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON3)); // 波形选择按钮
		}
        
        hItem = WM_GetDialogItem(hDlg, GUI_ID_CURVE1);
		if(focus_curve == 1) {		
			WM_SetSize(hItem, g_curve_x_size, g_curve_y_size);
			WM_MoveChildTo(hItem, g_curve1_x, g_curve1_y);
            
			WM_MoveChildTo(WM_GetDialogItem(hDlg, GUI_ID_BUTTON1), g_choose_btn1_x, g_choose_btn1_y); 
		}
		else if(focus_curve == 2) {		
            hItem = WM_GetDialogItem(hDlg, GUI_ID_CURVE2);
			WM_SetSize(hItem, g_curve_x_size, g_curve_y_size);
			WM_MoveChildTo(hItem, g_curve2_x, g_curve2_y);
            
			WM_MoveChildTo(WM_GetDialogItem(hDlg, GUI_ID_BUTTON2), g_choose_btn2_x, g_choose_btn2_y);
		}	

		CurveInitSetup(hItem, 'F', MultiAA[1], 'T');
		TracingCurve_SetXYINFO(hItem,'T',0,nXMax,1,GUI_WHITE,\
			Sys_Lib_GetDispStr(DP_S),0,5,1,GUI_WHITE,"%",GUI_WHITE,AA_all_data.AA_detail.AA2_Type_Text);

        break;
#endif

    case WAVEFORM_FLOW:
		if(Menu_hWin.cWin.LoopFlag1 == Menu_hWin.cWin.FocusCurve || Menu_hWin.cWin.FocusCurve == 3) {		
            Menu_hWin.cWin.LoopFlag1 = 0;
            
            WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_CURVE3)); // 隐藏第三道LOOP 
            WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON3)); // 波形选择按钮
		}
        
        hItem = WM_GetDialogItem(hDlg, GUI_ID_CURVE1);
		if(focus_curve == 1) {		
			WM_SetSize(hItem, g_curve_x_size, g_curve_y_size);
			WM_MoveChildTo(hItem, g_curve1_x, g_curve1_y);
            
			WM_MoveChildTo(WM_GetDialogItem(hDlg, GUI_ID_BUTTON1), g_choose_btn1_x, g_choose_btn1_y); 
		}
		else if(focus_curve == 2) {		
            hItem = WM_GetDialogItem(hDlg, GUI_ID_CURVE2);
			WM_SetSize(hItem, g_curve_x_size, g_curve_y_size);
			WM_MoveChildTo(hItem, g_curve2_x, g_curve2_y);
            
			WM_MoveChildTo(WM_GetDialogItem(hDlg, GUI_ID_BUTTON2), g_choose_btn2_x, g_choose_btn2_y);
		}

		CurveInitSetup(hItem, 'F', MultiArray[1], 'T');
		TracingCurve_SetXYINFO(hItem,'T',0,nXMax,1,GUI_WHITE,
			Sys_Lib_GetDispStr(DP_S),-3,3,1,GUI_WHITE,"LPM",GUI_WHITE,Sys_Lib_GetDispStr(DP_FLOW));

        break;

    case WAVEFORM_VOLUME:
		if(Menu_hWin.cWin.LoopFlag1 == Menu_hWin.cWin.FocusCurve || Menu_hWin.cWin.FocusCurve == 3) {		
            Menu_hWin.cWin.LoopFlag1 = 0;
            
            WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_CURVE3)); // 隐藏第三道LOOP 
            WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON3)); // 波形选择按钮
		}
        
        hItem = WM_GetDialogItem(hDlg, GUI_ID_CURVE1);
		if(focus_curve == 1) {		
			WM_SetSize(hItem, g_curve_x_size, g_curve_y_size);
			WM_MoveChildTo(hItem, g_curve1_x, g_curve1_y);
            
			WM_MoveChildTo(WM_GetDialogItem(hDlg, GUI_ID_BUTTON1), g_choose_btn1_x, g_choose_btn1_y); 
		}
		else if(focus_curve == 2) {		
            hItem = WM_GetDialogItem(hDlg, GUI_ID_CURVE2);
			WM_SetSize(hItem, g_curve_x_size, g_curve_y_size);
			WM_MoveChildTo(hItem, g_curve2_x, g_curve2_y);
            
			WM_MoveChildTo(WM_GetDialogItem(hDlg, GUI_ID_BUTTON2), g_choose_btn2_x, g_choose_btn2_y);
		}

		CurveInitSetup(hItem, 'F', MultiArray[2], 'T');

		TracingCurve_SetXYINFO(hItem,'T',0,nXMax,1,GUI_WHITE,
			Sys_Lib_GetDispStr(DP_S),0,30,5,GUI_WHITE,"mL",GUI_WHITE,Sys_Lib_GetDispStr(DP_VOLUME));

        break;

#ifdef SPO2_SWITCH
    case WAVEFORM_SPO2://心率
		if(Menu_hWin.cWin.LoopFlag1 == Menu_hWin.cWin.FocusCurve || Menu_hWin.cWin.FocusCurve == 3) {		
            Menu_hWin.cWin.LoopFlag1 = 0;
            
            WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_CURVE3)); // 隐藏第三道LOOP 
            WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON3)); // 波形选择按钮
		}
        
        hItem = WM_GetDialogItem(hDlg, GUI_ID_CURVE1);
		if(focus_curve == 1) {		
			WM_SetSize(hItem, g_curve_x_size, g_curve_y_size);
			WM_MoveChildTo(hItem, g_curve1_x, g_curve1_y);
            
			WM_MoveChildTo(WM_GetDialogItem(hDlg, GUI_ID_BUTTON1), g_choose_btn1_x, g_choose_btn1_y); 
		}
		else if(focus_curve == 2) {		
            hItem = WM_GetDialogItem(hDlg, GUI_ID_CURVE2);
			WM_SetSize(hItem, g_curve_x_size, g_curve_y_size);
			WM_MoveChildTo(hItem, g_curve2_x, g_curve2_y);
            
			WM_MoveChildTo(WM_GetDialogItem(hDlg, GUI_ID_BUTTON2), g_choose_btn2_x, g_choose_btn2_y);
		}

		TracingCurve_SetXScale(hItem, 1);
		TracingCurve_SetViewMode(hItem, 0, 'F');

        if(Masimo_SpO2_Config_On)
		{
			TracingCurve_SetRange(hItem,20);
			TracingCurve_SetXYINFO(hItem,'T',0,nXMax,1,GUI_WHITE,Sys_Lib_GetDispStr(DP_S),-5,5,1,GUI_WHITE,"",GUI_WHITE, Sys_Lib_GetDispStr(DP_PLETH));
		}
		else
		{
			TracingCurve_SetRange(hItem,1);
			TracingCurve_SetXYINFO(hItem,'T',0,nXMax,1,GUI_WHITE,Sys_Lib_GetDispStr(DP_S),0,100,20,GUI_WHITE,"",GUI_WHITE, Sys_Lib_GetDispStr(DP_PLETH));                 
			TracingCurve_SetYsign(hItem,'F');
		}
        TracingCurve_SetExceedRangeFlg(hItem,0);

        break;

#endif
    case WAVEFORM_CO2:
		if(Menu_hWin.cWin.LoopFlag1 == Menu_hWin.cWin.FocusCurve || Menu_hWin.cWin.FocusCurve == 3) {		
            Menu_hWin.cWin.LoopFlag1 = 0;
            
            WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_CURVE3)); // 隐藏第三道LOOP 
            WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON3)); // 波形选择按钮
		}
        
        hItem = WM_GetDialogItem(hDlg, GUI_ID_CURVE1);
		if(focus_curve == 1) {		
			WM_SetSize(hItem, g_curve_x_size, g_curve_y_size);
			WM_MoveChildTo(hItem, g_curve1_x, g_curve1_y);
            
			WM_MoveChildTo(WM_GetDialogItem(hDlg, GUI_ID_BUTTON1), g_choose_btn1_x, g_choose_btn1_y); 
		}
		else if(focus_curve == 2) {		
            hItem = WM_GetDialogItem(hDlg, GUI_ID_CURVE2);
			WM_SetSize(hItem, g_curve_x_size, g_curve_y_size);
			WM_MoveChildTo(hItem, g_curve2_x, g_curve2_y);
            
			WM_MoveChildTo(WM_GetDialogItem(hDlg, GUI_ID_BUTTON2), g_choose_btn2_x, g_choose_btn2_y);
		}
		
		CurveInitSetup(hItem, 'F', 1, 'T');
		TracingCurve_SetXYINFO(hItem,'T',0,nXMax,1,GUI_WHITE,
			Sys_Lib_GetDispStr(DP_S),0,CO2_Disp.co2max,CO2_Disp.co2step,GUI_WHITE,CO2_Disp.co2_unit,GUI_WHITE,Sys_Lib_GetDispStr(DP_CO2));

        break;

	case LOOP_V_CO2:
		if(Menu_hWin.cWin.LoopFlag1 == Menu_hWin.cWin.FocusCurve || Menu_hWin.cWin.FocusCurve == 3) {		
            Menu_hWin.cWin.LoopFlag1 = 0;
            
            WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_CURVE3)); // 隐藏第三道LOOP 
            WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON3)); // 波形选择按钮
		}
        
        hItem = WM_GetDialogItem(hDlg, GUI_ID_CURVE1);
		if(focus_curve == 1) {		
			WM_SetSize(hItem, g_curve_x_size, g_curve_y_size);
			WM_MoveChildTo(hItem, g_curve1_x, g_curve1_y);
            
			WM_MoveChildTo(WM_GetDialogItem(hDlg, GUI_ID_BUTTON1), g_choose_btn1_x, g_choose_btn1_y); 
		}
		else if(focus_curve == 2) {		
            hItem = WM_GetDialogItem(hDlg, GUI_ID_CURVE2);
			WM_SetSize(hItem, g_curve_x_size, g_curve_y_size);
			WM_MoveChildTo(hItem, g_curve2_x, g_curve2_y);
            
			WM_MoveChildTo(WM_GetDialogItem(hDlg, GUI_ID_BUTTON2), g_choose_btn2_x, g_choose_btn2_y);
		}

		CurveInitSetup(hItem, 'F', 1, 'T');
		TracingCurve_SetXYINFO(hItem,'T',0,30,5,GUI_WHITE,
			Sys_Lib_GetDispStr(DP_ML),0,CO2_Disp.co2max,CO2_Disp.co2step,GUI_WHITE,CO2_Disp.co2_unit,GUI_WHITE,Sys_Lib_GetDispStr(DP_VCO2));

		break;

    case LOOP_PV:
        hItem = WM_GetDialogItem(hDlg, GUI_ID_CURVE1);
		if(focus_curve == 1) {		
			WM_MoveChildTo(hItem, g_curve1_x1, g_curve1_y);
			WM_MoveChildTo(WM_GetDialogItem(hDlg, GUI_ID_BUTTON1), g_choose_btn1_x1, g_choose_btn1_y);
		}
		else if(focus_curve == 2){		
            hItem = WM_GetDialogItem(hDlg, GUI_ID_CURVE2);
			WM_MoveChildTo(hItem, g_curve1_x1, g_curve2_y);
			WM_MoveChildTo(WM_GetDialogItem(hDlg, GUI_ID_BUTTON2), g_choose_btn2_x1, g_choose_btn2_y);
		}		
		else if(focus_curve == 3)
		{
            hItem = WM_GetDialogItem(hDlg, GUI_ID_CURVE3);
			WM_ShowWindow(hItem);
			WM_ShowWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON3));
			if(Menu_hWin.cWin.LoopFlag1 == 1)
			{
				WM_MoveChildTo(hItem, g_curve3_x, g_curve1_y);
				WM_MoveChildTo(WM_GetDialogItem(hDlg, GUI_ID_BUTTON3), g_choose_btn3_x, g_choose_btn3_y); 
				WM_MoveChildTo(WM_GetDialogItem(hDlg, GUI_ID_BUTTON1), g_choose_btn1_x1, g_choose_btn1_y);
			}
			else if(Menu_hWin.cWin.LoopFlag1 == 2)
			{
				WM_MoveChildTo(hItem, g_curve3_x, g_curve2_y);
				WM_MoveChildTo(WM_GetDialogItem(hDlg, GUI_ID_BUTTON3), g_choose_btn3_x, g_choose_btn3_y1);
				WM_MoveChildTo(WM_GetDialogItem(hDlg, GUI_ID_BUTTON2), g_choose_btn2_x1, g_choose_btn2_y);
			}
		}	       

		WM_SetSize(hItem, g_loop_x_size, g_loop_y_size);
		CurveInitSetup(hItem, 'T', MultiArray[2], 'T');
		TracingCurve_SetRangeX(hItem, MultiArray[0]);
		TracingCurve_SetXYINFO(hItem,'T',-1,5,1,GUI_WHITE,Sys_Lib_GetDispStr(DP_CMH2O),\
			0,30,5,GUI_WHITE,Sys_Lib_GetDispStr(DP_ML),GUI_WHITE,"P-V");

		break;

	case LOOP_VF:
        hItem = WM_GetDialogItem(hDlg, GUI_ID_CURVE1);
		if(focus_curve == 1) {		
			WM_MoveChildTo(hItem, g_curve1_x1, g_curve1_y);
			WM_MoveChildTo(WM_GetDialogItem(hDlg, GUI_ID_BUTTON1), g_choose_btn1_x1, g_choose_btn1_y);
		}
		else if(focus_curve == 2){		
            hItem = WM_GetDialogItem(hDlg, GUI_ID_CURVE2);
			WM_MoveChildTo(hItem, g_curve1_x1, g_curve2_y);
			WM_MoveChildTo(WM_GetDialogItem(hDlg, GUI_ID_BUTTON2), g_choose_btn2_x1, g_choose_btn2_y);
		}		
		else if(focus_curve == 3)
		{
            hItem = WM_GetDialogItem(hDlg, GUI_ID_CURVE3);
			WM_ShowWindow(hItem);
			WM_ShowWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON3));
			if(Menu_hWin.cWin.LoopFlag1 == 1)
			{
				WM_MoveChildTo(hItem, g_curve3_x, g_curve1_y);
				WM_MoveChildTo(WM_GetDialogItem(hDlg, GUI_ID_BUTTON3), g_choose_btn3_x, g_choose_btn3_y); 
				WM_MoveChildTo(WM_GetDialogItem(hDlg, GUI_ID_BUTTON1), g_choose_btn1_x1, g_choose_btn1_y);
			}
			else if(Menu_hWin.cWin.LoopFlag1 == 2)
			{
				WM_MoveChildTo(hItem, g_curve3_x, g_curve2_y);
				WM_MoveChildTo(WM_GetDialogItem(hDlg, GUI_ID_BUTTON3), g_choose_btn3_x, g_choose_btn3_y1);
				WM_MoveChildTo(WM_GetDialogItem(hDlg, GUI_ID_BUTTON2), g_choose_btn2_x1, g_choose_btn2_y);
			}
		}

		WM_SetSize(hItem, g_loop_x_size, g_loop_y_size);
		CurveInitSetup(hItem, 'T', MultiArray[1], 'T');
		TracingCurve_SetRangeX(hItem, MultiArray[2]);
		TracingCurve_SetXYINFO(hItem,'T',0,30,5,GUI_WHITE,Sys_Lib_GetDispStr(DP_ML),
			-3,3,1,GUI_WHITE,Sys_Lib_GetDispStr(DP_LPM),GUI_WHITE,"V-F");

		break;

	case LOOP_PF:
        hItem = WM_GetDialogItem(hDlg, GUI_ID_CURVE1);
		if(focus_curve == 1) {		
			WM_MoveChildTo(hItem, g_curve1_x1, g_curve1_y);
			WM_MoveChildTo(WM_GetDialogItem(hDlg, GUI_ID_BUTTON1), g_choose_btn1_x1, g_choose_btn1_y);
		}
		else if(focus_curve == 2){		
            hItem = WM_GetDialogItem(hDlg, GUI_ID_CURVE2);
			WM_MoveChildTo(hItem, g_curve1_x1, g_curve2_y);
			WM_MoveChildTo(WM_GetDialogItem(hDlg, GUI_ID_BUTTON2), g_choose_btn2_x1, g_choose_btn2_y);
		}		
		else if(focus_curve == 3)
		{
            hItem = WM_GetDialogItem(hDlg, GUI_ID_CURVE3);
			WM_ShowWindow(hItem);
			WM_ShowWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON3));
			if(Menu_hWin.cWin.LoopFlag1 == 1)
			{
				WM_MoveChildTo(hItem, g_curve3_x, g_curve1_y);
				WM_MoveChildTo(WM_GetDialogItem(hDlg, GUI_ID_BUTTON3), g_choose_btn3_x, g_choose_btn3_y); 
				WM_MoveChildTo(WM_GetDialogItem(hDlg, GUI_ID_BUTTON1), g_choose_btn1_x1, g_choose_btn1_y);
			}
			else if(Menu_hWin.cWin.LoopFlag1 == 2)
			{
				WM_MoveChildTo(hItem, g_curve3_x, g_curve2_y);
				WM_MoveChildTo(WM_GetDialogItem(hDlg, GUI_ID_BUTTON3), g_choose_btn3_x, g_choose_btn3_y1);
				WM_MoveChildTo(WM_GetDialogItem(hDlg, GUI_ID_BUTTON2), g_choose_btn2_x1, g_choose_btn2_y);
			}
		}

		WM_SetSize(hItem, g_loop_x_size, g_loop_y_size);
		CurveInitSetup(hItem, 'T', MultiArray[1], 'T');
		TracingCurve_SetRangeX(hItem, MultiArray[0]);
		TracingCurve_SetXYINFO(hItem, 'T', -1, 5, 1, GUI_WHITE, Sys_Lib_GetDispStr(DP_CMH2O), -3, 3, 1, GUI_WHITE,
			Sys_Lib_GetDispStr(DP_LMIN), GUI_WHITE, "P-F");

		break;
    }
}

/***************************************************************************************************
* @brief  wave select menu 波形显示选择函数
* @para   
* @retval 
*/

static void _cbDialog15(WM_MESSAGE * pMsg)
{
    WM_HWIN hDlg,hWin;
    int Id,NCode,Menu_Id;
    int NumItems,Flags;
	static unsigned char init_flag = 1;
    MENU_ITEM_DATA pData0;
    hDlg = pMsg->hWin;
    switch (pMsg->MsgId)
    {
    case WM_INIT_DIALOG:
        WINDOW_SetBkColor(hDlg,GUI_YAN_BLUE);
        hWin = WM_GetDialogItem(hDlg, GUI_ID_MENU0);
        MENU_SetVerticalFlg(hWin,1); //  20130514
        
		{
			pData0.Flags = 0;//MENU_IF_SEPARATOR;
			pData0.hSubmenu = 0;
			pData0.Id = WAVEFORM_PRESSURE;
			pData0.pText = Sys_Lib_GetDispStr(DP_PAW);
			MENU_AddItem(hWin,&pData0);

			pData0.Flags = 0;//MENU_IF_SEPARATOR;
			pData0.hSubmenu = 0;
			pData0.Id = WAVEFORM_FLOW;
			pData0.pText = Sys_Lib_GetDispStr(DP_FLOW);    
			MENU_AddItem(hWin,&pData0);

			pData0.Flags = 0;//MENU_IF_SEPARATOR;
			pData0.hSubmenu = 0;
			pData0.Id = WAVEFORM_VOLUME;
			pData0.pText = Sys_Lib_GetDispStr(DP_VOLUME);
			MENU_AddItem(hWin,&pData0);

#ifdef SPO2_SWITCH
			pData0.Flags = 0;//MENU_IF_SEPARATOR;
			pData0.hSubmenu = 0;
			pData0.Id = WAVEFORM_SPO2;
			pData0.pText = Sys_Lib_GetDispStr(DP_PLETH);
			MENU_AddItem(hWin,&pData0);

			if(!SpO2ModuleExist)
			{
				MENU_DisableItem(hWin,WAVEFORM_SPO2);
			}
			else //  20130418
			{
				if(SpO2ModuleOff)
				{
					MENU_DisableItem(hWin,WAVEFORM_SPO2);
				}
			}
#ifdef LANGUAGE_CHINESE_FLAG
			MENU_DisableItem(hWin,WAVEFORM_SPO2);
#endif   

#endif
			pData0.Flags = 0;//MENU_IF_SEPARATOR;
			pData0.hSubmenu = 0;
			pData0.Id = WAVEFORM_CO2;
			pData0.pText = Sys_Lib_GetDispStr(DP_CO2);
			MENU_AddItem(hWin,&pData0);
			if(!AA_Module_Flag.EtCO2ModuleExist)
			{
				MENU_DisableItem(hWin,WAVEFORM_CO2);
			}
			else //  20130418
			{
				if(!EtCO2ModuleOff)
				{
					MENU_DisableItem(hWin,WAVEFORM_CO2);
				}
			}        

#ifdef AA_WAVE_SWITCH
			pData0.Flags = 0;
			pData0.hSubmenu = 0;
			pData0.Id = WAVEFORM_N2O;
			pData0.pText = Sys_Lib_GetDispStr(DP_N2O);
			MENU_AddItem(hWin,&pData0);
			//      if(((AA_Module_Flag.EtCO2ModuleExist&0x06) != 0)&&(EtCO2ModuleOff) /*&& (AA_all_data.AA_state.dev_info.module_type & 4)*/ )
			if((EtCO2ModuleOff) && (((AA_Module_Flag.PhaseInTypeModuleExist == 1)&&(Masimo_EtCO2_Config_On)) \
				|| (MasimoISA_EtCO2_Config_On) || (Drager_EtCO2_Config_On)))
			{
				MENU_EnableItem(hWin,WAVEFORM_N2O);
			}
			else
			{
				MENU_DisableItem(hWin,WAVEFORM_N2O);
			}

			pData0.Flags = 0;
			pData0.hSubmenu = 0;
			pData0.Id = WAVEFORM_PAA;
			pData0.pText = Sys_Lib_GetDispStr(DP_AA1);
			MENU_AddItem(hWin,&pData0);
			// 		if(((EtCO2ModuleExist&0x06) != 0)&&(EtCO2ModuleOff) && (pAll_Data->slow.VerVal.sen_type & 4) )
			if((EtCO2ModuleOff) && (((AA_Module_Flag.PhaseInTypeModuleExist == 1)&&(Masimo_EtCO2_Config_On)) \
				|| (MasimoISA_EtCO2_Config_On) || (Drager_EtCO2_Config_On)))
			{
				MENU_EnableItem(hWin,WAVEFORM_PAA);
			}
			else
			{
				MENU_DisableItem(hWin,WAVEFORM_PAA);
			}
			pData0.Flags = 0;
			pData0.hSubmenu = 0;
			pData0.Id = WAVEFORM_SAA;
			pData0.pText = Sys_Lib_GetDispStr(DP_AA2);
			MENU_AddItem(hWin,&pData0);
			// 		if(((EtCO2ModuleExist&0x06) != 0)&&(EtCO2ModuleOff) && (pAll_Data->slow.VerVal.sen_type & 4) )
			if((EtCO2ModuleOff) && (((AA_Module_Flag.PhaseInTypeModuleExist == 1)&&(Masimo_EtCO2_Config_On)) || (MasimoISA_EtCO2_Config_On)))
			{
				MENU_EnableItem(hWin,WAVEFORM_SAA);
			}
			else
			{
				MENU_DisableItem(hWin,WAVEFORM_SAA);
			}
#endif

			pData0.Flags = 0;
			pData0.hSubmenu = 0;
			pData0.Id = LOOP_V_CO2;
			pData0.pText = Sys_Lib_GetDispStr(DP_VCO2);
			MENU_AddItem(hWin,&pData0);
			if ((!AA_Module_Flag.EtCO2ModuleExist)||!EtCO2ModuleOff /*||(Masimo_EtCO2_Config_On != 1)*/)
			{
				MENU_DisableItem(hWin,LOOP_V_CO2);
			}
			else
			{
				MENU_EnableItem(hWin,LOOP_V_CO2);
			}

			pData0.Flags = 0;
			pData0.hSubmenu = 0;
			pData0.Id = WAVEFORM_LOOP;
			pData0.pText = Sys_Lib_GetDispStr(DP_LOOP);
			MENU_AddItem(hWin,&pData0);
		}

		if(Menu_hWin.cWin.FocusCurve == 1) // 第三行是换图，点击Btn1
		{
			if(Menu_hWin.cWin.LoopFlag1 == 2 || Menu_hWin.cWin.LoopFlag1 == 0)
			{
				MENU_DeleteItem(hWin, LOOP_PV);
				MENU_DeleteItem(hWin, LOOP_VF);
				MENU_DeleteItem(hWin, LOOP_PF);
                if(Menu_hWin.cWin.LoopFlag1 == 0)
                {
                    MENU_EnableItem(hWin, WAVEFORM_LOOP);
                }
                else
                {
                    MENU_DisableItem(hWin, WAVEFORM_LOOP);
                }
			}
			else
			{
				// P-V
				pData0.Flags = 0;
				pData0.hSubmenu = 0;
				pData0.Id = LOOP_PV;
				pData0.pText = "P-V";
				MENU_AddItem(hWin,&pData0);
				// V-F
				//pData0.Flags = 0;
				//pData0.hSubmenu = 0;
				pData0.Id = LOOP_VF;
				pData0.pText = "V-F";
				MENU_AddItem(hWin,&pData0);
				// P-F
				//pData0.Flags = 0;
				//pData0.hSubmenu = 0;
				pData0.Id = LOOP_PF;
				pData0.pText = "P-F";
				MENU_AddItem(hWin,&pData0);
				//
				MENU_DisableItem(hWin, WAVEFORM_LOOP);
			}
		}
		else if(Menu_hWin.cWin.FocusCurve == 2) // 第二行是换图，点击Btn2
		{
			if(Menu_hWin.cWin.LoopFlag1 == 1 || Menu_hWin.cWin.LoopFlag1 == 0)
			{
				MENU_DeleteItem(hWin, LOOP_PV);
				MENU_DeleteItem(hWin, LOOP_VF);
				MENU_DeleteItem(hWin, LOOP_PF);
                if(Menu_hWin.cWin.LoopFlag1 == 0)
                {
                    MENU_EnableItem(hWin, WAVEFORM_LOOP);
                }
                else
                {
                    MENU_DisableItem(hWin, WAVEFORM_LOOP);
                }
			}
			else
			{
				// P-V
				pData0.Flags = 0;
				pData0.hSubmenu = 0;
				pData0.Id = LOOP_PV;
				pData0.pText = "P-V";
				MENU_AddItem(hWin,&pData0);
				// V-F
				//pData0.Flags = 0;
				//pData0.hSubmenu = 0;
				pData0.Id = LOOP_VF;
				pData0.pText = "V-F";
				MENU_AddItem(hWin,&pData0);
				// P-F
				//pData0.Flags = 0;
				//pData0.hSubmenu = 0;
				pData0.Id = LOOP_PF;
				pData0.pText = "P-F";
				MENU_AddItem(hWin,&pData0);
				//
				MENU_DisableItem(hWin, WAVEFORM_LOOP);
			}
		}
		else if(Menu_hWin.cWin.FocusCurve == 3)
		{
			// P-V
			pData0.Flags = 0;
			pData0.hSubmenu = 0;
			pData0.Id = LOOP_PV;
			pData0.pText = "P-V";
			MENU_AddItem(hWin,&pData0);
			// V-F
			//pData0.Flags = 0;
			//pData0.hSubmenu = 0;
			pData0.Id = LOOP_VF;
			pData0.pText = "V-F";
			MENU_AddItem(hWin,&pData0);
			// P-F
			//pData0.Flags = 0;
			//pData0.hSubmenu = 0;
			pData0.Id = LOOP_PF;
			pData0.pText = "P-F";
			MENU_AddItem(hWin,&pData0);
			//
			MENU_DisableItem(hWin, WAVEFORM_LOOP);
		}


		// MENU_SetBkColor(hWin,MENU_CI_ENABLED,0xD05356);
        MENU_SetBkColor(hWin,MENU_CI_SELECTED,0x4080FF);
        break;
    case WM_NOTIFY_PARENT:
        NCode = pMsg->Data.v;
        //Id = WM_GetId(pMsg->hWinSrc);
        switch(NCode)
        {
        case WM_NOTIFICATION_CLICKED:
			hWin = WM_GetDialogItem(hDlg, GUI_ID_MENU0);//sun add 20140521			
			Id = MENU_GetSel(pMsg->hWinSrc);
        
//			if(Id <= WAVEFORM_LOOP)
//			{
//				if(s_curve_menu_flag == 0)
//					s_curve_menu_flag = 1;
//				else
//					s_curve_menu_flag = 0;
//			}

			if(Id == WAVEFORM_LOOP)
			{
                MENU_DisableItem(hWin, WAVEFORM_LOOP);
                //Menu_hWin.cWin.LoopFlag2 = Menu_hWin.cWin.FocusCurve;
			}
            else if(Id < WAVEFORM_LOOP)
            {
                MENU_EnableItem(hWin, WAVEFORM_LOOP);
            }
			
//			BUTTON_SetPressed(WM_GetDialogItem(Menu_hWin.HomeWin.ParentWin, GUI_ID_BUTTON1), 0);
//			BUTTON_SetPressed(WM_GetDialogItem(Menu_hWin.HomeWin.ParentWin, GUI_ID_BUTTON2), 0);
//			BUTTON_SetPressed(WM_GetDialogItem(Menu_hWin.HomeWin.ParentWin, GUI_ID_BUTTON3), 0);

            if(Id!=Menu_hWin.cWin.WaveForms[Menu_hWin.cWin.FocusCurve])
            { 
                Menu_hWin.cWin.WaveForms[Menu_hWin.cWin.FocusCurve] = Id;

                WaveFromMenu(Id);

				WaveFormsId2 = Menu_hWin.cWin.WaveForms[2];
				RTEEPROMwrite(WAVEFORMSID2_ADDR, WaveFormsId2);

            }
            TracingCurve_SetXScale(Menu_hWin.HomeWin.Tobj[0],1);
            TracingCurve_SetXScale(Menu_hWin.HomeWin.Tobj[1],1);
            TracingCurve_SetXScale(Menu_hWin.HomeWin.Tobj[2],1);
			TracingCurve_SetXScale(Menu_hWin.HomeWin.Tobj[3],1);
            PlanMenuType = MAIN_MENU;
            Task |= MEAU_UPDATE;
            break;
        //endof case WM_NOTIFICATION_CLICKED
        //zkq add 20170119  旋钮可以进行波形选择
        case GUI_KEY_LEFT:
             hWin = WM_GetDialogItem(hDlg, GUI_ID_MENU0);
             NumItems = MENU_GetNumItems(hWin);
             Id = MENU_GetSel(hWin);

            if(Id > 0 && Id <= NumItems-1)
            {
                MENU_SetSel(hWin,Id-1);
            }
            else if(Id == 0)
            {
                MENU_SetSel(hWin,NumItems-1);
            }
            break;
        case GUI_KEY_RIGHT:
            hWin = WM_GetDialogItem(hDlg, GUI_ID_MENU0);
            NumItems = MENU_GetNumItems(hWin);
            Id = MENU_GetSel(hWin);
            if(Id >= 0 && Id <= NumItems-2)
            {
                MENU_SetSel(hWin,Id+1);
            }
            else if(Id == NumItems-1)
            {
                MENU_SetSel(hWin,0);
            }
            break;
        case GUI_KEY_ENTER:
            hWin = WM_GetDialogItem(hDlg, GUI_ID_MENU0);
            Id = MENU_GetSel(hWin);
            Flags = MENU_GetItemFlags(hWin,Id);

            if(Flags != 1)
            {
                if(Id == WAVEFORM_LOOP)
                {
                    MENU_DisableItem(hWin, WAVEFORM_LOOP);
                    //Menu_hWin.cWin.LoopFlag2 = Menu_hWin.cWin.FocusCurve;
                }
                else if(Id < WAVEFORM_LOOP)
                {
                    MENU_EnableItem(hWin, WAVEFORM_LOOP);
                }                             

                if(Id!=Menu_hWin.cWin.WaveForms[Menu_hWin.cWin.FocusCurve])
                { 
                    Menu_hWin.cWin.WaveForms[Menu_hWin.cWin.FocusCurve] = Id;

                    WaveFromMenu(Id);

    				WaveFormsId2 = Menu_hWin.cWin.WaveForms[2];
    				RTEEPROMwrite(WAVEFORMSID2_ADDR, WaveFormsId2);
                }                
                TracingCurve_SetXScale(Menu_hWin.HomeWin.Tobj[0],1);
                TracingCurve_SetXScale(Menu_hWin.HomeWin.Tobj[1],1);
                TracingCurve_SetXScale(Menu_hWin.HomeWin.Tobj[2],1);
				TracingCurve_SetXScale(Menu_hWin.HomeWin.Tobj[3],1);
                PlanMenuType = MAIN_MENU;
                Task |= MEAU_UPDATE;
            }
            break;
        }
        break;
    default:
        WM_DefaultProc(pMsg);
    }
}
/***************************************************************************************************
* @brief  activate standby win
* @para   
* @retval 
*/
static void _cbDialog16(WM_MESSAGE * pMsg)
{
	WM_HWIN hDlg,hItem0,hItem1,hItem2,hItem3, hItem4,hItem5/*hLine0*/;
	int Id,NCode;
	WIDGET* pWidget = WIDGET_H2P(pMsg->hWin);
	hDlg = pMsg->hWin;
	switch (pMsg->MsgId)
	{

	case WM_INIT_DIALOG:
		hItem0 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON0);
		BUTTON_SetBkColor(hItem0,0,0xD05356);
		BUTTON_SetText(hItem0,Sys_Lib_GetDispStr(DP_ACTIVATESTANDBY));
		hItem1 = WM_GetDialogItem(hDlg, GUI_ID_TEXT0);
		TEXT_SetTextAlign(hItem1,TEXT_CF_HCENTER|TEXT_CF_VCENTER);
		TEXT_SetText(hItem1,Sys_Lib_GetDispStr(DP_NOVENTDELIVER));

		hItem2 = WM_GetDialogItem(hDlg, GUI_ID_IMAGETEXT0);
		WM_ShowWindow(hItem2);
		IMAGETEXT_SetBitmap(hItem2,&bmEN_Standby2);
		WM_MoveTo(hItem2,254,200);
		IMAGETEXT_SetBkColor(hItem2,GUI_YAN_BLUE);

		hItem5 = WM_GetDialogItem(hDlg, GUI_ID_TEXT1);
		TEXT_SetBkColor(hItem5,GUI_BLACK);//画线
		hItem3 = WM_GetDialogItem(hDlg, GUI_ID_IMAGETEXT1);
		WM_HideWindow(hItem3);
		
		break;
	case WM_NOTIFY_PARENT:
		NCode = pMsg->Data.v;
		Id = WM_GetId(pMsg->hWinSrc);
		switch(NCode)
		{
		case WM_NOTIFICATION_CLICKED:
			switch(Id)
			{
			case GUI_ID_BUTTON0:
				WM_HideWindow(WM_GetDialogItem(Menu_hWin.hWin, GUI_ID_BUTTON8));
// 				if(WM_IsWindow(Menu_hWin.ToolsWin.CPBInfoWin))
// 				{
// 					WM_HideWindow(Menu_hWin.ToolsWin.CPBInfoWin);
// 				}

				hItem4 = WM_GetDialogItem(Menu_hWin.hWin, GUI_ID_BUTTON7);//sun add 20140519
				//BUTTON_SetBkColor(hItem4,0,0xFFD080);
				BUTTON_SetText(hItem4,Sys_Lib_GetDispStr(DP_START));
				BUTTON_SetPressed(hItem4, 0);
// 				if(MasimoISA_EtCO2_Config_On && (AA_Module_Flag.EtCO2ModuleExist&0x04) == 4)

				if( (Drager_EtCO2_Config_On && AA_all_data.AA_state.AA_ModuleState.pump_state == AA_pump_high)\
					|| (MasimoISA_EtCO2_Config_On && AA_Module_Flag.PhaseInTypeModuleExist == 2))
				{
					SendOperationMessage(SET_AA_PUMPOFF,USART2);//关泵
				}
				else if((AA_Module_Flag.EtCO2ModuleExist == 1) && (EtCO2_Config_On || EtCO2_KM7002_Config_On))
				{
					SendOperationMessage(CO2_MODULEOFF,USART2);
				}
				StandbyState=0xFF;
				BackUpMode = 0;
				Menu_hWin.InfoWin.InfoChangeFlag |= MODES_INFO_CHANGED_FLAG;
// 				Menu_hWin.InfoWin.InfoChangeFlag |= CPB_CHANGE_FLAG;
				PlanMenuType = STANDBY_MENU;
				Task |= MEAU_UPDATE;
				SendOperationMessage(ENTER_STANDBY,USART0);
				LedUpdateRegister |= STANDBY_LED;
				SendOperationMessage(LED_UPDATE_OPERATION,USART3);
				break;
			}
		}
		break;
	case WM_NOTIFY_PARENT_REFLECTION:
		BUTTON_SetPressed(WM_GetDialogItem(Menu_hWin.hWin, GUI_ID_BUTTON7), 0);
		//FunctionKeyPressedResume(Menu_hWin.hWin,0);
		PlanMenuType = MAIN_MENU;
		Task |= MEAU_UPDATE;
		break;
	case WM_GET_ID:
		pMsg->Data.v = pWidget->Id;
		break;
	default:
		WM_DefaultProc(pMsg);
		break;
	}
}

/***************************************************************************************************
* @brief  function menu button
* @para   
* @retval 
*/
#define FUN_BTN_BK_COLOR				0x292424//0x6b573a
#define FUN_BTN_DOWN_BK_COLOR			0xF8644E//0xe0e0e0//0x7049e5//0x7d6e5c//0x897a68
#define STANDBY_BTN_BK_COLOR			0xa1a545//0xc9cc84//0x79CA2A
#define BTN_BK_COLOR					0x1c1a1a//0x312e2d//0x282424//0x443723//0x6b573a
#define VENT_WIN_BK_COLOR				0x292424//GUI_BLACK//0x615740//0x6d624b//0x615740
#define VENT_BTN_BK_COLOR				0x1d1a19//0x1e1c1c//0x6b573a
#define VENT_BTN_DOWN_BK_COLOR			0xF8644E//0xe0e0e0//0x7049e5//0x9680F1//0xc2b9f3
#define VENT_BTN_LINE_COLOR				0x9A9A9A
#define MODE_BTN_BK_COLOR				0x1b1818//0x171313
#define DESKTOP_BK_COLOR				0x1b1818//0x171313//0x1d1a19//GUI_BLACK//0x171109

void TimerRectPaintParaInit(WM_HWIN hDlg)
{
	WM_HWIN hItem;
	char pText[20] = {0};
	int index = 0, sec = 0, min = 0, hour = 0;
	//计时器--start/stop
	hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT1);
	MultiText_SetsStyte(hItem,2);
	MultiText_SetWinBkColor(hItem,FUN_BTN_BK_COLOR);
	MultiText_SetBKColor(hItem,BTN_BK_COLOR/*STANDBY_BK_COLOR*/);
	MultiText_SetFrameColor(hItem,VENT_BTN_LINE_COLOR);
	if(GetTimerHandle())//判断句柄是否存在 只有点暂停，才会删除定时器句柄
	{
		MultiText_EX_SetBitmapEx(hItem, &bmIconStop, 23, 8);
	}
	else
	{
		MultiText_EX_SetBitmapEx(hItem, &bmIconStart, 21, 8);
	}
	//计时器--clear
	hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT2);
	MultiText_SetsStyte(hItem,2);
	MultiText_SetWinBkColor(hItem,FUN_BTN_BK_COLOR);
	MultiText_SetBKColor(hItem,BTN_BK_COLOR/*STANDBY_BK_COLOR*/);
	MultiText_SetFrameColor(hItem,VENT_BTN_LINE_COLOR);
	MultiText_EX_SetBitmapEx(hItem, &bmIconClear, 21, 8);
	//计时器--计时
	hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT0);//计时
	MultiText_SetsStyte(hItem,1); 
	MultiText_SetWinBkColor(hItem,FUN_BTN_BK_COLOR);
	MultiText_SetBKColor(hItem,FUN_BTN_BK_COLOR);
	MultiText_SetFrameColor(hItem,STANDBY_BTN_BK_COLOR);
	SetTimerHandle(hItem);//传入显示计时的控件的句柄
	sec = GetTimerSec();
	min = GetTimerMin();
	hour = GetTimerHour();			
	sprintf(pText, "%02d.%02d.%02d", hour, min, sec);
	MultiText_SetParaData(hItem, pText);
	MultiText_SetTextRect(hItem, 6, 112, 2, 32, MultiText_Data);
	MultiText_SetTextFont(hItem, &GUI_Font_32, MultiText_Data);
	MultiText_SetTextAlign(hItem, GUI_TA_HCENTER|GUI_TA_VCENTER, MultiText_Data);

	//////////////////////////////////////////////////////////////////////////
	hItem = WM_GetDialogItem(hDlg,GUI_ID_TEXT4);
	IMAGETEXT_SetBkColor(hItem,DESKTOP_BK_COLOR);
	IMAGETEXT_SetFrameColor(hItem,FUN_BTN_BK_COLOR);
	IMAGETEXT_SetStyle(hItem,3);

	//
	//hItem = WM_GetDialogItem(hDlg,GUI_ID_TEXT3);
	////TEXT_SetText(hItem,"计时器");
	//TEXT_SetText(hItem,Sys_Lib_GetDispStr(DP_TIME));
	//TEXT_SetBkColor(hItem,FUN_BTN_BK_COLOR);
	//WM_HideWindow(hItem);

}

static void _cbDialog18(WM_MESSAGE * pMsg)
{   
    WM_HWIN hDlg,hItem0,hItem1,hItem2,hItem3,hItem4,hItem6,hItem7,hItem8;
    int NCode, Id;
	int m = 0;
	int sec = 0, min = 0, hour = 0;
	char pText[10] = {0};

    hDlg = pMsg->hWin;
    switch (pMsg->MsgId)
    {
    case WM_INIT_DIALOG:
        WINDOW_SetBkColor(hDlg, GUI_BLACK/*0x420606*/);
        hItem0 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON0);
        hItem1 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON1);
        hItem2 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON2);
        hItem3 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON3);
        hItem4 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON4);
        hItem6 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON6);
        hItem7 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON7);        
		hItem8 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON8);

        BUTTON_SetBkColor(hItem0,0,0x292420); // 0xD05356
        BUTTON_SetBkColor(hItem0,3,WINDOW_BK_COLOR);
        BUTTON_SetFont(hItem0,&GUI_Font_20);
		BUTTON_SetTextColor(hItem0, 0, GUI_WHITE);

        BUTTON_SetBkColor(hItem1,0,0x292420);
        BUTTON_SetBkColor(hItem1,3,WINDOW_BK_COLOR);
        BUTTON_SetFont(hItem1,&GUI_Font_20);
		BUTTON_SetTextColor(hItem1, 0, GUI_WHITE);

        BUTTON_SetBkColor(hItem2,0,0x292420);
        BUTTON_SetBkColor(hItem2,3,WINDOW_BK_COLOR);
		BUTTON_SetFont(hItem2,&GUI_Font_20);
		BUTTON_SetTextColor(hItem2, 0, GUI_WHITE);
        
        BUTTON_SetBkColor(hItem3,0,0x292420);
        BUTTON_SetBkColor(hItem3,3,WINDOW_BK_COLOR);
		BUTTON_SetFont(hItem3,&GUI_Font_20);
		BUTTON_SetTextColor(hItem3, 0, GUI_WHITE);
        BUTTON_SetTextWrapMode(hItem3,GUI_WRAPMODE_CHAR);

        BUTTON_SetBkColor(hItem4,0,0x78F6EF);//0xD05356//0x00FFFF
        BUTTON_SetBkColor(hItem4,3,WINDOW_BK_COLOR);
		BUTTON_SetFont(hItem4,&GUI_Font_20);
		BUTTON_SetTextColor(hItem0, 0, GUI_WHITE);
        BUTTON_SetTextWrapMode(hItem0,GUI_WRAPMODE_WORD);

		BUTTON_SetBkColor(hItem6,0,0x292420);
		BUTTON_SetBkColor(hItem6,3,WINDOW_BK_COLOR);
		BUTTON_SetFont(hItem6,&GUI_Font_20);
		BUTTON_SetTextColor(hItem6, 0, GUI_WHITE);

		BUTTON_SetBkColor(hItem8,0,0x292420);
		BUTTON_SetBkColor(hItem8,3,WINDOW_BK_COLOR);
		BUTTON_SetFont(hItem8,&GUI_Font_20);
		BUTTON_SetTextColor(hItem8, 0, GUI_WHITE);

	    BUTTON_SetBkColor(hItem7,0,0xA1A545);//0xFFD080
        BUTTON_SetBkColor(hItem7,3,WINDOW_BK_COLOR);
		BUTTON_SetFont(hItem7,&GUI_Font_20);
		BUTTON_SetTextColor(hItem7, 0, GUI_WHITE);
        BUTTON_SetTextWrapMode(hItem7, GUI_WRAPMODE_WORD);

        BUTTON_SetText(hItem4,Sys_Lib_GetDispStr(DP_ALARMSILENCE));
        BUTTON_SetText(hItem0,Sys_Lib_GetDispStr(DP_ALARMS));
        BUTTON_SetText(hItem3,Sys_Lib_GetDispStr(DP_MONITORING));
        BUTTON_SetText(hItem6,Sys_Lib_GetDispStr(DP_TRENDS));
        BUTTON_SetText(hItem8,Sys_Lib_GetDispStr(DP_TOOLS));
        BUTTON_SetText(hItem2,Sys_Lib_GetDispStr(DP_SYSTEM));
        
		if(StandbyState == 0xFF)
		{
			BUTTON_SetText(hItem7,Sys_Lib_GetDispStr(DP_START));
			WM_HideWindow(hItem3);
			WM_HideWindow(hItem8);
		}
		else
		{
			BUTTON_SetText(hItem7,Sys_Lib_GetDispStr(DP_STANDBY)); 
			WM_ShowWindow(hItem3);
			WM_ShowWindow(hItem8);
		}

		TimerRectPaintParaInit(hDlg);

        break;
    case WM_NOTIFY_PARENT:
        NCode = pMsg->Data.v;
        Id = WM_GetId(pMsg->hWinSrc);
        switch(NCode)
        {
        case WM_NOTIFICATION_CLICKED:
            if(  (!(MenuOperateStatus&CALIBRATION_PRECEED)) 
              && (!(MenuOperateStatus&FRAMEWIN_POP)) )
            {
                switch(Id)
                {
                //case GUI_ID_BUTTON0:
                //    if(!CHECK_DEBUG)
                //    {
                //        if(!(Task & MEAU_UPDATE))  //djq add 20140210 
                //        {
                //            FunctionKeyPressedResume(hDlg,Id);
                //            PlanMenuType = MODES_MENU;
                //            Task |= MEAU_UPDATE;
                //        }
                //        else 
                //        {
                //            BUTTON_SetPressed(WM_GetDialogItem(hDlg, GUI_ID_BUTTON0),0);       
                //        }                        
                //    }
                //    else
                //    {
                //        BUTTON_SetPressed(WM_GetDialogItem(hDlg, GUI_ID_BUTTON0),0);       
                //    }
                //    break;
                    
                case GUI_ID_BUTTON0: // GUI_ID_BUTTON1
                    if(!CHECK_DEBUG)
                    {
                        if(!(Task & MEAU_UPDATE))  //djq add 20140210 
                        {
							/**********************************************************/	
							ModeChange_Count = 0;
							PresetMode_Flag = 0;
							for(m=0; m<VENTBUTTON_NUM; m++)
							{
								ButtonExCtrl_SetStyle(BreathSettingParaHandle[m],0);
							}		
							BreathPara_Assign(&Breath_Setting_Tmp, &BeathSettingPara);
							BreathMode_BeforeConfirm = BreathMode_BeforeACGO;
							if(WM_IsWindow(Menu_hWin.bWin))
							{
								WM_SendMessageNoPara(Menu_hWin.bWin, WM_INIT_DIALOG);			
							}
							/**********************************************************/	
                            FunctionKeyPressedResume(hDlg,Id);
                            PlanMenuType = ALARM_MENU;
                            Task |= MEAU_UPDATE;
                        }
                        else 
                        {
                            BUTTON_SetPressed(WM_GetDialogItem(hDlg, GUI_ID_BUTTON0),0);       
                        }                        
                    }
                    else
                    {
                        BUTTON_SetPressed(WM_GetDialogItem(hDlg, GUI_ID_BUTTON0),0);
                    }
                    break;
                    
                case GUI_ID_BUTTON2:
                    if(!(Task & MEAU_UPDATE))  //djq add 20140210 
                    {
						/**********************************************************/	
						ModeChange_Count = 0;
						PresetMode_Flag = 0;
						for(m=0; m<VENTBUTTON_NUM; m++)
						{
							ButtonExCtrl_SetStyle(BreathSettingParaHandle[m],0);
						}		
						BreathPara_Assign(&Breath_Setting_Tmp, &BeathSettingPara);
						BreathMode_BeforeConfirm = BreathMode_BeforeACGO;
						if(WM_IsWindow(Menu_hWin.bWin))
						{
							WM_SendMessageNoPara(Menu_hWin.bWin, WM_INIT_DIALOG);			
						}
						/**********************************************************/	
                        FunctionKeyPressedResume(hDlg,Id);
                        PlanMenuType = SYSTEM_SETTING_MENU;
                        Task |= MEAU_UPDATE;
                    }
                    else 
                    {
                        BUTTON_SetPressed(WM_GetDialogItem(hDlg, GUI_ID_BUTTON2),0);       
                    }                        
                    break;
                    
                case GUI_ID_BUTTON3:
                    if(!(Task & MEAU_UPDATE))  //djq add 20140210 
                    {
						/**********************************************************/	
						ModeChange_Count = 0;
						PresetMode_Flag = 0;
						for(m=0; m<VENTBUTTON_NUM; m++)
						{
							ButtonExCtrl_SetStyle(BreathSettingParaHandle[m],0);
						}		
						BreathPara_Assign(&Breath_Setting_Tmp, &BeathSettingPara);
						BreathMode_BeforeConfirm = BreathMode_BeforeACGO;
						if(WM_IsWindow(Menu_hWin.bWin))
						{
							WM_SendMessageNoPara(Menu_hWin.bWin, WM_INIT_DIALOG);			
						}
						/**********************************************************/	
                        FunctionKeyPressedResume(hDlg,Id);
                        PlanMenuType = MONITOR_MENU;
                        Task |= MEAU_UPDATE;
                    }
                    else 
                    {
                        BUTTON_SetPressed(WM_GetDialogItem(hDlg, GUI_ID_BUTTON3),0);       
                    }
                    break;
                    
                case GUI_ID_BUTTON4://静音  //sun add 20140514
                    if(!CHECK_DEBUG)
                    {
                        if(!(Task & MEAU_UPDATE))
                        {
                            if(!Start_Standby_Flag)
                            {
                                Start_Standby_Flag = 2;
                            }
#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C) 
                            else if(AlarmsWork.presentAlarms || (AlarmsWork.presentAlarms1 & (~ALARM_NOTLATCH)))
#else
                            else if(AlarmsWork.presentAlarms || (AlarmsWork.presentAlarms1 & (~ALARM_NOTLATCH)))
#endif
                            {
                                BUTTON_SetPressed(WM_GetDialogItem(hDlg, GUI_ID_BUTTON4),0);
                            }
                        }
                        else
                        {
#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C) 
                            if((AlarmsWork.presentAlarms & (~ALARM_STANDBY_ENTER)) >= 0x20000000)
#else
                            if((AlarmsWork.presentAlarms & (~ALARM_STANDBY_ENTER)) >= 0x10000000)
#endif
                            {
                                BUTTON_SetPressed(WM_GetDialogItem(hDlg, GUI_ID_BUTTON4),0);
                            }
                        }
                    }
                    else
                    {
                        BUTTON_SetPressed(WM_GetDialogItem(hDlg, GUI_ID_BUTTON4),0);
                    }
                    break;

		        case GUI_ID_BUTTON6://趋势  //sun add 20140514
                    if(!CHECK_DEBUG)
                    {
                        if(!(Task & MEAU_UPDATE))
                        {
							/**********************************************************/	
							ModeChange_Count = 0;
							PresetMode_Flag = 0;
							for(m=0; m<VENTBUTTON_NUM; m++)
							{
								ButtonExCtrl_SetStyle(BreathSettingParaHandle[m],0);
							}		
							BreathPara_Assign(&Breath_Setting_Tmp, &BeathSettingPara);
							BreathMode_BeforeConfirm = BreathMode_BeforeACGO;
							if(WM_IsWindow(Menu_hWin.bWin))
							{
								WM_SendMessageNoPara(Menu_hWin.bWin, WM_INIT_DIALOG);			
							}
							/**********************************************************/	

                            FunctionKeyPressedResume(hDlg,Id);
                            PlanMenuType = TREND_MENU;
                            Task |= MEAU_UPDATE;
                        }
                        else 
                        {
                            BUTTON_SetPressed(WM_GetDialogItem(hDlg, GUI_ID_BUTTON6),0);
                        }
                    }
                    else
                    {
                        BUTTON_SetPressed(WM_GetDialogItem(hDlg, GUI_ID_BUTTON6),0);
                    }
                    break;
				case GUI_ID_BUTTON8:  //sun add 20140514
					if(!CHECK_DEBUG)
					{
						if(!(Task & MEAU_UPDATE))
						{
							/**********************************************************/	
							ModeChange_Count = 0;
							PresetMode_Flag = 0;
							for(m=0; m<VENTBUTTON_NUM; m++)
							{
								ButtonExCtrl_SetStyle(BreathSettingParaHandle[m],0);
							}		
							BreathPara_Assign(&Breath_Setting_Tmp, &BeathSettingPara);
							BreathMode_BeforeConfirm = BreathMode_BeforeACGO;
							if(WM_IsWindow(Menu_hWin.bWin))
							{
								WM_SendMessageNoPara(Menu_hWin.bWin, WM_INIT_DIALOG);			
							}
							/**********************************************************/	

							FunctionKeyPressedResume(hDlg,Id);
							PlanMenuType = TOOLS_MENU;
							Task |= MEAU_UPDATE;
						}
						else 
						{
							BUTTON_SetPressed(WM_GetDialogItem(hDlg, GUI_ID_BUTTON8),0);
						}
					}
					else
					{
						BUTTON_SetPressed(WM_GetDialogItem(hDlg, GUI_ID_BUTTON8),0);
					}
					break;
                case GUI_ID_BUTTON7://待机  //sun add 20140514
                    if(!(Task & MEAU_UPDATE))
                    {
						/**********************************************************/	
						ModeChange_Count = 0;
						PresetMode_Flag = 0;
						for(m=0; m<VENTBUTTON_NUM; m++)
						{
							ButtonExCtrl_SetStyle(BreathSettingParaHandle[m],0);
						}		
						BreathPara_Assign(&Breath_Setting_Tmp, &BeathSettingPara);
						BreathMode_BeforeConfirm = BreathMode_BeforeACGO;
						if(WM_IsWindow(Menu_hWin.bWin))
						{
							WM_SendMessageNoPara(Menu_hWin.bWin, WM_INIT_DIALOG);			
						}
						/**********************************************************/	

                        //FunctionKeyPressedResume(hDlg,Id);
                        if(!Start_Standby_Flag)
                        {
                            Start_Standby_Flag = 1;
                        }
                        else
                        {
                            BUTTON_SetPressed(WM_GetDialogItem(hDlg, GUI_ID_BUTTON7),0);
                        }
                    }
                    else
                    {
                        BUTTON_SetPressed(WM_GetDialogItem(hDlg, GUI_ID_BUTTON7),0);
                    }
					if(StandbyState == 0xFF) //ylk add 20181224
					{
						WM_ShowWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON8));
// 						if(WM_IsWindow(Menu_hWin.ToolsWin.CPBInfoWin))
// 						{
// 							WM_ShowWindow(Menu_hWin.ToolsWin.CPBInfoWin);
// 						}
						if((Drager_EtCO2_Config_On)&&(EtCO2ModuleOff))
						{
							SendOperationMessage(SET_AA_PUMPON,USART2);
						}
						//InitMonitorRank_Auto(1);///----
						Min_Limit = 0;// 待机切换为工作，开始报警检测重新计时监测 
					}
					SetRefreshFlag(1);	
					SetTimerClearFlag(0);
                    break;
                default:
                    break;
                }
                if(StandbyState == 0) //ljs add 20130528
                {
                    TracingCurve_SetXScale(Menu_hWin.HomeWin.Tobj[0],1);
                    TracingCurve_SetXScale(Menu_hWin.HomeWin.Tobj[1],1);
                    TracingCurve_SetXScale(Menu_hWin.HomeWin.Tobj[2],1);
					TracingCurve_SetXScale(Menu_hWin.HomeWin.Tobj[3],1);
                }
            }
            else
            {
                BUTTON_SetPressed(pMsg->hWinSrc,0);
            }
			//
			if (!(MenuOperateStatus&CALIBRATION_PRECEED))
			{
				switch (Id)
				{
				case GUI_ID_TEXT1:
					hItem0 = WM_GetDialogItem(hDlg,GUI_ID_TEXT0);
					hItem1 = WM_GetDialogItem(hDlg,GUI_ID_TEXT1);
					if(!GetTimerHandle())//Start
					{
						SetTimerStart(1000);//1s
						SetRefreshFlag(1);//不点暂停，点Clear，继续计时
						MultiText_EX_SetBitmapEx(hItem1, &bmIconStop, 23, 5);

					}
					else//Stop
					{
						SetTimerStop();
						sec = GetTimerSec();
						min = GetTimerMin();
						hour = GetTimerHour();
						sprintf(pText, "%02d.%02d.%02d", hour,min,sec);
						MultiText_ParaUpdate(hItem0, pText);
						MultiText_EX_SetBitmapEx(hItem1, &bmIconStart, 23, 5);
					}
					break;
				case GUI_ID_TEXT2://Clear
					hItem0 = WM_GetDialogItem(hDlg,GUI_ID_TEXT0);
					hItem1 = WM_GetDialogItem(hDlg,GUI_ID_TEXT1);

					SetTimerStop(hItem0);
					MultiText_ParaUpdate(hItem0, "00.00.00");
					SetTimerSec(0);
					SetTimerMin(0);
					SetTimerHour(0);	

					MultiText_EX_SetBitmapEx(hItem1, &bmIconStart, 23, 5);
					break;
				case GUI_ID_TEXT0:
					break;


				}
			}

			//
            break;
        default:
            break;
        }
        break;
    default:
        WM_DefaultProc(pMsg);
        break;
    }
}

/***************************************************************************************************
* @brief  oxygen sensor cali win
* @para   
* @retval 
*/
static void _cbDialog21(WM_MESSAGE * pMsg)
{
    WM_HWIN hDlg,hItem0,hItem1,hItem2,hItem3;
    int Id,NCode;
    static int times = 1;
    hDlg = pMsg->hWin;
    switch (pMsg->MsgId)
    {
    case WM_INIT_DIALOG:
        WINDOW_SetUse_3D(hDlg,1);
        hItem0 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON0);
        hItem1 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON1);
		BUTTON_SetFont(hItem0,&GUI_Font_16);
		BUTTON_SetFont(hItem1,&GUI_Font_16);
        hItem2 = WM_GetDialogItem(hDlg, GUI_ID_TEXT0);
        TEXT_SetTextWrapMode(hItem2,GUI_WRAPMODE_WORD/*GUI_WRAPMODE_CHAR*/);//add zkq 20161117                
// 		hItem3 = WM_GetDialogItem(hDlg,GUI_ID_TEXT1);
		BUTTON_SetText(hItem0,Sys_Lib_GetDispStr(DP_EXIT));
		BUTTON_SetText(hItem1,Sys_Lib_GetDispStr(DP_ENTER));
		if(oxyChooseFlag == 0)//氧浓度为21% 
        {
			if(MasimoISA_EtCO2_Config_On && (AA_Module_Flag.EtCO2ModuleExist == 2)&&(AA_Module_Flag.PhaseInTypeModuleExist==2))
			{
// 				TEXT_SetText(hItem3,"1.");
				//TEXT_SetText(hItem2,Sys_Lib_GetDispStr(DP_EXPOSEAMBIENTAIR));
// 				TEXT_SetText(hItem2," 1) Expose the gas module sampling line to ambient air. \n\n 2) Please wait for the gas module to be ready. \n\n 3) Press the Enter button. ");
				TEXT_SetText(hItem2,"Expose the gas module sampling line to ambient air.  Press the Enter button.");
				if ((AA_all_data.AA_state.AA_ModuleState.zero_state == disable) 
					||(AA_all_data.AA_state.AA_ModuleState.zero_state == zero_zeroing) 
					|| (AA_all_data.AA_state.AA_ModuleState.span_state == SPAN_InProg))
				{
					WM_DisableWindow(hItem1);
				}
				else
				{
					WM_EnableWindow(hItem1);
				}

			}
			else
			{
				WM_MoveChildTo(hItem2,5,5);
				TEXT_SetText(hItem2,Sys_Lib_GetDispStr(DP_WHENAIRSUPPLYISNORMAL));//通流速>=10L/MIN的空气或将\n氧传感器置于空气中>两分钟
			}
		}
		else//氧浓度为100%
		{   
			if((BeathSettingPara.BreathMode == MANUAL_MODE) || (BeathSettingPara.BreathMode == ACGO_MODE))//ylk modify 20181212
			{
				WM_EnableWindow(hItem1);
			}
			else
			{
				WM_DisableWindow(hItem1);
			}
			if(MasimoISA_EtCO2_Config_On && (AA_Module_Flag.EtCO2ModuleExist == 2)&&(AA_Module_Flag.PhaseInTypeModuleExist==2))
			{
// 				TEXT_SetText(hItem3,"2.");
				//Switch to Bag mode,disconnect breathing tubes,plug the bag port and adjust flowmeter\'s oxygen flow more than 10L/min.\
				Connect gas module sample line to elbow and plug elbow into Y piece. Press the Enter button.
				TEXT_SetText(hItem2,"Open the ACGO， adjust the oxygen flow more than 10 L/min. connet the gas module sampling line to elbow and plug elbow into ACGO port.");
				if ((AA_all_data.AA_state.AA_ModuleState.zero_state != disable) &&(AA_all_data.AA_state.AA_ModuleState.zero_state != zero_zeroing)
					&& AA_all_data.AA_state.AA_ModuleState.span_state != SPAN_InProg
					&&(BeathSettingPara.BreathMode == MANUAL_MODE) || (BeathSettingPara.BreathMode == ACGO_MODE))
				{
					WM_EnableWindow(hItem1);
				}
				else 
				{
					WM_DisableWindow(hItem1);
				}
			}
			else
			{
				WM_MoveChildTo(hItem2,5,5);
				TEXT_SetText(hItem2,Sys_Lib_GetDispStr(DP_ADJUSTFLOWMETEROXYMORE10));
			}
        }
        break;
    case WM_NOTIFY_PARENT:
        NCode = pMsg->Data.v;
        Id = WM_GetId(pMsg->hWinSrc);
        switch(NCode)
        {
        case WM_NOTIFICATION_CLICKED:
            switch(Id)
            {
            case GUI_ID_BUTTON0://exit
                if(!(MenuOperateStatus & CALIBRATION_PRECEED))//  if... 20130118
                {
// 					if((MasimoISA_EtCO2_Config_On == 2) && ((AA_Module_Flag.EtCO2ModuleExist&0x04) == 4))

					if( (Drager_EtCO2_Config_On && AA_all_data.AA_state.AA_ModuleState.pump_state == AA_pump_high)\
						|| (MasimoISA_EtCO2_Config_On && AA_Module_Flag.PhaseInTypeModuleExist == 2))
					{
						SendOperationMessage(SET_AA_PUMPOFF,USART2);//关泵
					}
                    hItem0 = WM_GetDialogItem(Menu_hWin.ActivedWin, GUI_ID_MULTIPAGE0);
                    MULTIPAGE_SetPageChangeEnable(hItem0,1);
                    MenuOperateStatus &= ~FRAMEWIN_POP;
                    PlanMenuType = SYSTEM_SETTING_MENU;
                    Task |= MEAU_UPDATE;
                }
                break;
            case GUI_ID_BUTTON1://enter
                //add zkq 20161028
                if(oxyChooseFlag == 0) //21校验
                {
                    hItem1 = WM_GetDialogItem(Menu_hWin.ActivedWin, GUI_ID_MULTIPAGE0);
				    MULTIPAGE_SetPageChangeEnable(hItem1,0);
                    //zy 2016.6.29  

                    delay_time[0]=90*1000 + OS_TimeMS;
                    // SendOperationMessage(OXY_CALIBRATION,USART0);
                    
				    CalibrationType = OXY_CALIBRATION;
				    MenuOperateStatus |= CALIBRATION_PRECEED;                
				    hItem1 = WM_GetDialogItem(MULTIPAGE_GetWindow(hItem1,3), GUI_ID_PROGBAR0);
				    WM_ShowWindow(hItem1);
                    
                    PROGBAR_SetText(hItem1,Sys_Lib_GetDispStr(DP_CALIBRATING));
                    
                    //*//关闭弹出的提示窗口   20130417
                    MenuOperateStatus &= ~FRAMEWIN_POP;
                    PlanMenuType = SYSTEM_SETTING_MENU;
                    Task |= MEAU_UPDATE;//*/
                }
                else
                {
#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C) //100校验
                    if((BeathSettingPara.BreathMode == MANUAL_MODE) || (BeathSettingPara.BreathMode == ACGO_MODE))
#else
					if(1)
#endif
                    {
				        hItem1 = WM_GetDialogItem(Menu_hWin.ActivedWin, GUI_ID_MULTIPAGE0);
				        MULTIPAGE_SetPageChangeEnable(hItem1,0);
                        //zy 2016.6.29  

                        delay_time[0]=90*1000 + OS_TimeMS;
                        // SendOperationMessage(OXY_CALIBRATION,USART0);
                        
				        CalibrationType = OXY_CALIBRATION;
				        MenuOperateStatus |= CALIBRATION_PRECEED;                
				        hItem1 = WM_GetDialogItem(MULTIPAGE_GetWindow(hItem1,3), GUI_ID_PROGBAR0);
				        WM_ShowWindow(hItem1);                        
						PROGBAR_SetText(hItem1,Sys_Lib_GetDispStr(DP_CALIBRATING));

                        //*//关闭弹出的提示窗口   20130417
                        MenuOperateStatus &= ~FRAMEWIN_POP;
                        PlanMenuType = SYSTEM_SETTING_MENU;
                        Task |= MEAU_UPDATE;//*/
                    }
                    else
				    {
					    BUTTON_SetPressed(pMsg->hWinSrc,0);
				    }
                }
                break;//endof case GUI_ID_BUTTON1
            }
            break;//endof case WM_NOTIFICATION_CLICKED
            
        default:break;
        }
    default:
        WM_DefaultProc(pMsg);
    }
}
/***************************************************************************************************
*
*
*/
static void _cbDialog24(WM_MESSAGE * pMsg)  //sun add 20140529
{
    WM_HWIN hDlg,hItem,hItem0,hItem1,hItem2,hItem3, hText1;
    WM_HWIN hwin, hwinpara;
    int NCode, Id;
    hDlg = pMsg->hWin;
    switch (pMsg->MsgId)
    {
    case WM_INIT_DIALOG:
        WINDOW_SetUse_3D(hDlg,1);
        hItem0 = WM_GetDialogItem(hDlg, GUI_ID_TEXT0);
		hText1 = WM_GetDialogItem(hDlg, GUI_ID_TEXT1);
        hItem1 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON0);
        //BUTTON_SetBkColor(hItem1,0,0xD05356);
        hItem2 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON1);
        //BUTTON_SetBkColor(hItem2,0,0xD05356);
        hItem3 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON2);
        //BUTTON_SetBkColor(hItem3,0,0xD05356);
        BUTTON_SetText(hItem1,Sys_Lib_GetDispStr(DP_OXY21));
        BUTTON_SetText(hItem2,Sys_Lib_GetDispStr(DP_OXY100));
        
		TEXT_SetText(hItem0,Sys_Lib_GetDispStr(DP_SELECTO2CONCENTRATION));

        TEXT_SetTextWrapMode(hItem0,GUI_WRAPMODE_WORD);//add zkq 20161117
        BUTTON_SetText(hItem3,Sys_Lib_GetDispStr(DP_EXIT));
		if(MasimoISA_EtCO2_Config_On == 2)//存在顺磁氧
		{
// 			if(pAll_Data->slow.GenVal.span_err)//校准错误
			if(AA_all_data.AA_state.module_err_message[1]&0x02)//ylk modify 20181203
			{
				TEXT_SetText(hText1,Sys_Lib_GetDispStr(DP_LASTO2SPANCALFAIL));
			}
			else
			{
				TEXT_SetText(hText1,"");
			}
		}
        
        break;
    case WM_NOTIFY_PARENT:
        NCode = pMsg->Data.v;
        Id = WM_GetId(pMsg->hWinSrc);
        switch(NCode)
        {
        case WM_NOTIFICATION_CLICKED:
            switch(Id)
            {
            case GUI_ID_BUTTON0:
                oxyChooseFlag = 0;
                PlanMenuType = OXYGEN_SENSOR_CAL_MENU;
                Task |=MEAU_UPDATE;
                GUI_EndDialog(hDlg, 0);
                break;
            case GUI_ID_BUTTON1:
                oxyChooseFlag = 1;
                PlanMenuType = OXYGEN_SENSOR_CAL_MENU;
                Task |=MEAU_UPDATE;
                GUI_EndDialog(hDlg, 0);
                break;
            case GUI_ID_BUTTON2:  //退出
// 					if((MasimoISA_EtCO2_Config_On == 2) && ((AA_Module_Flag.EtCO2ModuleExist&0x04) == 4))

					if( (Drager_EtCO2_Config_On && AA_all_data.AA_state.AA_ModuleState.pump_state == AA_pump_high)\
						|| (MasimoISA_EtCO2_Config_On && AA_Module_Flag.PhaseInTypeModuleExist == 2))
					{
						SendOperationMessage(SET_AA_PUMPOFF,USART2);//关泵
					}
                    hwinpara = WM_GetDialogItem(Menu_hWin.ActivedWin,GUI_ID_MULTIPAGE0);
                    hwinpara = MULTIPAGE_GetWindow(hwinpara,3);
                    WM_DeleteWindow(Menu_hWin.RecordWin);
                    Menu_hWin.RecordWin = 0;
                    hwin = WM_GetDialogItem(hwinpara, GUI_ID_BUTTON2);
                    BUTTON_SetPressed(hwin,0);
                    MenuOperateStatus&=~FRAMEWIN_POP;
                    PlanMenuType = SYSTEM_SETTING_MENU;
                    Task |= MEAU_UPDATE;
                    hItem = WM_GetDialogItem(Menu_hWin.ActivedWin, GUI_ID_MULTIPAGE0);
                    MULTIPAGE_SetPageChangeEnable(hItem,1);
                break;
            }
            break;
        }
        break;
    default:
        WM_DefaultProc(pMsg);
    }
}


/***************************************************************************************************
* @brief  Modes -> Modes 
* @para   
* @retval 
*/
static void _cbDialog27(WM_MESSAGE * pMsg)
{
    WM_HWIN hDlg,hItem0,hItem1,  hItem2,hItem3,hItem4,hItem5,hItem6,hItem7,hItem8,hItem9,hItem10,hItem11;
//    WM_MESSAGE Msg;
	WM_HWIN hBtn_APRV, hBtn_DuoPhasic, hBtn_VS;
    int Id,NCode;
	int m = 0;
    WM_HWIN i,j;
    hDlg = pMsg->hWin;
    switch (pMsg->MsgId)
    {
    case WM_KEY:
        break;
    case WM_INIT_DIALOG:
		//WINDOW_SetBkColor(hDlg,GUI_RED);
		hItem0 = WM_GetDialogItem(hDlg,GUI_ID_CHECK0);
		CHECKBOX_SetState(hItem0,CHECKBOX_IsChecked(hItem0));//ylk add 20181115

		hItem11 = WM_GetDialogItem(hDlg, GUI_ID_TEXT1);
		TEXT_SetBkColor(hItem11,GUI_BLACK);

		hItem2 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON2);
		BUTTON_SetBkColor(hItem2,0,0xD05356);
		BUTTON_SetText(hItem2,"VCV");

        hBtn_APRV = WM_GetDialogItem(hDlg, GUI_ID_BUTTON10);
        BUTTON_SetBkColor(hBtn_APRV,0,0xD05356);
        BUTTON_SetText(hBtn_APRV,"APRV");
		hBtn_DuoPhasic = WM_GetDialogItem(hDlg, GUI_ID_BUTTON11);
		BUTTON_SetBkColor(hBtn_DuoPhasic,0,0xD05356);
		BUTTON_SetText(hBtn_DuoPhasic,"DuoPhasic");
		hBtn_VS = WM_GetDialogItem(hDlg, GUI_ID_BUTTON12);
		BUTTON_SetBkColor(hBtn_VS,0,0xD05356);
		BUTTON_SetText(hBtn_VS,"VS");

        hItem3 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON3);
        BUTTON_SetBkColor(hItem3,0,0xD05356);
        BUTTON_SetText(hItem3,"PCV");

        hItem4 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON4);
        BUTTON_SetBkColor(hItem4,0,0xD05356);
        BUTTON_SetText(hItem4,"PRVC");
		if(PRVCMode_Chose_Flag == 1)
		{
			WM_ShowWindow(hItem4);			
		}
		else
		{
			WM_HideWindow(hItem4);
		}

        hItem5 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON5);
        BUTTON_SetBkColor(hItem5,0,0xD05356);
        BUTTON_SetText(hItem5,"SIMV-V");

        hItem6 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON6);
        BUTTON_SetBkColor(hItem6,0,0xD05356);
        BUTTON_SetText(hItem6,"SIMV-P");
        if(Mode_Chose_Flag)            //hanbinsharp add 20131208
        {	
            WM_ShowWindow(hItem6);
        }
        else
        {
            WM_HideWindow(hItem6);
        }
        hItem7 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON7);
        BUTTON_SetBkColor(hItem7,0,0xD05356);
        BUTTON_SetText(hItem7,"SIMV-PRVC");
		if(PRVCMode_Chose_Flag == 1)
		{
			WM_ShowWindow(hItem7);			
		}
		else
		{
			WM_HideWindow(hItem7);
		}

        hItem8 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON8);
        BUTTON_SetBkColor(hItem8,0,0xD05356);
        BUTTON_SetText(hItem8,"SPONT/PSV");
        if(Mode_Chose_Flag)            //hanbinsharp add 20131208
        {	
            WM_ShowWindow(hItem8);
        }
        else
        {
            WM_HideWindow(hItem8);
        }
//         hItem9 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON9); //  20130704
//         BUTTON_SetBkColor(hItem9,0,0xD05356);
//         BUTTON_SetText(hItem9,"MANUAL");
// #if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C) 
//         WM_HideWindow(hItem9);
// #endif
// #ifdef MANNUL_SWITCH
//         WM_HideWindow(hItem9);
// #endif
		hItem10 = WM_GetDialogItem(hDlg, GUI_ID_CHECK0);
		CHECKBOX_SetText(hItem10,Sys_Lib_GetDispStr(DP_ADDSIGH));

        hItem0 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON0);
        BUTTON_SetBkColor(hItem0,0,0xD05356);
        hItem1 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON1);
        BUTTON_SetBkColor(hItem1,0,0xD05356);

        ModesKeyInit(hDlg);
        
        BUTTON_SetText(hItem0,Sys_Lib_GetDispStr(DP_CONFIRM));
        BUTTON_SetText(hItem1,Sys_Lib_GetDispStr(DP_CANCEL));
        
        break;
    case WM_NOTIFY_PARENT_REFLECTION:
        FunctionKeyPressedResume(Menu_hWin.hWin,0);
		/**********************************************************/	
		ModeChange_Count = 0;
		PresetMode_Flag = 0;
		for(m=0; m<VENTBUTTON_NUM; m++)
		{
			ButtonExCtrl_SetStyle(BreathSettingParaHandle[m],0);
		}		
		BreathPara_Assign(&Breath_Setting_Tmp, &BeathSettingPara);
		BreathMode_BeforeConfirm = BreathMode_BeforeACGO;
		if(WM_IsWindow(Menu_hWin.bWin))
		{
			WM_SendMessageNoPara(Menu_hWin.bWin, WM_INIT_DIALOG);			
		}
		/**********************************************************/	
        if(StandbyState==0xFF)
        {
            PlanMenuType = STANDBY_MENU;
        }
        else
        {
            PlanMenuType = MAIN_MENU;
        }
        Task |= MEAU_UPDATE;
        break;
    case WM_NOTIFY_PARENT:
        NCode = pMsg->Data.v;
        Id = WM_GetId(pMsg->hWinSrc);
        switch(NCode)
        {
        case WM_NOTIFICATION_CLICKED:
            switch(Id)
            {
//              case GUI_ID_BUTTON0:
//              case GUI_ID_BUTTON1:
				case GUI_ID_CHECK0://ylk add 20181115
					hItem0 = WM_GetDialogItem(hDlg, GUI_ID_CHECK0);
					WM_SetEnableState(hItem0,1);
					if(CHECKBOX_IsChecked(hItem0))
					{
						CHECKBOX_SetState(hItem0, 1);
						Breath_Setting_Tmp.BreathMode = SIGH_MODE;
						//BreathMode_BeforeACGO = SIGH_MODE;
						BreathMode_BeforeConfirm = SIGH_MODE;
					}
					else
					{
						CHECKBOX_SetState(hItem0, 0);
						Breath_Setting_Tmp.BreathMode = VCV_MODE;
						//BreathMode_BeforeACGO = SIGH_MODE;
						BreathMode_BeforeConfirm = SIGH_MODE;
					}
					break;
				case GUI_ID_BUTTON2:
				case GUI_ID_BUTTON3:
				case GUI_ID_BUTTON4:
				case GUI_ID_BUTTON5:
				case GUI_ID_BUTTON6:
				case GUI_ID_BUTTON7:
				case GUI_ID_BUTTON8:
// 				case GUI_ID_BUTTON9:
				case GUI_ID_BUTTON10:
				case GUI_ID_BUTTON11:
				case GUI_ID_BUTTON12:
					j = WM_GetDialogItem(hDlg, Id);
					i = WM_GetDialogItem(hDlg, GUI_ID_BUTTON2);//GUI_ID_BUTTON0 //cpp md 20130704
					while((!BUTTON_IsPressed(i))|(i==j))
					{
						i = WM_GetNextSibling(i);
						if(i==WM_GetDialogItem(hDlg, GUI_ID_BUTTON12))//GUI_ID_BUTTON6 //cpp md 20130704
						{
							break;
						}
					}
					BUTTON_SetPressed(i,0);
					BUTTON_SetPressed(j,1);
					if(Id!=GUI_ID_BUTTON2)//GUI_ID_BUTTON0 //cpp md 20130704
					{
						j = WM_GetDialogItem(hDlg, GUI_ID_CHECK0);
						CHECKBOX_Uncheck(j);
						WM_SetEnableState(j,0);
					}
					else
					{
						j = WM_GetDialogItem(hDlg, GUI_ID_CHECK0);
						WM_SetEnableState(j,1);
						if(!CHECKBOX_IsChecked(j)&(Id==GUI_ID_CHECK0))
						{
							CHECKBOX_Check(j);
						}
					}

					/*************************ylk add 20181009********************/
					BreathPara_Assign(&Breath_Setting_Tmp, &BeathSettingPara);
                    ModesSettingConfirm(hDlg, &Breath_Setting_Tmp);//保存模式到临时结构体
					SetVtValue_Limit(Patient_GetInfo(),Breath_Setting_Tmp.BreathMode);
					if(BreathMode_AfterACGO == ACGO_MODE || BreathMode_AfterMANUAL == MANUAL_MODE)
					{
						BreathMode_BeforeConfirm = Breath_Setting_Tmp.BreathMode;
					}
                    ModeChange_Count += 1;
                    if(Breath_Setting_Tmp.BreathMode == BeathSettingPara.BreathMode)
					{
						for(m = 0; m < VENTBUTTON_NUM; m++)
						{
							ButtonExCtrl_SetStyle(BreathSettingParaHandle[m],0);
						}
                        PresetMode_Flag = 0;
					}
					else
					{
                        if(ModeChange_Count <= 1)
                        {
                            BUTTON_SetPrevPressed(i,1);//当前模式按钮样式
                        }
						for(m = 0; m < VENTBUTTON_NUM; m++)
						{
							ButtonExCtrl_SetStyle(BreathSettingParaHandle[m],1);
						}
                        PresetMode_Flag = 1;
					}	
					ReviseParamatersAllMode(Breath_Setting_Tmp.BreathMode, &Breath_Setting_Tmp, AlarmSettingPara.PressureHighValue);
 					if(WM_IsWindow(Menu_hWin.bWin))
 					{
 						WM_SendMessageNoPara(Menu_hWin.bWin, WM_INIT_DIALOG);			
 					}
					if(Breath_Setting_Tmp.BreathMode != PSV_MODE)
					{
						MULTIPAGE_DisablePage(WM_GetDialogItem(Menu_hWin.ActivedWin, GUI_ID_MULTIPAGE0), 1);
					}
					else
					{
						MULTIPAGE_EnablePage(WM_GetDialogItem(Menu_hWin.ActivedWin, GUI_ID_MULTIPAGE0), 1);
					}

					break;
				case GUI_ID_BUTTON0:
				case GUI_ID_BUTTON1:
					if(StandbyState!=0xFF)
					{
						PlanMenuType=MAIN_MENU;
					}
					else
					{
						PlanMenuType=STANDBY_MENU;
					}
					Task|=MEAU_UPDATE;
					if(Id == GUI_ID_BUTTON1) //concel
					{
						PresetMode_Flag = 0;
						ModeChange_Count = 0;
						BreathPara_Assign(&Breath_Setting_Tmp, &BeathSettingPara);
						if(BreathMode_AfterACGO == ACGO_MODE || BreathMode_AfterMANUAL == MANUAL_MODE)
						{
							BreathMode_BeforeConfirm = BreathMode_BeforeACGO;
						}
						for(m=0; m<VENTBUTTON_NUM; m++)
						{
							ButtonExCtrl_SetStyle(BreathSettingParaHandle[m],0);
						}  
						if(WM_IsWindow(Menu_hWin.bWin))
						{
							WM_SendMessageNoPara(Menu_hWin.bWin, WM_INIT_DIALOG);			
						}
					}
					if(Id==GUI_ID_BUTTON0) //confirm
					{
						//  [11/8/2022 lyy add]
#ifdef DEMO
						if (Demo_Switch)
						{
							Demo_Change_Cycle = 1;
						}
#endif
						//ylk add 20190917 解决备份通气时，再点确认按钮，通气停止问题
						//点确认按钮，如果模式未改变，命令不发送
						if( (BeathSettingPara.BreathMode != Breath_Setting_Tmp.BreathMode) || 
							(BeathSettingPara.ApnoeaMode != Breath_Setting_Tmp.ApnoeaMode) ) // 解决backup模式 PCV切换失败问题
						{													
							PresetMode_Flag = 0;
							ModeChange_Count = 0;
							for(m=0; m<VENTBUTTON_NUM; m++)
							{
								ButtonExCtrl_SetStyle(BreathSettingParaHandle[m],0);
							}                                  
							BreathPara_Assign(&BeathSettingPara, &Breath_Setting_Tmp);                                                               
							ModesSettingConfirm(hDlg, &Breath_Setting_Tmp);
							SetVtValue_Limit(Patient_GetInfo(),Breath_Setting_Tmp.BreathMode);
							BreathMode_BeforeACGO = BeathSettingPara.BreathMode;
							ReviseParamatersAllMode(BeathSettingPara.BreathMode,&BeathSettingPara,AlarmSettingPara.PressureHighValue); //sun add 20140702								
							if(WM_IsWindow(Menu_hWin.bWin))
							{
								WM_SendMessageNoPara(Menu_hWin.bWin, WM_INIT_DIALOG);
							}
							Initcbdialog32string(Patient_GetInfo(), BeathSettingPara.BreathMode);
							if((BreathMode_AfterMANUAL != MANUAL_MODE) && (BreathMode_AfterACGO != ACGO_MODE)) 
							{
								SendSetModes(BeathSettingPara.BreathMode);
								Menu_hWin.InfoWin.InfoChangeFlag |= MODES_INFO_CHANGED_FLAG;
							}   
							Min_Limit = 0;//模式切换后，开始报警检测重新计时监测 
							if(BreathMode_AfterMANUAL == MANUAL_MODE)
							{
								BeathSettingPara.BreathMode = MANUAL_MODE;
							}
							if(BreathMode_AfterACGO == ACGO_MODE)
							{
								BeathSettingPara.BreathMode = ACGO_MODE;
							}
						}													
					}
					i=WM_GetDialogItem(Menu_hWin.hWin,GUI_ID_BUTTON0);//MODE键恢复
					BUTTON_SetPressed(i,0);
					break;
            }
            break;
        }
        break;
    default:
        WM_DefaultProc(pMsg);
    }
}
/***************************************************************************************************
* @brief  功能键按下恢复
* @para   
* @retval 
*/
void FunctionKeyPressedResume(WM_HWIN hDlg,int Id)
{
    WM_HWIN i,j;
    int flg = 0;
    j = WM_GetDialogItem(hDlg, Id);
    i = WM_GetDialogItem(hDlg, GUI_ID_BUTTON0);
    while((!BUTTON_IsPressed(i))|(i==j))
    {
        i = WM_GetNextSibling(i);
        if(i==0)
        {
            flg = 1;
            break;
        }
    }
    if(!flg)
    {
        BUTTON_SetPressed(i,0);
    }
}
/***************************************************************************************************
* @brief  模式设置确认
* @para   
* @retval 
*/
static void ModesSettingConfirm(WM_HWIN hDlg, BREATH_SET_PARA *BeathPara)
{
    WM_HWIN i;
    i = WM_GetDialogItem(hDlg, GUI_ID_CHECK0);
    if(!CHECKBOX_IsChecked(i))
    {
        i = WM_GetDialogItem(hDlg, GUI_ID_BUTTON12);//GUI_ID_BUTTON9
        while(!BUTTON_IsPressed(i))
        {
            i = WM_GetPrevSibling(i);
            if(i==WM_GetDialogItem(hDlg, GUI_ID_BUTTON2)) //GUI_ID_BUTTON0 
            {
                break;
            }
        }
    }
    BeathPara->PreBreathMode = BeathPara->BreathMode; //djq add 20140314 存储上一次的模式
    BeathPara->PreApneaMode = BeathPara->ApnoeaMode; //djq add 20140314 存储上一次的模式
    //ManualModeFlag = 0;
    switch(WM_GetId(i))
    {
    case  GUI_ID_CHECK0:
        BeathPara->BreathMode = SIGH_MODE;
        break;
    case  GUI_ID_BUTTON2:
        BeathPara->BreathMode = VCV_MODE;
        break;
    case  GUI_ID_BUTTON3:
        BeathPara->BreathMode = PCV_MODE;
        break;
    case  GUI_ID_BUTTON4:
        BeathPara->BreathMode = PRVC_MODE;
        break;
    case  GUI_ID_BUTTON5:
        BeathPara->BreathMode = SIMV_VCV_MODE;
        break;
    case  GUI_ID_BUTTON6:
        BeathPara->BreathMode = SIMV_PCV_MODE;
        break;
    case  GUI_ID_BUTTON7:
        BeathPara->BreathMode = SIMV_PRVC_MODE;
        break;
    case  GUI_ID_BUTTON8:
        BeathPara->BreathMode = PSV_MODE;
        break;
    case  GUI_ID_BUTTON9:
        //ManualModeFlag = 1;
        BeathPara->BreathMode = MANUAL_MODE; //  20130704
		break;
	case  GUI_ID_BUTTON10:
		BeathPara->BreathMode = APRV_MODE; //  20130704
		break;
	case  GUI_ID_BUTTON11:
		BeathPara->BreathMode = BIPAP_MODE; //  20130704
		break;
	case  GUI_ID_BUTTON12:
		BeathPara->BreathMode = VS_MODE; //  20130704
		break;
    default:
        break;
    }

	if((BeathPara->BreathMode != SIMV_PRVC_MODE) && (BeathPara->BreathMode != PRVC_MODE))
	{
        if(MonitorPara.PRVC_VT_LOW_Flag != 0 || (AlarmsWork.presentAlarms1 & ALAEM_PRVC_VT_LOW))
        {
            MonitorPara.PRVC_VT_LOW_Flag = 0;
            AlarmsWork.presentAlarms1 &= ~ALAEM_PRVC_VT_LOW;
            AlarmsWork.unclearedAlarms1 &= ~ALAEM_PRVC_VT_LOW;
        }
	}
}

/***************************************************************************************************
* @brief  Alarms -> Limits2
* @para   
* @retval 
*/
static void _cbDialog29(WM_MESSAGE * pMsg)
{
    WM_HWIN hDlg,hItem,hItem1,hText0,hText2,hText4,hText6,hText8,hText9,hText5,hText7;
    WM_HWIN tWin;
    int NCode,Id,flg=0;
    int Data;
    WM_MESSAGE Msg;
    hDlg = pMsg->hWin;
    switch (pMsg->MsgId)
    {
    case WM_INIT_DIALOG:
        hItem = WM_GetDialogItem(hDlg, GUI_ID_IMAGETEXT0);
        IMAGETEXT_SetBitmap(hItem,&bmHigh_limit);
        hItem = WM_GetDialogItem(hDlg, GUI_ID_IMAGETEXT1);
        IMAGETEXT_SetBitmap(hItem,&bmLow_limit);

        hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT1);
        TEXT_SetFont(hItem,&GUI_Font_15);
        hItem1 = WM_GetDialogItem(hDlg, GUI_ID_TEXT3);
        TEXT_SetFont(hItem1,&GUI_Font_15);
        
        switch(ETCO2Unit)
        {
        case 0:
            TEXT_SetText(hItem,Sys_Lib_GetDispStr(DP_PERCENT));
            TEXT_SetText(hItem1,Sys_Lib_GetDispStr(DP_PERCENT));            
            break;
        case 1:
            TEXT_SetText(hItem,Sys_Lib_GetDispStr(DP_MMHG));
            TEXT_SetText(hItem1,Sys_Lib_GetDispStr(DP_MMHG));
            break;
        case 2:
            TEXT_SetText(hItem,Sys_Lib_GetDispStr(DP_KPA));
            TEXT_SetText(hItem1,Sys_Lib_GetDispStr(DP_KPA));
            break;
        default:
            break;
        }
 
        hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT5);
        TEXT_SetFont(hItem,&GUI_Font_15);
        hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT7);
        TEXT_SetFont(hItem,&GUI_Font_15);
        hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT9);
        TEXT_SetFont(hItem,&GUI_Font_15);

        hText0 = WM_GetDialogItem(hDlg,GUI_ID_TEXT0);
        hText2 = WM_GetDialogItem(hDlg,GUI_ID_TEXT2);
        hText4 = WM_GetDialogItem(hDlg,GUI_ID_TEXT4);
        hText6 = WM_GetDialogItem(hDlg,GUI_ID_TEXT6);
        hText8 = WM_GetDialogItem(hDlg,GUI_ID_TEXT8);    
        AlarmLimitParaInit(hDlg,&AlarmSettingPara,1);

#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C) //add ylk 20170911
		hText5 = WM_GetDialogItem(hDlg,GUI_ID_TEXT5);
        TEXT_SetText(hText5,Sys_Lib_GetDispStr(DP_PERCENT));
		hText7 = WM_GetDialogItem(hDlg,GUI_ID_TEXT7);
        TEXT_SetText(hText7,Sys_Lib_GetDispStr(DP_PERCENT));
		hText9 = WM_GetDialogItem(hDlg,GUI_ID_TEXT9);
        TEXT_SetText(hText9,Sys_Lib_GetDispStr(DP_PERCENT));

        TEXT_SetText(hText4,Sys_Lib_GetDispStr(DP_FIN2O));
#endif        
        TEXT_SetText(hText0,Sys_Lib_GetDispStr(DP_ETCO2));
        TEXT_SetText(hText2,Sys_Lib_GetDispStr(DP_FICO2));

		TEXT_SetText(hText6,Sys_Lib_GetDispStr(DP_ETAA1));
		TEXT_SetText(hText8,Sys_Lib_GetDispStr(DP_FIAA1));

		switch(AA_all_data.AA_detail.AA_AA1.ID)//ylk modify 不同麻醉气体类型
		{
		case IM_AGT_HAL:
			TEXT_SetText(hText6, Sys_Lib_GetDispStr(DP_ETHAL)); 
			TEXT_SetText(hText8, Sys_Lib_GetDispStr(DP_FIHAL)); 
			break;
		case IM_AGT_ENF:
			TEXT_SetText(hText6, Sys_Lib_GetDispStr(DP_ETENF)); 
			TEXT_SetText(hText8, Sys_Lib_GetDispStr(DP_FIENF)); 
			break;
		case IM_AGT_ISO:
			TEXT_SetText(hText6, Sys_Lib_GetDispStr(DP_ETISO));
			TEXT_SetText(hText8, Sys_Lib_GetDispStr(DP_FIISO));
			break;
		case IM_AGT_SEV:
			TEXT_SetText(hText6, Sys_Lib_GetDispStr(DP_ETSEV)); 
			TEXT_SetText(hText8, Sys_Lib_GetDispStr(DP_FISEV));
			break;
		case IM_AGT_DES:
			TEXT_SetText(hText6, Sys_Lib_GetDispStr(DP_ETDES)); 
			TEXT_SetText(hText8, Sys_Lib_GetDispStr(DP_FIDES)); 
			break;
		default: 
			break;
		}

        break;
        
    case WM_NOTIFY_PARENT:
        NCode = pMsg->Data.v;
        Id = WM_GetId(pMsg->hWinSrc);
		switch(Id)
		{
		case GUI_ID_BUTTON0:
		case GUI_ID_BUTTON1:
		case GUI_ID_BUTTON2:
		case GUI_ID_BUTTON3:
		case GUI_ID_BUTTON4:
		case GUI_ID_BUTTON5:
		case GUI_ID_BUTTON6:
		case GUI_ID_BUTTON7:
		case GUI_ID_BUTTON8:
		case GUI_ID_BUTTON9:
			switch(NCode)
			{
			case WM_NOTIFICATION_LOST_FOCUS: //  20130604
				tWin = WM_GetDialogItem(hDlg,GUI_ID_BUTTON0);
				while(!ButtonRotation_GetTriangleSign(tWin))
				{
					tWin = WM_GetNextSibling(tWin);
					if(tWin==0)
					{
						flg = 1;
						break;
					}
				}
				if(!flg)
				{
					ButtonRotation_SetTriangleSign(tWin,0);
				}
				break;
			case WM_NOTIFICATION_CLICKED:
			case GUI_KEY_ENTER:
				if(ButtonRotation_GetTriangleSign(pMsg->hWinSrc))
				{
					ButtonRotation_SetTriangleSign(pMsg->hWinSrc,0);
					Data = ButtonRotation_GetData1(pMsg->hWinSrc);
					ButtonRotation_SetData(pMsg->hWinSrc,Data);
					AlarmSettingValueUpdate(Data,1,&AlarmSettingPara,&Menu_hWin.HomeWin,StandbyState,Id);//transmit the parameter to control mcu
				}
				else
				{
					tWin = WM_GetDialogItem(hDlg,GUI_ID_BUTTON0);
					while(!ButtonRotation_GetTriangleSign(tWin))
					{
						tWin = WM_GetNextSibling(tWin);
						if(tWin==0)
						{
							flg = 1;
							break;
						}
					}
					if(!flg)
					{
						ButtonRotation_SetTriangleSign(tWin,0);
					}
					Data = ButtonRotation_GetData(pMsg->hWinSrc);
					ButtonRotation_SetData1(pMsg->hWinSrc,Data);
					WM_SetFocus(pMsg->hWinSrc);
					ButtonRotation_SetTriangleSign(pMsg->hWinSrc,1);
				}
				break;
			case GUI_KEY_LEFT:
				if(ButtonRotation_GetTriangleSign(pMsg->hWinSrc))//if setting now?
				{
					if(AlarmParameterLimitAnalysis(pMsg->hWinSrc,1,GUI_KEY_LEFT,&AlarmSettingPara))
					{
						Msg.MsgId   = WM_NOTIFICATION_VALUE_CHANGED;
						Msg.Data.v  = GUI_KEY_LEFT;
						Msg.hWin = pMsg->hWinSrc;
						WM_SendMessage(pMsg->hWinSrc,&Msg);
					}
				}
				break;
			case GUI_KEY_RIGHT:
				if(ButtonRotation_GetTriangleSign(pMsg->hWinSrc))//if setting now?
				{
					if(AlarmParameterLimitAnalysis(pMsg->hWinSrc,1,GUI_KEY_RIGHT,&AlarmSettingPara))
					{
						Msg.MsgId   = WM_NOTIFICATION_VALUE_CHANGED;
						Msg.Data.v  = GUI_KEY_RIGHT;
						Msg.hWin = pMsg->hWinSrc;
						WM_SendMessage(pMsg->hWinSrc,&Msg);
					}
				}
				break;
			}
			break;
		}
        break;
    default:
        WM_DefaultProc(pMsg);
    }
    
}
/***************************************************************************************************
* @brief  Alarms -> Limits3
* @para   
* @retval 
*/
static void _cbDialog40(WM_MESSAGE * pMsg)
{
    WM_HWIN hDlg,hItem,hText0,hText2;
    WM_HWIN tWin;
    int NCode,Id,flg=0;
    int Data;
    WM_MESSAGE Msg;
    hDlg = pMsg->hWin;
    switch (pMsg->MsgId)
    {
    case WM_INIT_DIALOG:
        hItem = WM_GetDialogItem(hDlg, GUI_ID_IMAGETEXT0);
        IMAGETEXT_SetBitmap(hItem,&bmHigh_limit);
        hItem = WM_GetDialogItem(hDlg, GUI_ID_IMAGETEXT1);
        IMAGETEXT_SetBitmap(hItem,&bmLow_limit);

        hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT1);
		TEXT_SetText(hItem, Sys_Lib_GetDispStr(DP_BPM));//bpm
        TEXT_SetFont(hItem,&GUI_Font_15);
        hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT3);
		TEXT_SetText(hItem, Sys_Lib_GetDispStr(DP_PERCENT));//%
        TEXT_SetFont(hItem,&GUI_Font_15);
        hText0 = WM_GetDialogItem(hDlg,GUI_ID_TEXT0);
        hText2 = WM_GetDialogItem(hDlg,GUI_ID_TEXT2);
        AlarmLimitParaInit(hDlg,&AlarmSettingPara,2);
        
        TEXT_SetText(hText0,Sys_Lib_GetDispStr(DP_PULSE));
        TEXT_SetText(hText2,Sys_Lib_GetDispStr(DP_SPO2));
        
        break;
    case WM_NOTIFY_PARENT:
        NCode = pMsg->Data.v;
        Id = WM_GetId(pMsg->hWinSrc);
		switch(Id)
		{
		case GUI_ID_BUTTON0:
		case GUI_ID_BUTTON1:
		case GUI_ID_BUTTON2:
		case GUI_ID_BUTTON3:
			switch(NCode)
			{
			case WM_NOTIFICATION_LOST_FOCUS: //  20130604
				tWin = WM_GetDialogItem(hDlg,GUI_ID_BUTTON0);
				while(!ButtonRotation_GetTriangleSign(tWin))
				{
					tWin = WM_GetNextSibling(tWin);
					if(tWin==0)
					{
						flg = 1;
						break;
					}
				}
				if(!flg)
				{
					ButtonRotation_SetTriangleSign(tWin,0);
				}
				break;
			case WM_NOTIFICATION_CLICKED:
			case GUI_KEY_ENTER:
				if(ButtonRotation_GetTriangleSign(pMsg->hWinSrc))
				{
					ButtonRotation_SetTriangleSign(pMsg->hWinSrc,0);
					Data = ButtonRotation_GetData1(pMsg->hWinSrc);
					ButtonRotation_SetData(pMsg->hWinSrc,Data);
					AlarmSettingValueUpdate(Data,2,&AlarmSettingPara,&Menu_hWin.HomeWin,StandbyState,Id);//transmit the parameter to control mcu
				}
				else
				{
					tWin = WM_GetDialogItem(hDlg,GUI_ID_BUTTON0);
					while(!ButtonRotation_GetTriangleSign(tWin))
					{
						tWin = WM_GetNextSibling(tWin);
						if(tWin==0)
						{
							flg = 1;
							break;
						}
					}
					if(!flg)
					{
						ButtonRotation_SetTriangleSign(tWin,0);
					}
					Data = ButtonRotation_GetData(pMsg->hWinSrc);
					ButtonRotation_SetData1(pMsg->hWinSrc,Data);
					WM_SetFocus(pMsg->hWinSrc);
					ButtonRotation_SetTriangleSign(pMsg->hWinSrc,1);
				}
				break;
			case GUI_KEY_LEFT:
				if(ButtonRotation_GetTriangleSign(pMsg->hWinSrc))//if setting now?
				{
					if(AlarmParameterLimitAnalysis(pMsg->hWinSrc,2,GUI_KEY_LEFT,&AlarmSettingPara))
					{
						Msg.MsgId   = WM_NOTIFICATION_VALUE_CHANGED;
						Msg.Data.v  = GUI_KEY_LEFT;
						Msg.hWin = pMsg->hWinSrc;
						WM_SendMessage(pMsg->hWinSrc,&Msg);
					}
				}
				break;
			case GUI_KEY_RIGHT:
				if(ButtonRotation_GetTriangleSign(pMsg->hWinSrc))//if setting now?
				{
					if(AlarmParameterLimitAnalysis(pMsg->hWinSrc,2,GUI_KEY_RIGHT,&AlarmSettingPara))
					{
						Msg.MsgId   = WM_NOTIFICATION_VALUE_CHANGED;
						Msg.Data.v  = GUI_KEY_RIGHT;
						Msg.hWin = pMsg->hWinSrc;
						WM_SendMessage(pMsg->hWinSrc,&Msg);
					}
				}
				break;
			}
			break;
		}
        break;
    default:
        WM_DefaultProc(pMsg);
    }
}
/***************************************************************************************************
* @brief  vent para win
* @para   
* @retval 
*/
static void _cbDialog30(WM_MESSAGE * pMsg)
{
    WM_HWIN hDlg;
    WM_HWIN tWin;
    int NCode,Id,flg=0;
    int Data;
    static Int64U TestFlag = 1;//测试
    static unsigned char Triangleing = 0;
    WM_MESSAGE Msg;
    hDlg = pMsg->hWin;
    switch (pMsg->MsgId)
    {
    case WM_INIT_DIALOG:
        WINDOW_SetBkColor(hDlg,0x404040/*0x420606*/);//
 		VentParaWinInit(hDlg,&Breath_Setting_Tmp);
        break;
    case WM_NOTIFY_PARENT:
        NCode = pMsg->Data.v;
        Id = WM_GetId(pMsg->hWinSrc);
        switch(NCode)
        {
        case WM_NOTIFICATION_LOST_FOCUS: //  20130604
            if((MenuType!=SYSTEM_SETTING_MENU) && (MenuType!=ENGINEER_MENU) && (MenuType!=ENGINEER_CALIBRATION_MENU)
             &&(MenuType!=OXYGEN_SENSOR_CAL_MENU) && (MenuType!=UPDATE_PROGRAM_MODE) &&(MenuType!=OXY_CHOOSE_MENU)
             &&(MenuType!=MODES_MENU)&& (MenuType!=MONITOR_MENU)&& (MenuType!=ALARM_MENU) && (MenuType!=ACTIVATE_STANDBY_MENU)
             &&(MenuType!=TREND_MENU))
            {
                if(WM_IsVisible(Menu_hWin.TextWin))  //sun add 20140707
                {
                    WM_HideWindow(Menu_hWin.TextWin);
                }
			}
            tWin = WM_GetDialogItem(hDlg,GUI_ID_BUTTON0);
            while(!ButtonExCtrl_GetTriangleSign(tWin))
            {
                tWin = WM_GetNextSibling(tWin);
                if(tWin==0)
                {
                    flg = 1;
                    break;
                }
            }
            if(!flg)
            {
                ButtonExCtrl_SetTriangleSign(tWin,0);
            }
            break;
            
        
        case WM_NOTIFICATION_CLICKED:
        case GUI_KEY_ENTER:
            if(MenuType==WAVEFORM_SETTING_MENU)
            {
                PlanMenuType=MAIN_MENU;
                Task|=MEAU_UPDATE;
            }
            if(ButtonExCtrl_GetTriangleSign(pMsg->hWinSrc))
            {
                 if(!Triangleing)
                 {   
                    if((MenuType!=SYSTEM_SETTING_MENU) && (MenuType!=ENGINEER_MENU) && (MenuType!=ENGINEER_CALIBRATION_MENU)
                     &&(MenuType!=OXYGEN_SENSOR_CAL_MENU) && (MenuType!=UPDATE_PROGRAM_MODE) &&(MenuType!=OXY_CHOOSE_MENU)
                     &&(MenuType!=MODES_MENU)&& (MenuType!=MONITOR_MENU)&& (MenuType!=ALARM_MENU) && (MenuType!=ACTIVATE_STANDBY_MENU)
                     &&(MenuType!=TREND_MENU))
                    {
                        if(WM_IsVisible(Menu_hWin.TextWin))  //sun add 20140707
                        { 
                            WM_HideWindow(Menu_hWin.TextWin);							
                        }
                    }
                    WM_SelectWindow(pMsg->hWinSrc);
                    ButtonExCtrl_SetTriangleSign(pMsg->hWinSrc,0);
                    Data = ButtonExCtrl_GetData1(pMsg->hWinSrc);
                    ButtonExCtrl_SetData(pMsg->hWinSrc,Data);
                    
                    // 控制参数PLimit关联压力报警上限
                    if((BeathSettingPara.BreathMode == VS_MODE) && (Id == 0x183))
                    {
                        AlarmSettingPara.PressureHighValue = Data;
                        //WM_SendMessageNoPara(Menu_hWin.Alarm_Win.AlarmWin_Lim1,WM_INIT_DIALOG);
                    }
                        
                    SendSetParameter(Data,Id);//transmit the parameter to control mcu

					if(!PresetMode_Flag)
					{
                        if(BreathMode_AfterMANUAL == MANUAL_MODE)
                        {
                            Breath_Setting_Tmp.BreathMode = BreathMode_AfterMANUAL;
                        }
                        if(BreathMode_AfterACGO == ACGO_MODE)
                        {
                            Breath_Setting_Tmp.BreathMode = BreathMode_AfterACGO;
                        }
                        BreathPara_Assign(&BeathSettingPara, &Breath_Setting_Tmp);//ylk add 20181009	
						
						if((BeathSettingPara.BreathMode == VS_MODE) && (Id == 0x183))
						{
							AlarmSettingPara.PressureHighValue = BeathSettingPara.SettingPHLimitValue;
							//WM_SendMessageNoPara(Menu_hWin.Alarm_Win.AlarmWin_Lim1,WM_INIT_DIALOG);
						}
					}
                    
                    ParaSettingState.flg = 0;
                    ParaSettingState.hWin = 0;
                    Triangleing = 0;
                 }
            }
            else //start setting
            {
                if(StandbyState==0)
                {
                    WM_BringToBottom(Menu_hWin.HomeWin.ParentWin);
                }
                else
                {
                    WM_BringToBottom(Menu_hWin.Swin);
                }
                tWin = WM_GetDialogItem(hDlg,GUI_ID_BUTTON0);
                while(!ButtonExCtrl_GetTriangleSign(tWin))
                {
                    tWin = WM_GetNextSibling(tWin);
                    if(tWin==0)
                    {
                        flg = 1;
                        break;
                    }
                }
                if(!flg)
                {
                    ButtonExCtrl_SetTriangleSign(tWin,0);
                }
                Data = ButtonExCtrl_GetData(pMsg->hWinSrc);
                ButtonExCtrl_SetData(pMsg->hWinSrc,Data);
                WM_SetFocus(pMsg->hWinSrc);
                ButtonExCtrl_SetTriangleSign(pMsg->hWinSrc,1);  

                ParaSettingState.flg = 1;
                ParaSettingState.hWin = pMsg->hWinSrc;
            }
            break;
        case GUI_KEY_LEFT:
            if(ButtonExCtrl_GetTriangleSign(pMsg->hWinSrc))//if setting now?
            {
                if(ParameterLimitAnalysis(pMsg->hWinSrc,GUI_KEY_LEFT))
                {
                    Msg.MsgId   = WM_NOTIFICATION_VALUE_CHANGED;
                    Msg.Data.v  = GUI_KEY_LEFT;
                    Msg.hWin = pMsg->hWinSrc;
                    WM_SendMessage(pMsg->hWinSrc,&Msg);
                }
				 if((MenuType!=SYSTEM_SETTING_MENU) && (MenuType!=ENGINEER_MENU) && (MenuType!=ENGINEER_CALIBRATION_MENU) 
					 &&(MenuType!=OXYGEN_SENSOR_CAL_MENU) && (MenuType!=UPDATE_PROGRAM_MODE) &&(MenuType!=OXY_CHOOSE_MENU)
					 && (MenuType!=MODES_MENU)&& (MenuType!=MONITOR_MENU)&& (MenuType!=ALARM_MENU) && (MenuType!=ACTIVATE_STANDBY_MENU)
                     && (MenuType!=TREND_MENU))
				 {
					if(WM_IsVisible(Menu_hWin.TextWin))
					{
						_ControlTextShow(contorlText);
						memset(contorlText,0,sizeof(contorlText));
					}
					else
					{
						//WM_ShowWindow(Menu_hWin.TextWin);
						_ControlTextShow(contorlText);
						memset(contorlText,0,sizeof(contorlText));
					}
				}
            }
            else
            {
            }
            break;
        case GUI_KEY_RIGHT:
            if(ButtonExCtrl_GetTriangleSign(pMsg->hWinSrc))//if setting now?
            {
                if(ParameterLimitAnalysis(pMsg->hWinSrc,GUI_KEY_RIGHT))
                {
                    //ButtonExCtrl_DataAdd(hWin);
                    Msg.MsgId   = WM_NOTIFICATION_VALUE_CHANGED;
                    Msg.Data.v  = GUI_KEY_RIGHT;
                    Msg.hWin = pMsg->hWinSrc;
                    WM_SendMessage(pMsg->hWinSrc,&Msg);
                }
				  if((MenuType!=SYSTEM_SETTING_MENU) && (MenuType!=ENGINEER_MENU) && (MenuType!=ENGINEER_CALIBRATION_MENU) 
					 &&(MenuType!=OXYGEN_SENSOR_CAL_MENU) && (MenuType!=UPDATE_PROGRAM_MODE) &&(MenuType!=OXY_CHOOSE_MENU)
					 && (MenuType!=MODES_MENU)&& (MenuType!=MONITOR_MENU)&& (MenuType!=ALARM_MENU) && (MenuType!=ACTIVATE_STANDBY_MENU)
                     && (MenuType!=TREND_MENU))
				 {
					if(WM_IsVisible(Menu_hWin.TextWin))
					{
						_ControlTextShow(contorlText);
						memset(contorlText,0,sizeof(contorlText));
					}
					else
					{
						//WM_ShowWindow(Menu_hWin.TextWin);
						_ControlTextShow(contorlText);
						memset(contorlText,0,sizeof(contorlText));
					}
				}
            }
            else
            {
            }
            break;
        }
        break;
    default:
        WM_DefaultProc(pMsg);
    }
}
/***************************************************************************************************
* @brief  
* @para   
* @retval 
*/
static void VentParaWinInitNormal(WM_HWIN hDlg,BREATH_SET_PARA * Parameter,int BreathMode)
{
    WM_HWIN hItem0,hItem1,hItem2,hItem3,hItem4,hItem5,hItem6,hItem7;
    int Value; //  20130617
    int i = 0, j;
    hItem0 = WM_GetDialogItem(hDlg,GUI_ID_BUTTON0);
    hItem1 = WM_GetDialogItem(hDlg,GUI_ID_BUTTON1);
    hItem2 = WM_GetDialogItem(hDlg,GUI_ID_BUTTON2);
    hItem3 = WM_GetDialogItem(hDlg,GUI_ID_BUTTON3);
    hItem4 = WM_GetDialogItem(hDlg,GUI_ID_BUTTON4);
    hItem5 = WM_GetDialogItem(hDlg,GUI_ID_BUTTON5);
    hItem6 = WM_GetDialogItem(hDlg,GUI_ID_BUTTON6);
	hItem7 = WM_GetDialogItem(hDlg,GUI_ID_BUTTON7);
	ButtonExCtrl_SetTriangleSign(hItem0,0);
    ButtonExCtrl_SetTriangleSign(hItem1,0);
    ButtonExCtrl_SetTriangleSign(hItem2,0);
    ButtonExCtrl_SetTriangleSign(hItem3,0);
    ButtonExCtrl_SetTriangleSign(hItem4,0);
    ButtonExCtrl_SetTriangleSign(hItem5,0);
    ButtonExCtrl_SetTriangleSign(hItem6,0);
	ButtonExCtrl_SetTriangleSign(hItem7,0);
	if(LanguageType == Disp_Lg_Chinese)
	{
		ButtonExCtrl_SetFontEx(hItem0,&GUI_Font_15,2);
		ButtonExCtrl_SetFontEx(hItem1,&GUI_Font_15,2);
		ButtonExCtrl_SetFontEx(hItem2,&GUI_Font_15,2);
		ButtonExCtrl_SetFontEx(hItem3,&GUI_Font_15,2);
		ButtonExCtrl_SetFontEx(hItem4,&GUI_Font_15,2);
		ButtonExCtrl_SetFontEx(hItem5,&GUI_Font_15,2);
		ButtonExCtrl_SetFontEx(hItem6,&GUI_Font_15,2);
		ButtonExCtrl_SetFontEx(hItem7,&GUI_Font_15,2);
	}
	else
	{
		ButtonExCtrl_SetFontEx(hItem0,&GUI_Font_20,2);//GUI_Font_16
		ButtonExCtrl_SetFontEx(hItem1,&GUI_Font_20,2);
		ButtonExCtrl_SetFontEx(hItem2,&GUI_Font_20,2);
		ButtonExCtrl_SetFontEx(hItem3,&GUI_Font_20,2);
		ButtonExCtrl_SetFontEx(hItem4,&GUI_Font_20,2);
		ButtonExCtrl_SetFontEx(hItem5,&GUI_Font_20,2);
		ButtonExCtrl_SetFontEx(hItem6,&GUI_Font_20,2);
		ButtonExCtrl_SetFontEx(hItem7,&GUI_Font_20,2);
	}
	i = WM_GetDialogItem(hDlg,GUI_ID_BUTTON0);//ylk add 20181009
	for(j = 0;j < VENTBUTTON_NUM; j++)
	{
		BreathSettingParaHandle[j] = i;
		i = WM_GetNextSibling(i);
	}
//     switch(Parameter->BreathMode)
	switch(BreathMode)
    {
    case VCV_MODE:
    case SIGH_MODE:
        //*//   20130617
        Value = Parameter->SettingIEValue;
        if(Value >= 6)
        {
            Value = 10*60*(100-Parameter->SettingTpValue)/((10+IE_RatioTable[Value])*Parameter->SettingRateValue*1.0) + 0.5;
        }
        else
        {
            Value = IE_RatioTable[Value]*60*(100-Parameter->SettingTpValue)/((10+IE_RatioTable[Value])*Parameter->SettingRateValue);//  20130306
        }
        Parameter->SettingTiValue = Value;// (Value/10)*10;
     
        if(!WM_IsVisible(hItem0))
        {
            WM_ShowWindow(hItem0);
        }

        ButtonExCtrl_SetFontInfo(hItem0,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_VT),Sys_Lib_GetDispStr(DP_ML));
    

		if(Parameter->SettingVtValue >= 100)
		{
			ButtonExCtrl_SetButtonDataInfo(hItem0,Vt_LowLimit,Vt_HighLimit,Parameter->SettingVtValue,10,0);
		}
		else
		{
			ButtonExCtrl_SetButtonDataInfo(hItem0,Vt_LowLimit,Vt_HighLimit,Parameter->SettingVtValue,5,0);
		}

        if(Parameter->SettingVtValue*100*3 / Parameter->SettingTiValue < 100) 
        {
            Parameter->SettingVtValue = Parameter->SettingTiValue;
        }
        if(Parameter->SettingVtValue*100 / Parameter->SettingTiValue > 1500)
        {
            Parameter->SettingVtValue = 1500/100 * Parameter->SettingTiValue; 
        }
        if(Parameter->SettingVtValue > 100)  //sun add 20140707
        {
            if((Parameter->SettingVtValue % 10) != 0)
            {
                Parameter->SettingVtValue = Parameter->SettingVtValue - (Parameter->SettingVtValue % 10);
            }
        }
        else
        {
            if((Parameter->SettingVtValue % 5) != 0)
            {
                Parameter->SettingVtValue = Parameter->SettingVtValue - (Parameter->SettingVtValue % 5);
            }
        }
        Value = Parameter->SettingIEValue;
        if(Value >= 6)
        {
            Value = 10*60*(100)/((10+IE_RatioTable[Value])*Parameter->SettingRateValue*1.0) + 0.5;//  20130306
        }
        else
        {
            Value = IE_RatioTable[Value]*60*(100)/((10+IE_RatioTable[Value])*Parameter->SettingRateValue);//  20130306
        }
        Parameter->SettingTiValue = Value;// (Value/10)*10;

        ButtonExCtrl_SetData(hItem0,Parameter->SettingVtValue);
        if(!WM_IsVisible(hItem1))
        {
            WM_ShowWindow(hItem1);
        }
        ButtonExCtrl_SetFontInfo(hItem1,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_IE),Sys_Lib_GetDispStr(DP_EMPTY));
#if (LOGO_FLAG == ENDURE_LOGO)
		ButtonExCtrl_SetButtonDataInfo(hItem1,0,28,Parameter->SettingIEValue,1,0);
#else
		ButtonExCtrl_SetButtonDataInfo(hItem1,0,24,Parameter->SettingIEValue,1,0);
#endif
        ButtonExCtrl_SetData(hItem1,Parameter->SettingIEValue);
        if(!WM_IsVisible(hItem2))
        {
            WM_ShowWindow(hItem2);
        }

        ButtonExCtrl_SetFontInfo(hItem2,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_F),Sys_Lib_GetDispStr(DP_BPM));//bmp cpp md 20130328     
		ButtonExCtrl_SetButtonDataInfo(hItem2,2,100,Parameter->SettingotherRateValue,1,0);
        ButtonExCtrl_SetData(hItem2,Parameter->SettingotherRateValue);
        if(!WM_IsVisible(hItem3))
        {
            WM_ShowWindow(hItem3);
        }

        ButtonExCtrl_SetFontInfo(hItem3,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_PAUSE),Sys_Lib_GetDispStr(DP_PERCENT));
        ButtonExCtrl_SetButtonDataInfo(hItem3,0,60,Parameter->SettingTpValue,5,0);
        ButtonExCtrl_SetData(hItem3,Parameter->SettingTpValue);

        if(WM_IsVisible(hItem4)) //  20130617
        {
            WM_HideWindow(hItem4);
        }
                
        if(WM_IsVisible(hItem5)) //cpp del 20130407
        {
            WM_HideWindow(hItem5);
        }

        //*//  20130328
        if(!WM_IsVisible(hItem6))
        {
            WM_ShowWindow(hItem6);
        }
		ButtonExCtrl_SetFontInfo(hItem6,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_PEEP),Sys_Lib_GetDispStr(DP_CMH2O));
		ButtonExCtrl_SetButtonDataInfo(hItem6,2,30,Parameter->SettingPEEPValue,1,0);
		ButtonExCtrl_SetData(hItem6, Parameter->SettingPEEPValue);
		//*/
		if(WM_IsVisible(hItem7)) //sun add 20140623
        {
            WM_HideWindow(hItem7);
        }
        break;
        
    case PCV_MODE:
        //*//   20130618
        Value = Parameter->SettingIEValue;
        if(Value >= 6)
        {
            //Value = 10*60*(100-Parameter->SettingTpValue)/((10+IE_RatioTable[Value])*Parameter->SettingRateValue);//  20130306
            Value = 10*60*100/((10+IE_RatioTable[Value])*Parameter->SettingRateValue*1.0) + 0.5;//  20130306
        }
        else
        {
            //Value = IE_RatioTable[Value]*60*(100-Parameter->SettingTpValue)/((10+IE_RatioTable[Value])*Parameter->SettingRateValue);//  20130306
            Value = IE_RatioTable[Value]*60*100/((10+IE_RatioTable[Value])*Parameter->SettingRateValue);//  20130306
        }
        Parameter->SettingTiValue = Value; //(Value/10)*10(); // 先除以10，去掉尾数，后乘以10就变成整数了
        //*///

        if(!WM_IsVisible(hItem0))
        {
            WM_ShowWindow(hItem0);
        }
        ButtonExCtrl_SetFontInfo(hItem0,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_PINSP),Sys_Lib_GetDispStr(DP_CMH2O));
        if(Parameter->SettingPinspValue >= AlarmSettingPara.PressureHighValue)//PEEP最小值为2
        {  //2013-12-16
            Parameter->SettingPinspValue = AlarmSettingPara.PressureHighValue - 1;
            ButtonExCtrl_SetButtonDataInfo(hItem0,5,70,Parameter->SettingPinspValue,1,0);
        }
        else 
        {
            ButtonExCtrl_SetButtonDataInfo(hItem0,5,70,Parameter->SettingPinspValue,1,0);
        }
        //*//  20130328
        if(Parameter->SettingPinspValue + Parameter->SettingPEEPValue >= AlarmSettingPara.PressureHighValue)
        {
            Parameter->SettingPinspValue = AlarmSettingPara.PressureHighValue - BeathSettingPara.SettingPEEPValue - 1;//5
            if(Parameter->SettingPinspValue <= 5)  Parameter->SettingPinspValue = 5;
        }//*/
        ButtonExCtrl_SetData(hItem0,Parameter->SettingPinspValue);
        if(!WM_IsVisible(hItem1))
        {
            WM_ShowWindow(hItem1);
        }
        ButtonExCtrl_SetFontInfo(hItem1,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_IE),"");
#if (LOGO_FLAG == ENDURE_LOGO)
		ButtonExCtrl_SetButtonDataInfo(hItem1,0,28,Parameter->SettingIEValue,1,0);
#else
		ButtonExCtrl_SetButtonDataInfo(hItem1,0,24,Parameter->SettingIEValue,1,0);
#endif
        ButtonExCtrl_SetData(hItem1,Parameter->SettingIEValue);
        if(!WM_IsVisible(hItem2))
        {
            WM_ShowWindow(hItem2);
        }
        ButtonExCtrl_SetFontInfo(hItem2,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_F),Sys_Lib_GetDispStr(DP_BPM));//bmp cpp md 201328
        ButtonExCtrl_SetButtonDataInfo(hItem2,2,100,Parameter->SettingotherRateValue,1,0);//1->4, cpp md 20130328 //40->100 cpp md 20130328
        ButtonExCtrl_SetData(hItem2,Parameter->SettingotherRateValue);//SettingSimvRateValue cpp md 20130328
        if(WM_IsVisible(hItem3))
        {
            WM_HideWindow(hItem3);
        }

        if(!WM_IsVisible(hItem4)) //  20130607
        {
            WM_ShowWindow(hItem4);
        }
        ButtonExCtrl_SetFontInfo(hItem4,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_TSLOPE),Sys_Lib_GetDispStr(DP_S));
        ButtonExCtrl_SetButtonDataInfo(hItem4,0,200,Parameter->SettingTslopeValue,10,2);
        if(Parameter->SettingTiValue < Parameter->SettingTslopeValue) //  20130618
        {
            Parameter->SettingTslopeValue = Parameter->SettingTiValue - Parameter->SettingTiValue%10;
        }
        ButtonExCtrl_SetData(hItem4,Parameter->SettingTslopeValue);
        
        if(WM_IsVisible(hItem5)) //  20130523
        {
            WM_HideWindow(hItem5);
        }
		if(!WM_IsVisible(hItem6))
        {
            WM_ShowWindow(hItem6);
        }
        ButtonExCtrl_SetFontInfo(hItem6,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_PEEP),Sys_Lib_GetDispStr(DP_CMH2O));
        ButtonExCtrl_SetButtonDataInfo(hItem6,0,30,Parameter->SettingPEEPValue,1,0);//2 cpp md 20130328
        if(Parameter->SettingPinspValue + Parameter->SettingPEEPValue >= AlarmSettingPara.PressureHighValue)
        {
            Parameter->SettingPEEPValue = AlarmSettingPara.PressureHighValue - BeathSettingPara.SettingPinspValue - 1;//5
        }
        ButtonExCtrl_SetData(hItem6,Parameter->SettingPEEPValue);
		if(WM_IsVisible(hItem7)) //sun add 20140623
        {
            WM_HideWindow(hItem7);
        }
        break;
        
    case PRVC_MODE:
        if(!WM_IsVisible(hItem0))
        {
            WM_ShowWindow(hItem0);
        }
        ButtonExCtrl_SetFontInfo(hItem0,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_VT),Sys_Lib_GetDispStr(DP_ML));
		if(Parameter->SettingVtValue >= 100)
		{
			ButtonExCtrl_SetButtonDataInfo(hItem0,Vt_LowLimit,Vt_HighLimit,Parameter->SettingVtValue,10,0);
		}
		else
		{
			ButtonExCtrl_SetButtonDataInfo(hItem0,Vt_LowLimit,Vt_HighLimit,Parameter->SettingVtValue,5,0);
		}
        ButtonExCtrl_SetData(hItem0,Parameter->SettingVtValue);
        if(!WM_IsVisible(hItem1))
        {
            WM_ShowWindow(hItem1);
        }
        ButtonExCtrl_SetFontInfo(hItem1,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_IE),"");
#if (LOGO_FLAG == ENDURE_LOGO)
		ButtonExCtrl_SetButtonDataInfo(hItem1,0,28,Parameter->SettingIEValue,1,0);
#else
		ButtonExCtrl_SetButtonDataInfo(hItem1,0,24,Parameter->SettingIEValue,1,0);
#endif
        ButtonExCtrl_SetData(hItem1,Parameter->SettingIEValue);//
        if(!WM_IsVisible(hItem2))
        {
            WM_ShowWindow(hItem2);
        }
        ButtonExCtrl_SetFontInfo(hItem2,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_F),Sys_Lib_GetDispStr(DP_BPM));//bmp cpp md 201328
        ButtonExCtrl_SetButtonDataInfo(hItem2,2,100,Parameter->SettingotherRateValue,1,0);//1->4, cpp md 20130328 //40->100 cpp md 20130328
        ButtonExCtrl_SetData(hItem2,Parameter->SettingotherRateValue);//SettingSimvRateValue cpp md 20130328
        if(WM_IsVisible(hItem3))
        {
            WM_HideWindow(hItem3);
        }

        if(!WM_IsVisible(hItem4)) //  20130607
        {
            WM_ShowWindow(hItem4);
        }
        ButtonExCtrl_SetFontInfo(hItem4,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_TSLOPE),Sys_Lib_GetDispStr(DP_S));
        ButtonExCtrl_SetButtonDataInfo(hItem4,0,200,Parameter->SettingTslopeValue,10,2);
        ButtonExCtrl_SetData(hItem4,Parameter->SettingTslopeValue);
        
        if(WM_IsVisible(hItem5)) //  20130523
        {
            WM_HideWindow(hItem5);
        }  
		if(!WM_IsVisible(hItem6))
        {
            WM_ShowWindow(hItem6);
        }
        ButtonExCtrl_SetFontInfo(hItem6,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_PEEP),Sys_Lib_GetDispStr(DP_CMH2O));
        ButtonExCtrl_SetButtonDataInfo(hItem6,2,30,Parameter->SettingPEEPValue,1,0);
        ButtonExCtrl_SetData(hItem6,Parameter->SettingPEEPValue);
		if(WM_IsVisible(hItem7)) //sun add 20140623
        {
            WM_HideWindow(hItem7);
        }
        break;
        
    case SIMV_VCV_MODE:
        if(!WM_IsVisible(hItem0))
        {
            WM_ShowWindow(hItem0);
        }
        ButtonExCtrl_SetFontInfo(hItem0,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_VT),Sys_Lib_GetDispStr(DP_ML));
		if(Parameter->SettingVtValue >= 100)
		{
			ButtonExCtrl_SetButtonDataInfo(hItem0,Vt_LowLimit,Vt_HighLimit,Parameter->SettingVtValue,10,0);
		}
		else
		{
			ButtonExCtrl_SetButtonDataInfo(hItem0,Vt_LowLimit,Vt_HighLimit,Parameter->SettingVtValue,5,0);
		}
        ButtonExCtrl_SetData(hItem0,Parameter->SettingVtValue);
        if(!WM_IsVisible(hItem1))
        {
            WM_ShowWindow(hItem1);
        }
        ButtonExCtrl_SetFontInfo(hItem1,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_TINSP),Sys_Lib_GetDispStr(DP_S));
//        Parameter->SettingTiValue = (int)(Parameter->SettingTiValue/10);//  20130422
//        Parameter->SettingTiValue = (int)(Parameter->SettingTiValue*10);//  20130422
        ButtonExCtrl_SetButtonDataInfo(hItem1,10,1000,Parameter->SettingTiValue,10,2);//1,100,Parameter->SettingTiValue 1,1 cpp md 20130411
        ButtonExCtrl_SetData(hItem1,Parameter->SettingTiValue);
        if(!WM_IsVisible(hItem2))
        {
            WM_ShowWindow(hItem2);
        }
        ButtonExCtrl_SetFontInfo(hItem2,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_F),Sys_Lib_GetDispStr(DP_BPM));//bmp cpp md 201328
        if(6000 / Parameter->SettingSimvRateValue < Parameter->SettingTiValue) //  20130515
            Parameter->SettingSimvRateValue = Parameter->SettingRateValue;
        ButtonExCtrl_SetButtonDataInfo(hItem2,1,40,Parameter->SettingSimvRateValue,1,0);
        ButtonExCtrl_SetData(hItem2,Parameter->SettingSimvRateValue);
        
        if(!WM_IsVisible(hItem3))
        {
            WM_ShowWindow(hItem3);
        }
        ButtonExCtrl_SetFontInfo(hItem3,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_PSUPP),Sys_Lib_GetDispStr(DP_CMH2O));
        if(Parameter->SettingPsupportValue >= AlarmSettingPara.PressureHighValue)//PEEP最小值为2
        {  //2013-12-16
            Parameter->SettingPsupportValue = AlarmSettingPara.PressureHighValue - 1;
            ButtonExCtrl_SetButtonDataInfo(hItem3,0,70,Parameter->SettingPsupportValue,1,0);//60->70 cpp md 20130417 //5->0 cpp md 20130723
        }
        else 
        {
            ButtonExCtrl_SetButtonDataInfo(hItem3,0,70,Parameter->SettingPsupportValue,1,0);//60->70 cpp md 20130417 //5->0 cpp md 20130723
        }        
        //*//  20130523
        if(Parameter->SettingPsupportValue + Parameter->SettingPEEPValue >= AlarmSettingPara.PressureHighValue)
        {
            Parameter->SettingPsupportValue = AlarmSettingPara.PressureHighValue - BeathSettingPara.SettingPEEPValue - 1;
            if(Parameter->SettingPsupportValue <= 0)  Parameter->SettingPsupportValue = 0; //5->0 cpp md 20130723
        }//*/
        ButtonExCtrl_SetData(hItem3,Parameter->SettingPsupportValue);

        if(!WM_IsVisible(hItem4)) //  20130607
        {
            WM_ShowWindow(hItem4);
        }
        ButtonExCtrl_SetFontInfo(hItem4,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_TSLOPE),Sys_Lib_GetDispStr(DP_S));
        ButtonExCtrl_SetButtonDataInfo(hItem4,0,200,Parameter->SettingTslopeValue,10,2);
        ButtonExCtrl_SetData(hItem4,Parameter->SettingTslopeValue);
        
        if(!WM_IsVisible(hItem5))
        {
            WM_ShowWindow(hItem5);
        }
        if(Parameter->TriggerMode==FLOW_TRIGGER)
        {
            ButtonExCtrl_SetFontInfo(hItem5,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_FSENS),Sys_Lib_GetDispStr(DP_LPM));//F-trigger //cpp md 20130327
#if(LOGO_FLAG == PENLON_LOGO)//ylk add 20180709
			ButtonExCtrl_SetButtonDataInfo(hItem5,10,200,Parameter->SettingFtrValue,1,1);
#else
			ButtonExCtrl_SetButtonDataInfo(hItem5,5,200,Parameter->SettingFtrValue,1,1);
#endif
            ButtonExCtrl_SetData(hItem5,Parameter->SettingFtrValue);
        }
        else
        {
            ButtonExCtrl_SetFontInfo(hItem5,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_PSENS),Sys_Lib_GetDispStr(DP_CMH2O));//P-trigger //cpp md 20130327
//#if(LOGO_FLAG == PENLON_LOGO)//ylk add 20180709
//			ButtonExCtrl_SetButtonDataInfo(hItem5,1,20,Parameter->SettingPtrValue,1,0);
//#elif(LOGO_FLAG == HOSPITECH_LOGO)
			ButtonExCtrl_SetButtonDataInfo(hItem5,-20,0,Parameter->SettingPtrValue,1,0);
//#else
//			ButtonExCtrl_SetButtonDataInfo(hItem5,0,20,Parameter->SettingPtrValue,1,0);
//#endif
            ButtonExCtrl_SetData(hItem5,Parameter->SettingPtrValue);
        }
        
        //*//  20130328
		if(!WM_IsVisible(hItem6))
        {
            WM_ShowWindow(hItem6);
        }
        ButtonExCtrl_SetFontInfo(hItem6,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_PEEP),Sys_Lib_GetDispStr(DP_CMH2O));
        ButtonExCtrl_SetButtonDataInfo(hItem6,2,30,Parameter->SettingPEEPValue,1,0);
        if(Parameter->SettingPsupportValue + Parameter->SettingPEEPValue >= AlarmSettingPara.PressureHighValue)
        {
            Parameter->SettingPEEPValue = AlarmSettingPara.PressureHighValue - BeathSettingPara.SettingPsupportValue - 1;//5
        }        
        ButtonExCtrl_SetData(hItem6,Parameter->SettingPEEPValue);

		//sun add 20140623
		if(!WM_IsVisible(hItem7)) //sun add 20140623
        {
            WM_ShowWindow(hItem7);
        }
        ButtonExCtrl_SetFontInfo(hItem7,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_ESENS),Sys_Lib_GetDispStr(DP_PERCENT));
        ButtonExCtrl_SetButtonDataInfo(hItem7,5,80,Parameter->EsenseValue,5,0);
		ButtonExCtrl_SetData(hItem7,Parameter->EsenseValue);
		//*/
        break;
        
    case SIMV_PCV_MODE:
        if(!WM_IsVisible(hItem0))
        {
            WM_ShowWindow(hItem0);
        }
        ButtonExCtrl_SetFontInfo(hItem0,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_PINSP),Sys_Lib_GetDispStr(DP_CMH2O));
        ButtonExCtrl_SetButtonDataInfo(hItem0,5,70,Parameter->SettingPinspValue,1,0);
        //*//  20130523
        if(Parameter->SettingPinspValue + Parameter->SettingPEEPValue >= AlarmSettingPara.PressureHighValue)
        {
            Parameter->SettingPinspValue = AlarmSettingPara.PressureHighValue - BeathSettingPara.SettingPEEPValue - 1;//5
            if(Parameter->SettingPinspValue <= 5)  Parameter->SettingPinspValue = 5;
        }//*/        
        ButtonExCtrl_SetData(hItem0,Parameter->SettingPinspValue);
        if(!WM_IsVisible(hItem1))
        {
            WM_ShowWindow(hItem1);
        }
        ButtonExCtrl_SetFontInfo(hItem1,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_TINSP),Sys_Lib_GetDispStr(DP_S));
//        Parameter->SettingTiValue = (int)(Parameter->SettingTiValue/10);//  20130422
//        Parameter->SettingTiValue = (int)(Parameter->SettingTiValue*10);//  20130422
        ButtonExCtrl_SetButtonDataInfo(hItem1,10,1000,Parameter->SettingTiValue,10,2);//Parameter->SettingTiValue cpp md 20130411
        ButtonExCtrl_SetData(hItem1,Parameter->SettingTiValue);
        if(!WM_IsVisible(hItem2))
        {
            WM_ShowWindow(hItem2);
        }
        ButtonExCtrl_SetFontInfo(hItem2,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_F),Sys_Lib_GetDispStr(DP_BPM));//bmp cpp md 20130328    
        if(6000 / Parameter->SettingSimvRateValue < Parameter->SettingTiValue) //  20130515
            Parameter->SettingSimvRateValue = Parameter->SettingRateValue;
        
        ButtonExCtrl_SetButtonDataInfo(hItem2,1,40,Parameter->SettingSimvRateValue,1,0);
        ButtonExCtrl_SetData(hItem2,Parameter->SettingSimvRateValue);
        if(!WM_IsVisible(hItem3))
        {
            WM_ShowWindow(hItem3);
        }
        ButtonExCtrl_SetFontInfo(hItem3,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_PSUPP),Sys_Lib_GetDispStr(DP_CMH2O));
        ButtonExCtrl_SetButtonDataInfo(hItem3,0,70,Parameter->SettingPsupportValue,1,0);//60->70 cpp md 20130417 //5->0 cpp md 20130723
//         if(Parameter->SettingPsupportValue > Parameter->SettingPinspValue) //  20130523
//             Parameter->SettingPsupportValue = Parameter->SettingPinspValue;
        ButtonExCtrl_SetData(hItem3,Parameter->SettingPsupportValue);


        if(!WM_IsVisible(hItem4)) //  20130607
        {
            WM_ShowWindow(hItem4);
        }
        ButtonExCtrl_SetFontInfo(hItem4,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_TSLOPE),Sys_Lib_GetDispStr(DP_S));
        ButtonExCtrl_SetButtonDataInfo(hItem4,0,200,Parameter->SettingTslopeValue,10,2);
        ButtonExCtrl_SetData(hItem4,Parameter->SettingTslopeValue);
        
        if(!WM_IsVisible(hItem5))
        {
            WM_ShowWindow(hItem5);
        }
		if(Parameter->TriggerMode==FLOW_TRIGGER)
		{
			ButtonExCtrl_SetFontInfo(hItem5,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_FSENS),Sys_Lib_GetDispStr(DP_LPM));//F-trigger //cpp md 20130327
#if(LOGO_FLAG == PENLON_LOGO)//ylk add 20180709
			ButtonExCtrl_SetButtonDataInfo(hItem5,10,200,Parameter->SettingFtrValue,1,1);
#else
			ButtonExCtrl_SetButtonDataInfo(hItem5,5,200,Parameter->SettingFtrValue,1,1);
#endif
			ButtonExCtrl_SetData(hItem5,Parameter->SettingFtrValue);
		}
		else
		{
			ButtonExCtrl_SetFontInfo(hItem5,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_PSENS),Sys_Lib_GetDispStr(DP_CMH2O));//P-trigger //cpp md 20130327
//#if(LOGO_FLAG == PENLON_LOGO)//ylk add 20180709
//			ButtonExCtrl_SetButtonDataInfo(hItem5,1,20,Parameter->SettingPtrValue,1,0);
//#elif(LOGO_FLAG == HOSPITECH_LOGO)
			ButtonExCtrl_SetButtonDataInfo(hItem5,-20,0,Parameter->SettingPtrValue,1,0);
//#else
//			ButtonExCtrl_SetButtonDataInfo(hItem5,0,20,Parameter->SettingPtrValue,1,0);
//#endif
			ButtonExCtrl_SetData(hItem5,Parameter->SettingPtrValue);
		}
        //*//  20130328
		if(!WM_IsVisible(hItem6))
        {
            WM_ShowWindow(hItem6);
        }
        ButtonExCtrl_SetFontInfo(hItem6,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_PEEP),Sys_Lib_GetDispStr(DP_CMH2O));
        ButtonExCtrl_SetButtonDataInfo(hItem6,2,30,Parameter->SettingPEEPValue,1,0);
        if(Parameter->SettingPinspValue + Parameter->SettingPEEPValue >= AlarmSettingPara.PressureHighValue)
        {
            Parameter->SettingPEEPValue = AlarmSettingPara.PressureHighValue - BeathSettingPara.SettingPinspValue - 1;//5
        }        
        ButtonExCtrl_SetData(hItem6,Parameter->SettingPEEPValue);		

		//sun add 20140623
		if(!WM_IsVisible(hItem7)) //sun add 20140623
        {
            WM_ShowWindow(hItem7);
        }
        ButtonExCtrl_SetFontInfo(hItem7,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_ESENS),Sys_Lib_GetDispStr(DP_PERCENT));
        ButtonExCtrl_SetButtonDataInfo(hItem7,5,80,Parameter->EsenseValue,5,0);
		ButtonExCtrl_SetData(hItem7,Parameter->EsenseValue);
        //*/
        break;
        
    case SIMV_PRVC_MODE:
        if(!WM_IsVisible(hItem0))
        {
            WM_ShowWindow(hItem0);
        }
        ButtonExCtrl_SetFontInfo(hItem0,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_VT),Sys_Lib_GetDispStr(DP_ML));
		if(Parameter->SettingVtValue >= 100)
		{
			ButtonExCtrl_SetButtonDataInfo(hItem0,Vt_LowLimit,Vt_HighLimit,Parameter->SettingVtValue,10,0);
		}
		else
		{
			ButtonExCtrl_SetButtonDataInfo(hItem0,Vt_LowLimit,Vt_HighLimit,Parameter->SettingVtValue,5,0);
		}
        ButtonExCtrl_SetData(hItem0,Parameter->SettingVtValue);
        if(!WM_IsVisible(hItem1))
        {
            WM_ShowWindow(hItem1);
        }
        ButtonExCtrl_SetFontInfo(hItem1,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_TINSP),Sys_Lib_GetDispStr(DP_S));
//        Parameter->SettingTiValue = (int)(Parameter->SettingTiValue/10);//  20130422
//        Parameter->SettingTiValue = (int)(Parameter->SettingTiValue*10);//  20130422        
        ButtonExCtrl_SetButtonDataInfo(hItem1,10,1000,Parameter->SettingTiValue,10,2);//Parameter->SettingTiValue cpp md 20130411
        ButtonExCtrl_SetData(hItem1,Parameter->SettingTiValue);
        if(!WM_IsVisible(hItem2))
        {
            WM_ShowWindow(hItem2);
        }
        ButtonExCtrl_SetFontInfo(hItem2,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_F),Sys_Lib_GetDispStr(DP_BPM));//bmp cpp md 20130328 
        if(6000 / Parameter->SettingSimvRateValue < Parameter->SettingTiValue) //  20130515
            Parameter->SettingSimvRateValue = Parameter->SettingRateValue;
        
        ButtonExCtrl_SetButtonDataInfo(hItem2,1,40,Parameter->SettingSimvRateValue,1,0);
        ButtonExCtrl_SetData(hItem2,Parameter->SettingSimvRateValue);
        if(!WM_IsVisible(hItem3))
        {
            WM_ShowWindow(hItem3);
        }
        ButtonExCtrl_SetFontInfo(hItem3,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_PSUPP),Sys_Lib_GetDispStr(DP_CMH2O));
        ButtonExCtrl_SetButtonDataInfo(hItem3,0,70,Parameter->SettingPsupportValue,1,0); //5->0 cpp md 20130723
        //*//  20130523
        if(Parameter->SettingPsupportValue + Parameter->SettingPEEPValue >= AlarmSettingPara.PressureHighValue)
        {
            Parameter->SettingPsupportValue = AlarmSettingPara.PressureHighValue - BeathSettingPara.SettingPEEPValue - 1;
            if(Parameter->SettingPsupportValue <= 0)  Parameter->SettingPsupportValue = 0; //5->0 cpp md 20130723
        }//*/
        ButtonExCtrl_SetData(hItem3,Parameter->SettingPsupportValue);

        if(!WM_IsVisible(hItem4)) //  20130607
        {
            WM_ShowWindow(hItem4);
        }
        ButtonExCtrl_SetFontInfo(hItem4,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_TSLOPE),Sys_Lib_GetDispStr(DP_S));
        ButtonExCtrl_SetButtonDataInfo(hItem4,0,200,Parameter->SettingTslopeValue,10,2);
        ButtonExCtrl_SetData(hItem4,Parameter->SettingTslopeValue);
                
        if(!WM_IsVisible(hItem5))
        {
            WM_ShowWindow(hItem5);
        }
		if(Parameter->TriggerMode==FLOW_TRIGGER)
		{
			ButtonExCtrl_SetFontInfo(hItem5,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_FSENS),Sys_Lib_GetDispStr(DP_LPM));//F-trigger //cpp md 20130327
#if(LOGO_FLAG == PENLON_LOGO)//ylk add 20180709
			ButtonExCtrl_SetButtonDataInfo(hItem5,10,200,Parameter->SettingFtrValue,1,1);
#else
			ButtonExCtrl_SetButtonDataInfo(hItem5,5,200,Parameter->SettingFtrValue,1,1);
#endif
			ButtonExCtrl_SetData(hItem5,Parameter->SettingFtrValue);
		}
		else
		{
			ButtonExCtrl_SetFontInfo(hItem5,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_PSENS),Sys_Lib_GetDispStr(DP_CMH2O));//P-trigger //cpp md 20130327
//#if(LOGO_FLAG == PENLON_LOGO)//ylk add 20180709
//			ButtonExCtrl_SetButtonDataInfo(hItem5,1,20,Parameter->SettingPtrValue,1,0);
//#elif(LOGO_FLAG == HOSPITECH_LOGO)
			ButtonExCtrl_SetButtonDataInfo(hItem5,-20,0,Parameter->SettingPtrValue,1,0);
//#else
//			ButtonExCtrl_SetButtonDataInfo(hItem5,0,20,Parameter->SettingPtrValue,1,0);
//#endif
			ButtonExCtrl_SetData(hItem5,Parameter->SettingPtrValue);
		}
		if(!WM_IsVisible(hItem6))
        {
            WM_ShowWindow(hItem6);
        }
        ButtonExCtrl_SetFontInfo(hItem6,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_PEEP),Sys_Lib_GetDispStr(DP_CMH2O));
        ButtonExCtrl_SetButtonDataInfo(hItem6,2,30,Parameter->SettingPEEPValue,1,0);
        if(Parameter->SettingPsupportValue + Parameter->SettingPEEPValue >= AlarmSettingPara.PressureHighValue)
        {
            Parameter->SettingPEEPValue = AlarmSettingPara.PressureHighValue - BeathSettingPara.SettingPsupportValue - 1;//5
        }        
        ButtonExCtrl_SetData(hItem6,Parameter->SettingPEEPValue);	

		//sun add 20140623
		if(!WM_IsVisible(hItem7)) //sun add 20140623
        {
            WM_ShowWindow(hItem7);
        }
        ButtonExCtrl_SetFontInfo(hItem7,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_ESENS),Sys_Lib_GetDispStr(DP_PERCENT));
        ButtonExCtrl_SetButtonDataInfo(hItem7,5,80,Parameter->EsenseValue,5,0);
		ButtonExCtrl_SetData(hItem7,Parameter->EsenseValue);
        break;
        
    case PSV_MODE:
        if(WM_IsVisible(hItem0))
        {
            WM_HideWindow(hItem0);
        }
        if(WM_IsVisible(hItem1))
        {
            WM_HideWindow(hItem1);
        }
        if(WM_IsVisible(hItem2))
        {
            WM_HideWindow(hItem2);
        }
        if(!WM_IsVisible(hItem3))
        {
            WM_ShowWindow(hItem3);
        }
        ButtonExCtrl_SetFontInfo(hItem3,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_PSUPP),Sys_Lib_GetDispStr(DP_CMH2O));
        ButtonExCtrl_SetButtonDataInfo(hItem3,0,70,Parameter->SettingPsupportValue,1,0); //5->0 cpp md 20130723
        //*//  20130523
        if(  Parameter->SettingPsupportValue + Parameter->SettingPEEPValue >= AlarmSettingPara.PressureHighValue )
        {
            Parameter->SettingPsupportValue = AlarmSettingPara.PressureHighValue - BeathSettingPara.SettingPEEPValue - 1;
            if(Parameter->SettingPsupportValue <= 0)  Parameter->SettingPsupportValue = 0; //5->0 cpp md 20130723
        }//*/
        ButtonExCtrl_SetData(hItem3,Parameter->SettingPsupportValue);

        if(!WM_IsVisible(hItem4)) //  20130607
        {
            WM_ShowWindow(hItem4);
        }
        ButtonExCtrl_SetFontInfo(hItem4,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_TSLOPE),Sys_Lib_GetDispStr(DP_S));
        ButtonExCtrl_SetButtonDataInfo(hItem4,0,200,Parameter->SettingTslopeValue,10,2);
        ButtonExCtrl_SetData(hItem4,Parameter->SettingTslopeValue);
        
        if(!WM_IsVisible(hItem5))
        {
            WM_ShowWindow(hItem5);
        }
		if(Parameter->TriggerMode==FLOW_TRIGGER)
		{
			ButtonExCtrl_SetFontInfo(hItem5,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_FSENS),Sys_Lib_GetDispStr(DP_LPM));//F-trigger //cpp md 20130327
#if(LOGO_FLAG == PENLON_LOGO)//ylk add 20180709
			ButtonExCtrl_SetButtonDataInfo(hItem5,10,200,Parameter->SettingFtrValue,1,1);
#else
			ButtonExCtrl_SetButtonDataInfo(hItem5,5,200,Parameter->SettingFtrValue,1,1);
#endif
			ButtonExCtrl_SetData(hItem5,Parameter->SettingFtrValue);
		}
		else
		{
			ButtonExCtrl_SetFontInfo(hItem5,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_PSENS),Sys_Lib_GetDispStr(DP_CMH2O));//P-trigger //cpp md 20130327
//#if(LOGO_FLAG == PENLON_LOGO)
//			ButtonExCtrl_SetButtonDataInfo(hItem5,1,20,Parameter->SettingPtrValue,1,0);
//#elif(LOGO_FLAG == HOSPITECH_LOGO)
			ButtonExCtrl_SetButtonDataInfo(hItem5,-20,0,Parameter->SettingPtrValue,1,0);
//#else
//			ButtonExCtrl_SetButtonDataInfo(hItem5,0,20,Parameter->SettingPtrValue,1,0);
//#endif
			ButtonExCtrl_SetData(hItem5,Parameter->SettingPtrValue);
		}
        //*//  20130328
		if(!WM_IsVisible(hItem6))
        {
            WM_ShowWindow(hItem6);
        }
        ButtonExCtrl_SetFontInfo(hItem6,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_PEEP),Sys_Lib_GetDispStr(DP_CMH2O));
        ButtonExCtrl_SetButtonDataInfo(hItem6,2,30,Parameter->SettingPEEPValue,1,0);
        if(Parameter->SettingPsupportValue + Parameter->SettingPEEPValue >= AlarmSettingPara.PressureHighValue)
        {
            Parameter->SettingPEEPValue = AlarmSettingPara.PressureHighValue - BeathSettingPara.SettingPsupportValue - 1;//5
        }        
        ButtonExCtrl_SetData(hItem6,Parameter->SettingPEEPValue);


		//sun add 20140623
		if(!WM_IsVisible(hItem7)) //sun add 20140623
        {
            WM_ShowWindow(hItem7);
        }
        ButtonExCtrl_SetFontInfo(hItem7,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_ESENS),Sys_Lib_GetDispStr(DP_PERCENT));
        ButtonExCtrl_SetButtonDataInfo(hItem7,5,80,Parameter->EsenseValue,5,0);
		ButtonExCtrl_SetData(hItem7,Parameter->EsenseValue);
        //*/
        break;

    case ACGO_MODE:        
    case MANUAL_MODE:
//         if(WM_IsVisible(hItem0))
//         {
//             WM_HideWindow(hItem0);
//         }
//         if(WM_IsVisible(hItem1))
//         {
//             WM_HideWindow(hItem1);
//         }
//         if(WM_IsVisible(hItem2))
//         {
//             WM_HideWindow(hItem2);
//         }
//         if(WM_IsVisible(hItem3))
//         {
//             WM_HideWindow(hItem3);
//         }
//         if(WM_IsVisible(hItem4)) //  20130607
//         {
//             WM_HideWindow(hItem4);
//         }       
//         if(WM_IsVisible(hItem5))
//         {
//             WM_HideWindow(hItem5);
//         }
//         if(WM_IsVisible(hItem6))
//         {
//             WM_HideWindow(hItem6);
//         }
// 		if(WM_IsVisible(hItem7)) //sun add 20140623
//         {
//             WM_HideWindow(hItem7);
//         }
		break;
	case VS_MODE:
		if (!WM_IsVisible(hItem0))
		{
			WM_ShowWindow(hItem0);
		}
		ButtonExCtrl_SetFontInfo(hItem0,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_VT),Sys_Lib_GetDispStr(DP_ML));
		if(Parameter->SettingVtSupportValue >= 100)
		{
			ButtonExCtrl_SetButtonDataInfo(hItem0,Vt_LowLimit,Vt_HighLimit,Parameter->SettingVtSupportValue,10,0);
		}
		else
		{
			ButtonExCtrl_SetButtonDataInfo(hItem0,Vt_LowLimit,Vt_HighLimit,Parameter->SettingVtSupportValue,5,0);
		}
		ButtonExCtrl_SetData(hItem0,Parameter->SettingVtSupportValue);
		//
		if(!WM_IsVisible(hItem2))
		{
			WM_ShowWindow(hItem2);
		}
		ButtonExCtrl_SetFontInfo(hItem2,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_F),Sys_Lib_GetDispStr(DP_BPM));//bmp cpp md 20130328     
		ButtonExCtrl_SetButtonDataInfo(hItem2,2,60,Parameter->SettingVSRateValue,1,0);
		ButtonExCtrl_SetData(hItem2,Parameter->SettingVSRateValue);

		if(!WM_IsVisible(hItem3))
		{
			WM_ShowWindow(hItem3);
		}
		ButtonExCtrl_SetFontInfo(hItem3,GUI_WHITE,GUI_WHITE,"PLimit",Sys_Lib_GetDispStr(DP_CMH2O));     
		ButtonExCtrl_SetButtonDataInfo(hItem3,10,100,Parameter->SettingPHLimitValue,1,0);
		ButtonExCtrl_SetData(hItem3,Parameter->SettingPHLimitValue);

		if(!WM_IsVisible(hItem4)) //  20130607
		{
			WM_ShowWindow(hItem4);
		}
		ButtonExCtrl_SetFontInfo(hItem4,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_TSLOPE),Sys_Lib_GetDispStr(DP_S));
		ButtonExCtrl_SetButtonDataInfo(hItem4,0,200,Parameter->SettingTslopeValue,10,2);
		ButtonExCtrl_SetData(hItem4,Parameter->SettingTslopeValue);

		if(!WM_IsVisible(hItem5))
		{
			WM_ShowWindow(hItem5);
		}
		if(Parameter->TriggerMode==FLOW_TRIGGER)
		{
			ButtonExCtrl_SetFontInfo(hItem5,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_FSENS),Sys_Lib_GetDispStr(DP_LPM));//F-trigger //cpp md 20130327
			ButtonExCtrl_SetButtonDataInfo(hItem5,5,200,Parameter->SettingFtrValue,1,1);
			ButtonExCtrl_SetData(hItem5,Parameter->SettingFtrValue);
		}
		else
		{
			ButtonExCtrl_SetFontInfo(hItem5,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_PSENS),Sys_Lib_GetDispStr(DP_CMH2O));//P-trigger //cpp md 20130327
			ButtonExCtrl_SetButtonDataInfo(hItem5,-20,0,Parameter->SettingPtrValue,1,0);
			ButtonExCtrl_SetData(hItem5,Parameter->SettingPtrValue);
		}

		if(!WM_IsVisible(hItem6))
		{
			WM_ShowWindow(hItem6);
		}
		ButtonExCtrl_SetFontInfo(hItem6,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_PEEP),Sys_Lib_GetDispStr(DP_CMH2O));
		ButtonExCtrl_SetButtonDataInfo(hItem6,2,50,Parameter->SettingPEEPValue,1,0);
		ButtonExCtrl_SetData(hItem6, Parameter->SettingPEEPValue);

		if(!WM_IsVisible(hItem7)) //sun add 20140623
		{
			WM_ShowWindow(hItem7);
		}
		ButtonExCtrl_SetFontInfo(hItem7,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_ESENS),Sys_Lib_GetDispStr(DP_PERCENT));
		ButtonExCtrl_SetButtonDataInfo(hItem7,5,80,Parameter->EsenseValue,5,0);
		ButtonExCtrl_SetData(hItem7,Parameter->EsenseValue);

		if (WM_IsVisible(hItem1))
		{
			WM_HideWindow(hItem1);
		}
		
		break;
	case APRV_MODE:
		if (!WM_IsVisible(hItem0))
		{
			WM_ShowWindow(hItem0);
		}
		if (!WM_IsVisible(hItem1))
		{
			WM_ShowWindow(hItem1);
		}
		if (!WM_IsVisible(hItem2))
		{
			WM_ShowWindow(hItem2);
		}
		if (!WM_IsVisible(hItem3))
		{
			WM_ShowWindow(hItem3);
		}	
		if (WM_IsVisible(hItem5))
		{
			WM_HideWindow(hItem5);
		}
		if (WM_IsVisible(hItem6))
		{
			WM_HideWindow(hItem6);
		}
		if (WM_IsVisible(hItem7))
		{
			WM_HideWindow(hItem7);
		}

		if(!WM_IsVisible(hItem4)) //  20130607
		{
			WM_ShowWindow(hItem4);
		}
		ButtonExCtrl_SetFontInfo(hItem4,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_TSLOPE),Sys_Lib_GetDispStr(DP_S));
		ButtonExCtrl_SetButtonDataInfo(hItem4,0,200,Parameter->SettingTslopeValue,10,2);
		ButtonExCtrl_SetData(hItem4,Parameter->SettingTslopeValue);

		//高压水平
		ButtonExCtrl_SetButtonDataInfo(hItem0,5, 80,Parameter->SettingPhighValue,1,0);
		ButtonExCtrl_SetData(hItem0,Parameter->SettingPhighValue);
		ButtonExCtrl_SetFontInfo(hItem0,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_PHIGH),Sys_Lib_GetDispStr(DP_CMH2O));
		//低压水平
		ButtonExCtrl_SetFontInfo(hItem1, GUI_WHITE, GUI_WHITE, Sys_Lib_GetDispStr(DP_PLOW),Sys_Lib_GetDispStr(DP_CMH2O));
		ButtonExCtrl_SetButtonDataInfo(hItem1, 2, 50, Parameter->SettingPlowValue, 1, 0);
		ButtonExCtrl_SetData(hItem1, Parameter->SettingPlowValue);
		//高压时间
		ButtonExCtrl_SetFontInfo(hItem2, GUI_WHITE, GUI_WHITE, Sys_Lib_GetDispStr(DP_THIGH), Sys_Lib_GetDispStr(DP_S)); 
		ButtonExCtrl_SetButtonDataInfo(hItem2, 2, 100, Parameter->SettingThighValue, 1, 1);                                 
		ButtonExCtrl_SetData(hItem2, Parameter->SettingThighValue);                                                      
		//低压时间
		ButtonExCtrl_SetFontInfo(hItem3, GUI_WHITE, GUI_WHITE, Sys_Lib_GetDispStr(DP_TLOW), Sys_Lib_GetDispStr(DP_S)); 
		ButtonExCtrl_SetButtonDataInfo(hItem3, 3, 100, Parameter->SettingTlowValue, 1, 1);                            
		ButtonExCtrl_SetData(hItem3, Parameter->SettingTlowValue);                                                         
		break;
	case BIPAP_MODE:
		if (!WM_IsVisible(hItem0))
		{
			WM_ShowWindow(hItem0);
		}
		if (!WM_IsVisible(hItem1))
		{
			WM_ShowWindow(hItem1);
		}
		if (!WM_IsVisible(hItem2))
		{
			WM_ShowWindow(hItem2);
		}
		if (!WM_IsVisible(hItem3))
		{
			WM_ShowWindow(hItem3);
		}
		if (!WM_IsVisible(hItem4))
		{
			WM_ShowWindow(hItem4);
		}

		if (WM_IsVisible(hItem5))
		{
			WM_HideWindow(hItem5);
		}
		if (WM_IsVisible(hItem6))
		{
			WM_HideWindow(hItem6);
		}
		if (WM_IsVisible(hItem7))
		{
			WM_HideWindow(hItem7);
		}
		//高压水平
		ButtonExCtrl_SetButtonDataInfo(hItem0,5, 80,Parameter->SettingPhighValue,1,0);
		ButtonExCtrl_SetData(hItem0,Parameter->SettingPhighValue);
		ButtonExCtrl_SetFontInfo(hItem0,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_PHIGH),Sys_Lib_GetDispStr(DP_CMH2O));
		//低压水平
		ButtonExCtrl_SetFontInfo(hItem1, GUI_WHITE, GUI_WHITE, Sys_Lib_GetDispStr(DP_PLOW), Sys_Lib_GetDispStr(DP_CMH2O));
		ButtonExCtrl_SetButtonDataInfo(hItem1, 2, 50, Parameter->SettingPlowValue, 1, 0);
		ButtonExCtrl_SetData(hItem1, Parameter->SettingPlowValue);
                                                   
		//呼吸频率
		// 可以用VCV的呼吸频率，发送命令不用
		ButtonExCtrl_SetFontInfo(hItem2,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_F),Sys_Lib_GetDispStr(DP_BPM));
		ButtonExCtrl_SetButtonDataInfo(hItem2,2,100,Parameter->SettingBipapRateValue,1,0);
		ButtonExCtrl_SetData(hItem2,Parameter->SettingBipapRateValue);

		//高压时间
		ButtonExCtrl_SetFontInfo(hItem3, GUI_WHITE, GUI_WHITE, Sys_Lib_GetDispStr(DP_THIGH), Sys_Lib_GetDispStr(DP_S)); 
		ButtonExCtrl_SetButtonDataInfo(hItem3, 2, 100, Parameter->SettingThighValue, 1, 1);                                 
		ButtonExCtrl_SetData(hItem3, Parameter->SettingThighValue);   
		
		// Tslope
		ButtonExCtrl_SetFontInfo(hItem4,GUI_WHITE,GUI_WHITE,Sys_Lib_GetDispStr(DP_TSLOPE),Sys_Lib_GetDispStr(DP_S));
		ButtonExCtrl_SetButtonDataInfo(hItem4,0,200,Parameter->SettingTslopeValue,10,2);
		if(Parameter->SettingTiValue < Parameter->SettingTslopeValue)
		{
			Parameter->SettingTslopeValue = Parameter->SettingTiValue - Parameter->SettingTiValue%10;
		}
		ButtonExCtrl_SetData(hItem4,Parameter->SettingTslopeValue);
		break;
    default:
        break;
    }
}
static void VentParaWinInit(WM_HWIN hDlg,BREATH_SET_PARA * Parameter)
{
	if((Parameter->BreathMode == MANUAL_MODE)||(Parameter->BreathMode == ACGO_MODE))
	{
		VentParaWinInitNormal(hDlg,Parameter,BreathMode_BeforeConfirm);
	}
	else
	{
		VentParaWinInitNormal(hDlg,Parameter,Parameter->BreathMode);
	}
}
/***************************************************************************************************
* @brief  模式键初始化
* @para   
* @retval 
*/
static void ModesKeyInit(WM_HWIN hDialog)
{
    WM_HWIN j;
    //*//  20130426
    j = WM_GetDialogItem(hDialog,GUI_ID_CHECK0);   BUTTON_SetState(j,CHECKBOX_BI_ACTIV);
    j = WM_GetDialogItem(hDialog,GUI_ID_BUTTON2);  BUTTON_SetState(j,0);
    j = WM_GetDialogItem(hDialog,GUI_ID_BUTTON3);  BUTTON_SetState(j,0);
    j = WM_GetDialogItem(hDialog,GUI_ID_BUTTON4);  BUTTON_SetState(j,0);
    j = WM_GetDialogItem(hDialog,GUI_ID_BUTTON5);  BUTTON_SetState(j,0);
    j = WM_GetDialogItem(hDialog,GUI_ID_BUTTON6);  BUTTON_SetState(j,0);
    j = WM_GetDialogItem(hDialog,GUI_ID_BUTTON7);  BUTTON_SetState(j,0);
    j = WM_GetDialogItem(hDialog,GUI_ID_BUTTON8);  BUTTON_SetState(j,0);
//     j = WM_GetDialogItem(hDialog,GUI_ID_BUTTON9);  BUTTON_SetState(j,0);
	j = WM_GetDialogItem(hDialog,GUI_ID_BUTTON10);  BUTTON_SetState(j,0);
	j = WM_GetDialogItem(hDialog,GUI_ID_BUTTON11);  BUTTON_SetState(j,0);
	j = WM_GetDialogItem(hDialog,GUI_ID_BUTTON12);  BUTTON_SetState(j,0);

    //*///
    switch(Breath_Setting_Tmp.BreathMode)//ylk modify 20181009
    {
    case SIGH_MODE:
        j = WM_GetDialogItem(hDialog, GUI_ID_BUTTON2);
        BUTTON_SetState(j,BUTTON_STATE_PRESSED|WIDGET_GetState(j));
        j = WM_GetDialogItem(hDialog, GUI_ID_CHECK0);
        CHECKBOX_SetState(j,CHECKBOX_BI_ACTIV);
		
    case VCV_MODE:
        j = WM_GetDialogItem(hDialog, GUI_ID_BUTTON2);
        BUTTON_SetState(j,BUTTON_STATE_PRESSED|WIDGET_GetState(j));
        j = WM_GetDialogItem(hDialog, GUI_ID_CHECK0);
        WM_SetEnableState(j,1);
        break;
    case PCV_MODE:
        j = WM_GetDialogItem(hDialog, GUI_ID_BUTTON3);
        BUTTON_SetState(j,BUTTON_STATE_PRESSED|WIDGET_GetState(j));
        j = WM_GetDialogItem(hDialog, GUI_ID_CHECK0);
        WM_SetEnableState(j,0);
        CHECKBOX_SetState(j,CHECKBOX_BI_INACTIV);
        break;
    case PRVC_MODE:
        j = WM_GetDialogItem(hDialog, GUI_ID_BUTTON4);
        BUTTON_SetState(j,BUTTON_STATE_PRESSED|WIDGET_GetState(j));
        j = WM_GetDialogItem(hDialog, GUI_ID_CHECK0);
        WM_SetEnableState(j,0);
        CHECKBOX_SetState(j,CHECKBOX_BI_INACTIV);
        break;	
		
    case SIMV_VCV_MODE:
        j = WM_GetDialogItem(hDialog, GUI_ID_BUTTON5);
        BUTTON_SetState(j,BUTTON_STATE_PRESSED|WIDGET_GetState(j));
        j = WM_GetDialogItem(hDialog, GUI_ID_CHECK0);
        WM_SetEnableState(j,0);
        CHECKBOX_SetState(j,CHECKBOX_BI_INACTIV);
        break;
    case SIMV_PCV_MODE:
        j = WM_GetDialogItem(hDialog, GUI_ID_BUTTON6);
        BUTTON_SetState(j,BUTTON_STATE_PRESSED|WIDGET_GetState(j));
        j = WM_GetDialogItem(hDialog, GUI_ID_CHECK0);
        WM_SetEnableState(j,0);
        CHECKBOX_SetState(j,CHECKBOX_BI_INACTIV);
        break;
    case SIMV_PRVC_MODE:
        j = WM_GetDialogItem(hDialog, GUI_ID_BUTTON7);
        BUTTON_SetState(j,BUTTON_STATE_PRESSED|WIDGET_GetState(j));
        j = WM_GetDialogItem(hDialog, GUI_ID_CHECK0);
        WM_SetEnableState(j,0);
        CHECKBOX_SetState(j,CHECKBOX_BI_INACTIV);
        break;
		
    case PSV_MODE:
        j = WM_GetDialogItem(hDialog, GUI_ID_BUTTON8);
        BUTTON_SetState(j,BUTTON_STATE_PRESSED|WIDGET_GetState(j));
        j = WM_GetDialogItem(hDialog, GUI_ID_CHECK0);
        WM_SetEnableState(j,0);
        CHECKBOX_SetState(j,CHECKBOX_BI_INACTIV);
        break;

//     case MANUAL_MODE:
//         j = WM_GetDialogItem(hDialog, GUI_ID_BUTTON9);
//         BUTTON_SetState(j,BUTTON_STATE_PRESSED|WIDGET_GetState(j));
//         j = WM_GetDialogItem(hDialog, GUI_ID_CHECK0);
//         WM_SetEnableState(j,0);
//         CHECKBOX_SetState(j,CHECKBOX_BI_INACTIV);
//         break;
        
	case APRV_MODE:
		j = WM_GetDialogItem(hDialog, GUI_ID_BUTTON10);
		BUTTON_SetState(j,BUTTON_STATE_PRESSED|WIDGET_GetState(j));
		j = WM_GetDialogItem(hDialog, GUI_ID_CHECK0);
		WM_SetEnableState(j,0);
		break;
	case BIPAP_MODE:
		j = WM_GetDialogItem(hDialog, GUI_ID_BUTTON11);
		BUTTON_SetState(j,BUTTON_STATE_PRESSED|WIDGET_GetState(j));
		j = WM_GetDialogItem(hDialog, GUI_ID_CHECK0);
		WM_SetEnableState(j,0);
		break;
	case VS_MODE:
		j = WM_GetDialogItem(hDialog, GUI_ID_BUTTON12);
		BUTTON_SetState(j,BUTTON_STATE_PRESSED|WIDGET_GetState(j));
		j = WM_GetDialogItem(hDialog, GUI_ID_CHECK0);
		WM_SetEnableState(j,0);
		break;
    default:
        break;
    }
}

/***************************************************************************************************
* @brief  HOME菜单创建
* @para   
* @retval 
*/
HOME_MENU_HWIN _HomeMenuWinCreat(void)
{
    WM_HWIN hDialog,hItem;
    HOME_MENU_HWIN hmWin;
    hDialog = GUI_CreateDialogBox(_aDialogCreate22,
                                  GUI_COUNTOF(_aDialogCreate22),
                                  &_cbDialog9, 0, 0, 0);    
    hmWin.ParentWin = hDialog;

    hItem = WM_GetDialogItem(hDialog, GUI_ID_CURVE0);
    hmWin.Tobj[0] = hItem;
    hItem = WM_GetDialogItem(hDialog, GUI_ID_CURVE1);
    hmWin.Tobj[1] = hItem;
    hItem = WM_GetDialogItem(hDialog, GUI_ID_CURVE2);
    hmWin.Tobj[2] = hItem;
	hItem = WM_GetDialogItem(hDialog, GUI_ID_CURVE3);
	hmWin.Tobj[3] = hItem;

    hItem = WM_GetDialogItem(hDialog, GUI_ID_TEXT0);
    hmWin.Robj[0] = hItem;
    hItem = WM_GetDialogItem(hDialog, GUI_ID_TEXT1);
    hmWin.Robj[1] = hItem;
    hItem = WM_GetDialogItem(hDialog, GUI_ID_TEXT2);
    hmWin.Robj[2] = hItem;
    hItem = WM_GetDialogItem(hDialog, GUI_ID_TEXT3);
    hmWin.Robj[3] = hItem;
    hItem = WM_GetDialogItem(hDialog, GUI_ID_TEXT4);
    hmWin.Robj[4] = hItem;
    hItem = WM_GetDialogItem(hDialog, GUI_ID_TEXT5);
    hmWin.Robj[5] = hItem;
	//  [2/16/2022 yaoyao add]第七个快捷监测参数
	hItem = WM_GetDialogItem(hDialog, GUI_ID_TEXT15);
	hmWin.Robj[6] = hItem;
	//
    hItem = WM_GetDialogItem(hDialog, GUI_ID_TEXT7);//MAC
    hmWin.Left_Robj[0] = hItem;
	hItem = WM_GetDialogItem(hDialog, GUI_ID_TEXT9);//AA1
	hmWin.Left_Robj[1] = hItem;
	hItem = WM_GetDialogItem(hDialog, GUI_ID_TEXT10);//AA2
	hmWin.Left_Robj[2] = hItem;
	hItem = WM_GetDialogItem(hDialog, GUI_ID_TEXT11);//N2O
	hmWin.Left_Robj[3] = hItem;
	hItem = WM_GetDialogItem(hDialog, GUI_ID_TEXT12);// CO2
	hmWin.Left_Robj[4] = hItem;

    hItem = WM_GetDialogItem(hDialog, GUI_ID_TEXT6);//压力表
    hmWin.Left_Robj[5] = hItem;

//    hItem = WM_GetDialogItem(hDialog, GUI_ID_TEXT6);//
//    hmWin.Left_Robj[5] = hItem;
//    hItem = WM_GetDialogItem(hDialog, GUI_ID_TEXT13);//Start/Stop
//    hmWin.Left_Robj[6] = hItem;
//    hItem = WM_GetDialogItem(hDialog, GUI_ID_TEXT14);//Clear
//    hmWin.Left_Robj[7] = hItem;

    TracingCurve_AllocDispBuffer(hmWin.Tobj[0],TRACINGCURVE_DATALEN_DEFAULT);
    TracingCurve_AllocDispBuffer(hmWin.Tobj[1],TRACINGCURVE_DATALEN_DEFAULT);
    TracingCurve_AllocDispBuffer(hmWin.Tobj[2],TRACINGCURVE_DATALEN_DEFAULT);
	TracingCurve_AllocDispBuffer(hmWin.Tobj[3],TRACINGCURVE_DATALEN_DEFAULT);

    return(hmWin);
}


/***************************************************************************************************
*氧浓度校验选择氧浓度窗口
*
*
*/
WM_HWIN _AffirmO2Choose(void)  //sun add 20140529
{
    WM_HWIN hDialog;
    hDialog = GUI_CreateDialogBox(_aDialogCreate24,
                                  GUI_COUNTOF(_aDialogCreate24),
                                  &_cbDialog24, 0, 0, 0);
    return(hDialog);
}
/***************************************************************************************************
* @brief  氧浓度校验窗
* @para   
* @retval 
*/
WM_HWIN _OxygenSensorCalWin(void)
{
    WM_HWIN hDialog;
    hDialog = GUI_CreateDialogBox(_aDialogCreate12,
                                  GUI_COUNTOF(_aDialogCreate12),
                                  &_cbDialog21, 0, 0, 0);
    return(hDialog);
}

/***************************************************************************************************
* @brief  密码输入窗
* @para   
* @retval 
*/
WM_HWIN _PasswordInputWin(void)
{
    WM_HWIN hDialog;
    hDialog = GUI_CreateDialogBox(_aDialogCreate10,
                                  GUI_COUNTOF(_aDialogCreate10),
                                  &_cbDialog12, 0, 0, 0);
    return(hDialog);
}
/***************************************************************************************************
* @brief  工程师校验窗
* @para   
* @retval 
*/
WM_HWIN _EngineerCalibrationWin(void)
{
    WM_HWIN hDialog;
    hDialog = GUI_CreateDialogBox(_aDialogCreate11,
                                  GUI_COUNTOF(_aDialogCreate11),
                                  &_cbDialog13, 0, 0, 0);
    return(hDialog);
}
/***************************************************************************************************
* @brief  功能菜单按钮创建
* @para   
* @retval 
*/
WM_HWIN FunctionMenuButtonCreat(void)
{
    WM_HWIN hDlg,hWin;
    hDlg = GUI_CreateDialogBox(_aDialogCreate20,
                               GUI_COUNTOF(_aDialogCreate20),
                               &_cbDialog18, 0, 0, 0);    

    return(hDlg);
}

static void _cbDialogDeskTopPrompt(WM_MESSAGE * pMsg)
{
	WM_HWIN hDlg,hItem;
	static char Paint = 0;
	int NCode, Id;
	OS_PRIO prio;
	hDlg = pMsg->hWin;
	switch (pMsg->MsgId)
	{
	case WM_INIT_DIALOG:
		WINDOW_SetBkColor(hDlg,WINDOW_BK_COLOR);//STANDBY_BK_COLOR		0x410400
		hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT0);
		TEXT_SetTextColor(hItem,GUI_WHITE);
		TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		TEXT_SetTextWrapMode(hItem, GUI_WRAPMODE_WORD);
		break;
	case WM_USER:
 		hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT0);
 		prio = HomePrompt_GetHighestPrio();
 		TEXT_SetText(hItem,HomePrompt_String);
		break;
	default:
		WM_DefaultProc(pMsg);

	}
}
/***************************************************************************************************
* @brief  待机窗创建
* @para   
* @retval 
*/
WM_HWIN _StandbyWinCreat(void)
{
    WM_HWIN hWin;

    hWin = GUI_CreateDialogBox(_aDialogCreate14,
                               GUI_COUNTOF(_aDialogCreate14),
                               &_cbDialog14, 0, 0, 0);
    return(hWin);
}
/***************************************************************************************************
* @brief  开机提示是否自检窗口
* @para   
* @retval 
*/

WM_HWIN _SelfCheck_AskCreat(void)//开机提示是否自检窗口
{
    WM_HWIN hWin;
    hWin = GUI_CreateDialogBox(_aDialogSelfCheck_Ask,       //GUI_FLAG 回调函数需要完善
                             GUI_COUNTOF(_aDialogSelfCheck_Ask),
                             &_cbSelfCheck_Ask, 0, 0, 0);
    return hWin;
}
/***************************************************************************************************
* @brief  
* @para   
* @retval 
*/
WM_HWIN _TrendData_AskCreat(void)
{
	WM_HWIN hWin;
	hWin = GUI_CreateDialogBox(_aDialogSelfCheck_Ask,       
							GUI_COUNTOF(_aDialogSelfCheck_Ask),
							&_cbTrendData_Ask, 0, 0, 0);
	return hWin;
}
/***************************************************************************************************
* @brief  呼吸参数窗创建
* @para   
* @retval 
*/
WM_HWIN _VentParaWinCreat(void)
{
    WM_HWIN hWin;
    hWin = GUI_CreateDialogBox(_aDialogCreate21,
                               GUI_COUNTOF(_aDialogCreate21),
                               &_cbDialog30, 0, 0, 0);
    return(hWin);
}
/***************************************************************************************************
* @brief  模式
* @para   
* @retval 
*/
WM_HWIN _ModesSettingWin(void)
{
    WM_HWIN hWin1,hWin2,hWin3,hWin4;

    hWin1 = FRAMEWIN_Create("",0,WM_CF_SHOW,3,   146, 642+37, 250);//调整窗口位置
    hWin3 = MULTIPAGE_CreateEx(8, 10, 625+37, 230, hWin1, WM_CF_SHOW, 0, GUI_ID_MULTIPAGE0);

    hWin2 = GUI_CreateDialogBox(_aDialogCreate17,
                                GUI_COUNTOF(_aDialogCreate17),
                                &_cbDialog27, 0, 0, 0);
    hWin4 = GUI_CreateDialogBox(_aDialogCreate1,
                                GUI_COUNTOF(_aDialogCreate1),
                                &_cbDialog1, 0, 0, 0);

    MULTIPAGE_AddPage(hWin3, hWin2, Sys_Lib_GetDispStr(DP_MODES));
    MULTIPAGE_AddPage(hWin3, hWin4, Sys_Lib_GetDispStr(DP_BACKUP));

    MULTIPAGE_SelectPage(hWin3,0);
    FRAMEWIN_AddCloseButton(hWin1,0,5);
//     if(BeathSettingPara.BreathMode!=PSV_MODE)
    if(Breath_Setting_Tmp.BreathMode!=PSV_MODE)
    {
        MULTIPAGE_DisablePage(hWin3,1);
    }
    return(hWin1);
}
/***************************************************************************************************
* @brief  报警
* @para   
* @retval 
*/
WM_HWIN _AlarmsHandleWin(void)
{
    WM_HWIN hWin1,hWin2,hWin3,hWin4,hWin5,hWin6;
    //hWin = MultiColumnWin_CreateEx(0, 60, 159, 540, 0,
    //                           WM_CF_SHOW, 0, 0);
    hWin1 = FRAMEWIN_Create("",0,WM_CF_SHOW,3,   146, 555+87+37, 250);
	hWin3 = MULTIPAGE_CreateEx(8, 10, 625+37, 230, hWin1, WM_CF_SHOW, 0, GUI_ID_MULTIPAGE0);
	FRAMEWIN_AddCloseButton(hWin1,0,5);

    hWin2 = GUI_CreateDialogBox(_aDialogCreate3,
                                GUI_COUNTOF(_aDialogCreate3),
                                &_cbDialog3, 0, 0, 0);
	Menu_hWin.Alarm_Win.AlarmWin_Lim1 = hWin2;

    hWin4 = GUI_CreateDialogBox(_aDialogCreate19,
                                GUI_COUNTOF(_aDialogCreate19),
                                &_cbDialog29, 0, 0, 0);
	Menu_hWin.Alarm_Win.AlarmWin_Lim2 = hWin4;

#ifdef SPO2_SWITCH
    hWin6 = GUI_CreateDialogBox(_aDialogCreate40,
                                GUI_COUNTOF(_aDialogCreate40),
                                &_cbDialog40, 0, 0, 0);
	Menu_hWin.Alarm_Win.AlarmWin_Lim3 = hWin6;

#endif
    hWin5 = GUI_CreateDialogBox(_aDialogCreate7,
                                GUI_COUNTOF(_aDialogCreate7),
                                &_cbDialog7, 0, 0, 0);
	Menu_hWin.Alarm_Win.AlarmWin_Info = hWin5;

  
    MULTIPAGE_AddPage(hWin3, hWin2, Sys_Lib_GetDispStr(DP_LIMITS1));
    MULTIPAGE_AddPage(hWin3, hWin4, Sys_Lib_GetDispStr(DP_LIMITS2));
#ifdef SPO2_SWITCH
	MULTIPAGE_AddPage(hWin3, hWin6, Sys_Lib_GetDispStr(DP_LIMITS3));
 #endif
    MULTIPAGE_AddPage(hWin3, hWin5, Sys_Lib_GetDispStr(DP_ALARMLOG));
    
// #if (LOGO_FLAG == MSW_LOGO)
//     MULTIPAGE_DisablePage(hWin3,2);
// #endif
//     MULTIPAGE_SelectPage(hWin3,0);
	if(!AlarmEnterFlag) //ylk add 20190126
	{
		MULTIPAGE_SelectPage(hWin3,0);
	}
	else
	{
		MULTIPAGE_SelectPage(hWin3,3);
	}
	AlarmEnterFlag = 0;
    return(hWin1);
}
/***************************************************************************************************
* @brief  系统
* @para   
* @retval 
*/
WM_HWIN _SystemHandleWin(void)
{
    WM_HWIN hWin1,hWin2,hWin3,hWin4,hWin5,hWin6;
	hSpO2 = 0;
	hComp = 0;
    hWin1 = FRAMEWIN_Create("",0,WM_CF_SHOW,3, 146, 642+37, 250);//ylk modify 调整对话框大小
    hWin2 = MULTIPAGE_CreateEx(8, 10, 625+37, 230, hWin1, WM_CF_SHOW, 0, GUI_ID_MULTIPAGE0);//
    FRAMEWIN_AddCloseButton(hWin1,0,5);
    hWin3 = GUI_CreateDialogBox(_aDialogCreate8,
                                GUI_COUNTOF(_aDialogCreate8),
                                &_cbDialog8, 0, 0, 0);
    hWin4 = GUI_CreateDialogBox(_aDialogCreate4,
                                GUI_COUNTOF(_aDialogCreate4),
                                &_cbDialog4, 0, 0, 0);
    hWin5 = GUI_CreateDialogBox(_aDialogCreate6,
                                GUI_COUNTOF(_aDialogCreate6),
                                &_cbDialog6, 0, 0, 0);
    hWin6 = GUI_CreateDialogBox(_aDialogCreate5,
                                GUI_COUNTOF(_aDialogCreate5),
                                &_cbDialog5, 0, 0, 0);
    
    MULTIPAGE_AddPage(hWin2, hWin3, Sys_Lib_GetDispStr(DP_INFORMATION));
    MULTIPAGE_AddPage(hWin2, hWin4, Sys_Lib_GetDispStr(DP_SETTINGS));
    MULTIPAGE_AddPage(hWin2, hWin5, Sys_Lib_GetDispStr(DP_DATETIME));
    MULTIPAGE_AddPage(hWin2, hWin6, Sys_Lib_GetDispStr(DP_CALIBRATION));
    
    MULTIPAGE_SelectPage(hWin2,0);
    if(StandbyState!=0xFF)
    {
        MULTIPAGE_DisablePage(hWin2,3);
    }
    else
    {
        MULTIPAGE_EnablePage(hWin2,3);
    }
    return(hWin1);
}
/***************************************************************************************************
* @brief  监测
* @para   
* @retval 
*/
WM_HWIN _MonitorParameterWin(void)
{
    WM_HWIN hWin1,hWin2,hWin3,hWin5;

    hWin1 = FRAMEWIN_Create("",0,WM_CF_SHOW,3,   146, 642+37, 250);//调整窗口位置
    hWin3 = MULTIPAGE_CreateEx(8, 10, 625+37, 230, hWin1, WM_CF_SHOW, 0, GUI_ID_MULTIPAGE0);
    hWin2 = GUI_CreateDialogBox(_aDialogCreate2,
                                GUI_COUNTOF(_aDialogCreate2),
                                &_cbDialog2, 0, 0, 0);
	MULTIPAGE_AddPage(hWin3, hWin2, Sys_Lib_GetDispStr(DP_VALUES1));

#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C)//add ylk 20170907
    hWin5 = GUI_CreateDialogBox(_aDialogCreateValues2,
                                GUI_COUNTOF(_aDialogCreateValues2),
                                &_cbDialogValues2, 0, 0, 0);
    MULTIPAGE_AddPage(hWin3, hWin5, Sys_Lib_GetDispStr(DP_VALUES2));
#endif

    FRAMEWIN_AddCloseButton(hWin1,0,5);
    
   //MULTIPAGE_AddPage(hWin3, hWin4, " 波形设置 "); //sun del 20140521
    MULTIPAGE_SelectPage(hWin3,0);
    
    return(hWin1);
}
/***************************************************************************************************
* @brief  波形选择
* @para   
* @retval 
*/
MENU_Handle _GraphicMenuCreat(CURVE_HWIN *hObj)
{
    MENU_Handle hWin;
    WM_HWIN hItem;
    hWin = GUI_CreateDialogBox(_aDialogCreate15,
                               GUI_COUNTOF(_aDialogCreate15),
                               &_cbDialog15, 0, 0, 0);
    
    hItem = WM_GetDialogItem(hWin, GUI_ID_MENU0);

    switch(hObj->FocusCurve)
    {
    case 0:
        MENU_SetSel(hItem,hObj->WaveForms[0]);
        break;
    case 1:
        MENU_SetSel(hItem,hObj->WaveForms[1]);        
		WM_MoveTo(hWin, 475, 255);
        break;
	case 2:  
		MENU_SetSel(hItem,hObj->WaveForms[2]);
		WM_MoveTo(hWin, 475, 410);
		break;
	case 3:
		MENU_SetSel(hItem,hObj->WaveForms[3]);        
		WM_MoveTo(hWin, 275, 310);
		break;
    }
    return hWin;
}

/***************************************************************************************************
* @brief  激活待机窗
* @para   
* @retval 
*/
WM_HWIN ActivateStandbyWinCreat(void)
{
    WM_HWIN hWin,hWin1;
    hWin1 = FRAMEWIN_Create("",0,WM_CF_SHOW,3,   145, 679, 250);//调整窗口位置
    hWin = GUI_CreateDialogBox(_aDialogCreate16,\
                               GUI_COUNTOF(_aDialogCreate16),\
                               &_cbDialog16, hWin1, 0, 0);
    FRAMEWIN_AddCloseButton(hWin1,0,5);
    return hWin1;
}
/***************************************************************************************************
* @brief  波形菜单初始化
* @para   
* @retval 
*/
#if 1

static void CurveMenuInit(CURVE_HWIN *hWin,WM_HWIN hDlg)
{
    CurveInit(hDlg, WAVEFORM_PRESSURE, 0);
    CurveInit(hDlg, hWin->WaveForms[1], 1);
    CurveInit(hDlg, hWin->WaveForms[2], 2);
	if(Menu_hWin.cWin.LoopFlag1 != 0) {
        CurveInit(hDlg, hWin->WaveForms[3], 3);
    }        
}

#else

static void CurveMenuInit(CURVE_HWIN *hWin,WM_HWIN hDlg)
{
    WM_HWIN hItem0,hItem1,hItem2,hItem3;
	//  [2/14/2022 yaoyao add]选择麻气之后显示相关检测参数；未选配麻气，隐藏相关参数，波形左移，波形x轴最大值改为12s
	int nXMax = 8;//波形x轴最大值
	//
    hItem0 = WM_GetDialogItem(hDlg, GUI_ID_CURVE0);
    hItem1 = WM_GetDialogItem(hDlg, GUI_ID_CURVE1);
    hItem2 = WM_GetDialogItem(hDlg, GUI_ID_CURVE2);
	hItem3 = WM_GetDialogItem(hDlg, GUI_ID_CURVE3);

	if(Menu_hWin.cWin.LoopFlag1 != 0)
	{
		WM_ShowWindow(hItem3);
	}
	else
	{
		WM_HideWindow(hItem3);
	}

	//
// 	TracingCurve_SetBKColor(hItem0,GUI_DARKBLUE);
// 	TracingCurve_SetBKColor(hItem1,GUI_GREEN);
// 	TracingCurve_SetBKColor(hItem2,GUI_BLUE);
	//

//#if 0
//	//  [2/14/2022 yaoyao add]将根据是否选配麻气修改波形x轴量程以及波形显示位置和相关监测参数；选择麻气之后显示相关检测参数
//	if ((EtCO2_Config_On != 0) || (Masimo_EtCO2_Config_On != 0) 
//		|| (MasimoISA_EtCO2_Config_On != 0) || (EtCO2_KM7002_Config_On != 0)
//		|| (Drager_EtCO2_Config_On != 0))//  [5/18/2022 yaoyao add]未判断的麻气类型
//	{
//		nXMax = 8;
//		WM_SetWindowPos(hItem0,224/*171*/,   0+42+33,   400,  108);
//		WM_SetWindowPos(hItem1,224/*171*/,   141+42, 400,  108);
//		WM_SetWindowPos(hItem2,224/*171*/,   249+42, 400,  108);
//	}
//	else
//	{
//		nXMax = 12;
//		WM_SetWindowPos(hItem0,5+25,   0+42+33,   548/*474*/,  108);
//		WM_SetWindowPos(hItem1,5+25,	141+42, 548/*474*/,  108);
//		WM_SetWindowPos(hItem2,5+25,	249+42, 548/*474*/,  108);
//	} 
//#endif

    switch(hWin->LoopFlag1)
    {
    case 0:
        TracingCurve_SetViewMode(hItem0,0,'F');
        TracingCurve_SetViewMode(hItem1,0,'F');
        TracingCurve_SetViewMode(hItem2,0,'F');
		TracingCurve_SetViewMode(hItem3,0,'T');
		break;
    case 1:
		//  [2/14/2022 yaoyao add]将根据是否选配麻气修改波形x轴量程以及波形显示位置和相关监测参数；选择麻气之后显示相关检测参数
		//if ((EtCO2_Config_On != 0) || (Masimo_EtCO2_Config_On != 0) 
		//	|| (MasimoISA_EtCO2_Config_On != 0) || (EtCO2_KM7002_Config_On != 0)
		//	|| (Drager_EtCO2_Config_On != 0))//  [5/18/2022 yaoyao add]未判断的麻气类型
		//{

			//WM_SetSize(hItem0,400,150-33); 
			//TracingCurve_SetViewMode(hItem0,0,'F');

			//WM_SetSize(hItem1,200,210);  
			//WM_MoveChildTo(hItem1, 224/*171*/, 150);
			//TracingCurve_SetViewMode(hItem1,0,'T');//LOOP
        
			//WM_SetSize(hItem2,200,210);
			//WM_MoveChildTo(hItem2, 365, 150);
			//TracingCurve_SetViewMode(hItem2,0,'T'); 


		//}
		//else
		//{
		//	WM_SetSize(hItem0,548/*474*/,150-33); 
		//	TracingCurve_SetViewMode(hItem0,0,'F');
		//	
		//	WM_SetSize(hItem1,280/*243*/,210);  
		//	WM_MoveChildTo(hItem1, 5, 150);
		//	TracingCurve_SetViewMode(hItem1,0,'T');//LOOP

		//	WM_SetSize(hItem2,280/*243*/,210);
		//	WM_MoveChildTo(hItem2, 287/*250*/, 150);
		//	TracingCurve_SetViewMode(hItem2,0,'T'); 
		//}
        break;
    default:break;
    }

#if 0
	if(!((WaveFormsId2 == 4)||(WaveFormsId2 == 5)))			//gyh  add 20230727 环形波界面BUG 右边重新运行后显示CO2波形
	{
		if(WaveFormsId2 >= 4 && WaveFormsId2 <= 7)
		{
			if(!EtCO2ModuleOff)
			{
				if (hWin->LoopFlag1 == 1)
				{
					WaveFormsId2 = LOOP_VF;
					Menu_hWin.cWin.WaveForms[2] = LOOP_VF;
				}
				else
				{
					WaveFormsId2 = WAVEFORM_VOLUME;
					Menu_hWin.cWin.WaveForms[2] = WAVEFORM_VOLUME;
				}
				if ((MenuType==WAVEFORM_SETTING_MENU) && Menu_hWin.ActivedWin)
				{
					MENU_SetSel(WM_GetDialogItem(Menu_hWin.ActivedWin, GUI_ID_MENU0), Menu_hWin.cWin.WaveForms[2]);
				}
			}
			else
			{
				hWin->WaveForms[2] = WaveFormsId2;
			}
		}
		else if(WaveFormsId2 == 3)
		{
			if(SpO2ModuleOff)
			{
				if (hWin->LoopFlag1 == 1)
				{
					WaveFormsId2 = LOOP_VF;
					Menu_hWin.cWin.WaveForms[2] = LOOP_VF;
				}
				else
				{
					WaveFormsId2 = WAVEFORM_VOLUME;
					Menu_hWin.cWin.WaveForms[2] = WAVEFORM_VOLUME;
				}
				if ((MenuType==WAVEFORM_SETTING_MENU) && Menu_hWin.ActivedWin)
				{
					MENU_SetSel(WM_GetDialogItem(Menu_hWin.ActivedWin, GUI_ID_MENU0), Menu_hWin.cWin.WaveForms[2]);
				}
			}
			else
			{
				hWin->WaveForms[2] = WaveFormsId2;
			}
		}
	}
	//
	if (((Menu_hWin.cWin.WaveForms[1]==LOOP_PV||Menu_hWin.cWin.WaveForms[1]==LOOP_VF)&&(Menu_hWin.cWin.WaveForms[2]!=LOOP_PV &&Menu_hWin.cWin.WaveForms[2]!=LOOP_VF))
		||((Menu_hWin.cWin.WaveForms[2]==LOOP_PV||Menu_hWin.cWin.WaveForms[2]==LOOP_VF)&&(Menu_hWin.cWin.WaveForms[1]!=LOOP_PV &&Menu_hWin.cWin.WaveForms[1]!=LOOP_VF))
		)
	{
		Menu_hWin.cWin.WaveForms[1] = WAVEFORM_FLOW;
		Menu_hWin.cWin.WaveForms[2] = WAVEFORM_VOLUME;
		WaveFormsId2 = WAVEFORM_VOLUME;
	}

#endif // #if 0

    switch(hWin->WaveForms[0])
    {
    case WAVEFORM_PRESSURE:
		TracingCurve_SetRange(hItem0,MultiArray[0]);

        TracingCurve_SetXYINFO(hItem0,'T',0,nXMax,1,GUI_WHITE,Sys_Lib_GetDispStr(DP_S),\
            -1,5,1,GUI_WHITE,Sys_Lib_GetDispStr(DP_CMH2O),GUI_WHITE,Sys_Lib_GetDispStr(DP_PAW));

        TracingCurve_SetVigilanceLine(hItem0,'T',GUI_WHITE,AlarmSettingPara.PressureHighValue);
        break;
    case WAVEFORM_FLOW:
		TracingCurve_SetRange(hItem0,MultiArray[1]);

        TracingCurve_SetXYINFO(hItem0,'T',0,nXMax,1,GUI_WHITE,Sys_Lib_GetDispStr(DP_S),\
            -3,3,1,GUI_WHITE,"LPM",GUI_WHITE,Sys_Lib_GetDispStr(DP_FLOW));
		break;
    case WAVEFORM_VOLUME:
		TracingCurve_SetRange(hItem0,MultiArray[2]);

        TracingCurve_SetXYINFO(hItem0,'T',0,nXMax,1,GUI_WHITE,Sys_Lib_GetDispStr(DP_S),\
            0,30,5,GUI_WHITE,"mL",GUI_WHITE,Sys_Lib_GetDispStr(DP_VOLUME));

		break;
#ifdef SPO2_SWITCH
    case WAVEFORM_SPO2:
		if(Masimo_SpO2_Config_On)
		{
			TracingCurve_SetRange(hItem0,20);
			TracingCurve_SetXYINFO(hItem0,'T',0,nXMax,1,GUI_WHITE,Sys_Lib_GetDispStr(DP_S),\
				-5,5,1,GUI_WHITE,"",GUI_WHITE,Sys_Lib_GetDispStr(DP_PLETH));
		}
		else
		{
			TracingCurve_SetXYINFO(hItem0,'T',0,nXMax,1,GUI_WHITE,Sys_Lib_GetDispStr(DP_S),\
				0,100,20,GUI_WHITE,"",GUI_WHITE,Sys_Lib_GetDispStr(DP_PLETH));

			TracingCurve_SetYsign(hItem0,'F'); //  20130717    
		}
        
        break;
#endif
    case WAVEFORM_CO2:
		TracingCurve_SetRange(hItem0,MultiArray[3]);//  CO2添加量程变换
		TracingCurve_SetXYINFO(hItem0,'T',0,nXMax,1,GUI_WHITE,Sys_Lib_GetDispStr(DP_S),
			0,CO2_Disp.co2max,CO2_Disp.co2step,GUI_WHITE,CO2_Disp.co2_unit,GUI_WHITE,Sys_Lib_GetDispStr(DP_CO2));//CO2

        break;

	case LOOP_V_CO2:
		TracingCurve_SetRange(hItem0,MultiArray[3]);
		TracingCurve_SetRangeX(hItem0,MultiArray[2]);
		TracingCurve_SetXYINFO(hItem0,'T',0,30,5,GUI_WHITE,Sys_Lib_GetDispStr(DP_ML),
			0,CO2_Disp.co2max,CO2_Disp.co2step,GUI_WHITE,CO2_Disp.co2_unit,GUI_WHITE,Sys_Lib_GetDispStr(DP_VCO2));
		break;

#ifdef AA_WAVE_SWITCH
    case WAVEFORM_N2O:
        TracingCurve_SetRange(hItem0,1);
        TracingCurve_SetXYINFO(hItem0,'T',0,nXMax,1,GUI_WHITE,Sys_Lib_GetDispStr(DP_S),\
            0,100,20,GUI_WHITE,"%",GUI_WHITE,Sys_Lib_GetDispStr(DP_N2O));
        break;
    case WAVEFORM_PAA:
        TracingCurve_SetRange(hItem0,MultiAA[0]);

        TracingCurve_SetXYINFO(hItem0,'T',0,nXMax,1,GUI_WHITE,Sys_Lib_GetDispStr(DP_S),\
            0,5,1,GUI_WHITE,"%",GUI_WHITE,AA_all_data.AA_detail.AA1_Type_Text);
        break;
    case WAVEFORM_SAA:
        TracingCurve_SetRange(hItem0,MultiAA[1]);
        TracingCurve_SetXYINFO(hItem0,'T',0,nXMax,1,GUI_WHITE,Sys_Lib_GetDispStr(DP_S),\
            0,5,1,GUI_WHITE,"%",GUI_WHITE,AA_all_data.AA_detail.AA2_Type_Text);
        break;
#endif
    }
    switch(hWin->WaveForms[1])
    {
    case WAVEFORM_PRESSURE:
		TracingCurve_SetRange(hItem1,MultiArray[0]);

        TracingCurve_SetXYINFO(hItem1,'T',0,nXMax,1,GUI_WHITE,Sys_Lib_GetDispStr(DP_S),\
            -1,5,1,GUI_WHITE,Sys_Lib_GetDispStr(DP_CMH2O),GUI_WHITE,Sys_Lib_GetDispStr(DP_PAW));

		TracingCurve_SetVigilanceLine(hItem1,'T',GUI_WHITE,AlarmSettingPara.PressureHighValue);
        break;
    case WAVEFORM_FLOW:
		TracingCurve_SetRange(hItem1,MultiArray[1]);

        TracingCurve_SetXYINFO(hItem1,'T',0,nXMax,1,GUI_WHITE,Sys_Lib_GetDispStr(DP_S),\
            -3,3,1,GUI_WHITE,"LPM",GUI_WHITE,Sys_Lib_GetDispStr(DP_FLOW));

			break;
    case WAVEFORM_VOLUME:
		TracingCurve_SetRange(hItem1,MultiArray[2]);

        TracingCurve_SetXYINFO(hItem1,'T',0,nXMax,1,GUI_WHITE,Sys_Lib_GetDispStr(DP_S),\
            0,30,5,GUI_WHITE,"mL",GUI_WHITE,Sys_Lib_GetDispStr(DP_VOLUME));

        break;
#ifdef SPO2_SWITCH
    case WAVEFORM_SPO2:
		if(Masimo_SpO2_Config_On)
		{
			TracingCurve_SetRange(hItem1,20);
			TracingCurve_SetXYINFO(hItem1,'T',0,nXMax,1,GUI_WHITE,Sys_Lib_GetDispStr(DP_S),\
				-5,5,1,GUI_WHITE,"",GUI_WHITE,Sys_Lib_GetDispStr(DP_PLETH));
		}
		else
		{
			TracingCurve_SetXYINFO(hItem1,'T',0,nXMax,1,GUI_WHITE,Sys_Lib_GetDispStr(DP_S),\
				0,100,20,GUI_WHITE,"",GUI_WHITE,Sys_Lib_GetDispStr(DP_PLETH));

			TracingCurve_SetYsign(hItem1,'F'); //  20130717
		}
        break;
#endif
    case WAVEFORM_CO2:
		TracingCurve_SetRange(hItem1,MultiArray[3]);//  [2/14/2022 yaoyao add]CO2添加量程变换

		TracingCurve_SetXYINFO(hItem1,'T',0,nXMax,1,GUI_WHITE,Sys_Lib_GetDispStr(DP_S),
			0,CO2_Disp.co2max,CO2_Disp.co2step,GUI_WHITE,CO2_Disp.co2_unit,GUI_WHITE,Sys_Lib_GetDispStr(DP_CO2));//CO2

        break;

	case LOOP_V_CO2:
		TracingCurve_SetRange(hItem1,MultiArray[3]);
		TracingCurve_SetRangeX(hItem1,MultiArray[2]);
		TracingCurve_SetXYINFO(hItem1,'T',0,30,5,GUI_WHITE,Sys_Lib_GetDispStr(DP_ML),
			0,CO2_Disp.co2max,CO2_Disp.co2step,GUI_WHITE,CO2_Disp.co2_unit,GUI_WHITE,Sys_Lib_GetDispStr(DP_VCO2));
		break;

	case LOOP_PV:
		WM_SetSize(hItem1, g_loop_x_size, g_loop_y_size);
		WM_MoveChildTo(hItem1, g_curve1_x1, g_curve1_y);

		TracingCurve_SetRange(hItem1,MultiArray[2]);
		TracingCurve_SetRangeX(hItem1,MultiArray[0]);
        TracingCurve_SetXYINFO(hItem1,'T',-1,5,1,GUI_WHITE,Sys_Lib_GetDispStr(DP_CMH2O),
			0,30,5,GUI_WHITE,"mL",GUI_WHITE,"P-V");
        break;
	case LOOP_VF:
		WM_SetSize(hItem1, g_loop_x_size, g_loop_y_size);
		WM_MoveChildTo(hItem1, g_curve1_x1, g_curve1_y);

		TracingCurve_SetRange(hItem1,MultiArray[1]);
		TracingCurve_SetRangeX(hItem1,MultiArray[2]);
        TracingCurve_SetXYINFO(hItem1,'T', 0,30,5,GUI_WHITE,"mL", -3,3,1,GUI_WHITE,"LPM", \
            GUI_WHITE,"V-F");
        break;
	case LOOP_PF:
		WM_SetSize(hItem1, g_loop_x_size, g_loop_y_size);
		WM_MoveChildTo(hItem1, g_curve1_x1, g_curve1_y);

		TracingCurve_SetRange(hItem1, MultiArray[1]);
		TracingCurve_SetRangeX(hItem1, MultiArray[0]);
		TracingCurve_SetXYINFO(hItem1, 'T', -1, 5, 1, GUI_WHITE, Sys_Lib_GetDispStr(DP_CMH2O), 
			-3, 3, 1, GUI_WHITE, Sys_Lib_GetDispStr(DP_LMIN), GUI_WHITE, "P-F");
		break;

#ifdef AA_WAVE_SWITCH
    case WAVEFORM_N2O:
        TracingCurve_SetRange(hItem1,1);
        TracingCurve_SetXYINFO(hItem1,'T',0,nXMax,1,GUI_WHITE,Sys_Lib_GetDispStr(DP_S),\
            0,100,20,GUI_WHITE,"%",GUI_WHITE,Sys_Lib_GetDispStr(DP_N2O));
        break;
    case WAVEFORM_PAA:
        TracingCurve_SetRange(hItem1,MultiAA[0]);

        TracingCurve_SetXYINFO(hItem1,'T',0,nXMax,1,GUI_WHITE,Sys_Lib_GetDispStr(DP_S),\
            0,5,1,GUI_WHITE,"%",GUI_WHITE,AA_all_data.AA_detail.AA1_Type_Text);
        break;
    case WAVEFORM_SAA:
        TracingCurve_SetRange(hItem1,MultiAA[1]);
        TracingCurve_SetXYINFO(hItem1,'T',0,nXMax,1,GUI_WHITE,Sys_Lib_GetDispStr(DP_S),\
            0,5,1,GUI_WHITE,"%",GUI_WHITE,AA_all_data.AA_detail.AA2_Type_Text);
        break;
#endif
    }

    switch(hWin->WaveForms[3])
    {
        case LOOP_PV:
			WM_SetSize(hItem3, g_loop_x_size, g_loop_y_size);
			WM_MoveChildTo(hItem3, g_curve3_x, g_curve1_y);

            TracingCurve_SetRange(hItem3,MultiArray[2]);
            TracingCurve_SetRangeX(hItem3,MultiArray[0]);
            TracingCurve_SetXYINFO(hItem3,'T',-1,5,1,GUI_WHITE,Sys_Lib_GetDispStr(DP_CMH2O),
                0,30,5,GUI_WHITE,"mL",GUI_WHITE,"P-V");
            break;
		case LOOP_VF:
			WM_SetSize(hItem3, g_loop_x_size, g_loop_y_size);
			WM_MoveChildTo(hItem3, g_curve3_x, g_curve1_y);

            TracingCurve_SetRange(hItem3,MultiArray[1]);
            TracingCurve_SetRangeX(hItem3,MultiArray[2]);
            TracingCurve_SetXYINFO(hItem3,'T', 0,30,5,GUI_WHITE,"mL", -3,3,1,GUI_WHITE,"LPM", \
                GUI_WHITE,"V-F");
            break;
		case LOOP_PF:
			WM_SetSize(hItem3, g_loop_x_size, g_loop_y_size);
			WM_MoveChildTo(hItem3, g_curve3_x, g_curve1_y);

            TracingCurve_SetRange(hItem3, MultiArray[1]);
            TracingCurve_SetRangeX(hItem3, MultiArray[0]);
            TracingCurve_SetXYINFO(hItem3, 'T', -1, 5, 1, GUI_WHITE, Sys_Lib_GetDispStr(DP_CMH2O), 
                -3, 3, 1, GUI_WHITE, Sys_Lib_GetDispStr(DP_LMIN), GUI_WHITE, "P-F");
            break;

    }

    switch(hWin->WaveForms[2])
    {
    case WAVEFORM_PRESSURE:
		TracingCurve_SetRange(hItem2,MultiArray[0]);

        TracingCurve_SetXYINFO(hItem2,'T',0,nXMax,1,GUI_WHITE,Sys_Lib_GetDispStr(DP_S),\
            -1,5,1,GUI_WHITE,Sys_Lib_GetDispStr(DP_CMH2O),GUI_WHITE,Sys_Lib_GetDispStr(DP_PAW));

		TracingCurve_SetVigilanceLine(hItem2,'T',GUI_WHITE,AlarmSettingPara.PressureHighValue);
        break;
    case WAVEFORM_FLOW:
		TracingCurve_SetRange(hItem2,MultiArray[1]);

        TracingCurve_SetXYINFO(hItem2,'T',0,nXMax,1,GUI_WHITE,Sys_Lib_GetDispStr(DP_S),\
            -3,3,1,GUI_WHITE,"LPM",GUI_WHITE,Sys_Lib_GetDispStr(DP_FLOW));
        break;
    case WAVEFORM_VOLUME:
		TracingCurve_SetRange(hItem2,MultiArray[2]);

        TracingCurve_SetXYINFO(hItem2,'T',0,nXMax,1,GUI_WHITE,Sys_Lib_GetDispStr(DP_S),\
            0,30,5,GUI_WHITE,"mL",GUI_WHITE,Sys_Lib_GetDispStr(DP_VOLUME));
        break;
#ifdef SPO2_SWITCH
    case WAVEFORM_SPO2:
		if(Masimo_SpO2_Config_On)
		{
			TracingCurve_SetRange(hItem2,20);
			TracingCurve_SetXYINFO(hItem2,'T',0,nXMax,1,GUI_WHITE,Sys_Lib_GetDispStr(DP_S),\
				-5,5,1,GUI_WHITE,"",GUI_WHITE,Sys_Lib_GetDispStr(DP_PLETH));
		}
		else
		{
			TracingCurve_SetXYINFO(hItem2,'T',0,nXMax,1,GUI_WHITE,Sys_Lib_GetDispStr(DP_S),\
				0,100,20,GUI_WHITE,"",GUI_WHITE,Sys_Lib_GetDispStr(DP_PLETH));

			TracingCurve_SetYsign(hItem2,'F'); //  20130717
		}
        break;
#endif
    case WAVEFORM_CO2:
		TracingCurve_SetRange(hItem2,MultiArray[3]);//  [2/14/2022 yaoyao add]CO2添加量程变换
		TracingCurve_SetXYINFO(hItem2,'T',0,nXMax,1,GUI_WHITE,Sys_Lib_GetDispStr(DP_S),
			0,CO2_Disp.co2max,CO2_Disp.co2step,GUI_WHITE,CO2_Disp.co2_unit,GUI_WHITE,Sys_Lib_GetDispStr(DP_CO2));//CO2
        break;

	case LOOP_V_CO2:
		TracingCurve_SetRange(hItem2,MultiArray[3]);
		TracingCurve_SetRangeX(hItem2,MultiArray[2]);
		TracingCurve_SetXYINFO(hItem2,'T',0,30,5,GUI_WHITE,Sys_Lib_GetDispStr(DP_ML),
			0,CO2_Disp.co2max,CO2_Disp.co2step,GUI_WHITE,CO2_Disp.co2_unit,GUI_WHITE,Sys_Lib_GetDispStr(DP_VCO2));
		break;

    case LOOP_PV:
		TracingCurve_SetRange(hItem2,MultiArray[2]);
		TracingCurve_SetRangeX(hItem2,MultiArray[0]);
        TracingCurve_SetXYINFO(hItem2,'T',-1,5,1,GUI_WHITE,Sys_Lib_GetDispStr(DP_CMH2O),
			0,30,5,GUI_WHITE,"mL",GUI_WHITE,"P-V");
        break;
    case LOOP_VF:
		TracingCurve_SetRange(hItem2,MultiArray[1]);
		TracingCurve_SetRangeX(hItem2,MultiArray[2]);
        TracingCurve_SetXYINFO(hItem2,'T', 0,30,5,GUI_WHITE,"mL", -3,3,1,GUI_WHITE,"LPM", \
            GUI_WHITE,"V-F");
		break;
	case LOOP_PF:
		TracingCurve_SetRange(hItem2, MultiArray[1]);
		TracingCurve_SetRangeX(hItem2, MultiArray[0]);
		TracingCurve_SetXYINFO(hItem2, 'T', -1, 5, 1, GUI_WHITE, Sys_Lib_GetDispStr(DP_CMH2O), -3, 3, 1, GUI_WHITE, Sys_Lib_GetDispStr(DP_LMIN),
			GUI_WHITE, "P-F");
		break;

#ifdef AA_WAVE_SWITCH
    case WAVEFORM_N2O:
        TracingCurve_SetRange(hItem2,1);
        TracingCurve_SetXYINFO(hItem2,'T',0,nXMax,1,GUI_WHITE,Sys_Lib_GetDispStr(DP_S),\
            0,100,20,GUI_WHITE,"%",GUI_WHITE,Sys_Lib_GetDispStr(DP_N2O));
        break;
    case WAVEFORM_PAA:
        TracingCurve_SetRange(hItem2,MultiAA[0]);

        TracingCurve_SetXYINFO(hItem2,'T',0,nXMax,1,GUI_WHITE,Sys_Lib_GetDispStr(DP_S),\
            0,5,1,GUI_WHITE,"%",GUI_WHITE,AA_all_data.AA_detail.AA1_Type_Text);
        break;
    case WAVEFORM_SAA:
        TracingCurve_SetRange(hItem2,MultiAA[1]);
        TracingCurve_SetXYINFO(hItem2,'T',0,nXMax,1,GUI_WHITE,Sys_Lib_GetDispStr(DP_S),\
            0,5,1,GUI_WHITE,"%",GUI_WHITE,AA_all_data.AA_detail.AA2_Type_Text);
        break;
#endif
    }//*/
}

#endif
/***************************************************************************************************
* @brief  实时参数窗初始化
* @para   
* @retval 
*/
static void MultiTextSetFrame(WM_HWIN hWin)
{
	MultiText_SetBKColor(hWin, GUI_BLACK);
	MultiText_SetLeftFrameSize(hWin, 1);
	MultiText_SetRightFrameSize(hWin, 1);
	MultiText_SetBotFrameSize(hWin, 1);
	MultiText_SetFrameColor(hWin, 0x591d11);
}
static void LeftMultiTextSetFrame(WM_HWIN hWin)
{
	MultiText_SetBKColor(hWin, GUI_BLACK);
	// 	MultiText_SetLeftFrameSize(hWin, 1);
	MultiText_SetRightFrameSize(hWin, 1);
	MultiText_SetBotFrameSize(hWin, 1);
	MultiText_SetFrameColor(hWin, 0x2d2d2d);
	//  [2/14/2022 yaoyao add]未选择麻气，隐藏左侧监测区
	//if ((EtCO2_Config_On != 0) || (Masimo_EtCO2_Config_On != 0) 
	//	|| (MasimoISA_EtCO2_Config_On != 0) || (EtCO2_KM7002_Config_On != 0)
	//	|| (Drager_EtCO2_Config_On != 0))//  [5/18/2022 yaoyao add]未判断的麻气类型
	//{
	//	WM_ShowWindow(hWin);
	//}
	//else
	//{
	//	WM_HideWindow(hWin);
	//}
}

static void LeftMultiTextSetFrame1(WM_HWIN hWin)
{
	MultiText_SetBKColor(hWin, GUI_BLACK);
	// 	MultiText_SetLeftFrameSize(hWin, 1);
	MultiText_SetRightFrameSize(hWin, 1);
	//MultiText_SetBotFrameSize(hWin, 1);
	MultiText_SetFrameColor(hWin, 0x2d2d2d);
	//  [2/14/2022 yaoyao add]未选择麻气，隐藏左侧监测区
	//if ((EtCO2_Config_On != 0) || (Masimo_EtCO2_Config_On != 0) 
	//	|| (MasimoISA_EtCO2_Config_On != 0) || (EtCO2_KM7002_Config_On != 0)
	//	|| (Drager_EtCO2_Config_On != 0))//  [5/18/2022 yaoyao add]未判断的麻气类型
	//{
	//	WM_ShowWindow(hWin);
	//}
	//else
	//{
	//	WM_HideWindow(hWin);
	//}
}
/*
*函数功能：对数据进行四舍五入
*20220214 yaoyao
*/
int Rounding(float fData, int nNarrowMul)
{
	float tmp_data = fData;
	float tmp_narrow_mul = 1.0*nNarrowMul;
	if(nNarrowMul != 0)
	{
		tmp_data = tmp_data/tmp_narrow_mul;
		tmp_data = tmp_data + 0.5;
	}

	return (int)tmp_data;
}
//工作界面上左侧监测区--麻气检测参数显示初始化
static void LeftRectPaintParaInit(WM_HWIN hDlg)
{
	WM_HWIN hItem;
	char pText[20] = {0};
	char pText1[20] = {0};
	int index = 0, sec = 0, min = 0, hour = 0;
	float MacValue = 0;
	GUI_COLOR name_color = GUI_WHITE;

#if 0
	hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT13);
	LeftMultiTextSetFrame(hItem);
	MultiText_SetTopFrameSize(hItem, 1);
	if(GetTimerHandle())//判断句柄是否存在 只有点暂停，才会删除定时器句柄
	{
		MultiText_SetParaName(hItem, Sys_Lib_GetDispStr(DP_STOP));
	}
	else
	{
		MultiText_SetParaName(hItem, Sys_Lib_GetDispStr(DP_START));
	}
	MultiText_SetTextRect(hItem, 1, 90, 0, 30, MultiText_Name);
	MultiText_SetTextFont(hItem, &GUI_Font_20, MultiText_Name);
	MultiText_SetTextAlign(hItem, GUI_TA_HCENTER|GUI_TA_VCENTER, MultiText_Name);
	//
	hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT14);
	LeftMultiTextSetFrame(hItem);
	MultiText_SetTopFrameSize(hItem, 1);  
	MultiText_SetParaName(hItem, Sys_Lib_GetDispStr(DP_CLEAR));
	MultiText_SetTextRect(hItem, 2, 81, 0, 30, MultiText_Name);
	MultiText_SetTextFont(hItem, &GUI_Font_20, MultiText_Name);
	MultiText_SetTextAlign(hItem, GUI_TA_HCENTER|GUI_TA_VCENTER, MultiText_Name);
	//
	hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT6);//计时
	LeftMultiTextSetFrame(hItem);
	if(!GetBitmapFlag())//定时器未开启或定时器清零时
	{
		MultiText_EX_SetBitmapEx(hItem, &bmTimer, 10, 15);
	}
	else
	{
		if(GetTimerStartFlag())
		{
			MultiText_EX_SetBitmapEx(hItem, &bmIconStart, 10, 15);
		}
		else
		{
			MultiText_EX_SetBitmapEx(hItem, &bmIconStop, 10, 15);
		}
	}
	SetTimerHandle(hItem);//传入显示计时的控件的句柄
	sec = GetTimerSec();
	min = GetTimerMin();
	hour = GetTimerHour();			
	sprintf(pText, "%02d.%02d.%02d", hour, min, sec);
	MultiText_SetParaData(hItem, pText);
	MultiText_SetTextRect(hItem, 50, 120, 19, 30, MultiText_Data);
	MultiText_SetTextFont(hItem, &GUI_Font32B_ASCII, MultiText_Data);
	MultiText_SetTextAlign(hItem, GUI_TA_HCENTER, MultiText_Data);
#endif

	//hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT7);
	//LeftMultiTextSetFrame(hItem);

	//hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT10);
	//LeftMultiTextSetFrame(hItem);

	//hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT12);
	//LeftMultiTextSetFrame(hItem);
	//hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT9);
	//LeftMultiTextSetFrame(hItem);
	//hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT11);
	//LeftMultiTextSetFrame(hItem);

	// 是否显示CO2
	if ((EtCO2_Config_On != 0) || (Masimo_EtCO2_Config_On != 0) 
		|| (MasimoISA_EtCO2_Config_On != 0) || (EtCO2_KM7002_Config_On != 0)
		|| (Drager_EtCO2_Config_On != 0)) 
	{
		if((index = Get_MonitorRank_AA(DP_CO2)) != -1)
		{
			hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT12);
			//name 
			MultiText_SetParaName(hItem, Sys_Lib_GetDispStr(DP_CO2));
			MultiText_SetTextRect(hItem, 2, 30, 6, 25, MultiText_Name);
			MultiText_SetTextFont(hItem, &GUI_Font_20, MultiText_Name);
			MultiText_SetTextColor(hItem, GUI_WHITE, MultiText_Name);
			MultiText_SetTextAlign(hItem, GUI_TA_LEFT, MultiText_Name);
			//unit
			switch(ETCO2Unit)
			{
			case 0:
				MultiText_SetParaUnit(hItem, Sys_Lib_GetDispStr(DP_PERCENT));
				break;
			case 1:
				MultiText_SetParaUnit(hItem, Sys_Lib_GetDispStr(DP_MMHG));
				break;
			case 2:
				MultiText_SetParaUnit(hItem, Sys_Lib_GetDispStr(DP_KPA));
				break;
			default:break;
			}
			MultiText_SetTextRect(hItem, 2, 25, 46, 25, MultiText_Unit);
			MultiText_SetTextFont(hItem, &GUI_Font_20, MultiText_Unit);
			MultiText_SetTextColor(hItem, GUI_WHITE, MultiText_Unit);
			MultiText_SetTextAlign(hItem, GUI_TA_LEFT, MultiText_Unit);

			//MaxValue
			if((ETCO2Unit == 0)||(ETCO2Unit == 2))
			{
				strcpy(pText, _Float2String((float)AlarmSettingPara.FiCO2HighValue/10,1));
				MultiText_SetParaMaxValue(hItem, pText);

				strcpy(pText1, _Float2String((float)AlarmSettingPara.FiCO2LowValue/10,1));
				MultiText_SetParaMinValue(hItem, pText1);
			}
			else
			{
				sprintf(pText, "%d", AlarmSettingPara.FiCO2HighValue);
				MultiText_SetParaMaxValue(hItem, pText);

				sprintf(pText1, "%d", AlarmSettingPara.FiCO2LowValue);
				MultiText_SetParaMinValue(hItem, pText1);
			}
			//  [2/11/2022 yaoyao]解决左侧的快捷监测区CO2的上下限制当限值大于10时显示不全问题
			MultiText_SetTextRect(hItem, 105, 25, 6, 25, MultiText_MaxValue);//[2/11/2022 yaoyao] MultiText_SetTextRect(hItem, 95-9, 15, 13, 15, MultiText_MaxValue);
			MultiText_SetTextFont(hItem, &GUI_Font_16, MultiText_MaxValue);
			MultiText_SetTextAlign(hItem, GUI_TA_LEFT, MultiText_MaxValue);
			MultiText_SetTextColor(hItem, GUI_WHITE, MultiText_MaxValue);

			//Data
			if((AA_Module_Flag.EtCO2ModuleExist) && (EtCO2ModuleOff))
			{
				if((ETCO2Unit == 0)||(ETCO2Unit == 2))//%、kPa
				{
					if(CO2_Parameters.insCO2 > 150/*133*/)
					{
						strcpy(pText, "--");
					}
					else if (CO2_Parameters.insCO2<100)
					{
						strcpy(pText, _Float2String((float)(CO2_Parameters.insCO2/10),1));
					}
					else
					{
						sprintf(pText, "%d",(int)Rounding(CO2_Parameters.insCO2,10));
					}
					//
					if(CO2_Parameters.etCO2 > 150/*133*/)
					{					
						strcpy(pText1, "--");
					}
					else if (CO2_Parameters.etCO2<100)//  [2/14/2022 yaoyao]大于等于10的数据，没有小数，小于10 的数据保存1位小数
					{
						strcpy(pText1, _Float2String((float)(CO2_Parameters.etCO2/10),1));
					}
					else
					{
						sprintf(pText1, "%d",(int)Rounding(CO2_Parameters.etCO2,10));
					}
				}
				else
				{
					if(CO2_Parameters.insCO2 > 113/*100*/)
					{
						strcpy(pText, "--");
					}
					else if (CO2_Parameters.insCO2 <10)//  [2/14/2022 yaoyao]大于等于10的数据，没有小数，小于10 的数据保存1位小数
					{
						strcpy(pText, _Float2String((float)(CO2_Parameters.insCO2),1));
					}
					else
					{
						sprintf(pText, "%d",(int)Rounding(CO2_Parameters.insCO2,1));
					}
					//
					if(CO2_Parameters.etCO2 > 113/*100*/)
					{					
						strcpy(pText1, "--");
					}
					else if (CO2_Parameters.etCO2 < 10)//  [2/14/2022 yaoyao]大于等于10的数据，没有小数，小于10 的数据保存1位小数
					{
						strcpy(pText1, _Float2String((float)(CO2_Parameters.etCO2),1));
					}
					else
					{
						sprintf(pText1, "%d",(int)Rounding(CO2_Parameters.etCO2,1));
					}
				}
			}
			else
			{
				strcpy(pText, "--");
				strcpy(pText1, "--");
			}
			//FiCO2 Data
			MultiText_SetParaData(hItem, pText); // 
			MultiText_SetTextRect(hItem, 30, 70, 13, 50, MultiText_Data);//[2/11/2022 yaoyao] MultiText_SetTextRect(hItem, 46, 38, 13, 30, MultiText_Data);
			//  [2/11/2022 yaoyao modify] 将FiCO2值字体调小
			MultiText_SetTextFont(hItem, &GUI_Font32B_ASCII, MultiText_Data);//[2/11/2022 yaoyao] MultiText_SetTextFont(hItem, &GUI_Font32B_ASCII, MultiText_Data);
			MultiText_SetTextColor(hItem, GUI_WHITE, MultiText_Data);
			MultiText_SetTextAlign(hItem, GUI_TA_HCENTER|GUI_TA_VCENTER, MultiText_Data);

			//EtCO2 Data
			MultiText_SetOtherParaData(hItem, pText1); // 
			MultiText_SetTextRect(hItem, 125, 70, 13, 50, MultiText_Data1);//[2/11/2022 yaoyao] MultiText_SetTextRect(hItem, 111, 36, 13, 30, MultiText_Data1);
			MultiText_SetTextFont(hItem, &GUI_Font32B_ASCII, MultiText_Data1);
			MultiText_SetTextColor(hItem, GUI_WHITE, MultiText_Data1);
			MultiText_SetTextAlign(hItem, GUI_TA_HCENTER|GUI_TA_VCENTER, MultiText_Data1);

			//MaxValue
			if((ETCO2Unit == 0)||(ETCO2Unit == 2))
			{
				strcpy(pText, _Float2String((float)AlarmSettingPara.EtCO2HighValue/10,1));

				strcpy(pText1, _Float2String((float)AlarmSettingPara.EtCO2LowValue/10,1));
			}
			else
			{
				sprintf(pText, "%d", AlarmSettingPara.EtCO2HighValue);

				sprintf(pText1, "%d", AlarmSettingPara.EtCO2LowValue);
			}
			MultiText_SetOtherParaMaxValue(hItem, pText);
			MultiText_SetTextRect(hItem, 200, 20, 6, 20, MultiText_MaxValue1);//[2/11/2022 yaoyao] MultiText_SetTextRect(hItem, 161-9, 15, 13, 15, MultiText_MaxValue1);
			MultiText_SetTextFont(hItem, &GUI_Font_16, MultiText_MaxValue1);
			MultiText_SetTextAlign(hItem, GUI_TA_RIGHT, MultiText_MaxValue1);
			MultiText_SetTextColor(hItem, GUI_WHITE, MultiText_MaxValue1);

			//MinValue
			MultiText_SetOtherParaMinValue(hItem, pText1);
			MultiText_SetTextRect(hItem, 200, 20, 46, 20, MultiText_MinValue1);//[2/11/2022 yaoyao] MultiText_SetTextRect(hItem, 161-9, 15, 28, 15, MultiText_MinValue1);
			MultiText_SetTextFont(hItem, &GUI_Font_16, MultiText_MinValue1);
			MultiText_SetTextAlign(hItem, GUI_TA_RIGHT, MultiText_MinValue1);
			MultiText_SetTextColor(hItem, GUI_WHITE, MultiText_MinValue1);
		}
	}

	// 是否显示AA N2O MAC
	//if((MasimoISA_EtCO2_Config_On != 0) || (Drager_EtCO2_Config_On != 0) || 
	//	((Masimo_EtCO2_Config_On == 1) && (AA_Module_Flag.PhaseInTypeModuleExist != 4))) 

	if((Drager_EtCO2_Config_On != 0) || 
		((MasimoISA_EtCO2_Config_On != 0) || (Masimo_EtCO2_Config_On != 0) && 
		((AA_Module_Flag.PhaseInTypeModuleExist == 1) || 
		(AA_Module_Flag.PhaseInTypeModuleExist == 2) || 
		(AA_Module_Flag.PhaseInTypeModuleExist == 3) ) )
		)
	{
		if((index = Get_MonitorRank_AA(DP_AA1)) != -1)
		{
			hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT9);//AA1 
			//LeftMultiTextSetFrame(hItem);

			switch(AA_all_data.AA_detail.AA_AA1.ID)
			{
			case IM_AGT_HAL:
				name_color = HALCOLOR;
				MultiText_SetParaName(hItem, Sys_Lib_GetDispStr(DP_HAL));
				break;
			case IM_AGT_ENF:
				name_color = ENFCOLOR;
				MultiText_SetParaName(hItem, Sys_Lib_GetDispStr(DP_ENF));
				break;
			case IM_AGT_ISO:
				name_color = ISOCOLOR;
				MultiText_SetParaName(hItem, Sys_Lib_GetDispStr(DP_ISO));
				break;
			case IM_AGT_SEV:
				name_color = SEVCOLOR;
				MultiText_SetParaName(hItem, Sys_Lib_GetDispStr(DP_SEV));
				break;
			case IM_AGT_DES:
				name_color = DESCOLOR;
				MultiText_SetParaName(hItem, Sys_Lib_GetDispStr(DP_DES));
				break;
			case IM_NO_AGT:
			default:
				name_color = GUI_WHITE;
				MultiText_SetParaName(hItem, Sys_Lib_GetDispStr(DP_AA1));
				break;
			}

			MultiText_SetTextColor(hItem, name_color, MultiText_Name);
			MultiText_SetTextColor(hItem, name_color, MultiText_Unit);
			MultiText_SetTextColor(hItem, name_color, MultiText_Data);
			MultiText_SetTextColor(hItem, name_color, MultiText_MaxValue);
			MultiText_SetTextColor(hItem, name_color, MultiText_MinValue);
			MultiText_SetTextColor(hItem, name_color, MultiText_Data1);
			MultiText_SetTextColor(hItem, name_color, MultiText_MaxValue1);
			MultiText_SetTextColor(hItem, name_color, MultiText_MinValue1);

			MultiText_SetTextRect(hItem, 2, 30, 6, 25, MultiText_Name);
			MultiText_SetTextFont(hItem, &GUI_Font_20, MultiText_Name);
			MultiText_SetTextAlign(hItem, GUI_TA_LEFT, MultiText_Name);
			//unit
			MultiText_SetParaUnit(hItem, Sys_Lib_GetDispStr(DP_PERCENT));
			MultiText_SetTextRect(hItem, 2, 25, 46, 25, MultiText_Unit);
			MultiText_SetTextFont(hItem, &GUI_Font_20, MultiText_Unit);
			MultiText_SetTextAlign(hItem, GUI_TA_LEFT, MultiText_Unit);

			//FiAA Data
			if(AA_all_data.AA_detail.AA_AA1.ID) 		
			{
				if((AA_Module_Flag.EtCO2ModuleExist)&&(EtCO2ModuleOff))
				{
					if((AA_all_data.AA_detail.AA_AA1.FI/10) >= 10)
					{
						strcpy(pText, _Float2String((float)(AA_all_data.AA_detail.AA_AA1.FI/10.0),0));
					}
					else
					{
						strcpy(pText, _Float2String((float)(AA_all_data.AA_detail.AA_AA1.FI/10.0),1));					
					}
				}
				else
				{
					strcpy(pText, "--");
				}
			}
			else
			{
				strcpy(pText, "--");
			}
			MultiText_SetParaData(hItem, pText); // 
			MultiText_SetTextRect(hItem, 30, 70, 13, 50, MultiText_Data);
			MultiText_SetTextFont(hItem, &GUI_Font32B_ASCII, MultiText_Data);
			MultiText_SetTextAlign(hItem, GUI_TA_HCENTER|GUI_TA_VCENTER, MultiText_Data);

			//FiAA MaxValue
			strcpy(pText, _Float2String((float)AlarmSettingPara.FiAAHighValue/10,1));
			MultiText_SetParaMaxValue(hItem, pText);
			MultiText_SetTextRect(hItem, 105, 25, 6, 25, MultiText_MaxValue);
			MultiText_SetTextFont(hItem, &GUI_Font_16, MultiText_MaxValue);
			MultiText_SetTextAlign(hItem, GUI_TA_LEFT, MultiText_MaxValue);
			//FiAA MinValue
			strcpy(pText, _Float2String((float)AlarmSettingPara.FiAALowValue/10,1));
			MultiText_SetParaMinValue(hItem, pText);
			MultiText_SetTextRect(hItem, 105, 25, 46, 25, MultiText_MinValue);//
			MultiText_SetTextFont(hItem, &GUI_Font_16, MultiText_MinValue);
			MultiText_SetTextAlign(hItem, GUI_TA_LEFT, MultiText_MinValue);
			//EtAA Data
			if(AA_all_data.AA_detail.AA_AA1.ID)
			{
				if((AA_Module_Flag.EtCO2ModuleExist)&&(EtCO2ModuleOff))
				{
					if((AA_all_data.AA_detail.AA_AA1.ET/10) >= 10)
					{
						strcpy(pText1, _Float2String((float)(AA_all_data.AA_detail.AA_AA1.ET/10.0),0));					
					}
					else
					{
						strcpy(pText1, _Float2String((float)(AA_all_data.AA_detail.AA_AA1.ET/10.0),1));					
					}				
				}
				else
				{
					strcpy(pText1, "--");
				}
			}
			else
			{
				strcpy(pText1, "--");
			}
			MultiText_SetOtherParaData(hItem, pText1); // 
			MultiText_SetTextRect(hItem, 125, 70, 13, 50, MultiText_Data1);
			MultiText_SetTextFont(hItem, &GUI_Font32B_ASCII, MultiText_Data1);
			MultiText_SetTextAlign(hItem, GUI_TA_HCENTER|GUI_TA_VCENTER, MultiText_Data1);

			//MaxValue
			strcpy(pText1, _Float2String((float)AlarmSettingPara.EtAAHighValue/10,1));
			MultiText_SetOtherParaMaxValue(hItem, pText1);
			MultiText_SetTextRect(hItem, 200, 20, 6, 20, MultiText_MaxValue1);
			MultiText_SetTextFont(hItem, &GUI_Font_16, MultiText_MaxValue1);
			MultiText_SetTextAlign(hItem, GUI_TA_RIGHT, MultiText_MaxValue1);
			//MinValue
			strcpy(pText1, _Float2String((float)AlarmSettingPara.EtAALowValue/10,1));
			MultiText_SetOtherParaMinValue(hItem, pText1);
			MultiText_SetTextRect(hItem, 200, 20, 46, 20, MultiText_MinValue1);
			MultiText_SetTextFont(hItem, &GUI_Font_16, MultiText_MinValue1);
			MultiText_SetTextAlign(hItem, GUI_TA_RIGHT, MultiText_MinValue1);
		}

		if((index = Get_MonitorRank_AA(DP_N2O)) != -1)
		{
			hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT11);//N2O
			//LeftMultiTextSetFrame(hItem);

			MultiText_SetParaName(hItem, Sys_Lib_GetDispStr(DP_N2O));
			MultiText_SetTextRect(hItem, 2, 30, 6, 25, MultiText_Name);
			MultiText_SetTextFont(hItem, &GUI_Font_20, MultiText_Name);
			MultiText_SetTextColor(hItem, N2OColor, MultiText_Name);
			MultiText_SetTextAlign(hItem, GUI_TA_LEFT, MultiText_Name);
			//unit
			MultiText_SetParaUnit(hItem, Sys_Lib_GetDispStr(DP_PERCENT));
			MultiText_SetTextRect(hItem, 2, 25, 46, 25, MultiText_Unit);
			MultiText_SetTextFont(hItem, &GUI_Font_20, MultiText_Unit);
			MultiText_SetTextColor(hItem, N2OColor, MultiText_Unit);
			//Data FiN2O EtN2O
			if((EtCO2ModuleOff)&&(!N2O_Switch)&&(AA_Module_Flag.EtCO2ModuleExist)&&(AA_Module_Flag.Module_State_Flag == Connect_Valid)\
				&&(AA_all_data.AA_detail.AA_N2O.Accuracy == ACCURACY_Iso))//djq modify 20161230 气道适配器异常处理
			{
				//FiN2O
				if(AA_all_data.AA_detail.AA_N2O.FI != 0xFF)
				{
					if(AA_all_data.AA_detail.AA_N2O.FI > 82)
					{
						strcpy(pText, "--");
					}
					else
					{
						strcpy(pText, _Float2String((float)(AA_all_data.AA_detail.AA_N2O.FI),0));
					}
				}
				else
				{
					strcpy(pText, "--");
				}

				//EtN2O
				if(AA_all_data.AA_detail.AA_N2O.ET != 0xFF)
				{
					if(AA_all_data.AA_detail.AA_N2O.ET > 82)
					{
						strcpy(pText1, "--");
					}
					else
					{
						strcpy(pText, _Float2String((float)(AA_all_data.AA_detail.AA_N2O.ET),0));
					}
				}
				else
				{
					strcpy(pText1, "--");
				}
			}
			else
			{
				strcpy(pText, "--");
				strcpy(pText1, "--");
			}
			MultiText_SetParaData(hItem, pText); // 
			MultiText_SetTextRect(hItem, 30, 70, 13, 50, MultiText_Data);
			MultiText_SetTextFont(hItem, &GUI_Font32B_ASCII, MultiText_Data);
			MultiText_SetTextColor(hItem, N2OColor, MultiText_Data);
			MultiText_SetTextAlign(hItem, GUI_TA_HCENTER|GUI_TA_VCENTER, MultiText_Data);

			MultiText_SetOtherParaData(hItem, pText1); // 
			MultiText_SetTextRect(hItem, 125, 70, 13, 50, MultiText_Data1);
			MultiText_SetTextFont(hItem, &GUI_Font32B_ASCII, MultiText_Data1);
			MultiText_SetTextColor(hItem, N2OColor, MultiText_Data1);
			MultiText_SetTextAlign(hItem, GUI_TA_HCENTER|GUI_TA_VCENTER, MultiText_Data1);

			//FiN2O MaxValue
			strcpy(pText, _Float2String((float)AlarmSettingPara.N2OHighValue,0));
			MultiText_SetParaMaxValue(hItem, pText);
			MultiText_SetTextRect(hItem, 105, 25, 6, 25, MultiText_MaxValue);
			MultiText_SetTextFont(hItem, &GUI_Font_16, MultiText_MaxValue);
			MultiText_SetTextAlign(hItem, GUI_TA_LEFT, MultiText_MaxValue);
			MultiText_SetTextColor(hItem, N2OColor, MultiText_MaxValue);
			//MinValue
			strcpy(pText, _Float2String((float)AlarmSettingPara.N2OLowValue,0));
			MultiText_SetParaMinValue(hItem, pText);
			MultiText_SetTextRect(hItem, 105, 25, 46, 25, MultiText_MinValue);
			MultiText_SetTextFont(hItem, &GUI_Font_16, MultiText_MinValue);
			MultiText_SetTextAlign(hItem, GUI_TA_LEFT, MultiText_MinValue);
			MultiText_SetTextColor(hItem, N2OColor, MultiText_MinValue);
		}

		if((index = Get_MonitorRank_AA(DP_MAC)) != -1)//MAC
		{
			hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT7);

			MultiText_SetParaName(hItem, Sys_Lib_GetDispStr(DP_MAC));
			MultiText_SetTextRect(hItem, 40, 70, 8, 50, MultiText_Name);
			MultiText_SetTextFont(hItem, &GUI_Font_32, MultiText_Name);
			//Data
			MacValue = AA_Para.MAC/10.0;
			if((EtCO2ModuleOff)&&(AA_Module_Flag.EtCO2ModuleExist)&&(AA_Module_Flag.Module_State_Flag == Connect_Valid)\
				&&(AA_all_data.AA_detail.AA_AA1.Accuracy == ACCURACY_Iso))//djq modify 20161230 气道适配器异常处理//zkq add 高精度允许监测
			{
				if((AA_Module_Flag.INIT_AA_MODULE_FLAG&(0x02)) == 0x02)
				{
					if(MacValue == 0.0)
					{
						strcpy(pText, "0");
					}
					else
					{
						if((int)MacValue > 100)//
						{
							strcpy(pText, "--");
						}	
						else
						{
							strcpy(pText, _Float2String((float)(MacValue/10+0.005),1));
						}
					}
				}
				else
				{
					strcpy(pText, "--");
				}
			}
			else
			{
				strcpy(pText, "--");
			}
			MultiText_SetParaData(hItem, pText); // pText
			MultiText_SetTextRect(hItem, 120, 80, 10, 50, MultiText_Data);
			MultiText_SetTextFont(hItem, &GUI_Font32B_ASCII, MultiText_Data);
		}
	}

	// Fi Et
	hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT8);
	//LeftMultiTextSetFrame(hItem);
	MultiText_SetParaName(hItem, Sys_Lib_GetDispStr(DP_FI));
	MultiText_SetTextRect(hItem, 55, 20, 3, 20, MultiText_Name);
	MultiText_SetTextFont(hItem, &GUI_Font_24, MultiText_Name);
	MultiText_SetOtherParaName(hItem, Sys_Lib_GetDispStr(DP_ET));
	MultiText_SetTextRect(hItem, 145, 20, 3, 20, MultiText_Name1);
	MultiText_SetTextFont(hItem, &GUI_Font_24, MultiText_Name1);
}



static void RectPaintParaInit(WM_HWIN hDlg)
{
	char *String;
	char pText[20] = {0};
	char pText1[20] = {0};
	WM_HWIN hItem[7];
	const GUI_FONT *font;
	int index = 0;
	float MacValue = 0;

	hItem[0] = WM_GetDialogItem(hDlg, GUI_ID_TEXT0);
	hItem[1] = WM_GetDialogItem(hDlg, GUI_ID_TEXT1);
	hItem[2] = WM_GetDialogItem(hDlg, GUI_ID_TEXT2);
	hItem[3] = WM_GetDialogItem(hDlg, GUI_ID_TEXT3);
	hItem[4] = WM_GetDialogItem(hDlg, GUI_ID_TEXT4);
	hItem[5] = WM_GetDialogItem(hDlg, GUI_ID_TEXT5);
	hItem[6] = WM_GetDialogItem(hDlg, GUI_ID_TEXT15);//  [2/16/2022 yaoyao add]右侧快捷监测区添加一个监测参数

//	MultiTextSetFrame(hItem[0]);
//	MultiTextSetFrame(hItem[1]);
//	MultiTextSetFrame(hItem[2]);
//	MultiTextSetFrame(hItem[3]);
//	MultiTextSetFrame(hItem[4]);
//	MultiTextSetFrame(hItem[5]);
//	MultiTextSetFrame(hItem[6]);//  [2/16/2022 yaoyao add]右侧快捷监测区添加一个监测参数

//	MultiText_SetTopFrameSize(hItem[0], 1);

	// 	if(LanguageType == Disp_Lg_Chinese)
	// 	{
	// 		font = &GUI_Font_15;
	// 	}
	// 	else
	// 	{
	// 		font = &GUI_Font_16;
	// 	}

	if((index = Get_MonitorRank_Rapid(DP_PPEAK)) != -1)//Ppeak
	{
		//Name
		MultiText_SetParaName(hItem[index], Sys_Lib_GetDispStr(DP_PPEAK));
		MultiText_SetTextRect(hItem[index], 8, 55, 5, 25, MultiText_Name);//  [2/17/2022 yaoyao ]控件宽度+15解决：其他语言显示不全
		MultiText_SetTextFont(hItem[index], &GUI_Font_20, MultiText_Name);
		//Data
		//  [2/21/2022 yaoyao modify]解决Ppeak参数在快捷监测区初始化有问题
		//strcpy(pText, _Float2String((float)MonitorPara.PpeakValue,0));
		if(BeathSettingPara.BreathMode == ACGO_MODE)
		{
			strcpy(pText, "--");
		}
		else
		{
			if(MonitorPara.PpeakValue > 1000)
			{				
				strcpy(pText, "--");
			}
			else if(MonitorPara.PpeakValue < 0)
			{	
				strcpy(pText, _Float2String((float)MonitorPara.PpeakValue,0));	
			}
			else
			{
				strcpy(pText, _Float2String((float)MonitorPara.PpeakValue/10,1));				
			}
		}
		//
		MultiText_SetParaData(hItem[index], pText); // pText
		MultiText_SetTextRect(hItem[index], 8, 100, 35, 50, MultiText_Data);
		MultiText_SetTextFont(hItem[index], &GUI_Font32B_ASCII, MultiText_Data);
		MultiText_SetTextColor(hItem[index], 0x01ffff, MultiText_Data);
		//Unit
		MultiText_SetParaUnit(hItem[index], Sys_Lib_GetDispStr(DP_CMH2O));
		MultiText_SetTextRect(hItem[index], 100, 50, 5, 30, MultiText_Unit);
		MultiText_SetTextFont(hItem[index], &GUI_Font_20, MultiText_Unit);
		MultiText_SetTextAlign(hItem[index], GUI_TA_RIGHT, MultiText_Unit);
		//MaxValue
		sprintf(pText,"%d",AlarmSettingPara.PressureHighValue);
		MultiText_SetParaMaxValue(hItem[index], pText);
		MultiText_SetTextRect(hItem[index], 120, 30, 35, 30, MultiText_MaxValue);
		MultiText_SetTextFont(hItem[index], &GUI_Font_16, MultiText_MaxValue);
		MultiText_SetTextAlign(hItem[index], GUI_TA_RIGHT, MultiText_MaxValue);
		//MinValue
		sprintf(pText,"%d",AlarmSettingPara.PressureLowValue);
		MultiText_SetParaMinValue(hItem[index], pText);
		MultiText_SetTextRect(hItem[index], 120, 30, 55, 30, MultiText_MinValue);
		MultiText_SetTextFont(hItem[index], &GUI_Font_16, MultiText_MinValue);
		MultiText_SetTextAlign(hItem[index], GUI_TA_RIGHT, MultiText_MinValue);
	}
	if((index = Get_MonitorRank_Rapid(DP_PEEP)) != -1)//PEEP
	{
		//ParaName
		MultiText_SetParaName(hItem[index], Sys_Lib_GetDispStr(DP_PEEP));
		MultiText_SetTextRect(hItem[index], 8, 55, 5, 25, MultiText_Name);
		MultiText_SetTextFont(hItem[index], &GUI_Font_20, MultiText_Name);
		//Data
		//  [2/21/2022 yaoyao]解决PEEP参数在快捷监测区初始化有问题
		//strcpy(pText, _Float2String((float)MonitorPara.PeepValue,0));
		if(BeathSettingPara.BreathMode == ACGO_MODE)
		{
			strcpy(pText, "--");
		}
		else
		{
			if(MonitorPara.PeepValue > 1000) //zkq 20161127
			{
				strcpy(pText, "--");
			}
			else if(MonitorPara.PeepValue < 0)
			{	
				strcpy(pText, _Float2String((float)MonitorPara.PeepValue,0));	
			}
			else
			{
				strcpy(pText, _Float2String((float)MonitorPara.PeepValue/10,1));	
			}
		}
		//
		MultiText_SetParaData(hItem[index], pText);
		MultiText_SetTextRect(hItem[index], 8, 100, 35, 50, MultiText_Data);
		MultiText_SetTextFont(hItem[index], &GUI_Font32B_ASCII, MultiText_Data);
		MultiText_SetTextColor(hItem[index], 0x01ffff, MultiText_Data);
		//ParaUnit
		MultiText_SetParaUnit(hItem[index], Sys_Lib_GetDispStr(DP_CMH2O));
		MultiText_SetTextRect(hItem[index], 100, 50, 5, 30, MultiText_Unit);
		MultiText_SetTextFont(hItem[index], &GUI_Font_20, MultiText_Unit);
		MultiText_SetTextAlign(hItem[index], GUI_TA_RIGHT, MultiText_Unit);
	}
	if((index = Get_MonitorRank_Rapid(DP_MV)) != -1)//MV
	{
		//ParaName
		MultiText_SetParaName(hItem[index], Sys_Lib_GetDispStr(DP_MV));
		MultiText_SetTextRect(hItem[index], 8, 55, 5, 25, MultiText_Name);
		MultiText_SetTextFont(hItem[index], &GUI_Font_20, MultiText_Name);
		//Data 
		if(MonitorPara.MinuteVolumeValue > 6000)
		{
			strcpy(pText, "--");
		}
		else if((MonitorPara.MinuteVolumeValue <= 6000) && (MonitorPara.MinuteVolumeValue >= 1000))
		{
			strcpy(pText, _Float2String((float)MonitorPara.MinuteVolumeValue/100, 1));
		}
		else if(MonitorPara.MinuteVolumeValue < 1000)
		{
			strcpy(pText, _Float2String((float)MonitorPara.MinuteVolumeValue/100, 2));
		}
		else if(MonitorPara.MinuteVolumeValue == 0)
		{
			strcpy(pText, _Float2String(0, 0));
		}
		MultiText_SetParaData(hItem[index], pText);
		MultiText_SetTextRect(hItem[index], 8, 100, 35, 50, MultiText_Data);
		MultiText_SetTextFont(hItem[index], &GUI_Font32B_ASCII, MultiText_Data);
		MultiText_SetTextColor(hItem[index], 0x01ffff, MultiText_Data);
		//ParaUnit
		MultiText_SetParaUnit(hItem[index], Sys_Lib_GetDispStr(DP_LMIN));
		MultiText_SetTextRect(hItem[index], 100, 50, 5, 30, MultiText_Unit);
		MultiText_SetTextFont(hItem[index], &GUI_Font_20, MultiText_Unit);
		MultiText_SetTextAlign(hItem[index], GUI_TA_RIGHT, MultiText_Unit);
		//MaxValue
		sprintf(pText,"%d",AlarmSettingPara.MVHighValue);
		MultiText_SetParaMaxValue(hItem[index], pText);
		MultiText_SetTextRect(hItem[index], 120, 30, 35, 30, MultiText_MaxValue);
		MultiText_SetTextFont(hItem[index], &GUI_Font_16, MultiText_MaxValue);
		MultiText_SetTextAlign(hItem[index], GUI_TA_RIGHT, MultiText_MaxValue);
		//MinValue
		sprintf(pText,"%d",AlarmSettingPara.MVLowValue);
		MultiText_SetParaMinValue(hItem[index], pText);
		MultiText_SetTextRect(hItem[index], 120, 30, 55, 30, MultiText_MinValue);
		MultiText_SetTextFont(hItem[index], &GUI_Font_16, MultiText_MinValue);
		MultiText_SetTextAlign(hItem[index], GUI_TA_RIGHT, MultiText_MinValue);
	}

	if((index = Get_MonitorRank_Rapid(DP_VTE)) != -1)//VTE
	{
		//ParaName
		MultiText_SetParaName(hItem[index], Sys_Lib_GetDispStr(DP_VTE));
		MultiText_SetTextRect(hItem[index], 8, 55, 5, 25, MultiText_Name);
		MultiText_SetTextFont(hItem[index], &GUI_Font_20, MultiText_Name);
		//Data
		//  [2/21/2022 yaoyao modify]
		//strcpy(pText, _Float2String((float)MonitorPara.VteValue,0));
		if(BeathSettingPara.BreathMode == ACGO_MODE)
		{
			strcpy(pText, "--");
		}
		else
		{
			if(MonitorPara.VteValue > 2500)//  20130812
			{			
				strcpy(pText, "--");
			}
			else if(MonitorPara.VteValue < 0)
			{	
				strcpy(pText, _Float2String((float)MonitorPara.VteValue,0));	
			}
			else
			{
				strcpy(pText, _Float2String((float)MonitorPara.VteValue,0));
			}
		}
		MultiText_SetParaData(hItem[index], pText);
		MultiText_SetTextRect(hItem[index], 8, 100, 35, 50, MultiText_Data);
		MultiText_SetTextFont(hItem[index], &GUI_Font32B_ASCII, MultiText_Data);
		MultiText_SetTextColor(hItem[index], 0x01ffff, MultiText_Data);
		//ParaUnit
		MultiText_SetParaUnit(hItem[index], Sys_Lib_GetDispStr(DP_ML));
		MultiText_SetTextRect(hItem[index], 100, 50, 5, 30, MultiText_Unit);
		MultiText_SetTextFont(hItem[index], &GUI_Font_20, MultiText_Unit);
		MultiText_SetTextAlign(hItem[index], GUI_TA_RIGHT, MultiText_Unit);
		//MaxValue
		sprintf(pText,"%d",AlarmSettingPara.VteHighValue);
		MultiText_SetParaMaxValue(hItem[index], pText);
		MultiText_SetTextRect(hItem[index], 120, 30, 35, 30, MultiText_MaxValue);
		MultiText_SetTextFont(hItem[index], &GUI_Font_16, MultiText_MaxValue);
		MultiText_SetTextAlign(hItem[index], GUI_TA_RIGHT, MultiText_MaxValue);
		//MinValue
		if(AlarmSettingPara.VteLowValue >= 20)
		{
			sprintf(pText,"%d",AlarmSettingPara.VteLowValue);
		}
		else
		{
			strcpy(pText, Sys_Lib_GetDispStr(DP_OFF));			
		}
		MultiText_SetParaMinValue(hItem[index], pText);
		MultiText_SetTextRect(hItem[index], 120, 30, 55, 30, MultiText_MinValue);
		MultiText_SetTextFont(hItem[index], &GUI_Font_16, MultiText_MinValue);
		MultiText_SetTextAlign(hItem[index], GUI_TA_RIGHT, MultiText_MinValue);
	}
	if((index = Get_MonitorRank_Rapid(DP_VTI)) != -1)//VTi
	{
		//ParaName
		MultiText_SetParaName(hItem[index], Sys_Lib_GetDispStr(DP_VTI));
		MultiText_SetTextRect(hItem[index], 8, 55, 5, 25, MultiText_Name);
		MultiText_SetTextFont(hItem[index], &GUI_Font_20, MultiText_Name);
		//Data
		if(BeathSettingPara.BreathMode == ACGO_MODE)//  [2/21/2022 yaoyao add]
		{
			strcpy(pText, "--");
		}
		else
		{
			strcpy(pText, _Float2String((float)MonitorPara.VtiValue,0));
		}
		MultiText_SetParaData(hItem[index], pText);
		MultiText_SetTextRect(hItem[index], 8, 100, 35, 50, MultiText_Data);
		MultiText_SetTextFont(hItem[index], &GUI_Font32B_ASCII, MultiText_Data);
		MultiText_SetTextColor(hItem[index], 0x01ffff, MultiText_Data);
		//ParaUnit
		MultiText_SetParaUnit(hItem[index], Sys_Lib_GetDispStr(DP_ML));
		MultiText_SetTextRect(hItem[index], 100, 50, 5, 30, MultiText_Unit);
		MultiText_SetTextFont(hItem[index], &GUI_Font_20, MultiText_Unit);
		MultiText_SetTextAlign(hItem[index], GUI_TA_RIGHT, MultiText_Unit);
	}
	if((index = Get_MonitorRank_Rapid(DP_F)) != -1)//VTE
	{
		//ParaName
		MultiText_SetParaName(hItem[index], Sys_Lib_GetDispStr(DP_F));
		MultiText_SetTextRect(hItem[index], 8, 55, 5, 25, MultiText_Name);
		MultiText_SetTextFont(hItem[index], &GUI_Font_20, MultiText_Name);
		//Data
		//  [2/21/2022 yaoyao modify]
		//strcpy(pText, _Float2String((float)MonitorPara.RateTotalValue,0));
		if(BeathSettingPara.BreathMode == ACGO_MODE)
		{
			strcpy(pText, "--");
		}
		else
		{
			if(MonitorPara.RateTotalValue > 100)
			{
				strcpy(pText, "--");
			}
			else
			{
				if(MonitorPara.RateTotalValue < 0)
				{
					MonitorPara.RateTotalValue = 0;
				}
				strcpy(pText, _Float2String((float)MonitorPara.RateTotalValue, 0));
			}
		}
		MultiText_SetParaData(hItem[index], pText);
		MultiText_SetTextRect(hItem[index], 8, 100, 35, 50, MultiText_Data);
		MultiText_SetTextFont(hItem[index], &GUI_Font32B_ASCII, MultiText_Data);
		MultiText_SetTextColor(hItem[index], 0x01ffff, MultiText_Data);
		//ParaUnit
		MultiText_SetParaUnit(hItem[index], Sys_Lib_GetDispStr(DP_BPM));
		MultiText_SetTextRect(hItem[index], 100, 50, 5, 30, MultiText_Unit);
		MultiText_SetTextFont(hItem[index], &GUI_Font_20, MultiText_Unit);
		MultiText_SetTextAlign(hItem[index], GUI_TA_RIGHT, MultiText_Unit);
		//MaxValue
		sprintf(pText,"%d",AlarmSettingPara.RateHighValue);
		MultiText_SetParaMaxValue(hItem[index], pText);
		MultiText_SetTextRect(hItem[index], 120, 30, 35, 30, MultiText_MaxValue);
		MultiText_SetTextFont(hItem[index], &GUI_Font_16, MultiText_MaxValue);
		MultiText_SetTextAlign(hItem[index], GUI_TA_RIGHT, MultiText_MaxValue);
		//MinValue
		sprintf(pText,"%d",AlarmSettingPara.RateLowValue);
		MultiText_SetParaMinValue(hItem[index], pText);
		MultiText_SetTextRect(hItem[index], 120, 30, 55, 30, MultiText_MinValue);
		MultiText_SetTextFont(hItem[index], &GUI_Font_16, MultiText_MinValue);
		MultiText_SetTextAlign(hItem[index], GUI_TA_RIGHT, MultiText_MinValue);
	}

	if((index = Get_MonitorRank_Rapid(DP_FIO2)) != -1)//FiO2
	{
		//ParaName
		MultiText_SetParaName(hItem[index], Sys_Lib_GetDispStr(DP_FIO2));
		MultiText_SetTextRect(hItem[index], 8, 55, 5, 25, MultiText_Name);
		MultiText_SetTextFont(hItem[index], &GUI_Font_20, MultiText_Name);
		//ParaUnit
		MultiText_SetParaUnit(hItem[index], Sys_Lib_GetDispStr(DP_PERCENT));
		MultiText_SetTextRect(hItem[index], 100, 50, 5, 30, MultiText_Unit);
		MultiText_SetTextFont(hItem[index], &GUI_Font_20, MultiText_Unit);
		MultiText_SetTextAlign(hItem[index], GUI_TA_RIGHT, MultiText_Unit);
		//Data
		if(!O2ModuleOff)
		{
#ifdef Pre_ACGO
			if(BeathSettingPara.BreathMode == ACGO_MODE)
			{
				strcpy(pText,"--");		
			}
			else
#endif
			{
				if(O2_Sensor_Disp_(MonitorPara.FiO2Value))
				{
					strcpy(pText,"--");
				}
				else if(O2_Sensor_Disp100(MonitorPara.FiO2Value))
				{
					strcpy(pText, _Float2String((float)100,0));
				}
				else
				{
					strcpy(pText, _Float2String((float)MonitorPara.FiO2Value,0));
				}
			}
		}
		else
		{
			strcpy(pText,"--");
		}
		MultiText_SetParaData(hItem[index], pText);
		MultiText_SetTextRect(hItem[index], 8, 100, 35, 50, MultiText_Data);
		MultiText_SetTextFont(hItem[index], &GUI_Font32B_ASCII, MultiText_Data);
		MultiText_SetTextColor(hItem[index], 0x01ffff, MultiText_Data);
		//MaxValue
		if(AlarmSettingPara.FiO2HighValue!=101)
		{
			sprintf(pText,"%d",AlarmSettingPara.FiO2HighValue);
		}
		else
		{
			strcpy(pText, Sys_Lib_GetDispStr(DP_OFF));
		}
		MultiText_SetParaMaxValue(hItem[index], pText);
		MultiText_SetTextRect(hItem[index], 120, 30, 35, 30, MultiText_MaxValue);
		MultiText_SetTextFont(hItem[index], &GUI_Font_16, MultiText_MaxValue);
		MultiText_SetTextAlign(hItem[index], GUI_TA_RIGHT, MultiText_MaxValue);
		//MinValue
		sprintf(pText,"%d",AlarmSettingPara.FiO2LowValue);
		MultiText_SetParaMinValue(hItem[index], pText);
		MultiText_SetTextRect(hItem[index], 120, 30, 55, 30, MultiText_MinValue);
		MultiText_SetTextFont(hItem[index], &GUI_Font_16, MultiText_MinValue);
		MultiText_SetTextAlign(hItem[index], GUI_TA_RIGHT, MultiText_MinValue);
	}
	//  [2/16/2022 yaoyao 注释]将CO2永远在左侧
// 	if((index = Get_MonitorRank_Rapid(DP_ETCO2)) != -1)//EtCO2
// 	{
// 		//ParaName
// 		MultiText_SetParaName(hItem[index], Sys_Lib_GetDispStr(DP_ETCO2));
// 		MultiText_SetTextRect(hItem[index], 8, 35, 3, 15, MultiText_Name);
// 		MultiText_SetTextFont(hItem[index], &GUI_Font_16, MultiText_Name);
// 		//ParaUnit
// 		switch(ETCO2Unit)
// 		{
// 		case 0:
// 			MultiText_SetParaUnit(hItem[index], Sys_Lib_GetDispStr(DP_PERCENT));
// 			break;
// 		case 1:
// 			MultiText_SetParaUnit(hItem[index], Sys_Lib_GetDispStr(DP_MMHG));
// 			break;
// 		case 2:
// 			MultiText_SetParaUnit(hItem[index], Sys_Lib_GetDispStr(DP_KPA));
// 			break;
// 		default:break;
// 		}
// 		MultiText_SetTextRect(hItem[index], 75, 40, 3, 15, MultiText_Unit);
// 		MultiText_SetTextFont(hItem[index], &GUI_Font_16, MultiText_Unit);
// 		MultiText_SetTextAlign(hItem[index], GUI_TA_RIGHT, MultiText_Unit);
// 		//Data
// 		if((!AA_Module_Flag.EtCO2ModuleExist)||(!EtCO2ModuleOff))
// 		{
// 			strcpy(pText, "--");
// 		}
// 		else
// 		{   
// 			if((ETCO2Unit == 0)||(ETCO2Unit == 2))
// 			{
// 				if(CO2_Parameters.etCO2 > 150/*133*/)
// 				{					
// 					strcpy(pText, "--");
// 				}
// 				else
// 				{
// 					strcpy(pText, _Float2String((float)(CO2_Parameters.etCO2/10),1));
// 				}
// 			}
// 			else
// 			{
// 				if(CO2_Parameters.etCO2 > 113/*100*/)
// 				{					
// 					strcpy(pText, "--");
// 				}
// 				else
// 				{
// 					strcpy(pText, _Float2String((float)(CO2_Parameters.etCO2),1));
// 				}
// 			}
// 		}
// 		MultiText_SetParaData(hItem[index], pText);
// 		MultiText_SetTextRect(hItem[index], 8, 75, 23, 50, MultiText_Data);
// 		MultiText_SetTextFont(hItem[index], &GUI_Font32B_ASCII, MultiText_Data);	
// 		//MaxValue
// 		if((ETCO2Unit == 0)||(ETCO2Unit == 2))
// 		{
// 			strcpy(pText,_Float2String((float)AlarmSettingPara.EtCO2HighValue/10,1));
// 		}
// 		else
// 		{
// 			sprintf(pText, "%d", AlarmSettingPara.EtCO2HighValue);
// 		}
// 		MultiText_SetParaMaxValue(hItem[index], pText);
// 		MultiText_SetTextRect(hItem[index], 85, 30, 20, 15, MultiText_MaxValue);
// 		MultiText_SetTextFont(hItem[index], &GUI_Font_16, MultiText_MaxValue);
// 		MultiText_SetTextAlign(hItem[index], GUI_TA_RIGHT, MultiText_MaxValue);
// 		//MinValue
// 		if((ETCO2Unit == 0)||(ETCO2Unit == 2))
// 		{
// 			strcpy(pText, _Float2String((float)AlarmSettingPara.EtCO2LowValue /10,1));
// 			if(AlarmSettingPara.EtCO2LowValue == AlarmSettingPara.EtCO2HighValue) 
// 			{
// 				strcpy(pText, _Float2String((float)(AlarmSettingPara.EtCO2LowValue-1)/10,1));
// 			}        
// 		}
// 		else
// 		{
// 			sprintf(pText, "%d", AlarmSettingPara.EtCO2LowValue);
// 			if(AlarmSettingPara.EtCO2LowValue == AlarmSettingPara.EtCO2HighValue) 
// 			{
// 				sprintf(pText, "%d", AlarmSettingPara.EtCO2LowValue - 1);
// 			}
// 		}
// 		MultiText_SetParaMinValue(hItem[index], pText);
// 		MultiText_SetTextRect(hItem[index], 85, 30, 35, 15, MultiText_MinValue);
// 		MultiText_SetTextFont(hItem[index], &GUI_Font_16, MultiText_MinValue);
// 		MultiText_SetTextAlign(hItem[index], GUI_TA_RIGHT, MultiText_MinValue);
// 	}

	//  [2/16/2022 yaoyao add]Pulse监测参数移到右边
	if((index = Get_MonitorRank_Rapid(DP_PULSE)) != -1)
	{
		//ParaName
		MultiText_SetParaName(hItem[index], Sys_Lib_GetDispStr(DP_PULSE));
		MultiText_SetTextRect(hItem[index], 8, 60, 5, 25, MultiText_Name);
		MultiText_SetTextFont(hItem[index], &GUI_Font_20, MultiText_Name);
		//ParaUnit
		MultiText_SetParaUnit(hItem[index], Sys_Lib_GetDispStr(DP_BPM));
		MultiText_SetTextRect(hItem[index], 100, 50, 5, 30, MultiText_Unit);
		MultiText_SetTextFont(hItem[index], &GUI_Font_20, MultiText_Unit);
		MultiText_SetTextAlign(hItem[index], GUI_TA_RIGHT, MultiText_Unit);
		//Data
		if((SpO2ModuleExist) && (!SpO2ModuleOff))
		{
			if((SpO2_Parameters.PulseRate > 0) && (SpO2_Parameters.PulseRate < 250))
			{
				strcpy(pText, _Float2String((float)SpO2_Parameters.PulseRate,0));
			}
			else
			{
				strcpy(pText, "--");
			}
		}
		else
		{
			strcpy(pText, "--");
		}
		MultiText_SetParaData(hItem[index], pText);
		MultiText_SetTextRect(hItem[index], 8, 100, 35, 50, MultiText_Data);
		MultiText_SetTextFont(hItem[index], &GUI_Font32B_ASCII, MultiText_Data);	
		MultiText_SetTextColor(hItem[index], 0x01ffff, MultiText_Data);
		//MaxValue
// 		sprintf(pText, "%d", AlarmSettingPara.PulseHighValue);
// 		MultiText_SetParaMaxValue(hItem[index], pText);
		MultiText_SetTextRect(hItem[index], 120, 30, 35, 30, MultiText_MaxValue);
		MultiText_SetTextFont(hItem[index], &GUI_Font_16, MultiText_MaxValue);
		MultiText_SetTextAlign(hItem[index], GUI_TA_RIGHT, MultiText_MaxValue);
		//MinValue
// 		sprintf(pText, "%d", AlarmSettingPara.PulseLowValue);
// 		MultiText_SetParaMinValue(hItem[index], pText);
		MultiText_SetTextRect(hItem[index], 120, 30, 55, 30, MultiText_MinValue);
		MultiText_SetTextFont(hItem[index], &GUI_Font_16, MultiText_MinValue);
		MultiText_SetTextAlign(hItem[index], GUI_TA_RIGHT, MultiText_MinValue);
	}
	//  [2/16/2022 yaoyao add]SpO2
	if((index = Get_MonitorRank_Rapid(DP_SPO2)) != -1)
	{
		//ParaName
		MultiText_SetParaName(hItem[index], Sys_Lib_GetDispStr(DP_SPO2));
		MultiText_SetTextRect(hItem[index], 8, 60, 5, 25, MultiText_Name);
		MultiText_SetTextFont(hItem[index], &GUI_Font_20, MultiText_Name);
		//ParaUnit
		MultiText_SetParaUnit(hItem[index], Sys_Lib_GetDispStr(DP_PERCENT));
		MultiText_SetTextRect(hItem[index], 100, 50, 5, 30, MultiText_Unit);
		MultiText_SetTextFont(hItem[index], &GUI_Font_20, MultiText_Unit);
		MultiText_SetTextAlign(hItem[index], GUI_TA_RIGHT, MultiText_Unit);
		//Data
		if((SpO2ModuleExist) && (!SpO2ModuleOff))
		{
			if((SpO2_Parameters.SpO2 > 0) && (SpO2_Parameters.SpO2 <= 100))
			{
				strcpy(pText, _Float2String((float)SpO2_Parameters.SpO2,0));				
			}
			else
			{				
				strcpy(pText, "--");
			}
		}
		else
		{
			strcpy(pText, "--");
		}
		MultiText_SetParaData(hItem[index], pText);
		MultiText_SetTextRect(hItem[index], 8, 100, 35, 50, MultiText_Data);
		MultiText_SetTextFont(hItem[index], &GUI_Font32B_ASCII, MultiText_Data);	
		MultiText_SetTextColor(hItem[index], 0x01ffff, MultiText_Data);
		//MaxValue
		if(AlarmSettingPara.SpO2HighValue>=100)
		{
			strcpy(pText, Sys_Lib_GetDispStr(DP_OFF));
		}
		else
		{
			sprintf(pText, "%d", AlarmSettingPara.SpO2HighValue);
		}		
		MultiText_SetParaMaxValue(hItem[index], pText);
		MultiText_SetTextRect(hItem[index], 120, 30, 35, 30, MultiText_MaxValue);
		MultiText_SetTextFont(hItem[index], &GUI_Font_16, MultiText_MaxValue);
		MultiText_SetTextAlign(hItem[index], GUI_TA_RIGHT, MultiText_MaxValue);
		//MinValue
		sprintf(pText, "%d", AlarmSettingPara.SpO2LowValue);
		MultiText_SetParaMinValue(hItem[index], pText);
		MultiText_SetTextRect(hItem[index], 120, 30, 55, 30, MultiText_MinValue);
		MultiText_SetTextFont(hItem[index], &GUI_Font_16, MultiText_MinValue);
		MultiText_SetTextAlign(hItem[index], GUI_TA_RIGHT, MultiText_MinValue);
	}
	//  [2/16/2022 yaoyao add]添加右侧快捷监测参数PMEAN
	if ((index = Get_MonitorRank_Rapid(DP_PMEAN)) != -1)
	{
		//Name
		MultiText_SetParaName(hItem[index], Sys_Lib_GetDispStr(DP_PMEAN));
		MultiText_SetTextRect(hItem[index], 8, 60, 5, 25, MultiText_Name);
		MultiText_SetTextFont(hItem[index], &GUI_Font_20, MultiText_Name);
		//Data
		if(BeathSettingPara.BreathMode == ACGO_MODE)
		{
			strcpy(pText, "--");
		}
		else
		{
			if(MonitorPara.PmeanValue > 1000)
			{
				strcpy(pText, "--");
			}
			else if(MonitorPara.PmeanValue < 0)
			{	
				strcpy(pText, _Float2String((float)MonitorPara.PmeanValue,0));	
			}
			else
			{
				strcpy(pText, _Float2String((float)MonitorPara.PmeanValue/10,1));				
			}
		}
		MultiText_SetParaData(hItem[index], pText);
		MultiText_SetTextRect(hItem[index], 8, 100, 35, 50, MultiText_Data);
		MultiText_SetTextFont(hItem[index], &GUI_Font32B_ASCII, MultiText_Data);
		MultiText_SetTextColor(hItem[index], 0x01ffff, MultiText_Data);
		//Unit
		MultiText_SetParaUnit(hItem[index], Sys_Lib_GetDispStr(DP_CMH2O));
		MultiText_SetTextRect(hItem[index], 100, 50, 5, 30, MultiText_Unit);
		MultiText_SetTextFont(hItem[index], &GUI_Font_20, MultiText_Unit);
		MultiText_SetTextAlign(hItem[index], GUI_TA_RIGHT, MultiText_Unit);
	}
	LeftRectPaintParaInit(hDlg);
}


/***************************************************************************************************
*bit7:ACGO CLOSE
*bit6:ACGO OPEN
*bit5:MANUAL CLOSE
*bit4:MANUAL OPEN
*/

#define MANUAL_OPEN  (unsigned char)(1<<4)
#define MANUAL_CLOSE (unsigned char)(1<<5)
#define ACGO_OPEN    (unsigned char)(1<<6)
#define ACGO_CLOSE   (unsigned char)(1<<7)

void AlarmLimitParaChange(ALARM_SET_PARA *AlarmPara,unsigned char *change_flag)
{
	WM_MESSAGE Msg;
	unsigned char i = 0;

	if( (((*change_flag&MANUAL_OPEN) == MANUAL_OPEN) && (BeathSettingPara.BreathMode != ACGO_MODE  ))//手动开 && 当前模式是机控
		|| (((*change_flag&ACGO_CLOSE)  == ACGO_CLOSE)  && (BeathSettingPara.BreathMode == MANUAL_MODE))//ACGO关 && 当前是手动模式
		)//病人类型改变
	{
		Min_Limit = 0;//ACGO切换为机控，开始报警检测重新计时监测 
		*change_flag  &= 0x0F;//高四位清零
		//保存机控下相关参数的数值
		if(Alarm_SaveVent_para.PATIENTTYPE_ChangeFlag != 1)
		{
			Alarm_SaveVent_para.PressureLowValue = AlarmPara->PressureLowValue;
			//Alarm_SaveVent_para.PressureHighValue = AlarmPara->PressureHighValue;
			Alarm_SaveVent_para.RateLowValue = AlarmPara->RateLowValue;
			Alarm_SaveVent_para.RateHighValue = AlarmPara->RateHighValue;
			Alarm_SaveVent_para.MVLowValue = AlarmPara->MVLowValue;
			Alarm_SaveVent_para.MVHighValue = AlarmPara->MVHighValue;
			Alarm_SaveVent_para.VteLowValue = AlarmPara->VteLowValue;
			Alarm_SaveVent_para.VteHighValue = AlarmPara->VteHighValue;
		}
		//相关参数重新赋值
		AlarmPara->PressureLowValue = 0;
		AlarmPara->RateLowValue = 0;
		AlarmPara->RateHighValue = 100;
		AlarmPara->MVLowValue = 0;
		AlarmPara->MVHighValue = 99;
// #ifdef VT_SET_MAX_MIN
		AlarmPara->VteLowValue = 0;
// #else
// 		AlarmPara->VteLowValue = 10;
// #endif
		AlarmPara->VteHighValue = 2000;

		if(MenuType==ALARM_MENU)
		{
			if(WM_IsWindow(Menu_hWin.Alarm_Win.AlarmWin_Lim1))//ylk modify 20181128
			{
				WM_SendMessageNoPara(Menu_hWin.Alarm_Win.AlarmWin_Lim1,WM_INIT_DIALOG);
			}
		}
		if(StandbyState == 0)
		{
			if(WM_IsWindow(Menu_hWin.HomeWin.ParentWin))
			{
				WM_SendMessageNoPara(Menu_hWin.HomeWin.ParentWin,WM_INIT_DIALOG);
			}
		}
	}
	else if( (*change_flag&MANUAL_CLOSE) == MANUAL_CLOSE || (*change_flag&ACGO_OPEN) == ACGO_OPEN )//手动关或ACGO开
	{
		Alarm_SaveVent_para.PATIENTTYPE_ChangeFlag = 0;
		*change_flag  &= 0x0F;//高四位清零
		if(BeathSettingPara.BreathMode != ACGO_MODE)//手动关 & 当前是机控状态
		{
			Min_Limit = 0;//手动切换为机控，开始报警检测重新计时监测 
			if(AlarmsWork.presentAlarms & ALARM_MV_LOW)//清除报警
			{
				AlarmsWork.presentAlarms &= ~ALARM_MV_LOW;
			}
			if(AlarmsWork.presentAlarms & ALARM_MV_HIGH)//清除报警
			{
				AlarmsWork.presentAlarms &= ~ALARM_MV_HIGH;
			}
			if(AlarmsWork.presentAlarms & ALARM_RATE_LOW)//清除报警
			{
				AlarmsWork.presentAlarms &= ~ALARM_RATE_LOW;
			}
			if(AlarmsWork.presentAlarms1 & ALARM_RATE_HIGH)//清除报警
			{
				AlarmsWork.presentAlarms1 &= ~ALARM_RATE_HIGH;
			}
		}
		//还原机控下相关参数的数值
		if(Alarm_SaveVent_para.PressureLowValue >= AlarmPara->PressureHighValue)
        {
            AlarmPara->PressureLowValue = AlarmPara->PressureHighValue - 5;
        }
        else
        {
            AlarmPara->PressureLowValue = Alarm_SaveVent_para.PressureLowValue;
        }
		//AlarmPara->PressureHighValue = Alarm_SaveVent_para.PressureHighValue;
		AlarmPara->RateLowValue = Alarm_SaveVent_para.RateLowValue;
		AlarmPara->RateHighValue = Alarm_SaveVent_para.RateHighValue;
		AlarmPara->MVLowValue = Alarm_SaveVent_para.MVLowValue;
		AlarmPara->MVHighValue = Alarm_SaveVent_para.MVHighValue;
		AlarmPara->VteLowValue = Alarm_SaveVent_para.VteLowValue;
		AlarmPara->VteHighValue = Alarm_SaveVent_para.VteHighValue;

		if(MenuType==ALARM_MENU)
		{
			if(WM_IsWindow(Menu_hWin.Alarm_Win.AlarmWin_Lim1))//ylk modify 20181128
			{
				WM_SendMessageNoPara(Menu_hWin.Alarm_Win.AlarmWin_Lim1,WM_INIT_DIALOG);
			}
		}
		if(StandbyState == 0)
		{
			if(WM_IsWindow(Menu_hWin.HomeWin.ParentWin))
			{
				WM_SendMessageNoPara(Menu_hWin.HomeWin.ParentWin,WM_INIT_DIALOG);
			}
		}
	}
}

/***************************************************************************************************
* @brief  报警限初始化
* @para   
* @retval 
*/
static void AlarmLimitParaInit(WM_HWIN hDlg,ALARM_SET_PARA *Para,int index)
{
    WM_HWIN hWin;
    WM_HWIN hButton3; //  20130717

	WM_HWIN hButton4; //add ylk 20170908
	WM_HWIN hButton5;
	WM_HWIN hButton6;
	WM_HWIN hButton7;
	WM_HWIN hButton8;
	WM_HWIN hButton9;

    int j=0; //  20130522
    if(index==0)
    {
        hWin = WM_GetDialogItem(hDlg, GUI_ID_BUTTON0);
        {
            ButtonRotation_SetDataInfo(hWin,10,99,Para->PressureHighValue,1,0); //1 cpp md 20130402
        }
        //*/
        ButtonRotation_SetData(hWin,Para->PressureHighValue);
        ButtonRotation_SetTriangleSign(hWin,0);//按下->切换标签->再次进入 不显示按下 
        hWin = WM_GetDialogItem(hDlg, GUI_ID_BUTTON1);
        ButtonRotation_SetDataInfo(hWin,0,98,Para->PressureLowValue,1,0);//1,98 cpp md 20130402
        ButtonRotation_SetData(hWin,Para->PressureLowValue);
        ButtonRotation_SetTriangleSign(hWin,0);
//
        hWin = WM_GetDialogItem(hDlg, GUI_ID_BUTTON2);
        ButtonRotation_SetDataInfo(hWin,1,99,Para->MVHighValue,1,0);
        ButtonRotation_SetData(hWin,Para->MVHighValue);
        ButtonRotation_SetTriangleSign(hWin,0);
        hWin = WM_GetDialogItem(hDlg, GUI_ID_BUTTON3);
        ButtonRotation_SetDataInfo(hWin,0,98,Para->MVLowValue,1,0);
        ButtonRotation_SetData(hWin,Para->MVLowValue); 
        ButtonRotation_SetTriangleSign(hWin,0);
//
        hWin = WM_GetDialogItem(hDlg, GUI_ID_BUTTON4);
        ButtonRotation_SetDataInfo(hWin,1,100,Para->RateHighValue,1,0);
        ButtonRotation_SetData(hWin,Para->RateHighValue);
        ButtonRotation_SetTriangleSign(hWin,0);
        hWin = WM_GetDialogItem(hDlg, GUI_ID_BUTTON5);
        ButtonRotation_SetDataInfo(hWin,0,99,Para->RateLowValue,1,0);
        ButtonRotation_SetData(hWin,Para->RateLowValue);
        ButtonRotation_SetTriangleSign(hWin,0);
//
        hWin = WM_GetDialogItem(hDlg, GUI_ID_BUTTON6);
        ButtonRotation_SetDataInfo(hWin,19,101,Para->FiO2HighValue,1,0);//djq modify 20161127 22~101 → 19~101
        ButtonRotation_SetDataInvildSign(hWin,2);//设置成上限OFF 
        ButtonRotation_SetData(hWin,Para->FiO2HighValue);
        ButtonRotation_SetTriangleSign(hWin,0);
        hWin = WM_GetDialogItem(hDlg, GUI_ID_BUTTON7);
        ButtonRotation_SetDataInfo(hWin,18,99,Para->FiO2LowValue,1,0);  //djq modify 20161127 20~99 → 18~99
        ButtonRotation_SetData(hWin,Para->FiO2LowValue);
        ButtonRotation_SetTriangleSign(hWin,0);

        hWin = WM_GetDialogItem(hDlg, GUI_ID_BUTTON8);
		ButtonRotation_SetDataInfo(hWin,10,2000,Para->VteHighValue,10,0);
		ButtonRotation_SetData(hWin,Para->VteHighValue);
        ButtonRotation_SetTriangleSign(hWin,0); 

        hWin = WM_GetDialogItem(hDlg, GUI_ID_BUTTON9);
		ButtonRotation_SetDataInfo(hWin,0,1990,Para->VteLowValue,10,0);
		ButtonRotation_SetDataInvildSign(hWin,1);//设置成下限OFF  //  20130328
        ButtonRotation_SetData(hWin,Para->VteLowValue);
        ButtonRotation_SetTriangleSign(hWin,0);

        hWin = WM_GetDialogItem(hDlg, GUI_ID_BUTTON10);
        ButtonRotation_SetDataInfo(hWin,10,60,Para->ApneaTimerValue,5,0);
        ButtonRotation_SetData(hWin,Para->ApneaTimerValue);
        ButtonRotation_SetTriangleSign(hWin,0);
    }
    else if(index == 1)
    {
        hWin = WM_GetDialogItem(hDlg, GUI_ID_BUTTON0);
        if((ETCO2Unit == 0)||(ETCO2Unit == 2)) //  20130502
        {
            ButtonRotation_SetDataInfo(hWin,1,150/*133*/,(int)(Para->EtCO2HighValue),1,1);
            ButtonRotation_SetData(hWin,(int)(Para->EtCO2HighValue));
        }
        else
        {
            ButtonRotation_SetDataInfo(hWin,1,113,Para->EtCO2HighValue,1,0);
            ButtonRotation_SetData(hWin,Para->EtCO2HighValue);
        }
        ButtonRotation_SetTriangleSign(hWin,0);
//      if(!AA_Module_Flag.EtCO2ModuleExist || !EtCO2ModuleOff) //ylk modify 20181220
		if((!AA_Module_Flag.EtCO2ModuleExist)||(!EtCO2ModuleOff) \
			|| (!(EtCO2_KM7002_Config_On || EtCO2_Config_On \
			|| Masimo_EtCO2_Config_On || MasimoISA_EtCO2_Config_On || Drager_EtCO2_Config_On)))
        {
            WM_DisableWindow(hWin);
        }
		//
        hWin = WM_GetDialogItem(hDlg, GUI_ID_BUTTON1);
        if((ETCO2Unit == 0)||(ETCO2Unit == 2)) //  20130502
        {
            ButtonRotation_SetDataInfo(hWin,0,149/*132*/,Para->EtCO2LowValue,1,1);
            ButtonRotation_SetData(hWin,Para->EtCO2LowValue);
            if(Para->EtCO2HighValue == Para->EtCO2LowValue) //  20130531
            {
                ButtonRotation_SetDataInfo(hWin,0,149/*132*/,Para->EtCO2LowValue-1,1,1);
                ButtonRotation_SetData(hWin,Para->EtCO2LowValue-1);
            }
        }
        else
        {
            ButtonRotation_SetDataInfo(hWin,0,112,Para->EtCO2LowValue,1,0);
            ButtonRotation_SetData(hWin,Para->EtCO2LowValue);
            if(Para->EtCO2HighValue == Para->EtCO2LowValue) //  20130531
            {
                ButtonRotation_SetDataInfo(hWin,0,112,Para->EtCO2LowValue-1,1,0);
                ButtonRotation_SetData(hWin,Para->EtCO2LowValue-1);
            }
        }
        ButtonRotation_SetTriangleSign(hWin,0);
//      if(!AA_Module_Flag.EtCO2ModuleExist || !EtCO2ModuleOff) //ylk modify 20181220
		if((!AA_Module_Flag.EtCO2ModuleExist)||(!EtCO2ModuleOff) \
			|| (!(EtCO2_KM7002_Config_On || EtCO2_Config_On \
			|| Masimo_EtCO2_Config_On || MasimoISA_EtCO2_Config_On || Drager_EtCO2_Config_On)))
        {
            WM_DisableWindow(hWin);
        }
		//
        hWin = WM_GetDialogItem(hDlg, GUI_ID_BUTTON2);
        if((ETCO2Unit == 0)||(ETCO2Unit == 2)) //  20130502 
        {
            ButtonRotation_SetDataInfo(hWin,1,150/*133*/,(int)(Para->FiCO2HighValue),1,1);
            ButtonRotation_SetData(hWin,(int)(Para->FiCO2HighValue));
        }
        else
        {
            ButtonRotation_SetDataInfo(hWin,1,113,Para->FiCO2HighValue,1,0);
            ButtonRotation_SetData(hWin,Para->FiCO2HighValue);
        }
        ButtonRotation_SetTriangleSign(hWin,0);
//      if(!AA_Module_Flag.EtCO2ModuleExist || !EtCO2ModuleOff) //ylk modify 20181220
		if((!AA_Module_Flag.EtCO2ModuleExist)||(!EtCO2ModuleOff) \
			|| (!(EtCO2_KM7002_Config_On || EtCO2_Config_On \
			|| Masimo_EtCO2_Config_On || MasimoISA_EtCO2_Config_On || Drager_EtCO2_Config_On)))
        {
            WM_DisableWindow(hWin);
        }
		//      
        #if 0  //cpp del 20130717
        hWin = WM_GetDialogItem(hDlg, GUI_ID_BUTTON3);
        if((ETCO2Unit == 0)||(ETCO2Unit == 2)) //  20130502
        {
            ButtonRotation_SetDataInfo(hWin,0,132,Para->FiCO2LowValue,1,1);
            ButtonRotation_SetData(hWin,Para->FiCO2LowValue);
            if(Para->FiCO2HighValue == Para->FiCO2LowValue) //  20130531
            {
                ButtonRotation_SetDataInfo(hWin,0,132,Para->FiCO2LowValue-1,1,1);
                ButtonRotation_SetData(hWin,Para->FiCO2LowValue-1);
            }
        }
        else
        {
            ButtonRotation_SetDataInfo(hWin,0,99,Para->FiCO2LowValue,1,0);
            ButtonRotation_SetData(hWin,Para->FiCO2LowValue);
            if(Para->FiCO2HighValue == Para->FiCO2LowValue) //  20130531
            {
                ButtonRotation_SetDataInfo(hWin,0,99,Para->FiCO2LowValue-1,1,0);
                ButtonRotation_SetData(hWin,Para->FiCO2LowValue-1);
            }
        }
        ButtonRotation_SetTriangleSign(hWin,0);
        if(!AA_Module_Flag.EtCO2ModuleExist || !EtCO2ModuleOff)
        {
            WM_DisableWindow(hWin);
        }
        
        #else
        hButton3 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON3);
        if((ETCO2Unit == 0)||(ETCO2Unit == 2))
        {
            ButtonRotation_SetDataInfo(hButton3,0,0,0,1,1);
        }
        else
        {
            ButtonRotation_SetDataInfo(hButton3,0,0,0,1,0);
        }
        ButtonRotation_SetTriangleSign(hButton3,0);
        ButtonRotation_SetData(hButton3,0);
        WM_DisableWindow(hButton3);
        WM_HideWindow(hButton3);
        #endif
		//
		hWin = WM_GetDialogItem(hDlg,GUI_ID_BUTTON4);
        ButtonRotation_SetDataInfo(hWin,1,82,Para->N2OHighValue,1,0);
        ButtonRotation_SetData(hWin,Para->N2OHighValue); //cpp add 20130626
        ButtonRotation_SetTriangleSign(hWin,0);
//      if(AA_Module_Flag.EtCO2ModuleExist == 0 || EtCO2ModuleOff == 0 ||  !(AA_all_data.AA_state.dev_info.module_type & 0x04) )
		if(!EtCO2ModuleOff)
		{
            WM_DisableWindow(hWin);
        }
		if(!((Masimo_EtCO2_Config_On || MasimoISA_EtCO2_Config_On || Drager_EtCO2_Config_On) && AA_Module_Flag.EtCO2ModuleExist == 2))
		{
			WM_DisableWindow(hWin);
		}
		if((AA_Module_Flag.EtCO2ModuleExist == 2)&&(AA_all_data.AA_state.dev_info.module_type == AA_PHASEIN_CO2ONLY))
		{
			WM_DisableWindow(hWin);
		}
        if((Masimo_EtCO2_Config_On || MasimoISA_EtCO2_Config_On || Drager_EtCO2_Config_On) && AA_Module_Flag.PhaseInTypeModuleExist == 6)  //gyh  add  20230727 MASIMO旁流CO2屏蔽AA报警设置
		{
			WM_DisableWindow(hWin);
		}
        //
        hWin = WM_GetDialogItem(hDlg,GUI_ID_BUTTON5);
        ButtonRotation_SetDataInfo(hWin,0,99,Para->N2OLowValue,1,0);
        ButtonRotation_SetData(hWin,Para->N2OLowValue); //cpp add 20130626
        ButtonRotation_SetTriangleSign(hWin,0);
//      if(AA_Module_Flag.EtCO2ModuleExist == 0 || EtCO2ModuleOff == 0 ||  !(AA_all_data.AA_state.dev_info.module_type & 0x04) )
		if(!EtCO2ModuleOff)
		{
			WM_DisableWindow(hWin);
		}
		if(!((Masimo_EtCO2_Config_On || MasimoISA_EtCO2_Config_On || Drager_EtCO2_Config_On) && AA_Module_Flag.EtCO2ModuleExist == 2))
		{
			WM_DisableWindow(hWin);
		}
		if((AA_Module_Flag.EtCO2ModuleExist == 2)&&(AA_all_data.AA_state.dev_info.module_type == AA_PHASEIN_CO2ONLY))
		{
			WM_DisableWindow(hWin);
		}
                if((Masimo_EtCO2_Config_On || MasimoISA_EtCO2_Config_On || Drager_EtCO2_Config_On) && AA_Module_Flag.PhaseInTypeModuleExist == 6)  //gyh  add  20230727 MASIMO旁流CO2屏蔽AA报警设置
		{
			WM_DisableWindow(hWin);
		}

#if (MACHINE_TYPE == X6 || MACHINE_TYPE == Z6) 
		WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON4));
		WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_BUTTON5));
#endif

//--------------------------------------------------------------------
        hWin = WM_GetDialogItem(hDlg,GUI_ID_TEXT6);
		switch(AA_all_data.AA_detail.AA_AA1.ID)
        {
        case IM_NO_AGT:
            j = 6;
            break;
        case IM_AGT_HAL:
            j = 7;
            break;
        case IM_AGT_ENF:
            j = 8;
            break;
        case IM_AGT_ISO:
            j = 9;
            break;
        case IM_AGT_SEV:
            j = 10;
            break;
        case IM_AGT_DES:
            j = 11;
            break;
        }

#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C) //add ylk 20170908
		TEXT_SetText(hWin,Sys_Lib_GetDispStr(AAName[j]));
#endif
       
		hWin = WM_GetDialogItem(hDlg,GUI_ID_BUTTON6);
		switch(AA_all_data.AA_detail.AA_AA1.ID)
        {
        case IM_NO_AGT: //NO AA
        case IM_AGT_HAL: //HAL
        case IM_AGT_ENF: //ENF
        case IM_AGT_ISO: //ISO
            ButtonRotation_SetDataInfo(hWin,1,80,Para->EtAAHighValue,1,1);
            break;
        case IM_AGT_SEV: //SEV
            ButtonRotation_SetDataInfo(hWin,1,100,Para->EtAAHighValue,1,1);
            break;
        case IM_AGT_DES: //DES
            ButtonRotation_SetDataInfo(hWin,1,200,Para->EtAAHighValue,1,1);
            break;
        default:
            break;
        }
        ButtonRotation_SetData(hWin,Para->EtAAHighValue); //cpp add 20130626
        ButtonRotation_SetTriangleSign(hWin,0);
        ButtonRotation_SetDataInvildSign(hWin,2);//上限关闭
//      if(EtCO2ModuleOff == 0 || !(pAll_Data->slow.VerVal.sen_type & 0x04) )
// 		if(EtCO2ModuleOff == 0 || !(AA_all_data.AA_state.dev_info.module_type == AA_PHASEIN_OR) )
//         {
//             WM_DisableWindow(hWin);
//         }
//         else
//         {
//             WM_EnableWindow(hWin);
//         }

		if(!EtCO2ModuleOff)
		{
			WM_DisableWindow(hWin);
		}
		if(!((Masimo_EtCO2_Config_On || MasimoISA_EtCO2_Config_On || Drager_EtCO2_Config_On) && AA_Module_Flag.EtCO2ModuleExist == 2))
		{
			WM_DisableWindow(hWin);
		}
		if((AA_Module_Flag.EtCO2ModuleExist == 2)&&(AA_all_data.AA_state.dev_info.module_type == AA_PHASEIN_CO2ONLY))
		{
			WM_DisableWindow(hWin);
		}
                if((Masimo_EtCO2_Config_On || MasimoISA_EtCO2_Config_On || Drager_EtCO2_Config_On) && AA_Module_Flag.PhaseInTypeModuleExist == 6)   //gyh  add  20230727 MASIMO旁流CO2屏蔽AA报警设置
		{
			WM_DisableWindow(hWin);
		}
#if (MACHINE_TYPE == X6 || MACHINE_TYPE == Z6) 
        hButton6 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON6);
        WM_HideWindow(hButton6);
#endif

        //------------------------------------------------------------------------------------------
        hWin = WM_GetDialogItem(hDlg,GUI_ID_BUTTON7);
		switch(AA_all_data.AA_detail.AA_AA1.ID)
        {
        case IM_NO_AGT: //NO AA
        case IM_AGT_HAL: //HAL
        case IM_AGT_ENF: //ENF
        case IM_AGT_ISO: //ISO
            ButtonRotation_SetDataInfo(hWin,0,78,Para->EtAALowValue,1,1);
            break;
        case IM_AGT_SEV: //SEV
            ButtonRotation_SetDataInfo(hWin,0,98,Para->EtAALowValue,1,1);
            break;
        case IM_AGT_DES: //DES
            ButtonRotation_SetDataInfo(hWin,0,198,Para->EtAALowValue,1,1);
            break;
        default:
            break;
        }
        ButtonRotation_SetData(hWin,Para->EtAALowValue); //cpp add 20130626
        ButtonRotation_SetTriangleSign(hWin,0);
//      if(EtCO2ModuleOff == 0 || !(pAll_Data->slow.VerVal.sen_type & 0x04) )
// 		if(EtCO2ModuleOff == 0 || !(AA_all_data.AA_state.dev_info.module_type == AA_PHASEIN_OR) )
//         {
//             WM_DisableWindow(hWin);
//         }
//         else
//         {
//             WM_EnableWindow(hWin);
//         }
		if(!EtCO2ModuleOff)
		{
			WM_DisableWindow(hWin);
		}
		if(!((Masimo_EtCO2_Config_On || MasimoISA_EtCO2_Config_On || Drager_EtCO2_Config_On) && AA_Module_Flag.EtCO2ModuleExist == 2))
		{
			WM_DisableWindow(hWin);
		}
		if((AA_Module_Flag.EtCO2ModuleExist == 2)&&(AA_all_data.AA_state.dev_info.module_type == AA_PHASEIN_CO2ONLY))
		{
			WM_DisableWindow(hWin);
		}
                if((Masimo_EtCO2_Config_On || MasimoISA_EtCO2_Config_On || Drager_EtCO2_Config_On) && AA_Module_Flag.PhaseInTypeModuleExist == 6)     //gyh  add  20230727 MASIMO旁流CO2屏蔽AA报警设置
		{
			WM_DisableWindow(hWin);
		}
#if (MACHINE_TYPE == X6 || MACHINE_TYPE == Z6) 
        hButton7 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON7);
        WM_HideWindow(hButton7);
#endif
        //------------------------------------------------------------------------------------------
        hWin = WM_GetDialogItem(hDlg,GUI_ID_TEXT8);
		switch(AA_all_data.AA_detail.AA_AA1.ID)
        {
        case IM_NO_AGT:
            j = 0;
            break;
        case IM_AGT_HAL:
            j = 1;
            break;
        case IM_AGT_ENF:
            j = 2;
            break;
        case IM_AGT_ISO:
            j = 3;
            break;
        case IM_AGT_SEV:
            j = 4;
            break;
        case IM_AGT_DES:
            j = 5;
            break;
        }

#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C) //add ylk 20170908
		TEXT_SetText(hWin,Sys_Lib_GetDispStr(AAName[j]));
#endif

        hWin = WM_GetDialogItem(hDlg,GUI_ID_BUTTON8);
		switch(AA_all_data.AA_detail.AA_AA1.ID)        
		{
        case IM_NO_AGT: //NO AA
        case IM_AGT_HAL: //HAL
        case IM_AGT_ENF: //ENF
        case IM_AGT_ISO: //ISO
            ButtonRotation_SetDataInfo(hWin,1,80,Para->FiAAHighValue,1,1);
            break;
        case IM_AGT_SEV: //SEV
            ButtonRotation_SetDataInfo(hWin,1,100,Para->FiAAHighValue,1,1);
            break;
        case IM_AGT_DES: //DES
            ButtonRotation_SetDataInfo(hWin,1,200,Para->FiAAHighValue,1,1);
            break;
        default:
            break;
        }
        ButtonRotation_SetData(hWin,Para->FiAAHighValue); //cpp add 20130626
        ButtonRotation_SetTriangleSign(hWin,0);
        ButtonRotation_SetDataInvildSign(hWin,2);//上限关闭
//      if(EtCO2ModuleOff == 0 || !(pAll_Data->slow.VerVal.sen_type & 0x04) )
// 		if(EtCO2ModuleOff == 0 || !(AA_all_data.AA_state.dev_info.module_type == AA_PHASEIN_OR) )
//         {
//             WM_DisableWindow(hWin);
//         }
//         else
//         {
//             WM_EnableWindow(hWin);
//         }
		if(!EtCO2ModuleOff)
		{
			WM_DisableWindow(hWin);
		}
		if(!((Masimo_EtCO2_Config_On || MasimoISA_EtCO2_Config_On || Drager_EtCO2_Config_On) && AA_Module_Flag.EtCO2ModuleExist == 2))
		{
			WM_DisableWindow(hWin);
		}
		if((AA_Module_Flag.EtCO2ModuleExist == 2)&&(AA_all_data.AA_state.dev_info.module_type == AA_PHASEIN_CO2ONLY))
		{
			WM_DisableWindow(hWin);
		}
                if((Masimo_EtCO2_Config_On || MasimoISA_EtCO2_Config_On || Drager_EtCO2_Config_On) && AA_Module_Flag.PhaseInTypeModuleExist == 6)    //gyh  add  20230727 MASIMO旁流CO2屏蔽AA报警设置
		{
			WM_DisableWindow(hWin);
		}
#if (MACHINE_TYPE == X6 || MACHINE_TYPE == Z6) 
        hButton8 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON8);
        WM_HideWindow(hButton8);
#endif
        //------------------------------------------------------------------------------------------
        hWin = WM_GetDialogItem(hDlg,GUI_ID_BUTTON9);
		switch(AA_all_data.AA_detail.AA_AA1.ID)
		{
        case IM_NO_AGT:  //NO AA
        case IM_AGT_HAL: //HAL
        case IM_AGT_ENF: //ENF
        case IM_AGT_ISO: //ISO
            ButtonRotation_SetDataInfo(hWin,0,78,Para->FiAALowValue,1,1);
            break;
        case IM_AGT_SEV: //SEV
            ButtonRotation_SetDataInfo(hWin,0,98,Para->FiAALowValue,1,1);
            break;
        case IM_AGT_DES: //DES
            ButtonRotation_SetDataInfo(hWin,0,198,Para->FiAALowValue,1,1);
            break;
        default:
            break;
        }
        ButtonRotation_SetData(hWin,Para->FiAALowValue); //cpp add 20130626
        ButtonRotation_SetTriangleSign(hWin,0);
//      if(EtCO2ModuleOff == 0 || !(pAll_Data->slow.VerVal.sen_type & 0x04) )
// 		if(EtCO2ModuleOff == 0 || !(AA_all_data.AA_state.dev_info.module_type == AA_PHASEIN_OR) )
//         {
//             WM_DisableWindow(hWin);
//         }
//         else
//         {
//             WM_EnableWindow(hWin);
//         }
		if(!EtCO2ModuleOff)
		{
			WM_DisableWindow(hWin);
		}
		if(!((Masimo_EtCO2_Config_On || MasimoISA_EtCO2_Config_On || Drager_EtCO2_Config_On) && AA_Module_Flag.EtCO2ModuleExist == 2))
		{
			WM_DisableWindow(hWin);
		}
		if((AA_Module_Flag.EtCO2ModuleExist == 2)&&(AA_all_data.AA_state.dev_info.module_type == AA_PHASEIN_CO2ONLY))
		{
			WM_DisableWindow(hWin);
		}
                if((Masimo_EtCO2_Config_On || MasimoISA_EtCO2_Config_On || Drager_EtCO2_Config_On) && AA_Module_Flag.PhaseInTypeModuleExist == 6)    //gyh  add  20230727 MASIMO旁流CO2屏蔽AA报警设置
		{
			WM_DisableWindow(hWin);
		}

#if (MACHINE_TYPE == X6 || MACHINE_TYPE == Z6) 
        hButton9 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON9);
        WM_HideWindow(hButton9);
#endif
//
    }
	else
	{
	    hWin = WM_GetDialogItem(hDlg, GUI_ID_BUTTON0);
        ButtonRotation_SetDataInfo(hWin,31,250,Para->PulseHighValue,1,0);
        ButtonRotation_SetData(hWin,Para->PulseHighValue);
        ButtonRotation_SetTriangleSign(hWin,0);
        if(!SpO2ModuleExist)
        {
            WM_DisableWindow(hWin);
        }
        else
        {
            if(SpO2ModuleOff) //  20130409
            { 
                WM_DisableWindow(hWin);
            }
        }

//
        hWin = WM_GetDialogItem(hDlg, GUI_ID_BUTTON1);
        ButtonRotation_SetDataInfo(hWin,30,249,Para->PulseLowValue,1,0);
        ButtonRotation_SetData(hWin,Para->PulseLowValue);
        ButtonRotation_SetTriangleSign(hWin,0);
        if(!SpO2ModuleExist)
        {
            WM_DisableWindow(hWin);
        }
        else
        {
            if(SpO2ModuleOff) //  20130409
            {
                WM_DisableWindow(hWin);
            }
        }
//
        hWin = WM_GetDialogItem(hDlg, GUI_ID_BUTTON2);
        ButtonRotation_SetDataInfo(hWin,50,100,Para->SpO2HighValue,1,0);
        ButtonRotation_SetDataInvildSign(hWin,2);//设置成上限关闭
        ButtonRotation_SetData(hWin,Para->SpO2HighValue);
        ButtonRotation_SetTriangleSign(hWin,0);
        if(!SpO2ModuleExist)
        {
            WM_DisableWindow(hWin);
        }
        else
        {
            if(SpO2ModuleOff) //  20130409
            {
                WM_DisableWindow(hWin);
            }
        }
//
        hWin = WM_GetDialogItem(hDlg, GUI_ID_BUTTON3);
        ButtonRotation_SetDataInfo(hWin,49,99,Para->SpO2LowValue,1,0);
        ButtonRotation_SetData(hWin,Para->SpO2LowValue);
        ButtonRotation_SetTriangleSign(hWin,0);
        if(!SpO2ModuleExist)
        {
            WM_DisableWindow(hWin);
        }
        else
        {
            if(SpO2ModuleOff) //  20130409
            {
                WM_DisableWindow(hWin);
            }
        }
    }
}
/***************************************************************************************************
* @brief  报警参数限制关系
* @para   
* @retval 
*/
static int AlarmParameterLimitAnalysis(ButtonRotation_Handle hObj,int page,int direction,ALARM_SET_PARA *Para)
{
    int r = 0;
    ButtonRotation_Obj* pObj;
    pObj = ButtonRotation_H2P(hObj);
    if(direction == GUI_KEY_LEFT)
    {
        switch(page)
        {
        case 0:
            switch(pObj->Widget.Id)
            {
            case GUI_ID_BUTTON0: //PressureHigh
                if(  (BeathSettingPara.BreathMode == PCV_MODE)
					|| ( ((BeathSettingPara.BreathMode == MANUAL_MODE)||(BeathSettingPara.BreathMode == ACGO_MODE))
					&& (BreathMode_BeforeACGO == PCV_MODE) )
					)
                {
                    if(  ((pObj->Data1 - pObj->BDInfo.Step) >= (pObj->BDInfo.Min))
                      && ((pObj->Data1 - pObj->BDInfo.Step) > (Para->PressureLowValue))
                      && ((pObj->Data1 - pObj->BDInfo.Step) > (BeathSettingPara.SettingPinspValue + BeathSettingPara.SettingPEEPValue)) //  20130522
                      )
                    {
                        pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
                        r=1;
                    }
                }
                else if((BeathSettingPara.BreathMode == SIMV_PCV_MODE)
					|| ( ((BeathSettingPara.BreathMode == MANUAL_MODE)||(BeathSettingPara.BreathMode == ACGO_MODE))
					&& (BreathMode_BeforeACGO == SIMV_PCV_MODE) )
					)
                {
                    if(  ((pObj->Data1 - pObj->BDInfo.Step) >= (pObj->BDInfo.Min))
                      && ((pObj->Data1 - pObj->BDInfo.Step) > (Para->PressureLowValue))
                      && ((pObj->Data1 - pObj->BDInfo.Step) > (BeathSettingPara.SettingPinspValue + BeathSettingPara.SettingPEEPValue)) //  20130522
                      && ((pObj->Data1 - pObj->BDInfo.Step) > (BeathSettingPara.SettingPsupportValue + BeathSettingPara.SettingPEEPValue))
                      )
                    {
                        pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
                        r=1;
                    }
                }
                else
                if(  (BeathSettingPara.BreathMode == SIMV_VCV_MODE) //  20130523
                  || (BeathSettingPara.BreathMode == SIMV_PRVC_MODE)
                  || (BeathSettingPara.BreathMode == PSV_MODE)
				  || ( ((BeathSettingPara.BreathMode == MANUAL_MODE)||(BeathSettingPara.BreathMode == ACGO_MODE))
				  && (BreathMode_BeforeACGO == SIMV_VCV_MODE) || (BreathMode_BeforeACGO == SIMV_PRVC_MODE) 
				  || (BreathMode_BeforeACGO == PSV_MODE))
                  )
                {
                    if(  ((pObj->Data1 - pObj->BDInfo.Step) >= (pObj->BDInfo.Min))
                      && ((pObj->Data1 - pObj->BDInfo.Step) > (Para->PressureLowValue))
                      && ((pObj->Data1 - pObj->BDInfo.Step) > (BeathSettingPara.SettingPsupportValue + BeathSettingPara.SettingPEEPValue))
                      )
                    {
                        pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
                        r=1;
                    }
                }                
                else
                {
                    if(  ((pObj->Data1 - pObj->BDInfo.Step) >= (pObj->BDInfo.Min))
                      && ((pObj->Data1 - pObj->BDInfo.Step) > (Para->PressureLowValue))
                      && ((pObj->Data1 - pObj->BDInfo.Step) > (BeathSettingPara.SettingPEEPValue)) //  20130522
                      )
                    {
                        pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
                        r=1;
                    }
                }
                break;
            case GUI_ID_BUTTON1:
                if((pObj->Data1 - pObj->BDInfo.Step) >= (pObj->BDInfo.Min))
                {
                    pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
                    r=1;
                }
                break;
            case GUI_ID_BUTTON2:
                if(  ((pObj->Data1 - pObj->BDInfo.Step) >= (pObj->BDInfo.Min))
                  && ((pObj->Data1 - pObj->BDInfo.Step) > (Para->MVLowValue)))
                {
                    pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
                    r=1;
                }
                break;
            case GUI_ID_BUTTON3:
                if((pObj->Data1 - pObj->BDInfo.Step) >= (pObj->BDInfo.Min))
                {
                    pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
                    r=1;
                }
                break;
            case GUI_ID_BUTTON4:
                if(  ((pObj->Data1 - pObj->BDInfo.Step) >= (pObj->BDInfo.Min))
                  && ((pObj->Data1 - pObj->BDInfo.Step) > (Para->RateLowValue)))
                {
                    pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
                    r=1;
                }
                break;
            case GUI_ID_BUTTON5:
                if((pObj->Data1 - pObj->BDInfo.Step) >= (pObj->BDInfo.Min))
                {
                    pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
                    r=1;
                }
                break;
            case GUI_ID_BUTTON6:
                if(  ((pObj->Data1 - pObj->BDInfo.Step) >= (pObj->BDInfo.Min))
                  && ((pObj->Data1 - pObj->BDInfo.Step) > (Para->FiO2LowValue)))
                {
                    pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
                    r=1;
                }
                break;
            case GUI_ID_BUTTON7:
                if((pObj->Data1 - pObj->BDInfo.Step) >= (pObj->BDInfo.Min))
                {
                    pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
                    r=1;
                }
                break;
            case GUI_ID_BUTTON8:
                if(  ((pObj->Data1 - pObj->BDInfo.Step) >= (pObj->BDInfo.Min))
                  && ((pObj->Data1 - pObj->BDInfo.Step) > (Para->VteLowValue)))
                {
                    pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
                    r=1;
                }
                break;
            case GUI_ID_BUTTON9:
                if((pObj->Data1 - pObj->BDInfo.Step) >= (pObj->BDInfo.Min))
                {
                    pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
                    r=1;
                }
                break;
            case GUI_ID_BUTTON10:
                if((pObj->Data1 - pObj->BDInfo.Step) >= (pObj->BDInfo.Min))
                {
                    pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
                    r=1;
                }
                break;
            case GUI_ID_BUTTON11:
                if((pObj->Data1 - pObj->BDInfo.Step) >= (pObj->BDInfo.Min))
                {
                    pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
                    r=1;
                }
                break;
            default:
                break;
            }
            break; //endof case 0:
        
        case 1:
            switch(pObj->Widget.Id)
            {
            case GUI_ID_BUTTON0:
                if(  ((pObj->Data1 - pObj->BDInfo.Step) >= (pObj->BDInfo.Min))
                  && ((pObj->Data1 - pObj->BDInfo.Step) > (Para->EtCO2LowValue))
                  )
                {
                    pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
                    r=1;
                }
                break;
            case GUI_ID_BUTTON1:
                if((pObj->Data1 - pObj->BDInfo.Step) >= (pObj->BDInfo.Min))
                {
                    pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
                    r=1;
                }
                break;
            case GUI_ID_BUTTON2:
                if(  ((pObj->Data1 - pObj->BDInfo.Step) >= (pObj->BDInfo.Min))
                  && ((pObj->Data1 - pObj->BDInfo.Step) > (Para->FiCO2LowValue))
                  )
                {
                    pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
                    r=1;
                }
                break;
            case GUI_ID_BUTTON3:
                if((pObj->Data1 - pObj->BDInfo.Step) >= (pObj->BDInfo.Min))
                {
                    pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
                    r=1;
                }
                break;                
            case GUI_ID_BUTTON4:
                if(((pObj->Data1 - pObj->BDInfo.Step) >= (pObj->BDInfo.Min))
                  &((pObj->Data1 - pObj->BDInfo.Step) > (Para->N2OLowValue)))
                {
                    pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
                    r=1;
                }
                break;
            case GUI_ID_BUTTON5:
                if((pObj->Data1 - pObj->BDInfo.Step) >= (pObj->BDInfo.Min))
                {
                    pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
                    r=1;
                }
                break;
            case GUI_ID_BUTTON6:
                if(((pObj->Data1 - pObj->BDInfo.Step) >= (pObj->BDInfo.Min))
                  &((pObj->Data1 - pObj->BDInfo.Step) > (Para->EtAALowValue)))
                {
                    pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
                    r=1;
                }
                break;
            case GUI_ID_BUTTON7:
                if((pObj->Data1 - pObj->BDInfo.Step) >= (pObj->BDInfo.Min))
                {
                    pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
                    r=1;
                }
                break;
            case GUI_ID_BUTTON8:
                if(((pObj->Data1 - pObj->BDInfo.Step) >= (pObj->BDInfo.Min))
                  &((pObj->Data1 - pObj->BDInfo.Step) > (Para->FiAALowValue)))
                {
                    pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
                    r=1;
                }
                break;
            case GUI_ID_BUTTON9:
                if((pObj->Data1 - pObj->BDInfo.Step) >= (pObj->BDInfo.Min))
                {
                    pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
                    r=1;
                }
                break;
			default:
                break;
            }
            break; //endof case 1:
		case 2:
			switch(pObj->Widget.Id)
            {
            case GUI_ID_BUTTON0:
                if(  ((pObj->Data1 - pObj->BDInfo.Step) >= (pObj->BDInfo.Min))
                  && ((pObj->Data1 - pObj->BDInfo.Step) > (Para->PulseLowValue)))
                {
                    pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
                    r=1;
                }
                break;
            case GUI_ID_BUTTON1:
                if((pObj->Data1 - pObj->BDInfo.Step) >= (pObj->BDInfo.Min))
                {
                    pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
                    r=1;
                }
                break;
            case GUI_ID_BUTTON2:
                if(  ((pObj->Data1 - pObj->BDInfo.Step) >= (pObj->BDInfo.Min))
                  && ((pObj->Data1 - pObj->BDInfo.Step) > (Para->SpO2LowValue)))
                {
                    pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
                    r=1;
                }
                break;
            case GUI_ID_BUTTON3:
                if((pObj->Data1 - pObj->BDInfo.Step) >= (pObj->BDInfo.Min))
                {
                    pObj->Data1 = (pObj->Data1) - (pObj->BDInfo.Step);
                    r=1;
                }
			default:
                break;
            }
                break;
			break;
        default:
            break;
        }
    } //endof if(direction == GUI_KEY_LEFT)
    
    else if(direction == GUI_KEY_RIGHT)
    {
        switch(page)
        {
        case 0:
            switch(pObj->Widget.Id)
            {
            case GUI_ID_BUTTON0://PressureHigh
                if( (BeathSettingPara.BreathMode == PCV_MODE)
                  ||(BeathSettingPara.BreathMode == SIMV_PCV_MODE)
				  || ( ((BeathSettingPara.BreathMode == MANUAL_MODE)||(BeathSettingPara.BreathMode == ACGO_MODE))
				  && (BreathMode_BeforeACGO == PCV_MODE) || (BreathMode_BeforeACGO == SIMV_PCV_MODE) )
                  )
                {
                    if(  ((pObj->Data1 + pObj->BDInfo.Step) <= pObj->BDInfo.Max)
                      && ((pObj->Data1 + pObj->BDInfo.Step) > (BeathSettingPara.SettingPinspValue + BeathSettingPara.SettingPEEPValue))//  20130522
                      )
                    {
                        pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
                        r = 1;
                    }
            
                }
                else
                if( (BeathSettingPara.BreathMode == SIMV_VCV_MODE) //  20130523
                  ||(BeathSettingPara.BreathMode == SIMV_PRVC_MODE)
                  ||(BeathSettingPara.BreathMode == PSV_MODE)
				  || ( ((BeathSettingPara.BreathMode == MANUAL_MODE)||(BeathSettingPara.BreathMode == ACGO_MODE))
				  && (BreathMode_BeforeACGO == SIMV_VCV_MODE) || (BreathMode_BeforeACGO == SIMV_PRVC_MODE) 
				  || (BreathMode_BeforeACGO == PSV_MODE) )
                  )
                {
                    if(  ((pObj->Data1 + pObj->BDInfo.Step) <= pObj->BDInfo.Max)
                      && ((pObj->Data1 + pObj->BDInfo.Step) > (BeathSettingPara.SettingPsupportValue + BeathSettingPara.SettingPEEPValue))
                      )
                    {
                        pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
                        r = 1;
                    }
            
                }                
                else
                {
                    if(  ((pObj->Data1 + pObj->BDInfo.Step) <= pObj->BDInfo.Max)
                      && ((pObj->Data1 + pObj->BDInfo.Step) > (BeathSettingPara.SettingPEEPValue)) //  20130522
                      )
                    {
                        pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
                        r = 1;
                    }
                }
                break;
            case GUI_ID_BUTTON1:
                if(  ((pObj->Data1 + pObj->BDInfo.Step) <= pObj->BDInfo.Max)
                  && ((pObj->Data1 + pObj->BDInfo.Step) < (Para->PressureHighValue)))
                {
                    pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
                    r = 1;
                }
                break;
            case GUI_ID_BUTTON2:
                if((pObj->Data1 + pObj->BDInfo.Step) <= pObj->BDInfo.Max)
                {
                    pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
                    r = 1;
                }
                break;
            case GUI_ID_BUTTON3:
                if(  ((pObj->Data1 + pObj->BDInfo.Step) <= pObj->BDInfo.Max)
                  && ((pObj->Data1 + pObj->BDInfo.Step) < (Para->MVHighValue)))
                {
                    pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
                    r = 1;
                }
                break;
            case GUI_ID_BUTTON4:
                if((pObj->Data1 + pObj->BDInfo.Step) <= pObj->BDInfo.Max)
                {
                    pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
                    r = 1;
                }
                break;
            case GUI_ID_BUTTON5:
                if(  ((pObj->Data1 + pObj->BDInfo.Step) <= pObj->BDInfo.Max)
                  && ((pObj->Data1 + pObj->BDInfo.Step) < (Para->RateHighValue)))
                {
                    pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
                    r = 1;
                }
                break;
            case GUI_ID_BUTTON6:
                if((pObj->Data1 + pObj->BDInfo.Step) <= pObj->BDInfo.Max)
                {
                    pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
                    r = 1;
                }
                break;
            case GUI_ID_BUTTON7:
                if( ((pObj->Data1 + pObj->BDInfo.Step) <= pObj->BDInfo.Max)
                  && ((pObj->Data1 + pObj->BDInfo.Step) < (Para->FiO2HighValue)))
                {
                    pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
                    r = 1;
                }
                break;
            case GUI_ID_BUTTON8:
                if((pObj->Data1 + pObj->BDInfo.Step) <= pObj->BDInfo.Max)
                {
                    pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
                    r = 1;
                }
                break;
            case GUI_ID_BUTTON9:
                if(  ((pObj->Data1 + pObj->BDInfo.Step) <= pObj->BDInfo.Max)
                  && ((pObj->Data1 + pObj->BDInfo.Step) < (Para->VteHighValue)))
                {
                    pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
                    r = 1;
                }
                break;
            case GUI_ID_BUTTON10:
                if((pObj->Data1 + pObj->BDInfo.Step) <= pObj->BDInfo.Max)
                {
                    pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
                    r = 1;
                }
                break;
            case GUI_ID_BUTTON11:
                if((pObj->Data1 + pObj->BDInfo.Step) <= pObj->BDInfo.Max)
                {
                    pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
                    r = 1;
                }
                break;
            default:
                break;
            }
            break; //endof case 0:
            
        case 1:
            switch(pObj->Widget.Id)
            {
            case GUI_ID_BUTTON0:
                if((pObj->Data1 + pObj->BDInfo.Step) <= pObj->BDInfo.Max)
                {
                    pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
                    r = 1;
                }
                break;
            case GUI_ID_BUTTON1:
                if(  ((pObj->Data1 + pObj->BDInfo.Step) <= pObj->BDInfo.Max)
                  && ((pObj->Data1 + pObj->BDInfo.Step) < (Para->EtCO2HighValue))
                  )
                {
                    pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
                    r = 1;
                }
                break;
            case GUI_ID_BUTTON2:
                if((pObj->Data1 + pObj->BDInfo.Step) <= pObj->BDInfo.Max)
                {
                    pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
                    r = 1;
                }
                break;
            case GUI_ID_BUTTON3:
                if(  ((pObj->Data1 + pObj->BDInfo.Step) <= pObj->BDInfo.Max)
                  && ((pObj->Data1 + pObj->BDInfo.Step) < (Para->FiCO2HighValue))
                  )
                {
                    pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
                    r = 1;
                }
                break;

            case GUI_ID_BUTTON4:
                if((pObj->Data1 + pObj->BDInfo.Step)<=pObj->BDInfo.Max)
                {
                    pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
                    r = 1;
                }
                break;
            case GUI_ID_BUTTON5:
                if(((pObj->Data1 + pObj->BDInfo.Step)<=pObj->BDInfo.Max)
                  &((pObj->Data1 + pObj->BDInfo.Step)<(Para->N2OHighValue)))
                {
                    pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
                    r = 1;
                }
                break;
            case GUI_ID_BUTTON6:
                if((pObj->Data1 + pObj->BDInfo.Step)<=pObj->BDInfo.Max)
                {
                    pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
                    r = 1;
                }
                break;
            case GUI_ID_BUTTON7:
                if(((pObj->Data1 + pObj->BDInfo.Step)<=pObj->BDInfo.Max)
                  &((pObj->Data1 + pObj->BDInfo.Step)<(Para->EtAAHighValue)))
                {
                    pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
                    r = 1;
                }
                break;
            case GUI_ID_BUTTON8:
                if((pObj->Data1 + pObj->BDInfo.Step)<=pObj->BDInfo.Max)
                {
                    pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
                    r = 1;
                }
                break;
            case GUI_ID_BUTTON9:
                if(((pObj->Data1 + pObj->BDInfo.Step)<=pObj->BDInfo.Max)
                  &((pObj->Data1 + pObj->BDInfo.Step)<(Para->FiAAHighValue)))
                {
                    pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
                    r = 1;
                }
                break;
            default:
                break;
            }
            break; //endof case 1:
        case 2:
            switch(pObj->Widget.Id)
            {
            case GUI_ID_BUTTON0:
                if((pObj->Data1 + pObj->BDInfo.Step)<=pObj->BDInfo.Max)
                {
                    pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
                    r = 1;
                }
                break;
            case GUI_ID_BUTTON1:
                if(((pObj->Data1 + pObj->BDInfo.Step)<=pObj->BDInfo.Max)
                  &((pObj->Data1 + pObj->BDInfo.Step)<(Para->PulseHighValue)))
                {
                    pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
                    r = 1;
                }
                break;
            case GUI_ID_BUTTON2:
                if((pObj->Data1 + pObj->BDInfo.Step)<=pObj->BDInfo.Max)
                {
                    pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
                    r = 1;
                }
                break;
            case GUI_ID_BUTTON3:
                if(((pObj->Data1 + pObj->BDInfo.Step)<=pObj->BDInfo.Max)
                  &((pObj->Data1 + pObj->BDInfo.Step)<(Para->SpO2HighValue)))
                {
                    pObj->Data1 = (pObj->Data1) + (pObj->BDInfo.Step);
                    r = 1;
                }
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    } //endof else if(direction == GUI_KEY_RIGHT)
    return r;
}
/***************************************************************************************************
* @brief  报警设置值更新
* @para   
* @retval 
*/
static void AlarmSettingValueUpdate(int data,int page,ALARM_SET_PARA *Para,HOME_MENU_HWIN *hWin,int WorkState,int Id)
{
    WM_MESSAGE Msg;
	char pText[10] = {0};
    int PawFlag = 0;
	int index = 0;

    switch(page)
    {
    case 0:
        switch(Id)
        {
        case  GUI_ID_BUTTON0:
			Para->PressureHighValue = data;
			if((index = Get_MonitorRank_Rapid(DP_PPEAK)) != -1)//判断当前参数是否可以显示
			{
				if(WorkState==0)//不是待机模式 
				{
#ifdef WAVE_PAW_ADD_ALARMHIGH
					PawFlag = GetPawAlarmRange();
					if(Menu_hWin.cWin.WaveForms[0] == WAVEFORM_PRESSURE)
					{
						TracingCurve_SetRange(Menu_hWin.HomeWin.Tobj[0],PawFlag);
						TracingCurve_SetXScale(Menu_hWin.HomeWin.Tobj[0],1);
					}
					if(Menu_hWin.cWin.WaveForms[1] == WAVEFORM_PRESSURE)
					{
						TracingCurve_SetRange(Menu_hWin.HomeWin.Tobj[1],PawFlag);
						TracingCurve_SetXScale(Menu_hWin.HomeWin.Tobj[1],1);
					}
#endif
					sprintf(pText, "%d", data);	
					MultiText_MaxValueUpdate(hWin->Robj[index], pText);
				}
				SendOperationMessage(SEND_PH_LIMIT,USART0);
				// test
				if(BeathSettingPara.BreathMode == VS_MODE)
				{
					Breath_Setting_Tmp.SettingPHLimitValue = data;
					BeathSettingPara.SettingPHLimitValue = Breath_Setting_Tmp.SettingPHLimitValue;
					WM_SendMessageNoPara(Menu_hWin.bWin, WM_INIT_DIALOG);
				}
			}
			break;
		case  GUI_ID_BUTTON1:
			Para->PressureLowValue = data;
			if(BeathSettingPara.BreathMode != MANUAL_MODE)
			{
				Alarm_SaveVent_para.PressureLowValue = Para->PressureLowValue;
			}
			if((index = Get_MonitorRank_Rapid(DP_PPEAK)) != -1)
			{
				if(WorkState == 0)
				{
					sprintf(pText, "%d", data);	
					MultiText_MinValueUpdate(hWin->Robj[index], pText);
				}
			}
            break;
        case  GUI_ID_BUTTON2:
			Para->MVHighValue = data;
			if(BeathSettingPara.BreathMode != MANUAL_MODE)
			{
				Alarm_SaveVent_para.MVHighValue = Para->MVHighValue;
			}
			if((index = Get_MonitorRank_Rapid(DP_MV)) != -1)
			{
				if(WorkState == 0)
				{
					sprintf(pText, "%d", data);	
					MultiText_MaxValueUpdate(hWin->Robj[index], pText);
				}
			}			
            break;
        case  GUI_ID_BUTTON3:
			Para->MVLowValue = data;
			if(BeathSettingPara.BreathMode != MANUAL_MODE)
			{
				Alarm_SaveVent_para.MVLowValue = Para->MVLowValue;
			}
			if((index = Get_MonitorRank_Rapid(DP_MV)) != -1)
			{
				if(WorkState == 0)
				{
					sprintf(pText, "%d", data);	
					MultiText_MinValueUpdate(hWin->Robj[index], pText);
				}
			}		
            break;
        case  GUI_ID_BUTTON4:
            Para->RateHighValue = data;
			if(BeathSettingPara.BreathMode != MANUAL_MODE)
			{
				Alarm_SaveVent_para.RateHighValue = Para->RateHighValue;
			}
			if((index = Get_MonitorRank_Rapid(DP_F)) != -1)
			{
				if(WorkState == 0)
				{
					sprintf(pText, "%d", data);	
					MultiText_MaxValueUpdate(hWin->Robj[index], pText);
				}
			}
            break;
        case  GUI_ID_BUTTON5:
            Para->RateLowValue = data;
			if(BeathSettingPara.BreathMode != MANUAL_MODE)
			{
				Alarm_SaveVent_para.RateLowValue = Para->RateLowValue;
			}
			if((index = Get_MonitorRank_Rapid(DP_F)) != -1)
			{
				if(WorkState == 0)
				{
					sprintf(pText, "%d", data);	
					MultiText_MinValueUpdate(hWin->Robj[index], pText);
				}
			}
            break;
        case  GUI_ID_BUTTON6:
			Para->FiO2HighValue = data;
			if((index = Get_MonitorRank_Rapid(DP_FIO2)) != -1)//判断当前参数是否可以显示
			{
				if(WorkState==0)
				{
					if(data == 101)
					{
						strcpy(pText, Sys_Lib_GetDispStr(DP_OFF));
					}
					else
					{
						sprintf(pText, "%d", data);						
					}
					MultiText_MaxValueUpdate(hWin->Robj[index], pText);
				}
			}
            break;
        case GUI_ID_BUTTON7:
			Para->FiO2LowValue = data;
			if((index = Get_MonitorRank_Rapid(DP_FIO2)) != -1)//判断当前参数是否可以显示
			{
				if(WorkState==0)
				{
					sprintf(pText, "%d", data);
					MultiText_MinValueUpdate(hWin->Robj[index], pText);
				}
			}
            break;
        case GUI_ID_BUTTON8:
			Para->VteHighValue = data;
			if(BeathSettingPara.BreathMode != MANUAL_MODE)
			{
				Alarm_SaveVent_para.VteHighValue = Para->VteHighValue;
			}
			if((index = Get_MonitorRank_Rapid(DP_VTE)) != -1)//判断当前参数是否可以显示
			{
				if(WorkState==0)
				{
					sprintf(pText, "%d", data);
					MultiText_MaxValueUpdate(hWin->Robj[index], pText);
				}
			}
            break;
        case GUI_ID_BUTTON9:
			Para->VteLowValue = data;
			if(BeathSettingPara.BreathMode != MANUAL_MODE)
			{
				Alarm_SaveVent_para.VteLowValue = Para->VteLowValue;
			}
			if((index = Get_MonitorRank_Rapid(DP_VTE)) != -1)//判断当前参数是否可以显示
			{
				if(WorkState==0)
				{
					if(data==10)
					{
						strcpy(pText, Sys_Lib_GetDispStr(DP_OFF));
 					}
 					else
 					{
						sprintf(pText, "%d", data);
					}
					MultiText_MinValueUpdate(hWin->Robj[index], pText);
				}
			}
            break;
        case GUI_ID_BUTTON10:
            Para->ApneaTimerValue = data;
            break;
        default:
            break;
        }
        break;
    case 1:
        switch(Id)
        {
        case  GUI_ID_BUTTON0:
			Para->EtCO2HighValue = data;
			if((ETCO2Unit == 0)||(ETCO2Unit == 2))
			{
				strcpy(pText, _Float2String((float)data/10,1));			
			}
			else
			{
				sprintf(pText, "%d", data);
			}
			if(WorkState==0)
			{
				//  [2/17/2022 yaoyao 注释]CO2只在左侧快捷监测区
				/*if((index = Get_MonitorRank_Rapid(DP_ETCO2)) != -1)
				{
					MultiText_MaxValueUpdate(hWin->Robj[index], pText);
				}*/
				if((index = Get_MonitorRank_AA(DP_CO2)) != -1)
				{
					MultiText_MaxValueUpdate1(hWin->Left_Robj[index], pText);
				}
			}
            break;
        case  GUI_ID_BUTTON1:
			Para->EtCO2LowValue = data;
			if((ETCO2Unit == 0)||(ETCO2Unit == 2))
			{
				strcpy(pText, _Float2String((float)data/10,1));	
			}
			else
			{
				sprintf(pText, "%d", data);
			}
			if(WorkState==0)
			{
				// [2/17/2022 yaoyao 注释]CO2只在左侧快捷监测区
// 				if((index = Get_MonitorRank_Rapid(DP_ETCO2)) != -1)
// 				{
// 					MultiText_MinValueUpdate(hWin->Robj[index], pText);
// 				}
				if((index = Get_MonitorRank_AA(DP_CO2)) != -1)
				{
					MultiText_MinValueUpdate1(hWin->Left_Robj[index], pText);
				}
			}
            break;
        case  GUI_ID_BUTTON2:
            Para->FiCO2HighValue = data;
			if((ETCO2Unit == 0)||(ETCO2Unit == 2))
			{
				strcpy(pText, _Float2String((float)data/10,1));	
			}
			else
			{
				sprintf(pText, "%d", data);
			}
			if(WorkState==0)
			{
				if((index = Get_MonitorRank_AA(DP_CO2)) != -1)
				{
					MultiText_MaxValueUpdate(hWin->Left_Robj[index], pText);
				}
			}
            break;
        case  GUI_ID_BUTTON3:
            Para->FiCO2LowValue = 0;//data; //cpp md 20130717
            break;            
        case  GUI_ID_BUTTON4:
            Para->N2OHighValue = data;
			if(WorkState==0)
			{
				if((index = Get_MonitorRank_AA(DP_N2O)) != -1)
				{
					strcpy(pText, _Float2String((float)data,0));
					MultiText_MaxValueUpdate(hWin->Left_Robj[index], pText);
					//
					MultiText_MaxValueUpdate1(hWin->Left_Robj[index], pText);
				}
			}
            break;
        case  GUI_ID_BUTTON5:
            Para->N2OLowValue = data;
			if(WorkState==0)
			{
				if((index = Get_MonitorRank_AA(DP_N2O)) != -1)
				{
					strcpy(pText, _Float2String((float)data,0));
					MultiText_MinValueUpdate(hWin->Left_Robj[index], pText);
					//
					MultiText_MinValueUpdate1(hWin->Left_Robj[index], pText);
				}
			}
            break;
        case  GUI_ID_BUTTON6:
            Para->EtAAHighValue = data;
			if(WorkState==0)
			{
				if((index = Get_MonitorRank_AA(DP_AA1)) != -1)
				{
					strcpy(pText, _Float2String((float)data/10,1));
					MultiText_MaxValueUpdate1(hWin->Left_Robj[index], pText);
				}
// 				if((index = Get_MonitorRank_AA(DP_AA2)) != -1)
// 				{
// 					strcpy(pText, _Float2String((float)data/10,1));
// 					MultiText_MaxValueUpdate1(hWin->Left_Robj[index], pText);
// 				}
			}
            break;
        case  GUI_ID_BUTTON7:
            Para->EtAALowValue = data;
			if(WorkState==0)
			{
				if((index = Get_MonitorRank_AA(DP_AA1)) != -1)
				{
					strcpy(pText, _Float2String((float)data/10,1));
					MultiText_MinValueUpdate1(hWin->Left_Robj[index], pText);
				}
// 				if((index = Get_MonitorRank_AA(DP_AA2)) != -1)
// 				{
// 					strcpy(pText, _Float2String((float)data/10,1));
// 					MultiText_MinValueUpdate1(hWin->Left_Robj[index], pText);
// 				}
			}
            break;
        case  GUI_ID_BUTTON8:
            Para->FiAAHighValue = data;
			if(WorkState==0)
			{	
				if((index = Get_MonitorRank_AA(DP_AA1)) != -1)
				{				
					strcpy(pText, _Float2String((float)data/10,1));
					MultiText_MaxValueUpdate(hWin->Left_Robj[index], pText);
				}
// 				if((index = Get_MonitorRank_AA(DP_AA2)) != -1)
// 				{				
// 					strcpy(pText, _Float2String((float)data/10,1));
// 					MultiText_MaxValueUpdate(hWin->Left_Robj[index], pText);
// 				}
			}
            break;
        case  GUI_ID_BUTTON9:
            Para->FiAALowValue = data;
			if(WorkState==0)
			{
				if((index = Get_MonitorRank_AA(DP_AA1)) != -1)
				{
					strcpy(pText, _Float2String((float)data/10,1));
					MultiText_MinValueUpdate(hWin->Left_Robj[index], pText);
				}
// 				if((index = Get_MonitorRank_AA(DP_AA2)) != -1)
// 				{
// 					strcpy(pText, _Float2String((float)data/10,1));
// 					MultiText_MinValueUpdate(hWin->Left_Robj[index], pText);
// 				}
			}
            break;
        default:
            break;
        }
        break;
	case 2:
		switch(Id)
		{
        case  GUI_ID_BUTTON0:
            Para->PulseHighValue = data;
            break;
        case  GUI_ID_BUTTON1:
            Para->PulseLowValue = data;
            break;
        case  GUI_ID_BUTTON2:
            Para->SpO2HighValue = data;
			if(WorkState==0)
			{
				//  [2/16/2022 yaoyao modify ]将血氧的参数移到右侧快捷监测区
// 				if((index = Get_MonitorRank_AA(DP_SPO2)) != -1)
// 				{
// 					strcpy(pText, _Float2String((float)data,0));
// 					MultiText_MaxValueUpdate(hWin->Left_Robj[index], pText);
// 				}
				if((index = Get_MonitorRank_Rapid(DP_SPO2)) != -1)
				{
					strcpy(pText, _Float2String((float)data,0));
					MultiText_MaxValueUpdate(hWin->Robj[index], pText);
				}
			}
            break;
        case  GUI_ID_BUTTON3:
            Para->SpO2LowValue = data;
			if(WorkState==0)
			{
				//  [2/16/2022 yaoyao modify ]将血氧的参数移到右侧快捷监测区
// 				if((index = Get_MonitorRank_AA(DP_SPO2)) != -1)
// 				{
// 					strcpy(pText, _Float2String((float)data,0));
// 					MultiText_MinValueUpdate(hWin->Left_Robj[index], pText);
// 				}
				if((index = Get_MonitorRank_Rapid(DP_SPO2)) != -1)
				{
					strcpy(pText, _Float2String((float)data,0));
					MultiText_MinValueUpdate(hWin->Robj[index], pText);
				}
			}
            break;
		}
		break;
    default:
        break;
    }
}
/***************************************************************************************************
* @brief  更换 MULTIPAGE 文字
* @para   
* @retval 
*/
static void ChangeMultiPageText(int Type,WM_HWIN ParentWin)
{
    MULTIPAGE_SetText(ParentWin,  Sys_Lib_GetDispStr(DP_INFORMATION), 0);
    MULTIPAGE_SetText(ParentWin,  Sys_Lib_GetDispStr(DP_SETTINGS), 1);
    MULTIPAGE_SetText(ParentWin,  Sys_Lib_GetDispStr(DP_DATETIME), 2);
    MULTIPAGE_SetText(ParentWin,  Sys_Lib_GetDispStr(DP_CALIBRATION), 3);
}
//update program
static const GUI_WIDGET_CREATE_INFO _aDialogCreateUpdate[] =
{
    { WINDOW_CreateIndirect,    "",       0,              325,  192, 270, 160, FRAMEWIN_CF_ACTIVE },//调整窗口位置
    { CHECKBOX_CreateIndirect,  "GUI",   GUI_ID_CHECK0,   30,  20,  80,  40, 0},
    { CHECKBOX_CreateIndirect,  "BDU",   GUI_ID_CHECK1,   150, 20,  80,  40, 0},
    { CHECKBOX_CreateIndirect,  "PMU",   GUI_ID_CHECK2,   30,  60,  80,  40, 0},
    { TEXT_CreateIndirect,      "",      GUI_ID_TEXT0,    20,  50,  220,  60, TEXT_CF_HCENTER|TEXT_CF_VCENTER },
    { BUTTON_CreateIndirect,    "",   GUI_ID_BUTTON0,     20,   115, 100, 30, 0},
    { BUTTON_CreateIndirect,    "",   GUI_ID_BUTTON1,     140,  115, 100, 30, 0},
};

static void _cbDialogUpdate(WM_MESSAGE * pMsg)
{
    WM_HWIN hButton0,hButton1;
    WM_HWIN hCheck0,hCheck1,hCheck2;
    WM_HWIN hDlg,hItem;
    int Id,NCode;
    hDlg = pMsg->hWin;
    switch (pMsg->MsgId)
    {
    case WM_INIT_DIALOG:
        WINDOW_SetUse_3D(hDlg,1);
        //MenuOperateStatus |= UPDATE_PROGRAM;

        hButton0 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON0);
        hButton1 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON1);
        hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT0);   //提示语句栏
        TEXT_SetTextWrapMode(hItem,GUI_WRAPMODE_WORD);
        WM_HideWindow(hItem);
        //创建
        hCheck0 = WM_GetDialogItem(hDlg, GUI_ID_CHECK0);
        hCheck1 = WM_GetDialogItem(hDlg, GUI_ID_CHECK1);
        hCheck2 = WM_GetDialogItem(hDlg, GUI_ID_CHECK2);
        
        CHECKBOX_SetText(hCheck0, Sys_Lib_GetDispStr(DP_GUI));
        CHECKBOX_SetText(hCheck1, Sys_Lib_GetDispStr(DP_BDU));
        CHECKBOX_SetText(hCheck2, Sys_Lib_GetDispStr(DP_PMU));
        
        BUTTON_SetText(hButton0,Sys_Lib_GetDispStr(DP_EXIT));
        BUTTON_SetText(hButton1,Sys_Lib_GetDispStr(DP_UPGRADE));
        BUTTON_SetTextWrapMode(hButton1,GUI_WRAPMODE_WORD);
        break;
    case WM_NOTIFY_PARENT:
        NCode = pMsg->Data.v;
        Id = WM_GetId(pMsg->hWinSrc);
        switch(NCode)
        {
        case WM_NOTIFICATION_CLICKED:
            if((UpdateSend_Flag == 0) && (UpdateRecv_Flag == 0))//选择升级程序界面
            {
                switch(Id)
                {
                case GUI_ID_BUTTON0: //退出
                    PlanMenuType = SYSTEM_SETTING_MENU;
                    Task |=MEAU_UPDATE;
                    hItem = WM_GetDialogItem(Menu_hWin.ActivedWin, GUI_ID_MULTIPAGE0);
                    MULTIPAGE_SetPageChangeEnable(hItem,1);
                    MenuOperateStatus &= ~FRAMEWIN_POP;//FRAMEWIN_POP标志位在输入密码时加入到寄存器中，退出时需要清除
                    break;
                case GUI_ID_BUTTON1: //升级
                    UpdateSend_Flag |= (CHECKBOX_GetState(WM_GetDialogItem(WM_GetParent(pMsg->hWinSrc),GUI_ID_CHECK0))&0x1)<<0;//GUI
                    UpdateSend_Flag |= (CHECKBOX_GetState(WM_GetDialogItem(WM_GetParent(pMsg->hWinSrc),GUI_ID_CHECK1))&0x1)<<1;//BDU
                    UpdateSend_Flag |= (CHECKBOX_GetState(WM_GetDialogItem(WM_GetParent(pMsg->hWinSrc),GUI_ID_CHECK2))&0x1)<<2;//PMU
                    UpdateSend_Flag |= 0xA0;//0b10100000 表示可以开始发送握手命令了
                    UpdateRecv_Flag |= 0xA0;//0b10100000 表示开始接收应答命令
                    //得到了想要升级的程序的标志，准备发送升级握手命令，隐藏当前的对话框，并且显示提示信息
                    break;
                }
            }
            else if(((UpdateSend_Flag & 0xF0) == 0x90) && ((UpdateRecv_Flag & 0xF0) == 0x90))//出错后，询问是否继续
            {
                switch(Id)
                {
                case GUI_ID_BUTTON0: //退出
                    //取消升级
                    BDU_UpdateFlag = 0xAA;//发送
                    PMU_UpdateFlag = 0xAA;
                    SendOperationMessage(UPDATE_COMMAND,USART0);
                    SendOperationMessage(UPDATE_COMMAND,USART3);

                    PlanMenuType = SYSTEM_SETTING_MENU;
                    Task |=MEAU_UPDATE;
                    hItem = WM_GetDialogItem(Menu_hWin.ActivedWin, GUI_ID_MULTIPAGE0);
                    MULTIPAGE_SetPageChangeEnable(hItem,1);
                    MenuOperateStatus &= ~FRAMEWIN_POP;//FRAMEWIN_POP标志位在输入密码时加入到寄存器中，退出时需要清除
                     
                    //清除EEPROM

                    UpdateSend_Flag = 0;
                    UpdateRecv_Flag = 0;
                    UpdateProgramTimeOutCnt = 0;
                    break;
                case GUI_ID_BUTTON1: //升级
                    UpdateSend_Flag &= ~0x90;   
                    UpdateSend_Flag |= 0xA0;   //进入完成界面
                    break;
                }
            }
            else if(((UpdateSend_Flag & 0xF0) == 0xA0) && ((UpdateRecv_Flag & 0xF0) == 0x90))//接收完成，询问是继续升级还是取消
            {
                switch(Id)
                {
                case GUI_ID_BUTTON0: //退出
                    PlanMenuType = SYSTEM_SETTING_MENU;
                    Task |=MEAU_UPDATE;
                    hItem = WM_GetDialogItem(Menu_hWin.ActivedWin, GUI_ID_MULTIPAGE0);
                    MULTIPAGE_SetPageChangeEnable(hItem,1);
                    MenuOperateStatus &= ~FRAMEWIN_POP;//FRAMEWIN_POP标志位在输入密码时加入到寄存器中，退出时需要清除
                    //清除EEPROM
                    RTEEPROMwrite(ADDRESSFLAG_ADDR,0x00);
                    RTEEPROMwrite(SOFTWARE_UPGRADE_ADDR1,0x00);
                    RTEEPROMwrite(SOFTWARE_UPGRADE_ADDR2,0x00);
                    RTEEPROMwrite(SOFTWARE_UPGRADE_BACKUP_ADDR1,0x00);
                    RTEEPROMwrite(SOFTWARE_UPGRADE_BACKUP_ADDR2,0x00);
                    //取消升级
                    BDU_UpdateFlag = 0xAA;//发送
                    PMU_UpdateFlag = 0xAA;
                    SendOperationMessage(UPDATE_COMMAND,USART0);
                    SendOperationMessage(UPDATE_COMMAND,USART3);   

                    UpdateSend_Flag = 0;
                    UpdateRecv_Flag = 0;
                    break;
                case GUI_ID_BUTTON1: //升级
                    PlanMenuType = SYSTEM_SETTING_MENU;
                    Task |=MEAU_UPDATE;
                    hItem = WM_GetDialogItem(Menu_hWin.ActivedWin, GUI_ID_MULTIPAGE0);
                    MULTIPAGE_SetPageChangeEnable(hItem,1);
                    MenuOperateStatus &= ~FRAMEWIN_POP;//FRAMEWIN_POP标志位在输入密码时加入到寄存器中，退出时需要清除
                    UpdateSend_Flag = 0;
                    UpdateRecv_Flag = 0;
                    break;
                }
            }
            break;
        }
        break;
    default:
        WM_DefaultProc(pMsg);
    }
}


WM_HWIN _EngineerUpdateWin(void)
{
    WM_HWIN hDialog;
    hDialog = GUI_CreateDialogBox(_aDialogCreateUpdate,
                                  GUI_COUNTOF(_aDialogCreateUpdate),
                                  &_cbDialogUpdate, 0, 0, 0);
    return(hDialog);
}
/************************************************************************************
*函数名；UpdateProgramWin
*函数功能：查询在线升级命令的状态，根据状态
           改变升级菜单的内容并且发送和分析接收到的命令。
*调用位置：maintask.c的INFO_WINDOW_UPDATE任务中，每隔118ms左右调用一次
*备注：函数中的四种状态
       发送：UpdateSend_Flag     0xA0    ------      UpdateRecv_Flag    0xA0   开始发送数据
       接收：UpdateSend_Flag     0x90    ------      UpdateRecv_Flag    0xA0   等待接收数据
       分析：UpdateSend_Flag     0x90    ------      UpdateRecv_Flag    0x90   接收完成，分析数据
       完成：UpdateSend_Flag     0xA0    ------      UpdateRecv_Flag    0x90   分析完成，确认操作
             
*************************************************************************************
*/
void UpdateProgramWin(WM_HWIN hWin)
{
    WM_HWIN hButton0,hButton1,hItem;
    WM_HWIN hCheck0,hCheck1,hCheck2,hCheck3;
    static int Timer = 0; //接收计时，最大2s
    int Finish_Flag = 0;
    char string[60] = {'\0'};
    static char  UpdateFlag = 0;//程序升级地址选择标志位
    static char  WriteEEPROM_Flag = 0;//为1时可以写eeprom，为0时禁止写eeprom

    hButton0 = WM_GetDialogItem(hWin, GUI_ID_BUTTON0);
    hButton1 = WM_GetDialogItem(hWin, GUI_ID_BUTTON1);
    hCheck0  = WM_GetDialogItem(hWin, GUI_ID_CHECK0);
    hCheck1  = WM_GetDialogItem(hWin, GUI_ID_CHECK1);
    hCheck2  = WM_GetDialogItem(hWin, GUI_ID_CHECK2);
    hCheck3  = WM_GetDialogItem(hWin, GUI_ID_CHECK3);
    hItem    = WM_GetDialogItem(hWin, GUI_ID_TEXT0);   //提示语句栏

    if(((UpdateSend_Flag & 0xF0) == 0xA0) && ((UpdateRecv_Flag & 0xF0) == 0xA0))//开始发送命令
    {
        //更新显示
        WM_HideWindow(hButton0);
        WM_HideWindow(hButton1);
        WM_HideWindow(hCheck0);
        WM_HideWindow(hCheck1);
        WM_HideWindow(hCheck2);
        WM_HideWindow(hCheck3);
        WM_ShowWindow(hItem);
        //允许写eeprom
        WriteEEPROM_Flag = 1;
        
        TEXT_SetText(hItem,Sys_Lib_GetDispStr(DP_PLEASEWAIT));        
        
        //发送命令
        Timer = 0;//首先清空接收计时器
        
        if((UpdateSend_Flag & 0x01) == 0x01)//GUI 进行升级
        {
            UpdateRecv_Flag |= 0x1;
        }
        if((UpdateSend_Flag & 0x02) != (UpdateRecv_Flag & 0x02))//BDU    发送被置位，并且没有接收到回应时发送命令
        {
            BDU_UpdateFlag = 0x55;//发送
            SendOperationMessage(UPDATE_COMMAND,USART0);
        }
        if((UpdateSend_Flag & 0x04) != (UpdateRecv_Flag & 0x04))//PMU    送被置位，并且没有接收到回应时发送命令
        {
            PMU_UpdateFlag = 0x55;//发送
            SendOperationMessage(UPDATE_COMMAND,USART3);
        }       
        UpdateSend_Flag &= ~0xA0;//清除发送命令
        UpdateSend_Flag |= 0x90; //更新为发送完成，等待接受

        //send 
    }
    else if(((UpdateSend_Flag & 0xF0) == 0x90) && ((UpdateRecv_Flag & 0xF0) == 0xA0))//等待应答完成，超时2s即完成一次
    {
        Timer++;//Timer每一个数字表示118ms左右。
        if(Timer >= 16)//接收一次超时，进入接收检测
        {
            UpdateRecv_Flag &= ~0xA0;//清除正在接受标志
            UpdateRecv_Flag |= 0x90; //更新为接收完成

        }
    }
    else if(((UpdateSend_Flag & 0xF0) == 0x90) && ((UpdateRecv_Flag & 0xF0) == 0x90))//应答接收完成（未判断正确性）
    {
        if(((UpdateSend_Flag & 0x0F) == (UpdateRecv_Flag & 0x0F)) && ((UpdateRecv_Flag & 0x0F) != 0))//接收完成，并且结果是正确的
        {
            //设置为接收成功标志
            UpdateSend_Flag &= ~0x90;
            UpdateSend_Flag |= 0xA0; 
        }
        else if(UpdateProgramTimeOutCnt < 4)//重新发送
        {
            UpdateSend_Flag &= ~0x90;//清除发送完成命令
            UpdateSend_Flag |= 0xA0; //更新为等待发送
            UpdateRecv_Flag &= ~0x90;
            UpdateRecv_Flag |= 0xA0;  //正在接受
            UpdateProgramTimeOutCnt++;
        }
        else if(UpdateProgramTimeOutCnt >= 4)//超时，报错，并且询问是否继续升级程序
        {
            //弹起按键
            BUTTON_SetPressed(hButton0,0);
            BUTTON_SetPressed(hButton1,0);
            //判断报错类型

            WM_ShowWindow(hButton0);
            WM_ShowWindow(hButton1);
            WM_ShowWindow(hItem);
            //根据不同的回复报不同的错误 UpdateRecv_Flag

            if((UpdateRecv_Flag&0x0f) == 0)//没有可以升级的程序
            {
                strcpy(string, Sys_Lib_GetDispStr(DP_NOPROGRAMUPGRAD));
            }
            else 
            {
                if(((UpdateSend_Flag&0x02) == 0x02)&&((UpdateRecv_Flag&0x2) == 0))//BDU接收标志位
                {
                    strcat(string,"BDU");
                    BDU_UpdateFlag = 0xAA;//发送
                    SendOperationMessage(UPDATE_COMMAND,USART0);
                }
                if(((UpdateSend_Flag&0x04) == 0x04)&&((UpdateRecv_Flag&0x4) == 0))//PMU接收标志位
                {
                    if(((UpdateSend_Flag&0x02) == 0x02)&&((UpdateRecv_Flag&0x2) == 0))//BDU接收标志位
                    {
                        strcat(string," and PMU");
                    }
                    else 
                    {
                        strcat(string,"PMU");
                    }
                    PMU_UpdateFlag = 0xAA;//发送
                    SendOperationMessage(UPDATE_COMMAND,USART3);
                }
                //strcat(string,"\nno response!");
                
                strcat(string,Sys_Lib_GetDispStr(DP_NORESPONSE));
                
            }
            
            BUTTON_SetText(hButton0,Sys_Lib_GetDispStr(DP_CANCEL));
            BUTTON_SetText(hButton1,Sys_Lib_GetDispStr(DP_CONTINUE));            
            
            TEXT_SetText(hItem,string);            
        }
    }
    else if(((UpdateSend_Flag & 0xF0) == 0xA0) && ((UpdateRecv_Flag & 0xF0) == 0x90))//接收完成，询问是继续升级还是取消
    {
        UpdateProgramTimeOutCnt = 0;
        //向EEPROM存储升级消息
        if((UpdateRecv_Flag&0x0f) == 0)//没有可以升级的程序
        {
            UpdateSend_Flag = 0;
            UpdateRecv_Flag = 0;
            BUTTON_SetPressed(hButton0,0);
            BUTTON_SetPressed(hButton1,0);
            WM_ShowWindow(hButton0);
            WM_ShowWindow(hButton1);
            WM_ShowWindow(hCheck0);
            WM_ShowWindow(hCheck1);
            WM_ShowWindow(hCheck2);
            WM_ShowWindow(hCheck3);
            WM_HideWindow(hItem);
            //创建

                BUTTON_SetText(hButton0,Sys_Lib_GetDispStr(DP_EXIT));
                BUTTON_SetText(hButton1,Sys_Lib_GetDispStr(DP_UPGRADE));
        }
        else
        {
            if(WriteEEPROM_Flag)//第一次进入该界面写eeprom
            {
                //开始升级
                UpdateFlag = 0x55;//选择 主 地址作为程序升级标志
//                if(UpdateFlag == 0x55)//写主地址
                {
                    RTEEPROMwrite(SOFTWARE_UPGRADE_ADDR1,(UpdateRecv_Flag & 0x0f));
                    RTEEPROMwrite(SOFTWARE_UPGRADE_ADDR2,0xAA);

                    RTEEPROMwrite(ADDRESSFLAG_ADDR,UpdateFlag);
                    WriteEEPROM_Flag = 0;
                }
                if((RTEEPROMread(SOFTWARE_UPGRADE_ADDR2) != 0xAA) 
                || (RTEEPROMread(SOFTWARE_UPGRADE_ADDR1) != (UpdateRecv_Flag & 0x0f)))//主存储地址错误,或者选择了此地址
                {
                    UpdateFlag = 0xAA;//选择 次 地址作为程序升级标志
                    RTEEPROMwrite(SOFTWARE_UPGRADE_ADDR1,0);
                    RTEEPROMwrite(SOFTWARE_UPGRADE_ADDR2,0);
                    
                    RTEEPROMwrite(SOFTWARE_UPGRADE_BACKUP_ADDR1,(UpdateRecv_Flag & 0x0f));
                    RTEEPROMwrite(SOFTWARE_UPGRADE_BACKUP_ADDR2,0xAA);

                    RTEEPROMwrite(ADDRESSFLAG_ADDR,UpdateFlag);
                    WriteEEPROM_Flag = 0;
                }
                //取消没有响应的板子
                if(!((UpdateRecv_Flag&0x02) == 0x02))//BDU接收标志位
                {
                    BDU_UpdateFlag = 0xAA;//发送
                    SendOperationMessage(UPDATE_COMMAND,USART0);
                }
                if(!((UpdateRecv_Flag&0x04) == 0x04))//PMU接收标志位
                {
                    PMU_UpdateFlag = 0xAA;//发送
                    SendOperationMessage(UPDATE_COMMAND,USART3);
                }
            }
            //弹起按键
            BUTTON_SetPressed(hButton0,0);
            BUTTON_SetPressed(hButton1,0);
                        
            WM_ShowWindow(hButton0);
            WM_ShowWindow(hButton1);
            WM_ShowWindow(hItem);
            
            BUTTON_SetText(hButton0,Sys_Lib_GetDispStr(DP_CANCEL));
            BUTTON_SetText(hButton1,Sys_Lib_GetDispStr(DP_CONTINUE));
            TEXT_SetText(hItem,Sys_Lib_GetDispStr(DP_RESTARTMACHINE));            
        }
    }
}

static const GUI_WIDGET_CREATE_INFO _aDialogCreateText[] =
{
    { WINDOW_CreateIndirect,    "",       0,              45,   375, 220, 20, FRAMEWIN_CF_ACTIVE },
    { TEXT_CreateIndirect, "",GUI_ID_TEXT4,  0,   0,220,  20,   TEXT_CF_VCENTER|TEXT_CF_HCENTER},//Please rotate button!//TEXT_CF_LEFT
//  //TextExCtrl_CreateIndirect
};
static void _cbDialog108(WM_MESSAGE * pMsg)
{
	WM_HWIN hDlg,hItem;
    hDlg = pMsg->hWin;
    switch (pMsg->MsgId)
    {
    case WM_INIT_DIALOG:
        hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT4);
        //TEXT_SetTextFont();
        TEXT_SetTextColor(hItem, GUI_BLACK);
		//WINDOW_SetBkColor(hDlg, GUI_WHITE);
        TEXT_SetText(hItem, "");
        break;
    default:
        WM_DefaultProc(pMsg);
    }
        

}
/***************************************************************************************************
*   参数限制提示窗口  //sun add 20140521
*
*
*/
WM_HWIN _ControlSettingWin()
{
    WM_HWIN hWin1;

    //hWin1 = FRAMEWIN_Create("",0,WM_CF_SHOW,550,60,80,100);
    //FRAMEWIN_AddText(5,3,440,20,hWin1,"",GUI_TA_LEFT|GUI_TA_VCENTER);
	//hWin3 = MULTIPAGE_CreateEx(0, 0, 450, 0, hWin1, WM_CF_SHOW, 0, GUI_ID_MULTIPAGE0);

	hWin1 = GUI_CreateDialogBox(_aDialogCreateText,
                                GUI_COUNTOF(_aDialogCreateText),
                                &_cbDialog108, 0, 0, 0);
	Menu_hWin.TextWin = hWin1;
    //FRAMEWIN_AddCloseButton(hWin1,0,5);
    //MULTIPAGE_AddPage(hWin3, hWin2, "");
	return hWin1;
}

















//*******************************************************************************************
//趋势

static const GUI_WIDGET_CREATE_INFO _aDialogCreate100[] =  //sun add 20140514
{
  { WINDOW_CreateIndirect,    "Dialog 2",   0,   0,    0,   600,  350, FRAMEWIN_CF_ACTIVE },
  { TEXT_CreateIndirect,           "",         GUI_ID_TEXT0,       20,   20,   400,  200,  TEXT_CF_HCENTER|TEXT_CF_VCENTER},
};

/*************************************************************************************************************
*
*       _cbDialog100
*/
static void _cbDialog100(WM_MESSAGE * pMsg)  //sun add 20140514
{
    WM_HWIN hDlg;
    TextExCtrl_Handle hObj;
    hDlg = pMsg->hWin;
    switch (pMsg->MsgId)
	{
	case WM_INIT_DIALOG:
		hObj = WM_GetDialogItem(hDlg, GUI_ID_TEXT0);
		TEXT_SetText(hObj,"This is Trend!!!");
		break;
	case WM_NOTIFY_PARENT_REFLECTION:
        FunctionKeyPressedResume(Menu_hWin.hWin,0);
        if(StandbyState==0xFF)
        {
            PlanMenuType = STANDBY_MENU;
        }
        else
        {
            PlanMenuType = MAIN_MENU;
        }
        Task |= MEAU_UPDATE;
        break;
    default:
        WM_DefaultProc(pMsg);
	}
}

/***************************************************************************************************
* @brief  趋势
* @para   
* @retval 
*/
WM_HWIN _TrendChartWin(void)  //sun add 20140514
{
    WM_HWIN hWin1,hWin2,hWin3;
    //hWin = MultiColumnWin_CreateEx(0, 60, 159, 540, 0,
    //                           WM_CF_SHOW, 0, 0);
    hWin1 = FRAMEWIN_Create("",0,WM_CF_SHOW,3,   146, 555, 250);
    FRAMEWIN_AddCloseButton(hWin1,0,5);
    hWin2 = GUI_CreateDialogBox(_aDialogCreate100,
                                GUI_COUNTOF(_aDialogCreate100),
                                &_cbDialog100, 0, 0, 0);
    hWin3 = MULTIPAGE_CreateEx(8, 10, 538, 230, hWin1, WM_CF_SHOW, 0, GUI_ID_MULTIPAGE0);
    
    MULTIPAGE_AddPage(hWin3, hWin2, Sys_Lib_GetDispStr(DP_TRENDS));
    
    MULTIPAGE_SelectPage(hWin3,0);
    return(hWin1);
}

void EmptyAlarmRecordList(ALARM_RECORD_LIST * ListHead)  //sun add 20140701
{
    ListHead->Head = 0;//RTEEPROMread(ALARM_RECORD_LIST_ADDR);
    ListHead->Rear = 0;//RTEEPROMread(ALARM_RECORD_LIST_ADDR+1);
    ListHead->Count = 0;//RTEEPROMread(ALARM_RECORD_LIST_ADDR+2);
    RTEEPROMwrite(ALARM_RECORD_LIST_ADDR, 0);
    RTEEPROMwrite(ALARM_RECORD_LIST_ADDR+1, 0);
    RTEEPROMwrite(ALARM_RECORD_LIST_ADDR+2, 0);
	RTEEPROMwrite(ALARM_RECORD_LIST_ADDR+3, 0);
	RTEEPROMwrite(ALARM_RECORD_LIST_ADDR+4, 0);
	RTEEPROMwrite(ALARM_RECORD_LIST_ADDR+5, 0);}

void ReviseParamatersAllMode(int mode,BREATH_SET_PARA *Para,int Ph)
{
	if(BreathMode_AfterACGO == ACGO_MODE || BreathMode_AfterMANUAL == MANUAL_MODE)
	{
		Para->PreBreathMode = BreathMode_BeforeACGO;
		ReviseParamatersAllMode_Normal(mode,Para,Ph);
	}
	else
	{
		ReviseParamatersAllMode_Normal(mode,Para,Ph);
	}
}
void ReviseParamatersAllMode_Normal(int mode,BREATH_SET_PARA *Para,int Ph)
{
    int timaybe = 0;
    int index = 0,TiValue = 0;
    Int64U lowFlow = 0;
	//if( (mode != VCV_MODE) && (mode != SIGH_MODE))
	//{
		//Para->SettingTpValue = 0;
		//USART0_TXD_RIGSTER = SEND_SET_TP_FLAG;
		//if(USART0_TXD_Busy_FLAG==FALSE)
		//{
			//Inquire_TXD_Handler(USART0);
		//}			
	//}			
    switch(mode)
    {
    case SIMV_VCV_MODE:
        Para->SettingRateValue = Para->SettingSimvRateValue;
        //if(Para->SettingTiValue >TI_MAX_100)
        //{
        //    Para->SettingTiValue = TI_MAX_100;
        //}
		if(Patient_GetPType() == PATIENTTYPE_ADULT)//ylk md 20191126
		{
			if(Para->SettingTiValue < 20)
			{
				Para->SettingTiValue = 20;
			}
			if(Para->SettingTiValue >TI_MAX_100)
			{
				Para->SettingTiValue = TI_MAX_100;
			}
		}
		else if(Patient_GetPType() == PATIENTTYPE_INFANT)
		{
			if(Para->SettingTiValue > 300)
			{
				Para->SettingTiValue = 300;
			}
		}
		else
		{
			if(Para->SettingTiValue >TI_MAX_100)
			{
				Para->SettingTiValue = TI_MAX_100;
			}
		}

        if(Ph <= MAX_PAW)
        {
            if((Para->SettingPEEPValue)+(Para->SettingPsupportValue)>= Ph)
            {
                if(Ph - Para->SettingPEEPValue -1 < 0)
                {
                    Para->SettingPsupportValue = 0;
                }
                else
                {
                    Para->SettingPsupportValue = Ph - Para->SettingPEEPValue -1;
                }
            }
        }
        else if((Para->SettingPEEPValue)+(Para->SettingPsupportValue)> MAX_PAW)
        {
            Para->SettingPsupportValue = MAX_PAW - Para->SettingPEEPValue;
        }
        if((Para->PreBreathMode == VCV_MODE)
         ||((mode == PSV_MODE)&&(Para->ApnoeaMode == VCV_MODE)))
        {
            // 判断当前频率与Ti是否匹配，如果不匹配，则引入SIMV_VCV的频率
            if((6000/Para->SettingRateValue - EXPTIME_LIMIT)< Para->SettingTiValue)
            {
                Para->SettingRateValue = Para->SettingotherRateValue;
            }
        }
        else
        {
            if((Para->PreBreathMode == PCV_MODE)||(Para->PreBreathMode == PRVC_MODE)||(Para->PreBreathMode == BIPAP_MODE)
                ||((mode == PSV_MODE)&&(Para->ApnoeaMode == PCV_MODE))
                )
            {
                // 判断当前频率与Ti是否匹配，如果不匹配，则引入SIMV_VCV的频率            
                if((6000/Para->SettingRateValue - EXPTIME_LIMIT)< Para->SettingTiValue)
                {
                    Para->SettingRateValue = Para->SettingotherRateValue;
                }
            }
        }
        lowFlow = Para->SettingVtValue*10000/Para->SettingTiValue;
        if(Para->SettingVtValue*100/Para->SettingTiValue  > 1500 )
        {
			Para->SettingVtValue = 15 * Para->SettingTiValue;
            if(Para->SettingVtValue  >= 100)
            {
                if(Para->SettingVtValue%10 != 0)
                {
                    Para->SettingVtValue = Para->SettingVtValue - Para->SettingVtValue%10;
                }
            }
            else
            {
                if( Para->SettingVtValue%5 != 0)
                {
                    Para->SettingVtValue = Para->SettingVtValue - Para->SettingVtValue%5;
                }
            }
        }
        else if(lowFlow < FLOW_MIN)
        //else if(Para->SettingVtValue <  Para->SettingSimvTiValue/6)//sun add 20140513
        {
            if(Para->SettingTiValue >= 100)
            {
                if(((int)(Para->SettingTiValue))%10 != 0)
                {
                    Para->SettingVtValue = Para->SettingTiValue - ((int)( Para->SettingTiValue))%10 + 10;
                }
                else
                {
                    Para->SettingVtValue =Para->SettingTiValue*FLOW_MIN/10000 + 5;//sun add 20140513
                }
            }
            else
            {
                if(((int)(Para->SettingTiValue))%5!=0)
                {
                    Para->SettingVtValue = Para->SettingTiValue - ((int)( Para->SettingTiValue))%5 + 5;
                }
                else
                {
                    Para->SettingVtValue = Para->SettingTiValue*FLOW_MIN/10000 + 5;//sun add 20140513
                }
            }
        }        //Designer Remark 15
        /*if(VT_Min_Flag)
        {
            if(Para->SettingVtValue < VTL_INFANT_LIMIT)
            {
                Para->SettingVtValue = VTL_INFANT_LIMIT;
            }
        }
        else
        {
            if(Para->SettingVtValue < 50)
            {
                Para->SettingVtValue = 50;
            }
        }
        if(Para->SettingVtValue > VTH_ADULT_LIMIT)
        {
            Para->SettingVtValue = VTH_ADULT_LIMIT;
        }*/
		//ylk md 20191126
		if(Para->SettingVtValue < Vt_LowLimit)
		{
			Para->SettingVtValue = Vt_LowLimit;
		}
		if(Para->SettingVtValue > Vt_HighLimit)
		{
			Para->SettingVtValue = Vt_HighLimit;
		}

        if(Para->SettingVtValue > 100)  //sun add 20140618
        {
            Para->SettingVtValue = (Para->SettingVtValue / 10) * 10;
        }
        else
        {
            Para->SettingVtValue = (Para->SettingVtValue / 5) * 5;
        }
        if(Para->SettingPEEPValue < 3)
        {
            Para->SettingPEEPValue = 0;
        }
        if(Para->SettingPsupportValue > PARA_PSUPPORTH_LIMIT)
        {
            Para->SettingPsupportValue = PARA_PSUPPORTH_LIMIT;
        }
        if(Para->SettingTslopeValue > Para->SettingTiValue)
        {
            Para->SettingTslopeValue = Para->SettingTiValue - Para->SettingTiValue%10;
        }
        Para->SettingSimvRateValue = Para->SettingRateValue;//切换完模式后，将新的频率存放到寄存器中，
                                                            //这样可以避免SIMV_V->SIMV_P->SIMV_V 之间连续切换导致频率错误的问题
        break;
    case PRVC_MODE:
    case PCV_MODE:
        Para->SettingRateValue = Para->SettingotherRateValue;
        if((Para->PreBreathMode == SIMV_VCV_MODE) 
         ||(Para->PreBreathMode == SIMV_PCV_MODE)
         ||(Para->PreBreathMode == SIMV_PRVC_MODE))
        {    
            // 判断当前频率与Ti是否匹配，如果不匹配，则引入SIMV_VCV的频率            
            if((6000/Para->SettingRateValue - EXPTIME_LIMIT)< Para->SettingTiValue)
            {
//                 if(Para->SettingSimvRateValue < 4)
//                 {
//                     Para->SettingRateValue = 4; 
//                 }
//                 else
//                 {
                    Para->SettingRateValue = Para->SettingSimvRateValue; 
//                 }
            }
            TiConvertIE(Para->SettingTiValue, Para);
            //保证吸呼比和时间频率保持一致
            Para->SettingTiValue = IEConvertTi(0,Para->SettingIEValue, Para);
        }
        if(Para->SettingTiValue >TI_MAX_100)
        {
            Para->SettingTiValue = TI_MAX_100;
        }
        if(mode == PCV_MODE)
        {
            if(Ph <= MAX_PAW)
            {
                if((Para->SettingPEEPValue)+(Para->SettingPinspValue)>= Ph)
                {
                    if(Ph <8)
                    {
                        Para->SettingPinspValue = 5;
                        Para->SettingPEEPValue = 0;
                        AlarmSettingPara.PressureHighValue = 8;
                    }
                    else
                    {
                        if(Ph - Para->SettingPEEPValue > 5)
                        {
                            Para->SettingPinspValue = Ph - Para->SettingPEEPValue - 1;
                        }
                        else
                        {
                            Para->SettingPinspValue = 5;
                            Para->SettingPEEPValue =  Ph - 6;
                        }
                    }
                }
            }
            else
            {
                if((Para->SettingPEEPValue)+(Para->SettingPinspValue)> MAX_PAW)
                {
                    Para->SettingPinspValue = MAX_PAW - Para->SettingPEEPValue;
                }
            }
        }
        else if(mode == PRVC_MODE)
        {
            if(Para->SettingPEEPValue >= Ph)
            {
                Para->SettingPEEPValue = Ph - 1;
            }
        }
        if(Para->SettingPEEPValue < 3)
        {
            Para->SettingPEEPValue = 0;
        }
        if(Para->SettingPinspValue > PARA_PINSPH_LIMIT)
        {
            Para->SettingPinspValue = PARA_PINSPH_LIMIT;
        }
        if(Para->SettingTslopeValue > Para->SettingTiValue)
        {
            Para->SettingTslopeValue = Para->SettingTiValue;
        }
        Para->SettingotherRateValue = Para->SettingRateValue;
        break;
    case VCV_MODE:
    case SIGH_MODE:
        Para->SettingRateValue = Para->SettingotherRateValue;
        //如果不是vcv和sigh之间切换 ，Tpause = 0    20140318 
        if( (Para->PreBreathMode != VCV_MODE)
           &&(Para->PreBreathMode != SIGH_MODE))
        {
            Para->SettingTpValue = 0;
        }

        if((Para->PreBreathMode == SIMV_PCV_MODE)
          ||(Para->PreBreathMode == SIMV_PRVC_MODE)
          ||(Para->PreBreathMode == SIMV_VCV_MODE))//???
        {
            // 判断当前频率与Ti是否匹配，如果不匹配，则引入SIMV_VCV的频率            
            if((6000/Para->SettingRateValue - EXPTIME_LIMIT)< Para->SettingTiValue)
            {
//                 if(Para->SettingSimvRateValue < 4)
//                 {
//                     Para->SettingRateValue = 4; 
//                 }
//                 else
//                 {
                    Para->SettingRateValue = Para->SettingSimvRateValue; 
//                 }
            }
            //判断当前吸气最大吸气时间是否与暂停时间匹配，如果不匹配，则调整暂停时间
            if((6000/Para->SettingRateValue - EXPTIME_LIMIT)< (Para->SettingTiValue*(Para->SettingTpValue + 100))/100.0)
            {
                int num,temp;
                temp = (6000/Para->SettingRateValue - EXPTIME_LIMIT - Para->SettingTiValue)*100/Para->SettingTiValue;
                num = temp % 5;
                Para->SettingTpValue = temp - num;
            }
            TiConvertIE(Para->SettingTiValue, Para);
            //保证吸呼比和时间频率保持一致
            Para->SettingTiValue = IEConvertTi(0,Para->SettingIEValue, Para);
            //if(Para->SettingTiValue >TI_MAX_100)
            //{
            //    Para->SettingTiValue = TI_MAX_100;
            //}
			if(Patient_GetPType() == PATIENTTYPE_ADULT)
			{
				if(Para->SettingTiValue < 20)
				{
					Para->SettingTiValue = 20;
				}
				if(Para->SettingTiValue >TI_MAX_100)
				{
					Para->SettingTiValue = TI_MAX_100;
				}
			}
			else if(Patient_GetPType() == PATIENTTYPE_INFANT)
			{
				if(Para->SettingTiValue > 300)
				{
					Para->SettingTiValue = 300;
				}
			}
			else
			{
				if(Para->SettingTiValue >TI_MAX_100)
				{
					Para->SettingTiValue = TI_MAX_100;
				}
			}
        }
        if(Para->SettingIEValue >= 6)
        {
            TiValue = (60*10.0*(100-Breath_Setting_Tmp.SettingTpValue)) / ((10+IE_RatioTable[Breath_Setting_Tmp.SettingIEValue])*Breath_Setting_Tmp.SettingRateValue*1.0) + 0.5; //ylk modify 20180102 BeathSettingPara
        }
        else
        {
            TiValue = (60.0*IE_RatioTable[Breath_Setting_Tmp.SettingIEValue]*(100-Breath_Setting_Tmp.SettingTpValue)) / ((10+IE_RatioTable[Breath_Setting_Tmp.SettingIEValue])*Breath_Setting_Tmp.SettingRateValue);
        }
        lowFlow = Para->SettingVtValue*10000/TiValue;//Para->SettingTiValue
        if(Para->SettingVtValue*100/TiValue  > 1500)
        {
            Para->SettingVtValue = 15 * TiValue;
            if(Para->SettingVtValue  >= 100)
            {
                if(Para->SettingVtValue%10 != 0)
                {
                    Para->SettingVtValue = Para->SettingVtValue - Para->SettingVtValue%10 ;
                }
            }
            else
            {
                if( Para->SettingVtValue%5 != 0)
                {
                    Para->SettingVtValue = Para->SettingVtValue - Para->SettingVtValue%5;
                }
            }
        }
        else if(lowFlow < FLOW_MIN)
        //else if(Para->SettingVtValue < Para->SettingTiValue/6)//sun add 20140513
        {
            if(TiValue  >= 100)
            {
                if(TiValue%10!=0)
                {
                    Para->SettingVtValue = TiValue - TiValue%10 + 10;
                }
                else
                {
                    Para->SettingVtValue =TiValue*FLOW_MIN/10000 +5;//sun add 20140513
                }
            }
            else
            {
                if(TiValue%5!=0)
                {
                    Para->SettingVtValue = TiValue - TiValue%5 + 5;
                }
                else
                {
                    Para->SettingVtValue =  TiValue*FLOW_MIN/10000 +5;//sun add 20140513
                }
            }
        }
        
        /*if(VT_Min_Flag)
        {
            if(Para->SettingVtValue < VTL_INFANT_LIMIT)
            {
                Para->SettingVtValue = VTL_INFANT_LIMIT;
            }
        }
        else
        {
            if(Para->SettingVtValue < 50)
            {
                Para->SettingVtValue = 50;
            }
        }
        if(Para->SettingVtValue > VTH_ADULT_LIMIT)
        {
            Para->SettingVtValue = VTH_ADULT_LIMIT;
        }*/
		if(Para->SettingVtValue <Vt_LowLimit)//ylk md 20191126
		{
			Para->SettingVtValue = Vt_LowLimit;
		}
		if(Para->SettingVtValue > Vt_HighLimit)
		{
			Para->SettingVtValue = Vt_HighLimit;
		}

        if(Para->SettingVtValue > 100)  //sun add 20140618
        {
            Para->SettingVtValue = (Para->SettingVtValue / 10) * 10;
        }
        else
        {
            Para->SettingVtValue = (Para->SettingVtValue / 5) * 5;
        }
        if(Para->SettingTpValue < 0)
        {
            Para->SettingTpValue = 0;
        }
        if(Para->SettingTpValue > 60)
        {
            Para->SettingTpValue = 60;
        }
        if(Para->SettingTslopeValue > Para->SettingTiValue)
        {
            Para->SettingTslopeValue = Para->SettingTiValue;
        }
        Para->SettingotherRateValue = Para->SettingRateValue;
        break;

    case PSV_MODE:
        Para->SettingRateValue = Para->SettingotherRateValue;
        if(Para->ApnoeaMode == VCV_MODE)
        {
            //if(Para->SettingTiValue >TI_MAX_100)
            //{
            //    Para->SettingTiValue = TI_MAX_100;
            //}
			if(Patient_GetPType() == PATIENTTYPE_ADULT)//ylk md 20191126
			{
				if(Para->SettingTiValue < 20)
				{
					Para->SettingTiValue = 20;
				}
				if(Para->SettingTiValue >TI_MAX_100)
				{
					Para->SettingTiValue = TI_MAX_100;
				}
			}
			else if(Patient_GetPType() == PATIENTTYPE_INFANT)
			{
				if(Para->SettingTiValue > 300)
				{
					Para->SettingTiValue = 300;
				}
			}
			else
			{
				if(Para->SettingTiValue >TI_MAX_100)
				{
					Para->SettingTiValue = TI_MAX_100;
				}
			}
            
//             if((Para->PreBreathMode == SIMV_VCV_MODE)||(Para->PreBreathMode == SIMV_PCV_MODE))
			if((Para->PreBreathMode == SIMV_PCV_MODE)
				||(Para->PreBreathMode == SIMV_PRVC_MODE)
				||(Para->PreBreathMode == SIMV_VCV_MODE))//???           
			{
                // 判断当前频率与Ti是否匹配，如果不匹配，则引入SIMV_VCV的频率
                if((6000/Para->SettingRateValue - EXPTIME_LIMIT)< Para->SettingTiValue)
                {
                    Para->SettingRateValue = Para->SettingSimvRateValue;
                }
				//new add 
				TiConvertIE(Para->SettingTiValue, Para);
				//保证吸呼比和时间频率保持一致
				Para->SettingTiValue = IEConvertTi(0,Para->SettingIEValue, Para);

            }
            lowFlow = Para->SettingVtValue*10000/Para->SettingTiValue;
            if(Para->SettingVtValue*100/Para->SettingTiValue  > 1500)
            {
                Para->SettingVtValue = 15 * Para->SettingTiValue;
                if(Para->SettingVtValue  >= 100)
                {
                    if(Para->SettingVtValue%10 != 0)
                    {
                        Para->SettingVtValue = Para->SettingVtValue - Para->SettingVtValue%10 ;
                    }
                }
                else
                {
                    if( Para->SettingVtValue%5 != 0)
                    {
                        Para->SettingVtValue = Para->SettingVtValue - Para->SettingVtValue%5 ;
                    }
                }
            }
            else if(lowFlow < FLOW_MIN)
            //else if(Para->SettingVtValue < Para->SettingTiValue/6)//sun add 20140513
            {
                if(Para->SettingTiValue  >= 100)
                {
                    if(Para->SettingTiValue%10!=0)
                    {
                        Para->SettingVtValue = Para->SettingTiValue - Para->SettingTiValue%10 + 10;
                    }
                    else
                    {
                        Para->SettingVtValue =Para->SettingTiValue*FLOW_MIN/10000 + 5;//sun add 20140513
                    }
                }
                else
                {
                    if( Para->SettingTiValue%5!=0)
                    {
                        Para->SettingVtValue = Para->SettingTiValue - Para->SettingTiValue%5 + 5;
                    }
                    else
                    {
                        Para->SettingVtValue =  Para->SettingTiValue*FLOW_MIN/10000 + 5;//sun add 20140513
                    }
                }
            }
        }
        else if(Para->ApnoeaMode == PCV_MODE)
        {
            if(Ph <= MAX_PAW)
            {
                if(Para->SettingPinspValue > Para->SettingPsupportValue)
                {  
                    if( (Para->SettingPEEPValue)+(Para->SettingPinspValue) >= Ph )
                    {
                        if(Ph < 8)
                        {
                            Para->SettingPinspValue = 5;
                            Para->SettingPsupportValue = 5;
                            Para->SettingPEEPValue = 0;
                            AlarmSettingPara.PressureHighValue = 8;
                        }
                        else    
                        {
                            if(Ph - Para->SettingPEEPValue > 5)
                            {
                                Para->SettingPinspValue = Ph - Para->SettingPEEPValue - 1;
                            }
                            else
                            {
                                Para->SettingPinspValue = 5;
                                Para->SettingPEEPValue =  Ph - 6;
                            }
                            if((Para->SettingPEEPValue)+(Para->SettingPsupportValue)>= Ph)
                            {
                                Para->SettingPsupportValue = Ph - Para->SettingPEEPValue -1;
                            }
                        }
                    }
                }
                else
                {
                    if((Para->SettingPEEPValue)+(Para->SettingPsupportValue) >= Ph)
                    {
                        if(Ph < 8)
                        {
                            Para->SettingPsupportValue = 5;
                            Para->SettingPinspValue = 5;
                            Para->SettingPEEPValue = 0;
                            AlarmSettingPara.PressureHighValue = 8;
                        }
                        else
                        {
                            if(Ph <= MAX_PAW)  //sun add 20140512
                            {
                                if((Para->SettingPEEPValue)+(Para->SettingPsupportValue)>= Ph)  //sun add 20140512
                                {
                                    Para->SettingPsupportValue = Ph - Para->SettingPEEPValue - 1;
                                }
                            }
                            else  //sun add 20140512
                            {
                                if((Para->SettingPEEPValue)+(Para->SettingPsupportValue)> MAX_PAW)  //sun add 20140512
                                {
                                    Para->SettingPsupportValue = MAX_PAW - Para->SettingPEEPValue;
                                }
                            }
                        }
                    }
                }

            }
            else
            {
                if((Para->SettingPEEPValue)+(Para->SettingPinspValue)> MAX_PAW)
                {
                    Para->SettingPinspValue = MAX_PAW - Para->SettingPEEPValue;
                }
                if((Para->SettingPEEPValue)+(Para->SettingPsupportValue)> MAX_PAW)
                {
                    Para->SettingPsupportValue = MAX_PAW - Para->SettingPEEPValue;
                }
            }
        }
//         if(Para->SettingSimvRateValue < 4) /ylk del 20190102
//         {
//             Para->SettingRateValue = 4; 
//         }
        if(Para->SettingPinspValue > PARA_PINSPH_LIMIT)
        {
            Para->SettingPinspValue = PARA_PINSPH_LIMIT;
        }
        if(Para->SettingPsupportValue < 0)
        {
            Para->SettingPsupportValue = 0;
        }
        if(Para->SettingPsupportValue > PARA_PSUPPORTH_LIMIT)
        {
            Para->SettingPsupportValue = PARA_PSUPPORTH_LIMIT;
        }
        if(Para->SettingPEEPValue < 3)
        {
            Para->SettingPEEPValue = 0;
        }
        if(Para->SettingTslopeValue > Para->SettingTiValue)
        {
            Para->SettingTslopeValue = Para->SettingTiValue;
        }
        Para->SettingotherRateValue = Para->SettingRateValue;
        break;
    case SIMV_PCV_MODE:
    case SIMV_PRVC_MODE:
        Para->SettingRateValue = Para->SettingSimvRateValue;
        
        // 判断当前频率与Ti是否匹配，如果不匹配，则引入SIMV_VCV的频率            
        if((6000/Para->SettingRateValue - EXPTIME_LIMIT)< Para->SettingTiValue)
        {
            Para->SettingRateValue = Para->SettingotherRateValue; //VCV模式下的频率只在小于SIMV模式下的频率是才会进入到这里
        }

        if(Para->SettingTiValue > TI_MAX_100)
        {
            Para->SettingTiValue = TI_MAX_100;
        }
        if(mode == SIMV_PCV_MODE)
        {
            //先判断Pinsp和peep的关系，优先调整peep
            if(Ph<=MAX_PAW)
            {
                if((Para->SettingPEEPValue)+(Para->SettingPinspValue)>= Ph)
                {
                    if(Ph < 6)
                    {
                        Para->SettingPinspValue = 5;
                        Para->SettingPEEPValue = 0;
                        Ph = 6;
                    }
                    else if((Ph - Para->SettingPEEPValue ) < 6)
                    {
                        Para->SettingPinspValue = 5;
                        Para->SettingPEEPValue = Ph - Para->SettingPinspValue - 1;
                    }
                    else
                    {
                        Para->SettingPinspValue = Ph - Para->SettingPEEPValue - 1;
                    }
                }
            }
            else
            {
                if((Para->SettingPEEPValue)+(Para->SettingPinspValue)> MAX_PAW)
                {
                    Para->SettingPinspValue = MAX_PAW - Para->SettingPEEPValue;
                }
            }
        }
        if(Para->SettingPEEPValue < 3)
        {
            Para->SettingPEEPValue = 0;
        }
        //判断peep和Psupp的关系，优先调整psupp
        if(Ph<=MAX_PAW)
        {
            if((Para->SettingPEEPValue)+(Para->SettingPsupportValue)>= Ph)
            {
                if(Ph - Para->SettingPEEPValue -1 < 0)
                {
                    Para->SettingPsupportValue = 0;
                    Para->SettingPEEPValue = Ph - 1;

                }
                else if(Ph - Para->SettingPEEPValue -1  >PARA_PSUPPORTH_LIMIT)
                {
                    Para->SettingPsupportValue = PARA_PSUPPORTH_LIMIT;
                }
                else
                {
                    Para->SettingPsupportValue = Ph - Para->SettingPEEPValue -1;
                }
            }
        }
        else
        {
            if((Para->SettingPEEPValue)+(Para->SettingPsupportValue)> MAX_PAW)
            {
                Para->SettingPsupportValue = MAX_PAW - Para->SettingPEEPValue;
            }
        }
        if(Para->SettingPEEPValue < 3)
        {
            Para->SettingPEEPValue = 0;
        }
        if(Para->SettingPinspValue > PARA_PINSPH_LIMIT)
        {
            Para->SettingPinspValue = PARA_PINSPH_LIMIT;
        }
        if(Para->SettingPsupportValue > PARA_PSUPPORTH_LIMIT)
        {
            Para->SettingPsupportValue = PARA_PSUPPORTH_LIMIT;
        }
        if(Para->SettingTslopeValue > Para->SettingTiValue)
        {
            Para->SettingTslopeValue = Para->SettingTiValue - Para->SettingTiValue%10;
        }
        Para->SettingSimvRateValue = Para->SettingRateValue;
        break;
	case VS_MODE:
		//
		Para->SettingRateValue = Para->SettingVSRateValue;

		//
		if(Para->SettingVtSupportValue <Vt_LowLimit)
		{
			Para->SettingVtSupportValue = Vt_LowLimit;
		}
		if(Para->SettingVtSupportValue > Vt_HighLimit)
		{
			Para->SettingVtSupportValue = Vt_HighLimit;
		}
		if(Para->SettingVtSupportValue > 100)
		{
			Para->SettingVtSupportValue = (Para->SettingVtSupportValue / 10) * 10;
		}
		else
		{
			Para->SettingVtSupportValue = (Para->SettingVtSupportValue / 5) * 5;
		}

		break;
	case APRV_MODE:
		break;
	case BIPAP_MODE:
		Para->SettingRateValue = Para->SettingBipapRateValue;
		break;
	default:
        break;
    }
}

#define BATTERY_RATIO ((double)(0.003845215))   //详见AdcIntHandler()函数中的注释  关机电压656->10V
#define LINE_DISPNUM  (10)//每行显示数量
#define PAGE_DISPLINE (8)//每页显示行数
#define DISP_NUM_COLOR     (0xd26f00)
static void _cbDialogBatteryCalibration(WM_MESSAGE * pMsg)
{
    WM_MESSAGE Msg;
    WM_HWIN hDlg,hItem,hText0,hText1,hText2,hText3,hText4,hText5,hText6,hText7,hText8,hButton0,hButton1;
    static char *DispNum = NULL;
    static char PageNUm = 0;//0  1  2
    int Id,NCode;
    char DesStrin[30] = {0};
    GUI_COLOR FrameColor[8] = {GUI_YELLOW , GUI_CYAN ,GUI_YELLOW , GUI_CYAN ,GUI_YELLOW , GUI_CYAN ,GUI_YELLOW , GUI_CYAN };
    hDlg = pMsg->hWin;
    switch (pMsg->MsgId)
    {
    case WM_INIT_DIALOG:
		WINDOW_SetUse_3D(hDlg,1);
        MenuOperateStatus |= CALIBRATION_PRECEED;

        hText0 = WM_GetDialogItem(hDlg,GUI_ID_TEXT0);
		hText1 = WM_GetDialogItem(hDlg,GUI_ID_TEXT1);
		hText2 = WM_GetDialogItem(hDlg,GUI_ID_TEXT2);
		hText3 = WM_GetDialogItem(hDlg,GUI_ID_TEXT3);
		hText4 = WM_GetDialogItem(hDlg,GUI_ID_TEXT4);
		hText5 = WM_GetDialogItem(hDlg,GUI_ID_TEXT5);
		hText6 = WM_GetDialogItem(hDlg,GUI_ID_TEXT6);
		hButton0 = WM_GetDialogItem(hDlg,GUI_ID_BUTTON0);
		hButton1 = WM_GetDialogItem(hDlg,GUI_ID_BUTTON1);

        BUTTON_SetTextWrapMode(hButton0,GUI_WRAPMODE_WORD);
        BUTTON_SetTextWrapMode(hButton1,GUI_WRAPMODE_WORD);

		TEXT_SetText(hText0,Sys_Lib_GetDispStr(DP_BATTERYVOLTAGE)); // TEXT_SetBkColor(hText0, GUI_RED);
		TEXT_SetTextColor(hText1,DISP_NUM_COLOR);
#ifndef NEW_POWER_COMM
        TEXT_SetText(hText2,"V");
#else
		TEXT_SetText(hText2,"%");
#endif
		BUTTON_SetText(hButton0,Sys_Lib_GetDispStr(DP_EXIT));
// 		BUTTON_SetText(hButton1,System_Display[ STR_Battery_Cal ]);
		WM_HideWindow(hButton1);

// 		sprintf(DesStrin, "GUI test version: .%02d", GUIVersion.TestVersion);
// 		TEXT_SetText(hText3, DesStrin);
// 		sprintf(DesStrin, "BDU test version: .%02d", BDUVersion.TestVersion);
// 		TEXT_SetText(hText4, DesStrin);

		if(EtCO2_Config_On)
		{
			TEXT_SetText(hText5, "Option: NationalMED");
		}
		else if(EtCO2_KM7002_Config_On)
		{
			TEXT_SetText(hText5, "Option: KingST");
		}
		else if(Masimo_EtCO2_Config_On)
		{
			TEXT_SetText(hText5, "Option: Masimo IRMA");
		}
		else if(MasimoISA_EtCO2_Config_On)
		{
			TEXT_SetText(hText5, "Option: Masimo ISA");
		}
		else if(Drager_EtCO2_Config_On)
		{
			TEXT_SetText(hText5, "Option: Drager");
		}
		else
		{
			TEXT_SetText(hText5, "Option: See infomation page");
		}
        break;
    case WM_NOTIFY_PARENT:
        NCode = pMsg->Data.v;
        Id = WM_GetId(pMsg->hWinSrc);
        switch(NCode)
        {
        case WM_NOTIFICATION_CLICKED:
            switch(Id)
            {
            //在发送数据的过程中，不能进行电池校验。
            case GUI_ID_BUTTON0:
				hItem = WM_GetDialogItem(Menu_hWin.ActivedWin, GUI_ID_MULTIPAGE0);
				MULTIPAGE_SetPageChangeEnable(hItem,1);
				MenuOperateStatus &= ~(CALIBRATION_PRECEED|FRAMEWIN_POP);
				PlanMenuType = SYSTEM_SETTING_MENU;
				Task |=MEAU_UPDATE;
				break;
            case GUI_ID_BUTTON1:  //点击校验后，直接退出

				hItem = WM_GetDialogItem(Menu_hWin.ActivedWin, GUI_ID_MULTIPAGE0);
				MULTIPAGE_SetPageChangeEnable(hItem,1);
				MenuOperateStatus &= ~(CALIBRATION_PRECEED|FRAMEWIN_POP);
				PlanMenuType = SYSTEM_SETTING_MENU;
				Task |=MEAU_UPDATE;
				break;
            default:
                break;
            }
            break;
        case WM_NOTIFICATION_RELEASED:
            break;
        }
    case WM_USER:
        hText1 = WM_GetDialogItem(hDlg, GUI_ID_TEXT1);//Valtage Value
#ifndef NEW_POWER_COMM
        sprintf(DesStrin,"%2.1f",BATTERY_RATIO * (RealtimePara.BatteryVoltage));
#else
		sprintf(DesStrin,"%2.2f",RealtimePara.BatteryVoltage/100.00);
#endif
        TEXT_SetText(hText1,DesStrin);
        break;
    default:
        WM_DefaultProc(pMsg);
        break;
    }
}
WM_HWIN _BatteryCalibrationWin(void)
{
    WM_HWIN hDialog;
    hDialog = GUI_CreateDialogBox(_aDialogBatteryCalibration,
        GUI_COUNTOF(_aDialogBatteryCalibration),
        &_cbDialogBatteryCalibration, 0, 0, 0);
    return(hDialog);
}


static void _cbDialogMedibusComm(WM_MESSAGE * pMsg)
{
	WM_MESSAGE Msg;
	WM_HWIN hDlg, hItem, hButton0, hButton1, hText0, hText1, hText2;
	int Id,NCode;
	static Int8U Comm_flag = 0xFF;

	hDlg = pMsg->hWin;

	//WINDOW_SetBkColor(hDlg,GUI_RED);
	hText0 = WM_GetDialogItem(hDlg,GUI_ID_TEXT0);
	hText1 = WM_GetDialogItem(hDlg,GUI_ID_TEXT1);
	hText2 = WM_GetDialogItem(hDlg,GUI_ID_TEXT2);
	hButton0 = WM_GetDialogItem(hDlg,GUI_ID_BUTTON0);
	hButton1 = WM_GetDialogItem(hDlg,GUI_ID_BUTTON1);
	switch (pMsg->MsgId)
	{
	case WM_INIT_DIALOG:
		TEXT_SetFont(hText0,&GUI_Font_24);
		TEXT_SetText(hText0,Sys_Lib_GetDispStr(DP_MEDIBUSPROSTATRSTOP));
		//TEXT_SetText(hText2,"Medibus Communication Setting");
		BUTTON_SetText(hButton0,Sys_Lib_GetDispStr(DP_EXIT));

		if(!Medibus_GetUserStopFlag()) //允许通信
		{
			if(Medibus_GetCommActiveFlag())//通信连接
			{
				TEXT_SetText(hText1,Sys_Lib_GetDispStr(DP_MESIBUSPROACTIVE));
				BUTTON_SetPressed(hButton1,0);
				BUTTON_SetText(hButton1,Sys_Lib_GetDispStr(DP_STOP));
				WM_EnableWindow(hButton1);
			}
			else//等待通信连接
			{
				TEXT_SetText(hText1,Sys_Lib_GetDispStr(DP_WAITINGCOMMCONNEC));
				BUTTON_SetPressed(hButton1,0);
				BUTTON_SetText(hButton1,Sys_Lib_GetDispStr(DP_PLEASEWAIT));
				WM_DisableWindow(hButton1);
			}
		}
		else //停止通信
		{
			if(Medibus_GetPCResStopFlag())
			{
				TEXT_SetText(hText1,Sys_Lib_GetDispStr(DP_WAITPCRESPONSE));
				BUTTON_SetText(hButton1,Sys_Lib_GetDispStr(DP_PLEASEWAIT));
				WM_DisableWindow(hButton1);
			}
			else
			{
				TEXT_SetText(hText1,Sys_Lib_GetDispStr(DP_MEDIBUSPROSTOP));
				BUTTON_SetText(hButton1,Sys_Lib_GetDispStr(DP_START));
				WM_EnableWindow(hButton1);
			}
		}

		break;
	case WM_USER:
		if(!Medibus_GetUserStopFlag()) //允许通信
		{
			if(Medibus_GetCommActiveFlag())//通信连接
			{
				TEXT_SetText(hText1,Sys_Lib_GetDispStr(DP_MESIBUSPROACTIVE));
				BUTTON_SetPressed(hButton1,0);
				BUTTON_SetText(hButton1,Sys_Lib_GetDispStr(DP_STOP));
				WM_EnableWindow(hButton1);
			}
			else//等待通信连接
			{
				TEXT_SetText(hText1,Sys_Lib_GetDispStr(DP_WAITINGCOMMCONNEC));
				BUTTON_SetPressed(hButton1,0);
				BUTTON_SetText(hButton1,Sys_Lib_GetDispStr(DP_PLEASEWAIT));
				WM_DisableWindow(hButton1);
			}
		}
		else //停止通信
		{
			if(Medibus_GetPCResStopFlag())
			{
				TEXT_SetText(hText1,Sys_Lib_GetDispStr(DP_WAITPCRESPONSE));
				BUTTON_SetText(hButton1,Sys_Lib_GetDispStr(DP_PLEASEWAIT));
				WM_DisableWindow(hButton1);
			}
			else
			{
				TEXT_SetText(hText1,Sys_Lib_GetDispStr(DP_MEDIBUSPROSTOP));
				BUTTON_SetText(hButton1,Sys_Lib_GetDispStr(DP_START));
				WM_EnableWindow(hButton1);
			}
		}
		break;
	case WM_NOTIFY_PARENT:
		NCode = pMsg->Data.v;
		Id = WM_GetId(pMsg->hWinSrc);
		switch(NCode)
		{
		case WM_NOTIFICATION_CLICKED:
			switch(Id)
			{
			case GUI_ID_BUTTON0: //exit
				hItem = WM_GetDialogItem(Menu_hWin.ActivedWin, GUI_ID_MULTIPAGE0);
				MULTIPAGE_SetPageChangeEnable(hItem,1);
				MenuOperateStatus &= ~(CALIBRATION_PRECEED|FRAMEWIN_POP);
				PlanMenuType = SYSTEM_SETTING_MENU;
				Task |=MEAU_UPDATE;
				break;
			case GUI_ID_BUTTON1:
				Comm_flag = !Comm_flag;
				if(Comm_flag)
				{
					BUTTON_SetPressed(hButton1,0);
					if(Medibus_GetCommActiveFlag())//通信连接
					{
						TEXT_SetText(hText1,Sys_Lib_GetDispStr(DP_WAITPCRESPONSE));
						BUTTON_SetText(hButton1,Sys_Lib_GetDispStr(DP_PLEASEWAIT));
						WM_DisableWindow(hButton1);
						Medibus_SetUserStopFlag(0x01);
						RTEEPROMwrite(Medibus_COMM_ADDR,0x01);
					}
				}
				else
				{
					BUTTON_SetPressed(hButton1,0);
					if(!Medibus_GetCommActiveFlag())//通信连接
					{
						TEXT_SetText(hText1,Sys_Lib_GetDispStr(DP_WAITINGCOMMCONNEC));
						BUTTON_SetText(hButton1,Sys_Lib_GetDispStr(DP_PLEASEWAIT));
						WM_DisableWindow(hButton1);
						Medibus_SetUserStopFlag(0x00);
						RTEEPROMwrite(Medibus_COMM_ADDR,0x00);
					}
				}
				break;
			default:
				break;
			}
			break;
		case WM_NOTIFICATION_RELEASED:
			BUTTON_SetPressed(pMsg->hWinSrc,0);
			break;
		}
	default:
		WM_DefaultProc(pMsg);
		break;
	}
}
WM_HWIN _MedibusCommWin(void)
{
	WM_HWIN hDialog;
	hDialog = GUI_CreateDialogBox(_aDialogMedibusComm,
		GUI_COUNTOF(_aDialogMedibusComm),
		&_cbDialogMedibusComm, 0, 0, 0);
	return(hDialog);
}



#define DISPSTRING_NUM (16+3+1)//32->num   7->'-'   1->'\0'
#define KEYSTRING_NUM  (16)
#define DISPSTRING_DEFAULT "____-____-____-____"
/*****************************************************************
根据授权码和芯片ID计算当前的选配信息。
SrcString:16位软件授权码
keyStr：8位设备芯片ID号码
******************************************************************
*/
int GetOptional_SetFlag(char *SrcString, char *keyStr)
{
	unsigned int Optional_Flag = 0;
	char DesStrin[8] = {0};
	int ret = 0;
	DES_Decrypt_String(SrcString,keyStr,DesStrin); //des解密,得到选配数组
	Paramagnetic_O2_Flag = 0;//氧电池
	//对比包头包尾是否正确
	if((DesStrin[OPTIONAL_INDEX_HEAD]==OPTIONAL_PASSWD_HEAD)
		&&(DesStrin[OPTIONAL_INDEX_TAIL]==OPTIONAL_PASSWD_TAIL))
	{
		//筛选出选配标志,24位选配标志
		Optional_Flag = ((DesStrin[OPTIONAL_INDEX_BYTE_0]<<(OPTIONAL_INDEX_BYTE_0-1)*4)
			|(DesStrin[OPTIONAL_INDEX_BYTE_1]<<(OPTIONAL_INDEX_BYTE_1-1)*4)
			|(DesStrin[OPTIONAL_INDEX_BYTE_2]<<(OPTIONAL_INDEX_BYTE_2-1)*4) 
			|(DesStrin[OPTIONAL_INDEX_BYTE_3]<<(OPTIONAL_INDEX_BYTE_3-1)*4)
			|(DesStrin[OPTIONAL_INDEX_BYTE_4]<<(OPTIONAL_INDEX_BYTE_4-1)*4)
			|(DesStrin[OPTIONAL_INDEX_BYTE_5]<<(OPTIONAL_INDEX_BYTE_5-1)*4));
		//开通相关选配
#ifndef LANGUAGE_CHINESE_FLAG
		//PSV
		Mode_Chose_Flag = ((Optional_Flag & OPTIONAL_PSV_MODEFLAG)?1:0);
#endif
		//PRVC
		PRVCMode_Chose_Flag = ((Optional_Flag & OPTIONAL_PRVC_MODEFLAG)?1:0);
		//VS
		VS_Mode_Flag = (Optional_Flag & OPTIONAL_VS_MODEFLAG)?1:0;
		//Infant
		VT_Min_Flag = (Optional_Flag & OPTIONAL_INFANTFLAG)?1:0;
		//APRV
		APRV_Mode_Flag = (Optional_Flag & OPTIONAL_APRV_MODEFLAG)?1:0;

		FlowPause_Config_On = (Optional_Flag & OPTIONAL_FLOWPAUSE_FLAG)?1:0;
		LungRec_Config_On = (Optional_Flag & OPTIONAL_LUNGREC_FLAG)?1:0;

		//Nebulizer
		//Nebulizer_Flag = (Optional_Flag & OPTIONAL_NEBULIZERFLAG)?1:0;
		//EtCO2
#if (LOGO_FLAG == MSW_LOGO)
        if(Optional_Flag & OPTIONAL_MASIMOETCO2FLAG)//masimo AA
        {
          Masimo_EtCO2_Config_On = 1;
          EtCO2_Config_On = 0;
		  EtCO2_KM7002_Config_On = 0;
          UART1_Init(9600);

		  AA_Module_Flag.INIT_AA_MODULE_FLAG = 2; 
		  AA_ModuleInit(PhaseIn); //ylk add 20181128
        }
        else if(Optional_Flag & OPTIONAL_ETCO2FLAG)//国产CO2
        {
          EtCO2_Config_On = 1;
		  EtCO2_KM7002_Config_On = 0;
          MasimoISA_EtCO2_Config_On = 0;
          Masimo_EtCO2_Config_On = 0;
          UART1_Init(38400);
        }
		else if(Optional_Flag & OPTIONAL_KM7002_MODEFLAG)//金嘉信CO2
		{
			EtCO2_Config_On = 0;
			EtCO2_KM7002_Config_On = 1;
            Masimo_EtCO2_Config_On = 0;
            MasimoISA_EtCO2_Config_On = 0;
			UART1_Init(38400);
		}
		else
		{
			EtCO2_Config_On = 0;
			EtCO2_KM7002_Config_On = 0;
            Masimo_EtCO2_Config_On = 0;			
		}
#else
		if(Optional_Flag & OPTIONAL_MASIMOETCO2FLAG)//Masimo 和国产   互斥     有限开通masimo
		{
			EtCO2_KM7002_Config_On = 0;
			MasimoISA_EtCO2_Config_On = 0;
			Masimo_EtCO2_Config_On = 1;
			Drager_EtCO2_Config_On = 0;
			EtCO2_Config_On = 0;
            AA_Module_Flag.ETCO2_FLAG = 0;    //9600  Masimo   区分波特率
            UART2_Init(9600); 
            AA_Module_Flag.EtCO2ModuleExist = 0;
			Paramagnetic_O2_Flag = 0;

 			AA_Module_Flag.INIT_AA_MODULE_FLAG = 2; //
 			AA_ModuleInit(PhaseIn); //ylk add 20181128
		}
		else if((Optional_Flag & OPTIONAL_DRAGER_AA_FLAG)
			||(Optional_Flag & OPTIONAL_DRAGER_AA_O2_FLAG)) //ylk add 20181128
		{
			Masimo_EtCO2_Config_On = 0;
			MasimoISA_EtCO2_Config_On = 0;
			if(Optional_Flag & OPTIONAL_DRAGER_AA_FLAG)
			{
				Drager_EtCO2_Config_On = 1;
				Paramagnetic_O2_Flag = 0;//氧电池
			}
			else
			{
				Drager_EtCO2_Config_On = 2;
				Paramagnetic_O2_Flag = 1;//顺磁氧
			}
			EtCO2_Config_On = 0;
			EtCO2_KM7002_Config_On = 0;
			AA_Module_Flag.ETCO2_FLAG = 0;
			AA_Module_Flag.INIT_AA_MODULE_FLAG = 4;//用作初始化标志，如果手动选择过麻醉气体，则清零
			AA_ModuleInit(Drager);
			UART2_Init(19200);
		}
		else if(Optional_Flag & OPTIONAL_ETCO2FLAG)
		{
			EtCO2_KM7002_Config_On = 0;
			MasimoISA_EtCO2_Config_On = 0;
			Masimo_EtCO2_Config_On = 0;
			Drager_EtCO2_Config_On = 0;
			EtCO2_Config_On = 1;
            AA_Module_Flag.ETCO2_FLAG = 1;    //38400 国产
            UART2_Init(38400);
            AA_Module_Flag.EtCO2ModuleExist = 0;
			Paramagnetic_O2_Flag = 0;
		}
		else if(Optional_Flag & OPTIONAL_MASIMOISAO2FLAG)
		{
			EtCO2_KM7002_Config_On = 0;
			MasimoISA_EtCO2_Config_On = 2;
			Paramagnetic_O2_Flag = 1;//顺磁氧
			Masimo_EtCO2_Config_On = 0;
			Drager_EtCO2_Config_On = 0;
			EtCO2_Config_On = 0;
			AA_Module_Flag.ETCO2_FLAG = 0;    //9600
			UART2_Init(9600);
			AA_Module_Flag.EtCO2ModuleExist = 0;
 			AA_Module_Flag.INIT_AA_MODULE_FLAG = 2; //
 			AA_ModuleInit(PhaseIn);//ylk add 20181128
            AA_SetIRO2Value(0x08);
			AA_SetO2Option(0x01);
			AA_SetO2SensorType(Sensor_O2Spring);
		}
		else if(Optional_Flag & OPTIONAL_MASIMOISAFLAG)
		{
			EtCO2_KM7002_Config_On = 0;
			Paramagnetic_O2_Flag = 0;
			MasimoISA_EtCO2_Config_On = 1;
			Masimo_EtCO2_Config_On = 0;
			Drager_EtCO2_Config_On = 0;
			EtCO2_Config_On = 0;
			AA_Module_Flag.ETCO2_FLAG = 0;    //9600
			UART2_Init(9600);
			AA_Module_Flag.EtCO2ModuleExist = 0;

 			AA_Module_Flag.INIT_AA_MODULE_FLAG = 2; //
 			AA_ModuleInit(PhaseIn); //ylk add 20181128

			AA_SetIRO2Value(0x08);
			AA_SetO2Option(0x00);
			AA_SetO2SensorType(Sensor_O2Bat);
		}
		else if(Optional_Flag & OPTIONAL_KM7002_MODEFLAG)
		{
			MasimoISA_EtCO2_Config_On = 0;
			Masimo_EtCO2_Config_On = 0;
			Drager_EtCO2_Config_On = 0;
			EtCO2_Config_On = 0;
			AA_Module_Flag.ETCO2_FLAG = 1;    //38400 国产
			UART2_Init(38400);
			AA_Module_Flag.EtCO2ModuleExist = 0;
			Paramagnetic_O2_Flag = 0;
			EtCO2_KM7002_Config_On = 1;
		}
		else
		{
			Paramagnetic_O2_Flag = 0;
			MasimoISA_EtCO2_Config_On = 0;
			Masimo_EtCO2_Config_On = 0;
			Drager_EtCO2_Config_On = 0;
			EtCO2_Config_On = 0;
            AA_Module_Flag.EtCO2ModuleExist = 0;
			EtCO2_KM7002_Config_On = 0;

 			AA_ModuleInit(PhaseIn); //ylk add 2018128
		}
		//SpO2
#ifdef SPO2_SWITCH
		if(Optional_Flag & OPTIONAL_MASIMOSPO2FLAG)//Masimo 和国产   互斥     有限开通masimo
		{
			Masimo_SpO2_Config_On = 1;
			SpO2_Config_On = 0;
			SpO2_Choose = 2;
			UART1_Init(9600);
		}
		else if(Optional_Flag & OPTIONAL_SPO2FLAG)
		{
			Masimo_SpO2_Config_On = 0;
			SpO2_Config_On = 1;
			SpO2_Choose = 1;
			UART1_Init(4800);
		}
		else
		{
			SpO2_Choose = 0;
			Masimo_SpO2_Config_On = 0;
			SpO2_Config_On = 0;
		}
#endif
#endif

        if(Optional_Flag & OPTIONAL_DRIVERGASFLAG)//djq add 20170609 驱动气体选配
        {
            DriverGas_Chose_Flag = 1;
            Menu_hWin.InfoWin.InfoChangeFlag |= DRIVER_GAS_TYPE_FLAG;
        }
        else
        {
            DriverGas_Chose_Flag = 0;
            Menu_hWin.InfoWin.InfoChangeFlag |= DRIVER_GAS_TYPE_FLAG;
        }
		
		if(Optional_Flag & OPTIONAL_FIO2SENSOR_MODEFLAG&&!Paramagnetic_O2_Flag)//ylk add 20180314 FiO2选配
		{
			FiO2_Config_On = 1;
		}
		else
		{
			FiO2_Config_On = 0;
			O2ModuleOff = 1;//关闭氧浓度检测
		}
		ret = 1;
	}
 	else //ylk add 20181128 必须要设置默认值 
 	{
 		AA_ModuleInit(PhaseIn);     
 	}

	DEMOFLAG = 1;
    if(DEMOFLAG)
	{
		Mode_Chose_Flag = 1;
		VT_Min_Flag = 1;
		PRVCMode_Chose_Flag = 1;
		VS_Mode_Flag = 1;
		APRV_Mode_Flag = 1;
		DriverGas_Chose_Flag = 1;
		FlowPause_Config_On = 1;
		LungRec_Config_On = 1;
		//CO2
// 		EtCO2_Config_On = 1;
// 		SpO2_Config_On = 1;
		Masimo_SpO2_Config_On = 1;
		Masimo_EtCO2_Config_On = 1;
		AA_Module_Flag.PhaseInTypeModuleExist = 1;
	}

#ifdef LANGUAGE_CHINESE_FLAG
	Mode_Chose_Flag = 1;	//国标下PSV为标配
#endif
#if(LOGO_FLAG == PENLON_LOGO)//Penlon下氧浓度为标配
	FiO2_Config_On = 1;
#endif

	return ret;
}

static void _cbDialogSoftwareOptional(WM_MESSAGE * pMsg)
{
	WM_HWIN hDlg,hItem,hEdit,hText,hButton16,hButton17,hButton18;
	static Int8U DispIndex = 0;
	int Id,NCode;
	static char DispString[DISPSTRING_NUM] = {0};
	char KeyString[70] = {0},KeyStringTemp[70] = {0};//20161117 修改数组大小 防止俄语死机 
	char DesStrin[8] = {0};
	hDlg = pMsg->hWin;
	hEdit = WM_GetDialogItem(hDlg, GUI_ID_EDIT0);
	switch (pMsg->MsgId)
	{
	case WM_INIT_DIALOG:
        hButton16 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON16);
		hButton17 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON17);
		hButton18 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON18);
		strcpy(DispString,DISPSTRING_DEFAULT);
		DispIndex = 0;

		MULTIEDIT_SetFont(hEdit,&GUI_BatangChe24x24);
		MULTIEDIT_SetText(hEdit,DispString);

		hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT3);
		TEXT_SetText(hItem,DeviceSerial_String);
		hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT0);

        BUTTON_SetText(hButton16,Sys_Lib_GetDispStr(DP_EXIT));
        BUTTON_SetText(hButton17,Sys_Lib_GetDispStr(DP_ENTER));
        BUTTON_SetText(hButton18,Sys_Lib_GetDispStr(DP_BACKSPACE));
		BUTTON_SetTextWrapMode(hButton18, GUI_WRAPMODE_WORD);
        TEXT_SetText(hItem,Sys_Lib_GetDispStr(DP_INPUTPASSWORD));

		break;
	case WM_NOTIFY_PARENT:
		NCode = pMsg->Data.v;
		Id = WM_GetId(pMsg->hWinSrc);
		switch(NCode)
		{
		case WM_NOTIFICATION_CLICKED:
			hText = WM_GetDialogItem(hDlg, GUI_ID_TEXT1);
            TEXT_SetTextWrapMode(hText,GUI_WRAPMODE_WORD);//add zkq 20161117
			FunctionKeyPressedResume(hDlg,Id);

			if(Id - GUI_ID_BUTTON0 <= 0xF)
			{
				TEXT_SetText(hText,"");
				if(((DispIndex+1)%5) == 0)//跳过‘-’
				{
					DispIndex++;
				}
				if(DispIndex < DISPSTRING_NUM - 1)
				{
					DispString[DispIndex++] = HexTable[Id - GUI_ID_BUTTON0];
				}
			}
			else
			{
				switch(Id)
				{
				case GUI_ID_BUTTON16://Exit
					hItem = WM_GetDialogItem(Menu_hWin.ActivedWin, GUI_ID_MULTIPAGE0);
					MULTIPAGE_SetPageChangeEnable(hItem,1);
					MenuOperateStatus &= ~FRAMEWIN_POP;
					PlanMenuType = SYSTEM_SETTING_MENU;
					Task |=MEAU_UPDATE;
					break;
				case GUI_ID_BUTTON17://Enter
					hText = WM_GetDialogItem(hDlg, GUI_ID_TEXT1);
					hItem = WM_GetDialogItem(Menu_hWin.ActivedWin, GUI_ID_MULTIPAGE0);
					if(DispIndex < DISPSTRING_NUM - 1)//密码不足位数
					{

                            strcpy(KeyStringTemp,Sys_Lib_GetDispStr(DP_INPUTCOMPLETEPASSWORD));

					}
					else//识别密码
					{
						char index = 0,cnt = 0,tmp = 0,num = 0;
						for(cnt = 0,index = 0;index < KEYSTRING_NUM;cnt++,index++)
						{                     
							if(DispString[cnt] == '-')
							{
								cnt++;
							}
							switch(DispString[cnt])
							{
							case '0':
							case '1':
							case '2':
							case '3':
							case '4':
							case '5':
							case '6':
							case '7':
							case '8':
							case '9':
								tmp = DispString[cnt] - '0';
								break;
							case 'A':
							case 'B':
							case 'C':
							case 'D':
							case 'E':
							case 'F':
								tmp = DispString[cnt] - 'A' + 10;
								break;
							default:
								break;
							}  
							if(index%2)
							{
								KeyString[num++] |= (tmp&0xf);
							}
							else
							{
								KeyString[num] = tmp<<4;
							}
						}
					    memcpy(KeyStringTemp,KeyString,16);
						if(GetOptional_SetFlag(KeyStringTemp,DeviceSerial_Table))//密码正确
						{

							strcpy(KeyStringTemp,Sys_Lib_GetDispStr(DP_PASSWORDCORRECT));

							for(index = 0;index < 8;index++)//保存当前输入的软件授权码
							{
								RTEEPROMwrite(OPTIONAL_0_ADDR+index,KeyString[index]);  //
								RTEEPROMwrite(OPTIONAL_0_ADDR_BACK+index,KeyString[index]);
							}
						}
						else//密码错误
						{
							strcpy(KeyStringTemp,Sys_Lib_GetDispStr(DP_PASSWORDINCORRECT));
						}
					}

					//alarm_str_init(LanguageType); 
                    //SYS_str_init();

					InitMonitorRank_Auto(1);//  [2/17/2022 yaoyao add]更新工作下的界面布局配置//解决：进行选配后没有进行关机开机操作时，进入工作界面，呈现上一次选配结果的界面布局 	
                    VentParaWinInit(Menu_hWin.bWin,&BeathSettingPara); 
					TEXT_SetText(hText,KeyStringTemp);
					strcpy(DispString,DISPSTRING_DEFAULT);
					DispIndex = 0;
					break;
				case GUI_ID_BUTTON18://Backspace
					if(DispIndex > 0)
					{
						DispIndex--;
					}
					if(((DispIndex+1)%5) == 0)//跳过‘-’
					{
						DispIndex--;
					}
					DispString[DispIndex] = '_';
					break;
				default:
					break;
				}
			}
			MULTIEDIT_SetText(hEdit,DispString);
			break;
		case WM_NOTIFICATION_RELEASED:
			BUTTON_SetPressed(pMsg->hWinSrc,0);
			break;
		}
	default:
		WM_DefaultProc(pMsg);
		break;
	}
}
WM_HWIN _SoftwareOptionalWin(void)
{
    WM_HWIN hDialog;
    hDialog = GUI_CreateDialogBox(_aDialogSoftwareOptional,
        GUI_COUNTOF(_aDialogSoftwareOptional),
        &_cbDialogSoftwareOptional, 0, 0, 0);
    return(hDialog);
}
static const GUI_WIDGET_CREATE_INFO _aDialogCreate25[] =  //sun add 20140708
{
    { WINDOW_CreateIndirect,    "",       0,           345,  192, 270, 160, FRAMEWIN_CF_ACTIVE },
    { TEXT_CreateIndirect,      "",     GUI_ID_TEXT0,   20,  20, 250,  60, TEXT_CF_LEFT|TEXT_CF_VCENTER},
    { BUTTON_CreateIndirect,    "Enter",   GUI_ID_BUTTON0,   175,  115,  75,  30, 0},
    { BUTTON_CreateIndirect,    "Exit",   GUI_ID_BUTTON1,  20,  115,  75,  30, 0},
};
/***************************************************************************************************
*
*
*/
static void _cbDialog25(WM_MESSAGE * pMsg)  //sun add 20140708
{
    WM_HWIN hDlg,hItem0,hItem1,hItem2;
    WM_HWIN hItem,hwin,hwinpara,hText;
    int NCode, Id;
    unsigned char flag = 0;
    hDlg = pMsg->hWin;
    switch (pMsg->MsgId)
    {
    case WM_INIT_DIALOG:
        WINDOW_SetUse_3D(hDlg,1);
        hItem0 = WM_GetDialogItem(hDlg, GUI_ID_TEXT0);
        hItem1 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON0);
        hItem2 = WM_GetDialogItem(hDlg, GUI_ID_BUTTON1);
        
        BUTTON_SetText(hItem1,Sys_Lib_GetDispStr(DP_ENTER));
        BUTTON_SetText(hItem2,Sys_Lib_GetDispStr(DP_EXIT));
		if(MasimoISA_EtCO2_Config_On)
		{
			flag = 0;
			//TEXT_SetText(hItem0,System_Display[ STR_Iso_gas_moudle ]);
			TEXT_SetText(hItem0,Sys_Lib_GetDispStr(DP_RMGAS));
			TEXT_SetTextWrapMode(hItem0, GUI_WRAPMODE_WORD);
			WM_EnableWindow(hItem1);
		}
		else
		{
//			if((pAll_Data->slow.GenVal.zero_disab == 1)||(pAll_Data->slow.GenVal.zero_inprogress != 0))
			if((AA_all_data.AA_state.AA_ModuleState.zero_state == disable) ||(AA_all_data.AA_state.AA_ModuleState.zero_state == zero_zeroing))  //sun add 20140708
			{
				flag = 1;
				TEXT_SetText(hItem0,Sys_Lib_GetDispStr(DP_MAKESUREGASMODULEIS));
				WM_DisableWindow(hItem1);
			}
			else
			{
				flag = 0;
				//TEXT_SetText(hItem0,System_Display[ STR_Iso_gas_moudle ]);
				TEXT_SetText(hItem0,Sys_Lib_GetDispStr(DP_RMGAS));
				TEXT_SetTextWrapMode(hItem0, GUI_WRAPMODE_WORD);
				WM_EnableWindow(hItem1);
			}        
		}
        break;
    case WM_NOTIFY_PARENT:
        NCode = pMsg->Data.v;
        Id = WM_GetId(pMsg->hWinSrc);
        switch(NCode)
        {
        case WM_NOTIFICATION_CLICKED:
            switch(Id)
            {
            case GUI_ID_BUTTON0:
                if(flag == 1)
                {
                    hItem = WM_GetDialogItem(Menu_hWin.ActivedWin,GUI_ID_MULTIPAGE0);
                    hwinpara = MULTIPAGE_GetWindow(hItem,3);
                    WM_DeleteWindow(Menu_hWin.RecordWin);
                    Menu_hWin.RecordWin = 0;
                    hwin = WM_GetDialogItem(hwinpara, GUI_ID_BUTTON4);
                    BUTTON_SetPressed(hwin,0);
                    hwin = WM_GetDialogItem(hwinpara, GUI_ID_BUTTON5);
                    BUTTON_SetPressed(hwin,0);
					PlanMenuType = SYSTEM_SETTING_MENU;
                    Task |=MEAU_UPDATE;
                    MenuOperateStatus&=~FRAMEWIN_POP;
                    MULTIPAGE_SetPageChangeEnable(hItem,1);
                }
                else
                {
                    hwinpara = WM_GetDialogItem(Menu_hWin.ActivedWin,GUI_ID_MULTIPAGE0);
                    hwinpara = MULTIPAGE_GetWindow(hwinpara,3);
                    MenuOperateStatus |= CALIBRATION_PRECEED;
					if((Masimo_EtCO2_Config_On )&& (AA_Module_Flag.EtCO2ModuleExist == 2))//masimo IRMA
					{
						DataTransmit(0x02,IM_SET_MODE,Usart2_Tx_Buffer,USART2);//設置phasein運行模式為测量模式 sy add 20140604
						DataTransmit(IM_CAL_NORM,IM_ZERO_CAL,Usart2_Tx_Buffer,USART2);//執行歸零指令
					}

                    CalibrationType = PHASEIN_ZERO_CALIBRATION;
                    hText = WM_GetDialogItem(hwinpara, GUI_ID_TEXT3);
                    hwin=WM_GetDialogItem(hwinpara, GUI_ID_PROGBAR0);
                    WM_ShowWindow(hwin);

                    PROGBAR_SetText(hwin,Sys_Lib_GetDispStr(DP_CALIBRATING));

                    WM_DeleteWindow(Menu_hWin.RecordWin);
                    Menu_hWin.RecordWin = 0;
                    MenuOperateStatus&=~FRAMEWIN_POP;
                }
                break;  
            case GUI_ID_BUTTON1:
				//退出校验界面后发送关泵命令
// 				if((MasimoISA_EtCO2_Config_On == 2) && ((AA_Module_Flag.EtCO2ModuleExist&0x04) == 4)) //ylk modify 20181220

				if( (Drager_EtCO2_Config_On && AA_all_data.AA_state.AA_ModuleState.pump_state == AA_pump_high)\
					|| ((MasimoISA_EtCO2_Config_On || Masimo_EtCO2_Config_On) && AA_Module_Flag.PhaseInTypeModuleExist))
				{
					SendOperationMessage(SET_AA_PUMPOFF,USART2);
				}
                hItem = WM_GetDialogItem(Menu_hWin.ActivedWin,GUI_ID_MULTIPAGE0);
                hwinpara = MULTIPAGE_GetWindow(hItem,3);
                WM_DeleteWindow(Menu_hWin.RecordWin);
                Menu_hWin.RecordWin = 0;
                hwin = WM_GetDialogItem(hwinpara, GUI_ID_BUTTON4);
                BUTTON_SetPressed(hwin,0);
                hwin = WM_GetDialogItem(hwinpara, GUI_ID_BUTTON5);
                BUTTON_SetPressed(hwin,0);
				PlanMenuType = SYSTEM_SETTING_MENU;
                Task |=MEAU_UPDATE;
                MenuOperateStatus&=~FRAMEWIN_POP;
                MULTIPAGE_SetPageChangeEnable(hItem,1);
                break;
            }
            break;
        }
        break;
    default:
        WM_DefaultProc(pMsg);
        break;
    }
}
/***************************************************************************************************
*
*Phasein 模块校零
*
*/
WM_HWIN PhaseinZeroCail(void)
{
    WM_HWIN hDialog;
    hDialog = GUI_CreateDialogBox(_aDialogCreate25,
                                  GUI_COUNTOF(_aDialogCreate25),
                                  &_cbDialog25, 0, 0, 0);
    return(hDialog);
}
/***************************************************************************************************/

static void _cbDialogAlarmInfoWindow(WM_MESSAGE * pMsg)
{
	WM_HWIN hDlg,hItem;
	static char Paint = 0;
	int NCode, Id;
	OS_PRIO prio;
	hDlg = pMsg->hWin;
	switch (pMsg->MsgId)
	{
	case WM_INIT_DIALOG:
// 		WINDOW_SetBkColor(hDlg,GUI_GREEN);		
// 		hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT1);
// 		TEXT_SetTextColor(hItem,GUI_WHITE);
// 		TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
// 		TEXT_SetTextWrapMode(hItem, GUI_WRAPMODE_WORD);
		break;
	case WM_NOTIFY_PARENT://ylk add 20190126 点报警弹出报警列表
		NCode = pMsg->Data.v;
		Id = WM_GetId(pMsg->hWinSrc);   
		switch(NCode)
		{
		case WM_NOTIFICATION_CLICKED:
		case WM_NOTIFICATION_GOT_FOCUS:
			if(!(Task & MEAU_UPDATE)&&(MenuType != ALARM_MENU)&&(AlarmsWork.presentAlarms||AlarmsWork.presentAlarms1)
				&&!(MenuOperateStatus & CALIBRATION_PRECEED)
				&&!(MenuOperateStatus&FRAMEWIN_POP)&&(!CHECK_DEBUG))
			{
				PlanMenuType = ALARM_MENU;
				Task |= MEAU_UPDATE;
				FunctionKeyPressedResume(Menu_hWin.hWin,GUI_ID_BUTTON0);//弹起菜单按钮
				BUTTON_SetPressed(WM_GetDialogItem(Menu_hWin.hWin, GUI_ID_BUTTON0),1);  
				AlarmEnterFlag = 1;
			}
			break;
		}
		break;
	case WM_USER:
// 		hItem = WM_GetDialogItem(hDlg, GUI_ID_TEXT1);
// 		prio = HomePrompt_GetHighestPrio();
// 		TEXT_SetText(hItem,HomePrompt_String);
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}
WM_HWIN _AlarmInfoCreat(void)
{
	WM_HWIN hWin;
	hWin = GUI_CreateDialogBox(_aDialogCreateAlarmInfo,
		GUI_COUNTOF(_aDialogCreateAlarmInfo),
		&_cbDialogAlarmInfoWindow, 0, 0, 0);
	return hWin;
}

// extern GUI_CONST_STORAGE GUI_BITMAP bmCPB_ImageInfo_CN;
// extern GUI_CONST_STORAGE GUI_BITMAP bmCPB_ImageInfo_EN;

// static void _cbDialogCPBInfoWindow(WM_MESSAGE * pMsg)
// {
// 	WM_HWIN hDlg,hItem;
// 	int NCode, Id;
// 	hDlg = pMsg->hWin;
// 	switch (pMsg->MsgId)
// 	{
// 	case WM_INIT_DIALOG:
// 		WINDOW_SetBkColor(hDlg,GUI_BLACK);		
// 		if(LanguageType == Disp_Lg_Chinese)
// 		{
// 			MultiText_EX_SetBitmapEx(WM_GetDialogItem(hDlg, GUI_ID_TEXT0), &bmCPB_ImageInfo_CN, 0, 0);
// 		}
// 		else
// 		{
// 			MultiText_EX_SetBitmapEx(WM_GetDialogItem(hDlg, GUI_ID_TEXT0), &bmCPB_ImageInfo_EN, 0, 0);
// 		}
// 		
// 		break;
// 	default:
// 		WM_DefaultProc(pMsg);
// 	}
// }
// WM_HWIN _CPBInfoCreat(void)
// {
// 	WM_HWIN hWin;
// 	hWin = GUI_CreateDialogBox(_aDialogCreateCPBInfo,
// 		GUI_COUNTOF(_aDialogCreateCPBInfo),
// 		&_cbDialogCPBInfoWindow, 0, 0, 0);
// 	return hWin;
// }

static void _cbDialogSoftwareVersion(WM_MESSAGE *pMsg)
{
	WM_HWIN hDlg,hItem;
	int Id, NCode;
	char strVer[30] = {0};
	hDlg = pMsg->hWin;

	switch (pMsg->MsgId)
	{
	case WM_INIT_DIALOG:
		hItem = WM_GetDialogItem(hDlg,GUI_ID_BUTTON0);
		BUTTON_SetText(hItem,Sys_Lib_GetDispStr(DP_EXIT));
		//GUI
		hItem = WM_GetDialogItem(hDlg,GUI_ID_TEXT0);
		sprintf(strVer, "%s:\t%01d\.%02d\.%02d\.%02d", Sys_Lib_GetDispStr(DP_GUI),GUIVersion.MainVersion,GUIVersion.SubVersion,GUIVersion.DebugVersion,GUIVersion.TestVersion);
		TEXT_SetText(hItem, strVer);
		//BDU
		hItem = WM_GetDialogItem(hDlg,GUI_ID_TEXT1);
		sprintf(strVer, "%s:\t%01d\.%02d\.%02d\.%02d", Sys_Lib_GetDispStr(DP_BDU),BDUVersion.MainVersion,BDUVersion.SubVersion,BDUVersion.DebugVersion,BDUVersion.TestVersion);
		TEXT_SetText(hItem, strVer);
		//PMU
		hItem = WM_GetDialogItem(hDlg,GUI_ID_TEXT2);
		sprintf(strVer, "%s:\t%01d\.%02d\.%02d\.%02d", Sys_Lib_GetDispStr(DP_PMU),PMUVersion.MainVersion,PMUVersion.SubVersion,PMUVersion.DebugVersion,PMUVersion.TestVersion);
		TEXT_SetText(hItem, strVer);
// 		//EGM/EFM
// 		hItem = WM_GetDialogItem(hDlg,GUI_ID_TEXT3);
// 		sprintf(strVer, "%s:\t%01d\.%02d\.%02d\.%02d", Sys_Lib_GetDispStr(DP_EGM),EFMVersion.MainVersion,GUIVersion.SubVersion,GUIVersion.DebugVersion,GUIVersion.TestVersion);
// 		TEXT_SetText(hItem, strVer);
		//Lang
		hItem = WM_GetDialogItem(hDlg,GUI_ID_TEXT4);
#ifdef __ICCARM__
		sprintf(strVer, "%s:\t%01d\.%02d\.%02d\.%02d", Sys_Lib_GetDispStr(DP_LANG),			
			*((unsigned char *)STR_BASE_ADDR + 0),
			*((unsigned char *)STR_BASE_ADDR + 1),
			*((unsigned char *)STR_BASE_ADDR + 2),
			*((unsigned char *)STR_BASE_ADDR + 3));

#else
		sprintf(strVer,"%s:\t%01d.%02d.%02d",  Sys_Lib_GetDispStr(DP_LANG),	1 , 0, 0);
#endif
		TEXT_SetText(hItem, strVer);
		//Font
		hItem = WM_GetDialogItem(hDlg,GUI_ID_TEXT5);
#ifndef LANGUAGE_CHINESE_FLAG
#ifdef __ICCARM__
		sprintf(strVer, "%s:\t%01d\.%02d\.%02d\.%02d", Sys_Lib_GetDispStr(DP_FONT),			
			*((unsigned char *)FONT_SIF_BASE_ADDR + 0),
			*((unsigned char *)FONT_SIF_BASE_ADDR + 1),
			*((unsigned char *)FONT_SIF_BASE_ADDR + 2),
			*((unsigned char *)FONT_SIF_BASE_ADDR + 3));
#else
		sprintf(strVer,"%s:\t%01d.%02d.%02d",  Sys_Lib_GetDispStr(DP_FONT),1 , 0, 0);
#endif
		TEXT_SetText(hItem, strVer);
#endif
		//PM
		hItem = WM_GetDialogItem(hDlg,GUI_ID_TEXT6);
		sprintf(strVer, "%s:\t%01d\.%02d\.%02d\.%02d", Sys_Lib_GetDispStr(DP_PM),PM_Version_Main,PM_Version_Sub,PM_Version_Debug,PM_Version_Test);
		TEXT_SetText(hItem, strVer);

		break;
	case WM_NOTIFY_PARENT:
		NCode = pMsg->Data.v;
		Id = WM_GetId(pMsg->hWinSrc);
		switch (NCode)
		{
		case WM_NOTIFICATION_CLICKED:
			switch (Id)
			{
			case GUI_ID_BUTTON0:
				hItem = WM_GetDialogItem(Menu_hWin.ActivedWin, GUI_ID_MULTIPAGE0);
				MULTIPAGE_SetPageChangeEnable(hItem,1);
				MenuOperateStatus &= ~FRAMEWIN_POP;
				PlanMenuType = SYSTEM_SETTING_MENU;
				Task |=MEAU_UPDATE;
				break;
			}
			break;
		}
		break;
	default:
		WM_DefaultProc(pMsg);
		break;
	}
}
WM_HWIN _SoftwareVersionWin(void)
{
	WM_HWIN hDlg;
	hDlg = GUI_CreateDialogBox(_aDialogSoftwareVersion,
		GUI_COUNTOF(_aDialogSoftwareVersion),
		&_cbDialogSoftwareVersion, 0, 0, 0);
	return(hDlg);
}

static void _cbDialogShutDown_Ask(WM_MESSAGE * pMsg)
{
	WM_HWIN hDlg,hItem;
	int Id,NCode;
	char Value[5];
	hDlg = pMsg->hWin;
	switch (pMsg->MsgId)
	{
	case WM_INIT_DIALOG:
		WINDOW_SetBkColor(hDlg,0xFADCD6/*WINDOW_BK_COLOR*/);
// 		WINDOW_SetFrameFlag(hDlg,2);
		AskTimer = 7;

		hItem = WM_GetDialogItem(hDlg,GUI_ID_TEXT0);
		TEXT_SetFont(hItem,&GUI_Font_32);
		TEXT_SetText(hItem,"Countdown to shutdown!");
		hItem = WM_GetDialogItem(hDlg,GUI_ID_TEXT1);
		TEXT_SetFont(hItem,&GUI_Font32B_ASCII);
		TEXT_SetTextAlign(hItem,GUI_TA_RIGHT|GUI_TA_BOTTOM);
		sprintf(Value,"%d",AskTimer-2);
		TEXT_SetText(hItem,Value);
		hItem = WM_GetDialogItem(hDlg,GUI_ID_TEXT2);
		TEXT_SetTextAlign(hItem,GUI_TA_LEFT|GUI_TA_BOTTOM);
		TEXT_SetText(hItem,Sys_Lib_GetDispStr(DP_S));
		TEXT_SetFont(hItem,&GUI_Font_32);
		break;
	case WM_USER:
		hItem = WM_GetDialogItem(hDlg,GUI_ID_TEXT1);
		sprintf(Value,"%d",AskTimer-2);
		TEXT_SetText(hItem,Value);
		if (AskTimer<=2)
		{
			SendShutDownState = SHUT_DOWN;
			ShutDownAskMenu_Flag = 0;
			Task |= MEAU_UPDATE;
		}
		break;
	default:
		WM_DefaultProc(pMsg);
		break;
	}
}
WM_HWIN _ShutDown_AskWin(void)
{
	WM_HWIN hDlg;
	hDlg = GUI_CreateDialogBox(_aDialogShutDown_Ask,
		GUI_COUNTOF(_aDialogShutDown_Ask),
		&_cbDialogShutDown_Ask,0,0,0);
	return (hDlg);
}
