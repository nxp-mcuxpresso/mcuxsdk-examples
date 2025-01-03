/**
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "usb_device_config.h"
#include "usb_device.h"
#include "usb_device_class.h"
#include "usb_device_cdc_ecm.h"
#include "usb_device_descriptor.h"
#include "usb_eth_adapter.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern void BOARD_InitHardware(void);
extern void USB_DeviceClockInit(void);
extern void USB_DeviceIsrEnable(void);

extern usb_device_class_struct_t cdcEcmClass;

#if USB_DEVICE_CONFIG_USE_TASK
void USB_DeviceTaskFn(void *deviceHandle);
#endif

usb_status_t USB_DeviceCdcEcmCallback(class_handle_t handle, uint32_t event, void *param);
usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param);

/*******************************************************************************
 * Variables
 ******************************************************************************/
usb_eth_nic_t ethNicHandle;

usb_device_class_config_struct_t cdcEcmConfig[] = {
    {
        .classCallback = USB_DeviceCdcEcmCallback,
        .classInfomation = &cdcEcmClass,
    },
};

usb_device_class_config_list_struct_t cdcEcmConfigList = {
    .config = cdcEcmConfig,
    .count = ARRAY_SIZE(cdcEcmConfig),
    .deviceCallback = USB_DeviceCallback,
};

USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static uint8_t notify_req[sizeof(usb_setup_struct_t) + 8];

USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static uint8_t dataOutBuffer[APP_ETH_FRAME_MAX_LENGTH];

USB_DMA_NONINIT_DATA_ALIGN(USB_DATA_ALIGN_SIZE)
static uint8_t zlpBuffer;

uint32_t appEvent;

/*******************************************************************************
 * Code
 ******************************************************************************/
static void APP_Init(void)
{
    USB_DeviceClockInit();

    ethNicHandle.deviceHandle = NULL;
    ethNicHandle.cdcEcmHandle = NULL;
    ethNicHandle.ethHandle = &ethAdapterHandle;

    if (ETH_ADAPTER_Init() != ETH_ADAPTER_OK)
    {
        (void)usb_echo("ETH_ADAPTER_Init() occurs error.\r\n");
    }

    if (USB_DeviceClassInit(CONTROLLER_ID, &cdcEcmConfigList, &ethNicHandle.deviceHandle) != kStatus_USB_Success)
    {
        (void)usb_echo("USB_DeviceClassInit() occurs error.\r\n");
    }
    else
    {
        ethNicHandle.cdcEcmHandle = cdcEcmConfigList.config->classHandle;
        USB_FillStringDescriptorBuffer();
        (void)usb_echo("USB CDC-ECM NIC Device\r\n");
    }

    USB_DeviceIsrEnable();

    /* Add one delay here to make the DP pull down long enough to allow host to detect the previous disconnection. */
    SDK_DelayAtLeastUs(5000U, SDK_DEVICE_MAXIMUM_CPU_CLOCK_FREQUENCY);
    USB_DeviceRun(ethNicHandle.deviceHandle);
}

static void APP_EncapsulateUSBRequest(uint8_t *buffer, usb_setup_struct_t *setup, uint8_t *data, uint32_t length)
{
    uint8_t offset = sizeof(usb_setup_struct_t);

    if (buffer)
    {
        memcpy(buffer, setup, offset);
    }

    if (data)
    {
        memcpy(buffer + offset, data, length);
    }
}

static void APP_CheckLinkChange(void)
{
    bool link = false;
    (void)ETH_ADAPTER_GetLinkStatus(&link);
    if (ethNicHandle.linkStatus != link)
    {
        ethNicHandle.linkStatus = link;
        APP_ETH_NIC_EVENT_SET(appEvent, kAPP_NotifyNetworkChange);
    }
}

