.PHONY: all debug build flash clean fresh tests prepare detect-hardware

#external libraries (external to arduino-cli, at least)
LIBS = "./external/GyverButton/"

$(info getting boards)
#sets BOARD and PORT to hopefully correct values
#for reference, here are some valid BOARDLIST examples:
#/dev/cu.usbmodem14101             serial   Serial Port (USB) Arduino Uno arduino:avr:uno arduino:avr
BOARDLIST ?= $(shell arduino-cli board list | grep "arduino:avr" | tail -f -n 1 -)
ifeq ($(BOARDLIST),)
    $(info trying again with a more lenient query)
    BOARDLIST = $(shell arduino-cli board list | grep "/")
endif
ifneq ($(BOARDLIST),No boards found.)
    PORT := $(firstword $(BOARDLIST))
    BOARD := $(filter arduino:avr:% , $(BOARDLIST))
    ifeq ($(BOARD),)
        $(warning could not determine board type, assuming arduino nano)
        BOARD := "arduino:avr:nano"
    endif
    ifeq ($(PORT),)
        $(error could not determine board from line: $(BOARDLIST))
    endif
else
    PORT := ""
endif

all : | flash debug
debug : detect-hardware
	stty -F $(PORT) raw 9600
	cat $(PORT)

flash : | detect-hardware build
	arduino-cli upload -i "./build/$(subst :,.,$(BOARD))/pocketpotato.ino.elf" --fqbn $(BOARD) -p $(PORT)
build : detect-hardware
	arduino-cli compile --fqbn $(BOARD) --libraries $(LIBS) ./ -e
#tests :	# TODO: make test cases
#	echo "generate test binaries"
clean :
	rm ./build -drf
	rm ./__pycache__ -drf
prepare :
	arduino-cli core install arduino:avr
	arduino-cli lib install "Adafruit SSD1306"

detect-hardware :
	@echo "$(BOARDLIST)"
	@if [ "$(BOARDLIST)" == "No boards found." ]; then\
		echo "No boards found to be connected."; \
		exit 1; \
	fi
	@echo "using port $(PORT)"
	@echo "and board $(BOARD)"
	@echo ""