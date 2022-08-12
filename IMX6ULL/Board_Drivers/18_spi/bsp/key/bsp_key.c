#include "bsp_key.h"
#include "../delay/bsp_delay.h"

void key_init(void)
{
    //  设置复用
    IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0);
    //  设置电气属性
    IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0xF080);
    //  设置为输入
    GPIO1->GDIR &= ~(1 << 18);
}

char key_scan(void)
{
    char ret = 0;

    ret = (GPIO1->DR >> 18) & 1;

    return ret;
}

char key_getVal(void)
{
    char ret = 0, status = 0;
    static unsigned char release = 1;   //  按键释放

    status = key_scan();

    if(release && status == 0)
    {
        delay_ms(15);
        if(ret == 0)
        {
            ret = KEY_ON;
            release = 0; 
        }
    }
    else if(status == 1)
    {
        ret = KEY_NONE;
        release = 1;
    }
    else if(status == 0)
        ret = KEY_ON;

    return ret;
}