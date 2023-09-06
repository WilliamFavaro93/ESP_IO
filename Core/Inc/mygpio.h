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
#ifndef INC_MYGPIO_H_
#define INC_MYGPIO_H_
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stm32l0xx_hal.h>
/* Defines -------------------------------------------------------------------*/
typedef enum
{
	INPUT = 0,
	OUTPUT = 1
} MyGPIO_Mode;

typedef struct
{
	GPIO_TypeDef* GPIO;
	uint16_t Pin;
	uint8_t Mode;
	uint8_t PinState;
} MyGPIO;
/* Variables -----------------------------------------------------------------*/
/* Private Function definition -----------------------------------------------*/
/* Public Function -----------------------------------------------------------*/
/* Public Function definition ------------------------------------------------*/
void MYGPIO_Init(MyGPIO* Self, 	GPIO_TypeDef* GPIO, uint16_t Pin, MyGPIO_Mode Mode);
void MYGPIO_PinState_Update(MyGPIO* Self);


#endif /* INC_MYGPIO_H_ */
