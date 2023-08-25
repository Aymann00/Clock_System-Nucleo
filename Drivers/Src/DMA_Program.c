/*
 ******************************************************************************
 * @file           : DMA_Program.c
 * @Author         : Mohammed Ayman Shalaby
 * @brief          : Main program body
 * @Date           : Jul 20, 2023
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 Ayman.
 * All rights reserved.
 *
 ******************************************************************************
 */

/*==============================================================================================================================================
 * INCLUDES SECTION START
 *==============================================================================================================================================*/
#include <stdint.h>

#include "STM32F446xx.h"
#include "../Inc/ErrTypes.h"

#include "../Inc/DMA_Interface.h"
#include "../Inc/DMA_Private.h"

/*==============================================================================================================================================
 * INCLUDES SECTION END
 *==============================================================================================================================================*/

/*==============================================================================================================================================
 * GLOBAL VARIABLES SECTION START
 *==============================================================================================================================================*/

static void (*DMA1_STREAM_PTR_TOFUNC[8][DMA_INT_NUM])(void) = {NULL};
static void (*DMA2_STREAM_PTR_TOFUNC[8][DMA_INT_NUM])(void) = {NULL};

static DMA_RegDef_t *DMA[2] = {DMA1, DMA2};

/*==============================================================================================================================================
 * GLOBAL VARIABLES SECTION END
 *==============================================================================================================================================*/

/*==============================================================================================================================================
 * MODULES IMPLEMENTATION
 *==============================================================================================================================================*/

/**
 * @brief : The Function Clears A Specified  Interrupt Flag of A Specified Stream in a Certain DMA Controller of the Two DMA Controllers
 *
 * @param : DMAController => Enum that holds Options for Available DMA Controllers we have -> To choose Check enum ( @DMA_CONTROLLER_t )
 * @param : StreamNumber => Enum that holds Options for Available Streams in the DMA Controller -> Check Options ( @DMA_STREAMS_t )
 * @param : InterruptFlag => Enum that holds All Interrupt Flags that can Generate Interrupt in a Certain Stream -> Check Options ( @DMA_Interrupt_Flags_t )
 * @return: ERRORS_t => Error Status To Indicate if Function Worked Properly
 */
Error_State_t DMA_ClearInterruptFlag(DMA_CONTROLLER_t DMAController, DMA_STREAMS_t StreamNumber, DMA_Interrupt_Flag_t InterruptFlag)
{
	Error_State_t Local_u8ErrorStatus = DMA_OK;

    uint8_t Local_u8RegisterShift = 0;

    if (StreamNumber < DMA_STREAM0 || StreamNumber > DMA_STREAM7 ||
        InterruptFlag < FIFO_ERROR_IT_FLAG || InterruptFlag > TRANSFER_COMPLETE_IT_FLAG ||
        DMAController < DMA1_CONTROLLER || DMAController > DMA2_CONTROLLER)
    {
        Local_u8ErrorStatus = DMA_NOK;
    }
    else
    {
        /* Configurations Are Correct */

        if (StreamNumber % 2 == 0)
        {
            /* Even */
            Local_u8RegisterShift = 0;
        }
        else
        {
            /* Odd */
            Local_u8RegisterShift = 6;
        }

        DMA[DMAController]->IFCR[StreamNumber / 2] = (1 << InterruptFlag << Local_u8RegisterShift);
    }
    return Local_u8ErrorStatus;
}

/**
 * @brief : The Function Reads A Specified  Interrupt Flag of A Specified Stream in a Certain DMA Controller of the Two DMA Controllers
 *
 * @param : DMAController => Enum that holds Options for Available DMA Controllers we have -> To choose Check enum ( @DMA_CONTROLLER_t )
 * @param : StreamNumber => Enum that holds Options for Available Streams in the DMA Controller -> Check Options ( @DMA_STREAMS_t )
 * @param : InterruptFlag => Enum that holds All Interrupt Flags that can Generate Interrupt in a Certain Stream -> Check Options ( @DMA_Interrupt_Flags_t )
 * @param : InterruptFlagStatus => Pointer to a Variable that will hold the Status of the Interrupt Flag -> Check Options ( @DMA_Interrupt_Flag_Status_t )
 * @return: ERRORS_t => Error Status To Indicate if Function Worked Properly
 */
