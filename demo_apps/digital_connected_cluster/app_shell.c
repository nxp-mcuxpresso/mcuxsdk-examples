/*
 * Copyright 2020 - 2021,2024-2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>
#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "ff.h"
#include <stdbool.h>
#include <porting.h>
#include <sys/atomic.h>
#include <sys/byteorder.h>
#include <sys/util.h>
#include <sys/slist.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include "sco_audio_pl.h"
#include <bluetooth/hfp_ag.h>
#include <bluetooth/hfp_hf.h>
#include "fsl_debug_console.h"
#include "fsl_shell.h"
#include "app_shell.h"

#include "app_discover.h"
#include "app_connect.h"
#include "app_avrcp.h"
#include "app_handsfree.h"
#include "app_handsfree_ag.h"
#include "app_map_mce.h"
#include "app_pbap_pce.h"
#include "display_interface.h"
#include "app_dcc.h"
#include "app_a2dp_source.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static shell_status_t shellBt(shell_handle_t shellHandle, int32_t argc, char **argv);
extern void app_enable_intercom(uint8_t en);
/*******************************************************************************
 * Variables
 ******************************************************************************/

SHELL_COMMAND_DEFINE(bt,
                     "\r\n\"bt\": BT related function\r\n"
                     "  USAGE: bt [discover|connect|disconnect|delete_all|paired_list]\r\n"
					 "    discover             start to find Bluetooth devices\r\n"
					 "    connect <device_index>   \n"
					 "                         connect to the device that is found in discovery,\r\n"
                     "                         bt connect <device index> index is device index from discovery list displayed \r\n"
					 "						   bt connect <device index> PH/ph connect as Passenger headset, default is Rider headset"
                  	 "    connect_paired  <device_index> \n"
                  	 "                         The <device index> is from paired device list displayed"
                     "    disconnect           disconnect Bluetooth link \r\n"
                     "                         bt disconnect <device type> \r\n"
                     "                         For example:\r\n"
                     "                         bt disconnect M/m -> for Rider mobile device disconnection\r\n"
                     "                         bt disconnect H/h -> for Rider headset disconnection\r\n"
					 "						   bt disconnect PH/ph -> for Passenger headset disconnection\r\n"
                     "    delete_all           delete all devices. Ensure to disconnect the HCI link connection with the peer "
                     "                         device before attempting to delete the bonding information.\r\n"
		             "    delete_dev           bt delete_dev <index> To delete particular device from paired list\r\n"
		             "    paired_list          Get the paired devices\r\n"
		             "    set_hstype           bt set_hstype <index> <type>"
		             "                         index - index from paired list"
		             "                         type PH/ph - set as passenger headset"
		             "                         type H/h - set as rider headset"


		             "\r\n\"bt\": AVRCP related function\r\n"
                     "    USAGE: bt [play|pause|volume up|volume down|next|previous]\r\n"
                     "    play                 to play the song\r\n"
		             "    pause                to pause the song for\r\n"
		             "    volume up            increase the volume\r\n"
		             "    volume down          decrease the volume\r\n"
		             "    next                 goto the next song\r\n"
		             "    previous             goto the previous song\r\n"
		             "    get_song_detail      get current playing song details"
		             "    hs_play              <option>  to start music in DUAL A2DP mode"
		             "                         h: start play with rider headset  ph: with passenger headset"
					 "    hs_stop              <option>  to stop music in DUAL A2DP mode"
				     "                         h: stop play with rider headset   ph: with passenger headset"

					 "\r\n\"bt\": HFP related function\r\n"
                   	 "  USAGE: bt [dial|aincall|eincall]\r\n"
					 "    dial                 dial out call.\r\n"
					 "    hf_aincall           accept the incoming call.\r\n"
					 "    hf_eincall <index>   index 0: call decline w/o msg 1: with msg  end an incoming call.\r\n"
		             "    clcc                 Query list of current calls.\r\n"
					 "    micVolume            Update mic Volume.\r\n"
					 "    speakerVolume        Update Speaker Volume.\r\n"
					 "    lastdial             call the last dialed number.\r\n"

					 "\r\n\"bt\": PBAP related function\r\n"
        			 "  USAGE: bt [get_pbap_data | contact_list ]"
        			 "    get_pbap_data <option>  0:All contacts 1:Missed calls 2:Incoming 3:Outgoing \r\n"
					 "    contact_list 0           show downloaded contact list\r\n"

					 "\r\n\"bt\": MAP related function\r\n"
		 	 	 	 "  USAGE: bt reg_msg_ntf           Register for message notification            \r\n"
					 "  USAGE: bt show_messages         To show the available messages rider wants to set\r\n"
					 "  USAGE: bt set_message <index>   To set the message get from the list  \r\n"
					 "  USAGE: bt get_message           To get the messages from connected mobile     \r\n"
					 "  USAGE: bt send_message          To send the message from the connected mobile  \r\n"

		 	 	 	 "\r\n\"bt\": A2DP bridge|DUAL A2DP mode functions\r\n"
	     	 	 	 "  USAGE: bt a2dp_mode <option>  0:A2DP bridge Phone music streaming to Rider Headset"
	     	 	 	 "                                1:DUAL A2DP USB drive music streaming to Rider Headset and Passenger Headset"
	     	 	 	 "                                2:DUAL A2DP default music streaming to Rider Headset and Passenger Headset \r\n",

                     shellBt,
                     SHELL_IGNORE_PARAMETER_COUNT);


