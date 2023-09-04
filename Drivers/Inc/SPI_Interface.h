/*
 *@file		:	SPI_Interface.h
 *@author	: 	Mohamed Wael
 *@brief	:	Main Functions & Enumerators Defines FOR SPI Peripheral
 */

#ifndef SPI_INC_SPI_INTERFACE_H_
#define SPI_INC_SPI_INTERFACE_H_

/******************* MAIN INCLUDES *********************/
#include <stdint.h>
#include "../../LIBRARY//ErrTypes.h"
/*******************************************************/


/************** MAIN USER DEFINED VARIABLES ************/
typedef enum {
	SPI_NUMBER1,
	SPI_NUMBER2,SPI_NUMBER3,SPI_NUMBER4
}SPI_SPI_NUMBER_t;
typedef enum {
	BAUDRATE_FpclkBY2,
	BAUDRATE_FpclkBY4,
	BAUDRATE_FpclkBY8,
	BAUDRATE_FpclkBY16,
	BAUDRATE_FpclkBY32,
	BAUDRATE_FpclkBY64,
	BAUDRATE_FpclkBY128,
	BAUDRATE_FpclkBY256,
}SPI_BAUDRATE_VALUES_t;

typedef enum {
	CLOCK_POLARITY_IDLE_LOW,
	CLOCK_POLARITY_IDLE_HIGH,
}SPI_CLOCK_POLARITY_t;

typedef enum {
	CLOCK_PHASE_CAPTURE_FIRST,
	CLOCK_PHASE_CAPTURE_SECOND
}SPI_CLOCK_PHASE_t;

typedef enum {

	TRANSFER_MODE_FULL_DUPLEX,
	TRANSFER_MODE_HALF_DUPLEX_RECEIVER=2,
	TRANSFER_MODE_HALF_DUPLEX_TRANSMITTER,
	TRANSFER_MODE_SIMPLEX,					/*Selected if the Chip is receiver always*/

}SPI_TRANSFER_MODES_t;

typedef enum {
	FRAME_FORMAT_MSB_FIRST,
	FRAME_FORMAT_LSB_FIRST,

}SPI_FRAME_FORMAT_TYPE_t;

typedef enum {
	DATA_FRAME_SIZE_8BITS,
	DATA_FRAME_SIZE_16BITS,
}SPI_DATA_FRAME_SIZE_t;

typedef enum {
	CHIP_MODE_SLAVE	,
	CHIP_MODE_MASTER
}SPI_CHIP_MODE_t;

typedef enum {

	SLAVE_MANAGE_HW,
	SLAVE_MANAGE_SW_SLAVE_ACTIVE=2,
	SLAVE_MANAGE_SW_SLAVE_INACTIVE,

}SPI_SLAVE_MANAGE_STATE_t;

typedef enum {
	CRC_STATE_DISABLED,
	CRC_STATE_ENABLED
}SPI_CRC_STATE_t;

typedef enum {
	MULTIMASTER_PROVIDED,
	MULTIMASTER_NOT_PROVIDED,
}SPI_MULTIMASTER_STATE_t;

typedef enum {
	SPI_FLAGS_RXNE,
	SPI_FLAGS_TXE,
	SPI_FLAGS_CHSIDE,
	SPI_FLAGS_UDR,
	SPI_FLAGS_CRCERR,
	SPI_FLAGS_MODF,
	SPI_FLAGS_OVR,
	SPI_FLAGS_BSY,
	SPI_FLAGS_FE,
}SPI_FLAGS_t;

typedef enum {
	SPI_INTERRUPT_ERRIE=5,
	SPI_INTERRUPT_RXNEIE,
	SPI_INTERRUPT_TXEIE,
}SPI_INTERRUPTS_t;

typedef enum {
	NO_SRC,
	SOURCE_TX,
	SOURCE_RX_SLAVE,
	SOURCE_RX_MASTER
}IRQ_SOURCES_t;

