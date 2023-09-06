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


/* Includes ------------------------------------------------------------------*/
#include <mygpio.h>
/* Defines -------------------------------------------------------------------*/
/* Variables -----------------------------------------------------------------*/
/* Private Function definition -----------------------------------------------*/
void MYGPIO_Init(MyGPIO* Self, 	GPIO_TypeDef* GPIO, uint16_t Pin, MyGPIO_Mode Mode)
{
	Self->GPIO = GPIO;
	Self->Pin = Pin;
	Self->Mode = Mode;
}

void MYGPIO_PinState_Update(MyGPIO* Self)
{
	if(Self->Mode == OUTPUT)
		HAL_GPIO_WritePin(Self->GPIO, Self->Pin, Self->PinState);
	else
		Self->PinState = HAL_GPIO_ReadPin(Self->GPIO, Self->Pin);
}
/* Public Function -----------------------------------------------------------*/
/* Private Function ----------------------------------------------------------*/
/* End of the file -----------------------------------------------------------*/
