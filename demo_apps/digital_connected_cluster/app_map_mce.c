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
#include <bluetooth/rfcomm.h>
#include <bluetooth/map_mce.h>
#include <bluetooth/sdp.h>
#include "BT_common.h"
#include "app_map_mce.h"
#include "app_connect.h"
#include "app_discover.h"

#define MAP_MCE_CLASS_OF_DEVICE (0x10010CU) /* Object Transfer, Computer, Laptop */

#define MAP_MCE_MAS_TX_NET_BUF_COUNT   (1U)
#define MAP_MCE_MAS_TX_NET_BUF_SIZE    (1024U + 2U) /* L2CAP I-frame Enhanced Control Field(2-byte) */

#define MAX_MESSAGE_SIZE 700

#define SDP_CLIENT_USER_BUF_LEN 512U
#define MAX_MESSAGES 3
#define MAX_MESSAGE_LENGTH 120
NET_BUF_POOL_FIXED_DEFINE(map_sdp_client_pool, CONFIG_BT_MAX_CONN, SDP_CLIENT_USER_BUF_LEN, CONFIG_NET_BUF_USER_DATA_SIZE, NULL);

static uint8_t g_isGetMessage = 0;
static uint8_t g_mapMessageIndex = 0;
uint32_t glb_message_list_cntr = 0,glb_manual_get_list =0;
char map_msg_example[MAX_MESSAGE_SIZE];
bool g_mapconnection = 0 ;

char g_mapMessages[MAX_MESSAGES][MAX_MESSAGE_LENGTH] = {
    "I'll call you back later",
    "Can�t pick up, riding. Will get back to you ASAP",
    "On the bike � can�t talk. Will ping you shortly"
};

static struct bt_sdp_attribute map_mce_attrs[] = {
    BT_SDP_NEW_SERVICE,
    /* ServiceClassIDList */
    BT_SDP_LIST(
        BT_SDP_ATTR_SVCLASS_ID_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 3), //35 03
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
            BT_SDP_ARRAY_16(BT_SDP_MAP_MCE_SVCLASS) //11 33
        },
        )
    ),
    BT_SDP_LIST(
        BT_SDP_ATTR_PROTO_DESC_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 17), //35, 11
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 3), // 35 , 3
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                BT_SDP_ARRAY_16(BT_SDP_PROTO_L2CAP) //01 00
            },
            )
        },
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 5),// 35 05
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                BT_SDP_ARRAY_16(BT_SDP_PROTO_RFCOMM), // 00 03
            },
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UINT8), //08
                BT_SDP_ARRAY_8(BT_RFCOMM_CHAN_MAP_MCE) //RFCOMM CHANNEL
            },
            )
        },
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 3),// 35 03
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                BT_SDP_ARRAY_16(BT_SDP_PROTP_OBEX) // 00 08
            },
            )
        },
        )
    ),
        /* BluetoothProfileDescriptorList */
    BT_SDP_LIST(
        BT_SDP_ATTR_PROFILE_DESC_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 8), //35 08
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6), //35 06
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                BT_SDP_ARRAY_16(BT_SDP_MAP_SVCLASS) //11 34
            },
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UINT16), //09
                BT_SDP_ARRAY_16(0x0104U) //01 04
            },
            )
        },
        )
    ),
    BT_SDP_SERVICE_NAME("MAP MNS-name"),
    /* GoepL2CapPsm */
    BT_SDP_ATTR_GOEP_L2CAP_PSM,
    {
        BT_SDP_TYPE_SIZE(BT_SDP_UINT16),
        BT_SDP_ARRAY_16(BT_BR_PSM_MAP_MCE)
    },
    /*  SupportedFeatures */
    BT_SDP_ATTR_MAP_SUPPORTED_FEATURES,
    {
        BT_SDP_TYPE_SIZE(BT_SDP_UINT32),
        BT_SDP_ARRAY_32(CONFIG_BT_MAP_MCE_MAS_SUPPORTED_FEATURES)
    },
};

void app_print_map_messages(void)
{
	PRINTF(	"1: %s\r\n2: %s\r\n3: %s\r\n",
			g_mapMessages[0],g_mapMessages[1],g_mapMessages[2]);
}

void app_map_push_msg_index(uint8_t index)
{
	g_mapMessageIndex = index;
}

void map_mce_mns_reg_connect(struct bt_conn *conn){

	PRINTF("MAP connect and register notification\r\n");
	g_appInstance.acl_conn = bt_conn_ref(conn);
	sdp_discover_for_map_client(g_appInstance.acl_conn);

}
static uint8_t map_sdp_discover_cb(struct bt_conn *conn, struct bt_sdp_client_result *result)
{
    int res;
    uint16_t scn;
    uint16_t psm = 0;
    uint32_t supported_features;
    uint16_t map_version;
    const char *service_name;
    uint8_t mas_instance_id;
    uint8_t supported_msg_type;

    if ((g_appInstance.acl_conn == conn) && (result) && (result->resp_buf))
    {
#ifdef APP_DEBUG_EN
        PRINTF("MAP sdp success callback\r\n");
#endif
        res = bt_sdp_get_proto_param(result->resp_buf, BT_SDP_PROTO_RFCOMM, &scn);
        if (res < 0)
        {
            PRINTF("REFCOMM channel number is not found\r\n");
            return BT_SDP_DISCOVER_UUID_CONTINUE;
        }
#ifdef APP_DEBUG_EN
        PRINTF("REFCOMM channel number %d\r\n", scn);
#endif
        res = bt_sdp_get_goep_l2cap_psm(result->resp_buf, &psm);
        if (res < 0)
        {
            g_appInstance.goep_version = BT_GOEP_VERSION_1_1;
#ifdef APP_DEBUG_EN
            PRINTF("L2CAP PSM is not found\n");
#endif
        }
        else
        {
            g_appInstance.goep_version = BT_GOEP_VERSION_2_0;
#ifdef APP_DEBUG_EN
            PRINTF("L2CAP PSM  0x%04X\n", psm);
#endif
        }
        res = bt_sdp_get_profile_version(result->resp_buf, BT_SDP_MAP_SVCLASS, &map_version);
        if (res < 0)
        {
            PRINTF("MAP version is not found with (err %d)\n", res);
        }
        else
        {
#ifdef APP_DEBUG_EN
            PRINTF("MAP version 0x%04X\n", map_version);
#endif
            g_appInstance.map_version = map_version;
        }
        res = bt_sdp_get_pbap_map_ctn_features(result->resp_buf, &supported_features);
        if (res < 0)
        {
            switch (g_appInstance.map_version)
            {
                case BT_MAP_VERSION_1_1:
                    g_appInstance.supported_features = BT_MAP_MSE_MAS_SUPPORTED_FEATURES_V11;
                    break;
                case BT_MAP_VERSION_1_2:
                    g_appInstance.supported_features = BT_MAP_MSE_MAS_SUPPORTED_FEATURES_V12;
                    break;
                case BT_MAP_VERSION_1_3:
                    g_appInstance.supported_features = BT_MAP_MSE_MAS_SUPPORTED_FEATURES_V13;
                    break;
                case BT_MAP_VERSION_1_4:
                    g_appInstance.supported_features = BT_MAP_MSE_MAS_SUPPORTED_FEATURES_V14;
                    break;
                default:
                    g_appInstance.supported_features = 0;
                    break;
            }
#ifdef APP_DEBUG_EN
            PRINTF("Supported features is not found Use the default supported features 0x%08X\r\n", g_appInstance.supported_features);
#endif
        }
        else
        {
#ifdef APP_DEBUG_EN
            PRINTF("MAP supported features 0x%08X\r\n", supported_features);
#endif
            g_appInstance.supported_features = supported_features;
        }
        res = bt_sdp_get_instance_id(result->resp_buf, &mas_instance_id);
        if (res < 0)
        {
            PRINTF("MAS instance ID is not found\r\n");
        }
        else
        {
#ifdef APP_DEBUG_EN
            PRINTF("MAS instance ID %d\r\n", mas_instance_id);
#endif
            g_appInstance.mas_instance_id = mas_instance_id;
        }
        res = bt_sdp_get_supported_msg_type(result->resp_buf, &supported_msg_type);
        if (res < 0)
        {
            PRINTF("Supported message type is not found with (err %d)\r\n", res);
        }
        else
        {
#ifdef APP_DEBUG_EN
            PRINTF("Supported message type 0x%02X\r\n", supported_msg_type);
#endif
        }
        res = bt_sdp_get_service_name(result->resp_buf, &service_name);
        if (res < 0)
        {
            PRINTF("Service name is not found with (err %d)\r\n", res);
        }
        else
        {
#ifdef APP_DEBUG_EN
            PRINTF("Service name %s\r\n", service_name);
#endif
        }
        if ((scn != 0U) || (psm != 0U))
        {
            PRINTF("Message Access Server found. Connecting ...\n");
            g_appInstance.psm = psm;
            g_appInstance.scn = scn;
            if (g_appInstance.goep_version >= BT_GOEP_VERSION_2_0)
            {
                res = bt_map_mce_psm_connect(g_appInstance.acl_conn, psm, supported_features, &g_appInstance.mce_mas);
            }
            else
            {
                res = bt_map_mce_scn_connect(g_appInstance.acl_conn, (uint8_t)scn, supported_features, &g_appInstance.mce_mas);
            }
            if (0 != res)
            {
                PRINTF("fail to connect MSE (err: %d)\r\n", res);
            }
        }
        return BT_SDP_DISCOVER_UUID_STOP;
    }
    else
    {
        PRINTF("sdp fail callback\r\n");
        return BT_SDP_DISCOVER_UUID_CONTINUE;
    }
}

