#include "KeypadHandler.h"

// Constructor
KeypadHandler::KeypadHandler()
    : keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS) {
}

// Get the currently pressed key
char KeypadHandler::getKey() {
    return keypad.getKey();  // Use the Keypad library's getKey() method
}
