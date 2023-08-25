/*******************************************************************************
 * Filename              :   SCB_Interface.h
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
#ifndef SCB_INCLUDE_SCB_INTERFACE_H_
#define SCB_INCLUDE_SCB_INTERFACE_H_
typedef enum
{
    GP_16_SP_0 = 0x05FA0000UL, // 16 Group of Priority & 0 Sub Group of Priority
    GP_8_SP_2 = 0x05FA0400UL,  // 8 Group of Priority & 2 Sub Group of Priority
    GP_4_SP_4 = 0x05FA0500UL,  // 4 Group of Priority & 4 Sub Group of Priority
    GP_2_SP_8 = 0x05FA0600UL,  // 2 Group of Priority & 8 Sub Group of Priority
    GP_0_SP_16 = 0x05FA0700UL, // 0 Group of Priority & 16 Sub Group of Priority
} PRIGROUP_t;
typedef enum
{
    MEMFAULT = 16,
    BUSFAULT,
    USAGEFAULT
} Fault_t;
typedef enum
{
    NMI_HANDLER = 0,
    HARDFAULT_HANDLER,
    MEMMANAGE_HANDLER,
    BUSFAULT_HANDLER,
    USAGEFAULT_HANDLER,
    SVCALL_HANDLER,
    DEBUGMONITOR_HANDLER,
    PENDSV_HANDLER
} HANDLER_t;
/*==============================================================================================================================================
 *@fn SCB_VoidSetPriorityGroup
 *@brief Setting Priority Group
 *@paramter[in] Local_u8PriorityGroup: Priority Group
 *@retval void
 *==============================================================================================================================================*/
void SCB_VoidSetPriorityGroup(PRIGROUP_t Local_u8PriorityGroup);
/*==============================================================================================================================================
 *@fn SCB_VoidEnableFaultException
 *@brief Enable Fault Exception
 *@paramter[in] Local_Fault: Fault Type
 *@retval void
 *==============================================================================================================================================*/
void SCB_VoidEnableFaultException(Fault_t Local_Fault);
/*==============================================================================================================================================
 *@fn SCB_VoidDisableFaultException
 *@brief Disable Fault Exception
 *@paramter[in] Local_Fault: Fault Type
 *@retval void
 *==============================================================================================================================================*/
void SCB_VoidDisableFaultException(Fault_t Local_Fault);
#endif /* SCB_INCLUDE_SCB_INTERFACE_H_ */
