/*
 * Copyright 2020 - 2021, 2024-2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <porting.h>
#include <string.h>
#include <errno/errno.h>
#include <stdbool.h>
#include <sys/atomic.h>
#include <sys/byteorder.h>
#include <sys/util.h>
#include <sys/slist.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include <bluetooth/l2cap.h>
#include <bluetooth/a2dp.h>
#include <bluetooth/a2dp_codec_sbc.h>
#include <bluetooth/hfp_ag.h>
#include <bluetooth/hfp_hf.h>
#include <bluetooth/map_mce.h>
#include <bluetooth/sdp.h>
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "app_connect.h"
#include "app_avrcp.h"
#include "app_a2dp_source.h"
#include "app_a2dp_sink.h"
#include "app_handsfree_ag.h"
#include "app_handsfree.h"
#include "app_pbap_pce.h"
#include "app_map_mce.h"
#include "display_interface.h"
#include "lfs.h"
#include "littlefs_pl.h"


static int app_auto_connect_del_addr(bt_addr_t const *addr);
static void connected(struct bt_conn *conn, uint8_t err);
static void disconnected(struct bt_conn *conn, uint8_t reason);
static void security_changed(struct bt_conn *conn, bt_security_t level,
				 enum bt_security_err err);
static uint8_t map_sdp_discover_cb(struct bt_conn *conn, struct bt_sdp_client_result *result);

extern struct bt_map_mce_mas_cb map_mas_cb;
extern struct bt_map_mce_mns_cb map_mns_cb;
#define SDP_CLIENT_USER_BUF_LEN        512U
NET_BUF_POOL_FIXED_DEFINE(sdp_client_pool, CONFIG_BT_MAX_CONN, SDP_CLIENT_USER_BUF_LEN, CONFIG_NET_BUF_USER_DATA_SIZE, NULL);

bt_addr_t g_riderHsAddr, g_riderPhoneAddr, g_passengerHsAddr;

uint8_t g_connectInitRiderHs = 0, g_connectInitRiderPhone = 0 , g_connectInitPassengerHs = 0;
uint8_t g_profileConnectedPhone = 0, g_profileConnectedRiderHs = 0, g_profileConnectedPassengerHs = 0;
uint8_t g_isRiderHeadset=1;
uint8_t g_auto_connect_paired_devices = 0;
uint8_t g_auto_connect_device_index = 1;
static struct bt_conn_cb conn_callbacks = {
    .connected = connected,
    .disconnected = disconnected,
    .security_changed = security_changed,
};

paired_device_t paired_devices[MAX_PAIRED_DEVICES] = {0};
int g_pairedDeviceCount = 0;

uint8_t connection_status = 0;
int con_retries = 2;
int con_attempts = 0;
uint8_t g_auto_connection_status = 0;

#if !((defined AUTO_CONNECT_USE_BOND_INFO) && (AUTO_CONNECT_USE_BOND_INFO))
static lfs_t * lfs;
static lfs_file_t g_lfsFile;
//#define FILE_NAME "peer_addr"
#endif

#define LFS_PAIRED_DEVICES_FILE  "paired_devices"
#define SETUP_CONNECTION_DELAY K_MSEC(2000)
struct k_work_delayable setup_auto_connection_work;

void hfp_ag_discover(struct bt_conn_info *info)
{

	int res;
	/*
	 * Do an SDP Query on Successful ACL connection complete with the
	 * required device
	 */
	if (0 == memcmp(info->br.dst, &g_riderHsAddr, 6U))
	{
		res = bt_hfp_ag_discover(conn_rider_hs, &app_hfp_ag_discover);
		if (res)
		{
#ifdef APP_DEBUG_EN
			PRINTF("DUT as HFP-AG SDP discovery failed (err %d)\n",res);
#endif
		}
		else
		{
#ifdef APP_DEBUG_EN
			PRINTF("DUT as HFP-AG SDP discovery started\n");
#endif
		}
	}
	if(0 == memcmp(info->br.dst, &g_passengerHsAddr, 6U))
	{
		res = bt_hfp_ag_discover(conn_passenger_hs, &app_hfp_ag_discover);
				if (res)
				{
		#ifdef APP_DEBUG_EN
					PRINTF("DUT as HFP-AG SDP discovery failed (err %d)\n",res);
		#endif
				}
				else
				{
		#ifdef APP_DEBUG_EN
					PRINTF("DUT as HFP-AG SDP discovery started\n");
		#endif
				}
	}
}

