/*
 ******************************************************************************
 * @file           : SPI_Interface.h
 * @Author         : Mohammed Ayman Shalaby
 * @brief          : SPI Interface Header file
 * @Date           : Aug 20, 2023
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 Ayman.
 * All rights reserved.
 *
 ******************************************************************************
 */

#ifndef SPI_INTERFACE_H_
#define SPI_INTERFACE_H_

/* ======================================================================
 * SPI Option Enums
 * ====================================================================== */

/** SPI_t
 *  ========================================================================
 *  @brief : This Enum is used to select the SPI Peripheral
 *  @enum  : @SPI_t
 *  ========================================================================
 */
typedef enum
{
    SPI1_APB2 = 0,
    SPI2_APB1 = 1,
    SPI3_APB1 = 2,
    SPI4_APB2 = 3,
    SPI_MAX_NUM = 4,

} SPI_t;

/** SPI_Mode_t
 *  ========================================================================
 *  @brief : This Enum is used to select the SPI Mode
 *  @enum  : @SPI_Mode_t
 *  ========================================================================
 */
typedef enum
{
    SPI_FULL_DUPLEX = 0,
    SPI_HALF_DUPLEX_TX,
    SPI_HALF_DUPLEX_RX,
    SPI_SIMPLEX_TX,
    SPI_SIMPLEX_RX,

} SPI_Mode_t;

/** SPI_SLAVE_MANAGEMENT_t
 * ========================================================================
 *  @brief : This Enum is used to select the SPI Slave  Management
 *  @enum  : @SPI_SLAVE_MANAGEMENT_t
 * ========================================================================
 */
typedef enum
{
    SPI_HW_SLAVE_MANAGE = 0,
    SPI_SW_SLAVE_MANAGE,

} SPI_SLAVE_MANAGEMENT_t;

/** SPI_DIRECTION_t
 * ========================================================================
 *  @brief : This Enum is used to select the SPI Data Direction
 *  @enum  : @SPI_DIRECTION_t
 * ========================================================================
 */
typedef enum
{
    SPI_MSB_FIRST = 0,
    SPI_LSB_FIRST,

} SPI_DIRECTION_t;

/** SPI_SW_SLAVE_STATUS_t
 * ========================================================================
 *  @brief : This Enum is used to select the SPI SW Slave Select Status
 *  @enum  : @SPI_SW_SLAVE_STATUS_t
 * ========================================================================
 */
typedef enum
{
    SPI_SLAVE_SELECTED = 0,
    SPI_SLAVE_NOT_SELECTED,

} SPI_SW_SLAVE_STATUS_t;

/** SPI_ROLE_t
 * ========================================================================
 *  @brief : This Enum is used to select the SPI Role
 *  @enum  : @SPI_ROLE_t
 * ========================================================================
 */
typedef enum
{
    SPI_SLAVE = 0,
    SPI_MASTER

} SPI_ROLE_t;

/** SPI_CRC_STATUS_t
 * ========================================================================
 *  @brief : This Enum is used to select the SPI CRC Status
 *  @enum  : @SPI_CRC_STATUS_t
 * ========================================================================
 */
typedef enum
{
    SPI_CRC_DIS = 0,
    SPI_CRC_EN,

} SPI_CRC_STATUS_t;

/** SPI_DATA_SIZE_t
 * ========================================================================
 *  @brief : This Enum is used to select the SPI Data Size
 *  @enum  : @SPI_DATA_SIZE_t
 * ========================================================================
 */
typedef enum
{
    SPI_1BYTE = 0,
    SPI_2BYTE,

} SPI_DATA_SIZE_t;

/** SPI_BAUDRATE_t
 * ========================================================================
 *  @brief : This Enum is used to select the SPI Baudrate
 *  @enum  : @SPI_BAUDRATE_t
 * ========================================================================
 */
