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


int main(void)
{
    int_init();
    imx6u_clkinit();
    delay_init();
    clk_enable();
    init_led();
    beep_init();
    key_init();
    exti_init();
    epit1_init(0, 66000000 / 2);    // 500ms
    keyfilter_init();

    //led_toggle();

    unsigned char state = 0;
    while(1)
    {
        led_switch(state);
        state = !state;
        delay_ms(500);
    }

    return 0;
}