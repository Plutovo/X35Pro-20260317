#include "GUI_Font_cfg.h"
#include "string.h"
#ifdef __ICCARM__
#include "includes.h"
#endif

GUI_FONT GUI_Font_15;
GUI_FONT GUI_Font_16;
GUI_FONT GUI_Font_20;
GUI_FONT GUI_Font_24;
GUI_FONT GUI_Font_32;

void GUI_Font_Init(void)
{
#ifdef __ICCARM__
    
//    __disable_interrupt();
//    memcpy(FONT_SIF_15_RADDR, FONT_SIF_15_SADDR, FONT_SIF_15_SIZE);
//    memcpy(FONT_SIF_16_RADDR, FONT_SIF_16_SADDR, FONT_SIF_14_SIZE);
//    memcpy(FONT_SIF_20_RADDR, FONT_SIF_20_SADDR, FONT_SIF_14_SIZE);
//    memcpy(FONT_SIF_24_RADDR, FONT_SIF_24_SADDR, FONT_SIF_14_SIZE);
//    memcpy(FONT_SIF_32_RADDR, FONT_SIF_32_SADDR, FONT_SIF_14_SIZE);
//    __enable_interrupt();
    
	GUI_SIF_CreateFont(FONT_SIF_15_RADDR, &GUI_Font_15, GUI_SIF_TYPE_PROP);
	GUI_SIF_CreateFont(FONT_SIF_16_RADDR, &GUI_Font_16, GUI_SIF_TYPE_PROP);
	GUI_SIF_CreateFont(FONT_SIF_20_RADDR, &GUI_Font_20, GUI_SIF_TYPE_PROP);
	GUI_SIF_CreateFont(FONT_SIF_24_RADDR, &GUI_Font_24, GUI_SIF_TYPE_PROP);
	GUI_SIF_CreateFont(FONT_SIF_32_RADDR, &GUI_Font_32, GUI_SIF_TYPE_PROP);
#else
	GUI_Font_15 = *FONT_SIF_15_RADDR;
	GUI_Font_16 = *FONT_SIF_16_RADDR;
	GUI_Font_20 = *FONT_SIF_20_RADDR;
	GUI_Font_24 = *FONT_SIF_24_RADDR;
	GUI_Font_32 = *FONT_SIF_32_RADDR;
#endif
}

