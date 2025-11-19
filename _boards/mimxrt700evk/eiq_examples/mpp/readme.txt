Files origin from sdk v25.06:

├─ cm33_core1
│   ├── app.h
│   │   /* examples/_boards/mimxrt700evk/multicore_examples/rpmsg_lite_pingpong_rtos/cm33_core1/app.h, not modified */
│   ├── hardware_init.changed
│   │   /* examples/_boards/mimxrt700evk/multicore_examples/rpmsg_lite_pingpong_rtos/cm33_core1/hardware_init.c, not modified */
│   ├── pin_mux.c
│   │   /* examples/_boards/mimxrt700evk/multicore_examples/rpmsg_lite_pingpong_rtos/cm33_core1/pin_mux.c, not modified */
│   └── pin_mux.h
│       /* examples/_boards/mimxrt700evk/multicore_examples/rpmsg_lite_pingpong_rtos/cm33_core1/pin_mux.h, not modified */
├─ drivers
│   └── dsp_config.h
│       /* examples/_boards/mimxrt700evk/multicore_examples/dsp_config.h, not modified
│          added only to be able to compile the core0 app when using RPSMG middleware (DSP not used actually) */
├─ inc
│   ├── app.h
│   │   /* created */
│   ├── FreeRTOSConfig.h
│   │   /* boards/mimxrt700evk/freertos_examples/freertos_hello/cm33_core0/FreeRTOSConfig.h and modified to fit
│   │      mpp demo - configTICK_RATE_HZ is changed to 1000 */
│   ├── pin_mux.h
│   │   /* boards/mimxrt700evk/ezhv_examples/flexio_dvp/cm33_core0/pin_mux.h, modified to add pmic init function*/
│   └── fsl_debug_console_conf.h
│       /* boards/mimxrt700evk/edgefast_bluetooth_examples/shell/cm33_core0, not modified */
├─ src
│   ├── hardware_init.c
│   │   /* created to redefine BOARD_Init() for mpp demo */
│   └── pin_mux.c
│        /* boards/mimxrt700evk/ezhv_examples/flexio_dvp/cm33_core0/pin_mux.c, modified to add pmic init function
|           and RX configuration in UART config fuction. */
├─ MIMXRT798Sxxxx_cm33_core0_flash.ld
|    /* devices/RT/RT700/MIMXRT798S/gcc/MIMXRT798Sxxxx_cm33_core0_flash.ld, modified to meet mpp requirements. */
└─ MIMXRT798Sxxxx_cm33_core1_ram.ld
     /* devices/RT/RT700/MIMXRT798S/gcc/MIMXRT798Sxxxx_cm33_core1_ram.ld, modified to meet mpp requirements
        (different RPMSG_SHMEM_START address). */
