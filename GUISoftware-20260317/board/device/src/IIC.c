#include "includes.h"
#include "IIC.h"
#include "TrendTable.h"
extern int ETCO2Unit;
void I2C_delay(void)
{                           
  
#ifdef IAR_LPC_1788_STK       //1788 ??15?nop??,????,??18?Nop
  asm("NOP"); asm("NOP"); asm("NOP");
  asm("NOP"); asm("NOP"); asm("NOP");	
  asm("NOP"); asm("NOP"); asm("NOP");
  asm("NOP"); asm("NOP"); asm("NOP");	
  asm("NOP"); asm("NOP"); asm("NOP");
  asm("NOP"); asm("NOP"); asm("NOP");
  
  
#endif
}

unsigned char  SCL_read()
{
  FIO0DIR &= ~(IIC_CLK); 
  I2C_delay();	
  return ((FIO0PIN >> 28) & 1);
}
unsigned char SDA_read()
{ 
  FIO0DIR &= ~(IIC_SDA); 
  I2C_delay() ;
  return ((FIO0PIN >> 27) & 1);	
}

void I2C_Start(void)
{
  SDA_H()
	I2C_delay();
  SCL_H()
	I2C_delay();	
  SDA_L()
	I2C_delay();	
  SCL_L()
	I2C_delay();	
}

void I2C_Stop(void)
{
  SCL_L()
	I2C_delay();	
  SDA_L()
	I2C_delay();	
  SCL_H()
	I2C_delay();	
  SDA_H()
	I2C_delay();	
} 

void I2C_Ack(void)
{	
  SCL_L()
	I2C_delay();	
  SDA_L()
	I2C_delay();	
  SCL_H()
	I2C_delay();	
  SCL_L()
	I2C_delay();	
}   

void I2C_NoAck(void)
{	
  SCL_L()
	I2C_delay();	
  SDA_H()
	I2C_delay();	
  SCL_H()
	I2C_delay();	
  SCL_L()
	I2C_delay();	
} 
  int i;
void I2C_WaitAck(void) 	 //???:=1?ACK,=0?ACK
{

  SCL_L()
	I2C_delay();	
  SDA_H()			
	I2C_delay();	
  SCL_H()
	I2C_delay();	
  while( (SDA_read()))
  {
    i++;
    if(i>100)
      break;
  }
  SCL_L()
	I2C_delay();	
}

void I2C_SendByte(unsigned char SendByte) //????????//
{
  unsigned char i=8;
  while(i--)
  {
    SCL_L()
      I2C_delay();	
    if(SendByte&0x80)
    {                    
      SDA_H()
    }
    else 
    {
      SDA_L()
    }  
    SendByte<<=1;
    I2C_delay();	
    SCL_H()
      I2C_delay();	
  }
  SCL_L()
}  

unsigned char I2C_ReadByte(void)  //????????//
{  
  unsigned char i=8;
  unsigned char ReceiveByte=0;
  
  SDA_H()				
	while(i--)
	{
      ReceiveByte<<=1;  
      SCL_L()
		I2C_delay();	
      SCL_H()
		I2C_delay();	
      if(SDA_read())
      {
        ReceiveByte|=0x01;
      }
	}
  SCL_L()
	return ReceiveByte;
} 

void RTEEPROMwrite(unsigned int reg,unsigned char REG_data)		 //void
{
  __disable_interrupt();
  FIO1PIN &= ~(1<<16);	
  I2C_Start();
  if(reg & 0x10000)
  	I2C_SendByte(0xA0 | IIC_PAGE);
  else
    I2C_SendByte(0xA0);
  
  I2C_WaitAck()  ;
  I2C_SendByte(0xff & reg>>8 );
  I2C_WaitAck();	
  I2C_SendByte(0xff & reg);
  I2C_WaitAck();			
  I2C_SendByte(REG_data);
  I2C_WaitAck();   
  I2C_Stop(); 
  FIO1PIN |= 1<<16;  
  __enable_interrupt();    
  
  
}

