#include <PS2Keyboard.h>
#include <Keyboard.h>
#include <LiquidCrystal.h>
#include "sha256.c"

const int DataPin = 2;
const int IRQpin =  3;

const int SwitchPin = 8;

bool outputState = false;

PS2Keyboard keyboard;

const int rs = 10, en = 16, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

char input[64];
char account[64];

int len = 0;

int state = 0; //0 for "input account" and 1 for "input password"

void setup() {
  delay(1000);
  Serial.begin(9600);
  keyboard.begin(DataPin, IRQpin);
  Keyboard.begin();

  pinMode(SwitchPin, INPUT);

  lcd.begin(16,2);

  dispPrompt();
}

void loop() {
  if (keyboard.available()) {
    // read the next key
    char c = keyboard.read();
    if (digitalRead(SwitchPin) == HIGH) {
      dispPrompt();
      state = 2;
      KWrite(c);
    } else {
      // This is the password input mode
      if (c == PS2_ENTER){
        if(state == 0){ //account name being entered
          input[len] = '\0';
          strcpy(account, input);
          clearInput();
          len = 0;
          state = 1;
          dispPrompt();
        } else if (state == 1){ //password name being entered
          input[len] = '\0';
          uint8_t hash[32];
          char hash_in[128] = {0};
          // Serial.println(account);
          // Serial.println(input);
          strcat(hash_in, account);
          strcat(hash_in, input);
          calc_sha_256(hash, hash_in, len);
          for(int i = 0; i < 32; i++){
            Serial.print(hash[i]);
          }
          clearInput();
          len = 0;
          Serial.println();
          state = 0;
          dispPrompt();
        }
      } else if(c == PS2_DELETE){
        if(len > 0){len--;}
        input[len] = '\0';
        dispPrompt();
      } else {
        input[len] = c;
        len++;
        dispPrompt();
      }
    }
  }
}

void KWrite(char c) {
  if (c == PS2_ENTER) {
    Keyboard.write(KEY_RETURN);
  } else if (c == PS2_TAB) {
    Keyboard.write(KEY_TAB);
  } else if (c == PS2_ESC) {
    Keyboard.write(KEY_ESC);
  } else if (c == PS2_PAGEDOWN) {
    Keyboard.write(KEY_PAGE_DOWN);
  } else if (c == PS2_PAGEUP) {
    Keyboard.write(KEY_PAGE_UP);
  } else if (c == PS2_LEFTARROW) {
    Keyboard.write(KEY_LEFT_ARROW);
  } else if (c == PS2_RIGHTARROW) {
    Keyboard.write(KEY_RIGHT_ARROW);
  } else if (c == PS2_UPARROW) {
    Keyboard.write(KEY_UP_ARROW);
  } else if (c == PS2_DOWNARROW) {
    Keyboard.write(KEY_DOWN_ARROW);
  } else if (c == PS2_DELETE) {
    Keyboard.write(KEY_BACKSPACE);
  } else {
    // otherwise, just send the normal character
    Keyboard.write(c);
  }
}

void dispPrompt(){
  lcd.clear();
  if (state == 0){
    lcd.setCursor(0,0); 
    lcd.print("Account Name:");
    lcd.setCursor(0,1); 
    lcd.print(input);
  } else if (state == 1){
    String stars = "";
    for (int x=0; x < len; x++){
      stars += "*";
    }
    lcd.setCursor(0,0); 
    lcd.print("Password:");
    lcd.setCursor(0,1); 
    lcd.print(stars);
  } else if (state == 2){
    lcd.setCursor(0,0); 
    lcd.print("Pass Through");
  }
}

void clearInput(){
  for(int x = 0; x < sizeof(input) / sizeof(input[0]); x++){
    input[x] = '\0';
  }
}