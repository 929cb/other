#pragma once

#include "../../imx6u/imx6u.h"

void spi_init(ECSPI_Type* base);
unsigned char spich0_readwrite_byte(ECSPI_Type* base, unsigned char txdata);
