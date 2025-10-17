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
#define EXAMPLE_EP_BD_ALIGN       128U
#define EXAMPLE_TX_INTR_MSG_DATA  1U
#define EXAMPLE_RX_INTR_MSG_DATA  2U
#define EXAMPLE_TX_MSIX_ENTRY_IDX 0U
#define EXAMPLE_RX_MSIX_ENTRY_IDX 1U
#define EXAMPLE_FRAME_FID         1U

#ifndef PHY_STABILITY_DELAY_US
#define PHY_STABILITY_DELAY_US (500000U)
#endif

#if !(defined(FSL_FEATURE_NETC_HAS_NO_SWITCH) && FSL_FEATURE_NETC_HAS_NO_SWITCH)
/* ENETC pseudo port for management */
#ifndef EXAMPLE_SWT_SI
#define EXAMPLE_SWT_SI kNETC_ENETC1PSI0
#endif
/* Switch pseudo port */
#ifndef EXAMPLE_SWT_PSEUDO_PORT
#define EXAMPLE_SWT_PSEUDO_PORT 0x4U
#endif
#endif

#define APP_SWT_HSR_ISQGID_BASE       0
#define APP_SWT_HSR_ISEID_BASE        0
#define APP_SWT_HSR_ETEID_BASE        0
#define APP_SWT_HSR_INTERNAL_VID_BASE 2000
#define APP_SWT_HSR_PGID              1
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
typedef enum _app_netc_hsr_operation_mode
{
    kNETC_HSR_OPERATION_MODE_H = 0U,
    kNETC_HSR_OPERATION_MODE_N,
    kNETC_HSR_OPERATION_MODE_T,
    kNETC_HSR_OPERATION_MODE_U,
    kNETC_HSR_OPERATION_MODE_X,
} app_netc_hsr_operation_mode_t;

typedef struct _app_netc_port_hsr_config
{
    bool enableHsr;
    netc_hw_port_idx_t srPortIdxA;
    netc_hw_port_idx_t srPortIdxB;
    app_netc_hsr_operation_mode_t operMode;
} app_netc_port_hsr_config_t;

/* Rx buffer memeory type. */
typedef uint8_t rx_buffer_t[EXAMPLE_EP_RXBUFF_SIZE_ALIGN];

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* EP resource. */
static ep_handle_t g_ep_handle;

#if !(defined(FSL_FEATURE_NETC_HAS_NO_SWITCH) && FSL_FEATURE_NETC_HAS_NO_SWITCH)
/* SWT resource. */
static swt_handle_t g_swt_handle;
static swt_config_t g_swt_config;
static swt_transfer_config_t swtTxRxConfig;
#endif

/* Buffer descriptor resource. */
AT_NONCACHEABLE_SECTION_ALIGN(static netc_rx_bd_t g_rxBuffDescrip[EXAMPLE_EP_RING_NUM][EXAMPLE_EP_RXBD_NUM],
                              EXAMPLE_EP_BD_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static rx_buffer_t g_rxDataBuff[EXAMPLE_EP_RING_NUM][EXAMPLE_EP_RXBD_NUM],
                              EXAMPLE_EP_BUFF_SIZE_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t g_txFrame[EXAMPLE_EP_TEST_FRAME_SIZE], EXAMPLE_EP_BUFF_SIZE_ALIGN);
#if !(defined(FSL_FEATURE_NETC_HAS_NO_SWITCH) && FSL_FEATURE_NETC_HAS_NO_SWITCH)
AT_NONCACHEABLE_SECTION_ALIGN(static netc_tx_bd_t g_mgmtTxBuffDescrip[EXAMPLE_EP_TXBD_NUM], EXAMPLE_EP_BD_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static netc_cmd_bd_t g_cmdBuffDescrip[EXAMPLE_EP_TXBD_NUM], EXAMPLE_EP_BD_ALIGN);
#endif
AT_NONCACHEABLE_SECTION(static uint8_t g_rxFrame[EXAMPLE_EP_RXBUFF_SIZE_ALIGN]);
static uint64_t rxBuffAddrArray[EXAMPLE_EP_RING_NUM][EXAMPLE_EP_RXBD_NUM];
#if !(defined(FSL_FEATURE_NETC_HAS_NO_SWITCH) && FSL_FEATURE_NETC_HAS_NO_SWITCH)
static netc_tx_frame_info_t g_mgmtTxDirty[EXAMPLE_EP_TXBD_NUM];
static netc_tx_frame_info_t mgmtTxFrameInfo;
#endif
// static netc_tx_frame_info_t txFrameInfo;
static volatile bool txOver;

