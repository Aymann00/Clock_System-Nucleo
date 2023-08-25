/*
 ******************************************************************************
 * @file           : SPI_Program.c
 * @Author         : Mohammed Ayman Shalaby
 * @brief          : Main program body for SPI driver
 * @Date           : Aug 20 , 2023
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

#include "../../Library/ErrTypes.h"
#include "../../Library/STM32F446xx.h"

#include "../Inc/SPI_Interface.h"
#include "../Inc/SPI_Private.h"

/*==============================================================================================================================================
 * INCLUDES SECTION END
 *==============================================================================================================================================*/

/*==============================================================================================================================================
 * GLOBAL VARIABLES SECTION START
 *==============================================================================================================================================*/

/* SPI Peripherals Base Addresses */
static SPI_RegDef_t *SPI[SPI_MAX_NUM] = {SPI1, SPI2 , SPI3 , SPI4};

/* SPI Interrupt Pointers to Functions */
static void (*SPI_PTR_TO_FUNC[SPI_MAX_NUM][SPI_IT_MAX_NUM])(void) = {NULL};

/* SPI Transceive Data To Be Received To Use In ISR */
static uint16_t *SPI_TransceiveDataToBeReceived[SPI_MAX_NUM] = {NULL};

/* SPI Transceive Buffer To Be Received To Use In ISR */
static uint8_t *SPI_TransceiveBufferToBeReceived[SPI_MAX_NUM] = {NULL};

/* SPI Transceive Buffer To Be Sent To Use In ISR */
static uint8_t *SPI_TransceiveBufferToBeSent[SPI_MAX_NUM] = {NULL};

/* SPI Transceive Buffer Size To Use In ISR */
static uint16_t SPI_TransceiveBufferSize[SPI_MAX_NUM] = {0};

/* SPI Transceive Buffer Node Role To Check On In ISR */
/* SPI_MASTER or SPI_SLAVE */
static SPI_ROLE_t SPI_TransceiveBufferNodeRole[SPI_MAX_NUM] = {0};

/* SPI Transceive Buffer Role Direction To Check On In ISR */
/* SPI_ROLE_TRANSMIT or SPI_ROLE_RECEIVE */
static SPI_ROLE_DIR_t SPI_TransceiveBufferRoleDirection[SPI_MAX_NUM] = {0};

/* SPI IRQ Sources To Use In ISR */
/* SPI_NO_IRQ_SRC or SPI_TRANSCEIVE_DATA_IRQ_SRC or SPI_TRANSCEIVE_BUFFER_IRQ_SRC */
static SPI_IRQ_SRC_t IRQ_SRC[SPI_MAX_NUM] = {SPI_NO_IRQ_SRC};

/*==============================================================================================================================================
 * GLOBAL VARIABLES SECTION END
 *==============================================================================================================================================*/

/*==============================================================================================================================================
 * MODULES IMPLEMENTATION
 *==============================================================================================================================================*/
/**
 * @brief  : This Function is Used to Initialize The SPI Peripheral According to The Required Configuration in The Configuration Struct
 *
 * @param  : Config => This is a Pointer to Struct of Type SPI_Config_t That Holds The Required Configuration
 * @return : ERRORS_t   => This Return Parameter is Used to Indicate The Function Execution If Executed Correctly or NOT
 * @note   : This Function is Used to Initialize The SPI Peripheral According to The Required Configuration in The Configuration Struct
 *           Some of The Configuration Parameters are For Master Role Only Like ( BaudRate, SlaveSelectOutputType )
 *           and Some of The Configuration Parameters are For Slave Role Only Like ( SlaveManage )
 *           So You Should Select The Required Configuration Parameters According to The Required Role , Configurations For Different Roles has no Effect
 */
