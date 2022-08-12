#include "bsp_uart.h"

//  uart1的io初始化
void uart_io_init(void)
{
    IOMUXC_SetPinMux(IOMUXC_UART1_TX_DATA_UART1_TX, 0);
    IOMUXC_SetPinMux(IOMUXC_UART1_RX_DATA_UART1_RX, 0);

    IOMUXC_SetPinConfig(IOMUXC_UART1_TX_DATA_UART1_TX, 0x10B0);
    IOMUXC_SetPinConfig(IOMUXC_UART1_RX_DATA_UART1_RX, 0x10B0);
}

void uart_disable(UART_Type* base)
{
    base->UCR1 &= ~(1 << 0);
}

void uart_enable(UART_Type* base)
{
    base->UCR1 |= (1 << 0);
}

void uart_softreset(UART_Type* base)
{
    base->UCR2 &= ~(1 << 0);
    while((base->UCR2 & 1) == 0);   //  等待复位完成
}

//  初始化
void uart_init(void)
{
    uart_io_init();  //串口IO初始化
   
    uart_disable(UART1); /* 先关闭 UART1 */
    uart_softreset(UART1); //软复位
    UART1->UCR1 = 0; /* 先清除 UCR1 寄存器 */
    UART1->UCR1 &= ~(1<<14); /* 关闭自动波特率检测 */

    UART1->UCR2 |= (1<<14) | (1<<5) | (1<<2) | (1<<1); //  设置 UART 的 UCR2 寄存器，设置字长，停止位，校验模式，关闭硬件流控
    UART1->UCR3 |= 1<<2; /* UCR3 的 bit2 必须为 1 */
/*
* 设置波特率
* 波特率计算公式:Baud Rate = Ref Freq / (16 * (UBMR + 1)/(UBIR+1)) 
* 如果要设置波特率为 115200，那么可以使用如下参数:
* Ref Freq = 80M 也就是寄存器 UFCR 的 bit9:7=101, 表示 1 分频
* UBMR = 3124
* UBIR = 71
* 因此波特率= 80000000/(16 * (3124+1)/(71+1))
* = 80000000/(16 * 3125/72) 
* = (80000000*72) / (16*3125) 
* = 115200
*/
    UART1->UFCR = 5<<7; /* ref freq 等于 ipg_clk/1=80Mhz */
    UART1->UBIR = 71;
    UART1->UBMR = 3124;

#if 0
uart_setbaudrate(UART1, 115200, 80000000); /* 设置波特率 */
#endif
 
    uart_enable(UART1); /* 使能串口 */
}


//  串口发送字符
void my_putc(unsigned char c)
{
    while(((UART1->USR2 >> 3) & 1) == 0);
    UART1->UTXD = c & 0xFF;
}

int fputc(int c, FILE* f)
{
    while(((UART1->USR2 >> 3) & 1) == 0);
    UART1->UTXD = c & 0xFF;

    return c;
}

//  串口接收
unsigned char my_getc(void)
{
    while((UART1->USR2 & 1) == 0);
    return UART1->URXD;
}

void my_puts(char* str)
{
    while(str && *str)
    {
        my_putc(*str++);
    }
}