static struct bt_sdp_discover_params discov_map_mse = {
    .uuid = BT_UUID_DECLARE_16(BT_SDP_MAP_MSE_SVCLASS),
    .func = map_sdp_discover_cb,
    .pool = &map_sdp_client_pool,
};

static struct bt_sdp_record map_mce_rec = BT_SDP_RECORD(map_mce_attrs);

/* The following message is encoded by G-7bit including twenty "Bluetooth MAP Test!".
The message can be used for testing with Android phone and boards with MSE function.
The message may not delivered successfully by MSE(e.g. Android phone) because the
phone number is 0000000000000. If wanting MSE to deliver this message successfully,
users need to modify the phone number(FN, N, TEL and 000000000000f0 in the message)
to the valid and modify the length(1080) accordingly.
For example, if users want to deliver the messsage to the phone number 123456,
FN, N and TEL shall be +123456, 000000000000f0 shall be 214365 and
the length(1080) shall be 1048(1080 - (14 - 6) * 4).
For example, If users want to deliver the messsage to the phone number 1234567,
FN, N and TEL shall be +1234567, 000000000000f0 shall be 214365f7 and
the length(1080) shall be 1056(1080 - (14 - 8) * 4). */
#define MAP_MCE_MSG_G_7BIT \
"BEGIN:BMSG\r\n\
VERSION:1.0\r\n\
STATUS:READ\r\n\
TYPE:SMS_GSM\r\n\
FOLDER:\r\n\
BEGIN:BENV\r\n\
BEGIN:VCARD\r\n\
VERSION:3.0\r\n\
FN:+919104539859\r\n\
N:+919104539859\r\n\
TEL:+919104539859\r\n\
END:VCARD\r\n\
BEGIN:BBODY\r\n\
ENCODING:G-7BIT\r\n\
LENGTH:1044\r\n\
BEGIN:MSG\r\n\
00410005910180f60000a0050003080401622e90905d2fd3df6f3a1ad40c4241d4f29c1e52c85c2021bb5ea6bfdf7434a8198482a8e5393da498b9404276bd4c7fbfe9685033080551cb737a4841738184ec7a99fe7ed3d1a066100aa296e7f490a2e60209d9f532fdfda6a341cd2014442dcfe92185cd0512b2eb65fafb4d47839a4128885a9ed3438a9b0b2464d7cbf4f79b8e063583\r\n\
END:MSG\r\n\
BEGIN:MSG\r\n\
00410005910180f60000a0050003080402a0206a794e0f29702e90905d2fd3df6f3a1ad40c4241d4f29c1e52e45c2021bb5ea6bfdf7434a8198482a8e5393da488c15c2021bb5ea6bfdf7434a8198482a8e5393da488c55c2021bb5ea6bfdf7434a8198482a8e5393da488c95c2021bb5ea6bfdf7434a8198482a8e5393da488cd5c2021bb5ea6bfdf7434a8198482a8e5393da488d15c\r\n\
END:MSG\r\n\
BEGIN:MSG\r\n\
00410005910180f60000a0050003080403404276bd4c7fbfe9685033080551cb737a4811abb9404276bd4c7fbfe9685033080551cb737a4811b3b9404276bd4c7fbfe9685033080551cb737a4811bbb9404276bd4c7fbfe9685033080551cb737a4811c3b9404276bd4c7fbfe9685033080551cb737a4811cbb9404276bd4c7fbfe9685033080551cb737a482183b9404276bd4c7fbfe9\r\n\
END:MSG\r\n\
BEGIN:MSG\r\n\
0041000d91191940358995f0000012050003080404d0a066100aa296e7f410\r\n\
END:MSG\r\n\
END:BBODY\r\n\
END:BENV\r\n\
END:BMSG"

void app_build_map_msg(const char *number) {
	snprintf(map_msg_example, MAX_MESSAGE_SIZE,
			"BEGIN:BMSG\r\n"
			"VERSION:1.0\r\n"
			"STATUS:UNREAD\r\n"
			"TYPE:SMS_GSM\r\n"
			"FOLDER:\r\n"
			"BEGIN:VCARD\r\n"
			"VERSION:2.1\r\n"
			"N;CHARSET=UTF-8:\r\n"
			"TEL;CHARSET=UTF-8:\r\n"
			"END:VCARD\r\n"
			"BEGIN:BENV\r\n"
			"BEGIN:VCARD\r\n"
			"VERSION:2.1\r\n"
			"FN;CHARSET=UTF-8:%s\r\n"
			"N;CHARSET=UTF-8:%s\r\n"
			"TEL:%s\r\n"
			"END:VCARD\r\n"
			"BEGIN:BBODY\r\n"
			"CHARSET:UTF-8\r\n"
			"LANGUAGE:UNKNOWN\r\n"
			"LENGTH:%d\r\n"
			"BEGIN:MSG\r\n"
			"%s\r\n"
			"END:MSG\r\n"
			"END:BBODY\r\n"
			"END:BENV\r\n"
			"END:BMSG", number, number, number, strlen(g_mapMessages[g_mapMessageIndex]), g_mapMessages[g_mapMessageIndex]);
	PRINTF("Mobile Number is: %s\nText message is %s and length is %d\r\n",number, g_mapMessages[g_mapMessageIndex],strlen(g_mapMessages[g_mapMessageIndex]));
}

struct map_hdr
{
    uint8_t *value;
    uint16_t length;
};

