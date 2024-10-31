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

#include "drv_usb.h"

#if (INC_DRIVER_COMM_USB == 1)

/**
  * @brief  Transmit one PDU to bus
  * @param  Device ID (which can handle)
  * @param  String
  * @param  length
  * @retval Driver operation status.
  */
drv_status_type drv_usb_transmit(uint8_t dev_id, uint8_t* data, uint16_t len)
{
	drv_status_type status = DRV_BUSY;

	/* Definition for FS and HS identification done in USB_DEVICE/Target/usbd_conf.h*/
#if defined(STM32H7A3xxQ)
	status =  CDC_Transmit_HS(data, len);
#endif

#if defined(STM32H743xx)
	status =  CDC_Transmit_FS(data, len);
#endif

	return status;
}


#endif
