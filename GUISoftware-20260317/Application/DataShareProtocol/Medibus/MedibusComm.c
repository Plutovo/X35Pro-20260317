#include "MedibusComm_cfg.h"
#include "MedibusComm.h"
#include "string.h"
#include "DataShare.h"
#include "uart_dp.h"


 
/*
*@{ In IAR, The address of variables in SDRAM is 0xA0700000 to 0xA07FFFFF,init by yourself
*/

#ifdef __ICCARM__
__no_init static Medibus_Typedef Medibus_Type           POOL_EXRAM;
__no_init static Medibus_ASCII Medibus_Conver           POOL_EXRAM;
__no_init static Medibus_Measured Old_MeasuredVal       POOL_EXRAM;
__no_init static Medibus_Measured MeasuredVal           POOL_EXRAM;
__no_init static char *TextMessage_Buf[MEDIBUS_TEXTNUM] POOL_EXRAM; //text message
#else
//VS use
static Medibus_Typedef Medibus_Type         = {0};
static Medibus_ASCII Medibus_Conver         = {0};
static Medibus_Measured Old_MeasuredVal     = {0};
static Medibus_Measured MeasuredVal         = {0};
static const char *TextMessage_Buf[MEDIBUS_TEXTNUM];
#endif

/*
*@} end
*/

static MedibusFSM_Var FSM_Var;
static Procedure Steps[] = 
{
    FSMStep_Idle, \
    FSMStep_Reset,\
    FSMStep_DevID,\
    FSMStep_PCID, \
    FSMStep_Start,\
    FSMStep_Stop
};

static u8 Medibus_DEVID[5]      = "8888";//设备识别号
static u8 Medibus_DEVName[35]   = "\'Siriusmed Device:X55\'";//设备名称
static u8 Medibus_DEVREV[12]    = "01.00:03.00";//设备版本:medibus版本

extern AA_ALL_DATA AA_all_data;
extern unsigned char StandbyState;
extern MONITOR_PARA MonitorPara;
extern C002_CO2_PARAMETERS CO2_Parameters;
extern BREATH_SET_PARA BeathSettingPara;
extern VENT_TIME VentTime;
extern int ETCO2Unit;
extern int MenuType;
extern MENU_HWIN Menu_hWin;

/*
*{ Medibus Finite State Machine
*/

FSM_STATE FSMStep_Idle(void * arg)
{
    MedibusFSM_States state = FSM_IDLE;
    MedibusFSM_Var *p = (MedibusFSM_Var *)arg;
    
    if(p == NULL)
    {
        return state;
    }
    
    if(!(p->FSM_DevUserCommStop))
    {
        Medibus_VarInit();//重新初始化       
        Chip_EXUART_IntDisable(LPC_EXUART1, EXUART_IER_THREINT);
        p->FSM_Tmr1Flag |= 0x01;
        p->FSM_Tmr1TimOut = MEDIBUS_TMRCOUNT3;//3s
        p->FSM_Tmr2Count = 0;
        p->FSM_Tmr2Flag = 0;
        p->FSM_Tmr2TimOut = MEDIBUS_TMRCOUNT2;
        p->FSM_CommActiveFlag = 0;
        p->FSM_RecvFlag = 0;
        Medibus_SendCMD(MEDIBUS_ESC,MEDIBUS_ICC);
        state = FSM_RESET;

        //当前是否是Medibus窗口
        if((MenuType == MEDIBUS_COMM_MENU)&&(WM_IsWindow(Menu_hWin.RecordWin)))
        {
            WM_SendMessageNoPara(Menu_hWin.RecordWin,WM_USER);
        }        
    }
    else
    {
       state = FSM_IDLE; 
    }
    
    return state;
}

FSM_STATE FSMStep_Reset(void * arg)
{
    MedibusFSM_States state = FSM_RESET;
    Medibus_ASCII MeauConver = {0};
    FlagStatus status = error;
    MedibusFSM_Var *p = (MedibusFSM_Var *)arg;
    
    if(p == NULL)
    {
        return state;
    }
    
    if(p->FSM_RecvFlag)//更改定时器计时
    {
        p->FSM_RecvFlag = 0;
        if(p->FSM_Tmr1TimOut != MEDIBUS_TMRCOUNT10)//是否更改过
        {
            //p->FSM_Tmr1TimOut = MEDIBUS_TMRCOUNT10;//超时时间设置为10S
            //p->FSM_Tmr1Count = 0;//重新计时
            p->FSM_Tmr1Flag |= 1<<0;//开始计时
        }
    }
    
    if((p->FSM_Tmr1Flag)&0x02)//3s/10s 计时完成 未收到命令 再次发送 并将定时器时间设置为3s
    {
        p->FSM_Tmr1Flag &= ~(1<<1);
        p->FSM_Tmr1TimOut = MEDIBUS_TMRCOUNT3;
        p->FSM_Tmr1Count = 0;
        p->FSM_Tmr1Flag |= 1<<0;
        p->FSM_CommActiveFlag = 0;//通信复位
        Medibus_SendCMD(MEDIBUS_ESC,MEDIBUS_ICC);
        state = FSM_RESET;
    }   
    else if(Medibus_read_DP_Recv() == dp_Phb)
    {
        //p->FSM_Tmr1Flag &= ~(1<<0);//停止计时
        if(Medibus_Type.Medibus_RecvBuf[2] == MEDIBUS_ICC)
        {
            MeauConver = HEX2ASCII(Medibus_Type.Medibus_RecvBuf[1] + Medibus_Type.Medibus_RecvBuf[2]);
            if(MeauConver.ASCII_HIGH == Medibus_Type.Medibus_RecvBuf[3] \
                && MeauConver.ASCII_LOW == Medibus_Type.Medibus_RecvBuf[4])//CHK true
            {
                state = FSM_DEVID;                  
            }
            else
            {
                state = FSM_RESET;//CHK err
                Medibus_SendCMD(MEDIBUS_SOH,MEDIBUS_NAK);//校验错误
            } 
            p->FSM_Tmr1Flag |= 1<<0;//继续计时
            p->FSM_Tmr1TimOut = MEDIBUS_TMRCOUNT3;//3s
            p->FSM_Tmr1Count = 0;//重新计时             
        }
        else
        {
            p->FSM_Tmr1Flag |= 1<<0;//继续计时 计数不清零
            state = FSM_RESET;//当前不是ICC命令
            //Medibus_SendCMD(MEDIBUS_SOH,(Medibus_CMD)Medibus_Type.Medibus_RecvBuf[2]);//响应未知命令
        }
        memset(Medibus_Type.Medibus_RecvBuf,0,MEDIBUS_FIFORXNUM);
    }
    else
    {
        state = FSM_RESET;
    }
    
    return state;
}

FSM_STATE FSMStep_DevID(void * arg)
{
    MedibusFSM_States state = FSM_DEVID;
    Medibus_ASCII MeauConver = {0};
    FlagStatus status = error;
    MedibusFSM_Var *p = (MedibusFSM_Var *)arg;
    
    if(p == NULL)
    {
        return state;
    }
    
    if(p->FSM_RecvFlag)//更改定时器计时为10S
    {
        p->FSM_RecvFlag = 0;
        if(p->FSM_Tmr1TimOut != MEDIBUS_TMRCOUNT10)//是否更改过
        {
            p->FSM_Tmr1TimOut = MEDIBUS_TMRCOUNT10;//超时时间设置为10S
            p->FSM_Tmr1Count = 0;//重新计时
            p->FSM_Tmr1Flag |= 1<<0;//开始计时
        }
    }    
    
    if((p->FSM_Tmr1Flag)&0x02)//3s计时完成 未收到命令 通讯复位 定时器变为3S
    {
        p->FSM_Tmr1Flag &= ~(1<<1);
        p->FSM_Tmr1TimOut = MEDIBUS_TMRCOUNT3;
        p->FSM_Tmr1Count = 0;
        p->FSM_Tmr1Flag |= 1<<0;         
        Medibus_SendCMD(MEDIBUS_ESC,MEDIBUS_ICC);//收不到PC命令 复位通信
        state = FSM_RESET;
    }
    else if(Medibus_read_DP_Recv() == dp_Phb) //收到一帧
    {
        if(Medibus_Type.Medibus_RecvBuf[2] == MEIDBUS_DEVPCID)//PC索要设备ID命令
        {
            MeauConver = HEX2ASCII(Medibus_Type.Medibus_RecvBuf[1] + Medibus_Type.Medibus_RecvBuf[2]);
            if(MeauConver.ASCII_HIGH == Medibus_Type.Medibus_RecvBuf[3] \
                && MeauConver.ASCII_LOW == Medibus_Type.Medibus_RecvBuf[4])//CHK true
            {
                Medibus_SendDeviceID();//发送设备ID
                state = FSM_PCID;
                Medibus_SendCMD(MEDIBUS_ESC,MEIDBUS_DEVPCID);//索要PCID
            }
            else
            {
                state = FSM_DEVID;
                Medibus_SendCMD(MEDIBUS_SOH,MEDIBUS_NAK);
            }            
        }
        else //当前不是PC索要Device ID命令
        {
            state = FSM_DEVID;            
            Medibus_SendCMD(MEDIBUS_SOH,(Medibus_CMD)Medibus_Type.Medibus_RecvBuf[2]);//响应未知命令
        }
        p->FSM_Tmr1Flag |= 1<<0;//开始计时
        p->FSM_Tmr1TimOut = MEDIBUS_TMRCOUNT3;//3s
        p->FSM_Tmr1Count = 0;//重新计时
        memset(Medibus_Type.Medibus_RecvBuf,0,MEDIBUS_FIFORXNUM);
    }
    else
    {
        state = FSM_DEVID;
    }
    
    return state;
}

