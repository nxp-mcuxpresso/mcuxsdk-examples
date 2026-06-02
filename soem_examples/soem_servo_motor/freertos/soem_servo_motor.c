/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdlib.h>
#include <stdio.h>

#include "cia402.h"
#include "servo.h"

#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define NUM_1M (1000000UL)
#define CYCLE_PERIOD_NS 1000000 // 1ms
#define SOEM_PERIOD 	(CYCLE_PERIOD_NS / 1000) // 1ms

#define asda_b3_VendorId 0x000001dd
#define asda_b3_ProductID 0x00006080

#define sv680_VendorId 0x00100000
#define sv680_ProductID 0x000c0116

#define nxp_VendorId 0x00000CC2
#define nxp_ProductID 0x00000002

#define MAX_SERVO 2
#define MAX_AXIS 2

#define RT_TASK_STACK_SIZE 1024
/*******************************************************************************
 * Variables
 ******************************************************************************/
/*! @brief Buffer descriptors should be in non-cacheable region and should be align to "ENET_BUFF_ALIGNMENT". */
AT_NONCACHEABLE_SECTION_ALIGN(static enet_rx_bd_struct_t g_rxBuffDescrip[ENET_RXBD_NUM], ENET_BUFF_ALIGNMENT);
AT_NONCACHEABLE_SECTION_ALIGN(static enet_tx_bd_struct_t g_txBuffDescrip[ENET_TXBD_NUM], ENET_BUFF_ALIGNMENT);

/*! @brief The data buffers can be in cacheable region or in non-cacheable region.
 * If use cacheable region, the alignment size should be the maximum size of "CACHE LINE SIZE" and "ENET_BUFF_ALIGNMENT"
 * If use non-cache region, the alignment size is the "ENET_BUFF_ALIGNMENT".
 */
AT_NONCACHEABLE_SECTION_ALIGN(
    static uint8_t g_rxDataBuff[ENET_RXBD_NUM][SDK_SIZEALIGN(ENET_RXBUFF_SIZE, ENET_BUFF_ALIGNMENT)],
    ENET_BUFF_ALIGNMENT);
AT_NONCACHEABLE_SECTION_ALIGN(
    static uint8_t g_txDataBuff[ENET_TXBD_NUM][SDK_SIZEALIGN(ENET_TXBUFF_SIZE, ENET_BUFF_ALIGNMENT)],
    ENET_BUFF_ALIGNMENT);

static enet_buffer_config_t buffConfig[] = {{
    ENET_RXBD_NUM,
    ENET_TXBD_NUM,
    SDK_SIZEALIGN(ENET_RXBUFF_SIZE, ENET_BUFF_ALIGNMENT),
    SDK_SIZEALIGN(ENET_TXBUFF_SIZE, ENET_BUFF_ALIGNMENT),
    &g_rxBuffDescrip[0],
    &g_txBuffDescrip[0],
    &g_rxDataBuff[0][0],
    &g_txDataBuff[0][0],
    true,
    true,
    NULL,
}};

static char IOmap[1500];

static char *tp[MAX_SERVO] = {

// DELTA
"Cyclic=1; Scale=46603; Bias=0; Accel=8; Decel=8; Max_speed=3600; TpArrays=[(0:2000),(270:1000),(270:2000),(180:1000),(180:2000),(0:1000),(0:2000),(0:1000)];",

// INOVANCE
"Cyclic=1; Scale=186414; Bias=0; Accel=8; Decel=8; Max_speed=3600; TpArrays=[(0:2000),(270:1000),(270:2000),(180:1000),(180:2000),(0:1000),(0:2000),(0:1000)];",

};

static StackType_t IdleTaskStack[configMINIMAL_STACK_SIZE];
static StaticTask_t IdleTaskTCB;

static StackType_t TimerTaskStacj[configMINIMAL_STACK_SIZE];
static StaticTask_t TimerTaskTCB;

static TaskHandle_t rt_task = NULL;
static StaticTask_t xTaskBuffer;
static StackType_t rt_task_stack[RT_TASK_STACK_SIZE];

static struct enet_if_port if_port;

