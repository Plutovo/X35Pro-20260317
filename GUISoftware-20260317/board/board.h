#ifndef __BOARD_H_
#define __BOARD_H_

#include "chip.h"
#include "string.h"
#include "includes.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MOSC            (12MHZ)
#define RTCOSC          (32768UL)

#define EXP(dst, src, arg1, arg2)   ((dst == src)?arg1:arg2)
#define BOARDTYPE(arg1, arg2)       EXP(Board_GetMod(), (u32)Board_1_x, arg1, arg2)
//向上取整
#define CEIL(v, div) ((v+div-1)/div)

#define MB  (1024UL*1024UL)
#define KB  (1024UL)


#define SRAM_BASE_ADDR          0x10000000
#define AHBRAM_BASE_ADDR        0x20000000
#define SDRAM_BASE_ADDR		    0xA0000000
#define SDRAM_SIZE              0x01000000

/* MT48LC8M16A2 */
//TREF
#define SDRAM_MT48_REFRESH          EMC_NANOSECOND(64000000/4096)
//Precharge command period
//动态存储器预充电命令周期
#define SDRAM_MT48_TRP              EMC_NANOSECOND(18)
//Active to precharge command period
//动态存储器有效至预充电命令周期
#define SDRAM_MT48_TRAS             EMC_NANOSECOND(42)
//Last-data-out to active command time
//动态存储器最后数据输出至有效时间
#define SDRAM_MT48_TAPR             EMC_CLOCK(1)
//Data-in to Precharge Command
//tDPL + tRP 动态存储器数据输入至有效命令时间
#define SDRAM_MT48_TDAL             EMC_CLOCK(5)
//Write recovery time
//动态存储器写入恢复时间
#define SDRAM_MT48_TWR              EMC_CLOCK(2)
//Active to active command period
//动态存储器有效至有效命令周期
#define SDRAM_MT48_TRC              EMC_NANOSECOND(60)
//Auto-refresh period and auto-refresh to active command period
//动态存储器自动刷新周期
#define SDRAM_MT48_TRFC             EMC_NANOSECOND(60)
//Self-refresh exit time
//动态存储器自刷新退出时间
#define SDRAM_MT48_TSREX            EMC_CLOCK(1)
//Exit self-refresh to active command time
//动态存储器退出自刷新
#define SDRAM_MT48_TXSR             EMC_NANOSECOND(67)
//Active bank A to active bank B latency
//动态存储器有效组A至有效组B时间寄存器位
#define SDRAM_MT48_TRRD             EMC_NANOSECOND(12)
//Load mode register to active command time
//动态存储器装载模式寄存器至有效命令时间位
#define SDRAM_MT48_TMRD             EMC_CLOCK(2)
//ACTIVE-to-READ or WRITE delay(RAS)
#define SDRAM_MT48_TRCD             EMC_NANOSECOND(18)

/* HY57V281620F */
#define SDRAM_HY57_REFRESH          EMC_NANOSECOND(64000000/4096)
#define SDRAM_HY57_TRP              EMC_NANOSECOND(20)
#define SDRAM_HY57_TRAS             EMC_NANOSECOND(42)
#define SDRAM_HY57_TAPR             EMC_CLOCK(1)
#define SDRAM_HY57_TDAL             EMC_CLOCK(4)
#define SDRAM_HY57_TWR              EMC_CLOCK(2)
#define SDRAM_HY57_TRC              EMC_NANOSECOND(65)
#define SDRAM_HY57_TRFC             EMC_NANOSECOND(63)
#define SDRAM_HY57_TSREX            EMC_CLOCK(1)
#define SDRAM_HY57_TXSR             EMC_NANOSECOND(67)
#define SDRAM_HY57_TRRD             EMC_NANOSECOND(15)
#define SDRAM_HY57_TMRD             EMC_CLOCK(2)
#define SDRAM_HY57_TRCD             EMC_NANOSECOND(20)


/* IS42S81600F */
#define SDRAM_IS42_REFRESH          EMC_NANOSECOND(64000000/4096)
#define SDRAM_IS42_TRP              EMC_NANOSECOND(15)
#define SDRAM_IS42_TRAS             EMC_NANOSECOND(37)
#define SDRAM_IS42_TAPR             EMC_CLOCK(1)
#define SDRAM_IS42_TDAL             EMC_NANOSECOND(30)
#define SDRAM_IS42_TWR              EMC_CLOCK(2)
#define SDRAM_IS42_TRC              EMC_NANOSECOND(60)
#define SDRAM_IS42_TRFC             EMC_NANOSECOND(60)
#define SDRAM_IS42_TSREX            EMC_CLOCK(1)
#define SDRAM_IS42_TXSR             EMC_NANOSECOND(67)
#define SDRAM_IS42_TRRD             EMC_NANOSECOND(14)
#define SDRAM_IS42_TMRD             EMC_NANOSECOND(14)
#define SDRAM_IS42_TRCD             EMC_NANOSECOND(15)


/* S29GLXXXX */
/* tCS */
#define NORFLASH_S29_WAITWEN        EMC_NANOSECOND(0)
/* tCE-tOE*/
#define NORFLASH_S29_WAITOEN        EMC_NANOSECOND(0)
/* tCE*/
#define NORFLASH_S29_WAITRD         EMC_NANOSECOND(100)
/* disable page read */
#define NORFLASH_S29_WAITPG         EMC_NANOSECOND(0)
/* tDS */
#define NORFLASH_S29_WAITWR         EMC_NANOSECOND(30)
#define NORFLASH_S29_WAITTURN       EMC_CLOCK(8)

void Board_SystemInit(void);

#ifdef __cplusplus
}
#endif

#endif /* __BOARD_H_ */
