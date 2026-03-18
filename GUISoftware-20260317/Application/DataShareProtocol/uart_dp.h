#ifndef _UART_DP_H
#define _UART_DP_H
#include "ring_buffer.h"
#include "ddsis_uart.h"
#include "HL7.h"
#include "MedibusComm.h"

#define RECV_PC_SIZE        1
#define RECV_PC_DEPTH       (8*1024)
#define RECV_PC_FIFO_SIZE   (RECV_PC_SIZE*RECV_PC_DEPTH)

#define SEND_PC_SIZE        RECV_PC_SIZE
#define SEND_PC_DEPTH       RECV_PC_DEPTH
#define SEND_PC_FIFO_SIZE   RECV_PC_FIFO_SIZE

//一次发送的最大数据
#define SEND_MAX_SIZE       (1*1024)


typedef void (*UART_RECV_T)(u8);
typedef void (*UART_SEND_T)(void);
#define UART_INIT_T UART_SEND_T
#define UART_FSM_T  UART_SEND_T
#define UART_TIM_T  UART_SEND_T
#define UART_RECVDP_T   UART_SEND_T

//数据共享协议
typedef enum
{
    share_prot_hl7 = 0,
    share_prot_medibus,
    share_prot_last,
}share_prot_t;

typedef struct
{
    UART_RECV_T uart_recv;
    UART_SEND_T uart_send;
}uart_func_t;


/*串口超时复位结构体*/
typedef struct
{
    u16 uart_timeout_count;    
    u8  uart_timeout_flag;
}uart_timeout_t;

typedef struct
{
    //数据分享协议整合
    //初始化
    UART_INIT_T uart_init[share_prot_last];
    //处理接收的数据
    UART_RECVDP_T uart_recv[share_prot_last];
    //状态机
    UART_FSM_T prot_fsm[share_prot_last];
    //计时器
    UART_TIM_T uart_tim[share_prot_last];
    //中断接收和发送
    uart_func_t uart_func;    
}uart_dp_t;


void uart_dp_init(ddsis_comx com);
void uart_dp_opinit(void);
void uart_dp_setprot(share_prot_t prot);
void uart_dp_send(void);
void uart_dp_recv(void);
void uart_dp_fsm(void);
void uart_dp_tim(void);


void uart_dp_clrrb(void);
u32 uart_dp_itemscnt(void);
bool uart_dp_isrecvempty(void);
u32 uart_dp_rdrecv(u8 *buf, u32 num);
u32 uart_dp_wrsend(u8 *buf, u32 num);

void uart_dp_presend(void);

void uart_dp_irqsend(void);
void uart_dp_irqrecv(u8 data);

uart_func_t* uart_dp_readop(void);
void uart_dp_timeout(void);
#endif




