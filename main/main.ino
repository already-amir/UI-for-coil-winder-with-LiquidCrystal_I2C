#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define up     12
#define down   13
#define ok     27
#define back   14


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

byte github[] = {
  B01001,
  B01111,
  B01111,
  B01111,
  B00110,
  B10110,
  B01110,
  B00110
 
  
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

  SAVE_SUCCESS,
  SAVE_UNSUCCESS,

  MENU_INFO,

  MENU_ZERO_STATE

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
    int bios=8*last_save;
    EEPROM.put(bios + 0,turns);
    EEPROM.put(bios + 4,coil_width);
    EEPROM.put(bios + 8,wire_width);
    last_save ++;
  }
  // *********************************** TO DO 
}

void load_setings(int save_num){
  int bios= 8 * save_num;
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

const int main_menu_item_lenght = 5;
const char* main_menu_item[]={
  "START",
  "LOAD SETTINGS",
  "SETTING",
  "ZERO STATE",
  "INFO"
};


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
      lcd.setCursor(3,0);
      lcd.print("WINDING...");
      lcd.setCursor(0, 1);
      lcd.print("PAUSE");
      lcd.setCursor(13, 1);
      lcd.print("ESC");
      break;
      
    case MENU_START_PAUSE:
      lcd.setCursor(5,0);
      lcd.print("PAUSED");
      lcd.setCursor(0, 1);
      lcd.print("CONTINUE");
      lcd.setCursor(13, 1);
      lcd.print("ESC");
      break;

    case END_SUCCESS:
      lcd.setCursor(3,0);
      lcd.print("WINDING DONE");
      lcd.setCursor(4,1);
      lcd.print("SUCCESSFULY");
      break;

    case END_UNSUCCESS:
      lcd.setCursor(0,0);
      lcd.print("WINDING WAS NOT");
      lcd.setCursor(3,1);
      lcd.print("SUCCESSFUL");
      break;
    case MENU_LOAD_SETTINGS:
      for (int i = 0; i < 2; i++){
        int ind= top_index + i;
        if (ind >= max_save){
          break;
        }
        lcd.setCursor(0 , i);
        if (ind == selected_item){
          lcd.print(">");
        }
        else{
          lcd.print(" ");
        }
        lcd.print(load_item[ind]);
      }
      break;

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
      lcd.setCursor(0, 0);
      lcd.print("TURNS: ");
      lcd.print(turns);
      lcd.setCursor(0, 1);
      lcd.print("OK");
      lcd.setCursor(8, 1);
      lcd.print("BACK");
      break;

    case MENU_SETTINGS_COIL:
      lcd.setCursor(0, 0);
      lcd.print("COIL WIDTH: ");
      lcd.print(coil_width,1);
      lcd.setCursor(0, 1);
      lcd.print("OK");
      lcd.setCursor(8, 1);
      lcd.print("BACK");
      break;

    case MENU_SETTINGS_WIRE:
      lcd.setCursor(0, 0);
      lcd.print("WIRE WIDTH: ");
      lcd.print(wire_width,2);
      lcd.setCursor(0, 1);
      lcd.print("OK");
      lcd.setCursor(8, 1);
      lcd.print("BACK");
      break;

    case SAVE_SUCCESS:
      lcd.setCursor(3,0);
      lcd.print("SAVE SUCCESSFULY");
      break;
    case SAVE_UNSUCCESS:
      lcd.setCursor(3,0);
      lcd.print("SAVE WAS UNSUCCESSFUL");
      break;

    case MENU_INFO:
      lcd.setCursor(0, 0);
      lcd.print("COIL WINDER V1.0");
      lcd.setCursor(0, 1);
      lcd.print("BY ALI ");
      break;

    case MENU_ZERO_STATE:
      lcd.setCursor(0, 0);
      lcd.print("STATE ZERO");
      break;

  }
}





void setup() {

  lcd.init();
  lcd.backlight();

  pinMode(up, INPUT_PULLUP);
  pinMode(down, INPUT_PULLUP);
  pinMode(ok, INPUT_PULLUP);
  pinMode(back, INPUT_PULLUP);

  EEPROM.begin(64);
  show();

  //lcd.createChar(3, Check);
  //lcd.setCursor(0, 1);
  //lcd.write(3);
}


void upp(){
  if (selected_item>0){
    selected_item--;
    if (selected_item<top_index){
      top_index--;
    }
    show();
  }
}