FSM_STATE FSMStep_PCID(void * arg)
{
    u8 i = 0, tmp_CHK = 0;
    MedibusFSM_States state = FSM_PCID;
    Medibus_ASCII MeauConver = {0};  
    FlagStatus status = error;
    MedibusFSM_Var *p = (MedibusFSM_Var *)arg;
    
    if(p == NULL)
    {
        return state;
    }
    
    if(p->FSM_RecvFlag)//更改定时器计时为10S
    {
        p->FSM_RecvFlag = 0;
        if(p->FSM_Tmr1TimOut != MEDIBUS_TMRCOUNT10)//是否更改过
        {
            p->FSM_Tmr1TimOut = MEDIBUS_TMRCOUNT10;//超时时间设置为10S
            p->FSM_Tmr1Count = 0;//重新计时
            p->FSM_Tmr1Flag |= 1<<0;//开始计时
        }
    }
    
    if((p->FSM_Tmr1Flag)&0x02)//计时完成 未收到命令 通讯复位
    {
        p->FSM_Tmr1Flag &= ~(1<<1);
        p->FSM_Tmr1Count = 0;
        p->FSM_Tmr1Flag |= 1<<0;
        if(p->FSM_Tmr1TimOut == MEDIBUS_TMRCOUNT10)//uart有数据
        {
            state = FSM_PCID;            
            Medibus_SendCMD(MEDIBUS_ESC,MEIDBUS_DEVPCID); //再次发送 
        }
        else //uart无数据 FSM直接复位
        {
            state = FSM_RESET;
            Medibus_SendCMD(MEDIBUS_ESC,MEDIBUS_ICC);
        }
        p->FSM_Tmr1TimOut = MEDIBUS_TMRCOUNT3;//更改超时时间为3s
    }
    else if(Medibus_read_DP_Recv() == dp_Phb)
    {
        for(i=1;i<(Medibus_Type.Medibus_RecvBuf[0]-2);i++)
        {
            tmp_CHK += Medibus_Type.Medibus_RecvBuf[i];
        }
        MeauConver = HEX2ASCII(tmp_CHK);
        if(Medibus_Type.Medibus_RecvBuf[2] == MEIDBUS_DEVPCID)
        {
            if(MeauConver.ASCII_HIGH == Medibus_Type.Medibus_RecvBuf[Medibus_Type.Medibus_RecvBuf[0] - 2] \
                && MeauConver.ASCII_LOW == Medibus_Type.Medibus_RecvBuf[Medibus_Type.Medibus_RecvBuf[0] - 1]) //CHK true
            {
                state = FSM_START;//通信有效
                p->FSM_CommActiveFlag = 1;
                //当前是否是Medibus窗口
                if((MenuType == MEDIBUS_COMM_MENU)&&(WM_IsWindow(Menu_hWin.RecordWin)))
                {
                    WM_SendMessageNoPara(Menu_hWin.RecordWin,WM_USER);
                }
            }
            else
            {
                state = FSM_PCID;//校验和错误 请求再次发送
                Medibus_SendCMD(MEDIBUS_SOH,MEDIBUS_NAK);                
            }           
        }
        else
        {
            state = FSM_PCID;
            Medibus_SendCMD(MEDIBUS_SOH,(Medibus_CMD)Medibus_Type.Medibus_RecvBuf[2]);//响应未知命令
        }
        p->FSM_Tmr1Flag |= 1<<0;//开始计时
        p->FSM_Tmr1TimOut = MEDIBUS_TMRCOUNT3;//3s
        p->FSM_Tmr1Count = 0;//重新计时    
        memset(Medibus_Type.Medibus_RecvBuf,0,MEDIBUS_FIFORXNUM);
    }
    else
    {
        state = FSM_PCID;
    }    
    
    return state;
}

FSM_STATE FSMStep_Start(void * arg)
{
    u8 i = 0, tmp_CHK = 0;
    MedibusFSM_States state = FSM_START;
    Medibus_ASCII MeauConver = {0};   
    FlagStatus status = error;
    MedibusFSM_Var *p = (MedibusFSM_Var *)arg;
    
    if(p == NULL)
    {
        return state;
    }
    
    if(!(p->FSM_Tmr2Flag & 0x01)) //开始标志未置位
    {
        p->FSM_Tmr2Flag |= 1<<0;
        p->FSM_Tmr2TimOut = MEDIBUS_TMRCOUNT2;//2S发送 维持medibus通信
    }
    else if((p->FSM_Tmr2Flag)&0x02) //维持通信
    {
        p->FSM_Tmr2Flag &= ~(1<<1);
        Medibus_SendCMD(MEDIBUS_ESC,MEDIBUS_NOP);//3S发送NOP 维持通信
    }
    
    if((p->FSM_Tmr1Flag)&0x02)//超时,复位 3S内收不到PC回复的NOP命令，则复位通信
    {
        p->FSM_Tmr1Flag &= ~(1<<1);  
        p->FSM_Tmr1TimOut = MEDIBUS_TMRCOUNT3;
        p->FSM_Tmr1Count = 0;
        p->FSM_Tmr1Flag |= 0x01; 
        p->FSM_CommActiveFlag = 0;
        //当前是否是Medibus窗口
        if((MenuType == MEDIBUS_COMM_MENU)&&(WM_IsWindow(Menu_hWin.RecordWin)))
        {
            WM_SendMessageNoPara(Menu_hWin.RecordWin,WM_USER);
        }
        state = FSM_RESET;
    }    
    else if(Medibus_read_DP_Recv() == dp_Phb)
    {
        for(i=1;i<(Medibus_Type.Medibus_RecvBuf[0]-2);i++)
        {
            tmp_CHK += Medibus_Type.Medibus_RecvBuf[i];
        }
        MeauConver = HEX2ASCII(tmp_CHK);

        if(Medibus_Type.Medibus_RecvBuf[Medibus_Type.Medibus_RecvBuf[0] - 2] == MeauConver.ASCII_HIGH\
            && Medibus_Type.Medibus_RecvBuf[Medibus_Type.Medibus_RecvBuf[0] - 1] == MeauConver.ASCII_LOW) //CHK true
        {
            switch(Medibus_Type.Medibus_RecvBuf[2])
            {
            case MEIDBUS_MEASURED1: //监测数据
                state = FSM_START;                    
                Medibus_SendMeauData();
                break;
            case MEIDBUS_MEASURED2: //暂时不使用
                state = FSM_START;                    
                Medibus_SendCMD(MEDIBUS_SOH,(Medibus_CMD)Medibus_Type.Medibus_RecvBuf[2]);//不能识别的命令
                break;
            case MEIDBUS_TEXTMESS:  //文本信息
                state = FSM_START;                    
                Medibus_SendTextMess();
                break; 
            case MEDIBUS_STOP:  //PC stop命令收到 通讯复位
                state = FSM_IDLE;
                break;
            case MEDIBUS_NOP:   //Idle NOP CMD,Not response
                state = FSM_START;
                break;
            default:
                state = FSM_START;                    
                Medibus_SendCMD(MEDIBUS_SOH,(Medibus_CMD)Medibus_Type.Medibus_RecvBuf[2]);//不能识别的命令
                break;
            }
        }
        else
        {
            state = FSM_START;
            Medibus_SendCMD(MEDIBUS_SOH,MEDIBUS_NAK);//CHK err            
        }
    
        p->FSM_Tmr1Flag |= 1<<0;//开始计时
        p->FSM_Tmr1TimOut = MEDIBUS_TMRCOUNT3;//3s
        p->FSM_Tmr1Count = 0;//重新计时        
        memset(Medibus_Type.Medibus_RecvBuf,0,MEDIBUS_FIFORXNUM);
    }
    else if(p->FSM_DevUserCommStop)//device主动按下stop命令
    {
        //通讯不可恢复的命令
        state = FSM_STOP;
        Medibus_SendCMD(MEDIBUS_ESC,MEDIBUS_STOP);//dev发送stop命令
        p->FSM_PCResStopFlag = 1;
        p->FSM_Tmr1Flag |= 1<<0;//开始计时
        p->FSM_Tmr1TimOut = MEDIBUS_TMRCOUNT3;//3s
        p->FSM_Tmr1Count = 0;//重新计时   
        p->FSM_Tmr2TimOut = MEDIBUS_TMRCOUNT20;
        p->FSM_Tmr2Count = 0;
        p->FSM_Tmr2Flag = 1;
    }
    else
    {
        state = FSM_START;
    }
    
    return state;    
}

