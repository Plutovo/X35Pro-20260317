/*  #include directives go here */
#include "Drager.h"

//extern AA_MODULE_FLAG_REG AA_Module_Flag;

/*  #define directives go here  */
typedef struct
{
    u8 FrameSelectMask[8];
    u16 RTDataMask;
    u16 INT;
}DRAGER_CMD_FRAMEDATA;

typedef struct
{
    u16 ParaSelectMask;
    u8 Unit;
}DRAGER_CMD_CHANGEUNITS;

typedef struct
{
    u16 ValueSelectMask;
    s16 Value;
    u8 ParaMode;
}DRAGER_CMD_ACCEPTEXPARA;

typedef struct
{
    u8 ParaToZero;
    u8 ZeroType;
    u8 ZeroMode;
}DRAGER_CMD_INITZERO;

typedef struct
{
    u8 TimeData[6];
    u8 TimeInfoSelector;
}DRAGER_CMD_ADJUSTTIME;

typedef struct
{
    u8 BaseTime;
    u8 OperatMode;
    u8 InformationSelect;
    DRAGER_CMD_FRAMEDATA FrameData;
    DRAGER_CMD_CHANGEUNITS ChangeParaUnits;
    DRAGER_CMD_ACCEPTEXPARA AcceptExPara;
    u8 SelectAnestheticAgent;
    u8 SwitchBreathMode;
    DRAGER_CMD_INITZERO InitiateZero;
    DRAGER_CMD_ADJUSTTIME Adjusttime;
    u8 SwitchValves;
    u8 SwitchPump;
}DRAGER_CMD;

/*  declarations of external variables go here  */
u8 RX_Drager[40] = {0};
u8 TX_Drager[20] = {0};
u8 ReceiveDragerFlag = 0;
u8 DragerDataNum = 0;
u8 DragerDataNum_Count = 0;
u8 ReceiveDragerErr = 0;
u8 DragerSendDataNum = 0;

u16 Drager_Return_Flag = 0; //接收响应存储标志，按位判断，由低到高依次为
//bit0:基本时间间隔；bit1:工作模式；     bit2:设备信息；        bit3:病人数据；   bit4:停止连续发送
//bit5:改变参数单位；bit6:接收额外参数； bit7:选择麻醉气体类型；bit8:改变呼吸探测模式
//bit9:初始化零点；  bit10:调整时间信息；bit11:传输模块特征；   bit12:切换阀；    bit13:切换泵

u16 Drager_Para_Flag = 0; //参数存储标志，按位判断，由低到高依次为：
//bit0：工作模式，bit1：序列号，bit2：CO2、N2O慢速参数，bit3：O2慢速参数，bit4：AA1慢速参数
//bit5：AA2慢速参数，bit6：压力和泵流量，bit7：通知信息，bit8：实时数据，bit9：停止连续发送
//bit10：改变参数单位，bit11：接收额外参数，bit12：接收麻醉气体，bit13：校零，bit14：切换阀
//bit15：切换泵


unsigned char Drager_Message_Register[10] = {0};

struct  //参数详细信息
{
    u16 PAI;//可用参数,0不可用，1可用
    u16 PII;//无效参数,0无效，1有效
    u16 HSP;//需要用户设置的参数，bit0：CO2，bit1：N2O，bit2：AA1，bit3：AA2，bit4：O2，bit6：PawMean，bit7：Pressure
    u8 MISCSTAT;//组件状态
    u8 OMS;//工作模式
}Drager_ParaInfo;

struct DRAGER_AA_DATA Drager_AA1_Data = {0,0,0,0,0,0,0,0,0};
struct DRAGER_AA_DATA Drager_AA2_Data = {0,0,0,0,0,0,0,0,0};

struct DRAGER_GAS_DATA Drager_O2_Data = {0,0,0,0,0,0,0};
struct DRAGER_GAS_DATA Drager_CO2_Data = {0,0,0,0,0,0,0};
struct DRAGER_GAS_DATA Drager_N2O_Data = {0,0,0,0,0,0,0};

struct DRAGER_MODULEFLAG DRAGER_FLAG = {0,Measure,0,0,0,0,0,0,0,NoneAgent,0,0,0};

struct DRAGER_PRESSANDPUMP Drager_PressureAndPump = {0,0,0,0,0};

struct
{
    u16 HighPriority;
    u16 MedPriority;
    u16 LowPriority;
}Drager_Message = {0,0,0};

struct
{
    u8 PnueCtrl;  //气动控制方式，0：手动，1：自动
    u8 PenuAvail; //气动可用性，0：不可用，1：可用
    u8 ZeroControl; //校零方式，0：自动，1：手动
}Drager_ModuleFeature = {0,0,0};

struct DRAGER_RT_DATA Drager_RT_Data = {0,0,0,0,0,0};

struct DRAGER_RESPINFO Drager_RespirationInfo = {0,0,0};

struct DRAGER_DEVICEINFO Drager_DeviceInfo = {{0},{0},{0},{0},{0},{0}};

u16 Drager_BaseTime = 0;//返回的基本间隔时间
u8 Drager_BreathDetectionMode = 0;//$1E,设置呼吸探测模式
u8 Drager_User_Zero = 0;//手动校零需要UI配合拔管

DRAGER_ACK Drager_Ack;
DRAGER_CMD Drager_Cmd;

//Send_Data *Drager_Send;

/*  prototypes  */
enum IM_FUNC_STS ReceiveDragerData(u8 data);
u8 Check_Drager_Sum(u8 len);
enum IM_FUNC_STS AnalysisDragerData(void);
u8 CalculateCheckSum(u8 len,u8* tx_buffer);
void AnalysisDragerErr(u8 err);

//void Drager_SetBaseTime(u16 basetime);
void Drager_GetBaseTime(u8* tx_buffer);
u8 Drager_Accessexpara(u16 valuemask,u16 value,u8 paramode,u8* tx_buffer);
void Drager_TransModuleMask(u8* tx_buffer);

u8 Drager_SetOpMode(u8 opmode,u8* tx_buffer);
u8 Drager_StopContinueTrans(u8* tx_buffer);
u8 Drager_ChangeParaUnit(u16 paramask,u8 unit,u8* tx_buffer);
u8 Drager_SelectAA(u8 anestheticagent,u8* tx_buffer);
//u8 Drager_InitZero(void);
void Drager_Adjusttime(u8 hour,u8 minu,u8 sec,u8 day,u8 month,u8 year,u8* tx_buffer);
u8 Drager_SwitchValves(u8 valve,u8* tx_buffer);
u8 Drager_SwitchPumps(u8 pump,u8* tx_buffer);

//void Init_Drager(Send_Data *USART_SEND);  //初始化串口发送函数
void Init_Drager(void);
void Drager_SetConnect(void); //握手、开泵、校零
u8 Drager_GetDivInfo(u8 infoselect,u8* tx_buffer);  //获取设备信息
u8 Drager_ChangeBreathMode(u8 breathmode,u8* tx_buffer);  //设置呼吸探测模式
u8 Drager_JudgeAgent(void); //1:需要手动设置AA，2:无需设置，自动识别
u8 Drager_GetPatientData(u16 framemask,u16 rtmask,u16 ints,u8* tx_buffer);
u16 Drager_GetAAData(u16 channel);  //获取AA参数状态
u16 Drager_GetGasData(u16 channel); //获取O2、N2O、Air参数状态
u16 Drager_GetModuleState(u8* tx_buffer);

/*
****************************************************************************************************
函数名称：Init_Drager
入口参数：串口发送函数
出口参数：无
功能描述：初始化麻醉气体模块
****************************************************************************************************
*/
//void Init_Drager(Send_Data *USART_SEND)
//{
//    Drager_Send = USART_SEND;
//}
void Init_Drager(void)
{
    unsigned char i, *p;
    p = &RX_Drager[0];//指向接收缓冲区的首址
    i = sizeof(RX_Drager);
    while(i--)
    {
        *p++ = 0;
    }
    p = &TX_Drager[0];
    i = sizeof(TX_Drager);
    while(i--)
    {
        *p++ = 0;
    }
    ReceiveDragerFlag = 0;
    DragerDataNum = 0;
    DragerDataNum_Count = 0;
    ReceiveDragerErr = 0;
    DragerSendDataNum = 0;
    Drager_Return_Flag = 0;
    Drager_Para_Flag = 0;
    p = &Drager_Message_Register[0];
    i = sizeof(Drager_Message_Register);
    while(i--)
    {
        *p++ = 0;
    }
    p = (unsigned char *)(&Drager_Cmd);
    i = sizeof(Drager_Cmd);
    while(i--)
    {
        *p++ = 0;
    }
    p = (unsigned char *)(&Drager_Ack);
    i = sizeof(Drager_Ack);
    while(i--)
    {
        *p++ = 0;
    }
    p = (unsigned char *)(&Drager_ParaInfo);
    i = sizeof(Drager_ParaInfo);
    while(i--)
    {
        *p++ = 0;
    }
    p = (unsigned char *)(&Drager_AA1_Data);
    i = sizeof(Drager_AA1_Data);
    while(i--)
    {
        *p++ = 0;
    }
    p = (unsigned char *)(&Drager_AA2_Data);
    i = sizeof(Drager_AA2_Data);
    while(i--)
    {
        *p++ = 0;
    }
    p = (unsigned char *)(&Drager_O2_Data);
    i = sizeof(Drager_O2_Data);
    while(i--)
    {
        *p++ = 0;
    }
    p = (unsigned char *)(&Drager_CO2_Data);
    i = sizeof(Drager_ParaInfo);
    while(i--)
    {
        *p++ = 0;
    }
    p = (unsigned char *)(&Drager_N2O_Data);
    i = sizeof(Drager_N2O_Data);
    while(i--)
    {
        *p++ = 0;
    }
    p = (unsigned char *)(&DRAGER_FLAG);
    i = sizeof(DRAGER_FLAG);
    while(i--)
    {
        *p++ = 0;
    }
    p = (unsigned char *)(&Drager_PressureAndPump);
    i = sizeof(Drager_PressureAndPump);
    while(i--)
    {
        *p++ = 0;
    }
    p = (unsigned char *)(&Drager_Message);
    i = sizeof(Drager_Message);
    while(i--)
    {
        *p++ = 0;
    }
    p = (unsigned char *)(&Drager_ModuleFeature);
    i = sizeof(Drager_ModuleFeature);
    while(i--)
    {
        *p++ = 0;
    }
    p = (unsigned char *)(&Drager_RT_Data);
    i = sizeof(Drager_RT_Data);
    while(i--)
    {
        *p++ = 0;
    }
    p = (unsigned char *)(&Drager_RespirationInfo);
    i = sizeof(Drager_RespirationInfo);
    while(i--)
    {
        *p++ = 0;
    }
    p = (unsigned char *)(&Drager_DeviceInfo);
    i = sizeof(Drager_DeviceInfo);
    while(i--)
    {
        *p++ = 0;
    }
    
    Drager_BaseTime = 0;
    Drager_BreathDetectionMode = 0;
    Drager_User_Zero = 0;
}