void downn(const int num){
  if (selected_item<num-1){
    selected_item++;
    if (selected_item>top_index+1){
      top_index++;
    }
    show();
  }
}

void loop() {

  if (curr_menu==MENU_MAIN){
    if (is_button_pressed(up)){
      upp();
    }
    if (is_button_pressed(down)){
      downn(main_menu_item_lenght);
    }
    if(is_button_pressed(ok)){
      switch (selected_item){
        case 0: curr_menu=MENU_START; break;
        case 1: curr_menu=MENU_LOAD_SETTINGS;break;
        case 2: curr_menu=MENU_SETTINGS;break;
        case 3: curr_menu=MENU_ZERO_STATE;break;
        case 4: curr_menu=MENU_INFO; break;
        
      }
      selected_item=0;
      top_index=0;
      show();
    }
  }

  else if (curr_menu== MENU_START){
    
    if(is_button_pressed(ok)){
      // **************************************** TODO pause prep
      curr_menu=MENU_START_PAUSE;
      show();
    }
    if(is_button_pressed(back)){
      // ***************************************8 TODO terminate
      curr_menu=END_UNSUCCESS;
      show();
    }
    
    //******************************************* TODO  winding
  }
  else if (curr_menu== MENU_START_PAUSE){
    
    if(is_button_pressed(ok)){
      // **************************************** TODO continue prep
      curr_menu=MENU_START;
      show();
    }
    if(is_button_pressed(back)){
      //**************************************** TODO terminate
      curr_menu=END_UNSUCCESS;
      show();
    }
    // **************************************** TODO pause
    
  }
  else if (curr_menu== END_SUCCESS){
  
    if(is_button_pressed(ok)||is_button_pressed(back)){
      curr_menu=MENU_MAIN;
      show();
    }
  }
  else if (curr_menu== END_UNSUCCESS){
    
    if(is_button_pressed(ok)||is_button_pressed(back)){
      curr_menu=MENU_MAIN;
      show();
    }
    //**************************************** TODO esc
    
  }
  else if (curr_menu== MENU_LOAD_SETTINGS){
    if (is_button_pressed(up)){

    }
    if (is_button_pressed(down)){

    }
    if(is_button_pressed(ok)){

    }
    if(is_button_pressed(back)){
      
    }
    
  }
  else if (curr_menu== MENU_SETTINGS){
    if (is_button_pressed(up)){

    }
    if (is_button_pressed(down)){

    }
    if(is_button_pressed(ok)){

    }
    if(is_button_pressed(back)){
      
    }
    
  }
  else if (curr_menu== MENU_SETTINGS_TURNS){
    if (is_button_pressed(up)){

    }
    if (is_button_pressed(down)){

    }
    if(is_button_pressed(ok)){

    }
    if(is_button_pressed(back)){
      
    }

  }
  else if (curr_menu== MENU_SETTINGS_COIL){
    if (is_button_pressed(up)){

    }
    if (is_button_pressed(down)){

    }
    if(is_button_pressed(ok)){

    }
    if(is_button_pressed(back)){
      
    }
    
  }
  else if (curr_menu== MENU_SETTINGS_WIRE){
    if (is_button_pressed(up)){

    }
    if (is_button_pressed(down)){

    }
    if(is_button_pressed(ok)){

    }
    if(is_button_pressed(back)){
      
    }
    
  }
  else if (curr_menu== SAVE_SUCCESS){
    if (is_button_pressed(up)){

    }
    if (is_button_pressed(down)){

    }
    if(is_button_pressed(ok)){

    }
    if(is_button_pressed(back)){
      
    }
    
  }
  else if (curr_menu== SAVE_UNSUCCESS){
    if (is_button_pressed(up)){

    }
    if (is_button_pressed(down)){

    }
    if(is_button_pressed(ok)){

    }
    if(is_button_pressed(back)){
      
    }
    
  }
  else if (curr_menu== MENU_INFO){
    if (is_button_pressed(up)){

    }
    if (is_button_pressed(down)){

    }
    if(is_button_pressed(ok)){

    }
    if(is_button_pressed(back)){
      
    }
    
  }
  else if (curr_menu== MENU_ZERO_STATE){
    if (is_button_pressed(up)){

    }
    if (is_button_pressed(down)){

    }
    if(is_button_pressed(ok)){

    }
    if(is_button_pressed(back)){
      
    }
    
  }

  
  
  


}
