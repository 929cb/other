#include "bsp_epit.h"
#include "../int/bsp_int.h"
#include "../led/bsp_led.h"

//初始化EPIT1
void epit1_init(unsigned int frac, unsigned int val)
{
    if(frac > 4095)
        frac = 4095;

    EPIT1->CR = 0;
    EPIT1->CR = (1 << 1) | (1 << 2) | (1 << 3) | (frac << 4) | (1 << 24);

    EPIT1->LR = val;

    EPIT1->CMPR = 0;

    GIC_EnableIRQ(EPIT1_IRQn);
    system_regiter_irqhandler(EPIT1_IRQn, epit1_irqhandler, NULL);
    EPIT1->CR |= (1 << 0);
}

void epit1_irqhandler(unsigned int gicciar, void* param)
{
    static unsigned char state = 0;

    if(EPIT1->SR & (1 << 0))
    {
        state = !state;
        led_switch(state);
    }

    //  清除中断标志位
    EPIT1->SR |= (1 << 0);
}