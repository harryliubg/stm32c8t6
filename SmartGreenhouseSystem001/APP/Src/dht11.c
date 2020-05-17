#include "dht11.h"
#include "delay.h"

//DHT11���ģʽ����
void DHT11_IO_OUT()	
{
	GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = TEMP_CHECK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(TEMP_CHECK_GPIO_Port, &GPIO_InitStruct);
}

//DHT11����ģʽ����
void DHT11_IO_IN()	
{
	GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = TEMP_CHECK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(TEMP_CHECK_GPIO_Port, &GPIO_InitStruct);
}

//��λDHT11
void DHT11_Rst(void)	   
{                 
	DHT11_IO_OUT(); 	//����Ϊ���
	DHT11_DQ_OUT=0; 	//����DQ
	delay_ms(20);    	//��������18ms
	DHT11_DQ_OUT=1; 	//DQ=1 
	delay_us(30);     	//��������20~40us
}

//�ȴ�DHT11�Ļ�Ӧ
//����1:δ��⵽DHT11�Ĵ���
//����0:����
uint8_t DHT11_Check(void) 	   
{   
	uint8_t retry=0;
	DHT11_IO_IN();      //����Ϊ���	 
	while (DHT11_DQ_IN&&retry<100)//DHT11������40~80us
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=100)return 1;
	else retry=0;
	while (!DHT11_DQ_IN&&retry<100)//DHT11���ͺ���ٴ�����40~80us
	{
		retry++;
		delay_us(1);
	};
	if(retry>=100)return 1;	    
	return 0;
}

//��DHT11��ȡһ��λ
//����ֵ��1/0
uint8_t DHT11_Read_Bit(void) 			 
{
 	uint8_t retry=0;
	while(DHT11_DQ_IN&&retry<100)//�ȴ���Ϊ�͵�ƽ
	{
		retry++;
		delay_us(1);
	}
	retry=0;
	while(!DHT11_DQ_IN&&retry<100)//�ȴ���ߵ�ƽ
	{
		retry++;
		delay_us(1);
	}
	delay_us(40);//�ȴ�40us
	if(DHT11_DQ_IN)return 1;
	else return 0;		   
}

//��DHT11��ȡһ���ֽ�
//����ֵ������������
uint8_t DHT11_Read_Byte(void)    
{        
	uint8_t i,dat;
	dat=0;
	for (i=0;i<8;i++) 
	{
   		dat<<=1; 
	    dat|=DHT11_Read_Bit();
    }						    
    return dat;
}

//��DHT11��ȡһ������
//temp:�¶�ֵ(��Χ:0~50��)
//humi:ʪ��ֵ(��Χ:20%~90%)
//����ֵ��0,����;1,��ȡʧ��
uint8_t DHT11_Read_Data(uint8_t *temp,uint8_t *humi)    
{        
 	uint8_t buf[5];
	uint8_t i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//��ȡ40λ����
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humi=buf[0];
			*temp=buf[2];
		}
	}else return 1;
	return 0;	    
}

//��ʼ��DHT11��IO�� DQ ͬʱ���DHT11�Ĵ���
//����1:������
//����0:����     	 
uint8_t DHT11_Init(void)
{
	uint8_t count=10;
		do
		{
			 DHT11_Rst();
			if(DHT11_Check()==0)
				break;
			delay_ms(10);
			count--;
		}while(count);
		if(count>0)
			return 0;
		else 
			return 1;
}

void tempCtrl(uint8_t mode)
{
	if(mode)
	{
		HAL_GPIO_WritePin(TEMP_CTRL_GPIO_Port,TEMP_CTRL_Pin, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(TEMP_CTRL_GPIO_Port,TEMP_CTRL_Pin, GPIO_PIN_RESET);
	}
}

void humiCtrl(uint8_t mode)
{
	if(mode)
	{
		HAL_GPIO_WritePin(HUMI_CTRL_GPIO_Port,HUMI_CTRL_Pin, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(HUMI_CTRL_GPIO_Port,HUMI_CTRL_Pin, GPIO_PIN_RESET);
	}
}
