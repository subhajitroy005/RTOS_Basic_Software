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

#ifndef OS_UTILS_TYPE_DEF_MACRO_GLOBAL_H_
#define OS_UTILS_TYPE_DEF_MACRO_GLOBAL_H_

/* Global conventions */
#define FLAG_SET				(1)
#define FLAG_RESET				(0)
#define	FLAG_ENABLE				(1)
#define FLAG_DISABLE 			(0)

/* Driver related status */
#define DRV_OK    				(0)
#define DRV_ERR     			(1)
#define DRV_BUSY    			(2)
#define DRV_TIMEOUT 			(3)

#define KERNEL_OK    			(0)
#define KERNEL_ERR      		(1)
#define KERNEL_WRONG_REQ		(-1)
/* Event flags reference */
#define EVENT_BIT_DRV_ERR_GET (1<<0)


#endif /* OS_UTILS_TYPE_DEF_MACRO_GLOBAL_H_ */
