# Copyright 2025-2026 NXP
# SPDX-License-Identifier: BSD-3-Clause

# Enable Assert hooks in release mode.
mcux_remove_macro(
    TARGETS release
    CC "-DNDEBUG"
)

# This affects macros like __FILE__ used by assert so only the short filename appears in strings.
mcux_add_iar_configuration(
    TARGETS  release
   CC "     --no_path_in_file_macros "
)

# For ARM GCC (release target), rewrite absolute paths ${SdkRootDirPath}/ to empty.
# This shortens what ends up in __FILE__ expansions and other macro expansions at compile time.
# Additional, more specific mapping for the wireless middleware asserts.
mcux_add_armgcc_configuration(
    TARGETS  release
    CC "     -fmacro-prefix-map=${SdkRootDirPath}/=\
             -fmacro-prefix-map=${SdkRootDirPath}/components=\
             -fmacro-prefix-map=${SdkRootDirPath}/middleware/wireless/=\
             -fmacro-prefix-map=${SdkRootDirPath}/middleware/wireless/bluetooth/hci_transport/source=\
             -fmacro-prefix-map=${SdkRootDirPath}/middleware/wireless/ble_controller/src=\
             -fmacro-prefix-map=${SdkRootDirPath}/middleware/wireless/framework/services=\
             -fmacro-prefix-map=${SdkRootDirPath}/middleware/wireless/framework/platform/wireless_nbu= "
)
