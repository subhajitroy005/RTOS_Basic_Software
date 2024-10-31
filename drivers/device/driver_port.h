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

#ifndef OS_DRIVERS_DEVICE_DRIVER_PORT_H_
#define OS_DRIVERS_DEVICE_DRIVER_PORT_H_

/* This MACRO supprts following variants
 *
 * */
#if defined(STM32L431xx)
#include "stm32l4xx_hal.h"
#endif


/* This MACRO supprts following variants
 * *
 */
#if defined(STM32H7A3xxQ)
#include "stm32h7xx.h"
#include "usbd_cdc_if.h"
#endif

#if defined(STM32H743xx)
#include "stm32h743xx.h"
#include "usbd_cdc_if.h"
#endif

/* This MACRO supprts following variants
 * *
 */

#if defined(STM32F429xx)
#include "stm32f4xx.h"
#endif

#if defined(STM32F765xx)
#include "stm32f7xx.h"
#endif


#endif /* OS_DRIVERS_DEVICE_DRIVER_PORT_H_ */
