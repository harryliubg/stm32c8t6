#include "App.h"


SysValueStr sysValueSta={0};
dataType datasta[5]={0};

uint8_t Rxbuff1[BUFF1NUM][BUFF1MAXLEN];
uint8_t *inputbuff1addr=Rxbuff1[0];
uint8_t *outputbuff1addr=Rxbuff1[0];
uint8_t *endbuff1addr=Rxbuff1[BUFF1NUM-1];

uint8_t Rxbuff2[BUFF2NUM][BUFFMAXLEN];
uint8_t *inputbuff2addr=Rxbuff2[0];
uint8_t *outputbuff2addr=Rxbuff2[0];
uint8_t *endbuff2addr=Rxbuff2[BUFF2NUM-1];

uint8_t Txbuff[BUFFMAXLEN];
uint8_t lightCtrlOldSta=0;
uint8_t tempCtrlOldSta=0;
uint8_t humiCtrlOldSta=0;
static  void Rxbuffnalysis(char *rxbuff)
{
	char *index=NULL;
	char *offset=NULL;
	uint8_t count=0,addrcount=0;
	memset(&datasta,0,10*sizeof(dataType));
	index=strstr(rxbuff,"count");
	if(index!=NULL)
	{
		offset=strstr(index,":");
		count=offset[1]-0x30;
		if(count>0)
		{
			for(uint8_t i=0;i<count;i++)
			{
				index=strstr(offset,"value");
				offset=strstr(index,"}");
				addrcount=offset-index-7;
				memcpy(datasta[i].value,index+7,addrcount);
				index=strstr(offset,":");
				offset=strstr(index,"}");
				addrcount=offset-index-3;
				memcpy(datasta[i].id,index+2,addrcount);
				Debug_printf("id:%s,value:%s\r\n",datasta[i].id,datasta[i].value);
				if(strstr(datasta[i].id,"tempCtrl"))
				{
					if(strstr(datasta[i].value,"1"))
					{
						sysValueSta.tempCtrl=1;
						if(lightCtrlOldSta!=sysValueSta.tempCtrl)
						{
							Debug_printf("�򿪼����豸\r\n");
							lightCtrlOldSta=sysValueSta.tempCtrl;
						}
					}
					else
					{
						sysValueSta.tempCtrl=0;
						if(lightCtrlOldSta!=sysValueSta.tempCtrl)
						{
							Debug_printf("�رտ������豸\r\n");
							lightCtrlOldSta=sysValueSta.tempCtrl;
						}
					}
				}
				else if(strstr(datasta[i].id,"humiCtrl"))
				{
					if(strstr(datasta[i].value,"1"))
					{
						sysValueSta.humiCtrl=1;
						if(humiCtrlOldSta!=sysValueSta.humiCtrl)
						{
							Debug_printf("�򿪼�ʪ�豸\r\n");
							humiCtrlOldSta=sysValueSta.humiCtrl;
						}						
					}
					else
					{
						sysValueSta.humiCtrl=0;
						if(humiCtrlOldSta!=sysValueSta.humiCtrl)
						{
							Debug_printf("�رռ�ʪ�豸\r\n");
							humiCtrlOldSta=sysValueSta.humiCtrl;
						}							
					}				
				}
				else if(strstr(datasta[i].id,"lightCtrl"))
				{
					if(strstr(datasta[i].value,"1"))
					{
						sysValueSta.lightCtrl=1;
						if(lightCtrlOldSta!=sysValueSta.lightCtrl)
						{
							Debug_printf("�򿪹����豸\r\n");
							lightCtrlOldSta=sysValueSta.lightCtrl;
						}									
					}
					else
					{
						sysValueSta.lightCtrl=0;
						if(lightCtrlOldSta!=sysValueSta.lightCtrl)
						{
							Debug_printf("�رչ����豸\r\n");
							lightCtrlOldSta=sysValueSta.lightCtrl;
						}								
					}				
				}				
			}
		}
	}
}

