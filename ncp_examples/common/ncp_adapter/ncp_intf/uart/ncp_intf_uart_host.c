/*
 * Copyright 2022-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */
#if CONFIG_NCP_UART
#include "ncp_intf_uart.h"
#include "fsl_gpio.h"
#include "fsl_lpuart.h"
#include "fsl_lpuart_edma.h"
#include "fsl_dmamux.h"
#include "fsl_edma.h"

NCP_LOG_MODULE_REGISTER(ncp_uart, CONFIG_LOG_NCP_INTF_LEVEL);

/*******************************************************************************
 * Defines
 ******************************************************************************/

extern uint32_t BOARD_DebugConsoleSrcFreq(void);
#define NCP_UART                     LPUART3
#define NCP_UART_CLK_FREQ            BOARD_DebugConsoleSrcFreq()
#define NCP_UART_IRQ                 LPUART3_IRQn
#define NCP_UART_NVIC_PRIO           5U

#define NCP_UART_DMA                 DMA0
#define NCP_UART_DMAMUX              DMAMUX
#define NCP_UART_DMA_CHN0_IRQ        DMA0_DMA16_IRQn
#define NCP_UART_DMA_CHN1_IRQ        DMA1_DMA17_IRQn
#define NCP_UART_DMA_NVIC_PRIO       5U
#define NCP_UART_DMA_TX_CHANNEL      0U
#define NCP_UART_DMA_RX_CHANNEL      1U
#define NCP_UART_DMA_TX_REQUEST      kDmaRequestMuxLPUART3Tx
#define NCP_UART_DMA_RX_REQUEST      kDmaRequestMuxLPUART3Rx

/*******************************************************************************
 * Variables
 ******************************************************************************/

static const ncp_pm_ops_t *s_pm_ops = NULL;

static lpuart_edma_handle_t s_uart_dma_handle;
static edma_handle_t s_dma_tx_handle;
static edma_handle_t s_dma_rx_handle;

typedef struct {
    LPUART_Type *base;
    volatile rx_state_t rx_state;
    volatile OSA_EVENT_HANDLE_DEFINE(event);
} ncp_uart_ctx_t;

static ncp_uart_ctx_t s_uart_ctx = {0};

AT_NONCACHEABLE_SECTION_ALIGN_INIT(static uint8_t rx_buffer[TLV_CMD_BUF_SIZE], 4);

static void ncp_uart_rx_task(void *argv);
static OSA_TASK_HANDLE_DEFINE(s_uart_task_handle);
static OSA_TASK_DEFINE(ncp_uart_rx_task, NCP_UART_TASK_PRIORITY, 1, NCP_UART_TASK_STACK_SIZE, 0);

OSA_MUTEX_HANDLE_DEFINE(s_uart_mutex);

/*******************************************************************************
 * Code
 ******************************************************************************/

void uart_transfer_callback(LPUART_Type *base, lpuart_edma_handle_t *handle, status_t status, void *userData)
{
    ncp_uart_ctx_t *ctx = (ncp_uart_ctx_t *)userData;

    if (kStatus_LPUART_TxIdle == status)
    {
        OSA_EventSet((osa_event_handle_t)ctx->event, UART_EVENT_TX_DONE);
    }

    if (kStatus_LPUART_RxIdle == status)
    {
        if (ctx->rx_state == RX_STATE_HEADER)
        {
            OSA_EventSet((osa_event_handle_t)ctx->event, UART_EVENT_RX_HEADER);
        }
        else if (ctx->rx_state == RX_STATE_PAYLOAD)
        {
            OSA_EventSet((osa_event_handle_t)ctx->event, UART_EVENT_RX_DONE);
        }
    }
}

static void uart_init_hw(void)
{
    lpuart_config_t lpuart_config;

    s_uart_ctx.base = NCP_UART;

    NVIC_SetPriority(NCP_UART_IRQ, NCP_UART_NVIC_PRIO);

    LPUART_GetDefaultConfig(&lpuart_config);
    lpuart_config.baudRate_Bps = NCP_UART_BAUDRATE;
    lpuart_config.enableTx = true;
    lpuart_config.enableRx = true;
    lpuart_config.enableRxRTS = true;
    lpuart_config.enableTxCTS = true;

    /* Initialize LPUART */
    if (LPUART_Init(NCP_UART, &lpuart_config, NCP_UART_CLK_FREQ) != kStatus_Success)
    {
        NCP_LOG_ERR("LPUART_Init failed!");
    }
}

