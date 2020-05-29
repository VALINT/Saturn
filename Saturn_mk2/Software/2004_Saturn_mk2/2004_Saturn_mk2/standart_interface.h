/*------------------------------------------------------------------------------------------------------
 * Standard interface module
 *______________________________________________________________________________________________________
 *  __    __ __ __     __   _____	 _____ _____   __    __  _  ____    __    ____  _____
 *  \ \  / //  \\ \    \_\ / ___/	/ ___/|_   _| /  \  |  \| ||  _ \  /  \  | |\ \|_   _|
 *   \ \/ //    \\ \___ |/ \___ \	\___ \  | |  /    \ | |\  || |_|| /    \ | |/ /  | |
 *    \__//__/\__\\____\   /____/	/____/  |_| /__/\__\|_| \_||____//__/\__\|_|\_\  |_|
 *					 _  __  _  _____  ____  ____  ____    _    ____  ____
 *					| ||  \| ||_   _|| ___|| |\ \| ___|  / \  /  __\| ___|
 *					| || |\  |  | |  | ___|| |/ /| ___| / _ \ | |__ | ___|
 *					|_||_| \_|  |_|  |____||_|\_\|_|   /_/ \_\\____/|____|
 *_______________________________________________________________________________________________________
 *
 * Created: 18-Sep-2018 23:05:14
 *  Author: VAL
 *------------------------------------------------------------------------------------------------------- 
 *	This module consist program realization of communication interfaces.
 *	Using hardware interfaces (need for easy programing if you need using interface)
 *-------------------------------------------------------------------------------------------------------
 *	History:
 *		-	18-Sep-2018
 *          Creation of this module.
 *          Possible to work with three popular interfaces I2C SPI and UART for AVR atmega8 and atmega328
 *      -   06-Oct-2019
 *          Added some macros and functions for convenient work with SPI on AVR (need for SD card work)
 *      -   24-Jan-2020
 *          Added possibility to work with STM23F10X. Now, only SPI and UART but still not full control.
 *
 *-------------------------------------------------------------------------------------------------------
 *	Features:
 *		-	Using hardware I2C interface (protocol) 
 *      -   Using hardware SPI interface
 *      -   Using hardware UART interface 
 *
 *-------------------------------------------------------------------------------------------------------
 */

#ifndef STANDART_INTERFACE_H_
#define STANDART_INTERFACE_H_

#include <stdint.h>

//-------------------------------------------------------------------------------------------------------
// ***** CHOOSE YOUR MCU *****
//-------------------------------------------------------------------------------------------------------

#define atmega8
//#define atmega328
//#define stm32f10x


//-------------------------------------------------------------------------------------------------------
// ***** CHOOSE BAUDRATE IF IT NEED *****
//-------------------------------------------------------------------------------------------------------

#ifndef BAUD	//If baud rate not is not defined, set default value
	#define BAUD 9600 // Baud rate
#endif


//-------------------------------------------------------------------------------------------------------
// Technical part
//-------------------------------------------------------------------------------------------------------

//If we use any AVR MCU choose this parameter automatically
#ifdef atmega8 //If we use atmega8
    #define _AVR
#endif

#ifdef atmega328//If we use atmega328
    #define _AVR
#endif

//If we use STM32 MCU choose parameter _STM32 automatically
#ifdef stm32f10x
    #define _STM32
    #define _F10X
#endif

//Include needed for work libraries (for stm32f10x series)
#ifdef _STM32
    #ifdef _F10X
        #include "stm32f10x_rcc.h"
        #include "stm32f10x_gpio.h"
        #include "stm32f10x_usart.h"
        #include "stm32f10x_spi.h"
    #endif
#endif

//Include needed for work libraries (for AVR)
#ifdef _AVR
    #include <avr/io.h>
    
    #ifndef F_CPU	//If FCPU is not defined, set default value
        #define F_CPU 8000000UL
    #endif
    
    #define UBRRL_value (F_CPU/(BAUD*16))-1
    
    #include <util/delay.h>
#endif

//-------------------------------------------------------------------------------------------------------
//	UART/USART HARDWARE part
//-------------------------------------------------------------------------------------------------------

