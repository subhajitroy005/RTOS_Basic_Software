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

#ifndef OS_IPC_EVENTS_SIGNAL_H_
#define OS_IPC_EVENTS_SIGNAL_H_

/*Kernel functionality include */
#include <bsw/kernel/kernel.h>

/* Global typedef */
#include <bsw/utils/type_def/typedef_global.h>


/**************  API Export *****************/
#ifdef __cplusplus
extern "C" {
#endif

kernel_status_type 	ipc_signal_init(void);
EventGroupHandle_t*	ipc_signal_get_event_handle(ipc_signal_list_type event_name);

#ifdef __cplusplus
}
#endif
/**************  END API Export *****************/



#endif /* OS_IPC_EVENTS_SIGNAL_H_ */
