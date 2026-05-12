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
/* ITRC Software event 0 is mapped to signal name IN34 */
#define APP_ITRC_IN      kITRC_In34
#define APP_ITRC_IN_MASK ITRC_STATUS1_IN34_STATUS_MASK

/* ITRC IRQ is mapped to OUT0 */
#define IRQ_ITRC_OUT      kITRC_Out_0
#define IRQ_ITRC_OUT_MASK ITRC_STATUS_OUT0_STATUS_MASK

#define APP_ITRC_IRQN ITRC0_IRQn
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
