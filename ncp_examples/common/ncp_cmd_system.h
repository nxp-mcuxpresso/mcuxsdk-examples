/*!\file ncp_cmd_system.h
 *\brief This file provies power save commands for NCP system.
 */
/* Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __NCP_CMD_SYSTEM_H__
#define __NCP_CMD_SYSTEM_H__

#include "ncp_cmd_common.h"

/** System NCP subclass */
/** subclass type for system configure */
#define NCP_CMD_SYSTEM_CONFIG      0x00000000
/** subclass type for system test */
#define NCP_CMD_SYSTEM_TEST        0x00100000
/** subclass type for system power managerment */
#define NCP_CMD_SYSTEM_POWERMGMT   0x00200000
/** subclass type for system asynchronous event */
#define NCP_CMD_SYSTEM_ASYNC_EVENT 0x00300000

/** System Configure command */
/** Wi-Fi system set configuration command ID */
#define NCP_CMD_SYSTEM_CONFIG_SET  (NCP_CMD_SYSTEM | NCP_CMD_SYSTEM_CONFIG | NCP_MSG_TYPE_CMD | 0x00000001) /* set-device-cfg */
/** Wi-Fi system set configuration command response ID */
#define NCP_RSP_SYSTEM_CONFIG_SET  (NCP_CMD_SYSTEM | NCP_CMD_SYSTEM_CONFIG | NCP_MSG_TYPE_RESP | 0x00000001)
/** Wi-Fi system get configuration command ID */
#define NCP_CMD_SYSTEM_CONFIG_GET  (NCP_CMD_SYSTEM | NCP_CMD_SYSTEM_CONFIG | NCP_MSG_TYPE_CMD | 0x00000002) /* get-device-cfg */
/** Wi-Fi system get configuration command response ID */
#define NCP_RSP_SYSTEM_CONFIG_GET  (NCP_CMD_SYSTEM | NCP_CMD_SYSTEM_CONFIG | NCP_MSG_TYPE_RESP | 0x00000002)
/** system configuration encrypted communication command ID */
#define NCP_CMD_SYSTEM_CONFIG_ENCRYPT (NCP_CMD_SYSTEM | NCP_CMD_SYSTEM_CONFIG | NCP_MSG_TYPE_CMD | 0x00000003) /* ncp_encrypt */
/** system configuration encrypted communication command response ID */
#define NCP_RSP_SYSTEM_CONFIG_ENCRYPT (NCP_CMD_SYSTEM | NCP_CMD_SYSTEM_CONFIG | NCP_MSG_TYPE_RESP | 0x00000003)
/** system configuration host type communication command ID */
#define NCP_CMD_SYSTEM_HOST_TYPE     (NCP_CMD_SYSTEM | NCP_CMD_SYSTEM_CONFIG | NCP_MSG_TYPE_CMD | 0x00000004)
/** system configuration host type communication command response ID */
#define NCP_RSP_SYSTEM_HOST_TYPE     (NCP_CMD_SYSTEM | NCP_CMD_SYSTEM_CONFIG | NCP_MSG_TYPE_RESP | 0x00000004)
/** system configuration device reset command ID */
#define NCP_CMD_SYSTEM_CONFIG_DEVICE_RESET (NCP_CMD_SYSTEM | NCP_CMD_SYSTEM_CONFIG | NCP_MSG_TYPE_CMD | 0x00000005) /* ncp-dev-reset */
/** system configuration device reset command response ID */
#define NCP_RSP_SYSTEM_CONFIG_DEVICE_RESET (NCP_CMD_SYSTEM | NCP_CMD_SYSTEM_CONFIG | NCP_MSG_TYPE_RESP | 0x00000005)

/** Wi-Fi system power manager wakeup configuration command ID */
#define NCP_CMD_SYSTEM_POWERMGMT_WAKE_CFG (NCP_CMD_SYSTEM | NCP_CMD_SYSTEM_POWERMGMT | NCP_MSG_TYPE_CMD | 0x00000001) /* ncp-wake-cfg */
/** Wi-Fi system power manager wakeup configuration command response ID */
#define NCP_RSP_SYSTEM_POWERMGMT_WAKE_CFG (NCP_CMD_SYSTEM | NCP_CMD_SYSTEM_POWERMGMT | NCP_MSG_TYPE_RESP | 0x00000001)
/** Wi-Fi system MCU deep sleep power save mode command ID */
#define NCP_CMD_SYSTEM_POWERMGMT_MCU_SLEEP (NCP_CMD_SYSTEM | NCP_CMD_SYSTEM_POWERMGMT | NCP_MSG_TYPE_CMD | 0x00000002) /* ncp-mcu-sleep */
/** Wi-Fi system MCU deep sleep power save mode command response ID */
#define NCP_RSP_SYSTEM_POWERMGMT_MCU_SLEEP (NCP_CMD_SYSTEM | NCP_CMD_SYSTEM_POWERMGMT | NCP_MSG_TYPE_RESP | 0x00000002)
/** Wi-Fi system power manager wakeup host command ID */
#define NCP_CMD_SYSTEM_POWERMGMT_WAKEUP_HOST (NCP_CMD_SYSTEM | NCP_CMD_SYSTEM_POWERMGMT | NCP_MSG_TYPE_CMD | 0x00000003) /* ncp-wakeup-host */
/** Wi-Fi system power manager wakeup host command response ID */
#define NCP_RSP_SYSTEM_POWERMGMT_WAKEUP_HOST (NCP_CMD_SYSTEM | NCP_CMD_SYSTEM_POWERMGMT | NCP_MSG_TYPE_RESP | 0x00000003)

