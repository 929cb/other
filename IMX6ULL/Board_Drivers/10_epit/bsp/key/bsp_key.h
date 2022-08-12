#pragma once

#include "../../imx6u/fsl_iomuxc.h"
#include "../../imx6u/MCIMX6Y2.h"

enum keyType
{
    KEY_NONE = 0,
    KEY_ON
};

void key_init(void);
char key_scan(void);
char key_getVal(void);
