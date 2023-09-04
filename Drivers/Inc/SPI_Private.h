/*
 *@file		:	SPI_Private.h
 *@author	: 	Mohamed Wael
 *@brief	:	Main Private Defines FOR SPI Peripheral
 */

#ifndef SPI_INC_SPI_PRIVATE_H_
#define SPI_INC_SPI_PRIVATE_H_

/***************** STATIC FUNCTIONS ********************/

/*
 * @function 		:	SPI_Check_Configs
 * @brief			:	Private Function To Check Passed SPI Configurations
 * @param			:	SPI_Configs
 * @retval			:	Error State
 */
static Error_State_t SPI_Check_Configs(const SPI_CONFIGS_t * SPI_Configs);

/*
 * @function 		:	SPI_IRQ_Source_HANDLE
 * @brief			:	Private Function To handle the Different Sources of the IRQ
 * @param			:	SPI_Num, The Enabled SPI Number
 * @retval			:	Error State
 */
static void SPI_IRQ_Source_HANDLE(SPI_SPI_NUMBER_t SPI_Num);
/************** End of STATIC FUNCTIONS ****************/

/*****************Private Defines***********************/
#define MAX_SPIs_NUMBER	4

#define BAUD_RATE_START_BITS	3u
#define BAUD_RATE_MASK			0b111

#define CLOCK_POL_MASK			1u
#define CLOCK_POL_START_BITS	1u

#define CLOCK_PHASE_START_BITS	0u
#define CLOCK_PHASE_MASK		1u

#define TRANSFER_MODE_MASK		0b11
#define TRANS_MODE_START_BITS	14u

#define RX_ONLY_MODE_MASK		1
#define RX_ONLY_START_BITS		10

#define FRAME_TYPE_MASK			1u
#define FRAME_TYPE_START_BITS	7u

#define CRC_ENABLE_MASK			1u
#define CRC_ENABLE_START_BITS	13u

#define SLAVE_MANAGE_START_BITS	8u
#define SLAVE_MANAGE_MASK		0b11

#define CHIP_MODE_START_BITS	2u
#define CHIP_MODE_MASK			1u

#define FRAME_SIZE_START_BITS	11u
#define FRAME_SIZE_MASK			1u

#define SSOE_MASK				1u
#define SSOE_BIT_START			2u

#define SPI_ENABLE_BIT_START	6

#define GARBAGE_VALUE			0

#define SPI_MAX_INTERRUPTS		3

#define RXDMAEN_BIT				0
#define TXDMAEN_BIT				1

#define	SSI_BIT					8

#endif /* SPI_INC_SPI_PRIVATE_H_ */
typedef enum
{
    SPI_NO_IRQ_SRC = 0x00,
    SPI_TRANSCEIVE_DATA_IRQ_SRC = 0x01,
    SPI_TRANSCEIVE_BUFFER_IRQ_SRC = 0x02,

} SPI_IRQ_SRC_t;

typedef enum
{
    SPI_ROLE_TRANSMIT = 0x00,
    SPI_ROLE_RECEIVE = 0x01,
} SPI_ROLE_DIR_t;



#endif /* USART_PRIVATE_H_ */