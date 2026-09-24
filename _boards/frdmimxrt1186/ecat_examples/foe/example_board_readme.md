# ecat_foe on FRDM-IMXRT1186

## 1. Hardware requirements

| Item | Note |
|------|------|
| FRDM-IMXRT1186 A2 | Target board |
| RJ45 network cable | Board EtherCAT Port0 to the host PC |
| Type-C USB cable | Debug console and flashing |
| Host PC with TwinCAT 3 | Windows 10 recommended |
| MCUXpresso Secure Provisioning Tool (SPT) | Image signing and flashing |

## 2. Board settings

| Jumpers | Setting | Function |
|---------|---------|----------|
| J12, J13 | 2-3 | eCAT0 routed to J57A (default) |
| J17, J18 | 2-3 | eCAT1 routed to J57B (default) |

Boot mode settings used in this document (J60, pins 1-2-3 from left to right):

| J60[1,2,3] | Purpose |
|------------|---------|
| 100 | Serial download mode (used to program with SPT) |
| 001 | Normal boot from flash (used to run the demo) |

## 3. EtherCAT and console connections

| Connector | Function |
|-----------|----------|
| J57A | EtherCAT Port0 - connect this one to the TwinCAT PC |
| J57B | EtherCAT Port1 |
| OpenSDA USB | Debug console and flashing |

Serial terminal settings:

| Parameter | Value |
|-----------|-------|
| Baud rate | 115200 |
| Data bits | 8 |
| Parity | None |
| Stop bits | 1 |
| Flow control | None |

## 4. MCUboot memory layout

| Region | From | To | Size |
|--------|------|----|------|
| MCUboot code | 0x04000000 | 0x0403FFFF | 256 kB |
| Primary slot | 0x04040000 | 0x0423FFFF | 2048 kB |
| Secondary slot | 0x04240000 | 0x0443FFFF | 2048 kB |

| MCUboot parameter | Value |
|-------------------|-------|
| Header size | 1024 bytes |
| Signing algorithm | ECDSA-P256 |
| Write alignment | 4 bytes |
| Image handling strategy | `SWAP_MOVE` |

## 5. Optional: EEPROM LPI2C emulator

FRDM-IMXRT1186 can emulate the ESC EEPROM in software. **Skip this chapter if you use the
on board EEPROM hardware.**

1. Rework the board:

   | Action | Components |
   |--------|-----------|
   | Remove jumper | J31 pin1-pin2, J35 pin1-pin2 |
   | Remove resistor | R297 pin1-pin2, R299 pin1-pin2 |
   | Add resistor | R30, R31, R297 pin1-pin3, R299 pin1-pin3 |

2. Wiring:

   | From | To |
   |------|----|
   | J31 pin 1 | J2 pin 18 |
   | J35 pin 1 | J2 pin 20 |

