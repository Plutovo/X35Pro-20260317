#include "Debug_App.h"

DEBUG_APP_S DebugApp_Context = {0};

/***************************************************************************
*函数名：DebugApp_SetDispStr1
*功能：用于设置提示1的显示信息，并且更新显示
*参数：Str:要显示的字符串;Str_Num:显示字符串的长度；
*返回值：显示成功返回最大能够显示的字符串长度值，显示失败返回-1
****************************************************************************
*/
int DebugApp_SetDispStr1(char *Str,int Str_Num)
{
    int ret = -1;//返回-1 为错误类型
    if(DEBUGAPP_STRLEN >= Str_Num)
    {
        strncpy(DebugApp_Context.DebugApp_DispStr1,Str,Str_Num);
        DebugApp_Context.DebugApp_UpFlag = DebugApp_Paint;
        ret = DEBUGAPP_STRLEN;
    }
    return ret;
}
/***************************************************************************
*函数名：DebugApp_SetDispStr2
*功能：用于设置提示1的显示信息，并且更新显示
*参数：Str:要显示的字符串;Str_Num:显示字符串的长度；
*返回值：显示成功返回最大能够显示的字符串长度值，显示失败返回-1
****************************************************************************
*/
int DebugApp_SetDispStr2(char *Str,int Str_Num)
{
    int ret = -1;//返回-1 为错误类型
    if(DEBUGAPP_STRLEN >= Str_Num)
    {
        strncpy(DebugApp_Context.DebugApp_DispStr2,Str,Str_Num);
        DebugApp_Context.DebugApp_UpFlag = DebugApp_Paint;
        ret = DEBUGAPP_STRLEN;
    }
    return ret;
}
/***************************************************************************
*函数名：DebugApp_SetVariable
*功能：用于设置变量数组的值
*参数：Index:数组的索引;Number:数据的值
*返回值：显示成功返回1，显示失败返回0
****************************************************************************
*/
int DebugApp_SetVariable(int Index,int Number)
{
    int ret = 0;//返回-1 为错误类型
    if(DEBUGAPP_NUM >= Index)
    {
        DebugApp_Context.DebugApp_Variable[Index] = Number;
        DebugApp_Context.DebugApp_UpFlag = DebugApp_Paint;
        ret = 1;
    }
    return ret;
}

/***************************************************************************
*函数名：DebugApp_AddVariable
*功能：用于设置变量数组的值
*参数：Index:数组的索引;Number:数据的值
*返回值：显示成功返回1，显示失败返回0
****************************************************************************
*/
int DebugApp_AddVariable(int Index)
{
    int ret = 0;//返回-1 为错误类型
    if(DEBUGAPP_NUM >= Index)
    {
        DebugApp_Context.DebugApp_Variable[Index]++;
        DebugApp_Context.DebugApp_UpFlag = DebugApp_Paint;
        ret = 1;
    }
    return ret;
}
/***************************************************************************
*函数名：DebugApp_SubVariable
*功能：用于设置变量数组的值
*参数：Index:数组的索引;Number:数据的值
*返回值：显示成功返回1，显示失败返回0
****************************************************************************
*/
int DebugApp_SubVariable(int Index)
{
    int ret = 0;//返回-1 为错误类型
    if(DEBUGAPP_NUM >= Index)
    {
        DebugApp_Context.DebugApp_Variable[Index]--;
        DebugApp_Context.DebugApp_UpFlag = DebugApp_Paint;
        ret = 1;
    }
    return ret;
}
/***************************************************************************
*函数名：DebugApp_IsPaint
*功能：用于判断当前是否可以显示Debug字符串
*返回值：可以显示返回值为1，不可以显示返回值为0
****************************************************************************
*/
int DebugApp_IsPaint(void)
{
    int ret = 0;
    if(DebugApp_Context.DebugApp_UpFlag == DebugApp_Paint)
    {
        ret = 1;
    }
    return ret;
}
