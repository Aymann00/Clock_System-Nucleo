/*
 *@file		:	SPI_Program.c
 *@author	: 	Mohamed Wael
 *@brief	:	Main Program body for SPI Peripheral
 */

/******************* MAIN INCLUDES *********************/
#include <stdint.h>
#include "../../Library/STM32F446xx.h"
#include "../../Library/ErrTypes.h"


#include "../Inc/SPI_Interface.h"
#include "../Inc/SPI_Private.h"
/*******************************************************/


/********************** MAIN PV ************************/

static SPI_RegDef_t * SPIs[MAX_SPIs_NUMBER]={SPI1,SPI2,SPI3,SPI4};

static uint8_t IRQ_Source[MAX_SPIs_NUMBER]={NO_SRC};

/*2D array of USARTs Call back functions*/
static void (*SPI_pf_CallBackFuncs[MAX_SPIs_NUMBER][SPI_MAX_INTERRUPTS])(void)={NULL};

/*Variable to Save the Data buffer size globally*/
static uint8_t Global_Data_Size=0;

/*Variable to Save the Data buffer globally*/
static uint8_t* Global_Data_Buffer=NULL;

/*Variable to Save the Received globally*/
static uint8_t* Global_Received_Data=NULL;
/*******************************************************/

/****************** MAIN FUNCTIONS *********************/

Error_State_t SPI_Init(const SPI_CONFIGS_t * SPI_Config)
{
	Error_State_t Error_State = OK;
	/*Check Passed Configurations*/
	if (OK == SPI_Check_Configs(SPI_Config))
	{
		/*1- Set Baud Rate Value if Master*/
		if (SPI_Config->Chip_Mode == CHIP_MODE_MASTER)
		{
			SPIs[SPI_Config->SPI_Num]->CR1 &= ~((BAUD_RATE_MASK)<<BAUD_RATE_START_BITS);
			SPIs[SPI_Config->SPI_Num]->CR1 |=  ((SPI_Config->BaudRate_Value)<<BAUD_RATE_START_BITS);
		}
		/*2- Set Clock Polarity*/
		SPIs[SPI_Config->SPI_Num]->CR1 &= ~((CLOCK_POL_MASK)<<CLOCK_POL_START_BITS);
		SPIs[SPI_Config->SPI_Num]->CR1 |=  ((SPI_Config->Clock_Polarity)<<CLOCK_POL_START_BITS);

		/*3- Set Clock Phase*/
		SPIs[SPI_Config->SPI_Num]->CR1 &= ~((CLOCK_PHASE_MASK)<<CLOCK_PHASE_START_BITS);
		SPIs[SPI_Config->SPI_Num]->CR1 |=  ((SPI_Config->Clock_Phase)<<CLOCK_PHASE_START_BITS);

		/*4- Set Transfer Mode*/
		if(SPI_Config->Transfer_Mode != TRANSFER_MODE_SIMPLEX)
		{
			SPIs[SPI_Config->SPI_Num]->CR1 &= ~((TRANSFER_MODE_MASK)<<TRANS_MODE_START_BITS);
			SPIs[SPI_Config->SPI_Num]->CR1 |=  ((SPI_Config->Transfer_Mode)<<TRANS_MODE_START_BITS);
		}
		else {
			SPIs[SPI_Config->SPI_Num]->CR1 &= ~((RX_ONLY_MODE_MASK)<<RX_ONLY_START_BITS);
			SPIs[SPI_Config->SPI_Num]->CR1 |=  (1<<RX_ONLY_START_BITS);

		}
		/*5- Set Frame Format Type*/
		SPIs[SPI_Config->SPI_Num]->CR1 &= ~((FRAME_TYPE_MASK)<<FRAME_TYPE_START_BITS);
		SPIs[SPI_Config->SPI_Num]->CR1 |=  ((SPI_Config->Frame_Type)<<FRAME_TYPE_START_BITS);

		/*6- Set CRC Enable State*/
		SPIs[SPI_Config->SPI_Num]->CR1 &= ~((CRC_ENABLE_MASK)<<CRC_ENABLE_START_BITS);
		SPIs[SPI_Config->SPI_Num]->CR1 |=  ((SPI_Config->CRC_State)<<CRC_ENABLE_START_BITS);

		/*7- Set Slave Management state*/
		SPIs[SPI_Config->SPI_Num]->CR1 &= ~((SLAVE_MANAGE_MASK)<<SLAVE_MANAGE_START_BITS);
		SPIs[SPI_Config->SPI_Num]->CR1 |=  ((SPI_Config->Slave_Manage_State)<<SLAVE_MANAGE_START_BITS);

		/*8- Set CHIP State*/
		SPIs[SPI_Config->SPI_Num]->CR1 &= ~((CHIP_MODE_MASK)<<CHIP_MODE_START_BITS);
		SPIs[SPI_Config->SPI_Num]->CR1 |=  ((SPI_Config->Chip_Mode)<<CHIP_MODE_START_BITS);

		/*9- Set Data Frame SIZE*/
		SPIs[SPI_Config->SPI_Num]->CR1 &= ~((FRAME_SIZE_MASK)<<FRAME_SIZE_START_BITS);
		SPIs[SPI_Config->SPI_Num]->CR1 |=  ((SPI_Config->Frame_Size)<<FRAME_SIZE_START_BITS);

		/*10- Set MultiMaster Ability State if Master*/
		if (SPI_Config->Chip_Mode == CHIP_MODE_MASTER)
		{
			SPIs[SPI_Config->SPI_Num]->CR2 &= ~((SSOE_MASK)<<SSOE_BIT_START);
			SPIs[SPI_Config->SPI_Num]->CR2 |=  ((SPI_Config->MultiMaster_State)<<SSOE_BIT_START);
		}
		/*11- Enable SPI*/
		SPIs[SPI_Config->SPI_Num]->CR1 |=  ((1)<<SPI_ENABLE_BIT_START);
	}
	else
	{
		Error_State = SPI_Check_Configs(SPI_Config);
	}
	return Error_State;
}

