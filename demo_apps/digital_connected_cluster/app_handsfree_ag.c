/*
 * Copyright 2020 - 2021,2024-2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_debug_console.h"

#include "FreeRTOS.h"
#include "task.h"

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
#include <bluetooth/hfp_ag.h>
#include <bluetooth/sdp.h>
#include <bluetooth/rfcomm.h>
#include "BT_common.h"
#include "sco_audio_pl.h"
#include "BT_config.h"
#include "app_handsfree_ag.h"
#include "app_handsfree.h"
#include "app_connect.h"
#include "app_shell.h"
#include "app_a2dp_source.h"
#include "clock_config.h"

/* User may need to change it for real production */
#define APP_CLASS_OF_DEVICE (0x200000U)
#define HFP_AG_CODEC_ID_CVSD                         0x01U
#define HFP_AG_CODEC_ID_MSBC                         0x02U
struct bt_conn *conn_rider_hs = NULL;
struct bt_conn *conn_passenger_hs = NULL;

volatile uint8_t g_sCallStatus = 0;
uint8_t g_hfWbsEnable = 0, g_agWbsEnable = 0,g_hfOutCall = 0;
uint8_t g_sHfpInCallingStatus = 0xff,g_hfNbs =0 ,g_agRhNbs = 0 , g_agPhNbs = 0;

uint8_t g_sHfpAgRhDisconnecting =0,g_sHfpAgPhDisconnecting =0;
uint8_t g_intercomEnabled;
uint8_t g_intercomPaused =0;

uint16_t g_phoneESCO = 0;
uint16_t g_rhsESCO = 0;
uint16_t g_phsESCO = 0;

app_hfp_ag_t g_HfpAgs[CONFIG_BT_HFP_AG_MAX_CONN];
app_hfp_ag_t *g_HfpAg;
static app_hfp_ag_t *g_HfpAg_ph;
uint8_t g_appAgConnectInitialized = 0;

void hfp_ag_sco_connected_callback(struct bt_hfp_ag *ag, struct bt_conn *sco_conn);
void hfp_ag_sco_disconnected_callback(struct bt_hfp_ag *ag);

struct k_work_delayable setup_audio_ag_work;
#define SETUP_AUDIO_DELAY K_MSEC(200)

static struct bt_sdp_attribute hfp_ag_attrs[] = {
    BT_SDP_NEW_SERVICE,
    BT_SDP_LIST(
        BT_SDP_ATTR_SVCLASS_ID_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6), //35 06
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
            BT_SDP_ARRAY_16(BT_SDP_HANDSFREE_AGW_SVCLASS) //11 1F
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
                BT_SDP_ARRAY_16(BT_RFCOMM_CHAN_HFP_AG) //channel number
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
    BT_SDP_SERVICE_NAME("Handsfree_ag"),
    BT_SDP_SUPPORTED_FEATURES(0x2100),
};
static struct bt_sdp_record hfp_ag_rec = BT_SDP_RECORD(hfp_ag_attrs);

/* app_ag_save_instance and app_ag_remove_instance are only called in the same hfp callback one by one,
 * so don't need mutext to protect allocating/freeing g_HfpAgs.
 */
static app_hfp_ag_t *app_ag_save_instance(struct bt_hfp_ag *hfp_ag)
{
    for (uint8_t index = 0; index < APP_MAX_HFP_AG_CONN; index++)
    {
        if (g_HfpAgs[index].hfp_agHandle == hfp_ag)
        {
            return &g_HfpAgs[index];
        }
    }

    for (uint8_t index = 0; index < APP_MAX_HFP_AG_CONN; index++)
    {
        if (g_HfpAgs[index].hfp_agHandle == NULL)
        {
            g_HfpAgs[index].hfp_agHandle = hfp_ag;
            return &g_HfpAgs[index];
        }
    }

    return NULL;
}

static void app_ag_remove_instance(struct bt_hfp_ag *hfp_ag)
{
    for (uint8_t index = 0; index < APP_MAX_HFP_AG_CONN; index++)
    {
        if (g_HfpAgs[index].hfp_agHandle == hfp_ag)
        {
            g_HfpAgs[index].hfp_agHandle = NULL;
            break;
        }
    }
}

static app_hfp_ag_t *app_ag_get_instance(struct bt_hfp_ag *hfp_ag)
{
    for (uint8_t index = 0; index < APP_MAX_HFP_AG_CONN; index++)
    {
        if (g_HfpAgs[index].hfp_agHandle == hfp_ag)
        {
            return &g_HfpAgs[index];
        }
    }

    return NULL;
}

static uint8_t app_ag_instance_index(struct bt_hfp_ag *hfp_ag)
{
    for (uint8_t index = 0; index < APP_MAX_HFP_AG_CONN; index++)
    {
        if (g_HfpAgs[index].hfp_agHandle == hfp_ag)
        {
            return index;
        }
    }

    return 0;
}

