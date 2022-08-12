#pragma once

#include "../../imx6u/imx6u.h"

#define SECONDS_IN_MINUTE   (60)
#define SECONDS_IN_HOUR     (60 * SECONDS_IN_MINUTE)
#define SECONDS_IN_DAY      (24 * SECONDS_IN_HOUR)
#define DAYS_IN_YEAR        (365)
#define YEAR_RANGE_START    (1970)
#define YEAR_RANGE_END      (2099)


//  时间相关结构体
struct rtc_time
{
    unsigned short year;
    unsigned char month;
    unsigned char day;
    unsigned char hour;
    unsigned char minute;
    unsigned char second;
};


void rtc_init(void);
void rtc_enable(void);
void rtc_disable(void);
unsigned char rtc_isleapyear(unsigned short year);
uint64_t rtc_date_to_second(struct rtc_time* rtcTime);
void rtc_settime(struct rtc_time* rtcTime);
uint64_t rtc_getseconds(void);
void rtc_convertseconds_to_datetime(u64 seconds, struct rtc_time *datetime);
void rtc_gettime(struct rtc_time* rtcTime);
