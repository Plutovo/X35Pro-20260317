#ifndef __DMA_H
#define __DMA_H

#define LCD2DMA_NUM 4000
#define LCD_BUFFER_NUM 6
#define LCD_BLOCK (GLCD_X_SIZE * GLCD_Y_SIZE * BPP) / (LCD2DMA_NUM * 4 )

extern  struct DMA_Link DMA_Link1[LCD_BUFFER_NUM][LCD_BLOCK];
void LCD_COPYBUFFER(int LayerIndex, int src,int dst);
void DMA_Init(void);
void DMA_Interrupt(void);
struct DMA_Link 
{
	volatile unsigned int src_addr;
	volatile unsigned int des_addr;
	volatile struct DMA_Link *LLI;
	volatile unsigned int control;
};
#endif
