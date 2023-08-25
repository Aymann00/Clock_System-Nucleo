/*
 ******************************************************************************
 * @file           : NVIC_Interface.h
 * @Author         : Mohammed Ayman Shalaby
 * @brief          : Main program body
 * @Date           : Jul 7, 2023
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 Ayman.
 * All rights reserved.
 *
 ******************************************************************************
 */
#ifndef NVIC_INC_NVIC_INTERFACE_H_
#define NVIC_INC_NVIC_INTERFACE_H_

/* ======================================================================
 * NVIC Option Enums
 * ====================================================================== */

/**
 * @enum  : @IRQNum_t
 * @brief : Holds Values of ALL IRQs that NVIC can Control
 *
 */
typedef enum
{
	WWDG_IRQ = 0 , PVD_IRQ , TAMP_STAMP_IRQ ,RTC_WKUP_IRQ , FLASH_IRQ , RCC_IRQ , EXTI0_IRQ , EXTI1_IRQ , EXTI2_IRQ , EXTI3_IRQ , EXTI4_IRQ ,
	DMA1_Stream0_IRQ , DMA1_Stream1_IRQ , DMA1_Stream2_IRQ , DMA1_Stream3_IRQ , DMA1_Stream4_IRQ , DMA1_Stream5_IRQ , DMA1_Stream6_IRQ , ADC_IRQ ,
	CAN1_TX_IRQ , CAN1_RX0_IRQ , CAN1_RX1_IRQ , CAN1_SCE_IRQ , EXTI9_5_IRQ , TIM1_BRK_TIM9_IRQ , TIM1_UP_TIM10_IRQ ,
	TIM1_TRG_COM_TIM11_IRQ , TIM1_CC_IRQ , TIM2_IRQ , TIM3_IRQ , TIM4_IRQ , I2C1_EV_IRQ , I2C1_ER_IRQ , I2C2_EV_IRQ ,
	I2C2_ER_IRQ , SPI1_IRQ , SPI2_IRQ , USART1_IRQ , USART2_IRQ , USART3_IRQ ,EXTI15_10_IRQ , RTC_Alarm_IRQ ,OTG_FS_WKUP_IRQ , TIM8_BRK_TIM12_IRQ , TIM8_UP_TIM13_IRQ ,	TIM8_TRG_COM_TIM14_IRQ ,TIM8_CC_IRQ ,DMA1_Stream7_IRQ , FMC_IRQ , SDIO_IRQ , TIM5_IRQ ,SPI3_IRQ , UART4_IRQ ,
	UART5_IRQ ,	TIM6_DAC_IRQ , TIM7_IRQ ,DMA2_Stream0_IRQ , DMA2_Stream1_IRQ , DMA2_Stream2_IRQ , DMA2_Stream3_IRQ , DMA2_Stream4_IRQ ,
    CAN2_TX_IRQ = 63 , CAN2_RX0_IRQ ,CAN2_RX1_IRQ , CAN2_SCE_IRQ , OTG_FS_IRQ ,DMA2_Stream5_IRQ , DMA2_Stream6_IRQ , DMA2_Stream7_IRQ , USART6_IRQ ,
	I2C3_EV_IRQ , I2C3_ER_IRQ ,OTG_HS_EP1_OUT_IRQ , OTG_HS_EP1_IN_IRQ ,	OTG_HS_WKUP_IRQ ,OTG_HS_IRQ , DCMI_IRQ , FPU_IRQ = 81 ,
	SPI4_IRQ = 84 ,SAI1_IRQ = 87 , SAI2_IRQ = 91 , QuadSPI_IRQ , HDMI_CEC_IRQ , SPDIF_Rx_IRQ , FMPI2C1_IRQ ,FMPI2C1_ERROR_IRQ , NUMBER_OF_IRQs

}IRQNum_t;

/**
 * @enum  : @PENDING_t
 * @brief : Holds Value of Pending State
 *
 */
typedef enum
{
	NOT_PENDING = 0 ,	PENDING

}PENDING_t;

/**
 * @enum  :  @ACTIVE_t
 * @brief : This Enum Holds Options of All Available Active STATUS
 *
 */
