/*
 *@file		:	I2C_Private.h
 *@author	: 	Mohamed Wael
 *@brief	:	Main Privates for I2C Peripheral
 */

#ifndef I2C_INC_I2C_PRIVATE_H_
#define I2C_INC_I2C_PRIVATE_H_

/***************** STATIC FUNCTIONS ********************/
/*
 * @function 		:	I2C_Set_SCL_Frequency
 * @brief			:	Set SCL Frequency of the wanted I2C
 * @param			:	I2C Configurations
 * @retval			:	VOID
 */
static void I2C_Set_SCL_Frequency(const I2C_Configs_t * I2C_Configs);

/*
 * @function 		:	I2C_CheckConfigs
 * @brief			:	Check I2c Configurations
 * @param			:	I2C Configurations
 * @retval			:	Error State
 */
static Error_State_t I2C_CheckConfigs(const I2C_Configs_t * I2C_Configs);

/*

@function         :    I2C_CLR_ADDR
@brief            :    Software sequence to Clear ADDR Flag
@param            :    I2C Number
@retval            :    VOID
*/
 void I2C_CLR_ADDR(I2C_I2C_NUMBER_t I2C_Num);
/************** End of STATIC FUNCTIONS ****************/



/*****************Private Defines***********************/

#define MAX_I2C_NUMBERS			3

#define ACK_STATE_BIT			10
#define STRETCH_STATE_BIT		7
#define PEC_STATE_BIT			5
#define ADDMODE_BIT				15
#define ADDVALUE_BIT			1
#define I2C_MODE_BIT			15
#define START_GENERATE_BIT		8
#define STOP_GENERATE_BIT		9
#define DMAEN_BIT				11
#define PE_BIT					0

#define PCLK_MASK				0b111111
#define ADDVALUE_MASK			0b1111111
/**************End of Private Defines*******************/


#endif /* I2C_INC_I2C_PRIVATE_H_ */
