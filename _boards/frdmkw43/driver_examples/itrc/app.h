/*
 * Copyright 2026 NXP
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
/* ITRC Software event 0 is mapped to signal name IN35 */
#define APP_ITRC_IN      kITRC_In35
#define APP_ITRC_IN_MASK ITRC_STATUS1_IN35_STATUS_MASK

/* ITRC IRQ is mapped to OUT2 */
#define IRQ_ITRC_OUT      kITRC_Out_2
#define IRQ_ITRC_OUT_MASK ITRC_STATUS_OUT2_STATUS_MASK

#define APP_ITRC_IRQN ITRC_IRQn

#ifndef ITRC0
#define ITRC0 ITRC_0
#endif
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */