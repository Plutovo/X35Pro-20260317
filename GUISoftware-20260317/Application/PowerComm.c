#include "PowerComm.h"
#include "Static_Menu_Creat.h"

extern unsigned char PM_Version_Main;
extern unsigned char PM_Version_Sub;
extern unsigned char PM_Version_Test;
extern unsigned char PM_Version_Debug;
extern unsigned char   Usart4_Tx_Buffer[10];

static char DataIndex = 0;
static short DataSum = 0;
POWER_Comm_t PowerComm;
POWER_Para0_t PowerPara0 = {0,0,0,0,0,5};
POWER_Para1_t PowerPara1;
POWER_Para2_t PowerPara2;



static void Power_unPack(void)
{
    switch (PowerComm.DataType)
    {
        //长数据
    case 0:
//        PowerPara0.Product = (PowerComm.DataBuf[0] + (PowerComm.DataBuf[1]<<8));
        PowerPara0.Software1 = (PowerComm.DataBuf[2] + (PowerComm.DataBuf[3]<<8));
        PowerPara0.Software2 = (PowerComm.DataBuf[4] + (PowerComm.DataBuf[5]<<8));
        PM_Version_Main = PowerComm.DataBuf[5];
        PM_Version_Sub = PowerComm.DataBuf[4];
        PM_Version_Test = PowerComm.DataBuf[2];
        PM_Version_Debug = PowerComm.DataBuf[3];
//        
//        PowerPara0.Hardware1 = (PowerComm.DataBuf[6] + (PowerComm.DataBuf[7]<<8));
//        PowerPara0.Hardware2 = (PowerComm.DataBuf[8] + (PowerComm.DataBuf[9]<<8));
        
//        PowerPara0.RunTime = (PowerComm.DataBuf[10] + (PowerComm.DataBuf[11]<<8));
        PowerPara0.PowerState = (PowerComm.DataBuf[12] + (PowerComm.DataBuf[13]<<8));

//        PowerPara0.Current24 = (PowerComm.DataBuf[14] + (PowerComm.DataBuf[15]<<8));
//        PowerPara0.Voltage24 = (PowerComm.DataBuf[16] + (PowerComm.DataBuf[17]<<8));
//        PowerPara0.Current12 = (PowerComm.DataBuf[18] +(PowerComm.DataBuf[19]<<8));
//        PowerPara0.Voltage12 = (PowerComm.DataBuf[20] + (PowerComm.DataBuf[21]<<8));
//        PowerPara0.CurrentPM = (PowerComm.DataBuf[22] + (PowerComm.DataBuf[23]<<8));
//        PowerPara0.VoltagePM = (PowerComm.DataBuf[24] + (PowerComm.DataBuf[25]<<8));
//        PowerPara0.CurrentCharge = (PowerComm.DataBuf[26] + (PowerComm.DataBuf[27]<<8));
//        PowerPara0.VoltageCharge = (PowerComm.DataBuf[28] + (PowerComm.DataBuf[29]<<8));

        break;
    case 1:
//        PowerPara1.Battery1 = (PowerComm.DataBuf[0] + (PowerComm.DataBuf[1]<<8));//1和2看成一个整体
//        PowerPara1.Battery2 = (PowerComm.DataBuf[2] +(PowerComm.DataBuf[3]<<8));//
//        PowerPara1.BatteryTemper = (PowerComm.DataBuf[4] +(PowerComm.DataBuf[5]<<8));
//        PowerPara1.BatteryCurrent = (PowerComm.DataBuf[6] + (PowerComm.DataBuf[7]<<8));
//        PowerPara1.BatteryVoltage = (PowerComm.DataBuf[8] + (PowerComm.DataBuf[9]<<8));
        PowerPara1.BatteryShengYuRongLiang = (PowerComm.DataBuf[10] + (PowerComm.DataBuf[11]<<8));
        //        PowerPara1.BatteryChargeTime = (PowerComm.DataBuf[12] + (PowerComm.DataBuf[13]<<8));//无效
        PowerPara1.OnOffState = (PowerComm.DataBuf[16] + (PowerComm.DataBuf[17]<<8));
        
        //        PowerPara1.Battery1_Charging = (PowerComm.DataBuf[26] + (PowerComm.DataBuf[27]<<8));
//        PowerPara1.Battery2_Charging = (PowerComm.DataBuf[28] + (PowerComm.DataBuf[29]<<8));
//        if ((PowerPara1.BatteryChargeTime == 0)&& (PowerPara0.PowerState != 1))
//        {
//            PowerPara1.BatteryShengYuRongLiang = 100;
//        }
        break;
    case 2:
//        PowerPara2.Battery1_Voltage = (PowerComm.DataBuf[0] + (PowerComm.DataBuf[1]<<8));
//        PowerPara2.Battery2_Voltage = (PowerComm.DataBuf[2] + (PowerComm.DataBuf[3]<<8));
//        PowerPara2.Battery1_Capacity = (PowerComm.DataBuf[4] + (PowerComm.DataBuf[5]<<8));
//        PowerPara2.Battery2_Capacity = (PowerComm.DataBuf[6] + (PowerComm.DataBuf[7]<<8));
//        PowerPara2.Charge_Discharge_State = (PowerComm.DataBuf[8] + (PowerComm.DataBuf[9]<<8));
        break;
        //短数据
    case 0x04:
         PowerPara0.PowerState = (PowerComm.DataBuf[0] + (PowerComm.DataBuf[1]<<8));
       break;
    case 0x12:
         PowerPara1.BatteryShengYuRongLiang = (PowerComm.DataBuf[0] + (PowerComm.DataBuf[1]<<8));
       break;
    case 0x13:
//         PowerPara1.BatteryChargeTime = (PowerComm.DataBuf[0] + (PowerComm.DataBuf[1]<<8));
//        if ((PowerPara1.BatteryChargeTime == 0)&& (PowerPara0.PowerState != 1))
//        {
//            PowerPara1.BatteryShengYuRongLiang = 100;
//        }
       break;
       
    }
}
/*
*电源管理板传输数据接收
*/
void Power_Receive(unsigned char data)
{
	static unsigned char RxBuff[POWER_LEN] = {0};
    static unsigned char sucCurStatus = HEADER;
    int nLen = 0;
    switch (sucCurStatus)
    {
    case HEADER:
    default:
        if (data == POWER_HEADER)
        {
            DataSum = data;
            sucCurStatus = COMMTYPE;
        }
        break;
    case COMMTYPE:
        if (data == PM_TO_GUI)
        {
            PowerComm.CommType = data;
            DataSum += data;
            sucCurStatus = DATALENGTH;
        }
        else
        {
            sucCurStatus = HEADER; 
        }
        break;
    case DATALENGTH:
        PowerComm.DataLen = data-2;
        if (PowerComm.DataLen > 50)
        {
            PowerComm.DataLen = 50;
        }
        DataSum += data;
        sucCurStatus = DATATYPE; 
        if (data == POWER_HEADER)
        {
            DataSum = data;
            sucCurStatus = COMMTYPE;
        }
        break;
    case DATATYPE:
        PowerComm.DataType = data;
        DataSum += data;
        DataIndex = 0;
        sucCurStatus = DATA;       
        if (data == POWER_HEADER)
        {
            DataSum = data;
            sucCurStatus = COMMTYPE;
        }
        break;
    case DATA:
        DataSum += data;
        PowerComm.DataBuf[DataIndex++] = data;
        if (DataIndex >= PowerComm.DataLen)
        {
            sucCurStatus = CHECKSUM;       
        }
       break;
    case CHECKSUM:
        if (data == (DataSum&0xFF))
        {
            Power_unPack();
        }
        sucCurStatus = HEADER;  
        break;
    }
}

