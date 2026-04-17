/*
 * Copyright 2022-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */
#if CONFIG_NCP_UART
#include "ncp_intf_uart.h"
#include "fsl_flexcomm.h"
#include "fsl_usart.h"
#include "fsl_dma.h"
#include "fsl_pm_core.h"

NCP_LOG_MODULE_REGISTER(ncp_uart, CONFIG_LOG_NCP_INTF_LEVEL);

/*******************************************************************************
 * Defines
 ******************************************************************************/

#define NCP_UART_FRG_CLK \
    (&(const clock_frg_clk_config_t){0, kCLOCK_FrgMainClk, 255, 0}) /*!< Select FRG0 mux as frg_pll */
#define NCP_UART_CLK_ATTACH        kFRG_to_FLEXCOMM0
#define NCP_UART                   USART0
#define NCP_UART_CLK_FREQ          CLOCK_GetFlexCommClkFreq(0)
#define NCP_UART_IRQ               FLEXCOMM0_IRQn
#define NCP_UART_NVIC_PRIO         5U

#define NCP_UART_DMA               DMA0
#define NCP_UART_DMA_IRQ           DMA0_IRQn
#define NCP_UART_DMA_NVIC_PRIO     5U
#define NCP_UART_DMA_RX_CHANNEL    0U
#define NCP_UART_DMA_TX_CHANNEL    1U

/* Dynamic calculation based on max transfer size */
#define DMA_CALC_DESC_NUM(max_size) \
    (((max_size) + DMA_MAX_TRANSFER_COUNT - 1) / DMA_MAX_TRANSFER_COUNT)
/* DMA chain descriptor defines */
#define DMA_RX_CHAIN_DESC_NUM     DMA_CALC_DESC_NUM(TLV_CMD_BUF_SIZE) /* Number of RX chain descriptors */
#define DMA_TX_CHAIN_DESC_NUM     DMA_CALC_DESC_NUM(TLV_CMD_BUF_SIZE) /* Number of TX chain descriptors */

/*******************************************************************************
 * Variables
 ******************************************************************************/
static const ncp_pm_ops_t *s_pm_ops = NULL;

static pm_wakeup_source_t s_uart_wakeup_src;

/* DMA related handles */
static dma_handle_t s_dma_tx_handle;
static dma_handle_t s_dma_rx_handle;

/* DMA chain descriptors */
DMA_ALLOCATE_LINK_DESCRIPTORS_AT_NONCACHEABLE(s_rx_link_descriptors, DMA_RX_CHAIN_DESC_NUM);
DMA_ALLOCATE_LINK_DESCRIPTORS_AT_NONCACHEABLE(s_tx_link_descriptors, DMA_TX_CHAIN_DESC_NUM);

typedef struct {
    USART_Type *base;
    volatile OSA_EVENT_HANDLE_DEFINE(event);
} ncp_uart_ctx_t;

static ncp_uart_ctx_t s_uart_ctx = {0};

AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t rx_buffer[TLV_CMD_BUF_SIZE], 32);
static void ncp_uart_rx_task(void *argv);

static OSA_TASK_HANDLE_DEFINE(s_uart_task_handle);
static OSA_TASK_DEFINE(ncp_uart_rx_task, NCP_UART_TASK_PRIORITY, 1, NCP_UART_TASK_STACK_SIZE, 0);

static OSA_MUTEX_HANDLE_DEFINE(s_uart_mutex);

/* Those symbols should be exported by the Linker Script */
extern uint32_t __active_buf_bss_start __attribute__((weak));
extern uint32_t __active_buf_bss_end __attribute__((weak));
active_sram_bank_t pm2_active_sram_bank = {0};

/*******************************************************************************
 * API
 ******************************************************************************/

