#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

#define up     12
#define down   13
#define ok     27
#define back   14

#define lcd_l  16
#define lcd_h  2

#define max_save  25

int selected_item = 0;
int top_index = 0;

int turns =  1000;
float coil_width = 20.0;
float wire_width = 0.25;
// temps
int temp_turns;
float temp_coil_width;
float temp_wire_width;

int changing_save=0;

bool winding_active=false;

unsigned long last_sysmsg_time=0;

LiquidCrystal_I2C lcd(0x27, lcd_l, lcd_h);


struct save_stng{
  char s_name[lcd_l];
  int s_turn;
  float s_coil;
  float s_wir;
};
save_stng saves[max_save];


enum menu_state {
  MENU_MAIN,
  MENU_START,
  MENU_START_PAUSE,
  END_SUCCESS,
  END_UNSUCCESS,
  SAVE_SUCCESS,
  MENU_LOAD_SETTINGS,
  MENU_SETTINGS,
  MENU_LOAD_SETTINGS_SUCCESSFUL,
  MENU_SETTINGS_TURNS,
  MENU_SETTINGS_COIL,
  MENU_SETTINGS_WIRE,
  
  MENU_INFO,
  MENU_ZERO_STATE,
  MENU_LOAD_SETTINGS_SELECT,
  MENU_LOAD_SETTINGS_SELECT_DETAIL
  
};
menu_state curr_menu = MENU_MAIN;


const int main_menu_item_lenght = 5;
const char* main_menu_item[]={
  "START",
  "LOAD SETTINGS",
  "SETTING",
  "STATE ZERO",
  "INFO"
};


const int settings_item_menue_lenght=4;
const char* settings_item_menue[]={
  "TURNS",
  "COIL WIDTH",
  "WIRE WIDTH",
  "SAVE TO EEPROM"
};

const int load_settings_select_lenght=3;
const char* load_settings_select[]={
  "LOAD",
  "EDIT",
  "DETAIL",
};

// load all settings
void load_all_settings(){
  for (int i=0;i<max_save;i++){
    int address=i*sizeof(save_stng);
    EEPROM.get(address,saves[i]);
  }
}

//__________________run only once
// returns all the setting to defalt (0)
void zero_saves(){
  for (int i=0;i<max_save;i++){
    int address=i*sizeof(save_stng);
    save_stng temp;
    sprintf(temp.s_name, "save %d", i);
    temp.s_turn=0;
    temp.s_coil=0.0f;
    temp.s_wir=0.0f;
    EEPROM.put(address,temp);
  }
  EEPROM.commit();
}

// save only one setting
void save_settings(int index){
  int address=index*sizeof(save_stng);
  save_stng temp;
  temp.s_coil=coil_width;
  temp.s_turn=turns;
  temp.s_wir=wire_width;
  sprintf(temp.s_name, "save %d", index);
  EEPROM.put(address,temp);
  EEPROM.commit();
  load_all_settings();
}

// to copy data from saves to variables
void update_setting(int index){

  turns=saves[index].s_turn;
  coil_width=saves[index].s_coil;
  wire_width=saves[index].s_wir;

}
// is butten pressed + debunce 
unsigned long last_press = 0;
bool is_button_pressed(int pin){
  if (digitalRead(pin)==LOW && millis() - last_press > 200){
    last_press=millis();
    return true;
  }
  return false;
}

