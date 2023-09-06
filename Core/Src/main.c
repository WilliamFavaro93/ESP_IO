/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "mygpio.h"
#include "mytimer.h"
#include "mytimeloop.h"
#include "ina233.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum
{
	ADDRESS_PinState 	= 0x01,
	ALM_RELE_PinState 	= 0x02,
	FAULT_RELE_PinState = 0x03,
	CMD_RELE_PinState 	= 0x04,

	INA1_VIN_UV_mV_LSB 	= 0x80,
	INA1_VIN_UV_mV_MSB 	= 0x81,
	INA1_VIN_OV_mV_LSB 	= 0x82,
	INA1_VIN_OV_mV_MSB 	= 0x83,
	INA1_IIN_OV_uV_LSB 	= 0x84,
	INA1_IIN_OV_uV_MSB 	= 0x85,
} CONTROL_Index;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define INA233_SIZE		5
#define ADDRESS_SIZE	4
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

/* USER CODE BEGIN PV */
MyGPIO ALM_RELE[INA233_SIZE];
MyGPIO FAULT_RELE[INA233_SIZE];
MyGPIO CMD_RELE[INA233_SIZE];
MyGPIO ADDRESS[ADDRESS_SIZE];
MyGPIO EXT_INT;

INA233 INA[INA233_SIZE];

uint8_t CONTROL[255];

TimeLoop INA_TimeLoop;

//Per inviare messaggi come CLEAR_FAULTS
uint8_t i2c2_Data;

//Per gestire HAL_I2C_AddrCallback(..)
uint8_t rxcount;
uint8_t txcount;
uint8_t i2c1_Register;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C2_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */
void MYGPIO_ALM_RELE_Init(MyGPIO* Self);
void MYGPIO_CMD_RELE_Init(MyGPIO* Self);
void MYGPIO_FAULT_RELE_Init(MyGPIO* Self);
void MYGPIO_ADDRESS_Init(MyGPIO* Self);
void MYGPIO_EXT_INT_Init(MyGPIO* Self);
void MX_GPIO_I2C_OwnAddress1_Update(I2C_HandleTypeDef* hi2c, uint8_t ADDRESS1_PinState, uint8_t ADDRESS2_PinState, uint8_t ADDRESS3_PinState);
void CMD_RELE_Update(MyGPIO* Self, uint8_t Rele_IDNumber, uint8_t ReleControl);
void CONTROL_Init();

void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t transferDirection, uint16_t addressMatchCode);
void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C2_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  MYGPIO_ADDRESS_Init((MyGPIO*) ADDRESS);
  MYGPIO_ALM_RELE_Init((MyGPIO*) ALM_RELE);
  MYGPIO_CMD_RELE_Init((MyGPIO*) CMD_RELE);
  MYGPIO_FAULT_RELE_Init((MyGPIO*) FAULT_RELE);
  MYGPIO_EXT_INT_Init((MyGPIO*) &EXT_INT);

  for(uint8_t i = 1; i < INA233_SIZE; i++)
	  INA233_Init(&INA[i], (0x40 + i - 1) << 1);

  //Esempio di aggiornamento warn limit
  INA[2].VIN_UV_mV = 1000;
  INA[2].VIN_OV_mV = 2000;
  INA[2].IIN_OV_uV = 80000;

  INA[1].VIN_UV_mV = 1000;
  INA[1].VIN_OV_mV = 2000;

  for(uint8_t i = 1; i < INA233_SIZE; i++)
  {
	  INA233_WARN_LIMIT_Update(&INA[i]);

	  HAL_I2C_Mem_Write(&hi2c2, INA[i].Address, MFR_CALIBRATION, 1, (uint8_t *)&INA[i].Calibration, 2, 1000);
	  HAL_I2C_Mem_Write(&hi2c2, INA[i].Address, VIN_UV_WARN_LIMIT, 1, (uint8_t *)&INA[i].VIN_UV, 2, 1000);
	  HAL_I2C_Mem_Write(&hi2c2, INA[i].Address, VIN_OV_WARN_LIMIT, 1, (uint8_t *)&INA[i].VIN_OV, 2, 1000);
	  HAL_I2C_Mem_Write(&hi2c2, INA[i].Address, IOUT_OC_WARN_LIMIT, 1, (uint8_t *)&INA[i].IIN_OV, 2, 1000);
  }

  MYTIMELOOP_Init(&INA_TimeLoop, 500, uwTick);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  //Address update
	  for(uint8_t i = 1; i < ADDRESS_SIZE; i++)
		  MYGPIO_PinState_Update(&ADDRESS[i]);
	  CONTROL[ADDRESS_PinState] = (ADDRESS[1].PinState << 0) + (ADDRESS[2].PinState << 1) + (ADDRESS[3].PinState << 2);
	  MX_GPIO_I2C_OwnAddress1_Update(&hi2c1, ADDRESS[1].PinState, ADDRESS[2].PinState, ADDRESS[3].PinState);
	  //Abilita la possibilità di rispondere in I2C
	  HAL_I2C_EnableListen_IT(&hi2c1);

