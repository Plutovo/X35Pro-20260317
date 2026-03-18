#ifndef _ERROR_TEST_APP_H_
#define _ERROR_TEST_APP_H_

#include "system_app.h"

//呼吸机头文件
#include "static_menu_creat.h"
#include "alarm_handle.h"

//错误代码的级别
#define ERROR_FLAG_H 0
#define ERROR_FLAG_L 1

#define ERROR_STRING_SIZE	100		//故障提示内容的大小



//错误代码显示内容的结构体，可以根据错误代码查询显示内容。
typedef struct{
	U8 error_number;
	U8 error_string_chinese[ERROR_STRING_SIZE];
	U8 error_string_english[ERROR_STRING_SIZE];       
}Error_Flag_S;



/***********************************************************
 *函数名：Save_ErrorNumber_Reault
 *参数  ：Save_Offset_Addr：项目存储的偏移地址，作为标志位
          error_number:错误编码
          Result_Value：检测结果，数据  
 *返回值：无
 *被调用位置：每一项检测完成后调用
 *调用函数：
 *功能  ：将当前错误代码和结果保存到相应的变量当中
************************************************************
*/
void Save_ErrorNumber_Reault(U8 Save_Offset_Addr,U64 *error_number,U16 Result_Value);


void Get_Error_Number(U64 error_nomber ,U8 H_or_L);
void Error_Handing_L(U64 error_nomber);
void Error_Handing_H(U64 error_nomber);


#endif  //_ERROR_APP_H_