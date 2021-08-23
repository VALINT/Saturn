#include "date_time.h"

static const uint8_t month_days [12] =
	{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
//   Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec

void convert_from_Unix_time(uint32_t second, date_time* out_data)
{
	unsigned long a;
	char b;
	char c;
	char d;
	unsigned long time;

	time = second%SEC_PER_DAY;
	a = ((second+43200)/(SEC_PER_DAY>>1)) + (2440587<<1) + 1;
	a>>=1;
	out_data->w_day = 1+a%7;
	a+=32044;
	b=(4*a+3)/146097;
	a=a-(146097*b)/4;
	c=(4*a+3)/1461;
	a=a-(1461*c)/4;
	d=(5*a+2)/153;
	out_data->day=a-(153*d+2)/5+1;
	out_data->month=d+3-12*(d/10);
	out_data->year=100*b+c-4800+(d/10)-2000;
	out_data->hour=time/3600;
	out_data->minute=(time%3600)/60;
	out_data->second=(time%3600)%60;
}

uint32_t convert_to_Unix_time(date_time* data)
{
	char a;
	int y;
	char m;
	unsigned long Uday;
	unsigned long time;

	a=((14-data->month)/12);
	y=data->year+2000+4800-a;
	m=data->month+(12*a)-3;
	Uday=(data->day+((153*m+2)/5)+365*y+(y/4)-(y/100)+(y/400)-32045)-2440588;
	time=Uday*SEC_PER_DAY;
	time+=data->second+data->minute*60+data->hour*3600;
	return time;
}

void decode_date_time(uint32_t second, date_time* data)
{
	uint32_t year_day = 0;
	
	data->second = second%SEC_PER_MIN;
	data->minute = (second%SEC_PER_HOUR)/SEC_PER_MIN;
	data->hour = (second%SEC_PER_DAY)/SEC_PER_HOUR;
	
	data->year = second/SEC_PER_NYEAR;
	
	year_day = second/SEC_PER_DAY;
	data->w_day = (year_day + 5 )%7;
	year_day -= ( (data->year) * 365 + data->year/4 + (data->year%4 != 0) );
	
	
	for(uint8_t i = 0; i < 12; i++)
	{
		if(year_day > (month_days[i] + ((i == 1) && (data->year%4 == 0))))
		{
			year_day -= (month_days[i] + ((i == 1) && (data->year%4 == 0)));
		}
		else
		{
			data->month = i;
			data->day = year_day+1;
			break;
		}
	}	
}

void decode_from_seconds(uint32_t second, date_time* data)
{
	uint32_t year_day = 0;
	
	data->second = second%SEC_PER_MIN;
	if(data->second == 0)
	{
		data->minute = (second%SEC_PER_HOUR)/SEC_PER_MIN;
		if(data->minute == 0)
		{
			data->hour = (second%SEC_PER_DAY)/SEC_PER_HOUR;
			if(data->hour == 0)
			{
				data->year = second/SEC_PER_NYEAR;
				year_day = (second%SEC_PER_NYEAR)/SEC_PER_DAY;
				if(year_day < data->year/4) data->year--;
				else year_day -= data->year/4;
				
				for(uint8_t i = 0; i < 12; i++)
				{
					if(year_day > (month_days[i] + ((i == 2) && (data->year/4 == 0))))
					{
						year_day -= (month_days[i] + ((i == 2) && (data->year/4 == 0)));
						
					}
					else
					{
						data->month = i;
						break;
					}
				}
			}
		}
	}
}

uint32_t encode_seconds(date_time* data)
{
	uint32_t second = 0, days = 0;
	second += data->second;
	second += data->minute * SEC_PER_MIN;
	second += data->hour * SEC_PER_HOUR;
	days 	+= (data->year) * 365 + data->year/4 + (data->year%4 != 0);
	for(int i = 0; i < data->month-1; i++) 
		days+= month_days[i] + ((i == 1) && (data->year%4 == 0));
	days 	+= data->day;
	second += days * SEC_PER_DAY;
	return second;
}

void start_clock(uint32_t second)
{
	_init_RTC(second);
}

void write_time(date_time* data)
{
	_write_time(convert_to_Unix_time(data));
}	
