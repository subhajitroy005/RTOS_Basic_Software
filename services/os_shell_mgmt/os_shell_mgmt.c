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

#include "os_shell_mgmt.h"
#if (INC_SERVICE_OS_SHELL_MGMT == 1)
/* Driver stream buffers   */
int test_condition;

static uint8_t 	temp_byte_buff,
				byte_count,
				print_buffer[PRINTK_BUFF_LENGTH],
				timestamp_buffer[10], // For a max 32bit number
				/* REPL processing*/
				cmd_string_data[CMD_STRING_LENGTH],
				sub_cmd_string_data[CMD_STRING_LENGTH],
				cmd_string_index,
				sub_cmd_string_index,
				command_selector,
				cmd_running_state;


int printk(uint8_t *ptr)
{
#if (ITM_DEBUG_EN == 1) //  In ITM debug we don,t need time-stamp as of now
			drv_itm_print(ptr);
#else
	/*
	 * Print the time-stamp
	 * */
	int len,DataIdx;
	ATOMIC_ENTER_CRITICAL();
	if(global_debug_status.debug_global_en == FLAG_ENABLE)
	{
		sprintf((char*)timestamp_buffer,"[%d] ",(int)drv_time_get_ticks());
		len = strlen((char*)timestamp_buffer);

		for (DataIdx = 0; DataIdx < len; DataIdx++)
		{
			temp_byte_buff = timestamp_buffer[DataIdx];
			ringbuffer_putchar(&pipe_uart_1_drv_tx_handle, temp_byte_buff);
		}
	}
	/*
	 * Print the String
	 * */
	len = strlen((char*)ptr);
	if(global_debug_status.debug_global_en == FLAG_ENABLE)
	{
		for (DataIdx = 0; DataIdx < len; DataIdx++)
		{
			temp_byte_buff = ptr[DataIdx];
			ringbuffer_putchar(&pipe_uart_1_drv_tx_handle, temp_byte_buff);
		}
		memset(ptr,0,PRINTK_BUFF_LENGTH);
	}
	ATOMIC_EXIT_CRITICAL();
	return len;
#endif
}

int print_os_shell(uint8_t *ptr)
{
#if (ITM_DEBUG_EN == 1)
			drv_itm_print(ptr);
#else
	int DataIdx;
	int len = strlen((char*)ptr);
	for (DataIdx = 0; DataIdx < len; DataIdx++)
	{
		temp_byte_buff = ptr[DataIdx];
		byte_count = ringbuffer_putchar(&pipe_uart_1_drv_tx_handle, temp_byte_buff);



	}
	memset(ptr,0,PRINTK_BUFF_LENGTH);
	return len;
#endif
}

/* Repel related support function */
static void shell_print_home(void)
{
	sprintf((char*)&print_buffer[0],"\n\r%s >",SYSTEM_NAME);
	print_os_shell(&print_buffer[0]);
}

static void shell_print_version(void)
{
	print_os_shell((uint8_t*)"************************\n\r");
	sprintf((char*)print_buffer,"System:     %s\n\r"		,SYSTEM_NAME);
	print_os_shell(&print_buffer[0]);
	sprintf((char*)print_buffer,"Branch: %s\n\r"			,CURR_BRANCH);
	print_os_shell(&print_buffer[0]);
	sprintf((char*)print_buffer,"Commit_ID: %s\n\r"			,COMMIT_ID);
    print_os_shell(&print_buffer[0]);
    sprintf((char*)print_buffer,"Last Update: %s\n\r"		,TIME_STAMP);
    print_os_shell(&print_buffer[0]);
	print_os_shell((uint8_t*)"************************\n\r");
}

#if (ITM_DEBUG_EN == 0)

static void shell_print_abort(uint8_t slect)
{
	switch(slect)
	{
		case 1:
			sprintf((char*)&print_buffer[0],"Wrong cmd!");
			print_os_shell(&print_buffer[0]);
		break;

		case 2:
			sprintf((char*)&print_buffer[0],"Wrong argument!");
			print_os_shell(&print_buffer[0]);
		break;
	}

}

/* Send data */