/* MAC address. */
static uint8_t g_macAddr[6] = {0x54, 0x27, 0x8d, 0x00, 0x00, 0x00};

/*******************************************************************************
 * Code
 ******************************************************************************/

#if defined(FSL_FEATURE_NETC_HAS_SWITCH_TAG) && FSL_FEATURE_NETC_HAS_SWITCH_TAG
/*! @brief Build Frame for single ring transmit. */
static void APP_BuildBroadCastFrameSwtTag(void)
{
    netc_swt_tag_port_no_ts_t tag = {.comTag = {.tpid    = NETC_SWITCH_DEFAULT_ETHER_TYPE,
                                                .subType = kNETC_TagToPortNoTs,
                                                .type    = kNETC_TagForward,
                                                .qv      = 1,
                                                .ipv     = 0,
                                                .dr      = 0,
                                                .swtId   = 1,
                                                .port    = 0}};
    uint32_t headerSize           = 14U + sizeof(tag);
    uint32_t length               = EXAMPLE_EP_TEST_FRAME_SIZE - headerSize;
    uint32_t count;

    for (count = 0; count < 6U; count++)
    {
        g_txFrame[count] = 0xFFU;
    }
    memcpy(&g_txFrame[6], &g_macAddr[0], 6U);
    memcpy(&g_txFrame[12], &tag, sizeof(tag));

    g_txFrame[12 + sizeof(tag)] = (length >> 8U) & 0xFFU;
    g_txFrame[13 + sizeof(tag)] = length & 0xFFU;

    for (count = 0; count < length; count++)
    {
        g_txFrame[count + headerSize] = count % 0xFFU;
    }
}
#endif

static status_t APP_ReclaimCallback(ep_handle_t *handle, uint8_t ring, netc_tx_frame_info_t *frameInfo, void *userData)
{
    // txFrameInfo = *frameInfo;
    return kStatus_Success;
}

#if !(defined(FSL_FEATURE_NETC_HAS_NO_SWITCH) && FSL_FEATURE_NETC_HAS_NO_SWITCH)
static status_t APP_SwtReclaimCallback(swt_handle_t *handle, netc_tx_frame_info_t *frameInfo, void *userData)
{
    mgmtTxFrameInfo = *frameInfo;
    return kStatus_Success;
}
#endif

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
}

status_t APP_SWT_SendFrame(void)
{
    netc_buffer_struct_t txBuff = {.buffer = &g_txFrame, .length = sizeof(g_txFrame)};
    netc_frame_struct_t txFrame = {.buffArray = &txBuff, .length = 1};
    status_t result             = kStatus_Success;

    APP_BuildBroadCastFrameSwtTag();
    txOver = false;
    result = SWT_SendFrame(&g_swt_handle, &txFrame, NULL, NULL);
    if (result != kStatus_Success)
    {
        PRINTF("\r\nTransmit frame failed!\r\n");
        return result;
    }
    while (!txOver)
    {
    }
    SWT_ReclaimTxDescriptor(&g_swt_handle, 0);
    if (mgmtTxFrameInfo.status != kNETC_EPTxSuccess)
    {
        PRINTF("\r\nTransmit frame has error!\r\n");
        return kStatus_Fail;
    }
    PRINTF("The frame transmitted success!\r\n");

    return result;
}

