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

#ifndef OS_UTILS_TYPE_DEF_TYPEDEF_GLOBAL_H_
#define OS_UTILS_TYPE_DEF_TYPEDEF_GLOBAL_H_

#include "macro_global.h"
#include <app/bsw_config/arch_conf/mcu/mcu_config.h>
#include <app/bsw_config/os_conf/os_config.h>
#include <bsw/kernel/COMM_STACK/canopen-stack/src/core/co_core.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>
/*Configuration include */
#include <bsw/drivers/device/driver_port.h>

/* task related status */
typedef int 	drv_status_type;
typedef uint8_t kernel_status_type;

typedef struct _diagnostics_type{
	uint32_t err_code;
	uint32_t timestamp;
	uint32_t count;

}diagnostics_type;


enum timer_types
{
	UPCOUNTER,
	DOWNCOUNTER,
	PWM_OP,
	INPUT_CAPTURE
};


/*###################  Driver #################
 * UART Driver
 * USB  Driver
 * IIC Driver
 * Time Driver
 * -------------------------------------------*/
/* driver level macro isolation  */
#if defined(STM32L431xx) || defined(STM32F765xx)
#define UART_DRV_HANDLE_TYPE 	UART_HandleTypeDef
#define IIC_DRV_HANDLE_TYPE		I2C_HandleTypeDef
#define	CAN_DRV_HANDLE_TYPE		CAN_HandleTypeDef
#define	WDG_DRV_HANDLE_TYPE		IWDG_HandleTypeDef
/* CAN RX FIFO abstruction */
#define CAN_RX_FIFO_0			CAN_FILTER_FIFO0
#define CAN_RX_FIFO_1			CAN_FILTER_FIFO0
#endif

#if defined (STM32H7A3xxQ) || defined(STM32H743xx)
#define UART_DRV_HANDLE_TYPE 	UART_HandleTypeDef
#define IIC_DRV_HANDLE_TYPE		I2C_HandleTypeDef
#define	CAN_DRV_HANDLE_TYPE		FDCAN_HandleTypeDef
#define	WDG_DRV_HANDLE_TYPE		IWDG_HandleTypeDef
#define USB_DRV_HANDLE_TYPE		USBD_HandleTypeDef
#define TIMER_HANDLE_TYPE		TIM_HandleTypeDef


#define DRV_TIMER_CHANNEL_1		TIM_CHANNEL_1
#define DRV_TIMER_CHANNEL_2		TIM_CHANNEL_2
#define DRV_TIMER_CHANNEL_3		TIM_CHANNEL_3
#define DRV_TIMER_CHANNEL_4		TIM_CHANNEL_4


/* CAN RX FIFO abstruction */
#define CAN_RX_FIFO_0			FDCAN_FILTER_TO_RXFIFO0
#define CAN_RX_FIFO_1			FDCAN_FILTER_TO_RXFIFO1
#endif

#if defined(STM32F429xx)
#define UART_DRV_HANDLE_TYPE 	UART_HandleTypeDef
#define IIC_DRV_HANDLE_TYPE		I2C_HandleTypeDef
#define	CAN_DRV_HANDLE_TYPE		CAN_HandleTypeDef
#define	WDG_DRV_HANDLE_TYPE		IWDG_HandleTypeDef
#define TIMER_HANDLE_TYPE		TIM_HandleTypeDef
#define SPI_HANDLE_TYPE			SPI_HandleTypeDef
/* CAN RX FIFO abstruction */
#define CAN_RX_FIFO_0			CAN_RX_FIFO0
#define CAN_RX_FIFO_1			CAN_RX_FIFO1

#define DRV_TIMER_CHANNEL_1			TIM_CHANNEL_1
#define DRV_TIMER_CHANNEL_2			TIM_CHANNEL_2
#define DRV_TIMER_CHANNEL_3			TIM_CHANNEL_3
#define DRV_TIMER_CHANNEL_4			TIM_CHANNEL_4

