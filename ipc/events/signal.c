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

#include "signal.h"


/* Driver related events */
/* UART driver events */
static StaticEventGroup_t ipc_event_drv_uart_buff;
static EventGroupHandle_t ipc_event_drv_uart_handle;

static StaticEventGroup_t ipc_event_iic_buff;
static EventGroupHandle_t ipc_event_iic_handle;







/**
 * drv_serial_get_handle
 * @ret:
 *
 */
kernel_status_type ipc_signal_init(void)
{
	kernel_status_type status = KERNEL_OK;
	/* Initialize the driver related events */
	ipc_event_drv_uart_handle 	= xEventGroupCreateStatic(&ipc_event_drv_uart_buff);

	if(ipc_event_drv_uart_handle == NULL)
	{
		status |= KERNEL_ERR;
	}

	ipc_event_iic_handle 		= xEventGroupCreateStatic(&ipc_event_iic_buff);

	if(ipc_event_iic_handle == NULL)
	{
		status |= KERNEL_ERR;
	}
	/* Initialize the service relates events */

	/* Initialize the application events */
	return status;
}



/**
 * drv_serial_get_handle
 * @ret:
 *
 */
EventGroupHandle_t*	ipc_signal_get_event_handle(ipc_signal_list_type event_name)
{
	switch(event_name)
	{
		case IPC_EVENT_UART_DRV:
			return &ipc_event_drv_uart_handle;
		break;

		case IPC_EVENT_IIC_DRV:
			return &ipc_event_iic_handle;
		break;

		default:
			return NULL;
		break;
	}
}






