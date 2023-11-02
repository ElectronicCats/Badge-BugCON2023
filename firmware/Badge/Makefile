BOARD_TAG = arduino:mbed_rp2040:pico
MONITOR_PORT = /dev/cu.usbmodem111301
ARDUINO_CLI_PATH := arduino-cli

compile:
	$(ARDUINO_CLI_PATH) compile --fqbn $(BOARD_TAG)

upload:
	$(ARDUINO_CLI_PATH) upload -p $(MONITOR_PORT) --fqbn $(BOARD_TAG) --verbose

monitor:
	$(ARDUINO_CLI_PATH) monitor -p $(MONITOR_PORT)

clean:
	$(ARDUINO_CLI_PATH) cache clean

all: compile upload monitor