FSM_STATE FSMStep_Stop(void * arg)
{
    MedibusFSM_States state = FSM_STOP;
    Medibus_ASCII MeauConver = {0};   
    FlagStatus status = error;
    MedibusFSM_Var *p = (MedibusFSM_Var *)arg;
    
    if(p == NULL)
    {
        return state;
    }     
    
    if((p->FSM_Tmr2Flag)&0x02) //等待PC回复stop 20s无响应直接停止
    {
        p->FSM_Tmr1Flag = 0;//开始计时
        p->FSM_Tmr1TimOut = MEDIBUS_TMRCOUNT3;//3s
        p->FSM_Tmr1Count = 0;//重新计时   
        p->FSM_Tmr2TimOut = MEDIBUS_TMRCOUNT3;
        p->FSM_Tmr2Count = 0;
        p->FSM_Tmr2Flag = 0;
        state = FSM_IDLE;
        //设备停止通信        
        Chip_EXUART_IntDisable(LPC_EXUART1, EXUART_IER_THREINT);
        exuart1_init(9600, EXUART_LCR_PARITY_EN|EXUART_LCR_PARITY_EVEN);
        Medibus_VarInit();
        p->FSM_DevUserCommStop = 1;
        p->FSM_PCResStopFlag = 0;
        //当前是否是Medibus窗口
        if((MenuType == MEDIBUS_COMM_MENU)&&(WM_IsWindow(Menu_hWin.RecordWin)))
        {
            WM_SendMessageNoPara(Menu_hWin.RecordWin,WM_USER);
        }        
    }
    else if((p->FSM_Tmr1Flag)&0x02)//超时,复位 3S内收不到PC回复的NOP命令，则复位通信
    {
        p->FSM_Tmr1Flag &= ~(1<<1);  
        p->FSM_Tmr1TimOut = MEDIBUS_TMRCOUNT3;
        p->FSM_Tmr1Count = 0;
        p->FSM_Tmr1Flag |= 0x01;  
        Medibus_SendCMD(MEDIBUS_ESC,MEDIBUS_STOP);//dev发送stop命令
        state = FSM_STOP;
    }
    else if(Medibus_read_DP_Recv() == dp_Phb)
    {
        if(Medibus_Type.Medibus_RecvBuf[2] == MEDIBUS_STOP)
        {
            MeauConver = HEX2ASCII(Medibus_Type.Medibus_RecvBuf[1] + Medibus_Type.Medibus_RecvBuf[2]);
            if(MeauConver.ASCII_HIGH == Medibus_Type.Medibus_RecvBuf[3] \
                && MeauConver.ASCII_LOW == Medibus_Type.Medibus_RecvBuf[4])//CHK true
            {
                state = FSM_IDLE; 
                //设备停止通信             
                Chip_EXUART_IntDisable(LPC_EXUART1, EXUART_IER_THREINT);
                exuart1_init(9600, EXUART_LCR_PARITY_EN|EXUART_LCR_PARITY_EVEN);
                Medibus_VarInit();
                p->FSM_DevUserCommStop = 1;
                p->FSM_PCResStopFlag = 0;
                //当前是否是Medibus窗口
                if((MenuType == MEDIBUS_COMM_MENU)&&(WM_IsWindow(Menu_hWin.RecordWin)))
                {
                    WM_SendMessageNoPara(Menu_hWin.RecordWin,WM_USER);
                }
            }
            else //CHK ERR
            {
                state = FSM_STOP;
                Medibus_SendCMD(MEDIBUS_SOH,MEDIBUS_NAK);
            }          
        }
        else
        {
            state = FSM_STOP; 
            Medibus_SendCMD(MEDIBUS_SOH,(Medibus_CMD)Medibus_Type.Medibus_RecvBuf[2]);//响应未知命令
        }
        memset(Medibus_Type.Medibus_RecvBuf,0,MEDIBUS_FIFORXNUM);      
    }
    else
    {
        state = FSM_STOP;
    }
    
    return state; 
}

void Medibus_FSM(void * invar)
{
    static MedibusFSM_States state = FSM_IDLE;
	state = (MedibusFSM_States)Steps[state](invar);
}

void Medibus_FSM_Start(void)
{
    Medibus_GetMeasuredData();
    Medibus_FSM(&FSM_Var);
}

/*
*@}
*/

void Medibus_frmrecv(void)
{
    static u8 buf[MEDIBUS_FIFORXNUM] = {0};
    static u32 i = 1;
    
    //接收FIFO非空 && 允许读(上次读出的数据已经处理)
    while(uart_dp_isrecvempty() == false && FSM_Var.FSM_DP_Recv == dp_allow)
    {
        FSM_Var.FSM_RecvFlag = 1;
        switch(FSM_Var.FSM_Recv_State)
        {
        case Medibus_Str:
            if(uart_dp_itemscnt() >= 1)
            {
                uart_dp_rdrecv(buf, 1);
                if(buf[0] == MEDIBUS_ESC || buf[0] == MEDIBUS_SOH)
                {
                    FSM_Var.FSM_Recv_State = Medibus_End;
                }
                else
                {
                    buf[0] = 0;
                    FSM_Var.FSM_Recv_State = Medibus_Str;
                }
            }
            else
            {
                FSM_Var.FSM_Err_Recv = err_wait;
                FSM_Var.FSM_Recv_State = Medibus_Str;
            }
            break;
        case Medibus_End:
            while(uart_dp_itemscnt() >= 1)
            {
                uart_dp_rdrecv(&buf[i], 1);
                if(buf[i] == MEDIBUS_CR)
                {
                    FSM_Var.FSM_Err_Recv = err_suc;
                    FSM_Var.FSM_Recv_State = Medibus_Str;
                    memcpy(&Medibus_Type.Medibus_RecvBuf[1], buf, i+1);
                    Medibus_Type.Medibus_RecvBuf[0] = i+1;
                    FSM_Var.FSM_DP_Recv = dp_Phb;
                    i = 1;
                    return;
                }
                else
                {
                    i++;
                }
                
                //数据接收超出
                if(i>MEDIBUS_FIFORXNUM)
                {
                    FSM_Var.FSM_Err_Recv = err_none;
                    FSM_Var.FSM_Recv_State = Medibus_Str;                    
                    Medibus_Type.Medibus_RecvBuf[0] = 0;
                    i = 1;
                }
            }
            
            //接收不完全
            FSM_Var.FSM_Err_Recv = err_wait;
            FSM_Var.FSM_Recv_State = Medibus_End;            
            break;
        default:
            break;
        }
        
        //接收不完全 停止循环 等待串口接收新数据
        if(FSM_Var.FSM_Err_Recv == err_wait)
        {
            break;
        }        
    }
}



/*
*@}
*/


Medibus_dp_t Medibus_read_DP_Recv(void)
{
    Medibus_dp_t tmp = FSM_Var.FSM_DP_Recv;
    if(tmp == dp_Phb)   //一帧接收完成 待读取
    {
        FSM_Var.FSM_DP_Recv = dp_allow;
    }
    return tmp;
}

/*
*@{ Medibus timer set/get
*/

void Medibus_timer(void)
{
    Medibus_Timer1();
    Medibus_Timer2();   
}

void Medibus_Timer1(void)
{
    if(FSM_Var.FSM_Tmr1Flag&0x01)
    {
        if(FSM_Var.FSM_Tmr1Count >= FSM_Var.FSM_Tmr1TimOut)
        {
            FSM_Var.FSM_Tmr1Count = 0;
            FSM_Var.FSM_Tmr1Flag |= 1<<1;
        }
        else
        {
            FSM_Var.FSM_Tmr1Count++;
        }
    }    
}


//timer2
void Medibus_Timer2(void)
{
    if(FSM_Var.FSM_Tmr2Flag&0x01)
    {
        if(FSM_Var.FSM_Tmr2Count >= FSM_Var.FSM_Tmr2TimOut)
        {
            FSM_Var.FSM_Tmr2Count = 0;
            FSM_Var.FSM_Tmr2Flag |= 1<<1;
        }
        else
        {
            FSM_Var.FSM_Tmr2Count++;
        }
    }    
}

