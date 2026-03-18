/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              µC/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : SIM_X.c
Purpose     : Windows Simulator externals
              The init routine in this file can be used to set up the
              simulator
---------------------------END-OF-HEADER------------------------------
*/

#include "SIM.h"
#include "GUI.h"
#include "Key_Handle.h"
#include "Demo_AlarmTest.h"

extern unsigned char Picture_Load_Complete;
extern unsigned char butter_press_count;
void SIM_HARDKEY_Callback(int KeyIndex, int State)
{
	int x = 1000;
	if(State)
	{
		switch(KeyIndex)
		{
		case 3:
			if(Demo_Alarm)
			{
				DEMO_TestAlarm(0x55);
			}
			GUI_StoreKey(GUI_KEY_RIGHT);
			break;
		case 4:
			if(Demo_Alarm)
			{
				DEMO_TestAlarm(0xDD);
			}
			GUI_StoreKey(GUI_KEY_LEFT);
			break;
		case 5:
			if( !Picture_Load_Complete )
			{
				butter_press_count++;  
			}
			GUI_StoreKey(GUI_KEY_ENTER);
			break;
		/*case 3:
			GUI_StoreKey(GUI_KEY_STANDBY);
			break;
		case 4:
			GUI_StoreKey(GUI_KEY_ALARM_RESET);
			break;
		case 5:
			GUI_StoreKey(GUI_KEY_SILENCE);
			break;
		case 6:
			GUI_StoreKey(GUI_KEY_HOME);
			break;*/
		}
	}
}

void SIM_X_Init() {
  //if (((LCD_GetDevCap(LCD_DEVCAP_XSIZE) * LCD_GetXMag()) == 320) && 
  //    ((LCD_GetDevCap(LCD_DEVCAP_YSIZE) * LCD_GetYMag()) == 240) && 
  //    (LCD_GetNumLayers() == 1))
  {
    SIM_SetLCDPos(42,94);                   // Define the position of the LCD in the bitmap
    SIM_SetTransColor  (0xff0000);          // Define the transparent color

	SIM_HARDKEY_SetCallback(0,&SIM_HARDKEY_Callback);
	SIM_HARDKEY_SetCallback(1,&SIM_HARDKEY_Callback);
	SIM_HARDKEY_SetCallback(2,&SIM_HARDKEY_Callback);
	SIM_HARDKEY_SetCallback(3,&SIM_HARDKEY_Callback);
	SIM_HARDKEY_SetCallback(4,&SIM_HARDKEY_Callback);
	SIM_HARDKEY_SetCallback(5,&SIM_HARDKEY_Callback);
	SIM_HARDKEY_SetCallback(6,&SIM_HARDKEY_Callback);
/*
    SIM_SetLCDColorBlack(0, 0x808080);      // Define black color
    SIM_SetLCDColorWhite(0, 0xc0c0c0);      // Define white color
*/
  }
}


