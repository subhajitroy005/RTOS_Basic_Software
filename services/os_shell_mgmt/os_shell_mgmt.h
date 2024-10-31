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

#ifndef OS_SERVICES_OS_SHELL_MGMT_OS_SHELL_MGMT_H_
#define OS_SERVICES_OS_SHELL_MGMT_OS_SHELL_MGMT_H_

/* Global typedef */
#include <app/bsw_config/os_conf/os_config.h>
#include <bsw/utils/type_def/typedef_global.h>
/* OS config files */
#include <bsw/kernel/kernel.h>
/*IPC include */
#include <bsw/ipc/events/signal.h>
#include <bsw/ipc/mqueue/mqueue.h>
#include <bsw/utils/ringbuffer/ringbuffer.h>
#include <app/shared_data/co_obj_ref.h>
/* Global data include  */
#include <bsw/ipc/global/global_var.h>
/* SW version purpose */
#include <app/app_config/app_version.h>
#include <bsw/drivers/device/cpu/drv_cpu.h>
/* Driver level include  */
#include <bsw/drivers/device/time/drv_time.h>

#define CMD_STRING_LENGTH 	20
#define PRINTK_BUFF_LENGTH  PIPE_UART_1_DRV_TX_SIZE
#define SUB_CMD_SELECT	  	0
#define	CMD_SELECT			1
#define CMD_RUNNING			1
#define CMD_NOT_RUNNING		0

/**************  API Export *****************/
#ifdef __cplusplus
extern "C" {
#endif


/* Proc OS Shell mgmt core */
void 	proc_service_os_shell_mgmt(void * parm);
int 	printk(uint8_t *ptr);

#ifdef __cplusplus
}
#endif
/**************  END API Export *****************/

#endif /* OS_SERVICES_OS_SHELL_MGMT_OS_SHELL_MGMT_H_ */
