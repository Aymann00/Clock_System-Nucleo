/*
 *@file		:	I2C_Interface.h
 *@author	: 	Mohamed Wael
 *@brief	:	Main Functions for I2C Peripheral
 */

#ifndef I2C_INC_I2C_INTERFACE_H_
#define I2C_INC_I2C_INTERFACE_H_

/******************* MAIN INCLUDES *********************/
#include <stdint.h>
#include "ErrTypes.h"
/*******************************************************/

/************** MAIN USER DEFINED VARIABLES ************/
typedef enum {
	I2C_NUMBER_1,
	I2C_NUMBER_2,
	I2C_NUMBER_3
}I2C_I2C_NUMBER_t;
typedef enum {
	ACK_STATE_DISABLED,
	ACK_STATE_ENABLED,
}I2C_ACK_STATE_t;

typedef enum {
	CLK_STRETCH_ENABLED,
	CLK_STRETCH_DISABLED
}I2C_CLK_STRETCH_STATE_t;

typedef enum {
	PACKET_ERR_CHECK_DISABLED,
	PACKET_ERR_CHECK_ENABLED,
}I2C_PACKET_ERROR_CHECK_STATE_t;

typedef enum {
	ADDRESSING_MODE_7BITS,
	ADDRESSING_MODE_10BITS,
}I2C_ADDRESSING_MODE_t;

typedef enum {
	MASTER_MODE_STANDARD,
	MASTER_MODE_FAST,
}I2C_MASTER_MODE_t;

typedef enum {
	FLAGS_SR1_SB	,			/*Start BIT Flag : Set when a Start condition generated.*/

	FLAGS_SR1_ADDR  ,			/*Address matched flag Set by hardware as soon
	 	 	 	 	 	 	 	 as the received slave address matched with
	 	 	 	 	 	 	 	 the OAR registers content*/

	FLAGS_SR1_BTF,				/*Byte transferred finished flag*/

	FLAGS_SR1_STOPF = 4,		/*Set by hardware when a Stop condition is
								detected on the bus by the slave after an acknowledge*/

	FLAGS_SR1_RXNE=6,			/*Set when DR is not empty in receiver mode.*/

	FLAGS_SR1_TXE,				/*Set when DR is empty in transmission mode.*/

	FLAGS_SR1_BERR,				/*BUS ERROR FLAG : Set by hardware when the interface detects
	 	 	 	 	 	 	 	 				Misplaced Start or Stop condition*/

	FLAGS_SR1_ARLO,				/*Arbitration Lost : Set by hardware when the interface
						loses the arbitration of the bus to another master */

	FLAGS_SR1_AF,				/*Acknowledge failure : Set by hardware when no acknowledge is
												returned*/

	FLAGS_SR1_OVR,				/*OVER RUN / UNDER RUN Error*/
}I2C_FLAGS_SR1_t;

typedef enum {
	FLAGS_SR2_BUSY=1,

}I2C_FLAGS_SR2_t;
typedef enum {
	I2C_INTERRUPTS_ERRIE=8,
	I2C_INTERRUPTS_EVEIE,
	I2C_INTERRUPTS_BUFIE,
}I2C_INTERRUPTS_t;

typedef struct {

	I2C_I2C_NUMBER_t				I2C_Num;
	I2C_CLK_STRETCH_STATE_t			Stretch_state;
	I2C_PACKET_ERROR_CHECK_STATE_t	PEC_State;
	uint8_t 						I2C_Pclk_MHZ;
	I2C_ADDRESSING_MODE_t			ADD_Mode;
	uint8_t							Chip_Address;
	I2C_MASTER_MODE_t				I2C_Mode;
	uint8_t							SCL_Frequency_KHZ;
}I2C_Configs_t;

/******** END OF MAIN USER DEFINED VARIABLES ***********/


/****************** MAIN FUNCTIONS *********************/

