/*
 ******************************************************************************
 * @file           : Service.c
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

/* ========================================================================= *
 *                            INCLUDES SECTION                               *
 * ========================================================================= */

#include <stdint.h>

#include "../../Library/ErrTypes.h"
#include "../../Library/STM32F446xx.h"

#include "../../Drivers/Inc/RCC_Interface.h"
#include "../../Drivers/Inc/GPIO_Interface.h"
#include "../../Drivers/Inc/NVIC_Interface.h"
#include "../../Drivers/Inc/SCB_Interface.h"
#include "../../Drivers/Inc/DMA_Interface.h"
#include "../../Drivers/Inc/EXTI_Interface.h"
#include "../../Drivers/Inc/SYSCFG_Interface.h"
#include "../../Drivers/Inc/I2C_Interface.h"
#include "../../Drivers/Inc/SPI_Interface.h"
#include "../../Drivers/Inc/UART_Interface.h"
#include "../../Drivers/Inc/SYSTICK_Interface.h"

#include "../../HAL/Inc/DS1307_Interface.h"

#include "../Inc/Service.h"

/* ========================================================================= *
 *                        GLOBAL VARIABLES SECTION                           *
 * ========================================================================= */

UART_Config_t * UART_CONFIG ;
DS1307_Config_t Date_Time_RTC ;

SPI_CONFIGS_t * SPI_CONFIG ;

I2C_Configs_t * I2C_CONFIG ;

UART_Interrupts_t UART2INTERRUPTS =
{
		.IDLE=UART_Disable ,.PE=UART_Disable ,
		.RXN=UART_Disable , .TC=UART_Disable,.TX=UART_Disable
};

UART_Config_t _USART2 = {

		.BaudRate 		= BaudRate_9600,
		.Direction		= RX_TX,
		.OverSampling	= OverSamplingBy16,
		.ParityState	= UART_Disable,
		.StopBits		= OneStopBit,
		.UART_ID		= UART_2,
		.WordLength		= _8Data,
		.Interrupts		= 	&UART2INTERRUPTS
};

I2C_Configs_t	_I2C1 ={
		.ADD_Mode			=	ADDRESSING_MODE_7BITS,
		.Chip_Address		=	10,
		.I2C_Mode			=	MASTER_MODE_STANDARD,
		.I2C_Num			=	I2C_NUMBER_1,
		.I2C_Pclk_MHZ		=	16,
		.PEC_State			=	PACKET_ERR_CHECK_DISABLED,
		.SCL_Frequency_KHZ	=	100,
		.Stretch_state		=	CLK_STRETCH_ENABLED
};
/* ========================================================================= *
 *                    FUNCTIONS IMPLEMENTATION SECTION                       *
 * ========================================================================= */

void WRONG_OptionChoosen ( void )
{
	/* If Wrong Option is Provided */
	SendNew_Line(  ) ;
	USART_SendStringPolling( UART_CONFIG->UART_ID , "  Wrong Option , Enter Option (1-3) ") ;
	/* delay & clear terminal */
	DELAY_500ms( ) ;
	Clear_Terminal( ) ;

}

void Check_IF_ContinueisNeeded( void )
{
	/* Received Char From User After Making His Functionality */
	uint8_t  Local_ReceivedChar = 0 ;

	/* Wait Until User Press 'y' */
	SendNew_Line( ) ;
	USART_SendStringPolling( UART_CONFIG->UART_ID , "[+] Do you want to continue? [y/n] " ) ;
	SendNew_Line( ) ;
	Local_ReceivedChar = UART_u16Receive(UART_CONFIG) ;
	UART_voidTransmitData(UART_CONFIG, Local_ReceivedChar ) ;


	/* If User Pressed n Or N or Any Other Character other Than y & Y
	 *  End the Program */
	if( Local_ReceivedChar == 'n' || Local_ReceivedChar == 'N'
			|| ( Local_ReceivedChar != 'y' && Local_ReceivedChar != 'Y') )
	{
		/* Stuck in a while Loop */
		Clear_Terminal( ) ;

		USART_SendStringPolling( UART_CONFIG->UART_ID , (char*)"┌──────────── •✧✧• ────────────┐\n") ;
		USART_SendStringPolling( UART_CONFIG->UART_ID , (char*)"-           Bye Bye :)         - \n") ;
		USART_SendStringPolling( UART_CONFIG->UART_ID , (char*)"└──────────── •✧✧• ────────────┘\n") ;

		while( 1 ) ;
	}
	else
	{
		Clear_Terminal( ) ;
	}

}

