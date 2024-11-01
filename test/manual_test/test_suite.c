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

#include "test_suite.h"

/*************************************************************
 * Func:
 * Desc:
 *
 * @parm:
 * @ret:
 *
 ************************************************************/
#if (RUN_UART_LOOPBACK_TEST == 1)

static StreamBufferHandle_t * ipc_uart_rx_char_recv;
static uint8_t uart_rx_byte[2];
static uint8_t uart_rx_byte_count;

void RunUartLoopbackTest_init(void)
{
	/*Initialize the driver */
	drv_serial_init(UART_1);
	/* Get the handles */;

	drv_serial_transmit(SERIAL_DEBUG_COMM,"UART TEST\n\r",11);
	drv_serial_transmit(SERIAL_DEBUG_COMM,"UART Start printing i/c data...\n\r",33);


}

void RunUartLoopbackTest_Run(void)
{
//	/* echo back the incoming character received */
//	uart_rx_byte_count = xStreamBufferReceive( *ipc_uart_rx_char_recv,
//							                    ( void * ) uart_rx_byte,
//							                    1,
//												(TickType_t)0);
//							/* OS shell management send*/
//	if(uart_rx_byte_count != 0)
//	{
//		drv_serial_transmit(SERIAL_DEBUG_COMM, uart_rx_byte, 1);
//	}
}

#endif

/*************************************************************
 * Func:
 * Desc:
 *
 * @parm:
 * @ret:
 *
 ************************************************************/
#if (RUN_GPIO_TEST == 1)
void GPIOTest_run(void)
{
	drv_gpio_set_pin(DRV_GPIO_PORT_A, 3);
	drv_gpio_set_pin(DRV_GPIO_PORT_A, 4);
	drv_gpio_set_pin(DRV_GPIO_PORT_A, 5);
	drv_gpio_set_pin(DRV_GPIO_PORT_A, 6);
	drv_gpio_set_pin(DRV_GPIO_PORT_A, 7);
	drv_time_delay_ms(1000);
	drv_gpio_clear_pin(DRV_GPIO_PORT_A, 3);
	drv_gpio_clear_pin(DRV_GPIO_PORT_A, 4);
	drv_gpio_clear_pin(DRV_GPIO_PORT_A, 5);
	drv_gpio_clear_pin(DRV_GPIO_PORT_A, 6);
	drv_gpio_clear_pin(DRV_GPIO_PORT_A, 7);
	drv_time_delay_ms(1000);
}
#endif

/*************************************************************
 * Func:
 * Desc:
 *
 * @parm:
 * @ret:
 *
 ************************************************************/
#if (RUN_IIC_SERVICE_LOOPBACK_TEST == 1)
/* local variables */
static QueueHandle_t	   * pipe_iic_tx;
static QueueHandle_t	   * pipe_iic_rx;
static i2c_pdu_struct_type	iic_pdu_buff;
uint8_t iic_opration_state;
uint8_t print_buffer[PRINTK_BUFF_LENGTH];

#define IIC_TEST_COMM		DS3502_COMM
#define IIC_TEST_ADDRESS	UNNAMED_DEV_1
#define REG_ADDESS			(2)
#define WRITE_VAL			(0x3e)

void IIC_Loopback_test_init(void)
{
	pipe_iic_tx					= ipc_get_queue_handle(PIPE_IIC_PDU_TX);
	pipe_iic_rx					= ipc_get_queue_handle(PIPE_IIC_PDU_RX);
	iic_opration_state 			= 1;
}

