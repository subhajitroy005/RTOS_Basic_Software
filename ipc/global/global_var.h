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

#ifndef OS_IPC_GLOBAL_GLOBAL_VAR_H_
#define OS_IPC_GLOBAL_GLOBAL_VAR_H_
#include <app/bsw_config/arch_conf/board/board.h>
#include <bsw/utils/type_def/typedef_global.h>


/* External variables declaration */
extern global_drv_status_type 			global_drv_status;
extern global_debug_status_type			global_debug_status;
extern global_service_status_type		global_service_status;
#endif /* OS_IPC_GLOBAL_GLOBAL_VAR_H_ */
