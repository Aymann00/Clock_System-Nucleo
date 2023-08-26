/*******************************************************************************
 * Filename              :   DS1307_Program.c
 * Author                :   Mohamemd Waleed Gad
 * Origin Date           :   Aug 24, 2023
 * Version               :   1.0.0
 * Compiler              :   GCC ARM Embedded Toolchain
 * Target                :
 * Notes                 :   None
 **
 *******************************************************************************/
/************************ SOURCE REVISION LOG *********************************
 *
 *    Date    Version   Author             Description
 *  14/10/20   1.0.0   Mohamemd Waleed   Initial Release.
 *
 *******************************************************************************/

/* ================================================================= *
 *                            INCLUDES                               *
 * ================================================================= */
#include <stdint.h>

#include "ErrTypes.h"
#include "STM32F446xx.h"

#include "../Inc/I2C_Interface.h"

#include "../Inc/DS1307_Interface.h"
#include "../Inc/DS1307_Private.h"

/* ================================================================= *
 *                            IMPLEMENTATION                         *
 * ================================================================= */


extern I2C_REG_t *I2Cs[3];
/**
 * @fn     : RTC_DateTimetoBCD
 * @brief  : This Function Returns Array of Values to Be Set inside the DS1307 Registers
 * @param  : Config => Pointer to Config Structure of DS1307 Config in it Time & Date
 * @return : Array of Values Needs to be Passed to DS1307 to Set Time
 */
uint8_t * DS1307_DateTimetoBCD( DS1307_Config_t * Config )
{
	/* Array to Send to RTC DS1307 via I2C */
	static uint8_t ConfigArr[ DS1307_WRITE_ARR_SIZE ]={0};

	uint8_t Local_u8Variable = 0 ;

	uint8_t Local_u8Digit = 0 ;

	uint8_t Local_u8Counter = 0 ;

	/* To Set Pointer to The First Location */
	ConfigArr[ DS1307_FIRST_LOC ] = 0x00 ;

	/* Calculate Seconds */
	Local_u8Variable = Config->Seconds ;

	for( Local_u8Counter = 0 ; Local_u8Counter < 2  ; Local_u8Counter++)
	{
		Local_u8Digit = Local_u8Variable % 10 ;
		Local_u8Variable = Local_u8Variable / 10 ;

		ConfigArr[ DS1307_SEC_ADD+1 ] |= Local_u8Digit << ( Local_u8Counter * 4 ) ;
	}

	/* Calculate Minutes */
	Local_u8Variable = Config->Minutes ;

	for( Local_u8Counter = 0 ; Local_u8Counter < 2 ; Local_u8Counter++ )
	{
		Local_u8Digit = Local_u8Variable % 10 ;
		Local_u8Variable = Local_u8Variable / 10 ;

		ConfigArr[ DS1307_MIN_ADD+1 ] |= Local_u8Digit << (Local_u8Counter * 4) ;
	}

	/* Calculating Hours : By Default 24 Hours Mode */

	Local_u8Variable = Config->Hours ;

	for( Local_u8Counter = 0 ; Local_u8Counter < 2 ; Local_u8Counter++ )
	{
		Local_u8Digit = Local_u8Variable %10 ;

		Local_u8Variable = Local_u8Variable / 10 ;

		ConfigArr[ DS1307_HR_ADD+1 ] |= Local_u8Digit <<(Local_u8Counter*4);
	}

	/* Day Calcultion */

	ConfigArr[ DS1307_DAY_ADD+1 ] = Config->Day ;

	/* Date Calculation */

	Local_u8Variable = Config->Date ;

	for( Local_u8Counter = 0 ; Local_u8Counter < 2 ; Local_u8Counter ++ )
	{
		Local_u8Digit = Local_u8Variable %10 ;

		Local_u8Variable = Local_u8Variable / 10 ;

		ConfigArr[ DS1307_DATE_ADD+1 ] |= Local_u8Digit <<(Local_u8Counter *4);
	}

	/* Month Calculation */

	Local_u8Variable = Config->Month ;

	for( Local_u8Counter = 0 ; Local_u8Counter < 2 ; Local_u8Counter++ )
	{
		Local_u8Digit = Local_u8Variable % 10 ;

		Local_u8Variable = Local_u8Variable / 10 ;

		ConfigArr[ DS1307_MONTH_ADD+1 ] |= Local_u8Digit <<(Local_u8Counter*4);
	}

	/* Year */

	Local_u8Variable = Config->Year ;

	for( Local_u8Counter = 0 ; Local_u8Counter < 2 ; Local_u8Counter++ )
	{
		Local_u8Digit = Local_u8Variable % 10 ;

		Local_u8Variable = Local_u8Variable / 10 ;

		ConfigArr[ DS1307_YEAR_ADD+1 ] |= Local_u8Digit <<(Local_u8Counter*4);
	}
	return ConfigArr ;
}


/**
 * @fn     : RTC_BCDToDateTime
 * @brief  : This Function Returns A Pointer to Structure that Holds Date & Time Values Normal
 * @param  : ReadingsArray => Pointer to Readings Array Read From DS1307 in BCD & Needed to Be Converted to Normal Integers
 * @return : Pointer to Structure of Type DS1307_Config_t
 */
