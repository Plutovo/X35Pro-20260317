#ifndef __I2C_VC_H
#define __I2C_VC_H
void RTEEPROMwrite(unsigned int reg,unsigned char REG_data);
unsigned char RTEEPROMread(unsigned int  reg);
void I2C0_MultiSend(unsigned int reg,unsigned short int *buffer,unsigned int num)  ;
void I2C0_MultiReceive(unsigned int reg,unsigned short int  *buffer,unsigned int num) ; 

#endif