/*******************************************************************************
 * Filename              :   UART_Program.c
 * Author                :   Mohamemd Waleed Gad
 * Origin Date           :   Aug 16, 2023
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

/*==============================================================================================================================================
 * Includes
 *==============================================================================================================================================*/
#include <stdint.h>
#include "../../../LIB/STM32F446.h"
#include "../../../LIB/ErrorType.h"
#include "../Include/UART_Interface.h"
#include "../Include/UART_Private.h"

/*==============================================================================================================================================
 * GLOBAL VARIABLES SECTION
 *==============================================================================================================================================*/
/* Array Of Pointers To UART Registers */
static USART_Reg_t *UART[UARTNUMBER] = {USART1, USART2, USART3, UART4, UART5, USART6};

static void (*UART_PTR_TO_FUNC[UARTNUMBER][FlagsNumber])(void) = {NULL};

static uint8_t UART_RecieveBufferFlag[UARTNUMBER] = {0};

static uint8_t UART_RecieveBufferSize[UARTNUMBER] = {0};

static uint16_t *UART_RecievedBufferIT[UARTNUMBER] = {0};

static uint16_t *UART_RecievedBuffer[UARTNUMBER] = {0};
/*==============================================================================================================================================
 * FUNCTION DEFINITION SECTION
 *==============================================================================================================================================*/
/*==============================================================================================================================================
 *@fn      : USART_voidReadFlag
 *@brief  : This function is used to read the status of a specific flag
 *@paramter[in]  : UART_ID_t UART_ID : ID of the UART peripheral
 *paramter[in]   : USART_Flag_t Flag : Flag to be read
 *@retval uint8_t : Status of the flag
 *==============================================================================================================================================*/
uint8_t UART_ReadFlag(UART_ID_t UART_ID, USART_Flag_t Flag)
{
	/* Read Flag & return it */
	return ((UART[UART_ID]->SR >> Flag) & 1);
}
/*==============================================================================================================================================
 *@fn      : UART_voidInit
 *@brief  : This function is used to initialize the UART peripheral
 *@paramter[in]  : UART_Config_t *UART_Config
 *@retval void
 *==============================================================================================================================================*/
void UART_voidInit(UART_Config_t *UART_Config)
{
	/* Enabling UART */
	UART[UART_Config->UART_ID]->CR1 |= (UART_Enable << CR1_UE);

	/* Setting Baud Rate */
	UART_SettingBaudRate(UART_Config);

	/* Setting Word Lenght */
	UART[UART_Config->UART_ID]->CR1 &= ~(OneBitMasking << CR1_M);
	UART[UART_Config->UART_ID]->CR1 |= ((UART_Config->WordLength) << CR1_M);

	/* Setting Parity State */
	UART[UART_Config->UART_ID]->CR1 &= ~(OneBitMasking << CR1_PCE);
	UART[UART_Config->UART_ID]->CR1 |= ((UART_Config->ParityState) << CR1_PCE);

	/* Parity Selection */
	UART[UART_Config->UART_ID]->CR1 &= ~(OneBitMasking << CR1_PS);
	UART[UART_Config->UART_ID]->CR1 |= ((UART_Config->Parity) << CR1_PS);

	/* Setting Stop Bits*/
	UART[UART_Config->UART_ID]->CR2 &= ~(TwoBitMasking << CR2_STOP);
	UART[UART_Config->UART_ID]->CR2 |= ((UART_Config->StopBits) << CR2_STOP);

	/* Setting Over Sampling */
	UART[UART_Config->UART_ID]->CR2 &= ~(OneBitMasking << CR1_OVER8);
	UART[UART_Config->UART_ID]->CR2 |= ((UART_Config->OverSampling) << CR1_OVER8);

	/* Setting UART Interrupts */
	UART_voidInterruptConfig(UART_Config);
	/* Setting Direction */
	UART[UART_Config->UART_ID]->CR1 &= ~(TwoBitMasking << CR1_RE);
	UART[UART_Config->UART_ID]->CR1 |= ((UART_Config->Direction) << CR1_RE);
}

/*==============================================================================================================================================
 *@fn    : UART_SettingBaudRate
 *@brief  : This function is used to set the baud rate of the UART peripheral
 *@paramter[in]  : UART_Config_t *UART_Config
 *@retval void
 *==============================================================================================================================================*/
