/*
 *@file		:	SPI_Program.c
 *@author	: 	Mohamed Wael
 *@brief	:	Main Program body for SPI Peripheral
 */

/******************* MAIN INCLUDES *********************/
#include <stdint.h>
#include "../../LIBRARY/STM32F446xx.h"
#include "../../LIBRARY/ErrTypes.h"


#include "../Inc/SPI_Interface.h"
#include "../Inc/SPI_Private.h"
/*******************************************************/


/********************** MAIN PV ************************/

static SPI_REG_t * SPIs[MAX_SPIs_NUMBER]={SPI1,SPI2,SPI3,SPI4};

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
			SPIs[SPI_Config->SPI_Num]->SPI_CR1 &= ~((BAUD_RATE_MASK)<<BAUD_RATE_START_BITS);
			SPIs[SPI_Config->SPI_Num]->SPI_CR1 |=  ((SPI_Config->BaudRate_Value)<<BAUD_RATE_START_BITS);
		}
		/*2- Set Clock Polarity*/
		SPIs[SPI_Config->SPI_Num]->SPI_CR1 &= ~((CLOCK_POL_MASK)<<CLOCK_POL_START_BITS);
		SPIs[SPI_Config->SPI_Num]->SPI_CR1 |=  ((SPI_Config->Clock_Polarity)<<CLOCK_POL_START_BITS);

		/*3- Set Clock Phase*/
		SPIs[SPI_Config->SPI_Num]->SPI_CR1 &= ~((CLOCK_PHASE_MASK)<<CLOCK_PHASE_START_BITS);
		SPIs[SPI_Config->SPI_Num]->SPI_CR1 |=  ((SPI_Config->Clock_Phase)<<CLOCK_PHASE_START_BITS);

		/*4- Set Transfer Mode*/
		if(SPI_Config->Transfer_Mode != TRANSFER_MODE_SIMPLEX)
		{
			SPIs[SPI_Config->SPI_Num]->SPI_CR1 &= ~((TRANSFER_MODE_MASK)<<TRANS_MODE_START_BITS);
			SPIs[SPI_Config->SPI_Num]->SPI_CR1 |=  ((SPI_Config->Transfer_Mode)<<TRANS_MODE_START_BITS);
		}
		else {
			SPIs[SPI_Config->SPI_Num]->SPI_CR1 &= ~((RX_ONLY_MODE_MASK)<<RX_ONLY_START_BITS);
			SPIs[SPI_Config->SPI_Num]->SPI_CR1 |=  (1<<RX_ONLY_START_BITS);

		}
		/*5- Set Frame Format Type*/
		SPIs[SPI_Config->SPI_Num]->SPI_CR1 &= ~((FRAME_TYPE_MASK)<<FRAME_TYPE_START_BITS);
		SPIs[SPI_Config->SPI_Num]->SPI_CR1 |=  ((SPI_Config->Frame_Type)<<FRAME_TYPE_START_BITS);

		/*6- Set CRC Enable State*/
		SPIs[SPI_Config->SPI_Num]->SPI_CR1 &= ~((CRC_ENABLE_MASK)<<CRC_ENABLE_START_BITS);
		SPIs[SPI_Config->SPI_Num]->SPI_CR1 |=  ((SPI_Config->CRC_State)<<CRC_ENABLE_START_BITS);

		/*7- Set Slave Management state*/
		SPIs[SPI_Config->SPI_Num]->SPI_CR1 &= ~((SLAVE_MANAGE_MASK)<<SLAVE_MANAGE_START_BITS);
		SPIs[SPI_Config->SPI_Num]->SPI_CR1 |=  ((SPI_Config->Slave_Manage_State)<<SLAVE_MANAGE_START_BITS);

		/*8- Set CHIP State*/
		SPIs[SPI_Config->SPI_Num]->SPI_CR1 &= ~((CHIP_MODE_MASK)<<CHIP_MODE_START_BITS);
		SPIs[SPI_Config->SPI_Num]->SPI_CR1 |=  ((SPI_Config->Chip_Mode)<<CHIP_MODE_START_BITS);

		/*9- Set Data Frame SIZE*/
		SPIs[SPI_Config->SPI_Num]->SPI_CR1 &= ~((FRAME_SIZE_MASK)<<FRAME_SIZE_START_BITS);
		SPIs[SPI_Config->SPI_Num]->SPI_CR1 |=  ((SPI_Config->Frame_Size)<<FRAME_SIZE_START_BITS);

		/*10- Set MultiMaster Ability State if Master*/
		if (SPI_Config->Chip_Mode == CHIP_MODE_MASTER)
		{
			SPIs[SPI_Config->SPI_Num]->SPI_CR2 &= ~((SSOE_MASK)<<SSOE_BIT_START);
			SPIs[SPI_Config->SPI_Num]->SPI_CR2 |=  ((SPI_Config->MultiMaster_State)<<SSOE_BIT_START);
		}
		/*11- Enable SPI*/
		SPIs[SPI_Config->SPI_Num]->SPI_CR1 |=  ((1)<<SPI_ENABLE_BIT_START);
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
					Flag_State	=	GET_BIT(SPIs[SPI_Config->SPI_Num]->SPI_SR , SPI_FLAGS_TXE);
				}
				/*Put Data in DR*/
				SPIs[SPI_Config->SPI_Num]->SPI_DR = *Data;
				Counter++;
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
						Flag_State	=	GET_BIT(SPIs[SPI_Config->SPI_Num]->SPI_SR , SPI_FLAGS_RXNE);
					}
					/*Read the Received data*/
					Received_Data[Counter++] = SPIs[SPI_Config->SPI_Num]->SPI_DR ;
				}
			}
			else if (SPI_Config->Chip_Mode == CHIP_MODE_MASTER)
			{
				while (Counter < Buffer_Size)
				{
					/* writing garbage in the Tx Buffer to start Receiving*/
					SPIs[SPI_Config->SPI_Num]->SPI_DR = GARBAGE_VALUE;
					/*Wait till data is Received*/
					while (Flag_State != FLAG_SET)
					{
						Flag_State	=	GET_BIT(SPIs[SPI_Config->SPI_Num]->SPI_SR , SPI_FLAGS_RXNE);
					}
					/*Read the Received data*/
					Received_Data[Counter++] = SPIs[SPI_Config->SPI_Num]->SPI_DR ;
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
		if (( SPI_Config->SPI_Num >=SPI_NUMBER1) && ( SPI_Config->SPI_Num <=SPI_NUMBER2))
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
			while( ! (GET_BIT(SPIs[SPI_Config->SPI_Num]->SPI_SR,SPI_FLAGS_TXE) ) );

			/*Put First Data in DR*/
			SPIs[SPI_Config->SPI_Num]->SPI_DR = Data[0];

			/*Enable Transmission complete interrupt*/
			SPIs[SPI_Config->SPI_Num]->SPI_CR2 |= (1<<(SPI_INTERRUPT_TXEIE));

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
Error_State_t SPI_Receive_IT(const SPI_CONFIGS_t * SPI_Config, uint8_t * Received_Data ,uint8_t Buffer_Size , void (* SPI_RXC_CallBackFunc)(void))
{
	Error_State_t 	Error_State = 	OK	;
	if ((NULL != Received_Data) && (NULL != SPI_RXC_CallBackFunc))
	{
		if (( SPI_Config->SPI_Num >=SPI_NUMBER1) && ( SPI_Config->SPI_Num <=SPI_NUMBER2))
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
				SPIs[SPI_Config->SPI_Num]->SPI_CR2 |= (1<<(SPI_INTERRUPT_RXNEIE));

			}
			else if (SPI_Config->Chip_Mode == CHIP_MODE_MASTER)
			{
				/*Set IRQ Source*/
				IRQ_Source[SPI_Config->SPI_Num] = SOURCE_RX_MASTER;
				/* writing garbage in the Tx Buffer to start Receiving*/
				SPIs[SPI_Config->SPI_Num]->SPI_DR = GARBAGE_VALUE;

				/*Enable Receive complete Interrupt*/
				SPIs[SPI_Config->SPI_Num]->SPI_CR2 |= (1<<(SPI_INTERRUPT_RXNEIE));
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
		SPIs[SPI_Num]->SPI_CR2 |= (1<<(RXDMAEN_BIT));
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
		SPIs[SPI_Num]->SPI_CR2 |= (1<<(TXDMAEN_BIT));
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
			SPIs[SPI_Num]->SPI_CR1 &= ~(1<<(SSI_BIT));
		}
		else if (Slave_State == SLAVE_STATE_DEACTIVATED)
		{
			SPIs[SPI_Num]->SPI_CR1 |= (1<<(SSI_BIT));
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
			SPIs[SPI_Num]->SPI_CR2 &= ~(1<<(SPI_INTERRUPT_TXEIE));

			/*Clear IRQ Source*/
			IRQ_Source[SPI_Num] = NO_SRC;

			/*Call The call Back Function*/
			SPI_pf_CallBackFuncs[SPI_Num][SPI_FLAGS_TXE]();
		}

		/*Buffer isn't completely sent*/
		else {
			/*Send the next data element in the buffer*/
			SPIs[SPI_Num]->SPI_DR = Global_Data_Buffer[Counter++];
		}
	}
	else if (IRQ_Source[SPI_Num] == SOURCE_RX_SLAVE)
	{
		static uint16_t Counter=0;
		/*Whole buffer Receiving is done*/
		if (Global_Data_Size==1)
		{
			/*Disable the RXC interrupt*/
			SPIs[SPI_Num]->SPI_CR2 &= ~(1<<(SPI_INTERRUPT_RXNEIE));

			/*Clear IRQ Source*/
			IRQ_Source[SPI_Num] = NO_SRC;

			/*Receive the next data element*/
			Global_Received_Data[Counter++] = SPIs[SPI_Num]->SPI_DR;

			/*Call The call Back Function*/
			SPI_pf_CallBackFuncs[SPI_Num][SPI_FLAGS_RXNE]();

		}
		else
		{
			if (Counter == Global_Data_Size)
			{
				/*Disable the RXC interrupt*/
				SPIs[SPI_Num]->SPI_CR2 &= ~(1<<(SPI_INTERRUPT_RXNEIE));

				/*Clear IRQ Source*/
				IRQ_Source[SPI_Num] = NO_SRC;

				/*Call The call Back Function*/
				SPI_pf_CallBackFuncs[SPI_Num][SPI_FLAGS_RXNE]();
			}
			else {
				/*Receive the next data element*/
				Global_Received_Data[Counter++] = SPIs[SPI_Num]->SPI_DR;
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
			SPIs[SPI_Num]->SPI_CR2 &= ~(1<<(SPI_INTERRUPT_RXNEIE));

			/*Clear IRQ Source*/
			IRQ_Source[SPI_Num] = NO_SRC;

			/*Call The call Back Function*/
			SPI_pf_CallBackFuncs[SPI_Num][SPI_FLAGS_RXNE]();
		}
		else {

			/* writing garbage in the Tx Buffer to start Receiving*/
			SPIs[SPI_Num]->SPI_DR = GARBAGE_VALUE;

			/*Receive the next data element*/
			Global_Received_Data[Counter++] = SPIs[SPI_Num]->SPI_DR;
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
            {
                /* Set Role Globally */
                SPI_TransceiveBufferNodeRole[Config->SPINumber] = SPI_SLAVE;

                /* Enable Receive Buffer Not Empty Interrupt */
                SPI[Config->SPINumber]->CR2 |= (1 << SPI_RXNEIE);
            }
            else if (Config->NodeRole == SPI_MASTER)
            {
                /* Set Role Globally */
                SPI_TransceiveBufferNodeRole[Config->SPINumber] = SPI_MASTER;

                /* Wait Until The Transmit Buffer is Empty */
                SPI_WaitUntilFlagSet(Config->SPINumber, SPI_TRANSMIT_BUFFER_E_FLAG);

                /* Send Any Value to Initiate Communication */
                SPI[Config->SPINumber]->DR = 0xFF;

                /* Enable Receive Buffer Not Empty Interrupt */
                SPI[Config->SPINumber]->CR2 |= (1 << SPI_RXNEIE);
            }
        }

        /* Sending Buffer is Required */
        if (BufferToSend != NULL)
        {
            /* Set Direction Globally */
            SPI_TransceiveBufferRoleDirection[Config->SPINumber] = SPI_ROLE_TRANSMIT;

            /* Set Buffer To Send Globally */
            SPI_TransceiveBufferToBeSent[Config->SPINumber] = BufferToSend;

            /* wait Until The Transmit Buffer is Empty */
            SPI_WaitUntilFlagSet(Config->SPINumber, SPI_TRANSMIT_BUFFER_E_FLAG);

            /* Send First Data Item */
            SPI[Config->SPINumber]->DR = SPI_TransceiveBufferToBeSent[Config->SPINumber][0];

            /* Enable Receive Buffer Not Empty Interrupt */
            SPI[Config->SPINumber]->CR2 |= (1 << SPI_RXNEIE);
        }
    }
    return Local_u8ErrorStatus;
}

/**
 * @brief  : This Function is Used to Set The SPI Mode According to The Required Configuration in The Configuration Struct
 *
 * @param  : Config => This is a Pointer to Struct of Type SPI_Config_t That Holds The Required Configuration
 * @return : ERRORS_t   => This Return Parameter is Used to Indicate The Function Execution If Executed Correctly or NOT
 * @note   : Private Function Used Inside The SPI_Init Function
 */
static ERRORS_t SPI_SetMode(SPI_Config_t *Config)
{
    ERRORS_t Local_u8ErrorStatus = SPI_OK;
    if (Config != NULL)
    {
        switch (Config->Mode)
        {
        case SPI_FULL_DUPLEX:
            SPI[Config->SPINumber]->CR1 &= (~(1 << SPI_BIDIMODE));
            SPI[Config->SPINumber]->CR1 &= (~(1 << SPI_RXONLY));
            break;

        case SPI_HALF_DUPLEX_TX:
            SPI[Config->SPINumber]->CR1 |= (1 << SPI_BIDIMODE);
            SPI[Config->SPINumber]->CR1 |= (1 << SPI_BIDIOE);
            break;

        case SPI_HALF_DUPLEX_RX:
            SPI[Config->SPINumber]->CR1 |= (1 << SPI_BIDIMODE);
            SPI[Config->SPINumber]->CR1 &= (~(1 << SPI_BIDIOE));
            break;

        case SPI_SIMPLEX_TX:
            SPI[Config->SPINumber]->CR1 &= (~(1 << SPI_BIDIMODE));
            SPI[Config->SPINumber]->CR1 &= (~(1 << SPI_RXONLY));
            break;

        case SPI_SIMPLEX_RX:
            SPI[Config->SPINumber]->CR1 &= (~(1 << SPI_BIDIMODE));
            SPI[Config->SPINumber]->CR1 |= (1 << SPI_RXONLY);
            break;

        default:
            Local_u8ErrorStatus = SPI_NOK;
            break;
        }
    }
    else
    {
        Local_u8ErrorStatus = NULL_POINTER;
    }
    return Local_u8ErrorStatus;
}

/**
 * @brief  : This Function is Used to Check on SPI Configuration Structure Before Passing it to The SPI_Init Function
 *
 * @param  : Configuration => This is a Pointer to Struct of Type SPI_Config_t That Holds The Required Configuration
 * @return : ERRORS_t     => This Return Parameter is Used to Indicate The Function Execution If Executed Correctly or NOT
 */
static ERRORS_t SPI_CheckConfig(SPI_Config_t *Configuration)
{
    ERRORS_t Local_u8ErrorStatus = SPI_OK;

    if (Configuration->SPINumber < SPI1_APB2 || Configuration->SPINumber > SPI4_APB2 ||
        Configuration->BaudRate < SPI_PERIPH_CLK_BY2 || Configuration->BaudRate > SPI_PERIPH_CLK_BY256 ||
        Configuration->ClockPhase < SPI_CAPTURE_LEADING || Configuration->ClockPhase > SPI_CAPTURE_TRAILING ||
        Configuration->ClockPolarity < SPI_CLK_IDLE_LOW || Configuration->ClockPolarity > SPI_CLK_IDLE_HIGH ||
        Configuration->DataWidth < SPI_1BYTE || Configuration->DataWidth > SPI_2BYTE ||
        Configuration->Direction < SPI_MSB_FIRST || Configuration->Direction > SPI_LSB_FIRST ||
        Configuration->CRC_Status < SPI_CRC_DIS || Configuration->CRC_Status > SPI_CRC_EN ||
        Configuration->Mode < SPI_FULL_DUPLEX || Configuration->Mode > SPI_SIMPLEX_RX ||
        Configuration->NodeRole < SPI_SLAVE || Configuration->NodeRole > SPI_MASTER ||
        Configuration->SlaveManage < SPI_HW_SLAVE_MANAGE || Configuration->SlaveManage > SPI_SW_SLAVE_MANAGE ||
        Configuration->SlaveSelectOutputType < SPI_SS_OUT_DIS || Configuration->SlaveSelectOutputType > SPI_SS_OUT_EN ||
        Configuration->InterruptEnable.RXNE < SPI_IT_DIS || Configuration->InterruptEnable.RXNE > SPI_IT_EN ||
        Configuration->InterruptEnable.TXE < SPI_IT_DIS || Configuration->InterruptEnable.TXE > SPI_IT_EN ||
        Configuration->InterruptEnable.ERR < SPI_IT_DIS || Configuration->InterruptEnable.ERR > SPI_IT_EN)
    {
        Local_u8ErrorStatus = SPI_INVALID_CONFIG;
    }

    return Local_u8ErrorStatus;
}

/**
 * @brief  : This Function is Used to Handle The SPI Interrupts
 *
 * @param  : SPINumber => This Parameter is Used to Select The SPI Peripheral Number to Be Used -> @SPI_t
 * @return : ERRORS_t => This Return Parameter is Used to Indicate The Function Execution If Executed Correctly or NOT
 * @note   : This Function is Used Inside The SPI Interrupt Handlers (Private Function)
 */
static ERRORS_t SPI_HANDLE_IT(SPI_t SPINumber)
{
    ERRORS_t Local_u8ErrorStatus = SPI_OK;

    if (SPINumber < SPI1_APB2 || SPINumber > SPI4_APB2)
    {
        Local_u8ErrorStatus = SPI_NOK;
    }
    else
    {
        /* Correct Parameter */

        /* IF The IRQ Source is Transceive Data */
        if (IRQ_SRC[SPINumber] == SPI_TRANSCEIVE_DATA_IRQ_SRC)
        {
            /* Clear IRQ Source */
            IRQ_SRC[SPINumber] = SPI_NO_IRQ_SRC;

            /* Reading is Required Only */
            /* Read the Received Data */
            if (SPI_TransceiveDataToBeReceived[SPINumber] != NULL)
            {
                *SPI_TransceiveDataToBeReceived[SPINumber] = SPI[SPINumber]->DR;
            }

            /* Disable  Rx Buffer Not Empty Interrupt */
            SPI[SPINumber]->CR2 &= (~(1 << SPI_RXNEIE));

            /* Invoke Call Back Function */
            if (SPI_PTR_TO_FUNC[SPINumber][SPI_IT_RXNE] != NULL)
            {
                SPI_PTR_TO_FUNC[SPINumber][SPI_IT_RXNE]();
            }
        }

        /* IF The IRQ Source is Transceive Buffer */
        if (IRQ_SRC[SPINumber] == SPI_TRANSCEIVE_BUFFER_IRQ_SRC)
        {
            /* Variable to Indicate if The Buffer Size is Reached or Not */
            static uint8_t BufferSizeReached = SPI_BUFFER_SIZE_NOT_REACHED;

            /* Counter to Indicate The Number of Data Sent or Received For Slave Role */
            static uint16_t TransmitCounterSlave = 1;
            static uint16_t ReceiveCounterSlave = 0;

            /* Counter to Indicate The Number of Data Sent or Received For Master Role */
            static uint16_t TransmitCounterMaster = 1;
            static uint16_t ReceiveCounterMaster = 0;

            switch (SPI_TransceiveBufferNodeRole[SPINumber])
            {
            case SPI_SLAVE:

                /* Slave is Transmitter */
                if (SPI_TransceiveBufferRoleDirection[SPINumber] == SPI_ROLE_TRANSMIT)
                {

                    /* if Buffer Size is Reached */
                    if (TransmitCounterSlave == SPI_TransceiveBufferSize[SPINumber])
                    {
                        BufferSizeReached = SPI_BUFFER_SIZE_REACHED;
                        /* Reset Counter */
                        TransmitCounterSlave = 1;
                    }
                    else
                    {
                        /* Send Next Data */
                        SPI[SPINumber]->DR = SPI_TransceiveBufferToBeSent[SPINumber][TransmitCounterSlave++];
                    }
                }
                /* Slave is Receiver */
                else if (SPI_TransceiveBufferRoleDirection[SPINumber] == SPI_ROLE_RECEIVE)
                {

                    /* Receive the Data */
                    SPI_TransceiveBufferToBeReceived[SPINumber][ReceiveCounterSlave++] = SPI[SPINumber]->DR;

                    /* if Buffer Size is Reached */
                    if (ReceiveCounterSlave == SPI_TransceiveBufferSize[SPINumber])
                    {
                        BufferSizeReached = SPI_BUFFER_SIZE_REACHED;
                        /* Reset Counter */
                        ReceiveCounterSlave = 0;
                    }
                }

                break;

            case SPI_MASTER:

                /* Master is Transmitter */
                if (SPI_TransceiveBufferRoleDirection[SPINumber] == SPI_ROLE_TRANSMIT)
                {
                    if (TransmitCounterMaster == SPI_TransceiveBufferSize[SPINumber])
                    {
                        BufferSizeReached = SPI_BUFFER_SIZE_REACHED;
                        /* Reset Counter */
                        TransmitCounterMaster = 1;
                    }
                    else
                    {
                        /* Send Next Data */
                        SPI[SPINumber]->DR = SPI_TransceiveBufferToBeSent[SPINumber][TransmitCounterMaster++];
                    }
                }
                /* Master is Receiver */
                else if (SPI_TransceiveBufferRoleDirection[SPINumber] == SPI_ROLE_RECEIVE)
                {

                    /* Receive the Data */
                    SPI_TransceiveBufferToBeReceived[SPINumber][ReceiveCounterMaster++] = SPI[SPINumber]->DR;

                    if (ReceiveCounterMaster == SPI_TransceiveBufferSize[SPINumber])
                    {
                        BufferSizeReached = SPI_BUFFER_SIZE_REACHED;
                        /* Reset Counter */
                        ReceiveCounterMaster = 0;
                    }
                    else
                    {
                        /* Send Any Value to Initiate Communication */
                        SPI[SPINumber]->DR = 0xFF;
                    }
                }
                break;
            }
            /* If Buffer Size is Reached  , Disable Rx Buffer Not Empty Interrupt & Invoke Call Back Function */
            if (BufferSizeReached == SPI_BUFFER_SIZE_REACHED)
            {

                /* Clear IRQ Source */
                IRQ_SRC[SPINumber] = SPI_NO_IRQ_SRC;

                /* Disable  Rx Buffer Not Empty Interrupt */
                SPI[SPINumber]->CR2 &= (~(1 << SPI_RXNEIE));

                /* Invoke Call Back Function */
                if (SPI_PTR_TO_FUNC[SPINumber][SPI_IT_RXNE] != NULL)
                {
                    SPI_PTR_TO_FUNC[SPINumber][SPI_IT_RXNE]();
                }
                /* Reset Variables*/
                BufferSizeReached = SPI_BUFFER_SIZE_NOT_REACHED;
            }
        }
    }
    return Local_u8ErrorStatus;
}

/* ======================================================================
 * INTERRUPT HANDLERS
 * ====================================================================== */

/* SPI1 Interrupt Handler */
void SPI1_IRQHandler(void)
{
    SPI_HANDLE_IT(SPI1_APB2);
}

/* SPI2 Interrupt Handler */
void SPI2_IRQHandler(void)
{
    SPI_HANDLE_IT(SPI2_APB1);
}

/* SPI3 Interrupt Handler */
void SPI3_IRQHandler(void)
{
    SPI_HANDLE_IT(SPI3_APB1);
}

/* SPI4 Interrupt Handler */
void SPI4_IRQHandler(void)
{
    SPI_HANDLE_IT(SPI4_APB2);
}
