#include "bsp_led.h"
#include "../delay/bsp_delay.h"

void init_led(void)
{
    //  设置复用
    IOMUXC_SetPinMux(IOMUXC_GPIO1_IO03_GPIO1_IO03, 0);
    //  设置电气属性
    IOMUXC_SetPinConfig(IOMUXC_GPIO1_IO03_GPIO1_IO03, 0x10B0);

    //  设置IO方向
    GPIO1->GDIR = 0x08;
    //  打开led
    GPIO1->DR = 0;
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
