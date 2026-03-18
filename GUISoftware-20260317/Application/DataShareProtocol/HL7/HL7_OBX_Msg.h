#ifndef _HL7_OBX_MSG_H
#define _HL7_OBX_MSG_H
#include "stdtypes.h"
#include "DataShare.h"
#include "HL7_Common.h"

void HL7_Msg_Send(void);
void HL7_Read_Mode(DS_Mode_t mode, HL7_Para_t *para);

#endif
