/** @file ncp_intf_usb_device_cdc.c
 *
 *  @brief main file
 *
 *  Copyright 2024 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *  The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */
#include "fsl_pm_core.h"
#include "usb_misc.h"
#include "ncp_intf_usb_device_cdc.h"
#include "ncp_debug.h"
#include "ncp_tlv_adapter.h"
#include "ncp_adapter.h"
#include "ncp_pm.h"

#if CONFIG_NCP_USB
extern usb_cdc_vcom_struct_t s_cdcVcom;
extern USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE) uint8_t s_currRecvBuf[DATA_BUFF_SIZE];
OSA_SEMAPHORE_HANDLE_DEFINE(usb_device_tx_sem);

#if (CONFIG_NCP_DEBUG)
#define NCP_USB_STATS_INC(x) NCP_STATS_INC(intf.x)
#else
#define NCP_USB_STATS_INC(x)
#endif

static const ncp_pm_ops_t *s_pm_ops = NULL;
static pm_wakeup_source_t usbWakeupSource;

void ncp_usb_put_tx_sem(void)
{
    OSA_SemaphorePost(usb_device_tx_sem);
}

int ncp_usb_device_recv(uint8_t *recv_data, uint32_t packet_len)
{
    static int usb_rx_len = 0;
    static uint16_t usb_transfer_len = 0;

    if (usb_rx_len < TLV_CMD_HEADER_LEN)
    {
        usb_rx_len += packet_len;
        if(usb_rx_len >= TLV_CMD_HEADER_LEN)
        {
            usb_transfer_len = ((recv_data[TLV_CMD_SIZE_HIGH_BYTES] << 8) | recv_data[TLV_CMD_SIZE_LOW_BYTES]) + NCP_CHKSUM_LEN;
        }
        else
        {
            ncp_adap_d("[%s] packet too short: %d", __func__, packet_len);
        }
    }
    else
    {
        if ((packet_len <= usb_transfer_len - usb_rx_len) && (usb_rx_len <= usb_transfer_len))
        {
            ncp_adap_d("[%s] incomplete packet. expected: %d, got: %d", __func__, usb_transfer_len, packet_len);
            usb_rx_len = usb_rx_len + packet_len;
        }
        else
        {
            ncp_adap_e("[%s] transfer warning. data_len : %d  ", __func__, packet_len);
            NCP_USB_STATS_INC(err);
            usb_rx_len = 0;
            usb_transfer_len = 0;
            return 0;
        }
    }

    if (usb_rx_len >= usb_transfer_len && (usb_transfer_len >= TLV_CMD_HEADER_LEN))
    {
        ncp_adap_d("recv data len: %d", packet_len);
        ncp_tlv_dispatch(&s_currRecvBuf[0], usb_transfer_len - NCP_CHKSUM_LEN);
        usb_rx_len = 0;
        usb_transfer_len = 0;
        NCP_USB_STATS_INC(rx);
        ncp_adap_d("usb data recv success");
    }

    return usb_rx_len;
}

static int ncp_usb_device_send(uint8_t *data, size_t data_len, tlv_send_callback_t cb)
{
    uint16_t packet_size        = 0;
    uint16_t remaining_data_len = data_len;
    int lpm_usb_retry_cnt = 20;
    ARG_UNUSED(cb);

    ncp_adap_d("usb transfer_size :%d!\r\n", data_len);

    while (s_cdcVcom.suspend != kStatus_Idle)
    {
      OSA_TimeDelay(1);
    }

    if (s_pm_ops && s_pm_ops->enter_critical)
    {
        s_pm_ops->enter_critical();
    }

    /* Wait for USB re-init done */
    lpm_usb_retry_cnt = 200;
    while(lpm_usb_retry_cnt > 0 && 1 != s_cdcVcom.attach)
    {
        OSA_TimeDelay(50);
        lpm_usb_retry_cnt--;
    }

    if(0 == lpm_usb_retry_cnt)
    {
        ncp_adap_e("usb enum failed from LPM");
    }

    while (remaining_data_len > 0)
    {
        packet_size = (remaining_data_len > TLV_CMD_BUF_SIZE) ? TLV_CMD_BUF_SIZE : remaining_data_len;
		/*Max packet size is 16k*/
        packet_size = (packet_size > 0x4000) ? 0x4000 : packet_size;

        USB_DeviceCdcAcmSend(s_cdcVcom.cdcAcmHandle, USB_CDC_VCOM_BULK_IN_ENDPOINT,
                             (uint8_t *)data + data_len - remaining_data_len, packet_size);

        OSA_SemaphoreWait(usb_device_tx_sem, osaWaitForever_c);

        remaining_data_len -= packet_size;
    }

    if (s_pm_ops && s_pm_ops->exit_critical)
    {
        s_pm_ops->exit_critical();
    }

    NCP_USB_STATS_INC(tx);

    return 0;
}

