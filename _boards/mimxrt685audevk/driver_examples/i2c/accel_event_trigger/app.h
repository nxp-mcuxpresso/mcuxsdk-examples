/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_INT_IRQn        (GPIO_INTA_IRQn)
#define EXAMPLE_SENSOR_INT_GPIO GPIO
#define EXAMPLE_SENSOR_INT_PORT 2
#define EXAMPLE_SENSOR_INT_PIN  15
#define EXAMPLE_PinIntHandler   GPIO_INTA_DriverIRQHandler

/*!< Power down all unnecessary blocks and enable RBB during deep sleep*/
#define EXAMPLE_DEEPSLEEP_RUNCFG0 (SYSCTL0_PDSLEEPCFG0_RBB_PD_MASK)
#define EXAMPLE_DEEPSLEEP_RAM_APD 0xFFFFF8U
#define EXAMPLE_DEEPSLEEP_RAM_PPD 0x0U
#define EXAMPLE_EXCLUDE_FROM_DEEPSLEEP                                                                            \
    (((const uint32_t[]){EXAMPLE_DEEPSLEEP_RUNCFG0,                                                               \
                         (SYSCTL0_PDSLEEPCFG1_FLEXSPI_SRAM_APD_MASK | SYSCTL0_PDSLEEPCFG1_FLEXSPI_SRAM_PPD_MASK), \
                         EXAMPLE_DEEPSLEEP_RAM_APD, EXAMPLE_DEEPSLEEP_RAM_PPD}))
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void EXAMPLE_InitWakeupPin(void);
void EXAMPLE_EnterDeepSleep(void);
void BOARD_InitHardware(void);
void APP_ACCEL_IntHandler(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
