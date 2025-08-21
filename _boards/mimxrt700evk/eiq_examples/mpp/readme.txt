Files origin from sdk v25.06:

├── inc
│   ├── app.h
│   │   /* created */
│   ├── FreeRTOSConfig.h
│   │   /* boards/mimxrt700evk/freertos_examples/freertos_hello/cm33_core0/FreeRTOSConfig.h and modified to fit
│   │      mpp demo - configTICK_RATE_HZ is changed to 1000 */
│   └── pin_mux.h
│       /* boards/mimxrt700evk/ezhv_examples/flexio_dvp/cm33_core0/pin_mux.h, modified to add pmic init function*/
├─ src
│   ├── hardware_init.c
│   │   /* created to redefine BOARD_Init() for mpp demo */
│   └── pin_mux.c
│        /* boards/mimxrt700evk/ezhv_examples/flexio_dvp/cm33_core0/pin_mux.c, modified to add pmic init function */
└── MIMXRT798Sxxxx_cm33_core0_flash.ld
    /* devices/MIMXRT798S/gcc/MIMXRT798Sxxxx_cm33_core0_flash.ld, modified to meet mpp requirements. */