ERRORS_t SPI_Init(SPI_Config_t *Config)
{
    ERRORS_t Local_u8ErrorStatus = SPI_OK;

    if (SPI_INVALID_CONFIG != SPI_CheckConfig(Config))
    {

        /* Set The BaudRate */
        /* For Master Role Only */
        if (Config->NodeRole == SPI_MASTER)
        {
            SPI[Config->SPINumber]->CR1 &= (SPI_BR_MASK);
            SPI[Config->SPINumber]->CR1 |= (Config->BaudRate << SPI_BR);
        }

        /* Set Clock Polarity */
        SPI[Config->SPINumber]->CR1 &= (~(1 << SPI_CPOL));
        SPI[Config->SPINumber]->CR1 |= (Config->ClockPolarity << SPI_CPOL);

        /* Set Clock Phase */
        SPI[Config->SPINumber]->CR1 &= (~(1 << SPI_CPHA));
        SPI[Config->SPINumber]->CR1 |= (Config->ClockPhase << SPI_CPHA);

        /* Set The SPI Mode */
        SPI_SetMode(Config);

        /* Set Direction */
        SPI[Config->SPINumber]->CR1 &= (~(1 << SPI_LSBFIRST));
        SPI[Config->SPINumber]->CR1 |= (Config->Direction << SPI_LSBFIRST);

        /* Set CRC Status */
        SPI[Config->SPINumber]->CR1 &= (~(1 << SPI_CRCEN));
        SPI[Config->SPINumber]->CR1 |= (Config->CRC_Status << SPI_CRCEN);

        /* Set Slave Management */
        /* For Slave Role Only */
        if (Config->NodeRole == SPI_SLAVE)
        {
            SPI[Config->SPINumber]->CR1 &= (~(1 << SPI_SSM));
            SPI[Config->SPINumber]->CR1 |= (Config->SlaveManage << SPI_SSM);
        }

        /* Set Node Role */
        SPI[Config->SPINumber]->CR1 &= (~(1 << SPI_MSTR));
        SPI[Config->SPINumber]->CR1 |= (Config->NodeRole << SPI_MSTR);

        /* Set The Data Width */
        SPI[Config->SPINumber]->CR1 &= (~(1 << SPI_DFF));
        SPI[Config->SPINumber]->CR1 |= (Config->DataWidth << SPI_DFF);

        /* Set Slave Select Output Type */
        /* For Master Role Only */
        if (Config->NodeRole == SPI_MASTER)
        {
            SPI[Config->SPINumber]->CR2 &= (~(1 << SPI_SSOE));
            SPI[Config->SPINumber]->CR2 |= (Config->SlaveSelectOutputType << SPI_SSOE);
        }

        /* Configure SPI Interrupts */
        SPI_ENABLE_IT(Config);

        /* Enable SPI Peripheral */
        SPI[Config->SPINumber]->CR1 |= (1 << SPI_SPE);
    }
    else
    {
        Local_u8ErrorStatus = SPI_NOK;
    }
    return Local_u8ErrorStatus;
}

/**
 * @brief  : This Function is Used to Configure SPI Interrupts According to The Required Configuration in The Configuration Struct
 *
 * @param  : Config => This is a Pointer to Struct of Type SPI_Config_t That Holds The Required Configuration
 * @return : ERRORS_t => This Return Parameter is Used to Indicate The Function Execution If Executed Correctly or NOT
 * @note   : This Function is Used Inside The SPI_Init Function , You Can Use it Independently if You Want to Change The Interrupts Configuration
 */
ERRORS_t SPI_ENABLE_IT(SPI_Config_t *Config)
{
    ERRORS_t Local_u8ErrorStatus = SPI_OK;

    if (SPI_INVALID_CONFIG != SPI_CheckConfig(Config))
    {
        /* Set Tx Buffer Empty Interrupt */
        SPI[Config->SPINumber]->CR2 &= (~(1 << SPI_TXEIE));
        SPI[Config->SPINumber]->CR2 |= (Config->InterruptEnable.TXE << SPI_TXEIE);

        /* Set Rx Buffer Not Empty Interrupt */
        SPI[Config->SPINumber]->CR2 &= (~(1 << SPI_RXNEIE));
        SPI[Config->SPINumber]->CR2 |= (Config->InterruptEnable.RXNE << SPI_RXNEIE);

        /* Set Error Interrupt */
        SPI[Config->SPINumber]->CR2 &= (~(1 << SPI_ERRIE));
        SPI[Config->SPINumber]->CR2 |= (Config->InterruptEnable.ERR << SPI_ERRIE);
    }
    else
    {
        Local_u8ErrorStatus = SPI_NOK;
    }
    return Local_u8ErrorStatus;
}

