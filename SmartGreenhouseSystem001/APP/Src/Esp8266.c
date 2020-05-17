#include "Esp8266.h"

void Esp8266Rst(void)
{
	HAL_GPIO_WritePin(GPIOA,WIFIRST_Pin, GPIO_PIN_RESET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(GPIOA,WIFIRST_Pin, GPIO_PIN_SET);
	HAL_Delay(2000);
}
/*
 *  @brief ESP8266ģ���ʼ��
 */
void ESP8266_Init(void)
{
	uint8_t count=0;
Esp8266StartInit:
		Esp8266Rst();
    SendCmd(AT, "OK", 1000,10);		//ģ����Ч�Լ��
    SendCmd(CWMODE, "OK", 2000,5);	//ģ�鹤��ģʽ
    SendCmd(CWDHCP, "OK", 2000,5);		//����DHCP
		HAL_UART_Transmit(&huart2, (uint8_t *)CWJAP, strlen((char *)CWJAP),0xffff);//������Ҫ���ӵ�WIFI�ȵ�SSID������
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
		if(SendCmd(CIFSR, "STAIP", 3000,20)==0)//��ѯ�Ƿ���IP
			goto Esp8266StartInit;
		if(SendCmd(CIPSTART, "CONNECT", 3000,10)==0) //���ӷ�����
			goto Esp8266StartInit;
		if(SendCmd(CIPSTATUS, "STATUS:3", 1000,5)==0)	//��ѯ�Ƿ���TCP����
			goto Esp8266StartInit;
		//ConnectSer(0);
    //SendCmd(CIPMODE1, "OK", 1000,5);	//����͸��ģʽ
		//SendCmd(CIPSEND, ">", 1000,5);	//����͸��ģʽ
}

void ConnectSer(uint8_t sta)
{
	if(sta)
	{
		HAL_UART_Transmit(&huart2, (uint8_t *)CIPSTART, strlen(CIPSTART),0xffff);//���ӷ�����
		Debug_printf("cmd:%s\r\n",CIPSTART);
	}
	else
	{
		HAL_UART_Transmit(&huart2, (uint8_t *)CIPCLOSE, strlen(CIPCLOSE),0xffff);//�Ͽ�������
		Debug_printf("cmd:%s\r\n",CIPCLOSE);
	}
}
//==========================================================
//	�������ƣ�	���뷢����״̬
//
//
//	��ڲ�����	��

//
//	���ز�����	��
//
//	˵����		
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
//	�������ƣ�	ESP8266_SendData
//
//	�������ܣ�	��������
//
//	��ڲ�����	data������
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void ESP8266_SendData(uint8_t *data)
{
		HAL_UART_Transmit(&huart2, (uint8_t *)data, strlen((char *)data),0xffff);//
		Debug_printf("data:%s\r\n",data);
}
/*
 *  @brief USART2���ڷ���AT������
 *  @para  cmd  AT����
 *  @para  result Ԥ�ڵ���ȷ������Ϣ
 *  @para  timeOut��ʱʱ��,ms
*  @para  sendCount,���ʹ���
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
        if((NULL != strstr((char *)outputbuff2addr+2, result)))	//�ж��Ƿ���Ԥ�ڵĽ��
        {
					//Debug_printf("Wifi ָ���OK\r\n");
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


