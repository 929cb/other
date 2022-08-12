#include "bsp_exti.h"
#include "../gpio/bsp_gpio.h"
#include "../int/bsp_int.h"
#include "../delay/bsp_delay.h"
#include "../beep/bsp_beep.h"

//  初始化外部中断，GPIO1_IO18
void exti_init(void)
{
    gpio_pin_config_t key_config;

    IOMUXC_SetPinMux(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0);
    IOMUXC_SetPinConfig(IOMUXC_UART1_CTS_B_GPIO1_IO18, 0xF080);

    //  GPIO初始化
    key_config.direction = GPIO_DigitalInput;
    key_config.interruptMode = GPIO_IntFallingEdge;
    gpio_init(GPIO1, 18, key_config);

    //  使能16～31的irq中断
    GIC_EnableIRQ(GPIO1_Combined_16_31_IRQn);

    //  注册中断
    system_regiter_irqhandler(GPIO1_Combined_16_31_IRQn, gpio1_io18_irqhandler, NULL);

    //  使能gpio1_18的中断
    gpio_enableint(GPIO1, 18);
}

//  中断处理函数
void gpio1_io18_irqhandler(unsigned int gicciar, void* param)
{
    static unsigned char state = 0;

    delay_ms(10);

    if(gpio_readPin(GPIO1, 18) == 0)
    {
        state = !state;
        beep_switch(state);
    }

    //  清除中断标志
    gpio_clearintflag(GPIO1, 18);
}