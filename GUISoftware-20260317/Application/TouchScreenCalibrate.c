#include "TouchScreenCalibrate.h"
//用于获取校验数据的点
static coordinate DisplaySample[CALIBRATEPOINT_NUM] =
{
    {50,  50},
    {750, 50},
    {50,  430},
    {750, 430},
};
//用于验证校验数据的点
static coordinate StandPoint[STAND_POINT] = 
{ 
    {20,  240},
    {780, 240},
    {400, 20 },
    {400, 460},
    {400, 240},
};

static float Logic2Phy_X = 0;
static float Logic2Phy_Y = 0; 
int left_num;        
static GUI_PID_STATE CalInfo[CALIBRATEPOINT_NUM];//calibration infomation
/****************************************************************/
//显示提示信息
static void _DispPromptString(const char *string)
{
	int x = 55,y = 65,sizex=400,sizey=30;
	//GUI_SetColor(SCR_BKCOLOR);
 //   LCD_FillRect(x,y,x+sizex, y+sizey);
    GUI_SetTextMode(GUI_TEXTMODE_TRANS);
    GUI_SetColor(SCR_COLOR);
#ifdef LANGUAGE_CHINESE_FLAG
    GUI_SetFont(&GUI_Font_16);
#else
    GUI_SetFont(&GUI_Font_24);
#endif
	GUI_DispStringAt(string,x,y);
// 	GUI_DispStringAt("Please touch the crosshair to calibrate:",55,65);
	GUI_Delay(1);
}
/****************************************************************/

static void ClearScreen(WM_HWIN hWin,unsigned char type)
{
  unsigned char str_tmp[180];
    WM_SelectWindow(hWin);
    GUI_SetColor(SCR_BKCOLOR);
    LCD_FillRect(0,0,LCD_XSIZE - 1, LCD_YSIZE - 1);
	switch(type)
	{
		case 0:
			_DispPromptString(Sys_Lib_GetDispStr(DP_TOUCHCROSSHAIRCAL));
			break;
		case 1:
// 			sprintf(str_tmp,"%s%d%s",System_Display[ STR_touch ],left_num,System_Display[ STR_Invalidtouch ]);
			sprintf(str_tmp,"%s%d", Sys_Lib_GetDispStr(DP_CLICKCROSS), left_num);
			_DispPromptString(str_tmp);
  			break;
		case 2:   //zkq add 2061226 清除屏幕  防止字体残留
            LCD_SetColor(0xFFFFFF);
            Bevel(0, 50+20, 637, 390,8);
            break;
		default:
			break;		
	}
}
/*******************************************************************************
* Function Name  : DrawCross
* Description    : 在指定座标画十字准星
* Input          : - Xpos: Row Coordinate
*                  - Ypos: Line Coordinate
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
static void DrawCross(WM_HWIN hWin,U16 Xpos,U16 Ypos,GUI_COLOR color)
{
    WM_SelectWindow(hWin);
    GUI_SetColor(color);
    LCD_DrawHLine(Xpos-15,Ypos,Xpos-2);
    LCD_DrawHLine(Xpos+2,Ypos,Xpos+15);
    LCD_DrawVLine(Xpos,Ypos-15,Ypos-2);
    LCD_DrawVLine(Xpos,Ypos+2,Ypos+15);
}
/*******************************************************************************
* Function Name  : _Convet_Coordinate2Ref
* Description    : 根据校准信息计算参考值
* Input          : ref. point coordinate
* Output         : None
* Return         : 0 success   -1 fail 
* Attention      : result save to GUI_TOUCH_Calxxxx -> GUITouchConf.h 
*******************************************************************************/
//K为实际坐标转换为逻辑坐标的系数
//XL、YL为某点的逻辑坐标  X、Y为某点的物理坐标
//通过4个点的坐标求出K
//KX={(XLB-XLA)/(XB-XA)+(XLD-XLD)/(XD-XC)}/2
//KY={(YLA-YLC)/(YA-YC)+(YLB-YLD)/(YB-YD)}/2
//使用K的值和第5点的物理坐标值，推出第5点的逻辑坐标值，然后和用户按下的第五点坐标比较，如果不再范围内则重新校验
int _Conver_Coordinate2Ref(GUI_PID_STATE ts_state[CALIBRATEPOINT_NUM])
{
	//swapxy  no,mirror_x no,mirror_y no
    Logic2Phy_X = ((ts_state[1].x - ts_state[0].x)/(float)(DisplaySample[1].x - DisplaySample[0].x) +
		           (ts_state[3].x - ts_state[2].x)/(float)(DisplaySample[3].x - DisplaySample[2].x))/2;

    Logic2Phy_Y = ((ts_state[3].y - ts_state[0].y)/(float)(DisplaySample[3].y - DisplaySample[0].y) +
		           (ts_state[2].y - ts_state[1].y)/(float)(DisplaySample[2].y - DisplaySample[1].y))/2; 
    GUI_TOUCH_CalLeft   = ts_state[0].x - DisplaySample[0].x*Logic2Phy_X;
	GUI_TOUCH_CalRight  = ts_state[0].x + (LCD_XSIZE - DisplaySample[0].x - 1)*Logic2Phy_X;
	GUI_TOUCH_CalTop    = ts_state[0].y - DisplaySample[0].y*Logic2Phy_Y;
	GUI_TOUCH_CalBottom = ts_state[0].y + (LCD_YSIZE - DisplaySample[0].y - 1)*Logic2Phy_Y;
	return 0;
}