DS1307_Config_t * DS1307_BCDToDateTime( uint8_t * ReadingsArray )
{
	static DS1307_Config_t DS1307Readings ;

	uint8_t Local_u8Variable = 0 ;
	uint8_t LSB = 0 ;
	uint8_t MSB = 0 ;

	/* Seconds Readings */
	Local_u8Variable = ReadingsArray[ DS1307_SEC_ADD ] ;

	LSB = Local_u8Variable & ( 0x0F ) ;
	MSB = ( Local_u8Variable & ( 0xF0 ) ) >> 4 ;

	DS1307Readings.Seconds = ( MSB * 10 ) + LSB ;

	/* Minutes */
	Local_u8Variable = ReadingsArray[ DS1307_MIN_ADD ] ;

	LSB = Local_u8Variable & ( 0x0F ) ;
	MSB = ( Local_u8Variable & ( 0xF0 ) ) >> 4 ;

	DS1307Readings.Minutes = ( MSB * 10 ) + LSB ;

	/* HOurs */

	Local_u8Variable = ReadingsArray[ DS1307_HR_ADD ] ;

	LSB = Local_u8Variable & ( 0x0F ) ;
	MSB = ( Local_u8Variable & ( 0xF0 ) ) >>4 ;

	DS1307Readings.Hours = ( MSB * 10 ) + LSB ;

	/* Day */

	DS1307Readings.Day = ReadingsArray[ DS1307_DAY_ADD ] ;

	/* Date */

	Local_u8Variable = ReadingsArray[ DS1307_DATE_ADD ] ;

	LSB = Local_u8Variable & ( 0x0F ) ;
	MSB = ( Local_u8Variable & ( 0xF0 ) )>>4 ;

	DS1307Readings.Date = ( MSB * 10 ) + LSB ;

	/* Month */
	Local_u8Variable = ReadingsArray[ DS1307_MONTH_ADD ] ;

	LSB = Local_u8Variable & ( 0x0F ) ;
	MSB = ( Local_u8Variable & ( 0xF0 ) )>>4 ;

	DS1307Readings.Month = ( MSB * 10 ) + LSB ;

	/* Year */
	Local_u8Variable = ReadingsArray[ DS1307_YEAR_ADD ] ;

	LSB = Local_u8Variable & ( 0x0F ) ;
	MSB = ( Local_u8Variable & ( 0xF0 ) ) >> 4 ;

	DS1307Readings.Year = ( MSB * 10 ) + LSB ;

	return &DS1307Readings ;
}

/**
 * @fn     : DS1307_WriteDateTime
 * @brief  : This Function Writes Data Into RTC Module to Set Date & Time
 * @param  : I2CConfig => Pointer to Structure of I2C Initialization
 * @param  : Config => Pointer to Structure of DS1307 Configuration of Setting Date & Time
 * @return : void
 * @note   : Before Calling This Function You Should Initialize I2C Pins as ( AF , Open Drain , Pull Up ) Through GPIO
 *           & Initialize a Certain I2C By Using ( I2C_Init )
 */
void DS1307_WriteDateTime(I2C_Configs_t * I2CConfig , DS1307_Config_t * DS1307_Config  )
{
	/* To Store In it BCD Values to Be Written to DS1307 Registers */
	static uint8_t *  PTR = NULL ;

	/* BCD Values Prepare them to Be Transferred to DS1307 RTC */
	PTR = 	DS1307_DateTimetoBCD(DS1307_Config);

	/*Send the RTC Values*/
	I2C_Master_Transmit(I2CConfig, DS1307_SLAVE_ADDRESS_WRITE, PTR ,8) ;

}

/**
 * @fn     : DS1307_ReadDateTime
 * @brief  : This Function Reads Data From RTC Module
 * @param  : I2CConfig => Pointer to Structure of I2C Initialization
 * @param  : ReturnDateTime => Pointer to Structure of DS1307 Configuration of to Return Current Date & Time
 * @return : DS_Config_t Pointer
 */

DS1307_Config_t * DS1307_ReadDateTime( I2C_Configs_t * I2CConfig )
{
	uint8_t Local_u8Counter = 0 ;

	static uint8_t ReceiveArr[ DS1307_RECEIVE_ARR_SIZE ] = { 0 } ;

	/* Send Address Packet with Write */
	I2C_SendAddressPacketMTransmitter(I2CConfig, DS1307_SLAVE_ADDRESS_WRITE) ;

	/* Send Word Address To Reset the Pointer */
	I2C_SendDataPacket(I2CConfig, DS1307_FIRST_LOC) ;

	/* Repeated Start & Send Address Packet with Read */
	I2C_SendAddressPacketMReceiver( I2CConfig , DS1307_SLAVE_ADDRESS_READ ) ;

	for( Local_u8Counter = 0 ; Local_u8Counter < DS1307_RECEIVE_ARR_SIZE ; Local_u8Counter++ )
	{
		I2C_Master_Receive(I2CConfig, &ReceiveArr[ Local_u8Counter ] ) ;

	}

	I2C_Send_Stop_Condition(I2CConfig->I2C_Num) ;


	return  DS1307_BCDToDateTime(ReceiveArr) ;

}