static void uart_init_dma(void)
{
    edma_config_t edma_config;

    NVIC_SetPriority(NCP_UART_DMA_CHN0_IRQ, NCP_UART_DMA_NVIC_PRIO);
    NVIC_SetPriority(NCP_UART_DMA_CHN1_IRQ, NCP_UART_DMA_NVIC_PRIO);

    /* Initialize DMAMUX */
    DMAMUX_Init(NCP_UART_DMAMUX);
    DMAMUX_SetSource(NCP_UART_DMAMUX, NCP_UART_DMA_TX_CHANNEL, NCP_UART_DMA_TX_REQUEST);
    DMAMUX_SetSource(NCP_UART_DMAMUX, NCP_UART_DMA_RX_CHANNEL, NCP_UART_DMA_RX_REQUEST);
    DMAMUX_EnableChannel(NCP_UART_DMAMUX, NCP_UART_DMA_TX_CHANNEL);
    DMAMUX_EnableChannel(NCP_UART_DMAMUX, NCP_UART_DMA_RX_CHANNEL);

    /* Initialize eDMA */
    EDMA_GetDefaultConfig(&edma_config);
    EDMA_Init(NCP_UART_DMA, &edma_config);

    /* Create eDMA handles */
    EDMA_CreateHandle(&s_dma_tx_handle, NCP_UART_DMA, NCP_UART_DMA_TX_CHANNEL);
    EDMA_CreateHandle(&s_dma_rx_handle, NCP_UART_DMA, NCP_UART_DMA_RX_CHANNEL);

    LPUART_TransferCreateHandleEDMA(NCP_UART, &s_uart_dma_handle, uart_transfer_callback, (void *)&s_uart_ctx, &s_dma_tx_handle, &s_dma_rx_handle);
}

static void uart_hardware_setup(void)
{
    uart_init_hw();
    uart_init_dma();
}

static int ncp_uart_init(void *argv)
{
    int ret = (int)NCP_STATUS_SUCCESS;

    ARG_UNUSED(argv);

    memset(&s_uart_ctx, 0, sizeof(s_uart_ctx));
    s_uart_ctx.rx_state = RX_STATE_IDLE;

    uart_hardware_setup();

    /* Create event group for eDMA synchronization */
    ret = OSA_EventCreate((osa_event_handle_t)s_uart_ctx.event, true);
    if (ret != kStatus_Success)
    {
        NCP_LOG_ERR("Failed to create event group!");
        goto cleanup_hw;
    }

    ret = OSA_MutexCreate((osa_mutex_handle_t)s_uart_mutex);
    if (ret != kStatus_Success)
    {
        NCP_LOG_ERR("Failed to create uart mutex!");
        goto cleanup_event;
    }

    ret = OSA_TaskCreate((osa_task_handle_t)s_uart_task_handle, OSA_TASK(ncp_uart_rx_task), NULL);
    if (ret != kStatus_Success)
    {
        NCP_LOG_ERR("Failed to create uart RX task!");
        goto cleanup_mutex;
    }

    return (int)NCP_STATUS_SUCCESS;

cleanup_mutex:
    OSA_MutexDestroy((osa_mutex_handle_t)s_uart_mutex);
cleanup_event:
    OSA_EventDestroy((osa_event_handle_t)s_uart_ctx.event);
cleanup_hw:
    LPUART_TransferAbortReceiveEDMA(NCP_UART, &s_uart_dma_handle);
    LPUART_TransferAbortSendEDMA(NCP_UART, &s_uart_dma_handle);
    EDMA_Deinit(NCP_UART_DMA);
    DMAMUX_Deinit(NCP_UART_DMAMUX);
    LPUART_Deinit(NCP_UART);

    return (int)NCP_STATUS_ERROR;
}