Error_State_t DMA_ReadInterruptFlag(DMA_CONTROLLER_t DMAController, DMA_STREAMS_t StreamNumber, DMA_Interrupt_Flag_t InterruptFlag, DMA_Interrupt_Flag_Status_t *InterruptFlagStatus)
{
	Error_State_t Local_u8ErrorStatus = DMA_OK;

    uint8_t Local_u8RegisterShift = 0;

    if (StreamNumber < DMA_STREAM0 || StreamNumber > DMA_STREAM7 ||
        InterruptFlag < FIFO_ERROR_IT_FLAG || InterruptFlag > TRANSFER_COMPLETE_IT_FLAG ||
        DMAController < DMA1_CONTROLLER || DMAController > DMA2_CONTROLLER || InterruptFlagStatus == NULL)
    {
        Local_u8ErrorStatus = DMA_NOK;
    }
    else
    {
        /* Configurations Are Correct */

        if (StreamNumber % 2 == 0)
        {
            /* Even */
            Local_u8RegisterShift = 0;
        }
        else
        {
            /* Odd */
            Local_u8RegisterShift = 6;
        }
        *InterruptFlagStatus = ((DMA[DMAController]->ISR[StreamNumber / 2] >> InterruptFlag >> Local_u8RegisterShift) & 0x01);
    }
    return Local_u8ErrorStatus;
}

/**
 * @brief : The Function Selects a Certain Channel on a Certain Stream in a Specifec DMA Controller
 *
 * @param : DMAController => Enum that holds Options for Available DMA Controllers we have -> To choose Check enum ( @DMA_CONTROLLER_t )
 * @param : StreamNumber => Enum that holds Options for Available Streams in the DMA Controller -> Check Options ( @DMA_STREAMS_t )
 * @param : ChannelNumber => Enum that holds All Possible Channels that can Request a Certain Stream -> Check Options ( @DMA_CHANNEL_t )
 * @return: ERRORS_t => Error Status To Indicate if Function Worked Properly
 */
Error_State_t DMA_SetChannel(DMA_CONTROLLER_t DMAController, DMA_STREAMS_t StreamNumber, DMA_CHANNEL_t ChannelNumber)
{
	Error_State_t Local_u8ErrorStatus = DMA_OK;

    if (StreamNumber < DMA_STREAM0 || StreamNumber > DMA_STREAM7 ||
        ChannelNumber < DMA_CHANNEL0 || ChannelNumber > DMA_CHANNEL7 ||
        DMAController < DMA1_CONTROLLER || DMAController > DMA2_CONTROLLER)
    {
        Local_u8ErrorStatus = DMA_NOK;
    }
    else
    {
        /* Configurations Are Correct */

        DMA[DMAController]->STREAM[StreamNumber].CR &= (DMA_CHSEL_MASK);
        DMA[DMAController]->STREAM[StreamNumber].CR |= (ChannelNumber << CHSEL);
    }
    return Local_u8ErrorStatus;
}

/**
 * @brief : The Function Set a Priority Level to a Certain Stream in a DMA Controller
 *
 * @param : DMAController => Enum that holds Options for Available DMA Controllers we have -> To choose Check enum ( @DMA_CONTROLLER_t )
 * @param : StreamNumber => Enum that holds Options for Available Streams in the DMA Controller -> Check Options ( @DMA_STREAMS_t )
 * @param : PriorityLevel => Enum that holds All Possible Values of Priority Levels you can give to a certain stream -> Check Options ( @DMA_PRIORITY_t )
 * @return: ERRORS_t => Error Status To Indicate if Function Worked Properly
 */
Error_State_t DMA_SetStreamPriority(DMA_CONTROLLER_t DMAController, DMA_STREAMS_t StreamNumber, DMA_PRIORITY_t PriorityLevel)
{
	Error_State_t Local_u8ErrorStatus = DMA_OK;

    if (DMAController < DMA1_CONTROLLER || DMAController > DMA2_CONTROLLER ||
        StreamNumber < DMA_STREAM0 || StreamNumber > DMA_STREAM7 ||
        PriorityLevel < DMA_LOW_PRIORITY || PriorityLevel > DMA_VERY_HIGH_PRIORITY)
    {
        Local_u8ErrorStatus = DMA_NOK;
    }
    else
    {
        /* Configurations Are Correct */

        DMA[DMAController]->STREAM[StreamNumber].CR &= (DMA_PL_MASK);
        DMA[DMAController]->STREAM[StreamNumber].CR |= (PriorityLevel << PL);
    }
    return Local_u8ErrorStatus;
}

