/*******************************************************************************
 * Filename              :   EXTI_Program.c
 * Author                :   Mohamemd Waleed Gad
 * Origin Date           :   Jul 27, 2023
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
#include "../Include/EXTI_Interface.h"
#include "../Include/EXTI_Private.h"

static void (*EXTI_PTR_TO_FUNCTION[23])(void) = {NULL};
/*==============================================================================================================================================
 * Function Definitions
 *==============================================================================================================================================*/
/*==============================================================================================================================================
 *@fn void EXTI_voidInit(EXTI_CONFG_t *Local_PtrEXTIConfg)
 *@brief This function is used to initialize the EXTI peripheral
 *@paramter[in] *Local_PtrEXTIConfg: pointer to structure of type EXTI_CONFG_t
 *@retval void
 *==============================================================================================================================================*/
void EXTI_voidInit(EXTI_CONFG_t *Local_PtrEXTIConfg)
{
    /* Set Trigger Edge Detection */
    EXTI_voidSetTriggerSelection(Local_PtrEXTIConfg->EXTILine, Local_PtrEXTIConfg->TriggerSelection);
    if (Local_PtrEXTIConfg->EXTIStatus == EXTI_ENABLE)
    {
        /* Enable EXTI Line */
        EXTI_voidEnableEXTI(Local_PtrEXTIConfg->EXTILine);
    }
    else if (Local_PtrEXTIConfg->EXTIStatus == EXTI_DISABLE)
    {
        /* Disable EXTI Line */
        EXTI_voidDisableEXTI(Local_PtrEXTIConfg->EXTILine);
    }
    /* Initialize Call Back Function */
    EXTI_voidSetCallBack(Local_PtrEXTIConfg->EXTILine, Local_PtrEXTIConfg->Copy_PtrFuncEXTI);
}
/*==============================================================================================================================================
 *@fn void EXTI_voidEnableEXTI(EXTI_LINE_t Local_u8Line)
 *@brief This function is used to enable the EXTI peripheral
 *@paramter[in] Local_u8Line: EXTI line number
 *@retval void
 *==============================================================================================================================================*/
void EXTI_voidEnableEXTI(EXTI_LINE_t Local_u8Line)
{
    /* Enable EXTI Line */
    EXTI->IMR |= (1 << Local_u8Line);
}
/*==============================================================================================================================================
 *@fn void EXTI_voidDisableEXTI(EXTI_LINE_t Local_u8Line)
 *@brief This function is used to disable the EXTI peripheral
 *@paramter[in] Local_u8Line: EXTI line number
 *@retval void
 *==============================================================================================================================================*/
void EXTI_voidDisableEXTI(EXTI_LINE_t Local_u8Line)
{
    /* Disable EXTI Line */
    EXTI->IMR &= ~(1 << Local_u8Line);
}
/*==============================================================================================================================================
 *@fn void EXTI_voidSetPendingFlag(EXTI_LINE_t Local_u8Line)
 *@brief This function is used to set the pending flag of the EXTI peripheral
 *@paramter[in] Local_u8Line: EXTI line number
 *@retval void
 *==============================================================================================================================================*/
void EXTI_voidClearPendingFlag(EXTI_LINE_t Local_u8Line)
{
    /* Clear Pending Flag */
    EXTI->PR |= (1 << Local_u8Line);
}
/*==============================================================================================================================================
 *@fn  void EXTI_voidReadPendingFlag(EXTI_LINE_t Local_u8Line, EXTI_PEND_t * EXTIStatus )
 *@brief This function is used to read the pending flag of the EXTI peripheral
 *@paramter[in] Local_u8Line: EXTI line number
 *@paramter[out] *EXTIStatus: pointer to variable of type EXTI_PEND_t
 *@retval void
 *==============================================================================================================================================*/
