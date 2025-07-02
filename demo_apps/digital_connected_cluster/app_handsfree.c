/*
 * Copyright 2021, 2024-2025 NXP
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
#include <bluetooth/hfp_hf.h>
#include <bluetooth/hfp_ag.h>
#include <bluetooth/sdp.h>
#include <bluetooth/rfcomm.h>
#include "BT_common.h"
#include "app_handsfree.h"
#include "app_handsfree_ag.h"
#include "db_gen.h"
#include "app_shell.h"
#include "app_connect.h"
#include "sco_audio_pl.h"
#include "app_pbap_pce.h"
#include "app_a2dp_sink.h"
#include "app_map_mce.h"
#include "app_a2dp_source.h"

#define HFP_HF_CODEC_ID_CVSD 0x01U
#define HFP_HF_CODEC_ID_MSBC 0x02U
#define APP_HFP_HF_INITIAL_VGS_GAIN 12
#define APP_HFP_HF_INITIAL_VGM_GAIN 12
#define HFP_CLASS_OF_DEVICE (0x200404U)
struct bt_conn *conn_rider_phone = NULL;
extern void app_hfp_hf_disconnected();

hfp_hf_get_config hfp_hf_config = {
    .bt_hfp_hf_vgs             = APP_HFP_HF_INITIAL_VGS_GAIN,
    .bt_hfp_hf_vgm             = APP_HFP_HF_INITIAL_VGM_GAIN,
};

static struct bt_sdp_attribute hfp_hf_attrs[] = {
    BT_SDP_NEW_SERVICE,
    BT_SDP_LIST(
        BT_SDP_ATTR_SVCLASS_ID_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6), //35 06
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
            BT_SDP_ARRAY_16(BT_SDP_HANDSFREE_SVCLASS) //11 1E
        },
        {
            BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
            BT_SDP_ARRAY_16(BT_SDP_GENERIC_AUDIO_SVCLASS) //12 03
        },
        )
    ),
    BT_SDP_LIST(
        BT_SDP_ATTR_PROTO_DESC_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 12),//35 10
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 3),// 35 06
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                BT_SDP_ARRAY_16(BT_SDP_PROTO_L2CAP) // 01 00
            },
            )
        },
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 5),// 35 05
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
                BT_SDP_ARRAY_16(BT_SDP_PROTO_RFCOMM) // 00 19
            },
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UINT8), //08
                BT_SDP_ARRAY_16(BT_RFCOMM_CHAN_HFP_HF) //channel number
            },
            )
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
                BT_SDP_ARRAY_16(BT_SDP_HANDSFREE_SVCLASS) //11 1E
            },
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UINT16), //09
                BT_SDP_ARRAY_16(0x0108U) //01 08
            },
            )
        },
        )
    ),
    BT_SDP_SERVICE_NAME("Handsfree"),
    BT_SDP_SUPPORTED_FEATURES(0x3400),
};
struct bt_sdp_record hfp_hf_rec = BT_SDP_RECORD(hfp_hf_attrs);

static void hf_connected(struct bt_conn *conn ,int err)
{
#if !((defined AUTO_CONNECT_USE_BOND_INFO) && (AUTO_CONNECT_USE_BOND_INFO))
    struct bt_conn_info info;
#endif

    conn_rider_phone = conn;
    sdp_discover_for_a2dp_source();

#if !((defined AUTO_CONNECT_USE_BOND_INFO) && (AUTO_CONNECT_USE_BOND_INFO))
    bt_conn_get_info(conn, &info);
#endif
}

static void hf_disconnected(struct bt_conn *conn)
{
    PRINTF("DUT as HFP-HF Disconnected with remote as HFP-AG device\n");
    g_hfNbs = 0;
    g_hfOutCall = 0;
    g_sHfpInCallingStatus = 0;
    g_sCallStatus = 0;
    app_hfp_hf_disconnected();
}

static void service(struct bt_conn *conn, uint32_t value)
{
    PRINTF("Service indicator value: %u\n", value);
}

static void call(struct bt_conn *conn, uint32_t value)
{
    if (value == 1)
    {
        g_sCallStatus = 2;
        app_hfp_ag_transfer_call_ind();
    }
    else if (value == 0)
    {
        g_sCallStatus = 0;
        app_hfp_ag_stop_incoming_call();
        app_hfp_ag_send_call_end_indicator();
        g_hfOutCall = 0;
        app_dual_a2dp_src_resume();
    }
    PRINTF("Call indicator value: %u\n", value);
}

static struct k_work setup_close_audio_work_intercom;

static void close_audio_connection_ag_intercom(struct k_work *work)
{
	app_intercom_to_rider_call();

}

static struct k_work setup_close_audio_work;

static void close_audio_connection_ag(struct k_work *work)
{
	app_hfp_ag_close_audio(RIDER_HEADSET);
}


static void call_setup(struct bt_conn *conn, uint32_t value)
{
#ifdef APP_DEBUG_EN
	PRINTF("Call Setup indicator value: %u and g_sHfpInCallingStatus %u\n", value,g_sHfpInCallingStatus);
	PRINTF("g_sCallStatus: %u\n", g_sCallStatus);
#endif

    if(g_sHfpInCallingStatus >= 0)
    {

    	if(value == 1 || value == 2)
    	{
    		app_dual_a2dp_src_pause();
    		k_work_init(&setup_close_audio_work_intercom, close_audio_connection_ag_intercom);
    		k_work_submit(&setup_close_audio_work_intercom);

    	}

    	if (value == 0 && g_sHfpInCallingStatus > 1 && (g_sCallStatus !=2))
    	{
#ifdef APP_DEBUG_EN
    		PRINTF("The call Ended: %u\n",value);
#endif
    		k_work_init(&setup_close_audio_work, close_audio_connection_ag);
    		k_work_submit(&setup_close_audio_work);
    		g_hfOutCall = 0;
    	}
    	else if (value == 2)
    	{
    		g_hfOutCall = 1;
    	}
    	g_sHfpInCallingStatus = value;
    	app_hfp_ag_send_setup_call_indication(value);
    	if (value == 0 && g_sCallStatus == 0)
    	    app_rider_call_to_intercom();
    }	
}

static void call_held(struct bt_conn *conn, uint32_t value)
{
    PRINTF("Call Held indicator value: %u\n", value);
    if (value == 1)
    {
        g_sCallStatus = 2;
    }
}

static void signal(struct bt_conn *conn, uint32_t value)
{
    PRINTF("Sig_val:%u", value);
}

static void roam(struct bt_conn *conn, uint32_t value)
{
	PRINTF("Roaming indicator value: %u\n", value);
}

static void battery(struct bt_conn *conn, uint32_t value)
{
	PRINTF("Battery indicator value: %u\n", value);
}

static void ring_cb(struct bt_conn *conn)
{
    g_sCallStatus = 1;
}
static void call_phnum(struct bt_conn *conn, char *number)
{
    PRINTF("Phone call number: %s\n", number);
    app_build_map_msg(number);
}

static void call_phname(struct bt_conn *conn, char *name)
{
    PRINTF("Phone call name: %s\n", name);
}
static void voicetag_phnum(struct bt_conn *conn, char *number)
{
    PRINTF("voice tag_phnum number: %s\n", number);
}

static void waiting_call(struct bt_conn *conn, hf_waiting_call_state_t *wcs)
{
    PRINTF("> CALL WAITING Received Number : %s\n", wcs->number);
    PRINTF("> Please use <multipcall> to handle multipe call operation\n");
    PRINTF(
        " bt multipcall 0. Release all Held Calls and set UUDB tone "
        "(Reject new incoming waiting call)\n");
    PRINTF(" bt multipcall 1. Release Active Calls and accept held/waiting call\n");
    PRINTF(
        " bt multipcall 2. Hold Active Call and accept already "
        "held/new waiting call\n");
    PRINTF(" bt multipcall 3. Conference all calls\n");
    PRINTF(" bt multipcall 4. Connect other calls and disconnect self from TWC\n");
}

static void get_peer_supported_features_ext(uint32_t bt_hfp_hf_peer_supported_features_ext)
{
#ifdef APP_DEBUG_EN
	PRINTF(" bt_hfp_hf_peer_supported_features_ext %d \n",bt_hfp_hf_peer_supported_features_ext);
#endif

    if (0 != (bt_hfp_hf_peer_supported_features_ext & 0x0200))
    {
#ifdef APP_DEBUG_EN
    	PRINTF("HFP_HF_WBS - %d\n",bt_hfp_hf_peer_supported_features_ext);
#endif
    	g_hfNbs =0;
    	if(g_agRhNbs || g_agPhNbs)
    		hfp_set_hfp_nb_codec();
    }
    else
    {
    	g_hfNbs = 1;
#ifdef APP_DEBUG_EN
    	PRINTF("HFP_HF_NBS\n");
#endif
    }
}

void hf_codec_selection(struct bt_conn *conn, uint8_t *codec)
{
	PRINTF(" hf_codec_selection %d \n",*codec);
	if( (0 == g_hfNbs) && (1 == g_agRhNbs) )
	{
		*codec = 1;
	}
}

void indicator_status(struct bt_conn *conn, hf_indicator_status_t *status)
{
    PRINTF("> ID : battchg %d\n", status->battchg);
    PRINTF("> ID : call %d\n", status->call);
    PRINTF("> ID : callheld %d\n", status->callheld);
    PRINTF("> ID : callsetup %d\n", status->callsetup);
    PRINTF("> ID : roam %d\n", status->roam);
    PRINTF("> ID : service %d\n", status->service);
    PRINTF("> ID : signal %d\n", status->signal);
}

void app_list_current_calls(struct bt_conn *conn, bt_hfp_hf_current_calls_state_t *current_calls)
{
   // printf("> current_calls : id %d\n", current_calls->id);
   //printf("> current_calls : dir %d\n", current_calls->dir);
    printf("> current_calls : status %d\n", current_calls->status);
    printf("> current_calls : mode %d\n", current_calls->mode);
    printf("> current_calls : mprty %s\n", current_calls->mprty);
    printf("> current_calls : number %s\n", current_calls->number);
   // printf("> current_calls : type %d\n", current_calls->type);
   //printf("> current_calls : alpha %s\n", current_calls->alpha);
    if(g_sCallStatus != 2)
    	printf(" update call active to rhs..\n");
}

#ifdef HFP_BRIDGING
void app_sco_connected_callback(struct bt_conn *acl, struct bt_conn *sco)
{
    struct bt_conn_info sco_info;
    uint16_t sco_handle;
    bt_conn_get_info(sco, &sco_info);
    bt_hci_get_conn_handle(sco,&sco_handle);
    uint8_t air_mode = sco_info.sco.air_mode;
    PRINTF("Phone eSCO connected, Handle: %u,air_mode : %u g_sHfpInCallingStatus %d\r\n",sco_handle,air_mode,g_sHfpInCallingStatus);

    if(g_appAgConnectInitialized)
    {
		g_phoneESCO = sco_handle;

		g_hfWbsEnable = (LMP_VOICE_AIR_CODING_TRANSPARENT == air_mode)? 1: 0; /* WBS or NBS */
		if (g_phoneESCO && g_rhsESCO)
		{
			enable_esco_bridge(g_phoneESCO,g_rhsESCO);
			return;
		}
		if(g_agRhNbs)
		{
			ag_nb_codec_select(RIDER_HEADSET,g_agRhNbs);
		}
		else
		{
			if(g_hfWbsEnable)
			{
				app_hfp_ag_codec_select(RIDER_HEADSET,HFP_HF_CODEC_ID_MSBC); /* mSBC codec */

			}else
			{
				app_hfp_ag_codec_select(RIDER_HEADSET,HFP_HF_CODEC_ID_CVSD); /* CVSD codec */
			}
		}
   }else {

	   PRINTF("\n\n Error !! Rider Headset HFP connection not present\n");
	   PRINTF("Disconnecting eSCO with Phone...\n\n");
	   bt_hfp_hf_close_audio(sco);

   }
}

