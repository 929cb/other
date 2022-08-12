#include "main.h"


void clk_enable(void)
{
    //  使能所有时钟
    CCM_CCGR0 = 0xFFFFFFFF;
    CCM_CCGR1 = 0xFFFFFFFF;
    CCM_CCGR2 = 0xFFFFFFFF;
    CCM_CCGR3 = 0xFFFFFFFF;
    CCM_CCGR4 = 0xFFFFFFFF;
    CCM_CCGR5 = 0xFFFFFFFF;
    CCM_CCGR6 = 0xFFFFFFFF;
}

void delay_short(volatile unsigned int n)
{
    while(n--);
}

void delay_ms(volatile unsigned int n)
{
    while(n--)
        delay_short(0x7FF); //396MHz
}

void led_toggle(void)
{
    while(1)
    {
        GPIOx_DR &= ~(1 << 3);
        delay_ms(500);
        GPIOx_DR |= (1 << 3);
        delay_ms(500);
    }
}

void init_led(void)
{
    //  设置复用
    SW_MUX_GPIO1_IO03 = 0x05;
    //  设置电气属性
    SW_PAD_GPIO1_IO03 = 0x10B0;

    //  设置IO方向
    GPIOx_GDIR = 0x08;
    //  打开led
    GPIOx_DR = 0;
}

int main(void)
{
    clk_enable();
    init_led();

    led_toggle();

    while(1);

    return 0;
}