static void APP_NotifyLinkStatus(void)
{
    usb_setup_struct_t req;
    uint32_t speedMap[2];

    req.bmRequestType = USB_REQUEST_TYPE_DIR_IN | USB_REQUEST_TYPE_TYPE_CLASS | USB_REQUEST_TYPE_RECIPIENT_INTERFACE;
    req.bRequest = USB_DEVICE_CDC_NETWORK_CONNECTION;
    req.wValue = (uint16_t)ethNicHandle.linkStatus;
    req.wIndex = USB_DEVICE_CDC_ECM_COMM_INTERFACE_NUMBER + 1;
    req.wLength = 0;

    APP_EncapsulateUSBRequest(notify_req, &req, NULL, 0);
    while (USB_DeviceCdcEcmSend(ethNicHandle.cdcEcmHandle, USB_DEVICE_CDC_ECM_COMM_INTERRUPT_IN_EP_NUMBER, notify_req, 8) != kStatus_USB_Success)
    {
#if USB_DEVICE_CONFIG_USE_TASK
        USB_DeviceTaskFn(ethNicHandle.deviceHandle);
#endif
    }

    req.bmRequestType = USB_REQUEST_TYPE_DIR_IN | USB_REQUEST_TYPE_TYPE_CLASS | USB_REQUEST_TYPE_RECIPIENT_INTERFACE;
    req.bRequest = USB_DEVICE_CDC_CONNECTION_SPEED_CHANGE;
    req.wValue = 0;
    req.wIndex = USB_DEVICE_CDC_ECM_COMM_INTERFACE_NUMBER + 1;
    req.wLength = 8;

    if (!ethNicHandle.linkStatus || (ETH_ADAPTER_GetLinkSpeed(&ethNicHandle.linkSpeed) != ETH_ADAPTER_OK))
    {
        ethNicHandle.linkSpeed = 0;
    }

    speedMap[0] = ethNicHandle.linkSpeed;
    speedMap[1] = ethNicHandle.linkSpeed;

    APP_EncapsulateUSBRequest(notify_req, &req, (uint8_t *)speedMap, 8);
    while (USB_DeviceCdcEcmSend(ethNicHandle.cdcEcmHandle, USB_DEVICE_CDC_ECM_COMM_INTERRUPT_IN_EP_NUMBER, notify_req, 16) != kStatus_USB_Success)
    {
#if USB_DEVICE_CONFIG_USE_TASK
        USB_DeviceTaskFn(ethNicHandle.deviceHandle);
#endif
    }

    if (ethNicHandle.linkStatus)
    {
        APP_ETH_NIC_EVENT_SET(appEvent, kAPP_UsbDataInXfer);
        APP_ETH_NIC_EVENT_SET(appEvent, kAPP_UsbDataOutXfer);
    }
    else
    {
        APP_ETH_NIC_EVENT_UNSET(appEvent, kAPP_UsbDataInXfer);
        APP_ETH_NIC_EVENT_UNSET(appEvent, kAPP_UsbDataOutXfer);
    }
}