/*
****************************************************************************************************
函数名称：ReceiveDragerData
入口参数：接收的字节
出口参数：无
功能描述：接收数据
****************************************************************************************************
*/
enum IM_FUNC_STS ReceiveDragerData(u8 data)
{
    if((ReceiveDragerFlag == 0)&&((data == 0x06)|(data == 0x15))) //接收帧头
    {
        ReceiveDragerFlag = 1;
        RX_Drager[0] = data;
        return IM_BUSY;
    }
    else if(ReceiveDragerFlag == 1)  //接收识别码
    {
        ReceiveDragerFlag = 2; 
        RX_Drager[1] = data;
        return IM_BUSY;
    }
    else if(ReceiveDragerFlag == 2)  //接收数据长度
    {
        ReceiveDragerFlag = 3;
        DragerDataNum = data;
        RX_Drager[2] = DragerDataNum;
        DragerDataNum_Count = 0;
        return IM_BUSY;
    }
    else if(ReceiveDragerFlag == 3)  //接收数据和校验和
    {
        if(DragerDataNum_Count < DragerDataNum)
        {
            RX_Drager[DragerDataNum_Count+3] = data;
            DragerDataNum_Count ++;
            return IM_BUSY;
        }
        else
        {
            RX_Drager[DragerDataNum_Count+3] = data;
            if(Check_Drager_Sum(DragerDataNum+3) == 1) //计算校验和
            {
                ReceiveDragerErr = 0;
                AnalysisDragerData();
                ReceiveDragerFlag = 0;
                return IM_OK;
            }
            else
            {
                ReceiveDragerErr = 1;
                ReceiveDragerFlag = 0;
                return IM_ERR;
            }
            
        }
    }
    else
    {
        ReceiveDragerFlag = 0;
        return IM_ERR;
    }
}

/*
****************************************************************************************************
函数名称：Check_Drager_Sum
入口参数：计算的长度
出口参数：无
功能描述：验证校验和
****************************************************************************************************
*/
u8 Check_Drager_Sum(u8 len)
{
    u8 CS = 0;
    int i = 0;
    for(i = 0;i < len;i ++)
    {
        CS = CS + RX_Drager[i];
    }
    CS = ~CS + 1;
    if(CS == RX_Drager[len])
    {
        return 1;
    }else
    {
        return 0;
    }
}

