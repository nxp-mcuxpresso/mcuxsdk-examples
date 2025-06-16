# AUDMIX with SAI Codec Example

## Overview
This example demonstrates how to use the AUDMIX (Audio Mixer) peripheral with SAI interfaces to mix audio streams. The AUDMIX peripheral is designed to mix two audio streams, each with up to 8 audio channels, producing a mixed output audio stream.

## Description
The example initializes three SAI peripherals:
- SAI1: Configured as TDM1 input to AUDMIX
- SAI2: Configured as TDM2 input to AUDMIX
- SAI3: Configured to receive the AUDMIX output

After initialization, the example checks if the TDM streams from SAI1 and SAI2 are synchronized (both clock frequency and frame rate). If synchronized, it enables mixed output mode and demonstrates attenuation control. If not synchronized, it falls back to using only TDM1 as the output source.

The example demonstrates:
1. Initialization and configuration of the AUDMIX peripheral
2. Checking synchronization between TDM streams
3. Configuring mixed output when streams are synchronized
4. Applying and controlling attenuation profiles
5. Falling back to single stream output when synchronization fails

## Hardware Requirements
- An NXP development board with AUDMIX and SAI peripherals
- Audio codec connected to the SAI interfaces
- Audio output device (headphones or speakers)

## Software Requirements
- MCUXpresso SDK
- Development environment (MCUXpresso IDE, IAR, Keil, etc.)

## Setup
1. Connect the development board to your computer
2. Build and flash the example to your board
3. Connect an audio output device to hear the mixed audio

## Running the Example
1. After flashing the example, open a terminal to view debug messages
2. The example will initialize the SAI and AUDMIX peripherals
3. It will check if TDM streams are synchronized and report the status
4. If synchronized, it will enable mixed output and demonstrate attenuation
5. If not synchronized, it will use only TDM1 as the output source
6. The example will report completion when finished

## Expected Output
```
AUDMIX example started!
TDM clock match status: Matched
TDM frame rate match status: Matched
TDM streams are synchronized, enabling mixed output.
AUDMIX example completed!
```

Or if streams are not synchronized:
```
AUDMIX example started!
TDM clock match status: Mismatched
TDM frame rate match status: Mismatched
TDM streams are not synchronized, using TDM1 output only.
AUDMIX example completed!
```

## Troubleshooting
- If TDM streams are not synchronized, check the SAI configuration to ensure both SAI1 and SAI2 are configured with the same sample rate, bit width, and channel count.
- If no audio is heard, verify the audio codec connections and configuration.
- Check the debug console output for status information.

## Customization
You can modify the example to:
- Change the audio sample rate by adjusting the `sampleRate_Hz` parameter
- Modify the attenuation profile by changing the attenuation configuration parameters
- Experiment with different output width settings (16/18/20/24-bit)
- Try different attenuation directions and timing

## Limitations
- The example assumes that audio data is being provided to the SAI interfaces
- Both TDM streams must have the same audio sample rate and number of channels for mixing to work properly
