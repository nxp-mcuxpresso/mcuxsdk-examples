/*
 * Copyright 2020 - 2022, 2024-2025 NXP
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
#include <app_avrcp.h>
#include "clock_config.h"
#include "board.h"
#include "fsl_adapter_audio.h"
#include "fsl_codec_common.h"
#include "fsl_codec_adapter.h"
#include "fsl_debug_console.h"
#include "app_connect.h"
#include "BT_common.h"
#include "BT_hci_api.h"
#include "bt_pal_conn_internal.h"
#include "bluetooth/avrcp.h"
#include "bluetooth/conn.h"
#include "app_a2dp_source.h"

#include "EM_timer.h"


#define A2DP_CLASS_OF_DEVICE (0x200404U)
#define APP_A2DP_STREAMER_SYNC_TASK_PRIORITY (5U)
/*If codec is cs42448, it doesn't support DAC CHANNAL, the macro A2DP_CODEC_DAC_VOLUME is meaningless.*/
#define A2DP_CODEC_DAC_VOLUME (100U) /* Range: 0 ~ 100 */
#define A2DP_CODEC_HP_VOLUME  (100U)  /* Range: 0 ~ 100 */
#define SDP_CLIENT_USER_BUF_LEN 512U
NET_BUF_POOL_FIXED_DEFINE(app_sdp_client_pool, CONFIG_BT_MAX_CONN,SDP_CLIENT_USER_BUF_LEN, CONFIG_NET_BUF_USER_DATA_SIZE, NULL);


struct bt_a2dp *default_a2dp_snk;
struct bt_a2dp_endpoint *default_a2dp_endpoint_snk;
static uint8_t g_audioStart;
extern a2dp_codec_config_t g_phoneA2dpConfig; // Declare the external variable


BT_A2DP_SBC_SINK_ENDPOINT(sbcEndpoint);
static uint8_t app_sdp_a2source_user(struct bt_conn *conn,
			   struct bt_sdp_client_result *result);

static struct bt_sdp_discover_params discov_a2dp_source =
{
		.uuid = BT_UUID_DECLARE_16(BT_SDP_AUDIO_SOURCE_SVCLASS),
		.func = app_sdp_a2source_user,
		.pool = &app_sdp_client_pool,
};


static uint8_t app_sdp_a2source_user(struct bt_conn *conn,
			   struct bt_sdp_client_result *result)
{
    uint16_t param;
    int res;
    PRINTF("A2DP src sdp success callback\r\n");
    if ((result) && (result->resp_buf))
    {
        res = bt_sdp_get_proto_param(result->resp_buf, BT_SDP_PROTO_L2CAP, &param);
        if (res < 0)
        {
            return BT_SDP_DISCOVER_UUID_CONTINUE;
        }
        if (param == BT_UUID_AVDTP_VAL)
        {
            PRINTF ("SRC_A2DP Service found. Connecting ...\n");
            default_a2dp_snk = bt_a2dp_connect(conn);
            if (NULL == default_a2dp_snk)
            {
                PRINTF ("fail to connect a2dp\r\n");
            }
            return BT_SDP_DISCOVER_UUID_STOP;
        }
        return BT_SDP_DISCOVER_UUID_CONTINUE;
    }
    else
    {
        return BT_SDP_DISCOVER_UUID_CONTINUE;
    }
}

static struct bt_sdp_attribute a2dp_sink_attrs[] = {
		BT_SDP_NEW_SERVICE,
		BT_SDP_LIST(
				BT_SDP_ATTR_SVCLASS_ID_LIST,
				BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 3), //35 03
				BT_SDP_DATA_ELEM_LIST(
						{
			BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
					BT_SDP_ARRAY_16(BT_SDP_AUDIO_SINK_SVCLASS) //11 0B
						},
				)
		),
		BT_SDP_LIST(
				BT_SDP_ATTR_PROTO_DESC_LIST,
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
									BT_SDP_ARRAY_16(BT_UUID_AVDTP_VAL) // 00 19
							},
					)
						},
						{
								BT_SDP_TYPE_SIZE_VAR(BT_SDP_SEQ8, 6),// 35 06
								BT_SDP_DATA_ELEM_LIST(
										{
									BT_SDP_TYPE_SIZE(BT_SDP_UUID16), //19
											BT_SDP_ARRAY_16(BT_UUID_AVDTP_VAL) // 00 19
										},
										{
												BT_SDP_TYPE_SIZE(BT_SDP_UINT16), //09
												BT_SDP_ARRAY_16(0X0100u) //AVDTP version: 01 00
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
						BT_SDP_ARRAY_16(BT_SDP_ADVANCED_AUDIO_SVCLASS) //11 0d
							},
							{
									BT_SDP_TYPE_SIZE(BT_SDP_UINT16), //09
									BT_SDP_ARRAY_16(0x0103U) //01 03
							},
					)
						},
				)
		),
		BT_SDP_SERVICE_NAME("A2DPSink"),
		BT_SDP_SUPPORTED_FEATURES(0x0001U),
};

