/*
 * Copyright 2024-2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <errno/errno.h>
#include <zephyr/types.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/byteorder.h>
#include <porting.h>

#include <bluetooth/hci.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include <bluetooth/l2cap.h>
#include <bluetooth/sdp.h>
#include <bluetooth/avrcp.h>
#include "fsl_shell.h"
#include "app_avrcp.h"

#include "app_connect.h"
#include "bt_pal_conn_internal.h"
#include "app_pbap_pce.h"
#include "app_a2dp_source.h"
#include "app_a2dp_sink.h"

static uint8_t g_coverNextGet;
static uint8_t g_flagVolumeInit = 0, g_ctTgVolume = 64;
uint8_t g_playStatus = 0;
uint8_t g_rspTypeSave = 0;
uint8_t g_trackChangeRegisterSrc = 0;
uint8_t g_playbackStatusChangeSrc = 0;
uint8_t g_folderUidCounter = 0;
uint16_t g_playerIdToOperate = 0xFFu;
uint8_t g_getTotalNumItems = 0;

bool g_avrcpControlConnectHfpHf = false;
bool g_browsingConnected =  false;

static struct net_buf *continue_rsp_buf;
static struct bt_avrcp_vendor_header continue_rsp_header;
#define PARSE_BUF_SIZE (1024u)
static uint8_t parse_buf[PARSE_BUF_SIZE];
#define SDP_CLIENT_USER_BUF_LEN        512U
NET_BUF_POOL_FIXED_DEFINE(app_sdp_client_pool, CONFIG_BT_MAX_CONN,SDP_CLIENT_USER_BUF_LEN,CONFIG_NET_BUF_USER_DATA_SIZE, NULL);
NET_BUF_POOL_FIXED_DEFINE(app_avrcp_continue_pool, 1u, 1024u, CONFIG_NET_BUF_USER_DATA_SIZE,NULL);


#if (defined(CONFIG_BT_BREDR) && ((CONFIG_BT_BREDR) > 0U))

uint8_t search_uid;
uint8_t now_playing_uid;
uint8_t cover_art_handle[64];
uint8_t g_coverArtHandleLen;
UCHAR ct_tg_absolute_volume[17U] = {0x00,0x09,0x11,0x1a,0x22,0x2b,0x33,0x3c,0x44,0x4c,0x55,0x5b,0x63,0x69,0x70,0x78,0x7f};
uint8_t volume_index=7;
#define FOLDER_UID    2
#define DEF_DATA(len) __aligned(4) uint8_t data[len];

#define APPL_AVRCP_MAX_PLAYER_ATTR_COUNT     4
#define APPL_AVRCP_MAX_PLAYER_ATTR_VAL_COUNT 4
#define APPL_AVRCP_MAX_ATTR_COUNT            8
#define APPL_AVRCP_MAX_PLAYER_EVENT_COUNT    13

#define BROWSE_GET_FOLDER_ITEMS 29

#if (defined(CONFIG_BT_AVRCP_COVER_ART) && ((CONFIG_BT_AVRCP_COVER_ART) > 0U))
uint8_t g_defaultCoverHandle;
static uint16_t g_coverArtXchgSize;
uint16_t sent;
static uint8_t img_properties[256u];
static uint16_t g_imagePropObjSize;
static uint32_t g_remaining;
#define IMAGE_FILE_LEN (512u * 4)
uint8_t image_data[512U];
uint16_t remote_cover_psm;
#endif

static void avrcp_auto_test(uint8_t print);
static uint8_t g_avrcpCbRegistered;

#define AVRCP_TG_SUPPORTED_FEATURES (0x01ffu)
#define AVRCP_CT_SUPPORTED_FEATURES (0x03Cfu)

static struct bt_sdp_attribute avrcp_tg_attrs[] = {
		BT_SDP_NEW_SERVICE,
		BT_SDP_LIST( //09
				BT_SDP_ATTR_SVCLASS_ID_LIST, //00 01
				BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 3), //35 03
				BT_SDP_DATA_ELEM_LIST(
						{
			BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
					BT_SDP_ARRAY_16(BT_SDP_AV_REMOTE_TARGET_SVCLASS) //11 0C
						},
				)
		),
		BT_SDP_LIST(//09
				BT_SDP_ATTR_PROTO_DESC_LIST, //00 04
				BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 16),//35 10
				BT_SDP_DATA_ELEM_LIST(
						{
			BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6),// 35 06
					BT_SDP_DATA_ELEM_LIST(
							{
				BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
						BT_SDP_ARRAY_16(BT_SDP_PROTO_L2CAP) // 01 00
							},
							{
									BT_SDP_TYPE_SIZE(BT_SDP_UINT16), //09
									BT_SDP_ARRAY_16(BT_UUID_AVCTP_VAL) // 00 17
							},
					)
						},
						{
								BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6),// 35 06
								BT_SDP_DATA_ELEM_LIST(
										{
									BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
											BT_SDP_ARRAY_16(BT_UUID_AVCTP_VAL) // 00 17
										},
										{
												BT_SDP_TYPE_SIZE(BT_SDP_UINT16), //09
												BT_SDP_ARRAY_16(0X0104u) //AVCTP version: 01 04
										},
								)
						},
				)
		),
		BT_SDP_LIST(//09
				BT_SDP_ATTR_PROFILE_DESC_LIST, //00 09
				BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 8), //35 08
				BT_SDP_DATA_ELEM_LIST(
						{
			BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6), //35 06
					BT_SDP_DATA_ELEM_LIST(
							{
				BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
						BT_SDP_ARRAY_16(BT_SDP_AV_REMOTE_SVCLASS) //11 0e
							},
							{
									BT_SDP_TYPE_SIZE(BT_SDP_UINT16), //09
									BT_SDP_ARRAY_16(0x0106U) //01 06
							},
					)
						},
				)
		),
		BT_SDP_SUPPORTED_FEATURES(AVRCP_TG_SUPPORTED_FEATURES),
};

static struct bt_sdp_record avrcp_tg_rec = BT_SDP_RECORD(avrcp_tg_attrs);

static struct bt_sdp_attribute avrcp_ct_attrs[] = {
		BT_SDP_NEW_SERVICE,
		BT_SDP_LIST( //09
				BT_SDP_ATTR_SVCLASS_ID_LIST, //00 01
				BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 3), //35 03
				BT_SDP_DATA_ELEM_LIST(
						{
			BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
					BT_SDP_ARRAY_16(BT_SDP_AV_REMOTE_CONTROLLER_SVCLASS) //11 0F
						},
				)
		),
		BT_SDP_LIST(//09
				BT_SDP_ATTR_PROTO_DESC_LIST, //00 04
				BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 16),//35 10
				BT_SDP_DATA_ELEM_LIST(
						{
			BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6),// 35 06
					BT_SDP_DATA_ELEM_LIST(
							{
				BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
						BT_SDP_ARRAY_16(BT_SDP_PROTO_L2CAP) // 01 00
							},
							{
									BT_SDP_TYPE_SIZE(BT_SDP_UINT16), //09
									BT_SDP_ARRAY_16(BT_UUID_AVCTP_VAL) // 00 17
							},
					)
						},
						{
								BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6),// 35 06
								BT_SDP_DATA_ELEM_LIST(
										{
									BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
											BT_SDP_ARRAY_16(BT_UUID_AVCTP_VAL) // 00 17
										},
										{
												BT_SDP_TYPE_SIZE(BT_SDP_UINT16), //09
												BT_SDP_ARRAY_16(0X0104u) //AVCTP version: 01 04
										},
								)
						},
				)
		),
		BT_SDP_LIST(//09
				BT_SDP_ATTR_PROFILE_DESC_LIST, //00 09
				BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 8), //35 08
				BT_SDP_DATA_ELEM_LIST(
						{
			BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6), //35 06
					BT_SDP_DATA_ELEM_LIST(
							{
				BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
						BT_SDP_ARRAY_16(BT_SDP_AV_REMOTE_SVCLASS) //11 0e
							},
							{
									BT_SDP_TYPE_SIZE(BT_SDP_UINT16), //09
									BT_SDP_ARRAY_16(0x0106U) //01 06
							},
					)
						},
				)
		),
		BT_SDP_SUPPORTED_FEATURES(AVRCP_CT_SUPPORTED_FEATURES),
};

static struct bt_sdp_record avrcp_ct_rec = BT_SDP_RECORD(avrcp_ct_attrs);
static uint8_t app_sdp_avrcp_user(struct bt_conn *conn, struct bt_sdp_client_result *result);

void avrcp_op_id_print(uint8_t op_id)
{
#ifdef APP_DEBUG_EN
	PRINTF("Opcode = VENDOR DEPENDENT\r\n");
	PRINTF("Operation ID:%d\r\n", op_id);
#endif

	switch (op_id)
	{
	case BT_AVRCP_OP_ID_SELECT:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = SELECT\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_UP:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = UP\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_DOWN:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = DOWN\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_LEFT:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = LEFT\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_RIGHT:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = RIGHT\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_RIGHT_UP:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = RIGHT UP\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_RIGHT_DOWN:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = RIGHT DOWN\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_LEFT_UP:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = LEFT UP\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_LEFT_DOWN:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = LEFT DOWN\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_ROOT_MENU:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = ROOT MENU\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_SETUP_MENU:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = SETUP MENU\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_CONTENTS_MENU:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = CONTENTS MENU\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_FAVORITE_MENU:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = FAVORITE MENU\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_EXIT:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = EXIT\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_0:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = 0\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_1:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = 1\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_2:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = 2\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_3:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = 3\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_4:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = 4\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_5:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = 5\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_6:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = 6\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_7:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = 7\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_8:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = 8\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_9:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = 9\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_DOT:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = DOT\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_ENTER:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = ENTER\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_CLEAR:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = CLEAR\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_CHANNEL_UP:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = CHANNEL UP\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_CHANNEL_DOWN:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = CHANNEL DOWN\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_PREVIOUS_CHANNEL:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = PREVIOUS CHANNEL\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_SOUND_SELECT:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = SOUND SELECT\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_INPUT_SELECT:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = INPUT SELECT\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_DISPLAY_INFORMATION:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = DISPLAY INFORMATION\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_HELP:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = HELP\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_PAGE_UP:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = PAGE UP\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_PAGE_DOWN:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = PAGE DOWN\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_POWER:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = POWER\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_VOLUME_UP:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = VOLUME UP\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_VOLUME_DOWN:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = VOLUME DOWN\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_MUTE:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = MUTE\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_PLAY:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = PLAY\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_STOP:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = STOP\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_PAUSE:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = PAUSE\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_RECORD:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = RECORD\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_REWIND:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = REWIND\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_FAST_FORWARD:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = FAST FORWARD\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_EJECT:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = EJECT\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_FORWARD:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = FORWARD\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_BACKWARD:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = BACKWARD\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_ANGLE:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = ANGLE\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_SUBPICTURE:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = SUBPICTURE\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_F1:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = F1\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_F2:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = F2\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_F3:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = F3\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_F4:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = F4\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_F5:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = F5\r\n");
#endif
		break;

	case BT_AVRCP_OP_ID_VENDOR_UNIQUE:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = VENDOR UNIQUE\r\n");
#endif
		break;

	default:
#ifdef APP_DEBUG_EN
		PRINTF("Operation ID = ???? (0x%02X)", op_id);
#endif
		break;
	}

	return;
}

void avrcp_opcode_print(uint8_t opcode)
{
	switch (opcode)
	{
	case BT_AVRCP_OPCODE_VENDOR_DEPENDENT:
#ifdef APP_DEBUG_EN
		PRINTF("Opcode = VENDOR DEPENDENT\r\n");
#endif
		break;

	case BT_AVRCP_OPCODE_UNIT_INFO:
#ifdef APP_DEBUG_EN
		PRINTF("Opcode = UNIT INFO\r\n");
#endif
		break;

	case BT_AVRCP_OPCODE_SUBUNIT_INFO:
#ifdef APP_DEBUG_EN
		PRINTF("Opcode = SUBUNIT INFO\r\n");
#endif
		break;

	case BT_AVRCP_OPCODE_PASS_THROUGH:
#ifdef APP_DEBUG_EN
		PRINTF("Opcode = PASS THROUGH\r\n");
#endif
		break;

	default:
#ifdef APP_DEBUG_EN
		PRINTF("Opcode = ???? (0x%02X)\r\n", opcode);
#endif
		break;
	}

	return;
}

void avrcp_response_type_print(uint8_t rsp_type)
{
	g_rspTypeSave = rsp_type;

	switch (rsp_type)
	{
	case BT_AVRCP_RESPONSE_TYPE_NOT_IMPLEMENTED:
#ifdef APP_DEBUG_EN
		PRINTF("Response Type = NOT IMPLEMENTED\r\n");
#endif
		break;

	case BT_AVRCP_RESPONSE_TYPE_ACCEPTED:
#ifdef APP_DEBUG_EN
		PRINTF("Response Type = ACCEPTED\r\n");
#endif
		break;

	case BT_AVRCP_RESPONSE_TYPE_REJECTED:
#ifdef APP_DEBUG_EN
		PRINTF("Response Type = REJECTED\r\n");
#endif
		break;

	case BT_AVRCP_RESPONSE_TYPE_INTERIM:
#ifdef APP_DEBUG_EN
		PRINTF("Response Type = INTERIM\r\n");
#endif
		break;

	case BT_AVRCP_RESPONSE_TYPE_STABLE:
#ifdef APP_DEBUG_EN
		PRINTF("Response Type = STABLE\r\n");
#endif
		break;

	case BT_AVRCP_RESPONSE_TYPE_CHANGED:
#ifdef APP_DEBUG_EN
		PRINTF("Response Type = CHANGED\r\n");
#endif
		break;

	default:
#ifdef APP_DEBUG_EN
		PRINTF("Response Type = ???? (0x%02X)\r\n", rsp_type);
#endif
		break;
	}

	return;
}

#if (defined(CONFIG_BT_AVRCP_TG) && ((CONFIG_BT_AVRCP_TG) > 0U))
uint8_t event_ids[] = {BT_AVRCP_EVENT_PLAYBACK_STATUS_CHANGED,
		BT_AVRCP_EVENT_TRACK_CHANGED,
		BT_AVRCP_EVENT_TRACK_REACHED_END,
		BT_AVRCP_EVENT_TRACK_REACHED_START,
		BT_AVRCP_EVENT_PLAYBACK_POS_CHANGED,
		BT_AVRCP_EVENT_BATT_STATUS_CHANGED,
		BT_AVRCP_EVENT_SYSTEM_STATUS_CHANGED,
		BT_AVRCP_EVENT_PLAYER_APP_SETTING_CHANGED,
		BT_AVRCP_EVENT_NOW_PLAYING_CONTENT_CHANGED,
		BT_AVRCP_EVENT_AVAILABLE_PLAYER_CHANGED,
		BT_AVRCP_EVENT_ADDRESSED_PLAYER_CHANGED,
		BT_AVRCP_EVENT_UIDS_CHANGED,
		BT_AVRCP_EVENT_VOLUME_CHANGED};

uint8_t registered_events[13][2];

static struct bt_sdp_discover_params discov_avrcp_tg =
{
		.uuid = BT_UUID_DECLARE_16(BT_SDP_AV_REMOTE_TARGET_SVCLASS),
		.func = app_sdp_avrcp_user,
		.pool = &app_sdp_client_pool,
};


void register_player_event(uint8_t event_id, uint8_t tl)
{
	if (event_id <= 13)
	{
		registered_events[event_id - 1][0] = 0x01;
		registered_events[event_id - 1][1] = tl;
	}
}

void avrcp_target_rsp_notify_cmd_interim(
		struct bt_conn *conn, struct bt_avrcp_control_msg *msg, uint8_t *data, uint16_t *rsp_len, uint8_t *response_type)
{
	struct bt_avrcp_event_rsp *rsp = (struct bt_avrcp_event_rsp *)data;
	struct bt_avrcp_vendor *vendor_msg       = &msg->vendor;
	uint8_t event_id                         = vendor_msg->parameter;
	*rsp_len                                 = sizeof(*rsp);

	rsp->event_id = event_id;

#ifdef APP_DEBUG_EN
	if(conn_rider_hs == conn)
			PRINTF(" RHS notify & interim \n");

	if(conn_rider_phone == conn)
			PRINTF(" Phone notify & interim \n");
#endif

	switch (event_id)
	{
	case BT_AVRCP_EVENT_PLAYBACK_STATUS_CHANGED:
#ifdef APP_DEBUG_EN
		PRINTF("Event-ID ->BT_AVRCP_EVENT_PLAYBACK_STATUS_CHANGED<0x%x>. rsp->play_status %d\n", event_id,g_playStatus);
#endif
		rsp->play_status = g_playStatus;
		register_player_event(event_id, msg->header.tl);
		g_playbackStatusChangeSrc = 1;
		break;

	case BT_AVRCP_EVENT_TRACK_CHANGED:
#ifdef APP_DEBUG_EN
		PRINTF("Event-ID - ->BT_AVRCP_EVENT_TRACK_CHANGED<0x%x>. \n", event_id);
#endif
		memset(rsp->identifier, 0, 8u);
		rsp->identifier[0] = 1u;
		register_player_event(event_id, msg->header.tl);
		g_trackChangeRegisterSrc = 1;
		break;

	case BT_AVRCP_EVENT_TRACK_REACHED_END:
	case BT_AVRCP_EVENT_TRACK_REACHED_START: /* Fall Through */
