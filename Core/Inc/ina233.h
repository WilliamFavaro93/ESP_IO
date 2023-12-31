#ifndef INA233_H
#define INA233_H
#include <stdint.h>
/*=========================================================================
    I2C ADDRESSES/BITS
    -----------------------------------------------------------------------*/
    #define INA233_ADDRESS_40                      (0x40)    // 1000000 (A1+A0=GND)
    #define INA233_ADDRESS_41                      (0x41)    // 1000001 (A1=GND, A0=VDD)
    #define INA233_ADDRESS_42                      (0x42)    // 1000010 (A1=GND, A0=SDA)
    #define INA233_ADDRESS_43                      (0x43)    // 1000011 (A1=GND, A0=SCL)
    #define INA233_ADDRESS_44                      (0x44)    // 1000100 (A1=VDD, A0=GND)
    #define INA233_ADDRESS_45                      (0x45)    // 1000101 (A1+A0=VDD)
    #define INA233_ADDRESS_46                      (0x46)    // 1000110 (A1=VDD, A0=SDA)
    #define INA233_ADDRESS_47                      (0x47)    // 1000111 (A1=VDD, A0=SCL)
    #define INA233_ADDRESS_48                      (0x48)    // 1001000 (A1=SDA, A0=GND)
    #define INA233_ADDRESS_49                      (0x49)    // 1001001 (A1=SDA, A0=VDD)
    #define INA233_ADDRESS_4A                      (0x4A)    // 1001010 (A1+A0=SDA)
    #define INA233_ADDRESS_4B                      (0x4B)    // 1001011 (A1=SDA, A0=SCL)
    #define INA233_ADDRESS_4C                      (0x4C)    // 1001100 (A1=SCL, A0=GND)
    #define INA233_ADDRESS_4D                      (0x4D)    // 1001101 (A1=SCL, A0=VDD)
    #define INA233_ADDRESS_4E                      (0x4E)    // 1001110 (A1=SCL, A0=SDA)
    #define INA233_ADDRESS_4F                      (0x4F)    // 1001111 (A1+A0=SCL)
/*=========================================================================*/

