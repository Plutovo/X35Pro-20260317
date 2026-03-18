/*
*
*
*最后修改日期：2013-11-26
*/

#ifndef _PROTOCOL_APP_H_
#define _PROTOCOL_APP_H_
#include "FRAM_Addr.h"
/*
#include "display.h"
#include "error.h"
#include "serial.h"
#include "context.h"
*/

//#ifndef I8
#define I8    signed char
#define U8  unsigned char     /* unsigned 8  bits. */
#define I16   signed short    /*   signed 16 bits. */
#define U16 unsigned short    /* unsigned 16 bits. */
#define I32   signed long   /*   signed 32 bits. */
#define U32 unsigned long   /* unsigned 32 bits. */
#define U64 unsigned long long
#define I16P I16              /*   signed 16 bits OR MORE ! */
#define U16P U16              /* unsigned 16 bits OR MORE ! */
//#endif  //I8
//#define _SELF_TEST

//颜色配置
#define Back_Color				    (0x605144)		//桌面窗口背景颜色
#define DialogBk_Color              (GUI_BLACK)     //自检窗体背景颜色
#define Font_Color				    (GUI_WHITE)		//桌面字体颜色
#define Button_0_Font_Color		    (GUI_WHITE)		//按钮弹起字体颜色
#define Button_1_Font_Color		    (GUI_WHITE)		//按钮按下字体颜色
#define Button_0_Bk_Color		    (0xFAEFE6)		//按钮

#define RESULT_PASS_COLOR           (GUI_GREEN)   //检测结果通过颜色
#define RESULT_FAIL_COLOR           (GUI_RED)    //检测结果不通过颜色
#define RESULT_SKIP_COLOR           (GUI_WHITE)  //检测结果跳过颜色
//#define Button_0_Bk_Cancel_Color    ()    //取消按钮背景色
#define Button_Quit_0_Bk_Color      (0xF7D7BD)   
#define Button_1_Bk_Color		    (0x4080FF)		//按钮
#define Framewin_Client_Color	    (0xcdaa94)		//对话框客户区颜色

#define Framewin_Title_Color        (GUI_BLACK)  //对话框标题栏颜色

#define Framewin_Title_Font_Color	(GUI_BLACK)		//对话框标题栏字体颜色
#define CheckResult_Font_Color      (0x858585)      //检测结果字体  银灰色
#define Desktop_Frame_Color         (0x633735)      //主界面边框颜色
#if 1 

//Error Code
    //BDU发送的错误代码
#define MANUAL_CTRLMODE_ERR      0x23 //手动/机控位置故障；

#define PRESSURE_OF_AIR_SUPPLY_OVERFLOW_ERR_L	(0x40)	//空气供应压力不在范围内
#define PRESSURE_OF_O2_SUPPLY_OVERFLOW_ERR_L	(0x41)	//氧气供应压力不在范围内
#define PRESSURE_OF_N2O_SUPPLY_OVERFLOW_ERR_L	(0x42)	//笑气供应压力不在范围内


#define O2_SENSOR_INVALID       (0x05)//传感器失效或者未连接，更换氧浓度传感器
#define O2_SENSOR_FAILURE       (0x0F)//传感器故障
#define F_SENSOR_INSP_TEST_ERR  (0x20)//吸气流量传感器测试错误
#define F_SENSOR_EXP_TEST_ERR   (0x02)//吸气流量传感器测试错误

#define P_SENSOR_TEST_ERR       (0x25)//压力传感器测试错误

#define CIRCUIR_COM_ERR             (0x32)//回路顺应性,顺应性测试错误

#define CIRCUIT_DISCONNECT_ERR    0x2D //管路未连接故障； 手动、顺应性、泄露检测
#define FRESH_VALVE_LEAK_ERR      0x37 //比例阀或新鲜气体阀漏气故障；手动、顺应性、泄露检测

#define LEAKAGE_RATE_HIGH       (0x14)//泄露量大  :手动气密性、回路泄漏量
    //UI自己判断的错误代码，或者通过0x8000得到的错误代码
#define BAROMETER_ERR           (0x50)//大气压力传感器错误，数值超范围


#define GAS_SUPPLY_FAILURE      (0x0A)//气源故障
#define GAS_SUPPLY_LOW          (0x19)//气源压力低
#define SAFETY_VLVE_FAILURE     (0x1E)//安全阀开启压力错误
#define FG_F_SENSOR_LINEAR_ERR  (0x2E)//EGM流量传感器错误
#define COMPLIANCE_HIGH         (0x28)//顺应性错误