typedef enum
{
    SPI_PERIPH_CLK_BY2 = 0,   /* By default 8MHZ */
    SPI_PERIPH_CLK_BY4 = 1,   /* By default 4MHZ */
    SPI_PERIPH_CLK_BY8 = 2,   /* By default 2MHZ */
    SPI_PERIPH_CLK_BY16 = 3,  /* By default 1MHZ */
    SPI_PERIPH_CLK_BY32 = 4,  /* By default 500KHZ */
    SPI_PERIPH_CLK_BY64 = 5,  /* By default 250KHZ */
    SPI_PERIPH_CLK_BY128 = 6, /* By default 125KHZ */
    SPI_PERIPH_CLK_BY256 = 7, /* By default 62.5KHZ */

} SPI_BAUDRATE_t;

/** SPI_CPOL_t
 * ========================================================================
 *  @brief : This Enum is used to select the SPI Clock Polarity
 *  @enum  : @SPI_CPOL_t
 * ========================================================================
 */
typedef enum
{
    SPI_CLK_IDLE_LOW = 0, /* By default */
    SPI_CLK_IDLE_HIGH = 1,

} SPI_CPOL_t;

/** SPI_CPHA_t
 * ========================================================================
 *  @brief : This Enum is used to select the SPI Clock Phase
 *  @enum  : @SPI_CPHA_t
 * ========================================================================
 */
typedef enum
{
    SPI_CAPTURE_LEADING = 0,  /* Sample data at first edge & Setup data at second edge */
    SPI_CAPTURE_TRAILING = 1, /* Setup data at first edge & Sample data at second edge */

} SPI_CPHA_t;

/** SPI_IT_STATUS_t
 * ========================================================================
 *  @brief : This Enum is used to select the SPI Interrupt Status
 *  @enum  : @SPI_IT_STATUS_t
 * ========================================================================
 */
typedef enum
{
    SPI_IT_DIS = 0,
    SPI_IT_EN,
} SPI_IT_STATUS_t;

/** SPI_SS_OTYPE_t
 * ========================================================================
 *  @brief : This Enum is used to select the SPI Slave Select Output Type
 *  @enum  : @SPI_SS_OTYPE_t
 * ========================================================================
 */
typedef enum
{
    SPI_SS_OUT_DIS = 0,
    SPI_SS_OUT_EN,

} SPI_SS_OTYPE_t;

/** SPI_FLAG_t
 * ========================================================================
 *  @brief : This Enum is used to select the SPI Flag
 *  @enum  : @SPI_FLAG_t
 * ========================================================================
 */
typedef enum
{
    SPI_RECEIVE_BUFFER_NE_FLAG = 0,
    SPI_TRANSMIT_BUFFER_E_FLAG = 1,
    SPI_UNDERRUN_FLAG = 3,
    SPI_CRC_ERROR_FLAG = 4,
    SPI_MODE_FAULT_FLAG = 5,
    SPI_OVERRUN_FLAG = 6,
    SPI_BUSY_FLAG = 7,
    SPI_FRAME_ERROR_FLAG = 8,

} SPI_FLAG_t;

/** SPI_FLAG_STATUS_t
 * ========================================================================
 *  @brief : This Enum is used to select the SPI Flag Status
 *  @enum  : @SPI_FLAG_STATUS_t
 * ========================================================================
 */

typedef enum
{
    SPI_FLAG_RESET = 0,
    SPI_FLAG_SET = 1,

} SPI_FLAG_STATUS_t;

/** SPI_DMA_LINE_t
 * ========================================================================
 *  @brief : This Enum is used to select the SPI DMA Line
 *  @enum  : @SPI_DMA_LINE_t
 * ========================================================================
 */
typedef enum
{
    SPI_DMA_TX_LINE = 0,
    SPI_DMA_RX_LINE,
} SPI_DMA_LINE_t;
/* ======================================================================
 * SPI Configuration Structs
 * ====================================================================== */

typedef enum
{
    SPI_HALFDUPLEX_RECEIVE = 0,
    SPI_HALFDUPLEX_TRASNMIT = 1,

} SPI_HALFDUPLEX_ROLE_t;

/** SPI_IT_ENABLE_t
 * ========================================================================
 *  @brief  : This struct is used to select the SPI Interrupt Enable
 *  @struct : @SPI_IT_ENABLE_t
 * ========================================================================
 */
typedef struct
{
    SPI_IT_STATUS_t RXNE;
    SPI_IT_STATUS_t TXE;
    SPI_IT_STATUS_t ERR;

} SPI_IT_ENABLE_t;

