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

#ifndef BSW_UTILS_ERROR_CODES_H_
#define BSW_UTILS_ERROR_CODES_H_

/*CAN error codes*/
/************************** CAN 1**********************************/
#define ERR_1000_CAN1_BUSOFF 					1000
#define ERR_1001_CAN1_PASSIVE					1001
#define ERR_1002_CAN1_DRV_TX					1002
#define ERR_1003_CO_CAN1_TIM					1003
#define ERR_1004_CO_CAN1_NODE_FATAL				1004
#define ERR_1005_CO_CAN1_HB						1005

/************************** CAN 2**********************************/
#define ERR_1025_CAN2_BUSOFF 					1025
#define ERR_1026_CAN2_PASSIVE					1026
#define ERR_1027_CAN2_DRV_TX					1027
#define ERR_1028_CO_CAN2_TIM					1028
#define ERR_1029_CO_CAN2_NODE_FATAL				1029
#define ERR_1030_CO_CAN2_HB						1030

/************************** CAN 3**********************************/
#define ERR_1050_CAN3_BUSOFF 					1050
#define ERR_1051_CAN3_PASSIVE					1051
#define ERR_1052_CAN3_DRV_TX					1052
#define ERR_1053_CO_CAN3_TIM					1053
#define ERR_1054_CO_CAN3_NODE_FATAL				1054
#define ERR_1055_CO_CAN3_HB						1055

/************************** CAN 4**********************************/
#define ERR_1075_CAN4_BUSOFF 					1075
#define ERR_1076_CAN4_PASSIVE					1076
#define ERR_1077_CAN4_DRV_TX					1077
#define ERR_1078_CO_CAN4_TIM					1078
#define ERR_1079_CO_CAN4_NODE_FATAL				1079
#define ERR_1080_CO_CAN4_HB						1080

/************************** IIC 1**********************************/
#define ERR_1200_IIC1_TX						1200
#define ERR_1201_IIC1_RX						1201
#define ERR_1202_IIC1_CH1_SLV_RD                1202
#define ERR_1203_IIC1_CH1_SLV_WR 				1203

/************************** IIC 2**********************************/
#define ERR_1225_IIC2_TX						1225
#define ERR_1226_IIC2_RX						1226
#define ERR_1227_IIC2_CH1_SLV_RD                1227
#define ERR_1228_IIC2_CH1_SLV_WR 				1228

/************************** IIC 3**********************************/
#define ERR_1250_IIC3_TX						1250
#define ERR_1251_IIC3_RX						1251
#define ERR_1252_IIC3_CH1_SLV_RD                1252
#define ERR_1253_IIC3_CH1_SLV_WR 				1253

/************************** IIC 4**********************************/
#define ERR_1275_IIC4_TX						1275
#define ERR_1276_IIC4_RX						1276
#define ERR_1277_IIC4_CH1_SLV_RD                1277
#define ERR_1278_IIC4_CH1_SLV_WR 				1278

/************************** UART **********************************/
#define ERR_1300_UART1_TX						1300
#define ERR_1305_UART2_TX						1305
#define ERR_1310_UART3_TX						1310
#define ERR_1315_UART4_TX						1315




#endif /* BSW_UTILS_ERROR_CODES_H_ */