/*=====================================================================================================================
    PMBus Commands
    	    NAME					CODE 	FUNCTION																		R/W		Number of Data Bytes 	Default value
     -----------------------------------------------------------------------------------------------------------------*/

    #define CLEAR_FAULTS			(0x03)	//Clears the status registers and rearms the black box registers for updating 	SB 		0 						N/A
    #define RESTORE_DEFAULT_ALL		(0x12)	//Restores internal registers to the default values 							SB 		0 						N/A
    #define CAPABILITY				(0x19)	//Retrieves the device capability												R 		1 						xB0
    #define IOUT_OC_WARN_LIMIT		(0x4A)	//Retrieves or stores the output overcurrent warn limit threshold  				R/W 	2 						x7FF8
    #define VIN_OV_WARN_LIMIT		(0x57)  //Retrieves or stores the input overvoltage warn limit threshold   				R/W 	2 						x7FF8
    #define VIN_UV_WARN_LIMIT		(0x58)  //Retrieves or stores the input undervoltage warn limit threshold  				R/W 	2 						x0000
    #define PIN_OP_WARN_LIMIT		(0x6B)  //Retrieves or stores the output overpower warn limit threshold    				R/W 	2 						x7FF8
    #define STATUS_BYTE				(0x78)  //Retrieves information about the device operating status          				R 		1 						x00
    #define STATUS_WORD				(0x79)  //Retrieves information about the device operating status          				R 		2 						x1000
    #define STATUS_IOUT				(0x7B)  //Retrieves information about the output current status          				R/W,CLR 1 						x00
    #define STATUS_INPUT			(0x7C)  //Retrieves information about the input status          	        			R/W,CLR 1 						x00
    #define STATUS_CML				(0x7E)  //Retrieves information about the communications status          				R/W,CLR 1 						x00
    #define STATUS_MFR_SPECIFIC		(0x80)  //Retrieves information about the manufacturer specific device status   		R/W,CLR 1 						x20
    #define READ_EIN				(0x86)	//Retrieves the energy reading measurement										Block_R 6 						6 bytes at x00
    #define READ_VIN				(0x88)	//Retrieves the measurement for the VBUS voltage								R 		2 						0x0000
    #define READ_IIN				(0x89)	//Retrieves the input current measurement, supports both + and - currents 		R 		2 						0x0000
    #define READ_VOUT				(0x8B)	//Mirrors READ_VIN																R 		2 						0x0000
    #define READ_IOUT				(0x8C)	//Mirrors READ_IIN for compatibility 											R		2						0x0000
    #define READ_POUT				(0x96)	//Mirrors READ_PIN for compatibility with possible VBUS connections				R		2						0x0000
    #define READ_PIN				(0x97)	//Retrieves the input power measurement											R		2						0x0000
    #define MFR_ID					(0x99)	//Retrieves the manufacturer ID in ASCII Characters (TI)						Block_R 2 						0x54, 0x49
    #define MFR_MODEL				(0x9A)	//Retrieves the device number in ASCII Characters (INA233)						Block_R 6 						0x49, 0x4E, 0x41, 0x32, 0x33, 0x33
    #define MFR_REVISION			(0x9B)	//Retrieves the device revision letter and number in ASCII (e.g.A0)				R 		2 						0x41,0x30
    #define MFR_ADC_CONFIG			(0xD0)	//Configures the ADC averaging modes, conversion times, and opr. modes  		R/W 	2 						0x4127
    #define MFR_READ_VSHUNT			(0xD1)	//Retrieves the shunt voltage measurement										R 		2 						0x0000
    #define MFR_ALERT_MASK			(0xD2)	//Allows masking of device warnings												R/W 	1 						0xF0
    #define MFR_CALIBRATION			(0xD4)	//Allows the value of the current-sense resistor calibration value to be input.
						                    //Must be programed at power-up. Default value is set to 1. 					R/W 	2 						0x0001
    #define MFR_DEVICE_CONFIG		(0xD5)	//Allows the ALERT pin polarity to be changed									R/W 	1 						0x02
    #define CLEAR_EIN				(0xD6)	//Clears the energy accumulator													S_B 	0 						N/A
    #define TI_MFR_ID				(0xE0)	//Returns a unique word for the manufacturer ID									R 		2 						0x5449 (TI in ASCII)
    #define TI_MFR_MODEL			(0xE1)	//Returns a unique word for the manufacturer model								R 		2 						33 in ASCII
    #define TI_MFR_REVISION			(0xE2)	//Returns a unique word for the manufacturer revision							R 		2 						A0 in ASCII

/*=========================================================================*//*=========================================================================*/
/*=========================================================================
    SHUNT VOLTAGE TELEMETRY & WARNING COEFFICIENTS
    -----------------------------------------------------------------------*/
    #define m_vs                (4)
    #define R_vs                (5)
    #define b_vs                (0)
/*=========================================================================*/

/*=========================================================================
    BUS VOLTAGE TELEMETRY & WARNING COEFFICIENTS
    -----------------------------------------------------------------------*/
    #define m_vb                (8)
    #define R_vb                (2)
    #define b_vb                (0)
/*=========================================================================*/

/*=========================================================================
    CURRENT & POWER CONSTANT TELEMETRY & WARNING COEFFICIENTS
    -----------------------------------------------------------------------*/
    #define b_c                (0)
    #define b_p                (0)
/*=========================================================================*/

typedef struct
{
	uint16_t Address;
	uint16_t Calibration; //= 2048. Quando il messaggio di CAL è uguale a 2048, IIN == VSHUNT.

	float _VIN_LSB_mV; //1.25 mV

	uint32_t VIN;
	uint32_t VIN_UV;
	uint32_t VIN_OV;

	uint32_t VIN_mV;
	uint32_t VIN_UV_mV;
	uint32_t VIN_OV_mV;

	float IIN_LSB_uV; //=2.5

	uint32_t IIN;
	uint32_t IIN_OV;

	uint32_t IIN_uV;
	uint32_t IIN_OV_uV;
} INA233;


void INA233_Init(INA233* Self, uint16_t Address);
void INA233_WARN_LIMIT_Conversion(INA233* Self);
void INA233_VIN_Conversion(INA233* Self);
void INA233_VIN_UV_mV_Conversion(INA233* Self);
void INA233_VIN_OV_mV_Conversion(INA233* Self);
void INA233_IIN_Conversion(INA233* Self);
void INA233_IIN_OV_uV_Conversion(INA233* Self);
#endif
