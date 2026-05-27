/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "prp_device.h"
#include "prp_frame.h"
#include "app.h"
#include "fsl_debug_console.h"
#include "fsl_netc_endpoint.h"

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

#define EXAMPLE_SWT_PORT_NUM 4

/* Buffer ring definitions */
#define BUFFER_RING_SIZE 32
#define BUFFER_SIZE EXAMPLE_EP_RXBUFF_SIZE_ALIGN

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/* Buffer ring structure */
typedef struct {
    uint8_t *buffers[BUFFER_RING_SIZE];
    volatile uint32_t read_idx;
    volatile uint32_t write_idx;
    uint32_t buffer_size;
} buffer_ring_t;

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

/* MAC address. */
static uint8_t g_macAddr[6] = {0x54, 0x27, 0x8d, 0x00, 0x00, 0x00};

/*******************************************************************************
 * Buffer Ring Functions
 ******************************************************************************/
static status_t buffer_ring_init(buffer_ring_t *ring, uint32_t buffer_size)
{
    ring->read_idx = 0;
    ring->write_idx = 0;
    ring->buffer_size = buffer_size;

    /* Initialize all buffers in the ring */
    for (uint32_t i = 0; i < BUFFER_RING_SIZE; i++) {
        ring->buffers[i] = &g_bufferPool[i][0];
    }

    /* All buffers are initially available (write_idx points to first buffer) */
    ring->write_idx = BUFFER_RING_SIZE;

    return kStatus_Success;
}

static uint8_t *buffer_ring_alloc(buffer_ring_t *ring)
{
    uint8_t *buffer = NULL;

    uint32_t available = (ring->write_idx - ring->read_idx);
    if (available > 0) {
        buffer = ring->buffers[ring->read_idx % BUFFER_RING_SIZE];
        ring->read_idx++;
    }

    return buffer;
}

static status_t buffer_ring_free(buffer_ring_t *ring, uint8_t *buffer)
{
    if (!buffer) {
        return kStatus_InvalidArgument;
    }

    /* Check if buffer belongs to our pool */
    bool found = false;
    for (uint32_t i = 0; i < BUFFER_RING_SIZE; i++) {
        if (ring->buffers[i] == buffer) {
            found = true;
            break;
        }
    }

    if (!found) {
        return kStatus_InvalidArgument;
    }

    /* Return buffer to the ring */
    ring->buffers[ring->write_idx % BUFFER_RING_SIZE] = buffer;
    ring->write_idx++;

    return kStatus_Success;
}

static void *rx_buff_alloc(swt_handle_t *handle, uint32_t length, void *userData)
{
    return buffer_ring_alloc(&g_buffer_ring);
}

static void rx_buff_free(swt_handle_t *handle, void *buffer, void *userData)
{
    buffer_ring_free(&g_buffer_ring, buffer);
}

