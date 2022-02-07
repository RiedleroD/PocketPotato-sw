// ---------- HARDWARE PINS ----------
#define BTN_UP 4
#define BTN_DOWN 3
#define BTN_LEFT 6
#define BTN_RIGHT 5
#define SH_R 7
#define SH_L 2
#define HOLD_TIMEOUT 500
#define STEP_TIMEOUT 200

// ---------- SOFTWARE SETTINGS ----------
#define GAME_FPS 60
#define MENU_FPS 5
#define APPS 9  // IMPORTANT!!!!! Change this if new apps were added. TODO: Maybe use sizeof(apps) instead of this constant

// ---------- EEPROM SETTINGS ----------
#define EEPROM_KEY 228  // Control key value
#define KEY_ADDR 0  // Control key. If it exists and value matches, settings were already saved, all OK
#define EEPROM_CONTR 1  // Contrast cell
#define EEPROM_POFF 2 // Auto power off cell

// ---------- APPS ----------
const char dino[] PROGMEM = "Dino";
const char pong[] PROGMEM = "Pong";
const char breakout[] PROGMEM = "Breakout";

const char* const apps[] PROGMEM = {dino, pong, breakout, dino, pong, breakout, dino, pong, breakout};  // Array of PROGMEM saved char arrays
const uint8_t app_eeprom_keys[] PROGMEM = {10, 11, 12}; // EEPROM cell for each app