void sdp_discover_for_hfp_hf(struct bt_conn_info *info)
{
	int res;
	/*
	 * Do an SDP Query on Successful ACL connection complete with the
	 * required device
	 */
	if (0 == memcmp(info->br.dst, &g_riderPhoneAddr, 6U))
	{
		int res;
		res = bt_hfp_hf_discover(conn_rider_phone, &app_hfp_hf_discover);
		if (res)
		{
#ifdef APP_DEBUG_EN
			PRINTF("DUT as HFP-Dev SDP discovery failed (err %d)\n",res);
#endif
		}
		else
		{
#ifdef APP_DEBUG_EN
			PRINTF("DUT as HFP-Dev SDP discovery started\n");
#endif
		}
	}
}

void app_clear_pbap_data(void)
{
	for (int index = 0; index < MAX_PHONE_CONNECTION; index++)
	{
		clear_pbap_data(index);
	}

}

static void setup_auto_connection(struct k_work *work)
{
	//PRINTF("app_auto_connect_paired_devices work.\n");
	app_auto_connect_paired_devices();

}

static void connected(struct bt_conn *conn, uint8_t err)
{
	struct bt_conn_info info;
        uint16_t conn_handle;
	if (err)
	{
		PRINTF("ACL Connection Failed (err %d)\n",err);
		if (g_connectInitRiderPhone)
		{
			g_connectInitRiderPhone = 0U;
#ifdef APP_DEBUG_EN
			PRINTF("DUT as HFP-HF to remote as HFP-AG Connection failed (err 0x%02x)\n", err);
#endif
			//app_hf_set_connectable();
	        if (g_appInstance.acl_conn != NULL)
	        {
	            g_appInstance.acl_conn = NULL;
	        }
			if (conn_rider_phone != NULL)
			{
				conn_rider_phone = NULL;
			}
			app_dual_a2dp_src_resume();
		}
		if (g_connectInitRiderHs)
		{
			g_connectInitRiderHs = 0U;
			if (conn_rider_hs != NULL)
			{
				conn_rider_hs = NULL;
			}
#ifdef APP_DEBUG_EN
			PRINTF("DUT as HFP-AG to remote as HFP-HF Connection failed (err %d)\n", err);
#endif
		}

		if (g_connectInitPassengerHs)
		{
			g_connectInitPassengerHs = 0U;
			if (conn_passenger_hs != NULL)
			{
				conn_passenger_hs = NULL;
			}
#ifdef APP_DEBUG_EN
			PRINTF("DUT as HFP-AG to remote as HFP-HF Connection failed (err %d)\n", err);
#endif
		}
		// Auto connect if failed
		if(g_auto_connection_status)
		{
			vTaskDelay(pdMS_TO_TICKS(5000));
			k_work_init_delayable(&setup_auto_connection_work, setup_auto_connection);
			k_work_schedule(&setup_auto_connection_work, SETUP_CONNECTION_DELAY);
		}
		return;
	}

	bt_conn_get_info(conn, &info);
        bt_hci_get_conn_handle(conn,&conn_handle);
	if (info.type == BT_CONN_TYPE_LE)
	{
		return;
	}

	if (g_connectInitRiderHs)
	{
		PRINTF("ACL Connection Successful with Rider Headset,Role: %d\n",info.role);
		g_connectInitRiderHs = 0U;
		g_profileConnectedRiderHs = 1;
		conn_rider_hs = conn;
		hfp_ag_discover(&info);

		uint8_t ret;
		struct net_buf *buf = NULL;

		struct bt_hci_cp_write_link_policy_settings *cp;
		buf = bt_hci_cmd_create(BT_HCI_OP_WRITE_LINK_POLICY_SETTINGS, sizeof(*cp));
		if (buf != NULL)
		{
			PRINTF("\r\nSet link policy \r\n");
			cp = net_buf_add(buf, sizeof(*cp));
			cp->handle = conn_handle;
			cp->link_policy_settings = A2DP_LINK_POLICY;
			err = bt_hci_cmd_send_sync(BT_HCI_OP_WRITE_LINK_POLICY_SETTINGS, buf, NULL);
		}

		//A2DP profile connection
		sdp_discover_for_a2dp_sink(1);
		connection_status |= (1 << RIDER_HEADSET);
    }
	else if (g_connectInitRiderPhone)
	{
		PRINTF("ACL Connection Successful with Rider Phone,Role: %d\n",info.role);
		g_connectInitRiderPhone = 0U;
		conn_rider_phone = conn;
#if AVRCP_BROWSING_ENABLE
		if(!bt_avrcp_browsing_connect(conn_rider_phone))
		{
			PRINTF("AVRCP Browsing Connect SUCCESS\r\n");
		}
#endif

		/*Profile level connection HFP HF*/
		sdp_discover_for_hfp_hf(&info);
		/*Profile level connection for PBAP*/
#if defined(PBAP_PROFILE_ENABLE) && (PBAP_PROFILE_ENABLE == 1)
		PRINTF( "\n\nPBAP connection\n");
		sdp_discover_for_pbap_client(conn_rider_phone);
		connection_status |= (1 << RIDER_PHONE);
		app_update_last_connected_device(info.br.dst->val,RIDER_PHONE);
		app_dual_a2dp_src_resume();
#endif
	} else if (g_connectInitPassengerHs)
	{
		PRINTF("ACL Connection Successful with Passenger Headset,Role: %d\n",info.role);
		g_connectInitPassengerHs=0U;
		g_profileConnectedPassengerHs = 1;
		conn_passenger_hs = conn;

		hfp_ag_discover(&info);// NO HFP connection to passenger headset, to be enabled later.

		uint8_t ret;
		struct net_buf *buf = NULL;

		struct bt_hci_cp_write_link_policy_settings *cp;
		buf = bt_hci_cmd_create(BT_HCI_OP_WRITE_LINK_POLICY_SETTINGS, sizeof(*cp));
		if (buf != NULL)
		{
			PRINTF("\r\nSet link policy \r\n");
			cp = net_buf_add(buf, sizeof(*cp));
			cp->handle = conn_handle;
			cp->link_policy_settings = 0x0000U;
			err = bt_hci_cmd_send_sync(BT_HCI_OP_WRITE_LINK_POLICY_SETTINGS, buf, NULL);
		}

		//A2DP profile connection
		sdp_discover_for_a2dp_sink(0);
		connection_status |= (1 << PASSENGER_HEADSET);
	}
	if(g_auto_connection_status)
	{
		k_work_init_delayable(&setup_auto_connection_work, setup_auto_connection);
		k_work_schedule(&setup_auto_connection_work, SETUP_CONNECTION_DELAY);
	}
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
    PRINTF("ACL Disconnection (reason %d)\n", reason);

    if(conn_rider_hs == conn)
    {
#ifdef APP_DEBUG_EN
    	PRINTF("Rider Headset disconnected !!\n");
#endif
        app_hfp_ag_disconnect();
		conn_rider_hs = NULL;
    	g_connectInitRiderHs = 0U;
    	g_avrcpControlConnectHfpHf = false;
    	g_sHfpAgRhDisconnecting =0;
    }else if(conn_passenger_hs == conn)
    {
#ifdef APP_DEBUG_EN
    	PRINTF("Passenger Headset disconnected !!\n");
#endif
        //app_hfp_ag_disconnect();
    	conn_passenger_hs = NULL;
    	g_connectInitPassengerHs = 0U;
    	g_avrcpControlConnectHfpHf = false;
    	g_sHfpAgPhDisconnecting =0;
    }
    else if(conn_rider_phone == conn)
    {
#ifdef APP_DEBUG_EN
    	PRINTF("Rider Phone disconnected !!\n");
#endif
    	bt_conn_unref(conn);
	conn_rider_phone = NULL;
        if (g_appInstance.acl_conn != NULL)
        {
            g_appInstance.acl_conn = NULL;
        }
		g_connectInitRiderPhone = 0U;
		app_a2dp_bridge_src_stop();
		app_clear_pbap_data();
		//app_hf_set_connectable();
    }
	else
	{
#ifdef APP_DEBUG_EN
    	PRINTF("Disconnection event from unknown device !!\n");
#endif
		return;
	}
}

