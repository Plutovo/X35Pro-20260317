#ifndef _OS_PRIO_H
#define _OS_PRIO_H
#include "stdio.h"
#include "stdtypes.h"
//typedef  unsigned int	u32;
//typedef  unsigned short u16;
//typedef  unsigned char  u8;

#ifdef __ICCARM__
#define AHB_RAM @ "AHBRAM"
#else
#define AHB_RAM
#endif

#define CPU_DATA		u32
#define CPU_SR			u32
#define OS_PRIO			u16

#define PRIO_INDEX(prio)    (prio/DEF_INT_CPU_NBR_BITS)
#define PRIO_BITS(prio)     (DEF_INT_CPU_NBR_BITS - 1 - prio%DEF_INT_CPU_NBR_BITS)

#define  DEF_INT_CPU_NBR_BITS       32
#define  OS_PRIO_TBL_SIZE(prio_size)           ((prio_size - 1u) / (DEF_INT_CPU_NBR_BITS) + 1u)

OS_PRIO		OS_PrioGetHighest (u32* OSPrioTbl);
OS_PRIO     OS_PrioGetHighest_Trail (u32* OSPrioTbl);
void		OS_PrioInsert (u32* OSPrioTbl, OS_PRIO  prio);
void        OS_PrioInsert_Trail (u32* OSPrioTbl, OS_PRIO  prio);
void		OS_PrioRemove (u32* OSPrioTbl, OS_PRIO  prio);
void        OS_PrioRemove_Trail (u32* OSPrioTbl, OS_PRIO  prio);
CPU_DATA    OS_PrioLookup (u32* OSPrioTbl, OS_PRIO  prio);
CPU_DATA	CPU_CntLeadZeros(CPU_DATA val);
CPU_DATA  CPU_CntTrailZeros (CPU_DATA  val);
#ifndef   __ICCARM__
CPU_DATA	CPU_CntLeadZeros32(CPU_DATA val);
#else
void		CPU_IntDis(void);
void		CPU_IntEn(void);
void		CPU_WaitForInt(void);
void		CPU_WaitForExcept(void);
CPU_DATA	CPU_RevBits(CPU_DATA val);
CPU_SR		CPU_SR_Save(void);
void		CPU_SR_Restore(CPU_SR cpu_sr);
CPU_DATA	CPU_CntTrailZeros(CPU_DATA val);
#endif
#endif

