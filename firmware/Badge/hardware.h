#ifndef HARDWARE_H
#define HARDWARE_H

#ifndef BOARD_NAME
	#ifdef USB_VID
		#define BOARD_NAME "ESP32-S3"
		#define ESP32_S3
	#else
		#define BOARD_NAME "ESP32"
		#define ESP32_DEVKIT
	#endif
#else
#define RP2040
#endif

#endif