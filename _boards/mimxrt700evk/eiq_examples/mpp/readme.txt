Files origin from sdk v25.06:

├── inc
│   ├── app.h
│   │   /* created */
│   ├── FreeRTOSConfig.h
│   │   /* boards/mimxrt700evk/freertos_examples/freertos_hello/cm33_core0/FreeRTOSConfig.h and modified to fit
│   │      mpp demo - configTICK_RATE_HZ is changed to 1000 */
│   └── pin_mux.h
│       /* boards/mimxrt700evk/vglite_examples/tiger_freertos/pin_mux.h, not modified */
├─ src
│   ├── hardware_init.c
│   │   /* created to redefine BOARD_Init() for mpp demo */
│   └── pin_mux.c
│        /* boards/mimxrt700evk/vglite_examples/tiger_freertos/pin_mux.c, not modified */
└── MIMXRT798Sxxxx_cm33_core0_flash.ld
    /* devices/MIMXRT798S/gcc/MIMXRT798Sxxxx_cm33_core0_flash.ld, modified to meet mpp requirements. */