/**
 * @brief : This Function Initializes DMA with All Configurations User Choose Through Making a Structure & Passing It By Reference
 *
 * @param : Init => Pointer to Struct ( DMA_INIT_STRUCT_t ) This Struct Holds All Configurations of the DMA
 *            1- DMAController   => The Required DMA Controller to Use -> Check All Available DMA Controllers Options ( @DMA_CONTROLLER_t )
 *            2- StreamNumber    => The Required Stream to Use -> Check All Available Stream Options ( @DMA_STREAMS_t )
 *            3- ChannelNumber   => Channel To Select To Request A Certain Stream -> Check All Available Channels Options ( @DMA_CHANNEL_t )
 *            4- PeriphBurst     => Peripheral Burst Size to Choose -> Check All Available Options of Peripheral Burst ( @DMA_PERIPH_BURST_t )
 *            5- MemBurst        => Memory Burst Size to Choose -> Check All Available Options of Memory Burst ( @DMA_MEM_BURST_t )
 *            6- Priority        => Priority Level to Choose for a certain Stream -> Check All Available Options of Priority levels ( @DMA_PRIORITY_t )
 *            7- MemDataWidth    => Memory Data Width to Choose for the Data Transfer -> Check All Avaiable Options ( @DMA_MEM_WIDTH_t )
 *            8- PeriphDataWidth => Peripheral Data Width to Choose for the Data Transfer -> Check All Available Options ( @DMA_PERIPH_WIDTH_t )
 *            9- MemInc          => Memory Increment To Choose If you want to enable it or disable - > Check For Options ( @DMA_MINC_t )
 *           10- PeriphInc       => Peripheral Increment To Choose If you want to enable it or disable - > Check For Options ( @DMA_PINC_t )
 *           11- Mode            => To Specify Mode of the DMA to Choose Between Options -> Check For Options ( @DMA_MODE_t )
 *           12- Direction       => To Specify Data Transfer Direction -> Check For Options ( @DMA_DATA_DIRECTION_t )
 *           13- EnableIT        => Struct To Enable or Disable Multilple Interrupts -> Check For Options ( @DMA_INT_ENABLE_STRUCT_t )
 *           14- FIFOMode        => To Specify FIFO Mode if Enabled or NOT -> Check For Options ( @DMA_FIFO_MODE_t )
 *           15- FIFOThreshold   => To Specify FIFO Threshold Level  -> Check For Options ( @DMA_FIFO_THRESHOLD_t )
 * @return: ERRORS_t => Error Status To Indicate if Function Worked Properly
 */
Error_State_t DMA_Init(DMA_INIT_STRUCT_t *Init)
{
	Error_State_t Local_u8ErrorStatus = OK;

    if (DMA_NOK != DMA_CheckInitConfig(Init))
    {
        /* Configuration Are OK */

        /* Make Sure that Stream is Disabled Before Doing Any Configurations */
        DMA_DisableStream(Init->DMAController, Init->StreamNumber);

        /* Set Channel */
        DMA_SetChannel(Init->DMAController, Init->StreamNumber, Init->ChannelNumber);

        /* Set Memory Burst */
        DMA[Init->DMAController]->STREAM[Init->StreamNumber].CR &= (DMA_MBURST_MASK);
        DMA[Init->DMAController]->STREAM[Init->StreamNumber].CR |= (Init->MemBurst << MBURST);

        /* Set Peripheral Burst */
        DMA[Init->DMAController]->STREAM[Init->StreamNumber].CR &= (DMA_PBURST_MASK);
        DMA[Init->DMAController]->STREAM[Init->StreamNumber].CR |= (Init->PeriphBurst << PBURST);

        /* Set Priority Level */
        DMA_SetStreamPriority(Init->DMAController, Init->StreamNumber, Init->Priority);

        /* Set Memory Data Size */
        DMA[Init->DMAController]->STREAM[Init->StreamNumber].CR &= (DMA_MSIZE_MASK);
        DMA[Init->DMAController]->STREAM[Init->StreamNumber].CR |= (Init->MemDataWidth << MSIZE);

        /* Set Peripheral Data Size */
        DMA[Init->DMAController]->STREAM[Init->StreamNumber].CR &= (DMA_PSIZE_MASK);
        DMA[Init->DMAController]->STREAM[Init->StreamNumber].CR |= (Init->PeriphDataWidth << MSIZE);

        /* Data Transfer Direction */
        DMA[Init->DMAController]->STREAM[Init->StreamNumber].CR &= (DMA_DIR_MASK);
        DMA[Init->DMAController]->STREAM[Init->StreamNumber].CR |= (Init->Direction << DIR);

        /* Set DMA Mode */
        /* Circular OR Normal */
        DMA[Init->DMAController]->STREAM[Init->StreamNumber].CR &= (DMA_CIRC_MASK);
        DMA[Init->DMAController]->STREAM[Init->StreamNumber].CR |= (Init->Mode << CIRC);

        /* Set Double Buffer Mode */
        DMA[Init->DMAController]->STREAM[Init->StreamNumber].CR &= (DMA_DBM_MASK);
        DMA[Init->DMAController]->STREAM[Init->StreamNumber].CR |= (Init->DoubleBuffer << DBM);

        /* Set Memory Increment Option */
        DMA[Init->DMAController]->STREAM[Init->StreamNumber].CR &= (DMA_MINC_MASK);
        DMA[Init->DMAController]->STREAM[Init->StreamNumber].CR |= (Init->MemInc << MINC);

        /* Set Peripheral Increment Option */
        DMA[Init->DMAController]->STREAM[Init->StreamNumber].CR &= (DMA_PINC_MASK);
        DMA[Init->DMAController]->STREAM[Init->StreamNumber].CR |= (Init->PeriphInc << PINC);

        /* Set Interrupts Configurations Enabled OR Disabled */
        DMA_EnableIT(Init);

        /* FIFO Mode */
        DMA[Init->DMAController]->STREAM[Init->StreamNumber].FCR &= (DMA_DMDIS_MASK);
        DMA[Init->DMAController]->STREAM[Init->StreamNumber].FCR |= (Init->FIFOMode << DMDIS);

        /* FIFO Threshold Selection */
        DMA[Init->DMAController]->STREAM[Init->StreamNumber].FCR &= (DMA_FTH_MASK);
        DMA[Init->DMAController]->STREAM[Init->StreamNumber].FCR |= (Init->FIFOThreshold << FTH);
    }
    else
    {
        /* Configuration Are NOK */

        Local_u8ErrorStatus = DMA_WRONG_CONFIGURATION;
    }
    return Local_u8ErrorStatus;
}

