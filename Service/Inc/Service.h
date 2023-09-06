/*
 ******************************************************************************
 * @file           : Service.h
 * @Author         : MOHAMMEDs & HEMA
 * @brief          : Service Header File
 * @Date           : Aug 30, 2023
 ******************************************************************************
 */
#ifndef INC_SERVICE_H_
#define INC_SERVICE_H_

/* ========================================================================= *
 *                            MACROS SECTION                                 *
 * ========================================================================= */

#define ZERO_ASCII 48

#define NUM_OF_TRIES 0x03

typedef enum
{
	NO_OPTION = 0x00,
	DISPLAY_OPTION = (0x01 + ZERO_ASCII),
	SET_ALARM_OPTION = (0x02 + ZERO_ASCII),
	SET_DATE_TIME_OPTION = (0x03 + ZERO_ASCII),

} OPTIONS_t;

/* ========================================================================= *
 *                      FUNCTIONS PROTOTYPES SECTION                         *
 * ========================================================================= */

/*=======================================================================================
 * @fn		 		:	Clock_Init
 * @brief			:	Enable The Pripherals Clocks
 * @param			:	void
 * @retval			:	void
 * ======================================================================================*/
void Clock_Init(void);

/*=======================================================================================
 * @fn		 		:	Pins_Init
 * @brief			:	Set The Pins Configurations
 * @param			:	void
 * @retval			:	void
 * ======================================================================================*/
void Pins_Init(void);

/*=======================================================================================
 * @fn		 		:	USART2_Init
 * @brief			:	Initialize USART2
 * @param			:	void
 * @retval			:	void
 * ======================================================================================*/
void USART2_Init(void);

/*=======================================================================================
 * @fn		 		:	SPI1_Init
 * @brief			:	Initialize SPI1
 * @param			:	void
 * @retval			:	void
 * ======================================================================================*/
void SPI1_Init(void);

/*=======================================================================================
 * @fn		 		:	I2C1_Init
 * @brief			:	Initialize I2C1
 * @param			:	void
 * @retval			:	void
 * ======================================================================================*/
void I2C1_Init(void);

/** ============================================================================
 * @fn 				: ID_Reception
 *
 * @brief 			: This Function is Used to Receive ID From User Digit By Digit
 *
 * @param[in]		: void
 *
 * @return 			: uint8_t *ID_Ptr > Pointer to ID Array
 *
 * ============================================================================
 */
uint8_t *ID_Reception(void);

/** ============================================================================
 * @fn 				: Pass_Reception
 *
 * @brief 			: This Function is Used to Receive Password From User Digit By Digit
 *
 * @param[in]		: void
 *
 * @return 			: uint8_t *Pass_Ptr > Pointer to Password Array
 *
 * ============================================================================
 */
uint8_t *Pass_Reception(void);

/** ============================================================================
 * @fn 				: Clear_Terminal
 *
 * @brief 			: This Function is Used to Clear Putty Terminal

 * @param[in]		: void
 *
 * @return 			: void
 *
 * ============================================================================
 */
void Clear_Terminal(void);

/** ============================================================================
 * @fn 				: Display_Menu
 *
 * @brief 			: This Function is Called When User Enters the System , It Displays the Menu to the User
 *                    and Asks Him to Choose From the Following Options :
 *                    1- Display Date & Time
 * 				      2- Set Alarm
 * 				      3- Set Date & Time
 *
 * @param[in]		: void
 *
 * @return 			: OPTIONS_t > Option Choosen by User ( 1-3 )
 *
 * @note			: This Function is Called Only Once When User Enters the System ( ID & Password are Correct )
 * ============================================================================
 */
OPTIONS_t Display_Menu(void);

/** ============================================================================
 * @fn 				: SendNew_Line
 *
 * @brief 			: This Function is Used to Send New Line in Putty Terminal

 * @param[in]		: void
 *
 * @return 			: void
 *
 * ============================================================================
 */
void SendNew_Line(void);

/** ============================================================================
 * @fn 				: Check_LoginInfo
 *
 * @brief 			: This Function is Called When User Enters the System ,
 *                    It Checks on the ID & Password if ID is Equal to Password Inverted or Not
 * 				      if ID is Equal to Password Inverted , User Enters the System ,
 *                    if Not , User is Asked to Enter ID & Password Again Till he Finish His Number of Tries
 *          	   	  if Number of Tries is Finished , SPI Data is Sent to Light Up the Red LED on BluePill Board,
 *                    IN this Case   SPI Call Back Function is Called to Execute the Shutdown Sequence
 *                    and the Program is Stuck in it
 *
 * @param[in]		:  uint8_t *ID_Ptr > Pointer to ID Array
 * 					   uint8_t *Pass_Ptr > Pointer to Password Array
 * 					   uint8_t TriesNumber > Number of Tries Allowed to User to Enter the System Before Shutting Down
 *
 * @return 			: void
 * 					  This Function Never Returns
 * ============================================================================
 */
