#ifndef KEYPADHANDLER_H
#define KEYPADHANDLER_H

#include <Keypad.h>

class KeypadHandler {
public:
    KeypadHandler();
    char getKey();

private:
    // Configuration for the keypad rows and columns
    static const byte ROWS = 1;
    static const byte COLS = 4;
    char keys[ROWS][COLS] = {
        { '-', '1', '5', '0' }
    };   
    byte rowPins[ROWS] = { D7 };
    byte colPins[COLS] = { D1, D2, D5, D6 };

    // Keypad object
    Keypad keypad;
};

#endif