/**
 * @brief : The Function Configures Interrupt Enabling Or Disabling Of All Interrupts in a Certain Stream in a Certain DMA Controller
 *
 * @param : DMA_InitConfig =>  Struct that holds all configurations of the DMA -> Check Struct ( @DMA_INIT_STRUCT_t )
 * @note  : This Function is Being Called in DMA_Init Function , So No Need to Call it Again Unless You Want to Change Interrupt Configurations
 * @return: ERRORS_t => Error Status To Indicate if Function Worked Properly
 */
Error_State_t DMA_EnableIT(DMA_INIT_STRUCT_t *DMA_InitConfig)
{
	Error_State_t Local_u8ErrorStatus = DMA_OK;

    if (DMA_InitConfig->EnableIT.DirectModeErrorIT < DMA_INT_DISABLE || DMA_InitConfig->EnableIT.DirectModeErrorIT > DMA_INT_ENABLE ||
        DMA_InitConfig->EnableIT.FIFOErrorIT < DMA_INT_DISABLE || DMA_InitConfig->EnableIT.FIFOErrorIT > DMA_INT_ENABLE ||
        DMA_InitConfig->EnableIT.HalfTransferIT < DMA_INT_DISABLE || DMA_InitConfig->EnableIT.HalfTransferIT > DMA_INT_ENABLE ||
        DMA_InitConfig->EnableIT.TransferCompleteIT < DMA_INT_DISABLE || DMA_InitConfig->EnableIT.TransferCompleteIT > DMA_INT_ENABLE ||
        DMA_InitConfig->EnableIT.TransferErrorIT < DMA_INT_DISABLE || DMA_InitConfig->EnableIT.TransferErrorIT > DMA_INT_ENABLE)
    {
        Local_u8ErrorStatus = DMA_NOK;
    }
    else
    {
        /* Configurations Are Correct */

        /* Transfer Complete IT */
        DMA[DMA_InitConfig->DMAController]->STREAM[DMA_InitConfig->StreamNumber].CR &= (DMA_TCIE_MASK);
        DMA[DMA_InitConfig->DMAController]->STREAM[DMA_InitConfig->StreamNumber].CR |= (DMA_InitConfig->EnableIT.TransferCompleteIT << TCIE);

        /* Half Transfer Interrupt */
        DMA[DMA_InitConfig->DMAController]->STREAM[DMA_InitConfig->StreamNumber].CR &= (DMA_HTIE_MASK);
        DMA[DMA_InitConfig->DMAController]->STREAM[DMA_InitConfig->StreamNumber].CR |= (DMA_InitConfig->EnableIT.HalfTransferIT << HTIE);

        /* Transfer Error Interrupt */
        DMA[DMA_InitConfig->DMAController]->STREAM[DMA_InitConfig->StreamNumber].CR &= (DMA_TEIE_MASK);
        DMA[DMA_InitConfig->DMAController]->STREAM[DMA_InitConfig->StreamNumber].CR |= (DMA_InitConfig->EnableIT.TransferErrorIT << TEIE);

        /* Direct Mode Error Interrupt */
        DMA[DMA_InitConfig->DMAController]->STREAM[DMA_InitConfig->StreamNumber].CR &= (DMA_DMEIE_MASK);
        DMA[DMA_InitConfig->DMAController]->STREAM[DMA_InitConfig->StreamNumber].CR |= (DMA_InitConfig->EnableIT.DirectModeErrorIT << DMEIE);

        /* FIFO Error Interrupt */
        DMA[DMA_InitConfig->DMAController]->STREAM[DMA_InitConfig->StreamNumber].FCR &= (DMA_FEIE_MASK);
        DMA[DMA_InitConfig->DMAController]->STREAM[DMA_InitConfig->StreamNumber].FCR |= (DMA_InitConfig->EnableIT.FIFOErrorIT << FEIE);
    }
    return Local_u8ErrorStatus;
}

