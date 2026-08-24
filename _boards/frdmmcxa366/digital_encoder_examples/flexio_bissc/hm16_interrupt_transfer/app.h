/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_ENCODER_PROTOCOL      "BiSS-C"
#define DEMO_ENCODER_BIT_RATE      10000000U
#define DEMO_ENCODER_MT_LEN        12U
#define DEMO_ENCODER_ST_LEN        16U
#define DEMO_ENCODER_ACK_LEN       3U
#define DEMO_BISSC_FLEXIO_BASE     (FLEXIO0)
#define DEMO_BISSC_CLOCK_FREQUENCY CLOCK_GetFlexioClkFreq()
#define DEMO_BISSC_FLEXIO_IRQ      FLEXIO_IRQn
#define DEMO_BISSC_SL_PIN          17U
#define DEMO_BISSC_MA_PIN          18U
#define DEMO_BISSC_SHIFTER_START   0U
#define DEMO_BISSC_TIMER_START     0U
#define DEMO_BISSC_USE_HW_TRIGGER  0U
#define DEMO_BISSC_USE_INTERRUPT   1U
#define DEMO_BISSC_WAIT_TIMEOUT    0x200000U
#define DEMO_BISSC_FRAME_INTERVAL_US 200000U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