/**
 * @brief  : This Function is Used to Read SPI Flag Status ( Set or Reset )
 *
 * @param  : SPINum => This Parameter is Used to Select The SPI Peripheral Number to Be Used -> @SPI_t
 * @param  : Flag  =>  This Parameter is Used to Select The Flag to Be Read -> @SPI_FLAG_t
 * @param  : FlagState => This Parameter is Used to Return The Flag State ( Set or Reset ) -> @SPI_FLAG_STATUS_t
 * @return : ERRORS_t => This Return Parameter is Used to Indicate The Function Execution If Executed Correctly or NOT
 */
ERRORS_t SPI_ReadFlag(SPI_t SPINum, SPI_FLAG_t Flag, SPI_FLAG_STATUS_t *FlagState)
{
    ERRORS_t Local_u8ErrorStatus = SPI_OK;

    if (SPINum < SPI1_APB2 || SPINum > SPI4_APB2 ||
        Flag < SPI_RECEIVE_BUFFER_NE_FLAG || Flag > SPI_FRAME_ERROR_FLAG)
    {
        Local_u8ErrorStatus = SPI_NOK;
    }
    else
    {
        /* Correct Parameters */
        if (NULL != FlagState)
        {
            /* Return The Flag State */
            *FlagState = ((SPI[SPINum]->SR >> Flag) & 0x01);
        }
        else
        {
            Local_u8ErrorStatus = NULL_POINTER;
        }
    }
    return Local_u8ErrorStatus;
}

/**
 * @brief  : This Function is Activate or Deactivate a Slave Using the Software Slave Management
 *
 * @param  : Config => This is a Pointer to Struct of Type SPI_Config_t That Holds The Required Configuration
 * @param  : SlaveManagementState => This Parameter is Used to Select The Slave Management State -> @SPI_SW_SLAVE_STATUS_t
 * @return : ERRORS_t => This Return Parameter is Used to Indicate The Function Execution If Executed Correctly or NOT
 * @note   : To Use This Function You Should Select The Slave Management in The Configuration Struct as ( SPI_SW_SLAVE_MANAGE ) to Activate or Deactivate The Slave By SW
 *            It's Used Only in The Slave Role & while only one Slave is Connected to The Master
 */
ERRORS_t SPI_SwSlaveManage(SPI_Config_t *Config, SPI_SW_SLAVE_STATUS_t SlaveManagementState)
{
    ERRORS_t Local_u8ErrorStatus = SPI_OK;

    if (Config->SPINumber < SPI1_APB2 || Config->SPINumber > SPI4_APB2 ||
        SlaveManagementState < SPI_SLAVE_SELECTED || SlaveManagementState > SPI_SLAVE_NOT_SELECTED)
    {
        Local_u8ErrorStatus = SPI_NOK;
    }
    else
    {
        if (Config->NodeRole == SPI_SLAVE)
        {
            /* Correct Parameters */
            if (SPI_SLAVE_SELECTED == SlaveManagementState)
            {
                /* Slave Selected */
                SPI[Config->SPINumber]->CR1 &= (~(1 << SPI_SSI));
            }
            else
            {
                /* Slave Not Selected */
                SPI[Config->SPINumber]->CR1 |= (1 << SPI_SSI);
            }
        }
        else
        {
            Local_u8ErrorStatus = SPI_NOK;
        }
    }
    return Local_u8ErrorStatus;
}