/* Setup TX DMA chain */
static void dma_setup_tx_chain(uint8_t *buffer, uint32_t size)
{
    uint32_t remaining  = size;
    uint32_t offset     = 0;
    uint32_t desc_index = 0;
    uint32_t chunk_size = 0;
    void *next_desc     = NULL;
    bool is_last        = true;

    while (remaining > 0 && desc_index < DMA_TX_CHAIN_DESC_NUM)
    {
        chunk_size = (remaining > DMA_MAX_TRANSFER_COUNT) ?
                            DMA_MAX_TRANSFER_COUNT : remaining;
        is_last = (remaining <= chunk_size) || (desc_index == DMA_TX_CHAIN_DESC_NUM - 1);

        next_desc = NULL;
        if (!is_last && desc_index < DMA_TX_CHAIN_DESC_NUM - 1)
        {
            next_desc = &s_tx_link_descriptors[desc_index + 1];
        }

        DMA_SetupDescriptor(
            &s_tx_link_descriptors[desc_index],
            DMA_CHANNEL_XFER(!is_last, false, is_last, false, 1, 1, 0, chunk_size),
            buffer + offset,
            (void *)&NCP_UART->FIFOWR,
            next_desc
        );

        remaining -= chunk_size;
        offset += chunk_size;
        desc_index++;
    }

    /* Load the first descriptor to start the chain */
    DMA_LoadChannelDescriptor(NCP_UART_DMA, NCP_UART_DMA_TX_CHANNEL, &s_tx_link_descriptors[0]);
    DMA_EnableChannelPeriphRq(NCP_UART_DMA, NCP_UART_DMA_TX_CHANNEL);
    USART_EnableTxDMA(NCP_UART, true);
}

/* Setup RX DMA chain */
static void dma_setup_rx_chain(uint8_t *buffer, uint32_t size, bool is_header)
{
    uint32_t remaining  = size;
    uint32_t offset     = 0;
    uint8_t desc_index  = 0;
    uint32_t chunk_size = 0;
    void *next_desc     = NULL;
    bool is_last        = true;
    bool use_intA       = false;
    bool use_intB       = false;

    while (remaining > 0 && desc_index < DMA_RX_CHAIN_DESC_NUM)
    {
        chunk_size = (remaining > DMA_MAX_TRANSFER_COUNT) ?
                                DMA_MAX_TRANSFER_COUNT : remaining;
        is_last = (remaining <= DMA_MAX_TRANSFER_COUNT) || (desc_index == DMA_RX_CHAIN_DESC_NUM - 1);
        next_desc = NULL;

        use_intA = is_header && is_last;
        use_intB = (!is_header) && is_last;
        if (!is_last && (desc_index < DMA_RX_CHAIN_DESC_NUM - 1))
        {
            next_desc = &s_rx_link_descriptors[desc_index + 1];
        }

        DMA_SetupDescriptor(
            &s_rx_link_descriptors[desc_index],
            DMA_CHANNEL_XFER(!is_last, false, use_intA, use_intB, 1, 0, 1, chunk_size),
            (void *)&NCP_UART->FIFORD,
            buffer + offset,
            next_desc
        );

        remaining -= chunk_size;
        offset += chunk_size;
        desc_index++;
    }

    DMA_SubmitChannelDescriptor(&s_dma_rx_handle, &s_rx_link_descriptors[0]);
    DMA_EnableChannelPeriphRq(NCP_UART_DMA, NCP_UART_DMA_RX_CHANNEL);
    USART_EnableRxDMA(NCP_UART, true);

    DMA_StartTransfer(&s_dma_rx_handle);
}

static void dma_tx_callback(dma_handle_t *handle, void *param, bool transferDone, uint32_t intmode)
{
    ncp_uart_ctx_t *chain = (ncp_uart_ctx_t *)param;

    /* Disable UART TX DMA. */
    USART_EnableTxDMA(NCP_UART, false);

    if (intmode == kDMA_IntError)
    {
        OSA_EventSet((osa_event_handle_t)chain->event, UART_EVENT_TX_ERROR);
    }
    else
    {
        /* Enable tx idle interrupt */
        chain->base->INTENSET |= USART_INTENSET_TXIDLEEN_MASK;
    }
}

static void dma_rx_callback(dma_handle_t *handle, void *param, bool transferDone, uint32_t intmode)
{
    ncp_uart_ctx_t *chain = (ncp_uart_ctx_t *)param;

    /* Disable UART RX DMA. */
    USART_EnableRxDMA(NCP_UART, false);

    if (intmode == kDMA_IntA)
    {
        OSA_EventSet((osa_event_handle_t)chain->event, UART_EVENT_RX_HEADER);
    }
    else if (intmode == kDMA_IntB)
    {
        OSA_EventSet((osa_event_handle_t)chain->event, UART_EVENT_RX_DONE);
    }
    else if (intmode == kDMA_IntError)
    {
        OSA_EventSet((osa_event_handle_t)chain->event, UART_EVENT_RX_ERROR);
    }
}

