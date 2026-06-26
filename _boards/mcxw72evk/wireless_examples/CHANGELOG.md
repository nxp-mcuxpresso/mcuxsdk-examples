#### 26.09.00-pvw1
- [debug_nbu] Added Serial Manager port (`board_debug_nbu_serialmgr_port.c`) to route HCI logs over the main application serial port.
- [debug_nbu] Added fault-only HCI logging mode (`enable_hci_log_on_fault_only`): packets are forwarded only after an NBU fatal fault is detected, avoiding continuous logging overhead.
- [debug_nbu] Added Base64-encoded HCI log output (`enable_hci_log_base64`): each packet is emitted as a single ASCII-safe `@<base64>\n` line, allowing HCI logs to be safely interleaved with human-readable console text on the shared serial port.
- [hci_to_btsnoop.py] Added `--base64` mode to decode Base64 line-framed packets from live serial capture.
- [hci_to_btsnoop.py] Added `-i/--input` option to convert a previously saved console log file to BTSNOOP offline (requires `--base64`).

#### 25.09.00-rfp
- [refdes] Enabled NBU debug module to detect NBU faults and extract debug data.
- [refdes] Enabled debug console. Used to dump NBU context/state on NBU fault detection.

#### 25.06.00-pvw2
- [board_platform.h] Please note that the feature to calibrate XTAL32M with temperature by adding 'gBoardUseXtal32MTempComp' is currently not ready for implementation. We have observed issues when this feature is enabled
- [refdes] increase Timer manager stack `TM_TASK_STACK_SIZE' from 324 to 350 to avoid overflow on refdes applications
- [board_utility.c] Rework and remove unnecessary ADC APIs

#### 24.12.00 final release
- [refdes] : Change gAppLowPowerConstraintInNoBleActivity_c default value from deep sleep to deep power down (2 to 4) 

#### 24.12.00-pvw1
- update macro default values : BOARD_32MHZ_XTAL_CDAC_VALUE to 10, BOARD_32MHZ_XTAL_ISEL_VALUE to 11