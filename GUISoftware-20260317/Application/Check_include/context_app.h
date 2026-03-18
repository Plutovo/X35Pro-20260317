#ifndef _CONTEXT_APP_H_
#define _CONTEXT_APP_H_

#include "system_app.h"

//检测项目的结果存储变量
typedef struct 
{
    U8 Check_result[2];//其中Check_result[0]中为数据的低位，Check_result[1]中低4位为数据位。数据=Check_result[1]<<8 | Check_result[0]
                       //Check_result[1]中高4位为状态位
                       //分为两个字节方便存储
    union 
    {
        U8  Bit_8;
        U64 Bit_64;//为内部测试准备，内部测试的错误信息比较多
    }Error_Code;
}CHECK_RESULT_S;
//最终检测结果统计,各种状态（pass、fail、skip）出现的次数。
typedef struct 
{
    U8 _Manual[3];
    U8 Automatic[3];
}RESULT_REPORT_S;

//检测结果存储结构
typedef struct
{
    RESULT_REPORT_S Result_Report;
    CHECK_RESULT_S Manual;
    CHECK_RESULT_S Barometer;
    CHECK_RESULT_S GasAir;
    CHECK_RESULT_S GasN2O;
    CHECK_RESULT_S GasO2;
    CHECK_RESULT_S Flow_S;
    CHECK_RESULT_S Pressure_S;
    CHECK_RESULT_S FG_Flow_S;
    CHECK_RESULT_S Compliance;
    CHECK_RESULT_S Internal_Leakage;
    CHECK_RESULT_S Oxygen;
    CHECK_RESULT_S Battery;
    //CHECK_RESULT_S PEEP_Valve;
    //CHECK_RESULT_S Safety_Valve;
    //CHECK_RESULT_S Pop_off_Valve;
    //CHECK_RESULT_S ACGO_Switch;
}CHECK_INFO_S;
//环境变量，保存各种全局信息。Checking_Position变量的值 INTERNAL_LEAKAGE_ID
//djq modify 20160809  修改自检项目     POS_PEEP_Valve,  POS_Safety_Valve,POS_Pop_off_Valve,POS_ACGO,
typedef enum {  POS_Manual  ,   POS_Barometer,     POS_GasSypply,         POS_Flow_S  ,   POS_Pressure_S,    
                POS_Oxygen,     POS_Compliance,    POS_Circuit_Leakage,     POS_Battery,   
                POS_ERROR = 0xff} PositionFlag;//用于Checking_Position变量  
typedef enum {
                Ctrl_Choose,        //选择手动机控界面
                Ctrl_ManualPrompt,  //手动提示界面
                Ctrl_ManualTest,    //手动检测界面
                Ctrl_ManualFinish,  //手动检测完成
                Ctrl_AutoPrompt1,   //自动提示界面1
                Ctrl_AutoTest1,      //自动检测界面
                Ctrl_AutoFinish1,     //自动完成
                Ctrl_AutoPrompt2,   //自动提示界面2
                Ctrl_AutoTest2,      //自动检测界面
                Ctrl_AutoFinish2,     //自动完成
                Ctrl_AutoPrompt3,   //自动提示界面2
                Ctrl_AutoTest3,      //自动检测界面
                Ctrl_AutoFinish3,     //自动完成
            }ControlFlag;//正在检测的状态：djq add 20161018 
