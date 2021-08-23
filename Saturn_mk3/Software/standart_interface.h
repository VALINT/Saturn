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

#define stm32f10x


//-------------------------------------------------------------------------------------------------------
// ***** CHOOSE BAUDRATE IF IT NEED *****
//-------------------------------------------------------------------------------------------------------

#ifndef BAUD	//If baudrate not is not defined, set default value
	#define BAUD 9600 // Baud rate
#endif


//-------------------------------------------------------------------------------------------------------
// Techical part
//-------------------------------------------------------------------------------------------------------


//If we use STM32 MCU choose parameter _STM32 automaticaly
#if defined stm32f10x
    #define _STM32
    #define _F10X
#endif

//Include needed for work libaries (for stm32f10x serie)
#ifdef _STM32
    #ifdef _F10X
        #include "stm32f10x.h"
        #include "core_cm3.h"
    #endif
#endif

//-------------------------------------------------------------------------------------------------------
//	GPIO
//-------------------------------------------------------------------------------------------------------

#ifdef _STM32
	
	#define	PORTA			GPIOA
	#define	PORTB			GPIOB
	#define	PORTC			GPIOC
	#define	PORTD			GPIOD
	#define	PORTE			GPIOE
	#define	PORTF			GPIOF
	#define	PORTG			GPIOG
	
	#define PIN0			0
	#define PIN1			1
	#define PIN2			2
	#define PIN3			3
	#define PIN4			4
	#define PIN5			5
	#define PIN6			6
	#define PIN7			7
	#define PIN8			8
	#define PIN9			9
	#define PIN10			10
	#define PIN11			11
	#define PIN12			12
	#define PIN13			13
	#define PIN14			14
	#define PIN15			15
	
	#define PIN0_REG		0
	#define PIN1_REG		1
	#define PIN2_REG		2
	#define PIN3_REG		3
	#define PIN4_REG		4
	#define PIN5_REG		5
	#define PIN6_REG		6
	#define PIN7_REG		7
	#define PIN8_REG		0
	#define PIN9_REG		1
	#define PIN10_REG		2
	#define PIN11_REG		3
	#define PIN12_REG		4
	#define PIN13_REG		5
	#define PIN14_REG		6
	#define PIN15_REG		7
	
	#define PIN0_SHIFT_N	0
	#define PIN1_SHIFT_N	4
	#define PIN2_SHIFT_N	8
	#define PIN3_SHIFT_N	12
	#define PIN4_SHIFT_N	16
	#define PIN5_SHIFT_N	20
	#define PIN6_SHIFT_N	24
	#define PIN7_SHIFT_N	28
	#define PIN8_SHIFT_N	0
	#define PIN9_SHIFT_N	4
	#define PIN10_SHIFT_N	8
	#define PIN11_SHIFT_N	12
	#define PIN12_SHIFT_N	16
	#define PIN13_SHIFT_N	20
	#define PIN14_SHIFT_N	24
	#define PIN15_SHIFT_N	28

	#define M_INPUT			0x0
	#define M_OUT_2M		0x2
	#define M_OUT_10M		0x1
	#define	M_OUT_50M		0x3
	
	#define C_ANALOG		0x0
	#define C_FLOATING_IN	0x1
	#define C_IN_PP_PD		0x2
	
	#define C_GP_PP			0x0
	#define C_GP_OD			0x1
	#define	C_AF_PP			0x2
	#define	C_AF_OD			0x3
	
	#define INPUT_FLOATING			((C_FLOATING_IN << 2) | M_INPUT)
	#define	INPUT_PULL_UP			((C_IN_PP_PD << 2) | M_INPUT)
	#define	INPUT_PULL_DOWN			((C_IN_PP_PD << 2) | M_INPUT)
	#define	ANALOG_MODE				((C_ANALOG << 2) | M_INPUT)
	#define	OUTPUT_OPEN_DRAIN_2M	((C_GP_OD << 2) | M_OUT_2M)
	#define	OUTPUT_PUSH_PULL_2M		((C_GP_PP << 2) | M_OUT_2M)
	#define	OUTPUT_OPEN_DRAIN_10M	((C_GP_OD << 2) | M_OUT_10M)
	#define	OUTPUT_PUSH_PULL_10M	((C_GP_PP << 2) | M_OUT_10M)
	#define	OUTPUT_OPEN_DRAIN_50M	((C_GP_OD << 2) | M_OUT_50M)
	#define	OUTPUT_PUSH_PULL_50M	((C_GP_PP << 2) | M_OUT_50M)
	#define	ALTERN_PUSH_PULL_2M		((C_AF_PP << 2) | M_OUT_2M)
	#define	ALTERN_OPEN_DRAIN_2M    ((C_AF_OD << 2) | M_OUT_2M)
	#define	ALTERN_PUSH_PULL_10M    ((C_AF_PP << 2) | M_OUT_10M)
	#define	ALTERN_OPEN_DRAIN_10M   ((C_AF_OD << 2) | M_OUT_10M)
	#define	ALTERN_PUSH_PULL_50M    ((C_AF_PP << 2) | M_OUT_50M)
	#define	ALTERN_OPEN_DRAIN_50M   ((C_AF_OD << 2) | M_OUT_50M)
	
	#define ENABLE_PORT(port)				RCC->APB2ENR |= RCC_APB2ENR_IOP##port##EN
	#define	CLEAR_REG_FULL(port,reg)		port->reg = 0;
	#define CLEAR_REG_PART(port, reg, val)	port->reg &=~ val;
	#define SET_REG_PART(port, reg, val)	port->reg |= val;
	#define INIT_LOW_PIN(port, pin, func)	port->CRL |= (func << ( pin * 4 ))
	#define INIT_HIGH_PIN(port, pin, func)	port->CRH |= (func << ((pin - 8 ) * 4))
	#define CLEAR_INIT_LOW(port, pin)		port->CRL &=~ (0xf << ( pin * 4 ))
	#define CLEAR_INIT_HIGH(port, pin)		port->CRH &=~ (0xf << ((pin - 8 ) * 4))
	#define RESET_PIN(port, pin)			port->BRR = (0x1 << pin )
	#define CLEAR_PIN(port, pin)			port->BSRR = (0x100 << pin)
	#define SET_PIN(port, pin)				port->BSRR = (0x1 << pin)
	#define INVERT_PIN(port, pin)			port->ODR = port->ODR^(0x1 << pin)
	#define	READ_PIN(port, pin)				port_>IDR & (1 << pin)
	
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

