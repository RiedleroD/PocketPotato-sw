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
uint8_t menu = 0;

void setup() {
  Serial.begin(9600);
  Serial.println(F("called setup()"));

  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);

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

  checkEEPROM();// Check and load EEPROM values.
  Serial.println(F("entering loop()"));
}

void loop() {
  if (down.isClick() || down.isStep())
    menu = constrain(menu + 1, 0, appcount - 1); // Down - next menu item
  else if (up.isClick() || up.isStep())
    menu = constrain(menu - 1, 0, appcount - 1);  // Up - prev.
  else if (left.isClick()) { // Settings
    // ...
  }
  else if (right.isClick() || sh_r.isClick()) { // Start app
    beginGame();
    switch(menu) {
      case 0:
        testButtons();
        break;
      case 1:
        gameTest();
        break;
    }
    endGame();
  }
  drawMenu();
  display.display();
}
