/*
 ******************************************************************************
 * @file           : SPI_Private.h
 * @Author         : Mohammed Ayman Shalaby
 * @brief          : Private File For SPI Driver
 * @Date           : Aug 20, 2023
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 Ayman.
 * All rights reserved.
 *
 ******************************************************************************
 */

#ifndef USART_PRIVATE_H_
#define USART_PRIVATE_H_

/* ======================================================================
 * PRIVATE FUNCTION
 * ====================================================================== */

/**
 * @brief  : This Function is Used to Check on SPI Configuration Structure Before Passing it to The SPI_Init Function
 *
 * @param  : Configuration => This is a Pointer to Struct of Type SPI_Config_t That Holds The Required Configuration
 * @return : ERRORS_t     => This Return Parameter is Used to Indicate The Function Execution If Executed Correctly or NOT
 */
static ERRORS_t SPI_CheckConfig(SPI_Config_t *Configuration);

/**
 * @brief  : This Function is Used to Set The SPI Mode According to The Required Configuration in The Configuration Struct
 *
 * @param  : Config => This is a Pointer to Struct of Type SPI_Config_t That Holds The Required Configuration
 * @return : ERRORS_t   => This Return Parameter is Used to Indicate The Function Execution If Executed Correctly or NOT
 * @note   : Private Function Used Inside The SPI_Init Function
 */
static ERRORS_t SPI_SetMode(SPI_Config_t *Config);

/**
 * @brief  : This Function is Used to Handle The SPI Interrupts
 *
 * @param  : SPINumber => This Parameter is Used to Select The SPI Peripheral Number to Be Used -> @SPI_t
 * @return : ERRORS_t => This Return Parameter is Used to Indicate The Function Execution If Executed Correctly or NOT
 * @note   : This Function is Used Inside The SPI Interrupt Handlers (Private Function)
 */
static ERRORS_t SPI_HANDLE_IT(SPI_t SPINumber);

/* ======================================================================
 * PRIVATE MACROS
 * ====================================================================== */

#define SPI_BR_MASK 0xFFC7

#define SPI_BUFFER_SIZE_NOT_REACHED 0x00
#define SPI_BUFFER_SIZE_REACHED 0x01

/* ======================================================================
 * PRIVATE ENUMS
 * ====================================================================== */

/**
 * @brief  : This Enum is Used to Specify The SPI Interrupts
 */
typedef enum
{
    SPI_IT_TXE = 0x00,
    SPI_IT_RXNE = 0x01,
    SPI_IT_ERR = 0x02,
    SPI_IT_MAX_NUM = 0x03

} SPI_IT_SRC_t;

/**
 * @brief  : This Enum is Used to Specify The SPI Interrupt Sources
 */
typedef enum
{
    SPI_NO_IRQ_SRC = 0x00,
    SPI_TRANSCEIVE_DATA_IRQ_SRC = 0x01,
    SPI_TRANSCEIVE_BUFFER_IRQ_SRC = 0x02,

} SPI_IRQ_SRC_t;

typedef enum
{
    SPI_ROLE_TRANSMIT = 0x00,
    SPI_ROLE_RECEIVE = 0x01,
} SPI_ROLE_DIR_t;



#endif /* USART_PRIVATE_H_ */