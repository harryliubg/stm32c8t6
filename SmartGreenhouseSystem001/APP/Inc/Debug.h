#ifndef __DEBUG_H
#define __DEBUG_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "App.h"


typedef struct UartSta{
	uint8_t RxData;
	uint8_t StartFlag;
	uint8_t EndFlag;
	uint16_t RxLength;
	uint8_t TimeCount;
}SysUartSta;

extern SysUartSta DebugUart;
extern SysUartSta WifiUart;

int Debug_printf(const char *pcFormat, ...);

#ifdef __cplusplus
}
#endif
#endif 

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
