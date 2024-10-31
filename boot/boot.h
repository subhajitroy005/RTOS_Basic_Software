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

#ifndef _BOOT_H_
#define _BOOT_H_
/* Device drivers include for getting the handles*/
#include <app/bsw_config/arch_conf/board/board.h>
#include <app/bsw_config/os_conf/os_config.h>
#include <bsw/drivers/device/com/uart/drv_uart.h>
#include <bsw/drivers/device/com/iic/drv_iic.h>
#include <bsw/drivers/device/com/can/drv_can.h>
#include <bsw/drivers/device/com/spi/drv_spi.h>

#include <bsw/services/service_core.h>
#include <bsw/utils/type_def/macro_global.h>
#include <bsw/utils/type_def/typedef_global.h>

#include <test/manual_test/test_suite.h>
/* Typedef of LOW level driver handles list */
/* Here we have accommodate all the handles as per of the requirements */



/**************  API Export *****************/
#ifdef __cplusplus
extern "C" {
#endif


/* Entry Point of the OS and APP */
void boot_entry(low_level_driver_handles * handle_list);


#ifdef __cplusplus
}
#endif
/**************  END API Export *****************/
#endif
