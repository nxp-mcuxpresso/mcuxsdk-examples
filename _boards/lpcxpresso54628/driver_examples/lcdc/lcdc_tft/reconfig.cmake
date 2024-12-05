
# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/LPC/LPC54000/LPC54628/iar/LPC54628J512_flash.icf
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/driver_examples/lcdc/lcdc_tft/linker/LPC54608_flash.icf
)