static void reverse_order(uint8_t dest[6], const uint8_t src[6])
{
    for (int i = 0; i < 6; i++)
    {
        dest[i] = src[5 - i];  // Reverse order
    }
}


static void security_changed(struct bt_conn *conn, bt_security_t level,
				 enum bt_security_err err)
{
    char addr[BT_ADDR_LE_STR_LEN];

    bt_addr_to_str(bt_conn_get_dst_br(conn), addr, sizeof(addr));

    if (!err)
    {
        PRINTF("Security changed: %s level %u\n", addr, level);
        save_new_paired_device(conn,g_isRiderHeadset);
    }
    else
    {
        PRINTF("Security failed: %s level %u err %d\n", addr, level, err);
        if (err == BT_SECURITY_ERR_PIN_OR_KEY_MISSING || err == BT_SECURITY_ERR_AUTH_FAIL)
        {
            bt_addr_le_t addr;
            struct bt_conn_info info;
            int ret;

            bt_conn_get_info(conn, &info);
            if (info.type == BT_CONN_TYPE_LE)
            {
                return;
            }

            PRINTF("The peer device seems to have lost the bonding information.\n");
            PRINTF("Delete the bonding information of the peer, please try again.\n");
            addr.type = BT_ADDR_LE_PUBLIC;
            addr.a = *info.br.dst;
            ret = bt_unpair(BT_ID_DEFAULT, &addr);
            if (ret)
            {
                PRINTF("fail to delete.\n");
            }
            else
            {
            	uint8_t device_addr[6] = {0};
            	reverse_order(device_addr, addr.a.val);

            	remove_paired_device(device_addr);
                PRINTF("success to delete.\n");
            }
        }
//        PRINTF("Security failed: %s level %u err %d\n", addr, level, err);
    }
}

