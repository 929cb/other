#pragma once

#include "../../imx6u/fsl_iomuxc.h"
#include "../../imx6u/MCIMX6Y2.h"

#define ON 0
#define OFF 1

void beep_init(void);
void beep_switch(const char flag);
