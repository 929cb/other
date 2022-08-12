#include "bsp_rtc.h"

void rtc_init(void)
{
    struct rtc_time rtcTime; 

    SNVS->HPCOMR |= (1 << 31) | (1 << 8);

    rtcTime.year = 2022;
    rtcTime.month = 8;
    rtcTime.day = 11;
    rtcTime.hour = 8;
    rtcTime.minute = 0;
    rtcTime.second = 0;
    rtc_settime(&rtcTime);

    rtc_enable();
}

void rtc_enable(void)
{
    SNVS->LPCR |= (1 << 0);
    while((SNVS->LPCR & 1) == 0);
}

void rtc_disable(void)
{
    SNVS->LPCR &= ~(1 << 0);
    while((SNVS->LPCR & 1) == 1);
}


/*
 * @description	: 判断指定年份是否为闰年，闰年条件如下:
 * @param - year: 要判断的年份
 * @return 		: 1 是闰年，0 不是闰年
 */
unsigned char rtc_isleapyear(unsigned short year)
{	
	unsigned char value=0;
	
	if(year % 400 == 0)
		value = 1;
	else 
	{
		if((year % 4 == 0) && (year % 100 != 0))
			value = 1;
		else 
			value = 0;
	}
	return value;
}

//  将日期时间转换为秒数
uint64_t rtc_date_to_second(struct rtc_time* rtcTime)
{
    unsigned short i = 0;
    unsigned int days = 0;
    unsigned int monthdays[] = {0U, 0U, 31U, 59U, 90U, 120U, 151U, 181U, 212U, 243U, 273U, 304U, 334U};

    for(i = 1970; i < rtcTime->year; i++)
    {
        days += DAYS_IN_YEAR;
        if(rtc_isleapyear(i))
            days++;
    }

    days += monthdays[rtcTime->month];
    if(rtc_isleapyear(i) && rtcTime->month >= 3)
        days++;

    days += rtcTime->day - 1;

    return (days * SECONDS_IN_DAY +
            rtcTime->hour * SECONDS_IN_HOUR +
            rtcTime->minute * SECONDS_IN_MINUTE +
            rtcTime->second);
}

void rtc_settime(struct rtc_time* rtcTime)
{
    uint64_t seconds = 0;
    unsigned int temp = SNVS->LPCR;

    rtc_disable();
    seconds = rtc_date_to_second(rtcTime);

    // SNVS->LPSRTCMR = (unsigned int)(seconds >> 32); // 高15位
    // SNVS->LPSRTCLR = (unsigned int)(seconds & 0xFFFFFFFF); // 低32位 

    SNVS->LPSRTCMR = (unsigned int)(seconds >> 17); // 高15位
    SNVS->LPSRTCLR = (unsigned int)(seconds << 15); // 低17位 

    if(temp & 1) 
        rtc_enable();
}

uint64_t rtc_getseconds(void)
{
    // return ((uint64_t)((uint64_t)SNVS->LPSRTCMR << 32) | (SNVS->LPSRTCLR));
    return (SNVS->LPSRTCMR << 17) | (SNVS->LPSRTCLR >> 15);
}


/*
 * @description		: 将秒数转换为时间
 * @param - seconds	: 要转换的秒数
 * @param - datetime: 转换后的日期和时间
 * @return 			: 无
 */
void rtc_convertseconds_to_datetime(u64 seconds, struct rtc_time *datetime)
{
    u64 x;
    u64  secondsRemaining, days;
    unsigned short daysInYear;

    /* 每个月的天数       */
    unsigned char daysPerMonth[] = {0U, 31U, 28U, 31U, 30U, 31U, 30U, 31U, 31U, 30U, 31U, 30U, 31U};

    secondsRemaining = seconds; /* 剩余秒数初始化 */
    days = secondsRemaining / SECONDS_IN_DAY + 1; 		/* 根据秒数计算天数,加1是当前天数 */
    secondsRemaining = secondsRemaining % SECONDS_IN_DAY; /*计算天数以后剩余的秒数 */

	/* 计算时、分、秒 */
    datetime->hour = secondsRemaining / SECONDS_IN_HOUR;
    secondsRemaining = secondsRemaining % SECONDS_IN_HOUR;
    datetime->minute = secondsRemaining / 60;
    datetime->second = secondsRemaining % SECONDS_IN_MINUTE;

    /* 计算年 */
    daysInYear = DAYS_IN_YEAR;
    datetime->year = YEAR_RANGE_START;
    while(days > daysInYear)
    {
        /* 根据天数计算年 */
        days -= daysInYear;
        datetime->year++;

        /* 处理闰年 */
        if (!rtc_isleapyear(datetime->year))
            daysInYear = DAYS_IN_YEAR;
        else	/*闰年，天数加一 */
            daysInYear = DAYS_IN_YEAR + 1;
    }
	/*根据剩余的天数计算月份 */
    if(rtc_isleapyear(datetime->year)) /* 如果是闰年的话2月加一天 */
        daysPerMonth[2] = 29;

    for(x = 1; x <= 12; x++)
    {
        if (days <= daysPerMonth[x])
        {
            datetime->month = x;
            break;
        }
        else
        {
            days -= daysPerMonth[x];
        }
    }

    datetime->day = days;

}

void rtc_gettime(struct rtc_time* rtcTime)
{
    uint64_t seconds = 0;

    seconds = rtc_getseconds();
    rtc_convertseconds_to_datetime(seconds, rtcTime);
}