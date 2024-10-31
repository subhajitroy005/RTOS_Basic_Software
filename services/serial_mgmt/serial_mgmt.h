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

#ifndef OS_SERVICES_SERIAL_MGMT_SERIAL_MGMT_H_
#define OS_SERVICES_SERIAL_MGMT_SERIAL_MGMT_H_

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
/* Config */
#include <bsw/drivers/device/com/uart/drv_uart.h>
#include <bsw/utils/ringbuffer/ringbuffer.h>

/**************  API Export *****************/
#ifdef __cplusplus
extern "C" {
#endif


/* Proc serial mgmt core */
void proc_service_serial_mgmt(void * parm);



#ifdef __cplusplus
}
#endif
/**************  END API Export *****************/




#endif /* OS_SERVICES_SERIAL_MGMT_SERIAL_MGMT_H_ */