void app_connect(uint8_t device_type,uint8_t *addr)
{
    uint8_t corrected_addr[6] = {0};

	if(device_type == RIDER_PHONE)
	{
		if(conn_rider_phone == NULL )
		{
			app_dual_a2dp_src_pause();
			g_connectInitRiderPhone = 1U;
			memcpy(&g_riderPhoneAddr, addr, 6U);


			conn_rider_phone = bt_conn_create_br(&g_riderPhoneAddr, BT_BR_CONN_PARAM_DEFAULT);
			if (!conn_rider_phone)
			{
				g_connectInitRiderPhone = 0U;
				PRINTF("ACL connection failed\r\n");

				if (g_appInstance.acl_conn != NULL)
				{
					g_appInstance.acl_conn = NULL;
				}
				app_dual_a2dp_src_resume();
			}
			else
			{
				/* unref connection obj in advance as app user */
				bt_conn_unref(conn_rider_phone);
				if(conn_rider_phone == NULL )
						{
					PRINTF("Debug NULL Connecting Rider Phone\r\n");
						}
				PRINTF("Connecting Rider Phone\r\n");
			}
		} else
		{

			PRINTF(" Rider Phone connection already exists !! \r\n");
		}

	} else if(device_type == RIDER_HEADSET)
	{

		if(conn_rider_hs == NULL )
		{
		    g_connectInitRiderHs = 1U;
		    memcpy(&g_riderHsAddr, addr, 6U);
		    g_isRiderHeadset=1;


		    conn_rider_hs = bt_conn_create_br(&g_riderHsAddr, BT_BR_CONN_PARAM_DEFAULT);
		    if (!conn_rider_hs)
		    {
		        g_connectInitRiderHs = 0U;
		        PRINTF("ACL Connection Failed:\r\n");
		    }
		    else
		    {
		        /* unref connection obj in advance as app user */
		        bt_conn_unref(conn_rider_hs);
		        PRINTF("Connecting Rider Headset\r\n");
		    }

		} else {

			PRINTF("The Rider Headset connection already exists \r\n");
		}

	} else if( device_type == PASSENGER_HEADSET )
	{

		if(conn_passenger_hs == NULL )
		{
			g_connectInitPassengerHs = 1U;
			memcpy(&g_passengerHsAddr, addr, 6U);
			g_isRiderHeadset=0;


			conn_passenger_hs = bt_conn_create_br(&g_passengerHsAddr, BT_BR_CONN_PARAM_DEFAULT);
			if (!conn_passenger_hs)
			{
				g_connectInitPassengerHs = 0U;
				PRINTF("ACL Connection Failed:\r\n");
			}
			else
			{
				/* unref connection obj in advance as app user */
				bt_conn_unref(conn_passenger_hs);
				PRINTF("Connecting Passenger Headset \r\n");
			}

		}else {

			PRINTF("The Passenger Headset connection already exists \r\n");
		}

	}
}