void execute_cmd(void)
{
	/* if just a empty return then do nothing */
	if((strlen((char*)cmd_string_data) == 0) && (strlen((char*)sub_cmd_string_data) == 0) )
		return;

	if(strcmp((char*)cmd_string_data , (char*)"test") == 0)
	{
		if(strcmp((char*)sub_cmd_string_data , (char*)"iic") == 0)
		{
			/* read */


			cmd_running_state = CMD_RUNNING;
		}
		else
		{
			shell_print_abort(2);
		}
	}


	else if(strcmp((char*)cmd_string_data , (char*)"debug") == 0)
	{

		if(strcmp((char*)sub_cmd_string_data , "en") == 0)
		{
			print_os_shell((uint8_t*)"Serial debug print enabled!");
			global_debug_status.debug_global_en = FLAG_ENABLE;
			cmd_running_state = CMD_RUNNING;
		}

		else if(strcmp((char*)sub_cmd_string_data , (char*)"dis") == 0)
		{
			global_debug_status.debug_global_en = FLAG_DISABLE;
			print_os_shell((uint8_t*)"Serial debug print disabled!");
			cmd_running_state = CMD_NOT_RUNNING;
		}

		else
		{
			shell_print_abort(2);
		}
	}

	else if(strcmp((char*)cmd_string_data , (char*)"gw") == 0)
	{

		if(strcmp((char*)sub_cmd_string_data , "en") == 0)
		{
			print_os_shell((uint8_t*)"CAN MGMT: Gateway Enabled!\n\r");
			global_debug_status.can_mgmt_gw_status        = FLAG_ENABLE;
			global_debug_status.can_mgmt_gw_status_en     = FLAG_ENABLE;
			cmd_running_state = CMD_RUNNING;
		}

		else if(strcmp((char*)sub_cmd_string_data , (char*)"dis") == 0)
		{
			print_os_shell((uint8_t*)"CAN MGMT: Gateway Disabled!\n\r");
			global_debug_status.can_mgmt_gw_status        = FLAG_DISABLE;
			global_debug_status.can_mgmt_gw_status_en     = FLAG_ENABLE;
			cmd_running_state = CMD_RUNNING;
		}

		else
		{
			shell_print_abort(2);
		}
	}

//	else if(strcmp((char*)cmd_string_data , (char*)"th") == 0)
//	{
//
//		if(strcmp((char*)sub_cmd_string_data , "en") == 0)
//		{
//			print_os_shell((uint8_t*)"Throttle: Set to 20% !\n\r");
//			cmd_running_state = CMD_RUNNING;
//
//			/* Object dictionary of throttle */
//			master_tpdo_0_act_data_2_16 = 0x49F4;
//
//
//		}
//
//		else if(strcmp((char*)sub_cmd_string_data , (char*)"dis") == 0)
//		{
//			print_os_shell((uint8_t*)"Throttle: Set to 0% !\n\r\n\r");
//			cmd_running_state = CMD_RUNNING;
//
//			master_tpdo_0_act_data_2_16 = 0;
//
//		}
//
//		else
//		{
//			shell_print_abort(2);
//		}
//	}
//
//	else if(strcmp((char*)cmd_string_data , (char*)"intlk") == 0)
//	{
//
//		if(strcmp((char*)sub_cmd_string_data , "en") == 0)
//		{
//			print_os_shell((uint8_t*)"Interlock SET !\n\r");
//			cmd_running_state = CMD_RUNNING;
//
///* Interlock */
//			can_tx_buffer.can_dev_id 	= CAN_2;
//			can_tx_buffer.dlc 			= 8;
//			can_tx_buffer.m_id			= 0x626;
//			can_tx_buffer.data[0]		= 0x22;
//			can_tx_buffer.data[1]		= 0x16;
//			can_tx_buffer.data[2]		= 0x30;
//			can_tx_buffer.data[3]		= 00;
//			can_tx_buffer.data[4]       = 0xAC;
//			can_tx_buffer.data[5]		= 0x0D;
//			can_tx_buffer.data[6]		= 0;
//			can_tx_buffer.data[7]		= 0;
//			xQueueSend(pipe_can_app_tx_handle,
//						&can_tx_buffer,
//						(TickType_t)pdMS_TO_TICKS(1)
//					  );
//
//
//		}
//
//		else if(strcmp((char*)sub_cmd_string_data , (char*)"dis") == 0)
//		{
//			print_os_shell((uint8_t*)"Interlock RESET\n\r");
//			cmd_running_state = CMD_RUNNING;
//
//			can_tx_buffer.can_dev_id 	= CAN_2;
//			can_tx_buffer.dlc 			= 8;
//			can_tx_buffer.m_id			= 0x626;
//			can_tx_buffer.data[0]		= 0x22;
//			can_tx_buffer.data[1]		= 0x16;
//			can_tx_buffer.data[2]		= 0x30;
//			can_tx_buffer.data[3]		= 00;
//			can_tx_buffer.data[4]       = 0x9C;
//			can_tx_buffer.data[5]		= 0x18;
//			can_tx_buffer.data[6]		= 0;
//			can_tx_buffer.data[7]		= 0;
//			xQueueSend(pipe_can_app_tx_handle,
//						&can_tx_buffer,
//						(TickType_t)pdMS_TO_TICKS(1)
//					  );
//
//		}
//
//		else
//		{
//			shell_print_abort(2);
//		}
//	}
//
//	else if(strcmp((char*)cmd_string_data , (char*)"mast") == 0)
//	{
//
//		if(strcmp((char*)sub_cmd_string_data , "up") == 0)
//		{
//			print_os_shell((uint8_t*)"MAST: UP !\n\r");
//			cmd_running_state = CMD_RUNNING;
///* Mast */
//			master_tpdo_0_act_data_0_8			= 0x11;
//			master_tpdo_0_act_data_1_8			= 0;
//			master_tpdo_0_act_data_2_16 		= 0;
//			master_tpdo_0_act_data_4_8			= 0x04;
//			master_tpdo_0_act_data_5_8			= 0;
//			master_tpdo_0_act_data_6_8			= 0x31;
//			master_tpdo_0_act_data_7_8			= 0x0;
//
//			vTaskDelay(pdMS_TO_TICKS(2000));
//
//			master_tpdo_0_act_data_0_8			= 0x11;
//			master_tpdo_0_act_data_1_8			= 0;
//			master_tpdo_0_act_data_2_16 		= 0;
//			master_tpdo_0_act_data_4_8			= 0x00;
//			master_tpdo_0_act_data_5_8			= 0;
//			master_tpdo_0_act_data_6_8			= 0x31;
//			master_tpdo_0_act_data_7_8			= 0x0;
//
//
//		}
//
//		else if(strcmp((char*)sub_cmd_string_data , (char*)"down") == 0)
//		{
//			print_os_shell((uint8_t*)"MAST: DOWN !\n\r");
//			cmd_running_state = CMD_RUNNING;
//
//			master_tpdo_0_act_data_0_8			= 0x11;
//			master_tpdo_0_act_data_1_8			= 0;
//			master_tpdo_0_act_data_2_16 		= 0;
//			master_tpdo_0_act_data_4_8			= 0x10;
//			master_tpdo_0_act_data_5_8			= 0;
//			master_tpdo_0_act_data_6_8			= 0x31;
//			master_tpdo_0_act_data_7_8			= 0x00;
//
//			vTaskDelay(pdMS_TO_TICKS(4000));
//
//			master_tpdo_0_act_data_0_8			= 0x11;
//			master_tpdo_0_act_data_1_8			= 0;
//			master_tpdo_0_act_data_2_16 		= 0;
//			master_tpdo_0_act_data_4_8			= 0x00;
//			master_tpdo_0_act_data_5_8			= 0;
//			master_tpdo_0_act_data_6_8			= 0x31;
//			master_tpdo_0_act_data_7_8			= 0x0;
//
//		}
//
//		else
//		{
//			shell_print_abort(2);
//		}
//	}
//
//	else if(strcmp((char*)cmd_string_data , (char*)"horn") == 0)
//	{
//
//		if(strcmp((char*)sub_cmd_string_data , "en") == 0)
//		{
//			print_os_shell((uint8_t*)"Horn: EN 1S !\n\r");
//			cmd_running_state = CMD_RUNNING;
///* HORN */
//			master_tpdo_0_act_data_0_8			= 0x11;
//			master_tpdo_0_act_data_1_8			= 0;
//			master_tpdo_0_act_data_2_16 		= 0;
//			master_tpdo_0_act_data_4_8			= 0x08;
//			master_tpdo_0_act_data_5_8			= 0;
//			master_tpdo_0_act_data_6_8			= 0x31;
//			master_tpdo_0_act_data_7_8			= 0x0;
//
//			vTaskDelay(pdMS_TO_TICKS(1000));
//
//			master_tpdo_0_act_data_0_8			= 0x11;
//			master_tpdo_0_act_data_1_8			= 0;
//			master_tpdo_0_act_data_2_16 		= 0;
//			master_tpdo_0_act_data_4_8			= 0x00;
//			master_tpdo_0_act_data_5_8			= 0;
//			master_tpdo_0_act_data_6_8			= 0x31;
//			master_tpdo_0_act_data_7_8			= 0x0;
//
//		}
//
//		else if(strcmp((char*)sub_cmd_string_data , (char*)"dis") == 0)
//		{
//			print_os_shell((uint8_t*)"HORN: OFF !\n\r\n\r");
//			cmd_running_state = CMD_RUNNING;
//
//		}
//
//		else
//		{
//			shell_print_abort(2);
//		}
//	}

	else
	{
		shell_print_abort(1);
	}
}