#ifdef APP_DEBUG_EN
		PRINTF("Event-ID -> %s<0x%x>.",
				event_id == BT_AVRCP_EVENT_TRACK_REACHED_END ? "BT_AVRCP_EVENT_TRACK_REACHED_END" :
						"AVRCP_EVENT_TRACK_REACHED_START",
						event_id);
#endif
		register_player_event(event_id, msg->header.tl);
		break;

	case BT_AVRCP_EVENT_PLAYBACK_POS_CHANGED:
#ifdef APP_DEBUG_EN
		PRINTF("Event-ID ->BT_AVRCP_EVENT_PLAYBACK_POS_CHANGED<0x%x>.\n", event_id);
#endif
		rsp->playback_pos = 1000;
		register_player_event(event_id, msg->header.tl);
		break;

	case BT_AVRCP_EVENT_BATT_STATUS_CHANGED:
#ifdef APP_DEBUG_EN
		PRINTF("Event-ID ->BT_AVRCP_EVENT_BATT_STATUS_CHANGED<0x%x>.\n", event_id);
#endif
		rsp->battery_status = 0;
		register_player_event(event_id, msg->header.tl);
		break;

	case BT_AVRCP_EVENT_SYSTEM_STATUS_CHANGED:
#ifdef APP_DEBUG_EN
		PRINTF("Event-ID ->BT_AVRCP_EVENT_SYSTEM_STATUS_CHANGED<0x%x>.\n", event_id);
#endif
		rsp->system_status = 0;
		register_player_event(event_id, msg->header.tl);
		break;

	case BT_AVRCP_EVENT_PLAYER_APP_SETTING_CHANGED:
	{
#ifdef APP_DEBUG_EN
		PRINTF("Event-ID ->BT_AVRCP_EVENT_PLAYER_APP_SETTING_CHANGED<0x%x>.\n", event_id);
#endif
		rsp->setting_changed.num_of_attr           = 1;
		rsp->setting_changed.attr_vals[0].attr_id  = 1;
		rsp->setting_changed.attr_vals[0].value_id = 1;

		*rsp_len = sizeof(rsp) + sizeof(struct bt_avrcp_player_attr_value);
		register_player_event(event_id, msg->header.tl);
		break;
	}

#ifdef AVRCP_1_4
	case BT_AVRCP_EVENT_NOW_PLAYING_CONTENT_CHANGED:
#ifdef APP_DEBUG_EN
		PRINTF("Event-ID ->BT_AVRCP_EVENT_PLAYER_APP_SETTING_CHANGED<0x%x>.\n", event_id);
#endif
		register_player_event(event_id, msg->header.tl);
		break;

	case BT_AVRCP_EVENT_AVAILABLE_PLAYER_CHANGED:
#ifdef APP_DEBUG_EN
		PRINTF("Event-ID ->BT_AVRCP_EVENT_AVAILABLE_PLAYER_CHANGED<0x%x>.\n", event_id);
#endif
		register_player_event(event_id, msg->header.tl);
		break;

	case BT_AVRCP_EVENT_ADDRESSED_PLAYER_CHANGED:
#ifdef APP_DEBUG_EN
		PRINTF("Event-ID ->BT_AVRCP_EVENT_ADDRESSED_PLAYER_CHANGED<0x%x>.\n", event_id);
#endif
		rsp->addressed_player_changed.player_id   = 1;
		rsp->addressed_player_changed.uid_counter = 1;
		register_player_event(event_id, msg->header.tl);
		break;

	case BT_AVRCP_EVENT_UIDS_CHANGED:
#ifdef APP_DEBUG_EN
		PRINTF( "    Event-ID ->BT_AVRCP_EVENT_UIDS_CHANGED<0x%x>.\n", event_id);
#endif
		rsp->uid_counter = 1;
		register_player_event(event_id, msg->header.tl);
		break;

	case BT_AVRCP_EVENT_VOLUME_CHANGED:
#ifdef APP_DEBUG_EN
		PRINTF( "    Event-ID ->BT_AVRCP_EVENT_VOLUME_CHANGED<0x%x>\n", event_id);
#endif
		rsp->absolute_volume = g_ctTgVolume;
		register_player_event(event_id, msg->header.tl);
		break;

#endif /* AVRCP_1_4 */
	default:
#ifdef APP_DEBUG_EN
		PRINTF( "    Event-ID -> ??? \r\n");
#endif
		break;
	}

	/* Check Event ID */
	if (event_id > BT_AVRCP_EVENT_VOLUME_CHANGED)
	{
		*response_type = BT_AVRCP_RESPONSE_TYPE_REJECTED;
		data[0]        = BT_AVRCP_METADATA_ERROR_INVALID_PARAMETER;
		*rsp_len       = 1;
	}

	return;
}

static void avrcp_target_handle_vendor_dependent_msg(struct bt_conn *conn, struct bt_avrcp_control_msg *msg)
{
	DEF_DATA(128u);
	uint8_t rj_data = BT_AVRCP_METADATA_ERROR_INVALID_PARAMETER;
	void *rsp_param;
	uint16_t rsp_len;
	uint8_t response_type              = BT_AVRCP_RESPONSE_TYPE_STABLE;
	struct bt_avrcp_vendor *vendor_msg = &msg->vendor;

	/* Command Type */
#ifdef APP_DEBUG_EN

	if(conn_rider_hs == conn)
		PRINTF("From RHS ");

	PRINTF("Command Type:");
#endif
	switch (msg->header.ctype_response)
	{
	case BT_AVRCP_COMMAND_TYPE_CONTROL:
#ifdef APP_DEBUG_EN
		PRINTF("CONTROL\r\n");
#endif
		break;

	case BT_AVRCP_COMMAND_TYPE_STATUS:
#ifdef APP_DEBUG_EN
		PRINTF("STATUS\r\n");
#endif
		break;

	case BT_AVRCP_COMMAND_TYPE_NOTIFY:
#ifdef APP_DEBUG_EN
		PRINTF("NOTIFY\r\n");
#endif
		break;

	default:
#ifdef APP_DEBUG_EN
		PRINTF("UNKNOWN\r\n");
#endif
		break;
	}

	rsp_param = NULL;
	rsp_len   = 0u;
	switch (vendor_msg->pdu_id)
	{
	case BT_AVRCP_PDU_ID_GET_CAPABILITY:
	{
		uint8_t cap_id;
		cap_id = vendor_msg->parameter;
#ifdef APP_DEBUG_EN
		PRINTF("PDU-ID -> Get Capability\n");
		PRINTF("Capability ID  = 0x%02x\n", cap_id);
#endif
		if (cap_id == BT_AVRCP_CAP_ID_COMPANY_ID)
		{
			struct bt_avrcp_capability_company_id *rsp;
			rsp = (struct bt_avrcp_capability_company_id *)&data[0];

			rsp->capability_id    = cap_id;
			rsp->capability_count = 1;
			sys_put_be24(0x1958u, (uint8_t *)&rsp->company_ids[0]);

			rsp_param = rsp;
			rsp_len   = 5u;
		}
		else if (cap_id == BT_AVRCP_CAP_ID_EVENTS_SUPPORTED)
		{
			struct bt_avrcp_capability_events_supported *rsp;
			rsp = (struct bt_avrcp_capability_events_supported *)&data[0];

			rsp->capability_id    = cap_id;
			rsp->capability_count = 13u;
			for (uint8_t index = 0; index < 13u; index++)
			{
				rsp->event_ids[index] = event_ids[index];
			}

			rsp_param = &data[0];
			rsp_len   = 15u;
		}
		else
		{
			rsp_param     = &rj_data;
			rsp_len       = sizeof(rj_data);
			response_type = BT_AVRCP_RESPONSE_TYPE_REJECTED;
		}
		break;
	}
	case BT_AVRCP_PDU_ID_LIST_PLAYER_APP_SETTING_ATTR:
	{
		struct bt_avrcp_player_app_setting_attr_ids *rsp;
		rsp = (struct bt_avrcp_player_app_setting_attr_ids *)&data[0];

		PRINTF("PDU-ID -> List Player Appl. Setting Attributes<0x%x>\r\n", vendor_msg->pdu_id);

		rsp->num_of_attr = 1;
		rsp->attr_ids[0] = 1;

		rsp_param = &data[0];
		rsp_len   = 2;
		break;
	}
	case BT_AVRCP_PDU_ID_LIST_PLAYER_APP_SETTING_VAL:
	{
		struct bt_avrcp_player_app_setting_values *rsp;
		uint8_t attr_id = vendor_msg->parameter;
		PRINTF("PDU-ID -> List Player Appl. Setting Values<0x%x>\r\n", vendor_msg->pdu_id);
		PRINTF("Attribute ID  = 0x%02x\r\n", attr_id);

		/* Validate Attribute ID */
		if (attr_id != 1)
		{
			rsp_param     = &rj_data;
			rsp_len       = sizeof(rj_data);
			response_type = BT_AVRCP_RESPONSE_TYPE_REJECTED;
			break;
		}

		rsp               = (struct bt_avrcp_player_app_setting_values *)&data[0];
		rsp->num_of_value = 2;
		rsp->value_ids[0] = 1;
		rsp->value_ids[1] = 2;
		rsp_param         = &data[0];
		rsp_len           = 3;
		break;
	}
	case BT_AVRCP_PDU_ID_GET_CUR_PLAYER_APP_SETTING_VAL:
	{
		struct bt_avrcp_player_app_setting_attr_ids *cmd =
				&vendor_msg->player_attr_ids;
		struct bt_avrcp_player_app_attr_values *rsp;

		PRINTF("PDU-ID -> Get Current Player Appl. Setting Value<0x%x>\r\n", vendor_msg->pdu_id);
		PRINTF("No. of Attrbutes: %d\r\n", cmd->num_of_attr);

		if (cmd->num_of_attr > 1)
		{
			rsp_param     = &rj_data;
			rsp_len       = sizeof(rj_data);
			response_type = BT_AVRCP_RESPONSE_TYPE_REJECTED;
			break;
		}

		rsp                        = (struct bt_avrcp_player_app_attr_values *)&data[0];
		rsp->num_of_attr           = 1;
		rsp->attr_vals[0].attr_id  = 1;
		rsp->attr_vals[0].value_id = 1;
		rsp_param                  = &data[0];
		rsp_len                    = 3u;
		break;
	}
	case BT_AVRCP_PDU_ID_SET_PLAYER_APP_SETTING_VAL:
	{
		struct bt_avrcp_player_app_attr_values *cmd =
				&vendor_msg->player_attr_values;

		PRINTF("PDU-ID -> Set Player Appl. Setting Value<0x%x>\r\n", vendor_msg->pdu_id);
		PRINTF("No. of Attrbutes: %d\r\n", cmd->num_of_attr);

		if (cmd->num_of_attr > 1)
		{
			rsp_param     = &rj_data;
			rsp_len       = sizeof(rj_data);
			response_type = BT_AVRCP_RESPONSE_TYPE_REJECTED;
			break;
		}

		rsp_param = NULL;
		rsp_len   = 0;
		break;
	}
	case BT_AVRCP_PDU_ID_GET_PLAYER_APP_SETTING_ATTR_TXT:
	{
		struct bt_avrcp_player_app_setting_attr_ids *cmd =
				&vendor_msg->player_attr_ids;
		struct bt_avrcp_player_get_txt_rsp *rsp;

		if (cmd->num_of_attr > 1)
		{
			rsp_param     = &rj_data;
			rsp_len       = sizeof(rj_data);
			response_type = BT_AVRCP_RESPONSE_TYPE_REJECTED;
			break;
		}

		rsp = (struct bt_avrcp_player_get_txt_rsp *)&data[0];
		PRINTF("PDU-ID -> Get Player Appl. Setting Attr. Text<0x%x>\r\n", vendor_msg->pdu_id);
		PRINTF("No. of Attrbutes: %d\r\n", cmd->num_of_attr);
		rsp->num_of_id              = 1;
		rsp->texts[0].attr_value_id = 1;
		rsp->texts[0].char_set      = 0x6a;
		rsp->texts[0].string_len    = 9;
		rsp->texts[0].string        = (uint8_t *)"EQUALIZER";
		rsp_param                   = &data[0];
		rsp_len                     = 5 + sizeof(struct bt_avrcp_player_attr_value_text);
		break;
	}
	case BT_AVRCP_PDU_ID_GET_PLAYER_APP_SETTING_VAL_TXT:
	{
		struct bt_avrcp_get_player_app_setting_value_text *cmd =
				&vendor_msg->player_value_text;
		struct bt_avrcp_player_get_txt_rsp *rsp;

		if (cmd->num_of_value > 1)
		{
			rsp_param     = &rj_data;
			rsp_len       = sizeof(rj_data);
			response_type = BT_AVRCP_RESPONSE_TYPE_REJECTED;
			break;
		}

		rsp = (struct bt_avrcp_player_get_txt_rsp *)&data[0];
		PRINTF("PDU-ID -> Get Player Appl. Setting Value Text<0x%x>\r\n", vendor_msg->pdu_id);
		PRINTF("Attribute ID  = 0x%02x\r\n", cmd->attr_id);
		PRINTF("No. of Attr. Values  = 0x%02x\r\n", cmd->num_of_value);
		rsp->num_of_id              = 1;
		rsp->texts[0].attr_value_id = 1;
		rsp->texts[0].char_set      = 0x6a;
		rsp->texts[0].string_len    = 3;
		rsp->texts[0].string        = (uint8_t *)"OFF";
		rsp_param                   = &data[0];
		rsp_len                     = 5 + sizeof(struct bt_avrcp_player_attr_value_text);
		break;
	}
	case BT_AVRCP_PDU_ID_INFORM_DISPLAYABLE_CHAR_SET:
	{
		struct bt_avrcp_inform_displayable_char_set *cmd =
				&vendor_msg->inform_char_set;

		PRINTF("PDU-ID -> Inform Displayable Char Set<0x%x>\r\n", vendor_msg->pdu_id);
		PRINTF("No. of Char Set = 0x%02x\r\n", cmd->num_of_char);

		for (uint8_t i = 0; i < cmd->num_of_char; i++)
		{
			PRINTF("  - 0x%04x", cmd->char_sets[i]);
		}
		rsp_param = NULL;
		rsp_len   = 0;
		break;
	}
	case BT_AVRCP_PDU_ID_INFORM_BATTERY_STATUS:
	{
		uint8_t battery_status = vendor_msg->parameter;

		PRINTF("PDU-ID -> Inform Battery Status<0x%x>\r\n", vendor_msg->pdu_id);
		PRINTF("Battery Status = 0x%02x\r\n", battery_status);

		rsp_param = NULL;
		rsp_len   = 0;
		break;
	}
	case BT_AVRCP_PDU_ID_GET_ELEMENT_ATTRIBUTE:
	{
		struct bt_avrcp_get_element_attrs *cmd =
				&vendor_msg->element_attrs;

		PRINTF("PDU-ID -> Get Element Attributes<0x%x>\r\n", vendor_msg->pdu_id);
		PRINTF("No. of Attributes: %d\r\n", cmd->num_of_attr);
		if (cmd->num_of_attr > 3)
		{
			cmd->num_of_attr = 3;
		}

		if (cmd->num_of_attr == 0)
		{
			struct bt_avrcp_player_get_element_attr_rsp *rsp =
					(struct bt_avrcp_player_get_element_attr_rsp *)&data[0];

			rsp->num_of_attr = 3;

			rsp->attrs[0].attr_id    = 1;
			rsp->attrs[0].char_set   = 0x6a;
			rsp->attrs[0].string_len = 6;
			rsp->attrs[0].string     = (uint8_t *)"title1";
			rsp->attrs[1].attr_id    = 2;
			rsp->attrs[1].char_set   = 0x6a;
			rsp->attrs[1].string_len = 7;
			rsp->attrs[1].string     = (uint8_t *)"artist1";
			rsp->attrs[2].attr_id    = 8;
			rsp->attrs[2].char_set   = 0x6a;
			rsp->attrs[2].string_len = 7;
			rsp->attrs[2].string     = (uint8_t *)"1000004";

			rsp_param = rsp;
			rsp_len =
					sizeof(struct bt_avrcp_player_get_element_attr_rsp) + sizeof(struct bt_avrcp_element_attr) * 3;
		}
		break;
	}
	case BT_AVRCP_PDU_ID_GET_PLAY_STATUS:
	{
		struct bt_avrcp_play_status_rsp *rsp = (struct bt_avrcp_play_status_rsp *)&data[0];

		rsp->song_length = 2000;
		rsp->song_pos    = 0;
		rsp->play_status = 0;

		rsp_param = rsp;
		rsp_len   = sizeof(rsp);
		break;
	}

	case BT_AVRCP_PDU_ID_REGISTER_NOTIFICATION:
#ifdef APP_DEBUG_EN
		PRINTF("PDU-ID -> Register Notification<0x%x>\r\n", vendor_msg->pdu_id);
#endif
		response_type = BT_AVRCP_RESPONSE_TYPE_INTERIM;
		avrcp_target_rsp_notify_cmd_interim(conn, msg, data, &rsp_len, &response_type);
		rsp_param = data;
		break;

	case BT_AVRCP_PDU_ID_REQUEST_CONTINUING_RESPONSE:
	{
		uint8_t pdu_id = vendor_msg->parameter;
		PRINTF("PDU-ID -> Request Continue Response<0x%x>\r\n", vendor_msg->pdu_id);
		PRINTF("Continue PDU ID: 0x%02x\r\n", pdu_id);
		rsp_param = NULL;
		rsp_len   = 0;
		if (pdu_id == BT_AVRCP_PDU_ID_LIST_PLAYER_APP_SETTING_ATTR)
		{
			struct bt_avrcp_player_app_setting_attr_ids *rsp;
			rsp = (struct bt_avrcp_player_app_setting_attr_ids *)&data[0];

			PRINTF("PDU-ID -> List Player Appl. Setting Attributes<0x%x>\r\n", vendor_msg->pdu_id);

			vendor_msg->pdu_id = pdu_id;
			rsp->num_of_attr   = 1;
			rsp->attr_ids[0]   = 1;

			rsp_param = &data[0];
			rsp_len   = 2;
		}
		else
		{
			rsp_param     = &rj_data;
			rsp_len       = sizeof(rj_data);
			response_type = BT_AVRCP_RESPONSE_TYPE_REJECTED;
		}
		break;
	}
	case BT_AVRCP_PDU_ID_ABORT_CONTINUING_RESPONSE:
	{
		uint8_t pdu_id = vendor_msg->parameter;
		PRINTF("PDU-ID -> Abort Continue Response<0x%x>\r\n", vendor_msg->pdu_id);
		PRINTF("Abort PDU ID: 0x%02x\r\n", pdu_id);
		rsp_param = NULL;
		rsp_len   = 0;
		break;
	}

	case BT_AVRCP_PDU_ID_SET_ABSOLUTE_VOLUME:
	{
		data[0] = vendor_msg->parameter;
		PRINTF("PDU-ID -> Set Absolute Volume<0x%x>\r\n", vendor_msg->pdu_id);
		PRINTF("Volume: 0x%02x\r\n", vendor_msg->parameter);
		rsp_param = &data[0];
		response_type = BT_AVRCP_RESPONSE_TYPE_ACCEPTED;
		rsp_len   = 1;
		g_ctTgVolume = vendor_msg->parameter;
		if (conn_rider_hs != NULL)
		{
			avrcp_tg_set_absolute_volume(vendor_msg->parameter);
		}

		break;
	}
	case BT_AVRCP_PDU_ID_SET_ADDRESSED_PLAYER:
	{
		uint8_t player_id  = vendor_msg->parameter;

		data[0] = BT_AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL;
		PRINTF("PDU-ID -> Set Addressed Player<0x%x>\r\n", vendor_msg->pdu_id);
		PRINTF("Player ID: 0x%04x\r\n", player_id);
		rsp_param = &data[0];
		rsp_len   = 1;
		break;
	}
	case BT_AVRCP_PDU_ID_PLAY_ITEMS:
	{
		struct bt_avrcp_play_item *play = &vendor_msg->play_item;

		data[0] = BT_AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL;
		PRINTF("PDU-ID -> Play Item<0x%x>\r\n", vendor_msg->pdu_id);
		PRINTF(" - Scope : %d", play->scope);
		PRINTF(" - UID counter : 0x%04x\r\n", play->uid_counter);
		rsp_param = &data[0];
		rsp_len = 1;
		break;
	}
	case BT_AVRCP_PDU_ID_ADD_TO_NOW_PLAYING:
	{
		struct bt_avrcp_add_to_now_playing *play = &vendor_msg->add_now_play;

		data[0] = BT_AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL;
		PRINTF("PDU-ID -> Add To Now Playing<0x%x>\r\n", vendor_msg->pdu_id);
		PRINTF(" - Scope : %d", play->scope);
		PRINTF(" - UID counter : 0x%04x\r\n", play->uid_counter);
		rsp_param = &data[0];
		rsp_len = 1;
		break;
	}

	default:
	{
		PRINTF("Unknown PDU-ID <0x%x>\r\n", vendor_msg->pdu_id);
		rsp_param     = &rj_data;
		rsp_len       = sizeof(rj_data);
		response_type = BT_AVRCP_RESPONSE_TYPE_REJECTED;
		break;
	}
	}

	bt_avrcp_response_vendor_dependent(conn, vendor_msg->pdu_id, msg->header.tl, response_type, rsp_param, rsp_len);
}

