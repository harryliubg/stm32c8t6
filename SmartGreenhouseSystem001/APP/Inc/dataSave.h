#ifndef __DATASAVE__
#define __DATASAVE__
#include "main.h"
#include "string.h"
#include "flashaSve.h"
#include "App.h"
   
#define DEVICENAMEADDR 0X800F800
#define APIKEYNAMEADDR 0X800F860

void I2C_EEPROM_WriteData(uint16_t Addr, uint8_t Reg, uint8_t Value);   
HAL_StatusTypeDef I2C_EEPROM_WriteBuffer(uint16_t Addr, uint8_t Reg, uint16_t RegSize, uint8_t *pBuffer, uint16_t Length);   
uint8_t I2C_EEPROM_ReadData(uint16_t Addr, uint8_t Reg);   
HAL_StatusTypeDef I2C_EEPROM_ReadBuffer(uint16_t Addr, uint8_t Reg, uint16_t RegSize, uint8_t *pBuffer, uint16_t Length);   
void EEPROM_WriteData(uint16_t Addr, uint8_t Reg, uint8_t *DataBuff,uint16_t DataLen);

void uartDataAnalyze(uint8_t *databuff);
void SoftReset(void);






#endif



