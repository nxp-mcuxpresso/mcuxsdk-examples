# Low-power reference design central application

## 1 - overview
Please see the section overview of the peripheral application readme for the common description [lowpower_peripheral](../lowpower_peripheral/readme.md)

The low-power central reference design application is based on the temperature collector example that acts as a GAP central. In addition, it has the low-power feature enabled and optimized. The user can easily configure features such as Bluetooth scanning parameters, and other parameters related to the application, so that they can customize the application according to their eventual needs.
## 2 - Features
The low-power Central reference design application provides the following low-power features:
- Application core in Deep Sleep mode during scan and connection with selective RAM retention.
- Wake-up from Low Power mode with wake-up source such as Radio, Low-power timers, and IO.

The lp central application supports a Shell to output information from the device on LPUART interface. However, the shell is not source of wake-up when in Low-Power modes.

The Low-power reference design peripheral application supports up to 8 simultaneous connections: 7 lp central and one smartphone, or 8 lp central devices. However, the temperature service only supports one subscriber at a time.

By default, the first connected device is subscribed to temperature service and battery service. Only then, the other devices are subscribed to the battery service. The Low power central reference design is improved to also support battery notifications. Now, Low power central reference design also looks for battery service, enables its notifications and gets notified. It makes it not only a temperature_collector clone, as it can read battery level of the peripheral. No tweaking is needed, this feature is available by default.

## 3 - Running the low-power central reference design application

### Supported boards:
- KW45B41Z-EVK
- FRDM-MCXW71
- KW47-EVK
- MCX-W72-EVK

### Features/Use cases

The application behavior is as follows:

At POR, start scanning immediately, scanning stops in the following cases:
- On SW2 button press. When scanning is stopped, the device enters the Low Power mode.
- On 30 sec timer expiration, then goes to Deep Sleep mode.
- On connection establishment.

It establishes automatically a connection with a low-power reference design application (lp refdes app) or a temperature sensor by checking the temperature sensor service's UUID in the advertising message and retrieves the temperature value. Note that Low Power Reference Design disconnects immediately after temperature is received.

On disconnect, the Application core and radio core go to Deep Sleep mode with full RAM retention. If `gAppRestartScanAfterConnect` is set to 1, the radio core restarts the scan activity. Application core still goes to Deep Sleep mode between messages from CM3. SW2 button has no effect.

Shell over LPUART peripheral outputs scanning, connection information, and temperature value.
### 3.1 - Application configuration
Details on the Application configuration parameters are directly described in the app_preinclude.h file of the project.

Some highlights on the parameters in *app_preinclude.h* that can be changed for this application are given here:
- `gDebugConsoleEnable_d` to 1: Enables PRINTF.
- `gAppUseBonding_d` and `gAppUsePairing_d` to 0: Must be aligned to the same configuration as the peripheral device.
- `gAppUsePrivacy_d` to 0: must be aligned with the peripheral device.
- `gAppExtAdvEnable_d` to 1: enables the capability to scan and print scanned Extended Advertisements (requires OPT host lib).
- `gAppRestartScanAfterConnect` to 1: when connection ends, the device switches back to Scan mode.
- `gPLATFORM_DisableNbuLowpower_d` to 1: prevents Radio domain to go to Low Power mode.
- `gAppLowpowerEnabled_d` to 0: disables Low Power (for sanity test without low power).

### 3.2 - Expected data output on console
```
Copyright  2024  NXP

BLE Lowpower Central>
trying to connect to a lowpower peripheral refdes application
Short press SW2 to start/stop scanning
Short press SW3 to enable/disable low power
Long press SW3 to print a message

Scanning...

Scanning stopped!

Going into lowpower (RAMOFF)
```

Shen connected to a low power peripheral reference design application, following output should be displayed:
```
Copyright  2024  NXP

BLE Lowpower Central>
trying to connect to a lowpower peripheral refdes application
Short press SW2 to start/stop scanning
Short press SW3 to enable/disable low power
Long press SW3 to print a message

Scanning...

Found device:
NXP_TEMP
00:60:37:96:BE:12
Scanning stopped!

Connected!
Temperature: 24 C
Battery: 100 %

Disconnected!

Going into lowpower (RAMOFF)
```

Notes :
- When plugging the USB cable to power ON the board, you may miss the first characters until your terminal is fully connected to your device. Press power ON reset button to have full display.
- When paired and bounded, the message *paired* will no longer be displayed. You may see Privacy's ready instead.
