#include "HL7_Common.h"
#include "HL7_cfg.h"
#include "string.h"
#include "IAP.h"
#include "stdio.h"

char buf_sn[16] = {0};

void HL7_ReadSN(void)
{
    u32 SN = ReadDeviceSerial();
    sprintf(buf_sn, "%X", SN);
}

char* HL7_GetSN(void)
{
    return buf_sn;
}


void HL7_Para_clr(HL7_Para_t *para)
{
    para->ID = NULL;
    para->subID = NULL;
    para->name = NULL;
    para->value[0] = 0;
    para->unit = NULL;
    para->type = NULL;
    para->time = NULL; 
}

u32 HL7_FillBuf(HL7_MSG_t msg, u8 *dst, u32 dst_size, u8 **src, u32 src_size)
{
	volatile u32 size = 0;
    //volatile 
    u32 ofs = 0, len = 0, tmp_ofs = 0, i = 0;    
    //char buf[20] = {0};
    
	//计算总大小
	for(i=0; i<src_size; i++)
	{
		if(src[i] != NULL)
		{
			size += strlen((const char *)src[i]);
		}
	}

	if(size+src_size > dst_size)
	{
		return 0;
	}

	for(i=0; i<src_size; i++)
	{
		if(src[i] != NULL)
		{
			len = strlen((const char *)src[i]);
		}
		else
		{
			len = 0;
		}

		if(len != 0)
		{
			memcpy(&dst[ofs], src[i], len); 
			ofs += len;
		}
		dst[ofs] = HL7_ESC_FLD;
		ofs += 1;
	}

    //临时增加 发空包
    if(msg != HL7_MSG_PID)
    {
        tmp_ofs = ofs;
        for(i=tmp_ofs; i>0; i--)
        {
            if(dst[i-1] == HL7_ESC_FLD && 
               dst[i-2] == HL7_ESC_FLD)
            {
                dst[i-1] = 0;
                ofs -= 1;
            }
            else
            {
                break;
            }
        }
    }


    //是否去掉最后的'|' && 填充<CR>
    switch(msg)
    {
    case HL7_MSG_MSH:
    case HL7_MSG_PID:
    case HL7_MSG_OBR:
    case HL7_MSG_OBX_alrm:
    case HL7_MSG_OBX_msg:
        dst[ofs] = HL7_EOP_FLAG2;
        ofs += 1;        
        break;
    case HL7_MSG_OBX_monitor:
    case HL7_MSG_OBX_ctrl:
    case HL7_MSG_OBX_alrmlim:
        dst[ofs-1] = HL7_EOP_FLAG2;
        break;
    case HL7_MSG_MSA:
        break;
    default:
        break;
    }

	return ofs;
}


