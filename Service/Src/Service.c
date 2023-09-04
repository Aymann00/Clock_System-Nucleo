/*******************************************************************************
* Filename              :   Service.c
* Author                :   Mohamemd Waleed Gad
* Origin Date           :   Aug 29, 2023
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

#include <stdint.h>

#include "../../Library/ErrTypes.h"
#include "../../Library/STM32F446xx.h"

#include "../Inc/Service.h"
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


/* ========================================================================= *
 *                        GLOBAL VARIABLES SECTION                           *
 * ========================================================================= */

/* Alarm Time Array  */
uint8_t AlarmTime[5][3] = Filling;

/* Pointer to I2C1 Configuration Structure */
I2C_Configs_t *I2C1_PTR;

/* Pointer to UART2 Configuration Structure */
UART_Config_t *UART2_PTR;

/* Pointer to SPI1 Configuration Structure */
SPI_CONFIGS_t *SPI1_PTR;

/* Alarm Name Array */
uint8_t AlarmName[30] = {0};

/* Counter To Store The Alarm Name Length */
uint8_t AlarmNameCounter = 0;

/* ========================================================================= *
 *                    FUNCTIONS IMPLEMENTATION SECTION                       *
 * ========================================================================= */
/*==============================================================================================================================================
 *@fn      :  void ClockInit()
 *@brief  :  This Function Is Responsible For Initializing The Clocks For The Used Peripherals
 *@retval void :
 *==============================================================================================================================================*/
void ClockInit()
{

	/* Enable Clocks For GPIOA*/
	RCC_AHB1EnableCLK(GPIOAEN);

	/* Enable Clocks For GPIOB*/
	RCC_AHB1EnableCLK(GPIOBEN);

	/* Enable Clocks For USART2*/
	RCC_APB1EnableCLK(USART2EN);

	/* Enable Clocks For I2C1 */
	RCC_APB1EnableCLK(I2C1EN);

	/* Enable Clocks For SPI11 */
	RCC_APB2EnableCLK(SPI1EN);

}
/*==============================================================================================================================================
 *@fn      :  void PinInit()
 *@brief  :   This Function Is Responsible For Initializing The Pins For The Used Peripherals
 *@retval void :
 *==============================================================================================================================================*/
void PinInit()
{
	/* Configure UART Pins Through GPIO */
	GPIO_PinConfig_t PA3_UARTRX =
	{
			.Port = PORTA,
			.PinNum = PIN3,
			.Mode = ALTERNATE_FUNCTION,
			.AltFunc = AF7,
	};

	GPIO_PinConfig_t PA2_UARTTX =
	{
			.Port = PORTA,
			.PinNum = PIN2,
			.Mode = ALTERNATE_FUNCTION,
			.AltFunc = AF7,
	};

	/* Configure I2C Pins Through GPIO */
	GPIO_PinConfig_t PB8_SCLPIN =
	{
			.Mode = ALTERNATE_FUNCTION, .AltFunc = AF4, .OutputType = OPEN_DRAIN, .PinNum = PIN8, .Port = PORTB, .PullType = PULL_UP, .Speed = LOW_SPEED};
	GPIO_PinConfig_t PB9_SDAPIN =
	{
			.Mode = ALTERNATE_FUNCTION, .AltFunc = AF4, .OutputType = OPEN_DRAIN, .PinNum = PIN9, .Port = PORTB, .PullType = PULL_UP, .Speed = LOW_SPEED};

	GPIO_PinConfig_t PB6_EXTI =
	{
			.Port = PORTB,
			.PinNum = PIN6,
			.Mode = OUTPUT,
			.OutputType = PUSH_PULL,
			.PullType=PULL_UP,
	};

	/* SPI1 GPIO Pins Configuration Working in Simplex */
	GPIO_PinConfig_t SPI1_Pins[3] =
	{
			/* SPI1 MOSI Pin */
			{.AltFunc = AF5, .Mode = ALTERNATE_FUNCTION, .OutputType = PUSH_PULL, .PinNum = PIN7, .Port = PORTA, .PullType = NO_PULL, .Speed = LOW_SPEED},
			/* SPI1 SCK Pin */
			{.AltFunc = AF5, .Mode = ALTERNATE_FUNCTION, .OutputType = PUSH_PULL, .PinNum = PIN5, .Port = PORTA, .PullType = NO_PULL, .Speed = LOW_SPEED},
			/* SPI1 NSS Pin */
			{.AltFunc = AF5, .Mode = ALTERNATE_FUNCTION, .OutputType = PUSH_PULL, .PinNum = PIN4, .Port = PORTA, .PullType = PULL_UP, .Speed = LOW_SPEED}};

	/* Initialize Pin PB6 For EXTI */
	GPIO_u8PinInit(&PB6_EXTI);

	GPIO_u8SetPinValue(PORTB, PIN6, PIN_HIGH);

	/* Initialize Pin PA3 For UART RX */
	GPIO_u8PinInit(&PA3_UARTRX);

	/* Initialize Pin PA2 For UART TX */
	GPIO_u8PinInit(&PA2_UARTTX);

	/* Initialize I2C1 Pins */
	GPIO_u8PinInit(&PB8_SCLPIN);
	GPIO_u8PinInit(&PB9_SDAPIN);

	/* Initializing SPI1 Pins */
	GPIO_u8PinsInit(SPI1_Pins, 3);
}
/*==============================================================================================================================================
 *@fn      : void CalcAlarm(uint8_t AlarmNumber)
 *@brief  :  This Function Is Responsible For Calculating The Alarm Time And Storing It In The Global Array
 *@paramter[in]  : uint8_t AlarmNumber : The Number Of The Alarm To Be Set
 *@retval void :
 *==============================================================================================================================================*/
