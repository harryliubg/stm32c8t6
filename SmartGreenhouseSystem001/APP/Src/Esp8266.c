#include "Esp8266.h"

void Esp8266Rst(void)
{
	HAL_GPIO_WritePin(GPIOA,WIFIRST_Pin, GPIO_PIN_RESET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(GPIOA,WIFIRST_Pin, GPIO_PIN_SET);
	HAL_Delay(2000);
}
/*
 *  @brief ESP8266模块初始化
 */
void ESP8266_Init(void)
{
	uint8_t count=0;
Esp8266StartInit:
		Esp8266Rst();
    SendCmd(AT, "OK", 1000,10);		//模块有效性检查
    SendCmd(CWMODE, "OK", 2000,5);	//模块工作模式
    SendCmd(CWDHCP, "OK", 2000,5);		//设置DHCP
		HAL_UART_Transmit(&huart2, (uint8_t *)CWJAP, strlen((char *)CWJAP),0xffff);//配置需要连接的WIFI热点SSID和密码
		Debug_printf("cmd:%s\r\n",CWJAP);
		count=200;
		while(count)
		{
			if((inputbuff2addr!=outputbuff2addr)&&(WifiUart.EndFlag==1))
			{
				Debug_printf("%s\r\n",outputbuff2addr+2);
				if(strstr((char *)outputbuff2addr+2,"GOT IP"))
				{
						if(outputbuff2addr==endbuff2addr)
							outputbuff2addr=Rxbuff2[0];
						else
							outputbuff2addr+=BUFFMAXLEN;					
						break;				
				}
				else
				{
						if(outputbuff2addr==endbuff2addr)
							outputbuff2addr=Rxbuff2[0];
						else
							outputbuff2addr+=BUFFMAXLEN;	
				}
		 }
			HAL_Delay(100);
			count--;
		}
		if(count==0)
			goto Esp8266StartInit;
		if(SendCmd(CIFSR, "STAIP", 3000,20)==0)//查询是否有IP
			goto Esp8266StartInit;
		if(SendCmd(CIPSTART, "CONNECT", 3000,10)==0) //连接服务器
			goto Esp8266StartInit;
		if(SendCmd(CIPSTATUS, "STATUS:3", 1000,5)==0)	//查询是否建立TCP连接
			goto Esp8266StartInit;
		//ConnectSer(0);
    //SendCmd(CIPMODE1, "OK", 1000,5);	//配置透传模式
		//SendCmd(CIPSEND, ">", 1000,5);	//进入透传模式
}

void ConnectSer(uint8_t sta)
{
	if(sta)
	{
		HAL_UART_Transmit(&huart2, (uint8_t *)CIPSTART, strlen(CIPSTART),0xffff);//连接服务器
		Debug_printf("cmd:%s\r\n",CIPSTART);
	}
	else
	{
		HAL_UART_Transmit(&huart2, (uint8_t *)CIPCLOSE, strlen(CIPCLOSE),0xffff);//断开服务器
		Debug_printf("cmd:%s\r\n",CIPCLOSE);
	}
}
//==========================================================
//	函数名称：	进入发数据状态
//
//
//	入口参数：	无

//
//	返回参数：	无
//
//	说明：		
//==========================================================
void  ESP8266_EnterSendData(uint16_t len)
{
	char cmdBuf[32];
	sysValueSta.sendDataSta=0;
	sysValueSta.enterSendDataSta=0;
	sprintf(cmdBuf, "AT+CIPSEND=%d\r\n", len);	
	HAL_UART_Transmit(&huart2, (uint8_t *)cmdBuf, strlen(cmdBuf),0xffff);
	Debug_printf("cmd:%s\r\n",cmdBuf);
}
//==========================================================
//	函数名称：	ESP8266_SendData
//
//	函数功能：	发送数据
//
//	入口参数：	data：数据
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void ESP8266_SendData(uint8_t *data)
{
		HAL_UART_Transmit(&huart2, (uint8_t *)data, strlen((char *)data),0xffff);//
		Debug_printf("data:%s\r\n",data);
}
/*
 *  @brief USART2串口发送AT命令用
 *  @para  cmd  AT命令
 *  @para  result 预期的正确返回信息
 *  @para  timeOut延时时间,ms
*  @para  sendCount,发送次数
 */
uint8_t SendCmd(char* cmd, char* result, int timeOut,uint8_t sendCount)
{
	while(inputbuff2addr!=outputbuff2addr)
		{
			if(outputbuff2addr==endbuff2addr)
				outputbuff2addr=Rxbuff2[0];
			else
				outputbuff2addr+=BUFFMAXLEN;	
		}					
	for(uint8_t i=0;i<sendCount;i++)
	{
			Debug_printf("cmd:%s\n",cmd);
			HAL_UART_Transmit(&huart2, (uint8_t *)cmd, strlen((char *)cmd),0xffff);
			HAL_Delay(timeOut);
			while(inputbuff2addr!=outputbuff2addr)
			{
			   Debug_printf("rsp:%s\n",outputbuff2addr+2);
        if((NULL != strstr((char *)outputbuff2addr+2, result)))	//判断是否有预期的结果
        {
					//Debug_printf("Wifi 指令发送OK\r\n");
					if(outputbuff2addr==endbuff2addr)
						outputbuff2addr=Rxbuff2[0];
					else
						outputbuff2addr+=BUFFMAXLEN;							
					while(inputbuff2addr!=outputbuff2addr)
					{
						Debug_printf("rsp:%s\n",outputbuff2addr+2);
						if(outputbuff2addr==endbuff2addr)
							outputbuff2addr=Rxbuff2[0];
						else
							outputbuff2addr+=BUFFMAXLEN;							
					}				
					return 1;
        }
				if(outputbuff2addr==endbuff2addr)
					outputbuff2addr=Rxbuff2[0];
				else
					outputbuff2addr+=BUFFMAXLEN;					
			}					
	}
	return 0;
}