void Check_LoginInfo(uint8_t *ID_Ptr, uint8_t *Pass_Ptr, uint8_t TriesNumber);

/*=======================================================================================
 * @fn		 		:	ReadDateTime_FromPC
 * @brief			:	Read Date & Time From The user Via USART
 * @param			:	void
 * @retval			:	Error State
 * ======================================================================================*/
Error_State_t ReadDateTime_FromPC(void);

/*==============================================================================================================================================
 *@fn      : void InterruptsInit (void)
 *@brief  :  This Function Is Responsible For Initializing The Interrupts
 *@retval void :
 *==============================================================================================================================================*/
void Interrupts_Init(void);

/** ============================================================================
 * @fn 				: Check_IF_ContinueisNeeded
 *
 * @brief 			: This Function is Called When User Finishes His Functionality( Executing Choosin Option )
 *                    and Asks Him if He Wants to Continue Choosing Options or Not
 *
 * @param[in]		: void
 *
 * @return 			: void
 * 					  This Function Never Returns
 * ============================================================================
 */
void Check_IF_ContinueisNeeded(void);

/** ============================================================================
 * @fn 				: Wrong_OptionChoosen
 *
 * @brief 			: This Function is Called When User Passes Wrong Option to the System ,
 *                    It Clears the Terminal & Display a Message to the User to Enter a Valid Option
 *
 * @param[in]		: void
 *
 * @return 			: void
 * 					  This Function Never Returns
 * ============================================================================
 */
void WRONG_OptionChoosen(void);

/** ============================================================================
 * @fn 				: ShutDown_Sequence
 *
 * @brief 			: This Function is Called Inside Call Back Function of SPI ,
 *                    When Number of Tries of User is Finished &
 *                    RED_LED_CODE is Transmitted to the Blue Pill Board ,
 *                    This Function is Called to Execute the Shutdown Sequence
 *
 * @param[in]		: void
 *
 * @return 			: void
 *
 * ============================================================================
 */
void ShutDown_Sequence(void);

/*=======================================================================================
 * @fn		 		:	Reading_Time
 * @brief			:	Reading data from RTC and store data in Reading Array
 * @param			:	void
 * @retval			:	void
 * ======================================================================================*/
void Reading_Time(void);

/*=======================================================================================
 * @fn		 		:	Transmit_Time
 * @brief			:	Transmit Buffer of data via SPI (with interrupt)
 * @param			:	void
 * @retval			:	void
 * ======================================================================================*/
void Transmit_Time(void);

/*==============================================================================================================================================
 *@fn      : void SetAlarm()
 *@brief  :  This Function Is Responsible For Setting The Alarm Time
 *@retval void :
 *==============================================================================================================================================*/
void SetAlarm();

/*==============================================================================================================================================
 *@fn      : void CalcAlarm(uint8_t AlarmNumber)
 *@brief  :  This Function Is Responsible For Calculating The Alarm Time And Storing It In The Global Array
 *@paramter[in]  : uint8_t AlarmNumber : The Number Of The Alarm To Be Set
 *@retval void :
 *==============================================================================================================================================*/
void CalcAlarm(uint8_t AlarmNumber);

/*==============================================================================================================================================
 *@fn      :  void CompTime()
 *@brief  :   This Function Is Responsible For Comparing The Current Time With The Alarm Time And Send The Alarm Number To The Blue Pill If They Are Equal
 *@retval void :
 *==============================================================================================================================================*/
void CompTime();

/* ========================================================================= *
 *                                 HANDLERS			                         *
 * ========================================================================= */

/*==============================================================================================================================================
 *@fn      :  void SPI_CallBackFunc()
 *@brief   :  SPI Call Back Function in Case of User Finishes His Number of Tries ,  Executes the Shutdown Sequence
 *@retval  :  void
 *==============================================================================================================================================*/
void SPI_CallBackFunc(void);

/*==============================================================================================================================================
 *@fn      :  void SPI_CALL_BACK()
 *@brief   :  Empty SPI Call Back Function in Case of User Choose Display Date & Time Option
 *@retval  :  void
 *==============================================================================================================================================*/
void SPI_CALL_BACK(void);

/*==============================================================================================================================================
 *@fn      :  void SPI1_ISR()
 *@brief   :  This Function Is ISR For SPI In Case Of Alarm is Triggered to Send Inside the Call Back , Pulse to the Blue Pill Board
 *@retval  :  void
 *==============================================================================================================================================*/
void SPI1_ISR();

/*==============================================================================================================================================
 *@fn      : void SysTickPeriodicISR()
 *@brief  :  This Function Is The ISR For The SYSTICK Interrupt , It Is Responsible For Calling The CompTime Function Every One Second
 *@retval void :
 *==============================================================================================================================================*/
void SysTickPeriodicISR();

#endif /* INC_SERVICE_H_ */
