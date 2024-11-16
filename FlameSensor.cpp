// FlameSensor.cpp
#include "FlameSensor.h"

FlameSensor::FlameSensor(int pin, int threshold, unsigned long debounceDelay)
  : _pin(pin), _threshold(threshold), _debounceDelay(debounceDelay), _lastFlameOutTime(0), _noDebounce(true) {
  pinMode(_pin, INPUT);
}

bool FlameSensor::flameDetected() {  // only false if flame out duration > debounce delay
  unsigned int _sensorValue = analogRead(_pin);
  //Serial.println(analogRead(sensorValue));
  if (_sensorValue < _threshold) {  //flame on
    Serial.print("Sensor Value: ");
    Serial.println(_sensorValue);
    _lastFlameOutTime = 0;  // setup debounce logic
    _noDebounce = false;    // check for debounce after first flame
    return true;
  }
  if (_noDebounce) {
    return false;  // skip debounce if first run
  }
  if (_lastFlameOutTime == 0) {  // start debouncing
    Serial.println("Start debounce sequence!");
    _lastFlameOutTime = millis();
  }
  unsigned long _outDuration = millis() - _lastFlameOutTime;
  if (_outDuration >= _debounceDelay) {
    Serial.println("Debouncing finished!");
    _noDebounce = true;
    return false;
  }
  return true;
}
