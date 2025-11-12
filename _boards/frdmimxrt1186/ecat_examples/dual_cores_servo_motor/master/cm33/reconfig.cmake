# Add additional configuration
mcux_add_macro(
    CC "-DSDK_DEBUGCONSOLE_UART\
        -DSERIAL_PORT_TYPE_UART=1\
       -DPRINTF_ADVANCED_ENABLE=1\
       -DCORE1_IMAGE_COPY_TO_RAM"
)

mcux_add_armgcc_configuration(
    TARGETS debug release flexspi_nor_debug flexspi_nor_release
    LD "-Xlinker --defsym=__stack_size__=0x4000 -Xlinker --defsym=__heap_size__=0x10000"
    CC "-DCORE1_IMAGE_COPY_TO_RAM"
    CC "-O1"
)

mcux_add_iar_configuration(
    TARGETS debug release flexspi_nor_debug flexspi_nor_release
    CC "-DCORE1_IMAGE_COPY_TO_RAM"
    LD "--image_input=${APPLICATION_BINARY_DIR}/../dual_cores_servo_motor_secondary_core/iar/core1_image.bin,_core1_image,__core1_image,4 
        --keep _core1_image --config_def=__stack_size__=0x4000 --config_def=__heap_size__=0x10000"
)

mcux_add_mdk_configuration(
    TARGETS debug release flexspi_nor_debug flexspi_nor_release
    LD "--predefine=\"-D__stack_size__=0x4000\" --predefine=\"-D__heap_size__=0x10000\""
    CC "-DCORE1_IMAGE_COPY_TO_RAM"
)
