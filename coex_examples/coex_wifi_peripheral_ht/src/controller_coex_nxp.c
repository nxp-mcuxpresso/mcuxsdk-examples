/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * The BSD-3-Clause license can be found at https://spdx.org/licenses/BSD-3-Clause.html
 */

 #include "fsl_debug_console.h"

 #if(CONFIG_WIFI_BLE_COEX_APP)
 #include "cli.h"
 #include "dhcp-server.h"
 #include "wlan.h"
 // #include "wifi_ping.h"
 #include "iperf.h"
 #include <wm_net.h>
 #include "wlan_bt_fw.h"
 
 #ifndef RW612_SERIES
 #include "wifi_bt_config.h"
 #include "controller.h"
 #include "firmware_dnld.h"
 #if (CONFIG_BT_IND_DNLD || CONFIG_WIFI_IND_DNLD)
 #include "fw_loader_uart.h"
 #endif /* CONFIG_BT_IND_DNLD || CONFIG_WIFI_IND_DNLD */
 #else
 #include "fsl_rtc.h"
 #endif /* RW612_SERIES */
 #endif /* CONFIG_WIFI_BLE_COEX_APP */
 
 #ifdef RW612_SERIES
 #if(CONFIG_OT_CLI)
 #include "ot_platform_common.h"
 #include "fwk_platform_coex.h"
 #else
 #include "fwk_platform_ble.h"
 #endif /* CONFIG_OT_CLI */
 #endif /* RW612_SERIES */
 
 static void printSeparator(void)
 {
     PRINTF("========================================\r\n");
 }
 
 #if(CONFIG_WIFI_BLE_COEX_APP)
 static struct wlan_network uap_network;
 static struct wlan_network sta_network;
 extern int ping_cli_init(void);
 
 #ifdef RW612_SERIES
 static void dump_set_rtc_time_usage(void)
 {
     (void)PRINTF("Usage: wlan-set-rtc-time <year> <month> <day> <hour> <minute> <second>\r\n");
     (void)PRINTF("\r\nUsage example : \r\n");
     (void)PRINTF("wlan-set-rtc-time 2022 12 31 19 00\r\n");
 }
 
 static void test_wlan_set_rtc_time(int argc, char **argv)
 {
     rtc_datetime_t date;
     int ret;
 
     if (argc < 0)
     {
         (void)PRINTF("Error: invalid number of arguments\r\n");
         dump_set_rtc_time_usage();
         return;
     }
     date.year   = (uint16_t)atoi(argv[1]);
     date.month  = (uint8_t)atoi(argv[2]);
     date.day    = (uint8_t)atoi(argv[3]);
     date.hour   = (uint8_t)atoi(argv[4]);
     date.minute = (uint8_t)atoi(argv[5]);
     date.second = (uint8_t)atoi(argv[6]);
 
     /* RTC time counter has to be stopped before setting the date & time in the TSR register */
     RTC_EnableTimer(RTC, false);
 
     /* Set RTC time to default */
     ret = RTC_SetDatetime(RTC, &date);
     if (ret != kStatus_Success)
     {
         (void)PRINTF("Error: invalid number of arguments\r\n");
         dump_set_rtc_time_usage();
     }
 
     /* Start the RTC time counter */
     RTC_EnableTimer(RTC, true);
 
     /* Get date time */
     RTC_GetDatetime(RTC, &date);
 
     /* print default time */
     (void)PRINTF("Current datetime: %04hd-%02hd-%02hd %02hd:%02hd:%02hd\r\n", date.year, date.month, date.day,
                 date.hour, date.minute, date.second);
 }
 
 static void test_wlan_get_rtc_time(int argc, char **argv)
 {
     rtc_datetime_t date;
 
     /* Get date time */
     RTC_GetDatetime(RTC, &date);
 
     /* print default time */
     (void)PRINTF("Current datetime: %04hd-%02hd-%02hd %02hd:%02hd:%02hd\r\n", date.year, date.month, date.day,
                 date.hour, date.minute, date.second);
 }
 
 static struct cli_command wlan_prov_commands[] = {
     {"wlan-set-rtc-time", "<year> <month> <day> <hour> <minute> <second>", test_wlan_set_rtc_time},
     {"wlan-get-rtc-time", NULL, test_wlan_get_rtc_time},
 };
 
 static int wlan_prov_cli_init(void)
 {
     unsigned int i;
 
     for (i = 0; i < sizeof(wlan_prov_commands) / sizeof(struct cli_command); i++)
     {
         if (cli_register_command(&wlan_prov_commands[i]) != 0)
         {
             return -1;
         }
     }
 
     return 0;
 }
 #endif
 
 int wlan_event_callback(enum wlan_event_reason reason, void *data)
 {
     int ret;
     struct wlan_ip_config addr;
     char ip[16];
     static int auth_fail                      = 0;
     wlan_uap_client_disassoc_t *disassoc_resp = data;
 
     printSeparator();
     PRINTF("app_cb: WLAN: received event %d\r\n", reason);
     printSeparator();
 
     switch (reason)
     {
         case WLAN_REASON_INITIALIZED:
             PRINTF("app_cb: WLAN initialized\r\n");
             printSeparator();
 
             ret = wlan_basic_cli_init();
             if (ret != WM_SUCCESS)
             {
                 PRINTF("Failed to initialize BASIC WLAN CLIs\r\n");
                 return 0;
             }
 
             ret = wlan_cli_init();
             if (ret != WM_SUCCESS)
             {
                 PRINTF("Failed to initialize WLAN CLIs\r\n");
                 return 0;
             }
             PRINTF("WLAN CLIs are initialized\r\n");
             printSeparator();
 #ifdef RW610
             ret = wlan_enhanced_cli_init();
             if (ret != WM_SUCCESS)
             {
                 PRINTF("Failed to initialize WLAN CLIs\r\n");
                 return 0;
             }
             PRINTF("ENHANCED WLAN CLIs are initialized\r\n");
             printSeparator();
 #endif
             ret = ping_cli_init();
             if (ret != WM_SUCCESS)
             {
                 PRINTF("Failed to initialize PING CLI\r\n");
                 return 0;
             }
 
             ret = iperf_cli_init();
             if (ret != WM_SUCCESS)
             {
                 PRINTF("Failed to initialize IPERF CLI\r\n");
                 return 0;
             }
 
             ret = dhcpd_cli_init();
             if (ret != WM_SUCCESS)
             {
                 PRINTF("Failed to initialize DHCP Server CLI\r\n");
                 return 0;
             }
 #ifdef RW612_SERIES
             ret = wlan_prov_cli_init();
             if (ret != WM_SUCCESS)
             {
                 PRINTF("Failed to initialize PROV CLI\r\n");
                 return 0;
             }
 #endif
             PRINTF("CLIs Available:\r\n");
             printSeparator();
             help_command(0, NULL);
             printSeparator();
             break;
         case WLAN_REASON_INITIALIZATION_FAILED:
             PRINTF("app_cb: WLAN: initialization failed\r\n");
             break;
         case WLAN_REASON_AUTH_SUCCESS:
             PRINTF("app_cb: WLAN: authenticated to network\r\n");
             break;
         case WLAN_REASON_SUCCESS:
             PRINTF("app_cb: WLAN: connected to network\r\n");
             ret = wlan_get_address(&addr);
             if (ret != WM_SUCCESS)
             {
                 PRINTF("failed to get IP address\r\n");
                 return 0;
             }
 
             net_inet_ntoa(addr.ipv4.address, ip);
 
             ret = wlan_get_current_network(&sta_network);
             if (ret != WM_SUCCESS)
             {
                 PRINTF("Failed to get External AP network\r\n");
                 return 0;
             }
 
             PRINTF("Connected to following BSS:\r\n");
             PRINTF("SSID = [%s]\r\n", sta_network.ssid);
             if (addr.ipv4.address != 0U)
             {
                 PRINTF("IPv4 Address: [%s]\r\n", ip);
             }
 #if CONFIG_IPV6
             int i;
             for (i = 0; i < CONFIG_MAX_IPV6_ADDRESSES; i++)
             {
                 if (ip6_addr_isvalid(addr.ipv6[i].addr_state))
                 {
                     (void)PRINTF("IPv6 Address: %-13s:\t%s (%s)\r\n", ipv6_addr_type_to_desc((struct net_ipv6_config *)&addr.ipv6[i]),
                                 inet6_ntoa(addr.ipv6[i].address), ipv6_addr_state_to_desc(addr.ipv6[i].addr_state));
                 }
             }
             (void)PRINTF("\r\n");
 #endif
             auth_fail = 0;
             break;
         case WLAN_REASON_CONNECT_FAILED:
             PRINTF("app_cb: WLAN: connect failed\r\n");
             break;
         case WLAN_REASON_NETWORK_NOT_FOUND:
             PRINTF("app_cb: WLAN: network not found\r\n");
             break;
         case WLAN_REASON_NETWORK_AUTH_FAILED:
             PRINTF("app_cb: WLAN: network authentication failed\r\n");
             auth_fail++;
             if (auth_fail >= 3)
             {
                 PRINTF("Authentication Failed. Disconnecting ... \r\n");
                 wlan_disconnect();
                 auth_fail = 0;
             }
             break;
         case WLAN_REASON_ADDRESS_SUCCESS:
             PRINTF("network mgr: DHCP new lease\r\n");
             break;
         case WLAN_REASON_ADDRESS_FAILED:
             PRINTF("app_cb: failed to obtain an IP address\r\n");
             break;
         case WLAN_REASON_USER_DISCONNECT:
             PRINTF("app_cb: disconnected\r\n");
             auth_fail = 0;
             break;
         case WLAN_REASON_LINK_LOST:
             PRINTF("app_cb: WLAN: link lost\r\n");
             break;
         case WLAN_REASON_CHAN_SWITCH:
             PRINTF("app_cb: WLAN: channel switch\r\n");
             break;
         case WLAN_REASON_UAP_SUCCESS:
             PRINTF("app_cb: WLAN: UAP Started\r\n");
             ret = wlan_get_current_uap_network(&uap_network);
 
             if (ret != WM_SUCCESS)
             {
                 PRINTF("Failed to get Soft AP network\r\n");
                 return 0;
             }
 
             printSeparator();
             PRINTF("Soft AP \"%s\" started successfully\r\n", uap_network.ssid);
             printSeparator();
             if (dhcp_server_start(net_get_uap_handle()))
                 PRINTF("Error in starting dhcp server\r\n");
 
             PRINTF("DHCP Server started successfully\r\n");
             printSeparator();
             break;
         case WLAN_REASON_UAP_CLIENT_ASSOC:
             PRINTF("app_cb: WLAN: UAP a Client Associated\r\n");
             printSeparator();
             PRINTF("Client => ");
             print_mac((const char *)data);
             PRINTF("Associated with Soft AP\r\n");
             printSeparator();
             break;
         case WLAN_REASON_UAP_CLIENT_CONN:
             PRINTF("app_cb: WLAN: UAP a Client Connected\r\n");
             printSeparator();
             PRINTF("Client => ");
             print_mac((const char *)data);
             PRINTF("Connected with Soft AP\r\n");
             printSeparator();
             break;
         case WLAN_REASON_UAP_CLIENT_DISSOC:
             printSeparator();
             PRINTF("app_cb: WLAN: UAP a Client Dissociated:");
             PRINTF(" Client MAC => ");
             print_mac((const char *)(disassoc_resp->sta_addr));
             PRINTF(" Reason code => ");
             PRINTF("%d\r\n", disassoc_resp->reason_code);
             printSeparator();
             break;
         case WLAN_REASON_UAP_STOPPED:
             PRINTF("app_cb: WLAN: UAP Stopped\r\n");
             printSeparator();
             PRINTF("Soft AP \"%s\" stopped successfully\r\n", uap_network.ssid);
             printSeparator();
 
             dhcp_server_stop();
 
             PRINTF("DHCP Server stopped successfully\r\n");
             printSeparator();
             break;
         case WLAN_REASON_PS_ENTER:
             PRINTF("app_cb: WLAN: PS_ENTER\r\n");
             break;
         case WLAN_REASON_PS_EXIT:
             PRINTF("app_cb: WLAN: PS EXIT\r\n");
             break;
         default:
             PRINTF("app_cb: WLAN: Unknown Event: %d\r\n", reason);
     }
     return 0;
 }
 #endif
 
 #ifndef RW612_SERIES
 void coex_controller_init(void)
 {
     int result;
     (void) result;
 #if (CONFIG_BT_IND_DNLD || CONFIG_WIFI_IND_DNLD)
     BOARD_WIFI_BT_Enable(true);
     /* BT only firmware download over UART */
     (void)PRINTF("Download BT FW...\r\n");
     void *intf = uart_init_interface();
     assert(intf != NULL);
     result = firmware_download(bt_fw_bin, bt_fw_bin_len, intf, 0);
     assert(0 == result);
     (void)PRINTF("Download WLAN FW...\r\n");
 #endif /* CONFIG_BT_IND_DNLD || CONFIG_WIFI_IND_DNLD*/
     /* WLAN only/Combo firmware download over SDIO */
     result = wlan_init(wlan_fw_bin, wlan_fw_bin_len);  
     assert(0 == result);
     result = wlan_start(wlan_event_callback);
     assert(0 == result);
 }
 
 #else /* RW612_SERIES */
 
 #if(CONFIG_OT_CLI)
 extern void appOtStart(int argc, char *argv[]);
 #endif
 
 /* Initialize the platform */
 void coex_controller_init(void)
 {
     int result = 0;
     (void) result;
 
 #if(CONFIG_WIFI_BLE_COEX_APP)
     /* Wi-Fi firmware download over IMU */
     result = wlan_init(wlan_fw_bin, wlan_fw_bin_len);
     assert(0 == result);
 
     result = wlan_start(wlan_event_callback);
     assert(0 == result);
 
     vTaskDelay(1500);
 #endif /* CONFIG_WIFI_BLE_COEX_APP */
 
 #if(CONFIG_OT_CLI)
     /* Initialize 15.4+BLE combo controller first
     * Any other attemps to init a 15.4 or BLE single mode will be ignored
     * For BLE, ethermind calls controller_init which calls PLATFORM_InitBle which calls PLATFORM_InitControllers(connBle_c)
     * As we already initialized the controller in combo mode here, the PLATFORM_InitControllers(connBle_c) call does nothing
     * Same applies for PLATFORM_InitOt which calls PLATFORM_InitControllers(conn802_15_4_c)
     * For RW61x, 15.4/BLE combo image must be flashed at 15.4 firmware address Z154_IMAGE_A_OFFSET (0x85e0000) */
     PLATFORM_InitControllers(conn802_15_4_c | connBle_c);
 
 #if(CONFIG_WIFI_BLE_COEX_APP == 0)
     /* Initialize LWIP stack */
     tcpip_init(NULL, NULL);
 #endif
 
     appOtStart(0, NULL);
 #elif(CONFIG_DISABLE_BLE == 0)
     /* BTonly firmware download */
     PLATFORM_InitBle();
 #endif
 }
 #endif /* RW612_SERIES */
 