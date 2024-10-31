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

#ifndef BSW_SERVICES_DIAGNOSTICS_MGMT_DIAGNOSTICS_MGMT_H_
#define BSW_SERVICES_DIAGNOSTICS_MGMT_DIAGNOSTICS_MGMT_H_
/* Global typedef */
#include <app/bsw_config/arch_conf/board/board.h>
#include <bsw/utils/type_def/typedef_global.h>
#include <app/bsw_config/os_conf/os_config.h>
/* kernel include for event usage */
#include <bsw/kernel/kernel.h>
#include <bsw/kernel/COMM_STACK/canopen-stack/src/core/co_core.h>
/*IPC include */
#include <bsw/ipc/events/signal.h>
#include <bsw/ipc/mqueue/mqueue.h>
/* Global data include  */
#include <bsw/ipc/global/global_var.h>
/* Config */
#include <bsw/drivers/device/cpu/drv_cpu.h>
#include <bsw/drivers/device/com/can/drv_can.h>
#include <bsw/services/os_shell_mgmt/os_shell_mgmt.h>
#include <bsw/drivers/device/cpu/drv_cpu.h>
#include <bsw/drivers/device/flash/drv_flash.h>
#include <app/bsw_config/os_conf/can_network_config.h>
#include <app/bsw_config/os_conf/co_obj_dict.h>

/*########### API Export #########################################*/

#ifdef __cplusplus
extern "C" {
#endif
void syscall_diagnostics_log(uint32_t err_code, uint32_t count);
#ifdef __cplusplus
}
#endif




#endif /* BSW_SERVICES_DIAGNOSTICS_MGMT_DIAGNOSTICS_MGMT_H_ */
