#include "bsp_gpio.h"

void gpio_init(GPIO_Type* base, int pin, gpio_pin_config_t config)
{
    if(config.direction == GPIO_DigitalInput)
    {
        base->GDIR &= ~(1 << pin); 
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