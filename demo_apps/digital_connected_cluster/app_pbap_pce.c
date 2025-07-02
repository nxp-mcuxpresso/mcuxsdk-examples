/*
 * Copyright 2024-2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"

#include "FreeRTOS.h"
#include "task.h"

#include <porting.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/atomic.h>
#include <sys/byteorder.h>
#include <sys/util.h>
#include <sys/slist.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include <bluetooth/l2cap.h>
#include <bluetooth/sdp.h>
#include <bluetooth/pbap_pce.h>
#include "BT_common.h"
#include "BT_pbap_api.h"
#include "db_gen.h"
#include "app_pbap_pce.h"
#include "app_connect.h"
#include "app_discover.h"
#include "app_map_mce.h"
#include "mbedtls/base64.h"


#define SDP_CLIENT_USER_BUF_LEN 512U
NET_BUF_POOL_FIXED_DEFINE(pbap_sdp_client_pool, CONFIG_BT_MAX_CONN, SDP_CLIENT_USER_BUF_LEN, CONFIG_NET_BUF_USER_DATA_SIZE, NULL);
NET_BUF_POOL_FIXED_DEFINE(pbap_appl_pool, CONFIG_BT_MAX_CONN, CONFIG_BT_PBAP_PCE_MAX_PKT_LEN, CONFIG_NET_BUF_USER_DATA_SIZE, NULL);

#define CONFIG_BT_PRINTF_VALUE 1

#define PBAP_SET_PATH_CHILD_FLAG  "./"
#define PBAP_SET_PATH_ROOT_FLAG   "/"
#define PBAP_SET_PATH_PARENT_FLAG ".."

#define PBAP_PULL_PHONEBOOK_DEMO_NAME     "telecom/pb.vcf"
#define PBAP_PULL_VCARD_LISTING_MCH_DEMO_NAME "telecom/mch.vcf"
#define PBAP_PULL_VCARD_LISTING_ICH_DEMO_NAME "telecom/ich.vcf"
#define PBAP_PULL_VCARD_LISTING_OCH_DEMO_NAME "telecom/och.vcf"
#define PBAP_PULL_VCARD_ENTRY_DEMO_NAME   "4.vcf"

#define PBAP_ROOT_PATH                        "root"
#define PBAP_PHONEBOOK_DEMO_RELATIVE_PATH     "telecom"
//#define PBAP_VCARD_LISTING_DEMO_RELATIVE_PATH "cch"
#define PBAP_VCARD_LISTING_MCH_DEMO_RELATIVE_PATH "mch"
#define PBAP_VCARD_LISTING_ICH_DEMO_RELATIVE_PATH "ich"
#define PBAP_VCARD_LISTING_OCH_DEMO_RELATIVE_PATH "och"
#define PBAP_VCARD_LISTING_ABSOLUTE_PATH      "root/telecom"
#define PBAP_VCARD_LISTING_MCH_ABSOLUTE_PATH      "root/telecom/mch"
#define PBAP_VCARD_LISTING_ICH_ABSOLUTE_PATH      "root/telecom/ich"
#define PBAP_VCARD_ENTRY_ABSOLUTE_PATH           "root/telecom/och"
//#define PBAP_VCARD_ENTRY_ABSOLUTE_PATH        "root/telecom/cch"


#define PBAP_CLASS_OF_DEVICE (0x10020CU)/* Object Transfer, Phone, Smartphone */

app_pbap_pce_t g_PbapPce;

bool g_pbapConnected = 0;
int count = 0;

/*Added Below code for the PBAP entry store*/
#define MAX_CONTACTS 		400
#define MAX_MISSED_CALL 	20
#define MAX_INCOMING_CALL 	20
#define MAX_OUTGOING_CALL 	20

typedef struct {
    char name[100];
    char phone[15];
} contact_t;

typedef struct {
    char name[100];
    char phone[15];
} missedcall_t;

typedef struct {
    char name[100];
    char phone[15];
} incall_t;

typedef struct {
    char name[100];
    char phone[15];
} outcall_t;

static contact_t contact_list[MAX_CONTACTS][MAX_PHONE_CONNECTION];
static missedcall_t misscall_list[MAX_MISSED_CALL][MAX_PHONE_CONNECTION];
static incall_t incall_list[MAX_INCOMING_CALL][MAX_PHONE_CONNECTION];
static outcall_t outcall_list[MAX_OUTGOING_CALL][MAX_PHONE_CONNECTION];
static int g_contactCount = 0, g_misscallCount = 0, g_incallCount = 0, g_outcallCount = 0;
static int g_infoType = 0;
uint8_t g_contactIndex = 0;

/** @brief PBAP PCE structure */
struct bt_pbap_pce
{
    uint8_t pbap_handle;
    struct bt_conn *acl_conn;
    uint16_t goep_version;
    uint16_t max_pkt_len;
    struct bt_pbap_auth *auth;
    uint32_t peer_feature;
    uint32_t local_feature;
    bool lcl_wait;
    bool rem_wait;
}*app_pbap_pce[MAX_PHONE_CONNECTION];;
/**********End Here**************************/

