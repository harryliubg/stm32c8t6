#ifndef __DHT11_H
#define __DHT11_H
#include "App.h"



//IO操作函数											   
#define	DHT11_DQ_OUT    PBout(2)//数据端口	PB2
#define	DHT11_DQ_IN     PBin(2) //数据端口	PB2
   	
uint8_t DHT11_Init(void);//初始化DHT11
uint8_t DHT11_Read_Data(uint8_t *temp,uint8_t *humi);//读取温湿度
uint8_t DHT11_Read_Byte(void);//读出一个字节
uint8_t DHT11_Read_Bit(void);//读出一个位
uint8_t DHT11_Check(void);//检测是否存在DHT11
void DHT11_Rst(void);//复位DHT11 
void humiCtrl(uint8_t mode);
void tempCtrl(uint8_t mode);
#endif
