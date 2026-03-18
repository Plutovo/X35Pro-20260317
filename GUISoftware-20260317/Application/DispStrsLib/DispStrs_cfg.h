/******************** display strings config ********************/

#ifndef _DispStrs_cfg_H
#define _DispStrs_cfg_H


#include "stdtypes.h"

#define  OPTIMIZE_CUSTOMIZE
#define  SYS_STRS_SIZE	1200 
#ifdef __ICCARM__
#define  STR_BASE_ADDR 0xA0800000
#define  STR_ALLOC_LOCATION POOL_AHBRAM
#else
#define  STR_ALLOC_LOCATION 
#endif


#endif