static struct bt_sdp_attribute pbap_pce_attrs[] = {
    BT_SDP_NEW_SERVICE,
        BT_SDP_LIST(
        BT_SDP_ATTR_SVCLASS_ID_LIST,
        /* ServiceClassIDList */
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 3), //35 03
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
            BT_SDP_ARRAY_16(BT_SDP_PBAP_PCE_SVCLASS) //11 2E
        },
        )
    ),
    BT_SDP_LIST(
        BT_SDP_ATTR_PROFILE_DESC_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 8), //35 08
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6), //35 06
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                BT_SDP_ARRAY_16(BT_SDP_PBAP_SVCLASS) //11 30
            },
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UINT16), //09
                BT_SDP_ARRAY_16(0x0102U) //01 02
            },
            )
        },
        )
    ),
    BT_SDP_SERVICE_NAME("Phonebook Access PCE"),
};

static struct bt_sdp_record pbap_pce_rec = BT_SDP_RECORD(pbap_pce_attrs);

struct pbap_hdr
{
    uint8_t *value;
    uint16_t length;
};

void clear_pbap_data(int index)
{
	if(app_pbap_pce[index] != NULL)
	{
		PRINTF("Deleting Pbap contact list...\n");
		for(g_contactCount = MAX_CONTACTS-1; g_contactCount >= 0;  g_contactCount--)
		{
			contact_list[g_contactCount][index].name[0] = '\0';
			contact_list[g_contactCount][index].phone[0] = '\0';
		}

		for(g_misscallCount = MAX_MISSED_CALL-1; g_misscallCount >= 0;  g_misscallCount--)
		{
			misscall_list[g_misscallCount][index].name[0] = '\0';
			misscall_list[g_misscallCount][index].phone[0] = '\0';
		}

		for(g_incallCount = MAX_INCOMING_CALL-1; g_incallCount >= 0;  g_incallCount--)
		{
			incall_list[g_incallCount][index].name[0] = '\0';
			incall_list[g_incallCount][index].phone[0] = '\0';
		}

		for(g_outcallCount = MAX_OUTGOING_CALL-1; g_outcallCount >= 0;  g_outcallCount--)
		{
			outcall_list[g_outcallCount][index].name[0] = '\0';
			outcall_list[g_outcallCount][index].phone[0] = '\0';
		}

		g_contactCount = 0;
		g_misscallCount = 0;
		g_incallCount = 0;
		g_outcallCount = 0;
		g_contactIndex--;
		app_pbap_pce[index] = NULL;
	}
	return;
}

static void pbap_app_connected(struct bt_pbap_pce *pbap_pce)
{
	PRINTF("PBAP client connected\r\n");
	memcpy(g_PbapPce.currentpath, PBAP_ROOT_PATH, strlen(PBAP_ROOT_PATH));
	/*Need to set the pbap connection flag to set*/
	g_pbapConnected = 1;
	for(int index = 0; index < MAX_PHONE_CONNECTION; index++)
	{
		if(app_pbap_pce[index] == NULL)
		{
			app_pbap_pce[index] = pbap_pce;
			g_contactIndex++;
			break;
		}
	}
}

static void app_get_auth_info_cb(struct bt_pbap_pce *pbap_pce, struct bt_pbap_auth *pbap_atuh_info)
{
	return;
}

static void pbap_app_disconnected(struct bt_pbap_pce *pbap_pce, uint8_t result)
{
	PRINTF("PBAP disconnect successfully: %x\r\n", result);
	if (result == BT_PBAP_FORBIDDEN_RSP)
	{
		PRINTF("Possible reasons is Authentication failure\r\n");
	}
}

static void app_error_result(uint8_t result)
{
	PRINTF("PBAP app_error_result - 0x%02Xr\n", result);
	switch (result)
	{
	case BT_PBAP_BAD_REQ_RSP:
		PRINTF("Function not recognized or ill-formatted: %x\r\n", result);
		break;
	case BT_PBAP_NOT_IMPLEMENTED_RSP:
		PRINTF("Function recognized but not supported: %x\r\n", result);
		break;
	case BT_PBAP_UNAUTH_RSP:
		PRINTF("Access is not authorized: %x\r\n", result);
		break;
	case BT_PBAP_PRECOND_FAILED_RSP:
		PRINTF("One of request parameter is wrong: %x\r\n", result);
		break;
	case BT_PBAP_NOT_FOUND_RSP:
		PRINTF("object not found: %x\r\n", result);
		break;
	case BT_PBAP_NOT_ACCEPTABLE_RSP:
		PRINTF("PSE can not meet one of request parameter: %x\r\n", result);
		break;
	case BT_PBAP_NO_SERVICE_RSP:
		PRINTF("System condition prevents it: %x\r\n", result);
		break;
	case BT_PBAP_FORBIDDEN_RSP:
		PRINTF("Temporarily barred: %x\r\n", result);
		break;
	}
	return;
}

static void parse_vcard(const char *data, size_t len) {
    const char *ptr = data;
    char name[100] = {0};
    char phone[20] = {0};
    while ((ptr = strstr(ptr, "BEGIN:VCARD")) != NULL) {
        if (sscanf(ptr, "N:%99[^\n]", name) > 0 &&
            sscanf(ptr, "TEL;CELL:%19[^\n]", phone) > 0) {
            PRINTF("Contact: %s, Phone: %s\n", name, phone);
        }
        ptr++;  // Move to next occurrence
    }
}

