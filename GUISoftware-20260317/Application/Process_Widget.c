#include "Process_Widget.h"



#define ACTIVECOLOR_DEFAULT    GUI_WHITE
#define INACTIVECOLOR_DEFAULT  (0x575757)
#define BKCOLOR_DEFAULT        GUI_BLACK

#define INTERVAL_DEFAULT       10
#define SIZE_DEFAULT           8
#define NUMBER_DEFAULT         8
#define ROWNUMBER_DEFAULT      1
#define INDEX_DEFAULT          0 


/*********************************************************************
*
*       _Paint
*/

static void _Paint(ProcessWidget_Handle hObj, ProcessWidget_Obj* pObj)
{
    GUI_RECT r;
    int cnt = 0;
    WM_GetWindowRect(&r);

    GUI_SetColor(pObj->BkColor);
    LCD_FillRect(r.x0,r.y0,r.x1,r.y1);


    //pObj->Size = SIZE_DEFAULT;
    //pObj->Interval = INTERVAL_DEFAULT;
    for(cnt = 0;cnt < pObj->Number;cnt++)
    {
        if(cnt != pObj->Index)
        {
            GUI_SetColor(pObj->InactiveColor);
            GUI_FillCircle(cnt*(pObj->Size+pObj->Interval)+pObj->Size/2,pObj->Size/2,pObj->Size/2);
        }
        else
        {
            GUI_SetColor(pObj->ActiveColor);
            GUI_FillCircle(cnt*(pObj->Size+pObj->Interval)+pObj->Size/2,pObj->Size/2,pObj->Size/2);
        }
    }
}


static void _ProcessWidget_Callback (WM_MESSAGE*pMsg)
{
    ProcessWidget_Handle hObj = pMsg->hWin;
    ProcessWidget_Obj* pObj = ProcessWidget_H2P(hObj);
    if (WIDGET_HandleActive(hObj, pMsg) == 0)//用于处理窗体的默认事件，例如ID号获取等……
    {
        return;
    }

    /* Let widget handle the standard messages */
    switch (pMsg->MsgId)
    {
    case WM_PAINT:
        GUI_DEBUG_LOG("TEXT: _Callback(WM_PAINT)\n");
        _Paint(hObj, pObj);
        return;
    case WM_TOUCH:
        //_OnTouch(hObj, pObj, pMsg);
        return;
    case WM_KEY:
//        _OnKey(hObj, pMsg);
        return;
    case WM_DELETE:
        GUI_DEBUG_LOG("TEXT: _Callback(WM_DELETE)\n");
        //        _Delete(pObj);
        break;       /* No return here ... WM_DefaultProc needs to be called */
    }
    WM_DefaultProc(pMsg);
}




ProcessWidget_Handle ProcessWidget_CreateAsChild(
                                       int x0, int y0, int xsize, int ysize, WM_HWIN hParent,int Id,
                                       WM_CALLBACK* cb, int Flags)
{
    ProcessWidget_Handle hObj;
    int cnt;
    /* Create the window */
    WM_LOCK();
    hObj = WM_CreateWindowAsChild(x0, y0, xsize/*+2*HBorder*/, ysize/*+TBorder+BBorder*/, hParent,
        Flags, _ProcessWidget_Callback, sizeof(ProcessWidget_Obj) - sizeof(WM_Obj));
    if (hObj)
    {
        ProcessWidget_Obj* pObj = ProcessWidget_H2P(hObj);
        /* init widget specific variables */
        WIDGET__Init(&pObj->Widget,Id, WIDGET_STATE_FOCUSSABLE);


        pObj->Size = SIZE_DEFAULT;
        pObj->Interval = INTERVAL_DEFAULT;
        pObj->Number = NUMBER_DEFAULT;
        pObj->RowNum = ROWNUMBER_DEFAULT;
        pObj->Index = INDEX_DEFAULT;
        pObj->Style = Round;

        pObj->ActiveColor = ACTIVECOLOR_DEFAULT;
        pObj->InactiveColor = INACTIVECOLOR_DEFAULT;
        pObj->BkColor = BKCOLOR_DEFAULT;


    }
    WM_UNLOCK();
    return hObj;
}

ProcessWidget_Handle ProcessWidget_CreateIndirect(const GUI_WIDGET_CREATE_INFO* pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK* cb)
{
    ProcessWidget_Handle  hThis;
    //    int NumItems = (pCreateInfo->Para)       & 0xFF;
    //    GUI_USE_PARA(cb);
    hThis = ProcessWidget_CreateAsChild(pCreateInfo->x0 + x0, pCreateInfo->y0 + y0, pCreateInfo->xSize, pCreateInfo->ySize,
        hWinParent,pCreateInfo->Id,cb,pCreateInfo->Flags);
    return hThis;
}





void ProcessWidget_SetBkColor(ProcessWidget_Handle hObj,GUI_COLOR BkColor)
{
    if (hObj)
    {
        ProcessWidget_Obj* pObj;
        WM_LOCK();
        pObj = ProcessWidget_H2P(hObj);
        pObj->BkColor = BkColor;
        ProcessWidget_Invalidate(hObj);
        WM_UNLOCK();
    }
}

void ProcessWidget_SetActiveColor(ProcessWidget_Handle hObj,GUI_COLOR ActiveColor)
{
    if (hObj)
    {
        ProcessWidget_Obj* pObj;
        WM_LOCK();
        pObj = ProcessWidget_H2P(hObj);
        pObj->ActiveColor = ActiveColor;
        WM_UNLOCK();
        ProcessWidget_Invalidate(hObj);
    }
}

void ProcessWidget_SetInactiveColor(ProcessWidget_Handle hObj,GUI_COLOR InactiveColor)
{
    if (hObj)
    {
        ProcessWidget_Obj* pObj;
        WM_LOCK();
        pObj = ProcessWidget_H2P(hObj);
        pObj->InactiveColor = InactiveColor;
        ProcessWidget_Invalidate(hObj);
        WM_UNLOCK();
    }
}

int  ProcessWidget_SetIndexNumber(ProcessWidget_Handle hObj,int Index)
{
    int OldIndex = -1;
    if (hObj)
    {
        ProcessWidget_Obj* pObj;
        WM_LOCK();
        pObj = ProcessWidget_H2P(hObj);
        OldIndex = pObj->Index;
        pObj->Index = Index;
        ProcessWidget_Invalidate(hObj);
        WM_UNLOCK();
    }
    return OldIndex;
}