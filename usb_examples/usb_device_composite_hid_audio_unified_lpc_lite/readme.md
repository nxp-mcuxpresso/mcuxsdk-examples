# dev_composite_hid_audio_unified_lpc_lite_bm




## Overview

The USB Composite HID and Audio Unified application is a simple demonstration program based on the MCUXpresso SDK. 
It is enumerated as a playback and recording device. Users can record the sound from this device via the "Sound Recorder" in the Windows Accessories and play music with the device.


## System Requirement

### Hardware requirements

- Mini/micro USB cable
- USB A to micro AB cable
- Hardware (Tower System base/module) with a SGTL board (except MAPS-KS22)
- Personal Computer


### Software requirements

- The project files for the lite version example are in: 
<br> <MCUXpresso_SDK_Install>/boards/<board>/usb_examples/usb_device_composite_hid_audio_unified_lite/<rtos>/
<toolchain>.
<br>  The project files for a non-lite version example are in: 
<br> <MCUXpresso_SDK_Install>/boards/<board>/usb_examples/usb_device_composite_hid_audio_unified/<rtos>/<toolchain>.
> The <rtos> is Bare Metal or FreeRTOS OS.


## Getting Started

### Hardware Settings

> Set the hardware jumpers (Tower system/base module) to default settings.


### Prepare the example

1.  Download the program to the target board.
2.  Connect the target board to the external power source (the example is self-powered).
3.  Either press the reset button on your board or launch the debugger in your IDE to begin running
    the demo.
4.  Connect a USB cable between the PC host and the USB device port on the board.

For detailed instructions, see the appropriate board User's Guide.

## Run the example in Windows

1.  Plug in the device which is running composite example into PC. 
2.  For the Audio recorder, a USB AUDIO DEMO device is enumerated in the Device Manager.
3.  Right click on the sound control icon in the Start bar (near the clock) and select "Recording devices".
<br>![Sound control icon](usb_device_composite_hid_audio_right_click_icon.jpg "Sound control icon")
4.  In the opened window, select the "Microphone" device with the description "USB Audio + HID Demo" and click on the "Properties" button.
<br>![Select properties](usb_device_composite_hid_audio_unified_recorder_property.jpg "Select properties")
5.  In the new window, go to "Levels" tab, move the slide until 100%, and click on "OK".
<br>![Change level](usb_device_composite_hid_audio_unified_recorder_change_level.jpg "Change level")
6.  Ensure that "USB Audio + HID Demo" is still selected in the previous window and click on the "Set Default" button. Finally, click on the "OK" button. 
<br>![Set default](usb_device_composite_hid_audio_unified_recorder_default.jpg "Set default")
7.  Plug a line in cable into the line in slot, for example, J9 on SGTL board and connect the other end into PC or mobile phone.
8.  Open the "Sound Recorder" application and record audio.
9.  After recording, open the recorder file with any media player.
10. In the opened window, select the "Speakers" device with the description "USB Audio + HID Demo" and click on the "Properties" button.
<br>![Select properties](usb_device_composite_hid_audio_unified_speaker_property.jpg "Select properties")
11. In the new window, go to "Levels" tab, move the slide until 100%, and click on "OK".
<br>![Change level](usb_device_composite_hid_audio_unified_speaker_change_level.jpg "Change level")
12. Ensure that "USB Audio + HID Demo" is still selected in the previous window and click on the "Set Default" button. Finally, click on the "OK" button. 
<br>![Set default](usb_device_composite_hid_audio_unified_speaker_default.jpg "Set default")
13.  Open the Window Media Player application, select, and play the song. 
14. Use the mute/unmute button shown in the debug console's print information to mute/unmute speaker, it only control the speaker.

note<br>
1.  On some platforms, the recorder may have noise since the audio clock and USB SOF are not synchronized, this problem only happens on Windows but not on 
Mac OSX since the latter uses unified engine.
2.  USB audio class 2.0 is enabled by default.
3.  Based on the USB spec, the feedback endpoint data length should be 3 bytes if the device is full speed. In this case, device can work on Mac OS. However, device can not work on Win 10 and feedback data length must be set 4. There is a workaround to fix this issue, please open the macro
USB_DEVICE_WORKAROUND_AUDIO_20_WINDOWS when meets the following conditions:
<br> - USB device is full speed and USB audio class 2.0 is enabled.
<br> - USB device uses feedback endpoint.
<br> - USB host is Windows 10.
*/



## Supported Boards
- LPCXpresso54S018M
- LPCXpresso54628
- LPCXpresso54S018