static void store_pbap_data(char *line_start)
{
    char *delimiter_temp = NULL;
	if (strncmp(line_start, "FN", 2) == 0) {
		switch(g_infoType) {
			case 0:
				if((g_contactCount < MAX_CONTACTS) && (delimiter_temp = strchr(line_start,':')))
					strncpy(contact_list[g_contactCount][g_contactIndex - 1].name, delimiter_temp + 1, sizeof(contact_list[g_contactCount][g_contactIndex - 1].name) - 1);
			break;
			case 1:
				if((g_misscallCount < MAX_MISSED_CALL) && (delimiter_temp = strchr(line_start,':')))
					strncpy(misscall_list[g_misscallCount][g_contactIndex - 1].name, delimiter_temp + 1, sizeof(misscall_list[g_misscallCount][g_contactIndex - 1].name) - 1);
			break;
			case 2:
				if((g_incallCount < MAX_INCOMING_CALL) && (delimiter_temp = strchr(line_start,':')))
					strncpy(incall_list[g_incallCount][g_contactIndex - 1].name, delimiter_temp + 1, sizeof(incall_list[g_incallCount][g_contactIndex - 1].name) - 1);
			break;
			case 3:
				if((g_outcallCount < MAX_OUTGOING_CALL) && (delimiter_temp = strchr(line_start,':')))
					strncpy(outcall_list[g_outcallCount][g_contactIndex - 1].name, delimiter_temp + 1, sizeof(outcall_list[g_outcallCount][g_contactIndex - 1].name) - 1);
			break;
			default:
				PRINTF("Invalid call data input\n");
			break;
		}
	}
	else if (strncmp(line_start, "TEL", 3) == 0) {
		switch(g_infoType) {
			case 0:
				if((g_contactCount < MAX_CONTACTS)
						&& (delimiter_temp = strchr(line_start,':'))
						&& ((strncmp(contact_list[g_contactCount - 1][g_contactIndex - 1].phone, delimiter_temp + 1, sizeof(contact_list[g_contactCount - 1][g_contactIndex - 1].phone) - 1))
								|| (g_contactCount == 0)))
				{
					strncpy(contact_list[g_contactCount][g_contactIndex - 1].phone, delimiter_temp + 1, sizeof(contact_list[g_contactCount][g_contactIndex - 1].phone) - 1);
					g_contactCount++;
				}
			break;
			case 1:
				if((g_misscallCount < MAX_MISSED_CALL)
						&& (delimiter_temp = strchr(line_start,':'))
						&& ((strncmp(contact_list[g_misscallCount - 1][g_contactIndex - 1].phone, delimiter_temp + 1, sizeof(contact_list[g_misscallCount - 1][g_contactIndex - 1].phone) - 1))
								|| (g_misscallCount == 0)))
				{
					strncpy(misscall_list[g_misscallCount][g_contactIndex - 1].phone, delimiter_temp + 1, sizeof(misscall_list[g_misscallCount][g_contactIndex - 1].phone) - 1);
					g_misscallCount++;
				}
			break;
			case 2:
				if((g_incallCount < MAX_INCOMING_CALL)
						&& (delimiter_temp = strchr(line_start,':'))
						&& ((strncmp(contact_list[g_incallCount - 1][g_contactIndex - 1].phone, delimiter_temp + 1, sizeof(contact_list[g_incallCount - 1][g_contactIndex - 1].phone) - 1))
								|| (g_incallCount == 0)))
				{
					strncpy(incall_list[g_incallCount][g_contactIndex - 1].phone, delimiter_temp + 1, sizeof(incall_list[g_incallCount][g_contactIndex - 1].phone) - 1);
					g_incallCount++;
				}
			break;
			case 3:
				if((g_outcallCount < MAX_OUTGOING_CALL)
						&& (delimiter_temp = strchr(line_start,':'))
						&& ((strncmp(contact_list[g_outcallCount - 1][g_contactIndex - 1].phone, delimiter_temp + 1, sizeof(contact_list[g_outcallCount - 1][g_contactIndex - 1].phone) - 1))
								|| (g_outcallCount == 0)))
				{
					strncpy(outcall_list[g_outcallCount][g_contactIndex - 1].phone, delimiter_temp + 1, sizeof(outcall_list[g_outcallCount][g_contactIndex - 1].phone) - 1);
					g_outcallCount++;
				}
			break;
			default:
				PRINTF("Invalid call data input\n");
			break;
		}
		return;
	}
}

static void parse_vcard_entries(char *vcf_data )
{
    char *line_start = vcf_data;
    char *delimiter_temp=NULL;

    while (line_start && *line_start) {
        char *line_end = strstr(line_start, "\r\n");
        if (!line_end)
            break;

        *line_end = '\0';  // Temporarily null-terminate
        store_pbap_data(line_start);
        line_start = line_end + 2;
    }
}

static void print_pbap_data(int index, int type)
{
	switch(type) {
		case 0:
			for (int i = 0; i < g_contactCount; i++)
			{
				if(contact_list[i][index].name != NULL)
				{
					PRINTF("Contact %d: %s, %s\n", i + 1, contact_list[i][index].name, contact_list[i][index].phone);
				}
			}
		break;
		case 1:
			for (int i = 0; i < g_misscallCount; i++)
			{
				if(misscall_list[i][index].name != NULL)
				{
					PRINTF("Contact %d: %s, %s\n", i + 1, misscall_list[i][index].name, misscall_list[i][index].phone);
				}
			}
		break;
		case 2:
			for (int i = 0; i < g_incallCount; i++)
			{
				if(incall_list[i][index].name != NULL)
				{
					PRINTF("Contact %d: %s, %s\n", i + 1, incall_list[i][index].name, incall_list[i][index].phone);
				}
			}
		break;
		case 3:
			for (int i = 0; i < g_outcallCount; i++)
			{
				if(outcall_list[i][index].name != NULL)
				{
					PRINTF("Contact %d: %s, %s\n", i + 1, outcall_list[i][index].name, outcall_list[i][index].phone);
				}
			}
		break;
		default:
			PRINTF("Invalid call data input\n");
		break;
	}
}

