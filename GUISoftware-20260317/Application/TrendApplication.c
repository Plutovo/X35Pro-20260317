/*****************************************************
*     趋势移植需要修改的地方
*    1、修改参数最大索引值TREND_PARA_NUM
*    2、修改参数范围ParaMaxLimit[]和ParaMinLimit，参数名ParaNameIndex
*    3、TrendGraph_Init()趋势图变量的初始化，分配每个趋势图中包含的变量个数,配置趋势显示的颜色。
*  * 4、Get_TrendDate_1s()从系统的监测参数中定时取出趋势所需数据，保存到链表中，可以定制无效数据的类型,(重要)
*    5、InitTrendName()初始化趋势参数的显示名称
*    6、KeyBoard、queue_manul组件，以及KeyBoard控件的大小、按钮的多少....
*    7、按钮样式、窗体边框(Window控件重回中加入边框绘制的判断，并且在趋势的回调函数中设置Window背景色为GUI_BLACK+1)、
麻醉气体和其他特殊功能
*    8、数据包的大小TREND_PACKETSIZE = 1*单字节变量数 + 2*双字节变量数
*    9、趋势数据存储在EEPROM中的起始位置TREND_EEPROMADDR
* 趋势模块版本信息 V_20141129_01
******************************************************
*/
#include <stdlib.h>
#include "GUI.h"
#include "queue_manul.h"
#include "keyboard.h"
#include "time.h"
#include "TrendTable.h"
#include "JM1_SpO2.h"
#include "C002_CO2.h"

#include "co2_irma.h"

extern AA_PARAMETERS AA_Para;
unsigned char Para_10M_NameIndex[PARA_10M_NUM] = {
        PPEAK,PMEAN,PEEP,MV,MVSPN,
};

//BDU发送的检测值为实际值的100倍的参数集合
//#define PARA_100M_NUM   1         //Multiple
//unsigned char Para_100M_NameIndex[PARA_100M_NUM] = {};

//#if (MACHINE_TYPE == X45 || MACHINE_TYPE == X45A || MACHINE_TYPE == X45C)
#ifdef SPO2_SWITCH

//参数索引值集合
unsigned char ParaNameIndex[TREND_PARA_NUM] = {
        PPEAK,PMEAN,PEEP,FTOTAL,FSPN,FIO2,ETCO2,FICO2,ETN2O,FIN2O,SPO2,PULSE,
        VTE,VTI,RST,CDYN,MV,MVSPN,ETAA1,FIAA1,ETAA2,FIAA2,MAC_,
};
//参数最大限制值，与参数的索引值相对应
unsigned short ParaMaxLimit[TREND_PARA_NUM] = {
    100,100,100,120,120,100,150,150,100,100,100,250,
    2500,2500,600,300,990,990,199,199,199,199,5000,
};
//参数最小限制值，与参数的索引值相对应
unsigned short ParaMinLimit[TREND_PARA_NUM] = {
    0,  0,  0,  0,  0,  21, 0,  0,  0,  0,  0,  30, 
    0,   0,   0,   0,   0,  0,  0,  0,  0,  0,  0,
};
//参数小数点位数
const unsigned short ParaFloatLimit[TREND_PARA_NUM] = {
    0,  0,  0,  0,  0,  0,  1,  0,0,0,  0,   0, 
    0,  0,  0,  0,  1,  1,  0,0,0,0,0,
};

#else

//参数索引值集合
unsigned char ParaNameIndex[TREND_PARA_NUM] = {
        PPEAK, PMEAN, PEEP, FTOTAL, FSPN, FIO2, ETCO2, FICO2,ETN2O,FIN2O,
        VTE, VTI, RST, CDYN ,MV, MVSPN,ETAA1,FIAA1,ETAA2,FIAA2,MAC_,
};
//参数最大限制值，与参数的索引值相对应
unsigned short ParaMaxLimit[TREND_PARA_NUM] = {
    100, 100, 100, 120, 120, 100, 150, 150,100,100,
    2500, 2500, 600, 300, 990, 990, 199,199,199,199,5000,
};
//参数最小限制值，与参数的索引值相对应
unsigned short ParaMinLimit[TREND_PARA_NUM] = {
    0, 0, 0, 0, 0, 21, 0,  0,  0,  0,
    0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  0,
};
//参数小数点位数
const unsigned short ParaFloatLimit[TREND_PARA_NUM] = {
    0, 0, 0, 0, 0, 0, 1, 0,0,0,
    0, 0, 0, 0, 1, 1, 0,0,0,0,0,
};
#endif

const char *TrendParaName[TREND_PARA_NUM]={0};

const unsigned short MonitorName1_AA[12] = {
	DP_ETAA1,    DP_FIAA1,    DP_ETHAL,     DP_FIHAL,     DP_ETENF,     DP_FIENF,
	DP_ETISO,    DP_FIISO,    DP_ETSEV,     DP_FISEV,     DP_ETDES,     DP_FIDES,
};
const unsigned short MonitorName2_AA[12] = {
	DP_ETAA2,    DP_FIAA2,    DP_ETHAL,     DP_FIHAL,     DP_ETENF,     DP_FIENF,
	DP_ETISO,    DP_FIISO,    DP_ETSEV,     DP_FISEV,     DP_ETDES,     DP_FIDES,
};

/*
*定义在SDRAM中的队列名称。用于保存EEPROM的缓存数据
*/
Queue Tmp_Trend_1s;
Queue Tmp_Trend_1h;
Queue Tmp_Trend_4h;
Queue Tmp_Trend_12h;
Queue Tmp_Trend_24h;
#if (TREND_INFO_72H)
Queue Tmp_Trend_48h;
Queue Tmp_Trend_72h;
#endif

//趋势图数据在SDRAM中存储的起始地址
#define SDRAMCACHE_BASEADDR (0xA0600000)

//趋势图趋势表的buffer大小
#define TREND_BUFFERSIZE 360

//一个数据包存的大小，计算方法：所有变量字节数之和
#define TREND_PACKETSIZE     (34)//1*10 + 2*7 = 24 byte //ylk modify 20181123 (1*12 + 2*12)=34 
//EEPROM 存储结构  //趋势数据大小 0x02000~0x09FFF 32K
#define TREND_EEPROMADDR     (0x2000)//64k+10k 开始 //ylk modify 20181219 2800->2000

//各个阶段数据存储的位置
#define TREND_PIXEL_1h       (360)
#define TREND_PIXEL_4h       (270)
#define TREND_PIXEL_12h      (240)
#define TREND_PIXEL_24h      (180)
#if (TREND_INFO_72H)
#define TREND_PIXEL_48h      (180)
#define TREND_PIXEL_72h      (120)
#endif

#define TREND_SAVEADDR_1h    (TREND_EEPROMADDR) //()
#define TREND_SAVEADDR_4h    (TREND_SAVEADDR_1h + TREND_PIXEL_1h*TREND_PACKETSIZE)
#define TREND_SAVEADDR_12h   (TREND_SAVEADDR_4h + TREND_PIXEL_4h*TREND_PACKETSIZE)
#define TREND_SAVEADDR_24h   (TREND_SAVEADDR_12h + TREND_PIXEL_12h*TREND_PACKETSIZE)
#if (TREND_INFO_72H)
#define TREND_SAVEADDR_48h   (TREND_SAVEADDR_24h + TREND_PIXEL_24h*TREND_PACKETSIZE)
#define TREND_SAVEADDR_72h   (TREND_SAVEADDR_48h + TREND_PIXEL_48h*TREND_PACKETSIZE)
#endif

#define TREND_SAVE_INDEX     (TREND_EEPROMADDR-0x100)//存储EEPROM中每一个时间段的头位置
#define TREND_TIME           (TREND_EEPROMADDR-0x50) //存储趋势图的最新时间点

#if (TREND_INFO_72H)
#define TREND_TIME_NUM 6
#define TREND_MAX_TIME 72
const char *TrendTimeRange[TREND_TIME_NUM]={"1","4","12","24","48","72"};
const char TrendTimeRange_Index[TREND_TIME_NUM]={1,4,12,24,48,72};
#elif (TREND_INFO_24H)
#define TREND_TIME_NUM 4
#define TREND_MAX_TIME 24
const char *TrendTimeRange[TREND_TIME_NUM]={"1","4","12","24"};
const char TrendTimeRange_Index[TREND_TIME_NUM]={1,4,12,24};
#endif

#define TREND_CURVECOLOR_1             (0xFFC81E)
#define TREND_CURVECOLOR_2             (0x6AFF00)

int SecValue = 0,num = 0;
//时间
time_t timer0 = 0,timer1 = 0;
struct tm tblock;

typedef struct //EEPROM对应的存储结构
{
    unsigned short Trend_Pos_1h;  //0~359 下一个数据存储的位置
    unsigned short Trend_Pos_4h;
    unsigned short Trend_Pos_12h;
    unsigned short Trend_Pos_24h;
#if (TREND_INFO_72H)
    unsigned short Trend_Pos_48h;
    unsigned short Trend_Pos_72h;
#endif

    unsigned short Trend_Num_1h;  //0~360   0的时候表示为空 1 ~ 360 表示存储的数量
    unsigned short Trend_Num_4h;
    unsigned short Trend_Num_12h;
    unsigned short Trend_Num_24h;
#if (TREND_INFO_72H)
    unsigned short Trend_Num_48h;
    unsigned short Trend_Num_72h;
#endif
}TREND_EEPROM_S;   
TREND_EEPROM_S Trend_Pos;

typedef union //用于显存的缓存
{
    short tmp_short[TREND_BUFFERSIZE]; 
    char  tmp_char[TREND_BUFFERSIZE] ; 
}Trend_Disp_S;


typedef struct 
{
    TracingCurve_Handle   Graph_Handle[3]; //趋势图句柄    
    unsigned char Graph_Index[3];  //趋势图中每个波形对应的参数
    unsigned char Graph_TimeRange; //趋势图的时间范围
    unsigned char Graph_ChoosePos; //当前选中的趋势图按钮（修改趋势图，有效值：0~2 ）       

    TracingCurve_Handle   Table_Handle; //趋势表句柄    
    unsigned char Table_Index[7];  //趋势表中每隔表格对应的参数
    unsigned char Table_TimeRange; //趋势表的时间范围 
    unsigned char Table_ChoosePos; //当前选中的趋势表按钮（修改趋势表，有效值：0~6 ）       
}TREND_INFO_S;
TREND_INFO_S TrendPara_Info = {
                               0,0,0,//趋势图句柄
                               0,0,0,//每个趋势图对应的参数个数
                               1,    //趋势图初始时间范围
                               0xff, //当前选中的趋势图

                               0,             //趋势表句柄
                               0,0,0,0,0,0,0, //每个趋势表对应的参数个数
                               1,             //趋势表初始时间范围
                               0xff,          //当前选中的趋势表
                               };//趋势图趋势表的参数信息

//参数开关标志
extern unsigned char SpO2ModuleExist;
//extern unsigned char AA_Module_Flag.EtCO2ModuleExist;
extern unsigned char O2ModuleOff;   //djq add 20140213
extern unsigned char SpO2ModuleOff;
extern unsigned char EtCO2ModuleOff; 

/*
*  变量声明
*/
extern MENU_HWIN Menu_hWin;
extern int PlanMenuType;
extern int Task;
extern int StandbyState;
extern RTC_TIMER RTCTime;
extern int ETCO2Unit;//0:%     1:mmHg      2:kPa
//按钮上的箭头
extern GUI_CONST_STORAGE GUI_BITMAP bmblack_up;//
extern GUI_CONST_STORAGE GUI_BITMAP bmblack_down;//
extern GUI_CONST_STORAGE GUI_BITMAP bmwhite_up;//
extern GUI_CONST_STORAGE GUI_BITMAP bmwhite_down;//


extern C002_CO2_PARAMETERS CO2_Parameters;
extern JM1_SPO2_PARAMETERS SpO2_Parameters;
extern BREATH_SET_PARA BeathSettingPara;


/*
*  函数声明
*/
static void InsertEmpty_ToList(void);
static void __GetItemOfEEPROM(Item *tmp);
static void __SaveItemToEEPROM(Item tmp);
void InitTrendName(void);
void __EmptyDispQueue(void);


/* 
* 初始化EEPROM存储计数器，头结点的位置
*/
RTC_TIMER TrendSaveDate_Time = {0,0,0,0,0,0};

static void _InitTrendEEPROM_Time(RTC_TIMER *TrendTime)
{
    TrendTime->RTC_Year  = (RTEEPROMread(TREND_TIME)<<8)|RTEEPROMread(TREND_TIME+1);
    TrendTime->RTC_Mon   = RTEEPROMread(TREND_TIME+2);
    TrendTime->RTC_Mday  = RTEEPROMread(TREND_TIME+3);
    TrendTime->RTC_Hour  = RTEEPROMread(TREND_TIME+4);
    TrendTime->RTC_Min   = RTEEPROMread(TREND_TIME+5);
    TrendTime->RTC_Sec   = RTEEPROMread(TREND_TIME+6);    
}

//读取EEPROM中趋势数据的位置和数量信息
static void _InitTrendEEPROM_S(TREND_EEPROM_S *Trend_Pos)//备注：因为结构体在存储上是连续的，所以可以使用指针缩写！！
                                                         //例如：*(INT16U *)((INT16U *)(Trend_Pos)+sizeof(INT16U)*2) = &Trend_Pos->Trend_Num_12h
{
    Trend_Pos->Trend_Num_1h  = (RTEEPROMread(TREND_SAVE_INDEX + 10)<<8) | RTEEPROMread(TREND_SAVE_INDEX + 11);
    if(Trend_Pos->Trend_Num_1h > TREND_PIXEL_1h)
    {
        Trend_Pos->Trend_Num_1h = TREND_PIXEL_1h;
    }
    Trend_Pos->Trend_Num_4h  = (Trend_Pos->Trend_Num_1h != TREND_PIXEL_1h) ? 0 : (RTEEPROMread(TREND_SAVE_INDEX + 12)<<8) | RTEEPROMread(TREND_SAVE_INDEX + 13);        
    if(Trend_Pos->Trend_Num_4h > TREND_PIXEL_4h)
    {
        Trend_Pos->Trend_Num_4h = TREND_PIXEL_4h;
    }
    Trend_Pos->Trend_Num_12h = (Trend_Pos->Trend_Num_4h != TREND_PIXEL_4h) ? 0 : RTEEPROMread(TREND_SAVE_INDEX + 14);
    if(Trend_Pos->Trend_Num_12h > TREND_PIXEL_12h)
    {
        Trend_Pos->Trend_Num_12h = TREND_PIXEL_12h;
    }
    Trend_Pos->Trend_Num_24h = (Trend_Pos->Trend_Num_12h != TREND_PIXEL_12h) ? 0 : RTEEPROMread(TREND_SAVE_INDEX + 15);
    if(Trend_Pos->Trend_Num_24h > TREND_PIXEL_24h)
    {
        Trend_Pos->Trend_Num_24h = TREND_PIXEL_24h;
    }
#if (TREND_INFO_72H)
    Trend_Pos->Trend_Num_48h = (Trend_Pos->Trend_Num_24h != TREND_PIXEL_24h) ? 0 : RTEEPROMread(TREND_SAVE_INDEX + 16);
    if(Trend_Pos->Trend_Num_48h > TREND_PIXEL_48h)
    {
        Trend_Pos->Trend_Num_48h = TREND_PIXEL_48h;
    }
    Trend_Pos->Trend_Num_72h = (Trend_Pos->Trend_Num_48h != TREND_PIXEL_48h) ? 0 : RTEEPROMread(TREND_SAVE_INDEX + 17);
    if(Trend_Pos->Trend_Num_72h > TREND_PIXEL_72h)
    {
        Trend_Pos->Trend_Num_72h = TREND_PIXEL_72h;
    }
#endif

    Trend_Pos->Trend_Pos_1h  = (!Trend_Pos->Trend_Num_1h) ? 0 : (RTEEPROMread(TREND_SAVE_INDEX)<<8)     | RTEEPROMread(TREND_SAVE_INDEX + 1);
    if(Trend_Pos->Trend_Pos_1h >= TREND_PIXEL_1h)
    {
        Trend_Pos->Trend_Pos_1h = (Trend_Pos->Trend_Num_1h == TREND_PIXEL_1h)?0:Trend_Pos->Trend_Num_1h;
    }
    Trend_Pos->Trend_Pos_4h  = (!Trend_Pos->Trend_Num_4h) ? 0 : (RTEEPROMread(TREND_SAVE_INDEX + 2)<<8) | RTEEPROMread(TREND_SAVE_INDEX + 3);
    if(Trend_Pos->Trend_Pos_4h > TREND_PIXEL_4h)
    {
        Trend_Pos->Trend_Pos_4h = (Trend_Pos->Trend_Num_4h == TREND_PIXEL_4h)?0:Trend_Pos->Trend_Num_4h;
    }
    Trend_Pos->Trend_Pos_12h = (!Trend_Pos->Trend_Num_12h) ? 0 : RTEEPROMread(TREND_SAVE_INDEX + 4);
    if(Trend_Pos->Trend_Pos_12h > TREND_PIXEL_12h)
    {
        Trend_Pos->Trend_Pos_12h = (Trend_Pos->Trend_Num_12h == TREND_PIXEL_12h)?0:Trend_Pos->Trend_Num_12h;
    }
    Trend_Pos->Trend_Pos_24h = (!Trend_Pos->Trend_Num_24h) ? 0 : RTEEPROMread(TREND_SAVE_INDEX + 5);
    if(Trend_Pos->Trend_Pos_24h > TREND_PIXEL_24h)
    {
        Trend_Pos->Trend_Pos_24h = (Trend_Pos->Trend_Num_24h == TREND_PIXEL_24h)?0:Trend_Pos->Trend_Num_24h;
    }
#if (TREND_INFO_72H)
    Trend_Pos->Trend_Pos_48h = (!Trend_Pos->Trend_Num_48h) ? 0 : RTEEPROMread(TREND_SAVE_INDEX + 6);
    if(Trend_Pos->Trend_Pos_48h > TREND_PIXEL_48h)
    {
        Trend_Pos->Trend_Pos_48h = (Trend_Pos->Trend_Num_48h == TREND_PIXEL_48h)?0:Trend_Pos->Trend_Num_48h;
    }
    Trend_Pos->Trend_Pos_72h = (!Trend_Pos->Trend_Num_72h) ? 0 : RTEEPROMread(TREND_SAVE_INDEX + 7);
    if(Trend_Pos->Trend_Pos_72h > TREND_PIXEL_72h)
    {
        Trend_Pos->Trend_Pos_72h = (Trend_Pos->Trend_Num_72h == TREND_PIXEL_72h)?0:Trend_Pos->Trend_Num_72h;
    }
#endif
}