/** SPI_Config_t
 * ========================================================================
 *  @brief  : This struct is used to select the SPI Configuration
 *  @struct : @SPI_Config_t
 * ========================================================================
 */
typedef struct
{
    SPI_t SPINumber;
    SPI_ROLE_t NodeRole;
    SPI_Mode_t Mode;
    SPI_DATA_SIZE_t DataWidth;
    SPI_DIRECTION_t Direction;
    SPI_SLAVE_MANAGEMENT_t SlaveManage;
    SPI_BAUDRATE_t BaudRate;
    SPI_CPOL_t ClockPolarity;
    SPI_CPHA_t ClockPhase;
    SPI_SS_OTYPE_t SlaveSelectOutputType;
    SPI_CRC_STATUS_t CRC_Status;
    SPI_IT_ENABLE_t InterruptEnable;

} SPI_Config_t;

/* ======================================================================
 *                         FUNCTIONS PROTOTYPES
 * ====================================================================== */


/**
 * @brief  : This Function is Used to Initialize The SPI Peripheral According to The Required Configuration in The Configuration Struct

 * @param  : Config => This is a Pointer to Struct of Type SPI_Config_t That Holds The Required Configuration
 * @return : ERRORS_t   => This Return Parameter is Used to Indicate The Function Execution If Executed Correctly or NOT
 * @note   : This Function is Used to Initialize The SPI Peripheral According to The Required Configuration in The Configuration Struct
 *           Some of The Configuration Parameters are For Master Role Only Like ( BaudRate, SlaveSelectOutputType )
 *           and Some of The Configuration Parameters are For Slave Role Only Like ( SlaveManage )
 *           So You Should Select The Required Configuration Parameters According to The Required Role , Configurations For Different Roles has no Effect
 */
ERRORS_t SPI_Init(SPI_Config_t *Config);

/**
 * @brief  : This Function is Used to Configure SPI Interrupts According to The Required Configuration in The Configuration Struct
 *
 * @param  : Config => This is a Pointer to Struct of Type SPI_Config_t That Holds The Required Configuration
 * @return : ERRORS_t => This Return Parameter is Used to Indicate The Function Execution If Executed Correctly or NOT
 * @note   : This Function is Used Inside The SPI_Init Function , You Can Use it Independently if You Want to Change The Interrupts Configuration
 */
ERRORS_t SPI_ENABLE_IT(SPI_Config_t *Config);

/**
 * @brief  : This Function is Used to Read SPI Flag Status ( Set or Reset )
 *
 * @param  : SPINum => This Parameter is Used to Select The SPI Peripheral Number to Be Used -> @SPI_t
 * @param  : Flag  =>  This Parameter is Used to Select The Flag to Be Read -> @SPI_FLAG_t
 * @param  : FlagState => This Parameter is Used to Return The Flag State ( Set or Reset ) -> @SPI_FLAG_STATUS_t
 * @return : ERRORS_t => This Return Parameter is Used to Indicate The Function Execution If Executed Correctly or NOT
 */
ERRORS_t SPI_ReadFlag(SPI_t SPINum, SPI_FLAG_t Flag, SPI_FLAG_STATUS_t *FlagState);

/**
 * @brief  : This Function is Activate or Deactivate a Slave Using the Software Slave Management
 *
 * @param  : Config => This is a Pointer to Struct of Type SPI_Config_t That Holds The Required Configuration
 * @param  : SlaveManagementState => This Parameter is Used to Select The Slave Management State -> @SPI_SW_SLAVE_STATUS_t
 * @return : ERRORS_t => This Return Parameter is Used to Indicate The Function Execution If Executed Correctly or NOT
 * @note   : To Use This Function You Should Select The Slave Management in The Configuration Struct as ( SPI_SW_SLAVE_MANAGE ) to Activate or Deactivate The Slave By SW
 *            It's Used Only in The Slave Role & while only one Slave is Connected to The Master
 */
ERRORS_t SPI_SwSlaveManage(SPI_Config_t *Config, SPI_SW_SLAVE_STATUS_t SlaveManagementState);

