#include "error_app.h"
#include "Alarm_handle.h"//2013-12-03
#include "Static_Menu_Creat.h"//2013-12-03





/*
*外部变量声明
*/
extern struct list_node AlarmRecord;
extern ALARM_RECORD_LIST AlarmRecordList;

extern RTC_TIMER RTCReadTime(void);


/***********************************************************
 *						函数声明
************************************************************
*/
void Get_Error_Number(U64 error_nomber ,U8 H_or_L);
static void _Error_Handing(U64 error_nomber ,U8 H_or_L);
void Error_Handing_H(U64 error_nomber);
void Error_Handing_L(U64 error_nomber);

/***********************************************************
 *函数名：Save_ErrorNumber_Reault
 *参数  ：Save_Offset_Addr：项目存储的偏移地址，作为标志位
          error_number:错误编码
          Result_Value：检测结果，数据  
 *返回值：无
 *被调用位置：每一项检测完成后调用
 *调用函数：
 *功能  ：将当前错误代码和结果保存到相应的变量当中,并且将输入的错误编码变量清空
************************************************************
*/
void Save_ErrorNumber_Reault(U8 Save_Offset_Addr,U64 *error_number,U16 Result_Value)
{
    //无论是状态型还是数据型，都把结果写到里边，读取的时候，如果是状态型，则不读取数据
    switch(Save_Offset_Addr)
    {
        case MEM_MANUAL_OFFSET_ADDR:
            CONTEXT_APP.CheckInfo.Manual.Error_Code.Bit_8 = (*error_number)&0xff;
            CONTEXT_APP.CheckInfo.Manual.Check_result[1] = ((CONTEXT_APP.Checking_State&0x0f) << 4) | ((Result_Value & 0x0f00) >> 8);
            CONTEXT_APP.CheckInfo.Manual.Check_result[0] = 0;
            break;
        case MEM_BAROMETER_OFFSET_ADDR://djq add 20160809
            CONTEXT_APP.CheckInfo.Barometer.Error_Code.Bit_8 = (*error_number)&0xff;
            CONTEXT_APP.CheckInfo.Barometer.Check_result[1] = ((CONTEXT_APP.Checking_State&0x0f) << 4) | ((Result_Value & 0x0f00) >> 8);
            CONTEXT_APP.CheckInfo.Barometer.Check_result[0] = Result_Value & 0x00ff;
            break;
        case MEM_GASO2_OFFSET_ADDR:
            CONTEXT_APP.CheckInfo.GasO2.Error_Code.Bit_8 = (*error_number)&0xff;
            CONTEXT_APP.CheckInfo.GasO2.Check_result[1] = ((CONTEXT_APP.Checking_State&0x0f) << 4) | ((Result_Value & 0x0f00) >> 8);
            CONTEXT_APP.CheckInfo.GasO2.Check_result[0] = Result_Value & 0x00ff;
            break;
        case MEM_GASN2O_OFFSET_ADDR:
            CONTEXT_APP.CheckInfo.GasN2O.Error_Code.Bit_8 = (*error_number)&0xff;
            CONTEXT_APP.CheckInfo.GasN2O.Check_result[1] = ((CONTEXT_APP.Checking_State&0x0f) << 4) | ((Result_Value & 0x0f00) >> 8);//数据类型结果
            CONTEXT_APP.CheckInfo.GasN2O.Check_result[0] =  Result_Value & 0x00ff;
            break;
        case MEM_GASAIR_OFFSET_ADDR:
            CONTEXT_APP.CheckInfo.GasAir.Error_Code.Bit_8 = (*error_number)&0xff;
            CONTEXT_APP.CheckInfo.GasAir.Check_result[1] = ((CONTEXT_APP.Checking_State&0x0f) << 4) | ((Result_Value & 0x0f00) >> 8);
            CONTEXT_APP.CheckInfo.GasAir.Check_result[0] = Result_Value & 0x00ff;
            break;
        case MEM_FLOW_OFFSET_ADDR:
            CONTEXT_APP.CheckInfo.Flow_S.Error_Code.Bit_8 = (*error_number)&0xff;
            CONTEXT_APP.CheckInfo.Flow_S.Check_result[1] = ((CONTEXT_APP.Checking_State&0x0f) << 4);//状态类型的结果   | (0x0 << 4)
            CONTEXT_APP.CheckInfo.Flow_S.Check_result[0] = 0;
            break;
        case MEM_PRESSURE_OFFSET_ADDR:
            CONTEXT_APP.CheckInfo.Pressure_S.Error_Code.Bit_8 = (*error_number)&0xff;
            CONTEXT_APP.CheckInfo.Pressure_S.Check_result[1] = ((CONTEXT_APP.Checking_State&0x0f) << 4);//状态类型的结果   | (0x0 << 4)
            CONTEXT_APP.CheckInfo.Pressure_S.Check_result[0] = 0;
            break;
        //case MEM_FG_FLOW_OFFSET_ADDR:
        //    CONTEXT_APP.CheckInfo.FG_Flow_S.Error_Code.Bit_8 = (*error_number)&0xff;
        //    CONTEXT_APP.CheckInfo.FG_Flow_S.Check_result[1] = ((CONTEXT_APP.Checking_State&0x0f) << 4);//状态类型的结果
        //    CONTEXT_APP.CheckInfo.FG_Flow_S.Check_result[0] = 0;
        //    break;
        case MEM_COMPLIANCE_OFFSET_ADDR:
            CONTEXT_APP.CheckInfo.Compliance.Error_Code.Bit_8 = (*error_number)&0xff;
            CONTEXT_APP.CheckInfo.Compliance.Check_result[1] = ((CONTEXT_APP.Checking_State&0x0f) << 4) | ((Result_Value & 0x0f00) >> 8);
            CONTEXT_APP.CheckInfo.Compliance.Check_result[0] = Result_Value & 0x00ff;
            break;
        case MEM_INTERNAL_LEAKAGE_OFFSET_ADDR:
            CONTEXT_APP.CheckInfo.Internal_Leakage.Error_Code.Bit_8 = (*error_number)&0xff;
            CONTEXT_APP.CheckInfo.Internal_Leakage.Check_result[1] = ((CONTEXT_APP.Checking_State&0x0f) << 4) | ((Result_Value & 0x0f00) >> 8);
            CONTEXT_APP.CheckInfo.Internal_Leakage.Check_result[0] = Result_Value & 0x00ff;
            break;
        case MEM_BATTERY_OFFSET_ADDR:
            CONTEXT_APP.CheckInfo.Battery.Error_Code.Bit_8 = (*error_number)&0xff;
            CONTEXT_APP.CheckInfo.Battery.Check_result[1] = ((CONTEXT_APP.Checking_State&0x0f) << 4);//状态类型的结果
            CONTEXT_APP.CheckInfo.Battery.Check_result[0] = 0;
            break;
        case MEM_OXYGEN_OFFSET_ADDR:
            CONTEXT_APP.CheckInfo.Oxygen.Error_Code.Bit_8 = (*error_number)&0xff;
            CONTEXT_APP.CheckInfo.Oxygen.Check_result[1] = ((CONTEXT_APP.Checking_State&0x0f) << 4);//状态类型的结果   | (0x0 << 4)
            CONTEXT_APP.CheckInfo.Oxygen.Check_result[0] = 0;
            break;
        //djq del 20160809
        //case MEM_PEEP_OFFSET_ADDR:
        //    CONTEXT_APP.CheckInfo.PEEP_Valve.Error_Code.Bit_8 = (*error_number)&0xff;
        //    CONTEXT_APP.CheckInfo.PEEP_Valve.Check_result[1] = ((CONTEXT_APP.Checking_State&0x0f) << 4) | ((Result_Value & 0x0f00) >> 8);
        //    CONTEXT_APP.CheckInfo.PEEP_Valve.Check_result[0] = 0;
        //    break;
        //case MEM_SAFETY_OFFSET_ADDR:
        //    CONTEXT_APP.CheckInfo.Safety_Valve.Error_Code.Bit_8 = (*error_number)&0xff;
        //    CONTEXT_APP.CheckInfo.Safety_Valve.Check_result[1] = ((CONTEXT_APP.Checking_State&0x0f) << 4) | ((Result_Value & 0x0f00) >> 8);
        //    CONTEXT_APP.CheckInfo.Safety_Valve.Check_result[0] = 0;
        //    break;
        //case MEM_POP_OFF_OFFSET_ADDR:
        //    CONTEXT_APP.CheckInfo.Pop_off_Valve.Error_Code.Bit_8 = (*error_number)&0xff;
        //    CONTEXT_APP.CheckInfo.Pop_off_Valve.Check_result[1] = ((CONTEXT_APP.Checking_State&0x0f) << 4) | ((Result_Value & 0x0f00) >> 8);
        //    CONTEXT_APP.CheckInfo.Pop_off_Valve.Check_result[0] = 0;
        //    break;
        //case MEM_ACGO_OFFSET_ADDR:
        //    CONTEXT_APP.CheckInfo.ACGO_Switch.Error_Code.Bit_8 = (*error_number)&0xff;
        //    CONTEXT_APP.CheckInfo.ACGO_Switch.Check_result[1] = ((CONTEXT_APP.Checking_State&0x0f) << 4);//状态类型的结果
        //    CONTEXT_APP.CheckInfo.ACGO_Switch.Check_result[0] = 0;
        //    break;
        default:
            break;
    }
    *error_number = 0;//清空输入的错误编码
}
/***********************************************************
 *函数名：Get_Error_Number
 *参数  ：错误代码和错误的级别（级别ERROR_FLAG_H，ERROR_FLAG_L）
 *返回值：无
 *被调用位置：
 *调用函数：Memory_write()
 *功能  ：将当前错误代码保存到存储器
************************************************************
*/
void Get_Error_Number(U64 error_number ,U8 H_or_L) //2013-12-03
{        
    CONTEXT_APP.Current_ErrorCode = error_number;
}

