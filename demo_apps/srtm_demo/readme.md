# srtm_demo

## Overview

The srtm_demo demo application demonstrates the use of SRTM service provided in KSDK. The demo
works together with Linux kernel, providing virtual i2c service to it. Linux will utilize SRTM
APIs to configure the shared I2C. The commands are transferred via RPMSG to M core, then based on
service protocols, M core will handle the configuration to CODEC hardware.

The demo also demonstrate support for multiple partition reset.
