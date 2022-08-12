#include "bsp_delay.h"

void delay_short(volatile unsigned int n)
{
    while(n--);
}

void delay_ms(volatile unsigned int n)
{
    while(n--)
        delay_short(0x7FF); //396MHz
}