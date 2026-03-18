#include "system_app.h"


extern int SpO2ModuleExist;    //SpO2配置文件   存在为1
extern int EtCO2ModuleExist;   //ETCO2配置文件  存在为1


/*
*铁电存储器检测数据
*/
#define MEMORY_TEST_NUM   5
U8 Memory_Test_Data[MEMORY_TEST_NUM] = {0xf1,0xf2,0xf3,0xf4,0xf5};//检测铁电是否能够正确存取数据

/*
*一般故障标志位
*/
static U8 LOW_FLAG = 0;
U8 ALL_read_data[MEMORY_TEST_NUM];////////////////////////////////////////djq


/*
 *
 *变量声明
 *
*/
extern REALTIME_PARA RealtimePara;
/*
 *
 *函数声明
 *
*/
extern Int8U GetBatteryVolume(Int16U BatteryVoltage);

static U8 _DisplayBoard_Test(void);
static U8 _BDUBoard_Test(void);
static U8 _EFBoard_Test(void);
static void _Final_Handing(U8);


static U8 __Memory_Test(void);
static U8 __Battery_Test(void);
static U8 __CO2Model_Test(void);
static U8 __SPO2Model_Test(void);

static void ____Memory_Data_Handing(const U8 *key_data,const U8 *dest_data,U8 data_num);
static U8 ___Memory_write_data(U16 mem_addr);
static U8 ___Memory_read_data(U16 mem_addr);
static void ___Battery_Data_Handing(U16 voltage);
static U8 ___CO2Model_Config_Detection(void);
static U8 ___SPO2Model_Config_Detection(void);



/***********************************************************
 *函数名：Internal_Test
 *参数  ：无
 *返回值：产生警告和故障的总数
 *被调用位置：
 *功能  ：执行内部测试程序，如果出现错误，存储到ALARM LOG中，
		  并显示到当前检测项目之后
		  接收到全局的取消命令之后能够直接退出
************************************************************
*/
U8 Internal_Test(void)
{
    U8 AlarmHigh_Flag = 0;
	Disp_CheckItem_Text(CONTEXT_APP.Checking_Position);//显示当前检测项目名称
//	_DisplayBoard_Test();	//co2   spo2需要看手册
    if(CONTEXT_APP.Cancel_Flag != 1)
    {
	   AlarmHigh_Flag += _BDUBoard_Test();
    }
    if(CONTEXT_APP.Cancel_Flag != 1)
    {
//	   AlarmHigh_Flag +=  _EFBoard_Test();
    }
	_Final_Handing(AlarmHigh_Flag);
    //保存结果到变量当中
	//******************补全****************
    //Save_ErrorNumber_Reault(MEM_INTER_OFFSET_ADDR,&(CONTEXT_APP.Current_ErrorCode),0);
	return 0;	//return error number
}



/***********************************************************
 *函数名：_DisplayBoard_Test
 *参数  ：无
 *返回值：是否产生错误  0:没有错误产生，1：产生错误
 *被调用位置：Internal_Test()
 *功能  ：显示板内部测试
************************************************************
*/
static U8 _DisplayBoard_Test(void)
{
    U8 return_value = 0;
	return_value += __Memory_Test();
	return_value += __Battery_Test();
//	return_value += __CO2Model_Test();
//	return_value += __SPO2Model_Test();
    return return_value;
}



/***********************************************************
 *函数名：__Memory_Test
 *参数  ：无
 *返回值：无错误返回0，有错误大于0
 *被调用位置：_DisplayBoard_Test()
 *功能  ：存储器测试
************************************************************
*/
static U8 __Memory_Test(void)
{
    U8 return_value = 0;
	if(___Memory_write_data(MEM_OF_TEST_ADDR)!=0)
    {
        return_value ++;
        Error_Handing_L(MEM_ERR_H);//..............
        Disp_Result(2,RESULT_FAULT);
	}
	if(___Memory_read_data(MEM_OF_TEST_ADDR)!=0)
    {
        return_value ++;
		Error_Handing_L(MEM_ERR_H);//..............
        Disp_Result(2,RESULT_FAULT);
	}
    return return_value;
}