/*
 * @function 		:	SPI_Transmit
 * @brief			:	Transmit Data via SPI
 * @param			:	SPI Number
 * @param			:	pointer to Data Buffer
 * @param 			: 	Data Buffer Size
 * @retval			:	Error State
 */
Error_State_t SPI_Transmit(const SPI_CONFIGS_t * SPI_Config, uint16_t * Data , uint8_t Buffer_Size)
{
	Error_State_t 	Error_State = 	OK	;
	uint8_t 	  	Counter	  	=	0 	;
	uint8_t 	 	Flag_State  =	FLAG_RESET 	;
	if (NULL != Data)
	{
		if (( SPI_Config->SPI_Num >=SPI_NUMBER1) && ( SPI_Config->SPI_Num <=SPI_NUMBER4))
		{

			/*Send the Data*/
			while (Counter < Buffer_Size)
			{
				while (Flag_State != FLAG_SET)
				{
					Flag_State	=	GET_BIT(SPIs[SPI_Config->SPI_Num]->SR , SPI_FLAGS_TXE);
				}
				/*Put Data in DR*/
				SPIs[SPI_Config->SPI_Num]->DR = Data[Counter++];
			}
		}
		else {
			Error_State = SPI_WRONG_SPI_NUMBER;
		}
	}
	else {
		Error_State = Null_Pointer ;
	}
	return Error_State	;
}
/*
 * @function 		:	SPI_Receive
 * @brief			:	Receive Data via SPI
 * @param			:	Chip state (Master/Slave)
 * @param			:	SPI Number
 * @param			:	Buffer to save Data
 * @param 			: 	Data Buffer Size
 * @retval			:	Error State
 */