static void uart_transfer_handle_irq(void *base, void *handle)
{
    USART_Type *uart_base = (USART_Type *)base;
    ncp_uart_ctx_t *chain = (ncp_uart_ctx_t *)handle;

    if ((0U != (uart_base->INTENSET & USART_INTENSET_TXIDLEEN_MASK)) && (0U != (uart_base->INTSTAT & USART_INTSTAT_TXIDLE_MASK)))
    {
        /* Disable tx idle interrupt */
        uart_base->INTENCLR = USART_INTENCLR_TXIDLECLR_MASK;
        /* Tx idle interrupt happens means that all the tx data have been sent out to bus */
        OSA_EventSet((osa_event_handle_t)chain->event, UART_EVENT_TX_DONE);
    }

    if ((uart_base->STAT & USART_STAT_FRAMERRINT_MASK) != 0U)
    {
        /* Clear rx error state. */
        uart_base->STAT |= USART_STAT_FRAMERRINT_MASK;
        /* clear rxFIFO */
        uart_base->FIFOCFG |= USART_FIFOCFG_EMPTYRX_MASK;

        OSA_EventSet((osa_event_handle_t)chain->event, UART_EVENT_RX_FRAME_ERROR);
    }
}

static void uart_init_hw(void)
{
    int ret = (int)NCP_STATUS_SUCCESS;
    usart_config_t usartConfig;

    s_uart_ctx.base = NCP_UART;

    /* Attach FRG0 clock to FLEXCOMM0 */
    CLOCK_SetFRGClock(NCP_UART_FRG_CLK);
    CLOCK_AttachClk(NCP_UART_CLK_ATTACH);

    NVIC_SetPriority(NCP_UART_IRQ, NCP_UART_NVIC_PRIO);

    /* Get default USART config */
    USART_GetDefaultConfig(&usartConfig);
    usartConfig.baudRate_Bps = NCP_UART_BAUDRATE;
    usartConfig.enableTx = true;
    usartConfig.enableRx = true;
    usartConfig.enableHardwareFlowControl = true;
    usartConfig.txWatermark = kUSART_TxFifo0;
    usartConfig.rxWatermark = kUSART_RxFifo1;

    /* Initialize USART */
    ret = USART_Init(NCP_UART, &usartConfig, NCP_UART_CLK_FREQ);
    if (ret != kStatus_Success)
    {
        NCP_LOG_ERR("USART_Init failed!");
        return;
    }

    FLEXCOMM_SetIRQHandler(NCP_UART, uart_transfer_handle_irq, &s_uart_ctx);
    /* Enable NVIC IRQ. */
    (void)EnableIRQ(NCP_UART_IRQ);

    NCP_UART->INTENSET |= USART_INTENSET_FRAMERREN_MASK;
}

static void uart_init_dma(void)
{
    NVIC_SetPriority(NCP_UART_DMA_IRQ, NCP_UART_DMA_NVIC_PRIO);

    /* Initialize DMA */
    DMA_Init(NCP_UART_DMA);
    /* Create DMA handles */
    DMA_CreateHandle(&s_dma_tx_handle, NCP_UART_DMA, NCP_UART_DMA_TX_CHANNEL);
    DMA_CreateHandle(&s_dma_rx_handle, NCP_UART_DMA, NCP_UART_DMA_RX_CHANNEL);
    DMA_SetChannelPriority(NCP_UART_DMA, NCP_UART_DMA_TX_CHANNEL, kDMA_ChannelPriority3);
    DMA_SetChannelPriority(NCP_UART_DMA, NCP_UART_DMA_RX_CHANNEL, kDMA_ChannelPriority2);
    DMA_EnableChannel(NCP_UART_DMA, NCP_UART_DMA_TX_CHANNEL);
    DMA_EnableChannel(NCP_UART_DMA, NCP_UART_DMA_RX_CHANNEL);

    DMA_SetCallback(&s_dma_tx_handle, dma_tx_callback, &s_uart_ctx);
    DMA_SetCallback(&s_dma_rx_handle, dma_rx_callback, &s_uart_ctx);
}

