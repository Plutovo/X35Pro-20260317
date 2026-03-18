#include "inclde.h"
#include "DMA.h"

//struct DMA_Link DMA_Link1[LCD_BUFFER_NUM][LCD_BLOCK]@DMA_LINK;
/*
extern GUI_MEMDEV_Handle hMem1,hMem2;
void DMA_Init(void)
{
  int j;		
  volatile unsigned int addr1;
	
  DMACINTTCCLEAR = 3;                                
  DMACINTERRCLR  = 3;                              
  DMACCONFIGURATION = 1;                               

  DMA_Link1[0][0].control  = (unsigned int)(LCD2DMA_NUM | 2<<18 | 2<<21 | 3<<26);
  DMA_Link1[0][0].des_addr = (unsigned int)LCD_DISPLAY0 ;
  DMA_Link1[0][0].src_addr = (unsigned int)GUI_MEMDEV_GetDataPtr(hMem1);
  DMA_Link1[0][0].LLI = &DMA_Link1[0][1];
  for(j=1;j< LCD_BLOCK;j++)
  {
    DMA_Link1[0][j].control  = (unsigned int)(LCD2DMA_NUM | 2<<18 | 2<<21 | 3<<26 | 0x80000000);
    DMA_Link1[0][j].des_addr = DMA_Link1[0][j-1].des_addr + LCD2DMA_NUM * 4;
    DMA_Link1[0][j].src_addr = DMA_Link1[0][j-1].src_addr + LCD2DMA_NUM * 4;
    DMA_Link1[0][j].LLI = &DMA_Link1[0][j+1];
  }
  DMA_Link1[0][j-1].LLI = 0 ;
  
  DMACC0LLI = (unsigned int)&DMA_Link1[0][0];
  DMACC0CONTROL =  LCD2DMA_NUM | 2<<18 | 2<<21 | 3<<26 | 0x80000000;
  DMACC0DESTADDR = (unsigned int )(LCD_DISPLAY0 );
  DMACC0SRCADDR = (unsigned int)GUI_MEMDEV_GetDataPtr(hMem1);	
 
   /*
  DMA_Link1[0][0].control  = (unsigned int)(LCD2DMA_NUM | 2<<18 | 2<<21 | 3<<26);
  DMA_Link1[0][0].des_addr = (unsigned int)LCD_DISPLAY1 ;
  DMA_Link1[0][0].src_addr = (unsigned int)LCD_DISPLAY0;
  DMA_Link1[0][0].LLI = &DMA_Link1[0][1];
  
  DMA_Link1[1][0].control  = (unsigned int)(LCD2DMA_NUM | 2<<18 | 2<<21 | 3<<26);
  DMA_Link1[1][0].des_addr = (unsigned int)LCD_DISPLAY2 ;
  DMA_Link1[1][0].src_addr = (unsigned int)LCD_DISPLAY0;
  DMA_Link1[1][0].LLI = &DMA_Link1[1][1];  
  
  DMA_Link1[2][0].control  = (unsigned int)(LCD2DMA_NUM | 2<<18 | 2<<21 | 3<<26);
  DMA_Link1[2][0].des_addr = (unsigned int)LCD_DISPLAY0 ;
  DMA_Link1[2][0].src_addr = (unsigned int)LCD_DISPLAY1;
  DMA_Link1[2][0].LLI = &DMA_Link1[2][1];

  DMA_Link1[3][0].control  = (unsigned int)(LCD2DMA_NUM | 2<<18 | 2<<21 | 3<<26);
  DMA_Link1[3][0].des_addr = (unsigned int)LCD_DISPLAY2 ;
  DMA_Link1[3][0].src_addr = (unsigned int)LCD_DISPLAY1;
  DMA_Link1[3][0].LLI = &DMA_Link1[3][1];

  DMA_Link1[4][0].control  = (unsigned int)(LCD2DMA_NUM | 2<<18 | 2<<21 | 3<<26);
  DMA_Link1[4][0].des_addr = (unsigned int)LCD_DISPLAY0 ;
  DMA_Link1[4][0].src_addr = (unsigned int)LCD_DISPLAY2;
  DMA_Link1[4][0].LLI = &DMA_Link1[4][1];

  DMA_Link1[5][0].control  = (unsigned int)(LCD2DMA_NUM | 2<<18 | 2<<21 | 3<<26);
  DMA_Link1[5][0].des_addr = (unsigned int)LCD_DISPLAY1 ;
  DMA_Link1[5][0].src_addr = (unsigned int)LCD_DISPLAY2;
  DMA_Link1[5][0].LLI = &DMA_Link1[5][1];
 
  for(k=0;k<LCD_BUFFER_NUM;k++)
  {
    for(j=1;j< LCD_BLOCK;j++)
    {
      DMA_Link1[k][j].control  = (unsigned int)(LCD2DMA_NUM | 2<<18 | 2<<21 | 3<<26 | 0x80000000);
      DMA_Link1[k][j].des_addr = DMA_Link1[0][j-1].des_addr + LCD2DMA_NUM * 4;
      DMA_Link1[k][j].src_addr = DMA_Link1[0][j-1].src_addr + LCD2DMA_NUM * 4;
      DMA_Link1[k][j].LLI = &DMA_Link1[0][j+1];
    }
    DMA_Link1[k][j-1].LLI = 0 ;
  }
*/
//}
/*
unsigned char  DMA2LCD_Complish=1;
void DMA_Interrupt(void)
{
  if(DMACINTTCSTATUS & 1)
  {
    DMA2LCD_Complish=1;
    DMACINTTCCLEAR |= 1;
  } 
  VICADDRESS=0;  
}

/*
void LCD_COPYBUFFER(int LayerIndex, int src,int dst)
{

  unsigned int BufferSize,AddrSrc,AddrDst;
  BufferSize = (GLCD_X_SIZE * GLCD_Y_SIZE * BPP);

  //AddrSrc = LCD_DISPLAY0 + BufferSize * src;
  //AddrDst = LCD_DISPLAY0 + BufferSize * dst;
  //memcpy((void *)AddrDst, (void *)AddrSrc, BufferSize);
  
  
  if(src==0)
    switch(dst)
    {
    case 1: //0 to 1
      DMACC0LLI = (unsigned int)&DMA_Link1[0][0];
      DMACC0CONTROL =  LCD2DMA_NUM | 2<<18 | 2<<21 | 3<<26 | 0x80000000;
      DMACC0DESTADDR = (unsigned int )LCD_DISPLAY1;
      DMACC0SRCADDR = (unsigned int)LCD_DISPLAY0;	
      DMACC0CONFIGURATION = 1 | 1<<15;      
    break;
    case 2: //0 to 2
      DMACC0LLI = (unsigned int)&DMA_Link1[1][0];
      DMACC0CONTROL =  LCD2DMA_NUM | 2<<18 | 2<<21 | 3<<26 | 0x80000000;
      DMACC0DESTADDR = (unsigned int )LCD_DISPLAY2;
      DMACC0SRCADDR = (unsigned int)LCD_DISPLAY0;	
      DMACC0CONFIGURATION = 1 | 1<<15;      
    break;
    }
  
  if(src==1)
    switch(dst)
    {
    case 0: //1 to 0
      DMACC0LLI = (unsigned int)&DMA_Link1[2][0];
      DMACC0CONTROL =  LCD2DMA_NUM | 2<<18 | 2<<21 | 3<<26 | 0x80000000;
      DMACC0DESTADDR = (unsigned int )LCD_DISPLAY0;
      DMACC0SRCADDR = (unsigned int)LCD_DISPLAY1;	
      DMACC0CONFIGURATION = 1 | 1<<15;      
    break;
    case 2: //1 to 2
      DMACC0LLI = (unsigned int)&DMA_Link1[3][0];
      DMACC0CONTROL =  LCD2DMA_NUM | 2<<18 | 2<<21 | 3<<26 | 0x80000000;
      DMACC0DESTADDR = (unsigned int )LCD_DISPLAY2;
      DMACC0SRCADDR = (unsigned int)LCD_DISPLAY1;	
      DMACC0CONFIGURATION = 1 | 1<<15;      
    break;
    }
  
  if(src==2)
    switch(dst)
    {
    case 0: //2 to 0
      DMACC0LLI = (unsigned int)&DMA_Link1[2][0];
      DMACC0CONTROL =  LCD2DMA_NUM | 2<<18 | 2<<21 | 3<<26 | 0x80000000;
      DMACC0DESTADDR = (unsigned int )LCD_DISPLAY0;
      DMACC0SRCADDR = (unsigned int)LCD_DISPLAY2;	
      DMACC0CONFIGURATION = 1 | 1<<15;      
    break;
    case 1: //2 to 1
      DMACC0LLI = (unsigned int)&DMA_Link1[3][0];
      DMACC0CONTROL =  LCD2DMA_NUM | 2<<18 | 2<<21 | 3<<26 | 0x80000000;
      DMACC0DESTADDR = (unsigned int )LCD_DISPLAY1;
      DMACC0SRCADDR = (unsigned int)LCD_DISPLAY2;	
      DMACC0CONFIGURATION = 1 | 1<<15;      
    break;
    }

  
}*/