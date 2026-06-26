#include "Flash.h"

uint16_t MyFLASH_ReadHalfWord(uint32_t Address)//读-----0x08000000
{
	return *((__IO uint16_t *)(Address));	//使用指针访问指定地址下的数据并返回
}


void FLASH_programword(uint32_t page_address,uint64_t DATA_64)
{
	HAL_FLASH_Unlock();
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_BSY);
    for(uint8_t i=0; i<30; i++)      //最多重复10次，如果仍然失败，则返回
    {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, page_address, DATA_64) == HAL_OK) break;
    }
	
	HAL_FLASH_Lock();
}


void FLASH_ErasePage(uint8_t Addr)//擦除页
{
    HAL_FLASH_Unlock();      // 解锁Flash
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t PageError=0;//擦除错误地址

    EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;//仅擦除页
    EraseInitStruct.Banks = FLASH_BANK_2;
    EraseInitStruct.Page = Addr;//注：该page为0-15页
    EraseInitStruct.NbPages = 1;//擦除一页
    for(uint8_t i=0; i<30; i++)      //最多重复10次，如果仍然失败，则返回
    {
        if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) == HAL_OK) break;
    }
    HAL_FLASH_Lock();
}
