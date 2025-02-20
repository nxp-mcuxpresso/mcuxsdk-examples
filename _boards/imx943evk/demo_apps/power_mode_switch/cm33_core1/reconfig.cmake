mcux_add_configuration(
	CC "-DSDK_I2C_BASED_COMPONENT_USED=1\
        -DBOARD_USE_ADP5585=1\
        -DSCMI_LM_REQUEST_M7_SUSPEND_ENABLE=1"
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/demo_apps/power_mode_switch
)

mcux_add_macro(
    AS "-DNO_CRP"
)

mcux_add_iar_configuration(
    AS "-DCPU_${CONFIG_MCUX_HW_DEVICE_PART}${core_id_suffix_name}"
)
