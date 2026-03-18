/*
 * @brief Embedded Artists LPC1788 Sysinit file
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2013
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#include "board.h"

/* The System initialization code is called prior to the application and
   initializes the board for run-time operation. Board initialization
   includes clock setup and default pin muxing configuration. */

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/* EMC clock delay */
#define DELAY_CMD   0x0B
#define DELAY_FBCLK 0x0B
#define DELAY_CLK0  0x0B
#define DELAY_CLK1  0x00

/* SDRAM timing and chip Config */

STATIC const IP_EMC_DYN_CONFIG_T HY57V281620F_cfg = 
{
    //timing
    .RefreshPeriod  = SDRAM_HY57_REFRESH,
	.ReadConfig     = 0x01,				/* Command Delayed */
	.tRP            = SDRAM_HY57_TRP,   /* tRP */
    .tRAS           = SDRAM_HY57_TRAS,  /* tRAS */
    .tSREX          = SDRAM_HY57_TSREX, /* tSREX */
    .tAPR           = SDRAM_HY57_TAPR,	/* tAPR */
    .tDAL           = SDRAM_HY57_TDAL,	/* tDAL */
    .tWR            = SDRAM_HY57_TWR,   /* tWR */
    .tRC            = SDRAM_HY57_TRC,   /* tRC */
    .tRFC           = SDRAM_HY57_TRFC,	/* tRFC */
    .tXSR           = SDRAM_HY57_TXSR,	/* tXSR */
    .tRRD           = SDRAM_HY57_TRRD,	/* tRRD */
    .tMRD           = SDRAM_HY57_TMRD,	/* tMRD */
    
    //DYCS0
    .DevConfig[0].BaseAddr      =   EMC_ADDRESS_DYCS0,
    .DevConfig[0].RAS           =   3,
    .DevConfig[0].ModeRegister  =   EMC_DYN_MODE_WBMODE_PROGRAMMED | 
                                    EMC_DYN_MODE_OPMODE_STANDARD | 
                                    EMC_DYN_MODE_CAS_3 | 
                                    EMC_DYN_MODE_BURST_TYPE_SEQUENTIAL | 
                                    EMC_DYN_MODE_BURST_LEN_8,
    .DevConfig[0].DynConfig     =   EMC_DYN_CONFIG_DATA_BUS_16 | 
                                    EMC_DYN_CONFIG_8Mx16_4BANKS_12ROWS_9COLS | 
                                    EMC_DYN_CONFIG_MD_SDRAM,
    //DYCS1
    .DevConfig[1].BaseAddr      = 0,
    .DevConfig[1].RAS           = 0,
    .DevConfig[1].ModeRegister  = 0,
    .DevConfig[1].DynConfig     = 0,
    
    //DYCS2
    .DevConfig[2].BaseAddr      = 0,
    .DevConfig[2].RAS           = 0,
    .DevConfig[2].ModeRegister  = 0,
    .DevConfig[2].DynConfig     = 0, 
    
    //DYCS3
    .DevConfig[3].BaseAddr      = 0,
    .DevConfig[3].RAS           = 0,
    .DevConfig[3].ModeRegister  = 0,
    .DevConfig[3].DynConfig     = 0,   
};

/* NorFlash timing and chip Config */
STATIC const IP_EMC_STATIC_CONFIG_T S29GLXXXX_cfg = 
{
    .ChipSelect =   0,
    .Config     = 	EMC_STATIC_CONFIG_MEM_WIDTH_16 |
                    EMC_STATIC_CONFIG_CS_POL_ACTIVE_LOW |
                    EMC_STATIC_CONFIG_BLS_HIGH |
                    EMC_STATIC_CONFIG_EW_DISABLE,
                    
    .WaitWen    =   NORFLASH_S29_WAITWEN,
    .WaitOen    =   NORFLASH_S29_WAITOEN,
    .WaitRd     =   NORFLASH_S29_WAITRD,
    .WaitPage   =   NORFLASH_S29_WAITPG,
    .WaitWr     =   NORFLASH_S29_WAITWR,
    .WaitTurn   =   NORFLASH_S29_WAITTURN,
};

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/
const uint32_t OscRateIn = 12000000;
const uint32_t RTCOscRateIn = 32768;
/*****************************************************************************
 * Private functions
 ****************************************************************************/

/*****************************************************************************
 * Public functions
 ****************************************************************************/


/* Setup system clocking */
void Board_SetupClocking(void)
{
	/* Enable PBOOST for CPU clock over 100MHz */
	Chip_SYSCTL_EnableBoost();
    Chip_SYSCTL_SetFLASHAccess(FLASHTIM_120MHZ_CPU);
	Chip_SetupXtalClocking();
}

