mcux_add_macro(
    CC "-DFRDM_MCXE247\
       -DFREEDOM"
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath} 
    INCLUDES ${board_root}/${board}/demo_apps/ftm_lpit_adc12
)