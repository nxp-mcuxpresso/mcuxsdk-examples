
mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/multicore_examples
)

mcux_add_macro(
    CC "-DUSE_HYPERRAM=1"
)

mcux_add_mdk_configuration(
    CC "-DCORE1_IMAGE_COPY_TO_RAM"
    LD "--keep=*(*core1_code)"
)

mcux_add_iar_configuration(
    CC "-DCORE1_IMAGE_COPY_TO_RAM"
)

mcux_add_armgcc_configuration(
    CC "-DCORE1_IMAGE_COPY_TO_RAM"
)