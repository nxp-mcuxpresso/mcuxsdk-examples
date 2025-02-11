
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(audio_profile LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/edgefast_bluetooth_examples/audio_profile)

mcux_add_include(
 	BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/edgefast_bluetooth_examples/audio_profile
	TARGET edgefast_bluetooth_app.h
)
mcux_add_source(
	BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/edgefast_bluetooth_examples/audio_profile/${multicore_foldername}/edgefast_bluetooth_app.h
    CONFIG True
    PREINCLUDE True
)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES
    examples/edgefast_bluetooth_examples/audio_profile/FreeRTOSConfig.h
    examples/edgefast_bluetooth_examples/audio_profile/android_app/.idea/gradle.xml
    examples/edgefast_bluetooth_examples/audio_profile/android_app/.idea/misc.xml
    examples/edgefast_bluetooth_examples/audio_profile/android_app/.idea/modules.xml
    examples/edgefast_bluetooth_examples/audio_profile/android_app/.idea/runConfigurations.xml
    examples/edgefast_bluetooth_examples/audio_profile/android_app/AwsMusicControl.apk
    examples/edgefast_bluetooth_examples/audio_profile/android_app/AwsMusicControlPreferences.properties
    examples/edgefast_bluetooth_examples/audio_profile/android_app/app/build.gradle
    examples/edgefast_bluetooth_examples/audio_profile/android_app/app/proguard-rules.pro
    examples/edgefast_bluetooth_examples/audio_profile/android_app/app/src/main/AndroidManifest.xml
    examples/edgefast_bluetooth_examples/audio_profile/android_app/app/src/main/ic_launcher-web.png
    examples/edgefast_bluetooth_examples/audio_profile/android_app/app/src/main/ic_launcher_round-web.png
    examples/edgefast_bluetooth_examples/audio_profile/android_app/app/src/main/java/com/nxp/awsmusiccontrol/AboutActivity.java
    examples/edgefast_bluetooth_examples/audio_profile/android_app/app/src/main/java/com/nxp/awsmusiccontrol/MainActivity.java
    examples/edgefast_bluetooth_examples/audio_profile/android_app/app/src/main/java/com/nxp/awsmusiccontrol/iot/AwsConstants.java
    examples/edgefast_bluetooth_examples/audio_profile/android_app/app/src/main/java/com/nxp/awsmusiccontrol/iot/AwsShadow.java
    examples/edgefast_bluetooth_examples/audio_profile/android_app/app/src/main/java/com/nxp/awsmusiccontrol/iot/IPreferencesListener.java
    examples/edgefast_bluetooth_examples/audio_profile/android_app/app/src/main/java/com/nxp/awsmusiccontrol/iot/RgbLed.java
    examples/edgefast_bluetooth_examples/audio_profile/android_app/app/src/main/java/com/nxp/awsmusiccontrol/mqtt/Connection.java
    examples/edgefast_bluetooth_examples/audio_profile/android_app/app/src/main/res/drawable-hdpi/ic_action_refresh.png
    examples/edgefast_bluetooth_examples/audio_profile/android_app/app/src/main/res/drawable-hdpi/ic_led.png
    examples/edgefast_bluetooth_examples/audio_profile/android_app/app/src/main/res/drawable-hdpi/ic_led_filled.png
    examples/edgefast_bluetooth_examples/audio_profile/android_app/app/src/main/res/drawable-mdpi/ic_action_refresh.png
    examples/edgefast_bluetooth_examples/audio_profile/android_app/app/src/main/res/drawable-mdpi/ic_led.png
    examples/edgefast_bluetooth_examples/audio_profile/android_app/app/src/main/res/drawable-mdpi/ic_led_filled.png
    examples/edgefast_bluetooth_examples/audio_profile/android_app/app/src/main/res/drawable-xhdpi/ic_action_refresh.png
    examples/edgefast_bluetooth_examples/audio_profile/android_app/app/src/main/res/drawable-xhdpi/ic_led.png
    examples/edgefast_bluetooth_examples/audio_profile/android_app/app/src/main/res/drawable-xhdpi/ic_led_filled.png
    examples/edgefast_bluetooth_examples/audio_profile/android_app/app/src/main/res/drawable-xxhdpi/ic_action_refresh.png
    examples/edgefast_bluetooth_examples/audio_profile/android_app/app/src/main/res/drawable-xxhdpi/ic_led.png
    examples/edgefast_bluetooth_examples/audio_profile/android_app/app/src/main/res/drawable-xxhdpi/ic_led_filled.png
    examples/edgefast_bluetooth_examples/audio_profile/android_app/app/src/main/res/drawable/nxp.png
    examples/edgefast_bluetooth_examples/audio_profile/android_app/app/src/main/res/layout/activity_about.xml
    examples/edgefast_bluetooth_examples/audio_profile/android_app/app/src/main/res/layout/activity_main.xml
    examples/edgefast_bluetooth_examples/audio_profile/android_app/app/src/main/res/layout/content_about.xml
    examples/edgefast_bluetooth_examples/audio_profile/android_app/app/src/main/res/layout/content_main.xml
    examples/edgefast_bluetooth_examples/audio_profile/android_app/app/src/main/res/layout/music_info.xml
    examples/edgefast_bluetooth_examples/audio_profile/android_app/app/src/main/res/menu/main.xml
    examples/edgefast_bluetooth_examples/audio_profile/android_app/app/src/main/res/mipmap-hdpi/ic_launcher.png
    examples/edgefast_bluetooth_examples/audio_profile/android_app/app/src/main/res/mipmap-hdpi/ic_launcher_round.png
    examples/edgefast_bluetooth_examples/audio_profile/android_app/app/src/main/res/mipmap-mdpi/ic_launcher.png
    examples/edgefast_bluetooth_examples/audio_profile/android_app/app/src/main/res/mipmap-mdpi/ic_launcher_round.png
    examples/edgefast_bluetooth_examples/audio_profile/android_app/app/src/main/res/mipmap-xhdpi/ic_launcher.png
    examples/edgefast_bluetooth_examples/audio_profile/android_app/app/src/main/res/mipmap-xhdpi/ic_launcher_round.png
    examples/edgefast_bluetooth_examples/audio_profile/android_app/app/src/main/res/mipmap-xxhdpi/ic_launcher.png
    examples/edgefast_bluetooth_examples/audio_profile/android_app/app/src/main/res/mipmap-xxhdpi/ic_launcher_round.png
    examples/edgefast_bluetooth_examples/audio_profile/android_app/app/src/main/res/mipmap-xxxhdpi/ic_launcher.png
    examples/edgefast_bluetooth_examples/audio_profile/android_app/app/src/main/res/mipmap-xxxhdpi/ic_launcher_round.png
    examples/edgefast_bluetooth_examples/audio_profile/android_app/app/src/main/res/values/colors.xml
    examples/edgefast_bluetooth_examples/audio_profile/android_app/app/src/main/res/values/dimens.xml
    examples/edgefast_bluetooth_examples/audio_profile/android_app/app/src/main/res/values/drawables.xml
    examples/edgefast_bluetooth_examples/audio_profile/android_app/app/src/main/res/values/strings.xml
    examples/edgefast_bluetooth_examples/audio_profile/android_app/app/src/main/res/values/styles.xml
    examples/edgefast_bluetooth_examples/audio_profile/android_app/build.gradle
    examples/edgefast_bluetooth_examples/audio_profile/android_app/gradle.properties
    examples/edgefast_bluetooth_examples/audio_profile/android_app/gradle/wrapper/gradle-wrapper.jar
    examples/edgefast_bluetooth_examples/audio_profile/android_app/gradle/wrapper/gradle-wrapper.properties
    examples/edgefast_bluetooth_examples/audio_profile/android_app/gradlew
    examples/edgefast_bluetooth_examples/audio_profile/android_app/gradlew.bat
    examples/edgefast_bluetooth_examples/audio_profile/android_app/settings.gradle
    examples/edgefast_bluetooth_examples/audio_profile/app_a2dp.c
    examples/edgefast_bluetooth_examples/audio_profile/app_a2dp.h
    examples/edgefast_bluetooth_examples/audio_profile/app_common.c
    examples/edgefast_bluetooth_examples/audio_profile/app_common.h
    examples/edgefast_bluetooth_examples/audio_profile/app_music_control.c
    examples/edgefast_bluetooth_examples/audio_profile/app_music_control.h
    examples/edgefast_bluetooth_examples/audio_profile/app_streamer.c
    examples/edgefast_bluetooth_examples/audio_profile/app_streamer.h
    examples/edgefast_bluetooth_examples/audio_profile/cmd.c
    examples/edgefast_bluetooth_examples/audio_profile/cmd.h
    examples/edgefast_bluetooth_examples/audio_profile/ffconf.h
    examples/edgefast_bluetooth_examples/audio_profile/include/aws_clientcredential.h
    examples/edgefast_bluetooth_examples/audio_profile/include/aws_clientcredential_keys.h
    examples/edgefast_bluetooth_examples/audio_profile/include/core_mqtt_config.h
    examples/edgefast_bluetooth_examples/audio_profile/include/core_pkcs11_config.h
    examples/edgefast_bluetooth_examples/audio_profile/include/demo_config.h
    examples/edgefast_bluetooth_examples/audio_profile/include/kvstore_config.h
    examples/edgefast_bluetooth_examples/audio_profile/include/shadow_config.h
    examples/edgefast_bluetooth_examples/audio_profile/main.c
    examples/edgefast_bluetooth_examples/audio_profile/shadow_device_task.c
    examples/edgefast_bluetooth_examples/audio_profile/shadow_update_task.c
    examples/edgefast_bluetooth_examples/audio_profile/streamer_pcm.c
    examples/edgefast_bluetooth_examples/audio_profile/streamer_pcm.h
    examples/edgefast_bluetooth_examples/audio_profile/usb_host_fatfs.c
    examples/edgefast_bluetooth_examples/audio_profile/usb_host_fatfs.h
)

mcux_add_include(
  BASE_PATH ${SdkRootDirPath}
  INCLUDES
    examples/edgefast_bluetooth_examples/audio_profile
    examples/edgefast_bluetooth_examples/audio_profile/include
)

mcux_add_macro(
  CC "-DBT_BLE_PLATFORM_INIT_ESCAPE\
      -DCONTROLLER_INIT_ESCAPE\
      -DLPUART_RING_BUFFER_SIZE=1024U\
      -DMBEDTLS_USER_CONFIG_FILE=\\\"aws_mbedtls_config.h\\\"\
      -DMP3_DEC=1\
      -DPRINTF_ADVANCED_ENABLE=1\
      -DPRINTF_FLOAT_ENABLE=1\
      -DSDK_DEBUGCONSOLE_UART=1\
      -DSDK_OS_FREE_RTOS\
      -DSTREAMER_ENABLE_DECODER\
      -DSTREAMER_ENABLE_FILESRC\
      -DUSE_RTOS=1"
)

mcux_add_iar_configuration(
    LD "--semihosting"
)

mcux_add_mdk_configuration(
    LD "--library_type=microlib"
)

include(${SdkRootDirPath}/${board_root}/${board}/edgefast_bluetooth_examples/audio_profile/${multicore_foldername}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)