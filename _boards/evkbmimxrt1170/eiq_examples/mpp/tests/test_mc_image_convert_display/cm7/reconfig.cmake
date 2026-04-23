# board+application specific cmake

# needed to boot core 1
mcux_add_armgcc_configuration(
    CC "-DCORE1_IMAGE_COPY_TO_RAM \
        -DMCMGR_USED"
)

mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=0x1400 \
             __heap_size__=0x2000 \
            "
)
