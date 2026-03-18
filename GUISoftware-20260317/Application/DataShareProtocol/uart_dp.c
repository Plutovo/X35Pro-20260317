#include "uart_dp.h"
#include "string.h"


static uart_timeout_t extuart1_timeout    = {0};     //串口超时复位结构体
//接收/发送FIFO
static RINGBUFF_T exuart1_recv_rb;
static u8 exuart1_recv_fifo[RECV_PC_FIFO_SIZE] POOL_EXRAM;
static RINGBUFF_T exuart1_send_rb;
static u8 exuart1_send_fifo[SEND_PC_FIFO_SIZE] POOL_EXRAM;

//数据发送控制
static ddsis_uart_send_t exuart1_send;
static u8 exuart1_send_buf[SEND_MAX_SIZE] POOL_EXRAM;

static uart_dp_t uart_dp;
static ddsis_comx dp_com;

//协议选择
static share_prot_t share_prot;

void uart_dp_init(ddsis_comx com)
{
    memset(&exuart1_recv_rb,    0, sizeof(RINGBUFF_T));
    memset(&exuart1_send_rb,    0, sizeof(RINGBUFF_T));
    memset(&exuart1_send,       0, sizeof(ddsis_uart_send_t));
    memset(exuart1_recv_fifo,   0, RECV_PC_FIFO_SIZE);
    memset(exuart1_send_fifo,   0, SEND_PC_FIFO_SIZE);
    memset(exuart1_send_buf,    0, SEND_MAX_SIZE);
    memset(&extuart1_timeout,       0,sizeof(extuart1_timeout));
    
    RingBuffer_Init(&exuart1_recv_rb, exuart1_recv_fifo, RECV_PC_SIZE, RECV_PC_DEPTH);
    RingBuffer_Init(&exuart1_send_rb, exuart1_send_fifo, SEND_PC_SIZE, SEND_PC_DEPTH);
    
    exuart1_send.send_cur_len = 0;
    exuart1_send.send_sum_len = 0;
    exuart1_send.send_buf = exuart1_send_buf;
    exuart1_send.send_FIFO_size = UART_FIFO_SIZE;   //硬件FIFO大小
    exuart1_send.send_flag = uart_send_finish;
    exuart1_send.send_mode = uart_send_mode_it;  
    
    uart_dp.uart_init[share_prot_hl7] = HL7_Init;
    uart_dp.uart_init[share_prot_medibus] = Medibus_Init;
    
    uart_dp.uart_recv[share_prot_hl7] = NULL;
    uart_dp.uart_recv[share_prot_medibus] = Medibus_frmrecv;
    
    uart_dp.uart_tim[share_prot_hl7] = NULL;
    uart_dp.uart_tim[share_prot_medibus] = Medibus_timer;
    
    uart_dp.prot_fsm[share_prot_hl7] = HL7_Send;
    uart_dp.prot_fsm[share_prot_medibus] = Medibus_FSM_Start;
    
    uart_dp.uart_func.uart_recv = uart_dp_irqrecv;
    uart_dp.uart_func.uart_send = uart_dp_irqsend;    
    
    dp_com = com;
    share_prot = share_prot_hl7;//;share_prot_medibus
    uart_dp_opinit();    
}

void uart_dp_setprot(share_prot_t prot)
{
    share_prot = prot;
}

void uart_dp_opinit(void)
{
    if(uart_dp.uart_init[share_prot] != NULL)
    {
        uart_dp.uart_init[share_prot]();
    }
}

void uart_dp_fsm(void)
{
    if(uart_dp.prot_fsm[share_prot] != NULL)
    {
        uart_dp.prot_fsm[share_prot]();
    }
}

void uart_dp_tim(void)
{
    if(uart_dp.uart_tim[share_prot] != NULL)
    {
        uart_dp.uart_tim[share_prot]();
    }
}

void uart_dp_recv(void)
{
    if(uart_dp.uart_recv[share_prot] != NULL)
    {
        uart_dp.uart_recv[share_prot]();
    }
}

void uart_dp_send(void)
{
    uart_dp_presend();          //出FIFO准备发送，共用FIFO
    uart_dp_timeout();          //串口超时
}

void uart_dp_clrrb(void)
{
    RingBuffer_Flush(&exuart1_recv_rb);
    RingBuffer_Flush(&exuart1_send_rb);
}

//项目数
u32 uart_dp_itemscnt(void)
{
    return RingBuffer_GetCount(&exuart1_recv_rb);    
}

//判断是否为空
bool uart_dp_isrecvempty(void)
{
    if(RingBuffer_IsEmpty(&exuart1_recv_rb))
    {
        return true;
    }
    return false;
}

//数据接收出FIFO
u32 uart_dp_rdrecv(u8 *buf, u32 num)
{
    return RingBuffer_PopMult(&exuart1_recv_rb, buf, num);
}


//数据发送入FIFO
u32 uart_dp_wrsend(u8 *buf, u32 num)
{
    return RingBuffer_InsertMult(&exuart1_send_rb, buf, num);
}

//数据出FIFO准备发送
void uart_dp_presend(void)
{
    int send_size = 0;
    
    //30s发送中断没有发送数据 复位中断
    if(extuart1_timeout.uart_timeout_flag == 1)
    {
        extuart1_timeout.uart_timeout_flag = 0;
        uart_dp.uart_init[share_prot]();
    }
    
    send_size = RingBuffer_PopMult(&exuart1_send_rb, exuart1_send_buf, SEND_MAX_SIZE);
    if(send_size != 0)
    {
        exuart1_send.send_cur_len = 0;
        exuart1_send.send_sum_len = send_size;
        exuart1_send.send_flag = uart_send_start;
        if(exuart1_send.send_mode == uart_send_mode_it)
        {
            ddsis_uart_itconfig(dp_com, ddsis_it_thr, ENABLE);
        }
    }
}

//
void uart_dp_timeout(void)
{
    if(extuart1_timeout.uart_timeout_count > 500*40) //20s计时超时
    {
        extuart1_timeout.uart_timeout_count = 0;
        extuart1_timeout.uart_timeout_flag = 1;
    }
    else
    {
        extuart1_timeout.uart_timeout_count++;
    }
}

//中断发送和接收
//只管接收，数据处理不在这
void uart_dp_irqrecv(u8 data)
{
    RingBuffer_InsertMult(&exuart1_recv_rb, &data, 1);
}

//只管发送，数据处理不在这
void uart_dp_irqsend(void)
{
    extuart1_timeout.uart_timeout_count = 0;    
    ddsis_uart_sendbytes(dp_com, &exuart1_send);
}

uart_func_t* uart_dp_readop(void)
{
    return &uart_dp.uart_func;
}