/**
 * @brief  : This Function is Used to Change The SPI Node Role  in Half Duplex Mode ( Transmit or Receive )
 *
 * @param  : SPINum => This Parameter is Used to Select The SPI Peripheral Number to Be Used -> @SPI_t
 * @param  : Role => This Parameter is Used to Select The Role to Be Used -> @SPI_HALFDUPLEX_ROLE_t
 * @return : ERRORS_t => This Return Parameter is Used to Indicate The Function Execution If Executed Correctly or NOT
 * @note   : This Function is Used with Half Duplex Mode Only
 */
ERRORS_t SPI_HalfDuplexRoleChange(SPI_t SPINum, SPI_HALFDUPLEX_ROLE_t Role);

/**
 * @brief  : This Function is Used to Enable DMA  Request Line Between SPI and DMA ( TX or RX )
 * @param  : SPINum => This Parameter is Used to Select The SPI Peripheral Number to Be Used -> @SPI_t
 * @param  : DMA_Line => This Parameter is Used to Select The DMA Line to Be Used -> @SPI_DMA_LINE_t
 * @return : ERRORS_t => This Return Parameter is Used to Indicate The Function Execution If Executed Correctly or NOT
 * @note   : This Function is Used to Handle SPI Data Transmission Using DMA , You Should Call This Function Before Calling The SPI_Init Function
 */
ERRORS_t SPI_EnableDMALine(SPI_t SPINum, SPI_DMA_LINE_t DMA_Line);

/**
 * @brief  : This Function is Used to Disable DMA Request Line Between SPI and DMA ( TX or RX )
 * @param  : SPINum => This Parameter is Used to Select The SPI Peripheral Number to Be Used -> @SPI_t
 * @param  : DMA_Line => This Parameter is Used to Select The DMA Line to Be Used -> @SPI_DMA_LINE_t
 * @return : ERRORS_t => This Return Parameter is Used to Indicate The Function Execution If Executed Correctly or NOT
 * @note   : This Function is Used to Handle SPI Data Transmission Using DMA , You Should Call This Function Before Calling The SPI_Init Function
 */
ERRORS_t SPI_DisableDMALine(SPI_t SPINum, SPI_DMA_LINE_t DMA_Line);

/**
 * @brief  : This Function is Used to Wait Until a Flag is Set
 * @param  : SPINum => This Parameter is Used to Select The SPI Peripheral Number to Be Used -> @SPI_t
 * @param  : Flag => This Parameter is Used to Select The Flag to Be Read -> @SPI_FLAG_t
 * @return : ERRORS_t => This Return Parameter is Used to Indicate The Function Execution If Executed Correctly or NOT
 */
ERRORS_t SPI_WaitUntilFlagSet(SPI_t SPINum, SPI_FLAG_t Flag);

/**
 * @brief  : This Function is Used to Send Data & Receive Data at The Same Time
 * @param  : Config => This is a Pointer to Struct of Type SPI_Config_t That Holds The Required Configuration
 * @param  : ReceivedData => This is a Pointer to uint16_t Variable That Holds The Received Data
 * @param  : DataToSend => This is a Pointer of Type uint16_t That Holds The Data to Be Sent
 * @return : ERRORS_t => This Return Parameter is Used to Indicate The Function Execution If Executed Correctly or NOT
 * @note   : IF You Want to Send Data Only You Can Pass NULL to The ReceivedData Parameter ,
 *         IF You Want to Receive Data Only You Can NULL to The DataToSend Parameter
 */

ERRORS_t SPI_TransceiveData(SPI_Config_t *Config, uint16_t *ReceivedData, uint16_t *DataToSend);

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
ERRORS_t SPI_TransceiveBuffer(SPI_Config_t *Config, uint8_t *ReceviedBuffer, uint8_t *BufferToSend, uint16_t BufferSize);

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
ERRORS_t SPI_TransceiveData_IT(SPI_Config_t *Config, uint16_t *ReceivedData, uint16_t *DataToSend, void(*pv_CALL_BACK_FUNC));

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
ERRORS_t SPI_TransceiveBuffer_IT(SPI_Config_t *Config, uint8_t *ReceivedBuffer, uint8_t *BufferToSend, uint16_t BufferSize, void (*pv_CallBackFunc)(void));

#endif /* SPI_INTERFACE_H_ */