void UART_SettingBaudRate(UART_Config_t *UART_Config)
{
	float USART_DIV = 0;

	uint32_t DIV_Fraction = 0;

	UART[UART_Config->UART_ID]->BRR = 0;

	/* BaudRate Equation */
	USART_DIV = Clock_Freq / (float)(8U * (2U - UART_Config->OverSampling) * UART_Config->BaudRate);
	USART_DIV *= 1000UL;

	/* Set the Mantissa Part */
	UART[UART_Config->UART_ID]->BRR |= ((uint32_t)USART_DIV / 1000UL) << BRR_DIV_Mantissa;

	/* Calculte the Fraction */
	DIV_Fraction = (uint32_t)USART_DIV % 1000UL;

	DIV_Fraction = DIV_Fraction * (16 / (UART_Config->OverSampling + 1));

	/* get Round */
	DIV_Fraction += 500UL;

	UART[UART_Config->UART_ID]->BRR |= (uint32_t)(DIV_Fraction / 1000UL) << BRR_DIV_Fraction;
}
/*==============================================================================================================================================
 *@fn      : UART_voidInterruptConfig
 *@brief  : This function is used to configure the interrupts of the UART peripheral
 *@paramter[in]  :  UART_Config_t *UART_Config
 *@retval void
 *==============================================================================================================================================*/
void UART_voidInterruptConfig(UART_Config_t *UART_Config)
{
	/* Clearing Interrupt Bits */
	UART[UART_Config->UART_ID]->CR1 &= ~(FiveBitMasking << CR1_IDLEIE);
	/* Setting Interrupt Bits */
	/* Setting IDLE Interrupt */
	UART[UART_Config->UART_ID]->CR1 |= ((UART_Config->Interrupts->IDLE) << CR1_IDLEIE);
	/* Setting RXNE Interrupt */
	UART[UART_Config->UART_ID]->CR1 |= ((UART_Config->Interrupts->RXN) << CR1_RXNEIE);
	/* Setting TC Interrupt */
	UART[UART_Config->UART_ID]->CR1 |= ((UART_Config->Interrupts->TC) << CR1_TCIE);
	/* Setting TXE Interrupt */
	UART[UART_Config->UART_ID]->CR1 |= ((UART_Config->Interrupts->TX) << CR1_TXEIE);
	/* Setting PE Interrupt */
	UART[UART_Config->UART_ID]->CR1 |= ((UART_Config->Interrupts->PE) << CR1_PEIE);
}

/*==============================================================================================================================================
 *@fn      : UART_voidTransmit
 *@brief  : This is a asynchronus function used to transmit data  through UART peripheral
 *@paramter[in]  : u16 Copy_u16Data
 *@retval void
 *==============================================================================================================================================*/
void UART_voidTransmitData(UART_Config_t *UART_Config, uint16_t Copy_u16Data)
{
	/* Making sure that data regste is empty */
	while (((UART[UART_Config->UART_ID]->SR >> SR_TXE) & 1) == 0)
		;
	/*Setting Data to be Transfered */
	UART[UART_Config->UART_ID]->DR = Copy_u16Data;
	/*Waiting for Data to be transfered */
	while (((UART[UART_Config->UART_ID]->SR >> SR_TC) & 1) == 0)
		;
}
/*==============================================================================================================================================
 *@fn      : UART_u16Receive
 *@brief  :  This is a asynchronus function used to receive data  through UART peripheral
 *@paramter[in]  : void
 *@retval u16
 *==============================================================================================================================================*/
uint16_t UART_u16Receive(UART_Config_t *UART_Config)
{

	/* Wait for data to be transfered from other device*/
	while (((UART[UART_Config->UART_ID]->SR >> SR_RXNE) & 1) == 0)
		;
	/* Returning Data from Data register */
	return UART[UART_Config->UART_ID]->DR;
}
/*==============================================================================================================================================
 *@fn      : UART_voidTransmitIT
 *@brief  :  This is a function used to receive data through UART peripheral using interrupts
 * *@paramter[in]  : UART_Config_t *UART_Config
 *@retval u16
 *==============================================================================================================================================*/