void Medibus_SetUserStopFlag(u8 data)
{
    FSM_Var.FSM_DevUserCommStop = data;
}

u8 Medibus_GetCommActiveFlag(void)
{
    return FSM_Var.FSM_CommActiveFlag;
}

u8 Medibus_GetUserStopFlag(void)
{
    return FSM_Var.FSM_DevUserCommStop;
}

u8 Medibus_GetPCResStopFlag(void)
{
    return FSM_Var.FSM_PCResStopFlag;
}

/*
*@ }
*/

/*
*@{ Medibus data
*/
void Medibus_Init(void)
{
    u8 tmp = 0;
    
    exuart1_init(9600, EXUART_LCR_PARITY_EN|EXUART_LCR_PARITY_EVEN);
    uart_dp_clrrb();
        
    Medibus_VarInit();
    
    TextMessage_Buf[0]  = "VCV Mode";
    TextMessage_Buf[1]  = "PCV Mode";
    TextMessage_Buf[2]  = "PSV Mode";
    TextMessage_Buf[3]  = "PRVC Mode";
    TextMessage_Buf[4]  = "SIMV-VCV Mode";
    TextMessage_Buf[5]  = "SIMV-PCV Mode";
    TextMessage_Buf[6]  = "SIMV-PRVC Mode";
    TextMessage_Buf[7]  = "ACGO Mode";
    TextMessage_Buf[8]  = "MANUAL Mode";
    TextMessage_Buf[9]  = "VCV+SIGH Mode";
    TextMessage_Buf[10] = "mmHg";
    TextMessage_Buf[11] = "kPa";
    TextMessage_Buf[12] = "%";
    TextMessage_Buf[MEDIBUS_TEXTNUM-1] = "";
    
    //system first init
//    if(((RTEEPROMread(SYSTEM_INIT_HIGH_ADDR)<<8) | RTEEPROMread(SYSTEM_INIT_LOW_ADDR)) != SYSTEM_INIT_FLAG)
//    {
//        RTEEPROMwrite(SYSTEM_INIT_HIGH_ADDR,(SYSTEM_INIT_FLAG>>8)&0xFF);
//        RTEEPROMwrite(SYSTEM_INIT_LOW_ADDR,  SYSTEM_INIT_FLAG    &0xFF);
//        RTEEPROMwrite(Medibus_COMM_ADDR,0x01);
//    } 
    
    tmp = RTEEPROMread(Medibus_COMM_ADDR);
    if(tmp > 1)
    {
        tmp = 1;
        RTEEPROMwrite(Medibus_COMM_ADDR,tmp);
    }
    FSM_Var.FSM_DevUserCommStop = tmp;
}

void Medibus_VarInit(void)
{
    memset(&Medibus_Type,           0,sizeof(Medibus_Type));
    memset(&Old_MeasuredVal,        0,sizeof(Old_MeasuredVal));
    memset(&MeasuredVal,            0,sizeof(MeasuredVal));
    memset(&Medibus_Conver,         0,sizeof(Medibus_Conver));

    Old_MeasuredVal.CO2_Unit = 0xFF;//MeasuredVal.CO2_Unit = 0,let them different
    FSM_Var.FSM_CommActiveFlag = 0;//Medibus通信未建立
    
}

void Medibus_SendCMD(Medibus_DIR DIR,Medibus_CMD CMD)
{
    Medibus_Type.Medibus_SendNum = 5;//数据长度
    Medibus_Type.Medibus_SendBuf[0] = (u8)Medibus_Type.Medibus_SendNum;
    Medibus_Type.Medibus_SendBuf[1] = (u8)DIR;//发送or响应
    Medibus_Type.Medibus_SendBuf[2] = (u8)CMD;//具体命令
    Medibus_CheckSumCal(&Medibus_Type.Medibus_SendBuf[1],Medibus_Type.Medibus_SendNum);//校验和计算
    Medibus_Type.Medibus_SendBuf[Medibus_Type.Medibus_SendNum] = MEDIBUS_CR;//结束符
    
    uart_dp_wrsend(&Medibus_Type.Medibus_SendBuf[1], Medibus_Type.Medibus_SendNum);
}

void Medibus_SendDeviceID(void)
{
    Medibus_Type.Medibus_SendBuf[1] = (u8)MEDIBUS_SOH;
    Medibus_Type.Medibus_SendBuf[2] = (u8)MEIDBUS_DEVPCID;
    sprintf((char*)(Medibus_Type.Medibus_SendBuf+3),"%s%s%s",Medibus_DEVID,Medibus_DEVName,Medibus_DEVREV);
    Medibus_Type.Medibus_SendNum = strlen((char*)&Medibus_Type.Medibus_SendBuf[3]) + 5;
    Medibus_Type.Medibus_SendBuf[0] = Medibus_Type.Medibus_SendNum;
    Medibus_CheckSumCal(&Medibus_Type.Medibus_SendBuf[1],Medibus_Type.Medibus_SendNum);
    Medibus_Type.Medibus_SendBuf[Medibus_Type.Medibus_SendNum] = MEDIBUS_CR;
    
    uart_dp_wrsend(&Medibus_Type.Medibus_SendBuf[1], Medibus_Type.Medibus_SendNum);
}

