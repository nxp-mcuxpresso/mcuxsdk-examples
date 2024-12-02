/*
 * Copyright 2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_MASTER                 I3C0
#define EXAMPLE_I2C_BAUDRATE           400000
#define EXAMPLE_I3C_OD_BAUDRATE        2000000
#define EXAMPLE_I3C_PP_BAUDRATE        4000000
#define I3C_MASTER_CLOCK_FREQUENCY     CLOCK_GetI3CFClkFreq()
#define I3C_MASTER_SLAVE_ADDR_7BIT     0x1E
#define WAIT_TIME                      1000
#define I3C_DATA_LENGTH                33
#define EXAMPLE_DMA                    DMA0
#define EXAMPLE_I3C_TX_DMA_CHANNEL     (0U)
#define EXAMPLE_I3C_RX_DMA_CHANNEL     (1U)
#define EXAMPLE_I3C_TX_DMA_CHANNEL_MUX (kDma0RequestMuxI3c0Tx)
#define EXAMPLE_I3C_RX_DMA_CHANNEL_MUX (kDma0RequestMuxI3c0Rx)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
