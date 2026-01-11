# Board Support Files

## board

### [26.03.00]

- Add GPIO pull up workaround.
  Some chips could not pull up some GPIO pins which are MUX with GAC ACOMP
  if GPIO power domain is 1.8V.
  Add workaround function BOARD_ApplyGpioPullUpWorkaround, call it before
  using GPIO input function.

### [25.06.00]
- Initial version

## clock\_config

### [25.06.00]
- Initial version

## pin\_mux

### [25.06.00]
- Initial version
