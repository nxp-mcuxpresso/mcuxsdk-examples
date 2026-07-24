/*
 * Copyright 2024, 2026 NXP
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
#define BOARD_FLEXIO_BASE      HSP__FLEXIO_1
#define FLEXIO_CLOCK_FREQUENCY (CLOCK_GetRootClockFreq(kCLOCK_Root_MAIN_flexio1_fclk))
#define EXAMPLE_TX_DMA_SOURCE  kDmaRequestMux1HspFlexIO1Request0Request8
#define EXAMPLE_RX_DMA_SOURCE  kDmaRequestMux1HspFlexIO1Request2Request10

#if defined(RT2660_PRESILICON_DEVELOPMENT) && (RT2660_PRESILICON_DEVELOPMENT == 1)
#define FLEXIO_UART_TX_PIN 0U
#define FLEXIO_UART_RX_PIN 1U
#else
#define FLEXIO_UART_TX_PIN 20U
#define FLEXIO_UART_RX_PIN 21U
#endif

#define EXAMPLE_FLEXIO_UART_DMA_BASEADDR MAIN__EDMA3
#define FLEXIO_UART_TX_DMA_CHANNEL       0U
#define FLEXIO_UART_RX_DMA_CHANNEL       1U
#define FLEXIO_TX_SHIFTER_INDEX          0U
#define FLEXIO_RX_SHIFTER_INDEX          2U

#define BOARD_GetEDMAConfig(config)                        \
    {                                                      \
        static edma_channel_config_t channelConfig = {     \
            .enableMasterIDReplication = true,             \
        };                                                 \
        config.enableMasterIdReplication = true;           \
        config.channelConfig[0]          = &channelConfig; \
        config.channelConfig[1]          = &channelConfig; \
    }
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