static bool app_ag_check_idle_instance(void)
{
    for (uint8_t index = 0; index < APP_MAX_HFP_AG_CONN; index++)
    {
        if (g_HfpAgs[index].hfp_agHandle == NULL)
        {
            return true;
        }
    }

    return false;
}


static void ag_connected(struct bt_hfp_ag *hfp_ag, int err)
{
	struct bt_conn *ag_conn;
	ag_conn= bt_hfp_ag_get_conn(hfp_ag);
	if (conn_rider_hs == ag_conn)
    {

#ifdef APP_DEBUG_EN
         PRINTF("Rider HS HFP-AG connected !\r\n");
#endif

		 g_HfpAg = app_ag_save_instance(hfp_ag);

		 g_HfpAg->hfp_agHandle = hfp_ag;
		 g_HfpAg->hfp_in_calling_status = 1;
		 g_HfpAg->selectCodec = 1;

		 g_appAgConnectInitialized = 1;
		 app_hfp_ag_codec_select(RIDER_HEADSET,1);


	 } else if (conn_passenger_hs == ag_conn)
	 {

#ifdef APP_DEBUG_EN
         PRINTF("Passenger HS HFP-AG connected !\r\n");
#endif
		 g_HfpAg_ph = app_ag_save_instance(hfp_ag);

	  	 g_HfpAg_ph->hfp_agHandle = hfp_ag;
	 	 g_HfpAg_ph->hfp_in_calling_status = 1;
	 	 g_HfpAg_ph->selectCodec = 1;
	     app_hfp_ag_codec_select(PASSENGER_HEADSET,1);

	 } else
	 {
		PRINTF("Too many connections\n");
		bt_conn_unref(ag_conn);
		bt_hfp_ag_disconnect(hfp_ag);
		return;
	 }
	 bt_conn_unref(ag_conn);
}

static struct k_work setup_close_audio_work_hf;

static void close_audio_connection_hf(struct k_work *work)
{
	    // disconnect SCO with phone.
		bt_hfp_sco_disconnect(conn_rider_phone);

}

static void ag_disconnected(struct bt_hfp_ag *hfp_ag)
{
	struct bt_conn *ag_conn;
    ag_conn= bt_hfp_ag_get_conn(hfp_ag);

    if (conn_rider_hs == ag_conn)
   	{
#ifdef APP_DEBUG_EN
         PRINTF("Rider HS HFP-AG Disconnected !\r\n");
#endif
   		 g_HfpAg->hfp_agHandle = NULL;
   		 g_agRhNbs = 0;
   		 g_appAgConnectInitialized = 0;
    	 if(app_hfp_intercom_status())
    		app_enable_intercom(0);

 		 k_work_init(&setup_close_audio_work_hf, close_audio_connection_hf);
 		 k_work_submit(&setup_close_audio_work_hf);

   	 } else if  (conn_passenger_hs == ag_conn)
   	 {
#ifdef APP_DEBUG_EN
         PRINTF("Passenger HS HFP-AG Disconnected !\r\n");
#endif
   		 g_HfpAg_ph->hfp_agHandle= NULL;
   		 g_agPhNbs = 0;
   	 }
    bt_conn_unref(ag_conn);
    app_ag_remove_instance(hfp_ag);
    bt_hfp_ag_disconnect(hfp_ag);
}

hfp_ag_get_config hfp_ag_config = {
    .bt_hfp_ag_vgs             = 15,
    .bt_hfp_ag_vgm             = 15,
    .bt_hfp_ag_codec           = 1,
    .bt_hfp_ag_nrec            = 1,
    .bt_hfp_ag_inband          = 0,
    .bt_hfp_ag_codec_negotiate = 0,
    .bt_hfp_ag_dial            = 1,
};

static void bt_work_ata_response(struct k_work *work)
{
	app_hfp_ag_t *app_hfp_ag = CONTAINER_OF(work, app_hfp_ag_t, ataRespWork);
#ifdef APP_DEBUG_EN
    PRINTF("HFP HF have accepted the call !\n");
#endif

    app_hfp_ag->hfp_in_calling_status = 3;
            //bt_hfp_ag_send_callsetup_indicator(app_hfp_ag->hfp_agHandle, 0);
            bt_hfp_ag_send_call_indicator(app_hfp_ag->hfp_agHandle, 1);
            if (app_hfp_ag->xTimers != 0)
    {
            	xTimerStop(app_hfp_ag->xTimers, 0);
            	        	        xTimerDelete(app_hfp_ag->xTimers, 0);
            	        	        app_hfp_ag->xTimers = 0;
    }
            bt_hfp_ag_call_status_pl(app_hfp_ag->hfp_agHandle, hfp_ag_call_call_incoming);

}

void hfp_ag_register_service()
{
    bt_sdp_register_service(&hfp_ag_rec);
}