void app_sco_disconnected_callback(struct bt_conn *sco, uint8_t reason) {

	PRINTF("\n Phone SCO disconnected, g_sCallStatus=%d\n",g_sCallStatus);
	if (g_sCallStatus == 2) {
		g_hfOutCall = 0;
		app_hfp_ag_stop_incoming_call();
	}
	else if(g_rhsESCO)
	{
		app_hfp_ag_close_audio(RIDER_HEADSET);
	}

	g_phoneESCO = 0;

	if(!g_rhsESCO)
		app_rider_call_to_intercom();
}
#endif

static void app_hfp_hf_get_config( hfp_hf_get_config **config)
{
    *config = &hfp_hf_config;
}
static struct bt_hfp_hf_cb hf_cb = {
    .connected       = hf_connected,
    .disconnected    = hf_disconnected,
    .service         = service,
    .call            = call,
    .call_setup      = call_setup,
    .call_held       = call_held,
    .signal          = signal,
    .roam            = roam,
    .battery         = battery,
    .ring_indication = ring_cb,
    .call_phnum      = call_phnum,
	.call_phname      = call_phname,
    .voicetag_phnum  = voicetag_phnum,
    .waiting_call    = waiting_call,
    .indicator_status = indicator_status,
    .get_config      = app_hfp_hf_get_config,
    .get_peer_supported_features_ext = get_peer_supported_features_ext,
	.list_current_calls = app_list_current_calls,
#ifdef HFP_BRIDGING
    // Add the new SCO callbacks
    .sco_connected   = app_sco_connected_callback,
    .sco_disconnected = app_sco_disconnected_callback,
	.codec_selection_cb = hf_codec_selection,
#endif
};