status_t APP_SWT_ReceiveFrame()
{
    status_t result = kStatus_Success;
    uint32_t length;
    do
    {
        result = EP_GetRxFrameSize(&g_ep_handle, 0, &length);
    } while (result == kStatus_NETC_RxFrameEmpty);
    if (result != kStatus_Success)
    {
        return result;
    }

    result = EP_ReceiveFrameCopy(&g_ep_handle, 0, g_rxFrame, length, NULL);
    if (result != kStatus_Success)
    {
        return result;
    }
    PRINTF(" A frame received. The length is %d ", length);
    PRINTF(" Dest Address %02x:%02x:%02x:%02x:%02x:%02x Src Address %02x:%02x:%02x:%02x:%02x:%02x \r\n", g_rxFrame[0],
           g_rxFrame[1], g_rxFrame[2], g_rxFrame[3], g_rxFrame[4], g_rxFrame[5], g_rxFrame[6], g_rxFrame[7],
           g_rxFrame[8], g_rxFrame[9], g_rxFrame[10], g_rxFrame[11]);

    return result;
}

status_t APP_SWT_SetHsrModeN(swt_handle_t *handle, app_netc_port_hsr_config_t *hsrConfig)
{
    static netc_tb_ipf_config_t ipfEntryCfg = {0};
    status_t result                         = kStatus_Success;
    netc_tb_is_config_t isconf              = {0};
    netc_tb_vf_config_t vlanconf            = {0};
    netc_tb_et_config_t ettconf             = {0};
    uint32_t isEID                          = APP_SWT_HSR_ISEID_BASE;
    uint32_t etEID                          = APP_SWT_HSR_ETEID_BASE;
    uint16_t vid                            = APP_SWT_HSR_INTERNAL_VID_BASE;
    uint32_t nonSrPortMask;
    uint32_t vftEID;
    uint32_t ipfid;

    nonSrPortMask =
        ((1U << NETC_SOC_SWT_PORT_NUM) - 1U) & (~((1U << hsrConfig->srPortIdxA) | (1U << hsrConfig->srPortIdxB)));

    for (uint32_t i = 0U; i < NETC_SOC_SWT_PORT_NUM; i++)
    {
        if (i == hsrConfig->srPortIdxA || i == hsrConfig->srPortIdxB)
        {
            ipfEntryCfg.keye.srcPort     = i;
            ipfEntryCfg.keye.srcPortMask = 0x1f;
            ipfEntryCfg.cfge.fltfa       = kNETC_IPFForwardPermit;
            ipfEntryCfg.cfge.hr          = kNETC_SoftwareDefHR0;
            ipfEntryCfg.cfge.flta        = kNETC_IPFWithIngressStream;
            ipfEntryCfg.cfge.fltaTgt     = isEID;

            result = SWT_RxIPFAddTableEntry(handle, &ipfEntryCfg, &ipfid);
            if (result != kStatus_Success)
                return result;

            isconf.entryID            = isEID;
            isconf.cfge.fa            = kNETC_ISBridgeForward;
            isconf.cfge.sgiEID        = 0xffffffff;
            isconf.cfge.etEID         = 0xffffffff;
            isconf.cfge.isqEID        = 0xffffffff;
            isconf.cfge.iscEID        = 0xffffffff;
            isconf.cfge.etEID         = 0xffffffff;
            isconf.cfge.ifmeLenChange = 4;
            isconf.cfge.ifmEID        = NETC_FD_EID_ENCODE_OPTION_2(kNETC_AddCVlanPcpAndDei, vid);

            result = SWT_RxPSFPAddISTableEntry(handle, &isconf);
            if (result != kStatus_Success)
                return result;

            vlanconf.cfge.portMembership = nonSrPortMask | (1 << i);
            vlanconf.cfge.fid            = vid;
            vlanconf.cfge.baseETEID      = APP_SWT_HSR_ETEID_BASE;
            vlanconf.cfge.etaPortBitmap  = nonSrPortMask;
            vlanconf.cfge.mlo            = 2;
            vlanconf.cfge.mfo            = 2;
            vlanconf.keye.vid            = vid;
            result                       = SWT_BridgeAddVFTableEntry(handle, &vlanconf, &vftEID);
            if (result != kStatus_Success)
                return result;

            isEID++;
            vid++;
        }
        else
        {
            /* Bypass build warning. */
            uint8_t efmLenChange_temp = (uint8_t)-4;
            ettconf.entryID           = etEID;
            ettconf.cfge.esqaTgtEID   = 0xffffffff;
            ettconf.cfge.efmLenChange = efmLenChange_temp;
            ettconf.cfge.efmEID       = NETC_FD_EID_ENCODE_OPTION_1(kNETC_NoSqtAction, kNETC_DelVlan);

            result = SWT_TxEPPAddETTableEntry(handle, &ettconf);
            if (result != kStatus_Success)
                return result;

            etEID++;
        }
    }

    return result;
}