void Medibus_SendMeauData(void)
{
    u8 SendBuf_Offset = 0;
    Medibus_ASCII MeauConver = {0};
    
    Medibus_Type.Medibus_SendBuf[0] = 0;
    Medibus_Type.Medibus_SendBuf[1] = (u8)MEDIBUS_SOH;
    Medibus_Type.Medibus_SendBuf[2] = (u8)MEIDBUS_MEASURED1;
    
    SendBuf_Offset += 3;//3
    
    if(!StandbyState)
    {
    #ifdef __PPLAT    
        if(Old_MeasuredVal.PplatVal != MeasuredVal.PplatVal)
        {
    #ifndef __Medibus_Debug        
            Old_MeasuredVal.PplatVal = MeasuredVal.PplatVal;
    #endif
            MeauConver = HEX2ASCII(MEDIBUS_PPLAT);
            Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 0] = MeauConver.ASCII_HIGH;
            Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 1] = MeauConver.ASCII_LOW;
           
            Medibus_MeasDataFMT(MEDIBUS_PPLAT,&Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 2]);
            SendBuf_Offset += 6;
        }
    #endif
        
    #ifdef __PEEP
        if(Old_MeasuredVal.PEEPVal != MeasuredVal.PEEPVal)
        {
    #ifndef __Medibus_Debug        
            Old_MeasuredVal.PEEPVal = MeasuredVal.PEEPVal;
    #endif
            MeauConver = HEX2ASCII(MEDIBUS_PEEP);
            Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 0] = MeauConver.ASCII_HIGH;
            Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 1] = MeauConver.ASCII_LOW;
            
            Medibus_MeasDataFMT(MEDIBUS_PEEP,&Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 2]);
            SendBuf_Offset += 6;
        }
    #endif
        
    #ifdef __PPEAK
        if(Old_MeasuredVal.PpeakVal != MeasuredVal.PpeakVal)
        {
    #ifndef __Medibus_Debug        
            Old_MeasuredVal.PpeakVal = MeasuredVal.PpeakVal;
    #endif
            MeauConver = HEX2ASCII(MEDIBUS_PEAK);
            Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 0] = MeauConver.ASCII_HIGH;
            Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 1] = MeauConver.ASCII_LOW;
            
            Medibus_MeasDataFMT(MEDIBUS_PEAK,&Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 2]);
            SendBuf_Offset += 6;
        }
    #endif
        
    #ifdef __VTE
        if(Old_MeasuredVal.VTeVal != MeasuredVal.VTeVal)
        {
    #ifndef __Medibus_Debug        
            Old_MeasuredVal.VTeVal = MeasuredVal.VTeVal;
    #endif
            MeauConver = HEX2ASCII(MEDIBUS_VTE);
            Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 0] = MeauConver.ASCII_HIGH;
            Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 1] = MeauConver.ASCII_LOW;
            
            Medibus_MeasDataFMT(MEDIBUS_VTE,&Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 2]);
            SendBuf_Offset += 6;        
        }
    #endif
        
    #ifdef __VTI
        if(Old_MeasuredVal.VTiVal != MeasuredVal.VTiVal)
        {
    #ifndef __Medibus_Debug        
            Old_MeasuredVal.VTiVal = MeasuredVal.VTiVal;
    #endif
            MeauConver = HEX2ASCII(MEDIBUS_VTI);
            Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 0] = MeauConver.ASCII_HIGH;
            Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 1] = MeauConver.ASCII_LOW;
            
            Medibus_MeasDataFMT(MEDIBUS_VTI,&Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 2]);      
            SendBuf_Offset += 6;
        }
    #endif
        
    #ifdef __RATE
        if(Old_MeasuredVal.RateVal != MeasuredVal.RateVal)
        {
    #ifndef __Medibus_Debug        
            Old_MeasuredVal.RateVal = MeasuredVal.RateVal;
    #endif
            MeauConver = HEX2ASCII(MEDIBUS_RATE);
            Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 0] = MeauConver.ASCII_HIGH;
            Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 1] = MeauConver.ASCII_LOW;
            
            Medibus_MeasDataFMT(MEDIBUS_RATE,&Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 2]);         
            SendBuf_Offset += 6;
        }
    #endif
        
    #ifdef __IE
        if(Old_MeasuredVal.IEIVal != MeasuredVal.IEIVal)
        {
    #ifndef __Medibus_Debug
            Old_MeasuredVal.IEIVal = MeasuredVal.IEIVal;
    #endif
            MeauConver = HEX2ASCII(MEDIBUS_IEI);
            Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 0] = MeauConver.ASCII_HIGH;
            Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 1] = MeauConver.ASCII_LOW;
            
            Medibus_MeasDataFMT(MEDIBUS_IEI,&Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 2]);      
            SendBuf_Offset += 6;
        }
        if(Old_MeasuredVal.IEEVal != MeasuredVal.IEEVal)
        {
    #ifndef __Medibus_Debug        
            Old_MeasuredVal.IEEVal = MeasuredVal.IEEVal;
    #endif
            MeauConver = HEX2ASCII(MEDIBUS_IEE);
            Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 0] = MeauConver.ASCII_HIGH;
            Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 1] = MeauConver.ASCII_LOW;
            
            Medibus_MeasDataFMT(MEDIBUS_IEE,&Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 2]);            
            SendBuf_Offset += 6;
        }
    #endif
        
    #ifdef __MV
        if(Old_MeasuredVal.MVVal != MeasuredVal.MVVal)
        {
    #ifndef __Medibus_Debug        
            Old_MeasuredVal.MVVal = MeasuredVal.MVVal;
    #endif
            MeauConver = HEX2ASCII(MEDIBUS_MV);
            Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 0] = MeauConver.ASCII_HIGH;
            Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 1] = MeauConver.ASCII_LOW;
            
            Medibus_MeasDataFMT(MEDIBUS_MV,&Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 2]);               
            SendBuf_Offset += 6;
        }
    #endif
        
    #ifdef __FIO2
        if(Old_MeasuredVal.FiO2Val != MeasuredVal.FiO2Val)
        {
    #ifndef __Medibus_Debug        
            Old_MeasuredVal.FiO2Val = MeasuredVal.FiO2Val;
    #endif
            MeauConver = HEX2ASCII(MEDIBUS_FiO2);
            Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 0] = MeauConver.ASCII_HIGH;
            Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 1] = MeauConver.ASCII_LOW;
            
            Medibus_MeasDataFMT(MEDIBUS_FiO2,&Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 2]);       
            SendBuf_Offset += 6;
        }
    #endif
        
    #ifdef __ETCO2
        if(Old_MeasuredVal.EtCO2Val != MeasuredVal.EtCO2Val)
        {
    #ifndef __Medibus_Debug        
            Old_MeasuredVal.EtCO2Val = MeasuredVal.EtCO2Val;
    #endif
            MeauConver = HEX2ASCII(MEDIBUS_EtCO2);
            Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 0] = MeauConver.ASCII_HIGH;
            Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 1] = MeauConver.ASCII_LOW;
            
            Medibus_MeasDataFMT(MEDIBUS_EtCO2,&Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 2]); 
            SendBuf_Offset += 6;
        }
    #endif
        
    #ifdef __ETAA
        if(Old_MeasuredVal.EtAAVal != MeasuredVal.EtAAVal)
        {
    #ifndef __Medibus_Debug        
            Old_MeasuredVal.EtAAVal = MeasuredVal.EtAAVal;
    #endif
            switch(MeasuredVal.AA_Type)
            {
            case AX_Hal:
                MeauConver = HEX2ASCII(MEDIBUS_EtHAL);
                Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 0] = MeauConver.ASCII_HIGH;
                Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 1] = MeauConver.ASCII_LOW;
                Medibus_MeasDataFMT(MEDIBUS_EtHAL,&Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 2]);            
                break;
            case AX_Enf:
                MeauConver = HEX2ASCII(MEDIBUS_EtENF);
                Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 0] = MeauConver.ASCII_HIGH;
                Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 1] = MeauConver.ASCII_LOW;
                Medibus_MeasDataFMT(MEDIBUS_EtENF,&Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 2]);             
                break;
            case AX_Iso:
                MeauConver = HEX2ASCII(MEDIBUS_EtISO);
                Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 0] = MeauConver.ASCII_HIGH;
                Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 1] = MeauConver.ASCII_LOW;
                Medibus_MeasDataFMT(MEDIBUS_EtISO,&Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 2]);             
                break;
            case AX_Sev:
                MeauConver = HEX2ASCII(MEDIBUS_EtSEV);
                Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 0] = MeauConver.ASCII_HIGH;
                Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 1] = MeauConver.ASCII_LOW;
                Medibus_MeasDataFMT(MEDIBUS_EtSEV,&Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 2]);             
                break;
            case AX_Des:
            default:
                MeauConver = HEX2ASCII(MEDIBUS_EtDES);
                Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 0] = MeauConver.ASCII_HIGH;
                Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 1] = MeauConver.ASCII_LOW;
                Medibus_MeasDataFMT(MEDIBUS_EtDES,&Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 2]);             
                break;
            } 
            SendBuf_Offset += 6;        
        }
    #endif
        
    #ifdef __ETN2O
        if(Old_MeasuredVal.EtN2OVal != MeasuredVal.EtN2OVal)
        {
    #ifndef __Medibus_Debug        
            Old_MeasuredVal.EtN2OVal = MeasuredVal.EtN2OVal;
    #endif
            MeauConver = HEX2ASCII(MEDIBUS_EtN2O);
            Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 0] = MeauConver.ASCII_HIGH;
            Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 1] = MeauConver.ASCII_LOW;
            Medibus_MeasDataFMT(MEDIBUS_EtN2O,&Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 2]);  
            SendBuf_Offset += 6;  
        }
    #endif
        
    #ifdef __FLOWO2
        if(Old_MeasuredVal.FlowO2 != MeasuredVal.FlowO2)
        {
    #ifndef __Medibus_Debug        
            Old_MeasuredVal.FlowO2 = MeasuredVal.FlowO2;
    #endif
            MeauConver = HEX2ASCII(MEDIBUS_FlowO2);
            Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 0] = MeauConver.ASCII_HIGH;
            Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 1] = MeauConver.ASCII_LOW;
            Medibus_MeasDataFMT(MEDIBUS_FlowO2,&Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 2]);  
            SendBuf_Offset += 6;         
        }
    #endif
        
    #ifdef __FLOWN2O
        if(Old_MeasuredVal.FlowN2O != MeasuredVal.FlowN2O)
        {
    #ifndef __Medibus_Debug        
            Old_MeasuredVal.FlowN2O = MeasuredVal.FlowN2O;
    #endif
            MeauConver = HEX2ASCII(MEDIBUS_FlowN2O);
            Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 0] = MeauConver.ASCII_HIGH;
            Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 1] = MeauConver.ASCII_LOW;
            Medibus_MeasDataFMT(MEDIBUS_FlowN2O,&Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 2]);  
            SendBuf_Offset += 6;            
        }
    #endif
        
    #ifdef __FLOWAIR
        if(Old_MeasuredVal.FlowAir != MeasuredVal.FlowAir)
        {
    #ifndef __Medibus_Debug        
            Old_MeasuredVal.FlowAir = MeasuredVal.FlowAir;
    #endif
            MeauConver = HEX2ASCII(MEDIBUS_FlowAir);
            Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 0] = MeauConver.ASCII_HIGH;
            Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 1] = MeauConver.ASCII_LOW;
            Medibus_MeasDataFMT(MEDIBUS_FlowAir,&Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 2]);  
            SendBuf_Offset += 6;          
        }
    #endif
        
    #ifdef __PULSE
        if(Old_MeasuredVal.PulseVal != MeasuredVal.PulseVal)
        {
    #ifndef __Medibus_Debug        
            Old_MeasuredVal.PulseVal = MeasuredVal.PulseVal;
    #endif
            MeauConver = HEX2ASCII(MEDIBUS_Pulse);
            Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 0] = MeauConver.ASCII_HIGH;
            Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 1] = MeauConver.ASCII_LOW;
            Medibus_MeasDataFMT(MEDIBUS_Pulse,&Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 2]);  
            SendBuf_Offset += 6;           
        }
    #endif
        
    #ifdef __SPO2
        if(Old_MeasuredVal.SpO2Val != MeasuredVal.SpO2Val)
        {
    #ifndef __Medibus_Debug        
            Old_MeasuredVal.SpO2Val = MeasuredVal.SpO2Val;
    #endif
            MeauConver = HEX2ASCII(MEDIBUS_SpO2);
            Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 0] = MeauConver.ASCII_HIGH;
            Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 1] = MeauConver.ASCII_LOW;
            Medibus_MeasDataFMT(MEDIBUS_SpO2,&Medibus_Type.Medibus_SendBuf[SendBuf_Offset + 2]);  
            SendBuf_Offset += 6;           
        }
    #endif
    }
    
    Medibus_Type.Medibus_SendNum = SendBuf_Offset - 1 + 3;
    Medibus_Type.Medibus_SendBuf[0] = Medibus_Type.Medibus_SendNum;
    Medibus_CheckSumCal(&Medibus_Type.Medibus_SendBuf[1],Medibus_Type.Medibus_SendNum);
    Medibus_Type.Medibus_SendBuf[Medibus_Type.Medibus_SendNum] = MEDIBUS_CR;
    
    uart_dp_wrsend(&Medibus_Type.Medibus_SendBuf[1], Medibus_Type.Medibus_SendNum);    
}

