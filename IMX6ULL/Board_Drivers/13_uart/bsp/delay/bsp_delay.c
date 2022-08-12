#include "bsp_delay.h"
#include "../int/bsp_int.h"
#include "../led/bsp_led.h"
#include "../beep/bsp_beep.h"

void delay_short(volatile unsigned int n)
{
    while(n--);
}

void delay_ms2(volatile unsigned int n)
{
    while(n--)
        delay_short(0x7FF); //396MHz
}


//  延时初始化
void delay_init(void)
{
    GPT1->CR = 0;

    GPT1->CR = 1 << 15;
    while((GPT1->CR >> 15) & 1);

    //  设置时钟源为ipg_clk=66MHz，restart模式
    GPT1->CR |= (1 << 1) | (1 << 6);
    GPT1->PR = 65;  //  66MHz/66=1MHz

    GPT1->OCR[0] = 0xFFFFFFFF;

#if 0
    GPT1->OCR[0] = 1000000 / 2; //  输出比较通道1，周期500ms

    GPT1->IR = 1 << 0;  //  使能输出比较通道1中断

    GIC_EnableIRQ(GPT1_IRQn);
    system_regiter_irqhandler(GPT1_IRQn, gpt1_irqhandler, NULL);

#endif
    //使能gpt1
    GPT1->CR |= 1 << 0;
}

void delay_us(unsigned int us)
{
    unsigned long oldcnt, newcnt;
    unsigned long tcntvalue = 0;

    oldcnt = GPT1->CNT;

    while(1)
    {
        newcnt = GPT1->CNT;

        if(newcnt != oldcnt)
        {
            if(newcnt > oldcnt)
                tcntvalue += newcnt - oldcnt;
            else
                tcntvalue += 0xFFFFFFFF - oldcnt + newcnt;

            oldcnt = newcnt;
            if(tcntvalue >= us)
                break;
        }
    }
}

void delay_ms(unsigned int ms)
{
    while(ms--)
    {
        delay_us(1000);
    }
}

#if 0
void gpt1_irqhandler(unsigned int gicciar, void* param)
{
    static unsigned char state = 0;

    if(GPT1->SR & (1 << 0))
    {
        state = !state;
        led_switch(state);
        beep_switch(state);
    }

    GPT1->SR |= 1 << 0;
}
#endif