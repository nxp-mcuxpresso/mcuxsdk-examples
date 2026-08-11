/*
 * Copyright 2022, 2024-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_MASTER                  HSP__I3C
#define I3C_MASTER_CLOCK_FREQUENCY      CLOCK_GetRootClockFreq(kCLOCK_Root_MAIN_i3c0_fclk)
#define EXAMPLE_USE_SETDASA_ASSIGN_ADDR 1

#define EXAMPLE_DMA                    MAIN__EDMA3
#define EXAMPLE_I3C_TX_DMA_CHANNEL     (0U)
#define EXAMPLE_I3C_RX_DMA_CHANNEL     (1U)
#define EXAMPLE_I3C_TX_DMA_CHANNEL_MUX (kDmaRequestMux1HspI3CToBus)
#define EXAMPLE_I3C_RX_DMA_CHANNEL_MUX (kDmaRequestMux1HspI3CFromBus)

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
