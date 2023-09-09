/**
  ******************************************************************************
  * @file
  * @author Willam.Favaro
  * @date
  * @brief
  *
  ******************************************************************************
  * @note
  *
  *
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INC_MYTIMELOOP_H_
#define INC_MYTIMELOOP_H_

/** @defgroup MYTIMELOOP MYTIMELOOP
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
/* Defines -------------------------------------------------------------------*/
/* Variables -----------------------------------------------------------------*/
typedef struct
{
	uint32_t _LastRunTime;
	uint16_t _SpanTime;
	uint8_t  _Enable;
} TimeLoop;
/* Private Function definition -----------------------------------------------*/
/* Public Function -----------------------------------------------------------*/
/* Public Function definition ------------------------------------------------*/
void MYTIMELOOP_Init(TimeLoop* Self, uint16_t SpanTime, uint32_t ActualTime);
uint8_t MYTIMELOOP_Run(TimeLoop* Self, uint32_t ActualTime);

#endif /* INC_MYTIMELOOP_H_ */
