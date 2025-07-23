/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_EP_BD_ALIGN          128U
#define EXAMPLE_TX_INTR_MSG_DATA     1U
#define EXAMPLE_RX_INTR_MSG_DATA     2U
#define EXAMPLE_EP_TX_INTR_MSG_DATA  3U
#define EXAMPLE_TX_MSIX_ENTRY_IDX    0U
#define EXAMPLE_RX_MSIX_ENTRY_IDX    1U
#define EXAMPLE_EP_TX_MSIX_ENTRY_IDX 2U
#define EXAMPLE_FRAME_FID            1U

/* ENETC pseudo port for management */
#ifndef EXAMPLE_SWT_SI
#define EXAMPLE_SWT_SI kNETC_ENETC1PSI0
#endif
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* Rx buffer memeory type. */
typedef uint8_t rx_buffer_t[EXAMPLE_EP_RXBUFF_SIZE_ALIGN];

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* EP resource. */
static ep_handle_t g_ep_handle;

/* SWT resource. */
static swt_handle_t g_swt_handle;
static swt_config_t g_swt_config;

/* Buffer descriptor resource. */
AT_NONCACHEABLE_SECTION_ALIGN(static netc_rx_bd_t g_rxBuffDescrip[EXAMPLE_EP_RING_NUM][EXAMPLE_EP_RXBD_NUM],
                              EXAMPLE_EP_BD_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static netc_tx_bd_t g_txBuffDescrip[EXAMPLE_EP_RING_NUM][EXAMPLE_EP_TXBD_NUM],
                              EXAMPLE_EP_BD_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static rx_buffer_t g_rxDataBuff[EXAMPLE_EP_RING_NUM][EXAMPLE_EP_RXBD_NUM],
                              EXAMPLE_EP_BUFF_SIZE_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static netc_cmd_bd_t g_cmdBuffDescrip[EXAMPLE_EP_TXBD_NUM], EXAMPLE_EP_BD_ALIGN);

static uint64_t rxBuffAddrArray[EXAMPLE_EP_RING_NUM][EXAMPLE_EP_RXBD_NUM];
static netc_tx_frame_info_t g_txDirty[EXAMPLE_EP_RING_NUM][EXAMPLE_EP_TXBD_NUM];
static netc_tx_frame_info_t txFrameInfo;
static volatile bool txOver;

static uint8_t g_macAddr[6] = {0x54, 0x27, 0x8d, 0x00, 0x00, 0x00};

/*******************************************************************************
 * Code
 ******************************************************************************/

static status_t APP_ReclaimCallback(ep_handle_t *handle, uint8_t ring, netc_tx_frame_info_t *frameInfo, void *userData)
{
    txFrameInfo = *frameInfo;
    (void)txFrameInfo;
    return kStatus_Success;
}

void msgintrCallback(MSGINTR_Type *base, uint8_t channel, uint32_t pendingIntr)
{
    /* Transmit interrupt */
    if ((pendingIntr & (1U << EXAMPLE_TX_INTR_MSG_DATA)) != 0U)
    {
        EP_CleanTxIntrFlags(&g_ep_handle, 1, 0);
        txOver = true;
    }
    /* Receive interrupt */
    if ((pendingIntr & (1U << EXAMPLE_RX_INTR_MSG_DATA)) != 0U)
    {
        EP_CleanRxIntrFlags(&g_ep_handle, 1);
    }
    /* EP Transmit interrupt */
    if ((pendingIntr & (1U << EXAMPLE_EP_TX_INTR_MSG_DATA)) != 0U)
    {
        EP_CleanTxIntrFlags(&g_ep_handle, 2, 0);
        txOver = true;
    }
}

