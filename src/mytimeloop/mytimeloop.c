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
#include <mytimeloop.h>
/* Defines -------------------------------------------------------------------*/
/* Variables -----------------------------------------------------------------*/
/* Private Function definition -----------------------------------------------*/
/* Public Function -----------------------------------------------------------*/

/*
 * @brief
 * @author William Favaro
 * @date
 * @note
 */
void MYTIMELOOP_Init(TimeLoop* Self, uint16_t SpanTime, uint32_t ActualTime)
{
	Self->_LastRunTime 	= ActualTime;
	Self->_SpanTime 	= SpanTime;
	Self->_Enable 		= 1;
}


/*
 * @brief
 * @author William Favaro
 * @date
 * @note
 */
uint8_t MYTIMELOOP_Run(TimeLoop* Self, uint32_t ActualTime)
{
	if(Self->_Enable)
	{
		if((ActualTime - Self->_LastRunTime) > Self->_SpanTime)
		{
			Self->_LastRunTime += Self->_SpanTime;
			return 1;
		}
	}
	return 0;
}
/* Private Function ----------------------------------------------------------*/
/* End of the file -----------------------------------------------------------*/

