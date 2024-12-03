# meterlib1phrc_test

## Overview
The Kinetis-M metering library is used in Kinetis-M series chips and mainly used in metering applications for accurate
computation of the active energy, reactive energy, active power, reactive power, apparent power, RMS voltage, and RMS
current.
Normally this metering library is used for three-phase four-wire, two-phase two-wire or single-phase one-wire applications.
This application will guide to use the library in non-standard single-phase multi-channel metering applications. Furthermore,
the implementation described in this document can also be extended to other single-phase non-standard meters.
The Kinetis-M microcontrollers, e.g. KM34Z128 and KM35Z75M, can offer a high-performance analog front-end (24-bit AFE)
combined with an embedded Programmable Gain Amplifier(PGA). Besides high-performance analog peripherals like
auxiliary 16-bit SAR ADC, these devices integrate memory, input-output ports, digital blocks, and a variety of communication
options.
Moreover, on KM35Z75M, the ARM Cortex-M0+ core, with MMAU (Memory Mapped Arithmetic Unit) support for 32-bit and
64-bit math, enables fast execution of metering algorithms.
There are two metering libraries provided with Kinetis-M chips, Filter-Based library and FFT library. This application will use
Filter-Based library.
The Filter-Based Metering Algorithm can be easily integrated into an electronic power meter application. The algorithm
requires only instantaneous voltage and current samples to be provided at constant sampling intervals. These instantaneous
voltage and current samples are usually measured by an AFE with the help of a resistor divider, in the case of a phase voltage
measurement, and a shunt resistor, current transformer or a Rogowski coil in the case of a phase current measurement. All
current measurement sensors introduce a phase shift into current measurement. Therefore, it is necessary to align the phases
of the instantaneous voltage and current samples prior to their use by the Filter-Based Metering Algorithm using either a
digital filter or with the aid of delayed sampling.

This example demonstrates use of the metering library in typical one-phase power meter application with Rogowski coil sensor.

## Supported Boards
- [TWR-KM35Z75M](../../_boards/twrkm35z75m/demo_apps/meterlib1phrc_test/example_board_readme.md)
