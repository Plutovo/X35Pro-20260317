#ifndef __LCD1_H
#define __LCD1_H
#include "arm_comm.h"
typedef struct _Bmp_t
{
    Int32U  H_Size;
    Int32U  V_Size;
    Int32U  BitsPP;
    Int32U  BytesPP;
    pInt32U pPalette;
    pInt32U pPicStream;
    pInt8U  pPicDesc;
} Bmp_t, *pBmp_t;



//for LG-PHILIP
/*
#define C_GLCD_REFRESH_FREQ     (50HZ)
#define C_GLCD_H_SIZE           640
#define C_GLCD_H_PULSE          50
#define C_GLCD_H_FRONT_PORCH    12
#define C_GLCD_H_BACK_PORCH     12
#define C_GLCD_V_SIZE           480
#define C_GLCD_V_PULSE          50
#define C_GLCD_V_FRONT_PORCH    12
#define C_GLCD_V_BACK_PORCH     12
*/
// for SHARP DG52
/*
#define C_GLCD_REFRESH_FREQ     (50HZ)
#define C_GLCD_H_SIZE           800
#define C_GLCD_H_PULSE          240//72//240//
#define C_GLCD_H_FRONT_PORCH    400//35//400//
#define C_GLCD_H_BACK_PORCH     430//53//430//
#define C_GLCD_V_SIZE           480
#define C_GLCD_V_PULSE          6//6
#define C_GLCD_V_FRONT_PORCH    3//3
#define C_GLCD_V_BACK_PORCH     29//29
#define C_GLCD_PWR_ENA_DIS_DLY  10000//10000
#define C_GLCD_ENA_DIS_DLY      10000

#define CRSR_PIX_32     0
#define CRSR_PIX_64     1
#define CRSR_ASYNC      0
#define CRSR_FRAME_SYNC 2

*/
void GLCD_Init ();
void GLCD_SetPallet (const Int32U * pPallete);
void GLCD_Ctrl (Boolean bEna);
void GLCD_Cursor_Cfg(int Cfg);
void GLCD_Cursor_En(int cursor);
void GLCD_Cursor_Dis(int cursor);
void GLCD_Move_Cursor(int x, int y);
void GLCD_Copy_Cursor (const Int32U *pCursor, int cursor, int size);

#define BPP	2 //RGB565 2×Ö½Ú
#define GLCD_X_SIZE        800	         /* LCD Horizontal length */
#define GLCD_Y_SIZE        480           /* LCD Vertical length */
#define LCD_PPL                GLCD_X_SIZE   
#define LCD_LPP                GLCD_Y_SIZE 
#ifdef IAR_LPC_1788_STK
	#define LCD_HSW                1            
	#define LCD_HFP                5           
	#define LCD_HBP                256          
	#define LCD_VSW                1           
	#define LCD_VFP                5             
	#define LCD_VBP                28    
#endif

#define divide(num) ((num & 0x1F) | (((num >> 5) & 0x1F) << 27))
#define C_GLCD_CLK_PER_LINE     (C_GLCD_H_SIZE + C_GLCD_H_PULSE + C_GLCD_H_FRONT_PORCH + C_GLCD_H_BACK_PORCH)
#define C_GLCD_LINES_PER_FRAME  (C_GLCD_V_SIZE + C_GLCD_V_PULSE + C_GLCD_V_FRONT_PORCH + C_GLCD_V_BACK_PORCH)
#define C_GLCD_PIX_CLK          (35MHZ)

//extern Int32U SDRAM_BASE_ADDR;

#define LCD_VRAM_BASE_ADDR   SDRAM_BASE_ADDR
#define LCD_CURSOR_BASE_ADDR ((Int32U)0xFFE10800)
#define C_GLCD_PWR_ENA_DIS_DLY  10000//10000
#define C_GLCD_ENA_DIS_DLY      10000

#endif

