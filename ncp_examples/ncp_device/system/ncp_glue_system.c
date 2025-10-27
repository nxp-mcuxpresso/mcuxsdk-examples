/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */

#include "board.h"
#include "ncp_debug.h"
#include "ncp_config.h"
#include "ncp_cmd_system.h"
#include "ncp_glue_system.h"
#include "ncp_pm.h"
#include "fsl_pm_device.h"
#include "fsl_pm_core.h"
#include "app_notify.h"
#if CONFIG_NCP_WIFI
#include "wlan.h"
#endif
#if CONFIG_NCP_USE_ENCRYPT
#include "mbedtls_device.h"
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/
#if (CONFIG_NCP_WIFI && !CONFIG_NCP_BLE && !CONFIG_NCP_OT)
extern OSA_TIMER_HANDLE_DEFINE(wake_timer);
#endif
int current_PM_mode;

extern uint16_t g_cmd_seqno;
extern uint8_t cmd_buf[NCP_INBUF_SIZE];
uint8_t sys_res_buf[NCP_SYS_INBUF_SIZE];

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define HOST_SLEEP_DEF_WAKE_TIME 5000

/*******************************************************************************
 * Code
 ******************************************************************************/

NCPCmd_DS_SYS_COMMAND *ncp_sys_get_resp_buf()
{
    ncp_get_sys_resp_buf_lock();
    return (NCPCmd_DS_SYS_COMMAND *)(sys_res_buf);
}

/** Prepare TLV command response */
int ncp_sys_prepare_status(uint32_t cmd, uint16_t result)
{
    NCPCmd_DS_SYS_COMMAND *cmd_res = ncp_sys_get_resp_buf();
    cmd_res->header.cmd            = cmd;

    cmd_res->header.size     = NCP_CMD_HEADER_LEN;
    cmd_res->header.seqnum   = 0x00;
    cmd_res->header.result   = result;
    return NCP_SUCCESS;
}

uint8_t *ncp_sys_evt_status(uint32_t evt_id, void *msg)
{
    uint8_t *event_buf        = NULL;
    app_notify_msg_t *message = (app_notify_msg_t *)msg;
    int total_len             = 0;

    total_len = message->data_len + NCP_CMD_HEADER_LEN;
    event_buf = (uint8_t *)OSA_MemoryAllocate(total_len);
    if (event_buf == NULL)
    {
        ncp_e("failed to allocate memory for event");
        return NULL;
    }

    NCP_COMMAND *evt_hdr = (NCP_COMMAND *)event_buf;
    evt_hdr->cmd                = evt_id;
    evt_hdr->size               = total_len;
    evt_hdr->seqnum             = 0x00;
    evt_hdr->result             = message->reason;
    if (message->data_len)
        memcpy(event_buf + NCP_CMD_HEADER_LEN, message->data, message->data_len);

    return event_buf;
}

static int ncp_sys_set_config(void *tlv)
{
    NCP_CMD_SYSTEM_CFG *sys_cfg = (NCP_CMD_SYSTEM_CFG *)tlv;
    int ret                     = NCP_SUCCESS;
    const char *mod_name = NULL, *var_name = NULL, *value = NULL;

    if ((*sys_cfg->module_name == '\0') || (*sys_cfg->variable_name == '\0') || (*sys_cfg->value == '\0'))
    {
        ncp_e("invalid params");
        ret = -NCP_FAIL;
        goto done;
    }

    mod_name = sys_cfg->module_name;
    var_name = sys_cfg->variable_name;
    value    = sys_cfg->value;

    ret = ncp_set_conf(mod_name, var_name, value);

done:
    ncp_sys_prepare_status(NCP_RSP_SYSTEM_CONFIG_SET, ret);

    return ret;
}

static int ncp_sys_get_config(void *tlv)
{
    NCP_CMD_SYSTEM_CFG *sys_cfg = (NCP_CMD_SYSTEM_CFG *)tlv;
    int ret                     = NCP_SUCCESS;
    const char *mod_name, *var_name;
    char value[CONFIG_VALUE_MAX_LEN] = {0};

    if ((*sys_cfg->module_name == '\0') || (*sys_cfg->variable_name == '\0'))
    {
        ncp_e("invalid params");
        ncp_sys_prepare_status(NCP_RSP_SYSTEM_CONFIG_GET, NCP_CMD_RESULT_ERROR);
        return -NCP_FAIL;
    }

    mod_name = sys_cfg->module_name;
    var_name = sys_cfg->variable_name;

    ret = ncp_get_conf(mod_name, var_name, value, sizeof(value));
    if (ret != WM_SUCCESS)
    {
        ncp_sys_prepare_status(NCP_RSP_SYSTEM_CONFIG_GET, NCP_CMD_RESULT_ERROR);
        return ret;
    }

    NCPCmd_DS_SYS_COMMAND *cmd_res = ncp_sys_get_resp_buf();
    cmd_res->header.cmd            = NCP_RSP_SYSTEM_CONFIG_GET;
    cmd_res->header.size           = NCP_CMD_HEADER_LEN;
    cmd_res->header.seqnum         = 0x00;
    cmd_res->header.result         = NCP_CMD_RESULT_OK;

    NCP_CMD_SYSTEM_CFG *sys_cfg_res = (NCP_CMD_SYSTEM_CFG *)&cmd_res->params.system_cfg;
    (void)memcpy(sys_cfg_res->module_name, sys_cfg->module_name, sizeof(sys_cfg_res->module_name));
    (void)memcpy(sys_cfg_res->variable_name, sys_cfg->variable_name, sizeof(sys_cfg_res->variable_name));
    (void)memcpy(sys_cfg_res->value, value, sizeof(sys_cfg_res->value));

    cmd_res->header.size += sizeof(NCP_CMD_SYSTEM_CFG);

    return ret;
}