static void map_mce_mns_connected(struct bt_map_mce_mns *mce_mns);
static void map_mce_mns_disconnected(struct bt_map_mce_mns *mce_mns, uint8_t result);
static void app_send_event_cb(struct bt_map_mce_mns *mce_mns, struct net_buf *buf, enum bt_obex_req_flags flag);
static void map_mce_mas_connected(struct bt_map_mce_mas *mce_mas);
static void map_mce_mas_disconnected(struct bt_map_mce_mas *mce_mas, uint8_t result);
static void app_abort_cb(struct bt_map_mce_mas *mce_mas, uint8_t result);
static void app_get_folder_listing_cb(struct bt_map_mce_mas *mce_mas, uint8_t result, struct net_buf *buf);
static void app_set_folder_cb(struct bt_map_mce_mas *mce_mas, uint8_t result);
static void app_get_msg_listing_cb(struct bt_map_mce_mas *mce_mas, uint8_t result, struct net_buf *buf);
static void app_get_msg_cb(struct bt_map_mce_mas *mce_mas, uint8_t result, struct net_buf *buf);
static void app_set_msg_status_cb(struct bt_map_mce_mas *mce_mas, uint8_t result);
static void app_push_msg_cb(struct bt_map_mce_mas *mce_mas, uint8_t result, char *name);
static void app_set_ntf_reg_cb(struct bt_map_mce_mas *mce_mas, uint8_t result);
static void app_update_inbox_cb(struct bt_map_mce_mas *mce_mas, uint8_t result);
static void app_get_mas_inst_info_cb(struct bt_map_mce_mas *mce_mas, uint8_t result, struct net_buf *buf);
static void app_set_owner_status_cb(struct bt_map_mce_mas *mce_mas, uint8_t result);
static void app_get_owner_status_cb(struct bt_map_mce_mas *mce_mas, uint8_t result, struct net_buf *buf);
static void app_get_convo_listing_cb(struct bt_map_mce_mas *mce_mas, uint8_t result, struct net_buf *buf);
static void app_set_ntf_filter_cb(struct bt_map_mce_mas *mce_mas, uint8_t result);
static void app_state_machine(void);

struct app_map_mce_instance g_appInstance;
NET_BUF_POOL_FIXED_DEFINE(mce_mas_tx_pool, MAP_MCE_MAS_TX_NET_BUF_COUNT, BT_L2CAP_BUF_SIZE(MAP_MCE_MAS_TX_NET_BUF_SIZE), CONFIG_NET_BUF_USER_DATA_SIZE, NULL);

struct bt_map_mce_mas_cb map_mas_cb = {
    .connected = map_mce_mas_connected,
    .disconnected = map_mce_mas_disconnected,
    .abort = app_abort_cb,
    .get_folder_listing = app_get_folder_listing_cb,
    .set_folder = app_set_folder_cb,
    .get_msg_listing = app_get_msg_listing_cb,
    .get_msg = app_get_msg_cb,
    .set_msg_status = app_set_msg_status_cb,
    .push_msg = app_push_msg_cb,
    .set_ntf_reg = app_set_ntf_reg_cb,
    .update_inbox = app_update_inbox_cb,
    .get_mas_inst_info = app_get_mas_inst_info_cb,
    .set_owner_status = app_set_owner_status_cb,
    .get_owner_status = app_get_owner_status_cb,
    .get_convo_listing = app_get_convo_listing_cb,
    .set_ntf_filter = app_set_ntf_filter_cb,
};

struct bt_map_mce_mns_cb map_mns_cb = {
    .connected = map_mce_mns_connected,
    .disconnected = map_mce_mns_disconnected,
    .send_event = app_send_event_cb,
};

static uint8_t *app_strnstr(const uint8_t *haystack, size_t haystack_len, const char *needle)
{
    size_t index = 0;
    size_t needle_len = strlen(needle);

    if (haystack_len < needle_len)
    {
        return NULL;
    }

    do
    {
        if (memcmp(&haystack[index], needle, needle_len) == 0)
        {
            return (uint8_t *)&haystack[index + needle_len];
        }
        index++;
    } while(index < (haystack_len - needle_len));

    return NULL;
}

static void app_print_body(struct net_buf *buf)
{
    struct map_hdr body;
    if (bt_map_mce_get_body(buf, &body.value, &body.length) == 0)
    {
        PRINTF ("============== BODY ==============\r\n");
        PRINTF("%.*s\r\n", body.length, body.value);
        PRINTF ("============ END BODY ============\r\n");
    }
    else
    {
        PRINTF ("BODY not Found \r\n");
    }
}

static bool app_app_param_cb(struct bt_data *data, void *user_data)
{
    switch (data->type)
    {
        case BT_MAP_TAG_ID_NEW_MESSAGE:
            PRINTF ("New Message - %d\r\n",  data->data[0]);
            break;

        case BT_MAP_TAG_ID_MAS_INSTANCE_ID:
            PRINTF ("MAS Instance ID - %d\r\n", data->data[0]);
            break;

        case BT_MAP_TAG_ID_FOLDER_LISTING_SIZE:
            if (data->data_len < 2U)
            {
                return false;
            }
            PRINTF ("Folder Listing Size - %d\r\n", sys_get_be16(data->data));
            break;

        case BT_MAP_TAG_ID_LISTING_SIZE:
            if (data->data_len < 2U)
            {
                return false;
            }
            PRINTF ("Listing Size - %d\r\n", sys_get_be16(data->data));
            break;

        case BT_MAP_TAG_ID_FRACTION_DELIVER:
            PRINTF ("Fraction Deliver - %d\r\n", data->data[0]);
            break;

        case BT_MAP_TAG_ID_MSE_TIME:
            PRINTF ("MSE Time - %.*s\r\n", data->data_len, data->data);
            break;

        case BT_MAP_TAG_ID_DATABASE_IDENTIFIER:
            PRINTF ("Database Identifier - ");
            for (uint8_t index = 0; index < data->data_len; index++)
            {
                PRINTF("%02X", data->data[index]);
            }
            PRINTF("\r\n");
            break;

        case BT_MAP_TAG_ID_CONV_LIST_VER_CNTR:
            PRINTF("Conversation Listing Version Counter - %.*s\r\n", data->data_len, data->data);
            break;

        case BT_MAP_TAG_ID_PRESENCE_AVAILABILITY:
            PRINTF("Presence Availability - %d\r\n", data->data[0]);
            break;

        case BT_MAP_TAG_ID_PRESENCE_TEXT:
            PRINTF ("============== Presence Text ==============\r\n");
            PRINTF("%.*s\r\n", data->data_len, data->data);
            PRINTF ("============ END Presence Text ============\r\n");
            break;

        case BT_MAP_TAG_ID_LAST_ACTIVITY:
            PRINTF("Last Activity - %.*s\r\n", data->data_len, data->data);
            break;

        case BT_MAP_TAG_ID_CHAT_STATE:
            PRINTF("Chat State - %d\r\n", data->data[0]);
            break;

        case BT_MAP_TAG_ID_FOLDER_VER_CNTR:
            PRINTF ("Folder Version Counter - %.*s\r\n", data->data_len, data->data);
            break;

        case BT_MAP_TAG_ID_OWNER_UCI:
            PRINTF ("============== Owner UCI ==============\r\n");
            PRINTF("%.*s\r\n", data->data_len, data->data);
            PRINTF ("============ END Owner UCI ============\r\n");
            break;

        case BT_MAP_TAG_ID_MAP_SUPPORTED_FEATURES:
            if (data->data_len < 4U)
            {
                return false;
            }
            PRINTF ("Supported Features - %08X\r\n", sys_get_be32(data->data));
            break;

        default:
            break;
    }

    return true;
}

static void map_mce_mns_connected(struct bt_map_mce_mns *mce_mns)
{
    g_appInstance.mce_mns = mce_mns;
    PRINTF("MCE MNS connection\r\n");
    if (bt_map_mce_mns_get_max_pkt_len(mce_mns, &g_appInstance.mns_max_pkt_len) == 0)
    {
        PRINTF("MAX Packet Length - %d\r\n", g_appInstance.mns_max_pkt_len);
    }
    else
    {
        PRINTF("MAX Packet Length is invalid\r\n");
    }
    app_state_machine();
}

static void map_mce_mns_disconnected(struct bt_map_mce_mns *mce_mns, uint8_t result)
{
    g_appInstance.mce_mns = NULL;
    PRINTF("MCE MNS disconnection - 0x%02X\r\n", result);
	glb_manual_get_list--;
    app_state_machine();
}

