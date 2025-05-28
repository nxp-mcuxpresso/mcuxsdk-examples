# Getting Started

## Hardware Settings
  - For eUSB mode, please disable USB_DEVICE_CONFIG_CHARGER_DETECT macro and USB_DEVICE_CONFIG_DETACH_ENABLE. RT700 eUSB does not support DCD
    feature (device charger detect) and VBUS detection.
  - For eUSB repeater mode, please populate R417 and R418, and remove R409 and R410. This is default setting on the board.
  - For eUSB native mode, please populate R409 and R410, and remove R417 and R418.