void avrcp_control_received(struct bt_conn *conn, struct bt_avrcp_control_msg *msg, int err)
{
	int ret;

	if (err)
	{
		PRINTF("error msg\r\n");
		return;
	}

	switch (msg->header.op_code)
	{
	case BT_AVRCP_OPCODE_UNIT_INFO:
	{
		ret = bt_avrcp_response_info(conn, BT_AVRCP_OPCODE_UNIT_INFO, BT_AVRCP_SUBUNIT_TYPE_PANEL, msg->header.tl);
		if (ret)
		{
#ifdef APP_DEBUG_EN
			PRINTF("fail to call bt_avrcp_response_info\n");
#endif
		}
		break;
	}
	case BT_AVRCP_OPCODE_SUBUNIT_INFO:
		ret = bt_avrcp_response_info(conn, BT_AVRCP_OPCODE_SUBUNIT_INFO, BT_AVRCP_SUBUNIT_TYPE_PANEL, msg->header.tl);
		if (ret)
		{
#ifdef APP_DEBUG_EN
			PRINTF("fail to call bt_avrcp_response_info\n");
#endif
		}
		break;
	case BT_AVRCP_OPCODE_PASS_THROUGH:

		ret = bt_avrcp_response_passthrough(conn, BT_AVRCP_RESPONSE_TYPE_ACCEPTED, msg);
		if (ret)
		{
#ifdef APP_DEBUG_EN
			PRINTF("fail to call bt_avrcp_response_passthrough\n");
#endif
		}

		if(!app_get_a2dp_mode())
		{
#ifdef APP_DEBUG_EN
			PRINTF("Forward passthrough cmd to phone\n");
#endif
			/*Need to send the command to the mobile device from the DUT*/
			ret = bt_avrcp_send_passthrough(conn_rider_phone, msg->pass_th.op_id, 0x00 ,msg->pass_th.state_flag);

			if (ret)
			{
#ifdef APP_DEBUG_EN
			PRINTF("Failed bt_avrcp_send_passthrough Press: Error Code = 0x%04X\n", ret);
#endif
			}

		}else {

			/* handle a2dp start for Rider HS or Passenger HS */
#ifdef APP_DEBUG_EN
			PRINTF("DUAL A2DP mode, receive passthrough cmd! \n");
#endif
		}


		break;
	case BT_AVRCP_OPCODE_VENDOR_DEPENDENT:
		avrcp_target_handle_vendor_dependent_msg(conn, msg);
		break;
	default:
		break;
	}
}

void avrcp_target_get_folder_items_send_reject(struct bt_conn *conn,
		struct bt_avrcp_browsing_cmd *cmd,
		uint8_t rsp_status)
{
	bt_avrcp_response_browsing(conn, cmd->header.pdu_id, cmd->header.tl, &rsp_status, 1);
}

void avrcp_target_handle_get_folder_items_req(struct bt_conn *conn, struct bt_avrcp_browsing_cmd *cmd)
{
	uint8_t rsp_status;
	uint32_t start_item;
	uint32_t end_item;
	struct bt_avrcp_item *item;
	struct bt_avrcp_get_folder_items_rsp *response;

	rsp_status = 0u;

	PRINTF("PDU ID - Get Folder Items(0x%x)\r\n", cmd->header.pdu_id);

	/* Validate the scope */
	if (cmd->folder_items.scope > BT_AVRCP_SCOPE_NOW_PLAYING)
	{
		rsp_status = BT_AVRCP_BOW_ERROR_INVALID_SCOPE;
	}

	start_item = cmd->folder_items.start_item;
	end_item   = cmd->folder_items.end_item;
	if (start_item > end_item)
	{
		rsp_status = BT_AVRCP_BOW_ERROR_RANGE_OUT_OF_BOUNDS;
	}

	if (rsp_status != 0u)
	{
		avrcp_target_get_folder_items_send_reject(conn, cmd, rsp_status);
		return;
	}

	PRINTF("Scope : %d\r\n", cmd->folder_items.scope);
	PRINTF("Start Item Index : %d\r\n", start_item);
	PRINTF("End Item Index : %d\r\n", end_item);

	if (0x00 == cmd->folder_items.attr_count)
	{
		PRINTF("Requested ALL Attributes\r\n");
	}
	else if (0xFF == cmd->folder_items.attr_count)
	{
		PRINTF("Requested NO Attributes\r\n");
	}

	switch (cmd->folder_items.scope)
	{
	case BT_AVRCP_SCOPE_MEDIA_PLAYER_LIST:
	{
		// uint16_t rsp_data_len = sizeof (*response) + sizeof (*item);
		DEF_DATA(sizeof(*response) + sizeof(*item));
		response = (struct bt_avrcp_get_folder_items_rsp *)&data[0];

		if (start_item >= 1)
		{
			rsp_status = BT_AVRCP_BOW_ERROR_RANGE_OUT_OF_BOUNDS;
			avrcp_target_get_folder_items_send_reject(conn, cmd, rsp_status);
			return;
		}

		end_item = 0;

		response->status       = BT_AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL;
		response->uid_counter  = 1u;
		response->num_of_items = 1u;
		for (uint8_t index = start_item; index < (end_item + 1); index++)
		{
			item = &response->items[index];

			item->item_type                  = BT_AVRCP_ITEM_TYPER_MEDIA_PLAYER;
			item->item_length                = 0;
			item->player_item.player_id      = 1;
			item->player_item.player_type    = 1;
			item->player_item.player_subtype = 0;
			item->player_item.play_status    = 0;
			item->player_item.player_type    = 1;
			memset(&item->player_item.feature_mask[0], 0xFFu, 16u);
			item->player_item.char_set = 0x6a;
			item->player_item.name_len = 7;
			item->player_item.name     = (uint8_t *)"player1";
		}

		bt_avrcp_response_browsing(conn, cmd->header.pdu_id, cmd->header.tl, response,
				sizeof(*response) + sizeof(*item));
		break;
	}

	case BT_AVRCP_SCOPE_VIRTUAL_FILESYSTEM:
	case BT_AVRCP_SCOPE_SEARCH:
	{
		// uint16_t rsp_data_len = sizeof (*response) + sizeof (*item);
		DEF_DATA(sizeof(*response) + sizeof(*item) * 2);
		struct bt_avrcp_item *item;

		response               = (struct bt_avrcp_get_folder_items_rsp *)&data[0];
		response->status       = BT_AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL;
		response->uid_counter  = 1;
		response->num_of_items = 2;

		item            = &response->items[0];
		item->item_type = BT_AVRCP_ITEM_TYPER_MEDIA;
		memset(item->media_item.media_uid, 0, 8);
		item->media_item.media_uid[0]       = 1;
		item->media_item.media_type         = 0;
		item->media_item.char_set           = 0x6a;
		item->media_item.name_len           = 6;
		item->media_item.name               = (uint8_t *)"media1";
		item->media_item.num_of_attr        = 3;
		item->media_item.attrs[0].attr_id   = 1;
		item->media_item.attrs[0].char_set  = 0x6a;
		item->media_item.attrs[0].value_len = 6;
		item->media_item.attrs[0].value_str = (uint8_t *)"title1";
		item->media_item.attrs[1].attr_id   = 2;
		item->media_item.attrs[1].char_set  = 0x6a;
		item->media_item.attrs[1].value_len = 7;
		item->media_item.attrs[1].value_str = (uint8_t *)"artist1";
		item->media_item.attrs[2].attr_id   = 8;
		item->media_item.attrs[2].char_set  = 0x6a;
		item->media_item.attrs[2].value_len = 7;
		item->media_item.attrs[2].value_str = (uint8_t *)"1000004";

		item            = &response->items[1];
		item->item_type = BT_AVRCP_ITEM_TYPER_FOLDER;
		memset(item->folder_item.folder_uid, 0, 8);
		item->folder_item.folder_uid[0] = 2;
		item->folder_item.folder_type   = 5;
		item->folder_item.playable      = 1;
		item->folder_item.char_set      = 0x6a;
		item->folder_item.name_len      = 7;
		item->folder_item.name          = (uint8_t *)"folder1";

		bt_avrcp_response_browsing(conn, cmd->header.pdu_id, cmd->header.tl, response,
				sizeof(*response) + sizeof(*item) * 2);
		break;
	}

	case BT_AVRCP_SCOPE_NOW_PLAYING:
	{
		// uint16_t rsp_data_len = sizeof (*response) + sizeof (*item);
		DEF_DATA(sizeof(*response) + sizeof(*item) * 1);
		struct bt_avrcp_item *item;

		response               = (struct bt_avrcp_get_folder_items_rsp *)&data[0];
		response->status       = BT_AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL;
		response->uid_counter  = 1;
		response->num_of_items = 1;

		item            = &response->items[0];
		item->item_type = BT_AVRCP_ITEM_TYPER_MEDIA;
		memset(item->media_item.media_uid, 0, 8);
		item->media_item.media_uid[0]       = 1;
		item->media_item.media_type         = 0;
		item->media_item.char_set           = 0x6a;
		item->media_item.name_len           = 6;
		item->media_item.name               = (uint8_t *)"media1";
		item->media_item.num_of_attr        = 3;
		item->media_item.attrs[0].attr_id   = 1;
		item->media_item.attrs[0].char_set  = 0x6a;
		item->media_item.attrs[0].value_len = 6;
		item->media_item.attrs[0].value_str = (uint8_t *)"title1";
		item->media_item.attrs[1].attr_id   = 2;
		item->media_item.attrs[1].char_set  = 0x6a;
		item->media_item.attrs[1].value_len = 7;
		item->media_item.attrs[1].value_str = (uint8_t *)"artist1";
		item->media_item.attrs[2].attr_id   = 8;
		item->media_item.attrs[2].char_set  = 0x6a;
		item->media_item.attrs[2].value_len = 7;
		item->media_item.attrs[2].value_str = (uint8_t *)"1000004";

		bt_avrcp_response_browsing(conn, cmd->header.pdu_id, cmd->header.tl, response,
				sizeof(*response) + sizeof(*item) * 1);
		break;
	}
	}
}