/*
 *函数名：_Error_Handing
 *参数  ：错误代码和错误的级别（级别ERROR_FLAG_H，ERROR_FLAG_L）
 *返回值：无
 *被调用位置：Error_Handing_H（）、Error_Handing_L（）
 *功能  ：将当前错误代码保存到存储器，并且在适当的位置显示
*/
static void _Error_Handing(U64 error_nomber ,U8 H_or_L)
{
	Get_Error_Number(error_nomber, H_or_L);
}
/***********************************************************
 *函数名：Error_Handing_H
 *参数  ：高级别的错误代码
 *返回值：无
 *被调用位置：
 *功能  ：将当前错误代码保存到存储器，并且在适当的位置显示
************************************************************
*/
void Error_Handing_H(U64 error_nomber)
{
	_Error_Handing(error_nomber, ERROR_FLAG_H);
    CONTEXT_APP.AlarmHigh_Flag = 1;
}

/***********************************************************
 *函数名：Error_Handing_L
 *参数  ：低级别的错误代码
 *返回值：无
 *被调用位置：
 *功能  ：将当前错误代码保存到存储器，并且在适当的位置显示
************************************************************
*/
void Error_Handing_L(U64 error_nomber)
{
	_Error_Handing(error_nomber, ERROR_FLAG_L);
}