#if 0
	  //Alarm - Fault Rele Update
	  for(uint8_t i = 1; i < INA233_SIZE; i++)
	  {
		  MYGPIO_PinState_Update(&ALM_RELE[i]);
		  FAULT_RELE[i].PinState = ALM_RELE[i].PinState;
		  MYGPIO_PinState_Update(&FAULT_RELE[i]);
	  }
	  CONTROL_1B[ALM_RELE_PinState] = (ALM_RELE[1].PinState << 0) + (ALM_RELE[2].PinState << 1) + (ALM_RELE[3].PinState << 2) + (ALM_RELE[4].PinState << 3);
	  CONTROL_1B[FAULT_RELE_PinState] = (FAULT_RELE[1].PinState << 0) + (FAULT_RELE[2].PinState << 1) + (FAULT_RELE[3].PinState << 2) + (FAULT_RELE[4].PinState << 3);
#endif

	  //Comando Rele Update
	  for(uint8_t i = 1; i < INA233_SIZE; i++)
	  {
		  CMD_RELE_Update(&CMD_RELE[i], i, CONTROL[CMD_RELE_PinState]);
		  MYGPIO_PinState_Update(&CMD_RELE[i]);
	  }

	  if(MYTIMELOOP_Run(&INA_TimeLoop, uwTick))
	  {
		  for(uint8_t i = 1; i < INA233_SIZE; i++)
		  {
			  //azzerra i fault
			  i2c2_Data = CLEAR_FAULTS;
			  HAL_I2C_Master_Transmit(&hi2c2, INA[i].Address, (uint8_t *)&i2c2_Data, 1, 1000);
			  //interroga l'adc
			  HAL_I2C_Mem_Read(&hi2c2, INA[i].Address, READ_VIN, 1, (uint8_t *)&INA[i].VIN, 2, 1000);
			  HAL_I2C_Mem_Read(&hi2c2, INA[i].Address, READ_IIN, 1, (uint8_t *)&INA[i].IIN, 2, 1000);
			  //converte i valori ottenuti
			  INA233_VIN_Conversion(&INA[i]);
			  INA233_IIN_Conversion(&INA[i]);
		  }
	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00000708;
  hi2c1.Init.OwnAddress1 = 128;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x00000708;
  hi2c2.Init.OwnAddress1 = 128;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_8|GPIO_PIN_9
                          |GPIO_PIN_10|GPIO_PIN_11, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12|GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA1 PA8 PA9
                           PA10 PA11 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_8|GPIO_PIN_9
                          |GPIO_PIN_10|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA2 PA12 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB12 PB13 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB14 PB15 */
  GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB3 PB4 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI2_3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_3_IRQn);

  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void MYGPIO_ALM_RELE_Init(MyGPIO* Self)
{
	MYGPIO_Init(&Self[1], GPIOB, GPIO_PIN_15, INPUT);
	MYGPIO_Init(&Self[2], GPIOA, GPIO_PIN_2, INPUT);
	MYGPIO_Init(&Self[3], GPIOA, GPIO_PIN_12, INPUT);
	MYGPIO_Init(&Self[4], GPIOB, GPIO_PIN_14, INPUT);
}

void MYGPIO_CMD_RELE_Init(MyGPIO* Self)
{
	MYGPIO_Init(&Self[1], GPIOA, GPIO_PIN_8, OUTPUT);
	MYGPIO_Init(&Self[2], GPIOA, GPIO_PIN_1, OUTPUT);
	MYGPIO_Init(&Self[3], GPIOA, GPIO_PIN_11, OUTPUT);
	MYGPIO_Init(&Self[4], GPIOB, GPIO_PIN_13, OUTPUT);
}

void MYGPIO_FAULT_RELE_Init(MyGPIO* Self)
{
	MYGPIO_Init(&Self[1], GPIOA, GPIO_PIN_9, OUTPUT);
	MYGPIO_Init(&Self[2], GPIOA, GPIO_PIN_0, OUTPUT);
	MYGPIO_Init(&Self[3], GPIOA, GPIO_PIN_10, OUTPUT);
	MYGPIO_Init(&Self[4], GPIOB, GPIO_PIN_12, OUTPUT);
}

