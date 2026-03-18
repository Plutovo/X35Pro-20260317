#ifndef _DDSIS_UART_H
#define _DDSIS_UART_H
#include "lpc_types.h"

#define UART_FIFO_SIZE  16

#define ddsis_it_rbr    (1<<0)
#define ddsis_it_thr    (1<<1)
#define ddsis_it_rls    (1<<2)

#define ddsis_thr_active    0x00

typedef enum
{
    uart_send_finish = 0,
    uart_send_start = 1,
}ddsis_uart_send_flag_t;

typedef enum
{
    uart_send_mode_poll = 0,
    uart_send_mode_it,
}ddsis_uart_send_mode_t;

/*串口帧发送结构体*/  
typedef struct 
{  
    u16 send_sum_len;                   //要发送的帧数据长度  
    u16 send_cur_len;                   //当前已经发送的数据长度  
    u16 send_FIFO_size;                 //硬件FIFO大小
    ddsis_uart_send_flag_t send_flag;   //是否发送标志  
    ddsis_uart_send_mode_t  send_mode;  //是否使用中断发送 0:不使用 1:使用
    u8  *send_buf;                      //指向要发送的数据缓冲区     
}ddsis_uart_send_t;

/*
    ddsis_com1, UART0
    ddsis_com2, UART1
    ddsis_com3, UART2
    ddsis_com4, UART3
    ddsis_com5, UART4
    ddsis_com6, EXUART1
    ddsis_com7, EXUART2
    ddsis_com8, EXUART3
    ddsis_com9, EXUART4
*/
#define ddsis_com_num   9
typedef enum
{
    ddsis_com1 = 0,
    ddsis_com2,
    ddsis_com3,
    ddsis_com4,
    ddsis_com5,
    ddsis_com6,
    ddsis_com7,
    ddsis_com8,
    ddsis_com9,
    ddsis_comlst,
}ddsis_comx;

typedef enum
{
    ddsis_com_state_free = 0,    
    ddsis_com_state_busy,
}ddsis_com_state_t;


typedef struct
{
    void        (*ddsis_com_sendbyte)(u8);
    u8          (*ddsis_com_recvbyte)(void);
    FlagStatus  (*ddsis_com_checkbusy)(void);
    void        (*ddsis_com_itconfig)(u16, FunctionalState);    
}ddsis_com_t;

void ddsis_uart_sendbyte(ddsis_comx uart, u8 data);
void ddsis_uart_sendbytes(ddsis_comx uart, ddsis_uart_send_t* ddsis_send);
u8 ddsis_uart_recvbyte(ddsis_comx uart);
ddsis_com_state_t ddsis_uart_checkbusy(ddsis_comx uart);
void ddsis_uart_itconfig(ddsis_comx uart, u16 uart_it, FunctionalState state);

#endif

