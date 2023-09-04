/*
 *@file		:	SPI_Interface.h
 *@author	: 	Mohamed Wael
 *@brief	:	Main Functions & Enumerators Defines FOR SPI Peripheral
 */

#ifndef SPI_INC_SPI_INTERFACE_H_
#define SPI_INC_SPI_INTERFACE_H_


/************** MAIN USER DEFINED VARIABLES ************/
typedef enum {
	SPI_NUMBER1,
	SPI_NUMBER2,
	SPI_NUMBER3,
	SPI_NUMBER4
}SPI_SPI_NUMBER_t;
typedef enum {
	BAUDRATE_FpclkBY2,
	BAUDRATE_FpclkBY4,
	BAUDRATE_FpclkBY8,
	BAUDRATE_FpclkBY16,
	BAUDRATE_FpclkBY32,
	BAUDRATE_FpclkBY64,
	BAUDRATE_FpclkBY128,
	BAUDRATE_FpclkBY256,
}SPI_BAUDRATE_VALUES_t;

typedef enum {
	CLOCK_POLARITY_IDLE_LOW,
	CLOCK_POLARITY_IDLE_HIGH,
}SPI_CLOCK_POLARITY_t;

typedef enum {
	CLOCK_PHASE_CAPTURE_FIRST,
	CLOCK_PHASE_CAPTURE_SECOND
}SPI_CLOCK_PHASE_t;

typedef enum {

	TRANSFER_MODE_FULL_DUPLEX,
	TRANSFER_MODE_HALF_DUPLEX_RECEIVER=2,
	TRANSFER_MODE_HALF_DUPLEX_TRANSMITTER,
	TRANSFER_MODE_SIMPLEX,					/*Selected if the Chip is receiver always*/

}SPI_TRANSFER_MODES_t;

typedef enum {
	FRAME_FORMAT_MSB_FIRST,
	FRAME_FORMAT_LSB_FIRST,

}SPI_FRAME_FORMAT_TYPE_t;

typedef enum {
	DATA_FRAME_SIZE_8BITS,
	DATA_FRAME_SIZE_16BITS,
}SPI_DATA_FRAME_SIZE_t;

typedef enum {
	CHIP_MODE_SLAVE	,
	CHIP_MODE_MASTER
}SPI_CHIP_MODE_t;

typedef enum {

	SLAVE_MANAGE_HW,
	SLAVE_MANAGE_SW_SLAVE_ACTIVE=2,
	SLAVE_MANAGE_SW_SLAVE_INACTIVE,

}SPI_SLAVE_MANAGE_STATE_t;

typedef enum {
	CRC_STATE_DISABLED,
	CRC_STATE_ENABLED
}SPI_CRC_STATE_t;

typedef enum {
	MULTIMASTER_PROVIDED,
	MULTIMASTER_NOT_PROVIDED,
}SPI_MULTIMASTER_STATE_t;

typedef enum {
	SPI_FLAGS_RXNE,
	SPI_FLAGS_TXE,
	SPI_FLAGS_CHSIDE,
	SPI_FLAGS_UDR,
	SPI_FLAGS_CRCERR,
	SPI_FLAGS_MODF,
	SPI_FLAGS_OVR,
	SPI_FLAGS_BSY,
	SPI_FLAGS_FE,
}SPI_FLAGS_t;

typedef enum {
	SPI_INTERRUPT_ERRIE=5,
	SPI_INTERRUPT_RXNEIE,
	SPI_INTERRUPT_TXEIE,
}SPI_INTERRUPTS_t;

typedef enum {
	NO_SRC,
	SOURCE_TX,
	SOURCE_RX_SLAVE,
	SOURCE_RX_MASTER
}IRQ_SOURCES_t;