void bt_work_init_ag()
{
    for (uint8_t index = 0; index < CONFIG_BT_HFP_AG_MAX_CONN; index++)
    {
        k_work_init(&g_HfpAgs[index].ataRespWork, bt_work_ata_response);
        g_HfpAgs[index].hfp_in_calling_status = 0xFF;
        g_HfpAgs[index].hfp_agHandle = NULL;
    }

    g_HfpAg_ph->hfp_agHandle = NULL;
    app_hfp_ag_select_conn(0);

}

static void setup_audio_ag_connection(struct k_work *work)
{
	PRINTF("trigger audio connection \n");
	//hfp_trigger_codec_connection();
}

void at_brsf(struct bt_hfp_ag *hfp_ag, uint32_t hf_features)
{
	struct bt_conn *ag_conn;
	ag_conn= bt_hfp_ag_get_conn(hfp_ag);

	if (conn_rider_hs == ag_conn)
	{
		g_appAgConnectInitialized = 1;
	}

	if (0 != (hf_features & 0x0200))
	{
#ifdef APP_DEBUG_EN
		PRINTF("HF Feature :HFP_AG_WBS\n");
#endif
	}
	else
	{

		if (conn_rider_hs == ag_conn)
		{
#ifdef APP_DEBUG_EN
		PRINTF("RH HF Feature :HFP_AG_NBS\n");
#endif
			g_agRhNbs = 1;
			hfp_set_hfp_nb_codec();
		}

		if (conn_passenger_hs == ag_conn)
		{
#ifdef APP_DEBUG_EN
		PRINTF("PH HF Feature :HFP_AG_NBS\n");
#endif
			g_agPhNbs = 1;
			hfp_set_hfp_nb_codec();
		}
	}
	//if call exist then setup eSCO
	if(g_sCallStatus == 2 && !g_phoneESCO)
	{
		k_work_init_delayable(&setup_audio_ag_work, setup_audio_ag_connection);
		k_work_schedule(&setup_audio_ag_work, SETUP_AUDIO_DELAY);
	}
	bt_conn_unref(ag_conn);
}

void dial(struct bt_hfp_ag *hfp_ag, char *number)
{
#ifdef APP_DEBUG_EN
    PRINTF("HFP HF have a in coming call :%s\n", number);
#endif
    if (g_HfpAg->hfp_in_calling_status == 1)
    {
#ifdef APP_DEBUG_EN
        PRINTF("Simulate a outcoming calling!!\n");
#endif
        bt_hfp_ag_send_callsetup_indicator(hfp_ag, 1);

        g_HfpAg->hfp_in_calling_status = 2;
    }
}

void ata_response(struct bt_hfp_ag *hfp_ag)
{
	app_hfp_ag_t *app_hfp_ag = app_ag_get_instance(hfp_ag);

	if(g_HfpAg == app_hfp_ag)
	{
		//bt_hfp_ag_send_callsetup_indicator(app_hfp_ag->hfp_agHandle, 0);
		bt_hfp_ag_send_call_indicator(app_hfp_ag->hfp_agHandle, 1);

		/* send ATA response to phone */
		if(g_HfpAg == app_hfp_ag)
			hfp_AnswerCall();

		app_hfp_ag->hfp_in_calling_status = 3;
	}
}

void chup_response(struct bt_hfp_ag *hfp_ag)
{
	app_hfp_ag_t *app_hfp_ag = app_ag_get_instance(hfp_ag);

	if (app_hfp_ag == NULL)
	{
	      return;
	}

    if(g_HfpAg == app_hfp_ag)
    {
#ifdef APP_DEBUG_EN
    PRINTF("HFP HF rh have ended the call\n");
#endif
    g_hfOutCall = 0;
	app_hfp_ag->hfp_in_calling_status = 1;
	bt_hfp_ag_call_status_pl(hfp_ag, hfp_ag_call_call_end);
	bt_hfp_ag_send_call_indicator(hfp_ag, 0);

		if (g_HfpAg->xTimers != 0)
		{
			xTimerStop(g_HfpAg->xTimers, 0);
			xTimerDelete(g_HfpAg->xTimers, 0);
			g_HfpAg->xTimers = 0;
		}
		if(g_intercomEnabled && !g_intercomPaused)
		{
			bt_hfp_ag_call_status_pl(g_HfpAg_ph->hfp_agHandle, hfp_ag_call_call_end);
			bt_hfp_ag_send_call_indicator(g_HfpAg_ph->hfp_agHandle, 0);

		}else
		{
			hfp_RejectCall(0);
		}
    }else if(g_HfpAg_ph == app_hfp_ag)
    {
#ifdef APP_DEBUG_EN
    PRINTF("HFP HF ph have ended the call\n");
#endif

		app_hfp_ag->hfp_in_calling_status = 1;
		bt_hfp_ag_call_status_pl(g_HfpAg_ph->hfp_agHandle, hfp_ag_call_call_end);
		bt_hfp_ag_send_call_indicator(g_HfpAg_ph->hfp_agHandle, 0);

		if(g_intercomEnabled)
		{
			bt_hfp_ag_call_status_pl(g_HfpAg->hfp_agHandle, hfp_ag_call_call_end);
			bt_hfp_ag_send_call_indicator(g_HfpAg->hfp_agHandle, 0);

		}
    }
}

