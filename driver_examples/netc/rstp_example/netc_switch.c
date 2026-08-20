/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "app.h"
#include "fsl_debug_console.h"
#include "fsl_netc_endpoint.h"
#include "stp_freertos_adapter.h"
#include "fsl_rgpio.h"
#include "semphr.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
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
#endif

/* Buffer ring definitions */
#define BUFFER_RING_SIZE 16
#define BUFFER_SIZE EXAMPLE_EP_RXBUFF_SIZE_ALIGN

#define NULL_ENTRY_ID             (0xFFFFFFFF)
#define PHY_POLL_INTERVAL_MS      100
#define PHY_LINK_WAIT_TIMEOUT_MS  5000U
#define PHY_LINK_POLL_INTERVAL_US 10000U
#define TX_TIMEOUT_MS             100U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/* Buffer ring structure */
typedef struct
{
    uint8_t *buffers[BUFFER_RING_SIZE];
    volatile uint32_t read_idx;
    volatile uint32_t write_idx;
    uint32_t buffer_size;
    SemaphoreHandle_t mutex;
} buffer_ring_t;

typedef struct
{
    bool link;
    uint32_t speed_mbps;
    bool full_duplex;
} phy_status_t;

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
#if !(defined(FSL_FEATURE_NETC_HAS_NO_SWITCH) && FSL_FEATURE_NETC_HAS_NO_SWITCH)
AT_NONCACHEABLE_SECTION_ALIGN(static netc_tx_bd_t g_mgmtTxBuffDescrip[EXAMPLE_EP_TXBD_NUM], EXAMPLE_EP_BD_ALIGN);
AT_NONCACHEABLE_SECTION_ALIGN(static netc_cmd_bd_t g_cmdBuffDescrip[EXAMPLE_EP_TXBD_NUM], EXAMPLE_EP_BD_ALIGN);
#endif

/* Buffer ring for zero-copy */
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t g_bufferPool[BUFFER_RING_SIZE][BUFFER_SIZE], EXAMPLE_EP_BUFF_SIZE_ALIGN);
static buffer_ring_t g_buffer_ring;

static uint64_t rxBuffAddrArray[EXAMPLE_EP_RING_NUM][EXAMPLE_EP_RXBD_NUM];
#if !(defined(FSL_FEATURE_NETC_HAS_NO_SWITCH) && FSL_FEATURE_NETC_HAS_NO_SWITCH)
static netc_tx_frame_info_t g_mgmtTxDirty[EXAMPLE_EP_TXBD_NUM];
static netc_tx_frame_info_t mgmtTxFrameInfo;
#endif

static volatile bool txOver;
static phy_status_t g_phy_status[EXAMPLE_SWT_MAX_PORT_NUM];

/* MAC address. */
static uint8_t g_macAddr[6]    = {0x54, 0x27, 0x8d, 0x00, 0x00, 0x00};
static uint8_t bpdu_macAddr[6] = {0x01, 0x80, 0xc2, 0x00, 0x00, 0x00};

/*******************************************************************************
 * Buffer Ring Functions
 ******************************************************************************/
static status_t buffer_ring_init(buffer_ring_t *ring, uint32_t buffer_size)
{
    ring->read_idx    = 0;
    ring->write_idx   = 0;
    ring->buffer_size = buffer_size;

    /* Initialize all buffers in the ring */
    for (uint32_t i = 0; i < BUFFER_RING_SIZE; i++)
    {
        ring->buffers[i] = &g_bufferPool[i][0];
    }

    /* All buffers are initially available (write_idx points to first buffer) */
    ring->write_idx = BUFFER_RING_SIZE;

    ring->mutex = xSemaphoreCreateMutex();
    if (ring->mutex == NULL)
    {
        return kStatus_Fail;
    }

    return kStatus_Success;
}

static uint8_t *buffer_ring_alloc(buffer_ring_t *ring)
{
    uint8_t *buffer = NULL;

    xSemaphoreTake(ring->mutex, portMAX_DELAY);
    uint32_t available = (ring->write_idx - ring->read_idx);
    if (available > 0)
    {
        buffer = ring->buffers[ring->read_idx % BUFFER_RING_SIZE];
        ring->read_idx++;
    }
    xSemaphoreGive(ring->mutex);

    return buffer;
}

