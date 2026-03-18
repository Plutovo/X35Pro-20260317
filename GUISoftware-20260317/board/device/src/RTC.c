#include "includes.h"
#include "RTC.h"
 //2016.5.16 ZY 修改：RTC寄存器名前加上"RTC"
void RTC_Init(void)
{		 
    Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_RTC);  
		
    RTCILR = 0;	
    RTCAMR = 0xff ;
    RTCCIIR = 2; //分钟中断
    RTCCCR= 1 | 1<<4;//打开定时器
	
}

void RTCStart(void)
{
    RTCCCR_bit.CLKEN = 1;
}
void RTCStop(void)
{
    RTCCCR_bit.CLKEN = 0;
}

void RTCSetTime(RTC_TIMER Time)
{
    RTCSEC = Time.RTC_Sec;
    RTCMIN = Time.RTC_Min;
    RTCHOUR = Time.RTC_Hour;
    RTCDOM = Time.RTC_Mday;
    RTCMONTH = Time.RTC_Mon;
    RTCYEAR = Time.RTC_Year;
}

RTC_TIMER RTCReadTime(void)
{
    RTC_TIMER Time;
    Time.RTC_Sec = RTCSEC;
    if(Time.RTC_Sec >= 60)
    {
        Time.RTC_Sec = 0;
    }
    Time.RTC_Min = RTCMIN;
    if(Time.RTC_Min >= 60)
    {
        Time.RTC_Min = 0;
       	RTCMIN = Time.RTC_Min;
    }
    Time.RTC_Hour = RTCHOUR;
    if(Time.RTC_Hour >= 24)
    {
        Time.RTC_Hour = 0;
        RTCHOUR = Time.RTC_Hour;
    }
    //Time.RTC_Wday = DOW;
    //Time.RTC_Yday = DOY;
    Time.RTC_Mon = RTCMONTH;
    if((Time.RTC_Mon > 12)||(Time.RTC_Mon < 1))
    {
        Time.RTC_Mon = 1;
        RTCMONTH = Time.RTC_Mon;
    }
    Time.RTC_Year = RTCYEAR;
    if((Time.RTC_Year > 2065)||(Time.RTC_Year < 2015))
    {
        Time.RTC_Year = 2015;
        RTCYEAR = Time.RTC_Year;
    }
    Time.RTC_Mday = RTCDOM;
    //日期的判断要放在最后，因为要使用到年和月的变量，所以应该先为年和月赋值。
    if(Time.RTC_Mday > GetThisYears_OfDay(Time.RTC_Year,Time.RTC_Mon))
    {
        Time.RTC_Mday = 1;
        RTCDOM = Time.RTC_Mday;
    }
    return(Time);
}