void CalcAlarm(uint8_t AlarmNumber)
{
	/* Variable To Store The Received Data From UART */
	uint8_t RecTemp[8] = {0};

	/* Receive The Alarm Time From UART And Store It In The Array */
	UART_voidRecieveBuffer(UART2_PTR, RecTemp, 8);

	/* Store The Received Data In The Global Array */
	AlarmTime[AlarmNumber - 48][0] = (RecTemp[0] - 48) * 10 + (RecTemp[1] - 48);
	AlarmTime[AlarmNumber - 48][1] = (RecTemp[3] - 48) * 10 + (RecTemp[4] - 48);
	AlarmTime[AlarmNumber - 48][2] = (RecTemp[6] - 48) * 10 + (RecTemp[7] - 48);
}
/*==============================================================================================================================================
 *@fn      :  void CompTime()
 *@brief  :   This Function Is Responsible For Comparing The Current Time With The Alarm Time And Send The Alarm Number To The Blue Pill If They Are Equal
 *@retval void :
 *==============================================================================================================================================*/
void CompTime()
{
	/* Variable To Store The Received Time From The RTC */
	DS1307_Config_t *RecievedTime;

	/* Reading The Current Time From The RTC */
	RecievedTime = DS1307_ReadDateTime(I2C1_PTR);

	/* Array To Store The Current Time From The Current Time Recieved From The RTC */
	uint8_t CurrentTime[3] = {
			RecievedTime->Hours, RecievedTime->Minutes, RecievedTime->Seconds};

	/* Variable To Loop On The Alarm Time Array  Counter1 For Looping On Alarm Number & Counter2 For Looping On The Alarm Time */
	uint8_t Counter1, Counter2 = 0;

	/* Variable To Check The Equality Between The Current Time And The Alarm Time */
	Equality_t EqualityCheck = NotEqual;

	/* Loop On The Alarm Number */
	for (Counter1 = 0; Counter1 < 5; Counter1++)
	{
		/* Check If The Alarm Time Is Not Empty */
		if (AlarmTime[Counter1][0] != 0xFF)
		{
			/* Loop On The Alarm Time */
			for (Counter2 = 0; Counter2 < 3; Counter2++)
			{
				/* Check If The Current Time Is Equal To The Alarm Time */
				if (AlarmTime[Counter1][Counter2] != CurrentTime[Counter2])
				{
					/* If Not Equal Break The Loop And Continue To The Next Alarm Number */
					EqualityCheck = NotEqual;
					break;
				}
				/* If Equal Continue The Loop */
				else
				{
					/* If The Loop Reached The End Of The Alarm Time Array Then The Current Time Is Equal To The Alarm Time */
					EqualityCheck = Equal;
				}
			}
		}
		/* If The Alarm Time Is Empty Continue To The Next Alarm Number */
		else
		{
			continue;
		}
		/* If The Current Time Is Equal To The Alarm Time Send The Alarm Number To The Blue Pill */
		if (EqualityCheck == Equal)
		{
			/* Variable To Store The Alarm Number */
			AlarmName[0] = ++Counter1;
			/* Send The Alarm Number To The Blue Pill */
			SPI_Transmit_IT(SPI1_PTR, AlarmName, 30, &SPI1_ISR);
		}
	}
}
/*==============================================================================================================================================
 *@fn      : void SPI1_ISR()
 *@brief  :  This Function Is The ISR For The SPI1 Interrupt
 *@retval void :
 *==============================================================================================================================================*/
void SPI1_ISR()
{

	/* Notify The Blue Pill That The Alarm Is Fired */
	GPIO_u8SetPinValue(PORTB, PIN6, PIN_LOW);

}
/*==============================================================================================================================================
 *@fn      : void SetAlarm()
 *@brief  :  This Function Is Responsible For Setting The Alarm Time
 *@retval void :
 *==============================================================================================================================================*/
