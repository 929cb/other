#pragma once

#include "../../imx6u/fsl_iomuxc.h"
#include "../../imx6u/MCIMX6Y2.h"


typedef enum _gpio_pin_direction
{
    GPIO_DigitalInput = 0U,
    GPIO_DigitalOutput
};

typedef struct _gpio_pin_config
{
    _gpio_pin_direction direction;
    uint8_t outputLogic;
}gpio_pin_config_t;


void gpio_init(GPIO_Type* base, int pin, gpio_pin_config_t config);
void gpio_writePin(GPIO_Type* base, int pin, char val);
char gpio_readPin(GPIO_Type* base, int pin);