status_t APP_SWT_SetHSH(swt_handle_t *handle, app_netc_port_hsr_config_t *hsrConfig)
{
    status_t status                   = kStatus_Success;
    netc_tb_iseqg_config_t isqgConfig = {0};
    netc_swt_port_sr_config_t sr      = {0};
    uint32_t isqgId                   = APP_SWT_HSR_ISQGID_BASE;

    if (!hsrConfig->enableHsr)
    {
        sr.isqEID = 0xFFFF;
        for (uint32_t i = 0U; i < NETC_SOC_SWT_PORT_NUM; i++)
        {
            SWT_SetPortSR(handle, (netc_hw_port_idx_t)i, &sr);
            SWT_SetPortGroup(handle, (netc_hw_port_idx_t)i, 0);
        }

        return status;
    }

    for (uint32_t i = 0U; i < NETC_SOC_SWT_PORT_NUM; i++)
    {
        memset(&sr, 0, sizeof(sr));
        if (i == hsrConfig->srPortIdxA || i == hsrConfig->srPortIdxB)
        {
            sr.pathId = (i == hsrConfig->srPortIdxA) ? 0 : 1;
            sr.isqEID = 0xFFFF;
            sr.sdfa   = 1;
            sr.srPort = 1;

            if (hsrConfig->operMode == kNETC_HSR_OPERATION_MODE_T)
                sr.txSqta = 1;
            else if (hsrConfig->operMode == kNETC_HSR_OPERATION_MODE_X)
                sr.srcPortFlt = 1;

            SWT_SetPortSR(handle, (netc_hw_port_idx_t)i, &sr);
            SWT_SetPortGroup(handle, (netc_hw_port_idx_t)i, APP_SWT_HSR_PGID);
            g_swt_config.ports[i].bridgeCfg.enMacStationMove = 0;
            SWT_EnablePortMacStationMove(handle, (netc_hw_port_idx_t)i,
                                         g_swt_config.ports[i].bridgeCfg.enMacStationMove);
        }
        else
        {
            isqgConfig.entryID    = isqgId;
            isqgConfig.cfge.sqTag = kNETC_SqHsrTag;

            status = SWT_FRERAddISEQGTableEntry(handle, &isqgConfig);
            if (status != kStatus_Success)
                return status;

            sr.sdfa   = 1;
            sr.txSqta = 1;
            sr.isqEID = isqgId;
            SWT_SetPortSR(handle, (netc_hw_port_idx_t)i, &sr);

            isqgId++;

            if (hsrConfig->operMode == kNETC_HSR_OPERATION_MODE_U)
                SWT_SetPortGroup(handle, (netc_hw_port_idx_t)i, APP_SWT_HSR_PGID);
            else
                SWT_SetPortGroup(handle, (netc_hw_port_idx_t)i, 0);
        }
    }

    if (hsrConfig->operMode == kNETC_HSR_OPERATION_MODE_N)
    {
        status = APP_SWT_SetHsrModeN(handle, hsrConfig);
        if (status != kStatus_Success)
        {
            PRINTF("\r\n HSR ModeN set error, status=%d\n", status);
            return status;
        }
    }

    return status;
}

