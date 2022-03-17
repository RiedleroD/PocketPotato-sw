#include <SPI.h>
#include <EEPROM.h>
#include <Adafruit_GFX.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_SSD1306.h>
#include <GyverButton.h>
#include <util/delay.h>
#include "config.h"
#include "bitmaps.h"

#define SSD1306_NO_SPLASH
Adafruit_SSD1306 display(128, 64, &SPI, OLED_DC, OLED_RESET, OLED_CS);
GButton up(BTN_UP);
GButton down(BTN_DOWN);
GButton left(BTN_LEFT);
GButton right(BTN_RIGHT);
GButton sh_r(SH_R);
GButton sh_l(SH_L);

#include "util/menu.cpp"
#include "games/games.cpp"

void setup() {
  Serial.begin(9600);
  Serial.println(F("called setup()"));

  SPI.begin();

  up.setTickMode(AUTO); // Automatically check button status
  down.setTickMode(AUTO);
  right.setTickMode(AUTO);
  left.setTickMode(AUTO);
  sh_r.setTickMode(AUTO);
  sh_l.setTickMode(AUTO);

  left.setTimeout(HOLD_TIMEOUT);  // Set timeouts
  up.setStepTimeout(STEP_TIMEOUT);
  down.setStepTimeout(STEP_TIMEOUT);
  left.setStepTimeout(STEP_TIMEOUT);
  right.setStepTimeout(STEP_TIMEOUT);

  display.setTextSize(1);//config text
  display.setTextColor(SSD1306_WHITE);
  display.cp437(true);

  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();
  showLogo();

  loadSettings();//load settings from EEPROM
  Serial.println(F("entering loop()"));
}

void loop() {
	const uint8_t select = menu::draw(apps, appcount);

	beginGame();
	switch (select) {
		case 0:
			testButtons();
			break;
		case 1:
			gameTest();
			break;
		case 2:
			eepromtest();
			break;
		case 3:
			snake::run();
			break;
		case 4:
			showSettings();
			break;
		case 5:
			dino::game();
			break;
	}
	endGame();
}
