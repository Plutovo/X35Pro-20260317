#include "Data_Handle.h"


Int16U BatteryVoltageSample[FILTER_NUM] = {0};
/*
********************************************************************************
名称：DataFilterHandle_Battery
功能描述：对电池电量的ad值进行有小到大排序，并且获取其中最小的有效值。
入口参数：Pdata ：电池电量结构(数组)的首地址  Len：数据的长度
返回值：输入数据的中的最小有效值。
********************************************************************************
*/
#define EffectivePercent   (0.05)   //5%   某一有效数值在整个数据中占据的最小百分比，低于这个百分比的数值为无效
volatile unsigned short int BatteryVoltageDisplay;
volatile unsigned short int battery_tmp[100];
Int16U  DataFilterHandle_Battery(Int16U *Pdata,Int16U Len)
{
    volatile Int16U  index = 0;
    volatile Int16U  num = 0;
    volatile Int16U  temp = 0;
    volatile Int16U  max_num,max;
    if((Pdata != NULL)&&(Len >= 2))
    {
        //由小到大排序，数组最小的位置的值为最小。
        while(num < Len - 1)
        {
            max = *Pdata;
            max_num = 0;
            for(index = 1;index < Len - num;index++)
            {
                if(*(Pdata+index) > max)
                {
                    max = *(Pdata+index);
                    max_num = index;
                }
            }
            temp = *(Pdata+index-1);
            *(Pdata+index-1) = *(Pdata+max_num);
            *(Pdata+max_num) = temp;
            num++;
        }
				//memcpy((void *)battery_tmp,  Pdata,sizeof(battery_tmp));
        for(index=0;index<sizeof(battery_tmp)/2;index++)
        {
              battery_tmp[index]= *(Pdata + index)<< 2;
        }         
	
#ifdef IAR_LPC_1788_STK		
        BatteryVoltageDisplay= *(Pdata + 50);
#endif	  				
#if 0
        //根据排序的数据选择有效的数据返回
        if(Len * EffectivePercent < 1)//数据总长度不足以得到有效的百分比，则选择第二小的数据返回
        {
            max = *(Pdata + 1);//过滤掉最小值，返回第二小的数据
        }
        else
        {
            num = 0;
            max_num = 1;
            max = *Pdata;
            //按照从小到大的顺序，如果某一个值在当前的数据中所占的百分比大于等于有效百分比，那么返回这个数据
            while((num < Len)&&(max_num < Len * EffectivePercent))
            {
                num++;
                if(max == *(Pdata+num))
                {
                    max_num++;
                }
                else
                {
                    max = *(Pdata+num);
                    max_num = 1;
                }
            }
            if(num >= Len)
            {
                max = *(Pdata + (int)(Len * EffectivePercent));//如果没有符合有效百分比的数值，则返回第二小的数据
            }
        }
#endif
        max = *(Pdata);
    }
    else
    {
        max = 0;
    }
    return (max);
}
int BatteryAdFilter(int flag,int *BatteryVoltage)
{
    int ret = 0;
    //BatteryVoltageSample的下标依次为  0、100、200、300、400、500		
#ifdef IAR_LPC_1788_STK		
    *BatteryVoltage = DataFilterHandle_Battery(&BatteryVoltageSample[(flag-1)*(FILTER_NUM/6)],FILTER_NUM/6);
#endif	   
		if(flag == 6)
    {
        ret = DataFilterHandle_Battery(&BatteryVoltageSample[0],FILTER_NUM);//BatteryVoltageSample  600个数据从小到大排序，取最低的ad值50个进行均值。
        
    }
    return ret;
}


/*
********************************************************************************
名称：
功能描述：
入口参数：
********************************************************************************
*/
Int16U  CalculatePressureValue(Int16U data)
{
    Int16U temp=0;
    if(data>1024)
    {
        data = 1024;
    }
    if(data*2500/1024>400)
    {
        temp = (Int16U)(((double)(data)*2500/1024 - 400)* 6.895/22);
    }
    return (temp);
}