/*
****************************************************************************************************
函数名称：AnalysisDragerData
入口参数：无
出口参数：无
功能描述：分析数据
****************************************************************************************************
*/
enum IM_FUNC_STS AnalysisDragerData(void)
{
    int i = 0;
    Drager_Ack.Err = 0;
    switch(RX_Drager[1])  //识别码
    {
    case DRAGER_ID_BASETIME:  //基本时间间隔
        if(RX_Drager[0] == 0x06)
        {
            Drager_Ack.BaseTime = (RX_Drager[3]<<8)|RX_Drager[4];
            Drager_BaseTime = Drager_Ack.BaseTime;
        }else
        {
            Drager_Ack.Err = RX_Drager[3];
            Drager_BaseTime = 0;
        }
        Drager_Return_Flag |= (1<<0);
        break;
    case DRAGER_ID_OPERATION:  //工作模式
        if(RX_Drager[0] == 0x06)
        {
            Drager_Ack.OperatMode = RX_Drager[3];
        }else
        {
            Drager_Ack.Err = RX_Drager[3];
        }
        Drager_Return_Flag |= (1<<1);
        break;
    case DRAGER_ID_INFORMATION:  //设备信息
        if(RX_Drager[0] == 0x06)
        {
            for( i = 0;i < 10;i ++)
                Drager_Ack.Information.Data[i] = RX_Drager[i+11];
            Drager_Ack.Information.Select = RX_Drager[21];
            switch(Drager_Ack.Information.Select)
            {
            case 0x00:  //供应商代码
                for(i = 0;i < 5;i ++)
                {
                    Drager_DeviceInfo.Drager_Vendor_Code[i] = Drager_Ack.Information.Data[i];
                    Drager_DeviceInfo.Drager_Dev_Flag |= 0x01;
                }
                break;
            case 0x01: //序列号
                for(i = 0;i < 9;i ++)
                {
                    Drager_DeviceInfo.Drager_Serial_Num[i] = Drager_Ack.Information.Data[i];
                    Drager_DeviceInfo.Drager_Dev_Flag |= 0x02;
                }
                break;
            case 0x02:  //硬件版本
                for(i = 0;i < 2;i ++)
                {
                    Drager_DeviceInfo.Drager_HandleVersion[i] = Drager_Ack.Information.Data[i];
                    Drager_DeviceInfo.Drager_Dev_Flag |= 0x04;
                }
                break;
            case 0x03:  //软件版本
                for(i = 0;i < 8;i ++)
                {
                    Drager_DeviceInfo.Drager_SoftwareVersion[i] = Drager_Ack.Information.Data[i];
                    Drager_DeviceInfo.Drager_Dev_Flag |= 0x08;
                }
                break;
            case 0x05:  //产品名称
                for(i = 0;i < 4;i ++)
                {
                    Drager_DeviceInfo.Drager_ProductName[i] = Drager_Ack.Information.Data[i];
                    Drager_DeviceInfo.Drager_Dev_Flag |= 0x10;
                }
                break;
            case 0x06:  //零件编号
                for(i = 0;i < 7;i ++)
                {
                    Drager_DeviceInfo.Drager_PartNumber[i] = Drager_Ack.Information.Data[i];
                    Drager_DeviceInfo.Drager_Dev_Flag |= 0x20;
                }
                break;
            }
            Drager_Para_Flag |= (1<<1);
        }else
        {
            Drager_Ack.Err = RX_Drager[3];
        }
        Drager_Return_Flag |= (1<<2);
        break;
    case DRAGER_ID_FRAMEDATA:  //病人数据
        if(RX_Drager[0] == 0x06)
        {
            for(i = 0;i < 10;i ++)
                Drager_Ack.FrameData.FrameData[i] = RX_Drager[i+3];
            Drager_Ack.FrameData.FrameNum = RX_Drager[13];
            Drager_Ack.FrameData.ModuleStatus = RX_Drager[14];
            Drager_Ack.FrameData.RTDataMask = (RX_Drager[15]<<8)|RX_Drager[16];
            Drager_Ack.FrameData.RTParaStatus = (RX_Drager[17]<<8)|RX_Drager[18];
            for(i = 0;i < DragerDataNum-16;i ++)
                Drager_Ack.FrameData.RTData[i] = RX_Drager[i+19];
            switch(Drager_Ack.FrameData.FrameNum)
            {
            case 0x03:  //CO2、N2O浓度
                Drager_CO2_Data.FI = (Drager_Ack.FrameData.FrameData[0]<<8)|Drager_Ack.FrameData.FrameData[1];
                Drager_CO2_Data.ET = (Drager_Ack.FrameData.FrameData[2]<<8)|Drager_Ack.FrameData.FrameData[3];
                Drager_CO2_Data.PS = Drager_Ack.FrameData.FrameData[8];
                Drager_CO2_Data.ParaMode = Drager_Ack.FrameData.FrameData[8]>>6;
                Drager_CO2_Data.ZeroState = (Drager_Ack.FrameData.FrameData[8]>>5)&0x01;
                Drager_CO2_Data.ZeroResult = (Drager_Ack.FrameData.FrameData[8]>>4)&0x01;
                Drager_CO2_Data.Accuracy = Drager_Ack.FrameData.FrameData[8]&0x03;
                Drager_CO2_Data.Unit = (Drager_Ack.FrameData.FrameData[8]>>3)&0x01;
                
                Drager_N2O_Data.FI = (Drager_Ack.FrameData.FrameData[4]<<8)|Drager_Ack.FrameData.FrameData[5];
                Drager_N2O_Data.ET = (Drager_Ack.FrameData.FrameData[6]<<8)|Drager_Ack.FrameData.FrameData[7];
                Drager_N2O_Data.PS = Drager_Ack.FrameData.FrameData[9];
                Drager_N2O_Data.ParaMode = Drager_Ack.FrameData.FrameData[9]>>6;
                Drager_N2O_Data.ZeroState = (Drager_Ack.FrameData.FrameData[9]>>5)&0x01;
                Drager_N2O_Data.ZeroResult = (Drager_Ack.FrameData.FrameData[9]>>4)&0x01;
                Drager_N2O_Data.Accuracy = Drager_Ack.FrameData.FrameData[9]&0x03;
                Drager_N2O_Data.Unit = (Drager_Ack.FrameData.FrameData[9]>>3)&0x01;
                break;
            case 0x04:  //O2浓度
                Drager_O2_Data.FI = (Drager_Ack.FrameData.FrameData[0]<<8)|Drager_Ack.FrameData.FrameData[1];
                Drager_O2_Data.ET = (Drager_Ack.FrameData.FrameData[2]<<8)|Drager_Ack.FrameData.FrameData[3];
                Drager_O2_Data.PS = Drager_Ack.FrameData.FrameData[8];
                Drager_O2_Data.ParaMode = Drager_Ack.FrameData.FrameData[8]>>6;
                Drager_O2_Data.ZeroState = (Drager_Ack.FrameData.FrameData[8]>>5)&0x01;
                Drager_O2_Data.ZeroResult = (Drager_Ack.FrameData.FrameData[8]>>4)&0x01;
                Drager_O2_Data.Accuracy = Drager_Ack.FrameData.FrameData[8]&0x03;
                Drager_O2_Data.Unit = (Drager_Ack.FrameData.FrameData[8]>>3)&0x01;
                break;
            case 0x0B:  //压力和泵流量
                Drager_PressureAndPump.WatertrapWarning = Drager_Ack.FrameData.FrameData[1]>>7;
                Drager_PressureAndPump.WatertrapFull = (Drager_Ack.FrameData.FrameData[1]>>6)&0x01;
                Drager_PressureAndPump.WatertrapDisconnected = (Drager_Ack.FrameData.FrameData[1]>>5)&0x01;
                Drager_PressureAndPump.ValvesState = (Drager_Ack.FrameData.FrameData[1]>>2)&0x03;
                Drager_PressureAndPump.PumpFlowState = Drager_Ack.FrameData.FrameData[1]&0x03;
                Drager_PressureAndPump.Pressure = (Drager_Ack.FrameData.FrameData[2]<<8)|Drager_Ack.FrameData.FrameData[3];
                Drager_PressureAndPump.PumpFlow = (Drager_Ack.FrameData.FrameData[4]<<8)|Drager_Ack.FrameData.FrameData[5];
                Drager_PressureAndPump.Pre_ParaMode = Drager_Ack.FrameData.FrameData[8]>>6;
                Drager_PressureAndPump.Pre_ZeroState = (Drager_Ack.FrameData.FrameData[8]>>5)&0x01;
                Drager_PressureAndPump.Pre_ZeroResult = (Drager_Ack.FrameData.FrameData[8]>>4)&0x01;
                Drager_PressureAndPump.Pre_Accuracy = Drager_Ack.FrameData.FrameData[8]&0x03;
                Drager_PressureAndPump.Pre_Unit = (Drager_Ack.FrameData.FrameData[8]>>3)&0x01;
                Drager_PressureAndPump.Flow_ParaMode = Drager_Ack.FrameData.FrameData[9]>>6;
                Drager_PressureAndPump.Flow_ZeroState = (Drager_Ack.FrameData.FrameData[9]>>5)&0x01;
                Drager_PressureAndPump.Flow_ZeroResult = (Drager_Ack.FrameData.FrameData[9]>>4)&0x01;
                Drager_PressureAndPump.Flow_Accuracy = Drager_Ack.FrameData.FrameData[9]&0x03;
                Drager_PressureAndPump.Flow_Unit = (Drager_Ack.FrameData.FrameData[9]>>3)&0x01;
                break;
            case 0x0D:  //呼吸信息
                Drager_RespirationInfo.ApneaDuration = (Drager_Ack.FrameData.FrameData[4]<<8)|Drager_Ack.FrameData.FrameData[5];
                Drager_RespirationInfo.RespirationRate = Drager_Ack.FrameData.FrameData[8];
                Drager_RespirationInfo.BreathDetectionMode = Drager_Ack.FrameData.FrameData[9];
                break;
            case 0x0E:  //参数详细状态
                Drager_ParaInfo.PAI = (Drager_Ack.FrameData.FrameData[0]<<8)|Drager_Ack.FrameData.FrameData[1];
                Drager_ParaInfo.PII = (Drager_Ack.FrameData.FrameData[2]<<8)|Drager_Ack.FrameData.FrameData[3];
                Drager_ParaInfo.HSP = (Drager_Ack.FrameData.FrameData[4]<<8)|Drager_Ack.FrameData.FrameData[5];
                Drager_ParaInfo.MISCSTAT = Drager_Ack.FrameData.FrameData[7];
                Drager_ParaInfo.OMS = Drager_Ack.FrameData.FrameData[9];
                if((Drager_ParaInfo.PAI>>2)&0x03)
                {
                    DRAGER_FLAG.SetAgent = AgentAvailable;
                }else
                {
                    DRAGER_FLAG.SetAgent = NoAgentAvailable;
                }
                
                if(Drager_ParaInfo.OMS == 0x00)
                {
                    DRAGER_FLAG.OperateMode = Measure;
                }else if(Drager_ParaInfo.OMS == 0x01)
                {
                    DRAGER_FLAG.OperateMode = Standby_Mode;
                }else if(Drager_ParaInfo.OMS == 0x02)
                {
                    DRAGER_FLAG.OperateMode = ComponentsOff;
                }else if(Drager_ParaInfo.OMS == 0x03)
                {
                    DRAGER_FLAG.OperateMode = TurnToMeasure;
                }else if(Drager_ParaInfo.OMS == 0x04)
                {
                    DRAGER_FLAG.OperateMode = TurnToStandby;
                }else if(Drager_ParaInfo.OMS == 0x05)
                {
                    DRAGER_FLAG.OperateMode = ClosingComponents;
                }
                //Drager_Para_Flag |= (1<<0);
                break;
            case 0x10:  //生理药剂1
                Drager_AA1_Data.FI = (Drager_Ack.FrameData.FrameData[0]<<8)|Drager_Ack.FrameData.FrameData[1];
                Drager_AA1_Data.ET = (Drager_Ack.FrameData.FrameData[2]<<8)|Drager_Ack.FrameData.FrameData[3];
                Drager_AA1_Data.IDMF = Drager_Ack.FrameData.FrameData[6];
                Drager_AA1_Data.PAS = Drager_Ack.FrameData.FrameData[7];
                Drager_AA1_Data.PID = Drager_Ack.FrameData.FrameData[8];
                Drager_AA1_Data.PS = Drager_Ack.FrameData.FrameData[9];
                Drager_AA1_Data.ParaMode = Drager_Ack.FrameData.FrameData[9]>>6;
                Drager_AA1_Data.ZeroState = (Drager_Ack.FrameData.FrameData[9]>>5)&0x01;
                Drager_AA1_Data.ZeroResult = (Drager_Ack.FrameData.FrameData[9]>>4)&0x01;
                Drager_AA1_Data.Accuracy = Drager_Ack.FrameData.FrameData[9]&0x03;
                Drager_AA1_Data.Unit = (Drager_Ack.FrameData.FrameData[9]>>3)&0x01;
                if(Drager_AA1_Data.IDMF&0x03)
                {
                    DRAGER_FLAG.SetAgent = AgentIdentifiaction;
                    DRAGER_FLAG.SetAgent_Finish_Flag = 1;
                }else
                {
                    DRAGER_FLAG.SetAgent = NoAgentAvailable;
                }
                break;
            case 0x11:  //生理药剂2
                Drager_AA2_Data.FI = (Drager_Ack.FrameData.FrameData[0]<<8)|Drager_Ack.FrameData.FrameData[1];
                Drager_AA2_Data.ET = (Drager_Ack.FrameData.FrameData[2]<<8)|Drager_Ack.FrameData.FrameData[3];
                Drager_AA2_Data.IDMF = 0;
                Drager_AA2_Data.PAS = Drager_Ack.FrameData.FrameData[7];
                Drager_AA2_Data.PID = Drager_Ack.FrameData.FrameData[8];
                Drager_AA2_Data.PS = Drager_Ack.FrameData.FrameData[9];
                Drager_AA2_Data.ParaMode = Drager_Ack.FrameData.FrameData[9]>>6;
                Drager_AA2_Data.ZeroState = (Drager_Ack.FrameData.FrameData[9]>>5)&0x01;
                Drager_AA2_Data.ZeroResult = (Drager_Ack.FrameData.FrameData[9]>>4)&0x01;
                Drager_AA2_Data.Accuracy = Drager_Ack.FrameData.FrameData[9]&0x03;
                Drager_AA2_Data.Unit = (Drager_Ack.FrameData.FrameData[9]>>3)&0x01;
                break;
            case 0x12:  //参数单位信息 djq del 20170313单位在0x03帧处得到，这里不需要
//                Drager_CO2_Data.Unit = Drager_Ack.FrameData.FrameData[0];
//                Drager_N2O_Data.Unit = Drager_Ack.FrameData.FrameData[1];
//                Drager_AA1_Data.Unit = Drager_Ack.FrameData.FrameData[2];
//                Drager_AA2_Data.Unit = Drager_Ack.FrameData.FrameData[3];
//                Drager_O2_Data.Unit = Drager_Ack.FrameData.FrameData[4];
                break;
            case 0x13:  //通知
                Drager_Message.HighPriority = (Drager_Ack.FrameData.FrameData[0]<<8)|Drager_Ack.FrameData.FrameData[1];
                Drager_Message.MedPriority = (Drager_Ack.FrameData.FrameData[2]<<8)|Drager_Ack.FrameData.FrameData[3];
                Drager_Message.LowPriority = (Drager_Ack.FrameData.FrameData[4]<<8)|Drager_Ack.FrameData.FrameData[5];
                //if(Drager_Message.HighPriority == 0x73)
                //{
                //    DRAGER_FLAG.ZeroState = 1;  //正在校零
                //}
                //else if(Drager_Message.HighPriority == 0x72)
                //{
                //    DRAGER_FLAG.ZeroState = 2;  //校零成功
                //}
                //else if(Drager_Message.HighPriority == 0x71)
                //{
                //    DRAGER_FLAG.ZeroState = 3;  //校零失败
                //}
                AnalysisDragerMessage(Drager_Message.HighPriority,Drager_Message.MedPriority,Drager_Message.LowPriority);
                break;
            default:
                break;
            }
            DRAGER_FLAG.ZeroRequest = Drager_Ack.FrameData.ModuleStatus & 0x01;
            DRAGER_FLAG.BreathPhase = (Drager_Ack.FrameData.ModuleStatus>>3) & 0x01;
            DRAGER_FLAG.CheckWaterOrLine = (Drager_Ack.FrameData.ModuleStatus>>2) & 0x01;
            DRAGER_FLAG.ComponentFail = (Drager_Ack.FrameData.ModuleStatus>>6) & 0x01;
            DRAGER_FLAG.BreathAvail = (Drager_Ack.FrameData.ModuleStatus>>4) & 0x01;
            DRAGER_FLAG.NorespOrApnea = (Drager_Ack.FrameData.ModuleStatus>>4) & 0x01;
            
            Drager_RT_Data.Num = 0;
            if(Drager_Ack.FrameData.RTDataMask&0x01)
            {
                Drager_RT_Data.CO2 = (Drager_Ack.FrameData.RTData[Drager_RT_Data.Num]<<8)
                    | Drager_Ack.FrameData.RTData[Drager_RT_Data.Num+1];
                Drager_RT_Data.Num += 2;
            }
            if((Drager_Ack.FrameData.RTDataMask>>1)&0x01)
            {
                Drager_RT_Data.N2O = (Drager_Ack.FrameData.RTData[Drager_RT_Data.Num]<<8)
                    | Drager_Ack.FrameData.RTData[Drager_RT_Data.Num+1];
                Drager_RT_Data.Num += 2;
            }
            if((Drager_Ack.FrameData.RTDataMask>>2)&0x01)
            {
                Drager_RT_Data.AA1 = (Drager_Ack.FrameData.RTData[Drager_RT_Data.Num]<<8)
                    | Drager_Ack.FrameData.RTData[Drager_RT_Data.Num+1];
                Drager_RT_Data.Num += 2;
            }
            if((Drager_Ack.FrameData.RTDataMask>>3)&0x01)
            {
                Drager_RT_Data.AA2 = (Drager_Ack.FrameData.RTData[Drager_RT_Data.Num]<<8)
                    | Drager_Ack.FrameData.RTData[Drager_RT_Data.Num+1];
                Drager_RT_Data.Num += 2;
            }
            if((Drager_Ack.FrameData.RTDataMask>>4)&0x01)
            {
                Drager_RT_Data.O2 = (Drager_Ack.FrameData.RTData[Drager_RT_Data.Num]<<8)
                    | Drager_Ack.FrameData.RTData[Drager_RT_Data.Num+1];
                //Drager_RT_Data.Num += 2;
            }
        }else
        {
            for(i = 0;i < 8;i ++)
                Drager_Ack.FrameData.SelectMaskErr[i] = RX_Drager[i+3];
            Drager_Ack.Err = RX_Drager[11];
        }
        Drager_Return_Flag |= (1<<3);
        break;
    case DRAGER_ID_STOPTRANS:  //停止连续传送
        if(RX_Drager[0] == 0x06)
        {
            Drager_Ack.StopConTrans = 1;
            DRAGER_FLAG.Connect_Flag = 1;
        }
        Drager_Return_Flag |= (1<<4);
        break;
    case DRAGER_ID_CHANGEUNITS:  //改变单位
        if(RX_Drager[0] == 0x06)
        {
            Drager_Ack.ChangeParaUnits = 1;
        }else
        {
            Drager_Ack.Err = RX_Drager[3];
        }
        Drager_Return_Flag |= (1<<5);
        break;
    case DRAGER_ID_ACCEPTPARA:  //接收额外参数
        if(RX_Drager[0] == 0x06)
        {
            Drager_Ack.AcceptExParaData = 1;
        }else
        {
            Drager_Ack.Err = RX_Drager[3];
        }
        Drager_Return_Flag |= (1<<6);
        break;
    case DRAGER_ID_SELECTAA:  //选择麻醉气体类型
        if(RX_Drager[0] == 0x06)
        {
            Drager_Ack.SelectAnestheticAgent = 1;
            DRAGER_FLAG.SetAgent_Finish_Flag = 1;
        }else
        {
            Drager_Ack.Err = RX_Drager[3];
        }
        Drager_Return_Flag |= (1<<7);
        break;
    case DRAGER_ID_CHANGEBREATH:  //改变呼吸探测模式
        if(RX_Drager[0] == 0x06)
        {
            Drager_Ack.SwitchBreathMode = 1;
        }else
        {
            Drager_Ack.Err = RX_Drager[3];
        }
        Drager_Return_Flag |= (1<<8);
        break;
    case DRAGER_ID_INITZERO:  //初始化零点
        if(RX_Drager[0] == 0x06)
        {
            Drager_Ack.InitiateZero = 1;
        }else
        {
            Drager_Ack.Err = RX_Drager[3];
        }
        Drager_Return_Flag |= (1<<9);
        break;
    case DRAGER_ID_ADJUSTTIME:  //调整时间信息
        if(RX_Drager[0] == 0x06)
        {
            Drager_Ack.Adjusttime = 1;
        }else
        {
            Drager_Ack.Err = RX_Drager[3];
        }
        Drager_Return_Flag |= (1<<10);
        break;
    case DRAGER_ID_TRANSMODULEMASK:  //传输模块特征
        if(RX_Drager[0] == 0x06)
        {
            for(i = 0;i < 4;i ++)
            {
                Drager_Ack.GenericModuleMask[i] = RX_Drager[i+3];
            }
            Drager_ModuleFeature.PnueCtrl = (Drager_Ack.GenericModuleMask[3]>>2)&0x01;
            Drager_ModuleFeature.PenuAvail = (Drager_Ack.GenericModuleMask[3]>>1)&0x01;
            Drager_ModuleFeature.ZeroControl = Drager_Ack.GenericModuleMask[3]&0x01;
        }else
        {
            Drager_Ack.Err = RX_Drager[3];
        }
        Drager_Return_Flag |= (1<<11);
        break;
    case DRAGER_ID_SWITCHVALVES:  //切换阀
        if(RX_Drager[0] == 0x06)
        {
            Drager_Ack.SwitchValves = 1;
        }else
        {
            Drager_Ack.Err = RX_Drager[3];
        }
        Drager_Return_Flag |= (1<<12);
        break;
    case DRAGER_ID_SWITCHPUMP:  //切换泵
        if(RX_Drager[0] == 0x06)
        {
            Drager_Ack.SwitchPump = Drager_Cmd.SwitchPump;
            if(Drager_Ack.SwitchPump == 0x00)
            {
                DRAGER_FLAG.PumpState = 0;
            }else if(Drager_Ack.SwitchPump == 0x02)
            {
                DRAGER_FLAG.PumpState = 1;
            }
            Drager_Para_Flag |= (1<<15);
        }else
        {
            Drager_Ack.Err = RX_Drager[3];
        }
        Drager_Return_Flag |= (1<<13);
        break;
    default:
        break;
    }
    for(i = 0;i < sizeof(RX_Drager);i ++)
    {
        RX_Drager[i] = 0;
    }
    if(Drager_Ack.Err)
    {
        AnalysisDragerErr(Drager_Ack.Err);
        //return IM_ERR;
        return IM_OK;
    }
    else
    {
        //AnalysisDragerErr(0);
        return IM_OK;
    }
}