/**
 * @brief  : This Function is Used to Change The SPI Node Role  in Half Duplex Mode ( Transmit or Receive )
 *
 * @param  : SPINum => This Parameter is Used to Select The SPI Peripheral Number to Be Used -> @SPI_t
 * @param  : Role => This Parameter is Used to Select The Role to Be Used -> @SPI_HALFDUPLEX_ROLE_t
 * @return : ERRORS_t => This Return Parameter is Used to Indicate The Function Execution If Executed Correctly or NOT
 * @note   : This Function is Used with Half Duplex Mode Only
 */
ERRORS_t SPI_HalfDuplexRoleChange(SPI_t SPINum, SPI_HALFDUPLEX_ROLE_t Role)
{
    ERRORS_t Local_u8ErrorStatus = SPI_OK;

    if (SPINum < SPI1_APB2 || SPINum > SPI4_APB2 ||
        Role < SPI_HALFDUPLEX_RECEIVE || Role > SPI_HALFDUPLEX_TRASNMIT)
    {
        Local_u8ErrorStatus = SPI_NOK;
    }
    else
    {
        /* Correct Parameters */
        SPI[SPINum]->CR1 &= (~(1 << SPI_BIDIOE));
        SPI[SPINum]->CR1 |= (Role << SPI_BIDIOE);
    }
    return Local_u8ErrorStatus;
}

/**
 * @brief  : This Function is Used to Enable DMA  Request Line Between SPI and DMA ( TX or RX )
 * @param  : SPINum => This Parameter is Used to Select The SPI Peripheral Number to Be Used -> @SPI_t
 * @param  : DMA_Line => This Parameter is Used to Select The DMA Line to Be Used -> @SPI_DMA_LINE_t
 * @return : ERRORS_t => This Return Parameter is Used to Indicate The Function Execution If Executed Correctly or NOT
 * @note   : This Function is Used to Handle SPI Data Transmission Using DMA , You Should Call This Function Before Calling The SPI_Init Function
 */
ERRORS_t SPI_EnableDMALine(SPI_t SPINum, SPI_DMA_LINE_t DMA_Line)
{
    ERRORS_t Local_u8ErrorStatus = SPI_OK;

    if (SPINum < SPI1_APB2 || SPINum > SPI4_APB2)
    {
        switch (DMA_Line)
        {
        case SPI_DMA_RX_LINE:
            SPI[SPINum]->CR2 |= (1 << SPI_RXDMAEN);
            break;

        case SPI_DMA_TX_LINE:
            SPI[SPINum]->CR2 |= (1 << SPI_TXDMAEN);
            break;

        default:
            Local_u8ErrorStatus = SPI_NOK;
            break;
        }
    }
    else
    {
        Local_u8ErrorStatus = SPI_NOK;
    }
    return Local_u8ErrorStatus;
}

/**
 * @brief  : This Function is Used to Disable DMA Request Line Between SPI and DMA ( TX or RX )
 * @param  : SPINum => This Parameter is Used to Select The SPI Peripheral Number to Be Used -> @SPI_t
 * @param  : DMA_Line => This Parameter is Used to Select The DMA Line to Be Used -> @SPI_DMA_LINE_t
 * @return : ERRORS_t => This Return Parameter is Used to Indicate The Function Execution If Executed Correctly or NOT
 * @note   : This Function is Used to Handle SPI Data Transmission Using DMA , You Should Call This Function Before Calling The SPI_Init Function
 */
ERRORS_t SPI_DisableDMALine(SPI_t SPINum, SPI_DMA_LINE_t DMA_Line)
{
    ERRORS_t Local_u8ErrorStatus = SPI_OK;

    if (SPINum < SPI1_APB2 || SPINum > SPI4_APB2)
    {
        switch (DMA_Line)
        {
        case SPI_DMA_RX_LINE:
            SPI[SPINum]->CR2 &= (~(1 << SPI_RXDMAEN));
            break;

        case SPI_DMA_TX_LINE:
            SPI[SPINum]->CR2 &= (~(1 << SPI_TXDMAEN));
            break;

        default:
            Local_u8ErrorStatus = SPI_NOK;
            break;
        }
    }
    else
    {
        Local_u8ErrorStatus = SPI_NOK;
    }
    return Local_u8ErrorStatus;
}

