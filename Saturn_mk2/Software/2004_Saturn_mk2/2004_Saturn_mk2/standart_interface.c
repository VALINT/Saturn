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
 *///------------------------------------------------------------------------------------------------------ 

#include "standart_interface.h"

//---------------------------------------------------------------------------------------------------------
//UART part
/*
#ifdef _AVR
    void _uart_init(void)
    {
        #ifdef atmega328
            UBRR0L = UBRRL_value;
            #if USE_2X
                UCSR0A |= (1 << U2X0);
            #else
                UCSR0A &= ~(1 << U2X0);
            #endif
            // Enable USART transmitter/receiver 
            UCSR0B = (1 << TXEN0) | (1 << RXEN0);
            UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);   // 8 data bits, 1 stop bit
        #endif
        
        #ifdef atmega8
            UBRRL = UBRRL_value;
            #if USE_2X
                UCSRA |= (1 << U2X);
            #else
                UCSRA &= ~(1 << U2X);
            #endif
            // Enable USART transmitter/receiver 
            UCSRB = (1 << RXCIE)|(1 << TXEN) | (1 << RXEN);
            UCSRC = (1 << URSEL) | (3 << UCSZ0); 
        #endif
        
    }

    void _uart_write_char(char data)
    {
        #ifdef atmega328
            while(! (UCSR0A & (1 << UDRE0))){}
            UDR0 = data;
        #endif
        
        #ifdef atmega8
            while(! (UCSRA & (1 << UDRE))){}
            UDR = data;
        #endif
    }

    void _uart_send_array(char* data)
    {
        while(*data != '\0')
        {
            _uart_write_char(*data);
            data++;
        }
    }
    
    void _uart_send_array_s(const char* data)
    { 
        while(*data != '\0')
        {
            _uart_write_char(*data);
            data++;
        }
    }
#endif
    
#ifdef _STM32
    void _uart_init()
    {
        GPIO_InitTypeDef GPIO_InitStruct;
        USART_InitTypeDef USART1_InitStruct;
        
        // Enable clock for USART1 unit
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
        
        // Configure USART pins
        GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;  //RX / TX outputs
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;   	
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
        GPIO_Init(GPIOA, &GPIO_InitStruct);
        
        GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;  //RX / TX outputs
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;   	
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
        GPIO_Init(GPIOA, &GPIO_InitStruct);
        
        // Remap USART1 pins
        //GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);    
   
        // Configure USART1
        USART1_InitStruct.USART_BaudRate = BAUD;
        USART1_InitStruct.USART_WordLength = USART_WordLength_8b;
        USART1_InitStruct.USART_StopBits = USART_StopBits_1;
        USART1_InitStruct.USART_Parity = USART_Parity_No;
        USART1_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
        USART1_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
        USART_Init(USART1, &USART1_InitStruct);
        USART_Cmd(USART1, ENABLE);
    }
    
    void _uart_write_char(char data)
    {
        USART_SendData(USART1, data );
        while(!USART_GetFlagStatus(USART1, USART_FLAG_TC));
    }

    void _uart_send_array(char* data)
    {
        while(*data != '\0')
        {
            USART_SendData(USART1, *data );
            while(!USART_GetFlagStatus(USART1, USART_FLAG_TC));
            data ++;
        }
    }
    
    void _uart_send_array_s(const char* data)
    {
        while(*data != '\0')
        {
            USART_SendData(USART1, *data );
            while(!USART_GetFlagStatus(USART1, USART_FLAG_TC));
            data ++;
        }
    }   
#endif
*/
//---------------------------------------------------------------------------------------------------------
// I2C part
    
#ifdef _AVR
    void _i2c_init(void)
    {
        TWBR = 0xFF;
    }

    void _i2c_start_operation(void)
    {
        TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
        while ((TWCR & (1<<TWINT)) == 0);
    }

    void _i2c_stop_operation(void)
    {
        TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
    }

    void _i2c_send_byte(uint8_t byte)
    {
        TWDR = byte;
        TWCR = (1 << TWINT) | (1 << TWEN);
        while ((TWCR & (1 << TWINT)) == 0);
    }

    void _i2c_send_data(uint8_t data, uint8_t addres)
    {
        _i2c_start_operation();
        _i2c_send_byte(addres);
        _i2c_send_byte(data);
        _i2c_stop_operation();
    }

    //void i2c_send_array(uint8_t &arr_start, uint8_t lenght);

    uint8_t _i2c_reseive_byte(void)
    {
        TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
        while ((TWCR & (1 << TWINT)) == 0);
        return TWDR;
    }

    uint8_t _i2c_reseive_last_byte(void)
    {	
        TWCR = (1 << TWINT) | (1 << TWEN);
        while ((TWCR & (1 << TWINT)) == 0);
        return TWDR;
    }
