#ifndef _HL7_MAPTBLE_H
#define _HL7_MAPTBLE_H
#include "HL7_cfg.h"
#include "DataShare.h"

char HL7_map_read_paraid(u32 idx);
char HL7_map_read_ctrlparaid(u32 idx);
char HL7_map_read_alrmlimid(u32 idx);
u8 *HL7_map_read_alrmid_grp0(u32 idx);
u8 *HL7_map_read_alrmid_grp1(u32 idx);
u8 *HL7_map_read_mode(DS_Mode_t idx);

#endif
