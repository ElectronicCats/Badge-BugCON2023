/*
 * Copyright (c) 2019, ArduinoGetStarted.com. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * - Neither the name of the ArduinoGetStarted.com nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ARDUINOGETSTARTED.COM "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ARDUINOGETSTARTED.COM BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include "ezTouch.h"

ezTouch::ezTouch(int pin) : ezTouch(pin, INPUT_PULLUP){};

ezTouch::ezTouch(int pin, int mode) {
  btnPin = pin;
  debounceTime = 0;
  count = 0;
  countMode = COUNT_FALLING;

  // pinMode(btnPin, mode);
	previousSteadyState = getTouchValue();
  lastSteadyState = previousSteadyState;
  lastFlickerableState = previousSteadyState;

  lastDebounceTime = 0;
}

int ezTouch::getTouchValue(void) {
#if defined(ESP32_DEVKIT)
  return touchRead(btnPin) < TOUCH_THRESHOLD;
#elif defined(ESP32_S3)
  if (btnPin == TOUCH_LEFT) {
    return touchRead(btnPin) > TOUCH_THRESHOLD + 10000;
  } else {
    return touchRead(btnPin) > TOUCH_THRESHOLD;
  }
#else
  return -1;
#endif
}

void ezTouch::setDebounceTime(unsigned long time) {
  debounceTime = time;
}

int ezTouch::getState(void) {
  return !lastSteadyState;
}

int ezTouch::getStateRaw(void) {
#if defined(ESP32_DEVKIT) || defined(ESP32_S3)
  return touchRead(btnPin);
#else
  return -1;
#endif
}

bool ezTouch::isPressed(void) {
  if (previousSteadyState == LOW && lastSteadyState == HIGH)
    return true;
  else
    return false;
}

bool ezTouch::isReleased(void) {
  if (previousSteadyState == HIGH && lastSteadyState == LOW)
    return true;
  else
    return false;
}

void ezTouch::setCountMode(int mode) {
  countMode = mode;
}

unsigned long ezTouch::getCount(void) {
  return count;
}

void ezTouch::resetCount(void) {
  count = 0;
}

void ezTouch::loop(void) {
  // read the state of the switch/button:
  int currentState = getTouchValue();
  unsigned long currentTime = millis();

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch/button changed, due to noise or pressing:
  if (currentState != lastFlickerableState) {
    // reset the debouncing timer
    lastDebounceTime = currentTime;
    // save the the last flickerable state
    lastFlickerableState = currentState;
  }

  if ((currentTime - lastDebounceTime) >= debounceTime) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // save the the steady state
    previousSteadyState = lastSteadyState;
    lastSteadyState = currentState;
  }

  if (previousSteadyState != lastSteadyState) {
    if (countMode == COUNT_BOTH)
      count++;
    else if (countMode == COUNT_FALLING) {
      if (previousSteadyState == HIGH && lastSteadyState == LOW)
        count++;
    } else if (countMode == COUNT_RISING) {
      if (previousSteadyState == LOW && lastSteadyState == HIGH)
        count++;
    }
  }
}
