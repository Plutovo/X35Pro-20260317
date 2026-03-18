#include "MainTask.h"
#include "arm_comm.h"
#include "Communication_Handle.h"
#include "Static_Menu_Creat.h"
#include "iolpc1788 and 2478.h"
#include "Alarm_handle.h"
#include "CO2_IRMA.h"
#include "JM1_SpO2.h" //  20130408
#include "C002_CO2.h" //  20130328
#include "AddFunETCO2.H"
#include "MasimoSPO2.h"
#include "AA_Module.h"
#include "WIDGET_Tools.h"
#include "Demo_AlarmTest.h"

INT8U   USART0_TXD_Busy_FLAG = FALSE;
INT8U   USART1_TXD_Busy_FLAG = FALSE;
INT8U   USART2_TXD_Busy_FLAG = FALSE; //  20130402
INT8U   USART3_TXD_Busy_FLAG = FALSE;
//
INT8U   Usart0_Tx_Buffer[4] = {0};
#if (LOGO_FLAG == MSW_LOGO)
INT8U   Usart1_Tx_Buffer[8] = {0};
#else
INT8U   Usart1_Tx_Buffer[4] = {0};
#endif
INT8U   Usart2_Tx_Buffer[20] = {0};
INT8U   Usart3_Tx_Buffer[4] = {0};

unsigned char   Usart4_Tx_Buffer[10];
//
INT64U  USART0_TXD_RIGSTER = 0; //INT32U cpp md 20130402
INT8U   USART1_TXD_RIGSTER = 0;
INT16U   USART2_TXD_RIGSTER = 0; //  20130402
INT16U  USART3_TXD_RIGSTER = 0;
INT64U  USART0_CHECK_RIGSTER = 0;//djq add 20140312  重复发送命令标志寄存器

INT8U ModeChange_Flag = 0;
unsigned int ETCO2_ConnFlag = 0;
unsigned char butter_press_count;
extern unsigned char Picture_Load_Complete;
//sun add 20140514 软件升级
extern char UpdateSend_Flag;//它的高4位代表是否发送命令：0b1010表示需要发送升级握手命令到相应的下位机，0b1001表示发送完成，0b0000表示不发送。UpdateSend_Flag的低4位表示要发送的命令：从低位到高位依次表示GUI、BDU和PMU。
extern char UpdateRecv_Flag;//它的高4位代表是否接受完成（或者超时）：0b1010表示正在接受（或者超时），0b1001表示接受完成（如果），0b0000表示没有进入握手程序。
extern char BDU_UpdateFlag;//用于发送命令时的数据位，状态为0x55 升级开始；0xaa 升级取消；0 初始状态
extern char PMU_UpdateFlag;//用于发送命令时的数据位，状态为0x55 升级开始；0xaa 升级取消；0 初始状态
extern unsigned char AssistShowFlag;//肺触发显示标志
extern unsigned char DirverGasFlag;//驱动类型标志
//extern int BreathMode_BeforeACGO;
//extern int BreathMode_BeforeManual;
extern Int16U LowAlarmPriority;//sun add 20141224
extern Int8U CHECK_DEBUG;
extern unsigned char EtCO2_Config_On;
extern unsigned char EtCO2_KM7002_Config_On;
extern unsigned char Masimo_EtCO2_Config_On ;
extern unsigned char alarm_ACGO_Flag;
//extern unsigned int LanguageType;
extern ALARM_SAVEVENT_PARA Alarm_SaveVent_para;
extern BREATH_SET_PARA Breath_Setting_Tmp;
extern unsigned char PresetMode_Flag;
extern char ModeChange_Count;
extern ButtonExCtrl_Handle BreathSettingParaHandle[8];
extern unsigned char AcgoTimeFlag;
extern unsigned char AcgoPromptFlag;
//extern AA_MODULE_FLAG_REG AA_Module_Flag;
//
const INT16U DeviceKeyTable[] =
{
    0x7F,
    0x78,0x72,0x71,0x70,
    0x6A,0x69,0x68,0x62,
    0x61,0x60,
    0x55,0xdd,
    0x2A,0x32,
    0
};
const INT16U GUIKeyTable[]=
{
    GUI_KEY_ENTER,
    GUI_KEY_MANUAL_INSP, GUI_KEY_INSP_HOLD,   GUI_KEY_NEBULIZER,  GUI_KEY_FREEZE,
    GUI_KEY_EXP_HOLD,    GUI_KEY_RETURN_HOME, GUI_KEY_ALARM_RESET,GUI_KEY_LOCK,
    GUI_KEY_STANDBY,     GUI_KEY_SILENCE,
    GUI_KEY_RIGHT,       GUI_KEY_LEFT,
    GUI_KEY_EXP_HOLD_RELEASED,GUI_KEY_INSP_HOLD_RELEASED,
    ERR_KEY
};
extern MONITOR_PARA MonitorPara;
extern REALTIME_PARA RealtimePara;
extern BREATH_SET_PARA BeathSettingPara;
extern ALARM_SET_PARA AlarmSettingPara;
extern INT16U  AlarmPriority;
extern INT16U  LedUpdateRegister;

extern void DataTransmit(INT16U data,INT8U type,INT8U * Pointer,INT8U UsartPort);

extern INT16U InspHoldState;
extern INT16U ExpHoldState;
extern INT16U SuctionState;
extern int StandbyState;
extern INT16U GasSourceState;
extern int BackUpMode;
extern int BuzzerTestVolume; //  20130401

INT16U DeviceKeyToGUIKey(INT16U key);
void Uart3IntHandler(void);
void Uart4IntHandler(void);


extern int Task; //  20130606
extern int PlanMenuType; //  20130606
extern int MenuType; //  20130606
extern int StandbyState; //  20130606

extern MENU_HWIN Menu_hWin;
extern ALARM_SET_PARA AlarmSettingPara;
extern VERSION_PARA GUIVersion,BDUVersion,PMUVersion; //  20130328
extern ALARM_WORK AlarmsWork;
extern int InspFlagForCurve;
extern int InspFlagForAlarm;
extern int CalibrateFinishedFlag;
//extern int AcgoModeFlag; //  20130415
//extern int ManualModeFlag;
extern int TriggerHappenFlag;
extern unsigned char InspFlag;
extern const int IE_RatioTable[];//  20130328
extern JM1_SPO2_PARAMETERS SpO2_Parameters;//  20130408
extern C002_CO2_PARAMETERS CO2_Parameters;//  20130328
extern unsigned char  SpO2ModuleExist;
extern char HeartBeatFlag;

//djq add 20140305 大气压力
extern int Barameter_Pressure;         //设置信息界面，大气压力值   0~999mmHg
extern int Barameter_Stand_Pressure;   //校验地大气压力值   0~999mmHg
extern int Barameter_Switch;           //0 ：大气压传感器开，显示为BDU上传的值   1：关，显示为设置值。

