#include "../imx6u/fsl_iomuxc.h"
#include "../imx6u/MCIMX6Y2.h"
#include "../bsp/clk/bsp_clk.h"
#include "../bsp/led/bsp_led.h"
#include "../bsp/beep/bsp_beep.h"
#include "../bsp/delay/bsp_delay.h"
#include "../bsp/key/bsp_key.h"


int main(void)
{
    clk_enable();
    init_led();
    beep_init();
    key_init();

    //led_toggle();

    
    while(1)
    {
        if(key_getVal() == KEY_ON)
        {
            beep_switch(ON);

        }
        //delay_ms(500);
        else beep_switch(OFF);
        //delay_ms(1000);
    }

    return 0;
}