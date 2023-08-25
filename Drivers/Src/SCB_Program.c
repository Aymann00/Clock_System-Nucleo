/*******************************************************************************
 * Filename              :   SCB_Program.c
 * Author                :   Mohamemd Waleed Gad
 * Origin Date           :   Jul 17, 2023
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
#include "../Include/SCB_Interface.h"
#include "../Include/SCB_Private.h"
/*==============================================================================================================================================
 * GLOBAL VARIABLES SECTION
 *==============================================================================================================================================*/

static void (*SCB_PTR_TO_FUNCTION[8])(void) = {NULL};

/*==============================================================================================================================================
 * FUNCTION DEFINITION SECTION
 *==============================================================================================================================================*/
/*==============================================================================================================================================
 *@fn SCB_VoidSetPriorityGroup
 *@brief Setting Priority Group
 *@paramter[in] Local_u8PriorityGroup: Priority Group
 *@retval void
 *==============================================================================================================================================*/
void SCB_VoidSetPriorityGroup(PRIGROUP_t Local_u8PriorityGroup)
{
    /*Setting Priority Group*/
    SCB->AIRCR = Local_u8PriorityGroup;
}
/*==============================================================================================================================================
 *@fn SCB_VoidEnableFaultException
 *@brief Enable Fault Exception
 *@paramter[in] Local_Fault: Fault Type
 *@retval void
 *==============================================================================================================================================*/
void SCB_VoidEnableFaultException(Fault_t Local_Fault)
{
    /*setting Enable Bit For Fault Exception*/
    SCB->SHCSR |= (1 << Local_Fault);
}
/*==============================================================================================================================================
 *@fn SCB_VoidDisableFaultException
 *@brief Disable Fault Exception
 *@paramter[in] Local_Fault: Fault Type
 *@retval void
 *==============================================================================================================================================*/
void SCB_VoidDisableFaultException(Fault_t Local_Fault)
{
    /*Clearing Enable Bit For Fault Exception*/
    SCB->SHCSR &= ~(1 << Local_Fault);
}

uint8_t SCB_u8SetHandler(HANDLER_t Local_Handler, void (*Local_ptrToFunction)(void))
{
    uint8_t Local_u8ErrorState = OK;
    if (Local_Handler < PENDSV_HANDLER && Local_ptrToFunction != NULL)
    {
        SCB_PTR_TO_FUNCTION[Local_Handler] = Local_ptrToFunction;
    }
    else
    {
        Local_u8ErrorState = NOK;
    }
    return Local_u8ErrorState;
}

/*==============================================================================================================================================
 * HANDLERS SECTION
 *==============================================================================================================================================*/

void NMI_Handler(void)
{
    if (SCB_PTR_TO_FUNCTION[NMI_HANDLER] != NULL)
    {
        SCB_PTR_TO_FUNCTION[NMI_HANDLER]();
    }
}

void HardFault_Handler(void)
{
    if (SCB_PTR_TO_FUNCTION[HARDFAULT_HANDLER] != NULL)
    {
        SCB_PTR_TO_FUNCTION[HARDFAULT_HANDLER]();
    }
}

void MemManage_Handler(void)
{
    if (SCB_PTR_TO_FUNCTION[MEMMANAGE_HANDLER] != NULL)
    {
        SCB_PTR_TO_FUNCTION[MEMMANAGE_HANDLER]();
    }
}

void BusFault_Handler(void)
{
    if (SCB_PTR_TO_FUNCTION[BUSFAULT_HANDLER] != NULL)
    {
        SCB_PTR_TO_FUNCTION[BUSFAULT_HANDLER]();
    }
}

void UsageFault_Handler(void)
{
    if (SCB_PTR_TO_FUNCTION[USAGEFAULT_HANDLER] != NULL)
    {
        SCB_PTR_TO_FUNCTION[USAGEFAULT_HANDLER]();
    }
}

void SVC_Handler(void)
{
    if (SCB_PTR_TO_FUNCTION[SVCALL_HANDLER] != NULL)
    {
        SCB_PTR_TO_FUNCTION[SVCALL_HANDLER]();
    }
}

void DebugMon_Handler(void)
{
    if (SCB_PTR_TO_FUNCTION[DEBUGMONITOR_HANDLER] != NULL)
    {
        SCB_PTR_TO_FUNCTION[DEBUGMONITOR_HANDLER]();
    }
}

void PendSV_Handler(void)
{
    if (SCB_PTR_TO_FUNCTION[PENDSV_HANDLER] != NULL)
    {
        SCB_PTR_TO_FUNCTION[PENDSV_HANDLER]();
    }
}
