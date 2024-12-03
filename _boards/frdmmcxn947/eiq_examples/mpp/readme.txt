Files origin from mcx-n10 sdk v2.14.0:

├── config.cmake
│   /* boards/frdmmcxn947/eiq_examples/tflm_label_image/cm33_core0/armgcc/config.cmake
|      and freertos-kernel modules */
├── flags.cmake
│   /* boards/frdmmcxn947/eiq_examples/tflm_label_image/cm33_core0/armgcc/flags.cmake concatenated with */
|   /* boards/frdmmcxn947/rtos_examples/freertos_hello/cm33_core0/armgcc/flags.cmake */
├── inc
│   ├── board.h
│   │   /* boards/frdmmcxn947/eiq_examples/tflm_label_image/cm33_core0/board.h modified by adding flexio, DMA and LCD configurations. */
│   ├── board_init.h
│   │   /* boards/frdmmcxn947/eiq_examples/tflm_label_image/cm33_core0/board_init.h, not modified */
│   ├── clock_config.h
│   │   /* boards/frdmmcxn947/eiq_examples/tflm_label_image/cm33_core0/clock_config.h, not modified */
│   ├── utick_config.h
│   │   /* Added */
│   ├── FreeRTOSConfig.h
│   │   /* boards/frdmmcxn947/rtos_examples/freertos_hello/cm33_core0/FreeRTOSConfig.h modified to fit mpp demo - configTICK_RATE_HZ is changed to 1000, configurable heap size and set 
│   │      configMAX_PRIORITIESto 6 */
│   └── pin_mux.h
│   |    /* boards/frdmmcxn947/eiq_examples/tflm_label_image/cm33_core0/pin_mux.h modified for camera pins and to add flexio LCD pins config from  
|   |       boards/mcxn9xxbrk/driver_examples/flexio/mculcd/edma_transfer/cm33_core0/pin_mux.h */
├─ src
│   ├── board.c
│   │   /* boards/frdmmcxn947/eiq_examples/tflm_label_image/cm33_core0/board.c modified to fix I2C SCCB for camera */
│   ├── board_init.c
│   │   /* boards/frdmmcxn947/eiq_examples/tflm_label_image/cm33_core0/board_init.c modified to init I2C for camera */
│   ├── clock_config.c
│   │   /* boards/frdmmcxn947/eiq_examples/tflm_label_image/cm33_core0/clock_config.c modified to slow down MAIN_CLK (pll0) for camera */
│   └── pin_mux.c
│   |    /* boards/frdmmcxn947/eiq_examples/tflm_label_image/cm33_core0/pin_mux.c, modified camera pins (from SE project mcxn9xxbrk_flexio_lcd_smartdma_camera) 
|   |       and to add flexio LCD pins config from boards/frdmmcxn947/driver_examples/flexio/mculcd/edma_transfer/cm33_core0/pin_mux.c */
├── MCXN947_cm33_core0_flash.ld
│   /* devices/MCXN947/gcc/MCXN947_cm33_core0_flash.ld, modified
│      to remove core1 memory section and rpmsg-shared-mem, in order to enlarge core0 sram */
└── drivers (should not be built, use SDK drivers) 
    │   /* cherry-picked from branch develop/sdk_api_2.x in folder components/video/ */
    ├── fsl_video_common.c
    ├── fsl_video_common.h
    ├── fsl_camera_device.h
    ├── fsl_camera.h
    │   /* cherry-picked from branch origin/release/2.14.0_mcxn1x_rfp in folder components/video/camera/device/sccb */
    ├── fsl_sccb.c
    ├── fsl_sccb.h
    │   /* from /devices/MCXN947/drivers not modified */
    ├── fsl_inputmux_connections.h
    │   /* from /platform/drivers/inputmux/ not modified */
    ├── fsl_inputmux.c
    ├── fsl_inputmux.h
    │   /* from /platform/drivers/lpflexcomm/lpi2c/ not modified */
    ├── fsl_lpi2c.c
    ├── fsl_lpi2c.h
    │   /* cherry-picked from branch origin/release/2.14.0_mcxn1x_rfp in folder components/video/ */
    ├── fsl_ov7670.c
    ├── fsl_ov7670.h
    │   /* cherry-picked from branch origin/release/2.14.0_mcxn1x_rfp in folder platform/drivers/smartdma and modified */
    ├── fsl_smartdma.c
    ├── fsl_smartdma.h
    ├── fsl_smartdma_mcxn.c
    └── fsl_smartdma_mcxn.h