int alarm_Absorber_Falg = 0;  //sun add 20140710
extern char oxyChooseFlag;  //sun add 20140529   //modify 20180530 int -> char
extern int TouchScreenCalibrate;//用于判断是否在触摸屏校验窗口
extern unsigned char Masimo_SpO2_Config_On; //0:关闭   1：开启
extern unsigned char SpO2_Choose;//1：国产   2：Masimo
extern signed char *Masimo_Buffer;
//sun add 20151102
extern INT8U   ETCO2_FLAG;
extern unsigned char EtCO2ModuleOff;
extern unsigned char MasimoISA_EtCO2_Config_On;
void SendSetModes(int modes);//  20130418
void SendOperationMessage(unsigned char type,unsigned char UsartPort); //  20130606
extern Int16U InquirePresentAlarmsPriority(Int64U Alarms,Int64U Alarms1);
//extern void CO2_ModuleDecoding(unsigned char * pointer,C002_CO2_PARAMETERS * CO2_Para);
extern unsigned char Drager_EtCO2_Config_On; //0:关闭   1：开启麻醉气体    2：开启麻醉气体+顺磁氧
extern Int16U AlarmPriorityIT;
#ifdef DEMO   //  [11/8/2022 lyy add]Demo
extern char Demo_Switch;
extern char Demo_Change_Cycle_A;
#endif
/***************************************************************************************************
* @brief
* @para
* @retval
*/
INT16U Inquire_TXD_Handler(INT8U UsartPort)
{
    INT64U   i;
    INT16U  err;
    INT64U temp = 1;//  20130402
    err=0;
    if(UsartPort==USART0)
    {
#ifdef SELFTEST
        if(USART0_TXD_RIGSTER)//正常模式
#endif
        {
            for(i=0; i<64; i++) //32 cpp md 20130402
            {
                if(USART0_TXD_RIGSTER&(temp)) //1<<i //cpp md 20130402 或改为 (INT64U)1<<i
                {
                    break;
                }
                temp =temp <<1;
            }
            switch(temp) //1<<i //cpp md 20130402
            {
            case SEND_UPDATEPROGRAM_FLAG://0xaa 取消   0x55 升级  //sun add 20140514
                DataTransmit(BDU_UpdateFlag,UPDATE_COMMAND,Usart0_Tx_Buffer,USART0);
                USART0_TXD_RIGSTER &= ~SEND_UPDATEPROGRAM_FLAG;
                USART0_TXD_Busy_FLAG=TRUE;
                break;
            case SEND_SET_VT_FLAG:
                DataTransmit(BeathSettingPara.SettingVtValue,VT_COMMAND_SET,Usart0_Tx_Buffer,USART0);
                USART0_TXD_RIGSTER &= ~SEND_SET_VT_FLAG;
                USART0_TXD_Busy_FLAG=TRUE;
                break;
            case SEND_SET_PT_TRIG_FLAG:
                //20240510 lyy modify 有负值，发送绝对值
                DataTransmit(abs(BeathSettingPara.SettingPtrValue),PT_COMMAND_SET,Usart0_Tx_Buffer,USART0);
                USART0_TXD_RIGSTER &= ~SEND_SET_PT_TRIG_FLAG;
                USART0_TXD_Busy_FLAG=TRUE;
                break;
            case SEND_SET_FLOW_TRIG_FLAG:
                DataTransmit(BeathSettingPara.SettingFtrValue,FLOW_TRIGGER_COMMAND_SET,Usart0_Tx_Buffer,USART0);
                USART0_TXD_RIGSTER &= ~SEND_SET_FLOW_TRIG_FLAG;
                USART0_TXD_Busy_FLAG=TRUE;
                break;
            case SEND_SET_FIO2_FLAG:
                DataTransmit(BeathSettingPara.SettingFiO2Value,FIO2_COMMAND_SET,Usart0_Tx_Buffer,USART0);
                USART0_TXD_RIGSTER &= ~SEND_SET_FIO2_FLAG;
                USART0_TXD_Busy_FLAG=TRUE;
                break;
            case SEND_SET_TI_FLAG:
                if((BeathSettingPara.BreathMode == VCV_MODE) || (BeathSettingPara.BreathMode == SIGH_MODE))
                {
                    DataTransmit(BeathSettingPara.SettingTiValue*(1 - BeathSettingPara.SettingTpValue/100.0)+0.5,TI_COMMAND_SET,Usart0_Tx_Buffer,USART0);
                }
                else
                {
                    DataTransmit(BeathSettingPara.SettingTiValue,TI_COMMAND_SET,Usart0_Tx_Buffer,USART0);
                }
                //DataTransmit(BeathSettingPara.SettingTiValue,TI_COMMAND_SET,Usart0_Tx_Buffer,USART0);
                USART0_TXD_RIGSTER &= ~SEND_SET_TI_FLAG;
                USART0_TXD_Busy_FLAG=TRUE;
                break;
            case SEND_SET_TP_FLAG:
                DataTransmit(BeathSettingPara.SettingTpValue,TP_COMMAND_SET,Usart0_Tx_Buffer,USART0);
                USART0_TXD_RIGSTER &= ~SEND_SET_TP_FLAG;
                USART0_TXD_Busy_FLAG=TRUE;
                break;
            case SEND_SET_PSV_FLAG:
                DataTransmit(BeathSettingPara.SettingPsupportValue,PSV_COMMAND_SET,Usart0_Tx_Buffer,USART0);
                USART0_TXD_RIGSTER &= ~SEND_SET_PSV_FLAG;
                USART0_TXD_Busy_FLAG=TRUE;
                break;
            case SEND_SET_PEEP_FLAG:
                DataTransmit(BeathSettingPara.SettingPEEPValue,PEEP_COMMAND_SET,Usart0_Tx_Buffer,USART0);
                USART0_TXD_RIGSTER &= ~SEND_SET_PEEP_FLAG;
                USART0_TXD_Busy_FLAG=TRUE;
                break;
            case SEND_SET_F_FLAG:
                DataTransmit(BeathSettingPara.SettingRateValue,F_COMMAND_SET,Usart0_Tx_Buffer,USART0);
                USART0_TXD_RIGSTER &= ~SEND_SET_F_FLAG;
                USART0_TXD_Busy_FLAG=TRUE;
                break;
            case SEND_SET_F_SIMV_FLAG:
                DataTransmit(BeathSettingPara.SettingSimvRateValue,F_SIMV_COMMAND_SET,Usart0_Tx_Buffer,USART0);
                USART0_TXD_RIGSTER &= ~SEND_SET_F_SIMV_FLAG;
                USART0_TXD_Busy_FLAG=TRUE;
                break;
            case SEND_SET_PCV_FLAG:
                DataTransmit(BeathSettingPara.SettingPinspValue,PCV_COMMOND_SET,Usart0_Tx_Buffer,USART0);
                USART0_TXD_RIGSTER &= ~SEND_SET_PCV_FLAG;
                USART0_TXD_Busy_FLAG=TRUE;
                break;
            case SEND_SET_TAP_FLAG:
                DataTransmit(AlarmSettingPara.ApneaTimerValue,TAP_COMMAND_SET,Usart0_Tx_Buffer,USART0);
                USART0_TXD_RIGSTER &= ~SEND_SET_TAP_FLAG;
                USART0_TXD_Busy_FLAG=TRUE;
                break;

            case SEND_SET_PHL_FLAG:
                DataTransmit(AlarmSettingPara.PressureHighValue,PHL_COMMAND_SET,Usart0_Tx_Buffer,USART0);
//                DataTransmit(BeathSettingPara.SettingPHLimitValue,PHL_COMMAND_SET,Usart0_Tx_Buffer,USART0);
                USART0_TXD_RIGSTER &= ~SEND_SET_PHL_FLAG;
                USART0_TXD_Busy_FLAG=TRUE;
                break;
                
            case SEND_RUN_STANDBY_FLAG:
                DataTransmit(StandbyState,RUN_STANDBY_COMMAND,Usart0_Tx_Buffer,USART0);
                USART0_TXD_RIGSTER &= ~SEND_RUN_STANDBY_FLAG;
                USART0_TXD_Busy_FLAG=TRUE;
                break;
            case SEND_O2_SCANT_FLAG:
                DataTransmit((GasSourceState>>8)&0xFF,O2_SCANT_COMMAND_ALARM,Usart0_Tx_Buffer,USART0);
                USART0_TXD_RIGSTER &= ~SEND_O2_SCANT_FLAG;
                USART0_TXD_Busy_FLAG=TRUE;
                break;
            case SEND_AIR_SCANT_FLAG:
                DataTransmit(GasSourceState&0xFF,AIR_SCANT_COMMAND_ALARM,Usart0_Tx_Buffer,USART0);
                USART0_TXD_RIGSTER &= ~SEND_AIR_SCANT_FLAG;
                USART0_TXD_Busy_FLAG=TRUE;
                break;
            case SEND_APNEA_FLAG:
                DataTransmit(APNEA_MODE,SYSTEM_MODE_COMMAND,Usart0_Tx_Buffer,USART0);
                USART0_TXD_RIGSTER &= ~SEND_APNEA_FLAG;
                USART0_TXD_Busy_FLAG=TRUE;
                break;
            case SEND_SYSTEM_MODE_FLAG:
                DataTransmit(BeathSettingPara.BreathMode,SYSTEM_MODE_COMMAND,Usart0_Tx_Buffer,USART0);
                USART0_TXD_RIGSTER &= ~SEND_SYSTEM_MODE_FLAG;
                USART0_TXD_Busy_FLAG=TRUE;
                break;
            case SEND_TRIG_ACTIVE_FLAG:
                DataTransmit(BeathSettingPara.TriggerMode,TRIG_ACTIVE_COMMAND,Usart0_Tx_Buffer,USART0);
                USART0_TXD_RIGSTER &= ~SEND_TRIG_ACTIVE_FLAG;
                USART0_TXD_Busy_FLAG=TRUE;
                break;
            case SEND_INSP_HOLD_FLAG:
                DataTransmit(InspHoldState,INSP_HOLD_COMMAND,Usart0_Tx_Buffer,USART0);
                USART0_TXD_RIGSTER &= ~SEND_INSP_HOLD_FLAG;
                USART0_TXD_Busy_FLAG=TRUE;
                break;
            case SEND_EXP_HOLD_FLAG:
                DataTransmit(ExpHoldState,EXP_PAUSE_COMMAND,Usart0_Tx_Buffer,USART0);
                USART0_TXD_RIGSTER &= ~SEND_EXP_HOLD_FLAG;
                USART0_TXD_Busy_FLAG=TRUE;
                break;
            case SEND_SUCTION_FLAG:
                DataTransmit(SuctionState,O2_SUCTION_COMMAND,Usart0_Tx_Buffer,USART0);
                USART0_TXD_RIGSTER &= ~SEND_SUCTION_FLAG;
                USART0_TXD_Busy_FLAG=TRUE;
                break;
            case SEND_MANUAL_INSP_FLAG:
                DataTransmit(0,MANUAL_INSP_COMMAND,Usart0_Tx_Buffer,USART0);
                USART0_TXD_RIGSTER &= ~SEND_MANUAL_INSP_FLAG;
                USART0_TXD_Busy_FLAG=TRUE;
                break;
            case SEND_PRESSURE_CALIBRATION_FLAG:
                DataTransmit(PRESS_ZERO_CORRECT_COM<<8,SYSTEM_MODE_COMMAND,Usart0_Tx_Buffer,USART0);
                USART0_TXD_RIGSTER &= ~SEND_PRESSURE_CALIBRATION_FLAG;
                USART0_TXD_Busy_FLAG=TRUE;
                break;
            case SEND_FLOW_CALIBRATION_FLAG:
                DataTransmit(FLOW_SENSOR_CORRECT_COM<<8,SYSTEM_MODE_COMMAND,Usart0_Tx_Buffer,USART0);
                USART0_TXD_RIGSTER &= ~SEND_FLOW_CALIBRATION_FLAG;
                USART0_TXD_Busy_FLAG=TRUE;
                break;

            case SEND_OXY_CALIBRATION_FLAG:
                if(oxyChooseFlag == 0)  //sun add 20140529
                {
                    DataTransmit(OXY_ZERO_CORRECT_COM<<8,SYSTEM_MODE_COMMAND,Usart0_Tx_Buffer,USART0);
                }
                else
                {
                    DataTransmit(OXY_GAIN_CORRECT_COM<<8,SYSTEM_MODE_COMMAND,Usart0_Tx_Buffer,USART0);
                }
                //DataTransmit(OXY_ZERO_CORRECT_COM<<8,SYSTEM_MODE_COMMAND,Usart0_Tx_Buffer,USART0);
                USART0_TXD_RIGSTER &= ~SEND_OXY_CALIBRATION_FLAG;
                USART0_TXD_Busy_FLAG=TRUE;
                break;
            case SEND_EXP_VALVE_CALIBRATION_FLAG:
                DataTransmit(EXP_VALVE_CORRECT_COM<<8,SYSTEM_MODE_COMMAND,Usart0_Tx_Buffer,USART0);
                USART0_TXD_RIGSTER &= ~SEND_EXP_VALVE_CALIBRATION_FLAG;
                USART0_TXD_Busy_FLAG=TRUE;
                break;
            case SEND_INSP_VALVE_CALIBRATION_FACTORY_FLAG:
                DataTransmit(FACTORY_VALVE_CORRECT_COM<<8,SYSTEM_MODE_COMMAND,Usart0_Tx_Buffer,USART0);
                USART0_TXD_RIGSTER &= ~SEND_INSP_VALVE_CALIBRATION_FACTORY_FLAG;
                USART0_TXD_Busy_FLAG=TRUE;
                break;
            case SEND_EXP_FLOW_SENSOR_CALIBRATION_FLAG:
                DataTransmit(LOCAL_VALVE_CORRECT_COM<<8,SYSTEM_MODE_COMMAND,Usart0_Tx_Buffer,USART0);
                USART0_TXD_RIGSTER &= ~SEND_EXP_FLOW_SENSOR_CALIBRATION_FLAG;
                USART0_TXD_Busy_FLAG=TRUE;
                break;
            //*//  20130221
            case SEND_SIGH_TIME_FLAG:
                DataTransmit(BeathSettingPara.SettingSighTimesValue,SIGH_TIMES_COMMAND_SET,Usart0_Tx_Buffer,USART0);
                USART0_TXD_RIGSTER &= ~SEND_SIGH_TIME_FLAG;
                USART0_TXD_Busy_FLAG=TRUE;
                break;
//            case SEND_SET_TSLOP_FLAG: //  20130607
//                DataTransmit(BeathSettingPara.SettingTslopeValue,TSLOPE_COMMAND_SET,Usart0_Tx_Buffer,USART0);
//                USART0_TXD_RIGSTER &= ~SEND_SET_TSLOP_FLAG;
//                USART0_TXD_Busy_FLAG=TRUE;
//                break;
            case SEND_SET_TSLOP_FLAG:
                if(BeathSettingPara.SettingTslopeValue < 20)//发送值与实际是100倍关系，即1s = 1000数字量
                {
                    if(BeathSettingPara.SettingTiValue >= 20)//当时间大于等于0.2s，Tslope小于0.2s，发送给BDU的Tslope实际为20
                    {
                        DataTransmit(20,TSLOPE_COMMAND_SET,Usart0_Tx_Buffer,USART0);
                    }
                    else//当时间等于0.1s，Tslope小于0.2s，发送给BDU的Tslope实际为10
                    {
                        DataTransmit(10,TSLOPE_COMMAND_SET,Usart0_Tx_Buffer,USART0);
                    }
                }
                else
                {
                    DataTransmit(BeathSettingPara.SettingTslopeValue,TSLOPE_COMMAND_SET,Usart0_Tx_Buffer,USART0);
                }
                USART0_TXD_RIGSTER &= ~SEND_SET_TSLOP_FLAG;
                USART0_TXD_Busy_FLAG=TRUE;
                break;
            case SEND_SET_ESENSE_FLAG:
                DataTransmit(BeathSettingPara.EsenseValue,   ESENSE_COMMAND_VALUE,Usart0_Tx_Buffer,USART0);
                USART0_TXD_RIGSTER &= ~SEND_SET_ESENSE_FLAG;
                USART0_TXD_Busy_FLAG=TRUE;
                break;
            case SEND_BAROMETER_FLAG:  //发送使用地大气压力值，和开启大气压力传感器命令
                DataTransmit(Barameter_Pressure,BAROMETER_SEND_COMMAND_VALUE,Usart0_Tx_Buffer,USART0);
                USART0_TXD_RIGSTER &= ~SEND_BAROMETER_FLAG;
                USART0_TXD_Busy_FLAG=TRUE;
                break;
            case SEND_BAROMETER_STAND_FLAG://发送校验地大气压力值。
                DataTransmit(Barameter_Stand_Pressure,BAROMETER_SEND_STAND_COMMAND_VALUE,Usart0_Tx_Buffer,USART0);
                USART0_TXD_RIGSTER &= ~SEND_BAROMETER_STAND_FLAG;
                USART0_TXD_Busy_FLAG=TRUE;
                break;
            case SEND_BACKUP_MODE_FLAG:
                DataTransmit(BeathSettingPara.ApnoeaMode,BACKUP_MODE_COMMAND,Usart0_Tx_Buffer,USART0);
                USART0_TXD_RIGSTER &= ~SEND_BACKUP_MODE_FLAG;
                USART0_TXD_Busy_FLAG=TRUE;
                break;
            case SEND_ASK_VERSION_FORBDU://询问BDU版本号
                DataTransmit(0x55,VERSION_ASK_COMMAND,Usart0_Tx_Buffer,USART0);
                USART0_TXD_RIGSTER &= ~SEND_ASK_VERSION_FORBDU;
                USART0_TXD_Busy_FLAG=TRUE;
                break;
            case SEND_DIRVER_GAS_FORBDU://发送驱动气体 zkq add 20170407
                if(!DirverGasFlag)//Air
                {
                    DataTransmit(0xAA,DIRVER_GAS_VALUE,Usart0_Tx_Buffer,USART0);
                }
                else//O2
                {
                    DataTransmit(0x55,DIRVER_GAS_VALUE,Usart0_Tx_Buffer,USART0);
                }
                USART0_TXD_RIGSTER &= ~SEND_DIRVER_GAS_FORBDU;
                USART0_TXD_Busy_FLAG=TRUE;
                break;
            //
            case SEND_NPLSWITCH_FLAG: //NIF P0.1 肺复张
                DataTransmit((u16)((Tools_GetMeasureFlag()<<8) | Tools_GetTypeSelect()), SWITCH_COMMAND_SET,  \
                    Usart0_Tx_Buffer, USART0);
                USART0_TXD_RIGSTER &= ~SEND_NPLSWITCH_FLAG;
                USART0_TXD_Busy_FLAG=TRUE;
                break;   
            case SEND_LUNGPAWTIME_FLAG://肺复张压力、时间
                DataTransmit((u16)(LungRec_GetTime()<<8 | LungRec_GetPressure()), LUNGPAWTIME_COMMAND_SET, \
                    Usart0_Tx_Buffer, USART0);
                USART0_TXD_RIGSTER &= ~SEND_LUNGPAWTIME_FLAG;
                USART0_TXD_Busy_FLAG=TRUE;
                break;                
            // Phigh
            case SEND_SET_PHIGH_FLAG:
                DataTransmit(BeathSettingPara.SettingPhighValue,PHIGH_COMMAND_SET,Usart0_Tx_Buffer,USART0);
                USART0_TXD_RIGSTER &= ~SEND_SET_PHIGH_FLAG;
                USART0_TXD_Busy_FLAG=TRUE;
                break; 
            // Plow
            case SEND_SET_PLOW_FLAG:
                DataTransmit(BeathSettingPara.SettingPlowValue,PLOW_COMMAND_SET,Usart0_Tx_Buffer,USART0);
                USART0_TXD_RIGSTER &= ~SEND_SET_PLOW_FLAG;
                USART0_TXD_Busy_FLAG=TRUE;
                break;
            // Thigh
            case SEND_SET_THIGH_FLAG:
                DataTransmit(BeathSettingPara.SettingThighValue*10,THIGH_COMMAND_SET,Usart0_Tx_Buffer,USART0);
                USART0_TXD_RIGSTER &= ~SEND_SET_THIGH_FLAG;
                USART0_TXD_Busy_FLAG=TRUE;
                break;
            // Tlow
            case SEND_SET_TLOW_FLAG:
                DataTransmit(BeathSettingPara.SettingTlowValue*10,TLOW_COMMAND_SET,Usart0_Tx_Buffer,USART0);
                USART0_TXD_RIGSTER &= ~SEND_SET_TLOW_FLAG;
                USART0_TXD_Busy_FLAG=TRUE;
                break;                
            // VTSupport
            case SEND_SET_VTSUPP_FLAG:
                DataTransmit(BeathSettingPara.SettingVtSupportValue,VTSUPPORT_COMMAND_SET,Usart0_Tx_Buffer,USART0);
                USART0_TXD_RIGSTER &= ~SEND_SET_VTSUPP_FLAG;
                USART0_TXD_Busy_FLAG=TRUE;
                break;   
            // VS Rate
            case SEND_SET_VS_F_FLAG:
                DataTransmit(BeathSettingPara.SettingVSRateValue,F_COMMAND_SET,Usart0_Tx_Buffer,USART0);
                USART0_TXD_RIGSTER &= ~SEND_SET_VS_F_FLAG;
                USART0_TXD_Busy_FLAG=TRUE;
                break; 
            // BIPAP Rate
            case SEND_SET_F_BIPAP_FLAG:
                DataTransmit(BeathSettingPara.SettingBipapRateValue,F_BIPAP_COMMAND_SET,Usart0_Tx_Buffer,USART0);
                USART0_TXD_RIGSTER &= ~SEND_SET_F_BIPAP_FLAG;
                USART0_TXD_Busy_FLAG=TRUE;
                break;                
            default:
                err=1;
                break;
            }
        }
#ifdef SELFTEST
        else if(CHECK_DEBUG == 1)//自检模式
        {
            err = Test_TXD_Handle(USART0);
        }
        else //正常工作模式，但是没有命令要发送
        {
            err = 1;
        }
#endif
    }
#if (LOGO_FLAG == MSW_LOGO)
    else if(UsartPort == USART1)
    {
        for(i = 0; i< 7; i++)
        {
            if(USART1_TXD_RIGSTER&(1<<i))
            {
                break;
            }
        }
        if(i == CO2_PUMPRUNHOUR)
        {
            USART1_TXD_RIGSTER &= ~(1<<CO2_PUMPRUNHOUR);
            USART1_TXD_Busy_FLAG=TRUE;
            CheckCo2PumpRunTime();
        }
        else if(i == CO2_BTPSCOMPENSATE)
        {
            USART1_TXD_RIGSTER &= ~(1<<CO2_BTPSCOMPENSATE);
            USART1_TXD_Busy_FLAG=TRUE;
            BTPSCompensate();
        }
        else if(i == CO2_O2COMPENSATE)
        {
            USART1_TXD_RIGSTER &= ~(1<<CO2_O2COMPENSATE);
            USART1_TXD_Busy_FLAG=TRUE;
            O2CO2Compensate();
        }
        else if(i == CO2_PUMPOCCLUSION)
        {
            USART1_TXD_RIGSTER &= ~(1<<CO2_PUMPOCCLUSION);
            USART1_TXD_Busy_FLAG=TRUE;
            CheckCo2PumpOcclusion();
        }
        else if(i == CO2_MODULEONOFF)
        {
            USART1_TXD_RIGSTER &= ~(1<<CO2_MODULEONOFF);
            USART1_TXD_Busy_FLAG=TRUE;
            if(StandbyState)
            {
                Co2ModuleONOFF(0);
            }
            else
            {
                Co2ModuleONOFF(1);
            }
        }
        else if(i == CO2_MODULEON)
        {
            USART1_TXD_RIGSTER &= ~(1<<CO2_MODULEON);
            USART1_TXD_Busy_FLAG=TRUE;
            Co2ModuleONOFF(1);
        }
        else if(i == CO2_MODULEOFF)
        {
            USART1_TXD_RIGSTER &= ~(1<<CO2_MODULEOFF);
            USART1_TXD_Busy_FLAG=TRUE;
            Co2ModuleONOFF(0);
        }
        else
        {
            err = 1;
        }
    }
#endif
    else if (UsartPort==USART2) //  20130705
    {
#if (LOGO_FLAG == MSW_LOGO)
        if(0)
#else
        if(AA_Module_Flag.ETCO2_FLAG == 1)
#endif
        {
            for(i = 0; i< 7; i++)
            {
                if(USART2_TXD_RIGSTER&(1<<i))
                {
                    break;
                }
            }
            if(i == CO2_PUMPRUNHOUR)
            {
                USART2_TXD_RIGSTER &= ~(1<<CO2_PUMPRUNHOUR);
                USART2_TXD_Busy_FLAG=TRUE;
                CheckCo2PumpRunTime();
            }
            else if(i == CO2_BTPSCOMPENSATE)
            {
                USART2_TXD_RIGSTER &= ~(1<<CO2_BTPSCOMPENSATE);
                USART2_TXD_Busy_FLAG=TRUE;
                BTPSCompensate();
            }
            else if(i == CO2_O2COMPENSATE)
            {
                USART2_TXD_RIGSTER &= ~(1<<CO2_O2COMPENSATE);
                USART2_TXD_Busy_FLAG=TRUE;
                O2CO2Compensate();
            }
            else if(i == CO2_PUMPOCCLUSION)
            {
                USART2_TXD_RIGSTER &= ~(1<<CO2_PUMPOCCLUSION);
                USART2_TXD_Busy_FLAG=TRUE;
                CheckCo2PumpOcclusion();
            }
            else if(i == CO2_MODULEONOFF)
            {
                USART2_TXD_RIGSTER &= ~(1<<CO2_MODULEONOFF);
                USART2_TXD_Busy_FLAG=TRUE;
                if(StandbyState)
                {
                    Co2ModuleONOFF(0);
                }
                else
                {
                    Co2ModuleONOFF(1);
                }
            }
            else if(i == CO2_MODULEON)
            {
                USART2_TXD_RIGSTER &= ~(1<<CO2_MODULEON);
                USART2_TXD_Busy_FLAG=TRUE;
                Co2ModuleONOFF(1);
            }
            else if(i == CO2_MODULEOFF)
            {
                USART2_TXD_RIGSTER &= ~(1<<CO2_MODULEOFF);
                USART2_TXD_Busy_FLAG=TRUE;
                Co2ModuleONOFF(0);
            }
            else
            {
                err = 1;
            }
        }
        else if(AA_Module_Flag.ETCO2_FLAG == 0)//djq add 2013-12-23
        {
            for(i=0; i<16; i++)
            {
                if(USART2_TXD_RIGSTER&(1<<i))
                {
                    break;
                }
                temp = temp<<1;
            }
            switch(temp)
            {
//            case SEND_MODULE_SWITCH_FLAG:
//              DataTransmit(EtCO2ModuleOff+1,MODULE_SWITCH_COMMAND,Usart2_Tx_Buffer,USART2);
//              USART2_TXD_RIGSTER &= ~SEND_MODULE_SWITCH_FLAG;
//              USART2_TXD_Busy_FLAG=TRUE;
//              break;
//            case SEND_CAL_ZERO_FLAG:
//              DataTransmit(IM_CAL_NORM,IM_ZERO_CAL,Usart2_Tx_Buffer,USART2);
//              USART2_TXD_RIGSTER &= ~SEND_CAL_ZERO_FLAG;
//              USART2_TXD_Busy_FLAG=TRUE;
//              break;
              //zkq add 20171106

            case AA_MONITOR_STOPCONTTRANS:
                AA_GetCMDFrame(AA_CMD_StopContinueTrans,Usart2_Tx_Buffer);
                USART2_TXD_RIGSTER &= ~AA_MONITOR_STOPCONTTRANS;
                USART2_TXD_Busy_FLAG=TRUE;
                break;
            case AA_MONITOR_INITZERO:
                AA_GetCMDFrame(AA_CMD_InitZero,Usart2_Tx_Buffer);
                USART2_TXD_RIGSTER &= ~AA_MONITOR_INITZERO;
                USART2_TXD_Busy_FLAG=TRUE;
                break;
            case AA_MONITOR_SWITCHPUMPS:
                AA_GetCMDFrame(AA_CMD_SwitchPumps,Usart2_Tx_Buffer);
                USART2_TXD_RIGSTER &= ~AA_MONITOR_SWITCHPUMPS;
                USART2_TXD_Busy_FLAG=TRUE;
                break;
            case AA_MONITOR_SWITCHVALVES:
                AA_GetCMDFrame(AA_CMD_SwitchValves,Usart2_Tx_Buffer);
                USART2_TXD_RIGSTER &= ~AA_MONITOR_SWITCHVALVES;
                USART2_TXD_Busy_FLAG=TRUE;
                break;
            case AA_MONITOR_SETOPMODE:
                AA_GetCMDFrame(AA_CMD_SetOpMode,Usart2_Tx_Buffer);
                USART2_TXD_RIGSTER &= ~AA_MONITOR_SETOPMODE;
                USART2_TXD_Busy_FLAG=TRUE;
                break;
            case AA_MONITOR_SETAAGAS:
                AA_GetCMDFrame(AA_CMD_AA_SetAA,Usart2_Tx_Buffer);
                USART2_TXD_RIGSTER &= ~AA_MONITOR_SETAAGAS;
                USART2_TXD_Busy_FLAG=TRUE;
                break;
            case AA_MONITOR_SETEXPARA:
                AA_GetCMDFrame(AA_CMD_Setpara,Usart2_Tx_Buffer);
                USART2_TXD_RIGSTER &= ~AA_MONITOR_SETEXPARA;
                USART2_TXD_Busy_FLAG=TRUE;
                break;
            case AA_MONITOR_SETUNIT:
                AA_GetCMDFrame(AA_CMD_SetParaUnit,Usart2_Tx_Buffer);
                USART2_TXD_RIGSTER &= ~AA_MONITOR_SETUNIT;
                USART2_TXD_Busy_FLAG=TRUE;
                break;
            case AA_MONITOR_SETBREATHDETECTION:
                AA_GetCMDFrame(AA_CMD_SetBreathDetection,Usart2_Tx_Buffer);
                USART2_TXD_RIGSTER &= ~AA_MONITOR_SETBREATHDETECTION;
                USART2_TXD_Busy_FLAG=TRUE;
                break;
            case AA_MONITOR_SETPATIENTDATA:
                AA_GetCMDFrame(AA_CMD_SetPatientData,Usart2_Tx_Buffer);
                USART2_TXD_RIGSTER &= ~AA_MONITOR_SETPATIENTDATA;
                USART2_TXD_Busy_FLAG=TRUE;
                break;
            case AA_MONITOR_GETDEVINFO:
                AA_GetCMDFrame(AA_CMD_GetDevInfo,Usart2_Tx_Buffer);
                USART2_TXD_RIGSTER &= ~AA_MONITOR_GETDEVINFO;
                USART2_TXD_Busy_FLAG=TRUE;
                break;
            case AA_MONITOR_SETIRO2DELAY:
                AA_GetCMDFrame(AA_CMD_SetIRO2Delay,Usart2_Tx_Buffer);
                USART2_TXD_RIGSTER &= ~AA_MONITOR_SETIRO2DELAY;
                USART2_TXD_Busy_FLAG=TRUE;
                break;
            case AA_MONITOR_SETO2OPTION:
                AA_GetCMDFrame(AA_CMD_SetO2Option,Usart2_Tx_Buffer);
                USART2_TXD_RIGSTER &= ~AA_MONITOR_SETO2OPTION;
                USART2_TXD_Busy_FLAG=TRUE;
                break;
            case AA_MONITOR_SETO2SENSORTYPE:
                AA_GetCMDFrame(AA_CMD_SetO2SensorType,Usart2_Tx_Buffer);
                USART2_TXD_RIGSTER &= ~AA_MONITOR_SETO2SENSORTYPE;
                USART2_TXD_Busy_FLAG=TRUE;
                break;
            case AA_MONITOR_SPANCAL:
                AA_GetCMDFrame(AA_CMD_SpanCal,Usart2_Tx_Buffer);
                USART2_TXD_RIGSTER &= ~AA_MONITOR_SPANCAL;
                USART2_TXD_Busy_FLAG=TRUE;
                break;
            case AA_MONITOR_SPANCAL21:
                AA_GetCMDFrame(AA_CMD_SpanCal21,Usart2_Tx_Buffer);
                USART2_TXD_RIGSTER &= ~AA_MONITOR_SPANCAL21;
                USART2_TXD_Busy_FLAG=TRUE;
                break;
            default:
            {
                err=1;
            }
            }
            if(err != 1)//ylk add 20181204 数据发送
            {
                U2THR = Usart2_Tx_Buffer[0];
            }
        }
    }

    else if(UsartPort==USART3)
    {
        for(i=0; i<8; i++) //3 cpp md 20130402
        {
            if(USART3_TXD_RIGSTER&(1<<i))
            {
                break;
            }
        }
        if(i==ALARM_PRIORITY_CHANGED)
        {
//            AlarmPriority=InquirePresentAlarmsPriority(AlarmsWork.presentAlarms,AlarmsWork.presentAlarms1);   //zy 2016.8.11
//            if(AlarmPriority&HIGH_PRIORITY)
//            {
//                AlarmPriority |= 0x2001;
//            }
//            if(AlarmPriority&MEDIUM_PRIORITY)  //sun add 20140512
//            {
//                AlarmPriority |= 0x4002;
//            }
//            if(AlarmPriority&LOW_PRIORITY)  //sun add 20140512
//            {
//                AlarmPriority |= 0x4004;
//            }

//            DataTransmit(AlarmPriority,ALARM_STATE_CHANGE_COM,Usart3_Tx_Buffer,USART3);
//            AlarmPriority &= 0xFF;//  20130402

            DataTransmit(AlarmPriorityIT,ALARM_STATE_CHANGE_COM,Usart3_Tx_Buffer,USART3);
            USART3_TXD_RIGSTER &= ~SEND_ALARM_UPDATE_FLAG;
            USART3_TXD_Busy_FLAG=TRUE;
        }
        else if(i==LED_UPDATE_OPERATION)
        {
            DataTransmit(LedUpdateRegister,LED_CHANGE_COM,Usart3_Tx_Buffer,USART3);
            USART3_TXD_RIGSTER &= ~SEND_LED_UPDATE_FLAG;
            USART3_TXD_Busy_FLAG=TRUE;
        }
        else if(i==HANDSHAKE_INFO_DELIVER)
        {
            DataTransmit(0x55,HANDSHAKE_SEND_COM,Usart3_Tx_Buffer,USART3);
            USART3_TXD_RIGSTER &= ~SEND_HANDSHAKE_FLAG;
            USART3_TXD_Busy_FLAG=TRUE;
        }
        //*//  20130402
        else if(i==BUZZER_CONTROL_OPERATION)
        {
            DataTransmit(AlarmsWork.BeepState,BUZZER_CONTROL_COM,Usart3_Tx_Buffer,USART3);
            USART3_TXD_RIGSTER &= ~SEND_BUZZER_CONTROL_FLAG;
            USART3_TXD_Busy_FLAG=TRUE;
        }
        else if(i==BUZZER_VOLUME_SETTING)
        {
            //i = AlarmSettingPara.AlarmVolumeValue | 0xFF00;//cpp del 20121225
            i = 4 - AlarmSettingPara.AlarmVolumeValue | 0xFF00;//  20121225
            DataTransmit(i,BUZZER_TEST_COM,Usart3_Tx_Buffer,USART3);
            USART3_TXD_RIGSTER &= ~SEND_BUZZER_VOLUME_FLAG;
            USART3_TXD_Busy_FLAG=TRUE;
        }
        else if(i==BUZZER_VOLUME_TEST)
        {
            DataTransmit(BuzzerTestVolume,BUZZER_TEST_COM,Usart3_Tx_Buffer,USART3);
            USART3_TXD_RIGSTER &= ~SEND_BUZZER_TEST_FLAG;
            USART3_TXD_Busy_FLAG=TRUE;
        }
        //*//  20130402
        else if(i==BUZZER_VOLUME_START)
        {
            DataTransmit(0xFF,BUZZER_START_COM,Usart3_Tx_Buffer,USART3);
            USART3_TXD_RIGSTER &= ~SEND_BUZZER_START_FLAG;
            USART3_TXD_Busy_FLAG=TRUE;
        }//*/
        else if(i==0x07)//sun add 20140514
        {
            DataTransmit(PMU_UpdateFlag,UPDATE_COMMAND,Usart3_Tx_Buffer,USART3);
            USART3_TXD_RIGSTER &= ~U3SEND_UPDATEPROGRAM_FLAG;
            USART3_TXD_Busy_FLAG=TRUE;
        }
     
        else if(i==SEND_MACHINE_MODE_FLAG)
        {
            DataTransmit(LCD_SWITCH_MODE,COMM_MACHINE_MODE,Usart3_Tx_Buffer,USART3);
            USART3_TXD_RIGSTER &= ~U3SEND_MACHINE_MODE_FLAG;
            USART3_TXD_Busy_FLAG=TRUE;    
        }
        
        else
        {
            err=1;
        }
    }
    return(err);
}

