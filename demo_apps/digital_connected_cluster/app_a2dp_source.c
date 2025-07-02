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
#include <bluetooth/sdp.h>
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "app_connect.h"
#include "a2dp_pl_media_48KHz.h"
#include "app_shell.h"
#include "app_a2dp_source.h"
#include "app_avrcp.h"
#include "app_pbap_pce.h"
#include "wav_file.h"
#include "app_handsfree_ag.h"
#include "app_a2dp_sink.h"

/* User may need to change it for real production */
#define SDP_CLIENT_USER_BUF_LEN		512U
#define A2DP_SRC_PERIOD_MS    10
#define APPL_A2DP_MTU   (672U)

NET_BUF_POOL_FIXED_DEFINE(app_sdp_client_pool, CONFIG_BT_MAX_CONN,SDP_CLIENT_USER_BUF_LEN, CONFIG_NET_BUF_USER_DATA_SIZE, NULL);

a2dp_codec_config_t g_phoneA2dpConfig;
uint8_t app_sdp_a2sink_user(struct bt_conn *conn,struct bt_sdp_client_result *result);
void print_a2dp_config_paramater();
extern uint8_t app_get_snk_a2dp_status();

struct bt_sdp_discover_params discov_a2dp_sink =
{
    .uuid = BT_UUID_DECLARE_16(BT_SDP_AUDIO_SINK_SVCLASS),
    .func = app_sdp_a2sink_user,
    .pool = &app_sdp_client_pool,
};

static uint16_t g_a2dpSrcNumSamples;
static int32_t g_a2dpSrcSentMs;
static uint32_t g_a2dpSrcMissedCount;
static volatile uint8_t g_dualA2dpSrcPlayback;
static int g_ToneIndex;

uint8_t g_flagReadReady=0;
uint8_t g_playUsbMusic=0;
uint8_t g_playDefaultMusic=0;
uint16_t  g_medialen;
uint8_t *song;
wav_file_t wav_file;
static uint32_t g_a2dpSrcSf;
static TimerHandle_t g_a2dpSrcTimer;
uint8_t g_a2dpSrcNc;

struct bt_a2dp *default_a2dp_src;
struct bt_a2dp *rider_hs_a2dp_src;
struct bt_a2dp *passenger_hs_a2dp_src;
uint8_t g_dualA2dpSrcMode=0;
uint8_t g_dualA2dpPlayStatus=0;
uint8_t g_dualA2dpRhPlayStatus=0;
uint8_t g_dualA2dpPhPlayStatus=0;
uint8_t g_riderHsAudioStart=0;
uint8_t g_riderHsAudioStarting = 0;
uint8_t g_passengerHsAudioStart=0;
uint8_t g_a2dpSnkPlayStatus = 0;
uint8_t g_rhsIndex =0;
uint8_t g_phsIndex =0;
struct bt_a2dp_endpoint *default_a2dp_endpoint_src;
struct bt_a2dp_endpoint *rhs_a2dp_endpoint_src;
struct bt_a2dp_endpoint *phs_a2dp_endpoint_src;
struct bt_a2dp_endpoint *a2dp_endpoints[3];
//header media play
uint8_t * media;
uint8_t g_mallocA2dp = 0;
uint8_t g_a2dpXtimer=0;
uint8_t g_audioFileOpened=0;

//BT_A2DP_SBC_SOURCE_ENDPOINT(sbcEndpoint, A2DP_SBC_SAMP_FREQ_44100 );
//BT_A2DP_SBC_SOURCE_ENDPOINT(sbcEndpoint, A2DP_SBC_SAMP_FREQ_48000 );

BT_A2DP_SBC_SOURCE_ENDPOINT(sbcEndpointIdx1, A2DP_SBC_SAMP_FREQ_44100 );
BT_A2DP_SBC_SOURCE_ENDPOINT(sbcEndpointIdx2, A2DP_SBC_SAMP_FREQ_44100 );

void app_edgefast_a2dp_update_parameters(const a2dp_codec_config_t *config);

uint8_t g_a2dpConnectInitialized;
struct k_work_delayable configure_sbc_work;
#define CONFIGURE_SBC_DELAY K_MSEC(2000)
static struct bt_sdp_attribute a2dp_source_attrs[] = {
    BT_SDP_NEW_SERVICE,
    BT_SDP_LIST(
        BT_SDP_ATTR_SVCLASS_ID_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 3),
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE(BT_SDP_UUID16),
            BT_SDP_ARRAY_16(BT_SDP_AUDIO_SOURCE_SVCLASS)
        },
        )
    ),
    BT_SDP_LIST(
        BT_SDP_ATTR_PROTO_DESC_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 16),
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6),
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16),
                BT_SDP_ARRAY_16(BT_SDP_PROTO_L2CAP)
            },
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UINT16),
                BT_SDP_ARRAY_16(BT_UUID_AVDTP_VAL)
            },
            )
        },
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6),
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16),
                BT_SDP_ARRAY_16(BT_UUID_AVDTP_VAL)
            },
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UINT16),
                BT_SDP_ARRAY_16(0X0100u)
            },
            )
        },
        )
    ),
    BT_SDP_LIST(
        BT_SDP_ATTR_PROFILE_DESC_LIST,
        BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 8),
        BT_SDP_DATA_ELEM_LIST(
        {
            BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6),
            BT_SDP_DATA_ELEM_LIST(
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UUID16),
                BT_SDP_ARRAY_16(BT_SDP_ADVANCED_AUDIO_SVCLASS)
            },
            {
                BT_SDP_TYPE_SIZE(BT_SDP_UINT16),
                BT_SDP_ARRAY_16(0x0103U)
            },
            )
        },
        )
    ),
    BT_SDP_SERVICE_NAME("A2DPSource"),
    BT_SDP_SUPPORTED_FEATURES(0x0001U),
};