void Check_LoginInfo( uint8_t * ID_Ptr , uint8_t * Pass_Ptr , uint8_t TriesNumber  )
{
	/* Data To Send Via SPI if Number of Tries is Finished */
	uint8_t DATA_SENT_viaSPI = RED_LED_CODE ;

	/* Variable to Hold Return of Function Checking on ID & Inverted Pass */
	ID_PASS_EQUALITY_t ID_PASS_Relation = ID_NOEQUAL_INVERTED_PASS ;

	/* Check if ID Equals Password Inverted */
	ID_PASS_Relation = ID_Equal_InvertedPass( ID_Ptr , Pass_Ptr , NUM_OF_ID_PASS_DIGITS );

	/* Loop Terminates if Number of Tries is Finished */
	while( TriesNumber > 0 )
	{
		/* IF ID is Not Equal to Inverted Password */
		if( ID_PASS_Relation == ID_NOEQUAL_INVERTED_PASS  )
		{
			/* Decrement Number of Tries */
			TriesNumber -- ;

			/* Clear Putty Terminal */
			Clear_Terminal( ) ;

			/* If Number of Tries is Finished Terminate the While Loop */
			if( TriesNumber == 0 )
			{
				break ;
			}

			/* Take ID & Password again From User for The Next Try and Check on Them
			 * if ID is Equal To Password Inverted
			 */
			ID_PASS_Relation = TryAgain(ID_Ptr, Pass_Ptr) ;
		}
		/* IF ID is Equal to the Password Inverted Terminate The Loop */
		else if( ID_PASS_Relation == ID_EQUAL_INVERTED_PASS )
		{
			break ;
		}

	}

	Clear_Terminal( ) ;

	/* Number of Tries Finished */
	if( TriesNumber == 0 )
	{
		/* Send A Signal To Light Up the Red LED ON BluePill */
		SPI_Transmit_IT(SPI_CONFIG, &DATA_SENT_viaSPI , 1 , SPI_CallBackFunc ) ;
		/* Stuck in the Call Back Function */

	}
	else
	{
		/* Number of Tries Not Finished */
	}

}

ID_PASS_EQUALITY_t TryAgain ( uint8_t * ID_Ptr , uint8_t * Pass_Ptr )
{
	ID_PASS_EQUALITY_t ID_PASS_Relation = ID_NOEQUAL_INVERTED_PASS ;

	/* Receive ID From User */
	ID_Ptr = ID_Reception( ) ;

	/* Receive Password From User */
	Pass_Ptr = Pass_Reception( ) ;

	/* Check if ID is Equal to Inverted Password */
	ID_PASS_Relation = ID_Equal_InvertedPass( ID_Ptr , Pass_Ptr , NUM_OF_ID_PASS_DIGITS );

	return ID_PASS_Relation ;
}

