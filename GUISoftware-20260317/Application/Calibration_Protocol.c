/*
缺陷：接收数据的时候，只要求了第一包数据必须为头数据包，其他数据包没有要求按顺序，
      如果数据包没有按照顺序接收，并没有报错和错误处理。而且一个数据包多次发送也
      能正确接收。送数据的时候也存在数据包顺序的问题。
应该改成：按包的顺序接收，接收成功的数据包不在响应，如果顺序错误应该有提示信息。


缺陷：没有通讯超时功能
应该改成：有超时功能，并且通讯失败应该有次数限制


缺陷：应该在待机下才能够联接，联接之后不能进入工作状态
应该改成：

缺陷：发送和接收同用一个Buffer，有可能在没有发送完成的时候又产生了接收，那么就会冲突。
应该改成：两个buffer或者加入保护标志，避免资源冲突。
*/


#include "Calibration_Protocol.h"
//#include "EXT_UART.h"
#include "MainTask.h"
#include "Static_Menu_Creat.h"
#include "exuart_17xx_40xx.h"

//数据的收发每次都是先低位后高位。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。
/*不同的应用程序独有的内容*/
extern BATTERY_AD_PARA  BatteryPara;
#define BATTERY_CALI_HEAD_LEN   (14+16) //电池校验Cali_Ctrl_S.Cali_Data_S.PackageNum = 1的时候，前面的电池校验时间和报警数值
										//0  1  2  3  4  5  6-7   8-9   10-11 12-13   14-29
										//YY YY MM DD HH MM STEP3 STEP2 LOW   EMPTY   预留
extern void Save_Threth_Alarm_Battery(BATTERY_AD_PARA *Battery_P);//保存从PC下载的电池信息
extern RTC_TIMER RTCTime;



#ifdef __ICCARM__
__no_init CALI_PROTOCOL_CTRL_S Cali_Ctrl_S POOL_EXRAM;
#else
CALI_PROTOCOL_CTRL_S Cali_Ctrl_S;
#endif


void Init_CaliCtrl(void)
{
    memset(&Cali_Ctrl_S, 0, sizeof(Cali_Ctrl_S));
    
	Cali_Ctrl_S.CMD_Type = 0;
	Cali_Ctrl_S.CMD_Data = 0;
	Cali_Ctrl_S.NextStep = Idle_State;

	Cali_Ctrl_S.Cali_Data_S.PackageTotal = 0;
	Cali_Ctrl_S.Cali_Data_S.PackageNum = 0;
	Cali_Ctrl_S.Cali_Data_S.PackageLen = 0;
}

void SerialPort_Send_Ask(char AskNum,char Addition)
{
    Cali_Ctrl_S.ASK_Buffer[0] = 0x24;
    Cali_Ctrl_S.ASK_Buffer[1] = 0x40;
    Cali_Ctrl_S.ASK_Buffer[2] = CALI_ASK; //区别是数据还是应答
    Cali_Ctrl_S.ASK_Buffer[3] = AskNum;
    Cali_Ctrl_S.ASK_Buffer[4] = Addition;
//    if(BUSY)
    {
        //WriteExtUsart1(Cali_Ctrl_S.ASK_Buffer[0]);//发送数据包的长度
        Chip_EXUART_SendByte(LPC_EXUART1, Cali_Ctrl_S.ASK_Buffer[0]);//发送数据包的长度
        while(Chip_EXUART_CheckFIFOBusy(LPC_EXUART1) != RESET);
    }
}

/*
SerialPort_Send_Data 函数功能：计算校验和，并且开始发送(打包数据)
PackageTotal：本次发送的数据包的总个数
PackageNum:当前数据包在全部数据包的排号（序号）
DataLen_PrePackage：当前数据包内数据的字节数（按字节）
Buffer:数据写入到Buffer中去，在发送中断中发送。
*/

void SerialPort_Send_Data(Int8U PackageTotal,Int8U PackageNum,Int8U DataLen_PrePackage,char *Buffer)
{
    int Index = 0;
    Int16U CheckSum = 0;
    Cali_Ctrl_S.ASK_Buffer[0] = 0x24;
    Cali_Ctrl_S.ASK_Buffer[1] = 0x40;
    Cali_Ctrl_S.ASK_Buffer[2] = CALI_DATA; //区别是数据还是应答
    Cali_Ctrl_S.ASK_Buffer[3] = PackageNum;//数据包的编号
    Cali_Ctrl_S.ASK_Buffer[4] = DataLen_PrePackage;//当前数据包内数据的个数
    CheckSum = Cali_Ctrl_S.ASK_Buffer[2] + Cali_Ctrl_S.ASK_Buffer[3] + Cali_Ctrl_S.ASK_Buffer[4];
    if(PackageNum == 0)//
    {
        //存储总共要发送的数据包的个数
        *(Buffer + 0) = (PackageTotal & 0xff);

        CheckSum += *(Buffer + 0);//计算校验和
        *(Buffer + 1) = CheckSum&0xff;//存储校验和
    }
    else//数据长度
    {
        for(Index = 0;Index<DataLen_PrePackage;Index++)
        {
            CheckSum += *(Buffer + Index);//计算校验和
        }
        *(Buffer + Index) = CheckSum&0xff;//存储校验和
    }
//    if(BUSY)
    {
        //WriteExtUsart1(Cali_Ctrl_S.ASK_Buffer[0]);//开始发送数据包
        Chip_EXUART_SendByte(LPC_EXUART1, Cali_Ctrl_S.ASK_Buffer[0]);//开始发送数据包
        while(Chip_EXUART_CheckFIFOBusy(LPC_EXUART1) != RESET);
    }
}

