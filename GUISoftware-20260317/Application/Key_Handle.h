#ifndef Key_Handle_H
#define Key_Handle_H
#include "ButtonExCtrl.h"
#include "WM.h"
#include "GUI.H"
#include "MainTask.h"
//窗口
#define MODES_MENU                0x01
#define MONITOR_MENU              0x02
#define ALARM_MENU                0x03
#define STANDBY_MENU              0x04
#define SYSTEM_SETTING_MENU       0x05
#define TREND_MENU                0x06//sun add 20140514
#define CALIBRATION_MENU          0x07
#define WAVEFORM_SETTING_MENU     0x08
#define MAIN_MENU                 0x09
#define DATE_SETTING_MENU         0x0A
#define SYSTEM_INFO_MENU          0x0B
#define PRESSURE_CALI_MENU        0x0C
#define FLOW_CALI_MENU            0x0D
#define OXY_SENSOR_CALI_MENU      0x0E
#define ENGINEER_MENU             0x0F
#define ENGINEER_CALIBRATION_MENU 0x10
#define ACTIVATE_STANDBY_MENU     0x11
#define OXYGEN_SENSOR_CAL_MENU    0x12
#define ALARM_RECORD_MENU         0x13
#define UPDATE_PROGRAM_MODE       0x14  //sun add 20140514
#define OXY_CHOOSE_MENU           0x15  //sun add 20140529
#define BATTERY_CALIBRATION_MENU  0x16 //电池校验界面
#define SOFTWART_OPTIONAL_MENU    0x17
#define SELFCHECK_MENU            0x18 //开机自检
#define SELFCHECK_ASK_MENU        0x19 //开机自检询问
#define TOUCH_CALIBRATION_MENU    0x1A//触摸屏校验
#define PHASEIN_ZERO_CAL_MENU     0x1B//phasein校零

#define FONT_VERSION_DISPLAY_MENU 0X1C//字库版本显示菜单
#define MEDIBUS_COMM_MENU		  0x1D //medibus协议开始和停止
#define PATIENT_MENU              0x1F //Patient
#define PATIENT_DATA_ASK_MENU     0x20 //Patient
#define TOOLS_MENU				  0x21 
#define CPBINFO_MENU			  0x22 
#define SOFTWARE_VERSION_MENU 0x1E

//BDU命令码
#define EXIT_STANDBY              0x05
#define ENTER_STANDBY             0x0A
//
#define TRIGGER_UPDATE            0x0F
#define SIGH_TIMES_UPDATE         0x13
//
#define PRESSURE_CALIBRATION      0x14
#define FLOW_CALIBRATION          0x19
#define OXY_CALIBRATION           0x1E
//
#define EXP_VALVE_CALIBRATION           0x23
#define INSP_VALVE_CALIBRATION_FACTORY  0x28
#define EXP_FLOW_SENSOR_CALIBRATION     0x2D
//
#define AIR_FAILURE_INFORM              0x32
#define OXY_FAILURE_INFORM              0x37
#define APNEA_INFORM                    0x3C
#define OXY_NORMAL_INFORM               0x41
#define AIR_NORMAL_INFORM               0x46
#define SEND_PH_LIMIT                   0x4B
#define INSP_HOLD_START                 0x50
#define INSP_HOLD_END                   0x55
#define EXP_HOLD_START                  0x5A
#define EXP_HOLD_END                    0x5F
#define SUCTION_ACTIVE                  0x6E
#define SUCTION_CANCEL                  0x73
#define MANUAL_INSP_START               0x78
#define BAROMETER_SEND                  0xE3   //djq add 20140305 大气压力传感器关闭，大气压力传感器开启
#define BAROMETER_STAND_SEND            0xE5   //校验地大气压力值发送
#define ASK_VERSIONFOR_BDU_COM          0x10   //向BDU请求版本号
#define PHASEIN_ZERO_CALIBRATION        0x18  //sun add 20151103  //PhaseIn零点校验
#define DIRVER_GAS_FOR_BDU_COM          0x1A  //向BDU发送驱动气体类型

#define NPLSWITCH_SEND_COM                  0x8F	//NIF P01 肺复张
#define LUNGPAWTIME_SEND_COM                0x8E	//发送肺复张压力和时间设定值

//
#define                 USART0                          0x55
#define                 USART1                          0x56 //  20130325
#define                 USART2                          0x57 //  20130325
#define                 USART3                          0x5d
#define                 EXT_UART1                       0xAA
#define                 EXT_UART2                       0xAD
//
#define GUI_KEY_RETURN            GUI_KEY_ESCAPE
#define GUI_KEY_MENU              GUI_KEY_BACKSPACE
#define GUI_KEY_INSP_HOLD         GUI_KEY_INSERT
#define GUI_KEY_EXP_HOLD          GUI_KEY_DELETE
#define GUI_KEY_RETURN_HOME       GUI_KEY_HOME
#define GUI_KEY_MANUAL_INSP       GUI_KEY_END
#define GUI_KEY_FREEZE            GUI_KEY_F1
#define GUI_KEY_LOCK              GUI_KEY_F2 //GUI_KEY_SPACE //cpp md 20130325
#define GUI_KEY_STANDBY           GUI_KEY_SPACE //GUI_KEY_F2 //cpp md 20130325 
#define GUI_KEY_SILENCE           GUI_KEY_SHIFT
#define GUI_KEY_ALARM_RESET       GUI_KEY_CONTROL
//
#define ERR_KEY   50
#define GUI_KEY_INSP_HOLD_RELEASED 0xD0
#define GUI_KEY_EXP_HOLD_RELEASED  0xD1
#define GUI_KEY_NEBULIZER          GUI_KEY_TAB
#define LISTBOX_EXPAND             0x01
#define CALIBRATION_PRECEED        0x2
#define FRAMEWIN_POP               0x4
//
typedef struct
{
    int MsgType;
    int KeyValue;
} KEY_MSG_DATA;
//
typedef struct
{
    unsigned char Key_Lock_Flag;
    unsigned char Key_Silence_flag;
    unsigned char Key_Menu_flag;
    unsigned char Key_Freeze_flag;
    unsigned char Key_Run_Flag;
    unsigned char Key_O2_Suction_flag;
} KEY_MSG;
//
void Knob_Handle(int KeyValue);
void Function_Key_Handle(int KeyValue);
#endif
