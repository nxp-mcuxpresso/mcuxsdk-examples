# uart_wifi_bridge

## Overview

uart_wifi_bridge

## Supported Boards
- [MIMXRT1060-EVKB](../../_boards/evkbmimxrt1060/wifi_examples/common/wifi_examples_readme.md)
- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/wifi_examples/common/wifi_examples_readme.md)
- [MIMXRT1060-EVKC](../../_boards/evkcmimxrt1060/wifi_examples/common/wifi_examples_readme.md)
- [FRDM-RW612](../../_boards/frdmrw612/wifi_examples/common/wifi_examples_readme.md)
- [RD-RW612-BGA](../../_boards/rdrw612bga/wifi_examples/common/wifi_examples_readme.md)

## FW download support monolithic
- The compilation of uart_wifi_bridge is enabled monolithic by default.
- How to use release mfg FW with non-monolithic configuration:
    - Change the monolithic macro from cmake
      For [RD-RW612-BGA] board mcuxsdk/examples/_boards/rdrw612bga/wifi_examples/uart_wifi_bridge/reconfig.cmake
      For [FRDM-RW612] board mcuxsdk/examples/_boards/rdrw612bga/wifi_examples/uart_wifi_bridge/reconfig.cmake
      Set CONFIG_MONOLITHIC_WIFI from 1 to 0.
    - Then uart_wifi_bridge build will not link FW, need to load FW separatly.
    
- How to use release mfg FW with monolithic configuration:    
    - Covert FW from .bin to .inc
      Copy the FW to mcuxsdk/components/conn_fwloader/fw_bin/
      Rename the FW:
          rw61x_sb_ble_15d4_combo_a2.bin for CPU2 combo FW,
          rw61x_sb_ble_a2.bin for CPU2 single ble FW,
          rw61x_sb_wifi_a2.bin for CPU1 wifi FW
      Use mcuxsdk/components/conn_fwloader/fw_bin/script/wlan_gen_fw_incs.py to convert FW to inc files
          Under mcuxsdk/components/conn_fwloader/fw_bin folder: 
          Run python script "python script/wlan_gen_fw_incs.py"
          The inc files will generate at mcuxsdk/components/conn_fwloader/fw_bin/inc and replace the origin inc files automatically.
    - Compile uart_wifi_bridge with enabled monolithc macro by default.
      Then uart_wifi_bridge build and link FW to a monolithic image, no need to load FW separatly.