void MYGPIO_ADDRESS_Init(MyGPIO* ADDRESS)
{
	MYGPIO_Init(&ADDRESS[1], GPIOA, GPIO_PIN_15, INPUT);
	MYGPIO_Init(&ADDRESS[2], GPIOB, GPIO_PIN_3, INPUT);
	MYGPIO_Init(&ADDRESS[3], GPIOB, GPIO_PIN_4, INPUT);
}

void MYGPIO_EXT_INT_Init(MyGPIO* EXT_INT)
{
	MYGPIO_Init(EXT_INT, GPIOC, GPIO_PIN_13, OUTPUT);
}

void MX_GPIO_I2C_OwnAddress1_Update(I2C_HandleTypeDef* hi2c, uint8_t ADDRESS1_PinState, uint8_t ADDRESS2_PinState, uint8_t ADDRESS3_PinState)
{
	uint8_t hi2c_Init_OwnAddress1 = 0x40;
	hi2c_Init_OwnAddress1 += (ADDRESS1_PinState << 0);
	hi2c_Init_OwnAddress1 += (ADDRESS2_PinState << 1);
	hi2c_Init_OwnAddress1 += (ADDRESS3_PinState << 2);
	hi2c_Init_OwnAddress1 = hi2c_Init_OwnAddress1 << 1;

	//Update I2C Slave Address
	if(hi2c->Init.OwnAddress1 != hi2c_Init_OwnAddress1)
	{
		hi2c->Init.OwnAddress1 = hi2c_Init_OwnAddress1;
		if (HAL_I2C_Init(hi2c) != HAL_OK)
		{
		Error_Handler();
		}

		/** Configure Analogue filter
		*/
		if (HAL_I2CEx_ConfigAnalogFilter(hi2c, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
		{
		Error_Handler();
		}

		/** Configure Digital filter
		*/
		if (HAL_I2CEx_ConfigDigitalFilter(hi2c, 0) != HAL_OK)
		{
		Error_Handler();
		}
	}
}

void CMD_RELE_Update(MyGPIO* Self, uint8_t Rele_IDNumber, uint8_t ReleControl)
{
	uint8_t BitPosition = 1 << (Rele_IDNumber - 1);
	Self->PinState = (BitPosition & ReleControl);
}

void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode)
{
	if (TransferDirection == I2C_DIRECTION_TRANSMIT)  // if the master wants to transmit the data
	{
		rxcount = 0;
		HAL_I2C_Slave_Seq_Receive_IT(hi2c, (uint8_t *)&i2c1_Register, 1, I2C_FIRST_FRAME);
	}
	else
	{
		txcount = 0;
		HAL_I2C_Slave_Seq_Transmit_IT(hi2c, (uint8_t *)&CONTROL[i2c1_Register + txcount], 1, I2C_FIRST_FRAME);
	}
}

void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	txcount++;
	HAL_I2C_Slave_Seq_Transmit_IT(hi2c, (uint8_t *)&CONTROL[i2c1_Register + txcount], 1, I2C_NEXT_FRAME);
}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	HAL_I2C_Slave_Seq_Receive_IT(hi2c, (uint8_t *)&CONTROL[i2c1_Register + rxcount], 1, I2C_NEXT_FRAME);
	rxcount++;
}

void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c)
{
	HAL_I2C_EnableListen_IT(hi2c);
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
	if (HAL_I2C_GetError(hi2c) == 0x04)  // AF error
	{

	}

	/* BERR Error commonly occurs during the Direction switch
	 * Here we the software reset bit is set by the HAL error handler
	 * Before resetting this bit, we make sure the I2C lines are released and the bus is free
	 * I am simply reinitializing the I2C to do so
	 */
	else if (HAL_I2C_GetError(hi2c) == 0x01)  // BERR Error
	{
		HAL_I2C_DeInit(hi2c);
		HAL_I2C_Init(hi2c);
		rxcount = 0;  // reset the count
	}

	HAL_I2C_EnableListen_IT(hi2c);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	for(uint8_t i = 1; i < INA233_SIZE; i++)
	{
		MYGPIO_PinState_Update(&ALM_RELE[i]);
		FAULT_RELE[i].PinState = !(ALM_RELE[i].PinState);
		MYGPIO_PinState_Update(&FAULT_RELE[i]);
	}

	CONTROL[ALM_RELE_PinState] = (ALM_RELE[1].PinState << 0) + (ALM_RELE[2].PinState << 1) + (ALM_RELE[3].PinState << 2) + (ALM_RELE[4].PinState << 3);
	CONTROL[FAULT_RELE_PinState] = (FAULT_RELE[1].PinState << 0) + (FAULT_RELE[2].PinState << 1) + (FAULT_RELE[3].PinState << 2) + (FAULT_RELE[4].PinState << 3);

	EXT_INT.PinState = !(CONTROL[ALM_RELE_PinState]);
	MYGPIO_PinState_Update(&EXT_INT);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
