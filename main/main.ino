#include <PS2Keyboard.h>
#include <Keyboard.h>
#include <LiquidCrystal_I2C.h>

const int DataPin = 6;
const int IRQpin =  3;

const int SwitchPin = 8;

bool outputState = false;

PS2Keyboard keyboard;
LiquidCrystal_I2C lcd(0x20,16,2);

uint8_t salt[32] = { 
  0x22, 0x3A, 0x6B, 0x5E, 0x4E, 0x01, 0x55, 0x11, 
  0xA0, 0x63, 0x82, 0x1E, 0x23, 0x9C, 0xE5, 0xF3, 
  0x5C, 0x17, 0x17, 0x2B, 0x39, 0x84, 0x0D, 0x92, 
  0xA0, 0xCC, 0xE2, 0xF6, 0xE4, 0x39, 0x21, 0xBC 
  }; // 256-bit salt

String input = "";

void setup() {
  delay(1000);
  Serial.begin(9600);
  keyboard.begin(DataPin, IRQpin);
  Keyboard.begin();

  pinMode(SwitchPin, INPUT);

  lcd.init();
  lcd.clear();         
  lcd.backlight();
}

void loop() {
  if (keyboard.available()) {
    // read the next key
    char c = keyboard.read();
    if (digitalRead(SwitchPin) == HIGH) {
      KWrite(c);
    } else {
      // This is the password input mode
      if (c == PS2_ENTER){
        String hash = hashFunction(salt, input);
        Serial.print(hash);
        input = "";
        lcd.clear();
      } else {
        input += c;
        LCDStars();
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

String hashFunction(uint8_t salt[32], String message) {
  uint32_t hash[8];
  for (int i = 0; i < 8; i++) {
    hash[i] = salt[i * 4] << 24 | salt[i * 4 + 1] << 16 | salt[i * 4 + 2] << 8 | salt[i * 4 + 3];
  }
  
  // Hash the message
  for (int i = 0; i < message.length(); i++) {
    uint32_t rotate = (i % 32);
    uint32_t temp = ((uint32_t) message[i]) << rotate | ((uint32_t) message[i]) >> (32 - rotate);
    int index = i % 8;
    hash[index] ^= temp;
  }
  
  // Concatenate the hash values as a string
  String result;
  for (int i = 0; i < 8; i++) {
    result += String(hash[i], HEX);
  }

  return result;
}

void LCDStars(){
//prints stars on bottom line for each char in input
  lcd.clear();
  String stars = "";
  for (int x=0; x < input.length(); x++){
    stars += "*";
  }
  lcd.setCursor(0,1); 
  lcd.print(stars);
}



