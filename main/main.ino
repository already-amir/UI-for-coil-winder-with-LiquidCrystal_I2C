#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define BTN_UP     12
#define BTN_DOWN   13
#define BTN_OK     27
#define BTN_BACK   14


enum menu_state {
  MENU_MAIN,

  MENU_START,
  MENU_START_PAUSE,
  END_START_SUCCESS,
  END_START_UNSUCCESS,

  MENU_LOAD_SETTINGS,

  MENU_SETTINGS,
  MENU_SETTINGS_TURNS,
  MENU_SETTINGS_COIL,
  MENU_SETTINGS_WIRE,

  MENU_INFO

  // *********************************** TO ASK
};




int turns =  1000;
float coil_width = 20;
float wire_width = 0.25;

int last_save = 0;
int max_save = 25;
void save_settings(){
  if (last_save < max_save){
    int bios=10*last_save;
    EEPROM.put(bios + 0,turns);
    EEPROM.put(bios + 4,coil_width);
    EEPROM.put(bios + 8,wire_width);
    last_save ++;
  }
  // *********************************** TO DO 
}

void load_setings(int save_num){
  int bios= 10 * save_num;
  EEPROM.get(bios + 0,turns);
  EEPROM.get(bios + 4,coil_width);
  EEPROM.get(bios + 8,wire_width);
}

unsigned long last_press = 0;
bool is_button_pressed(int pin){
  if (digitalRead(pin)==LOW && millis() - last_press > 200){
    last_press=millis();
    return true;
  }
  return false;
}

const int main_menu_item_lenght = 4;
const char* main_menu_item[]={
  "START",
  "LOAD SETTINGS",
  "SETTING",
  "INFO"
};

const int load_item_length=25;
const char* load_item[]={
  "UNDEFINED 1",
  "UNDEFINED 2",
  "UNDEFINED 3",
  "UNDEFINED 4",
  "UNDEFINED 5",
  "UNDEFINED 6",
  "UNDEFINED 7",
  "UNDEFINED 8",
  "UNDEFINED 9",
  "UNDEFINED 10",
  "UNDEFINED 11",
  "UNDEFINED 12",
  "UNDEFINED 13",
  "UNDEFINED 14",
  "UNDEFINED 15",
  "UNDEFINED 16",
  "UNDEFINED 17",
  "UNDEFINED 18",
  "UNDEFINED 19",
  "UNDEFINED 20",
  "UNDEFINED 21",
  "UNDEFINED 22",
  "UNDEFINED 23",
  "UNDEFINED 24",
  "UNDEFINED 25"
};

const int settings_item_menue_lenght=4;
const char* settings_item_menue[]={
  "TURNS",
  "COIL WIDTH",
  "WIRE WIDTH",
  "SAVE"
}





void setup() {
 

}

menu_state curr_menu = MENU_MAIN;
int selected_item = 0;
int top_index = 0;
void loop() {
  // put your main code here, to run repeatedly:

}