void app_disconnect(uint8_t device_type)
{
	int err=0;
	if(device_type == RIDER_HEADSET )
	{
		g_sHfpAgRhDisconnecting = 1;
	    if (err = bt_conn_disconnect(conn_rider_hs, BT_HCI_ERR_REMOTE_USER_TERM_CONN))
	    {
	        PRINTF("Disconnection failed (err %d)\r\n",err);
	    }
	}
	else if(device_type == RIDER_PHONE)
	{
	    if (err = bt_conn_disconnect(conn_rider_phone, BT_HCI_ERR_REMOTE_USER_TERM_CONN))
	    {
	        PRINTF("Disconnection failed (err %d)\r\n",err);
	    }
	}else if(device_type == PASSENGER_HEADSET)
	{
	    if (err = bt_conn_disconnect(conn_passenger_hs, BT_HCI_ERR_REMOTE_USER_TERM_CONN))
	    {
	        PRINTF("Disconnection failed (err %d)\r\n",err);
	    }
	}
}

void app_schedule_auto_connect()
{
	g_auto_connection_status =1;
	PRINTF("Connecting to devices .. !!\n");
	vTaskDelay(pdMS_TO_TICKS(4000));
	k_work_init_delayable(&setup_auto_connection_work, setup_auto_connection);
	k_work_schedule(&setup_auto_connection_work, SETUP_CONNECTION_DELAY);
}

