/**
  ******************************************************************************
  * @file	mytimer.c
  * @author Willam.Favaro
  * @date	2022/09/15
  * @brief	mytimer structure methods
  *
  ******************************************************************************
  * @note
  *
  *
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include <mytimer.h>
/* Defines -------------------------------------------------------------------*/
/* Variables -----------------------------------------------------------------*/
/* Private Function definition -----------------------------------------------*/
/* Public Function -----------------------------------------------------------*/

/*
 * @brief This method manages MyTimer._Value and MyTimer._Trigger
 * @author William Favaro
 * @date 15/09/2022
 * @note This method works only if MyTimer.Enable and/or MyTimer.Refresh aren't equal to 0
 */
void MYTIMER_Countdown(MyTimer * Self, uint8_t Condition)
{
	// Se Enable è uguale a 0, non fare nulla
	if(!(Self->Enable))
		return;

	//Se il valore di refresh è uguale a 0, non fare nulla
	if(!(Self->Refresh))
		return;

	if(Condition)
		Self->_Value = Self->Refresh;
	else if(Self->_Value)
		Self->_Value--;

	Self->_Trigger = !(Self->_Value);
}

/* Private Function ----------------------------------------------------------*/
/* End of the file -----------------------------------------------------------*/