static void APP_TransferFrameUSBIn(void)
{
    (void)ETH_ADAPTER_RecvFrameQueue();

    if (ethNicHandle.ethHandle->rxFrameQueue.valid_len)
    {
        eth_adapter_frame_buf_t *buf = &ethNicHandle.ethHandle->rxFrameQueue.queue[ethNicHandle.ethHandle->rxFrameQueue.idx];
        eth_adapter_dst_frame_type_t type;
        uint8_t sendToHost = 0U;

        if (ETH_ADAPTER_IdentifyDstFrameType(buf, &type) != ETH_ADAPTER_OK)
        {
            ETH_ADAPTER_FrameQueuePop(&ethNicHandle.ethHandle->rxFrameQueue, NULL);
        }
        else
        {
            switch (type)
            {
                case ETH_ADAPTER_DST_FRAME_UNICAST:
                    if (ethNicHandle.unicastFramePass)
                    {
                        sendToHost = 1U;
                    }
                    break;

                case ETH_ADAPTER_DST_FRAME_MULTICAST:
                    if (ethNicHandle.multicastFramePass)
                    {
                        sendToHost = 1U;
                    }
                    break;

                case ETH_ADAPTER_DST_FRAME_BOARDCAST:
                    if (ethNicHandle.boardcastFramePass)
                    {
                        sendToHost = 1U;
                    }
                    break;

                default:
                    break;
            }

            if (sendToHost)
            {
                for (uint32_t frame_total_len = buf->len, sent_len = 0; frame_total_len > 0;)
                {
                    if (frame_total_len > USB_DEVICE_CDC_ECM_CLASS_DESCRIPTOR_MAX_SEGMENT_SIZE)
                    {
                        usb_status_t status = USB_DeviceCdcEcmSend(ethNicHandle.cdcEcmHandle, USB_DEVICE_CDC_ECM_DATA_BULK_IN_EP_NUMBER, &buf->payload[sent_len], USB_DEVICE_CDC_ECM_CLASS_DESCRIPTOR_MAX_SEGMENT_SIZE);
                        switch (status)
                        {
                            case kStatus_USB_Busy:
                                break;

                            case kStatus_USB_Success:
                                ETH_ADAPTER_FrameQueuePop(&ethNicHandle.ethHandle->rxFrameQueue, NULL);
                                sent_len += USB_DEVICE_CDC_ECM_CLASS_DESCRIPTOR_MAX_SEGMENT_SIZE;
                                frame_total_len -= USB_DEVICE_CDC_ECM_CLASS_DESCRIPTOR_MAX_SEGMENT_SIZE;
                                break;

                            default:
                                frame_total_len = 0;
                                break;
                        }

                        if (!ethNicHandle.attachStatus)
                        {
                            (void)USB_DeviceCancel(ethNicHandle.deviceHandle, USB_DEVICE_CDC_ECM_DATA_BULK_IN_EP_NUMBER | USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_IN);
                            break;
                        }
                    }
                    else
                    {
                        usb_status_t status = USB_DeviceCdcEcmSend(ethNicHandle.cdcEcmHandle, USB_DEVICE_CDC_ECM_DATA_BULK_IN_EP_NUMBER, &buf->payload[sent_len], frame_total_len);
                        switch (status)
                        {
                            case kStatus_USB_Busy:
                                break;

                            case kStatus_USB_Success:
                                ETH_ADAPTER_FrameQueuePop(&ethNicHandle.ethHandle->rxFrameQueue, NULL);
                                sent_len += frame_total_len;
                                frame_total_len = 0;
                                break;

                            default:
                                frame_total_len = 0;
                                break;
                        }

                        if (!ethNicHandle.attachStatus)
                        {
                            (void)USB_DeviceCancel(ethNicHandle.deviceHandle, USB_DEVICE_CDC_ECM_DATA_BULK_IN_EP_NUMBER | USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_IN);
                            break;
                        }
                    }

#if USB_DEVICE_CONFIG_USE_TASK
                    USB_DeviceTaskFn(ethNicHandle.deviceHandle);
#endif
                }

#if (defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)) || (defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
                if (!(buf->len % USB_DEVICE_CDC_ECM_DATA_BULK_IN_EP_MAXPKT_SIZE_HS))
#else
                if (!(buf->len % USB_DEVICE_CDC_ECM_DATA_BULK_IN_EP_MAXPKT_SIZE_FS))
#endif
                {
                    if (USB_DeviceCdcEcmSend(ethNicHandle.cdcEcmHandle, USB_DEVICE_CDC_ECM_DATA_BULK_IN_EP_NUMBER, &zlpBuffer, 0) != kStatus_USB_Success)
                    {
                        if (!ethNicHandle.attachStatus)
                        {
                            (void)USB_DeviceCancel(ethNicHandle.deviceHandle, USB_DEVICE_CDC_ECM_DATA_BULK_IN_EP_NUMBER | USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_IN);
                        }
                    }

#if USB_DEVICE_CONFIG_USE_TASK
                    USB_DeviceTaskFn(ethNicHandle.deviceHandle);
#endif
                }
            }
        }
    }
}

static void APP_TransferFrameUSBOut(void)
{
    (void)ETH_ADAPTER_SendFrameQueue();

    if (USB_DeviceCdcEcmRecv(ethNicHandle.cdcEcmHandle, USB_DEVICE_CDC_ECM_DATA_BULK_OUT_EP_NUMBER, dataOutBuffer, APP_ETH_FRAME_MAX_LENGTH) != kStatus_USB_Success)
    {
        if (!ethNicHandle.attachStatus)
        {
            (void)USB_DeviceCancel(ethNicHandle.deviceHandle, USB_DEVICE_CDC_ECM_DATA_BULK_OUT_EP_NUMBER | USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_OUT);
        }
    }

#if USB_DEVICE_CONFIG_USE_TASK
    USB_DeviceTaskFn(ethNicHandle.deviceHandle);
#endif
}

