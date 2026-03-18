#include "DispStrs_cfg.h"
#include "string.h"
#ifdef __ICCARM__
#include "includes.h"
#endif

#define DISP_STRS_SIZE  (2*1024*1024)
#define DISP_STRS_SADDR (0x80040000)

void GUI_DispStrs_Init(void)
{
#ifdef __ICCARM__
    __disable_interrupt();
    memcpy((unsigned int *)STR_BASE_ADDR, (unsigned int *)DISP_STRS_SADDR, DISP_STRS_SIZE);
    __enable_interrupt();
#endif
}


