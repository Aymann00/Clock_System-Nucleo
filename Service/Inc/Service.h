/*
 * Service.h
 *
 *  Created on: Aug 28, 2023
 *      Author: hp
 */

#ifndef INC_SERVICE_H_
#define INC_SERVICE_H_


/* ========================================================================= *
 *                         PRIVATE MACROS SECTION                            *
 * ========================================================================= */

#define WRONG_DAY				8u
#define FIRST_TIME				1u
#define NOT_FIRST_TIME			0u
#define CALENDER_FORMAT			23u

#define FIRST_LETTER_OF_DAY		10u
#define SECOND_LETTER_OF_DAY	11u
#define THIRD_LETTER_OF_DAY		12u

#define ZERO_ASCII				48u

#define MAX_DATE				31u
#define MAX_HOURS				23u
#define MAX_MINUTES				59u
#define MAX_SECONDS				59u
#define MAX_MONTH				12u
#define MAX_YEAR				99u



/* ========================================================================= *
 *                      FUNCTIONS PROTOTYPES SECTION                         *
 * ========================================================================= */

void Clock_Init(void);

void Pins_Init(void);

void Peripherals_Init(void);

Error_State_t ReadDateTime_FromPC(void) ;

static DS1307_DAYS_t FindDay(uint8_t * Calender);

static void Calculate_Calender(DS1307_Config_t * Date_Time_To_RTC,uint8_t * Date_Time_From_USART);

static Error_State_t Check_Calender(DS1307_Config_t * Date_Time_To_RTC);


#endif /* INC_SERVICE_H_ */
