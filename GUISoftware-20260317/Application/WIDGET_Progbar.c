/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              礐/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : WIDGET_Progbar.c
Purpose     : Simple demo shows the use of the PROGBAR widget
----------------------------------------------------------------------
*/
#include "gui.h"
#include "progbar.h"
#include "LCDConf.H"
#include "MainTask.h"


void Disable_Interrupt(void);
void Enable_Interrupt(void);

/*******************************************************************
*
*            Shows the use of progress bars
*
********************************************************************
*/
void DemoProgBar(void)
{
	int i;
	PROGBAR_Handle hProgBar;
	/* Create `em */
	switch(LOGO_FLAG)
	{
	case SIRIUSMED_LOGO:
		hProgBar = PROGBAR_Create(230,286,332,25, WM_CF_SHOW);
		PROGBAR_SetFont(hProgBar, &GUI_Font_15);
		PROGBAR_SetTextAlign(hProgBar, GUI_TA_HCENTER);
		if (LCD_GetDevCap(LCD_DEVCAP_BITSPERPIXEL) <= 4)
		{
			PROGBAR_SetBarColor(hProgBar, 0, 0x91B600);
			PROGBAR_SetBarColor(hProgBar, 1, GUI_LIGHTGRAY);
		}
		else
		{
			PROGBAR_SetBarColor(hProgBar, 0, 0x742000);
			PROGBAR_SetBarColor(hProgBar, 1, GUI_LIGHTGRAY);
		}
		//PROGBAR_SetText(hProgBar, "Self-Test");
		for (i=0; i<=100; i++)
		{
			PROGBAR_SetValue(hProgBar, i);
			GUI_Delay(50);
			WM_Paint(hProgBar);
		}
		break;
	case HOSPITECH_LOGO:
		hProgBar = PROGBAR_Create(234,370,332,25, WM_CF_SHOW);//(140,360,360,30, WM_CF_SHOW);    hanbinsharp mod 20131202
		//PROGBAR_EnableMemdev(ahProgBar[0]);
		PROGBAR_SetFont(hProgBar, &GUI_Font_15);
		PROGBAR_SetTextAlign(hProgBar, GUI_TA_HCENTER);
		if (LCD_GetDevCap(LCD_DEVCAP_BITSPERPIXEL) <= 4)
		{
			PROGBAR_SetBarColor(hProgBar, 0, 0x0A00C7);//0x91B600
			PROGBAR_SetBarColor(hProgBar, 1, GUI_LIGHTGRAY);
		}
		else
		{
			PROGBAR_SetBarColor(hProgBar, 0, 0x0A00C7);    //0x742000  0xC7000A   //hanbinsharp mod 20131202
			PROGBAR_SetBarColor(hProgBar, 1, GUI_LIGHTGRAY);
			/*
			PROGBAR_SetBarColor(hProgBar, 0, 0x00B691);      
			PROGBAR_SetBarColor(hProgBar, 1, GUI_LIGHTGRAY);
			*/
		}
    //Disable_Interrupt(); 
		InitTrendQueue_Alias(hProgBar);
		//Enable_Interrupt();
		for (i=0; i<=100; i++)
		{
			PROGBAR_SetValue(hProgBar, i);
			GUI_Delay(50);
			//WM_Paint(hProgBar);
		}
		break;
	case NEUTRAL_LOGO:
	case NEW_SIRIUSMED_LOGO:
	case ENDURE_LOGO:
#ifdef LANGUAGE_CHINESE_FLAG //add ylk 20171114 国标新界面
		hProgBar = PROGBAR_Create(305,233,495,4, WM_CF_SHOW);
		PROGBAR_SetTextFlag(hProgBar,1);
		if (LCD_GetDevCap(LCD_DEVCAP_BITSPERPIXEL) <= 4)
		{
			PROGBAR_SetBarColor(hProgBar, 0, 0xFCFE73);
			PROGBAR_SetBarColor(hProgBar, 1, 0x4E2300);
		}
		else
		{
			PROGBAR_SetBarColor(hProgBar, 0, 0xFCFE73);
			PROGBAR_SetBarColor(hProgBar, 1, 0xBD7C33);
		}

		//Disable_Interrupt(); 
		InitTrendQueue_Alias(hProgBar);
		//Enable_Interrupt();
		GUI_SetFont(&GUI_Font_16);
		GUI_SetColor(GUI_WHITE);
		for (i=0; i<=100; i++)
		{
			PROGBAR_SetValue(hProgBar, i);
			GUI_Delay(55);
			WM_Paint(hProgBar);
		}
#else
		hProgBar = PROGBAR_Create(0,413,800,4, WM_CF_SHOW);
		PROGBAR_SetTextFlag(hProgBar,1);
		if (LCD_GetDevCap(LCD_DEVCAP_BITSPERPIXEL) <= 4)
		{
			PROGBAR_SetBarColor(hProgBar, 0, 0xFCFE73);
			PROGBAR_SetBarColor(hProgBar, 1, 0x4E2300);
		}
		else
		{
			PROGBAR_SetBarColor(hProgBar, 0, 0xFCFE73);
			PROGBAR_SetBarColor(hProgBar, 1, 0x4E2300);
		}

		//Disable_Interrupt(); 
		InitTrendQueue_Alias(hProgBar);
		//Enable_Interrupt();
		GUI_SetFont(&GUI_Font_16);
		GUI_SetColor(GUI_WHITE);
		GUI_DispCharAt('%',404,398);
		for (i=0; i<=100; i++)
		{
			if(i != 100)
			{
				GUI_DispDecAt(i,387,398,2);
			}
			else
			{
				GUI_DispDecAt(i,382,398,3);
			}
			PROGBAR_SetValue(hProgBar, i);
 			GUI_Delay(1/*55*/);
			WM_Paint(hProgBar);
		}
#endif
		break;
    case MS_WESTFALIA_LOGO:
		hProgBar = PROGBAR_Create(0,423,800,6, WM_CF_SHOW);
		PROGBAR_SetTextFlag(hProgBar,1);
		if (LCD_GetDevCap(LCD_DEVCAP_BITSPERPIXEL) <= 4)
		{
			PROGBAR_SetBarColor(hProgBar, 0, 0x317CF6);
			PROGBAR_SetBarColor(hProgBar, 1, 0xF7F7F7);
		}
		else
		{
			PROGBAR_SetBarColor(hProgBar, 0, 0x317CF6);
			PROGBAR_SetBarColor(hProgBar, 1, 0xF7F7F7);
		}

		//Disable_Interrupt(); 
		//InitTrendQueue_Alias(hProgBar);
		//Enable_Interrupt();
		GUI_SetFont(&GUI_Font_16);
		GUI_SetColor(GUI_BLACK);
		GUI_DispCharAt('%',404,445);
		for (i=0; i<=100; i++)
		{
			if(i != 100)
			{
				GUI_DispDecAt(i,387,445,2);
			}
			else
			{
				GUI_DispDecAt(i,382,445,3);
			}
			PROGBAR_SetValue(hProgBar, i);
			GUI_Delay(50);
			WM_Paint(hProgBar);
		}
		break;
	case SASVAT_LOGO:
		hProgBar = PROGBAR_Create(0,413,800,4, WM_CF_SHOW);
		PROGBAR_SetTextFlag(hProgBar,1);
		if (LCD_GetDevCap(LCD_DEVCAP_BITSPERPIXEL) <= 4)
		{
			PROGBAR_SetBarColor(hProgBar, 0, 0x4C1E3E);
			PROGBAR_SetBarColor(hProgBar, 1, 0xDEC53C);
		}
		else
		{
			PROGBAR_SetBarColor(hProgBar, 0, 0x4C1E3E);
			PROGBAR_SetBarColor(hProgBar, 1, 0xDEC53C);
		}

		Disable_Interrupt(); 
		InitTrendQueue_Alias(hProgBar);
		Enable_Interrupt();
		GUI_SetFont(&GUI_Font_16);
		GUI_SetColor(0x4C1E3E);
		GUI_DispCharAt('%',404,428);
		for (i=0; i<=100; i++)
		{
			if(i != 100)
			{
				GUI_DispDecAt(i,387,428,2);
			}
			else
			{
				GUI_DispDecAt(i,382,428,3);
			}
			PROGBAR_SetValue(hProgBar, i);
			GUI_Delay(50);
			WM_Paint(hProgBar);
		}
		break;          
	case ADVANCED_LOGO:
		hProgBar = PROGBAR_Create(0,435,800,6, WM_CF_SHOW);
		PROGBAR_SetTextFlag(hProgBar,1);
		if (LCD_GetDevCap(LCD_DEVCAP_BITSPERPIXEL) <= 4)
		{
			PROGBAR_SetBarColor(hProgBar, 0, 0x8D4B01);
			PROGBAR_SetBarColor(hProgBar, 1, 0xBB9B9B);
		}
		else
		{
			PROGBAR_SetBarColor(hProgBar, 0, 0x8D4B01);
			PROGBAR_SetBarColor(hProgBar, 1, 0xBB9B9B);
		}

		Disable_Interrupt(); 
		InitTrendQueue_Alias(hProgBar);
		Enable_Interrupt();
		GUI_SetFont(&GUI_Font_16);
		GUI_SetColor(0x4C1E3E);
		GUI_DispCharAt('%',404,450);
		for (i=0; i<=100; i++)
		{
			if(i != 100)
			{
				GUI_DispDecAt(i,387,450,2);
			}
			else
			{
				GUI_DispDecAt(i,382,450,3);
			}
			PROGBAR_SetValue(hProgBar, i);
			GUI_Delay(50);
			WM_Paint(hProgBar);
		}
		break; 

	case MSW_LOGO:
		hProgBar = PROGBAR_Create(0,407,800,7, WM_CF_SHOW);
		PROGBAR_SetTextFlag(hProgBar,1);

		if (LCD_GetDevCap(LCD_DEVCAP_BITSPERPIXEL) <= 4)
		{
			PROGBAR_SetBarColor(hProgBar, 0, 0x1872f3);
			PROGBAR_SetBarColor(hProgBar, 1, 0xCDCDCD);
		}
		else
		{
			PROGBAR_SetBarColor(hProgBar, 0, 0x1872f3);
			PROGBAR_SetBarColor(hProgBar, 1, 0xCDCDCD);
		}

		Disable_Interrupt(); 
		InitTrendQueue_Alias(hProgBar);
		Enable_Interrupt();
		GUI_SetFont(&GUI_Font32B_ASCII);
        GUI_SetBkColor(0x929497);
		GUI_SetColor(0xf7f7f7);
		GUI_DispCharAt('%',413,435);
        
		for (i=0; i<=100; i++)
		{
			if(i != 100)
			{
				GUI_DispDecAt(i,383,435,2);
			}
			else
			{
				GUI_DispDecAt(i,368,435,3);
			}
			PROGBAR_SetValue(hProgBar, i);
			GUI_Delay(50);
			WM_Paint(hProgBar);
		}
		break;
	case GALILEA_LOGO:
		hProgBar = PROGBAR_Create(0,413,800,8, WM_CF_SHOW);
		PROGBAR_SetTextFlag(hProgBar,1);
		if (LCD_GetDevCap(LCD_DEVCAP_BITSPERPIXEL) <= 4)
		{
			PROGBAR_SetBarColor(hProgBar, 0, 0xFFB107);
			PROGBAR_SetBarColor(hProgBar, 1, 0xA9A9A9);
		}
		else
		{
			PROGBAR_SetBarColor(hProgBar, 0, 0x07B1FF);
			PROGBAR_SetBarColor(hProgBar, 1, 0xA9A9A9);
		}

		//Disable_Interrupt(); 
		InitTrendQueue_Alias(hProgBar);
		//Enable_Interrupt();
		//GUI_SetFont(&GUI_Font_16);
		//GUI_SetColor(GUI_WHITE);
		//GUI_DispCharAt('%',404,398);
		for (i=0; i<=100; i++)
		{
			PROGBAR_SetValue(hProgBar, i);
			GUI_Delay(50);
			//WM_Paint(hProgBar);
		}
		break;
	case PENLON_LOGO:
		hProgBar = PROGBAR_Create(0,434,800,4, WM_CF_SHOW);
		PROGBAR_SetTextFlag(hProgBar,1);
		PROGBAR_SetBarColor(hProgBar, 0, 0x0000FF);
		PROGBAR_SetBarColor(hProgBar, 1, 0x767575);

 		InitTrendQueue_Alias(hProgBar);
 		LCD_SetColor(GUI_WHITE);
		GUI_SetFont(&GUI_Font_16);
		GUI_DispCharAt('%',407,450);
		for (i=0; i<=100; i++)
		{
            Battery_Collect(); 
			if(i == 100)
			{
				GUI_DispDecAt(i,384,450,3);
			}
			else
			{
				GUI_DispDecAt(i,390,450,2);
			}
			
			PROGBAR_SetValue(hProgBar, i);
			GUI_Delay(50);//15
		}
		break;
	case ANDROMEDA:
		hProgBar = PROGBAR_Create(0,434,800,4, WM_CF_SHOW);
		PROGBAR_SetTextFlag(hProgBar,1);
		PROGBAR_SetBarColor(hProgBar, 0, 0x854913);
		PROGBAR_SetBarColor(hProgBar, 1, 0xf5cf9b);
		InitTrendQueue_Alias(hProgBar);
		LCD_SetColor(GUI_BLACK);
		GUI_SetFont(&GUI_Font_16);
		GUI_DispCharAt('%',407,450);
		for (i=0; i<=100; i++)
		{
			if(i == 100)
			{
				GUI_DispDecAt(i,384,450,3);
			}
			else
			{
				GUI_DispDecAt(i,390,450,2);
			}

			PROGBAR_SetValue(hProgBar, i);
			GUI_Delay(50);//15
		}
		break;
	case PROMED_LOGO://ylk add 20190524
		hProgBar = PROGBAR_Create(0,420,800,4, WM_CF_SHOW);
		PROGBAR_SetTextFlag(hProgBar,1);

		PROGBAR_SetBarColor(hProgBar, 0, 0x2bb96e);
		PROGBAR_SetBarColor(hProgBar, 1, 0xd99800);

        InitTrendQueue_Alias(hProgBar);
		GUI_SetFont(&GUI_Font_16);			
		GUI_SetColor(0xd99800);
		GUI_DispCharAt('%',401,402);

		for (i=0; i<=100; i++)
		{
			if(i != 100)
			{
				GUI_DispDecAt(i,385,401,2);
			}
			else
			{
				GUI_DispDecAt(i,377, 402,3);
			}
			PROGBAR_SetValue(hProgBar, i);
			GUI_Delay(50);
			//WM_Paint(hProgBar);
		}      
		break;
	case ADVANCE_INC_LOGO:
		//hProgBar = PROGBAR_Create(0,434,800,4, WM_CF_SHOW);
		//PROGBAR_SetTextFlag(hProgBar,1);
		//PROGBAR_SetBarColor(hProgBar, 0, 0x854913);
		//PROGBAR_SetBarColor(hProgBar, 1, 0xf5cf9b);
		//InitTrendQueue_Alias(hProgBar);
		//LCD_SetColor(GUI_BLACK);
		//GUI_SetFont(&GUI_Font_16);
		//GUI_DispCharAt('%',407,450);
		//for (i=0; i<=100; i++)
		//{
		//	if(i == 100)
		//	{
		//		GUI_DispDecAt(i,384,450,3);
		//	}
		//	else
		//	{
		//		GUI_DispDecAt(i,390,450,2);
		//	}

		//	PROGBAR_SetValue(hProgBar, i);
		//	GUI_Delay(50);//15
		//}
		hProgBar = PROGBAR_Create(0,435,800,6, WM_CF_SHOW);
		PROGBAR_SetTextFlag(hProgBar,1);
		if (LCD_GetDevCap(LCD_DEVCAP_BITSPERPIXEL) <= 4)
		{
			PROGBAR_SetBarColor(hProgBar, 0, 0x8D4B01);
			PROGBAR_SetBarColor(hProgBar, 1, 0xBB9B9B);
		}
		else
		{
			PROGBAR_SetBarColor(hProgBar, 0, 0x8D4B01);
			PROGBAR_SetBarColor(hProgBar, 1, 0xBB9B9B);
		}

		Disable_Interrupt(); 
		InitTrendQueue_Alias(hProgBar);
		Enable_Interrupt();
		GUI_SetFont(&GUI_Font_16);
		GUI_SetColor(0x4C1E3E);
		GUI_DispCharAt('%',404,450);
		for (i=0; i<=100; i++)
		{
			if(i != 100)
			{
				GUI_DispDecAt(i,387,450,2);
			}
			else
			{
				GUI_DispDecAt(i,382,450,3);
			}
			PROGBAR_SetValue(hProgBar, i);
			GUI_Delay(50);
			WM_Paint(hProgBar);
		}
		break;

	case RN5NO_LOGO:		//[8/14/2023 gyh add]
		hProgBar = PROGBAR_Create(0,420,800,4, WM_CF_SHOW);
		PROGBAR_SetTextFlag(hProgBar,1);

		PROGBAR_SetBarColor(hProgBar, 0, 0xFCFE73);
		PROGBAR_SetBarColor(hProgBar, 1, GUI_BLACK);

		InitTrendQueue_Alias(hProgBar);
		GUI_SetFont(&GUI_Font_16);			
		GUI_SetColor(GUI_WHITE);
		GUI_DispCharAt('%',401,402);

		for (i=0; i<=100; i++)
		{
			if(i != 100)
			{
				GUI_DispDecAt(i,385,401,2);
			}
			else
			{
				GUI_DispDecAt(i,377, 402,3);
			}
			PROGBAR_SetValue(hProgBar, i);
			GUI_Delay(50);
		}      
		break;

	case WEX35_LOGO:		
		hProgBar = PROGBAR_Create(0,420,800,4, WM_CF_SHOW);
		PROGBAR_SetTextFlag(hProgBar,1);

		PROGBAR_SetBarColor(hProgBar, 0, 0xFCFE73);
		PROGBAR_SetBarColor(hProgBar, 1, 0xBD7D31);

		InitTrendQueue_Alias(hProgBar);
		GUI_SetFont(&GUI_Font_16);			
		GUI_SetColor(GUI_WHITE);
		GUI_DispCharAt('%',401,402);

		for (i=0; i<=100; i++)
		{
			if(i != 100)
			{
				GUI_DispDecAt(i,385,401,2);
			}
			else
			{
				GUI_DispDecAt(i,377, 402,3);
			}
			PROGBAR_SetValue(hProgBar, i);
			GUI_Delay(50);
		}      
		break;
	}

	AlarmList_Init();
	PROGBAR_Delete(hProgBar);
	GUI_SetBkColor(0x420606);
	GUI_Clear();
}
/*******************************************************************
*
*                 main
*
********************************************************************
*/
void Widget_Progbar(void)
{
    GUI_Clear();
    DemoProgBar();
}
