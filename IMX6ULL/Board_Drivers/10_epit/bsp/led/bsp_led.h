#pragma once

#include "../../imx6u/fsl_iomuxc.h"
#include "../../imx6u/MCIMX6Y2.h"

void init_led(void);
void led_toggle(void);
void led_switch(const char state);
