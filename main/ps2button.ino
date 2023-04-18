#include <PS2Keyboard.h>
#include <Keyboard.h>
#include <LiquidCrystal_I2C.h>

PS2Keyboard keyboard;
LiquidCrystal_I2C lcd(0x20, 16, 2);

#define TOGGLE_SWITCH_PIN 5
#define LED_PIN 6

const int DataPin = 4;
const int IRQpin =  3;

String* accountNames = new String[10];
bool toggleSwitchStatePrev = LOW;
bool isMode1 = true;
int currentScreen = 0;
char x;
char lastKey;
int currentAccount = 0;
int numAccounts = 0; 
String newAccountName = "";
String input = "";

uint8_t salt[32] = { 
  0x22, 0x3A, 0x6B, 0x5E, 0x4E, 0x01, 0x55, 0x11, 
  0xA0, 0x63, 0x82, 0x1E, 0x23, 0x9C, 0xE5, 0xF3, 
  0x5C, 0x17, 0x17, 0x2B, 0x39, 0x84, 0x0D, 0x92, 
  0xA0, 0xCC, 0xE2, 0xF7, 0xE4, 0x39, 0x21, 0xBC 
  }; // 256-bit salt

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
  String stars = "";
  for (int x=0; x < input.length(); x++){
    stars += "*";
  }
  lcd.setCursor(0,1); 
  lcd.print(stars);
}

void ProcessInput(){
    String hash = hashFunction(salt, input);
    for(int x = 0; x < hash.length(); x ++){
         KWrite(hash[x]);
    }
    Serial.println(hash);
}

void setup() {
  pinMode(TOGGLE_SWITCH_PIN, INPUT_PULLUP); // Set the toggle switch pin as input and enable the internal pull-up resistor
  pinMode(LED_PIN, OUTPUT); 
  lcd.init(); // initialize the lcd
  lcd.backlight();
  Serial.begin(9600); // Start the serial communication
  keyboard.begin(DataPin, IRQpin);
}

void loop() {
  bool toggleSwitchState = digitalRead(TOGGLE_SWITCH_PIN);
  if (toggleSwitchState != toggleSwitchStatePrev && toggleSwitchState == LOW) { // If the toggle switch is toggled
    isMode1 = !isMode1; // Toggle the mode flag
    if (isMode1) {
      digitalWrite(LED_PIN, LOW);
      lcd.noDisplay();
    } else {
      digitalWrite(LED_PIN, HIGH);
      lcd.display(); // turn on the display in mode2
      lcd.clear(); // clear the display
      lcd.setCursor(0, 0);
      lcd.print("Select account:");
      lcd.setCursor(0, 1);
        if (numAccounts == 0) {
          lcd.print("*No Accounts*");
        } else {
          lcd.print(accountNames[currentAccount]);
       }
    }
  }
  toggleSwitchStatePrev = toggleSwitchState; 
  
  if (isMode1 && keyboard.available()) {
    char c = keyboard.read(); 
    KWrite(c);
  } else if (!isMode1 && keyboard.available()) {
      x = keyboard.read();
      lastKey=x;
      if (x == PS2_UPARROW) { 
        if (currentScreen == 0 && numAccounts > 0){
          lcd.clear();
          lcd.setCursor(0, 1);
          lcd.print("                ");
          currentAccount = (currentAccount + numAccounts - 1) % numAccounts; 
        }
    } else if (x == PS2_DOWNARROW) { 
      if (currentScreen == 0 && numAccounts > 0){ 
        lcd.setCursor(0, 1);
        lcd.print("                ");
        currentAccount = (currentAccount + 1) % numAccounts;  
      }
    } else if (x == PS2_LEFTARROW) { 
      lcd.clear();
      if (currentScreen == 0) {
        currentScreen = 1;
        newAccountName = "";
      } else {
        currentScreen = 0;
      }
    } else if (x == PS2_RIGHTARROW) { 
      lcd.clear();
      if (currentScreen == 1) {
        currentScreen = 0;
        newAccountName = "";
      } else {
        currentScreen = 1;
      }
    } else if (x == PS2_DELETE) { 
      if (newAccountName.length() > 0) {
        newAccountName = newAccountName.substring(0, newAccountName.length() - 1);
        lcd.clear(); // clear the display
        lcd.setCursor(0, 0);
        lcd.print("Create account:");
        lcd.setCursor(0, 1);
        lcd.print("Name: " + newAccountName + " ");
      }
    } else if (x == PS2_ENTER) { 
      if (currentScreen == 1 && newAccountName.length() > 0) {
        String* newAccountNames = new String[numAccounts + 1];
        for (int i = 0; i < numAccounts; i++) {
          newAccountNames[i] = accountNames[i];
        }
        newAccountNames[numAccounts] = newAccountName;
        delete[] accountNames;
        accountNames = newAccountNames;
        numAccounts++;
        lcd.clear();
        currentScreen = 0;
        newAccountName = "";
      } else if (currentScreen==0 && accountNames[currentAccount]) {
          currentScreen=3;
      } else if (currentScreen==3) {
        
      }
    } else if (x == PS2_TAB) { //use tab to delete accounts
      if (currentScreen == 0 && numAccounts > 0) {
        String* newAccountNames = new String[numAccounts - 1];
        for (int i = 0, j = 0; i < numAccounts; i++) {
          if (i != currentAccount) {
            newAccountNames[j] = accountNames[i];
            j++;
          }
        }
        delete[] accountNames;
        accountNames = newAccountNames;
        numAccounts--;
        if (currentAccount >= numAccounts) {
          currentAccount = numAccounts - 1;
        }
      }
    }
    if (currentScreen == 0) {
    lastKey = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Select account: ");
    lcd.setCursor(0, 1);
    if (numAccounts == 0) {
      lcd.print("*No Accounts*");
    } else {
      lcd.print(accountNames[currentAccount]);
    }
  } else if (currentScreen == 1) {
    lcd.setCursor(0, 0);
    lcd.print("Create account:");
    lcd.setCursor(0, 1);
    lcd.print("Name: ");
    if (lastKey >= ' ' && lastKey <= '~') {
      newAccountName += lastKey;
      if (lastKey != 0) {
        lcd.setCursor(0, 1);
        lcd.print("Name: " + newAccountName);
      }
    }
    lastKey = 0;
  } else if (currentScreen == 3) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter password: ");
    input += lastKey;
    Serial.println(input);
    LCDStars();
  }
  
  delay(100); 
  }
}
