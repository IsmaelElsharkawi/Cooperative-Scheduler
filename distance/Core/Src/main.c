/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "queue.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TRIG_PIN  GPIO_PIN_9
#define TRIG_PORT GPIOA
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
uint32_t IC_Vall = 0;
uint32_t IC_Val2 = 0;
uint32_t volatile Difference = 0;
uint8_t Is_First_capture = 0;
uint8_t volatile Distance = 2;
int volatile countMsec =0;
int volatile countSec= 0;
int volatile R = 0;
int volatile i = 0;
int volatile Sysflag = 1;
int volatile  inverseDistance = 10;
extern TIM_HandleTypeDef htim1;
extern q * delay_queue;


TIM_HandleTypeDef htim1;

/* USER CODE BEGIN PV */
void delay (uint16_t time); //Micro Delay 
void HCSR04_Read (void);
void input_capture(void);

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef * htim)
{
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) //if the interrupt source is channel
	{
		input_capture ();
	}
}


/*MicroSec Delay*/
void delay (uint16_t time)
{
	__HAL_TIM_SetCounter(&htim1,0);
	while (__HAL_TIM_GetCounter (&htim1) < time);
}

/*UltraSonic Read*/
void HCSR04_Read (void)
{
	HAL_GPIO_WritePin (TRIG_PORT, TRIG_PIN,GPIO_PIN_SET);    //PULL the Trig pin high
	delay(10); //wait for 10 MicroSec 
	HAL_GPIO_WritePin (TRIG_PORT, TRIG_PIN,GPIO_PIN_RESET);  //Pull the Trig pin Low 
	__HAL_TIM_ENABLE_IT(&htim1, TIM_IT_CC1);
	
}

/* HCSRO4_Read  */
void input_capture (void)
{
	if (Is_First_capture == 0) //if the first value is not captured 
	{
		IC_Vall = HAL_TIM_ReadCapturedValue (&htim1, TIM_CHANNEL_1); //(catch rising edge then read the first value)
		Is_First_capture = 1; //set the first capture as true 
		//change polarity to falling edge 
		__HAL_TIM_SET_CAPTUREPOLARITY (&htim1, TIM_CHANNEL_1,TIM_INPUTCHANNELPOLARITY_FALLING); //
	}
	else if (Is_First_capture == 1) //if the first is already capture
	{
		IC_Val2 = HAL_TIM_ReadCapturedValue(&htim1, TIM_CHANNEL_1); // read second
		__HAL_TIM_SET_COUNTER(&htim1,0); //reset counter
		
		if (IC_Val2 > IC_Vall)
		{
			Difference = IC_Val2 - IC_Vall;
		}
		else if (IC_Vall > IC_Val2)
		{
			Difference = (0xffff - IC_Vall) + IC_Val2;
		}
		
		Distance = Difference * 0.034/2;
		Is_First_capture = 0; //set it back to false 
		
		__HAL_TIM_SET_CAPTUREPOLARITY (&htim1, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
		__HAL_TIM_DISABLE_IT (&htim1, TIM_IT_CC1);
	}
}	

/*task3 measure distance*/ 
void task_4_Buzz_sound(void);


void task_3_measure_distance(void)
{
	//__disable_irq();
	HCSR04_Read();
	//inverseDistance = Distance;
	//QueTask(task_4_Buzz_sound,7);
	//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12,GPIO_PIN_SET);
	//for (i=0; i<10000; i++){}
	//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12,GPIO_PIN_RESET);
	//QueTask(task_4_Buzz_sound,0);
	//ReRunMe(2);
	//__enable_irq();
}


/*task4 Buzz Sound*/ 
void task_4_Buzz_sound(void)
{
	//__disable_irq();
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12,GPIO_PIN_SET);
	//Sysflag =1;
	for (i=0; i<10000; i++){}
	//HAL_Delay(100*inverseDistance);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12,GPIO_PIN_RESET);
	Sysflag=0;
  ReRunMe(Distance);
		
	QueTask(task_3_measure_distance, 1);
	//__enable_irq();
}


void task_5_systick(void)
	{
		
	//__disable_irq();	
	countMsec +=1;
	
	if ((countMsec %1000) == 0)
	{
		countMsec = 0; 
		//Sysflag =1;
			t * current_p;
			t * deleted;	
		
    current_p = delay_queue->head_of_queue;

    while(current_p!=NULL){
        current_p->delay -= 1;
        if(current_p->delay<=0)
				{
        	QueTask(current_p->f, current_p->priority);
        	delay_queue->head_of_queue = current_p->next_task;
					deleted = current_p;
					current_p = current_p->next_task;
					free (deleted);
        }
				else 
				{
					current_p = current_p->next_task;
				}
        
        
    }
//		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12,GPIO_PIN_RESET);
	}
	
	//__enable_irq();
}
/* Task 5 Run the Buzz Relative to the Distance */
/* void task_5_Run_Buzz(void)
{
}*/


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
/* USER CODE BEGIN PFP */

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
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
	
//	SysTick->LOAD = 32000000-1;
//	SysTick->CTRL = 7;
	
	
	HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_1);
	init();
	//QueTask(task_3_measure_distance, 2);
	QueTask(task_4_Buzz_sound, 0);
	
  
	/* USER CODE END 2 */


  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		//HCSR04_Read();
		Dispatch();
		//HCSR04_Read();

		//HAL_Delay (200);
    
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
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
  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 8-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 0xffff-1;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9|GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA9 PA12 */
  GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
