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
#define BOARD_FLEXIO_BASE         FLEXIO0
#define FLEXIO_A_FORMAT_DR_PIN    16U /* PORT2_8  / FLEXIO0_D16 */
#define FLEXIO_A_FORMAT_TX_PIN    17U /* PORT2_9  / FLEXIO0_D17 */
#define FLEXIO_A_FORMAT_RX_PIN    18U /* PORT2_10 / FLEXIO0_D18 */
#define A_FORMAT_TX_SHIFTER_INDEX 0U
#define A_FORMAT_RX_SHIFTER_INDEX 1U
#define A_FORMAT_DR_TIMER_INDEX   0U
#define A_FORMAT_TX_TIMER_INDEX   1U
#define A_FORMAT_RX_TIMER_INDEX   2U
#define FLEXIO_CLOCK_FREQUENCY    CLOCK_GetFlexioClkFreq()

/* A-format encoder address is 3 bits (0..7). Matches the working FRDM-IMXRT1186
 * demo (ENC_ADDR=3); the 1-to-1 SetAddress command below forces the single
 * encoder on the bus to this address before any read. */
#define DEMO_ENCODER_ADDRESS     3U
#define DEMO_ENCODER_ST_BITS     20U
#define DEMO_ENCODER_MT_BITS     16U
#define DEMO_ENCODER_ST_MASK     0x000FFFFFU
#define DEMO_ENCODER_MT_MASK     0x0000FFFFU
#define DEMO_POLL_PERIOD_US      100000U
#define DEMO_TEMPERATURE_PERIOD  10U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
