#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

#mcux_add_include(
#    INCLUDES .
#)

mcux_add_configuration(
           CC "-DBOARD_USE_PCA6416A=1"
)

mcux_add_configuration(
           CC "-DBOARD_USE_TPM=1"
)

mcux_add_configuration(
           CC "-DSDK_I2C_BASED_COMPONENT_USED=1"
)


mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/evkmimx8ulp/rsc_table.c
            examples/_boards/evkmimx8ulp/rsc_table.h
            middleware/multicore/remoteproc/remoteproc.h
            examples/_boards/evkmimx8ulp/srtm_config.h
            examples/_boards/evkmimx8ulp/drivers/lpi2c/fsl_lpi2c_freertos.c
            examples/_boards/evkmimx8ulp/drivers/lpi2c/fsl_lpi2c_freertos.h
            examples/_boards/evk9mimx8ulp/FreeRTOSConfigBoard.h
            examples/_boards/evkmimx8ulp/app_srtm.c
            examples/_boards/evkmimx8ulp/app_srtm.h
	    examples/_boards/evkmimx8ulp/multicore_examples/rpmsg_lite_pingpong_rtos_linux/remote/hardware_init.c
	    examples/_boards/evkmimx8ulp/multicore_examples/rpmsg_lite_pingpong_rtos_linux/remote/app.h
	    examples/_boards/evkmimx8ulp/multicore_examples/rpmsg_lite_pingpong_rtos_linux/remote/rpmsg_config.h
	    examples/_boards/evkmimx8ulp/multicore_examples/rpmsg_lite_pingpong_rtos_linux/FreeRTOSConfig.h

)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/evkmimx8ulp
             middleware/multicore/remoteproc
             examples/_boards/evkmimx8ulp/drivers/lpi2c
	     examples/_boards/evkmimx8ulp/multicore_examples/rpmsg_lite_pingpong_rtos_linux
	     examples/_boards/evkmimx8ulp/multicore_examples/rpmsg_lite_pingpong_rtos_linux/remote
)

mcux_add_macro(
    CC "-DSDK_I2C_BASED_COMPONENT_USED=1\
       -DBOARD_USE_PCA6416A=1"
    AS "-DNO_CRP"
)

mcux_remove_iar_configuration(
  TARGETS flash_release
  CC "-Oh"
)

mcux_add_iar_configuration(
    TARGETS flash_release
    CC "-Ohs"
)