static void uart_hardware_setup(void)
{
    uart_init_hw();
    uart_init_dma();
}

static int uart_enter_power_down(void)
{
    /* Abort any ongoing DMA transfers */
    DMA_AbortTransfer(&s_dma_rx_handle);
    DMA_AbortTransfer(&s_dma_tx_handle);
    /* Disable DMA channels */
    DMA_DisableChannel(NCP_UART_DMA, NCP_UART_DMA_TX_CHANNEL);
    DMA_DisableChannel(NCP_UART_DMA, NCP_UART_DMA_RX_CHANNEL);
    /* Deinit DMA */
    DMA_Deinit(NCP_UART_DMA);
    /* Deinit USART */
    USART_Deinit(NCP_UART);

    return (int)NCP_PM_STATUS_SUCCESS;
}

static int uart_exit_power_down(void)
{
    uart_hardware_setup();

    OSA_EventClear((osa_event_handle_t)s_uart_ctx.event, UART_EVENT_RX_MASK);

    dma_setup_rx_chain(rx_buffer, TLV_CMD_HEADER_LEN, true);

    return (int)NCP_PM_STATUS_SUCCESS;
}

static void uart_get_active_sram_bank(void)
{
#if !NCP_UART_IS_HIGH_BAUD
    uint32_t bit_count = 0;

    if (&__active_buf_bss_start == NULL || &__active_buf_bss_end == NULL)
    {
        return;
    }

    pm2_active_sram_bank.start = GET_BANK_NUM(&__active_buf_bss_start);
    pm2_active_sram_bank.end   = GET_BANK_NUM(&__active_buf_bss_end);

    if (pm2_active_sram_bank.end >= pm2_active_sram_bank.start)
    {
        bit_count = pm2_active_sram_bank.end - pm2_active_sram_bank.start + 1U;
        pm2_active_sram_bank.active_mask = 0x1U | (((1U << bit_count) - 1U) << pm2_active_sram_bank.start);
    }

    NCP_LOG_DBG("PM2 Active SRAM: Bank %u - Bank %u, Mask: 0x%x",
                pm2_active_sram_bank.start,
                pm2_active_sram_bank.end,
                pm2_active_sram_bank.active_mask);
#endif

    pm2_active_sram_bank.intf_flags = !NCP_UART_IS_HIGH_BAUD;
}

static int ncp_uart_init(void *argv)
{
    int ret = (int)NCP_STATUS_SUCCESS;
    (void)argv;

    uart_get_active_sram_bank();

    uart_hardware_setup();

    /* Create event group for DMA synchronization */
    ret = OSA_EventCreate((osa_event_handle_t)s_uart_ctx.event, true);
    if (ret != kStatus_Success)
    {
        NCP_LOG_ERR("Failed to create event group!");
        goto cleanup_hw;
    }

    /* Create mutex for thread safety */
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
    DMA_AbortTransfer(&s_dma_rx_handle);
    DMA_AbortTransfer(&s_dma_tx_handle);
    DMA_DisableChannel(NCP_UART_DMA, NCP_UART_DMA_TX_CHANNEL);
    DMA_DisableChannel(NCP_UART_DMA, NCP_UART_DMA_RX_CHANNEL);
    DMA_Deinit(NCP_UART_DMA);
    USART_Deinit(NCP_UART);

    return (int)NCP_STATUS_ERROR;
}

static int ncp_uart_deinit(void *argv)
{
    (void)argv;

    /* Abort any ongoing DMA transfers */
    DMA_AbortTransfer(&s_dma_rx_handle);
    DMA_AbortTransfer(&s_dma_tx_handle);

    (void)OSA_TaskDestroy((osa_task_handle_t)s_uart_task_handle);

    /* Destroy synchronization objects */
    OSA_EventDestroy((osa_event_handle_t)s_uart_ctx.event);
    OSA_MutexDestroy((osa_mutex_handle_t)s_uart_mutex);

    /* Disable DMA channels */
    DMA_DisableChannel(NCP_UART_DMA, NCP_UART_DMA_TX_CHANNEL);
    DMA_DisableChannel(NCP_UART_DMA, NCP_UART_DMA_RX_CHANNEL);
    /* Deinit DMA */
    DMA_Deinit(NCP_UART_DMA);

    /* Deinit USART */
    USART_Deinit(NCP_UART);

    return NCP_STATUS_SUCCESS;
}

