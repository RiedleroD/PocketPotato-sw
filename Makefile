.PHONY: build flash clean fresh tests

#some variable
BOARD = "arduino:avr:nano"
LIBS = "./external/GyverButton/"
PORT = /dev/ttyUSB0

all : | flash debug
debug :
	stty -F $(PORT) raw 9600
	cat $(PORT)

flash : build
	arduino-cli upload -i "./build/arduino.avr.nano/pocketpotato.ino.elf" --fqbn $(BOARD) -p $(PORT)
build :
	arduino-cli compile --fqbn $(BOARD) --libraries $(LIBS) ./ -e
tests :
	echo "generate test binaries"
clean :
	rm ./build -drf
prepare :
	arduino-cli core install arduino:avr
	arduino-cli lib install "Adafruit SSD1306"

fresh : | clean build