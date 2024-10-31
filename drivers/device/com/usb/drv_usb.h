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

#ifndef OS_DRIVERS_DEVICE_USB_DRV_USB_H_
#define OS_DRIVERS_DEVICE_USB_DRV_USB_H_

/*Configurations */
#include <app/bsw_config/arch_conf/board/board.h>
#include <bsw/utils/type_def/typedef_global.h>
/* kernel include for event usage */
#include <bsw/kernel/kernel.h>
/*IPC include */
#include <bsw/ipc/events/signal.h>
/* Global data include  */
#include <bsw/ipc/global/global_var.h>
/* Low level driver include */

/* driver level port include */
#include <bsw/drivers/device/driver_port.h>




/**************  API Export *****************/
#ifdef __cplusplus
extern "C" {
#endif

#if (INC_DRIVER_COMM_USB == 1)

drv_status_type			drv_usb_transmit(uint8_t dev_id, uint8_t* data, uint16_t len);


#else // If there is no inclusion give the warning
#warning "USB Driver Not included!!"
#endif

#ifdef __cplusplus
}
#endif
/**************  END API Export *****************/



#endif /* OS_DRIVERS_DEVICE_USB_DRV_USB_H_ */
