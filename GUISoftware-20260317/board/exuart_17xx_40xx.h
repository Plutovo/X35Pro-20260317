#ifndef __EXUART_17XX_40XX_H_
#define __EXUART_17XX_40XX_H_
#include "sys_config.h"
#include "ring_buffer.h"
#ifdef __cplusplus
extern "C" {
#endif
    
/** @defgroup EXUART_17XX_40XX CHIP: LPC17xx/40xx UART driver
 * @ingroup CHIP_17XX_40XX_Drivers
 * @{
 */
/**
 * @brief EXUART Type: TL16C554 or TL16C550
 */      
typedef enum
{
    TL16C550X = 0,
    TL16C554X = 1,
}LPC_EXUART_TYPE_T;
    
/**
 * @brief EXUART register block structure
 */   
typedef struct
{
	union 
    {
		__IO uint8_t  DLL;			/*!< Divisor Latch LSB. Least significant byte of the baud rate divisor value. The full divisor is used to generate a baud rate from the fractional rate divider (DLAB = 1). */
		__O  uint8_t  THR;			/*!< Transmit Holding Register. The next character to be transmitted is written here (DLAB = 0). */
		__I  uint8_t  RBR;			/*!< Receiver Buffer Register. Contains the next received character to be read (DLAB = 0). */
	};

	union 
    {
		__IO uint8_t IER;			/*!< Interrupt Enable Register. Contains individual interrupt enable bits for the 7 potential UART interrupts (DLAB = 0). */
		__IO uint8_t DLM;			/*!< Divisor Latch MSB. Most significant byte of the baud rate divisor value. The full divisor is used to generate a baud rate from the fractional rate divider (DLAB = 1). */
	};

	union 
    {
		__O  uint8_t FCR;			/*!< FIFO Control Register. Controls UART FIFO usage and modes. */
		__I  uint8_t IIR;			/*!< Interrupt ID Register. Identifies which interrupt(s) are pending. */
	};

	__IO uint8_t LCR;				/*!< Line Control Register. Contains controls for frame formatting and break generation. */
	__IO uint8_t MCR;				/*!< Modem Control Register. Only present on USART ports with full modem support. */
	__I  uint8_t LSR;				/*!< Line Status Register. Contains flags for transmit and receive status, including line errors. */
	__I  uint8_t MSR;				/*!< Modem Status Register. Only present on USART ports with full modem support. */
	__IO uint8_t SCR;				/*!< Scratch Pad Register. Eight-bit temporary storage for software. */    
}LPC_EXUART_T;

/**
 * @brief Macro defines for UART Receive Buffer register
 */
#define EXUART_RBR_MASKBIT    (0xFF)		        /*!< UART Received Buffer mask bit (8 bits) */

/**
 * @brief Macro defines for UART Interrupt Enable Register
 */
#define EXUART_IER_RBRINT      (1 << 0)	/*!< RBR Interrupt enable */
#define EXUART_IER_THREINT     (1 << 1)	/*!< THR Interrupt enable */
#define EXUART_IER_RLSINT      (1 << 2)	/*!< RX line status interrupt enable */
#define EXUART_IER_MSINT       (1 << 3)	/*!< Modem status interrupt enable */
#define EXUART_IER_BITMASK     (0x0F)	/*!< UART interrupt enable register bit mask */
/**
 * @brief Macro defines for UART Interrupt Identification Register
 */
#define EXUART_IIR_INTSTAT_PEND   (1 << 0)	/*!< Interrupt pending status - Active low */
#define EXUART_IIR_FIFO_EN        (3 << 6)	/*!< These bits are equivalent to FCR[0] */
#define EXUART_IIR_BITMASK        (0x0F)		/*!< UART interrupt identification register bit mask */

/* Interrupt ID bit definitions */
#define EXUART_IIR_INTID_RLS      (3 << 1)	/*!< Interrupt identification: Receive line interrupt */
#define EXUART_IIR_INTID_RDA      (2 << 1)	/*!< Interrupt identification: Receive data available interrupt */
#define EXUART_IIR_INTID_CTI      (6 << 1)	/*!< Interrupt identification: Character time-out indicator interrupt */
#define EXUART_IIR_INTID_THRE     (1 << 1)	/*!< Interrupt identification: THRE interrupt */
#define EXUART_IIR_INTID_MODEM    (0 << 1)	/*!< Interrupt identification: Modem interrupt */

/**
 * @brief Macro defines for UART FIFO Control Register
 */
#define EXUART_FCR_FIFO_EN        (1 << 0)	/*!< UART FIFO enable */
#define EXUART_FCR_RX_RS          (1 << 1)	/*!< UART RX FIFO reset */
#define EXUART_FCR_TX_RS          (1 << 2)	/*!< UART TX FIFO reset */
#define EXUART_FCR_DMAMODE_SEL    (1 << 3)	/*!< UART DMA mode selection */
#define EXUART_FCR_BITMASK        (0xCF)	/*!< UART FIFO control bit mask */

#define EXUART_TX_FIFO_SIZE       (16)

/* FIFO trigger level bit definitions */
#define EXUART_FCR_TRG_LEV0       (0)			/*!< UART FIFO trigger level 0: 1 character */
#define EXUART_FCR_TRG_LEV1       (1 << 6)	/*!< UART FIFO trigger level 1: 4 character */
#define EXUART_FCR_TRG_LEV2       (2 << 6)	/*!< UART FIFO trigger level 2: 8 character */
#define EXUART_FCR_TRG_LEV3       (3 << 6)	/*!< UART FIFO trigger level 3: 14 character */

/**
 * @brief Macro defines for UART Line Control Register
 */
/* UART word length select bit definitions */
#define EXUART_LCR_WLEN_MASK      (3 << 0)		/*!< UART word length select bit mask */
#define EXUART_LCR_WLEN5          (0 << 0)		/*!< UART word length select: 5 bit data mode */
#define EXUART_LCR_WLEN6          (1 << 0)		/*!< UART word length select: 6 bit data mode */
#define EXUART_LCR_WLEN7          (2 << 0)		/*!< UART word length select: 7 bit data mode */
#define EXUART_LCR_WLEN8          (3 << 0)		/*!< UART word length select: 8 bit data mode */

/* UART Stop bit select bit definitions */
#define EXUART_LCR_SBS_MASK       (1 << 2)		/*!< UART stop bit select: bit mask */
#define EXUART_LCR_SBS_1BIT       (0 << 2)		/*!< UART stop bit select: 1 stop bit */
#define EXUART_LCR_SBS_2BIT       (1 << 2)		/*!< UART stop bit select: 2 stop bits (in 5 bit data mode, 1.5 stop bits) */

/* UART Parity enable bit definitions */
#define EXUART_LCR_PARITY_EN      (1 << 3)		/*!< UART Parity Enable */
#define EXUART_LCR_PARITY_DIS     (0 << 3)		/*!< UART Parity Disable */
#define EXUART_LCR_PARITY_ODD     (0 << 4)		/*!< UART Parity select: Odd parity */
#define EXUART_LCR_PARITY_EVEN    (1 << 4)		/*!< UART Parity select: Even parity */
#define EXUART_LCR_PARITY_F_1     (2 << 4)		/*!< UART Parity select: Forced 1 stick parity */
#define EXUART_LCR_PARITY_F_0     (3 << 4)		/*!< UART Parity select: Forced 0 stick parity */
#define EXUART_LCR_BREAK_EN       (1 << 6)		/*!< UART Break transmission enable */
#define EXUART_LCR_DLAB_EN        (1 << 7)		/*!< UART Divisor Latches Access bit enable */
#define EXUART_LCR_BITMASK        (0xFF)		/*!< UART line control bit mask */

/**
 * @brief Macro defines for UART Modem Control Register
 */
#define EXUART_MCR_DTR_CTRL       (1 << 0)		/*!< Source for modem output pin DTR */
#define EXUART_MCR_RTS_CTRL       (1 << 1)		/*!< Source for modem output pin RTS */
#define EXUART_MCR_INT_EN         (1 << 3)      /*!< Enable external interrupt */
#define EXUART_MCR_LOOPB_EN       (1 << 4)		/*!< Loop back mode select */
#define EXUART_MCR_AUTO_EN        (1 << 5)		/*!< Enable Auto RTS flow-control */
#define EXUART_MCR_BITMASK        (0x3B)	    /*!< UART bit mask value */

/**
 * @brief Macro defines for UART Line Status Register
 */
#define EXUART_LSR_RDR        (1 << 0)	/*!< Line status: Receive data ready */
#define EXUART_LSR_OE         (1 << 1)	/*!< Line status: Overrun error */
#define EXUART_LSR_PE         (1 << 2)	/*!< Line status: Parity error */
#define EXUART_LSR_FE         (1 << 3)	/*!< Line status: Framing error */
#define EXUART_LSR_BI         (1 << 4)	/*!< Line status: Break interrupt */
#define EXUART_LSR_THRE       (1 << 5)	/*!< Line status: Transmit holding register empty */
#define EXUART_LSR_TEMT       (1 << 6)	/*!< Line status: Transmitter empty */
#define EXUART_LSR_FIFOE      (1 << 7)	/*!< Line status: Error in FIFO */
#define EXUART_LSR_BITMASK    (0xFF)    /*!< UART Line status bit mask */

/**
 * @brief Macro defines for UART Modem Status Register
 */
#define EXUART_MSR_DELTA_CTS      (1 << 0)	/*!< Modem status: State change of input CTS */
#define EXUART_MSR_DELTA_DSR      (1 << 1)	/*!< Modem status: State change of input DSR */
#define EXUART_MSR_LO2HI_RI       (1 << 2)	/*!< Modem status: Low to high transition of input RI */
#define EXUART_MSR_DELTA_DCD      (1 << 3)	/*!< Modem status: State change of input DCD */
#define EXUART_MSR_CTS            (1 << 4)	/*!< Modem status: Clear To Send State */
#define EXUART_MSR_DSR            (1 << 5)	/*!< Modem status: Data Set Ready State */
#define EXUART_MSR_RI             (1 << 6)	/*!< Modem status: Ring Indicator State */
#define EXUART_MSR_DCD            (1 << 7)	/*!< Modem status: Data Carrier Detect State */
#define EXUART_MSR_BITMASK        (0xFF)	/*!< Modem status: MSR register bit-mask value */

/**
 * @brief	Transmit a single data byte through the UART peripheral
 * @param	pUART	: Pointer to selected UART peripheral
 * @param	data	: Byte to transmit
 * @return	Nothing
 * @note	This function attempts to place a byte into the UART transmit
 *			FIFO or transmit hold register regard regardless of UART state
 */
STATIC INLINE void Chip_EXUART_SendByte(LPC_EXUART_T *pUART, uint8_t data)
{
	pUART->THR = (uint8_t) data;
}

/**
 * @brief	Read a single byte data from the UART peripheral
 * @param	pUART	: Pointer to selected UART peripheral
 * @return	A single byte of data read
 * @note	This function reads a byte from the UART receive FIFO or
 *			receive hold register regard regardless of UART state. The
 *			FIFO status should be read first prior to using this function
 */
STATIC INLINE uint8_t Chip_EXUART_ReadByte(LPC_EXUART_T *pUART)
{
	return (uint8_t) (pUART->RBR & EXUART_RBR_MASKBIT);
}

/**
 * @brief	Enable UART interrupts
 * @param	pUART	: Pointer to selected UART peripheral
 * @param	intMask	: OR'ed Interrupts to enable in the Interrupt Enable Register (IER)
 * @return	Nothing
 * @note	Use an OR'ed value of UART_IER_* definitions with this function
 *			to enable specific UART interrupts. The Divisor Latch Access Bit
 *			(DLAB) in LCR must be cleared in order to access the IER register.
 *			This function doesn't alter the DLAB state
 */
STATIC INLINE void Chip_EXUART_IntEnable(LPC_EXUART_T *pUART, uint8_t intMask)
{
	pUART->IER |= intMask;
}

/**
 * @brief	Disable UART interrupts
 * @param	pUART	: Pointer to selected UART peripheral
 * @param	intMask	: OR'ed Interrupts to disable in the Interrupt Enable Register (IER)
 * @return	Nothing
 * @note	Use an OR'ed value of UART_IER_* definitions with this function
 *			to disable specific UART interrupts. The Divisor Latch Access Bit
 *			(DLAB) in LCR must be cleared in order to access the IER register.
 *			This function doesn't alter the DLAB state
 */
STATIC INLINE void Chip_EXUART_IntDisable(LPC_EXUART_T *pUART, uint8_t intMask)
{
	pUART->IER &= ~intMask;
}

/**
 * @brief	Returns UART interrupts that are enabled
 * @param	pUART	: Pointer to selected UART peripheral
 * @return	Returns the enabled UART interrupts
 * @note	Use an OR'ed value of UART_IER_* definitions with this function
 *			to determine which interrupts are enabled. You can check
 *			for multiple enabled bits if needed.
 */
STATIC INLINE uint8_t Chip_EXUART_GetIntsEnabled(LPC_EXUART_T *pUART)
{
	return pUART->IER;
}

/**
 * @brief	Read the Interrupt Identification Register (IIR)
 * @param	pUART	: Pointer to selected UART peripheral
 * @return	Current pending interrupt status per the IIR register
 */
STATIC INLINE uint8_t Chip_EXUART_ReadIntIDReg(LPC_EXUART_T *pUART)
{
	return pUART->IIR;
}

/**
 * @brief	Setup the UART FIFOs
 * @param	pUART	: Pointer to selected UART peripheral
 * @param	fcr		: FIFO control register setup OR'ed flags
 * @return	Nothing
 * @note	Use OR'ed value of UART_FCR_* definitions with this function
 *			to select specific options. For example, to enable the FIFOs
 *			with a RX trip level of 8 characters, use something like
 *			(UART_FCR_FIFO_EN | UART_FCR_TRG_LEV2)
 */
STATIC INLINE void Chip_EXUART_SetupFIFOS(LPC_EXUART_T *pUART, uint8_t fcr)
{
	pUART->FCR = fcr;
}

/**
 * @brief	Configure data width, parity and stop bits
 * @param	pUART	: Pointer to selected pUART peripheral
 * @param	config	: UART configuration, OR'ed values of UART_LCR_* defines
 * @return	Nothing
 * @note	Select OR'ed config options for the UART from the UART_LCR_*
 *			definitions. For example, a configuration of 8 data bits, 1
 *			stop bit, and even (enabled) parity would be
 *			(UART_LCR_WLEN8 | UART_LCR_SBS_1BIT | UART_LCR_PARITY_EN | UART_LCR_PARITY_EVEN)
 */
STATIC INLINE void Chip_EXUART_ConfigData(LPC_EXUART_T *pUART, uint8_t config)
{
	pUART->LCR = config;
}

/**
 * @brief	Enable access to Divisor Latches
 * @param	pUART	: Pointer to selected UART peripheral
 * @return	Nothing
 */
STATIC INLINE void Chip_EXUART_EnableDivisorAccess(LPC_EXUART_T *pUART)
{
	pUART->LCR |= EXUART_LCR_DLAB_EN;
}

/**
 * @brief	Disable access to Divisor Latches
 * @param	pUART	: Pointer to selected UART peripheral
 * @return	Nothing
 */
STATIC INLINE void Chip_EXUART_DisableDivisorAccess(LPC_EXUART_T *pUART)
{
	pUART->LCR &= ~EXUART_LCR_DLAB_EN;
}

/**
 * @brief	Set LSB and MSB divisor latch registers
 * @param	pUART	: Pointer to selected UART peripheral
 * @param	dll		: Divisor Latch LSB value
 * @param	dlm		: Divisor Latch MSB value
 * @return	Nothing
 * @note	The Divisor Latch Access Bit (DLAB) in LCR must be set in
 *			order to access the USART Divisor Latches. This function
 *			doesn't alter the DLAB state.
 */
STATIC INLINE void Chip_EXUART_SetDivisorLatches(LPC_EXUART_T *pUART, uint8_t dll, uint8_t dlm)
{
	pUART->DLL = (uint32_t) dll;
	pUART->DLM = (uint32_t) dlm;
}

/**
 * @brief	Return modem control register/status
 * @param	pUART	: Pointer to selected UART peripheral
 * @return	Modem control register (status)
 * @note	Mask bits of the returned status value with UART_MCR_*
 *			definitions for specific statuses.
 */
STATIC INLINE uint32_t Chip_EXUART_ReadModemControl(LPC_EXUART_T *pUART)
{
	return pUART->MCR;
}

/**
 * @brief	Set modem control register/status
 * @param	pUART	: Pointer to selected UART peripheral
 * @param	mcr		: Modem control register flags to set
 * @return	Nothing
 * @note	Use an Or'ed value of UART_MCR_* definitions with this
 *			call to set specific options.
 */
STATIC INLINE void Chip_EXUART_SetModemControl(LPC_EXUART_T *pUART, uint8_t mcr)
{
	pUART->MCR |= mcr;
}

/**
 * @brief	Clear modem control register/status
 * @param	pUART	: Pointer to selected UART peripheral
 * @param	mcr		: Modem control register flags to clear
 * @return	Nothing
 * @note	Use an Or'ed value of UART_MCR_* definitions with this
 *			call to clear specific options.
 */
STATIC INLINE void Chip_EXUART_ClearModemControl(LPC_EXUART_T *pUART, uint8_t mcr)
{
	pUART->MCR &= ~mcr;
}

/**
 * @brief	Return Line Status register/status (LSR)
 * @param	pUART	: Pointer to selected UART peripheral
 * @return	Line Status register (status)
 * @note	Mask bits of the returned status value with UART_LSR_*
 *			definitions for specific statuses.
 */
STATIC INLINE uint32_t Chip_EXUART_ReadLineStatus(LPC_EXUART_T *pUART)
{
	return pUART->LSR;
}

/**
 * @brief	Return Modem Status register/status (MSR)
 * @param	pUART	: Pointer to selected UART peripheral
 * @return	Modem Status register (status)
 * @note	Mask bits of the returned status value with UART_MSR_*
 *			definitions for specific statuses.
 */
STATIC INLINE uint32_t Chip_EXUART_ReadModemStatus(LPC_EXUART_T *pUART)
{
	return pUART->MSR;
}

#ifdef CHIP_EXUART_USER
/**
 * @brief	Set the EXUART Chip Type
 * @param	Nothing
 * @return	Nothing
 */
void Chip_EXUART_SetChipType(LPC_EXUART_TYPE_T type);
#endif

/**
 * @brief	Reset the pUART peripheral
 * @param	Nothing
 * @return	Nothing
 */
void Chip_EXUART_Reset(void);

/**
 * @brief	Initializes the pUART peripheral
 * @param	pUART		: Pointer to selected pUART peripheral
 * @return	Nothing
 */
void Chip_EXUART_Init(LPC_EXUART_T *pUART);

/**
 * @brief	De-initializes the pUART peripheral.
 * @param	pUART		: Pointer to selected pUART peripheral
 * @return	Nothing
 */
void Chip_EXUART_DeInit(LPC_EXUART_T *pUART);

/**
 * @brief	Check whether if UART is busy or not
 * @param	pUART	: Pointer to selected pUART peripheral
 * @return	RESET if UART is not busy, otherwise return SET
 */
FlagStatus Chip_EXUART_CheckBusy(LPC_EXUART_T *pUART);

/**
 * @brief	Check whether if UART FIFO is busy or not
 * @param	pUART	: Pointer to selected pUART peripheral
 * @return	RESET if UART is not busy, otherwise return SET
 */
FlagStatus Chip_EXUART_CheckFIFOBusy(LPC_EXUART_T *pUART);

/**
 * @brief	Transmit a byte array through the UART peripheral (non-blocking)
 * @param	pUART		: Pointer to selected UART peripheral
 * @param	data		: Pointer to bytes to transmit
 * @param	numBytes	: Number of bytes to transmit
 * @return	The actual number of bytes placed into the FIFO
 * @note	This function places data into the transmit FIFO until either
 *			all the data is in the FIFO or the FIFO is full. This function
 *			will not block in the FIFO is full. The actual number of bytes
 *			placed into the FIFO is returned. This function ignores errors.
 */
int Chip_EXUART_Send(LPC_EXUART_T *pUART, const void *data, int numBytes);

/**
 * @brief	Read data through the UART peripheral (non-blocking)
 * @param	pUART		: Pointer to selected UART peripheral
 * @param	data		: Pointer to bytes array to fill
 * @param	numBytes	: Size of the passed data array
 * @return	The actual number of bytes read
 * @note	This function reads data from the receive FIFO until either
 *			all the data has been read or the passed buffer is completely full.
 *			This function will not block. This function ignores errors.
 */
int Chip_EXUART_Read(LPC_EXUART_T *pUART, void *data, int numBytes);

/**
 * @brief	Sets best dividers to get a target bit rate (without fractional divider)
 * @param	pUART		: Pointer to selected UART peripheral
 * @param	baudrate	: Target baud rate (baud rate = bit rate)
 * @return	The actual baud rate, or 0 if no rate can be found
 */
uint32_t Chip_EXUART_SetBaud(LPC_EXUART_T *pUART, uint32_t baudrate);

/**
 * @brief	Transmit a byte array through the UART peripheral (blocking)
 * @param	pUART		: Pointer to selected UART peripheral
 * @param	data		: Pointer to data to transmit
 * @param	numBytes	: Number of bytes to transmit
 * @return	The number of bytes transmitted
 * @note	This function will send or place all bytes into the transmit
 *			FIFO. This function will block until the last bytes are in the FIFO.
 */
int Chip_EXUART_SendBlocking(LPC_EXUART_T *pUART, const void *data, int numBytes);

/**
 * @brief	Read data through the UART peripheral (blocking)
 * @param	pUART		: Pointer to selected UART peripheral
 * @param	data		: Pointer to data array to fill
 * @param	numBytes	: Size of the passed data array
 * @return	The size of the dat array
 * @note	This function reads data from the receive FIFO until the passed
 *			buffer is completely full. The function will block until full.
 *			This function ignores errors.
 */
int Chip_EXUART_ReadBlocking(LPC_EXUART_T *pUART, void *data, int numBytes);


/* TL16C550X CLK*/
#define LPC_TL16C550X_CLK   8000000

/* TL16C554X CLK*/
#define LPC_TL16C554X_CLK   11059200

/*if def CHIP_EXUART_USER, Chip Clk and Base addr init by user*/
#ifdef CHIP_EXUART_USER
/*In TL16C550, LPC_EXUART3 and LPC_EXUART4 should not use*/
extern volatile uint32_t LPC_EXUART1_BASE;
extern volatile uint32_t LPC_EXUART2_BASE;
extern volatile uint32_t LPC_EXUART3_BASE;
extern volatile uint32_t LPC_EXUART4_BASE;

#else
#ifdef CHIP_TL16C554X
#define LPC_EXUART_BASE     EMC_ADDRESS_CS1
#define LPC_EXUART1_BASE    (LPC_EXUART_BASE | (0x00 << 3))
#define LPC_EXUART2_BASE    (LPC_EXUART_BASE | (0x01 << 3))
#define LPC_EXUART3_BASE    (LPC_EXUART_BASE | (0x02 << 3))
#define LPC_EXUART4_BASE    (LPC_EXUART_BASE | (0x03 << 3))
#define LPC_EXUART_RATE     LPC_TL16C554X_CLK
#else   
/*In TL16C550, LPC_EXUART3 and LPC_EXUART4 should not use*/
#define LPC_EXUART1_BASE    EMC_ADDRESS_CS2
#define LPC_EXUART2_BASE    EMC_ADDRESS_CS3
#define LPC_EXUART3_BASE    EMC_ADDRESS_CS2
#define LPC_EXUART4_BASE    EMC_ADDRESS_CS2
#define LPC_EXUART_RATE     LPC_TL16C550X_CLK
#endif
#endif

#define LPC_EXUART1         ((LPC_EXUART_T *)LPC_EXUART1_BASE)
#define LPC_EXUART2         ((LPC_EXUART_T *)LPC_EXUART2_BASE)
#define LPC_EXUART3         ((LPC_EXUART_T *)LPC_EXUART3_BASE)
#define LPC_EXUART4         ((LPC_EXUART_T *)LPC_EXUART4_BASE)

/**
 * @}
 */    
    
#ifdef __cplusplus
}
#endif    
    
#endif  /* __EXUART_17XX_40XX_H_ */