void buffInit(void)
{
	memset(Rxbuff1,0,BUFF1NUM*BUFF1MAXLEN);
	inputbuff1addr=Rxbuff1[0];
  outputbuff1addr=Rxbuff1[0];
	endbuff1addr=Rxbuff1[BUFF1NUM-1];
	memset(Rxbuff2,0,BUFF2NUM*BUFFMAXLEN);
	inputbuff2addr=Rxbuff2[0];
  outputbuff2addr=Rxbuff2[0];
	endbuff2addr=Rxbuff2[BUFF2NUM-1];
}


void buffClear(uint8_t  *buffAddr)
{
	memset(buffAddr,0,BUFFMAXLEN);
}
static void deviceInfoInit(void)
{
	uint8_t datatemp[50]={0};
	uint8_t len=0;
	STMFLASH_Read_Data( (uint32_t)DEVICENAMEADDR, &len,1); 
	if(len==0xff)
	{
		Debug_printf("��ȡʧ��\r\n");
		memcpy(sysValueSta.DeviID,DEVICEID,strlen(DEVICEID));
		len=strlen(DEVICEID);
		datatemp[0]=len;
		memcpy(datatemp+1,DEVICEID,len);
		STMFLASH_Write_Data( (uint32_t)DEVICENAMEADDR,datatemp,len+1);
		
		len=0;
		memset(datatemp,0,50);
		memcpy(sysValueSta.ApiKey,APIKEY,strlen(APIKEY));
		len=strlen(APIKEY);
		datatemp[0]=len;
		memcpy(datatemp+1,APIKEY,len);
		STMFLASH_Write_Data( (uint32_t)APIKEYNAMEADDR,datatemp,len+1);
		
	}
	else
	{
		Debug_printf("device id��ȡ�ɹ�\r\n");
		STMFLASH_Read_Data( (uint32_t)DEVICENAMEADDR, datatemp,len+1); 
		memcpy(sysValueSta.DeviID,datatemp+1,len);
		
		memset(datatemp,0,50);
		len=0;
		STMFLASH_Read_Data( (uint32_t)APIKEYNAMEADDR, &len,1); 
		if(len==0xff)
		{
			Debug_printf("apikey��ȡʧ��\r\n");
			memset(datatemp,0,50);
			memcpy(sysValueSta.ApiKey,APIKEY,strlen(APIKEY));
			len=strlen(APIKEY);
			datatemp[0]=len;
			memcpy(datatemp+1,APIKEY,len);
			STMFLASH_Write_Data( (uint32_t)APIKEYNAMEADDR,datatemp,len+1);
		}
		else
		{
			Debug_printf("apikey��ȡ�ɹ�len=%d\r\n",len);
			STMFLASH_Read_Data( (uint32_t)APIKEYNAMEADDR, datatemp,len+1); 
			memcpy(sysValueSta.ApiKey,datatemp+1,len);
		}
	}
	Debug_printf("device_id:%s\r\n",sysValueSta.DeviID);
	Debug_printf("APIkey:%s\r\n",sysValueSta.ApiKey);

}
static void OneNet_FillBuf(char *buf)
{

	char text[100];
	char buf1[128];

	memset(text, 0, sizeof(text));
	memset(buf1, 0, sizeof(buf1));

	strcpy(buf1, "{");

	memset(text, 0, sizeof(text));
//	sprintf(text, "\"GPS\":{\"lon\":%s,\"lat\":%s},\"temp\":%d,\"humi\":%d", lon, lat,Lierta_system.sys_Temper,Lierta_system.sys_Humidity);
	sprintf(text, "\"temp\":%d,\"humi\":%d,\"light\":%d",sysValueSta.temp,sysValueSta.humi,sysValueSta.lightInfo);
	strcat(buf1, text);

	strcat(buf1, "}");

	sprintf(buf, "POST /devices/%s/datapoints?type=3 HTTP/1.1\r\napi-key:%s\r\nHost:api.heclouds.com\r\nContent-Length:%d\r\n\r\n",

					sysValueSta.DeviID, sysValueSta.ApiKey, strlen(buf1));

	strcat(buf, buf1);
}
static void infoReport(uint8_t *databuff)
{
	memset(databuff,0,BUFFMAXLEN);
	OneNet_FillBuf((char *)databuff);
}
static void sensorInfoGet(void)
{
	DHT11_Read_Data(&sysValueSta.temp,&sysValueSta.humi);//��ʪ�����ݻ�ȡ
	if(sysValueSta.temp<TEMPTHRESHOLD)
	{
		Debug_printf("�¶ȵ͹���ֵ\r\n");
		sysValueSta.tempWarnFlag=1;
	}
	else
	{
		sysValueSta.tempWarnFlag=0;
	}
	if(sysValueSta.humi<HUMITHRESHOLD)
	{
		Debug_printf("ʪ�ȵ�����ֵ\r\n");
		sysValueSta.humiWarnFlag=1;
	}
	else
	{
		sysValueSta.humiWarnFlag=0;
	}
	lightSensorGet(&sysValueSta.lightInfo);//������Ϣ��ȡ
	if(sysValueSta.lightInfo==0)
	{
		Debug_printf("���ճ�����ֵ\r\n");
		sysValueSta.lightWarnFlag=1;
	}
	else
	{
		sysValueSta.lightWarnFlag=0;
	}
}
static void DeviceCtr(void)
{
	if(HAL_GPIO_ReadPin(KEY0_GPIO_Port,KEY0_Pin)==GPIO_PIN_RESET)//��������
	{
		if(sysValueSta.clearWarningFlag==0)
		{
			Debug_printf("���������־λ\r\n");
			sysValueSta.clearWarningFlag=1;
			sysValueSta.clearWarnCount=0;
		}
		
	}
	if(sysValueSta.lightWarnFlag==0&&sysValueSta.humiWarnFlag&&sysValueSta.tempWarnFlag==0)
	{
			warningCtrl(0);
			sysValueSta.warningCtrl=0;
	}
	else
	{
		if(sysValueSta.clearWarningFlag==0)
			warningCtrl(1);
		sysValueSta.warningCtrl=1;
	}
	if(sysValueSta.lightWarnFlag==1)
	{
		lightCtrl(1);
	}
	else
	{
		if(sysValueSta.lightCtrl)
				lightCtrl(1);
		else
				lightCtrl(0);
	}
	if(sysValueSta.humiWarnFlag==1)
	{
		humiCtrl(1);
	}
	else
	{
		if(sysValueSta.humiCtrl)
				humiCtrl(1);
		else
				humiCtrl(0);
	}
	if(sysValueSta.tempWarnFlag==1)
	{
		tempCtrl(1);
	}
	else
	{
		if(sysValueSta.tempCtrl)
				tempCtrl(1);
		else
				tempCtrl(0);
	}
}
void SysInit(void)
{
	buffInit();//���������
	deviceInfoInit();//�豸��Ϣ��ʼ��
	sysValueSta.warningCtrl=0;//������־λ���
	sysValueSta.infoUpdateCount=0;//��Ϣ���¼�������
	sysValueSta.lightCtrl=0;//���տ���������
	sysValueSta.tempCtrl=0;//�¶ȿ���������
	sysValueSta.humiCtrl=0;//ʪ�ȿ���������
	sysValueSta.lightWarnFlag=0;//���ձ����������
	sysValueSta.tempWarnFlag=0;//�¶ȱ����������
	sysValueSta.humiWarnFlag=0;//ʪ�ȱ����������
	sysValueSta.infoUpdateFlag=0;//��ʱ��Ϣ���±��λ���
	sysValueSta.infoUpdateStartFlag=0;//��ʱ��Ϣ���¿�ʼ���λ���
	sysValueSta.serverConnectFlag=0;//���������ӱ��λ����
	sysValueSta.serverStartConnectFlag=0;//��ʼ���ӷ���������
	sysValueSta.getInfoFlag=0;//��ȡ��Ϣ���λ����
	sysValueSta.sendDataSta=0;//��������״̬����
	sysValueSta.enterSendDataSta=0;//���뷢����״̬����
	sysValueSta.reportInfoFlag=0;//�ϱ���Ϣ���λ����
	sysValueSta.SysUpdateInfosta=UpdateInit;//ϵͳ��Ϣ���³�ʼ��״̬
	sysValueSta.clearWarningFlag=0;//����������λ
	oledInit();//��ʾ��ʼ��
	if(DHT11_Init()==0)//��ʪ�ȴ�������ʼ��
	{
			Debug_printf("DHT11_Init ok\r\n");
	}
	else
		Debug_printf("DHT11_Init fail\r\n");
	ESP8266_Init();//ESP8266��ʼ��
	oledInitSuc();//��ʾ�豸��ʼ���ɹ���Ϣ
	HAL_Delay(1000);
	sysValueSta.sensorInfoUpdateCount=0;
}
void infoUpdateTimeCallback(void)
{
	if(sysValueSta.infoUpdateStartFlag==1)
	{
		sysValueSta.infoUpdateCount++;
		if(sysValueSta.infoUpdateCount>60000)
			sysValueSta.infoUpdateCount=0;
		if(sysValueSta.infoUpdateCount==3000)
		{
			sysValueSta.infoUpdateFlag=1;
			sysValueSta.infoUpdateCount=0;
			sysValueSta.infoUpdateStartFlag=0;	
			sysValueSta.infoUpdatetimeOutCount=0;			
		}
	}
	if(sysValueSta.infoUpdateFlag==1)
	{
		sysValueSta.infoUpdatetimeOutCount++;
		if(sysValueSta.infoUpdatetimeOutCount>60000)
			sysValueSta.infoUpdatetimeOutCount=0;
	}
	sysValueSta.sensorInfoUpdateCount++;
	if(sysValueSta.sensorInfoUpdateCount>1000)
		sysValueSta.sensorInfoUpdateCount=0;
	if(sysValueSta.clearWarningFlag==1)
	{
		sysValueSta.clearWarnCount++;
		if(sysValueSta.clearWarnCount==120000)
		{
			sysValueSta.clearWarnCount=0;
			sysValueSta.clearWarningFlag=0;
			Debug_printf("�ָ�������־λ\r\n");
		}
		if(sysValueSta.clearWarnCount>120005)
			sysValueSta.clearWarnCount=0;
	}
}