void EXTI_voidReadPendingFlag(EXTI_LINE_t Local_u8Line, EXTI_PEND_t *EXTIStatus)
{
    /* Read Pending Flag */
    *EXTIStatus = (((EXTI->PR) >> Local_u8Line) & 0x01);
}
/*==============================================================================================================================================
 *@fn void EXTI_voidSetSoftwareTrigger(EXTI_LINE_t Local_u8Line)
 *@brief This function is used to set the software trigger of the EXTI peripheral
 *@paramter[in] Local_u8Line: EXTI line number
 *@retval void
 *==============================================================================================================================================*/
void EXTI_voidSetSoftwareTrigger(EXTI_LINE_t Local_u8Line)
{
    /* Set Software Trigger */
    EXTI->SWIER |= (1 << Local_u8Line);
}
/*==============================================================================================================================================
 *@fn void EXTI_voidSetTriggerSelection(EXTI_LINE_t Local_u8Line,TRIG_SELECT_t Local_u8TriggerSelection)
 *@brief This function is used to set the trigger selection of the EXTI peripheral
 *@paramter[in] Local_u8Line: EXTI line number
 *@paramter[in] Local_u8TriggerSelection: trigger selection
 *@retval void
 *==============================================================================================================================================*/
void EXTI_voidSetTriggerSelection(EXTI_LINE_t Local_u8Line, TRIG_SELECT_t Local_u8TriggerSelection)
{
    /* Set Trigger Edge Detection */
    if (Local_u8TriggerSelection == RISING_TRG)
    {
        /* Set Rising Trigger Selection */
        EXTI->RTSR |= (1 << Local_u8Line);
    }
    else if (Local_u8TriggerSelection == FALLING_TRG)
    {
        /* Set Falling Trigger Selection */
        EXTI->FTSR |= (1 << Local_u8Line);
    }
    else if (Local_u8TriggerSelection == ON_CHANGE_TRG)
    {
        /* Set Both Rising And Falling Trigger Selection */
        EXTI->RTSR |= (1 << Local_u8Line);
        EXTI->FTSR |= (1 << Local_u8Line);
    }
}
/*==============================================================================================================================================
 *@fn void EXTI_voidSetCallBack(EXTI_LINE_t Local_u8Line,void (*Local_PtrFunc)(void))
 *@brief This function is used to set the callback function of the EXTI peripheral
 *@paramter[in] Local_u8Line: EXTI line number
 *@paramter[in] Local_PtrFunc: pointer to function to be executed when EXTI interrupt occurs
 *@retval void
 *==============================================================================================================================================*/
void EXTI_voidSetCallBack(EXTI_LINE_t Local_u8Line, void (*Local_PtrFunc)(void))
{
    /* Set Call Back Function */
    EXTI_PTR_TO_FUNCTION[Local_u8Line] = Local_PtrFunc;
}

/*==============================================================================================================================================
 * HANDLERS SECTION
 *==============================================================================================================================================*/

void EXTI0_IRQHandler(void)
{
    /* Clear Pending Flag */
    EXTI_voidClearPendingFlag(EXTI0);

    if (EXTI_PTR_TO_FUNCTION[EXTI0] != NULL)
    {
        EXTI_PTR_TO_FUNCTION[EXTI0]();
    }
}

void EXTI1_IRQHandler(void)
{
    /* Clear Pending Flag */
    EXTI_voidClearPendingFlag(EXTI1);

    if (EXTI_PTR_TO_FUNCTION[EXTI1] != NULL)
    {
        EXTI_PTR_TO_FUNCTION[EXTI1]();
    }
}

void EXTI2_IRQHandler(void)
{
    /* Clear Pending Flag */
    EXTI_voidClearPendingFlag(EXTI2);

    if (EXTI_PTR_TO_FUNCTION[EXTI2] != NULL)
    {
        EXTI_PTR_TO_FUNCTION[EXTI2]();
    }
}

void EXTI3_IRQHandler(void)
{
    /* Clear Pending Flag */
    EXTI_voidClearPendingFlag(EXTI3);

    if (EXTI_PTR_TO_FUNCTION[EXTI3] != NULL)
    {
        EXTI_PTR_TO_FUNCTION[EXTI3]();
    }
}

