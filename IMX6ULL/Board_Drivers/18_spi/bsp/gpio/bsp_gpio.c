#include "bsp_gpio.h"

void gpio_init(GPIO_Type* base, int pin, gpio_pin_config_t config)
{
    if(config.direction == GPIO_DigitalInput)
    {
        base->GDIR &= ~(1 << pin); 
        
        gpio_intconfig(base, pin, config.interruptMode);
    }
    else
    {
        base->GDIR |= (1 << pin);
        gpio_writePin(base, pin, config.outputLogic);
    }

}

void gpio_writePin(GPIO_Type* base, int pin, char val)
{
    if(!val)
    {
        base->DR &= ~(1 << pin);
    }
    else
    {
        base->DR |= (1 << pin);
    }
}

char gpio_readPin(GPIO_Type* base, int pin)
{
    return ((base->DR) >> pin) & 1;
}


//  使能指定IO中断
void gpio_enableint(GPIO_Type* base, unsigned int pin)
{
    base->IMR |= (1 << pin);
}
//  失能指定IO中断
void gpio_disableint(GPIO_Type* base, unsigned int pin)
{
    base->IMR &= ~(1 << pin);
}

//  清除中断标志位
void gpio_clearintflag(GPIO_Type* base, unsigned int pin)
{
    base->ISR |= (1 << pin);
}

//  GPIO中断初始化函数
void gpio_intconfig(GPIO_Type* base, unsigned int pin, gpio_interrupt_mode_t mode)
{
    volatile uint32_t* icr;
    uint32_t icrShift;

    icrShift = pin;
    base->EDGE_SEL &= ~(1 << pin);  //  选择不忽略该引脚上ICR寄存器设置

    if(pin < 16)
        icr = &(base->ICR1);
    else
    {
        icr = &(base->ICR2);
        icrShift -= 16;
    }

    switch (mode)
    {
    case GPIO_IntLowLevel:
        *icr &= ~(3 << (2 * icrShift));
        break;
    case GPIO_IntHighLevel:
        *icr &= ~(3 << (2 * icrShift));
        *icr |= (1 << (2 * icrShift));
        break;
    case GPIO_IntRisingEdge:
        *icr &= ~(3 << (2 * icrShift));
        *icr |= (2 << (2 * icrShift));
        break;
    case GPIO_IntFallingEdge:
        *icr &= ~(3 << (2 * icrShift));
        *icr |= (3 << (2 * icrShift));
        break;
    case GPIO_IntRisingOrFallingEdge:   
        base->EDGE_SEL |= (1 << pin);
        break;
    default:
        break;
    }
}