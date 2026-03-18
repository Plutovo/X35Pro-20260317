#ifndef WIDGET_TOOLS_H
#define WIDGET_TOOLS_H

#include "TracingCurve.h"

typedef struct
{
	WM_HWIN LungWin;
	WM_HWIN CPBInfoWin;
	TracingCurve_Handle Tobj[1];
} TOOLS_MENU_HWIN;



 
void initTools(unsigned char  Patient_Type);
void LungRecCountDown(void);
void LungRec_EndDeal(void);
int LungRec_GetTime(void);
int LungRec_GetPressure(void);

int Tools_GetMeasureFlag(void);
int Tools_GetTypeSelect(void);
int Tools_GetTime(void);

int CPB_GetFlag(void);
int LungRec_GetStartStopFlag(void);

void LungRec_CurveDataSave(TracingCurve_Handle hObj,char flag,float pressure);


#endif