3. Define the `EEPROM_I2C_EMULATOR` macro in the project to enable the emulator.
4. Continue with [6. Prepare the demo](#6-prepare-the-demo).

## 6. Prepare the demo

### 6.1 Generate the SSC source code

`<SSC_DIR>` = `./boards/frdmimxrt1186/ecat_examples/foe/<core>/SSC`

1. Download the Slave Stack Code Tool (SSC Tool V5.13) from the Beckhoff website and install it.
2. Open the SSC tool.
3. `File` > `New` > `Import`, then select `<SSC_DIR>/foe.xml`.
4. `Custom` > `NXP ECAT...` > `OK` to select the SubDevice description file.
5. `Tool` > `Application` > `Import`, then select `<SSC_DIR>/foe.xlsx`.
6. `Project` > `Create new Slave Files`, keep the default `Source Folder`
   (`<SSC_DIR>/Src`), then click `Start`.
7. Delete `foe.c` from the `Src` folder.

### 6.2 Connect the hardware

1. Connect J57A (EtherCAT Port0) to the TwinCAT 3 PC with the RJ45 cable.
2. Connect the OpenSDA USB port to the host PC and open the serial terminal
   (see [3. EtherCAT and console connections](#3-ethercat-and-console-connections)).

### 6.3 Build the images

| Project | Path |
|---------|------|
| MCUboot | `boards/frdmimxrt1186/ota_examples/mcuboot_opensource/cm33` |
| FoE example | `boards/frdmimxrt1186/ecat_examples/foe/cm33` |

### 6.4 Sign and flash with the SPT

1. Download the MCUXpresso Secure Provisioning Tool from the NXP website.
2. Set J60[1,2,3] to `100` to enter serial download mode, then reset the board.
3. Open the SPT, create a new workspace for the processor and connect the board over UART.
4. Check the boot memory in `Target` > `Boot Memory`.
5. In the Build image view select `mcuboot_opensource_cm33.elf` as `Source executable image`.
6. Open `Tools` > `MCUboot` > `Sign Image` and configure:
   - Image to be signed: `ecat_foe.bin`
   - Signing key: `middleware/mcuboot_opensource/boot/nxp_mcux_sdk/keys/sign-ecdsa-p256-priv.pem`
     (or the copy next to the prebuilt application)
   - `imgtool` arguments:

     | Argument | Value |
     |----------|-------|
     | version | 1.0 |
     | align | 4 |
     | header-size | 0x400 |
     | pad-header | (empty) |
     | slot-size | 0x200000 |
     | max-sectors | 800 |
     | pad | (empty) |
     | confirm | (empty) |

   - Click `Sign`, then set the target address to `0x04040000`.
   - Close the dialog with `Save & Close`.
7. On the Build tab, verify that `Build script hooks` contains the pre-build script.
8. In `Additional Images`, verify that the signed application is configured as `Image 1`.
9. Build and write the bootable image into the processor.

> See the "MCUboot workflow" chapter of `MCUXSPTUG.pdf` on the SPT documentation page for
> more details.

### 6.5 Run the FoE example

Set J60[1,2,3] to `001` and reset the board. The serial console prints:

```
hello sbl.
Bootloader Version 1.9.0
Primary image: magic=good, swap_type=0x1, copy_done=0x3, image_ok=0x1
Secondary image: magic=unset, swap_type=0x1, copy_done=0x3, image_ok=0x3
Boot source: primary slot
Swap type: none
Bootloader chainload address offset: 0x40000
Reset_Handler address offset: 0x40400
Jumping to the image

Start the SSC FoE example...
Image version: 2.3.0
```

### 6.6 Write the ESI file to EEPROM

1. Copy the ESI file `ECAT-FOE.xml` generated by the SSC tool to
   `<TwinCAT_installation_folder>/<twincat_version>/Config/io/EtherCAT/`.
2. Restart the TwinCAT 3 IDE.
3. Scan the device and perform `Update EEPROM`.

### 6.7 Create the PLC project

Scan and load the device in TwinCAT 3, then create a PLC project with this MAIN program:

```
PROGRAM MAIN
VAR
    Delay : TON;
    LedIn AT %I* : BOOL;
    LedOut AT %Q* : BOOL;
END_VAR

Delay(IN := TRUE, PT := T#1S);
IF Delay.Q THEN
    LedOut := NOT LedIn;
    Delay(IN := FALSE);
END_IF;
```

Then click `TwinCAT` > `Restart TwinCAT (Config Mode)` to activate the configuration.

## 7. Upgrade the firmware over FoE

### 7.1 Sign the new image

1. Build the new FoE image.
2. Open the SPT and create a new workspace, then `Tools` > `MCUboot` > `Sign Image`.
3. Add the FoE image as `Input Image` and select the signing key
   `middleware/mcuboot_opensource/boot/nxp_mcux_sdk/keys/sign-ecdsa-p256-priv.pem`.
4. Set the `imgtool` arguments:

   | Argument | Value |
   |----------|-------|
   | version | 1.1 (must be higher than the installed version) |
   | align | 4 |
   | header-size | 0x400 |
   | pad-header | (empty) |
   | slot-size | 0x200000 |
   | max-sectors | 800 |
   | pad | remove this argument |
   | confirm | remove this argument |

5. Click `Sign`.

### 7.2 Download the image over FoE

1. Open the TwinCAT 3 project.
2. Click `Device` > `Box1` > `Online` > `Bootstrap`.
3. Click `Download`, select `ecat_foe_cm33_signed.bin`, enter the password `12369874`
   and confirm with `Ok`. The serial console prints:

```
Firmware upgrade in progress...
FoE_StoreImage: processed 116 bytes
FoE_StoreImage: processed 232 bytes
...
FoE_StoreImage: upload complete (42028 bytes)
write magic number offset = 0x43ff00
Update image success
```

4. Click `Init` to restart the board.

### 7.3 Read the image back (optional)

Click `Upload`, select `ecat_foe_cm33_signed.bin` and confirm with `Ok`.

## 8. Notes

* Every subsequent upgrade needs a version number higher than the installed one; repeat
  [7.1 Sign the new image](#71-sign-the-new-image) to produce it.
* For more details about the bootloader, refer to the `mcuboot_opensource` example.
