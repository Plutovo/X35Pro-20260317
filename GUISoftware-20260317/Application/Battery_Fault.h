#ifndef _BATTERY_FAULT_
#define _BATTERY_FAULT_
#include "Static_Menu_creat.h"

typedef struct  
{
	u8 Bat_Conn[10];
	u8 Bat_Count;
	u8 Bat_DisConnCount;
	u8 Bat_DisConnFlag;
}BATTERY_CONN;


#endif