void UART_u16ReceiveIT(UART_Config_t *UART_Config, void (*pv_CallBackFunc)(void))
{

	/* Setting CallBack Function */
	UART_PTR_TO_FUNC[UART_Config->UART_ID][RXN] = pv_CallBackFunc;
	/* Enabling Read Data Register Not Empty Interrupt */
	UART[UART_Config->UART_ID]->CR1 |= (1 << CR1_RXNEIE);
}
/*==============================================================================================================================================
 *@fn    USART_voidRecieveBuffer
 *@brief  This function is used to receive a string through UART peripheral using polling
 *@paramter[in]  Copy_p8Buffer : Pointer to the Buffer which the received string will be written to
 *@paramter[in]  Copy_u8BufferSize : The Size of the Buffer
 *@paramter[in] UART_Config_t *UART_Config : Pointer to the UART peripheral configuration structure
 *@retval void
 *==============================================================================================================================================*/
void UART_voidRecieveBuffer(UART_Config_t *UART_Config, uint16_t *Copy_p8Buffer, uint16_t Copy_u8BufferSize)
{

	/* Local Variable to hold the counter */
	uint16_t Local_u8Counter = 0;

	/* Looping on the buffer */
	for (Local_u8Counter = 0; Local_u8Counter < Copy_u8BufferSize; Local_u8Counter++)
	{

		/* Receiving Data */
		Copy_p8Buffer[Local_u8Counter] = UART_u16Receive(UART_Config);
	}
}
/*==============================================================================================================================================
 *@fn    USART_voidTransmitBuffer
 *@brief  This function is used to transmit a string through UART peripheral using polling
 *@paramter[in]  Copy_p8Buffer : Pointer to the Buffer which the string will be transmitted from
 *@paramter[in]  Copy_u8BufferSize : The Size of the Buffer
 *@paramter[in] UART_Config_t *UART_Config : Pointer to the UART peripheral configuration structure
 *@retval void
 *==============================================================================================================================================*/
void UART_voidTransmitBuffer(UART_Config_t *UART_Config, uint16_t *Copy_p8Buffer, uint16_t Copy_u8BufferSize)
{

	/* Local Variable to hold the counter */
	uint16_t Local_u8Counter = 0;

	/* Looping on the buffer */
	for (Local_u8Counter = 0; Local_u8Counter < Copy_u8BufferSize; Local_u8Counter++)
	{

		/* Transmitting Data */
		UART_voidTransmitData(UART_Config, Copy_p8Buffer[Local_u8Counter]);
	}
}
/*==============================================================================================================================================
 *@fn    UART_VoidEnableDMATransmit
 *@brief   This function is used to enable the DMA for the UART peripheral
 *@paramter[in] UART_Config_t *UART_Config : Pointer to the UART peripheral configuration structure
 *@retval void
 *==============================================================================================================================================*/
void UART_VoidEnableDMATransmit(UART_Config_t *UART_Config)
{
	/* Enabling DMA for Transmit */
	UART[UART_Config->UART_ID]->CR3 |= (1 << CR3_DMAT);
}
/*==============================================================================================================================================
 *@fn    UART_VoidEnableDMAReciver
 *@brief   This function is used to enable the DMA for the UART peripheral
 *@paramter[in] UART_Config_t *UART_Config : Pointer to the UART peripheral configuration structure
 *@retval void
 *==============================================================================================================================================*/
void UART_VoidEnableDMAReciver(UART_Config_t *UART_Config)
{
	/* Enabling DMA for Recieving */
	UART[UART_Config->UART_ID]->CR3 |= (1 << CR3_DMAR);
}
/*==============================================================================================================================================
 *@fn    USART_voidRecieveBufferIR
 *@brief  This function is used to receive a string through UART peripheral using interrupts
 *@paramter[in]  Copy_p8Buffer : Pointer to the Buffer which the received string will be written to
 *@paramter[in]  Copy_u8BufferSize : The Size of the Buffer
 *@paramter[in] UART_Config_t *UART_Config : Pointer to the UART peripheral configuration structure
 *@retval void
 *==============================================================================================================================================*/