status_t APP_SWT_MacLearning_Forwarding(void)
{
    status_t result                  = kStatus_Success;
    netc_rx_bdr_config_t rxBdrConfig = {0};
    netc_tx_bdr_config_t txBdrConfig = {0};
    netc_bdr_config_t bdrConfig      = {.rxBdrConfig = &rxBdrConfig, .txBdrConfig = &txBdrConfig};
    netc_msix_entry_t msixEntry[3];
    ep_config_t g_ep_config;
    uint32_t msgAddr;
    uint32_t i;

    for (uint8_t ring = 0U; ring < EXAMPLE_EP_RING_NUM; ring++)
    {
        for (uint8_t index = 0U; index < EXAMPLE_EP_RXBD_NUM; index++)
        {
            rxBuffAddrArray[ring][index] = (uint64_t)(uintptr_t)&g_rxDataBuff[ring][index];
        }
    }

    /* MSIX and interrupt configuration. */
    MSGINTR_Init(EXAMPLE_MSGINTR, &msgintrCallback);
    msgAddr              = MSGINTR_GetIntrSelectAddr(EXAMPLE_MSGINTR, 0);
    msixEntry[0].control = kNETC_MsixIntrMaskBit;
    msixEntry[0].msgAddr = msgAddr;
    msixEntry[0].msgData = EXAMPLE_TX_INTR_MSG_DATA;
    msixEntry[1].control = kNETC_MsixIntrMaskBit;
    msixEntry[1].msgAddr = msgAddr;
    msixEntry[1].msgData = EXAMPLE_RX_INTR_MSG_DATA;
    msixEntry[2].control = kNETC_MsixIntrMaskBit;
    msixEntry[2].msgAddr = msgAddr;
    msixEntry[2].msgData = EXAMPLE_EP_TX_INTR_MSG_DATA;

    bdrConfig.rxBdrConfig[0].bdArray       = &g_rxBuffDescrip[0][0];
    bdrConfig.rxBdrConfig[0].len           = EXAMPLE_EP_RXBD_NUM;
    bdrConfig.rxBdrConfig[0].extendDescEn  = false;
    bdrConfig.rxBdrConfig[0].buffAddrArray = &rxBuffAddrArray[0][0];
    bdrConfig.rxBdrConfig[0].buffSize      = EXAMPLE_EP_RXBUFF_SIZE_ALIGN;
    bdrConfig.rxBdrConfig[0].msixEntryIdx  = EXAMPLE_RX_MSIX_ENTRY_IDX;
    bdrConfig.rxBdrConfig[0].enThresIntr   = true;
    bdrConfig.rxBdrConfig[0].enCoalIntr    = true;
    bdrConfig.rxBdrConfig[0].intrThreshold = 1;

    bdrConfig.txBdrConfig[0].bdArray      = &g_txBuffDescrip[0][0];
    bdrConfig.txBdrConfig[0].len          = EXAMPLE_EP_TXBD_NUM;
    bdrConfig.txBdrConfig[0].dirtyArray   = &g_txDirty[0][0];
    bdrConfig.txBdrConfig[0].msixEntryIdx = EXAMPLE_EP_TX_MSIX_ENTRY_IDX;
    bdrConfig.txBdrConfig[0].enIntr       = true;

    (void)EP_GetDefaultConfig(&g_ep_config);
    g_ep_config.si                 = EXAMPLE_SWT_SI;
    g_ep_config.siConfig.txRingUse = 1;
    g_ep_config.siConfig.rxRingUse = 1;
    g_ep_config.reclaimCallback    = APP_ReclaimCallback;
    g_ep_config.msixEntry          = &msixEntry[0];
    g_ep_config.entryNum           = 3;

    result = EP_Init(&g_ep_handle, &g_macAddr[0], &g_ep_config, &bdrConfig);
    if (result != kStatus_Success)
    {
        return result;
    }

    SWT_GetDefaultConfig(&g_swt_config);

    for (i = 0; i < EXAMPLE_PORT_NUM; i++)
    {
        g_swt_config.ports[i].ethMac.miiMode   = EXAMPLE_SWT_PORT_MODE;
        g_swt_config.ports[i].ethMac.miiSpeed  = EXAMPLE_SWT_PORT_SPEED;
        g_swt_config.ports[i].ethMac.miiDuplex = EXAMPLE_SWT_PORT_DUPLEX;
        g_swt_config.ports[i].bridgeCfg.isRxVlanAware = false;
    }

    g_swt_config.bridgeCfg.dVFCfg.portMembership = EXAMPLE_SWT_USED_PORT_BITMAP;
    g_swt_config.bridgeCfg.dVFCfg.enUseFilterID = true;
    g_swt_config.bridgeCfg.dVFCfg.filterID = EXAMPLE_FRAME_FID;
    g_swt_config.bridgeCfg.dVFCfg.mfo = kNETC_FDBLookUpWithFlood;
    g_swt_config.bridgeCfg.dVFCfg.mlo = kNETC_HardwareMACLearn;

    g_swt_config.cmdRingUse            = 1U;
    g_swt_config.cmdBdrCfg[0].bdBase   = &g_cmdBuffDescrip[0];
    g_swt_config.cmdBdrCfg[0].bdLength = 8U;

    result = SWT_Init(&g_swt_handle, &g_swt_config);
    if (result != kStatus_Success)
    {
        return result;
    }
    
    return result;
}

int main(void)
{
    status_t result = kStatus_Success;

    BOARD_InitHardware();

    result = APP_MDIO_Init();
    if (result != kStatus_Success)
    {
        PRINTF("\r\nMDIO Init failed!\r\n");
        return result;
    }

    result = APP_PHY_Init();
    if (result != kStatus_Success)
    {
        PRINTF("\r\nPHY Init failed!\r\n");
        return result;
    }

    result = APP_SWT_MacLearning_Forwarding();
    if (result != kStatus_Success)
    {
        PRINTF("\r\nSwitch Init failed!\r\n");
        return result;
    }

    PRINTF("\r\nNETC Switch example start.\r\n");
    PRINTF("\r\nFrame forwarding....\r\n");

    while (1)
    {
    }
}
