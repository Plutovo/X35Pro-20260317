#ifndef _HL7_H
#define _HL7_H
#include "HL7_inc_ext.h"

typedef struct
{
    u8 SB[1];
    u8 EOP[2];
}HL7_COMM_t;
 
void HL7_Init(void);
void HL7_Send(void);

#endif


