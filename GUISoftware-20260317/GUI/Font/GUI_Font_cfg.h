#ifndef _GUI_FONT_CFG_H
#define _GUI_FONT_CFG_H
#include "GUI.h"

#define KB  (1024UL)
#define MB  (1024UL*1024UL)

#ifdef __ICCARM__
#define FONT_SIF_BASE_ADDR  0x80440000
//14
#define FONT_SIF_15_SADDR	((unsigned int*)(0x80440000+4))
#define FONT_SIF_15_RADDR	((unsigned int*)(0x80440000+4))//((unsigned int*)0xA0C00000)
#define FONT_SIF_15_SIZE    (512*KB-4)
//16
#define FONT_SIF_16_SADDR	((unsigned int*)0x804C0000)
#define FONT_SIF_16_RADDR	((unsigned int*)0x804C0000)//((unsigned int*)0xA0C80000)
#define FONT_SIF_16_SIZE    (512*KB)
//20
#define FONT_SIF_20_SADDR	((unsigned int*)0x80540000)
#define FONT_SIF_20_RADDR	((unsigned int*)0x80540000)//((unsigned int*)0xA0D00000)
#define FONT_SIF_20_SIZE    (768*KB)
//24
#define FONT_SIF_24_SADDR	((unsigned int*)0x80600000)
#define FONT_SIF_24_RADDR	((unsigned int*)0x80600000)//((unsigned int*)0xA0DC0000)
#define FONT_SIF_24_SIZE    (1*MB)
//32
#define FONT_SIF_32_SADDR	((unsigned int*)0x80700000)
#define FONT_SIF_32_RADDR	((unsigned int*)0x80700000)//((unsigned int*)0xA0EC0000)
#define FONT_SIF_32_SIZE    (1*MB)

#define FONT_LIB_SIZE   (FONT_SIF_14_SIZE + \
                        FONT_SIF_16_SIZE + \
                        FONT_SIF_20_SIZE + \
                        FONT_SIF_24_SIZE + \
                        FONT_SIF_32_SIZE)

#else

//15
#define FONT_SIF_15_SADDR	&GUI_FontArial_Unicode_MS15
#define FONT_SIF_15_RADDR	&GUI_FontArial_Unicode_MS15
//16
#define FONT_SIF_16_SADDR	&GUI_FontArial_Unicode_MS16
#define FONT_SIF_16_RADDR	&GUI_FontArial_Unicode_MS16
//20
#define FONT_SIF_20_SADDR	&GUI_FontArial_Unicode_MS20
#define FONT_SIF_20_RADDR	&GUI_FontArial_Unicode_MS20
//24
#define FONT_SIF_24_SADDR	&GUI_FontArial_Unicode_MS24
#define FONT_SIF_24_RADDR	&GUI_FontArial_Unicode_MS24
//32
#define FONT_SIF_32_SADDR	&GUI_FontArial_Unicode_MS32
#define FONT_SIF_32_RADDR	&GUI_FontArial_Unicode_MS32

#endif

extern GUI_FONT GUI_Font_15;
extern GUI_FONT GUI_Font_16;
extern GUI_FONT GUI_Font_20;
extern GUI_FONT GUI_Font_24;
extern GUI_FONT GUI_Font_32;

void GUI_Font_Init(void);

#endif
