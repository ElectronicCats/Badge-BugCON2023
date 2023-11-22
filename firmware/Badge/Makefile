BOARD ?= rp2040 # rp2040, esp32, esp32s3

# BOARD_TAG_1 = arduino:mbed_rp2040:pico
BOARD_TAG_1 = rp2040:rp2040:rpipico
MONITOR_PORT_1 = /dev/cu.usbmodem11401
BOARD_TAG_2 = esp32:esp32:esp32wrover
MONITOR_PORT_2 = /dev/cu.wchusbserial550D0100521
BOARD_TAG_3 = esp32:esp32:esp32s3:CDCOnBoot=cdc
MONITOR_PORT_3 = /dev/cu.usbmodem11101
ARDUINO_CLI_PATH := arduino-cli

ifeq ($(BOARD),rp2040)
	BOARD_TAG := $(BOARD_TAG_1)
	MONITOR_PORT := $(MONITOR_PORT_1)
else ifeq ($(BOARD),esp32)
	BOARD_TAG := $(BOARD_TAG_2)
	MONITOR_PORT := $(MONITOR_PORT_2)
else ifeq ($(BOARD),esp32s3)
	BOARD_TAG := $(BOARD_TAG_3)
	MONITOR_PORT := $(MONITOR_PORT_3)
endif

test:
	@if [ -z "$(BOARD_TAG)" ]; then echo "BOARD is not set, please set BOARD to rp2040 or esp32"; exit 1; fi

help:
	@echo "Usage: make [target] [BOARD=rp2040|esp32|esp32s3]"
	@echo "Targets:"
	@echo "  compile: compile the firmware"
	@echo "  upload: upload the firmware to the board"
	@echo "  monitor: monitor the serial port"
	@echo "  clean: clean the cache"
	@echo "  all: compile, upload, monitor"
	@echo "Examples:"
	@echo "  make compile BOARD=rp2040"
	@echo "  make upload BOARD=rp2040"
	@echo "  make all BOARD=esp32s3"

translate:
	@python3 web_files_converter.py

compile:
	$(ARDUINO_CLI_PATH) compile --fqbn $(BOARD_TAG) --export-binaries

upload:
	@$(ARDUINO_CLI_PATH) upload -p $(MONITOR_PORT) --fqbn $(BOARD_TAG) --verbose

monitor:
	@$(ARDUINO_CLI_PATH) monitor -p $(MONITOR_PORT)

clean:
	@$(ARDUINO_CLI_PATH) cache clean

all: test translate compile upload monitor

.PHONY: compile upload monitor clean all