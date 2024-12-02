# EdgeLock 2GO Agent (S)

This demo demonstrates how to use the EdgeLock 2GO service for provisioning keys and certificates into the MCU device.
Those keys and certificates can then be used to establish mutual-authenticated TLS connections to cloud services such as AWS or Azure.

The workspace structure (when building this order must be respected):
- el2go_agent_s: project running in Secure processing environment (SPE)
- el2go_agent_ns: project running in Non-secure processing environment (NSPE)

Details on building and running the application are included in the
[el2go_agent_ns readme](../el2go_agent_ns/readme.md).

## Hardware requirements

- FRDM-RW612 or RD-RW61X-BGA board
- USB-C (FRDM-RW612) or Micro-USB (RD-RW61X-BGA) cable
- Personal Computer

## Board settings

No special settings are required.

## Supported Boards
