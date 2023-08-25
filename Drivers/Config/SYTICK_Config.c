/*
 ******************************************************************************
 * @file           : SYTICK_Config.c
 * @Author         : Mohammed Ayman Shalaby
 * @brief          : SYSTICK Configuration File
 * @Date           : May 12, 2023
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 Ayman.
 * All rights reserved.
 *
 ******************************************************************************
 */

/* ======================================================================
 * INCLUDES SECTION
 * ====================================================================== */
#include <stdint.h>

#include "../../Library/ErrTypes.h"

#include "../Inc/SYSTICK_Interface.h"

/* ======================================================================
 * CONFIGURATION
 * ====================================================================== */

/* For Options Refer To Interface File :  @SYSTICK_EXCEPTION_t  &  @SYSTICK_CLK_SOURCE_t */

SYSTICK_CONFIG_t SYSTICK_TIMER_CONFIG =
{
  .Exception = DISABLE_SYSTICK_EXCEPTION , .CLK = SYSTICK_AHB_BY8
};
