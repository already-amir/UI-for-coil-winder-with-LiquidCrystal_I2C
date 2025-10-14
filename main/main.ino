#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>


#define BTN_UP     12
#define BTN_DOWN   13
#define BTN_OK     27
#define BTN_BACK   14


void setup() {
  lcd.init();
  lcd.backlight();

  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_OK, INPUT_PULLUP);
  pinMode(BTN_BACK, INPUT_PULLUP);

  EEPROM.begin(64);
  loadSettings();

  showMenu();
}

void loop() {}