#ifndef _STDTYPES_H
#define _STDTYPES_H

#ifdef __ICCARM__

#include "lpc_types.h"
#define POOL_HEAP    @ "gui_heap_pool"
#define POOL_BLOCK   @ "gui_block_pool"
#define POOL_TREND   @ "trend_pool"
#define POOL_MEM     @ "mem_pool"
#define POOL_RES     @ "res_pool"
#define POOL_STRS    @ "strs_pool"
#define POOL_PICS    @ "pics_pool"
#define POOL_FONT    @ "font_pool"
#define POOL_EXRAM   @ "exram_pool"
#define POOL_AHBRAM  @ "AHBRAM"
#else

#define POOL_HEAP
#define POOL_BLOCK 
#define POOL_TREND
#define POOL_MEM
#define POOL_RES
#define POOL_STRS
#define POOL_PICS
#define POOL_FONT
#define POOL_EXRAM
#define POOL_AHBRAM

#define s8  char
#define s16 short
#define s32 int
#define s64 long long

#define u8  unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long

typedef enum {RESET = 0, SET = !RESET} FlagStatus, SetState;

#define error RESET
#define success SET

#endif

typedef struct
{
    u8 RTC_Sec;      /* Secend value - [0,59]*/
    u8 RTC_Min;     /* Minute value - [0,59] */
    u8 RTC_Hour;    /* Hour value - [0,23] */
    u8 RTC_Mday;    /* Day of the month value - [1,31] */
    u8 RTC_Mon;     /* Month value - [1,12] */
    u16  RTC_Year;    /* Year value - [0,4095] */
} RTC_TIMER;

#endif