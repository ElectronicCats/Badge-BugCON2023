#ifndef BUTTONS_H
#define BUTTONS_H

#include <ezButton.h>

#include "hardware.h"

#if defined(ESP32) || defined(ESP32_S3)
#include "ezTouch.h"
#endif

#define BUTTON_LEFT 27   // GPIO27
#define BUTTON_RIGHT 26  // GPIO26
#define TOUCH_LEFT 14    // GPIO14
#define TOUCH_RIGHT 12   // GPIO12
#define DEBOUNCE_TIME_MS 50

#ifdef RP2040
ezButton buttonLeft(BUTTON_LEFT);
ezButton buttonRight(BUTTON_RIGHT);
#else
ezTouch buttonLeft(TOUCH_LEFT);
ezTouch buttonRight(TOUCH_RIGHT);
#endif

#endif