# V2X Base Test - imx943evk

## Hardware requirements

- i.MX943 EVK board
- USB Type-C cable
- Personal computer with a serial terminal (115200 baud, 8N1)

## Board settings

No special jumper settings are required beyond the default debug console setup.

## System Manager configuration

The running master core must be granted ownership of the V2X message units
used by this example. When the System Manager is built from `mx94alt.cfg`,
these V2X MUs are already owned by the master and no change is needed.

When using a System Manager built from a configuration other than
`mx94alt.cfg`, add the following resources to the master's domain in the
board configuration file (`.cfg`), similar to the RPMsg example:

```
V2X_APP0            OWNER
V2X_DEBUG           OWNER
V2X_HSM1            OWNER
V2X_HSM2            OWNER
V2X_SHE0            OWNER
V2X_SHE1            OWNER
```

## Running the demo

Build for the target core, flash, and observe the serial console. The example
prints the V2X firmware version, TRNG start result, power-state result, and the
number of debug-dump words returned by the enclave.

## Example output

```
*************** EdgeLock V2X base demo *******************

****************** Bring up V2X firmware *****************
V2X firmware is running.

****************** Get V2X FW version ********************
V2X FW version: 1.0.7  commit 0x60a0acab

****************** Start V2X TRNG ************************
V2X TRNG started.

****************** Set V2X power state ON ****************
V2X power state set to ON.

****************** V2X debug dump ************************
V2X debug dump (20 words):
  [ 0] 0x   101ff
  [ 1] 0x       c
  [ 2] 0x   1008a
  [ 3] 0x       4
  [ 4] 0x   10262
  [ 5] 0x17020402
  [ 6] 0x   10263
  [ 7] 0x8b000000
  [ 8] 0x   10263
  [ 9] 0x     340
  [10] 0x   10263
  [11] 0x8b000000
  [12] 0x   10263
  [13] 0x      20
  [14] 0x   101d3
  [15] 0x     2e0
  [16] 0x   101f3
  [17] 0x       0
  [18] 0x   1008a
  [19] 0xe60011e3

*************** EdgeLock V2X base demo END ***************
```
## Building the boot image (imx-mkimage)

These examples can run on the M33S, M70, and M71 cores, so the application image
must be packed into the bootable `flash.bin` together with the V2X dummy
container. Building the image requires an imx-mkimage that appends the V2X dummy
container to the flash targets.

In `iMX94/soc.mak`, add `$(V2X_DUMMY)` immediately before `-out flash.bin` on
the relevant flash targets (`flash_m70`, `flash_m71`, `flash_m33s`, and
`flash_m33s_m70_m71`). For example, the `flash_m33s` target becomes:

```
-m33 $(M33S_IMG) 1 $(M33S_TCM_ADDR) $(M33S_TCM_ADDR_ALIAS) $(V2X_DUMMY) -out flash.bin
```

Note that `$(V2X_DUMMY)` only makes the boot ROM place the V2X firmware image at
its designated address; it does not start the V2X firmware. The V2X firmware is
not running after boot ROM finishes. The application is responsible for bringing
up the V2X firmware at run time (this example performs that bring-up step before
using any V2X service).
