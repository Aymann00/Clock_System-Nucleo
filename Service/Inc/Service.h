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

#define WRONG_DAY		8
#define FIRST_TIME		1
#define NOT_FIRST_TIME	0

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
