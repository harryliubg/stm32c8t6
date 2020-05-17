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
#define TEMPTHRESHOLD 10  //温度阈值
#define HUMITHRESHOLD 40  //湿度阈值

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
	updateSta SysUpdateInfosta;//系统信息更新状态
	uint8_t clearWarningFlag:1;//清除报警标记位
	uint8_t serverConnectFlag:1;//服务器连接标记位
	uint8_t enterSendDataSta:1;//进入发数据状态
	uint8_t sendDataSta:1;//发数据状态
	uint8_t getInfoFlag:1;//获取数据标志位
	uint8_t reportInfoFlag:1;//上报数据标志位
	uint8_t serverStartConnectFlag:1;//开始连接服务器标记位
	uint8_t infoUpdateFlag:1;//信息更新标志位
	uint8_t infoUpdateStartFlag:1;//信息更新标志位
	uint8_t lightInfo;//光照信息0：正常，1：异常
	uint8_t temp;//温度
	uint8_t humi;//湿度
	uint8_t lightWarnFlag:1;//光照报警标记
	uint8_t tempWarnFlag:1;//温度报警标记
	uint8_t humiWarnFlag:1;//湿度报警标记
	uint8_t lightCtrl:1;//光照控制器状态
	uint8_t tempCtrl:1;//温度控制器状态
	uint8_t humiCtrl:1;//湿度控制器状态
	uint8_t warningCtrl:1;//报警器控制状态
	uint16_t infoUpdateCount;//信息更新计数
	uint16_t infoUpdatetimeOutCount;//信息超时
	uint16_t sensorInfoUpdateCount;//传感器信息获取计数
	uint32_t clearWarnCount;//报警恢复计数
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
