#pragma once

#include "../../imx6u/imx6u.h"


void epit1_init(unsigned int frac, unsigned int val);
void epit1_irqhandler(unsigned int gicciar, void* param);