void SetAlarm()
{
	/* Variable To Store The Alarm Number */
	uint8_t ChooseNum = 0;

	/* Ask The User To Choose The Alarm Number */
	USART_SendStringPolling(UART_2, "Please Choose Alarm Number From ( 1 ~ 5 )\nYour Choice: ");

	/* Receive The Alarm Number From The User */
	ChooseNum = UART_u16Receive(UART2_PTR);

	/* Ask The User To Enter The Alarm Name */
	USART_SendStringPolling(UART_2, "Please Enter Alarm Name: ");

	/* Loop To Receive The Alarm Name From The User Until The User Press Enter */
	for (AlarmNameCounter = 1; AlarmNameCounter <30 ; AlarmNameCounter++)
	{

		/* Receive The Alarm Name From The User */
		AlarmName[AlarmNameCounter] = UART_u16Receive(UART2_PTR);
		if ( AlarmName[AlarmNameCounter] == 13 )
		{
			break;
		}

	}

	/* Check If The Alarm Number Is In The Range */
	if (ChooseNum > '0' && ChooseNum < '6')
	{
		USART_SendStringPolling(UART_2, "Please Enter Your Alarm in this sequence xx:xx:xx\n");
		CalcAlarm(ChooseNum - 1);
	}
	/* If The Alarm Number Is Not In The Range Send Wrong Choice To The User */
	else
	{
		/* Send Wrong Choice To The User */
		USART_SendStringPolling(UART_2, " Wrong Choice ");
		/* Ask The User To Choose The Alarm Number Again */
		SetAlarm();
	}
}

/*==============================================================================================================================================
 *@fn      : void PeripheralInit()
 *@brief  :  This Function Is Responsible For Initializing The Peripherals
 *@retval void :
 *==============================================================================================================================================*/
void PeripheralInit()
{
	/* UART2 Interrupts Configuration */
	static UART_Interrupts_t UART2INTERRUPTS = {.IDLE = UART_Disable, .PE = UART_Disable, .RXN = UART_Disable, .TC = UART_Disable, .TX = UART_Disable};

	static UART_Config_t UART2_Config = {.UART_ID = UART_2, .Direction = RX_TX, .BaudRate = BaudRate_9600, .OverSampling = OverSamplingBy16, .ParityState = UART_Disable, .StopBits = OneStopBit, .WordLength = _8Data, .Interrupts = &UART2INTERRUPTS};

	/* I2C Configuration */
	static I2C_Configs_t I2C1_Config =
	{
			.ADD_Mode = ADDRESSING_MODE_7BITS, .I2C_Mode = MASTER_MODE_STANDARD, .I2C_Num = I2C_NUMBER_1, .I2C_Pclk_MHZ = 16, .SCL_Frequency_KHZ = 100, .Chip_Address = 34, .PEC_State = PACKET_ERR_CHECK_DISABLED, .Stretch_state = CLK_STRETCH_ENABLED};

	/* Initialize I2C Struct Globally */
	I2C1_PTR = &I2C1_Config;

	/* Initialize UART Struct Globally */
	UART2_PTR = &UART2_Config;

	/* Initialize UART2 */
	UART_voidInit(&UART2_Config);

	/* Initialize I2C1 */
	I2C_Init(&I2C1_Config);

	/* SPI1 Configuration */
	static SPI_CONFIGS_t SPI1Config =
	{
			.BaudRate_Value = BAUDRATE_FpclkBY256, .CRC_State = CRC_STATE_DISABLED, .Chip_Mode = CHIP_MODE_MASTER, .Clock_Phase = CLOCK_PHASE_CAPTURE_FIRST, .Clock_Polarity = CLOCK_POLARITY_IDLE_LOW, .Frame_Size = DATA_FRAME_SIZE_8BITS, .Frame_Type = FRAME_FORMAT_MSB_FIRST, .MultiMaster_State = MULTIMASTER_PROVIDED, .SPI_Num = SPI_NUMBER1, .Transfer_Mode = TRANSFER_MODE_FULL_DUPLEX};

	/* SPI1 Initialization */
	SPI_Init(&SPI1Config);

	/* Initialize SPI Struct Globally */
	SPI1_PTR = &SPI1Config;
}
void InterruptsInit (void)
{

	/* Set 2 Group Priorities & 8 Sub Priorities*/
	SCB_VoidSetPriorityGroup(GP_2_SP_8);

	/* Set SPI to Group Priority Zero*/
	NVIC_SetPriority(SPI1_IRQ, 0);

	/* Set SYSTICK to Group Priority One*/
	SCB_VoidSetCorePriority(SYSTICK_FAULT, (1 << 7));

	NVIC_EnableIRQ(SPI1_IRQ);

}
