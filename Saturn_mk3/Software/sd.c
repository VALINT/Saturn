#include "sd.h"

static FATFS       	fs;                                     //
static DIR         	dir;                                    //
static FILINFO     	fno;                                    //
static WORD        	s1;                                     //  
static FRESULT     	result;                                 //

static uint8_t		try_counter = 0;

int8_t isWAV(char *string);

void sd_init(global_data_t* status)
{
	if((try_counter == 5) && !status->sd_error) try_counter = 0;
	if((status->sd_timer == 0) && !status->sd_error && !status->sd_ready)
	{
		result = pf_mount(&fs);
		if(result == FR_OK)
		{
			status->sd_mounted = true;
			
			result = pf_opendir(&dir,"");
			if(result == FR_OK)
			{
				while(1)
				{
					result = pf_readdir(&dir,&fno);		// Get file or DIR name
					if(result == FR_OK)
					{
						int8_t temp = isWAV(fno.fname);
						if(temp == 1)
						{
							status->sd_songs_amount++;
						}
						else if(temp == -1)
						{
							status->sd_ready = true;
							return;
						}
					}
					else
					{
						break;
					}						
				}
			}
		}			
		
		if(result != FR_OK)
		{
			status->sd_timer = 1000;
			status->sd_ready = false;
			status->sd_mounted = false;
			try_counter ++;
			if(try_counter == 5)
			{
				status->sd_error = true;
			}
			return;
		}
	}
}

void sd_read(global_data_t* status)
{
	uint8_t num = status->sd_1st_song_num;
	result = pf_opendir(&dir,"");
	if(result == FR_OK)							//If read access is failed go to FAIL mode
	{
		while(1)
		{
			result = pf_readdir(&dir,&fno);
			if(result == FR_OK)							//If read access is failed go to FAIL mode
			{
				if(isWAV(fno.fname) == 1)					//If last read file is WAV
				{
					if(num == 0)
					{
						for(uint8_t i = 0; i < 13; i++)
						{
							status->sd_1st_song_name[i] = fno.fname[i];
						}
						return;
					}
					else
					{
						num--;
					}
				}
			}
		}
	}
}

int8_t isWAV(char *string)					//Check if the most resently read file is *.WAV
{
	for(uint8_t i = 0; i < 13; i++)
	{
		if(*(string+i) == '\0')				//If name have 0 length it is end of directive 
		{
			if(i == 0)
				return -1;					
			return 0;
		}
		else if(((*(string+i) == '.') && (*(string+i+1) == 'W') && (*(string+i+2) == 'A') && (*(string+i+3) == 'V'))||
				((*(string+i) == '.') && (*(string+i+1) == 'w') && (*(string+i+2) == 'a') && (*(string+i+3) == 'v')))
		return 1;
	}
	return 0;
}
