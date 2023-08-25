/*
 ******************************************************************************
 * @file           : NVIC_Program.c
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

/*==============================================================================================================================================
 * INCLUDES SECTION START
 *==============================================================================================================================================*/
#include <stdint.h>
#include "../../Library/STM32F446xx.h"
#include "../../Library/ErrTypes.h"

#include "../Inc/NVIC_Interface.h"
#include "../Inc/NVIC_Private.h"

/*==============================================================================================================================================
 * INCLUDES SECTION END
 *==============================================================================================================================================*/



/*==============================================================================================================================================
 * MODULES IMPLEMENTATION
 *==============================================================================================================================================*/

/**
 * @fn    : NVIC_EnableIRQ
 * @brief : This Function Takes IRQ Number and Enables Peripheral From NVIC
 * @param : IRQ_Number -> From enum ( @IRQNum_t )
 * @retval: ErrorStatus to Indicate if Function Worked Correct or Not
 * @note  : Provide IRQ From the Specified in the enum only
 */
Error_State_t NVIC_EnableIRQ( IRQNum_t IRQ_Number )
{
	Error_State_t Local_u8ErrorStatus = OK ;

	/* Variable to Holds Reg Number For the Specified IRQn */
	uint8_t REG_Number = ( IRQ_Number/32 ) ;

	/* Variable to Hold Bits Start For the Specified IRQn */
	uint8_t BITS_Start = ( IRQ_Number%32 ) ;

	if( IRQ_Number >= WWDG_IRQ && IRQ_Number < NUMBER_OF_IRQs )
	{
		NVIC->ISER[ REG_Number ] = ( 1 << BITS_Start ) ;
	}
	else
	{
		Local_u8ErrorStatus = NOK ;
	}
	return Local_u8ErrorStatus ;
}



/**
 * @fn    : NVIC_DisableIRQ
 * @brief : This Function Takes IRQ Number and Disables Peripheral From NVIC
 * @param : IRQ_Number -> From enum ( @IRQNum_t )
 * @retval: ErrorStatus to Indicate if Function Worked Correct or Not
 * @note  : Provide IRQ From the Specified in the enum only
 */
Error_State_t NVIC_DisableIRQ( IRQNum_t IRQ_Number )
{
	Error_State_t Local_u8ErrorStatus = OK ;

	/* Variable to Holds Reg Number For the Specified IRQn */
	uint8_t REG_Number = ( IRQ_Number/32 ) ;

	/* Variable to Hold Bits Start For the Specified IRQn */
	uint8_t BITS_Start = ( IRQ_Number%32 ) ;

	if( IRQ_Number >= WWDG_IRQ && IRQ_Number < NUMBER_OF_IRQs )
	{
		NVIC->ICER[ REG_Number ] = ( 1 << BITS_Start ) ;
	}
	else
	{
		Local_u8ErrorStatus = NOK ;
	}
	return Local_u8ErrorStatus ;
}


/**
 * @fn    : NVIC_SetPendingIRQ
 * @brief : This Function Takes IRQ Number and Sets the Pending Flag of the Specified IRQ
 * @param : IRQ_Number -> From enum ( @IRQNum_t )
 * @retval: ErrorStatus to Indicate if Function Worked Correct or Not
 * @note  : Provide IRQ From the Specified in the enum only
 */
Error_State_t NVIC_SetPendingIRQ( IRQNum_t IRQ_Number )
{
	Error_State_t Local_u8ErrorStatus = OK ;

	/* Variable to Holds Reg Number For the Specified IRQn */
	uint8_t REG_Number = ( IRQ_Number / 32 ) ;

	/* Variable to Hold Bits Start For the Specified IRQn */
	uint8_t BITS_Start = ( IRQ_Number % 32 ) ;

	if( IRQ_Number >= WWDG_IRQ && IRQ_Number < NUMBER_OF_IRQs )
	{
		NVIC->ISPR[ REG_Number ] = ( 1 << BITS_Start ) ;
	}
	else
	{
		Local_u8ErrorStatus = NOK ;
	}
	return Local_u8ErrorStatus ;
}


/**
 * @fn    : NVIC_ClearPendingIRQ
 * @brief : This Function Takes IRQ Number and Clears the Pending Flag of the Specified IRQ
 * @param : IRQ_Number -> From enum ( @IRQNum_t )
 * @retval: ErrorStatus to Indicate if Function Worked Correct or Not
 * @note  : Provide IRQ From the Specified in the enum only
 */