void hfp_hf_register_service()
{
	bt_sdp_register_service(&hfp_hf_rec);
}
void hfp_hf_init(void)
{
    int err;

    err = bt_hfp_hf_register(&hf_cb);
    if (err < 0)
    {
        PRINTF("HFP HF Registration failed (err %d)\n", err);
    }
}

int app_hfp_hf_discover(struct bt_conn *conn, uint8_t channel)
{
    int err = 0;

    if (conn_rider_phone == conn)
    {
        err = bt_hfp_hf_connect(conn_rider_phone, channel);
        if (err)
        {
            PRINTF("fail to connect hfp_hf (err: %d)\r\n", err);
        }
    }
    return err;
}

void hfp_AnswerCall(void)
{
    bt_hfp_hf_send_cmd(conn_rider_phone, BT_HFP_HF_ATA);
    g_sCallStatus = 2;
}

void hfp_RejectCall(bool reject_with_msg)
{
	if(reject_with_msg == 0)
	{
		bt_hfp_hf_send_cmd(conn_rider_phone, BT_HFP_HF_AT_CHUP);
	}
	else if(reject_with_msg == 1)
	{
		bt_hfp_hf_send_cmd(conn_rider_phone, BT_HFP_HF_AT_CHUP);
#ifdef MAP_PROFILE_ENABLE
		app_map_push_message();
#endif
	}
    g_sCallStatus = 0;
}

