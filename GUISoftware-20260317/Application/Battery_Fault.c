#include "Battery_Fault.h"

static BATTERY_CONN Battery_Conn;
 
void Battery_Collect(void)
{
	if(Battery_Conn.Bat_Count % 10 == 0 && Battery_Conn.Bat_Count < 100)
	{
 		Battery_Conn.Bat_Conn[Battery_Conn.Bat_Count/10] = CheckChargeStatus();
	}
	Battery_Conn.Bat_Count++;
}
 
void Battery_Check(void)
{
	Int8U i;
	for(i=0;i<10;i++)
	{
 		if(Battery_Conn.Bat_Conn[i] == 1)
 		{
 			Battery_Conn.Bat_DisConnCount++;
 		}
	}
	if(Battery_Conn.Bat_DisConnCount >= 5)  Battery_Conn.Bat_DisConnFlag = 1;
}

Int8U Battery_GetDisconnFlag(void)
{
	return Battery_Conn.Bat_DisConnFlag;
}

void Battery_SetDisconnFlag(Int8U flag)
{
	Battery_Conn.Bat_DisConnFlag = flag;
}