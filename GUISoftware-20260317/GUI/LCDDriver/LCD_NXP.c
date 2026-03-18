#include <stddef.h>           /* needed for definition of NULL */
#include "LCD_Private.H"      /* private modul definitions & config */
#include "GUI_Private.H"
#include "GUIDebug.h"
#include "LCDConf.h"
#include "board.h"
//#include "EMC.h"
//********************************************************
//#include "sdram.h"
#include <stdlib.h>
//********************************************************
#define   LCDUSERCOLOR LCD_COLORINDEX
static U32 _ColorIndex2COLOR(int Index)
{
    static U32 _Color;
    static int _Index_Cache = -1;
    if (_Index_Cache != Index)
    {
        _Index_Cache = Index;
        _Color = LCD_Index2Color(Index);
    }
    return _Color;
}
void SetPixel(int x, int y, U32 color)
{
    *(Int16U*)(SDRAM_BASE_ADDR+(y*LCD_XSIZE+x)*2)= color;
}
unsigned int GetPixelIndex(int x, int y)
{
    return 1;
}
static void XorPixel   (int x, int y)
{
    LCD_PIXELINDEX Index = GetPixelIndex(x,y);
    SetPixel(x,y,LCD_NUM_COLORS-1-Index);
}
/*
*********************************************************
*                                                       *
*       LCD_L0_XorPixel                                 *
*                                                       *
*********************************************************
Purpose:  This routine is called by emWin. It writes 1 pixel into the
          display.
*/
void LCD_L0_XorPixel(int x, int y)
{
    XorPixel(x, y);
}
void LCD_L0_SetPixelIndex(int x, int y, int ColorIndex)
{
    SetPixel(x, y, ColorIndex);
}
/*
*********************************************************
*                                                       *
*          LCD_L0_DrawHLine optimized                   *
*                                                       *
*          16 bit bus, Using BITBLT                     *
*                                                       *
*********************************************************
*/
void LCD_L0_DrawHLine  (int x0, int y0,  int x1)
{
    if (GUI_Context.DrawMode & LCD_DRAWMODE_XOR)
    {
        while (x0 <= x1)
        {
            XorPixel(x0, y0);
            x0++;
        }
    }
    else
    {
        while (x0 <= x1)
        {
            SetPixel(x0, y0, LCDUSERCOLOR);
            x0++;
        }
    }
}
/*
*********************************************************
*                                                       *
*          LCD_L0_DrawVLine optimized                   *
*                                                       *
*          16 bit bus, using BITBLT                     *
*                                                       *
*********************************************************
*/
void LCD_L0_DrawVLine  (int x, int y0,  int y1)
{
    if (GUI_Context.DrawMode & LCD_DRAWMODE_XOR)
    {
        while (y0 <= y1)
        {
            XorPixel(x, y0);
            y0++;
        }
    }
    else
    {
        while (y0 <= y1)
        {
            SetPixel(x, y0, LCDUSERCOLOR);
            y0++;
        }
    }
}
/*
*********************************************************
*                                                       *
*          LCD_FillRect                                 *
*                                                       *
*********************************************************
*/
void LCD_L0_FillRect(int x0, int y0, int x1, int y1)
{
#if !LCD_SWAP_XY
    for (; y0 <= y1; y0++)
    {
        LCD_L0_DrawHLine(x0,y0, x1);
    }
#else
    for (; x0 <= x1; x0++)
    {
        LCD_L0_DrawVLine(x0,y0, y1);
    }
#endif
}
/*
**********************************************************
*                                                        *
*         Draw a line                                    *
*                                                        *
**********************************************************
*/
static void  DrawBitLine1BPP(int x, int y, U8 const*p, int Diff, int xsize,
                             const LCD_PIXELINDEX*pTrans)
{
    U16 Pixels = ((*p) << 8) | (*(p + 1));
    U8 RemPixels;
    p++;
    x += Diff;
    RemPixels = 16 - Diff;
    Pixels <<= Diff;
    if (GUI_Context.DrawMode & LCD_DRAWMODE_XOR)
    {
        do
        {
            if (RemPixels==0)
            {
                Pixels = ((*(p + 1)) << 8) | (*(p + 2));
                p += 2;
                RemPixels = 16;
            }
            if (Pixels & 0x8000)
            {
                XorPixel(x, y);
            }
            RemPixels--;
            Pixels <<=1;
            x++;
        }
        while (--xsize);
    }
    else if(GUI_Context.DrawMode & LCD_DRAWMODE_TRANS)
    {
        do
        {
            if (RemPixels==0)
            {
                Pixels = ((*(p + 1)) << 8) | (*(p + 2));
                p += 2;
                RemPixels = 16;
            }
            if (Pixels & 0x8000)
            {
                SetPixel(x, y, *(pTrans+1));
            }
            RemPixels--;
            Pixels <<=1;
            x++;
        }
        while (--xsize);
    }
    else
    {
        do
        {
            if (RemPixels==0)
            {
                Pixels = ((*(p + 1)) << 8) | (*(p + 2));
                p += 2;
                RemPixels = 16;
            }
            if (Pixels & 0x8000)
            {
                SetPixel(x, y, *(pTrans+1));
            }
            else
            {
                SetPixel(x, y, *pTrans);
            }
            RemPixels--;
            Pixels <<=1;
            x++;
        }
        while (--xsize);
    }
}
/*
    *********************************************
    *                                           *
    *      Draw Bitmap 2 BPP                    *
    *                                           *
    *********************************************
*/
#if (LCD_MAX_LOG_COLORS > 2)
static void  DrawBitLine2BPP(int x, int y, U8 const*p, int Diff, int xsize,
                             const LCD_PIXELINDEX*pTrans)
{
    LCD_PIXELINDEX pixels;
    /*
    // Jump to right entry point
    */
    pixels = *p;
    if (GUI_Context.DrawMode & LCD_DRAWMODE_TRANS) switch (Diff&3)
        {
        case 0:
            goto WriteTBit0;
        case 1:
            goto WriteTBit1;
        case 2:
            goto WriteTBit2;
        default:
            goto WriteTBit3;
        }
    else switch (Diff&3)
        {
        case 0:
            goto WriteBit0;
        case 1:
            goto WriteBit1;
        case 2:
            goto WriteBit2;
        default:
            goto WriteBit3;
        }
    /*
            Write without transparency
    */
WriteBit0:
    SetPixel(x+0, y, *(pTrans+(pixels>>6)));
    if (!--xsize)
    {
        return;
    }
WriteBit1:
    SetPixel(x+1, y, *(pTrans+(3&(pixels>>4))));
    if (!--xsize)
    {
        return;
    }
WriteBit2:
    SetPixel(x+2, y, *(pTrans+(3&(pixels>>2))));
    if (!--xsize)
    {
        return;
    }
WriteBit3:
    SetPixel(x+3, y, *(pTrans+(3&(pixels))));
    if (!--xsize)
    {
        return;
    }
    pixels = *(++p);
    x+=4;
    goto WriteBit0;
    /*
            Write with transparency
    */
WriteTBit0:
    if (pixels&(3<<6))
    {
        SetPixel(x+0, y, *(pTrans+(pixels>>6)));
    }
    if (!--xsize)
    {
        return;
    }
WriteTBit1:
    if (pixels&(3<<4))
    {
        SetPixel(x+1, y, *(pTrans+(3&(pixels>>4))));
    }
    if (!--xsize)
    {
        return;
    }
WriteTBit2:
    if (pixels&(3<<2))
    {
        SetPixel(x+2, y, *(pTrans+(3&(pixels>>2))));
    }
    if (!--xsize)
    {
        return;
    }
WriteTBit3:
    if (pixels&(3<<0))
    {
        SetPixel(x+3, y, *(pTrans+(3&(pixels))));
    }
    if (!--xsize)
    {
        return;
    }
    pixels = *(++p);
    x+=4;
    goto WriteTBit0;
}
#endif
/*
    *********************************************
    *                                           *
    *      Draw Bitmap 4 BPP                    *
    *                                           *
    *********************************************
*/
#if (LCD_MAX_LOG_COLORS > 4)
static void  DrawBitLine4BPP(int x, int y, U8 const*p, int Diff, int xsize,
                             const LCD_PIXELINDEX*pTrans)
{
    U8 pixels;
    /*
    // Jump to right entry point
    */
    pixels = *p;
    if (GUI_Context.DrawMode & LCD_DRAWMODE_TRANS)
    {
        if ((Diff&1) ==0)
        {
            goto WriteTBit0;
        }
        goto WriteTBit1;
    }
    else
    {
        if ((Diff&1) ==0)
        {
            goto WriteBit0;
        }
        goto WriteBit1;
    }
    /*
            Write without transparency
    */
WriteBit0:
    SetPixel(x+0, y, *(pTrans+(pixels>>4)));
    if (!--xsize)
    {
        return;
    }
WriteBit1:
    SetPixel(x+1, y, *(pTrans+(pixels&0xf)));
    if (!--xsize)
    {
        return;
    }
    x+=2;
    pixels = *(++p);
    goto WriteBit0;
    /*
            Write with transparency
    */
WriteTBit0:
    if (pixels>>4)
    {
        SetPixel(x+0, y, *(pTrans+(pixels>>4)));
    }
    if (!--xsize)
    {
        return;
    }
WriteTBit1:
    if (pixels&0xf)
    {
        SetPixel(x+1, y, *(pTrans+(pixels&0xf)));
    }
    if (!--xsize)
    {
        return;
    }
    x+=2;
    pixels = *(++p);
    goto WriteTBit0;
}
#endif
/*
    *********************************************
    *                                           *
    *      Draw Bitmap 8 BPP  (256 colors)      *
    *                                           *
    *********************************************
*/
#if (LCD_MAX_LOG_COLORS > 16)
static void  DrawBitLine8BPP(int x, int y, U8 const*p, int xsize, const
                             LCD_PIXELINDEX*pTrans)
{
    LCD_PIXELINDEX pixel;
    if ((GUI_Context.DrawMode & LCD_DRAWMODE_TRANS)==0)
    {
        if (pTrans)
        {
            for (; xsize > 0; xsize--,x++,p++)
            {
                pixel = *p;
                SetPixel(x, y, *(pTrans+pixel));
            }
        }
        else
        {
            for (; xsize > 0; xsize--,x++,p++)
            {
                SetPixel(x, y, *p);
            }
        }
    }
    else       /* Handle transparent bitmap */
    {
        if (pTrans)
        {
            for (; xsize > 0; xsize--, x++, p++)
            {
                pixel = *p;
                if (pixel)
                {
                    SetPixel(x+0, y, *(pTrans+pixel));
                }
            }
        }
        else
        {
            for (; xsize > 0; xsize--, x++, p++)
            {
                pixel = *p;
                if (pixel)
                {
                    SetPixel(x+0, y, pixel);
                }
            }
        }
    }
}
#endif
/***************************************************************************************************
*
*       _DrawBitLine16BPP
*********************************************************************/
#if (LCD_BITSPERPIXEL > 8)
static void DrawBitLine16BPP(int x, int y, U16 const*p, int xsize, const LCD_PIXELINDEX*pTrans)
{
    LCD_PIXELINDEX pixel;
    if ((GUI_Context.DrawMode & LCD_DRAWMODE_TRANS)==0)
    {
        if (pTrans)
        {
            for (; xsize > 0; xsize--,x++,p++)
            {
                pixel = *p;
                SetPixel(x, y, *(pTrans+pixel));
            }
        }
        else
        {
            for (; xsize > 0; xsize--,x++,p++)
            {
                SetPixel(x, y, *p);
            }
        }
    }
    else       /* Handle transparent bitmap */
    {
        if (pTrans)
        {
            for (; xsize > 0; xsize--, x++, p++)
            {
                pixel = *p;
                if (pixel)
                {
                    SetPixel(x+0, y, *(pTrans+pixel));
                }
            }
        }
        else
        {
            for (; xsize > 0; xsize--, x++, p++)
            {
                pixel = *p;
                if (pixel)
                {
                    SetPixel(x+0, y, pixel);
                }
            }
        }
    }
}
#endif
/*
*********************************************************
*                                                       *
*         Universal draw Bitmap routine                 *
*                                                       *
*********************************************************
*/
void LCD_L0_DrawBitmap   (int x0, int y0,
                          int xsize, int ysize,
                          int BitsPerPixel,
                          int BytesPerLine,
                          const U8* pData, int Diff,
                          const LCD_PIXELINDEX* pTrans)
{
    int i;
    for (i=0; i<ysize; i++)
    {
        switch (BitsPerPixel)
        {
        case 1:
            DrawBitLine1BPP(x0, i+y0, pData, Diff, xsize, pTrans);
            break;
#if (LCD_MAX_LOG_COLORS > 2)
        case 2:
            DrawBitLine2BPP(x0, i+y0, pData, Diff, xsize, pTrans);
            break;
#endif
#if (LCD_MAX_LOG_COLORS > 4)
        case 4:
            DrawBitLine4BPP(x0, i+y0, pData, Diff, xsize, pTrans);
            break;
#endif
#if (LCD_MAX_LOG_COLORS > 16)
        case 8:
            DrawBitLine8BPP(x0, i+y0, pData, xsize, pTrans);
            break;
#endif
#if (LCD_BITSPERPIXEL > 8)
        case 16:
            DrawBitLine16BPP(x0, i+y0, (const U16 *)pData, xsize, pTrans);
            break;
#endif
        }
        pData += BytesPerLine;
    }
}
/********************************************************
*                                                       *
*       LCD_L0_SetOrg                                   *
*                                                       *
*********************************************************

Purpose:        Sets the original position of the virtual display.
                Has no function at this point with the PC-driver.
*/
int OrgX, OrgY;
void LCD_L0_SetOrg(int x, int y)
{
    OrgX = x;
    OrgY = y;
}
/*
*********************************************************
*                                                       *
*       LCD_On                                          *
*       LCD_Off                                         *
*                                                       *
*********************************************************
*/
void LCD_Off          (void)
{
}
void LCD_On           (void)
{
}
void LCD_Init_Controler()
{
}
unsigned int LCD_L0_GetPixelIndex(int x, int y)
{
    return GetPixelIndex(x,y);
}
/*
*********************************************************
*                                                       *
*                   LCD_L0_SetLUTEntry                  *
*                                                       *
*********************************************************
*/
void LCD_L0_SetLUTEntry(U8 Pos, LCD_COLOR color)
{
}
/*
*********************************************************
*                                                       *
*       LCD_L0_ReInit : Re-Init the display             *
*                                                       *
*********************************************************
ReInit contains all of the code that can be re-executed at any point without
changing or even disturbing what can be seen on the LCD.
Note that it is also used as a subroutine by LCD_Init().
*/
void  LCD_L0_ReInit(void)
{
    LCD_Init_Controler();                     /* macro defined in config */
}
/*
*********************************************************
*                                                       *
*       LCD_Init : Init the display                     *
*                                                       *
*********************************************************
*/
int LCD_L0_Init(void)
{
    GUI_DEBUG_LOG("\nLCD_Init()");
    LCD_L0_ReInit();
    LCD_Off();
    return 0;    /* Init successfull ! */
}
/*
*********************************************************
*                                                       *
*       LCD_L0_CheckInit                                *
*                                                       *
*  Check if display controller(s) is/are still          *
*  properly initialized                                 *
*                                                       *
*********************************************************
Return value:   0 => No error, init is O.K.
*/
int LCD_L0_CheckInit(void)
{
    return 0;
}
