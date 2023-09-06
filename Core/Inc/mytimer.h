/**
  ******************************************************************************
  * @file	mytimer.h
  * @author Willam.Favaro
  * @date	2022/09/15
  * @brief	mytimer structure definition
  *
  ******************************************************************************
  * @note
  *
  *
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INC_MYTIMER_H_
#define INC_MYTIMER_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
/* Defines -------------------------------------------------------------------*/
/* Typedef Struct definition -------------------------------------------------*/
/**
  * @brief  MyTimer structure definition
  */
typedef struct
{
	uint8_t  Enable;			/**< PUBLIC:  define if the timer is enabled to work. */
	uint32_t Refresh;			/**< PUBLIC:  define how many countdown() need before to trigger. */
	uint8_t  _Trigger;			/**< PRIVATE: define if the timer is triggered. */
	uint32_t _Value;			/**< PRIVATE: define the lasting number of Countdown() before to trigger. */
} MyTimer;
/* Public Function definition ------------------------------------------------*/

void MYTIMER_Countdown(MyTimer * Self, uint8_t Condition);

#endif /* INC_MYTIMER_H_ */