static int ncp_uart_deinit(void *argv)
{
    ARG_UNUSED(argv);

    LPUART_TransferAbortReceiveEDMA(NCP_UART, &s_uart_dma_handle);
    LPUART_TransferAbortSendEDMA(NCP_UART, &s_uart_dma_handle);

    /* Destroy task */
    (void)OSA_TaskDestroy((osa_task_handle_t)s_uart_task_handle);

    OSA_EventDestroy((osa_event_handle_t)s_uart_ctx.event);
    OSA_MutexDestroy((osa_mutex_handle_t)s_uart_mutex);

    /* Deinitialize eDMA */
    EDMA_Deinit(NCP_UART_DMA);
    DMAMUX_Deinit(NCP_UART_DMAMUX);

    /* Deinitialize LPUART */
    LPUART_Deinit(NCP_UART);

    return (int)NCP_STATUS_SUCCESS;
}

static int ncp_uart_recv(uint8_t *tlv_buf, size_t *tlv_sz)
{
    int ret = (int)NCP_STATUS_SUCCESS;
    osa_event_flags_t flags;
    lpuart_transfer_t receiveXfer;
    uint32_t cmd_len;

    NCP_ASSERT(NULL != tlv_buf);
    NCP_ASSERT(NULL != tlv_sz);

    /* Clear RX events */
    OSA_EventClear((osa_event_handle_t)s_uart_ctx.event, UART_EVENT_RX_MASK);

    s_uart_ctx.rx_state = RX_STATE_HEADER;
    receiveXfer.data = tlv_buf;
    receiveXfer.dataSize = TLV_CMD_HEADER_LEN;

    if (kStatus_Success != LPUART_ReceiveEDMA(NCP_UART, &s_uart_dma_handle, &receiveXfer))
    {
        NCP_LOG_ERR("Failed to start eDMA receive for header!");
        ret = (int)NCP_STATUS_ERROR;
        goto exit;
    }

    OSA_EventWait((osa_event_handle_t)s_uart_ctx.event,
                                        UART_EVENT_RX_MASK,
                                        false,
                                        osaWaitForever_c,
                                        &flags);
    if (!(flags & UART_EVENT_RX_HEADER))
    {
        NCP_LOG_ERR("Failed to receive TLV header!");
        NCP_UART_STATS_INC(drop);
        ret = (int)NCP_STATUS_ERROR;
        goto exit;
    }

    NCP_LOG_DBG("Received TLV header");

    cmd_len = (tlv_buf[TLV_CMD_SIZE_HIGH_BYTES] << 8) | tlv_buf[TLV_CMD_SIZE_LOW_BYTES];
    if (cmd_len < TLV_CMD_HEADER_LEN || cmd_len > TLV_CMD_BUF_SIZE)
    {
        NCP_LOG_ERR("Invalid command length: %u", cmd_len);
        NCP_UART_STATS_INC(lenerr);
        NCP_UART_STATS_INC(drop);
        ret = (int)NCP_STATUS_ERROR;
        goto exit;
    }

    s_uart_ctx.rx_state = RX_STATE_PAYLOAD;
    receiveXfer.data = tlv_buf + TLV_CMD_HEADER_LEN;
    receiveXfer.dataSize = cmd_len - TLV_CMD_HEADER_LEN + NCP_CHKSUM_LEN;

    if (kStatus_Success != LPUART_ReceiveEDMA(NCP_UART, &s_uart_dma_handle, &receiveXfer))
    {
        NCP_LOG_ERR("Failed to start eDMA receive for payload!");
        ret = (int)NCP_STATUS_ERROR;
        goto exit;
    }

    OSA_EventWait((osa_event_handle_t)s_uart_ctx.event,
                                        UART_EVENT_RX_MASK,
                                        false,
                                        osaWaitForever_c,
                                        &flags);

    if (flags & UART_EVENT_RX_DONE)
    {
        s_uart_ctx.rx_state = RX_STATE_IDLE;
        *tlv_sz = cmd_len;
        NCP_UART_STATS_INC(rx);
        NCP_LOG_DBG("Received %zu bytes", *tlv_sz);
        NCP_LOG_HEXDUMP_DBG(tlv_buf, *tlv_sz + NCP_CHKSUM_LEN);
        ret = (int)NCP_STATUS_SUCCESS;
    }
    else
    {
        NCP_LOG_ERR("LPUART RX eDMA transfer failed!");
        NCP_UART_STATS_INC(drop);
        ret = (int)NCP_STATUS_ERROR;
    }

exit:
    s_uart_ctx.rx_state = RX_STATE_IDLE;
    LPUART_TransferAbortReceiveEDMA(NCP_UART, &s_uart_dma_handle);

    return ret;
}