/*
*  更换病人的时候从新初始化趋势数据信息
*/
static void __SetTrend_PosAndTime(TREND_EEPROM_S *Trend_Pos,RTC_TIMER *TrendTime)
{
    RTEEPROMwrite(TREND_SAVE_INDEX    ,(Trend_Pos->Trend_Pos_1h & 0xff00)>>8);
    RTEEPROMwrite(TREND_SAVE_INDEX + 1, Trend_Pos->Trend_Pos_1h & 0xff);
    RTEEPROMwrite(TREND_SAVE_INDEX + 2,(Trend_Pos->Trend_Pos_4h & 0xff00)>>8);
    RTEEPROMwrite(TREND_SAVE_INDEX + 3, Trend_Pos->Trend_Pos_4h & 0xff); 
    
    RTEEPROMwrite(TREND_SAVE_INDEX + 4, Trend_Pos->Trend_Pos_12h & 0xff);        
    RTEEPROMwrite(TREND_SAVE_INDEX + 5, Trend_Pos->Trend_Pos_24h & 0xff);        
#if (TREND_INFO_72H)
    RTEEPROMwrite(TREND_SAVE_INDEX + 6, Trend_Pos->Trend_Pos_48h & 0xff);        
    RTEEPROMwrite(TREND_SAVE_INDEX + 7, Trend_Pos->Trend_Pos_72h & 0xff);        
#endif
    RTEEPROMwrite(TREND_SAVE_INDEX +10,(Trend_Pos->Trend_Num_1h & 0xff00)>>8);
    RTEEPROMwrite(TREND_SAVE_INDEX +11, Trend_Pos->Trend_Num_1h & 0xff);
    RTEEPROMwrite(TREND_SAVE_INDEX +12,(Trend_Pos->Trend_Num_4h & 0xff00)>>8);
    RTEEPROMwrite(TREND_SAVE_INDEX +13, Trend_Pos->Trend_Num_4h & 0xff); 
    
    RTEEPROMwrite(TREND_SAVE_INDEX +14, Trend_Pos->Trend_Num_12h & 0xff);        
    RTEEPROMwrite(TREND_SAVE_INDEX +15, Trend_Pos->Trend_Num_24h & 0xff);        
#if (TREND_INFO_72H)
    RTEEPROMwrite(TREND_SAVE_INDEX +16, Trend_Pos->Trend_Num_48h & 0xff);        
    RTEEPROMwrite(TREND_SAVE_INDEX +17, Trend_Pos->Trend_Num_72h & 0xff);        
#endif        
        
    RTEEPROMwrite(TREND_TIME  ,(TrendTime->RTC_Year & 0xff00)>>8);
    RTEEPROMwrite(TREND_TIME+1,TrendTime->RTC_Year & 0xff);
    RTEEPROMwrite(TREND_TIME+2,TrendTime->RTC_Mon);
    RTEEPROMwrite(TREND_TIME+3,TrendTime->RTC_Mday);
    RTEEPROMwrite(TREND_TIME+4,TrendTime->RTC_Hour);
    RTEEPROMwrite(TREND_TIME+5,TrendTime->RTC_Min);
    RTEEPROMwrite(TREND_TIME+6,TrendTime->RTC_Sec);
}


static void _ClearTrendEEPROM_S(TREND_EEPROM_S *Trend_Pos,RTC_TIMER *TrendTime)
{
    //存储最新时间
    *TrendTime  = RTCTime;

    //清空趋势信息
    Trend_Pos->Trend_Pos_1h  = 0;
    Trend_Pos->Trend_Pos_4h  = 0;
    Trend_Pos->Trend_Pos_12h = 0;
    Trend_Pos->Trend_Pos_24h = 0;
#if (TREND_INFO_72H)
    Trend_Pos->Trend_Pos_48h = 0;
    Trend_Pos->Trend_Pos_72h = 0;
#endif
    
    Trend_Pos->Trend_Num_1h  = 0;
    Trend_Pos->Trend_Num_4h  = 0;
    Trend_Pos->Trend_Num_12h = 0;
    Trend_Pos->Trend_Num_24h = 0;
#if (TREND_INFO_72H)
    Trend_Pos->Trend_Num_48h = 0;
    Trend_Pos->Trend_Num_72h = 0;
#endif
    
    //清空趋势图当前显示的信息
    __EmptyDispQueue();

    //清空趋势图在EEPROM中的信息
    __SetTrend_PosAndTime(Trend_Pos,TrendTime);        
}

void ClearTrendInfo(void)
{
    _ClearTrendEEPROM_S(&Trend_Pos,&TrendSaveDate_Time);//EEPROM Position 
}

//存储的数据量过大，可能导致死机
//注:当开启存储数据功能时，来回切换时间量程会导致死机***********************************************
/*
*存储一个数据包到EEPROM中,并且存储每一段数据的首个数据的位置和该段存储的数据的个数
*
*/
void SaveTrendToEEPROM(Item *item,int TimeRange,TREND_EEPROM_S *Trend_Pos)
{
    unsigned int Trend_Addr ;
    unsigned char index = 0,addr_cnt = 0;
    switch(TimeRange)
    {
    case 1:
        Trend_Addr = TREND_SAVEADDR_1h + TREND_PACKETSIZE*Trend_Pos->Trend_Pos_1h;
        Trend_Pos->Trend_Pos_1h++;
        if(Trend_Pos->Trend_Pos_1h >= TREND_PIXEL_1h)//计数器超过了当前的存储范围，跳转到存储首地址
        {
            Trend_Pos->Trend_Pos_1h = 0;    //计数器清空
            if(Trend_Addr >= TREND_SAVEADDR_1h + TREND_PACKETSIZE*TREND_PIXEL_1h)//当前存储地址超出了最大地址,这个条件是预防异常数据的，正常不会到达这里
            {
                Trend_Addr = TREND_SAVEADDR_1h;//跳转到首地址
                Trend_Pos->Trend_Pos_1h++;//计数器递增
            }
        }
				//item->Para[0]=50;
        RTEEPROMwrite(TREND_SAVE_INDEX    ,(Trend_Pos->Trend_Pos_1h & 0xff00)>>8);
        RTEEPROMwrite(TREND_SAVE_INDEX + 1, Trend_Pos->Trend_Pos_1h & 0xff);
        //计算并且存储项目（数据包的个数）
        if(Trend_Pos->Trend_Num_1h >= TREND_PIXEL_1h)
        {
            Trend_Pos->Trend_Num_1h = TREND_PIXEL_1h;  //最大值已经被存储了360 
        }
        else 
        {
            Trend_Pos->Trend_Num_1h++;
            RTEEPROMwrite(TREND_SAVE_INDEX + 10    ,(Trend_Pos->Trend_Num_1h & 0xff00)>>8);
            RTEEPROMwrite(TREND_SAVE_INDEX + 11    , Trend_Pos->Trend_Num_1h & 0xff);
        }

        //存储最新时间
        RTEEPROMwrite(TREND_TIME  ,(item->Time.RTC_Year & 0xff00)>>8);
        RTEEPROMwrite(TREND_TIME+1,item->Time.RTC_Year & 0xff);
        RTEEPROMwrite(TREND_TIME+2,item->Time.RTC_Mon);
        RTEEPROMwrite(TREND_TIME+3,item->Time.RTC_Mday);
        RTEEPROMwrite(TREND_TIME+4,item->Time.RTC_Hour);
        RTEEPROMwrite(TREND_TIME+5,item->Time.RTC_Min);
        RTEEPROMwrite(TREND_TIME+6,item->Time.RTC_Sec);
        break;
    case 4:
        Trend_Addr = TREND_SAVEADDR_4h + TREND_PACKETSIZE*Trend_Pos->Trend_Pos_4h;
        Trend_Pos->Trend_Pos_4h++;
        if(Trend_Pos->Trend_Pos_4h >= TREND_PIXEL_4h)//计数器超过了当前的存储范围，跳转到存储首地址
        {
            Trend_Pos->Trend_Pos_4h = 0;    //计数器清空
            if(Trend_Addr >= TREND_SAVEADDR_4h + TREND_PACKETSIZE*TREND_PIXEL_4h)//当前存储地址超出了最大地址,这个条件是预防异常数据的，正常不会到达这里
            {
                Trend_Addr = TREND_SAVEADDR_4h;//跳转到首地址
                Trend_Pos->Trend_Pos_4h++;//计数器递增
            }
        }
        RTEEPROMwrite(TREND_SAVE_INDEX + 2,(Trend_Pos->Trend_Pos_4h & 0xff00)>>8);
        RTEEPROMwrite(TREND_SAVE_INDEX + 3, Trend_Pos->Trend_Pos_4h & 0xff);        
        //计算并且存储项目（数据包的个数）
        if(Trend_Pos->Trend_Num_4h >= TREND_PIXEL_4h)
        {
            Trend_Pos->Trend_Num_4h = TREND_PIXEL_4h;  //最大值已经被存储了
        }
        else 
        {
            Trend_Pos->Trend_Num_4h++;
            RTEEPROMwrite(TREND_SAVE_INDEX + 12    ,(Trend_Pos->Trend_Num_4h & 0xff00)>>8);
            RTEEPROMwrite(TREND_SAVE_INDEX + 13    , Trend_Pos->Trend_Num_4h & 0xff);
        }
        break;
    case 12:
        Trend_Addr = TREND_SAVEADDR_12h + TREND_PACKETSIZE*Trend_Pos->Trend_Pos_12h;
        Trend_Pos->Trend_Pos_12h++;
        if(Trend_Pos->Trend_Pos_12h >= TREND_PIXEL_12h)//计数器超过了当前的存储范围，跳转到存储首地址
        {
            Trend_Pos->Trend_Pos_12h = 0;    //计数器清空
            if(Trend_Addr >= TREND_SAVEADDR_12h + TREND_PACKETSIZE*TREND_PIXEL_12h)//当前存储地址超出了最大地址,这个条件是预防异常数据的，正常不会到达这里
            {
                Trend_Addr = TREND_SAVEADDR_12h;//跳转到首地址
                Trend_Pos->Trend_Pos_12h++;//计数器递增
            }
        }
        RTEEPROMwrite(TREND_SAVE_INDEX + 4, Trend_Pos->Trend_Pos_12h & 0xff);        
        //计算并且存储项目（数据包的个数）
        if(Trend_Pos->Trend_Num_12h >= TREND_PIXEL_12h)
        {
            Trend_Pos->Trend_Num_12h = TREND_PIXEL_12h;  //最大值已经被存储了
        }
        else 
        {
            Trend_Pos->Trend_Num_12h++;
            RTEEPROMwrite(TREND_SAVE_INDEX + 14    , Trend_Pos->Trend_Num_12h & 0xff);
        }
        break;
    case 24:
        Trend_Addr = TREND_SAVEADDR_24h + TREND_PACKETSIZE*Trend_Pos->Trend_Pos_24h;
        Trend_Pos->Trend_Pos_24h++;
        if(Trend_Pos->Trend_Pos_24h >= TREND_PIXEL_24h)//计数器超过了当前的存储范围，跳转到存储首地址
        {
            Trend_Pos->Trend_Pos_24h = 0;    //计数器清空
            if(Trend_Addr >= TREND_SAVEADDR_24h + TREND_PACKETSIZE*TREND_PIXEL_24h)//当前存储地址超出了最大地址,这个条件是预防异常数据的，正常不会到达这里
            {
                Trend_Addr = TREND_SAVEADDR_24h;//跳转到首地址
                Trend_Pos->Trend_Pos_24h++;//计数器递增
            }
        }
        RTEEPROMwrite(TREND_SAVE_INDEX + 5, Trend_Pos->Trend_Pos_24h & 0xff);        
        //计算并且存储项目（数据包的个数）
        if(Trend_Pos->Trend_Num_24h >= TREND_PIXEL_24h)
        {
            Trend_Pos->Trend_Num_24h = TREND_PIXEL_24h;  //最大值已经被存储了
        }
        else 
        {
            Trend_Pos->Trend_Num_24h++;
            RTEEPROMwrite(TREND_SAVE_INDEX + 15    , Trend_Pos->Trend_Num_24h & 0xff);
        }
        break;
#if (TREND_INFO_72H)
    case 48:
        Trend_Addr = TREND_SAVEADDR_48h + TREND_PACKETSIZE*Trend_Pos->Trend_Pos_48h;
        Trend_Pos->Trend_Pos_48h++;
        if(Trend_Pos->Trend_Pos_48h >= TREND_PIXEL_48h)//计数器超过了当前的存储范围，跳转到存储首地址
        {
            Trend_Pos->Trend_Pos_48h = 0;    //计数器清空
            if(Trend_Addr >= TREND_SAVEADDR_48h + TREND_PACKETSIZE*TREND_PIXEL_48h)//当前存储地址超出了最大地址,这个条件是预防异常数据的，正常不会到达这里
            {
                Trend_Addr = TREND_SAVEADDR_48h;//跳转到首地址
                Trend_Pos->Trend_Pos_48h++;//计数器递增
            }
        }
        RTEEPROMwrite(TREND_SAVE_INDEX + 6, Trend_Pos->Trend_Pos_48h & 0xff);        
        //计算并且存储项目（数据包的个数）
        if(Trend_Pos->Trend_Num_48h >= TREND_PIXEL_48h)
        {
            Trend_Pos->Trend_Num_48h = TREND_PIXEL_48h;  //最大值已经被存储了
        }
        else 
        {
            Trend_Pos->Trend_Num_48h++;
            RTEEPROMwrite(TREND_SAVE_INDEX + 16    , Trend_Pos->Trend_Num_48h & 0xff);
        }
        break;
    case 72:
        Trend_Addr = TREND_SAVEADDR_72h + TREND_PACKETSIZE*Trend_Pos->Trend_Pos_72h;
        Trend_Pos->Trend_Pos_72h++;
        if(Trend_Pos->Trend_Pos_72h >= TREND_PIXEL_72h)//计数器超过了当前的存储范围，跳转到存储首地址
        {
            Trend_Pos->Trend_Pos_72h = 0;    //计数器清空
            if(Trend_Addr >= TREND_SAVEADDR_72h + TREND_PACKETSIZE*TREND_PIXEL_72h)//当前存储地址超出了最大地址,这个条件是预防异常数据的，正常不会到达这里
            {
                Trend_Addr = TREND_SAVEADDR_72h;//跳转到首地址
                Trend_Pos->Trend_Pos_72h++;//计数器递增
            }
        }
        RTEEPROMwrite(TREND_SAVE_INDEX + 7, Trend_Pos->Trend_Pos_72h & 0xff);        
        //计算并且存储项目（数据包的个数）
        if(Trend_Pos->Trend_Num_72h >= TREND_PIXEL_72h)
        {
            Trend_Pos->Trend_Num_72h = TREND_PIXEL_72h;  //最大值已经被存储了
        }
        else 
        {
            Trend_Pos->Trend_Num_72h++;
            RTEEPROMwrite(TREND_SAVE_INDEX + 17, Trend_Pos->Trend_Num_72h & 0xff);
        }
        break;
#endif
    default:
        break;
    }
    //保存当前的趋势数据，可以考录使用RTEEPROMwrite_Data函数组
    addr_cnt = 0;
    for(index = 0;index < TREND_PARA_NUM;index++)
    {
        if(index < INDEX_START_16U) //保存单字节参数
        {
            RTEEPROMwrite(Trend_Addr+addr_cnt,item->Para[ParaNameIndex[index]]&0xff);
            addr_cnt+=1;
        }                     
        else //保存双字节参数
        {
            RTEEPROMwrite(Trend_Addr+addr_cnt , (item->Para[ParaNameIndex[index]]&0xff00)>>8);
            RTEEPROMwrite(Trend_Addr+addr_cnt+1,(item->Para[ParaNameIndex[index]]&0xff));
            addr_cnt+=2;
        }
    }
}


/*
*根据链表的数据，重新刷写EEPROM,LPC2478使用的是单个字节存储的方式。
*
*/ 
unsigned int Trend_Addr ;
void SaveTrendToEEPROM_AllData(int TimeRange,TREND_EEPROM_S *Trend_Pos)
{
    
    unsigned short tmp_pos = 0;
    Node * tmp_node;
    int cnt;
    switch(TimeRange)
    {
    case 1:
        tmp_node = Tmp_Trend_1h.front.next;
        Trend_Addr = TREND_SAVEADDR_1h + TREND_PACKETSIZE*tmp_pos;
        for(cnt = 0;cnt < Trend_Pos->Trend_Num_1h;cnt++)
        {
            Trend_Addr = TREND_SAVEADDR_1h + TREND_PACKETSIZE*tmp_pos;

            if(tmp_pos >= TREND_PIXEL_1h)//计数器超过了当前的存储范围，跳转到存储首地址
            {
                Trend_Addr = TREND_SAVEADDR_1h;
                tmp_pos = 1;
            }
            else 
            { 
                tmp_pos++;
            }
           /* RTEEPROMwrite_Start(Trend_Addr);
            __SaveItemToEEPROM(tmp_node->item);      
            RTEEPROMwrite_End();   */
		    I2C0_MultiSend(Trend_Addr,tmp_node->item.Para,TREND_PARA_NUM);
            tmp_node = tmp_node->next;
            //write
        }
        break;
    case 4:
        tmp_node = Tmp_Trend_4h.front.next;
        Trend_Addr = TREND_SAVEADDR_4h + TREND_PACKETSIZE*tmp_pos;
        for(cnt = 0;cnt < Trend_Pos->Trend_Num_4h;cnt++)
        {
            Trend_Addr = TREND_SAVEADDR_4h + TREND_PACKETSIZE*tmp_pos;

            if(tmp_pos >= TREND_PIXEL_4h)//计数器超过了当前的存储范围，跳转到存储首地址
            {
                Trend_Addr = TREND_SAVEADDR_4h;
                tmp_pos = 1;
            }
            else 
            { 
                tmp_pos++;
            }
						I2C0_MultiSend(Trend_Addr,tmp_node->item.Para,TREND_PARA_NUM);

            tmp_node = tmp_node->next;
        }
        break;
    case 12:
        tmp_node = Tmp_Trend_12h.front.next;
        Trend_Addr = TREND_SAVEADDR_12h + TREND_PACKETSIZE*tmp_pos;
        for(cnt = 0;cnt < Trend_Pos->Trend_Num_12h;cnt++)
        {
            Trend_Addr = TREND_SAVEADDR_12h + TREND_PACKETSIZE*tmp_pos;

            if(tmp_pos >= TREND_PIXEL_12h)//计数器超过了当前的存储范围，跳转到存储首地址
            {
                Trend_Addr = TREND_SAVEADDR_12h;
                tmp_pos = 1;
            }
            else 
            { 
                tmp_pos++;
            }
						I2C0_MultiSend(Trend_Addr,tmp_node->item.Para,TREND_PARA_NUM);

            tmp_node = tmp_node->next;
        }
        break;
    case 24:
        tmp_node = Tmp_Trend_24h.front.next;
        Trend_Addr = TREND_SAVEADDR_24h + TREND_PACKETSIZE*tmp_pos;
        for(cnt = 0;cnt < Trend_Pos->Trend_Num_24h;cnt++)
        {
            Trend_Addr = TREND_SAVEADDR_24h + TREND_PACKETSIZE*tmp_pos;

            if(tmp_pos >= TREND_PIXEL_24h)//计数器超过了当前的存储范围，跳转到存储首地址
            {
                Trend_Addr = TREND_SAVEADDR_24h;
                tmp_pos = 1;
            }
            else 
            { 
                tmp_pos++;
            }
						I2C0_MultiSend(Trend_Addr,tmp_node->item.Para,TREND_PARA_NUM);

            tmp_node = tmp_node->next;
        }
        break;
#if (TREND_INFO_72H)
    case 48:
        tmp_node = Tmp_Trend_48h.front.next;
        Trend_Addr = TREND_SAVEADDR_48h + TREND_PACKETSIZE*tmp_pos;
        for(cnt = 0;cnt < Trend_Pos->Trend_Num_48h;cnt++)
        {
            Trend_Addr = TREND_SAVEADDR_48h + TREND_PACKETSIZE*tmp_pos;

            if(tmp_pos >= TREND_PIXEL_48h)//计数器超过了当前的存储范围，跳转到存储首地址
            {
                Trend_Addr = TREND_SAVEADDR_48h;
                tmp_pos = 1;
            }
            else 
            { 
                tmp_pos++;
            }
						I2C0_MultiSend(Trend_Addr,tmp_node->item->Para,TREND_PARA_NUM);

            tmp_node = tmp_node->next;
        }
        break;
    case 72:
        tmp_node = Tmp_Trend_72h.front.next;
        Trend_Addr = TREND_SAVEADDR_72h + TREND_PACKETSIZE*tmp_pos;
        for(cnt = 0;cnt < Trend_Pos->Trend_Num_72h;cnt++)
        {
            Trend_Addr = TREND_SAVEADDR_72h + TREND_PACKETSIZE*tmp_pos;

            if(tmp_pos >= TREND_PIXEL_72h)//计数器超过了当前的存储范围，跳转到存储首地址
            {
                Trend_Addr = TREND_SAVEADDR_72h;
                tmp_pos = 1;
            }
            else 
            { 
                tmp_pos++;
            }
						I2C0_MultiSend(Trend_Addr,tmp_node->item->Para,TREND_PARA_NUM);

            tmp_node = tmp_node->next;
        }
        break;
#endif
    default:
        break;
    }
}

