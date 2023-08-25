/*
 ******************************************************************************
 * @file           : SYSTICK_Interface.h
 * @Author         : Mohammed Ayman Shalaby
 * @brief          : User Interface File
 * @Date           : May 12, 2023
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 Ayman.
 * All rights reserved.
 *
 ******************************************************************************
 */
#ifndef SYSTICK_INTERFACE_H_
#define SYSTICK_INTERFACE_H_

/* ======================================================================
 * SYSTICK Option Enums
 * ====================================================================== */

/**
 * @enum  : @SYSTICK_EXCEPTION_t
 * @brief : This Enum Holds Values of All Options For Configuring Exception For Systick
 *
 */

typedef enum
{
    DISABLE_SYSTICK_EXCEPTION = 0,/**< DISABLE_SYSTICK_EXCEPTION */
    ENABLE_SYSTICK_EXCEPTION      /**< ENABLE_SYSTICK_EXCEPTION */

} SYSTICK_EXCEPTION_t;




/**
 * @enum  : @SYSTICK_CLK_SOURCE_t
 * @brief : This  Enum Holds Options For System Timer Clock
 *
 */

typedef enum
{
    SYSTICK_AHB_BY8 = 0,/**< SYSTICK_AHB_BY8 */
    SYSTICK_AHB         /**< SYSTICK_AHB */

} SYSTICK_CLK_SOURCE_t;

/* ---------------------------------------------------------------------------------------- */
/* ------------------------------- SYSTICK CONFIG STRUCTURE ------------------------------- */
/* ---------------------------------------------------------------------------------------- */

typedef struct
{
    SYSTICK_EXCEPTION_t Exception;
    SYSTICK_CLK_SOURCE_t CLK;

} SYSTICK_CONFIG_t;


/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------- FUCTION PROTOTYPES SECTION START ------------------------------- */
/* ------------------------------------------------------------------------------------------------ */

/* Note : Kindly Read the Documentation Clearly Before Using the Functions */

/**
 * @fn     : SYSTICK_Delayms
 * @brief  : Fuction That Makes Delay in Software Based on Passed Number of Milliseconds
 * @param  : Copy_u32TimeInMillis -> Time Passed To Delay in Milliseconds
 * @retval : ErrorStatus To Indicate if Function Worked Correctly or Not
 * @note   : => While Using ( STYSTICK_AHB ) Configuration Max Delay you can make in milliseconds is ( 1000 ms = 1s ) if you want more than this value
 *              you can go with the other configuration ( SYSTICK_AHB_BY8 ) or Just Use ( SYSTICK_AHB ) & Repeat Calling the Function
 *
 *           => While Using ( SYSTICK_AHB_BY8 ) Configuration Max Delay you can make in milliseconds is ( 8000 ms = 8s )
 **/

ERRORS_t SYSTICK_Delayms(uint32_t Copy_u32TimeInMillis) ;



/**
 * @fn     : SYSTICK_Delayus
 * @brief  : Fuction That Makes Delay in Software Based on Passed Number of MicroSeconds
 * @param  : Copy_u32TimeInMicroSeconds => Time Passed To Delay in MicroSeconds
 * @retval : ErrorStatus To Indicate If Function Worked Properly
 * @note   : => While Using ( STYSTICK_AHB ) Configuration Max Delay you can make in microseconds is ( 1000000 us = 1s ) if you want more than this value
 *
 *           => While Using ( SYSTICK_AHB_BY8 ) Configuration Max Delay you can make in microseconds is ( 8000000 us = 8s )
 **/
ERRORS_t SYSTICK_Delayus(uint32_t Copy_u32TimeInMicroSeconds) ;

#endif /* SYSTICK_INTERFACE_H_ */