static void map_mce_mas_connected(struct bt_map_mce_mas *mce_mas)
{
    g_appInstance.mce_mas = mce_mas;

    PRINTF("MAP client connected\r\n");
    if (bt_map_mce_get_max_pkt_len(mce_mas, &g_appInstance.max_pkt_len) == 0)
    {
        PRINTF("MAX Packet Length - %d\r\n", g_appInstance.max_pkt_len);
    }
    else
    {
        PRINTF("MAX Packet Length is invalid\r\n");
    }

    app_set_ntf_reg(1);
}

static void map_mce_mas_disconnected(struct bt_map_mce_mas *mce_mas, uint8_t result)
{
	if (g_mapconnection)
	{
		g_appInstance.mce_mas = NULL;
		PRINTF("MAP client disconnection - 0x%02X\r\n", result);
		g_mapconnection = 0;
	}
}

void app_mce_disconnect(void)
{
    PRINTF("MAP MCE MAS Disconnect\r\n");
    if (bt_map_mce_disconnect(g_appInstance.mce_mas) < 0)
    {
        PRINTF("Failed to disconnect\r\n");
    }
}

void app_mce_connect(void)
{
    int res;

    PRINTF("MAP MCE Connect\r\n");
    if (g_appInstance.goep_version >= BT_GOEP_VERSION_2_0)
    {
        res = bt_map_mce_psm_connect(g_appInstance.acl_conn, g_appInstance.psm, g_appInstance.supported_features, &g_appInstance.mce_mas);
    }
    else
    {
        res = bt_map_mce_scn_connect(g_appInstance.acl_conn, g_appInstance.scn, g_appInstance.supported_features, &g_appInstance.mce_mas);
    }

    if (res < 0)
    {
        PRINTF("fail to connect MSE (err: %d)\r\n", res);
    }
    g_mapconnection = 1;
}

void app_mce_abort(void)
{
    PRINTF("MAP Abort\r\n");
    int err = bt_map_mce_abort(g_appInstance.mce_mas);

    if (err == -EINPROGRESS)
    {
        PRINTF("Abort is pending\r\n");
    }
    else if (err == 0)
    {
        /* nothing to do */
    }
    else
    {
        PRINTF("Failed to abort\r\n");
    }
}

static void app_abort_cb(struct bt_map_mce_mas *mce_mas, uint8_t result)
{
    PRINTF("MCE Abort - 0x%02X\r\n", result);
}

void app_mce_mns_disconnect(void)
{
    PRINTF("MAP MCE MNS Disconnect\r\n");
    if (bt_map_mce_mns_disconnect(g_appInstance.mce_mns) < 0)
    {
        PRINTF("Failed to close MNS transport\r\n");
    }
}

static void app_send_event_cb(struct bt_map_mce_mns *mce_mns, struct net_buf *buf, enum bt_obex_req_flags flag)
{
    uint8_t result;
    PRINTF ("MAP Recv Send Event\r\n");
    bt_map_mce_app_param_parse(buf, app_app_param_cb, NULL);
    app_print_body(buf);
    net_buf_unref(buf);

    if (flag & BT_OBEX_REQ_END)
    {
        result = BT_MAP_RSP_SUCCESS;
    }
    else
    {
        result = BT_MAP_RSP_CONTINUE;
    }
    if (bt_map_mce_send_event_response(mce_mns, result, false) < 0)
    {
        PRINTF ("Failed to send event response\r\n");
    }
}

void app_get_folder_listing(uint8_t num_srmp_wait)
{
    struct net_buf *buf;

    PRINTF("MAP Get Folder Listing\r\n");
    buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
    net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_GET_FOLDER_LISTING(g_appInstance.mce_mas, BT_OBEX_REQ_UNSEG));
    g_appInstance.num_srmp_wait = num_srmp_wait;
    if (bt_map_mce_get_folder_listing(g_appInstance.mce_mas, buf, (g_appInstance.num_srmp_wait ? true : false), BT_OBEX_REQ_UNSEG) != 0)
    {
        net_buf_unref(buf);
        PRINTF ("Failed to get folder listing\r\n");
    }
}

static void app_get_folder_listing_cb(struct bt_map_mce_mas *mce_mas, uint8_t result, struct net_buf *buf)
{
    PRINTF ("MAP Get Folder Listing CNF - 0x%02X\r\n", result);
    bt_map_mce_app_param_parse(buf, app_app_param_cb, NULL);
    //app_print_body(buf);
    net_buf_unref(buf);

    if (result == BT_MAP_RSP_CONTINUE)
    {
        g_appInstance.num_srmp_wait = g_appInstance.num_srmp_wait ? g_appInstance.num_srmp_wait - 1 : 0;
        buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
        net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_GET_FOLDER_LISTING(g_appInstance.mce_mas, BT_OBEX_REQ_END));
        if (bt_map_mce_get_folder_listing(mce_mas, buf, (g_appInstance.num_srmp_wait ? true : false), BT_OBEX_REQ_END) != 0)
        {
            net_buf_unref(buf);
            PRINTF ("Failed to get folder listing\r\n");
        }
    }
    else
    {
    	g_isGetMessage = 1;
        app_state_machine();
    }
}

void app_set_folder(char *name)
{
    PRINTF("MAP Set Folder\r\n");
    if (name != NULL)
    {
        PRINTF("Name - %s\r\n", name);
    }
    if (bt_map_mce_set_folder(g_appInstance.mce_mas, name) != 0)
    {
        PRINTF ("Failed to set folder\r\n");
    }
}

static void app_set_folder_cb(struct bt_map_mce_mas *mce_mas, uint8_t result)
{
    PRINTF ("MAP Set Folder CNF - 0x%02X\r\n", result);
    if (result == BT_MAP_RSP_SUCCESS)
    {
        app_state_machine();
    }
}

void app_get_msg_listing(char *name, uint16_t max_list_cnt, uint8_t wait)
{
    struct net_buf *buf;

    PRINTF("MAP Get MSG Listing\r\n");
    PRINTF("MAX List Count - %d\r\n", max_list_cnt);
    PRINTF("SRMP Wait Count - %d\r\n", wait);
    buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
    net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_GET_MSG_LISTING(g_appInstance.mce_mas, name, BT_OBEX_REQ_UNSEG));
    BT_MAP_ADD_MAX_LIST_COUNT(buf, max_list_cnt);
    g_appInstance.num_srmp_wait = wait;
    if (bt_map_mce_get_msg_listing(g_appInstance.mce_mas, buf, name, (g_appInstance.num_srmp_wait ? true : false), BT_OBEX_REQ_UNSEG) != 0)
    {
        net_buf_unref(buf);
        PRINTF ("Failed to get msg listing\r\n");
    }
}

static void app_get_msg_listing_cb(struct bt_map_mce_mas *mce_mas, uint8_t result, struct net_buf *buf)
{
    struct map_hdr body;
    uint8_t *msg_handle;
    uint8_t index;

    PRINTF ("MAP Get MSG Listing CNF - 0x%02X\r\n", result);
    bt_map_mce_app_param_parse(buf, app_app_param_cb, NULL);
    if (bt_map_mce_get_body(buf, &body.value, &body.length) == 0)
    {
        //PRINTF ("============== BODY ==============\r\n");
        //PRINTF("%.*s\r\n", body.length, body.value);
        //PRINTF ("============ END BODY ============\r\n");
        if (g_appInstance.msg_handle[0] == '\0')
        {
            msg_handle = app_strnstr(body.value, body.length, "msg handle = \"");
            if (msg_handle == NULL)
            {
                msg_handle = app_strnstr(body.value, body.length, "msg handle=\"");
            }
            if (msg_handle != NULL)
            {
                for (index = 0; index < BT_MAP_MSG_HANDLE_SIZE / 2U - 1U; index++)
                {
                    if (msg_handle[index] == '"')
                    {
                        break;
                    }
                    g_appInstance.msg_handle[index] = (char)msg_handle[index];
                }
                g_appInstance.msg_handle[index] = '\0';
            }
        }
    }
    else
    {
        PRINTF ("BODY not Found \r\n");
    }
    net_buf_unref(buf);

    if (result == BT_MAP_RSP_CONTINUE)
    {
        g_appInstance.num_srmp_wait = g_appInstance.num_srmp_wait ? g_appInstance.num_srmp_wait - 1 : 0;
        buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
        net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_GET_MSG_LISTING(g_appInstance.mce_mas, NULL, BT_OBEX_REQ_END));
        if (bt_map_mce_get_msg_listing(mce_mas, buf, NULL, (g_appInstance.num_srmp_wait ? true : false), BT_OBEX_REQ_END) != 0)
        {
            net_buf_unref(buf);
            PRINTF ("Failed to get msg listing\r\n");
        }
    }
    else
    {
        app_state_machine();
    }
}