OPTIONS_t Display_Menu( void )
{
	OPTIONS_t ChoosenOption = NO_OPTION ;

	USART_SendStringPolling( UART_CONFIG->UART_ID , (char*)"┌──────────── •✧✧• ────────────┐\n") ;
	USART_SendStringPolling( UART_CONFIG->UART_ID , (char*)"-  Welcome To My Clock System  - \n") ;
	USART_SendStringPolling( UART_CONFIG->UART_ID , (char*)"└──────────── •✧✧• ────────────┘\n") ;

	SendNew_Line( ) ;

	USART_SendStringPolling( UART_CONFIG->UART_ID , (char*)"  ========================================================================\n " ) ;
	USART_SendStringPolling( UART_CONFIG->UART_ID , (char*)"||                     Choose From The Following Menu :                 ||  " ) ;
	SendNew_Line( ) ;
	USART_SendStringPolling( UART_CONFIG->UART_ID , (char*)"  ========================================================================\n" ) ;

	/* Options */
	USART_SendStringPolling( UART_CONFIG->UART_ID , "1- Display Date & Time  \n" ) ;
	USART_SendStringPolling( UART_CONFIG->UART_ID , "2- Set Alarm            \n" ) ;
	USART_SendStringPolling( UART_CONFIG->UART_ID , "3- Set Date & Time      \n" ) ;
	USART_SendStringPolling( UART_CONFIG->UART_ID , "[+] select option (1-3) : " ) ;

	/* Receive Option From User */
	ChoosenOption = (uint8_t)UART_u16Receive(UART_CONFIG);

	/* Display Choosen Option to User */
	UART_voidTransmitData(UART_CONFIG,  ChoosenOption ) ;

	return ChoosenOption ;
}

void SendNew_Line( void )
{
	/* Send New Line In Terminal */
	USART_SendStringPolling( UART_CONFIG->UART_ID , (char*)"\n" ) ;
}

void Clear_Terminal( void )
{
	/* Clear Putty Terminal */
	USART_SendStringPolling(UART_CONFIG->UART_ID, CLEAR_TERMINAL );
}

ID_PASS_EQUALITY_t ID_Equal_InvertedPass( uint8_t * ID , uint8_t * Pass , uint8_t Size )
{
	/* Variable that Holds the Value if ID Equals Password Inverted */
	ID_PASS_EQUALITY_t ID_InvertedPass_Relation = ID_NOEQUAL_INVERTED_PASS ;

	/* Loop Counter */
	uint8_t Local_u8Counter = 0 ;

	/* Invert Password */
	uint8_t * InvertedPass = InvertPass( Pass , Size ) ;

	/* Check on Every Element if ID Equals Inverted Password*/
	for( Local_u8Counter = 0 ; Local_u8Counter < Size ; Local_u8Counter++ )
	{
		/* IF Digit From ID Equals Digit From Inverted Password */
		if( ID[ Local_u8Counter ] == InvertedPass[ Local_u8Counter ] )
		{
			ID_InvertedPass_Relation = ID_EQUAL_INVERTED_PASS ;
		}
		else
		{
			/* IF One Digit is Not Equal */
			ID_InvertedPass_Relation = ID_NOEQUAL_INVERTED_PASS ;
			break ;
		}
	}
	return ID_InvertedPass_Relation ;
}

uint8_t * InvertPass( uint8_t * Arr , uint8_t ArrSize )
{
	/* Loop Counter */
	uint8_t Local_u8Counter = 0 ;

	/* Array to Hold Password After Being Inverted */
	static uint8_t InvertedArr[ NUM_OF_ID_PASS_DIGITS ] = { 0 } ;

	/* Inverting Algorithm */
	for( Local_u8Counter = 0 ; Local_u8Counter < ArrSize ; Local_u8Counter++ )
	{
		InvertedArr[ Local_u8Counter ] = Arr[ ArrSize - Local_u8Counter - 1 ] ;
	}
	return InvertedArr ;
}

uint8_t * ID_Reception( void )
{
	/* Array to Store the ID */
	static uint8_t ID[ NUM_OF_ID_PASS_DIGITS ] = { 0 } ;

	/* For Loop Counter */
	uint8_t Local_u8Counter = 0 ;

	/* Received Digit From User via USART2 */
	uint8_t Local_u8ReceivedDigit = 0 ;

	/* Display to the User Text to Enter the ID */
	USART_SendStringPolling( UART_CONFIG->UART_ID  , (char*)"Enter 4 Digits ID : " ) ;

	/* Receive ID From User */
	for( Local_u8Counter = 0 ; Local_u8Counter < NUM_OF_ID_PASS_DIGITS ; Local_u8Counter++ )
	{
		/* Receive ID From User Digit By Digit */
		Local_u8ReceivedDigit = UART_u16Receive(UART_CONFIG) ;

		/* Fill ID Array */
		ID[ Local_u8Counter ] = Local_u8ReceivedDigit ;

		/* Transmit the Received Data to Visualize it on Putty */
		UART_voidTransmitData(UART_CONFIG, Local_u8ReceivedDigit) ;
	}

	/* Send New Line to the terminal */
	SendNew_Line( ) ;

	return ID ;
}