/*
****************************************************************************************************
函数名称：Check_Drager_Sum
入口参数：无
出口参数：无
功能描述：计算校验和
****************************************************************************************************
*/
u8 CalculateCheckSum(u8 len,u8* tx_buffer)
{
    u8 CS = 0;
    int i = 0;
    for(i = 0;i < len;i ++)
    {
        CS = CS + *(tx_buffer+i);
    }
    CS = ~CS + 1;
    return CS;
}

/*
****************************************************************************************************
函数名称：Drager_SetBaseTime
入口参数：BaseTime
出口参数：无
功能描述：设置基本时间间隔
****************************************************************************************************
*/
//void Drager_SetBaseTime(u16 basetime)
//{
//  Drager_Cmd.BaseTime = basetime;
//  TX_Drager[0] = 0x10;
//  TX_Drager[1] = 0x02;
//  DragerSendDataNum = 2;
//  TX_Drager[2] = DRAGER_ID_BASETIME;
//  TX_Drager[3] = Drager_Cmd.BaseTime;
//  TX_Drager[4] = CalculateCheckSum(DragerSendDataNum+2);
//  //(*Drager_Send)(TX_Drager,DragerSendDataNum+3);
//}

/*
****************************************************************************************************
函数名称：Drager_GetBaseTime
入口参数：BaseTime,tx_array
出口参数：无
功能描述：获取基本时间间隔
****************************************************************************************************
*/
void Drager_GetBaseTime(u8* tx_buffer)
{
    *tx_buffer = 0x10;
    *(tx_buffer+1) = 0x02;
    DragerSendDataNum = 2;
    *(tx_buffer+2) = DRAGER_ID_BASETIME;
    *(tx_buffer+3) = 0xFF;
    *(tx_buffer+4) = CalculateCheckSum(DragerSendDataNum+2,tx_buffer);
}
/*
****************************************************************************************************
函数名称：Drager_SetOpMode
入口参数：u8 OpMode,u8* tx_buffer
出口参数：u8
功能描述：设置工作模式
****************************************************************************************************
*/
u8 Drager_SetOpMode(u8 opmode,u8* tx_buffer)
{
    Drager_Cmd.OperatMode = opmode;
    *tx_buffer = 0x10;
    *(tx_buffer+1) = 0x02;
    DragerSendDataNum = 2;
    *(tx_buffer+2) = DRAGER_ID_OPERATION;
    *(tx_buffer+3) = Drager_Cmd.OperatMode;
    *(tx_buffer+4) = CalculateCheckSum(DragerSendDataNum+2,tx_buffer);
    return DragerSendDataNum+3;
    //(*Drager_Send)(TX_Drager,DragerSendDataNum+3);
}
/*
****************************************************************************************************
函数名称：Drager_GetOpMode
入口参数：无
出口参数：全局变量：Drager_OperateMode
功能描述：获取当前工作模式
****************************************************************************************************
*/
void Drager_GetOpMode(void)
{
    Drager_GetPatientData(DRAGER_PARINFO,DRAGER_RT_NONE,0,TX_Drager);
}
/*
****************************************************************************************************
函数名称：Drager_GetDivInfo
入口参数：infoselect,供应商代码：0x00，序列号:0x01，硬件版本：0x02，
软件版本：0x03,产品名称：0x05，零件号码：0x06
出口参数：长度
功能描述：获取设备信息,目前只使用序列号
****************************************************************************************************
*/
u8 Drager_GetDivInfo(u8 infoselect,u8* tx_buffer)
{
    int i = 0;
    Drager_Cmd.InformationSelect = infoselect;
    *tx_buffer = 0x10;
    *(tx_buffer+1) = 0x0A;
    DragerSendDataNum = 10;
    *(tx_buffer+2) = DRAGER_ID_INFORMATION;
    for(i = 0;i < 8;i ++)
        *(tx_buffer+3+i) = 0x00;
    *(tx_buffer+11) = Drager_Cmd.InformationSelect;
    *(tx_buffer+12) = CalculateCheckSum(DragerSendDataNum+2,tx_buffer);
    //(*Drager_Send)(TX_Drager,DragerSendDataNum+3);
}
/*
****************************************************************************************************
函数名称：Drager_GetPatientData
入口参数：参数选择,RTMask,INTS
1、选择码：bit0：CO2、N2O浓度，bit1：O2浓度，bit2：压力和泵流量，bit3：呼吸信息
bit4：参数详细状况，bit5：生理药剂1，bit6：生理药剂2，bit7：参数单位信息，bit8：通知信息
2、实时参数码：bit0：CO2，bit1：N2O，bit2：生理药剂1，bit3：生理药剂2，bit4：O2，bit14：数据包数量
3、设置时间间隔，连续数据发送时间=基本时间间隔*设置时间间隔（ms）
出口参数：长度
功能描述：获取病人信息
****************************************************************************************************
*/
u8 Drager_GetPatientData(u16 framemask,u16 rtmask,u16 ints,u8* tx_buffer)
{
    int i = 0;
    Drager_Cmd.FrameData.RTDataMask = rtmask;
    Drager_Cmd.FrameData.INT = ints;
    *tx_buffer = 0x10;
    *(tx_buffer+1) = 0x0D;
    DragerSendDataNum = 13;
    *(tx_buffer+2) = DRAGER_ID_FRAMEDATA;
    for(i = 0;i < 8;i ++)
    {
        Drager_Cmd.FrameData.FrameSelectMask[i] = 0x00;
        *(tx_buffer+3+i) = 0x00;
    }
    if(framemask & DRAGER_CO2ANDN2O)
    {
        Drager_Cmd.FrameData.FrameSelectMask[7] |= 0x08;
        *(tx_buffer+10) |= 0x08;
    }
    if(framemask & DRAGER_O2)
    {
        Drager_Cmd.FrameData.FrameSelectMask[7] |= 0x10;
        *(tx_buffer+10) |= 0x10;
    }
    if(framemask & DRAGER_PRESSUREANDPUMP)
    {
        Drager_Cmd.FrameData.FrameSelectMask[6] |= 0x08;
        *(tx_buffer+9) |= 0x08;
    }
    if(framemask & DRAGER_BREATHINFO)
    {
        Drager_Cmd.FrameData.FrameSelectMask[6] |= 0x20;
        *(tx_buffer+9) |= 0x20;
    }
    if(framemask & DRAGER_PARINFO)
    {
        Drager_Cmd.FrameData.FrameSelectMask[6] |= 0x40;
        *(tx_buffer+9) |= 0x40;
    }
    if(framemask & DRAGER_AA1)
    {
        Drager_Cmd.FrameData.FrameSelectMask[5] |= 0x01;
        *(tx_buffer+8) |= 0x01;
    }
    if(framemask & DRAGER_AA2)
    {
        Drager_Cmd.FrameData.FrameSelectMask[5] |= 0x02;
        *(tx_buffer+8) |= 0x02;
    }
    if(framemask & DRAGER_PARUNIT)
    {
        Drager_Cmd.FrameData.FrameSelectMask[5] |= 0x04;
        *(tx_buffer+8) |= 0x04;
    }
    if(framemask & DRAGER_MESSAGE)
    {
        Drager_Cmd.FrameData.FrameSelectMask[5] |= 0x08;
        *(tx_buffer+8) |= 0x08;
    }
    *(tx_buffer+11) = (Drager_Cmd.FrameData.RTDataMask>>8)&0xff;
    *(tx_buffer+12) = Drager_Cmd.FrameData.RTDataMask&0xff;
    *(tx_buffer+13) = (Drager_Cmd.FrameData.INT>>8)&0xff;
    *(tx_buffer+14) = Drager_Cmd.FrameData.INT&0xff;
    *(tx_buffer+15) = CalculateCheckSum(DragerSendDataNum+2,tx_buffer);
    return DragerSendDataNum+3;
    //(*Drager_Send)(TX_Drager,DragerSendDataNum+3);
}
/*
****************************************************************************************************
函数名称：Drager_StopContinueTrans
入口参数：无
出口参数：长度
功能描述：停止连续信息发送
****************************************************************************************************
*/
u8 Drager_StopContinueTrans(u8* tx_buffer)
{
    *tx_buffer = 0x10;
    *(tx_buffer+1) = 0x01;
    DragerSendDataNum = 1;
    *(tx_buffer+2) = DRAGER_ID_STOPTRANS;
    *(tx_buffer+3) = CalculateCheckSum(DragerSendDataNum+2,tx_buffer);
    return DragerSendDataNum+3;
    //(*Drager_Send)(TX_Drager,DragerSendDataNum+3);
}
/*
****************************************************************************************************
函数名称：Drager_ChangeParaUnit
入口参数：参数选择码:DRAGER_CO2/DRAGER_N2O/DRAGER_AA1/DRAGER_AA2/DRAGER_O2
(bit0：O2，bit1：N2O，bit2：AA1，bit3：AA2，bit4：O2)
单位:ATS：DRAGER_UNIT_ATS，ATPS：DRAGER_UNIT_ATPS
出口参数：无
功能描述：改变参数单位
****************************************************************************************************
*/
u8 Drager_ChangeParaUnit(u16 paramask,u8 unit,u8* tx_buffer)
{
    //Drager_Cmd.ChangeParaUnits.ParaSelectMask = paramask;
    Drager_Cmd.ChangeParaUnits.Unit = unit;
    *tx_buffer = 0x10;
    *(tx_buffer+1) = 0x04;
    DragerSendDataNum = 4;
    *(tx_buffer+2) = DRAGER_ID_CHANGEUNITS;
    //TX_Drager[3] = (Drager_Cmd.ChangeParaUnits.ParaSelectMask>>8)&0xff;
    //TX_Drager[4] = Drager_Cmd.ChangeParaUnits.ParaSelectMask&0xff;
    *(tx_buffer+3) = 0x00;
    *(tx_buffer+4) = 0x00;
    if(paramask & DRAGER_CO2)
    {
        *(tx_buffer+4) |= 0x01;
    }
    if(paramask & DRAGER_N2O)
    {
        *(tx_buffer+4) |= 0x02;
    }
    if(paramask & DRAGER_AA1)
    {
        *(tx_buffer+4) |= 0x04;
    }
    if(paramask & DRAGER_AA2)
    {
        *(tx_buffer+4) |= 0x08;
    }
    if(paramask & DRAGER_O2)
    {
        *(tx_buffer+4) |= 0x10;
    }
    Drager_Cmd.ChangeParaUnits.ParaSelectMask = ((*(tx_buffer+4))<<8)|(*(tx_buffer+5));
    *(tx_buffer+5) = Drager_Cmd.ChangeParaUnits.Unit;
    *(tx_buffer+6) = CalculateCheckSum(DragerSendDataNum+2,tx_buffer);
    return DragerSendDataNum+3;
    //(*Drager_Send)(TX_Drager,DragerSendDataNum+3);
}
/*
****************************************************************************************************
函数名称：Drager_Accessexpara
入口参数：值选择码，值，参数模式
出口参数：长度
功能描述：接收额外参数
****************************************************************************************************
*/
u8 Drager_Accessexpara(u16 valuemask,u16 value,u8 paramode,u8* tx_buffer)
{
    Drager_Cmd.AcceptExPara.ValueSelectMask = valuemask;
    Drager_Cmd.AcceptExPara.Value = value;
    Drager_Cmd.AcceptExPara.ParaMode = paramode;
    *tx_buffer = 0x10;
    *(tx_buffer+1) = 0x06;
    DragerSendDataNum = 6;
    *(tx_buffer+2) = DRAGER_ID_ACCEPTPARA;
    *(tx_buffer+3) = (Drager_Cmd.AcceptExPara.ValueSelectMask>>8)&0xff;
    *(tx_buffer+4) = Drager_Cmd.AcceptExPara.ValueSelectMask&0xff;
    *(tx_buffer+5) = (Drager_Cmd.AcceptExPara.Value>>8)&0xff;
    *(tx_buffer+6) = Drager_Cmd.AcceptExPara.Value&0xff;
    *(tx_buffer+7) = Drager_Cmd.AcceptExPara.ParaMode;
    *(tx_buffer+8) = CalculateCheckSum(DragerSendDataNum+2,tx_buffer);
    return DragerSendDataNum+3;
    //(*Drager_Send)(TX_Drager,DragerSendDataNum+3);
}
/*
****************************************************************************************************
函数名称：Drager_SelectAA
入口参数：anestheticagent,自动识别：AUTOIDENTIFICATION，氟烷：HALOTHANE，安氟醚：ENFLURANE，异氟醚：ISOFLURANE，
七氟醚：SEVOFLURANE，地氟醚：DESFLURANE，不改变：DONOTCHANGE
出口参数：长度
功能描述：选择麻醉气体
****************************************************************************************************
*/
u8 Drager_SelectAA(u8 anestheticagent,u8* tx_buffer)
{
    Drager_Cmd.SelectAnestheticAgent = anestheticagent;
    *tx_buffer = 0x10;
    *(tx_buffer+1) = 0x03;
    DragerSendDataNum = 3;
    *(tx_buffer+2) = DRAGER_ID_SELECTAA;
    *(tx_buffer+3) = Drager_Cmd.SelectAnestheticAgent;
    *(tx_buffer+4) = 0x00;
    *(tx_buffer+5) = CalculateCheckSum(DragerSendDataNum+2,tx_buffer);
    return DragerSendDataNum+3;
    //(*Drager_Send)(TX_Drager,DragerSendDataNum+3);
}
/*
****************************************************************************************************
函数名称：Drager_ChangeBreathMode
入口参数：breathmode,程序探测：0x01，自动激活：0x02，在呼吸相1后自动唤醒：0x05,..2:0x06,..3:0x07,..4:0x08,..5:0x09
出口参数：长度
功能描述：改变呼吸探测模式
****************************************************************************************************
*/
u8 Drager_ChangeBreathMode(u8 breathmode,u8* tx_buffer)
{
    Drager_BreathDetectionMode = breathmode;
    Drager_Cmd.SwitchBreathMode = breathmode;
    *tx_buffer = 0x10;
    *(tx_buffer+1) = 0x02;
    DragerSendDataNum = 2;
    *(tx_buffer+2) = DRAGER_ID_CHANGEBREATH;
    *(tx_buffer+3) = Drager_Cmd.SwitchBreathMode;
    *(tx_buffer+4) = CalculateCheckSum(DragerSendDataNum+2,tx_buffer);
    return DragerSendDataNum+3;
    //(*Drager_Send)(TX_Drager,DragerSendDataNum+3);
}
/*
****************************************************************************************************
函数名称：Drager_InitZero
入口参数：zeropara，zerotype
调零参数：必须参数：0x00，所有参数：0x01
校零类型：室空气零点：0x01，无零点请求：0xFF
出口参数：长度
功能描述：手动校零
****************************************************************************************************
*/
u8 Drager_InitZero(u8* tx_buffer)  //(u8 zeropara,u8 zerotype)
{
    int i = 0;
    Drager_Cmd.InitiateZero.ParaToZero = 0x01;//zeropara;
    Drager_Cmd.InitiateZero.ZeroType = 0x01;//zerotype;
    Drager_Cmd.InitiateZero.ZeroMode = 0x00;
    *tx_buffer = 0x10;
    *(tx_buffer+1) = 0x0B;
    DragerSendDataNum = 11;
    *(tx_buffer+2) = DRAGER_ID_INITZERO;
    *(tx_buffer+3) = 0x00;
    *(tx_buffer+4) = 0x00;
    *(tx_buffer+5) = Drager_Cmd.InitiateZero.ParaToZero;
    for(i = 0;i < 5;i ++)
        *(tx_buffer+6+i) = 0x00;
    *(tx_buffer+11) = Drager_Cmd.InitiateZero.ZeroType;
    *(tx_buffer+12) = Drager_Cmd.InitiateZero.ZeroMode;
    *(tx_buffer+13) = CalculateCheckSum(DragerSendDataNum+2,tx_buffer);
    return DragerSendDataNum+3;
    //(*Drager_Send)(TX_Drager,DragerSendDataNum+3);
}
/*
****************************************************************************************************
函数名称：Drager_Adjusttime
入口参数：hour,minu,sec,month,day,year
出口参数：无
功能描述：调整时间信息
****************************************************************************************************
*/
void Drager_Adjusttime(u8 hour,u8 minu,u8 sec,u8 day,u8 month,u8 year,u8* tx_buffer)
{
    int i = 0;
    Drager_Cmd.Adjusttime.TimeInfoSelector = 0x00;
    *tx_buffer = 0x10;
    *(tx_buffer+1) = 0x09;
    DragerSendDataNum = 9;
    *(tx_buffer+2) = DRAGER_ID_ADJUSTTIME;
    Drager_Cmd.Adjusttime.TimeData[0] = hour;
    Drager_Cmd.Adjusttime.TimeData[1] = minu;
    Drager_Cmd.Adjusttime.TimeData[2] = sec;
    Drager_Cmd.Adjusttime.TimeData[3] = day;
    Drager_Cmd.Adjusttime.TimeData[4] = month;
    Drager_Cmd.Adjusttime.TimeData[5] = year;
    for(i = 0;i < 6;i ++)
    {
        *(tx_buffer+3+i) = Drager_Cmd.Adjusttime.TimeData[i];
    }
    *(tx_buffer+9) = 0x00;
    *(tx_buffer+10) = Drager_Cmd.Adjusttime.TimeInfoSelector;
    *(tx_buffer+11) = CalculateCheckSum(DragerSendDataNum+2,tx_buffer);
    //(*Drager_Send)(TX_Drager,DragerSendDataNum+3);
}
/*
****************************************************************************************************
函数名称：Drager_TransModuleMask
入口参数：无
出口参数：无
功能描述：传输模块信息
****************************************************************************************************
*/
void Drager_TransModuleMask(u8* tx_buffer)
{
    *tx_buffer = 0x10;
    *(tx_buffer+1) = 0x01;
    DragerSendDataNum = 1;
    *(tx_buffer+2) = DRAGER_ID_TRANSMODULEMASK;
    *(tx_buffer+3) = CalculateCheckSum(DragerSendDataNum+2,tx_buffer);
    //(*Drager_Send)(TX_Drager,DragerSendDataNum+3);
}
/*
****************************************************************************************************
函数名称：Drager_SwitchValves
入口参数：valve，采样气体：DRAGER_VALVE_SAMPLE，室空气：DRAGER_VALVE_ROOM
出口参数：无
功能描述：切换阀
****************************************************************************************************
*/
u8 Drager_SwitchValves(u8 valve,u8* tx_buffer)
{
    Drager_Cmd.SwitchValves = valve;
    *tx_buffer = 0x10;
    *(tx_buffer+1) = 0x02;
    DragerSendDataNum = 2;
    *(tx_buffer+2) = DRAGER_ID_SWITCHVALVES;
    *(tx_buffer+3) = Drager_Cmd.SwitchValves;
    *(tx_buffer+4) = CalculateCheckSum(DragerSendDataNum+2,tx_buffer);
    return DragerSendDataNum+3;
    //(*Drager_Send)(TX_Drager,DragerSendDataNum+3);
}
/*
****************************************************************************************************
函数名称：Drager_SwitchPumps
入口参数：pump,无流量：DRAGER_PUMP_OFF，高流量：DRAGER_PUMP_HIGH
出口参数：无
功能描述：切换泵
****************************************************************************************************
*/
u8 Drager_SwitchPumps(u8 pump,u8* tx_buffer)
{
    Drager_Cmd.SwitchPump = pump;
    *tx_buffer = 0x10;
    *(tx_buffer+1) = 0x02;
    DragerSendDataNum = 2;
    *(tx_buffer+2) = DRAGER_ID_SWITCHPUMP;
    *(tx_buffer+3) = Drager_Cmd.SwitchPump;
    *(tx_buffer+4) = CalculateCheckSum(DragerSendDataNum+2,tx_buffer);
    return DragerSendDataNum+3;
    //(*Drager_Send)(TX_Drager,DragerSendDataNum+3);
}
/*
****************************************************************************************************
函数名称：AnalysisDragerMessage
入口参数：u8 high_message,u8 med_message,u8 low_message
出口参数：无
功能描述：错误码分析,注意处理之后要清除
****************************************************************************************************
*/
void AnalysisDragerMessage(u8 high_message,u8 med_message,u8 low_message)
{
    u8 now_message = 0;
    int i = 0;
    if((high_message == DragerMSG_No_Message) && 
       (med_message == DragerMSG_No_Message) && 
           (low_message == DragerMSG_No_Message))
    {
        for(i = 0;i < 5;i ++)
        {
            Drager_Message_Register[i] = 0;
        }
    }
    else
    {
        if(high_message != 0)
        {
            now_message = high_message;
        }
        else if(med_message != 0)
        {
            now_message = med_message;
        }
        else
        {
            now_message = low_message;
        }
        switch(now_message)
        {
        case DragerMSG_No_Message:
            for(i = 0;i < 5;i ++)
            {
                Drager_Message_Register[i] = 0;
            }
            break;
        case DragerMSG_Supply_Voltage_Bad:  //bit0
            Drager_Message_Register[0] |= 0x01;
            break;
        case DragerMSG_CO2sensor_Hardware_Fail: //bit1
            Drager_Message_Register[0] |= 0x02;
            break;
        case DragerMSG_O2sensor_Hardware_Fail:  //bit2
            Drager_Message_Register[0] |= 0x04;
            break;
        case DragerMSG_Gas_Inlet_Leakage:       //bit3
            Drager_Message_Register[0] |= 0x08;
            break;
        case DragerMSG_Gas_Inlet_Occlusion:     //bit4
            Drager_Message_Register[0] |= 0x10;
            break;      
        case DragerMSG_Gas_Outlet_Occulusion:   //bit5
            Drager_Message_Register[0] |= 0x20;
            break;
        case DragerMSG_Occulusion:              //bit6
            Drager_Message_Register[0] |= 0x40;
            break;
        case DragerMSG_Pneum_Hardware_Error:    //bit7
            Drager_Message_Register[0] |= 0x80;
            break;
        case DragerMSG_Check_Flow:              //bit0
            Drager_Message_Register[1] |= 0x01;
            break;
        case DragerMSG_SampleLine_NotConnected: //bit1
            Drager_Message_Register[1] |= 0x02;
            break;   
        case DragerMSG_SampleLNotC_or_GasInLeak://bit2
            Drager_Message_Register[1] |= 0x04;
            break;
        case DragerMSG_Check_Watertrap:         //bit3
            Drager_Message_Register[1] |= 0x08;
            break;
        case DragerMSG_Watertrap_Full:          //bit4
            Drager_Message_Register[1] |= 0x10;
            break;
        case DragerMSG_CheckWater_or_GasInOcculsion:    //bit5
            Drager_Message_Register[1] |= 0x20;
            break;
        case DragerMSG_WaterFull_or_GasInOcculsion:     //bit6
            Drager_Message_Register[1] |= 0x40;
            break;  
        case DragerMSG_Fan_Not_turning:         //bit7
            Drager_Message_Register[1] |= 0x80;
            break;
        case DragerMSG_Warm_up:                 //bit0
            Drager_Message_Register[2] |= 0x01;
            break;
        case DragerMSG_Sensor_Too_Warm:         //bit1
            Drager_Message_Register[2] |= 0x02;
            break;
        case DragerMSG_Zero_Required:           //bit2
            Drager_Message_Register[2] |= 0x04;
            break;
        case DragerMSG_Zero_Failed:             //bit3
            Drager_Message_Register[2] |= 0x08;
            break;            
        case DragerMSG_Zero_Ok:                 //bit4
            Drager_Message_Register[2] |= 0x10;
            break;
        case DragerMSG_Zero_In_Progress:        //bit5
            Drager_Message_Register[2] |= 0x20;
            break;
        case DragerMSG__Zero_Canceled:          //bit6
            Drager_Message_Register[2] |= 0x40;
            break;
        case DragerMSG_Calibration_Required:    //bit7
            Drager_Message_Register[2] |= 0x80;
            break;
        case DragerMSG_Standby_Mode:            //bit0
            Drager_Message_Register[3] |= 0x01;
            break;                 
        case DragerMSG_Switched_Off_Mode:       //bit1
            Drager_Message_Register[3] |= 0x02;
            break;
        case DragerMSG_Pump_Switched_Off:       //bit2
            Drager_Message_Register[3] |= 0x04;
            break;
        case DragerMSG_Valve_Not_Sample:        //bit3
            Drager_Message_Register[3] |= 0x08;
            break;
        case DragerMSG_Version_Mismatch:        //bit4
            Drager_Message_Register[3] |= 0x10;
            break;
        case DragerMSG_Download_Incomplete:     //bit5
            Drager_Message_Register[3] |= 0x20;
            break;          
        case DragerMSG_Pneum_Not_Avail:         //bit6
            Drager_Message_Register[3] |= 0x40; 
            break;
        case DragerMSG_CO2sensor_Not_Avail:     //bit7
            Drager_Message_Register[3] |= 0x80;
            break;
        case DragerMSG_O2sensor_Not_Avail:      //bit0
            Drager_Message_Register[4] |= 0x01;
            break;
        case DragerMSG_Pneum_Lost:              //bit1
            Drager_Message_Register[4] |= 0x02;
            break;
        case DragerMSG_CO2sensor_Lost:          //bit2
            Drager_Message_Register[4] |= 0x04;
            break;               
        case DragerMSG_O2sensor_Lost:           //bit3
            Drager_Message_Register[4] |= 0x08;
            break; 
        default:
            break;
        }
    }
}
/*
****************************************************************************************************
函数名称：AnalysisDragerErr
入口参数：err
出口参数：无
功能描述：错误码分析
****************************************************************************************************
*/
void AnalysisDragerErr(u8 err)
{
    int i = 0;
    switch(err)
    {
    case 0x00:
        for(i = 0;i < 5;i ++)
        {
            Drager_Message_Register[i+5] = 0;
        }
        break;
    case 0x01:  //有子部件正在工作
        Drager_Message_Register[5] |= 0x01;
        break;
    case 0x02:  //错误的参数
        Drager_Message_Register[5] |= 0x02;
        break;
    case 0x03:  //错误的单位
        Drager_Message_Register[5] |= 0x04;
        break;
    case 0x04:  //气体不支持
        Drager_Message_Register[5] |= 0x08;
        break;
    case 0x10:  //参数不支持
        Drager_Message_Register[5] |= 0x10;
        break;
    case 0x11:  //参数不允许
        Drager_Message_Register[5] |= 0x20;
        break;
    case 0x12:  //数据包不支持
        Drager_Message_Register[5] |= 0x40;
        break;
    case 0x13:  //实时数据不支持
        Drager_Message_Register[5] |= 0x80;
        break;
    case 0x14:  //错误的基本间隔时间
        Drager_Message_Register[6] |= 0x01;
        break;
    case 0x20:  //EEPROM访问错误
        Drager_Message_Register[6] |= 0x02;
        break;
    case 0x22:  //非易变性存储器访问错误
        Drager_Message_Register[6] |= 0x04;
        break;
    case 0x31:  //脱水器满
        Drager_Message_Register[6] |= 0x08;
        break;
    case 0x74:  //校验和错误
        Drager_Message_Register[6] |= 0x10;
        break;
    case 0x79:  //校验值保存失败恢复旧值
        Drager_Message_Register[6] |= 0x20;
        break;
    case 0x7A:  //校验值保存失败旧值丢失
        Drager_Message_Register[6] |= 0x40;
        break;
    case 0x7B:  //硬件管理EEPROM访问失败
        Drager_Message_Register[6] |= 0x80;
        break;
    case 0x7C:  //获取软件错误失败
        Drager_Message_Register[7] |= 0x01;
        break;
    case 0x90:  //校验取消
        Drager_Message_Register[7] |= 0x02;
        break;
    case 0x91:  //无可用校验数据
        Drager_Message_Register[7] |= 0x04;
        break;
    case 0x92:  //只采集校验数据
        Drager_Message_Register[7] |= 0x08;
        break;
    case 0x93:  //校验数据传输
        Drager_Message_Register[7] |= 0x10;
        break;
    case 0xA0:  //延时时间为0
        Drager_Message_Register[7] |= 0x20;
        break;
    case 0xA4:  //数据不可用
        Drager_Message_Register[7] |= 0x40;
        break;
    case 0xA5:  //气体零点参数错误
        Drager_Message_Register[7] |= 0x80;
        break;
    case 0xC0:  //子部件对这个功能不可用
        Drager_Message_Register[8] |= 0x01;
        break;
    case 0xC1:  //子部件对这个模式不支持
        Drager_Message_Register[8] |= 0x02;
        break;
    case 0xCE:  //写访问不允许
        Drager_Message_Register[8] |= 0x04;
        break;
    case 0xCF:  //信息不存在
        Drager_Message_Register[8] |= 0x08;
        break;
    case 0xFF:  //未知的命令
        Drager_Message_Register[8] |= 0x10;
        break;
    default:
        break;
    }
}
/*
****************************************************************************************************
函数名称：Drager_Err
入口参数：err
出口参数：无
功能描述：错误码分析
****************************************************************************************************
*/
void Drager_Err(u8 err)
{
    switch(err)
    {
    case DRAGER_ERR_STANDBY_MODE:  //模块是待机状态，module is in standby mode
        break;
    case 0x02:  //错误的参数
        break;
    case 0x03:  //错误的单位
        break;
    case 0x04:  //气体不支持
        break;
    case 0x10:  //参数不支持
        break;
    default:
        break;
    }
}