static void OneNet_CheckDevDtatBuf(char *buf)
{
	sprintf(buf, "GET /devices/%s/datapoints?datastream_id=tempCtrl,humiCtrl,lightCtrl HTTP/1.1\r\napi-key:%s\r\nHost:api.heclouds.com\r\n\r\n",sysValueSta.DeviID, sysValueSta.ApiKey);
}
static  void infoGet(uint8_t *databuff)
{
	memset(databuff,0,BUFFMAXLEN);
	OneNet_CheckDevDtatBuf((char *)databuff);
}
static void infoUpdate(void)
{
	uint16_t datalen=0;
	if(sysValueSta.SysUpdateInfosta==UpdateInit)
	{
			if(sysValueSta.reportInfoFlag==0)
			{
				sysValueSta.reportInfoFlag=1;
				infoReport(Txbuff);//��Ϣ�ϱ�
				datalen=strlen((char *)Txbuff);
				ESP8266_EnterSendData(datalen);//WiFi���뷢������ģʽ
			}
			else
			{
				if(sysValueSta.enterSendDataSta==1)
				{
					sysValueSta.enterSendDataSta=0;
					ESP8266_SendData(Txbuff);//����ƽ̨��������
				}
				if(sysValueSta.sendDataSta==1)
				{
					sysValueSta.sendDataSta=0;
					sysValueSta.SysUpdateInfosta=GetInfo;
					sysValueSta.reportInfoFlag=0;
					sysValueSta.infoUpdateFlag=0;
					sysValueSta.infoUpdateCount=0;
					sysValueSta.infoUpdateStartFlag=1;						
				}				
	    }
	}
	if(sysValueSta.infoUpdateFlag==1)
	{
		if(sysValueSta.infoUpdatetimeOutCount==10000)
		{
			Debug_printf("������Ӧ��ʱ���رշ���������\r\n");
			sysValueSta.SysUpdateInfosta=updateTimeOut;			
			ConnectSer(0);//�Ͽ�����������
			return ;
		}
		if(sysValueSta.serverConnectFlag==1)
		{
			if(sysValueSta.SysUpdateInfosta==GetInfo)
			{
				if(sysValueSta.getInfoFlag==0)
				{
					Debug_printf("get\r\n");
					sysValueSta.getInfoFlag=1;
					infoGet(Txbuff);//���ݻ�ȡ���
					datalen=strlen((char *)Txbuff);
					ESP8266_EnterSendData(datalen);//���뷢������ģʽ
				}
				else
				{
					if(sysValueSta.enterSendDataSta==1)
					{
						sysValueSta.enterSendDataSta=0;
						ESP8266_SendData(Txbuff);//��������
					}
					if(sysValueSta.sendDataSta==1)
					{
						sysValueSta.sendDataSta=0;
						sysValueSta.SysUpdateInfosta=ReportInfo;
						sysValueSta.getInfoFlag=0;
						return ;
					}
				}
			}
			else if(sysValueSta.SysUpdateInfosta==ReportInfo)
			{
				if(sysValueSta.reportInfoFlag==0)
				{
					Debug_printf("report\r\n");
					Debug_printf("temp=%d\r\nhumi=%d\r\n",sysValueSta.temp,sysValueSta.humi);
					Debug_printf("lightInfo=%d\r\n",sysValueSta.lightInfo);
					sysValueSta.reportInfoFlag=1;
					infoReport(Txbuff);//�ϱ���Ϣ���
					datalen=strlen((char *)Txbuff);
					ESP8266_EnterSendData(datalen);//���뷢������ģʽ
				}
				else
				{
					if(sysValueSta.enterSendDataSta==1)
					{
						sysValueSta.enterSendDataSta=0;
						ESP8266_SendData(Txbuff);//��������
					}
					if(sysValueSta.sendDataSta==1)
					{
						sysValueSta.sendDataSta=0;
						sysValueSta.getInfoFlag=0;
						sysValueSta.SysUpdateInfosta=GetInfo;
						sysValueSta.reportInfoFlag=0;
						sysValueSta.infoUpdateFlag=0;
						sysValueSta.infoUpdateCount=0;
						sysValueSta.infoUpdateStartFlag=1;						
					}				
			  }
		  }
		}
		else
		{
			if(sysValueSta.serverStartConnectFlag==0)
			{
				Debug_printf("���ӷ�����\r\n");
				sysValueSta.serverStartConnectFlag=1;
				ConnectSer(1);//���ӷ�����
			}
		}
 }
}