void Medibus_MeasDataFMT(u8 data_type, u8 *buf)
{
    Medibus_I2A Int2Ascii = {0};
    
    switch(data_type)
    {
    case MEDIBUS_PPLAT:
        Int2Ascii = Medibus_int2ascii((u16)(abs(MeasuredVal.PplatVal)/10));
        if(MeasuredVal.PplatVal >= 0)
        {
            buf[0] = MEDIBUS_ASCIISPACE;
        }
        else
        {
            buf[0] = '-';
        }
        if(Int2Ascii.c == '0')
        {
            buf[1] = MEDIBUS_ASCIISPACE;
        }
        else
        {
            buf[1] = Int2Ascii.c;
        }
        buf[2] = Int2Ascii.d;
        buf[3] = MEDIBUS_ASCIISPACE;        
        break;
    case MEDIBUS_PEEP:
        Int2Ascii = Medibus_int2ascii((u16)(abs(MeasuredVal.PEEPVal)/10));
        if(MeasuredVal.PEEPVal >= 0)
        {
             buf[0] = MEDIBUS_ASCIISPACE;
        }
        else
        {
             buf[0] = '-';
        }
        if(Int2Ascii.c == '0')
        {
            buf[1] = MEDIBUS_ASCIISPACE;
        }
        else
        {
            buf[1] = Int2Ascii.c;
        }
        buf[2] = Int2Ascii.d;
        buf[3] = MEDIBUS_ASCIISPACE;        
        break;
    case MEDIBUS_PEAK:
        Int2Ascii = Medibus_int2ascii((u16)(abs(MeasuredVal.PpeakVal)/10));
        if(MeasuredVal.PpeakVal >= 0)
        {
            buf[0] = MEDIBUS_ASCIISPACE;
        }
        else
        {
            buf[0] = '-';
        }
        if(Int2Ascii.c == '0')
        {
            buf[1] = MEDIBUS_ASCIISPACE;
        }
        else
        {
            buf[1] = Int2Ascii.c;
        }
        buf[2] = Int2Ascii.d;
        buf[3] = MEDIBUS_ASCIISPACE;        
        break;
    case MEDIBUS_VTE:
    case MEDIBUS_VTI:        
        if(data_type == MEDIBUS_VTE)
        {
            Int2Ascii = Medibus_int2ascii(MeasuredVal.VTeVal);
        }
        else
        {
            Int2Ascii = Medibus_int2ascii(MeasuredVal.VTiVal); 
        }
        if(Int2Ascii.a == '0')
        {
            buf[0] = MEDIBUS_ASCIISPACE;
            if(Int2Ascii.b == '0')
            {
                buf[1] = MEDIBUS_ASCIISPACE;
                if(Int2Ascii.c == '0')
                {
                    buf[2] = MEDIBUS_ASCIISPACE;
                }
                else
                {
                    buf[2] = Int2Ascii.c;
                }
            }
            else
            {
                buf[1] = Int2Ascii.b;
                buf[2] = Int2Ascii.c;
            }
        }
        else
        {
            buf[0] = Int2Ascii.a;
            buf[1] = Int2Ascii.b;
            buf[2] = Int2Ascii.c;
        }
        buf[3] = Int2Ascii.d;//如果进这里,最低位不用判断是否为0             
        break;
    case MEDIBUS_RATE:
        Int2Ascii = Medibus_int2ascii(MeasuredVal.RateVal);
        if(Int2Ascii.c == '0')
        {
            buf[0] = MEDIBUS_ASCIISPACE;
        }
        else
        {
            buf[0] = Int2Ascii.c;
        }
        buf[1] = Int2Ascii.d;   
        buf[2] = MEDIBUS_ASCIISPACE;
        buf[3] = MEDIBUS_ASCIISPACE;        
        break;
    case MEDIBUS_IEI:
    case MEDIBUS_IEE:
        if(data_type == MEDIBUS_IEI)
        {
            Int2Ascii = Medibus_int2ascii(MeasuredVal.IEIVal);
        }
        else
        {
            Int2Ascii = Medibus_int2ascii(MeasuredVal.IEEVal);
        }
        if(Int2Ascii.b == '0')
        {
            buf[0] = MEDIBUS_ASCIISPACE;
        }
        else
        {
            buf[0] = Int2Ascii.b;
        }
        buf[1] = Int2Ascii.c;        
        buf[2] = '.';   
        buf[3] = Int2Ascii.d; 
        break;
    case MEDIBUS_MV:
        Int2Ascii = Medibus_int2ascii(MeasuredVal.MVVal/10);
        if(Int2Ascii.b == '0')
        {
            buf[0] = MEDIBUS_ASCIISPACE;
        }
        else
        {
            buf[0] = Int2Ascii.b;
        }        
        buf[1] = Int2Ascii.c;        
        buf[2] = '.';   
        buf[3] = Int2Ascii.d;        
        break;
    case MEDIBUS_FiO2:
    case MEDIBUS_EtN2O:
        if(data_type == MEDIBUS_FiO2)
        {
            Int2Ascii = Medibus_int2ascii(MeasuredVal.FiO2Val);
        }
        else
        {
            Int2Ascii = Medibus_int2ascii(MeasuredVal.EtN2OVal);
        }
    
        if(Int2Ascii.b == '0')
        {
            buf[0] = MEDIBUS_ASCIISPACE;
            if(Int2Ascii.c == '0')
            {
                buf[1] = MEDIBUS_ASCIISPACE;
                                
            }
            else
            {
                buf[1] = Int2Ascii.c;                
            }
            buf[2] = Int2Ascii.d; 
        }
        else
        {
            buf[0] = Int2Ascii.b;
            buf[1] = Int2Ascii.c;
            buf[2] = Int2Ascii.d;
        }
        buf[3] = MEDIBUS_ASCIISPACE;        
        break;
    case MEDIBUS_EtCO2:
    case MEDIBUS_EtCO2_kPa:
    case MEDIBUS_EtCO2_mmHg:        
        Int2Ascii = Medibus_int2ascii(MeasuredVal.EtCO2Val);
        if(ds_readpara_CO2Unit() == 0 || ds_readpara_CO2Unit() == 2)
        {
            if(Int2Ascii.b == '0')
            {
                buf[0] = MEDIBUS_ASCIISPACE;
            }
            else
            {
                buf[0] = Int2Ascii.b;
            } 
            buf[1] = Int2Ascii.c;        
            buf[2] = '.';   
            buf[3] = Int2Ascii.d;          
        }
        else
        {
            if(Int2Ascii.c == '0')
            {
                buf[0] = MEDIBUS_ASCIISPACE;
            }
            else
            {
                buf[0] = Int2Ascii.c;
            }
            buf[1] = Int2Ascii.d;  
            buf[2] = MEDIBUS_ASCIISPACE;   
            buf[3] = MEDIBUS_ASCIISPACE;             
        }
        break;
    case MEDIBUS_FlowO2:
    case MEDIBUS_FlowN2O:
    case MEDIBUS_FlowAir:
        if(data_type == MEDIBUS_FlowO2)
        {
            if(MeasuredVal.FlowO2 <= FLOW_MAX_MB)
            {
                Int2Ascii = Medibus_int2ascii(MeasuredVal.FlowO2);
            }
            else
            {
                Int2Ascii = Medibus_int2ascii(FLOW_MAX_MB);
            }
        }
        else if(data_type == MEDIBUS_FlowN2O)
        {
            if(MeasuredVal.FlowN2O <= FLOW_MAX_MB)
            {
                Int2Ascii = Medibus_int2ascii(MeasuredVal.FlowN2O);
            }
            else
            {
                Int2Ascii = Medibus_int2ascii(FLOW_MAX_MB);
            }            
        }
        else
        {
            if(MeasuredVal.FlowAir <= FLOW_MAX_MB)
            {
                Int2Ascii = Medibus_int2ascii(MeasuredVal.FlowAir);
            }
            else
            {
                Int2Ascii = Medibus_int2ascii(FLOW_MAX_MB);
            }             
        }
        if(Int2Ascii.a == '0')
        {
            buf[0] = MEDIBUS_ASCIISPACE;
            if(Int2Ascii.b == '0')
            {
                buf[1] = MEDIBUS_ASCIISPACE;
                if(Int2Ascii.c == '0')
                {
                    buf[2] = MEDIBUS_ASCIISPACE;
                }
                else
                {
                    buf[2] = Int2Ascii.c;
                }
            }
            else
            {
                buf[1] = Int2Ascii.b;
                buf[2] = Int2Ascii.c;
            }
        }
        else
        {
            buf[0] = Int2Ascii.a;
            buf[1] = Int2Ascii.b;
            buf[2] = Int2Ascii.c;
        }
        buf[3] = Int2Ascii.d;
        break;
    case MEDIBUS_EtHAL:
    case MEDIBUS_EtENF:
    case MEDIBUS_EtISO:
    case MEDIBUS_EtSEV:
    case MEDIBUS_EtDES:
        Int2Ascii = Medibus_int2ascii(MeasuredVal.EtAAVal);
        if(Int2Ascii.b == '0')
        {
            buf[0] = MEDIBUS_ASCIISPACE;
        }
        else
        {
            buf[0] = Int2Ascii.b;
        }
        buf[1] = Int2Ascii.c;        
        buf[2] = '.';   
        buf[3] = Int2Ascii.d;         
        break;
    case MEDIBUS_Pulse:
    case MEDIBUS_SpO2:
        if(data_type == MEDIBUS_Pulse)
        {
            Int2Ascii = Medibus_int2ascii(MeasuredVal.PulseVal);
        }
        else
        {
            Int2Ascii = Medibus_int2ascii(MeasuredVal.SpO2Val);
        }
        if(Int2Ascii.b == '0')
        {
            buf[0] = MEDIBUS_ASCIISPACE;
            if(Int2Ascii.c == '0')
            {
                buf[1] = MEDIBUS_ASCIISPACE;
                                
            }
            else
            {
                buf[1] = Int2Ascii.c;                
            }
            buf[2] = Int2Ascii.d; 
        }
        else
        {
            buf[0] = Int2Ascii.b;
            buf[1] = Int2Ascii.c;
            buf[2] = Int2Ascii.d;
        }
        buf[3] = MEDIBUS_ASCIISPACE;          
        break;
    default:
        Int2Ascii = Medibus_int2ascii(0x00);
        break;
    }
}