void print_contact_list(int type)
{
    for (int index = 0; index < MAX_PHONE_CONNECTION; index++)
	{
		if(app_pbap_pce[index] != NULL)
		{
			PRINTF("========= Stored Contacts for connection:%d =========\n",index+1);
			print_pbap_data(index, type);
			PRINTF("========= End of Contact List for connection:%d =====\n",index+1);
		}
	}
}

static void app_print_body(struct net_buf *buf)
{
    struct pbap_hdr body;
    if (bt_pbap_pce_get_body(buf, &body.value, &body.length) != 0 || body.length == 0)
    {
        PRINTF("No valid body data found.\n");
        return;
    }

    // Allocate memory for processing
    char *vcf_data = (char *)malloc(body.length + 1);
    if (!vcf_data)
    {
        PRINTF("Memory allocation failed for vCard body processing\n");
        return;
    }
    memcpy(vcf_data, body.value, body.length);
    vcf_data[body.length] = '\0';
#ifdef APP_DEBUG_EN
    	//PRINTF("======= Extracted vCard Body =======\n%s\n", vcf_data);
#endif

    // **Call function to extract contacts**
    parse_vcard_entries(vcf_data);

    free(vcf_data);
}

static bool app_app_param_cb(struct bt_data *data, void *user_data)
{
	PRINTF("app_app_param_cb - 0x%02Xr\n", data->type);
	switch (data->type)
	{
	case BT_PBAP_TAG_ID_PHONE_BOOK_SIZE:
		PRINTF("Phonebook Size - %d\r\n", sys_get_be16(data->data));
		break;

	case BT_PBAP_TAG_ID_NEW_MISSED_CALLS:
		PRINTF("New Missed Calls -%d\r\n", data->data[0]);
		break;

	case BT_PBAP_TAG_ID_PRIMARY_FOLDER_VERSION:
		PRINTF("Primary Floder Version - ");
		for (uint8_t index = 0; index < data->data_len; index++)
		{
			PRINTF("%02X", data->data[index]);
		}
		PRINTF("\r\n");
		break;

	case BT_PBAP_TAG_ID_SECONDARY_FOLDER_VERSION:
		PRINTF("Secondary Floder Version - ");
		for (uint8_t index = 0; index < data->data_len; index++)
		{
			PRINTF("%02X", data->data[index]);
		}
		PRINTF("\r\n");
		break;

	case BT_PBAP_TAG_ID_DATABASE_IDENTIFIER:
		PRINTF("Database Identifier - ");
		for (uint8_t index = 0; index < data->data_len; index++)
		{
			PRINTF("%02X", data->data[index]);
		}
		PRINTF("\r\n");
		break;
	}
	return true;
}

void pbap_download_status_infotoApplication(pbap_download_status_t status)
{
	PRINTF("Download Status: ");
	switch (status)
	{
	case PBAP_DOWNLOAD_FAILED:
		PRINTF("failed infotoApplication.\n");
		break;
	case PBAP_DOWNLOAD_IN_PROGRESS:
		PRINTF("in progress infotoApplication.\n");
		break;
	case PBAP_DOWNLOAD_SUCCESSFUL:
		PRINTF("successful infotoApplication.\n");
		break;
	default:
		PRINTF("unknown status infotoApplication.\n");
		break;
	}
}

void pbap_contacts_list_infotoApplication(uint16_t length, uint8_t *value)
{
	PRINTF("\nPULLED PHONEBOOK LIST infotoApplication\n");
	PRINTF("\n----------------------------********------------------------------\r\n");
	PRINTF("Body length: %zu\r\n",length);
	//PRINTF("Body content: %.*s\r\n", length, value);
	PRINTF("\n----------------------------********------------------------------\r\n");
}