void IIC_Loopback_test_run(void)
{
	switch(iic_opration_state)
	{
		case 1: /* write the data in registers */
			iic_pdu_buff.device_id = IIC_TEST_COMM;
			iic_pdu_buff.iic_operation = IIC_OPERATION_REG_WRITE;
			iic_pdu_buff.slave_address = IIC_TEST_ADDRESS;
			iic_pdu_buff.reg_addr = REG_ADDESS;
			iic_pdu_buff.txn_data[0] = WRITE_VAL;;
			/* do a IIC operation */
			xQueueSend(*pipe_iic_tx,
						&iic_pdu_buff,
						(TickType_t)0
					  );
			sprintf(print_buffer, "IIC TEST-> Writing... Channel %d DevAddr: 0x%x data: 0x%x\n\r",
					iic_pdu_buff.device_id,
					iic_pdu_buff.slave_address,
					iic_pdu_buff.txn_data[0]);
			printk(print_buffer);

			iic_opration_state = 2; // next state wait for reply
		break;

		case 2: // wait for write feedback
			if(xQueueReceive(*pipe_iic_rx,
								&iic_pdu_buff,
								(TickType_t)0) == pdTRUE)
			{
				if(iic_pdu_buff.op_status == KERNEL_OK)
				{
					iic_opration_state = 3; // read the register
					sprintf(print_buffer, "IIC TEST-> Write complete! \n\r");
					printk(print_buffer);
				}
				else
				{
					sprintf(print_buffer, "IIC TEST-> Write fail! Retrying...\n\r");
					printk(print_buffer);
					drv_time_delay_ms(1000);
					/*wait for some time and try again */
					iic_opration_state = 1;
				}
			}
		break;

		case 3: // read register
			iic_pdu_buff.device_id = IIC_TEST_COMM;
			iic_pdu_buff.iic_operation = IIC_OPERATION_REG_READ;
			iic_pdu_buff.slave_address = IIC_TEST_ADDRESS;
			iic_pdu_buff.reg_addr = REG_ADDESS;
			iic_pdu_buff.txn_data[0] = 0;
			/* do a IIC operation */
			xQueueSend(*pipe_iic_tx,
						&iic_pdu_buff,
						(TickType_t)0
					  );
			sprintf(print_buffer, "IIC TEST-> Reading... DevAddr: 0x%x\n\r",
					iic_pdu_buff.slave_address);
			printk(print_buffer);

			iic_opration_state = 4; // next state wait for rep
		break;

		case 4: // read register feedback from service
			if(xQueueReceive(*pipe_iic_rx,
							&iic_pdu_buff,
							(TickType_t)0) == pdTRUE)
			{
				if(iic_pdu_buff.op_status == KERNEL_OK)
				{
					iic_opration_state = 3; // read the register
					sprintf(print_buffer, "IIC TEST-> Read complete! Read data 0x%x\n\r", iic_pdu_buff.txn_data[0]);
					printk(print_buffer);
					drv_time_delay_ms(1000);
					iic_opration_state = 1;
				}
				else
				{
					sprintf(print_buffer, "IIC TEST-> Read fail! Retrying...\n\r");
					drv_time_delay_ms(1000);
					/*wait for some time and try again */
					iic_opration_state = 3;
				}
			}
		break;
	}


}

#endif

/*************************************************************
 * Func:
 * Desc:
 *
 * @parm:
 * @ret:
 *
 ************************************************************/
#if (RUN_CAN_DRV_TX_TEST == 1)

can_pdu_struct_type	can_pdu_buffer;
int counter = 0;

void CAN_drv_tx_test_init(void)
{
	can_pdu_buffer.dlc = 8;
	can_pdu_buffer.m_id = 0x100;
	can_pdu_buffer.data[0] = 0xAA;
	can_pdu_buffer.data[1] = 0xFF;
	if(drv_can_init(CAN_1) == DRV_OK)
	{
		printk((uint8_t*)"[TEST] CAN-1 INIT completed..!");
	}
	else
	{
		printk((uint8_t*)"[TEST] CAN-2 INIT Failed..!");
	}
}

void CAN_drv_tx_test_run(void)
{
	can_pdu_buffer.data[2] = counter++;
	if(drv_can_transmit(CAN_1, &can_pdu_buffer) == DRV_OK)
	{
		printk((uint8_t*)"CAN DATA TX\n\r");
	}
	else
	{
		printk((uint8_t*)"CAN DATA FAIL...\n\r");
	}
	HAL_Delay(500);
}

#endif

/*************************************************************
 * Func:
 * Desc:
 *
 * @parm:
 * @ret:
 *
 ************************************************************/
#if (RUN_CAN_DRV_RX_TEST == 1)
uint8_t print_buffer[PRINTK_BUFF_LENGTH];
static can_pdu_struct_type	can_pdu_buffer;
static QueueHandle_t*		drv_rx_handle_ref;
static QueueHandle_t*		drv_rx_2_handle_ref;


void CAN_drv_rx_test_init(void)
{
	drv_time_delay_ms(1000);


	if(drv_can_init(CAN_1) == DRV_OK)
	{
		printk("[TEST] CAN-1 INIT completed..!\n\r");
	}
	else
	{
		printk("[TEST] CAN-1 INIT Failed..!\n\r");
	}

	if(drv_can_init(CAN_2) == DRV_OK)
	{
		printk("[TEST] CAN-2 INIT completed..!\n\r");
	}
	else
	{
		printk("[TEST] CAN-2 INIT Failed..!\n\r");
	}
}