SDK_ALIGN(static uint8_t s_shellHandleBuffer[SHELL_HANDLE_SIZE], 4);
static shell_handle_t s_shellHandle;

/*******************************************************************************
 * Code
 ******************************************************************************/
static uint32_t hfp_get_value_from_str(char *ch)
{
      uint8_t selectIndex = 0;
      uint8_t value = 0;

      for (selectIndex = 0; selectIndex < strlen(ch); ++selectIndex)
      {
          if ((ch[selectIndex] < '0') || (ch[selectIndex] > '9'))
          {
              PRINTF("The Dial parameter is wrong\r\n");
              return kStatus_SHELL_Error;
          }
      }
      if (selectIndex == 0U)
      {
          PRINTF("The Dial parameter is wrong\r\n");
      }
      else if(selectIndex == 1U)
      {
        value = (ch[0] - '0');
      }
      else if(selectIndex == 2U)
      {
        value = (ch[0] - '0')*10 + (ch[1] - '0');
      }
      return value;
}

static shell_status_t shellBt(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    uint8_t *addr;

    if (argc < 1)
    {
        PRINTF("the parameter count is wrong\r\n");
    }
    if (strcmp(argv[1], "discover") == 0)
    {
        app_discover();
    }
    else if (strcmp(argv[1], "discovery") == 0)
    {
    	bt_br_set_connectable(false);
        if (bt_br_set_connectable(true))
        {
            PRINTF("BR/EDR set/reset connectable failed\n");
            return kStatus_SHELL_Error;
        }
        if (bt_br_set_discoverable(true))
        {
            PRINTF("BR/EDR set discoverable failed\n");
            return kStatus_SHELL_Error;
        }
        PRINTF("BR/EDR set connectable and discoverable done\n");
    }
    else if (strcmp(argv[1], "connect") == 0)
    {
        uint8_t selectIndex = 0;
        char *ch            = argv[2];

        if (argc < 2)
        {
        	PRINTF("Invalid index parameter.\n");
            return kStatus_SHELL_Error;
        }

        for (selectIndex = 0; selectIndex < strlen(ch); ++selectIndex)
        {
            if ((ch[selectIndex] < '0') || (ch[selectIndex] > '9'))
            {
                PRINTF("the parameter is wrong\r\n");
                return kStatus_SHELL_Error;
            }
        }

        switch (strlen(ch))
        {
            case 1:
                selectIndex = ch[0] - '0';
                break;
            case 2:
                selectIndex = (ch[0] - '0') * 10 + (ch[1] - '0');
                break;
            default:
            	PRINTF("Invalid index parameter.\n");
            	return kStatus_SHELL_Error;
        }

        if (selectIndex == 0U || selectIndex > APP_INQUIRY_NUM_RESPONSES)
        {
        	PRINTF("Invalid index range. Choose between 1 and %d\n", APP_INQUIRY_NUM_RESPONSES);
        	return kStatus_SHELL_Error;
        }

        //Retrieve device address and COD from discovery list
        uint8_t *addr = br_discovery_results[selectIndex - 1].addr.val;
        uint8_t *cod = br_discovery_results[selectIndex - 1].cod;

        //Get device type using COD
        uint8_t device_type = get_device_type_from_cod(cod);

       // PRINTF("Device [%d]: Address: %02X:%02X:%02X:%02X:%02X:%02X, Type: %s\n",
       //selectIndex, addr[0], addr[1], addr[2], addr[3], addr[4], addr[5], device_type);

        if (g_pairedDeviceCount >= MAX_PAIRED_DEVICES)
        {
         	PRINTF("Paired devices reached max number, \n Please remove an existing device to pair new device.\n");
        	print_paired_devices();
        	return kStatus_SHELL_Error;
        }

        //Automatically connect if it's a Mobile Phone or Headset
        if (device_type == PHONE)
        {
        	app_connect(RIDER_PHONE,addr);

        }else if (device_type == HEADSET)
        {
        	if((strcmp(argv[3], "PH") == 0) || (strcmp(argv[3], "ph") == 0))
        	{
        		PRINTF("Connecting PASSENGER_HEADSET \n");
        		app_connect(PASSENGER_HEADSET, addr);

        	}else
        	{
        		app_connect(RIDER_HEADSET, addr);
        	}

        }else
        {
        	PRINTF("Device is not a Mobile Phone or Headset.\n");
        }
    }
    else if (strcmp(argv[1], "connect_paired") == 0)
    {
		if (argc < 2)
		{
			PRINTF("the parameter count is wrong\r\n");
			return kStatus_SHELL_Error;
		}
		uint8_t device_index = 0;
		char *ch = argv[2];
		device_index = ch[0] - '0';
		
		if(!g_auto_connection_status)
			connect_paired_device(device_index);
		else
			PRINTF("Please wait auto-connection in progress !\r\n");
    }
    else if(strcmp(argv[1], "set_hstype") == 0)
    {
    	if (argc < 3)
		{
			PRINTF("the parameter count is wrong\r\n");
			return kStatus_SHELL_Error;
		}

		uint8_t device_index = 0;
		char *ch = argv[2];
		device_index = ch[0] - '0';

		if(device_index < 1 || device_index > g_pairedDeviceCount)
		{
			PRINTF("Failed, Invalid device index !\n");
			return kStatus_SHELL_Error;
		}

    	uint8_t dev_type = paired_devices[device_index - 1].device_type;

    	if (!(((dev_type & 0x0F) == RIDER_HEADSET) || ( (dev_type & 0x0F) == PASSENGER_HEADSET)))
    	{
    		PRINTF("Failed, not a headset device !\n");
    		return kStatus_SHELL_Error;
    	}

    	if(((strcmp(argv[3], "H") == 0) || (strcmp(argv[3], "h") == 0))
    		&& (paired_devices[device_index - 1].device_type != RIDER_HEADSET))
    	{
    		paired_devices[device_index - 1].device_type = RIDER_HEADSET;

    	}
    	else if(((strcmp(argv[3], "PH") == 0) || (strcmp(argv[3], "ph") == 0))
    			&& ( (dev_type & 0x0F) != PASSENGER_HEADSET))
    	{
    		paired_devices[device_index - 1].device_type = PASSENGER_HEADSET;
    	}
    	else
    	{
    		PRINTF("Enter valid headset type: H/h or PH/ph \nOR \n");
    		PRINTF("Choose different headset type then the currently set\n");
    		return kStatus_SHELL_Error;
    	}

    	int err = app_save_paired_devices();
    	if (!err)
    		PRINTF("Updated headset type successfully\n\n");

    }
    else if (strcmp(argv[1], "disconnect") == 0)
    {
    	if((strcmp(argv[2], "M") == 0) || (strcmp(argv[2], "m") == 0))
    	{
    		app_disconnect(RIDER_PHONE);
    	}
    	else if((strcmp(argv[2], "H") == 0) || (strcmp(argv[2], "h") == 0))
    	{
    		app_disconnect(RIDER_HEADSET);

    	}else if((strcmp(argv[2], "PH") == 0) || (strcmp(argv[2], "ph") == 0))
    	{
    		app_disconnect(PASSENGER_HEADSET);
    	}
    	else
    	{
    		printf("Enter valid argument\n"
    				"M or m to disconnect rider phone\n"
    				"H or h to disconnect rider headset\n"
    				"PH or ph to disconnect passenger headset\n");
    	}
    }
//**************AVRCP commands start ***************
    else if (strcmp(argv[1], "play") == 0)
    {
			uint8_t select_op = 0;
			char *ch = argv[2];
			if (argc < 2)
					{
						PRINTF("the parameter count is wrong\r\n");
						return kStatus_SHELL_Error;
					}
			select_op = ch[0] - '0';
			avrcp_play_button(select_op);
	}
    else if (strcmp(argv[1], "pause") == 0)
	{
			uint8_t select_op = 0;
			char *ch = argv[2];
			if (argc < 2)
					{
						PRINTF("the parameter count is wrong\r\n");
						return kStatus_SHELL_Error;
					}
			select_op = ch[0] - '0';

			avrcp_pause_button(select_op);
	}
    else if (strcmp(argv[1], "next") == 0)
    {
          	uint8_t select_op = 0;
          	char *ch = argv[2];
          	if (argc < 2)
          	        {
          	            PRINTF("the parameter count is wrong\r\n");
          	            return kStatus_SHELL_Error;
          	        }
         // 	select_op = ch[0] - '0';
          	avrcp_forward_backward(1);
    }

    else if (strcmp(argv[1], "previous") == 0)
    {
          	uint8_t select_op = 0;
          	char *ch = argv[2];
          	if (argc < 2)
          	        {
          	            PRINTF("the parameter count is wrong\r\n");
          	            return kStatus_SHELL_Error;
          	        }
         // 	select_op = ch[0] - '0';

          	avrcp_forward_backward(0);
    }
    else if (strcmp(argv[1], "volume") == 0)
    {
              	uint8_t select_op = 0;
              	char *ch = argv[2];
              	if (argc < 2)
              	        {
              	            PRINTF("the parameter count is wrong\r\n");
              	            return kStatus_SHELL_Error;
              	        }
              	if (strcmp(argv[2], "up") == 0)
              	{
              		avrcp_volume_up_down(1);

              	}else if (strcmp(argv[2], "down") == 0)
              	{
              		avrcp_volume_up_down(0);
              	}
    } else if (strcmp(argv[1], "a2dp_mode") == 0)
    {
              	uint8_t select_op = 0;
              	char *ch = argv[2];
              	if (argc < 2)
              	        {
              	            PRINTF("the parameter count is wrong\r\n");
              	            return kStatus_SHELL_Error;
              	        }

              	select_op = ch[0] - '0';

              	app_set_a2dp_music_source(select_op);

    }else if (strcmp(argv[1], "hs_play") == 0)
    {
              	uint8_t select_op = 0;
              	char *ch = argv[2];
              	if (argc < 2)
              	        {
              	            PRINTF("the parameter count is wrong\r\n");
              	            return kStatus_SHELL_Error;
              	        }

              	if (strcmp(argv[2], "h") == 0)
              	{
              		app_a2dp_music_play(1);


              	}else if (strcmp(argv[2], "ph") == 0)
              	{
              		app_a2dp_music_play(0);
              	}

    }else if (strcmp(argv[1], "hs_stop") == 0)
    {
              	uint8_t select_op = 0;
              	char *ch = argv[2];
              	if (argc < 2)
              	        {
              	            PRINTF("the parameter count is wrong\r\n");
              	            return kStatus_SHELL_Error;
              	        }

              	if (strcmp(argv[2], "h") == 0)
              	{
              		app_a2dp_music_stop(1);


              	}else if (strcmp(argv[2], "ph") == 0)
              	{
              		app_a2dp_music_stop(0);
              	}

    }
    else if (strcmp(argv[1], "ca_connect") == 0)
    {
               	//uint8_t select_op = 0;
               	char *ch = argv[2];
               	if (argc < 2)
               	        {
               	            PRINTF("the parameter count is wrong\r\n");
               	            return kStatus_SHELL_Error;
               	        }
               	//select_op = ch[0] - '0';
				avrcp_ca_init_initiator();
				avrcp_ca_init_connect();
    }
    else if (strcmp(argv[1], "ca_getimage") == 0)
    {
                //uint8_t select_op = 0;
                char *ch = argv[2];
                if (argc < 2)
                   	{
                   	     PRINTF("the parameter count is wrong\r\n");
                   	     return kStatus_SHELL_Error;
                   	 }
                 //select_op = ch[0] - '0';
                 avrcp_ca_get_image();
    }
    else if (strcmp(argv[1], "ct_connect") == 0)
    {
               	//uint8_t select_op = 0;
                char *ch = argv[2];
                if (argc < 2)
                   {
                       	  PRINTF("the parameter count is wrong\r\n");
                       	  return kStatus_SHELL_Error;
                    }
                //select_op = ch[0] - '0';
                cmd_control_connect();
    }
    else if (strcmp(argv[1], "browsing_connect") == 0)
        {
        	//uint8_t select_op = 0;
        	char *ch = argv[2];
        	if (argc < 2)
        	{
        		PRINTF("the parameter count is wrong\r\n");
        		return kStatus_SHELL_Error;
        	}
        	//select_op = ch[0] - '0';
        	cmd_browsing_connect();
        }
        else if (strcmp(argv[1], "browse_song_list") == 0)
        {
        	//uint8_t select_op = 0;
        	char *ch = argv[2];
        	if (argc < 2)
        	{
        		PRINTF("the parameter count is wrong\r\n");
        		return kStatus_SHELL_Error;
        	}
        	//select_op = ch[0] - '0';
        	avrcp_browse_get_folder_items();
        }
        else if (strcmp(argv[1], "play_selected_song") == 0)
        {
        	uint8_t select_index = 0;
        	char *ch = argv[2];

        	if (argc < 2)
        	{
        		PRINTF("the parameter count is wrong\r\n");
        		return kStatus_SHELL_Error;
        	}

        	for (select_index = 0; select_index < strlen(ch); ++select_index)
        	{
        		if ((ch[select_index] < '0') || (ch[select_index] > '9'))
        		{
        			PRINTF("the parameter is wrong\r\n");
        			return kStatus_SHELL_Error;
        		}
        	}

        	switch (strlen(ch))
        	{
        	case 1:
        		select_index = ch[0] - '0';
        		break;
        	case 2:
        		select_index = (ch[0] - '0') * 10 + (ch[1] - '0');
        		break;
        	default:
        		PRINTF("the parameter is wrong\r\n");
        		break;
        	}

        	if (select_index == 0U)
        	{
        		PRINTF("the parameter is wrong\r\n");
        	}

        	avrcp_browse_play_folder_items(select_index);

        }
        else if (strcmp(argv[1], "get_song_detail") == 0)
        {
        	//uint8_t select_op = 0;
        	char *ch = argv[2];
        	if (argc < 2)
        	{
        		PRINTF("the parameter count is wrong\r\n");
        		return kStatus_SHELL_Error;
        	}
        	//select_op = ch[0] - '0';
        	//avrcp_get_playsong_detail();
			avrcp_ct_get_element_attributes();
        }		
//**************AVRCP commands end***************

//**************PBAP commands start**************
    else if (strcmp(argv[1], "get_pbap_data") == 0)
    {
    	char *ch = argv[2];
    	if (argc < 2)
    	{
    		PRINTF("the parameter count is wrong\r\n");
    		return kStatus_SHELL_Error;
    	}
    	if (strcmp(argv[2], "0") == 0)
    	{
    		/*CALL_TYPE_PB*/
    		pbap_pull_phonebook(0);
    	}
    	else if (strcmp(argv[2], "1") == 0)
    	{
    		/*CALL_TYPE_MISSED*/
    		pbap_pull_phonebook(1);
    	}
    	else if (strcmp(argv[2], "2") == 0)
    	{
    		/*CALL_TYPE_INCOMING*/
    		pbap_pull_phonebook(2);
    	}
    	else if (strcmp(argv[2], "3") == 0)
    	{
    		/*CALL_TYPE_OUTGOING*/
    		pbap_pull_phonebook(3);
    	}
    }else if (strcmp(argv[1], "contact_list") == 0)
    {
    	uint8_t select_op = 0;
    	char *ch = argv[2];
    	if (argc < 2)
    	{
    		PRINTF("the parameter count is wrong\r\n");
    		return kStatus_SHELL_Error;
    	}
    	if (strcmp(argv[2], "0") == 0)
    	{
    		/*CALL_TYPE_PB*/
    		print_contact_list(0);
    	}
    	else if (strcmp(argv[2], "1") == 0)
    	{
    		/*CALL_TYPE_MISSED*/
    		print_contact_list(1);
    	}
    	else if (strcmp(argv[2], "2") == 0)
    	{
    		/*CALL_TYPE_INCOMING*/
    		print_contact_list(2);
    	}
    	else if (strcmp(argv[2], "3") == 0)
    	{
    		/*CALL_TYPE_OUTGOING*/
    		print_contact_list(3);
    	}


    }
//**************PBAP commands end ***************
    else if (strcmp(argv[1], "openaudio") == 0)
    {
        app_hfp_ag_open_audio(RIDER_HEADSET);
    }
    else if (strcmp(argv[1], "closeaudio") == 0)
    {
        app_hfp_ag_close_audio(RIDER_HEADSET);
    }
    else if (strcmp(argv[1], "sincall") == 0)
    {
    	PRINTF("INCOMING\n");
        app_hfp_ag_start_incoming_call();
    }
    else if (strcmp(argv[1], "stwcincall") == 0)
    {
        app_hfp_ag_start_twc_incoming_call();
    }
    else if (strcmp(argv[1], "ag_aincall") == 0)
    {
        app_hfp_ag_accept_incoming_call();
    }
    else if (strcmp(argv[1], "ag_eincall") == 0)
    {
        app_hfp_ag_stop_incoming_call();

    }else if (strcmp(argv[1], "clcc") == 0)
    {
        hfp_hf_query_list_current_calls();
    }
    else if (strcmp(argv[1], "select_codec") == 0)
    {
        if (argc < 2)
        {
            PRINTF("the parameter count is wrong\r\n");
            return kStatus_SHELL_Error;
        }
        app_hfp_ag_codec_select(RIDER_HEADSET,hfp_get_value_from_str(argv[2]));
    }
    else if (strcmp(argv[1], "set_mic_volume") == 0)
    {
        if (argc < 2)
        {
            PRINTF("the parameter count is wrong\r\n");
            return kStatus_SHELL_Error;
        }
        app_hfp_ag_volume_update(hf_volume_type_mic, hfp_get_value_from_str(argv[2]));

    } 
    else if (strcmp(argv[1], "set_speaker_volume") == 0)
    {
        if (argc < 2)
        {
            PRINTF("the parameter count is wrong\r\n");
            return kStatus_SHELL_Error;
        }
        app_hfp_ag_volume_update(hf_volume_type_speaker, hfp_get_value_from_str(argv[2]));
    }
    else if (strcmp(argv[1], "set_tag") == 0)
    {
        uint8_t selectIndex = 0;
        char *ch            = argv[2];

        if (argc < 2)
        {
            PRINTF("the parameter count is wrong\r\n");
            return kStatus_SHELL_Error;
        }

        for (selectIndex = 0; selectIndex < strlen(ch); ++selectIndex)
        {
            if ((ch[selectIndex] < '0') || (ch[selectIndex] > '9'))
            {
                PRINTF("The Dial parameter is wrong\r\n");
                return kStatus_SHELL_Error;
            }
        }

        if (selectIndex == 0U)
        {
            PRINTF("The Dial parameter is wrong\r\n");
        }
        app_hfp_ag_set_phnum_tag(ch);
    }
	else if (strcmp(argv[1], "delete_all") == 0)
    {
		int err = 0;

		if (conn_rider_hs != NULL)
		{
			app_disconnect(RIDER_HEADSET);
			while(conn_rider_hs != NULL);
		}
		if (conn_rider_phone != NULL)
		{
			app_disconnect(RIDER_PHONE);
			while(conn_rider_phone != NULL);
		}
		if (conn_passenger_hs != NULL)
		{
			app_disconnect(PASSENGER_HEADSET);
			while(conn_passenger_hs != NULL);
		}

		err = bt_unpair(BT_ID_DEFAULT, NULL);

		if (err != 0)
		{
			PRINTF("failed reason = %d\r\n", err);
		}
		else
		{
			PRINTF("success\r\n");
			PRINTF("clear_paired_devices_from_lfs.\n\n");
			vTaskDelay(pdMS_TO_TICKS(50));
			app_clear_paired_devices();
		}
    }
	else if (strcmp(argv[1], "delete_dev") == 0)
	{
        if (argc < 2)
        {
            PRINTF("the parameter count is wrong\r\n");
            return kStatus_SHELL_Error;
        }
		uint8_t device_index = 0;
		char *ch = argv[2];
		device_index = ch[0] - '0';

		if(device_index < 1 || device_index > g_pairedDeviceCount)
		{
			PRINTF("Failed, Invalid device index !\n");
			return kStatus_SHELL_Error;
		}

		uint8_t dev_type = paired_devices[device_index - 1].device_type;


		if (( (dev_type & 0x0F) == RIDER_PHONE) && 	(conn_rider_phone != NULL))
		{
			app_disconnect(RIDER_PHONE);
			PRINTF("Disconnecting device...\n");
			while(conn_rider_phone != NULL);
		}
		else if (( (dev_type & 0x0F) == RIDER_HEADSET) && (conn_rider_hs != NULL))
		{
			app_disconnect(RIDER_HEADSET);
			PRINTF("Disconnecting device...\n");
			while(conn_rider_hs != NULL);
		}
		else if (( (dev_type & 0x0F) == PASSENGER_HEADSET) && (conn_passenger_hs != NULL))
		{
			app_disconnect(PASSENGER_HEADSET);
			PRINTF("Disconnecting device...\n");
			while(conn_passenger_hs != NULL);
		}

		PRINTF("delete_device:%d\n",device_index);
		vTaskDelay(pdMS_TO_TICKS(50));
		delete_device(device_index);
	}
	else if (strcmp(argv[1], "paired_list") == 0)
    {
		print_paired_devices();
    }
    else if (strcmp(argv[1], "dial") == 0)
    {
        uint8_t selectIndex = 0;
        char *ch            = argv[2];

        if (argc < 2)
        {
            PRINTF("the parameter count is wrong\r\n");
            return kStatus_SHELL_Error;
        }

        for (selectIndex = 0; selectIndex < strlen(ch); ++selectIndex)
        {
            if ((ch[selectIndex] < '0') || (ch[selectIndex] > '9'))
            {
                PRINTF("The Dial parameter is wrong\r\n");
                return kStatus_SHELL_Error;
            }
        }

        if (selectIndex == 0U)
        {
            PRINTF("The Dial parameter is wrong\r\n");
        }
        hfp_dial(ch);
    }
    else if (strcmp(argv[1], "svr") == 0)
    {
        hfp_start_voice_recognition();
    }
    else if (strcmp(argv[1], "evr") == 0)
    {
        hfp_stop_voice_recognition();
    }
     else if (strcmp(argv[1], "micVolume") == 0)
    {
        if (argc < 2)
        {
            PRINTF("the parameter count is wrong\r\n");
            return kStatus_SHELL_Error;
        }
        hfp_volume_update(hf_volume_type_mic, hfp_get_value_from_str(argv[2]));

    }
    else if (strcmp(argv[1], "speakerVolume") == 0)
    {
        if (argc < 2)
        {
            PRINTF("the parameter count is wrong\r\n");
            return kStatus_SHELL_Error;
        }
        hfp_volume_update(hf_volume_type_speaker, hfp_get_value_from_str(argv[2]));
    }

   else if (strcmp(argv[1], "lastdial") == 0)
   {
      hfp_last_dial();
    }
   else if (strcmp(argv[1], "memorydial") == 0)
   {
      if (argc < 2)
      {
          PRINTF("the parameter count is wrong\r\n");
          return kStatus_SHELL_Error;
      }
      dial_memory(hfp_get_value_from_str(argv[2]));
    }
    else if (strcmp(argv[1], "clip") == 0)
    {
       hfp_enable_clip(1);
    }
    else if (strcmp(argv[1], "disclip") == 0)
    {
       hfp_enable_clip(0);
    }
    else if (strcmp(argv[1], "ccwa") == 0)
    {
       hfp_enable_ccwa(1);
    }
    else if (strcmp(argv[1], "disccwa") == 0)
    {
       hfp_enable_ccwa(0);
    }
     else if (strcmp(argv[1], "multipcall") == 0)
    {
        if (argc < 2)
        {
            PRINTF("the parameter count is wrong\r\n");
            return kStatus_SHELL_Error;
        }
        hfp_multiparty_call_option(hfp_get_value_from_str(argv[2]));
    }

    else if (strcmp(argv[1], "hf_aincall") == 0)
    {

      hfp_AnswerCall();
    }
    else if (strcmp(argv[1], "voicetag") == 0)
    {

      hfp_hf_get_last_voice_tag_number();
    }

    else if (strcmp(argv[1], "hf_eincall") == 0)
    {
    	if(strcmp(argv[2], "0") == 0)
    	{
    		hfp_RejectCall(0);
    	}
    	else if(strcmp(argv[2], "1") == 0)
    	{
    		hfp_RejectCall(1);
    	}
    	else
    	{
    		PRINTF("Please select the valid option to hangup the call\r\n");
    	}
    }
    else if (strcmp(argv[1], "triggercodec") == 0)
    {
        hfp_trigger_codec_connection();
    }
    else if (strcmp(argv[1], "getIndicatorStatus") == 0)
    {
        bt_hfp_hf_get_peer_indicator_status(conn_rider_phone);
    }
	else if(strcmp(argv[1], "show_messages") == 0)
	{
		/*Standred messages*/
		app_print_map_messages();
	}
	else if (strcmp(argv[1], "set_message") == 0)
	{
		char *index_str;
		char *number = "+910000000000";
		uint8_t index = 0U;

		if (argc < 2)
		{
			app_print_map_messages();
		    return kStatus_SHELL_Error;
		}

		index_str = argv[2];

		if ((index_str[0] == '1') || (index_str[0] == '2') || (index_str[0] == '3'))
		{
		     index = index_str[0] - '0';
		}
		else
		{
		     PRINTF("the parameter is wrong\r\n");
		     app_print_map_messages();
		     return kStatus_SHELL_Error;
		}
		app_map_push_msg_index(index - 1);
		app_build_map_msg(number);

	}
	else if (strcmp(argv[1], "get_message") == 0)
    {
    	if (argc < 2)
    	{
    		PRINTF("the parameter count is wrong\r\n");
    		return kStatus_SHELL_Error;
    	}
    	map_pull_message(hfp_get_value_from_str(argv[2]));

    }
	else if (strcmp(argv[1], "send_message") == 0)
	{
		app_map_push_message();
	}
	else if (strcmp(argv[1], "reg_msg_ntf") == 0)
    {
    	if (argc < 2)
    	{
    		PRINTF("the parameter count is wrong\r\n");
    		return kStatus_SHELL_Error;
    	}
    	if(conn_rider_phone!=NULL)
    	{
    		map_mce_mns_reg_connect(conn_rider_phone);
    	}else {
    		PRINTF("Error ! Rider Phone not connected !\r\n");
    	}
    }
	else if(strcmp(argv[1], "intercom") == 0)
	{
		char *index_str;
		uint8_t index = 0U;

		if (argc < 2)
		{
		     PRINTF("the parameter count is wrong\r\n");
		     return kStatus_SHELL_Error;
		}

		index_str = argv[2];

		if ((index_str[0] == '0') || (index_str[0] == '1'))
		{
		     index = index_str[0] - '0';
		}
		else
		{
		     PRINTF("the parameter is wrong\r\n");
		     return kStatus_SHELL_Error;
		}
		app_enable_intercom(index);
	}
    else
    {
    	PRINTF("invalid parameter\n");
    }

    return kStatus_SHELL_Success;
}

void app_shell_init(void)
{
    DbgConsole_Flush();
    /* Init SHELL */
    s_shellHandle = &s_shellHandleBuffer[0];
    SHELL_Init(s_shellHandle, g_serialHandle, ">> ");
    PRINTF("\r\n");

    /* Add new command to commands list */
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(bt));
}
