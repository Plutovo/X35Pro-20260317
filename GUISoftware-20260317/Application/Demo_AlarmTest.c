#include "Demo_AlarmTest.h"
//#include "Config.h"
#include "Alarm_handle.h"

#if ISDEMO_NOTNORMAL
#define DEMO_ALARM_NUM		ALARM_DRAGER_MODULEFAIL
#define DEMO_ALARM_NUM1		ALARM_SPO2_MODULE_OFF

unsigned char Demo_Alarm = 0;
Int64U Demo_AlarmTest;
Int64U Demo_AlarmTest1;
extern ALARM_WORK AlarmsWork;
extern ALARM_RECORD_LIST AlarmRecordList;
extern void EmptyAlarmRecordList(ALARM_RECORD_LIST* ListHead);

void DEMO_AlarmClear(void)
{

	AlarmsWork.presentAlarms = 0;
	AlarmsWork.newAlarms = 0;
	Demo_AlarmTest = DEMO_ALARM_NUM;
	AlarmsWork.newAlarms = Demo_AlarmTest;

	AlarmsWork.presentAlarms1 = 0;
	AlarmsWork.newAlarms1 = 0;
	Demo_AlarmTest1 = 0;
	AlarmsWork.newAlarms1 = Demo_AlarmTest1;

	EmptyAlarmRecordList(&AlarmRecordList);
}

void DEMO_TestAlarm(unsigned char direction)
{
	if(direction == 0x55)//right
	{
		//
		if (Demo_AlarmTest == DEMO_ALARM_NUM)
		{
			Demo_AlarmTest1 = 1;
			AlarmsWork.presentAlarms1 = 0; 
			AlarmsWork.unclearedAlarms1 = 0;
			AlarmsWork.newAlarms1 = Demo_AlarmTest1;
		}
		else
		{
			if (Demo_AlarmTest1!= (DEMO_ALARM_NUM1<<1))
			{
				Demo_AlarmTest1 <<= 1;
				AlarmsWork.presentAlarms1 = 0;
				if (Demo_AlarmTest1 > DEMO_ALARM_NUM1)
				{
					AlarmsWork.newAlarms1 = 0;
				}
				else 
				{
					AlarmsWork.newAlarms1 = Demo_AlarmTest1;
				}
			}
		}
		if (Demo_AlarmTest1 == DEMO_ALARM_NUM1)
		{
			Demo_AlarmTest = 1;
			AlarmsWork.presentAlarms = 0; 
			AlarmsWork.unclearedAlarms = 0;
			AlarmsWork.newAlarms = Demo_AlarmTest;
		}
		else
		{
			if (Demo_AlarmTest  != (DEMO_ALARM_NUM<<1))
			{
				Demo_AlarmTest <<= 1;
				AlarmsWork.presentAlarms = 0;
				if (Demo_AlarmTest > DEMO_ALARM_NUM)
				{
					AlarmsWork.newAlarms = 0;
				}
				else
				{
					AlarmsWork.newAlarms = Demo_AlarmTest;
				}
			}
		}
	}
	else if(direction == 0xDD)//left
	{

		//
		if (Demo_AlarmTest == 1)
		{
			Demo_AlarmTest1 = DEMO_ALARM_NUM1;
			AlarmsWork.presentAlarms1 = 0; 
			AlarmsWork.unclearedAlarms1 = 0;
			AlarmsWork.newAlarms1 = Demo_AlarmTest1;
		}
		else
		{
			if (Demo_AlarmTest1!=0)
			{
				Demo_AlarmTest1 >>= 1;
				AlarmsWork.presentAlarms1 = 0;
				AlarmsWork.newAlarms1 = Demo_AlarmTest1;
			}
		}
		if (Demo_AlarmTest1 == 1)
		{
			Demo_AlarmTest = DEMO_ALARM_NUM;
			AlarmsWork.presentAlarms = 0; 
			AlarmsWork.unclearedAlarms = 0;
			AlarmsWork.newAlarms = Demo_AlarmTest;
		}
		else
		{
			if (Demo_AlarmTest  != 0)
			{
				Demo_AlarmTest >>= 1;
				AlarmsWork.presentAlarms = 0;
				AlarmsWork.newAlarms = Demo_AlarmTest;
			}
		}
	}
}

#endif
