#include "os_prio.h"

/*
************************************************************************************************************************
*                                           GET HIGHEST PRIORITY TASK WAITING
*
* Description: This function is called by other services to determine the highest priority task
*              waiting on the event.
*
* Arguments  : none
*
* Returns    : The priority of the Highest Priority Task (HPT) waiting for the event
*
* Note(s)    : The OS Idle priority must exist
************************************************************************************************************************
*/
OS_PRIO  OS_PrioGetHighest (u32* OSPrioTbl)
{
    CPU_DATA  *p_tbl;
    OS_PRIO    prio;

    prio  = (OS_PRIO)0;
    p_tbl = &OSPrioTbl[0];
    while (*p_tbl == (CPU_DATA)0) 
    {
		prio += DEF_INT_CPU_NBR_BITS;
		p_tbl++;
    }

    prio += (OS_PRIO)CPU_CntLeadZeros(*p_tbl);
    return (prio);
}

//????
OS_PRIO  OS_PrioGetHighest_Trail (u32* OSPrioTbl)
{
    CPU_DATA  *p_tbl;
    OS_PRIO    prio;

    prio  = (OS_PRIO)0;
    p_tbl = &OSPrioTbl[0];
    while (*p_tbl == (CPU_DATA)0) 
    {
		prio += DEF_INT_CPU_NBR_BITS;
		p_tbl++;
    }

    prio += (OS_PRIO)CPU_CntTrailZeros(*p_tbl);
    return (prio);
}


void  OS_PrioInsert (u32* OSPrioTbl, OS_PRIO  prio)
{
    CPU_DATA  bit;
    CPU_DATA  bit_nbr;
    OS_PRIO   ix;

    ix             = prio / DEF_INT_CPU_NBR_BITS;
    bit_nbr        = (CPU_DATA)prio & (DEF_INT_CPU_NBR_BITS - 1u);
    bit            = 1u;
    bit          <<= (DEF_INT_CPU_NBR_BITS - 1u) - bit_nbr;
    OSPrioTbl[ix] |= bit;
}

void  OS_PrioInsert_Trail (u32* OSPrioTbl, OS_PRIO  prio)
{
    CPU_DATA  bit;
    CPU_DATA  bit_nbr;
    OS_PRIO   ix;

    ix             = prio / DEF_INT_CPU_NBR_BITS;
    bit_nbr        = (CPU_DATA)prio & (DEF_INT_CPU_NBR_BITS - 1u);
    bit            = 1u;
    bit          <<= bit_nbr;
    OSPrioTbl[ix] |= bit;
}


void  OS_PrioRemove (u32* OSPrioTbl, OS_PRIO  prio)
{
    CPU_DATA  bit;
    CPU_DATA  bit_nbr;
    OS_PRIO   ix;

    ix             = prio / DEF_INT_CPU_NBR_BITS;
    bit_nbr        = (CPU_DATA)prio & (DEF_INT_CPU_NBR_BITS - 1u);
    bit            = 1u;
    bit          <<= (DEF_INT_CPU_NBR_BITS - 1u) - bit_nbr;
    OSPrioTbl[ix] &= ~bit;
}

void  OS_PrioRemove_Trail (u32* OSPrioTbl, OS_PRIO  prio)
{
    CPU_DATA  bit;
    CPU_DATA  bit_nbr;
    OS_PRIO   ix;

    ix             = prio / DEF_INT_CPU_NBR_BITS;
    bit_nbr        = (CPU_DATA)prio & (DEF_INT_CPU_NBR_BITS - 1u);
    bit            = 1u;
    bit          <<= bit_nbr;
    OSPrioTbl[ix] &= ~bit;
}

