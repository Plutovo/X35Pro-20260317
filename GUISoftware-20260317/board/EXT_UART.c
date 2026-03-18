#include "EXT_UART.h"
#include "board.h"
#include "sys.h"
#include "Communication_Handle.h" //sun add 20140512
#include "Calibration_Protocol.h"
/*****************************************************************************/
extern CALI_PROTOCOL_CTRL_S Cali_Ctrl_S;
extern INT8U   EXTUSART1_TXD_Busy_FLAG;
extern INT8U   EXTUsart1_Tx_Buffer[5];



static INT8U ReadExtUsart1(void)
{
    INT8U data;
    data = ERHR1;
    return data;
}
static INT8U ReadExtUsart1Status(void)
{
    INT8U data;
    data = EISR1;
    return data;
}
void WriteExtUsart1(INT8U data)
{
    ETHR1 = data;
}
static INT8U ReadExtUsart2(void)
{
    INT8U data;
    data = ERHR2;
    return data;
}
static INT8U ReadExtUsart2Status(void)
{
    INT8U data;
    data = EISR2;
    return data;
}
static void WriteExtUsart2(INT8U data)
{
    ETHR2 = data;;
}
static void ExtUsartIntHandler(void)
{
    static    INT16U  ExtUart2RxBuffer[3]= {0};
    static    INT8U   Rx_num=0,Tx_num = 0;
    //static        INT8U   Tx_num=0;
    static    INT16U  Wait_Syn=TRUE;
    static    INT16U  Flowmeter_Syn = TRUE;
    static    INT16U  LastData=0;
    static   INT16U EXT1_TXD_Cnt = 1;
    static   INT8U EXT1_Head1 = 0;
    INT8U data,cnt = 0;
    INT8U   ExtUart1data = 0;
    if(IO0INTSTATR)
    {
        if(IO0INTSTATR_bit.P0_4)
        {
            data = ReadExtUsart2Status();
            if(data&RXRDY)
            {
            }
            else if(data&TXRDY)
            {
            }
            IO0INTCLR_bit.P0_4 = 1;
        }
        if(IO0INTSTATR_bit.P0_5)
        {
            data = ReadExtUsart1Status();
            if(data&RXRDY)//接收数据
            {
                data = ReadExtUsart1();
                if(Cali_Ctrl_S.RXD_Busy)//解析协议
                {
                    SerialPort_RXD_Analysis(data);
                }
                else if((EXT1_Head1 == 0x24)&&(data == 0x40))//判断帧头
                {
                    Cali_Ctrl_S.RXD_Busy = 1;
                }
                EXT1_Head1 = data;
            }
            else if(data&TXRDY)//发送数据
            {
                if(Cali_Ctrl_S.ASK_Buffer[2] == CALI_DATA)//发送数据
                {
                    if(EXT1_TXD_Cnt < ASK_BUFFER_LEN)//发送数据的包头信息
                    {
                        WriteExtUsart1(Cali_Ctrl_S.ASK_Buffer[EXT1_TXD_Cnt]);
                        EXT1_TXD_Cnt++;
                    }
                    else if(EXT1_TXD_Cnt < ASK_BUFFER_LEN + Cali_Ctrl_S.ASK_Buffer[4] + 1)//发送数据信息
                    {
                        WriteExtUsart1(Cali_Ctrl_S.Cali_Data_S.DataBuffer[EXT1_TXD_Cnt - ASK_BUFFER_LEN]);
                        EXT1_TXD_Cnt++;
                    }
                    else //数据发送完成
                    {
                        for(cnt = 0;cnt < ASK_BUFFER_LEN;cnt++)
                        {
                            Cali_Ctrl_S.ASK_Buffer[cnt] = 0;                      
                        }
                        EXT1_TXD_Cnt = 1;
                    }
                }
                else if(Cali_Ctrl_S.ASK_Buffer[2] == CALI_ASK)//发送应答
                {
                    WriteExtUsart1(Cali_Ctrl_S.ASK_Buffer[EXT1_TXD_Cnt]);
                    EXT1_TXD_Cnt++;
                    if(EXT1_TXD_Cnt >= ASK_BUFFER_LEN)//数据发送完成
                    {
                        for(cnt = 0;cnt < ASK_BUFFER_LEN;cnt++)
                        {
                            Cali_Ctrl_S.ASK_Buffer[cnt] = 0;                      
                        }
                        EXT1_TXD_Cnt = 1;
                    }
                }
            }
            IO0INTCLR_bit.P0_5 = 1;
        }
    }
    VICADDRESS = 0;
}