static void ncp_uart_rx_task(void *argv)
{
    int ret;
    size_t tlv_size = 0;

    ARG_UNUSED(argv);

    while (1)
    {
        ret = ncp_uart_recv(rx_buffer, &tlv_size);
        if (NCP_STATUS_SUCCESS == ret)
        {
            ncp_tlv_dispatch(rx_buffer, tlv_size);
        }
        else
        {
            NCP_LOG_ERR("Failed to receive TLV command!");
        }
    }
}

static int ncp_uart_send(uint8_t *tlv_buf, size_t tlv_sz, tlv_send_callback_t cb)
{
    int ret = (int)NCP_STATUS_SUCCESS;
    osa_event_flags_t flags;
    lpuart_transfer_t sendXfer;

    ARG_UNUSED(cb);
    NCP_ASSERT(NULL != tlv_buf);

    if (OSA_MutexLock((osa_mutex_handle_t)s_uart_mutex, osaWaitForever_c) != KOSA_StatusSuccess)
    {
        return (int)NCP_STATUS_ERROR;
    }

    if (s_pm_ops && s_pm_ops->enter_critical)
    {
        s_pm_ops->enter_critical();
    }

    /* Clear TX events */
    OSA_EventClear((osa_event_handle_t)s_uart_ctx.event, UART_EVENT_TX_MASK);

    sendXfer.data = tlv_buf;
    sendXfer.dataSize = tlv_sz;
    NCP_LOG_DBG("Sending: %zu bytes", tlv_sz);
    NCP_LOG_HEXDUMP_DBG(tlv_buf, tlv_sz);

    if (kStatus_Success != LPUART_SendEDMA(NCP_UART, &s_uart_dma_handle, &sendXfer))
    {
        LPUART_TransferAbortSendEDMA(NCP_UART, &s_uart_dma_handle);
        NCP_LOG_ERR("Failed to start eDMA send!");
        ret = (int)NCP_STATUS_ERROR;
        goto exit;
    }

    /* Wait for TX completion or error */
    OSA_EventWait((osa_event_handle_t)s_uart_ctx.event,
                                        UART_EVENT_TX_MASK,
                                        false,
                                        osaWaitForever_c,
                                        &flags);
    if (flags & UART_EVENT_TX_DONE)
    {
        NCP_UART_STATS_INC(tx);
        NCP_LOG_DBG("Total sent: %zu bytes", tlv_sz);
        ret = (int)NCP_STATUS_SUCCESS;
    }
    else
    {
        NCP_LOG_ERR("LPUART TX eDMA transfer failed!");
        ret = (int)NCP_STATUS_ERROR;
        goto exit;
    }

exit:
    if (s_pm_ops && s_pm_ops->exit_critical)
    {
        s_pm_ops->exit_critical();
    }

    OSA_MutexUnlock((osa_mutex_handle_t)s_uart_mutex);

    return ret;
}

static int ncp_uart_pm_exit(uint8_t pm_state)
{
#if NCP_UART_IS_HIGH_BAUD
    GPIO_PinWrite(GPIO1, 27, 0);
    ncp_pm_delay_us(1000);
    GPIO_PinWrite(GPIO1, 27, 1);
    return NCP_PM_STATUS_SUCCESS;
#else
    ARG_UNUSED(pm_state);
    return NCP_PM_STATUS_SKIP;
#endif
}

static ncp_intf_pm_ops_t ncp_uart_pm_ops =
{
    .init  = NULL,
    .prep  = NULL,
    .enter = NULL,
    .exit  = ncp_uart_pm_exit,
};

static ncp_intf_ops_t ncp_intf_ops =
{
    .init   = ncp_uart_init,
    .deinit = ncp_uart_deinit,
    .send   = ncp_uart_send,
    .recv   = ncp_uart_recv,
    .reset  = NULL,
    .pm_ops = &ncp_uart_pm_ops,
    .set_host_type = NULL,
};

const ncp_intf_ops_t *ncp_intf_get_ops(void)
{
    return &ncp_intf_ops;
}
#endif /* CONFIG_NCP_UART */