void avrcp_browsing_received(struct bt_conn *conn, struct bt_avrcp_browsing_cmd *cmd, int err)
{
	switch (cmd->header.pdu_id)
	{
	case BT_AVRCP_PDU_ID_GET_FOLDER_ITEMS:
		avrcp_target_handle_get_folder_items_req(conn, cmd);
		break;

	case BT_AVRCP_PDU_ID_SET_BROWSED_PLAYER:
	{
		uint16_t len;
		struct bt_avrcp_set_browsed_player_rsp rsp;
		uint16_t player_id = cmd->set_browsed_player.player_id;
		PRINTF("PDU ID - Set Browsed Player\r\n");
		PRINTF("Player ID - 0x%04x", player_id);

		if (player_id != 1)
		{
			rsp.status = BT_AVRCP_BOW_ERROR_INVALID_PLAYER_ID;
			len= 1;
		}
		else
		{
			rsp.status       = BT_AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL;
			rsp.uid_counter  = 1;
			rsp.num_of_items = 2;
			rsp.char_set     = 0x006A;
			rsp.folder_depth = 0; /* Setting to ROOT folder */
			len = sizeof(rsp);
		}

		bt_avrcp_response_browsing(conn, cmd->header.pdu_id, cmd->header.tl, &rsp,
				len);
		break;
	}

	case BT_AVRCP_PDU_ID_CHANGE_PATH:
	{
		struct bt_avrcp_change_path_rsp rsp;

		PRINTF("PDU ID - Change Path (0x%x)\r\n");
		rsp.status       = BT_AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL;
		rsp.num_of_items = 1;

		bt_avrcp_response_browsing(conn, cmd->header.pdu_id, cmd->header.tl, &rsp,
				sizeof(rsp));
		break;
	}

	case BT_AVRCP_PDU_ID_GET_ITEM_ATTRIBUTES:
	{
		struct bt_avrcp_get_item_attrs_rsp rsp;

		rsp.status      = BT_AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL;
		rsp.num_of_attr = 0;
		bt_avrcp_response_browsing(conn, cmd->header.pdu_id, cmd->header.tl, &rsp,
				sizeof(rsp));
		break;
	}

	case BT_AVRCP_PDU_ID_SEARCH:
	{
		struct bt_avrcp_search_rsp rsp;

		rsp.status       = BT_AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL;
		rsp.uid_counter  = 1;
		rsp.num_of_items = 1;
		bt_avrcp_response_browsing(conn, cmd->header.pdu_id, cmd->header.tl, &rsp,
				sizeof(rsp));
		break;
	}

	case BT_AVRCP_PDU_ID_GET_TOTAL_NUM_ITEMS:
	{
		struct bt_avrcp_get_total_num_of_items_rsp rsp;

		rsp.status       = BT_AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL;
		rsp.uid_counter  = 1;
		rsp.num_of_items = 2;
		bt_avrcp_response_browsing(conn, cmd->header.pdu_id, cmd->header.tl, &rsp,
				sizeof(rsp));
		break;
	}

	default:
		break;
	}
}
#endif

#if (defined(CONFIG_BT_AVRCP_CT) && ((CONFIG_BT_AVRCP_CT) > 0U))

void avrcp_print_event_nofity_rsp(uint8_t event_id, struct bt_avrcp_event_rsp *event_res)
{
#ifdef APP_AVRCP_DEBUG_EN
	PRINTF( "Event-ID:<0x%x>,g_playStatus:<0x%x>\n", event_id,event_res->play_status);
#endif

	switch (event_id)
	{
	case BT_AVRCP_EVENT_PLAYBACK_STATUS_CHANGED:
#ifdef APP_AVRCP_DEBUG_EN
		PRINTF("Event:PLAYBACK_STATUS_CHANGED :0x%x,", event_id);
#endif
		g_playStatus = event_res->play_status;
		switch (event_res->play_status)
		{
		case 0x00:
			PRINTF("PLAYBACK STOPPED\r\n");
			avrcp_tg_notify(event_id,event_res->play_status);
			break;

		case 0x01:
			PRINTF("PLAYING\r\n");
			avrcp_tg_notify(event_id,event_res->play_status);
			break;

		case 0x02:
			PRINTF("PLAYBACK PAUSED\r\n");
			avrcp_tg_notify(event_id,event_res->play_status);
			break;

		case 0x03:
			//PRINTF("FWD_SEEK\r\n");
			break;

		case 0x04:
			//PRINTF("REV_SEEK\r\n");
			break;

		case 0xFF:
#ifdef APP_AVRCP_DEBUG_EN
			PRINTF("PLAYBACK STATUS ERROR\r\n");
#endif
			break;

		default:
			PRINTF("??\r\n");
			break;
		}

		break;

		case BT_AVRCP_EVENT_TRACK_CHANGED:
#ifdef APP_AVRCP_DEBUG_EN
			PRINTF("Event: TRACK_CHANGE\r\n");
#endif
			avrcp_tg_notify(event_id,event_res->play_status);
			//avrcp_get_playsong_detail();
			break;

		case BT_AVRCP_EVENT_TRACK_REACHED_END:
#ifdef APP_AVRCP_DEBUG_EN
			PRINTF("Event: TRACK_REACHED_END\r\n");
#endif
			break;

		case BT_AVRCP_EVENT_TRACK_REACHED_START:
#ifdef APP_AVRCP_DEBUG_EN
			PRINTF("Event: TRACK_REACHED_START\r\n");
#endif
			break;

		case BT_AVRCP_EVENT_PLAYBACK_POS_CHANGED:
#ifdef APP_AVRCP_DEBUG_EN
			PRINTF("Event-ID ->BT_AVRCP_EVENT_PLAYBACK_POS_CHANGED<0x%x>\r\n", event_id);
#endif
			break;

		case BT_AVRCP_EVENT_BATT_STATUS_CHANGED:
#ifdef APP_AVRCP_DEBUG_EN
			PRINTF("Event-ID ->BT_AVRCP_EVENT_BATT_STATUS_CHANGED<0x%x>\r\n", event_id);
#endif
			break;

		case BT_AVRCP_EVENT_SYSTEM_STATUS_CHANGED:
#ifdef APP_AVRCP_DEBUG_EN
			PRINTF("Event-ID ->BT_AVRCP_EVENT_SYSTEM_STATUS_CHANGED<0x%x>\r\n", event_id);
#endif
			break;

		case BT_AVRCP_EVENT_PLAYER_APP_SETTING_CHANGED:
#ifdef APP_AVRCP_DEBUG_EN
			PRINTF("Event-ID ->BT_AVRCP_EVENT_PLAYER_APP_SETTING_CHANGED<0x%x>\r\n", event_id);
#endif
			break;

#ifdef AVRCP_1_4
		case BT_AVRCP_EVENT_NOW_PLAYING_CONTENT_CHANGED:
#ifdef APP_AVRCP_DEBUG_EN
			PRINTF("Event-ID ->BT_AVRCP_EVENT_NOW_PLAYING_CONTENT_CHANGED<0x%x>\r\n", event_id);
#endif
			break;

		case BT_AVRCP_EVENT_AVAILABLE_PLAYER_CHANGED:
#ifdef APP_AVRCP_DEBUG_EN
			PRINTF("Event-ID ->BT_AVRCP_EVENT_AVAILABLE_PLAYER_CHANGED<0x%x>\r\n", event_id);
#endif
			break;

		case BT_AVRCP_EVENT_ADDRESSED_PLAYER_CHANGED:
#ifdef APP_AVRCP_DEBUG_EN
			PRINTF("Event-ID ->BT_AVRCP_EVENT_ADDRESSED_PLAYER_CHANGED<0x%x>\r\n", event_id);
#endif
			break;

		case BT_AVRCP_EVENT_UIDS_CHANGED:
#ifdef APP_AVRCP_DEBUG_EN
			PRINTF("Event-ID ->BT_AVRCP_EVENT_UIDS_CHANGED<0x%x>\r\n", event_id);
#endif
			break;

		case BT_AVRCP_EVENT_VOLUME_CHANGED:
#ifdef APP_AVRCP_DEBUG_EN
			PRINTF("Event-ID ->BT_AVRCP_EVENT_VOLUME_CHANGED<0x%x>\r\n", event_id);
#endif
			avrcp_tg_notify(event_id,event_res->absolute_volume);
			break;

#endif /* AVRCP_1_4 */
		default:
			PRINTF("AVRCP Event-ID -> ??? \r\n");
			break;
	}
	return;
}

void avrcp_print_vendor_cmd_rsp_content(struct bt_avrcp_vendor *vendor_rsp)
{
	switch (vendor_rsp->pdu_id)
	{
	case BT_AVRCP_PDU_ID_GET_CAPABILITY:
	{
		struct bt_avrcp_capability_company_id *caps =
				&vendor_rsp->comapny_id_rsp;
		struct bt_avrcp_capability_events_supported *events =
				&vendor_rsp->events_suported_rsp;

		if (BT_AVRCP_CAP_ID_COMPANY_ID == caps->capability_id)
		{
			uint32_t company_id;
#ifdef APP_AVRCP_DEBUG_EN
			PRINTF("No. of Company ID's Supported: %d: ", caps->capability_count);
#endif
			for (uint8_t i = 0; i < caps->capability_count; i++)
			{
				/* Extract Company ID's */
				company_id = sys_get_be24((uint8_t *)&caps->company_ids[i]);
#ifdef APP_AVRCP_DEBUG_EN
				PRINTF("0x%06x,", company_id);
#endif

			}
		}
		else if (BT_AVRCP_CAP_ID_EVENTS_SUPPORTED == caps->capability_id)
		{
#ifdef APP_AVRCP_DEBUG_EN
			PRINTF("No. of Events Supported: %d: ", events->capability_count);
			for (uint8_t i = 0; i < events->capability_count; i++)
			{
				PRINTF("0x%02x,", events->event_ids[i]);
			}
#endif
		}
		else
		{
			PRINTF("Unknown Capability ID Received.\r\n");
		}
		PRINTF("\r\n");
	}
	break;

	case BT_AVRCP_PDU_ID_LIST_PLAYER_APP_SETTING_ATTR:
	{
		struct bt_avrcp_player_app_setting_attr_ids *att_ids =
				&vendor_rsp->attr_ids_rsp;

#ifdef APP_AVRCP_DEBUG_EN
		PRINTF("No. of Player Appl. Attributes = 0x%0x: ", att_ids->num_of_attr);

		for (uint8_t i = 0; i < att_ids->num_of_attr; i++)
		{
			PRINTF("0x%0x,", att_ids->attr_ids[i]);
		}
		PRINTF("\r\n");
#endif
	}

	break;

	case BT_AVRCP_PDU_ID_LIST_PLAYER_APP_SETTING_VAL:
	{
		struct bt_avrcp_player_app_setting_values *values =
				&vendor_rsp->values_rsp;

#ifdef APP_AVRCP_DEBUG_EN

		PRINTF("No. of Player Appl. Attr. Values = 0x%0x:", values->num_of_value);

		for (uint8_t i = 0; i < values->num_of_value; i++)
		{
			PRINTF("0x%0x,", values->value_ids[i]);
		}
		PRINTF("\r\n");
#endif
	}

	break;

	case BT_AVRCP_PDU_ID_GET_CUR_PLAYER_APP_SETTING_VAL:
	{
		struct bt_avrcp_player_app_attr_values *values =
				&vendor_rsp->attr_values_rsp;
#ifdef APP_AVRCP_DEBUG_EN

		PRINTF("No. of Player Appl. Attributes = 0x%0x,", values->num_of_attr);

		for (uint8_t i = 0; i < values->num_of_attr; i++)
		{
			PRINTF(" - ID: 0x%0x", values->attr_vals[i].attr_id);
			PRINTF(" - Value: 0x%0x", values->attr_vals[i].value_id);
		}
#endif
	}
	break;

	case BT_AVRCP_PDU_ID_SET_PLAYER_APP_SETTING_VAL:
		/* No params expected in the response */
		break;

	case BT_AVRCP_PDU_ID_GET_PLAYER_APP_SETTING_ATTR_TXT: /* Fall Through */
	case BT_AVRCP_PDU_ID_GET_PLAYER_APP_SETTING_VAL_TXT:
	{
		uint8_t attr_val_txt[64];
#ifdef APP_AVRCP_DEBUG_EN

		struct bt_avrcp_player_get_txt_rsp *rsp =
				&vendor_rsp->get_txt_rsp;

		for (uint8_t i = 0; i < rsp->num_of_id; i++)
		{
			/* Attribute Value Text */
			memcpy(attr_val_txt, &rsp->texts[i].string[0], rsp->texts[i].string_len);
			attr_val_txt[rsp->texts[i].string_len] = '\0';

			if (BT_AVRCP_PDU_ID_GET_PLAYER_APP_SETTING_ATTR_TXT == vendor_rsp->pdu_id)
			{
				PRINTF("Attr. ID: 0x%0x", rsp->texts[i].attr_value_id);
			}
			else
			{
				PRINTF("Attr. Val ID: 0x%0x", rsp->texts[i].attr_value_id);
			}

			PRINTF("Value Txt: %s", attr_val_txt);
		}
#endif
	}
	break;

	case BT_AVRCP_PDU_ID_INFORM_DISPLAYABLE_CHAR_SET:
		/* No params expected in the response */
		break;

	case BT_AVRCP_PDU_ID_INFORM_BATTERY_STATUS:
		/* No params expected in the response */
		break;

	case BT_AVRCP_PDU_ID_GET_ELEMENT_ATTRIBUTE:
	{
		uint8_t attr_val[64];
		struct bt_avrcp_player_get_element_attr_rsp *rsp =
				&vendor_rsp->element_attr_rsp;

#ifdef APP_AVRCP_DEBUG_EN
#endif
		PRINTF("No. of Attributes: %d", rsp->num_of_attr);
		for (uint8_t i = 0; i < rsp->num_of_attr; i++)
		{
			/* Attribute value */
			memcpy(attr_val, rsp->attrs[i].string, rsp->attrs[i].string_len);

			attr_val[rsp->attrs[i].string_len] = '\0';
#ifdef APP_AVRCP_DEBUG_EN
			PRINTF("\nID: 0x%04x ", rsp->attrs[i].attr_id);
			PRINTF("Value: %s,", attr_val);
			PRINTF("Len:%d, Value: %s", rsp->attrs[i].string_len, attr_val);
#endif

			if ((rsp->attrs[i].attr_id == 0x08u) && (rsp->attrs[i].string_len != 0u))
			{
#ifdef APP_AVRCP_DEBUG_EN
				PRINTF("save cover art handle and get image\r\n");
#endif

				memset(cover_art_handle, 0, sizeof(cover_art_handle));
				g_coverArtHandleLen = rsp->attrs[i].string_len + 1;
				memcpy(cover_art_handle, attr_val,
						rsp->attrs[i].string_len > sizeof(cover_art_handle) - 1 ?
								sizeof(cover_art_handle) - 1 : rsp->attrs[i].string_len);

				g_coverNextGet=0;
				cover_art_get_image(0);
			}
		}
	}
	break;

	case BT_AVRCP_PDU_ID_GET_PLAY_STATUS:
	{
		struct bt_avrcp_play_status_rsp *rsp =
				&vendor_rsp->play_status_rsp;

#ifdef APP_AVRCP_DEBUG_EN
		PRINTF("Current Media Play Status:\r\n");
		PRINTF(" - Song Total Length(in msec): %d", rsp->song_length);
		PRINTF(" - Song Current postion: %d", rsp->song_pos);
		PRINTF(" - Song Play Status: \r\n");

		switch (rsp->play_status)
		{
		case 0x00:
			PRINTF("0x00(STOPPED)\r\n");
			break;

		case 0x01:
			PRINTF("0x01(PLAYING)\r\n");
			break;

		case 0x02:
			PRINTF("0x02(PAUSED)\r\n");
			break;

		case 0x03:
			PRINTF("0x03(FWD_SEEK)\r\n");
			break;

		case 0x04:
			PRINTF("0x04(REV_SEEK)\r\n");
			break;

		case 0xFF:
			PRINTF("0xFF(ERROR)\r\n");
			break;

		default:
			PRINTF("??\r\n");
			break;
		}

		PRINTF("\r\n");
#endif
	}
	break;

	case BT_AVRCP_PDU_ID_REGISTER_NOTIFICATION:
	{
		struct bt_avrcp_event_rsp *rsp =
				&vendor_rsp->event_rsp;

		avrcp_print_event_nofity_rsp(rsp->event_id, rsp);
	}
	break;

	case BT_AVRCP_PDU_ID_REQUEST_CONTINUING_RESPONSE: /* Fallthrough */
	case BT_AVRCP_PDU_ID_ABORT_CONTINUING_RESPONSE:
		break;

	case BT_AVRCP_PDU_ID_SET_ABSOLUTE_VOLUME:
	{
#ifdef APP_AVRCP_DEBUG_EN
		PRINTF("Volume: 0x%02x\r\n", vendor_rsp->parameter);
#endif
	}
	break;

	case BT_AVRCP_PDU_ID_SET_ADDRESSED_PLAYER:
	case BT_AVRCP_PDU_ID_PLAY_ITEMS:         /* Fall Through */
	case BT_AVRCP_PDU_ID_ADD_TO_NOW_PLAYING: /* Fall Through */
	{
#ifdef APP_AVRCP_DEBUG_EN
		if (BT_AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL == vendor_rsp->parameter)
		{
			PRINTF("Status: Operation Completed Without error\r\n");
		}
#endif
	}
	break;

	default:
		break;
	}

	return;
}