/*
****************************************************************************************************
函数名称：Drager_SetConnect
入口参数：无
出口参数：无
功能描述：与模块建立连接, 开泵，校零
****************************************************************************************************
*/
void Drager_SetConnect(void)
{
    static u8 temp_pump = 0;
    if(DRAGER_FLAG.Connect_Flag == 0)
    {
        Drager_StopContinueTrans(TX_Drager);
    }
    if(temp_pump == 0)
    {
        Drager_TransModuleMask(TX_Drager);
        //    if((Drager_Return_Flag>>11)&0x01)
        //    {
        //      if(Drager_ModuleFeature.PenuAvail)
        //      {
        //        if(Drager_ModuleFeature.PnueCtrl)
        //        {
        //          temp_pump = 1;
        //        }else
        //        {
        //          Drager_SwitchPumps(0x00); //开机默认泵关
        //          if((Drager_Return_Flag>>13)&0x01)
        //          {
        //            temp_pump = 1;
        //          }
        //        }
        //      }else
        //      {
        //        temp_pump = 1;
        //      }
        //    }
        temp_pump = 1;
    }
}

/*
****************************************************************************************************
函数名称：Drager_JudgeAgent
入口参数：无
出口参数：0：无,1：需要手动设置，2：无需设置，自动识别
功能描述：判断麻醉气体,进入条件：DRAGER_FLAG.SetAgent_Finish_Flag = 0
若返回值为1，则需发送选择麻醉气体的命令。
****************************************************************************************************
*/
u8 Drager_JudgeAgent(void)
{
    if(DRAGER_FLAG.SetAgent == NoneAgent)
    {
        Drager_GetPatientData(DRAGER_PARINFO,DRAGER_RT_NONE,0,TX_Drager);
    }else if(DRAGER_FLAG.SetAgent == AgentAvailable)
    {
        Drager_GetPatientData(DRAGER_AA1,DRAGER_RT_NONE,0,TX_Drager);
    }
    if(DRAGER_FLAG.SetAgent == NoAgentAvailable)
    {
        return 1;
    }else if(DRAGER_FLAG.SetAgent == AgentIdentifiaction)
    {
        return 2;
    }else
    {
        return 0;
    }
}
/*
****************************************************************************************************
函数名称：Drager_GetAAData
入口参数：channel:DRAGER_AA1/DRAGER_AA2
出口参数：错误代码
功能描述：获取麻醉气体数据
****************************************************************************************************
*/
u16 Drager_GetAAData(u16 channel)
{
    u16 temp = 0;
    if(DRAGER_FLAG.OperateMode == Standby_Mode)
    {
        temp = DRAGER_ERR_STANDBY_MODE;
        return temp;
    }else
    {
        if(channel == DRAGER_AA1)
        {
            if((Drager_ParaInfo.PAI>>2)&0x01)
            {
                if((Drager_ParaInfo.PII>>2)&0x01)
                {
                    temp = DRAGER_ERR_AA1_NOOPERATE;
                    return temp;
                }else
                {
                    if(Drager_Message.HighPriority == 0x23) //堵塞
                    {
                        temp = DRAGER_ERR_OCCLUSION;
                        return temp;
                    }else
                    {
                        if(Drager_AA1_Data.ZeroState)
                        {
                            temp = DRAGER_ERR_ZEROINPROGRESS;
                            return temp;
                        }else
                        {
                            if(Drager_AA1_Data.Accuracy >= 2)
                            {
                                return 0; //数据正常
                            }else
                            {
                                if(Drager_ParaInfo.MISCSTAT&0x01)
                                {
                                    temp = DRAGER_ERR_WARMUP;
                                }else
                                {
                                    temp = DRAGER_ERR_NOACCURACY;
                                }
                                return temp;
                            }
                        }
                    }
                }
            }else //(Drager_ParaInfo.PAI>>2)&0x01 == 0
            {
                temp = DRAGER_ERR_AA1_NOAVAILABLE;
                return temp;
            }
        }//if(channel == DRAGER_AA1)
        else if(channel == DRAGER_AA2)
        {
            if((Drager_ParaInfo.PAI>>3)&0x01)
            {
                if((Drager_ParaInfo.PII>>3)&0x01)
                {
                    temp = DRAGER_ERR_AA2_NOOPERATE;
                    return temp;
                }else
                {
                    if(Drager_Message.HighPriority == 0x23) //堵塞
                    {
                        temp = DRAGER_ERR_OCCLUSION;
                        return temp;
                    }else
                    {
                        if(Drager_AA2_Data.ZeroState)
                        {
                            temp = DRAGER_ERR_ZEROINPROGRESS;
                            return temp;
                        }else
                        {
                            if(Drager_AA2_Data.Accuracy >= 2)
                            {
                                return 0; //数据正常
                            }else
                            {
                                if(Drager_ParaInfo.MISCSTAT&0x01)
                                {
                                    temp = DRAGER_ERR_WARMUP;
                                }else
                                {
                                    temp = DRAGER_ERR_NOACCURACY;
                                }
                                return temp;
                            }
                        }
                    }
                }
            }else //(Drager_ParaInfo.PAI>>2)&0x01 == 0
            {
                temp = DRAGER_ERR_AA2_NOAVAILABLE;
                return temp;
            }
        }//if(channel == DRAGER_AA2)
        else
        {
            return DRAGER_ERR_INPUTPARAWRONG;
        }
    }
}

