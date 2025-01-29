/*
 * UltraSonic.c
 *
 */

#include "UltraSonic.h"
#include "main.h"
uint8_t DistanceCall()
{
	Ultra_Sonic *pUS;
	uint8_t dis=0;

	HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_RESET);
	usDelay(2);

	HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_SET);
	usDelay(10);
	HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_RESET);

	while(HAL_GPIO_ReadPin(ECHO_GPIO_Port, ECHO_Pin)==GPIO_PIN_RESET);
	pUS->numTicks=0;
	pUS->distance=0;

	while(HAL_GPIO_ReadPin(ECHO_GPIO_Port, ECHO_Pin)==GPIO_PIN_SET)
	{
		(pUS->numTicks)++;
		usDelay(1);
	};

	pUS->distance = (((pUS->numTicks)+0.0f)*0.034/2)*2.54;
	dis=(uint8_t)(pUS->distance);
	return dis;
}

void usDelay(uint8_t uSec)
{
	for(int i=0;i<uSec;i++);
}

