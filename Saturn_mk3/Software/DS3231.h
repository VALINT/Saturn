/*------------------------------------------------------------------------------------------------------
 * Common RTC library
 *______________________________________________________________________________________________________
 *			__    __ __ __     __   _____	 ____  _____  ____ 
 *			\ \  / //  \\ \    \_\ / ___/	| |\ \|_   _|/  __\
 *			 \ \/ //    \\ \___ |/ \___ \	| |/ /  | |  | |__ 
 *			  \__//__/\__\\____\   /____/	|_|\_\  |_|  \____/
 *				  _     _  ____  ____    _    ____  _   _
 *				 | |   | || __ \| |\ \  / \  | |\ \\ \_/ /
 *				 | |__ | || __ || |/ / / _ \ | |/ / \   /
 *				 |____||_||____/|_|\_\/_/ \_\|_|\_\  |_|
 *_______________________________________________________________________________________________________
 *
 * Created: 19-Sep-2018 01:09:14
 *  Author: VAL
 *------------------------------------------------------------------------------------------------------- 
 *	This module consist program realization of communication interfaces.
 *	Using hardware interfaces (need for easy programing if you need using interface)
 *-------------------------------------------------------------------------------------------------------
 *	History:
 *		-	19-Sep-2018
 *			Start of writing this module
 *-------------------------------------------------------------------------------------------------------
 *	Features:
 *		-	Using hardware I2C interface (protocol) 
 *
 *-------------------------------------------------------------------------------------------------------
 */

#ifndef DS3231_H_
#define DS3231_H_

#include <stdint.h>
#include "standart_interface.h"

//------------------------------------------------------------------------------------------------------
//	Universal data structure

typedef struct rtc_data
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
	// Temperature
	uint8_t		wtemperature;
	uint8_t		ftemperature;	
} rtc_data;

#define RTC_WADDR 0xD0//0b11010000
#define RTC_RADDR 0xd1//0b11010001

uint8_t DS3231_init(void);
void DS3231_read_time(rtc_data *data);
void DS3231_read_date(rtc_data *data);
void DS3231_read_alarm(rtc_data *data);
void DS3231_write_time(rtc_data *data);
void DS3231_write_date(rtc_data *data);
void DS3231_read_temp(rtc_data *data);
void DS3231_write_temp(rtc_data *data);

#endif /* RTC_H_ */
