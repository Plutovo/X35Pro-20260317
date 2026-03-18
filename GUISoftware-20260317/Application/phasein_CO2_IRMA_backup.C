/*
*********************************************************************************************************
文件名称：co2_irma.c
文件内容：co2_irma模块C源文件。
          CO2_IRMA模块主要完成对PHASE IN公司气体模块的底层控制
                  返回模块产生的各种气体数据和设置模块的各种参数
*********************************************************************************************************
*/
//#ifndef Config_H
//#include "Config.h"
//#endif
#define  _CO2_IRMA_C_
/*
#include <dos.h>
*/
#include <stdio.h>
#include "co2_irma.h"
#include "Alarm_handle.h"
#include "GUI.h"
#include "Key_Handle.h"
#include "Static_Menu_Creat.h"

extern  unsigned char EtCO2ModuleExist;//djq add 2013-12-23
extern ALARM_WORK AlarmsWork;//sun add 20140821
extern Int16U phaseinAlarm;  //phasein 技术报警标志  //sun add 20140821
extern int PlanMenuType;
extern MENU_HWIN Menu_hWin;
extern int Task;
extern unsigned char pHaseinMoudleCheck;//检测pHasein是否连接标志
unsigned int pHaseinFlag = 0;//检测pHasein是否连接的计时标志
extern unsigned char EtCO2ModuleOff;
extern unsigned char EtCO2_Config_On;
extern unsigned char Masimo_EtCO2_Config_On ;
////////////////////////////////////////////////////////////////////////////////
//帧数据起始标识
////////////////////////////////////////////////////////////////////////////////
//接收帧命令 2009-12-23 ht 将接收的帧号定义为枚举
////////////////////////////////////////////////////////////////////////////////
//【变量定义】
static enum IM_FUNC_STS  Irma_OnOff;     //模块打开/关闭状态 [IM_ON/IM_OFF/IM_INIT]
IM_ALL_DATA      pAll_DataPP;//指向用户区数据，增加定义时赋初值 2009-12-8 ht add
IM_ALL_DATA      * pAll_Data = &pAll_DataPP;//指向用户区数据，增加定义时赋初值 2009-12-8 ht add
static IM_ALL_DATA UserDataMem;//供用户使用的数据的空间 2009-12-8 ht add
static struct IRMA_RX_CTRL Irma_RX_Ctrl;//数据接收控制字
static struct IRMA_RX_FRAME Irma_RX_Frame[8];//接收缓冲区，采用队列结构 2009-12-8 ht
//指向接收缓冲区指针的定义，必须在Irma_UserData_Init()中初始化为0，必须保证每次通讯恢复时指针为0
static unsigned char RxbufTail;//指向接收缓冲区队尾，指示新接收的来自模块的数据应放在缓冲区的哪个位置
static unsigned char RxbufHead;//指向接收缓冲区队首，用于指示用户数据已更新至缓冲区的哪个位置
//接收缓冲区指针（tail，head）掩码(保留低三位，当前缓冲区大小为8)
#define IM_RXBUF_PTR_MASK 0x07
#define IM_RX_TAIL (RxbufTail&IM_RXBUF_PTR_MASK)
#define IM_RX_HEAD (RxbufHead&IM_RXBUF_PTR_MASK)
//增加读写标志，用于保护用户区UserDataMem中的数据 2009-12-9 ht
static unsigned char ReadFlag = 0;
static unsigned char WriteFlag = 1;
////////////////////////////////////////////////////////////////////////////////
static void Irma_UserData_Init(void);//用户数据初始化(清用户空间;清发送控制字;设置自带O2)
static void Irma_UserData_Updt(void); //用户数据更新
static enum IM_FUNC_STS Irma_setOnOff(enum IM_FUNC_STS onoff);//该函数转静态 2009-12-9 ht
int Im_getSlowData(enum IM_GET_DATA FirstPara, unsigned char SecondPara);//该函数转静态 2009-12-9 ht
void Module_Alarm_Judge(void);  //phasein技术报警判断函数  //sun add 20140821
static enum IM_FUNC_STS  Irma_getOnOff(void);//该函数转静态 2009-12-10 ht
////////////////////////////////////////////////////////////////////////////////
//【函数区】
//接收状态变量缩写(用于Irma_RX_Int/Irma_RX_Process函数)
#define STATE0       Irma_RX_Ctrl.R_state[0]
#define STATE1       Irma_RX_Ctrl.R_state[1]
#define COMMERR_CNT  Irma_RX_Ctrl.R_commEr_cnt
#define COMM_TMOUT   Irma_RX_Ctrl.R_commEr_flag

//【初始化】
/******************************************************************************
  名称：模块数据初始化函数
  作用：上电后使本模块内的标志、状态等数据初始化。
  入口：无
  调用：上电时在main函数，或在用户的上级初始化函数中
******************************************************************************/
//void  Irma_Host_Init(IM_ALL_DATA  * pAll, char ( * pSendByte)(char ch))
/*改由本地申请数据需要的空间，防止用户修改接收数据 */
void  Irma_Host_Init(void)
{
    unsigned char i, *p;
    //设置模块为"初始化状态"且通讯状态为失败
    Irma_setOnOff(IM_INIT);
    COMM_TMOUT = 1;
    //接收帧和接收控制初始化
    p = &(Irma_RX_Frame[0].id);//指向接收缓冲区的首址
    i = sizeof(Irma_RX_Frame);
    while(i--)
    {
        *p++ = 0;
    }
    p = &Irma_RX_Ctrl.R_nowbyte;
    i = sizeof(Irma_RX_Ctrl);
    while(i--)
    {
        *p++ = 0;
    }
    //用户数据初始化，清用户空间
    Irma_UserData_Init();
}
//函数功能：清用户数据空间; 清发送控制字;在函数中使用户数据区指针指向本地申请的存储空间UserDataMem
//输入：无
//输出：无
static void  Irma_UserData_Init(void)
{
    unsigned char *p;
    unsigned int   i;
#ifdef IM_DEBUG
    if (pAll_Data == NULL)
    {
        printf("\n\n\n\n");//首次运行时执行，以后不再执行
    }
#endif
    pAll_Data = &UserDataMem; //指向用户数据的存储空间（非用户提供的空间，本地申请）
    RxbufHead = 0; //指向接收缓冲区的指针复位为0
    RxbufTail = 0;
    //清用户空间
    p = (unsigned char *)pAll_Data;
    i = sizeof(IM_ALL_DATA);
    while(i--)
    {
        *p++ = 0;
    }
}
//函数功能：仅当新接收的一帧数据正确时，数据才存储到缓冲区Irma_RX_Frame，
//注：需将此函数放到与模块相连的串口中断函数中。
//输入：RxByte--接收到的1字节数据
//输出：将收到的数据赋给 static Irma_RX_Frame 结构.
//      根据接收情况，返回3个可能的结果: (见"enum IM_FUNC_STS")
//           IM_OK:    数据接收正常
//           IM_BUSY:  数据正在接收
//           IM_NODEF: 数据接收错误<比如不是IRMA数据或未收到数据>
/******************************************************************************/
enum IM_FUNC_STS  Irma_RX_Int(unsigned char RxByte)
{
#define NOWBYTE      Irma_RX_Ctrl.R_nowbyte
#define CHKSUM       Irma_RX_Ctrl.R_chksum
#define RTNSTAT      Irma_RX_Ctrl.R_rtnstat
    unsigned char NowByte = NOWBYTE;
    unsigned char state = 0;
    