/***************************************************************************************************
* @brief
* @para
* @retval
*/

void DataTransmit(INT16U data,INT8U type,INT8U * Pointer,INT8U UsartPort)
{
    switch(UsartPort)
    {
    case USART0:
        *Pointer ='@';
        *(Pointer+1)=type;
        *(Pointer+2)=data&0xFF;
        *(Pointer+3)=(data>>8)&0xFF;
#ifndef __debug
        U0THR ='$';
#endif
        break;
    case USART3:
        *Pointer ='@';
        *(Pointer+1)=type;
        *(Pointer+2)=data&0xFF;
        *(Pointer+3)=(data>>8)&0xFF;
        U3THR ='$';
        break;
    case USART1:
        *Pointer =IRMA_DATA_FLAG2;
        *(Pointer+1)=type;
        *(Pointer+2)=(INT8U)data;
        *(Pointer+3)=~(type+data)+1;
        U1THR =IRMA_DATA_FLAG1;
        break;
    case USART2:
        if(AA_Module_Flag.ETCO2_FLAG == 0)
		{
			if(AA_Module_Flag.EtCO2ModuleExist == 1)
			{
				*Pointer =IRMA_DATA_FLAG2;
				*(Pointer+1)=type;
				*(Pointer+2)=(INT8U)data;
				*(Pointer+3)=~(type+data)+1;
				U2THR =IRMA_DATA_FLAG1;
			}
            else
            {
				U2THR =IRMA_DATA_FLAG1;
				U2THR =IRMA_DATA_FLAG2;
				U2THR=type;
				U2THR=(INT8U)data;
				U2THR=~(type+data)+1;
            }
		}
		else
		{
			*Pointer =IRMA_DATA_FLAG2;
			*(Pointer+1)=type;
			*(Pointer+2)=(INT8U)data;
			*(Pointer+3)=~(type+data)+1;
			U2THR =IRMA_DATA_FLAG1;
		}
        break;
    }
}

