
mcux_add_macro(
    CC "-DFRDM_KE17Z512\
       -DFREEDOM"
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/demo_apps/ftm_lpit_adc12/clock_config.c
    SOURCES examples/_boards/${board}/demo_apps/ftm_lpit_adc12/clock_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath} 
    INCLUDES examples/_boards/${board}/demo_apps/ftm_lpit_adc12
)
