#ifndef _SERIAL_APP_H_
#define _SERIAL_APP_H_

#include "system_app.h"






/***********************************************************
           用于发送自检命令的索引，充当队列使用
************************************************************
*/
//BDU命令个数
#define TEST_COMMUNICATION_NUM            (17)

#define TEST_COM_STARTTEST                0x0000000000000001
#define TEST_COM_MANUAL                   0x0000000000000002
#define TEST_COM_BAROMETER                0x0000000000000004//djq add 20160809
#define TEST_COM_AIRSUPPLY                0x0000000000000008
#define TEST_COM_N2OSUPPLY                0x0000000000000010
#define TEST_COM_O2SUPPLY                 0x0000000000000020
#define TEST_COM_OXYGEN_SENSOR            0x0000000000000040
#define TEST_COM_FLOW_SENSOR_ZERO         0x0000000000000080
#define TEST_COM_PRESS_SENSOR_ZERO        0x0000000000000100
#define TEST_COM_FLOW_SENSOR              0x0000000000000200
#define TEST_COM_PRESS_SENSOR             0x0000000000000400
#define TEST_COM_PEEP_VALVE               0x0000000000000800
#define TEST_COM_SAFETY_VALVE             0x0000000000001000
#define TEST_COM_POP_OFF_VALVE            0x0000000000002000
#define TEST_COM_CIRCUIT_COMPLIANCE       0x0000000000004000
#define TEST_COM_CIRCUIT_LEAKAGE          0x0000000000008000

#define TEST_COM_FINISHTEST               0x0000000000010000//这是最后一个，以后加入命令都要在他的前面

/***********************************************************
 *函数名：Test_TXD_Handle
 *参数  ：目标串口标志位
 *返回值：0：还有命令未发送   1：所有命令发送完成。
 *被调用位置：Inquiry_TXD_Handle()
 *功能  ：向目标串口发送自检中使用的命令
************************************************************
*/
U8 Test_TXD_Handle(U8 UsartPort);

/***********************************************************
 *函数名：Serial_Communaction_Handing
 *参数  ：通讯目标编码
 *返回值：返回的结果或者错误编码
 *被调用位置：
 *功能  ：按照目标通讯编码的格式打包数据包，发送数据包，等待
	  接收数据包，超时处理，接受数据包，解析出数据，返回数据
************************************************************
*/
U32 Serial_Communaction_Handing(U32 serial_number);

#endif  //_SERIAL_APP_H_