# pit_scout

## Overview
The pit_scout project is a simple demonstration program of the SDK PIT driver for synchronous output. It sets the PIT hardware block to a periodic rate of 1 second. Synchronous output when PIT counter equals the modolus value. Synchronous output is set to toggle mode. After reset, SYNC_OUT is low-level, when PIT counter equals MODULO value firstly, SYNC_OUT toggles to high-level and keep high-level until PIT counter equal MODULO value next time, SYNC_OUT toggles to low-level.Use the Oscilloscope to measure the signal of XBARA_OUT10, the sync_out waveform will be outputted.

## Supported Boards
