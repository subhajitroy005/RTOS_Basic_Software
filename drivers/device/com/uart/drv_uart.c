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

#include "drv_uart.h"

#if (INC_DRIVER_COMM_UART == 1)

/* Serial driver handle */
static drv_serial_handle_type 	serial_handle;

/* Operational Variables*/
static uint8_t 					temp_rx_char_buff;
static uint32_t					uart_counter = 0;

/**
  * @brief  Get the driver level handles to boot
  * 		and pass the low level handles.
  * @param  None
  * @retval pointer to can driver handle
  */
drv_serial_handle_type * drv_serial_get_handle(void)
{
	return &serial_handle;
}

/**
  * @brief  Initialize the driver
  * 		if the driver already initialized at low level
  * 		don't init it again..
  * @param  None
  * @retval Driver operation status.
  */
drv_status_type drv_serial_init(uint8_t device_id)
{
	drv_status_type drv_staus = DRV_OK;

	/* Check if the handles are NULL or not */
	if(serial_handle.handle[device_id] != NULL)
	{
		global_drv_status.drv_serial[device_id].status = HAL_UART_GetState(serial_handle.handle[device_id]);
		/* Update the error state */
		global_drv_status.drv_serial[device_id].error_state = HAL_UART_GetError(serial_handle.handle[device_id]);

		/* check if the drivers are initialized properly or not */
		if(global_drv_status.drv_serial[device_id].error_state == HAL_OK)
		{
			/*Start the communication */
			drv_staus |= UART_Start_Receive_IT(serial_handle.handle[device_id], &temp_rx_char_buff, 1);
		}
	}

	return drv_staus;
}

/**
  * @brief  Transmit one PDU to bus
  * @param  Device ID (which can handle)
  * @param  Data array
  * @param  length
  * @retval Driver operation status.
  */
drv_status_type drv_serial_transmit(uint8_t dev_id, uint8_t* data, uint16_t len)
{
	drv_status_type status = DRV_OK;

	/* Send one by one character if the handle is not null */
	if(serial_handle.handle[dev_id] != NULL)
	{
		/* Timeout 10mS*/
		for (int i = 0; i<len; i++)
		{
			status |= HAL_UART_Transmit(serial_handle.handle[dev_id]
										,&data[i]
										,1
										,10
										);
		}
	}
	else
	{
		status |= DRV_ERR;
		uart_counter = uart_counter + 1;
		if(dev_id == UART_1){
			syscall_diagnostics_log(ERR_1300_UART1_TX, uart_counter);
		}
		else if (dev_id == UART_2){
			syscall_diagnostics_log(ERR_1305_UART2_TX, uart_counter);
		}
		else if (dev_id == UART_3){
			syscall_diagnostics_log(ERR_1310_UART3_TX, uart_counter);
		}
		else{
			syscall_diagnostics_log(ERR_1315_UART4_TX, uart_counter);
		}
	}

	return status;
}


/**
  * @brief Callbacks
  */

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	/* Not planned for the implementation */
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	/* Read the character*/
	HAL_UART_Receive_IT(huart, &temp_rx_char_buff, 1);

#if defined(SERIAL_DEBUG_COMM)
//	if(huart == serial_handle.handle[SERIAL_DEBUG_COMM])
//	{
#if(PERIPHERAL_UART_1_EN == 1)
		/* Put the character in queue [ Multiplexed here according the handle type */
		/* NO context switch will happen here so no thread safe needed */
		ringbuffer_putchar(&pipe_uart_1_drv_rx_handle, temp_rx_char_buff);
#endif
//	}
#else
#error "SERIAL_DEBUG_COMM Not defined please define in board.h"
#endif
}


void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{

}


#endif
