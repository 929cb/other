#include "../imx6u/imx6u.h"
#include "../bsp/clk/bsp_clk.h"
#include "../bsp/led/bsp_led.h"
#include "../bsp/beep/bsp_beep.h"
#include "../bsp/delay/bsp_delay.h"
#include "../bsp/key/bsp_key.h"
#include "../bsp/int/bsp_int.h"
#include "../bsp/exti/bsp_exti.h"
#include "../bsp/epit/bsp_epit.h"
#include "../bsp/keyfilter/bsp_keyfilter.h"
#include "../bsp/uart/bsp_uart.h"
#include <stdio.h>

int main(void)
{
    int_init();
    imx6u_clkinit();
    delay_init();
    uart_init();
    clk_enable();
    init_led();
    beep_init();
    key_init();
    exti_init();
    epit1_init(0, 66000000 / 2);    // 500ms
    keyfilter_init();

    unsigned char c;
    while(1)
    {
        my_puts("please input a char:");
        printf("nihao:::");
        c = my_getc();
        if(c == '\n') continue;
        my_putc(c ^ 32);
        my_puts("\r\n");
    }

    return 0;
}