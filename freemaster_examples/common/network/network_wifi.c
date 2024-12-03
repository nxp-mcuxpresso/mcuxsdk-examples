/*
 * Copyright 2021-2024  NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * FreeMASTER Communication Driver - Network Example Application Code
 */

#include <string.h>
#include <stdio.h>
#include "pin_mux.h"
#include "board.h"

#include "wlan_bt_fw.h"
#include "wlan.h"
#include "wifi.h"
#include "wm_net.h"

#include "lwip/dhcp.h"
#include "lwip/prot/dhcp.h"
#include "lwip/opt.h"
#include "lwip/netifapi.h"

#include "freemaster.h"
#include "network_wifi.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

// Hardwired SSID, passphrase of AP to connect to
// Change this to fit your AP
#ifndef FMSTR_DEMO_SSID
#define FMSTR_DEMO_SSID "nxp_wifi_demo"
#endif
#ifndef FMSTR_DEMO_PASSPHRASE
#define FMSTR_DEMO_PASSPHRASE ""
#endif

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////
static struct wlan_network sta_network;
static int auth_fail   = 0;
static int initialized = 0;

static void print_ipaddr(unsigned short port)
{
    int ret = 0;
    struct wlan_ip_config addr;

    ret = wlan_get_address(&addr);
    if (ret == WM_SUCCESS)
    {
        PRINTF("IPv4 Address     : %s\n", inet_ntoa(addr.ipv4.address));
        PRINTF("IPv4 Subnet mask : %s\n", inet_ntoa(addr.ipv4.netmask));
        PRINTF("IPv4 Gateway     : %s\n\n", inet_ntoa(addr.ipv4.gw));
    }

    if (port != 0U)
    {
        PRINTF(
            "Run the FreeMASTER PC Host application, select the network communication\n"
            "plug-in and connect to this board's IP Address specified above at port %d.\n\n",
            port);
    }
}

static void network_wifi_connect(void)
{
    int ret                  = 0;
    uint8_t network_name_len = 0;
    uint8_t ssid_len         = 0;
    uint8_t psk_len          = 0;
    memset(&sta_network, 0, sizeof(struct wlan_network));

    network_name_len = (strlen("sta_network") < WLAN_NETWORK_NAME_MAX_LENGTH) ? (strlen("sta_network") + 1) :
                                                                                WLAN_NETWORK_NAME_MAX_LENGTH;
    strncpy(sta_network.name, "sta_network", network_name_len);

    /* Copy WiFi SSID */
    ssid_len = (strlen(FMSTR_DEMO_SSID) <= IEEEtypes_SSID_SIZE) ? strlen(FMSTR_DEMO_SSID) : IEEEtypes_SSID_SIZE;
    memcpy(sta_network.ssid, (const char *)FMSTR_DEMO_SSID, ssid_len);

    /* WiFi settings */
    sta_network.ip.ipv4.addr_type = ADDR_TYPE_DHCP;
    sta_network.ssid_specific     = 1;

    /* Copy WiFi Password */
    if (strlen(FMSTR_DEMO_PASSPHRASE))
    {
        sta_network.security.type = WLAN_SECURITY_WILDCARD;
        psk_len = (strlen(FMSTR_DEMO_PASSPHRASE) <= (WLAN_PSK_MAX_LENGTH - 1)) ? strlen(FMSTR_DEMO_PASSPHRASE) :
                                                                                 (WLAN_PSK_MAX_LENGTH - 1);
        strncpy(sta_network.security.psk, FMSTR_DEMO_PASSPHRASE, psk_len);
        sta_network.security.psk_len = psk_len;
    }
    else
    {
        sta_network.security.type = WLAN_SECURITY_NONE;
    }

    /* Add WiFi network to list */
    ret = wlan_add_network(&sta_network);
    if (ret != 0)
    {
        PRINTF(" Failed to add network %d\r\n", ret);
        return;
    }

    /* Start connecting */
    ret = wlan_connect(sta_network.name);
    if (ret != WM_SUCCESS)
    {
        PRINTF("Failed to connect %d\r\n", ret);
    }

    PRINTF("Connecting to %s .....\n", sta_network.ssid);
}

/* Callback Function passed to WLAN Connection Manager. The callback function
 * gets called when there are WLAN Events that need to be handled by the
 * application.
 */
