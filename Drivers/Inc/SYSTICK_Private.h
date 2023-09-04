/*
 ******************************************************************************
 * @file           : SYSTICK_Private.h
 * @Author         : Mohammed Ayman Shalaby
 * @brief          : Private File For Macros
 * @Date           : May 12, 2023
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
#ifndef SYSTICK_INC_SYSTICK_PRIVATE_H_
#define SYSTICK_INC_SYSTICK_PRIVATE_H_

#define AHB_TICK_TIME     0.0625f
#define AHB_BY8_TICK_TIME 0.5f

#define 	ENABLE_MASK				0b1
#define 	ENABLE_BIT_ACCESS		0

#define 	INTERRUPT_MASK			0b1
#define 	INTERRUPT_BIT_ACCESS	1u

#define		CLKSRC_MASK				0b1
#define 	CLKSRC_BIT_ACCESS		2u

#endif /* SYSTICK_INC_SYSTICK_PRIVATE_H_ */