#endif



/*************************************************************
 * Func:
 * Desc:
 *
 * @parm:
 * @ret:
 *
 ************************************************************/
typedef struct _low_level_driver_handles
{

#if(CONFIG_MCU_NO_OF_UART_PERIPHERAL > 0)
	UART_DRV_HANDLE_TYPE 	* uart_handle[CONFIG_MCU_NO_OF_UART_PERIPHERAL];
#else
#warning "No UART peripheral included!"
#endif

#if(CONFIG_MCU_NO_OF_IIC_PERIPHERAL > 0)
	IIC_DRV_HANDLE_TYPE 	* iic_handle[CONFIG_MCU_NO_OF_IIC_PERIPHERAL];
#else
#warning "No IIC peripheral included!"
#endif

#if(CONFIG_MCU_NO_OF_CAN_PERIPHERAL > 0)
	CAN_DRV_HANDLE_TYPE 	* can_handle[CONFIG_MCU_NO_OF_CAN_PERIPHERAL];
#else
#warning "No CAN peripheral included!"
#endif


#if(CONFIG_MCU_NO_OF_TIMER_PERIPHERAL > 0)
	TIMER_HANDLE_TYPE		* timer_handle[CONFIG_MCU_NO_OF_TIMER_PERIPHERAL];
	uint8_t					timer_type[CONFIG_MCU_NO_OF_TIMER_PERIPHERAL];
#else
#warning "No Timer peripheral included!"
#endif

#if(CONFIG_MCU_NO_OF_SPI_PERIPHERAL > 0)
	SPI_HANDLE_TYPE		* spi_handle[CONFIG_MCU_NO_OF_SPI_PERIPHERAL];
#else
#warning "No SPI peripheral included!"
#endif

#if(CONFIG_MCU_WDG_EN == 1)
	WDG_DRV_HANDLE_TYPE 	* wdg_handle;
#endif

}low_level_driver_handles;


/*************************************************************
 * Func:        UART Driver
 * Desc:
 *
 * @parm:
 * @ret:
 *
 ************************************************************/
#if (INC_DRIVER_COMM_UART == 1)
typedef struct _drv_serial_handle_type
{
	UART_DRV_HANDLE_TYPE * handle[CONFIG_MCU_NO_OF_UART_PERIPHERAL];
	drv_status_type status[CONFIG_MCU_NO_OF_UART_PERIPHERAL];
}drv_serial_handle_type;
#endif

/*************************************************************
 * Func:        UART Driver
 * Desc:
 *
 * @parm:
 * @ret:
 *
 ************************************************************/
#if (INC_DRIVER_COMM_SPI == 1)
typedef struct _drv_spi_handle_type
{
	SPI_HANDLE_TYPE * handle[CONFIG_MCU_NO_OF_SPI_PERIPHERAL];
	drv_status_type   status[CONFIG_MCU_NO_OF_SPI_PERIPHERAL];
}drv_spi_handle_type;
#endif


/*************************************************************
 * Func: USB Driver
 * Desc:
 *
 * @parm:
 * @ret:
 *
 ************************************************************/
#if (INC_DRIVER_COMM_USB == 1)
typedef struct _drv_usb_handle_type
{
	USB_DRV_HANDLE_TYPE * handle;
}drv_usb_handle_type;
#endif

/*************************************************************
 * Func: IIC Driver
 * Desc:
 *
 * @parm:
 * @ret:
 *
 ************************************************************/
#if (INC_DRIVER_COMM_IIC == 1)
typedef struct _drv_iic_handle_type
{
	IIC_DRV_HANDLE_TYPE * handle[CONFIG_MCU_NO_OF_IIC_PERIPHERAL];
	drv_status_type status[CONFIG_MCU_NO_OF_IIC_PERIPHERAL];
}drv_iic_handle_type;
#endif

/*************************************************************
 * Func: Time Driver
 * Desc:
 *
 * @parm:
 * @ret:
 *
 ************************************************************/