void Medibus_SendTextMess(void)
{
    u8 len = 0;
    Medibus_ASCII MeauConver = {0};
    
    Medibus_GetTextMess();
    Medibus_Type.Medibus_SendBuf[0] = 0;
    Medibus_Type.Medibus_SendBuf[1] = (u8)MEDIBUS_SOH;
    Medibus_Type.Medibus_SendBuf[2] = (u8)MEIDBUS_TEXTMESS;//回复收到的命令
    
    len += 3;
    
    if(!StandbyState)
    {
    #ifdef __BREATHMODE    
        if(Old_MeasuredVal.BreathMode != MeasuredVal.BreathMode)
        {
    #ifndef __Medibus_Debug        
            //Old_MeasuredVal.BreathMode = MeasuredVal.BreathMode;
    #endif
            switch(MeasuredVal.BreathMode)
            {
            case VCV_MODE:
            default:
                MeauConver = HEX2ASCII(MEDIBUS_VCV);
                Medibus_Type.Medibus_SendBuf[len + 0] = MeauConver.ASCII_HIGH;
                Medibus_Type.Medibus_SendBuf[len + 1] = MeauConver.ASCII_LOW;
                Medibus_Type.Medibus_SendBuf[len + 2] = strlen(TextMessage_Buf[0]); 
                strcpy((char*)&Medibus_Type.Medibus_SendBuf[len + 3],TextMessage_Buf[0]);
                len += Medibus_Type.Medibus_SendBuf[len + 2] + 3;
                break;
            case PCV_MODE:
                MeauConver = HEX2ASCII(MEDIBUS_PCV);
                Medibus_Type.Medibus_SendBuf[len + 0] = MeauConver.ASCII_HIGH;
                Medibus_Type.Medibus_SendBuf[len + 1] = MeauConver.ASCII_LOW;
                Medibus_Type.Medibus_SendBuf[len + 2] = strlen(TextMessage_Buf[1]);
                strcpy((char*)&Medibus_Type.Medibus_SendBuf[len + 3],TextMessage_Buf[1]); 
                len += Medibus_Type.Medibus_SendBuf[len + 2] + 3;
                break;
            case PSV_MODE:
                MeauConver = HEX2ASCII(MEDIBUS_PSV);
                Medibus_Type.Medibus_SendBuf[len + 0] = MeauConver.ASCII_HIGH;
                Medibus_Type.Medibus_SendBuf[len + 1] = MeauConver.ASCII_LOW;
                Medibus_Type.Medibus_SendBuf[len + 2] = strlen(TextMessage_Buf[2]);
                strcpy((char*)&Medibus_Type.Medibus_SendBuf[len + 3],TextMessage_Buf[2]); 
                len += Medibus_Type.Medibus_SendBuf[len + 2] + 3;
                break;
            case PRVC_MODE:
                MeauConver = HEX2ASCII(MEDIBUS_PRVC);
                Medibus_Type.Medibus_SendBuf[len + 0] = MeauConver.ASCII_HIGH;
                Medibus_Type.Medibus_SendBuf[len + 1] = MeauConver.ASCII_LOW;
                Medibus_Type.Medibus_SendBuf[len + 2] = strlen(TextMessage_Buf[3]);
                strcpy((char*)&Medibus_Type.Medibus_SendBuf[len + 3],TextMessage_Buf[3]);
                len += Medibus_Type.Medibus_SendBuf[len + 2] + 3;            
                break;
            case SIMV_VCV_MODE:
                MeauConver = HEX2ASCII(MEDIBUS_SIMV_V);
                Medibus_Type.Medibus_SendBuf[len + 0] = MeauConver.ASCII_HIGH;
                Medibus_Type.Medibus_SendBuf[len + 1] = MeauConver.ASCII_LOW;
                Medibus_Type.Medibus_SendBuf[len + 2] = strlen(TextMessage_Buf[4]);
                strcpy((char*)&Medibus_Type.Medibus_SendBuf[len + 3],TextMessage_Buf[4]);  
                len += Medibus_Type.Medibus_SendBuf[len + 2] + 3;
                break;
            case SIMV_PCV_MODE:
                MeauConver = HEX2ASCII(MEDIBUS_SIMV_P);
                Medibus_Type.Medibus_SendBuf[len + 0] = MeauConver.ASCII_HIGH;
                Medibus_Type.Medibus_SendBuf[len + 1] = MeauConver.ASCII_LOW;
                Medibus_Type.Medibus_SendBuf[len + 2] = strlen(TextMessage_Buf[5]);
                strcpy((char*)&Medibus_Type.Medibus_SendBuf[len + 3],TextMessage_Buf[5]);
                len += Medibus_Type.Medibus_SendBuf[len + 2] + 3;            
                break;
            case SIMV_PRVC_MODE:
                MeauConver = HEX2ASCII(MEDIBUS_SIMV_PRVC);
                Medibus_Type.Medibus_SendBuf[len + 0] = MeauConver.ASCII_HIGH;
                Medibus_Type.Medibus_SendBuf[len + 1] = MeauConver.ASCII_LOW;
                Medibus_Type.Medibus_SendBuf[len + 2] = strlen(TextMessage_Buf[6]);
                strcpy((char*)&Medibus_Type.Medibus_SendBuf[len + 3],TextMessage_Buf[6]);   
                len += Medibus_Type.Medibus_SendBuf[len + 2] + 3;
                break;
            case ACGO_MODE:
                MeauConver = HEX2ASCII(MEDIBUS_ACGO);
                Medibus_Type.Medibus_SendBuf[len + 0] = MeauConver.ASCII_HIGH;
                Medibus_Type.Medibus_SendBuf[len + 1] = MeauConver.ASCII_LOW;
                Medibus_Type.Medibus_SendBuf[len + 2] = strlen(TextMessage_Buf[7]);
                strcpy((char*)&Medibus_Type.Medibus_SendBuf[len + 3],TextMessage_Buf[7]);     
                len += Medibus_Type.Medibus_SendBuf[len + 2] + 3;
                break;
            case MANUAL_MODE:
                MeauConver = HEX2ASCII(MEDIBUS_MANUAL);
                Medibus_Type.Medibus_SendBuf[len + 0] = MeauConver.ASCII_HIGH;
                Medibus_Type.Medibus_SendBuf[len + 1] = MeauConver.ASCII_LOW;
                Medibus_Type.Medibus_SendBuf[len + 2] = strlen(TextMessage_Buf[8]);
                strcpy((char*)&Medibus_Type.Medibus_SendBuf[len + 3],TextMessage_Buf[8]);  
                len += Medibus_Type.Medibus_SendBuf[len + 2] + 3;
                break;
            case SIGH_MODE:
                MeauConver = HEX2ASCII(MEDIBUS_SIGH);
                Medibus_Type.Medibus_SendBuf[len + 0] = MeauConver.ASCII_HIGH;
                Medibus_Type.Medibus_SendBuf[len + 1] = MeauConver.ASCII_LOW;
                Medibus_Type.Medibus_SendBuf[len + 2] = strlen(TextMessage_Buf[9]);
                strcpy((char*)&Medibus_Type.Medibus_SendBuf[len + 3],TextMessage_Buf[9]);  
                len += Medibus_Type.Medibus_SendBuf[len + 2] + 3;
                break;
            }
            Medibus_Type.Medibus_SendBuf[len] = MEDIBUS_MESSETX;
            len++;
        }
    #endif
        
    #ifdef __ETCO2
        if(Old_MeasuredVal.CO2_Unit != MeasuredVal.CO2_Unit)
        {
    #ifndef __Medibus_Debug        
            Old_MeasuredVal.CO2_Unit = MeasuredVal.CO2_Unit;
    #endif
            switch(MeasuredVal.CO2_Unit)
            {
            case 0://%
            default:
                MeauConver = HEX2ASCII(MEDIBUS_UNIT_SIGN);
                Medibus_Type.Medibus_SendBuf[len + 0] = MeauConver.ASCII_HIGH;
                Medibus_Type.Medibus_SendBuf[len + 1] = MeauConver.ASCII_LOW;
                Medibus_Type.Medibus_SendBuf[len + 2] = strlen(TextMessage_Buf[12]);  
                strcpy((char*)&Medibus_Type.Medibus_SendBuf[len + 3],TextMessage_Buf[12]); 
                len += Medibus_Type.Medibus_SendBuf[len + 2] + 3;
                break;
            case 1://mmhg
                MeauConver = HEX2ASCII(MEDIBUS_UNIT_MMHG);
                Medibus_Type.Medibus_SendBuf[len + 0] = MeauConver.ASCII_HIGH;
                Medibus_Type.Medibus_SendBuf[len + 1] = MeauConver.ASCII_LOW;
                Medibus_Type.Medibus_SendBuf[len + 2] = strlen(TextMessage_Buf[10]);
                strcpy((char*)&Medibus_Type.Medibus_SendBuf[len + 3],TextMessage_Buf[10]);
                len += Medibus_Type.Medibus_SendBuf[len + 2] + 3;
                break;
            case 2://kPa
                MeauConver = HEX2ASCII(MEDIBUS_UNIT_KPA);
                Medibus_Type.Medibus_SendBuf[len + 0] = MeauConver.ASCII_HIGH;
                Medibus_Type.Medibus_SendBuf[len + 1] = MeauConver.ASCII_LOW;
                Medibus_Type.Medibus_SendBuf[len + 2] = strlen(TextMessage_Buf[11]);    
                strcpy((char*)&Medibus_Type.Medibus_SendBuf[len + 3],TextMessage_Buf[11]);
                len += Medibus_Type.Medibus_SendBuf[len + 2] + 3;
                break;
            }
            Medibus_Type.Medibus_SendBuf[len] = MEDIBUS_MESSETX;
            len++;
        }
    #endif  
    }
    
    Medibus_Type.Medibus_SendNum = len - 1 + 3;//len + CHK_H + CHK_L + CR - [0] = total len
    Medibus_Type.Medibus_SendBuf[0] = Medibus_Type.Medibus_SendNum;
    Medibus_CheckSumCal(&Medibus_Type.Medibus_SendBuf[1],Medibus_Type.Medibus_SendNum);
    Medibus_Type.Medibus_SendBuf[Medibus_Type.Medibus_SendNum] = MEDIBUS_CR;
    
    uart_dp_wrsend(&Medibus_Type.Medibus_SendBuf[1], Medibus_Type.Medibus_SendNum);    
}