typedef enum
{
  NOT_ACTIVE = 0 , ACTIVE

}ACTIVE_t;

/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------- FUCTION PROTOTYPES SECTION START ------------------------------- */
/* ------------------------------------------------------------------------------------------------ */

/**
 * @fn    : NVIC_EnableIRQ
 * @brief : This Function Takes IRQ Number and Enables Peripheral From NVIC
 * @param : IRQ_Number -> From enum ( @IRQNum_t )
 * @retval: ErrorStatus to Indicate if Function Worked Correct or Not
 * @note  : Provide IRQ From the Specified in the enum only
 */
Error_State_t NVIC_EnableIRQ( IRQNum_t IRQ_Number ) ;


/**
 * @fn    : NVIC_DisableIRQ
 * @brief : This Function Takes IRQ Number and Disables Peripheral From NVIC
 * @param : IRQ_Number -> From enum ( @IRQNum_t )
 * @retval: ErrorStatus to Indicate if Function Worked Correct or Not
 * @note  : Provide IRQ From the Specified in the enum only
 */
Error_State_t NVIC_DisableIRQ( IRQNum_t IRQ_Number ) ;


/**
 * @fn    : NVIC_SetPendingIRQ
 * @brief : This Function Takes IRQ Number and Sets the Pending Flag of the Specified IRQ
 * @param : IRQ_Number -> From enum ( @IRQNum_t )
 * @retval: ErrorStatus to Indicate if Function Worked Correct or Not
 * @note  : Provide IRQ From the Specified in the enum only
 */
Error_State_t NVIC_SetPendingIRQ( IRQNum_t IRQ_Number ) ;


/**
 * @fn    : NVIC_ClearPendingIRQ
 * @brief : This Function Takes IRQ Number and Clears the Pending Flag of the Specified IRQ
 * @param : IRQ_Number -> From enum ( @IRQNum_t )
 * @retval: ErrorStatus to Indicate if Function Worked Correct or Not
 * @note  : Provide IRQ From the Specified in the enum only
 */
Error_State_t NVIC_ClearPendingIRQ( IRQNum_t IRQ_Number ) ;



/**
 * @fn     : NVIC_GetPendingIRQ
 * @brief  : This Function Take IRQ Number and a Pointer to dereference the pointer with the pending state
 * @param  : IRQ_Number -> From enum ( @IRQNum_t )
 * @param  : Pending_State -> A Pointer to a Variable that User Takes From it the Pending State ( Pending_t )
 * @retval : ErrorStatus to Indicate if Function Worked Correctly or Not
 */
Error_State_t NVIC_GetPendingIRQ( IRQNum_t IRQ_Number , PENDING_t * Pending_State  ) ;


/**
 * @fn     : NVIC_GetActiveIRQ
 * @brief  : This Function Take IRQ Number and a Pointer to dereference the pointer with the Active state
 * @param  : IRQ_Number -> From enum ( @IRQNum_t )
 * @param  : Active_State -> A Pointer to a Variable that User Takes From it the Active State ( Active_t )
 * @retval : ErrorStatus to Indicate if Function Worked Correctly or Not
 */
Error_State_t NVIC_GetActiveIRQ( IRQNum_t IRQ_Number , ACTIVE_t * Active_State ) ;




/**
 * @fn     : NVIC_SetPriority
 * @brief  : This Function Take IRQ Number and a Priority and assigns the priority to the associated IRQ
 * @param  : IRQ_Number -> From enum ( @IRQNum_t )
 * @param  : Copy_u8Priority -> Priority of the Specified IRQ
 * @retval : ErrorStatus to Indicate if Function Worked Correctly or Not
 */
Error_State_t NVIC_SetPriority( IRQNum_t IRQ_Number , uint8_t  Copy_u8Priority ) ;

/* ---------------------------------------------------------------------------------------------- */
/* ------------------------------- FUCTION PROTOTYPES SECTION END ------------------------------- */
/* ---------------------------------------------------------------------------------------------- */

#endif /* NVIC_INC_NVIC_INTERFACE_H_ */