void app_get_msg(char *name, bool attachment, bool charset, uint8_t wait)
{
    struct net_buf *buf;

    PRINTF("MAP Get MSG\r\n");
    PRINTF("Name - %s\r\n", name);
    PRINTF("Attachment - %d\r\n", attachment);
    PRINTF("Charset - %d\r\n", charset);
    PRINTF("SRMP Wait Count - %d\r\n", wait);
    buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
    net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_GET_MSG(g_appInstance.mce_mas, BT_OBEX_REQ_UNSEG));
    BT_MAP_ADD_ATTACHMENT(buf, (uint8_t)attachment);
    BT_MAP_ADD_CHARSET(buf, (uint8_t)charset);
    g_appInstance.num_srmp_wait = wait;
    if (bt_map_mce_get_msg(g_appInstance.mce_mas, buf, name, (g_appInstance.num_srmp_wait ? true : false), BT_OBEX_REQ_UNSEG) != 0)
    {
        net_buf_unref(buf);
        PRINTF ("Failed to get msg\r\n");
    }
}

static void app_get_msg_cb(struct bt_map_mce_mas *mce_mas, uint8_t result, struct net_buf *buf)
{
    PRINTF ("MAP Get MSG CNF - 0x%02X\r\n", result);
    bt_map_mce_app_param_parse(buf, app_app_param_cb, NULL);
    app_print_body(buf);
    net_buf_unref(buf);

    if (result == BT_MAP_RSP_CONTINUE)
    {
        g_appInstance.num_srmp_wait = g_appInstance.num_srmp_wait ? g_appInstance.num_srmp_wait - 1 : 0;
        buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
        net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_GET_MSG(g_appInstance.mce_mas, BT_OBEX_REQ_END));
        if (bt_map_mce_get_msg(mce_mas, buf, NULL, (g_appInstance.num_srmp_wait ? true : false), BT_OBEX_REQ_END) != 0)
        {
            net_buf_unref(buf);
            PRINTF ("Failed to get msg\r\n");
        }
    }
    else
    {
        app_state_machine();
    }
}

void app_set_msg_status(char *name, uint8_t status_ind, uint8_t status_val)
{
    struct net_buf *buf;

    PRINTF("MAP Set MSG Status\r\n");
    PRINTF("Name - %s\r\n", name);
    PRINTF("Status Indicator - %d\r\n", status_ind);
    PRINTF("Status Value - %d\r\n", status_val);
    buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
    net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_SET_MSG_STATUS(g_appInstance.mce_mas, BT_OBEX_REQ_UNSEG));
    BT_MAP_ADD_STATUS_INDICATOR(buf, status_ind);
    BT_MAP_ADD_STATUS_VALUE(buf, status_val);

    if (bt_map_mce_set_msg_status(g_appInstance.mce_mas, buf, name, BT_OBEX_REQ_UNSEG) != 0)
    {
        net_buf_unref(buf);
        PRINTF ("Failed to set msg status\r\n");
    }
}

static void app_set_msg_status_cb(struct bt_map_mce_mas *mce_mas, uint8_t result)
{
    PRINTF ("MAP Set MSG Status CNF - 0x%02X\r\n", result);
    app_state_machine();
    /* If the result is BT_MAP_RSP_CONTINUE and the message is not sent completely,
        call bt_map_mce_set_msg_status to continue to send */
}

void app_push_msg(char *name, bool charset)
{
    struct net_buf *buf;
    enum bt_obex_req_flags flags = BT_OBEX_REQ_UNSEG;
    uint16_t max_body_len;
    uint16_t actual;

    PRINTF("MAP Push MSG\r\n");
    if (name != NULL)
    {
        PRINTF("Name - %s\r\n", name);
    }
    PRINTF("Charset - %d\r\n", charset);
    buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
    net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_PUSH_MSG(g_appInstance.mce_mas, name, flags));
    BT_MAP_ADD_CHARSET(buf, (uint8_t)charset);
    max_body_len = g_appInstance.max_pkt_len;
    max_body_len -= BT_MAP_MCE_RSV_LEN_PUSH_MSG(g_appInstance.mce_mas, name, flags) - BT_L2CAP_BUF_SIZE(2U);
    max_body_len -= buf->len; /* application parameters */
    max_body_len -= sizeof(struct bt_obex_hdr_bytes);

    actual = strlen(map_msg_example);
    if (actual > max_body_len)
    {
        actual = max_body_len;
        flags = BT_OBEX_REQ_START;
    }
    PRINTF("MAP Data:\n%s\n",map_msg_example);

    if (flags == BT_OBEX_REQ_START)
    {
        BT_MAP_ADD_BODY(buf, (uint8_t *)map_msg_example, actual);
    }
    else
    {
        BT_MAP_ADD_END_OF_BODY(buf, (uint8_t *)map_msg_example, actual);
    }
    if (bt_map_mce_push_msg(g_appInstance.mce_mas, buf, name, flags) != 0)
    {
        net_buf_unref(buf);
        PRINTF ("Failed to push msg\r\n");
    }
    else
    {
        g_appInstance.tx_cnt = actual;
    }
}

static void app_push_msg_cb(struct bt_map_mce_mas *mce_mas, uint8_t result, char *name)
{
    struct net_buf *buf;
    uint16_t actual;
    enum bt_obex_req_flags flags = BT_OBEX_REQ_END;
    uint16_t max_body_len;

    PRINTF ("MAP Push MSG CNF - 0x%02X\r\n", result);
    if (name != NULL)
    {
        PRINTF ("Name - %.*s\r\n", BT_MAP_MSG_HANDLE_SIZE / 2U, name);
    }

    if (result == BT_MAP_RSP_CONTINUE)
    {
        if (g_appInstance.tx_cnt < strlen(map_msg_example))
        {
            buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
            net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_PUSH_MSG(g_appInstance.mce_mas, NULL, flags));
            max_body_len = g_appInstance.max_pkt_len;
            max_body_len -= BT_MAP_MCE_RSV_LEN_PUSH_MSG(g_appInstance.mce_mas, NULL, flags) - BT_L2CAP_BUF_SIZE(2U);
            max_body_len -= sizeof(struct bt_obex_hdr_bytes);
            actual = strlen(map_msg_example) - g_appInstance.tx_cnt;
            if (actual > max_body_len)
            {
                actual = max_body_len;
                flags = BT_OBEX_REQ_CONTINUE;
            }
            if (flags == BT_OBEX_REQ_CONTINUE)
            {
                BT_MAP_ADD_BODY(buf, (uint8_t *)(map_msg_example + g_appInstance.tx_cnt), actual);
            }
            else
            {
                BT_MAP_ADD_END_OF_BODY(buf, (uint8_t *)(map_msg_example + g_appInstance.tx_cnt), actual);
            }
            if (bt_map_mce_push_msg(mce_mas, buf, NULL, flags) != 0)
            {
                net_buf_unref(buf);
                PRINTF ("Failed to push msg\r\n");
            }
            else
            {
                g_appInstance.tx_cnt += actual;
            }
        }
    }
    else
    {
        app_state_machine();
    }
}

