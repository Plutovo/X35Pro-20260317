/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              礐/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : BUTTON_Private.h
Purpose     : BUTTON private header file
--------------------END-OF-HEADER-------------------------------------
*/
#ifndef BUTTON_PRIVATE_H
#define BUTTON_PRIVATE_H
#include "WM.h"
#include "BUTTON.h"
#if GUI_WINSUPPORT
#define BUTTON_ID 0x42555454
#define BUTTON_H2P(h) (BUTTON_Obj*) GUI_ALLOC_h2p(h)
#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
#define BUTTON_ASSERT_IS_VALID_PTR(p) GUI_DEBUG_ERROROUT_IF(p->DebugId != BUTTON_ID, "BUTTON.C: Wrong handle type or object not init'ed")
#define BUTTON_INIT_ID(p)   p->DebugId = BUTTON_ID
#define BUTTON_DEINIT_ID(p) p->DebugId = BUTTON_ID+1
#else
#define BUTTON_ASSERT_IS_VALID_PTR(p)
#define BUTTON_INIT_ID(p)
#define BUTTON_DEINIT_ID(p)
#endif
typedef struct
{
    GUI_COLOR aBkColor[4];
    GUI_COLOR aTextColor[3];
    const GUI_FONT GUI_UNI_PTR * pFont;
	const GUI_FONT GUI_UNI_PTR * pFont_Minor;//可选文字字体
    I16 Align;
    GUI_WRAPMODE WrapMode;//add 20161115 是否自动换行
	int PrevPressed_ID;//ylk add 20181009
} BUTTON_PROPS;
typedef struct
{
    WIDGET Widget;
    BUTTON_PROPS Props;
    WM_HMEM hpText;
    int Button_Use_3D;
    WM_HMEM ahDrawObj[3];

	WM_HMEM minorText;//可选文字字体
	unsigned char minor_x,minor_y;

#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
    int DebugId;
#endif
    unsigned char ReleasedFlag;//能够点击弹起标志
} BUTTON_Obj;
extern BUTTON_PROPS BUTTON__DefaultProps;
void BUTTON__SetDrawObj(BUTTON_Handle hObj, int Index, GUI_DRAW_HANDLE hDrawObj);
#endif   /* GUI_WINSUPPORT */
#endif   /* BUTTON_H */
