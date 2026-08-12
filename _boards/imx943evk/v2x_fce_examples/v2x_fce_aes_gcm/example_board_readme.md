## Board-specific notes (i.MX943 EVK)

- The V2X/FCE (V2X-FH) message unit is not represented by a fixed SDK
  base-address macro on i.MX943. This board port defines `EXAMPLE_V2X_FCE_MU_BASE`
  in `app.h` to `0x47320000`, matching the Linux device-tree node `v2x_mu2`.
- The V2X/FCE MU must be assigned to the running core in the XRDC / System
  Manager configuration for the session to open successfully.
- The EdgeLock enclave firmware is loaded and authenticated by ROM / System
  Manager at boot; the demo does not load any FW image.

## Expected output

The full serial log produced by the demo on the i.MX943 EVK is:

```
*************** EdgeLock V2X/FCE AES-GCM demo **********

****************** Bring up V2X firmware *****************
V2X firmware is running.

****************** Get V2X FW version ********************
V2X FW version: 1.0.7  commit 0x60a0acab

****************** Ping V2X/FCE service ****************
Ping V2X/FCE service successfully.
****************** Open V2X/FCE service ****************
Open V2X/FCE service successfully.

****************** Get V2X/FCE info *******************
V2X/FCE info:
V2X/FCE API version : 1.0.0
Number of FIFO entries : 256

****************** Load AES-256 plain key ****************
Loaded AES-256 key into slot 0.
Loaded AES-256 key into slot 1.
Loaded AES-256 key into slot 2.
Loaded AES-256 key into slot 3.
Loaded AES-256 key into slot 4.
Loaded AES-256 key into slot 5.
Loaded AES-256 key into slot 6.
Loaded AES-256 key into slot 7.

****************** AES-GCM encrypt ***********************
Ciphertext:
8995ae2e6df3dbf96fac7b7137bae67f
Tag:
eca5aa77d51d4a0a14d9c51e1da474ab
Ciphertext and tag match the NIST KAT vector.

****************** AES-GCM decrypt ***********************
Decrypted plaintext:
2d71bcfa914e4ac045b2aa60955fad24
Decrypted plaintext matches the original.

****************** Close V2X/FCE service ***************
Close V2X/FCE service successfully.

*************** EdgeLock V2X/FCE demo END **************
```

## System Manager configuration

For the running core (M33 / master) to open the V2X/FCE service and for the
eDMA engine inside the V2X enclave to reach the DDR data buffers, the System
Manager (SM) must grant the master access to the V2X message units and expose
the DDR region used by the demo.

- If the System Manager image is built from `mx94alt.cfg`, no changes are
  required: that configuration already assigns the V2X MUs and the DDR region
  to the AP domain.
- If the System Manager image is built from any non-`mx94alt.cfg`
  configuration (for example `mx94evkrpmsg.cfg` or `mx94evk.cfg`), the
  configuration must be modified as shown below before rebuilding SM.

Using `mx94evkrpmsg.cfg` as an example, apply the following changes
(`git diff configs/other/mx94evkrpmsg.cfg`):

1. Grant the master (OWNER) access to the V2X message units so it can open the
   V2X/FCE service. Add these entries to the OWNER block:

   ```
   V2X_APP0            OWNER
   V2X_DEBUG           OWNER
   V2X_HSM1            OWNER
   V2X_HSM2            OWNER
   V2X_SHE0            OWNER
   V2X_SHE1            OWNER
   ```

2. Allow the eDMA inside the V2X enclave to access the 0x86000000 DDR region.
   In the `AP-NS` section add:

   ```
   DDR                 EXEC, begin=0x086000000, end=0x089FFFFFF
   ```

After editing the SM configuration, rebuild the System Manager image and
reflash it so the changes take effect.
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
