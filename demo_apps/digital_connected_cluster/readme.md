# digital_connected_cluster

## Overview
This example demo is for demonstrating multiple Bluetooth profiles tailored to two-wheeler use case scenarios, enabling below features for enhanced rider experience.
> Where device is connected to Rider Mobile, Rider Headset and Passenger Headset. 

1.  A2DP audio bridging : Phone music is played to Rider Headset. Device receives the audio streaming data and forwards to Rider Headset. 
2.  eSCO bridge (HFP call bridging) : Phone call audio is routed to Rider Headset.  User can answer\reject call from Rider headset or using shell command.
                                      Once eSCO connection with Phone and Rider Headset established the IW611\IW612 controller will forward eSCO Audio packets from Phone to Rider Headset & vice versa. 
									  
3.  DUAL A2DP : Play local default music or music from USB drive to both Rider and Passenger Headset. 

4.  Intercom : With this feature Rider and Passenger will able to communicate each other. User can end intercom with shell command or end call from Headset.

5.  PBAP and MAP : Phone contact download using PBAP profile, regeter & receive message notification. 


## Prepare the Demo

1.  Open example's project and build it.

2.  Connect a USB cable between the PC host and the OpenSDA USB port on the target board.

3.  Open a serial terminal on PC for OpenSDA serial device with these settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

4.  Download the program to the target board.

5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the example.

## Running the demo
The log below shows the output of the example in the terminal window. 

Note:
1. Please use the command "help" to view the specific commands supported by the example.
2. The shell information "SHELL build: Aug 10 2021" may be different, which depends on the compile date.
3. For MAP and PBAP with iOS, need to allow contact sync and message notification from Bluetooth settings.
4. When running example first time, make sure to delete existing all paired devices.
   bt delete_all 

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DIGITAL CONNECTED CLUSTER Demo started....
download starts(400568)
........................................................................................................................
download success!
Bluetooth initialized
Copyright  2024  NXP
>>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The suppprted command details are as follows :
"bt": BT related function

  USAGE: bt [discover|connect|disconnect|delete_all|paired_list]

    discover             to find Bluetooth devices

    connect <device_index>
                         connect to the device that is found in discovery,

                         bt connect <device index> index is device index from discovery list displayed

						 bt connect <device index>             // Connect to Rider Headset or Mobile.
                         bt connect <device index>  PH/ph     // Connect to Passenger Headset. 
						    
	connect_paired  <device_index>
	
                         The <device index> is from paired device list displayed 
						 
	disconnect           disconnect Bluetooth link

                         bt disconnect <device type>

                         For example:

                         bt disconnect M/m -> for Rider mobile device disconnection

                         bt disconnect H/h -> for Rider headset disconnection

                         bt disconnect PH/ph -> for Passenger headset disconnection

    delete_all           delete all devices. The device is connected then it will be disconnected.

    delete_dev <index>   To delete particular device from paired list

    paired_list          Get the paired devices

    set_hstype           bt set_hstype <index> <type>                        
							index - index from paired list                        
							type ph - set as passenger headset                        
							type h - set as rider headset

"bt": AVRCP related function

    USAGE: bt [play|pause|volume up|volume down|next|previous]

    play                 to play the song

    pause                to pause the song for

    volume up            increase the volume

    volume down          decrease the volume

    next                 goto the next song

    previous             goto the previous song

    get_song_detail      get current playing song details   
	
	hs_play<option>      to start music in DUAL A2DP mode                        
							h: start play with rider headset 
							ph: with passenger headset  
						 
	hs_stop<option>      to stop music in DUAL A2DP mode            
							h: stop play with rider headset   
							ph: with passenger headset

"bt": HFP related function

  USAGE: bt [dial|aincall|eincall]

    dial                 dial out call.

    hf_aincall           accept the incoming call.

    hf_eincall <index>   index 0: call decline w/o msg 1: with msg  end an incoming call.

    clcc                 Query list of current calls.

    micVolume            Update mic Volume.

    speakerVolume        Update Speaker Volume.

    lastdial             call the last dialed number.



"bt": PBAP related function

  USAGE: bt [get_pbap_data | contact_list ]    
  
    get_pbap_data <option>  0:All contacts 1:Missed calls 2:Incoming 3:Outgoing

    contact_list 0           show downloaded contact list



"bt": MAP related function

  USAGE: bt reg_msg_ntf           Register for message notification

  USAGE: bt show_messages         To show the available default messages rider wants to set

  USAGE: bt set_message <index>   To set the default rejet call message.

  USAGE: bt get_message           To get the messages from connected mobile

  USAGE: bt send_message          To send the message from the connected mobile



"bt": A2DP bridge|DUAL A2DP mode functions

  USAGE: bt a2dp_mode <option> 
		0:A2DP bridge Phone music streaming to Rider Headset                             
		1:DUAL A2DP USB drive music streaming to Rider Headset and Passenger Headset                           
		2:DUAL A2DP default music streaming to Rider Headset and Passenger Headset