/*
 *函数名：___Memory_write_data
 *参数  ：存储器的地址
 *返回值：0表示成功，1表示不成功
 *被调用位置：__Memory_Test()
 *功能  ：向指定地址发送指定的数据包
 *
*/
static U8 ___Memory_write_data(U16 mem_addr)
{
	U32 i = 0;
    U8  return_num = 0;
	while(i < MEMORY_TEST_NUM){
		return_num = Memory_write(mem_addr + i,Memory_Test_Data[i++]);
		//return_num = Memory_write(mem_addr + i*sizeof(U32),Memory_Test_Data[i++]);
        if(return_num){
            break; 
        }
	}
	//Memory_Test_Data
	//调用写存储器函数
	return return_num;//正确返回0
}
/*
 *函数名：___Memory_read_data
 *参数  ：存储器的地址
 *返回值：0表示成功，1表示不成功
 *被调用位置：__Memory_Test()
 *功能  ：从指定地址读取数据到指定的数据包
 *
*/
static U8 ___Memory_read_data(U16 mem_addr)
{
	U8 read_data[MEMORY_TEST_NUM];
	U32 i = 0;


	//调用读存储器函数
	while(i < MEMORY_TEST_NUM){
		read_data[i] = Memory_read(mem_addr + i);
	//	read_data[i] = Memory_read(mem_addr + i*sizeof(U8));
        ALL_read_data[i] = read_data[i];
		i++;
	}
                
	____Memory_Data_Handing(Memory_Test_Data,read_data,MEMORY_TEST_NUM);	//存  取数据对比
	return 0;//成功（不成功则不能运行到这）
}
/*
 *函数名：____Memory_Data_Handing
 *参数  ：发送的数据包，接受到的数据包
 *返回值：无
 *被调用位置：__Memory_Test()
 *功能  ：对比收发的数据包，如果出错，则进行故障处理
 *
*/

static void ____Memory_Data_Handing(const U8 *key_data,const U8 *dest_data,U8 data_num)
{
    I8 key_num = data_num-1;
    while((key_num) + 1){		//对比存  取数据
        if(*(key_data + key_num) != *(dest_data + key_num)){
            Error_Handing_H(MEM_ERR_H);//..............
            Disp_Result(2,RESULT_FAULT);
        }
        key_num--;
    }
}

/***********************************************************
 *函数名：__Battery_Test
 *参数  ：无
 *返回值：无错误返回0，有错误大于0
 *被调用位置：_DisplayBoard_Test()
 *功能  ：电池电压测试
************************************************************
*/
static U8 __Battery_Test(void)
{
    U8 return_value = 0;
	double voltage = 0;
	___Battery_Data_Handing(RealtimePara.BatteryVoltage);
    return return_value;
}

/*
 *函数名：___Battery_Data_Handing
 *参数  ：电池电压
 *返回值：无
 *被调用位置：__Battery_Test()
 *功能  ：根据电池电压的检测
 *
*/
static void ___Battery_Data_Handing(U16 voltage)
{
    if(GetBatteryVolume(voltage) == 1){	//电池电量耗尽
        LOW_FLAG++;
        Error_Handing_L(BATTERY_EMPTY_ERR_L);
    } else if(GetBatteryVolume(voltage) == 2){		//电池电量过低
        LOW_FLAG++;
        Error_Handing_L(BATTERY_LOW_ERR_L);
    }else if(voltage == 0){
        LOW_FLAG++;
        Error_Handing_L(BATTERY_UNKNOWN_ERR_L);	//未检测到电池
    }
}


/***********************************************************
 *函数名：__CO2Model_Test
 *参数  ：无
 *返回值：无
 *被调用位置：_DisplayBoard_Test()
 *功能  ：CO2模块测试
************************************************************
*/
static U8 __CO2Model_Test(void)
{
	U32 return_number = 0;
	if(___CO2Model_Config_Detection() == 0){//存在		
		if((return_number = Serial_Communaction_Handing(S_SEND_TO_CO2)) != 0){	
			//通讯部分的处理都在Serial_Communaction_Handing（）函数中完成
			//错误代码处理
			LOW_FLAG++;
			Error_Handing_L(return_number);
		}
	} else {
		LOW_FLAG++;
		Error_Handing_L(CO2_UNKNOWN_ERR_L);//CO2模块不存在
	}
    return 0;
}	
/*
 *函数名：___CO2Model_Config_Detection
 *参数  ：无
 *返回值：0模块存在，1模块不存在
 *被调用位置：__CO2Model_Test()
 *功能  ：检测配置文件中，模块是否存在
 *
*/
static U8 ___CO2Model_Config_Detection(void)
{
	U32 return_number = EtCO2ModuleExist;
	//调用读寄存器函数，读配置文件MEM_OF_CO2_ADDR地址，查看CO2模块是否存在
    
	return (return_number?0:1);
}