static uint32_t buffer_ring_available(buffer_ring_t *ring)
{
    uint32_t available;

    available = (ring->write_idx - ring->read_idx);

    return available;
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
    static netc_tb_ipf_config_t ipfEntryCfg = {0};
    uint32_t ipfid;
    uint8_t *buff;

    /* Initialize buffer ring */
    result = buffer_ring_init(&g_buffer_ring, BUFFER_SIZE);
    if (result != kStatus_Success) {
        PRINTF("\r\nFailed to initialize buffer ring!\r\n");
        return result;
    }
    PRINTF("\r\nBuffer ring initialized with %d buffers\r\n", BUFFER_RING_SIZE);

    PRINTF("\r\nWait PHY link up, please link up all switch ports.\r\n");

    for (uint8_t index = 0U; index < EXAMPLE_EP_RXBD_NUM; index++) {
        buff = buffer_ring_alloc(&g_buffer_ring);
        if (buff == NULL) {
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

    /* Wait PHY link up. */
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
        g_swt_config.ports[i].bridgeCfg.txVlanAction      = kNETC_TxDelOuterVlan;
        g_swt_config.ports[i].bridgeCfg.defaultVlan.vid   = 1;
        g_swt_config.ports[i].commonCfg.ipfCfg.enIPFTable = true;
    }
    g_swt_config.ports[3].bridgeCfg.isRxVlanAware     = true;
    g_swt_config.ports[3].bridgeCfg.txVlanAction          = kNETC_TxDelOuterVlan;
    g_swt_config.ports[3].bridgeCfg.defaultVlan.vid       = 1;
    g_swt_config.ports[3].commonCfg.ipfCfg.enIPFTable = true;

    g_swt_config.bridgeCfg.dVFCfg.portMembership = 0x1FU;
    g_swt_config.bridgeCfg.dVFCfg.enUseFilterID  = true;
    g_swt_config.bridgeCfg.dVFCfg.filterID       = EXAMPLE_FRAME_FID;
    g_swt_config.bridgeCfg.dVFCfg.mfo            = kNETC_FDBLookUpWithFlood;
    g_swt_config.bridgeCfg.dVFCfg.mlo            = kNETC_DisableMACLearn;

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
    swtTxRxConfig.rxZeroCopy = 1;
    swtTxRxConfig.rxBuffAlloc = rx_buff_alloc;
    swtTxRxConfig.rxBuffFree = rx_buff_free;
    result = SWT_ManagementTxRxConfig(&g_swt_handle, &g_ep_handle, &swtTxRxConfig);
    if (kStatus_Success != result)
    {
        PRINTF("\r\n%s: %d, Failed to config TxRx!\r\n", __func__, __LINE__);
        return result;
    }

    /* Unmask MSIX message interrupt. */
    EP_MsixSetEntryMask(&g_ep_handle, EXAMPLE_TX_MSIX_ENTRY_IDX, false);
    EP_MsixSetEntryMask(&g_ep_handle, EXAMPLE_RX_MSIX_ENTRY_IDX, false);

    ipfEntryCfg.keye.srcPortMask = 0x1f;
    ipfEntryCfg.cfge.fltfa       = kNETC_IPFRedirectToMgmtPort;
    ipfEntryCfg.cfge.hr          = kNETC_SoftwareDefHR0;
    ipfEntryCfg.cfge.flta        = kNETC_IPFNoAction;
    for (int i = 0; i < EXAMPLE_SWT_MAX_PORT_NUM; i++) {
        ipfEntryCfg.keye.srcPort     = i;
        result = SWT_RxIPFAddTableEntry(&g_swt_handle, &ipfEntryCfg, &ipfid);
        if (result != kStatus_Success) {
            PRINTF("\r\n%s: %d, Add IPF error!\r\n", __func__, __LINE__);
        }
    }

    return result;
}

status_t APP_SWT_ReceiveFrame(uint8_t **buffer, uint32_t *length, uint8_t *portid)
{
    status_t result = kStatus_Success;
    netc_frame_attr_t attr = {0};
    netc_buffer_struct_t rxBuff;
    netc_frame_struct_t rxFrame;

    rxBuff.length = 0;
    rxBuff.buffer = NULL;
    rxFrame.buffArray = &rxBuff;
    rxFrame.length = 1;
    do {
#if defined(FSL_FEATURE_NETC_HAS_SWITCH_TAG) && FSL_FEATURE_NETC_HAS_SWITCH_TAG
        result = SWT_ReceiveFrame(&g_swt_handle, &rxFrame, NULL);
#else
        result = SWT_ReceiveFrame(&g_swt_handle, &rxFrame, &attr);
#endif
    } while (result == kStatus_NETC_RxFrameEmpty);

    if (result == kStatus_Success) {
	*buffer = rxBuff.buffer;
	*length = rxBuff.length;
        *portid = attr.srcPort;
    }

    return result;
}

#if defined(FSL_FEATURE_NETC_HAS_SWITCH_TAG) && FSL_FEATURE_NETC_HAS_SWITCH_TAG
static int frame_add_switch_tag(uint8_t *txFrame, uint8_t *data, uint16_t len, uint8_t portid)
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

static int frame_del_switch_tag(uint8_t *txFrame, uint8_t *data)
{
    memmove(data, txFrame, 12);

    return 0;
}
#endif

/* Zero-copy ethernet send function */
static int ethernet_send_frame(prp_port_t *port, uint8_t *data, uint16_t len)
{
    netc_buffer_struct_t txBuff;
    netc_frame_struct_t txFrame;
    status_t result = kStatus_Success;
#if defined(FSL_FEATURE_NETC_HAS_SWITCH_TAG) && FSL_FEATURE_NETC_HAS_SWITCH_TAG
    uint16_t tag_len = sizeof(netc_swt_tag_port_no_ts_t);
#else
    swt_mgmt_tx_arg_t txArg = {0};
    uint16_t tag_len = 0;
#endif
    uint16_t total_len = len + tag_len;
    uint8_t *txData = data - tag_len;

#if defined(FSL_FEATURE_NETC_HAS_SWITCH_TAG) && FSL_FEATURE_NETC_HAS_SWITCH_TAG
    /* Add switch tag in-place (data buffer must have room for it) */
    frame_add_switch_tag(txData, data, len, port->portid);
#endif

    txBuff.buffer = txData;
    txBuff.length = total_len;
    txFrame.buffArray = &txBuff;
    txFrame.length = 1;

    txOver = false;
#if defined(FSL_FEATURE_NETC_HAS_SWITCH_TAG) && FSL_FEATURE_NETC_HAS_SWITCH_TAG
    result = SWT_SendFrame(&g_swt_handle, &txFrame, NULL, NULL);
#else
    txArg.ring = 0;
    result = SWT_SendFrame(&g_swt_handle, txArg, (netc_hw_port_idx_t)(kNETC_SWITCH0Port0 + port->portid), false, &txFrame, NULL, NULL);
#endif
    if (result != kStatus_Success)
    {
        PRINTF("\r\nTransmit frame failed!\r\n");
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
        PRINTF("\r\nTransmit frame has error!\r\n");
        return kStatus_Fail;
    }

#if defined(FSL_FEATURE_NETC_HAS_SWITCH_TAG) && FSL_FEATURE_NETC_HAS_SWITCH_TAG
    frame_del_switch_tag(txData, data);
#endif

    return 0;
}

static void prp_master_recieve(uint8_t *data, uint16_t len)
{
    /* Add PRP frame process after the frame recieved on CPU */
}

/* Main example function */
void prp_example_main(void *pvParameters)
{
    uint8_t *master_mac = g_macAddr;
    uint8_t test_data[] = {
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0x54, 0x27, 0x8D, 0x00, 0x00, 0x00,
        0x08, 0x00,
        0x45, 0x00, 0x00, 0x1C,
    };
    uint32_t length;
#if defined(FSL_FEATURE_NETC_HAS_SWITCH_TAG) && FSL_FEATURE_NETC_HAS_SWITCH_TAG
    netc_swt_tag_host_t *swt_tag;
    uint32_t tag_len = sizeof(netc_swt_tag_host_t);
#else
    uint32_t tag_len = 0;
#endif
    prp_port_type_t swt_port[EXAMPLE_SWT_PORT_NUM] = {PRP_PT_NONE};
    uint8_t *rx_buffer, *tx_buffer;
    prp_frame_t *rx_frame = NULL;
    uint8_t portid;

    PRINTF("\r\nNETC PRP example\r\n");
    if (APP_SWT_Init() != kStatus_Success) {
        PRINTF("\r\nNETC SWT Init error\r\n");
	return;
    }
    PRINTF("Available buffers: %d\r\n", buffer_ring_available(&g_buffer_ring));

    prp_priv_t *prp = prp_create_device(master_mac, master_mac);
    if (!prp) {
        PRINTF("Failed to create PRP device\n");
        return;
    }

    prp->ports[PRP_PT_SLAVE_A].send_frame = ethernet_send_frame;
    prp->ports[PRP_PT_SLAVE_B].send_frame = ethernet_send_frame;
    prp->ports[PRP_PT_INTERLINK].send_frame = ethernet_send_frame;
    prp->master_rx_callback = prp_master_recieve;
    prp->supervision_buffer = buffer_ring_alloc(&g_buffer_ring);
    if (prp->supervision_buffer == NULL) {
        PRINTF("No available buffer in ring\r\n");
	goto err;
    }

    swt_port[0] = PRP_PT_SLAVE_A;
    swt_port[1] = PRP_PT_SLAVE_B;
    swt_port[2] = PRP_PT_INTERLINK;

    prp->ports[PRP_PT_SLAVE_A].portid = 0;
    prp->ports[PRP_PT_SLAVE_B].portid = 1;
    prp->ports[PRP_PT_INTERLINK].portid = 2;

    tx_buffer = buffer_ring_alloc(&g_buffer_ring);
    if (tx_buffer == NULL) {
        PRINTF("No available buffer in ring\r\n");
    } else {
        uint16_t buffer_size = BUFFER_SIZE - tag_len;

        memcpy(tx_buffer + tag_len, test_data, sizeof(test_data));
        prp->prp_tx(prp, tx_buffer + tag_len, sizeof(test_data), buffer_size);
        buffer_ring_free(&g_buffer_ring, tx_buffer);
    }

    rx_frame = pvPortMalloc(sizeof(prp_frame_t));
    if (rx_frame == NULL) {
        goto err;
    }

    while(1) {
        if (APP_SWT_ReceiveFrame(&rx_buffer, &length, &portid) != kStatus_Success)
            continue;

#if defined(FSL_FEATURE_NETC_HAS_SWITCH_TAG) && FSL_FEATURE_NETC_HAS_SWITCH_TAG
        swt_tag = (netc_swt_tag_host_t *)(rx_buffer + 12);

        if (swt_tag->comTag.tpid != 0x3afd) {
            buffer_ring_free(&g_buffer_ring, rx_buffer);
            continue;
        }
	portid = swt_tag->comTag.port;
#endif
        memset(rx_frame, 0, sizeof(prp_frame_t));
        rx_frame->buffer = rx_buffer;

#if defined(FSL_FEATURE_NETC_HAS_SWITCH_TAG) && FSL_FEATURE_NETC_HAS_SWITCH_TAG
        /* Remove switch tag by moving MAC addresses */
        memmove(rx_buffer + sizeof(netc_swt_tag_host_t), rx_buffer, 12);
#endif
        rx_frame->data = rx_buffer + tag_len;
	rx_frame->buffer_size = BUFFER_SIZE - tag_len;
        prp_fill_frame_info(prp, rx_frame, length - tag_len, swt_port[portid]);

        prp->prp_rx(prp, rx_frame);
        buffer_ring_free(&g_buffer_ring, rx_buffer);
    }

err:
    prp_destroy_device(prp);
}