static uint32_t timer_irq_period = 0; /* unit: microsecond*/

static struct timeval system_time_base = {
	.tv_sec = 0,
	.tv_usec =  0
};

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   configSTACK_DEPTH_TYPE *pulIdleTaskStackSize)
{
    *ppxIdleTaskTCBBuffer   = &IdleTaskTCB;
    *ppxIdleTaskStackBuffer = &IdleTaskStack[0];
    *pulIdleTaskStackSize   = configMINIMAL_STACK_SIZE;
}

void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize)
{
    *ppxTimerTaskTCBBuffer   = &TimerTaskTCB;
    *ppxTimerTaskStackBuffer = &TimerTaskStacj[0];
    *pulTimerTaskStackSize   = configMINIMAL_STACK_SIZE;
}

struct servo_t servo[MAX_SERVO];
struct axis_t axis[MAX_AXIS];

static void EtherCAT_servo_init(struct servo_t *svo, struct axis_t *ax)
{
	int i;
	memset(svo, 0, sizeof(struct servo_t) * MAX_SERVO);
	memset(ax, 0, sizeof(struct axis_t) * MAX_AXIS);
	for (i = 0; i < MAX_SERVO; i++) {
		svo[i].slave_id = i;
		svo[i].axis_num = 1;
	}
	
	for (i = 0; i < 1; i++) {
		svo[i].VendorId = asda_b3_VendorId;
		svo[i].ProductID = asda_b3_ProductID;
	}

    for (i = 1; i < 2; i++) {
		svo[i].VendorId = sv680_VendorId;
		svo[i].ProductID = sv680_ProductID;
	}

	for (i = 0; i < MAX_SERVO; i++) {
		ax[i].servo = svo + i;
		ax[i].axis_offset = 0;
	}
}

static int general_servo_setup(uint16 slave) {
	int i;
	int ret = 0;
	int chk = 10;
	struct servo_t *svo = NULL;
	for (i = 0; i < MAX_SERVO; i++) {
		if (servo[i].slave_id + 1 == slave) {
			svo = &servo[i];
			break;
		}
	}
	if (svo) {
		while (chk--) {
			ret = servo_pdo_remap(svo);
			if (ret)
				break;
		}
	}

	if (!ret)
		return 0;
	else
		return -1;
}

static int nxp_servo_setup(uint16 slave) {
	int i;
	int ret = 0;
	int chk = 10;
	struct servo_t *svo = NULL;
	for (i = 0; i < MAX_SERVO; i++) {
		if (servo[i].slave_id + 1 == slave) {
			svo = &servo[i];
			break;
		}
	}
	if (svo) {
		while (chk--) {
			ret = servo_pdo_activate_map(svo);
			if (ret)
				break;
		}
	}

	if (!ret)
		return 0;
	else
		return -1;
}

static int asda_b3_servo_setup(uint16 slave) {
	int i;
	int ret = 0;
	int chk = 10;
	struct servo_t *svo = NULL;
	for (i = 0; i < MAX_SERVO; i++) {
		if (servo[i].slave_id + 1 == slave) {
			svo = &servo[i];
			break;
		}
	}
	if (svo) {
		while (chk--) {
			ret = servo_pdo_remap(svo);
			if (ret)
				break;
		}
	}

	if (!ret)
		return 0;

	int8_t  Obj60c2[9][2] = {{12, -5},{25, -5}, {37, -5}, {5, -4},{62, -5}, {75, -5},{87, -5},{1, -3}, {2, -3}};
	int8_t num_8b[2];
	int wkc = 0;
	if (CYCLE_PERIOD_NS > 1000000) {
		num_8b[0] = CYCLE_PERIOD_NS / 1000000;
		num_8b[1] = -3;
	} else {
		int index = ((CYCLE_PERIOD_NS - 1) / 125000);
		num_8b[0] = Obj60c2[index][0];
		num_8b[1] = Obj60c2[index][1];
	}
	int obj_60c2_index = 0x60c2;
	for ( i = 0; i < svo->axis_num; i++) {
		obj_60c2_index += i * 0x800;
		wkc += ec_SDOwrite(slave, obj_60c2_index, 0x01, 0, 1, &num_8b[0], EC_TIMEOUTSAFE);
		wkc += ec_SDOwrite(slave, obj_60c2_index, 0x02, 0, 1, &num_8b[1], EC_TIMEOUTSAFE);
	}
	return wkc == svo->axis_num * 2 ? 1 : 0;
}

