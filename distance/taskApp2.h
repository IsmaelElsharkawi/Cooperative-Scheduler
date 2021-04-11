#ifndef TASKAPP2_H
#define TASKAPP2_H
#include "main.h"
#include "queue.h"
#include "taskApp2.h"
#include "stm32l4xx_it.h"

void task_3_measure_distance(void);
void task_4_Buzz_sound(void);

void delay (uint16_t time); //Micro Delay 
void HCSR04_Read (void);
void input_capture(void);
#endif
