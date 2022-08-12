#pragma once

#include "../../imx6u/fsl_iomuxc.h"
#include "../../imx6u/MCIMX6Y2.h"


//  描述中断触发类型
typedef enum _gpio_interrupt
{
    GPIO_NoIntMode = 0U,            // 无中断功能
    GPIO_IntLowLevel = 1U,          // 低电平触发
    GPIO_IntHighLevel = 2U,         // 高电平触发
    GPIO_IntRisingEdge = 3U,        // 上升沿触发
    GPIO_IntFallingEdge = 4U,       // 下降沿触发
    GPIO_IntRisingOrFallingEdge = 5U// 上升/下降沿触发
}gpio_interrupt_mode_t;

enum _gpio_pin_direction
{
    GPIO_DigitalInput = 0U,
    GPIO_DigitalOutput
};

typedef struct _gpio_pin_config
{
    enum _gpio_pin_direction direction;
    uint8_t outputLogic;
    gpio_interrupt_mode_t interruptMode;
}gpio_pin_config_t;


void gpio_init(GPIO_Type* base, int pin, gpio_pin_config_t config);
void gpio_writePin(GPIO_Type* base, int pin, char val);
char gpio_readPin(GPIO_Type* base, int pin);

void gpio_enableint(GPIO_Type* base, unsigned int pin);
void gpio_disableint(GPIO_Type* base, unsigned int pin);
void gpio_clearintflag(GPIO_Type* base, unsigned int pin);
void gpio_intconfig(GPIO_Type* base, unsigned int pin, gpio_interrupt_mode_t mode);
