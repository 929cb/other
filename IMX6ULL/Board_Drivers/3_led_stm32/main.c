#include "imx6u.h"


void clk_enable(void)
{
    //  使能所有时钟
    CCM->CCGR0 = 0xFFFFFFFF;
    CCM->CCGR1 = 0xFFFFFFFF;
    CCM->CCGR2 = 0xFFFFFFFF;
    CCM->CCGR3 = 0xFFFFFFFF;
    CCM->CCGR4 = 0xFFFFFFFF;
    CCM->CCGR5 = 0xFFFFFFFF;
    CCM->CCGR6 = 0xFFFFFFFF;
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
        GPIO1->DR &= ~(1 << 3);
        delay_ms(500);
        GPIO1->DR |= (1 << 3);
        delay_ms(500);
    }
}

void init_led(void)
{
    //  设置复用
    IOMUX_SW_MUX->GPIO1_IO03 = 0x05;
    //  设置电气属性
    IOMUX_SW_PAD->GPIO1_IO03 = 0x10B0;

    //  设置IO方向
    GPIO1->GDIR = 0x08;
    //  打开led
    GPIO1->DR = 0;
}

int main(void)
{
    clk_enable();
    init_led();

    led_toggle();

    while(1);

    return 0;
}