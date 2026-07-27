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
#define BOARD_FLEXIO_BASE      FLEXIO0

#define FLEXIO_T_FORMAT_RX_PIN    18U
#define FLEXIO_T_FORMAT_DR_PIN    16U
#define FLEXIO_T_FORMAT_TX_PIN    17U
#define T_FORMAT_TX_SHIFTER_INDEX 0U
#define T_FORMAT_RX_SHIFTER_INDEX 1U
#define T_FORMAT_DR_TIMER_INDEX   0U
#define T_FORMAT_TX_TIMER_INDEX   1U
#define T_FORMAT_RX_TIMER_INDEX   2U
#define FLEXIO_CLOCK_FREQUENCY    CLOCK_GetFlexioClkFreq()
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
