#ifndef FLASH_H
#define FLASH_H
#include "main.h"
#ifdef __cplusplus
extern "C" {
#endif
uint64_t qFlash_ReadData(uint32_t faddr);
void qFlash_WriteData(uint32_t addr,uint64_t *data,uint16_t num);
uint32_t qFlash_ErasePage(uint32_t PageNum);
#ifdef __cplusplus
}
#endif
#endif 