Error_State_t SPI_Receive(const SPI_CONFIGS_t * SPI_Config, uint16_t * Received_Data ,uint8_t Buffer_Size)
{
	Error_State_t 	Error_State = 	OK	;
	uint8_t 	  	Counter	  	=	0 	;
	uint8_t 	 	Flag_State  =	FLAG_RESET 	;
	if (NULL != Received_Data)
	{
		if ((SPI_Config->SPI_Num>=SPI_NUMBER1) && (SPI_Config->SPI_Num<=SPI_NUMBER4))
		{
			if (SPI_Config->Chip_Mode == CHIP_MODE_SLAVE)
			{
				while (Counter < Buffer_Size)
				{
					/*Wait till data is Received*/
					while (Flag_State != FLAG_SET)
					{
						Flag_State	=	GET_BIT(SPIs[SPI_Config->SPI_Num]->SR , SPI_FLAGS_RXNE);
					}
					/*Read the Received data*/
					Received_Data[Counter++] = SPIs[SPI_Config->SPI_Num]->DR ;
				}
			}
			else if (SPI_Config->Chip_Mode == CHIP_MODE_MASTER)
			{
				while (Counter < Buffer_Size)
				{
					/* writing garbage in the Tx Buffer to start Receiving*/
					SPIs[SPI_Config->SPI_Num]->DR = GARBAGE_VALUE;
					/*Wait till data is Received*/
					while (Flag_State != FLAG_SET)
					{
						Flag_State	=	GET_BIT(SPIs[SPI_Config->SPI_Num]->SR , SPI_FLAGS_RXNE);
					}
					/*Read the Received data*/
					Received_Data[Counter++] = SPIs[SPI_Config->SPI_Num]->DR ;
				}
			}
			else {
				Error_State = SPI_WRONG_CHIP_MODE;
			}

		}
		else {
			Error_State = SPI_WRONG_SPI_NUMBER;
		}
	}
	else {
		Error_State = Null_Pointer ;
	}
	return Error_State	;
}

/*
 * @function 		:	SPI_Transmit_IT
 * @brief			:	Transmit Data via SPI and
 * 						generate interrupt when Transmission is complete
 * @param			:	SPI Number
 * @param			:	Data To Send
 * @param 			: 	Data Buffer Size
 * @param			:	CallBack Function
 * @retval			:	Error State
 */
Error_State_t SPI_Transmit_IT(const SPI_CONFIGS_t * SPI_Config, uint8_t * Data , uint8_t Buffer_Size, void (* SPI_TXC_CallBackFunc)(void))
{
	Error_State_t 	Error_State = 	OK	;
	if ((NULL != Data) && (NULL != SPI_TXC_CallBackFunc))
	{
		if (( SPI_Config->SPI_Num >=SPI_NUMBER1) && ( SPI_Config->SPI_Num <=SPI_NUMBER4))
		{
			/*Set IRQ Source*/
			IRQ_Source[SPI_Config->SPI_Num] = SOURCE_TX;

			/*Set Call Back Globally*/
			SPI_pf_CallBackFuncs[SPI_Config->SPI_Num][SPI_FLAGS_TXE]= SPI_TXC_CallBackFunc ;

			/*Set data to be sent globally*/
			Global_Data_Buffer = Data ;

			/*Set Buffer Size Globally*/
			Global_Data_Size   = Buffer_Size;

			/*wait till TDR is ready*/
			while( ! (GET_BIT(SPIs[SPI_Config->SPI_Num]->SR,SPI_FLAGS_TXE) ) );

			/*Put First Data in DR*/
			SPIs[SPI_Config->SPI_Num]->DR = Data[0];

			/*Enable Transmission complete interrupt*/
			SPIs[SPI_Config->SPI_Num]->CR2 |= (1<<(SPI_INTERRUPT_TXEIE));

		}
		else {
			Error_State = SPI_WRONG_SPI_NUMBER;
		}
	}
	else {
		Error_State = Null_Pointer ;
	}
	return Error_State	;
}

/*
 * @function 		:	SPI_Receive_IT
 * @brief			:	Receive Data via SPI and generate interrupt when receive is complete
 * @param			:	Chip state (Master/Slave)
 * @param			:	SPI Number
 * @param			:	Buffer to save Data
 * @param 			: 	Data Buffer Size
 * @param			:	CallBack Function
 * @retval			:	Error State
 */