/*
*按照EEPROM中存储数据的索引信息，读取数据写入到SDRAM中，并且初始化数据链表(头结点、尾结点和计数器)
*应该配合RTEEPROMread_Start和RTEEPROMread_End使用
*/
/*static void __GetItemOfEEPROM(Item *tmp)
{
    unsigned char index = 0;
    unsigned char cnt_multiple = 0;
    unsigned char cnt_inter = 0;
    for(index = 0;index < TREND_PARA_NUM;index++)
    {
        if(index < INDEX_START_16U) //保存单字节参数
        {
            tmp->Para[ParaNameIndex[index]]  = RTEEPROMread_Data();
        }
        else //保存双字节参数
        {
            tmp->Para[ParaNameIndex[index]]    = (RTEEPROMread_Data()<<8)|RTEEPROMread_Data();
        }

        if((index == MV)||(index == MVspn))
        {
            cnt_inter = 10;
        }
        else
        {
            cnt_inter = 1;
        }
        
        if(((tmp->Para[index] > ParaMaxLimit[index]*cnt_inter)||(tmp->Para[index] < ParaMinLimit[index]*cnt_inter))&&(index != ETCO2))//过滤掉不在范围内的数据
        {
            tmp->Para[index] = 0;
        }
        else if(index == ETCO2) 
        {
            if(ETCO2Unit == 1)//0~100
            {
              if((tmp->Para[index]>=100)||(tmp->Para[index] < 0))
               {
                   tmp->Para[index] = 0;
               }
            }
            else//0~13.3
            {
              if((tmp->Para[index]>=133)||(tmp->Para[index] < 0))
               {
                   tmp->Para[index] = 0;
               }                 
            }
        }
        
    }  
}  */

/*
*按照EEPROM中存储数据的索引信息，读取数据写入到SDRAM中，并且初始化数据链表(头结点、尾结点和计数器)
*应该配合RTEEPROMwrite_Start和RTEEPROMwrite_End使用
*/
/*static void __SaveItemToEEPROM(Item tmp)
{
    unsigned char index = 0;
    for(index = 0;index < TREND_PARA_NUM;index++)
    {
        if(index < INDEX_START_16U) //保存单字节参数
        {
            RTEEPROMwrite_Data(tmp.Para[ParaNameIndex[index]]&0xff);
        }
        else //保存双字节参数
        {
            RTEEPROMwrite_Data((tmp.Para[ParaNameIndex[index]]&0xff00)>>8);
            RTEEPROMwrite_Data(tmp.Para[ParaNameIndex[index]]&0xff);
        }
    }    
}     */

/*
* 将EEPROM中的数据读取出来，并且存储到相应的队列中
* PositionP:EEPROM中的首位置
* NumberP：队列在EEPROM中存储的元素个数
* DesQueue：队列的地址
* PixelNum：队列该队列最大存储元素的个数
* TREND_SAVEADDR：该队列数据在EEPROM中存储的首地址
*/
static void _SaveTrendDateToQueue(unsigned short *PositionP,unsigned short *NumberP,Queue *DesQueue,int PixelNum,unsigned int TREND_SAVEADDR)
{
    unsigned int Trend_Addr ;
    int cnt;
    Item tmp;
        //判断当前时间段内的结点数量是否正常
        if(*NumberP > PixelNum)
        {
            *NumberP = PixelNum ;
        }
        //判断当前时间段的位置信息是否正常
        if(*PositionP >= PixelNum)
        {
            if(*NumberP == PixelNum)//如果已经存满了数据，则下个数据从偏移地址0开始
            {
                *PositionP = 0;
            }
            else//否则从当前数据的下一个位置开始
            {
                *PositionP = *NumberP;
            }
        }
        //计算得出数据的首地址。PS：Trend_Pos.Trend_Pos_1h为最新的数据，但是插入链表时应该先插入最老的数据，所以应该得到最先存入的数据地址。
        if(*NumberP != PixelNum)//没有超过360个，表示从首地址开始的
        {
            Trend_Addr = TREND_SAVEADDR;
        }
        else 
				if((*NumberP == PixelNum) && (*PositionP == 0))//存的数时TREND_PIXEL_1h的整数倍
        {
            Trend_Addr = TREND_SAVEADDR;
        }
        else//表示至少存了TREND_PIXEL_1h+1（361）个数，那么当前最新的数据的下一个地址，应该存放的是首个数据例如：{6,7,8,9,1,2,3,4,5}                                                                                      //9表示最后存的数据，那么他下一个数
                                                                                                  //就是最开始存的数据
        {
            Trend_Addr = TREND_SAVEADDR + TREND_PACKETSIZE*(*PositionP);//最新数据的下一个地址。
        }

        for(cnt = 0;cnt < *NumberP;cnt++)
        {
            if(Trend_Addr >= TREND_SAVEADDR + TREND_PACKETSIZE*PixelNum)
            {
                Trend_Addr = TREND_SAVEADDR;
            }				
						I2C0_MultiReceive(Trend_Addr,tmp.Para,TREND_PARA_NUM);
            InsertQueue(tmp,DesQueue);
            Trend_Addr += (TREND_PACKETSIZE);
        }
}


/*
*  将EEPROM中的数据读取出来，并且存储到相应的队列中。
*  未完成功能：开机后，根据最后一个数据的时间和当前的时间，计算出空闲的时间，并且将空白段填充0
*/
void ReadTrendOfEEPROM(WM_HWIN hProgBar)
{
/////////////////////////////////////////////////////////////////////////////////////////////////////
    Item tmp;
    int  cnt ,pos;
    RTC_TIMER Now_Time = RTCTime;
    unsigned int Trend_Addr ;
    _InitTrendEEPROM_S(&Trend_Pos);//EEPROM Position
    if(Trend_Pos.Trend_Num_1h != 0)//数据不为空
    {
        _SaveTrendDateToQueue(&Trend_Pos.Trend_Pos_1h,&Trend_Pos.Trend_Num_1h,&Tmp_Trend_1h,TREND_PIXEL_1h,TREND_SAVEADDR_1h);
        if(Trend_Pos.Trend_Num_4h != 0)
        {
            _SaveTrendDateToQueue(&Trend_Pos.Trend_Pos_4h,&Trend_Pos.Trend_Num_4h,&Tmp_Trend_4h,TREND_PIXEL_4h,TREND_SAVEADDR_4h);
            if(Trend_Pos.Trend_Num_12h != 0)
            {
                _SaveTrendDateToQueue(&Trend_Pos.Trend_Pos_12h,&Trend_Pos.Trend_Num_12h,&Tmp_Trend_12h,TREND_PIXEL_12h,TREND_SAVEADDR_12h);
                if(Trend_Pos.Trend_Num_24h != 0)
                {
                    _SaveTrendDateToQueue(&Trend_Pos.Trend_Pos_24h,&Trend_Pos.Trend_Num_24h,&Tmp_Trend_24h,TREND_PIXEL_24h,TREND_SAVEADDR_24h);
#if (TREND_INFO_72H)
                    if(Trend_Pos.Trend_Num_48h != 0)
                    {
                        _SaveTrendDateToQueue(&Trend_Pos.Trend_Pos_48h,&Trend_Pos.Trend_Num_48h,&Tmp_Trend_48h,TREND_PIXEL_48h,TREND_SAVEADDR_48h);
                        if(Trend_Pos.Trend_Num_72h != 0)
                        {
                            _SaveTrendDateToQueue(&Trend_Pos.Trend_Pos_72h,&Trend_Pos.Trend_Num_72h,&Tmp_Trend_72h,TREND_PIXEL_72h,TREND_SAVEADDR_72h);
                        }
                    }
#endif
                }
            }
        }
    }
}

/*根据输入的年份和月份，返回当前月份的日期数*/
static int GetMon_Day(int year,int mon)
{
    int Mon_Val;
    if(mon == 2)
    {
        Mon_Val = LeapYear(year);
    }
    else if((mon == 4)||(mon == 6)||(mon == 9)||(mon == 11))
    {
        Mon_Val = 30;
    }
    else
    {
        Mon_Val = 31;
    }
    return Mon_Val;
}

void __EmptyDispQueue(void)
{
    EmptyQueue(&Tmp_Trend_1s);
    EmptyQueue(&Tmp_Trend_1h);
    EmptyQueue(&Tmp_Trend_4h);
    EmptyQueue(&Tmp_Trend_12h);
    EmptyQueue(&Tmp_Trend_24h);
#if (TREND_INFO_72H)
    EmptyQueue(&Tmp_Trend_48h);
    EmptyQueue(&Tmp_Trend_72h);
#endif
}

//将队列中的参数个数赋值给TrendPos变量
//flag:0  queuenum->TrendPos.num
//flag:1  TrendPos.pos = TrendPos.num
void __ModifyTrendEEPROM_S(void)
{
    Trend_Pos.Trend_Num_1h     = (Tmp_Trend_1h.items > TREND_PIXEL_1h)?TREND_PIXEL_1h:Tmp_Trend_1h.items; 
    Trend_Pos.Trend_Num_4h     = (Tmp_Trend_4h.items > TREND_PIXEL_4h)?TREND_PIXEL_4h:Tmp_Trend_4h.items; 
    Trend_Pos.Trend_Num_12h    = (Tmp_Trend_12h.items > TREND_PIXEL_12h)?TREND_PIXEL_12h:Tmp_Trend_12h.items; 
    Trend_Pos.Trend_Num_24h    = (Tmp_Trend_24h.items > TREND_PIXEL_24h)?TREND_PIXEL_24h:Tmp_Trend_24h.items; 
#if (TREND_INFO_72H)
    Trend_Pos.Trend_Num_48h    = (Tmp_Trend_48h.items > TREND_PIXEL_48h)?TREND_PIXEL_48h:Tmp_Trend_48h.items; 
    Trend_Pos.Trend_Num_72h    = (Tmp_Trend_72h.items > TREND_PIXEL_72h)?TREND_PIXEL_72h:Tmp_Trend_72h.items;
#endif     
    //0开始，表示下一个数据存储的位置
    Trend_Pos.Trend_Pos_1h   = (Tmp_Trend_1h.items == TREND_PIXEL_1h)?0:Tmp_Trend_1h.items;
    Trend_Pos.Trend_Pos_4h   = (Tmp_Trend_4h.items == TREND_PIXEL_4h)?0:Tmp_Trend_4h.items;
    Trend_Pos.Trend_Pos_12h  = (Tmp_Trend_12h.items == TREND_PIXEL_12h)?0:Tmp_Trend_12h.items;
    Trend_Pos.Trend_Pos_24h  = (Tmp_Trend_24h.items == TREND_PIXEL_24h)?0:Tmp_Trend_24h.items;
#if (TREND_INFO_72H)
    Trend_Pos.Trend_Pos_48h  = (Tmp_Trend_48h.items == TREND_PIXEL_48h)?0:Tmp_Trend_48h.items;
    Trend_Pos.Trend_Pos_72h  = (Tmp_Trend_72h.items == TREND_PIXEL_72h)?0:Tmp_Trend_72h.items;
#endif      
}


/*
*初始化趋势图数据缓存队列：地址、队列长度、清空队列 和 每段数据在EEPROM中头数据的位置
*
*
*/
#define SDRAMADDR_INTERVAL ((TREND_PARA_NUM*2+15)*360+0x1000)//ylk add 20181123
void InitTrendQueue(WM_HWIN hProgBar)//备注，算法有待优化，以便插入最少量的数据
{
    RTC_TIMER RealTime = RTCTime;
    int Clear_Flag = 0;  //清除标志
    
    //初始化SDRAM中链表信息
    InitializeQueue(&Tmp_Trend_1s, SDRAMCACHE_BASEADDR);  
    QueueMaxItem(&Tmp_Trend_1s,15);////////////10s内存储数据的数量
    InitializeQueue(&Tmp_Trend_1h, SDRAMCACHE_BASEADDR + SDRAMADDR_INTERVAL);  //0X5000
    QueueMaxItem(&Tmp_Trend_1h,TREND_PIXEL_1h);
    InitializeQueue(&Tmp_Trend_4h, SDRAMCACHE_BASEADDR + SDRAMADDR_INTERVAL*2);  
    QueueMaxItem(&Tmp_Trend_4h,TREND_PIXEL_4h);
    InitializeQueue(&Tmp_Trend_12h,SDRAMCACHE_BASEADDR + SDRAMADDR_INTERVAL*3);  
    QueueMaxItem(&Tmp_Trend_12h,TREND_PIXEL_12h);
    InitializeQueue(&Tmp_Trend_24h,SDRAMCACHE_BASEADDR + SDRAMADDR_INTERVAL*4);  
    QueueMaxItem(&Tmp_Trend_24h,TREND_PIXEL_24h);
#if (TREND_INFO_72H)
    InitializeQueue(&Tmp_Trend_48h,SDRAMCACHE_BASEADDR + SDRAMADDR_INTERVAL*5);  
    QueueMaxItem(&Tmp_Trend_48h,TREND_PIXEL_48h);
    InitializeQueue(&Tmp_Trend_72h,SDRAMCACHE_BASEADDR + SDRAMADDR_INTERVAL*6);  
    QueueMaxItem(&Tmp_Trend_72h,TREND_PIXEL_72h);
#endif
    __EmptyDispQueue();

    //读取上次趋势图中最新的一个时间点    RTCTime
    _InitTrendEEPROM_Time(&TrendSaveDate_Time);
    //判断当前时间和上一次使用的时间，是否在3天之内，如果不在3天之内，则清除全部的趋势数据
#if (TREND_INFO_24H)
    //舍弃了1天整（几率可以忽略）
    if(((RealTime.RTC_Year - TrendSaveDate_Time.RTC_Year == 1)&&(RealTime.RTC_Mon == 1)&&(TrendSaveDate_Time.RTC_Mon == 12)&&(RealTime.RTC_Mday + (31 - TrendSaveDate_Time.RTC_Mday) <= 1))//例：2014.1.1    2013.12.29   不是同一年的情况 
    || ((RealTime.RTC_Year == TrendSaveDate_Time.RTC_Year)&&(RealTime.RTC_Mon == TrendSaveDate_Time.RTC_Mon)&&(RealTime.RTC_Mday - TrendSaveDate_Time.RTC_Mday <= 1)) //例：2014.1.4    2014.1.5
    || ((RealTime.RTC_Year == TrendSaveDate_Time.RTC_Year)&&(RealTime.RTC_Mon - TrendSaveDate_Time.RTC_Mon == 1)&&((RealTime.RTC_Mday + (GetMon_Day(TrendSaveDate_Time.RTC_Year,TrendSaveDate_Time.RTC_Mon) - TrendSaveDate_Time.RTC_Mday) <= 1))))//例：2014.3.1   2014.2.28
#elif (TREND_INFO_72H)
    //舍弃了3天整（几率可以忽略）
    if(((RealTime.RTC_Year - TrendSaveDate_Time.RTC_Year == 1)&&(RealTime.RTC_Mon == 1)&&(TrendSaveDate_Time.RTC_Mon == 12)&&(RealTime.RTC_Mday + (31 - TrendSaveDate_Time.RTC_Mday) <= 3))//例：2014.1.1    2013.12.29   不是同一年的情况 
    || ((RealTime.RTC_Year == TrendSaveDate_Time.RTC_Year)&&(RealTime.RTC_Mon == TrendSaveDate_Time.RTC_Mon)&&(RealTime.RTC_Mday - TrendSaveDate_Time.RTC_Mday <= 3)) //例：2014.1.4    2014.1.5
    || ((RealTime.RTC_Year == TrendSaveDate_Time.RTC_Year)&&(RealTime.RTC_Mon - TrendSaveDate_Time.RTC_Mon == 1)&&((RealTime.RTC_Mday + (GetMon_Day(TrendSaveDate_Time.RTC_Year,TrendSaveDate_Time.RTC_Mon) - TrendSaveDate_Time.RTC_Mday) <= 3))))//例：2014.3.1   2014.2.28
#endif
    {
        //读取EEPROM中的趋势数据，写入到链表中
        ReadTrendOfEEPROM(hProgBar);
        
        //计算关机的时间，根据关机的这段时间写入0 数据

        tblock.tm_year = RealTime.RTC_Year - 1900;
        tblock.tm_mon  = RealTime.RTC_Mon - 1;
        tblock.tm_mday = RealTime.RTC_Mday;
        tblock.tm_hour = RealTime.RTC_Hour;
        tblock.tm_min  = RealTime.RTC_Min;
        tblock.tm_sec  = RealTime.RTC_Sec - RealTime.RTC_Sec%10;//去掉个位 
        timer0 = mktime(&tblock);

        tblock.tm_year = TrendSaveDate_Time.RTC_Year - 1900;
        tblock.tm_mon  = TrendSaveDate_Time.RTC_Mon - 1;
        tblock.tm_mday = TrendSaveDate_Time.RTC_Mday;
        tblock.tm_hour = TrendSaveDate_Time.RTC_Hour;
        tblock.tm_min  = TrendSaveDate_Time.RTC_Min;
        tblock.tm_sec  = TrendSaveDate_Time.RTC_Sec - TrendSaveDate_Time.RTC_Sec%10;
        timer1 = mktime(&tblock);
        SecValue = timer0 - timer1;
        if(SecValue > TREND_MAX_TIME*3600)//Hour * 60 * 60  三天
        {
            Clear_Flag = 1;//超出时间范围
        }
        else 
        {
            num = SecValue/10 + ((SecValue%10)?1:0);
            for(SecValue = 0;SecValue < num;SecValue++)
            {
                InsertEmpty_ToList();//插入空白点
            }
        }
    }
    if(Clear_Flag == 1) //清除趋势图的链表的位置（相当于清除趋势的数据，但是没有清除EEPTOM中的趋势数据，只是清除了趋势数据的链表信息）
    {
        ClearTrendInfo();
    }
    else ////将更新后的链表数据全部写入到EEPROM相应的位置中，并且更新时间
    { 
        __ModifyTrendEEPROM_S();
        __SetTrend_PosAndTime(&Trend_Pos,&RealTime);//将新的链表索引结构体存入EEPROM
        if(Trend_Pos.Trend_Num_1h > 0)
        {
            SaveTrendToEEPROM_AllData(1, &Trend_Pos);//将新生成的链表存储到EEPROM中       
            if(Trend_Pos.Trend_Num_4h > 0)
            {
                SaveTrendToEEPROM_AllData(4, &Trend_Pos);//将新生成的链表存储到EEPROM中                  
                if(Trend_Pos.Trend_Num_12h > 0)
                {
                    SaveTrendToEEPROM_AllData(12, &Trend_Pos);//将新生成的链表存储到EEPROM中                  
                    if(Trend_Pos.Trend_Num_24h > 0)
                    {
                        SaveTrendToEEPROM_AllData(24, &Trend_Pos);//将新生成的链表存储到EEPROM中                  
#if (TREND_INFO_72H)
                        if(Trend_Pos.Trend_Num_48h > 0)
                        {
                            SaveTrendToEEPROM_AllData(48, &Trend_Pos);//将新生成的链表存储到EEPROM中                  
                            if(Trend_Pos.Trend_Num_72h > 0)
                            {
                                SaveTrendToEEPROM_AllData(72, &Trend_Pos);//将新生成的链表存储到EEPROM中                  
                            }
                        }
#endif
                    }
                }
            }
        }
    }
}


