Hardware requirements
=====================
- USB Type-C cable
- MCIMX93-EVK board
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
4.  Make sure the FDT file and key bootargs are correctly set before booting the linux kernel. Following is the example commands on A core U-Boot console:
    =>setenv fdtfile imx93-11x11-evk-rpmsg.dtb
    =>setenv mmcargs 'setenv bootargs ${jh_clk} clk-imx93.mcore_booted console=${console} root=${mmcroot} snd_pcm.max_alloc_per_card=134217728'
    =>saveenv
5.  Boot Linux BSP to u-boot, and load M core image from SD card to run. (Put the image into SD card device before)
    => load mmc 1:1 0x80000000 imx93-11x11-evk_m33_TCM_sai_low_power_audio.bin
    => cp.b 0x80000000 0x201e0000 ${filesize}
    => bootaux 0x1ffe0000 0

******************
NOTE
******************
1.  The 16/24/32bits for PCM Music stream are supported.
2.  Since the music files are typically large, users could create a new large size partition in the SD card to place the music files.
3.  Please make sure audio file (such as xxx.wav) in any filesystem that Linux can access.

******************
Playback command
******************
Note:
1. Please use the command "cat /proc/asound/cards" to check the wm8962 sound card number (on A core Linux console).
E.g: Type command:
        ~# cat /proc/asound/cards
     The available sound cards can be shown:

     0 [micfilaudio    ]: micfil-audio - micfil-audio
                          micfil-audio
     1 [wm8962audio    ]: wm8962-audio - wm8962-audio
                          wm8962-audio

Then the wm8962 sound card number is 1.

When playback the .wav file (on A Core Linux console):
1.  If you want to playback with pause/resume command, below command can be used:
      "aplay -Dhw:1 xxx.wav -N";
    press space key on the keyboard to pause, and press the space key again to resume.
2.  93 board supports audio format of S16/S24/S32, and sample rates could be 32kHz, 48kHz and 96kHz, NOT inlcuding 44.1kHz.
3.  If want to playback with low power mode and specified period-size, could use command:
      "aplay -Dhw:1 --buffer-size=xxx --period-size=xxx xxx.wav -N &" or
      "aplay -Dhw:1 --buffer-time=xxx --period-time=xxx xxx.wav -N &".
    E.g: "aplay -Dhw:1 --period-time=500000 --buffer-time=10000000 xxx.wav -N &"
    Now please use "echo mem > /sys/power/state" command to make A core enter suspend mode and the playback work normally.
    Note, make sure the A core has enough time to fill the audio buffer before going into suspend mode.

When recording sound, could use command:
       arecord -Dhw:1,0 -c2 -r48000 -fS32_LE test.wav

Running the demo
================

####################  LOW POWER AUDIO TASK ####################

	Build Time: Apr 26 2023--15:55:05
Start SRTM communication
********************************
 Wait the Linux kernel boot up to create the link between M core and A core.

********************************
Handle Peer Core Linkup
The rpmsg channel between M core and A core created!
********************************


Task A is working now.




