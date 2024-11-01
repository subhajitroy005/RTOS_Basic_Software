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

#ifndef __TESTSUITE_H__
#define __TESTSUITE_H__				1

#include <kernel/kernel.h>
#include <bsw_config/arch_conf/board/board.h>
#include <drivers/device/driver_port.h>
#include <drivers/device/gpio/drv_gpio.h>
#include <drivers/device/com/iic/drv_iic.h>
#include <drivers/device/com/can/drv_can.h>
#include <drivers/device/com/uart/drv_uart.h>
#include <drivers/device/time/drv_time.h>
#include <ipc/mqueue/mqueue.h>
#include <services/os_shell_mgmt/os_shell_mgmt.h>
#include <drivers/device/flash/drv_flash.h>

#include <drivers/module/PCA9685_IIC/PCA9685_IIC.h>
#include <drivers/module/MCP23017_IIC/MCP23017_IIC.h>
#include <drivers/module/DS3202_IIC/DS3502_IIC.h>
#include <drivers/module/INA230_IIC/INA230_IIC.h>
#include <drivers/module/MCP45HVX1/MCP45HVX1.h>
#include <drivers/module/MCP3427_IIC/MCP3427_IIC.h>
#include <drivers/module/MCP4441_IIC/MCP4441_IIC.h>

/* Please select the following est conditions  */

#define MANUAL_TEST_EN					0	// Global test enable flag
											// This should be enable for testing
											// any manual testing

/* Driver level test */
#define RUN_UART_LOOPBACK_TEST 			0
#define RUN_GPIO_TEST 					0
#define RUN_FLASH_TEST					0
#define RUN_TIMER_PWM_TEST				0
#define RUN_CAN_DRV_TX_TEST				0
#define RUN_CAN_DRV_RX_TEST				0
#define RUN_IIC_SCAN_TEST				0
#define RUN_HOKUYO_GPIO_TEST			0
/* Recomended driver level test should be completed first
 * before service level test*/

/* Service level test */
#define RUN_IIC_SERVICE_LOOPBACK_TEST	0
#define RUN_PCA9685_WRITE_TEST			0
#define RUN_MCP23017_WRITE_TEST			0
#define RUN_DS3502_WRITE_TEST			0
#define RUN_INA230_TEST					0
#define RUN_MCP45HVX1_POT_TEST			0
#define RUN_MCP3427_READ_TEST			0
#define RUN_MCP4441_WRITE_TEST			0


#ifdef __cplusplus
extern "C" {
#endif


void ManualTestRunner(void);

#ifdef __cplusplus
}
#endif


#endif /* __TESTSUITE_H__ */