static void servo_setup(struct servo_t *servo, int servo_num) {
	int i;
	for (i = 0; i < servo_num; i++) {
		if (servo[i].VendorId == asda_b3_VendorId && servo[i].ProductID == asda_b3_ProductID) {
			servo[i].slave->PO2SOconfig = asda_b3_servo_setup;
			PRINTF("\r\n delta_servo_setup success!\r\n");
		} else if (servo[i].VendorId == nxp_VendorId && servo[i].ProductID == nxp_ProductID) {
			servo[i].slave->PO2SOconfig = nxp_servo_setup;
			PRINTF("\r\n nxp_servo_setup success!\r\n");
		} else {
			servo[i].slave->PO2SOconfig = general_servo_setup;
		}

	}
}

void irq_wake_task(void)
{
    BaseType_t xHigherPriorityTaskWoken;

    if (rt_task)
    {
        xHigherPriorityTaskWoken = pdFALSE;
        vTaskNotifyGiveFromISR(rt_task, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

void OSAL_TIMER_IRQHandler(void)
{
    /* Clear interrupt flag. */
    GPT_ClearStatusFlags(OSAL_TIMER, kGPT_OutputCompare1Flag);

    system_time_base.tv_usec += SOEM_PERIOD;

    if (system_time_base.tv_usec >= NUM_1M)
    {
        system_time_base.tv_sec += system_time_base.tv_usec / NUM_1M;
        system_time_base.tv_usec = system_time_base.tv_usec % NUM_1M;
    }

    irq_wake_task();

    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F, Cortex-M7, Cortex-M7F
     * Store immediate overlapping
     * exception return operation might vector to incorrect interrupt
     */
    SDK_ISR_EXIT_BARRIER;
}

static void osal_timer_init(uint32_t usec, uint32_t priority)
{
    uint32_t gptFreq;
    gpt_config_t gptConfig;

    assert(usec != 0);
    assert(priority < (1UL << __NVIC_PRIO_BITS));

    timer_irq_period = usec;
    GPT_GetDefaultConfig(&gptConfig);
    GPT_Init(OSAL_TIMER, &gptConfig);
    gptFreq = OSAL_TIMER_CLK_FREQ;
    /* Divide GPT clock source frequency to 1MHz */
    GPT_SetClockDivider(OSAL_TIMER, gptFreq / NUM_1M);
    /* Set both GPT modules to 1 second duration */
    GPT_SetOutputCompareValue(OSAL_TIMER, kGPT_OutputCompare_Channel1, timer_irq_period);
    /* Enable GPT Output Compare1 interrupt */
    GPT_EnableInterrupts(OSAL_TIMER, kGPT_OutputCompare1InterruptEnable);
    /* Enable at the Interrupt */
    NVIC_SetPriority(OSAL_TIMER_IRQ_ID, priority);
    EnableIRQ(OSAL_TIMER_IRQ_ID);
    GPT_StartTimer(OSAL_TIMER);
}

/* OSHW: register enet port to SOEM stack */
static int if_port_init(void)
{
    struct soem_if_port soem_port;
    memset(&if_port, 0, sizeof(if_port));
    if_port.bufferConfig = buffConfig;
    if_port.base         = EXAMPLE_ENET;
    /* The miiMode should be set according to the different PHY interfaces. */
#ifdef EXAMPLE_PHY_INTERFACE_RGMII
    if_port.mii_mode                   = kENET_RgmiiMode;
#else
    if_port.mii_mode                   = kENET_RmiiMode;
#endif
    if_port.phy_config.autoNeg         = true;
    if_port.phy_config.phyAddr         = EXAMPLE_PHY_ADDRESS;
    if_port.phy_config.resource        = EXAMPLE_PHY_RESOURCE;
    if_port.phy_config.ops             = EXAMPLE_PHY_OPS;
    if_port.srcClock_Hz                = EXAMPLE_CLOCK_FREQ;
    if_port.phy_autonego_timeout_count = PHY_AUTONEGO_TIMEOUT_COUNT;
    if_port.phy_stability_delay_us     = PHY_STABILITY_DELAY_US;
    
    soem_port.port_init = enet_init;
    soem_port.port_send = enet_send;
    soem_port.port_recv = enet_recv;
    soem_port.port_link_status = enet_link_status;
    soem_port.port_close = enet_close;
    strncpy(soem_port.ifname, SOEM_PORT_NAME, SOEM_IF_NAME_MAXLEN);
    strncpy(soem_port.dev_name, "enet", SOEM_DEV_NAME_MAXLEN);
    soem_port.port_pri = &if_port;
    return register_soem_port(&soem_port);
}

void control_task(void *ifname)
{
	int expectedWKC;
	volatile int wkc;
	int chk, i;
    int wkc_lost = 0;
	PRINTF("Starting motion task\r\n");
	const TickType_t xBlockTime = pdMS_TO_TICKS(500);
	EtherCAT_servo_init(servo, axis);

	/* initialise SOEM, and if_port */
	if (ec_init(ifname)) {
		PRINTF("ec_init on %s succeeded.\n",ifname);
		/* find and auto-config slaves */
		if ( ec_config_init(FALSE) > 0 ) {
			PRINTF("%d slaves found and configured.\n",ec_slavecount);
			if (ec_slavecount < MAX_SERVO) {
				PRINTF("The number of Servo scanned is not consistent with configed, please reconfirm\n");
				return;
			}

			i = servo_slave_check(servo, MAX_SERVO);
			if (i < 0) {
				PRINTF("The infomation of Servo:%d is not consistent with scanned, please reconfirm\n", -i);
				return;
			}

			ec_configdc();
			chk = 100;
			while (chk--) {
				if (servo_synced_check(servo, MAX_SERVO) == 1) {
					break;
				}
			}

			for (i = 0; i < MAX_SERVO; i++) {
				if(servo[i].slave->hasdc > 0) {
				ec_dcsync0(servo[i].slave_id + 1, TRUE, CYCLE_PERIOD_NS, CYCLE_PERIOD_NS * 3);
				}
			}

			servo_setup(servo, MAX_SERVO);	
			ec_config_map(&IOmap);
			for (i = 0; i < MAX_AXIS; i++) {
				axis_nc_init(&axis[i], tp[i], CYCLE_PERIOD_NS);
			}
			PRINTF("Slaves mapped, state to SAFE_OP.\n");
			/* wait for all slaves to reach SAFE_OP state */
			ec_statecheck(0, EC_STATE_SAFE_OP,  EC_TIMEOUTSTATE * 4);
			for (i = 0; i < MAX_AXIS; i++) {
				PDO_write_targe_position(&axis[i], axis[i].current_position);
				axis_nc_start(&axis[i]);
			}
			PRINTF("segments : %d : %lu %lu %lu %lu\n",ec_group[0].nsegments ,ec_group[0].IOsegment[0],ec_group[0].IOsegment[1],ec_group[0].IOsegment[2],ec_group[0].IOsegment[3]);

			PRINTF("Request operational state for all slaves\n");
			expectedWKC = (ec_group[0].outputsWKC * 2) + ec_group[0].inputsWKC;
			PRINTF("Calculated workcounter %d\n", expectedWKC);
			ec_slave[0].state = EC_STATE_OPERATIONAL;
			/* send one valid process data to make outputs in slaves happy*/
			ec_send_processdata();
			ec_receive_processdata(EC_TIMEOUTRET);
			/* request OP state for all slaves */
			ec_writestate(0);
			chk = 500;
			/* wait for all slaves to reach OP state */
			do {
				ec_send_processdata();
				ec_receive_processdata(EC_TIMEOUTRET);
				ec_statecheck(1, EC_STATE_OPERATIONAL, 50000);
			} while (chk-- && (ec_slave[0].state != EC_STATE_OPERATIONAL));

			for (i = 0; i < MAX_AXIS; i++) {
				PDO_write_targe_position(&axis[i], axis[i].current_position);
			}

			PRINTF("Request operational state for all slaves\r\n");
			expectedWKC = (ec_group[0].outputsWKC * 2) + ec_group[0].inputsWKC;
			PRINTF("Calculated workcounter %d\r\n", expectedWKC);
			ec_slave[0].state = EC_STATE_OPERATIONAL;
			/* send one valid process data to make outputs in slaves happy*/
			ec_send_processdata();
			ec_receive_processdata(EC_TIMEOUTRET);
			/* request OP state for all slaves */
			ec_writestate(0);
			chk = 500;
			/* wait for all slaves to reach OP state */
			do {
				ec_send_processdata();
				ec_receive_processdata(EC_TIMEOUTRET);
				ec_statecheck(1, EC_STATE_OPERATIONAL, 50000);
			} while (chk-- && (ec_slave[0].state != EC_STATE_OPERATIONAL));

			
			if (ec_slave[0].state != EC_STATE_OPERATIONAL) {
				PRINTF("Not all slaves reached operational state.\r\n");
			} else {
				PRINTF("Operational state reached for all slaves.\r\n");
				/* send one valid process data to make outputs in slaves happy*/
				ec_send_processdata();
			
				int op_num = 0;
				while (1) {
					/* SOEM receive data */
					wkc = ec_receive_processdata(EC_TIMEOUTRET);
					
					/* servo motor application processing code */
					for(i = 0; i < MAX_AXIS; i++) {
						if (axis_start(&axis[i], op_mode_csp) != 1) {
								op_num++;
						}
					}
					if (op_num == 0) {
						if(wkc >= expectedWKC) {
							for(i = 0; i < MAX_AXIS; i++) {
								axis[i].current_velocity = PDO_read_actual_velocity(&axis[i]);
								axis[i].current_position = PDO_read_actual_position(&axis[i]);
								if (axis[i].axis_status.csp_status == csp_status_running || axis[i].axis_status.csp_status == csp_status_pre_stop) {
									int pos = axis_nc_get_next_pos(&axis[i]);
									PDO_write_targe_position(&axis[i], pos);
								}
							} 
							if (axis[MAX_AXIS-1].axis_status.csp_status == csp_status_stop) {
								break;
							}
						} else {
							wkc_lost++;
							ec_slave[0].state = EC_STATE_OPERATIONAL;
							ec_writestate(0);
						}
					} else {
						op_num = 0;
					}

					/* SOEM tramsmit data */
					ec_send_processdata();
					ulTaskNotifyTake(pdFALSE, xBlockTime);
				}
				PRINTF("wkc_lost = %d\r\n", wkc_lost);
				PRINTF("\r\nRequest init state for all slaves\r\n");
				for(i = 1; i<=ec_slavecount ; i++) {
					if(ec_slave[i].state != EC_STATE_OPERATIONAL) {
						PRINTF("Slave %d State=0x%2.2x StatusCode=0x%4.4x : %s\r\n",
						i, ec_slave[i].state, ec_slave[i].ALstatuscode, ec_ALstatuscode2string(ec_slave[i].ALstatuscode));
					}
				}

				ec_slave[0].state = EC_STATE_INIT;
				/* request INIT state for all slaves */
				ec_writestate(0);
			}
			/* stop SOEM, close socket */
			ec_close();
		} else {
			PRINTF("No socket connection on %s\nExecute as root\r\n",ifname);
		}
	} else {
		PRINTF("ec_init error\r\n");
	}
}

/*!
 * @brief Main function
 */
int main(void)
{
    BOARD_InitHardware();

    PRINTF("Start the soem_servo_rt1180 FreeRTOS example...\r\n");

    osal_timer_init(SOEM_PERIOD, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
    if_port_init();
    rt_task = xTaskCreateStatic(/* The function that implements the task. */
                                control_task, "RT_task", RT_TASK_STACK_SIZE, SOEM_PORT_NAME, configMAX_PRIORITIES - 1,
                                rt_task_stack, &xTaskBuffer);

    vTaskStartScheduler();
    return 0;
}

