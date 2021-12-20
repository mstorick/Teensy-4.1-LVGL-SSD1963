# Teensy-4.1-LVGL-UTFT-SSD1963
This repo is the code I used to get LVGL working on a SSD1963-based touchscreen display, using a Teensy 4.1.

It uses UTFT as a driver for the LVGL code.

This is a platformio project.

## Instructions for cloning
First, download the Teensy 4.1 LVGL folder and open it in platformio.  Attempt to compile so that platformio downloads all the necessary libraries/platforms.  Then, find your .platformio folder (it is usually located in your home directory).  Modify the files as mentioned in UTFT_modifications.txt and misc_library_modifications.txt.  For convenience you can find the modified files in the .platformio folder in this repository.  Afterward, be sure to copy lv_conf.h to .pio/libdeps/teensy41/lvgl/ in this project file (it is needed for lvgl to compile).

If you have a touchscreen on your display, you will need to calibrate it yourself, and replace my values for A, B, C, D, E, and F in main.cpp.  Also, for initial testing it may be beneficial to use my_disp_flush_old() instead of my_disp_flush() in main.cpp.  The latter is faster but may introduce problems if your display is different than mine.

Hopefully this can help some people get on their feet with setting up a teensy 4.1 LVGL project.  It took me a while to figure out myself.

If you encounter any issues be sure your frameworks are the same version as mine:
 - Platformio Core version = 5.2.4
 - Teensyduino version = 1.55
 - Arduino version = 1.8.5
 - LVGL version = 8.0.1
