/*
 ******************************************************************************
 * @file           : Service.h
 * @Author         : Mohammed Ayman Shalaby
 * @brief          : Main program body
 * @Date           : Aug 28, 2023
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 Ayman.
 * All rights reserved.
 *
 ******************************************************************************
 */
#ifndef INC_SERVICE_H_
#define INC_SERVICE_H_

/* ========================================================================= *
 *                         PRIVATE MACROS SECTION                            *
 * ========================================================================= */

#define CLEAR_TERMINAL    "\033\143"

#define DELAY_1s()          for (uint16_t n = 1000 ; n > 0 ; n-- )\
		                    for (uint16_t y = 0 ; y < 3195 ; y++ )

#define DELAY_500ms()    for (uint16_t n = 500 ; n > 0 ; n-- )\
						 for (uint16_t i = 0 ; i < 3195 ; i++ )

#define ZERO_ASCII      48

#define RED_LED_CODE   0x39

/* ========================================================================= *
 *                         PRIVATE ENUMS SECTION                             *
 * ========================================================================= */

typedef enum
{
	NUM_OF_USART_PINS = 0x02 , NUM_OF_SPI_PINS = 0x03 , NUM_OF_ID_PASS_DIGITS = 0x04 ,
	NUM_OF_TRIES = 0x03

} MAGIG_NUMBERS_t ;

typedef enum
{
	ID_EQUAL_INVERTED_PASS = 0x00 , ID_NOEQUAL_INVERTED_PASS = 0x01
} ID_PASS_EQUALITY_t;

typedef enum
{
	NO_OPTION = 0x00 , DISPLAY_OPTION = ( 0x01 + ZERO_ASCII ) , SET_ALARM_OPTION = ( 0x02 + ZERO_ASCII ) , SET_DATE_TIME_OPTION = ( 0x03 + ZERO_ASCII ) ,

} OPTIONS_t ;

/* ========================================================================= *
 *                      FUNCTIONS PROTOTYPES SECTION                         *
 * ========================================================================= */
void Clock_Init( void ) ;

void Pins_Init( void ) ;

void USART2_Init ( void ) ;

void SPI1_Init( void ) ;

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

void Interrupts_Init( void ) ;

void Check_IF_ContinueisNeeded( void );

void WRONG_OptionChoosen ( void ) ;

void ShutDown_Sequence( void ) ;

#endif /* INC_SERVICE_H_ */
