#include <PS2Keyboard.h>

const int DataPin = 4;
const int IRQpin =  3;

PS2Keyboard keyboard;

void setup() {
  delay(1000);
  keyboard.begin(DataPin, IRQpin);
  Serial.begin(9600);
  Serial.println("Keyboard Test:");
}

void loop() {
  if (keyboard.available()) {
    
    // read the next key
    char c = keyboard.read();
    
    // check for some of the special keys
    if (c == PS2_ENTER) {
      Serial.println();
    } else if (c == PS2_TAB) {
      Serial.println("[Tab]");
    } else if (c == PS2_ESC) {
      Serial.println("[ESC]");
    } else if (c == PS2_PAGEDOWN) {
      Serial.println("[PgDn]");
    } else if (c == PS2_PAGEUP) {
      Serial.println("[PgUp]");
    } else if (c == PS2_LEFTARROW) {
      Serial.println("[Left]");
    } else if (c == PS2_RIGHTARROW) {
      Serial.println("[Right]");
    } else if (c == PS2_UPARROW) {
      Serial.println("[Up]");
    } else if (c == PS2_DOWNARROW) {
      Serial.println("[Down]");
    } else if (c == PS2_DELETE) {
      Serial.println("[Del]");
    } else {
      
      // otherwise, just print all normal characters
      Serial.println(c);
    }
  }
}
