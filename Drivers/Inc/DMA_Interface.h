/*
 ******************************************************************************
 * @file           : DMA_Interface.h
 * @Author         : Mohammed Ayman Shalaby
 * @brief          : User Interface file for DMA Module
 * @Date           : Jul 20, 2023
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 Ayman.
 * All rights reserved.
 *
 ******************************************************************************
 */

#ifndef DMA_INTERFACE_H_
#define DMA_INTERFACE_H_

/* ======================================================================
 * DMA Option Enums
 * ====================================================================== */

/**
 * @brief : Enum that holds Options for Available DMA Controllers we have
 * @enum  : @DMA_CONTROLLER_t
 */
typedef enum
{
    DMA1_CONTROLLER = 0,
    DMA2_CONTROLLER = 1
} DMA_CONTROLLER_t;

/**
 * @brief : Enum that holds Options for Available Streams in the DMA Controller
 * @enum  : @DMA_STREAMS_t
 */
typedef enum
{
    DMA_STREAM0 = 0, /* DMA Stream 0 */
    DMA_STREAM1 = 1, /* DMA Stream 1 */
    DMA_STREAM2 = 2, /* DMA Stream 2 */
    DMA_STREAM3 = 3, /* DMA Stream 3 */
    DMA_STREAM4 = 4, /* DMA Stream 4 */
    DMA_STREAM5 = 5, /* DMA Stream 5 */
    DMA_STREAM6 = 6, /* DMA Stream 6 */
    DMA_STREAM7 = 7  /* DMA Stream 7 */

} DMA_STREAMS_t;

/**
 * @brief : Enum that holds All Interrupt Flags that can Generate Interrupt in a Certain Stream
 * @enum  : @DMA_Interrupt_Flag_t
 */
typedef enum
{
    FIFO_ERROR_IT_FLAG = 0,        /* FIFO Error Interrupt Flag */
    DIRECT_MODE_ERROR_IT_FLAG = 2, /* Direct Mode Error Interrupt Flag */
    TRANSFER_ERROR_IT_FLAG = 3,    /* Transfer Error Interrupt Flag */
    HALF_TRANSFER_IT_FLAG = 4,     /* Half Transfer Interrupt Flag */
    TRANSFER_COMPLETE_IT_FLAG = 5  /* Transfer Complete Interrupt Flag */
} DMA_Interrupt_Flag_t;

/**
 * @brief : Enum that holds Interrupt Flags Status ( Raised or Not Raised ) of a Certain Stream
 * @enum  : @DMA_Interrupt_Flag_Status_t
 */
typedef enum
{
    DMA_FLAG_RESET = 0, /* Flag is not Raised */
    DMA_FLAG_SET = 1    /* Flag is Raised */

} DMA_Interrupt_Flag_Status_t;

/**
 * @brief : Enum that holds All Possible Channels that can Request a Certain Stream
 * @enum  : @DMA_CHANNEL_t
 */
typedef enum
{
    DMA_CHANNEL0 = 0,
    DMA_CHANNEL1 = 1,
    DMA_CHANNEL2 = 2,
    DMA_CHANNEL3 = 3,
    DMA_CHANNEL4 = 4,
    DMA_CHANNEL5 = 5,
    DMA_CHANNEL6 = 6,
    DMA_CHANNEL7 = 7,

} DMA_CHANNEL_t;

/**
 * @brief : Enum that holds Options for Memory Burst
 * @enum  : @DMA_MEM_BURST_t
 */
typedef enum
{
    DMA_MEM_SINGLE_TRANSFER = 0,
    DMA_MEM_BURST4_TRANSFER = 1,
    DMA_MEM_BURST8_TRANSFER = 2,
    DMA_MEM_BURST16_TRANSFER = 3,

} DMA_MEM_BURST_t;

/**
 * @brief : Enum that holds Options for Peripheral Burst
 * @enum  : @DMA_PERIPH_BURST_t
 */
typedef enum
{
    DMA_PERIPH_SINGLE_TRANSFER = 0,
    DMA_PERIPH_BURST4_TRANSFER = 1,
    DMA_PERIPH_BURST8_TRANSFER = 2,
    DMA_PERIPH_BURST16_TRANSFER = 3,
} DMA_PERIPH_BURST_t;

/**
 * @brief : Enum that holds Options for Priority Levels
 * @enum  : @DMA_PRIORITY_t
 */
typedef enum
{
    DMA_LOW_PRIORITY = 0,       /* Low Priority */
    DMA_MEDIUM_PRIORITY = 1,    /* Medium Priority */
    DMA_HIGH_PRIORITY = 2,      /* High Priority */
    DMA_VERY_HIGH_PRIORITY = 3, /* Very High Priority */

} DMA_PRIORITY_t;

