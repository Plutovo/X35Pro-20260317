#ifndef _BDU_COMM_H_
#define _BDU_COMM_H_

typedef enum
{
  GAS_Channel_O2 = 0,
}GAS_Channel_t;


typedef struct ModuleComm 
{
	unsigned short BDU_CommFlag;
	unsigned short GAS_CommFlag;
// 	unsigned short EGM_CommFlag;
//  unsigned short EGM_CommRstCount;
//  unsigned char  EGM_CommRstFlag;
}MODULECOMM;

typedef enum
{
	AC_WORK = 0x11,
	BAT_WORK = 0x22,
}Power_Work_t;

#define Filter_NumP 15   //递推平均滤波 压力数据采样个数  //wff add 20190813
#define Filter_NumF 15    //递推平均滤波 流量数据采样个数
#define Filter_NumV 15    //递推平均滤波 容积数据采样个数
#define Ceilc(n,m)  (n + m -1)/m    //向上取整宏定义实现

extern MODULECOMM Module_Comm;

void BDU_Comm_Set(unsigned short data);
void GAS_Comm_Set(unsigned short data);
unsigned short BDU_Comm_Get(void);
unsigned short GAS_Comm_Get(void);
unsigned char BDU_CommFault_Get(void);
unsigned char GAS_CommFault_Get(void);
void Check_CommFault(void);
float Data_Rounding(float data, int narrow_mul,unsigned char DecDig);
unsigned int IE_readRatio(unsigned int idx);

int Filter_RecvAver(int data, int *buf, int num); //wff add 20190813
int Filter_RecvAverP(int data);
int Filter_RecvAverF(int data);
int Filter_RecvAverV(int data);
#endif