#define COMMUNICATION_TIMEOUT_ERR       (0xa1)//通讯超时代码
#define COMMUNICATION_TIMEOUT_MAX       (3)//超时最大次数
//超时时间  Test
//#define MANUAL_TIMEOUT_NUM          1000//(30*1000)//大气压力超时时间
//#define OXYGEN_TIMEOUT_NUM          1000//(30*1000)//氧浓度传感器数据上传等待时间
//#define AIR_SUPPLY_TIMEOUT_NUM	    1000//(15*1000)//气体供应上传等待时间
//#define FS_TIMEOUT_NUM              1000//(30*1000)//流量传感器数据上传等待时间
//#define FG_FS_TIMEOUT_NUM           1000//(30*1000)//新鲜气体流量传感器数据上传等待时间
//#define PS_TIMEOUT_NUM				1000//(30*1000)//压力传感器数据上传等待时间
//#define PPV_TIMEOUT_NUM				1000//(30*1000)//PEEP阀数据上传等待时间
//#define POPV_TIMEOUT_NUM			1000//(30*1000)//Pop-off阀数据上传等待时间
//#define SV_TIMEOUT_NUM				1000//(30*1000)//安全阀数据上传等待时间
//#define CC_TIMEOUT_NUM				1000//(30*1000)//病人呼吸回路数据上传等待时间
//#define CL_TIMEOUT_NUM				1000//(30*1000)//病人呼吸回路数据上传等待时间
//超时时间
#ifndef _SELF_TEST
#define MANUAL_TIMEOUT_NUM          (60*1000)//手动气密性检测超时时间
#define BAROMETER_TIMEOUT_NUM	    (10*1000)//气体供应上传等待时间
#define AIR_SUPPLY_TIMEOUT_NUM	    (15*1000)//气体供应上传等待时间
#define OXYGEN_TIMEOUT_NUM          (120*1000)//氧浓度传感器数据上传等待时间
#define FS_TIMEOUT_NUM              (30*1000)//流量传感器数据上传等待时间
#define FG_FS_TIMEOUT_NUM           (30*1000)//新鲜气体流量传感器数据上传等待时间
#define PS_TIMEOUT_NUM				(30*1000)//压力传感器数据上传等待时间
#define PPV_TIMEOUT_NUM				(30*1000)//PEEP阀数据上传等待时间
#define POPV_TIMEOUT_NUM			(30*1000)//Pop-off阀数据上传等待时间
#define SV_TIMEOUT_NUM				(30*1000)//安全阀数据上传等待时间
#define CC_TIMEOUT_NUM				(60*1000)//病人呼吸回路数据上传等待时间
#define CL_TIMEOUT_NUM				(30*1000)//病人呼吸回路数据上传等待时间
#else
#define MANUAL_TIMEOUT_NUM          (1)//手动气密性检测超时时间
#define BAROMETER_TIMEOUT_NUM	   (1)//气体供应上传等待时间
#define AIR_SUPPLY_TIMEOUT_NUM	    (1)//气体供应上传等待时间
#define OXYGEN_TIMEOUT_NUM         (500)//氧浓度传感器数据上传等待时间
#define FS_TIMEOUT_NUM             (1)//流量传感器数据上传等待时间
#define FG_FS_TIMEOUT_NUM          (1)//新鲜气体流量传感器数据上传等待时间
#define PS_TIMEOUT_NUM				(1)//压力传感器数据上传等待时间
#define PPV_TIMEOUT_NUM				(1)//PEEP阀数据上传等待时间
#define POPV_TIMEOUT_NUM			(1)//Pop-off阀数据上传等待时间
#define SV_TIMEOUT_NUM			(1)//安全阀数据上传等待时间
#define CC_TIMEOUT_NUM				(1)//病人呼吸回路数据上传等待时间
#define CL_TIMEOUT_NUM				(1)//病人呼吸回路数据上传等待时间
#endif
//Communication command  BDU EGM
#define BDU_TEST_START				(0x16)//开始气路控制板检测命令
#define BDU_TEST_RETURN				(0x17)//返回的气路控制板检测数据命令

#define S_SEND_TO_BDU_MAN			(0x21)//开始手动回路检测命令
#define S_RECV_OF_BDU_MAN			(0x22)//返回手动回路监测结果

