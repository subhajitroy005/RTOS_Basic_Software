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

#ifndef OS_SERVICES_SERVICE_CORE_H_
#define OS_SERVICES_SERVICE_CORE_H_


/* Global typedef */
#include <app/bsw_config/os_conf/os_config.h>
#include <bsw/utils/type_def/typedef_global.h>
/* OS config files */
#include <bsw/kernel/kernel.h>
/*IPC include */
#include <bsw/ipc/events/signal.h>
/* Global data include  */
#include <bsw/ipc/global/global_var.h>
/* Utilities include */

/* CPU drivers */
#include <bsw/drivers/device/cpu/drv_cpu.h>

/*All services include */
#include <bsw/services/serial_mgmt/serial_mgmt.h>
#include <bsw/services/iic_mgmt/iic_mgmt.h>
#include <bsw/services/can_mgmt/can_mgmt.h>
#include <bsw/services/os_shell_mgmt/os_shell_mgmt.h>
#include <test/manual_test/test_suite.h>
/* Application includes */
#include <app/app_core/app_core.h>




/**************  API Export *****************/
#ifdef __cplusplus
extern "C" {
#endif


/* Entry Point of the OS and APP */
kernel_status_type	service_init(void);
void				service_start_kernel(void);


#ifdef __cplusplus
}
#endif
/**************  END API Export *****************/



#endif /* OS_SERVICES_SERVICE_CORE_H_ */
