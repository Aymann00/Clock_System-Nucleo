/*
 *@file		:	I2C_Program.c
 *@author	: 	Mohamed Wael
 *@brief	:	Main Program for I2C Peripheral
 */

/******************* MAIN INCLUDES *********************/
#include <stdint.h>
#include "../../Library/STM32F446xx.h"
#include "../../Library/ErrTypes.h"


#include "../Inc/I2C_Interface.h"
#include "../Inc/I2C_Private.h"
/*******************************************************/


/********************** MAIN PV ************************/

I2C_REG_t * I2Cs[MAX_I2C_NUMBERS]={I2C1,I2C2,I2C3};


/*******************************************************/



/****************** MAIN FUNCTIONS *********************/

/*
 * @function 		:	SPI_Init
 * @brief			:	Initialize SPI
 * @param			:	SPI Configurations structure
 * @retval			:	Error State
 */
Error_State_t I2C_Init(const I2C_Configs_t * I2C_Configs)
{
	Error_State_t Error_State = OK ;

	if ( OK == I2C_CheckConfigs(I2C_Configs) )
	{
		/*Set Clock Stretch State*/
		I2Cs[I2C_Configs->I2C_Num]->I2C_CR1 &= ~(1<<(STRETCH_STATE_BIT));
		I2Cs[I2C_Configs->I2C_Num]->I2C_CR1 |=  ((I2C_Configs->Stretch_state)<<(STRETCH_STATE_BIT));
		/*Set Packet Error Check State*/
		I2Cs[I2C_Configs->I2C_Num]->I2C_CR1 &= ~(1<<(PEC_STATE_BIT));
		I2Cs[I2C_Configs->I2C_Num]->I2C_CR1 |=  ((I2C_Configs->PEC_State)<<(PEC_STATE_BIT));

		/*Set APB (Peripheral) Clock*/
		I2Cs[I2C_Configs->I2C_Num]->I2C_CR2 &= ~(PCLK_MASK);
		I2Cs[I2C_Configs->I2C_Num]->I2C_CR2 |=  ((I2C_Configs->I2C_Pclk_MHZ));

		/*Set Addressing Mode*/
		I2Cs[I2C_Configs->I2C_Num]->I2C_OAR1 &= ~(1<<(ADDMODE_BIT));
		I2Cs[I2C_Configs->I2C_Num]->I2C_OAR1 |=  ((I2C_Configs->ADD_Mode)<<(ADDMODE_BIT));

		/*Set Own Chip Address*/
		I2Cs[I2C_Configs->I2C_Num]->I2C_OAR1 &= ~(ADDVALUE_MASK<<(ADDVALUE_BIT));
		I2Cs[I2C_Configs->I2C_Num]->I2C_OAR1 |=  ((I2C_Configs->Chip_Address)<<(ADDVALUE_BIT));

		/*Set The Mode when the Chip Transferred to master*/
		I2Cs[I2C_Configs->I2C_Num]->I2C_CCR &= ~(1<<(I2C_MODE_BIT));
		I2Cs[I2C_Configs->I2C_Num]->I2C_CCR |=  ((I2C_Configs->I2C_Mode)<<(I2C_MODE_BIT));

		/*Set SCL Frequency*/
		I2C_Set_SCL_Frequency(I2C_Configs);
	}
	else {

		Error_State = I2C_CheckConfigs(I2C_Configs);

	}
	return Error_State ;
}


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
Error_State_t I2C_Send_Start_Condition(I2C_I2C_NUMBER_t	I2C_Num)
{
	Error_State_t	Error_State = OK;

	if ((I2C_Num >=I2C_NUMBER_1) && (I2C_Num <=I2C_NUMBER_3))
	{
		/*Send Start Condition*/
		I2Cs[I2C_Num]->I2C_CR1 |= (1<<START_GENERATE_BIT);

		/*Wait till Start Condition is Generated*/
		while (!(GET_BIT(I2Cs[I2C_Num]->I2C_SR1,FLAGS_SR1_SB)));

	}
	else {
		Error_State = I2C_WRONG_I2C_NUMBER;
	}

	return Error_State ;
}


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
Error_State_t I2C_Send_Stop_Condition(I2C_I2C_NUMBER_t	I2C_Num)
{
	Error_State_t	Error_State = OK;

	if ((I2C_Num >=I2C_NUMBER_1) && (I2C_Num <=I2C_NUMBER_3))
	{
		/*Send Start Condition*/
		I2Cs[I2C_Num]->I2C_CR1 |= (1<<STOP_GENERATE_BIT);

		/*Wait till Stop Condition is Detected*/
		//while (!(GET_BIT(I2Cs[I2C_Num]->I2C_SR1,FLAGS_SR1_STOPF)));

		//I2Cs[I2C_Num]->I2C_CR1 |= (0);
	}
	else {
		Error_State = I2C_WRONG_I2C_NUMBER;
	}

	return Error_State ;
}

