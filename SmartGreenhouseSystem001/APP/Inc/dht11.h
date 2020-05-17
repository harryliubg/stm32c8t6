#ifndef __DHT11_H
#define __DHT11_H
#include "App.h"



//IO��������											   
#define	DHT11_DQ_OUT    PBout(2)//���ݶ˿�	PB2
#define	DHT11_DQ_IN     PBin(2) //���ݶ˿�	PB2
   	
uint8_t DHT11_Init(void);//��ʼ��DHT11
uint8_t DHT11_Read_Data(uint8_t *temp,uint8_t *humi);//��ȡ��ʪ��
uint8_t DHT11_Read_Byte(void);//����һ���ֽ�
uint8_t DHT11_Read_Bit(void);//����һ��λ
uint8_t DHT11_Check(void);//����Ƿ����DHT11
void DHT11_Rst(void);//��λDHT11 
void humiCtrl(uint8_t mode);
void tempCtrl(uint8_t mode);
#endif
