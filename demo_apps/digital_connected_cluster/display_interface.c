/*
 *  Copyright 2025 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
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
#include <bluetooth/sdp.h>
#include <bluetooth/hfp_ag.h>
#include <bluetooth/hfp_hf.h>

#include "BT_smp_api.h"
#include "BT_sm_api.h"
#include "smp_pl.h"
#include "bt_pal_conn_internal.h"

#include "display_interface.h"
#include "app_discover.h"
#include "app_connect.h"
#include "lfs.h"
#include "littlefs_pl.h"  // Ensure this contains the declaration

uint32_t g_brDiscoverResultCount;

extern struct bt_br_discovery_result br_discovery_results[APP_INQUIRY_NUM_RESPONSES];

static void br_device_found(size_t index, const bt_addr_t *addr, int8_t rssi,
                            const uint8_t cod[3], const uint8_t eir[240], uint8_t device_type)
{
    char br_addr[BT_ADDR_STR_LEN];
    char name[239];
    int len = 240;

    (void)memset(name, 0, sizeof(name));

    while (len)
    {
        if (len < 2)
        {
            break;
        }

        /* Look for early termination */
        if (!eir[0])
        {
            break;
        }

        /* check if field length is correct */
        if (eir[0] > len - 1)
        {
            break;
        }

        switch (eir[1])
        {
        case BT_DATA_NAME_SHORTENED:
        case BT_DATA_NAME_COMPLETE:
            memcpy(name, &eir[2], (eir[0] - 1) > (sizeof(name) - 1) ? (sizeof(name) - 1) : (eir[0] - 1));
            break;
        default:
            break;
        }

        /* Parse next AD Structure */
        len -= (eir[0] + 1);
        eir += (eir[0] + 1);
    }

    bt_addr_to_str(addr, br_addr, sizeof(br_addr));
    PRINTF("[%d]:%s,RSSI %i,Type:%d,Name:%s  Cod %06x\r\n", index + 1, br_addr, rssi, device_type, name,cod);
}

void set_br_discover_result_count(size_t count)
{
	g_brDiscoverResultCount = count;
}

uint32_t get_br_discover_result_count()
{
	return g_brDiscoverResultCount;
}

uint8_t get_device_type_from_cod(const uint8_t cod[3])
{
    if (cod == NULL) {
        return INVALID_DEVICE;
    }

    uint8_t major_class = BT_COD_MAJOR_DEVICE_CLASS(cod);
    uint8_t minor_class = BT_COD_MINOR_DEVICE_CLASS(cod);
   //PRINTF("cod:%06x,Major:%02x,Minor:%02x\r\n", cod,major_class, minor_class);

    // Determine device type
    if (major_class == BT_COD_MAJOR_PHONE) {
        return PHONE;
    }
    else if (major_class == BT_COD_MAJOR_AUDIO_VIDEO)
    {
        if (minor_class == BT_COD_MAJOR_AUDIO_VIDEO_MINOR_WEARABLE_HEADSET ||
            minor_class == BT_COD_MAJOR_AUDIO_VIDEO_MINOR_HANDS_FREE ||
			minor_class == BT_COD_MAJOR_AUDIO_VIDEO_MINOR_HEADPHONES || (minor_class == BT_COD_MAJOR_AUDIO_VIDEO_MINOR_LOUDSPEAKER))
        {
            return HEADSET;
        }

    }
    return INVALID_DEVICE;
}

void br_discovery_complete(struct bt_br_discovery_result *results, size_t count)
{
    size_t index;
    uint8_t device_type = INVALID_DEVICE;  // Default INVALID_DEVICE to avoid printing unnecessary devices

    set_br_discover_result_count(count);
    PRINTF("BR/EDR discovery complete :%d\r\n",g_brDiscoverResultCount);

    for (index = 0; index < count; ++index)
    {
        // Validate COD first
        if (!BT_COD_VALID(results[index].cod)) {
            continue;  // Skip invalid COD entries
        }
        device_type = INVALID_DEVICE;

        // Get device type from COD
        device_type = get_device_type_from_cod(results[index].cod);

        // Only print if it's Mobile, Headset, or Speaker
        if (device_type != INVALID_DEVICE)
        {
            br_device_found(index, &results[index].addr, results[index].rssi,results[index].cod, results[index].eir, device_type);
        }
    }
}

