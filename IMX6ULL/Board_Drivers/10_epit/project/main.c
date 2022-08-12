#include "../imx6u/imx6u.h"
#include "../bsp/clk/bsp_clk.h"
#include "../bsp/led/bsp_led.h"
#include "../bsp/beep/bsp_beep.h"
#include "../bsp/delay/bsp_delay.h"
#include "../bsp/key/bsp_key.h"
#include "../bsp/int/bsp_int.h"
#include "../bsp/exti/bsp_exti.h"
#include "../bsp/epit/bsp_epit.h"


int main(void)
{
    int_init();
    imx6u_clkinit();
    clk_enable();
    init_led();
    beep_init();
    key_init();
    exti_init();
    epit1_init(0, 66000000 / 2);    // 500ms

    //led_toggle();

    
    while(1)
    {
        
    }

    return 0;
}