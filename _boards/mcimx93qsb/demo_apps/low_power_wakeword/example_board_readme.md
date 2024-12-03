
Hardware requirements
=====================
- USB Type-C cable
- MCIMX93-QSB board
- J-Link Debug Probe
- 12V~20V power supply
- Personal Computer

Board settings
============
No special settings are required. This demo uses the microphones from the board.

**Note!**
1.  This case does not support ddr and flash target.
2.  This case runs together with Linux and the Linux release version should be not lower than 5.15.X_2.0.0.
3.  wm8962 codec is used for audio playback and PDM microphones present on the board are used for audio recording through MICFIL hardware.

Prepare the Demo
================
1.  Connect 12V~20V power supply and switch SW301 to power on the board.
2.  Connect a USB Type-C cable between the host PC and the J1401 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Boot Linux BSP to u-boot, and load M core image from SD card to run. (Put the image into SD card before.)
    => load mmc 1:1 0x80000000 imx93-9x9-qsb_m33_TCM_low_power_wakeword.bin
    => cp.b 0x80000000 0x201e0000 0x30000
    => bootaux 0x1ffe0000 0
5.  Append "clk-imx93.mcore_booted" and "snd_pcm.max_alloc_per_card=134217728" in u-boot "mmcargs" env, before booting linux.
    => setenv mmcargs 'setenv bootargs ${jh_clk} console=${console} root=${mmcroot} clk-imx93.mcore_booted snd_pcm.max_alloc_per_card=134217728'
6.  Select the appropriate FDT file:
    => setenv fdtfile imx93-9x9-qsb-rpmsg-lpv.dtb
7.  Boot Linux


Running the demo
===============
When the demo boots successfully on M Core, the log on the terminal will look like:

    #################### LOW POWER VOICE DEMO START ####################

    Build Time: XXX XX XXXX--XX:XX:XX 
    ********************************
    Wait the Linux kernel boot up to create the link between M core and A core.

    ********************************
    APP_SRTM_InitAudioService
    Start SRTM communication
    The rpmsg channel between M core and A core created!
    ********************************

    VoiceSeekerLight_Create status: 0x0
    VoiceSeekerLight lib initialized!
    VoiceSpot library version: 0.24.0.1680276948
    VoiceSpot model version: 0.13.1
    VoiceSpot model string: HeyNXP_en-US_1

    Main task is working now.
    Handle Peer Core Linkup


******************
simple usage with audio record/play on Linux
******************


1. In Linux, check that the rpmsg sound cards for wm8962 and micfil have been setup, and note their ID (may vary between reboots)
    root@imx93evk:~# cat /proc/asound/cards
     0 [wm8962audio    ]: wm8962-audio - wm8962-audio
                          wm8962-audio
     1 [micfilaudio    ]: micfil-audio - micfil-audio
                          micfil-audio

2. In this case, micfil has ID: 1. Then start audio recording with command (-c option can be tuned to select number of microphones):
    arecord -Dhw:1,0 --buffer-time=10000000 -r16000 -fS32_LE -c4 test.wav

3. Make Linux on A core enter suspend:
    "echo mem > /sys/power/state"

4. When a wakeword ("Hey NXP") is detected by M core, Linux will resume and audio record will continue on Linux.

5. Following command can be used to playback an audio file:
      "aplay -Dplughw:0,0 xxx.wav"
   Note that playback will be automatically paused when Linux suspends and restarted after Linux has resumed.


******************
usage with NXP-AFE and advanced audio processing on Linux
******************
See NXP-AFE documentation in Linux User's guide to properly configure Linux ALSA paths and enable advanced audio processing applications.
Then Linux can be suspended by "echo mem > /sys/power/state"
It will be resumed when Cortex-M detects the wakeword.
