#ifndef __TEST_H
#define __TEST_H



extern unsigned char RTC_Time[10],Crystal_Time[10];
extern unsigned int TEST_ITEM,TEST_Flag;
extern int Crystal_time_count,Crystal_time_selece,Crystal_time_Error;
extern unsigned int RTC_Start_Record;


unsigned char SDRAM_Test (void);
unsigned char FM24C64_Test(void);
void crystal_Test(void);
void UART0_Test_Send(unsigned char *buffer,unsigned int MAX_NUM);
void UART1_Test_Send(unsigned char *buffer,unsigned int MAX_NUM);
void UART2_Test_Send(unsigned char *buffer,unsigned int MAX_NUM);
void UART3_Test_Send(void);
void UART4_Test_Send(unsigned char *buffer,unsigned int MAX_NUM);
void UART5_Test_Send(unsigned char *buffer,unsigned int MAX_NUM);
void UART_Test(unsigned char item);
void Select_test(unsigned int i,unsigned int state);

void test_init(void); 






extern unsigned short int crystal_time[3];
extern unsigned char uart_test_buffer[110];
extern volatile unsigned char UART_Check_Right;   
extern unsigned char UART_Check[6],UART_Test_Flag;
extern int rtc_test_count;

#endif