static void brva(struct bt_hfp_ag *hfp_ag, uint32_t value)
{
    PRINTF("HFP voice recognition :%d\n", value);
}


static struct k_work setup_call_audio_work;
static struct k_work setup_call_audio_work_ph;

static void open_audio_connection_ag(struct k_work *work)
{

	vTaskDelay(10);
#ifdef APP_DEBUG_EN
	PRINTF("HFP g_sCallStatus :%d g_sHfpInCallingStatus %d \n", g_sCallStatus , g_sHfpInCallingStatus);
#endif
	if(g_sCallStatus || g_sHfpInCallingStatus || g_intercomEnabled)
    	app_hfp_ag_open_audio(RIDER_HEADSET);

}

static void open_audio_connection_ag_ph(struct k_work *work)
{
	app_hfp_ag_open_audio(PASSENGER_HEADSET);

}

static void codec_negotiate(struct bt_hfp_ag *hfp_ag, uint32_t value)
{
    PRINTF("HFP codec negotiation :%d\n", value);

    if (g_HfpAg->hfp_agHandle == hfp_ag)
    {
	    //Get call details, if no intercom enabled..
    	if(!g_intercomEnabled)
    		bt_hfp_hf_query_list_current_calls(conn_rider_phone);

    	k_work_init(&setup_call_audio_work, open_audio_connection_ag);
    	k_work_submit(&setup_call_audio_work);

    } else if (g_HfpAg_ph->hfp_agHandle == hfp_ag)
    {
    	k_work_init(&setup_call_audio_work, open_audio_connection_ag_ph);
    	k_work_submit(&setup_call_audio_work);
    }


}

void ag_nb_codec_select(uint8_t hs_type,uint32_t value)
{
#ifdef APP_DEBUG_EN
    PRINTF("HFP NB codec selected :%d\n", value);
#endif
    if(hs_type == RIDER_HEADSET)
    {
       	k_work_init(&setup_call_audio_work, open_audio_connection_ag);
       	k_work_submit(&setup_call_audio_work);
    }else
    {
       	k_work_init(&setup_call_audio_work, open_audio_connection_ag_ph);
      	k_work_submit(&setup_call_audio_work);
    }
}

static void chld(struct bt_hfp_ag *hfp_ag, uint8_t option, uint8_t index)
{
	app_hfp_ag_t *app_hfp_ag = app_ag_get_instance(hfp_ag);

		    if (app_hfp_ag == NULL)
		    {
		        return;
		    }
    PRINTF("AT_CHLD mutlipcall option  index :%d %d\n", option, index);
    if (option == 0)
    {
#ifdef APP_DEBUG_EN
        PRINTF(
            " Release all Held Calls and set UUDB tone "
            "(Reject new incoming waiting call)\n");
#endif
    }
    else if (option == 1)
    {
#ifdef APP_DEBUG_EN
        PRINTF("  Release Active Calls and accept held/waiting call\n");
#endif
    }
    else if (option == 2)
    {
#ifdef APP_DEBUG_EN
        PRINTF("  Hold Active Call and accept already held/new waiting call\n");
#endif
    }
    else if (option == 3)
    {
#ifdef APP_DEBUG_EN
        PRINTF(" bt multipcall 3. Conference all calls\n");
#endif
    }
    else if (option == 4)
    {
#ifdef APP_DEBUG_EN
        PRINTF(" bt multipcall 4. Connect other calls and disconnect self from TWC\n");
#endif
    }

    bt_hfp_hf_multiparty_call_option(conn_rider_phone, (hf_multiparty_call_option_t)option);

    if (app_hfp_ag->xTwcTimers  != 0)
        {
        	 xTimerStop(app_hfp_ag->xTwcTimers, 0);
        	        xTimerDelete(app_hfp_ag->xTwcTimers, 0);
        	        app_hfp_ag->xTwcTimers = 0;
        }
}

void get_config(struct bt_hfp_ag *hfp_ag, hfp_ag_get_config **config)
{
    *config = &hfp_ag_config;
}


static struct bt_hfp_ag_cb ag_cb = {
    .connected       = ag_connected,
    .disconnected    = ag_disconnected,
    .ata_response    = ata_response,
    .chup_response   = chup_response,
    .dial            = dial,
    .brva            = brva,
    .chld            = chld,
    .codec_negotiate = codec_negotiate,
    .get_config      = get_config,
	.hfu_brsf        = at_brsf,

#ifdef HFP_BRIDGING
    // Add the new SCO callbacks
    .sco_connected   = hfp_ag_sco_connected_callback,
    .sco_disconnected = hfp_ag_sco_disconnected_callback,
#endif
};