/*
 * @function 		:	SPI_Init
 * @brief			:	Initialize SPI
 * @param			:	SPI Configurations structure
 * @retval			:	Error State
 */
Error_State_t I2C_Init(const I2C_Configs_t * I2C_Configs);


/*
 * @function 		:	I2C_Send_Start_Condition
 * @brief			:	Send start condition by the chip
 * @param			:	I2C Number
 * @retval			:	Error State
 * @Note			:	Calling this function for the first time will send Start Condition
						and make the Chip State a Master.
 * @Note			:	Calling for second time without sending stop condition
 						will send a repeated start
 */
Error_State_t I2C_Send_Start_Condition(I2C_I2C_NUMBER_t	I2C_Num);

/*
 * @function 		:	I2C_Send_Stop_Condition
 * @brief			:	Send Stop condition by the chip
 * @param			:	I2C Number
 * @retval			:	Error State
 * @Note			:	1-	Calling this function as Master Must be done after the last
 * 							byte is written to the DR register.
 * 							(The Chip automatically goes back to slave mode).
 *
 * 						2- Calling this function in As slave will
 * 							Release the SCL and SDA lines after the current byte transfer
*/
Error_State_t I2C_Send_Stop_Condition(I2C_I2C_NUMBER_t	I2C_Num);

/*
 * @function 		:	I2C_Master_Transmit
 * @brief			:	Transmit (Write) data to Slave device
 * @param			:	I2C Configurations
 * @param			:	Slave Address
 * @param			:	Data to be send
 * @retval			:	Error State
*/
Error_State_t I2C_Master_Transmit(const I2C_Configs_t * I2C_Configs , uint8_t SlaveADD , uint8_t *DataToSend ,uint8_t data_Size);

/*

@function         :    I2C_SendAddressPacketMTransmitter
@brief            :    Send Address Packet
@param            :    Pointer to I2c Config Structure
@param            :    Address to Send
@retval           :    VOID
 */
void I2C_SendAddressPacketMTransmitter( I2C_Configs_t * Config , uint8_t Address );

/*

@function         :    I2C_SendDataPacket
@brief            :    Send Data Packet
@param            :    Pointer to I2c Config Structure
@param            :    Data to Send
@retval           :    VOID
 */
void I2C_SendDataPacket(I2C_Configs_t * Config , uint8_t Data );

/*

@function         :    I2C_SendAddressPacketMReceiver
@brief            :    Send Address Packet
@param            :    Pointer to I2c Config Structure
@param            :    Address to Send
@retval           :    VOID
 */
void I2C_SendAddressPacketMReceiver( I2C_Configs_t * Config , uint8_t Address );

/*
 * @function 		:	I2C_Master_Receive
 * @brief			:	Receive (READ) data From Slave device
 * @param			:	I2C Configurations
 * @param			:	Variable to save Received data
 * @retval			:	Error State
 */
Error_State_t I2C_Master_Receive(const I2C_Configs_t * I2C_Configs , uint8_t *ReceivedData);

/*
 * @function 		:	I2C_Enable_DMA
 * @brief			:	Enable DMA Stream
 * @param			:	I2C Number
 * @retval			:	Error State
*/
Error_State_t I2C_Enable_DMA(I2C_I2C_NUMBER_t	I2C_Num);

/*
 * @function 		:	I2C_Disable_DMA
 * @brief			:	Disable DMA Stream
 * @param			:	I2C Number
 * @retval			:	Error State
*/
Error_State_t I2C_Disable_DMA(I2C_I2C_NUMBER_t	I2C_Num);

/*

@function         :    I2C_CLR_ADDR
@brief            :    Software sequence to Clear ADDR Flag
@param            :    I2C Number
@retval            :    VOID
*/
 void I2C_CLR_ADDR(I2C_I2C_NUMBER_t I2C_Num);
/***************End of MAIN FUNCTIONS*******************/

#endif /* I2C_INC_I2C_INTERFACE_H_ */