void app_auto_connect_paired_devices()
{
   if (con_attempts > (con_retries * 3 +1))
	{
		g_auto_connection_status =0;
		if(connection_status & RIDER_PHONE_BIT)
			PRINTF("The Rider Phone connected !!\n");

		if(connection_status & RIDER_HEADSET_BIT)
			PRINTF("The Rider Headset connected !!\n");

		if(connection_status & PASSENGER_HEADSET_BIT)
			PRINTF("The Passenger Headset connected !!\n");
		return;
	}

	if(con_attempts == 0)
		g_auto_connection_status =1;

	con_attempts++;

	if((connection_status & RIDER_PHONE_BIT)
		&& (connection_status & RIDER_HEADSET_BIT)
		&& (connection_status & PASSENGER_HEADSET_BIT))
	{
		g_auto_connection_status =0;
		PRINTF("The Rider Phone, Headset and Passenger Headset connected !!\n");
		return;
	}

	switch (con_attempts % 3)
	{
	    case 1:
	    	if (!(connection_status & RIDER_PHONE_BIT))
	    	{
	    		app_auto_connect_device(RIDER_PHONE);
	    		return;
	    	}
	        break;
	    case 2:
	    	if (!(connection_status & RIDER_HEADSET_BIT))
	    	{
	    		app_auto_connect_device(RIDER_HEADSET);
	    		return;
	    	}
	        break;
	    case 0:
	    	if (!(connection_status & PASSENGER_HEADSET_BIT))
	    	{
	    		app_auto_connect_device(PASSENGER_HEADSET);
	    		return;
	    	}
	        break;
	}
	app_auto_connect_paired_devices();
}

void app_connect_init(void)
{
    bt_conn_cb_register(&conn_callbacks);
}


void app_lfs_init(void)
{

    lfs = lfs_pl_init();
    if (!lfs)
    {
    	PRINTF("lfs_pl_init failed!\n");
    }

    app_read_paired_devices();

}

int app_read_paired_devices()
{
    if (!lfs)
    {
        PRINTF("LittleFS not initialized!\n");
        return -1;
    }

    lfs_file_t file;
    int err = lfs_file_open(lfs, &file, LFS_PAIRED_DEVICES_FILE, LFS_O_RDONLY);
    if (err < 0)
    {
//        PRINTF("Failed to open paired devices file: error code = %d\n", err);
        return err;
    }

    // First, read the stored g_pairedDeviceCount
    err = lfs_file_read(lfs, &file, &g_pairedDeviceCount, sizeof(g_pairedDeviceCount));
    if (err < 0 || g_pairedDeviceCount > MAX_PAIRED_DEVICES)
    {
        PRINTF("Failed to read paired device count or count is invalid.\n");
        g_pairedDeviceCount = 0;  // Reset in case of error
        lfs_file_close(lfs, &file);
        return -1;
    }

    //Then, read the paired devices list
    err = lfs_file_read(lfs, &file, paired_devices, g_pairedDeviceCount * sizeof(paired_device_t));
    if (err < 0)
    {
        PRINTF("Failed to read paired devices. Error code = %d\n", err);
        g_pairedDeviceCount = 0;  // Reset in case of error
        lfs_file_close(lfs, &file);
        return -1;
    }

    lfs_file_close(lfs, &file);

    if(g_pairedDeviceCount){
    	PRINTF("Paired Devices List:\n");
    }

    for (int i = 0; i < g_pairedDeviceCount; i++)
    {
        PRINTF("[%d] Address: %02X:%02X:%02X:%02X:%02X:%02X, Name: %s, Type: %d\n",
               i + 1,
               paired_devices[i].addr[0], paired_devices[i].addr[1], paired_devices[i].addr[2],
               paired_devices[i].addr[3], paired_devices[i].addr[4], paired_devices[i].addr[5],
               paired_devices[i].name, paired_devices[i].device_type);
    }

    return 0;
}

