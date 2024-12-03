#list app specific source files
list(APPEND APP_MODEL_SRC
    ${S}examples/camera_ultraface_mobilefacenet_view/face_box_utils.c
    ${S}examples/camera_ultraface_mobilefacenet_view/event_listener.c
    ${S}examples/models/get_top_n.cpp
    ${S}examples/models/utils.cpp
    ${S}examples/models/ultraface_slim_quant_int8/ultraface_output_postproc.c
    ${S}internal/models/mobilefacenet/mobilefacenet_output_postproc_quantized.cpp
)