/***************************************************************************************************
* @brief
* @para
* @retval
*/
void ReceiveDataProcess(INT16U data,INT8U type,INT8U UsartPort)
{
    static INT16U TsXscale=0,TsYscale=0;

    INT16U temp;
    INT16U m;
    static unsigned char flag = 0;
    if(UsartPort==USART0)
    {
#ifdef DEMO               // 20221116 lyy add演示模式下不向BDU接收数据
		if (!Demo_Switch)
#endif
		{
        Content_Check_Handler(data,type);
        switch(type)
        {
        case(UPDATE_COMMAND):  //sun add 20140514
        {
            if(((UpdateSend_Flag & 0xF0) == 0x90) && ((UpdateRecv_Flag & 0xF0) == 0xA0))//只有在等待接受模式下，才能接收
            {
                if(data == 0x55)//接受升级
                {
                    UpdateRecv_Flag |= 0x2;
                }
                else if(data == 0xaa)//数据接收失败，不升级
                {

                }
            }
        }
        break;
        case (FLOW_COMMAND_VALUE):
        {
            //RealtimePara.Flow = (data - 32768)*3/50;  //sun del 20140526
            //RealtimePara.Flow = (data - 32768)*3;  //sun add 20140526
            RealtimePara.Flow = Filter_RecvAverF((data - 32768)*3);//滤波 20190814
        }
        break;
        case (PRESSURE_COMMAND_VALUE):
        {
            //RealtimePara.Pressure = (data - 32768)/10;  //sun del 20140526
            //RealtimePara.Pressure = (data - 32768);  //sun add 20140526
            RealtimePara.Pressure = Filter_RecvAverP(data - 32768); //滤波 20190814
        }
        break;
        //*//  20130402
        case (O2_GAS_PRESSURE_COMMAND_VALUE):
        {
            BDU_Comm_Set(0);//超时计数清零 //ylk add 20181115
            RealtimePara.O2GasPressure = data; //400   for test 20130607
        }
        break;
        case (AIR_PRESSURE_COMMAND_VALUE):
        {
            RealtimePara.AirGasPressure = data;
        }
        break;
        //*/
        case (VOLUME_COMMAND_VALUE):
        {
            //RealtimePara.Volume = data;
            RealtimePara.Volume = Filter_RecvAverV(data);//滤波 20190814
        }
        break;
        case (INSP_SEND_COMMAND):
        {
            MonitorPara.VentState = INSP_PHASE;
            InspFlagForCurve = 1;
            InspFlagForAlarm = 1;
            if((unsigned char)(data&0xFF) == (unsigned char)MANUAL_TRIGGER)//手动触发
            {
                InspFlag = 1;
            }
        }
        break;
        case (VTI_COMMAND_VALUE):
        {
            MonitorPara.VtiValue = data;
        }
        break;
        case (VTE_COMMAND_VALUE):
        {
            MonitorPara.VteValue = data;
        }
        break;
        case (VES_COMMAND_VALUE):
        {
            MonitorPara.MVspnValue = data;
        }
        break;
        case (VE_COMMAND_VALUE):
        {
            MonitorPara.MinuteVolumeValue = data;
        }
        break;
        case (PEEP_COMMAND_VALUE):
        {
            MonitorPara.PeepValue = data;
        }
        break;
        case (Pp_COMMAND_VALUE):
        {
            MonitorPara.PplatValue = data;
        }
        break;
        case (Pm_COMMAND_VALUE):
        {
            MonitorPara.PmeanValue = data;
        }
        break;
        case (Pk_COMMAND_VALUE):
        {
            MonitorPara.PpeakValue = data;
        }
        break;
        case (FSPN_COMMAND_VALUE):
        {
            MonitorPara.fspnValue = data;
        }
        break;
        case (FIO2_COMMAND_VALUE):
        {
//            if(MasimoISA_EtCO2_Config_On != 2)//Masimo ISA不监测O2
            if((Drager_EtCO2_Config_On != 2) && (MasimoISA_EtCO2_Config_On !=2)) //ylk modify 20181204
            {
                MonitorPara.FiO2Value = data;
            }
        }
        break;
        case (COMPLIANCE_COMMAND_VALUE):
        {
            MonitorPara.ComplianceValue = data/10;
        }
        break;
        case (RESIST_COMMAND_VALUE):
        {
            MonitorPara.ResistValue = data;
        }
        break;
        case (Ft_COMMAND_VALUE):
        {
            MonitorPara.RateTotalValue = data;
        }
        break;
        case (BARO_SEND_COMMAND):
        {
            //MonitorPara. = data;
        }
        break;
        case (PRESSURE_TRIGGER_COMMAND):
        {
            Menu_hWin.InfoWin.InfoChangeFlag |= ASSIST_INFO_CHANGED_FLAG;
            AssistShowFlag = 1;
            TriggerHappenFlag = 1;
        }
        break;
        case (EXP_SEND_COMMAND):
        {
            MonitorPara.VentState = EXP_PHASE;
            Menu_hWin.InfoWin.InfoChangeFlag |= ASSIST_INFO_CHANGED_FLAG;
        }
        break;
        case (SYSTEM_MODE_COMMAND):
        {

            //*//  20130415 //cpp del 20130717
            Int8U DataLow = 0;
            Int8U DataHigh = 0;
            DataHigh = (data>>8)&0xFF;
            DataLow  = data&0xFF;
            switch(DataHigh)
            {
                //static int BreathMode_BeforeManual;
                //static int BreathMode_BeforeACGO;

                case (PRESS_ZERO_CORRECT_COM):
                case (FLOW_SENSOR_CORRECT_COM):
                case (OXY_ZERO_CORRECT_COM): //TODO 确定协议命令
                case (OXY_GAIN_CORRECT_COM):
                case (FACTORY_VALVE_CORRECT_COM):
                case (LOCAL_VALVE_CORRECT_COM):
                case (EXP_VALVE_CORRECT_COM):
                    if(DataLow == CORRECT_SUSCESS_COM)
                    {
                        CalibrateFinishedFlag = 1;
                    }
                    else if(DataLow == CORRECT_FAILURE_COM)
                    {
                    }
                    break;
#ifdef ACGO_SWITCH
                case (ACGO_MODE_COMMAND):
                    {
                        if(Menu_hWin.TextWin)
                        {
                            if(WM_IsVisible(Menu_hWin.TextWin))  //sun add 20140707
                            {
                                WM_HideWindow(Menu_hWin.TextWin);
                            }
                        }
                        if(DataLow == ACGO_OK_COMMAND)
                        {
                            WM_HWIN hwin;
#if (LOGO_FLAG == PENLON_LOGO)
                            SendOperationMessage(BUZZER_VOLUME_START,USART3);//切到ACGO时发出提示音 ylk add 20190418
#endif
                            if(MenuType == MODES_MENU)
                            {
                                if(StandbyState==0xFF)
                                {
                                    PlanMenuType=STANDBY_MENU;
                                }
                                else
                                {
                                    PlanMenuType=MAIN_MENU;
                                }
                                Task|=MEAU_UPDATE;
                                if(WM_IsWindow(Menu_hWin.hWin))
                                {
                                    BUTTON_SetPressed(WM_GetDialogItem(Menu_hWin.hWin,GUI_ID_BUTTON0),0);
                                    BUTTON_SetPressed(WM_GetDialogItem(Menu_hWin.hWin,GUI_ID_BUTTON1),0);
                                }
                            }

                            if(BeathSettingPara.BreathMode != ACGO_MODE)
                            {
                                if(BeathSettingPara.BreathMode != MANUAL_MODE)
                                {
                                    BreathMode_BeforeManual = BeathSettingPara.BreathMode;
                                    BreathMode_BeforeACGO = BreathMode_BeforeManual;
                                    BeathSettingPara.BreathMode = ACGO_MODE;
                                    Menu_hWin.InfoWin.InfoChangeFlag |= MODES_INFO_CHANGED_FLAG;
                                }
                                else
                                {
                                    BreathMode_BeforeManual = MANUAL_MODE;
                                    BeathSettingPara.BreathMode = ACGO_MODE;
                                    Menu_hWin.InfoWin.InfoChangeFlag |= MODES_INFO_CHANGED_FLAG;
                                }
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
                                BreathMode_AfterACGO = ACGO_MODE;
                                ModeChange_Flag = 1;
                                Alarm_SaveVent_para.ALARM_ValUpdateFlag |= 0x4F;
                            }
                        }
                        else if(DataLow == ACGO_ERROR_COMMAND)
                        {
                            if(MenuType == MODES_MENU)
                            {
                                if(StandbyState==0xFF)
                                {
                                    PlanMenuType=STANDBY_MENU;
                                    Task|=MEAU_UPDATE;
                                }
                                else
                                {
                                    PlanMenuType=MAIN_MENU;
                                    Task|=MEAU_UPDATE;
                                }
                                if(WM_IsWindow(Menu_hWin.hWin))
                                {
                                    BUTTON_SetPressed(WM_GetDialogItem(Menu_hWin.hWin,GUI_ID_BUTTON0),0);
                                    BUTTON_SetPressed(WM_GetDialogItem(Menu_hWin.hWin,GUI_ID_BUTTON1),0);
                                }
                            }

                            AcgoTimeFlag = 0;
                            AcgoPromptFlag = 0;

                            WM_HWIN hwin;
                            if(BreathMode_BeforeManual == BreathMode_BeforeACGO)
                            {
                                BeathSettingPara.BreathMode = BreathMode_BeforeManual;
                                if(WM_IsWindow(Menu_hWin.hWin))
                                {
                                    hwin = WM_GetDialogItem(Menu_hWin.hWin, GUI_ID_BUTTON0);
                                    WM_EnableWindow(hwin);
                                }
                            }
                            else
                            {
                                if(BreathMode_BeforeManual == MANUAL_MODE)
                                {
                                    BeathSettingPara.BreathMode = MANUAL_MODE;
                                }
                                else
                                {
                                    BeathSettingPara.BreathMode = BreathMode_BeforeACGO;
                                }
//                              BeathSettingPara.BreathMode = MANUAL_MODE;
                                BreathMode_BeforeManual = BreathMode_BeforeACGO;
                            }
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

                            SendSetModes(BeathSettingPara.BreathMode);//发送当前模式给BDU
                            SendOperationMessage(EXIT_STANDBY,USART0);
                            Menu_hWin.InfoWin.InfoChangeFlag |= MODES_INFO_CHANGED_FLAG;

                            BreathMode_AfterACGO = 0;
                            ModeChange_Flag = 1;
                            Alarm_SaveVent_para.ALARM_ValUpdateFlag |= 0x8F;
                        }
                    }
                    if(Menu_hWin.bWin)
                    {
                        WM_SendMessageNoPara(Menu_hWin.bWin, WM_INIT_DIALOG); //  20130607
                    }
                    if (Menu_hWin.RecordWin && MenuType==OXYGEN_SENSOR_CAL_MENU)
                    {
                        WM_SendMessageNoPara(Menu_hWin.RecordWin, WM_INIT_DIALOG);
                    }
                    break;
#endif
#ifdef MANNUL_SWITCH
                case (MANUAL_MODE_COMMAND):
                    {
                        if(Menu_hWin.TextWin)
                        {
                            if(WM_IsVisible(Menu_hWin.TextWin))  //sun add 20140707
                            {
                                WM_HideWindow(Menu_hWin.TextWin);
                            }
                        }
                        if(DataLow == MANUAL_OK_COMMAND)//djq add 20161126
                        {
                            WM_HWIN hwin;

                            if(MenuType == MODES_MENU)
                            {
                                if(StandbyState==0xFF)
                                {
                                    PlanMenuType=STANDBY_MENU;
                                }
                                else
                                {
                                    PlanMenuType=MAIN_MENU;
                                }
                                Task|=MEAU_UPDATE;
                                if(WM_IsWindow(Menu_hWin.hWin))
                                {
                                    BUTTON_SetPressed(WM_GetDialogItem(Menu_hWin.hWin,GUI_ID_BUTTON0),0);
                                    BUTTON_SetPressed(WM_GetDialogItem(Menu_hWin.hWin,GUI_ID_BUTTON1),0);
                                }
                            }

                            Alarm_SaveVent_para.ALARM_ValUpdateFlag &= 0x0F;//ylk add 20190212 高四位先清零
                            if(BeathSettingPara.BreathMode != MANUAL_MODE)
                            {
                                if(BeathSettingPara.BreathMode != ACGO_MODE)//如果当前不是ACGO模式
                                {
                                    BreathMode_BeforeManual = BeathSettingPara.BreathMode;
                                    BreathMode_BeforeACGO = BreathMode_BeforeManual;
                                    BeathSettingPara.BreathMode = MANUAL_MODE;
                                    Menu_hWin.InfoWin.InfoChangeFlag |= MODES_INFO_CHANGED_FLAG;
                                }
                                else//如果当前是ACGO模式
                                {
                                    BreathMode_BeforeManual = MANUAL_MODE;
                                }
                                //SendSetModes(BeathSettingPara.BreathMode);
                            }
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

                            BreathMode_AfterMANUAL = MANUAL_MODE;
                            ModeChange_Flag = 1;
                            Alarm_SaveVent_para.ALARM_ValUpdateFlag |= 0x1F;
                        }
                        else if(DataLow == MANUAL_ERROR_COMMAND)
                        {
                            if(MenuType == MODES_MENU)
                            {
                                if(StandbyState==0xFF)
                                {
                                    PlanMenuType=STANDBY_MENU;
                                    Task|=MEAU_UPDATE;
                                }
                                else
                                {
                                    PlanMenuType=MAIN_MENU;
                                    Task|=MEAU_UPDATE;
                                }
                                if(WM_IsWindow(Menu_hWin.hWin))
                                {
                                    BUTTON_SetPressed(WM_GetDialogItem(Menu_hWin.hWin,GUI_ID_BUTTON0),0);
                                    BUTTON_SetPressed(WM_GetDialogItem(Menu_hWin.hWin,GUI_ID_BUTTON1),0);
                                }
                            }
                            WM_HWIN hwin;
                            Alarm_SaveVent_para.ALARM_ValUpdateFlag &= 0x0F;//ylk add 20190212 高四位先清零
                            if(BeathSettingPara.BreathMode == ACGO_MODE)//如果当前是ACGO模式，清除手动激活状态
                            {
                                BreathMode_BeforeManual = BreathMode_BeforeACGO;
                            }
                            else//如果当前不是ACGO模式，还原为存储的机控状态并且刷新模式。
                            {
                                BeathSettingPara.BreathMode = BreathMode_BeforeACGO;
                                BreathMode_BeforeManual = BreathMode_BeforeACGO;
                                if(Menu_hWin.hWin)
                                {
                                    hwin = WM_GetDialogItem(Menu_hWin.hWin, GUI_ID_BUTTON0);
                                    WM_EnableWindow(hwin);
                                }
                                SendSetModes(BeathSettingPara.BreathMode);
                                Menu_hWin.InfoWin.InfoChangeFlag |= MODES_INFO_CHANGED_FLAG;
                            }
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

                            BreathMode_AfterMANUAL = 0;
                            Menu_hWin.InfoWin.InfoChangeFlag |= MODES_INFO_CHANGED_FLAG;//刷新模式
                            ModeChange_Flag = 1;
                            Alarm_SaveVent_para.ALARM_ValUpdateFlag |= 0x2F;
                        }
                    }
                    if(Menu_hWin.bWin)
                    {
                        WM_SendMessageNoPara(Menu_hWin.bWin, WM_INIT_DIALOG); //  20130607
                    }
                    if (Menu_hWin.RecordWin && MenuType==OXYGEN_SENSOR_CAL_MENU)
                    {
                        WM_SendMessageNoPara(Menu_hWin.RecordWin, WM_INIT_DIALOG);
                    }
                    break;
#else
                case (MANUAL_MODE_COMMAND)://djq add 20161014  X5程序下收到手动命令应该不响应，并且向BDU发送当前的机控模式命令，让BDU回到机控模式
                    {
                        if(DataLow == MANUAL_OK_COMMAND)
                        {
                            SendSetModes(BeathSettingPara.BreathMode);
                        }
                    }
#endif
            }
        }
        break;

        case (PMIN_COMMAND_VALUE):
        {
            MonitorPara.PminValue = ((int)data - 32768);//((int)data - 32768)/10; //不需要除以10 cpp md 20130402
        }
        break;
        //*//   20130402
        case (BDU_MAIN_VERSION_COMMAND):
        {
            BDUVersion.MainVersion = (data >> 8) & 0xff;
            BDUVersion.SubVersion  = data & 0xff;
            DataTransmit(data,BDU_MAIN_VERSION_COMMAND,Usart0_Tx_Buffer,USART0);
        }
        break;
        case (BDU_SUB_VERSION_COMMAND):
        {
            BDUVersion.DebugVersion = data & 0xff;
			BDUVersion.TestVersion = (data>>8) & 0xff;    //zy 2016/06/02
            DataTransmit(data ,BDU_SUB_VERSION_COMMAND,Usart0_Tx_Buffer,USART0);
        }
        break;
				//*/
        case (BAROMETER_RECV_COMMAND_VALUE):   //djq add 20140305
        {
            if(!Barameter_Switch)//大气压力传感器开，则接收数据
            {
                if((data >= 0) && (data <= 999))
                {
                    Barameter_Pressure = data;
                }
            }
        }
        break;
        case (PRVC_VT_LOW_COMMAND):
        {
            MonitorPara.PRVC_VT_LOW_Flag = data;
        }
        break;
        case (ABSORBER_DISCONNECT_COMMAND):  //sun add 20140710
        {
            MonitorPara.ABSORBER_Flag = data;
        }
        break;

        default:
            break;
        }

		}
	}
    else if(UsartPort==USART3)
    {
        switch(type)
        {
        case(UPDATE_COMMAND):  //sun add 20140514
        {
            if(((UpdateSend_Flag & 0xF0) == 0x90) && ((UpdateRecv_Flag & 0xF0) == 0xA0))//只有在等待接受模式下，才能接收
            {
                if(data == 0x55)//接受升级
                {
                    UpdateRecv_Flag |= 0x4;
                }
                else if(data == 0xaa)//数据接收失败，不升级
                {

                }
            }
        }
        break;
        case KEY_SEND_COM:
            if( !Picture_Load_Complete )
            {
              butter_press_count++;
            }
			GUI_StoreKeyMsg(DeviceKeyToGUIKey(data),1);
			break;
		case KNOB_SEND_COM:
			GUI_StoreKeyMsg(DeviceKeyToGUIKey(data),1);
			/*****************************20171127  gyf  add 旋转旋钮测试报警字符串**************************/
			if(Demo_Alarm)
			{
				DEMO_TestAlarm(data);
			}
			break;
        case HANDSHAKE_SEND_COM:
            break;
        case TS_XSCALE_SEND_COM:
            //data = data&0x7F7F;
            TsXscale = data;//(data>>8&0xFF)<<7 + data&0xFF;
            break;
        case TS_YSCALE_SEND_COM:
            //data = data&0x7F7F;
            TsYscale = data;
            if((!(TsYscale&0x8080))&(!(TsXscale&0x8080))&&(!flag))
            {
                flag = 1;
                temp =  ((TsYscale>>8)&0xFF)<<7;
                TsYscale = 1024 - (temp + (TsYscale&0xFF));
                // TsYscale = temp + (TsYscale&0xFF);
                temp = ((TsXscale>>8)&0xFF)<<7;
                TsXscale = temp + (TsXscale&0xff);
                //TsXscale = temp + (TsXscale&0xff);
                //GUI_TOUCH_Convert(TsXscale,TsYscale,1);
                if(!TouchScreenCalibrate)
                {
                    GUI_TOUCH_Convert(TsXscale,TsYscale,1);
                }
                else
                {
                    GUI_TOUCH_StoreState(TsXscale,TsYscale);
                }
            }
            else if(((TsYscale&0x8080))&((TsXscale&0x8080)))
            {
                flag = 0;
                TsYscale &= 0x7F7F;
                TsXscale &= 0x7F7F;
                temp =  ((TsYscale>>8)&0xFF)<<7;
                TsYscale = 1024 - (temp + (TsYscale&0xFF));
                temp = ((TsXscale>>8)&0xFF)<<7;
                TsXscale = 1024 -(temp + (TsXscale&0xff));
                //GUI_TOUCH_Convert(TsXscale,TsYscale,0);
                if(!TouchScreenCalibrate)
                {
                    GUI_TOUCH_Convert(TsXscale,TsYscale,0);
                }
                else
                {
                    GUI_TOUCH_StoreState(-1,-1);
                }
            }
            TsXscale = 0;
            TsYscale = 0;
            break;
        //*//   20130402
        case (PMU_MAIN_VERSION_COMMAND):
        {
            PMUVersion.MainVersion = (data >> 8) & 0xff;
            PMUVersion.SubVersion  = data & 0xff;
            DataTransmit(data,PMU_MAIN_VERSION_COMMAND,Usart3_Tx_Buffer,USART3);
        }
        break;
        case (PMU_SUB_VERSION_COMMAND):
        {
            PMUVersion.DebugVersion = data & 0xff;
            PMUVersion.TestVersion  = (data >> 8) & 0xff;
            DataTransmit(data,PMU_SUB_VERSION_COMMAND,Usart3_Tx_Buffer,USART3);
        }//*/
        break;

        default:{}
        }
    }
}