Error_State_t SPI_Receive_IT(const SPI_CONFIGS_t * SPI_Config, uint16_t * Received_Data ,uint8_t Buffer_Size , void (* SPI_RXC_CallBackFunc)(void))
{
	Error_State_t 	Error_State = 	OK	;
	if ((NULL != Received_Data) && (NULL != SPI_RXC_CallBackFunc))
	{
		if (( SPI_Config->SPI_Num >=SPI_NUMBER1) && ( SPI_Config->SPI_Num <=SPI_NUMBER4))
		{
			/*Set Call Back Globally*/
			SPI_pf_CallBackFuncs[SPI_Config->SPI_Num][SPI_FLAGS_RXNE]= SPI_RXC_CallBackFunc ;

			/*Set data to be RECEIVED globally*/
			Global_Received_Data = Received_Data ;

			/*Set Buffer Size Globally*/
			Global_Data_Size   = Buffer_Size;

			if (SPI_Config->Chip_Mode == CHIP_MODE_SLAVE)
			{
				/*Set IRQ Source*/
				IRQ_Source[SPI_Config->SPI_Num] = SOURCE_RX_SLAVE;

				/*Enable Receive complete Interrupt*/
				SPIs[SPI_Config->SPI_Num]->CR2 |= (1<<(SPI_INTERRUPT_RXNEIE));

			}
			else if (SPI_Config->Chip_Mode == CHIP_MODE_MASTER)
			{
				/*Set IRQ Source*/
				IRQ_Source[SPI_Config->SPI_Num] = SOURCE_RX_MASTER;
				/* writing garbage in the Tx Buffer to start Receiving*/
				SPIs[SPI_Config->SPI_Num]->DR = GARBAGE_VALUE;

				/*Enable Receive complete Interrupt*/
				SPIs[SPI_Config->SPI_Num]->CR2 |= (1<<(SPI_INTERRUPT_RXNEIE));
			}
			else {
				Error_State = SPI_WRONG_CHIP_MODE;
			}
		}
		else {
			Error_State = SPI_WRONG_SPI_NUMBER;
		}
	}
	else {
		Error_State = Null_Pointer ;
	}
	return Error_State	;


}

/*
 * @function 		:	SPI_Enable_DMA_RX
 * @brief			:	Enable DMA Line for SPI Receiving
 * @param			:	SPI NUMBER
 * @retval			:	Error State
 */
Error_State_t SPI_Enable_DMA_RX(SPI_SPI_NUMBER_t SPI_Num)
{
	Error_State_t Error_State = OK;

	if (( SPI_Num >=SPI_NUMBER1) && ( SPI_Num <=SPI_NUMBER4))
	{
		SPIs[SPI_Num]->CR2 |= (1<<(RXDMAEN_BIT));
	}
	else {
		Error_State = SPI_WRONG_SPI_NUMBER;
	}
	return Error_State ;
}

/*
 * @function 		:	SPI_Enable_DMA_TX
 * @brief			:	Enable DMA Line for SPI Transmitting
 * @param			:	SPI NUMBER
 * @retval			:	Error State
 */
Error_State_t SPI_Enable_DMA_TX(SPI_SPI_NUMBER_t SPI_Num)
{
	Error_State_t Error_State = OK;

	if (( SPI_Num >=SPI_NUMBER1) && ( SPI_Num <=SPI_NUMBER4))
	{
		SPIs[SPI_Num]->CR2 |= (1<<(TXDMAEN_BIT));
	}
	else {
		Error_State = SPI_WRONG_SPI_NUMBER;
	}
	return Error_State ;
}

/*
 * @function 		:	SPI_Internal_Slave_Activate
 * @brief			:	Enable Slave by itself
 * @param			:	SPI NUMBER
 * @param			:	Slave_State
 * @retval			:	Error State
 * @warning			:	this function is used only if the Chip mode is slave and
 * 						SW Slave management is enabled
 */
Error_State_t SPI_SET_Internal_Slave_State(SPI_SPI_NUMBER_t SPI_Num, SLAVE_STATE_t Slave_State)
{
	Error_State_t Error_State = OK;

	if (( SPI_Num >=SPI_NUMBER1) && ( SPI_Num <=SPI_NUMBER4))
	{
		if (Slave_State == SLAVE_STATE_ACTIVATED)
		{
			SPIs[SPI_Num]->CR1 &= ~(1<<(SSI_BIT));
		}
		else if (Slave_State == SLAVE_STATE_DEACTIVATED)
		{
			SPIs[SPI_Num]->CR1 |= (1<<(SSI_BIT));
		}
		else {
			Error_State = WRONG_SLAVE_STATE;
		}
	}
	else {
		Error_State = SPI_WRONG_SPI_NUMBER;
	}
	return Error_State ;
}