int app_hfp_ag_discover(struct bt_conn *conn, uint8_t channel)
{
	struct bt_hfp_ag *hfp_agHandle;
    int status                   = 0;
    if (!app_ag_check_idle_instance())
    {
    	PRINTF("No idle instance\r\n");
    		        return 0;
    }
    hfp_ag_config.server_channel = channel;
    status = bt_hfp_ag_connect(conn, &hfp_ag_config, &ag_cb, &hfp_agHandle);
        if (0 != status)
        {
            PRINTF("Fail to connect hfp_hf (err: %d)\r\n", status);
        }
    return status;
}

int app_hfp_ag_disconnect()
{
	if(g_HfpAg->hfp_agHandle)
		return bt_hfp_ag_disconnect(g_HfpAg->hfp_agHandle);
	else
		return 0;
}

static void vTimerRingCallback(TimerHandle_t xTimer)
{
    if(g_HfpAg->hfp_agHandle)
    	bt_hfp_ag_send_callring(g_HfpAg->hfp_agHandle);
}

static void vTimerTwcRingCallback(TimerHandle_t xTimer)
{
    if(g_HfpAg->hfp_agHandle)
    	bt_hfp_ag_send_callring(g_HfpAg->hfp_agHandle);
}

int app_hfp_ag_start_incoming_call()
{
    if (g_HfpAg->hfp_in_calling_status == 1 && g_HfpAg->hfp_agHandle)
    {

#ifdef APP_DEBUG_EN
    	PRINTF("Simulate a incoming call an incoming calling!!\r\n");
#endif
        bt_hfp_ag_send_callsetup_indicator(g_HfpAg->hfp_agHandle, 1);
        g_HfpAg->xTimers = xTimerCreate("RingTimer", (2000) + 10, pdTRUE, 0, vTimerRingCallback);
        xTimerStart(g_HfpAg->xTimers, 0);
        bt_hfp_ag_send_callring(g_HfpAg->hfp_agHandle);
        g_HfpAg->hfp_in_calling_status = 2;
        return 0;
    }
    return -1;
}

void app_hfp_ag_transfer_ring_ind(void)
{
    if (g_HfpAg->hfp_in_calling_status == 2 && g_HfpAg->hfp_agHandle)
    {
    	bt_hfp_ag_send_callsetup_indicator(g_HfpAg->hfp_agHandle, 1);
		bt_hfp_ag_send_callring(g_HfpAg->hfp_agHandle);
    }
}

int app_hfp_ag_start_twc_incoming_call(void)
{
    if (g_HfpAg->hfp_in_calling_status == 3 && g_HfpAg->hfp_agHandle)
    {
#ifdef APP_DEBUG_EN
        PRINTF("Simulate a mutiple call incoming call!!\n");
#endif
        bt_hfp_ag_send_callsetup_indicator(g_HfpAg->hfp_agHandle, 1);
        bt_hfp_ag_send_ccwa_indicator(g_HfpAg->hfp_agHandle, "1234567");
        g_HfpAg->xTwcTimers = xTimerCreate("TwcRingTimer", (2000) + 10, pdTRUE, 0, vTimerTwcRingCallback);
        xTimerStart(g_HfpAg->xTwcTimers, 0);
        bt_hfp_ag_send_callring(g_HfpAg->hfp_agHandle);
        g_HfpAg->hfp_in_calling_status = 4;
        return 0;
    }
    return -1;
}

void app_hfp_ag_open_audio(uint8_t hs_type)
{

    if(hs_type == RIDER_HEADSET && g_HfpAg->hfp_agHandle != NULL )
    {

#ifdef APP_DEBUG_EN
    	PRINTF("HFP RH AG audio codec=%d\n",(g_HfpAg->selectCodec - 1));
#endif
        bt_hfp_ag_open_audio(g_HfpAg->hfp_agHandle, g_HfpAg->selectCodec - 1);
        g_HfpAg->hfp_in_calling_status =2;

    } else if (hs_type == PASSENGER_HEADSET && g_HfpAg_ph->hfp_agHandle != NULL)
    {
#ifdef APP_DEBUG_EN
    	PRINTF("HFP PH AG audio codec=%d\n",(g_HfpAg_ph->selectCodec - 1));
#endif
        bt_hfp_ag_open_audio(g_HfpAg_ph->hfp_agHandle, g_HfpAg_ph->selectCodec - 1);

        if (g_intercomEnabled && g_rhsESCO)
        {
        	bt_hfp_ag_send_call_indicator(g_HfpAg->hfp_agHandle, 1);
        	bt_hfp_ag_send_callsetup_indicator(g_HfpAg->hfp_agHandle, 0);
        }

    }

}

void app_hfp_ag_close_audio(uint8_t hs_type)
{
	if(hs_type == RIDER_HEADSET && g_HfpAg->hfp_agHandle != NULL)
	{
		bt_hfp_ag_close_audio(g_HfpAg->hfp_agHandle);

	} else if (hs_type == PASSENGER_HEADSET && g_HfpAg_ph->hfp_agHandle != NULL)
	{
		bt_hfp_ag_close_audio(g_HfpAg_ph->hfp_agHandle);
	}
}