static struct bt_sdp_record a2dp_source_rec = BT_SDP_RECORD(a2dp_source_attrs);

void a2dp_source_register_service()
{
	bt_sdp_register_service(&a2dp_source_rec);
}

int data_send_source(uint8_t *data, uint32_t length)
{
	int ret=0;
	/*Send the data to the external remote headset*/

	if( g_phoneESCO || g_rhsESCO || g_phsESCO || rider_hs_a2dp_src == NULL )
	{
		/*A2DP mode not allowed during HFP call audio !*/
		//Check if can pause music in that case ..
		return 1;
	}

	//If no rider headset connected, need to suspend instead of pause.

	if(g_riderHsAudioStart && !g_dualA2dpSrcMode)
	{
		ret = bt_a2dp_src_media_write(rhs_a2dp_endpoint_src, data, length);

	}else if(!g_riderHsAudioStart && !g_riderHsAudioStarting)
	{
		app_a2dp_src_start(1);
	}

	return ret;

}

uint8_t app_set_a2dp_music_source(uint8_t music_source)
{

	if( g_phoneESCO || g_rhsESCO || g_phsESCO )
	{
		//PRINTF("\nA2DP mode not allowed during HFP call audio !\n");
		return 0;
	}

	if(music_source == 0)
	{

#ifdef APP_DEBUG_EN
	    PRINTF("\nDUAL A2DP stop and Enable A2DP bridge!\n");
#endif
	    if(!app_get_snk_a2dp_status())
	    	app_a2dp_src_stop(1);

	    app_a2dp_src_stop(0);

	    bt_a2dp_set_ep_codec_enable(rhs_a2dp_endpoint_src,0);
		g_playUsbMusic=0;
	    g_playDefaultMusic=0;
	    g_dualA2dpSrcMode = 0;
	    g_flagReadReady=0;
	    //avrcp_tg_notify(1,1);
	    app_a2dp_snk_resume();

	}else if(music_source == 1)
	{
#ifdef APP_DEBUG_EN
		PRINTF("\n DUAL A2DP start with USB Music set!\n");
#endif
		if(g_riderHsAudioStart && !g_dualA2dpSrcMode)
		{
			//PRINTF("\n Phone Music should be paused when start DUAL streaming with headsets !\n");
			app_a2dp_snk_pause();
		}
		bt_a2dp_set_ep_codec_enable(rhs_a2dp_endpoint_src,1);
		g_playDefaultMusic=0;
		g_playUsbMusic=1;
		app_set_dual_a2dp_src_mode(1);

		if(g_riderHsAudioStart || g_passengerHsAudioStart)
			g_dualA2dpSrcPlayback = 1U;

		avrcp_tg_set_absolute_volume(64);

	}else if(music_source == 2)
	{
#ifdef APP_DEBUG_EN
		PRINTF("\n DUAL A2DP start with default Music set!\n");
#endif
		if(g_riderHsAudioStart && !g_dualA2dpSrcMode)
		{
			//PRINTF("\n Phone Music should be paused when start DUAL streaming with headsets !\n");
			app_a2dp_snk_pause();
		}

		bt_a2dp_set_ep_codec_enable(rhs_a2dp_endpoint_src,1);

		g_playUsbMusic=0;
	    g_playDefaultMusic=1;
	    app_set_dual_a2dp_src_mode(1);

	    if(g_riderHsAudioStart || g_passengerHsAudioStart)
	    		g_dualA2dpSrcPlayback = 1U;

	    avrcp_tg_set_absolute_volume(64);
	    avrcp_tg_notify(1,1);

	}
	else{

		PRINTF("\n Invalid input ! \n");
		PRINTF("\n Options 0: A2DP bridge mode(Phone Music)"
				"1: DUAL a2dp USB Music 2:DUAL a2dp Default Music ! \n");
		return 0;
	}
	return 1;
}

void app_a2dp_snk_pause()
{

	if(conn_rider_phone == NULL)
    	 return;

    if(app_get_snk_a2dp_status() && g_riderHsAudioStart)
    {
#ifdef APP_DEBUG_EN
    	PRINTF("Pause Phone A2DP SNK ! \n");
#endif
    	g_a2dpSnkPlayStatus=1;
    	avrcp_pause_button(1);
		//vTaskDelay(50);
    }

}

void app_a2dp_snk_resume()
{

	if(conn_rider_phone == NULL)
    	 return;

	if(g_dualA2dpSrcMode)
		return;

    if(g_a2dpSnkPlayStatus)
    {
#ifdef APP_DEBUG_EN
    	PRINTF("Resume A2DP SNK ! \n");
#endif
    	g_a2dpSnkPlayStatus = 0;
    	vTaskDelay(50);
    	avrcp_play_button(1);

    }
}

uint8_t app_get_a2dp_mode()
{
	return g_dualA2dpSrcMode;
}
void app_a2dp_music_play(uint8_t rider_hs)
{
	if( g_phoneESCO || g_rhsESCO || g_phsESCO )
	{
		//	PRINTF("\n eSCO is still present.. !\n");
			return;
	}

    if(g_dualA2dpSrcMode && !rider_hs)
    {
    	app_a2dp_src_start(0);

    } else if (g_dualA2dpSrcMode && rider_hs)
    {
       //Manage avrcp with RH only
    	app_a2dp_src_start(1);

    }else if (!g_dualA2dpSrcMode && rider_hs)
    {
    	avrcp_play_button(1);
    }

}

