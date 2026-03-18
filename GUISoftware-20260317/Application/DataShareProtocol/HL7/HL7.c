#include "string.h"
#include "HL7.h"
#include "HL7_cfg.h"
#include "HL7_maptble.h"
#include "HL7_MSH.h"
#include "HL7_PID.h"
#include "HL7_OBR.h"
#include "HL7_OBX.h"
#include "HL7_OBX_Para.h"
#include "HL7_OBX_CtrlPara.h"
#include "HL7_OBX_AlrmLim.h"
//ext
#include "DataShare_cfg.h"
#include "uart_dp.h"
#include "DataShare.h"

static u8 send_buf[HL7_FRM_MAX_SIZE] POOL_EXRAM;
static HL7_COMM_t HL7_COMM POOL_EXRAM;

void HL7_MSG_Send(HL7_MSGID_t id, HL7_OBX_chse_t chse, u8 *sendbuf, u32 size);

void HL7_Init(void)
{
    exuart1_init(125000, EXUART_LCR_PARITY_DIS);
    uart_dp_clrrb();
    memset(send_buf,    0,  HL7_FRM_MAX_SIZE);
    memset(&HL7_COMM,   0,  sizeof(HL7_COMM_t));
    HL7_COMM.SB[0] = HL7_SYNC_FLAG;
    HL7_COMM.EOP[0] = HL7_EOP_FLAG1;
    HL7_COMM.EOP[1] = HL7_EOP_FLAG2;
    
    HL7_MSH_Init();
    HL7_Para_Init();
    HL7_ReadSN();
}

void HL7_Send(void)
{
    static u8 para_cnt = 0, ctrlpara_cnt = 0;
    u32 size = 0;
    //时基: 100ms, 时间:5S
    if(para_cnt > 50)
    {
        para_cnt = 0;
#ifdef _HL7_PARA     
        if(ds_readpara_StdyState() == 0x00)
        {
            /* 监测参数发送 */
            HL7_MSG_Send(HL7_MSGID_Para, HL7_OBX_monitor, send_buf, size);
        }
#endif
    }
    else
    {
        para_cnt++;
    }
    
    if(ctrlpara_cnt > 30)
    {
        ctrlpara_cnt = 0;
        if(HL7_CtrlPara_Change() == true)
        {
#ifdef _HL7_CTRLPARA            
            //控制参数发送
            HL7_MSG_Send(HL7_MSGID_CtrlPara, HL7_OBX_ctrl, send_buf, size);
#endif
        }

#ifdef _HL7_ALRMLIM
        if(HL7_AlrmLim_Change() == true)
        {
            //报警限制
            HL7_MSG_Send(HL7_MSGID_AlrmLim, HL7_OBX_alrmlim, send_buf, size);        
        }
#endif          
        
#ifdef _HL7_ALRM        
        //存在报警
        if(ds_alrmisexist() == true)
        {
            //报警
            HL7_MSG_Send(HL7_MSGID_Alrm, HL7_OBX_alrm, send_buf, size);   
        }
#endif   
        
#ifdef _HL7_MESG  
        //文本信息
        HL7_MSG_Send(HL7_MSGID_Mesg, HL7_OBX_Mesg, send_buf, size);
#endif
    }
    else
    {
        ctrlpara_cnt++;
    }
}

void HL7_MSG_Send(HL7_MSGID_t id, HL7_OBX_chse_t chse, u8 *sendbuf, u32 size)
{
    //文本信息
    //<SB>
    uart_dp_wrsend(HL7_COMM.SB, 1);
    //MSH
    size = HL7_MSH_Fill(id, send_buf);
    uart_dp_wrsend(send_buf, size);
    //PID
    size = HL7_PID_Fill(send_buf);
    uart_dp_wrsend(send_buf, size);        
    //OBR
    size = HL7_OBR_Fill(send_buf);
    uart_dp_wrsend(send_buf, size);  
    //OBX
    HL7_OBX_Send(chse);
    //<EB><CR>
    uart_dp_wrsend(HL7_COMM.EOP, 2);     
}




