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
//Menus must be have an Array and the corresponding length as PROGMEM variables.
//These lists should be defined where they are used.
//Here is an example of what this should look like:

//const char _test1[] PROGMEM = "first";
//const char _test2[] PROGMEM = "second";
//
//const char* const testArr[] PROGMEM = {_test1,_test2,_test3,_test4,_test5,_test6,_test7,_test8};
//
//const int testLen PROGMEM = sizeof(testArr)/sizeof(char*);

//Main Menu

MENUSTRINGS5(app,"Button Test","Testgame","EEPROM Test","Snake","Settings");