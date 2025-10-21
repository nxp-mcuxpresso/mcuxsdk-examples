/*
 * Copyright 2022-2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

#include "fsl_phyrtl8211f.h"
#include "fsl_pcal6524.h"
#include "fsl_rgpio.h"
#include "fsl_tpm.h"

#include "ethercattype.h"
#include "nicdrv.h"
#include "ethercatbase.h"
#include "ethercatmain.h"
#include "ethercatdc.h"
#include "ethercatcoe.h"
#include "ethercatfoe.h"
#include "ethercatconfig.h"
#include "ethercatprint.h"
#include "enet/soem_enet.h"
#include "enet/enet.h"
#include "soem_port.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
#define EXAMPLE_ENET_NUM    1U
#define EXAMPLE_NETC_HAS_NO_SWITCH 1U

#define EXAMPLE_ENET         ENET
#define EXAMPLE_PHY_ADDRESS  0x02U
#define ENET_CLOCK_ROOT      kCLOCK_Root_WakeupAxi
#define EXAMPLE_CLOCK_FREQ   CLOCK_GetIpFreq(ENET_CLOCK_ROOT)
#define EXAMPLE_PHY_INTERFACE_RGMII

#define CLOCK_GRANULARITY_NS 40UL
#define CLOCK_GRANULARITY_FRE (1000000000UL/CLOCK_GRANULARITY_NS)
#define CLOCK_INCREASE_PER_SEC 1000000000UL 
/*${macro:end}*/

#ifndef PHY_AUTONEGO_TIMEOUT_COUNT
#define PHY_AUTONEGO_TIMEOUT_COUNT (800000U)
#endif

#ifndef PHY_STABILITY_DELAY_US
#define PHY_STABILITY_DELAY_US (500000U)
#endif

#define SOEM_PORT_NAME "enet0"
#define ENET_RXBD_NUM          (4)
#define ENET_TXBD_NUM          (4)

#define ENET_RXBUFF_SIZE       (ENET_FRAME_MAX_FRAMELEN)
#define ENET_TXBUFF_SIZE       (ENET_FRAME_MAX_FRAMELEN)

/*! @brief GPT timer will be used to calculate the system time and delay */
#define BOARD_TPM            TPM2
#define DEMO_TIMER_PERIOD_US (1000000U)

/* Interrupt number and interrupt handler for the TPM instance used */
#define BOARD_TPM_IRQ_NUM TPM2_IRQn
#define BOARD_TPM_HANDLER TPM2_IRQHandler

/* Get source clock for TPM driver */
#define LPTPM_CLOCK_ROOT kCLOCK_Root_Tpm2
#define LPTPM_CLOCK_GATE kCLOCK_Tpm2
#define TPM_SOURCE_CLOCK CLOCK_GetIpFreq(LPTPM_CLOCK_ROOT)

/* Calculate the clock division based on the PWM frequency to be obtained */
#define TPM_PRESCALER TPM_CalculateCounterClkDiv(BOARD_TPM, 1000000U / DEMO_TIMER_PERIOD_US, TPM_SOURCE_CLOCK);
     
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
uint64_t gettime(void);
void nsleep_to (uint64_t nsec_target);
void osal_gettime(struct timeval *current_time);
void osal_timer_init(uint32_t priority);
int if_port_init(void);
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
