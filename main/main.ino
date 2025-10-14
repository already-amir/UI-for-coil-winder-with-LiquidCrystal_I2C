#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define BTN_UP     12
#define BTN_DOWN   13
#define BTN_OK     27
#define BTN_BACK   14


byte Check[] = {
  B00000,
  B00001,
  B00011,
  B10110,
  B11100,
  B01000,
  B00000,
  B00000
};


enum menu_state {
  MENU_MAIN,

  MENU_START,
  MENU_START_PAUSE,
  END_SUCCESS,
  END_UNSUCCESS,

  MENU_LOAD_SETTINGS,

  MENU_SETTINGS,
  MENU_SETTINGS_TURNS,
  MENU_SETTINGS_COIL,
  MENU_SETTINGS_WIRE,

  MENU_INFO

  // *********************************** TO ASK
};
menu_state curr_menu = MENU_MAIN;
int selected_item = 0;
int top_index = 0;



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
};

void show(){
  lcd.clear();

  switch(curr_menu){
    
    case MENU_MAIN:
      for (int i = 0; i < 2; i++){
        int ind= top_index + i;
        if (ind >= main_menu_item_lenght){
          break;
        }
        lcd.setCursor(0 , i);
        if (ind == selected_item){
          lcd.print(">");
        }
        else{
          lcd.print(" ");
        }
        lcd.print(main_menu_item[ind]);
      }
      break;

    case MENU_START:
      lcd.setCursor(0,2);
      lcd.print("WINDING...");
      lcd.setCursor(1, 0);
      lcd.print("PAUSE");
      lcd.setCursor(1, 8);
      lcd.print("ESC");
      break;
      
    case MENU_START_PAUSE:
      lcd.setCursor(0,2);
      lcd.print("PAUSED");
      lcd.setCursor(1, 0);
      lcd.print("PAUSE");
      lcd.setCursor(1, 8);
      lcd.print("ESC");
      break;

    case END_SUCCESS:
    case END_UNSUCCESS:

    case MENU_LOAD_SETTINGS:
    case MENU_SETTINGS:
      for(int i=0;i<2;i++){
        int ind = top_index + i;
        if (ind >= settings_item_menue_lenght){
          break;
        }
        lcd.setCursor(0,i);
        if (ind==selected_item){
          lcd.print(">");
        }
        else{
          lcd.print(" ");
        }
        lcd.print(settings_item_menue[ind]);
      }
      break;
    case MENU_SETTINGS_TURNS:
    case MENU_SETTINGS_COIL:
    case MENU_SETTINGS_WIRE:
    case MENU_INFO:
      break;

    
  }


}





void setup() {
  lcd.createChar(3, Check);
  lcd.setCursor(0, 1);
  lcd.write(3);
}


void loop() {


}
