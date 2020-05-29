/*-----------------------------------------------------------------------------------------------------------------------
 * DHT11/DHT22 module
 *_______________________________________________________________________________________________________________________
 *  __    __ __ __     __   _____	  ____   _   _  _____   ___   ___
 *  \ \  / //  \\ \    \_\ / ___/	 |  _ \ | |_| ||_   _| /_  | /_  |
 *   \ \/ //    \\ \___ |/ \___ \	 | |_| ||  _  |  | |     | |   | |
 *    \__//__/\__\\____\   /____/	 |____/ |_| |_|  |_|     |_|   |_|
 *				   _     _  ____  ____    _    ____  _   _
 *				  | |   | || __ \| |\ \  / \  | |\ \\ \_/ /
 *				  | |__ | || __ || |/ / / _ \ | |/ / \   /
 *			      |____||_||____/|_|\_\/_/ \_\|_|\_\  |_|
 *_______________________________________________________________________________________________________________________
 * Created: 19.02.2018 22:16:40
 *  Author: VAL
 *-----------------------------------------------------------------------------------------------------------------------
 */ 
#include "dht11.h"

void request()
{
	DHT_DDR  |=  (1<<DHT_PIN);
	DHT_PORT &= ~(1<<DHT_PIN);	
	_delay_ms(20);			
	DHT_PORT |=  (1<<DHT_PIN);
}

void response(DHT *data)
{
	DHT_DDR &= ~(1<<DHT_PIN);
	while( DHT_INREG & (1<<DHT_PIN));
	while((DHT_INREG & (1<<DHT_PIN))==0);
	while( DHT_INREG & (1<<DHT_PIN));
}

uint8_t receiveData(DHT *data)
{
	uint8_t c = 0;
	for (uint8_t q=0; q<8; q++)
	{
		while((DHT_INREG & (1<<DHT_PIN)) == 0);
		_delay_us(30);
		if(DHT_INREG & (1<<DHT_PIN)) c = (c<<1)|(0x01);
		else c = (c<<1);
		while(DHT_INREG & (1<<DHT_PIN));
	}
	return c;
}

uint8_t	DHTreadData(DHT *data)
{
	request(data);
	response(data);
	data->rhWhole			= receiveData(data);
	data->rhFractional		= receiveData(data);
	data->tempWhole			= receiveData(data);
	data->tempFractional	= receiveData(data);
	data->checkSum			= receiveData(data);
	return( (data->rhWhole + data->rhFractional + data->tempWhole + data->tempFractional) != data->checkSum);
}