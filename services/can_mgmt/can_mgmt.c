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

#include "can_mgmt.h"

#if (DRV_DEBUG_EN == 1)
static uint8_t 			   		print_buffer[PRINTK_BUFF_LENGTH];
#endif

static drv_can_handle_type 		*can_driver_handle;
static TimerHandle_t			co_ticks_timer_handle;
static StaticTimer_t			co_ticks_timer_buffer;
static uint8_t					gw_en_flag 				= FLAG_SET;
static can_pdu_struct_type 		local_rx_buff_data,local_tx_buff_data;
static uint8_t					status;
static uint32_t 				count = 0;
/* *************** [ Process functions : init ] *********]
 *
 *
 * ***************************************************
 *  */


/* *************** [ call back declaration ] *********]
 *
 *
 * ***************************************************
 *  */
static void co_fb_timer_cb( TimerHandle_t xTimer );


/* *************** [ Process functions : init ] *********]
 *
 *
 * ***************************************************
 *  */
/**
 * drv_serial_init
 * @ret:
 *
 */
static kernel_status_type proc_service_can_mgmt_init(void)
{
	global_service_status.can_mgmt_service_status = KERNEL_OK;

	can_driver_handle = drv_can_get_handle();

	/* driver level int */
#if (PERIPHERAL_CAN_1_EN == 1)
	if(drv_can_init(CAN_1) == DRV_OK)
	{
#if (DRV_DEBUG_EN == 1)
		printk((uint8_t*)"[ CAN-DRV ][OK] CAN_1 DRV Init success!\n\r");

#endif
	}
	else
	{
		global_service_status.can_mgmt_service_status |= KERNEL_ERR;

		printk((uint8_t*)"[ CAN-DRV ][ERR] CAN_1 DRV Init failed!\n\r");
	}
#endif


#if (PERIPHERAL_CAN_2_EN == 1)
	if(drv_can_init(CAN_2) == DRV_OK)
	{
#if (DRV_DEBUG_EN == 1)
		printk((uint8_t*)"[ CAN-DRV ][OK] CAN_2 DRV Init success!\n\r");

#endif
	}
	else
	{
		global_service_status.can_mgmt_service_status |= KERNEL_ERR;

		printk((uint8_t*)"[ CAN-DRV ][ERR] CAN_2 DRV Init failed!\n\r");
	}
#endif


#if (PERIPHERAL_CAN_3_EN == 1)
	if(drv_can_init(CAN_3) == DRV_OK)
	{
#if (DRV_DEBUG_EN == 1)
		printk((uint8_t*)"[ CAN-DRV ][OK] CAN_3 DRV Init success!\n\r");

#endif
	}
	else
	{
		global_service_status.can_mgmt_service_status |= KERNEL_ERR;

		printk((uint8_t*)"[ CAN-DRV ][ERR] CAN_3 DRV Init failed!\n\r");
	}
#endif

	/* RTOS timer : This is the heart of all CO Timer services */
	co_ticks_timer_handle = xTimerCreateStatic("CO_FB_TIMER",
											pdMS_TO_TICKS(CO_TIMER_ISR_PERIOD),
											pdTRUE,
											( void * ) 0,
											co_fb_timer_cb,
											&co_ticks_timer_buffer
											);
	if(co_ticks_timer_handle == NULL)
	{
		global_service_status.can_mgmt_service_status |= KERNEL_ERR;
	}
	else{
		count = count +1;
		if (PERIPHERAL_CAN_1_EN == 1){
			syscall_diagnostics_log(ERR_1003_CO_CAN1_TIM, count);
		}
		else if (PERIPHERAL_CAN_2_EN == 1){
			syscall_diagnostics_log(ERR_1028_CO_CAN2_TIM, count);
		}
		else if(PERIPHERAL_CAN_3_EN == 1){
			syscall_diagnostics_log(ERR_1053_CO_CAN3_TIM, count);
		}
		else{
			syscall_diagnostics_log(ERR_1078_CO_CAN4_TIM, count);
		}
	}




	if(global_service_status.can_mgmt_service_status == KERNEL_OK)
	{
#if(NO_OF_ACTIVE_CO_NODES > 0)
		/* Initializing the CANOPEN nodes */
		for(int i=0 ; i< NO_OF_ACTIVE_CO_NODES ; i++ )
		{
			CONodeInit(	global_canopen_handles[i].co_node_handle,
						global_canopen_handles[i].co_node_slave_spec
						);

			if(CONodeGetErr(global_canopen_handles[i].co_node_handle) == CO_ERR_NONE)
			{
				uint32_t native_node_timer_ticks 				= COTmrGetTicks(&(global_canopen_handles[i].co_node_handle->Tmr),
																				CO_TIMER_ISR_PERIOD,
																				CO_TMR_UNIT_1MS
																				);
				uint32_t native_node_timer_start_delay_ticks 	= COTmrGetTicks(&(global_canopen_handles[i].co_node_handle->Tmr),
																				CO_TIMER_ISR_PERIOD,
																				CO_TMR_UNIT_1MS
																				);
				status = FLAG_RESET;
				status = COTmrCreate(&(global_canopen_handles[i].co_node_handle->Tmr),
									native_node_timer_start_delay_ticks ,
									native_node_timer_ticks,
									global_canopen_handles[i].timer_cb_fnc,
									NULL
									);
				/* No fault in timer creation then start the node */
				if(status > 0)
				{

					/* Slave started as pre-operational */
					CONmtSetMode(	&(global_canopen_handles[i].co_node_handle->Nmt),
									global_canopen_handles[i].co_node_init_state
								);
					CONodeStart(global_canopen_handles[i].co_node_handle);

					/* Updating the timer time stamps */
					global_canopen_handles[i].pdo_trigger_time_stamp	= drv_time_get_ticks();

					sprintf((char*)print_buffer, "[ CAN MGMT : CO] Node[0x%x] Started!\n\r",
												(int)global_canopen_handles[i].co_node_handle->NodeId);
					printk(print_buffer);
				}
				else
				{
					sprintf((char*)print_buffer, "[ CAN MGMT : CO] Node[0x%x] Failed : Timer err!\n\r",
												(int)global_canopen_handles[i].co_node_handle->NodeId);
					printk(print_buffer);
				}
			}
			else
			{
				sprintf((char*)print_buffer, "[ CAN MGMT : CO] Node[0x%x] Failed : Node init err [%d]!\n\r",(int)global_canopen_handles[i].co_node_handle->NodeId,
																											CONodeGetErr(global_canopen_handles[i].co_node_handle)
																											);
				printk(print_buffer);
			}
		}
		/*Start the RTOS timer for CO timer service */
		/* Start the timer */
		xTimerStart(co_ticks_timer_handle,pdMS_TO_TICKS(0));
		vTaskDelay(pdMS_TO_TICKS(1));

#endif

		if(gw_en_flag == FLAG_ENABLE)
		{
			printk((uint8_t*)"[ CAN MGMT ] CAN gateway Enabled! \n\r");
		}
		else
		{
			printk((uint8_t*)"[ CAN MGMT ] CAN gateway Disabled! \n\r");
		}
	}

	return (kernel_status_type)global_service_status.can_mgmt_service_status;
}