/* Sets up system pin muxing */
void Board_SetupMuxing(void)
{
    Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_EMC);
	Chip_SYSCTL_PeriphReset(SYSCTL_RESET_IOCON);

	/* Setup data, address, and EMC control pins with high slew rate */
    /* P3.0-P3.15 */
    //1<<9 | 1
    IOCON_P3_00 = 0x201;//D0
    IOCON_P3_01 = 0x201;//D1
    IOCON_P3_02 = 0x201;//D2 
    IOCON_P3_03 = 0x201;//D3 
    IOCON_P3_04 = 0x201;//D4
    IOCON_P3_05 = 0x201;//D5
    IOCON_P3_06 = 0x201;//D6
    IOCON_P3_07 = 0x201;//D7
    IOCON_P3_08 = 0x201;//D8
    IOCON_P3_09 = 0x201;//D9
    IOCON_P3_10 = 0x201;//D10
    IOCON_P3_11 = 0x201;//D11
    IOCON_P3_12 = 0x201;//D12
    IOCON_P3_13 = 0x201;//D13
    IOCON_P3_14 = 0x201;//D14
    IOCON_P3_15 = 0x201;//D15   

    /* P4.0-P4.31 */
    IOCON_P4_00 = 0x201;//A0
    IOCON_P4_01 = 0x201;//A1
    IOCON_P4_02 = 0x201;//A2
    IOCON_P4_03 = 0x201;//A3
    IOCON_P4_04 = 0x201;//A4
    IOCON_P4_05 = 0x201;//A5
    IOCON_P4_06 = 0x201;//A6
    IOCON_P4_07 = 0x201;//A7
    IOCON_P4_08 = 0x201;//A8
    IOCON_P4_09 = 0x201;//A9
    IOCON_P4_10 = 0x201;//A10
    IOCON_P4_11 = 0x201;//A11
    IOCON_P4_12 = 0x201;//A12
    IOCON_P4_13 = 0x201;//A13
    IOCON_P4_14 = 0x201;//A14
    IOCON_P4_15 = 0x201;//A15
    IOCON_P4_16 = 0x201;//A16
    IOCON_P4_17 = 0x201;//A17
    IOCON_P4_18 = 0x201;//A18
    IOCON_P4_19 = 0x201;//A19
    IOCON_P4_20 = 0x201;//A20
    IOCON_P4_21 = 0x201;//A21
    IOCON_P4_22 = 0x201;//A22
    IOCON_P4_23 = 0x201;//A22
    IOCON_P4_24 = 0x201;
    IOCON_P4_25 = 0x201;
    IOCON_P4_26 = 0x201;
    IOCON_P4_27 = 0x201;
    IOCON_P4_28 = 0x201;
    IOCON_P4_29 = 0x201;
    IOCON_P4_30 = 0x201;
    IOCON_P4_31 = 0x201;
   
    /* P2.14-P2.31 */
    IOCON_P2_14 = 0x201;
    IOCON_P2_15 = 0x201;
    IOCON_P2_16 = 0x201;
    IOCON_P2_17 = 0x201;
    IOCON_P2_18 = 0x201;  
    IOCON_P2_19 = 0x201; 
    IOCON_P2_20 = 0x201;
    IOCON_P2_21 = 0x201;
    IOCON_P2_22 = 0x201;
    IOCON_P2_23 = 0x201;
    IOCON_P2_24 = 0x201;
    IOCON_P2_25 = 0x201;
    IOCON_P2_26 = 0x201;
    IOCON_P2_27 = 0x201;
    IOCON_P2_28 = 0x201;
    IOCON_P2_29 = 0x201; 
    IOCON_P2_30 = 0x201;
    IOCON_P2_31 = 0x201;
    
    IOCON_P5_00 = 0x201;
    IOCON_P5_01 = 0x201;   
}

/* Setup external memories */
void Board_SetupExtMemory(void)
{
	/* Setup EMC Delays */
	/* Move all clock delays together */
	LPC_SYSCTL->EMCDLYCTL = (DELAY_CMD) | (DELAY_FBCLK << 8) | (DELAY_CLK0 << 16 | (DELAY_CLK1 << 24));

	/* Setup EMC Clock Divider for divide by 1 */
	/* Setup EMC clock for a divider of 1 from CPU clock. Enable EMC clock for
	   external memory setup of DRAM. */
    Chip_Clock_SetEMCClockDiv(SYSCTL_EMC_DIV1);
	Chip_SYSCTL_PeriphReset(SYSCTL_RESET_EMC);

	/* Init EMC Controller -Enable-LE mode- clock ratio 1:1 */
	Chip_EMC_Init(1, 0, 0);
	/* Init EMC Dynamic Controller */
	Chip_EMC_Dynamic_Init((IP_EMC_DYN_CONFIG_T *) &HY57V281620F_cfg);   //  MT48LC8M16A2_cfg

	/* Init EMC Static Controller CS0 */
	Chip_EMC_Static_Init((IP_EMC_STATIC_CONFIG_T *) &S29GLXXXX_cfg);

	/* EMC Shift Control */
	LPC_SYSCTL->SCS |= 1;
}

/* Set up and initialize hardware prior to call to main */
void Board_SystemInit(void)
{
	Board_SetupMuxing();    
	Board_SetupClocking();    
	Board_SetupExtMemory();
    Chip_EXUART_Reset();
}