void CAN_drv_rx_test_run(void)
{
	if( xQueueReceive( pipe_can_1_drv_rx_handle,
			         	&can_pdu_buffer,
			            ( TickType_t )0
					 ) == pdPASS )
	{
		sprintf(print_buffer, "[TEST] CAN-1 Data Rx ID 0x%x\n\r",can_pdu_buffer.m_id);
		printk(print_buffer);
		sprintf(print_buffer, "[TEST] Length %d\n\r",can_pdu_buffer.dlc);
		printk(print_buffer);
		for(int i = 0;i < can_pdu_buffer.dlc; i++)
		{
			sprintf(print_buffer, "-[%d]-0x%x-\n\r",i,can_pdu_buffer.data[i]);
			printk(print_buffer);
		}
	}

	if( xQueueReceive( pipe_can_2_drv_rx_handle,
			         	&can_pdu_buffer,
			            ( TickType_t )0
					 ) == pdPASS )
	{
		sprintf(print_buffer, "[TEST] CAN-2 Data Rx ID 0x%x\n\r",can_pdu_buffer.m_id);
		printk(print_buffer);
		sprintf(print_buffer, "[TEST] Length %d\n\r",can_pdu_buffer.dlc);
		printk(print_buffer);
		for(int i = 0;i < can_pdu_buffer.dlc; i++)
		{
			sprintf(print_buffer, "-[%d]-0x%x-\n\r",i,can_pdu_buffer.data[i]);
			printk(print_buffer);
		}
	}
}

#endif

/*************************************************************
 * Func:
 * Desc:
 *
 * @parm:
 * @ret:
 *
 ************************************************************/
#if (RUN_IIC_SCAN_TEST == 1)
uint8_t print_buffer[PRINTK_BUFF_LENGTH];
uint8_t iic_txn_buffer[8];
void IIC_scan_test_init(void)
{
	drv_iic_init();
	printk("TEST-> IIC Address Scanning...\n\r");
}

void IIC_scan_test_run(void)
{
	/* Each IIC channel */
	/* IIC channel 1 */
	printk("TEST-> Channel 1 Scanning...\n\r");
	for(int i = 0 ; i < 255; i++)
	{
		iic_txn_buffer[0] = i;
		if(drv_iic_device_ready(IIC_1, i) == DRV_OK)
		{
			sprintf(print_buffer, "TEST-> Active Device Address %x\n\r",i);
			printk(print_buffer);
		}
	}
	printk("TEST-> Channel 1 scanning complete!\n\r");

	printk("TEST-> Channel 2 Scanning...\n\r");
	for(int i = 0 ; i < 255; i++)
	{
		iic_txn_buffer[0] = i;
		if(drv_iic_device_ready(IIC_2, i) == DRV_OK)
		{
			sprintf(print_buffer, "TEST-> Active Device Address %x\n\r",i);
			printk(print_buffer);
		}
	}
	printk("TEST-> Channel 2 scanning complete!\n\r");
	HAL_Delay(2000);
}

#endif

/*************************************************************
 * Func:
 * Desc:
 *
 * @parm:
 * @ret:
 *
 ************************************************************/
#if (RUN_PCA9685_WRITE_TEST == 1)
uint8_t print_buffer[PRINTK_BUFF_LENGTH];
uint8_t txn_data[8];

#define TEST_PCA_ADDRESS	UNNAMED_DEV_3
#define TEST_PCA_CHANNEL	IIC_4

void pca9685_write_test_init(void)
{
	sprintf(print_buffer, "TEST-> PCA9685 IIC channel: %d Device address 0x%x\n\r", TEST_PCA_CHANNEL, TEST_PCA_ADDRESS);
	printk(print_buffer);

	if(drv_pca9685_iic_init(TEST_PCA_ADDRESS) == KERNEL_OK)
	{
		printk("TEST-> PCA9685 Initialized successfully!\n\r");
	}
	else
	{
		printk("TEST-> PCA9685 Initialization Fail!\n\r");
	}

	uint8_t readdata = drv_pca9685_iic_read_register(TEST_PCA_ADDRESS, REG_MODE_0);
	sprintf(print_buffer, "TEST-> Int Reg val 0x%x\n\r",readdata);
	printk(print_buffer);
}

