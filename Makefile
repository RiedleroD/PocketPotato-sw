.PHONY: all build flash clean prepare

#external libraries (external to arduino-cli, at least)
LIBS = "./external/GyverButton/"
#path to the binary file of the program
BIN_FP = ./build/$(subst :,.,$(BOARD))/pocketpotato.ino.elf

all : | flash

flash : $(BIN_FP)
	avrdude -c usbasp -p m328p -U flash:w:"$(BIN_FP)":e
build : $(BIN_FP)
$(BIN_FP) : *.h */*.cpp *.ino
	arduino-cli compile --fqbn arduino:nano --libraries $(LIBS) ./ -e
clean :
	rm ./build -drf
	rm ./__pycache__ -drf
prepare :
	arduino-cli core install arduino:avr
	arduino-cli lib install "Adafruit SSD1306"