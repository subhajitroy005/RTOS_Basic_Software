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

#ifndef OS_SERVICES_IIC_MGMT_IIC_MGMT_H_
#define OS_SERVICES_IIC_MGMT_IIC_MGMT_H_

/* Global typedef */
#include <app/bsw_config/arch_conf/board/board.h>
#include <bsw/utils/type_def/typedef_global.h>
/* kernel include for event usage */
#include <bsw/kernel/kernel.h>
/*IPC include */
#include <bsw/ipc/events/signal.h>
#include <bsw/ipc/mqueue/mqueue.h>
/* Global data include  */
#include <bsw/ipc/global/global_var.h>
/* Driver level init */
#include <bsw/drivers/device/com/iic/drv_iic.h>
#include <bsw/services/os_shell_mgmt/os_shell_mgmt.h>
#include <bsw/services/diagnostics_mgmt/diagnostics_mgmt.h>
#include <app/bsw_config/os_conf/os_config.h>
#include <bsw/utils/error_codes.h>
/**************  API Export *****************/
#ifdef __cplusplus
extern "C" {
#endif

#if (INC_SERVICE_IIC_MGMT == 1 )

#if (INC_DRIVER_COMM_IIC == 0)
#error "IIC Management service include but driver disabled!!"
#endif

/* Proc iic mgmt core */
void proc_service_iic_mgmt(void * parm);

#else // If there is no inclusion give the warning
#warning "IIC Management service Not included!!"
#endif

#ifdef __cplusplus
}
#endif
/**************  END API Export *****************/


#endif /* OS_SERVICES_IIC_MGMT_IIC_MGMT_H_ */
