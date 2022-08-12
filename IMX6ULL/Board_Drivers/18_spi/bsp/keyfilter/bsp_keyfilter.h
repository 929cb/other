#pragma once

#include "../../imx6u/imx6u.h"

void keyfilter_init(void);
void filtertimer_init(unsigned int val);
void filtertimer_stop(void);
void filtertimer_restart(unsigned int val);
void filtertimer_irqhandler(unsigned int gicciar, void* param);
void gpio1_16_31_irqhandler(unsigned int gicciar, void* param);