void avrcp_print_vendor_cmd_rsp(struct bt_avrcp_vendor *vendor_rsp)
{
#ifdef APP_DEBUG_EN
	//PRINTF("avrcp_vendor_cmd_rsp:<0x%x>,packet_type:<0x%x>,param_len:<0x%x>\r\n",vendor_rsp->pdu_id,vendor_rsp->packet_type,vendor_rsp->parameter_len);
#endif

	switch (vendor_rsp->pdu_id)
	{
	case BT_AVRCP_PDU_ID_GET_CAPABILITY:
#ifdef APP_DEBUG_EN
		PRINTF("Get Capability <0x%x>.", vendor_rsp->pdu_id);
#endif
		break;

	case BT_AVRCP_PDU_ID_LIST_PLAYER_APP_SETTING_ATTR:
#ifdef APP_DEBUG_EN
		PRINTF("List Player Appl. Setting Attributes<0x%x>\r\n", vendor_rsp->pdu_id);
#endif
		break;

	case BT_AVRCP_PDU_ID_LIST_PLAYER_APP_SETTING_VAL:
#ifdef APP_DEBUG_EN
		PRINTF("List Player Appl. Setting Values<0x%x>\r\n", vendor_rsp->pdu_id);
#endif
		break;

	case BT_AVRCP_PDU_ID_GET_CUR_PLAYER_APP_SETTING_VAL:
#ifdef APP_DEBUG_EN
		PRINTF("Get Current Player Appl. Setting Value<0x%x>\r\n", vendor_rsp->pdu_id);
#endif
		break;

	case BT_AVRCP_PDU_ID_SET_PLAYER_APP_SETTING_VAL:
#ifdef APP_DEBUG_EN
		PRINTF("Set Player Appl. Setting Value<0x%x>\r\n", vendor_rsp->pdu_id);
#endif
		break;

	case BT_AVRCP_PDU_ID_GET_PLAYER_APP_SETTING_ATTR_TXT:
#ifdef APP_DEBUG_EN
		PRINTF("Get Player Appl. Setting Attr. Text<0x%x>\r\n", vendor_rsp->pdu_id);
#endif
		break;

	case BT_AVRCP_PDU_ID_GET_PLAYER_APP_SETTING_VAL_TXT:
#ifdef APP_DEBUG_EN
		PRINTF("Get Player Appl. Setting Value Text<0x%x>\r\n", vendor_rsp->pdu_id);
#endif
		break;

	case BT_AVRCP_PDU_ID_INFORM_DISPLAYABLE_CHAR_SET:
#ifdef APP_DEBUG_EN
		PRINTF("Inform Displayable Char Set<0x%x>\r\n", vendor_rsp->pdu_id);
#endif
		break;

	case BT_AVRCP_PDU_ID_INFORM_BATTERY_STATUS:
#ifdef APP_DEBUG_EN
		PRINTF("Inform Battery Status<0x%x>\r\n", vendor_rsp->pdu_id);
#endif
		break;

	case BT_AVRCP_PDU_ID_GET_ELEMENT_ATTRIBUTE:
#ifdef APP_DEBUG_EN
		PRINTF("Get Element Attributes<0x%x>\r\n", vendor_rsp->pdu_id);
#endif
		break;

	case BT_AVRCP_PDU_ID_GET_PLAY_STATUS:
#ifdef APP_DEBUG_EN
		PRINTF("Get Play Status<0x%x>\r\n", vendor_rsp->pdu_id);
#endif
		break;

	case BT_AVRCP_PDU_ID_REGISTER_NOTIFICATION:
#ifdef APP_DEBUG_EN
		PRINTF("Register Notification<0x%x>\r\n", vendor_rsp->pdu_id);
#endif
		break;

	case BT_AVRCP_PDU_ID_REQUEST_CONTINUING_RESPONSE:
#ifdef APP_DEBUG_EN
		PRINTF("Request Continue Response <0x%x>\r\n", vendor_rsp->pdu_id);
#endif
		break;

	case BT_AVRCP_PDU_ID_ABORT_CONTINUING_RESPONSE:
#ifdef APP_DEBUG_EN
		PRINTF("Abort Continue Response <0x%x>\r\n", vendor_rsp->pdu_id);
#endif
		break;

	case BT_AVRCP_PDU_ID_SET_ABSOLUTE_VOLUME:
#ifdef APP_DEBUG_EN
		PRINTF("Set Absolute Volume<0x%x>\r\n", vendor_rsp->pdu_id);
#endif
		break;

	case BT_AVRCP_PDU_ID_SET_ADDRESSED_PLAYER:
#ifdef APP_DEBUG_EN
		PRINTF("Set Addressed Player<0x%x>\r\n", vendor_rsp->pdu_id);
#endif
		break;

	case BT_AVRCP_PDU_ID_PLAY_ITEMS:
#ifdef APP_DEBUG_EN
		PRINTF("Play Item<0x%x>\r\n", vendor_rsp->pdu_id);
#endif
		break;

	case BT_AVRCP_PDU_ID_ADD_TO_NOW_PLAYING:
#ifdef APP_DEBUG_EN
		PRINTF("Add To Now Playing<0x%x>\r\n", vendor_rsp->pdu_id);
#endif
		break;

	default:
#ifdef APP_DEBUG_EN
		PRINTF("Unknown PDU-ID<0x%x>\r\n", vendor_rsp->pdu_id);
#endif
		break;
	}

	switch (vendor_rsp->packet_type)
	{
	case BT_AVRCP_PACKET_TYPE_SINGLE:
#ifdef APP_DEBUG_EN
		PRINTF("Single\r\n");
#endif
		break;

	case BT_AVRCP_PACKET_TYPE_START:
#ifdef APP_DEBUG_EN
		PRINTF("Start\r\n");
		PRINTF("====== Params are Not Displaying ======= \r\n");
		PRINTF("Take Action to Send Continue/Abort Command\r\n");
#endif
		return;
		/* break; */

	case BT_AVRCP_PACKET_TYPE_CONTINUE:
#ifdef APP_DEBUG_EN
		PRINTF("Continue\r\n");
		PRINTF("====== Params are Not Displaying ======= \r\n");
		PRINTF("Take Action to Send Continue/Abort Command\r\n");
#endif
		return;
		/* break; */

	case BT_AVRCP_PACKET_TYPE_END:
#ifdef APP_DEBUG_EN
		PRINTF("End\r\n");
		PRINTF("====== Params are Not Displaying ======= \r\n");
#endif

		/**
		 * Since the previously received fragments are not saves, parameters will not be correct.
		 * So returning from here.
		 */
		return;
		/* break; */
	}
#ifdef APP_DEBUG_EN
	PRINTF("Param Length: 0x%04x\r\n", vendor_rsp->parameter_len);
#endif

	return;
}

void avrcp_control_rsp_received(struct bt_conn *conn, struct bt_avrcp_control_msg *msg, int err)
{
	uint16_t vu_op_id;

	if ((err) || (msg == NULL))
	{
		PRINTF("respone fail\r\n");
		return;
	}

	if ((msg->header.ctype_response == BT_AVRCP_RESPONSE_TYPE_REJECTED) ||
			(msg->header.ctype_response == BT_AVRCP_RESPONSE_TYPE_NOT_IMPLEMENTED))
	{
#ifdef APP_DEBUG_EN
		PRINTF("avrcp error response\r\n");
		avrcp_response_type_print(msg->header.ctype_response);
#endif
		return;
	}

#ifdef APP_DEBUG_EN
		//avrcp_opcode_print(msg->header.op_code);
		//avrcp_response_type_print(msg->header.ctype_response);
#endif
	g_rspTypeSave = msg->header.ctype_response;

	if (msg->header.op_code == BT_AVRCP_OPCODE_PASS_THROUGH)
	{
		struct bt_avrcp_pass_through *pass_th = &msg->pass_th;

#ifdef APP_DEBUG_EN
		//avrcp_op_id_print(pass_th->op_id);
		PRINTF("Passthrough cmd, state = %s\r\n",pass_th->state_flag == 0 ? "Pressed":"Released");
#endif

	}
	else if (msg->header.op_code == BT_AVRCP_OPCODE_VENDOR_DEPENDENT)
	{

#ifdef APP_AVRCP_DEBUG_EN
		avrcp_print_vendor_cmd_rsp(&msg->vendor);
#endif
		avrcp_print_vendor_cmd_rsp_content(&msg->vendor);
	}


}

static void app_avrcp_process_continue_packet(struct bt_avrcp_vendor_header *header, struct net_buf *buf)
{
	bool continue_packet_finish = false;

	if (header->packet_type != BT_AVRCP_PACKET_TYPE_SINGLE) {
		if (header->packet_type == BT_AVRCP_PACKET_TYPE_START) {

			if (continue_rsp_buf != NULL) {
				net_buf_unref(continue_rsp_buf);
				continue_rsp_buf = NULL;
			}

			continue_rsp_buf = net_buf_alloc(&app_avrcp_continue_pool, osaWaitForever_c);
			if (continue_rsp_buf == NULL) {
				PRINTF("fail to alloc response buf\r\n");
				return;
			}
			continue_rsp_header.packet_type = BT_AVRCP_PACKET_TYPE_SINGLE; /* change the packet type as signal */
			continue_rsp_header.parameter_len = 0u;
			continue_rsp_header.pdu_id = header->pdu_id;
		}

		if (!continue_rsp_buf) {
			return;
		}

		if (net_buf_tailroom(continue_rsp_buf) >= buf->len) {
			net_buf_add_mem(continue_rsp_buf, buf->data, buf->len);
			continue_rsp_header.parameter_len += header->parameter_len;

			if (header->packet_type != BT_AVRCP_PACKET_TYPE_END) {
				if (bt_avrcp_send_vendor_dependent(conn_rider_phone, BT_AVRCP_PDU_ID_REQUEST_CONTINUING_RESPONSE, &header->pdu_id)) {
					continue_packet_finish = true;
				}
			} else {
				continue_packet_finish = true;
			}
		} else {
			if (header->packet_type != BT_AVRCP_PACKET_TYPE_END) {
				(void)bt_avrcp_send_vendor_dependent(conn_rider_phone, BT_AVRCP_PDU_ID_ABORT_CONTINUING_RESPONSE, &header->pdu_id);
			}
			continue_packet_finish = true;
		}

		if (continue_packet_finish) {
			struct bt_avrcp_vendor* vendor = bt_avrcp_vendor_rsp_parse(&continue_rsp_header, continue_rsp_buf, parse_buf, PARSE_BUF_SIZE);
			if (vendor == NULL) {
				/* the parse_buf size may need to increase, or the continue_rsp_buf's data is wrong */
				PRINTF("parse fail\r\n");
			} else {
#ifdef APP_AVRCP_DEBUG_EN
				avrcp_print_vendor_cmd_rsp(vendor);
#endif
				avrcp_print_vendor_cmd_rsp_content(vendor);
			}
			//avrcp_auto_test(0); /* test next case */
		}

		return;
	}
}

static void avrcp_vendor_dependent_continue_rsp(struct bt_conn *conn, struct bt_avrcp_vendor_header *header, struct net_buf *buf)
{
	app_avrcp_process_continue_packet(header, buf);
	net_buf_unref(buf);
}

