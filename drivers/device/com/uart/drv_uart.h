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
#ifndef OS_DRIVERS_DEVICE_COM_UART_DRV_UART_H_
#define OS_DRIVERS_DEVICE_COM_UART_DRV_UART_H_
/*Configurations */
#include <app/bsw_config/arch_conf/board/board.h>
#include <app/bsw_config/os_conf/os_config.h>
#include <bsw/utils/type_def/typedef_global.h>
/* kernel include for event usage */
#include <bsw/kernel/kernel.h>
/*IPC include */
#include <bsw/ipc/events/signal.h>
#include <bsw/ipc/mqueue/mqueue.h>
#include <bsw/utils/ringbuffer/ringbuffer.h>
/* Global data include  */
#include <bsw/ipc/global/global_var.h>
/* Low level driver include */
#include <bsw/drivers/device/driver_port.h>
/*diagnostics*/
#include <bsw/utils/error_codes.h>
#include <bsw/services/diagnostics_mgmt/diagnostics_mgmt.h>


/**************  API Export *****************/
#ifdef __cplusplus
extern "C" {
#endif

#if (INC_DRIVER_COMM_UART == 1)

drv_serial_handle_type*	drv_serial_get_handle(void);
drv_status_type			drv_serial_init(uint8_t device_id);
drv_status_type			drv_serial_transmit(uint8_t dev_id, uint8_t* data, uint16_t len);

#else // If there is no inclusion give the warning
#warning "UART Driver Not included!!"
#endif

#ifdef __cplusplus
}
#endif
/**************  END API Export *****************/

#endif /* OS_DRIVERS_DEVICE_COM_UART_DRV_UART_H_ */