void app_set_ntf_reg(bool ntf_status)
{
    struct net_buf *buf;

    PRINTF("MAP Set Notification Registration\r\n");
    PRINTF("Notification Status - %d\r\n", ntf_status);
    buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
    net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_SET_NTF_REG(g_appInstance.mce_mas));
    BT_MAP_ADD_NOTIFICATION_STATUS(buf, ntf_status ? 1 : 0);

    if (bt_map_mce_set_ntf_reg(g_appInstance.mce_mas, buf) != 0)
    {
        net_buf_unref(buf);
        PRINTF ("Failed to set notification registration\r\n");
    }
}

static void app_set_ntf_reg_cb(struct bt_map_mce_mas *mce_mas, uint8_t result)
{
    PRINTF ("MAP Set Notification Registration CNF - 0x%02X\r\n", result);
}

void app_update_inbox(void)
{
    PRINTF("MAP Update Inbox\r\n");

    if (bt_map_mce_update_inbox(g_appInstance.mce_mas) != 0)
    {
        PRINTF ("Failed to udpate inbox\r\n");
    }
}

static void app_update_inbox_cb(struct bt_map_mce_mas *mce_mas, uint8_t result)
{
    PRINTF ("MAP Update Inbox CNF - 0x%02X\r\n", result);
    app_state_machine();
}

void app_get_mas_inst_info(uint8_t mas_inst_id, uint8_t wait)
{
    struct net_buf *buf;

    PRINTF("MAP Get MAS Instance Info\r\n");
    PRINTF("MAS Instance ID - %d\r\n", mas_inst_id);
    PRINTF("SRMP Wait Count - %d\r\n", wait);
    buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
    net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_GET_MAS_INST_INFO(g_appInstance.mce_mas, BT_OBEX_REQ_UNSEG));
    BT_MAP_ADD_MAS_INSTANCE_ID(buf, mas_inst_id);
    g_appInstance.num_srmp_wait = wait;
    if (bt_map_mce_get_mas_inst_info(g_appInstance.mce_mas, buf, (g_appInstance.num_srmp_wait ? true : false), BT_OBEX_REQ_UNSEG) != 0)
    {
        net_buf_unref(buf);
        PRINTF ("Failed to get MAS instance infomation\r\n");
    }
}

static void app_get_mas_inst_info_cb(struct bt_map_mce_mas *mce_mas, uint8_t result, struct net_buf *buf)
{
    PRINTF ("MAP Get MAS Instance Info CNF - 0x%02X\r\n", result);
    bt_map_mce_app_param_parse(buf, app_app_param_cb, NULL);
    app_print_body(buf);
    net_buf_unref(buf);

    if (result == BT_MAP_RSP_CONTINUE)
    {
        g_appInstance.num_srmp_wait = g_appInstance.num_srmp_wait ? g_appInstance.num_srmp_wait - 1 : 0;
        buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
        net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_GET_MAS_INST_INFO(g_appInstance.mce_mas, BT_OBEX_REQ_END));
        if (bt_map_mce_get_mas_inst_info(mce_mas, buf, (g_appInstance.num_srmp_wait ? true : false), BT_OBEX_REQ_END) != 0)
        {
            net_buf_unref(buf);
            PRINTF ("Failed to get MAS instance infomation\r\n");
        }
    }
    else
    {
        app_state_machine();
    }
}

void app_set_owner_status(uint8_t chat_state)
{
    struct net_buf *buf;

    PRINTF("MAP Set Owner Status\r\n");
    PRINTF("Chat State - %d\r\n", chat_state);
    buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
    net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_SET_OWNER_STATUS(g_appInstance.mce_mas, BT_OBEX_REQ_UNSEG));
    BT_MAP_ADD_CHAT_STATE(buf, chat_state);
    if (bt_map_mce_set_owner_status(g_appInstance.mce_mas, buf, BT_OBEX_REQ_UNSEG) != 0)
    {
        net_buf_unref(buf);
        PRINTF ("Failed to set owner status\r\n");
    }
}

static void app_set_owner_status_cb(struct bt_map_mce_mas *mce_mas, uint8_t result)
{
    PRINTF ("MAP Set Owner Status CNF - 0x%02X\r\n", result);
    app_state_machine();
}

void app_get_owner_status(uint8_t wait)
{
    struct net_buf *buf;

    PRINTF("MAP Get Owner Status\r\n");
    PRINTF("SRMP Wait Count - %d\r\n", wait);
    buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
    net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_GET_OWNER_STATUS(g_appInstance.mce_mas, BT_OBEX_REQ_UNSEG));
    g_appInstance.num_srmp_wait = wait;
    if (bt_map_mce_get_owner_status(g_appInstance.mce_mas, buf, (g_appInstance.num_srmp_wait ? true : false), BT_OBEX_REQ_UNSEG) != 0)
    {
        net_buf_unref(buf);
        PRINTF ("Failed to get owner status\r\n");
    }
}

static void app_get_owner_status_cb(struct bt_map_mce_mas *mce_mas, uint8_t result, struct net_buf *buf)
{
    PRINTF ("MAP Get Owner Status CNF - 0x%02X\r\n", result);
    bt_map_mce_app_param_parse(buf, app_app_param_cb, NULL);
    net_buf_unref(buf);

    if (result == BT_MAP_RSP_CONTINUE)
    {
        g_appInstance.num_srmp_wait = g_appInstance.num_srmp_wait ? g_appInstance.num_srmp_wait - 1 : 0;
        buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
        net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_GET_OWNER_STATUS(g_appInstance.mce_mas, BT_OBEX_REQ_END));
        if (bt_map_mce_get_owner_status(mce_mas, buf, (g_appInstance.num_srmp_wait ? true : false), BT_OBEX_REQ_END) != 0)
        {
            net_buf_unref(buf);
            PRINTF ("Failed to get owner status\r\n");
        }
    }
    else
    {
        app_state_machine();
    }
}

void app_get_convo_listing(uint16_t max_list_cnt, uint8_t wait)
{
    struct net_buf *buf;

    PRINTF("MAP Get Conversation Listing\r\n");
    PRINTF("MAX List Count - %d\r\n", max_list_cnt);
    PRINTF("SRMP Wait Count - %d\r\n", wait);
    buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
    net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_GET_CONVO_LISTING(g_appInstance.mce_mas, BT_OBEX_REQ_UNSEG));
    BT_MAP_ADD_MAX_LIST_COUNT(buf, max_list_cnt);
    g_appInstance.num_srmp_wait = wait;
    if (bt_map_mce_get_convo_listing(g_appInstance.mce_mas, buf, (g_appInstance.num_srmp_wait ? true : false), BT_OBEX_REQ_UNSEG) != 0)
    {
        net_buf_unref(buf);
        PRINTF ("Failed to get conversation listing\r\n");
    }
}

static void app_get_convo_listing_cb(struct bt_map_mce_mas *mce_mas, uint8_t result, struct net_buf *buf)
{
    PRINTF ("MAP Get Conversation Listing CNF - 0x%02X\r\n", result);
    bt_map_mce_app_param_parse(buf, app_app_param_cb, NULL);
    app_print_body(buf);
    net_buf_unref(buf);

    if (result == BT_MAP_RSP_CONTINUE)
    {
        g_appInstance.num_srmp_wait = g_appInstance.num_srmp_wait ? g_appInstance.num_srmp_wait - 1 : 0;
        buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
        net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_GET_CONVO_LISTING(g_appInstance.mce_mas, BT_OBEX_REQ_END));
        if (bt_map_mce_get_convo_listing(mce_mas, buf, (g_appInstance.num_srmp_wait ? true : false), BT_OBEX_REQ_END) != 0)
        {
            net_buf_unref(buf);
            PRINTF ("Failed to get conversation listing\r\n");
        }
    }
    else
    {
        app_state_machine();
    }
}

