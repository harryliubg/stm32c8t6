#include "lightSensor.h"

void lightSensorGet(uint8_t *lightInfo)
{
	if(HAL_GPIO_ReadPin(LIGHT_CHECK_GPIO_Port, LIGHT_CHECK_Pin)==GPIO_PIN_RESET)
	{
		*lightInfo=0;
	}
	else
	{
		*lightInfo=1;
	}
}

void lightCtrl(uint8_t mode)
{
	if(mode)
	{
		HAL_GPIO_WritePin(LIGHT_CTRL_GPIO_Port,LIGHT_CTRL_Pin, GPIO_PIN_RESET);
	}
	else
	{
		HAL_GPIO_WritePin(LIGHT_CTRL_GPIO_Port,LIGHT_CTRL_Pin, GPIO_PIN_SET);
	}
}

