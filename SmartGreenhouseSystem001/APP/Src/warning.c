#include "warning.h"


void warningCtrl(uint8_t mode)
{
	if(mode)
	{
		HAL_GPIO_WritePin(WARNING_CTRL_GPIO_Port,WARNING_CTRL_Pin, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(WARNING_CTRL_GPIO_Port,WARNING_CTRL_Pin, GPIO_PIN_RESET);
	}
}