static void app_pull_phonebook_cb(struct bt_pbap_pce *pbap_pce, uint8_t result, struct net_buf *buf)
{
	int revert;
	int revert1;
	static uint8_t pbap_in_progress = 0;

#ifdef APP_DEBUG_EN
	//PRINTF("app_pull_phonebook_cb - 0x%02Xr\n", result);
#endif
	if (result != PBAP_CONTINUE_RSP && result != BT_PBAP_SUCCESS_RSP)
	{
		app_error_result(result);
		net_buf_unref(buf);
		return;
	}
	else
	{
#ifdef APP_DEBUG_EN
		//PRINTF("Pull phonebook result - 0x%02X\r\n", result);
#endif
	}
	bt_pbap_pce_app_param_parse(buf, app_app_param_cb, NULL);

	struct pbap_hdr body;
	if (bt_pbap_pce_get_body(buf, &body.value, &body.length) == 0)
	{
		if ((body.length > 0) && (!pbap_in_progress))
		{
			pbap_in_progress = 1;
			pbap_download_status_infotoApplication(PBAP_DOWNLOAD_IN_PROGRESS);
		}
	}
	app_print_body(buf);
	net_buf_unref(buf);

	if (result == PBAP_CONTINUE_RSP)
	{
		g_PbapPce.lcl_srmp_wait = (--g_PbapPce.num_srmp_wait) > 0 ? true : false;

		buf = net_buf_alloc(&pbap_appl_pool, osaWaitNone_c);
		if (buf == NULL)
		{
			return;
		}
		net_buf_reserve(buf, BT_PBAP_PCE_RSV_LEN_PULL_PHONEBOOK(g_PbapPce.pbap_pceHandle, BT_OBEX_REQ_END));

		revert = bt_pbap_pce_pull_phonebook(g_PbapPce.pbap_pceHandle, buf, NULL, g_PbapPce.lcl_srmp_wait,
				BT_OBEX_REQ_END);
		if (revert != 0)
		{
			pbap_in_progress = 0;
			net_buf_unref(buf);
			PRINTF("pb_download callback  failed %x", result);
			pbap_download_status_infotoApplication(PBAP_DOWNLOAD_FAILED);
		}
	}
	else if (result == BT_PBAP_SUCCESS_RSP)
	{
		pbap_in_progress = 0;
    	pbap_download_status_infotoApplication(PBAP_DOWNLOAD_SUCCESSFUL);
    	print_contact_list(g_infoType);
#if 0
		count ++;
		/*revert = app_set_phonebook_path(PBAP_SET_PATH_CHILD_FLAG PBAP_PHONEBOOK_DEMO_RELATIVE_PATH);
        if (revert < 0)
        {
            PRINTF("set path command send failed\r\n");
            return;
        }
        memcpy(g_PbapPce.currentpath, PBAP_VCARD_LISTING_ABSOLUTE_PATH, strlen(PBAP_VCARD_LISTING_ABSOLUTE_PATH) + 1);*/
		if (count == 1) {
			PRINTF("app_pull_vcard_listing mch\r\n");
			revert1 = app_pull_phonebook(PBAP_PULL_VCARD_LISTING_MCH_DEMO_NAME);
		} else if (count == 2) {
			PRINTF("app_pull_vcard_listing ich\r\n");
			revert1 = app_pull_phonebook(PBAP_PULL_VCARD_LISTING_ICH_DEMO_NAME);
		} else if (count == 3) {
			PRINTF("app_pull_vcard_listing och\r\n");
			revert1 = app_pull_phonebook(PBAP_PULL_VCARD_LISTING_OCH_DEMO_NAME);
		} else {
			//nothing
		}
#endif
	}
	return;
}

static void app_set_phonebook_path_cb(struct bt_pbap_pce *pbap_pce, uint8_t result)
{
	PRINTF("app_set_phonebook_path_cb - 0x%02Xr\n", result);
	int revert = 0;
	switch (result)
	{
	case BT_PBAP_SUCCESS_RSP:
		PRINTF("pbap pse path set success\r\n");
		break;

	default:
		PRINTF("pbap pse path set fail %x\r\n", result);
		return;
	}
	if (strcmp(g_PbapPce.currentpath, PBAP_VCARD_LISTING_ABSOLUTE_PATH) == 0)
	{
		PRINTF("app_pull_vcard_listing mch\r\n");
		revert = app_pull_vcard_listing(PBAP_PULL_VCARD_LISTING_MCH_DEMO_NAME);
	}
	else if (strcmp(g_PbapPce.currentpath, PBAP_VCARD_LISTING_MCH_ABSOLUTE_PATH) == 0)
	{
		PRINTF("app_pull_vcard_listing ich\r\n");
		revert = app_pull_vcard_listing(PBAP_PULL_VCARD_LISTING_ICH_DEMO_NAME);
	}
	else if (strcmp(g_PbapPce.currentpath, PBAP_VCARD_LISTING_ICH_ABSOLUTE_PATH) == 0)
	{
		PRINTF("app_pull_vcard_listing och\r\n");
		revert = app_pull_vcard_listing(PBAP_PULL_VCARD_LISTING_OCH_DEMO_NAME);
	}
	else if (strcmp(g_PbapPce.currentpath, PBAP_VCARD_ENTRY_ABSOLUTE_PATH) == 0)
	{
		PRINTF("app_pull_vcard_entry 4\r\n");
		revert = app_pull_vcard_entry(PBAP_PULL_VCARD_ENTRY_DEMO_NAME);
	}
	else if (strcmp(g_PbapPce.currentpath, PBAP_ROOT_PATH) == 0)
	{
		PRINTF("bt_pbap_pce_disconnect root\r\n");
		revert = bt_pbap_pce_disconnect(pbap_pce);
	}

	if (revert < 0)
	{
		PRINTF("set path command send failed\r\n");
		return;
	}
}