void app_a2dp_music_stop(uint8_t rider_hs)
{

    if(g_dualA2dpSrcMode && !rider_hs)
    {
    	app_a2dp_src_stop(0);

    } else if (g_dualA2dpSrcMode && rider_hs)
    {
       //Manage avrcp with RH only
    	app_a2dp_src_stop(1);

    }else if (!g_dualA2dpSrcMode && rider_hs)
    {
    	avrcp_pause_button(1);
    }

}

void app_a2dp_src_start(uint8_t rider_hs)
{

	//int err;

	if( g_phoneESCO || g_rhsESCO || g_phsESCO || app_hfp_intercom_status())
	{
		return;
	}
#ifdef APP_DEBUG_EN
		PRINTF("\nSRC start RHS %d  PHS %d and rider_hs %d!\n",g_riderHsAudioStart,g_passengerHsAudioStart,rider_hs);
#endif

	if(rider_hs && !g_riderHsAudioStart)
	{

		if(rider_hs_a2dp_src == NULL)
		{
			avrcp_pause_button(1);
			return;
		}
		g_riderHsAudioStarting = 1;

#ifdef APP_DEBUG_EN
		PRINTF("\nA2DP streaming start to Rider Headset !\n");
#endif
		bt_a2dp_start(rhs_a2dp_endpoint_src);

	} else if(!rider_hs && !g_passengerHsAudioStart)
	{

		if(passenger_hs_a2dp_src == NULL)
			return;

#ifdef APP_DEBUG_EN
		PRINTF("\nA2DP streaming start to Passenger Headset !\n");
#endif
		vTaskDelay(20);
		bt_a2dp_start(phs_a2dp_endpoint_src);
	}

}

void app_a2dp_src_stop(uint8_t rider_hs)
{

	int err;
	PRINTF("\nRH audio stop rider_hs %d g_riderHsAudioStart %d g_passengerHsAudioStart %d \n ",rider_hs,g_riderHsAudioStart,g_passengerHsAudioStart);

	if(rider_hs && g_riderHsAudioStart)
	{
		PRINTF("\nRH audio stop \n ");
		err=bt_a2dp_stop(rhs_a2dp_endpoint_src);

		if(!err)
			g_riderHsAudioStart=0;
		else
			PRINTF("\nRH stop Error code=%d ",err);

	} else if(!rider_hs && g_passengerHsAudioStart)
	{
		PRINTF("\nPH audio stop \n ");
		err=bt_a2dp_stop(phs_a2dp_endpoint_src);

		if(!err)
			g_passengerHsAudioStart=0;
		else
			PRINTF("\nPH stop Error code=%d ",err);
	}

}

void app_a2dp_bridge_src_stop()
{
	if(!g_dualA2dpSrcMode)
	{
		bt_a2dp_stop(rhs_a2dp_endpoint_src);
		g_riderHsAudioStart=0;
	}
}

void app_set_dual_a2dp_src_mode(uint8_t dual_src_enable)
{
   //int err;

	if(dual_src_enable)
	{
		if(g_dualA2dpSrcMode == 0 && !g_dualA2dpPlayStatus)

		{
			g_dualA2dpSrcMode = 1;
			app_a2dp_src_start(1);
			app_a2dp_src_start(0);
		}

	}else if (g_dualA2dpSrcMode)
	{
		g_dualA2dpSrcMode=0;
		app_a2dp_src_stop(1);
		app_a2dp_src_stop(0);
	}
}

void app_dual_a2dp_src_pause()
{

	if( conn_rider_hs == NULL && conn_passenger_hs == NULL)
		return;

    if(g_dualA2dpSrcMode && !g_dualA2dpPlayStatus)
    {
#ifdef APP_DEBUG_EN
    	PRINTF("Pause DUAL A2DP ! \n");
#endif
    	g_dualA2dpPlayStatus=1;
    	g_dualA2dpRhPlayStatus = g_riderHsAudioStart;
    	g_dualA2dpPhPlayStatus = g_passengerHsAudioStart;
		app_a2dp_src_stop(1);
		app_a2dp_src_stop(0);
		vTaskDelay(10);
    }

}

void app_dual_a2dp_src_resume()
{
	if( g_phoneESCO || g_rhsESCO || g_phsESCO || app_hfp_intercom_status())
	{
		return;
	}
	if(g_dualA2dpPlayStatus && g_sCallStatus != 2)
    {
#ifdef APP_DEBUG_EN
    	PRINTF("Resume DUAL A2DP ! \n");
#endif
    	if(g_dualA2dpRhPlayStatus)
    	{
    		app_a2dp_src_start(1);
    		avrcp_tg_notify(1,1);
    	}

    	if(g_dualA2dpPhPlayStatus)
    		app_a2dp_src_start(0);

    	g_dualA2dpRhPlayStatus = 0;
    	g_dualA2dpPhPlayStatus = 0;
    	g_dualA2dpPlayStatus=0;

    }
}