uint8_t * Pass_Reception( void )
{

	/* Array to Store the Password */
	static uint8_t Pass[ NUM_OF_ID_PASS_DIGITS ] = { 0 } ;

	/* For Loop Counter */
	uint8_t Local_u8Counter = 0 ;

	/* Received Digit From User via USART2 */
	uint8_t Local_u8ReceivedDigit = 0 ;

	/* Display to the User Text to Enter the ID */
	USART_SendStringPolling( UART_CONFIG->UART_ID  , (char*)"Enter Password : " ) ;

	/* Receive Pass From User */
	for( Local_u8Counter = 0 ; Local_u8Counter < NUM_OF_ID_PASS_DIGITS ; Local_u8Counter++ )
	{
		/* Receive Pass From User Digit By Digit */
		Local_u8ReceivedDigit = UART_u16Receive(UART_CONFIG) ;

		/* Fill ID Array */
		Pass[ Local_u8Counter ] = Local_u8ReceivedDigit ;

		/* Transmit the * to Make the Password Invisible */
		UART_voidTransmitData(UART_CONFIG, '*') ;
	}
	DELAY_500ms( ) ;

	/* Send New Line to the terminal */
	SendNew_Line( ) ;

	return Pass ;

}

void Clock_Init( void )
{
	/* Enable USART2 Clock */
	RCC_APB1EnableCLK( USART2EN ) ;

	/* Enable SPI1 Clock */
	RCC_APB2EnableCLK( SPI1EN ) ;

	/* Enable GPIO PortA Clock */
	RCC_AHB1EnableCLK( GPIOAEN ) ;

}

void Pins_Init( void )
{
	/* USART2 GPIO Pins Configuration Working in Full Duplex */
	GPIO_PinConfig_t  USART2_Pins [ NUM_OF_USART_PINS ] =
	{
			/* USART2 Tx Pin */
			{ .AltFunc = AF7 , .Mode = ALTERNATE_FUNCTION , .OutputType = PUSH_PULL , .PinNum = PIN2 , .Port = PORTA , .PullType = NO_PULL , .Speed = LOW_SPEED } ,
			/* USART2 Rx Pin */
			{ .AltFunc = AF7 , .Mode = ALTERNATE_FUNCTION , .OutputType = PUSH_PULL , .PinNum = PIN3 , .Port = PORTA , .PullType = NO_PULL , .Speed = LOW_SPEED }
	};

	/* Initializing USART2 Pins */
	GPIO_u8PinsInit( USART2_Pins , NUM_OF_USART_PINS ) ;

	/* SPI1 GPIO Pins Configuration Working in Simplex */
	GPIO_PinConfig_t SPI1_Pins [ NUM_OF_SPI_PINS ] =
	{
			/* SPI1 MOSI Pin */
			{ .AltFunc = AF5 , .Mode = ALTERNATE_FUNCTION , .OutputType = PUSH_PULL , .PinNum = PIN7 , .Port = PORTA , .PullType = NO_PULL , .Speed = LOW_SPEED } ,
			/* SPI1 SCK Pin */
			{ .AltFunc = AF5 , .Mode = ALTERNATE_FUNCTION , .OutputType = PUSH_PULL , .PinNum = PIN5 , .Port = PORTA , .PullType = NO_PULL , .Speed = LOW_SPEED } ,
			/* SPI1 NSS Pin */
			{ .AltFunc = AF5 , .Mode = ALTERNATE_FUNCTION , .OutputType = PUSH_PULL , .PinNum = PIN4 , .Port = PORTA , .PullType = PULL_UP , .Speed = LOW_SPEED }
	};

	/* Initializing SPI1 Pins */
	GPIO_u8PinsInit(SPI1_Pins, NUM_OF_SPI_PINS ) ;

	/*I2C GPIO Pins Configurations*/
	GPIO_PinConfig_t I2C1_Pins [NUM_OF_I2C_PINS] = {
			{
					.Mode 		= ALTERNATE_FUNCTION,
					.AltFunc 	= AF4,
					.OutputType = OPEN_DRAIN,
					.PullType	= PULL_UP,
					.Port		= PORTB,
					.PinNum		= PIN8,
					.Speed		= LOW_SPEED
			},
			{
					.Mode 		= ALTERNATE_FUNCTION,
					.AltFunc 	= AF4,
					.OutputType = OPEN_DRAIN,
					.PullType	= PULL_UP,
					.Port		= PORTB,
					.PinNum		= PIN9,
					.Speed		= LOW_SPEED
			}
	};

	/* Initializing I2C1 Pins */
	GPIO_u8PinsInit(I2C1_Pins, NUM_OF_I2C_PINS ) ;
}