/*
*    每秒钟取出两个点存储到SDRAM中，共取10s内的15个数据(由数据缓存队列的长度决定，最多20个数据)
*填满队列或者到了下一个10s之后，取这个队列中相同变量的平均值，保存到最近时间点的buffer（1h）中。
*如果最近时间点的buffer存满，则向后递推存储。
*/
#define ACTIVE_MINNUM 3//10s内至少存储ACTIVE_MINNUM(大于0)个数据，才能判定本次为有效数据
                       //测试的时候需要快速得到大量的数据，这时设置为0，每秒得到1个数据
                       
void Get_TrendDate_1s(MONITOR_PARA *monitor,RTC_TIMER RTCTime)
{
    Item tmp,tmp_item;
    Node *tmp_node;
    int cnt = 0, max = 0,cnt_inter = 1;
    unsigned char cnt_multiple = 0;
    unsigned char sec = RTCTime.RTC_Sec;
    static int Date_Flag = 0;
    static int Last_sec  = 100;
    static int num_4h  = 0;
    static int num_12h = 0;
    static int num_24h = 0;
#if (TREND_INFO_72H)
    static int num_48h = 0;
    static int num_72h = 0;
#endif
    Int8U Item_number[TREND_PARA_NUM] = {0};
    
    //清空tmp
    InitItem(&tmp);
    InitItem(&tmp_item);

    if((StandbyState != 0xFF)&&(!Date_Flag))//待机下不进行数据处理
    {
        if(QueueIsFull(&Tmp_Trend_1s))
        {
            Date_Flag = 1;
        }
        else if(Date_Flag == 0)
        {
            //保存本次趋势参数参数，并判断其有效性(多为选配，查看该选配是否开启)。如果参数无效，则保存0
            tmp.Para[PPEAK]  = monitor->PpeakValue;
            tmp.Para[PMEAN]  = monitor->PmeanValue;
            tmp.Para[PEEP]   = monitor->PeepValue;
            tmp.Para[MV]     = monitor->MinuteVolumeValue;
            tmp.Para[MVSPN]  = monitor->MVspnValue;
            tmp.Para[VTE]    = monitor->VteValue;
            tmp.Para[VTI]    = monitor->VtiValue;
            tmp.Para[FTOTAL] = monitor->RateTotalValue;
            tmp.Para[FSPN]   = monitor->fspnValue;
            tmp.Para[CDYN]   = monitor->ComplianceValue;
            if((BeathSettingPara.SettingTpValue!=0)
                &&((BeathSettingPara.BreathMode==VCV_MODE)||(BeathSettingPara.BreathMode==SIGH_MODE)))
            {
                tmp.Para[RST]    =monitor->ResistValue;
            }
            else
            {
                tmp.Para[RST]    =0;
            }
            if((AA_Module_Flag.EtCO2ModuleExist)&&(EtCO2ModuleOff))
            {
                tmp.Para[ETCO2] = CO2_Parameters.etCO2_mmhg;
				tmp.Para[FICO2] = CO2_Parameters.insCO2_mmhg;//ylk add 20181122 新增参数
            }
            else 
            {
                tmp.Para[ETCO2] = 0;
				tmp.Para[FICO2] = 0;
            }
#ifdef SPO2_SWITCH
            if((SpO2ModuleExist)&&(!SpO2ModuleOff))
            {
				tmp.Para[SPO2]   = SpO2_Parameters.SpO2;
				tmp.Para[PULSE]  = SpO2_Parameters.PulseRate;
            }
            else 
            {
                tmp.Para[SPO2]   = 0;
                tmp.Para[PULSE]  = 0;
            }
#endif
            if(O2ModuleOff)
            {
                tmp.Para[FIO2]   = monitor->FiO2Value;
            }
            else
            {
                tmp.Para[FIO2]   = monitor->FiO2Value;
            }
			if((AA_Module_Flag.EtCO2ModuleExist)&&(EtCO2ModuleOff))//ylk add 20181122 新增参数
			{
				tmp.Para[ETN2O]  = AA_Para.EtN2O;
				tmp.Para[FIN2O]  = AA_Para.InN2O;

				tmp.Para[ETAA1]  = AA_Para.EtAX1 ;
				tmp.Para[FIAA1]  = AA_Para.InAX1 ;
				tmp.Para[ETAA2]  = AA_Para.EtAX2 ;
				tmp.Para[FIAA2]  = AA_Para.InAX2 ;
				tmp.Para[MAC_]   = AA_Para.MAC;//djq modify 20170523去掉了/10，保留2位小数
			}
			else 
			{
				tmp.Para[ETN2O]  = 0;
				tmp.Para[FIN2O]  = 0;

				tmp.Para[ETAA1]  = 0 ;
				tmp.Para[FIAA1]  = 0 ;
				tmp.Para[ETAA2]  = 0 ;
				tmp.Para[FIAA2]  = 0 ;
				tmp.Para[MAC_]   = 0;
			}
            //判断数据是否在正常范围内，不在范围内的赋值为0.
            for(cnt = 0;cnt < TREND_PARA_NUM;cnt ++)
            {
                for(cnt_multiple = 0;cnt_multiple < PARA_10M_NUM;cnt_multiple++)//确定参数的放大倍数
                {
                    cnt_inter = 1;
                    if(cnt == Para_10M_NameIndex[cnt_multiple])
                    {
                        cnt_inter = 10;
                        break;
                    }
                    //if(cnt == Para_100M_NameIndex[cnt_100m])
                    //{
                    //    cnt_inter = 100;
                    //    break;
                    //}
                }
                if((tmp.Para[cnt] > ParaMaxLimit[cnt]*cnt_inter)||(tmp.Para[cnt] < ParaMinLimit[cnt]*cnt_inter))//过滤掉不在范围内的数据
                {
                    tmp.Para[cnt] = 0;
                }
            }
            InsertQueue(tmp,&Tmp_Trend_1s);
        }
    }

    //清空tmp
    InitItem(&tmp);
    InitItem(&tmp_item);

#if (ACTIVE_MINNUM)
    if(Last_sec/10 != sec/10)//10秒的整倍数，进入数据存储函数
#endif
    {
        Date_Flag = 0;
        Last_sec = sec;
        //保存时间
        tmp.Time.RTC_Sec = Last_sec - Last_sec%10;
        tmp.Time.RTC_Min = RTCTime.RTC_Min;
        tmp.Time.RTC_Hour = RTCTime.RTC_Hour;
        tmp.Time.RTC_Mday = RTCTime.RTC_Mday;
        tmp.Time.RTC_Mon = RTCTime.RTC_Mon;
        tmp.Time.RTC_Year = RTCTime.RTC_Year;
        //在此处算取平均数，并且存到第一小时缓存区的第一个位置
        //如果第一小时存满，则将首项目挪动到下一个小时的缓存区
        //
        max = QueueItemCount(&Tmp_Trend_1s);
        
        if(StandbyState!=0xFF)//如果在待机模式下，则不进行数据处理
        {
            //取各个监测参数的平均值
            if(max > ACTIVE_MINNUM)
            {
                tmp_node = Tmp_Trend_1s.front.next;
                //求10s相同参数数据累加和
                for(cnt = 0;cnt < max;cnt++)//在一个10s的阶段中，如果切换了待机状态，数据准确度会受到影响。所以应该在切换待机的下一个10s开始计数
                {
                    if(tmp_node == NULL)
                    {
                        break;
                    }
                    for(cnt_inter = 0;cnt_inter < TREND_PARA_NUM;cnt_inter++)
                    {
                        //如果当前的数据为0则不累加到结果中
                        if(!tmp_node->item.Para[cnt_inter])
                        {
                            continue;
                        }
                        Item_number[cnt_inter] += 1;
                        tmp.Para[cnt_inter] += tmp_node->item.Para[cnt_inter];
                    }
                    tmp_node = tmp_node->next;
                }
                //求10s相同参数数据平局值
                for(cnt = 0;cnt < TREND_PARA_NUM;cnt ++)
                {
                    if(Item_number[cnt])///确保除数大于零
                    {
                        for(cnt_multiple = 0;cnt_multiple < PARA_10M_NUM;cnt_multiple++) //确定参数的放大倍数
                        {
                            cnt_inter = 1;
                            if(cnt == Para_10M_NameIndex[cnt_multiple])
                            {
                                cnt_inter = 10;
                                break;
                            }
                            //if(cnt == Para_100M_NameIndex[cnt_100m])
                            //{
                            //    cnt_inter = 100;
                            //    break;
                            //}
                        }
                        if(tmp.Para[cnt] % (Item_number[cnt]*10) >= 5*Item_number[cnt])//
                        {
                            tmp.Para[cnt]  = tmp.Para[cnt]/(Item_number[cnt]*cnt_inter) + 1;
                        }
                        else //舍弃小数位
                        {
                            tmp.Para[cnt]  /= (Item_number[cnt]*cnt_inter);
                        }                 
                    }
                }
            }
            else //如果本次10s内累计的数据量过少，则不能算做有效数据，则清空
            {
                InitItem(&tmp);
                InitItem(&tmp_item);
            }
        }
        SaveTrendToEEPROM(&tmp, 1, &Trend_Pos);//将当前数据存储到EEPROM 中

        if(QueueIsFull(&Tmp_Trend_1h))//一小时队列已经存满，需要取出时间最老的一个点，存到下一个小时中去
        {
            DeleteQueue(&tmp_item,&Tmp_Trend_1h);
            InsertQueue(tmp,&Tmp_Trend_1h);
            if(num_4h%4 == 0)//每隔4个点取一个值
            {
                SaveTrendToEEPROM(&tmp_item, 4, &Trend_Pos);//将当前数据存储到EEPROM 中
                num_4h = 0;
                if(QueueIsFull(&Tmp_Trend_4h)) //////////////////这里有问题，每个buffer替换出来的数据不能直接加入到下一个buffer中，而是应该多个数据取平均值
                {
                    DeleteQueue(&tmp,&Tmp_Trend_4h);
                    InsertQueue(tmp_item,&Tmp_Trend_4h);
                    if(num_12h%3 == 0)//在4h的基础上，每隔3个点取一个值
                    {
                        SaveTrendToEEPROM(&tmp, 12, &Trend_Pos);//将当前数据存储到EEPROM 中
                        num_12h = 0;
                        if(QueueIsFull(&Tmp_Trend_12h))
                        {
                            DeleteQueue(&tmp_item,&Tmp_Trend_12h);
                            InsertQueue(tmp,&Tmp_Trend_12h);
                            if(num_24h%2 == 0)//在12h基础上，每隔2个点取一个点
                            {
                                SaveTrendToEEPROM(&tmp_item, 24, &Trend_Pos);//将当前数据存储到EEPROM 中
                                num_24h = 0;
                                if(QueueIsFull(&Tmp_Trend_24h))
                                {
                                    DeleteQueue(&tmp,&Tmp_Trend_24h);
                                    InsertQueue(tmp_item,&Tmp_Trend_24h);
#if (TREND_INFO_72H)
                                    if(num_48h%2 == 0)//在24h基础上，每隔2个点取一个点
                                    {
                                        SaveTrendToEEPROM(&tmp, 48, &Trend_Pos);//将当前数据存储到EEPROM 中
                                        num_48h = 0;
                                        if(QueueIsFull(&Tmp_Trend_48h))
                                        {
                                            DeleteQueue(&tmp_item,&Tmp_Trend_48h);
                                            InsertQueue(tmp,&Tmp_Trend_48h);
                                            if(num_72h%3 != 1)//在48h基础上，每3 个点中，舍弃中间的一个点
                                            {
                                                SaveTrendToEEPROM(&tmp_item, 72, &Trend_Pos);//将当前数据存储到EEPROM 中
                                                num_72h = 0;
                                                if(QueueIsFull(&Tmp_Trend_72h))
                                                {
                                                    DeleteQueue(&tmp,&Tmp_Trend_72h);
                                                    InsertQueue(tmp_item,&Tmp_Trend_72h);
                                                }
                                                else 
                                                {
                                                    InsertQueue(tmp_item,&Tmp_Trend_72h);
                                                }
                                            }//else 72h

                                            num_72h++;
                                        }
                                        else 
                                        {
                                            InsertQueue(tmp,&Tmp_Trend_48h);
                                        }
                                    }//else num_48h

                                    num_48h++;            
#endif
                                }
                                else 
                                {
                                    InsertQueue(tmp_item,&Tmp_Trend_24h);
                                }
                            }//else num_24h
                            
                            num_24h++;
                        }
                        else 
                        {
                            InsertQueue(tmp,&Tmp_Trend_12h);
                        }
                    }//else num_12h

                    num_12h++;
                }
                else 
                {
                    InsertQueue(tmp_item,&Tmp_Trend_4h);
                }
            }//else num_4h 
            
            num_4h++;
        }
        else 
        {
            InsertQueue(tmp,&Tmp_Trend_1h);
        }
        EmptyQueue(&Tmp_Trend_1s);
    }
}

/*插入空点到链表中,并且更新Trend_Pos结构体*/
static void InsertEmpty_ToList(void)
{
    Item tmp,tmp_item;
    int cnt = 0;
    static int num_4h  = 0;
    static int num_12h = 0;
    static int num_24h = 0;
#if (TREND_INFO_72H)
    static int num_48h = 0;
    static int num_72h = 0;
#endif
   //memset(&tmp,0,sizeof(Item));
    InitItem(&tmp);
    if(QueueIsFull(&Tmp_Trend_1h))//一小时队列已经存满，需要取出时间最老的一个点，存到下一个小时中去
    {
        DeleteQueue(&tmp_item,&Tmp_Trend_1h);
        InsertQueue(tmp,&Tmp_Trend_1h);
        if(num_4h%4 == 0)//每隔4个点取一个值
        {
            num_4h = 0;
            if(QueueIsFull(&Tmp_Trend_4h)) //////////////////这里有问题，每个buffer替换出来的数据不能直接加入到下一个buffer中，而是应该多个数据取平均值
            {
                DeleteQueue(&tmp,&Tmp_Trend_4h);
                InsertQueue(tmp_item,&Tmp_Trend_4h);
                if(num_12h%3 == 0)//在4h的基础上，每隔3个点取一个值
                {
                    num_12h = 0;
                    if(QueueIsFull(&Tmp_Trend_12h))
                    {
                        DeleteQueue(&tmp_item,&Tmp_Trend_12h);
                        InsertQueue(tmp,&Tmp_Trend_12h);
                        if(num_24h%2 == 0)//在12h基础上，每隔2个点取一个点
                        {
                            num_24h = 0;
                            if(QueueIsFull(&Tmp_Trend_24h))
                            {
                                DeleteQueue(&tmp,&Tmp_Trend_24h);
                                InsertQueue(tmp_item,&Tmp_Trend_24h);
#if (TREND_INFO_72H)
                                if(num_48h%2 == 0)//在24h基础上，每隔2个点取一个点
                                {
                                    num_48h = 0;
                                    if(QueueIsFull(&Tmp_Trend_48h))
                                    {
                                        DeleteQueue(&tmp_item,&Tmp_Trend_48h);
                                        InsertQueue(tmp,&Tmp_Trend_48h);
                                        if(num_72h%3 != 1)//在48h基础上，每3 个点中，舍弃中间的一个点
                                        {
                                            num_72h = 0;
                                            if(QueueIsFull(&Tmp_Trend_72h))
                                            {
                                                DeleteQueue(&tmp,&Tmp_Trend_72h);
                                                InsertQueue(tmp_item,&Tmp_Trend_72h);
                                            }
                                            else 
                                            {
                                                InsertQueue(tmp_item,&Tmp_Trend_72h);
                                            }
                                        }//else 72h

                                        num_72h++;
                                    }
                                    else 
                                    {
                                        InsertQueue(tmp,&Tmp_Trend_48h);
                                    }
                                }//else num_48h

                                num_48h++;                                    
#endif
                            }
                            else 
                            {
                                InsertQueue(tmp_item,&Tmp_Trend_24h);
                            }
                        }//else num_24h
                        
                        num_24h++;
                    }
                    else 
                    {
                        InsertQueue(tmp,&Tmp_Trend_12h);
                    }
                }//else num_12h

                num_12h++;
            }
            else 
            {
                InsertQueue(tmp_item,&Tmp_Trend_4h);
            }
        }//else num_4h 
        
        num_4h++;
    }
    else 
    {
        InsertQueue(tmp,&Tmp_Trend_1h);
    }
}