/* NCP system CRC check error event ID */
#define NCP_EVENT_CRC_CHECK_ERROR  (NCP_CMD_SYSTEM | NCP_CMD_SYSTEM_ASYNC_EVENT | NCP_MSG_TYPE_EVENT | 0x0000000f)
/** NCP host device encrypted communication event ID */
#define NCP_EVENT_SYSTEM_ENCRYPT       (NCP_CMD_SYSTEM | NCP_CMD_SYSTEM_CONFIG | NCP_MSG_TYPE_EVENT | 0x00000003)
/** NCP host device encrypted communication stop event ID */
#define NCP_EVENT_SYSTEM_ENCRYPT_STOP  (NCP_CMD_SYSTEM | NCP_CMD_SYSTEM_CONFIG | NCP_MSG_TYPE_EVENT | 0x00000004)
/** NCP system device reset event ID */
#define NCP_EVENT_SYSTEM_DEV_RESET  (NCP_CMD_SYSTEM | NCP_CMD_SYSTEM_CONFIG | NCP_MSG_TYPE_EVENT | 0x00000005)

/** Wi-Fi system test loopback command ID */
#define NCP_CMD_SYSTEM_TEST_LOOPBACK  (NCP_CMD_SYSTEM | NCP_CMD_SYSTEM_TEST | NCP_MSG_TYPE_CMD | 0x00000001) /* test-loopback */
#define NCP_RSP_SYSTEM_TEST_LOOPBACK  (NCP_CMD_SYSTEM | NCP_CMD_SYSTEM_TEST | NCP_MSG_TYPE_RESP | 0x00000001)

#define NCP_CMD_SYSTEM_INVALID_CMD    (NCP_CMD_SYSTEM | NCP_CMD_SYSTEM_CONFIG | NCP_MSG_TYPE_CMD | 0x0000000a) /* invalid command */
#define NCP_RSP_SYSTEM_INVALID_CMD    (NCP_CMD_SYSTEM | NCP_CMD_SYSTEM_CONFIG | NCP_MSG_TYPE_RESP | 0x0000000a)

#define MODULE_NAME_MAX_LEN 16
#define VAR_NAME_MAX_LEN  32
#define CONFIG_VALUE_MAX_LEN 256

#if CONFIG_NCP_USE_ENCRYPT
#define NCP_CMD_ENCRYPT_ACTION_INIT                 0
#define NCP_CMD_ENCRYPT_ACTION_DATA                 1
#define NCP_CMD_ENCRYPT_ACTION_VERIFY               2
#define NCP_CMD_ENCRYPT_ACTION_STOP                 3
#endif

/** NCP system configuration */
typedef NCP_TLV_PACK_START struct _NCP_CMD_SYSTEM_CFG
{
    /* the name of system config file: sys, prov, wlan */
    char module_name[MODULE_NAME_MAX_LEN];
    /* the name of entry */
    char variable_name[VAR_NAME_MAX_LEN];
    /* set value/returned result */
    char value[CONFIG_VALUE_MAX_LEN];
} NCP_TLV_PACK_END NCP_CMD_SYSTEM_CFG;

/** NCP device sleep configuration */
typedef NCP_TLV_PACK_START struct _NCP_CMD_POWERMGMT_MCU_SLEEP
{
    /** Enable flag:
     * 0 = Disabled
     * 1 = Enabled
     */
    uint8_t enable;
    /** Host power management mode:
     * 1 = PM1
     * 2 = PM2
     * 3 = PM3
     */
    uint8_t pm_mode;
    /** Duration to stay in the selected power management mode (in milliseconds) */
    uint32_t timeout;
} NCP_TLV_PACK_END NCP_CMD_POWERMGMT_MCU_SLEEP;

/** NCP device wakeup NCP host configuration. */
typedef NCP_TLV_PACK_START struct _NCP_CMD_POWERMGMT_WAKEUP_HOST
{
    /**
    0: disable,
    1: enable
    */
    uint8_t enable;
} NCP_TLV_PACK_END NCP_CMD_POWERMGMT_WAKEUP_HOST;

/** NCP host device encrypted communication . */
typedef NCP_TLV_PACK_START struct _NCP_CMD_ENCRYPT
{   
    /** 
    0: trigger encrypted communication flow
    1: send handshake data to NCP device
    2: verify the encryption communication
    */
    uint8_t action;
    /**
    checksum of keys and IVs when action is 2
    */
    uint32_t arg;
} NCP_TLV_PACK_END NCP_CMD_ENCRYPT;

/** This structure is used for region code configuration. */
typedef NCP_TLV_PACK_START struct _NCP_CMD_HOST_TYPE
{
    uint32_t host_type;
} NCP_TLV_PACK_END NCP_CMD_HOST_TYPE;

/** NCP system command */
typedef NCP_TLV_PACK_START struct _NCPCmd_DS_SYS_COMMAND
{
    /** Command Header : Command */
    NCP_COMMAND header;
    /** Command Body */
    union
    {
        /** System configuration */
        NCP_CMD_SYSTEM_CFG system_cfg;
        /** NCP device sleep configuration. */
        NCP_CMD_POWERMGMT_MCU_SLEEP mcu_sleep_config;
        /** Control for NCP device wakeup NCP host. */
        NCP_CMD_POWERMGMT_WAKEUP_HOST host_wakeup_ctrl;
        /** NCP host and device encrypted communication. */
        NCP_CMD_ENCRYPT encrypt;
        /** NCP host and device host type communication.*/
        NCP_CMD_HOST_TYPE host_type;
    } params;
} NCP_TLV_PACK_END NCPCmd_DS_SYS_COMMAND, MCU_NCPCmd_DS_SYS_COMMAND;

#endif /* __NCP_CMD_SYSTEM_H__ */
