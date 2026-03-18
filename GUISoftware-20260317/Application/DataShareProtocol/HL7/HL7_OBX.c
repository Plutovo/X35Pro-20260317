#include "HL7_OBX.h"
#include "HL7_cfg.h"
#include "HL7_Common.h"
#include "HL7_OBX_Para.h"
#include "HL7_OBX_CtrlPara.h"
#include "HL7_OBX_AlrmLim.h"
#include "HL7_OBX_Alrm.h"
#include "HL7_OBX_Msg.h"

void HL7_OBX_Send(HL7_OBX_chse_t chse)
{
    switch(chse)
    {
    case HL7_OBX_monitor:   //监测参数
        HL7_Para_Send();
        break;
    case HL7_OBX_ctrl:  //控制参数
        HL7_CtrlPara_Send();
        break;
    case HL7_OBX_alrmlim:   //报警限制
        HL7_AlrmLim_Send();
        break;
    case HL7_OBX_alrm:  //报警
        HL7_Alrm_Send();
        break;
    case HL7_OBX_Mesg:
        HL7_Msg_Send();
        break;
    }
}