void pca9685_write_test_run(void)
{

		txn_data[0] = 0xFF;
		txn_data[0] = 0xFF;
		txn_data[0] = 0;
		txn_data[0] = 0;
		if( drv_pca9685_iic_write_register(TEST_PCA_ADDRESS, REG_LED0_ON_L, txn_data, 1) == KERNEL_OK)
		{
			printk("TEST-> PCA9685 Write Success! ALL LED ON\n\r");
		}
		else
		{
			printk("TEST-> PCA9685 Write FAIL!\n\r");
		}
		drv_time_delay_ms(1000);
		txn_data[0] = 1;
		txn_data[0] = 1;
		txn_data[0] = 0xFF;
		txn_data[0] = 0xFF;
		if( drv_pca9685_iic_write_register(TEST_PCA_ADDRESS, REG_LED0_ON_L, txn_data, 1) == KERNEL_OK)
		{
			printk("TEST-> PCA9685 Write Success! ALL LED ON\n\r");
		}
		else
		{
			printk("TEST-> PCA9685 Write FAIL!\n\r");
		}

//	txn_data[0] = 0xFF;
//	drv_pca9685_iic_write_register(REG_LED0_ON_L, txn_data, 1);
//	txn_data[0] = 0xFF;
//	drv_pca9685_iic_write_register(REG_LED0_ON_L+1, txn_data, 1);
//
//	txn_data[0] = 0;
//	drv_pca9685_iic_write_register(REG_LED0_ON_L+2, txn_data, 1);
//
//	txn_data[0] = 0;
//	if( drv_pca9685_iic_write_register(REG_LED0_ON_L+3, txn_data, 1) == KERNEL_OK)
//	{
//		printk("PCA Write Success! ALL LED ON\n\r");
//	}
//	else
//	{
//		printk("PCA Write FAIL!\n\r");
//	}
//
//	drv_time_delay_ms(1000);
//
////
//	txn_data[0] = 0x0;
//	drv_pca9685_iic_write_register(REG_LED0_ON_L, txn_data, 1);
//	txn_data[0] = 0x0;
//	drv_pca9685_iic_write_register(REG_LED0_ON_L+1, txn_data, 1);
//
//	txn_data[0] = 0xFF;
//	drv_pca9685_iic_write_register(REG_LED0_ON_L+2, txn_data, 1);
//
//	txn_data[0] = 0xFF;
//	if( drv_pca9685_iic_write_register(REG_LED0_ON_L+3, txn_data, 1) == KERNEL_OK)
//	{
//		printk("PCA Write Success! ALL LED ON\n\r");
//	}
//	else
//	{
//		printk("PCA Write FAIL!\n\r");
//	}
//
	drv_time_delay_ms(1000);

	uint8_t readdata = drv_pca9685_iic_read_register(TEST_PCA_ADDRESS, REG_LED0_ON_L+3);
		sprintf(print_buffer, "Reg val %x\n\r",readdata);
		printk(print_buffer);
}

#endif

/*************************************************************
 * Func:
 * Desc:
 *
 * @parm:
 * @ret:
 *
 ************************************************************/
#if(RUN_MCP23017_WRITE_TEST == 1)
uint8_t print_buffer[PRINTK_BUFF_LENGTH];
uint8_t txn_data[8];

void mcp23017_write_test_init(void)
{
	sprintf(print_buffer, "TEST-> MCP23017 Channel: %d Device address %x\n\r", MCP23017_COMM, MCP23017_IIC_ADDRESS);
	printk(print_buffer);

	if(drv_mcp23017_iic_init(MCP23017_IIC_ADDRESS) == KERNEL_OK)
	{
		printk("TEST-> MCP23017 Initialized successfully!\n\r");
	}
	else
	{
		printk("TEST-> MCP23017 Initialization Fail!\n\r");
	}
}

void mcp23017_write_test_run(void)
{

	printk("TEST-> PORT A Pin setting...!\n\r");
	for(int i = 0; i < 8; i++ )
	{
		if( drv_mcp23017_iic_write(MCP23017_IIC_ADDRESS, PORT_A, i, FLAG_SET) == KERNEL_OK)
		{
			sprintf(print_buffer, "TEST-> Pin %d ON\n\r",i);
			printk(print_buffer);
		}
		drv_time_delay_ms(100);
	}

	printk("TEST-> PORT B Pin setting...!\n\r");
	for(int i = 0; i < 8; i++ )
	{
		if( drv_mcp23017_iic_write(MCP23017_IIC_ADDRESS, PORT_B, i, FLAG_SET) == KERNEL_OK)
		{
			sprintf(print_buffer, "TEST-> Pin %d ON\n\r",i);
			printk(print_buffer);
		}
		drv_time_delay_ms(100);
	}

	drv_time_delay_ms(1000);

	printk("TEST-> PORT A Pin toggle!\n\r");
	for(int i = 0; i < 8; i++ )
	{
		if( drv_mcp23017_iic_write(MCP23017_IIC_ADDRESS, PORT_A, i, FLAG_RESET) == KERNEL_OK)
		{
			sprintf(print_buffer, "TEST-> Pin %d OFF\n\r",i);
			printk(print_buffer);
		}
		drv_time_delay_ms(100);
	}

	printk("TEST-> PORT B Pin setting...!\n\r");
	for(int i = 0; i < 8; i++ )
	{
		if( drv_mcp23017_iic_write(MCP23017_IIC_ADDRESS, PORT_B, i, FLAG_RESET) == KERNEL_OK)
		{
			sprintf(print_buffer, "TEST-> Pin %d OFF\n\r",i);
			printk(print_buffer);
		}
		drv_time_delay_ms(100);
	}


	drv_time_delay_ms(1000);
}
#endif