/***************End of MAIN FUNCTIONS*******************/

/***************** STATIC FUNCTIONS ********************/

/*
 * @function 		:	SPI_Check_Configs
 * @brief			:	Private Function To Check Passed SPI Configurations
 * @param			:	SPI_Configs
 * @retval			:	Error State
 */
static Error_State_t SPI_Check_Configs(const SPI_CONFIGS_t * SPI_Configs)
{
	Error_State_t Error_State = OK ;

	if (NULL != SPI_Configs)
	{
		if ((SPI_Configs->SPI_Num >= SPI_NUMBER1)&&(SPI_Configs->SPI_Num <= SPI_NUMBER4))
		{
			if ((SPI_Configs->BaudRate_Value>=BAUDRATE_FpclkBY2)&&(SPI_Configs->BaudRate_Value<=BAUDRATE_FpclkBY256))
			{
				if((SPI_Configs->CRC_State == CRC_STATE_ENABLED)||(SPI_Configs->CRC_State == CRC_STATE_DISABLED))
				{
					if ((SPI_Configs->Chip_Mode == CHIP_MODE_MASTER)||(SPI_Configs->Chip_Mode == CHIP_MODE_SLAVE))
					{
						if ((SPI_Configs->Clock_Phase == CLOCK_PHASE_CAPTURE_SECOND)||(SPI_Configs->Clock_Phase == CLOCK_PHASE_CAPTURE_FIRST))
						{
							if ((SPI_Configs->Clock_Polarity == CLOCK_POLARITY_IDLE_LOW)||(SPI_Configs->Clock_Polarity == CLOCK_POLARITY_IDLE_HIGH))
							{
								if ((SPI_Configs->Frame_Size == DATA_FRAME_SIZE_16BITS)||(SPI_Configs->Frame_Size == DATA_FRAME_SIZE_8BITS))
								{
									if ((SPI_Configs->Frame_Type== FRAME_FORMAT_MSB_FIRST)||(SPI_Configs->Frame_Type == FRAME_FORMAT_LSB_FIRST))
									{
										if ((SPI_Configs->Slave_Manage_State >= SLAVE_MANAGE_HW)&&(SPI_Configs->Slave_Manage_State <= SLAVE_MANAGE_SW_SLAVE_INACTIVE))
										{
											if ((SPI_Configs->Transfer_Mode >= TRANSFER_MODE_FULL_DUPLEX)&&(SPI_Configs->Transfer_Mode <= TRANSFER_MODE_SIMPLEX))
											{
												if ((SPI_Configs->MultiMaster_State == MULTIMASTER_NOT_PROVIDED)||(SPI_Configs->MultiMaster_State == MULTIMASTER_PROVIDED))
												{
													Error_State = OK;
												}
												else {
													Error_State =  SPI_WRONG_MULTIMASTER_STATE;
												}
											}
											else {
												Error_State = SPI_WRONG_TRANSFER_MODE ;
											}
										}
										else {
											Error_State = SPI_WRONG_SLAVE_MANAGE_STATE;
										}
									}
									else{
										Error_State = SPI_WRONG_FRAME_TYPE;
									}
								}
								else{
									Error_State = SPI_WRONG_FRAME_SIZE;
								}
							}
							else{
								Error_State = SPI_WRONG_CLOCK_POLARITY;
							}
						}
						else {
							Error_State = SPI_WRONG_CLOCK_PHASE;
						}
					}
					else{
						Error_State = SPI_WRONG_CHIP_MODE;
					}
				}
				else {
					Error_State = SPI_WRONG_CRC_STATE;
				}
			}
			else {
				Error_State = SPI_WRONG_BAUDRATE;
			}
		}
		else {
			Error_State = SPI_WRONG_SPI_NUMBER;
		}
	}
	else {
		Error_State = Null_Pointer;
	}
	return Error_State ;
}