/**
 * @brief  : This Function is Used to Wait Until a Flag is Set
 * @param  : SPINum => This Parameter is Used to Select The SPI Peripheral Number to Be Used -> @SPI_t
 * @param  : Flag => This Parameter is Used to Select The Flag to Be Read -> @SPI_FLAG_t
 * @return : ERRORS_t => This Return Parameter is Used to Indicate The Function Execution If Executed Correctly or NOT
 */
ERRORS_t SPI_WaitUntilFlagSet(SPI_t SPINum, SPI_FLAG_t Flag)
{
    ERRORS_t Local_u8ErrorStatus = SPI_OK;

    SPI_FLAG_STATUS_t Local_u8FlagState = SPI_FLAG_RESET;

    if (SPINum < SPI1_APB2 || SPINum > SPI4_APB2 ||
        Flag < SPI_RECEIVE_BUFFER_NE_FLAG || Flag > SPI_BUSY_FLAG)
    {
        Local_u8ErrorStatus = SPI_NOK;
    }
    else
    {
        /* Correct Parameters */
        while (SPI_FLAG_SET != Local_u8FlagState)
        {
            SPI_ReadFlag(SPINum, Flag, &Local_u8FlagState);
        }
    }
    return Local_u8ErrorStatus;
}

/**
 * @brief  : This Function is Used to Send Data & Receive Data at The Same Time
 * @param  : Config => This is a Pointer to Struct of Type SPI_Config_t That Holds The Required Configuration
 * @param  : ReceivedData => This is a Pointer to uint16_t Variable That Holds The Received Data
 * @param  : DataToSend => This is a Pointer of Type uint16_t That Holds The Data to Be Sent
 * @return : ERRORS_t => This Return Parameter is Used to Indicate The Function Execution If Executed Correctly or NOT
 * @note   : IF You Want to Send Data Only You Can Pass NULL to The ReceivedData Parameter ,
 *         IF You Want to Receive Data Only You Can NULL to The DataToSend Parameter
 */

ERRORS_t SPI_TransceiveData(SPI_Config_t *Config, uint16_t *ReceivedData, uint16_t *DataToSend)
{
    ERRORS_t Local_u8ErrorStatus = SPI_OK;

    if (SPI_INVALID_CONFIG != SPI_CheckConfig(Config))
    {
        /* Sending Data is Required */
        if (DataToSend != NULL)
        {
            /* Check on The Node Role ,  Both Master & Slave Can Send Data */
            if (Config->NodeRole == SPI_MASTER || Config->NodeRole == SPI_SLAVE)
            {
                /* Wait Until The Transmit Buffer is Empty */
                SPI_WaitUntilFlagSet(Config->SPINumber, SPI_TRANSMIT_BUFFER_E_FLAG);

                /* Both Master & Slave Can Send Data */
                SPI[Config->SPINumber]->DR = *DataToSend;
            }
        }

        /* Receiving Data is Required */
        if (ReceivedData != NULL)
        {
            if (Config->NodeRole == SPI_MASTER)
            {
                /* Wait Until The Transmit Buffer is Empty */
                SPI_WaitUntilFlagSet(Config->SPINumber, SPI_TRANSMIT_BUFFER_E_FLAG);

                /* Send Any Value to Initiate Communication */
                SPI[Config->SPINumber]->DR = 0xFF;

                /* Wait Until The Receive Buffer is Not Empty */
                SPI_WaitUntilFlagSet(Config->SPINumber, SPI_RECEIVE_BUFFER_NE_FLAG);

                /* Read Received Data */
                *ReceivedData = SPI[Config->SPINumber]->DR;
            }
            else if (Config->NodeRole == SPI_SLAVE)
            {
                /* Wait Until The Receive Buffer is Not Empty */
                SPI_WaitUntilFlagSet(Config->SPINumber, SPI_RECEIVE_BUFFER_NE_FLAG);

                /* Read Received Data */
                *ReceivedData = SPI[Config->SPINumber]->DR;
            }
        }
    }
    else
    {
        Local_u8ErrorStatus = SPI_NOK;
    }
    return Local_u8ErrorStatus;
}