/***************************************************************************************************
* @brief
* @para
* @retval
*/
void Uart0IntHandler(void)
{
    static    INT16U  Uart0RxBuffer[3]= {0};
    static    INT8U   Rx_num=0;
    static    INT8U   Tx_num=0;
    static    INT16U  Wait_Syn=TRUE;
    static    INT16U  LastData=0;
    INT16U    CurrentData;
    INT8U         iir_value;
    iir_value = U0IIR_bit.IID;
    if(iir_value == 0x02)//receive int
    {
        if(Wait_Syn==TRUE)
        {
            CurrentData = U0RBR;
            if((LastData=='$') && (CurrentData=='@'))
            {
                Wait_Syn = FALSE;
            }
            LastData = CurrentData;
        }
        else
        {
            Uart0RxBuffer[Rx_num] = U0RBR;
            Rx_num++;
            if(Rx_num>=3)
            {
#ifdef SELFTEST
                if((CHECK_DEBUG == 1))//如果当前为自检模式，则进入自检处理函数
                {
                    if(!Test_RXD_Handle(Uart0RxBuffer[1]|(Uart0RxBuffer[2]<<8),Uart0RxBuffer[0],USART0))
                    {
                        //如果当前接收到到的不是自检命令，调用正常的数据接收命令
                        ReceiveDataProcess(Uart0RxBuffer[1]|(Uart0RxBuffer[2]<<8),Uart0RxBuffer[0],USART0);
                    }
                }
                else
#endif
                {
                    ReceiveDataProcess(Uart0RxBuffer[1]|(Uart0RxBuffer[2]<<8),Uart0RxBuffer[0],USART0);
                }
                Wait_Syn = TRUE;
                Rx_num=0;
                LastData=0;
            }
        }
    }
    else if(iir_value == 0x01)//send int
    {
        if(Tx_num<4)
        {
#ifndef __debug
					U0THR = Usart0_Tx_Buffer[Tx_num];
#endif
            Tx_num++;
        }
        else
        {
            if(Inquire_TXD_Handler(USART0)!=0)
            {
                USART0_TXD_Busy_FLAG=FALSE;
            }
            Tx_num=0;
        }
    }
    else if(iir_value == 0x06)//ylk add 20190905 数据超时检测
    {
        CurrentData = U0RBR;
    }
}

/***************************************************************************************************
* @brief
* @para
* @retval
*/
void Uart1IntHandler(void)
{
#if (LOGO_FLAG == MSW_LOGO)//zkq modify 20170504
    static    INT8U  Uart1RxBuffer[11]= {0};
    Int8U  data;
    static    INT8U   Tx_num=0;
    static    INT8U   LastData=0;
#else
    static    INT8U  Uart1RxBuffer[5]= {0};
    INT16U    Data;
#endif
    static    INT8U   Rx_num=0;
    static    INT16U  Wait_Syn=TRUE;
    Int32U        IIRValue;
    IIRValue = U1IIR_bit.IID;
#if (LOGO_FLAG == MSW_LOGO)
    if(IIRValue == 0x01)
    {
        if(Tx_num < 8)
        {
            U1THR = Usart1_Tx_Buffer[Tx_num];
            Tx_num++;
        }
        else
        {
            if(Inquire_TXD_Handler(USART1)!=0)
            {
                USART1_TXD_Busy_FLAG=FALSE;
            }
            Tx_num = 0;
        }
    }
    else if(IIRValue == 0x02)
    {
        //receive FIFO full
        data = U1RBR;
         GAS_Comm_Set(0);
         if(Wait_Syn==TRUE)
        {
            if(data=='<')
            {
                Wait_Syn = FALSE;
            }
        }
        else
        {
            if((LastData=='>')&(data=='<'))
            {
                if(AA_Module_Flag.EtCO2ModuleExist == 1)
                {
                    CO2_ModuleDecoding(&Uart1RxBuffer[0],&CO2_Parameters);
                }
                //CO2_ModuleDecoding(&Uart1RxBuffer[0],&CO2_Parameters);
                Rx_num = 0;
                ETCO2_ConnFlag = 0;//国产CO2连接标志

                //if( EtCO2_Config_On && (!(AA_Module_Flag.EtCO2ModuleExist&0x01)) )//选配&最低位没有被置为1
                if((EtCO2_Config_On || EtCO2_KM7002_Config_On) && (!(AA_Module_Flag.EtCO2ModuleExist)))
                {
                    AA_Module_Flag.EtCO2ModuleExist = 1;
                    //alarm_str_init(LanguageType);
                    InitAlarmInfoRegister();
                }
                //Wait_Syn = TRUE;
            }
            else
            {
                if(Rx_num>=11)
                {
                    Rx_num = 0;
                    Wait_Syn = TRUE;
                }
                else
                {
                    Uart1RxBuffer[Rx_num] = data;
                    Rx_num++;
                }
            }
        }
        LastData = data;
    }
    else if(IIRValue == 0x06)//ylk add 20190905 数据超时检测
    {
        data = U1RBR;
    }
#else
    if(IIRValue == 0x02)
    {
        Data = U1RBR;
        if(SpO2_Choose == 1)//国产
        {
            if(Wait_Syn==TRUE)
            {
                if((Data&0x80)!=0)
                {
                    Wait_Syn = FALSE;
                    Rx_num = 0;
                    Uart1RxBuffer[Rx_num] = Data;
                }
            }
            else
            {
                Rx_num++;
                Uart1RxBuffer[Rx_num] = Data;
                if(Rx_num>=4)
                {
                    SPO2_DataDecoding(&SpO2_Parameters, Uart1RxBuffer);
                    if(SpO2_Parameters.HeartBeatFlag)
                    {
                        HeartBeatFlag = 1;
                    }
                    Wait_Syn = TRUE;
                    if(!SpO2ModuleExist)
                    {
                        SpO2ModuleExist = 1;
                    }
                }
            }
        }
        else if(SpO2_Choose == 2)//Masimo
        {
            SPO2ReceiveNormalMode_NormalSpeed(Data);
            if(SPO2CurveReceOK)
            {
                GetWaveDataSmartToneON();
                //                HeartBeatFlag = 1;//djq del 20170325  放在这里如果标志不能及时清除，则HeartBeatFlag可能一直被置位
                if(!SpO2ModuleExist)
                {
                    SpO2ModuleExist = 2;
                }
            }

        }
    }
    else if(IIRValue == 0x01)
    {
    }
    else if(IIRValue == 0x06)//ylk add 20190905 数据超时检测
    {
        Data = U1RBR;
    }
#endif
}

/***************************************************************************************************
* @brief
* @para
* @retval
*/
void Uart2IntHandler(void)
{
    static    INT8U   Uart2RxBuffer[11]= {0};
    static    INT8U   Rx_num=0;
    static    INT8U   Tx_num=0;
    static    INT16U  Wait_Syn=TRUE;
    static    INT8U   LastData=0;
    Int32U IIRValue;
    Int8U  data;
    IIRValue = U2IIR;
    IIRValue = (IIRValue>>1)&0x07;
#if (LOGO_FLAG == MSW_LOGO)
    if(0)//MSW下不再监测国产CO2
#else
    if(AA_Module_Flag.ETCO2_FLAG == 1)
#endif
    {
        if(IIRValue == 0x01)
        {
            if(Tx_num < 8)
            {
                U2THR = Usart2_Tx_Buffer[Tx_num];
                Tx_num++;
            }
            else
            {
                if(Inquire_TXD_Handler(USART2)!=0)
                {
                    USART2_TXD_Busy_FLAG=FALSE;
                }
                Tx_num = 0;
            }
        }
        else if(IIRValue == 0x02)
        {
            //receive FIFO full
            data = U2RBR;
            GAS_Comm_Set(0);
            if(Wait_Syn==TRUE)
            {
                if(data=='<')
                {
                    Wait_Syn = FALSE;
                }
            }
            else
            {
                if((LastData=='>')&(data=='<'))
                {
                    CO2_ModuleDecoding(&Uart2RxBuffer[0],&CO2_Parameters);
                    Rx_num = 0;
                    ETCO2_ConnFlag = 0;//国产CO2连接标志
                    //etco2_runon = 1; //cpp del 20130220
                    if(!(AA_Module_Flag.EtCO2ModuleExist))
                    {
                        AA_Module_Flag.EtCO2ModuleExist = 1;
                        AA_Module_Flag.Module_State_Flag = Connect_Valid;
                        AA_all_data.AA_detail.AA_CO2.Accuracy = ACCURACY_Iso;
                        //zy   2016.6.29
                        if((Masimo_EtCO2_Config_On || EtCO2_Config_On || EtCO2_KM7002_Config_On || Drager_EtCO2_Config_On))
						{
							EtCO2ModuleOff = RTEEPROMread(ETCO2_ON_OFF_ADDR);
						}
						else
						{
							EtCO2ModuleOff = 0;
						}
                        //alarm_str_init(LanguageType);
                        InitAlarmInfoRegister();
                    }
                    //Wait_Syn = TRUE;
                }
                else
                {
                    if(Rx_num>=11)
                    {
                        Rx_num = 0;
                        Wait_Syn = TRUE;
                    }
                    else
                    {
                        Uart2RxBuffer[Rx_num] = data;
                        Rx_num++;
                    }
                }
            }
            LastData = data;
        }
        else if(IIRValue == 0x06)//ylk add 20190905 数据超时检测
        {
            data = U2RBR;
        }
    }
    else if(AA_Module_Flag.ETCO2_FLAG == 0)
    {
        //djq alter 2013-12-23
        if(IIRValue == 0x01)
        {
//            if(Tx_num < 5)
//            {
//                U2THR = Usart2_Tx_Buffer[Tx_num];
//                Tx_num++;
//            }
//            else
//            {
//                if(Inquire_TXD_Handler(USART2)!=0)
//                {
//                    USART2_TXD_Busy_FLAG=FALSE;
//                }
//                Tx_num = 0;
//            }

            if(Usart2_Tx_Buffer[0] == 0xAA)
            {
                if(Tx_num < 4)
                {
                    U2THR = Usart2_Tx_Buffer[Tx_num+1];
                    Tx_num++;
                }
                else
                {
                    if(Inquire_TXD_Handler(USART2)!=0)
                    {
                        USART2_TXD_Busy_FLAG=FALSE;
                    }
                    Tx_num = 0;
                }
            }
            else
            {
                if(Tx_num < Usart2_Tx_Buffer[1]+2)
                {
                    U2THR = Usart2_Tx_Buffer[Tx_num+1];
                    Tx_num++;
                }
                else
                {
                    if(Inquire_TXD_Handler(USART2)!=0)
                    {
                        USART2_TXD_Busy_FLAG=FALSE;
                    }
                    Tx_num = 0;
                }
            }

        }
        else if(IIRValue == 0x02)
        {
            data = U2RBR;
            GAS_Comm_Set(0);
//            Irma_RX_Int(data);
            //AA_RX_Receive指针在使用之前应该先判断，否则在开机未初始化指针之前调用，死机
            if((AA_RX_Receive) && (AA_RX_Receive(data) == IM_OK))
            {
                AA_Module_Flag.EtCO2ModuleExist = 2;
                AA_Module_Flag.Module_State_Flag = Connect_Valid;
                if(AA_Module_Flag.Comm_Count < 50)
                {
                    AA_Module_Flag.Comm_Count = 50;
                }
            }
        }
        else if(IIRValue == 0x06)//ylk add 20190905 数据超时检测
        {
            data = U2RBR;
        }
    }

}

/***************************************************************************************************
* @brief
* @para
* @retval
*/
void Uart3IntHandler(void)
{
    static    INT16U  Uart3RxBuffer[3]= {0};
    static    INT8U   Rx_num=0;
    static    INT8U   Tx_num=0;
    static    INT16U  Wait_Syn=TRUE;
    static    INT16U  LastData=0;
    INT16U    CurrentData;
    Int32U        IIRValue;
    IIRValue = U3IIR_bit.IID;
    if(IIRValue == 0x02)
    {
        if(Wait_Syn==TRUE)
        {
            CurrentData = U3RBR;
            //U2THR = CurrentData;// for testing
            if((LastData=='$') && (CurrentData=='@'))
            {
                Wait_Syn = FALSE;
            }
            LastData = CurrentData;
        }
        else
        {
            Uart3RxBuffer[Rx_num] = U3RBR;
            //U2THR = Uart1RxBuffer[Rx_num];
            Rx_num++;
            if(Rx_num>=3)
            {
                ReceiveDataProcess(Uart3RxBuffer[1]|(Uart3RxBuffer[2]<<8),Uart3RxBuffer[0],USART3);
                Wait_Syn = TRUE;
                Rx_num=0;
                LastData=0;
            }
        }
    }
    else if(IIRValue == 0x01)
    {
        if(Tx_num<4)
        {
            U3THR = Usart3_Tx_Buffer[Tx_num];
            Tx_num++;
        }
        else
        {
            if(Inquire_TXD_Handler(USART3)!=0)
            {
                USART3_TXD_Busy_FLAG=FALSE;
            }
            Tx_num=0;
        }
    }

    else if(IIRValue == 0x06)//ylk add 20190905 数据超时检测
    {
        CurrentData = U3RBR;
    }
}
void Uart4IntHandler(void)
{
  INT16U    CurrentData;
  Int32U    IIRValue;
  IIRValue = U4IIR_bit.IID;
  if(IIRValue == 0x02)
  {
      CurrentData = U4RBR;
      Power_Receive(CurrentData);
  }

}
/***************************************************************************************************
* @brief
* @para
* @retval
*/
INT16U DeviceKeyToGUIKey(INT16U key)
{
    INT8U i;
    for(i=0; i<17; i++)
    {
        if(key==DeviceKeyTable[i])
        {
            break;
        }
    }
    key = GUIKeyTable[i];
    return(key);
}