/* Timer Handles generic to pwm / inout capture */
/*--------------------------------------------*/
#if(INC_DRIVER_TIMER == 1)
typedef struct _drv_timer_handle_type
{
	TIMER_HANDLE_TYPE		* timer_handle[CONFIG_MCU_NO_OF_TIMER_PERIPHERAL];
	uint8_t 				  timer_type[CONFIG_MCU_NO_OF_TIMER_PERIPHERAL];
	uint8_t					  timer_channel[CONFIG_MCU_NO_OF_TIMER_PERIPHERAL];
}drv_timer_handle_type;
#endif


/*************************************************************
 * Func:  CAN Driver
 * Desc:
 *
 * @parm:
 * @ret:
 *
 ************************************************************/
#if (INC_DRIVER_COMM_CAN == 1)
typedef struct _drv_can_handle_type
{
	CAN_DRV_HANDLE_TYPE * handle[CONFIG_MCU_NO_OF_CAN_PERIPHERAL];
	drv_status_type status[CONFIG_MCU_NO_OF_CAN_PERIPHERAL];
}drv_can_handle_type;
#endif

/*************************************************************
 * Func: GPIO Driver
 * Desc:
 *
 * @parm:
 * @ret:
 *
 ************************************************************/
typedef enum _drv_gpio_port_list
{
	DRV_GPIO_PORT_A,
	DRV_GPIO_PORT_B,
	DRV_GPIO_PORT_C,
	DRV_GPIO_PORT_D,
	DRV_GPIO_PORT_E,
#if defined (STM32H7A3xxQ) || defined(STM32H743xx)
	DRV_GPIO_PORT_F,
	DRV_GPIO_PORT_G,
	DRV_GPIO_PORT_H,
	DRV_GPIO_PORT_I,
	DRV_GPIO_PORT_J,
	DRV_GPIO_PORT_K
#endif
}drv_gpio_port_list_type;

/*************************************************************
 * Func: IWDG Driver
 * Desc:
 *
 * @parm:
 * @ret:
 *
 ************************************************************/
typedef struct _drv_wdg_handle_type
{
	WDG_DRV_HANDLE_TYPE * handle;
}drv_wdg_handle_type;

/*************************************************************
 * Func:
 * Desc:
 *
 * @parm:
 * @ret:
 *
 ************************************************************/
/*###################  IPC ####################
 * Queue
 * Stream Buffer
 * -------------------------------------------*/

/*--------------- Massage Queue --------------*/
typedef enum _ipc_stiatic_buff_list
{
	IPC_S_BUFF_DRV_UART_DEBUG_COMM_RX,
	IPC_S_BUFF_OS_SHELL_MGMT_RX,
	IPC_S_BUFF_OS_SHELL_MGMT_TX,
	IPC_S_BUFF_DRV_USB_HOST_COMM_RX,
	IPC_QUEUE_SERIAL_DEBUG_LOG,
	IPC_RING_BUFFER
}ipc_stiatic_buff_list_type;

typedef enum _ipc_mqueue_list
{
	PIPE_CAN_PDU_TX,
	PIPE_CAN_PDU_RX,
	PIPE_CAN_PDU_RX_2,
	PIPE_CAN_DRV_RX,
	PIPE_CAN_DRV_RX_2,
	PIPE_IIC_PDU_TX,
	PIPE_IIC_PDU_RX,
}ipc_mqueue_list_type;

/*--------------- Signals and events --------*/
typedef enum _ipc_signal_list
{
	IPC_EVENT_UART_DRV,
	IPC_EVENT_IIC_DRV
}ipc_signal_list_type;

/*#############################################*/




/*************************************************************
 * Func:
 * Desc:
 *
 * @parm:
 * @ret:
 *
 ************************************************************/
/*###################  Processes ###############
 * CAN_PDU_TX and RX
 *
 * -------------------------------------------*/