/**
 * @brief  : This Function is Used to Send & Receive a Buffer of Data at The Same Time
 * @param  : Config => This is a Pointer to Struct of Type SPI_Config_t That Holds The Required Configuration
 * @param  : ReceviedBuffer => This is a Pointer to uint8_t Array That Holds The Received Data
 * @param  : BufferToSend => This is a Pointer to uint8_t Array That Holds The Data to Be Sent
 * @param  : BufferSize => This is a Variable of Type uint16_t That Holds The Size of The Buffer to Be Sent & Received
 * @return : ERRORS_t => This Return Parameter is Used to Indicate The Function Execution If Executed Correctly or NOT
 * @note   : IF You Want to Send Data Only You Can Pass NULL to The ReceivedBuffer Parameter ,
 *      IF You Want to Receive Data Only You Can Pass NULL to The BufferToSend Parameter
 */
ERRORS_t SPI_TransceiveBuffer(SPI_Config_t *Config, uint8_t *ReceviedBuffer, uint8_t *BufferToSend, uint16_t BufferSize)
{
    ERRORS_t Local_u8ErrorStatus = SPI_OK;

    uint16_t Local_u16Counter = 0;

    if (Config->SPINumber < SPI1_APB2 || Config->SPINumber > SPI4_APB2)
    {
        Local_u8ErrorStatus = SPI_NOK;
    }
    else
    {
        /* Correct Parameters */

        for (Local_u16Counter = 0; Local_u16Counter < BufferSize; Local_u16Counter++)
        {
            SPI_TransceiveData(Config, (uint16_t *)&ReceviedBuffer[Local_u16Counter], (uint16_t *)&BufferToSend[Local_u16Counter]);
        }
    }
    return Local_u8ErrorStatus;
}

/**
 * @brief  : This Function is Used to Send & Receive Data Using Interrupts ( Non Blocking Function ) AKA Asynchronous
 *
 * @param  : Config => This is a Pointer to Struct of Type SPI_Config_t That Holds The Required Configuration
 * @param  : ReceivedData => This is a Pointer to uint16_t Variable That Holds The Received Data
 * @param  : DataToSend => This is a Pointer of Type uint16_t That Holds The Data to Be Sent
 * @param  : pv_CALL_BACK_FUNC => This is a Pointer to Function That Holds The Call Back Function Address
 * @return : ERRORS_t => This Return Parameter is Used to Indicate The Function Execution If Executed Correctly or NOT
 * @note   : You Can Use This Function Directly without Enabling RXNE Interrupt in The Configuration Struct , as This Function Enables it Automatically
 *          IF You Want to Send Data Only You Can Pass NULL to The ReceivedData Parameter , IF You Want to Receive Data Only You Can NULL to The DataToSend Parameter
 */