#endif


static void service_os_shell_mgmt_init(void)
{
	/* Variables initialize */
	command_selector = CMD_SELECT; // default first command will be there
	cmd_running_state = CMD_RUNNING;
	cmd_string_index = 0;
	sub_cmd_string_index = 0;
	memset(cmd_string_data, 0, CMD_STRING_LENGTH);
	memset(sub_cmd_string_data, 0, CMD_STRING_LENGTH);

	/* Disables all global debug status */
	global_debug_status.debug_global_en 		= FLAG_ENABLE;
	global_debug_status.debug_host_comm_mgmt_rx = FLAG_DISABLE;
	global_debug_status.debug_host_comm_mgmt_tx = FLAG_DISABLE;

#if (ITM_DEBUG_EN == 1)
	drv_itm_init();
	vTaskDelay(10);
#endif
	/* Print the sw version*/
	shell_print_version();
	/* go to shell home */
	print_os_shell((uint8_t*)"Debug log printing...\n\r");

#if (ITM_DEBUG_EN == 0)
	print_os_shell((uint8_t*)"Press ESC to go in CMD mode\n\r");
#endif
	//printf("\n\r%s >",SYSTEM_NAME);
	/* Service status update */
	global_service_status.os_shell_mgmt_service_staus = KERNEL_OK;
}

