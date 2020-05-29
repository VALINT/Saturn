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

#ifndef RTC_H_
#define RTC_H_

#include <stdint.h>
#include "standart_interface.h"

//Chose RTC IC
#define DS3231
//#define DS1307

//------------------------------------------------------------------------------------------------------
//	Universal data structure

typedef struct rtc_data
{
	// Time
	uint8_t		sec;
	uint8_t		min;
	uint8_t		hour;
	// Alarm 1
	uint8_t		al1sec;
	uint8_t		al1min;
	uint8_t		al1hour;
	// Alarm 2
	uint8_t		al2sec;
	uint8_t		al2min;
	uint8_t		al2hour;	
	// Date
	uint8_t		wday;
	uint8_t		day;
	uint8_t		month;
	uint8_t		year;
	// Temperature
	uint8_t		wtemperature;
	uint8_t		ftemperature;	
} rtc_data;

#ifdef DS3231
	
	#define RTC_WADDR 0b11010000
	#define RTC_RADDR 0b11010001

	uint8_t RTC_init(void);
	
	void RTC_read_time(rtc_data *data);
	
	void RTC_read_date(rtc_data *data);
	
	void RTC_read_alarm(rtc_data *data);
		
	void RTC_write_time(rtc_data *data);
	
	void RTC_write_date(rtc_data *data);
	
	void RTC_read_temp(rtc_data *data);
	
	void RTC_write_temp(rtc_data *data);
#endif



#endif /* RTC_H_ */