/*************************************************************
 * Func:
 * Desc:
 *
 * @parm:
 * @ret:
 *
 ************************************************************/
#if (RUN_DS3502_WRITE_TEST == 1)

uint8_t print_buffer[PRINTK_BUFF_LENGTH];

void ds3502_write_test_init(void)
{
	printk("TEST-> DS3502 Digipot write test TEST\n\r");

	sprintf(print_buffer, "TEST-> DS3502 Channel: %d Device address %x\n\r", DS3502_COMM, DS3502_2_IIC_ADDRESS);
	printk(print_buffer);

	if(drv_ds3502_iic_init(DS3502_2_IIC_ADDRESS) == KERNEL_OK)
	{
		printk("TEST-> DS3502 Initialized successfully!\n\r");
	}
	else
	{
		printk("TEST-> DS3502 Initialization Fail!\n\r");
	}
}


void ds3502_write_test_run(void)
{
	printk("TEST-> DS3502 Pot writing..!\n\r");
	for(int i = 0; i <= 127 ; i += 5)
	{
		if( drv_ds3502_iic_write_pot(DS3502_2_IIC_ADDRESS, i) == KERNEL_OK)
		{
			sprintf(print_buffer, "TEST-> Write Val %d\n\r", i);
			printk(print_buffer);
		}
		drv_time_delay_ms(1000);
	}
}
#endif

/*************************************************************
 * Func:
 * Desc:
 *
 * @parm:
 * @ret:
 *
 ************************************************************/
#if (RUN_MCP4441_WRITE_TEST == 1)

uint8_t print_buffer[PRINTK_BUFF_LENGTH];

void mcp4441_write_test_init(void)
{
	printk("TEST-> MCP4441 Digipot write test TEST\n\r");

	sprintf(print_buffer, "TEST-> MCP4441 Channel: %d Device address %x\n\r", MCP4441_COMM, MCP441_IIC_ADDRESS);
	printk(print_buffer);

	if(drv_mcp4441_iic_init(MCP441_IIC_ADDRESS) == KERNEL_OK)
	{
		printk("TEST-> MCP4441 Initialized successfully!\n\r");
	}
	else
	{
		printk("TEST-> MCP4441 Initialization Fail!\n\r");
	}
}
void mcp4441_write_test_run(void)
{
	uint8_t wiper_num = 1;
	printk("TEST-> MCP4441 Pot writing..!\n\r");
	for(int i = 0; i <= 127 ; i += 5)
	{
		if( drv_mcp4441_iic_write_pot(MCP441_IIC_ADDRESS,wiper_num, i) == KERNEL_OK)
		{
			sprintf(print_buffer, "TEST-> Write Val %d\n\r", i);
			printk(print_buffer);
		}
		drv_time_delay_ms(1000);
	}
}

#endif

/*************************************************************
 * Func:
 * Desc:
 *
 * @parm:
 * @ret:
 *
 ************************************************************/
#if (RUN_HOKUYO_GPIO_TEST == 1)

void hokuyo_gpio_test_init(void)
{
	printk("TEST-> Hokuyo gpio tesr!\n\r");

	drv_gpio_set_pin_state(SICK_1_PWR_EN_PORT, SICK_1_PWR_EN_PIN, FLAG_ENABLE);
	printk("TEST-> Hokuyo 1 power enabled!\n\r");

	printk("TEST-> Hokuyo 1 Area 1 selectd!\n\r");
	drv_gpio_set_pin_state(SICK_1_AREA_1_PORT, SICK_1_AREA_1_PIN, FLAG_ENABLE);
	drv_gpio_set_pin_state(SICK_1_AREA_2_PORT, SICK_1_AREA_2_PIN, FLAG_DISABLE);
	drv_gpio_set_pin_state(SICK_1_AREA_3_PORT, SICK_1_AREA_3_PIN, FLAG_DISABLE);
	drv_gpio_set_pin_state(SICK_1_AREA_4_PORT, SICK_1_AREA_4_PIN, FLAG_DISABLE);
}

