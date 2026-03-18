#include "ddsis_uart.h"
#include "exuart_17xx_40xx.h"

////////////////////////////////////////////////////////////////////////////////
static void ddsis_com1_sendbyte(u8 data)
{

}

static u8   ddsis_com1_recvbyte(void)
{

}

static FlagStatus ddsis_com1_checkbusy(void)
{

}

void ddsis_com1_itconfig(u16 uart_it, FunctionalState state)
{

}

////////////////////////////////////////////////////////////////////////////////
static void ddsis_com2_sendbyte(u8 data)
{

}

static u8   ddsis_com2_recvbyte(void)
{

}

static FlagStatus ddsis_com2_checkbusy(void)
{

}

void ddsis_com2_itconfig(u16 uart_it, FunctionalState state)
{
   
}

////////////////////////////////////////////////////////////////////////////////

static void ddsis_com3_sendbyte(u8 data)
{

}

static u8   ddsis_com3_recvbyte(void)
{

}

static FlagStatus ddsis_com3_checkbusy(void)
{

}

void ddsis_com3_itconfig(u16 uart_it, FunctionalState state)
{
      
}

////////////////////////////////////////////////////////////////////////////////
static void ddsis_com4_sendbyte(u8 data)
{

}

static u8   ddsis_com4_recvbyte(void)
{

}

static FlagStatus ddsis_com4_checkbusy(void)
{

}

void ddsis_com4_itconfig(u16 uart_it, FunctionalState state)
{
      
}

////////////////////////////////////////////////////////////////////////////////
static void ddsis_com5_sendbyte(u8 data)
{

}

static u8   ddsis_com5_recvbyte(void)
{

}

static FlagStatus ddsis_com5_checkbusy(void)
{

}

void ddsis_com5_itconfig(u16 uart_it, FunctionalState state)
{
       
}

////////////////////////////////////////////////////////////////////////////////


static void ddsis_com6_sendbyte(u8 data)
{
    Chip_EXUART_SendByte(LPC_EXUART1, data);
}

static u8   ddsis_com6_recvbyte(void)
{
    return Chip_EXUART_ReadByte(LPC_EXUART1);
}

static FlagStatus ddsis_com6_checkbusy(void)
{
    return Chip_EXUART_CheckFIFOBusy(LPC_EXUART1);
}

void ddsis_com6_itconfig(u16 uart_it, FunctionalState state)
{
    if(state == ENABLE)
    {
        Chip_EXUART_IntEnable(LPC_EXUART1, EXUART_IER_THREINT);
    }
    else
    {
        Chip_EXUART_IntDisable(LPC_EXUART1, EXUART_IER_THREINT);
    }
}

////////////////////////////////////////////////////////////////////////////////

static void ddsis_com7_sendbyte(u8 data)
{
    Chip_EXUART_SendByte(LPC_EXUART2, data);
}

static u8   ddsis_com7_recvbyte(void)
{
    return Chip_EXUART_ReadByte(LPC_EXUART2);
}

static FlagStatus ddsis_com7_checkbusy(void)
{
    return Chip_EXUART_CheckFIFOBusy(LPC_EXUART2);
}

void ddsis_com7_itconfig(u16 uart_it, FunctionalState state)
{
    if(state == ENABLE)
    {
        Chip_EXUART_IntEnable(LPC_EXUART2, EXUART_IER_THREINT);
    }
    else
    {
        Chip_EXUART_IntDisable(LPC_EXUART2, EXUART_IER_THREINT);
    }    
}

////////////////////////////////////////////////////////////////////////////////
static void ddsis_com8_sendbyte(u8 data)
{

}

static u8   ddsis_com8_recvbyte(void)
{

}

static FlagStatus ddsis_com8_checkbusy(void)
{

}

void ddsis_com8_itconfig(u16 uart_it, FunctionalState state)
{
    
}

////////////////////////////////////////////////////////////////////////////////
static void ddsis_com9_sendbyte(u8 data)
{

}

static u8   ddsis_com9_recvbyte(void)
{

}

static FlagStatus ddsis_com9_checkbusy(void)
{

}

void ddsis_com9_itconfig(u16 uart_it, FunctionalState state)
{
   
}

////////////////////////////////////////////////////////////////////////////////