// what to show in each state
void show(){
  lcd.clear();

  switch(curr_menu){
    
    case MENU_MAIN:
      for (int i = 0; i < lcd_h; i++){
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
      lcd.setCursor((lcd_l-10)/2,0);
      lcd.print("WINDING...");
      lcd.setCursor(0, 1);
      lcd.print("PAUSE");
      lcd.setCursor(lcd_l-3, 1);
      lcd.print("ESC");
      break;
      
    case MENU_START_PAUSE:
      lcd.setCursor((lcd_l-6)/2,0);
      lcd.print("PAUSED");
      lcd.setCursor(0, 1);
      lcd.print("CONTINUE");
      lcd.setCursor(lcd_l-3, 1);
      lcd.print("ESC");
      break;

    case END_SUCCESS:
      lcd.setCursor((lcd_l-12)/2,0);
      lcd.print("WINDING DONE");
      lcd.setCursor((lcd_l-12)/2,1);
      lcd.print("SUCCESSFULY");
      break;

    case END_UNSUCCESS:
      lcd.setCursor((lcd_l-16)/2,0);
      lcd.print("WINDING WAS NOT");
      lcd.setCursor((lcd_l-12)/2,1);
      lcd.print("SUCCESSFUL");
      break;
      //...........................................
    case SAVE_SUCCESS:
      lcd.setCursor((lcd_l-12)/2,0);
      lcd.print("SAVED");
      lcd.setCursor((lcd_l-12)/2,1);
      lcd.print("SUCCESSFULY");
      break;

    
    case MENU_LOAD_SETTINGS:
      for (int i = 0; i < lcd_h; i++){
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
        lcd.print(String(ind));
        lcd.print("  ");
        lcd.print(saves[ind].s_turn);
        lcd.print("|");
        lcd.print(saves[ind].s_coil);
        lcd.print("|");
        lcd.print(saves[ind].s_wir);
      }
      break;

    case MENU_SETTINGS:
      for(int i=0;i<lcd_h;i++){
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

    case MENU_LOAD_SETTINGS_SUCCESSFUL:
      lcd.setCursor((lcd_l-12)/2, 0);
      lcd.print("SAVE LOADED");
      lcd.setCursor((lcd_l-12)/2, 1);
      lcd.print("SUCCESSFULLY");
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

    
    case MENU_INFO:
      lcd.setCursor((lcd_l-16)/2, 0);
      lcd.print("COIL WINDER V1.1");
      lcd.setCursor((lcd_l-12)/2, 1);
      lcd.print("BY ALI & AMIR");
      break;

    case MENU_ZERO_STATE:
      lcd.setCursor((lcd_l-10)/2, 0);
      lcd.print("STATE ZERO");
      lcd.setCursor(0, 1);
      lcd.print("<--");
      lcd.setCursor(lcd_l-3, 1);
      lcd.print("-->");
      break;

    case MENU_LOAD_SETTINGS_SELECT:
      for(int i=0;i<lcd_h;i++){
        int ind = top_index + i;
        if (ind >= load_settings_select_lenght){
          break;
        }
        lcd.setCursor(0,i);
        if (ind==selected_item){
          lcd.print(">");
        }
        else{
          lcd.print(" ");
        }
        lcd.print(load_settings_select[ind]);
      }
      break;

    case MENU_LOAD_SETTINGS_SELECT_DETAIL:
      lcd.setCursor(0, 0);
      lcd.print("T: ");
      lcd.print(saves[changing_save].s_turn);
      lcd.setCursor(8, 0);
      lcd.print("C: ");
      lcd.print(saves[changing_save].s_coil);
      lcd.setCursor(0, 1);
      lcd.print("W: ");
      lcd.print(saves[changing_save].s_wir);
      lcd.setCursor(8, 1);
      lcd.print("OK");
      break;


  }
}




// up key
void up_key(){
  if (selected_item>0){
    selected_item--;
    if (selected_item<top_index){
      top_index--;
    }
    show();
  }
}

// down key
void down_key(const int num){
  if (selected_item<num-1){
    selected_item++;
    if (selected_item>top_index+1){
      top_index++;
    }
    show();
  }
}


// TODO motor settings
unsigned long last_step_time = 0;
int current_turn = 0;

void winding_routine(){
  if (!winding_active){
    return;
  }
  
  if (millis() - last_step_time >= 5) { 
    last_step_time = millis();

    //******************************************* TODO  winding

    current_turn++;
    if (current_turn >= turns) {
      winding_active = false;
      last_sysmsg_time=millis();
      curr_menu = END_SUCCESS;
      current_turn=0;
      show();
    }
  }
}


void setup() {

  lcd.init();
  lcd.backlight();

  pinMode(up, INPUT_PULLUP);
  pinMode(down, INPUT_PULLUP);
  pinMode(ok, INPUT_PULLUP);
  pinMode(back, INPUT_PULLUP);
  
  EEPROM.begin(256);
  // only run it once
  //zero_saves();
  load_all_settings();
  show();
}

void loop() {

  if (curr_menu==MENU_MAIN){
    if (is_button_pressed(up)){
      up_key();
    }
    if (is_button_pressed(down)){
      down_key(main_menu_item_lenght);
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

    winding_active=true;
    winding_routine();

    if(is_button_pressed(ok)){
      winding_active=false;
      curr_menu=MENU_START_PAUSE;
      show();
    }
    if(is_button_pressed(back)){
      winding_active=false;
      curr_menu=END_UNSUCCESS;
      last_sysmsg_time=millis();
      show();
    }
    
  }
  else if (curr_menu== MENU_START_PAUSE){
    
    if(is_button_pressed(ok)){
      curr_menu=MENU_START;
      show();
    }
    if(is_button_pressed(back)){
      last_sysmsg_time=millis();
      curr_menu=END_UNSUCCESS;
      show();
    }
  }
  else if (curr_menu == END_SUCCESS || curr_menu == END_UNSUCCESS  ){
  
    if(is_button_pressed(ok)||is_button_pressed(back)||millis()-last_sysmsg_time>5000){
      curr_menu=MENU_MAIN;
      selected_item=0;
      top_index=0;
      show();
    }
  }
  else if (curr_menu==SAVE_SUCCESS ){
  
    if(is_button_pressed(ok)||is_button_pressed(back)||millis()-last_sysmsg_time>5000){
      curr_menu=MENU_SETTINGS;
      show();
    }
  }
  else if (curr_menu== MENU_LOAD_SETTINGS){

    if (is_button_pressed(up)){
      up_key();
    }
    if (is_button_pressed(down)){
      down_key(max_save);
    }
    if(is_button_pressed(ok)){
      changing_save=selected_item;
      curr_menu=MENU_LOAD_SETTINGS_SELECT;
      selected_item=0;
      top_index=0;
      show();
    }
    if(is_button_pressed(back)){
      curr_menu=MENU_MAIN;
      selected_item=1;
      top_index=0;
      show();
    }
  }
  else if (curr_menu== MENU_LOAD_SETTINGS_SELECT){

    if (is_button_pressed(up)){
      up_key();
    }
    if (is_button_pressed(down)){
      down_key(load_settings_select_lenght);
    }
    if(is_button_pressed(ok)){
      switch (selected_item){
        case 0:
          update_setting(changing_save);
          last_sysmsg_time=millis(); 
          curr_menu=MENU_LOAD_SETTINGS_SUCCESSFUL;
          break;
        case 1:
          curr_menu=MENU_SETTINGS;
          break;
        case 2:
          curr_menu=MENU_LOAD_SETTINGS_SELECT_DETAIL;
          break;
      }
      selected_item=0;
      top_index=0;
      show();
    }
    if(is_button_pressed(back)){
      selected_item=changing_save;
      top_index=max(changing_save-1,0);
      curr_menu=MENU_LOAD_SETTINGS;
      show();
    }
  }
  else if (curr_menu == MENU_LOAD_SETTINGS_SELECT_DETAIL){
  
    if(is_button_pressed(ok)||is_button_pressed(back)){
      curr_menu=MENU_LOAD_SETTINGS_SELECT;
      selected_item=2;
      top_index=1;
      show();
    }
  }
  else if (curr_menu == MENU_LOAD_SETTINGS_SUCCESSFUL){
  
    if(is_button_pressed(ok)||is_button_pressed(back)||millis()-last_sysmsg_time>5000){
      curr_menu=MENU_LOAD_SETTINGS;
      show();
    }
  }
  else if (curr_menu== MENU_SETTINGS){
    if (is_button_pressed(up)){
      up_key();
    }
    if (is_button_pressed(down)){
      down_key(settings_item_menue_lenght);
    }
    if(is_button_pressed(ok)){
      switch (selected_item){
        case 0: curr_menu=MENU_SETTINGS_TURNS;temp_turns=turns; break;
        case 1: curr_menu=MENU_SETTINGS_COIL;temp_coil_width=coil_width; break;
        case 2: curr_menu=MENU_SETTINGS_WIRE;temp_wire_width=wire_width; break;
        case 3: curr_menu=SAVE_SUCCESS;save_settings(changing_save);last_sysmsg_time=millis();break;
      }
      show();
    }
    if (is_button_pressed(back)){
      curr_menu=MENU_MAIN;
      selected_item=2;
      top_index=1;
      show();
    }  
  }
  else if (curr_menu== MENU_SETTINGS_TURNS){
    if (is_button_pressed(up)){
      turns+=10;
      show();
    }
    if (is_button_pressed(down)){
      turns-=10;
      if (turns<0){
        turns=0;
      }
      show();
    }
    if(is_button_pressed(ok)){
      curr_menu=MENU_SETTINGS;
      show();
    }
    if(is_button_pressed(back)){
      turns=temp_turns;
      curr_menu=MENU_SETTINGS;
      show();
    }
  }
  else if (curr_menu== MENU_SETTINGS_COIL){
    if (is_button_pressed(up)){
      coil_width+=0.5;
      show();
    }
    if (is_button_pressed(down)){
      coil_width-=0.5;
      if (coil_width<0){
        coil_width=0.0f;
      }
      show();
    }
    if(is_button_pressed(ok)){
      curr_menu=MENU_SETTINGS;
      show();
    }
    if(is_button_pressed(back)){
      coil_width=temp_coil_width;
      curr_menu=MENU_SETTINGS;
      show();
    }
  }
  else if (curr_menu== MENU_SETTINGS_WIRE){
    if (is_button_pressed(up)){
      wire_width+=0.01;
      show();
    }
    if (is_button_pressed(down)){
      wire_width-=0.01;
      if (wire_width<0){
        wire_width=0.0f;
      }
      show();
    }
    if(is_button_pressed(ok)){
      curr_menu=MENU_SETTINGS;
      show();
    }
    if(is_button_pressed(back)){
      wire_width=temp_wire_width;
      curr_menu=MENU_SETTINGS;
      show();
    }
  }
  else if (curr_menu== MENU_INFO){
    
    if(is_button_pressed(back)||is_button_pressed(ok)){
      curr_menu=MENU_MAIN;
      top_index=3;
      selected_item=4;
      show();
    }
  
  }
  else if (curr_menu== MENU_ZERO_STATE){
    if (is_button_pressed(up)){
      //********************************* manual left
    }
    if (is_button_pressed(down)){
      //********************************* manual right
    }
    if(is_button_pressed(ok)){
      //********************************* auto state zero
    }
    if(is_button_pressed(back)){
      curr_menu=MENU_MAIN;
      selected_item=3;
      top_index=3;
      show();
    }
  }
}