/*
 * @function 		:	I2C_Master_Transmit
 * @brief			:	Transmit (Write) data to Slave device
 * @param			:	I2C Configurations
 * @param			:	Slave Address
 * @param			:	Data buffer to be send
 * @param			:	Number of data elements to be send
 * @retval			:	Error State
 */
Error_State_t I2C_Master_Transmit(const I2C_Configs_t * I2C_Configs , uint8_t SlaveADD , uint8_t * DataToSend, uint8_t data_Size)
{
	Error_State_t	Error_State = OK;

	Error_State = I2C_CheckConfigs(I2C_Configs);

	uint8_t Counter=0;

	if (OK == Error_State)
	{

		/*Wait till Bus is FREE (Not busy)*/
		while((GET_BIT(I2Cs[I2C_Configs->I2C_Num]->I2C_SR2,FLAGS_SR2_BUSY)));

		/*Enable I2C*/
		I2Cs[I2C_Configs->I2C_Num]->I2C_CR1 |= (1<<PE_BIT);

		/*Send Start Condition*/
		I2C_Send_Start_Condition(I2C_Configs->I2C_Num);

		/*Send Slave Address With Write Signal (LSB = 0)*/
		I2Cs[I2C_Configs->I2C_Num]->I2C_DR = (SlaveADD);

		/*Wait Till Address is Sent*/
		while (!(GET_BIT(I2Cs[I2C_Configs->I2C_Num]->I2C_SR1,FLAGS_SR1_ADDR)));

		/*Clear ADDR*/
		I2C_CLR_ADDR(I2C_Configs->I2C_Num) ;

		/*Send data Bytes till Buffer ends*/
		while (Counter <=data_Size)
		{
			/*wait till TxD Register empty*/
			while (!(GET_BIT(I2Cs[I2C_Configs->I2C_Num]->I2C_SR1,FLAGS_SR1_TXE)));

			/*Write data in DR*/
			I2Cs[I2C_Configs->I2C_Num]->I2C_DR = DataToSend[Counter++];

			/*wait till Byte transfer is finished*/
			while (!(GET_BIT(I2Cs[I2C_Configs->I2C_Num]->I2C_SR2,FLAGS_SR1_BTF)));
		}
		I2C_Send_Stop_Condition(I2C_NUMBER_1);
	}
	else {

		/*Error => Do Nothing*/

	}
	return Error_State ;
}
/*
 * @function 		:	I2C_Master_Receive
 * @brief			:	Receive (READ) data From Slave device
 * @param			:	I2C Configurations
 * @param			:	Variable to save Received data
 * @retval			:	Error State
 */
Error_State_t I2C_Master_Receive(const I2C_Configs_t * I2C_Configs , uint8_t *ReceivedData)
{
	Error_State_t	Error_State = OK;

	Error_State = I2C_CheckConfigs(I2C_Configs);

	if (OK == Error_State)
	{

		/*wait till RxNE Register Not empty*/
		while (!(GET_BIT(I2Cs[I2C_Configs->I2C_Num]->I2C_SR1,FLAGS_SR1_RXNE)));

		/*Read data in the DR*/
		*ReceivedData = I2Cs[I2C_Configs->I2C_Num]->I2C_DR;
	}
	else {

		/*Error => Do Nothing*/

	}
	return Error_State ;
}


/*
 * @function 		:	I2C_Enable_DMA
 * @brief			:	Enable DMA Stream
 * @param			:	I2C Number
 * @retval			:	Error State
 */