/*
SerialPort_RXD_Analysis:在串口接收函数中调用，不应该包含数据的包头信息0x24 0x40
功能：用于接受数据的解析，并且根据解析的内容填写控制结构体，
*/ 

#define SerialPort_HeadNum   2
void SerialPort_RXD_Analysis(Int8U data)
{
    static Int8U  Type = 0;
    static Int16U Index = 0;
	static Int16U Recv_Cnt = 0;
	static Int8U  Recv_Sum = 0;
	static int CMD_Buffer[2] = {0};
    int InitFlag = 0;
    if(Index == 0)//确定命令类型
    {
        Type = data;   
        Index = 1;
    }
    else if(Index <= SerialPort_HeadNum)//确定编码和附加
    {
        CMD_Buffer[Index - 1] = data;
        if(Index == SerialPort_HeadNum)
        {
            Index = 0xFF;
        }
        else 
        {
            Index = 2;   
        }
    }
    if(Index == 0xFF) //解析协议
    {
        switch(Type)
        {
        case CALI_CMD: //接收到命令
			if(!Cali_Ctrl_S.CMD_Type)//如果当前已经有命令了，则不能再接收其他命令
			{
				switch(CMD_Buffer[0])//解析命令编码
				{
				case ASK_CMD_UPLOADING_DEVICE_ID:
					Cali_Ctrl_S.CMD_Type = CMD_Buffer[0];
					Cali_Ctrl_S.CMD_Data = CMD_Buffer[1];
					Cali_Ctrl_S.NextStep = Send_DevType;

					InitFlag = 1;//先初始化，在发送数据，否则无响应
					break;
				case ASK_CMD_UPLOADING_CALI_DATA:
					switch(CMD_Buffer[1])//解析附加数据
					{
					case ADDI_CALI_BATTERY://上传电池校验数据
						Cali_Ctrl_S.CMD_Type = CMD_Buffer[0];
						Cali_Ctrl_S.CMD_Data = CMD_Buffer[1];
						Cali_Ctrl_S.NextStep = Send_UploadingData;

						InitFlag = 1;//先初始化，在发送数据，否则无响应
						break;
					}
					break;
				case ASK_CMD_DOWNLOAD_CALI_DATA:
					switch(CMD_Buffer[1])//解析附加数据
					{
					case ADDI_CALI_BATTERY://接收电池校验数据
						Cali_Ctrl_S.CMD_Type = CMD_Buffer[0];
						Cali_Ctrl_S.CMD_Data = CMD_Buffer[1];
						Cali_Ctrl_S.NextStep = Send_DownloadCmdAsk;
	                    
						InitFlag = 1;//先初始化，在发送数据，否则无响应}           
						break;
					}
					break;
				}
			}
			else
			{
				InitFlag = 1;//先初始化，在发送数据，否则无响应  
			}
            break;
        case CALI_DATA://接收到数据
			if(Cali_Ctrl_S.CMD_Type == ASK_CMD_DOWNLOAD_CALI_DATA)//必须在下载数据模式才能接受数据
			{
				switch(Recv_Cnt)
				{
				case 0:
                    //如果当前不是第一个数据包，那么总的数据包数量应该大于0才有意义
                    if((CMD_Buffer[0] == 0)
                     ||(Cali_Ctrl_S.Cali_Data_S.PackageTotal > 0))
                    {
                        Recv_Sum = Type + CMD_Buffer[0] + CMD_Buffer[1];
                        Cali_Ctrl_S.Cali_Data_S.PackageNum = CMD_Buffer[0];//当前数据包编号
                        Cali_Ctrl_S.Cali_Data_S.PackageLen = CMD_Buffer[1];//当前数据包长度
                        Recv_Cnt++;
                    }
                    else//否则重新初始化,等待有效的第一包数据
                    {
				        InitFlag = 1;//先初始化，在发送数据，否则无响应  
                    }
					break;
				case 0xFFFF://收到PC的校验和与自己计算的对比，得到应答码
					if(Recv_Sum == data)//校验和正确继续发送
					{
						Cali_Ctrl_S.ASK_Type = ASK_DATA_OK;
						Cali_Ctrl_S.ASK_Data = 0;
					}
					else//校验和错误，重新发送
					{
						Cali_Ctrl_S.ASK_Type = ASK_DATA_RESEND;
						Cali_Ctrl_S.ASK_Data = 0;
					}
					if(Cali_Ctrl_S.Cali_Data_S.PackageTotal == 0)//第一个数据包，包含全部的数据信息
					{
						Cali_Ctrl_S.NextStep = Recv_DownloadData_Start;
					}
					else if(Cali_Ctrl_S.Cali_Data_S.PackageTotal != CMD_Buffer[0])//不是第一个也不是最后一个数据包
					{
						Cali_Ctrl_S.NextStep = Recv_DownloadData;
					}
					else//是最后一个数据包
					{
						Cali_Ctrl_S.NextStep = Recv_DownloadData_End;
					}
					InitFlag = 1;//先初始化，在发送数据，否则无响应  
					break;
				default: //接收到的数据
					Recv_Sum += data;
					if(CMD_Buffer[0] != 0)
					{
						Cali_Ctrl_S.Cali_Data_S.DataBuffer[Recv_Cnt - 1] = data;
					}
					else//CMD_Buffer[0]==0  第一个数据包
					{
						Cali_Ctrl_S.Cali_Data_S.PackageTotal = data;
					}
					if(Recv_Cnt >= CMD_Buffer[1])
					{
						Recv_Cnt = 0xFFFF;
					}
					else
					{
						Recv_Cnt++;
					}
					break;
				}
			}
			else
			{
				InitFlag = 1;//先初始化，在发送数据，否则无响应  
			}
            break;
        case CALI_ASK: //接收到PC的应答(只有在上传数据的时候会收到PC的应答标志)      
			if(Cali_Ctrl_S.CMD_Type == ASK_CMD_UPLOADING_CALI_DATA)//必须在上传数据模式下才能接收到PC的应答
			{
				Cali_Ctrl_S.ASK_Type = CMD_Buffer[0];
				Cali_Ctrl_S.ASK_Data = CMD_Buffer[1];
				Cali_Ctrl_S.NextStep = Recv_UploadingAsk;
			}
		    InitFlag = 1;//先初始化，在发送数据，否则无响应
            break;        
        }
        
    }
    
    if(InitFlag == 1)
    {
        Index = 0;
        Type = 0;
		Recv_Cnt = 0;
        CMD_Buffer[0] = 0;
        CMD_Buffer[1] = 0;
        Cali_Ctrl_S.RXD_Busy = 0;
    }      
}