void USART2_Init ( void )
{
	/* USART2 Interrupts Configuration */
	static UART_Interrupts_t USART2Interrupts =
	{
			.IDLE = UART_Disable , .PE = UART_Disable , .RXN = UART_Disable , .TC = UART_Disable , .TX = UART_Disable

	} ;

	/* USART2 Configuration */
	static UART_Config_t USART2Config =
	{
			.UART_ID = UART_2 ,	.BaudRate = BaudRate_9600 , .Direction = RX_TX , .OverSampling = OverSamplingBy16 ,
			.ParityState =  UART_Disable , .StopBits = OneStopBit , .WordLength = _8Data , .Interrupts = &USART2Interrupts
	};

	/* USART2 Initialization */
	UART_voidInit(&USART2Config) ;

	/* Initialize UART Struct Globally */
	UART_CONFIG = &USART2Config ;

}

void Interrupts_Init( void )
{
	NVIC_EnableIRQ( SPI1_IRQ ) ;
}

void SPI1_Init( void )
{
	/* SPI1 Configuration */
	static SPI_CONFIGS_t SPI1Config =
	{
			.BaudRate_Value = BAUDRATE_FpclkBY256 , .CRC_State = CRC_STATE_DISABLED ,
			.Chip_Mode = CHIP_MODE_MASTER , .Clock_Phase = CLOCK_PHASE_CAPTURE_FIRST , .Clock_Polarity = CLOCK_POLARITY_IDLE_LOW ,
			.Frame_Size = DATA_FRAME_SIZE_8BITS , .Frame_Type = FRAME_FORMAT_MSB_FIRST , .MultiMaster_State = MULTIMASTER_PROVIDED , .SPI_Num = SPI_NUMBER1 ,
			.Transfer_Mode = TRANSFER_MODE_FULL_DUPLEX
	};

	/* SPI1 Initialization */
	SPI_Init( &SPI1Config ) ;

	/* Initialize SPI Struct Globally */
	SPI_CONFIG = &SPI1Config ;
}

void I2C1_Init( void ){

	/*I2C1 Configurations*/
	I2C_Configs_t	_I2C1 ={
			.ADD_Mode			=	ADDRESSING_MODE_7BITS,
			.Chip_Address		=	10,
			.I2C_Mode			=	MASTER_MODE_STANDARD,
			.I2C_Num			=	I2C_NUMBER_1,
			.I2C_Pclk_MHZ		=	16,
			.PEC_State			=	PACKET_ERR_CHECK_DISABLED,
			.SCL_Frequency_KHZ	=	100,
			.Stretch_state		=	CLK_STRETCH_ENABLED
	};

	/* I2C1 Initialization */
	I2C_Init(&_I2C1);

	I2C_CONFIG	=	&_I2C1	;

}

