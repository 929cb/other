#pragma once

#include "../../imx6u/imx6u.h"

void exti_init(void);
void gpio1_io18_irqhandler(unsigned int gicciar, void* param);
