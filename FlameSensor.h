// FlameSensor.h
#ifndef FLAMESENSOR_H
#define FLAMESENSOR_H

#include <Arduino.h>

class FlameSensor {
public:
    FlameSensor(int pin, int threshold, unsigned long debounceDelay);
    bool flameDetected();

private:
    int _pin;
    int _threshold;
    unsigned long _debounceDelay;
    unsigned long _lastFlameOutTime;
    bool _noDebounce;
};

#endif
