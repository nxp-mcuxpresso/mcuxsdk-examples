
if ("${board}" STREQUAL "frdmrw612" OR
    "${board}" STREQUAL "rdrw612bga")
        ExternalMCUXProject_Add(
                APPLICATION mcuboot
                SOURCE_DIR  ${APP_DIR}/../../../../ota_examples/mcuboot_opensource
    )

    # Let's build the secondary application first
    add_dependencies(${DEFAULT_IMAGE} mcuboot)

    # Add custom setting for mcuboot
    set_config_bool(mcuboot CONFIG_BOOT_CUSTOM_DEVICE_SETUP y)
    set_config_int(mcuboot CONFIG_BOOT_FLASH_ACT_APP_ADDRESS 0x08020000)
    set_config_int(mcuboot CONFIG_BOOT_FLASH_CAND_APP_ADDRESS 0x08460000)
    set_config_int(mcuboot CONFIG_MCUBOOT_MAX_IMG_SECTORS 1088)
    set_config_bool(mcuboot CONFIG_BOOT_SIGNATURE_TYPE_RSA y)
endif()
