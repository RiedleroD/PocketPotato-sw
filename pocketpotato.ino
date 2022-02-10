#include <EEPROM.h>
#include <Adafruit_GFX.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_SSD1306.h>
#include <GyverButton.h>
#include <util/delay.h>
#include "config.h"
#include "bitmaps.h"

#define SSD1306_NO_SPLASH
//TODO: initialize with hardware SPI
Adafruit_SSD1306 display(128, 64, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
GButton up(BTN_UP, LOW_PULL);
GButton down(BTN_DOWN, LOW_PULL);
GButton left(BTN_LEFT, LOW_PULL);
GButton right(BTN_RIGHT, LOW_PULL);
GButton sh_r(SH_R, LOW_PULL);
GButton sh_l(SH_L, LOW_PULL);
uint8_t menu = 0;

void setup() {
	Serial.begin(9600);
	Serial.println(F("called setup()"));
	
	up.setTickMode(AUTO); // Automatically check button status
	down.setTickMode(AUTO);
	right.setTickMode(AUTO);
	left.setTickMode(AUTO);
	sh_r.setTickMode(AUTO);
	sh_l.setTickMode(AUTO);

	left.setTimeout(HOLD_TIMEOUT);  // Set timeouts
	up.setStepTimeout(STEP_TIMEOUT);
	down.setStepTimeout(STEP_TIMEOUT);
	
	display.setTextSize(1);//config text
	display.setTextColor(SSD1306_WHITE);
	display.cp437(true);
	
	display.begin(SSD1306_SWITCHCAPVCC);
	display.clearDisplay();
	showLogo();
	
	checkEEPROM();// Check and load EEPROM values.
	Serial.println(F("entering loop()"));
}

void loop() {
	if (down.isClick() || down.isStep())
		menu = constrain(menu + 1, 0, appcount - 1); // Down - next menu item
	else if (up.isClick() || up.isStep())
		menu = constrain(menu - 1, 0, appcount - 1);  // Up - prev.
	else if (left.isHold()) { // Settings
		// ...
	}
	else if (right.isClick() || sh_r.isClick()) { // Start app
		//    startApp();
	}
	drawMenu();
}
