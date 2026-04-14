#
# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_mdk_configuration(
        CC "-DCORE1_IMAGE_COPY_TO_RAM"
)

# Add additional configuration
mcux_add_macro(
    CC "-DSDK_DEBUGCONSOLE_UART\
        -DSERIAL_PORT_TYPE_UART=1\
       -DPRINTF_ADVANCED_ENABLE=1\
       -DCORE1_IMAGE_COPY_TO_RAM"
)

mcux_add_source(
	BASE_PATH ${SdkRootDirPath}
	SOURCES ${board_root}/${board}/ecat_examples/dual_cores_servo_motor/master/${core_id}/clock_config.c
	SOURCES ${board_root}/${board}/ecat_examples/dual_cores_servo_motor/master/${core_id}/clock_config.h
)

mcux_add_armgcc_configuration(
    TARGETS debug release flexspi_nor_debug flexspi_nor_release flexspi_nor_hyperram_debug flexspi_nor_hyperram_relese
    LD "-Xlinker --defsym=__stack_size__=0x4000 -Xlinker --defsym=__heap_size__=0x10000"
    CC "-DCORE1_IMAGE_COPY_TO_RAM"
)

mcux_add_iar_configuration(
    TARGETS debug release flexspi_nor_debug flexspi_nor_release flexspi_nor_hyperram_debug flexspi_nor_hyperram_relese
    CC "-DCORE1_IMAGE_COPY_TO_RAM"
    LD "--image_input=${APPLICATION_BINARY_DIR}/../dual_cores_ecat_servo_motor_secondary_core/iar/core1_image.bin,_core1_image,__core1_image,4 \
        --keep _core1_image --config_def=__stack_size__=0x4000 --config_def=__heap_size__=0x10000"
)

mcux_add_mdk_configuration(
    TARGETS debug release flexspi_nor_debug flexspi_nor_release flexspi_nor_hyperram_debug flexspi_nor_hyperram_relese
    LD "--predefine=\"-D__stack_size__=0x4000\" --predefine=\"-D__heap_size__=0x10000\""
    CC "-DCORE1_IMAGE_COPY_TO_RAM"
    LD "--keep=*(*core1_code)"
)

mcux_add_include(
    TOOLCHAINS mdk armgcc
    BASE_PATH ${APPLICATION_BINARY_DIR}
    INCLUDES ../dual_cores_ecat_servo_motor_secondary_core/${CONFIG_TOOLCHAIN}/
)