int app_hfp_ag_accept_incoming_call()
{
    if (g_HfpAg->hfp_in_calling_status == 2 && g_HfpAg->hfp_agHandle)
    {
#ifdef APP_DEBUG_EN
        PRINTF("HFP AG have accepted the incoming call\n");
#endif
        g_HfpAg->hfp_in_calling_status = 3;
        bt_hfp_ag_send_call_indicator(g_HfpAg->hfp_agHandle, 1);
        bt_hfp_ag_send_callsetup_indicator(g_HfpAg->hfp_agHandle, 0);
        if (g_HfpAg->xTimers != 0)
        {
            xTimerStop(g_HfpAg->xTimers, 0);
            xTimerDelete(g_HfpAg->xTimers, 0);
            g_HfpAg->xTimers = 0;
        }
        bt_hfp_ag_call_status_pl(g_HfpAg->hfp_agHandle, hfp_ag_call_call_incoming);
        return 0;
    }
    return -1;
}

int app_hfp_ag_stop_incoming_call()
{
	if(g_HfpAg->hfp_agHandle == NULL )
	{
		return -1;
	}
	if (g_HfpAg->hfp_in_calling_status >= 2)
	    {
			bt_hfp_ag_call_status_pl(g_HfpAg->hfp_agHandle, hfp_ag_call_call_end);
			if (g_HfpAg->xTimers != 0)
	        {
			   	xTimerStop(g_HfpAg->xTimers, 0);
			    xTimerDelete(g_HfpAg->xTimers, 0);
			    g_HfpAg->xTimers = 0;
	        }
			if(g_sCallStatus != 2)
				bt_hfp_ag_send_call_indicator(g_HfpAg->hfp_agHandle, 0);
#ifdef APP_DEBUG_EN
        PRINTF("HFP AG have ended the call\n");
#endif
        g_HfpAg->hfp_in_calling_status = 1;
        return 0;
    }
    return -1;
}

void app_hfp_ag_send_call_end_indicator()
{
	if(g_HfpAg->hfp_agHandle == NULL )
		return;

	bt_hfp_ag_send_call_indicator(g_HfpAg->hfp_agHandle, 0);
}

int app_hfp_ag_codec_select(uint8_t hs_type, uint8_t codec)
{

    if(hs_type == RIDER_HEADSET )
    {
#ifdef APP_DEBUG_EN
        PRINTF("HFP AG Rider select codec\n");
#endif
    	g_HfpAg->selectCodec = codec;
    	return bt_hfp_ag_codec_selector(g_HfpAg->hfp_agHandle, codec);

    } else if (hs_type == PASSENGER_HEADSET)
    {
    	g_HfpAg_ph->selectCodec = codec;
    	return bt_hfp_ag_codec_selector(g_HfpAg_ph->hfp_agHandle, codec);
    }
    else
    {
      	//Error return in case of unsupported hs_type
       	return -1;
    }
}

void app_hfp_ag_set_phnum_tag(char *name)
{
	if(g_HfpAg->hfp_agHandle)
		bt_hfp_ag_set_phnum_tag(g_HfpAg->hfp_agHandle, name);
}

void app_hfp_ag_volume_update(hf_ag_volume_type_t type, int volume)
{
    if(g_HfpAg->hfp_agHandle)
    	bt_hfp_ag_set_volume_control(g_HfpAg->hfp_agHandle, type, volume);
}

void app_hfp_ag_select_conn(uint8_t index)
{
    g_HfpAg = &g_HfpAgs[index];
}

void app_hfp_ag_transfer_call_ind(void)
{
   if(g_HfpAg->hfp_agHandle != NULL)
   {

		if (g_HfpAg->hfp_in_calling_status == 2 || g_sCallStatus == 2)
		{
#ifdef APP_DEBUG_EN
        PRINTF("HFP AG have accepted the incoming call\n");
#endif
			g_HfpAg->hfp_in_calling_status = 3;
			bt_hfp_ag_send_call_indicator(g_HfpAg->hfp_agHandle, 1);
		}

   }
}

