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

#ifndef BSW_DRIVERS_DEVICE_FLASH_DRV_FLASH_H_
#define BSW_DRIVERS_DEVICE_FLASH_DRV_FLASH_H_

/*Configurations */
#include <app/bsw_config/arch_conf/board/board.h>
#include <bsw/utils/type_def/typedef_global.h>
/* kernel include for event usage */
#include <bsw/kernel/kernel.h>
#include <bsw/kernel/COMM_STACK/canopen-stack/src/core/co_core.h>
/*IPC include */
#include <bsw/ipc/events/signal.h>
#include <bsw/ipc/mqueue/mqueue.h>
/* Global data include  */
#include <bsw/ipc/global/global_var.h>
/* Low level driver include */
#include <bsw/drivers/device/driver_port.h>
#include <app/bsw_config/os_conf/can_network_config.h>
#include <bsw/drivers/device/time/drv_time.h>
/**************  API Export *****************/

#define MIN_PAGE_ACCESS_ADDRESS 0x08080000
#define MAX_PAGE_ACCESS_ADDRESS 0x080801FF



extern const CO_IF_NVM_DRV co_drv_flash;


#ifdef __cplusplus
extern "C" {
#endif



int32_t 	drv_flash_read_data (uint32_t start_page_address, uint8_t * data_ptr, int32_t no_of_bytes);
int32_t 	drv_flash_write_data (uint32_t start_page_address, uint8_t * data, int32_t no_of_bytes);

#ifdef __cplusplus
}
#endif


#endif /* BSW_DRIVERS_DEVICE_FLASH_DRV_FLASH_H_ */