static int ncp_uart_recv(uint8_t *tlv_buf, size_t *tlv_sz)
{
    int ret = (int)NCP_STATUS_SUCCESS;
    osa_event_flags_t flags;
    uint32_t cmd_len;
    uint32_t payload_len;

    NCP_ASSERT(NULL != tlv_buf);
    NCP_ASSERT(NULL != tlv_sz);

    *tlv_sz = 0;

    OSA_EventClear((osa_event_handle_t)s_uart_ctx.event, UART_EVENT_RX_MASK);

    dma_setup_rx_chain(tlv_buf, TLV_CMD_HEADER_LEN, true);

    OSA_EventWait((osa_event_handle_t)s_uart_ctx.event,
                                        UART_EVENT_RX_MASK,
                                        false,
                                        osaWaitForever_c,
                                        &flags);
    if (flags & UART_EVENT_RX_FRAME_ERROR)
    {
        NCP_LOG_DBG("UART RX frame error!");
        NCP_UART_STATS_INC(drop);
        ret = (int)NCP_STATUS_ERROR;
        goto exit;
    }

    if (!(flags & UART_EVENT_RX_HEADER))
    {
        NCP_LOG_ERR("Failed to receive RX header!");
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

    if (s_pm_ops && s_pm_ops->enter_critical)
    {
        s_pm_ops->enter_critical();
    }

    payload_len = cmd_len - TLV_CMD_HEADER_LEN + NCP_CHKSUM_LEN;
    dma_setup_rx_chain(tlv_buf + TLV_CMD_HEADER_LEN, payload_len, false);

    /* Wait for complete transfer */
    OSA_EventWait((osa_event_handle_t)s_uart_ctx.event,
                                        UART_EVENT_RX_MASK,
                                        false,
                                        osaWaitForever_c,
                                        &flags);
    if (flags & UART_EVENT_RX_DONE)
    {
        *tlv_sz = cmd_len;
        NCP_UART_STATS_INC(rx);
        NCP_LOG_DBG("Received %zu bytes", *tlv_sz);
        NCP_LOG_HEXDUMP_DBG(tlv_buf, *tlv_sz + NCP_CHKSUM_LEN);
        ret = (int)NCP_STATUS_SUCCESS;
    }
    else
    {
        NCP_LOG_ERR("UART RX DMA transfer failed!");
        NCP_UART_STATS_INC(drop);
        ret = (int)NCP_STATUS_ERROR;
    }

    if (s_pm_ops && s_pm_ops->exit_critical)
    {
        s_pm_ops->exit_critical();
    }

exit:
    DMA_AbortTransfer(&s_dma_rx_handle);
    USART_EnableRxDMA(NCP_UART, false);

    return ret;
}

static void ncp_uart_rx_task(void *argv)
{
    (void)argv;
    int ret;
    size_t tlv_size = 0;

    while (1)
    {
        ret = ncp_uart_recv(rx_buffer, &tlv_size);
        if (NCP_STATUS_SUCCESS == ret)
        {
            ncp_tlv_dispatch(rx_buffer, tlv_size);
        }
        else
        {
            NCP_LOG_DBG("Failed to receive TLV command!");
        }
    }
}

static int ncp_uart_send(uint8_t *tlv_buf, size_t tlv_sz, tlv_send_callback_t cb)
{
    (void)cb;
    int ret = (int)NCP_STATUS_SUCCESS;
    osa_event_flags_t flags;

    NCP_ASSERT(NULL != tlv_buf);

    /* Acquire mutex for thread safety */
    if (OSA_MutexLock((osa_mutex_handle_t)s_uart_mutex, osaWaitForever_c) != KOSA_StatusSuccess)
    {
        return NCP_STATUS_ERROR;
    }

    if (s_pm_ops && s_pm_ops->enter_critical)
    {
        s_pm_ops->enter_critical();
    }

    OSA_EventClear((osa_event_handle_t)s_uart_ctx.event, UART_EVENT_TX_MASK);

    NCP_LOG_DBG("Sending: %zu bytes", tlv_sz);
    NCP_LOG_HEXDUMP_DBG(tlv_buf, tlv_sz);

    /* Setup TX DMA chain */
    dma_setup_tx_chain(tlv_buf, tlv_sz);
    DMA_StartTransfer(&s_dma_tx_handle);

    /* Wait for TX completion or error*/
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
        USART_EnableTxDMA(NCP_UART, false);
        DMA_AbortTransfer(&s_dma_tx_handle);
        NCP_LOG_ERR("UART TX DMA transfer failed!");
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

static void ncp_uart_reset(void)
{
    uint32_t status;

    while (1)
    {
        status = USART_GetStatusFlags(NCP_UART);
        /* Check if the uart is transmitting */
        bool tx_idle = (status & kUSART_TxIdleFlag) != 0U;
        bool rx_idle = (((status & (kUSART_RxFifoNotEmptyFlag | kUSART_RxError)) == 0U) &&
                       ((status & kUSART_RxIdleFlag) != 0U));
        if (tx_idle && rx_idle)
        {
            break;
        }

        OSA_TimeDelay(1);
    }
}

static int ncp_uart_pm_init(void)
{
    s_pm_ops = ncp_pm_get_ops();

    if (s_pm_ops && s_pm_ops->init_wakeup_src)
    {
        s_pm_ops->init_wakeup_src(&s_uart_wakeup_src, (uint32_t)NCP_UART_DMA_IRQ, true);
    }

    return (int)NCP_PM_STATUS_SUCCESS;
}

static int ncp_uart_pm_prep(uint8_t pm_state, uint8_t event_type, void *data)
{
    (void)pm_state;
    (void)event_type;
    (void)data;

    return 0;
}

static int ncp_uart_pm_enter(uint8_t pm_state)
{
    switch (pm_state)
    {
        case NCP_PM_STATE_PM2:
#if !NCP_UART_IS_HIGH_BAUD
            /* Enable wakeup source */
            if (s_pm_ops && s_pm_ops->enable_wakeup_src)
            {
                s_pm_ops->enable_wakeup_src(&s_uart_wakeup_src);
            }
#endif
            break;
        case NCP_PM_STATE_PM3:
            uart_enter_power_down();
            break;
        default:
            break;
    }

    return NCP_PM_STATUS_SUCCESS;
}

static int ncp_uart_pm_exit(uint8_t pm_state)
{

    if (s_pm_ops && s_pm_ops->get_wakeup_src)
    {
        s_pm_ops->get_wakeup_src(&s_uart_wakeup_src);
    }

    switch (pm_state)
    {
        case NCP_PM_STATE_PM2:
#if !NCP_UART_IS_HIGH_BAUD
            if (s_pm_ops && s_pm_ops->disable_wakeup_src)
            {
                s_pm_ops->disable_wakeup_src(&s_uart_wakeup_src);
                (void)EnableIRQ(NCP_UART_DMA_IRQ);
            }
#endif
            break;
        case NCP_PM_STATE_PM3:
            BOARD_InitPins_NCP_UART();
            uart_exit_power_down();
            break;
        default:
            break;
    }

    return NCP_PM_STATUS_SUCCESS;
}

static ncp_intf_pm_ops_t ncp_uart_pm_ops =
{
    .init  = ncp_uart_pm_init,
    .prep  = ncp_uart_pm_prep,
    .enter = ncp_uart_pm_enter,
    .exit  = ncp_uart_pm_exit,
};

static ncp_intf_ops_t ncp_intf_ops =
{
    .init   = ncp_uart_init,
    .deinit = ncp_uart_deinit,
    .send   = ncp_uart_send,
    .recv   = ncp_uart_recv,
    .reset  = ncp_uart_reset,
    .pm_ops = &ncp_uart_pm_ops,
};

const ncp_intf_ops_t *ncp_intf_get_ops(void)
{
    return &ncp_intf_ops;
}
#endif /* CONFIG_NCP_UART */