static void WifiRxDataProcess(void)
{
	while(inputbuff2addr!=outputbuff2addr)
	{
		Debug_printf("%s\r\n",outputbuff2addr+2);
		if(strstr((char *)(outputbuff2addr+2),">"))
		{
			sysValueSta.enterSendDataSta=1;
		}
		if(strstr((char *)(outputbuff2addr+2),"HTTP/1.1"))
		{
			sysValueSta.sendDataSta=1;
			if(strstr((char *)(outputbuff2addr+2),"data"))
			{
				Debug_printf("get suc\r\n");	
				Rxbuffnalysis((char *)(outputbuff2addr+2));			//���ݽ���	
			}
			else
			{
				Debug_printf("�ϱ��ɹ����رշ�����\r\n");				
				ConnectSer(0);//�رշ�����
			}
		}
		if(sysValueSta.SysUpdateInfosta==updateTimeOut)
		{
			if(strstr((char *)(outputbuff2addr+2),"CLOSED")&&strstr((char *)(outputbuff2addr+2),"OK"))
			{
				sysValueSta.sendDataSta=0;
				sysValueSta.reportInfoFlag=0;
				sysValueSta.infoUpdateFlag=0;
				sysValueSta.getInfoFlag=0;
				sysValueSta.infoUpdateCount=0;
				sysValueSta.infoUpdatetimeOutCount=0;
				sysValueSta.infoUpdateStartFlag=1;	
				sysValueSta.SysUpdateInfosta=GetInfo;
			}
		}
		if(outputbuff2addr==endbuff2addr)
			outputbuff2addr=Rxbuff2[0];
		else
			outputbuff2addr+=BUFFMAXLEN;				
	}
}



void AppTask(void)
{
	if(sysValueSta.sensorInfoUpdateCount==500)//500ms����һ�δ���������
	{
		sysValueSta.sensorInfoUpdateCount=0;
		sensorInfoGet();//���������ݻ�ȡ
		displayAPP();//LCD��ʾ����
	}
	DeviceCtr();//�豸����
	infoUpdate();//��ƽ̨���ݸ���
	WifiRxDataProcess();//WIFI�������ݸ���
}