static void app_pull_vcard_listing_cb(struct bt_pbap_pce *pbap_pce, uint8_t result, struct net_buf *buf)
{
	PRINTF("app_pull_vcard_listing_cb - 0x%02Xr\n", result);
	int revert;
	if (result != PBAP_CONTINUE_RSP && result != BT_PBAP_SUCCESS_RSP)
	{
		PRINTF("result=%d,result=!= PBAP_CONTINUE_RSP && result != BT_PBAP_SUCCESS_RSP",result);
		app_error_result(result);
		net_buf_unref(buf);
		return;
	}
	else
	{
		PRINTF("pull vcard listing result - 0x%02X\r\n", result);
	}
	bt_pbap_pce_app_param_parse(buf, app_app_param_cb, NULL);
	app_print_body(buf);
	net_buf_unref(buf);

	if (result == BT_PBAP_CONTINUE_RSP)
	{
		if (result == BT_PBAP_CONTINUE_RSP)
		{
			if ((g_PbapPce.goep_version == BT_GOEP_VERSION_1_1 || g_PbapPce.rem_srmp_wait || g_PbapPce.lcl_srmp_wait))
			{
				g_PbapPce.lcl_srmp_wait = --g_PbapPce.num_srmp_wait > 0 ? true : false;
				buf = net_buf_alloc(&pbap_appl_pool, osaWaitNone_c);
				if (buf == NULL)
				{
					return;
				}
				net_buf_reserve(buf, BT_PBAP_PCE_RSV_LEN_PULL_VCARD_LISTING(g_PbapPce.pbap_pceHandle, BT_OBEX_REQ_END));

				revert = bt_pbap_pce_pull_vcard_listing(g_PbapPce.pbap_pceHandle, buf, NULL, g_PbapPce.lcl_srmp_wait, BT_OBEX_REQ_END);
				if (revert != 0)
				{
					net_buf_unref(buf);
					PRINTF("pull_vcard_listing callback failed %x", result);
				}
			}
		}
	}
	else if (result == BT_PBAP_SUCCESS_RSP)
	{
		if (strcmp(g_PbapPce.currentpath, PBAP_VCARD_LISTING_ABSOLUTE_PATH) == 0)
		{
			revert = app_set_phonebook_path(PBAP_SET_PATH_CHILD_FLAG PBAP_VCARD_LISTING_MCH_DEMO_RELATIVE_PATH);
			if (revert < 0)
			{
				PRINTF("MCH set path command send failed\r\n");
				return;
			}
			PRINTF("MCH set path command send success\r\n");
			memcpy(g_PbapPce.currentpath, PBAP_VCARD_LISTING_MCH_ABSOLUTE_PATH, BT_str_len(PBAP_VCARD_LISTING_MCH_ABSOLUTE_PATH) + 1);
		}
		else if (strcmp(g_PbapPce.currentpath, PBAP_VCARD_LISTING_MCH_ABSOLUTE_PATH) == 0)
		{
			revert = app_set_phonebook_path(PBAP_SET_PATH_CHILD_FLAG PBAP_VCARD_LISTING_ICH_DEMO_RELATIVE_PATH);
			if (revert < 0)
			{
				PRINTF("ICH set path command send failed\r\n");
				return;
			}
			PRINTF("ICH set path command send success\r\n");
			memcpy(g_PbapPce.currentpath, PBAP_VCARD_LISTING_ICH_ABSOLUTE_PATH, BT_str_len(PBAP_VCARD_LISTING_ICH_ABSOLUTE_PATH) + 1);
		}
		else if (strcmp(g_PbapPce.currentpath, PBAP_VCARD_LISTING_ICH_ABSOLUTE_PATH) == 0)
		{
			revert = app_set_phonebook_path(PBAP_SET_PATH_CHILD_FLAG PBAP_VCARD_LISTING_OCH_DEMO_RELATIVE_PATH);
			if (revert < 0)
			{
				PRINTF("OCH set path command send failed\r\n");
				return;
			}
			PRINTF("OCH set path command send success\r\n");
			memcpy(g_PbapPce.currentpath, PBAP_VCARD_ENTRY_ABSOLUTE_PATH, strlen(PBAP_VCARD_ENTRY_ABSOLUTE_PATH) + 1);
		}
	}
	return;
}

static void app_pull_vcard_entry_cb(struct bt_pbap_pce *pbap_pce, uint8_t result, struct net_buf *buf)
{
	PRINTF("app_pull_vcard_entry_cb - 0x%02Xr\n", result);
	int revert;
	if (result != PBAP_CONTINUE_RSP && result != BT_PBAP_SUCCESS_RSP)
	{
		app_error_result(result);
		net_buf_unref(buf);
		return;
	}
	else
	{
		PRINTF("pull vcard listing result - 0x%02X\r\n", result);
	}
	bt_pbap_pce_app_param_parse(buf, app_app_param_cb, NULL);
	app_print_body(buf);
	net_buf_unref(buf);

	if (result == BT_PBAP_CONTINUE_RSP)
	{
		if ((g_PbapPce.goep_version == BT_GOEP_VERSION_1_1 || g_PbapPce.rem_srmp_wait || g_PbapPce.lcl_srmp_wait))
		{
			g_PbapPce.lcl_srmp_wait = --g_PbapPce.num_srmp_wait > 0 ? true : false;
			buf = net_buf_alloc(&pbap_appl_pool, osaWaitNone_c);
			if (buf == NULL)
			{
				return;
			}
			net_buf_reserve(buf, BT_PBAP_PCE_RSV_LEN_PULL_VCARD_ENTRY(g_PbapPce.pbap_pceHandle, BT_OBEX_REQ_END));

			revert = bt_pbap_pce_pull_vcard_entry(g_PbapPce.pbap_pceHandle, buf, NULL, g_PbapPce.lcl_srmp_wait, BT_OBEX_REQ_END);
			if (revert != 0)
			{
				net_buf_unref(buf);
				PRINTF("pull_vcard_entry callback failed %x", result);
			}
		}
		else if (result == BT_PBAP_SUCCESS_RSP)
		{
			revert = app_set_phonebook_path(PBAP_SET_PATH_ROOT_FLAG);
			if (revert < 0)
			{
				PRINTF("set path command send failed\r\n");
				return;
			}
			memcpy(g_PbapPce.currentpath, PBAP_ROOT_PATH, strlen(PBAP_ROOT_PATH) + 1);
		}
	}
}