static void a2dp_pl_produce_media(void)
{

    /* Music Audio is Stereo */
    if(g_flagReadReady==1)
    {

		/* Give data to callback */
    	if(rider_hs_a2dp_src != NULL && passenger_hs_a2dp_src != NULL)
    	{
    	   a2dp_endpoints[0] = rhs_a2dp_endpoint_src;
    	   a2dp_endpoints[1] = phs_a2dp_endpoint_src;
    	   a2dp_endpoints[2] = NULL;
    	   bt_a2dp_src_media_write_ext(a2dp_endpoints, song, g_medialen);
	   } else if(rider_hs_a2dp_src != NULL)
	   {
		   bt_a2dp_src_media_write(rhs_a2dp_endpoint_src, song, g_medialen);

	   } else if(passenger_hs_a2dp_src != NULL)
	   {
		   bt_a2dp_src_media_write(phs_a2dp_endpoint_src, song, g_medialen);
	   }


		vPortFree(song);

		g_flagReadReady=2;
    }


}

static void a2dp_pl_playback_timeout_handler(TimerHandle_t timer_id)
{
	int32_t now_ms, period_ms;
	TickType_t ticks;

    /* If stopped then return */
    if (0U == g_dualA2dpSrcPlayback)
    {
        return;
    }

    /* Get the current time */
    if (0U != __get_IPSR())
    {
        ticks = xTaskGetTickCountFromISR();
    }
    else
    {
        ticks = xTaskGetTickCount();
    }

    now_ms = ((uint32_t)((uint64_t)(ticks)*1000uL / (uint64_t)configTICK_RATE_HZ));
    period_ms = A2DP_SRC_PERIOD_MS;

    /* Adjust the period */
    if (g_a2dpSrcSentMs > 0)
    {
        period_ms = (now_ms > g_a2dpSrcSentMs)?
            (now_ms - g_a2dpSrcSentMs): A2DP_SRC_PERIOD_MS;
    }

    /* Get the number of samples */
    g_a2dpSrcNumSamples = (period_ms * g_a2dpSrcSf) / 1000;
    g_a2dpSrcMissedCount += (period_ms * g_a2dpSrcSf) % 1000;

    /* Raw adjust for the drift */
    while (g_a2dpSrcMissedCount >= 1000)
    {
        g_a2dpSrcNumSamples++;
        g_a2dpSrcMissedCount -= 1000;
    }

    /* Update the sent timestamp */
    g_a2dpSrcSentMs = now_ms;

    if(g_playDefaultMusic)
    {
    	if(!g_a2dpXtimer)
    	{
    		PRINTF("\nplay_default_music: %d!\n",g_playDefaultMusic);
    	}

    	g_flagReadReady=3;

    }else if (g_playUsbMusic){

    	a2dp_pl_produce_media();
    }

}


static void a2dp_pl_start_playback_timer(void)
{
	g_a2dpXtimer =1;
    g_a2dpSrcTimer = NULL;
   // PRINTF("\n A2DP timer started g_playDefaultMusic: %d!\n",g_playDefaultMusic);
    g_a2dpSrcTimer = xTimerCreate("a2dp play", (A2DP_SRC_PERIOD_MS / portTICK_PERIOD_MS),
                                  pdTRUE, NULL, a2dp_pl_playback_timeout_handler);
	g_a2dpSrcSentMs = 0;
    xTimerStart(g_a2dpSrcTimer, 0);
}

static void music_control_a2dp_stop_callback(struct bt_a2dp *a2dp,int err)
{

#ifdef APP_DEBUG_EN
    	PRINTF("A2DP control stop callback err %d! \n",err);
#endif

    if(rider_hs_a2dp_src == a2dp)
    {
    	g_riderHsAudioStart = 0;

        if(!g_dualA2dpSrcMode)
        	avrcp_tg_notify(1,0);
    }

    if(passenger_hs_a2dp_src == a2dp)
    	g_passengerHsAudioStart = 0;
}


static void music_control_a2dp_start_callback(int err)
{

#ifdef APP_DEBUG_EN
    	PRINTF("A2DP control callback err %d! \n",err);
#endif


	if(g_playUsbMusic && !g_audioFileOpened)
	{

		int ret=wav_file_open(&wav_file,"1:/dcc_test.wav");

		if(ret)
		{
			PRINTF("\nwav file open fail!\n");
			g_playUsbMusic=0;
			g_audioFileOpened=0;
		}
		 else
		{
			g_flagReadReady=2;
			g_playDefaultMusic=0;
			g_audioFileOpened=1;
#ifdef APP_DEBUG_EN
    		PRINTF("\nwav file open OK!\n");
    		PRINTF("size = %d\n",wav_file.size);
			PRINTF("bits per sample = %d\n",wav_file.bits);
			PRINTF("samples_rate = %d\n",wav_file.sample_rate);
			PRINTF("channel_count = %d\n",wav_file.channels);
			PRINTF("total_samples = %d\n",wav_file.samples);
			PRINTF("bytes per sample = %d\n",wav_file.bytes_per_sample);
#endif
		 }

	}

     if(g_playDefaultMusic)
     {
    	 g_dualA2dpSrcPlayback = 1U;

     }else if(g_audioFileOpened && g_playUsbMusic)
     {
    	 g_dualA2dpSrcPlayback = 1U;
	 }

	 if(!g_a2dpXtimer)
	 {
	 	 //Later need to add stop timer
	 	 a2dp_pl_start_playback_timer();
	 }

}

static void music_control_idx1_a2dp_start_callback(int err)
{
	if(g_rhsIndex == 1)
	{
		g_riderHsAudioStart = 1;
		g_riderHsAudioStarting = 0;

		if(!g_dualA2dpSrcMode)
			avrcp_tg_notify(1,1);
	}
	if(g_phsIndex == 1)
	{
	  	 g_passengerHsAudioStart = 1;
	}
	music_control_a2dp_start_callback(err);
}