#if !(defined(FSL_FEATURE_NETC_HAS_NO_SWITCH) && FSL_FEATURE_NETC_HAS_NO_SWITCH)
status_t APP_SWT_Init(void)
{
    status_t result                  = kStatus_Success;
    netc_rx_bdr_config_t rxBdrConfig = {0};
    netc_tx_bdr_config_t txBdrConfig = {0};
    netc_bdr_config_t bdrConfig      = {.rxBdrConfig = &rxBdrConfig, .txBdrConfig = &txBdrConfig};
    bool link                        = false;
    netc_msix_entry_t msixEntry[2];
    netc_hw_mii_mode_t phyMode;
    netc_hw_mii_speed_t phySpeed;
    netc_hw_mii_duplex_t phyDuplex;
    ep_config_t g_ep_config;
    uint32_t msgAddr;

    PRINTF("\r\nNETC Switch frame loopback example start.\r\n");

    /* MSIX and interrupt configuration. */
    MSGINTR_Init(EXAMPLE_MSGINTR, &msgintrCallback);
    msgAddr              = MSGINTR_GetIntrSelectAddr(EXAMPLE_MSGINTR, 0);
    msixEntry[0].control = kNETC_MsixIntrMaskBit;
    msixEntry[0].msgAddr = msgAddr;
    msixEntry[0].msgData = EXAMPLE_TX_INTR_MSG_DATA;
    msixEntry[1].control = kNETC_MsixIntrMaskBit;
    msixEntry[1].msgAddr = msgAddr;
    msixEntry[1].msgData = EXAMPLE_RX_INTR_MSG_DATA;

    bdrConfig.rxBdrConfig[0].bdArray       = &g_rxBuffDescrip[0][0];
    bdrConfig.rxBdrConfig[0].len           = EXAMPLE_EP_RXBD_NUM;
    bdrConfig.rxBdrConfig[0].extendDescEn  = false;
    bdrConfig.rxBdrConfig[0].buffAddrArray = &rxBuffAddrArray[0][0];
    bdrConfig.rxBdrConfig[0].buffSize      = EXAMPLE_EP_RXBUFF_SIZE_ALIGN;
    bdrConfig.rxBdrConfig[0].msixEntryIdx  = EXAMPLE_RX_MSIX_ENTRY_IDX;
    bdrConfig.rxBdrConfig[0].enThresIntr   = true;
    bdrConfig.rxBdrConfig[0].enCoalIntr    = true;
    bdrConfig.rxBdrConfig[0].intrThreshold = 1;

    (void)EP_GetDefaultConfig(&g_ep_config);
    g_ep_config.si                 = EXAMPLE_SWT_SI;
    g_ep_config.siConfig.txRingUse = 1;
    g_ep_config.siConfig.rxRingUse = 1;
    g_ep_config.reclaimCallback    = APP_ReclaimCallback;
    g_ep_config.msixEntry          = &msixEntry[0];
    g_ep_config.entryNum           = 2;
#ifdef EXAMPLE_ENABLE_CACHE_MAINTAIN
    g_ep_config.rxCacheMaintain = true;
    g_ep_config.txCacheMaintain = true;
#endif
    result = EP_Init(&g_ep_handle, &g_macAddr[0], &g_ep_config, &bdrConfig);
    if (result != kStatus_Success)
    {
        return result;
    }

    SWT_GetDefaultConfig(&g_swt_config);

    /* Wait PHY link up. */
    PRINTF("Wait for PHY link up...\r\n");
    for (int i = 0; i < EXAMPLE_SWT_MAX_PORT_NUM; i++)
    {
        /* Only check the enabled port. */
        if (((1U << i) & EXAMPLE_SWT_USED_PORT_BITMAP) == 0U)
        {
            continue;
        }

        do
        {
            result = APP_PHY_GetLinkStatus(EXAMPLE_SWT_PORT0 + i, &link);
        } while ((result != kStatus_Success) || (!link));
        result = APP_PHY_GetLinkModeSpeedDuplex(EXAMPLE_SWT_PORT0 + i, &phyMode, &phySpeed, &phyDuplex);
        if (result != kStatus_Success)
        {
            PRINTF("\r\n%s: %d, Failed to get link status(mode, speed, dumplex)!\r\n", __func__, __LINE__);
            return result;
        }
        g_swt_config.ports[i].ethMac.miiMode              = phyMode;
        g_swt_config.ports[i].ethMac.miiSpeed             = phySpeed;
        g_swt_config.ports[i].ethMac.miiDuplex            = phyDuplex;
        g_swt_config.ports[i].bridgeCfg.isRxVlanAware     = true;
        g_swt_config.ports[i].bridgeCfg.txVlanAction	  = kNETC_TxDelOuterVlan;
        g_swt_config.ports[i].bridgeCfg.defaultVlan.vid	  = 1;
        g_swt_config.ports[i].commonCfg.ipfCfg.enIPFTable = true;
    }

    g_swt_config.bridgeCfg.dVFCfg.portMembership = 0x1FU;
    g_swt_config.bridgeCfg.dVFCfg.enUseFilterID  = true;
    g_swt_config.bridgeCfg.dVFCfg.filterID       = EXAMPLE_FRAME_FID;
    g_swt_config.bridgeCfg.dVFCfg.mfo            = kNETC_FDBLookUpWithFlood;
    g_swt_config.bridgeCfg.dVFCfg.mlo            = kNETC_HardwareMACLearn;

    g_swt_config.cmdRingUse            = 1U;
    g_swt_config.cmdBdrCfg[0].bdBase   = &g_cmdBuffDescrip[0];
    g_swt_config.cmdBdrCfg[0].bdLength = 8U;

    result = SWT_Init(&g_swt_handle, &g_swt_config);
    if (result != kStatus_Success)
    {
        PRINTF("\r\n%s: %d, Failed to initialize switch!\r\n", __func__, __LINE__);
        return result;
    }

    /* Configure switch transfer resource. */
    swtTxRxConfig.enUseMgmtRxBdRing            = false;
    swtTxRxConfig.enUseMgmtTxBdRing            = true;
    swtTxRxConfig.mgmtTxBdrConfig.bdArray      = &g_mgmtTxBuffDescrip[0];
    swtTxRxConfig.mgmtTxBdrConfig.len          = EXAMPLE_EP_TXBD_NUM;
    swtTxRxConfig.mgmtTxBdrConfig.dirtyArray   = &g_mgmtTxDirty[0];
    swtTxRxConfig.mgmtTxBdrConfig.msixEntryIdx = EXAMPLE_TX_MSIX_ENTRY_IDX;
    swtTxRxConfig.mgmtTxBdrConfig.enIntr       = true;
    swtTxRxConfig.reclaimCallback              = APP_SwtReclaimCallback;
#ifdef EXAMPLE_ENABLE_CACHE_MAINTAIN
    swtTxRxConfig.rxCacheMaintain = true;
    swtTxRxConfig.txCacheMaintain = true;
#endif
    result = SWT_ManagementTxRxConfig(&g_swt_handle, &g_ep_handle, &swtTxRxConfig);
    if (kStatus_Success != result)
    {
        PRINTF("\r\n%s: %d, Failed to config TxRx!\r\n", __func__, __LINE__);
        return result;
    }

    /* Unmask MSIX message interrupt. */
    EP_MsixSetEntryMask(&g_ep_handle, EXAMPLE_TX_MSIX_ENTRY_IDX, false);
    EP_MsixSetEntryMask(&g_ep_handle, EXAMPLE_RX_MSIX_ENTRY_IDX, false);

    return result;
}
#endif

