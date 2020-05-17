#ifndef __APP_H
#define __APP_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "string.h"
#include "main.h"
#include "usart.h"
#include "Esp8266.h"
#include "dht11.h"
#include "delay.h"
#include "lightSensor.h"
#include "warning.h"
#include "dataSave.h"
#include "flashaSve.h"
#include "oled.h"
#include <stdio.h>

#define BUFF1MAXLEN 200
#define BUFFMAXLEN 1024
#define BUFF1NUM 2
#define BUFF2NUM 3
#define DEVICEID "561687728"
#define APIKEY "sthcANARVrBthqNM=hWbQlxqsms="
#define TEMPTHRESHOLD 10  //�¶���ֵ
#define HUMITHRESHOLD 40  //ʪ����ֵ

extern uint8_t Rxbuff1[BUFF1NUM][BUFF1MAXLEN];
extern uint8_t *inputbuff1addr;
extern uint8_t *outputbuff1addr;
extern uint8_t *endbuff1addr;

extern uint8_t Rxbuff2[BUFF2NUM][BUFFMAXLEN];
extern uint8_t *inputbuff2addr;
extern uint8_t *outputbuff2addr;
extern uint8_t *endbuff2addr;

typedef enum sysInfoUpdate{
	UpdateInit=1,
	GetInfo,
	ReportInfo,
	updateTimeOut,

}updateSta;

typedef struct sysValue{
	updateSta SysUpdateInfosta;//ϵͳ��Ϣ����״̬
	uint8_t clearWarningFlag:1;//����������λ
	uint8_t serverConnectFlag:1;//���������ӱ��λ
	uint8_t enterSendDataSta:1;//���뷢����״̬
	uint8_t sendDataSta:1;//������״̬
	uint8_t getInfoFlag:1;//��ȡ���ݱ�־λ
	uint8_t reportInfoFlag:1;//�ϱ����ݱ�־λ
	uint8_t serverStartConnectFlag:1;//��ʼ���ӷ��������λ
	uint8_t infoUpdateFlag:1;//��Ϣ���±�־λ
	uint8_t infoUpdateStartFlag:1;//��Ϣ���±�־λ
	uint8_t lightInfo;//������Ϣ0��������1���쳣
	uint8_t temp;//�¶�
	uint8_t humi;//ʪ��
	uint8_t lightWarnFlag:1;//���ձ������
	uint8_t tempWarnFlag:1;//�¶ȱ������
	uint8_t humiWarnFlag:1;//ʪ�ȱ������
	uint8_t lightCtrl:1;//���տ�����״̬
	uint8_t tempCtrl:1;//�¶ȿ�����״̬
	uint8_t humiCtrl:1;//ʪ�ȿ�����״̬
	uint8_t warningCtrl:1;//����������״̬
	uint16_t infoUpdateCount;//��Ϣ���¼���
	uint16_t infoUpdatetimeOutCount;//��Ϣ��ʱ
	uint16_t sensorInfoUpdateCount;//��������Ϣ��ȡ����
	uint32_t clearWarnCount;//�����ָ�����
	char DeviID[30];
	char ApiKey[50];

}SysValueStr;

extern SysValueStr sysValueSta;
typedef struct data
{
	char id[20];
	char value[10];
}dataType;

void buffInit(void);
void buffClear(uint8_t  *buffAddr);
void infoUpdateTimeCallback(void);
void SysInit(void);
void AppTask(void);
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
