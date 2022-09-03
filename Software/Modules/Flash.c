#include "Flash.h"

//双字读写
uint64_t qFlash_ReadData(uint32_t faddr)
{
	return *(__IO uint64_t*)faddr; 
}
void qFlash_WriteData(uint32_t addr,uint64_t *data,uint16_t num)
{
	HAL_FLASH_Unlock();
	for(uint16_t i=0;i<num;i++)
	{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, addr+8*i, data[i]);//第四步：写入数据
	}
	HAL_FLASH_Lock();
}

uint32_t qFlash_ErasePage(uint32_t PageNum)
{
	uint32_t pageError = 0;
	HAL_FLASH_Unlock();	
	FLASH_EraseInitTypeDef flash_erase;
	flash_erase.TypeErase = FLASH_TYPEERASE_PAGES;
	flash_erase.Page = PageNum;
	flash_erase.NbPages = 1;
	HAL_FLASHEx_Erase(&flash_erase,&pageError);
	HAL_FLASH_Lock();
	return pageError;
}