static ddsis_com_t ddsis_com[ddsis_com_num] =  
{
    {
        .ddsis_com_sendbyte     = ddsis_com1_sendbyte,
        .ddsis_com_recvbyte     = ddsis_com1_recvbyte,
        .ddsis_com_checkbusy    = ddsis_com1_checkbusy,
        .ddsis_com_itconfig     = ddsis_com1_itconfig,
    },
    {
        .ddsis_com_sendbyte     = ddsis_com2_sendbyte,
        .ddsis_com_recvbyte     = ddsis_com2_recvbyte,
        .ddsis_com_checkbusy    = ddsis_com2_checkbusy,
        .ddsis_com_itconfig     = ddsis_com2_itconfig,        
    },
    {
        .ddsis_com_sendbyte     = ddsis_com3_sendbyte,
        .ddsis_com_recvbyte     = ddsis_com3_recvbyte,
        .ddsis_com_checkbusy    = ddsis_com3_checkbusy,
        .ddsis_com_itconfig     = ddsis_com3_itconfig,        
    },
    {
        .ddsis_com_sendbyte     = ddsis_com4_sendbyte,
        .ddsis_com_recvbyte     = ddsis_com4_recvbyte,
        .ddsis_com_checkbusy    = ddsis_com4_checkbusy,
        .ddsis_com_itconfig     = ddsis_com4_itconfig,        
    },
    {
        .ddsis_com_sendbyte     = ddsis_com5_sendbyte,
        .ddsis_com_recvbyte     = ddsis_com5_recvbyte,
        .ddsis_com_checkbusy    = ddsis_com5_checkbusy,
        .ddsis_com_itconfig     = ddsis_com5_itconfig,        
    },
    {
        .ddsis_com_sendbyte     = ddsis_com6_sendbyte,
        .ddsis_com_recvbyte     = ddsis_com6_recvbyte,
        .ddsis_com_checkbusy    = ddsis_com6_checkbusy,
        .ddsis_com_itconfig     = ddsis_com6_itconfig,        
    },
    {
        .ddsis_com_sendbyte     = ddsis_com7_sendbyte,
        .ddsis_com_recvbyte     = ddsis_com7_recvbyte,
        .ddsis_com_checkbusy    = ddsis_com7_checkbusy,
        .ddsis_com_itconfig     = ddsis_com7_itconfig,        
    },
    {
        .ddsis_com_sendbyte     = ddsis_com8_sendbyte,
        .ddsis_com_recvbyte     = ddsis_com8_recvbyte,
        .ddsis_com_checkbusy    = ddsis_com8_checkbusy,
        .ddsis_com_itconfig     = ddsis_com8_itconfig,        
    },
    {
        .ddsis_com_sendbyte     = ddsis_com9_sendbyte,
        .ddsis_com_recvbyte     = ddsis_com9_recvbyte,
        .ddsis_com_checkbusy    = ddsis_com9_checkbusy,
        .ddsis_com_itconfig     = ddsis_com9_itconfig,        
    },    
};

//发送一个字节
void ddsis_uart_sendbyte(ddsis_comx uart, u8 data)
{
    ddsis_com[uart].ddsis_com_sendbyte(data);
}

//一次发送多个字节
void ddsis_uart_sendbytes(ddsis_comx uart, ddsis_uart_send_t* ddsis_send)
{
    u16 i = 0, tmp = 0;
    
    if(ddsis_send->send_mode != uart_send_mode_poll)
    {
        ddsis_uart_itconfig(uart, ddsis_it_thr, DISABLE);
    }
    
    if(ddsis_uart_checkbusy(uart) == ddsis_com_state_free)
    {
        if(ddsis_send->send_flag == uart_send_start)  
        {
            tmp = ddsis_send->send_sum_len - ddsis_send->send_cur_len;
            
            if(tmp > ddsis_send->send_FIFO_size)
            {
                for(i=0; i<ddsis_send->send_FIFO_size; i++) 
                {
                    ddsis_uart_sendbyte(uart, \
                        ddsis_send->send_buf[ddsis_send->send_cur_len++]);
                }    
                if(ddsis_send->send_mode != uart_send_mode_poll)
                {
                    ddsis_uart_itconfig(uart, ddsis_it_thr, ENABLE);
                }
            }
            else  
            {
                for(i=0; i<tmp; i++)
                {             
                    ddsis_uart_sendbyte(uart, \
                        ddsis_send->send_buf[ddsis_send->send_cur_len++]);
                }
                ddsis_send->send_flag = uart_send_finish;
                
                if(ddsis_send->send_mode != uart_send_mode_poll)
                {
                    ddsis_uart_itconfig(uart, ddsis_it_thr, DISABLE);      
                }
            }
        }
        else
        {
        }
    }
    else
    {
        if(ddsis_send->send_mode != uart_send_mode_poll)
        {
            ddsis_uart_itconfig(uart,  ddsis_it_thr,   ENABLE);
        }        
    }
}

//接收一个字节
u8 ddsis_uart_recvbyte(ddsis_comx uart)
{
    return ddsis_com[uart].ddsis_com_recvbyte();
}

//检查发送是否忙
ddsis_com_state_t ddsis_uart_checkbusy(ddsis_comx uart)
{
    return (ddsis_com_state_t)(ddsis_com[uart].ddsis_com_checkbusy());
}

//使能中断
void ddsis_uart_itconfig(ddsis_comx uart, u16 uart_it, FunctionalState state)
{
    ddsis_com[uart].ddsis_com_itconfig(uart_it, state);
}