void hokuyo_gpio_test_run(void)
{

}

#endif

/*************************************************************
 * Func:
 * Desc:
 *
 * @parm:
 * @ret:
 *
 ************************************************************/
#if (RUN_INA230_TEST == 1)

/* Test Conditions
 *
 *
 * */

#define TEST_INA230_IIC_ADDRESS INA230_2_IIC_ADDRESS

/*
 *
 * */
uint8_t print_buffer[PRINTK_BUFF_LENGTH];

void ina230_test_int(void)
{
	printk("TEST-> INA230 Test!\n\r");

	if(drv_ina230_iic_init(TEST_INA230_IIC_ADDRESS) == KERNEL_OK)
	{
		sprintf(print_buffer, "TEST-> INA230 Init SUCCESS! device addr [ 0x%x]\n\r", TEST_INA230_IIC_ADDRESS);
		printk(print_buffer);
	}
	else
	{
		sprintf(print_buffer, "TEST-> INA230 Init FAIL! device addr [ 0x%x]\n\r", TEST_INA230_IIC_ADDRESS);
		printk(print_buffer);
	}

}

void ina230_test_run(void)
{
	float read_volt = drv_ina230_iic_read_volt(TEST_INA230_IIC_ADDRESS);
	float read_curr = drv_ina230_iic_read_current(TEST_INA230_IIC_ADDRESS);

	if(read_volt != -1)
	{
		sprintf(print_buffer, "TEST-> INA230 read volt [ %d ] V\n\r", (int)read_volt);
		printk(print_buffer);
	}
	else
	{
		printk("TEST-> INA230 volt read FAIL!\n\r");
	}

	if(read_curr != -1)
	{
		sprintf(print_buffer, "TEST-> INA230 read curr [ %d ] A\n\r", (int)read_curr);
		printk(print_buffer);
	}
	else
	{
		printk("TEST-> INA230 curr read FAIL!\n\r");
	}

	drv_time_delay_ms(1000);
}

#endif

/*************************************************************
 * Func:
 * Desc:
 *
 * @parm:
 * @ret:
 *
 ************************************************************/
#if (RUN_MCP45HVX1_POT_TEST == 1)

uint8_t print_buffer[PRINTK_BUFF_LENGTH];

/* Change here for configure */
#define TEST_MCP45HVX1_ADDRESS	(0x78)


void mcp45hvx1_init(void)
{
	printk("TEST-> MCP45HVX1 POT Write Test!\n\r");

	if(drv_mcp45hvx1_iic_init(TEST_MCP45HVX1_ADDRESS) == KERNEL_OK)
	{
		printk("TEST-> MCP45HVX1 Init success!\n\r");

		/* Stacker testing AUTO_MAN relay set to go to auto mode */
		drv_gpio_set_pin_state(DRV_GPIO_PORT_B, GPIO_PIN_13,FLAG_SET);
		drv_gpio_set_pin_state(DRV_GPIO_PORT_B, GPIO_PIN_14,FLAG_RESET);


	}
	else
	{
		printk("TEST-> MCP45HVX1 Init failed!\n\r");
	}
}

void mcp45hvx1_run(void)
{
	kernel_status_type status = KERNEL_OK;

	for(int i=0 ; i< 255; i+= 5)
	{
		sprintf(print_buffer, "TEST-> MCP45HVX1 POT Val [ %d ]\n\r", i);
		printk(print_buffer);

		drv_gpio_set_pin_state(DRV_GPIO_PORT_B, GPIO_PIN_8, FLAG_SET);
		status |= drv_mcp45hvx1_set_pot(TEST_MCP45HVX1_ADDRESS, i);
		drv_gpio_set_pin_state(DRV_GPIO_PORT_B, GPIO_PIN_8, FLAG_RESET);

		(status == KERNEL_OK) ? printk("TEST-> MCP45HVX1 pot WR OK!\n\r") : printk("TEST-> MCP45HVX1 pot WR ERR!\n\r");

		HAL_Delay(2000);
	}
}

#endif

/*************************************************************
 * Func:
 * Desc:
 *
 * @parm:
 * @ret:
 *
 ************************************************************/
#if (RUN_MCP3427_READ_TEST == 1)