/*
 *函数名：__SPO2Model_Test
 *参数  ：无
 *返回值：无
 *被调用位置：_DisplayBoard_Test()
 *功能  ：SPO2模块测试
 * 
*/
static U8 __SPO2Model_Test(void)
{
	U32 return_number = 0;
	//调用读寄存器函数，读配置文件MEM_OF_SPO2_ADDR地址，查看SPO2模块是否存在
	if(___SPO2Model_Config_Detection() == 0){
		if((return_number = Serial_Communaction_Handing(S_SEND_TO_SPO2)) != 0){
			//错误代码处理
			LOW_FLAG++;
			Error_Handing_L(return_number);
		}	
	}else {
		LOW_FLAG++;
		Error_Handing_L(SPO2_UNKNOWN_ERR_L);//错误处理
	}
    return 0;
}

/*
 *函数名：___SPO2Model_Config_Detection
 *参数  ：无
 *返回值：0模块存在，1模块不存在
 *被调用位置：__SPO2Model_Test()
 *功能  ：检测配置文件中，模块是否存在
 *
*/
static U8 ___SPO2Model_Config_Detection(void)
{
	U32 return_number = EtCO2ModuleExist;
	//调用读寄存器函数，读配置文件MEM_OF_SPO2_ADDR地址，查看SPO2模块是否存在
	return (return_number?0:1);
}

/***********************************************************
 *函数名：_BDUBoard_Test
 *参数  ：无
 *返回值：无
 *被调用位置：Internal_Test()
 *功能  ：气路控制板内部测试
 ***********************************************************
*/
static U8 _BDUBoard_Test(void)
{
	U8 *return_number = NULL;
    U8 return_value = 0;
	//return_number = Serial_Communaction_Handing(BDU_TEST_START);
    if(CONTEXT_APP.Cancel_Flag != 1){

        if(return_number[0] != 0xff){//存在故障
            //错误代码处理,可能有多个错误需要处理
        
            while((*return_number) != 0xff){
                Error_Handing_L(*return_number);//这里应该是高级故障，但是因为有多个故障要接收，所以暂时写成这样
                return_number++;
            }
            return_value = 1;
            Disp_Result(2,RESULT_FAULT);
        }
            //通过检测
    }
    return return_value;
}



/***********************************************************
 *函数名：_EFBoard_Test
 *参数  ：无
 *返回值：无错误返回0，有错误大于0
 *被调用位置：Internal_Test()
 *功能  ：气路控制板内部测试
 ***********************************************************
*/
static U8 _EFBoard_Test(void)
{
	U8 *return_number = NULL;
    U8 return_value = 0;
//	return_number = Serial_Communaction_Handing(S_SEND_TO_EF);
    if(CONTEXT_APP.Cancel_Flag != 1){
        if(return_number[0] != 0xff){//存在故障
            //错误代码处理,可能有多个错误需要处理
        
            while((*return_number) != 0xff){
                Error_Handing_L(*return_number);//这里应该是高级故障，但是因为有多个故障要接收，所以暂时写成这样
                return_number++;
            }
            Disp_Result(2,RESULT_FAULT);
            return_value = 1;
        }
    }
    return return_value;
}

/***********************************************************
 *函数名：_Final_Handing
 *参数  ：高级报警标志，大于零则出现高级报警
 *返回值：无错误返回0，有错误大于0
 *被调用位置：Internal_Test()
 *功能  ：内部检测结束后，进行所有的故障处理，决定是否继续运行
 ***********************************************************
*/
static void _Final_Handing(U8 AlarmHigh_Flag)
{
	//整合所有的故障，如果没有严重故障，但是有轻微故障，询问用户是否继续运行，并且记录用户的选择
	//没有故障则直接退出。
    if((CONTEXT_APP.Cancel_Flag == 1)||(CONTEXT_APP.Global_CancelFlag == 1))
    {
        CONTEXT_APP.Cancel_Flag = 0;
        Disp_Result(2,RESULT_SKIP);
    }
    else 
    {
        if(AlarmHigh_Flag == 0)
        {
            if(LOW_FLAG)
            {
                if(Disp_Dialog_Internal_1() == 1)
                {//no
                    //Error_Handing_L();//可能多个错误
                    Disp_Result(2,RESULT_FAULT);
                }
                else 
                {
                    Disp_Result(2,RESULT_SKIP);//蜂鸣器错误
                }
            } 
            else
            {
                Disp_Result(2,RESULT_SUCCESS);
            }
        }
        else 
        {
            CONTEXT_APP.AlarmHigh_Flag = 1;
            Disp_Result(2,RESULT_FAULT);
        }
    }
}


