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
#include <stdio.h>


/* 背景颜色索引 */
unsigned int backcolor[10] = {
	LCD_BLUE, 		LCD_GREEN, 		LCD_RED, 	LCD_CYAN, 	LCD_YELLOW, 
	LCD_LIGHTBLUE, 	LCD_DARKBLUE, 	LCD_WHITE, 	LCD_BLACK, 	LCD_ORANGE

}; 

int main(void)
{
unsigned char key = 0;
	int i = 3, t = 0;
	char buf[160];
	struct rtc_time rtcdate;
	unsigned char state = OFF;


	int_init(); 				/* 初始化中断(一定要最先调用！) */
	imx6u_clkinit();			/* 初始化系统时钟 			*/
	delay_init();				/* 初始化延时 			*/
	clk_enable();				/* 使能所有的时钟 			*/
	init_led();					/* 初始化led 			*/
	beep_init();				/* 初始化beep	 		*/
	uart_init();				/* 初始化串口，波特率115200 */
	lcd_init();					/* 初始化LCD 			*/
	rtc_init(); 				/* 初始化RTC	 		*/

	tftlcd_dev.forecolor = LCD_RED;
	lcd_show_string(50, 10, 400, 24, 24, (char*)"ALPHA-IMX6UL RTC TEST");    /* 显示字符串 */
	lcd_show_string(50, 40, 200, 16, 16, (char*)"ATOM@ALIENTEK");  
	lcd_show_string(50, 60, 200, 16, 16, (char*)"2019/3/27");  
	tftlcd_dev.forecolor = LCD_BLUE;
	memset(buf, 0, sizeof(buf));
	
	while(1)
	{
		if(t==100)	//1s时间到了
		{
			t=0;
			printf("will be running %d s......\r", i);
			
			lcd_fill(50, 90, 370, 110, tftlcd_dev.backcolor); /* 清屏 */
			sprintf(buf, "will be running %ds......", i);
			lcd_show_string(50, 90, 300, 16, 16, buf); 
			i--;
			if(i < 0)
				break;
		}

		key = key_scan();
		if(key == ON)
		{
			rtcdate.year = 2018;
   			rtcdate.month = 1;
    		rtcdate.day = 15;
    		rtcdate.hour = 16;
    		rtcdate.minute = 23;
    		rtcdate.second = 0;
			rtc_settime(&rtcdate); /* 初始化时间和日期 */
			printf("\r\n RTC Init finish\r\n");
			break;
		}
			
		delay_ms(10);
		t++;
	}
	tftlcd_dev.forecolor = LCD_RED;
	lcd_fill(50, 90, 370, 110, tftlcd_dev.backcolor); /* 清屏 */
	lcd_show_string(50, 90, 200, 16, 16, (char*)"Current Time:");  			/* 显示字符串 */
	tftlcd_dev.forecolor = LCD_BLUE;

	while(1)					
	{	
		rtc_gettime(&rtcdate);
		sprintf(buf,"%d/%d/%d %d:%d:%d",rtcdate.year, rtcdate.month, rtcdate.day, rtcdate.hour, rtcdate.minute, rtcdate.second);
		lcd_fill(50,110, 300,130, tftlcd_dev.backcolor);
		lcd_show_string(50, 110, 250, 16, 16,(char*)buf);  /* 显示字符串 */
		
		state = !state;
		led_switch(state);
		delay_ms(1000);	/* 延时一秒 */
	}
	return 0;
}