/*
*    传入一个节点和一个数字n，则返回距离该节点n个距离的节点指针。
*/
static Node *_GetNextXNoteP(Node * SrcP,int num)
{
    Node *p = SrcP;
    int cnt;
    for(cnt = 0;cnt < num;cnt++)
    {
        if(p->next == NULL)
        {
            break;
        }
        p = p->next;
    }
    return p;
}

static Node *_GetPerXNoteP(Node * SrcP,int num)
{
    Node *p = SrcP;
    int cnt;
    for(cnt = 0;cnt < num;cnt++)
    {
        if(p->prev == NULL)
        {
            break;
        }
        p = p->prev;
    }
    return p;
}


/*
*函数名：Fill_TrendGraphDispBuffer
*    根据输入的时间量程和参数名称，取出显示所需要的数据，
*计算合并显示点之后，存储到相应的趋势图buffer中
*返回值：成功1，失败0（没有该变量）   

*备注：每隔多个点取一个点时，应该取其平均值，但是现在先没取
*/

//专用于Fill_TrendGraphDispBuffer函数中（为了使代码简单化）
static int _Fill_TrendGraphDispS(Trend_Disp_S *Trend_DispTmp,Node **tmp_node,int cnt,int ParaNum,int next_num)
{
    if((tmp_node == NULL)||(cnt >= TREND_BUFFERSIZE))
    {

    }
    else
    {
        if(ParaNum >= INDEX_START_16U)//单双字节区分
        {
            Trend_DispTmp->tmp_short[cnt]  = (*tmp_node)->item.Para[ParaNum];
        }
        else
        {
            Trend_DispTmp->tmp_char[cnt]  = (*tmp_node)->item.Para[ParaNum];
        }
        *tmp_node = _GetNextXNoteP(*tmp_node,next_num);
    }
    return 0;
}


//填充一个趋势图的显示buffer
int Fill_TrendGraphDispBuffer(TracingCurve_Handle hObj,const char *ParaName)
{
    Trend_Disp_S Trend_DispTmp = {0};
    int cnt,Index = 10,ParaNum = 10;
    int TimeRange = 0,tmp_num = 0;
    static Node *tmp_node;
    static char cnt_48h = 0;
    int Max_Addr = 0;
    if (hObj)
    {
        TracingCurve_Obj* pObj;
        WM_LOCK();
        pObj = TracingCurve_H2P(hObj);
        TimeRange = (int)pObj->XYAD_INFO.XMax;
        //判断输入的趋势图中是否存在该变量。
        for(cnt = 0;cnt < pObj->TrendNumber;cnt++)
        {
            if(!strcmp(pObj->Trend_Info[cnt].ParaText,ParaName))
            {
                Index = cnt;
                break;
            }  
        }
        if(Index != 10)//存在该变量
        {
            //判断该变量在存储时的位置
            for(cnt = 0;cnt < TREND_PARA_NUM;cnt++)
            {
                if(!strcmp(TrendParaName[cnt],ParaName))
                {
                    ParaNum = cnt;
                    break;
                }
            }

            switch(TimeRange)
            {
            case 1:
                tmp_node = Tmp_Trend_1h.front.next;
                //没有数据的区间使用0填充
                for(cnt = (pObj->TrendPixelNum - Tmp_Trend_1h.items);cnt < pObj->TrendPixelNum;cnt++)//如果显示BUFFER 没有填满，剩下的地方补充0 
                {
                    _Fill_TrendGraphDispS(&Trend_DispTmp,&tmp_node,cnt,ParaNum,1);
                }
                break;
            case 4:
                //4h
                if(Tmp_Trend_4h.items > 0)//
                {
                    tmp_node = Tmp_Trend_4h.front.next;
                    for(cnt = (Tmp_Trend_4h.max_item - Tmp_Trend_4h.items);cnt < Tmp_Trend_4h.max_item;cnt++)//如果显示BUFFER 没有填满，剩下的地方补充0 
                    {
                        _Fill_TrendGraphDispS(&Trend_DispTmp,&tmp_node,cnt,ParaNum,1);
                    }
                }
                //1h
                if(Tmp_Trend_1h.items > 0)//
                {
                    tmp_node = Tmp_Trend_1h.front.next;
                    for(cnt = (pObj->TrendPixelNum - Tmp_Trend_1h.items/4);cnt < pObj->TrendPixelNum;cnt++)//如果显示BUFFER 没有填满，剩下的地方补充0 
                    {
                        _Fill_TrendGraphDispS(&Trend_DispTmp,&tmp_node,cnt,ParaNum,4);
                    }
                }
                break;
            case 12:
                //12h
                if(Tmp_Trend_12h.items > 0)//
                {
                    tmp_node = Tmp_Trend_12h.front.next;
                    for(cnt = (Tmp_Trend_12h.max_item - Tmp_Trend_12h.items);cnt < Tmp_Trend_12h.max_item;cnt++)//如果显示BUFFER 没有填满，剩下的地方补充0 
                    { 
                        _Fill_TrendGraphDispS(&Trend_DispTmp,&tmp_node,cnt,ParaNum,1);
                    }
                }    
                //4h
                if(Tmp_Trend_4h.items > 0)//
                {
                    tmp_node = Tmp_Trend_4h.front.next;
                    //cnt 应该从12个小时结束后，4小时量程中的第一个有效数据开始
                    Max_Addr = Tmp_Trend_12h.max_item + Tmp_Trend_4h.max_item/3;
                    for(cnt = Max_Addr - Tmp_Trend_4h.items/3;cnt < Max_Addr;cnt++)//如果显示BUFFER 没有填满，剩下的地方补充0 
                    {
                        _Fill_TrendGraphDispS(&Trend_DispTmp,&tmp_node,cnt,ParaNum,3);
                    }
                }
                //1h
                if(Tmp_Trend_1h.items > 0)//
                {
                    tmp_node = Tmp_Trend_1h.front.next;
                    for(cnt = (pObj->TrendPixelNum - Tmp_Trend_1h.items/12);cnt < pObj->TrendPixelNum;cnt++)//如果显示BUFFER 没有填满，剩下的地方补充0 
                    {
                        _Fill_TrendGraphDispS(&Trend_DispTmp,&tmp_node,cnt,ParaNum,12);
                    }
                }
                break;
            case 24:
                //24h
                if(Tmp_Trend_24h.items > 0)//
                {
                    tmp_node = Tmp_Trend_24h.front.next;
                    for(cnt = Tmp_Trend_24h.max_item - Tmp_Trend_24h.items;cnt < Tmp_Trend_24h.max_item;cnt++)//如果显示BUFFER 没有填满，剩下的地方补充0 
                    { 
                        _Fill_TrendGraphDispS(&Trend_DispTmp,&tmp_node,cnt,ParaNum,1);
                    }
                }    
                //12h
                if(Tmp_Trend_12h.items > 0)//
                {
                    tmp_node = Tmp_Trend_12h.front.next;
                    //cnt 应该从Tmp_Trend_24h.max_item以后的第一个有效数据开始
                    Max_Addr = Tmp_Trend_24h.max_item + Tmp_Trend_12h.max_item/2;
                    for(cnt = Max_Addr - Tmp_Trend_12h.items/2;cnt < Max_Addr;cnt++)//如果显示BUFFER 没有填满，剩下的地方补充0 
                    { 
                        _Fill_TrendGraphDispS(&Trend_DispTmp,&tmp_node,cnt,ParaNum,2);
                    }
                }        
                //4h
                if(Tmp_Trend_4h.items > 0)//
                {
                    tmp_node = Tmp_Trend_4h.front.next;
                    Max_Addr = Tmp_Trend_24h.max_item + Tmp_Trend_12h.max_item/2 + Tmp_Trend_4h.max_item/6;
                    for(cnt = Max_Addr - Tmp_Trend_4h.items/6;cnt < Max_Addr;cnt++)//如果显示BUFFER 没有填满，剩下的地方补充0 
                    {
                        _Fill_TrendGraphDispS(&Trend_DispTmp,&tmp_node,cnt,ParaNum,6);
                    }
                }   
                //1h
                if(Tmp_Trend_1h.items > 0)//
                {
                    tmp_node = Tmp_Trend_1h.front.next;
                    for(cnt = (pObj->TrendPixelNum - Tmp_Trend_1h.items/24);cnt < pObj->TrendPixelNum;cnt++)//如果显示BUFFER 没有填满，剩下的地方补充0 
                    {
                        _Fill_TrendGraphDispS(&Trend_DispTmp,&tmp_node,cnt,ParaNum,24);
                    }
                }
                break;
#if (TREND_INFO_72H)
            case 48:
                if(Tmp_Trend_48h.items > 0)//
                {
                    tmp_node = Tmp_Trend_48h.front.next;
                    for(cnt = (Tmp_Trend_48h.max_item - Tmp_Trend_48h.items);cnt < Tmp_Trend_48h.max_item;cnt++)//如果显示BUFFER 没有填满，剩下的地方补充0 
                    { 
                        _Fill_TrendGraphDispS(&Trend_DispTmp,&tmp_node,cnt,ParaNum,1);
                    }
                }                             
                if(Tmp_Trend_24h.items > 0)//
                {
                    tmp_node = Tmp_Trend_24h.front.next;
                    Max_Addr = Tmp_Trend_48h.max_item + Tmp_Trend_24h.max_item/2;
                    for(cnt = Max_Addr - Tmp_Trend_24h.items/2;cnt < Max_Addr;cnt++)//如果显示BUFFER 没有填满，剩下的地方补充0 
                    { 
                        _Fill_TrendGraphDispS(&Trend_DispTmp,&tmp_node,cnt,ParaNum,2);
                    }
                }                             
                if(Tmp_Trend_12h.items > 0)//
                {
                    tmp_node = Tmp_Trend_12h.front.next;
                    Max_Addr = Tmp_Trend_48h.max_item + Tmp_Trend_24h.max_item/2 + Tmp_Trend_12h.max_item/4;
                    for(cnt = Max_Addr - Tmp_Trend_12h.items/4;cnt < Max_Addr;cnt++)//如果显示BUFFER 没有填满，剩下的地方补充0 
                    { 
                        _Fill_TrendGraphDispS(&Trend_DispTmp,&tmp_node,cnt,ParaNum,4);
                    }
                }                             
                if(Tmp_Trend_4h.items > 0)//
                {
                    tmp_node = Tmp_Trend_4h.front.next;
                    Max_Addr = Tmp_Trend_48h.max_item + Tmp_Trend_24h.max_item/2 + Tmp_Trend_12h.max_item/4 + Tmp_Trend_4h.max_item/12;
                    for(cnt = Max_Addr - Tmp_Trend_4h.items/12;cnt < Max_Addr;cnt++)//如果显示BUFFER 没有填满，剩下的地方补充0 
                    {
                        _Fill_TrendGraphDispS(&Trend_DispTmp,&tmp_node,cnt,ParaNum,12);
                    }
                }            
                if(Tmp_Trend_1h.items > 0)//
                {
                    tmp_node = Tmp_Trend_1h.front.next;
                    for(cnt = (pObj->TrendPixelNum - (int)(Tmp_Trend_1h.items/48.0+0.5));cnt < pObj->TrendPixelNum;cnt++)//如果显示BUFFER 没有填满，剩下的地方补充0 
                    {
                        _Fill_TrendGraphDispS(&Trend_DispTmp,&tmp_node,cnt,ParaNum,48);
                    }
                }
                break;
            case 72:
                if(Tmp_Trend_72h.items > 0)//
                {
                    tmp_node = Tmp_Trend_72h.front.next;
                    for(cnt = (Tmp_Trend_72h.max_item - Tmp_Trend_72h.items);cnt < Tmp_Trend_72h.max_item;cnt++)//如果显示BUFFER 没有填满，剩下的地方补充0 
                    { 
                        _Fill_TrendGraphDispS(&Trend_DispTmp,&tmp_node,cnt,ParaNum,1);
                    }
                }         
                if(Tmp_Trend_48h.items > 0)//
                {
                    tmp_node = Tmp_Trend_48h.front.next;
                    Max_Addr = Tmp_Trend_72h.max_item + (Tmp_Trend_48h.max_item*2)/3;
                    for(cnt = Max_Addr - (Tmp_Trend_48h.items*2)/3;cnt < Max_Addr;cnt++)//如果显示BUFFER 没有填满，剩下的地方补充0 
                    { 
                        if(cnt_48h%2 == 1)//跳过中间的一个点
                        {
                            _Fill_TrendGraphDispS(&Trend_DispTmp,&tmp_node,cnt,ParaNum,2);                            
                        }
                        else 
                        {
                            _Fill_TrendGraphDispS(&Trend_DispTmp,&tmp_node,cnt,ParaNum,1);                            
                        }
                        cnt_48h++;
                    }
                }                             
                if(Tmp_Trend_24h.items > 0)//
                {
                    tmp_node = Tmp_Trend_24h.front.next;
                    Max_Addr = Tmp_Trend_72h.max_item + (Tmp_Trend_48h.max_item*2)/3 + Tmp_Trend_24h.max_item/3;
                    for(cnt = Max_Addr - Tmp_Trend_24h.items/3;cnt < Max_Addr;cnt++)//如果显示BUFFER 没有填满，剩下的地方补充0 
                    { 
                        _Fill_TrendGraphDispS(&Trend_DispTmp,&tmp_node,cnt,ParaNum,3);
                    }
                }                             
                if(Tmp_Trend_12h.items > 0)//
                {
                    tmp_node = Tmp_Trend_12h.front.next;
                    Max_Addr = Tmp_Trend_72h.max_item + (Tmp_Trend_48h.max_item*2)/3 + Tmp_Trend_24h.max_item/3 + Tmp_Trend_12h.max_item/6;
                    for(cnt = Max_Addr - Tmp_Trend_12h.items/6;cnt < Max_Addr;cnt++)//如果显示BUFFER 没有填满，剩下的地方补充0 
                    { 
                        _Fill_TrendGraphDispS(&Trend_DispTmp,&tmp_node,cnt,ParaNum,6);
                    }
                }                             
                if(Tmp_Trend_4h.items > 0)//
                {
                    tmp_node = Tmp_Trend_4h.front.next;
                    Max_Addr = Tmp_Trend_72h.max_item + (Tmp_Trend_48h.max_item*2)/3 + Tmp_Trend_24h.max_item/3 + Tmp_Trend_12h.max_item/6 + Tmp_Trend_4h.max_item/18;
                    for(cnt = Max_Addr - Tmp_Trend_4h.items/18;cnt < Max_Addr;cnt++)//如果显示BUFFER 没有填满，剩下的地方补充0 
                    {
                        _Fill_TrendGraphDispS(&Trend_DispTmp,&tmp_node,cnt,ParaNum,18);
                    }
                }            
                if(Tmp_Trend_1h.items > 0)//
                {
                    tmp_node = Tmp_Trend_1h.front.next;
                    for(cnt = (pObj->TrendPixelNum - Tmp_Trend_1h.items/72);cnt < pObj->TrendPixelNum;cnt++)//如果显示BUFFER 没有填满，剩下的地方补充0 
                    {
                        _Fill_TrendGraphDispS(&Trend_DispTmp,&tmp_node,cnt,ParaNum,72);
                    }
                }
                break;
#endif
            default:
                break;
            }

        }
        WM_UNLOCK();
    }
    if(ParaNum >= INDEX_START_16U)//单双字节区分
    {
        TracingTrend_FullBuffer(hObj,Index,Trend_DispTmp.tmp_short);
    }
    else 
    {
        TracingTrend_FullBuffer(hObj,Index,Trend_DispTmp.tmp_char);
    }
    return 0;
}

//填充趋势表buffer

