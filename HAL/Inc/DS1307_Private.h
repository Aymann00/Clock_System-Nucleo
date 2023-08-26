/*******************************************************************************
 * Filename              :   DS1307_Private.h
 * Author                :   Mohamemd Waleed Gad
 * Origin Date           :   Aug 24, 2023
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
#ifndef DS1307_INCLUDE_DS1307_PRIVATE_H_
#define DS1307_INCLUDE_DS1307_PRIVATE_H_


#define DS1307_SEC_ADD (0x00)   /* Address of seconds register */
#define DS1307_MIN_ADD (0x01)   /* Address of minutes register */
#define DS1307_HR_ADD (0x02)    /* Address of hours register */
#define DS1307_DAY_ADD (0x03)   /* Address of day register */
#define DS1307_DATE_ADD (0x04)  /* Address of date register */
#define DS1307_MONTH_ADD (0x05) /* Address of month register */
#define DS1307_YEAR_ADD (0x06)  /* Address of year register */


#endif /* DS1307_INCLUDE_DS1307_PRIVATE_H_ */