/*
****************************************************************************************************
函数名称：Drager_GetGasData
入口参数：channel:DRAGER_O2/DRAGER_CO2/DRAGER_N2O
出口参数：错误代码
功能描述：获取常规气体数据
****************************************************************************************************
*/
u16 Drager_GetGasData(u16 channel)
{
    u16 temp = 0;
    if(DRAGER_FLAG.OperateMode == Standby_Mode)
    {
        temp = DRAGER_ERR_STANDBY_MODE;
        return temp;
    }else
    {
        if(channel == DRAGER_O2)
        {
            if((Drager_ParaInfo.PAI>>4)&0x01)
            {
                if((Drager_ParaInfo.PII>>4)&0x01)
                {
                    temp = DRAGER_ERR_GAS_NOOPERATE;
                    return temp;
                }else
                {
                    if(Drager_Message.HighPriority == 0x23) //堵塞
                    {
                        temp = DRAGER_ERR_OCCLUSION;
                        return temp;
                    }else
                    {
                        if(Drager_O2_Data.ZeroState)
                        {
                            temp = DRAGER_ERR_ZEROINPROGRESS;
                            return temp;
                        }else
                        {
                            if(Drager_O2_Data.Accuracy >= 2)
                            {
                                return 0; //数据正常
                            }else
                            {
                                if(Drager_ParaInfo.MISCSTAT&0x01)
                                {
                                    temp = DRAGER_ERR_WARMUP;
                                }else
                                {
                                    temp = DRAGER_ERR_NOACCURACY;
                                }
                                return temp;
                            }
                        }
                    }
                }
            }else
            {
                temp = DRAGER_ERR_GAS_NOAVAILABLE;
                return temp;
            }
        }else if(channel == DRAGER_CO2)
        {
            if(Drager_ParaInfo.PAI&0x01)
            {
                if(Drager_ParaInfo.PII&0x01)
                {
                    temp = DRAGER_ERR_GAS_NOOPERATE;
                    return temp;
                }else
                {
                    if(Drager_Message.HighPriority == 0x23) //堵塞
                    {
                        temp = DRAGER_ERR_OCCLUSION;
                        return temp;
                    }else
                    {
                        if(Drager_CO2_Data.ZeroState)
                        {
                            temp = DRAGER_ERR_ZEROINPROGRESS;
                            return temp;
                        }else
                        {
                            if(Drager_CO2_Data.Accuracy >= 2)
                            {
                                return 0; //数据正常
                            }else
                            {
                                if(Drager_ParaInfo.MISCSTAT&0x01)
                                {
                                    temp = DRAGER_ERR_WARMUP;
                                }else
                                {
                                    temp = DRAGER_ERR_NOACCURACY;
                                }
                                return temp;
                            }
                        }
                    }
                }
            }else
            {
                temp = DRAGER_ERR_GAS_NOAVAILABLE;
                return temp;
            }
        }else if(channel == DRAGER_N2O)
        {
            if((Drager_ParaInfo.PAI>>1)&0x01)
            {
                if((Drager_ParaInfo.PII>>1)&0x01)
                {
                    temp = DRAGER_ERR_GAS_NOOPERATE;
                    return temp;
                }else
                {
                    if(Drager_Message.HighPriority == 0x23) //堵塞
                    {
                        temp = DRAGER_ERR_OCCLUSION;
                        return temp;
                    }else
                    {
                        if(Drager_N2O_Data.ZeroState)
                        {
                            temp = DRAGER_ERR_ZEROINPROGRESS;
                            return temp;
                        }else
                        {
                            if(Drager_N2O_Data.Accuracy >= 2)
                            {
                                return 0; //数据正常
                            }else
                            {
                                if(Drager_ParaInfo.MISCSTAT&0x01)
                                {
                                    temp = DRAGER_ERR_WARMUP;
                                }else
                                {
                                    temp = DRAGER_ERR_NOACCURACY;
                                }
                                return temp;
                            }
                        }
                    }
                }
            }else
            {
                temp = DRAGER_ERR_GAS_NOAVAILABLE;
                return temp;
            }
        }else
        {
            return DRAGER_ERR_INPUTPARAWRONG;
        }
    }
}

