/*******************************************************************************
 * Filename              :   DS1307_Interface.h
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
#ifndef DS1307_INCLUDE_DS1307_INTERFACE_H_
#define DS1307_INCLUDE_DS1307_INTERFACE_H_

/* =================================================================================== *
 *                                   PRIVATE ENUMS                                     *
 * =================================================================================== */
typedef enum
{
	DS1307_SUNDAY = 1 , DS1307_MONDAY , DS1307_TUESDAY , DS1307_WEDNESDAY , DS1307_THURSDAY , DS1307_FRIDAY ,
	DS1307_SATURDAY
}DS1307_DAYS_t;


typedef struct
{
    uint8_t Seconds ;
    uint8_t Minutes ;
    uint8_t Hours ;
    DS1307_DAYS_t Day ;
    uint8_t Date ;
    uint8_t Month ;
    uint8_t Year ;

} DS1307_Config_t;

/* =================================================================================== *
 *                                   PRIVATE MACROS                                    *
 * =================================================================================== */
#define DS1307_FIRST_LOC 0x00 /* Address of First Location in DS1307 Registers */
#define DS1307_WRITE_ARR_SIZE 0x08
#define DS1307_RECEIVE_ARR_SIZE 0x07

#define DS1307_SLAVE_ADDRESS_WRITE 0b11010000
#define DS1307_SLAVE_ADDRESS_READ  0b11010001

/* =================================================================================== *
 *                                   PROTOTYPES                                        *
 * =================================================================================== */


/**
 * @fn     : RTC_DateTimetoBCD
 * @brief  : This Function Returns Array of Values to Be Set inside the DS1307 Registers
 * @param  : Config => Pointer to Config Structure of DS1307 Config in it Time & Date
 * @return : Array of Values Needs to be Passed to DS1307 to Set Time
 */
uint8_t * DS1307_DateTimetoBCD( DS1307_Config_t * Config );

/**
 * @fn     : RTC_BCDToDateTime
 * @brief  : This Function Returns A Pointer to Structure that Holds Date & Time Values Normal
 * @param  : ReadingsArray => Pointer to Readings Array Read From DS1307 in BCD & Needed to Be Converted to Normal Integers
 * @return : Pointer to Structure of Type DS1307_Config_t
 */
DS1307_Config_t * DS1307_BCDToDateTime( uint8_t * ReadingsArray );

/**
 * @fn     : DS1307_WriteDateTime
 * @brief  : This Function Writes Data Into RTC Module to Set Date & Time
 * @param  : I2CConfig => Pointer to Structure of I2C Initialization
 * @param  : Config => Pointer to Structure of DS1307 Configuration of Setting Date & Time
 * @return : void
 * @note   : Before Calling This Function You Should Initialize I2C Pins as ( AF , Open Drain , Pull Up ) Through GPIO
 *           & Initialize a Certain I2C By Using ( I2C_Init )
 */
void DS1307_WriteDateTime(I2C_Configs_t * I2CConfig , DS1307_Config_t * Config  );

/**
 * @fn     : DS1307_ReadDateTime
 * @brief  : This Function Reads Data From RTC Module
 * @param  : I2CConfig => Pointer to Structure of I2C Initialization
 * @param  : ReturnDateTime => Pointer to Structure of DS1307 Configuration of to Return Current Date & Time
 * @return : DS_Config_t Pointer
 */

DS1307_Config_t * DS1307_ReadDateTime( I2C_Configs_t * I2CConfig );

#endif /* DS1307_INCLUDE_DS1307_INTERFACE_H_ */
