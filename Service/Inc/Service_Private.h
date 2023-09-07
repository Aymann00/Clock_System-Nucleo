/*
 ******************************************************************************
 * @file           : Service_Private.h
 * @Author         : MOHAMMEDs & HEMA
 * @brief          : Service Private Header file
 * @Date           : Aug 30, 2023
 ******************************************************************************
 */
#ifndef _SERVICE_PRIVATE_H_
#define _SERVICE_PRIVATE_H_

/* ========================================================================= *
 *                         PRIVATE MACROS SECTION                            *
 * ========================================================================= */

#define FIRST_TIME 1u
#define NOT_FIRST_TIME 0u

#define CALENDER_FORMAT 23u

#define CLEAR_TERMINAL "\033\143"

#define DELAY_1s()                      \
    for (uint16_t n = 1000; n > 0; n--) \
        for (uint16_t y = 0; y < 3195; y++)

#define DELAY_500ms()                  \
    for (uint16_t n = 500; n > 0; n--) \
        for (uint16_t i = 0; i < 3195; i++)

#define RED_LED_CODE 0x39
#define DISPLAY_CODE 0x41
#define GREEN_LED_CODE 0x44
#define ALARMCODE 100

#define FIRST_LETTER_OF_DAY 10u
#define SECOND_LETTER_OF_DAY 11u
#define THIRD_LETTER_OF_DAY 12u
#define WRONG_DAY 8u

#define MAX_DATE 31u
#define MAX_HOURS 23u
#define MAX_MINUTES 59u
#define MAX_SECONDS 59u
#define MAX_MONTH 12u
#define MAX_YEAR 99u

#define Filling                                                                                              \
    {                                                                                                        \
        {0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF}, { 0xFF, 0xFF, 0xFF } \
    }

/* ========================================================================= *
 *                         PRIVATE ENUMS SECTION                             *
 * ========================================================================= */

typedef enum
{
    NUM_OF_USART_PINS = 0x02,
    NUM_OF_SPI_PINS = 0x03,
    NUM_OF_ID_PASS_DIGITS = 0x04,
    NUM_OF_I2C_PINS = 0x02

} MAGIC_NUMBERS_t;

typedef enum
{
    ID_EQUAL_INVERTED_PASS = 0x00,
    ID_NOEQUAL_INVERTED_PASS = 0x01

} ID_PASS_EQUALITY_t;

typedef enum
{
    Equal,
    NotEqual

} Equality_t;

/* ========================================================================= *
 *                         PRIVATE FUNCTIONS SECTION                         *
 * ========================================================================= */

/** ============================================================================
 * @fn 				: TryAgain
 *
 * @brief 			: This Function is Called When User Enters Wrong ID & Password ,
 *                    It Asks User to Enter ID & Password Again
 *
 * @param[in]		: uint8_t *ID_Ptr > Pointer to ID Array
 *                    uint8_t *Pass_Ptr > Pointer to Password Array
 *
 * @return 			: ID_PASS_EQUALITY_t > ID_EQUAL_INVERTED_PASS if ID is Equal to Password Inverted
 *                    ID_NOEQUAL_INVERTED_PASS if ID is Not Equal to Password Inverted
 *
 * @note			: This Function is Called in Check_LoginInfo Function( Private Function )
 * ============================================================================
 */
static ID_PASS_EQUALITY_t TryAgain(uint8_t *ID_Ptr, uint8_t *Pass_Ptr);

/** ============================================================================
 * @fn 				: ID_Equal_InvertedPass
 *
 * @brief 			: This Function is Called to Check if ID is Equal to Password Inverted or Not
 *
 * @param[in]		: uint8_t *ID > Pointer to ID Array
 *     			      uint8_t *Pass > Pointer to Password Array
 *    		 		  uint8_t Size > Size of ID & Password Arrays
 *
 * @return 			: ID_PASS_EQUALITY_t > ( ID_EQUAL_INVERTED_PASS ) if ID is Equal to Password Inverted
 *  				( ID_NOEQUAL_INVERTED_PASS ) if ID is Not Equal to Password Inverted
 *
 * @note			: This Function is Called in TryAgain Function( Private Function )
 *
 * ============================================================================
 */
static ID_PASS_EQUALITY_t ID_Equal_InvertedPass(uint8_t *ID, uint8_t *Pass, uint8_t Size);

/** ============================================================================
 * @fn 				: InvertPass
 *
 * @brief 			: This Function is Called Used to Invert the Password Entered by User
 *
 * @param[in]		: uint8_t *Arr > Pointer to ID Array
 * 					  uint8_t ArrSize > Size of ID Array
 *
 * @return 			: uint8_t * > Pointer to Inverted Password Array
 *
 * @note			: This Function is Called in ID_Equal_InvertedPass Function( Private Function )
 * ============================================================================
 */
static uint8_t *InvertPass(uint8_t *Arr, uint8_t ArrSize);

/*=======================================================================================
 * @fn		 		:	FindDay
 * @brief			:	Find The Day given by the user
 * @param			:	Pointer to the Calender Array received from user
 * @retval			:	The Day received from the user
 * ======================================================================================*/
static DS1307_DAYS_t FindDay(uint8_t *Calender);

/*=======================================================================================
 * @fn		 		:	Calculate_Calender
 * @brief			:	Translate The Calender given by the user from ASCII to Decimal values
 * 						And save it in a structure which will be sent to RTC.
 * @param			:	Pointer to the Calender struct which will be sent to RTC
 * @param			:	Pointer to the Calender Array received from user
 * @retval			:	void
 * ======================================================================================*/
static void Calculate_Calender(DS1307_Config_t *Date_Time_To_RTC, uint8_t *Date_Time_From_USART);

/*=======================================================================================
 * @fn		 		:	Check_Calender
 * @brief			:	Check The Calender given by the user
 * @param			:	Pointer to the Calender struct which will be sent to RTC
 * @retval			:	Error State
 * ======================================================================================*/
static Error_State_t Check_Calender(DS1307_Config_t *Date_Time_To_RTC);

#endif /* _SERVICE_PRIVATE_H_ */
