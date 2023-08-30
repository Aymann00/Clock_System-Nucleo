/*
 ******************************************************************************
 * @file           : Service.h
 * @Author         : Mohammed Ayman Shalaby
 * @brief          : Main program body
 * @Date           : Aug 30, 2023
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 Ayman.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
#ifndef INC_SERVICE_H_
#define INC_SERVICE_H_

void Clock_Init( void ) ;

void Pins_Periph_Init( void ) ;

void Reading_Time(void);

void Transmit_Time(void);

void SPI_CALL_BACK(void);
#endif /* INC_SERVICE_H_ */
