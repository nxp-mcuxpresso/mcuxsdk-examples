/*
 * Copyright 2025 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

 #ifndef _RTE_DEVICE_H
 #define _RTE_DEVICE_H
 
 extern void BOARD_InitDEBUG_UARTPins();
 extern void BOARD_InitDEBUG_UARTPins_deinit();
 
 /* Driver name mapping. */
 /* User needs to provide the implementation of LPUARTX_GetFreq/LPUARTX_InitPins/LPUARTX_DeinitPins for the enabled
  * LPUART instance. */
 #define RTE_USART5        1
 #define RTE_USART5_DMA_EN 1
 
 /* UART configuration. */
 #define RTE_USART5_PIN_INIT        BOARD_InitDEBUG_UARTPins
 #define RTE_USART5_PIN_DEINIT      BOARD_InitDEBUG_UARTPins_deinit
 #define RTE_USART5_DMA_TX_CH       16
 #define RTE_USART5_DMA_TX_PERI_SEL (uint16_t) kDmaRequestMux1LPUART5LPUART13Tx
 #define RTE_USART5_DMA_TX_DMAMUX_BASE DMAMUX_1
 #define RTE_USART5_DMA_TX_DMA_BASE EDMA
 #define RTE_USART5_DMA_RX_CH       17
 #define RTE_USART5_DMA_RX_PERI_SEL (uint16_t) kDmaRequestMux1LPUART5LPUART13Rx
 #define RTE_USART5_DMA_RX_DMAMUX_BASE DMAMUX_1
 #define RTE_USART5_DMA_RX_DMA_BASE EDMA
 
 #endif /* _RTE_DEVICE_H */
 