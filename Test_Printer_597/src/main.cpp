#include <Arduino.h>
#include <Adafruit_Thermal.h>
#include <SoftwareSerial.h>

#define TX_PIN D4
#define RX_PIN D3
#define DEBUG_MODE false
#define NB_CHAR_MAX_PRINTED 32
#define ALLIGN_TO_LEFT true

SoftwareSerial mySerial(RX_PIN, TX_PIN);
Adafruit_Thermal printer(&mySerial);

String serial_input;

void setup() {
  Serial.begin(9600);
  mySerial.begin(19200);
  Serial.flush();
  delay(5000);
  mySerial.flush();

  if (DEBUG_MODE) {
    Serial.println("printer initialisation");
  }
  delay(1000);
  printer.begin();
  delay(1000);
  printer.flush();
  delay(1000);
  serial_input = "";
}

bool next_space(String text, int counter, int current_line) {
  for ( ; text.charAt(counter) != ' ' || counter <= text.length(); counter++, current_line++) {
    if (current_line > 32)
      return(false);
  }
  return (true);
}

void allign_left(String text) {
  for (int counter = 0, current_line = 0; counter <= text.length(); counter++, current_line++) {
    if (text.charAt(counter) == ' ') {
      if (!next_space(text, counter, current_line)) {
        counter++;
        printer.print('\n');
        current_line = 0;
      }
    }
    printer.print(text.charAt(counter));
  }
}

void loop() {
  serial_input = Serial.readString();
  if (serial_input != "") {
    if (ALLIGN_TO_LEFT) {
      allign_left(serial_input);
    } else {
      printer.println(serial_input);
    }
    if (DEBUG_MODE) {
      Serial.println(serial_input);
    }
    serial_input = "";
  }
}