int main(void)
{
    status_t result                      = kStatus_Success;
    app_netc_port_hsr_config_t hsrConfig = {0};

    BOARD_InitHardware();

    PRINTF("MCUX SDK version: %s\r\n", MCUXSDK_VERSION_FULL_STR);

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

    for (uint8_t ring = 0U; ring < EXAMPLE_EP_RING_NUM; ring++)
    {
        for (uint8_t index = 0U; index < EXAMPLE_EP_RXBD_NUM; index++)
        {
            rxBuffAddrArray[ring][index] = (uint64_t)(uintptr_t)&g_rxDataBuff[ring][index];
        }
    }

    APP_SWT_Init();

    PRINTF("\r\n Configure HSR!\r\n");
    hsrConfig.enableHsr  = 1;
    hsrConfig.srPortIdxA = (netc_hw_port_idx_t)0;
    hsrConfig.srPortIdxB = (netc_hw_port_idx_t)2;
    hsrConfig.operMode   = kNETC_HSR_OPERATION_MODE_H;
    result               = APP_SWT_SetHSH(&g_swt_handle, &hsrConfig);
    if (result != kStatus_Success)
    {
        PRINTF("\r\n HSR set error, result=%d\n", result);
        return result;
    }

    PRINTF("\r\n Press any key to send a broadcast frame\r\n");
    GETCHAR();
    APP_SWT_SendFrame();

    while (1)
    {
        APP_SWT_ReceiveFrame();
    }
}