void app_set_ntf_filter(uint32_t ntf_filter_mask)
{
    struct net_buf *buf;

    PRINTF("MAP Set Notification Filter\r\n");
    PRINTF("Notification Filter Mask - %d\r\n", ntf_filter_mask);
    buf = net_buf_alloc(&mce_mas_tx_pool, osaWaitForever_c);
    net_buf_reserve(buf, BT_MAP_MCE_RSV_LEN_SET_NTF_FILTER(g_appInstance.mce_mas));
    BT_MAP_ADD_NOTIFICATION_FILTER_MASK(buf, ntf_filter_mask);

    if (bt_map_mce_set_ntf_filter(g_appInstance.mce_mas, buf) != 0)
    {
        net_buf_unref(buf);
        PRINTF ("Failed to set notification filter\r\n");
    }
}

static void app_set_ntf_filter_cb(struct bt_map_mce_mas *mce_mas, uint8_t result)
{
    PRINTF ("MAP Set NTF Filter CNF - 0x%02X\r\n", result);
    app_state_machine();
}

static void app_check_supported_features(void)
{
    uint8_t *state = &g_appInstance.state;

    if ((*state == WAIT_GET_MSG_LISTING) || (*state == WAIT_SET_MSG_STATUS))
    {
        if (g_appInstance.supported_features & BT_MAP_CONVO_LISTING)
        {
            *state = GET_CONVO_LISTING;
        }
        else
        {
            PRINTF("[%d] ~ [%d] Skip, BT_MAP_CONVO_LISTING is not supported\r\n", GET_CONVO_LISTING, WAIT_GET_CONVO_LISTING);
        }
    }

    if ((*state == WAIT_GET_MSG_LISTING) || (*state == WAIT_SET_MSG_STATUS) || (*state == WAIT_GET_CONVO_LISTING))
    {
        if (g_appInstance.supported_features & BT_MAP_INST_INFO_FEATURE)
        {
            *state = GET_MAS_INST_INFO;
            return;
        }
        else
        {
            PRINTF("[%d] ~ [%d] Skip, BT_MAP_INST_INFO_FEATURE is not supported\r\n", GET_MAS_INST_INFO, WAIT_GET_MAS_INST_INFO);
        }
    }

    if ((*state == WAIT_GET_MSG_LISTING) || (*state == WAIT_SET_MSG_STATUS) || (*state == WAIT_GET_CONVO_LISTING) ||
        (*state == WAIT_GET_MAS_INST_INFO))
    {
        if (g_appInstance.supported_features & BT_MAP_NTF_FILTERING)
        {
            *state = SET_NTF_FILTER;
            return;
        }
        else
        {
            PRINTF("[%d] ~ [%d] Skip, BT_MAP_NTF_FILTERING is not supported\r\n", SET_NTF_FILTER, WAIT_SET_NTF_FILTER);
        }
    }

    if ((*state == WAIT_GET_MSG_LISTING) || (*state == WAIT_SET_MSG_STATUS) || (*state == WAIT_GET_CONVO_LISTING) ||
        (*state == WAIT_GET_MAS_INST_INFO) || (*state == WAIT_SET_NTF_FILTER))
    {
        if (g_appInstance.supported_features & BT_MAP_NTF_REG_FEATURE)
        {
            *state = SET_NTF_REG_ON;
            return;
        }
        else
        {
            PRINTF("[%d] ~ [%d] Skip, BT_MAP_NTF_REG_FEATURE is not supported\r\n", SET_NTF_REG_ON, WAIT_SET_NTF_REG_OFF);
        }
    }

    if ((*state == WAIT_GET_MSG_LISTING) || (*state == WAIT_SET_MSG_STATUS) || (*state == WAIT_GET_CONVO_LISTING) ||
        (*state == WAIT_GET_MAS_INST_INFO) || (*state == WAIT_SET_NTF_FILTER) || (*state == WAIT_SET_NTF_REG_OFF))
    {
        if ((g_appInstance.supported_features & BT_MAP_OWNER_STATUS) && (g_appInstance.supported_features & BT_MAP_UPLOADING_FEATURE))
        {
            *state = GET_OWNER_STATUS;
            return;
        }
        else
        {
            PRINTF("[%d] ~ [%d] Skip, BT_MAP_OWNER_STATUS is not supported\r\n", GET_OWNER_STATUS, WAIT_SET_OWNER_STATUS);
        }
        if (g_appInstance.supported_features & BT_MAP_UPLOADING_FEATURE)
        {
            *state = SET_FOLDER_PARENT;
            return;
        }
        else
        {
            PRINTF("[%d] ~ [%d] Skip, BT_MAP_UPLOADING_FEATURE is not supported\r\n", SET_FOLDER_PARENT, WAIT_PUSH_MSG);
        }
        *state = MCE_MAS_DISCONNECT;
    }
}

void app_map_push_message()
{
	if(g_isGetMessage)
	{
		g_appInstance.state = SET_FOLDER_PARENT;
		app_state_machine();

	}
	else
	{
		PRINTF("The get_message command must be executed prior to send_message command.\r\n");
	}
}


