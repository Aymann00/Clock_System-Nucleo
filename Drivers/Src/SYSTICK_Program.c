/*
 ******************************************************************************
 * @file           : SYSTICK_Program.c
 * @Author         : Mohammed Ayman Shalaby
 * @brief          : SYSTICK Main Program File
 * @Date           : May 12, 2023
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

#include "../Inc/SYSTICK_Interface.h"
#include "../Inc/SYSTICK_Private.h"

/*==============================================================================================================================================
 * INCLUDES SECTION END
 *==============================================================================================================================================*/



/*==============================================================================================================================================
 * GLOBAL VARIABLES SECTION START
 *==============================================================================================================================================*/

extern SYSTICK_CONFIG_t SYSTICK_TIMER_CONFIG ;

/*==============================================================================================================================================
 * GLOBAL VARIABLES SECTION END
 *==============================================================================================================================================*/


/*==============================================================================================================================================
 * MODULES IMPLEMENTATION
 *==============================================================================================================================================*/

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

ERRORS_t SYSTICK_Delayms(uint32_t Copy_u32TimeInMillis)
{
	ERRORS_t Local_u8ErrorStatus = OK ;

	/* Variable To Carry RELOAD Value */
	uint32_t RELOAD_Value = 0 ;

	/* Check on TIMER_CLK  */
	if( SYSTICK_TIMER_CONFIG.CLK == SYSTICK_AHB )
	{
		RELOAD_Value = ( Copy_u32TimeInMillis * 1000UL ) / AHB_TICK_TIME ;
	}
	else if( SYSTICK_TIMER_CONFIG.CLK == SYSTICK_AHB_BY8 )
	{
		RELOAD_Value = ( Copy_u32TimeInMillis * 1000UL ) / AHB_BY8_TICK_TIME ;
	}
	else
	{
		Local_u8ErrorStatus = NOK ;
	}

	/* Setting Reload Value */
	( SYSTICK->SYST_RVR ) = RELOAD_Value ;

	/* Clear Current */
	( SYSTICK->SYST_CVR ) = 0 ;

	/* Set Exception */
	( SYSTICK->SYST_CSR ) &= ~(1<<CSR_TICKINT) ;
	( SYSTICK->SYST_CSR ) |= ( ( SYSTICK_TIMER_CONFIG.Exception ) << CSR_TICKINT ) ;

	/* Set Clock Source */
	( SYSTICK->SYST_CSR ) &= ~(1<<CSR_CLKSOURCE) ;
	( SYSTICK->SYST_CSR ) |= ( ( SYSTICK_TIMER_CONFIG.CLK ) << CSR_CLKSOURCE ) ;

	/* Enable Timer */
	( SYSTICK->SYST_CSR ) |= ( 1 << CSR_ENABLE ) ;

	/* Check on Flag */
	while( !( ( (SYSTICK->SYST_CSR)>>CSR_COUNTFLAG )&0x01) ) ;

	/* Disable Timer */
	( SYSTICK->SYST_CSR ) &= ~( 1 << CSR_ENABLE ) ;

	return Local_u8ErrorStatus ;
}


/**
 * @fn     : SYSTICK_Delayus
 * @brief  : Fuction That Makes Delay in Software Based on Passed Number of MicroSeconds
 * @param  : Copy_u32TimeInMicroSeconds => Time Passed To Delay in MicroSeconds
 * @retval : ErrorStatus To Indicate If Function Worked Properly
 * @note   : => While Using ( STYSTICK_AHB ) Configuration Max Delay you can make in microseconds is ( 1000000 us = 1s ) if you want more than this value
 *
 *           => While Using ( SYSTICK_AHB_BY8 ) Configuration Max Delay you can make in microseconds is ( 8000000 us = 8s )
 **/
ERRORS_t SYSTICK_Delayus(uint32_t Copy_u32TimeInMicroSeconds)
{
	ERRORS_t Local_u8ErrorStatus = OK ;

	/* Variable To Carry RELOAD Value */
	uint32_t RELOAD_Value = 0 ;

	/* Check on TIMER_CLK  */
	if( SYSTICK_TIMER_CONFIG.CLK == SYSTICK_AHB )
	{
		RELOAD_Value = ( Copy_u32TimeInMicroSeconds  / AHB_TICK_TIME ) ;
	}
	else if( SYSTICK_TIMER_CONFIG.CLK == SYSTICK_AHB_BY8 )
	{
		RELOAD_Value = ( Copy_u32TimeInMicroSeconds  / AHB_BY8_TICK_TIME ) ;
	}

	/* Setting Reload Value */
	( SYSTICK->SYST_RVR ) = RELOAD_Value ;

	/* Clear Current */
	( SYSTICK->SYST_CVR ) = 0 ;

	/* Set Exception */
	( SYSTICK->SYST_CSR ) &= ~(1<<CSR_TICKINT) ;
	( SYSTICK->SYST_CSR ) |= ( ( SYSTICK_TIMER_CONFIG.Exception ) << CSR_TICKINT ) ;

	/* Set Clock Source */
	( SYSTICK->SYST_CSR ) &= ~(1<<CSR_CLKSOURCE) ;
	( SYSTICK->SYST_CSR ) |= ( ( SYSTICK_TIMER_CONFIG.CLK ) << CSR_CLKSOURCE ) ;

	/* Enable Timer */
	( SYSTICK->SYST_CSR ) |= ( 1 << CSR_ENABLE ) ;

	/* Check on Flag */
	while( !( ( (SYSTICK->SYST_CSR)>>CSR_COUNTFLAG )&0x01) ) ;

	/* Disable Timer */
	( SYSTICK->SYST_CSR ) &= ~( 1 << CSR_ENABLE ) ;

	return Local_u8ErrorStatus ;
}