    pHaseinMoudleCheck = 0;//检测pHasein是否连接标志
    pHaseinFlag = 0;//检测pHasein是否连接的计时标志
    switch(NOWBYTE)
    {
    case 1://IRMA_RX_FLG2
        if(RxByte == IRMA_DATA_FLAG2) //0x55
        {
            NOWBYTE = 2;
        }
        else //[NOWBYTE=1 & RxByte != 0x55  ErrData!]
        {
            NOWBYTE = 0;
        }
        RTNSTAT = IM_NODEF;
        break;
    case 2://IRMA_RX_ID
        if(RxByte < 10) //共10帧数据
            //接收新数据, 否则返回NOWBYTE0放弃此帧
        {
            Irma_RX_Frame[IM_RX_TAIL].id = RxByte;
            CHKSUM += RxByte;     //start chksum from here
            state  = RxByte + 1; //change the Irma_RX_Ctrl.R_state[0] here
            NOWBYTE = 3;
            RTNSTAT = IM_BUSY;
        }
        else //[R_state=2 & RxByte>10  ErrData!]
        {
            NOWBYTE = 0;
            RTNSTAT = IM_NODEF;
        }
        break;
    case 3://IRMA_RX_STS
        Irma_RX_Frame[IM_RX_TAIL].status = RxByte;
        CHKSUM += RxByte;
        NOWBYTE = 4;
        RTNSTAT = IM_BUSY;
        break;
    case 4://WAVE_DATA_STA
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13://WAVE_DATA_END//不计算，只插数 2009-12-22 ht modified
        Irma_RX_Frame[IM_RX_TAIL].wave_data[NOWBYTE-4] = RxByte;
        break;
    case 14://SLOW_DATA_STA
    case 15:
    case 16:
    case 17:
    case 18:
    case 19://SLOW_DATA_END
        Irma_RX_Frame[IM_RX_TAIL].slow_data[NOWBYTE-14] = RxByte;
        //CHKSUM += RxByte;
        //NOWBYTE++;
        break;
    case 20://nCHK_SUM
        Irma_RX_Frame[IM_RX_TAIL].chk_sum = RxByte;
//        CHKSUM += RxByte; //收到的校验和是算出的校验和的补码,2者相加应=0
        CHKSUM = 0;//djq add 2013-12-23
        if(EtCO2ModuleExist != 2)
        {
            EtCO2ModuleExist = 2;   //djq add 2013-12-23
            
            //ZY 增加 2016.6.29
            if((EtCO2ModuleExist) && (Masimo_EtCO2_Config_On || EtCO2_Config_On))
            {
                EtCO2ModuleOff = RTEEPROMread(ETCO2_ON_OFF_ADDR); 
            }
            else
            {
                EtCO2ModuleOff = 0;
            }             
            alarm_str_init();
        }
        if(CHKSUM==0) //Data OK
        {
            //获取模块状态
            switch (Irma_OnOff)
            {
            case IM_ON:   //通讯正常且数据正确,【将STATE0 | 0x80】表示可以刷新用户数据
                STATE0 = (0x80 | state);
                break;
            case IM_OFF:  //通讯失败超时
            case IM_INIT: //初始化状态   [STATE1 >= 90, 切换状态到IM_ON]
            default:      //2009-12-22 ht add
                //连续收2组正确数据才进入IM_ON
                STATE1 += Irma_RX_Frame[IM_RX_TAIL].id;
                if(STATE1 >= 90) //0+1+..+9=45
                {
                    STATE0 |= 0x80;
                    STATE1 = 90;
                    Irma_OnOff = IM_ON;  //【设置状态到onoff】
                    COMM_TMOUT = 0;      //通讯恢复正常
                    //printf("\n\n\n\n\n\n              Have set IM_ON\n");
                }
                break;
            }
            ++RxbufTail;//该帧数据正确，调整指针指向下一存储区
            RTNSTAT = IM_OK;
        }
        else        //Data CHK ERR  此帧将不被处理
        {
            //数据校验错误,清STATE0,STATE1.
            STATE0 = 0;
            STATE1 = 0;
            RTNSTAT = IM_NODEF;
        }
        NOWBYTE = 0;
        break;
    case 0://IRMA_RX_FLG1
    default:
        if(RxByte == IRMA_DATA_FLAG1) //0xAA
        {
            NOWBYTE = 1;
        }
        else //[NOWBYTE=0 & RxByte != 0xAA  ErrData!]
        {
            NOWBYTE = 0;
        }
        CHKSUM  = 0;
        RTNSTAT = IM_NODEF;
        break;
    } //switch(NOWBYTE) End
    if((NowByte >= 4) && (NowByte <= 19))
    {
        CHKSUM += RxByte;
        NOWBYTE++;
    }
    return (RTNSTAT);
}
//函数功能：将缓冲区Irma_RX_Frame中新收到的数据按协议赋值到pUserData指向的用户数据存储空间
//输入：无
//输出：无 
static void Irma_UserData_Updt(void)
{
#define FRAME_ID     (Irma_RX_Frame[IM_RX_HEAD].id)
#define SUMMARY_STAT (Irma_RX_Frame[IM_RX_HEAD].status)
#define WAVE_DATA0   (Irma_RX_Frame[IM_RX_HEAD].wave_data[0])
#define WAVE_DATA1   (Irma_RX_Frame[IM_RX_HEAD].wave_data[2])
#define WAVE_DATA2   (Irma_RX_Frame[IM_RX_HEAD].wave_data[4])
#define WAVE_DATA3   (Irma_RX_Frame[IM_RX_HEAD].wave_data[6])
#define WAVE_DATA4   (Irma_RX_Frame[IM_RX_HEAD].wave_data[8])
#define SLOW_DATA0   (Irma_RX_Frame[IM_RX_HEAD].slow_data[0])
#define SLOW_DATA1   (Irma_RX_Frame[IM_RX_HEAD].slow_data[1])
#define SLOW_DATA2   (Irma_RX_Frame[IM_RX_HEAD].slow_data[2])
#define SLOW_DATA3   (Irma_RX_Frame[IM_RX_HEAD].slow_data[3])
#define SLOW_DATA4   (Irma_RX_Frame[IM_RX_HEAD].slow_data[4])
#define SLOW_DATA5   (Irma_RX_Frame[IM_RX_HEAD].slow_data[5])
    unsigned char updt = 0;//是否更新了数据，用于判断是否更新wavedata
    unsigned char *pwave = NULL;//指向波形数据的存储区
    unsigned char i = 0;
    static char zeroDisab = 0, zeroInprogress = 0;
    WM_HWIN hItem;
    while (IM_RX_HEAD != IM_RX_TAIL)
    {
        switch (Irma_RX_Frame[IM_RX_HEAD].id)  //将case条件中的数值换成枚举常量
        {
        case IRMA_INSP_VALS: //frame0: InspVals
            pAll_Data->slow.InspVal[IM_GAS_CO2] = SLOW_DATA0;
            pAll_Data->slow.InspVal[IM_GAS_N2O] = SLOW_DATA1;
            pAll_Data->slow.InspVal[IM_GAS_AX1] = SLOW_DATA2;
            pAll_Data->slow.InspVal[IM_GAS_AX2] = SLOW_DATA3;
            pAll_Data->slow.InspVal[IM_GAS_O2]  = SLOW_DATA4;
            break;
        case IRMA_EXP_VALS: //frame1: ExpVals
            pAll_Data->slow.ExpVal[IM_GAS_CO2] = SLOW_DATA0;
            pAll_Data->slow.ExpVal[IM_GAS_N2O] = SLOW_DATA1;
            pAll_Data->slow.ExpVal[IM_GAS_AX1] = SLOW_DATA2;
            pAll_Data->slow.ExpVal[IM_GAS_AX2] = SLOW_DATA3;
            pAll_Data->slow.ExpVal[IM_GAS_O2]  = SLOW_DATA4;
            break;
        case IRMA_MOM_VALS: //frame2: MomentVals
            pAll_Data->slow.MomtVal[IM_GAS_CO2] = SLOW_DATA0;
            pAll_Data->slow.MomtVal[IM_GAS_N2O] = SLOW_DATA1;
            pAll_Data->slow.MomtVal[IM_GAS_AX1] = SLOW_DATA2;
            pAll_Data->slow.MomtVal[IM_GAS_AX2] = SLOW_DATA3;
            pAll_Data->slow.MomtVal[IM_GAS_O2]  = SLOW_DATA4;
            break;
        case IRMA_GEN_VALS: //frame3: GenVals
            pAll_Data->slow.GenVal.bre_rate  = SLOW_DATA0;//0~254bpm
            pAll_Data->slow.GenVal.lstbre_tm = SLOW_DATA1;          //0~255s

            if (pAll_Data->slow.GenVal.ax1_id != SLOW_DATA2 & 0x7)  //zy 2016.7.12
            { 
                pAll_Data->slow.GenVal.ax1_id    = SLOW_DATA2 & 0x7;//主麻醉气ID
                AlarmsWork.presentAlarms &= ~(ALARM_FIAA_HIGH | ALARM_ETAA_HIGH | ALARM_FIAA_LOW |ALARM_ETAA_LOW);
            }
            if (pAll_Data->slow.GenVal.ax2_id != SLOW_DATA3 & 0x7)  //zy 2016.7.12     
            {
                pAll_Data->slow.GenVal.ax2_id    = SLOW_DATA3 & 0x7;//从麻醉气ID
                AlarmsWork.presentAlarms &= ~(ALARM_FIAA_HIGH | ALARM_ETAA_HIGH | ALARM_FIAA_LOW |ALARM_ETAA_LOW);
            }
            
            
            switch(pAll_Data->slow.GenVal.ax1_id)
            {
            case AX_Hal:
                pAll_Data->slow.GenVal.ax1_color = HALCOLOR;
                strcpy(pAll_Data->slow.GenVal.ax1_type, "HAL");
                break;
            case AX_Enf:
                pAll_Data->slow.GenVal.ax1_color = ENFCOLOR;
                strcpy(pAll_Data->slow.GenVal.ax1_type, "ENF");
                break;
            case AX_Iso:
                pAll_Data->slow.GenVal.ax1_color = ISOCOLOR;
                strcpy(pAll_Data->slow.GenVal.ax1_type, "ISO");
                break;
            case AX_Sev:
                pAll_Data->slow.GenVal.ax1_color = SEVCOLOR;
                strcpy(pAll_Data->slow.GenVal.ax1_type, "SEV");
                break;
            case AX_Des:  //sun add 20140605
                pAll_Data->slow.GenVal.ax1_color = DESCOLOR;
                strcpy(pAll_Data->slow.GenVal.ax1_type, "DES");
                break;
            default:
                strcpy(pAll_Data->slow.GenVal.ax1_type, " ");
            }
            switch(pAll_Data->slow.GenVal.ax2_id)
            {
            case AX_Hal:
                pAll_Data->slow.GenVal.ax2_color = HALCOLOR;
                strcpy(pAll_Data->slow.GenVal.ax2_type, "HAL");
                break;
            case AX_Enf:
                pAll_Data->slow.GenVal.ax2_color = ENFCOLOR;
                strcpy(pAll_Data->slow.GenVal.ax2_type, "ENF");
                break;
            case AX_Iso:
                pAll_Data->slow.GenVal.ax2_color = ISOCOLOR;
                strcpy(pAll_Data->slow.GenVal.ax2_type, "ISO");
                break;
            case AX_Sev:
                pAll_Data->slow.GenVal.ax2_color = SEVCOLOR;
                strcpy(pAll_Data->slow.GenVal.ax2_type, "SEV");
                break;
            case AX_Des:  //sun add 20140605
                pAll_Data->slow.GenVal.ax2_color = DESCOLOR;
                strcpy(pAll_Data->slow.GenVal.ax2_type, "DES");
                break;
            default:
                strcpy(pAll_Data->slow.GenVal.ax2_type, " ");
            }
            //大气压力(500~1300 -> 50.0~130.0kPa)[HEX] //2009-5-19 ls 'int' to 'uint'
            pAll_Data->slow.GenVal.atmp      = ((unsigned int)SLOW_DATA4 << 8) | SLOW_DATA5;
            break;
        case IRMA_SENSOR_REGS:
            pAll_Data->slow.GenVal.sen_mode  = SLOW_DATA0 & 0x3;     //0~3
            pAll_Data->slow.GenVal.nobre_tmout  = SLOW_DATA5;        //20~60s
            pAll_Data->slow.GenVal.o2_compn  = SLOW_DATA1; //0~115%
            //低5位=SenErrReg(SLOW_DATA2), 高3位=AdaptStatsReg(SLOW_DATA3)
            pAll_Data->stat.sbyte[2]  = (SLOW_DATA3<<5) | (SLOW_DATA2&0x1F);
            pAll_Data->stat.sbyte[1]  = SLOW_DATA4; //DataValidReg
            
            
            pAll_Data->slow.GenVal.sw_err = SLOW_DATA2 & 0x01; //软件版本错误，重启传感器 add 20140604
            
            pAll_Data->slow.GenVal.hW_err = SLOW_DATA2 & 0x02;  //硬件版本错误，更换传感器报警 20140604
            
            pAll_Data->slow.GenVal.m_fail = SLOW_DATA2 & 0x04; //电机速度超限 sy add 20140604
            
            pAll_Data->slow.GenVal.un_cal = SLOW_DATA2 & 0x08; //出厂校准缺失中sy add 20140604
            
            pAll_Data->slow.GenVal.repl_adapt = SLOW_DATA3 & 0x01; //更换适配器sy add 20140604
            
            pAll_Data->slow.GenVal.no_adapt = SLOW_DATA3 & 0x02; //无适配器，请连接适配器sy add 20140604
            
            // pAll_Data->slow.GenVal.o2_clg = SLOW_DATA3 & 0x04; //氧气端口堵塞sy add 20140604
                
            pAll_Data->slow.GenVal.co2_or = SLOW_DATA4 & 0x01; //二氧化碳超差sy add 20140604
            
            pAll_Data->slow.GenVal.n2o_or = SLOW_DATA4 & 0x02; //一氧化二氮超差sy add 20140604
            
            pAll_Data->slow.GenVal.ax_or = SLOW_DATA4 & 0x04; //至少一种麻醉气体超差sy add 20140604
            
            //   pAll_Data->slow.GenVal.o2_or = SLOW_DATA4 & 0x08; //氧气超差sy add 20140604
            pAll_Data->slow.GenVal.temp_or = SLOW_DATA4 & 0x10; //内部温度高sy add 20140604
            
            pAll_Data->slow.GenVal.press_or = SLOW_DATA4 & 0x20;  //大气压超出运行范围sy add 20140604
            
            pAll_Data->slow.GenVal.zero_or = SLOW_DATA4 & 0x40; //零基准不准确，需要归零sy add 20140604
            
            pAll_Data->slow.GenVal.agen_id_unreliable = SLOW_DATA4 & 0x80; //麻醉气体标识和浓度不可信sy add 20140604
            
            break;
        case IRMA_CONFIG_DATA: //frame5: ConfigData Versions
            //SLOW_DATA0 -> ConfigReg0
            //SLOW_DATA4 -> ComfigReg1
            //配置位集合ConfigReg1(b0)+ConfigReg0(b7->b0)
            pAll_Data->slow.VerVal.sen_type  = (((unsigned int)SLOW_DATA4<<8) | SLOW_DATA0);// & 0x1FF; //9bits->16bits
            alarm_str_init();
            //Hard Ver. BCD Code[0~99]
            pAll_Data->slow.VerVal.hw_ver    = SLOW_DATA1;
            //Soft Ver. BCD Code[0~9999]
            pAll_Data->slow.VerVal.sw_ver    = ((unsigned int)SLOW_DATA2<<8) | SLOW_DATA3;
            //Comm Protocol Ver. BCD Code
            pAll_Data->slow.VerVal.commp_ver = SLOW_DATA5;
            break;
        case IRMA_SERVICE_DATA: //frame6: ServiceData
            pAll_Data->stat.sbyte[0] = SLOW_DATA2 & 0xF;//SLOW_DATA2低4位=校正状态(主流+支流)
            pAll_Data->slow.GenVal.ir_o2_dly  = SLOW_DATA2 >> 4;//SLOW_DATA2高4位=IR->O2延时【SideStream Only】
            
            
            pAll_Data->slow.GenVal.zero_disab = SLOW_DATA2 & 0x01;//该位不为零则禁止0点标校   sy add 20140606
            pAll_Data->slow.GenVal.zero_inprogress = SLOW_DATA2 & 0x02;//该位不为零则零点标校进行中 sy add 20140606
//            if((pAll_Data->slow.GenVal.zero_disab != zeroDisab)||(pAll_Data->slow.GenVal.zero_inprogress != zeroInprogress))  //sun add 20140929
//            {
//                if(PlanMenuType == SYSTEM_SETTING_MENU)
//                {
//                    hItem = WM_GetDialogItem(Menu_hWin.ActivedWin,GUI_ID_MULTIPAGE0);
//                    WM_SendMessageNoPara(MULTIPAGE_GetWindow(hItem,3),WM_INIT_DIALOG);
//                    Task|=MEAU_UPDATE;
//                }
//            }
//            zeroDisab = pAll_Data->slow.GenVal.zero_disab;
//            zeroInprogress = pAll_Data->slow.GenVal.zero_inprogress;
            
            
            pAll_Data->slow.GenVal.n2o_compn  = SLOW_DATA3;//0~100%
            pAll_Data->slow.GenVal.atmp_cuve = (int)SLOW_DATA5 - 20;//-20~234[-20=-2.0kPa,234=23.4kPa]
            //模块串号3B [HI->LO: SLOW_DATA4->SLOW_DATA0->SLOW_DATA1]
            pAll_Data->slow.VerVal.serial_num = ((long)SLOW_DATA4<<16) |
                                                ((unsigned int)SLOW_DATA0<<8) | //2009-5-19 ls 'int' to 'uint'
                                                SLOW_DATA1;
            break;
        default: //frame7/8/9 Reserved
            break;
        }
        ++RxbufHead;
        updt = 1; //有数据更新
    }
    if (updt) //wavedata只更新最新的数据
    {
        if(EtCO2ModuleExist == 2)
        {
            Module_Alarm_Judge(); //sun add 20140821
        }
        --RxbufHead;
        pAll_Data->fast.frm_id    = FRAME_ID;
        pAll_Data->fast.sum_stat  = \
                                    pAll_Data->stat.sbyte[3]  = SUMMARY_STAT; //模块总状态
        for (pwave = &WAVE_DATA0; i <= 4; i++, pwave++)//计算波形值
        {
            pAll_Data->fast.wv_val[i] = ((unsigned int)(*pwave)<<8);
            pAll_Data->fast.wv_val[i] += *(++pwave);
        }
        ++RxbufHead;
    }
}
////////////////////////////////////////////////////////////////////////////////
//print use data on screen
#ifdef IM_DEBUG
void printUdata(void)
{
    printf("ID=%d    STS=%d\n", pAll_Data->fast.frm_id, pAll_Data->fast.sum_stat);
    printf("wavedata: CO2=%d  N2O=%d  AX1=%d  AX2=%d  O2=%d\n", pAll_Data->fast.wv_val[IM_GAS_CO2],
           pAll_Data->fast.wv_val[IM_GAS_N2O], pAll_Data->fast.wv_val[IM_GAS_AX1],
           pAll_Data->fast.wv_val[IM_GAS_AX2], pAll_Data->fast.wv_val[IM_GAS_O2]);
    printf("slowdata:\n");
    switch (pAll_Data->fast.frm_id)
    {
    case 0: //frame0: InspVals
        printf("INSP  CO2=%d  N2O=%d  AX1=%d  AX2=%d  O2=%d\n", pAll_Data->slow.InspVal[IM_GAS_CO2],
               pAll_Data->slow.InspVal[IM_GAS_N2O], pAll_Data->slow.InspVal[IM_GAS_AX1],
               pAll_Data->slow.InspVal[IM_GAS_AX2], pAll_Data->slow.InspVal[IM_GAS_O2]);
        break;
    case 1: //frame1: ExpVals
        printf("EXP  CO2=%d  N2O=%d  AX1=%d  AX2=%d  O2=%d\n", pAll_Data->slow.ExpVal[IM_GAS_CO2],
               pAll_Data->slow.ExpVal[IM_GAS_N2O], pAll_Data->slow.ExpVal[IM_GAS_AX1],
               pAll_Data->slow.ExpVal[IM_GAS_AX2], pAll_Data->slow.ExpVal[IM_GAS_O2]);
        break;
    case 2: //frame2: MomentVals
        printf("MOM  CO2=%d  N2O=%d  AX1=%d  AX2=%d  O2=%d\n", pAll_Data->slow.MomtVal[IM_GAS_CO2],
               pAll_Data->slow.MomtVal[IM_GAS_N2O], pAll_Data->slow.MomtVal[IM_GAS_AX1],
               pAll_Data->slow.MomtVal[IM_GAS_AX2], pAll_Data->slow.MomtVal[IM_GAS_O2]);
        break;
    case 3: //frame3: GenVals
        printf("rate=%d  lastbreathtime=%d  ax1=%d  ax2=%d  atmp=%d\n", pAll_Data->slow.GenVal.bre_rate,
               pAll_Data->slow.GenVal.lstbre_tm, pAll_Data->slow.GenVal.ax1_id,
               pAll_Data->slow.GenVal.ax2_id, pAll_Data->slow.GenVal.atmp);
        break;
    case 4:
        printf("mode=%d  apnea time=%d  o2_comp=%d\n", pAll_Data->slow.GenVal.sen_mode,
               pAll_Data->slow.GenVal.nobre_tmout, pAll_Data->slow.GenVal.o2_compn);
        break;
    case 5: //frame5: ConfigData Versions
        break;
    case 6: //frame6: ServiceData
        printf("n2o_comp=%d\n", pAll_Data->slow.GenVal.n2o_compn);
        break;
    default: //frame7/8/9 Reserved
        break;
    }
}
#endif
//函数功能：处理缓冲区数据，并返回数据处理的结果状态
//输入：无
//输出:数据处理的结果：IM_OK表示更新了用户区数据，
//                     IM_ERR表示模块接收数据有错误
//                     IM_BUSY表示用户正在读数据
enum IM_FUNC_STS Irma_RX_Process(void)
{
#ifdef IM_DEBUG
    static unsigned int cnt = 1500;
#endif
    static unsigned char reset_flag = 0;//用于在IM_OFF AND IN_INIT清除用户区数据
    switch(Irma_getOnOff())
    {
    case IM_ON:     //打开状态(实时更新用户数据)
        if(STATE0 & 0x80) //接收数据正确, 处理数据,处理完毕会将STATE0 = 0
        {
            //【更新用户数据】//////////////////////////////////////////////
            if (ReadFlag)//加入互斥标志，保护数据
            {
                return IM_BUSY; //用户正在读数据
            }
            else
            {
                WriteFlag = 1;
                Irma_UserData_Updt();
                WriteFlag = 0;
            }
#ifdef IM_DEBUG
            if (--cnt == 0)
            {
                cnt = 1800;
                printUdata();
            }
#endif
            STATE0 = 0;//更新完毕,清STATE0,等待下一正确帧
            reset_flag = 1;
            //printf("Have Update user data.\n");
            return IM_OK;
        }
        break;//一次接收数据错误，直接返回错误，但不能将用户数据清零
    case IM_OFF:    //通讯失败超时（由用户通过setoffonline通知）
    case IM_INIT:   //初始化状态(用户数据清零;保持通讯超时标志;)
        //进入初始化状态的条件：开机重启
        if (reset_flag)
        {
            Irma_UserData_Init();
            reset_flag = 0;
        }
        break;
    default:        //状态错误,不作任何处理
        break;
    }
    return IM_ERR;
}
//函数功能：根据输入氧浓度值求出氧补偿值
//输入：氧浓度值
//输出：氧补偿值
int ConvO2ConcToComp(int conc)
{
    if (conc == 255||conc == 254)//若是氧传感器打开或关闭的命令参数，则直接返回。
    {
        return conc;
    }
    //根据phasein主流模块开发手册，将氧浓度值划分为3个区间，每个区间一个补偿值
    //70-100补85，30-70补50，0-30补21
    if (conc > 70)//超过100（理论上不会）也补85
    {
        conc = 85;
    }
    else if (conc > 30)
    {
        conc = 50;
    }
    else
    {
        conc = 21;
    }
    return conc;
}
//函数功能：根据输入笑气浓度值求出笑气补偿值
//输入：氧浓度值
//输出：氧补偿值
static unsigned char ConvN2OConcToComp(unsigned char conc)
{
    //根据phasein主流模块开发手册，将笑气浓度值划分为2个区间，每个区间统一补偿值
    //30-70补50，0-30补0
    if (conc > 30)//大于70时也补50
    {
        conc = 50;
    }
    else
    {
        conc = 0;
    }
    return conc;
}
//函数功能：创建发送帧
//输入：cmd   - 要发送的指令   para - 指令所带参数
//      ptx_frame  用户发送帧的首地址
//输出：通过ptx_frame返回组织好的发送帧
void Irma_CreateTxFrame(enum IM_TX_CMD cmd, unsigned char para, struct IRMA_TX_FRAME *ptx_frame)
{
    if (cmd == IM_SET_O2VOL)//氧补偿命令，计算氧补偿值
    {
        para = ConvO2ConcToComp(para);
    }
    else if (cmd == IM_SET_N2OVOL)//笑气补偿命令，计算笑气补偿值
    {
        para = ConvN2OConcToComp(para);
    }
    ptx_frame->flag1 = IRMA_DATA_FLAG1;//0xAA
    ptx_frame->flag2 = IRMA_DATA_FLAG2;//0x55
    ptx_frame->id = cmd;
    ptx_frame->para = para;
    ptx_frame->chk_sum = ~(cmd+para)+1;
}
//函数功能：创建零点标定的命令帧
//输入：ptx_frame  用户提供的零点标定命令帧的首地址
//输出：通过ptx_frame返回组织好的零点标定命令帧
void Irma_CreateNormZeroCalFrame(struct IRMA_TX_FRAME *ptx_frame)
{
    Irma_CreateTxFrame(IM_ZERO_CAL, IM_CAL_NORM, ptx_frame);
}
//函数功能：仅针对旁流模块，创建使模块进入定点标定状态的命令帧（Pre span calibration zeroing）
//输入：ptx_frame  用户提供的命令帧的首地址
//输出：通过ptx_frame返回组织好的命令帧
void Irma_CreatePreSpanCalZeroFrame(struct IRMA_TX_FRAME *ptx_frame)
{
    Irma_CreateTxFrame(IM_ZERO_CAL, IM_CAL_SPAN, ptx_frame);
}
//【用户接口】
/******************************************************************************
  名称：获取CO2模块打开关闭状态
  入口：无
  返回：打开或关闭状态(enum IM_FUNC_STS) [IM_ON/IM_OFF/IM_INIT]
  调用：用户界面CO2模块打开/关闭设置按键
******************************************************************************/
static enum IM_FUNC_STS  Irma_getOnOff(void)
{
    return(Irma_OnOff);
}
/******************************************************************************
  名称：设置CO2模块打开关闭状态
  入口：onoff - 要设置的状态(enum IM_FUNC_STS) [IM_ON/IM_OFF/IM_INIT]
  返回：设置结果(enum IM_FUNC_STS)[IM_OK/IM_ERR]
  调用：用户界面CO2模块打开/关闭设置按键
  【注意：此函数不要放在任何循环中!!】
******************************************************************************/
static enum IM_FUNC_STS Irma_setOnOff(enum IM_FUNC_STS onoff)
{
    switch(onoff)
    {
    case IM_OFF:
    case IM_INIT:
        STATE0 = 0;
        STATE1 = 0;
    case IM_ON:
        Irma_OnOff = onoff;  //【设置状态到onoff】
        return(IM_OK);
    default:
        return(IM_ERR);
    }
}
//函数功能：当用户检测到通讯超时，设置模块为通讯失败状态
//输入：无
//输出：无
void Irma_setOffline(void)
{
    Irma_OnOff = IM_OFF;
    COMM_TMOUT = 1;
}
/******************************************************************************
  名称：获取CO2模块通讯超时标志
  入口：无
  返回：真或假状态(enum IM_FUNC_STS) [IM_TRUE/IM_FALSE]
  调用：用户产生"CO2模块通讯失败"报警
******************************************************************************/
enum IM_FUNC_STS  Irma_getCommErrFlag(void)
{
    if(COMM_TMOUT)
    {
        return(IM_TRUE);
    }
    else
    {
        return(IM_FALSE);
    }
}
////////////////////////////////////////////////////////////////////////////////
/******************************************************************************
  名称：获取模块产生的状态位
  入口：status - 指定要获取的状态位，参照enum IM_GEN_STS
  返回：IM_TRUE=1, IM_FALSE=0
  调用：用户状态判断
******************************************************************************/
enum IM_FUNC_STS  Irma_getState(enum IM_GEN_STS status)
{
    unsigned char s = pAll_Data->stat.sbyte[3-((status>>3)&0x3)]; //[3-status/8]
    status %= 8;
    if(s&(1<<status))
    {
        return(IM_TRUE);
    }
    else
    {
        return(IM_FALSE);
    }
}
/******************************************************************************
  名称：获取CO2模块产生的"波形数据"
  入口：Para  - 指定要获取的值(气体波形),参见enum IM_GAS_DATA
  返回：获取值
  调用：用户显示
******************************************************************************/
unsigned int Im_getWaveData(enum IM_GAS_DATA Para)
{
    unsigned int waveData;
    //气体波形数据精度: 0.01% 2B无符号整型
    switch(Para)
    {
    case IM_GAS_CO2:  //wv_val[0]
    case IM_GAS_N2O:  //wv_val[1]
    case IM_GAS_AX1:  //wv_val[2]
    case IM_GAS_AX2:  //wv_val[3]
    case IM_GAS_O2:   //wv_val[4]
        waveData = pAll_Data->fast.wv_val[Para];
        return(waveData);
    default:
        return(0);
    }
}
//函数功能：供用户获取波形数据
//输入：gastype--气体类型
//      returnval--要获取的数值的地址，需用户传入地址。
//输出：IM_OK--获取成功    IM-BUSY--正在更新用户区数据
enum IM_FUNC_STS Irma_getWaveData(enum IM_GAS_DATA gastype, unsigned int *returnval)
{
    if (WriteFlag)
    {
        return IM_BUSY; //正在更新用户数据，返回忙标志
    }
    else
    {
        ++ReadFlag;                          //读标志计数，该值反映正在读用户数据的任务数
        *returnval = Im_getWaveData(gastype);//获取波形数据
        --ReadFlag;
        return IM_OK;
    }
}
enum IM_FUNC_STS Irma_getWaveColor(enum IM_GAS_DATA gastype, unsigned int *returnval)
{
}
/******************************************************************************
  名称：获取CO2模块产生的"慢速数据"
  入口：FirstPara  - 指定要获取的值,参见enum IM_GET_DATA
        SecondPara - 气体吸呼浓度需要指定第2参数，气体类型. 不用时置0
  返回：获取值
  调用：用户显示
******************************************************************************/
//转静态并更改返回值
int Im_getSlowData(enum IM_GET_DATA FirstPara, unsigned char SecondPara)
{
    unsigned int tmp;//改为16位变量
    switch(FirstPara)
    {
    case IM_GET_FRM_ID:
        return(pAll_Data->fast.frm_id);
    case IM_GET_SUM_STS:
        return(pAll_Data->fast.sum_stat);
        //气体吸呼浓度,精度:0.1%或1% 1B无符号字节型
    case IM_GET_INSP:
    case IM_GET_EXP:
    case IM_GET_MOMT:
        switch(SecondPara)
        {
        case IM_GAS_CO2:  //sl_val[0]
        default:
            SecondPara = IM_GAS_CO2;
            break;
        case IM_GAS_N2O:  //sl_val[1]
        case IM_GAS_AX1:  //sl_val[2]
        case IM_GAS_AX2:  //sl_val[3]
        case IM_GAS_O2:   //sl_val[4]
            break;
        }
        if(FirstPara == IM_GET_INSP)
        {
            return(pAll_Data->slow.InspVal[SecondPara]);
        }
        if(FirstPara == IM_GET_EXP)
        {
            return(pAll_Data->slow.ExpVal[SecondPara]);
        }
        if(FirstPara == IM_GET_MOMT)
        {
            return(pAll_Data->slow.MomtVal[SecondPara]);
        }
        //RID=0x03  GenVals
    case IM_GET_BRE_RATE:         //获取呼吸频率[bpm]
        return(pAll_Data->slow.GenVal.bre_rate);
    case IM_GET_LASTBRE_TM:       //获取距上次呼吸时间[255s MAX]
        return(pAll_Data->slow.GenVal.lstbre_tm);
    case IM_GET_AX1:              //获取主麻醉气ID
        return(pAll_Data->slow.GenVal.ax1_id);
    case IM_GET_AX2:              //获取从麻醉气ID
        return(pAll_Data->slow.GenVal.ax2_id);
    case IM_GET_ATMP:             //获取大气压力值(2B)
        return(pAll_Data->slow.GenVal.atmp);
        //RID=0x04  SensorRegs
    case IM_GET_MODE:             //获取模块状态
        return(pAll_Data->slow.GenVal.sen_mode);
    case IM_GET_O2_CMPEN:         //获取O2补偿浓度
        return(pAll_Data->slow.GenVal.o2_compn);
    case IM_GET_NOBRE_TMOUT:      //获取无呼吸超时时间[20~60s]
        return(pAll_Data->slow.GenVal.nobre_tmout);
        //RID=0x05  SensorInfo 支持旁流模块(仍5种,返3类:ISA_CO2/ISA_AX/ISA_AX+)
    case IM_GET_SEN_TYPE:         //获取模块类型[CO2 Only/ICU/AX/OR/AX+/OR+]
        tmp = pAll_Data->slow.VerVal.sen_type;
        SecondPara = (char)(tmp >> 8) & 0x01;
        FirstPara  = (unsigned char)tmp;
        tmp = tmp & 0x8000;//取最高位:0=主流/1=旁流
        if(!tmp) //【主流类型(tmp=0)】
        {
            if(SecondPara) //AgentID option fitted
            {
                //【主流红色模块(AX+/OR+)】
                if((FirstPara&0xF8) == 0xF8) //麻醉气位有0的, 将返回UNKNOW
                {
                    if(FirstPara&0x1) //O2 option fitted
                    {
                        return(IM_SENTYPE_ORAUTO);
                    }
                    else              //No O2
                    {
                        return(IM_SENTYPE_AXAUTO);
                    }
                }
            }
            else //【主流蓝色AX/OR 或 黄色模块CO2/ICU】
            {
                //是否正确读出类型值?  2009-3-10 Modified
                //(检测CO2_CFG位是否为1,1=正常,因所有IRMA必有CO2监测,0时返回UNKOWN)
                if(FirstPara&0x2)
                {
                    switch(FirstPara&0xFC) //default=UNKNOW
                    {
                    case 0x0: //无麻醉气监测(CO2/ICU)
                        if(FirstPara&0x1) //ICU (黄)(带O2)
                        {
                            return(IM_SENTYPE_ICU);
                        }
                        else              //CO2 Only (黄)(不带O2)
                        {
                            return(IM_SENTYPE_CO2ONLY);
                        }
                    case 0xFC://有麻醉气监测(AX/OR)
                        if(FirstPara&0x1) //OR  (蓝)(带O2)
                        {
                            return(IM_SENTYPE_OR);
                        }
                        else              //AX  (蓝)(不带O2)
                        {
                            return(IM_SENTYPE_AX);
                        }
                    }
                }
            }
        }
        else //【旁流类型(tmp=1)】
        {
            if(SecondPara) //AgentID option fitted
            {
                //【旁流红色模块(AX+/OR+)】
                if((FirstPara&0xF8) == 0xF8) //麻醉气位有0的, 将返回UNKNOW
                {
                    return(IM_SENTYPE_ISA_AXAUTO);     //[0x13]
                }
            }
            else//【蓝色AX/OR 或 黄色模块CO2/ICU】
            {
                if(FirstPara&0x2)
                {
                    switch(FirstPara&0xFC)//default=UNKNOW
                    {
                    case 0x0: //无麻醉气监测(黄色 CO2/ICU)
                        return(IM_SENTYPE_ISA_CO2);//[0x11]
                    case 0xFC://有麻醉气监测(蓝色 AX/OR)
                        return(IM_SENTYPE_ISA_AX); //[0x12]
                    }
                }
            }
        }
        return(IM_SENTYPE_UNKNOW); //以上都没返回, 返回UNKNOW
    case IM_GET_N2O_CMPEN:        //N2O补偿浓度
        return(pAll_Data->slow.GenVal.n2o_compn);
    case IM_GET_ATMP_CUVE:        //大气压-管道压+20的值[-2.0~23.4kPa 0=-2.0,254=23.4]
//            return(pAll_Data->slow.VerVal.rid6_byte5);
        return(pAll_Data->slow.GenVal.atmp_cuve);
    case IM_GET_IR_O2_DLY:        //RID6_Byte2_b4~b7 红外检测到O2检测间隔[0~15]【sidestream only】
        return(pAll_Data->slow.GenVal.ir_o2_dly);
    default:
        return(0);
    }
}
//函数功能：供用户获取慢速数据
//输入：command--要获取哪种数据， gastype--气体类型，仅用于获取气体浓度时
//      returnval--要获取的数值的地址，需用户传入地址。
//输出：IM_OK--获取成功    IM-BUSY--正在更新用户区数据
enum IM_FUNC_STS Irma_getSlowData(enum IM_GET_DATA command, unsigned char gastype, int * returnval)
{
    if (WriteFlag)
    {
        return IM_BUSY;  //正在更新用户数据，返回忙标志
    }
    else
    {
        ++ReadFlag;         //读标志计数，该值反映正在读用户数据的任务数
        *returnval = Im_getSlowData(command, gastype);
        --ReadFlag;
        return IM_OK;
    }
}
//函数功能：获取版本信息
//输入：pSN          - 用户指定存放SN串号(24b)变量的指针
//      pSW - 用户指定存放软件版本(16b)变量的指针
//      pHW - 用户指定存放硬件版本(8b)变量的指针
//      pCP - 用户指定存放通讯版本(8b)变量的指针
//      所有数据为BCD码: 如:0x1234=>Ver:1.2.3.4
//返回：函数返回2个状态值: IM_OK(获取正确),IM_BUSY(正在更新用户区数据)
enum IM_FUNC_STS  Irma_getVersion(long *pSN,          unsigned int  *pSW,
                                  unsigned int *pHW,  unsigned int  *pCP)
{
    if (WriteFlag)
    {
        return IM_BUSY;
    }
    else
    {
        ++ReadFlag;
        *pCP = pAll_Data->slow.VerVal.commp_ver;//8b
        *pHW = pAll_Data->slow.VerVal.hw_ver;   //8b
        *pSW = pAll_Data->slow.VerVal.sw_ver;   //16b
        *pSN = pAll_Data->slow.VerVal.serial_num & 0xFFFFFFL;//24b
        --ReadFlag;
    }
    return(IM_OK);
}
//函数功能：phasein技术报警判断函数
void Module_Alarm_Judge(void)  //sun add 20140821
{
#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45C) 
    if(pAll_Data->slow.GenVal.sw_err!=0)
    {
        if(!(AlarmsWork.presentAlarms & ALARM_PHASEIN_SW_ERR))
        {
            AlarmsWork.newAlarms |= ALARM_PHASEIN_SW_ERR;  
        }
    }
    else
    {
        AlarmsWork.presentAlarms &= ~ALARM_PHASEIN_SW_ERR;
    }
    if(pAll_Data->slow.GenVal.hW_err!=0)
    {
        if(!(AlarmsWork.presentAlarms & ALARM_PHASEIN_HW_ERR))
        {              
            AlarmsWork.newAlarms |= ALARM_PHASEIN_HW_ERR;
        }
    }
    else
    {
        AlarmsWork.presentAlarms &= ~ALARM_PHASEIN_HW_ERR;
    }
    if(pAll_Data->slow.GenVal.m_fail!=0)
    {
        if(!(AlarmsWork.presentAlarms & ALARM_PHASEIN_MOTOR_ERR))
        {
            AlarmsWork.newAlarms |= ALARM_PHASEIN_MOTOR_ERR;
        }
    }
    else
    {
        AlarmsWork.presentAlarms &= ~ALARM_PHASEIN_MOTOR_ERR;
    }
    if(pAll_Data->slow.GenVal.un_cal!=0)
    {
        if(!(AlarmsWork.presentAlarms & ALARM_PHASEIN_UN_CAL))
        {
            AlarmsWork.newAlarms |= ALARM_PHASEIN_UN_CAL;
        }
    }
    else
    {
        AlarmsWork.presentAlarms &= ~ALARM_PHASEIN_UN_CAL;
    }
    if(pAll_Data->slow.GenVal.repl_adapt!=0)
    {
        if(!(AlarmsWork.presentAlarms & ALARM_GAS_REPL_ADP))
        {
            AlarmsWork.newAlarms |= ALARM_GAS_REPL_ADP;
        }
    }
    else
    {
        AlarmsWork.presentAlarms &= ~ALARM_GAS_REPL_ADP;
    }
    if(pAll_Data->slow.GenVal.no_adapt!=0)
    {
        if(!(AlarmsWork.presentAlarms & ALARM_PHASEIN_NO_ADP))
        {
            AlarmsWork.newAlarms |= ALARM_PHASEIN_NO_ADP;
        }
    }
    else
    {
        AlarmsWork.presentAlarms &= ~ALARM_PHASEIN_NO_ADP;
    }
    if(pAll_Data->slow.GenVal.co2_or!=0)
    {
        if(!(AlarmsWork.presentAlarms & ALARM_PHASEIN_CO2_OR))
        {
            AlarmsWork.newAlarms |= ALARM_PHASEIN_CO2_OR;
        }
    }
    else
    {
        AlarmsWork.presentAlarms &= ~ALARM_PHASEIN_CO2_OR;
    }
    if(pAll_Data->slow.GenVal.n2o_or!=0)
    {
        if(!(AlarmsWork.presentAlarms & ALARM_PHASEIN_N2O_OR))
        {
            AlarmsWork.newAlarms |= ALARM_PHASEIN_N2O_OR;
        }
    }
    else
    {
        AlarmsWork.presentAlarms &= ~ALARM_PHASEIN_N2O_OR;
    }
    if(pAll_Data->slow.GenVal.ax_or!=0)
    {
        if(!(AlarmsWork.presentAlarms & ALARM_PHASEIN_AX_OR))
        {                
            AlarmsWork.newAlarms |= ALARM_PHASEIN_AX_OR;
        }
    }
    else
    {
        AlarmsWork.presentAlarms &= ~ALARM_PHASEIN_AX_OR;
    }
    if(pAll_Data->slow.GenVal.temp_or!=0)
    {
        if(!(AlarmsWork.presentAlarms & ALARM_PHASEIN_TEMP_OR))
        { 
            AlarmsWork.newAlarms |= ALARM_PHASEIN_TEMP_OR;
        }
    }
    else
    {
        AlarmsWork.presentAlarms &= ~ALARM_PHASEIN_TEMP_OR;
    }
    if(pAll_Data->slow.GenVal.press_or!=0)
    {
        if(!(AlarmsWork.presentAlarms & ALARM_PHASEIN_PRESS_OR))
        { 
            AlarmsWork.newAlarms |= ALARM_PHASEIN_PRESS_OR;
        }
    }
    else
    {
        AlarmsWork.presentAlarms &= ~ALARM_PHASEIN_PRESS_OR;
    }
    if(pAll_Data->slow.GenVal.zero_or!=0)
    {
        if(!(AlarmsWork.presentAlarms & ALARM_PHASEIN_ZERO_OR))
        {
            AlarmsWork.newAlarms |= ALARM_PHASEIN_ZERO_OR;
        }
    }
    else
    {
        AlarmsWork.presentAlarms &= ~ALARM_PHASEIN_ZERO_OR;
    }
    if(pAll_Data->slow.GenVal.agen_id_unreliable!=0)
    {
        if(!(AlarmsWork.presentAlarms & ALARM_PHASEIN_AGENTID_UNREAL))
        { 
            AlarmsWork.newAlarms |= ALARM_PHASEIN_AGENTID_UNREAL;
        }
    }
    else
    {
        AlarmsWork.presentAlarms &= ~ALARM_PHASEIN_AGENTID_UNREAL;
    }            
#endif
}
////////////////////////////////////////////////////////////////////////////////
