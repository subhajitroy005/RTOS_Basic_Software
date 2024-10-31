/*
 * DS3502.c
 *
 *  Created on: Sep 28, 2023
 *      Author: subhajit-roy
 */
#include "DS3502_IIC.h"

#if (INC_DRIVER_DS3502 == 1)

#ifdef DS3502_COMM // Check for COM bus

/* IIC management IPC ref  */
static i2c_pdu_struct_type	iic_pdu_buffer;
static uint64_t				set_time_ms;
static uint8_t				data_buffer[32];



kernel_status_type drv_ds3502_iic_init(uint8_t device_address)
{
	kernel_status_type op_status = KERNEL_OK;

	/* Set the IOCON register
	 * AUTO increment enabled */
	data_buffer[0] = 0;
	op_status |= drv_ds3502_iic_write_register(device_address, REG_CR, data_buffer, 1);
	/* All set as output for both PORT A and PORT B*/
	drv_time_delay_ms(1);

	return op_status;
}

kernel_status_type	drv_ds3502_iic_write_pot(uint8_t device_address, uint8_t pot_val)
{
	kernel_status_type op_status = KERNEL_OK;
	/**/
	data_buffer[0] = pot_val;
	op_status |= drv_ds3502_iic_write_register(device_address, REG_CTRL, data_buffer, 1);

	return op_status;
}


/* Thease are basic read and write operation */
kernel_status_type drv_ds3502_iic_write_register(uint8_t device_address, uint8_t reg_address, uint8_t* data , uint8_t length)
{
	if(length == 1)	// single register write
	{
		iic_pdu_buffer.device_id = DS3502_COMM;
		iic_pdu_buffer.iic_operation = IIC_OPERATION_REG_WRITE;
		iic_pdu_buffer.slave_address = device_address;
		iic_pdu_buffer.reg_addr = reg_address;
		iic_pdu_buffer.txn_data[0] = data[0];
		/* do a IIC operation */
		xQueueSend(pipe_iic_pdu_tx_handle,
					&iic_pdu_buffer,
					(TickType_t)0
				  );
	}
	else
	{
		iic_pdu_buffer.device_id = DS3502_COMM;
		iic_pdu_buffer.iic_operation = IIC_OPERATION_WRITE;
		iic_pdu_buffer.slave_address = device_address;
		iic_pdu_buffer.txn_data[0] = reg_address;
		iic_pdu_buffer.length = length + 1;
		for(int i = 0; i < length; i++)
		{
			iic_pdu_buffer.txn_data[i+1] =  data[i];
		}
		/* do a IIC operation */
		xQueueSend(pipe_iic_pdu_tx_handle,
					&iic_pdu_buffer,
					(TickType_t)0
				  );
	}
	/* Mark the current time */
	iic_pdu_buffer.op_status = KERNEL_ERR;
	set_time_ms = drv_time_get_ticks();
	/* wait for the feedback */
	while((drv_time_get_ticks() - set_time_ms) < IIC_ACK_TIMEOUT_MS)
	{
		xQueuePeek( pipe_iic_pdu_rx_handle,
		             &iic_pdu_buffer,
					(TickType_t)0
		           );
		/* If the PDU receive for */
		if((iic_pdu_buffer.device_id == DS3502_COMM) && \
		   (iic_pdu_buffer.slave_address == device_address))
		{
			xQueueReceive(pipe_iic_pdu_rx_handle,
							&iic_pdu_buffer,
							(TickType_t)0
						  );
		    break;
		}
	}

	return iic_pdu_buffer.op_status;
}

uint8_t drv_ds3502_iic_read_register(uint8_t device_address, uint8_t reg_address)
{
	iic_pdu_buffer.device_id = DS3502_COMM;
	iic_pdu_buffer.iic_operation = IIC_OPERATION_REG_READ;
	iic_pdu_buffer.slave_address = device_address;
	iic_pdu_buffer.reg_addr = reg_address;
	/* do a IIC operation */
	xQueueSend(pipe_iic_pdu_tx_handle,
				&iic_pdu_buffer,
				(TickType_t)0
			  );

	/* Mark the current time */
	iic_pdu_buffer.op_status = KERNEL_ERR;
	set_time_ms = drv_time_get_ticks();

	while((drv_time_get_ticks() - set_time_ms) < IIC_ACK_TIMEOUT_MS)
	{
		xQueuePeek( pipe_iic_pdu_rx_handle,
			         &iic_pdu_buffer,
					 (TickType_t)0
			      );
		/* If the PDU receive for */
		if((iic_pdu_buffer.device_id == DS3502_COMM) && \
				(iic_pdu_buffer.slave_address == device_address))
		{
			xQueueReceive(pipe_iic_pdu_rx_handle,
							&iic_pdu_buffer,
							(TickType_t)0
						  );
			 break;
		}
	}
	if(iic_pdu_buffer.op_status == KERNEL_OK)
		return iic_pdu_buffer.txn_data[0];
	else
		return 2;
}

#else
#error "DS3502 Driver included but bus DS3502_COMM bus not defined!!"
#endif

#endif
