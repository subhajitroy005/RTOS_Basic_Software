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
#include "diagnostics_mgmt.h"


void syscall_diagnostics_log(uint32_t err_code, uint32_t count){
	diagnostics_type local_dignostic_buffer;
	local_dignostic_buffer.err_code = err_code;
	local_dignostic_buffer.timestamp = drv_time_get_ticks();
	local_dignostic_buffer.count = count;
	xQueueSend(pipe_diagnostics_handle,
				   &local_dignostic_buffer,
				   pdMS_TO_TICKS(1));

}