typedef enum
{
    DMA_DOUBLE_BUFFER_DIS = 0, /* Double Buffer Mode Disable */
    DMA_DOUBLE_BUFFER_EN = 1   /* Double Buffer Mode Enable */
} DMA_DOUBLE_BUFFER_t;

/**
 * @brief : Enum that holds Options for Memory Data Width
 * @enum  : @DMA_MEM_WIDTH_t
 */
typedef enum
{
    DMA_MEM_DATA_WIDTH_8BITS = 0,  /* Memory Data Width 8 Bits */
    DMA_MEM_DATA_WIDTH_16BITS = 1, /* Memory Data Width 16 Bits */
    DMA_MEM_DATA_WIDTH_32BITS = 2, /* Memory Data Width 32 Bits */

} DMA_MEM_WIDTH_t;

/**
 * @brief : Enum that holds Options for Peripheral Data Width
 * @enum  : @DMA_PERIPH_WIDTH_t
 */
typedef enum
{
    DMA_PERIPH_DATA_WIDTH_8BITS = 0,  /* Peripheral Data Width 8 Bits */
    DMA_PERIPH_DATA_WIDTH_16BITS = 1, /* Peripheral Data Width 16 Bits */
    DMA_PERIPH_DATA_WIDTH_32BITS = 2, /* Peripheral Data Width 32 Bits */

} DMA_PERIPH_WIDTH_t;

/**
 * @brief : Enum that holds Options for Memory Increment Mode
 * @enum  : @DMA_MINC_t
 */
typedef enum
{
    DMA_MINC_DISABLE = 0, /* Memory Increment Mode Disable */
    DMA_MINC_ENABLE = 1,  /* Memory Increment Mode Enable */

} DMA_MINC_t;

/**
 * @brief : Enum that holds Options for Peripheral Increment Mode
 * @enum  : @DMA_PINC_t
 */
typedef enum
{
    DMA_PINC_DISABLE = 0, /* Peripheral Increment Mode Disable */
    DMA_PINC_ENABLE = 1,  /* Peripheral Increment Mode Enable */

} DMA_PINC_t;

/**
 * @brief : Enum that holds Options for DMA Mode
 * @enum  : @DMA_MODE_t
 */
typedef enum
{
    DMA_NORMAL = 0,
    DMA_CIRCULAR = 1

} DMA_MODE_t;

/**
 * @brief : Enum that holds Data Transfer Direction
 * @enum  : @DMA_DATA_DIRECTION_t
 */
typedef enum
{
    DMA_PERIPH_TO_MEM = 0,
    DMA_MEM_TO_PERIPH = 1,
    DMA_MEM_TO_MEM = 2

} DMA_DATA_DIRECTION_t;

/**
 * @brief : Enum that holds Options for Initial State of Interrupts ( Enable or Disable )
 * @enum  : @DMA_INT_INITIAL_STATE_t
 */
typedef enum
{
    DMA_INT_DISABLE = 0,
    DMA_INT_ENABLE = 1

} DMA_INT_INITIAL_STATE_t;

/**
 * @brief : Struct that holds All Interrupts that can be Enabled or Disabled
 * @struct: @DMA_INT_ENABLE_STRUCT_t
 * @note  : This Struct is Inside DMA_INIT_STRUCT_t Struct & used to Enable or Disable Multiple Interrupts Inside (DMA_Init) Function
 *           you Can Use the Struct Separately to Enable or Disable Interrupts throug (EnableIT) Function
 */
typedef struct
{
    DMA_INT_INITIAL_STATE_t FIFOErrorIT;
    DMA_INT_INITIAL_STATE_t DirectModeErrorIT;
    DMA_INT_INITIAL_STATE_t TransferErrorIT;
    DMA_INT_INITIAL_STATE_t HalfTransferIT;
    DMA_INT_INITIAL_STATE_t TransferCompleteIT;

} DMA_INT_ENABLE_STRUCT_t;

/**
 * @brief : Enum that holds Options for FIFO Mode If you Want to Enable or Disable it
 * @enum  : @DMA_FIFO_MODE_t
 */
typedef enum
{
    DMA_FIFOMODE_DISABLE = 0,
    DMA_FIFOMODE_ENABLE = 1

} DMA_FIFO_MODE_t;

/**
 * @brief : Enum that holds Options for FIFO Threshold Level
 * @enum  : @DMA_FIFO_THRESHOLD_t
 */
typedef enum
{
    DMA_FIFO_THRESHOLD_1QUARTER_FULL = 0,
    DMA_FIFO_THRESHOLD_HALF_FULL = 1,
    DMA_FIFO_THRESHOLD_3QUARTERS_FULL = 2,
    DMA_FIFO_THRESHOLD_FULL = 3

} DMA_FIFO_THRESHOLD_t;

