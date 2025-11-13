Hardware requirements
===================
- USB-C cable
- Network cable RJ45 standard
- FRDM-MCXE247 board
- Personal Computer

Board settings
============
Before running this demo, please study the flash partitioning required
by the ELA_CSEC module. Correct partitioning of the flash is also showcased
in the ela_csec driver example. This partitioning step is NOT done by
the lwIP examples which use mbedTLS and must be completed before running
these examples in order to utilize the underlying ELA_CSEC acceleration.