int wlan_event_callback(enum wlan_event_reason reason, void *data)
{
    switch (reason)
    {
        case WLAN_REASON_AUTH_SUCCESS:
            PRINTF("WLAN: authenticated to network\n");
            break;
        /* Connected to WiFi AP  */
        case WLAN_REASON_SUCCESS:
            PRINTF("WLAN: connected to network\n");
            PRINTF("Connected to following BSS:\n");
            PRINTF("SSID = [%s]\n\n", sta_network.ssid);
            auth_fail = 0;

            /* Print IP address */
            print_ipaddr(FMSTR_NET_PORT);
            break;
        case WLAN_REASON_CONNECT_FAILED:
            PRINTF("WLAN: connect failed\n");
            break;
        case WLAN_REASON_NETWORK_NOT_FOUND:
            PRINTF("WLAN: network not found\n");
            break;
        case WLAN_REASON_NETWORK_AUTH_FAILED:
            PRINTF("WLAN: network authentication failed\n");
            auth_fail++;
            if (auth_fail >= 3)
            {
                PRINTF("Authentication Failed. Disconnecting ... \n");
                wlan_disconnect();
                auth_fail = 0;
            }
            break;
        case WLAN_REASON_ADDRESS_SUCCESS:
            PRINTF("WLAN: WLAN_REASON_ADDRESS_SUCCESS\n");
            break;
        case WLAN_REASON_ADDRESS_FAILED:
            PRINTF("WLAN: failed to obtain an IP address\n");
            break;
        case WLAN_REASON_LINK_LOST:
            PRINTF("WLAN: WLAN_REASON_LINK_LOST\n");
            break;
        case WLAN_REASON_CHAN_SWITCH:
            PRINTF("WLAN: WLAN_REASON_CHAN_SWITCH\n");
            break;
        case WLAN_REASON_WPS_DISCONNECT:
            PRINTF("WLAN: WLAN_REASON_WPS_DISCONNECT\n");
            break;
        case WLAN_REASON_USER_DISCONNECT:
            PRINTF("WLAN: WLAN_REASON_USER_DISCONNECT\n");
            break;
        /* WiFi driver initialized */
        case WLAN_REASON_INITIALIZED:
            PRINTF("WLAN Driver initialized\n");
            network_wifi_connect();
            initialized = 1;
            break;
        case WLAN_REASON_INITIALIZATION_FAILED:
            PRINTF("WLAN: initialization failed\r\n");
            break;
        case WLAN_REASON_PS_ENTER:
            PRINTF("WLAN: WLAN_REASON_PS_ENTER\n");
            break;
        case WLAN_REASON_PS_EXIT:
            PRINTF("WLAN: WLAN_REASON_PS_EXIT\n");
            break;
        case WLAN_REASON_UAP_SUCCESS:
            PRINTF("WLAN: WLAN_REASON_UAP_SUCCESS\n");
            break;
        case WLAN_REASON_UAP_CLIENT_ASSOC:
            PRINTF("WLAN: WLAN_REASON_UAP_CLIENT_ASSOC\n");
            break;
        case WLAN_REASON_UAP_CLIENT_DISSOC:
            PRINTF("WLAN: WLAN_REASON_UAP_CLIENT_DISSOC\n");
            break;
        case WLAN_REASON_UAP_START_FAILED:
            PRINTF("WLAN: WLAN_REASON_UAP_START_FAILED\n");
            break;
        case WLAN_REASON_UAP_STOP_FAILED:
            PRINTF("WLAN: WLAN_REASON_UAP_STOP_FAILED\n");
            break;
        case WLAN_REASON_UAP_STOPPED:
            PRINTF("WLAN: WLAN_REASON_UAP_STOPPED\n");
            break;
        default:
            PRINTF("WLAN: Unknown reason: %d\n", reason);
    }

    return 0;
}

void Network_Init()
{
    int32_t result = 0;

    /* potentially unused variable */
    result = result;

    memset(&sta_network, 0, sizeof(sta_network));

    PRINTF("Initialize WLAN Driver\n");

    /* Initialize WIFI Driver */
    result = wlan_init(wlan_fw_bin, wlan_fw_bin_len);
    assert(WM_SUCCESS == result);

    /* Start WIFI Driver */
    result = wlan_start(wlan_event_callback);
    assert(WM_SUCCESS == result);

    /* Wait for wifi initialized right */
    while (!initialized)
    {
        /* wait for interface up */
        OSA_TimeDelay(500);
    }
}

void Network_Poll(void)
{
}
