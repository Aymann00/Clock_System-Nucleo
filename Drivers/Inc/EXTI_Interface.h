/*******************************************************************************
 * Filename              :   EXTI_Interface.h
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
#ifndef EXTI_INCLUDE_EXTI_INTERFACE_H_
#define EXTI_INCLUDE_EXTI_INTERFACE_H_
/*==============================================================================================================================================
 * ENUMS DEFINITION SECTION
 *==============================================================================================================================================*/
/*==============================================================================================================================================
 * @enum TRIG_SELECT_t
 * @brief This enum is used to select the trigger type of the EXTI peripheral
 *==============================================================================================================================================*/
typedef enum
{
	RISING_TRG = 0, /*!< EXTI Trigger Rising Edge */
	FALLING_TRG,    /*!< EXTI Trigger Falling Edge */
	ON_CHANGE_TRG   /*!< EXTI Trigger On Change */

} TRIG_SELECT_t;
/*==============================================================================================================================================
 * @enum EXTI_PEND_t
 * @brief This enum is used to select the pending flag status of the EXTI peripheral
 *==============================================================================================================================================*/
typedef enum
{

	NOT_PENDED = 0, /*!< EXTI Line Not Pending */
	PENDED          /*!< EXTI Line Pending */

} EXTI_PEND_t;
/*==============================================================================================================================================
 * @enum EXTI_STATE_t
 * @brief This enum is used to select the state of the EXTI peripheral
 *==============================================================================================================================================*/
typedef enum
{
	EXTI_DISABLE = 0, /*!< EXTI Line Disabled */
	EXTI_ENABLE       /*!< EXTI Line Enabled */

} EXTI_STATE_t;
/*==============================================================================================================================================
 * @enum EXTI_LINE_t
 * @brief This enum is used to select the EXTI line number
 *==============================================================================================================================================*/
typedef enum
{
	EXTI0 = 0,   /*!< EXTI Line0 */
	EXTI1,       /*!< EXTI Line1 */
	EXTI2,       /*!< EXTI Line2 */
	EXTI3,       /*!< EXTI Line3 */
	EXTI4,       /*!< EXTI Line4 */
	EXTI5,       /*!< EXTI Line5 */
	EXTI6,       /*!< EXTI Line6 */
	EXTI7,       /*!< EXTI Line7 */
	EXTI8,       /*!< EXTI Line8 */
	EXTI9,       /*!< EXTI Line9 */
	EXTI10,      /*!< EXTI Line10 */
	EXTI11,      /*!< EXTI Line11 */
	EXTI12,      /*!< EXTI Line12 */
	EXTI13,      /*!< EXTI Line13 */
	EXTI14,      /*!< EXTI Line14 */
	EXTI15,      /*!< EXTI Line15 */
	EXTI16,      /*!< EXTI Line16 */
	EXTI17,      /*!< EXTI Line17 */
	EXTI18,      /*!< EXTI Line18 */
	EXTI20 = 20, /*!< EXTI Line20 */
	EXTI21,      /*!< EXTI Line21 */
	EXTI22       /*!< EXTI Line22 */
} EXTI_LINE_t;
/*==============================================================================================================================================
 * STRUCTURE DEFINITION SECTION
 *==============================================================================================================================================*/
/*==============================================================================================================================================
 * @struct EXTI_CONFG_t
 * @brief This structure is used to set the configuration of the EXTI peripheral
 *==============================================================================================================================================*/
typedef struct
{
	EXTI_LINE_t EXTILine;           /*!< EXTI Line Number */
	TRIG_SELECT_t TriggerSelection; /*!< EXTI Trigger Selection */
	EXTI_STATE_t EXTIStatus;        /*!< EXTI Line Status */
	void (*Copy_PtrFuncEXTI)(void); /*!< Pointer to function to be executed when EXTI interrupt occurs */
} EXTI_CONFG_t;
/*==============================================================================================================================================
 *@fn void EXTI_voidInit(EXTI_CONFG_t *Local_PtrEXTIConfg)
 *@brief This function is used to initialize the EXTI peripheral
 *@paramter[in] *Local_PtrEXTIConfg: pointer to structure of type EXTI_CONFG_t
 *@retval void
 *==============================================================================================================================================*/