/**
 * @brief : Enum that holds All Possible Interrupts that can Occur in a Certain Stream To Pass To CallBack Function
 * @enum  : @DMA_CALLBACK_ID_t
 */
typedef enum
{
    DMA_TRANSFER_CMP_CALLBACK = 0,
    DMA_HALF_TRANSFER_CALLBACK = 1,
    DMA_TRANSFER_ERROR_CALLBACK = 2,
    DMA_DIRECT_MODE_ERROR_CALLBACK = 3,
    DMA_FIFO_ERROR_CALLBACK = 4

} DMA_CALLBACK_ID_t;

/**
 * @brief : Struct that holds all configurations of the DMA
 * @struct: @DMA_INIT_STRUCT_t
 */
typedef struct
{
    DMA_CONTROLLER_t DMAController;     /* DMA Controller Number */
    DMA_STREAMS_t StreamNumber;         /* DMA Stream Number */
    DMA_CHANNEL_t ChannelNumber;        /* DMA Channel Number */
    DMA_PERIPH_BURST_t PeriphBurst;     /* DMA Peripheral Burst */
    DMA_MEM_BURST_t MemBurst;           /* DMA Memory Burst */
    DMA_PRIORITY_t Priority;            /* DMA Priority */
    DMA_MEM_WIDTH_t MemDataWidth;       /* DMA Memory Data Width */
    DMA_PERIPH_WIDTH_t PeriphDataWidth; /* DMA Peripheral Data Width */
    DMA_MINC_t MemInc;                  /* DMA Memory Increment Mode */
    DMA_PINC_t PeriphInc;               /* DMA Peripheral Increment Mode */
    DMA_MODE_t Mode;                    /* DMA Mode */
    DMA_DOUBLE_BUFFER_t DoubleBuffer;   /* DMA Double Buffer */
    DMA_DATA_DIRECTION_t Direction;     /* DMA Data Direction */
    DMA_INT_ENABLE_STRUCT_t EnableIT;   /* DMA Interrupt Enable */
    DMA_FIFO_MODE_t FIFOMode;           /* DMA FIFO Mode */
    DMA_FIFO_THRESHOLD_t FIFOThreshold; /* DMA FIFO Threshold */
} DMA_INIT_STRUCT_t;

/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------- FUCTION PROTOTYPES SECTION START ------------------------------- */
/* ------------------------------------------------------------------------------------------------ */

/**
 * @brief : The Function Clears A Specified  Interrupt Flag of A Specified Stream in a Certain DMA Controller of the Two DMA Controllers
 *
 * @param : DMAController => Enum that holds Options for Available DMA Controllers we have -> To choose Check enum ( @DMA_CONTROLLER_t )
 * @param : StreamNumber => Enum that holds Options for Available Streams in the DMA Controller -> Check Options ( @DMA_STREAMS_t )
 * @param : InterruptFlag => Enum that holds All Interrupt Flags that can Generate Interrupt in a Certain Stream -> Check Options ( @DMA_Interrupt_Flags_t )
 * @return: ERRORS_t => Error Status To Indicate if Function Worked Properly
 */
Error_State_t DMA_ClearInterruptFlag(DMA_CONTROLLER_t DMAController, DMA_STREAMS_t StreamNumber, DMA_Interrupt_Flag_t InterruptFlag);

/**
 * @brief : The Function Reads A Specified  Interrupt Flag of A Specified Stream in a Certain DMA Controller of the Two DMA Controllers
 *
 * @param : DMAController => Enum that holds Options for Available DMA Controllers we have -> To choose Check enum ( @DMA_CONTROLLER_t )
 * @param : StreamNumber => Enum that holds Options for Available Streams in the DMA Controller -> Check Options ( @DMA_STREAMS_t )
 * @param : InterruptFlag => Enum that holds All Interrupt Flags that can Generate Interrupt in a Certain Stream -> Check Options ( @DMA_Interrupt_Flags_t )
 * @param : InterruptFlagStatus => Pointer to a Variable that will hold the Status of the Interrupt Flag -> Check Options ( @DMA_Interrupt_Flag_Status_t )
 * @return: ERRORS_t => Error Status To Indicate if Function Worked Properly
 */
Error_State_t DMA_ReadInterruptFlag(DMA_CONTROLLER_t DMAController, DMA_STREAMS_t StreamNumber, DMA_Interrupt_Flag_t InterruptFlag, DMA_Interrupt_Flag_Status_t *InterruptFlagStatus);