/* *************** [ Process functions : task] *********]
 *
 *
 * ***************************************************
 *  */
/**
 * drv_serial_init
 * @ret:
 *
 */
void proc_service_can_mgmt(void * parm)
{
	/* Start time offset */
	vTaskDelay(pdMS_TO_TICKS(TIME_OFFSET_CAN_MANAGEMENT));

	/* if initialization failed don't run the task */
	if(proc_service_can_mgmt_init() != KERNEL_OK)
	{
		printk((uint8_t*)"[ CAN MGMT ] [ ERR ] Service suspended! \n\r");
		vTaskSuspend(NULL);
	}
	else
	{
		printk((uint8_t*)"[ CAN MGMT ] [ OK ] Service started! \n\r");
	}

	while(true)
	{
#if (CAN_MGMT_GATEWAY == 1)
		if (gw_en_flag == FLAG_SET)
		{

			if(xQueueReceive(pipe_can_1_drv_rx_handle,
		         	&local_rx_buff_data,
		            ( TickType_t )0
				 ) == pdTRUE)
			{
				if(drv_can_transmit(CAN_2, &local_rx_buff_data) == DRV_OK)
				{
#if (GW_DEBUG_EN == 1)
					sprintf((char*)print_buffer, "[CAN_MGMT:GW] [0x%x][%d]CAN1->CAN2\n\r",(int)local_rx_buff_data.m_id,local_rx_buff_data.dlc);
					printk(print_buffer);
#endif
				}
				else
				{
					printk((uint8_t*)"[CAN-DRV] CAN_2 DRV TX failed!\n\r");
				}
			}


			if(xQueueReceive( pipe_can_2_drv_rx_handle,
		         	&local_rx_buff_data,
		            ( TickType_t )0
				 ) == pdTRUE)
			{
				if(drv_can_transmit(CAN_1, &local_rx_buff_data) == DRV_OK)
				{
#if (GW_DEBUG_EN == 1)
					sprintf((char*)print_buffer, "[CAN_MGMT:GW] [0x%x][%d]CAN2->CAN1\n\r",(int)local_rx_buff_data.m_id,local_rx_buff_data.dlc);
					printk(print_buffer);
#endif
				}
				else
				{
					printk((uint8_t*)"[CAN-DRV] CAN_1 DRV TX failed!\n\r");
				}
			}
		}
#endif
#if(NO_OF_ACTIVE_CO_NODES > 0) // If the CANOPen architecture implemented
		for(int i=0; i<NO_OF_ACTIVE_CO_NODES; i++ )
		{
			if(global_canopen_handles[i].co_node_is_active == FLAG_ENABLE)
			{
				CONodeProcess(global_canopen_handles[i].co_node_handle);
				COTmrProcess(&(global_canopen_handles[i].co_node_handle->Tmr));
			}
		}
#else // Normal queue based operation
#if (PERIPHERAL_CAN_1_EN == 1)
		if(xQueueReceive(pipe_can_1_drv_rx_handle,
						 &local_tx_buff_data,
						 pdMS_TO_TICKS(0)) == pdTRUE )
		{
			if(xQueueSend(pipe_can_app_rx_handle,
					   &local_tx_buff_data,
					   pdMS_TO_TICKS(1)) != pdTRUE)
			{

			}
		}
#endif

#if (PERIPHERAL_CAN_2_EN == 1)
		if(xQueueReceive(pipe_can_2_drv_rx_handle,
						 &local_tx_buff_data,
						 pdMS_TO_TICKS(0)) == pdTRUE )
		{
			if(xQueueSend(pipe_can_app_rx_handle,
					   &local_tx_buff_data,
					   pdMS_TO_TICKS(1)) != pdTRUE)
			{

			}
		}
#endif

#if (PERIPHERAL_CAN_3_EN == 1)
		if(xQueueReceive(pipe_can_3_drv_rx_handle,
						 &local_tx_buff_data,
						 pdMS_TO_TICKS(0)) == pdTRUE )
		{
			if(xQueueSend(pipe_can_app_rx_handle,
					   &local_tx_buff_data,
					   pdMS_TO_TICKS(1)) != pdTRUE)
			{

			}
		}
#endif

#endif
		if(drv_can_get_protocol_status(local_rx_buff_data.can_dev_id) != DRV_OK){
			sprintf((char*)print_buffer, "[CAN_MGMT] CAN-%d Get Protocol Status Failed!\n\r",(int)local_rx_buff_data.can_dev_id);
			printk(print_buffer);
		}


		if(xQueuePeek(pipe_can_app_tx_handle,
					&local_rx_buff_data,
					( TickType_t )1) == pdTRUE)
		{
			if(drv_can_get_tx_buffer_pending_status(local_rx_buff_data.can_dev_id) == 0)
			{
				/* application level custom CAN messages request */
				if(xQueueReceive( pipe_can_app_tx_handle,
								&local_rx_buff_data,
								( TickType_t )1
					 ) == pdTRUE)
				{
					if(drv_can_transmit(local_rx_buff_data.can_dev_id, &local_rx_buff_data) != DRV_OK)
					{
						sprintf((char*)print_buffer, "[CAN_MGMT] CAN-%d APP TX failed!\n\r",(int)local_rx_buff_data.can_dev_id);
						printk(print_buffer);
					}
				}
			}
		}
	}/* End of while(1)*/
}