static struct bt_pbap_pce_cb pce_cb = {
	.connected     = pbap_app_connected,
	.disconnected  = pbap_app_disconnected,
	.get_auth_info = app_get_auth_info_cb,
	.pull_phonebook     = app_pull_phonebook_cb,
	.set_phonebook_path = app_set_phonebook_path_cb,
	.pull_vcard_listing = app_pull_vcard_listing_cb,
	.pull_vcard_entry   = app_pull_vcard_entry_cb,
};


int app_pull_phonebook(char *name)
{
	PRINTF("app_pull_phonebook - %s\n", name);
	API_RESULT retval = 0;
	struct net_buf *buf;

	if (name == NULL)
	{
		return -EINVAL;
	}

	buf = net_buf_alloc(&pbap_appl_pool, osaWaitNone_c);
	if (!buf)
	{
		return -EINVAL;
	}
	net_buf_reserve(buf, BT_PBAP_PCE_RSV_LEN_PULL_PHONEBOOK(g_PbapPce.pbap_pceHandle, BT_OBEX_REQ_UNSEG));

	BT_PBAP_ADD_PARAMS_MAX_LIST_COUNT(buf, 65535);

	g_PbapPce.num_srmp_wait = 0;
	g_PbapPce.lcl_srmp_wait = g_PbapPce.num_srmp_wait > 0 ? true : false;

	retval = bt_pbap_pce_pull_phonebook(g_PbapPce.pbap_pceHandle, buf, name, g_PbapPce.lcl_srmp_wait, BT_OBEX_REQ_UNSEG);
	PRINTF("PBAP_pull_complete\r\n");
	if (API_SUCCESS != retval)
	{
		net_buf_unref(buf);
		PRINTF("Send request failed\r\n");
        pbap_download_status_infotoApplication(PBAP_DOWNLOAD_FAILED);
	}
	return retval;
}

int app_set_phonebook_path(char *name)
{
	PRINTF("app_set_phonebook_path - %s\n", name);
	API_RESULT retval = 0;
	struct net_buf *buf;

	if (name == NULL)
	{
		return -EINVAL;
	}

	buf = net_buf_alloc(&pbap_appl_pool, osaWaitNone_c);
	if (!buf)
	{
		return -EINVAL;
	}
	net_buf_reserve(buf, BT_PBAP_PCE_RSV_LEN_SET_PATH(g_PbapPce.pbap_pceHandle));

	retval = bt_pbap_pce_set_phonebook_path(g_PbapPce.pbap_pceHandle, buf, name);
	if (API_SUCCESS != retval)
	{
		net_buf_unref(buf);
		PRINTF("Send request failed\r\n");
	}

	return retval;
}

int app_pull_vcard_listing(char *name)
{
	PRINTF("app_pull_vcard_listing - %s\n", name);
	API_RESULT retval = 0;
	struct net_buf *buf;

	buf = net_buf_alloc(&pbap_appl_pool, osaWaitNone_c);
	if (!buf)
	{
		return -EINVAL;
	}
	net_buf_reserve(buf, BT_PBAP_PCE_RSV_LEN_PULL_VCARD_LISTING(g_PbapPce.pbap_pceHandle, BT_OBEX_REQ_UNSEG));

	BT_PBAP_ADD_PARAMS_MAX_LIST_COUNT(buf, 65535);

	g_PbapPce.num_srmp_wait = 0;
	g_PbapPce.lcl_srmp_wait = g_PbapPce.num_srmp_wait > 0 ? true : false;

	retval = bt_pbap_pce_pull_vcard_listing(g_PbapPce.pbap_pceHandle, buf, name, g_PbapPce.lcl_srmp_wait, BT_OBEX_REQ_UNSEG);
	if (API_SUCCESS != retval)
	{
		net_buf_unref(buf);
		PRINTF("Send request failed\r\n");
	}

	return retval;
}

int app_pull_vcard_entry(char *name)
{
	PRINTF("app_pull_vcard_entry - %s\n", name);
	API_RESULT retval = 0;
	struct net_buf *buf;

	if (name == NULL)
	{
		return -EINVAL;
	}

	buf = net_buf_alloc(&pbap_appl_pool, osaWaitNone_c);
	if (!buf)
	{
		return -EINVAL;
	}
	net_buf_reserve(buf, BT_PBAP_PCE_RSV_LEN_PULL_VCARD_ENTRY(g_PbapPce.pbap_pceHandle, BT_OBEX_REQ_UNSEG));

	g_PbapPce.num_srmp_wait = 0;
	g_PbapPce.lcl_srmp_wait = g_PbapPce.num_srmp_wait > 0 ? true : false;

	retval = bt_pbap_pce_pull_vcard_entry(g_PbapPce.pbap_pceHandle, buf, name, g_PbapPce.lcl_srmp_wait, BT_OBEX_REQ_UNSEG);
	if (API_SUCCESS != retval)
	{
		net_buf_unref(buf);
		PRINTF("Send request failed\r\n");
	}

	return retval;
}