/***************************************************************************************************
* @brief
* @para
* @retval
*/
void SendSetParameter(int Value,int Id)
{
    if( (Breath_Setting_Tmp.BreathMode == VCV_MODE)
      || (Breath_Setting_Tmp.BreathMode == SIGH_MODE)
      || (Breath_Setting_Tmp.BreathMode == PRVC_MODE)
      || (Breath_Setting_Tmp.BreathMode == SIMV_VCV_MODE)
      || (Breath_Setting_Tmp.BreathMode == SIMV_PRVC_MODE)
      || ((Breath_Setting_Tmp.BreathMode == PSV_MODE) && (Breath_Setting_Tmp.ApnoeaMode == VCV_MODE))
      || (Breath_Setting_Tmp.BreathMode == MANUAL_MODE)
      || (Breath_Setting_Tmp.BreathMode == ACGO_MODE)
      || (Breath_Setting_Tmp.BreathMode == APRV_MODE)   
      || (Breath_Setting_Tmp.BreathMode == BIPAP_MODE)
      || (Breath_Setting_Tmp.BreathMode == VS_MODE)
      )
      {
        switch(Id)
        {
        case GUI_ID_BUTTON0:
            if (Breath_Setting_Tmp.BreathMode == VS_MODE)
			{
				Breath_Setting_Tmp.SettingVtSupportValue = Value;
                if(!PresetMode_Flag)
                {
                    USART0_TXD_RIGSTER |= SEND_SET_VTSUPP_FLAG;
                    USART0_CHECK_RIGSTER |= SEND_SET_VTSUPP_FLAG;
                }                
			}
			else if((Breath_Setting_Tmp.BreathMode == APRV_MODE) || (Breath_Setting_Tmp.BreathMode == BIPAP_MODE))
			{
				Breath_Setting_Tmp.SettingPhighValue = Value;
                if(!PresetMode_Flag)
                {
                    USART0_TXD_RIGSTER |= SEND_SET_PHIGH_FLAG;
                    USART0_CHECK_RIGSTER |= SEND_SET_PHIGH_FLAG;
                }
			}
			else
			{
				Breath_Setting_Tmp.SettingVtValue = Value;
                if(!PresetMode_Flag)
                {
                    USART0_TXD_RIGSTER |= SEND_SET_VT_FLAG;
                    USART0_CHECK_RIGSTER |= SEND_SET_VT_FLAG;
                }
			}                       

            //更改潮气量后，如果还存在 "未达到目标潮气量"报警，则清除
            AlarmsWork.presentAlarms1 &= ~ALAEM_PRVC_VT_LOW;
            AlarmsWork.unclearedAlarms1 &= ~ALAEM_PRVC_VT_LOW;
            break;
        case GUI_ID_BUTTON1:
            if( (Breath_Setting_Tmp.BreathMode == APRV_MODE) || (Breath_Setting_Tmp.BreathMode == BIPAP_MODE) )
			{
				Breath_Setting_Tmp.SettingPlowValue = Value;
                if(!PresetMode_Flag)
                {
                    USART0_TXD_RIGSTER |= SEND_SET_PLOW_FLAG;
                    USART0_CHECK_RIGSTER |= SEND_SET_PLOW_FLAG;
                }
			}
            else if( (Breath_Setting_Tmp.BreathMode == VCV_MODE)
              || (Breath_Setting_Tmp.BreathMode == SIGH_MODE)
              || (Breath_Setting_Tmp.BreathMode == PRVC_MODE)
              || ((Breath_Setting_Tmp.BreathMode == PSV_MODE)&&(Breath_Setting_Tmp.ApnoeaMode == VCV_MODE)) //  20130513
              || (((Breath_Setting_Tmp.BreathMode == MANUAL_MODE)||(Breath_Setting_Tmp.BreathMode == ACGO_MODE))
              && ((BreathMode_BeforeACGO == VCV_MODE)||(BreathMode_BeforeACGO == SIGH_MODE)||(BreathMode_BeforeACGO == PRVC_MODE)
              || ((BreathMode_BeforeACGO == PSV_MODE)&&(Breath_Setting_Tmp.ApnoeaMode == VCV_MODE))))
              )
            {
                Breath_Setting_Tmp.SettingIEValue = Value;
                if(Value >= 6)
                {
//                    MonitorPara.IEValue = (float)10/IE_RatioTable[Value]; //  20130615

                    Value = 10*60*100/((10+IE_RatioTable[Value])*Breath_Setting_Tmp.SettingRateValue*1.0) + 0.5;//IE转换成TI cpp //cpp del 20130306
                    //Value = 10*60*(100-Breath_Setting_Tmp.SettingTpValue)/((10+IE_RatioTable[Value])*Breath_Setting_Tmp.SettingRateValue);//  20130306
                }
                else
                {
//                    MonitorPara.IEValue = (float)(IE_RatioTable[Value]/10); //  20130615

                    Value = IE_RatioTable[Value]*60*100/((10+IE_RatioTable[Value])*Breath_Setting_Tmp.SettingRateValue);//cpp del 20130306
                    //Value = IE_RatioTable[Value]*60*(100-Breath_Setting_Tmp.SettingTpValue)/((10+IE_RatioTable[Value])*Breath_Setting_Tmp.SettingRateValue);//  20130306
                }
                Breath_Setting_Tmp.SettingTiValue = Value;

                if(!PresetMode_Flag)
                {
                    USART0_TXD_RIGSTER |= SEND_SET_TI_FLAG;
                    USART0_CHECK_RIGSTER |= SEND_SET_TI_FLAG;
                }

            }
            else//SIMV-V SIMV-PRVC
            {
                Breath_Setting_Tmp.SettingTiValue = Value;
                if(!PresetMode_Flag)
                {
                    USART0_TXD_RIGSTER |= SEND_SET_TI_FLAG;
                    USART0_CHECK_RIGSTER |= SEND_SET_TI_FLAG;
                }

                //*//  20130615
                Breath_Setting_Tmp.settingTeValue = (6000/Breath_Setting_Tmp.SettingSimvRateValue - Breath_Setting_Tmp.SettingTiValue);
                {
//                    MonitorPara.IEValue = (float)Breath_Setting_Tmp.SettingTiValue / Breath_Setting_Tmp.settingTeValue;
                }//*///
            }
            //*///endof   20130402
            break;
        case GUI_ID_BUTTON2:
            if(Breath_Setting_Tmp.BreathMode == APRV_MODE)
			{
				Breath_Setting_Tmp.SettingThighValue = Value;
                if(!PresetMode_Flag)
                {
                    USART0_TXD_RIGSTER |= SEND_SET_THIGH_FLAG;
                    USART0_CHECK_RIGSTER |= SEND_SET_THIGH_FLAG;
                }                
			} 
			else if(Breath_Setting_Tmp.BreathMode == BIPAP_MODE)
			{               
				Breath_Setting_Tmp.SettingBipapRateValue = Value;
                if(!PresetMode_Flag)
                {
                    USART0_TXD_RIGSTER |= SEND_SET_F_BIPAP_FLAG;
                    USART0_CHECK_RIGSTER |= SEND_SET_F_BIPAP_FLAG;
                }                  
			}
			else if(Breath_Setting_Tmp.BreathMode == VS_MODE)
			{
				Breath_Setting_Tmp.SettingVSRateValue = Value;
                if(!PresetMode_Flag)
                {
                    USART0_TXD_RIGSTER |= SEND_SET_VS_F_FLAG;
                    USART0_CHECK_RIGSTER |= SEND_SET_VS_F_FLAG;
                }                  
			}
            else if((Breath_Setting_Tmp.BreathMode ==SIMV_VCV_MODE)
              ||(Breath_Setting_Tmp.BreathMode ==SIMV_PRVC_MODE)

              ||( ((Breath_Setting_Tmp.BreathMode == MANUAL_MODE)||(Breath_Setting_Tmp.BreathMode == ACGO_MODE))
              &&((BreathMode_BeforeACGO == SIMV_VCV_MODE)||(BreathMode_BeforeACGO == SIMV_PRVC_MODE)) )
                )
            {
                Breath_Setting_Tmp.SettingSimvRateValue = Value;
                Breath_Setting_Tmp.SettingRateValue = Value;
                if(!PresetMode_Flag)
                {
                    USART0_TXD_RIGSTER |= SEND_SET_F_SIMV_FLAG;
                    USART0_CHECK_RIGSTER |= SEND_SET_F_SIMV_FLAG;
                }
            }
            else
            {
                Breath_Setting_Tmp.SettingRateValue = Value;
                Breath_Setting_Tmp.SettingotherRateValue = Value;
                if(!PresetMode_Flag)
                {
                    USART0_TXD_RIGSTER |= SEND_SET_F_FLAG;
                    USART0_CHECK_RIGSTER |= SEND_SET_F_FLAG;
                }

                //*//  20130402 发送完F后，还需再发送Ti
                Value = Breath_Setting_Tmp.SettingIEValue;
                if(Value >= 6)
                {
                    Value = 10*60*100/((10+IE_RatioTable[Value])*Breath_Setting_Tmp.SettingRateValue*1.0) + 0.5;//IE转换成TI cpp //cpp del 20130306
                    //Value = 10*60*(100-Breath_Setting_Tmp.SettingTpValue)/((10+IE_RatioTable[Value])*Breath_Setting_Tmp.SettingRateValue);//  20130306
                }
                else
                {
                    Value = IE_RatioTable[Value]*60*100/((10+IE_RatioTable[Value])*Breath_Setting_Tmp.SettingRateValue);//cpp del 20130306
                    //Value = IE_RatioTable[Value]*60*(100-Breath_Setting_Tmp.SettingTpValue)/((10+IE_RatioTable[Value])*Breath_Setting_Tmp.SettingRateValue);//  20130306
                }
                Breath_Setting_Tmp.SettingTiValue = Value;
                if(!PresetMode_Flag)
                {
                    USART0_TXD_RIGSTER |= SEND_SET_TI_FLAG;
                    USART0_CHECK_RIGSTER |= SEND_SET_TI_FLAG;
                }

                //*///endof   20130402
            }
            break;
        case GUI_ID_BUTTON3:
			if(Breath_Setting_Tmp.BreathMode == APRV_MODE)
			{
				Breath_Setting_Tmp.SettingTlowValue = Value;
                if(!PresetMode_Flag)
                {
                    USART0_TXD_RIGSTER |= SEND_SET_TLOW_FLAG;
                    USART0_CHECK_RIGSTER |= SEND_SET_TLOW_FLAG;
                }                
			}
			else if(Breath_Setting_Tmp.BreathMode == BIPAP_MODE)
			{
				Breath_Setting_Tmp.SettingThighValue = Value;
                if(!PresetMode_Flag)
                {
                    USART0_TXD_RIGSTER |= SEND_SET_THIGH_FLAG;
                    USART0_CHECK_RIGSTER |= SEND_SET_THIGH_FLAG;
                }                 
			} 
			else if(Breath_Setting_Tmp.BreathMode == VS_MODE)
			{
				Breath_Setting_Tmp.SettingPHLimitValue = Value;
                if(!PresetMode_Flag)
                {
                    USART0_TXD_RIGSTER |= SEND_SET_PHL_FLAG;
                    USART0_CHECK_RIGSTER |= SEND_SET_PHL_FLAG;
                }                 
			}             
            else if((Breath_Setting_Tmp.BreathMode==VCV_MODE)||(Breath_Setting_Tmp.BreathMode==SIGH_MODE)
            ||( ((Breath_Setting_Tmp.BreathMode == MANUAL_MODE)||(Breath_Setting_Tmp.BreathMode == ACGO_MODE))
            &&((BreathMode_BeforeACGO == VCV_MODE)||(BreathMode_BeforeACGO == SIGH_MODE)) )
               )
            {
                Breath_Setting_Tmp.SettingTpValue = Value;
                if(!PresetMode_Flag)
                {
                    USART0_TXD_RIGSTER |= SEND_SET_TP_FLAG;
                    USART0_CHECK_RIGSTER |= SEND_SET_TP_FLAG;
                }

                //*//  20130402 发送完Tp后，还需再发送Ti
                Value = Breath_Setting_Tmp.SettingIEValue;
                if(Value >= 6)
                {
                    Value = 10*60*100/((10+IE_RatioTable[Value])*Breath_Setting_Tmp.SettingRateValue*1.0) + 0.5;//IE转换成TI cpp //cpp del 20130306
                    //Value = 10*60*(100-Breath_Setting_Tmp.SettingTpValue)/((10+IE_RatioTable[Value])*Breath_Setting_Tmp.SettingRateValue);//  20130306
                }
                else
                {
                    Value = IE_RatioTable[Value]*60*100/((10+IE_RatioTable[Value])*Breath_Setting_Tmp.SettingRateValue);//cpp del 20130306
                    //Value = IE_RatioTable[Value]*60*(100-Breath_Setting_Tmp.SettingTpValue)/((10+IE_RatioTable[Value])*Breath_Setting_Tmp.SettingRateValue);//  20130306
                }
                Breath_Setting_Tmp.SettingTiValue = Value;
                if(!PresetMode_Flag)
                {
                    USART0_TXD_RIGSTER |= SEND_SET_TI_FLAG;
                    USART0_CHECK_RIGSTER |= SEND_SET_TI_FLAG;
                }

                //*///endof   20130402
            }
            else
            {
                Breath_Setting_Tmp.SettingPsupportValue = Value;
                if(!PresetMode_Flag)
                {
                    USART0_TXD_RIGSTER |= SEND_SET_PSV_FLAG;
                    USART0_CHECK_RIGSTER |= SEND_SET_PSV_FLAG;
                }

                Menu_hWin.InfoWin.InfoChangeFlag |= MODES_INFO_CHANGED_FLAG;
            }
            break;

        case GUI_ID_BUTTON4: //  20130607
            Breath_Setting_Tmp.SettingTslopeValue = Value;
            if(!PresetMode_Flag)
            {
                USART0_TXD_RIGSTER |= SEND_SET_TSLOP_FLAG;
                USART0_CHECK_RIGSTER |= SEND_SET_TSLOP_FLAG;
            }
            break;

        case GUI_ID_BUTTON5: //GUI_ID_BUTTON4 //cpp md 20130607
            if(Breath_Setting_Tmp.TriggerMode==PRESSURE_TRIGGER)
            {
                Breath_Setting_Tmp.SettingPtrValue = Value;
                if(!PresetMode_Flag)
                {
                    USART0_TXD_RIGSTER |= SEND_SET_PT_TRIG_FLAG;
                    USART0_CHECK_RIGSTER |= SEND_SET_PT_TRIG_FLAG;
                }

            }
            else
            {
                Breath_Setting_Tmp.SettingFtrValue = Value;
                if(!PresetMode_Flag)
                {
                    USART0_TXD_RIGSTER |= SEND_SET_FLOW_TRIG_FLAG;
                    USART0_CHECK_RIGSTER |= SEND_SET_FLOW_TRIG_FLAG;
                }

            }
            break;
        case GUI_ID_BUTTON6: //GUI_ID_BUTTON5 cpp md 20130607
            if(Value < 3)
            {
                Value = 0;
            }
            Breath_Setting_Tmp.SettingPEEPValue = Value;
            if(!PresetMode_Flag)
            {
                USART0_TXD_RIGSTER |= SEND_SET_PEEP_FLAG;
                USART0_CHECK_RIGSTER |= SEND_SET_PEEP_FLAG;
            }

        break;
//        case GUI_ID_BUTTON6: //cpp del 20130607
//            Breath_Setting_Tmp.SettingFiO2Value = Value;
//            USART0_TXD_RIGSTER |= SEND_SET_FIO2_FLAG;
//            break;
        case GUI_ID_BUTTON7: //sun add 20140623
             Breath_Setting_Tmp.EsenseValue = Value;
             if(!PresetMode_Flag)
             {
                 USART0_TXD_RIGSTER |= SEND_SET_ESENSE_FLAG;
                 USART0_CHECK_RIGSTER |= SEND_SET_ESENSE_FLAG;
             }
        break;
        default:
            break;
        }
    }
    else
    if( (Breath_Setting_Tmp.BreathMode == PCV_MODE)
      || (Breath_Setting_Tmp.BreathMode == SIMV_PCV_MODE)
      ||((Breath_Setting_Tmp.BreathMode == PSV_MODE)&&(Breath_Setting_Tmp.ApnoeaMode == PCV_MODE)) //  20130513

      || ((Breath_Setting_Tmp.BreathMode == MANUAL_MODE) || (Breath_Setting_Tmp.BreathMode == ACGO_MODE))
      )
    {
        switch(Id)
        {
        case GUI_ID_BUTTON0:
            Breath_Setting_Tmp.SettingPinspValue = Value;
             if(!PresetMode_Flag)
             {
                USART0_TXD_RIGSTER |= SEND_SET_PCV_FLAG;
                USART0_CHECK_RIGSTER |= SEND_SET_PCV_FLAG;
             }

            break;
        case GUI_ID_BUTTON1:
            if( (Breath_Setting_Tmp.BreathMode == APRV_MODE)
              || (Breath_Setting_Tmp.BreathMode == BIPAP_MODE)

              || ( ((Breath_Setting_Tmp.BreathMode == MANUAL_MODE)||(Breath_Setting_Tmp.BreathMode == ACGO_MODE))
              && ((BreathMode_BeforeACGO == APRV_MODE)||(BreathMode_BeforeACGO == BIPAP_MODE)) )
              )
            {
                Breath_Setting_Tmp.SettingThighValue = Value;
                if(!PresetMode_Flag)
                {
                    USART0_TXD_RIGSTER |= SEND_SET_THIGH_FLAG;
                    USART0_CHECK_RIGSTER |= SEND_SET_THIGH_FLAG;
                }

            }
            else
            if( (Breath_Setting_Tmp.BreathMode == PCV_MODE)
              || ((Breath_Setting_Tmp.BreathMode == PSV_MODE)&&(Breath_Setting_Tmp.ApnoeaMode == PCV_MODE)) //  20130513

              || ( ((Breath_Setting_Tmp.BreathMode == MANUAL_MODE)||(Breath_Setting_Tmp.BreathMode == ACGO_MODE))
              && ((BreathMode_BeforeACGO == PCV_MODE)||((BreathMode_BeforeACGO == PSV_MODE) && (Breath_Setting_Tmp.ApnoeaMode == PCV_MODE))) )
              )
            {
                Breath_Setting_Tmp.SettingIEValue = Value;
                if(Value >= 6)
                {
                    Value = 10*60*100/((10+IE_RatioTable[Value])*Breath_Setting_Tmp.SettingRateValue*1.0) + 0.5;//IE转换成TI cpp
                }
                else
                {
                    Value = IE_RatioTable[Value]*60*100/((10+IE_RatioTable[Value])*Breath_Setting_Tmp.SettingRateValue);
                }
                Breath_Setting_Tmp.SettingTiValue = Value;
                if(!PresetMode_Flag)
                {
                    USART0_TXD_RIGSTER |= SEND_SET_TI_FLAG;
                    USART0_CHECK_RIGSTER |= SEND_SET_TI_FLAG;
                }
            }
            else
            {
                Breath_Setting_Tmp.SettingTiValue = Value;
                if(!PresetMode_Flag)
                {
                    USART0_TXD_RIGSTER |= SEND_SET_TI_FLAG;
                    USART0_CHECK_RIGSTER |= SEND_SET_TI_FLAG;
                }

            }
            //*///endof   20130402
            break;
        case GUI_ID_BUTTON2:
            if((Breath_Setting_Tmp.BreathMode == SIMV_PCV_MODE)

             ||( ((Breath_Setting_Tmp.BreathMode == MANUAL_MODE)||(Breath_Setting_Tmp.BreathMode == ACGO_MODE))
             &&(BreathMode_BeforeACGO == SIMV_PCV_MODE) )
               )
            {
                Breath_Setting_Tmp.SettingSimvRateValue = Value;
                Breath_Setting_Tmp.SettingRateValue = Value;
                if(!PresetMode_Flag)
                {
                    USART0_TXD_RIGSTER |= SEND_SET_F_SIMV_FLAG;
                    USART0_CHECK_RIGSTER |= SEND_SET_F_SIMV_FLAG;
                }

            }
            else
            if((Breath_Setting_Tmp.BreathMode == APRV_MODE)

                ||( ((Breath_Setting_Tmp.BreathMode == MANUAL_MODE)||(Breath_Setting_Tmp.BreathMode == ACGO_MODE))
                &&(BreathMode_BeforeACGO == APRV_MODE) )
                  )
            {
                Breath_Setting_Tmp.SettingTlowValue = Value;
                if(!PresetMode_Flag)
                {
                    USART0_TXD_RIGSTER |= SEND_SET_TLOW_FLAG;
                    USART0_CHECK_RIGSTER |= SEND_SET_TLOW_FLAG;
                }

            }
            else
            if( (Breath_Setting_Tmp.BreathMode == PCV_MODE)
              || ((Breath_Setting_Tmp.BreathMode == PSV_MODE)&&(Breath_Setting_Tmp.ApnoeaMode == PCV_MODE)) //  20130513

              ||( ((Breath_Setting_Tmp.BreathMode == MANUAL_MODE)||(Breath_Setting_Tmp.BreathMode == ACGO_MODE))
              &&(BreathMode_BeforeACGO == PCV_MODE)||((BreathMode_BeforeACGO == PSV_MODE)&&(Breath_Setting_Tmp.ApnoeaMode == PCV_MODE)) )
              )
            {
                Breath_Setting_Tmp.SettingRateValue = Value;
                Breath_Setting_Tmp.SettingotherRateValue = Value;
                if(!PresetMode_Flag)
                {
                    USART0_TXD_RIGSTER |= SEND_SET_F_FLAG;
                    USART0_CHECK_RIGSTER |= SEND_SET_F_FLAG;

                }


                //*//  20130402 发送完频率后，还需再发送吸气时间
                Value = Breath_Setting_Tmp.SettingIEValue;
                if(Value >= 6)
                {
                    Value = 10*60*100/((10+IE_RatioTable[Value])*Breath_Setting_Tmp.SettingRateValue*1.0) + 0.5;//IE转换成TI cpp
                }
                else
                {
                   Value = IE_RatioTable[Value]*60*100/((10+IE_RatioTable[Value])*Breath_Setting_Tmp.SettingRateValue);
                }
                Breath_Setting_Tmp.SettingTiValue = Value;
                if(!PresetMode_Flag)
                {
                    USART0_TXD_RIGSTER |= SEND_SET_TI_FLAG;
                    USART0_CHECK_RIGSTER |= SEND_SET_TI_FLAG;
                }

                //*///endof   20130221
            }
            else
            {
                Breath_Setting_Tmp.SettingBipapRateValue = Value;
                Breath_Setting_Tmp.SettingRateValue = Value;
                if(!PresetMode_Flag)
                {
                    USART0_TXD_RIGSTER |= SEND_SET_F_BIPAP_FLAG;
                    USART0_CHECK_RIGSTER |= SEND_SET_F_BIPAP_FLAG;
                }

            }
            break;
        case GUI_ID_BUTTON3:
            if((Breath_Setting_Tmp.BreathMode!=PCV_MODE)

                ||( ((Breath_Setting_Tmp.BreathMode == MANUAL_MODE)||(Breath_Setting_Tmp.BreathMode == ACGO_MODE))
                &&(BreathMode_BeforeACGO!=PCV_MODE) )
              )
            {
                Breath_Setting_Tmp.SettingPsupportValue = Value;
                if(!PresetMode_Flag)
                {
                    USART0_TXD_RIGSTER |= SEND_SET_PSV_FLAG;
                    USART0_CHECK_RIGSTER |= SEND_SET_PSV_FLAG;
                }

                Menu_hWin.InfoWin.InfoChangeFlag |= MODES_INFO_CHANGED_FLAG;
            }
            break;

        case GUI_ID_BUTTON4: //  20130607
            Breath_Setting_Tmp.SettingTslopeValue = Value;
            if(!PresetMode_Flag)
            {
                USART0_TXD_RIGSTER |= SEND_SET_TSLOP_FLAG;
                USART0_CHECK_RIGSTER |= SEND_SET_TSLOP_FLAG;

            }
            break;

        case GUI_ID_BUTTON5: //GUI_ID_BUTTON4 //cpp md 20130607
            if(Breath_Setting_Tmp.TriggerMode==PRESSURE_TRIGGER)
            {
                Breath_Setting_Tmp.SettingPtrValue = Value;
                if(!PresetMode_Flag)
                {
                    USART0_TXD_RIGSTER |= SEND_SET_PT_TRIG_FLAG;
                    USART0_CHECK_RIGSTER |= SEND_SET_PT_TRIG_FLAG;
                }

            }
            else
            {
                Breath_Setting_Tmp.SettingFtrValue = Value;
                if(!PresetMode_Flag)
                {
                    USART0_TXD_RIGSTER |= SEND_SET_FLOW_TRIG_FLAG;
                    USART0_CHECK_RIGSTER |= SEND_SET_FLOW_TRIG_FLAG;
                }

            }
            break;
        case GUI_ID_BUTTON6: //GUI_ID_BUTTON5 //cpp md 20130607
            if(Value < 3)
            {
                Value = 0;
            }
             Breath_Setting_Tmp.SettingPEEPValue = Value;
             if(!PresetMode_Flag)
             {
                 USART0_TXD_RIGSTER |= SEND_SET_PEEP_FLAG;
                 USART0_CHECK_RIGSTER |= SEND_SET_PEEP_FLAG;
             }
            break;

        case GUI_ID_BUTTON7:
             Breath_Setting_Tmp.EsenseValue = Value;
             if(!PresetMode_Flag)
             {
                 USART0_TXD_RIGSTER |= SEND_SET_ESENSE_FLAG;
                 USART0_CHECK_RIGSTER |= SEND_SET_ESENSE_FLAG;
             }
            break;

        default:
            break;
        }
    }
    if(USART0_TXD_Busy_FLAG!=TRUE)
    {
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
            BreathPara_Assign(&BeathSettingPara, &Breath_Setting_Tmp);
        }
        Inquire_TXD_Handler(USART0);
    }
