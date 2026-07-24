/*
 * Copyright 2023, 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_I2C_MASTER_BASE (HSP__LPI2C_1_BASE)

#define BOARD_GetEDMAConfig(config)                                              \
    {                                                                            \
        static edma_channel_config_t channelConfig = {                           \
            .enableMasterIDReplication = true,                                   \
        };                                                                       \
        config.enableMasterIdReplication = true;                                 \
        config.channelConfig[0]          = &channelConfig;                       \
        config.channelConfig[1]          = &channelConfig;                       \
    }

/* Get frequency of lpi2c clock */
#define LPI2C_CLOCK_FREQUENCY CLOCK_GetRootClockFreq(kCLOCK_Root_MAIN_lpi2c1_fclk)

#define LPI2C_MASTER_CLOCK_FREQUENCY LPI2C_CLOCK_FREQUENCY

#define EXAMPLE_LPI2C_MASTER_DMA (MAIN__EDMA3)

#define DEMO_LPI2C_TRANSMIT_EDMA_CHANNEL kDmaRequestMux1HspLPI2C1Tx
#define DEMO_LPI2C_RECEIVE_EDMA_CHANNEL  kDmaRequestMux1HspLPI2C1Rx

#define LPI2C_TRANSMIT_DMA_CHANNEL 0U
#define LPI2C_RECEIVE_DMA_CHANNEL  1U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
