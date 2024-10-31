/*
# Copyright (C) 2024 Subhajit Roy
# This file is part of RTOS Basic Software
#
# RTOS Basic Software is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# RTOS Basic Software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
*/

#include "drv_flash.h"


/* Local variables */
static FLASH_EraseInitTypeDef erase_init_struct;



static void     DrvNvmInit  (void);
static uint32_t DrvNvmRead  (uint32_t start, uint8_t *buffer, uint32_t size);
static uint32_t DrvNvmWrite (uint32_t start, uint8_t *buffer, uint32_t size);





const CO_IF_NVM_DRV co_drv_flash = {
    DrvNvmInit,
    DrvNvmRead,
    DrvNvmWrite
};



static void DrvNvmInit(void)
{

}

static uint32_t DrvNvmRead(uint32_t start, uint8_t *buffer, uint32_t size)
{
    uint32_t idx = 0;
    idx = 0;

    return (idx);
}

static uint32_t DrvNvmWrite(uint32_t start, uint8_t *buffer, uint32_t size)
{
    uint32_t idx = 0;
    uint32_t pos;



    return (idx);
}





int32_t drv_flash_read_data (uint32_t start_page_address, uint8_t * data_ptr, int32_t no_of_bytes)
{
	int recv_count = 0;

	for(int i=0; i< no_of_bytes; ++i )
	{
		recv_count++;
		*(data_ptr++) = *(__IO uint8_t *)(start_page_address++) ;
	}

	return recv_count;
}



int32_t drv_flash_write_data (uint32_t start_page_address, uint8_t * data, int32_t no_of_bytes)
{
	int32_t		no_of_double_words 	= (no_of_bytes / 8);
	uint64_t 	double_word 		= 0;
	uint32_t 	PAGEError;
	int 		no_of_written 		= 0;

	/* Unlock the Flash to enable the flash control register access *************/
	HAL_FLASH_Unlock();
    /* Erase the user Flash area*/
	uint32_t end_p_addr = start_page_address + no_of_bytes;

	/* Fill EraseInit structure*/
#if defined(STM32F429xx)
#define FLASH_PAGE_SIZE 2048 // Page size in bytes (2 KB)

	erase_init_struct.TypeErase   		= FLASH_TYPEERASE_SECTORS;
	erase_init_struct.Banks 			= start_page_address;
	erase_init_struct.NbSectors     	= ((start_page_address - end_p_addr)/FLASH_PAGE_SIZE) +1;
#endif
//	if (FLASH_ErasePage(&erase_init_struct, &PAGEError) != HAL_OK)
//	{
//	 /*Error occurred while page erase.*/
//	  return HAL_FLASH_GetError();
//	}
//	for(int i =0; i<no_of_bytes; ++i )
//	{
//		FLASH_ErasePage(start_page_address++);
//	}



    /* Program the user Flash area word by double_word*/

	for(int i = 0; i < no_of_double_words; ++i)
	{
		double_word = 0;
		double_word = ((double*)data)[i];
#if defined(STM32F429xx) || defined(STM32L431xx) || defined(STM32F765xx)
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, start_page_address, double_word) == HAL_OK)
		{
			start_page_address 	+= 8;  // use StartPageAddress += 2 for half word and 8 for double word
			no_of_written 		+= 8;
		}
#endif
#if defined (STM32H7A3xxQ) || defined (STM32H743xx)
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, start_page_address, double_word) == HAL_OK)
		{
			start_page_address 	+= 4;  // use StartPageAddress += 2 for half word and 8 for double word
			no_of_written 		+= 4;
		}
#endif
		else
		{
		/* Error occurred while writing data in Flash memory*/
		 return HAL_FLASH_GetError ();
		}
	}

	/* Lock the Flash to disable the flash control register access (recommended
	  to protect the FLASH memory against possible unwanted operation) *********/
	HAL_FLASH_Lock();

	return no_of_written;
}