CPU_DATA OS_PrioLookup(u32* OSPrioTbl, OS_PRIO prio)
{
    CPU_DATA bit;
    OS_PRIO   ix;
    
    ix  = PRIO_INDEX(prio);
    bit = PRIO_BITS(prio);
    
    if(((OSPrioTbl[ix]&(1<<bit))>>bit) == 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

#ifndef  __ICCARM__	//VS use,IAR use cpu_a.asm

/*
*********************************************************************************************************
*                                  CPU COUNT LEAD ZEROs LOOKUP TABLE
*
* Note(s) : (1) Index into bit pattern table determines the number of leading zeros in an 8-bit value :
*
*                         b07  b06  b05  b04  b03  b02  b01  b00    # Leading Zeros
*                         ---  ---  ---  ---  ---  ---  ---  ---    ---------------
*                          1    x    x    x    x    x    x    x            0
*                          0    1    x    x    x    x    x    x            1
*                          0    0    1    x    x    x    x    x            2
*                          0    0    0    1    x    x    x    x            3
*                          0    0    0    0    1    x    x    x            4
*                          0    0    0    0    0    1    x    x            5
*                          0    0    0    0    0    0    1    x            6
*                          0    0    0    0    0    0    0    1            7
*                          0    0    0    0    0    0    0    0            8
*********************************************************************************************************
*/
static  const  unsigned char  CPU_CntLeadZerosTbl[256] = {                             /* Data vals :                      */
/*   0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F   */
    8u,  7u,  6u,  6u,  5u,  5u,  5u,  5u,  4u,  4u,  4u,  4u,  4u,  4u,  4u,  4u,  /*   0x00 to 0x0F                   */
    3u,  3u,  3u,  3u,  3u,  3u,  3u,  3u,  3u,  3u,  3u,  3u,  3u,  3u,  3u,  3u,  /*   0x10 to 0x1F                   */
    2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  /*   0x20 to 0x2F                   */
    2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  2u,  /*   0x30 to 0x3F                   */
    1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  /*   0x40 to 0x4F                   */
    1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  /*   0x50 to 0x5F                   */
    1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  /*   0x60 to 0x6F                   */
    1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  1u,  /*   0x70 to 0x7F                   */
    0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  /*   0x80 to 0x8F                   */
    0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  /*   0x90 to 0x9F                   */
    0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  /*   0xA0 to 0xAF                   */
    0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  /*   0xB0 to 0xBF                   */
    0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  /*   0xC0 to 0xCF                   */
    0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  /*   0xD0 to 0xDF                   */
    0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  /*   0xE0 to 0xEF                   */
    0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u,  0u   /*   0xF0 to 0xFF                   */
};

/*
*********************************************************************************************************
*                                         CPU_CntLeadZeros()
*
* Description : Count the number of contiguous, most-significant, leading zero bits in a data value.
*
* Argument(s) : val         Data value to count leading zero bits.
*
* Return(s)   : Number of contiguous, most-significant, leading zero bits in 'val', if NO error(s).
*
*               DEF_INT_CPU_U_MAX_VAL,                                              otherwise.
*
* Caller(s)   : CPU_CntTrailZeros(),
*               Application.
*
*               This function is a CPU module application programming interface (API) function & MAY 
*               be called by application function(s).
*
* Note(s)     : (1) (a) Supports the following data value sizes :
*
*                       (3) 32-bits
*                       (3) For 32-bit values :
*
*                             b31  b30  b29  ...  b04  b03  b02  b01  b00    # Leading Zeros
*                             ---  ---  ---       ---  ---  ---  ---  ---    ---------------
*                              1    x    x         x    x    x    x    x            0
*                              0    1    x         x    x    x    x    x            1
*                              0    0    1         x    x    x    x    x            2
*                              :    :    :         :    :    :    :    :            :
*                              :    :    :         :    :    :    :    :            :
*                              0    0    0         1    x    x    x    x           27
*                              0    0    0         0    1    x    x    x           28
*                              0    0    0         0    0    1    x    x           29
*                              0    0    0         0    0    0    1    x           30
*                              0    0    0         0    0    0    0    1           31
*                              0    0    0         0    0    0    0    0           32

*
*
*                       See also 'CPU COUNT LEAD ZEROs LOOKUP TABLE  Note #1'.
*********************************************************************************************************
*/

CPU_DATA  CPU_CntTrailZeros (CPU_DATA  val)
{
	CPU_DATA  nbr_lead_zeros;

	nbr_lead_zeros = 31 - CPU_CntLeadZeros32((unsigned int)val);
	return (nbr_lead_zeros);
}

CPU_DATA  CPU_CntLeadZeros (CPU_DATA  val)
{
    CPU_DATA  nbr_lead_zeros;

    nbr_lead_zeros = CPU_CntLeadZeros32((unsigned int)val);
    return (nbr_lead_zeros);
}

CPU_DATA  CPU_CntLeadZeros32 (CPU_DATA  val)
{
    CPU_DATA  ix;
    CPU_DATA  nbr_lead_zeros;

    if (val > 0x0000FFFFu) {
        if (val > 0x00FFFFFFu) {                                                /* Chk bits [31:24] :                   */
                                                                                /* .. Nbr lead zeros =               .. */
            ix             = (CPU_DATA)(val >> 24u);                            /* .. lookup tbl ix  = 'val' >> 24 bits */
            nbr_lead_zeros = (CPU_DATA)(CPU_CntLeadZerosTbl[ix] +  0u);         /* .. plus nbr msb lead zeros =  0 bits.*/

        } else {                                                                /* Chk bits [23:16] :                   */
                                                                                /* .. Nbr lead zeros =               .. */
            ix             = (CPU_DATA)(val >> 16u);                            /* .. lookup tbl ix  = 'val' >> 16 bits */
            nbr_lead_zeros = (CPU_DATA)(CPU_CntLeadZerosTbl[ix] +  8u);         /* .. plus nbr msb lead zeros =  8 bits.*/
        }

    } else {
        if (val > 0x000000FFu) {                                                /* Chk bits [15:08] :                   */
                                                                                /* .. Nbr lead zeros =               .. */
            ix             = (CPU_DATA)(val >>  8u);                            /* .. lookup tbl ix  = 'val' >>  8 bits */
            nbr_lead_zeros = (CPU_DATA)(CPU_CntLeadZerosTbl[ix] + 16u);         /* .. plus nbr msb lead zeros = 16 bits.*/

        } else {                                                                /* Chk bits [07:00] :                   */
                                                                                /* .. Nbr lead zeros =               .. */
            ix             = (CPU_DATA)(val >>  0u);                            /* .. lookup tbl ix  = 'val' >>  0 bits */
            nbr_lead_zeros = (CPU_DATA)(CPU_CntLeadZerosTbl[ix] + 24u);         /* .. plus nbr msb lead zeros = 24 bits.*/
        }
    }

    return (nbr_lead_zeros);
}
#endif