//发送查询电源板版本命令
//void Power_CheckVersion(unsigned char *TxBuff)
//{
//    *(TxBuff+HEADER) = POWER_HEADER;
//    *(TxBuff+DEVICE) = POWER_DEVICE;
//    *(TxBuff+EVENT) = POWER_VERSION; 
//    *(TxBuff+END) = POWER_END;
//    SendMessage(TxBuff, POWER_LEN);
//}
////发送查询电源板电池电压
//void Power_CheckBatteryVoltage(unsigned char *TxBuff)
//{
//    *(TxBuff+HEADER) = POWER_HEADER;
//    *(TxBuff+DEVICE) = POWER_DEVICE;
//    *(TxBuff+EVENT) = POWER_VOLTAGE;
//    *(TxBuff+END) = POWER_END;
//    SendMessage(TxBuff, POWER_LEN);
//}
void Power_SendMessage(unsigned char *TxBuff,int Num)
{
    int i =0;
    for (i=0; i<Num;i++)
    {
        UART4_SendByte_Poll(*(TxBuff+i));
    }
}
void Power_SendCommond(unsigned char StartAddr,unsigned short data)
{
	unsigned short CheckSum = 0;
	unsigned char i = 0;
    *(Usart4_Tx_Buffer+0) = POWER_HEADER;
    *(Usart4_Tx_Buffer+1) = GUI_TO_PM;
    *(Usart4_Tx_Buffer+2) = POWER_DATA_LENGTH;
    *(Usart4_Tx_Buffer+3) = StartAddr;
    *(Usart4_Tx_Buffer+4) = data&0xFF;
    *(Usart4_Tx_Buffer+5) = (data>>8)&0xFF;
    
    for (i=0;i<POWER_DATA_LENGTH+2;i++)
    {
        CheckSum += *(Usart4_Tx_Buffer+i);
    }
    *(Usart4_Tx_Buffer+6) = CheckSum&0xFF;
    Power_SendMessage(Usart4_Tx_Buffer,7);
}
/*
* 向电源管理板发送机器型号；0长按，1开关
*/
void Power_SendMachineModel(unsigned short type)
{
   Power_SendCommond(START_ADDR_ONOFF_TYPE,type); 
}
/*
* 向电源管理板发送开关机；0开机，1关机 3取消手动关机
*/
void Power_SendOnOffState(unsigned short state)
{
   Power_SendCommond(START_ADDR_ONOFF_STATE,state); 
}

void Power_SendBatManageType(unsigned short state)
{
   Power_SendCommond(START_ADDR_BAT_MANAGE_TYPE,state); 
}

//////////////////////////////////////////////////////////////////////////////////////////////////
/*
* 返回值：电源状态  FALSE断开 TRUE接通
*/
unsigned char CheckAcStatus(void)
{
    if (PowerPara0.PowerState&0x04)
        return TRUE;
    else
        return FALSE;
}
/*
* 返回值：电池状态  0断开 1接通
*/
unsigned char CheckBatteryStatus(void)
{
   return(PowerPara0.PowerState&0x01); 
}
/*
* 返回值：电池充电状态 0充电中 1充电完成 2没有电池
*/
unsigned char CheckChargeStatus(void)
{
    unsigned char state = 1;    
    if (PowerPara0.PowerState&0x01)
    {
        if (PowerPara1.BatteryShengYuRongLiang <10000)
        {
            state = 0;
        }
        else if(PowerPara1.BatteryShengYuRongLiang == 10000)
        {
            state = 1;
        }
    }
    else
    {
        state = 2;
    }
    return(state);
}


int Power_GetBatteryVoltage()
{
    return PowerPara1.BatteryShengYuRongLiang;
}
unsigned short Power_GetOnOffState()
{
    return PowerPara1.OnOffState;
}