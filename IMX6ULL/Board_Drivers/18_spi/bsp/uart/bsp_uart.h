#pragma once

#include "../../imx6u/imx6u.h"
#include <stdio.h>

void uart_init(void);
void my_putc(unsigned char c);
unsigned char my_getc(void);
void my_puts(char* str);