Error_State_t NVIC_ClearPendingIRQ( IRQNum_t IRQ_Number )
{
	Error_State_t Local_u8ErrorStatus = OK ;

	/* Variable to Holds Reg Number For the Specified IRQn */
	uint8_t REG_Number = ( IRQ_Number/32 ) ;

	/* Variable to Hold Bits Start For the Specified IRQn */
	uint8_t BITS_Start = ( IRQ_Number%32 ) ;

	if( (IRQ_Number >= WWDG_IRQ) && (IRQ_Number < NUMBER_OF_IRQs) )
	{
		NVIC->ICPR[ REG_Number ] = ( 1 << BITS_Start ) ;
	}
	else
	{
		Local_u8ErrorStatus = NOK ;
	}
	return Local_u8ErrorStatus ;
}


/**
 * @fn     : NVIC_GetPendingIRQ
 * @brief  : This Function Take IRQ Number and a Pointer to dereference the pointer with the pending state
 * @param  : IRQ_Number -> From enum ( @IRQNum_t )
 * @param  : Pending_State -> A Pointer to a Variable that User Takes From it the Pending State ( Pending_t )
 * @retval : ErrorStatus to Indicate if Function Worked Correctly or Not
 */
Error_State_t NVIC_GetPendingIRQ( IRQNum_t IRQ_Number , PENDING_t * Pending_State  )
{
	Error_State_t Local_u8ErrorStatus = OK ;

	/* Variable to Holds Reg Number For the Specified IRQn */
	uint8_t REG_Number = ( IRQ_Number/32 ) ;

	/* Variable to Hold Bits Start For the Specified IRQn */
	uint8_t BITS_Start = ( IRQ_Number%32 ) ;

	if(  NULL != Pending_State )
	{
		if( (IRQ_Number >= WWDG_IRQ) && (IRQ_Number < NUMBER_OF_IRQs)  )
		{
			if( ( ( NVIC->ISPR[ REG_Number ] >> ( BITS_Start ) ) & 0x01 ) )
			{
				*Pending_State = PENDING ;
			}
			else
			{
				*Pending_State = NOT_PENDING ;
			}
		}
		else
		{
			Local_u8ErrorStatus = NOK ;
		}
	}
	else
	{
		Local_u8ErrorStatus = NULL_POINTER ;
	}
	return Local_u8ErrorStatus ;
}



/**
 * @fn     : NVIC_GetActiveIRQ
 * @brief  : This Function Take IRQ Number and a Pointer to dereference the pointer with the Active state
 * @param  : IRQ_Number -> From enum ( @IRQNum_t )
 * @param  : Active_State -> A Pointer to a Variable that User Takes From it the Active State ( Active_t )
 * @retval : ErrorStatus to Indicate if Function Worked Correctly or Not
 */
Error_State_t NVIC_GetActiveIRQ( IRQNum_t IRQ_Number , ACTIVE_t * Active_State )
{
	Error_State_t Local_u8ErrorStatus = OK ;

	/* Variable to Holds Reg Number For the Specified IRQn */
	uint8_t REG_Number = ( IRQ_Number/32 ) ;

	/* Variable to Hold Bits Start For the Specified IRQn */
	uint8_t BITS_Start = ( IRQ_Number%32 ) ;

	if( NULL !=  Active_State )
	{
		if( (IRQ_Number >= WWDG_IRQ) && (IRQ_Number < NUMBER_OF_IRQs)  )
		{
			if( ( ( NVIC->IABR[ REG_Number ] >> BITS_Start ) & 0x01 ) )
			{
				*Active_State = ACTIVE ;
			}
			else
			{
				*Active_State = NOT_ACTIVE ;
			}
		}
		else
		{
			Local_u8ErrorStatus = NOK ;
		}
	}
	else
	{
		Local_u8ErrorStatus = NULL_POINTER ;
	}
	return Local_u8ErrorStatus ;
}


/**
 * @fn     : NVIC_SetPriority
 * @brief  : This Function Take IRQ Number and a Priority and assigns the priority to the associated IRQ
 * @param  : IRQ_Number -> From enum ( @IRQNum_t )
 * @param  : Copy_u8Priority -> Priority of the Specified IRQ
 * @retval : ErrorStatus to Indicate if Function Worked Correctly or Not
 */
Error_State_t NVIC_SetPriority( IRQNum_t IRQ_Number , uint8_t  Copy_u8Priority )
{
	Error_State_t Local_u8ErrorStatus = OK ;

	if( (IRQ_Number >= WWDG_IRQ) && (IRQ_Number  < NUMBER_OF_IRQs) )
	{
		/* BIT MASKING */
		NVIC->IPR[IRQ_Number] &= ( ~ ( PRIORITY_REG_MASK ) ) ;
		NVIC->IPR[IRQ_Number] = ( Copy_u8Priority << 4 ) ;
	}
	else
	{
		Local_u8ErrorStatus = NOK ;
	}
	return Local_u8ErrorStatus ;
}
