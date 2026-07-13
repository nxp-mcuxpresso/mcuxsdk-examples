## Overview
This application implements the peripheral role. It:
1. Advertises using a connectable advertiser
2. Enables temperature service
3. Records a new temperature reading every second
4. Updates the temperature in the temperature service any time a new temperature is recorded

## Running the demo
If you paired to another phone before, first perform a full flash erase and reprogram the application.
The HCIOT_PERIPH device must be forgotten/unpaired on the smartphone after reflashing the application, since the bonding passkey is lost on the MCXW23.

1.There are two possible modes for the MCXW23 at startup:
	1.A. Upon the first startup, the MCXW23 automatically enters shelf-mode.
	1.A.1.the expected output is shown below:
		Using bdAddr from UUID
		bd_addr = ed:41:cf:0f:d9:ce
		Eventstore content:
		Last recorded device state: 0
		Flash erase done
		Entering device state 1
		Flash write done

	1.A.2.Press WAKEUP button to enter active state and start advertising.
		The expected output is similar to the following:
		Using bdAddr from UUID
		bd_addr = ed:41:cf:0f:d9:ce
		Eventstore content:
		0s: Device state changed to 1
		0s: Device state changed to 2
		0s: Radio state changed to advertising
		Last recorded device state: 2
		Entering device state 2
		Start ADV
		Flash erase done
		Flash write done
		Measurement initiated
		Measurement started
		Measurement done @ 5: 21
	1.B.1.If the application has already run, The MCXW23 automatically starts advertising so no need to press the WAKEUP button.
	The expected output is similar to the following:
		Using bdAddr from UUID
		bd_addr = 7f:6d:8b:36:fc:ef
		Eventstore content:
		1s: Device state changed to 1
		0s: Device state changed to 2
		0s: Radio state changed to advertising
		Last recorded device state: 2
		Entering device state 2
		Start ADV
		Flash erase done
		Flash write done
		Measurement initiated
		Measurement done @ 7: 27
		Measurement initiated
		Measurement done @ 8: 27
		Measurement initiated
		Measurement done @ 9: 27
		
2. Open NXP IoT toolbox on your smartphone.
3. Click on "Thermometer"
4. There are two possible scenarios: the health_care_iot_peripheral bonding feature is either enabled or disabled:

	4.A. Bonding Disabled:
    	4.A.1. The app lists a device called "HCIOT_PERIPH" marked as 'Unbonded'. Click on it.
    	4.A.2. The Thermometer window is opened. Thermometer and Sensor Location both show '---'
    	4.A.3. wait for a moment to simulate a temperature measurement. A measurement for the ambient temperature value is shown on the smartphone with name of the sensor location.
	
	4.B. Bonding Enabled:
	There are two possible scenarios when the bonding feature is enabled:
		4.B.A The health_care_iot_peripheral is not paired yet (first time):
			4.B.A.1. The app lists a device called "HCIOT_PERIPH" marked as 'Unbonded'. Click on it.
			4.B.A.2. The Thermometer window is opened. The status is Disconnected. Thermometer and Sensor Location both show '---'
			4.B.A.3. Click on 'Connect'.
			4.B.A.4. A pop-up called 'Bluetooth pairing request' is opened to enter the passkey for bonding, Enter 999999.
			4.B.A.5. Wait for a moment to simulate a temperature measurement. A measurement for the ambient temperature value is shown on the smartphone with name of the sensor location.

		4.B.B The health_care_iot_peripheral is already paired :
			4.B.B.1. The app lists a device called "HCIOT_PERIPH" marked as 'Bonded'. Click on it.
			4.B.B.2. The Thermometer window is opened. The status is Connected. Thermometer and Sensor Location both show '---'
        	4.B.B.3. Wait for a moment to simulate a temperature measurement. A measurement for the ambient temperature value is shown on the smartphone with name of the sensor location.

## DC-DC Bypass in Low Power

To enable HV_SM mode with DC-DC bypass during low power states, define:

```c
#define SUPPLY_MODE_DCDCBYPASS 1
```

This configuration allows the DC-DC converter to be bypassed when entering low-power states, which can provide additional power savings depending on the application requirements

## Power Profiling Mode

The application supports a low-power profiling mode designed to minimize power consumption

### Configuration

To enable power profiling mode, set the following define:

```c
#define BLE_POWER_PROFILING 1
```

Please note that this mode disables the watchdog, Brown-out Detection (BOD), memory profiling, the battery measurement task, and the temperature measurement task.

#### Supported boards:
- FRDM-MCXW23
- MCXW23-EVK

## Supported Boards
- [MCXW23-EVK](../../../_boards/mcxw23evk/wireless_examples/reference_design/health_care_iot_peripheral/example_board_readme.md)
- [FRDM-MCXW23](../../../_boards/frdmmcxw23/wireless_examples/reference_design/health_care_iot_peripheral/example_board_readme.md)