void print_paired_devices(void)
{
    PRINTF("Paired Devices List:\n");
    if (g_pairedDeviceCount == 0)
    {
    	check_and_clear_bonded_devices();
        PRINTF("No paired devices found.\n");
        return;
    }

    for (int i = 0; i < g_pairedDeviceCount; i++)
    {
        PRINTF("[%d] Address: %02X:%02X:%02X:%02X:%02X:%02X, Name: %s, Type: %d\n",
               i + 1,
               paired_devices[i].addr[0], paired_devices[i].addr[1], paired_devices[i].addr[2],
               paired_devices[i].addr[3], paired_devices[i].addr[4], paired_devices[i].addr[5],
               paired_devices[i].name, paired_devices[i].device_type);
    }
}

void app_update_last_connected_device(const uint8_t addr[6] , uint8_t device_type)
{

	//Reverse the address bytes (Little Endian → Big Endian)
	uint8_t device_addr[6];
	uint8_t updateInfo =0;

	for (int i = 0; i < 6; i++) {
		device_addr[i] = addr[5 - i];
	}

	for (int i = 0; i < g_pairedDeviceCount; i++)
	{
		if((device_type & 0x0F) == (paired_devices[i].device_type & 0x0F) )
		{

			if (memcmp(paired_devices[i].addr, device_addr, 6) == 0)
			{

				if (!(paired_devices[i].device_type & LAST_CONNECTED_MASK))
						updateInfo =1;

				paired_devices[i].device_type |= LAST_CONNECTED_MASK;

			}else if (paired_devices[i].device_type & LAST_CONNECTED_MASK)
			{
				paired_devices[i].device_type &= ~LAST_CONNECTED_MASK;
				updateInfo =1;
			}
		}
	}

	if(updateInfo)
	{
		PRINTF(" Saving last connected information !\n");
		app_save_paired_devices();
	}

}

void save_new_paired_device(struct bt_conn *conn, uint8_t isRiderHeadset)
{
	struct bt_conn_info conn_info;
	if (bt_conn_get_info(conn, &conn_info) != 0)
	{
		PRINTF("Failed to get connection info.\n");
		return;
	}

	uint8_t addr[6];

	if (conn_info.type == BT_CONN_TYPE_BR) {
		memcpy(addr, conn_info.br.dst->val, 6);
	} else {
		memcpy(addr, conn_info.le.dst->a.val, 6);
	}
	//Reverse the address bytes (Little Endian → Big Endian)
	uint8_t device_addr[6];
	for (int i = 0; i < 6; i++) {
		device_addr[i] = addr[5 - i];
	}

	for (int i = 0; i < g_pairedDeviceCount; i++)
	{
		if (memcmp(paired_devices[i].addr, device_addr, 6) == 0)
	     {
	            return;
	     }
    }

	char name[64] = "Unknown";  // Default name
	uint8_t cod[3] = {0};       // Default COD

	// Try to find name & COD from discovery list
	for (size_t j = 0; j < APP_INQUIRY_NUM_RESPONSES; j++)
	{
		if (bt_addr_cmp(&br_discovery_results[j].addr, (bt_addr_t *)addr) == 0)
		{
			// Extract name from EIR
			int eir_len = 240;
			uint8_t *eir_data = br_discovery_results[j].eir;
			while (eir_len)
			{
				if (eir_len < 2 || eir_data[0] == 0) break;
				if (eir_data[1] == BT_DATA_NAME_COMPLETE || eir_data[1] == BT_DATA_NAME_SHORTENED)
				{
					int copy_len = (eir_data[0] - 1 > sizeof(name) - 1) ? sizeof(name) - 1 : eir_data[0] - 1;
					memcpy(name, &eir_data[2], copy_len);
					name[copy_len] = '\0';  // Ensure null-termination
					break;
				}
				eir_len -= (eir_data[0] + 1);
				eir_data += (eir_data[0] + 1);
			}

			// Retrieve COD
			memcpy(cod, br_discovery_results[j].cod, 3);
			break;
		}
	}

	// Get the device type using COD
	uint8_t device_type = get_device_type_from_cod(cod);


	if (device_type == HEADSET)
	{
		if(isRiderHeadset)
		{
			device_type = RIDER_HEADSET;
		}else
		{
			device_type = PASSENGER_HEADSET;
		}
	}

	//PRINTF("Saving new paired device: Address: %02X:%02X:%02X:%02X:%02X:%02X, Name: %s, Type: %d\n",
	//			addr[0], addr[1], addr[2], addr[3], addr[4], addr[5], name, device_type);

	app_save_paired_device(device_addr, name, device_type);
	PRINTF("Updated Paired Devices list !!\n\n");

	app_read_paired_devices();
}