static status_t buffer_ring_free(buffer_ring_t *ring, uint8_t *buffer)
{
    if (!buffer)
    {
        return kStatus_InvalidArgument;
    }

    /* Check if buffer belongs to our pool */
    bool found = false;
    for (uint32_t i = 0; i < BUFFER_RING_SIZE; i++)
    {
        if (ring->buffers[i] == buffer)
        {
            found = true;
            break;
        }
    }

    if (!found)
    {
        return kStatus_InvalidArgument;
    }

    /* Return buffer to the ring */
    xSemaphoreTake(ring->mutex, portMAX_DELAY);
    ring->buffers[ring->write_idx % BUFFER_RING_SIZE] = buffer;
    ring->write_idx++;
    xSemaphoreGive(ring->mutex);

    return kStatus_Success;
}

/*******************************************************************************
 * Callback Functions
 ******************************************************************************/
static status_t APP_ReclaimCallback(ep_handle_t *handle, uint8_t ring, netc_tx_frame_info_t *frameInfo, void *userData)
{
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
    uint8_t *buff;

    /* Initialize buffer ring */
    result = buffer_ring_init(&g_buffer_ring, BUFFER_SIZE);
    if (result != kStatus_Success)
    {
        PRINTF("\r\nFailed to initialize buffer ring!\r\n");
        return result;
    }
    PRINTF("\r\nBuffer ring initialized with %d buffers\r\n", BUFFER_RING_SIZE);

    PRINTF("\r\nWait PHY link up, please link up all switch ports.\r\n");

    for (uint8_t index = 0U; index < EXAMPLE_EP_RXBD_NUM; index++)
    {
        buff = buffer_ring_alloc(&g_buffer_ring);
        if (buff == NULL)
        {
            PRINTF("No available buffer in ring\r\n");
            return kStatus_Fail;
        }
        rxBuffAddrArray[0][index] = (uint64_t)(uintptr_t)buff;
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

    /* Wait PHY link up with timeout, then configure each enabled port. */
    for (int i = 0; i < EXAMPLE_SWT_MAX_PORT_NUM; i++)
    {
        /* Only check the enabled port. */
        if (((1U << i) & EXAMPLE_SWT_USED_PORT_BITMAP) == 0U)
        {
            continue;
        }

        uint32_t elapsed_ms = 0U;
        link                = false;
        while (elapsed_ms < PHY_LINK_WAIT_TIMEOUT_MS)
        {
            result = APP_PHY_GetLinkStatus(EXAMPLE_SWT_PORT0 + i, &link);
            if (result == kStatus_Success && link)
            {
                break;
            }
            SDK_DelayAtLeastUs(PHY_LINK_POLL_INTERVAL_US, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
            elapsed_ms += 10U;
        }

        if (!link)
        {
            PRINTF("Port %d: PHY link wait timeout, using default 1G/Full config\r\n", i);
            g_swt_config.ports[i].ethMac.miiMode   = kNETC_RgmiiMode;
            g_swt_config.ports[i].ethMac.miiSpeed  = kNETC_MiiSpeed1000M;
            g_swt_config.ports[i].ethMac.miiDuplex = kNETC_MiiFullDuplex;
        }
        else
        {
            result = APP_PHY_GetLinkModeSpeedDuplex(EXAMPLE_SWT_PORT0 + i, &phyMode, &phySpeed, &phyDuplex);
            if (result != kStatus_Success)
            {
                PRINTF("\r\n%s: %d, Failed to get link status(mode, speed, duplex)!\r\n", __func__, __LINE__);
                return result;
            }
            g_swt_config.ports[i].ethMac.miiMode   = phyMode;
            g_swt_config.ports[i].ethMac.miiSpeed  = phySpeed;
            g_swt_config.ports[i].ethMac.miiDuplex = phyDuplex;
        }
        g_swt_config.ports[i].bridgeCfg.isRxVlanAware     = true;
        g_swt_config.ports[i].bridgeCfg.txVlanAction      = kNETC_TxDelOuterVlan;
        g_swt_config.ports[i].bridgeCfg.defaultVlan.vid   = 1;
        g_swt_config.ports[i].commonCfg.ipfCfg.enIPFTable = true;
    }
    g_swt_config.ports[EXAMPLE_SWT_PSEUDO_PORT].bridgeCfg.isRxVlanAware     = true;
    g_swt_config.ports[EXAMPLE_SWT_PSEUDO_PORT].bridgeCfg.txVlanAction      = kNETC_TxDelOuterVlan;
    g_swt_config.ports[EXAMPLE_SWT_PSEUDO_PORT].bridgeCfg.defaultVlan.vid   = 1;
    g_swt_config.ports[EXAMPLE_SWT_PSEUDO_PORT].commonCfg.ipfCfg.enIPFTable = true;

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
    /* Uncomment the following lines to enable zero-copy RX mode. */
/*  swtTxRxConfig.rxZeroCopy  = 1;
    swtTxRxConfig.rxBuffAlloc = rx_buff_alloc;
    swtTxRxConfig.rxBuffFree  = rx_buff_free; */
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

void netc_sw_trap_bpdu(bool enable)
{
    static netc_tb_ipf_config_t ipfEntryCfg = {0};
    static uint32_t ipfid = 0xFFFFFFFFU;
    status_t result;

    if (!enable)
    {
        if (ipfid != 0xFFFFFFFFU)
        {
            result = SWT_RxIPFDelTableEntry(&g_swt_handle, ipfid);
            if (result != kStatus_Success)
            {
                PRINTF("\r\n%s: %d, Del IPF error!\r\n", __func__, __LINE__);
            }
            ipfid = 0xFFFFFFFFU;
        }
        return;
    }

    memcpy(ipfEntryCfg.keye.dmac, bpdu_macAddr, 6);
    memset(ipfEntryCfg.keye.dmacMask, 0xff, 6);
    ipfEntryCfg.cfge.fltfa = kNETC_IPFRedirectToMgmtPort;
    ipfEntryCfg.cfge.hr    = kNETC_SoftwareDefHR0;
    ipfEntryCfg.cfge.flta  = kNETC_IPFNoAction;
    result = SWT_RxIPFAddTableEntry(&g_swt_handle, &ipfEntryCfg, &ipfid);
    if (result != kStatus_Success)
    {
        PRINTF("\r\n%s: %d, Add IPF error!\r\n", __func__, __LINE__);
        ipfid = 0xFFFFFFFFU;
    }
}

#if defined(FSL_FEATURE_NETC_HAS_SWITCH_TAG) && FSL_FEATURE_NETC_HAS_SWITCH_TAG
static int frame_add_switch_tag(uint8_t *txFrame, uint8_t *data, uint8_t portid)
{
    netc_swt_tag_port_no_ts_t tmp = {
        .comTag = {
            .tpid    = NETC_SWITCH_DEFAULT_ETHER_TYPE,
            .subType = kNETC_TagToPortNoTs,
            .type    = kNETC_TagToPort,
            .qv      = 1,
            .ipv     = 0,
            .dr      = 0,
            .swtId   = 1,
            .port    = portid
        }
    };

    memmove(txFrame, data, 12);
    memcpy(txFrame + 12, &tmp, sizeof(tmp));

    return 0;
}

static int frame_del_switch_tag(uint8_t *buffer, uint32_t *length)
{
    uint8_t tag_len = sizeof(netc_swt_tag_host_t);

    *length = *length - tag_len;

    memmove(buffer + 12, buffer + 12 + tag_len, *length - 12);

    return 0;
}
#endif

status_t netc_sw_transmit(uint8_t portid, uint8_t *data, uint16_t len)
{
    netc_buffer_struct_t txBuff;
    netc_frame_struct_t txFrame;
    status_t result = kStatus_Success;
#if defined(FSL_FEATURE_NETC_HAS_SWITCH_TAG) && FSL_FEATURE_NETC_HAS_SWITCH_TAG
    uint16_t tag_len = sizeof(netc_swt_tag_port_no_ts_t);
#else
    swt_mgmt_tx_arg_t txArg = {0};
    uint16_t tag_len        = 0;
#endif
    uint16_t total_len = len + tag_len;
    uint8_t *txData;

    txOver = false;
    txData = buffer_ring_alloc(&g_buffer_ring);
    if (txData == NULL)
    {
        PRINTF("No available buffer in ring\r\n");
        return kStatus_Fail;
    }
    else
    {
        memcpy(txData + tag_len, data, len);
    }

#if defined(FSL_FEATURE_NETC_HAS_SWITCH_TAG) && FSL_FEATURE_NETC_HAS_SWITCH_TAG
    /* Add switch tag in-place (data buffer must have room for it) */
    frame_add_switch_tag(txData, txData + tag_len, portid);
#endif

    txBuff.buffer     = txData;
    txBuff.length     = total_len;
    txFrame.buffArray = &txBuff;
    txFrame.length    = 1;

#if defined(FSL_FEATURE_NETC_HAS_SWITCH_TAG) && FSL_FEATURE_NETC_HAS_SWITCH_TAG
    result = SWT_SendFrame(&g_swt_handle, &txFrame, NULL, NULL);
#else
    txArg.ring = 0;
    result = SWT_SendFrame(&g_swt_handle, txArg, (netc_hw_port_idx_t)(kNETC_SWITCH0Port0 + portid), false, &txFrame, NULL, NULL);
#endif
    if (result != kStatus_Success)
    {
        PRINTF("\r\nTransmit frame failed, result=%d!\r\n", result);
        buffer_ring_free(&g_buffer_ring, txData);
        return result;
    }

    while (!txOver)
    {
    }

#if defined(FSL_FEATURE_NETC_HAS_SWITCH_TAG) && FSL_FEATURE_NETC_HAS_SWITCH_TAG
    SWT_ReclaimTxDescriptor(&g_swt_handle, 0);
#else
    SWT_ReclaimTxDescriptor(&g_swt_handle, false, 0);
#endif
    if (mgmtTxFrameInfo.status != kNETC_EPTxSuccess)
    {
        PRINTF("\r\nTransmit frame has error, status=%d!\r\n", mgmtTxFrameInfo.status);
        buffer_ring_free(&g_buffer_ring, txData);
        return kStatus_Fail;
    }

    buffer_ring_free(&g_buffer_ring, txData);

    return kStatus_Success;
}

status_t APP_SWT_ReceiveFrame(uint8_t *rx_buffer, uint32_t *length, uint8_t *portid)
{
    status_t result = kStatus_Success;
#if defined(FSL_FEATURE_NETC_HAS_SWITCH_TAG) && FSL_FEATURE_NETC_HAS_SWITCH_TAG
    netc_swt_tag_host_t *swt_tag;
#else
    netc_frame_attr_t attr = {0};
#endif

    do
    {
        result = SWT_GetRxFrameSize(&g_swt_handle, length);
        if (result == kStatus_NETC_RxFrameEmpty)
        {
            vTaskDelay(10);
        }
    } while (result == kStatus_NETC_RxFrameEmpty);

    if (result != kStatus_Success)
    {
        if (result == kStatus_NETC_RxHRZeroFrame)
        {
            SWT_ReceiveFrameCopy(&g_swt_handle, NULL, 0, NULL);
        }
        return result;
    }

#if defined(FSL_FEATURE_NETC_HAS_SWITCH_TAG) && FSL_FEATURE_NETC_HAS_SWITCH_TAG
    result = SWT_ReceiveFrameCopy(&g_swt_handle, rx_buffer, *length, NULL);
#else
    result = SWT_ReceiveFrameCopy(&g_swt_handle, rx_buffer, *length, &attr);
#endif
    if (result != kStatus_Success)
    {
        return result;
    }

#if defined(FSL_FEATURE_NETC_HAS_SWITCH_TAG) && FSL_FEATURE_NETC_HAS_SWITCH_TAG
    swt_tag = (netc_swt_tag_host_t *)(rx_buffer + 12);
    if (swt_tag->comTag.tpid != NETC_SWITCH_DEFAULT_ETHER_TYPE)
    {
        return kStatus_Fail;
    }
    *portid = swt_tag->comTag.port;

    frame_del_switch_tag(rx_buffer, length);
#else
    *portid = attr.srcPort;
#endif

    return result;
}

static void netc_sw_rx_task(void *pvParameters)
{
    uint8_t *rx_buffer;
    uint32_t length;
    uint8_t portid;

    rx_buffer = buffer_ring_alloc(&g_buffer_ring);
    configASSERT(rx_buffer != NULL);

    while (1)
    {
        if (APP_SWT_ReceiveFrame(rx_buffer, &length, &portid) != kStatus_Success)
        {
            continue;
        }

        stp_rx_handler(portid, rx_buffer, length);
    }
}

void netc_sw_stp_port_learning(uint8_t portIdx, bool enable)
{
    netc_swt_port_stg_mode_t state;

    if (enable)
    {
        state = kNETC_LearnWithoutFowrad;
    }
    else
    {
        state = kNETC_DiscardFrame;
    }

    SWT_SetPortSTGState(&g_swt_handle, (netc_hw_port_idx_t)portIdx, 0U, state);
}

void netc_sw_stp_port_forwarding(uint8_t portIdx, bool enable)
{
    netc_swt_port_stg_mode_t state;

    if (enable)
    {
        state = kNETC_ForwardFrame;
    }
    else
    {
        state = kNETC_DiscardFrame;
    }

    SWT_SetPortSTGState(&g_swt_handle, (netc_hw_port_idx_t)portIdx, 0U, state);
}

void netc_sw_port_fdb_flush(uint8_t portIdx)
{
    netc_tb_fdb_search_criteria_t criteria = {0};
    netc_tb_fdb_rsp_data_t rsp;
    uint32_t entryID[64];
    int count;

    criteria.resumeEntryId = NULL_ENTRY_ID;

    criteria.cfge.portBitmap = (uint32_t)1U << portIdx;
    criteria.cfgeMc          = kNETC_FDBCfgeMacthPortBitmap;
    criteria.keyeMc          = kNETC_FDBKeyeMacthAny;
    criteria.acteMc          = kNETC_FDBActeMacthAny;

    do
    {
        count                  = 0;
        criteria.resumeEntryId = NULL_ENTRY_ID;
        while (SWT_BridgeSearchFDBTableEntry(&g_swt_handle, &criteria, &rsp) == kStatus_Success && count < 64)
        {
            entryID[count++]       = rsp.status;
            criteria.resumeEntryId = rsp.status;
        }
        for (int i = 0; i < count; i++)
        {
            SWT_BridgeDelFDBTableEntry(&g_swt_handle, entryID[i]);
        }
    } while (count == 64);
}

void netc_rx_task_create(void)
{
    BaseType_t ret = xTaskCreate(
        netc_sw_rx_task,
        "netc_rx_Task",
        configMINIMAL_STACK_SIZE + 256,
        NULL,
        configMAX_PRIORITIES - 2,
        NULL
    );
    configASSERT(ret == pdPASS);
}

static void phy_poll_status(uint8_t port_index)
{
    netc_hw_mii_mode_t phyMode;
    netc_hw_mii_speed_t phySpeed   = kNETC_MiiSpeed10M;
    netc_hw_mii_duplex_t phyDuplex = kNETC_MiiHalfDuplex;
    uint32_t speed_mbps            = 0;
    bool full_duplex               = false;
    bool link                      = false;
    status_t result;

    result = APP_PHY_GetLinkStatus(EXAMPLE_SWT_PORT0 + port_index, &link);

    if (result == kStatus_Success && link)
    {
        result = APP_PHY_GetLinkModeSpeedDuplex(EXAMPLE_SWT_PORT0 + port_index, &phyMode, &phySpeed, &phyDuplex);

        if (result == kStatus_Success)
        {
            full_duplex = phyDuplex;
            switch (phySpeed)
            {
                case kNETC_MiiSpeed10M:
                    speed_mbps = 10;
                    break;
                case kNETC_MiiSpeed100M:
                    speed_mbps = 100;
                    break;
                case kNETC_MiiSpeed1000M:
                    speed_mbps = 1000;
                    break;
                case kNETC_MiiSpeed2500M:
                    speed_mbps = 2500;
                    break;
                default:
                    speed_mbps = 0;
                    break;
            }
        }
    }

    if (g_phy_status[port_index].link != link ||
        g_phy_status[port_index].speed_mbps != speed_mbps ||
        g_phy_status[port_index].full_duplex != full_duplex)
    {
        PRINTF("PHY%d status changed: link=%d, speed=%d Mbps, duplex=%s\r\n",
               port_index, link, speed_mbps, full_duplex ? "full" : "half");

        g_phy_status[port_index].link        = link;
        g_phy_status[port_index].speed_mbps  = speed_mbps;
        g_phy_status[port_index].full_duplex = full_duplex;

        stp_link_status_changed(port_index, link, speed_mbps, full_duplex);
    }
}

static void phy_poll_task(void *pvParameters)
{
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = pdMS_TO_TICKS(PHY_POLL_INTERVAL_MS);

    memset(g_phy_status, 0, sizeof(g_phy_status));

    xLastWakeTime = xTaskGetTickCount();

    while (1)
    {
        for (uint8_t port_index = 0; port_index < EXAMPLE_SWT_MAX_PORT_NUM; port_index++)
        {
            if (((1U << port_index) & EXAMPLE_SWT_USED_PORT_BITMAP) != 0U)
            {
                phy_poll_status(port_index);
            }
        }

        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

void phy_poll_task_create(void)
{
    BaseType_t ret = xTaskCreate(
        phy_poll_task,
        "phy_poll_Task",
        configMINIMAL_STACK_SIZE + 128,
        NULL,
        configMAX_PRIORITIES - 4,
        NULL
    );
    configASSERT(ret == pdPASS);
}
