.PHONY: build flash clean fresh tests

#some variable
PATH_TO = ./somewhere/

flash : build
	echo "TODO"
build :
	arduino-cli compile --fqbn "arduino:avr:nano" --libraries "external/GyverButton/" ./
tests :
	echo "generate test binaries"
clean :
	echo "remove generated files"
prepare :
	arduino-cli core install arduino:avr
	arduino-cli lib install "Adafruit SSD1306"

fresh : | clean build