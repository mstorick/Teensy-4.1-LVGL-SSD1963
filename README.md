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

## Using only UTFT
If you would like to only use UTFT, don't need any of the platformio installation steps.  Simply make sure teensyduino is installed, change the three UTFT library files (should be installed with teensyduino) mentioned in UTFT_modifications.txt, and run the arduino sketch UTFT_test.ino.

## Pinout
| Teensy 4.1 pin | SSD1963 Display pin | XPT2046 | External 5v supply |
|----------------|---------------------|---------|--------------------|
| 24             | DB0                 |         |                    |
| 25             | DB1                 |         |                    |
| 26             | DB2                 |         |                    |
| 27             | DB3                 |         |                    |
| 17             | DB4                 |         |                    |
| 16             | DB5                 |         |                    |
| 22             | DB6                 |         |                    |
| 23             | DB7                 |         |                    |
| 20             | DB8                 |         |                    |
| 21             | DB9                 |         |                    |
| 1              | DB10                |         |                    |
| 0              | DB11                |         |                    |
| 19             | DB12                |         |                    |
| 18             | DB13                |         |                    |
| 14             | DB14                |         |                    |
| 15             | DB15                |         |                    |
| 3.3v           | 3.3v                |         |                    |
| GND            | GND                 |         |                    |
| 3              | RS                  |         |                    |
| 4              | WR                  |         |                    |
| 22             | CS                  |         |                    |
| 23             | REST/Reset          |         |                    |
| 3.3v           | RD                  |         |                    |
|                | 5v                  |         | +5v                |
|                | LED_A               |         | +5v                |
| 10             |                     | T_CS (slave select)   |                    |
| 13             |                     | T_CLK   |                    |
| 11             |                     | T_DIN   |                    |
| 12             |                     | T_DO    |                    |
| 9              |                     | T_IRQ   |                    |

## Suggestions/Questions
If you have any questions or suggestions feel free to leave an issue or a pull request.