void read_paired_dev_from_memory()
{
	app_read_paired_devices();
}

int remove_paired_device(const uint8_t addr[6])
{
    if (g_pairedDeviceCount == 0)
    {
        PRINTF("No paired devices to remove.\n");
        return -1;
    }

    int index = -1;
    for (int i = 0; i < g_pairedDeviceCount; i++)
    {
        if (memcmp(paired_devices[i].addr, addr, 6) == 0)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        PRINTF("Device not found in paired list.\n");
        return -1;
    }

    // Shift remaining entries
    for (int i = index; i < g_pairedDeviceCount - 1; i++)
    {
        paired_devices[i] = paired_devices[i + 1];
    }
    g_pairedDeviceCount--;

    // Save updated list to LittleFS
    PRINTF("Device removed from paired list.\n");
    app_save_paired_devices();
    PRINTF("Paired device list:\n");
    app_read_paired_devices();

    return 0;
}

int delete_device(int index)
{
    if (g_pairedDeviceCount == 0)
    {
        PRINTF("No paired devices to remove.\n");
        return -1;
    }

    if (index < 1 || index > g_pairedDeviceCount)
    {
        PRINTF("Invalid device index! Choose between 1 and %d\n", g_pairedDeviceCount);
        return -1;
    }

    // Convert 1-based index to 0-based array index
    int actual_index = index - 1;

    PRINTF("Removing device [%d] Address: %02X:%02X:%02X:%02X:%02X:%02X\n",
           index,
           paired_devices[actual_index].addr[0], paired_devices[actual_index].addr[1],
           paired_devices[actual_index].addr[2], paired_devices[actual_index].addr[3],
           paired_devices[actual_index].addr[4], paired_devices[actual_index].addr[5]);

    //Step 1: Unpair the device
    bt_addr_le_t addr;
    addr.type = BT_ADDR_LE_PUBLIC;
    memcpy(addr.a.val, paired_devices[actual_index].addr, 6);

    PRINTF("Removing device [%d] Address: %02X:%02X:%02X:%02X:%02X:%02X\n",
           index,
		   addr.a.val[0], addr.a.val[1],addr.a.val[2], addr.a.val[3],
		   addr.a.val[4], addr.a.val[5]);

    int ret = bt_unpair(BT_ID_DEFAULT, &addr);
    if (ret == 0)
    {
        PRINTF("Device unpaired successfully.\n");
    }
    else
    {
        PRINTF("Failed to unpair device.\n");
    }

    //Step 2: Shift remaining entries to remove the device
    for (int i = actual_index; i < g_pairedDeviceCount - 1; i++)
    {
        paired_devices[i] = paired_devices[i + 1];
    }
    g_pairedDeviceCount--;

    vTaskDelay(pdMS_TO_TICKS(50));
    //Step 3: Save the updated paired list to LittleFS
    app_save_paired_devices();

    PRINTF("Device removed from paired list and LFS updated.\n");

    return 0;
}
void connect_paired_device(uint8_t device_index)
{
	uint8_t device_addr[6] = {0};
	if (device_index < 1 || device_index > g_pairedDeviceCount)
	{
		PRINTF("Invalid device index! Choose between 1 and %d\n", g_pairedDeviceCount);
		return;
	}
	//Get the correct paired device
	uint8_t *addr = paired_devices[device_index - 1].addr;

	PRINTF("Connecting to [%d] Address: %02X:%02X:%02X:%02X:%02X:%02X, Name: %s, Type: %d\n",
			device_index,
			addr[0], addr[1], addr[2], addr[3], addr[4], addr[5],
			paired_devices[device_index - 1].name, paired_devices[device_index - 1].device_type);

	//Reverse the address bytes (Little Endian → Big Endian)
	for (int i = 0; i < 6; i++) {
		device_addr[i] = addr[5 - i];
	}
	uint8_t dev_type = paired_devices[device_index - 1].device_type;
	//Call the correct connection function based on device type
	if ( (dev_type & 0x0F) == RIDER_PHONE)
	{
		app_connect(RIDER_PHONE, device_addr);
	}
	else if ( (dev_type & 0x0F) == RIDER_HEADSET)
	{
		app_connect(RIDER_HEADSET, device_addr);

	}else if ( (dev_type & 0x0F) == PASSENGER_HEADSET)
	{
		app_connect(PASSENGER_HEADSET, device_addr);

	} else
	{
		PRINTF("Failed, Invalid device type !\n");
	}
}

