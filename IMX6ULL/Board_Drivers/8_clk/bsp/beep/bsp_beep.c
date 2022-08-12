#include "bsp_beep.h"

void beep_init(void)
{
    //  设置端口复用
    IOMUXC_SetPinMux(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01, 0);
    //  设置电气属性
    IOMUXC_SetPinConfig(IOMUXC_SNVS_SNVS_TAMPER1_GPIO5_IO01, 0x10B0);
    //  设置为输出
    GPIO5->GDIR |= (1 << 1);
    //  默认关闭
    GPIO5->DR |= (1 << 1);
}

void beep_switch(const char flag)
{
    if(flag == ON)
    {
        GPIO5->DR &= ~(1 << 1); 
    }
    else if(flag == OFF)
    {
        GPIO5->DR |= (1 << 1);
    }
}