/*
 * @function 		:	SPI_IRQ_Source_HANDLE
 * @brief			:	Private Function To handle the Different Sources of the IRQ
 * @param			:	SPI_Num, The Enabled SPI Number
 * @retval			:	Error State
 */
static void SPI_IRQ_Source_HANDLE(SPI_SPI_NUMBER_t SPI_Num)
{

	if (IRQ_Source[SPI_Num] == SOURCE_TX)
	{
		static uint16_t Counter=1;
		/*Complete buffer Transmission is done*/
		if (Counter == Global_Data_Size)
		{
			/*Disable the TC interrupt*/
			SPIs[SPI_Num]->CR2 &= ~(1<<(SPI_INTERRUPT_TXEIE));

			/*Clear IRQ Source*/
			IRQ_Source[SPI_Num] = NO_SRC;

			/*Call The call Back Function*/
			SPI_pf_CallBackFuncs[SPI_Num][SPI_FLAGS_TXE]();
		}

		/*Buffer isn't completely sent*/
		else {
			/*Send the next data element in the buffer*/
			SPIs[SPI_Num]->DR = Global_Data_Buffer[Counter++];
		}
	}
	else if (IRQ_Source[SPI_Num] == SOURCE_RX_SLAVE)
	{
		static uint16_t Counter=0;
		/*Whole buffer Receiving is done*/
		if (Global_Data_Size==1)
		{
			/*Disable the RXC interrupt*/
			SPIs[SPI_Num]->CR2 &= ~(1<<(SPI_INTERRUPT_RXNEIE));

			/*Clear IRQ Source*/
			IRQ_Source[SPI_Num] = NO_SRC;

			/*Receive the next data element*/
			Global_Received_Data[Counter++] = SPIs[SPI_Num]->DR;

			/*Call The call Back Function*/
			SPI_pf_CallBackFuncs[SPI_Num][SPI_FLAGS_RXNE]();

		}
		else
		{
			if (Counter == Global_Data_Size)
			{
				/*Disable the RXC interrupt*/
				SPIs[SPI_Num]->CR2 &= ~(1<<(SPI_INTERRUPT_RXNEIE));

				/*Clear IRQ Source*/
				IRQ_Source[SPI_Num] = NO_SRC;

				/*Call The call Back Function*/
				SPI_pf_CallBackFuncs[SPI_Num][SPI_FLAGS_RXNE]();
			}
			else {
				/*Receive the next data element*/
				Global_Received_Data[Counter++] = SPIs[SPI_Num]->DR;
			}
		}
	}
	else if (IRQ_Source[SPI_Num] == SOURCE_RX_MASTER)
	{
		static uint16_t Counter=0;
		/*Whole buffer Receiving is done*/
		if (Counter == Global_Data_Size)
		{
			/*Disable the RXC interrupt*/
			SPIs[SPI_Num]->CR2 &= ~(1<<(SPI_INTERRUPT_RXNEIE));

			/*Clear IRQ Source*/
			IRQ_Source[SPI_Num] = NO_SRC;

			/*Call The call Back Function*/
			SPI_pf_CallBackFuncs[SPI_Num][SPI_FLAGS_RXNE]();
		}
		else {

			/* writing garbage in the Tx Buffer to start Receiving*/
			SPIs[SPI_Num]->DR = GARBAGE_VALUE;

			/*Receive the next data element*/
			Global_Received_Data[Counter++] = SPIs[SPI_Num]->DR;
		}
	}
}
/************** End of STATIC FUNCTIONS ****************/


/********************* IRQ HANDLERS ********************/

void SPI1_IRQHandler (void)
{
	SPI_IRQ_Source_HANDLE(SPI_NUMBER1);
}

void SPI2_IRQHandler (void)
{
	SPI_IRQ_Source_HANDLE(SPI_NUMBER2);
}
void SPI3_IRQHandler (void)
{
	SPI_IRQ_Source_HANDLE(SPI_NUMBER3);
}
void SPI4_IRQHandler (void)
{
	SPI_IRQ_Source_HANDLE(SPI_NUMBER4);
}

/****************** End OF IRQ HANDLERS ****************/
