#pragma once

#include "../../imx6u/imx6u.h"

void delay_ms2(volatile unsigned int n);
void delay_init(void);
void gpt1_irqhandler(unsigned int gicciar, void* param);
void delay_us(unsigned int us);
void delay_ms(unsigned int ms);
