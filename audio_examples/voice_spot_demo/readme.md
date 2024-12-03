# voice_spot_demo

## Overview
The voice_spot_demo application demonstrates Voice Spot initialization and processing.
It gets the audio data from the on-board microphone, looks for a wake word (Hey NXP) and outputs
the audio data to the speaker.

## Running the demo
When the example runs successfully, you should see similar output on the serial terminal as below:

VoiceSpot example started!
rdspVoiceSpot_CreateControl: voicespot_status = 0
rdspVoiceSpot_CreateInstance: voicespot_status = 0
rdspVoiceSpot_OpenInstance: voicespot_status = 0
rdspVoiceSpot_EnableAdaptiveThreshold: voicespot_status = 0
VoiceSpot library version: 0.22.2.0
VoiceSpot model version: 0.13.1
VoiceSpot model string: HeyNXP_en-US_1
Voice spot init done.
Trigger event found: Event = 0, Frame = 13488, class_string = HeyNXP, Score = 1024, trigger_sample = 2697800, estimation_sample = 2697800, start_offset_samples = 12012, start_sample = 2685788, stop_offset_samples = 3212, stop_sample = 2694588

## Supported Boards
- [EVK-MIMX8ULP](../../_boards/evkmimx8ulp/audio_examples/voice_spot_demo/example_board_readme.md)