/******************System calls*********************/
void syscall_can_mgmt_gw_en(uint8_t en_flag)
{
	if(en_flag == FLAG_SET)
	{

		/* Enable the gateway flag */
		gw_en_flag = FLAG_ENABLE;
		/* Debug print */
		printk((uint8_t*)"[CAN_MGMT:GW] Gateway enabled!\n\r");
	}
	else
	{
		/* Disable the gateway flag */
		gw_en_flag = FLAG_DISABLE;
		/* Debug print */
		printk((uint8_t*)"[CAN_MGMT:GW] Gateway disabled!\n\r");
	}
}

void syscall_can_mgmt_co_node_en(uint8_t node_index, uint8_t en_flag)
{
	if(node_index < NO_OF_ACTIVE_CO_NODES)
	{
		global_canopen_handles[node_index].co_node_is_active = en_flag;

		sprintf((char*)print_buffer, "[CAN_MGMT:CO] CO NODE - 0x%x status %d!\n\r",
										(int)global_canopen_handles[node_index].co_node_handle->NodeId,
										en_flag);
		printk(print_buffer);
	}
	else
	{
		sprintf((char*)print_buffer, "[CAN_MGMT:CO] CO NODE EN index %d not found!\n\r",node_index);
		printk(print_buffer);

	}
}

