#include "Debug.h"
#include <stdarg.h>
#include "string.h"
SysUartSta DebugUart={0};
SysUartSta WifiUart={0};


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1)
	{
		if(DebugUart.StartFlag==0)
		{
			DebugUart.StartFlag=1;
			DebugUart.EndFlag=0;
			DebugUart.RxLength=0;
			DebugUart.TimeCount=0;
			memset(inputbuff1addr,0,BUFF1MAXLEN);
		}
		if(DebugUart.RxLength<BUFF1MAXLEN-2)
		{
			DebugUart.TimeCount=0;
			inputbuff1addr[DebugUart.RxLength+2]=DebugUart.RxData;
			DebugUart.RxLength++;
		}
		HAL_UART_Receive_IT(&huart1,&DebugUart.RxData,1);
	}
	else if(huart->Instance==USART2)
	{
		if(WifiUart.StartFlag==0)
		{
			WifiUart.StartFlag=1;
			WifiUart.EndFlag=0;
			WifiUart.RxLength=0;
			WifiUart.TimeCount=0;
			buffClear(inputbuff2addr);
		}
		if(WifiUart.RxLength<BUFFMAXLEN-2)
		{
			WifiUart.TimeCount=0;
			inputbuff2addr[WifiUart.RxLength+2]=WifiUart.RxData;
			WifiUart.RxLength++;
		}
		HAL_UART_Receive_IT(&huart2,&WifiUart.RxData,1);
	}
}


/******************************************************************************
 * @函数名:Debug_printf
 * @参数:同库函数"printf"用法，自动增加回车换行
 * @返回值:执行成功返回，不成功返回-1
******************************************************************************/
#define DEBUGBUFFMAX 1024
char debugbuff[DEBUGBUFFMAX]={0};
int Debug_printf(const char *pcFormat, ...)
{
    int iRet = 0;
    va_list list;
    memset(debugbuff,0,DEBUGBUFFMAX);
    va_start(list,pcFormat);
    iRet = vsnprintf(debugbuff,DEBUGBUFFMAX-2,pcFormat,list);
    va_end(list);
		//debugbuff[iRet]='\r';
   // debugbuff[iRet+1]='\n';
    HAL_UART_Transmit(&huart1,(uint8_t *)debugbuff,iRet,0xffff);
    return iRet;
}