/* Give the test devive address */
#define MCP3427_TEST_ADDRESS (0xD0)




uint8_t print_buffer[PRINTK_BUFF_LENGTH];



void mcp3427_int(void)
{
	if(drv_mcp3427_iic_init(MCP3427_TEST_ADDRESS) == KERNEL_OK)
	{
		printk("TEST-> MCP3427 Init Success!\n\r");
	}
	else
	{
		printk("TEST-> MCP3427 Init Failed!\n\r");
	}
	HAL_Delay(1000);
}

void mcp3427_run(void)
{
	float ip_volt;
	int volt;

	//printk((uint8_t*)"TEST-> MCP3427 CH-1 Read\n\r");
	ip_volt = drv_mcp3427_iic_read_volt(MCP3427_TEST_ADDRESS,MCP3427_CH_1);
	volt = (int)(ip_volt*1000*5.0);
	sprintf(print_buffer, "TEST-> MCP3427 CH-1 Read mV [ %d ]\n\r\n\r", volt);
	printk(print_buffer);

	HAL_Delay(4);

	//printk((uint8_t*)"TEST-> MCP3427 CH-2 Read\n\r");
	ip_volt = drv_mcp3427_iic_read_volt(MCP3427_TEST_ADDRESS,MCP3427_CH_2);
	volt = (int)(ip_volt*1000*5.0);
	sprintf(print_buffer, "TEST-> MCP3427 CH-2 Read mV [ %d ]\n\r\n\r", volt);
	printk(print_buffer);
	HAL_Delay(4);
}

#endif


#if(RUN_FLASH_TEST == 1)

uint8_t rx_buffer[10] = {0};
uint8_t tx_buffer[10] = {0};
uint8_t print_buffer[PRINTK_BUFF_LENGTH];

void run_flash_read_write_test_init(void)
{

	printk("TEST-> Flash Driver Write/Read Test!\n\r");

	printk("TEST-> Tx Data: \n\r");
	for(int i=0; i< 20; i++)
	{
		tx_buffer[i] = i;
		sprintf(print_buffer, "[ %d ]\n\r", i);
		printk(print_buffer);
		vTaskDelay(100);
	}
	/* Writing to Flash */
	printk("\n\rTEST-> Writing to FLash...\n\r");
	uint32_t status = 0;
	status = drv_flash_write_data(MIN_PAGE_ACCESS_ADDRESS, tx_buffer, 20);
	sprintf(print_buffer, "Write Status[ 0x%x ]\n\r", status);
	printk(print_buffer);



	printk("TEST-> Reading Flash...\n\r");
	drv_flash_read_data(MIN_PAGE_ACCESS_ADDRESS, rx_buffer, 20);

	printk("TEST-> Rx Data: \n\r");
	for(int i=0; i< 20; i++)
	{
		vTaskDelay(100);
		sprintf(print_buffer, "[ %d ]\n\r", rx_buffer[i]);
		printk(print_buffer);
	}

}

void run_flash_read_write_test_run(void)
{

}

#endif


/*************************************************************
 * Func:
 * Desc:
 *
 * @parm:
 * @ret:
 *
 ************************************************************/
#if(RUN_TIMER_PWM_TEST == 1)

#define TEST_TIMER_ID			TIMER_1
#define TEST_TIMR_CHANNEL		TIM_CHANNEL_1
// %
static float dutycycle = 10.0;

uint8_t print_buffer[PRINTK_BUFF_LENGTH];

void run_timer_driver_pwm_test_init(void)
{
	printk("TEST-> Timer Driver PWM Test!\n\r");
	drv_gpio_set_pin(DRV_GPIO_PORT_E, GPIO_PIN_5);
}


void run_timer_driver_pwm_test_run(void)
{

	for(dutycycle	= 0;  dutycycle <= 100.0;  dutycycle +=10.0)
	{
		vTaskDelay(pdMS_TO_TICKS(2000));

		if(drv_timer_pwm_output(TEST_TIMER_ID , dutycycle, TEST_TIMR_CHANNEL) == DRV_OK)
		{
			sprintf(print_buffer, "TEST-> Timer[ %d ] duty cycle [ %d ] STARTED\n\r",
									TEST_TIMER_ID,
									(int)dutycycle);
			printk(print_buffer);
		}
		else
		{
			sprintf(print_buffer, "TEST-> Timer[ %d ] duty cycle [ %d ] FAILED\n\r",
									TEST_TIMER_ID,
									(int)dutycycle);
			printk(print_buffer);
		}
	}

	for(dutycycle	= 0;  dutycycle >= -100.0;  dutycycle -=10.0)
	{
		vTaskDelay(pdMS_TO_TICKS(1500));

		if(drv_timer_pwm_output(TEST_TIMER_ID , dutycycle, TEST_TIMR_CHANNEL) == DRV_OK)
		{
			sprintf(print_buffer, "TEST-> Timer[ %d ] duty cycle [ %d ] STARTED\n\r",
									TEST_TIMER_ID,
									(int)dutycycle);
			printk(print_buffer);
		}
		else
		{
			sprintf(print_buffer, "TEST-> Timer[ %d ] duty cycle [ %d ] FAILED\n\r",
									TEST_TIMER_ID,
									(int)dutycycle);
			printk(print_buffer);
		}
	}
}