/*
*  根据传入的参数索引号，初始化趋势图
*  ParaIndex :当前趋势的参数索引号码
*  TimeRange :当前趋势时间范围
*  TimeSign  :是否显示时间轴标志
*  Init_Flag :初始化标志   0:不初始化，直接更新波形   1：初始化
*/
void TrendGraph_Init(TracingCurve_Handle hObj,int ParaIndex,int TimeRange,const char TimeSign,int Init_Flag)
{
    GUI_COLOR First_Color = TREND_CURVECOLOR_1,Second_Color = TREND_CURVECOLOR_2;
    if(Init_Flag)
    {
        TracingTrend_FreeBffer(hObj);
    }
    switch(ParaIndex)
    {
    case PPEAK:
    case PMEAN:
        if(Init_Flag)
        {
            TracingTrend_SetInfo(hObj,TimeSign,TimeRange,ParaMinLimit[PPEAK] ,ParaMaxLimit[PPEAK],ParaFloatLimit[PPEAK],sizeof(char),TREND_BUFFERSIZE,TrendParaName[PPEAK],First_Color);
            TracingTrend_AddPara(hObj,1,TrendParaName[PMEAN],Second_Color);   
        }
        Fill_TrendGraphDispBuffer(hObj,TrendParaName[PPEAK]);
        Fill_TrendGraphDispBuffer(hObj,TrendParaName[PMEAN]);
        break;
    case PEEP:
        if(Init_Flag)
        {
            TracingTrend_SetInfo(hObj,TimeSign,TimeRange,ParaMinLimit[PEEP] ,ParaMaxLimit[PEEP],ParaFloatLimit[PEEP],sizeof(char),TREND_BUFFERSIZE,TrendParaName[PEEP],First_Color);
        }
        Fill_TrendGraphDispBuffer(hObj,TrendParaName[PEEP]);
        break;
    case FTOTAL:
    case FSPN:
        if(Init_Flag)
        {
            TracingTrend_SetInfo(hObj,TimeSign,TimeRange,ParaMinLimit[FTOTAL],ParaMaxLimit[FTOTAL],ParaFloatLimit[FTOTAL],sizeof(char),TREND_BUFFERSIZE,TrendParaName[FTOTAL],First_Color);
            TracingTrend_AddPara(hObj,1,TrendParaName[FSPN],Second_Color);   
        }
        Fill_TrendGraphDispBuffer(hObj,TrendParaName[FTOTAL]);
        Fill_TrendGraphDispBuffer(hObj,TrendParaName[FSPN]);
        break;
    case FIO2:
        if(Init_Flag)
        {
            TracingTrend_SetInfo(hObj,TimeSign,TimeRange,ParaMinLimit[FIO2] ,ParaMaxLimit[FIO2],ParaFloatLimit[FIO2],sizeof(char),TREND_BUFFERSIZE,TrendParaName[FIO2],First_Color);
        }
        Fill_TrendGraphDispBuffer(hObj,TrendParaName[FIO2]);
        break;
    case ETCO2:
	case FICO2://ylk add 20181121
         if(Init_Flag)
        {
            switch(ETCO2Unit)//0:%     1:mmHg      2:kPa
            {
            case 0:
                TracingTrend_SetInfo(hObj,TimeSign,TimeRange,ParaMinLimit[ETCO2] ,ParaMaxLimit[ETCO2],1,sizeof(char),TREND_BUFFERSIZE,TrendParaName[ETCO2],First_Color);
				TracingTrend_AddPara(hObj,1,TrendParaName[FICO2],Second_Color);               
				break;
            case 1:
                TracingTrend_SetInfo(hObj,TimeSign,TimeRange,0,113,0,sizeof(char),TREND_BUFFERSIZE,TrendParaName[ETCO2],First_Color);
				TracingTrend_AddPara(hObj,1,TrendParaName[FICO2],Second_Color);
                break;
            case 2:
                TracingTrend_SetInfo(hObj,TimeSign,TimeRange,ParaMinLimit[ETCO2] ,ParaMaxLimit[ETCO2],1,sizeof(char),TREND_BUFFERSIZE,TrendParaName[ETCO2],First_Color);
				TracingTrend_AddPara(hObj,1,TrendParaName[FICO2],Second_Color);               
				break;
            }
        }
        Fill_TrendGraphDispBuffer(hObj,TrendParaName[ETCO2]);
		Fill_TrendGraphDispBuffer(hObj,TrendParaName[FICO2]);
        break;
	case ETN2O:
	case FIN2O:
		if(Init_Flag)
		{
			TracingTrend_SetInfo(hObj,TimeSign,TimeRange,ParaMinLimit[ETN2O] ,ParaMaxLimit[ETN2O],0,sizeof(char),TREND_BUFFERSIZE,TrendParaName[ETN2O],First_Color);
			TracingTrend_AddPara(hObj,1,TrendParaName[FIN2O],Second_Color);   
		}
		Fill_TrendGraphDispBuffer(hObj,TrendParaName[ETN2O]);
		Fill_TrendGraphDispBuffer(hObj,TrendParaName[FIN2O]);
		break;
#ifdef SPO2_SWITCH
    case SPO2:
        if(Init_Flag)
        {
            TracingTrend_SetInfo(hObj,TimeSign,TimeRange,ParaMinLimit[SPO2],ParaMaxLimit[SPO2],ParaFloatLimit[SPO2],sizeof(char),TREND_BUFFERSIZE,TrendParaName[SPO2],First_Color);
        }
        Fill_TrendGraphDispBuffer(hObj,TrendParaName[SPO2]);
        break;
    case PULSE:
        if(Init_Flag)
        {
            TracingTrend_SetInfo(hObj,TimeSign,TimeRange,ParaMinLimit[PULSE] ,ParaMaxLimit[PULSE],ParaFloatLimit[PULSE],sizeof(char),TREND_BUFFERSIZE,TrendParaName[PULSE],First_Color);
        }
        Fill_TrendGraphDispBuffer(hObj,TrendParaName[PULSE]);
        break;
#endif
    case VTE:
    case VTI:
        if(Init_Flag)
        {
            TracingTrend_SetInfo(hObj,TimeSign,TimeRange,ParaMinLimit[VTE] ,ParaMaxLimit[VTE],ParaFloatLimit[VTE],sizeof(short),TREND_BUFFERSIZE,TrendParaName[VTE],First_Color);
            TracingTrend_AddPara(hObj,1,TrendParaName[VTI],Second_Color);   
        }
        Fill_TrendGraphDispBuffer(hObj,TrendParaName[VTE]);
        Fill_TrendGraphDispBuffer(hObj,TrendParaName[VTI]);
        break;
    case RST:
        if(Init_Flag)
        {
            TracingTrend_SetInfo(hObj,TimeSign,TimeRange,ParaMinLimit[RST] ,ParaMaxLimit[RST],ParaFloatLimit[RST],sizeof(short),TREND_BUFFERSIZE,TrendParaName[RST],First_Color);
        }
        Fill_TrendGraphDispBuffer(hObj,TrendParaName[RST]);
        break;
    case CDYN:
        if(Init_Flag)
        {
            TracingTrend_SetInfo(hObj,TimeSign,TimeRange,ParaMinLimit[CDYN] ,ParaMaxLimit[CDYN],ParaFloatLimit[CDYN],sizeof(short),TREND_BUFFERSIZE,TrendParaName[CDYN],First_Color);
        }
        Fill_TrendGraphDispBuffer(hObj,TrendParaName[CDYN]);
        break;
    case MV:
    case MVSPN:
        if(Init_Flag)
        {
            TracingTrend_SetInfo(hObj,TimeSign,TimeRange,ParaMinLimit[MV] ,ParaMaxLimit[MV],ParaFloatLimit[MV],sizeof(short),TREND_BUFFERSIZE,TrendParaName[MV],First_Color);
            TracingTrend_AddPara(hObj,1,TrendParaName[MVSPN],Second_Color);   
        }
        Fill_TrendGraphDispBuffer(hObj,TrendParaName[MV]);
        Fill_TrendGraphDispBuffer(hObj,TrendParaName[MVSPN]);
        break;
	case ETAA1://ylk add 20181121
	case FIAA1:
		if(Init_Flag)
		{
			TrendParaName[ETAA1] = Sys_Lib_GetDispStr(MonitorName1_AA[AA_all_data.AA_detail.AA_AA1.ID*2]);
			TrendParaName[FIAA1] = Sys_Lib_GetDispStr(MonitorName1_AA[AA_all_data.AA_detail.AA_AA1.ID*2+1]);
			TracingTrend_SetAAFlag(hObj,1);
			TracingTrend_SetInfo(hObj,TimeSign,TimeRange,ParaMinLimit[ETAA1] ,ParaMaxLimit[ETAA1],1,sizeof(short),TREND_BUFFERSIZE,TrendParaName[ETAA1],First_Color);
			TracingTrend_AddPara(hObj,1,TrendParaName[FIAA1],Second_Color);   
		}
		Fill_TrendGraphDispBuffer(hObj,TrendParaName[ETAA1]);
		Fill_TrendGraphDispBuffer(hObj,TrendParaName[FIAA1]);
		break;
	case ETAA2:
	case FIAA2:
		if(Init_Flag)
		{
			TrendParaName[ETAA2] = Sys_Lib_GetDispStr(MonitorName2_AA[AA_all_data.AA_detail.AA_AA2.ID*2]);
			TrendParaName[FIAA2] = Sys_Lib_GetDispStr(MonitorName2_AA[AA_all_data.AA_detail.AA_AA2.ID*2+1]);
			TracingTrend_SetAAFlag(hObj,2);
			TracingTrend_SetInfo(hObj,TimeSign,TimeRange,ParaMinLimit[ETAA2] ,ParaMaxLimit[ETAA2],1,sizeof(short),TREND_BUFFERSIZE,TrendParaName[ETAA2],First_Color);
			TracingTrend_AddPara(hObj,1,TrendParaName[FIAA2],Second_Color);   
		}
		Fill_TrendGraphDispBuffer(hObj,TrendParaName[ETAA2]);
		Fill_TrendGraphDispBuffer(hObj,TrendParaName[FIAA2]);
		break;
	case MAC_:
		if(Init_Flag)
		{
			TracingTrend_SetInfo(hObj,TimeSign,TimeRange,ParaMinLimit[MAC_] ,ParaMaxLimit[MAC_],2,sizeof(short),TREND_BUFFERSIZE,TrendParaName[MAC_],First_Color);
		}
		Fill_TrendGraphDispBuffer(hObj,TrendParaName[MAC_]);
		break;
    }
}

//////////////////////////////////////////////////////////////////////////////////


//专用于Full_TrendTableDispBuffer函数中（为了使代码简单化）
static int _Fill_TrendTableDispS(short *Trend_DispTmp,Node **tmp_node,int cnt,int ParaNum,int next_num)
{
    if(tmp_node == NULL)
    {
        return 1;
    }
    
    Trend_DispTmp[cnt]  = (*tmp_node)->item.Para[ParaNum];
    *tmp_node = _GetPerXNoteP(*tmp_node,next_num);
    return 0;
}

/*填充趋势表一列的显示buffer*/
int Fill_TrendTableDispBuffer(TrendTable_Handle hObj,const char *ParaName,int Column)
{
    short Trend_DispTmp[TREND_BUFFERSIZE] = {0};
    int cnt,Index = 10,ParaNum = 10;
    int TimeRange = 0,tmp_num = 0;
    static Node *tmp_node;
    static char cnt_48h = 0;
    int Start_Addr = 0;
    if (hObj)
    {
        TrendTable_Obj* pObj;
        WM_LOCK();
        pObj = TrendTable_H2P(hObj);
        TimeRange = (int)pObj->TimeRange;
        if((Column >= 1)&&(Column <= pObj->ColumnNumber))//判断索引号是否在合理范围内
        {
            //判断该变量在存储时的位置
            for(cnt = 0;cnt < TREND_PARA_NUM;cnt++)
            {
                if(!strcmp(TrendParaName[cnt],ParaName))
                {
                    ParaNum = cnt;
                    if((!strcmp(TrendParaName[MV],ParaName))||(!strcmp(TrendParaName[MVSPN],ParaName)))
                    {
                        TrendTable_SetDecimal(hObj,Column,1);
                    }
                    else
                    {
                        TrendTable_SetDecimal(hObj,Column,0);
                    }
                    break;
                }
            }

            switch(TimeRange)
            {
            case 1:
                tmp_node = Tmp_Trend_1h.front.prev;
                for(cnt = 0;cnt < Tmp_Trend_1h.items;cnt++)//如果显示BUFFER 没有填满，剩下的地方补充0 
                {
                    _Fill_TrendTableDispS(Trend_DispTmp,&tmp_node,cnt,ParaNum,1);
                }
                break;
            case 4:
                if(Tmp_Trend_1h.items > 0)//
                {
                    tmp_node = Tmp_Trend_1h.front.prev;
                    for(cnt = 0;cnt < Tmp_Trend_1h.items/4;cnt++)//如果显示BUFFER 没有填满，剩下的地方补充0 
                    {
                        _Fill_TrendTableDispS(Trend_DispTmp,&tmp_node,cnt,ParaNum,4);
                    }
                }          
                if(Tmp_Trend_4h.items > 0)//
                {
                    tmp_node = Tmp_Trend_4h.front.prev;
                    Start_Addr = pObj->ItemNumber - Tmp_Trend_4h.max_item;
                    for(cnt = Start_Addr;cnt < Start_Addr+Tmp_Trend_4h.items;cnt++)//如果显示BUFFER 没有填满，剩下的地方补充0 
                    {
                        _Fill_TrendTableDispS(Trend_DispTmp,&tmp_node,cnt,ParaNum,1);
                    }
                }

                break;
            case 12:
                if(Tmp_Trend_1h.items > 0)//
                {
                    tmp_node = Tmp_Trend_1h.front.prev;
                    for(cnt = 0;cnt < Tmp_Trend_1h.items/12;cnt++)//如果显示BUFFER 没有填满，剩下的地方补充0 
                    {
                        _Fill_TrendTableDispS(Trend_DispTmp,&tmp_node,cnt,ParaNum,12);
                    }
                }
                if(Tmp_Trend_4h.items > 0)//
                {
                    tmp_node = Tmp_Trend_4h.front.prev;
                    //cnt 应该从12个小时结束后，4小时量程中的第一个有效数据开始
                    Start_Addr = Tmp_Trend_1h.max_item/12;
                    for(cnt = Start_Addr;cnt < Start_Addr + Tmp_Trend_4h.items/3;cnt++)//如果显示BUFFER 没有填满，剩下的地方补充0 
                    {
                        _Fill_TrendTableDispS(Trend_DispTmp,&tmp_node,cnt,ParaNum,3);
                    }
                }
                if(Tmp_Trend_12h.items > 0)//
                {
                    tmp_node = Tmp_Trend_12h.front.prev;
                    Start_Addr += Tmp_Trend_4h.max_item/3;
                    for(cnt = Start_Addr;cnt < Start_Addr+Tmp_Trend_12h.items;cnt++)//如果显示BUFFER 没有填满，剩下的地方补充0 
                    { 
                        _Fill_TrendTableDispS(Trend_DispTmp,&tmp_node,cnt,ParaNum,1);
                    }
                }                             
                break;
            case 24:
                if(Tmp_Trend_1h.items > 0)//
                {
                    tmp_node = Tmp_Trend_1h.front.prev;
                    for(cnt = 0;cnt < Tmp_Trend_1h.items/24;cnt++)//如果显示BUFFER 没有填满，剩下的地方补充0 
                    {
                        _Fill_TrendTableDispS(Trend_DispTmp,&tmp_node,cnt,ParaNum,24);
                    }
                }
                if(Tmp_Trend_4h.items > 0)//
                {
                    tmp_node = Tmp_Trend_4h.front.prev;
                    Start_Addr = Tmp_Trend_1h.max_item/24;
                    for(cnt = Start_Addr;cnt < Start_Addr+Tmp_Trend_4h.items/6;cnt++)//如果显示BUFFER 没有填满，剩下的地方补充0 
                    {
                        _Fill_TrendTableDispS(Trend_DispTmp,&tmp_node,cnt,ParaNum,6);
                    }
                }            
                if(Tmp_Trend_12h.items > 0)//
                {
                    tmp_node = Tmp_Trend_12h.front.prev;
                    //cnt 应该从Tmp_Trend_24h.max_item以后的第一个有效数据开始
                    Start_Addr += Tmp_Trend_4h.max_item/6;
                    for(cnt = Start_Addr;cnt < Start_Addr+Tmp_Trend_12h.items/2;cnt++)//如果显示BUFFER 没有填满，剩下的地方补充0 
                    { 
                        _Fill_TrendTableDispS(Trend_DispTmp,&tmp_node,cnt,ParaNum,2);
                    }
                }                             
                if(Tmp_Trend_24h.items > 0)//
                {
                    tmp_node = Tmp_Trend_24h.front.prev;
                    Start_Addr += Tmp_Trend_12h.max_item/2;
                    for(cnt = Start_Addr;cnt < Start_Addr+Tmp_Trend_24h.items;cnt++)//如果显示BUFFER 没有填满，剩下的地方补充0 
                    { 
                        _Fill_TrendTableDispS(Trend_DispTmp,&tmp_node,cnt,ParaNum,1);
                    }
                }                             
                break;
#if (TREND_INFO_72H)
            case 48:
                if(Tmp_Trend_1h.items > 0)//
                {
                    tmp_node = Tmp_Trend_1h.front.prev;
                    for(cnt = 0;cnt < (int)(Tmp_Trend_1h.items/48.0+0.5);cnt++)//如果显示BUFFER 没有填满，剩下的地方补充0 
                    {
                        _Fill_TrendTableDispS(Trend_DispTmp,&tmp_node,cnt,ParaNum,48);
                    }
                }
                if(Tmp_Trend_4h.items > 0)//
                {
                    tmp_node = Tmp_Trend_4h.front.prev;
                    Start_Addr = Tmp_Trend_1h.max_item/48;
                    for(cnt = Start_Addr;cnt < Start_Addr+Tmp_Trend_4h.items/12;cnt++)//如果显示BUFFER 没有填满，剩下的地方补充0 
                    {
                        _Fill_TrendTableDispS(Trend_DispTmp,&tmp_node,cnt,ParaNum,12);
                    }
                }            
                if(Tmp_Trend_12h.items > 0)//
                {
                    tmp_node = Tmp_Trend_12h.front.prev;
                    Start_Addr += Tmp_Trend_4h.max_item/12;
                    for(cnt = Start_Addr;cnt < Start_Addr+Tmp_Trend_12h.items/4;cnt++)//如果显示BUFFER 没有填满，剩下的地方补充0 
                    { 
                        _Fill_TrendTableDispS(Trend_DispTmp,&tmp_node,cnt,ParaNum,4);
                    }
                }                             
                if(Tmp_Trend_24h.items > 0)//
                {
                    tmp_node = Tmp_Trend_24h.front.prev;
                    Start_Addr += Tmp_Trend_12h.max_item/4;
                    for(cnt = Start_Addr;cnt < Start_Addr+Tmp_Trend_24h.items/2;cnt++)//如果显示BUFFER 没有填满，剩下的地方补充0 
                    { 
                        _Fill_TrendTableDispS(Trend_DispTmp,&tmp_node,cnt,ParaNum,2);
                    }
                }                             
                if(Tmp_Trend_48h.items > 0)//
                {
                    tmp_node = Tmp_Trend_48h.front.prev;
                    Start_Addr += Tmp_Trend_24h.max_item/2;
                    for(cnt = Start_Addr;cnt < Start_Addr+Tmp_Trend_48h.items;cnt++)//如果显示BUFFER 没有填满，剩下的地方补充0 
                    { 
                        _Fill_TrendTableDispS(Trend_DispTmp,&tmp_node,cnt,ParaNum,1);
                    }
                }                             
                break;
            case 72:
                if(Tmp_Trend_1h.items > 0)//
                {
                    tmp_node = Tmp_Trend_1h.front.prev;
                    for(cnt = 0;cnt < Tmp_Trend_1h.items/72;cnt++)//如果显示BUFFER 没有填满，剩下的地方补充0 
                    {
                        _Fill_TrendTableDispS(Trend_DispTmp,&tmp_node,cnt,ParaNum,72);
                    }
                }
                if(Tmp_Trend_4h.items > 0)//
                {
                    tmp_node = Tmp_Trend_4h.front.prev;
                    Start_Addr = Tmp_Trend_1h.max_item/72;
                    for(cnt = Start_Addr;cnt < Start_Addr+Tmp_Trend_4h.items/18;cnt++)//如果显示BUFFER 没有填满，剩下的地方补充0 
                    {
                        _Fill_TrendTableDispS(Trend_DispTmp,&tmp_node,cnt,ParaNum,18);
                    }
                }            
                if(Tmp_Trend_12h.items > 0)//
                {
                    tmp_node = Tmp_Trend_12h.front.prev;
                    Start_Addr += Tmp_Trend_4h.max_item/18;
                    for(cnt = Start_Addr;cnt < Start_Addr+Tmp_Trend_12h.items/6;cnt++)//如果显示BUFFER 没有填满，剩下的地方补充0 
                    { 
                        _Fill_TrendTableDispS(Trend_DispTmp,&tmp_node,cnt,ParaNum,6);
                    }
                }                             
                if(Tmp_Trend_24h.items > 0)//
                {
                    tmp_node = Tmp_Trend_24h.front.prev;
                    Start_Addr += Tmp_Trend_12h.max_item/6;
                    for(cnt = Start_Addr;cnt < Start_Addr+Tmp_Trend_24h.items/3;cnt++)//如果显示BUFFER 没有填满，剩下的地方补充0 
                    { 
                        _Fill_TrendTableDispS(Trend_DispTmp,&tmp_node,cnt,ParaNum,3);
                    }
                }                             
                if(Tmp_Trend_48h.items > 0)//
                {
                    tmp_node = Tmp_Trend_48h.front.prev;
                    Start_Addr += Tmp_Trend_24h.max_item/6;
                    for(cnt = Start_Addr;cnt < Start_Addr+(Tmp_Trend_48h.items*2)/3;cnt++)//如果显示BUFFER 没有填满，剩下的地方补充0 
                    { 
                        if(cnt_48h%2 == 1)//跳过中间的一个点
                        {
                            _Fill_TrendTableDispS(Trend_DispTmp,&tmp_node,cnt,ParaNum,2);                            
                        }
                        else 
                        {
                            _Fill_TrendTableDispS(Trend_DispTmp,&tmp_node,cnt,ParaNum,1);                            
                        }
                        cnt_48h++;
                    }
                }                             
                if(Tmp_Trend_72h.items > 0)//
                {
                    tmp_node = Tmp_Trend_72h.front.prev;
                    Start_Addr += (Tmp_Trend_48h.max_item*2)/3;
                    for(cnt = Start_Addr;cnt < Start_Addr+Tmp_Trend_72h.items;cnt++)//如果显示BUFFER 没有填满，剩下的地方补充0 
                    { 
                        _Fill_TrendTableDispS(Trend_DispTmp,&tmp_node,cnt,ParaNum,1);
                    }
                }         
                break;
#endif
            default:
                break;
            }

        }
        WM_UNLOCK();
    }
    
    TrendTable_FillBuffer(hObj,Trend_DispTmp,Column);
    return 0;
}

