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

SPI_CONFIGS_t * SPI_CONFIG ;

/* ========================================================================= *
 *                    FUNCTIONS IMPLEMENTATION SECTION                       *
 * ========================================================================= */

void WRONG_OptionChoosen ( void )
{
	/* If Wrong Option is Provided */
	SendNew_Line(  ) ;
	USART_SendStringPolling( UART_CONFIG->UART_ID , "  Wrong Option , Enter Option (1-3) ") ;
	/* delay & clear terminal */
	DELAY_1s( ) ;
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
		SPI_Transmit_IT(SPI_CONFIG, (uint16_t*) RED_LED_CODE , 1 , SPI_CallBackFunc ) ;
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
			.BaudRate_Value = BAUDRATE_FpclkBY8 , .CRC_State = CRC_STATE_DISABLED ,
			.Chip_Mode = CHIP_MODE_MASTER , .Clock_Phase = CLOCK_PHASE_CAPTURE_FIRST , .Clock_Polarity = CLOCK_POLARITY_IDLE_LOW ,
			.Frame_Size = DATA_FRAME_SIZE_8BITS , .Frame_Type = FRAME_FORMAT_MSB_FIRST , .MultiMaster_State = MULTIMASTER_PROVIDED , .SPI_Num = SPI_NUMBER1 ,
			.Transfer_Mode = TRANSFER_MODE_FULL_DUPLEX
	};

	/* SPI1 Initialization */
	SPI_Init( &SPI1Config ) ;

	/* Initialize SPI Struct Globally */
	SPI_CONFIG = &SPI1Config ;
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