//-------------------------------------------------------------------------------------------------------
//	I2C HARDWARE part
//-------------------------------------------------------------------------------------------------------

//Procedures and functions needed for this interface work. 
#ifdef _STM32

    typedef enum {I2C_OK, I2C_BUSY, I2C_SCL_FAULT, I2C_SDA_FAULT, I2C_BUS_FAULT, I2C_ACK_FAIL} I2C_ERR;

    I2C_ERR _i2c_init(void);
    
    void _i2c_start_operation(void);
    
    void _i2c_stop_operation(void);
	
	void _i2c_send_address(uint8_t byte);
    
    I2C_ERR _i2c_send_byte(uint8_t byte);
    
    uint8_t _i2c_receive_byte(void);
    
    uint8_t _i2c_receive_last_byte(void);
    
#endif

//-------------------------------------------------------------------------------------------------------
//	SPI HARDWARE part
//-------------------------------------------------------------------------------------------------------
 
//Procedures and functions needed for work with this interface.     
#ifdef _STM32 
	
	#define	SPI1NSS				4
	#define	SPI1SCK				5
	#define	SPI1MISO			6
	#define SPI1MOSI			7
	
	#define	SPI1NSS_REG			4
	#define	SPI1SCK_REG			5
	#define	SPI1MISO_REG		6
	#define SPI1MOSI_REG		7
	
	#define	SPI1NSS_SHIFT_N		16
	#define	SPI1SCK_SHIFT_N		20
	#define	SPI1MISO_SHIFT_N	24
	#define SPI1MOSI_SHIFT_N	28
	
	#define	SPI2NSS				12
	#define	SPI2SCK				13
	#define	SPI2MISO			14
	#define SPI2MOSI			15

	#define	SPI2NSS_REG			4
	#define	SPI2SCK_REG			5
	#define	SPI2MISO_REG		6
	#define SPI2MOSI_REG		7

	#define	SPI2NSS_SHIFT_N		16
	#define	SPI2SCK_SHIFT_N		20
	#define	SPI2MISO_SHIFT_N	24
	#define SPI2MOSI_SHIFT_N	28
	
	//CPHA
	#define SPI_FIRST_CLOCK_PHASE	0x0000
	#define SPI_SECOND_CLOCK_PHASE	0x0001
	//CPOL
	#define SPI_CLK_POLARITY_HIGH	0x0002
	#define SPI_CLK_POLARITY_LOW	0x0000
	//MSTR	
	#define SPI_MASTER_MODE			0x0004
	#define SPI_SLAVE_MODE			0x0000
	//BR
	#define SPI_CLS_DIV_2			0x0000
	#define SPI_CLS_DIV_4			0x0008
	#define SPI_CLS_DIV_8			0x0010
	#define SPI_CLS_DIV_16			0x0018
	#define SPI_CLS_DIV_32			0x0020
	#define SPI_CLS_DIV_64			0x0028
	#define SPI_CLS_DIV_128			0x0030
	#define SPI_CLS_DIV_256			0x0038
	//SPE
	#define SPI_ENABLE				0x0040
	#define SPI_DISABLE				0x0000
	//LSBFIRST
	#define SPI_MSB_FIRST			0x0000
	#define SPI_LSB_FIRST			0x0080
	//SSI
	#define SPI_NSS_SOFT_EN			0x0100
	#define SPI_NSS_SOFT_DIS		0x0000
	//SSM
	#define SPI_SOFT_SLAVE_EN		0x0200
	#define SPI_SOFT_SLAVE_SID 	 	0x0000
	//RXONLY
	#define	SPI_FULL_DUPLEX			0x0000
	#define SPI_HALF_DUPLEX			0x0400
	//DFF
	#define SPI_8_BIT_FRAME			0x0000
	#define SPI_16_BIT_FRAME		0x0800
	//CRCNEXT
	#define SPI_DATA_PHASE			0x0000
	#define SPI_CRC_PHASE			0x1000
	//CRCEN
	#define SPI_COUNT_CRC_EN		0x2000
	#define SPI_COUNT_CRC_DIS		0x0000
	//BIDIOE
	#define SPI_RECEIVE_ONLY		0x0000
	#define SPI_TRANSMIT_EN			0x4000
	//BIDIMODE
	#define SPI_2_WIRES_UNIDIR		0x0000
	#define SPI_1_WIRES_BIDIR		0x8000

	//CR2
	#define SPI_RX_DMA_INT_EN		0x01
	#define SPI_TX_DMA_INT_EN		0x02
	#define SPI_SS_OUT_INT_EN		0x04
	#define SPI_ERROR_INT_EN		0x10
	#define SPI_RX_INT_EN			0x20
	#define SPI_TX_INT_EN			0x40

	//SR
	#define SPI_DATA_RECEIVED		0x01
	#define SPI_DATA_TRANSMITED		0x02
	#define SPI_UNDERRUN_FLAG		0x08
	#define SPI_CRC_ERROR			0x10
	#define SPI_MODE_FAULT			0x20
	#define SPI_OVERRUN_FLAG		0x40
	#define SPI_BUSY				0x80

    #define _spi_disableSS_m        SET_PIN(GPIOB, SPI2NSS);//
    #define _spi_enableSS_m         RESET_PIN(GPIOB, SPI2NSS);//
    #define _spi_statSS_m       
    #define _spi_writeByte_m(data)  do{_spi_send_byte(SPI2, data);}while(0)
    #define _spi_readByte_m   		do{data = _spi_read_byte(SPI2);}while(0)
    
    inline static uint8_t _spi_readByte_i(void)
    {
        SPI2->DR = 0xff;
		while(!SPI2->SR & SPI_SR_TXE){};
		while(SPI2->SR & SPI_SR_BSY){};
        return ((uint8_t)SPI2->DR);
    }
    
    // Init SPI module accordingly to defined parameters above 
    void _spi_init(void); 
    //
    void _spi_writeByte(uint8_t data); 
	
	void _spi_send_byte(SPI_TypeDef*, char);
	char _spi_read_byte(SPI_TypeDef*);
    //
    uint8_t _spi_readByte(void);
    //
    uint8_t _spi_writeReadByte(uint8_t data);
    //
    void _spi_writeArray(uint8_t num, uint8_t *data);
    //
    void _spi_writeReadArray(uint8_t num, uint8_t *data);
#endif
    
#endif /* STANDART_INTERFACE_H_ */
