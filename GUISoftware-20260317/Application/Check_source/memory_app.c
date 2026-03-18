#include "memory_app.h"
extern RTC_TIMER RTCReadTime();


/********************************************************
 *函数名：Memory_write
 *参数  ：存储器的地址，要发送的数据
 *返回值：0表示成功，1表示不成功
 *被调用位置：
 *功能  ：向指定地址写入指定的数据，并且改变环境变量
*********************************************************
*/
U8 Memory_write(U16 mem_addr,const U8 data)
{
	return !RTEEPROMwrite((U16)mem_addr,data);
}


/********************************************************
 *函数名：Memory_read
 *参数  ：存储器的地址，接收数据存储的位置
 *返回值：0表示成功，1表示不成功
 *被调用位置：
 *功能  ：从指定的地址读取数据
*********************************************************
*/
U8 Memory_read(U16 mem_addr)
{
	return RTEEPROMread(mem_addr);
}

/********************************************************
 *函数名：Memory_delete
 *参数  ：要删除数据存储器的地址
 *返回值：0表示成功，1表示不成功
 *被调用位置：
 *功能  ：删除指定地址的数据，并且改变环境变量
*********************************************************
*/
U8 Memory_delete(U16 mem_addr)
{
	return 0;
}

/********************************************************
 *函数名：Memory_delete
 *参数  ：无
 *返回值：无
 *被调用位置：每次退出检测的时候
 *功能  ：保存这一次检测的内容
*********************************************************
*/
void Memory_Save_CheckResult(int CheckItem_Flag)
{

    U8   cnt;
    U8   tmp;
    U64  Error_Code;
    U8   Final_State = 0;
    RTC_TIMER time = RTCReadTime();
    if(CheckItem_Flag == LEFT_CHECKFLAG)
    {
        RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_MANUAL_OFFSET_ADDR,CONTEXT_APP.CheckInfo.Manual.Error_Code.Bit_8);
        RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_MANUAL_OFFSET_ADDR + 1,CONTEXT_APP.CheckInfo.Manual.Check_result[1]);
		RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_MANUAL_OFFSET_ADDR + 2,CONTEXT_APP.CheckInfo.Manual.Check_result[0]);

		RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_MANUAL_YEAR_OFFSET_ADDR,(time.RTC_Year%100)); //年份的低两位
        RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_MANUAL_MONTH_OFFSET_ADDR,time.RTC_Mon);
        RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_MANUAL_DAY_OFFSET_ADDR,time.RTC_Mday);
        RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_MANUAL_HOUR_OFFSET_ADDR,time.RTC_Hour);
        RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_MANUAL_MIN_OFFSET_ADDR,time.RTC_Min);

		//获取所有结果的状态标志,并且将相应的状态加入到相应的变量当中
        Final_State = ((CONTEXT_APP.CheckInfo.Manual.Check_result[1] & 0xC0));
        CONTEXT_APP.CheckInfo.Result_Report._Manual[Final_State>>6]++;
		RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_LEFT_MANUAL_OFFSET_ADDR + (RESULT_SUCCESS>>2),CONTEXT_APP.CheckInfo.Result_Report._Manual[(RESULT_SUCCESS>>2)]);
        RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_LEFT_MANUAL_OFFSET_ADDR + (RESULT_FAULT>>2),CONTEXT_APP.CheckInfo.Result_Report._Manual[(RESULT_FAULT>>2)]);
        RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_LEFT_MANUAL_OFFSET_ADDR + (RESULT_SKIP>>2),CONTEXT_APP.CheckInfo.Result_Report._Manual[(RESULT_SKIP>>2)]);  

    }
    else if(CheckItem_Flag == RIGHT_CHECKFLAG)
    {
        //djq add 20160809
        RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_BAROMETER_OFFSET_ADDR,CONTEXT_APP.CheckInfo.Barometer.Error_Code.Bit_8 );
        RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_BAROMETER_OFFSET_ADDR + 1,CONTEXT_APP.CheckInfo.Barometer.Check_result[1]);
        RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_BAROMETER_OFFSET_ADDR + 2,CONTEXT_APP.CheckInfo.Barometer.Check_result[0]);

        RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_GASO2_OFFSET_ADDR,CONTEXT_APP.CheckInfo.GasO2.Error_Code.Bit_8 );
        RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_GASO2_OFFSET_ADDR + 1,CONTEXT_APP.CheckInfo.GasO2.Check_result[1]);
        RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_GASO2_OFFSET_ADDR + 2,CONTEXT_APP.CheckInfo.GasO2.Check_result[0]);

        RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_FLOW_OFFSET_ADDR,CONTEXT_APP.CheckInfo.Flow_S.Error_Code.Bit_8);
        RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_FLOW_OFFSET_ADDR + 1,CONTEXT_APP.CheckInfo.Flow_S.Check_result[1]);
        RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_FLOW_OFFSET_ADDR + 2,CONTEXT_APP.CheckInfo.Flow_S.Check_result[0]);

        RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_PRESSURE_OFFSET_ADDR,CONTEXT_APP.CheckInfo.Pressure_S.Error_Code.Bit_8);
        RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_PRESSURE_OFFSET_ADDR + 1,CONTEXT_APP.CheckInfo.Pressure_S.Check_result[1]);
        RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_PRESSURE_OFFSET_ADDR + 2,CONTEXT_APP.CheckInfo.Pressure_S.Check_result[0]);

		//RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_FG_FLOW_OFFSET_ADDR,CONTEXT_APP.CheckInfo.FG_Flow_S.Error_Code.Bit_8);
  //      RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_FG_FLOW_OFFSET_ADDR + 1,CONTEXT_APP.CheckInfo.FG_Flow_S.Check_result[1]);
  //      RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_FG_FLOW_OFFSET_ADDR + 2,CONTEXT_APP.CheckInfo.FG_Flow_S.Check_result[0]);

        RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_COMPLIANCE_OFFSET_ADDR,CONTEXT_APP.CheckInfo.Compliance.Error_Code.Bit_8);
        RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_COMPLIANCE_OFFSET_ADDR + 1,CONTEXT_APP.CheckInfo.Compliance.Check_result[1]);
        RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_COMPLIANCE_OFFSET_ADDR + 2,CONTEXT_APP.CheckInfo.Compliance.Check_result[0]);

        RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_INTERNAL_LEAKAGE_OFFSET_ADDR,CONTEXT_APP.CheckInfo.Internal_Leakage.Error_Code.Bit_8);
        RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_INTERNAL_LEAKAGE_OFFSET_ADDR + 1,CONTEXT_APP.CheckInfo.Internal_Leakage.Check_result[1]);
        RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_INTERNAL_LEAKAGE_OFFSET_ADDR + 2,CONTEXT_APP.CheckInfo.Internal_Leakage.Check_result[0]);
    
        RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_OXYGEN_OFFSET_ADDR,CONTEXT_APP.CheckInfo.Oxygen.Error_Code.Bit_8);
        RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_OXYGEN_OFFSET_ADDR + 1,CONTEXT_APP.CheckInfo.Oxygen.Check_result[1]);
        RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_OXYGEN_OFFSET_ADDR + 2,CONTEXT_APP.CheckInfo.Oxygen.Check_result[0]);

        RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_BATTERY_OFFSET_ADDR,CONTEXT_APP.CheckInfo.Battery.Error_Code.Bit_8);
        RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_BATTERY_OFFSET_ADDR + 1,CONTEXT_APP.CheckInfo.Battery.Check_result[1]);
        RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_BATTERY_OFFSET_ADDR + 2,CONTEXT_APP.CheckInfo.Battery.Check_result[0]);

        //RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_ACGO_OFFSET_ADDR,CONTEXT_APP.CheckInfo.ACGO_Switch.Error_Code.Bit_8);
        //RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_ACGO_OFFSET_ADDR + 1,CONTEXT_APP.CheckInfo.ACGO_Switch.Check_result[1]);
        //RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_ACGO_OFFSET_ADDR + 2,CONTEXT_APP.CheckInfo.ACGO_Switch.Check_result[0]);
        //RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_POP_OFF_OFFSET_ADDR,CONTEXT_APP.CheckInfo.Pop_off_Valve.Error_Code.Bit_8);
        //RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_POP_OFF_OFFSET_ADDR + 1,CONTEXT_APP.CheckInfo.Pop_off_Valve.Check_result[1]);
        //RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_POP_OFF_OFFSET_ADDR + 2,CONTEXT_APP.CheckInfo.Pop_off_Valve.Check_result[0]);
        //RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_PEEP_OFFSET_ADDR,CONTEXT_APP.CheckInfo.PEEP_Valve.Error_Code.Bit_8);
        //RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_PEEP_OFFSET_ADDR + 1,CONTEXT_APP.CheckInfo.PEEP_Valve.Check_result[1]);
        //RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_PEEP_OFFSET_ADDR + 2,CONTEXT_APP.CheckInfo.PEEP_Valve.Check_result[0]);
        //RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_SAFETY_OFFSET_ADDR,CONTEXT_APP.CheckInfo.Safety_Valve.Error_Code.Bit_8);
        //RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_SAFETY_OFFSET_ADDR + 1,CONTEXT_APP.CheckInfo.Safety_Valve.Check_result[1]);
        //RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_SAFETY_OFFSET_ADDR + 2,CONTEXT_APP.CheckInfo.Safety_Valve.Check_result[0]);



        RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_AUTOMATIC_YEAR_OFFSET_ADDR,(time.RTC_Year%100));
        RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_AUTOMATIC_MONTH_OFFSET_ADDR,time.RTC_Mon);
        RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_AUTOMATIC_DAY_OFFSET_ADDR,time.RTC_Mday);
        RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_AUTOMATIC_HOUR_OFFSET_ADDR,time.RTC_Hour);
        RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_AUTOMATIC_MIN_OFFSET_ADDR,time.RTC_Min);
        
        //获取所有结果的状态标志,并且将相应的状态加入到相应的变量当中
        //djq add 20160809
        Final_State = ((CONTEXT_APP.CheckInfo.Barometer.Check_result[1] & 0xC0));
        CONTEXT_APP.CheckInfo.Result_Report.Automatic[Final_State>>6]++;

        Final_State = ((CONTEXT_APP.CheckInfo.GasO2.Check_result[1] & 0xC0));
        CONTEXT_APP.CheckInfo.Result_Report.Automatic[Final_State>>6]++;

        Final_State = ((CONTEXT_APP.CheckInfo.Flow_S.Check_result[1] & 0xC0));
        CONTEXT_APP.CheckInfo.Result_Report.Automatic[Final_State>>6]++;

        Final_State = ((CONTEXT_APP.CheckInfo.Pressure_S.Check_result[1] & 0xC0));
        CONTEXT_APP.CheckInfo.Result_Report.Automatic[Final_State>>6]++;

        //Final_State = ((CONTEXT_APP.CheckInfo.FG_Flow_S.Check_result[1] & 0xC0));
        //CONTEXT_APP.CheckInfo.Result_Report.Automatic[Final_State>>6]++;

        Final_State = ((CONTEXT_APP.CheckInfo.Compliance.Check_result[1] & 0xC0));
        CONTEXT_APP.CheckInfo.Result_Report.Automatic[Final_State>>6]++;

        Final_State = ((CONTEXT_APP.CheckInfo.Internal_Leakage.Check_result[1] & 0xC0));
        CONTEXT_APP.CheckInfo.Result_Report.Automatic[Final_State>>6]++;

        Final_State = ((CONTEXT_APP.CheckInfo.Oxygen.Check_result[1] & 0xC0));
        CONTEXT_APP.CheckInfo.Result_Report.Automatic[Final_State>>6]++;

        Final_State = ((CONTEXT_APP.CheckInfo.Battery.Check_result[1] & 0xC0));
        CONTEXT_APP.CheckInfo.Result_Report.Automatic[Final_State>>6]++;

        //Final_State = ((CONTEXT_APP.CheckInfo.PEEP_Valve.Check_result[1] & 0xC0));
        //CONTEXT_APP.CheckInfo.Result_Report.Automatic[Final_State>>6]++;
        //Final_State = ((CONTEXT_APP.CheckInfo.Safety_Valve.Check_result[1] & 0xC0));
        //CONTEXT_APP.CheckInfo.Result_Report.Automatic[Final_State>>6]++;
        //Final_State = ((CONTEXT_APP.CheckInfo.Pop_off_Valve.Check_result[1] & 0xC0));
        //CONTEXT_APP.CheckInfo.Result_Report.Automatic[Final_State>>6]++;
        //Final_State = ((CONTEXT_APP.CheckInfo.ACGO_Switch.Check_result[1] & 0xC0));
        //CONTEXT_APP.CheckInfo.Result_Report.Automatic[Final_State>>6]++;

		RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_AUTOMATIC_OFFSET_ADDR + (RESULT_SUCCESS>>2),CONTEXT_APP.CheckInfo.Result_Report.Automatic[(RESULT_SUCCESS>>2)]);
        RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_AUTOMATIC_OFFSET_ADDR + (RESULT_FAULT>>2),CONTEXT_APP.CheckInfo.Result_Report.Automatic[(RESULT_FAULT>>2)]);
        RTEEPROMwrite(MEM_OF_CHECKRESULT_ADDR + MEM_AUTOMATIC_OFFSET_ADDR + (RESULT_SKIP>>2),CONTEXT_APP.CheckInfo.Result_Report.Automatic[(RESULT_SKIP>>2)]);  

    }
}

