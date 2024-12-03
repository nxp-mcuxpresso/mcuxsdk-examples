# Add additional configuration
mcux_add_macro(
  CC "-DRTCESL_PQ_ON=1 -DCPU_LPC55S69JBD100_cm33_core0=1"
  )

mcux_add_iar_configuration(
  TARGETS release
  CC "--no_unaligned_access"
  )

mcux_add_iar_configuration(
  LD "--config_def=__stack_size__=0x2000\
      --config_def=__heap_size__=0x1000\
      --config_def=__usb_use_usbram__=1"
  )

mcux_add_mdk_configuration(
 LD "--predefine=\"-D__stack_size__=0x2000\"\
     --predefine=\"-D__heap_size__=0x1000\"\
     --predefine=\"-D__usb_use_usbram__=1\""
)

mcux_add_mdk_configuration(
  TARGETS release
  CC "-mno-unaligned-access"
)

mcux_add_armgcc_configuration(
  CC "-mno-unaligned-access"
  TARGETS "release"
)

mcux_add_configuration(
  TOOLCHAINS mcux
  CC "-mno-unaligned-access"
  LD "-Xlinker --defsym=__stack_size__=0x2000\
      -Xlinker --defsym=__heap_size__=0x1000"
)