static int ncp_usb_device_init(void* argv)
{
    int ret = NCP_STATUS_SUCCESS;
    ARG_UNUSED(argv);

    ret = OSA_SemaphoreCreateBinary(usb_device_tx_sem);
    if (ret != kStatus_Success)
    {
        ncp_adap_e("Create usb device tx sem failed");
        return ret;
    }

    ret = usb_device_init();

    return ret;
}

static int ncp_usb_device_deinit(void* argv)
{
    ARG_UNUSED(argv);
    int ret = NCP_STATUS_SUCCESS;

    ret = OSA_SemaphoreDestroy(usb_device_tx_sem);
    if (ret != NCP_STATUS_SUCCESS)
    {
        ncp_adap_e("failed to destroy usb_device_tx_sem: %d", ret);
        return NCP_STATUS_ERROR;
    }

    return usb_device_deinit();
}

static int ncp_usb_device_pm_init(void)
{
    s_pm_ops = ncp_pm_get_ops();

    if (s_pm_ops && s_pm_ops->init_wakeup_src)
    {
        s_pm_ops->init_wakeup_src(&usbWakeupSource, (uint32_t)USB_IRQn, true);
    }

    return (int)NCP_PM_STATUS_SUCCESS;
}

static int ncp_usb_device_pm_prep(uint8_t pm_state, uint8_t event_type, void *data)
{
    ARG_UNUSED(event_type);
    ARG_UNUSED(data);

    if(pm_state == NCP_PM_STATE_PM2)
    {
        return NCP_STATUS_ERROR;
    }

    return 0;
}

static int ncp_usb_device_pm_enter(uint8_t pm_state)
{
    int ret = 0;

    if(pm_state == NCP_PM_STATE_PM3)
    {
        ret = USB_DeviceEnterPowerDown();
        if(ret != 0)
        {
            ncp_adap_e("Failed to deinit USB interface");
            return NCP_STATUS_ERROR;
        }
    }

    return NCP_STATUS_SUCCESS;
}

static int ncp_usb_device_pm_exit(uint8_t pm_state)
{
    int ret = 0;

    if (s_pm_ops && s_pm_ops->get_wakeup_src)
    {
        s_pm_ops->get_wakeup_src(&usbWakeupSource);
    }

    if(pm_state == NCP_PM_STATE_PM3)
    {
        ret = USB_DeviceExitPowerDown();
        if(ret != 0)
        {
            ncp_adap_e("Failed to init USB interface");
            return NCP_STATUS_ERROR;
        }
    }

    return NCP_STATUS_SUCCESS;
}

static ncp_intf_pm_ops_t ncp_usb_device_pm_ops =
{
    .init  = ncp_usb_device_pm_init,
    .prep  = ncp_usb_device_pm_prep,
    .enter = ncp_usb_device_pm_enter,
    .exit  = ncp_usb_device_pm_exit,
};


static ncp_intf_ops_t ncp_intf_ops =
{
    .init   = ncp_usb_device_init,
    .deinit = ncp_usb_device_deinit,
    .send   = ncp_usb_device_send,
    .recv   = NULL,
    .pm_ops = &ncp_usb_device_pm_ops,
    .set_host_type = NULL,
};

const ncp_intf_ops_t *ncp_intf_get_ops(void)
{
    return &ncp_intf_ops;
}
#endif