void Medibus_CheckSumCal(u8 *buf, u8 num)
{
    u8 i, CHK = 0;
    
    if(buf == NULL)
        return;
    
    for(i=0;i<num-3;i++)
    {
        CHK += buf[i];
    }
    Medibus_Conver = HEX2ASCII(CHK);
    buf[num-3] = Medibus_Conver.ASCII_HIGH;
    buf[num-2] = Medibus_Conver.ASCII_LOW;
}

Medibus_ASCII HEX2ASCII(u8 data)
{
    Medibus_ASCII Conver = {0};
    u8 data_low = 0,data_high = 0;
    
    data_high = (data>>4)&0x0F;
    data_low  = (data>>0)&0x0F;
    
    if(data_high <= 0x09)
    {
        Conver.ASCII_HIGH = '0' + data_high;
    }
    else if(data_high >= 0x0A && data_high <= 0x0F)
    {
        Conver.ASCII_HIGH = 'A' + data_high - 0x0A;
    }
    
    if(data_low <= 0x09)
    {
        Conver.ASCII_LOW = '0' + data_low;
    }
    else if(data_low >= 0x0A && data_low <= 0x0F)
    {
        Conver.ASCII_LOW = 'A' + data_low - 0x0A;
    }
    
    return Conver;
}

Medibus_I2A Medibus_int2ascii(u16 val)
{
    Medibus_I2A I2A = {0};
    
    I2A.a = val/1000    + 0x30;
    I2A.b = val/100%10  + 0x30;
    I2A.c = val/10%10   + 0x30;
    I2A.d = val%10      + 0x30;
    
    return I2A;
}

/*
*@}
*/

/*
*@{ get para / mode
*/

void Medibus_GetMeasuredData(void)
{
    
#ifdef __PPLAT
    MeasuredVal.PplatVal = ds_readpara_Pplat();
#endif
    
#ifdef __PEEP    
    MeasuredVal.PEEPVal  = ds_readpara_PEEP();
#endif 
    
#ifdef __PPEAK     
    MeasuredVal.PpeakVal = ds_readpara_Ppeak();
#endif
    
#ifdef __VTE  
    MeasuredVal.VTeVal   = ds_readpara_VTe();
#endif 
    
#ifdef __VTI     
    MeasuredVal.VTiVal   = ds_readpara_VTi();
#endif 
    
#ifdef __RATE    
    MeasuredVal.RateVal  = ds_readpara_f();
#endif 
    
#ifdef __IE     
    MeasuredVal.IEIVal   = ds_readpara_IEI();
    MeasuredVal.IEEVal   = ds_readpara_IEE();
#endif
    
#ifdef __MV     
    MeasuredVal.MVVal    = ds_readpara_MV();
#endif
    
#ifdef __FIO2
    MeasuredVal.FiO2Val  = ds_readpara_FiO2();
#endif
    
#ifdef __ETCO2
    MeasuredVal.EtCO2Val = ds_readpara_EtCO2();
    MeasuredVal.CO2_Unit = ds_readpara_CO2Unit();    
#endif
    
#ifdef __FLOWO2
    MeasuredVal.FlowO2   = ds_readpara_Flow_O2();
#endif
    
#ifdef __FLOWN2O
    MeasuredVal.FlowN2O  = ds_readpara_Flow_N2O();
#endif
    
#ifdef __FLOWAIR
    MeasuredVal.FlowAir  = ds_readpara_Flow_Air();
#endif
    
#ifdef __ETN2O
    MeasuredVal.EtN2OVal = ds_readpara_EtN2O();
#endif
    
#ifdef __ETAA
    MeasuredVal.EtAAVal  = ds_readpara_EtAA();
    MeasuredVal.AA_Type  = ds_readpara_AA1ID();    
#endif
    
#ifdef __PULSE
    MeasuredVal.PulseVal = ds_readpara_PR();
#endif
    
#ifdef __SPO2
    MeasuredVal.SpO2Val  = ds_readpara_SpO2();
#endif
    
}

void Medibus_GetTextMess(void)
{
    MeasuredVal.BreathMode = ds_readpara_BreathMode();
}

/*
*@}
*/




/*
*@}
*/