void avrcp_browsing_rsp_received(struct bt_conn *conn, struct bt_avrcp_browsing_rsp *rsp, int err)
{
	if (err)
	{
		PRINTF("browsing rsp error\r\n");
		return;
	}

//	PRINTF("\r\nbrowsing rsp:<0x%x>\r\n",rsp->header.pdu_id);
//	PRINTF("num_of_items:%d\r\n", rsp->folder_items.num_of_items);

	switch (rsp->header.pdu_id)
	{
	case BT_AVRCP_PDU_ID_GET_FOLDER_ITEMS:
		{
			if (rsp->folder_items.status != BT_AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL)
			{
				PRINTF("browsing rsp error code: %d\r\n", rsp->folder_items.status);
				break;
			}

			PRINTF("num_of_items:%d\r\n", rsp->folder_items.num_of_items);
			for (uint8_t index = 0; index < rsp->folder_items.num_of_items; index++)
			{
//				PRINTF("item_type:%d,", rsp->folder_items.items[index].item_type);
				if (rsp->folder_items.items[index].item_type == 1u) /* Media Player Item */
				{
					PRINTF("player_id:%d\r\n", rsp->folder_items.items[index].player_item.player_id);
					if (g_playerIdToOperate == 0xFF)
					{
						g_playerIdToOperate = rsp->folder_items.items[index].player_item.player_id;
					}
					PRINTF("name_len:%d,", rsp->folder_items.items[index].player_item.name_len);
					PRINTF("name:%s,", rsp->folder_items.items[index].player_item.name);
					PRINTF("g_playStatus:%d,", rsp->folder_items.items[index].player_item.play_status);
					PRINTF("browsing:%d\r\n",(rsp->folder_items.items[index].player_item.feature_mask[7] & (0x01U << 3)) ? 1 : 0);
				}
				else if (rsp->folder_items.items[index].item_type == 2u) /* Folder Item */
				{
					PRINTF("name_len:%d,", rsp->folder_items.items[index].folder_item.name_len);
					PRINTF("name:%s\r\n", rsp->folder_items.items[index].folder_item.name);
				}
				else if (rsp->folder_items.items[index].item_type == 3u) /* Media Element Item */
				{
					PRINTF("media_type:%d,", rsp->folder_items.items[index].media_item.media_type);
					PRINTF("name_len:%d,", rsp->folder_items.items[index].media_item.name_len);
//					PRINTF("name_uid_1:%d,", rsp->folder_items.uid_counter);
					g_folderUidCounter = rsp->folder_items.uid_counter;
//					PRINTF("name_uid_2:%d\r\n,", rsp->folder_items.items[index].folder_item.folder_uid);
					PRINTF("name:%s,", rsp->folder_items.items[index].media_item.name);
					PRINTF("num_of_attr:%d\r\n", rsp->folder_items.items[index].media_item.num_of_attr);
					for (uint8_t index2 = 0; index2 < rsp->folder_items.items[index].media_item.num_of_attr; index2++)
					{
						PRINTF("attr_id:%d,", rsp->folder_items.items[index].media_item.attrs[index2].attr_id);
						PRINTF("value_len:%d, ", rsp->folder_items.items[index].media_item.attrs[index2].value_len);
						PRINTF("value_str:%s\r\n", rsp->folder_items.items[index].media_item.attrs[index2].value_str);
					}
				}
			}
			PRINTF("\r\n");
			break;
		}
		break;

	case BT_AVRCP_PDU_ID_SET_BROWSED_PLAYER:
	{
		if (rsp->set_browsed_player.status != BT_AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL)
		{
			PRINTF("browsing rsp error code: %d", rsp->set_browsed_player.status);
			break;
		}
		PRINTF("BT_AVRCP_PDU_ID_SET_BROWSED_PLAYER\r\n");
		break;
	}

	case BT_AVRCP_PDU_ID_CHANGE_PATH:
	{
		if (rsp->change_path.status != BT_AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL)
		{
			PRINTF("browsing rsp error code: %d", rsp->change_path.status);
			break;
		}
		PRINTF("BT_AVRCP_PDU_ID_CHANGE_PATH\r\n");
		break;
	}

	case BT_AVRCP_PDU_ID_GET_ITEM_ATTRIBUTES:
	{
		if (rsp->get_item_attrs.status != BT_AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL)
		{
			PRINTF("browsing rsp error code: %d", rsp->get_item_attrs.status);
			break;
		}
		PRINTF("BT_AVRCP_PDU_ID_GET_ITEM_ATTRIBUTES\r\n");
		break;
	}

	case BT_AVRCP_PDU_ID_SEARCH:
	{
		if (rsp->search.status != BT_AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL)
		{
			PRINTF("browsing rsp error code: %d", rsp->search.status);
			break;
		}
		PRINTF("BT_AVRCP_PDU_ID_SEARCH\r\n");
		break;
	}

	case BT_AVRCP_PDU_ID_GET_TOTAL_NUM_ITEMS:
	{
		if (rsp->get_total_num_of_items.status != BT_AVRCP_METADATA_ERROR_OPERATION_SUCCESSFUL)
		{
			PRINTF("browsing rsp error code: %d", rsp->get_total_num_of_items.status);
			break;
		}
		PRINTF("BT_AVRCP_PDU_ID_GET_TOTAL_NUM_ITEMS\r\n");
		PRINTF("num_of_items:%d", rsp->get_total_num_of_items.num_of_items);
		g_getTotalNumItems = rsp->get_total_num_of_items.num_of_items;
		break;
	}
	default:
		break;
	}
}

#endif

void avrcp_control_connected(struct bt_conn *conn, int err)
{
	PRINTF("avrcp control connected\r\n");

	if(conn_rider_phone == conn)
	{
		PRINTF("Get CT Capabilities !\r\n");
		uint8_t cap_id = 3;

		if (bt_avrcp_send_vendor_dependent(conn_rider_phone, BT_AVRCP_PDU_ID_GET_CAPABILITY, &cap_id))
		{
			PRINTF("fail to call bt_avrcp_send_vendor_dependent\r\n");
		}

		/*Register for events */
		/*BT_AVRCP_EVENT_PLAYBACK_STATUS_CHANGED"*/
		avrcp_ct_register_notification(conn_rider_phone, BT_AVRCP_EVENT_PLAYBACK_STATUS_CHANGED);

		/*"BT_AVRCP_EVENT_TRACK_CHANGED"*/
		avrcp_ct_register_notification(conn_rider_phone, BT_AVRCP_EVENT_TRACK_CHANGED);

	}

	if(conn_rider_hs == conn)
	{
		PRINTF("Get TG Capabilities !\r\n");
		uint8_t cap_id = 3;

		if (bt_avrcp_send_vendor_dependent(conn_rider_hs, BT_AVRCP_PDU_ID_GET_CAPABILITY, &cap_id))
		{
			PRINTF("fail to call bt_avrcp_send_vendor_dependent\r\n");
		}
		g_flagVolumeInit = 1;
		avrcp_ct_register_notification(conn_rider_hs, BT_AVRCP_EVENT_VOLUME_CHANGED);

		//send music stop indication
		if(!app_get_a2dp_mode())
		{
			if(app_get_snk_a2dp_status())
				avrcp_tg_notify(1,1);
			else
				avrcp_tg_notify(1,0);
		}
	}
}

void avrcp_control_disconnected(struct bt_conn *conn, int err)
{
	PRINTF("AVRCP control disconnected\r\n");
}

void avrcp_browsing_connected(struct bt_conn *conn, int err)
{
	PRINTF("browsing connected\r\n");
	g_browsingConnected = true;
}

void avrcp_browsing_disconnected(struct bt_conn *conn, int err)
{
	PRINTF("browsing disconnected\r\n");
	g_browsingConnected = false;
}

void avrcp_send_result(struct bt_conn *conn, int err)
{
	if (err)
	{
#ifdef APP_DEBUG_EN
		PRINTF("send fail\r\n");
#endif
	}
}

void avrcp_play_button(uint8_t press)
{
	int err;

	if(conn_rider_phone == NULL)
    	 return;

	PRINTF(" AVRCP Play button :%d\r\n",press);
	if(press)
	{
		err = bt_avrcp_send_passthrough(conn_rider_phone, BT_AVRCP_OP_ID_PLAY, 0, 0);
		if (err)
		{
			PRINTF("1_AVRCP Play button :Fail\r\n");
		}

		err = bt_avrcp_send_passthrough(conn_rider_phone, BT_AVRCP_OP_ID_PLAY, 0, 1);
		if (err)
		{
			PRINTF("2_AVRCP Play button:Fail\r\n");
		}
	}
	else
	{
		err = bt_avrcp_send_passthrough(conn_rider_phone, BT_AVRCP_OP_ID_PLAY, 0, 0);
		if (err)
		{
			PRINTF("3_AVRCP Play button:Fail\r\n");
		}

		err = bt_avrcp_send_passthrough(conn_rider_phone, BT_AVRCP_OP_ID_PLAY, 0, 1);
		if (err)
		{
			PRINTF("4_AVRCP Play button:Fail\r\n");
		}
	}
}

void avrcp_pause_button(uint8_t press)
{
	int err;

	if(conn_rider_phone == NULL)
    	 return;

	PRINTF(" AVRCP Pause button :%d\r\n",press);

	if(press)
	{
		err = bt_avrcp_send_passthrough(conn_rider_phone, BT_AVRCP_OP_ID_PAUSE, 0, 0);
		if (err)
		{
			PRINTF("1_AVRCP pause button:Fail\r\n");
		}
		err = bt_avrcp_send_passthrough(conn_rider_phone, BT_AVRCP_OP_ID_PAUSE, 0, 1);
		if (err)
		{
			PRINTF("2_AVRCP pause button:Fail\r\n");
		}
	}
	else
	{
		err = bt_avrcp_send_passthrough(conn_rider_phone, BT_AVRCP_OP_ID_PAUSE, 0, 0);
		if (err)
		{
			PRINTF("3_AVRCP pause button:Fail\r\n");
		}
		err = bt_avrcp_send_passthrough(conn_rider_phone, BT_AVRCP_OP_ID_PAUSE, 0, 1);
		if (err)
		{
			PRINTF("4_AVRCP pause button:Fail\r\n");
		}
	}
}

void avrcp_forward_backward(uint8_t ind)
{
	int err;

	if(conn_rider_phone == NULL)
    	 return;

	if(ind)
	{
		err = bt_avrcp_send_passthrough(conn_rider_phone, BT_AVRCP_OP_ID_FORWARD, 0, 0);
		if (err)
		{
			PRINTF("1_AVRCP forward/backward:Fail\r\n");
		}

		err = bt_avrcp_send_passthrough(conn_rider_phone, BT_AVRCP_OP_ID_FORWARD, 0, 1);
		if (err)
		{
			PRINTF("2_AVRCP forward/backward:Fail\r\n");
		}

	}
	else
	{
		err = bt_avrcp_send_passthrough(conn_rider_phone, BT_AVRCP_OP_ID_BACKWARD, 0, 0);
		if (err)
		{
			PRINTF("3_AVRCP forward/backward:Fail\r\n");
		}

		err = bt_avrcp_send_passthrough(conn_rider_phone, BT_AVRCP_OP_ID_BACKWARD, 0, 1);
		if (err)
		{
			PRINTF("4_AVRCP forward/backward:Fail\r\n");
		}
	}

	if (err)
	{
		PRINTF("fail to call bt_avrcp_send_passthrough\r\n");
	}

}

void avrcp_ct_set_absolute_volume(uint8_t value)
{
	uint8_t volume = value;

	if(conn_rider_phone == NULL)
    	 return;

	PRINTF("CT SetAbsoluteVolume\r\n");

	if (bt_avrcp_send_vendor_dependent(conn_rider_phone, BT_AVRCP_PDU_ID_SET_ABSOLUTE_VOLUME, &volume))
	{
		PRINTF("AVRCP set absolute volume:Fail\r\n");
	}
}

void avrcp_volume_up_down(uint8_t ind)
{
	int err;

	if(conn_rider_phone == NULL)
    	 return;

	if(ind)
	{
		/*PASSTHROUGH VOLUME CHANGE TO THE HEADSET*/
		err = bt_avrcp_send_passthrough(conn_rider_hs, BT_AVRCP_OP_ID_VOLUME_UP, 0, 0);
		if (err)
		{
			PRINTF("1_AVRCP volume UP/DOWN:Fail\r\n");
		}

		err = bt_avrcp_send_passthrough(conn_rider_hs, BT_AVRCP_OP_ID_VOLUME_UP, 0, 1);
		if (err)
		{
			PRINTF("2_AVRCP volume UP/DOWN:Fail\r\n");
		}

		/*		PASSTHROUGH VOLUME CHANGE TO THE MOBILE
		err = bt_avrcp_send_passthrough(conn_rider_phone, BT_AVRCP_OP_ID_VOLUME_UP, 0, 0);
		if (err)
		{
			PRINTF("3_AVRCP volume UP/DOWN:Fail\r\n");
		}

		err = bt_avrcp_send_passthrough(conn_rider_phone, BT_AVRCP_OP_ID_VOLUME_UP, 0, 1);
		if (err)
		{
			PRINTF("4_AVRCP volume UP/DOWN:Fail\r\n");
		}*/
	}
	else
	{
		/*PASSTHROUGH VOLUME CHANGE TO THE HEADSET*/
		err = bt_avrcp_send_passthrough(conn_rider_hs, BT_AVRCP_OP_ID_VOLUME_DOWN, 0, 0);
		if (err)
		{
			PRINTF("5_AVRCP volume UP/DOWN:Fail\r\n");
		}
		err = bt_avrcp_send_passthrough(conn_rider_hs, BT_AVRCP_OP_ID_VOLUME_DOWN, 0, 1);
		if (err)
		{
			PRINTF("6_AVRCP volume UP/DOWN:Fail\r\n");
		}

		/*		PASSTHROUGH VOLUME CHANGE TO THE MOBILE
		err = bt_avrcp_send_passthrough(conn_rider_phone, BT_AVRCP_OP_ID_VOLUME_DOWN, 0, 0);
		if (err)
		{
			PRINTF("7_AVRCP volume UP/DOWN:Fail\r\n");
		}
		err = bt_avrcp_send_passthrough(conn_rider_phone, BT_AVRCP_OP_ID_VOLUME_DOWN, 0, 1);
		if (err)
		{
			PRINTF("8_AVRCP volume UP/DOWN:Fail\r\n");
		}*/
	}
}

void avrcp_ct_register_notification(struct bt_conn *conn, uint8_t reg_notify_type)
{
	int err;
	struct bt_avrcp_register_ntfy reg;
	vTaskDelay(50);
	switch (reg_notify_type)
	{
	case BT_AVRCP_EVENT_PLAYBACK_STATUS_CHANGED:
		reg.event_id = BT_AVRCP_EVENT_PLAYBACK_STATUS_CHANGED;
		reg.playback_interval = 0;

		if (bt_avrcp_send_vendor_dependent(conn, BT_AVRCP_PDU_ID_REGISTER_NOTIFICATION, &reg))
		{
#ifdef APP_DEBUG_EN
			PRINTF("Register notification : BT_AVRCP_EVENT_PLAYBACK_STATUS_CHANGED: Fail\r\n");
#endif
		}
		else {
#ifdef APP_DEBUG_EN
			PRINTF(" Register notification: BT_AVRCP_EVENT_PLAYBACK_STATUS_CHANGED: OK \r\n");
#endif
		}
		break;
	case BT_AVRCP_EVENT_TRACK_CHANGED:
		reg.event_id = BT_AVRCP_EVENT_TRACK_CHANGED;
		reg.playback_interval = 0;

		if (bt_avrcp_send_vendor_dependent(conn, BT_AVRCP_PDU_ID_REGISTER_NOTIFICATION, &reg))
		{
#ifdef APP_DEBUG_EN
			PRINTF("Register notification : BT_AVRCP_EVENT_TRACK_CHANGED: Fail\r\n");
#endif
		}
		else {
#ifdef APP_DEBUG_EN
			PRINTF("Register notification : BT_AVRCP_EVENT_TRACK_CHANGED: OK \r\n");
#endif
		}
		break;
	case BT_AVRCP_EVENT_VOLUME_CHANGED:
		reg.event_id = BT_AVRCP_EVENT_VOLUME_CHANGED;
		reg.playback_interval = 0;

		if (bt_avrcp_send_vendor_dependent(conn, BT_AVRCP_PDU_ID_REGISTER_NOTIFICATION, &reg))
		{
#ifdef APP_DEBUG_EN
			PRINTF("Register notification : BT_AVRCP_EVENT_VOLUME_CHANGED: Fail\r\n");
#endif
		}
		else {
#ifdef APP_DEBUG_EN
			PRINTF(" Register notification : BT_AVRCP_EVENT_VOLUME_CHANGED: OK\r\n");
#endif
		}
		break;
	case BT_AVRCP_EVENT_NOW_PLAYING_CONTENT_CHANGED:
		reg.event_id = BT_AVRCP_EVENT_NOW_PLAYING_CONTENT_CHANGED;
		reg.playback_interval = 0;

		if (bt_avrcp_send_vendor_dependent(conn, BT_AVRCP_PDU_ID_REGISTER_NOTIFICATION, &reg))
		{
#ifdef APP_DEBUG_EN
			PRINTF("Register notification : BT_AVRCP_EVENT_NOW_PLAYING_CONTENT_CHANGED: Fail\r\n");
#endif
		}
		else {
#ifdef APP_DEBUG_EN
			PRINTF("Register notification : BT_AVRCP_EVENT_NOW_PLAYING_CONTENT_CHANGED: OK\r\n");
#endif
		}
		break;

	case BT_AVRCP_EVENT_PLAYBACK_POS_CHANGED:
		reg.event_id = BT_AVRCP_EVENT_PLAYBACK_POS_CHANGED;
		reg.playback_interval = 0;

		if (bt_avrcp_send_vendor_dependent(conn, BT_AVRCP_PDU_ID_REGISTER_NOTIFICATION, &reg))
		{
#ifdef APP_DEBUG_EN
			PRINTF("Register notification :BT_AVRCP_EVENT_PLAYBACK_POS_CHANGED: Fail\r\n");
#endif
		}
		else {
#ifdef APP_DEBUG_EN
			PRINTF("Register notification : BT_AVRCP_EVENT_PLAYBACK_POS_CHANGED: OK\r\n");
#endif
		}
		break;
	default:
#ifdef APP_DEBUG_EN
		PRINTF("default\r\n");
#endif
		break;
	}
}