usb_status_t USB_DeviceCallback(usb_device_handle handle, uint32_t event, void *param)
{
    usb_status_t status = kStatus_USB_Error;

    switch (event)
    {
        case kUSB_DeviceEventBusReset:
#if (defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
#if !((defined FSL_FEATURE_SOC_USBPHY_COUNT) && (FSL_FEATURE_SOC_USBPHY_COUNT > 0U))
            /* The work-around is used to fix the HS device Chirping issue.
             * Please refer to the implementation for the detail information.
             */
            USB_DeviceHsPhyChirpIssueWorkaround();
#endif
#endif

#if (defined(USB_DEVICE_CONFIG_EHCI) && (USB_DEVICE_CONFIG_EHCI > 0U)) || \
    (defined(USB_DEVICE_CONFIG_LPCIP3511HS) && (USB_DEVICE_CONFIG_LPCIP3511HS > 0U))
            /* Get USB speed to configure the device, including max packet size and interval of the endpoints. */
            if (USB_DeviceClassGetSpeed(CONTROLLER_ID, &ethNicHandle.deviceSpeed) == kStatus_USB_Success)
            {
                USB_DeviceSetSpeed(handle, ethNicHandle.deviceSpeed);
            }
#endif

            if (ETH_ADAPTER_FrameQueueClear(&ethNicHandle.ethHandle->txFrameQueue) != ETH_ADAPTER_OK)
            {
                break;
            }

            if (ETH_ADAPTER_FrameQueueClear(&ethNicHandle.ethHandle->rxFrameQueue) != ETH_ADAPTER_OK)
            {
                break;
            }

            ethNicHandle.configuration = 0U;
            ethNicHandle.boardcastFramePass = 0U;
            ethNicHandle.multicastFramePass = 0U;
            ethNicHandle.unicastFramePass = 0U;
            ethNicHandle.attachStatus = 0U;
            ethNicHandle.linkStatus = 0U;

            APP_ETH_NIC_EVENT_CLEAR(appEvent);

            status = kStatus_USB_Success;
            break;

#if (defined(USB_DEVICE_CONFIG_DETACH_ENABLE) && (USB_DEVICE_CONFIG_DETACH_ENABLE > 0U))
        case kUSB_DeviceEventDetach:
            ethNicHandle.attachStatus = 0U;
            APP_ETH_NIC_EVENT_CLEAR(appEvent);
            status = kStatus_USB_Success;
            break;
#endif

        case kUSB_DeviceEventGetDeviceDescriptor:
            if (param)
            {
                status = USB_DeviceGetDeviceDescriptor(handle, (usb_device_get_device_descriptor_struct_t *)param);
            }
            break;

        case kUSB_DeviceEventGetConfigurationDescriptor:
            if (param)
            {
                status = USB_DeviceGetConfigurationDescriptor(handle, (usb_device_get_configuration_descriptor_struct_t *)param);
            }
            break;

        case kUSB_DeviceEventGetConfiguration:
            if (param)
            {
                *((uint8_t *)param) = ethNicHandle.configuration;

                status = kStatus_USB_Success;
            }
            break;

        case kUSB_DeviceEventSetConfiguration:
            ethNicHandle.configuration = *((uint8_t *)param);
            status = kStatus_USB_Success;
            break;

        case kUSB_DeviceEventGetInterface:
            if (param)
            {
                uint8_t interface = USB_SHORT_GET_HIGH(*((uint16_t *)param));
                if (interface < USB_DEVICE_CDC_ECM_INTERFACE_COUNT)
                {
                    *((uint16_t *)param) |= ethNicHandle.interfaceAltSetting[interface];

                    status = kStatus_USB_Success;
                }
            }
            break;

        case kUSB_DeviceEventSetInterface:
        {
            uint8_t interface = USB_SHORT_GET_HIGH(*((uint16_t *)param));
            uint8_t altSetting = (uint8_t)USB_SHORT_GET_LOW(*((uint16_t *)param));

            switch (interface)
            {
                case USB_DEVICE_CDC_ECM_COMM_INTERFACE_NUMBER:
                    if (altSetting < USB_DEVICE_CDC_ECM_COMM_INTERFACE_ALTERNATE_COUNT)
                    {
                        ethNicHandle.interfaceAltSetting[interface] = altSetting;
                        status = kStatus_USB_Success;
                    }
                    break;

                case USB_DEVICE_CDC_ECM_DATA_INTERFACE_NUMBER:
                    if (altSetting < USB_DEVICE_CDC_ECM_DATA_INTERFACE_ALTERNATE_COUNT)
                    {
                        ethNicHandle.interfaceAltSetting[interface] = altSetting;
                        status = kStatus_USB_Success;
                    }
                    break;

                default:
                    break;
            }
        }
        break;

        case kUSB_DeviceEventGetStringDescriptor:
            if (param)
            {
                /* Get device string descriptor request */
                status = USB_DeviceGetStringDescriptor(handle, (usb_device_get_string_descriptor_struct_t *)param);
            }
            break;

        default:
            status = kStatus_USB_InvalidRequest;
            break;
    }

    return status;
}

