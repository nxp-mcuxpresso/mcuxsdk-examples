cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(camera_gesture_recognition_view LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/eiq_examples/mpp/examples/camera_gesture_recognition_view)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/eiq_examples/mpp/camera_gesture_recognition_view/camera_gesture_recognition_view.c
            ${board_root}/${board}/eiq_examples/mpp/examples/camera_gesture_recognition_view/mpp_config.h
            middleware/eiq/mpp/common/draw_text.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/eiq_examples/mpp/examples/camera_gesture_recognition_view/
             middleware/eiq/mpp/common/
)

# board files
include(${SdkRootDirPath}/${board_root}/${board}/eiq_examples/mpp/board.cmake)
# model and post-processing
# model and post-processing
mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/middleware/eiq/mpp
    INCLUDES    models
                models/blaze_detector_ptq
                models/hand_landmark
                models/gesture_embedder
                models/canned_gesture_classifier
)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/middleware/eiq/mpp/models
    SOURCES utils.cpp
            utils.h
            blaze_detector_ptq/blaze_detector_ptq_output_postproc.c
            blaze_detector_ptq/*.h
            hand_landmark/hand_landmark_quant_output_postproc.c
            hand_landmark/*.h
            gesture_embedder/*.h
            canned_gesture_classifier/canned_gesture_classifier_output_postproc.c
            canned_gesture_classifier/*.h
)

# top level
include(${SdkRootDirPath}/CMakeLists.txt)
# board & app specific cmake
include(${SdkRootDirPath}/${board_root}/${board}/eiq_examples/mpp/examples/camera_gesture_recognition_view/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