static void music_control_idx2_a2dp_start_callback(int err)
{
	if(g_rhsIndex == 2)
	{
		g_riderHsAudioStart = 1;
		g_riderHsAudioStarting = 0;

		if(!g_dualA2dpSrcMode)
			avrcp_tg_notify(1,1);
	}
	if(g_phsIndex == 2)
	{
	   	g_passengerHsAudioStart = 1;
	}
    music_control_a2dp_start_callback(err);
}

void read_audio(void *pvParameters)
{

	while(1)
	{
		if(g_flagReadReady==2)
		{
		 g_medialen = (g_a2dpSrcNumSamples << g_a2dpSrcNc);
		 if(g_medialen!=0)
			{
			 song = (uint8_t *)pvPortMalloc(g_medialen);
			if (NULL == song)
			{
				PRINTF("\nmem alloc failed!\n");
				g_playUsbMusic=0;
				//return;
			}
			//PRINTF("g_medialen = %d\r\n",g_medialen);
			int ret=wav_file_read(&wav_file,song,g_medialen);
			if(ret==WAV_FILE_END)
			{
				//PRINTF("\nwav_file_read END!\n");
			    if(wav_file_rewind(&wav_file))
				{
					 PRINTF("\nwav_file_rewind fail!\n");
				}
			}
			else if(ret)
			{
				PRINTF("\nwav_file_read fail!\n");
				g_playUsbMusic=0;
				g_flagReadReady=0;
			}
			else
			{
				g_flagReadReady=1;
			}
			}
		}
	//Header Media audio read

		if(g_flagReadReady==3 && g_mallocA2dp == 0){

			//PRINTF("\n Read Audio!\n");

			/* Music Audio is Stereo */
			    g_medialen = (g_a2dpSrcNumSamples << g_a2dpSrcNc);

			    /* For mono or dual configuration, skip alternative samples */
			    if (1 == g_a2dpSrcNc)
			    {
			        uint16_t index;

			        /* Allocate Memory */
			        g_mallocA2dp = 1;
			        media = (uint8_t *)pvPortMalloc(g_medialen);

			        if (NULL == media)
			        {
			            return;
			        }

			        for (index = 0; index < g_a2dpSrcNumSamples; index++)
			        {
			            media[(2 * index)] = *((uint8_t *)beethoven + g_ToneIndex);
			            media[(2 * index) + 1] = *((uint8_t *)beethoven + g_ToneIndex + 1);
			            /* Update the tone index */
			            g_ToneIndex += 4u;
			            if (g_ToneIndex >= sizeof(beethoven))
			            {
			                g_ToneIndex = 0U;
			            }
			        }
			    }
			    else
			    {
			        if ((g_ToneIndex + (g_a2dpSrcNumSamples << 2)) > sizeof(beethoven))
			        {
			            g_mallocA2dp = 1;
			            media = (uint8_t *)pvPortMalloc(g_medialen);
			            if (NULL == media)
			            {
			                PRINTF("Memory Allocation failed in Produce Media\n");
			                return;
			            }
			            memcpy(media, ((uint8_t *)beethoven + g_ToneIndex), sizeof(beethoven) - g_ToneIndex);
			            memcpy(&media[sizeof(beethoven) - g_ToneIndex],
			                   ((uint8_t *)beethoven),
			                   ((g_a2dpSrcNumSamples << 2) - (sizeof(beethoven) - g_ToneIndex)));
			            /* Update the tone index */
			            g_ToneIndex = ((g_a2dpSrcNumSamples << 2) - (sizeof(beethoven) - g_ToneIndex));
			        }
			        else
			        {
			            media = ((uint8_t *)beethoven + g_ToneIndex);
			            /* Update the tone index */
			            g_ToneIndex += (g_a2dpSrcNumSamples << 2);
			            if (g_ToneIndex >= sizeof(beethoven))
			            {
			                g_ToneIndex = 0U;
			            }
			        }
			    }

	    /* Give data to callback */
			   if(rider_hs_a2dp_src != NULL && passenger_hs_a2dp_src != NULL)
			   {
				   a2dp_endpoints[0] = rhs_a2dp_endpoint_src;
				   a2dp_endpoints[1] = phs_a2dp_endpoint_src;
				   a2dp_endpoints[2] = NULL;
				   bt_a2dp_src_media_write_ext(a2dp_endpoints, media, g_medialen);
			   } else if(rider_hs_a2dp_src != NULL)
			   {
			       bt_a2dp_src_media_write(rhs_a2dp_endpoint_src, media, g_medialen);
			   } else if(passenger_hs_a2dp_src != NULL)
			   {
			       bt_a2dp_src_media_write(phs_a2dp_endpoint_src, media, g_medialen);
			   }
	   		   if (g_mallocA2dp == 1)
	   		   {
	   			   vPortFree(media);
	   			   g_mallocA2dp=0;
	   		   }

	    g_flagReadReady=4;
		}

	}
	vTaskDelete( NULL );
}

