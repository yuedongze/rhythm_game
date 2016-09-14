# rhythm_game
CS3420 Final Project by Dongze Yue (dy85) and Yang Liu (yl572)

Design: Rhythm Game

Submission:
block.c
block.h
board.c
board.h
clock_config.c
clock_config.h
controller.c
controller.h
data.c
data.h
definition.h
delay.c
delay.h
display.c
display.h
event.c
ffconf.h
game.c
game.h
hello_world.c
i2c.c
i2c.h
init.c
init.h
map.c
map.h
matrix.h
pin_mux.c
pin_mux.h
sdhc_config.h
start.c
start.h

In order to reproduce the Keil uVision project file, the following steps has to be done & included in the settings tab:
1, Include all the files in the submission into the Keil uVision project.
2, Obtain an official copy of MK64FN1MNxxx12 SDK. Within the project folder, copy SDK_Folder\middleware\fatfs_0.11a\, SDK_Folder\CMSIS, SDK_Folder\devices\MK64F12\ directories into the folder.
3, Open Keil uVision Settings, C/C++ Tab. In the Preprocessor Symbols>Define section, fill in

DEBUG, CPU_MK64FN1M0VMD12, PRINTF_FLOAT_ENABLE=0, SCANF_FLOAT_ENABLE=0, PRINTF_ADVANCED_ENABLE=0, SCANF_ADVANCED_ENABLE=0, FRDM_K64F, FREEDOM

Also, in the Include Paths section, Fill in

.\devices;.\devices\MK64F12\drivers;.\devices\MK64F12\utilities;.\devices\MK64F12;..\submission;.\fatfs_0.11a\src;.\fatfs_0.11a\src\fsl_sd_disk;.\sdmmc_2.0.0\inc;.\sdmmc_2.0.0\src;.\CMSIS\Include

In the Linker Tab, include the scatter file .\MK64FN1M0xxx12_flash.scf
4, Import the following files in to project Target, all of them can be found in the included folders

fsl_clock.c
fsl_clock.h
fsl_common.c
fsl_common.h
fsl_gpio.c
fsl_gpio.h
fsl_smc.c
fsl_smc.h
fsl_uart.c
fsl_uart.h
fsl_mpu.c
fsl_mpu.h
fsl_sdhc.c
fsl_sdhc.h

system_MK64F12.c
system_MK64F12.h
system_MK64F12.s

fsl_debug_console.c
fsl_debug_console.h

diskio.c
diskio.h
ff.c
ff.h
integer.h
fsl_sd_disk.c
fsl_sd_disk.h

fsl_mmc.c
fsl_sd.c
fsl_sdmmc.c

fsl_card.h
fsl_sdmmc.h
fsl_specification.h

5, In Keil Settings, Change the Target Tab>Read/Write Memory Areas to:
Tick IRAM1: Start:0x1FFF0000, Size:0x10000
Tick IRAM2: Start:0x20000000, Size:0x30000

6. Additional files included:

/maps/ folder: contains all the playing game maps that gets loaded onto the SD card.
/maps/extract.sh: the bash script written to convert Osu! official beatmaps to customized format
/maps/listen.py: the python script that runs on the laptop which listens to the serial port and plays music when game starts

And at last... Thanks for reading this!