/*******************************************************************************
* Function Name  : TouchAdValue_Save
* Description    : 向EEPROM中存储校验数据
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void TouchAdValue_Write(void)
{
    unsigned int CheckSum;
    
    CheckSum = GUI_TOUCH_CalLeft + GUI_TOUCH_CalRight + GUI_TOUCH_CalTop + GUI_TOUCH_CalBottom;
    
    RTEEPROMwrite(TOUCH_AD_LEFT,(GUI_TOUCH_CalLeft>>8)&0xFF);
    RTEEPROMwrite(TOUCH_AD_LEFT+1,GUI_TOUCH_CalLeft&0xFF);

    RTEEPROMwrite(TOUCH_AD_RIGHT,(GUI_TOUCH_CalRight>>8)&0xFF);
    RTEEPROMwrite(TOUCH_AD_RIGHT+1,GUI_TOUCH_CalRight&0xFF);

    RTEEPROMwrite(TOUCH_AD_TOP,(GUI_TOUCH_CalTop>>8)&0xFF);
    RTEEPROMwrite(TOUCH_AD_TOP+1,GUI_TOUCH_CalTop&0xFF);

    RTEEPROMwrite(TOUCH_AD_BOTTOM,(GUI_TOUCH_CalBottom>>8)&0xFF);
    RTEEPROMwrite(TOUCH_AD_BOTTOM+1,GUI_TOUCH_CalBottom&0xFF);

    RTEEPROMwrite(TOUCH_CHECKSUM,(CheckSum>>8)&0xFF);
    RTEEPROMwrite(TOUCH_CHECKSUM+1,CheckSum&0xFF);   
}
/*******************************************************************************
* Function Name  : TouchAdValue_Read
* Description    : 从EEPROM中读取校验数据
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
//#define ErrorLimit 200
void TouchAdValue_Read(int flag)
{
    unsigned int CheckSum;
    //add zkq 20161103
    CheckSum = (RTEEPROMread(TOUCH_CHECKSUM)<<8) | RTEEPROMread(TOUCH_CHECKSUM+1);
    
    if(CheckSum)
    {
        GUI_TOUCH_CalLeft =   (RTEEPROMread(TOUCH_AD_LEFT)<<8)   | RTEEPROMread(TOUCH_AD_LEFT+1);
        GUI_TOUCH_CalRight =  (RTEEPROMread(TOUCH_AD_RIGHT)<<8)  | RTEEPROMread(TOUCH_AD_RIGHT+1);
        GUI_TOUCH_CalTop =    (RTEEPROMread(TOUCH_AD_TOP)<<8)    | RTEEPROMread(TOUCH_AD_TOP+1);
        GUI_TOUCH_CalBottom = (RTEEPROMread(TOUCH_AD_BOTTOM)<<8) | RTEEPROMread(TOUCH_AD_BOTTOM+1);
    }
    else
    {
      GUI_TOUCH_CalLeft     = 25;
      GUI_TOUCH_CalRight    = 990;
      GUI_TOUCH_CalTop      = 75;
      GUI_TOUCH_CalBottom   = 1000;
      
      CheckSum = GUI_TOUCH_CalLeft + GUI_TOUCH_CalRight + GUI_TOUCH_CalTop + GUI_TOUCH_CalBottom;
      
      RTEEPROMwrite(TOUCH_AD_LEFT,(GUI_TOUCH_CalLeft>>8)&0xFF);
      RTEEPROMwrite(TOUCH_AD_LEFT+1,GUI_TOUCH_CalLeft&0xFF);

      RTEEPROMwrite(TOUCH_AD_RIGHT,(GUI_TOUCH_CalRight>>8)&0xFF);
      RTEEPROMwrite(TOUCH_AD_RIGHT+1,GUI_TOUCH_CalRight&0xFF);

      RTEEPROMwrite(TOUCH_AD_TOP,(GUI_TOUCH_CalTop>>8)&0xFF);
      RTEEPROMwrite(TOUCH_AD_TOP+1,GUI_TOUCH_CalTop&0xFF);

      RTEEPROMwrite(TOUCH_AD_BOTTOM,(GUI_TOUCH_CalBottom>>8)&0xFF);
      RTEEPROMwrite(TOUCH_AD_BOTTOM+1,GUI_TOUCH_CalBottom&0xFF);
        
      RTEEPROMwrite(TOUCH_CHECKSUM,(CheckSum>>8)&0xFF);
      RTEEPROMwrite(TOUCH_CHECKSUM+1,CheckSum&0xFF);
    }

}

#define CALI_ErrorLimit (10)
int CalibrationReselt(GUI_PID_STATE *pState,int index)
{
    int ret = 0;
    if(pState->Pressed == 1)
    {
        if((pState->x <= StandPoint[index].x+CALI_ErrorLimit)&&(pState->x >= StandPoint[index].x - CALI_ErrorLimit)
         &&(pState->y <= StandPoint[index].y+CALI_ErrorLimit)&&(pState->y >= StandPoint[index].y - CALI_ErrorLimit))
        {
            ret = 1;
        }
    }
    else
    {
        ret = 0;
    }
    return ret;
}

/*******************************************************************************
* Function Name  : TouchPanel_Calibrate
* Description    : 校准触摸屏
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
*******************************************************************************/
void TouchPanel_Calibrate(void)
{
    volatile char i = 0,index = 0,StandErrIndex = 0;
    int flg = 0;
    WM_HWIN hWin;
    hWin = WM_CreateWindow(0,0,LCD_XSIZE,LCD_YSIZE,WM_CF_SHOW,0,0);
    ClearScreen(hWin,0);
    left_num = 3;
    for(i=0; i<CALIBRATEPOINT_NUM; i++)
    {
        GUI_Delay(200);
        DrawCross(hWin,DisplaySample[i].x,DisplaySample[i].y,SCR_COLOR);
        flg = 0;
        GUI_TOUCH_StoreState(-1,-1);
        //__disable_interrupt();
        while( !flg )
        {
            
            if(GUI_TOUCH_GetState(&CalInfo[i]))
            {
                
                if((CalInfo[i].x!=-1)&&(CalInfo[i].y!=-1)&&(CalInfo[i].Pressed))
                {
					if((i == CALIBRATEPOINT_NUM -1))
					{
						if(_Conver_Coordinate2Ref(CalInfo) != -1)
						{
            	            GUI_TOUCH_StoreState(-1,-1);  
                            TouchScreenCalibrate = 0;
							GUI_TOUCH_SetDefaultCalibration();
                            DrawCross(hWin,DisplaySample[i].x,DisplaySample[i].y,SCR_BKCOLOR);
							//ClearScreen(hWin,1);//del zkq 20170116 防止屏幕闪烁两次
							for(index = 0;index < STAND_POINT;index++)
							{						
								    left_num = 3;
									ClearScreen(hWin,1);
									for(StandErrIndex = 0;StandErrIndex < STAND_POINT_ERR_NUM;)
									{
										DrawCross(hWin,StandPoint[index].x,StandPoint[index].y,SCR_COLOR);										
										GUI_TOUCH_GetState(&CalInfo[0]);
										if(CalibrationReselt(&CalInfo[0],index))
										{
											if(index == STAND_POINT - 1)
											{
												flg = 1;
												i = CALIBRATEPOINT_NUM;  
												TouchAdValue_Write();
											}
											GUI_TOUCH_StoreState(-1,-1);
											break;
										}
										else
										if(CalInfo[0].Pressed)
										{ 
											if(--left_num == 0)
											{
												StandErrIndex++;
												//ClearScreen(hWin,0);
												//GUI_Delay_1(200);
											}
											else
											{ 
												GUI_Delay_1(100); 
												ClearScreen(hWin,1);	
												//GUI_Delay_1(200);
												//index--;
											}

										}
										
									}
									DrawCross(hWin,StandPoint[index].x,StandPoint[index].y,SCR_BKCOLOR);
									GUI_Delay(1);
									if(StandErrIndex >= STAND_POINT_ERR_NUM)
									{
									 break;
									}
							}
							if((StandErrIndex >= STAND_POINT_ERR_NUM)&&(!flg))
							{
                                flg = 1;
                                i = -1;
                                ClearScreen(hWin,0);
                                TouchScreenCalibrate = 1;													
							}
						}
						else
						{
						    flg = 0;
						}
					}
					else //i != CALIBRATEPOINT_NUM -1
					{
                        flg = 1;
					}
                }
            }
            
        }
       // __enable_interrupt();
        DrawCross(hWin,DisplaySample[i].x,DisplaySample[i].y,SCR_BKCOLOR);
    }

    ClearScreen(hWin,2);//zkq add 20170116 最后清除触摸校验屏幕显示
    WM_DeleteWindow(hWin);
    TouchScreenCalibrate = 0;
}