ERRORS_t SPI_TransceiveData_IT(SPI_Config_t *Config, uint16_t *ReceivedData, uint16_t *DataToSend, void(*pv_CALL_BACK_FUNC))
{
    ERRORS_t Local_u8ErrorStatus = SPI_OK;

    if (Config->SPINumber < SPI1_APB2 || Config->SPINumber > SPI4_APB2)
    {
        Local_u8ErrorStatus = SPI_NOK;
    }
    else
    {
        if (pv_CALL_BACK_FUNC != NULL)
        {
            /* Correct Parameters */

            /* Setting IRQ Source */
            IRQ_SRC[Config->SPINumber] = SPI_TRANSCEIVE_DATA_IRQ_SRC;

            /* Set The Call Back Function */
            SPI_PTR_TO_FUNC[Config->SPINumber][SPI_IT_RXNE] = pv_CALL_BACK_FUNC;

            /* Receive Data is Required */
            if (ReceivedData != NULL)
            {
                /* Set Received Data Globally */
                SPI_TransceiveDataToBeReceived[Config->SPINumber] = ReceivedData;

                /* Check on The Node Role */
                /* Master Can't Receive Data , So Send Any Value to Initiate Communication */
                if (Config->NodeRole == SPI_MASTER)
                {
                    /* Wait Until The Transmit Buffer is Empty */
                    SPI_WaitUntilFlagSet(Config->SPINumber, SPI_TRANSMIT_BUFFER_E_FLAG);

                    /* Send Any Value to Initiate Communication */
                    SPI[Config->SPINumber]->DR = 0xFF;

                    /* Enable Rx Buffer Not Empty Interrupt */
                    SPI[Config->SPINumber]->CR2 |= (1 << SPI_RXNEIE);
                }
                else if (Config->NodeRole == SPI_SLAVE)
                {
                    /* Enable Rx Buffer Not Empty Interrupt */
                    SPI[Config->SPINumber]->CR2 |= (1 << SPI_RXNEIE);
                }
                /* Both Master & Slave Can Receive Data By Enabling Rx Buffer Not Empty Interrupt */
            }
            /* Sending Data is Required */
            if (DataToSend != NULL)
            {
                /* Both Master & Slave Can Send Data */
                /* Send Data */
                SPI[Config->SPINumber]->DR = *DataToSend;

                /* Enable Rx Buffer Not Empty Interrupt */
                SPI[Config->SPINumber]->CR2 |= (1 << SPI_RXNEIE);
            }
        }
        else
        {
            Local_u8ErrorStatus = NULL_POINTER;
        }
    }
    return Local_u8ErrorStatus;
}

/**
 * @brief  : This Function is Used to Send & Receive a Buffer of Data Using Interrupts ( Non Blocking Function ) AKA Asynchronous
 *
 * @param  : Config => This is a Pointer to Struct of Type SPI_Config_t That Holds The Required Configuration
 * @param  : ReceivedBuffer => This is a Pointer to uint8_t Array That Holds The Received Data
 * @param  : BufferToSend => This is a Pointer to uint8_t Array That Holds The Data to Be Sent
 * @param  : BufferSize => This is a Variable of Type uint16_t That Holds The Size of The Buffer to Be Sent & Received
 * @param  : pv_CallBackFunc => This is a Pointer to Function That Holds The Call Back Function Address
 * @return : ERRORS_t => This Return Parameter is Used to Indicate The Function Execution If Executed Correctly or NOT
 * @note   : You Can Use This Function Directly without Enabling RXNE Interrupt in The Configuration Struct , as This Function Enables it Automatically
 *         if You Want to Send Data Only You Can Pass NULL to The ReceivedBuffer Parameter , IF You Want to Receive Data Only You Can NULL to The BufferToSend Parameter
 */
ERRORS_t SPI_TransceiveBuffer_IT(SPI_Config_t *Config, uint8_t *ReceivedBuffer, uint8_t *BufferToSend, uint16_t BufferSize, void (*pv_CallBackFunc)(void))
{
    ERRORS_t Local_u8ErrorStatus = SPI_OK;

    if (Config->SPINumber < SPI1_APB2 || Config->SPINumber > SPI4_APB2)
    {
        Local_u8ErrorStatus = SPI_NOK;
    }
    else
    {
        /* Correct Parameters */
        /* Set IRQ Source */
        IRQ_SRC[Config->SPINumber] = SPI_TRANSCEIVE_BUFFER_IRQ_SRC;

        /* Set Call Back Function */
        SPI_PTR_TO_FUNC[Config->SPINumber][SPI_IT_RXNE] = pv_CallBackFunc;

        /* Set Buffer Size Globally */
        SPI_TransceiveBufferSize[Config->SPINumber] = BufferSize;

        /* Receive Buffer is Required */
        if (NULL != ReceivedBuffer)
        {
            /* Set Direction Globally */
            SPI_TransceiveBufferRoleDirection[Config->SPINumber] = SPI_ROLE_RECEIVE;

            /* Set ReceivedBuffer Globally */
            SPI_TransceiveBufferToBeReceived[Config->SPINumber] = ReceivedBuffer;

            /* Check on The Node Role */
            if (Config->NodeRole == SPI_SLAVE)
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