void app_endpoint_configured_idx1(struct bt_a2dp_endpoint_configure_result *result)
{
    if (result->err == 0)
    {
    	uint8_t index =1;
        k_work_cancel_delayable(&configure_sbc_work);

        g_a2dpSrcSf = bt_a2dp_sbc_get_sampling_frequency((struct bt_a2dp_codec_sbc_params *)&result->config.media_config->codec_ie[0]);
        g_a2dpSrcNc = bt_a2dp_sbc_get_channel_num((struct bt_a2dp_codec_sbc_params *)&result->config.media_config->codec_ie[0]);


        if(rider_hs_a2dp_src == result->a2dp)
        {
           	PRINTF("\nRHS!");
           	g_rhsIndex = index;
           	if(index == 1)
           	{
           		rhs_a2dp_endpoint_src = &sbcEndpointIdx1;
           	}
           	else if(index == 2)
           	{
           		rhs_a2dp_endpoint_src = &sbcEndpointIdx2;
           	}

           	if (g_dualA2dpSrcMode)
           	{
           		bt_a2dp_set_ep_codec_enable(rhs_a2dp_endpoint_src,1);
           		app_a2dp_src_start(1);
           	}else
           	{
           		bt_a2dp_set_ep_codec_enable(rhs_a2dp_endpoint_src,0);
           	}

        }else if(passenger_hs_a2dp_src == result->a2dp)
        {
           	PRINTF("\nPHS ");
           	g_phsIndex = index;
           	if(index == 1)
           	{
           		phs_a2dp_endpoint_src = &sbcEndpointIdx1;
           	}
           	else if(index == 2)
           	{
           		phs_a2dp_endpoint_src = &sbcEndpointIdx2;
           	}

        	bt_a2dp_set_ep_codec_enable(phs_a2dp_endpoint_src,1);
           	if (g_dualA2dpSrcMode)
           	{
           		app_a2dp_src_start(0);
           	}
        }

        PRINTF("A2DP endpoint configured index:%d!\n",index);

        if (g_dualA2dpSrcMode)
        {
        	PRINTF("\nDUAL A2DP mode- start music\n");
        	//if call active and then  mark dual A2DP as paused. To be checked. with RHS audio start=1
        }
    }
}

void app_endpoint_configured_idx2(struct bt_a2dp_endpoint_configure_result *result)
{
    if (result->err == 0)
    {
    	uint8_t index =2;

    	k_work_cancel_delayable(&configure_sbc_work);

        g_a2dpSrcSf = bt_a2dp_sbc_get_sampling_frequency((struct bt_a2dp_codec_sbc_params *)&result->config.media_config->codec_ie[0]);
        g_a2dpSrcNc = bt_a2dp_sbc_get_channel_num((struct bt_a2dp_codec_sbc_params *)&result->config.media_config->codec_ie[0]);


        if(rider_hs_a2dp_src == result->a2dp)
        {
           	PRINTF("\nRHS!");
           	g_rhsIndex = index;
           	if(index == 1)
           	{
           		rhs_a2dp_endpoint_src = &sbcEndpointIdx1;
           	}
           	else if(index == 2)
           	{
           		rhs_a2dp_endpoint_src = &sbcEndpointIdx2;
           	}
           	if (g_dualA2dpSrcMode)
           	{
          		bt_a2dp_set_ep_codec_enable(rhs_a2dp_endpoint_src,1);
           		app_a2dp_src_start(1);
           	}else
           	{
           		bt_a2dp_set_ep_codec_enable(rhs_a2dp_endpoint_src,0);
           	}
        }else if(passenger_hs_a2dp_src == result->a2dp)
        {
           	PRINTF("\nPHS ");
           	g_phsIndex = index;
           	if(index == 1)
           	{
           		phs_a2dp_endpoint_src = &sbcEndpointIdx1;
           	}
          	else if(index == 2)
          	{
          		phs_a2dp_endpoint_src = &sbcEndpointIdx2;
          	}
           	bt_a2dp_set_ep_codec_enable(phs_a2dp_endpoint_src,1);
           	if (g_dualA2dpSrcMode)
           	{
           		app_a2dp_src_start(0);
           	}
        }
        PRINTF("A2DP endpoint configured index:%d!\n",index);

        if (g_dualA2dpSrcMode)
        {
        	PRINTF("\nDUAL A2DP mode- start music\n");
        	//if call active and then  mark dual A2DP as paused. To be checked. with RHS audio start=1
        }
    }
}

void app_configured(int err)
{
    if (err)
    {
        PRINTF("A2DP configuration fail with(err %d)\n", err);
    }
    else
    {
    	PRINTF("A2DP configuration Done\n");
    }
}

static void configure_sbc_work_handle(struct k_work *work)
{
	PRINTF("a2dp connection k_work ! to check hs type \n");
    if (default_a2dp_src)
    {
        bt_a2dp_configure(default_a2dp_src, app_configured);
    }
}

void app_connected_src(struct bt_a2dp *a2dp, int err)
{

	if(g_profileConnectedRiderHs == 1)
	{
		//if(conn_rider_phone != NULL)
		{
			 cmd_init_ct();
			 g_avrcpControlConnectHfpHf = true;
			 avrcp_control_connect(conn_rider_hs);
		}
	}
    if (!err)
    {

    	if(g_profileConnectedRiderHs == 1)
    	{

    		if (g_a2dpConnectInitialized)
    		{
    			PRINTF("rider hs a2dp connection ! \n");

    			g_a2dpConnectInitialized = 0U;

#if defined(AUTO_A2DP_CONFIGURE) && (AUTO_A2DP_CONFIGURE == 1)
    			if(conn_rider_phone != NULL)
    			{
        			app_edgefast_a2dp_update_parameters(&g_phoneA2dpConfig);
    			}
#endif
    			//print_a2dp_config_paramater();
    			rider_hs_a2dp_src =  a2dp;
    			bt_a2dp_configure(a2dp, app_configured);

    		}
    		else
    		{

    			/* usually the peer headset will configure a2dp_source demo automatically if connecting from peer headset.
    			 * there are headsets that doesn't do it.
    			 */
    			k_work_init_delayable(&configure_sbc_work, configure_sbc_work_handle);
    			k_work_schedule(&configure_sbc_work, CONFIGURE_SBC_DELAY);
    		}
    	}
    	else if(g_profileConnectedPassengerHs == 1)
    	{

    		if (g_a2dpConnectInitialized)
    		{
    			PRINTF("\npassenger hs a2dp connection ! \n");
    			g_a2dpConnectInitialized = 0U;

#if defined(AUTO_A2DP_CONFIGURE) && (AUTO_A2DP_CONFIGURE == 1)
    			if(conn_rider_phone != NULL)
    			{
        			app_edgefast_a2dp_update_parameters(&g_phoneA2dpConfig);
    			}
#endif
    			//print_a2dp_config_paramater();
    			passenger_hs_a2dp_src =  a2dp;
    			bt_a2dp_configure(a2dp, app_configured);

    		}
    		else
    		{
    			/* usually the peer headset will configure a2dp_source demo automatically if connecting from peer headset.
    			 * there are headsets that doesn't do it.
    			 */
    			k_work_init_delayable(&configure_sbc_work, configure_sbc_work_handle);
    			k_work_schedule(&configure_sbc_work, CONFIGURE_SBC_DELAY);
    		}


    	}
    }
    else
    {

        if (default_a2dp_src != NULL)
        {
            default_a2dp_src = NULL;
        }
        PRINTF("a2dp connection fail (err %d)\n", err);
    }
    g_profileConnectedRiderHs = 0;
}

