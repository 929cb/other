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
#include <stdio.h>


/* 背景颜色索引 */
unsigned int backcolor[10] = {
	LCD_BLUE, 		LCD_GREEN, 		LCD_RED, 	LCD_CYAN, 	LCD_YELLOW, 
	LCD_LIGHTBLUE, 	LCD_DARKBLUE, 	LCD_WHITE, 	LCD_BLACK, 	LCD_ORANGE

}; 

int main(void)
{
    int_init();
    imx6u_clkinit();
    delay_init();
    uart_init();
    clk_enable();
    lcd_init();


    unsigned char index = 0;
    tftlcd_dev.forecolor = LCD_RED;
    while(1)
    {
        lcd_clear(backcolor[index]);
		delay_ms(1); 	  
		lcd_show_string(10, 40, 260, 32, 32,(char*)"ALPHA IMX6U"); 	
		lcd_show_string(10, 80, 240, 24, 24,(char*)"RGBLCD TEST");
		lcd_show_string(10, 110, 240, 16, 16,(char*)"ATOM@ALIENTEK");      					 
		lcd_show_string(10, 130, 240, 12, 12,(char*)"2019/8/14");	      					 
	    index++;
		if(index == 10)
			index = 0;
        delay_ms(1000);
    }

    return 0;
}