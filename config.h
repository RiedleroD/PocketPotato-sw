// ---------- HARDWARE PINS ----------
// D-Pad
#define BTN_UP 1
#define BTN_DOWN 3
#define BTN_LEFT 6
#define BTN_RIGHT 4
//Shoulder Buttons
#define SH_R 7
#define SH_L 2
//display control pins
#define OLED_MOSI  11
#define OLED_CLK   13
#define OLED_DC    9
#define OLED_CS    8
#define OLED_RESET 10

// ---------- SOFTWARE SETTINGS ----------
//timeouts for button presses
#define HOLD_TIMEOUT 300
#define STEP_TIMEOUT 50

// ---------- EEPROM ADDRESSES ----------

#define ADR_TESTSCORE 0
#define ADR_BRIGHTNESS 1

#define ADR_SNAKESCORE 16 // 16 bits!

// ---------- APPS ----------

const char _app1[] PROGMEM = "Controller Test";
const char _app2[] PROGMEM = "Dino";
const char _app3[] PROGMEM = "Pong";
const char _app4[] PROGMEM = "Breakout";

//TODO: compress strings lol
const char* const apps[] PROGMEM = {_app1,_app2,_app3,_app4};

const int appcount PROGMEM = sizeof(apps)/sizeof(char*);
