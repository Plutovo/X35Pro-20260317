#ifndef _HL7_COMMON_H
#define _HL7_COMMON_H
#include "stdtypes.h"

typedef enum
{   
    HL7_MSG_MSH = 0,
    HL7_MSG_PID,
    HL7_MSG_OBR,
    HL7_MSG_OBX_monitor,
    HL7_MSG_OBX_ctrl,
    HL7_MSG_OBX_alrmlim,
    HL7_MSG_OBX_alrm,
    HL7_MSG_OBX_msg,
    HL7_MSG_MSA,
}HL7_MSG_t;

typedef struct
{
    char *ID;       //para ID
    char *subID;    //
    char *name;     //para name
    char *unit;     //para unit
    char *type;     //para type
    char *time;     //para time
    char value[16];   //para value
}HL7_Para_t;

char* HL7_GetSN(void);
void HL7_ReadSN(void);
void HL7_Para_clr(HL7_Para_t *para);
u32 HL7_FillBuf(HL7_MSG_t msg, u8 *dst, u32 dst_size, u8 **src, u32 src_size);

#endif
