#include "SoftLIC.h"

void SoftLIC_EraseID(void)
{
	Int8U Index;

	for(Index = 0;Index < 8;Index++)
	{
		RTEEPROMwrite(DEVICE_NUM_0_ADDR			+ Index,0);
		RTEEPROMwrite(DEVICE_NUM_0_ADDR_BACK	+ Index,0);
	}
}

void SoftLIC_Change(void)
{
	char SoftSN[8] = {0}, SoftID[8] = {0};
	char Index;

	if((RTEEPROMread(DEVICE_FLAG_HIGH_ADDR) != OPTIONAL_PASSWD_HEAD) \
		|| (RTEEPROMread(DEVICE_FLAG_LOW_ADDR)  != OPTIONAL_PASSWD_TAIL))//如果FRAM中没有序列号
	{
		if((RTEEPROMread(DEVICE_FLAG_HIGH_ADDR_BACK) != OPTIONAL_PASSWD_HEAD) \
			|| (RTEEPROMread(DEVICE_FLAG_LOW_ADDR_BACK)  != OPTIONAL_PASSWD_TAIL))//如果FRAM中没有序列号
		{
			//板子第一次烧程序
			SoftLIC_EraseID();
		}
		else//如果备份地址可用
		{
			for(Index = 0;Index < 8;Index++)
			{
				SoftID[Index] = RTEEPROMread(DEVICE_NUM_0_ADDR_BACK+Index);   
			}
		}
	}
	else//如果FRAM中已经存在软件序列号
	{
		for(Index = 0;Index < 8;Index++)
		{
			SoftID[Index] = RTEEPROMread(DEVICE_NUM_0_ADDR+Index);   
		} 
	}
}