void avrcp_ct_get_element_attributes()
{
	uint8_t pdu_id;
	int err;
	DEF_DATA(13u);
	struct bt_avrcp_get_element_attrs *attrs = (struct bt_avrcp_get_element_attrs *)&data[0];

	PRINTF("GetElementAttributes\r\n");

	memset(&attrs->identifier[0], 0, 8u);
	attrs->num_of_attr = 0u; /* get all attrs */
	pdu_id             = BT_AVRCP_PDU_ID_GET_ELEMENT_ATTRIBUTE;
	/*for (uint8_t index = 0; index < attrs->num_of_attr; index++)
	{
		attrs->attr_ids[index]=index+1;
	}*/

	if (bt_avrcp_send_vendor_dependent(conn_rider_phone, pdu_id, attrs))
	{
		PRINTF("Get CT element attribute: Fail\r\n");
	}

}

int cmd_init_ct()
{
#ifdef APP_DEBUG_EN
	PRINTF("cmd_init_ct\r\n");
#endif

	struct bt_avrcp_cb cbs = {avrcp_control_connected,
			avrcp_control_disconnected,
			avrcp_browsing_connected,
			avrcp_browsing_disconnected,
			avrcp_send_result,
			avrcp_control_received,
			avrcp_control_rsp_received,
			avrcp_vendor_dependent_continue_rsp,
			avrcp_browsing_received,
			avrcp_browsing_rsp_received};


	if (!g_avrcpCbRegistered)
	{
		g_avrcpCbRegistered = 1;
		bt_sdp_register_service(&avrcp_ct_rec);
		bt_sdp_register_service(&avrcp_tg_rec);
		bt_avrcp_register_callback(&cbs);
	}


	return 0;
}

int cmd_init_tg()
{
	struct bt_avrcp_cb cbs = {avrcp_control_connected,
			avrcp_control_disconnected,
			avrcp_browsing_connected,
			avrcp_browsing_disconnected,
			avrcp_send_result,
			avrcp_control_received,
			avrcp_control_rsp_received,
			avrcp_vendor_dependent_continue_rsp,
			avrcp_browsing_received,
			avrcp_browsing_rsp_received};

	bt_sdp_register_service(&avrcp_tg_rec);

	if (!g_avrcpCbRegistered)
	{
		PRINTF("TG: g_avrcpCbRegistered %d\r\n");

		g_avrcpCbRegistered = 1;
		bt_avrcp_register_callback(&cbs);
	}
	PRINTF("cmd_init_tg success\r\n\n");

	return 0;
}

void avrcp_control_connect(struct bt_conn *default_conn)
{
	int err;
	if (err = bt_avrcp_control_connect(default_conn))
	{
		PRINTF("AVRCP control connect fail with err %d\n", err);
	}
	else
	{
		PRINTF("AVRCP control connect success\r\n");
	}
}

void cmd_control_connect()
{
	int err;

	if(conn_rider_phone == NULL)
    	 return;

	if (err = bt_avrcp_control_connect(conn_rider_phone))
	{
		PRINTF("AVRCP command control connect fail with err %d\n", err);
	}
	else
	{
		PRINTF("AVRCP command control connect success\r\n");
	}
}

void cmd_browsing_connect()
{
	int err;

	if(conn_rider_phone == NULL)
    	 return;

	if (err = bt_avrcp_browsing_connect(conn_rider_phone))
	{
		PRINTF("AVRCP browsing connect fail with err %d\r\n", err);
	}
	else
	{
		PRINTF("AVRCP browsing connect success\r\n");
	}
}

#if (defined(CONFIG_BT_AVRCP_COVER_ART) && ((CONFIG_BT_AVRCP_COVER_ART) > 0U))

void avrcp_cover_art_connected(uint8_t handle, struct bt_conn *conn, int err)
{
	PRINTF("connected\r\n");
}

void avrcp_cover_art_disconnected(uint8_t handle, int err)
{
	PRINTF("disconnected\r\n");
}

static void cover_art_get_image(uint8_t print)
{
	switch (g_coverNextGet)
	{
	case 0:
	{
		struct bt_avrcp_get_image_property param;

		PRINTF("0.bt_avrcp_get_image_property\r\n");
		if (print)
		{
			break;
		}

		cover_art_set_handle(&param.image_handle, &param.image_handle_len);
		param.wait             = 0;
		if (bt_avrcp_get_image_property(g_defaultCoverHandle, &param))
		{
			PRINTF("fail to call bt_avrcp_get_image_property\r\n");
		}
		break;
	}
	case 1:
	{
		struct bt_avrcp_get_image param;

		PRINTF("\n1.bt_avrcp_get_image\r\n");
		if (print)
		{
			break;
		}

		param.image_descriptor_data     = NULL;
		param.image_descriptor_data_len = 0u;
		cover_art_set_handle(&param.image_handle, &param.image_handle_len);
		param.wait                      = 0;
		if (bt_avrcp_get_image(g_defaultCoverHandle, &param))
		{
			PRINTF("fail to call bt_avrcp_get_image\r\n");
		}
		break;
	}
	default:
		break;
	}
}

void avrcp_cover_art_cmd_received(uint8_t handle, struct bt_avrcp_cover_art_cmd *cmd, int err)
{
	struct bt_avrcp_cover_art_rsp rsp;
	uint8_t send_response = 1;

	PRINTF("cover art cmd:\r\n");
	rsp.cmd = cmd->cmd;
	switch (cmd->cmd)
	{
	case BT_AVRCP_COVER_ART_CONNECT:
	{
		PRINTF("BT_AVRCP_COVER_ART_CONNECT\r\n");
		PRINTF("MAX Exchange Size: %d", cmd->connect.max_recv_size);
		g_coverArtXchgSize = cmd->connect.max_recv_size;
		rsp.response        = BT_AVRCP_CA_SUCCESS_RSP;
		break;
	}
	case BT_AVRCP_COVER_ART_GET_PROP:
	{
		struct bt_avrcp_get_image_property *get = &cmd->get_prop;

		PRINTF("BT_AVRCP_COVER_ART_GET_PROP\r\n");
		if (sent == 0)
		{
			rsp.response = BT_AVRCP_CA_SUCCESS_RSP;
			if ((get->image_handle == NULL) || (get->image_handle_len == 0))
			{
				rsp.response = BT_AVRCP_CA_BAD_REQ_RSP;
			}
			memset(&img_properties[0], 0, sizeof(img_properties));
			snprintf((char *)img_properties, sizeof(img_properties) - 1,
					"<image-properties version=\"1.0\" handle="
					"\"%s\"> \n"
					"<native encoding=\"JPEG\" pixel=\"640*480\" size=\"10240\"/>\n"
					"<variant encoding=\"GIF\" pixel=\"640*480\"/>\n"
					"<variant encoding=\"JPEG\" pixel=\"200*200\"/>\n"
					"<variant encoding=\"JPEG\" pixel=\"75*75\"/>\n"
					"</image-properties>",
					get->image_handle);

			g_imagePropObjSize = (uint16_t)strlen((char *)img_properties);
			g_remaining           = g_imagePropObjSize;
		}

		if (g_remaining > g_coverArtXchgSize)
		{
			rsp.get_prop.length = g_coverArtXchgSize;
			rsp.response        = BT_AVRCP_CA_CONTINUE_RSP;
		}
		else if (g_remaining != 0)
		{
			rsp.get_prop.length = (uint16_t)g_remaining;
			rsp.response        = BT_AVRCP_CA_SUCCESS_RSP;
		}
		else
		{
			rsp.get_prop.length = 0u;
			rsp.response        = BT_AVRCP_CA_SUCCESS_RSP;
		}

		rsp.get_prop.data = &img_properties[sent];
		sent += rsp.get_prop.length;
		g_remaining -= rsp.get_prop.length;
		if (g_remaining == 0)
		{
			g_imagePropObjSize = 0;
			sent                = 0;
		}
		break;
	}

	case BT_AVRCP_COVER_ART_GET_IMAGE:
	{
		struct bt_avrcp_get_image *get = &cmd->get_image;

		PRINTF("BT_AVRCP_COVER_ART_GET_IMAGE\r\n");
		if (sent == 0)
		{
			rsp.response = BT_AVRCP_CA_SUCCESS_RSP;
			if ((get->image_handle == NULL) || (get->image_handle_len == 0))
			{
				rsp.response = BT_AVRCP_CA_BAD_REQ_RSP;
			}

			if ((get->image_descriptor_data != NULL) && (get->image_descriptor_data_len != 0))
			{
				// to do:
			}

			g_remaining = IMAGE_FILE_LEN;
		}

		if (g_remaining > g_coverArtXchgSize)
		{
			rsp.get_image.length = g_coverArtXchgSize;
			rsp.response         = BT_AVRCP_CA_CONTINUE_RSP;
		}
		else if (g_remaining != 0)
		{
			rsp.get_image.length = (uint16_t)g_remaining;
			rsp.response         = BT_AVRCP_CA_SUCCESS_RSP;
		}
		else
		{
			rsp.get_image.length = 0u;
			rsp.response        = BT_AVRCP_CA_SUCCESS_RSP;
		}

		rsp.get_image.data = &image_data[0];
		sent += rsp.get_image.length;
		g_remaining -= rsp.get_image.length;
		if (g_remaining == 0)
		{
			sent = 0;
		}

		break;
	}

	case BT_AVRCP_COVER_ART_GET_THUMB:
	{
		struct bt_avrcp_get_linked_thumbnail *get = &cmd->get_thumb;

		PRINTF("BT_AVRCP_COVER_ART_GET_THUMB\r\n");
		if (sent == 0)
		{
			rsp.response = BT_AVRCP_CA_SUCCESS_RSP;
			if ((get->image_handle == NULL) || (get->image_handle_len == 0))
			{
				rsp.response = BT_AVRCP_CA_BAD_REQ_RSP;
			}

			g_remaining = IMAGE_FILE_LEN;
		}

		if (g_remaining > g_coverArtXchgSize)
		{
			rsp.get_thumb.length = g_coverArtXchgSize;
			rsp.response         = BT_AVRCP_CA_CONTINUE_RSP;
		}
		else if (g_remaining != 0)
		{
			rsp.get_thumb.length = (uint16_t)g_remaining;
			rsp.response         = BT_AVRCP_CA_SUCCESS_RSP;
		}
		else
		{
			rsp.get_thumb.length = 0u;
			rsp.response        = BT_AVRCP_CA_SUCCESS_RSP;
		}

		rsp.get_thumb.data = &image_data[0];
		sent += rsp.get_thumb.length;
		g_remaining -= rsp.get_thumb.length;
		if (g_remaining == 0)
		{
			sent = 0;
		}
		break;
	}

	case BT_AVRCP_COVER_ART_ABORT:
		PRINTF("BT_AVRCP_COVER_ART_ABORT\r\n");
		send_response = 0;
		break;

	default:
		break;
	}

	if (send_response)
	{
		bt_avrcp_response_cover_art(handle, rsp.cmd, rsp.response, &rsp.get_prop);
	}
}

void avrcp_cover_art_rsp_received(uint8_t handle, struct bt_avrcp_cover_art_rsp *rsp, int err)
{
	uint8_t next         = 0;
	uint8_t send_request = 0;
	PRINTF("cover art rsp:\r\n");
	switch (rsp->cmd)
	{
	case BT_AVRCP_COVER_ART_CONNECT:
	{
		PRINTF("BT_AVRCP_COVER_ART_CONNECT\r\n");
		PRINTF("MAX Exchange Size: %d", rsp->connect.max_recv_size);
		g_coverArtXchgSize = rsp->connect.max_recv_size;
		break;
	}
	case BT_AVRCP_COVER_ART_GET_PROP:
	{
		uint8_t str[128u];
		PRINTF("BT_AVRCP_COVER_ART_GET_PROP\r\n");
		memcpy(str, rsp->get_prop.data, (128u > rsp->get_prop.length) ? rsp->get_prop.length : 128u);
		str[127] = 0;
		PRINTF("%s", str);
		if (rsp->response == BT_AVRCP_CA_CONTINUE_RSP)
		{
			send_request = 1;
		}
		else
		{
			next = 1;
		}
		break;
	}

	case BT_AVRCP_COVER_ART_GET_IMAGE:
	{
		PRINTF("BT_AVRCP_COVER_ART_GET_IMAGE\r\n");
		PRINTF("image file length:%d, data len:%d", rsp->get_image.image_file_size,
				rsp->get_image.length);
		if (rsp->response == BT_AVRCP_CA_CONTINUE_RSP)
		{
			send_request = 1;
		}
		else
		{
			next = 1;
		}
		break;
	}

	case BT_AVRCP_COVER_ART_GET_THUMB:
	{
		PRINTF("BT_AVRCP_COVER_ART_GET_THUMB\r\n");
		PRINTF("data len:%d", rsp->get_thumb.length);
		if (rsp->response == BT_AVRCP_CA_CONTINUE_RSP)
		{
			send_request = 1;
		}
		else
		{
			next = 1;
		}
		break;
	}

	case BT_AVRCP_COVER_ART_ABORT:
		PRINTF("BT_AVRCP_COVER_ART_ABORT\r\n");
		break;

	default:
		break;
	}

	if (next == 1)
	{
		g_coverNextGet++;
		cover_art_get_image(0);
	}
	else if (send_request == 1)
	{
		bt_avrcp_send_request(handle, 0, rsp);
	}
}
#endif

static shell_status_t cmd_ca_init_responder(shell_handle_t shell, int32_t argc, char *argv[])
{
	struct bt_avrcp_cover_art_cb cb = {avrcp_cover_art_connected, avrcp_cover_art_disconnected,
			avrcp_cover_art_cmd_received, NULL};

	bt_avrcp_register_cover_art_cb(&cb);
	if (bt_avrcp_cover_art_start_responder(&g_defaultCoverHandle))
	{
		PRINTF(shell, "fail to call bt_avrcp_cover_art_start_responder\r\n");
	}
	else
	{
		PRINTF(shell, "success\r\n");
	}
	return kStatus_SHELL_Success;
}

void  avrcp_ca_init_initiator()
{
	struct bt_avrcp_cover_art_cb cb = {avrcp_cover_art_connected, avrcp_cover_art_disconnected, NULL,
			avrcp_cover_art_rsp_received};

	bt_avrcp_register_cover_art_cb(&cb);
	if (bt_avrcp_cover_art_start_initiator(&g_defaultCoverHandle))
	{
		PRINTF("fail to call bt_avrcp_cover_art_start_responder\r\n");
	}
	else
	{
		PRINTF("cover art init success\r\n");
	}

}