static struct bt_sdp_record a2dp_sink_rec = BT_SDP_RECORD(a2dp_sink_attrs);


void a2dp_sink_register_service()
{
	bt_sdp_register_service(&a2dp_sink_rec);

}

void sbc_configured(struct bt_a2dp_endpoint_configure_result *configResult)
{
	if (configResult->err == 0)
	{
		default_a2dp_endpoint_snk = &sbcEndpoint;
		bt_a2dp_set_ep_codec_enable(default_a2dp_endpoint_snk,0);
		uint32_t sampleRate_Hz  = bt_a2dp_sbc_get_sampling_frequency((struct bt_a2dp_codec_sbc_params *)&configResult->config.media_config->codec_ie[0]);

		// Extract and store the codec parameters
		struct bt_a2dp_codec_sbc_params *codec_params =
				(struct bt_a2dp_codec_sbc_params *)&configResult->config.media_config->codec_ie[0];

		g_phoneA2dpConfig.sample_rate = bt_a2dp_sbc_get_sampling_frequency(codec_params);
		g_phoneA2dpConfig.channel_mode = bt_a2dp_sbc_get_channel_num(codec_params);
		g_phoneA2dpConfig.block_length = bt_a2dp_sbc_get_block_length(codec_params);
		g_phoneA2dpConfig.subbands = bt_a2dp_sbc_get_subband_num(codec_params);
		g_phoneA2dpConfig.allocation_method = bt_a2dp_sbc_get_allocation_method(codec_params);
		g_phoneA2dpConfig.min_bitpool = codec_params->min_bitpool;
		g_phoneA2dpConfig.max_bitpool = codec_params->max_bitpool;

		PRINTF("Sample Rate: %s Hz,%x\r\n",
						g_phoneA2dpConfig.sample_rate == A2DP_SBC_SAMP_FREQ_16000 ? "16000" :
								g_phoneA2dpConfig.sample_rate == A2DP_SBC_SAMP_FREQ_32000 ? "32000" :
										g_phoneA2dpConfig.sample_rate == A2DP_SBC_SAMP_FREQ_44100 ? "44100" :
												g_phoneA2dpConfig.sample_rate == A2DP_SBC_SAMP_FREQ_48000 ? "48000" : "Unknown",
														g_phoneA2dpConfig.sample_rate);

#ifdef APP_DEBUG_EN
		// Print Macro Names and Values
		PRINTF("=== Codec Configuration ===\r\n");

		PRINTF("Sample Rate: %s Hz,%x\r\n",
				g_phoneA2dpConfig.sample_rate == A2DP_SBC_SAMP_FREQ_16000 ? "16000" :
						g_phoneA2dpConfig.sample_rate == A2DP_SBC_SAMP_FREQ_32000 ? "32000" :
								g_phoneA2dpConfig.sample_rate == A2DP_SBC_SAMP_FREQ_44100 ? "44100" :
										g_phoneA2dpConfig.sample_rate == A2DP_SBC_SAMP_FREQ_48000 ? "48000" : "Unknown",
												g_phoneA2dpConfig.sample_rate);
		PRINTF("Channel Mode: %s,%x\r\n",
				g_phoneA2dpConfig.channel_mode == A2DP_SBC_CH_MODE_MONO ? "Mono" :
						g_phoneA2dpConfig.channel_mode == A2DP_SBC_CH_MODE_DUAL ? "Dual Channel" :
								g_phoneA2dpConfig.channel_mode == A2DP_SBC_CH_MODE_STREO ? "Stereo" :
										g_phoneA2dpConfig.channel_mode == A2DP_SBC_CH_MODE_JOINT ? "Joint Stereo" : "Unknown",
												g_phoneA2dpConfig.channel_mode);
		PRINTF("Block Length: %d,%x\r\n",
				(g_phoneA2dpConfig.block_length == A2DP_SBC_BLK_LEN_4 ? 4 :
						g_phoneA2dpConfig.block_length == A2DP_SBC_BLK_LEN_8 ? 8 :
								g_phoneA2dpConfig.block_length == A2DP_SBC_BLK_LEN_12 ? 12 : 16),
								g_phoneA2dpConfig.block_length);
		PRINTF("Subbands: %d\r\n",
				(g_phoneA2dpConfig.subbands == A2DP_SBC_SUBBAND_4 ? 4 : 8),
				g_phoneA2dpConfig.subbands);
		PRINTF("Allocation Method: %s,%x\r\n",
				g_phoneA2dpConfig.allocation_method == A2DP_SBC_ALLOC_MTHD_SNR ? "SNR" : "Loudness",
						g_phoneA2dpConfig.allocation_method);
		PRINTF("Min Bitpool: 0x%X\r\n", g_phoneA2dpConfig.min_bitpool);
		PRINTF("Max Bitpool: 0x%X\r\n", g_phoneA2dpConfig.max_bitpool);
		PRINTF("================================\r\n");
#endif

		cmd_init_ct();

		/*AVRCP Profile level connection*/
		avrcp_control_connect(conn_rider_phone);
		if((!g_avrcpControlConnectHfpHf) && (conn_rider_hs != NULL) ) {
			vTaskDelay(100);
			avrcp_control_connect(conn_rider_hs);
		}
	}
	else
	{
		PRINTF("SBC_Deconfigured return (err %d)\n", configResult->err);
	}
}

