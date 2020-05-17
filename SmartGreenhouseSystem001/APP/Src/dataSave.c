#include "dataSave.h"



#define APIKEY "sthcANARVrBthqNM=hWbQlxqsms="
uint8_t dataSaveBuff[100]={0};
uint8_t Rxbuff[100]={0};

uint16_t tempdata[50]={0};
void STMFLASH_Write_Data( uint32_t WriteAddr, uint8_t * pBuffer, uint16_t NumToWrite )
{
    uint8_t len=0;
    memset(tempdata,0,50);
    for(uint8_t i=0;i<NumToWrite;i++)
    {
      tempdata[len]=pBuffer[i];
      i++;
      tempdata[len]=(tempdata[len]<<8)|pBuffer[i];
      len++;
    }
  //  Debug_printf("NumToWrite=%d,len=%d",NumToWrite,len);
    STMFLASH_Write(WriteAddr,tempdata,len);
}
void STMFLASH_Read_Data( uint32_t WriteAddr, uint8_t * pBuffer, uint16_t NumToRead)
{
    uint8_t len=0;uint8_t j=0;
    memset(tempdata,0,50);
    if(NumToRead%2==0)
      len=NumToRead/2; 
    else
      len=NumToRead/2+1; 
    STMFLASH_Read(WriteAddr,tempdata,len); 
    for(uint8_t i=0;i<len;i++)
    {
      pBuffer[j]=(tempdata[i]&0xff00)>>8;
      j++;
      pBuffer[j]=tempdata[i]&0xff;
      j++;
     // Debug_printf("0x%04x",tempdata[i]);
    }
}
void uartDataAnalyze(uint8_t *databuff)
{
	char *addr=NULL;char *offset=NULL;uint8_t len=0;
	addr=strstr((char *)databuff,"device_id");
	if(addr!=NULL)
	{
		offset=strstr(addr,":");
		addr=strstr(offset,"\r\n");
		len=addr-offset-1;
		dataSaveBuff[0]=len;
		memcpy(dataSaveBuff+1,offset+1,len);
		Debug_printf("len=%d\r\n%s",dataSaveBuff[0],dataSaveBuff+1);
		STMFLASH_Write_Data( (uint32_t)DEVICENAMEADDR,dataSaveBuff,len+1);
		Debug_printf("设备ID保存成功\r\n");
	}
	addr=strstr((char *)databuff,"api_key");
	if(addr!=NULL)
	{
		offset=strstr(addr,":");
		addr=strstr(offset,"\r\n");
		len=addr-offset-1;
		dataSaveBuff[0]=len;
		memcpy(dataSaveBuff+1,offset+1,len);
		Debug_printf("len=%d\r\n%s",dataSaveBuff[0],dataSaveBuff+1);
		STMFLASH_Write_Data( (uint32_t)APIKEYNAMEADDR,dataSaveBuff,len+1);
		Debug_printf("APIKEY保存成功\r\n");
	}

}

void SoftReset(void)
{
		Debug_printf("复位\r\n");
		__set_FAULTMASK(1); // 关闭所有中断
    NVIC_SystemReset(); // 复位
}

