/*
 * Copyright 2020,2021 NXP
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
#include "fsl_edma.h"
#include "fsl_dmamux.h"
/*${header:end}*/

extern AT_NONCACHEABLE_SECTION_ALIGN(uint16_t g_cadcSampleDataArray[DEMO_CADC_SAMPLE_COUNT], 2U);
/*${function:start}*/
SDK_ALIGN(uint16_t g_u16SrcAddr_channel, 2U);

edma_channel_transfer_config_t g_transferConfig0;
edma_channel_transfer_config_t g_transferConfig1;

/*!
 * @brief Initialize the DMAMUX.
 */
void DMAMUX_Configuration(void)
{
    DMAMUX_ConnectChannelToTriggerSource(DEMO_DMAMUX_BASEADDR, (dmamux_dma_channel_t)DEMO_DMA_CHANNEL_1,
                                         DEMO_DMA_ADC_SOURCE);
}

/*!
 * @brief Initialize the EDMA.
 *
 * This demo and edma settings intend to show workaround for the EDMA errata of MC56F81xxx with mark 0N91Z.
 *
 * EDMA errata of MC56F81xxx with mark 0N91Z:
 *   eDMA is not working as expected when the clock of the trigger source module is less than BUS_2X_CLK in fast mode.
 *   When the chip is configured in fast mode, where CPU operates at 2x bus frequency, and the peripheral that generate
 *   the DMA request operates at 1x bus frequency, the eDMA transfer may be executed multiple times with one trigger due
 *   to the signal used to clear the DMA request misaligns to bus clock.
 *
 *   Workaround:
 *     The workaround is to clear the eDMA request flag manually.
 *     1. Configure the source triggered eDMA channel to transfer the request flag clear message.
 *     2. Enable channel link in both minor loop and major loop by setting CITER_ELINKYES [ELINK] and CSR[MAJORELINK],
 *        and configure the linked channel number through CITER_ELINKYES [LINKCH] and CSR[MAJORLINKCH].
 *     3. Configure the linked eDMA channel to transfer the message that you want to move.
 *
 *   One example of workaround:
 *     ADC EOSI0 triggers eDMA to transfer ADC_RSLT0 and ADC_RSLT1 to memory and calculate the average after ten
 *     samples. Enable eDMA channel0 which triggered by ADC EOSI0 by configuring DMAMUX_CHCFG0, and configure eDMA
 * channel0 to clear the EOSI0 flag by transferring a variable contained constant 0x800 to ADC_STAT. Enable channel link
 * of eDMA channel0 to trigger eDMA channel1 after the completion of transfer. Configure eDMA channel1 to transfer
 * ADC_RSLT0 and ADC_RSLT1 to memory.
 *
 */
void EDMA_Configuration(void)
{
    edma_config_t sConfig;

    g_u16SrcAddr_channel = 0x800U;
    EDMA_GetDefaultConfig(&sConfig);
    EDMA_Init(DEMO_DMA_BASEADDR, &sConfig);

    EDMA_GetChannelDefaultTransferConfig(
        &g_transferConfig1, (uint32_t)&g_u16SrcAddr_channel, SDK_GET_REGISTER_BYTE_ADDR(ADC_Type, ADC, STAT),
        sizeof(uint16_t), sizeof(uint16_t), kEDMA_ChannelTransferWidth16Bits, kEDMA_ChannelTransferMemoryToPeripheral);

    /* minor loop link channel 0 */
    g_transferConfig1.bEnableChannelMajorLoopLink           = true;
    g_transferConfig1.bEnableChannelMinorLoopLink           = true;
    g_transferConfig1.eMajorLoopLinkChannel                 = DEMO_DMA_CHANNEL_0;
    g_transferConfig1.eMinorLoopLinkChannel                 = DEMO_DMA_CHANNEL_0;
    g_transferConfig1.bEnableChannelRequest                 = true;
    g_transferConfig1.i16SrcOffsetOfEachTransfer            = 0U;
    g_transferConfig1.i16DstOffsetOfEachTransfer            = 0U;
    g_transferConfig1.bDisableRequestAfterMajorLoopComplete = false;
    EDMA_SetChannelTransferConfig(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL_1, &g_transferConfig1);

    EDMA_GetChannelDefaultTransferConfig(&g_transferConfig0, CADC_RESULT_REG_BYTE_ADDR,
                                         (uint32_t)(uint8_t *)g_cadcSampleDataArray, sizeof(g_cadcSampleDataArray),
                                         sizeof(g_cadcSampleDataArray), kEDMA_ChannelTransferWidth16Bits,
                                         kEDMA_ChannelTransferPeripheralToMemory);
    g_transferConfig0.i16SrcOffsetOfEachTransfer            = 2U;
    g_transferConfig0.u16EnabledInterruptMask               = kEDMA_ChannelMajorLoopCompleteInterruptEnable;
    g_transferConfig0.bDisableRequestAfterMajorLoopComplete = false;
    g_transferConfig0.bEnableChannelRequest                 = true;
    g_transferConfig0.i32DstMajorLoopOffset                 = -32;
    g_transferConfig0.i32SrcMajorLoopOffset                 = -32;

    EDMA_SetChannelTransferConfig(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL_0, &g_transferConfig0);

    EnableIRQWithPriority(DMACH0_IRQn, 1);
}

#pragma interrupt alignsp saveall
void ivINT_DMACH0(void);
void ivINT_DMACH0(void)
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