static uint8_t bt_pbap_pce_sdp_user(struct bt_conn *conn, struct bt_sdp_client_result *result)
{
	int res;
	uint32_t peer_feature = 0;
	uint16_t rfommchannel = 0;
	uint16_t l2cappsm     = 0;
	uint16_t pbap_version = 0;
	uint8_t supported_repo = 0;
	if ((result) && (result->resp_buf))
	{
		PRINTF("PBAP sdp success callback\r\n");

		#if defined(MAP_PROFILE_ENABLE) && (MAP_PROFILE_ENABLE == 1)
				g_appInstance.acl_conn = conn;
				sdp_discover_for_map_client(g_appInstance.acl_conn);
		#endif

		res = bt_sdp_get_profile_version(result->resp_buf, BT_SDP_PBAP_SVCLASS, &pbap_version);
		if (res < 0)
		{
			PRINTF("pbap version is not found\r\n");
		}
		else
		{
			PRINTF("pbap version is %x\r\n", pbap_version);
			g_PbapPce.pbap_version = pbap_version;
		}
		res = bt_sdp_get_supported_repositories(result->resp_buf, &supported_repo);
		if(res < 0)
		{
			PRINTF("pbap pse supported repositories is not found\r\n");
		}
		else
		{
			PRINTF("pbap pse supported repositories is %x\r\n", supported_repo);
			g_PbapPce.supported_repositories = supported_repo;
		}
		res = bt_sdp_get_pbap_map_ctn_features(result->resp_buf, &peer_feature);
		if (res < 0)
		{
			PRINTF("supported feature not found, use default feature_config : %x\r\n", BT_PBAP_SUPPORTED_FEATURES_V11);
			g_PbapPce.peer_feature = BT_PBAP_SUPPORTED_FEATURES_V11;
		}
		else
		{
			PRINTF("supported feature = %x\r\n", peer_feature);
			g_PbapPce.peer_feature = peer_feature;
		}
		g_PbapPce.loacal_feature = CONFIG_BT_PBAP_PCE_SUPPORTED_FEATURE;
		res = bt_sdp_get_goep_l2cap_psm(result->resp_buf, &l2cappsm);
		if (res >= 0)
		{
			PRINTF("l2cap_psm found. Connecting ...\n");
			res = bt_pbap_pce_psm_connect(conn, l2cappsm, NULL, peer_feature, &g_PbapPce.pbap_pceHandle);
			if (res < 0)
			{
				PRINTF("Send connect command failed\r\n");
			}
			g_PbapPce.goep_version = BT_GOEP_VERSION_2_0;
			return BT_SDP_DISCOVER_UUID_STOP;
		}

		res = bt_sdp_get_proto_param(result->resp_buf, BT_SDP_PROTO_RFCOMM, &rfommchannel);
		if (res < 0)
		{
			PRINTF("Fail to find rfcomm channel and l2cap_psm!\r\n");
		}
		else
		{
			PRINTF("rfcomm channel found. Connecting ...\n");
			res = bt_pbap_pce_scn_connect(conn, rfommchannel, NULL, peer_feature, &g_PbapPce.pbap_pceHandle);
			if (res < 0)
			{
				PRINTF("Send connect command failed\r\n");
			}
			g_PbapPce.goep_version = BT_GOEP_VERSION_1_1;
		}
	}

	return BT_SDP_DISCOVER_UUID_STOP;
}

static struct bt_sdp_discover_params discov_pbap_pce = {
    .uuid = BT_UUID_DECLARE_16(BT_SDP_PBAP_PSE_SVCLASS),
    .func = bt_pbap_pce_sdp_user,
    .pool = &pbap_sdp_client_pool,
};


void sdp_discover_for_pbap_client(struct bt_conn *conn_handle)
{
	int retval;
	int err;
	vTaskDelay(200);
    retval       = bt_sdp_discover(conn_handle, &discov_pbap_pce);
    if (retval)
    {
        PRINTF("DUT as PBAP client SDP discovery failed: result\r\n");
    }
    else
    {
        PRINTF("DUT as PBAP client SDP discovery started\r\n");
    }
}

void pbap_register_service()
{
	int err;

	pbap_pce_rec.handle = DB_RECORD_PBAP_PCE;
	bt_sdp_register_service(&pbap_pce_rec);

    err = bt_pbap_pce_register(&pce_cb);
    if (err < 0)
    {
        PRINTF("PBAP register failed\r\n");
        return;
    }
}

int pbap_pull_phonebook(int type)
{
	int result = 0;
	printf("PULL PHONEBOOK started for ");
	/*Need to check whether the PBAP connection is established successfully or not*/
	if(g_pbapConnected == 1)
	{
		g_infoType = type;
		switch (type)
		{
		case CALL_TYPE_FAVORITE:
			PRINTF("PB call list \n");
			g_contactCount = 0;
			return app_pull_phonebook(PBAP_PULL_PHONEBOOK_DEMO_NAME);
			break;
		case CALL_TYPE_MISSED:
			PRINTF("Missed call list \n");
			g_misscallCount = 0;
			return app_pull_phonebook(PBAP_PULL_VCARD_LISTING_MCH_DEMO_NAME);
			break;
		case CALL_TYPE_INCOMING:
			PRINTF("Incoming call list \n");
			g_incallCount = 0;
			return app_pull_phonebook(PBAP_PULL_VCARD_LISTING_ICH_DEMO_NAME);
			break;
		case CALL_TYPE_OUTGOING:
			PRINTF("Outgoing call list \n");
			g_outcallCount = 0;
			return app_pull_phonebook(PBAP_PULL_VCARD_LISTING_OCH_DEMO_NAME);
			break;
		default:
			PRINTF("Unknown call type \n");
			break;
		}
		pbap_download_status_infotoApplication(PBAP_DOWNLOAD_FAILED);
	}
	else
	{
		PRINTF("PBAP Connection is not established\r\n");
	}
	return true;
}


