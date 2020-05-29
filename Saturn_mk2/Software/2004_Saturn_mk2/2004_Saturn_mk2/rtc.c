/*------------------------------------------------------------------------------------------------------
 * Common RTC library
 *______________________________________________________________________________________________________
 *			__    __ __ __     __   _____	 ____  _____  ____ 
 *			\ \  / //  \\ \    \_\ / ___/	| |\ \|_   _|/  __\
 *			 \ \/ //    \\ \___ |/ \___ \	| |/ /  | |  | |__ 
 *			  \__//__/\__\\____\   /____/	|_|\_\  |_|  \____/
 *			      _     _  ____  ____    _    ____  _   _
 *				 | |   | || __ \| |\ \  / \  | |\ \\ \_/ /
 *				 | |__ | || __ || |/ / / _ \ | |/ / \   /
 *				 |____||_||____/|_|\_\/_/ \_\|_|\_\  |_|
 *_______________________________________________________________________________________________________
 *
 * Created: 19-Sep-2018 01:09:14
 *  Author: VAL
 *-------------------------------------------------------------------------------------------------------
 */ 

#include "rtc.h"

uint8_t bcd2bin(uint8_t number)
{
	uint8_t temp = number;
	number &= 0x0f;
	temp >>= 4;
	return(10*temp+number);
}

uint8_t bin2bcd(uint8_t number)
{
	uint8_t temp = number%10;
	number /= 10;
	return((number << 4)+temp);
}

uint8_t RTC_init(void)
{
	_i2c_start_operation();
	_i2c_send_byte(RTC_WADDR);
	_i2c_send_byte(0x0E);
	_i2c_send_byte(0x20);
	_i2c_send_byte(0x08);
	_i2c_stop_operation();
	
	return(0);
}

void RTC_read_time(rtc_data *data)
{
	_i2c_start_operation();
	_i2c_send_byte(RTC_WADDR);
	_i2c_send_byte(0x00);
	_i2c_stop_operation();

	_i2c_start_operation();
	_i2c_send_byte(RTC_RADDR);
	data->sec  = bcd2bin(_i2c_reseive_byte());
	data->min  = bcd2bin(_i2c_reseive_byte());
	data->hour = bcd2bin(_i2c_reseive_last_byte());
	_i2c_stop_operation();	
}

void RTC_read_date(rtc_data *data)
{
	asm("cli");
	_i2c_start_operation();
	_i2c_send_byte(RTC_WADDR);
	_i2c_send_byte(0x03);
	_i2c_stop_operation();

	_i2c_start_operation();
	_i2c_send_byte(RTC_RADDR);
	data->wday	= bcd2bin(_i2c_reseive_byte());
	data->day	= bcd2bin(_i2c_reseive_byte());
	data->month = bcd2bin(0x7F & _i2c_reseive_byte());
	data->year	= bcd2bin(_i2c_reseive_last_byte());
	_i2c_stop_operation();
	asm("sei");
}

void RTC_read_alarm(rtc_data *data)
{
	asm("cli");
	_i2c_start_operation();
	_i2c_send_byte(RTC_WADDR);
	_i2c_send_byte(0x08);
	_i2c_stop_operation();

	_i2c_start_operation();
	_i2c_send_byte(RTC_RADDR);
	data->al1min	= bcd2bin(_i2c_reseive_byte());
	data->al1hour	= bcd2bin(_i2c_reseive_last_byte());
	_i2c_stop_operation();
	
	_i2c_start_operation();
	_i2c_send_byte(RTC_WADDR);
	_i2c_send_byte(0x0B);
	_i2c_stop_operation();

	_i2c_start_operation();
	_i2c_send_byte(RTC_RADDR);
	data->al2min	= bcd2bin(_i2c_reseive_byte());
	data->al2hour	= bcd2bin(_i2c_reseive_last_byte());
	_i2c_stop_operation();
	asm("sei");
}

void RTC_write_time(rtc_data *data)
{
	asm("cli");
	_i2c_start_operation();
	_i2c_send_byte(RTC_WADDR);
	_i2c_send_byte(0x00);
	_i2c_send_byte(bin2bcd(data->sec));
	_i2c_send_byte(bin2bcd(data->min));
	_i2c_send_byte(bin2bcd(data->hour));

	_i2c_stop_operation();
	asm("sei");
}

void RTC_write_date(rtc_data *data)
{
	asm("cli");
	_i2c_start_operation();
	_i2c_send_byte(RTC_WADDR);
	_i2c_send_byte(0x03);
	_i2c_send_byte(bin2bcd(data->wday));
	_i2c_send_byte(bin2bcd(data->day));
	_i2c_send_byte(0x80 | bin2bcd(data->month));
	_i2c_send_byte(bin2bcd(data->year));

	_i2c_stop_operation();
	asm("sei");
}

void RTC_write_alarm(rtc_data *data)
{
	asm("cli");
	_i2c_start_operation();
	_i2c_send_byte(RTC_WADDR);
	_i2c_send_byte(0x08);
	_i2c_send_byte(bin2bcd(data->al1min));
	_i2c_send_byte(bin2bcd(data->al1hour));
	_i2c_stop_operation();
	
	_i2c_start_operation();
	_i2c_send_byte(RTC_WADDR);
	_i2c_send_byte(0x0b);
	_i2c_send_byte(bin2bcd(data->al2min));
	_i2c_send_byte(bin2bcd(data->al2hour));
	_i2c_stop_operation();
	
	asm("sei");
}
	
void RTC_read_temp(rtc_data *data)
{
	_i2c_start_operation();
	_i2c_send_byte(RTC_WADDR);
	_i2c_send_byte(0x11);
	_i2c_stop_operation();

	_i2c_start_operation();
	_i2c_send_byte(RTC_RADDR);
	data->wtemperature  = ( _i2c_reseive_byte());
	data->ftemperature  = ((_i2c_reseive_last_byte()) >> 6)*25;
	_i2c_stop_operation();
}