/*
****************************************************************************************************
函数名称：Drager_GetModuleState
入口参数：无
出口参数：错误代码
功能描述：获取麻醉气体模块状态
****************************************************************************************************
*/
u16 Drager_GetModuleState(u8* tx_buffer)
{
    u16 temp = 0;
    if(DRAGER_FLAG.OperateMode == Measure)
    {
        if((Drager_CO2_Data.ZeroState)||(Drager_N2O_Data.ZeroState)||(Drager_O2_Data.ZeroState)
           ||(Drager_AA1_Data.ZeroState)||(Drager_AA2_Data.ZeroState))
        {
            
        }else
        {
            if(Drager_ModuleFeature.ZeroControl)
            {
                if(DRAGER_FLAG.ZeroRequest)
                {
                    if(Drager_User_Zero)
                    {
                        Drager_InitZero(tx_buffer);
                        Drager_User_Zero = 0;
                    }
                }else
                {
                    Drager_InitZero(tx_buffer);
                }
            }
        }
    }
    if(DRAGER_FLAG.CheckWaterOrLine)
    {
        if(Drager_PressureAndPump.WatertrapDisconnected)
        {
            temp |= DRAGER_ERR_WATERTRAPDISCONNECTED;
        }else if(Drager_PressureAndPump.WatertrapWarning)
        {
            temp |= DRAGER_ERR_WATERTRAPWILLFULL;
        }
        else if(Drager_PressureAndPump.WatertrapFull)
        {
            temp |= DRAGER_ERR_WATERTRAPFULL;
        }else
        {
            temp |= DRAGER_ERR_WATERTRAPWRONG;
        }
    }else
    {
        temp &= ~0x00F0;
    }
    if(DRAGER_FLAG.ComponentFail)
    {
        temp |= DRAGER_ERR_COMPONENTFAIL;
    }else
    {
        temp &= ~DRAGER_ERR_COMPONENTFAIL;
    }
    //  if(DRAGER_FLAG.BreathAvail)
    //  {
    //    ;
    //  }else
    //  {
    //    ;
    //  }
    if(DRAGER_FLAG.NorespOrApnea)
    {
        if(Drager_BreathDetectionMode == 1)
        {
            temp |= DRAGER_ERR_NORESPDECT; 
        }else
        {
            temp |= DRAGER_ERR_APNEA;
        }
    }else
    {
        if(Drager_BreathDetectionMode == 1)
        {
            temp &= ~DRAGER_ERR_NORESPDECT;
        }else
        {
            temp &= ~DRAGER_ERR_APNEA;
        }
    }
    return temp;
}