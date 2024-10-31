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

#ifndef OS_IPC_MQUEUE_MQUEUE_H_
#define OS_IPC_MQUEUE_MQUEUE_H_

/*Kernel functionality include */
#include <app/bsw_config/os_conf/os_config.h>
#include <app/bsw_config/arch_conf/board/board.h>
#include <bsw/kernel/kernel.h>
/* OS related configuration */
#include <bsw/utils/type_def/typedef_global.h>
#include <bsw/utils/ringbuffer/ringbuffer.h>




/* Exporting global queue handles */
#if(PERIPHERAL_CAN_1_EN == FLAG_ENABLE)
extern QueueHandle_t pipe_can_1_drv_rx_handle;
#endif

#if(PERIPHERAL_CAN_2_EN == FLAG_ENABLE)
extern QueueHandle_t pipe_can_2_drv_rx_handle;
#endif

#if(PERIPHERAL_CAN_3_EN == FLAG_ENABLE)
extern QueueHandle_t pipe_can_3_drv_rx_handle;
#endif

#if(PERIPHERAL_UART_1_EN == FLAG_SET)
extern struct ringbuffer	pipe_uart_1_drv_rx_handle;
extern struct ringbuffer	pipe_uart_1_drv_tx_handle;
#endif

#if(PERIPHERAL_USB_1_EN == FLAG_SET)
extern struct ringbuffer	pipe_usb_1_drv_rx_handle;
#endif

#if(INC_SERVICE_CAN_MGMT == FLAG_SET)
extern QueueHandle_t pipe_can_app_tx_handle;
extern QueueHandle_t pipe_can_app_rx_handle;
#endif


#if(INC_SERVICE_IIC_MGMT == FLAG_SET)
extern QueueHandle_t	pipe_iic_pdu_tx_handle;
extern QueueHandle_t	pipe_iic_pdu_rx_handle;
#endif

extern QueueHandle_t	pipe_diagnostics_handle;

/**************  API Export *****************/
#ifdef __cplusplus
extern "C" {
#endif

kernel_status_type 		ipc_mqueue_init(void);


#ifdef __cplusplus
}
#endif
/**************  END API Export *****************/



#endif /* OS_IPC_MQUEUE_MQUEUE_H_ */
