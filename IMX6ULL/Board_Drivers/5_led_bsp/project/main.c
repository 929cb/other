#include "../imx6u/fsl_iomuxc.h"
#include "../imx6u/MCIMX6Y2.h"
#include "../bsp/clk/bsp_clk.h"
#include "../bsp/led/bsp_led.h"


int main(void)
{
    clk_enable();
    init_led();

    led_toggle();

    while(1);

    return 0;
}