1.Device pair, connect and disconnection
		•bt discover – Start Bluetooth discovery to find nearby devices.
		•bt connect <index> – Connect to the discovered device at the specified index.
			Example: bt connect 1 (1 is index from discover list)
		•The headset by default identified as Rider Headset, to mark it as passenger headset use additional argument “ph”
			Example: bt connect 1  ph  (1 is index from discover list, ph for passenger headset)
		•bt disconnect <M(m)/H(h)/PH()ph> - Disconnect the Rider mobile, Rider headset and Passenger Headset

2.Connecting Paired Devices
		•If a device is already paired, then can connect using pair list index. 
			Example: bt paired_list – Retrieve the list of paired devices.
		•bt connect_paired <index> – Connect to a device from the paired list.
			Example: bt connect_paired 1 (Connects to the first device from paired list)

3.Deleting Paired Devices
		•bt delete_dev <index> – Delete a specific paired device.
			Example: bt delete_dev 1 (Deletes the first paired device)
		•bt delete_all – Remove all paired devices.(Please note, if any connection exists, will be disconnected when deleting pairing)

4.Change Headset type
		Following command can be used to change headset type i.e. from Rider headset to Passenger headset or Passenger headset to Rider headset 
		•bt set_hstype <index> <type>  : where index is - index from paired list
		- type PH/ph - set as passenger headset
		- type H/h - set as rider headset
		
5.Running A2DP Bridge
		After connecting the mobile phone and headset, the user can play music from the mobile to headset, and control music play\pause and 
		other operations via command line or few of the operations from headset directly i.e. play\pause, next\prev.
		Before using below operations, make sure a2dp_mode is set to 0.
		•bt a2dp_mode 0       // Enable a2dp bridge, 
		•bt play – Play the song.
		•bt pause – Pause the song.
		•bt volume up – Increase the volume.
		•bt volume down – Decrease the volume.
		•bt next – Skip to the next song.
		•bt previous – Go to the previous song.
		•bt get_song_detail – Retrieve details of the currently playing song.  
		•bt browse_getfolderitems   - retrieve list of songs 

6.Running SCO Bridge
		If an incoming call is received, the DUT displays the caller’s name and number and can answer\reject call using cli command or from headset. 
		The user can manage calls using the following commands:
		Making & Receiving Calls
		•bt dial <mobile_num> – Dial an outgoing call.
			Example: bt dial 9876543210
		•bt hf_aincall – Accept an incoming call.
		•bt hf_eincall <option >  – End an active call\reject call with message and without message. 
			0 : reject call without message
			1 : reject call with message
		•bt lastdial – Call the last dialed number.

7.Running Dual A2DP Streaming
		Create connection to Rider Headset and Passenger Headset, after connection is success send below command to start music streaming to both headset .
			•bt a2dp_mode 2 - This will play default music to both headset. 
			•bt a2dp_mode 1 - This will play song from USB drive, the song should be 44.1Khz wave file with name “dcc_test.wav”
			•bt a2dp_mode 0 - Initiate play from phone or headset to start music from Phone. 

		In DUAL mode enabled, play/stop individual headsets
			•bt hs_play   <option>  h: start play with rider headset, ph: with passenger headset
			•bt hs_stop   <option>  h: stop play with rider headset, ph: with passenger headset
		Notes - By default, the phone music (A2DP bridge mode) is enabled. The DUAL A2DP is paused when intercom is started and if can’t be resumed until intercom is disconnected. 

8.Running Intercom Feature
		Create connection to Rider Headset and Passenger Headset, after connection is success send  below command to start intercom between headsets.
			•bt intercom 1  - To enable intercom 
			•bt intercom  0 - To disconnect intercom. 
		Like normal call if call end button press on headset will end the ongoing intercom session.  
		If there is incoming\outgoing call then intercom will get disconnected and will be resumed once phone call is ended. 

9. Running PBAP Functions
		When connecting to phone the PBAP connection is initiated automatically.
		To fetch phonebook contacts\missed call\incoming\outgoing calls can use below command
			•bt get_pbap_data 0 – download phonebook contacts.
			•bt get_pbap_data 1 - get missed call list
			•bt get_pbap_data 2 - get incoming call list
			•bt get_pbap_data 3 - get outgoing call list

10. Running the MAP functions.
		When connecting to phone the MAP connection is initiated automatically. 
			•bt reg_msg_ntf 	- After registration, new messages will be fetched and showed on terminal. 
			•bt show_messages   - To show the available messages rider wants to set during rejecting the call.
			•bt set_message <index>  -	To set the pre-defined message from the list, which will be sent when rejecting call with message.
			•bt get_message     - To get the messages from connected mobile.
			•bt send_message    - To send the message from the connected mobile.


## Supported Boards
- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/edgefast_bluetooth_examples/shell/example_board_readme.md)

