mcux_remove_mdk_configuration(
    CC "-DCORE1_IMAGE_COPY_TO_RAM"
)

mcux_remove_iar_configuration(
    CC "-DCORE1_IMAGE_COPY_TO_RAM"
)

mcux_remove_armgcc_configuration(
    CC "-DCORE1_IMAGE_COPY_TO_RAM"
)
