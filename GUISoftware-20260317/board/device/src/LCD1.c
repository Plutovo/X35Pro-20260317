#include "includes.h"
#include "LCD1.h"

void lcd_clear(u32 color)
{
    u32 *pDst = (u32 *)LCD_VRAM_BASE_ADDR;//显示数据基地址
    u32 i= 0;
    for( i = 0; (800 * 600) > i; i++)
    {
        *pDst++ = color;//初始化颜色
    }
}
 
void GLCD_Init(void)
{
  volatile int i;
  
  Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_LCD);
  
  LCD_CTRL = 0;   /* disable power */
  
#ifdef IAR_LPC_1788_STK
  LCD_CFG =2;  //clock divide
#endif
	
  LCD_TIMH= (LCD_HBP-1)<<24 | (LCD_HFP-1)<<16 | (LCD_HSW-1) <<8 | ((LCD_PPL/16) - 1)<<2;
  LCD_TIMV= (LCD_VBP-1)<<24 | (LCD_VFP-1)<<16 | (LCD_VSW-1) <<10 | (LCD_LPP - 1);
  //LCD_POL = 1<<26 | 1<<13 | (LCD_PPL-1)<<16 ;
  LCD_POL = 1<<26 | (LCD_PPL-1)<<16;
 
  LCD_UPBASE  =  LCD_VRAM_BASE_ADDR;
  LCD_LPBASE  =  LCD_VRAM_BASE_ADDR;    

  LCD_INTMSK = 1<<3;
  LCD_CTRL =  1<<12 | 1<<5 | 6<<1 | 1<<11 | 1;
  
  lcd_clear(0x000000);
}

void GLCD_Cursor_Cfg(int Cfg)
{
    CRSR_CFG = Cfg;
}

void GLCD_Cursor_En(int cursor)
{
    CRSR_CTRL_bit.CrsrNum = cursor;
    CRSR_CTRL_bit.CrsrOn = 1;
}

void GLCD_Cursor_Dis(int cursor)
{
    CRSR_CTRL_bit.CrsrOn = 0;
}

void GLCD_Move_Cursor(int x, int y)
{
    if(0 <= x)
    {
        //no clipping
        CRSR_CLIP_bit.CrsrClipX = 0;
        CRSR_XY_bit.CrsrX = x;
    }
    else
    {
        //clip x
        CRSR_CLIP_bit.CrsrClipX = -x;
        CRSR_XY_bit.CrsrX = 0;
    }

    if(0 <= y)
    {
        //no clipping
        CRSR_CLIP_bit.CrsrClipY = 0;
        CRSR_XY_bit.CrsrY = y;
    }
    else
    {
        //clip y
        CRSR_CLIP_bit.CrsrClipY = -y;
        CRSR_XY_bit.CrsrY = 0;
    }
}

void GLCD_Copy_Cursor (const Int32U *pCursor, int cursor, int size)
{
    Int32U * pDst = (Int32U *)LCD_CURSOR_BASE_ADDR;
    pDst += cursor*64;
    for(int i = 0; i < size ; i++)
    {
        *pDst++ = *pCursor++;
    }
}

void GLCD_SetPallet (const Int32U * pPallete)
{
    pInt32U pDst = (pInt32U)LCD_PAL_BASE;
    assert(pPallete);
    for (Int32U i = 0; i < 128; i++)
    {
        *pDst++ = *pPallete++;
    }
}

void GLCD_Ctrl (Boolean bEna)
{
    if (bEna)
    {
        LCD_CTRL_bit.LcdEn = 1;
        for(volatile Int32U i = C_GLCD_PWR_ENA_DIS_DLY; i; i--)
        {
            ;
        }
        LCD_CTRL_bit.LcdPwr= 1;   // enable power
    }
    else
    {
        LCD_CTRL_bit.LcdPwr= 0;   // disable power
        for(volatile Int32U i = C_GLCD_PWR_ENA_DIS_DLY; i; i--)
        {
            ;
        }
        LCD_CTRL_bit.LcdEn = 0;
    }
}       