/**
 * @brief : The Function Selects a Certain Channel on a Certain Stream in a Specifec DMA Controller
 *
 * @param : DMAController => Enum that holds Options for Available DMA Controllers we have -> To choose Check enum ( @DMA_CONTROLLER_t )
 * @param : StreamNumber => Enum that holds Options for Available Streams in the DMA Controller -> Check Options ( @DMA_STREAMS_t )
 * @param : ChannelNumber => Enum that holds All Possible Channels that can Request a Certain Stream -> Check Options ( @DMA_CHANNEL_t )
 * @return: ERRORS_t => Error Status To Indicate if Function Worked Properly
 */
Error_State_t DMA_SetChannel(DMA_CONTROLLER_t DMAController, DMA_STREAMS_t StreamNumber, DMA_CHANNEL_t ChannelNumber);

/**
 * @brief : The Function Set a Priority Level to a Certain Stream in a DMA Controller
 *
 * @param : DMAController => Enum that holds Options for Available DMA Controllers we have -> To choose Check enum ( @DMA_CONTROLLER_t )
 * @param : StreamNumber => Enum that holds Options for Available Streams in the DMA Controller -> Check Options ( @DMA_STREAMS_t )
 * @param : PriorityLevel => Enum that holds All Possible Values of Priority Levels you can give to a certain stream -> Check Options ( @DMA_PRIORITY_t )
 * @return: ERRORS_t => Error Status To Indicate if Function Worked Properly
 */
Error_State_t DMA_SetStreamPriority(DMA_CONTROLLER_t DMAController, DMA_STREAMS_t StreamNumber, DMA_PRIORITY_t PriorityLevel);

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
Error_State_t DMA_Init(DMA_INIT_STRUCT_t *Init);

/**
 * @brief : The Function Configures Interrupt Enabling Or Disabling Of All Interrupts in a Certain Stream in a Certain DMA Controller
 *
 * @param : DMA_InitConfig =>  Struct that holds all configurations of the DMA -> Check Struct ( @DMA_INIT_STRUCT_t )
 * @note  : This Function is Being Called in DMA_Init Function , So No Need to Call it Again Unless You Want to Change Interrupt Configurations
 * @return: ERRORS_t => Error Status To Indicate if Function Worked Properly
 */
Error_State_t DMA_EnableIT(DMA_INIT_STRUCT_t *DMA_InitConfig);

/**
 * @brief : This Function Disables a Certain Stream in a Certain DMA Controller
 *
 * @param  : DMANumber    =>  Enum that holds Options for Available DMA Controllers we have -> To choose Check enum ( @DMA_CONTROLLER_t )
 * @param  : StreamNumber => Enum that holds Options for Available Streams in the DMA Controller -> Check Options ( @DMA_STREAMS_t )
 * @return : ERRORS_t => Error Status To Indicate if Function Worked Properly
 */
Error_State_t DMA_DisableStream(DMA_CONTROLLER_t DMANumber, DMA_STREAMS_t StreamNumber);

/**
 * @brief : This Function Sets Source Address & Distination Address & Enables the Stream
 *
 * @param  : InitConfig  => Struct that holds all configurations of the DMA -> Check Struct ( @DMA_INIT_STRUCT_t )
 * @param  : SrcAddress  => Pointer to a Variable that holds the Source Address
 * @param  : DestAddress => Pointer to a Variable that holds the Destination Address
 * @param  : DataLength  => Variable that holds the Number of Data Transfers
 * @return : ERRORS_t => Error Status To Indicate if Function Worked Properly
 */
Error_State_t DMA_StartTransfer(DMA_INIT_STRUCT_t *InitConfig, uint32_t *SrcAddress, uint32_t *DestAddress, uint16_t DataLength);

/**
 * @brief  : This Functoin Sets a CallBack Function to a Certain Interrupt
 * @fn     : DMA_SetCallBack
 * @param  : InitConfig => Struct that holds all configurations of the DMA -> Check Struct ( @DMA_INIT_STRUCT_t )
 * @param  : CallBackID => Enum that holds All Possible Interrupts that can Occur in a Certain Stream -> Check Options ( @DMA_CALLBACK_ID_t )
 * @param  : Copy_pvCallBack => Pointer to Function that will be Called when the Interrupt Occurs
 * @return : ERRORS_t => Error Status To Indicate if Function Worked Properly
 */
Error_State_t DMA_SetCallBack(DMA_INIT_STRUCT_t *InitConfig, DMA_CALLBACK_ID_t CallBackID, void (*Copy_pvCallBack)(void));

/* ---------------------------------------------------------------------------------------------- */
/* ------------------------------- FUCTION PROTOTYPES SECTION END ------------------------------- */
/* ---------------------------------------------------------------------------------------------- */
#endif
