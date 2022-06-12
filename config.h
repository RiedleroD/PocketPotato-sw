// ---------- HARDWARE PINS ----------
// D-Pad
#define BTN_UP A3
#define BTN_DOWN A2
#define BTN_LEFT A1
#define BTN_RIGHT A0
//Shoulder Buttons
#define SH_R A4
#define SH_L A5
//display control pins
#define OLED_MOSI  11
#define OLED_CLK   12
#define OLED_CS    10
#define OLED_DC    9
#define OLED_RESET 8
#define OLED_VCC1  6
#define OLED_VCC2  10
//LED
#define BAT_LED    5

// ---------- SOFTWARE SETTINGS ----------
//timeouts for button presses
#define HOLD_TIMEOUT 300
#define STEP_TIMEOUT 50

// ---------- EEPROM ADDRESSES ----------

#define ADR_TESTSCORE 0
#define ADR_BRIGHTNESS 1

#define ADR_SNAKESCORE 16 // 16 bits!
#define ADR_PONGSCORE 18 // 16 bits!
#define ADR_DINOSCORE 32 //16 bits

// ---------- APPS ----------
//Menus must be have an Array and the corresponding length as PROGMEM variables.
//These lists should be defined where they are used.
//Here is an example of what this should look like:

//for more info → see the macro in funcs.h

//Main Menu
MENUSTRINGS3(app,"Snake","Pong","Dino");
