#include <PS2Keyboard.h>
#include <Keyboard.h>

const int DataPin = 4;
const int IRQpin =  3;

const int SwitchPin = 8;

bool outputState = false;

PS2Keyboard keyboard;

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

void setup() {
  delay(1000);
  keyboard.begin(DataPin, IRQpin);
  Keyboard.begin();

  pinMode(SwitchPin, INPUT);
}

void loop() {
  if (keyboard.available()) {
    // read the next key
    char c = keyboard.read();
    
    if (digitalRead(SwitchPin) == HIGH) {
      KWrite(c);
    } else {
      Keyboard.write(KEY_RETURN);
    }

    
  }
}
