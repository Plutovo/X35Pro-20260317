
#include "TEST.h"
//#include "EMC.h"
#include "stdlib.h"
unsigned char RTC_Time[10],Crystal_Time[10];
unsigned int buffer;
unsigned int block;
unsigned int error_num;
unsigned char SDRAM_TEST,FRAM_TEST;
unsigned int TEST_ITEM=0,TEST_Flag;
int Crystal_time_selece,Crystal_time_count,Crystal_time_Error;
/*TEST_ITEM_BIT:
 0 AUTO_TEST
 1 ONE_TEST
 2 SIGNAL_LINE_PAD
 3 FRAM
 4 UPDATA_ONLINE
 5 BDU_UART
 6 EFM_UART
 7 CO2_UART
 8 SPO2_UART
 9 ALARM
 10 Crystal
 */

#define size 128
#define ROM_SIZE1 128*1024
char     Color_block;
 
 
unsigned char factory_set=0xff;

unsigned char FM24C64_Test()      
{
  unsigned char i;
  unsigned int block;   


  for(block = 0; block < ROM_SIZE1 / 128 ; block++)
   	  for(i=0;i<128;i++)
         RTEEPROMwrite(128 * block + i, 0xff) ;


   return 1;
}



        