void UART_voidRecieveBufferIT(UART_Config_t *UART_Config, uint16_t *Copy_p8Buffer, uint16_t Copy_u8BufferSize, void (*pv_CallBackFunc)(void))
{
	UART_RecieveBufferFlag[UART_Config->UART_ID] = 1;

	UART_RecieveBufferSize[UART_Config->UART_ID] = Copy_u8BufferSize;

	/* Setting CallBack Function */
	UART_PTR_TO_FUNC[UART_Config->UART_ID][RXN] = pv_CallBackFunc;

	/* Setting the Buffer */
	UART_RecievedBuffer[UART_Config->UART_ID] = Copy_p8Buffer;

	/* Enabling Read Data Register Not Empty Interrupt */
	UART[UART_Config->UART_ID]->CR1 |= (1 << CR1_RXNEIE);
}

/*==============================================================================================================================================
 *@fn    USART_voidTransmitBuffer
 *@brief  This function is used to transmit a string through UART peripheral using interrupts
 *@paramter[in]  Copy_p8Buffer : Pointer to the Buffer which the string will be transmitted from
 *@paramter[in]  Copy_u8BufferSize : The Size of the Buffer
 *@paramter[in] UART_Config_t *UART_Config : Pointer to the UART peripheral configuration structure
 *@retval void
 *==============================================================================================================================================*/
void UART_voidTransmitBufferIT(UART_Config_t *UART_Config, uint16_t *Copy_p8Buffer, uint16_t Copy_u8BufferSize);
/*==============================================================================================================================================
 * ISR
 *==============================================================================================================================================*/
