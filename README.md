Logitech LCD Plugin
===================
OBS plugin that adds Logitech Colour and Monochrome LCD support.

Build
=====
- Clone this git repo and put it inside `obs-studio\plugins\`
- Add this line`add_subdirectory(LogiLCD)` to `obs-studio\plugins\CMakeLists.txt`
- In Cmake
  - add an entry like this `LCDSDK_PATH` `"D:/LCDSDK_8.57.148/"`
  - Configure and Generate