/* *************** [ Call Back functions ] *********]
 *
 *
 * ***************************************************
 *  */
/**
 * ros_fb_timer_cb
 * Timer ISR for CO timer
 * @ret: N/A
 *
 */
void co_fb_timer_cb( TimerHandle_t xTimer )
{
	/* Timer process */

	for(int i=0; i<NO_OF_ACTIVE_CO_NODES; i++ )
	{
		/* If the Node is active then do the process */
		if(global_canopen_handles[i].co_node_is_active == FLAG_ENABLE)
		{
			/* Timer service */
			COTmrService(&(global_canopen_handles[i].co_node_handle->Tmr));
			/* Checking for interval */
			if( (drv_time_get_ticks() - global_canopen_handles[i].pdo_trigger_time_stamp) >= \
				global_canopen_handles[i].pdo_trigger_interval
				)
			{
				for(int j=0; ( (j < global_canopen_handles[i].co_no_of_tx_pdo ) && ( j < 4) ); j++)
				{
					COTPdoTrigPdo(&(global_canopen_handles[i].co_node_handle->TPdo[0]), j);
				}
				/* Update the time stamp */
				global_canopen_handles[i].pdo_trigger_time_stamp = drv_time_get_ticks();
			}
		}
	}
}

void cb_co_timer(void * arg)
{

}
/**
 * drv_serial_init
 * @ret:
 *
 */
void CONodeFatalError(void)
{
    volatile uint8_t debugExit = 0u;


    printk((uint8_t *)"[ CO MGMT: CB] Fatal Error!\n\r");
    count = count+1;
    if (PERIPHERAL_CAN_1_EN == 1){
		syscall_diagnostics_log(ERR_1004_CO_CAN1_NODE_FATAL, count);
	}
	else if (PERIPHERAL_CAN_2_EN == 1){
		syscall_diagnostics_log(ERR_1029_CO_CAN2_NODE_FATAL, count);
	}
	else if (PERIPHERAL_CAN_3_EN == 1){
		syscall_diagnostics_log(ERR_1054_CO_CAN3_NODE_FATAL, count);
	}
	else{
		syscall_diagnostics_log(ERR_1079_CO_CAN4_NODE_FATAL, count);
	}
    /* Place here your fatal error handling.
     * There is most likely a programming error.
     * !! Please don't ignore this errors. !!
     */
    for (;debugExit == 0u;);


}

void COTmrLock(void)
{
    /* This function helps to guarantee the consistancy
     * of the internal timer management while interrupted
     * by the used timer interrupt. Most likely you need
     * at this point on of the following mechanisms:
     * - disable the used hardware timer interrupt
     * - get a 'timer-mutex' from your RTOS (ensure to
     *   call COTmrService() in a timer triggered task)
     */
}

void COTmrUnlock(void)
{
    /* This function helps to guarantee the consistancy
     * of the internal timer management while interrupted
     * by the used timer interrupt. Most likely you need
     * at this point on of the following mechanisms:
     * - (re)enable the used hardware timer interrupt
     * - release the 'timer-mutex' from your RTOS (ensure
     *   to call COTmrService() in a timer triggered task)
     */
}

/******************************************************************************
* OPTIONAL CALLBACK FUNCTIONS
******************************************************************************/

void CONmtModeChange(CO_NMT *nmt, CO_MODE mode)
{
    (void)nmt;
    (void)mode;

    /* Optional: place here some code, which is called
     * when a NMT mode change is initiated.
     */
    //printk("[ CO MGMT : CB] Mode Change!\n\r");
}

void CONmtResetRequest(CO_NMT *nmt, CO_NMT_RESET reset)
{
    (void)nmt;
    (void)reset;

    /* Optional: place here some code, which is called
     * when a NMT reset is requested by the network.
     */
    printk((uint8_t *)"[ CO MGMT : CB] Reset Req!\n\r");
    printk((uint8_t *)"Reseting node......\n\r");
    reset_mcu();
}

void CONmtHbConsEvent(CO_NMT *nmt, uint8_t nodeId)
{
    (void)nmt;
    (void)nodeId;

    /* Optional: place here some code, which is called
     * called when heartbeat consumer is in use and
     * detects an error on monitored node(s).
     */
    printk((uint8_t *)"[ CO MGMT : CB] Heart Beat error!\n\r");
    count = count+1;
	if (PERIPHERAL_CAN_1_EN == 1){
		syscall_diagnostics_log(ERR_1005_CO_CAN1_HB, count);
	}
	else if (PERIPHERAL_CAN_2_EN == 1){
		syscall_diagnostics_log(ERR_1030_CO_CAN2_HB, count);
	}
	else if(PERIPHERAL_CAN_3_EN == 1){
		syscall_diagnostics_log(ERR_1055_CO_CAN3_HB, count);
	}
	else{
		syscall_diagnostics_log(ERR_1080_CO_CAN4_HB, count);
	}
}