void app_hfp_ag_send_setup_call_indication(uint32_t value)
{
	if(g_HfpAg->hfp_agHandle != NULL && !g_intercomEnabled)
	{
		bt_hfp_ag_send_callsetup_indicator(g_HfpAg->hfp_agHandle, value);
	}
}
void app_enable_intercom(uint8_t en)
{
	if(en)
	{
		if(g_phoneESCO)
			return;

		if( g_HfpAg->hfp_agHandle != NULL && g_HfpAg_ph->hfp_agHandle != NULL)
		{
#ifdef APP_DEBUG_EN
			PRINTF("Intercom started \n");
#endif
			g_intercomEnabled=1;
			g_intercomPaused=0;
			app_dual_a2dp_src_pause();
			app_a2dp_snk_pause();

			//Send call setup indication to Rider Headset and Passenger headset
			bt_hfp_ag_send_callsetup_indicator(g_HfpAg_ph->hfp_agHandle, 1);
			vTaskDelay(30);
			bt_hfp_ag_send_callsetup_indicator(g_HfpAg->hfp_agHandle, 1);

			//Negotiate codec
			if(g_agPhNbs || g_agRhNbs)
			{
				if(g_agRhNbs)
					ag_nb_codec_select(RIDER_HEADSET,g_agRhNbs);
				else
					app_hfp_ag_codec_select(RIDER_HEADSET,HFP_AG_CODEC_ID_CVSD); /* NBS codec */
			}
			else
			{
				app_hfp_ag_codec_select(RIDER_HEADSET,HFP_AG_CODEC_ID_MSBC); /* WBS codec */
			}
		}
		else
		{
#ifdef APP_DEBUG_EN
			PRINTF("Not enough connections to enable intercom \n");
#endif
		}

	}
	else
	{
#ifdef APP_DEBUG_EN
		PRINTF("Intercom stopped!\n");
#endif
		// Send call end indication
		if(g_HfpAg->hfp_agHandle)
		{
			bt_hfp_ag_send_call_indicator(g_HfpAg->hfp_agHandle, 0);
		}

		if(g_HfpAg_ph->hfp_agHandle)
		{
			bt_hfp_ag_send_call_indicator(g_HfpAg_ph->hfp_agHandle, 0);
		}

		if (g_rhsESCO && !g_phsESCO)
			app_hfp_ag_close_audio(RIDER_HEADSET);
		else if(g_phsESCO)
			app_hfp_ag_close_audio(PASSENGER_HEADSET);
		g_intercomPaused=0;
	}
}

int app_hfp_intercom_status()
{
	if ( g_intercomEnabled || g_intercomPaused)
		return 1;
	else
		return 0;
}

void app_intercom_to_rider_call()
{
	if(g_intercomEnabled && g_rhsESCO && g_phsESCO)
	{
		g_intercomPaused=1;

		if(g_HfpAg->hfp_agHandle)
		{
			bt_hfp_ag_send_call_indicator(g_HfpAg->hfp_agHandle, 0);
		}
		if(g_HfpAg_ph->hfp_agHandle)
		{
			bt_hfp_ag_send_call_indicator(g_HfpAg_ph->hfp_agHandle, 0);
		}
		app_hfp_ag_close_audio(PASSENGER_HEADSET);
	}
}

void app_rider_call_to_intercom()
{
	if (!g_phsESCO && !g_rhsESCO && !g_phsESCO)
	{

		if(g_intercomPaused && !g_sHfpInCallingStatus)
		{
			PRINTF("Enabled intercom back !");

			app_enable_intercom(1);

		}else if(g_intercomEnabled)
		{
			g_intercomEnabled = 0;
			app_a2dp_snk_resume();
			vTaskDelay(20);
		}

		if(!g_intercomEnabled)
			app_dual_a2dp_src_resume();
	}
}
void enable_esco_bridge(uint16_t stESCO, uint16_t ndESCO)
{
#ifdef APP_DEBUG_EN
    PRINTF("Sending Vendor command for eSCO bridge now\n");
#endif
    UCHAR new[6U];

    new[0] = 0x00;
    new[1] = 0x01;
    new[2] = (UCHAR)(stESCO & 0xFF);         // Lower byte
    new[3] = (UCHAR)((stESCO >> 8) & 0xFF);  // Upper byte
    new[4] = (UCHAR)(ndESCO & 0xFF);         // Lower byte
    new[5] = (UCHAR)((ndESCO >> 8) & 0xFF);  // Upper byte
    (BT_IGNORE_RETURN_VALUE) BT_hci_vendor_specific_command(0x006fU, new, sizeof(new));
}

