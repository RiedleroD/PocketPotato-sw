.PHONY: all debug build flash clean fresh tests prepare

#full qualifying board name (see `arduino-cli board search` for all boards)
BOARD = "arduino:avr:nano"
#external libraries (external to arduino-cli, at least)
LIBS = "./external/GyverButton/"
#port the arduino is connected to. TODO: remove and replace with `arduino-cli board list`
PORT = /dev/ttyUSB0

all : | flash debug
debug :
	stty -F $(PORT) raw 9600
	cat $(PORT)

flash : build
	arduino-cli upload -i "./build/arduino.avr.nano/pocketpotato.ino.elf" --fqbn $(BOARD) -p $(PORT)
build :
	arduino-cli compile --fqbn $(BOARD) --libraries $(LIBS) ./ -e
#tests :	# TODO: make test cases
#	echo "generate test binaries"
clean :
	rm ./build -drf
	rm ./__pycache__ -drf
prepare :
	arduino-cli core install arduino:avr
	arduino-cli lib install "Adafruit SSD1306"
