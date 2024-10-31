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

#ifndef OS_KERNEL_KERNEL_H_
#define OS_KERNEL_KERNEL_H_
/* Configuration include */
#include <app/bsw_config/os_conf/os_config.h>



/* If kernel selection config is defined then */
#ifndef OS_KERNEL_SELECT
#error "Please select the kernel in os/config/os_config/os_config.h [ OS_KERNEL_SELECT ] !!!"
#else

/* Includes all the free RTOS related header*/
#if (OS_KERNEL_SELECT == OS_KERNEL_FREERTOS)

#include <FreeRTOS.h>
#include <atomic.h>
#include <queue.h>
#include <task.h>
#include <timers.h>
#include <semphr.h>
#include <event_groups.h>
#include <stream_buffer.h>

#endif

#endif





/* FreeRTOS kernel integration */


#endif /* OS_KERNEL_KERNEL_H_ */