int app_save_paired_device(const uint8_t addr[6], const char *name, uint8_t device_type)
{

	if (!lfs)  //Ensure LFS is initialized
    {
        PRINTF("LittleFS not initialized!\n");
        return -1;
    }

	for (int i = 0; i < g_pairedDeviceCount; i++)
	{
		if (memcmp(paired_devices[i].addr, addr, 6) == 0)
		{
			PRINTF("Device already exists in paired list.\n");
			return 0;
		}
	}

    if (g_pairedDeviceCount >= MAX_PAIRED_DEVICES)
    {
    	PRINTF("Paired devices reached max number, \n Please remove an existing device to save new paired device.\n");
    	return 0;
    }

    vTaskDelay(pdMS_TO_TICKS(50));
    // Add new device
    memcpy(paired_devices[g_pairedDeviceCount].addr, addr, 6);
    strncpy(paired_devices[g_pairedDeviceCount].name, name, sizeof(paired_devices[g_pairedDeviceCount].name) - 1);
    paired_devices[g_pairedDeviceCount].device_type = device_type;
    g_pairedDeviceCount++;

    lfs_remove(lfs, LFS_PAIRED_DEVICES_FILE);
    vTaskDelay(pdMS_TO_TICKS(50));
    // Save to LittleFS
    lfs_file_t file;
    int err = lfs_file_open(lfs, &file, LFS_PAIRED_DEVICES_FILE, LFS_O_WRONLY | LFS_O_CREAT );
    if (err < 0)  //Debug: Check open file error
    {
        PRINTF("Failed to open file: error code = %d\n", err);
        return err;
    }

    err = lfs_file_write(lfs, &file, &g_pairedDeviceCount, sizeof(g_pairedDeviceCount));
    if (err < 0)
    {
        PRINTF("Failed to write paired device count. Error code = %d\n", err);
        lfs_file_close(lfs, &file);
        return err;
    }

    err = lfs_file_write(lfs, &file, paired_devices, g_pairedDeviceCount * sizeof(paired_device_t));
    if (err < 0)
    {
        PRINTF("Failed to write paired device. Error code = %d\n", err);
        lfs_file_close(lfs, &file);
        return err;
    }

    lfs_file_close(lfs, &file);
    return 0;
}


int app_save_paired_devices()
{
    if (!lfs)
    {
        PRINTF("LittleFS not initialized!\n");
        return -1;
    }

    lfs_remove(lfs, LFS_PAIRED_DEVICES_FILE);
    vTaskDelay(pdMS_TO_TICKS(50));

    lfs_file_t file;
    int err = lfs_file_open(lfs, &file, LFS_PAIRED_DEVICES_FILE, LFS_O_WRONLY | LFS_O_CREAT );
    if (err < 0)
    {
        PRINTF("Failed to open paired devices file: error code = %d\n", err);
        return err;
    }

    // First, write `g_pairedDeviceCount`
    err = lfs_file_write(lfs, &file, &g_pairedDeviceCount, sizeof(g_pairedDeviceCount));
    if (err < 0)
    {
        PRINTF("Failed to write paired device count. Error code = %d\n", err);
        lfs_file_close(lfs, &file);
        return err;
    }

    // Then, write the paired devices list
    err = lfs_file_write(lfs, &file, paired_devices, g_pairedDeviceCount * sizeof(paired_device_t));
    if (err < 0)
    {
        PRINTF("Failed to write paired devices. Error code = %d\n", err);
        lfs_file_close(lfs, &file);
        return err;
    }

    lfs_file_close(lfs, &file);
   // PRINTF("Paired devices saved to LFS.\n");
    return 0;
}

int app_clear_paired_devices()
{
    if (!lfs)
    {
        PRINTF("LittleFS not initialized!\n");
        return -1;
    }

    PRINTF("Clearing all paired devices from LittleFS...\n");

    //step 1: Reset the paired devices array in RAM
    memset(paired_devices, 0, sizeof(paired_devices));
    g_pairedDeviceCount = 0;

    //Step 2: Remove the paired devices file from LittleFS
    int err = lfs_remove(lfs, LFS_PAIRED_DEVICES_FILE);
    if (err < 0)
    {
        PRINTF("Failed to remove paired devices file: error code = %d\n", err);
        return err;
    }
    vTaskDelay(pdMS_TO_TICKS(50));
    PRINTF("All paired devices successfully removed from LFS.\n");
    return 0;
}


