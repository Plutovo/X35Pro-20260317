#ifndef _DEBUG_APP_H_
#define _DEBUG_APP_H_

#define DEBUGAPP_STRLEN   100
#define DEBUGAPP_NUM      10

typedef enum  {DebugApp_NoPaint,DebugApp_Paint} DEBUGAPP_UPFLAG_TYPE;

typedef struct
{
    char DebugApp_DispStr1[DEBUGAPP_STRLEN];
    char DebugApp_DispStr2[DEBUGAPP_STRLEN];
    int  DebugApp_Variable[DEBUGAPP_NUM];
    unsigned int DebugApp_Flag;
    DEBUGAPP_UPFLAG_TYPE DebugApp_UpFlag;
}DEBUG_APP_S;

extern DEBUG_APP_S DebugApp_Context;

/***************************************************************************
*函数名：DebugApp_SetVariable
*功能：用于设置变量数组的值
*参数：Index:数组的索引;Number:数据的值
*返回值：显示成功返回1，显示失败返回0
****************************************************************************
*/
int DebugApp_SetVariable(int Index,int Number);
int DebugApp_AddVariable(int Index);
int DebugApp_SubVariable(int Index);


#if defined(IAR_LPC_1788_SK)//IAR环境

#else//模拟软件环境
    
#endif

#endif