/**
 * @brief : This Function Disables a Certain Stream in a Certain DMA Controller
 *
 * @param  : DMANumber    =>  Enum that holds Options for Available DMA Controllers we have -> To choose Check enum ( @DMA_CONTROLLER_t )
 * @param  : StreamNumber => Enum that holds Options for Available Streams in the DMA Controller -> Check Options ( @DMA_STREAMS_t )
 * @return : ERRORS_t => Error Status To Indicate if Function Worked Properly
 */
Error_State_t DMA_DisableStream(DMA_CONTROLLER_t DMANumber, DMA_STREAMS_t StreamNumber)
{
	Error_State_t Local_u8ErrorStatus = DMA_OK;

    if (DMANumber < DMA1_CONTROLLER || DMANumber > DMA2_CONTROLLER ||
        StreamNumber < DMA_STREAM0 || StreamNumber > DMA_STREAM7)
    {
        Local_u8ErrorStatus = DMA_NOK;
    }
    else
    {
        /* Configurations Are Correct */

        DMA[DMANumber]->STREAM[StreamNumber].CR &= (DMA_EN_MASK);

        /* Wait Until Stream is Disabled */
        while (((DMA[DMANumber]->STREAM[StreamNumber].CR >> EN) & 1))
            ;
    }
    return Local_u8ErrorStatus;
}

/**
 * @brief : This Function Sets Source Address & Distination Address & Enables the Stream
 *
 * @param  : InitConfig  => Struct that holds all configurations of the DMA -> Check Struct ( @DMA_INIT_STRUCT_t )
 * @param  : SrcAddress  => Pointer to a Variable that holds the Source Address
 * @param  : DestAddress => Pointer to a Variable that holds the Destination Address
 * @param  : DataLength  => Variable that holds the Number of Data Transfers
 * @return : ERRORS_t => Error Status To Indicate if Function Worked Properly
 */
Error_State_t DMA_StartTransfer(DMA_INIT_STRUCT_t *InitConfig, uint32_t *SrcAddress, uint32_t *DestAddress, uint16_t DataLength)
{
	Error_State_t Local_u8ErrorStatus = DMA_OK;

    if (DMA_NOK != DMA_CheckInitConfig(InitConfig))
    {
        /* Set Number of Data Transfers */
        DMA[InitConfig->DMAController]->STREAM[InitConfig->StreamNumber].NDTR = DataLength;

        /* Check on Modes */
        /* Set Destination & Source Address According to Data Transfer Direction */
        switch (InitConfig->Direction)
        {
        case DMA_PERIPH_TO_MEM:
        case DMA_MEM_TO_MEM:
            /* Set Source */
            DMA[InitConfig->DMAController]->STREAM[InitConfig->StreamNumber].PAR = (uint32_t)SrcAddress;

            /* Set Destination */
            DMA[InitConfig->DMAController]->STREAM[InitConfig->StreamNumber].M0AR = (uint32_t)DestAddress;
            break;

        case DMA_MEM_TO_PERIPH:
            /* Set Source */
            DMA[InitConfig->DMAController]->STREAM[InitConfig->StreamNumber].M0AR = (uint32_t)SrcAddress;

            /* Set Destination */
            DMA[InitConfig->DMAController]->STREAM[InitConfig->StreamNumber].PAR = (uint32_t)DestAddress;
            break;
        }

        /* Enable Stream */
        DMA[InitConfig->DMAController]->STREAM[InitConfig->StreamNumber].CR |= (1 << EN);
    }
    else
    {
        Local_u8ErrorStatus = DMA_NOK;
    }
    return Local_u8ErrorStatus;
}

/**
 * @brief  : This Functoin Sets a CallBack Function to a Certain Interrupt
 * @fn     : DMA_SetCallBack
 * @param  : InitConfig => Struct that holds all configurations of the DMA -> Check Struct ( @DMA_INIT_STRUCT_t )
 * @param  : CallBackID => Enum that holds All Possible Interrupts that can Occur in a Certain Stream -> Check Options ( @DMA_CALLBACK_ID_t )
 * @param  : Copy_pvCallBack => Pointer to Function that will be Called when the Interrupt Occurs
 * @return : ERRORS_t => Error Status To Indicate if Function Worked Properly
 */
