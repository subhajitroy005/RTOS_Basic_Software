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

#include "service_core.h"

/* Status variables */
static kernel_status_type kernel_status;

/* Task Handles */
#if (INC_SERVICE_UART_MGMT == 1)
TaskHandle_t proc_handle_service_serial_mgmt = NULL;
StaticTask_t proc_buff_h_service_serial_mgmt;
StackType_t  proc_buff_service_serial_mgmt[ PROC_SERVICE_SERIAL_MGMT_STACK_SIZE ];
#endif

#if (INC_SERVICE_CAN_MGMT == 1)
TaskHandle_t proc_handle_service_can_mgmt = NULL;
StaticTask_t proc_buff_h_service_can_mgmt;
StackType_t  proc_buff_service_can_mgmt[ PROC_SERVICE_CAN_MGMT_STACK_SIZE ];
#endif

#if (INC_SERVICE_IIC_MGMT == 1)
TaskHandle_t proc_handle_service_iic_mgmt = NULL;
StaticTask_t proc_buff_h_service_iic_mgmt;
StackType_t  proc_buff_service_iic_mgmt[ PROC_SERVICE_IIC_MGMT_STACK_SIZE ];
#endif

#if (INC_SERVICE_OS_SHELL_MGMT == 1)
int test_cond;
TaskHandle_t proc_handle_service_os_shell_mgmt = NULL;
StaticTask_t proc_buff_h_service_os_shell_mgmt;
StackType_t  proc_buff_service_os_shell_mgmt[ PROC_SERVICE_OS_SHELL_MGMT_STACK_SIZE ];
#endif


#if (MANUAL_TEST_EN == 1)
TaskHandle_t test_suite_handle = NULL;
StaticTask_t test_suite_task;
StackType_t  test_suite_buffer[ TEST_SUITE_STACK_SIZE ];
#endif

static kernel_status_type register_service_tasks(void)
{
	kernel_status_type os_service_reg_status = KERNEL_OK;
#if ((INC_SERVICE_UART_MGMT == 1) && \
	 (RUN_UART_LOOPBACK_TEST == 0) \
	)
	/* Serial Management service proc */
	proc_handle_service_serial_mgmt = xTaskCreateStatic( proc_service_serial_mgmt,
												   "PROC_SER_SERV",
												   PROC_SERVICE_SERIAL_MGMT_STACK_SIZE,
												   NULL,
												   PROC_SERVICE_SERIAL_MGMT_PRIORITY,
												   proc_buff_service_serial_mgmt,
												   &proc_buff_h_service_serial_mgmt
	                							  );
	if( proc_handle_service_serial_mgmt == NULL )
	{
		/* Exception mechanism */
		os_service_reg_status |= KERNEL_ERR;
	}
	/* service status preset here will be set in service running */
	global_service_status.uart_mgmt_service_status = KERNEL_ERR;
#endif

#if ((INC_SERVICE_CAN_MGMT == 1) && \
	 (RUN_CAN_DRV_RX_TEST == 0) && \
	 (RUN_CAN_DRV_TX_TEST == 0) \
	)
	/*CAN management service proc */
	proc_handle_service_can_mgmt = xTaskCreateStatic( proc_service_can_mgmt,
													  "PROC_CAN_SERV",
													  PROC_SERVICE_CAN_MGMT_STACK_SIZE,
													  NULL,
													  PROC_SERVICE_CAN_MGMT_PRIORITY,
													  proc_buff_service_can_mgmt,
													  &proc_buff_h_service_can_mgmt
				    								);
	if(	proc_handle_service_can_mgmt == NULL)
	{
					/* Exception mechanism */
		os_service_reg_status |= KERNEL_ERR;
	}
	/* service status preset here will be set in service running */
	global_service_status.can_mgmt_service_status = KERNEL_ERR;
#endif

#if (INC_SERVICE_IIC_MGMT == 1)
	/*IIC management service proc */
	proc_handle_service_iic_mgmt = xTaskCreateStatic(
													proc_service_iic_mgmt,
													"PROC_IIC_SERV",
													PROC_SERVICE_IIC_MGMT_STACK_SIZE,
													NULL,
													PROC_SERVICE_IIC_MGMT_PRIORITY,
													proc_buff_service_iic_mgmt,
													&proc_buff_h_service_iic_mgmt
													);
	if(proc_handle_service_iic_mgmt == NULL)
	{
						/* Exception mechanism */
		os_service_reg_status |= KERNEL_ERR;
	}
	/* service status preset here will be set in service running */
	global_service_status.iic_mgmt_service_status = KERNEL_ERR;
#endif

#if (INC_SERVICE_OS_SHELL_MGMT == 1)

#if (MANUAL_TEST_EN == 1)
	test_cond = 1;
#else
	test_cond = 0;
#endif
	/* OS Shell  service proc */
	proc_handle_service_os_shell_mgmt = xTaskCreateStatic( 	proc_service_os_shell_mgmt,
															"PROC_OS_SHELL_SERV",
															PROC_SERVICE_OS_SHELL_MGMT_STACK_SIZE,
															&test_cond,
															PROC_SERVICE_OS_SHELL_MGMT_PRIORITY,
															proc_buff_service_os_shell_mgmt,
															&proc_buff_h_service_os_shell_mgmt
		            									 );
	if(proc_handle_service_os_shell_mgmt == NULL)
	{
			/* Exception mechanism */
		os_service_reg_status |= KERNEL_ERR;
	}
	/* service status preset here will be set in service running */
	global_service_status.os_shell_mgmt_service_staus = KERNEL_ERR;
#endif

#if (MANUAL_TEST_EN == 1 )
	/* Manual Test management  */
	test_suite_handle = xTaskCreateStatic( (TaskFunction_t)	ManualTestRunner,
											"TEST_SUITE",
											TEST_SUITE_STACK_SIZE,
											&test_cond,
											TEST_SUITE_PRIORITY,
											test_suite_buffer,
											&test_suite_task
										 );
	if(test_suite_handle == NULL)
	{
			/* Exception mechanism */
		os_service_reg_status |= KERNEL_ERR;
	}
#endif

	return os_service_reg_status;
}









/**
 * drv_serial_get_handle
 * @ret:
 *
 */
kernel_status_type service_init(void)
{
	kernel_status = KERNEL_OK;
	/* IPC related initialization need to be done first  */
	/* Events initialization */
	kernel_status |= ipc_signal_init();
	/* Message queue initialization */
	kernel_status |= ipc_mqueue_init();

	/* Register all the service task */
	kernel_status |= register_service_tasks();
	/* If the manual testing is running then
	 * don't run the application core */
	/* register app_tasks */
	kernel_status |= register_app_tasks();

	return kernel_status;
}

/**
 * drv_serial_get_handle
 * @ret:
 *
 */
void service_start_kernel(void)
{
	/*set the SVC handles to highest
	 * prio to overcome the error*/
	drv_cpu_SVC_prio_set();

	/*strat the tasks */
	vTaskStartScheduler();
}