/* UART1 ISR */
void USART1_IRQHandler(void)
{
	/* Comming from UART_ReciveBufferIT*/
	if ((UART_ReadFlag(UART_1, RXNE_Flage) == 1) && UART_RecieveBufferFlag[UART_1] == 1)
	{
		static uint8_t FlagCounter = 1;

		/* Reading Data */
		UART_RecievedBufferIT[UART_1][FlagCounter] = UART[UART_1]->DR;
		FlagCounter++;

		if (FlagCounter == UART_RecieveBufferSize[UART_1])
		{
			/* Disabling Read Data Register Not Empty Interrupt */
			UART_RecieveBufferFlag[UART_1] = 0;

			UART[UART_1]->CR1 &= ~(1 << CR1_RXNEIE);

			/* Calling the CallBack Function */
			UART_PTR_TO_FUNC[UART_1][RXNE_Flage]();
		}
	}

	/* Comming From UART_RecieBuffer*/
	if ((UART_ReadFlag(UART_1, RXNE_Flage) == 1))
	{
		*UART_RecievedBuffer[UART_1] = UART[UART_1]->DR;

		UART[UART_1]->CR1 &= ~(1 << CR1_RXNEIE);

		/* Calling the CallBack Function */
		UART_PTR_TO_FUNC[UART_1][RXN]();
	}
	/* Transmission Complete */
	if (UART_ReadFlag(UART_1, TC_Flage) == 1)
	{
		/* Clearing the TC Flag */
		UART[UART_1]->SR &= ~(1 << SR_TC);

		/* Calling the CallBack Function */
		UART_PTR_TO_FUNC[UART_1][TC_Flage]();
	}
	/* Transmit Data Register Empty */
	if (UART_ReadFlag(UART_1, TXE_Flage) == 1)
	{
		/* Clearing the TXE Flag */
		UART[UART_1]->SR &= ~(1 << SR_TXE);

		/* Calling the CallBack Function */
		UART_PTR_TO_FUNC[UART_1][TXE_Flage]();
	}

	/* Overrun Error */
	if (UART_ReadFlag(UART_1, ORE_Flage) == 1)
	{
		/* Clearing the ORE Flag */
		UART[UART_1]->SR &= ~(1 << SR_ORE);

		/* Calling the CallBack Function */
		UART_PTR_TO_FUNC[UART_1][ORE_Flage]();
	}

	/* Framing Error */
	if (UART_ReadFlag(UART_1, FE_Flage) == 1)
	{
		/* Clearing the FE Flag */
		UART[UART_1]->SR &= ~(1 << SR_FE);

		/* Calling the CallBack Function */
		UART_PTR_TO_FUNC[UART_1][FE_Flage]();
	}
	/* Noise Error */
	if (UART_ReadFlag(UART_1, NE_Flage) == 1)
	{
		/* Clearing the NE Flag */
		UART[UART_1]->SR &= ~(1 << SR_NE);

		/* Calling the CallBack Function */
		UART_PTR_TO_FUNC[UART_1][NE_Flage]();
	}

	/* Parity Error */
	if (UART_ReadFlag(UART_1, PE_Flage) == 1)
	{
		/* Clearing the PE Flag */
		UART[UART_1]->SR &= ~(1 << SR_PE);

		/* Calling the CallBack Function */
		UART_PTR_TO_FUNC[UART_1][PE_Flage]();
	}
}
/* UART2 ISR */
void USART2_IRQHandler(void)
{
	/* Comming from UART_ReciveBufferIT*/
	if ((UART_ReadFlag(UART_2, RXNE_Flage) == 1) && UART_RecieveBufferFlag[UART_2] == 1)
	{
		static uint8_t FlagCounter = 1;

		/* Reading Data */
		UART_RecievedBufferIT[UART_2][FlagCounter] = UART[UART_2]->DR;
		FlagCounter++;

		if (FlagCounter == UART_RecieveBufferSize[UART_2])
		{
			/* Disabling Read Data Register Not Empty Interrupt */
			UART_RecieveBufferFlag[UART_2] = 0;

			UART[UART_2]->CR1 &= ~(1 << CR1_RXNEIE);

			/* Calling the CallBack Function */
			UART_PTR_TO_FUNC[UART_2][RXNE_Flage]();
		}
	}

	/* Comming From UART_RecieBuffer*/
	if ((UART_ReadFlag(UART_2, RXNE_Flage) == 1))
	{
		*UART_RecievedBuffer[UART_2] = UART[UART_2]->DR;

		UART[UART_2]->CR1 &= ~(1 << CR1_RXNEIE);

		/* Calling the CallBack Function */
		UART_PTR_TO_FUNC[UART_2][RXN]();
	}
	/* Transmission Complete */
	if (UART_ReadFlag(UART_2, TC_Flage) == 1)
	{
		/* Clearing the TC Flag */
		UART[UART_2]->SR &= ~(1 << SR_TC);

		/* Calling the CallBack Function */
		UART_PTR_TO_FUNC[UART_2][TC_Flage]();
	}
	/* Transmit Data Register Empty */
	if (UART_ReadFlag(UART_2, TXE_Flage) == 1)
	{
		/* Clearing the TXE Flag */
		UART[UART_2]->SR &= ~(1 << SR_TXE);

		/* Calling the CallBack Function */
		UART_PTR_TO_FUNC[UART_2][TXE_Flage]();
	}

	/* Overrun Error */
	if (UART_ReadFlag(UART_2, ORE_Flage) == 1)
	{
		/* Clearing the ORE Flag */
		UART[UART_2]->SR &= ~(1 << SR_ORE);
		/* Calling the CallBack Function */
		UART_PTR_TO_FUNC[UART_2][ORE_Flage]();
	}
	/* Framing Error */
	if (UART_ReadFlag(UART_2, FE_Flage) == 1)
	{
		/* Clearing the FE Flag */
		UART[UART_2]->SR &= ~(1 << SR_FE);

		/* Calling the CallBack Function */
		UART_PTR_TO_FUNC[UART_2][FE_Flage]();
	}
	/* Noise Error */
	if (UART_ReadFlag(UART_2, NE_Flage) == 1)
	{
		/* Clearing the NE Flag */
		UART[UART_2]->SR &= ~(1 << SR_NE);

		/* Calling the CallBack Function */
		UART_PTR_TO_FUNC[UART_2][NE_Flage]();
	}

	/* Parity Error */
	if (UART_ReadFlag(UART_2, PE_Flage) == 1)
	{
		/* Clearing the PE Flag */
		UART[UART_2]->SR &= ~(1 << SR_PE);

		/* Calling the CallBack Function */
		UART_PTR_TO_FUNC[UART_2][PE_Flage]();
	}
}
/* UART3 ISR */
void USART3_IRQHandler(void)
{
	/* Comming from UART_ReciveBufferIT*/
	if ((UART_ReadFlag(UART_3, RXNE_Flage) == 1) && UART_RecieveBufferFlag[UART_3] == 1)
	{
		static uint8_t FlagCounter = 1;

		/* Reading Data */
		UART_RecievedBufferIT[UART_3][FlagCounter] = UART[UART_3]->DR;
		FlagCounter++;

		if (FlagCounter == UART_RecieveBufferSize[UART_3])
		{
			/* Disabling Read Data Register Not Empty Interrupt */
			UART_RecieveBufferFlag[UART_3] = 0;

			UART[UART_3]->CR1 &= ~(1 << CR1_RXNEIE);

			/* Calling the CallBack Function */
			UART_PTR_TO_FUNC[UART_3][RXNE_Flage]();
		}
	}

	/* Comming From UART_RecieBuffer*/
	if ((UART_ReadFlag(UART_3, RXNE_Flage) == 1))
	{
		*UART_RecievedBuffer[UART_3] = UART[UART_3]->DR;

		UART[UART_3]->CR1 &= ~(1 << CR1_RXNEIE);

		/* Calling the CallBack Function */
		UART_PTR_TO_FUNC[UART_3][RXN]();
	}
	/* Transmission Complete */
	if (UART_ReadFlag(UART_3, TC_Flage) == 1)
	{
		/* Clearing the TC Flag */
		UART[UART_3]->SR &= ~(1 << SR_TC);

		/* Calling the CallBack Function */
		UART_PTR_TO_FUNC[UART_3][TC_Flage]();
	}
	/* Transmit Data Register Empty */
	if (UART_ReadFlag(UART_3, TXE_Flage) == 1)
	{
		/* Clearing the TXE Flag */
		UART[UART_3]->SR &= ~(1 << SR_TXE);

		/* Calling the CallBack Function */
		UART_PTR_TO_FUNC[UART_3][TXE_Flage]();
	}

	/* Overrun Error */
	if (UART_ReadFlag(UART_3, ORE_Flage) == 1)
	{
		/* Clearing the ORE Flag */
		UART[UART_3]->SR &= ~(1 << SR_ORE);
		/* Calling the CallBack Function */
		UART_PTR_TO_FUNC[UART_3][ORE_Flage]();
	}
	/* Framing Error */
	if (UART_ReadFlag(UART_3, FE_Flage) == 1)
	{
		/* Clearing the FE Flag */
		UART[UART_3]->SR &= ~(1 << SR_FE);

		/* Calling the CallBack Function */
		UART_PTR_TO_FUNC[UART_3][FE_Flage]();
	}
	/* Noise Error */
	if (UART_ReadFlag(UART_3, NE_Flage) == 1)
	{
		/* Clearing the NE Flag */
		UART[UART_3]->SR &= ~(1 << SR_NE);

		/* Calling the CallBack Function */
		UART_PTR_TO_FUNC[UART_3][NE_Flage]();
	}

	/* Parity Error */
	if (UART_ReadFlag(UART_3, PE_Flage) == 1)
	{
		/* Clearing the PE Flag */
		UART[UART_3]->SR &= ~(1 << SR_PE);

		/* Calling the CallBack Function */
		UART_PTR_TO_FUNC[UART_3][PE_Flage]();
	}
}
/* UART4 ISR */
void UART4_IRQHandler(void)
{
	/* Comming from UART_ReciveBufferIT*/
	if ((UART_ReadFlag(UART_4, RXNE_Flage) == 1) && UART_RecieveBufferFlag[UART_4] == 1)
	{
		static uint8_t FlagCounter = 1;

		/* Reading Data */
		UART_RecievedBufferIT[UART_4][FlagCounter] = UART[UART_4]->DR;
		FlagCounter++;

		if (FlagCounter == UART_RecieveBufferSize[UART_4])
		{
			/* Disabling Read Data Register Not Empty Interrupt */
			UART_RecieveBufferFlag[UART_4] = 0;

			UART[UART_4]->CR1 &= ~(1 << CR1_RXNEIE);

			/* Calling the CallBack Function */
			UART_PTR_TO_FUNC[UART_4][RXNE_Flage]();
		}
	}

	/* Comming From UART_RecieBuffer*/
	if ((UART_ReadFlag(UART_4, RXNE_Flage) == 1))
	{
		*UART_RecievedBuffer[UART_4] = UART[UART_4]->DR;

		UART[UART_4]->CR1 &= ~(1 << CR1_RXNEIE);

		/* Calling the CallBack Function */
		UART_PTR_TO_FUNC[UART_4][RXN]();
	}
	/* Transmission Complete */
	if (UART_ReadFlag(UART_4, TC_Flage) == 1)
	{
		/* Clearing the TC Flag */
		UART[UART_4]->SR &= ~(1 << SR_TC);

		/* Calling the CallBack Function */
		UART_PTR_TO_FUNC[UART_4][TC_Flage]();
	}
	/* Transmit Data Register Empty */
	if (UART_ReadFlag(UART_4, TXE_Flage) == 1)
	{
		/* Clearing the TXE Flag */
		UART[UART_4]->SR &= ~(1 << SR_TXE);

		/* Calling the CallBack Function */
		UART_PTR_TO_FUNC[UART_4][TXE_Flage]();
	}

	/* Overrun Error */
	if (UART_ReadFlag(UART_4, ORE_Flage) == 1)
	{
		/* Clearing the ORE Flag */
		UART[UART_4]->SR &= ~(1 << SR_ORE);
		/* Calling the CallBack Function */
		UART_PTR_TO_FUNC[UART_4][ORE_Flage]();
	}
	/* Framing Error */
	if (UART_ReadFlag(UART_4, FE_Flage) == 1)
	{
		/* Clearing the FE Flag */
		UART[UART_4]->SR &= ~(1 << SR_FE);

		/* Calling the CallBack Function */
		UART_PTR_TO_FUNC[UART_4][FE_Flage]();
	}

	/* Noise Error */
	if (UART_ReadFlag(UART_4, NE_Flage) == 1)
	{
		/* Clearing the NE Flag */
		UART[UART_4]->SR &= ~(1 << SR_NE);

		/* Calling the CallBack Function */
		UART_PTR_TO_FUNC[UART_4][NE_Flage]();
	}

	/* Parity Error */
	if (UART_ReadFlag(UART_4, PE_Flage) == 1)
	{
		/* Clearing the PE Flag */
		UART[UART_4]->SR &= ~(1 << SR_PE);

		/* Calling the CallBack Function */
		UART_PTR_TO_FUNC[UART_4][PE_Flage]();
	}
}
/* UART5 ISR */
void UART5_IRQHandler(void)
{
	/* Comming from UART_ReciveBufferIT*/
	if ((UART_ReadFlag(UART_5, RXNE_Flage) == 1) && UART_RecieveBufferFlag[UART_5] == 1)
	{
		static uint8_t FlagCounter = 1;

		/* Reading Data */
		UART_RecievedBufferIT[UART_5][FlagCounter] = UART[UART_5]->DR;
		FlagCounter++;

		if (FlagCounter == UART_RecieveBufferSize[UART_5])
		{
			/* Disabling Read Data Register Not Empty Interrupt */
			UART_RecieveBufferFlag[UART_5] = 0;

			UART[UART_5]->CR1 &= ~(1 << CR1_RXNEIE);

			/* Calling the CallBack Function */
			UART_PTR_TO_FUNC[UART_5][RXNE_Flage]();
		}
	}

	/* Comming From UART_RecieBuffer*/
	if ((UART_ReadFlag(UART_5, RXNE_Flage) == 1))
	{
		*UART_RecievedBuffer[UART_5] = UART[UART_5]->DR;

		UART[UART_5]->CR1 &= ~(1 << CR1_RXNEIE);

		/* Calling the CallBack Function */
		UART_PTR_TO_FUNC[UART_5][RXN]();
	}
	/* Transmission Complete */
	if (UART_ReadFlag(UART_5, TC_Flage) == 1)
	{
		/* Clearing the TC Flag */
		UART[UART_5]->SR &= ~(1 << SR_TC);

		/* Calling the CallBack Function */
		UART_PTR_TO_FUNC[UART_5][TC_Flage]();
	}
	/* Transmit Data Register Empty */
	if (UART_ReadFlag(UART_5, TXE_Flage) == 1)
	{
		/* Clearing the TXE Flag */
		UART[UART_5]->SR &= ~(1 << SR_TXE);

		/* Calling the CallBack Function */
		UART_PTR_TO_FUNC[UART_5][TXE_Flage]();
	}

	/* Overrun Error */
	if (UART_ReadFlag(UART_5, ORE_Flage) == 1)
	{
		/* Clearing the ORE Flag */
		UART[UART_5]->SR &= ~(1 << SR_ORE);
		/* Calling the CallBack Function */
		UART_PTR_TO_FUNC[UART_5][ORE_Flage]();
	}
	/* Framing Error */
	if (UART_ReadFlag(UART_5, FE_Flage) == 1)
	{
		/* Clearing the FE Flag */
		UART[UART_5]->SR &= ~(1 << SR_FE);

		/* Calling the CallBack Function */
		UART_PTR_TO_FUNC[UART_5][FE_Flage]();
	}

	/* Noise Error */
	if (UART_ReadFlag(UART_5, NE_Flage) == 1)
	{
		/* Clearing the NE Flag */
		UART[UART_5]->SR &= ~(1 << SR_NE);

		/* Calling the CallBack Function */
		UART_PTR_TO_FUNC[UART_5][NE_Flage]();
	}

	/* Parity Error */
	if (UART_ReadFlag(UART_5, PE_Flage) == 1)
	{
		/* Clearing the PE Flag */
		UART[UART_5]->SR &= ~(1 << SR_PE);

		/* Calling the CallBack Function */
		UART_PTR_TO_FUNC[UART_5][PE_Flage]();
	}
}
/* UART6 ISR */
void USART6_IRQHandler(void)
{
	/* Comming from UART_ReciveBufferIT*/
	if ((UART_ReadFlag(UART_6, RXNE_Flage) == 1) && UART_RecieveBufferFlag[UART_6] == 1)
	{
		static uint8_t FlagCounter = 1;

		/* Reading Data */
		UART_RecievedBufferIT[UART_6][FlagCounter] = UART[UART_6]->DR;
		FlagCounter++;

		if (FlagCounter == UART_RecieveBufferSize[UART_6])
		{
			/* Disabling Read Data Register Not Empty Interrupt */
			UART_RecieveBufferFlag[UART_6] = 0;

			UART[UART_6]->CR1 &= ~(1 << CR1_RXNEIE);

			/* Calling the CallBack Function */
			UART_PTR_TO_FUNC[UART_6][RXNE_Flage]();
		}
	}

	/* Comming From UART_RecieBuffer*/
	if ((UART_ReadFlag(UART_6, RXNE_Flage) == 1))
	{
		*UART_RecievedBuffer[UART_6] = UART[UART_6]->DR;

		UART[UART_6]->CR1 &= ~(1 << CR1_RXNEIE);

		/* Calling the CallBack Function */
		UART_PTR_TO_FUNC[UART_6][RXN]();
	}
	/* Transmission Complete */
	if (UART_ReadFlag(UART_6, TC_Flage) == 1)
	{
		/* Clearing the TC Flag */
		UART[UART_6]->SR &= ~(1 << SR_TC);

		/* Calling the CallBack Function */
		UART_PTR_TO_FUNC[UART_6][TC_Flage]();
	}
	/* Transmit Data Register Empty */
	if (UART_ReadFlag(UART_6, TXE_Flage) == 1)
	{
		/* Clearing the TXE Flag */
		UART[UART_6]->SR &= ~(1 << SR_TXE);

		/* Calling the CallBack Function */
		UART_PTR_TO_FUNC[UART_6][TXE_Flage]();
	}

	/* Overrun Error */
	if (UART_ReadFlag(UART_6, ORE_Flage) == 1)
	{
		/* Clearing the ORE Flag */
		UART[UART_6]->SR &= ~(1 << SR_ORE);
		/* Calling the CallBack Function */
		UART_PTR_TO_FUNC[UART_6][ORE_Flage]();
	}
	/* Framing Error */
	if (UART_ReadFlag(UART_6, FE_Flage) == 1)
	{
		/* Clearing the FE Flag */
		UART[UART_6]->SR &= ~(1 << SR_FE);

		/* Calling the CallBack Function */
		UART_PTR_TO_FUNC[UART_6][FE_Flage]();
	}

	/* Noise Error */
	if (UART_ReadFlag(UART_6, NE_Flage) == 1)
	{
		/* Clearing the NE Flag */
		UART[UART_6]->SR &= ~(1 << SR_NE);

		/* Calling the CallBack Function */
		UART_PTR_TO_FUNC[UART_6][NE_Flage]();
	}

	/* Parity Error */
	if (UART_ReadFlag(UART_6, PE_Flage) == 1)
	{
		/* Clearing the PE Flag */
		UART[UART_6]->SR &= ~(1 << SR_PE);

		/* Calling the CallBack Function */
		UART_PTR_TO_FUNC[UART_6][PE_Flage]();
	}
}
