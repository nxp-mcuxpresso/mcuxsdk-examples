Hardware requirements
=====================
- USB Type-C cable
- IMX95 verdin board
- J-Link Debug Probe
- 12V~20V power supply
- Personal Computer
- Headphone

Board settings
============
No special settings are required.

**Note!**
1.  This case does not support ddr and flash target.

Prepare the Demo
================
1.  Connect 12V~20V power supply and J-Link Debug Probe to the board, switch SW301 to power on the board.
2.  Connect a USB Type-C cable between the host PC and the J1401 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Load flash.bin file with sm as alt config before starting the demo to ensure that the permissions of M7 involved in the demo are allowed.
    Build in imx-mkimage repo: make SOC=iMX95 flash_a55 LPDDR_TYPE=lpddr5 OEI=YES MSEL=1.
4.  Make sure the FDT file and key bootargs are correctly set before booting the linux kernel. Following is the example commands on A core U-Boot console:
    =>setenv fdtfile imx95-19x19-verdin-rpmsg.dtb
    =>setenv mmcargs 'setenv bootargs ${jh_clk} console=${console} root=${mmcroot} snd_pcm.max_alloc_per_card=134217728 pd_ignore_unused clk_ignore_unused'
    =>saveenv
5.  Boot Linux BSP to u-boot, and load M core image from SD card to run. (Put the image into SD card device before)
    load mmc 1:1 0x90400000 imx95-verdin-evk_m7_TCM_sai_low_power_audio.bin; cp.b 0x90400000 0x203c0000 0x40000; bootaux 0x00000000 1;
******************
NOTE
******************
1.  The 16/24/32bits for PCM Music stream are supported.
2.  Since the music files are typically large, users could create a new large size partition in the SD card to place the music files.
3.  Please make sure audio file (such as xxx.wav) in any filesystem that Linux can access.
4.  Please disable usb before Acore enter suspend mode used below command in linux console, Otherwise A55 enter suspend fail in verdin board:
    4.1 cd /sys/bus/platform/drivers/ci_hdrc/
    4.2 echo ci_hdrc.0 > unbind
    4.3 cd /sys/bus/platform/drivers/imx_usb/
    4.4 echo 4c200000.usb > unbind

******************
Playback command
******************
Note:
1. Please use the command "cat /proc/asound/cards" to check the wm8904 sound card number (on A core Linux console).
E.g: Type command:
        ~# cat /proc/asound/cards
     The available sound cards can be shown:
      0 [wm8904audio    ]: wm8904-audio - wm8904-audio
                           wm8904-audio
      1 [micfilaudio    ]: micfil-audio - micfil-audio
                           micfil-audio
	
Then the wm8904 sound card number is 0.

When playback the .wav file (on A Core Linux console):
1.  If you want to playback with pause/resume command, below command can be used:
      "aplay -Dhw:wm8904audio xxx.wav -N";
    press space key on the keyboard to pause, and press the space key again to resume.
2.  95 board supports audio format of S16/S24/S32, and sample rates could be 32kHz, 48kHz, 96kHz and 44.1kHz.
3.  If want to playback with low power mode and specified period-size, could use command:
      "aplay -Dhw:wm8904audio --buffer-size=xxx --period-size=xxx xxx.wav -N &" or
      "aplay -Dhw:wm8904audio --buffer-time=xxx --period-time=xxx xxx.wav -N &".
    E.g: "aplay -Dhw:0 --period-time=500000 --buffer-time=10000000 xxx.wav -N &"
    Now please use "echo mem > /sys/power/state" command to make A core enter suspend mode and the playback work normally.
    Note, make sure the A core has enough time to fill the audio buffer before going into suspend mode.
4. When recording sound, need set sound card route as difference between software setting and actual hardware connection:
       amixer  -c wm8904audio cset name='Left Capture Inverting Mux' 'IN2L'
  then could use command to record audio by codec into wav file:
       arecord -Dhw:0,0 -c2 -r48000 -fS32_LE test.wav
5. you can use below command to run record and play at the same time.
       arecord -Dhw:wm8904audio -r16000 -c2 -fS32_LE -traw | aplay -Dplughw:wm8904audio -r16000 --period-size=4096 -c2 -fS32_LE -traw

Running the demo

####################  LOW POWER AUDIO TASK ####################

    Build Time: Apr 19 2024--15:11:38
Start SRTM communication
********************************
 Wait the Linux kernel boot up to create the link between M core and A core.

********************************
Handle Peer Core Linkup
The rpmsg channel between M core and A core created!
********************************


Task A is working now.