void app_auto_connect_device(int device_type)
{

	int device_index=0;
	uint8_t device_addr[6] = {0};
	uint8_t *addr = NULL;

    if (g_pairedDeviceCount == 0)
    {
        PRINTF("No paired devices found.\n");
        return;
    }

    for (int i = 0; i < g_pairedDeviceCount; i++)
    {
		uint8_t dev_type = paired_devices[i].device_type;
    	if (( dev_type & 0x0F) == device_type  && (dev_type & LAST_CONNECTED_MASK) )
    	{
    		device_index=i+1;
    		addr = paired_devices[i].addr;
    		break;
    	}

    }

    if (!device_index)
    {
    	app_auto_connect_paired_devices();
    	return;
    }

	PRINTF("Connecting to [%d] Address: %02X:%02X:%02X:%02X:%02X:%02X, Name: %s, Type: %d\n",
			device_index,
			addr[0], addr[1], addr[2], addr[3], addr[4], addr[5],
			paired_devices[device_index - 1].name, paired_devices[device_index - 1].device_type);

	//Reverse the address bytes (Little Endian → Big Endian)
	for (int i = 0; i < 6; i++)
	{
		device_addr[i] = addr[5 - i];
	}

	app_connect(device_type, device_addr);

}

bool is_valid_device(uint8_t *addr)
{
    for (size_t i = 0; i < APP_INQUIRY_NUM_RESPONSES; i++)
    {
        if (bt_addr_cmp(&br_discovery_results[i].addr, (bt_addr_t *)addr) == 0)
        {
            uint8_t device_type = get_device_type_from_cod(br_discovery_results[i].cod);

            //Allow only Mobile Phone or Headset
            if ((device_type == PHONE) || (device_type == HEADSET))
            {
                return true;  //Valid device
            }
            return false; //Invalid device
        }
    }

    return false; //Device not found
}

static void bond_info(const struct bt_bond_info *info, void *user_data)
{
	bool *valid = (bool *)user_data;
	*valid = true;
}

void check_and_clear_bonded_devices()
{
	PRINTF("LFS is empty. Checking bonded devices...\n");
	bool valid = false;
	//Scan for bonded devices
	bt_foreach_bond(BT_ID_DEFAULT, bond_info, &valid);
	if (valid)
	{
		PRINTF("Bonded devices found. Removing all...\n");
		//Remove all bonded devices
		bt_unpair(BT_ID_DEFAULT, NULL);
		//Clear LFS storage to sync
		//clear_paired_devices_from_lfs();
	}
	else
	{
		PRINTF("No bonded devices found. Nothing to remove.\n");
	}
}

void hfp_dial(const char *number)
{
    bt_hfp_hf_dial(conn_rider_phone, number);
}

void hfp_last_dial(void)
{
    bt_hfp_hf_last_dial(conn_rider_phone);
}

