.PHONY: all debug build flash clean fresh prepare

#external libraries (external to arduino-cli, at least)
LIBS = "./external/GyverButton/"

#detecting OS
ifeq ($(OS),Windows_NT)
  $(error Windows is not supported!)
else
  UNAME_S := $(shell uname -s)
  ifeq ($(UNAME_S),Linux)
    STTY_F=F
  endif
  ifeq ($(UNAME_S),Darwin)
    STTY_F=f
  endif
endif


#sets BOARD and PORT to hopefully correct values
#for reference, here are some valid BOARDLIST examples:
#/dev/cu.usbmodem14101             serial   Serial Port (USB) Arduino Uno arduino:avr:uno arduino:avr
#/dev/ttyUSB0                      serial                     Unknown
ifneq ($(MAKECMDGOALS),clean)
    $(info getting boards)
    ifndef BOARDLIST
        ALLBOARDS := $(shell printf %q "$$(arduino-cli board list)")
        BOARDLIST := $(shell printf %b "$(ALLBOARDS)" | grep "arduino:avr" | tail -f -n 1)
        ifeq ($(BOARDLIST),)
            $(info trying again with a more lenient query)
            BOARDLIST := $(shell printf %b "$(ALLBOARDS)" | grep "/")
        endif
    endif
    ifneq ($(BOARDLIST),No boards found.)
        PORT := $(firstword $(BOARDLIST))
        BOARD := $(filter arduino:avr:% , $(BOARDLIST))
        ifeq ($(BOARD),)
            $(warning could not determine board type, assuming arduino nano)
            BOARD := arduino:avr:nano
        endif
        ifeq ($(PORT),)
            $(error could not determine port from line: $(BOARDLIST))
        endif
    else
        $(error no board connected)
    endif
    $(info using port $(PORT))
    $(info and board $(BOARD))
    $(info $(shell echo -e "\033[36mset environment-variable \"BOARDLIST\" to skip hardware detection\033[38m"))
    $(info )
endif

#path to the binary file of the program
BIN_FP = ./build/$(subst :,.,$(BOARD))/pocketpotato.ino.elf

all : | flash debug
debug :
	stty -$(STTY_F) $(PORT) raw 9600
	cat $(PORT)

flash : $(BIN_FP)
	arduino-cli upload -i "$(BIN_FP)" --fqbn $(BOARD) -p $(PORT)
build : $(BIN_FP)
$(BIN_FP) : *.h */*.cpp *.ino
	arduino-cli compile --fqbn $(BOARD) --libraries $(LIBS) ./ -e
clean :
	rm ./build -drf
	rm ./__pycache__ -drf
prepare :
	arduino-cli core install arduino:avr
	arduino-cli lib install "Adafruit SSD1306"