Error_State_t I2C_Enable_DMA(I2C_I2C_NUMBER_t	I2C_Num)
{
	Error_State_t Error_State = OK ;

	if ((I2C_Num >= I2C_NUMBER_1)&&(I2C_Num <= I2C_NUMBER_3))
	{
		/*Enable DMA Stream*/
		I2Cs[I2C_Num]->I2C_CR2 |= (1<<DMAEN_BIT);
	}
	else {
		Error_State = I2C_WRONG_I2C_NUMBER;
	}
	return Error_State	;
}

/*
 * @function 		:	I2C_Disable_DMA
 * @brief			:	Disable DMA Stream
 * @param			:	I2C Number
 * @retval			:	Error State
 */
Error_State_t I2C_Disable_DMA(I2C_I2C_NUMBER_t	I2C_Num)
{
	Error_State_t Error_State = OK ;

	if ((I2C_Num >= I2C_NUMBER_1)&&(I2C_Num <= I2C_NUMBER_3))
	{
		/*Disable DMA Stream*/
		I2Cs[I2C_Num]->I2C_CR2 &= (~(1<<DMAEN_BIT));
	}
	else {
		Error_State = I2C_WRONG_I2C_NUMBER;
	}
	return Error_State	;
}
/***************End of MAIN FUNCTIONS*******************/



/***************** STATIC FUNCTIONS ********************/

/*
 * @function 		:	I2C_Set_SCL_Frequency
 * @brief			:	Set SCL Frequency of the wanted I2C
 * @param			:	I2C Configurations
 * @retval			:	Error State
 */
static void I2C_Set_SCL_Frequency(const I2C_Configs_t * I2C_Configs)
{
	uint16_t CCR_Value =0;

	/*Calculate CCR Register Value*/
	CCR_Value = ((I2C_Configs->I2C_Pclk_MHZ) * (1000));
	CCR_Value /= (I2C_Configs->SCL_Frequency_KHZ);
	CCR_Value /= 2;

	I2Cs[I2C_Configs->I2C_Num]->I2C_CCR |= CCR_Value ;
}

/*
 * @function 		:	I2C_CheckConfigs
 * @brief			:	Check I2c Configurations
 * @param			:	I2C Configurations
 * @retval			:	Error State
 */
static Error_State_t I2C_CheckConfigs(const I2C_Configs_t * I2C_Configs)
{
	Error_State_t Error_State=OK;

	if (NULL != I2C_Configs)
	{

		if ( (I2C_Configs->ADD_Mode == ADDRESSING_MODE_10BITS) || (I2C_Configs->ADD_Mode == ADDRESSING_MODE_7BITS))
		{
			if ( (I2C_Configs->Chip_Address > 0) && (I2C_Configs->Chip_Address < 128))
			{
				if ((I2C_Configs->I2C_Mode == MASTER_MODE_STANDARD) || (I2C_Configs->I2C_Mode == MASTER_MODE_FAST))
				{
					if ((I2C_Configs->I2C_Num >= I2C_NUMBER_1) && (I2C_Configs->I2C_Num <= I2C_NUMBER_3))
					{
						if ((I2C_Configs->I2C_Pclk_MHZ < 45) && (I2C_Configs->I2C_Pclk_MHZ > 2 ))
						{
							if ((I2C_Configs->PEC_State == PACKET_ERR_CHECK_ENABLED) ||(I2C_Configs->PEC_State == PACKET_ERR_CHECK_DISABLED))
							{
								if (((I2C_Configs->SCL_Frequency_KHZ <=100) && (I2C_Configs->I2C_Mode == MASTER_MODE_STANDARD)) ||((I2C_Configs->SCL_Frequency_KHZ <=400) && (I2C_Configs->I2C_Mode == MASTER_MODE_FAST)))
								{
									if ((I2C_Configs->Stretch_state == CLK_STRETCH_ENABLED) ||(I2C_Configs->Stretch_state == CLK_STRETCH_DISABLED))
									{
										Error_State	=	OK;
									}
									else {
										Error_State	=	I2C_WRONG_CLK_STRETCH_STATE;
									}
								}
								else {
									Error_State	=	I2C_WRONG_SCL_FREQUENCY;
								}
							}
							else {
								Error_State	=	I2C_WRONG_PEC_STATE;
							}
						}
						else {
							Error_State	=	I2C_WRONG_PCLK;
						}
					}
					else {
						Error_State	=	I2C_WRONG_I2C_NUMBER;
					}
				}
				else {
					Error_State	=	I2C_WRONG_I2C_MASTER_MODE;
				}
			}
			else {
				Error_State	=	I2C_WRONG_ADDRESS_VALUE;
			}
		}
		else{
			Error_State = I2C_WRONG_ADDRESS_MODE_STATE;
		}
	}
	else {
		Error_State	=	Null_Pointer;
	}
	return Error_State ;
}