void ShutDown_Sequence( void )
{
	/* This Funciton is Called When Number of Tries of User is Finished & RED_LED_CODE is Transmitted to the
	 * Blue Pill Board
	 */
	USART_SendStringPolling( UART_CONFIG->UART_ID , (char*)"┌──────────── •✧✧• ────────────┐\n") ;
	USART_SendStringPolling( UART_CONFIG->UART_ID , (char*)"-     System Shut Down         - \n") ;
	USART_SendStringPolling( UART_CONFIG->UART_ID , (char*)"└──────────── •✧✧• ────────────┘\n") ;

	while(1) ;
}
Error_State_t ReadDateTime_FromPC(void)
{
	Error_State_t Error_State = OK;

	uint8_t Date_Time_USART[CALENDER_FORMAT] = {0};

	static uint8_t First_Time_Flag 	=	FIRST_TIME;

	if (First_Time_Flag == FIRST_TIME)
	{
		USART_SendStringPolling(UART_2,  "\nWELCOME To Set Date and Time Mode\n");
		First_Time_Flag		=	NOT_FIRST_TIME	;
	}
	USART_SendStringPolling(UART_2,  "Enter the Date And time in the Following Form\n");
	USART_SendStringPolling(UART_2,  "yy-mm-dd (First 3 Letters of Day Name) HH:MM:SS\n");
	for (uint8_t Local_Counter=0	;	Local_Counter<23	;	Local_Counter++)
	{
		Date_Time_USART[Local_Counter] = UART_u16Receive(&_USART2);
		UART_voidTransmitData(&_USART2, Date_Time_USART[Local_Counter]);
	}
	/*Find calender Values to be send to RTC*/
	Calculate_Calender(&Date_Time_RTC , Date_Time_USART);

	/*Estimate the Day Name*/
	Date_Time_RTC.Day	= FindDay(Date_Time_USART);

	/*Check the given Calender*/
	if (	OK	==	Check_Calender(&Date_Time_RTC)	)
	{
		Error_State	=OK;	
	}
	else {
		Error_State	=NOK;
	}

	return Error_State ;
}
static DS1307_DAYS_t FindDay(uint8_t * Calender)
{
	DS1307_DAYS_t Day = 0;

	/*Check the 3 Letters*/
	if ((('S'==Calender[FIRST_LETTER_OF_DAY]) || ('s'==Calender[FIRST_LETTER_OF_DAY]))&&(('A'==Calender[SECOND_LETTER_OF_DAY]) || ('a'==Calender[SECOND_LETTER_OF_DAY]))&&(('T'==Calender[THIRD_LETTER_OF_DAY]) || ('t'==Calender[THIRD_LETTER_OF_DAY])))
	{
		/*First 3 letters are SAT so the day is Saturday*/
		Day = DS1307_SATURDAY;
	}
	else if ((('S'==Calender[FIRST_LETTER_OF_DAY]) || ('s'==Calender[FIRST_LETTER_OF_DAY]))&&(('U'==Calender[SECOND_LETTER_OF_DAY]) || ('u'==Calender[SECOND_LETTER_OF_DAY]))&&(('N'==Calender[THIRD_LETTER_OF_DAY]) || ('n'==Calender[THIRD_LETTER_OF_DAY])))
	{
		/*First 3 letters are SUN so the day is Sunday*/
		Day = DS1307_SUNDAY;
	}
	else if ((('M'==Calender[FIRST_LETTER_OF_DAY]) || ('m'==Calender[FIRST_LETTER_OF_DAY]))&&(('O'==Calender[SECOND_LETTER_OF_DAY]) || ('o'==Calender[SECOND_LETTER_OF_DAY]))&&(('N'==Calender[THIRD_LETTER_OF_DAY]) || ('n'==Calender[THIRD_LETTER_OF_DAY])))
	{
		/*First 3 letters are MON so the day is Monday*/
		Day = DS1307_MONDAY;
	}

	else if ((('T'==Calender[FIRST_LETTER_OF_DAY]) || ('t'==Calender[FIRST_LETTER_OF_DAY]))&&(('U'==Calender[SECOND_LETTER_OF_DAY]) || ('u'==Calender[SECOND_LETTER_OF_DAY]))&&(('E'==Calender[THIRD_LETTER_OF_DAY]) || ('e'==Calender[THIRD_LETTER_OF_DAY])))
	{
		/*First 3 letters are Tue so the day is Tuesday*/
		Day = DS1307_TUESDAY;
	}
	else if ((('W'==Calender[FIRST_LETTER_OF_DAY]) || ('w'==Calender[FIRST_LETTER_OF_DAY]))&&(('E'==Calender[SECOND_LETTER_OF_DAY]) || ('e'==Calender[SECOND_LETTER_OF_DAY]))&&(('D'==Calender[THIRD_LETTER_OF_DAY]) || ('d'==Calender[THIRD_LETTER_OF_DAY])))
	{
		/*First 3 letters are wed so the day is Wednesday*/
		Day = DS1307_WEDNESDAY;
	}
	else if ((('T'==Calender[FIRST_LETTER_OF_DAY]) || ('t'==Calender[FIRST_LETTER_OF_DAY]))&&(('H'==Calender[SECOND_LETTER_OF_DAY]) || ('h'==Calender[SECOND_LETTER_OF_DAY]))&&(('U'==Calender[THIRD_LETTER_OF_DAY]) || ('u'==Calender[THIRD_LETTER_OF_DAY])))
	{
		/*First 3 letters are THU so the day is Thrusday*/
		Day = DS1307_THURSDAY;
	}
	else if ((('F'==Calender[FIRST_LETTER_OF_DAY]) || ('f'==Calender[FIRST_LETTER_OF_DAY]))&&(('R'==Calender[SECOND_LETTER_OF_DAY]) || ('r'==Calender[SECOND_LETTER_OF_DAY]))&&(('I'==Calender[THIRD_LETTER_OF_DAY]) || ('i'==Calender[THIRD_LETTER_OF_DAY])))
	{
		/*First 3 letters are FRI so the day is Friday*/
		Day = DS1307_FRIDAY;
	}
	else {
		/*Wrong Day*/
		Day	=	WRONG_DAY;
	}
	return Day ;
}
static void Calculate_Calender(DS1307_Config_t * Date_Time_To_RTC,uint8_t * Date_Time_From_USART)
{
	/*Calculate The Date*/
	Date_Time_To_RTC->Year 		= (((Date_Time_From_USART[0]-ZERO_ASCII) *10)+(Date_Time_From_USART[1]-ZERO_ASCII));
	Date_Time_To_RTC->Month 	= (((Date_Time_From_USART[3]-ZERO_ASCII) *10)+(Date_Time_From_USART[4]-ZERO_ASCII));
	Date_Time_To_RTC->Date 		= (((Date_Time_From_USART[6]-ZERO_ASCII) *10)+(Date_Time_From_USART[7]-ZERO_ASCII));

	/*Calculate the Time*/
	Date_Time_To_RTC->Hours 	= (((Date_Time_From_USART[15]-ZERO_ASCII) *10)+(Date_Time_From_USART[16]-ZERO_ASCII));
	Date_Time_To_RTC->Minutes	= (((Date_Time_From_USART[18]-ZERO_ASCII) *10)+(Date_Time_From_USART[19]-ZERO_ASCII));
	Date_Time_To_RTC->Seconds 	= (((Date_Time_From_USART[21]-ZERO_ASCII) *10)+(Date_Time_From_USART[22]-ZERO_ASCII));

}
static Error_State_t Check_Calender(DS1307_Config_t * Date_Time_To_RTC)
{
	Error_State_t Error_State = OK;
	if (	(Date_Time_To_RTC->Date>MAX_DATE) 	|| 	(Date_Time_To_RTC->Day==WRONG_DAY)
			||(Date_Time_To_RTC->Hours>MAX_HOURS)	||	(Date_Time_To_RTC->Minutes>MAX_MINUTES)
			||(Date_Time_To_RTC->Month >MAX_MONTH) || 	(Date_Time_To_RTC->Seconds>MAX_SECONDS)
			||(Date_Time_To_RTC->Year>MAX_YEAR)	)
	{
		Error_State	=	NOK;
	}
	return Error_State ;
}


