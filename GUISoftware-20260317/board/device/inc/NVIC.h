#ifndef __NVIC_H
#define __NVIC_H

void interrupt_init(void);

void GPIO_IRQHandler(void);
void TIMER0_IRQHandler (void) ;
void TIMER1_IRQHandler (void);
void TIMER2_IRQHandler (void);
void PWM0_IRQHandler(void);
void RTC_IRQHandler(void);


extern volatile int lcd_delay;
extern unsigned char  DMA2LCD_Complish;
extern unsigned char BATTERY_Charge,AC_OnLine;

#ifdef IAR_LPC_1788_STK

#define AIRCR_VECTKEY_MASK    ((uint32_t)0x05FA0000)
#define NVIC_PriorityGroup_0         ((uint32_t)0x700) 
                                                        
#define NVIC_PriorityGroup_1         ((uint32_t)0x600) 
                                                       
#define NVIC_PriorityGroup_2         ((uint32_t)0x500) 
                                                        
#define NVIC_PriorityGroup_3         ((uint32_t)0x400)
                                                       
#define NVIC_PriorityGroup_4         ((uint32_t)0x300) 
                                                         
void NVIC_PriorityGroupConfig(unsigned int NVIC_PriorityGroup);	
void NVIC_SetPriority(int IRQn, uint32_t priority);
void NVIC_EnableIRQ(int IRQn);

typedef enum IRQn
{
/******  Cortex-M3 Processor Exceptions Numbers ***************************************************/
  Reset_IRQn                    = 1,      /*!< 1 Reset Vector, invoked on PowerUp and warm reset*/
  NonMaskableInt_IRQn           = 2,      /*!< 2 Non Maskable Interrupt                         */
  HardFault_IRQn                = 3,      /*!< 3  Hard Fault, all classes of Fault              */
  MemoryManagement_IRQn         = 4,      /*!< 4 Cortex-M3 Memory Management Interrupt          */
  BusFault_IRQn                 = 5,      /*!< 5 Cortex-M3 Bus Fault Interrupt                  */
  UsageFault_IRQn               = 6,      /*!< 6 Cortex-M3 Usage Fault Interrupt                */
  SVCall_IRQn                   = 11,       /*!< 11 Cortex-M3 SV Call Interrupt                   */
  DebugMonitor_IRQn             = 12,       /*!< 12 Cortex-M3 Debug Monitor Interrupt             */
  PendSV_IRQn                   = 14,       /*!< 14 Cortex-M3 Pend SV Interrupt                   */
  SysTick_IRQn                  = 15,       /*!< 15 Cortex-M3 System Tick Interrupt               */

/******  LPC177x_8x Specific Interrupt Numbers *******************************************************/
  WDT_IRQn                      = 0 + 16,        /*!< Watchdog Timer Interrupt                         */
  TIMER0_IRQn                   = 1 + 16,        /*!< Timer0 Interrupt                                 */
  TIMER1_IRQn                   = 2 + 16,        /*!< Timer1 Interrupt                                 */
  TIMER2_IRQn                   = 3 + 16,        /*!< Timer2 Interrupt                                 */
  TIMER3_IRQn                   = 4 + 16,        /*!< Timer3 Interrupt                                 */
  UART0_IRQn                    = 5 + 16,        /*!< UART0 Interrupt                                  */
  UART1_IRQn                    = 6 + 16,        /*!< UART1 Interrupt                                  */
  UART2_IRQn                    = 7 + 16,        /*!< UART2 Interrupt                                  */
  UART3_IRQn                    = 8 + 16,        /*!< UART3 Interrupt                                  */
  PWM1_IRQn                     = 9 + 16,        /*!< PWM1 Interrupt                                   */
  I2C0_IRQn                     = 10 + 16,       /*!< I2C0 Interrupt                                   */
  I2C1_IRQn                     = 11 + 16,       /*!< I2C1 Interrupt                                   */
  I2C2_IRQn                     = 12 + 16,       /*!< I2C2 Interrupt                                   */
  Reserved0_IRQn                = 13 + 16,       /*!< Reserved                                         */
  SSP0_IRQn                     = 14 + 16,       /*!< SSP0 Interrupt                                   */
  SSP1_IRQn                     = 15 + 16,       /*!< SSP1 Interrupt                                   */
  PLL0_IRQn                     = 16 + 16,       /*!< PLL0 Lock (Main PLL) Interrupt                   */
  RTC_IRQn                      = 17 + 16,       /*!< Real Time Clock Interrupt                        */
  EINT0_IRQn                    = 18 + 16,       /*!< External Interrupt 0 Interrupt                   */
  EINT1_IRQn                    = 19 + 16,       /*!< External Interrupt 1 Interrupt                   */
  EINT2_IRQn                    = 20 + 16,       /*!< External Interrupt 2 Interrupt                   */
  EINT3_IRQn                    = 21 + 16,       /*!< External Interrupt 3 Interrupt                   */
  ADC_IRQn                      = 22 + 16,       /*!< A/D Converter Interrupt                          */
  BOD_IRQn                      = 23 + 16,       /*!< Brown-Out Detect Interrupt                       */
  USB_IRQn                      = 24 + 16,       /*!< USB Interrupt                                    */
  CAN_IRQn                      = 25 + 16,       /*!< CAN Interrupt                                    */
  DMA_IRQn                      = 26 + 16,       /*!< General Purpose DMA Interrupt                    */
  I2S_IRQn                      = 27 + 16,       /*!< I2S Interrupt                                    */
  ENET_IRQn                     = 28 + 16,       /*!< Ethernet Interrupt                               */
  MCI_IRQn                      = 29 + 16,       /*!< SD/MMC card I/F Interrupt                        */
  MCPWM_IRQn                    = 30 + 16,       /*!< Motor Control PWM Interrupt                      */
  QEI_IRQn                      = 31 + 16,       /*!< Quadrature Encoder Interface Interrupt           */
  PLL1_IRQn                     = 32 + 16,       /*!< PLL1 Lock (USB PLL) Interrupt                    */
  USBActivity_IRQn              = 33 + 16,       /*!< USB Activity interrupt                           */
  CANActivity_IRQn              = 34 + 16,       /*!< CAN Activity interrupt                           */
  UART4_IRQn                    = 35 + 16,       /*!< UART4 Interrupt                                  */
  SSP2_IRQn                     = 36 + 16,       /*!< SSP2 Interrupt                                   */
  LCD_IRQn                      = 37 + 16,       /*!< LCD Interrupt                                    */
  GPIO_IRQn                     = 38 + 16,       /*!< GPIO Interrupt                                   */
  PWM0_IRQn                     = 39 + 16,       /*!< PWM0 Interrupt                                   */
  EEPROM_IRQn                   = 40 + 16,       /*!< EEPROM Interrupt                           */
} IRQn_Type;
#endif


#endif