unsigned char RTEEPROMread(unsigned int  reg)
{   
  unsigned char REG_data; 
  __disable_interrupt();	
  I2C_Start();
  
  if(reg & 0x10000)
  	I2C_SendByte(0xA0 | IIC_PAGE);
  else
    I2C_SendByte(0xA0);
  
  I2C_WaitAck();
  I2C_SendByte(0xff & reg>>8 );
  I2C_WaitAck();	
  I2C_SendByte(0xff & reg);
  I2C_WaitAck();	
  
  I2C_Start();
  if(reg & 0x10000)
  	I2C_SendByte((0xA0 | IIC_PAGE) +1);
  else
    I2C_SendByte(0xA0 + 1);
  
  I2C_WaitAck();
  
  REG_data= I2C_ReadByte();
  I2C_NoAck();
  I2C_Stop();
  __enable_interrupt();  
  return REG_data;   
}	

void IIC_Init()
{
  FIO1PIN &= ~(1<<16); 	
}

void I2C0_MultiReceive(unsigned int reg,unsigned short int  *buffer,unsigned int num)  
{  
  unsigned int i; 
  
  unsigned char cnt_multiple = 0;
  unsigned char cnt_inter = 0;
  
  __disable_interrupt();
  I2C_Start();
  
  if(reg & 0x10000)
  	I2C_SendByte(0xA0 | IIC_PAGE);
  else
    I2C_SendByte(0xA0);
  
  I2C_WaitAck();
  I2C_SendByte(0xff & reg>>8 );
  I2C_WaitAck();	
  I2C_SendByte(0xff & reg);
  I2C_WaitAck();	
  
  I2C_Start();
  
  if(reg & 0x10000)
  	I2C_SendByte((0xA0 | IIC_PAGE) + 1);
  else
    I2C_SendByte(0xA0 + 1);
  
  I2C_WaitAck();
  
  //for(i=0;i< num-1 ; i++)
  for(i=0;i<num;i++)
  {
    if(i < INDEX_START_16U) //保存单字节参数
    {
      *(buffer + i)  = I2C_ReadByte();
      I2C_Ack();
    }
    else //保存双字节参数
    {
      unsigned char tmp;
      tmp= I2C_ReadByte();
      I2C_Ack();
      *(buffer + i)= (tmp<<8) | I2C_ReadByte();
      I2C_Ack();
    }
    
    if((i == MV)||(i == MVspn))          
    {
      cnt_inter = 10;
    }
    else
    {
      cnt_inter = 1;
    }
    
    if(((*(buffer + i) > ParaMaxLimit[i]*cnt_inter)||(*(buffer + i) < ParaMinLimit[i]*cnt_inter))&&(i != ETCO2))//过滤掉不在范围内的数据
    {
      *(buffer + i) = 0;
    }
    else if(i == ETCO2) 
    {
      if(ETCO2Unit == 1)//0~100
      {
        if((*(buffer + i)>=100) || (*(buffer + i) < 0))
        {
          *(buffer + i) = 0;
        }
      }
      else//0~13.3
      {
        if((*(buffer + i) >=133)||(*(buffer + i) < 0))
        {
          *(buffer + i) = 0;
        } 
      }
    }		
    
  } 
  *(buffer + i)= I2C_ReadByte();	
  I2C_NoAck();
  I2C_Stop();
    __enable_interrupt();        
} 


void I2C0_MultiSend(unsigned int reg,unsigned short int *buffer,unsigned int num)  
{  
  unsigned int i;  
  __disable_interrupt();
  FIO1PIN &= ~(1<<16);	
  I2C_Start();
  
  if(reg & 0x10000)
  	I2C_SendByte(0xA0 | IIC_PAGE);
  else
    I2C_SendByte(0xA0);
  
  I2C_WaitAck()  ;
  I2C_SendByte(0xff & reg>>8 );
  I2C_WaitAck();	
  I2C_SendByte(0xff & reg);
  I2C_WaitAck();			
  for(i = 0;i < num;i++)
  {
    if(i < INDEX_START_16U) //保存单字节参数
    {
      I2C_SendByte(*(buffer + i) & 0xff);   
      I2C_WaitAck(); 
    }
    else //保存双字节参数
    {
      I2C_SendByte((*(buffer + i) & 0xff00)>>8);
      I2C_WaitAck(); 
  	  I2C_SendByte(*(buffer + i) & 0xff);
      I2C_WaitAck(); 
  	}	
  }
  I2C_Stop();   
  FIO1PIN |= 1<<16;  
  __enable_interrupt();          
}  