void hfp_set_hfp_nb_codec(void)
{
	bt_hfp_hf_send_cmd(conn_rider_phone, BT_HFP_HF_AT_BAC);
}


void dial_memory(int location)
{
    bt_hfp_hf_dial_memory(conn_rider_phone, location);
}



void hfp_start_voice_recognition(void)
{
    bt_hfp_hf_start_voice_recognition(conn_rider_phone);
}

void hfp_hf_get_last_voice_tag_number(void)
{
    bt_hfp_hf_get_last_voice_tag_number(conn_rider_phone);
}

void hfp_stop_voice_recognition(void)
{
    bt_hfp_hf_stop_voice_recognition(conn_rider_phone);
}

void hfp_volume_update(hf_volume_type_t type, int volume)
{
    bt_hfp_hf_volume_update(conn_rider_phone, type, volume);
}

void hfp_enable_ccwa(uint8_t enable)
{
    if (enable)
    {
        bt_hfp_hf_enable_call_waiting_notification(conn_rider_phone);
    }
    else
    {
        bt_hfp_hf_disable_call_waiting_notification(conn_rider_phone);
    }
}

void hfp_enable_clip(uint8_t enable)
{
    if (enable)
    {
        bt_hfp_hf_enable_clip_notification(conn_rider_phone);
    }
    else
    {
        bt_hfp_hf_disable_clip_notification(conn_rider_phone);
    }
}

void hfp_multiparty_call_option(uint8_t option)
{
    bt_hfp_hf_multiparty_call_option(conn_rider_phone, (hf_multiparty_call_option_t)option);
}

void hfp_trigger_codec_connection(void)
{
    bt_hfp_hf_trigger_codec_connection(conn_rider_phone);
}

void hfp_hf_query_list_current_calls(void)
{
    bt_hfp_hf_query_list_current_calls(conn_rider_phone);
}
