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

#include "boot.h"

/* *****************************************************
 *
 * System level initialization here
 * 1. Device low level drivers
 * 2. Complex drivers
 *
 * *****************************************************/
/* Driver handles */
#if (INC_DRIVER_COMM_UART == 1)
static drv_serial_handle_type * serial_handle_ref;
#endif

#if (INC_DRIVER_COMM_SPI == 1)
static drv_spi_handle_type * spi_handle_ref;
#endif

#if (INC_DRIVER_COMM_IIC == 1)
static drv_iic_handle_type*	 iic_handle_ref;
#endif

#if (INC_DRIVER_COMM_CAN == 1)
static drv_can_handle_type* can_handle_ref;
#endif


#if (INC_DRIVER_TIMER == 1)
static drv_timer_handle_type * timer_handle_ref;
#endif

static drv_wdg_handle_type * drv_wdg_handle_ref;
/**
 * boot_entry - take the defined handles from the main entry and initialize the device peripherals
 * @handle_list: pointer to the kernel defines handles
 *
 */
void boot_entry(low_level_driver_handles * handle_list){
	/* Drivers initialization [ Update the driver handles
	 * as the peripherals are initialized at low level ]  */
#if (INC_DRIVER_COMM_UART == 1)
	serial_handle_ref = drv_serial_get_handle();
	for (int i = 0 ; i < CONFIG_MCU_NO_OF_UART_PERIPHERAL ; i++)
	{
		serial_handle_ref->handle[i] = handle_list->uart_handle[i];
	}
#endif

#if (INC_DRIVER_COMM_SPI == 1)
	spi_handle_ref = drv_spi_get_handle();
	for (int i = 0 ; i < CONFIG_MCU_NO_OF_SPI_PERIPHERAL ; i++)
	{
		spi_handle_ref->handle[i] = handle_list->spi_handle[i];
	}
#endif



#if (INC_DRIVER_COMM_IIC == 1)
	iic_handle_ref = drv_iic_get_handle();
	for (int i = 0 ; i < CONFIG_MCU_NO_OF_IIC_PERIPHERAL ; i++)
	{
		iic_handle_ref->handle[i] = handle_list->iic_handle[i];
	}
#endif

#if (INC_DRIVER_COMM_CAN == 1)
	can_handle_ref = drv_can_get_handle();
	for (int i = 0 ; i < CONFIG_MCU_NO_OF_CAN_PERIPHERAL ; i++)
	{
		can_handle_ref->handle[i] = handle_list->can_handle[i];
	}
#endif

#if(INC_DRIVER_TIMER == 1)
	timer_handle_ref = drv_timer_get_handle();
	for (int i = 0 ; i < CONFIG_MCU_NO_OF_TIMER_PERIPHERAL ; i++)
	{
		timer_handle_ref->timer_handle[i] 	= handle_list->timer_handle[i];
		timer_handle_ref->timer_type[i]		= handle_list->timer_type[i];
	}
#endif

	drv_wdg_handle_ref = drv_wdg_get_handle();

	drv_wdg_handle_ref->handle = handle_list->wdg_handle;

	/* Initialize the Service */
	kernel_status_type status = service_init();
	/* is status is ok and manual tests are not
	 * enabled then Start the kernel */
	/* IF the manual tests are not enabled */

	if(status == KERNEL_OK)
	{
		service_start_kernel();
	}
	/* Program shouldn't go after that  */
	while(true)
	{

	}

}



