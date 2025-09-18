mcux_add_macro(
    CC "-DCORE1_IMAGE_COPY_TO_RAM=1 \
        -DEIQ_EXAMPLE_HSRUN_CLOCK"
)

mcux_add_iar_configuration(
    LD "--image_input=${APPLICATION_BINARY_DIR}/../tflm_cifar10_hifi1_cm33_core1/iar/core1_image.bin,_core1_image,__core1_image,4 \
        --keep _core1_image"
)

mcux_add_mdk_configuration(
    LD "--keep=*(*core1_code)"
)

mcux_add_include(
    TOOLCHAINS mdk armgcc
    BASE_PATH ${APPLICATION_BINARY_DIR}
    INCLUDES ../tflm_cifar10_hifi1_cm33_core1/${CONFIG_TOOLCHAIN}/
)
