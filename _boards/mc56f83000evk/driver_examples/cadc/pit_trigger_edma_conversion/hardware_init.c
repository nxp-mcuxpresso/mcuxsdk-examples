/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "fsl_device_registers.h"
#include "pin_mux.h"
#include "board.h"
#include "clock_config.h"
#include "peripherals.h"
#include "app.h"
#include "fsl_dmamux.h"
#include "fsl_edma.h"
/*${header:end}*/

extern AT_NONCACHEABLE_SECTION_ALIGN(uint16_t g_cadcSampleDataArray[DEMO_CADC_SAMPLE_COUNT], 2U);
/*${function:start}*/

edma_channel_transfer_config_t g_transferConfig;

void DMAMUX_Configuration(void)
{
    DMAMUX_ConnectChannelToTriggerSource(DEMO_DMAMUX_BASEADDR, (dmamux_dma_channel_t)DEMO_DMA_CHANNEL_0,
                                         DEMO_DMA_ADC_SOURCE);
}

void EDMA_Configuration(void)
{
    EDMA_GetChannelDefaultTransferConfig(&g_transferConfig, CADC_RESULT_REG_BYTE_ADDR,
                                         (uint32_t)(uint8_t *)g_cadcSampleDataArray, 2U, 2U,
                                         kEDMA_ChannelTransferWidth16Bits, kEDMA_ChannelTransferPeripheralToMemory);
    g_transferConfig.i16SrcOffsetOfEachTransfer            = 0U;
    g_transferConfig.i16DstOffsetOfEachTransfer            = 2U;
    g_transferConfig.u16EnabledInterruptMask               = kEDMA_ChannelMajorLoopCompleteInterruptEnable;
    g_transferConfig.bDisableRequestAfterMajorLoopComplete = false;
    g_transferConfig.bEnableChannelRequest                 = true;
    g_transferConfig.bEnableDstMinorLoopOffset             = true;
    g_transferConfig.i32DstMajorLoopOffset                 = -32;
    g_transferConfig.i32SrcMajorLoopOffset                 = 0U;
    g_transferConfig.u16MinorLoopCountsEachMajorLoop       = 16U;

    EDMA_SetChannelTransferConfig(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL_0, &g_transferConfig);

    EnableIRQWithPriority(DMA0_IRQn, 1);
}

#pragma interrupt alignsp saveall
void ivINT_DMA0(void);
void ivINT_DMA0(void)
{
    DMA_Channel0_IRQHandler();
}
#pragma interrupt off

void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();

    BOARD_InitDebugConsole();

    EnableIRQWithPriority(BOARD_DEBUG_UART_RX_IRQ, BOARD_DEBUG_UART_ISR_PRORITY);
    EnableIRQWithPriority(BOARD_DEBUG_UART_TX_IRQ, BOARD_DEBUG_UART_ISR_PRORITY);
    EnableIRQWithPriority(BOARD_DEBUG_UART_RX_ERR_IRQ, BOARD_DEBUG_UART_ISR_PRORITY);
    EnableIRQWithPriority(BOARD_DEBUG_UART_TX_IDLE_IRQ, BOARD_DEBUG_UART_ISR_PRORITY);
    SetIRQBasePriority(0);
}
/*${function:end}*/