Error_State_t DMA_SetCallBack(DMA_INIT_STRUCT_t *InitConfig, DMA_CALLBACK_ID_t CallBackID, void (*Copy_pvCallBack)(void))
{
	Error_State_t Local_u8ErrorStatus = DMA_OK;

    if (DMA_NOK != DMA_CheckInitConfig(InitConfig))
    {
        if (CallBackID < DMA_TRANSFER_CMP_CALLBACK || CallBackID > DMA_FIFO_ERROR_CALLBACK || Copy_pvCallBack == NULL)
        {
            Local_u8ErrorStatus = DMA_NOK;
        }
        else
        {
            switch (InitConfig->DMAController)
            {
            case DMA1_CONTROLLER:
                DMA1_STREAM_PTR_TOFUNC[InitConfig->StreamNumber][CallBackID] = Copy_pvCallBack;
                break;

            case DMA2_CONTROLLER:
                DMA2_STREAM_PTR_TOFUNC[InitConfig->StreamNumber][CallBackID] = Copy_pvCallBack;
                break;
            }
        }
    }
    else
    {
        Local_u8ErrorStatus = DMA_NOK;
    }
    return Local_u8ErrorStatus;
}

/**
 * @brief : This Function Checks on the Configuration Structure of the DMA to Check if it's Valid or Not
 *
 * @param : DMA_InitConfig =>  Struct that holds all configurations of the DMA -> Check Struct ( @DMA_INIT_STRUCT_t )
 * @note  : This Function is a Private Function , Specified For Driver Use Only
 * @return: ERRORS_t => Error Status To Indicate if Function Worked Properly( DMA_OK ) or Not ( DMA_NOK )
 */
static Error_State_t DMA_CheckInitConfig(DMA_INIT_STRUCT_t *DMA_InitConfig)
{
	Error_State_t Local_u8ErrorStatus = OK;

    if (DMA_InitConfig->ChannelNumber < DMA_CHANNEL0 || DMA_InitConfig->ChannelNumber > DMA_CHANNEL7 ||
        DMA_InitConfig->Direction < DMA_PERIPH_TO_MEM || DMA_InitConfig->Direction > DMA_MEM_TO_MEM ||
        DMA_InitConfig->DMAController < DMA1_CONTROLLER || DMA_InitConfig->DMAController > DMA2_CONTROLLER ||
        DMA_InitConfig->FIFOMode < DMA_FIFOMODE_DISABLE || DMA_InitConfig->FIFOMode > DMA_FIFOMODE_ENABLE ||
        DMA_InitConfig->FIFOThreshold < DMA_FIFO_THRESHOLD_1QUARTER_FULL || DMA_InitConfig->FIFOThreshold > DMA_FIFO_THRESHOLD_FULL ||
        DMA_InitConfig->MemBurst < DMA_MEM_SINGLE_TRANSFER || DMA_InitConfig->MemBurst > DMA_MEM_BURST16_TRANSFER ||
        DMA_InitConfig->PeriphBurst < DMA_PERIPH_SINGLE_TRANSFER || DMA_InitConfig->PeriphBurst > DMA_PERIPH_BURST16_TRANSFER ||
        DMA_InitConfig->MemDataWidth < DMA_MEM_DATA_WIDTH_8BITS || DMA_InitConfig->MemDataWidth > DMA_MEM_DATA_WIDTH_32BITS ||
        DMA_InitConfig->MemInc < DMA_MINC_DISABLE || DMA_InitConfig->MemInc > DMA_MINC_ENABLE ||
        DMA_InitConfig->Mode < DMA_NORMAL || DMA_InitConfig->Mode > DMA_CIRCULAR ||
        DMA_InitConfig->PeriphDataWidth < DMA_PERIPH_DATA_WIDTH_8BITS || DMA_InitConfig->PeriphDataWidth > DMA_PERIPH_DATA_WIDTH_32BITS ||
        DMA_InitConfig->PeriphInc < DMA_PINC_DISABLE || DMA_InitConfig->PeriphInc > DMA_PINC_ENABLE ||
        DMA_InitConfig->Priority < DMA_LOW_PRIORITY || DMA_InitConfig->Priority > DMA_VERY_HIGH_PRIORITY ||
        DMA_InitConfig->StreamNumber < DMA_STREAM0 || DMA_InitConfig->StreamNumber > DMA_STREAM7)
    {
        Local_u8ErrorStatus = DMA_NOK;
    }
    else
    {
        Local_u8ErrorStatus = DMA_OK;
    }
    return Local_u8ErrorStatus;
}

