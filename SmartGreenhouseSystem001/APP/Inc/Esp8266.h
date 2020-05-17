#ifndef __ESP8266_H
#define __ESP8266_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "App.h"
#include "Debug.h"

#define AT          "AT\r\n"	
#define CWMODE      "AT+CWMODE=1\r\n"		//STAģʽ
#define CWDHCP 			"AT+CWDHCP=1,1\r\n"
#define CIFSR 			"AT+CIFSR\r\n" //��ѯIP��ַ
#define RST         "AT+RST\r\n"
#define CIFSR       "AT+CIFSR\r\n"
#define CWJAP       "AT+CWJAP=\"love123\",\"love123*\"\r\n"	//ssid: love123 ���룺love123*
#define CIPSTART    "AT+CIPSTART=\"TCP\",\"183.230.40.33\",80\r\n"		//HTTP������183.230.40.33/80
#define CIPCLOSE    "AT+CIPCLOSE\r\n"		//�Ͽ�����������
#define CIPMODE0    "AT+CIPMODE=0\r\n"		//��͸��ģʽ
#define CIPMODE1    "AT+CIPMODE=1\r\n"		//͸��ģʽ
#define CIPSEND     "AT+CIPSEND\r\n"
#define CIPSTATUS   "AT+CIPSTATUS\r\n"		//����״̬��ѯ

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


