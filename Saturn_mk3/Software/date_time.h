#ifndef _DATE_TIME_H_
#define _DATE_TIME_H_

#include "stdint.h"
#include "rtc.h"

#define SEC_PER_MIN 	60
#define SEC_PER_HOUR 	3600
#define SEC_PER_DAY		86400UL
#define SEC_PER_NYEAR	31536000UL

#define USE_UNIX_TIME

typedef struct
{
	//Time
	uint8_t 	second;	
	uint8_t 	minute;
	uint8_t 	hour;
	uint8_t 	time_mode;
	//Alarm1
	uint8_t 	al1_second;
	uint8_t 	al1_minute;
	uint8_t 	al1_hour;
	uint8_t 	al1_active;
	
	//Alarm2
	uint8_t 	al2_second;	
	uint8_t 	al2_minute;
	uint8_t 	al2_hour;
	uint8_t 	al2_active;
	
	//Date
	uint8_t 	w_day;
	uint8_t 	day;
	uint8_t 	month;
	uint8_t 	year;
} date_time;

void 		convert_from_Unix_time(uint32_t second, date_time* out_data);
uint32_t	convert_to_Unix_time(date_time* data);

void 		decode_date_time(uint32_t second, date_time* data);
void 		decode_from_seconds(uint32_t second, date_time* data);
uint32_t 	encode_seconds(date_time* data);
void 		start_clock(uint32_t second);
void 		write_time(date_time* data);

#endif