static void app_state_machine(void)
{
    uint8_t *state = &g_appInstance.state;

    switch (*state)
    {
        /* Browsing Feature */
        case WAIT_GET_FOLDER_LISTING_ROOT:
            PRINTF("[%d]: GET_FOLDER_LISTING_ROOT Complete\r\n", *state);
            *state = SET_FOLDER_TELECOM;
            break;
        case WAIT_SET_FOLDER_TELECOM:
            PRINTF("[%d]: SET_FOLDER_TELECOM Complete\r\n", *state);
            *state = SET_FOLDER_MSG;
            break;
        case WAIT_SET_FOLDER_MSG:
            PRINTF("[%d]: SET_FOLDER_MSG Complete\r\n", *state);
            *state = SET_FOLDER_INBOX;
            break;
        case WAIT_SET_FOLDER_INBOX:
            PRINTF("[%d]: SET_FOLDER_INBOX Complete\r\n", *state);
            *state = UPDATE_INBOX;
            break;
        case WAIT_UPDATE_INBOX:
            PRINTF("[%d]: UPDATE_INBOX Complete\r\n", *state);
            *state = GET_MSG_LISTING;
            break;
        case WAIT_GET_MSG_LISTING:
            PRINTF("[%d]: GET_MSG_LISTING Complete\r\n", *state);
            if(glb_manual_get_list >= 1 ) {
                 vTaskDelay(100);
            	 *state = MAP_NOT_STARTED;
            	 PRINTF("[%d]: TP:GET_MSG_LISTING Complete\r\n", *state);
            }
            else {
            if (g_appInstance.msg_handle[0] != '\0')
            {
                *state = GET_MSG;
            }
            else
            {
                PRINTF("MSG handle not found in GET_MSG_LISTING\r\n");
                PRINTF("[%d] ~ [%d] Skip\r\n", GET_MSG, WAIT_SET_MSG_STATUS);
                app_check_supported_features();
            	}
            }
            break;
        case WAIT_GET_MSG:
            PRINTF("[%d]: GET_MSG Complete\r\n", *state);
            *state = SET_MSG_STATUS;
            break;
        case WAIT_SET_MSG_STATUS:
            PRINTF("[%d]: SET_MSG_STATUS Complete\r\n", *state);
            app_check_supported_features();
            break;
        case WAIT_GET_CONVO_LISTING:
            PRINTF("[%d]: GET_CONVO_LISTING Complete\r\n", *state);
            app_check_supported_features();
            break;

        /* Instance Information Feature */
        case WAIT_GET_MAS_INST_INFO:
            PRINTF("[%d]: GET_MAS_INST_INFO Complete\r\n", *state);
            app_check_supported_features();
            break;

        /* Notification Feature */
        case WAIT_SET_NTF_FILTER:
            PRINTF("[%d]: SET_NTF_FILTER Complete\r\n", *state);
            app_check_supported_features();
            break;

        /* Notification Registration Feature */
        case WAIT_SET_NTF_REG_ON:
            PRINTF("[%d]: SET_NTF_REG_ON Complete\r\n", *state);
            *state = SET_NTF_REG_OFF;
            break;
        case WAIT_SET_NTF_REG_OFF:
            PRINTF("[%d]: SET_NTF_REG_OFF Complete\r\n", *state);
            app_check_supported_features();
            break;

        /* Uploading Feature */
        case WAIT_GET_OWNER_STATUS:
            PRINTF("[%d]: GET_OWNER_STATUS Complete\r\n", *state);
            *state = SET_OWNER_STATUS;
            break;
        case WAIT_SET_OWNER_STATUS:
            PRINTF("[%d]: SET_OWNER_STATUS Complete\r\n", *state);
            *state = SET_FOLDER_PARENT;
            break;
        case WAIT_SET_FOLDER_PARENT:
            PRINTF("[%d]: SET_FOLDER_PARENT Complete\r\n", *state);
            *state = SET_FOLDER_OUTBOX;
            break;
        case WAIT_SET_FOLDER_OUTBOX:
            PRINTF("[%d]: SET_FOLDER_OUTBOX Complete\r\n", *state);
            *state = PUSH_MSG;
            break;
        case WAIT_PUSH_MSG:
            PRINTF("[%d]: PUSH_MSG Complete\r\n", *state);
            //*state = MCE_MAS_DISCONNECT;
            break;
        case WAIT_MCE_MAS_DISCONNECT:
            PRINTF("[%d]: MCE_MAS_DISCONNECT Complete\r\n", *state);
            *state = MAP_NOT_STARTED;
        default:
            break;
    }

    switch (*state)
    {
        /* Browsing Feature */
        case GET_FOLDER_LISTING_ROOT:
            PRINTF("[%d]: GET_FOLDER_LISTING_ROOT\r\n", *state);
            app_get_folder_listing(0);
            *state = WAIT_GET_FOLDER_LISTING_ROOT;
            break;
        case SET_FOLDER_TELECOM:
            PRINTF("[%d]: SET_FOLDER_TELECOM\r\n", *state);
            app_set_folder("telecom");
            *state = WAIT_SET_FOLDER_TELECOM;
            break;
        case SET_FOLDER_MSG:
            PRINTF("[%d]: SET_FOLDER_MSG\r\n", *state);
            app_set_folder("msg");
            *state = WAIT_SET_FOLDER_MSG;
            break;
        case SET_FOLDER_INBOX:
            PRINTF("[%d]: SET_FOLDER_INBOX\r\n", *state);
            app_set_folder("inbox");
            *state = WAIT_SET_FOLDER_INBOX;
            break;
        case UPDATE_INBOX:
            PRINTF("[%d]: UPDATE_INBOX\r\n", *state);
            app_update_inbox();
            *state = WAIT_UPDATE_INBOX;
            break;
        case GET_MSG_LISTING:
            PRINTF("[%d]: GET_MSG_LISTING\r\n", *state);
            memset(g_appInstance.msg_handle, 0U, BT_MAP_MSG_HANDLE_SIZE / 2);
            app_get_msg_listing(NULL,glb_message_list_cntr , 0);
            *state = WAIT_GET_MSG_LISTING;
            break;
        case GET_MSG:
            PRINTF("[%d]: GET_MSG\r\n", *state);
            app_get_msg(g_appInstance.msg_handle, false, false, 0);
            *state = WAIT_GET_MSG;
            break;
        case SET_MSG_STATUS:
            PRINTF("[%d]: SET_MSG_STATUS\r\n", *state);
            app_set_msg_status(g_appInstance.msg_handle, 0, 0);
            *state = WAIT_SET_MSG_STATUS;
            break;
        case GET_CONVO_LISTING:
            PRINTF("[%d]: GET_CONVO_LISTING\r\n", *state);
            app_get_convo_listing(10, 0);
            *state = WAIT_GET_CONVO_LISTING;
            break;

        /* Instance Information Feature */
        case GET_MAS_INST_INFO:
            PRINTF("[%d]: GET_MAS_INST_INFO\r\n", *state);
            app_get_mas_inst_info(g_appInstance.mas_instance_id, 0);
            *state = WAIT_GET_MAS_INST_INFO;
            break;
        /* Notification Feature */
        case SET_NTF_FILTER:
            PRINTF("[%d]: SET_NTF_FILTER\r\n", *state);
            app_set_ntf_filter(0);
            *state = WAIT_SET_NTF_FILTER;
            break;

        /* Notification Registration Feature */
        case SET_NTF_REG_ON:
            PRINTF("[%d]: SET_NTF_REG_ON\r\n", *state);
            app_set_ntf_reg(true);
            *state = WAIT_SET_NTF_REG_ON;
            break;
        case SET_NTF_REG_OFF:
            PRINTF("[%d]: SET_NTF_REG_OFF\r\n", *state);
            app_set_ntf_reg(false);
            *state = WAIT_SET_NTF_REG_OFF;
            break;

        /* Uploading Feature */
        case GET_OWNER_STATUS:
            PRINTF("[%d]: GET_OWNER_STATUS\r\n", *state);
            app_get_owner_status(0);
            *state = WAIT_GET_OWNER_STATUS;
            break;
        case SET_OWNER_STATUS:
            PRINTF("[%d]: SET_OWNER_STATUS\r\n", *state);
            app_set_owner_status(0);
            *state = WAIT_SET_OWNER_STATUS;
            break;
        case PUSH_MSG:
            PRINTF("[%d]: PUSH_MSG\r\n", *state);
            app_push_msg(NULL, true);
            *state = WAIT_PUSH_MSG;
            break;
        case SET_FOLDER_PARENT:
            PRINTF("[%d]: SET_FOLDER_PARENT\r\n", *state);
            app_set_folder("../");
            *state = WAIT_SET_FOLDER_PARENT;
            break;
        case SET_FOLDER_OUTBOX:
            PRINTF("[%d]: SET_FOLDER_OUTBOX\r\n", *state);
            app_set_folder("outbox");
            *state = WAIT_SET_FOLDER_OUTBOX;
            break;
        case MCE_MAS_DISCONNECT:
            PRINTF("[%d]: MCE_MAS_DISCONNECT\r\n", *state);
            app_mce_disconnect();
            *state = WAIT_MCE_MAS_DISCONNECT;
            break;
        default:
            break;
    }
}

void map_register_service() {
//    bt_sdp_register_service(&map_mce_rec);
	int err=0;
    bt_sdp_register_service(&map_mce_rec);
    err = bt_map_mce_mas_register(&map_mas_cb);
    if (0 != err)
    {
        PRINTF("fail to register MCE MAS callback (err: %d)\r\n", err);
    }
    err = bt_map_mce_mns_register(&map_mns_cb);
    if (0 != err)
    {
        PRINTF("fail to register MCE MNS callback (err: %d)\r\n", err);
    }
}

void sdp_discover_for_map_client(struct bt_conn *conn_handle)
{
    int res;
    res = bt_sdp_discover(conn_handle, &discov_map_mse);
    if (res)
    {
    	PRINTF("DUT as MAP client SDP discovery failed: result\r\n");
    }
    else
    {
    	PRINTF("DUT as MAP client SDP discovery started\r\n");
    }
}

void map_pull_message(uint8_t message_list_cntr) {
	glb_message_list_cntr  = message_list_cntr;

	if (g_appInstance.supported_features & BT_MAP_BROWSING_FEATURE)
	{
		if(glb_manual_get_list >= 1) {
			g_appInstance.state = UPDATE_INBOX;
		}else {
		g_appInstance.state = GET_FOLDER_LISTING_ROOT;
		}
//		glb_manual_get_list = 0;
		glb_manual_get_list++;
		app_state_machine();
	}
	else
	{
		PRINTF("Browsing feature is not supported");
	}
}
