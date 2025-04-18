## Overview
This application demonstrates the main use case of the MCXW236.

## Overview
- The health_care_iot_peripheral does not support "Semihosting" debug option, because it includes some custom linker scripts.
Besides, since it uses power management, the semihosting debug option will probably cause a "SWD connection lost" error.
- The BLE Battery Service is only compatible with IoT Toolbox app running on Iphone.
- BLE Time Service is only compatible with IoT Toolbox app running on Iphone.

#### Supported boards:
- FRDM-MCXW32

