#ifndef __STMSVE_H__
#define __STMSVE_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "stm32f1xx_hal.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////
//�û������Լ�����Ҫ����
#define STM32_FLASH_SIZE 	256 	 		//��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_WREN 	1              	//ʹ��FLASHд��(0��������;1��ʹ��)
#define FLASH_WAITETIME  	50000          	//FLASH�ȴ���ʱʱ��

//FLASH��ʼ��ַ
#define STM32_FLASH_BASE 0x08000000 		//STM32 FLASH����ʼ��ַ
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

u8 STMFLASH_GetStatus(void);				  //���״̬
u8 STMFLASH_WaitDone(u16 time);				  //�ȴ���������
u8 STMFLASH_ErasePage(u32 paddr);			  //����ҳ
u8 STMFLASH_WriteHalfWord(u32 faddr, u16 dat);//д�����
u16 STMFLASH_ReadHalfWord(u32 faddr);		  //��������  
void STMFLASH_WriteLenByte(u32 WriteAddr,u32 DataToWrite,u16 Len);	//ָ����ַ��ʼд��ָ�����ȵ�����
u32 STMFLASH_ReadLenByte(u32 ReadAddr,u16 Len);						//ָ����ַ��ʼ��ȡָ����������
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);		//��ָ����ַ��ʼд��ָ�����ȵ�����
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);   		//��ָ����ַ��ʼ����ָ�����ȵ�����

//����д��
void Test_Write(u32 WriteAddr,u16 WriteData);
#endif /* __STMFLASH_H__ */

