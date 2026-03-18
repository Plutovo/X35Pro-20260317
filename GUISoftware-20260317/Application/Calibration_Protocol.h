#ifndef _CALIBRATION_PROTOCOL_H_
#define _CALIBRATION_PROTOCOL_H_

#include "Alarm_Handle.h"

/********************************************************************************************************
*
*                         宏定义
*
*********************************************************************************************************
*/

/*
程序中的限制值
*/
#define DATA_PACKAGE_MAXBYTE    (250) //每个数据包最大能包含的字节数
#define DATA_BUFFER_LEN         (255) //数据buffer定义大小
#define ASK_BUFFER_LEN          (5)   //应答buffer定义大小



////////////////////////////////////////////帧识别符
#define CALI_CMD             0xCC //命令帧
#define CALI_DATA            0x55 //数据帧
#define CALI_ASK             0xAA //应答帧


////////////////////////////////////////////命令（CMD）
/*
命令编码及命令 Ask编码
*/
#define ASK_CMD_BASE                 (0xC0)
#define ASK_CMD_UPLOADING_DEVICE_ID  (ASK_CMD_BASE | 0x1) //命令设备上传自己的设备号      
#define ASK_CMD_UPLOADING_CALI_DATA  (ASK_CMD_BASE | 0x2) //命令设备上传校验数据    
#define ASK_CMD_DOWNLOAD_CALI_DATA   (ASK_CMD_BASE | 0x3) //命令设备接受校验数据（下载校验数据到设备）   

/*
 数据Ask编码
*/
#define ASK_DATA_BASE               (0x50)
#define ASK_DATA_OK                 (ASK_DATA_BASE | 0x1)//当前数据包正确，请求发送下一包
#define ASK_DATA_RESEND             (ASK_DATA_BASE | 0x2)//PC（或者设备）接受到的数据校验和错误，申请重新发送
#define ASK_DATA_ERR                (ASK_DATA_BASE | 0x3)//设备检测到当前项目的数据错误或者没有数据，停止数据传输


////////////////////////////////////////////附加数据（ADDITION）
/*
    设备号编码 DEV

    设备号格式：
                高3位：    表示设备的类型（呼吸、麻醉、麻呼……）
                低5位：    表示该设备的外设链接情况（按位计算，最多5种）
                           bit 0b 1  1     1     1     1
                                     EGM   EFM   BDU   GUI
*/
#define ADDI_DEV_CRITICAL_CARE              (0x1 << 5)//呼吸机
#define ADDI_DEV_ANESTHETIC_WORKSTATION     (0x2 << 5)//麻醉机
#define ADDI_DEV_ANESTHETIC_MACHINE         (0x3 << 5)//麻醉呼吸机

/*
    校验类型编码 CALI
*/
#define ADDI_CALI_TYPE_BASE      0x00 //校验类型基址
#define ADDI_CALI_BATTERY        (ADDI_CALI_TYPE_BASE | 0x01)//电池校验
//define ADDITION_XXXXXX_CALI    

/*
    错误类型编码 ERR
*/
#define ADDI_ERR_TYPE_BASE       0x01 //错误类型基址
#define ADDI_ERR_STANDBY         (ADDI_ERR_TYPE_BASE | 0x01)//工作状态下无法连接
#define ADDI_ERR_CALI_PROCESS    (ADDI_ERR_TYPE_BASE | 0x02)//工作状态下无法连接
#define ADDI_ERR_DISABLE         (ADDI_ERR_TYPE_BASE | 0x03)//无效的校验类型


/********************************************************************************************************
*
*                         结构构造
*
*********************************************************************************************************
*/
//数据包的格式
//帧头（2Byte）+识别符（1Byte）+包编号（1Byte）+包数据长度（1Byte）     +数据（1~250Byte）+校验（1Byte）
//------------------------ASK_Buffer------------------------------      ----------DataBuffer-----------
//------------------------信息部分--------------------------------      -------数据部分---------
typedef struct
{
    Int8U PackageTotal;//本次发送总的数据包个数（不包含头包）
    Int8U PackageNum;//当前数据包的编号
    Int8U PackageLen;//当前数据包中数据的总字节数,不包含校验位
    Int8U DataBuffer[DATA_BUFFER_LEN];//数据buffer
}CALI_PROTOCOL_DATA_S;

/*控制的过程：*/
typedef enum {
	             Idle_State,            //空闲状态
	             Send_DevType ,         //1、发送设备类型
                 Send_UploadingData,    //2、发送上传的数据
			     Recv_UploadingAsk,     //3、接受上传的数据应答
				 Send_DownloadCmdAsk,   //4、发送下载数据命令的应答
				 Send_DownloadDataAsk,  //5、发送下载数据的应答
				 Recv_DownloadData_Start,//6、接收到第一包信息数据
			     Recv_DownloadData,      //6、接收下载的数据
				 Recv_DownloadData_End,  //7、接收完成
             } CaliCtrl;
typedef struct
{
	Int8U CMD_Type;       //命令的附加编码
	Int8U CMD_Data;       //命令的附加数据
    
	Int8U ASK_Type;       //应答的附加编码
	Int8U ASK_Data;       //应答的附加数据
	Int8U ASK_Buffer[ASK_BUFFER_LEN];
    
    Int8U RXD_Busy;       //接收功能忙碌中
    Int8U TXD_Busy;       //发送功能忙碌中
	CaliCtrl NextStep;    //下一步
	CALI_PROTOCOL_DATA_S Cali_Data_S;//
}CALI_PROTOCOL_CTRL_S;//协议控制


/********************************************************************************************************
*
*                         函数声明
*
*********************************************************************************************************
*/

void SerialPort_RXD_Analysis(Int8U data);
/*
CalibrationProcess_Ctrl:根据协议控制结构的内容执行。
在主函数的INFO_WINDOW_UPDATE任务中调用
*/ 
void CalibrationProcess_Ctrl(void);


#endif //_CALIBRATION_PROTOCOL_H_