static int ncp_sys_mcu_sleep(void *tlv)
{
    NCP_CMD_POWERMGMT_MCU_SLEEP *mcu_sleep_config = (NCP_CMD_POWERMGMT_MCU_SLEEP *)tlv;
    ncp_pm_cfg_t *power_cfg = ncp_pm_get_config();
    int ret = 0;

    power_cfg->enable = mcu_sleep_config->enable;
    /* MCU sleep is disabled */
    if (power_cfg->enable == 0)
    {
        memset(power_cfg, 0x0, sizeof(ncp_pm_cfg_t));
#if CONFIG_NCP_WIFI
        wlan_cancel_host_sleep();
        wlan_clear_host_sleep_config();
#endif
        /* Release current constraint of Power Manager */
        ncp_pm_configure_next_lowpower_mode(0, 0);
        goto out;
    }
    else
    {
        power_cfg->pm_mode = mcu_sleep_config->pm_mode;
#if CONFIG_NCP_USB
        if (strcmp(BOARD_NAME, "FRDM-RW612") == 0)
        {
            if (power_cfg->pm_mode == 2)
            {
                ncp_e("PM2 mode is not supported with FRDMRW612 board using USB interface");
                ncp_e("USB device enter/exit PM2 depends on signal from USB host, and no aviable AON GPIO on FRDM board for PM3");
                ncp_e("So remove PM2 mode for NCP device USB interface");
                ncp_e("Please choose another power mode");
                ret = -WM_FAIL;
                goto out;
            }
        }
#endif
        ncp_pm_configure_next_lowpower_mode(power_cfg->pm_mode, mcu_sleep_config->timeout);
        power_cfg->timeout = mcu_sleep_config->timeout;
    }
#if CONFIG_NCP_WIFI
    wlan_config_host_sleep(0, 1);
#endif
out:
    if (ret)
        ncp_sys_prepare_status(NCP_RSP_SYSTEM_POWERMGMT_MCU_SLEEP, NCP_CMD_RESULT_ERROR);
    else
        ncp_sys_prepare_status(NCP_RSP_SYSTEM_POWERMGMT_MCU_SLEEP, NCP_CMD_RESULT_OK);

    return WM_SUCCESS;
}

static int ncp_sys_wakeup_host(void *tlv)
{
    NCP_CMD_POWERMGMT_WAKEUP_HOST *wake_host_ctrl = (NCP_CMD_POWERMGMT_WAKEUP_HOST *)tlv;
    ncp_pm_cfg_t *power_cfg                       = ncp_pm_get_config();

    power_cfg->wakeup_host = wake_host_ctrl->enable;
    ncp_sys_prepare_status(NCP_RSP_SYSTEM_POWERMGMT_WAKEUP_HOST, NCP_CMD_RESULT_OK);

    return WM_SUCCESS;
}


struct cmd_t system_cmd_config[] = {
    {NCP_CMD_SYSTEM_CONFIG_SET, "ncp-set", ncp_sys_set_config, CMD_SYNC},
    {NCP_CMD_SYSTEM_CONFIG_GET, "ncp-get", ncp_sys_get_config, CMD_SYNC},
#if CONFIG_NCP_USE_ENCRYPT
    {NCP_CMD_SYSTEM_CONFIG_ENCRYPT, "ncp-encrypt", ncp_sys_encrypt, CMD_SYNC},
#endif
    {NCP_CMD_INVALID, NULL, NULL, NULL},
};

struct cmd_t system_cmd_powermgmt[] = {
    {NCP_CMD_SYSTEM_POWERMGMT_MCU_SLEEP, "ncp-mcu-sleep", ncp_sys_mcu_sleep, CMD_SYNC},
    {NCP_CMD_SYSTEM_POWERMGMT_WAKEUP_HOST, "ncp-wakeup-host", ncp_sys_wakeup_host, CMD_SYNC},
    {NCP_CMD_INVALID, NULL, NULL, NULL},
};

struct cmd_subclass_t cmd_subclass_system[] = {
    {NCP_CMD_SYSTEM_CONFIG, system_cmd_config},
    {NCP_CMD_SYSTEM_POWERMGMT, system_cmd_powermgmt},
    {NCP_CMD_INVALID, NULL},
};