void CONmtHbConsChange(CO_NMT *nmt, uint8_t nodeId, CO_MODE mode)
{
    (void)nmt;
    (void)nodeId;
    (void)mode;

    /* Optional: place here some code, which is called
     * when heartbeat consumer is in use and detects a
     * NMT state change on monitored node(s).
     */
    printk((uint8_t *)"[ CO MGMT :CB] Heart Beat change!\n\r");
}

CO_ERR COLssLoad(uint32_t *baudrate, uint8_t *nodeId)
{
    (void)baudrate;
    (void)nodeId;

    /* Optional: place here some code, which is called
     * when LSS client is in use and the CANopen node
     * is initialized.
     */
    printk((uint8_t *)"[ CO MGMT :CB] Lss Load!\n\r");
    return (CO_ERR_NONE);
}

CO_ERR COLssStore(uint32_t baudrate, uint8_t nodeId)
{
    (void)baudrate;
    (void)nodeId;

    /* Optional: place here some code, which is called
     * when LSS client is in use and the CANopen node
     * needs to store updated values.
     */
    printk((uint8_t *)"[ CO MGMT :CB] Lss Store!\n\r");
    return (CO_ERR_NONE);
}

void COIfCanReceive(CO_IF_FRM *frm)
{
    /* Optional: place here some code, which is called
     * when you need to handle CAN messages, which are
     * not part of the CANopen protocol.
     */
	can_pdu_struct_type local_rx_buff_data;

	local_rx_buff_data.can_dev_id 	= 0xFF;  // generic based on CO protocol So no specific CAN instance
	local_rx_buff_data.m_id	 		= frm->Identifier;
	local_rx_buff_data.dlc 			= frm->DLC;
	for (int  i = 0 ; i < local_rx_buff_data.dlc; i++)
	{
		local_rx_buff_data.data[i] = frm->Data[i];
	}

    xQueueSend(pipe_can_app_rx_handle,
    		   &local_rx_buff_data,
    		   (TickType_t)pdMS_TO_TICKS(1)
    		  );
    //printk("[ CO MGMT :CB] CAN receive!\n\r");
}

void COPdoTransmit(CO_IF_FRM *frm)
{
    (void)frm;

    /* Optional: place here some code, which is called
     * just before a PDO is transmitted. You may adjust
     * the given CAN frame which is send afterwards.
     */
    //printk("[ CO MGMT :CB] PDO Transmit\n\r");
}

int16_t COPdoReceive(CO_IF_FRM *frm)
{
    (void)frm;

    /* Optional: place here some code, which is called
     * right after receiving a PDO. You may adjust
     * the given CAN frame which is written into the
     * object dictionary afterwards or suppress the
     * write operation.
     */
    //printk("[ CO MGMT :CB] PDO Receive\n\r");
    return(0u);
}

void COPdoSyncUpdate(CO_RPDO *pdo)
{
    (void)pdo;

    /* Optional: place here some code, which is called
     * right after the object dictionary update due to
     * a synchronized PDO.
     */
    printk((uint8_t *)"[ CO MGMT :CB] Sync\n\r");
}

int16_t COParaDefault(struct CO_PARA_T *pg)
{
    (void)pg;

    /* Optional: place here some code, which is called
     * when a parameter group is restored to factory
     * settings.
     */
    printk((uint8_t *)"[ CO MGMT :CB] Parameter default\n\r");
    return (0u);
}

void CORpdoWriteData(CO_IF_FRM *frm, uint8_t pos, uint8_t size, CO_OBJ *obj)
{
    (void)frm;
    (void)pos;
    (void)size;
    (void)obj;

    /* Optional: place here some code, which is called
     * when a PDO is received with mapped values with
     * a size larger than 4 byte.
     */
    //printk("[ CO MGMT :CB] RPDO Write\n\r");
}

void COTpdoReadData(CO_IF_FRM *frm, uint8_t pos, uint8_t size, CO_OBJ *obj)
{
    (void)frm;
    (void)pos;
    (void)size;
    (void)obj;

    /* Optional: place here some code, which is called
     * when a PDO is constructed for transmission which
     * needs a mapped values with a size larger than 4 byte.
     */
   // printk("[ CO MGMT :CB] TPDO read\n\r");
}