void InitTrendName(void)
{
    TrendParaName[PPEAK]   = Sys_Lib_GetDispStr(DP_PPEAK);
    TrendParaName[PMEAN]   = Sys_Lib_GetDispStr(DP_PMEAN);
    TrendParaName[PEEP]    = Sys_Lib_GetDispStr(DP_PEEP);
    TrendParaName[FTOTAL]  = Sys_Lib_GetDispStr(DP_FTOTAL);
    TrendParaName[FSPN]    = Sys_Lib_GetDispStr(DP_FSPN);
    TrendParaName[FIO2]    = Sys_Lib_GetDispStr(DP_FIO2);
    TrendParaName[ETCO2]   = Sys_Lib_GetDispStr(DP_ETCO2);
	TrendParaName[FICO2]   = Sys_Lib_GetDispStr(DP_FICO2);
	TrendParaName[ETN2O]   = Sys_Lib_GetDispStr(DP_ETN2O);
	TrendParaName[FIN2O]   = Sys_Lib_GetDispStr(DP_FIN2O);
#ifdef SPO2_SWITCH
    TrendParaName[SPO2]    = Sys_Lib_GetDispStr(DP_SPO2);
    TrendParaName[PULSE]   = Sys_Lib_GetDispStr(DP_PULSE);
#endif

    TrendParaName[VTE]     = Sys_Lib_GetDispStr(DP_VTE);;
    TrendParaName[VTI]     = Sys_Lib_GetDispStr(DP_VTI);
    TrendParaName[RST]     = Sys_Lib_GetDispStr(DP_RST);
    TrendParaName[CDYN]    = Sys_Lib_GetDispStr(DP_CDYN);
    TrendParaName[MV]      = Sys_Lib_GetDispStr(DP_MV);
    TrendParaName[MVSPN]   = Sys_Lib_GetDispStr(DP_MVSPN);
	TrendParaName[ETAA1]   = Sys_Lib_GetDispStr(DP_ETAA1);
	TrendParaName[FIAA1]   = Sys_Lib_GetDispStr(DP_FIAA1);
	TrendParaName[ETAA2]   = Sys_Lib_GetDispStr(DP_ETAA2);
	TrendParaName[FIAA2]   = Sys_Lib_GetDispStr(DP_FIAA2);
	TrendParaName[MAC_]    = Sys_Lib_GetDispStr(DP_MAC);
}
/*
*   根据时间范围得到时间的索引值
*/
static int _GetTimeIndex(int TimeRange)
{
    int ret;
    switch(TimeRange)
    {
    case 1:
        ret = 0;
        break;
    case 4:
        ret = 1;
        break;
    case 12:
        ret = 2;
        break;
    case 24:
        ret = 3;
        break;
    case 48:
        ret = 4;
        break;
    case 72:
        ret = 5;
        break;
    default:
        ret = 1;
        break;
    }
    return ret;
}

#if (TREND_GRAPH_ON)
static const GUI_WIDGET_CREATE_INFO _aDialogCreateTrendGraph[] =
{
    { WINDOW_CreateIndirect,    "", 0,                 0,   0, 625+33,230, FRAMEWIN_CF_ACTIVE },  
    { TracingCurve_CreateIndirect, "",GUI_ID_CURVE0,     105,  26,  495,  84, 0},
    { TracingCurve_CreateIndirect, "",GUI_ID_CURVE1,     105,  98,  495,  85, 0},
    { BUTTON_CreateIndirect,    "",   GUI_ID_BUTTON0,    45, 16, 51,32},//TimeRange
    { BUTTON_CreateIndirect,    "",   GUI_ID_BUTTON1,    45, 52,51,64},//PARA 1
    { BUTTON_CreateIndirect,    "",   GUI_ID_BUTTON2,    45,120,51,65},//PARA 2
    { BUTTON_CreateIndirect,    "",   GUI_ID_BUTTON3,    444,2,80,32},//PARA 2
    { TEXT_CreateIndirect,      "",   GUI_ID_TEXT0,      94,2,350,32,TEXT_CF_HCENTER|TEXT_CF_VCENTER},
    { KEYBOARD_CreateIndirect,  "",   GUI_ID_KEYBOARD0,  84,45,300,130},
};


static void _cbDialogTrendGraph(WM_MESSAGE * pMsg)//当趋势图被删除时应该判断当前的按键控件是否被删除，如果没有则删除当前按键控件
{
    WM_HWIN hDlg,hWin;
    int Id,NCode,cnt;
    //test
    static char num = 0;
    char ButtonText[10] = {0};

    hDlg = pMsg->hWin;
    switch (pMsg->MsgId)
    {
    case WM_INIT_DIALOG:
        WINDOW_SetBkColor(hDlg,GUI_BLACK+1);
        //初始化时间范围按钮
        hWin = WM_GetDialogItem(pMsg->hWin,GUI_ID_BUTTON0);
		BUTTON_SetTextWrapMode(hWin, GUI_WRAPMODE_WORD);
		TrendPara_Info.Graph_TimeRange = RTEEPROMread(TREND_TIMERANGE);
		for(cnt = 0;cnt < TREND_TIME_NUM;cnt++)
		{
			if(TrendTimeRange_Index[cnt] == TrendPara_Info.Graph_TimeRange)
			{
				break;
			}
		}
		if(cnt == TREND_TIME_NUM)
		{
			TrendPara_Info.Graph_TimeRange = TrendTimeRange_Index[0];
		}

		NCode = TrendPara_Info.Graph_TimeRange;
		sprintf(ButtonText,"%d %s",NCode,Sys_Lib_GetDispStr(DP_HOUR));

        BUTTON_SetText(hWin,ButtonText);  
        BUTTON_SetBkColor(hWin,0,0xD05356);
        BUTTON_SetBkColor(hWin,3,GUI_BLACK);

		if(LanguageType == Disp_Lg_Chinese)
		{
			BUTTON_SetFont(hWin,&GUI_Font_15);
		}
		else
		{
			BUTTON_SetFont(hWin,&GUI_Font_16);
		}
		BUTTON_SetReleased(hWin,1);

        //初始化功能按钮
        hWin = WM_GetDialogItem(pMsg->hWin,GUI_ID_BUTTON1);
        BUTTON_SetText(hWin,TrendParaName[TrendPara_Info.Graph_Index[0]]);  
        BUTTON_SetBkColor(hWin,0,0xD05356);
        BUTTON_SetBkColor(hWin,3,GUI_BLACK);

		if(LanguageType == Disp_Lg_Chinese)
		{
			BUTTON_SetFont(hWin,&GUI_Font_15);
		}
		else
		{
			BUTTON_SetFont(hWin,&GUI_Font_16);
		}

        BUTTON_SetReleased(hWin,1);

        hWin = WM_GetDialogItem(pMsg->hWin,GUI_ID_BUTTON2);
        BUTTON_SetText(hWin,TrendParaName[TrendPara_Info.Graph_Index[1]]);  
        BUTTON_SetBkColor(hWin,0,0xD05356);
        BUTTON_SetBkColor(hWin,3,GUI_BLACK);
        //BUTTON_SetFont(hWin,&GUI_Font_15);

		if(LanguageType == Disp_Lg_Chinese)
		{
			BUTTON_SetFont(hWin,&GUI_Font_15);
		}
		else
		{
			BUTTON_SetFont(hWin,&GUI_Font_16);
		}
        BUTTON_SetReleased(hWin,1);


        hWin = WM_GetDialogItem(pMsg->hWin,GUI_ID_BUTTON3);
        BUTTON_SetText(hWin,"Clear Trend");  
        BUTTON_SetBkColor(hWin,0,0xD05356);
        BUTTON_SetBkColor(hWin,3,GUI_BLACK);
        BUTTON_SetFont(hWin,&GUI_Font_16);
        if(StandbyState != 0xFF)
        {
            WM_DisableWindow(hWin);
        }
        else
        {
            WM_EnableWindow(hWin);
        }
        WM_HideWindow(hWin);

        hWin = WM_GetDialogItem(pMsg->hWin,GUI_ID_TEXT0);
        TEXT_SetText(hWin,"Successful,Please restart the trend menu!");  
        TEXT_SetTextColor(hWin,0xD05356);
        TEXT_SetFont(hWin,&GUI_Font_16);
        WM_HideWindow(hWin);

        FunctionKeyPressedResume(hDlg,0);

        //隐藏键盘
        WM_HideWindow(WM_GetDialogItem(hDlg, GUI_ID_KEYBOARD0));

        WM_ShowWindow(WM_GetDialogItem(hDlg, GUI_ID_CURVE0));
        WM_ShowWindow(WM_GetDialogItem(hDlg, GUI_ID_CURVE1));

        WM_SetFocus(WM_GetDialogItem(hDlg, GUI_ID_CURVE0));

        break;
    case WM_NOTIFY_PARENT_REFLECTION:
        FunctionKeyPressedResume(Menu_hWin.hWin,0);
        if(StandbyState==0xFF)
        {
            PlanMenuType = STANDBY_MENU;
        }
        else
        {
            PlanMenuType = MAIN_MENU;
        }
        Task |= MEAU_UPDATE;
        break;
    case WM_NOTIFY_PARENT:
        NCode = pMsg->Data.v;
        Id = WM_GetId(pMsg->hWinSrc);
        switch(NCode)
        {
        case WM_NOTIFICATION_CLICKED://点击波形的时候会闪，可能是焦点变化引起的
            switch(Id)
            {
            case GUI_ID_CURVE0:
            case GUI_ID_CURVE1:
                TracingTrend_CursorMove(WM_GetDialogItem(hDlg,GUI_ID_CURVE0),1,0);
                TracingTrend_CursorMove(WM_GetDialogItem(hDlg,GUI_ID_CURVE1),1,0);
                break;
            case GUI_ID_BUTTON0:
                if(!BUTTON_GetState(WM_GetDialogItem(hDlg,Id)))
                {
                    if(WM_IsVisible(WM_GetDialogItem(hDlg,GUI_ID_KEYBOARD0)))
                    {
                        WM_HideWindow(WM_GetDialogItem(hDlg,GUI_ID_KEYBOARD0));
                        WM_SetFocus(WM_GetDialogItem(hDlg,GUI_ID_CURVE0));
                    }
                }
                else 
                {
                    WM_ShowWindow(WM_GetDialogItem(hDlg,GUI_ID_KEYBOARD0));
                    FunctionKeyPressedResume(hDlg,Id);
                    TrendPara_Info.Graph_ChoosePos = 0xff;
                    //设置键盘的大小及位置
                    WM_SetSize(WM_GetDialogItem(hDlg,GUI_ID_KEYBOARD0),160,TREND_TIME_NUM*35);
                    WM_MoveChildTo(WM_GetDialogItem(hDlg,GUI_ID_KEYBOARD0),84+13,2);//调整键盘位置
                    //初始化键盘
                    KEYBOARD_SetInfo(WM_GetDialogItem(hDlg,GUI_ID_KEYBOARD0),TREND_TIME_NUM,2,TREND_TIME_NUM/2,_GetTimeIndex(TrendPara_Info.Graph_TimeRange),TrendTimeRange);
                    WM_SetFocus(WM_GetDialogItem(hDlg,GUI_ID_KEYBOARD0));
                }
                break;
            case GUI_ID_BUTTON1:
            case GUI_ID_BUTTON2:
                if(!BUTTON_GetState(WM_GetDialogItem(hDlg,Id)))
                {
                    if(WM_IsVisible(WM_GetDialogItem(hDlg,GUI_ID_KEYBOARD0)))
                    {
                        WM_HideWindow(WM_GetDialogItem(hDlg,GUI_ID_KEYBOARD0));
                        WM_SetFocus(WM_GetDialogItem(hDlg,GUI_ID_CURVE0));
                    }
                }
                else
                {
                    WM_ShowWindow(WM_GetDialogItem(hDlg,GUI_ID_KEYBOARD0));
                    FunctionKeyPressedResume(hDlg,Id);
                    TrendPara_Info.Graph_ChoosePos = Id - GUI_ID_BUTTON1;//按下的按钮对应参数的索引值
                    //设置键盘的大小及位置
                    WM_SetSize(WM_GetDialogItem(hDlg,GUI_ID_KEYBOARD0),350,150+20+17+12);//ylk modify 20181121
//                     switch(Id)
//                     {
//                     case GUI_ID_BUTTON1:
                        WM_MoveChildTo(WM_GetDialogItem(hDlg,GUI_ID_KEYBOARD0),84+13,31-20-12);//调整键盘位置 //ylk modify 20181121
//                         break;
//                     case GUI_ID_BUTTON2:
//                         WM_MoveChildTo(WM_GetDialogItem(hDlg,GUI_ID_KEYBOARD0),84+13,31);//调整键盘位置
//                         break;
//                     }
                    //初始化键盘
                    KEYBOARD_SetInfo(WM_GetDialogItem(hDlg,GUI_ID_KEYBOARD0),TREND_PARA_NUM,5,5,TrendPara_Info.Graph_Index[Id-GUI_ID_BUTTON1],TrendParaName);//ylk modify 20181121 3->5 五行
                    WM_SetFocus(WM_GetDialogItem(hDlg,GUI_ID_KEYBOARD0));
                }

                break;
            case GUI_ID_BUTTON3:
                if(!BUTTON_GetState(pMsg->hWinSrc))
                {
                    if(WM_IsVisible(WM_GetDialogItem(hDlg,GUI_ID_KEYBOARD0)))
                    {
                        WM_HideWindow(WM_GetDialogItem(hDlg,GUI_ID_KEYBOARD0));
                        WM_SetFocus(WM_GetDialogItem(hDlg,GUI_ID_CURVE0));
                        FunctionKeyPressedResume(hDlg,Id);
                    }
                }
                else
                {
                    ClearTrendInfo();
                    WM_HideWindow(WM_GetDialogItem(hDlg,GUI_ID_KEYBOARD0));
                    WM_ShowWindow(WM_GetDialogItem(hDlg,GUI_ID_TEXT0));
                    WM_DisableWindow(WM_GetDialogItem(hDlg,GUI_ID_BUTTON0));
                    WM_DisableWindow(WM_GetDialogItem(hDlg,GUI_ID_BUTTON1));
                    WM_DisableWindow(WM_GetDialogItem(hDlg,GUI_ID_BUTTON2));
//                    WM_DisableWindow(WM_GetDialogItem(hDlg,GUI_ID_BUTTON3));
                    WM_DisableWindow(WM_GetDialogItem(hDlg,GUI_ID_CURVE0));
                    WM_DisableWindow(WM_GetDialogItem(hDlg,GUI_ID_CURVE1));
                }
                
                break;
            case GUI_ID_KEYBOARD0:
                break;
            default:
                break;
            }
            if(Id != GUI_ID_KEYBOARD0)
            {
                break;
            }
        case GUI_KEY_ENTER:
            if(Id == GUI_ID_KEYBOARD0)
            {
                Id = KEYBOARD_GetIndex(pMsg->hWinSrc); //获得要显示的参数索引值
                if(TrendPara_Info.Graph_ChoosePos == 0xff)
                {
                    TrendPara_Info.Graph_TimeRange = TrendTimeRange_Index[Id];
					RTEEPROMwrite(TREND_TIMERANGE, TrendPara_Info.Graph_TimeRange);

                    TracingTrend_SetRange(WM_GetDialogItem(hDlg, GUI_ID_CURVE0),TrendPara_Info.Graph_TimeRange);
                    TracingTrend_SetRange(WM_GetDialogItem(hDlg, GUI_ID_CURVE1),TrendPara_Info.Graph_TimeRange);
                    //初始化趋势图    
                    TrendGraph_Init(WM_GetDialogItem(hDlg, GUI_ID_CURVE0),TrendPara_Info.Graph_Index[0],TrendPara_Info.Graph_TimeRange,'F',0);
                    TrendGraph_Init(WM_GetDialogItem(hDlg, GUI_ID_CURVE1),TrendPara_Info.Graph_Index[1],TrendPara_Info.Graph_TimeRange,'T',0);
                    WM_HideWindow(pMsg->hWinSrc);
                }
                else 
                {
                    TrendPara_Info.Graph_Index[TrendPara_Info.Graph_ChoosePos] = Id;

                    TracingTrend_PaintFlag(WM_GetDialogItem(hDlg, GUI_ID_CURVE0),1);
                    TracingTrend_PaintFlag(WM_GetDialogItem(hDlg, GUI_ID_CURVE1),1);
                    //初始化趋势图    
                    TrendGraph_Init(WM_GetDialogItem(hDlg, GUI_ID_CURVE0),TrendPara_Info.Graph_Index[0],TrendPara_Info.Graph_TimeRange,'F',1);
                    TrendGraph_Init(WM_GetDialogItem(hDlg, GUI_ID_CURVE1),TrendPara_Info.Graph_Index[1],TrendPara_Info.Graph_TimeRange,'T',1);
                    
                    WM_HideWindow(pMsg->hWinSrc);
                    TrendPara_Info.Graph_ChoosePos = 0xff;                 
                }
                WM_SendMessageNoPara(hDlg,WM_INIT_DIALOG);
            }
            break;
        case GUI_KEY_LEFT:
            switch(Id)
            {
            case GUI_ID_CURVE0:
            case GUI_ID_CURVE1:
                TracingTrend_CursorMove(WM_GetDialogItem(hDlg,GUI_ID_CURVE0),0,0);
                TracingTrend_CursorMove(WM_GetDialogItem(hDlg,GUI_ID_CURVE1),0,0);
                break;
            }
            break;
        case GUI_KEY_RIGHT:
            switch(Id)
            {
            case GUI_ID_CURVE0:
            case GUI_ID_CURVE1:
                TracingTrend_CursorMove(WM_GetDialogItem(hDlg,GUI_ID_CURVE0),0,1);
                TracingTrend_CursorMove(WM_GetDialogItem(hDlg,GUI_ID_CURVE1),0,1);
                break;
            }
            break;
        }
    default:
        WM_DefaultProc(pMsg);
    }
}

