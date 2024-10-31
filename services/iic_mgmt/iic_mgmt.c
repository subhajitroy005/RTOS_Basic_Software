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

#include "iic_mgmt.h"

#if (INC_SERVICE_IIC_MGMT == 1 )


/* Local Variables */
static EventGroupHandle_t * iic_mgmt_iic_driver_event_handle_ref;
//static EventBits_t 			iic_mgmt_event_iic_drv_bits;

static BaseType_t			status;

static i2c_pdu_struct_type	iic_mgmt_iic_pdu_buff;
static uint8_t 				iic_mgmt_i2c_txn_buffer[10],iic_mgmt_drv_status;
#if (DRV_DEBUG_EN == 1)
static uint8_t 			    print_buffer[PRINTK_BUFF_LENGTH];
static uint32_t				iic_mgmt_counter = 0;
#endif
/* local functions declarations */
static kernel_status_type proc_service_iic_mgmt_init(void);



/**
 * proc_service_iic_mgmt
 * @ret:
 *
 */
void proc_service_iic_mgmt(void * parm)
{
	/* Start time offset */
	vTaskDelay(pdMS_TO_TICKS(TIME_OFFSET_IIC_MANAGEMENT));
	/* if initialization failed suapend the task */
	if(proc_service_iic_mgmt_init() != KERNEL_OK)
	{
		vTaskSuspend(NULL);
	}

	while(true)
	{
			status = xQueueReceive( pipe_iic_pdu_tx_handle,
                    				&iic_mgmt_iic_pdu_buff,
									(TickType_t)0
                 	 	 	 	   );
			if(status == pdTRUE)
			{
				switch(iic_mgmt_iic_pdu_buff.iic_operation)
				{
					/*
					 * Read multiple data
					 * */
					case IIC_OPERATION_WRITE:
						iic_mgmt_drv_status = DRV_OK;

						if(iic_mgmt_iic_pdu_buff.length > 32)
							iic_mgmt_iic_pdu_buff.length = 32;

						/* RUN until the maximum buffer value i.e 32 */
						for(int i=0; (i < iic_mgmt_iic_pdu_buff.length); i++)
						{
							iic_mgmt_i2c_txn_buffer[i] = iic_mgmt_iic_pdu_buff.txn_data[i];
						}

						iic_mgmt_drv_status |= drv_iic_transmit(iic_mgmt_iic_pdu_buff.device_id,
														iic_mgmt_iic_pdu_buff.slave_address,
														iic_mgmt_i2c_txn_buffer,
														iic_mgmt_iic_pdu_buff.length);
						if(iic_mgmt_drv_status == DRV_OK)
						{
							iic_mgmt_iic_pdu_buff.op_status = KERNEL_OK;

#if(DRV_DETAIL_DEBUG_EN == 1)
							for(int i = 0; i<iic_mgmt_iic_pdu_buff.length; i++)
							{
								sprintf((char*)print_buffer, "[DRV: IIC-> %d ] WR data[%d] - 0x%x!\n\r",
																iic_mgmt_iic_pdu_buff.device_id,
																(i),
																iic_mgmt_iic_pdu_buff.txn_data[i]
									   );
								printk(print_buffer);
							}
#endif
						}
						else
						{
							iic_mgmt_iic_pdu_buff.op_status = KERNEL_ERR;
#if (DRV_DEBUG_EN == 1)
							sprintf((char*)print_buffer, "[DRV: IIC-> %d ] WR OP Failed!\n\r",iic_mgmt_iic_pdu_buff.device_id);
							printk(print_buffer);
#endif
						}

						xQueueSend(pipe_iic_pdu_rx_handle,
									&iic_mgmt_iic_pdu_buff,
									(TickType_t)0
								   );
					break;

					/*
					 * Read multiple data
					 * */


					case IIC_OPERATION_READ:
						iic_mgmt_drv_status = DRV_OK;

						if(iic_mgmt_iic_pdu_buff.length > 31)
							iic_mgmt_iic_pdu_buff.length = 31; // one extra byte

						iic_mgmt_drv_status |= drv_iic_receive(iic_mgmt_iic_pdu_buff.device_id,
													(iic_mgmt_iic_pdu_buff.slave_address | 0x01),
													iic_mgmt_i2c_txn_buffer,
													iic_mgmt_iic_pdu_buff.length);

						iic_mgmt_iic_pdu_buff.drv_status = iic_mgmt_drv_status;
						/* The the driver transaction is successful */
						if(iic_mgmt_drv_status == DRV_OK)
						{
							/* Device register value/values  */
							for (int i=0; i<iic_mgmt_iic_pdu_buff.length; i++)
							{
								iic_mgmt_iic_pdu_buff.txn_data[i] =  iic_mgmt_i2c_txn_buffer[i];
#if(DRV_DETAIL_DEBUG_EN == 1)
								sprintf((char*)print_buffer, "[DRV: IIC-> %d ] RD data[%d] - 0x%x!\n\r",
																iic_mgmt_iic_pdu_buff.device_id,
																i,
																iic_mgmt_iic_pdu_buff.txn_data[i]
									   );
								printk(print_buffer);
#endif
							}

							iic_mgmt_iic_pdu_buff.op_status = KERNEL_OK;
						}
						else
						{
							iic_mgmt_iic_pdu_buff.op_status = KERNEL_ERR;
#if (DRV_DEBUG_EN == 1)
							sprintf((char*)print_buffer, "[DRV: IIC-> %d ] RD OP Failed!\n\r",iic_mgmt_iic_pdu_buff.device_id);
							printk(print_buffer);
#endif
						}

						xQueueSend(pipe_iic_pdu_rx_handle,
									&iic_mgmt_iic_pdu_buff,
									(TickType_t)0
								  );
					break;

					case IIC_OPERATION_REG_READ:
						iic_mgmt_drv_status = DRV_OK;

						/* Device register address */
						iic_mgmt_i2c_txn_buffer[0] = iic_mgmt_iic_pdu_buff.reg_addr;
						iic_mgmt_drv_status |= drv_iic_transmit(iic_mgmt_iic_pdu_buff.device_id,
													iic_mgmt_iic_pdu_buff.slave_address,
													iic_mgmt_i2c_txn_buffer,
												  1);

						iic_mgmt_drv_status |= drv_iic_receive(iic_mgmt_iic_pdu_buff.device_id,
													(iic_mgmt_iic_pdu_buff.slave_address | 0x01),
													iic_mgmt_i2c_txn_buffer,
													2);
						iic_mgmt_iic_pdu_buff.drv_status = iic_mgmt_drv_status;
						/* The the driver transaction is successful */
						if(iic_mgmt_drv_status == DRV_OK)
						{
							/* Device register value  */
							iic_mgmt_iic_pdu_buff.txn_data[0] =  iic_mgmt_i2c_txn_buffer[0];
							iic_mgmt_iic_pdu_buff.op_status = KERNEL_OK;
						}
						else
						{
							iic_mgmt_iic_pdu_buff.op_status = KERNEL_ERR;
#if (DRV_DEBUG_EN == 1)
							sprintf((char*)print_buffer, "[DRV: IIC-> %d ] RD OP Failed!\n\r",iic_mgmt_iic_pdu_buff.device_id);
							printk(print_buffer);
							iic_mgmt_counter = iic_mgmt_counter + 1;
							if (iic_mgmt_iic_pdu_buff.device_id == IIC_1){
								syscall_diagnostics_log(ERR_1202_IIC1_CH1_SLV_RD, iic_mgmt_counter);
							}
							else if (iic_mgmt_iic_pdu_buff.device_id == IIC_2){
								syscall_diagnostics_log(ERR_1227_IIC2_CH1_SLV_RD, iic_mgmt_counter);
							}
							else if (iic_mgmt_iic_pdu_buff.device_id == IIC_3){
								syscall_diagnostics_log(ERR_1252_IIC3_CH1_SLV_RD, iic_mgmt_counter);
							}
							else{
								syscall_diagnostics_log(ERR_1277_IIC4_CH1_SLV_RD, iic_mgmt_counter);
							}
#endif
						}

						xQueueSend( pipe_iic_pdu_rx_handle,
									&iic_mgmt_iic_pdu_buff,
									(TickType_t)pdMS_TO_TICKS(TIMEOUT_IIC_PIPE_OP)
								  );
					break;

					case IIC_OPERATION_REG_WRITE:
						iic_mgmt_drv_status = DRV_OK;

						/* Device register address */
						iic_mgmt_i2c_txn_buffer[0] = iic_mgmt_iic_pdu_buff.reg_addr;
						iic_mgmt_i2c_txn_buffer[1] = iic_mgmt_iic_pdu_buff.txn_data[0];
						iic_mgmt_drv_status |= drv_iic_transmit(iic_mgmt_iic_pdu_buff.device_id,
																iic_mgmt_iic_pdu_buff.slave_address,
																iic_mgmt_i2c_txn_buffer,
																2);

						iic_mgmt_iic_pdu_buff.drv_status = iic_mgmt_drv_status;

						if(iic_mgmt_drv_status == DRV_OK)
							iic_mgmt_iic_pdu_buff.op_status = KERNEL_OK;
						else
						{
							iic_mgmt_iic_pdu_buff.op_status = KERNEL_ERR;
#if (DRV_DEBUG_EN == 1)
							sprintf((char*)print_buffer, "[DRV: IIC-> %d ] WR OP Failed!\n\r",iic_mgmt_iic_pdu_buff.device_id);
							printk(print_buffer);
							iic_mgmt_counter = iic_mgmt_counter + 1;
							if (iic_mgmt_iic_pdu_buff.device_id == IIC_1){
								syscall_diagnostics_log(ERR_1203_IIC1_CH1_SLV_WR, iic_mgmt_counter);
							}
							else if (iic_mgmt_iic_pdu_buff.device_id == IIC_2){
								syscall_diagnostics_log(ERR_1228_IIC2_CH1_SLV_WR, iic_mgmt_counter);
							}
							else if (iic_mgmt_iic_pdu_buff.device_id == IIC_3){
								syscall_diagnostics_log(ERR_1253_IIC3_CH1_SLV_WR, iic_mgmt_counter);
							}
							else{
								syscall_diagnostics_log(ERR_1278_IIC4_CH1_SLV_WR, iic_mgmt_counter);
							}

#endif
						}

						xQueueSend( pipe_iic_pdu_rx_handle,
									&iic_mgmt_iic_pdu_buff,
									(TickType_t)pdMS_TO_TICKS(TIMEOUT_IIC_PIPE_OP)
								  );
					break;
				}
			}
	}
}

/**
 * proc_service_iic_mgmt_init
 * @ret:
 *
 */
kernel_status_type proc_service_iic_mgmt_init(void)
{
	global_service_status.iic_mgmt_service_status = KERNEL_OK;
	/* reset the variables */

	/* Get the IPC handles */
	/*events*/
	iic_mgmt_iic_driver_event_handle_ref = ipc_signal_get_event_handle(IPC_EVENT_IIC_DRV);
	if(iic_mgmt_iic_driver_event_handle_ref == NULL)
	{
		global_service_status.iic_mgmt_service_status |= KERNEL_ERR;
	}


	if(drv_iic_init() != DRV_OK)
	{
		/* TODO implement the strategy for reinitialize */
		global_service_status.iic_mgmt_service_status |= KERNEL_ERR;
	}

	return (kernel_status_type)global_service_status.iic_mgmt_service_status;
}


#endif


