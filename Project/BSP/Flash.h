#ifndef __Flash__
#define __Flash__

#include "main.h"
uint16_t MyFLASH_ReadHalfWord(uint32_t Address);//读-----0x08000000
void FLASH_programword(uint32_t page_address,uint64_t DATA_64);
void FLASH_ErasePage(uint8_t Addr);//擦除页

#endif
