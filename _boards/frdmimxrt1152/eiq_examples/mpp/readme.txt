Files origin from sdk v2.15.0:

├── config.cmake
│   /* boards/evkbmimxrt1170/rtos_examples/freertos_hello/cm7/armgcc/config.cmake combined with boards/evkbmimxrt1170/eiq_examples/
|   tflm_label_image/armgcc/config.cmake and modified to add deepviewrt module */
├── flags.cmake
│   /* boards/evkbmimxrt1170/eiq_examples/tflm_label_image/armgcc/flags.cmake, removed XMCD, enabled DCD */
├── inc
│   ├── board_init.h
│   │   /* boards/evkbmimxrt1170/eiq_examples/tflm_label_image/board_init.h, not modified */
│   ├── dcd.h
│   │   /* boards/evkbmimxrt1170/eiq_examples/tflm_label_image/dcd.h, not modified */
│   ├── FreeRTOSConfig.h
│   │   /* boards/evkbmimxrt1170/freertos_examples/freertos_hello/cm7/FreeRTOSConfig.h and modified to fit mpp demo - configTICK_RATE_HZ is changed to 1000 */
│   ├── pin_mux.h
│   │   /* boards/evkbmimxrt1170/eiq_examples/tflm_label_image/pin_mux.h, not modified */
│   └── fsl_debug_console_conf.h
│       /* boards/evkbmimxrt1170/edgefast_bluetooth_examples/shell/cm7, not modified */
├─ src
│   ├── board_init.c
│   │   /* created to redefine BOARD_Init() for mpp demo */
│   ├── dcd.c
│   │   /* boards/evkbmimxrt1170/eiq_examples/tflm_label_image/dcd.c, not modified */
│   └── pin_mux.c
│        /* boards/evkbmimxrt1170/eiq_examples/tflm_label_image/pin_mux.c, not modified */
└── MIMXRT1176xxxxx_cm7_flexspi_nor_sdram.ld
    /* boards/evkbmimxrt1170/devices/MIMXRT1176/gcc/MIMXRT1176xxxxx_cm7_flexspi_nor_sdram.ld, modified
        This file has been modified to support deepviewRT library(a new section .got has been added to m_data region */