#endif
//---------------------------------------------------------------------------------------------------------
// SPI part
//---------------------------------------------------------------------------------------------------------
/*
#ifdef _AVR   
    void _spi_init(void)
    {
      //
      SPI_DDRX |= (1<<SPI_MOSI)|(1<<SPI_SCK)|(1<<SPI_SS);
      SPI_DDRX &= ~(1<<SPI_MISO);
      
      SPI_PORTX |= (1<<SPI_MOSI)|(1<<SPI_SCK)|(1<<SPI_SS)|(1<<SPI_MISO);
       
      //
      SPCR = (0<<SPIE)|(1<<SPE)|(0<<DORD)|(1<<MSTR)|(0<<CPOL)|(0<<CPHA)|(0<<SPR1)|(0<<SPR0);
     // SPSR = (1<<SPI2X);
    }

    //
    void _spi_writeByte(uint8_t data)
    {
       SPDR = data; 
       while(!(SPSR & (1<<SPIF)));
    }

    //
    uint8_t _spi_readByte(void)
    {  
       SPDR = 0xff;
       while(!(SPSR & (1<<SPIF)));
       return SPDR; 
    }

    //
    uint8_t _spi_writeReadByte(uint8_t data)
    {  
       SPDR = data;
       while(!(SPSR & (1<<SPIF)));
       return SPDR; 
    }

    //
    void _spi_writeArray(uint8_t num, uint8_t *data)
    {
       while(num--){
          SPDR = *data++;
          while(!(SPSR & (1<<SPIF)));
       }
    }

    //
    void _spi_writeReadArray(uint8_t num, uint8_t *data)
    {
       while(num--){
          SPDR = *data;
          while(!(SPSR & (1<<SPIF)));
          *data++ = SPDR; 
       }
    }
#endif
    
#ifdef _STM32
    void _spi_init()
    {
        GPIO_InitTypeDef GPIO_InitStruct;
        SPI_InitTypeDef SPI1_InitStruct;
        // Enable clock for SPI1 unit
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
        
        // Configure SPI pins
        GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
        GPIO_Init(GPIOA, &GPIO_InitStruct);
        
        GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
        GPIO_Init(GPIOA, &GPIO_InitStruct);
        
        GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;  //RX / TX outputs
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;   	
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
        GPIO_Init(GPIOA, &GPIO_InitStruct);
        
        // Configure SPI1
        SPI1_InitStruct.SPI_Mode = SPI_Mode_Master;
        SPI1_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
        SPI1_InitStruct.SPI_DataSize = SPI_DataSize_8b;
        SPI1_InitStruct.SPI_NSS = SPI_NSS_Soft;
        SPI1_InitStruct.SPI_CPOL = SPI_CPOL_Low;
        SPI1_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
        SPI1_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
        SPI1_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
        //SPI1_InitStruct.SPI_CRCPolynomial = 7;
        SPI_Init(SPI1, &SPI1_InitStruct);
        SPI_Cmd(SPI1, ENABLE);
        //SPI_NSSInternalSoftwareConfig(SPI1, SPI_NSSInternalSoft_Set);
        //SPI_SSOutputCmd(SPI1, ENABLE);
    }
    
    void _spi_writeByte(uint8_t data)
    {
        SPI_I2S_SendData(SPI1, data);
        while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
    }
    uint8_t _spi_readByte(void)
    {
        SPI_I2S_SendData(SPI1, 0xff);
        while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));       
        return((uint8_t)SPI_I2S_ReceiveData(SPI1));
    }
    //
    uint8_t _spi_writeReadByte(uint8_t data)
    {
        SPI_I2S_SendData(SPI1, data);
        while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));       
        return((uint8_t)SPI_I2S_ReceiveData(SPI1));     
    }
    //
    void _spi_writeArray(uint8_t num, uint8_t *data)
    {
       while(num--){
           SPI_I2S_SendData(SPI1, *data);
           while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)); 
           data++;
       }
    }
    //
    void _spi_writeReadArray(uint8_t num, uint8_t *data)
    {
       while(num--){
           SPI_I2S_SendData(SPI1, *data);
           while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));       
           *data = ((uint8_t)SPI_I2S_ReceiveData(SPI1)); 
           data++;           
       }
    }
#endif
*/