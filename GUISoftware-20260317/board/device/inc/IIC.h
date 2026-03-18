#ifndef __IIC_H
#define __IIC_H
 #include "queue_manul.h"
#define IIC_SDA         (1 << 27 )      
#define IIC_CLK         (1 << 28 )     
#define SDA_H()	 {FIO0DIR |= IIC_SDA ;       FIO0PIN |= IIC_SDA;}	  //éèP2.00?a?3?(???????IIC_SDA=1
#define SCL_H()	 {FIO0DIR |= IIC_CLK ;       FIO0PIN |= IIC_CLK;}	  //éèP2.01?a?3???        IIC_CLK=1

#define SDA_L()	 {FIO0DIR |= IIC_SDA ;       FIO0PIN &= ~(IIC_SDA);}	  //éèP2.00?a?3???        IIC_SDA=0
#define SCL_L()	 {FIO0DIR |= IIC_CLK ;       FIO0PIN &= ~(IIC_CLK);}	  //éèP2.01?a?3???        IIC_CLK=0

#define IIC_PAGE 0x02        


void I2C_Start(void);
void I2C_Stop(void);
void I2C_delay(void);
void I2C_Ack(void);
void I2C_NoAck(void);
void I2C_WaitAck(void) ;
void I2C_SendByte(unsigned char SendByte);
unsigned char I2C_ReadByte(void);  //????????//
void IIC_Init(void);

void RTEEPROMwrite(unsigned  int  reg,unsigned char REG_data)	;
unsigned char RTEEPROMread(unsigned  int  reg);

void I2C0_MultiReceive(unsigned int reg,unsigned short int  *buffer,unsigned int num)  ;
void I2C0_MultiSend(unsigned int reg,unsigned short int *buffer,unsigned int num)  ;



#endif
