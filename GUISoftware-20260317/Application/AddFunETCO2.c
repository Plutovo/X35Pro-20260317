//
#include "MainTask.h"
#include "iolpc1788 and 2478.h"
#include "arm_comm.h"
#include "Alarm_handle.h"
//
unsigned char etco2pump_Occlusion = 0;
unsigned char etco2pump_needupdate = 0;
extern INT8U  Usart2_Tx_Buffer[20];
#if (LOGO_FLAG == MSW_LOGO)
extern INT8U  Usart1_Tx_Buffer[8];
#else
extern INT8U  Usart1_Tx_Buffer[8];
#endif
unsigned char btpsn2o = 0;
unsigned char lastbtpsn2o = 0;
unsigned char n2oo2 = 0;
unsigned char lastn2oo2 = 0;
extern ALARM_WORK AlarmsWork;
//

#if (LOGO_FLAG == MSW_LOGO)
void BTPSCompensate()
{
    Usart1_Tx_Buffer[0] = 'Z';
    Usart1_Tx_Buffer[1] = 0 ;
    Usart1_Tx_Buffer[2] = 0 ;
    Usart1_Tx_Buffer[3] = btpsn2o ;
    Usart1_Tx_Buffer[4] = n2oo2 ;
    Usart1_Tx_Buffer[5] = '>' ;
    U1THR = '<';
}
void O2CO2Compensate()
{
    Usart1_Tx_Buffer[0] = 'Z';
    Usart1_Tx_Buffer[1] = 0 ;
    Usart1_Tx_Buffer[2] = 0 ;
    Usart1_Tx_Buffer[3] = btpsn2o ;
    Usart1_Tx_Buffer[4] = n2oo2 ;
    Usart1_Tx_Buffer[5] = '>' ;
    U1THR = '<';
}
void CheckCo2PumpRunTime()
{
    Usart1_Tx_Buffer[0] = 'G';
    Usart1_Tx_Buffer[1] = '8';
    Usart1_Tx_Buffer[2] = '0';
    Usart1_Tx_Buffer[3] = '0';
    Usart1_Tx_Buffer[4] = '2';
    Usart1_Tx_Buffer[5] = '>';
    U1THR = '<';
}
void CheckCo2PumpOcclusion()
{
    Usart1_Tx_Buffer[0] = 'S';
    Usart1_Tx_Buffer[1] = '1';
    Usart1_Tx_Buffer[2] = '>';
    U1THR = '<';
}

void Co2ModuleONOFF(unsigned int onoff)
{
    if(onoff == 1)
    {
        Usart1_Tx_Buffer[0] = 'J';
        Usart1_Tx_Buffer[1] = '1';//泵关
        Usart1_Tx_Buffer[2] = '>';
        Usart1_Tx_Buffer[3] = '<';
        Usart1_Tx_Buffer[4] = 'L';
        Usart1_Tx_Buffer[5] = '1';//泵关
        Usart1_Tx_Buffer[6] = '>';
        U1THR = '<';
    }
    else
    {
        Usart1_Tx_Buffer[0] = 'J';
        Usart1_Tx_Buffer[1] = '0';//泵关
        Usart1_Tx_Buffer[2] = '>';
        Usart1_Tx_Buffer[3] = '<';
        Usart1_Tx_Buffer[4] = 'L';
        Usart1_Tx_Buffer[5] = '0';//泵关
        Usart1_Tx_Buffer[6] = '>';
        U1THR = '<';
    }
}
void CO2_Reset(void)
{
    Usart1_Tx_Buffer[0] = 'R';
    Usart1_Tx_Buffer[1] = 'E';
    Usart1_Tx_Buffer[2] = 'S';
    Usart1_Tx_Buffer[3] = 'E';
    Usart1_Tx_Buffer[4] = 'T';
    Usart1_Tx_Buffer[5] = '>';
    //UART2_SendData('<');
    U1THR = '<';
}
#else
void BTPSCompensate()
{
    Usart2_Tx_Buffer[0] = 'Z';
    Usart2_Tx_Buffer[1] = 0 ;
    Usart2_Tx_Buffer[2] = 0 ;
    Usart2_Tx_Buffer[3] = btpsn2o ;
    Usart2_Tx_Buffer[4] = n2oo2 ;
    Usart2_Tx_Buffer[5] = '>' ;
    U2THR = '<';
}
void O2CO2Compensate()
{
    Usart2_Tx_Buffer[0] = 'Z';
    Usart2_Tx_Buffer[1] = 0 ;
    Usart2_Tx_Buffer[2] = 0 ;
    Usart2_Tx_Buffer[3] = btpsn2o ;
    Usart2_Tx_Buffer[4] = n2oo2 ;
    Usart2_Tx_Buffer[5] = '>' ;
    U2THR = '<';
}
void CheckCo2PumpRunTime()
{
    Usart2_Tx_Buffer[0] = 'G';
    Usart2_Tx_Buffer[1] = '8';
    Usart2_Tx_Buffer[2] = '0';
    Usart2_Tx_Buffer[3] = '0';
    Usart2_Tx_Buffer[4] = '2';
    Usart2_Tx_Buffer[5] = '>';
    U2THR = '<';
}
void CheckCo2PumpOcclusion()
{
    Usart2_Tx_Buffer[0] = 'S';
    Usart2_Tx_Buffer[1] = '1';
    Usart2_Tx_Buffer[2] = '>';
    U2THR = '<';
}

