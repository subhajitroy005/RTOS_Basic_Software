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

#include "serial_mgmt.h"

#if (INC_SERVICE_UART_MGMT == 1)

static uint8_t 							temp_byte_buffer 	= 0;
static int								temp_byte_count 	= 0;

/**
 * drv_serial_init
 * @ret:
 *
 */
static kernel_status_type service_serial_mgmt_init(void)
{
	global_service_status.uart_mgmt_service_status = KERNEL_OK;
	/* Variable initialization */

	/* driver initialization */
	if(drv_serial_init(SERIAL_DEBUG_COMM) != DRV_OK)
	{
		/* TODO implement the strategy for reinitialize */
		global_service_status.uart_mgmt_service_status |= KERNEL_ERR;
	}
	return (kernel_status_type)global_service_status.uart_mgmt_service_status;
}
/**
 * drv_serial_init
 * @ret:
 *
 */
void proc_service_serial_mgmt(void * parm)
{
	/* Start time offset */
	vTaskDelay(pdMS_TO_TICKS(TIME_OFFSET_SERIAL_MANAGEMENT));

	/* If the task not initialize properly suspend the task */
	if(service_serial_mgmt_init() != KERNEL_OK)
	{
		vTaskSuspend(NULL);
	}

	while(true)
	{
		/* Service serial management process */

#if(PERIPHERAL_UART_1_EN == FLAG_SET)
		/* Forward the incoming character from higher layer */
		temp_byte_count = ringbuffer_getchar(&pipe_uart_1_drv_tx_handle, &temp_byte_buffer);
					/* OS shell management send*/
		if(temp_byte_count != 0)
		{
			drv_serial_transmit(UART_1, &temp_byte_buffer, 1);
		}
#endif

	}
}

#endif