void app_disconnected_src(struct bt_a2dp *a2dp)
{
   if (rider_hs_a2dp_src == a2dp)
   {
    	PRINTF("Rider hs a2dp disconnected\r\n");
    	rider_hs_a2dp_src = NULL;
    	g_riderHsAudioStart=0;
    	g_rhsIndex=0;
    	/*Pause phone music */
    	avrcp_pause_button(1);
   }

   if (passenger_hs_a2dp_src == a2dp)
   {
	    PRINTF("Passenger hs a2dp disconnected\r\n");
    	passenger_hs_a2dp_src = NULL;
    	g_phsIndex=0;
    	g_passengerHsAudioStart = 0;
   }

   if(passenger_hs_a2dp_src == NULL && rider_hs_a2dp_src == NULL)
   {
	    g_dualA2dpSrcPlayback = 0U;

   }


}

uint8_t app_sdp_a2sink_user(struct bt_conn *conn,struct bt_sdp_client_result *result)
{
    uint16_t param;
    uint8_t rider_hs=0;
    int res;

    if ((result) && (result->resp_buf))
    {
    	PRINTF("A2DP snk sdp success callback\r\n");
        res = bt_sdp_get_proto_param(result->resp_buf, BT_SDP_PROTO_L2CAP, &param);
        if (res < 0)
        {
        	PRINTF("PSM is not found\r\n");
            return BT_SDP_DISCOVER_UUID_CONTINUE;
        }
        if (param == BT_UUID_AVDTP_VAL)
        {
            PRINTF ("A2DP Service found. Connecting ...\n");
            g_a2dpConnectInitialized = 1U;

            // check if rider hs or passenger hs
            if(conn == conn_rider_hs){
            	PRINTF ("A2DP Service found. Connecting RHS...\n");
            	rider_hs_a2dp_src = bt_a2dp_connect(conn);
            	rider_hs=1;

            }else if(conn == conn_passenger_hs)
            {
            	PRINTF ("A2DP Service found. Connecting PHS...\n");
            	passenger_hs_a2dp_src = bt_a2dp_connect(conn);
            }


            if (rider_hs && NULL == rider_hs_a2dp_src)
            {
            	PRINTF("fail to connect a2dp source\r\n");

            }else if (!rider_hs && NULL == passenger_hs_a2dp_src){

            	PRINTF("fail to connect a2dp source\r\n");
            }

            return BT_SDP_DISCOVER_UUID_STOP;
        }
        return BT_SDP_DISCOVER_UUID_CONTINUE;
    }
    else
    {
    	PRINTF("sdp fail callback\r\n");
        return BT_SDP_DISCOVER_UUID_CONTINUE;
    }
}

void sdp_discover_for_a2dp_sink(uint8_t isRiderHS)
{
    int res;
    vTaskDelay(200);
    if(isRiderHS)
    {
		res = bt_sdp_discover(conn_rider_hs, &discov_a2dp_sink);  //default_conn

		if (res)
		{
			PRINTF("A2DP discovery failed for Rider Headset: result\r\n");
		}
		else
		{
			PRINTF("A2DP discovery started for Rider Headset\r\n");
		}

    } else
    {
		res = bt_sdp_discover(conn_passenger_hs, &discov_a2dp_sink);  //default_conn

		if (res)
		{
			PRINTF("A2DP discovery failed for Passenger Headset: result\r\n");
		}
		else
		{
			PRINTF("A2DP discovery started for Passenger Headset \r\n");
		}
    }
}

static void a2dp_src_edgefast_a2dp_init(void)
{
    struct bt_a2dp_connect_cb connectCb;
    connectCb.connected = app_connected_src;
    connectCb.disconnected = app_disconnected_src;
    sbcEndpointIdx1.control_cbs.start_play = music_control_idx1_a2dp_start_callback;
    sbcEndpointIdx1.control_cbs.configured = app_endpoint_configured_idx1;

    sbcEndpointIdx2.control_cbs.start_play = music_control_idx2_a2dp_start_callback;
    sbcEndpointIdx2.control_cbs.configured = app_endpoint_configured_idx2;

    bt_a2dp_register_endpoint(&sbcEndpointIdx1, BT_A2DP_AUDIO, BT_A2DP_SOURCE);
    bt_a2dp_register_endpoint(&sbcEndpointIdx2, BT_A2DP_AUDIO, BT_A2DP_SOURCE);

    bt_a2dp_register_connect_callback(&connectCb);
}