typedef enum {
	SLAVE_STATE_ACTIVATED,
	SLAVE_STATE_DEACTIVATED
}SLAVE_STATE_t;
typedef struct{
	SPI_SPI_NUMBER_t				SPI_Num;
	SPI_BAUDRATE_VALUES_t 			BaudRate_Value;
	SPI_CLOCK_POLARITY_t			Clock_Polarity;
	SPI_CLOCK_PHASE_t				Clock_Phase;
	SPI_TRANSFER_MODES_t			Transfer_Mode;
	SPI_FRAME_FORMAT_TYPE_t 		Frame_Type;
	SPI_DATA_FRAME_SIZE_t			Frame_Size;
	SPI_CHIP_MODE_t					Chip_Mode;
	SPI_SLAVE_MANAGE_STATE_t		Slave_Manage_State;
	SPI_CRC_STATE_t					CRC_State;
	SPI_MULTIMASTER_STATE_t			MultiMaster_State;
}SPI_CONFIGS_t;
/******** END OF MAIN USER DEFINED VARIABLES ***********/


/****************** MAIN FUNCTIONS *********************/

/*
 * @function 		:	SPI_Init
 * @brief			:	Initialize SPI
 * @param			:	SPI Configurations structure
 * @retval			:	Error State
 */
Error_State_t SPI_Init(const SPI_CONFIGS_t * SPI_Config);

/*
 * @function 		:	SPI_Transmit
 * @brief			:	Transmit Data via SPI
* @param			:	SPI Configurations structure
 * @param			:	pointer to Data Buffer
 * @param 			: 	Data Buffer Size
 * @retval			:	Error State
 */
Error_State_t SPI_Transmit(const SPI_CONFIGS_t * SPI_Config, uint16_t * Data , uint8_t Buffer_Size);

/*
 * @function 		:	SPI_Receive
 * @brief			:	Receive Data via SPI
 * @param			:	SPI Configurations structure
 * @param			:	Buffer to save Data
 * @param 			: 	Data Buffer Size
 * @retval			:	Error State
 */
Error_State_t SPI_Receive(const SPI_CONFIGS_t * SPI_Config, uint16_t * Received_Data ,uint8_t Buffer_Size);


/*
 * @function 		:	SPI_Transmit_IT
 * @brief			:	Transmit Data via SPI and
 * 						generate interrupt when Transmission is complete
 * @param			:	SPI Configurations structure
 * @param			:	Data To Send
 * @param 			: 	Data Buffer Size
 * @param			:	CallBack Function
 * @retval			:	Error State
 */
Error_State_t SPI_Transmit_IT(const SPI_CONFIGS_t * SPI_Config, uint8_t * Data , uint8_t Buffer_Size, void (* SPI_TXC_CallBackFunc)(void));

/*
 * @function 		:	SPI_Receive_IT
 * @brief			:	Receive Data via SPI and generate interrupt when receive is complete
 * @param			:	SPI Configurations structure
 * @param			:	Buffer to save Data
 * @param 			: 	Data Buffer Size
 * @param			:	CallBack Function
 * @retval			:	Error State
 */
Error_State_t SPI_Receive_IT(const SPI_CONFIGS_t * SPI_Config, uint16_t * Received_Data ,uint8_t Buffer_Size , void (* SPI_RXC_CallBackFunc)(void));

/*
 * @function 		:	SPI_Enable_DMA_RX
 * @brief			:	Enable DMA Line for SPI Receiving
 * @param			:	SPI NUMBER
 * @retval			:	Error State
 */
Error_State_t SPI_Enable_DMA_RX(SPI_SPI_NUMBER_t SPI_Num);

/*
 * @function 		:	SPI_Enable_DMA_TX
 * @brief			:	Enable DMA Line for SPI Transmitting
 * @param			:	SPI NUMBER
 * @retval			:	Error State
 */
Error_State_t SPI_Enable_DMA_TX(SPI_SPI_NUMBER_t SPI_Num);

/*
 * @function 		:	SPI_Internal_Slave_Activate
 * @brief			:	Enable Slave by itself
 * @param			:	SPI NUMBER
 * @param			:	Slave_State
 * @retval			:	Error State
 * @warning			:	this function is used only if the Chip mode is slave and
 * 						SW Slave management is enabled
 */
Error_State_t SPI_SET_Internal_Slave_State(SPI_SPI_NUMBER_t SPI_Num, SLAVE_STATE_t Slave_State);


/***************End of MAIN FUNCTIONS*******************/


#endif /* SPI_INC_SPI_INTERFACE_H_ */