/*
CalibrationProcess_Ctrl:在主函数的INFO_WINDOW_UPDATE任务中调用

功能：根据协议控制结构的内容执行相应内容。
*/ 
void CalibrationProcess_Ctrl(void)
{
	int Index = 0;
	static int SendBatteryAdCnt = 0;

    if(Cali_Ctrl_S.NextStep != Idle_State)
	{
		switch(Cali_Ctrl_S.NextStep)
		{
		case Send_DevType:        //1、发送设备类型
			SerialPort_Send_Ask(Cali_Ctrl_S.CMD_Type,ADDI_DEV_CRITICAL_CARE|0x1);//发送设备ID，GUI准备就绪
			Init_CaliCtrl();
			break;
		case Send_UploadingData:  //2、发送上传的数据
			//计算校验数据的长度和总的数据包数量
            if(Cali_Ctrl_S.Cali_Data_S.PackageNum == 0)//计算一共要发多少个数据包
            {
                Cali_Ctrl_S.Cali_Data_S.PackageTotal = (BatteryPara.Battery_Num * 2+BATTERY_CALI_HEAD_LEN)/DATA_PACKAGE_MAXBYTE + (((BatteryPara.Battery_Num * 2+BATTERY_CALI_HEAD_LEN)%DATA_PACKAGE_MAXBYTE)?1:0);
                Cali_Ctrl_S.Cali_Data_S.PackageLen = 1;
				SendBatteryAdCnt = 0;
            }
            else//计算当前数据包的长度
            {
				if(((BatteryPara.Battery_Num * 2+BATTERY_CALI_HEAD_LEN)%DATA_PACKAGE_MAXBYTE)*1)//最后一包数据个数小于  DATA_PACKAGE_MAXBYTE
				{
                    if(Cali_Ctrl_S.Cali_Data_S.PackageNum >= Cali_Ctrl_S.Cali_Data_S.PackageTotal)//最后一包数据
                    {
					    Cali_Ctrl_S.Cali_Data_S.PackageLen = (BatteryPara.Battery_Num * 2+BATTERY_CALI_HEAD_LEN)%DATA_PACKAGE_MAXBYTE;
                    }
                    else
                    {
					    Cali_Ctrl_S.Cali_Data_S.PackageLen = DATA_PACKAGE_MAXBYTE;
                    }
				}
				else
				{
					Cali_Ctrl_S.Cali_Data_S.PackageLen = DATA_PACKAGE_MAXBYTE;
				}
				//根据校验类型打包数据
				switch(Cali_Ctrl_S.CMD_Data)
				{
				case ADDI_CALI_BATTERY:
					break;
				}

				SendBatteryAdCnt +=  Cali_Ctrl_S.Cali_Data_S.PackageLen;
            }


			//发送数据，计算校验和
			SerialPort_Send_Data(Cali_Ctrl_S.Cali_Data_S.PackageTotal,
				            Cali_Ctrl_S.Cali_Data_S.PackageNum,
							Cali_Ctrl_S.Cali_Data_S.PackageLen,
							Cali_Ctrl_S.Cali_Data_S.DataBuffer);
            //如果当前的包大于等于总的包数，则清空结构体
			if(Cali_Ctrl_S.Cali_Data_S.PackageNum >= Cali_Ctrl_S.Cali_Data_S.PackageTotal)
			{
				Init_CaliCtrl();
			}
			Cali_Ctrl_S.NextStep = Idle_State;
			break;
		case Recv_UploadingAsk:   //3、接受上传的数据应答
			if(Cali_Ctrl_S.CMD_Type)//如果当前没有进行数据上传，则不应该进行应答
			{
				switch(Cali_Ctrl_S.ASK_Type)//解析附加数据
				{
				case ASK_DATA_OK:    //下一个
					Cali_Ctrl_S.Cali_Data_S.PackageNum++;
					Cali_Ctrl_S.NextStep = Send_UploadingData;
					break;
				case ASK_DATA_RESEND://重发
					Cali_Ctrl_S.NextStep = Send_UploadingData;
					break;
				case ASK_DATA_ERR:   //停止
					Init_CaliCtrl();
					break;
				}
			}
			Cali_Ctrl_S.ASK_Type = 0;
			Cali_Ctrl_S.ASK_Data = 0;
			break;
		case Send_DownloadCmdAsk:    //4、发送下载数据命令的应答
			SerialPort_Send_Ask(Cali_Ctrl_S.CMD_Type,Cali_Ctrl_S.CMD_Data);//
			Cali_Ctrl_S.NextStep = Idle_State;//此处刚开始数据下载，不应该清空Init_CaliCtrl
			break;
		case Send_DownloadDataAsk://5、发送下载数据的应答
			SerialPort_Send_Ask(Cali_Ctrl_S.ASK_Type,Cali_Ctrl_S.ASK_Data);//
			Cali_Ctrl_S.NextStep = Idle_State;//在Recv_DownloadData_End中已经使用了Init_CaliCtrl
			break;
		case Recv_DownloadData_Start://6、接收了第一包信息数据
		case Recv_DownloadData:      //7、接受下载的数据(接收一个数据包后响应)
		case Recv_DownloadData_End:  //8、接收下载数据的最后一个数据包
            if(Cali_Ctrl_S.ASK_Type == ASK_DATA_OK)//接收到了正确的数据包
			{
                switch(Cali_Ctrl_S.CMD_Data)//接收到的校验类型
				{
				case ADDI_CALI_BATTERY:
                    if(Cali_Ctrl_S.NextStep == Recv_DownloadData_End)//电池校验只有一个
					{
						BatteryPara.BatteryCali_Time = RTCTime;
                        BatteryPara.Battery_Step3 = Cali_Ctrl_S.Cali_Data_S.DataBuffer[0] | Cali_Ctrl_S.Cali_Data_S.DataBuffer[1]<<8;
						BatteryPara.Battery_Step2 = Cali_Ctrl_S.Cali_Data_S.DataBuffer[2] | Cali_Ctrl_S.Cali_Data_S.DataBuffer[3]<<8;
						BatteryPara.Battery_Low   = Cali_Ctrl_S.Cali_Data_S.DataBuffer[4] | Cali_Ctrl_S.Cali_Data_S.DataBuffer[5]<<8;
						BatteryPara.Battery_Empty = Cali_Ctrl_S.Cali_Data_S.DataBuffer[6] | Cali_Ctrl_S.Cali_Data_S.DataBuffer[7]<<8;

						Save_Threth_Alarm_Battery(&BatteryPara);
					}
					break;
				}
                if(Cali_Ctrl_S.NextStep == Recv_DownloadData_End)//下载完成
                {
                    Init_CaliCtrl();
                }
			}

			Cali_Ctrl_S.NextStep = Send_DownloadDataAsk;
			break;
		default: 
			break;
		}
	}

}

