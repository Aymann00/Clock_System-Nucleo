/*******************************************************************************
 * Filename              :   SYSCFG_Program.c
 * Author                :   Mohamemd Waleed Gad
 * Origin Date           :   Jul 26, 2023
 * Version               :   1.0.0
 * Compiler              :   GCC ARM Embedded Toolchain
 * Target                :
 * Notes                 :   None
 **
 *******************************************************************************/
/************************ SOURCE REVISION LOG *********************************
 *
 *    Date    Version   Author             Description
 *  14/10/20   1.0.0   Mohamemd Waleed   Initial Release.
 *
 *******************************************************************************/
#include <stdint.h>
#include "../../../LIB/STM32F446.h"
#include "../../../LIB/ErrorType.h"
#include "../Include/SYSCFG_Interface.h"
#include "../Include/SYSCFG_Private.h"

/*==============================================================================================================================================
 *@fn SYSFG_voidSetEXTIPort
 *@brief This function is used to set the port of the external interrupt line
 *@paramter[in]  EXTI_t Local_u8Line : the external interrupt line
 *@paramter[in]   GPIO_PORT_t Local_u8Port : the port of the external interrupt line
 *@retval void
 *==============================================================================================================================================*/
void SYSFG_voidSetEXTIPort(EXTI_t Local_u8Line, GPIO_PORT_t Local_u8Port)
{
    /*Variable To Hold Register Index */
    uint8_t Local_u8RegIndex = Local_u8Line / 4;
    /*Variable To Hold Bit Index */
    uint8_t Local_u8BitIndex = (Local_u8Line % 4) * 4;
    /*Clearing The 4 bits Required */
    SYSCFG->EXTICR[Local_u8RegIndex] &= ~(0xF << Local_u8BitIndex);
    /*Setting The 4 bits Required */
    SYSCFG->EXTICR[Local_u8RegIndex] |= (Local_u8Port << Local_u8BitIndex);
}
