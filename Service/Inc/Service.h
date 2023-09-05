/*
 ******************************************************************************
 * @file           : Service.h
 * @Author         : Mohammed Ayman Shalaby
 * @brief          : Main program body
 * @Date           : Aug 30, 2023
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 Ayman.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
#ifndef INC_SERVICE_H_
#define INC_SERVICE_H_

/* ========================================================================= *
 *                         PRIVATE MACROS SECTION                            *
 * ========================================================================= */

#define WRONG_DAY				8u
#define FIRST_TIME				1u
#define NOT_FIRST_TIME			0u
#define CALENDER_FORMAT			23u
#define CLEAR_TERMINAL    "\033\143"

#define DELAY_1s()          for (uint16_t n = 1000 ; n > 0 ; n-- )\
		for (uint16_t y = 0 ; y < 3195 ; y++ )

#define DELAY_500ms()    for (uint16_t n = 500 ; n > 0 ; n-- )\
		for (uint16_t i = 0 ; i < 3195 ; i++ )

#define ZERO_ASCII      48

#define RED_LED_CODE   0x39
#define DISPLAY_CODE   0x41
#define FIRST_LETTER_OF_DAY		10u
#define SECOND_LETTER_OF_DAY	11u
#define THIRD_LETTER_OF_DAY		12u


#define MAX_DATE				31u
#define MAX_HOURS				23u
#define MAX_MINUTES				59u
#define MAX_SECONDS				59u
#define MAX_MONTH				12u
#define MAX_YEAR				99u

#define ALARMCODE               100


#define Filling                 {{0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF}, { 0xFF, 0xFF, 0xFF }}

/* ========================================================================= *
 *                         PRIVATE ENUMS SECTION                             *
 * ========================================================================= */

typedef enum
{
	NUM_OF_USART_PINS = 0x02 , NUM_OF_SPI_PINS = 0x03 , NUM_OF_ID_PASS_DIGITS = 0x04 ,
	NUM_OF_TRIES = 0x03	,	NUM_OF_I2C_PINS = 0x02
} MAGIG_NUMBERS_t ;
typedef enum
{
	ID_EQUAL_INVERTED_PASS = 0x00 , ID_NOEQUAL_INVERTED_PASS = 0x01
} ID_PASS_EQUALITY_t;
typedef enum
{
	NO_OPTION = 0x00 , DISPLAY_OPTION = ( 0x01 + ZERO_ASCII ) , SET_ALARM_OPTION = ( 0x02 + ZERO_ASCII ) , SET_DATE_TIME_OPTION = ( 0x03 + ZERO_ASCII ) ,

} OPTIONS_t ;

typedef enum
{
	Equal,
	NotEqual
} Equality_t;
/* ========================================================================= *
 *                      FUNCTIONS PROTOTYPES SECTION                         *
 * ========================================================================= */
void Clock_Init( void ) ;

void Pins_Init( void ) ;

void USART2_Init ( void ) ;

void SPI1_Init( void ) ;

void I2C1_Init( void );

uint8_t * ID_Reception( void ) ;

uint8_t * Pass_Reception( void ) ;

uint8_t * InvertPass( uint8_t * Arr , uint8_t ArrSize ) ;

ID_PASS_EQUALITY_t ID_Equal_InvertedPass( uint8_t * ID , uint8_t * Pass , uint8_t Size ) ;

void Clear_Terminal( void ) ;

OPTIONS_t Display_Menu( void ) ;

void SendNew_Line( void ) ;

ID_PASS_EQUALITY_t TryAgain ( uint8_t * ID_Ptr , uint8_t * Pass_Ptr ) ;

void SPI_CallBackFunc( void ) ;

void Check_LoginInfo( uint8_t * ID_Ptr , uint8_t * Pass_Ptr , uint8_t TriesNumber  ) ;

Error_State_t ReadDateTime_FromPC(void) ;

/*==============================================================================================================================================
 *@fn      : void InterruptsInit (void)
 *@brief  :  This Function Is Responsible For Initializing The Interrupts
 *@retval void :
 *==============================================================================================================================================*/
void Interrupts_Init( void ) ;

static DS1307_DAYS_t FindDay(uint8_t * Calender);

void Check_IF_ContinueisNeeded( void );

static void Calculate_Calender(DS1307_Config_t * Date_Time_To_RTC,uint8_t * Date_Time_From_USART);

void WRONG_OptionChoosen ( void ) ;

static Error_State_t Check_Calender(DS1307_Config_t * Date_Time_To_RTC);

void ShutDown_Sequence( void ) ;

void Reading_Time(void);

void Transmit_Time(void);

void SPI_CALL_BACK(void);

/*==============================================================================================================================================
 *@fn      : void SetAlarm()
 *@brief  :  This Function Is Responsible For Setting The Alarm Time
 *@retval void :
 *==============================================================================================================================================*/
void SetAlarm();

/*==============================================================================================================================================
 *@fn      : void SPI1_ISR()
 *@brief  :  This Function Is The ISR For The SPI1 Interrupt
 *@retval void :
 *==============================================================================================================================================*/
void SPI1_ISR();



/*==============================================================================================================================================
 *@fn      : void SysTickPeriodicISR()
 *@brief  :  This Function Is The ISR For The SYSTICK Interrupt
 *@retval void :
 *==============================================================================================================================================*/
void SysTickPeriodicISR();

/*==============================================================================================================================================
 *@fn      : void CalcAlarm(uint8_t AlarmNumber)
 *@brief  :  This Function Is Responsible For Calculating The Alarm Time And Storing It In The Global Array
 *@paramter[in]  : uint8_t AlarmNumber : The Number Of The Alarm To Be Set
 *@retval void :
 *==============================================================================================================================================*/
void CalcAlarm(uint8_t AlarmNumber);

/*==============================================================================================================================================
 *@fn      :  void CompTime()
 *@brief  :   This Function Is Responsible For Comparing The Current Time With The Alarm Time And Send The Alarm Number To The Blue Pill If They Are Equal
 *@retval void :
 *==============================================================================================================================================*/
void CompTime();
#endif /* INC_SERVICE_H_ */
