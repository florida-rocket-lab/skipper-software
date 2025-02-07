# Skipper Software

This repository serves to have all the code for Skipper, whether
that be GNC or Avionics, in a single spot so that working on the 
code, uploading to the boards, or troubleshooting super easy.

Currently, we have the following setup for the code, but this is
subject to change in the future:
```
(repository root)
|--GNC
|  +--(.m MATLAB files and .slx Simulink model
|       to eventually be compiled and included
|       in "/Software/skipper-flight-computer")
|
+--Software
   |--skipper-lib
   |  +--(.cpp C++ file and .hpp header files
   |      that are included and used in some of
   |      the Arduino sketches. General rule is
   |      if multiple devices need the code, put
   |      it here and then include it.)
   |
   |--ground-transceiver
   |  +--(.ino Arduino files and .hpp header
   |      files to be uploaded to the Arduino
   |      UNO/MEGA on the ground)
   |
   |--interactive-terminal
   |  +--(.py Python files to create a usable,
   |      interactive terminal for communicating
   |      with Skipper and receiving valuable
   |      flight data from Skipper)
   |
   |--skipper-flight-computer
   |  +--(.ino Arduino files and .hpp header
   |      files to be uploaded to the Teensy
   |      4.0 on Skipper)
   |
   +--skipper-transceiver
      +--(.ino Arduino files and .hpp header
          files to be uploaded to the Arduino
          Nano on Skipper)
```
This repository is still **IN DEVELOPMENT**, and we'd greatly
appreciate your help in developing, if you are willing to code
well and **write documentation**. Look at the `.ino` files or the
`.hpp` files, and there should be tasks indicated by `// TODO: ...`.

Help Me
---
Thank the *FRL* gods; you found me! Help, Jose trapped me in the README.md, 
and he's using me as his optimization slave! Go to https://r.mtdv.me/save-skipper-the-arg 
to save Skipper!
