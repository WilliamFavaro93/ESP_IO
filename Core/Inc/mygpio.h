/**
  ******************************************************************************
  * @file	mygpio.h
  * @author Willam.Favaro
  * @date	20230831
  * @brief	MYGPIO is a struct that refers to a GPIO of the microcontroller.
  *
  ******************************************************************************
  * @note 	The purpose is give a better understanding of the code and
  * 		re-using the same name
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INC_MYGPIO_H_
#define INC_MYGPIO_H_
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stm32l0xx_hal.h>

/** @defgroup MYGPIO MYGPIO
  * @{
  */

/* Defines -------------------------------------------------------------------*/
/**
  * @brief   MyGPIO_Mode enumeration definition
  */
typedef enum
{
	INPUT = 0,	/*!<  */
	OUTPUT = 1	/*!<  */
} MyGPIO_Mode;
/**
  * @}
  */

/**
  * @brief   MYGPIO structure definition
  */
typedef struct
{
	GPIO_TypeDef* 	GPIO;		/*!<  */
	uint16_t 		Pin;		/*!<  */
	MyGPIO_Mode 	Mode;		/*!<  */
	uint8_t 		PinState;	/*!<  */
} MyGPIO;
/**
  * @}
  */
/* Variables -----------------------------------------------------------------*/
/* Private Function definition -----------------------------------------------*/
/* Public Function -----------------------------------------------------------*/
/* Public Function definition ------------------------------------------------*/
void MYGPIO_Init(MyGPIO* Self, 	GPIO_TypeDef* GPIO, uint16_t Pin, MyGPIO_Mode Mode);
void MYGPIO_PinState_Update(MyGPIO* Self);

/**
  * @}
  */

#endif /* INC_MYGPIO_H_ */
