mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/middleware/wireless/ble_controller/boards/kw47evk_nbu/nbu_ble/
    SOURCES app_preinclude.h
	PREINCLUDE TRUE
)

# Enable Assert hooks in release mode.
mcux_remove_macro(
    TARGETS release
    CC "-DNDEBUG"
)

mcux_add_source(
    SOURCES ../readme.txt
)
