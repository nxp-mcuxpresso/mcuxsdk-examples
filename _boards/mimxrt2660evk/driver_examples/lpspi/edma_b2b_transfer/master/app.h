/*
 * Copyright 2020, 2026 NXP
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
/* Master related */
#define EXAMPLE_LPSPI_MASTER_BASEADDR       (HSP__LPSPI_1)
#define DEMO_LPSPI_TRANSMIT_EDMA_CHANNEL    kDmaRequestMux0HspLPSPI1Tx
#define DEMO_LPSPI_RECEIVE_EDMA_CHANNEL     kDmaRequestMux0HspLPSPI1Rx
#define EXAMPLE_LPSPI_MASTER_DMA_BASE       (MAIN__EDMA5)
#define EXAMPLE_LPSPI_MASTER_DMA_RX_CHANNEL 0U
#define EXAMPLE_LPSPI_MASTER_DMA_TX_CHANNEL 1U

#define EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT     (kLPSPI_Pcs0)
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER (kLPSPI_MasterPcs0)

#define BOARD_GetEDMAConfig(config)                                              \
    {                                                                            \
        static edma_channel_config_t channelConfig = {                           \
            .enableMasterIDReplication = true,                                   \
        };                                                                       \
        config.enableMasterIdReplication = true;                                 \
        config.channelConfig[0]          = &channelConfig;                       \
        config.channelConfig[1]          = &channelConfig;                       \
    }

#define LPSPI_MASTER_CLK_FREQ CLOCK_GetRootClockFreq(kCLOCK_Root_MAIN_lpspi1_fclk)

#define EXAMPLE_LPSPI_DEALY_COUNT 0xFFFFFU
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif
