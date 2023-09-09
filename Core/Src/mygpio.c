/**
  ******************************************************************************
  * @file	mygpio.c
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


/* Includes ------------------------------------------------------------------*/
#include <mygpio.h>

/** @addtogroup MYGPIO MYGPIO
  * @{
  */

/* Defines -------------------------------------------------------------------*/
/* Variables -----------------------------------------------------------------*/
/* Private Function definition -----------------------------------------------*/
/* Public Function -----------------------------------------------------------*/
/**
 * @brief 	Initialize a MYGPIO struct
 * @param 	MYGPIO struct
 * @param 	GPIO Port
 * @param 	GPIO Pin
 * @param 	GPIO Mode
 */
void MYGPIO_Init(MyGPIO* Self, 	GPIO_TypeDef* GPIO, uint16_t Pin, MyGPIO_Mode Mode)
{
	Self->GPIO = GPIO;
	Self->Pin = Pin;
	Self->Mode = Mode;
}

/**
 * @brief 	OUTPUT GPIO: Modify GPIO's PinState based on MYGPIO's PinState
 * 			INPUT GPIO:  Modify MYGPIO's PinState based on GPIO's PinState
 * @param 	Self, MYGPIO
 */
void MYGPIO_PinState_Update(MyGPIO* Self)
{
	if(Self->Mode == OUTPUT)
		HAL_GPIO_WritePin(Self->GPIO, Self->Pin, Self->PinState);
	else
		Self->PinState = HAL_GPIO_ReadPin(Self->GPIO, Self->Pin);
}
/* Private Function ----------------------------------------------------------*/

/**
  * @}
  */

/* End of the file -----------------------------------------------------------*/