//Procedures and functions needed for this interface work. 

#ifdef _STM32
    void _uart_init(void);
    
    void _uart_write_char(char data);
    
    void _uart_send_array(char* data);
    
    void _uart_send_array_s(const char* data);
#endif

#ifdef _AVR
    void _uart_init(void);

    void _uart_write_char(char data);

    void _uart_send_array(char* data);
    
    void _uart_send_array_s(const char* data);
#endif
//-------------------------------------------------------------------------------------------------------
//	I2C HARDWARE part
//-------------------------------------------------------------------------------------------------------

//Procedures and functions needed for this interface work. 
#ifdef _AVR
    void _i2c_init(void);

    void _i2c_start_operation(void);

    void _i2c_stop_operation(void);

    void _i2c_send_byte(uint8_t byte);

    void _i2c_send_data(uint8_t data, uint8_t addres);

    uint8_t _i2c_reseive_byte(void);

    uint8_t _i2c_reseive_last_byte(void);
#endif
//-------------------------------------------------------------------------------------------------------
//	SPI HARDWARE part
//-------------------------------------------------------------------------------------------------------
 
//Procedures and functions needed for work with this interface. 
/*#ifdef _AVR
    #define SPI_PORTX   PORTB
    #define SPI_DDRX    DDRB

    #define SPI_MISO   4
    #define SPI_MOSI   3
    #define SPI_SCK    5
    #define SPI_SS     0

    #define _spi_disableSS_m(ss)  do{SPI_PORTX |= (1<<(ss)); }while(0)								//Disable SPI device
    #define _spi_enableSS_m(ss)   do{SPI_PORTX &= ~(1<<(ss)); }while(0)								//Enable SPI device
    #define _spi_statSS_m(ss)    (!(SPI_PORTX & (1<<(ss))))											//Device status
    #define _spi_writeByte_m(data)  do{ SPDR = data; while(!(SPSR & (1<<SPIF))); }while(0)			//Send SPI byte
    #define _spi_readByte_m(data)  do{ SPDR = 0xff; while(!(SPSR & (1<<SPIF))); data = SPDR;}while(0)//Read SPI byte
 
    inline static uint8_t _spi_readByte_i(void)														//Get byte on SPI
    {
        SPDR = 0xff;
        while(!(SPSR & (1<<SPIF)));
        return SPDR;   
    }

    // Init SPI module accordingly to defined parameters above 
    void _spi_init(void); 
    //
    void _spi_writeByte(uint8_t data); 
    //
    uint8_t _spi_readByte(void);
    //
    uint8_t _spi_writeReadByte(uint8_t data);
    //
    void _spi_writeArray(uint8_t num, uint8_t *data);
    //
    void _spi_writeReadArray(uint8_t num, uint8_t *data);
#endif
    
#ifdef _STM32 
    
    #define _spi_disableSS_m        GPIO_SetBits(GPIOA, GPIO_Pin_4);
    #define _spi_enableSS_m         GPIO_ResetBits(GPIOA, GPIO_Pin_4);
    #define _spi_statSS_m       
    #define _spi_writeByte_m(data)  do{SPI_I2S_SendData(SPI1, data);\
                                    while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));}while(0)
    #define _spi_readByte_m(data)   do{SPI_I2S_SendData(SPI1, 0xff);\
                                    while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));\
                                    data = (uint8_t)SPI_I2S_ReceiveData(SPI1) ;}while(0)
    
    inline static uint8_t _spi_readByte_i(void)
    {
        SPI_I2S_SendData(SPI1, 0xff);
        while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));    
        return ((uint8_t)SPI_I2S_ReceiveData(SPI1));
    }
    
    // Init SPI module accordingly to defined parameters above 
    void _spi_init(void); 
    //
    void _spi_writeByte(uint8_t data); 
    //
    uint8_t _spi_readByte(void);
    //
    uint8_t _spi_writeReadByte(uint8_t data);
    //
    void _spi_writeArray(uint8_t num, uint8_t *data);
    //
    void _spi_writeReadArray(uint8_t num, uint8_t *data);
#endif
  */  
#endif /* STANDART_INTERFACE_H_ */
