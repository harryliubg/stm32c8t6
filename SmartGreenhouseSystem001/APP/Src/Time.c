#include "Time.h"
#include "App.h"


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(DebugUart.StartFlag==1)
	{
		DebugUart.TimeCount++;
		if(DebugUart.TimeCount==2)
		{
			//Debug_printf("³¬Ê±\r\n");
			DebugUart.TimeCount=0;
			DebugUart.EndFlag=1;
			DebugUart.StartFlag=0;
			inputbuff1addr[0]=DebugUart.RxLength/256;
			inputbuff1addr[1]=DebugUart.RxLength%256;
			if(strstr((char *)(inputbuff1addr+2),"device_id")||strstr((char *)(inputbuff1addr+2),"api_key"))
			{
				uartDataAnalyze(inputbuff1addr+2);
			}
			if(strstr((char *)(inputbuff1addr+2),"reboot"))
			{
				SoftReset();
			}
			if(inputbuff1addr==endbuff1addr)
				inputbuff1addr=Rxbuff1[0];
			else
				inputbuff1addr+=BUFF1MAXLEN;
		}
		if(DebugUart.TimeCount>100)
		{
			DebugUart.TimeCount=0;	
		}
	}
	if(WifiUart.StartFlag==1)
	{
		WifiUart.TimeCount++;
		if(WifiUart.TimeCount==2)
		{
			//Debug_printf("³¬Ê±\r\n");
			WifiUart.TimeCount=0;
			WifiUart.EndFlag=1;
			WifiUart.StartFlag=0;
			inputbuff2addr[0]=WifiUart.RxLength/256;
			inputbuff2addr[1]=WifiUart.RxLength%256;
			if(strstr((char *)(inputbuff2addr+2),"CONNECT"))
			{
				sysValueSta.serverConnectFlag=1;
				sysValueSta.serverStartConnectFlag=0;
			}
			if(strstr((char *)(inputbuff2addr+2),"CLOSED"))
			{
				sysValueSta.serverConnectFlag=0;
				sysValueSta.serverStartConnectFlag=0;
			}
			if(inputbuff2addr==endbuff2addr)
				inputbuff2addr=Rxbuff2[0];
			else
				inputbuff2addr+=BUFFMAXLEN;
		}
		if(WifiUart.TimeCount>100)
		{
			WifiUart.TimeCount=0;	
		}
	}
}