/*

@function         :    I2C_CLR_ADDR
@brief            :    Software sequence to Clear ADDR Flag
@param            :    I2C Number
@retval            :    VOID
 */
void I2C_CLR_ADDR(I2C_I2C_NUMBER_t I2C_Num)
{
	uint32_t Temp_Reg = 0 ;

	Temp_Reg = I2Cs[I2C_Num]->I2C_SR1;
	Temp_Reg = I2Cs[I2C_Num]->I2C_SR2;


}


/*

@function         :    I2C_SendAddressPacketMTransmitter
@brief            :    Send Address Packet
@param            :    Pointer to I2c Config Structure
@param            :    Address to Send
@retval           :    VOID
 */
void I2C_SendAddressPacketMTransmitter( I2C_Configs_t * Config , uint8_t Address )
{
	/*Wait till Bus is FREE (Not busy)*/
	while((GET_BIT(I2Cs[Config->I2C_Num]->I2C_SR2,FLAGS_SR2_BUSY)));

	/*Enable I2C*/
	I2Cs[Config->I2C_Num]->I2C_CR1 |= (1<<0);

	/*Send Start Condition*/
	I2C_Send_Start_Condition(Config->I2C_Num);

	/*Send Slave Address With Write Signal (LSB = 0)*/
	I2Cs[Config->I2C_Num]->I2C_DR = (Address);

	/*Wait Till Address is Sent*/
	while (!(GET_BIT(I2Cs[Config->I2C_Num]->I2C_SR1,FLAGS_SR1_ADDR)));

	/*Clear ADDR*/
	I2C_CLR_ADDR(Config->I2C_Num) ;
}


/*

@function         :    I2C_SendDataPacket
@brief            :    Send Data Packet
@param            :    Pointer to I2c Config Structure
@param            :    Data to Send
@retval           :    VOID
 */
void I2C_SendDataPacket(I2C_Configs_t * Config , uint8_t Data )
{
	/*wait till TxD Register empty*/
	while (!(GET_BIT(I2Cs[Config->I2C_Num]->I2C_SR1,FLAGS_SR1_TXE)));

	/*Write data in DR*/
	I2Cs[Config->I2C_Num]->I2C_DR = Data;

	/*wait till Byte transfer is finished*/
	while (!(GET_BIT(I2Cs[Config->I2C_Num]->I2C_SR2,FLAGS_SR1_BTF)));
}

/*

@function         :    I2C_SendAddressPacketMReceiver
@brief            :    Send Address Packet
@param            :    Pointer to I2c Config Structure
@param            :    Address to Send
@retval           :    VOID
 */
void I2C_SendAddressPacketMReceiver( I2C_Configs_t * Config , uint8_t Address )
{
	/*Send Start Condition*/
	I2C_Send_Start_Condition(Config->I2C_Num);

	/*Send Slave Address With Read Signal (LSB = 1)*/
	I2Cs[Config->I2C_Num]->I2C_DR = ( Address );

	/*Enable ACK*/
	SET_BIT(I2Cs[I2C_NUMBER_1]->I2C_CR1,10);

	/*Wait Till Address is Sent*/
	while (!(GET_BIT(I2Cs[I2C_NUMBER_1]->I2C_SR1,FLAGS_SR1_ADDR)));

	/*Clear ADDR*/
	I2C_CLR_ADDR(I2C_NUMBER_1) ;

}


/************** End of STATIC FUNCTIONS ****************/




/********************* IRQ HANDLERS ********************/

/****************** End OF IRQ HANDLERS ****************/
