/*
 * Service.c
 *
 *  Created on: Aug 28, 2023
 *      Author: hp
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

#include "../../Service/Inc/Service.h"

/* ========================================================================= *
 *                        GLOBAL VARIABLES SECTION                           *
 * ========================================================================= */

DS1307_Config_t Date_Time_RTC ;

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

void Clock_Init(void)
{
	RCC_AHB1EnableCLK(GPIOAEN);

	RCC_AHB1EnableCLK(GPIOBEN);

	RCC_APB1EnableCLK(USART2EN);

	RCC_APB1EnableCLK(I2C1EN);

}



void Pins_Init(void)
{
	GPIO_PinConfig_t COM_PINS[4]={
			{
					.Mode 		= ALTERNATE_FUNCTION,
					.AltFunc 	= AF7,
					.OutputType = PUSH_PULL,
					.PullType	= NO_PULL,
					.Port		= PORTA,
					.PinNum		= PIN2,
					.Speed		= LOW_SPEED
			},
			{
					.Mode 		= ALTERNATE_FUNCTION,
					.AltFunc 	= AF7,
					.OutputType = PUSH_PULL,
					.PullType	= NO_PULL,
					.Port		= PORTA,
					.PinNum		= PIN3,
					.Speed		= LOW_SPEED

			},
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

	GPIO_u8PinsInit(COM_PINS, 4);

}

void Peripherals_Init(void)
{
	UART_voidInit(&_USART2);
	I2C_Init(&_I2C1);
}

Error_State_t ReadDateTime_FromPC(void)
{

	Error_State_t Error_State = OK;

	uint8_t Date_Time_USART[CALENDER_FORMAT] = {0};

	static uint8_t First_Time_Flag 	=	FIRST_TIME;

	if (First_Time_Flag == FIRST_TIME)
	{
		USART_SendStringPolling(UART_2,  "WELCOME To Set Date and Time Mode\n");

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
