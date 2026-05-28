
mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/driver_examples/asrc/asrc_m2m_polling
)

mcux_add_macro(
    CC "-DSDK_I2C_BASED_COMPONENT_USED=1\
       -DBOARD_USE_PCAL6408=1"
)
