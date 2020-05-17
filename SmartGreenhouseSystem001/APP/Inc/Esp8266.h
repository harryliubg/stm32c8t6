#ifndef __ESP8266_H
#define __ESP8266_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "App.h"
#include "Debug.h"

#define AT          "AT\r\n"	
#define CWMODE      "AT+CWMODE=1\r\n"		//STA模式
#define CWDHCP 			"AT+CWDHCP=1,1\r\n"
#define CIFSR 			"AT+CIFSR\r\n" //查询IP地址
#define RST         "AT+RST\r\n"
#define CIFSR       "AT+CIFSR\r\n"
#define CWJAP       "AT+CWJAP=\"love123\",\"love123*\"\r\n"	//ssid: love123 密码：love123*
#define CIPSTART    "AT+CIPSTART=\"TCP\",\"183.230.40.33\",80\r\n"		//HTTP服务器183.230.40.33/80
#define CIPCLOSE    "AT+CIPCLOSE\r\n"		//断开服务器连接
#define CIPMODE0    "AT+CIPMODE=0\r\n"		//非透传模式
#define CIPMODE1    "AT+CIPMODE=1\r\n"		//透传模式
#define CIPSEND     "AT+CIPSEND\r\n"
#define CIPSTATUS   "AT+CIPSTATUS\r\n"		//网络状态查询

void Esp8266Rst(void);
void ESP8266_Init(void);
uint8_t SendCmd(char* cmd, char* result, int timeOut,uint8_t sendCount);
void ESP8266_SendData(uint8_t *data);
void ConnectSer(uint8_t sta);
void  ESP8266_EnterSendData(uint16_t len);
#ifdef __cplusplus
}
#endif
#endif 