#define S_SEND_TO_BDU_BARO			(0x24)//开始气压计检测命令
#define S_RECV_OF_BDU_BARO			(0x25)//返回的气压计检测数据命令


#define S_SEND_TO_BDU_AS_AIR		(0x0b)//开始气体供应检测命令
#define S_SEND_TO_BDU_AS_N2O		(0x0c)//开始气体供应检测命令
#define S_SEND_TO_BDU_AS_O2  		(0x0d)//这个不是发送的命令，只是一个标志而已，没有实际意义！！！！！！！
#define S_RECV_OF_BDU_AS_AIR		(0x0c)//返回的氧气气源检测数据命令
#define S_RECV_OF_BDU_AS_N2O		(0x0d)//返回的氧气气源检测数据命令
#define S_RECV_OF_BDU_AS_O2			(0x0e)//返回的空气气源检测数据命令

#define S_SEND_TO_BDU_OXYGEN		(0x4A)//开始氧浓度传感器检测命令
#define S_RECV_OF_BDU_OXYGEN        (0x4B)//返回的氧浓度传感器检测数据命令

#define S_SEND_TO_BDU_FL_Z			(0x28)//开始流量传感器零点检测命令
#define S_RECV_OF_BDU_FL_Z			(0x29)//返回的流量传感器零点检测数据命令

#define S_SEND_TO_BDU_PS_Z			(0x2C)//开始压力传感器零点检测命令
#define S_RECV_OF_BDU_PS_Z			(0x2D)//返回的压力传感器零点检测数据命令

#define S_SEND_TO_BDU_FS			(0x38)//开始流量传感器检测命令
#define S_RECV_OF_BDU_FS			(0x39)//返回的流量感器检测数据命令

#define S_SEND_TO_BDU_PS			(0x32)//开始压力传感器检测命令
#define S_RECV_OF_BDU_PS			(0x33)//返回的压力传感器检测数据命令

#define	S_SEND_TO_BDU_PPV			(0x3B)//开始PEEP阀检测命令
#define S_RECV_OF_BDU_PPV			(0x3C)//返回的PEEP阀检测数据命令

#define	S_SEND_TO_BDU_SV			(0x36)//开始安全阀检测命令
#define S_RECV_OF_BDU_SV			(0x37)//返回的安全阀检测数据命令

#define	S_SEND_TO_BDU_POPV			(0x42)//开始Pop-off阀检测命令
#define S_RECV_OF_BDU_POPV			(0x43)//返回的Pop-off阀检测数据命令

#define S_SEND_TO_BDU_CL			(0x45)//开始回路泄露量
#define S_RECV_OF_BDU_AIRTIGHT	    (0x46)//返回的病人呼吸回路的泄露值

#define S_SEND_TO_BDU_CC			(0x47)//开始回路顺应性
#define S_RECV_OF_BDU_COMPLIANCE    (0x48)//返回的病人呼吸回路的顺应性

//#define S_SEND_TO_UI_ALARM			(0x4b)//开始报警状态检测命令
//#define S_RECV_OF_UI_ALARM			(0x4c)//返回的报警状态检测数据命令
#define S_SEND_TO_UI_ALARM_HIGH     (0x4b)//开始检测高级报警
#define S_SEND_TO_UI_ALARM_LOW      (0x4c)//开始检测低级报警
#define S_SEND_TO_UI_FINISH			(0x4f)//外设板结束标志

#define S_SEND_TO_BDU_FINISH		(0x4d)//气路控制板结束标志
#define	S_SEND_TO_EF_FINISH			(0x4e)//电子流量计版结束标志

//Communication command  EGM
#define S_EGM_COMMAND               (0xCC)//自检命令标识

#define S_SEND_TO_EGM_FL_Z			(0x23)//开始流量传感器零点检测命令
#define S_RECV_OF_EGM_FL_Z			(0x24)//返回的流量传感器零点检测数据命令

#define S_SEND_TO_EGM_FS			(0x25)//开始流量传感器检测命令
#define S_RECV_OF_EGM_FS			(0x26)//返回的流量感器检测数据命令



extern unsigned char MasimoISA_EtCO2_Config_On; //0:关闭   1：开启
extern unsigned char Drager_EtCO2_Config_On;
#endif

#endif  //_PROTOCOL_APP_H_