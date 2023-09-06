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
#include "ina233.h"
/* Defines -------------------------------------------------------------------*/
typedef enum
{
	MULTIPLICATION = 0,
	DIVISION = 1
} ArithmeticOperation;
/* Variables -----------------------------------------------------------------*/
/* Private Function definition -----------------------------------------------*/
uint32_t _Conversion(uint32_t ValueToConvert, float ConversionFactor, ArithmeticOperation ArithmeticOperation)
{
	float TemporaryResult;

	if(ArithmeticOperation == MULTIPLICATION)
		TemporaryResult = ValueToConvert * ConversionFactor;
	else if (ArithmeticOperation == DIVISION)
		TemporaryResult = ValueToConvert / ConversionFactor;

	return TemporaryResult;
}

/* Public Function -----------------------------------------------------------*/
void INA233_Init(INA233* Self, uint16_t Address)
{
	Self->Address 		= Address;
	Self->Calibration 	= 2048;
	Self->_VIN_LSB_mV 	= 1.25; //=1.25
	Self->IIN_LSB_uV 	= 2.50;

	Self->VIN_OV_mV		= 40960 - 1; //Massimo valore. Se metto 40960 invece di 40959 ottengo 0x8000 invece di 0x7FFF
	INA233_VIN_OV_mV_Conversion(Self);
	Self->IIN_OV_uV		= 81920 - 1;
	INA233_IIN_OV_uV_Conversion(Self);
}

void INA233_WARN_LIMIT_Update(INA233* Self)
{
	INA233_VIN_UV_mV_Conversion(Self);
	INA233_VIN_OV_mV_Conversion(Self);
	INA233_IIN_OV_uV_Conversion(Self);
}

void INA233_VIN_Conversion(INA233* Self)
{
	Self->VIN_mV = _Conversion(Self->VIN, Self->_VIN_LSB_mV, MULTIPLICATION);
}

void INA233_VIN_UV_mV_Conversion(INA233* Self)
{
	Self->VIN_UV = _Conversion(Self->VIN_UV_mV, Self->_VIN_LSB_mV, DIVISION);
}

void INA233_VIN_OV_mV_Conversion(INA233* Self)
{
	Self->VIN_OV = _Conversion(Self->VIN_OV_mV, Self->_VIN_LSB_mV, DIVISION);
}

void INA233_IIN_Conversion(INA233* Self)
{
	Self->IIN_uV = _Conversion(Self->IIN, Self->IIN_LSB_uV, MULTIPLICATION);
}

void INA233_IIN_OV_uV_Conversion(INA233* Self)
{
	Self->IIN_OV = _Conversion(Self->IIN_OV_uV, Self->IIN_LSB_uV, DIVISION);
}
/* Private Function ----------------------------------------------------------*/
/* End of the file -----------------------------------------------------------*/