void hfp_ag_sco_connected_callback(struct bt_hfp_ag *ag, struct bt_conn *sco_conn)
{
	struct bt_conn_info sco_info;
	struct bt_conn *ag_conn;
        uint16_t sco_handle;
	bt_conn_get_info(sco_conn, &sco_info);
        bt_hci_get_conn_handle(sco_conn,&sco_handle);
	uint8_t air_mode = sco_info.sco.air_mode;
	PRINTF("SCO connected,Handle: %u,air_mode : %u\r\n",sco_handle, air_mode);
	ag_conn= bt_hfp_ag_get_conn(ag);

 	if(conn_rider_hs == ag_conn)
        {
   			PRINTF(" Rider SCO connected \n");
   			g_rhsESCO = sco_handle;
   			g_agWbsEnable = (LMP_VOICE_AIR_CODING_TRANSPARENT == air_mode) ? 1 : 0; /* WBS or NBS */

   			if (g_agWbsEnable)
   				PRINTF("WBS\n");
   			else
   				PRINTF("NBS\n");

   			if (g_hfWbsEnable == g_agWbsEnable && g_phoneESCO) {

   				enable_esco_bridge(g_rhsESCO,g_phoneESCO);

   				if (!g_hfOutCall) {
   					app_hfp_ag_transfer_ring_ind();
   				} else {
   					//app_hfp_ag_transfer_call_ind();
   				}

   			} else if(!g_intercomEnabled){

   				app_hfp_ag_close_audio(RIDER_HEADSET);
   			}

   			if(g_intercomEnabled)
   			{
   				PRINTF("\nIntercom SCO2 \n");
   				if(g_HfpAg_ph->hfp_agHandle == NULL)
   				   	app_enable_intercom(0);

   				if(g_agPhNbs)
   				{
					PRINTF(" g_agRhNbs:%d\n", g_agRhNbs);
					ag_nb_codec_select(PASSENGER_HEADSET,g_agRhNbs);
   				}else
   				{
   					if (g_agWbsEnable)
   						app_hfp_ag_codec_select(PASSENGER_HEADSET,HFP_AG_CODEC_ID_MSBC); /* WBS codec */
   					else
   						app_hfp_ag_codec_select(PASSENGER_HEADSET,HFP_AG_CODEC_ID_CVSD); /* NBS codec */
   				}
   			}


    	} else if  (conn_passenger_hs == ag_conn)
   	{
   			 PRINTF(" Passenger SCO connected \n");
   			 g_phsESCO = sco_handle;

   	 		 if(g_intercomEnabled && ag)
   	 		 {
   	 			bt_hfp_ag_send_call_indicator(ag, 1);
   	 		bt_hfp_ag_send_callsetup_indicator(ag, 0);
   	 		 }
   	}

 	if(g_intercomEnabled && g_phsESCO && g_rhsESCO)
 	{
 		//Instead of hard codec eSCO handle , pass actual SCO handle.
 		enable_esco_bridge(g_rhsESCO,g_phsESCO);
 	}
 	bt_conn_unref(ag_conn);
}

void hfp_ag_sco_disconnected_callback(struct bt_hfp_ag *ag)
{
	struct bt_conn *ag_conn;
	ag_conn= bt_hfp_ag_get_conn(ag);

	if (conn_rider_hs == ag_conn)
    {
		/* Reset stored SCO2 handle */
		g_rhsESCO = 0;
    	if(g_intercomEnabled)
    		bt_hfp_ag_send_call_indicator(g_HfpAg->hfp_agHandle, 0);

    	if(g_phoneESCO && g_sHfpInCallingStatus && !g_sHfpAgRhDisconnecting)
    	{
    		PRINTF("Phone eSCO & call still active, ...\n");
    			if(g_hfWbsEnable)
    			{
    				app_hfp_ag_codec_select(RIDER_HEADSET,HFP_AG_CODEC_ID_MSBC); /* mSBC codec */
    			}
    			else
    			{
    				if(g_agRhNbs)
    				{
    					PRINTF("First SCO g_agRhNbs:%d\n", g_agRhNbs);
    					ag_nb_codec_select(RIDER_HEADSET,g_agRhNbs);

    				}else
    				{
    					app_hfp_ag_codec_select(RIDER_HEADSET,HFP_AG_CODEC_ID_CVSD); /* CVSD codec */
    				}
    			}

    	}


    } else if (conn_passenger_hs == ag_conn)
    {
    	PRINTF("Passenger SCO disconnected\r\n");
    	g_phsESCO = 0;
    	if(g_intercomPaused && g_rhsESCO && g_sHfpInCallingStatus)
    	{
    		if(!g_hfOutCall)
    			bt_hfp_ag_send_callsetup_indicator(g_HfpAg->hfp_agHandle, 1);

    	}else
    	{
    		PRINTF("Close rider audio \r\n");
    		app_hfp_ag_close_audio(RIDER_HEADSET);
    	}

    	if(g_intercomEnabled)
    	{
    		vTaskDelay(pdMS_TO_TICKS(40));
    		bt_hfp_ag_send_call_indicator(g_HfpAg_ph->hfp_agHandle, 0);
    	}
    }
	bt_conn_unref(ag_conn);
	if (!g_phoneESCO && !g_rhsESCO && !g_phsESCO)
	{

		if(g_intercomPaused && !g_sHfpInCallingStatus)
		{
			PRINTF("Enabled intercom back !");

			app_enable_intercom(1);

		} else if(g_intercomEnabled)
		{
			g_intercomEnabled = 0;
			app_a2dp_snk_resume();
			vTaskDelay(10);
		}

		if(!g_intercomEnabled)
			app_dual_a2dp_src_resume();
	}
}

void app_hfp_hf_disconnected()
{

	if(!(g_intercomPaused || g_intercomEnabled) )
	{
		app_hfp_ag_stop_incoming_call();
	}

}

