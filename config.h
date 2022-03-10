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

const char _app1[] PROGMEM = "Button Test";
const char _app2[] PROGMEM = "Testgame";
const char _app3[] PROGMEM = "EEPROM Test";
const char _app4[] PROGMEM = "Snake";
const char _app5[] PROGMEM = "Settings";
const char _app6[] PROGMEM = "IDK";
const char _app7[] PROGMEM = "IDK2";

//TODO: compress strings lol
const char* const apps[] PROGMEM = {_app1,_app2,_app3,_app4,_app5,_app6};

const int appcount PROGMEM = sizeof(apps)/sizeof(char*);


/*
const char _test1[] PROGMEM = "first";
const char _test2[] PROGMEM = "second";
const char _test3[] PROGMEM = "dreite";
const char _test4[] PROGMEM = "fuck";
const char _test5[] PROGMEM = "my";
const char _test6[] PROGMEM = "life";
const char _test7[] PROGMEM = "next";
const char _test8[] PROGMEM = "page";
const char* const testArr[] PROGMEM = {_test1,_test2,_test3,_test4,_test5,_test6,_test7,_test8};

const int testLen PROGMEM = sizeof(testArr)/sizeof(char*);
*/
