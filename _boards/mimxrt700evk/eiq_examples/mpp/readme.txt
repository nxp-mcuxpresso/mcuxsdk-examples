Files origin from sdk v2.15.004:

├── config.cmake
│   /* created based on tflm, freertos, display mipi and vglite config.cmake files. */
├── flags.cmake
│   /* boards/mimxrt700evk/vglite_examples/tiger_freertos/armgcc/flags.cmake, not modified */
├── tflm.cmake
│   /* boards/mimxrt700evk/eiq_examples/tflm_label_image/cm33_core0/armgcc/config.cmake, modified to add freertos */
├── inc
│   ├── board_init.h
│   │   /* created */
│   ├── clock_config.h
│   │   /* boards/mimxrt700evk/vglite_examples/tiger_freertos/clock_config.h, not modified */
│   ├── FreeRTOSConfig.h
│   │   /* boards/mimxrt700evk/freertos_examples/freertos_hello/cm33_core0/FreeRTOSConfig.h and modified to fit
│   │      mpp demo - configTICK_RATE_HZ is changed to 1000 */
│   └── pin_mux.h
│       /* boards/mimxrt700evk/vglite_examples/tiger_freertos/pin_mux.h, not modified */
├─ src
│   ├── board_init.c
│   │   /* created to redefine BOARD_Init() for mpp demo */
│   ├── clock_config.c
│   │   /* boards/mimxrt700evk/vglite_examples/tiger_freertos/clock_config.c, not modified */
│   └── pin_mux.c
│        /* boards/mimxrt700evk/vglite_examples/tiger_freertos/pin_mux.c, not modified */
└── MIMXRT798Sxxxx_cm33_core0_flash.ld
    /* devices/MIMXRT798S/gcc/MIMXRT798Sxxxx_cm33_core0_flash.ld, modified to meet mpp requirements. */

