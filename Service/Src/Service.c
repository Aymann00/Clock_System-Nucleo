/*
 ******************************************************************************
 * @file           : Service.c
 * @Author         : Mohammed Ayman Shalaby
 * @brief          : Main program body
 * @Date           : Aug 30, 2023
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

uint8_t ReadingArr[7]={ 0 } ;


DS1307_Config_t *ReadingStruct ;


GPIO_PinConfig_t I2C1_Pins [2] = {
		{
				.Mode         = ALTERNATE_FUNCTION,
				.AltFunc     = AF4,
				.OutputType = OPEN_DRAIN,
				.PullType    = PULL_UP,
				.Port        = PORTB,
				.PinNum        = PIN8,
				.Speed        = LOW_SPEED
		},
		{
				.Mode         = ALTERNATE_FUNCTION,
				.AltFunc     = AF4,
				.OutputType = OPEN_DRAIN,
				.PullType    = PULL_UP,
				.Port        = PORTB,
				.PinNum        = PIN9,
				.Speed        = LOW_SPEED
		}
};



SPI_CONFIGS_t SPI1Config =
{
		.BaudRate_Value = BAUDRATE_FpclkBY256 , .CRC_State = CRC_STATE_DISABLED ,
		.Chip_Mode = CHIP_MODE_MASTER , .Clock_Phase = CLOCK_PHASE_CAPTURE_FIRST , .Clock_Polarity = CLOCK_POLARITY_IDLE_LOW ,
		.Frame_Size = DATA_FRAME_SIZE_8BITS , .Frame_Type = FRAME_FORMAT_MSB_FIRST , .MultiMaster_State = MULTIMASTER_PROVIDED , .SPI_Num = SPI_NUMBER1 ,
		.Transfer_Mode = TRANSFER_MODE_FULL_DUPLEX
};

GPIO_PinConfig_t SPI_MOSI ={.Port= PORTA, .PinNum= PIN7, .OutputType=PUSH_PULL,  .Mode=ALTERNATE_FUNCTION, .PullType=NO_PULL ,.AltFunc=AF5 };
GPIO_PinConfig_t SPI_SCK  ={.Port= PORTA, .PinNum= PIN5, .OutputType=PUSH_PULL,  .Mode=ALTERNATE_FUNCTION, .PullType=NO_PULL ,.AltFunc=AF5 };
GPIO_PinConfig_t SPI_NSS  ={.Port= PORTA, .PinNum= PIN4, .OutputType=PUSH_PULL,  .Mode=ALTERNATE_FUNCTION, .PullType=PULL_UP ,.AltFunc=AF5 };



I2C_Configs_t I2C1_Config =
{
		.ADD_Mode = ADDRESSING_MODE_7BITS , .I2C_Mode = MASTER_MODE_STANDARD , .I2C_Num = I2C_NUMBER_1 , .I2C_Pclk_MHZ = 16 ,
		.SCL_Frequency_KHZ = 100 , .Chip_Address = 34 , .PEC_State = PACKET_ERR_CHECK_DISABLED ,
		.Stretch_state = CLK_STRETCH_ENABLED
};

/* ========================================================================= *
 *                    FUNCTIONS IMPLEMENTATION SECTION                       *
 * ========================================================================= */


 void Reading_Time(void)
{
	 /* Read Date & Time */
	 ReadingStruct = DS1307_ReadDateTime(&I2C1_Config);

	 /* Convert Reading Struct into Reading Array */
	 ReadingArr[0] = ReadingStruct->Seconds;
	 ReadingArr[1] = ReadingStruct->Minutes;
	 ReadingArr[2] = ReadingStruct->Hours;
	 ReadingArr[3] = ReadingStruct->Day;
	 ReadingArr[4] = ReadingStruct->Month;
	 ReadingArr[5] = ReadingStruct->Year;
	 ReadingArr[6] = ReadingStruct->Date;
}

 void Transmit_Time(void)
 {
	 SPI_Transmit_IT(&SPI1Config, ReadingArr , 7 , SPI_CALL_BACK ) ;
 }

 void Clock_Init( void )
 {
	 RCC_APB2EnableCLK(SPI1EN);
	 RCC_APB1EnableCLK(I2C1EN);
	 RCC_AHB1EnableCLK(GPIOAEN);
	 RCC_AHB1EnableCLK(GPIOBEN);
 }

 void Pins_Periph_Init( void )
 {
	 GPIO_u8PinInit(&SPI_MOSI);
	 GPIO_u8PinInit(&SPI_SCK);
	 GPIO_u8PinInit(&SPI_NSS);

	 /* Initializing I2C1 Pins */
	 GPIO_u8PinsInit(I2C1_Pins, 2 ) ;

	 SPI_Init(&SPI1Config);

	 I2C_Init(&I2C1_Config);
 }
