/*
 * Copyright 2022 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_MASTER                  I3C1
#define EXAMPLE_I2C_BAUDRATE            100000
#define EXAMPLE_I3C_OD_BAUDRATE         625000
#define EXAMPLE_I3C_PP_BAUDRATE         1250000
#define I3C_MASTER_CLOCK_FREQUENCY      CLOCK_GetI3cClkFreq(1U)
#define I3C_MASTER_SLAVE_ADDR_7BIT      0x1E
#define I3C_DATA_LENGTH                 33
#define EXAMPLE_USE_SETDASA_ASSIGN_ADDR 1

#define EXAMPLE_DMA                    DMA0
#define EXAMPLE_I3C_TX_DMA_CHANNEL     (0U)
#define EXAMPLE_I3C_RX_DMA_CHANNEL     (1U)
#define EXAMPLE_I3C_TX_DMA_CHANNEL_MUX (kDma0RequestMuxI3c1Tx)
#define EXAMPLE_I3C_RX_DMA_CHANNEL_MUX (kDma0RequestMuxI3c1Rx)

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