static void cover_art_set_handle(uint8_t **image_handle, uint16_t *image_handle_len)
{
	if (g_coverArtHandleLen > 0)
	{
		PRINTF("use cover art handle from test case 18\r\n");
		*image_handle     = cover_art_handle;
		*image_handle_len = g_coverArtHandleLen;
	}
	else
	{
		PRINTF("use cover art handle 1000004\r\n");
		*image_handle     = (uint8_t *)"1000004";
		*image_handle_len = 8u;
	}
}


void check_browsing_connected()
{
	if(g_browsingConnected == false)
	{
		cmd_browsing_connect();
	}
}

void avrcp_play_folder_items(uint8_t select_index)
{
	struct bt_avrcp_play_item play;

	uint8_t pdu_id;

	PRINTF("PlayItem(NowPlayingList)\r\n");

	play.scope = 3; /* NowPlayingList */
	memset(play.uid, 0, 8);
	play.uid[7]      = select_index;
	play.uid_counter = g_folderUidCounter;
	pdu_id           = BT_AVRCP_PDU_ID_PLAY_ITEMS;
	PRINTF("UID is %d\r\n",play.uid[7]);
	PRINTF("UID Counter is %d\r\n", play.uid_counter);
	if (bt_avrcp_send_vendor_dependent(conn_rider_phone, pdu_id, &play))
	{
		PRINTF("AVRCP play folder items : fail\r\n");
	}
}

#if 0
void avrcp_get_folder_items()
{
	check_browsing_connected();
	struct bt_avrcp_get_folder_items_cmd param;
	PRINTF("GetFolderItems(NowPlayingList)\r\n\n");

	param.scope      = BT_AVRCP_SCOPE_MEDIA_PLAYER_LIST; /* NowPlayingList */
	param.start_item = 0;
	param.end_item   = 0;
	param.attr_count = 0; /* all attr */

	if (bt_avrcp_get_folder_items(conn_rider_phone, &param))
	{
		PRINTF("AVRCP get folder items : fail\r\n");
	}

	param.scope      = BT_AVRCP_SCOPE_NOW_PLAYING; /* NowPlayingList */
	param.start_item = 0;
	param.end_item   = 3;
	param.attr_count = 0; /* all attr */

	if (bt_avrcp_get_folder_items(conn_rider_phone, &param))
	{
		PRINTF("AVRCP get folder items : fail\r\n");
	}
}
#endif

void avrcp_get_folder_items()
{
    check_browsing_connected();
	struct bt_avrcp_get_folder_items_cmd param;

	PRINTF("GetFolderItems(NowPlayingList)\r\n\n");
	uint16_t batch_size = 3; // Define batch size
	uint16_t start_item = 0;

	if(bt_avrcp_get_total_num_of_items(conn_rider_phone,BT_AVRCP_SCOPE_NOW_PLAYING))
	{
		PRINTF("fail to call bt_avrcp_get_total_num_items");
	}

	if(g_getTotalNumItems == 0)
	{
		PRINTF("No items available in NowPlayingList\r\n");
		return;
	}
	else
	{
		while (start_item < g_getTotalNumItems)
		{
			param.scope = BT_AVRCP_SCOPE_NOW_PLAYING;
			param.start_item = start_item;
			param.end_item = (start_item + batch_size - 1) < g_getTotalNumItems ? (start_item + batch_size - 1) : (g_getTotalNumItems - 1);
			param.attr_count = 0; // Retrieve all attributes

			if (bt_avrcp_get_folder_items(conn_rider_phone, &param))
			{
				PRINTF("Failed to call bt_avrcp_get_folder_items for range %d - %d\r\n", param.start_item, param.end_item);
				break;
			}
			else
			{
//				PRINTF("Requested folder items: %d - %d\r\n", param.start_item, param.end_item);
			}

			start_item += batch_size; // Move to the next batch
		}
	}
}
void avrcp_browse_play_folder_items(uint8_t select_index)
{
	PRINTF("avrcp_browse_play_folder_items\r\n");
	avrcp_play_folder_items(select_index);
}

void avrcp_get_playsong_detail()
{
	check_browsing_connected();
	if(g_browsingConnected == true)
	{
		PRINTF("avrcp_get_play_song_details\r\n");
		struct bt_avrcp_get_folder_items_cmd param;

		param.scope      = BT_AVRCP_SCOPE_NOW_PLAYING; /* NowPlayingList */
		param.start_item = 0;
		param.end_item   = 1; /* get the status of playing song only */
		param.attr_count = 0; /* all attr */

		if (bt_avrcp_get_folder_items(conn_rider_phone, &param))
		{
			PRINTF("AVRCP get play song details : fail\r\n");
		}
	}
}

void  avrcp_browse_get_folder_items()
{
	PRINTF("avrcp_browse_get_folder_items\r\n");
	avrcp_get_folder_items();
}

void  avrcp_ca_get_image()
{
	avrcp_ct_get_element_attributes();
}

static shell_status_t cmd_ca_init_initiator(shell_handle_t shell, int32_t argc, char *argv[])
{
	struct bt_avrcp_cover_art_cb cb = {avrcp_cover_art_connected, avrcp_cover_art_disconnected, NULL,
			avrcp_cover_art_rsp_received};

	bt_avrcp_register_cover_art_cb(&cb);
	if (bt_avrcp_cover_art_start_initiator(&g_defaultCoverHandle))
	{
		PRINTF(shell, "fail to call bt_avrcp_cover_art_start_responder\r\n");
	}
	else
	{
		PRINTF(shell, "success\r\n");
	}
	return kStatus_SHELL_Success;
}

static uint8_t app_sdp_avrcp_user(struct bt_conn *conn,
		struct bt_sdp_client_result *result)
{
	uint16_t param;
	int res;

	if ((result) && (result->resp_buf))
	{
		PRINTF("\n ca sdp callback\r\n");
		res = bt_sdp_get_addl_proto_param(result->resp_buf, BT_SDP_PROTO_L2CAP, 1, &param);
		if (res < 0)
		{
			return BT_SDP_DISCOVER_UUID_CONTINUE;
		}
		else
		{
			struct bt_avrcp_cover_art_connect connect;

			connect.l2cap_rpsm    = param;
			connect.max_recv_size = 512u;
			if (bt_avrcp_cover_art_connect(g_defaultCoverHandle, conn_rider_phone, &connect))
			{
				PRINTF("fail to call bt_avrcp_cover_art_connect\r\n");
			}
			return BT_SDP_DISCOVER_UUID_STOP;
		}
	}
	else
	{
		PRINTF("sdp fail callback\r\n");
		return BT_SDP_DISCOVER_UUID_CONTINUE;
	}
}

void  avrcp_ca_init_connect()
{
	int res = bt_sdp_discover(conn_rider_phone, &discov_avrcp_tg);
	if (res)
	{
		PRINTF("AVRCP SDP discovery failed\r\n");
	}
	else
	{
		PRINTF("AVRCP SDP discovery started\r\n");
	}
}

static shell_status_t cmd_ct_reg_ntf(shell_handle_t shell, int32_t argc, char *argv[])
{

	uint8_t event;
	uint32_t playback_interval = 0;
	struct bt_avrcp_register_ntfy reg;

	if (argc == 2) {
		event = strtoul(argv[1], NULL, 16);
	} else {
		PRINTF(shell, "wrong parameter\r\n");
		return kStatus_SHELL_Success;
	}

	if (event > 0x0du) {
		PRINTF(shell, "wrong parameter\r\n");
		return kStatus_SHELL_Success;
	}

	if (event == BT_AVRCP_EVENT_PLAYBACK_POS_CHANGED){
		playback_interval = 5;
	}

	reg.event_id = event;
	reg.playback_interval = playback_interval;
	//   test_enable = 0;
	if (bt_avrcp_send_vendor_dependent(conn_rider_phone, BT_AVRCP_PDU_ID_REGISTER_NOTIFICATION, &reg))
	{
		PRINTF("fail to call bt_avrcp_send_vendor_dependent\r\n");
	}

	return kStatus_SHELL_Success;
}

int avrcp_tg_notify_playback_status_changed()
{
	struct bt_avrcp_event_rsp *rsp;
	DEF_DATA(32u);
	uint16_t rsp_len;
	uint8_t event;

	event=BT_AVRCP_EVENT_PLAYBACK_STATUS_CHANGED;


	if (g_rspTypeSave != BT_AVRCP_RESPONSE_TYPE_INTERIM && !app_get_a2dp_mode())
	{
		/*BT_AVRCP_EVENT_PLAYBACK_STATUS_CHANGED"*/
		avrcp_ct_register_notification(conn_rider_phone, BT_AVRCP_EVENT_PLAYBACK_STATUS_CHANGED);
	}

	rsp = (struct bt_avrcp_event_rsp *)&data[0];
	rsp_len = sizeof(*rsp);
	rsp->event_id = event;
	rsp->play_status = g_playStatus;

	if((g_playbackStatusChangeSrc == 1) && (g_rspTypeSave != BT_AVRCP_RESPONSE_TYPE_INTERIM))
	{
		/*Need to send the Playback status register inbetween DUT --> Headset(Link-2)*/
		bt_avrcp_response_vendor_dependent(conn_rider_hs, BT_AVRCP_PDU_ID_REGISTER_NOTIFICATION,
				registered_events[event - 1][1], g_rspTypeSave,
				rsp, rsp_len);
		g_playbackStatusChangeSrc = 0;
	}
	return 0;
}

int avrcp_tg_notify_track_changed()
{
	struct bt_avrcp_event_rsp *rsp;
	DEF_DATA(32u);
	uint16_t rsp_len;
	uint8_t event;

	event=BT_AVRCP_EVENT_TRACK_CHANGED;


	if (g_rspTypeSave != BT_AVRCP_RESPONSE_TYPE_INTERIM && !app_get_a2dp_mode())
	{
		/*BT_AVRCP_EVENT_TRACK_CHANGED"*/
		if(conn_rider_phone != NULL)
			avrcp_ct_register_notification(conn_rider_phone, BT_AVRCP_EVENT_TRACK_CHANGED);

	}

	rsp = (struct bt_avrcp_event_rsp *)&data[0];
	rsp_len = sizeof(*rsp);
	rsp->event_id = event;

	if((g_trackChangeRegisterSrc == 1) && (g_rspTypeSave != BT_AVRCP_RESPONSE_TYPE_INTERIM))
	{
		/*Need to send the Playback status register inbetween DUT --> Headset(Link-2)*/
		bt_avrcp_response_vendor_dependent(conn_rider_hs, BT_AVRCP_PDU_ID_REGISTER_NOTIFICATION,
				registered_events[event - 1][1], g_rspTypeSave,
				rsp, rsp_len);
		g_trackChangeRegisterSrc = 0;
	}

	return 0;
}

int avrcp_tg_notify_volume_changed(uint8_t value)
{
	struct bt_avrcp_event_rsp *rsp;
	DEF_DATA(32u);
	uint16_t rsp_len = sizeof(*rsp);
	uint8_t event;

	rsp = (struct bt_avrcp_event_rsp *)&data[0];
	event = BT_AVRCP_EVENT_VOLUME_CHANGED;
	g_ctTgVolume = value;
	PRINTF("Send Volume changed status\n");

	if (g_rspTypeSave != BT_AVRCP_RESPONSE_TYPE_INTERIM)
	{
		/*BT_AVRCP_EVENT_VOLUME_CHANGED for the Link-2"*/
		avrcp_ct_register_notification(conn_rider_hs, BT_AVRCP_EVENT_VOLUME_CHANGED);
	}
	else if(g_flagVolumeInit == 1)
	{
		g_flagVolumeInit = 0;
		if(conn_rider_phone != NULL)
		{
			avrcp_tg_set_absolute_volume(g_ctTgVolume);
		}
	}
	else if (conn_rider_phone != NULL && !app_get_a2dp_mode())
	{
		rsp->absolute_volume = value;
		rsp->event_id = BT_AVRCP_EVENT_VOLUME_CHANGED;
		/* Send Volume Changed event to mobile */
		bt_avrcp_response_vendor_dependent(conn_rider_phone, BT_AVRCP_PDU_ID_REGISTER_NOTIFICATION,
				registered_events[event - 1][1],  BT_AVRCP_RESPONSE_TYPE_CHANGED,
				rsp, rsp_len);
	}
	return 0;
}

void avrcp_tg_set_absolute_volume(uint8_t value)
{
	uint8_t volume=10;
	volume=value;

	if(conn_rider_hs == NULL)
    	 return;

	PRINTF("TG Set_Absolute_Volume\r\n");

	if (bt_avrcp_send_vendor_dependent(conn_rider_hs, BT_AVRCP_PDU_ID_SET_ABSOLUTE_VOLUME, &volume))
	{
		PRINTF("fail to set Set_Absolute_Volume\r\n");
	}
}

int avrcp_tg_notify(uint8_t event,uint8_t value)
{
	struct bt_avrcp_event_rsp *rsp;
	DEF_DATA(32u);
	uint16_t rsp_len;

#ifdef APP_DEBUG_EN
	PRINTF("\navrcp_tg_notify Event:%d,value:%d\n",event, value);
#endif
	if(event == 1)
	{
		/*Playback status changed*/
		g_playStatus=value;
		avrcp_tg_notify_playback_status_changed();
	}

	if(event == 2)
	{
		/*Track changed*/
		avrcp_tg_notify_track_changed();
	}

	if(event == 13)
	{
		/*Volume changed*/
		avrcp_tg_notify_volume_changed(value);
		return 0;
	}

	rsp = (struct bt_avrcp_event_rsp *)&data[0];
	rsp_len = sizeof(*rsp);
	rsp->event_id = event;
	rsp->play_status = value;

	switch (event)
	{
	case BT_AVRCP_EVENT_PLAYBACK_STATUS_CHANGED:
		rsp->play_status = g_playStatus;
		break;

	case BT_AVRCP_EVENT_TRACK_CHANGED:
		memset(rsp->identifier, 0, 8u);
		rsp->identifier[0] = 1u;
		break;

	case BT_AVRCP_EVENT_TRACK_REACHED_END:
	case BT_AVRCP_EVENT_TRACK_REACHED_START: /* Fall Through */
		break;

	case BT_AVRCP_EVENT_PLAYBACK_POS_CHANGED:
		rsp->playback_pos = 1000;
		break;

	case BT_AVRCP_EVENT_BATT_STATUS_CHANGED:
		rsp->battery_status = 0;
		break;

	case BT_AVRCP_EVENT_SYSTEM_STATUS_CHANGED:
		rsp->system_status = 0;
		break;

	case BT_AVRCP_EVENT_PLAYER_APP_SETTING_CHANGED:
	{
		rsp->setting_changed.num_of_attr           = 1;
		rsp->setting_changed.attr_vals[0].attr_id  = 1;
		rsp->setting_changed.attr_vals[0].value_id = 1;

		rsp_len = sizeof(rsp) + sizeof(struct bt_avrcp_player_attr_value);
		break;
	}

	case BT_AVRCP_EVENT_NOW_PLAYING_CONTENT_CHANGED:
		break;

	case BT_AVRCP_EVENT_AVAILABLE_PLAYER_CHANGED:
		break;

	case BT_AVRCP_EVENT_ADDRESSED_PLAYER_CHANGED:
		rsp->addressed_player_changed.player_id   = 1;
		rsp->addressed_player_changed.uid_counter = 1;
		break;

	case BT_AVRCP_EVENT_UIDS_CHANGED:
		rsp->uid_counter = 1;
		break;

	case BT_AVRCP_EVENT_VOLUME_CHANGED:
		rsp->absolute_volume = value;
		break;

	default:
		break;
	}
//TODO: //needtobecheck
	bt_avrcp_response_vendor_dependent(conn_rider_hs, BT_AVRCP_PDU_ID_REGISTER_NOTIFICATION,
			registered_events[event - 1][1], BT_AVRCP_RESPONSE_TYPE_CHANGED,
			rsp, rsp_len);
	return 0;
}

#endif /* CONFIG_BT_BREDR */
