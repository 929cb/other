#include "bsp_keyfilter.h"
#include "../gpio/bsp_gpio.h"
#include "../int/bsp_int.h"
#include "../beep/bsp_beep.h"

void keyfilter_init(void)
{
    gpio_pin_config_t key_config;
    IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0);
    IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0xF080);

    key_config.direction = GPIO_DigitalInput;
    key_config.interruptMode = GPIO_IntFallingEdge;
    gpio_init(GPIO1, 18, key_config);

    GIC_EnableIRQ(GPIO1_Combined_16_31_IRQn);
    system_regiter_irqhandler(GPIO1_Combined_16_31_IRQn, gpio1_16_31_irqhandler, NULL);
    gpio_enableint(GPIO1, 18);

    //  初始化定时器
    filtertimer_init(66000000 / 100);
}

void filtertimer_init(unsigned int val)
{
    EPIT1->CR = 0;
    EPIT1->CR = (1 << 1) | (1 << 2) | (1 << 3) | (1 << 24);

    EPIT1->LR = val;
    EPIT1->CMPR = 0;

    GIC_EnableIRQ(EPIT1_IRQn);
    system_regiter_irqhandler(EPIT1_IRQn, filtertimer_irqhandler, NULL);
}

//  关闭epit1定时器
void filtertimer_stop(void)
{
    EPIT1->CR &= ~(1 << 0);
}

//  重启epit1定时器
void filtertimer_restart(unsigned int val)
{
    EPIT1->CR &= ~(1 << 0);
    EPIT1->LR = val;
    EPIT1->CR |= (1 << 0);
}
void filtertimer_irqhandler(unsigned int gicciar, void* param)
{
    static unsigned char state = 0;

    if(EPIT1->SR & (1 << 0))
    {
        filtertimer_stop();
        if(gpio_readPin(GPIO1, 18) == 0)
        {
            state = !state;
            beep_switch(state);
        }
    }

    EPIT1->SR |= 1 << 0;
}

//  按键中断服务函数
void gpio1_16_31_irqhandler(unsigned int gicciar, void* param)
{
    filtertimer_restart(66000000 / 100);

    gpio_clearintflag(GPIO1, 18);
}