/* Incoming byte processing */
#if (ITM_DEBUG_EN == 0)
static void incomming_char_processing(void)
{
	byte_count = ringbuffer_getchar(&pipe_uart_1_drv_rx_handle, &temp_byte_buff);

	if(byte_count != 0)
	{
		/* receive a cartridge return meaning
		 * execute the comand*/
		/*Echo the character */
		print_buffer[0] = temp_byte_buff;
		print_os_shell(print_buffer);

		switch(temp_byte_buff)
		{
			case '\r':
				/*anf any command is not running */
				if(cmd_running_state == CMD_NOT_RUNNING)
				{
					execute_cmd();
					/* command executing complete so clear the command string */
					memset(cmd_string_data, 0, CMD_STRING_LENGTH);
					memset(sub_cmd_string_data, 0, CMD_STRING_LENGTH);
					/*index also reset */
					cmd_string_index = sub_cmd_string_index = 0;
					/*back to command receive mode */
					command_selector = CMD_SELECT;
					/*goto home */
					shell_print_home();
					cmd_running_state = CMD_NOT_RUNNING;
				}

			break;

			/* escape character */
			case 27: //
				cmd_running_state = CMD_NOT_RUNNING;
				/*all debug disabled */
				global_debug_status.debug_global_en = FLAG_DISABLE;
				print_os_shell((uint8_t*)"Debug Log disabled!\n\r");
//				send_string(HOME_STRING, strlen(HOME_STRING));
				/* Command aborted so clear  */
				memset(cmd_string_data, 0, CMD_STRING_LENGTH);
				memset(sub_cmd_string_data, 0, CMD_STRING_LENGTH);
				/*index also reset */
				cmd_string_index = sub_cmd_string_index = 0;
				/*back to command receive mode */
				command_selector = CMD_SELECT;
				/*goto home */
				shell_print_home();
			break;

			/* space character */
			case 32:
				/*Space has received so next is sub command */
				command_selector = SUB_CMD_SELECT;
			break;

			/* back space */
			case 8:
				switch(command_selector)
				{
					case CMD_SELECT:
						cmd_string_index--;
					break;

					case SUB_CMD_SELECT:
						sub_cmd_string_index--;
					break;
				}
			break;

			/* start receiving command and arguments */
			default:
				switch(command_selector)
				{
					case CMD_SELECT:
						cmd_string_data[cmd_string_index++] = temp_byte_buff;
					break;

					case SUB_CMD_SELECT:
						sub_cmd_string_data[sub_cmd_string_index++] = temp_byte_buff;
					break;
				}
			break;

		}
	}
}

#endif

void proc_service_os_shell_mgmt(void * parm)
{
	/* Time offset */
	vTaskDelay(pdMS_TO_TICKS(TIME_OFFSET_OS_SHELL_MGMT));
	test_condition = *((int *)parm);
	/* Init case */
	service_os_shell_mgmt_init();
	while(true)
	{
#if (ITM_DEBUG_EN == 0)
		/* if not the manual test is running*/
		if(test_condition != 1)
		{
			/* rx handle case */
			incomming_char_processing();
		}
#else
		vTaskDelay(10000);
#endif
	}
}


/* printf system calls functionality */


#endif