void EXTI_voidInit(EXTI_CONFG_t *Local_PtrEXTIConfg);
/*==============================================================================================================================================
 *@fn void EXTI_voidEnableEXTI(EXTI_LINE_t Local_u8Line)
 *@brief This function is used to enable the EXTI peripheral
 *@paramter[in] Local_u8Line: EXTI line number
 *@retval void
 *==============================================================================================================================================*/
void EXTI_voidEnableEXTI(EXTI_LINE_t Local_u8Line);
/*==============================================================================================================================================
 *@fn void EXTI_voidDisableEXTI(EXTI_LINE_t Local_u8Line)
 *@brief This function is used to disable the EXTI peripheral
 *@paramter[in] Local_u8Line: EXTI line number
 *@retval void
 *==============================================================================================================================================*/
void EXTI_voidDisableEXTI(EXTI_LINE_t Local_u8Line);
/*==============================================================================================================================================
 *@fn void EXTI_voidSetPendingFlag(EXTI_LINE_t Local_u8Line)
 *@brief This function is used to set the pending flag of the EXTI peripheral
 *@paramter[in] Local_u8Line: EXTI line number
 *@retval void
 *==============================================================================================================================================*/
void EXTI_voidClearPendingFlag(EXTI_LINE_t Local_u8Line);
/*==============================================================================================================================================
 *@fn  void EXTI_voidReadPendingFlag(EXTI_LINE_t Local_u8Line, EXTI_PEND_t * EXTIStatus )
 *@brief This function is used to read the pending flag of the EXTI peripheral
 *@paramter[in] Local_u8Line: EXTI line number
 *@paramter[out] *EXTIStatus: pointer to variable of type EXTI_PEND_t
 *@retval void
 *==============================================================================================================================================*/
void EXTI_voidReadPendingFlag(EXTI_LINE_t Local_u8Line, EXTI_PEND_t *EXTIStatus);
/*==============================================================================================================================================
 *@fn void EXTI_voidSetSoftwareTrigger(EXTI_LINE_t Local_u8Line)
 *@brief This function is used to set the software trigger of the EXTI peripheral
 *@paramter[in] Local_u8Line: EXTI line number
 *@retval void
 *==============================================================================================================================================*/
void EXTI_voidSetSoftwareTrigger(EXTI_LINE_t Local_u8Line);
/*==============================================================================================================================================
 *@fn void EXTI_voidSetTriggerSelection(EXTI_LINE_t Local_u8Line,TRIG_SELECT_t Local_u8TriggerSelection)
 *@brief This function is used to set the trigger selection of the EXTI peripheral
 *@paramter[in] Local_u8Line: EXTI line number
 *@paramter[in] Local_u8TriggerSelection: trigger selection
 *@retval void
 *==============================================================================================================================================*/
void EXTI_voidSetTriggerSelection(EXTI_LINE_t Local_u8Line, TRIG_SELECT_t Local_u8TriggerSelection);
/*==============================================================================================================================================
 *@fn void EXTI_voidSetCallBack(EXTI_LINE_t Local_u8Line,void (*Local_PtrFunc)(void))
 *@brief This function is used to set the callback function of the EXTI peripheral
 *@paramter[in] Local_u8Line: EXTI line number
 *@paramter[in] Local_PtrFunc: pointer to function to be executed when EXTI interrupt occurs
 *@retval void
 *==============================================================================================================================================*/
void EXTI_voidSetCallBack(EXTI_LINE_t Local_u8Line, void (*Local_PtrFunc)(void));
#endif /* EXTI_INCLUDE_EXTI_INTERFACE_H_ */