#endif



/*************************************************************
 * Func:
 * Desc:
 *
 * @parm:
 * @ret:
 *
 ************************************************************/
void ManualTestRunner(void)
{
	/* Start time offset */
	vTaskDelay(pdMS_TO_TICKS(TIME_OFFSET_TEST_SUITE));
	/* setup condition */
#if (RUN_UART_LOOPBACK_TEST == 1)
	RunUartLoopbackTest_init();
#endif

#if (RUN_IIC_SERVICE_LOOPBACK_TEST == 1)
	IIC_Loopback_test_init();
#endif

#if (RUN_CAN_DRV_TX_TEST == 1)
	CAN_drv_tx_test_init();
#endif

#if (RUN_CAN_DRV_RX_TEST == 1)
	CAN_drv_rx_test_init();
#endif

#if (RUN_IIC_SCAN_TEST == 1)
	IIC_scan_test_init();
#endif

#if (RUN_PCA9685_WRITE_TEST == 1)
	pca9685_write_test_init();
#endif

#if (RUN_MCP23017_WRITE_TEST == 1)
	mcp23017_write_test_init();
#endif

#if (RUN_DS3502_WRITE_TEST == 1)
	ds3502_write_test_init();
#endif

#if (RUN_HOKUYO_GPIO_TEST == 1)
	hokuyo_gpio_test_init();
#endif

#if (RUN_INA230_TEST == 1)
	ina230_test_int();
#endif

#if (RUN_MCP45HVX1_POT_TEST == 1)
	mcp45hvx1_init();
#endif

#if(RUN_MCP3427_READ_TEST == 1)
	mcp3427_int();
#endif

#if (RUN_MCP4441_WRITE_TEST == 1)
	mcp4441_write_test_init();
#endif

#if(RUN_FLASH_TEST == 1)
	run_flash_read_write_test_init();
#endif

#if(RUN_TIMER_PWM_TEST == 1)
	run_timer_driver_pwm_test_init();
#endif

	/* running */
	while(true)
	{
		/* basic gpio toggling test */
#if (RUN_GPIO_TEST == 1)
		GPIOTest_run();
#endif
		/* Basic uart lookack test */
#if (RUN_UART_LOOPBACK_TEST == 1)
		RunUartLoopbackTest_Run();
#endif

#if (RUN_IIC_SERVICE_LOOPBACK_TEST == 1)
		IIC_Loopback_test_run();
#endif

#if (RUN_CAN_DRV_TX_TEST == 1)
		CAN_drv_tx_test_run();
#endif

#if (RUN_CAN_DRV_RX_TEST == 1)
		CAN_drv_rx_test_run();
#endif

#if (RUN_IIC_SCAN_TEST == 1)
	IIC_scan_test_run();
#endif

#if (RUN_PCA9685_WRITE_TEST == 1)
	pca9685_write_test_run();
#endif

#if (RUN_MCP23017_WRITE_TEST == 1)
	mcp23017_write_test_run();
#endif

#if (RUN_DS3502_WRITE_TEST == 1)
	ds3502_write_test_run();
#endif

#if (RUN_HOKUYO_GPIO_TEST == 1)
	hokuyo_gpio_test_run();
#endif

#if (RUN_INA230_TEST == 1)
	ina230_test_run();
#endif

#if (RUN_MCP45HVX1_POT_TEST == 1)
	mcp45hvx1_run();
#endif

#if(RUN_MCP3427_READ_TEST == 1)
	mcp3427_run();
#endif
#if (RUN_MCP4441_WRITE_TEST == 1)
	mcp4441_write_test_run();
#endif

#if(RUN_FLASH_TEST == 1)
	run_flash_read_write_test_run();
#endif

#if(RUN_TIMER_PWM_TEST == 1)
	run_timer_driver_pwm_test_run();
#endif
	}
}

