/*******************************************************************************
 * Filename              :   SYSCFG_Interface.h
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
#ifndef SYSCFG_INCLUDE_SYSCFG_INTERFACE_H_
#define SYSCFG_INCLUDE_SYSCFG_INTERFACE_H_
/* ======================================================================
 *
 * ====================================================================== */

typedef enum
{
    EXTI_LINE0 = 0,
    EXTI_LINE1,
    EXTI_LINE2,
    EXTI_LINE3,
    EXTI_LINE4,
    EXTI_LINE5,
    EXTI_LINE6,
    EXTI_LINE7,
    EXTI_LINE8,
    EXTI_LINE9,
    EXTI_LINE10,
    EXTI_LINE11,
    EXTI_LINE12,
    EXTI_LINE13,
    EXTI_LINE14,
    EXTI_LINE15
} EXTI_t;

typedef enum
{
    GPIO_PORTA = 0,
    GPIO_PORTB,
    GPIO_PORTC,
    GPIO_PORTD,
    GPIO_PORTE,
    GPIO_PORTF,
    GPIO_PORTG,
    GPIO_PORTH

} GPIO_PORT_t;

/*==============================================================================================================================================
 *@fn SYSFG_voidSetEXTIPort
 *@brief This function is used to set the port of the external interrupt line
 *@paramter[in]  EXTI_t Local_u8Line : the external interrupt line
 *@paramter[in]   GPIO_PORT_t Local_u8Port : the port of the external interrupt line
 *@retval void
 *==============================================================================================================================================*/
void SYSFG_voidSetEXTIPort(EXTI_t Local_u8Line, GPIO_PORT_t Local_u8Port);

#endif /* SYSCFG_INCLUDE_SYSCFG_INTERFACE_H_ */
