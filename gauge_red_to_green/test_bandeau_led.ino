#include "FastLED.h"

#define NUM_LEDS 17
#define DATA_PIN 4
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
#define MAX_RGB_VALUE 255

#define TIME_BETWEEN_EACH_INCREMENTATION 1000
#define TIME_BETWEEN_EACH_DECREMENTATION 250

CRGB leds[NUM_LEDS];

int counter;
int rgb_value_between_each_led;
unsigned long currentMillis;
unsigned long lastMillis1;
unsigned long lastMillis2;

void setup() {
  // contrôle
  currentMillis = 0;
  lastMillis1 = 0;
  lastMillis2 = 0;

  rgb_value_between_each_led = ceil(MAX_RGB_VALUE/NUM_LEDS);
  if (rgb_value_between_each_led == 0)
    rgb_value_between_each_led = 1;

  // Init led gestion
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  turn_off();
  counter = 0;
  // Init Sérial
  Serial.begin(9600);
  while (!Serial);
}

void loop() {
  currentMillis = millis();

  if ((currentMillis - lastMillis1) >= TIME_BETWEEN_EACH_INCREMENTATION) {
    if (/*condition you want to increment the gauge &&*/ (counter < NUM_LEDS)) {
      counter++;
    }
    lastMillis1 = currentMillis;
  }
  if ((currentMillis - lastMillis2) >= TIME_BETWEEN_EACH_DECREMENTATION) {
    if (/*condition you want to decrement the gauge &&*/ (counter > 0)) {
      counter--;
    }
    lastMillis2 = currentMillis;
  }

  refresh(counter);
}

void refresh(int nb_led_max) {
  for (int r = MAX_RGB_VALUE, g = 0, nb_led = 0; nb_led < nb_led_max; nb_led++, r -= rgb_value_between_each_led, g += rgb_value_between_each_led)
    leds[nb_led] = CRGB(r, g, 0);
  for (int nb_led = nb_led_max; nb_led < NUM_LEDS; nb_led++)
    leds[nb_led] = CRGB(0, 0, 0);
  FastLED.show();
}

void turn_off() {
  for (int nb_led = 0; nb_led < NUM_LEDS; nb_led++)
    leds[nb_led] = CRGB(0, 0, 0);
}