typedef enum {
	SLAVE_STATE_ACTIVATED,
	SLAVE_STATE_DEACTIVATED
}SLAVE_STATE_t;
typedef struct{
	SPI_SPI_NUMBER_t				SPI_Num;
	SPI_BAUDRATE_VALUES_t 			BaudRate_Value;
	SPI_CLOCK_POLARITY_t			Clock_Polarity;
	SPI_CLOCK_PHASE_t				Clock_Phase;
	SPI_TRANSFER_MODES_t			Transfer_Mode;
	SPI_FRAME_FORMAT_TYPE_t 		Frame_Type;
	SPI_DATA_FRAME_SIZE_t			Frame_Size;
	SPI_CHIP_MODE_t					Chip_Mode;
	SPI_SLAVE_MANAGE_STATE_t		Slave_Manage_State;
	SPI_CRC_STATE_t					CRC_State;
	SPI_MULTIMASTER_STATE_t			MultiMaster_State;
}SPI_CONFIGS_t;
/******** END OF MAIN USER DEFINED VARIABLES ***********/


/****************** MAIN FUNCTIONS *********************/

/*
 * @function 		:	SPI_Init
 * @brief			:	Initialize SPI
 * @param			:	SPI Configurations structure
 * @retval			:	Error State
 */
Error_State_t SPI_Init(const SPI_CONFIGS_t * SPI_Config);

/*
 * @function 		:	SPI_Transmit
 * @brief			:	Transmit Data via SPI
* @param			:	SPI Configurations structure
 * @param			:	pointer to Data Buffer
 * @param 			: 	Data Buffer Size
 * @retval			:	Error State
 */
Error_State_t SPI_Transmit(const SPI_CONFIGS_t * SPI_Config, uint16_t * Data , uint8_t Buffer_Size);

/*
 * @function 		:	SPI_Receive
 * @brief			:	Receive Data via SPI
 * @param			:	SPI Configurations structure
 * @param			:	Buffer to save Data
 * @param 			: 	Data Buffer Size
 * @retval			:	Error State
 */
Error_State_t SPI_Receive(const SPI_CONFIGS_t * SPI_Config, uint16_t * Received_Data ,uint8_t Buffer_Size);


/*
 * @function 		:	SPI_Transmit_IT
 * @brief			:	Transmit Data via SPI and
 * 						generate interrupt when Transmission is complete
 * @param			:	SPI Configurations structure
 * @param			:	Data To Send
 * @param 			: 	Data Buffer Size
 * @param			:	CallBack Function
 * @retval			:	Error State
 */
Error_State_t SPI_Transmit_IT(const SPI_CONFIGS_t * SPI_Config, uint8_t * Data , uint8_t Buffer_Size, void (* SPI_TXC_CallBackFunc)(void));

/*
 * @function 		:	SPI_Receive_IT
 * @brief			:	Receive Data via SPI and generate interrupt when receive is complete
 * @param			:	SPI Configurations structure
 * @param			:	Buffer to save Data
 * @param 			: 	Data Buffer Size
 * @param			:	CallBack Function
 * @retval			:	Error State
 */
Error_State_t SPI_Receive_IT(const SPI_CONFIGS_t * SPI_Config, uint8_t * Received_Data ,uint8_t Buffer_Size , void (* SPI_RXC_CallBackFunc)(void));

/*
 * @function 		:	SPI_Enable_DMA_RX
 * @brief			:	Enable DMA Line for SPI Receiving
 * @param			:	SPI NUMBER
 * @retval			:	Error State
 */
Error_State_t SPI_Enable_DMA_RX(SPI_SPI_NUMBER_t SPI_Num);

/*
 * @function 		:	SPI_Enable_DMA_TX
 * @brief			:	Enable DMA Line for SPI Transmitting
 * @param			:	SPI NUMBER
 * @retval			:	Error State
 */
Error_State_t SPI_Enable_DMA_TX(SPI_SPI_NUMBER_t SPI_Num);

/*
 * @function 		:	SPI_Internal_Slave_Activate
 * @brief			:	Enable Slave by itself
 * @param			:	SPI NUMBER
 * @param			:	Slave_State
 * @retval			:	Error State
 * @warning			:	this function is used only if the Chip mode is slave and
 * 						SW Slave management is enabled
 */
Error_State_t SPI_SET_Internal_Slave_State(SPI_SPI_NUMBER_t SPI_Num, SLAVE_STATE_t Slave_State);


/***************End of MAIN FUNCTIONS*******************/


#endif /* SPI_INC_SPI_INTERFACE_H_ */
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
