#ifndef _MYTYPES_H_
#define _MYTYPES_H_

#include "stdint.h"
#include "stdbool.h"

typedef union
{
	uint32_t draw[8];
	uint8_t out[8][4];
}Screen;

typedef struct
{
	//
	uint8_t		*interface_pointer;
	uint8_t		*data_pointer;
	uint8_t		*time_pointer;
	uint8_t		*date_pointer;
	//
	uint8_t		time_mode;
	uint8_t		time_menu_mode;
	uint8_t		date_mode;
	uint8_t		date_menu_mode;
	//
	uint8_t		sensor_type;
	uint8_t		font_number;
	uint8_t		template_number;
	uint8_t		day_bridhtness;
	uint8_t		nigth_bridhtness;
	uint8_t		day_bridhtness_hour;
	uint8_t		nigth_bridhtness_hour;
	//
	uint8_t		menu_mode;
	uint8_t		menu_neasted_page;
	uint8_t		menu_counter;
	//
	bool		dot;
	//
	uint16_t	sd_timer;
	bool		sd_error;
	bool		sd_inserted;
	bool		sd_ready;
	bool		sd_mounted;
	char		sd_1st_song_name[14];
	char		sd_2d_song_name[14];
	uint8_t		sd_1st_song_num;
	uint8_t		sd_2d_song_num;
	uint8_t		sd_1st_last_song_num;
	uint8_t		sd_2d_last_song_num;
	uint8_t		poly_songs_amount;
	uint16_t	sd_songs_amount;
	
}global_data_t;

#endif