typedef struct _can_pdu_struct
{
	uint8_t 	can_dev_id;
	uint32_t	m_id;
	uint8_t		data[8];
	uint8_t		dlc;
	uint8_t		length;
	uint8_t		flag_rtr;
	uint8_t		tx_buffer_id;
	uint8_t		rx_buffer_id;
}can_pdu_struct_type;

typedef struct _i2c_pdu_struct
{
	uint8_t		device_id;
	uint32_t	slave_address;
	uint8_t 	iic_operation;
	uint8_t		reg_addr;
	uint8_t		txn_data[32];
	uint8_t		length;
	uint8_t		op_status;
	uint8_t		drv_status;
}i2c_pdu_struct_type;

typedef struct _spi_pdu_struct
{
	uint8_t		device_id;
	uint8_t		tx_data[32];
	uint8_t		rx_data[32];
	uint8_t		length;
	uint8_t		drv_status;
}spi_pdu_struct_type;

typedef enum _i2c_operation
{
	IIC_OPERATION_WRITE,
	IIC_OPERATION_READ,
	IIC_OPERATION_REG_WRITE,
	IIC_OPERATION_REG_READ
}i2c_operation_type;
/*------------------------------*/



/* Driver status */
typedef struct _global_drv_status_serial_type
{
    uint32_t status;
    uint32_t error_state;
}global_drv_status_serial_type;

typedef struct _global_drv_status_can_type
{
    uint32_t status;
    uint32_t error_state;
    uint32_t rx_message_counter;
    uint32_t tx_message_counter;

}global_drv_status_can_type;

typedef struct _global_drv_status_iic_type
{
    uint32_t status;
    uint32_t error_state;

}global_drv_status_iic_type;

/* Drivers related variables */
typedef struct _global_drv_status
{
	/*uart driver status */
	global_drv_status_serial_type	drv_serial[CONFIG_MCU_NO_OF_UART_PERIPHERAL];
	global_drv_status_can_type		drv_can[CONFIG_MCU_NO_OF_CAN_PERIPHERAL];
	global_drv_status_iic_type		drv_iic[CONFIG_MCU_NO_OF_IIC_PERIPHERAL];
}global_drv_status_type;

/*--------------- Debug status --------------*/

typedef struct _global_debug_status
{
	uint64_t   debug_global_en		   :	1;
	/*Service level debug*/
	uint64_t   debug_host_comm_mgmt_rx :    1;
	uint64_t   debug_host_comm_mgmt_tx :    1;
	uint64_t   can_mgmt_gw_status	   :    1;
	uint64_t   can_mgmt_gw_status_en   :    1;

}global_debug_status_type;

typedef struct _global_service_status_type
{
	uint8_t uart_mgmt_service_status;
	uint8_t iic_mgmt_service_status;
	uint8_t can_mgmt_service_status;
	uint8_t os_shell_mgmt_service_staus;
}global_service_status_type;


/*
* State machine general struct
* */
typedef struct _state_machine
{
	uint8_t state;
	uint8_t substate;
	uint8_t signal;
}state_machine_struct_type;
/*#############################################*/

/* CAN filter configuration type */
typedef struct _can_filter_type
{
	uint8_t		can_dev_id;
	uint8_t		filter_config_index;
	uint32_t	filter_id;
	uint32_t	filter_mask;
	uint32_t    filter_rx;
}can_filter_type;


typedef struct _canopen_handle_type
{
	uint8_t			co_node_is_active;
	CO_NODE* 		co_node_handle;
	CO_NODE_SPEC* 	co_node_slave_spec;
	uint32_t		co_node_init_state;
	uint8_t			co_no_of_tx_pdo;
	uint16_t		pdo_trigger_interval;
	uint64_t		pdo_trigger_time_stamp;
	void (*timer_cb_fnc)(void * arg);
}canopen_handle_type;

#endif /* OS_UTILS_TYPE_DEF_TYPEDEF_GLOBAL_H_ */
