#include "finish_app.h"
#include "GUI.h"
#include "WM.h"


//恢复保存的空间的参数
static void _Item_Restore(void)//djq modify 20170109
{
    FRAMEWIN_SetDefaultBarColor(1,GUI_YAN_BLUE);
    FRAMEWIN_SetDefaultBarColor(0,GUI_YAN_BLUE);
    FRAMEWIN_SetDefaultTitleHeight(35);
    FRAMEWIN_SetDefaultBorderSize(2);
}

void Check_Release(void)    //在全部跳过或者完成检测时调用
{
    U8 cnt = 0;
    Serial_Communaction_Handing(S_SEND_TO_BDU_FINISH);  ////向BDU发送结束标志，这里有可能出问题
                                                        ////在串口忙时，没有立刻发送出结束标志，BDU就永远不会接收到结束了。
    CHECK_DEBUG = 0;    //结束检测标志
    Context_SetControlFlag(Ctrl_Choose);//djq add 20161018
    //要还原进入自检程序前的各种状态，包括：字体，背景颜色，控件设置（对话框标题栏颜色。。。）
    _Item_Restore();
    //删除自检中创建的控件，以便下次自检再次创建，节省内存。
    WM_DeleteWindow(Dialog_TwoButton_Prompt_HANDLE);
	Dialog_TwoButton_Prompt_HANDLE = 0;
    WM_DeleteWindow(Dialog_OneButton_Prompt_HANDLE);
	Dialog_OneButton_Prompt_HANDLE = 0;
//    if(CONTEXT_APP.Callback_Return != TWO_BUTTON_NO_ID)//没有进入到自检程序当中,则没有下面的对话框
    {
        CONTEXT_APP.Callback_Return = 0xff;
        WM_DeleteWindow(CheckItem_Left_HANDLE);
        WM_DeleteWindow(CheckItem_Right_HANDLE);
        WM_DeleteWindow(Desktop_Button_HANDLE);
        WM_DeleteWindow(CheckItem_Total_HANDLE);
    }
    WM_SetCallback(WM_HBKWIN,DESKTOP_CALL_OLD);        //清除桌面回调函数
    GUI_Clear();
    //djq add 20170109
    //LCD_SetColor(0xFFFFFF);
    //Bevel(0, 50, 552, 390,8);
    //Bevel(0, 50, 637, 390,8);  //sun add 20140516

}
//#define S_SEND_TO_BDU_FINISH			(0x4d)//气路控制板结束标志
//#define	S_SEND_TO_EF_FINISH			(0x4e)//电子流量计版结束标志
//#define S_SEND_TO_UI_FINISH			(0x4f)//外设板结束标志

//release resource