usb_status_t USB_DeviceCdcEcmCallback(usb_device_handle handle, uint32_t event, void *param)
{
    usb_status_t status = kStatus_USB_Success;
    usb_device_control_request_struct_t *request = (usb_device_control_request_struct_t *)param;
    usb_device_endpoint_callback_message_struct_t *epMsg = (usb_device_endpoint_callback_message_struct_t *)param;

    switch (event)
    {
        case kUSB_DeviceCdcEcmEventSendResponse:
            break;

        case kUSB_DeviceCdcEcmEventRecvResponse:
        {
            if (epMsg->length != USB_CANCELLED_TRANSFER_LENGTH)
            {
                eth_adapter_frame_buf_t frame;
                frame.len = epMsg->length;
                frame.payload = epMsg->buffer;
                (void)ETH_ADAPTER_FrameQueuePush(&ethNicHandle.ethHandle->txFrameQueue, &frame);
            }
        }
        break;

        case kUSB_DeviceCdcEcmEventNotifyResponse:
            break;

        case kUSB_DeviceCdcEcmEventSetEthernetPacketFilter:
            ethNicHandle.attachStatus = 1U;

            if (request->setup->wValue & USB_DEVICE_CDC_ECM_PACKET_TYPE_PROMISCUOUS_MASK)
            {
                ethNicHandle.boardcastFramePass = 1U;
                ethNicHandle.multicastFramePass = 1U;
                ethNicHandle.unicastFramePass = 1U;
            }
            else
            {
                if (request->setup->wValue & USB_DEVICE_CDC_ECM_PACKET_TYPE_ALL_MULTICAST_MASK)
                {
                    ethNicHandle.multicastFramePass = 1U;
                }
                else
                {
                    ethNicHandle.multicastFramePass = 0U;
                }

                if (request->setup->wValue & USB_DEVICE_CDC_ECM_PACKET_TYPE_DIRECTED_MASK)
                {
                    ethNicHandle.unicastFramePass = 1U;
                }
                else
                {
                    ethNicHandle.unicastFramePass = 0U;
                }

                if (request->setup->wValue & USB_DEVICE_CDC_ECM_PACKET_TYPE_BROADCAST_MASK)
                {
                    ethNicHandle.boardcastFramePass = 1U;
                }
                else
                {
                    ethNicHandle.boardcastFramePass = 0U;
                }
            }
            break;

        default:
            status = kStatus_USB_InvalidRequest;
            break;
    }

    return status;
}

#if defined(__CC_ARM) || (defined(__ARMCC_VERSION)) || defined(__GNUC__)
int main(void)
#else
void main(void)
#endif
{
    BOARD_InitHardware();
    APP_Init();

    while (1U)
    {
        if (ethNicHandle.attachStatus)
        {
            APP_CheckLinkChange();

            if (APP_ETH_NIC_EVENT_GET(appEvent, kAPP_NotifyNetworkChange))
            {
                APP_ETH_NIC_EVENT_UNSET(appEvent, kAPP_NotifyNetworkChange);
                APP_NotifyLinkStatus();
            }

            if (APP_ETH_NIC_EVENT_GET(appEvent, kAPP_UsbDataInXfer))
            {
                APP_TransferFrameUSBIn();
            }

            if (APP_ETH_NIC_EVENT_GET(appEvent, kAPP_UsbDataOutXfer))
            {
                APP_TransferFrameUSBOut();
            }
        }

#if USB_DEVICE_CONFIG_USE_TASK
        USB_DeviceTaskFn(ethNicHandle.deviceHandle);
#endif
    }
}