#ifdef DEMO  //  [11/8/2022 lyy add]Demo
	if(Demo_Switch)
	{
		Demo_Change_Cycle_A = 1;
	}
#endif
}

/***************************************************************************************************
* @brief
* @para
* @retval
*/
void SendSetModes(int modes)
{
    switch(modes)
    {
    case VCV_MODE:
        USART0_TXD_RIGSTER |= SEND_VCV_RELATED_PARA;
        USART0_CHECK_RIGSTER |= SEND_VCV_RELATED_PARA;
        break;
    case SIGH_MODE:
        USART0_TXD_RIGSTER |= SEND_SIGH_RELATED_PARA;
        USART0_CHECK_RIGSTER |= SEND_SIGH_RELATED_PARA;
        break;
    case PCV_MODE:
        USART0_TXD_RIGSTER |= SEND_PCV_RELATED_PARA;
        USART0_CHECK_RIGSTER |= SEND_PCV_RELATED_PARA;
        break;
    case PRVC_MODE:
        USART0_TXD_RIGSTER |= SEND_PRVC_RELATED_PARA;
        USART0_CHECK_RIGSTER |= SEND_PRVC_RELATED_PARA;

    case SIMV_VCV_MODE:
        USART0_TXD_RIGSTER |= SEND_SIMV_VCV_RELATED_PARA;
        USART0_CHECK_RIGSTER |= SEND_SIMV_VCV_RELATED_PARA;
        break;
    case SIMV_PCV_MODE:
        USART0_TXD_RIGSTER |= SEND_SIMV_PCV_RELATED_PARA;
        USART0_CHECK_RIGSTER |= SEND_SIMV_PCV_RELATED_PARA;
        break;
    case SIMV_PRVC_MODE:
        USART0_TXD_RIGSTER |= SEND_SIMV_PRVC_RELATED_PARA;
        USART0_CHECK_RIGSTER |= SEND_SIMV_PRVC_RELATED_PARA;
        break;

    case PSV_MODE:
        USART0_TXD_RIGSTER |= SEND_PSV_RELATED_PARA;
        USART0_CHECK_RIGSTER |= SEND_PSV_RELATED_PARA;
        if(BeathSettingPara.ApnoeaMode == PCV_MODE) //BackUpMode  //cpp md 20130513
        {
            USART0_TXD_RIGSTER |= SEND_SET_PCV_FLAG;
            USART0_CHECK_RIGSTER |= SEND_SET_PCV_FLAG;
        }
        else
        {
            USART0_TXD_RIGSTER |= SEND_SET_VT_FLAG;
            USART0_CHECK_RIGSTER |= SEND_SET_VT_FLAG;
        }
        break;
    case BACKUP_MODE: //sun add 20141224
        USART0_TXD_RIGSTER |= SEND_BACKUP_MODE_FLAG;
        USART0_CHECK_RIGSTER |= SEND_BACKUP_MODE_FLAG;
        break;
        
    case BIPAP_MODE:
        USART0_TXD_RIGSTER |= SEND_BIPAP_RELATED_PARA;
        USART0_CHECK_RIGSTER |= SEND_BIPAP_RELATED_PARA;
        break;
    case APRV_MODE:
        USART0_TXD_RIGSTER |= SEND_APRV_RELATED_PARA;
        USART0_CHECK_RIGSTER |= SEND_APRV_RELATED_PARA;
        break;
    case VS_MODE:
        USART0_TXD_RIGSTER |= SEND_VS_RELATED_PARA;
        USART0_CHECK_RIGSTER |= SEND_VS_RELATED_PARA;
        break;
        
    //*//  20130704
#ifndef MANNUL_SWITCH
    case MANUAL_MODE:
        USART0_TXD_RIGSTER |= SEND_MANUAL_RELATED_PARA;
        USART0_CHECK_RIGSTER |= SEND_MANUAL_RELATED_PARA;
        break;
#endif
    //*/
    default:
        break;
    }

    if(BeathSettingPara.TriggerMode==FLOW_TRIGGER)
    {
        USART0_TXD_RIGSTER |= SEND_SET_FLOW_TRIG_FLAG;
        USART0_CHECK_RIGSTER |= SEND_SET_FLOW_TRIG_FLAG;
    }
    else
    {
        USART0_TXD_RIGSTER |= SEND_SET_PT_TRIG_FLAG;
        USART0_CHECK_RIGSTER |= SEND_SET_PT_TRIG_FLAG;
    }
    if(USART0_TXD_Busy_FLAG==FALSE)
    {
        Inquire_TXD_Handler(USART0);
    }
}

