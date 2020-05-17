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
							Debug_printf("打开加温设备\r\n");
							lightCtrlOldSta=sysValueSta.tempCtrl;
						}
					}
					else
					{
						sysValueSta.tempCtrl=0;
						if(lightCtrlOldSta!=sysValueSta.tempCtrl)
						{
							Debug_printf("关闭开加温设备\r\n");
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
							Debug_printf("打开加湿设备\r\n");
							humiCtrlOldSta=sysValueSta.humiCtrl;
						}						
					}
					else
					{
						sysValueSta.humiCtrl=0;
						if(humiCtrlOldSta!=sysValueSta.humiCtrl)
						{
							Debug_printf("关闭加湿设备\r\n");
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
							Debug_printf("打开光照设备\r\n");
							lightCtrlOldSta=sysValueSta.lightCtrl;
						}									
					}
					else
					{
						sysValueSta.lightCtrl=0;
						if(lightCtrlOldSta!=sysValueSta.lightCtrl)
						{
							Debug_printf("关闭光照设备\r\n");
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
		Debug_printf("读取失败\r\n");
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
		Debug_printf("device id读取成功\r\n");
		STMFLASH_Read_Data( (uint32_t)DEVICENAMEADDR, datatemp,len+1); 
		memcpy(sysValueSta.DeviID,datatemp+1,len);
		
		memset(datatemp,0,50);
		len=0;
		STMFLASH_Read_Data( (uint32_t)APIKEYNAMEADDR, &len,1); 
		if(len==0xff)
		{
			Debug_printf("apikey读取失败\r\n");
			memset(datatemp,0,50);
			memcpy(sysValueSta.ApiKey,APIKEY,strlen(APIKEY));
			len=strlen(APIKEY);
			datatemp[0]=len;
			memcpy(datatemp+1,APIKEY,len);
			STMFLASH_Write_Data( (uint32_t)APIKEYNAMEADDR,datatemp,len+1);
		}
		else
		{
			Debug_printf("apikey读取成功len=%d\r\n",len);
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
	DHT11_Read_Data(&sysValueSta.temp,&sysValueSta.humi);//温湿度数据获取
	if(sysValueSta.temp<TEMPTHRESHOLD)
	{
		Debug_printf("温度低过阈值\r\n");
		sysValueSta.tempWarnFlag=1;
	}
	else
	{
		sysValueSta.tempWarnFlag=0;
	}
	if(sysValueSta.humi<HUMITHRESHOLD)
	{
		Debug_printf("湿度低于阈值\r\n");
		sysValueSta.humiWarnFlag=1;
	}
	else
	{
		sysValueSta.humiWarnFlag=0;
	}
	lightSensorGet(&sysValueSta.lightInfo);//光照信息获取
	if(sysValueSta.lightInfo==0)
	{
		Debug_printf("光照超过阈值\r\n");
		sysValueSta.lightWarnFlag=1;
	}
	else
	{
		sysValueSta.lightWarnFlag=0;
	}
}
static void DeviceCtr(void)
{
	if(HAL_GPIO_ReadPin(KEY0_GPIO_Port,KEY0_Pin)==GPIO_PIN_RESET)//按键按下
	{
		if(sysValueSta.clearWarningFlag==0)
		{
			Debug_printf("清除报警标志位\r\n");
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
	buffInit();//缓冲区清空
	deviceInfoInit();//设备信息初始化
	sysValueSta.warningCtrl=0;//报警标志位清空
	sysValueSta.infoUpdateCount=0;//信息更新计数清零
	sysValueSta.lightCtrl=0;//光照控制器清零
	sysValueSta.tempCtrl=0;//温度控制器清零
	sysValueSta.humiCtrl=0;//湿度控制器清零
	sysValueSta.lightWarnFlag=0;//光照报警标记清零
	sysValueSta.tempWarnFlag=0;//温度报警标记清零
	sysValueSta.humiWarnFlag=0;//湿度报警标记清零
	sysValueSta.infoUpdateFlag=0;//定时信息更新标记位清空
	sysValueSta.infoUpdateStartFlag=0;//定时信息更新开始标记位清空
	sysValueSta.serverConnectFlag=0;//服务器连接标记位清零
	sysValueSta.serverStartConnectFlag=0;//开始连接服务器清零
	sysValueSta.getInfoFlag=0;//获取信息标记位清零
	sysValueSta.sendDataSta=0;//发送数据状态清零
	sysValueSta.enterSendDataSta=0;//进入发数据状态清零
	sysValueSta.reportInfoFlag=0;//上报信息标记位清零
	sysValueSta.SysUpdateInfosta=UpdateInit;//系统信息更新初始化状态
	sysValueSta.clearWarningFlag=0;//清除报警标记位
	oledInit();//显示初始化
	if(DHT11_Init()==0)//温湿度传感器初始化
	{
			Debug_printf("DHT11_Init ok\r\n");
	}
	else
		Debug_printf("DHT11_Init fail\r\n");
	ESP8266_Init();//ESP8266初始化
	oledInitSuc();//显示设备初始化成功信息
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
			Debug_printf("恢复报警标志位\r\n");
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
				infoReport(Txbuff);//信息上报
				datalen=strlen((char *)Txbuff);
				ESP8266_EnterSendData(datalen);//WiFi进入发送数据模式
			}
			else
			{
				if(sysValueSta.enterSendDataSta==1)
				{
					sysValueSta.enterSendDataSta=0;
					ESP8266_SendData(Txbuff);//向云平台发送数据
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
			Debug_printf("服务器应答超时，关闭服务器连接\r\n");
			sysValueSta.SysUpdateInfosta=updateTimeOut;			
			ConnectSer(0);//断开服务器连接
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
					infoGet(Txbuff);//数据获取组包
					datalen=strlen((char *)Txbuff);
					ESP8266_EnterSendData(datalen);//进入发送数据模式
				}
				else
				{
					if(sysValueSta.enterSendDataSta==1)
					{
						sysValueSta.enterSendDataSta=0;
						ESP8266_SendData(Txbuff);//发送数据
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
					infoReport(Txbuff);//上报信息组包
					datalen=strlen((char *)Txbuff);
					ESP8266_EnterSendData(datalen);//进入发送数据模式
				}
				else
				{
					if(sysValueSta.enterSendDataSta==1)
					{
						sysValueSta.enterSendDataSta=0;
						ESP8266_SendData(Txbuff);//发送数据
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
				Debug_printf("连接服务器\r\n");
				sysValueSta.serverStartConnectFlag=1;
				ConnectSer(1);//连接服务器
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
				Rxbuffnalysis((char *)(outputbuff2addr+2));			//数据解析	
			}
			else
			{
				Debug_printf("上报成功，关闭服务器\r\n");				
				ConnectSer(0);//关闭服务器
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
	if(sysValueSta.sensorInfoUpdateCount==500)//500ms更新一次传感器数据
	{
		sysValueSta.sensorInfoUpdateCount=0;
		sensorInfoGet();//传感器数据获取
		displayAPP();//LCD显示更新
	}
	DeviceCtr();//设备控制
	infoUpdate();//云平台数据更新
	WifiRxDataProcess();//WIFI接收数据更新
}