/**
 * @brief  : This Function Handles the Interrupts of the DMA When DMA IRQ Handler is Called
 *
 * @param  : DMANumber => Enum that holds All Possible DMA Controllers -> Check Options ( @DMA_CONTROLLER_t )
 * @param  : StreamNumber => Enum that holds All Possible Streams -> Check Options ( @DMA_STREAMS_t )
 * @return : ERRORS_t => Error Status To Indicate if Function Worked Properly( DMA_OK ) or Not ( DMA_NOK )
 * @note   : This Function is a Private Function , Specified For Driver Use Only
 */
static Error_State_t DMA_IRQHandler(DMA_CONTROLLER_t DMANumber, DMA_STREAMS_t StreamNumber)
{
	Error_State_t Local_u8ErrorStatus = DMA_OK;

    if (DMANumber < DMA1_CONTROLLER || DMANumber > DMA2_CONTROLLER ||
        StreamNumber < DMA_STREAM0 || StreamNumber > DMA_STREAM7)
    {
        Local_u8ErrorStatus = DMA_NOK;
    }
    else
    {
        DMA_Interrupt_Flag_Status_t FlagStatus = DMA_FLAG_RESET;

        DMA_ReadInterruptFlag(DMANumber, StreamNumber, TRANSFER_COMPLETE_IT_FLAG, &FlagStatus);

        if (FlagStatus == DMA_FLAG_SET)
        {
            /* Transfer Complete IT Occured */

            DMA_ClearInterruptFlag(DMANumber, StreamNumber, TRANSFER_COMPLETE_IT_FLAG);

            if (DMA1_STREAM_PTR_TOFUNC[StreamNumber][DMA_TRANSFER_CMP_CALLBACK] != NULL && DMANumber == DMA1_CONTROLLER)
            {
                DMA1_STREAM_PTR_TOFUNC[StreamNumber][DMA_TRANSFER_CMP_CALLBACK]();
            }
            else if (DMA2_STREAM_PTR_TOFUNC[StreamNumber][DMA_TRANSFER_CMP_CALLBACK] != NULL && DMANumber == DMA2_CONTROLLER)
            {
                DMA2_STREAM_PTR_TOFUNC[StreamNumber][DMA_TRANSFER_CMP_CALLBACK]();
            }
        }

        DMA_ReadInterruptFlag(DMANumber, StreamNumber, HALF_TRANSFER_IT_FLAG, &FlagStatus);

        if (FlagStatus == DMA_FLAG_SET)
        {
            /* Half Transfer IT Occured */

            DMA_ClearInterruptFlag(DMANumber, StreamNumber, HALF_TRANSFER_IT_FLAG);

            if (DMA1_STREAM_PTR_TOFUNC[StreamNumber][DMA_HALF_TRANSFER_CALLBACK] != NULL && DMANumber == DMA1_CONTROLLER)
            {
                DMA1_STREAM_PTR_TOFUNC[StreamNumber][DMA_HALF_TRANSFER_CALLBACK]();
            }
            else if (DMA2_STREAM_PTR_TOFUNC[StreamNumber][DMA_HALF_TRANSFER_CALLBACK] != NULL && DMANumber == DMA2_CONTROLLER)
            {
                DMA2_STREAM_PTR_TOFUNC[StreamNumber][DMA_HALF_TRANSFER_CALLBACK]();
            }
        }

        DMA_ReadInterruptFlag(DMANumber, StreamNumber, TRANSFER_ERROR_IT_FLAG, &FlagStatus);

        if (FlagStatus == DMA_FLAG_SET)
        {
            /* Transfer Error IT Occured */

            DMA_ClearInterruptFlag(DMANumber, StreamNumber, TRANSFER_ERROR_IT_FLAG);

            if (DMA1_STREAM_PTR_TOFUNC[StreamNumber][DMA_TRANSFER_ERROR_CALLBACK] != NULL && DMANumber == DMA1_CONTROLLER)
            {
                DMA1_STREAM_PTR_TOFUNC[StreamNumber][DMA_TRANSFER_ERROR_CALLBACK]();
            }
            else if (DMA2_STREAM_PTR_TOFUNC[StreamNumber][DMA_TRANSFER_ERROR_CALLBACK] != NULL && DMANumber == DMA2_CONTROLLER)
            {
                DMA2_STREAM_PTR_TOFUNC[StreamNumber][DMA_TRANSFER_ERROR_CALLBACK]();
            }
        }
        DMA_ReadInterruptFlag(DMANumber, StreamNumber, DIRECT_MODE_ERROR_IT_FLAG, &FlagStatus);

        if (FlagStatus == DMA_FLAG_SET)
        {
            /* Direct Mode Error IT Occured */

            DMA_ClearInterruptFlag(DMANumber, StreamNumber, DIRECT_MODE_ERROR_IT_FLAG);

            if (DMA1_STREAM_PTR_TOFUNC[StreamNumber][DMA_DIRECT_MODE_ERROR_CALLBACK] != NULL && DMANumber == DMA1_CONTROLLER)
            {
                DMA1_STREAM_PTR_TOFUNC[StreamNumber][DMA_DIRECT_MODE_ERROR_CALLBACK]();
            }
            else if (DMA2_STREAM_PTR_TOFUNC[StreamNumber][DMA_DIRECT_MODE_ERROR_CALLBACK] != NULL && DMANumber == DMA2_CONTROLLER)
            {
                DMA2_STREAM_PTR_TOFUNC[StreamNumber][DMA_DIRECT_MODE_ERROR_CALLBACK]();
            }
        }

        DMA_ReadInterruptFlag(DMANumber, StreamNumber, FIFO_ERROR_IT_FLAG, &FlagStatus);

        if (FlagStatus == DMA_FLAG_SET)
        {
            /* FIFO Error IT Occured */

            DMA_ClearInterruptFlag(DMANumber, StreamNumber, FIFO_ERROR_IT_FLAG);

            if (DMA1_STREAM_PTR_TOFUNC[StreamNumber][DMA_FIFO_ERROR_CALLBACK] != NULL && DMANumber == DMA1_CONTROLLER)
            {
                DMA1_STREAM_PTR_TOFUNC[StreamNumber][DMA_FIFO_ERROR_CALLBACK]();
            }
            else if (DMA2_STREAM_PTR_TOFUNC[StreamNumber][DMA_FIFO_ERROR_CALLBACK] != NULL && DMANumber == DMA2_CONTROLLER)
            {
                DMA2_STREAM_PTR_TOFUNC[StreamNumber][DMA_FIFO_ERROR_CALLBACK]();
            }
        }
    }
    return Local_u8ErrorStatus;
}