void Co2ModuleONOFF(unsigned int onoff)
{
    if(onoff == 1)
    {
        Usart2_Tx_Buffer[0] = 'J';
        Usart2_Tx_Buffer[1] = '1';//泵关
        Usart2_Tx_Buffer[2] = '>';
        Usart2_Tx_Buffer[3] = '<';
        Usart2_Tx_Buffer[4] = 'L';
        Usart2_Tx_Buffer[5] = '1';//泵关
        Usart2_Tx_Buffer[6] = '>';
        U2THR = '<';
    }
    else
    {
        Usart2_Tx_Buffer[0] = 'J';
        Usart2_Tx_Buffer[1] = '0';//泵关
        Usart2_Tx_Buffer[2] = '>';
        Usart2_Tx_Buffer[3] = '<';
        Usart2_Tx_Buffer[4] = 'L';
        Usart2_Tx_Buffer[5] = '0';//泵关
        Usart2_Tx_Buffer[6] = '>';
        U2THR = '<';
    }
}
void CO2_Reset(void)
{
    Usart2_Tx_Buffer[0] = 'R';
    Usart2_Tx_Buffer[1] = 'E';
    Usart2_Tx_Buffer[2] = 'S';
    Usart2_Tx_Buffer[3] = 'E';
    Usart2_Tx_Buffer[4] = 'T';
    Usart2_Tx_Buffer[5] = '>';
    //UART2_SendData('<');
    U2THR = '<';
}
#endif

void AlarmCheckCo2PumpOcclusion(Int8U Vent_Flag)
{
    static unsigned char CountOcclusion = 0;
    static unsigned char CountNormal = 0;
    // if(Vent_Flag)
    {
        if(!(AlarmsWork.presentAlarms1&ALARM_ETCO2_LINE_OCCLUSION))
        {
            if(etco2pump_Occlusion)
            {
                CountOcclusion++;
                if(CountOcclusion>=5)
                {
                    AlarmsWork.newAlarms1 |= ALARM_ETCO2_LINE_OCCLUSION;
                    CountOcclusion = 0;
                }
            }
            else
            {
                CountOcclusion = 0;
            }
        }
        else
        {
            if(!etco2pump_Occlusion)
            {
                CountNormal++;
                if(CountNormal>=3)
                {
                    AlarmsWork.presentAlarms1 &= ~ALARM_ETCO2_LINE_OCCLUSION;
                    CountNormal = 0;
                }
            }
            else
            {
                CountNormal = 0;
            }
        }
    }
}


