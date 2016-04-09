# board information
BOARD_TAG     := uno
ARCHITECTURE  := avr

# Arduino configurations
ARDMK_DIR     := /usr/local/Cellar/arduino-mk/1.5
ARDUINO_DIR   := /Applications/Arduino.app/Contents/Java
AVR_TOOLS_DIR = /usr/local/CrossPack-AVR
MONITOR_PORT  := /dev/tty.usbmodem1411

ARDUINO_LIBS := stlport
LINKER_SCRIPTS := -L. -lstlport

include $(ARDMK_DIR)/Arduino.mk

# stlport directory where you cloned
STLPORT_DIR := $(HOME)/stlport-arduino
include STLport.mk
