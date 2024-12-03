
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(audio_profile LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/edgefast_bluetooth_examples/audio_profile)

mcux_add_include(
 	BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/audio_profile
	TARGET edgefast_bluetooth_app.h
)
mcux_add_source(
	BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/edgefast_bluetooth/boards/${board}/edgefast_bluetooth_examples/audio_profile/${multicore_foldername}/edgefast_bluetooth_app.h
    CONFIG True
    PREINCLUDE True
)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES
    middleware/edgefast_bluetooth/examples/audio_profile/FreeRTOSConfig.h
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/.idea/gradle.xml
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/.idea/misc.xml
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/.idea/modules.xml
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/.idea/runConfigurations.xml
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/AwsMusicControl.apk
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/AwsMusicControlPreferences.properties
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/app/build.gradle
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/app/proguard-rules.pro
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/app/src/main/AndroidManifest.xml
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/app/src/main/ic_launcher-web.png
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/app/src/main/ic_launcher_round-web.png
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/app/src/main/java/com/nxp/awsmusiccontrol/AboutActivity.java
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/app/src/main/java/com/nxp/awsmusiccontrol/MainActivity.java
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/app/src/main/java/com/nxp/awsmusiccontrol/iot/AwsConstants.java
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/app/src/main/java/com/nxp/awsmusiccontrol/iot/AwsShadow.java
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/app/src/main/java/com/nxp/awsmusiccontrol/iot/IPreferencesListener.java
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/app/src/main/java/com/nxp/awsmusiccontrol/iot/RgbLed.java
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/app/src/main/java/com/nxp/awsmusiccontrol/mqtt/Connection.java
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/app/src/main/res/drawable-hdpi/ic_action_refresh.png
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/app/src/main/res/drawable-hdpi/ic_led.png
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/app/src/main/res/drawable-hdpi/ic_led_filled.png
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/app/src/main/res/drawable-mdpi/ic_action_refresh.png
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/app/src/main/res/drawable-mdpi/ic_led.png
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/app/src/main/res/drawable-mdpi/ic_led_filled.png
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/app/src/main/res/drawable-xhdpi/ic_action_refresh.png
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/app/src/main/res/drawable-xhdpi/ic_led.png
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/app/src/main/res/drawable-xhdpi/ic_led_filled.png
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/app/src/main/res/drawable-xxhdpi/ic_action_refresh.png
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/app/src/main/res/drawable-xxhdpi/ic_led.png
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/app/src/main/res/drawable-xxhdpi/ic_led_filled.png
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/app/src/main/res/drawable/nxp.png
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/app/src/main/res/layout/activity_about.xml
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/app/src/main/res/layout/activity_main.xml
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/app/src/main/res/layout/content_about.xml
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/app/src/main/res/layout/content_main.xml
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/app/src/main/res/layout/music_info.xml
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/app/src/main/res/menu/main.xml
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/app/src/main/res/mipmap-hdpi/ic_launcher.png
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/app/src/main/res/mipmap-hdpi/ic_launcher_round.png
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/app/src/main/res/mipmap-mdpi/ic_launcher.png
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/app/src/main/res/mipmap-mdpi/ic_launcher_round.png
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/app/src/main/res/mipmap-xhdpi/ic_launcher.png
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/app/src/main/res/mipmap-xhdpi/ic_launcher_round.png
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/app/src/main/res/mipmap-xxhdpi/ic_launcher.png
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/app/src/main/res/mipmap-xxhdpi/ic_launcher_round.png
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/app/src/main/res/mipmap-xxxhdpi/ic_launcher.png
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/app/src/main/res/mipmap-xxxhdpi/ic_launcher_round.png
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/app/src/main/res/values/colors.xml
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/app/src/main/res/values/dimens.xml
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/app/src/main/res/values/drawables.xml
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/app/src/main/res/values/strings.xml
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/app/src/main/res/values/styles.xml
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/build.gradle
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/gradle.properties
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/gradle/wrapper/gradle-wrapper.jar
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/gradle/wrapper/gradle-wrapper.properties
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/gradlew
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/gradlew.bat
    middleware/edgefast_bluetooth/examples/audio_profile/android_app/settings.gradle
    middleware/edgefast_bluetooth/examples/audio_profile/app_a2dp.c
    middleware/edgefast_bluetooth/examples/audio_profile/app_a2dp.h
    middleware/edgefast_bluetooth/examples/audio_profile/app_common.c
    middleware/edgefast_bluetooth/examples/audio_profile/app_common.h
    middleware/edgefast_bluetooth/examples/audio_profile/app_music_control.c
    middleware/edgefast_bluetooth/examples/audio_profile/app_music_control.h
    middleware/edgefast_bluetooth/examples/audio_profile/app_streamer.c
    middleware/edgefast_bluetooth/examples/audio_profile/app_streamer.h
    middleware/edgefast_bluetooth/examples/audio_profile/cmd.c
    middleware/edgefast_bluetooth/examples/audio_profile/cmd.h
    middleware/edgefast_bluetooth/examples/audio_profile/ffconf.h
    middleware/edgefast_bluetooth/examples/audio_profile/include/aws_clientcredential.h
    middleware/edgefast_bluetooth/examples/audio_profile/include/aws_clientcredential_keys.h
    middleware/edgefast_bluetooth/examples/audio_profile/include/core_mqtt_config.h
    middleware/edgefast_bluetooth/examples/audio_profile/include/core_pkcs11_config.h
    middleware/edgefast_bluetooth/examples/audio_profile/include/demo_config.h
    middleware/edgefast_bluetooth/examples/audio_profile/include/kvstore_config.h
    middleware/edgefast_bluetooth/examples/audio_profile/include/shadow_config.h
    middleware/edgefast_bluetooth/examples/audio_profile/main.c
    middleware/edgefast_bluetooth/examples/audio_profile/shadow_device_task.c
    middleware/edgefast_bluetooth/examples/audio_profile/shadow_update_task.c
    middleware/edgefast_bluetooth/examples/audio_profile/streamer_pcm.c
    middleware/edgefast_bluetooth/examples/audio_profile/streamer_pcm.h
    middleware/edgefast_bluetooth/examples/audio_profile/usb_host_fatfs.c
    middleware/edgefast_bluetooth/examples/audio_profile/usb_host_fatfs.h
)

mcux_add_include(
  BASE_PATH ${SdkRootDirPath}
  INCLUDES
    middleware/edgefast_bluetooth/examples/audio_profile
    middleware/edgefast_bluetooth/examples/audio_profile/include
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

include(${SdkRootDirPath}/examples/_boards/${board}/edgefast_bluetooth_examples/audio_profile/${multicore_foldername}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)