void EXTI4_IRQHandler(void)
{
    /* Clear Pending Flag */
    EXTI_voidClearPendingFlag(EXTI4);

    if (EXTI_PTR_TO_FUNCTION[EXTI4] != NULL)
    {
        EXTI_PTR_TO_FUNCTION[EXTI4]();
    }
}

void EXTI9_5_IRQHandler(void)
{
    /* Variable To Hold Pending Status */
    EXTI_PEND_t PEND_Status = NOT_PENDED;

    /* Read Pending */
    EXTI_voidReadPendingFlag(EXTI5, &PEND_Status);

    if (PEND_Status == PENDED)
    {
        /* Clear Pending Flag */
        EXTI_voidClearPendingFlag(EXTI5);

        /* Set PTR To Function */
        if (NULL != EXTI_PTR_TO_FUNCTION[EXTI5])
        {
            EXTI_PTR_TO_FUNCTION[EXTI5]();
        }
    }

    /* Read Pending */
    EXTI_voidReadPendingFlag(EXTI6, &PEND_Status);

    if (PEND_Status == PENDED)
    {
        /* Clear Pending Flag */
        EXTI_voidClearPendingFlag(EXTI6);

        /* Set PTR To Function */
        if (NULL != EXTI_PTR_TO_FUNCTION[EXTI6])
        {
            EXTI_PTR_TO_FUNCTION[EXTI6]();
        }
    }

    /* Read Pending */
    EXTI_voidReadPendingFlag(EXTI7, &PEND_Status);

    if (PEND_Status == PENDED)
    {
        /* Clear Pending Flag */
        EXTI_voidClearPendingFlag(EXTI7);

        /* Set PTR To Function */
        if (NULL != EXTI_PTR_TO_FUNCTION[EXTI7])
        {
            EXTI_PTR_TO_FUNCTION[EXTI7]();
        }
    }

    /* Read Pending */
    EXTI_voidReadPendingFlag(EXTI8, &PEND_Status);

    if (PEND_Status == PENDED)
    {
        /* Clear Pending Flag */
        EXTI_voidClearPendingFlag(EXTI8);

        /* Set PTR To Function */
        if (NULL != EXTI_PTR_TO_FUNCTION[EXTI8])
        {
            EXTI_PTR_TO_FUNCTION[EXTI8]();
        }
    }

    /* Read Pending */
    EXTI_voidReadPendingFlag(EXTI9, &PEND_Status);

    if (PEND_Status == PENDED)
    {
        /* Clear Pending Flag */
        EXTI_voidClearPendingFlag(EXTI9);

        /* Set PTR To Function */
        if (NULL != EXTI_PTR_TO_FUNCTION[EXTI9])
        {
            EXTI_PTR_TO_FUNCTION[EXTI9]();
        }
    }
}