/***************************************************************************************************
* @brief
* @para
* @retval
*/
void SendOperationMessage(Int8U type,INT8U UsartPort)
{
    if(UsartPort == USART0)
    {
        switch(type)
        {
        case ASK_VERSIONFOR_BDU_COM:
            USART0_TXD_RIGSTER |= SEND_ASK_VERSION_FORBDU;
            USART0_CHECK_RIGSTER |= SEND_ASK_VERSION_FORBDU;
            if(USART0_TXD_Busy_FLAG==FALSE)
            {
                Inquire_TXD_Handler(USART0);
            }
            break;
        case UPDATE_COMMAND://程序升级命令，暂时不加到重复发送列表中。  //sun add 20140514
            USART0_TXD_RIGSTER |= SEND_UPDATEPROGRAM_FLAG;
            if(USART0_TXD_Busy_FLAG==FALSE)
            {
                Inquire_TXD_Handler(USART0);
            }
        break;
        case SEND_PH_LIMIT:
            USART0_TXD_RIGSTER |= SEND_SET_PHL_FLAG;
            USART0_CHECK_RIGSTER |= SEND_SET_PHL_FLAG; //djq add 20140312
            if(USART0_TXD_Busy_FLAG==FALSE)
            {
                Inquire_TXD_Handler(USART0);
            }
			break;
		case TRIGGER_UPDATE:
#ifdef DEMO//  [11/8/2022 lyy add]Demo
			if (!Demo_Switch)
#endif
			{
				if(BeathSettingPara.TriggerMode==PRESSURE_TRIGGER)
				{
					USART0_TXD_RIGSTER |=SEND_SET_PT_TRIG_FLAG|SEND_TRIG_ACTIVE_FLAG;
					USART0_CHECK_RIGSTER |= SEND_SET_PT_TRIG_FLAG|SEND_TRIG_ACTIVE_FLAG; //djq add 20140312
				}
				else if(BeathSettingPara.TriggerMode==FLOW_TRIGGER)
				{
					USART0_TXD_RIGSTER |=SEND_SET_FLOW_TRIG_FLAG|SEND_TRIG_ACTIVE_FLAG;
					USART0_CHECK_RIGSTER |= SEND_SET_FLOW_TRIG_FLAG|SEND_TRIG_ACTIVE_FLAG; //djq add 20140312
				}
				if(USART0_TXD_Busy_FLAG==FALSE)
				{
					Inquire_TXD_Handler(USART0);
				}
			}
			break;
        //*//  20130402
        case SIGH_TIMES_UPDATE:
            if(BeathSettingPara.BreathMode == SIGH_MODE)//VCV_MODE //cpp md 20130528
            {
                USART0_TXD_RIGSTER |= SEND_SIGH_TIME_FLAG;
                USART0_CHECK_RIGSTER |= SEND_SIGH_TIME_FLAG; //djq add 20140312
            }
            if(USART0_TXD_Busy_FLAG==FALSE)
            {
                Inquire_TXD_Handler(USART0);
            }
            break;
        //*/
		case EXIT_STANDBY:
#ifdef DEMO//  [11/8/2022 lyy add]Demo
			if (!Demo_Switch)
#endif
			{
				USART0_TXD_RIGSTER |= (SEND_RUN_STANDBY_FLAG)|(SEND_SET_PHL_FLAG);
				USART0_CHECK_RIGSTER |= (SEND_RUN_STANDBY_FLAG)|(SEND_SET_PHL_FLAG); //djq add 20140312
				SendSetModes(BeathSettingPara.BreathMode);
			}
			break;
		case ENTER_STANDBY:
			USART0_TXD_RIGSTER |= SEND_RUN_STANDBY_FLAG;
			USART0_CHECK_RIGSTER |= SEND_RUN_STANDBY_FLAG; //djq add 20140312
			if(USART0_TXD_Busy_FLAG==FALSE)
			{
				Inquire_TXD_Handler(USART0);
			}
			break;
		case PRESSURE_CALIBRATION:
            USART0_TXD_RIGSTER |= SEND_PRESSURE_CALIBRATION_FLAG;
            USART0_CHECK_RIGSTER |= SEND_PRESSURE_CALIBRATION_FLAG; //djq add 20140312
            if(USART0_TXD_Busy_FLAG==FALSE)
            {
                Inquire_TXD_Handler(USART0);
            }
            break;
        case FLOW_CALIBRATION:
            USART0_TXD_RIGSTER |= SEND_FLOW_CALIBRATION_FLAG;
            USART0_CHECK_RIGSTER |= SEND_FLOW_CALIBRATION_FLAG; //djq add 20140312
            if(USART0_TXD_Busy_FLAG==FALSE)
            {
                Inquire_TXD_Handler(USART0);
            }
            break;
        case OXY_CALIBRATION:
            USART0_TXD_RIGSTER |= SEND_OXY_CALIBRATION_FLAG;
            USART0_CHECK_RIGSTER |= SEND_OXY_CALIBRATION_FLAG; //djq add 20140312
            if(USART0_TXD_Busy_FLAG==FALSE)
            {
                Inquire_TXD_Handler(USART0);
            }
            break;
        case EXP_VALVE_CALIBRATION:
            USART0_TXD_RIGSTER |= SEND_EXP_VALVE_CALIBRATION_FLAG;
            USART0_CHECK_RIGSTER |= SEND_EXP_VALVE_CALIBRATION_FLAG; //djq add 20140312
            if(USART0_TXD_Busy_FLAG==FALSE)
            {
                Inquire_TXD_Handler(USART0);
            }
            break;
        case INSP_VALVE_CALIBRATION_FACTORY:
            USART0_TXD_RIGSTER |= SEND_INSP_VALVE_CALIBRATION_FACTORY_FLAG;
            USART0_CHECK_RIGSTER |= SEND_INSP_VALVE_CALIBRATION_FACTORY_FLAG; //djq add 20140312
            if(USART0_TXD_Busy_FLAG==FALSE)
            {
                Inquire_TXD_Handler(USART0);
            }
            break;
        case EXP_FLOW_SENSOR_CALIBRATION:
            USART0_TXD_RIGSTER |= SEND_EXP_FLOW_SENSOR_CALIBRATION_FLAG;
            USART0_CHECK_RIGSTER |= SEND_EXP_FLOW_SENSOR_CALIBRATION_FLAG; //djq add 20140312
            if(USART0_TXD_Busy_FLAG==FALSE)
            {
                Inquire_TXD_Handler(USART0);
            }
            break;
        case AIR_FAILURE_INFORM:
        case AIR_NORMAL_INFORM:
            USART0_TXD_RIGSTER |= SEND_AIR_SCANT_FLAG;
            USART0_CHECK_RIGSTER |= SEND_AIR_SCANT_FLAG; //djq add 20140312
            if(USART0_TXD_Busy_FLAG==FALSE)
            {
                Inquire_TXD_Handler(USART0);
            }
            break;
        case OXY_FAILURE_INFORM:
        case OXY_NORMAL_INFORM:
            USART0_TXD_RIGSTER |= SEND_O2_SCANT_FLAG;
            USART0_CHECK_RIGSTER |= SEND_O2_SCANT_FLAG; //djq add 20140312
            if(USART0_TXD_Busy_FLAG==FALSE)
            {
                Inquire_TXD_Handler(USART0);
            }
            break;
        case APNEA_INFORM:
            USART0_TXD_RIGSTER |= SEND_APNEA_FLAG;
            USART0_CHECK_RIGSTER |= SEND_APNEA_FLAG; //djq add 20140312
            if(USART0_TXD_Busy_FLAG==FALSE)
            {
                Inquire_TXD_Handler(USART0);
            }
            break;
        case INSP_HOLD_START:
        case INSP_HOLD_END:
            USART0_TXD_RIGSTER |= SEND_INSP_HOLD_FLAG;
            USART0_CHECK_RIGSTER |= SEND_INSP_HOLD_FLAG; //djq add 20140312
            if(USART0_TXD_Busy_FLAG==FALSE)
            {
                Inquire_TXD_Handler(USART0);
            }
            break;
        case EXP_HOLD_START:
        case EXP_HOLD_END:
            USART0_TXD_RIGSTER |= SEND_EXP_HOLD_FLAG;
            USART0_CHECK_RIGSTER |= SEND_EXP_HOLD_FLAG; //djq add 20140312
            if(USART0_TXD_Busy_FLAG==FALSE)
            {
                Inquire_TXD_Handler(USART0);
            }
            break;
        case SUCTION_ACTIVE:
        case SUCTION_CANCEL:
            USART0_TXD_RIGSTER |= SEND_SUCTION_FLAG;
            USART0_CHECK_RIGSTER |= SEND_SUCTION_FLAG; //djq add 20140312
            if(USART0_TXD_Busy_FLAG==FALSE)
            {
                Inquire_TXD_Handler(USART0);
            }
            break;
        case MANUAL_INSP_START:
            USART0_TXD_RIGSTER |= SEND_MANUAL_INSP_FLAG;
            USART0_CHECK_RIGSTER |= SEND_MANUAL_INSP_FLAG; //djq add 20140312
            if(USART0_TXD_Busy_FLAG==FALSE)
            {
                Inquire_TXD_Handler(USART0);
            }
            break;
        case BAROMETER_SEND:  //djq add 20140305  发送大气压力传感器开关，以及使用地大气压力
            USART0_TXD_RIGSTER |= SEND_BAROMETER_FLAG;
            USART0_CHECK_RIGSTER |= SEND_BAROMETER_FLAG; //djq add 20140312
            if(USART0_TXD_Busy_FLAG==FALSE)
            {
                Inquire_TXD_Handler(USART0);
            }
            break;
        case BAROMETER_STAND_SEND://djq add 20140305  发送大气压力传感器校验地的大气压力
            USART0_TXD_RIGSTER |= SEND_BAROMETER_STAND_FLAG;
            USART0_CHECK_RIGSTER |= SEND_BAROMETER_STAND_FLAG; //djq add 20140312
            if(USART0_TXD_Busy_FLAG==FALSE)
            {
                Inquire_TXD_Handler(USART0);
            }
            break;
        case DIRVER_GAS_FOR_BDU_COM://zkq add 20170407
            USART0_TXD_RIGSTER |= SEND_DIRVER_GAS_FORBDU;
            USART0_CHECK_RIGSTER |= SEND_DIRVER_GAS_FORBDU; //djq add 20140312
            if(USART0_TXD_Busy_FLAG==FALSE)
            {
              Inquire_TXD_Handler(USART0);
            }
          break;

        case NPLSWITCH_SEND_COM://肺复张
            USART0_TXD_RIGSTER |= SEND_NPLSWITCH_FLAG;
#ifdef USART0_TXDACK
            USART0_CHECK_RIGSTER |= SEND_NPLSWITCH_FLAG;
#endif
            if(USART0_TXD_Busy_FLAG==FALSE)
            {
                Inquire_TXD_Handler(USART0);
            }
            break;
        case LUNGPAWTIME_SEND_COM: //肺复张压力和时间
            USART0_TXD_RIGSTER |= SEND_LUNGPAWTIME_FLAG;
#ifdef USART0_TXDACK
            USART0_CHECK_RIGSTER |= SEND_LUNGPAWTIME_FLAG;
#endif
            if(USART0_TXD_Busy_FLAG==FALSE)
            {
                Inquire_TXD_Handler(USART0);
            }
            break; 
            
        default:
            {
            }
        }
    }
#if (LOGO_FLAG == MSW_LOGO)
    else if(UsartPort == USART1)
    {
        switch(type)
        {
        case CO2_PUMPRUNHOUR:
            USART1_TXD_RIGSTER |= (1<<CO2_PUMPRUNHOUR);
            if(USART1_TXD_Busy_FLAG==FALSE)
            {
                Inquire_TXD_Handler(USART1);
            }
            break;
        case CO2_BTPSCOMPENSATE:
            USART1_TXD_RIGSTER |= (1<<CO2_BTPSCOMPENSATE);
            if(USART1_TXD_Busy_FLAG==FALSE)
            {
                Inquire_TXD_Handler(USART1);
            }
            break;
        case CO2_O2COMPENSATE:
            USART1_TXD_RIGSTER |= (1<<CO2_O2COMPENSATE);
            if(USART1_TXD_Busy_FLAG==FALSE)
            {
                Inquire_TXD_Handler(USART1);
            }
            break;
        case CO2_PUMPOCCLUSION:
            USART1_TXD_RIGSTER |= (1<<CO2_PUMPOCCLUSION);
            if(USART1_TXD_Busy_FLAG==FALSE)
            {
                Inquire_TXD_Handler(USART1);
            }
            break;
        case CO2_MODULEONOFF:
            USART1_TXD_RIGSTER |= (1<<CO2_MODULEONOFF);
            if(USART1_TXD_Busy_FLAG==FALSE)
            {
                Inquire_TXD_Handler(USART1);
            }
            break;
        case CO2_MODULEON:
            USART1_TXD_RIGSTER |= (1<<CO2_MODULEON);
            if(USART1_TXD_Busy_FLAG==FALSE)
            {
                Inquire_TXD_Handler(USART1);
            }
            break;
        case CO2_MODULEOFF:
            USART1_TXD_RIGSTER |= (1<<CO2_MODULEOFF);
            if(USART1_TXD_Busy_FLAG==FALSE)
            {
                Inquire_TXD_Handler(USART1);
            }
            break;
        }
    }
#endif
    else if(UsartPort == USART2)
    {
#if (LOGO_FLAG == MSW_LOGO)
        if(0)
#else
        if(AA_Module_Flag.ETCO2_FLAG == 1)
#endif
        {
            switch(type)
            {
            case CO2_PUMPRUNHOUR:
                USART2_TXD_RIGSTER |= (1<<CO2_PUMPRUNHOUR);
                if(USART2_TXD_Busy_FLAG==FALSE)
                {
                    Inquire_TXD_Handler(USART2);
                }
                break;
            case CO2_BTPSCOMPENSATE:
                USART2_TXD_RIGSTER |= (1<<CO2_BTPSCOMPENSATE);
                if(USART2_TXD_Busy_FLAG==FALSE)
                {
                    Inquire_TXD_Handler(USART2);
                }
                break;
            case CO2_O2COMPENSATE:
                USART2_TXD_RIGSTER |= (1<<CO2_O2COMPENSATE);
                if(USART2_TXD_Busy_FLAG==FALSE)
                {
                    Inquire_TXD_Handler(USART2);
                }
                break;
            case CO2_PUMPOCCLUSION:
                USART2_TXD_RIGSTER |= (1<<CO2_PUMPOCCLUSION);
                if(USART2_TXD_Busy_FLAG==FALSE)
                {
                    Inquire_TXD_Handler(USART2);
                }
                break;
            case CO2_MODULEONOFF:
                USART2_TXD_RIGSTER |= (1<<CO2_MODULEONOFF);
                if(USART2_TXD_Busy_FLAG==FALSE)
                {
                    Inquire_TXD_Handler(USART2);
                }
                break;
            case CO2_MODULEON:
                USART2_TXD_RIGSTER |= (1<<CO2_MODULEON);
                if(USART2_TXD_Busy_FLAG==FALSE)
                {
                    Inquire_TXD_Handler(USART2);
                }
                break;
            case CO2_MODULEOFF:
                USART2_TXD_RIGSTER |= (1<<CO2_MODULEOFF);
                if(USART2_TXD_Busy_FLAG==FALSE)
                {
                    Inquire_TXD_Handler(USART2);
                }
                break;
            }
        }
        else if(AA_Module_Flag.ETCO2_FLAG == 0)//Masimo IRMA/ISA
        {
            AA_Module_SendOperationMessage(type);

//            switch(type)
//            {
//            case MODULESWITCH_FLAG:
//                USART2_TXD_RIGSTER |= SEND_MODULE_SWITCH_FLAG;
//                if(USART2_TXD_Busy_FLAG==FALSE)
//                {
//                    Inquire_TXD_Handler(USART2);
//                }
//                break;
//            case CALZERO_FLAG:
//                USART2_TXD_RIGSTER |= SEND_CAL_ZERO_FLAG;
//                if(USART2_TXD_Busy_FLAG==FALSE)
//                {
//                    Inquire_TXD_Handler(USART2);
//                }
//                break;
//            case SET_AA_OPERATE_STANDBY:
//                USART2_TXD_RIGSTER |= SEND_AA_OPERATE_STANDBY;
//                if(USART2_TXD_Busy_FLAG==FALSE)
//                {
//                    Inquire_TXD_Handler(USART2);
//                }
//              break;
//            case SET_AA_OPERATE_MEASURE:
//                USART2_TXD_RIGSTER |= SEND_AA_OPERATE_MEASURE;
//                if(USART2_TXD_Busy_FLAG==FALSE)
//                {
//                    Inquire_TXD_Handler(USART2);
//                }
//              break;
//            case SET_AA_PUMPON:
//                USART2_TXD_RIGSTER |= SEND_AA_PUMPON;
//                if(USART2_TXD_Busy_FLAG==FALSE)
//                {
//                    Inquire_TXD_Handler(USART2);
//                }
//              break;
//            case SET_AA_PUMPOFF:
//                USART2_TXD_RIGSTER |= SEND_AA_PUMPOFF;
//                if(USART2_TXD_Busy_FLAG==FALSE)
//                {
//                    Inquire_TXD_Handler(USART2);
//                }
//              break;
//            case SET_AA_IRO2DELAY:
//                USART2_TXD_RIGSTER |= SEND_AA_IRO2DELAY;
//                if(USART2_TXD_Busy_FLAG==FALSE)
//                {
//                    Inquire_TXD_Handler(USART2);
//                }
//              break;
//            case SET_AA_SETO2OPTION:
//                USART2_TXD_RIGSTER |= SEND_AA_SETO2OPTION;
//                if(USART2_TXD_Busy_FLAG==FALSE)
//                {
//                    Inquire_TXD_Handler(USART2);
//                }
//              break;
//            case SET_AA_SETO2SENSORTYPE:
//                USART2_TXD_RIGSTER |= SEND_AA_SETO2SENSORTYPE;
//                if(USART2_TXD_Busy_FLAG==FALSE)
//                {
//                    Inquire_TXD_Handler(USART2);
//                }
//              break;
//            case SET_AA_ZEROCAL:
//                USART2_TXD_RIGSTER |= SEND_AA_ZEROCAL;
//                if(USART2_TXD_Busy_FLAG==FALSE)
//                {
//                    Inquire_TXD_Handler(USART2);
//                }
//              break;
//            case SET_AA_SPANCALO2:
//                USART2_TXD_RIGSTER |= SEND_AA_SPANCALO2;
//                if(USART2_TXD_Busy_FLAG==FALSE)
//                {
//                    Inquire_TXD_Handler(USART2);
//                }
//              break;
//            case SET_AA_SPANCAL:
//                USART2_TXD_RIGSTER |= SEND_AA_SPANCAL;
//                if(USART2_TXD_Busy_FLAG==FALSE)
//                {
//                    Inquire_TXD_Handler(USART2);
//                }
//              break;
//                default:
//                {
//                }
//            }
        }
    }

    else if(UsartPort == USART3)
    {
        switch(type)
        {
        case UPDATE_COMMAND://程序升级命令，暂时不加到重复发送列表中。  //sun add 20140514
            USART3_TXD_RIGSTER |= U3SEND_UPDATEPROGRAM_FLAG;
            if(USART3_TXD_Busy_FLAG==FALSE)
            {
                Inquire_TXD_Handler(USART3);
            }
        break;
        case LED_UPDATE_OPERATION:
            USART3_TXD_RIGSTER |= SEND_LED_UPDATE_FLAG;
            if(USART3_TXD_Busy_FLAG==FALSE)
            {
                Inquire_TXD_Handler(USART3);
            }
            break;
        case ALARM_PRIORITY_CHANGED:
            USART3_TXD_RIGSTER |= SEND_ALARM_UPDATE_FLAG;
            if(USART3_TXD_Busy_FLAG==FALSE)
            {
                Inquire_TXD_Handler(USART3);
            }
            break;
            //*//  20130402
        case BUZZER_CONTROL_OPERATION:
            USART3_TXD_RIGSTER |= SEND_BUZZER_CONTROL_FLAG;
            if(USART3_TXD_Busy_FLAG==FALSE)
            {
                Inquire_TXD_Handler(USART3);
            }
            break;
        case BUZZER_VOLUME_SETTING:
            USART3_TXD_RIGSTER |= SEND_BUZZER_VOLUME_FLAG;
            if(USART3_TXD_Busy_FLAG==FALSE)
            {
                Inquire_TXD_Handler(USART3);
            }
            break;
        case BUZZER_VOLUME_TEST:
            USART3_TXD_RIGSTER |= SEND_BUZZER_TEST_FLAG;
            if(USART3_TXD_Busy_FLAG==FALSE)
            {
                Inquire_TXD_Handler(USART3);
            }
            break;
        //*//  20130402
        case BUZZER_VOLUME_START:
            USART3_TXD_RIGSTER |= SEND_BUZZER_START_FLAG;
            if(USART3_TXD_Busy_FLAG==FALSE)
            {
                Inquire_TXD_Handler(USART3);
            }
            break;
        case  SEND_MACHINE_MODE_FLAG:
            USART3_TXD_RIGSTER |= U3SEND_MACHINE_MODE_FLAG;
            if(USART3_TXD_Busy_FLAG==FALSE)
            {
                Inquire_TXD_Handler(USART3);
            }
            break;            
        }//*/
    }
}

void InitTrendQueue_Alias(WM_HWIN hProgBar)
{
    InitTrendQueue(hProgBar);
}


void SpO2_Chose(void)
{
    if((Masimo_SpO2_Config_On)&&(SpO2_Choose == 2))//Masimo
    {
        Masimo_Buffer = malloc(sizeof(signed char)*(TRACINGCURVE_DATALEN_DEFAULT+1));
        if(Masimo_Buffer == NULL)
        {
            GUI_Delay(100);
            Masimo_Buffer = malloc(sizeof(signed char)*(TRACINGCURVE_DATALEN_DEFAULT+1));
            if(Masimo_Buffer == NULL)
            {
                SpO2ModuleExist = 0;
                SpO2_Choose = 0;
            }
        }
        //如果读取错误，设置默认值
        Masimo_BeeperSwitch = RTEEPROMread(MASIMO_BEEPER_ADDR);
        if(Masimo_BeeperSwitch)
        {
            Masimo_BeeperSwitch = 1;
            RTEEPROMwrite(MASIMO_BEEPER_ADDR,Masimo_BeeperSwitch);
        }
        Masimo_FastSATSwitch = RTEEPROMread(MASIMO_FAST_SAT_ADDR);
        if(!Masimo_FastSATSwitch)
        {
            Masimo_FastSATSwitch = 0;
            RTEEPROMwrite(MASIMO_FAST_SAT_ADDR,Masimo_FastSATSwitch);
        }
        Masimo_SmartToneSwitch = RTEEPROMread(MASIMO_SMART_TONE_ADDR);
        if(Masimo_SmartToneSwitch)
        {
            Masimo_SmartToneSwitch = 1;
            RTEEPROMwrite(MASIMO_SMART_TONE_ADDR,Masimo_SmartToneSwitch);
        }
        Masimo_Sensitivity = RTEEPROMread(MASIMO_SENSITIVITY_ADDR);
        if((Masimo_Sensitivity > 2)||(!Masimo_Sensitivity))
        {
            Masimo_Sensitivity = 2;//默认为APOD
            RTEEPROMwrite(MASIMO_SENSITIVITY_ADDR,Masimo_Sensitivity);
        }
        Masimo_AverageTime = RTEEPROMread(MASIMO_AVERAGE_TIME_ADDR);
        if(Masimo_AverageTime > 7)
        {
            Masimo_AverageTime = 2;//默认为2s
            RTEEPROMwrite(MASIMO_AVERAGE_TIME_ADDR,Masimo_AverageTime);
        }

        InitialiazeSPO2Varity();
    }
}