/*==============================================================================================================================================
 * HANDLERS SECTION
 *==============================================================================================================================================*/

/* ==========================
 *           DMA1
 * ========================== */

void DMA1_Stream0_IRQHandler(void)
{
    DMA_IRQHandler(DMA1_CONTROLLER, DMA_STREAM0);
}
void DMA1_Stream1_IRQHandler(void)
{
    DMA_IRQHandler(DMA1_CONTROLLER, DMA_STREAM1);
}
void DMA1_Stream3_IRQHandler(void)
{
    DMA_IRQHandler(DMA1_CONTROLLER, DMA_STREAM3);
}
void DMA1_Stream4_IRQHandler(void)
{
    DMA_IRQHandler(DMA1_CONTROLLER, DMA_STREAM4);
}
void DMA1_Stream5_IRQHandler(void)
{
    DMA_IRQHandler(DMA1_CONTROLLER, DMA_STREAM5);
}
void DMA1_Stream6_IRQHandler(void)
{
    DMA_IRQHandler(DMA1_CONTROLLER, DMA_STREAM6);
}
void DMA1_Stream7_IRQHandler(void)
{
    DMA_IRQHandler(DMA1_CONTROLLER, DMA_STREAM7);
}

/* ==========================
 *           DMA2
 * ========================== */

void DMA2_Stream0_IRQHandler(void)
{
    DMA_IRQHandler(DMA2_CONTROLLER, DMA_STREAM0);
}
void DMA2_Stream1_IRQHandler(void)
{
    DMA_IRQHandler(DMA2_CONTROLLER, DMA_STREAM1);
}
void DMA2_Stream2_IRQHandler(void)
{
    DMA_IRQHandler(DMA2_CONTROLLER, DMA_STREAM2);
}
void DMA2_Stream3_IRQHandler(void)
{
    DMA_IRQHandler(DMA2_CONTROLLER, DMA_STREAM3);
}
void DMA2_Stream4_IRQHandler(void)
{
    DMA_IRQHandler(DMA2_CONTROLLER, DMA_STREAM4);
}
void DMA2_Stream5_IRQHandler(void)
{
    DMA_IRQHandler(DMA2_CONTROLLER, DMA_STREAM5);
}
void DMA2_Stream6_IRQHandler(void)
{
    DMA_IRQHandler(DMA2_CONTROLLER, DMA_STREAM6);
}
void DMA2_Stream7_IRQHandler(void)
{
    DMA_IRQHandler(DMA2_CONTROLLER, DMA_STREAM7);
}