void sbc_deconfigured(int err)
{
	if (err == 0)
	{
		g_audioStart = 0;
		/* Stop Audio Player */
		PRINTF("a2dp deconfigure\r\n");
	}
	else
	{
		PRINTF("SBC_Configured callback return (err %d)\n", err);
	}
}
uint8_t app_get_snk_a2dp_status()
{
	return g_audioStart;
}
void sbc_start_play(int err)
{

	if (err == 0)
	{
		g_audioStart = 1;

		if(!app_get_a2dp_mode())
		{
			/* Start Audio Player */
			app_a2dp_src_start(1);
			//PRINTF("a2dp start playing\r\n");

		} else
		{
			avrcp_pause_button(1);
		}
	}
	else
	{
		PRINTF("SBC_Start_Play Callback return (err %d)\n", err);
	}

}

void sbc_stop_play(int err)
{
	if (err == 0)
	{
		g_audioStart = 0;
		/* Stop Audio Player */
		if(!app_get_a2dp_mode())
		{
			app_a2dp_src_stop(1);
			PRINTF("a2dp stop playing\r\n");
		}
	}
	else
	{
		PRINTF("SBC_Stop_Play Callback return (err %d)\n", err);
	}
}

void sbc_streamer_data(uint8_t *data, uint32_t length)
{

	if ((data != NULL) && (length != 0U))
	{
		if(0 == g_audioStart)
		{
			/*return;*/
		}

		if(!app_get_a2dp_mode())
		{
			//PRINTF("a2dp streamer \n");

			data_send_source(data, length);
		}
	}
}

static void a2dp_snk_edgefast_a2dp_init(void)
{
	sbcEndpoint.control_cbs.configured = sbc_configured;
	sbcEndpoint.control_cbs.deconfigured = sbc_deconfigured;
	sbcEndpoint.control_cbs.start_play = sbc_start_play;
	sbcEndpoint.control_cbs.stop_play = sbc_stop_play;
	sbcEndpoint.control_cbs.sink_streamer_data = sbc_streamer_data;
	bt_a2dp_register_endpoint(&sbcEndpoint, BT_A2DP_AUDIO, BT_A2DP_SINK);
}

void sdp_discover_for_a2dp_source(void)
{
	int res;
	res = bt_sdp_discover(conn_rider_phone, &discov_a2dp_source);

	if (res)
	{
		PRINTF("A2DP-Sink discovery failed: result\r\n");
	}
	else
	{
		PRINTF("A2DP-Sink discovery started\r\n");
	}
}
void a2dp_sink_ready()
{
	int err = 0;


	API_RESULT retval;
	retval = API_SUCCESS;

	UCHAR new[1U] = {0x01};
	retval = BT_hci_vendor_specific_command(0x1A1,new,1);
	if(retval != API_SUCCESS)
	{
		PRINTF("A2DP Vendor command is not configured properly\n");
	}
	a2dp_snk_edgefast_a2dp_init();
}