//用于Gas_Flag变量
#define GAS_SUPPLY_O2         (0)
#define GAS_SUPPLY_N2O        (1)
#define GAS_SUPPLY_AIR        (2)
//用于Update_Flag变量，分辨左右使用 
#define UPDATE_NONE_ITEM      (0x00)//不更新
#define UPDATE_LEFT_POS       (0x01)//更新左侧位置信息
#define UPDATE_RIGHT_POS      (0x02)//更新右侧位置信息
#define UPDATE_TEST_COMMAND   (0x04)//发送命令,一般在更新位置或者发送命令超时后，设置为该状态
#define UPDATE_LEFT_FINISH    (0x08)//左侧完成
#define UPDATE_RIGHT_FINISH   (0x10)//右侧完成
#define UPDATE_HIDE_HANDLE    (0x20)
//用于ActiveWindow_Flag变量
#define ACTIVEWINDOW_NONE     (0x0)//当前没有提示窗口
#define ACTIVEWINDOW_SHOW     (0x1)//显示了提示窗口
#define ACTIVEWINDOW_HIDE     (0x2)//应该隐藏提示窗口
//用于LeftOrRight_Flag
#define NONE_CHECK_FLAG       (0x0)//没有检测
#define LEFT_CHECK_FLAG       (0x1)//设备检测
#define RIGHT_CHECK_FLAG      (0x2)//回路检测
/////////////////////////////////////////所有变量都应该加入保护措施，避免资源冲突。
typedef	struct
{
	U64 Current_ErrorCode;//当前的Error_Code
	U32 Callback_Return;  //当前窗口的回调函数的按键输入返回值(按键的ID)，每次被读出时将该变量赋值为0xff
    ControlFlag  Checking_Control;//正在检测的状态：选择手动机控界面、手动提示界面、手动检测界面、自动提示界面1、自动提示界面2、自动检测界面。
	PositionFlag Checking_Position;//正在检测的位置（显示的位置）
	U8	Checking_State;	  //正在检测项目的状态(0xff初始状态,用低4位表示,其中：高2位	 0检测成功  1检测失败 2忽略 3跳过) 在串口程序和Disp_Result程序中设置状态
                                                //                            低2为  00：状态型结果   01：数据型结果
                                              
                                                //     成功         失败          忽略          跳过
                                                //0000 0000    0000 0100     0000 1000     0000 1100  状态型
                                                //0000 0001    0000 0101     0000 1001     0000 1101  数据型
    U8  Pause_Flag;        // 检测暂停标志    1:当弹出提示界面后，暂停所有操作，等待用户选择      0:用户选择完成后继续检测
    U8  Cancel_Flag;       // 取消检测标志位， (0为初始状态，1为取消检测)
    U8  Global_CancelFlag; // 全局取消标志，用于一次性退出所有的自检项目。该标志置位的时候，不向串口发送任何命令。
    U8  AlarmHigh_Flag;    // 产生了高级报警，将在Disp_Result()函数中终止程序(死循环)   暂时没有用到
    U8  Update_Flag;        // 刷新标志，在串口接收和超时的地方置位，更新完成后清除
    U16 Result;            // 在传口中接收到的结果
    U8  Left_CheckItem_Num;  //左侧校验项目个数
    U8  Total_CheckItem_Num; //全部校验项目个数
    U8  ActiveWindow_Flag;   //自检中显示的提示窗口的状态标志
    U8  LeftOrRight_Flag;    //当前检测项目标志
	int TimeOutNum;          //超时时间，单位为ms
    CHECK_INFO_S CheckInfo; //检测结果
	U8  O2TestPrompt_Flag;	//是否开启氧浓度自检提示标志 0:不显示 1:显示
}CONTEXT_S;

typedef struct 
{
	U8 STUpdate_Flag;
	U8 O2SenTest_Flag;
	U8 O2SensorConn_Flag;
	U8 AltO2SW_Flag;
}ITEM_PROMPT;

extern CONTEXT_S CONTEXT_APP;//环境变量，定义在context.c中。






//自检流程切换函数，相关变量为Checking_Position、Update_Flag
void Context_NextLeft(void);
void Context_NextRight(void);
void Context_RetestItem(void);
//ActiveWindow_Flag变量相关函数
void Context_SetWindow_None(void);
void Context_SetWindow_Show(void);
void Context_SetWindow_Hide(void);
U8   Context_IsNodeWindow(void);
U8   Context_IsShowWindow(void);
U8   Context_IsHideWindow(void);
//LeftOrRight_Flag变量相关函数
void Context_SetNoneCheckFlag(void);
void Context_SetLeftCheckFlag(void);
void Context_SetRightCheckFlag(void);
U8   Context_IsNoneCheckFlag(void);
U8   Context_IsLeftCheckFlag(void);
U8   Context_IsRightCheckFlag(void);
//UpdateFlag标志操作
ControlFlag Context_SetControlFlag(ControlFlag Control_Flag);
int Context_SetUpdateFlag(int Update_Flag);
int Context_GetUpdateFlag(void);
//Position标志操作
PositionFlag Context_SetPosition(PositionFlag Pos);
PositionFlag Context_AddPosition();
PositionFlag Context_GetPosition(void);

void Context_SetO2PromptFlag(U8 flag);
U8 Context_GetO2PromptFlag(void);

void PromptSet_O2SenTestFlag(U8 data);
U8 PromptGet_O2SenTestFlag(void);
void PromptSet_STUpdateFlag(U8 data);
U8 PromptGet_STUpdateFlag(void);
void PromptSet_ALTO2Flag(U8 data);
U8 PromptGet_ALTO2Flag(void);
unsigned short Context_GetResult(void);

#endif  //_CONTEXT_APP_H_