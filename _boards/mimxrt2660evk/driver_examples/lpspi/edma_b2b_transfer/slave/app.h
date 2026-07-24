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
/* Slave related */
#define EXAMPLE_LPSPI_SLAVE_BASEADDR       (HSP__LPSPI_1)
#define DEMO_LPSPI_TRANSMIT_EDMA_CHANNEL   kDmaRequestMux0HspLPSPI1Tx
#define DEMO_LPSPI_RECEIVE_EDMA_CHANNEL    kDmaRequestMux0HspLPSPI1Rx
#define EXAMPLE_LPSPI_SLAVE_DMA_BASE       (MAIN__EDMA5)
#define EXAMPLE_LPSPI_SLAVE_DMA_RX_CHANNEL 0U
#define EXAMPLE_LPSPI_SLAVE_DMA_TX_CHANNEL 1U

#define BOARD_GetEDMAConfig(config)                                              \
    {                                                                            \
        static edma_channel_config_t channelConfig = {                           \
            .enableMasterIDReplication = true,                                   \
        };                                                                       \
        config.enableMasterIdReplication = true;                                 \
        config.channelConfig[0]          = &channelConfig;                       \
        config.channelConfig[1]          = &channelConfig;                       \
    }

#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_INIT     (kLPSPI_Pcs0)
#define EXAMPLE_LPSPI_SLAVE_PCS_FOR_TRANSFER (kLPSPI_SlavePcs0)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif
