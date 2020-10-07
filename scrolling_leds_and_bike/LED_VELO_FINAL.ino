#include "FastLED.h"
//arduino bike speedometer w serial.print()
//by Amanda Ghassaei 2012
//https://www.instructables.com/id/Arduino-Bike-Speedometer/
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
*/
//calculations
//tire radius ~ 13.5 inches
//circumference = pi*2*r =~85 inches
//max speed of 35mph =~ 616inches/second
//max rps =~7.25
#define reed 2//pin connected to read switch
//storage variables

#define NUM_LEDS 118
#define DATA_PIN 3
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
#define MILLISECOND_IN_HOUR 3600000.0
#define MAX_RGB_VALUE 255
#define LINE_SPACING 59.0
#define EMERGENT_PART_SINUS 0.4
#define NB_LEDS_IN_KM 60000.0

CRGB leds[NUM_LEDS];

int reedVal;
long timer;// time between one full rotation (in ms)
float mph;
float radius = 13.5;// tire radius (in inches)
float circumference;
int maxReedCounter = 100;//min time (in ms) of one rotation (for debouncing)
int reedCounter;

long last_millis;
long current_millis;
float kmh;
float pos;
float rgb_value;

void setup(){

  reedCounter = maxReedCounter;
  circumference = 2*3.14*radius;
  pinMode(reed, INPUT);

  // TIMER SETUP- the timer interrupt allows precise timed measurements of the reed switch
  //for more info about configuration of arduino timers see http://arduino.cc/playground/Code/Timer1
  cli();//stop interrupts
  //set timer1 interrupt at 1kHz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;
  // set timer count for 1khz increments
  OCR1A = 1999;// = (1/1000) / ((1/(16*10^6))*8) - 1
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS11 bit for 8 prescaler
  TCCR1B |= (1 << CS11);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  sei();//allow interrupts
  //END TIMER SETUP

  kmh = 0;
  last_millis = 0;
  current_millis = 0;
  rgb_value = 0;

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  for (int nb_led = 0; nb_led < NUM_LEDS; nb_led++)
    leds[nb_led] = CRGB(0, 0, 0);
  Serial.begin(9600);
  while (!Serial);
}

ISR(TIMER1_COMPA_vect) {//Interrupt at freq of 1kHz to measure reed switch
  reedVal = digitalRead(reed);//get val of A0
  if (reedVal){//if reed switch is closed
    if (reedCounter == 0){//min time between pulses has passed
      mph = (56.8*float(circumference))/float(timer);//calculate miles per hour
      timer = 0;//reset timer
      reedCounter = maxReedCounter;//reset reedCounter
    }
    else{
      if (reedCounter > 0){//don't let reedCounter go negative
        reedCounter -= 1;//decrement reedCounter
      }
    }
  }
  else{//if reed switch is open
    if (reedCounter > 0){//don't let reedCounter go negative
      reedCounter -= 1;//decrement reedCounter
    }
  }
  if (timer > 2000){
    mph = 0;//if no new pulses from reed switch- tire is still, set mph to 0
  }
  else{
    timer += 1;//increment timer
  }
}

void displayKMH(){
  Serial.println(mph*1.60924);
}

void loop(){
  //print KMH once a second
  displayKMH();
  kmh = 1.60924*mph;
  current_millis = millis();
  pos += kmh * ((current_millis - last_millis)*NB_LEDS_IN_KM/MILLISECOND_IN_HOUR);
  last_millis = current_millis;
  for (long i = 0; i < NUM_LEDS; i++) {
    rgb_value = sin((((i-pos) * TWO_PI) / LINE_SPACING) ) - EMERGENT_PART_SINUS;
    if (rgb_value <= 0)
      rgb_value = 0;
    if (rgb_value > 0)
      rgb_value = rgb_value*MAX_RGB_VALUE;
    leds[i] = CRGB(rgb_value, rgb_value, rgb_value);
  }
  FastLED.show();
}
