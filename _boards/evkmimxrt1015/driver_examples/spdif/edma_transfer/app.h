/*
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_SPDIF          SPDIF
#define EXAMPLE_DMA            DMA0
#define EXAMPLE_DMAMUX         DMAMUX
#define SPDIF_IRQ              SPDIF_IRQn
#define SPDIF_ErrorHandle      SPDIF_IRQHandler
#define SPDIF_RX_LEFT_CHANNEL  (0)
#define SPDIF_RX_RIGHT_CHANNEL (1)
#define SPDIF_TX_LEFT_CHANNEL  (2)
#define SPDIF_TX_RIGHT_CHANNEL (3)
#define SPDIF_RX_SOURCE        (85)
#define SPDIF_TX_SOURCE        (86)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