void EXTI15_10_IRQHandler(void)
{
    EXTI_PEND_t PEND_Status = NOT_PENDED;

    /* Read Pending */
    EXTI_voidReadPendingFlag(EXTI10, &PEND_Status);

    if (PEND_Status == PENDED)
    {
        /* Clear Pending Flag */
        EXTI_voidClearPendingFlag(EXTI10);

        /* Set PTR To Function */
        if (NULL != EXTI_PTR_TO_FUNCTION[EXTI10])
        {
            EXTI_PTR_TO_FUNCTION[EXTI10]();
        }
    }

    /* Read Pending */
    EXTI_voidReadPendingFlag(EXTI11, &PEND_Status);

    if (PEND_Status == PENDED)
    {
        /* Clear Pending Flag */
        EXTI_voidClearPendingFlag(EXTI11);

        /* Set PTR To Function */
        if (NULL != EXTI_PTR_TO_FUNCTION[EXTI11])
        {
            EXTI_PTR_TO_FUNCTION[EXTI11]();
        }
    }

    /* Read Pending */
    EXTI_voidReadPendingFlag(EXTI12, &PEND_Status);

    if (PEND_Status == PENDED)
    {
        /* Clear Pending Flag */
        EXTI_voidClearPendingFlag(EXTI12);

        /* Set PTR To Function */
        if (NULL != EXTI_PTR_TO_FUNCTION[EXTI12])
        {
            EXTI_PTR_TO_FUNCTION[EXTI12]();
        }
    }

    /* Read Pending */
    EXTI_voidReadPendingFlag(EXTI13, &PEND_Status);

    if (PEND_Status == PENDED)
    {
        /* Clear Pending Flag */
        EXTI_voidClearPendingFlag(EXTI13);

        /* Set PTR To Function */
        if (NULL != EXTI_PTR_TO_FUNCTION[EXTI13])
        {
            EXTI_PTR_TO_FUNCTION[EXTI13]();
        }
    }

    /* Read Pending */
    EXTI_voidReadPendingFlag(EXTI14, &PEND_Status);

    if (PEND_Status == PENDED)
    {
        /* Clear Pending Flag */
        EXTI_voidClearPendingFlag(EXTI14);

        /* Set PTR To Function */
        if (NULL != EXTI_PTR_TO_FUNCTION[EXTI14])
        {
            EXTI_PTR_TO_FUNCTION[EXTI14]();
        }
    }

    /* Read Pending */
    EXTI_voidReadPendingFlag(EXTI15, &PEND_Status);

    if (PEND_Status == PENDED)
    {
        /* Clear Pending Flag */
        EXTI_voidClearPendingFlag(EXTI15);

        /* Set PTR To Function */
        if (NULL != EXTI_PTR_TO_FUNCTION[EXTI15])
        {
            EXTI_PTR_TO_FUNCTION[EXTI15]();
        }
    }
}
/* EXTI16 */
void PVD_IRQHandler(void)
{
    /* Clear Pending Flag */
    EXTI_voidClearPendingFlag(EXTI16);

    /* Set PTR To Function */
    if (NULL != EXTI_PTR_TO_FUNCTION[EXTI16])
    {
        EXTI_PTR_TO_FUNCTION[EXTI16]();
    }
}
/* EXTI17 */
void RTC_Alarm_IRQHandler(void)
{
    /* Clear Pending Flag */
    EXTI_voidClearPendingFlag(EXTI17);

    /* Set PTR To Function */
    if (NULL != EXTI_PTR_TO_FUNCTION[EXTI17])
    {
        EXTI_PTR_TO_FUNCTION[EXTI17]();
    }
}
/* EXTI18 */
void OTG_FS_WKUP_IRQHandler(void)
{
    /* Clear Pending Flag */
    EXTI_voidClearPendingFlag(EXTI18);

    /* Set PTR To Function */
    if (NULL != EXTI_PTR_TO_FUNCTION[EXTI18])
    {
        EXTI_PTR_TO_FUNCTION[EXTI18]();
    }
}
/* EXTI20 */
void OTG_HS_WKUP_IRQHandler(void)
{
    /* Clear Pending Flag */
    EXTI_voidClearPendingFlag(EXTI20);

    /* Set PTR To Function */
    if (NULL != EXTI_PTR_TO_FUNCTION[EXTI20])
    {
        EXTI_PTR_TO_FUNCTION[EXTI20]();
    }
}
/* EXTI21 */
void TAMP_STAMP_IRQHandler(void)
{
    /* Clear Pending Flag */
    EXTI_voidClearPendingFlag(EXTI21);

    /* Set PTR To Function */
    if (NULL != EXTI_PTR_TO_FUNCTION[EXTI21])
    {
        EXTI_PTR_TO_FUNCTION[EXTI21]();
    }
}
/* EXTI22 */
void RTC_WKUP_IRQHandler(void)
{
    /* Clear Pending Flag */
    EXTI_voidClearPendingFlag(EXTI22);

    /* Set PTR To Function */
    if (NULL != EXTI_PTR_TO_FUNCTION[EXTI22])
    {
        EXTI_PTR_TO_FUNCTION[EXTI22]();
    }
}
