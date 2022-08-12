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
#include "../bsp/lcd/bsp_lcd.h"
#include "../bsp/lcd/bsp_lcdapi.h"
#include "../bsp/rtc/bsp_rtc.h"
#include "../bsp/ap3216c/bsp_ap3216c.h"
#include <stdio.h>


int main(void)
{
	//	
	unsigned short ir, ps, als;

	int_init(); 				/* 初始化中断(一定要最先调用！) */
	imx6u_clkinit();			/* 初始化系统时钟 			*/
	delay_init();				/* 初始化延时 			*/
	clk_enable();				/* 使能所有的时钟 			*/
	init_led();					/* 初始化led 			*/
	beep_init();				/* 初始化beep	 		*/
	uart_init();				/* 初始化串口，波特率115200 */
	//lcd_init();					/* 初始化LCD 			*/
	rtc_init(); 				/* 初始化RTC	 		*/
	ap3216c_init();
	
	unsigned char state = 0;
	while(1)					
	{	
		ap3216c_readreg(&ir, &ps, &als);
		printf("ir=%d, ps=%d, als=%d\r\n", ir, ps, als);

		state = !state;
		led_switch(state);
		delay_ms(1000);	/* 延时一秒 */
	}
	return 0;
}