void a2dp_source_ready(void)
{
	a2dp_src_edgefast_a2dp_init();
}

/*
void app_edgefast_a2dp_update_parameters(const a2dp_codec_config_t *config)
{
    bt_a2dp_endpoint_preset_buffersbcEndpoint[1] =
        (bt_a2dp_endpoint_preset_buffersbcEndpoint[1] & ~0xF0) | config->sample_rate;

    bt_a2dp_endpoint_preset_buffersbcEndpoint[1] =
        (bt_a2dp_endpoint_preset_buffersbcEndpoint[1] & ~0x0F) | config->channel_mode;

    bt_a2dp_endpoint_preset_buffersbcEndpoint[2] =
        (bt_a2dp_endpoint_preset_buffersbcEndpoint[2] & ~0xF0) | config->block_length;

    bt_a2dp_endpoint_preset_buffersbcEndpoint[2] =
        (bt_a2dp_endpoint_preset_buffersbcEndpoint[2] & ~0x0C) | config->subbands;

    bt_a2dp_endpoint_preset_buffersbcEndpoint[2] =
        (bt_a2dp_endpoint_preset_buffersbcEndpoint[2] & ~0x03) | config->allocation_method;

    bt_a2dp_endpoint_preset_buffersbcEndpoint[3] = config->min_bitpool; // Minimum bitpool
    bt_a2dp_endpoint_preset_buffersbcEndpoint[4] = config->max_bitpool; // Maximum bitpool

}

void print_a2dp_config_paramater()
{
#ifdef APP_DEBUG_EN
	 PRINTF("\n\n preset_buffer[0: %x][1: %x][2: %x][3: %x] ---------------------\n",
			 bt_a2dp_endpoint_preset_buffersbcEndpoint[0],
          bt_a2dp_endpoint_preset_buffersbcEndpoint[1],
			 bt_a2dp_endpoint_preset_buffersbcEndpoint[2],
			 bt_a2dp_endpoint_preset_buffersbcEndpoint[3]);

   // Print Sampling Frequency (from bt_a2dp_endpoint_preset_buffersbcEndpoint[1])
   PRINTF("Sampling Frequency:%s %x\n",
    ( ((bt_a2dp_endpoint_preset_buffersbcEndpoint[1] >> 4) & 0x0F) == 0x08) ? "16 kHz" :
    (  ((bt_a2dp_endpoint_preset_buffersbcEndpoint[1] >> 4) & 0x0F) == 0x04) ? "32 kHz" :
    (  ((bt_a2dp_endpoint_preset_buffersbcEndpoint[1] >> 4) & 0x0F) == 0x02) ? "44.1 kHz" :
    (  ((bt_a2dp_endpoint_preset_buffersbcEndpoint[1] >> 4) & 0x0F) == 0x01) ? "48 kHz" : "Unknown",
   		 ( (bt_a2dp_endpoint_preset_buffersbcEndpoint[1] >> 4) & 0x0F)
     );


   // Print Channel Mode (from bt_a2dp_endpoint_preset_buffersbcEndpoint[1])
    PRINTF("Channel Mode: %s\n",
        (bt_a2dp_endpoint_preset_buffersbcEndpoint[1] & 0x0F) == A2DP_SBC_CH_MODE_MONO ? "Mono" :
        (bt_a2dp_endpoint_preset_buffersbcEndpoint[1] & 0x0F) == A2DP_SBC_CH_MODE_DUAL ? "Dual" :
        (bt_a2dp_endpoint_preset_buffersbcEndpoint[1] & 0x0F) == A2DP_SBC_CH_MODE_STREO ? "Stereo" : "Joint Stereo");


    // Print Block Length (from bt_a2dp_endpoint_preset_buffersbcEndpoint[2])
    PRINTF("Block Length: 0x%x\n",
        ((bt_a2dp_endpoint_preset_buffersbcEndpoint[2] >> 4) & 0x0F) == A2DP_SBC_BLK_LEN_4 ? 4 :
        ((bt_a2dp_endpoint_preset_buffersbcEndpoint[2] >> 4) & 0x0F) == A2DP_SBC_BLK_LEN_8 ? 8 :
        ((bt_a2dp_endpoint_preset_buffersbcEndpoint[2] >> 4) & 0x0F) == A2DP_SBC_BLK_LEN_12 ? 12 : 16);

    // Print Subbands (from bt_a2dp_endpoint_preset_buffersbcEndpoint[2])
    PRINTF("Subbands: 0x%x\n",
        (bt_a2dp_endpoint_preset_buffersbcEndpoint[2] >> 2) & 0x03 == A2DP_SBC_SUBBAND_4 ? 4 : 8);

    // Print Allocation Method (from bt_a2dp_endpoint_preset_buffersbcEndpoint[2])
    PRINTF("Allocation Method: %s\n",
        (bt_a2dp_endpoint_preset_buffersbcEndpoint[2] & 0x03) == A2DP_SBC_ALLOC_MTHD_LOUDNESS ? "Loudness" : "SNR");

    // Print Bitpool (from bt_a2dp_endpoint_preset_buffersbcEndpoint[3] and [4])
    PRINTF("Bitpool: Min=0x%x, Max=0x%x\n", bt_a2dp_endpoint_preset_buffersbcEndpoint[3], bt_a2dp_endpoint_preset_buffersbcEndpoint[4]);
#endif
}
*/
