/** @file mbedtls_device.c
 *
 *  @brief This file provides NCP device mbedtls interfaces.
 *
 *  Copyright 2024 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "board.h"
#include "ncp_debug.h"
#include "ncp_config.h"
#include "ncp_cmd_system.h"
#include "ncp_glue_system.h"
#include "ncp_system.h"
#include "app_notify.h"
#include "fsl_rtc.h"
#include "ncp_tlv_adapter.h"
#include "wifi-decl.h"
#include "wlan.h"
#include "ncp_cmd_wifi.h"

#if CONFIG_NCP_USE_ENCRYPT

#include "mbedtls_common.h"

#define CONFIG_SYS_DATE_TIME        { 2025, 12, 31, 1, 1, 1 }

static void ncp_encrypt_handshake_task(void *pvParameters);
#define NCP_HOST_ENCRYPT_HANDSHAKE_TASK_PRIO   1
static OSA_TASK_HANDLE_DEFINE(ncp_encrypt_handshake_thread);
static OSA_TASK_DEFINE(ncp_encrypt_handshake_task, NCP_HOST_ENCRYPT_HANDSHAKE_TASK_PRIO, 1, 12288, 0);

static int ncp_send_event(uint32_t evt, uint8_t *buf, size_t len)
{
    int ret = 0;
    app_notify_msg_t msg = { 0, 0, len, (void*)buf };
    uint8_t *event_buf = ncp_sys_evt_status(evt, &msg);
    if (!event_buf)
    {
        ncp_e("ncp_sys_evt_status err");
        return -NCP_FAIL;
    }

    ret = system_ncp_send_response(event_buf);
    if (ret != 0)
    {
        ncp_e("system_ncp_send_response err %d", ret);
        return -NCP_FAIL;
    }

    if (event_buf)
    {
        (void) OSA_MemoryFree(event_buf);
        event_buf = NULL;
    }
    
    return len;
}

int port_mbedtls_send(void *ctx, const unsigned char *buf, size_t len)
{
    return ncp_send_event(NCP_EVENT_SYSTEM_ENCRYPT, (uint8_t*)buf, len);
}

static void ncp_encrypt_handshake_task(void *pvParameters)
{
    int ret = 0;
    NCP_ASSERT(_mbedtls);

    ncp_d("**** encrypt task called\r\n");

    ncp_d("**** next call mbedtls_ssl_handshake\r\n");
    ret = mbedtls_ssl_handshake(&_mbedtls->ssl);
    if (ret != 0)
    {
        ncp_e("mbedtls mbedtls_ssl_handshake fail %d", ret);
        ncp_e("%s", mbedtls_high_level_strerr(ret));
    }
    else 
    {
        ncp_d("**** mbedtls_ssl_handshake succ\r\n");

        ret = ncp_tlv_adapter_encrypt_enable();
        if (ret != 0)
        {
            ncp_e("ncp_tlv_adapter_encrypt_enable err %d", ret);
        }
    }

    (void) ncp_encrypt_teardown();

    ncp_d("**** encrypt init task exit %d\r\n", ret);
    (void) OSA_TaskDestroy(ncp_encrypt_handshake_thread);
}

int ncp_sys_encrypt(void *tlv)
{
    NCP_CMD_ENCRYPT *sys_encrypt = (NCP_CMD_ENCRYPT *)tlv;
    int ret                     = NCP_SUCCESS;

    NCPCmd_DS_SYS_COMMAND *cmd_res = ncp_sys_get_resp_buf();
    cmd_res->header.cmd            = NCP_RSP_SYSTEM_CONFIG_ENCRYPT;
    cmd_res->header.size           = NCP_CMD_HEADER_LEN;
    cmd_res->header.seqnum         = 0x00;
    cmd_res->header.result         = NCP_CMD_RESULT_OK;

    cmd_res->params.encrypt.action = sys_encrypt->action;
    cmd_res->header.size += sizeof(NCP_CMD_ENCRYPT);

    ncp_d("%s action %u", __FUNCTION__, sys_encrypt->action);
    switch (sys_encrypt->action)
    {
    case NCP_CMD_ENCRYPT_ACTION_INIT:
        {
            rtc_datetime_t rtc_date_time = CONFIG_SYS_DATE_TIME;
            ret = RTC_SetDatetime(RTC, &rtc_date_time);
            ret = ncp_encrypt_setup(NCP_TLS_ROLE_SERVER);
            if (ret == 0)
            {
                ret = (int)OSA_TaskCreate((osa_task_handle_t)ncp_encrypt_handshake_thread, 
                            OSA_TASK(ncp_encrypt_handshake_task), (osa_task_param_t)NULL);
            }

            if (ret != 0)
            {
                cmd_res->header.result = NCP_CMD_RESULT_ERROR;
                (void) ncp_encrypt_teardown();
            }
        }
        break;
    case NCP_CMD_ENCRYPT_ACTION_DATA:
        {
            uint8_t *data = (uint8_t*)&sys_encrypt[1];
            NCPCmd_DS_SYS_COMMAND *cmd = container_of(tlv, NCPCmd_DS_SYS_COMMAND, params);
            uint16_t len = cmd->header.size - NCP_CMD_HEADER_LEN - sizeof(NCP_CMD_ENCRYPT);
            (void) ncp_encrypt_process_handshake_data(data, len);
        }
        break;
    case NCP_CMD_ENCRYPT_ACTION_VERIFY:
        {
            if (_verify_num != sys_encrypt->arg)
            {
                cmd_res->header.result = NCP_CMD_RESULT_ERROR;
                ncp_e("verify num not equal 0x%X 0x%X", _verify_num, sys_encrypt->arg);
            }
            cmd_res->params.encrypt.arg = _verify_num;
            (void) PRINTF("ncp encrypt action verify, %s\r\n", 
                    _verify_num == sys_encrypt->arg ? "succ" : "fail");
        }
        break;
    case NCP_CMD_ENCRYPT_ACTION_STOP:
        {
            (void) PRINTF("NCP stop encrypted communication\r\n");
            ncp_tlv_adapter_encrypt_deinit();
            ret = ncp_send_event(NCP_EVENT_SYSTEM_ENCRYPT_STOP, NULL, 0);
            if (ret >= 0)
            {
                /* let the event be processed by host then send out the cmd response */
                OSA_TimeDelay(20);
            }
            else
            {
                ncp_e("ncp_send_event err %d", ret);
            }
        }
        break;
    default:
        ncp_w("NCP encrypt unkonwn action %d", sys_encrypt->action);
        break;
    }

    return ret;
}

#endif /* CONFIG_NCP_USE_ENCRYPT */
