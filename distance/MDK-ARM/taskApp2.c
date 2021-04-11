#include "taskApp2.h"

/* Private variables ---------------------------------------------------------*/
uint32_t IC_Vall = 0;
uint32_t IC_Val2 = 0;
uint32_t volatile Difference = 0;
uint8_t Is_First_capture = 0;
uint8_t volatile Distance = 2;
int volatile i = 0;
extern TIM_HandleTypeDef htim1;


#define TRIG_PIN  GPIO_PIN_9
#define TRIG_PORT GPIOA


//Credit is given to the author of this youtube video, who helped us in measuring distance using the sensor
//https://www.youtube.com/watch?v=iXbjAHdczJs
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

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef * htim)
{
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) //if the interrupt source is channel
	{
		input_capture ();
	}
}


void task_3_measure_distance(void)
{
	HCSR04_Read();
	ReRunMe(2);
}


/*task4 Buzz Sound*/
void task_4_Buzz_sound(void)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12,GPIO_PIN_SET);
	for (i=0; i<10000; i++){}
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12,GPIO_PIN_RESET);
  ReRunMe(Difference);

	//QueTask(task_3_measure_distance, 2);

}
