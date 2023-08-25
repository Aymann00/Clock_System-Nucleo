/*
 ******************************************************************************
 * @file           : EXTI_Private.h
 * @Author         : Mohammed Ayman Shalaby
 * @brief          : Private File For DMA Driver
 * @Date           : Jul 20, 2023
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 Ayman.
 * All rights reserved.
 *
 ******************************************************************************
 */

#ifndef DMA_PRIVATE_H_
#define DMA_PRIVATE_H_

/* ======================================================================
 * PRIVATE FUNCTION
 * ====================================================================== */

/**
 * @brief : This Function Checks on the Configuration Structure of the DMA to Check if it's Valid or Not
 *
 * @param : DMA_InitConfig =>  Struct that holds all configurations of the DMA -> Check Struct ( @DMA_INIT_STRUCT_t )
 * @note  : This Function is a Private Function , Specified For Driver Use Only
 * @return: ERRORS_t => Error Status To Indicate if Function Worked Properly( DMA_OK ) or Not ( DMA_NOK )
 */
static Error_State_t DMA_CheckInitConfig(DMA_INIT_STRUCT_t *DMA_InitConfig);

/**
 * @brief  : This Function Handles the Interrupts of the DMA When DMA IRQ Handler is Called
 *
 * @param  : DMANumber => Enum that holds All Possible DMA Controllers -> Check Options ( @DMA_CONTROLLER_t )
 * @param  : StreamNumber => Enum that holds All Possible Streams -> Check Options ( @DMA_STREAMS_t )
 * @return : ERRORS_t => Error Status To Indicate if Function Worked Properly( DMA_OK ) or Not ( DMA_NOK )
 * @note   : This Function is a Private Function , Specified For Driver Use Only
 */
static Error_State_t DMA_IRQHandler(DMA_CONTROLLER_t DMANumber, DMA_STREAMS_t StreamNumber);

/* ======================================================================
 * MASKS
 * ====================================================================== */
#define DMA_CHSEL_MASK 0xF1FFFFFFUL
#define DMA_MBURST_MASK 0xFE7FFFFFUL
#define DMA_PBURST_MASK 0xFF9FFFFFUL
#define DMA_PL_MASK 0xFFFCFFFFUL
#define DMA_MSIZE_MASK 0xFFFF9FFFUL
#define DMA_PSIZE_MASK 0xFFFFE7FFUL
#define DMA_DIR_MASK 0xFFFFFF3FUL
#define DMA_CIRC_MASK 0xFFFFFEFF
#define DMA_MINC_MASK 0xFFFFFBFFUL
#define DMA_PINC_MASK 0xFFFFFDFFUL
#define DMA_TCIE_MASK 0xFFFFFFEFUL
#define DMA_HTIE_MASK 0xFFFFFFF7UL
#define DMA_TEIE_MASK 0xFFFFFFFBUL
#define DMA_DMEIE_MASK 0xFFFFFFFDUL
#define DMA_DBM_MASK 0xFFFBFFFF

#define DMA_FEIE_MASK 0x3FUL
#define DMA_DMDIS_MASK 0xBBUL
#define DMA_FTH_MASK 0xBCUL

#define DMA_EN_MASK 0xFFFFFFFEUL

/* ======================================================================
 * MACROS
 * ====================================================================== */
#define DMA_INT_NUM 5U

#endif /* DMA_PRIVATE_H_ */