#endif

#if (TREND_TABLE_ON)
static const GUI_WIDGET_CREATE_INFO _aDialogCreateTrendTable[] =
{
    { WINDOW_CreateIndirect,    "", 0,                 2,   59, 572,368-35, FRAMEWIN_CF_ACTIVE },
    { TrendTable_CreateIndirect,"", GUI_ID_TRENDTABLE0,4,   45, 521,280, FRAMEWIN_CF_ACTIVE },
    { BUTTON_CreateIndirect,    "",   GUI_ID_BUTTON0,  6,    4, 61,38},//time range
    { BUTTON_CreateIndirect,    "",   GUI_ID_BUTTON1,  71,   4, 61,38},//
    { BUTTON_CreateIndirect,    "",   GUI_ID_BUTTON2,  136,    4, 61,38},//
    { BUTTON_CreateIndirect,    "",   GUI_ID_BUTTON3,  201,    4, 61,38},//
    { BUTTON_CreateIndirect,    "",   GUI_ID_BUTTON4,  266,    4, 61,38},//
    { BUTTON_CreateIndirect,    "",   GUI_ID_BUTTON5,  331,    4, 61,38},//
    { BUTTON_CreateIndirect,    "",   GUI_ID_BUTTON6,  396,    4, 61,38},//
    { BUTTON_CreateIndirect,    "",   GUI_ID_BUTTON7,  461,    4, 61,38},//

    { BUTTON_CreateIndirect,    "",   GUI_ID_BUTTON8,  529,   45, 35,117},//up
    { BUTTON_CreateIndirect,    "",   GUI_ID_BUTTON9,  529,  168, 35,117},//down
    { KEYBOARD_CreateIndirect,  "",   GUI_ID_KEYBOARD0,69,  45,350,220},
};

#define TOTAL_NUMBER               TREND_BUFFERSIZE
#define TOTAL_EACHPAGE_ROWNUM      10    //每一页的行数
#define TOTAL_EACHPAGE_COLUMNNUM   7     //每一页的列数
#define TOTAL_PAGENUM        (TOTAL_NUMBER/TOTAL_EACHPAGE_ROWNUM)
static void _cbDialogTrendTable(WM_MESSAGE * pMsg)//当趋势图被删除时应该判断当前的按键控件是否被删除，如果没有则删除当前按键控件
{
    WM_HWIN hDlg,hWin,hTable;
    char ButtonText[8] = {0};
    int Id,NCode,cnt;
    hDlg = pMsg->hWin;
    hTable = WM_GetDialogItem(hDlg,GUI_ID_TRENDTABLE0);
    switch (pMsg->MsgId)
    {
    case WM_INIT_DIALOG:
        WINDOW_SetBkColor(hDlg,TREND_WINDOW_BK_COLOR);
        //TimeRange
        hWin = WM_GetDialogItem(pMsg->hWin,GUI_ID_BUTTON0);
        NCode = TrendPara_Info.Table_TimeRange;
        sprintf(ButtonText,"%dHour",NCode);
        BUTTON_SetPressed(hWin,0);
        BUTTON_SetTextColor(hWin,0,MONITOR_FRAME_COLOR);
        BUTTON_SetTextColor(hWin,1,GUI_WHITE);
        BUTTON_SetText(hWin,ButtonText);  
        BUTTON_SetStyle(hWin,6);                          ///想要功能正常，必须保证BUTTON_SetStyle(6)连续点击能够按下和弹起
        BUTTON_SetBkColor(hWin,0,GUI_BLACK);
        BUTTON_SetBkColor(hWin,1,TREND_KEYPRESSED_LINE);
        BUTTON_SetBkColor(hWin,3,GUI_BLACK);
        BUTTON_SetLineColor(hWin,0,MONITOR_FRAME_COLOR);
        BUTTON_SetLineColor(hWin,1,TREND_KEY_LINE);

        for(cnt = 0;cnt<TOTAL_EACHPAGE_COLUMNNUM;cnt++)
        {
            hWin = WM_GetDialogItem(pMsg->hWin,GUI_ID_BUTTON1+cnt);
            BUTTON_SetPressed(hWin,0);
            BUTTON_SetTextColor(hWin,0,MONITOR_FRAME_COLOR);
            BUTTON_SetTextColor(hWin,1,GUI_WHITE);
            BUTTON_SetText(hWin,TrendParaName[TrendPara_Info.Table_Index[cnt]]);  
            BUTTON_SetStyle(hWin,6);
            BUTTON_SetBkColor(hWin,0,GUI_BLACK);
            BUTTON_SetBkColor(hWin,1,TREND_KEYPRESSED_LINE);
            BUTTON_SetBkColor(hWin,3,GUI_BLACK);
            BUTTON_SetLineColor(hWin,0,MONITOR_FRAME_COLOR);
            BUTTON_SetLineColor(hWin,1,TREND_KEY_LINE);
        }
        hWin = WM_GetDialogItem(hDlg, GUI_ID_BUTTON8);
        BUTTON_SetStyle(hWin,8);
        BUTTON_SetBkColor(hWin,0,GUI_BLACK);
        BUTTON_SetBkColor(hWin,1,WINDOW_BK_COLOR);
        BUTTON_SetLineColor(hWin,0,MONITOR_FRAME_COLOR);
        BUTTON_SetLineColor(hWin,1,MONITOR_FRAME_COLOR);
        BUTTON_SetBitmapEx(hWin,0,&bmblack_up,12,15);       
        BUTTON_SetBitmapEx(hWin,1,&bmwhite_up,12,15);
        hWin = WM_GetDialogItem(hDlg, GUI_ID_BUTTON9);
        BUTTON_SetStyle(hWin,8);
        BUTTON_SetBkColor(hWin,0,GUI_BLACK);
        BUTTON_SetBkColor(hWin,1,WINDOW_BK_COLOR);
        BUTTON_SetLineColor(hWin,0,MONITOR_FRAME_COLOR);
        BUTTON_SetLineColor(hWin,1,MONITOR_FRAME_COLOR);
        BUTTON_SetBitmapEx(hWin,0,&bmblack_down,12,90);       
        BUTTON_SetBitmapEx(hWin,1,&bmwhite_down,12,90);

        WM_BringToBottom(hTable);//趋势表设为最底层
        WM_SetFocus(hTable);
        WM_HideWindow(WM_GetDialogItem(hDlg,GUI_ID_KEYBOARD0));

        TrendTable_SetPagePosition(hTable,0,0);  //每次初始化都设置为0页0行
        TrendTable_SetPaintType(hTable,Paint_All);//设置初始化全部刷新 
        TrendPara_Info.Table_ChoosePos = 0xff;//初始化索引号

        break;
    case WM_NOTIFY_PARENT:
        NCode = pMsg->Data.v;
        Id = WM_GetId(pMsg->hWinSrc);
        switch(NCode)
        {
        case WM_NOTIFICATION_LOST_FOCUS:
            if(Id == GUI_ID_KEYBOARD0)
            {
                //键盘失去焦点后能够被隐藏
                if(TrendPara_Info.Table_ChoosePos != 0xff)
                {
                    WM_HideWindow(WM_GetDialogItem(hDlg,GUI_ID_KEYBOARD0));
                    TrendTable_SetPaintType(hTable,Paint_All);
                    WM_Paint(hTable);//如果不及时刷新，接下来就会进入趋势表的OnTouch函数中，会将Paint_All标志改变成Paint_Column，从而不能刷新到键盘
                }
            }   
            break;
        case WM_NOTIFICATION_RELEASED:
            if(Id == GUI_ID_BUTTON8)
            {
                TrendTable_SetPagePosition(hTable,TrendTable_GetPagePosition(hTable)-1,0);
                WM_SetFocus(hTable);
            }
            if(Id == GUI_ID_BUTTON9)
            {
                TrendTable_SetPagePosition(hTable,TrendTable_GetPagePosition(hTable)+1,0);
                WM_SetFocus(hTable);
            }
            break;
        case WM_NOTIFICATION_CLICKED://点击波形的时候会闪，可能是焦点变化引起的            {
            switch(Id)
            {
            case GUI_ID_BUTTON0://时间范围        
            case GUI_ID_BUTTON1:
            case GUI_ID_BUTTON2:
            case GUI_ID_BUTTON3:
            case GUI_ID_BUTTON4:
            case GUI_ID_BUTTON5:
            case GUI_ID_BUTTON6:
            case GUI_ID_BUTTON7:

                if(!BUTTON_GetState(pMsg->hWinSrc))
                {
                    TrendPara_Info.Table_ChoosePos = 0xff;
                    if(WM_IsVisible(WM_GetDialogItem(hDlg,GUI_ID_KEYBOARD0)))
                    {
                        WM_HideWindow(WM_GetDialogItem(hDlg,GUI_ID_KEYBOARD0));
                    }
                }
                else 
                {
                    if(TrendPara_Info.Table_ChoosePos != 0xff)//弹起其他按键
                    {
                        BUTTON_SetPressed(WM_GetDialogItem(hDlg,GUI_ID_BUTTON0+TrendPara_Info.Table_ChoosePos),0);
                    }
                    TrendPara_Info.Table_ChoosePos = Id-GUI_ID_BUTTON0;
                    if(Id == GUI_ID_BUTTON0)//设置时间范围Keyboard
                    {
                        //设置键盘的大小及位置
                        WM_SetSize(WM_GetDialogItem(hDlg,GUI_ID_KEYBOARD0),160,(TREND_TIME_NUM*35));
                        WM_MoveChildTo(WM_GetDialogItem(hDlg,GUI_ID_KEYBOARD0),4,45);

                        KEYBOARD_SetInfo(WM_GetDialogItem(hDlg,GUI_ID_KEYBOARD0),TREND_TIME_NUM,2,TREND_TIME_NUM/2,_GetTimeIndex(TrendPara_Info.Table_TimeRange),TrendTimeRange);
                    }
                    else//设置参数Keyboard 
                    {
                        //设置键盘的大小及位置
                        WM_SetSize(WM_GetDialogItem(hDlg,GUI_ID_KEYBOARD0),350,220);
                        WM_MoveChildTo(WM_GetDialogItem(hDlg,GUI_ID_KEYBOARD0),69,45);

                        KEYBOARD_SetInfo(WM_GetDialogItem(hDlg,GUI_ID_KEYBOARD0),TREND_PARA_NUM,5,4,TrendPara_Info.Table_Index[TrendPara_Info.Table_ChoosePos-1],TrendParaName);
                    }               
                    if(!WM_IsVisible(WM_GetDialogItem(hDlg,GUI_ID_KEYBOARD0)))
                    {
                        WM_ShowWindow(WM_GetDialogItem(hDlg,GUI_ID_KEYBOARD0));
                        //显示之后将键盘全部重绘一次（上一次选择的按键和本次选择的为同一个按键时，则重绘整个键盘）
                        KEYBOARD_SetIndex(WM_GetDialogItem(hDlg,GUI_ID_KEYBOARD0),KEYBOARD_GetIndex(WM_GetDialogItem(hDlg,GUI_ID_KEYBOARD0)));
                    }               
                    WM_SetFocus(WM_GetDialogItem(hDlg,GUI_ID_KEYBOARD0));
                }
                break;
            default:
                if(TrendPara_Info.Table_ChoosePos != 0xff)//按下趋势表其他地方的时候，应该弹起被按下的按键
                {
                    BUTTON_SetPressed(WM_GetDialogItem(hDlg,GUI_ID_BUTTON0+TrendPara_Info.Table_ChoosePos),0);
                    TrendPara_Info.Table_ChoosePos = 0xff;
                }
                break;
            }
            if(Id != GUI_ID_KEYBOARD0)//如果是选择了键盘上的按键，则进入GUI_KEY_ENTER事件中
            {
                break;
            }
        case GUI_KEY_ENTER:
            if(Id == GUI_ID_KEYBOARD0)
            {
                Id = KEYBOARD_GetIndex(pMsg->hWinSrc); //获得要显示的参数索引值
                if(TrendPara_Info.Table_ChoosePos == 0)//时间范围设置
                {
                    TrendPara_Info.Table_TimeRange	 = TrendTimeRange_Index[Id];
                    TrendTable_SetTimeRange(hTable,TrendPara_Info.Table_TimeRange);
                    for(cnt = 1;cnt < TOTAL_EACHPAGE_COLUMNNUM+1;cnt++)
                    {
                        Fill_TrendTableDispBuffer(hTable,TrendParaName[TrendPara_Info.Table_Index[cnt-1]],cnt/*索引值从1开始*/);       
                    }

                }
                else //参数设置
                {
                    TrendPara_Info.Table_Index[TrendPara_Info.Table_ChoosePos-1] = Id;
                    //                    Fill_TrendTableDispBuffer(hTable,TrendParaName[Id],TrendPara_Info.Table_ChoosePos);//列数从1开始
                    for(cnt = 1;cnt < TOTAL_EACHPAGE_COLUMNNUM+1;cnt++)
                    {
                        Fill_TrendTableDispBuffer(hTable,TrendParaName[TrendPara_Info.Table_Index[cnt-1]],cnt/*索引值从1开始*/);       
                    }
                }
                BUTTON_SetPressed(WM_GetDialogItem(hDlg,GUI_ID_BUTTON0+TrendPara_Info.Table_ChoosePos),0);
                TrendPara_Info.Table_ChoosePos = 0xff;
                WM_HideWindow(pMsg->hWinSrc);
                WM_SetFocus(hTable);
                WM_SendMessageNoPara(hDlg,WM_INIT_DIALOG);
            }
            break;
        case GUI_KEY_LEFT:
            if(Id == GUI_ID_TRENDTABLE0)
            {
                if(TrendTable_DecSel(hTable) == 1)
                {
                    //填充显示buffer数据
                    TrendTable_Invalidate(hTable);
                }
            }
            else if(Id == GUI_ID_KEYBOARD0)
            {

            }
            break;
        case GUI_KEY_RIGHT:  
            if(Id == GUI_ID_TRENDTABLE0)
            {
                if(TrendTable_IncSel(hTable) == 1)
                {
                    //填充显示buffer数据
                    TrendTable_Invalidate(hTable);
                }
            }
            else if(Id == GUI_ID_KEYBOARD0)
            {

            }

            break;
        }
    default:
        WM_DefaultProc(pMsg);

    }

}


#endif

WM_HWIN _TrendWindowCreat(void)
{
    WM_HWIN hWin1,hWin2,hWin3,hWin4;
    int cnt;
    //初始化趋势变量名
    InitTrendName();
    hWin1 = FRAMEWIN_Create("",0,WM_CF_SHOW,3,   146, 642+37, 250);//调整窗口位置
    hWin2 = MULTIPAGE_CreateEx(8, 10, 625+37, 230, hWin1, WM_CF_SHOW, 0, GUI_ID_MULTIPAGE0);
//    FRAMEWIN_AddCloseButton(hWin1,0,540);
#if (TREND_GRAPH_ON)
    hWin3 = GUI_CreateDialogBox(_aDialogCreateTrendGraph,
                                GUI_COUNTOF(_aDialogCreateTrendGraph),
                                &_cbDialogTrendGraph, 0, 0, 0);
//                                &_cbDialogTrendGraph, 0, 0, 0);
    MULTIPAGE_AddPage(hWin2,hWin3,Sys_Lib_GetDispStr(DP_TRENDGRAPH));

    //设置趋势图趋势表的默认参数信息
    TrendPara_Info.Graph_Handle[0] = WM_GetDialogItem(hWin3,GUI_ID_CURVE0);
    TrendPara_Info.Graph_Handle[1] = WM_GetDialogItem(hWin3,GUI_ID_CURVE1);
    TrendPara_Info.Graph_Index[0]  = PPEAK;
    TrendPara_Info.Graph_Index[1]  = PEEP;
    //TrendPara_Info.Graph_TimeRange = 1;
	TrendPara_Info.Graph_TimeRange = RTEEPROMread(TREND_TIMERANGE);
	for(cnt = 0;cnt < TREND_TIME_NUM;cnt++)
	{
		if(TrendTimeRange_Index[cnt] == TrendPara_Info.Graph_TimeRange)
		{
			break;
		}
	}
	if(cnt == TREND_TIME_NUM)
	{
		TrendPara_Info.Graph_TimeRange = TrendTimeRange_Index[0];
	}
    TrendPara_Info.Graph_ChoosePos = 0xff;
    //初始化趋势图
    TrendGraph_Init(WM_GetDialogItem(hWin3, GUI_ID_CURVE0),TrendPara_Info.Graph_Index[0],TrendPara_Info.Graph_TimeRange,'F',1);
    TrendGraph_Init(WM_GetDialogItem(hWin3, GUI_ID_CURVE1),TrendPara_Info.Graph_Index[1],TrendPara_Info.Graph_TimeRange,'T',1);

#endif

#if (TREND_TABLE_ON)
    hWin4 = GUI_CreateDialogBox(_aDialogCreateTrendTable,
                                GUI_COUNTOF(_aDialogCreateTrendTable),
                                &_cbDialogTrendTable, 0, 40, 40);
    MULTIPAGE_AddPage(hWin2,hWin4,Sys_Lib_GetDispStr(DP_TRENDTABLE));
    //设置趋势表趋势表的默认参数信息
    TrendPara_Info.Table_Index[0] = PPEAK;
    TrendPara_Info.Table_Index[1] = PMEAN;
    TrendPara_Info.Table_Index[2] = PEEP;
    TrendPara_Info.Table_Index[3] = PEEP;  
    TrendPara_Info.Table_Index[4] = MV;
    TrendPara_Info.Table_Index[5] = MVSPN;
    TrendPara_Info.Table_Index[6] = VTE;
    TrendPara_Info.Table_TimeRange = 1;
    //初始化趋势表
    TrendTable_SetInfo(WM_GetDialogItem(hWin4, GUI_ID_TRENDTABLE0),TOTAL_EACHPAGE_ROWNUM,TOTAL_EACHPAGE_COLUMNNUM,0,TREND_BUFFERSIZE,RTCTime,TrendPara_Info.Table_TimeRange);
    for(cnt = 0;cnt < TOTAL_EACHPAGE_COLUMNNUM;cnt++)
    {
        Fill_TrendTableDispBuffer(WM_GetDialogItem(hWin4, GUI_ID_TRENDTABLE0),TrendParaName[TrendPara_Info.Table_Index[cnt]],cnt+1/*索引值从1开始*/);       
    }
#endif 

#if (TREND_GRAPH_ON)
    //初始化趋势参数后，重新刷新趋势界面
    WM_SendMessageNoPara(hWin3,WM_INIT_DIALOG);
#endif  
    MULTIPAGE_SelectPage(hWin2,0);

    FRAMEWIN_AddCloseButton(hWin1,0,5);  
    return hWin1;
}